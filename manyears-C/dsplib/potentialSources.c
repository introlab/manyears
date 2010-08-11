/*******************************************************************************
 * ManyEars: Potential Sources - Source Code                                   *
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

#include "potentialSources.h"

/*******************************************************************************
 * potentialSourcesInit                                                        *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myPotentialSources                                             *
 *                              The potential sources object to be initialized *
 *              myParameters    List of parameters that is used to configure   *
 *                              the object                                     *
 *                                                                             *
 * Outputs:     myPotentialSources                                             *
 *                              The initialized potential sources object       *
 *                                                                             *
 * Description: This function initializes the potential sources object         *
 *                                                                             *
 ******************************************************************************/

void potentialSourcesInit(struct objPotentialSources *myPotentialSources, struct ParametersStruct *myParameters)
{

    /***************************************************************************
    * Step 1: Load parameters                                                  *
    ***************************************************************************/

    myPotentialSources->BF_MAXSOURCES = myParameters->P_BF_MAXSOURCES;

    /***************************************************************************
    * Step 1: Create arrays                                                    *
    ***************************************************************************/

    // Sources position
    myPotentialSources->sourcesPosition = (float**) newTable2D(myPotentialSources->BF_MAXSOURCES, 3, sizeof(float));

    // Sources energy
    myPotentialSources->sourcesEnergy = (float*) newTable1D(myPotentialSources->BF_MAXSOURCES, sizeof(float));

    // Number of sources
    myPotentialSources->numberSources = myPotentialSources->BF_MAXSOURCES;


}

/*******************************************************************************
 * potentialSourcesGetNumberSources                                            *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myPotentialSources                                             *
 *                              The potential sources object to be used        *
 *                                                                             *
 * Outputs:     unsigned int    The number of potential sources                *
 *                                                                             *
 * Description: This function returns the max number of potential sources      *
 *                                                                             *
 ******************************************************************************/

unsigned int potentialSourcesGetNumberSources(struct objPotentialSources *myPotentialSources)
{
    return myPotentialSources->numberSources;
}

/*******************************************************************************
 * potentialSourcesGetX                                                        *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myPotentialSources                                             *
 *                              The potential sources object to be used        *
 *              sourceIndex     The source index                               *
 *                                                                             *
 * Outputs:     float           The value of the X position of the source      *
 *                              which index is given                           *
 *                                                                             *
 * Description: This function returns the X position of the source which index *
 *              is given by sourceIndex                                        *
 *                                                                             *
 ******************************************************************************/

float potentialSourcesGetX(struct objPotentialSources *myPotentialSources, unsigned int sourceIndex)
{
    return myPotentialSources->sourcesPosition[sourceIndex][0];
}

/*******************************************************************************
 * potentialSourcesGetY                                                        *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myPotentialSources                                             *
 *                              The potential sources object to be used        *
 *              sourceIndex     The source index                               *
 *                                                                             *
 * Outputs:     float           The value of the Y position of the source      *
 *                              which index is given                           *
 *                                                                             *
 * Description: This function returns the Y position of the source which index *
 *              is given by sourceIndex                                        *
 *                                                                             *
 ******************************************************************************/

float potentialSourcesGetY(struct objPotentialSources *myPotentialSources, unsigned int sourceIndex)
{
    return myPotentialSources->sourcesPosition[sourceIndex][1];
}

/*******************************************************************************
 * potentialSourcesGetZ                                                        *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myPotentialSources                                             *
 *                              The potential sources object to be used        *
 *              sourceIndex     The source index                               *
 *                                                                             *
 * Outputs:     float           The value of the Z position of the source      *
 *                              which index is given                           *
 *                                                                             *
 * Description: This function returns the Z position of the source which index *
 *              is given by sourceIndex                                        *
 *                                                                             *
 ******************************************************************************/

float potentialSourcesGetZ(struct objPotentialSources *myPotentialSources, unsigned int sourceIndex)
{
    return myPotentialSources->sourcesPosition[sourceIndex][2];
}

/*******************************************************************************
 * potentialSourcesGetEnergy                                                   *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myPotentialSources                                             *
 *                              The potential sources object to be used        *
 *              sourceIndex     The source index                               *
 *                                                                             *
 * Outputs:     float           The value of the energy of the source          *
 *                              which index is given                           *
 *                                                                             *
 * Description: This function returns the energy of the source which index     *
 *              is given by sourceIndex                                        *
 *                                                                             *
 ******************************************************************************/

float potentialSourcesGetEnergy(struct objPotentialSources *myPotentialSources, unsigned int sourceIndex)
{
    return myPotentialSources->sourcesEnergy[sourceIndex];
}

/*******************************************************************************
 * potentialSourcesTerminate                                                   *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myPotentialSources                                             *
 *                              The potential sources object to be terminated  *
 *                                                                             *
 * Outputs:     (none)                                                         *
 *                                                                             *
 * Description: This function terminates the potential sources object          *
 *                                                                             *
 ******************************************************************************/

void potentialSourcesTerminate(struct objPotentialSources *myPotentialSources)
{

    // Free memory
    deleteTable2D((void**) myPotentialSources->sourcesPosition);
    deleteTable1D((void*) myPotentialSources->sourcesEnergy);

}
