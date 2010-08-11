#include "sourceManager.h"

/***********************************************************
* Constructor                                              *
***********************************************************/

SourceManager::SourceManager(int _maxSources, ColorPalette *_colorPalette)
{

    // Set the number of sources

    this->numberSources = 0;
    this->maxSources = _maxSources;

    // Link with the color palette

    this->sourcePalette = _colorPalette;

    // Reserve memory for vectors

    this->connectColors.reserve(this->maxSources);
    this->connectLatitudeAngles.reserve(this->maxSources);
    this->connectLongitudeAngles.reserve(this->maxSources);
    this->connectXPosition.reserve(this->maxSources);
    this->connectYPosition.reserve(this->maxSources);
    this->connectZPosition.reserve(this->maxSources);

}

/***********************************************************
* Update                                                   *
***********************************************************/

// +-------------------------------------------------------+
// | Reset (clear all sources)                             |
// +-------------------------------------------------------+

void SourceManager::reset()
{
    // Clean the vector and reserve space

    this->listSources.clear();
    this->numberSources = 0;
    this->listSources.reserve(this->maxSources);

}

// +-------------------------------------------------------+
// | Start                                                 |
// +-------------------------------------------------------+

void SourceManager::start()
{

    // Start using the palette
    this->sourcePalette->start();

    // Reset
    this->reset();

    // The manager is now active
    this->active = true;

}


// +-------------------------------------------------------+
// | Stop                                                  |
// +-------------------------------------------------------+

void SourceManager::stop()
{

    // Stop using the palette
    this->sourcePalette->stop();

    // The manager is inactive
    this->active = false;

}


// +-------------------------------------------------------+
// | Update the frame for plotting                         |
// +-------------------------------------------------------+

void SourceManager::updateFrame()
{

    // Protect this section with a mutex since the
    // values to be shown must not be updated as they
    // are painted
    //this->blockRendering.lock();

    //QDebug("Start modifying sources");

    int indexSource;

    float x;
    float y;
    float z;
    float longitude;
    float latitude;

    // Remove sources that have not been active

    indexSource = 0;

    this->connectColors.clear();
    this->connectLatitudeAngles.clear();
    this->connectLongitudeAngles.clear();
    this->connectXPosition.clear();
    this->connectYPosition.clear();
    this->connectZPosition.clear();

    if (this->active == true)
    {

        while (indexSource < this->numberSources)
        {
            // If it is active, set to inactive, process and then
            // prepare for the next iteration
            if (this->listSources[indexSource].active == true)
            {
                this->listSources[indexSource].active = false;

                this->connectColors.push_back(this->sourcePalette->getColor(this->listSources[indexSource].colorID));

                x = this->listSources[indexSource].x;
                y = this->listSources[indexSource].y;
                z = this->listSources[indexSource].z;

                longitude = atan2f(y,x) / (2*M_PI) * 360;
                latitude = asinf(z/(sqrt(x*x+y*y+z*z))) / (2*M_PI) * 360;

                this->connectLatitudeAngles.push_back(latitude);
                this->connectLongitudeAngles.push_back(longitude);
                this->connectXPosition.push_back(x);
                this->connectYPosition.push_back(y);
                this->connectZPosition.push_back(z);

                indexSource++;
            }
            // If inactive, remove from vector
            // Do not increment the index since this item is
            // removed
            else
            {
                this->sourcePalette->freeColor(this->listSources[indexSource].colorID);
                this->listSources.remove(indexSource);
                this->numberSources--;
            }
        }

    }

    //QDebug("Stop modifying sources");

    // Unlock
    //this->blockRendering.unlock();

}

// +-------------------------------------------------------+
// | Add a source for the current frame                    |
// +-------------------------------------------------------+

void SourceManager::addSource(int _sourceID, float _x, float _y, float _z)
{

    bool alreadyExists;

    if (this->active == true)
    {

        alreadyExists = false;

        // Look if this source already exists
        for (int indexSource = 0; indexSource < this->numberSources; indexSource++)
        {

            if (this->listSources[indexSource].sourceID == _sourceID)
            {

                alreadyExists = true;
                this->listSources[indexSource].active = true;
                this->listSources[indexSource].x = _x;
                this->listSources[indexSource].y = _y;
                this->listSources[indexSource].z = _z;
                break;

            }

        }

        // If the source did not exist, add it
        if (alreadyExists == false)
        {

            listSources.push_back(Source(_sourceID,this->sourcePalette->useColor(),_x,_y,_z));
            this->numberSources++;

        }

    }

}
