#ifndef SOURCEBUFFER_H
#define SOURCEBUFFER_H

#include <QObject>
#include <QEvent>
#include <QColor>
#include <QList>
#include <QCoreApplication>
#include <math.h>

#include "parametersManager.h"
#include "sourceEvents.h"



// XxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxX
// x                                                           x
// x Sources to be displayed                                   x
// x                                                           x
// XxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxX

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// % Potential and Tracked sources                             %
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class SourceDisplayed: public QObject
{

public:

    /***********************************************************
    * Constructor                                              *
    ***********************************************************/

    SourceDisplayed();
    SourceDisplayed(const SourceDisplayed& _sourceDisplayed);
    SourceDisplayed(const float _X, const float _Y, const float _Z, const QColor _color);

    /***********************************************************
    * Destructor                                               *
    ***********************************************************/

    ~SourceDisplayed();

    /***********************************************************
    * Operator                                                 *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Equal                                                 |
    // +-------------------------------------------------------+

    SourceDisplayed& operator=(const SourceDisplayed& _sourceDisplayed);

    /***********************************************************
    * Accessor                                                 *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | X                                                     |
    // +-------------------------------------------------------+

    float getX() const;

    // +-------------------------------------------------------+
    // | Y                                                     |
    // +-------------------------------------------------------+

    float getY() const;

    // +-------------------------------------------------------+
    // | Z                                                     |
    // +-------------------------------------------------------+

    float getZ() const;

    // +-------------------------------------------------------+
    // | Color                                                 |
    // +-------------------------------------------------------+

    QColor getColor() const;

    /***********************************************************
    * Mutator                                                  *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | X                                                     |
    // +-------------------------------------------------------+

    void setX(float _X);

    // +-------------------------------------------------------+
    // | Y                                                     |
    // +-------------------------------------------------------+

    void setY(float _Y);

    // +-------------------------------------------------------+
    // | Z                                                     |
    // +-------------------------------------------------------+

    void setZ(float _Z);

    // +-------------------------------------------------------+
    // | Color                                                 |
    // +-------------------------------------------------------+

    void setColor(QColor _color);

private:

    float X;
    float Y;
    float Z;
    QColor color;

};

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// % Tracked Source                                            %
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

// XxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxX
// x                                                           x
// x Sources for one frame                                     x
// x                                                           x
// XxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxX

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// % Potential Source                                          %
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class PotentialSourceOneFrame: public QObject
{

public:

    /***********************************************************
    * Constructor                                              *
    ***********************************************************/

    PotentialSourceOneFrame();
    PotentialSourceOneFrame(const PotentialSourceOneFrame& _potentialSourceOneFrame);

    /***********************************************************
    * Destructor                                               *
    ***********************************************************/

    ~PotentialSourceOneFrame();

    /***********************************************************
    * Operator                                                 *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Equal                                                 |
    // +-------------------------------------------------------+

    PotentialSourceOneFrame& operator=(const PotentialSourceOneFrame& _potentialSourceOneFrame);

    /***********************************************************
    * Accessors                                                *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Number of sources                                     |
    // +-------------------------------------------------------+

    int getNumberSources() const;

    // +-------------------------------------------------------+
    // | Get source (potential)                                |
    // +-------------------------------------------------------+

    PotentialSource getSourcePotential(const int _indexSource) const;

    // +-------------------------------------------------------+
    // | Get source (displayed)                                |
    // +-------------------------------------------------------+

    SourceDisplayed getSourceDisplayed(const int _indexSource, const int _ET) const;

    // +-------------------------------------------------------+
    // | Get all sources (displayed)                           |
    // +-------------------------------------------------------+

    QList<SourceDisplayed> getSourcesOneFrame(const int _ET) const;

    /***********************************************************
    * Mutator                                                  *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Push a new source                                     |
    // +-------------------------------------------------------+

    void pushSource(PotentialSource _newSource);

private:

    QList <PotentialSource> listSources;

};

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// % Tracked Source                                            %
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class TrackedSourceOneFrame: public QObject
{

public:

    /***********************************************************
    * Constructor                                              *
    ***********************************************************/

    TrackedSourceOneFrame();
    TrackedSourceOneFrame(const TrackedSourceOneFrame& _trackedSourceOneFrame);

    /***********************************************************
    * Destructor                                               *
    ***********************************************************/

    ~TrackedSourceOneFrame();

    /***********************************************************
    * Operator                                                 *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Equal                                                 |
    // +-------------------------------------------------------+

    TrackedSourceOneFrame& operator=(const TrackedSourceOneFrame& _trackedSourceOneFrame);

    /***********************************************************
    * Accessors                                                *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Number of sources                                     |
    // +-------------------------------------------------------+

    int getNumberSources() const;

    // +-------------------------------------------------------+
    // | Get source (potential)                                |
    // +-------------------------------------------------------+

    TrackedSource getSourceTracked(const int _indexSource) const;

    // +-------------------------------------------------------+
    // | Get source (displayed)                                |
    // +-------------------------------------------------------+

    SourceDisplayed getSourceDisplayed(const int _indexSource, QList<QColor>* _listColors, QList<int>* _listColorIDs, QList<int>* _listSourceIDs) const;

    // +-------------------------------------------------------+
    // | Get source exists                                     |
    // +-------------------------------------------------------+

    bool getSourceExists(const int _indexSource) const;

    // +-------------------------------------------------------+
    // | Get all sources (displayed)                           |
    // +-------------------------------------------------------+

    QList<SourceDisplayed> getSourcesOneFrame(QList<QColor>* _listColors, QList<int>* _listColorIDs, QList<int>* _listSourceIDs) const;

    /***********************************************************
    * Mutator                                                  *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Push a new source                                     |
    // +-------------------------------------------------------+

    void pushSource(TrackedSource _newSource);

private:

    QList <TrackedSource> listSources;

};

// XxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxX
// x                                                           x
// x Sources for all frame                                     x
// x                                                           x
// XxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxX

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// % Potential Source                                          %
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class PotentialSourceAllFrames: public QObject
{

public:

    /***********************************************************
    * Constructor                                              *
    ***********************************************************/

    PotentialSourceAllFrames();
    PotentialSourceAllFrames(const PotentialSourceAllFrames& _potentialSourceAllFrames);

    /***********************************************************
    * Destructor                                               *
    ***********************************************************/

    ~PotentialSourceAllFrames();

    /***********************************************************
    * Operator                                                 *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Equal                                                 |
    // +-------------------------------------------------------+

    PotentialSourceAllFrames& operator=(const PotentialSourceAllFrames& _potentialSourceAllFrames);

    /***********************************************************
    * Accessors                                                *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Number of frames                                      |
    // +-------------------------------------------------------+

    int getNumberFrames() const;

    // +-------------------------------------------------------+
    // | Get displayed sources for all frames                  |
    // +-------------------------------------------------------+

    QList< QList<SourceDisplayed> > getSourcesAllFrames(const float _ET) const;

    // +-------------------------------------------------------+
    // | Get potential sources at specified frame              |
    // +-------------------------------------------------------+

    PotentialSourceOneFrame getSourcesAtFrame(const int _indexFrame) const;

    /***********************************************************
    * Mutator                                                  *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Number of frames                                      |
    // +-------------------------------------------------------+

    void setNumberFrames(const int _numberFrames);

    // +-------------------------------------------------------+
    // | Push sources for one frame                            |
    // +-------------------------------------------------------+

    void pushSourcesOneFrame(PotentialSourceOneFrame _sources);

private:

    /***********************************************************
    * Fields                                                   *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Buffer of sources                                     |
    // +-------------------------------------------------------+

    QList<PotentialSourceOneFrame> listSources;

    int indexFrame;

};

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// % Tracked Source                                            %
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class TrackedSourceAllFrames: public QObject
{

public:

    /***********************************************************
    * Constructor                                              *
    ***********************************************************/

    TrackedSourceAllFrames();
    TrackedSourceAllFrames(const TrackedSourceAllFrames& _trackedSourceAllFrames);

    /***********************************************************
    * Destructor                                               *
    ***********************************************************/

    ~TrackedSourceAllFrames();

    /***********************************************************
    * Operator                                                 *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Equal                                                 |
    // +-------------------------------------------------------+

    TrackedSourceAllFrames& operator=(const TrackedSourceAllFrames& _trackedSourceAllFrames);

    /***********************************************************
    * Accessors                                                *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Number of frames                                      |
    // +-------------------------------------------------------+

    int getNumberFrames() const;

    // +-------------------------------------------------------+
    // | Get displayed sources for all frames                  |
    // +-------------------------------------------------------+

    QList< QList<SourceDisplayed> > getSourcesAllFrames(QList<QColor>* _listColors, QList<int>* _listColorIDs, QList<int>* _listSourceIDs) const;

    // +-------------------------------------------------------+
    // | Get potential sources at specified frame              |
    // +-------------------------------------------------------+

    TrackedSourceOneFrame getSourcesAtFrame(const int _indexFrame) const;

    /***********************************************************
    * Mutator                                                  *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Number of frames                                      |
    // +-------------------------------------------------------+

    void setNumberFrames(const int _numberFrames);

    // +-------------------------------------------------------+
    // | Push sources for one frame                            |
    // +-------------------------------------------------------+

    void pushSourcesOneFrame(TrackedSourceOneFrame _sources);

private:

    /***********************************************************
    * Fields                                                   *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Buffer of sources                                     |
    // +-------------------------------------------------------+

    QList<TrackedSourceOneFrame> listSources;

    int indexFrame;

};

// XxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxX
// x                                                           x
// x Sources Buffers Manager                                   x
// x                                                           x
// XxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxX

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// % Potential Source                                          %
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class PotentialSourceBufferManager: public QObject
{

public:

    /***********************************************************
    * Constructor                                              *
    ***********************************************************/

    PotentialSourceBufferManager();
    PotentialSourceBufferManager(const PotentialSourceBufferManager& _potentialSourceBufferManager);

    /***********************************************************
    * Destructor                                               *
    ***********************************************************/

    ~PotentialSourceBufferManager();

    /***********************************************************
    * Operator                                                 *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Equal                                                 |
    // +-------------------------------------------------------+

    PotentialSourceBufferManager& operator=(const PotentialSourceBufferManager& _potentialSourceBufferManager);

    /***********************************************************
    * Event receivers                                          *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Push                                                  |
    // +-------------------------------------------------------+

    void pushEventReceiver(QObject* _receiver);

    /***********************************************************
    * Accessors                                                *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | List of all sources                                   |
    // +-------------------------------------------------------+

    PotentialSourceAllFrames getPotentialSourceBuffer() const;

    /***********************************************************
    * Mutators                                                 *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Load parameters                                       |
    // +-------------------------------------------------------+

    void loadParametersView(ConfigurationVector _configurationVector);
    void loadParametersSystem(ConfigurationVector _configurationVector);

protected:

    /***********************************************************
    * Events                                                   *
    ***********************************************************/

    bool event(QEvent *event);

private:

    /***********************************************************
    * Fields                                                   *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Buffer of sources                                     |
    // +-------------------------------------------------------+

    PotentialSourceAllFrames potentialSourceBuffer;

    // +-------------------------------------------------------+
    // | Energy level                                          |
    // +-------------------------------------------------------+

    float ET;

    // +-------------------------------------------------------+
    // | Event receivers                                       |
    // +-------------------------------------------------------+

    QList<QObject*> listObjectReceiver;

};

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// % Tracked Source                                            %
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class TrackedSourceBufferManager: public QObject
{

public:

    /***********************************************************
    * Constructor                                              *
    ***********************************************************/

    TrackedSourceBufferManager();
    TrackedSourceBufferManager(const TrackedSourceBufferManager& _trackedSourceBufferManager);

    /***********************************************************
    * Destructor                                               *
    ***********************************************************/

    ~TrackedSourceBufferManager();

    /***********************************************************
    * Operator                                                 *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Equal                                                 |
    // +-------------------------------------------------------+

    TrackedSourceBufferManager& operator=(const TrackedSourceBufferManager& _trackedSourceBufferManager);

    /***********************************************************
    * Event receivers                                          *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Push                                                  |
    // +-------------------------------------------------------+

    void pushEventReceiver(QObject* _receiver);

    /***********************************************************
    * Accessors                                                *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | List of all sources                                   |
    // +-------------------------------------------------------+

    TrackedSourceAllFrames getTrackedSourceBuffer() const;

    /***********************************************************
    * Mutators                                                 *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Load parameters                                       |
    // +-------------------------------------------------------+

    void loadParameters(ConfigurationVector _configurationVector);

protected:

    /***********************************************************
    * Events                                                   *
    ***********************************************************/

    bool event(QEvent *event);

private:

    /***********************************************************
    * Fields                                                   *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Buffer of sources                                     |
    // +-------------------------------------------------------+

    TrackedSourceAllFrames trackedSourceBuffer;

    // +-------------------------------------------------------+
    // | Event receivers                                       |
    // +-------------------------------------------------------+

    QList<QObject*> listObjectReceiver;

    // +-------------------------------------------------------+
    // | Colors                                                |
    // +-------------------------------------------------------+

    QList<int> colorsFreeID;
    QList<int> colorsUsedID;
    QList<QColor> colorsValue;
    QList<int> sourcesUsedID;
    QList<int> matchCounter;

};

// XxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxX
// x                                                           x
// x Graph Events                                              x
// x                                                           x
// XxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxX

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// % Resize                                                    %
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class GraphResizeEvent: public QEvent
{

public:

    /***********************************************************
    * Constructor                                              *
    ***********************************************************/

    GraphResizeEvent(int _width, int _height, QObject* _objectResized, int _typeSource);

    GraphResizeEvent();

    GraphResizeEvent(const GraphResizeEvent &e);

    /***********************************************************
    * Destructor                                               *
    ***********************************************************/

    ~GraphResizeEvent();

    /***********************************************************
    * Constant                                                 *
    ***********************************************************/

    static const int POTENTIAL = 1;
    static const int TRACKED = 2;

    /***********************************************************
    * Operators                                                *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Equal                                                 |
    // +-------------------------------------------------------+

    GraphResizeEvent& operator=(const GraphResizeEvent& _graphResizeEvent);

    /***********************************************************
    * Accessors                                                *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Width                                                 |
    // +-------------------------------------------------------+

    int getWidth() const;

    // +-------------------------------------------------------+
    // | Height                                                |
    // +-------------------------------------------------------+

    int getHeight() const;

    // +-------------------------------------------------------+
    // | Object resized                                        |
    // +-------------------------------------------------------+

    QObject* getObjectResized() const;

    // +-------------------------------------------------------+
    // | Type                                                  |
    // +-------------------------------------------------------+

    int getTypeSource() const;

private:

    /***********************************************************
    * Fields                                                   *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Width                                                 |
    // +-------------------------------------------------------+

    int width;

    // +-------------------------------------------------------+
    // | Height                                                |
    // +-------------------------------------------------------+

    int height;

    // +-------------------------------------------------------+
    // | Object resized                                        |
    // +-------------------------------------------------------+

    QObject* objectResized;

    // +-------------------------------------------------------+
    // | Type                                                  |
    // +-------------------------------------------------------+

    int typeSource;

};



// XxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxX
// x                                                           x
// x Graph update events                                       x
// x                                                           x
// XxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxX

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// % Add one sample                                            %
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class UpdateGraphAddSampleEvent: public QEvent
{

public:

    /***********************************************************
    * Constructor                                              *
    ***********************************************************/

    UpdateGraphAddSampleEvent();
    UpdateGraphAddSampleEvent(const UpdateGraphAddSampleEvent& _updateGraphAddSampleEvent);
    UpdateGraphAddSampleEvent(QList<SourceDisplayed> _sources);


    /***********************************************************
    * Destructor                                               *
    ***********************************************************/

    ~UpdateGraphAddSampleEvent();

    /***********************************************************
    * Operator                                                 *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Equal                                                 |
    // +-------------------------------------------------------+

    UpdateGraphAddSampleEvent& operator=(const UpdateGraphAddSampleEvent& _updateGraphAddSampleEvent);

    /***********************************************************
    * Accessor                                                 *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Source                                                |
    // +-------------------------------------------------------+

    SourceDisplayed getSource(const int _indexSource) const;

    // +-------------------------------------------------------+
    // | Number of sources                                     |
    // +-------------------------------------------------------+

    int getNumberSources() const;

    // +-------------------------------------------------------+
    // | List of sources                                       |
    // +-------------------------------------------------------+

    QList <SourceDisplayed> getAllSources() const;

    /***********************************************************
    * Mutator                                                  *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Push source                                           |
    // +-------------------------------------------------------+

    void pushSource(const SourceDisplayed _source);


private:

    /***********************************************************
    * Fields                                                   *
    ***********************************************************/

    QList<SourceDisplayed> listSources;

};

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// % Resize                                                    %
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class UpdateGraphResizeEvent: public QEvent
{

public:

    /***********************************************************
    * Constructor                                              *
    ***********************************************************/

    UpdateGraphResizeEvent();
    UpdateGraphResizeEvent(const UpdateGraphResizeEvent& _updateGraphResizeEvent);
    UpdateGraphResizeEvent(QList< QList<SourceDisplayed> > _sources, int _width, int _height);

    /***********************************************************
    * Destructor                                               *
    ***********************************************************/

    ~UpdateGraphResizeEvent();

    /***********************************************************
    * Operator                                                 *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Equal                                                 |
    // +-------------------------------------------------------+

    UpdateGraphResizeEvent& operator=(const UpdateGraphResizeEvent& _updateGraphResizeEvent);

    /***********************************************************
    * Accessor                                                 *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | List of sources                                       |
    // +-------------------------------------------------------+

    QList< QList<SourceDisplayed> > getSourcesAllFrames() const;

    // +-------------------------------------------------------+
    // | Height                                                |
    // +-------------------------------------------------------+

    int getHeight() const;

    // +-------------------------------------------------------+
    // | Width                                                 |
    // +-------------------------------------------------------+

    int getWidth() const;

    /***********************************************************
    * Mutator                                                  *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | List of sources                                       |
    // +-------------------------------------------------------+

    void setSourcesAllFrames( QList< QList<SourceDisplayed> > _sources);

private:

    /***********************************************************
    * Fields                                                   *
    ***********************************************************/

    QList < QList<SourceDisplayed> > listSources;

    int width;

    int height;

};

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// % Refresh graph                                             %
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class UpdateGraphRefreshEvent: public QEvent
{

public:

    /***********************************************************
    * Constructor                                              *
    ***********************************************************/

    UpdateGraphRefreshEvent();
    UpdateGraphRefreshEvent(const UpdateGraphResizeEvent& _updateGraphRefreshEvent);
    UpdateGraphRefreshEvent(QList< QList<SourceDisplayed> > _sources);

    /***********************************************************
    * Destructor                                               *
    ***********************************************************/

    ~UpdateGraphRefreshEvent();

    /***********************************************************
    * Operator                                                 *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Equal                                                 |
    // +-------------------------------------------------------+

    UpdateGraphRefreshEvent& operator=(const UpdateGraphRefreshEvent& _updateGraphRefreshEvent);

    /***********************************************************
    * Accessor                                                 *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | List of sources                                       |
    // +-------------------------------------------------------+

    QList< QList<SourceDisplayed> > getSourcesAllFrames() const;

    /***********************************************************
    * Mutator                                                  *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | List of sources                                       |
    // +-------------------------------------------------------+

    void setSourcesAllFrames( QList< QList<SourceDisplayed> > _sources);

private:

    /***********************************************************
    * Fields                                                   *
    ***********************************************************/

    QList < QList<SourceDisplayed> > listSources;

};

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// % Show graph                                                %
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class ShowEvent: public QEvent
{

public:

    /***********************************************************
    * Constructor                                              *
    ***********************************************************/

    ShowEvent();
    ShowEvent(const ShowEvent& _showEvent);

    /***********************************************************
    * Destructor                                               *
    ***********************************************************/

    ~ShowEvent();

    /***********************************************************
    * Operator                                                 *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Equal                                                 |
    // +-------------------------------------------------------+

    ShowEvent& operator=(const ShowEvent& _showEvent);

};

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// % Clear                                                     %
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class GraphClearEvent: public QEvent
{

public:

    /***********************************************************
    * Constructor                                              *
    ***********************************************************/

    GraphClearEvent();

    GraphClearEvent(const GraphClearEvent &e);

    /***********************************************************
    * Destructor                                               *
    ***********************************************************/

    ~GraphClearEvent();

    /***********************************************************
    * Operators                                                *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Equal                                                 |
    // +-------------------------------------------------------+

    GraphClearEvent& operator=(const GraphClearEvent& _graphClearEvent);

};

#endif
