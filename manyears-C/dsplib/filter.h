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
 ******************************************************************************/

#ifndef FILTER_H
#define FILTER_H

// =============================================================================

#include <stdlib.h>

#include "mathadvanced.h"
#include "hardware.h"
#include "dynamicMemory.h"
#include "potentialSources.h"

#include "parameters.h"

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
                int FILTER_NBPARTICLES;

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
                int FILTER_BUFFERSIZE;

            // +-----------------------------------------------------------+
            // | Others                                                    |
            // +-----------------------------------------------------------+

                // Inverse of the number of particles per filter
                float FILTER_NBPARTICLESINV;

                // Resampling threshold times the number of particles per filter
                float FILTER_RSNMIN;

                // Maximum number of potential sources
                int BF_MAXSOURCES;

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
            float* oldState;

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

        void filterEstimatePositio2(struct objFilter *myFilter, float *positionX, float *positionY, float *positionZ);

        void filterProb(struct objFilter *myFilter, float positionX, float positionY, float positionZ, float *individualProb);

        void filterUpdateAPriori(struct objFilter *myFilter, float *Pqj_tm1);

        void filterUpdateWeights(struct objFilter *myFilter, float *Pqj_t, struct objPotentialSources *myPotentialSources);

        void filterResample(struct objFilter *myFilter);

        unsigned char filterResampleIfNeeded(struct objFilter *myFilter);

// =============================================================================

#endif
