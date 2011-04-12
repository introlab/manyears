/******************************************************************************* 
 * ManyEars: Filter - Header                                                   *
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

#ifndef FILTER_H
#define FILTER_H

#include <math.h>
#include <stdlib.h>

#include "../hardware.h"
#include "../parameters.h"
#include "../Localisation/potentialSources.h"
#include "../Utilities/dynamicMemory.h"
#include "../Utilities/mathadvanced.h"
#include "../Utilities/randGen.h"

/*******************************************************************************
 * Types (Do not edit)                                                         *
 ******************************************************************************/

    // Particle states
    #define             FILTER_PARTICLE_STOP     0      // Stop
    #define             FILTER_PARTICLE_CST      1      // Constant velocity
    #define             FILTER_PARTICLE_EXC      2      // Excitation

    // Buffer
    #define             FILTER_BUFFERNOTFULL     0      // Buffer has not been full yet
    #define             FILTER_BUFFERFULL        1      // Buffer has been full

/*******************************************************************************
 * Structures                                                                  *
 ******************************************************************************/

    struct objFilter
    {

        // +---------------------------------------------------------------+
        // | Parameters                                                    |
        // +---------------------------------------------------------------+

            // +-----------------------------------------------------------+
            // | General                                                   |
            // +-----------------------------------------------------------+

                // Number of particles per filter
                unsigned int FILTER_NBPARTICLES;

                // Standard deviation of the new particles
                float FILTER_STDDEVIATION;

            // +-----------------------------------------------------------+
            // | Prediction parameters                                     |
            // +-----------------------------------------------------------+

                // Stationary sources
                float FILTER_ALPHASTOP;
                float FILTER_BETASTOP;

                // Constant velocity source
                float FILTER_ALPHACONST;
                float FILTER_BETACONST;

                // Accelerated source
                float FILTER_ALPHAEXC;
                float FILTER_BETAEXC;

                // Additional inertia factors
                float FILTER_INERTIAX;
                float FILTER_INERTIAY;
                float FILTER_INERTIAZ;

                // Time interval between updates
                float FILTER_DELTAT;

                // Percentage of particles to have its state updated
                float FILTER_STATEUPDT;

                // Percentage of new particles to be stopped
                // (make sure total sums up to 1 (100%))
                float FILTER_NEWSTOP;

                // Percentage of new particles to have a constant velocity
                // (make sure total sums up to 1 (100%))
                float FILTER_NEWCONST;

                // Percentage of new particles to be excided
                // (make sure total sums up to 1 (100%))
                float FILTER_NEWEXC;

            // +-----------------------------------------------------------+
            // | Probability parameters                                    |
            // +-----------------------------------------------------------+

                // Probability that the source was active and is still active:
                // P(Aj(t)|Aj(t-1))
                float FILTER_AJT_AJTM1;

                // Probability that the source was not active and becomes active:
                // P(Aj(t)|¬Aj(t-1))
                float FILTER_AJT_NOTAJTM1;

                // Probability that a source is not observed even if it exists:
                // P0
                float FILTER_P0;

            // +-----------------------------------------------------------+
            // | Resampling parameters                                     |
            // +-----------------------------------------------------------+

                // This threshold must be reached in order to resample the filter
                float FILTER_RSTHRESHOLD;

            // +-----------------------------------------------------------+
            // | Past particles                                            |
            // +-----------------------------------------------------------+

                // Number of past values to put into buffer
                unsigned int FILTER_BUFFERSIZE;

            // +-----------------------------------------------------------+
            // | Others                                                    |
            // +-----------------------------------------------------------+

                // Inverse of the number of particles per filter
                float FILTER_NBPARTICLESINV;

                // Resampling threshold times the number of particles per filter
                float FILTER_RSNMIN;

                // Maximum number of potential sources
                unsigned int BF_MAXSOURCES;

        // +---------------------------------------------------------------+
        // | Variables                                                     |
        // +---------------------------------------------------------------+

            // +-----------------------------------------------------------+
            // | General                                                   |
            // +-----------------------------------------------------------+

                // Defines the position of each particle in (x,y,z) (meters)
                //
                // Size: [FILTER_NBPARTICLES][3]
                float** position;

                // Defines the velocity of each particle in (x,y,z) (meters/sec)
                //
                // Size: [FILTER_NBPARTICLES][3]
                float** velocity;

                // Defines the state of each particle (Stop, Constant velocity,
                // Excitation)
                //
                // Size: [FILTER_NBPARTICLES]
                unsigned char* state;

                // Defines the weight of each particle
                //
                // Size: [FILTER_NBPARTICLES]
                float* weight;

            // +-----------------------------------------------------------+
            // | Prediction parameters                                     |
            // +-----------------------------------------------------------+

                // Particle is stopped
                float aStop;
                float bStop;

                // Particle has a constant velocity
                float aConst;
                float bConst;

                // Particle is accelerated
                float aExc;
                float bExc;

            // +-----------------------------------------------------------+
            // | Probability parameters                                    |
            // +-----------------------------------------------------------+

                // P(Ej|Ov(t-1))
                float P_Ej__Ov_tm1;

                // P(Aj(t)|Ov(t-1))
                float P_Aj_t__Ov_tm1;

                // P(Obsj(t)|Ov(t-1))
                float P_Obsj_t__Ov_tm1;

            // +-----------------------------------------------------------+
            // | Past particules                                           |
            // +-----------------------------------------------------------+

                // Buffer to store xvj,i(t-T)
                //
                // Size: [FILTER_BUFFERSIZE][FILTER_NBPARTICLES][3];
                float ***pastPosition;

                // Pointer to keep track of where to write the new values
                unsigned int pastPositionPointer;

                // Flag to signal that the buffer has been full at least once
                unsigned char bufferWasFull;

        // +---------------------------------------------------------------+
        // | Temporary variables                                           |
        // +---------------------------------------------------------------+

            // p(Oq(t)|xvj,i(t))
            //
            // Size: [BF_MAXSOURCES][FILTER_NBPARTICLES]
            float** p_Oq_t__xv_jy_t;

            // p(xvj,i(t)|O(t)
            //
            // Size: [FILTER_NBPARTICLES]
            float* p_xvji_t__O_t;

            // Cumulative distribution function
            //
            // Size: [FILTER_NBPARTICLES]
            float* CDF;

            // Old positions of the particles
            //
            // Size: [FILTER_NBPARTICLES][3]
            float** oldPosition;

            // Old velocities of the particles
            //
            // Size: [FILTER_NBPARTICLES][3]
            float** oldVelocity;

            // Old states of the particles
            //
            // Size: [FILTER_NBPARTICLES]
            unsigned char* oldState;

    };

/*******************************************************************************
 * Prototypes                                                                  *
 ******************************************************************************/

        void filterInit(struct objFilter *myFilter, struct ParametersStruct *myParameters);

        void filterActivate(struct objFilter *myFilter, float positionX, float positionY, float positionZ, float probExists);

        void filterTerminate(struct objFilter *myFilter);

        void filterUtilNormalize(struct objFilter *myFilter);

        void filterUtilAddBuffer(struct objFilter *myFilter);

        unsigned char filterUtilGetNewState(struct objFilter *myFilter);

        void filterPredict(struct objFilter *myFilter);

        void filterEstimatePosition(struct objFilter *myFilter, float *positionX, float *positionY, float *positionZ);

        void filterProb(struct objFilter *myFilter, float positionX, float positionY, float positionZ, float *individualProb);

        void filterUpdateAPriori(struct objFilter *myFilter, float *Pqj_tm1);

        void filterUpdateWeights(struct objFilter *myFilter, float *Pqj_t, struct objPotentialSources *myPotentialSources);

        void filterResample(struct objFilter *myFilter);

        unsigned char filterResampleIfNeeded(struct objFilter *myFilter);

#endif
