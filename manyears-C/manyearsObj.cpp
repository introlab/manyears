#include "manyearsObj.h"

// ---
unsigned int frameCount = 0;
// ---

ManyEarsObject::ManyEarsObject(PotentialSourceManager *_potentialSourceManager, SourceManager *_sourceManager, Window *_windowPlot)
    : threadMutex(QMutex::Recursive)
{

    QMutexLocker locker(&threadMutex);

    this->recorder = new AudioManager(8,1024,2,48000,&updateCallBack,this);

    this->currentMixture = new struct objMixture;
    this->currentSources = new struct objSources;

    this->potentialSourceManager = _potentialSourceManager;
    this->sourceManager = _sourceManager;
    this->windowPlot = _windowPlot;

    this->running = false;

    // Connect signals from window to control the algoritm
    QObject::connect(this->windowPlot, SIGNAL(startCore()), this, SLOT(slotStart()));
    QObject::connect(this->windowPlot, SIGNAL(stopCore()), this, SLOT(slotStop()));
    QObject::connect(this, SIGNAL(confirmOperation()), this->windowPlot, SLOT(confirmReadyOperation()));

}

ManyEarsObject::~ManyEarsObject()
{

    QMutexLocker locker(&threadMutex);
    delete this->recorder;
    delete this->currentMixture;
    delete this->currentSources;

}

void ManyEarsObject::setRecordDevice(int _deviceIndex)
{
    QMutexLocker locker(&threadMutex);
    this->deviceIndex = _deviceIndex;
}

QString ManyEarsObject::getRecordDeviceName(int _deviceIndex)
{
    QMutexLocker locker(&threadMutex);
    return (this->recorder->getAudioDeviceName(_deviceIndex));
}

int ManyEarsObject::getRecordDeviceCount()
{
    QMutexLocker locker(&threadMutex);
    return (this->recorder->getAudioDevicesCount());
}

void ManyEarsObject::updateInputFrames()
{

    QMutexLocker locker(&threadMutex);
    int indexBuffer;
    int currentHopIndex;

    float *buffers;
    float *startFrame;

    buffers = this->recorder->getBuffers();

    currentHopIndex = this->recorder->getIndexHop();

    // Copy frames to the beamformer frames
    for (int indexChannel = 0; indexChannel < this->recorder->getNumberChannels(); indexChannel++)
    {    

        // Copy every hop
        for (int indexHop = 0; indexHop < this->recorder->getNumberHopPerFrame(); indexHop++)
        {

            indexBuffer = currentHopIndex * this->recorder->getHopSize() * this->recorder->getNumberChannels() + indexChannel * this->recorder->getHopSize();

            startFrame = buffers + indexBuffer;

            currentHopIndex++;

            if (currentHopIndex>=this->recorder->getNumberHopPerFrame())
            {
                currentHopIndex = 0;
            }

            // Copy

            beamformerLoadFrame(startFrame, ((char) indexChannel), (unsigned int) (indexHop * this->recorder->getHopSize()), (unsigned int) (indexHop * this->recorder->getHopSize() + this->recorder->getHopSize() - 1));

        }

    }  

}

void ManyEarsObject::findPotentialSources()
{
    QMutexLocker locker(&threadMutex);
    qDebug("Start finding beamformer maxima");

    // Find maxima
    //beamformerFindMaxima(this->potentialSourceManager->OqX, this->potentialSourceManager->OqY, this->potentialSourceManager->OqZ, this->potentialSourceManager->E0);

    // Load in potential sources
    //this->potentialSourceManager->updateFrame();

    //qDebug("Frame index: %d     E0: %f", frameCount++, this->potentialSourceManager->E0[0]);

}

void ManyEarsObject::applyFilter()
{
    QMutexLocker locker(&threadMutex);
    // Apply particle filter
    //mixtureUpdate(this->currentMixture, this->currentSources, this->potentialSourceManager->OqX, this->potentialSourceManager->OqY, this->potentialSourceManager->OqZ, this->potentialSourceManager->E0, 0.2);

}

void ManyEarsObject::loadSources()
{
    QMutexLocker locker(&threadMutex);
    // Load the sources in the source manager
    for (int indexSource = 0; indexSource < this->currentSources->maxNumberSources; indexSource++)
    {
        if (this->currentSources->sourcesID[indexSource] != -1)
        {

            this->sourceManager->addSource(this->currentSources->sourcesID[indexSource], this->currentSources->sourcesPosition[indexSource][0], this->currentSources->sourcesPosition[indexSource][1], this->currentSources->sourcesPosition[indexSource][2]);

        }
    }

    this->sourceManager->updateFrame();

    qDebug("askForupdateGraphs (ManyEarsObject) : %p",QThread::currentThread());

    // Ask for a refresh as soon as QT threads can be processed
    this->windowPlot->askForUpdateGraphs();

}

void  ManyEarsObject::start()
{
    QMutexLocker locker(&threadMutex);
    if (this->running == false)
    {

        // ManyEars: Initialize the beamformer
        beamformerInit();

        // ManyEars: Initialize the mixture
        mixtureInit(this->currentMixture);

        // ManyEars: Initialize the sources
        sourcesInit(this->currentSources);



        this->recorder->startRecording(this->deviceIndex);
        this->sourceManager->start();
        //this->potentialSourceManager->start(beamformerMaxPotentialSources());

        this->running = true;

    }

    // Flag this for the GUI
    emit(this->confirmOperation());

}

void  ManyEarsObject::stop()
{
    QMutexLocker locker(&threadMutex);

    if (this->running == true)
    {

        this->recorder->stopRecording();
        this->sourceManager->stop();
        //this->potentialSourceManager->stop();

        // ManyEars: Terminate the sources
        sourcesTerminate(this->currentSources);

        // ManyEars: Terminate the mixture
        mixtureTerminate(this->currentMixture);

        this->running = false;

    }

    // Flag this for the GUI
    emit(this->confirmOperation());

}


void ManyEarsObject::slotStart()
{
    QMutexLocker locker(&threadMutex);
    this->start();
}

void ManyEarsObject::slotStop()
{
    QMutexLocker locker(&threadMutex);
    this->stop();
}

void updateCallBack(void *objPtr)
{
    // Convert pointer
    ManyEarsObject* manyEarsPtr = reinterpret_cast<ManyEarsObject*>(objPtr);

    // Update frames
    manyEarsPtr->updateInputFrames();

    // Find maxima
    manyEarsPtr->findPotentialSources();

    // Apply particler filter
    manyEarsPtr->applyFilter();

    // Add sources
   //manyEarsPtr->loadSources();

}
