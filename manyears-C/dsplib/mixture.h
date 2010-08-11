/******************************************************************************* 
 * ManyEars: Mixture - Header                                                  *
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

#ifndef MIXTURE_H
#define MIXTURE_H

// =============================================================================

#include <stdlib.h>

#include "filter.h"
#include "beamformer.h"
#include "trackedSources.h"

#include "hardware.h"

#include "parameters.h"

/*******************************************************************************
 * Types (Do not edit)                                                         *
 ******************************************************************************/

    // Define the value for an ID that represents no filter
    #define      MIXTURE_NOFILTER              -1

    // Define the maximum value for an ID before wrapping around
    #define      MIXTURE_IDMAX                 100

    // Define the conflict indicator
    #define      MIXTURE_CONFLICT              1

    // Define the no conflict indicator
    #define      MIXTURE_NOCONFLICT            0

    // Define the source does not exist indicator
    #define      MIXTURE_SOURCEDOESNOTEXIST    0

    // Define the source exists indicator
    #define      MIXTURE_SOURCEEXISTS          1

/*******************************************************************************
 * Structures                                                                  *
 ******************************************************************************/

    struct objMixture
    {

        // +-------------------------------------------------------------------+
        // | Parameters                                                        |
        // +-------------------------------------------------------------------+

            // +---------------------------------------------------------------+
            // | General                                                       |
            // +---------------------------------------------------------------+           

            // Maximum number of filter
            int MIXTURE_NBFILTERS;

            // +---------------------------------------------------------------+
            // | Probability                                                   |
            // +---------------------------------------------------------------+

            // Probability that a new source appears: Pnew
            float MIXTURE_PNEW;

            // Probability that a false detection occurs
            float MIXTURE_PFALSE;

            // Threshold in order to get Pq from beamformer values
            float MIXTURE_ET;

            // +---------------------------------------------------------------+
            // | Source tracking                                               |
            // +---------------------------------------------------------------+

            // Threshold to reach in order to track a new source
            float MIXTURE_NEWTHRESHOLD;

            // +---------------------------------------------------------------+
            // | Source addition/removing                                      |
            // +---------------------------------------------------------------+

            // Threshold to reach in order to confirm a source really exists
            float MIXTURE_CONFIRMEXISTS;

            // Threshold to count a source as existing
            float MIXTURE_CONFIRMCOUNTTS;

            // Number of times the threshold must be reached
            int MIXTURE_CONFIRMCOUNT;

            // Maximum number of frames while the source has not been tracked
            // in order to delete this tracking for probation time
            int MIXTURE_CUMULATIVETIMEPROB;

            // Maximum number of frames while the source has not been tracked
            // in order to delete this tracking for time 1
            int MIXTURE_CUMULATIVETIME1;

            // Maximum number of frames while the source has not been tracked
            // in order to delete this tracking for time 2
            int MIXTURE_CUMULATIVETIME2;

            // Minimum value to consider to say that source is not observed for
            // probation time
            float MIXTURE_TOBSPROB;

            // Minimum value to consider to say that source is not observed for
            // time 1
            float MIXTURE_TOBS1;

            // Minimum value to consider to say that source is not observed for
            // time 2
            float MIXTURE_TOBS2;

            // Minimum horizontal angle difference that the new source needs to
            // have with the already existing filters (rad)
            float MIXTURE_NEWANGLE;

            // +---------------------------------------------------------------+
            // | Others                                                        |
            // +---------------------------------------------------------------+

            // Maximum number of potential sources
            int BF_MAXSOURCES;

            // Number of particles in filters
            int FILTER_NBPARTICLES;

            // Standard deviation
            float FILTER_STDDEVIATION;

            // Mixture base for f
            int MIXTURE_BASEF;

        // +-------------------------------------------------------------------+
        // | Variables                                                         |
        // +-------------------------------------------------------------------+

            // List of filters
            struct objFilter *listFilters;

            // Identification of filters
            signed char *filterID;

            // Confirm that the filter tracks a source that exists
            float *confirmSourceExists;

            // Sum of all probabilities when source does not exist yet
            float *sumProbSourceExists;

            // Flag that the filter tracks a source that exists
            unsigned char *flagSourceExists;

            // Counts the number of time the source may exist
            unsigned int *countSourceExists;

            // Probation cumulative time during which each filter has tracked the
            // source
            unsigned int *cumulativeTimeProb;

            // First cumulative time during which each filter has tracked the
            // source
            unsigned int *cumulativeTime1;

            // Second cumulative time during which each filter has tracked the
            // source
            unsigned int *cumulativeTime2;

            // Last ID used
            signed char lastID;

            // Number of filters tracking sources
            unsigned char numberTrackingInProgress;

        // +-------------------------------------------------------------------+
        // | Temporary variables                                               |
        // +-------------------------------------------------------------------+

            // Pq
            //
            // Size: [BF_MAXSOURCES]
            float* Pq;

            // P(f(q)):
            // Pfq[0][q] = False detection for source q
            // Pfq[1][q] = New source at source q
            // Pfq[2][q] = Source q assigned to filter 0
            // Pfq[3][q] = Source q assigned to filter 1
            // ...
            // Pfq[2 + Number of filters][q] = Source q assigned to filter (number of filters - 1)
            //
            // Size: [2 + MIXTURE_NBFILTERS][BF_MAXSOURCES]
            float** Pfq;

            // P(Oq|f(q)):
            // POq_fq[0][q] = False detection for source q
            // POq_fq[1][q] = New source at source q
            // POq_fq[2][q] = Source q assigned to filter 0
            // POq_fq[3][q] = Source q assigned to filter 1
            // ...
            // POq_fq[2 + Number of filters][q] = Source q assigned to filter (number of filters - 1)
            //
            // Size: [2 + MIXTURE_NBFILTERS][BF_MAXSOURCES]
            float** POq_fq;

            // p(Oq|xvj,i):
            //
            // Size: [FILTER_NBPARTICLES]
            float* pOq_xvji;

            // Pq,j(t)
            //
            // Size: [MIXTURE_NBFILTERS][BF_MAXSOURCES]
            float** Pqj_t;

            // Pq(t)(H0)
            //
            // Size: [BF_MAXSOURCES]
            float* Pq_t_H0;

            // Pq(t)(H2)
            //
            // Size: [BF_MAXSOURCES]
            float* Pq_t_H2;

            // f(q)
            //
            // Size: [BF_MAXSOURCES]
            unsigned char* fq;

            // Filter repetition indicator
            //
            // Size: [MIXTURE_NBFILTERS]
            unsigned char* filterConflict;

    };

/*******************************************************************************
 * Prototypes                                                                  *
 ******************************************************************************/

void mixtureInit(struct objMixture *myMixture, struct ParametersStruct *myParameters);

void mixtureTerminate(struct objMixture *myMixture);

void mixtureUpdate(struct objMixture *myMixture, struct objTrackedSources *myTrackedSources, struct objPotentialSources *myPotentialSources);

void mixtureUtilGetNextID(struct objMixture *myMixture);

// =============================================================================

#endif
