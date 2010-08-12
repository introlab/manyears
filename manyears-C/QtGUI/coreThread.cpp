#include "coreThread.h"

/***********************************************************
* Constructor                                              *
***********************************************************/

CoreThread::CoreThread()
{

}

/***********************************************************
* Destructor                                               *
***********************************************************/

CoreThread::~CoreThread()
{

}

/***********************************************************
* System parameters                                        *
***********************************************************/

void CoreThread::loadParameters(struct ParametersStruct _parameters)
{
    this->libraryParameters = _parameters;
}

/***********************************************************
* Input/Output manager                                     *
***********************************************************/

void CoreThread::setInputOuputManager(InputOutputManager* _manager)
{
    this->inputOutputManager = _manager;
}

/***********************************************************
* Thread control                                           *
***********************************************************/

// +-------------------------------------------------------+
// | Start                                                 |
// +-------------------------------------------------------+

void CoreThread::startThread()
{

    this->start();
    this->inputOutputManager->startThread();

}

// +-------------------------------------------------------+
// | Stop                                                  |
// +-------------------------------------------------------+

void CoreThread::stopThread()
{

    this->askToStopAndWait();

}

/***********************************************************
* Events receivers                                         *
***********************************************************/

// +-------------------------------------------------------+
// | Potential sources                                     |
// +-------------------------------------------------------+

void CoreThread::pushPotentialSourceEventReceiver(QObject* _eventReceiver)
{
    this->receiverPotentialSources.push_back(_eventReceiver);
}

// +-------------------------------------------------------+
// | Tracked sources                                       |
// +-------------------------------------------------------+

void CoreThread::pushTrackedSourceEventReceiver(QObject* _eventReceiver)
{
    this->receiverTrackedSources.push_back(_eventReceiver);
}

// +-------------------------------------------------------+
// | Run function                                          |
// +-------------------------------------------------------+

void CoreThread::run()
{

    float** buffer;

    unsigned int hopSize;
    unsigned int numberChannels;

    hopSize = this->inputOutputManager->getHopSize();
    numberChannels = this->inputOutputManager->getNumberChannels();

    // +---------------------------------------------------+
    // | Initialization                                    |
    // +---------------------------------------------------+

    this->libraryContext = createEmptyOverallContext();
    this->libraryContext.myParameters = new struct ParametersStruct;
    *(this->libraryContext.myParameters) = this->libraryParameters;

    beamformerInit(this->libraryContext.myBeamformer, this->libraryContext.myParameters);
    mixtureInit(this->libraryContext.myMixture, this->libraryContext.myParameters);
    potentialSourcesInit(this->libraryContext.myPotentialSources, this->libraryContext.myParameters);
    trackedSourcesInit(this->libraryContext.myTrackedSources, this->libraryContext.myParameters);

    // +---------------------------------------------------+
    // | Thread loop                                       |
    // +---------------------------------------------------+

    while(1)
    {

        // +-----------------------------------------------+
        // | Wait until a buffer is ready to be processed  |
        // +-----------------------------------------------+

        buffer = this->inputOutputManager->waitForBuffer();

        // Stop the loop if there is a NULL pointer,
        // which indicates acquisition has stopped
        if (buffer == NULL)
        {
            break;
        }

        // +-----------------------------------------------+
        // | Copy buffer to the library                    |
        // +-----------------------------------------------+

        for (int indexChannel = 0; indexChannel < numberChannels; indexChannel++)
        {

            beamformerPushFrames(this->libraryContext.myBeamformer, hopSize, indexChannel);
            beamformerAddFrame(this->libraryContext.myBeamformer, buffer[indexChannel], indexChannel, hopSize);

        }

        // +-----------------------------------------------+
        // | Return the buffer                             |
        // +-----------------------------------------------+

        this->inputOutputManager->giveBackBuffer(buffer);

        // +-----------------------------------------------+
        // | Find potential sources                        |
        // +-----------------------------------------------+

        beamformerFindMaxima(this->libraryContext.myBeamformer, this->libraryContext.myPotentialSources);

        // +-----------------------------------------------+
        // | Find tracked sources                          |
        // +-----------------------------------------------+

        mixtureUpdate(this->libraryContext.myMixture, this->libraryContext.myTrackedSources, this->libraryContext.myPotentialSources);

        // +-----------------------------------------------+
        // | Send potential sources found                  |
        // +-----------------------------------------------+

        PotentialSourceEvent tmpPotential;

        for (int indexSource = 0; indexSource < potentialSourcesGetNumberSources(this->libraryContext.myPotentialSources); indexSource++)
        {
            tmpPotential.pushSource(PotentialSource(potentialSourcesGetX(this->libraryContext.myPotentialSources, indexSource),
                                                    potentialSourcesGetY(this->libraryContext.myPotentialSources, indexSource),
                                                    potentialSourcesGetZ(this->libraryContext.myPotentialSources, indexSource),
                                                    potentialSourcesGetEnergy(this->libraryContext.myPotentialSources, indexSource),
                                                    indexSource));
        }
        for (int indexReceiver = 0; indexReceiver < this->receiverPotentialSources.size(); indexReceiver++)
        {
            QCoreApplication::postEvent(this->receiverPotentialSources[indexReceiver], new PotentialSourceEvent(tmpPotential));
        }

        // +-----------------------------------------------+
        // | Send tracked sources found                    |
        // +-----------------------------------------------+

        TrackedSourceEvent tmpTracked;

        for (int indexSource = 0; indexSource < trackedSourcesGetMaxNumberSources(this->libraryContext.myTrackedSources); indexSource++)
        {

            tmpTracked.pushSource(TrackedSource(trackedSourcesGetX(this->libraryContext.myTrackedSources, indexSource),
                                                trackedSourcesGetY(this->libraryContext.myTrackedSources, indexSource),
                                                trackedSourcesGetZ(this->libraryContext.myTrackedSources, indexSource),
                                                trackedSourcesGetID(this->libraryContext.myTrackedSources, indexSource)));
        }
        for (int indexReceiver = 0; indexReceiver < this->receiverTrackedSources.size(); indexReceiver++)
        {
            QCoreApplication::postEvent(this->receiverTrackedSources[indexReceiver], new TrackedSourceEvent(tmpTracked));
        }

    }



    // +---------------------------------------------------+
    // | Terminate                                         |
    // +---------------------------------------------------+

    beamformerTerminate(this->libraryContext.myBeamformer);
    mixtureTerminate(this->libraryContext.myMixture);
    potentialSourcesTerminate(this->libraryContext.myPotentialSources);
    trackedSourcesTerminate(this->libraryContext.myTrackedSources);

    deleteOverallContext(this->libraryContext);

    // +---------------------------------------------------+
    // | Clean the stream from any remaining buffers       |
    // +---------------------------------------------------+

    this->inputOutputManager->cleanStream();

    // +---------------------------------------------------+
    // | Flag that the thread has stopped                  |
    // +---------------------------------------------------+

    stopIsDone();

}

/***********************************************************
* Events                                                   *
***********************************************************/

bool CoreThread::event(QEvent *event)
{
    if (event->type() == QEvent::User)
    {

        // +-----------------------------------------------+
        // | Change the parameters                         |
        // +-----------------------------------------------+

        UpdateParametersEvent *updateParametersEvent = dynamic_cast<UpdateParametersEvent*>(event);

        if (updateParametersEvent != NULL)
        {

            bool wasRunning;

            wasRunning = this->isRunning();

            // Stop the thread
            stopThread();

            // Create a vector
            ConfigurationVector configurationVector;

            // Load values in vector
            configurationVector = updateParametersEvent->getConfigurationVector();

            // Load properties in the core

            // Beamformer
            this->libraryParameters.P_BF_MAXSOURCES = configurationVector.getElementInt("BEAMFORMER_MAXPOTENTIAL");
            this->libraryParameters.P_BF_NUMBERMAX = configurationVector.getElementInt("BEAMFORMER_NUMBERMAX");
            this->libraryParameters.P_BF_VALUETS = configurationVector.getElementFloat("BEAMFORMER_MAXTHRESHOLD");
            this->libraryParameters.P_BF_HANGLETOL = configurationVector.getElementFloat("BEAMFORMER_ANGLE_TOLERANCE_HORIZONTAL");
            this->libraryParameters.P_BF_HANGLETOLNEXT = configurationVector.getElementFloat("BEAMFORMER_ANGLE_TOLERANCE_HORIZONTAL_NEXT");
            this->libraryParameters.P_BF_VANGLETOL = configurationVector.getElementFloat("BEAMFORMER_ANGLE_TOLERANCE_VERTICAL");
            this->libraryParameters.P_BF_RANGE = configurationVector.getElementInt("BEAMFORMER_MAXRANGE");
            this->libraryParameters.P_BF_RIJDELAYS = configurationVector.getElementInt("BEAMFORMER_DELAYS_RESET");
            this->libraryParameters.P_BF_MICSGAIN_1 = configurationVector.getElementFloat("BEAMFORMER_MIC1_GAIN");
            this->libraryParameters.P_BF_MICSPOSITIONS_1_X = configurationVector.getElementFloat("BEAMFORMER_MIC1_X");
            this->libraryParameters.P_BF_MICSPOSITIONS_1_Y = configurationVector.getElementFloat("BEAMFORMER_MIC1_Y");
            this->libraryParameters.P_BF_MICSPOSITIONS_1_Z = configurationVector.getElementFloat("BEAMFORMER_MIC1_Z");
            this->libraryParameters.P_BF_MICSGAIN_2 = configurationVector.getElementFloat("BEAMFORMER_MIC2_GAIN");
            this->libraryParameters.P_BF_MICSPOSITIONS_2_X = configurationVector.getElementFloat("BEAMFORMER_MIC2_X");
            this->libraryParameters.P_BF_MICSPOSITIONS_2_Y = configurationVector.getElementFloat("BEAMFORMER_MIC2_Y");
            this->libraryParameters.P_BF_MICSPOSITIONS_2_Z = configurationVector.getElementFloat("BEAMFORMER_MIC2_Z");
            this->libraryParameters.P_BF_MICSGAIN_3 = configurationVector.getElementFloat("BEAMFORMER_MIC3_GAIN");
            this->libraryParameters.P_BF_MICSPOSITIONS_3_X = configurationVector.getElementFloat("BEAMFORMER_MIC3_X");
            this->libraryParameters.P_BF_MICSPOSITIONS_3_Y = configurationVector.getElementFloat("BEAMFORMER_MIC3_Y");
            this->libraryParameters.P_BF_MICSPOSITIONS_3_Z = configurationVector.getElementFloat("BEAMFORMER_MIC3_Z");
            this->libraryParameters.P_BF_MICSGAIN_4 = configurationVector.getElementFloat("BEAMFORMER_MIC4_GAIN");
            this->libraryParameters.P_BF_MICSPOSITIONS_4_X = configurationVector.getElementFloat("BEAMFORMER_MIC4_X");
            this->libraryParameters.P_BF_MICSPOSITIONS_4_Y = configurationVector.getElementFloat("BEAMFORMER_MIC4_Y");
            this->libraryParameters.P_BF_MICSPOSITIONS_4_Z = configurationVector.getElementFloat("BEAMFORMER_MIC4_Z");
            this->libraryParameters.P_BF_MICSGAIN_5 = configurationVector.getElementFloat("BEAMFORMER_MIC5_GAIN");
            this->libraryParameters.P_BF_MICSPOSITIONS_5_X = configurationVector.getElementFloat("BEAMFORMER_MIC5_X");
            this->libraryParameters.P_BF_MICSPOSITIONS_5_Y = configurationVector.getElementFloat("BEAMFORMER_MIC5_Y");
            this->libraryParameters.P_BF_MICSPOSITIONS_5_Z = configurationVector.getElementFloat("BEAMFORMER_MIC5_Z");
            this->libraryParameters.P_BF_MICSGAIN_6 = configurationVector.getElementFloat("BEAMFORMER_MIC6_GAIN");
            this->libraryParameters.P_BF_MICSPOSITIONS_6_X = configurationVector.getElementFloat("BEAMFORMER_MIC6_X");
            this->libraryParameters.P_BF_MICSPOSITIONS_6_Y = configurationVector.getElementFloat("BEAMFORMER_MIC6_Y");
            this->libraryParameters.P_BF_MICSPOSITIONS_6_Z = configurationVector.getElementFloat("BEAMFORMER_MIC6_Z");
            this->libraryParameters.P_BF_MICSGAIN_7 = configurationVector.getElementFloat("BEAMFORMER_MIC7_GAIN");
            this->libraryParameters.P_BF_MICSPOSITIONS_7_X = configurationVector.getElementFloat("BEAMFORMER_MIC7_X");
            this->libraryParameters.P_BF_MICSPOSITIONS_7_Y = configurationVector.getElementFloat("BEAMFORMER_MIC7_Y");
            this->libraryParameters.P_BF_MICSPOSITIONS_7_Z = configurationVector.getElementFloat("BEAMFORMER_MIC7_Z");
            this->libraryParameters.P_BF_MICSGAIN_8 = configurationVector.getElementFloat("BEAMFORMER_MIC8_GAIN");
            this->libraryParameters.P_BF_MICSPOSITIONS_8_X = configurationVector.getElementFloat("BEAMFORMER_MIC8_X");
            this->libraryParameters.P_BF_MICSPOSITIONS_8_Y = configurationVector.getElementFloat("BEAMFORMER_MIC8_Y");
            this->libraryParameters.P_BF_MICSPOSITIONS_8_Z = configurationVector.getElementFloat("BEAMFORMER_MIC8_Z");

            // Filter
            this->libraryParameters.P_FILTER_STDDEVIATION = configurationVector.getElementFloat("FILTER_STANDARDDEVIATION");
            this->libraryParameters.P_FILTER_ALPHASTOP = configurationVector.getElementFloat("FILTER_PREDICTION_STOP_ALPHA");
            this->libraryParameters.P_FILTER_BETASTOP = configurationVector.getElementFloat("FILTER_PREDICTION_STOP_BETA");
            this->libraryParameters.P_FILTER_ALPHACONST = configurationVector.getElementFloat("FILTER_PREDICTION_CONSTANT_ALPHA");
            this->libraryParameters.P_FILTER_BETACONST = configurationVector.getElementFloat("FILTER_PREDICTION_CONSTANT_BETA");
            this->libraryParameters.P_FILTER_ALPHAEXC = configurationVector.getElementFloat("FILTER_PREDICTION_ACCELERATED_ALPHA");
            this->libraryParameters.P_FILTER_BETAEXC = configurationVector.getElementFloat("FILTER_PREDICTION_ACCELERATED_BETA");
            this->libraryParameters.P_FILTER_INERTIAX = configurationVector.getElementFloat("FILTER_INERTIA_X");
            this->libraryParameters.P_FILTER_INERTIAY = configurationVector.getElementFloat("FILTER_INERTIA_Y");
            this->libraryParameters.P_FILTER_INERTIAZ = configurationVector.getElementFloat("FILTER_INERTIA_Z");
            this->libraryParameters.P_FILTER_DELTAT = configurationVector.getElementFloat("FILTER_DELTAT");
            this->libraryParameters.P_FILTER_STATEUPDT = configurationVector.getElementFloat("FILTER_STATEUPDATE");
            this->libraryParameters.P_FILTER_NEWSTOP = configurationVector.getElementFloat("FILTER_STOP_PERCENTAGE");
            this->libraryParameters.P_FILTER_NEWCONST = configurationVector.getElementFloat("FILTER_CONSTANT_PERCENTAGE");
            this->libraryParameters.P_FILTER_NEWEXC = configurationVector.getElementFloat("FILTER_ACCELERATED_PERCENTAGE");
            this->libraryParameters.P_FILTER_AJT_AJTM1 = configurationVector.getElementFloat("FILTER_ACTIVE_ACTIVE");
            this->libraryParameters.P_FILTER_AJT_NOTAJTM1 = configurationVector.getElementFloat("FILTER_INACTIVE_ACTIVE");
            this->libraryParameters.P_FILTER_P0 = configurationVector.getElementFloat("FILTER_P0");
            this->libraryParameters.P_FILTER_RSTHRESHOLD = configurationVector.getElementFloat("FILTER_RESAMPLING_THRESHOLD");
            this->libraryParameters.P_FILTER_BUFFERSIZE = configurationVector.getElementInt("FILTER_BUFFERSIZE");

            // Mixture
            this->libraryParameters.P_MIXTURE_NBFILTERS = configurationVector.getElementInt("MIXTURE_MAX_FILTERS");
            this->libraryParameters.P_MIXTURE_PNEW = configurationVector.getElementFloat("MIXTURE_PNEW");
            this->libraryParameters.P_MIXTURE_PFALSE = configurationVector.getElementFloat("MIXTURE_PFALSE");
            this->libraryParameters.P_MIXTURE_ET = configurationVector.getElementFloat("MIXTURE_ET");
            this->libraryParameters.P_MIXTURE_NEWTHRESHOLD = configurationVector.getElementFloat("MIXTURE_NEWSOURCE_THRESHOLD");
            this->libraryParameters.P_MIXTURE_CONFIRMEXISTS = configurationVector.getElementFloat("MIXTURE_CONFIRM_SOURCE_EXISTS");
            this->libraryParameters.P_MIXTURE_CONFIRMCOUNTTS = configurationVector.getElementFloat("MIXTURE_CONFIRM_COUNT_THRESHOLD");
            this->libraryParameters.P_MIXTURE_CONFIRMCOUNT = configurationVector.getElementInt("MIXTURE_CONFIRM_COUNT_COUNTER");
            this->libraryParameters.P_MIXTURE_NEWANGLE = configurationVector.getElementFloat("MIXTURE_NEWSOURCE_HORIZONTALANGLE");
            this->libraryParameters.P_MIXTURE_CUMULATIVETIMEPROB = configurationVector.getElementInt("MIXTURE_CUMULATIVE_TIME_PROBATION");
            this->libraryParameters.P_MIXTURE_TOBSPROB = configurationVector.getElementFloat("MIXTURE_NOTOBSERVED_PROBATION_THRESHOLD");
            this->libraryParameters.P_MIXTURE_CUMULATIVETIME1 = configurationVector.getElementInt("MIXTURE_CUMULATIVE_TIME_LEVEL1");
            this->libraryParameters.P_MIXTURE_TOBS1 = configurationVector.getElementFloat("MIXTURE_NOTOBSERVED_LEVEL1_THRESHOLD");
            this->libraryParameters.P_MIXTURE_CUMULATIVETIME2 = configurationVector.getElementInt("MIXTURE_CUMULATIVE_TIME_LEVEL2");
            this->libraryParameters.P_MIXTURE_TOBS2 = configurationVector.getElementFloat("MIXTURE_NOTOBSERVED_LEVEL2_THRESHOLD");

            // Microphone Sound Track
            this->libraryParameters.P_MICST_ALPHAD = configurationVector.getElementFloat("MICST_ALPHAD");
            this->libraryParameters.P_MICST_GAMMA = configurationVector.getElementFloat("MICST_GAMMA");
            this->libraryParameters.P_MICST_DELTA = configurationVector.getElementFloat("MICST_DELTA");
            this->libraryParameters.P_MCRA_ALPHAS = configurationVector.getElementFloat("MICST_MCRA_ALPHAS");
            this->libraryParameters.P_MCRA_ALPHAP = configurationVector.getElementFloat("MICST_MCRA_ALPHAP");
            this->libraryParameters.P_MCRA_ALPHAD = configurationVector.getElementFloat("MICST_MCRA_ALPHAD");
            this->libraryParameters.P_MCRA_L = configurationVector.getElementInt("MICST_MCRA_L");
            this->libraryParameters.P_MCRA_DELTA = configurationVector.getElementFloat("MICST_MCRA_DELTA");

            for (int indexReceiver = 0; indexReceiver < this->receiverPotentialSources.size(); indexReceiver++)
            {
                QCoreApplication::postEvent(this->receiverPotentialSources[indexReceiver], new GraphClearEvent());
            }
            for (int indexReceiver = 0; indexReceiver < this->receiverTrackedSources.size(); indexReceiver++)
            {
                QCoreApplication::postEvent(this->receiverTrackedSources[indexReceiver], new GraphClearEvent());
            }

            if (wasRunning == true)
            {
                startThread();
            }

        }

        return true;

    }

    return QThread::event(event);

}


/***********************************************************
* Thread access to shared fields                           *
***********************************************************/

// +-------------------------------------------------------+
// | Flags that the thread needs to stop                   |
// +-------------------------------------------------------+

void CoreThread::askToStopAndWait()
{

    // Tell the acquisition thread to stop
    this->inputOutputManager->stopThread();

    // Wait on the semaphore
    if (this->isRunning() == true)
    {
        this->semaStopDone.acquire();
    }

}

// +-------------------------------------------------------+
// | Stop has been performed                               |
// +-------------------------------------------------------+

void CoreThread::stopIsDone()
{

    // Release the semaphore
    this->semaStopDone.release();

}
