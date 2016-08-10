#ifndef TRACKEDSOURCEMANAGER_H
#define TRACKEDSOURCEMANAGER_H

#include <QVector>
#include <QMutex>
#include <QColor>
#include <math.h>

#include "colorPalette.h"
#include "parametersManager.h"

class TrackedSourceManager
{

public:

    /***********************************************************
    * Constructor                                              *
    ***********************************************************/

    TrackedSourceManager(ParametersManager* _parametersManager);

    /***********************************************************
    * Destructor                                               *
    ***********************************************************/

    ~TrackedSourceManager();

    /***********************************************************
    * Parameters                                               *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Init object with current parameters                   |
    // +-------------------------------------------------------+

    void init(const float _timeBuffer, const float _framePerSec, const int _maxTrackedSources, const int _sizeColorPalette);

    // +-------------------------------------------------------+
    // | Initialize again object with current parameters       |
    // +-------------------------------------------------------+

    void reinit(const float _timeBuffer, const float _framePerSec, const int _maxTrackedSources, const int _sizeColorPalette);

    // +-------------------------------------------------------+
    // | Update parameters                                     |
    // +-------------------------------------------------------+

    void updateParameters(ParametersManager* _parametersManager);

    /***********************************************************
    * Add new tracked sources                                  *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Begin filling the vector for the current frame        |
    // +-------------------------------------------------------+

    void writeBegin();

    // +-------------------------------------------------------+
    // | Add a new potential source in the vector              |
    // +-------------------------------------------------------+

    void write(const float _X, const float _Y, const float _Z, const int _ID);

    // +-------------------------------------------------------+
    // | End filling the vector for the current frame          |
    // +-------------------------------------------------------+

    void writeEnd();

    /***********************************************************
    * Read potential sources                                   *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Begin reading                                         |
    // +-------------------------------------------------------+

    void readBeginForAll();
    void readBegin();

    // +-------------------------------------------------------+
    // | Read a potential sources                              |
    // +-------------------------------------------------------+

    float readX(const int _indexSource);
    float readY(const int _indexSource);
    float readZ(const int _indexSource);
    int readID(const int _indexSource);
    int readColor(const int _indexSource);
    QColor readColorValue(const int _colorID);
    bool readExists(const int _indexSource);

    // +-------------------------------------------------------+
    // | End reading                                           |
    // +-------------------------------------------------------+

    void readEnd();
    void readEndForAll();

    /***********************************************************
    * Read all potential sources                               *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Begin reading                                         |
    // +-------------------------------------------------------+

    void readAllBegin();

    // +-------------------------------------------------------+
    // | Read a potential sources                              |
    // +-------------------------------------------------------+

    float readAllX(const int _indexSource);
    float readAllY(const int _indexSource);
    float readAllZ(const int _indexSource);
    int readAllID(const int _indexSource);
    int readAllColor(const int _indexSource);
    QColor readAllColorValue(const int _colorID);
    bool readAllExists(const int _indexSource);
    bool readAllNext();

    // +-------------------------------------------------------+
    // | End reading                                           |
    // +-------------------------------------------------------+

    void readAllEnd();

private:

    /***********************************************************
    * Modify colors                                            *
    ***********************************************************/

    void replaceColor(const int _colorID, const QColor _color);

    /***********************************************************
    * Fields                                                   *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Color management                                      |
    // +-------------------------------------------------------+

    QVector <int> pastColorID;
    QVector <int> pastSourceID;
    QVector <int> newColorID;
    QVector <int> newSourceID;

    // +-------------------------------------------------------+
    // | Size of the buffer and pointer to the current index   |
    // +-------------------------------------------------------+

    int sizeBuffer;
    int indexBufferWrite;
    int indexBufferRead;
    int indexBufferReadAll;
    int indexBufferReadAllStart;
    int maxTrackedSources;
    int indexTrackedSource;

    // +-------------------------------------------------------+
    // | Buffer that stores the all potential sources vectors  |
    // +-------------------------------------------------------+

    float **bufferWriteX;
    float **bufferWriteY;
    float **bufferWriteZ;
    int **bufferWriteID;
    int **bufferWriteColor;
    bool **bufferWriteTrackedSourceExists;
    float **bufferReadX;
    float **bufferReadY;
    float **bufferReadZ;
    int **bufferReadID;
    int **bufferReadColor;
    bool **bufferReadTrackedSourceExists;

    // +-------------------------------------------------------+
    // | List of colors                                        |
    // +-------------------------------------------------------+

    QVector<QColor> potentialColors;

    // +-------------------------------------------------------+
    // | Semaphore for reading / writing content               |
    // +-------------------------------------------------------+

    QMutex mutexReadWrite;

    // +-------------------------------------------------------+
    // | Semaphore for writing content                         |
    // +-------------------------------------------------------+

    QMutex mutexWritingInProgress;

    // +-------------------------------------------------------+
    // | Semaphore for reading content                         |
    // +-------------------------------------------------------+

    QMutex mutexReadingInProgress;

    // +-------------------------------------------------------+
    // | Semaphore for reading all content                     |
    // +-------------------------------------------------------+

    QMutex mutexReadingAllInProgress;

    // +-------------------------------------------------------+
    // | Flag to authorize write operations                    |
    // +-------------------------------------------------------+

    bool writingAllowed;

    // +-------------------------------------------------------+
    // | Flag to authorize read operations for all graphs      |
    // +-------------------------------------------------------+

    bool readingAllowedForAll;

    // +-------------------------------------------------------+
    // | Flag to authorize read operations for current graph   |
    // +-------------------------------------------------------+

    bool readingAllowed;

    // +-------------------------------------------------------+
    // | Flag to authorize read all frames in the buffer       |
    // +-------------------------------------------------------+

    bool readingAllAllowed;

    // +-------------------------------------------------------+
    // | Sources colors                                        |
    // +-------------------------------------------------------+

    ColorPalette *colorPalette;

};

#endif
