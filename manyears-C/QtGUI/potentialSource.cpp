#include "potentialSource.h"

/***********************************************************
* Constructor                                              *
***********************************************************/

PotentialSource::PotentialSource(const float _X, const float _Y, const float _Z, const float _E)
{

    this->setX(_X);
    this->setY(_Y);
    this->setZ(_Z);
    this->setE(_E);

}

PotentialSource::PotentialSource()
{

    this->setX(0.0);
    this->setY(0.0);
    this->setZ(0.0);
    this->setE(0.0);

}

/***********************************************************
* Destructor                                               *
***********************************************************/

PotentialSource::~PotentialSource()
{

}

/***********************************************************
* Accessors                                                *
***********************************************************/

// +-------------------------------------------------------+
// | Position                                              |
// +-------------------------------------------------------+

const float PotentialSource::getX()
{
    return (this->X);
}

const float PotentialSource::getY()
{
    return (this->Y);
}

const float PotentialSource::getZ()
{
    return (this->Z);
}

// +-------------------------------------------------------+
// | Energy                                                |
// +-------------------------------------------------------+

const float PotentialSource::getE()
{
    return (this->E);
}

/***********************************************************
* Mutators                                                 *
***********************************************************/

// +-------------------------------------------------------+
// | Position                                              |
// +-------------------------------------------------------+

void PotentialSource::setX(const float _X)
{
    this->X = _X;
}

void PotentialSource::setY(const float _Y)
{
    this->Y = _Y;
}

void PotentialSource::setZ(const float _Z)
{
    this->Z = _Z;
}

// +-------------------------------------------------------+
// | Energy                                                |
// +-------------------------------------------------------+

void PotentialSource::setE(const float _E)
{
    this->E = _E;
}
