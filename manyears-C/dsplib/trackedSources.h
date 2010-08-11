/*******************************************************************************
 * ManyEars: Tracked Sources - Header                                          *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Author: François Grondin                                                    *
 * Original Code: Jean-Marc Valin                                              *
 * Modified Code: Simon Brière                                                 *
 * Version: 1.1.0                                                              *
 * Date: June 29th, 2010                                                       *
 *                                                                             *
 * Disclaimer: This software is provided "as is". Use it at your own risk.     *
 ******************************************************************************/

#ifndef TRACKEDSOURCES_H
#define TRACKEDSOURCES_H

// =============================================================================

#include "parameters.h"

/*******************************************************************************
 * Types (Do not edit)                                                         *
 ******************************************************************************/

// Define the value for an ID that represents no source
#define      MIXTURE_NOSOURCE              -1

struct objTrackedSources
{

    // +-------------------------------------------------------------------+
    // | Parameters                                                        |
    // +-------------------------------------------------------------------+

        // Maximum number of filter
        int MIXTURE_NBFILTERS;

    // +-------------------------------------------------------------------+
    // | Variables                                                         |
    // +-------------------------------------------------------------------+

        // Sources ID
        signed char *sourcesID;

        // Sources position
        float **sourcesPosition;

        // Number of sources
        unsigned char numberSources;

        // Maximum number of sources
        unsigned char maxNumberSources;

};

/*******************************************************************************
 * Prototypes                                                                  *
 ******************************************************************************/

void trackedSourcesInit(struct objTrackedSources *mySources, struct ParametersStruct *myParameters);

unsigned int trackedSourcesGetMaxNumberSources(struct objTrackedSources *myTrackedSources);

unsigned int trackedSourcesGetNumberTrackedSources(struct objTrackedSources *myTrackedSources);

signed int trackedSourcesGetID(struct objTrackedSources *myTrackedSources, unsigned int sourceIndex);

float trackedSourcesGetX(struct objTrackedSources *myTrackedSources, unsigned int sourceIndex);

float trackedSourcesGetY(struct objTrackedSources *myTrackedSources, unsigned int sourceIndex);

float trackedSourcesGetZ(struct objTrackedSources *myTrackedSources, unsigned int sourceIndex);

void trackedSourcesTerminate(struct objTrackedSources *mySources);

// =============================================================================

#endif
