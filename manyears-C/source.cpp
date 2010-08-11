#include "source.h"

/***********************************************************
* Constructor                                              *
***********************************************************/

Source::Source(int _sourceID, int _colorID, float _x, float _y, float _z)
{
    this->sourceID = _sourceID;
    this->colorID = _colorID;
    this->active = true;
    this->x = _x;
    this->y = _y;
    this->z = _z;
}

Source::Source()
{
    this->sourceID = 0;
    this->colorID = 0;
    this->active = false;
    this->x = 0;
    this->y = 0;
    this->z = 0;
}
