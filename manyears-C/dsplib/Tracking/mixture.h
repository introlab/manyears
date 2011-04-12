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
 *                                                                             *
 *******************************************************************************
 *                                                                             *
 * ManyEars has been created and developped at:                                *
 *                                                                             *
 * IntroLab, Universite de Sherbrooke, Sherbrooke, Quebec, Canada              *
 *                                                                             *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * The following articles relate to the ManyEars project:                      *
 *                                                                             *
 * S. Brière, J.-M. Valin, F. Michaud, Dominic Létourneau, Embedded Auditory   *
 *     System for Small Mobile Robots, Proc. International Conference on       *
 *     Robotics and Automation (ICRA), 2008.                                   *
 *                                                                             *
 * J.-M. Valin, S. Yamamoto, J. Rouat, F. Michaud, K. Nakadai, H. G. Okuno,    *
 *     Robust Recognition of Simultaneous Speech By a Mobile Robot, IEEE       *
 *     Transactions on Robotics, Vol. 23, No. 4, pp. 742-752, 2007.            *
 *                                                                             *
 * J.-M. Valin, F. Michaud, J. Rouat, Robust Localization and Tracking of      *
 *     Simultaneous Moving Sound Sources Using Beamforming and Particle        *
 *     Filtering. Robotics and Autonomous Systems Journal (Elsevier), Vol. 55, *
 *     No. 3, pp. 216-228, 2007.                                               *
 *                                                                             *
 * S. Yamamoto, K. Nakadai, M. Nakano, H. Tsujino, J.-M. Valin, K. Komatani,   *
 *     T. Ogata, H. G. Okuno, Simultaneous Speech Recognition based on         *
 *     Automatic Missing-Feature Mask Generation integrated with Sound Source  *
 *     Separation (in Japanese). Journal of Robotic Society of Japan, Vol. 25, *
 *     No. 1, 2007.                                                            *
 *                                                                             *
 * J.-M. Valin, F. Michaud, J. Rouat, Robust 3D Localization and Tracking of   *
 *     Sound Sources Using Beamforming and Particle Filtering. Proc. IEEE      *
 *     International Conference on Acoustics, Speech and Signal Processing     *
 *     (ICASSP), pp. 841-844, 2006.                                            *
 *                                                                             *
 * S. Briere, D. Letourneau, M. Frechette, J.-M. Valin, F. Michaud, Embedded   *
 *     and integration audition for a mobile robot. Proceedings AAAI Fall      *
 *     Symposium Workshop Aurally Informed Performance: Integrating Machine    *
 *     Listening and Auditory Presentation in Robotic Systems, FS-06-01, 6-10, *
 *     2006                                                                    *
 *                                                                             *
 * S. Yamamoto, K. Nakadai, J.-M. Valin, J. Rouat, F. Michaud, K. Komatani,    *
 *     T. Ogata, H. G. Okuno, Making a robot recognize three simultaneous      *
 *     sentences in real-time. Proceedings of IEEE/RSJ International           *
 *     Conference on Intelligent Robots and Systems (IROS), 2005.              *
 *                                                                             *
 * M. Murase, S. Yamamoto, J.-M. Valin, K. Nakadai, K. Yamada, K. Komatani,    *
 *     T. Ogata, H. G. Okuno, Multiple Moving Speaker Tracking by Microphone   *
 *     Array on Mobile Robot. Proc. European Conference on Speech              *
 *     Communication and Technology (Interspeech), 2005.                       *
 *                                                                             *
 * S. Yamamoto, J.-M. Valin, K. Nakadai, J. Rouat, F. Michaud, T. Ogata, H.    *
 *     G. Okuno, Enhanced Robot Speech Recognition Based on Microphone Array   *
 *     Source Separation and Missing Feature Theory. Proc. International       *
 *     Conference on Robotics and Automation (ICRA), 2005.                     *
 *                                                                             *
 * J.-M. Valin, J. Rouat, F. Michaud, Enhanced Robot Audition Based on         *
 *     Microphone Array Source Separation with Post-Filter. Proc. IEEE/RSJ     *
 *     International Conference on Intelligent Robots and Systems (IROS),      *
 *     pp. 2123-2128, 2004.                                                    *
 *                                                                             *
 * J.-M. Valin, F. Michaud, J. Rouat, D. Létourneau, Robust Sound Source       *
 *     Localization Using a Microphone Array on a Mobile Robot. Proc. IEEE/RSJ *
 *     International Conference on Intelligent Robots and Systems (IROS),      *
 *     pp. 1228-1233, 2003.                                                    *
 *                                                                             *
 * J.-M. Valin, F. Michaud, B. Hadjou, J. Rouat, Localization of Simultaneous  *
 *     Moving Sound Sources for Mobile Robot Using a Frequency-Domain Steered  *
 *     Beamformer Approach. Proc. IEEE International Conference on Robotics    *
 *     and Automation (ICRA), pp. 1033-1038, 2004.                             *
 *                                                                             *
 * J.-M. Valin, J. Rouat, F. Michaud, Microphone Array Post-Filter for         *
 *     Separation of Simultaneous Non-Stationary Sources. Proc. IEEE           *
 *     International Conference on Acoustics, Speech and Signal Processing     *
 *     (ICASSP), pp. 221-224, 2004.                                            *
 *                                                                             *
 ******************************************************************************/

#ifndef MIXTURE_H
#define MIXTURE_H

#include <math.h>
#include <stdlib.h>

#include "../hardware.h"
#include "../parameters.h"

#include "../Localisation/beamformer.h"
#include "../Tracking/filter.h"
#include "../Tracking/trackedSources.h"
#include "../Utilities/idManager.h"

/*******************************************************************************
 * Types (Do not edit)                                                         *
 ******************************************************************************/

    // Define the value for an ID that represents no filter
    #define      MIXTURE_NOFILTER              ID_NOSOURCE

    // Define the maximum value for an ID before wrapping around
    #define      MIXTURE_IDMAX                 10000

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
            unsigned int MIXTURE_NBFILTERS;

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
            unsigned int MIXTURE_CONFIRMCOUNT;

            // Maximum number of frames while the source has not been tracked
            // in order to delete this tracking for probation time
            unsigned int MIXTURE_CUMULATIVETIMEPROB;

            // Maximum number of frames while the source has not been tracked
            // in order to delete this tracking for time 1
            unsigned int MIXTURE_CUMULATIVETIME1;

            // Maximum number of frames while the source has not been tracked
            // in order to delete this tracking for time 2
            unsigned int MIXTURE_CUMULATIVETIME2;

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
            unsigned int BF_MAXSOURCES;

            // Number of particles in filters
            unsigned int FILTER_NBPARTICLES;

            // Standard deviation
            float FILTER_STDDEVIATION;

            // Mixture base for f
            unsigned int MIXTURE_BASEF;

        // +-------------------------------------------------------------------+
        // | Variables                                                         |
        // +-------------------------------------------------------------------+

            // List of filters
            struct objFilter *listFilters;

            // Identification of filters
            ID_TYPE *filterID;

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

            // Number of filters tracking sources
            unsigned char numberTrackingInProgress;

            // ID Manager object
            struct objIDManager idManager;

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

#endif
