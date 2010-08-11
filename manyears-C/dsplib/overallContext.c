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

#include "overallContext.h"

struct objOverall createEmptyOverallContext()
{

    struct objOverall tmp;

    tmp.myBeamformer = (struct objBeamformer*) malloc(sizeof(struct objBeamformer));
    tmp.myMixture = (struct objMixture*) malloc(sizeof(struct objMixture));
    tmp.myPotentialSources = (struct objPotentialSources*) malloc(sizeof(struct objPotentialSources));
    tmp.myTrackedSources = (struct objTrackedSources*) malloc(sizeof(struct objTrackedSources));

    tmp.myParameters = (struct ParametersStruct*) malloc(sizeof(struct ParametersStruct));

    return tmp;

}

void deleteOverallContext(struct objOverall myContext)
{

    free(myContext.myBeamformer);
    free(myContext.myMixture);
    free(myContext.myPotentialSources);
    free(myContext.myTrackedSources);

    free(myContext.myParameters);

}
