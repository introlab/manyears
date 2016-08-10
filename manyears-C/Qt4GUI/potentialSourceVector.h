#ifndef POTENTIALSOURCEVECTOR_H
#define POTENTIALSOURCEVECTOR_H

#include <QVector>

#include "potentialSource.h"

class PotentialSourceVector
{

public:

    /***********************************************************
    * Constructor                                              *
    ***********************************************************/

    PotentialSourceVector();

    /***********************************************************
    * Destructor                                               *
    ***********************************************************/

    ~PotentialSourceVector();

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

    void vectorAdd(const PotentialSource _potentialSource);

    // +-------------------------------------------------------+
    // | End filling the vector                                |
    // +-------------------------------------------------------+

    void vectorEnd();

    /***********************************************************
    * Accessors                                                *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Potential source at the given index                   |
    // +-------------------------------------------------------+

    PotentialSource getPotentialSource(const int _indexPotentialSource);

    // +-------------------------------------------------------+
    // | Number of potential sources in the vector             |
    // +-------------------------------------------------------+

    const int getNumberPotentialSources();

    /***********************************************************
    * Mutators                                                 *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Potential source at the given index                   |
    // +-------------------------------------------------------+

    void setPotentialSource(const int _indexPotentialSource, const PotentialSource _potentialSource);

private:

    /***********************************************************
    * Fields                                                   *
    ***********************************************************/

    int numberPotentialSources;

    QVector <PotentialSource> listPotentialSource;

};


#endif
