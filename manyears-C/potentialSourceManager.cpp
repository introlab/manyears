#include "potentialSourceManager.h"

/***********************************************************
* Constructor                                              *
***********************************************************/

PotentialSourceManager::PotentialSourceManager(ParametersManager* _parametersManager)
{

    float timeBuffer;
    float framePerSec;
    int maxPotentialSources;
    float ET;

    timeBuffer = _parametersManager->getPropertyViewFloat("TIME_BUFFER");
    framePerSec = ((float) ParametersManager::SAMPLINGRATE) / ((float) ParametersManager::HOPSIZE);
    maxPotentialSources = _parametersManager->getPropertySystemInt("BEAMFORMER_MAXPOTENTIAL");
    ET = _parametersManager->getPropertySystemFloat("MIXTURE_ET");

    init(timeBuffer, framePerSec, maxPotentialSources, ET);

}

/***********************************************************
* Destructor                                               *
***********************************************************/

PotentialSourceManager::~PotentialSourceManager()
{

    // Wait for all operations to be completed
    this->mutexReadingInProgress.lock();
    this->mutexReadingInProgress.unlock();

    this->mutexWritingInProgress.lock();
    this->mutexWritingInProgress.unlock();

    this->mutexReadWrite.lock();
    this->mutexReadWrite.unlock();

    // Delete buffers

        for (int indexBuffer = 0; indexBuffer < this->sizeBuffer; indexBuffer++)
        {
            delete [] this->bufferWriteX[indexBuffer];
            delete [] this->bufferWriteY[indexBuffer];
            delete [] this->bufferWriteZ[indexBuffer];
            delete [] this->bufferWriteE[indexBuffer];
            delete [] this->bufferWriteColor[indexBuffer];
            delete [] this->bufferWritePotentialSourceExists[indexBuffer];
            delete [] this->bufferReadX[indexBuffer];
            delete [] this->bufferReadY[indexBuffer];
            delete [] this->bufferReadZ[indexBuffer];
            delete [] this->bufferReadE[indexBuffer];
            delete [] this->bufferReadColor[indexBuffer];
            delete [] this->bufferReadPotentialSourceExists[indexBuffer];
        }

        delete [] this->bufferWriteX;
        delete [] this->bufferWriteY;
        delete [] this->bufferWriteZ;
        delete [] this->bufferWriteE;
        delete [] this->bufferWriteColor;
        delete [] this->bufferWritePotentialSourceExists;
        delete [] this->bufferReadX;
        delete [] this->bufferReadY;
        delete [] this->bufferReadZ;
        delete [] this->bufferReadE;
        delete [] this->bufferReadColor;
        delete [] this->bufferReadPotentialSourceExists;

}

/***********************************************************
* Parameters                                               *
***********************************************************/

// +-------------------------------------------------------+
// | Init object with current parameters                   |
// +-------------------------------------------------------+

void PotentialSourceManager::init(const float _timeBuffer, const float _framePerSec, const int _maxPotentialSources, const float _ET)
{

    // +---------------------------------------------------+
    // | Save parameters                                   |
    // +---------------------------------------------------+

    // Save the size
    this->sizeBuffer = (int) (round(_timeBuffer * _framePerSec));

    // Save number of sources per frame
    this->maxPotentialSources = _maxPotentialSources;

    // +---------------------------------------------------+
    // | Initialize buffers                                |
    // +---------------------------------------------------+

    this->indexBufferWrite = 0;
    this->indexBufferRead = 0;
    this->bufferWriteX = new float* [this->sizeBuffer];
    this->bufferWriteY = new float* [this->sizeBuffer];
    this->bufferWriteZ = new float* [this->sizeBuffer];
    this->bufferWriteE = new float* [this->sizeBuffer];
    this->bufferWriteColor = new QColor* [this->sizeBuffer];
    this->bufferWritePotentialSourceExists = new bool* [this->sizeBuffer];
    this->bufferReadX = new float* [this->sizeBuffer];
    this->bufferReadY = new float* [this->sizeBuffer];
    this->bufferReadZ = new float* [this->sizeBuffer];
    this->bufferReadE = new float* [this->sizeBuffer];
    this->bufferReadColor = new QColor* [this->sizeBuffer];
    this->bufferReadPotentialSourceExists = new bool* [this->sizeBuffer];

    // Fill buffer with no potential sources
    for (int indexBuffer = 0; indexBuffer < this->sizeBuffer; indexBuffer++)
    {
        this->bufferWriteX[indexBuffer] = new float [this->maxPotentialSources];
        this->bufferWriteY[indexBuffer] = new float [this->maxPotentialSources];
        this->bufferWriteZ[indexBuffer] = new float [this->maxPotentialSources];
        this->bufferWriteE[indexBuffer] = new float [this->maxPotentialSources];
        this->bufferWriteColor[indexBuffer] = new QColor [this->maxPotentialSources];
        this->bufferWritePotentialSourceExists[indexBuffer] = new bool [this->maxPotentialSources];
        this->bufferReadX[indexBuffer] = new float [this->maxPotentialSources];
        this->bufferReadY[indexBuffer] = new float [this->maxPotentialSources];
        this->bufferReadZ[indexBuffer] = new float [this->maxPotentialSources];
        this->bufferReadE[indexBuffer] = new float [this->maxPotentialSources];
        this->bufferReadColor[indexBuffer] = new QColor [this->maxPotentialSources];
        this->bufferReadPotentialSourceExists[indexBuffer] = new bool [this->maxPotentialSources];

        for (int indexPotSource = 0; indexPotSource < this->maxPotentialSources; indexPotSource++)
        {

            this->bufferWriteX[indexBuffer][indexPotSource] = 0.0;
            this->bufferWriteY[indexBuffer][indexPotSource] = 0.0;
            this->bufferWriteZ[indexBuffer][indexPotSource] = 0.0;
            this->bufferWriteE[indexBuffer][indexPotSource] = 0.0;
            this->bufferWriteColor[indexBuffer][indexPotSource] = QColor();
            this->bufferWritePotentialSourceExists[indexBuffer][indexPotSource] = false;
            this->bufferReadX[indexBuffer][indexPotSource] = 0.0;
            this->bufferReadY[indexBuffer][indexPotSource] = 0.0;
            this->bufferReadZ[indexBuffer][indexPotSource] = 0.0;
            this->bufferReadE[indexBuffer][indexPotSource] = 0.0;
            this->bufferReadColor[indexBuffer][indexPotSource] = QColor();
            this->bufferReadPotentialSourceExists[indexBuffer][indexPotSource] = false;

        }
    }

    // +---------------------------------------------------+
    // | Potential source energy                           |
    // +---------------------------------------------------+

    this->ET = _ET;

    // +---------------------------------------------------+
    // | Potential source colors                           |
    // +---------------------------------------------------+

    this->numberColorLevels = 100;

    this->potentialColors.clear();

    for (int indexLevel = 0; indexLevel < this->numberColorLevels; indexLevel++)
    {

        int red = 255.0 * (((float) indexLevel) / ((float) (this->numberColorLevels - 1)));
        int blue = 255.0 - red;

        this->potentialColors.push_back(QColor(red,0x00,blue));

    }

    // +---------------------------------------------------+
    // | Flags                                             |
    // +---------------------------------------------------+

    this->writingAllowed = false;
    this->readingAllowed = false;
    this->readingAllowedForAll = false;
    this->readingAllAllowed = false;

}

// +-------------------------------------------------------+
// | Initialize again object with current parameters       |
// +-------------------------------------------------------+

void PotentialSourceManager::reinit(const float _timeBuffer, const float _framePerSec, const int _maxPotentialSources, const float _ET)
{

    // Protect from read / write to buffers
    this->mutexWritingInProgress.lock();
    this->mutexReadingInProgress.lock();
    this->mutexReadingAllInProgress.lock();
    this->mutexReadWrite.lock();

    // +---------------------------------------------------+
    // | Save previous parameters                          |
    // +---------------------------------------------------+

    // Indexes
    int prevSizeBuffer = this->sizeBuffer;
    int prevIndexBufferWrite1 = this->indexBufferWrite;
    int prevIndexBufferWrite2 = this->indexBufferWrite;
    int prevIndexDifference = this->indexBufferWrite - this->indexBufferRead;
    int prevMaxPotentialSources = this->maxPotentialSources;

    // Wrap around if needed
    if (prevIndexDifference < 0)
    {
        prevIndexDifference += prevSizeBuffer;
    }

    // Buffers pointers
    float** prevBufferWriteX = this->bufferWriteX;
    float** prevBufferWriteY = this->bufferWriteY;
    float** prevBufferWriteZ = this->bufferWriteZ;
    float** prevBufferWriteE = this->bufferWriteE;
    QColor** prevBufferWriteColor = this->bufferWriteColor;
    bool** prevBufferWritePotentialSourceExists = this->bufferWritePotentialSourceExists;
    float** prevBufferReadX = this->bufferReadX;
    float** prevBufferReadY = this->bufferReadY;
    float** prevBufferReadZ = this->bufferReadZ;
    float** prevBufferReadE = this->bufferReadE;
    QColor** prevBufferReadColor = this->bufferReadColor;
    bool** prevBufferReadPotentialSourceExists = this->bufferReadPotentialSourceExists;

    // +---------------------------------------------------+
    // | Create new buffer                                 |
    // +---------------------------------------------------+

    // New size
    this->sizeBuffer = (int) (round(_timeBuffer * _framePerSec));

    // New number of sources per frame
    this->maxPotentialSources = _maxPotentialSources;

    // Create new buffers
    this->bufferWriteX = new float* [this->sizeBuffer];
    this->bufferWriteY = new float* [this->sizeBuffer];
    this->bufferWriteZ = new float* [this->sizeBuffer];
    this->bufferWriteE = new float* [this->sizeBuffer];
    this->bufferWriteColor = new QColor* [this->sizeBuffer];
    this->bufferWritePotentialSourceExists = new bool* [this->sizeBuffer];
    this->bufferReadX = new float* [this->sizeBuffer];
    this->bufferReadY = new float* [this->sizeBuffer];
    this->bufferReadZ = new float* [this->sizeBuffer];
    this->bufferReadE = new float* [this->sizeBuffer];
    this->bufferReadColor = new QColor* [this->sizeBuffer];
    this->bufferReadPotentialSourceExists = new bool* [this->sizeBuffer];

    // Fill buffer with no potential sources
    for (int indexBuffer = 0; indexBuffer < this->sizeBuffer; indexBuffer++)
    {
        this->bufferWriteX[indexBuffer] = new float [this->maxPotentialSources];
        this->bufferWriteY[indexBuffer] = new float [this->maxPotentialSources];
        this->bufferWriteZ[indexBuffer] = new float [this->maxPotentialSources];
        this->bufferWriteE[indexBuffer] = new float [this->maxPotentialSources];
        this->bufferWriteColor[indexBuffer] = new QColor [this->maxPotentialSources];
        this->bufferWritePotentialSourceExists[indexBuffer] = new bool [this->maxPotentialSources];
        this->bufferReadX[indexBuffer] = new float [this->maxPotentialSources];
        this->bufferReadY[indexBuffer] = new float [this->maxPotentialSources];
        this->bufferReadZ[indexBuffer] = new float [this->maxPotentialSources];
        this->bufferReadE[indexBuffer] = new float [this->maxPotentialSources];
        this->bufferReadColor[indexBuffer] = new QColor [this->maxPotentialSources];
        this->bufferReadPotentialSourceExists[indexBuffer] = new bool [this->maxPotentialSources];

        for (int indexPotSource = 0; indexPotSource < this->maxPotentialSources; indexPotSource++)
        {

            this->bufferWriteX[indexBuffer][indexPotSource] = 0.0;
            this->bufferWriteY[indexBuffer][indexPotSource] = 0.0;
            this->bufferWriteZ[indexBuffer][indexPotSource] = 0.0;
            this->bufferWriteE[indexBuffer][indexPotSource] = 0.0;
            this->bufferWriteColor[indexBuffer][indexPotSource] = QColor();
            this->bufferWritePotentialSourceExists[indexBuffer][indexPotSource] = false;
            this->bufferReadX[indexBuffer][indexPotSource] = 0.0;
            this->bufferReadY[indexBuffer][indexPotSource] = 0.0;
            this->bufferReadZ[indexBuffer][indexPotSource] = 0.0;
            this->bufferReadE[indexBuffer][indexPotSource] = 0.0;
            this->bufferReadColor[indexBuffer][indexPotSource] = QColor();
            this->bufferReadPotentialSourceExists[indexBuffer][indexPotSource] = false;

        }
    }


    // +---------------------------------------------------+
    // | Fill buffer with sources from the previous buffer |
    // +---------------------------------------------------+

        // Find the smallest number of potential sources

        int smallerMaxPotentialSources;

        if (this->maxPotentialSources > prevMaxPotentialSources)
        {
            smallerMaxPotentialSources = prevMaxPotentialSources;
        }
        else
        {
            smallerMaxPotentialSources = this->maxPotentialSources;
        }

        // +---------------------------------------------------+
        // | Write buffer                                      |
        // +---------------------------------------------------+

        int oldWriteBufferCounter = 0;

        // Loop for each element in the buffers and copy

        for (int newIndexBufferWrite = this->sizeBuffer - 1; newIndexBufferWrite >= 0; newIndexBufferWrite--)
        {

            // Decrease the pointer and wrap around if needed

            prevIndexBufferWrite1--;

            if (prevIndexBufferWrite1 < 0)
            {
                prevIndexBufferWrite1 += prevSizeBuffer;
            }

            for (int indexPotSource = 0; indexPotSource < smallerMaxPotentialSources; indexPotSource++)
            {

                this->bufferWriteX[newIndexBufferWrite][indexPotSource] = prevBufferWriteX[prevIndexBufferWrite1][indexPotSource];
                this->bufferWriteY[newIndexBufferWrite][indexPotSource] = prevBufferWriteY[prevIndexBufferWrite1][indexPotSource];
                this->bufferWriteZ[newIndexBufferWrite][indexPotSource] = prevBufferWriteZ[prevIndexBufferWrite1][indexPotSource];
                this->bufferWriteE[newIndexBufferWrite][indexPotSource] = prevBufferWriteE[prevIndexBufferWrite1][indexPotSource];
                this->bufferWriteColor[newIndexBufferWrite][indexPotSource] = prevBufferWriteColor[prevIndexBufferWrite1][indexPotSource];
                this->bufferWritePotentialSourceExists[newIndexBufferWrite][indexPotSource] = prevBufferWritePotentialSourceExists[prevIndexBufferWrite1][indexPotSource];

            }

            // If we have reached the end of the old buffer, then stop

            oldWriteBufferCounter++;

            if (oldWriteBufferCounter >= prevSizeBuffer)
            {
                break;
            }

        }

        // Set the pointer to the next element
        this->indexBufferWrite = 0;

        // +---------------------------------------------------+
        // | Read buffer                                       |
        // +---------------------------------------------------+

        // This buffer needs to stay aligned with the write buffer
        // For this reason we use the previous index of the write
        // buffer as a starting point since this index is always
        // greater or equal to the read index

        int oldReadBufferCounter = 0;

        // Loop for each element in the buffers and copy

        for (int newIndexBufferRead = this->sizeBuffer - 1; newIndexBufferRead >= 0; newIndexBufferRead--)
        {

            // Decrease the pointer and wrap around if needed

            prevIndexBufferWrite2--;

            if (prevIndexBufferWrite2 < 0)
            {
                prevIndexBufferWrite2 += prevSizeBuffer;
            }

            for (int indexPotSource = 0; indexPotSource < smallerMaxPotentialSources; indexPotSource++)
            {

                this->bufferReadX[newIndexBufferRead][indexPotSource] = prevBufferReadX[prevIndexBufferWrite2][indexPotSource];
                this->bufferReadY[newIndexBufferRead][indexPotSource] = prevBufferReadY[prevIndexBufferWrite2][indexPotSource];
                this->bufferReadZ[newIndexBufferRead][indexPotSource] = prevBufferReadZ[prevIndexBufferWrite2][indexPotSource];
                this->bufferReadE[newIndexBufferRead][indexPotSource] = prevBufferReadE[prevIndexBufferWrite2][indexPotSource];
                this->bufferReadColor[newIndexBufferRead][indexPotSource] = prevBufferReadColor[prevIndexBufferWrite2][indexPotSource];
                this->bufferReadPotentialSourceExists[newIndexBufferRead][indexPotSource] = prevBufferReadPotentialSourceExists[prevIndexBufferWrite2][indexPotSource];

            }

            // If we have reached the end of the old buffer, then stop

            oldReadBufferCounter++;

            if (oldReadBufferCounter >= prevSizeBuffer)
            {
                break;
            }

        }

        // Set the pointer to the next element and wrap around if needed
        this->indexBufferRead = 0 - prevIndexDifference;

        if (this->indexBufferRead < 0)
        {
            this->indexBufferRead += this->sizeBuffer;
        }

    // +---------------------------------------------------+
    // | Delete old buffers                                |
    // +---------------------------------------------------+

        for (int indexBuffer = 0; indexBuffer < prevSizeBuffer; indexBuffer++)
        {
            delete [] prevBufferWriteX[indexBuffer];
            delete [] prevBufferWriteY[indexBuffer];
            delete [] prevBufferWriteZ[indexBuffer];
            delete [] prevBufferWriteE[indexBuffer];
            delete [] prevBufferWriteColor[indexBuffer];
            delete [] prevBufferWritePotentialSourceExists[indexBuffer];
            delete [] prevBufferReadX[indexBuffer];
            delete [] prevBufferReadY[indexBuffer];
            delete [] prevBufferReadZ[indexBuffer];
            delete [] prevBufferReadE[indexBuffer];
            delete [] prevBufferReadColor[indexBuffer];
            delete [] prevBufferReadPotentialSourceExists[indexBuffer];
        }

        delete [] prevBufferWriteX;
        delete [] prevBufferWriteY;
        delete [] prevBufferWriteZ;
        delete [] prevBufferWriteE;
        delete [] prevBufferWriteColor;
        delete [] prevBufferWritePotentialSourceExists;
        delete [] prevBufferReadX;
        delete [] prevBufferReadY;
        delete [] prevBufferReadZ;
        delete [] prevBufferReadE;
        delete [] prevBufferReadColor;
        delete [] prevBufferReadPotentialSourceExists;


    // +---------------------------------------------------+
    // | Potential source energy                           |
    // +---------------------------------------------------+

    this->ET = _ET;

    // +---------------------------------------------------+
    // | Flags                                             |
    // +---------------------------------------------------+

    this->writingAllowed = false;
    this->readingAllowed = false;
    this->readingAllowedForAll = false;
    this->readingAllAllowed = false;

    // Free
    this->mutexReadWrite.unlock();
    this->mutexReadingAllInProgress.unlock();
    this->mutexReadingInProgress.unlock();
    this->mutexWritingInProgress.unlock();

}

// +-------------------------------------------------------+
// | Update parameters                                     |
// +-------------------------------------------------------+

void PotentialSourceManager::updateParameters(ParametersManager *_parametersManager)
{

    float timeBuffer;
    float framePerSec;
    int maxPotentialSources;
    float ET;

    timeBuffer = _parametersManager->getPropertyViewFloat("TIME_BUFFER");
    framePerSec = ((float) ParametersManager::SAMPLINGRATE) / ((float) ParametersManager::HOPSIZE);
    maxPotentialSources = _parametersManager->getPropertySystemInt("BEAMFORMER_MAXPOTENTIAL");
    ET = _parametersManager->getPropertySystemFloat("MIXTURE_ET");

    reinit(timeBuffer, framePerSec, maxPotentialSources, ET);

}

/***********************************************************
* Add new potential sources                                *
***********************************************************/

// +-------------------------------------------------------+
// | Begin filling the vector for the current frame        |
// +-------------------------------------------------------+

void PotentialSourceManager::writeBegin()
{

    // Protect the write functions group
    this->mutexWritingInProgress.lock();

    // Protect this operation
    this->mutexReadWrite.lock();

    // Init the potential source pointer
    this->indexPotentialSource = 0;

    // Now writing can start
    this->writingAllowed = true;

    // Free the write functions group
    this->mutexWritingInProgress.unlock();

}

// +-------------------------------------------------------+
// | Add a new potential source in the vector              |
// +-------------------------------------------------------+

void PotentialSourceManager::write(const float _X, const float _Y, const float _Z, const float _E)
{

    // Protect the write functions group
    this->mutexWritingInProgress.lock();

    // Check if writing is allowed
    if (this->writingAllowed == true)
    {

        this->bufferWriteX[this->indexBufferWrite][this->indexPotentialSource] = _X;
        this->bufferWriteY[this->indexBufferWrite][this->indexPotentialSource] = _Y;
        this->bufferWriteZ[this->indexBufferWrite][this->indexPotentialSource] = _Z;
        this->bufferWriteE[this->indexBufferWrite][this->indexPotentialSource] = _E;
        this->bufferWriteColor[this->indexBufferWrite][this->indexPotentialSource] = getPotentialSourceColor(_E, this->indexPotentialSource);
        this->bufferWritePotentialSourceExists[this->indexBufferWrite][this->indexPotentialSource] = true;
        this->indexPotentialSource++;

    }

    // Free the write functions group
    this->mutexWritingInProgress.unlock();

}

// +-------------------------------------------------------+
// | End filling the vector for the current frame          |
// +-------------------------------------------------------+

void PotentialSourceManager::writeEnd()
{

    // Protect the write functions group
    this->mutexWritingInProgress.lock();

    // Set remaining potential sources slot to non existing
    for (int indexPotSource = this->indexPotentialSource; indexPotSource < this->maxPotentialSources; indexPotSource++)
    {

        this->bufferWriteX[this->indexBufferWrite][indexPotSource] = 0.0;
        this->bufferWriteY[this->indexBufferWrite][indexPotSource] = 0.0;
        this->bufferWriteZ[this->indexBufferWrite][indexPotSource] = 0.0;
        this->bufferWriteE[this->indexBufferWrite][indexPotSource] = 0.0;
        this->bufferWriteColor[this->indexBufferWrite][indexPotSource] = QColor();
        this->bufferWritePotentialSourceExists[this->indexBufferWrite][indexPotSource] = false;

    }

    // Write no longer allowed
    this->writingAllowed = false;

    // Increase the index and wrap around if needed
    this->indexBufferWrite++;
    if (this->indexBufferWrite == this->sizeBuffer)
    {
        this->indexBufferWrite = 0;
    }

    //qDebug("Potential Write: %d", this->indexBufferWrite);

    // Free the mutex
    this->mutexReadWrite.unlock();

    // Free the write functions group
    this->mutexWritingInProgress.unlock();

}

/***********************************************************
* Read potential sources                                   *
***********************************************************/

// +-------------------------------------------------------+
// | Begin reading                                         |
// +-------------------------------------------------------+

void PotentialSourceManager::readBeginForAll()
{

    // Protect the read functions group
    this->mutexReadingInProgress.lock();

    // Protect this operation
    this->mutexReadWrite.lock();

    // Copy values to the reading buffer
    for (int indexPotSource = 0; indexPotSource < this->maxPotentialSources; indexPotSource++)
    {
        this->bufferReadX[this->indexBufferRead][indexPotSource] = this->bufferWriteX[this->indexBufferRead][indexPotSource];
        this->bufferReadY[this->indexBufferRead][indexPotSource] = this->bufferWriteY[this->indexBufferRead][indexPotSource];
        this->bufferReadZ[this->indexBufferRead][indexPotSource] = this->bufferWriteZ[this->indexBufferRead][indexPotSource];
        this->bufferReadE[this->indexBufferRead][indexPotSource] = this->bufferWriteE[this->indexBufferRead][indexPotSource];
        this->bufferReadColor[this->indexBufferRead][indexPotSource] = this->bufferWriteColor[this->indexBufferRead][indexPotSource];
        this->bufferReadPotentialSourceExists[this->indexBufferRead][indexPotSource] = this->bufferWritePotentialSourceExists[this->indexBufferRead][indexPotSource];
    }

    // First step before reading done
    this->readingAllowedForAll = true;

    // Free the read functions group
    this->mutexReadingInProgress.unlock();

}

void PotentialSourceManager::readBegin()
{

    // Protect the read functions group
    this->mutexReadingInProgress.lock();

    if (this->readingAllowedForAll == true)
    {

        // Now reading can start
        this->readingAllowed = true;

    }

    // Free the read functions group
    this->mutexReadingInProgress.unlock();

}

// +-------------------------------------------------------+
// | Read a potential sources                              |
// +-------------------------------------------------------+

float PotentialSourceManager::readX(const int _indexSource)
{

    // Temporary variable
    float temp;

    // Protect the read functions group
    this->mutexReadingInProgress.lock();

    // Check if reading is allowed
    if (this->readingAllowed == true)
    {

        temp = this->bufferReadX[this->indexBufferRead][_indexSource];

    }

    // Free the read functions group
    this->mutexReadingInProgress.unlock();

    return temp;

}

float PotentialSourceManager::readY(const int _indexSource)
{

    // Temporary variable
    float temp;

    // Protect the read functions group
    this->mutexReadingInProgress.lock();

    // Check if reading is allowed
    if (this->readingAllowed == true)
    {

        temp = this->bufferReadY[this->indexBufferRead][_indexSource];

    }

    // Free the read functions group
    this->mutexReadingInProgress.unlock();

    return temp;


}

float PotentialSourceManager::readZ(const int _indexSource)
{

    // Temporary variable
    float temp;

    // Protect the read functions group
    this->mutexReadingInProgress.lock();

    // Check if reading is allowed
    if (this->readingAllowed == true)
    {

        temp = this->bufferReadZ[this->indexBufferRead][_indexSource];

    }

    // Free the read functions group
    this->mutexReadingInProgress.unlock();

    return temp;


}

float PotentialSourceManager::readE(const int _indexSource)
{

    // Temporary variable
    float temp;

    // Protect the read functions group
    this->mutexReadingInProgress.lock();

    // Check if reading is allowed
    if (this->readingAllowed == true)
    {

        temp = this->bufferReadE[this->indexBufferRead][_indexSource];

    }

    // Free the read functions group
    this->mutexReadingInProgress.unlock();

    return temp;

}

QColor PotentialSourceManager::readColor(const int _indexSource)
{

    // Temporary variable
    QColor temp;

    // Protect the read functions group
    this->mutexReadingInProgress.lock();

    // Check if reading is allowed
    if (this->readingAllowed == true)
    {

        temp = this->bufferReadColor[this->indexBufferRead][_indexSource];

    }

    // Free the read functions group
    this->mutexReadingInProgress.unlock();

    return temp;

}

bool PotentialSourceManager::readExists(const int _indexSource)
{

    // Temporary variable
    bool temp;

    // Protect the read functions group
    this->mutexReadingInProgress.lock();

    // Check if reading is allowed
    if (this->readingAllowed == true)
    {

        temp = this->bufferReadPotentialSourceExists[this->indexBufferRead][_indexSource];

    }

    // Free the read functions group
    this->mutexReadingInProgress.unlock();

    return temp;


}

// +-------------------------------------------------------+
// | End reading                                           |
// +-------------------------------------------------------+

void PotentialSourceManager::readEnd()
{

    // Protect the read functions group
    this->mutexReadingInProgress.lock();

    // Reading no longer allowed
    this->readingAllowed = false;

    // Free the write functions group
    this->mutexReadingInProgress.unlock();

}

void PotentialSourceManager::readEndForAll()
{

    // Protect the read functions group
    this->mutexReadingInProgress.lock();

    // Increase the index and wrap around if needed
    this->indexBufferRead++;
    if (this->indexBufferRead == this->sizeBuffer)
    {
        this->indexBufferRead = 0;
    }

    //qDebug("Potential Read: %d", this->indexBufferRead);

    // Remove the flag
    this->readingAllowedForAll = false;

    // Free the mutex
    this->mutexReadWrite.unlock();

    // Free the write functions group
    this->mutexReadingInProgress.unlock();

}

/***********************************************************
* Read all potential sources                               *
***********************************************************/

// +-------------------------------------------------------+
// | Begin reading                                         |
// +-------------------------------------------------------+

void PotentialSourceManager::readAllBegin()
{

    // Protect the read functions group
    this->mutexReadingAllInProgress.lock();

    // Protect this operation
    this->mutexReadWrite.lock();

    // Now reading can start
    this->readingAllAllowed = true;

    // Set pointer
    this->indexBufferReadAll = this->indexBufferRead;
    this->indexBufferReadAllStart = this->indexBufferReadAll;

    // Free the read functions group
    this->mutexReadingAllInProgress.unlock();

}

// +-------------------------------------------------------+
// | Read a potential sources                              |
// +-------------------------------------------------------+

float PotentialSourceManager::readAllX(const int _indexSource)
{

    // Temporary variable
    float temp;

    // Protect the read functions group
    this->mutexReadingAllInProgress.lock();

    // Check if reading is allowed
    if (this->readingAllAllowed == true)
    {

        temp = this->bufferReadX[this->indexBufferReadAll][_indexSource];

    }

    // Free the read functions group
    this->mutexReadingAllInProgress.unlock();

    return temp;

}

float PotentialSourceManager::readAllY(const int _indexSource)
{

    // Temporary variable
    float temp;

    // Protect the read functions group
    this->mutexReadingAllInProgress.lock();

    // Check if reading is allowed
    if (this->readingAllAllowed == true)
    {

        temp = this->bufferReadY[this->indexBufferReadAll][_indexSource];

    }

    // Free the read functions group
    this->mutexReadingAllInProgress.unlock();

    return temp;

}

float PotentialSourceManager::readAllZ(const int _indexSource)
{

    // Temporary variable
    float temp;

    // Protect the read functions group
    this->mutexReadingAllInProgress.lock();

    // Check if reading is allowed
    if (this->readingAllAllowed == true)
    {

        temp = this->bufferReadZ[this->indexBufferReadAll][_indexSource];

    }

    // Free the read functions group
    this->mutexReadingAllInProgress.unlock();

    return temp;

}

float PotentialSourceManager::readAllE(const int _indexSource)
{

    // Temporary variable
    float temp;

    // Protect the read functions group
    this->mutexReadingAllInProgress.lock();

    // Check if reading is allowed
    if (this->readingAllAllowed == true)
    {

        temp = this->bufferReadE[this->indexBufferReadAll][_indexSource];

    }

    // Free the read functions group
    this->mutexReadingAllInProgress.unlock();

    return temp;

}

QColor PotentialSourceManager::readAllColor(const int _indexSource)
{

    // Temporary variable
    QColor temp;

    // Protect the read functions group
    this->mutexReadingAllInProgress.lock();

    // Check if reading is allowed
    if (this->readingAllAllowed == true)
    {

        temp = this->bufferReadColor[this->indexBufferReadAll][_indexSource];

    }

    // Free the read functions group
    this->mutexReadingAllInProgress.unlock();

    return temp;

}

bool PotentialSourceManager::readAllExists(const int _indexSource)
{

    // Temporary variable
    float temp;

    // Protect the read functions group
    this->mutexReadingAllInProgress.lock();

    // Check if reading is allowed
    if (this->readingAllAllowed == true)
    {

        temp = this->bufferReadPotentialSourceExists[this->indexBufferReadAll][_indexSource];

    }

    // Free the read functions group
    this->mutexReadingAllInProgress.unlock();

    return temp;

}

bool PotentialSourceManager::readAllNext()
{

    // Increment pointer
    this->indexBufferReadAll++;
    if (this->indexBufferReadAll >= this->sizeBuffer)
    {
        this->indexBufferReadAll = 0;
    }

    // Check if the pointer has reached the end of the buffer
    return (this->indexBufferReadAll == this->indexBufferReadAllStart);

}

// +-------------------------------------------------------+
// | End reading                                           |
// +-------------------------------------------------------+

void PotentialSourceManager::readAllEnd()
{

    // Protect the read functions group
    this->mutexReadingAllInProgress.lock();

    // Reading no longer allowed
    this->readingAllAllowed = false;

    // Free the mutex
    this->mutexReadWrite.unlock();

    // Free the write functions group
    this->mutexReadingAllInProgress.unlock();

}

/***********************************************************
* Color management                                         *
***********************************************************/

// +-------------------------------------------------------+
// | Get the potential source color                        |
// +-------------------------------------------------------+

QColor PotentialSourceManager::getPotentialSourceColor(const float _E, const int _indexSource)
{

    float Pq;
    float v;
    int indexColor;

    // Compute energy probability
    switch(_indexSource)
    {
    case 0:

        v = _E / this->ET;

        if (v <= 1)
        {
            Pq = v * v / 2;
        }
        else
        {
            Pq = 1 - (0.5 / (v * v));
        }

        break;

    case 1:

        Pq = 0.3;

        break;

    case 2:

        Pq = 0.16;

        break;

    case 3:

        Pq = 0.03;

        break;
    }

    // Get corresponding color
    indexColor = round(Pq * (this->numberColorLevels - 1));

    return (this->potentialColors.at(indexColor));

}
