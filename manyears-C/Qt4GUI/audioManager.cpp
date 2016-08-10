#include "audioManager.h"
#include <QThread>
#include <QCoreApplication>

#ifdef __cplusplus
extern "C" {
#endif

#include "ManyEars/iodisk.h"

#ifdef __cplusplus
}
#endif

int bufferCounter = 0;
int bufferCounterGet = 0;

FILE *fp2 = NULL;

// ---
int counter = 0;
unsigned int bufferFrames = 512;
RtAudio::StreamParameters parameters;
RtAudio::StreamOptions options;
RtAudio tmpDac;
// ---

int myCallBack2(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *manager)
{
    qDebug() << "Callback entered";

    return 0;
}

AudioManager::AudioManager(int _numberChannels, int _frameSize, int _numberHopPerFrame, int _samplingRate, callBackFcn _callback, void *_parentObj, char _inputType)
{

    // Save parameters
    this->numberChannels = _numberChannels;
    this->frameSize = _frameSize;
    this->numberHopPerFrame = _numberHopPerFrame;
    this->samplingRate = _samplingRate;

    // Compute the hop size
    this->hopSize = this->frameSize / this->numberHopPerFrame;

    // Create buffers and initiates
    this->buffers = new float[this->numberChannels * this->numberHopPerFrame * this->hopSize];

    for (int indexBuffer = 0; indexBuffer < ((int) (this->numberChannels * this->numberHopPerFrame * this->hopSize)); indexBuffer++)
    {
        this->buffers[indexBuffer] = 0.0;
    }

    // Initially the first hop is to be filled
    this->indexHop = 0;

    // Save type of audio input
    this->inputType = _inputType;

    // Create audio driver
    if (this->inputType == AudioManager::TYPE_RECORD)
    {
        this->audioDriver = new RtAudio();
    }
    else if (this->inputType == AudioManager::TYPE_FILE)
    {
        this->fileTimer = new QTimer(this);
        QObject::connect(this->fileTimer, SIGNAL(timeout()), this, SLOT(processTimer()));
    }
    else
    {

    }

    // Save callback reference
    this->callback = _callback;
    this->parentObj = _parentObj;

}

AudioManager::AudioManager()
{

    // Create audio driver
    this->audioDriver = new RtAudio();

    this->inputType = AudioManager::TYPE_RECORD;

}

AudioManager::~AudioManager()
{

    closeStream();

    // Clear RtAudio object
    delete this->audioDriver;

    // Clear dynamic variales
    delete [] this->buffers;

}

unsigned int AudioManager::getAudioDevicesCount()
{

    return (this->audioDriver->getDeviceCount());

}

QString AudioManager::getAudioDeviceName(unsigned int _deviceIndex)
{

    RtAudio::DeviceInfo audioInfo;

    audioInfo = this->audioDriver->getDeviceInfo(_deviceIndex);

    return (audioInfo.name.c_str());

}

int AudioManager::getAudioDeviceIndex(QString _deviceName)
{

    int returnedIndex;

    returnedIndex = -1;

    for (unsigned int indexDevice = 0; indexDevice < this->getAudioDevicesCount(); indexDevice++)
    {
        if (this->getAudioDeviceName(indexDevice).compare(_deviceName) == 0)
        {
            returnedIndex = indexDevice;
            break;
        }
    }

    return returnedIndex;

}

void AudioManager::openStream(int _deviceIndex)
{

    unsigned int tmpHopSize;

    // Parameters for the stream
    RtAudio::StreamParameters audioParameters;

    // Define parameters of the audio
    audioParameters.deviceId = _deviceIndex;
    audioParameters.nChannels = this->numberChannels;
    audioParameters.firstChannel = 0;

    // Save hopsize to a variable outside the fields
    tmpHopSize = this->hopSize;

    // Open the stream
//    try {
//
//        qDebug() << "Sampling rate: " << this->samplingRate;
//        qDebug() << "Hop size: " << this->hopSize;
//        qDebug() << "Device index: " << _deviceIndex;
//        qDebug() << "Number channels: " << this->numberChannels;
//        //this->audioDriver->openStream( NULL, &audioParameters, RTAUDIO_SINT16, this->samplingRate, &tmpHopSize, &fillBuffer, this, NULL );
//        this->audioDriver->openStream( NULL, &audioParameters, RTAUDIO_SINT16, 48000, &tmpHopSize, &fillBuffer, this, NULL );
//
//    }

    parameters.deviceId = 0;
    parameters.nChannels = 8;
    unsigned int sampleRate = 48000;
    //unsigned int bufferFrames = 512;


    options.flags = RTAUDIO_NONINTERLEAVED;

    try {
      tmpDac.openStream( NULL, &parameters, RTAUDIO_SINT16,
                      sampleRate, &bufferFrames, &myCallBack2, NULL, &options );
    }

    catch ( RtError& e ) {

        qDebug(e.getMessage().c_str());
        //exit( 0 );

    }

//    tmpDac.startStream();

//    int a;
//
//    for (int tmp = 0; tmp < 100000000; tmp++)
//    {
//        a++;
//    }

}

void AudioManager::openStream(QString _filePath)
{

    short inputShort;

    int indexFile;

    this->filePath = _filePath;

    this->fileDriver = fopen(_filePath.toStdString().c_str(), "rb");

    indexFile = 0;

    while (!feof(this->fileDriver))
    {

        fread(&inputShort, sizeof(short), 1, this->fileDriver);
        indexFile++;

    }

    this->fileSize = indexFile;

    fclose(this->fileDriver);

    this->recordedStream = new signed short[this->fileSize];

    this->fileDriver = fopen(_filePath.toStdString().c_str(), "rb");

    indexFile = 0;

    while (!feof(this->fileDriver))
    {

        fread(&inputShort, sizeof(short), 1, this->fileDriver);
        this->recordedStream[indexFile] = inputShort;
        indexFile++;

    }

    fclose(this->fileDriver);

    this->sampleIndex = 0;

}

void AudioManager::startRecording()
{

    // Start the stream

    if (this->inputType == AudioManager::TYPE_RECORD)
    {

//        try {
        //bool tmp = tmpDac.isStreamOpen();
         unsigned int sampleRate = 48000;
//        tmpDac.openStream( NULL, &parameters, RTAUDIO_SINT16,
//                        sampleRate, &bufferFrames, &myCallBack2, NULL, &options );
            tmpDac.startStream();

//        }
//        catch ( RtError& e ) {
//
//            qDebug(e.getMessage().c_str());
//            //exit( 0 );
//
//        }

    }
    else if (this->inputType == AudioManager::TYPE_FILE)
    {

        this->fileTimer->setInterval(10*((float) this->hopSize)/((float) this->samplingRate));
        this->fileTimer->start();
    }
    else
    {

    }


}

void AudioManager::stopRecording()
{

    if (this->inputType == AudioManager::TYPE_RECORD)
    {

        // Stop the stream
        try
        {
            this->audioDriver->stopStream();

        }
        catch (RtError &e)
        {
            qDebug(e.getMessage().c_str());

            //exit(0);
        }

    }
    else if (this->inputType == AudioManager::TYPE_FILE)
    {
        this->fileTimer->stop();
    }
    else
    {

    }


}

void AudioManager::closeStream()
{

    if (this->inputType == AudioManager::TYPE_RECORD)
    {

        // Close the stream if opened
        try
        {

            if (this->audioDriver->isStreamOpen())
            {
                this->audioDriver->closeStream();
            }
        }
        catch (RtError &e)
        {
            qDebug(e.getMessage().c_str());

            //exit(0);
        }
    }
    else
    {
        delete [] this->recordedStream;
    }

}

void AudioManager::postEmptyFrame()
{

    float** newbuffers;

    newbuffers = this->newBuffers();

    for (int indexSample = 0; indexSample < this->hopSize; indexSample++)
    {
        for (int indexChannel = 0; indexChannel < this->numberChannels; indexChannel++)
        {

            newbuffers[indexChannel][indexSample] = 0.0;

        }

    }

    this->gotoCallBack();

}

int AudioManager::getNumberChannels()
{
    return (this->numberChannels);
}

float* AudioManager::getBuffers()
{   

    unsigned int indexStart;

    float** newbuffers;

    indexStart = this->getIndexHop() * this->getHopSize() * this->getNumberChannels();

    hopSize = this->getHopSize();
    numberChannels = this->getNumberChannels(); 

    this->stackIsUsed.lock();

    newbuffers = this->newBuffersStack.first();
    this->newBuffersStack.pop_front();

    this->stackIsUsed.unlock();

    //qDebug() << newbuffers[0][50];

    for (int indexChannel = 0; indexChannel < numberChannels; indexChannel++)
    {

        for (int indexSample = 0; indexSample < hopSize; indexSample++)
        {

            this->buffers[indexSample + hopSize * indexChannel] = newbuffers[indexChannel][indexSample];


        }

        delete [] newbuffers[indexChannel];

    }
    delete [] newbuffers;

    // Switch hop for next time
    this->updateIndexHop();

    return (this->buffers);
}

float** AudioManager::newBuffers()
{

    // Create new buffers

    float** newBuffers = new float*[this->numberChannels];

    for (int indexChannel = 0; indexChannel < this->numberChannels; indexChannel++)
    {
        newBuffers[indexChannel] = new float[this->hopSize];
    }

    this->stackIsUsed.lock();

    this->newBuffersStack.push_back(newBuffers);

    this->stackIsUsed.unlock();

    return newBuffers;

}

int AudioManager::getIndexHop()
{
    return (this->indexHop);
}

int AudioManager::getHopSize()
{
    return (this->hopSize);
}

int AudioManager::getNumberHopPerFrame()
{
    return (this->numberHopPerFrame);
}

void AudioManager::updateIndexHop()
{
    (this->indexHop)++;

    if ((this->indexHop) >= this->numberHopPerFrame)
    {
        this->indexHop = 0;
    }
}

void AudioManager::gotoCallBack()
{
    (*this->callback) (this->parentObj);
}

int AudioManager::getInputType() const
{
    return (this->inputType);
}

QString AudioManager::getFileName() const
{
    return (this->filePath);
}

void AudioManager::processTimer()
{ 

    float** newbuffers;


    if ((this->sampleIndex + this->numberChannels * this->hopSize) < this->fileSize)
    {

        newbuffers = this->newBuffers();

        for (int indexSample = 0; indexSample < this->hopSize; indexSample++)
        {
            for (int indexChannel = 0; indexChannel < this->numberChannels; indexChannel++)
            {

                newbuffers[indexChannel][indexSample] = ((float) this->recordedStream[this->sampleIndex]) / 32768.0;
                this->sampleIndex++;

            }

        }

    this->gotoCallBack();

    }

}

int fillBuffer(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *manager)
{
    //qDebug("CurrentThread (fillBuffer): %p mainAppThread %p",QThread::currentThread(),QCoreApplication::instance()->thread());

    AudioManager* managerPtr = reinterpret_cast<AudioManager*>(manager);

    int hopSize = managerPtr->getHopSize();
    int numberChannels = managerPtr->getNumberChannels();

    short *inputShortBuffer = (short*) (inputBuffer);

    float** newbuffers;

    newbuffers = managerPtr->newBuffers();

    fwrite(inputShortBuffer, sizeof(signed short), hopSize*numberChannels, fp2);

    for (int indexSample = 0; indexSample < hopSize; indexSample++)
    {
        for (int indexChannel = 0; indexChannel < numberChannels; indexChannel++)
        {

            newbuffers[indexChannel][indexSample] = ((float) (*inputShortBuffer)) / 32768.0;
            inputShortBuffer++;

        }

    }   

    // Call the callback
    managerPtr->gotoCallBack();

    return 0;
}
