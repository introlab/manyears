#ifndef POTENTIALSOURCEMANAGER_H
#define POTENTIALSOURCEMANAGER_H

#include <QVector>
#include <QMutex>
#include <QColor>
#include <math.h>

#include "parametersManager.h"

class PotentialSourceManager
{

public:

    /***********************************************************
    * Constructor                                              *
    ***********************************************************/

    PotentialSourceManager(ParametersManager* _parametersManager);

    /***********************************************************
    * Destructor                                               *
    ***********************************************************/

    ~PotentialSourceManager();

    /***********************************************************
    * Parameters                                               *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Init object with current parameters                   |
    // +-------------------------------------------------------+

    void init(const float _timeBuffer, const float _framePerSec, const int _maxPotentialSources, const float _ET);

    // +-------------------------------------------------------+
    // | Initialize again object with current parameters       |
    // +-------------------------------------------------------+

    void reinit(const float _timeBuffer, const float _framePerSec, const int _maxPotentialSources, const float _ET);

    // +-------------------------------------------------------+
    // | Update parameters                                     |
    // +-------------------------------------------------------+

    void updateParameters(ParametersManager* _parametersManager);

    /***********************************************************
    * Add new potential sources                                *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Begin filling the vector for the current frame        |
    // +-------------------------------------------------------+

    void writeBegin();

    // +-------------------------------------------------------+
    // | Add a new potential source in the vector              |
    // +-------------------------------------------------------+

    void write(const float _X, const float _Y, const float _Z, const float _E);

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
    float readE(const int _indexSource);
    QColor readColor(const int _indexSource);
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
    float readAllE(const int _indexSource);
    QColor readAllColor(const int _indexSource);
    bool readAllExists(const int _indexSource);
    bool readAllNext();

    // +-------------------------------------------------------+
    // | End reading                                           |
    // +-------------------------------------------------------+

    void readAllEnd();

private:

    /***********************************************************
    * Color management                                         *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Get the potential source color                        |
    // +-------------------------------------------------------+

    QColor getPotentialSourceColor(const float _E, const int _indexSource);

    /***********************************************************
    * Fields                                                   *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Size of the buffer and pointer to the current index   |
    // +-------------------------------------------------------+

    int sizeBuffer;
    int indexBufferWrite;
    int indexBufferRead;
    int indexBufferReadAll;
    int indexBufferReadAllStart;
    int maxPotentialSources;
    int indexPotentialSource;

    // +-------------------------------------------------------+
    // | Buffer that stores the all potential sources vectors  |
    // +-------------------------------------------------------+

    float **bufferWriteX;
    float **bufferWriteY;
    float **bufferWriteZ;
    float **bufferWriteE;
    QColor **bufferWriteColor;
    bool **bufferWritePotentialSourceExists;
    float **bufferReadX;
    float **bufferReadY;
    float **bufferReadZ;
    float **bufferReadE;
    QColor **bufferReadColor;
    bool **bufferReadPotentialSourceExists;

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
    // | Potential sources: energy                             |
    // +-------------------------------------------------------+

    float ET;
    int numberColorLevels;

};

#endif
