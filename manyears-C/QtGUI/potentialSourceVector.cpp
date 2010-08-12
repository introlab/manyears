#include "potentialSourceVector.h"

/***********************************************************
* Constructor                                              *
***********************************************************/

PotentialSourceVector::PotentialSourceVector()
{

    // By default create an empty vector
    this->vectorBegin();
    this->vectorEnd();

}

/***********************************************************
* Destructor                                               *
***********************************************************/

PotentialSourceVector::~PotentialSourceVector()
{

}

/***********************************************************
* Fill the vector                                          *
***********************************************************/

// +-------------------------------------------------------+
// | Begin filling the vector                              |
// +-------------------------------------------------------+

void PotentialSourceVector::vectorBegin()
{

    this->listPotentialSource.clear();
    this->numberPotentialSources = 0;

}

// +-------------------------------------------------------+
// | Add a new potential source in the vector              |
// +-------------------------------------------------------+

void PotentialSourceVector::vectorAdd(const PotentialSource _potentialSource)
{

    this->listPotentialSource.push_front(_potentialSource);
    this->numberPotentialSources++;

}

// +-------------------------------------------------------+
// | End filling the vector                                |
// +-------------------------------------------------------+

void PotentialSourceVector::vectorEnd()
{

}

/***********************************************************
* Accessors                                                *
***********************************************************/

// +-------------------------------------------------------+
// | Potential source at the given index                   |
// +-------------------------------------------------------+

PotentialSource PotentialSourceVector::getPotentialSource(int _indexPotentialSource)
{

    return (this->listPotentialSource[_indexPotentialSource]);

}

// +-------------------------------------------------------+
// | Number of potential sources in the vector             |
// +-------------------------------------------------------+

const int PotentialSourceVector::getNumberPotentialSources()
{

    return (this->numberPotentialSources);

}

/***********************************************************
* Mutators                                                 *
***********************************************************/

// +-------------------------------------------------------+
// | Potential source at the given index                   |
// +-------------------------------------------------------+

void PotentialSourceVector::setPotentialSource(const int _indexPotentialSource, const PotentialSource _potentialSource)
{

    this->listPotentialSource[_indexPotentialSource] = _potentialSource;

}
