#ifndef TRACKEDSOURCEVECTOR_H
#define TRACKEDSOURCEVECTOR_H

#include <QVector>

#include "trackedSource.h"

class TrackedSourceVector
{

public:

    /***********************************************************
    * Constructor                                              *
    ***********************************************************/

    TrackedSourceVector();

    /***********************************************************
    * Destructor                                               *
    ***********************************************************/

    ~TrackedSourceVector();

    /***********************************************************
    * Fill the vector                                          *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Begin filling the vector                              |
    // +-------------------------------------------------------+

    void vectorBegin();

    // +-------------------------------------------------------+
    // | Add a new potential source in the vector              |
    // +-------------------------------------------------------+

    void vectorAdd(const TrackedSource _trackedSource);

    // +-------------------------------------------------------+
    // | End filling the vector                                |
    // +-------------------------------------------------------+

    void vectorEnd();

    /***********************************************************
    * Check for an existing tracked source ID in the vector    *
    ***********************************************************/

    const bool trackedSourceIDExists(const int _trackedSourceID);

    /***********************************************************
    * Get the color ID of a tracked source from its ID         *
    ***********************************************************/

    const signed int getColorIDFromTrackedSourceID(const int _trackedSourceID);


    /***********************************************************
    * Accessors                                                *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Potential source at the given index                   |
    // +-------------------------------------------------------+

    const TrackedSource getTrackedSource(const int _indexTrackedSource);

    // +-------------------------------------------------------+
    // | Number of potential sources in the vector             |
    // +-------------------------------------------------------+

    const int getNumberTrackedSources();

    /***********************************************************
    * Mutators                                                 *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Potential source at the given index                   |
    // +-------------------------------------------------------+

    void setTrackedSource(const int _indexTrackedSource, const TrackedSource _trackedSource);

private:

    /***********************************************************
    * Fields                                                   *
    ***********************************************************/

    int numberTrackedSources;

    QVector <TrackedSource> listTrackedSource;

};


#endif
