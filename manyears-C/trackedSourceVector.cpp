#include "trackedSourceVector.h"

/***********************************************************
* Constructor                                              *
***********************************************************/

TrackedSourceVector::TrackedSourceVector()
{

    // By default create an empty vector
    this->vectorBegin();
    this->vectorEnd();

}

/***********************************************************
* Destructor                                               *
***********************************************************/

TrackedSourceVector::~TrackedSourceVector()
{

}

/***********************************************************
* Fill the vector                                          *
***********************************************************/

// +-------------------------------------------------------+
// | Begin filling the vector                              |
// +-------------------------------------------------------+

void TrackedSourceVector::vectorBegin()
{

    this->listTrackedSource.clear();
    this->numberTrackedSources = 0;

}

// +-------------------------------------------------------+
// | Add a new potential source in the vector              |
// +-------------------------------------------------------+

void TrackedSourceVector::vectorAdd(const TrackedSource _trackedSource)
{

    this->listTrackedSource.push_front(_trackedSource);
    this->numberTrackedSources++;

}

// +-------------------------------------------------------+
// | End filling the vector                                |
// +-------------------------------------------------------+

void TrackedSourceVector::vectorEnd()
{

}

/***********************************************************
* Check for an existing tracked source ID in the vector    *
***********************************************************/

const bool TrackedSourceVector::trackedSourceIDExists(const int _trackedSourceID)
{

    return (this->getColorIDFromTrackedSourceID(_trackedSourceID) != -1);

}

/***********************************************************
* Get the color ID of a tracked source from its ID         *
***********************************************************/

const signed int TrackedSourceVector::getColorIDFromTrackedSourceID(const int _trackedSourceID)
{

    // Loop for each tracked source in the vector
    // and check if the ID matches
    for (int indexTrackedSource = 0; indexTrackedSource < this->numberTrackedSources; indexTrackedSource++)
    {

        if (this->listTrackedSource[indexTrackedSource].getTrackedSourceID() == _trackedSourceID)
        {

            return (this->listTrackedSource[indexTrackedSource].getColorID());

        }

    }

    return -1;

}

/***********************************************************
* Accessors                                                *
***********************************************************/

// +-------------------------------------------------------+
// | Potential source at the given index                   |
// +-------------------------------------------------------+

const TrackedSource TrackedSourceVector::getTrackedSource(const int _indexTrackedSource)
{
    return (this->listTrackedSource[_indexTrackedSource]);
}

// +-------------------------------------------------------+
// | Number of potential sources in the vector             |
// +-------------------------------------------------------+

const int TrackedSourceVector::getNumberTrackedSources()
{
    return (this->numberTrackedSources);
}

/***********************************************************
* Mutators                                                 *
***********************************************************/

// +-------------------------------------------------------+
// | Potential source at the given index                   |
// +-------------------------------------------------------+

void TrackedSourceVector::setTrackedSource(const int _indexTrackedSource, const TrackedSource _trackedSource)
{
    this->listTrackedSource[_indexTrackedSource] = _trackedSource;
}
