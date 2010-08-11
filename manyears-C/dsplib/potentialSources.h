/*******************************************************************************
 * ManyEars: Potential Sources - Header                                        *
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

#ifndef POTENTIALSOURCES_H
#define POTENTIALSOURCES_H

// =============================================================================

#include "parameters.h"

/*******************************************************************************
 * Structures                                                                  *
 ******************************************************************************/

struct objPotentialSources
{

    // +-------------------------------------------------------------------+
    // | Parameters                                                        |
    // +-------------------------------------------------------------------+

        // Maximum number of potential sources
        int BF_MAXSOURCES;

    // +-------------------------------------------------------------------+
    // | Variables                                                         |
    // +-------------------------------------------------------------------+

        // Sources position
        float **sourcesPosition;

        // Sources energy
        float *sourcesEnergy;

        // Number of potential sources
        int numberSources;

};

/*******************************************************************************
 * Prototypes                                                                  *
 ******************************************************************************/

void potentialSourcesInit(struct objPotentialSources *myPotentialSources, struct ParametersStruct *myParameters);

unsigned int potentialSourcesGetNumberSources(struct objPotentialSources *myPotentialSources);

float potentialSourcesGetX(struct objPotentialSources *myPotentialSources, unsigned int sourceIndex);

float potentialSourcesGetY(struct objPotentialSources *myPotentialSources, unsigned int sourceIndex);

float potentialSourcesGetZ(struct objPotentialSources *myPotentialSources, unsigned int sourceIndex);

float potentialSourcesGetEnergy(struct objPotentialSources *myPotentialSources, unsigned int sourceIndex);

void potentialSourcesTerminate(struct objPotentialSources *myPotentialSources);

// =============================================================================

#endif
