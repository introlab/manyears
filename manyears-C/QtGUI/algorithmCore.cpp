#include "algorithmCore.h"

//// XxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxX
//// x                                                       x
//// x Algorithm Core Sources Objects                        x
//// x                                                       x
//// XxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxX
//
//// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//// % Potential Source                                      %
//// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
///***********************************************************
//* Constructor                                              *
//***********************************************************/
//
//PotentialSource::PotentialSource(float _X, float _Y, float _Z, float _E, int _index)
//{
//
//    this->setX(_X);
//    this->setY(_Y);
//    this->setZ(_Z);
//    this->setE(_E);
//    this->setIndex(_index);
//
//}
//
//PotentialSource::PotentialSource(const PotentialSource& _potentialSource)
//{
//
//    this->setX(_potentialSource.getX());
//    this->setY(_potentialSource.getY());
//    this->setZ(_potentialSource.getZ());
//    this->setE(_potentialSource.getE());
//    this->setIndex(_potentialSource.getIndex());
//
//}
//
//PotentialSource::PotentialSource()
//{
//
//}
//
///***********************************************************
//* Destructor                                               *
//***********************************************************/
//
//PotentialSource::~PotentialSource()
//{
//
//}
//
///***********************************************************
//* Operators                                                *
//***********************************************************/
//
//// +-------------------------------------------------------+
//// | Equal                                                 |
//// +-------------------------------------------------------+
//
//PotentialSource& PotentialSource::operator=(const PotentialSource& _potentialSource)
//{
//
//    this->setX(_potentialSource.getX());
//    this->setY(_potentialSource.getY());
//    this->setZ(_potentialSource.getZ());
//    this->setE(_potentialSource.getE());
//    this->setIndex(_potentialSource.getIndex());
//
//    return (*this);
//
//}
//
///***********************************************************
//* Accessors                                                *
//***********************************************************/
//
//// +-------------------------------------------------------+
//// | X                                                     |
//// +-------------------------------------------------------+
//
//float PotentialSource::getX() const
//{
//    return this->X;
//}
//
//// +-------------------------------------------------------+
//// | Y                                                     |
//// +-------------------------------------------------------+
//
//float PotentialSource::getY() const
//{
//    return this->Y;
//}
//
//// +-------------------------------------------------------+
//// | Z                                                     |
//// +-------------------------------------------------------+
//
//float PotentialSource::getZ() const
//{
//    return this->Z;
//}
//
//// +-------------------------------------------------------+
//// | E                                                     |
//// +-------------------------------------------------------+
//
//float PotentialSource::getE() const
//{
//    return this->E;
//}
//
//// +-------------------------------------------------------+
//// | Index                                                 |
//// +-------------------------------------------------------+
//
//int PotentialSource::getIndex() const
//{
//    return this->index;
//}
//
///***********************************************************
//* Mutator                                                  *
//***********************************************************/
//
//// +-------------------------------------------------------+
//// | X                                                     |
//// +-------------------------------------------------------+
//
//void PotentialSource::setX(float _X)
//{
//    this->X = _X;
//}
//
//// +-------------------------------------------------------+
//// | Y                                                     |
//// +-------------------------------------------------------+
//
//void PotentialSource::setY(float _Y)
//{
//    this->Y = _Y;
//}
//
//// +-------------------------------------------------------+
//// | Z                                                     |
//// +-------------------------------------------------------+
//
//void PotentialSource::setZ(float _Z)
//{
//    this->Z = _Z;
//}
//
//// +-------------------------------------------------------+
//// | E                                                     |
//// +-------------------------------------------------------+
//
//void PotentialSource::setE(float _E)
//{
//    this->E = _E;
//}
//
//// +-------------------------------------------------------+
//// | Index                                                 |
//// +-------------------------------------------------------+
//
//void PotentialSource::setIndex(int _index)
//{
//    this->index = _index;
//}
//
//// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//// % Tracked Source                                            %
//// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
//
///***********************************************************
//* Constructor                                              *
//***********************************************************/
//
//TrackedSource::TrackedSource(float _X, float _Y, float _Z, int _ID)
//{
//
//    this->setX(_X);
//    this->setY(_Y);
//    this->setZ(_Z);
//    this->setID(_ID);
//
//}
//
//TrackedSource::TrackedSource(const TrackedSource& _trackedSource)
//{
//
//    this->setX(_trackedSource.getX());
//    this->setY(_trackedSource.getY());
//    this->setZ(_trackedSource.getZ());
//    this->setID(_trackedSource.getID());
//
//}
//
//TrackedSource::TrackedSource()
//{
//
//}
//
///***********************************************************
//* Destructor                                               *
//***********************************************************/
//
//TrackedSource::~TrackedSource()
//{
//
//}
//
///***********************************************************
//* Operators                                                *
//***********************************************************/
//
//// +-------------------------------------------------------+
//// | Equal                                                 |
//// +-------------------------------------------------------+
//
//TrackedSource& TrackedSource::operator=(const TrackedSource& _trackedSource)
//{
//
//    this->setX(_trackedSource.getX());
//    this->setY(_trackedSource.getY());
//    this->setZ(_trackedSource.getZ());
//    this->setID(_trackedSource.getID());
//
//    return (*this);
//
//}
//
///***********************************************************
//* Accessors                                                *
//***********************************************************/
//
//// +-------------------------------------------------------+
//// | X                                                     |
//// +-------------------------------------------------------+
//
//float TrackedSource::getX() const
//{
//    return this->X;
//}
//
//// +-------------------------------------------------------+
//// | Y                                                     |
//// +-------------------------------------------------------+
//
//float TrackedSource::getY() const
//{
//    return this->Y;
//}
//
//// +-------------------------------------------------------+
//// | Z                                                     |
//// +-------------------------------------------------------+
//
//float TrackedSource::getZ() const
//{
//    return this->Z;
//}
//
//// +-------------------------------------------------------+
//// | ID                                                    |
//// +-------------------------------------------------------+
//
//int TrackedSource::getID() const
//{
//    return this->ID;
//}
//
///***********************************************************
//* Mutator                                                  *
//***********************************************************/
//
//// +-------------------------------------------------------+
//// | X                                                     |
//// +-------------------------------------------------------+
//
//void TrackedSource::setX(float _X)
//{
//    this->X = _X;
//}
//
//// +-------------------------------------------------------+
//// | Y                                                     |
//// +-------------------------------------------------------+
//
//void TrackedSource::setY(float _Y)
//{
//    this->Y = _Y;
//}
//
//// +-------------------------------------------------------+
//// | Z                                                     |
//// +-------------------------------------------------------+
//
//void TrackedSource::setZ(float _Z)
//{
//    this->Z = _Z;
//}
//
//// +-------------------------------------------------------+
//// | ID                                                    |
//// +-------------------------------------------------------+
//
//void TrackedSource::setID(int _ID)
//{
//    this->ID = _ID;
//}
//
//// XxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxX
//// x                                                       x
//// x Algorithm Core Sources Events                         x
//// x                                                       x
//// XxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxX
//
//// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//// % Potential source                                      %
//// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
///***********************************************************
//* Constructor                                              *
//***********************************************************/
//
//PotentialSourceEvent::PotentialSourceEvent(QList<PotentialSource> _list) : QEvent(QEvent::User)
//{
//
//    this->list = _list;
//
//}
//
//PotentialSourceEvent::PotentialSourceEvent() : QEvent(QEvent::User)
//{
//
//}
//
//PotentialSourceEvent::PotentialSourceEvent(const PotentialSourceEvent &e): QEvent(QEvent::User)
//{
//
//    (*this) = e;
//
//}
//
///***********************************************************
//* Destructor                                               *
//***********************************************************/
//
//PotentialSourceEvent::~PotentialSourceEvent()
//{
//
//}
//
//
///***********************************************************
//* Operators                                                *
//***********************************************************/
//
//// +-------------------------------------------------------+
//// | Equal                                                 |
//// +-------------------------------------------------------+
//
//PotentialSourceEvent& PotentialSourceEvent::operator=(const PotentialSourceEvent& _potentialSourceEvent)
//{
//
//    this->list.clear();
//
//    for (int indexSource = 0; indexSource < _potentialSourceEvent.getListSize(); indexSource++)
//    {
//
//        this->list.push_back(_potentialSourceEvent.getPotentialSource(indexSource));
//
//    }
//
//    return *this;
//
//}
//
///***********************************************************
//* Accessors                                                *
//***********************************************************/
//
//// +-------------------------------------------------------+
//// | PotentialSource                                       |
//// +-------------------------------------------------------+
//
//PotentialSource PotentialSourceEvent::getPotentialSource(const int indexSource) const
//{
//
//    return (this->list[indexSource]);
//
//}
//
//// +-------------------------------------------------------+
//// | List                                                  |
//// +-------------------------------------------------------+
//
//QList<PotentialSource> PotentialSourceEvent::getList() const
//{
//
//    return (this->list);
//
//}
//
//
//// +-------------------------------------------------------+
//// | Size                                                  |
//// +-------------------------------------------------------+
//
//int PotentialSourceEvent::getListSize() const
//{
//
//    return (this->list.size());
//
//}
//
///***********************************************************
//* Mutators                                                 *
//***********************************************************/
//
//// +-------------------------------------------------------+
//// | PotentialSource                                       |
//// +-------------------------------------------------------+
//
//void PotentialSourceEvent::pushSource(PotentialSource _source)
//{
//
//    this->list.push_back(_source);
//
//}
//
//
//// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//// % Tracked source                                        %
//// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
///***********************************************************
//* Constructor                                              *
//***********************************************************/
//
//TrackedSourceEvent::TrackedSourceEvent(QList<TrackedSource> _list) : QEvent(QEvent::User)
//{
//
//    this->list = _list;
//
//}
//
//TrackedSourceEvent::TrackedSourceEvent() : QEvent(QEvent::User)
//{
//
//}
//
//TrackedSourceEvent::TrackedSourceEvent(const TrackedSourceEvent &e): QEvent(QEvent::User)
//{
//
//    (*this) = e;
//
//}
//
///***********************************************************
//* Destructor                                               *
//***********************************************************/
//
//TrackedSourceEvent::~TrackedSourceEvent()
//{
//
//}
//
//
///***********************************************************
//* Operators                                                *
//***********************************************************/
//
//// +-------------------------------------------------------+
//// | Equal                                                 |
//// +-------------------------------------------------------+
//
//TrackedSourceEvent& TrackedSourceEvent::operator=(const TrackedSourceEvent& _trackedSourceEvent)
//{
//
//    this->list.clear();
//
//    for (int indexSource = 0; indexSource < _trackedSourceEvent.getListSize(); indexSource++)
//    {
//
//        this->list.push_back(_trackedSourceEvent.getTrackedSource(indexSource));
//
//    }
//
//    return *this;
//
//}
//
///***********************************************************
//* Accessors                                                *
//***********************************************************/
//
//// +-------------------------------------------------------+
//// | PotentialSource                                       |
//// +-------------------------------------------------------+
//
//TrackedSource TrackedSourceEvent::getTrackedSource(const int indexSource) const
//{
//
//    return (this->list[indexSource]);
//
//}
//
//// +-------------------------------------------------------+
//// | Size                                                  |
//// +-------------------------------------------------------+
//
//int TrackedSourceEvent::getListSize() const
//{
//
//    return (this->list.size());
//
//}
//
///***********************************************************
//* Mutators                                                 *
//***********************************************************/
//
//// +-------------------------------------------------------+
//// | PotentialSource                                       |
//// +-------------------------------------------------------+
//
//void TrackedSourceEvent::pushSource(TrackedSource _source)
//{
//
//    this->list.push_back(_source);
//
//}

// XxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxX
// x                                                       x
// x Algorithm Core                                        x
// x                                                       x
// XxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxX

/***********************************************************
* Constructor                                              *
***********************************************************/

AlgorithmCore::AlgorithmCore(const int _audioDeviceID)
{

    // Create an audio manager
    this->audioManager = new AudioManager(8, 1024, 2, 48000, &bufferIsFull, this, AudioManager::TYPE_RECORD);

    this->audioDeviceID = _audioDeviceID;

    this->audioManager->openStream(this->audioDeviceID);

    // Initialize flags
    this->flagAskToStop = false;
    this->flagIsRunning = false;

}

AlgorithmCore::AlgorithmCore(const QString _filePath)
{

    this->audioManager = new AudioManager(8, 1024, 2, 48000, &bufferIsFull, this, AudioManager::TYPE_FILE);

    this->audioDeviceID = -2;
    this->fileName = _filePath;

    this->audioManager->openStream(_filePath);

    // Initialize flags
    this->flagAskToStop = false;
    this->flagIsRunning = false;

}

/***********************************************************
* Destructor                                               *
***********************************************************/

AlgorithmCore::~AlgorithmCore()
{

    delete [] this->potentialX;
    delete [] this->potentialY;
    delete [] this->potentialZ;
    delete [] this->potentialE;
    delete [] this->trackedX;
    delete [] this->trackedY;
    delete [] this->trackedZ;
    delete [] this->trackedID;

    this->audioManager->closeStream();
    delete this->audioManager;

}

/***********************************************************
* Thread control                                           *
***********************************************************/

// +-------------------------------------------------------+
// | Run function                                          |
// +-------------------------------------------------------+

void AlgorithmCore::run()
{   

    // Initialize the system
    this->initSystem();

    while(this->isRunning())
    {

        // Wait for a frame to be ready to be processed
        this->waitFrame();

        // Update frames
        this->loadFrame();

        // Perform beamformer computation
        this->findPotentialSources();

        // Output potential sources (and generate events)
        this->outputPotentialSources();

        // Perform mixture of particle filters computation
        this->findTrackedSources();

        // Output tracked sources (and generate events)
        this->outputTrackedSources();

    }

    // Terminate the system
    this->terminateSystem();

    // Confirm the system stopped if needed
    this->confirmStop.release();

}

// +-------------------------------------------------------+
// | Start the thread                                      |
// +-------------------------------------------------------+

void AlgorithmCore::startThread(ParametersManager _parametersManager)
{

    // Set the flag
    this->setRunning(true);

    // Save the parameters
    this->currentParameters = _parametersManager;

    // Start the thread
    this->start();

    // Start audio recorder
    this->audioManager->startRecording();

}

// +-------------------------------------------------------+
// | Stop the thread                                       |
// +-------------------------------------------------------+

void AlgorithmCore::stopThread()
{

    if (this->isRunning())
    {

        // Set the flag
        this->setRunning(false);

        // Stop audio recorder
        this->audioManager->stopRecording();

        // Post one last frame
        this->audioManager->postEmptyFrame();

        // Wait for the confirmation
        this->confirmStop.acquire();

    }

}

// +-------------------------------------------------------+
// | Return a boolean to tell if the thread is running     |
// +-------------------------------------------------------+

bool AlgorithmCore::isRunning()
{

    bool tmp;

    this->criticalSection.lock();
    tmp = this->flagIsRunning;
    this->criticalSection.unlock();

    return tmp;

}

// +-------------------------------------------------------+
// | Set a boolean to tell if the thread is running        |
// +-------------------------------------------------------+

void AlgorithmCore::setRunning(bool _isRunning)
{

    this->criticalSection.lock();
    this->flagIsRunning = _isRunning;
    this->criticalSection.unlock();

}

// +-------------------------------------------------------+
// | Wait for a frame                                      |
// +-------------------------------------------------------+

void AlgorithmCore::waitFrame()
{

    this->frameIsReady.acquire();

}

// +-------------------------------------------------------+
// | Post a new frame                                      |
// +-------------------------------------------------------+

void AlgorithmCore::postFrame()
{

    this->frameIsReady.release();

}

// +-------------------------------------------------------+
// | Add an event receiver                                 |
// +-------------------------------------------------------+

void AlgorithmCore::registerObjectPotential(QObject* _object)
{

    this->objectReceiverPotentialList.push_back(_object);

}

void AlgorithmCore::registerObjectTracked(QObject* _object)
{

    this->objectReceiverTrackedList.push_back(_object);

}

void AlgorithmCore::registerObjectEnergy(QObject* _object)
{
    this->objectReceiverEnergyList.push_back(_object);
}


void AlgorithmCore::registerObjectConfirmStop(QObject* _object)
{

    this->objectReceiverConfirmStopList.push_back(_object);

}

void AlgorithmCore::registerObjectConfirmStopClear()
{

    this->objectReceiverConfirmStopList.clear();

}


/***********************************************************
* Accessor                                                 *
***********************************************************/

QString AlgorithmCore::getFileName() const
{
    return (this->fileName);
}

/***********************************************************
* Events                                                   *
***********************************************************/

// +-------------------------------------------------------+
// | General                                               |
// +-------------------------------------------------------+

bool AlgorithmCore::event(QEvent* event)
{

    if (event->type() == QEvent::User)
    {

        // +-----------------------------------------------+
        // | Change the parameters                         |
        // +-----------------------------------------------+

        UpdateParametersEvent *updateParametersEvent = dynamic_cast<UpdateParametersEvent*>(event);

        if (updateParametersEvent != NULL)
        {

            // Save the event
            this->pastEvent = new UpdateParametersEvent(*updateParametersEvent);

            // Save the state of the thread
            this->needsRestart = this->isRunning();

            // Clear the list of receivers
            registerObjectConfirmStopClear();

            // Add itself
            registerObjectConfirmStop(this);

            // Check if the thread is running
            if (this->isRunning())
            {
                // Stop the thread
                this->stopThread();
            }

            // Create a vector
            ConfigurationVector configurationVector;

            // Load values in vector
            configurationVector = this->pastEvent->getConfigurationVector();

            // Delete the past event
            delete this->pastEvent;
            this->pastEvent = NULL;

            // Load properties in the core

            // Beamformer
            this->currentParameters.setPropertySystemInt("BEAMFORMER_MAXPOTENTIAL", configurationVector.getElementInt("BEAMFORMER_MAXPOTENTIAL"));            
            this->currentParameters.setPropertySystemInt("BEAMFORMER_NUMBERMAX", configurationVector.getElementInt("BEAMFORMER_NUMBERMAX"));            
            this->currentParameters.setPropertySystemFloat("BEAMFORMER_MAXTHRESHOLD", configurationVector.getElementFloat("BEAMFORMER_MAXTHRESHOLD"));            
            this->currentParameters.setPropertySystemFloat("BEAMFORMER_ANGLE_TOLERANCE_HORIZONTAL", configurationVector.getElementFloat("BEAMFORMER_ANGLE_TOLERANCE_HORIZONTAL"));            
            this->currentParameters.setPropertySystemFloat("BEAMFORMER_ANGLE_TOLERANCE_HORIZONTAL_NEXT", configurationVector.getElementFloat("BEAMFORMER_ANGLE_TOLERANCE_HORIZONTAL_NEXT"));
            this->currentParameters.setPropertySystemFloat("BEAMFORMER_ANGLE_TOLERANCE_VERTICAL", configurationVector.getElementFloat("BEAMFORMER_ANGLE_TOLERANCE_VERTICAL"));
            this->currentParameters.setPropertySystemInt("BEAMFORMER_MAXRANGE", configurationVector.getElementInt("BEAMFORMER_MAXRANGE"));
            this->currentParameters.setPropertySystemInt("BEAMFORMER_DELAYS_RESET", configurationVector.getElementInt("BEAMFORMER_DELAYS_RESET"));            
            this->currentParameters.setPropertySystemFloat("BEAMFORMER_MIC1_GAIN", configurationVector.getElementFloat("BEAMFORMER_MIC1_GAIN"));            
            this->currentParameters.setPropertySystemFloat("BEAMFORMER_MIC1_X", configurationVector.getElementFloat("BEAMFORMER_MIC1_X"));            
            this->currentParameters.setPropertySystemFloat("BEAMFORMER_MIC1_Y", configurationVector.getElementFloat("BEAMFORMER_MIC1_Y"));            
            this->currentParameters.setPropertySystemFloat("BEAMFORMER_MIC1_Z", configurationVector.getElementFloat("BEAMFORMER_MIC1_Z"));            
            this->currentParameters.setPropertySystemFloat("BEAMFORMER_MIC2_GAIN", configurationVector.getElementFloat("BEAMFORMER_MIC2_GAIN"));            
            this->currentParameters.setPropertySystemFloat("BEAMFORMER_MIC2_X", configurationVector.getElementFloat("BEAMFORMER_MIC2_X"));            
            this->currentParameters.setPropertySystemFloat("BEAMFORMER_MIC2_Y", configurationVector.getElementFloat("BEAMFORMER_MIC2_Y"));            
            this->currentParameters.setPropertySystemFloat("BEAMFORMER_MIC2_Z", configurationVector.getElementFloat("BEAMFORMER_MIC2_Z"));            
            this->currentParameters.setPropertySystemFloat("BEAMFORMER_MIC3_GAIN", configurationVector.getElementFloat("BEAMFORMER_MIC3_GAIN"));            
            this->currentParameters.setPropertySystemFloat("BEAMFORMER_MIC3_X", configurationVector.getElementFloat("BEAMFORMER_MIC3_X"));            
            this->currentParameters.setPropertySystemFloat("BEAMFORMER_MIC3_Y", configurationVector.getElementFloat("BEAMFORMER_MIC3_Y"));            
            this->currentParameters.setPropertySystemFloat("BEAMFORMER_MIC3_Z", configurationVector.getElementFloat("BEAMFORMER_MIC3_Z"));            
            this->currentParameters.setPropertySystemFloat("BEAMFORMER_MIC4_GAIN", configurationVector.getElementFloat("BEAMFORMER_MIC4_GAIN"));            
            this->currentParameters.setPropertySystemFloat("BEAMFORMER_MIC4_X", configurationVector.getElementFloat("BEAMFORMER_MIC4_X"));            
            this->currentParameters.setPropertySystemFloat("BEAMFORMER_MIC4_Y", configurationVector.getElementFloat("BEAMFORMER_MIC4_Y"));            
            this->currentParameters.setPropertySystemFloat("BEAMFORMER_MIC4_Z", configurationVector.getElementFloat("BEAMFORMER_MIC4_Z"));            
            this->currentParameters.setPropertySystemFloat("BEAMFORMER_MIC5_GAIN", configurationVector.getElementFloat("BEAMFORMER_MIC5_GAIN"));            
            this->currentParameters.setPropertySystemFloat("BEAMFORMER_MIC5_X", configurationVector.getElementFloat("BEAMFORMER_MIC5_X"));            
            this->currentParameters.setPropertySystemFloat("BEAMFORMER_MIC5_Y", configurationVector.getElementFloat("BEAMFORMER_MIC5_Y"));            
            this->currentParameters.setPropertySystemFloat("BEAMFORMER_MIC5_Z", configurationVector.getElementFloat("BEAMFORMER_MIC5_Z"));            
            this->currentParameters.setPropertySystemFloat("BEAMFORMER_MIC6_GAIN", configurationVector.getElementFloat("BEAMFORMER_MIC6_GAIN"));            
            this->currentParameters.setPropertySystemFloat("BEAMFORMER_MIC6_X", configurationVector.getElementFloat("BEAMFORMER_MIC6_X"));            
            this->currentParameters.setPropertySystemFloat("BEAMFORMER_MIC6_Y", configurationVector.getElementFloat("BEAMFORMER_MIC6_Y"));            
            this->currentParameters.setPropertySystemFloat("BEAMFORMER_MIC6_Z", configurationVector.getElementFloat("BEAMFORMER_MIC6_Z"));            
            this->currentParameters.setPropertySystemFloat("BEAMFORMER_MIC7_GAIN", configurationVector.getElementFloat("BEAMFORMER_MIC7_GAIN"));            
            this->currentParameters.setPropertySystemFloat("BEAMFORMER_MIC7_X", configurationVector.getElementFloat("BEAMFORMER_MIC7_X"));            
            this->currentParameters.setPropertySystemFloat("BEAMFORMER_MIC7_Y", configurationVector.getElementFloat("BEAMFORMER_MIC7_Y"));            
            this->currentParameters.setPropertySystemFloat("BEAMFORMER_MIC7_Z", configurationVector.getElementFloat("BEAMFORMER_MIC7_Z"));            
            this->currentParameters.setPropertySystemFloat("BEAMFORMER_MIC8_GAIN", configurationVector.getElementFloat("BEAMFORMER_MIC8_GAIN"));            
            this->currentParameters.setPropertySystemFloat("BEAMFORMER_MIC8_X", configurationVector.getElementFloat("BEAMFORMER_MIC8_X"));
            this->currentParameters.setPropertySystemFloat("BEAMFORMER_MIC8_Y", configurationVector.getElementFloat("BEAMFORMER_MIC8_Y"));
            this->currentParameters.setPropertySystemFloat("BEAMFORMER_MIC8_Z", configurationVector.getElementFloat("BEAMFORMER_MIC8_Z"));

            // Filter
            this->currentParameters.setPropertySystemFloat("FILTER_STANDARDDEVIATION", configurationVector.getElementFloat("FILTER_STANDARDDEVIATION"));
            this->currentParameters.setPropertySystemFloat("FILTER_PREDICTION_STOP_ALPHA", configurationVector.getElementFloat("FILTER_PREDICTION_STOP_ALPHA"));
            this->currentParameters.setPropertySystemFloat("FILTER_PREDICTION_STOP_BETA", configurationVector.getElementFloat("FILTER_PREDICTION_STOP_BETA"));
            this->currentParameters.setPropertySystemFloat("FILTER_PREDICTION_CONSTANT_ALPHA", configurationVector.getElementFloat("FILTER_PREDICTION_CONSTANT_ALPHA"));
            this->currentParameters.setPropertySystemFloat("FILTER_PREDICTION_CONSTANT_BETA", configurationVector.getElementFloat("FILTER_PREDICTION_CONSTANT_BETA"));
            this->currentParameters.setPropertySystemFloat("FILTER_PREDICTION_ACCELERATED_ALPHA", configurationVector.getElementFloat("FILTER_PREDICTION_ACCELERATED_ALPHA"));
            this->currentParameters.setPropertySystemFloat("FILTER_PREDICTION_ACCELERATED_BETA", configurationVector.getElementFloat("FILTER_PREDICTION_ACCELERATED_BETA"));
            this->currentParameters.setPropertySystemFloat("FILTER_INERTIA_X", configurationVector.getElementFloat("FILTER_INERTIA_X"));
            this->currentParameters.setPropertySystemFloat("FILTER_INERTIA_Y", configurationVector.getElementFloat("FILTER_INERTIA_Y"));
            this->currentParameters.setPropertySystemFloat("FILTER_INERTIA_Z", configurationVector.getElementFloat("FILTER_INERTIA_Z"));
            this->currentParameters.setPropertySystemFloat("FILTER_DELTAT", configurationVector.getElementFloat("FILTER_DELTAT"));
            this->currentParameters.setPropertySystemFloat("FILTER_STATEUPDATE", configurationVector.getElementFloat("FILTER_STATEUPDATE"));
            this->currentParameters.setPropertySystemFloat("FILTER_STOP_PERCENTAGE", configurationVector.getElementFloat("FILTER_STOP_PERCENTAGE"));
            this->currentParameters.setPropertySystemFloat("FILTER_CONSTANT_PERCENTAGE", configurationVector.getElementFloat("FILTER_CONSTANT_PERCENTAGE"));
            this->currentParameters.setPropertySystemFloat("FILTER_ACCELERATED_PERCENTAGE", configurationVector.getElementFloat("FILTER_ACCELERATED_PERCENTAGE"));
            this->currentParameters.setPropertySystemFloat("FILTER_ACTIVE_ACTIVE", configurationVector.getElementFloat("FILTER_ACTIVE_ACTIVE"));
            this->currentParameters.setPropertySystemFloat("FILTER_INACTIVE_ACTIVE", configurationVector.getElementFloat("FILTER_INACTIVE_ACTIVE"));
            this->currentParameters.setPropertySystemFloat("FILTER_P0", configurationVector.getElementFloat("FILTER_P0"));
            this->currentParameters.setPropertySystemFloat("FILTER_RESAMPLING_THRESHOLD", configurationVector.getElementFloat("FILTER_RESAMPLING_THRESHOLD"));
            this->currentParameters.setPropertySystemInt("FILTER_BUFFERSIZE", configurationVector.getElementInt("FILTER_BUFFERSIZE"));

            // Mixture
            this->currentParameters.setPropertySystemInt("MIXTURE_MAX_FILTERS", configurationVector.getElementInt("MIXTURE_MAX_FILTERS"));
            this->currentParameters.setPropertySystemFloat("MIXTURE_PNEW", configurationVector.getElementFloat("MIXTURE_PNEW"));
            this->currentParameters.setPropertySystemFloat("MIXTURE_PFALSE", configurationVector.getElementFloat("MIXTURE_PFALSE"));
            this->currentParameters.setPropertySystemFloat("MIXTURE_ET", configurationVector.getElementFloat("MIXTURE_ET"));
            this->currentParameters.setPropertySystemFloat("MIXTURE_NEWSOURCE_THRESHOLD", configurationVector.getElementFloat("MIXTURE_NEWSOURCE_THRESHOLD"));
            this->currentParameters.setPropertySystemFloat("MIXTURE_CONFIRM_SOURCE_EXISTS", configurationVector.getElementFloat("MIXTURE_CONFIRM_SOURCE_EXISTS"));
            this->currentParameters.setPropertySystemFloat("MIXTURE_CONFIRM_COUNT_THRESHOLD", configurationVector.getElementFloat("MIXTURE_CONFIRM_COUNT_THRESHOLD"));
            this->currentParameters.setPropertySystemInt("MIXTURE_CONFIRM_COUNT_COUNTER", configurationVector.getElementInt("MIXTURE_CONFIRM_COUNT_COUNTER"));
            this->currentParameters.setPropertySystemFloat("MIXTURE_NEWSOURCE_HORIZONTALANGLE", configurationVector.getElementFloat("MIXTURE_NEWSOURCE_HORIZONTALANGLE"));
            this->currentParameters.setPropertySystemInt("MIXTURE_CUMULATIVE_TIME_PROBATION", configurationVector.getElementInt("MIXTURE_CUMULATIVE_TIME_PROBATION"));
            this->currentParameters.setPropertySystemFloat("MIXTURE_NOTOBSERVED_PROBATION_THRESHOLD", configurationVector.getElementFloat("MIXTURE_NOTOBSERVED_PROBATION_THRESHOLD"));
            this->currentParameters.setPropertySystemInt("MIXTURE_CUMULATIVE_TIME_LEVEL1", configurationVector.getElementInt("MIXTURE_CUMULATIVE_TIME_LEVEL1"));
            this->currentParameters.setPropertySystemFloat("MIXTURE_NOTOBSERVED_LEVEL1_THRESHOLD", configurationVector.getElementFloat("MIXTURE_NOTOBSERVED_LEVEL1_THRESHOLD"));
            this->currentParameters.setPropertySystemInt("MIXTURE_CUMULATIVE_TIME_LEVEL2", configurationVector.getElementInt("MIXTURE_CUMULATIVE_TIME_LEVEL2"));
            this->currentParameters.setPropertySystemFloat("MIXTURE_NOTOBSERVED_LEVEL2_THRESHOLD", configurationVector.getElementFloat("MIXTURE_NOTOBSERVED_LEVEL2_THRESHOLD"));

            // Microphone Sound Track
            this->currentParameters.setPropertySystemFloat("MICST_ALPHAD", configurationVector.getElementFloat("MICST_ALPHAD"));
            this->currentParameters.setPropertySystemFloat("MICST_GAMMA", configurationVector.getElementFloat("MICST_GAMMA"));
            this->currentParameters.setPropertySystemFloat("MICST_DELTA", configurationVector.getElementFloat("MICST_DELTA"));
            this->currentParameters.setPropertySystemFloat("MICST_MCRA_ALPHAS", configurationVector.getElementFloat("MICST_MCRA_ALPHAS"));
            this->currentParameters.setPropertySystemFloat("MICST_MCRA_ALPHAP", configurationVector.getElementFloat("MICST_MCRA_ALPHAP"));
            this->currentParameters.setPropertySystemFloat("MICST_MCRA_ALPHAD", configurationVector.getElementFloat("MICST_MCRA_ALPHAD"));
            this->currentParameters.setPropertySystemInt("MICST_MCRA_L", configurationVector.getElementInt("MICST_MCRA_L"));
            this->currentParameters.setPropertySystemFloat("MICST_MCRA_DELTA", configurationVector.getElementFloat("MICST_MCRA_DELTA"));


            // Clear the list of receivers
            registerObjectConfirmStopClear();

            // Restart the thread if needed
            if (this->needsRestart == true)
            {
                this->startThread(this->currentParameters);

            }

            // There needs to be one event for each object
            for (int indexReceiver = 0; indexReceiver < this->objectReceiverPotentialList.size(); indexReceiver++)
            {
                QCoreApplication::postEvent(this->objectReceiverPotentialList[indexReceiver], new GraphClearEvent());
                QCoreApplication::postEvent(this->objectReceiverTrackedList[indexReceiver], new GraphClearEvent());
            }

        }

        // +-----------------------------------------------+
        // | Restart                                       |
        // +-----------------------------------------------+

        AlgorithmRestartEvent *algorithmRestartEvent = dynamic_cast<AlgorithmRestartEvent*>(event);

        if (algorithmRestartEvent != NULL)
        {

            // Save the state of the thread
            this->needsRestart = this->isRunning();

            this->stopThread();

            if (this->audioManager->getInputType() == AudioManager::TYPE_FILE)
            {
                QString filePath = this->audioManager->getFileName();
                this->audioManager->closeStream();
                this->audioManager->openStream(filePath);
            }

            if (this->needsRestart)
            {
                this->startThread(this->currentParameters);
            }

            // There needs to be one event for each object
            for (int indexReceiver = 0; indexReceiver < this->objectReceiverPotentialList.size(); indexReceiver++)
            {
                QCoreApplication::postEvent(this->objectReceiverPotentialList[indexReceiver], new GraphClearEvent());
                QCoreApplication::postEvent(this->objectReceiverTrackedList[indexReceiver], new GraphClearEvent());
            }


        }

        return true;

    }

    return (AlgorithmCore::event(event));

}

/***********************************************************
* Computation steps                                        *
***********************************************************/

// +-------------------------------------------------------+
// | Initialize the system                                 |
// +-------------------------------------------------------+

void AlgorithmCore::initSystem()
{

    // Create the context
    this->algorithmContext = createEmptyOverallContext();

    // Generate the parameters structure
    *(this->algorithmContext.myParameters) = this->currentParameters.getParametersSystem();

    // Save number of sources
    this->numberPotentialSources = this->algorithmContext.myParameters->P_BF_MAXSOURCES;
    this->numberTrackedSources = this->algorithmContext.myParameters->P_MIXTURE_NBFILTERS;

    // Initialize the beamformer
    beamformerInit(this->algorithmContext.myBeamformer, this->algorithmContext.myParameters);

    // Initialize the mixture
    mixtureInit(this->algorithmContext.myMixture, this->algorithmContext.myParameters);

    // Initialize the potential sources
    potentialSourcesInit(this->algorithmContext.myPotentialSources, this->algorithmContext.myParameters);

    // Initialize the sources
    trackedSourcesInit(this->algorithmContext.myTrackedSources, this->algorithmContext.myParameters);

}

// +-------------------------------------------------------+
// | Load a new frame                                      |
// +-------------------------------------------------------+

void AlgorithmCore::loadFrame()
{

    int indexBuffer;
    int currentHopIndex;

    float *buffers;
    float *startFrame;

    // Get the buffer pointer
    buffers = this->audioManager->getBuffers();

    // Get the index of the hop
    currentHopIndex = this->audioManager->getIndexHop();

    // Copy frames to the beamformer frames
    for (int indexChannel = 0; indexChannel < this->audioManager->getNumberChannels(); indexChannel++)
    {

        beamformerPushFrames(this->algorithmContext.myBeamformer, this->audioManager->getHopSize(), indexChannel);

        indexBuffer = this->audioManager->getHopSize() * indexChannel;

        beamformerAddFrame(this->algorithmContext.myBeamformer, &buffers[indexBuffer], indexChannel, this->audioManager->getHopSize());

    }

}

// +-------------------------------------------------------+
// | Find potential sources                                |
// +-------------------------------------------------------+

void AlgorithmCore::findPotentialSources()
{

    // Perform the computations relative to the beamformer
    beamformerFindMaxima(this->algorithmContext.myBeamformer, this->algorithmContext.myPotentialSources);

}

// +-------------------------------------------------------+
// | Output potential sources                              |
// +-------------------------------------------------------+

void AlgorithmCore::outputPotentialSources()
{

    PotentialSourceEvent tmp;

    // Add these potential sources to the event
    for (int indexSource = 0; indexSource < this->numberPotentialSources; indexSource++)
    {
        tmp.pushSource(PotentialSource(this->algorithmContext.myPotentialSources->sourcesPosition[indexSource][0], this->algorithmContext.myPotentialSources->sourcesPosition[indexSource][1], this->algorithmContext.myPotentialSources->sourcesPosition[indexSource][2], this->algorithmContext.myPotentialSources->sourcesEnergy[indexSource], indexSource));
    }

    // There needs to be one event for each object
    for (int indexReceiver = 0; indexReceiver < this->objectReceiverPotentialList.size(); indexReceiver++)
    {
        QCoreApplication::postEvent(this->objectReceiverPotentialList[indexReceiver], new PotentialSourceEvent(tmp));
    }

}

// +-------------------------------------------------------+
// | Find tracked sources                                  |
// +-------------------------------------------------------+

void AlgorithmCore::findTrackedSources()
{

    // Perform the computations relative to the mixture of particle filters
    mixtureUpdate(this->algorithmContext.myMixture, this->algorithmContext.myTrackedSources, this->algorithmContext.myPotentialSources);
}

// +-------------------------------------------------------+
// | Output tracked sources                                |
// +-------------------------------------------------------+

void AlgorithmCore::outputTrackedSources()
{

    TrackedSourceEvent tmp;

    // Add these tracked sources to the event
    for (int indexSource = 0; indexSource < this->numberTrackedSources; indexSource++)
    {
        tmp.pushSource(TrackedSource(this->algorithmContext.myTrackedSources->sourcesPosition[indexSource][0], this->algorithmContext.myTrackedSources->sourcesPosition[indexSource][1], this->algorithmContext.myTrackedSources->sourcesPosition[indexSource][2], this->algorithmContext.myTrackedSources->sourcesID[indexSource]));
    }

    // There needs to be one event for each object
    for (int indexReceiver = 0; indexReceiver < this->objectReceiverTrackedList.size(); indexReceiver++)
    {
        QCoreApplication::postEvent(this->objectReceiverTrackedList[indexReceiver], new TrackedSourceEvent(tmp));
    }

}

// +-------------------------------------------------------+
// | Terminate the system                                  |
// +-------------------------------------------------------+

void AlgorithmCore::terminateSystem()
{

    // Terminate structures
    beamformerTerminate(this->algorithmContext.myBeamformer);
    mixtureTerminate(this->algorithmContext.myMixture);
    potentialSourcesTerminate(this->algorithmContext.myPotentialSources);
    trackedSourcesTerminate(this->algorithmContext.myTrackedSources);

    // Delete context
    deleteOverallContext(this->algorithmContext);

}

/***********************************************************
* Function called when buffer is ready                     *
***********************************************************/

void bufferIsFull(void *objPtr)
{

    // Convert pointer
    AlgorithmCore* algorithmCorePtr = reinterpret_cast<AlgorithmCore*>(objPtr);

    // Post semaphore to tell a new frame is ready
    algorithmCorePtr->postFrame();

}

// XxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxX
// x                                                       x
// x Algorithm Core Control Events                         x
// x                                                       x
// XxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxX

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// % Confirm the thread stopped                            %
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/***********************************************************
* Constructor                                              *
***********************************************************/

AlgorithmCoreConfirmStop::AlgorithmCoreConfirmStop(): QEvent(QEvent::User)
{

}

AlgorithmCoreConfirmStop::AlgorithmCoreConfirmStop(const AlgorithmCoreConfirmStop& _algorithmCoreConfirmStop): QEvent(QEvent::User)
{
    *this = _algorithmCoreConfirmStop;
}

/***********************************************************
* Destructor                                               *
***********************************************************/

AlgorithmCoreConfirmStop::~AlgorithmCoreConfirmStop()
{

}

/***********************************************************
* Operators                                                *
***********************************************************/

// +-------------------------------------------------------+
// | Equal                                                 |
// +-------------------------------------------------------+

AlgorithmCoreConfirmStop& AlgorithmCoreConfirmStop::operator=(const AlgorithmCoreConfirmStop& _algorithmCoreConfirmStop)
{

}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// % Restart thread with stream at the beginning           %
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/***********************************************************
* Constructor                                              *
***********************************************************/

AlgorithmRestartEvent::AlgorithmRestartEvent(): QEvent(QEvent::User)
{

}

AlgorithmRestartEvent::AlgorithmRestartEvent(const AlgorithmRestartEvent& _algorithmRestartEvent): QEvent(QEvent::User)
{
    *this = _algorithmRestartEvent;
}

/***********************************************************
* Destructor                                               *
***********************************************************/

AlgorithmRestartEvent::~AlgorithmRestartEvent()
{

}

/***********************************************************
* Operators                                                *
***********************************************************/

// +-------------------------------------------------------+
// | Equal                                                 |
// +-------------------------------------------------------+

AlgorithmRestartEvent& AlgorithmRestartEvent::operator=(const AlgorithmRestartEvent& _algorithmRestartEvent)
{

}


//
///***********************************************************
//* Constructor                                              *
//***********************************************************/
//
//Graph3DEnergyEvent::Graph3DEnergyEvent(): QEvent(QEvent::User)
//{
//
//}
//
//Graph3DEnergyEvent::Graph3DEnergyEvent(float* _spherePoints): QEvent(QEvent::User)
//{
//
//    for (int indexPoint = 0; indexPoint < SPHERE_NUMBERPOINTS; indexPoint++)
//    {
//        this->spherePoints[indexPoint] = _spherePoints[indexPoint];
//    }
//
//}
//
///***********************************************************
//* Destructor                                               *
//***********************************************************/
//
//Graph3DEnergyEvent::~Graph3DEnergyEvent()
//{
//
//}
//
///***********************************************************
//* Constructor                                              *
//***********************************************************/
//
//Graph3DTrianglesEvent::Graph3DTrianglesEvent(): QEvent(QEvent::User)
//{
//
//}
//
//Graph3DTrianglesEvent::Graph3DTrianglesEvent(struct objSphereTriangle _sphereTriangles, struct objSphere _spherePoints): QEvent(QEvent::User)
//{
//
//    for (int indexTriangle = 0; indexTriangle < SPHERE_NUMBERTRIANGLES; indexTriangle++)
//    {
//        for (int indexPoint = 0; indexPoint < 3; indexPoint++)
//        {
//            this->sphereTrianglesIndex[indexTriangle][indexPoint] = _sphereTriangles.sphereTriangles[indexTriangle][indexPoint];
//        }
//    }
//
//    for (int indexPoint = 0; indexPoint < SPHERE_NUMBERPOINTS; indexPoint++)
//    {
//        this->spherePoints[indexPoint][0] = _spherePoints.spherePoints[indexPoint][0];
//        this->spherePoints[indexPoint][1] = _spherePoints.spherePoints[indexPoint][1];
//        this->spherePoints[indexPoint][2] = _spherePoints.spherePoints[indexPoint][2];
//    }
//
//}
//
///***********************************************************
//* Destructor                                               *
//***********************************************************/
//
//Graph3DTrianglesEvent::~Graph3DTrianglesEvent()
//{
//
//}
//
///***********************************************************
//* Constructor                                              *
//***********************************************************/
//
//Graph3DGotoFrameEvent::Graph3DGotoFrameEvent(): QEvent(QEvent::User)
//{
//
//}
//
//Graph3DGotoFrameEvent::Graph3DGotoFrameEvent(int _frame): QEvent(QEvent::User)
//{
//    this->frame = _frame;
//}
//
///***********************************************************
//* Destructor                                               *
//***********************************************************/
//
//Graph3DGotoFrameEvent::~Graph3DGotoFrameEvent()
//{
//
//}

