#include <QObject>
#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QGridLayout>
#include <QTimer>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QtOpenGL>
#include <QMainWindow>
#include <QTextEdit>
#include <QCoreApplication>
#include <QVBoxLayout>
#include <QEvent>
#include <QFileDialog>
#include <QSplashScreen>
#include <QTimer>
#include <math.h>

#include "plot.h"
#include "colorPalette.h"
#include "graph2D.h"
#include "mdiWindow.h"
#include "algorithmCore.h"
#include "sourceBuffer.h"
#include "soundInputConfig.h"
#include "aboutDialog.h"

#include "inputOutputManager.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "micSoundTrack.h"
#include "parameters.h"
#include "dynamicMemory.h"
#include "randGen.h"
#include "fft.h"
#include "filter.h"
#include "mixture.h"

#ifdef __cplusplus
}
#endif

//#define SHOW_DEBUG

//#define TEST_RTAUDIO

int myCallBack(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *manager)
{
    qDebug() << "Samples ready";

    return 0;
}

//int fillBuffer2(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *manager);


void ManyEarsMsgHandler(QtMsgType type, const char *msg);



class PrintEvent : public QEvent
{
public:
    PrintEvent(QString message)
        : QEvent(QEvent::User)
    {
        m_message = message;
    }

    QString m_message;
};

class ManyEarsApp :  public QApplication
{
	
	
public:

    ManyEarsApp(int argc, char**argv)
        : QApplication(argc, argv)
    {

        m_window = new QMainWindow(NULL);
        m_edit = new QTextEdit(m_window);
        m_window->setCentralWidget(m_edit);

        m_window->resize(1280,1024);
#ifdef SHOW_DEBUG
        m_window->show();
#endif
        m_window->raise();

        //Install handlers
        qInstallMsgHandler(ManyEarsMsgHandler);
    }


    void printDebug(const QString &message)
    {

        QString currentText =  m_edit->toPlainText() +  message;
        m_edit->setText(currentText.right(1000));
        //QTextCursor c =  m_edit->textCursor();
        //c.movePosition(QTextCursor::End);
        //m_edit->setTextCursor(c);

    }

    virtual bool event(QEvent* event)
    {
        if (event->type() == QEvent::User)
        {
            PrintEvent *pEvent = dynamic_cast<PrintEvent*>(event);
            if (pEvent)
            {
                m_edit->append(pEvent->m_message);
            }
            return true;
        }

        return QApplication::event(event);
    }


    ~ManyEarsApp()
    {

        //Restore message handler
        qInstallMsgHandler(0);


    }

        protected:

    QMainWindow *m_window;
    QTextEdit *m_edit;

};



void ManyEarsMsgHandler(QtMsgType type, const char *msg)
{
    ManyEarsApp *app = dynamic_cast<ManyEarsApp*>(QCoreApplication::instance());

    if (app)
    {

        switch (type) {
        case QtDebugMsg:
            QCoreApplication::postEvent(app,new PrintEvent(QString("DEBUG: ") + QString(msg)));
            break;
        case QtWarningMsg:
            QCoreApplication::postEvent(app,new PrintEvent(QString("WARNING: ") + QString(msg)));
            break;
        case QtCriticalMsg:
            QCoreApplication::postEvent(app,new PrintEvent(QString("CRITITAL: ") + QString(msg)));
            break;
        case QtFatalMsg:
            QCoreApplication::postEvent(app,new PrintEvent(QString("FATAL: ") + QString(msg)));
            abort();
        }
    }
}

//#define TEST_RTAUDIO

int main(int argc, char *argv[])
{

    ManyEarsApp a( argc, argv );

    MDIWindow* mainWindow;

    mainWindow = new MDIWindow();

    return a.exec();

}

