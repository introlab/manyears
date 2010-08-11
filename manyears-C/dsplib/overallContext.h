/*******************************************************************************
 * ManyEars: Overall context - Header                                          *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Author: François Grondin                                                    *
 * Original Code: Jean-Marc Valin                                              *
 * Modified Code: Simon Brière                                                 *
 * Version: 1.1.0                                                              *
 * Date: July 1st, 2010                                                        *
 *                                                                             *
 * Disclaimer: This software is provided "as is". Use it at your own risk.     *
 ******************************************************************************/

#ifndef OVERALLCONTEXT_H
#define OVERALLCONTEXT_H

// =============================================================================

#include "beamformer.h"
#include "mixture.h"
#include "potentialSources.h"
#include "trackedSources.h"
#include "parameters.h"

/*******************************************************************************
 * Structures                                                                  *
 ******************************************************************************/

    struct objOverall
    {

        // +-------------------------------------------------------------------+
        // | Objects                                                           |
        // +-------------------------------------------------------------------+

        struct ParametersStruct* myParameters;

        struct objBeamformer* myBeamformer;
        struct objMixture* myMixture;
        struct objPotentialSources* myPotentialSources;
        struct objTrackedSources* myTrackedSources;

    };

/*******************************************************************************
 * Prototypes                                                                  *
 ******************************************************************************/

    struct objOverall createEmptyOverallContext();

    void deleteOverallContext(struct objOverall myContext);

// =============================================================================

#endif
