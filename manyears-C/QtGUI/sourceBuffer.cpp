#include "sourceBuffer.h"

// XxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxX
// x                                                       x
// x Sources to be displayed                               x
// x                                                       x
// XxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxX

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// % Potential and Tracked sources                         %
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/***********************************************************
* Constructor                                              *
***********************************************************/

SourceDisplayed::SourceDisplayed(const float _X, const float _Y, const float _Z, const QColor _color)
{

    this->setX(_X);
    this->setY(_Y);
    this->setZ(_Z);
    this->setColor(_color);

}

SourceDisplayed::SourceDisplayed(const SourceDisplayed& _sourceDisplayed)
{

    *this = _sourceDisplayed;

}

SourceDisplayed::SourceDisplayed()
{

}

/***********************************************************
* Destructor                                               *
***********************************************************/

SourceDisplayed::~SourceDisplayed()
{

}

/***********************************************************
* Operator                                                 *
***********************************************************/

// +-------------------------------------------------------+
// | Equal                                                 |
// +-------------------------------------------------------+

SourceDisplayed& SourceDisplayed::operator=(const SourceDisplayed& _sourceDisplayed)
{

    this->setX(_sourceDisplayed.getX());
    this->setY(_sourceDisplayed.getY());
    this->setZ(_sourceDisplayed.getZ());
    this->setColor(_sourceDisplayed.getColor());

    return (*this);

}

/***********************************************************
* Accessor                                                 *
***********************************************************/

// +-------------------------------------------------------+
// | X                                                     |
// +-------------------------------------------------------+

float SourceDisplayed::getX() const
{
    return this->X;
}

// +-------------------------------------------------------+
// | Y                                                     |
// +-------------------------------------------------------+

float SourceDisplayed::getY() const
{
    return this->Y;
}

// +-------------------------------------------------------+
// | Z                                                     |
// +-------------------------------------------------------+

float SourceDisplayed::getZ() const
{
    return this->Z;
}

// +-------------------------------------------------------+
// | Color                                                 |
// +-------------------------------------------------------+

QColor SourceDisplayed::getColor() const
{
    return this->color;
}

/***********************************************************
* Mutator                                                  *
***********************************************************/

// +-------------------------------------------------------+
// | X                                                     |
// +-------------------------------------------------------+

void SourceDisplayed::setX(float _X)
{
    this->X = _X;
}

// +-------------------------------------------------------+
// | Y                                                     |
// +-------------------------------------------------------+

void SourceDisplayed::setY(float _Y)
{
    this->Y = _Y;
}

// +-------------------------------------------------------+
// | Z                                                     |
// +-------------------------------------------------------+

void SourceDisplayed::setZ(float _Z)
{
    this->Z = _Z;
}

// +-------------------------------------------------------+
// | Color                                                 |
// +-------------------------------------------------------+

void SourceDisplayed::setColor(QColor _color)
{
    this->color = _color;
}


// XxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxX
// x                                                       x
// x Sources for one frame                                 x
// x                                                       x
// XxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxX

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// % Potential Source                                      %
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/***********************************************************
* Constructor                                              *
***********************************************************/

PotentialSourceOneFrame::PotentialSourceOneFrame()
{

}

PotentialSourceOneFrame::PotentialSourceOneFrame(const PotentialSourceOneFrame& _potentialSourceOneFrame)
{
    *this = _potentialSourceOneFrame;
}

/***********************************************************
* Destructor                                               *
***********************************************************/

PotentialSourceOneFrame::~PotentialSourceOneFrame()
{

}

/***********************************************************
* Operator                                                 *
***********************************************************/

// +-------------------------------------------------------+
// | Equal                                                 |
// +-------------------------------------------------------+

PotentialSourceOneFrame& PotentialSourceOneFrame::operator=(const PotentialSourceOneFrame& _potentialSourceOneFrame)
{

    this->listSources.clear();

    for (int indexSource = 0; indexSource < _potentialSourceOneFrame.getNumberSources(); indexSource++)
    {
        this->listSources.push_back(_potentialSourceOneFrame.getSourcePotential(indexSource));
    }

    return (*this);

}

/***********************************************************
* Accessors                                                *
***********************************************************/

// +-------------------------------------------------------+
// | Number of sources                                     |
// +-------------------------------------------------------+

int PotentialSourceOneFrame::getNumberSources() const
{
    return (this->listSources.size());
}

// +-------------------------------------------------------+
// | Get source (potential)                                |
// +-------------------------------------------------------+

PotentialSource PotentialSourceOneFrame::getSourcePotential(const int _indexSource) const
{
    return (this->listSources[_indexSource]);
}

// +-------------------------------------------------------+
// | Get source (displayed)                                |
// +-------------------------------------------------------+

SourceDisplayed PotentialSourceOneFrame::getSourceDisplayed(const int _indexSource, const int _ET) const
{

    PotentialSource tmp = this->getSourcePotential(_indexSource);

    float Pq = tmp.getE();

//    float v = tmp.getE() / _ET;
//
//    float Pq;
//
//    switch (tmp.getIndex())
//    {
//
//
//    case 0:
//
//        if (v <= 1)
//        {
//            Pq = v*v/2.0;
//        }
//        else
//        {
//            Pq = 1 - 1/(2*v*v);
//        }
//        break;
//
//    case 1:
//
//        Pq = 0.3;
//
//        break;
//
//    case 2:
//
//        Pq = 0.16;
//
//        break;
//
//    case 3:
//
//        Pq = 0.03;
//
//        break;
//
//
//    }

    //CHANGE COLOR HERE
    QColor myColor(0,0,0);

    //start from green to red
    myColor.setHsv(120 + (Pq * 240),230,180);

    SourceDisplayed rtnSource(tmp.getX(), tmp.getY(), tmp.getZ(), myColor);

    return rtnSource;

}

// +-------------------------------------------------------+
// | Get all sources (displayed)                           |
// +-------------------------------------------------------+

QList<SourceDisplayed> PotentialSourceOneFrame::getSourcesOneFrame(const int _ET) const
{

    QList<SourceDisplayed> tmp;

    for (int indexSource = 0; indexSource < this->getNumberSources(); indexSource++)
    {
        tmp.push_back(this->getSourceDisplayed(indexSource, _ET));
    }

    return tmp;

}

/***********************************************************
* Mutator                                                  *
***********************************************************/

// +-------------------------------------------------------+
// | Push a new source                                     |
// +-------------------------------------------------------+

void PotentialSourceOneFrame::pushSource(PotentialSource _newSource)
{
    this->listSources.push_back(_newSource);
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// % Tracked Source                                        %
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/***********************************************************
* Constructor                                              *
***********************************************************/

TrackedSourceOneFrame::TrackedSourceOneFrame()
{

}

TrackedSourceOneFrame::TrackedSourceOneFrame(const TrackedSourceOneFrame& _trackedSourceOneFrame)
{
    (*this) = _trackedSourceOneFrame;
}

/***********************************************************
* Destructor                                               *
***********************************************************/

TrackedSourceOneFrame::~TrackedSourceOneFrame()
{

}

/***********************************************************
* Operator                                                 *
***********************************************************/

// +-------------------------------------------------------+
// | Equal                                                 |
// +-------------------------------------------------------+

TrackedSourceOneFrame& TrackedSourceOneFrame::operator=(const TrackedSourceOneFrame& _trackedSourceOneFrame)
{

    this->listSources.clear();

    for (int indexSource = 0; indexSource < _trackedSourceOneFrame.getNumberSources(); indexSource++)
    {
        this->listSources.push_back(_trackedSourceOneFrame.getSourceTracked(indexSource));
    }

    return (*this);

}

/***********************************************************
* Accessors                                                *
***********************************************************/

// +-------------------------------------------------------+
// | Number of sources                                     |
// +-------------------------------------------------------+

int TrackedSourceOneFrame::getNumberSources() const
{
    return (this->listSources.size());
}

// +-------------------------------------------------------+
// | Get source (potential)                                |
// +-------------------------------------------------------+

TrackedSource TrackedSourceOneFrame::getSourceTracked(const int _indexSource) const
{
    return (this->listSources[_indexSource]);
}

// +-------------------------------------------------------+
// | Get source (displayed)                                |
// +-------------------------------------------------------+

SourceDisplayed TrackedSourceOneFrame::getSourceDisplayed(const int _indexSource, QList<QColor>* _listColors, QList<int>* _listColorIDs, QList<int>* _listSourceIDs) const
{

    TrackedSource tmp = this->getSourceTracked(_indexSource);  

    int selectedColorID = -1;

    for (int indexSourceID = 0; indexSourceID < _listSourceIDs->size(); indexSourceID++)
    {
        if (_listSourceIDs->at(indexSourceID) == tmp.getID())
        {
            selectedColorID = _listColorIDs->at(indexSourceID);
            break;
        }
    }

    QColor sourceColorValue;

    if (selectedColorID != -1)
    {
        sourceColorValue = _listColors->at(selectedColorID);
    }
    else
    {
        sourceColorValue = QColor(0x00,0x00,0x00);
    }

    SourceDisplayed rtnSource(tmp.getX(), tmp.getY(), tmp.getZ(), sourceColorValue);

    return rtnSource;

}

// +-------------------------------------------------------+
// | Get source exists                                     |
// +-------------------------------------------------------+

bool TrackedSourceOneFrame::getSourceExists(const int _indexSource) const
{

    return (this->getSourceTracked(_indexSource).getID() != -1);

}

// +-------------------------------------------------------+
// | Get all sources (displayed)                           |
// +-------------------------------------------------------+

QList<SourceDisplayed> TrackedSourceOneFrame::getSourcesOneFrame(QList<QColor>* _listColors, QList<int>* _listColorIDs, QList<int>* _listSourceIDs) const
{

    QList<SourceDisplayed> tmp;

    for (int indexSource = 0; indexSource < this->getNumberSources(); indexSource++)
    {

        if (this->getSourceExists(indexSource) == true)
        {

            tmp.push_back(this->getSourceDisplayed(indexSource, _listColors, _listColorIDs, _listSourceIDs));

        }
    }

    return tmp;

}

/***********************************************************
* Mutator                                                  *
***********************************************************/

// +-------------------------------------------------------+
// | Push a new source                                     |
// +-------------------------------------------------------+

void TrackedSourceOneFrame::pushSource(TrackedSource _newSource)
{
    this->listSources.push_back(_newSource);
}

// XxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxX
// x                                                       x
// x Sources for all frame                                 x
// x                                                       x
// XxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxX

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// % Potential Source                                      %
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/***********************************************************
* Constructor                                              *
***********************************************************/

PotentialSourceAllFrames::PotentialSourceAllFrames()
{

}

PotentialSourceAllFrames::PotentialSourceAllFrames(const PotentialSourceAllFrames& _potentialSourceAllFrames)
{

    (*this) = _potentialSourceAllFrames;

}

/***********************************************************
* Destructor                                               *
***********************************************************/

PotentialSourceAllFrames::~PotentialSourceAllFrames()
{

}

/***********************************************************
* Operator                                                 *
***********************************************************/

// +-------------------------------------------------------+
// | Equal                                                 |
// +-------------------------------------------------------+

PotentialSourceAllFrames& PotentialSourceAllFrames::operator=(const PotentialSourceAllFrames& _potentialSourceAllFrames)
{

    this->listSources.clear();  

    for (int indexFrame = 0; indexFrame < _potentialSourceAllFrames.getNumberFrames(); indexFrame++)
    {

        this->listSources.push_back(_potentialSourceAllFrames.getSourcesAtFrame(indexFrame));

    }

    return (*this);

}

/***********************************************************
* Accessors                                                *
***********************************************************/

// +-------------------------------------------------------+
// | Number of frames                                      |
// +-------------------------------------------------------+

int PotentialSourceAllFrames::getNumberFrames() const
{
    return (this->listSources.size());
}

// +-------------------------------------------------------+
// | Get displayed sources for all frames                  |
// +-------------------------------------------------------+

QList< QList<SourceDisplayed> > PotentialSourceAllFrames::getSourcesAllFrames(const float _ET) const
{

    QList< QList< SourceDisplayed > > tmpList;

    QList<PotentialSourceOneFrame>::const_iterator frameIterator;

    frameIterator = this->listSources.constBegin();

    for (int indexFrame = 0; indexFrame < this->listSources.size(); indexFrame++)
    {

        tmpList.push_back(frameIterator->getSourcesOneFrame(_ET));

        frameIterator++;

    }

    return tmpList;

}

// +-------------------------------------------------------+
// | Get potential sources at specified frame              |
// +-------------------------------------------------------+

PotentialSourceOneFrame PotentialSourceAllFrames::getSourcesAtFrame(const int _indexFrame) const
{

    PotentialSourceOneFrame tmp;

    for (int indexSource = 0; indexSource < (this->listSources[_indexFrame].getNumberSources()); indexSource++)
    {

        tmp.pushSource((this->listSources[_indexFrame]).getSourcePotential(indexSource));

    }

    return tmp;

}

/***********************************************************
* Mutator                                                  *
***********************************************************/

// +-------------------------------------------------------+
// | Number of frames                                      |
// +-------------------------------------------------------+

void PotentialSourceAllFrames::setNumberFrames(const int _numberFrames)
{

    while (this->listSources.size() > _numberFrames)
    {
        this->listSources.pop_front();
    }

    PotentialSourceOneFrame tmp;

    while (this->listSources.size() < _numberFrames)
    {
        this->listSources.push_back(tmp);
    }

}

// +-------------------------------------------------------+
// | Push sources for one frame                            |
// +-------------------------------------------------------+

void PotentialSourceAllFrames::pushSourcesOneFrame(PotentialSourceOneFrame _sources)
{

    this->listSources.push_back(_sources);
    this->listSources.pop_front();

}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// % Tracked Source                                        %
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/***********************************************************
* Constructor                                              *
***********************************************************/

TrackedSourceAllFrames::TrackedSourceAllFrames()
{

}

TrackedSourceAllFrames::TrackedSourceAllFrames(const TrackedSourceAllFrames& _trackedSourceAllFrames)
{
    (*this) = _trackedSourceAllFrames;
}

/***********************************************************
* Destructor                                               *
***********************************************************/

TrackedSourceAllFrames::~TrackedSourceAllFrames()
{

}

/***********************************************************
* Operator                                                 *
***********************************************************/

// +-------------------------------------------------------+
// | Equal                                                 |
// +-------------------------------------------------------+

TrackedSourceAllFrames& TrackedSourceAllFrames::operator=(const TrackedSourceAllFrames& _trackedSourceAllFrames)
{

    this->listSources.clear();

    for (int indexFrame = 0; indexFrame < _trackedSourceAllFrames.getNumberFrames(); indexFrame++)
    {

        this->listSources.push_back(_trackedSourceAllFrames.getSourcesAtFrame(indexFrame));

    }

    return (*this);

}

/***********************************************************
* Accessors                                                *
***********************************************************/

// +-------------------------------------------------------+
// | Number of frames                                      |
// +-------------------------------------------------------+

int TrackedSourceAllFrames::getNumberFrames() const
{
    return (this->listSources.size());
}

// +-------------------------------------------------------+
// | Get displayed sources for all frames                  |
// +-------------------------------------------------------+

QList< QList<SourceDisplayed> > TrackedSourceAllFrames::getSourcesAllFrames(QList<QColor>* _listColors, QList<int>* _listColorIDs, QList<int>* _listSourceIDs) const
{

    QList< QList< SourceDisplayed > > tmpList;

    QList<TrackedSourceOneFrame>::const_iterator frameIterator;

    frameIterator = this->listSources.constBegin();

    for (int indexFrame = 0; indexFrame < this->listSources.size(); indexFrame++)
    {

        tmpList.push_back(frameIterator->getSourcesOneFrame(_listColors, _listColorIDs, _listSourceIDs));

        frameIterator++;

    }

    return tmpList;

}

// +-------------------------------------------------------+
// | Get potential sources at specified frame              |
// +-------------------------------------------------------+

TrackedSourceOneFrame TrackedSourceAllFrames::getSourcesAtFrame(const int _indexFrame) const
{

    TrackedSourceOneFrame tmp;

    for (int indexSource = 0; indexSource < (this->listSources[_indexFrame].getNumberSources()); indexSource++)
    {

        tmp.pushSource((this->listSources[_indexFrame]).getSourceTracked(indexSource));

    }

    return tmp;

}

/***********************************************************
* Mutator                                                  *
***********************************************************/

// +-------------------------------------------------------+
// | Number of frames                                      |
// +-------------------------------------------------------+

void TrackedSourceAllFrames::setNumberFrames(const int _numberFrames)
{

    while (this->listSources.size() > _numberFrames)
    {
        this->listSources.pop_front();
    }

    TrackedSourceOneFrame tmp;

    while (this->listSources.size() < _numberFrames)
    {
        this->listSources.push_back(tmp);
    }

}

// +-------------------------------------------------------+
// | Push sources for one frame                            |
// +-------------------------------------------------------+

void TrackedSourceAllFrames::pushSourcesOneFrame(TrackedSourceOneFrame _sources)
{

    this->listSources.push_back(_sources);
    this->listSources.pop_front();

}





// XxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxX
// x                                                       x
// x Sources Buffers Manager                               x
// x                                                       x
// XxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxX

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// % Potential Source                                      %
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/***********************************************************
* Constructor                                              *
***********************************************************/

PotentialSourceBufferManager::PotentialSourceBufferManager()
{
    // Set default parameters
    this->potentialSourceBuffer.setNumberFrames(1);
}

PotentialSourceBufferManager::PotentialSourceBufferManager(const PotentialSourceBufferManager& _potentialSourceBufferManager)
{
    (*this) = _potentialSourceBufferManager;
}

/***********************************************************
* Destructor                                               *
***********************************************************/

PotentialSourceBufferManager::~PotentialSourceBufferManager()
{

}

/***********************************************************
* Operator                                                 *
***********************************************************/

// +-------------------------------------------------------+
// | Equal                                                 |
// +-------------------------------------------------------+

PotentialSourceBufferManager& PotentialSourceBufferManager::operator=(const PotentialSourceBufferManager& _potentialSourceBufferManager)
{

    this->potentialSourceBuffer = _potentialSourceBufferManager.getPotentialSourceBuffer();

    return (*this);

}

/***********************************************************
* Event receivers                                          *
***********************************************************/

// +-------------------------------------------------------+
// | Push                                                  |
// +-------------------------------------------------------+

void PotentialSourceBufferManager::pushEventReceiver(QObject* _receiver)
{

    this->listObjectReceiver.push_back(_receiver);

}

/***********************************************************
* Accessors                                                *
***********************************************************/

// +-------------------------------------------------------+
// | List of all sources                                   |
// +-------------------------------------------------------+

PotentialSourceAllFrames PotentialSourceBufferManager::getPotentialSourceBuffer() const
{
    return (this->potentialSourceBuffer);
}

/***********************************************************
* Mutators                                                 *
***********************************************************/

// +-------------------------------------------------------+
// | Load parameters: view                                 |
// +-------------------------------------------------------+

void PotentialSourceBufferManager::loadParametersView(ConfigurationVector _configurationVector)
{

    // Set the number of frames
    float xView = _configurationVector.getElementFloat("TIME_BUFFER");
    float timePerFrame = ((float) ParametersManager::HOPSIZE) / ((float) ParametersManager::SAMPLINGRATE);
    int numberFrames = round(xView / timePerFrame);
    this->potentialSourceBuffer.setNumberFrames(numberFrames);

}

// +-------------------------------------------------------+
// | Load parameters: system                               |
// +-------------------------------------------------------+

void PotentialSourceBufferManager::loadParametersSystem(ConfigurationVector _configurationVector)
{

    // Set the energy level
    this->ET = _configurationVector.getElementFloat("MIXTURE_ET");

}


/***********************************************************
* Events                                                   *
***********************************************************/

bool PotentialSourceBufferManager::event(QEvent *event)
{

    if (event->type() == QEvent::User)
    {

        // +-----------------------------------------------+
        // | Add new sources for one frame                 |
        // +-----------------------------------------------+

        PotentialSourceEvent *potentialSourceEvent = dynamic_cast<PotentialSourceEvent*>(event);

        if (potentialSourceEvent != NULL)
        {

            // Add this set of new sources
            PotentialSourceOneFrame currentFrame;

            for (int indexSource = 0; indexSource < potentialSourceEvent->getListSize(); indexSource++)
            {
                currentFrame.pushSource(potentialSourceEvent->getPotentialSource(indexSource));
            }

            this->potentialSourceBuffer.pushSourcesOneFrame(currentFrame);

            // Get the sources to be displayed for this frame

            QList<SourceDisplayed> tmp = currentFrame.getSourcesOneFrame(this->ET);

            // Post an event to the GUI for each receiver

            for (int indexReceiver = 0; indexReceiver < this->listObjectReceiver.size(); indexReceiver++)
            {
                QCoreApplication::postEvent(this->listObjectReceiver[indexReceiver], new UpdateGraphAddSampleEvent(tmp));
            }

        }

        // +-----------------------------------------------+
        // | Resize the graph and send all sources         |
        // +-----------------------------------------------+

        GraphResizeEvent *graphResizeEvent = dynamic_cast<GraphResizeEvent*>(event);

        if (graphResizeEvent != NULL)
        {

            // Get new dimensions
            int width = graphResizeEvent->getWidth();
            int height = graphResizeEvent->getHeight();

            // Get the reference of the object who produced the event
            QObject* objectResized = graphResizeEvent->getObjectResized();

            // Get the sources
            QList < QList<SourceDisplayed> > tmp;

            tmp = this->potentialSourceBuffer.getSourcesAllFrames(this->ET);

            // Post an event to the GUI for the receiver
            QCoreApplication::postEvent(objectResized, new UpdateGraphResizeEvent(tmp, width, height));

        }

        // +-----------------------------------------------+
        // | Clear all sources                             |
        // +-----------------------------------------------+

        GraphClearEvent *graphClearEvent = dynamic_cast<GraphClearEvent*>(event);

        if (graphClearEvent != NULL)
        {

            // Get the sources
            QList < QList<SourceDisplayed> > tmp;

            // Post an event to the GUI for each receiver

            for (int indexReceiver = 0; indexReceiver < this->listObjectReceiver.size(); indexReceiver++)
            {
                QCoreApplication::postEvent(this->listObjectReceiver[indexReceiver], new UpdateGraphRefreshEvent(tmp));
            }

        }

        // +-----------------------------------------------+
        // | Update parameters                             |
        // +-----------------------------------------------+

        UpdateParametersEvent *updateParametersEvent = dynamic_cast<UpdateParametersEvent*>(event);

        if (updateParametersEvent != NULL)
        {

            if (updateParametersEvent->getTypeVector() == UpdateParametersEvent::VIEW)
            {
                loadParametersView(updateParametersEvent->getConfigurationVector());
            }
            else
            {
                loadParametersSystem(updateParametersEvent->getConfigurationVector());
            }

            // Get the sources
            QList < QList<SourceDisplayed> > tmp;

            tmp = this->potentialSourceBuffer.getSourcesAllFrames(this->ET);

            // Post an event to the GUI for each receiver in order to refresh
            // these graphs

            for (int indexReceiver = 0; indexReceiver < this->listObjectReceiver.size(); indexReceiver++)
            {
                // Post parameters only if they are view parameters
                if (updateParametersEvent->getTypeVector() == UpdateParametersEvent::VIEW)
                {
                    QCoreApplication::postEvent(this->listObjectReceiver[indexReceiver], new UpdateParametersEvent(*updateParametersEvent));
                }

                // Then post the sources for any type of parameters (for system parameters, color may changed if ET changes)
                QCoreApplication::postEvent(this->listObjectReceiver[indexReceiver], new UpdateGraphRefreshEvent(tmp));
            }

        }

        return true;

    }

    return QObject::event(event);

}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// % Tracked Source                                        %
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/***********************************************************
* Constructor                                              *
***********************************************************/

TrackedSourceBufferManager::TrackedSourceBufferManager()
{
    // Set default parameters
    this->trackedSourceBuffer.setNumberFrames(1);

    // Fill color palette
    for (int indexColor = 0; indexColor < ParametersManager::SIZECOLORPALETTE; indexColor++)
    {
        this->colorsValue.push_back(QColor(0x00,0x00,0x00));
        this->colorsFreeID.push_back(indexColor);
    }
}

TrackedSourceBufferManager::TrackedSourceBufferManager(const TrackedSourceBufferManager& _trackedSourceBufferManager)
{
    *this = _trackedSourceBufferManager;
}

/***********************************************************
* Destructor                                               *
***********************************************************/

TrackedSourceBufferManager::~TrackedSourceBufferManager()
{

}

/***********************************************************
* Operator                                                 *
***********************************************************/

// +-------------------------------------------------------+
// | Equal                                                 |
// +-------------------------------------------------------+

TrackedSourceBufferManager& TrackedSourceBufferManager::operator=(const TrackedSourceBufferManager& _trackedSourceBufferManager)
{

    this->trackedSourceBuffer = _trackedSourceBufferManager.getTrackedSourceBuffer();

    return (*this);

}

/***********************************************************
* Event receivers                                          *
***********************************************************/

// +-------------------------------------------------------+
// | Push                                                  |
// +-------------------------------------------------------+

void TrackedSourceBufferManager::pushEventReceiver(QObject* _receiver)
{
    this->listObjectReceiver.push_back(_receiver);
}

/***********************************************************
* Accessors                                                *
***********************************************************/

// +-------------------------------------------------------+
// | List of all sources                                   |
// +-------------------------------------------------------+

TrackedSourceAllFrames TrackedSourceBufferManager::getTrackedSourceBuffer() const
{
    return (this->trackedSourceBuffer);
}

/***********************************************************
* Mutators                                                 *
***********************************************************/

// +-------------------------------------------------------+
// | Load parameters                                       |
// +-------------------------------------------------------+

void TrackedSourceBufferManager::loadParameters(ConfigurationVector _configurationVector)
{

    // Set the number of frames
    float xView = _configurationVector.getElementFloat("TIME_BUFFER");
    float timePerFrame = ((float) ParametersManager::HOPSIZE) / ((float) ParametersManager::SAMPLINGRATE);
    int numberFrames = round(xView / timePerFrame);
    this->trackedSourceBuffer.setNumberFrames(numberFrames);

    // Load colors
    for (int indexColor = 0; indexColor < ParametersManager::SIZECOLORPALETTE; indexColor++)
    {

        QString tmp = "SOURCE" + QString::number((indexColor+1)) + "_COLOR";
        QColor tmpColor = _configurationVector.getElementColor(tmp);

        this->colorsValue[indexColor] = tmpColor;

    }

}

/***********************************************************
* Events                                                   *
***********************************************************/

bool TrackedSourceBufferManager::event(QEvent *event)
{

    if (event->type() == QEvent::User)
    {

        // +-----------------------------------------------+
        // | Add new sources for one frame                 |
        // +-----------------------------------------------+

        TrackedSourceEvent *trackedSourceEvent = dynamic_cast<TrackedSourceEvent*>(event);

        if (trackedSourceEvent != NULL)
        {

            // Add this set of new sources
            TrackedSourceOneFrame currentFrame;

            for (int indexSourceID = 0; indexSourceID < this->sourcesUsedID.size(); indexSourceID++)
            {
                this->matchCounter[indexSourceID]++;
            }

            for (int indexSource = 0; indexSource < trackedSourceEvent->getListSize(); indexSource++)
            {

                TrackedSource currentTrackedSource = trackedSourceEvent->getTrackedSource(indexSource);
                bool alreadyAdded = false;               

                currentFrame.pushSource(currentTrackedSource);

                // If a source exists, then add color
                if (currentFrame.getSourceExists(indexSource) == true)
                {

                    // Check if this source ID was already there
                    for (int indexSourceID = 0; indexSourceID < this->sourcesUsedID.size(); indexSourceID++)
                    {
                        if (this->sourcesUsedID[indexSourceID] == currentTrackedSource.getID())
                        {
                            alreadyAdded = true;
                            this->matchCounter[indexSourceID] = 0;
                            break;
                        }
                    }

                    // If not add it
                    if (alreadyAdded == false)
                    {
                        int newID = this->colorsFreeID.front();
                        this->colorsFreeID.pop_front();
                        this->colorsFreeID.push_back(newID);
                        this->sourcesUsedID.push_back(currentTrackedSource.getID());
                        this->colorsUsedID.push_back(newID);
                        this->matchCounter.push_back(1);
                    }

                    // Check if some match are over
                    int indexMatch = 0;
                    while(indexMatch < this->matchCounter.size())
                    {
                        if (this->matchCounter[indexMatch] > this->trackedSourceBuffer.getNumberFrames())
                        {
                            this->matchCounter.removeAt(indexMatch);
                            this->colorsUsedID.removeAt(indexMatch);
                            this->sourcesUsedID.removeAt(indexMatch);
                        }
                        else
                        {
                            indexMatch++;
                        }
                    }

                }
            }

            this->trackedSourceBuffer.pushSourcesOneFrame(currentFrame);

            // Get the sources to be displayed for this frame

            QList<SourceDisplayed> tmp = currentFrame.getSourcesOneFrame(&this->colorsValue, &this->colorsUsedID, &this->sourcesUsedID);

            // Post an event to the GUI for each receiver

            for (int indexReceiver = 0; indexReceiver < this->listObjectReceiver.size(); indexReceiver++)
            {
                QCoreApplication::postEvent(this->listObjectReceiver[indexReceiver], new UpdateGraphAddSampleEvent(tmp));
            }

        }

        // +-----------------------------------------------+
        // | Resize the graph and send all sources         |
        // +-----------------------------------------------+

        GraphResizeEvent *graphResizeEvent = dynamic_cast<GraphResizeEvent*>(event);

        if (graphResizeEvent != NULL)
        {

            // Get new dimensions
            int width = graphResizeEvent->getWidth();
            int height = graphResizeEvent->getHeight();

            // Get the reference of the object who produced the event
            QObject* objectResized = graphResizeEvent->getObjectResized();

            // Get the sources
            QList < QList<SourceDisplayed> > tmp;

            tmp = this->trackedSourceBuffer.getSourcesAllFrames(&this->colorsValue, &this->colorsUsedID, &this->sourcesUsedID);

            // Post an event to the GUI for the receiver
            QCoreApplication::postEvent(objectResized, new UpdateGraphResizeEvent(tmp, width, height));

        }

        // +-----------------------------------------------+
        // | Clear all sources                             |
        // +-----------------------------------------------+

        GraphClearEvent *graphClearEvent = dynamic_cast<GraphClearEvent*>(event);

        if (graphClearEvent != NULL)
        {

            // Get the sources
            QList < QList<SourceDisplayed> > tmp;

            // Post an event to the GUI for each receiver

            for (int indexReceiver = 0; indexReceiver < this->listObjectReceiver.size(); indexReceiver++)
            {
                QCoreApplication::postEvent(this->listObjectReceiver[indexReceiver], new UpdateGraphRefreshEvent(tmp));
            }

        }

        // +-----------------------------------------------+
        // | Update parameters                             |
        // +-----------------------------------------------+

        UpdateParametersEvent *updateParametersEvent = dynamic_cast<UpdateParametersEvent*>(event);

        if (updateParametersEvent != NULL)
        {

            loadParameters(updateParametersEvent->getConfigurationVector());

            // Get the sources
            QList < QList<SourceDisplayed> > tmp;

            tmp = this->trackedSourceBuffer.getSourcesAllFrames(&this->colorsValue, &this->colorsUsedID, &this->sourcesUsedID);

            // Post an event to the GUI for each receiver in order to refresh
            // these graphs

            for (int indexReceiver = 0; indexReceiver < this->listObjectReceiver.size(); indexReceiver++)
            {
                QCoreApplication::postEvent(this->listObjectReceiver[indexReceiver], new UpdateParametersEvent(*updateParametersEvent));
                QCoreApplication::postEvent(this->listObjectReceiver[indexReceiver], new UpdateGraphRefreshEvent(tmp));
            }

        }


        return true;

    }

    return QObject::event(event);

}

// XxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxX
// x                                                       x
// x Graph update events                                   x
// x                                                       x
// XxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxX

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// % Potential Source: Add sample                          %
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/***********************************************************
* Constructor                                              *
***********************************************************/

UpdateGraphAddSampleEvent::UpdateGraphAddSampleEvent(): QEvent(QEvent::User)
{

}

UpdateGraphAddSampleEvent::UpdateGraphAddSampleEvent(const UpdateGraphAddSampleEvent& _updateGraphEvent): QEvent(QEvent::User)
{
    *this = _updateGraphEvent;
}

UpdateGraphAddSampleEvent::UpdateGraphAddSampleEvent(QList<SourceDisplayed> _sources): QEvent(QEvent::User)
{
    for (int indexSource = 0; indexSource < _sources.size(); indexSource++)
    {
        pushSource(_sources[indexSource]);
    }
}

/***********************************************************
* Destructor                                               *
***********************************************************/

UpdateGraphAddSampleEvent::~UpdateGraphAddSampleEvent()
{

}

/***********************************************************
* Operator                                                 *
***********************************************************/

// +-------------------------------------------------------+
// | Equal                                                 |
// +-------------------------------------------------------+

UpdateGraphAddSampleEvent& UpdateGraphAddSampleEvent::operator=(const UpdateGraphAddSampleEvent& _updateGraphAddSampleEvent)
{

    this->listSources.clear();

    for (int indexSource = 0; indexSource < _updateGraphAddSampleEvent.getNumberSources(); indexSource++)
    {
        this->listSources.push_back(_updateGraphAddSampleEvent.getSource(indexSource));
    }

    return (*this);

}

/***********************************************************
* Accessor                                                 *
***********************************************************/

// +-------------------------------------------------------+
// | Source                                                |
// +-------------------------------------------------------+

SourceDisplayed UpdateGraphAddSampleEvent::getSource(const int _indexSource) const
{
    return (this->listSources[_indexSource]);
}

// +-------------------------------------------------------+
// | Number of sources                                     |
// +-------------------------------------------------------+

int UpdateGraphAddSampleEvent::getNumberSources() const
{
    return (this->listSources.size());
}

// +-------------------------------------------------------+
// | List of sources                                       |
// +-------------------------------------------------------+

QList <SourceDisplayed> UpdateGraphAddSampleEvent::getAllSources() const
{
    return (this->listSources);
}


/***********************************************************
* Mutator                                                  *
***********************************************************/

// +-------------------------------------------------------+
// | Push source                                           |
// +-------------------------------------------------------+

void UpdateGraphAddSampleEvent::pushSource(const SourceDisplayed _source)
{
    this->listSources.push_back(_source);
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// % Potential Source: Resize                              %
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/***********************************************************
* Constructor                                              *
***********************************************************/

UpdateGraphResizeEvent::UpdateGraphResizeEvent(): QEvent(QEvent::User)
{

}

UpdateGraphResizeEvent::UpdateGraphResizeEvent(const UpdateGraphResizeEvent& _updateGraphResizeEvent): QEvent(QEvent::User)
{
    *this = _updateGraphResizeEvent;
}

UpdateGraphResizeEvent::UpdateGraphResizeEvent(QList< QList<SourceDisplayed> > _sources, int _width, int _height): QEvent(QEvent::User)
{
    this->listSources = _sources;
    this->width = _width;
    this->height = _height;
}


/***********************************************************
* Destructor                                               *
***********************************************************/

UpdateGraphResizeEvent::~UpdateGraphResizeEvent()
{

}

/***********************************************************
* Operator                                                 *
***********************************************************/

// +-------------------------------------------------------+
// | Equal                                                 |
// +-------------------------------------------------------+

UpdateGraphResizeEvent& UpdateGraphResizeEvent::operator=(const UpdateGraphResizeEvent& _updateGraphResizeEvent)
{
    this->setSourcesAllFrames(_updateGraphResizeEvent.getSourcesAllFrames());
    this->width = _updateGraphResizeEvent.getWidth();
    this->height = _updateGraphResizeEvent.getHeight();

    return (*this);
}

/***********************************************************
* Accessor                                                 *
***********************************************************/

// +-------------------------------------------------------+
// | List of sources                                       |
// +-------------------------------------------------------+

QList< QList<SourceDisplayed> > UpdateGraphResizeEvent::getSourcesAllFrames() const
{
    return (this->listSources);
}

// +-------------------------------------------------------+
// | Height                                                |
// +-------------------------------------------------------+

int UpdateGraphResizeEvent::getHeight() const
{
    return (this->height);
}

// +-------------------------------------------------------+
// | Width                                                 |
// +-------------------------------------------------------+

int UpdateGraphResizeEvent::getWidth() const
{
    return (this->width);
}

/***********************************************************
* Mutator                                                  *
***********************************************************/

// +-------------------------------------------------------+
// | List of sources                                       |
// +-------------------------------------------------------+

void UpdateGraphResizeEvent::setSourcesAllFrames( QList< QList<SourceDisplayed> > _sources)
{
    this->listSources = _sources;
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// % Refresh graph                                         %
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/***********************************************************
* Constructor                                              *
***********************************************************/

UpdateGraphRefreshEvent::UpdateGraphRefreshEvent(): QEvent(QEvent::User)
{

}

UpdateGraphRefreshEvent::UpdateGraphRefreshEvent(const UpdateGraphResizeEvent& _updateGraphRefreshEvent): QEvent(QEvent::User)
{
    *this = _updateGraphRefreshEvent;
}

UpdateGraphRefreshEvent::UpdateGraphRefreshEvent(QList< QList<SourceDisplayed> > _sources): QEvent(QEvent::User)
{
    this->listSources = _sources;
}

/***********************************************************
* Destructor                                               *
***********************************************************/

UpdateGraphRefreshEvent::~UpdateGraphRefreshEvent()
{

}

/***********************************************************
* Operator                                                 *
***********************************************************/

// +-------------------------------------------------------+
// | Equal                                                 |
// +-------------------------------------------------------+

UpdateGraphRefreshEvent& UpdateGraphRefreshEvent::operator=(const UpdateGraphRefreshEvent& _updateGraphRefreshEvent)
{
    this->setSourcesAllFrames(_updateGraphRefreshEvent.getSourcesAllFrames());

    return (*this);
}

/***********************************************************
* Accessor                                                 *
***********************************************************/

// +-------------------------------------------------------+
// | List of sources                                       |
// +-------------------------------------------------------+

QList< QList<SourceDisplayed> > UpdateGraphRefreshEvent::getSourcesAllFrames() const
{
    return (this->listSources);
}

/***********************************************************
* Mutator                                                  *
***********************************************************/

// +-------------------------------------------------------+
// | List of sources                                       |
// +-------------------------------------------------------+

void UpdateGraphRefreshEvent::setSourcesAllFrames( QList< QList<SourceDisplayed> > _sources)
{
    this->listSources = _sources;
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// % Show graph                                                %
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/***********************************************************
* Constructor                                              *
***********************************************************/

ShowEvent::ShowEvent(): QEvent(QEvent::User)
{

}

ShowEvent::ShowEvent(const ShowEvent& _showEvent): QEvent(QEvent::User)
{
    *this = _showEvent;
}

/***********************************************************
* Destructor                                               *
***********************************************************/

ShowEvent::~ShowEvent()
{

}

/***********************************************************
* Operator                                                 *
***********************************************************/

// +-------------------------------------------------------+
// | Equal                                                 |
// +-------------------------------------------------------+

ShowEvent& ShowEvent::operator=(const ShowEvent& _showEvent)
{
    return (*this);
}



/***********************************************************
* Constructor                                              *
***********************************************************/

GraphResizeEvent::GraphResizeEvent(int _width, int _height, QObject* _objectResized, int _typeSource): QEvent(QEvent::User)
{
    this->width = _width;
    this->height = _height;
    this->objectResized = _objectResized;
    this->typeSource = _typeSource;
}

GraphResizeEvent::GraphResizeEvent(): QEvent(QEvent::User)
{

}

GraphResizeEvent::GraphResizeEvent(const GraphResizeEvent &e): QEvent(QEvent::User)
{
    *this = e;
}

/***********************************************************
* Destructor                                               *
***********************************************************/

GraphResizeEvent::~GraphResizeEvent()
{

}

/***********************************************************
* Operators                                                *
***********************************************************/

// +-------------------------------------------------------+
// | Equal                                                 |
// +-------------------------------------------------------+

GraphResizeEvent& GraphResizeEvent::operator=(const GraphResizeEvent& _graphResizeEvent)
{
    this->width = _graphResizeEvent.getWidth();
    this->height = _graphResizeEvent.getHeight();
    this->objectResized = _graphResizeEvent.getObjectResized();
    this->typeSource = _graphResizeEvent.getTypeSource();

    return (*this);
}

/***********************************************************
* Accessors                                                *
***********************************************************/

// +-------------------------------------------------------+
// | Width                                                 |
// +-------------------------------------------------------+

int GraphResizeEvent::getWidth() const
{
    return (this->width);
}

// +-------------------------------------------------------+
// | Height                                                |
// +-------------------------------------------------------+

int GraphResizeEvent::getHeight() const
{
    return (this->height);
}

// +-------------------------------------------------------+
// | Object resized                                        |
// +-------------------------------------------------------+

QObject* GraphResizeEvent::getObjectResized() const
{
    return (this->objectResized);
}

// +-------------------------------------------------------+
// | Type                                                  |
// +-------------------------------------------------------+

int GraphResizeEvent::getTypeSource() const
{
    return (this->typeSource);
}


// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// % Clear                                                 %
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/***********************************************************
* Constructor                                              *
***********************************************************/

GraphClearEvent::GraphClearEvent(): QEvent(QEvent::User)
{

}

GraphClearEvent::GraphClearEvent(const GraphClearEvent &e): QEvent(QEvent::User)
{
    (*this) = e;
}

/***********************************************************
* Destructor                                               *
***********************************************************/

GraphClearEvent::~GraphClearEvent()
{

}

/***********************************************************
* Operators                                                *
***********************************************************/

// +-------------------------------------------------------+
// | Equal                                                 |
// +-------------------------------------------------------+

GraphClearEvent& GraphClearEvent::operator=(const GraphClearEvent& _graphClearEvent)
{

}
