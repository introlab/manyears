#include "trackedSourceManager.h"

/***********************************************************
* Constructor                                              *
***********************************************************/

TrackedSourceManager::TrackedSourceManager(ParametersManager* _parametersManager)
{

    float timeBuffer;
    float framePerSec;
    int maxTrackedSources;
    int sizeColorPalette;

    timeBuffer = _parametersManager->getPropertyViewFloat("TIME_BUFFER");
    framePerSec = ((float) ParametersManager::SAMPLINGRATE) / ((float) ParametersManager::HOPSIZE);
    maxTrackedSources = _parametersManager->getPropertySystemInt("MIXTURE_MAX_FILTERS");
    sizeColorPalette = ParametersManager::SIZECOLORPALETTE;

    init(timeBuffer, framePerSec, maxTrackedSources, sizeColorPalette);

    this->replaceColor(1, _parametersManager->getPropertyViewColor("SOURCE1_COLOR"));
    this->replaceColor(2, _parametersManager->getPropertyViewColor("SOURCE2_COLOR"));
    this->replaceColor(3, _parametersManager->getPropertyViewColor("SOURCE3_COLOR"));
    this->replaceColor(4, _parametersManager->getPropertyViewColor("SOURCE4_COLOR"));
    this->replaceColor(5, _parametersManager->getPropertyViewColor("SOURCE5_COLOR"));
    this->replaceColor(6, _parametersManager->getPropertyViewColor("SOURCE6_COLOR"));
    this->replaceColor(7, _parametersManager->getPropertyViewColor("SOURCE7_COLOR"));
    this->replaceColor(8, _parametersManager->getPropertyViewColor("SOURCE8_COLOR"));
    this->replaceColor(9, _parametersManager->getPropertyViewColor("SOURCE9_COLOR"));
    this->replaceColor(10, _parametersManager->getPropertyViewColor("SOURCE10_COLOR"));

}

/***********************************************************
* Destructor                                               *
***********************************************************/

TrackedSourceManager::~TrackedSourceManager()
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
            delete [] this->bufferWriteID[indexBuffer];
            delete [] this->bufferWriteColor[indexBuffer];
            delete [] this->bufferWriteTrackedSourceExists[indexBuffer];
            delete [] this->bufferReadX[indexBuffer];
            delete [] this->bufferReadY[indexBuffer];
            delete [] this->bufferReadZ[indexBuffer];
            delete [] this->bufferReadID[indexBuffer];
            delete [] this->bufferReadColor[indexBuffer];
            delete [] this->bufferReadTrackedSourceExists[indexBuffer];
        }

        delete [] this->bufferWriteX;
        delete [] this->bufferWriteY;
        delete [] this->bufferWriteZ;
        delete [] this->bufferWriteID;
        delete [] this->bufferWriteColor;
        delete [] this->bufferWriteTrackedSourceExists;
        delete [] this->bufferReadX;
        delete [] this->bufferReadY;
        delete [] this->bufferReadZ;
        delete [] this->bufferReadID;
        delete [] this->bufferReadColor;
        delete [] this->bufferReadTrackedSourceExists;

}

/***********************************************************
* Parameters                                               *
***********************************************************/

// +-------------------------------------------------------+
// | Init object with current parameters                   |
// +-------------------------------------------------------+

void TrackedSourceManager::init(const float _timeBuffer, const float _framePerSec, const int _maxTrackedSources, const int _sizeColorPalette)
{

    // Save the size
    this->sizeBuffer = (int) (round(_timeBuffer * _framePerSec));

    // Save number of sources per frame
    this->maxTrackedSources = _maxTrackedSources;

    // Initialize buffers
    this->indexBufferWrite = 0;
    this->indexBufferRead = 0;
    this->bufferWriteX = new float* [this->sizeBuffer];
    this->bufferWriteY = new float* [this->sizeBuffer];
    this->bufferWriteZ = new float* [this->sizeBuffer];
    this->bufferWriteID = new int* [this->sizeBuffer];
    this->bufferWriteColor = new int* [this->sizeBuffer];
    this->bufferWriteTrackedSourceExists = new bool* [this->sizeBuffer];
    this->bufferReadX = new float* [this->sizeBuffer];
    this->bufferReadY = new float* [this->sizeBuffer];
    this->bufferReadZ = new float* [this->sizeBuffer];
    this->bufferReadID = new int* [this->sizeBuffer];
    this->bufferReadColor = new int* [this->sizeBuffer];
    this->bufferReadTrackedSourceExists = new bool* [this->sizeBuffer];

    // Fill buffer with no potential sources
    for (int indexBuffer = 0; indexBuffer < this->sizeBuffer; indexBuffer++)
    {
        this->bufferWriteX[indexBuffer] = new float [this->maxTrackedSources];
        this->bufferWriteY[indexBuffer] = new float [this->maxTrackedSources];
        this->bufferWriteZ[indexBuffer] = new float [this->maxTrackedSources];
        this->bufferWriteID[indexBuffer] = new int [this->maxTrackedSources];
        this->bufferWriteColor[indexBuffer] = new int [this->maxTrackedSources];
        this->bufferWriteTrackedSourceExists[indexBuffer] = new bool [this->maxTrackedSources];
        this->bufferReadX[indexBuffer] = new float [this->maxTrackedSources];
        this->bufferReadY[indexBuffer] = new float [this->maxTrackedSources];
        this->bufferReadZ[indexBuffer] = new float [this->maxTrackedSources];
        this->bufferReadID[indexBuffer] = new int [this->maxTrackedSources];
        this->bufferReadColor[indexBuffer] = new int [this->maxTrackedSources];
        this->bufferReadTrackedSourceExists[indexBuffer] = new bool [this->maxTrackedSources];

        for (int indexPotSource = 0; indexPotSource < this->maxTrackedSources; indexPotSource++)
        {

            this->bufferWriteX[indexBuffer][indexPotSource] = 0.0;
            this->bufferWriteY[indexBuffer][indexPotSource] = 0.0;
            this->bufferWriteZ[indexBuffer][indexPotSource] = 0.0;
            this->bufferWriteID[indexBuffer][indexPotSource] = 0.0;
            this->bufferWriteColor[indexBuffer][indexPotSource] = 0;
            this->bufferWriteTrackedSourceExists[indexBuffer][indexPotSource] = false;
            this->bufferReadX[indexBuffer][indexPotSource] = 0.0;
            this->bufferReadY[indexBuffer][indexPotSource] = 0.0;
            this->bufferReadZ[indexBuffer][indexPotSource] = 0.0;
            this->bufferReadID[indexBuffer][indexPotSource] = 0.0;
            this->bufferReadColor[indexBuffer][indexPotSource] = 0;
            this->bufferReadTrackedSourceExists[indexBuffer][indexPotSource] = false;

        }
    }

    // Create the color palette
    QVector<QColor> tmp;

    tmp.clear();

    for (int indexColor = 0; indexColor < _sizeColorPalette; indexColor++)
    {
        tmp.push_back(QColor());
    }

    this->colorPalette = new ColorPalette(tmp);

    // Initialize the list of matching
    this->pastColorID.clear();
    this->pastSourceID.clear();
    this->newColorID.clear();
    this->newSourceID.clear();

    // No reading or writing allowed for now
    this->writingAllowed = false;
    this->readingAllowed = false;
    this->readingAllowedForAll = false;
    this->readingAllAllowed = false;

}

// +-------------------------------------------------------+
// | Initialize again object with current parameters       |
// +-------------------------------------------------------+

void TrackedSourceManager::reinit(const float _timeBuffer, const float _framePerSec, const int _maxTrackedSources, const int _sizeColorPalette)
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
    int prevMaxTrackedSources = this->maxTrackedSources;

    // Wrap around if needed
    if (prevIndexDifference < 0)
    {
        prevIndexDifference += prevSizeBuffer;
    }

    // Buffers pointers
    float** prevBufferWriteX = this->bufferWriteX;
    float** prevBufferWriteY = this->bufferWriteY;
    float** prevBufferWriteZ = this->bufferWriteZ;
    int** prevBufferWriteID = this->bufferWriteID;
    int** prevBufferWriteColor = this->bufferWriteColor;
    bool** prevBufferWriteTrackedSourceExists = this->bufferWriteTrackedSourceExists;
    float** prevBufferReadX = this->bufferReadX;
    float** prevBufferReadY = this->bufferReadY;
    float** prevBufferReadZ = this->bufferReadZ;
    int** prevBufferReadID = this->bufferReadID;
    int** prevBufferReadColor = this->bufferReadColor;
    bool** prevBufferReadTrackedSourceExists = this->bufferReadTrackedSourceExists;

    // +---------------------------------------------------+
    // | Create new buffer                                 |
    // +---------------------------------------------------+

    // New size
    this->sizeBuffer = (int) (round(_timeBuffer * _framePerSec));

    // New number of sources per frame
    this->maxTrackedSources = _maxTrackedSources;

    // Create new buffers
    this->bufferWriteX = new float* [this->sizeBuffer];
    this->bufferWriteY = new float* [this->sizeBuffer];
    this->bufferWriteZ = new float* [this->sizeBuffer];
    this->bufferWriteID = new int* [this->sizeBuffer];
    this->bufferWriteColor = new int* [this->sizeBuffer];
    this->bufferWriteTrackedSourceExists = new bool* [this->sizeBuffer];
    this->bufferReadX = new float* [this->sizeBuffer];
    this->bufferReadY = new float* [this->sizeBuffer];
    this->bufferReadZ = new float* [this->sizeBuffer];
    this->bufferReadID = new int* [this->sizeBuffer];
    this->bufferReadColor = new int* [this->sizeBuffer];
    this->bufferReadTrackedSourceExists = new bool* [this->sizeBuffer];

    // Fill buffer with no potential sources
    for (int indexBuffer = 0; indexBuffer < this->sizeBuffer; indexBuffer++)
    {
        this->bufferWriteX[indexBuffer] = new float [this->maxTrackedSources];
        this->bufferWriteY[indexBuffer] = new float [this->maxTrackedSources];
        this->bufferWriteZ[indexBuffer] = new float [this->maxTrackedSources];
        this->bufferWriteID[indexBuffer] = new int [this->maxTrackedSources];
        this->bufferWriteColor[indexBuffer] = new int [this->maxTrackedSources];
        this->bufferWriteTrackedSourceExists[indexBuffer] = new bool [this->maxTrackedSources];
        this->bufferReadX[indexBuffer] = new float [this->maxTrackedSources];
        this->bufferReadY[indexBuffer] = new float [this->maxTrackedSources];
        this->bufferReadZ[indexBuffer] = new float [this->maxTrackedSources];
        this->bufferReadID[indexBuffer] = new int [this->maxTrackedSources];
        this->bufferReadColor[indexBuffer] = new int [this->maxTrackedSources];
        this->bufferReadTrackedSourceExists[indexBuffer] = new bool [this->maxTrackedSources];

        for (int indexPotSource = 0; indexPotSource < this->maxTrackedSources; indexPotSource++)
        {

            this->bufferWriteX[indexBuffer][indexPotSource] = 0.0;
            this->bufferWriteY[indexBuffer][indexPotSource] = 0.0;
            this->bufferWriteZ[indexBuffer][indexPotSource] = 0.0;
            this->bufferWriteID[indexBuffer][indexPotSource] = 0;
            this->bufferWriteColor[indexBuffer][indexPotSource] = 0;
            this->bufferWriteTrackedSourceExists[indexBuffer][indexPotSource] = false;
            this->bufferReadX[indexBuffer][indexPotSource] = 0.0;
            this->bufferReadY[indexBuffer][indexPotSource] = 0.0;
            this->bufferReadZ[indexBuffer][indexPotSource] = 0.0;
            this->bufferReadID[indexBuffer][indexPotSource] = 0;
            this->bufferReadColor[indexBuffer][indexPotSource] = 0;
            this->bufferReadTrackedSourceExists[indexBuffer][indexPotSource] = false;

        }
    }


    // +---------------------------------------------------+
    // | Fill buffer with sources from the previous buffer |
    // +---------------------------------------------------+

        // Find the smallest number of potential sources

        int smallerMaxTrackedSources;

        if (this->maxTrackedSources > prevMaxTrackedSources)
        {
            smallerMaxTrackedSources = prevMaxTrackedSources;
        }
        else
        {
            smallerMaxTrackedSources = this->maxTrackedSources;
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

            for (int indexPotSource = 0; indexPotSource < smallerMaxTrackedSources; indexPotSource++)
            {

                this->bufferWriteX[newIndexBufferWrite][indexPotSource] = prevBufferWriteX[prevIndexBufferWrite1][indexPotSource];
                this->bufferWriteY[newIndexBufferWrite][indexPotSource] = prevBufferWriteY[prevIndexBufferWrite1][indexPotSource];
                this->bufferWriteZ[newIndexBufferWrite][indexPotSource] = prevBufferWriteZ[prevIndexBufferWrite1][indexPotSource];
                this->bufferWriteID[newIndexBufferWrite][indexPotSource] = prevBufferWriteID[prevIndexBufferWrite1][indexPotSource];
                this->bufferWriteColor[newIndexBufferWrite][indexPotSource] = prevBufferWriteColor[prevIndexBufferWrite1][indexPotSource];
                this->bufferWriteTrackedSourceExists[newIndexBufferWrite][indexPotSource] = prevBufferWriteTrackedSourceExists[prevIndexBufferWrite1][indexPotSource];

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

            for (int indexPotSource = 0; indexPotSource < smallerMaxTrackedSources; indexPotSource++)
            {

                this->bufferReadX[newIndexBufferRead][indexPotSource] = prevBufferReadX[prevIndexBufferWrite2][indexPotSource];
                this->bufferReadY[newIndexBufferRead][indexPotSource] = prevBufferReadY[prevIndexBufferWrite2][indexPotSource];
                this->bufferReadZ[newIndexBufferRead][indexPotSource] = prevBufferReadZ[prevIndexBufferWrite2][indexPotSource];
                this->bufferReadID[newIndexBufferRead][indexPotSource] = prevBufferReadID[prevIndexBufferWrite2][indexPotSource];
                this->bufferReadColor[newIndexBufferRead][indexPotSource] = prevBufferReadColor[prevIndexBufferWrite2][indexPotSource];
                this->bufferReadTrackedSourceExists[newIndexBufferRead][indexPotSource] = prevBufferReadTrackedSourceExists[prevIndexBufferWrite2][indexPotSource];

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
            delete [] prevBufferWriteID[indexBuffer];
            delete [] prevBufferWriteColor[indexBuffer];
            delete [] prevBufferWriteTrackedSourceExists[indexBuffer];
            delete [] prevBufferReadX[indexBuffer];
            delete [] prevBufferReadY[indexBuffer];
            delete [] prevBufferReadZ[indexBuffer];
            delete [] prevBufferReadID[indexBuffer];
            delete [] prevBufferReadColor[indexBuffer];
            delete [] prevBufferReadTrackedSourceExists[indexBuffer];
        }

        delete [] prevBufferWriteX;
        delete [] prevBufferWriteY;
        delete [] prevBufferWriteZ;
        delete [] prevBufferWriteID;
        delete [] prevBufferWriteColor;
        delete [] prevBufferWriteTrackedSourceExists;
        delete [] prevBufferReadX;
        delete [] prevBufferReadY;
        delete [] prevBufferReadZ;
        delete [] prevBufferReadID;
        delete [] prevBufferReadColor;
        delete [] prevBufferReadTrackedSourceExists;

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

void TrackedSourceManager::updateParameters(ParametersManager* _parametersManager)
{

    float timeBuffer;
    float framePerSec;
    int maxTrackedSources;
    int sizeColorPalette;

    timeBuffer = _parametersManager->getPropertyViewFloat("TIME_BUFFER");
    framePerSec = ((float) ParametersManager::SAMPLINGRATE) / ((float) ParametersManager::HOPSIZE);
    maxTrackedSources = _parametersManager->getPropertySystemInt("MIXTURE_MAX_FILTERS");
    sizeColorPalette = ParametersManager::SIZECOLORPALETTE;

    reinit(timeBuffer, framePerSec, maxTrackedSources, sizeColorPalette);

    this->replaceColor(1, _parametersManager->getPropertyViewColor("SOURCE1_COLOR"));
    this->replaceColor(2, _parametersManager->getPropertyViewColor("SOURCE2_COLOR"));
    this->replaceColor(3, _parametersManager->getPropertyViewColor("SOURCE3_COLOR"));
    this->replaceColor(4, _parametersManager->getPropertyViewColor("SOURCE4_COLOR"));
    this->replaceColor(5, _parametersManager->getPropertyViewColor("SOURCE5_COLOR"));
    this->replaceColor(6, _parametersManager->getPropertyViewColor("SOURCE6_COLOR"));
    this->replaceColor(7, _parametersManager->getPropertyViewColor("SOURCE7_COLOR"));
    this->replaceColor(8, _parametersManager->getPropertyViewColor("SOURCE8_COLOR"));
    this->replaceColor(9, _parametersManager->getPropertyViewColor("SOURCE9_COLOR"));
    this->replaceColor(10, _parametersManager->getPropertyViewColor("SOURCE10_COLOR"));

}

/***********************************************************
* Modify colors                                            *
***********************************************************/

void TrackedSourceManager::replaceColor(const int _colorID, const QColor _color)
{

    // Protect this operation
    this->mutexReadWrite.lock();

    // Change color
    this->colorPalette->replaceColor(_colorID, _color);

    // Free
    this->mutexReadWrite.unlock();

}


/***********************************************************
* Add new potential sources                                *
***********************************************************/

// +-------------------------------------------------------+
// | Begin filling the vector for the current frame        |
// +-------------------------------------------------------+

void TrackedSourceManager::writeBegin()
{

    // Protect the write functions group
    this->mutexWritingInProgress.lock();

    // Protect this operation
    this->mutexReadWrite.lock();

    // Init the tracked source pointer
    this->indexTrackedSource = 0;

    // Reset the stacks
    this->newColorID.clear();
    this->newSourceID.clear();

    // Now writing can start
    this->writingAllowed = true;

    // Free the write functions group
    this->mutexWritingInProgress.unlock();

}

// +-------------------------------------------------------+
// | Add a new potential source in the vector              |
// +-------------------------------------------------------+

void TrackedSourceManager::write(const float _X, const float _Y, const float _Z, const int _ID)
{

    // Protect the write functions group
    this->mutexWritingInProgress.lock();

    bool sourceAlreadyExists;

    int colorID;

    // Check if writing is allowed
    if (this->writingAllowed == true)
    {

        this->bufferWriteX[this->indexBufferWrite][this->indexTrackedSource] = _X;
        this->bufferWriteY[this->indexBufferWrite][this->indexTrackedSource] = _Y;
        this->bufferWriteZ[this->indexBufferWrite][this->indexTrackedSource] = _Z;
        this->bufferWriteID[this->indexBufferWrite][this->indexTrackedSource] = _ID;

        // Check if there was already a color for this source ID
        if (_ID != -1)
        {

            sourceAlreadyExists = false;

            for (int indexColor = 0; indexColor < this->pastSourceID.size(); indexColor++)
            {

                if (this->pastSourceID.at(indexColor) == _ID)
                {
                    colorID = this->pastColorID.at(indexColor);
                    sourceAlreadyExists = true;

                    break;
                }

            }

            // If the source is new, assign a color
            if (sourceAlreadyExists == false)
            {

                colorID = this->colorPalette->useColor();

            }

            this->bufferWriteColor[this->indexBufferWrite][this->indexTrackedSource] = colorID;
            this->bufferWriteTrackedSourceExists[this->indexBufferWrite][this->indexTrackedSource] = true;

            this->newColorID.push_back(colorID);
            this->newSourceID.push_back(_ID);
        }
        else
        {

            this->bufferWriteColor[this->indexBufferWrite][this->indexTrackedSource] = 0;
            this->bufferWriteTrackedSourceExists[this->indexBufferWrite][this->indexTrackedSource] = false;

        }

        this->indexTrackedSource++;

    }

    // Free the write functions group
    this->mutexWritingInProgress.unlock();

}

// +-------------------------------------------------------+
// | End filling the vector for the current frame          |
// +-------------------------------------------------------+

void TrackedSourceManager::writeEnd()
{

    // Protect the write functions group
    this->mutexWritingInProgress.lock();

    // Set remaining potential sources slot to non existing
    for (int indexPotSource = this->indexTrackedSource; indexPotSource < this->maxTrackedSources; indexPotSource++)
    {

        this->bufferWriteX[this->indexBufferWrite][indexPotSource] = 0.0;
        this->bufferWriteY[this->indexBufferWrite][indexPotSource] = 0.0;
        this->bufferWriteZ[this->indexBufferWrite][indexPotSource] = 0.0;
        this->bufferWriteID[this->indexBufferWrite][indexPotSource] = -1;
        this->bufferWriteColor[this->indexBufferWrite][indexPotSource] = 0;
        this->bufferWriteTrackedSourceExists[this->indexBufferWrite][indexPotSource] = false;

    }

    // Now check if a source used to exist and is no longer
    // there in order to free its associated color
    bool stillExists;

    for (int indexPast = 0; indexPast < this->pastColorID.size(); indexPast++)
    {

        stillExists = false;

        for (int indexNew = 0; indexNew < this->newColorID.size(); indexNew++)
        {

            if (this->pastColorID.at(indexPast) == this->newColorID.at(indexNew))
            {

                stillExists = true;

                break;

            }

        }

        if (stillExists == false)
        {

            this->colorPalette->freeColor(this->pastColorID.at(indexPast));

        }

    }

    // New stacks become the previous stacks
    this->pastColorID.clear();
    this->pastSourceID.clear();

    for (int indexStack = 0; indexStack < this->newColorID.size(); indexStack++)
    {

        this->pastColorID.push_back(this->newColorID.at(indexStack));
        this->pastSourceID.push_back(this->newSourceID.at(indexStack));

    }

    // Write no longer allowed
    this->writingAllowed = false;

    // Increase the index and wrap around if needed
    this->indexBufferWrite++;
    if (this->indexBufferWrite == this->sizeBuffer)
    {
        this->indexBufferWrite = 0;
    }

    //qDebug("Tracked Write: %d", this->indexBufferWrite);

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

void TrackedSourceManager::readBeginForAll()
{

    // Protect the read functions group
    this->mutexReadingInProgress.lock();

    // Protect this operation
    this->mutexReadWrite.lock();

    // Copy values to the reading buffer
    for (int indexPotSource = 0; indexPotSource < this->maxTrackedSources; indexPotSource++)
    {
        this->bufferReadX[this->indexBufferRead][indexPotSource] = this->bufferWriteX[this->indexBufferRead][indexPotSource];
        this->bufferReadY[this->indexBufferRead][indexPotSource] = this->bufferWriteY[this->indexBufferRead][indexPotSource];
        this->bufferReadZ[this->indexBufferRead][indexPotSource] = this->bufferWriteZ[this->indexBufferRead][indexPotSource];
        this->bufferReadID[this->indexBufferRead][indexPotSource] = this->bufferWriteID[this->indexBufferRead][indexPotSource];
        this->bufferReadColor[this->indexBufferRead][indexPotSource] = this->bufferWriteColor[this->indexBufferRead][indexPotSource];
        this->bufferReadTrackedSourceExists[this->indexBufferRead][indexPotSource] = this->bufferWriteTrackedSourceExists[this->indexBufferRead][indexPotSource];
    }

    // First step before reading done
    this->readingAllowedForAll = true;

    // Free the read functions group
    this->mutexReadingInProgress.unlock();

}

void TrackedSourceManager::readBegin()
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

float TrackedSourceManager::readX(const int _indexSource)
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

float TrackedSourceManager::readY(const int _indexSource)
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

float TrackedSourceManager::readZ(const int _indexSource)
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

int TrackedSourceManager::readID(const int _indexSource)
{

    // Temporary variable
    float temp;

    // Protect the read functions group
    this->mutexReadingInProgress.lock();

    // Check if reading is allowed
    if (this->readingAllowed == true)
    {

        temp = this->bufferReadID[this->indexBufferRead][_indexSource];

    }

    // Free the read functions group
    this->mutexReadingInProgress.unlock();

    return temp;

}

int TrackedSourceManager::readColor(const int _indexSource)
{

    // Temporary variable
    int temp;

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

QColor TrackedSourceManager::readColorValue(const int _colorID)
{

    // Temporary variable
    QColor temp;

    // Protect the read functions group
    this->mutexReadingInProgress.lock();

    // Check if reading is allowed
    if (this->readingAllowed == true)
    {

        temp = this->colorPalette->getColor(_colorID);

    }

    // Free the read functions group
    this->mutexReadingInProgress.unlock();

    return temp;

}

bool TrackedSourceManager::readExists(const int _indexSource)
{

    // Temporary variable
    bool temp;

    // Protect the read functions group
    this->mutexReadingInProgress.lock();

    // Check if reading is allowed
    if (this->readingAllowed == true)
    {

        temp = this->bufferReadTrackedSourceExists[this->indexBufferRead][_indexSource];

    }

    // Free the read functions group
    this->mutexReadingInProgress.unlock();

    return temp;


}

// +-------------------------------------------------------+
// | End reading                                           |
// +-------------------------------------------------------+

void TrackedSourceManager::readEnd()
{

    // Protect the read functions group
    this->mutexReadingInProgress.lock();

    // Reading no longer allowed
    this->readingAllowed = false;

    // Free the write functions group
    this->mutexReadingInProgress.unlock();

}

void TrackedSourceManager::readEndForAll()
{

    // Protect the read functions group
    this->mutexReadingInProgress.lock();

    // Increase the index and wrap around if needed
    this->indexBufferRead++;
    if (this->indexBufferRead == this->sizeBuffer)
    {
        this->indexBufferRead = 0;
    }

    //qDebug("Tracked Read: %d", this->indexBufferRead);

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

void TrackedSourceManager::readAllBegin()
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

float TrackedSourceManager::readAllX(const int _indexSource)
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

float TrackedSourceManager::readAllY(const int _indexSource)
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

float TrackedSourceManager::readAllZ(const int _indexSource)
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

int TrackedSourceManager::readAllID(const int _indexSource)
{

    // Temporary variable
    float temp;

    // Protect the read functions group
    this->mutexReadingAllInProgress.lock();

    // Check if reading is allowed
    if (this->readingAllAllowed == true)
    {

        temp = this->bufferReadID[this->indexBufferReadAll][_indexSource];

    }

    // Free the read functions group
    this->mutexReadingAllInProgress.unlock();

    return temp;

}

int TrackedSourceManager::readAllColor(const int _indexSource)
{

    // Temporary variable
    int temp;

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

QColor TrackedSourceManager::readAllColorValue(const int _colorID)
{

    // Temporary variable
    QColor temp;

    // Protect the read functions group
    this->mutexReadingAllInProgress.lock();

    // Check if reading is allowed
    if (this->readingAllAllowed == true)
    {

        temp = this->colorPalette->getColor(_colorID);

    }

    // Free the read functions group
    this->mutexReadingAllInProgress.unlock();

    return temp;

}


bool TrackedSourceManager::readAllExists(const int _indexSource)
{

    // Temporary variable
    float temp;

    // Protect the read functions group
    this->mutexReadingAllInProgress.lock();

    // Check if reading is allowed
    if (this->readingAllAllowed == true)
    {

        temp = this->bufferReadTrackedSourceExists[this->indexBufferReadAll][_indexSource];

    }

    // Free the read functions group
    this->mutexReadingAllInProgress.unlock();

    return temp;

}

bool TrackedSourceManager::readAllNext()
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

void TrackedSourceManager::readAllEnd()
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

