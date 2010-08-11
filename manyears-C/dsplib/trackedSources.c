/*******************************************************************************
 * ManyEars: Tracked Sources - Source Code                                     *
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

#include "trackedSources.h"

/*******************************************************************************
 * trackedSourcesInit                                                          *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myTrackedSources                                               *
 *                              The tracked sources object to be initialized   *
 *              myParameters    List of parameters that is used to configure   *
 *                              the object                                     *
 *                                                                             *
 * Outputs:     myTrackedSources                                               *
 *                              The tracked sources object to be initialized   *
 *                                                                             *
 * Description: This function initializes the tracked sources object           *
 *                                                                             *
 ******************************************************************************/

void trackedSourcesInit(struct objTrackedSources *myTrackedSources, struct ParametersStruct *myParameters)
{

    // Index to loop for each source
    unsigned char indexSource;

    /***************************************************************************
    * Step 1: Load parameters                                                  *
    ***************************************************************************/

    myTrackedSources->MIXTURE_NBFILTERS = myParameters->P_MIXTURE_NBFILTERS;

    /***************************************************************************
    * Step 1: Create arrays                                                    *
    ***************************************************************************/

    // Sources ID
    myTrackedSources->sourcesID = (signed char *) newTable1D(myTrackedSources->MIXTURE_NBFILTERS, sizeof(signed char));

    // Sources position
    myTrackedSources->sourcesPosition = (float **) newTable2D(myTrackedSources->MIXTURE_NBFILTERS, 3, sizeof(float));

    /***************************************************************************
    * Step 2: Initialize values                                                *
    ***************************************************************************/

    // Set no source for each spot
    for (indexSource = 0; indexSource < myTrackedSources->MIXTURE_NBFILTERS; indexSource++)
    {

        myTrackedSources->sourcesID[indexSource] = MIXTURE_NOSOURCE;

    }

    // Set the number of sources to 0
    myTrackedSources->numberSources = 0;

    // Save the maximum number of sources
    myTrackedSources->maxNumberSources = myTrackedSources->MIXTURE_NBFILTERS;

}

/*******************************************************************************
 * trackedSourcesGetMaxNumberSources                                           *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myTrackedSources                                               *
 *                              The tracked sources object to be used          *
 *                                                                             *
 * Outputs:     unsigned int    The max number of tracked sources              *
 *                                                                             *
 * Description: This function returns the max number of potential sources      *
 *                                                                             *
 ******************************************************************************/

unsigned int trackedSourcesGetMaxNumberSources(struct objTrackedSources *myTrackedSources)
{
    return myTrackedSources->maxNumberSources;
}

/*******************************************************************************
 * trackedSourcesGetNumberTrackedSources                                       *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myTrackedSources                                               *
 *                              The tracked sources object to be used          *
 *                                                                             *
 * Outputs:     unsigned int    The number of sources currently tracked        *
 *                                                                             *
 * Description: This function returns the number of sources currently tracked  *
 *                                                                             *
 ******************************************************************************/

unsigned int trackedSourcesGetNumberTrackedSources(struct objTrackedSources *myTrackedSources)
{
    return myTrackedSources->numberSources;
}

/*******************************************************************************
 * trackedSourcesGetID                                                         *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myTrackedSources                                               *
 *                              The tracked sources object to be used          *
 *              sourceIndex     The source index                               *
 *                                                                             *
 * Outputs:     signed int      The value of the ID of the source              *
 *                              which index is given                           *
 *                                                                             *
 * Description: This function returns the ID of the source which index         *
 *              is given by sourceIndex                                        *
 *                                                                             *
 ******************************************************************************/

signed int trackedSourcesGetID(struct objTrackedSources *myTrackedSources, unsigned int sourceIndex)
{
    return myTrackedSources->sourcesID[sourceIndex];
}

/*******************************************************************************
 * trackedSourcesGetX                                                          *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myTrackedSources                                               *
 *                              The tracked sources object to be used          *
 *              sourceIndex     The source index                               *
 *                                                                             *
 * Outputs:     float           The value of the X position of the source      *
 *                              which index is given                           *
 *                                                                             *
 * Description: This function returns the X position of the source which index *
 *              is given by sourceIndex                                        *
 *                                                                             *
 ******************************************************************************/

float trackedSourcesGetX(struct objTrackedSources *myTrackedSources, unsigned int sourceIndex)
{
    return myTrackedSources->sourcesPosition[sourceIndex][0];
}

/*******************************************************************************
 * trackedSourcesGetY                                                          *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myTrackedSources                                               *
 *                              The tracked sources object to be used          *
 *              sourceIndex     The source index                               *
 *                                                                             *
 * Outputs:     float           The value of the Y position of the source      *
 *                              which index is given                           *
 *                                                                             *
 * Description: This function returns the Y position of the source which index *
 *              is given by sourceIndex                                        *
 *                                                                             *
 ******************************************************************************/

float trackedSourcesGetY(struct objTrackedSources *myTrackedSources, unsigned int sourceIndex)
{
    return myTrackedSources->sourcesPosition[sourceIndex][1];
}

/*******************************************************************************
 * trackedSourcesGetZ                                                          *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myTrackedSources                                               *
 *                              The tracked sources object to be used          *
 *              sourceIndex     The source index                               *
 *                                                                             *
 * Outputs:     float           The value of the Z position of the source      *
 *                              which index is given                           *
 *                                                                             *
 * Description: This function returns the Z position of the source which index *
 *              is given by sourceIndex                                        *
 *                                                                             *
 ******************************************************************************/

float trackedSourcesGetZ(struct objTrackedSources *myTrackedSources, unsigned int sourceIndex)
{
    return myTrackedSources->sourcesPosition[sourceIndex][2];
}

/*******************************************************************************
 * trackedSourcesTerminate                                                     *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myTrackedSources                                               *
 *                             The tracked sources object to be terminated     *
 *                                                                             *
 * Outputs:     (none)                                                         *
 *                                                                             *
 * Description: This function terminates the tracked sources object            *
 *                                                                             *
 ******************************************************************************/

void trackedSourcesTerminate(struct objTrackedSources *myTrackedSources)
{

    // Free memory
    deleteTable1D((void*) myTrackedSources->sourcesID);
    deleteTable2D((void**) myTrackedSources->sourcesPosition);

}
