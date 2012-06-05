#include "StreamOutputWidget.h"
#include <QDebug>
#include <QMutexLocker>
#include "sourceEvents.h"
#include <QAudioDeviceInfo>
#include <QDateTime>


StreamOutputWidget::StreamOutputWidget(QWidget *parent)
    : QWidget(parent), m_mutex(QMutex::Recursive), m_audioOutput(NULL), m_IODevice(NULL)

{
    m_outCounter = 0;
    m_inCounter = 0;


    //Create GUI components
    m_ui.setupUi(this);

    m_timer =new QTimer(this);

    //Connect signals
    connect(m_ui.m_startButton,SIGNAL(clicked()),this,SLOT(startButtonClicked()));
    connect(m_ui.m_stopButton,SIGNAL(clicked()),this,SLOT(stopButtonClicked()));
    connect(m_ui.m_clearButton,SIGNAL(clicked()),this,SLOT(clearButtonClicked()));
    connect(m_timer,SIGNAL(timeout()),this,SLOT(refreshTimeout()));

    //Scan audio output devices
    scanOutputDevice();

    m_timer->start(100); //100ms timer
}

StreamOutputWidget::~StreamOutputWidget()
{



}

void StreamOutputWidget::scanOutputDevice()
{
    //Get available output devices...
    QList<QAudioDeviceInfo>	myList = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);

    for (int i =0; i < myList.size(); i++)
    {
        m_ui.m_comboDeviceSelection->addItem(myList[i].deviceName());
        //qDebug() << " Adding item " << myList[i].deviceName();
    }
}

void StreamOutputWidget::notify()
{

    QDateTime time = QDateTime::currentDateTime();
    QString myString = time.toString("hh:mm:ss.zzz");
    //qDebug() << myString << " StreamOutputWidget::notify()";

    if (m_audioOutput && m_IODevice)
    {
        //qDebug("StreamOutputWidget::notify() bytesFree : %i interval: %i",m_audioOutput->bytesFree(),m_audioOutput->notifyInterval());
        while(m_frames.size() > 0)
        {
            //Try to output as many frames as possible according to buffer size
            if ((unsigned int) m_audioOutput->bytesFree() >  m_frames.front().size() * sizeof(short))
            {
                //qDebug("Writing frame");
                m_IODevice->write((char*) m_frames.front().data(),m_frames.front().size() * sizeof(short));
                popFrame();
            }
            else
            {
                //Not enough buffer size, waiting for next round ...
                break;
            }
        }
    }
}

void StreamOutputWidget::startButtonClicked()
{
    qDebug("void StreamOutputWidget::startButtonClicked()");

    //Get available output devices...
    QList<QAudioDeviceInfo>	myList = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);

    if (m_audioOutput)
    {
        delete m_audioOutput;
        m_audioOutput = NULL;
    }

    for (int i =0; i < myList.size(); i++)
    {
        if (myList[i].deviceName() == m_ui.m_comboDeviceSelection->currentText())
        {
            //QAudioFormat
            QAudioFormat format;

            //Set up the format
            format.setFrequency(48000);
            format.setChannels(1);
            format.setSampleSize(16);
            format.setCodec("audio/pcm");
            format.setByteOrder(QAudioFormat::LittleEndian);
            format.setSampleType(QAudioFormat::SignedInt);
            m_audioOutput = new QAudioOutput(myList[i],format,this);

            //We have 10ms frames, this should be enough
            m_audioOutput->setNotifyInterval(5);
        }
    }

    if (m_audioOutput)
    {
        connect(m_audioOutput,SIGNAL(stateChanged(QAudio::State)),this,SLOT(audioStateChanged(QAudio::State)));
        connect(m_audioOutput,SIGNAL(notify()),this,SLOT(notify()));
        m_audioOutput->setBufferSize(1000000);
        m_IODevice = m_audioOutput->start();
        m_ui.m_controlTextEdit->append("Starting stream... \n");
        m_ui.m_startButton->setEnabled(false);
        m_ui.m_stopButton->setEnabled(true);
    }
}

void StreamOutputWidget::stopButtonClicked()
{
    qDebug("void StreamOutputWidget::stopButtonClicked()");

    if (m_audioOutput)
    {
        m_audioOutput->stop();
        delete m_audioOutput;
        m_audioOutput = NULL;
        m_IODevice = NULL;
    }

    m_ui.m_controlTextEdit->append("Stopping stream... \n");
    m_ui.m_startButton->setEnabled(true);
    m_ui.m_stopButton->setEnabled(false);
}

void StreamOutputWidget::clearButtonClicked()
{
    QMutexLocker lock(&m_mutex);

    //Reset counters
    m_inCounter = 0;
    m_outCounter = 0;

    //Clear frames
    m_frames.clear();

    //Remove text
    m_ui.m_controlTextEdit->clear();
}

void StreamOutputWidget::refreshTimeout()
{
    //qDebug("StreamOutputWidget::refreshTimeout()");
    m_ui.m_controlLcdIn->display(m_inCounter);
    m_ui.m_controlLcdOut->display(m_outCounter);
}

bool StreamOutputWidget::event(QEvent* event)
{
    if (event->type() == QEvent::User)
    {
        SeparatedSourceEvent* sepEvent = dynamic_cast<SeparatedSourceEvent*>(event);

        if (sepEvent && m_audioOutput)
        {
            //qDebug("Writing to stack...");
            pushFrame(sepEvent->m_data);
            notify();
            return true;
        }

        return false;
    }

    return false;
}

QVector<short> StreamOutputWidget::popFrame()
{
    QMutexLocker lock(&m_mutex);

    if (m_frames.empty())
    {
        qDebug("StreamOutputWidget::popFrame() - Underrun...");
        //m_ui.m_controlTextEdit->append("Audio underrun...");
        return QVector<short>(512,0);
    }
    else
    {
        //m_ui.m_controlLcdOut->display(m_ui.m_controlLcdOut->value() + 1);
        //m_ui.m_controlLcdIn->display(m_ui.m_controlLcdIn->value() - 1);
        QVector<short> vec = m_frames.front();
        m_frames.pop_front();
        m_outCounter++;
        m_inCounter--;
        return vec;
    }

}

void StreamOutputWidget::pushFrame(const QVector<short> &frame)
{
    m_mutex.lock();
    m_frames.push_back(frame);
    m_inCounter++;
    m_mutex.unlock();
}

void StreamOutputWidget::audioStateChanged(QAudio::State state )
{
    qDebug(" StreamOutputWidget::audioStateChanged(QAudio::State state = %i)",state);
    switch(state)
    {
    case QAudio::ActiveState:
        //0	Audio data is being processed, this state is set after start() is called
        //and while audio data is available to be processed.
        break;

    case QAudio::SuspendedState:
        //1	The audio device is in a suspended state, this state will only be entered after suspend() is called.
        break;

    case QAudio::StoppedState:
        //2	The audio device is closed, not processing any audio data
        break;

    case QAudio::IdleState:
        //3	The QIODevice passed in has no data and audio system's buffer
        //is empty, this state is set after start() is called and while no audio data is available to be processed.
        m_ui.m_controlTextEdit->append("Audio underrun, nothing to play.\n");
        break;
    }

}
