#include "trackedSource.h"

/***********************************************************
* Constructor                                              *
***********************************************************/

TrackedSource::TrackedSource(const float _X, const float _Y, const float _Z, const int _trackedSourceID, const int _colorID)
{

    this->setX(_X);
    this->setY(_Y);
    this->setZ(_Z);
    this->setTrackedSourceID(_trackedSourceID);
    this->setColorID(_colorID);

}

TrackedSource::TrackedSource()
{

    this->setX(0.0);
    this->setY(0.0);
    this->setZ(0.0);
    this->setTrackedSourceID(0);
    this->setColorID(0);

}

/***********************************************************
* Destructor                                               *
***********************************************************/

TrackedSource::~TrackedSource()
{

}

/***********************************************************
* Accessors                                                *
***********************************************************/

// +-------------------------------------------------------+
// | Position                                              |
// +-------------------------------------------------------+

const float TrackedSource::getX()
{
    return (this->X);
}

const float TrackedSource::getY()
{
    return (this->Y);
}

const float TrackedSource::getZ()
{
    return (this->Z);
}

// +-------------------------------------------------------+
// | Tracked source ID                                     |
// +-------------------------------------------------------+

const int TrackedSource::getTrackedSourceID()
{
    return (this->trackedSourceID);
}

// +-------------------------------------------------------+
// | Color ID                                              |
// +-------------------------------------------------------+

const int TrackedSource::getColorID()
{
    return (this->colorID);
}


/***********************************************************
* Mutators                                                 *
***********************************************************/

// +-------------------------------------------------------+
// | Position                                              |
// +-------------------------------------------------------+

void TrackedSource::setX(const float _X)
{
    this->X = _X;
}

void TrackedSource::setY(const float _Y)
{
    this->Y = _Y;
}

void TrackedSource::setZ(const float _Z)
{
    this->Z = _Z;
}

// +-------------------------------------------------------+
// | Tracked source ID                                     |
// +-------------------------------------------------------+

void TrackedSource::setTrackedSourceID(const int _trackedSourceID)
{
    this->trackedSourceID = _trackedSourceID;
}

// +-------------------------------------------------------+
// | Color ID                                              |
// +-------------------------------------------------------+

void TrackedSource::setColorID(const int _colorID)
{
    this->colorID = _colorID;
}
