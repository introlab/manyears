/******************************************************************************* 
 * ManyEars: Filter - Source Code                                              *
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

#ifdef USE_SIMD
#include <xmmintrin.h>
#endif

#include <stdio.h>

#include "Tracking/filter.h"

// ---
unsigned int resamplingCounter = 0;
// ---

/*******************************************************************************
 * filterInit                                                                  *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:  myFilter        The filter object to be initialized                *
 *          myParameters    List of parameters that is used to configure the   *
 *                          object                                             *
 *                                                                             *
 * Outputs: myFilter        The initialized filter object                      *
 *                                                                             *
 * Description: This function initializes the particle filter object           *
 *                                                                             *
 ******************************************************************************/

void filterInit(struct objFilter *myFilter, struct ParametersStruct *myParameters)
{

    /***************************************************************************
    * Step 1: Load parameters                                                  *
    ***************************************************************************/

    // +-----------------------------------------------------------+
    // | General                                                   |
    // +-----------------------------------------------------------+

        // Number of particles per filter
        myFilter->FILTER_NBPARTICLES = GLOBAL_NBPARTICLES;

        // Standard deviation of the new particles
        myFilter->FILTER_STDDEVIATION = myParameters->P_FILTER_STDDEVIATION;

    // +-----------------------------------------------------------+
    // | Prediction parameters                                     |
    // +-----------------------------------------------------------+

        // Stationary sources
        myFilter->FILTER_ALPHASTOP = myParameters->P_FILTER_ALPHASTOP;
        myFilter->FILTER_BETASTOP = myParameters->P_FILTER_BETASTOP;

        // Constant velocity source
        myFilter->FILTER_ALPHACONST = myParameters->P_FILTER_ALPHACONST;
        myFilter->FILTER_BETACONST = myParameters->P_FILTER_BETACONST;

        // Accelerated source
        myFilter->FILTER_ALPHAEXC = myParameters->P_FILTER_ALPHAEXC;
        myFilter->FILTER_BETAEXC = myParameters->P_FILTER_BETAEXC;

        // Additional inertia factors
        myFilter->FILTER_INERTIAX = myParameters->P_FILTER_INERTIAX;
        myFilter->FILTER_INERTIAY = myParameters->P_FILTER_INERTIAY;
        myFilter->FILTER_INERTIAZ = myParameters->P_FILTER_INERTIAZ;

        // Time interval between updates
        myFilter->FILTER_DELTAT = myParameters->P_FILTER_DELTAT;

        // Percentage of particles to have its state updated
        myFilter->FILTER_STATEUPDT = myParameters->P_FILTER_STATEUPDT;

        // Percentage of new particles to be stopped
        // (make sure total sums up to 1 (100%))
        myFilter->FILTER_NEWSTOP = myParameters->P_FILTER_NEWSTOP;

        // Percentage of new particles to have a constant velocity
        // (make sure total sums up to 1 (100%))
        myFilter->FILTER_NEWCONST = myParameters->P_FILTER_NEWCONST;

        // Percentage of new particles to be excided
        // (make sure total sums up to 1 (100%))
        myFilter->FILTER_NEWEXC = myParameters->P_FILTER_NEWEXC;

    // +-----------------------------------------------------------+
    // | Probability parameters                                    |
    // +-----------------------------------------------------------+

        // Probability that the source was active and is still active:
        // P(Aj(t)|Aj(t-1))
        myFilter->FILTER_AJT_AJTM1 = myParameters->P_FILTER_AJT_AJTM1;

        // Probability that the source was not active and becomes active:
        // P(Aj(t)|¬Aj(t-1))
        myFilter->FILTER_AJT_NOTAJTM1 = myParameters->P_FILTER_AJT_NOTAJTM1;

        // Probability that a source is not observed even if it exists:
        // P0
        myFilter->FILTER_P0 = myParameters->P_FILTER_P0;

    // +-----------------------------------------------------------+
    // | Resampling parameters                                     |
    // +-----------------------------------------------------------+

        // This threshold must be reached in order to resample the filter
        myFilter->FILTER_RSTHRESHOLD = myParameters->P_FILTER_RSTHRESHOLD;

    // +-----------------------------------------------------------+
    // | Past particles                                            |
    // +-----------------------------------------------------------+

        // Number of past values to put into buffer
        myFilter->FILTER_BUFFERSIZE = (unsigned int) myParameters->P_FILTER_BUFFERSIZE;

    // +-----------------------------------------------------------+
    // | Other                                                     |
    // +-----------------------------------------------------------+

        // Maximum number of potential sources
        myFilter->BF_MAXSOURCES = (unsigned int) myParameters->P_BF_MAXSOURCES;

        // Inverse of the number of particles per filter
        myFilter->FILTER_NBPARTICLESINV = 1.0f / myFilter->FILTER_NBPARTICLES;

        // Resampling threshold times the number of particles per filter
        myFilter->FILTER_RSNMIN = myFilter->FILTER_RSTHRESHOLD * myFilter->FILTER_NBPARTICLES;

    /***************************************************************************
    * Step 2: Create arrays                                                    *
    ***************************************************************************/

    // +---------------------------------------------------------------+
    // | Variables                                                     |
    // +---------------------------------------------------------------+

        // +-----------------------------------------------------------+
        // | General                                                   |
        // +-----------------------------------------------------------+

            myFilter->position = (float**) newTable2D(myFilter->FILTER_NBPARTICLES, 3, sizeof(float));
            myFilter->velocity = (float**) newTable2D(myFilter->FILTER_NBPARTICLES, 3, sizeof(float));
            myFilter->state = (unsigned char*) newTable1D(myFilter->FILTER_NBPARTICLES, sizeof(unsigned char));
            myFilter->weight = (float*) newTable1D(myFilter->FILTER_NBPARTICLES, sizeof(float));

        // +-----------------------------------------------------------+
        // | Past particules                                           |
        // +-----------------------------------------------------------+

            myFilter->pastPosition = (float***) newTable3D(myFilter->FILTER_BUFFERSIZE, myFilter->FILTER_NBPARTICLES, 3, sizeof(float));

    // +---------------------------------------------------------------+
    // | Temporary variables                                           |
    // +---------------------------------------------------------------+

        myFilter->p_Oq_t__xv_jy_t = (float**) newTable2D(myFilter->BF_MAXSOURCES, myFilter->FILTER_NBPARTICLES, sizeof(float));
        myFilter->p_xvji_t__O_t = (float*) newTable1D(myFilter->FILTER_NBPARTICLES, sizeof(float));
        myFilter->CDF = (float*) newTable1D(myFilter->FILTER_NBPARTICLES, sizeof(float));
        myFilter->oldPosition = (float**) newTable2D(myFilter->FILTER_NBPARTICLES, 3, sizeof(float));
        myFilter->oldVelocity = (float**) newTable2D(myFilter->FILTER_NBPARTICLES, 3, sizeof(float));
        myFilter->oldState = (unsigned char*) newTable1D(myFilter->FILTER_NBPARTICLES, sizeof(unsigned char));

    /***************************************************************************
    * Step 3: Compute the prediction parameters                                *
    ***************************************************************************/

        // For stopped particles
        myFilter->aStop = (float) (exp(-1.0 * myFilter->FILTER_ALPHASTOP * myFilter->FILTER_DELTAT));
        myFilter->bStop = (float) (myFilter->FILTER_BETASTOP * sqrt((1.0 - (myFilter->aStop * myFilter->aStop))));

        // For constant velocity particles
        myFilter->aConst = (float) (exp(-1.0 * myFilter->FILTER_ALPHACONST * myFilter->FILTER_DELTAT));
        myFilter->bConst = (float) (myFilter->FILTER_BETACONST * sqrt((1.0 - (myFilter->aConst * myFilter->aConst))));

        // For accelerated particles
        myFilter->aExc = (float) (exp(-1.0 * myFilter->FILTER_ALPHAEXC * myFilter->FILTER_DELTAT));
        myFilter->bExc = (float) (myFilter->FILTER_BETAEXC * sqrt((1.0 - (myFilter->aExc * myFilter->aExc))));

}

/*******************************************************************************
 * filterActivate                                                              *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:  myFilter        The filter object to be initialized                *
 *          positionX       The X coordinate of the mean of the particles      *
 *          positionY       The Y coordinate of the mean of the particles      *
 *          positionZ       The Z coordinate of the mean of the particles      *
 *          probExists      The probability that when the filter is            *
 *                          created the source really exists                   *
 *                                                                             *
 * Outputs: myFilter        The activated filter object                        *
 *                                                                             *
 * Description: This function activates the particle filter object. This is    *
 *              done only once the particle filter object has been initialized *
 *                                                                             *
 ******************************************************************************/

void filterActivate(struct objFilter *myFilter, float positionX, float positionY, float positionZ, float probExists)
{

    // Index to loop for each particle
    unsigned int indexParticle;

    // Index to loop for each frame in the buffer
    unsigned int indexBuffer;

    // ---
    //printf("Position - x: %f y: %f z: %f\n", positionX, positionY, positionZ);
    // ---

    /***************************************************************************
    * Step 1: Initialize particles                                             *
    ***************************************************************************/

        for (indexParticle = 0; indexParticle < myFilter->FILTER_NBPARTICLES; indexParticle++)
        {

            // *****************************************************************
            // * STEP A: Generate the position                                 *
            // *****************************************************************

            // The particle position has a mean at the desired position and the
            // standard deviation given
            myFilter->position[indexParticle][0] = positionX + myFilter->FILTER_STDDEVIATION * randn();
            myFilter->position[indexParticle][1] = positionY + myFilter->FILTER_STDDEVIATION * randn();
            myFilter->position[indexParticle][2] = positionZ + myFilter->FILTER_STDDEVIATION * randn();

            // *****************************************************************
            // * STEP B: Generate the state                                    *
            // *****************************************************************

            myFilter->state[indexParticle] = filterUtilGetNewState(myFilter);

            // *****************************************************************
            // * STEP C: Generate the velocity                                 *
            // *****************************************************************

            // Check if the particle is stopped
            if (myFilter->state[indexParticle] == FILTER_PARTICLE_STOP)
            {

                    // If so set velocity to zero
                    myFilter->velocity[indexParticle][0] = 0;
                    myFilter->velocity[indexParticle][1] = 0;
                    myFilter->velocity[indexParticle][2] = 0;

            }

            // Otherwise generate a random velocity
            else
            {

                    myFilter->velocity[indexParticle][0] = myFilter->FILTER_STDDEVIATION * randn() * myFilter->FILTER_INERTIAX;
                    myFilter->velocity[indexParticle][1] = myFilter->FILTER_STDDEVIATION * randn() * myFilter->FILTER_INERTIAY;
                    myFilter->velocity[indexParticle][2] = myFilter->FILTER_STDDEVIATION * randn() * myFilter->FILTER_INERTIAZ;

            }

            // *****************************************************************
            // * STEP D: Generate the weight                                   *
            // *****************************************************************

            myFilter->weight[indexParticle] = myFilter->FILTER_NBPARTICLESINV;

        }

        // Finally, normalize the position and the velocity to make sure points lie
        // on the unit sphere and their velocity is tangent to the sphere
        filterUtilNormalize(myFilter);

    /***************************************************************************
    * Step 2: Initialize probability                                           *
    ***************************************************************************/

        // The probability that the source is active given the fact it has just been
        // created is set to 1
        myFilter->P_Aj_t__Ov_tm1 = 1.0f;

        // The probability that the source exists given the fact it has just been
        // created may vary according to the energy of the beamformer
        // For this reason, a value between 0 and 1 is given to probExists and used
        myFilter->P_Ej__Ov_tm1 = probExists;

        // The probability that the source was observed given the fact it has just
        // been created is set to 0
        myFilter->P_Obsj_t__Ov_tm1 = (myFilter->P_Aj_t__Ov_tm1) * (myFilter->P_Ej__Ov_tm1);

    /***************************************************************************
    * Step 3: Buffer                                                           *
    ***************************************************************************/

        // Set the pointer to the beginning of the buffer
        myFilter->pastPositionPointer = 0;

        // Reset the flag since the buffer has not been full yet
        myFilter->bufferWasFull = FILTER_BUFFERNOTFULL;

        // Set all particles to initial particles
        for (indexBuffer = 0; indexBuffer < myFilter->FILTER_BUFFERSIZE; indexBuffer++)
        {
            for (indexParticle = 0; indexParticle < myFilter->FILTER_NBPARTICLES; indexParticle++)
            {

                myFilter->pastPosition[indexBuffer][indexParticle][0] = myFilter->position[indexParticle][0];
                myFilter->pastPosition[indexBuffer][indexParticle][1] = myFilter->position[indexParticle][1];
                myFilter->pastPosition[indexBuffer][indexParticle][2] = myFilter->position[indexParticle][2];

            }
        }

}

/*******************************************************************************
 * filterTerminate                                                             *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:  myFilter        The filter object to be terminated                 *
 *                                                                             *
 * Outputs: (none)                                                             *
 *                                                                             *
 * Description: This function terminates the particle filter object            *
 *                                                                             *
 ******************************************************************************/

void filterTerminate(struct objFilter *myFilter)
{

    // Free memory

    deleteTable2D((void**) myFilter->position);
    deleteTable2D((void**) myFilter->velocity);
    deleteTable1D((void*) myFilter->state);
    deleteTable1D((void*) myFilter->weight);
    deleteTable3D((void***) myFilter->pastPosition);
    deleteTable2D((void**) myFilter->p_Oq_t__xv_jy_t);
    deleteTable1D((void*) myFilter->p_xvji_t__O_t);
    deleteTable1D((void*) myFilter->CDF);
    deleteTable2D((void**) myFilter->oldPosition);
    deleteTable2D((void**) myFilter->oldVelocity);
    deleteTable1D((void*) myFilter->oldState);

}

/*******************************************************************************
 * filterUtilNormalize                                                         *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myFilter        Filter which positions and velocities need to  *
 *                              be normalized                                  *
 *                                                                             *
 * Outputs:     myFilter        Filter with normalized positions and           *
 *                              velocities                                     *
 *                                                                             *
 * Description: This function normalizes the position of each particle such    *
 *              it lies on a unit sphere. Moreover, the function normalizes    *
 *              the velocity of each particle such that it is tangent to the   *
 *              unit sphere                                                    *
 *                                                                             *
 ******************************************************************************/

void filterUtilNormalize(struct objFilter *myFilter)
{

    // Index to loop for each particle
    unsigned int indexParticle;

    // Magnitude for normalization of position
    float magnitudeInv;

    // Dot product result for normalization of velocity
    float dotProduct;

    // Loop for each particle
    for (indexParticle = 0; indexParticle < myFilter->FILTER_NBPARTICLES; indexParticle++)
    {

        /***********************************************************************
         * Step 1: Normalize the position                                      *
         **********************************************************************/

        // Compute the inverse of the magnitude in order to normalize and bring
        // the point on the unit sphere
        //
        // Check if too small
        magnitudeInv = 1.0f / sqrtf(myFilter->position[indexParticle][0] * myFilter->position[indexParticle][0] + myFilter->position[indexParticle][1] * myFilter->position[indexParticle][1] + myFilter->position[indexParticle][2] * myFilter->position[indexParticle][2]);

        // Normalize
        myFilter->position[indexParticle][0] *= magnitudeInv;
        myFilter->position[indexParticle][1] *= magnitudeInv;
        myFilter->position[indexParticle][2] *= magnitudeInv;

        /***********************************************************************
         * Step 2: Normalize the velocity                                      *
         **********************************************************************/

        // In ordert to do so, we first projectthe velocity vector on the
        // position vector. This is done by a dot product: x dot v. We obtain a
        // scalar value that we then multiply by the unit vector that has the
        // same direction as x. Since x is already normalized, we can do
        // (x dot v) * x. After this is obtained, we subtract this from the velocity
        // vector to obtain a vector that list in the plane which is tangent to the
        // sphere: v - (x dot v) * x.

        // Compute the dot product: x_x * v_x + x_y * v_y + x_z * v_z
        dotProduct = myFilter->position[indexParticle][0] * myFilter->velocity[indexParticle][0] + myFilter->position[indexParticle][1] * myFilter->velocity[indexParticle][1] + myFilter->position[indexParticle][2] * myFilter->velocity[indexParticle][2];

        // Subtract the dot product times the component from the vector v
        myFilter->velocity[indexParticle][0] -= (dotProduct * myFilter->position[indexParticle][0]);
        myFilter->velocity[indexParticle][1] -= (dotProduct * myFilter->position[indexParticle][1]);
        myFilter->velocity[indexParticle][2] -= (dotProduct * myFilter->position[indexParticle][2]);

    }

}

/*******************************************************************************
 * filterUtilAddBuffer                                                         *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myFilter        Filter which current positions of its          *
 *                              particles need to be added to its buffer       *
 *                                                                             *
 * Outputs:     myFilter        Updated filter                                 *
 *                                                                             *
 * Description: This function takes the positions of the current particles and *
 *              add them to the circular buffer                                *
 *                                                                             *
 ******************************************************************************/

void filterUtilAddBuffer(struct objFilter *myFilter)
{

    // Index to loop for each particle
    unsigned int indexParticle;

    // Loop for each particle
    for (indexParticle = 0; indexParticle < myFilter->FILTER_NBPARTICLES; indexParticle++)
    {

        // Add all particles position in the buffer
        myFilter->pastPosition[myFilter->pastPositionPointer][indexParticle][0] = myFilter->position[indexParticle][0];
        myFilter->pastPosition[myFilter->pastPositionPointer][indexParticle][1] = myFilter->position[indexParticle][1];
        myFilter->pastPosition[myFilter->pastPositionPointer][indexParticle][2] = myFilter->position[indexParticle][2];

    }

    // Increment the pointer
    myFilter->pastPositionPointer++;

    // If the pointer has reached the end of the buffer, then wrap around
    if (myFilter->pastPositionPointer == myFilter->FILTER_BUFFERSIZE)
    {

        myFilter->pastPositionPointer = 0;

        // Also set a flag to indicate that the buffer has been full at
        // least once
        myFilter->bufferWasFull = FILTER_BUFFERFULL;

    }

}

/*******************************************************************************
 * filterUtilGetNewState                                                       *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:        myFilter          The filter object to be used.              *
 *                                                                             *
 * Outputs:       state             The state obtained randomly                *
 *                                                                             *
 * Description:   This function generates a random state according to the      *
 *                parameters previously defined                                *
 *                                                                             *
 ******************************************************************************/

unsigned char filterUtilGetNewState(struct objFilter *myFilter)
{

    // Receives a random value
    float rndValue;

    // Generate a random value between 0 and 1
    rndValue = randu();

    // Check if it matches the probability that the
    // particle is stopped
    if (rndValue <= myFilter->FILTER_NEWSTOP)
    {
        return FILTER_PARTICLE_STOP;
    }

    // Check if it matches the probability that the
    // particle has a constant velocity
    else if ( (rndValue > myFilter->FILTER_NEWSTOP) && (rndValue <= (myFilter->FILTER_NEWCONST + myFilter->FILTER_NEWSTOP)) )
    {
        return FILTER_PARTICLE_CST;
    }

    // Check if it matches the probability that the
    // particle is accelerated
    else
    {
        return FILTER_PARTICLE_EXC;
    }

}

/*******************************************************************************
 * filterPredict                                                               *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:  myFilter        The filter object which particles position         *
 *                          needs to be predicted to be initialized            *
 *                                                                             *
 * Outputs: myFilter        The filter object with new particles position      *
 *                                                                             *
 * Description: This function predicts the new position of the particles       *
 *              within the filter                                              *
 *                                                                             *
 ******************************************************************************/

void filterPredict(struct objFilter *myFilter)
{

    // Index to loop for each particle
    unsigned int indexParticle;

    // Receives a random value to check if a state needs to be updated
    float rndValue;

    // Parameters to use
    float newA;
    float newB;

    // Loop for each particle
    for (indexParticle = 0; indexParticle < myFilter->FILTER_NBPARTICLES; indexParticle++)
    {

        /***********************************************************************
         * Step 1: Update the state if needed                                  *
         **********************************************************************/

        // Generate a random value between 0 and 1
        rndValue = randu();

        // Check if this value is below the percentage of states to be changed
        // If so, then update the state
        if (rndValue < myFilter->FILTER_STATEUPDT)
        {

            // Generate the new state
            myFilter->state[indexParticle] = filterUtilGetNewState(myFilter);

        }

        // Select appropriate parameters
        switch(myFilter->state[indexParticle])
        {

        case FILTER_PARTICLE_STOP:

            newA = myFilter->aStop;
            newB = myFilter->bStop;

            break;

        case FILTER_PARTICLE_CST:

            newA = myFilter->aConst;
            newB = myFilter->bConst;

            break;

        case FILTER_PARTICLE_EXC:

            newA = myFilter->aExc;
            newB = myFilter->bExc;

            break;

        }

        /***********************************************************************
         * Step 2: Update the velocity                                         *
         **********************************************************************/

        // Equation: x'_x_t = a * x'_x_t + b * Fx
        myFilter->velocity[indexParticle][0] = newA * myFilter->velocity[indexParticle][0] + newB * randn() * myFilter->FILTER_INERTIAX;

        // Equation: x'_y_t = a * x'_y_t + b * Fx
        myFilter->velocity[indexParticle][1] = newA * myFilter->velocity[indexParticle][1] + newB * randn() * myFilter->FILTER_INERTIAY;

        // Equation: x'_z_t = a * x'_z_t + b * Fx
        myFilter->velocity[indexParticle][2] = newA * myFilter->velocity[indexParticle][2] + newB * randn() * myFilter->FILTER_INERTIAZ;

        /***********************************************************************
         * Step 3: Update the position                                         *
         **********************************************************************/

        // Equation: x_x_t = x_x_(t-1) + deltaT * x'_x_t
        myFilter->position[indexParticle][0] = myFilter->position[indexParticle][0] + myFilter->FILTER_DELTAT * myFilter->velocity[indexParticle][0];

        // Equation: x_y_t = x_y_(t-1) + deltaT * x'_y_t
        myFilter->position[indexParticle][1] = myFilter->position[indexParticle][1] + myFilter->FILTER_DELTAT * myFilter->velocity[indexParticle][1];

        // Equation: x_z_t = x_z_(t-1) + deltaT * x'_z_t
        myFilter->position[indexParticle][2] = myFilter->position[indexParticle][2] + myFilter->FILTER_DELTAT * myFilter->velocity[indexParticle][2];

    }

    // Finally, normalize the position and the velocity to make sure points lie
    // on the unit sphere and their velocity is tangent to the sphere
    filterUtilNormalize(myFilter);

    // Add these particles in the buffer
    filterUtilAddBuffer(myFilter);

}

/*******************************************************************************
 * filterEstimatePosition                                                      *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:  myFilter    The filter object which particles are used to          *
 *                      estimate the position of the point from the            *
 *                      particles                                              *
 *                                                                             *
 * Outputs: positionX   The X coordinate of the estimated point                *
 *          positionY   The Y coordinate of the estimated point                *
 *          positionZ   The Z coordinate of the estimated point                *
 *                                                                             *
 * Description: This function estimates the point position from the particles  *
 *              in the buffer of the filter                                    *
 *                                                                             *
 ******************************************************************************/

void filterEstimatePosition(struct objFilter *myFilter, float *positionX, float *positionY, float *positionZ)
{

    // Index to loop for each particle
    unsigned int indexParticle;

    // Sum of all weights
    float sumWeights;

    // Inverse of the magnitude
    float magnitudeInv;

    // Reset all the sums
    sumWeights = 0;
    *positionX = 0;
    *positionY = 0;
    *positionZ = 0;

    for (indexParticle = 0; indexParticle < myFilter->FILTER_NBPARTICLES; indexParticle++)
    {

        // Add the weight of the current particle to the sum
        sumWeights += myFilter->weight[indexParticle];

        // Add the weighted position
        *positionX += myFilter->weight[indexParticle] * myFilter->pastPosition[myFilter->pastPositionPointer][indexParticle][0];
        *positionY += myFilter->weight[indexParticle] * myFilter->pastPosition[myFilter->pastPositionPointer][indexParticle][1];
        *positionZ += myFilter->weight[indexParticle] * myFilter->pastPosition[myFilter->pastPositionPointer][indexParticle][2];

    }

    if (sumWeights < 1E-10f)
    {
        sumWeights = (float) (1E-10);
    }

    // Normalize with the sum of weights
    *positionX /= sumWeights;
    *positionY /= sumWeights;
    *positionZ /= sumWeights;

    // Compute the inverse of the magnitude in order to normalize and bring
    // the point on the unit sphere
    if ((*positionX * *positionX + *positionY * *positionY + *positionZ * *positionZ) < 1E-6f)
    {
        magnitudeInv = 0;
    }
    else
    {
        magnitudeInv = (float) (1.0 / sqrt(*positionX * *positionX + *positionY * *positionY + *positionZ * *positionZ));
    }

    // Normalize
    *positionX *= magnitudeInv;
    *positionY *= magnitudeInv;
    *positionZ *= magnitudeInv;

}

/*******************************************************************************
 * filterProb                                                                  *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:  myFilter        The filter object used to find the probability     *
 *                          that each particle correctly represents the        *
 *                          given point                                        *
 *          positionX       The X coordinate of the point                      *
 *          positionY       The Y coordinate of the point                      *
 *          positionZ       The Z coordinate of the point                      *
 *                                                                             *
 * Outputs: individualProb  An array that contains a probability for each      *
 *                          particle. The array has the following format:      *
 *                                                                             *
 *          +--------------------+                                             *
 *  i = 0   | p(Oq(t)|xv_j,0(t)) |                                             *
 *          +--------------------+                                             *
 *  i = 1   | p(Oq(t)|xv_j,1(t)) |                                             *
 *          +--------------------+                                             *
 *   ...    |         ...        |                                             *
 *          +--------------------+                                             *
 *  i = N-1 |p(Oq(t)|xv_j,N-1(t))|                                             *
 *          +--------------------+                                             *
 *                                                                             *
 * Description: This function computes the probability for each particle in    *
 *              the filter to represent correctly the given point              *
 *                                                                             *
 ******************************************************************************/

void filterProb(struct objFilter *myFilter, float positionX, float positionY, float positionZ, float *individualProb)
{

    // Index to loop for each particle
    unsigned int indexParticle;

    // Distance between the particle and the point to be evaluated
    float distanceX;
    float distanceY;
    float distanceZ;
    float distance;

    // Exponent to be used in the probability computation
    float d;

    // Probability for the current particle
    float p_Oq_xji;

#ifdef USE_SIMD

    // SIMD registers
    __m128_mod regA, regB, regC, regD, regE, regF, regG, regH;

#endif

#ifndef USE_SIMD

    // Loop for each particle
    for (indexParticle = 0; indexParticle < myFilter->FILTER_NBPARTICLES; indexParticle++)
    {

        // Compute the equation: p(O_q|x_j,i) = 0.8 * exp(4*d) + 0.18 * exp(0.4*d) + 0.02 * exp(0.1*d)
        // where d = -[(x-x')^2 + (y-y')^2 + (z-z')^2]/sigma^2

        // Get the distance: (x-x')^2 + (y-y')^2 + (z-z')^2
        distanceX = positionX - myFilter->position[indexParticle][0];
        distanceY = positionY - myFilter->position[indexParticle][1];
        distanceZ = positionZ - myFilter->position[indexParticle][2];

        distance = distanceX * distanceX + distanceY * distanceY + distanceZ * distanceZ;

        // Compute the exponent: d = -1*distance/sigma^2 = -[(x-x')^2 + (y-y')^2 + (z-z')^2]/sigma^2
        d = -1.0f * distance / (myFilter->FILTER_STDDEVIATION * myFilter->FILTER_STDDEVIATION);

        // Compute the probability: P = 0.8 * exp(4*d) + 0.18 * exp(0.4*d) + 0.02 * exp(0.1*d)
        p_Oq_xji = 0.8f * expest(4.0f*d) + 0.18f * expest(0.4f*d) + 0.02f * expest(0.1f*d);


        // Save in the array of individual results
        individualProb[indexParticle] = p_Oq_xji;

    }

#else

    // Load the constant -1 * 4 / ( 128 * stdDev * stdDev )
    regA.m128_f32[0] = -1.0 * 4.0 / (128.0 * myFilter->FILTER_STDDEVIATION * myFilter->FILTER_STDDEVIATION);
    regA.m128_f32[1] = -1.0 * 4.0 / (128.0 * myFilter->FILTER_STDDEVIATION * myFilter->FILTER_STDDEVIATION);
    regA.m128_f32[2] = -1.0 * 4.0 / (128.0 * myFilter->FILTER_STDDEVIATION * myFilter->FILTER_STDDEVIATION);
    regA.m128_f32[3] = -1.0 * 4.0 / (128.0 * myFilter->FILTER_STDDEVIATION * myFilter->FILTER_STDDEVIATION);

    // Load the constant 0.4 / ( 128 * stdDev * stdDev )
    regB.m128_f32[0] = -1.0 * 0.4 / (128.0 * myFilter->FILTER_STDDEVIATION * myFilter->FILTER_STDDEVIATION);
    regB.m128_f32[1] = -1.0 * 0.4 / (128.0 * myFilter->FILTER_STDDEVIATION * myFilter->FILTER_STDDEVIATION);
    regB.m128_f32[2] = -1.0 * 0.4 / (128.0 * myFilter->FILTER_STDDEVIATION * myFilter->FILTER_STDDEVIATION);
    regB.m128_f32[3] = -1.0 * 0.4 / (128.0 * myFilter->FILTER_STDDEVIATION * myFilter->FILTER_STDDEVIATION);

    // Load the constant 0.1 / ( 128 * stdDev * stdDev )
    regC.m128_f32[0] = -1.0 * 0.1 / (128.0 * myFilter->FILTER_STDDEVIATION * myFilter->FILTER_STDDEVIATION);
    regC.m128_f32[1] = -1.0 * 0.1 / (128.0 * myFilter->FILTER_STDDEVIATION * myFilter->FILTER_STDDEVIATION);
    regC.m128_f32[2] = -1.0 * 0.1 / (128.0 * myFilter->FILTER_STDDEVIATION * myFilter->FILTER_STDDEVIATION);
    regC.m128_f32[3] = -1.0 * 0.1 / (128.0 * myFilter->FILTER_STDDEVIATION * myFilter->FILTER_STDDEVIATION);

    // Loop for each particle
    for (indexParticle = 0; (indexParticle + 3) < myFilter->FILTER_NBPARTICLES; indexParticle+=4)
    {

        // +--------------------------------------------------------------------+
        // | Compute distance                                                   |
        // +--------------------------------------------------------------------+

        // Compute the equation: p(O_q|x_j,i) = 0.8 * exp(4*d) + 0.18 * exp(0.4*d) + 0.02 * exp(0.1*d)
        // where d = -[(x-x')^2 + (y-y')^2 + (z-z')^2]/sigma^2

        // Load x
        regD.m128_f32[0] = myFilter->position[indexParticle][0];
        regD.m128_f32[1] = myFilter->position[indexParticle+1][0];
        regD.m128_f32[2] = myFilter->position[indexParticle+2][0];
        regD.m128_f32[3] = myFilter->position[indexParticle+3][0];

        // Load x'
        regE.m128_f32[0] = positionX;
        regE.m128_f32[1] = positionX;
        regE.m128_f32[2] = positionX;
        regE.m128_f32[3] = positionX;

        // x' - x
        regF.m128 = _mm_sub_ps(regE.m128,regD.m128);

        // (x' - x)^2
        regH.m128 = _mm_mul_ps(regF.m128,regF.m128);

        // Load y
        regD.m128_f32[0] = myFilter->position[indexParticle][1];
        regD.m128_f32[1] = myFilter->position[indexParticle+1][1];
        regD.m128_f32[2] = myFilter->position[indexParticle+2][1];
        regD.m128_f32[3] = myFilter->position[indexParticle+3][1];

        // Load y'
        regE.m128_f32[0] = positionY;
        regE.m128_f32[1] = positionY;
        regE.m128_f32[2] = positionY;
        regE.m128_f32[3] = positionY;

        // y' - y
        regF.m128 = _mm_sub_ps(regE.m128,regD.m128);

        // (y' - y)^2
        regG.m128 = _mm_mul_ps(regF.m128,regF.m128);

        // (x' - x)^2 + (y' - y)^2
        regH.m128 = _mm_add_ps(regH.m128,regG.m128);

        // Load z
        regD.m128_f32[0] = myFilter->position[indexParticle][2];
        regD.m128_f32[1] = myFilter->position[indexParticle+1][2];
        regD.m128_f32[2] = myFilter->position[indexParticle+2][2];
        regD.m128_f32[3] = myFilter->position[indexParticle+3][2];

        // Load z'
        regE.m128_f32[0] = positionZ;
        regE.m128_f32[1] = positionZ;
        regE.m128_f32[2] = positionZ;
        regE.m128_f32[3] = positionZ;

        // z' - z
        regF.m128 = _mm_sub_ps(regE.m128,regD.m128);

        // (z' - z)^2
        regG.m128 = _mm_mul_ps(regF.m128,regF.m128);

        // (x' - x)^2 + (y' - y)^2 + (z' - z)^2
        regH.m128 = _mm_add_ps(regH.m128,regG.m128);

        // Load the constant 1
        regD.m128_f32[0] = 1.0;
        regD.m128_f32[1] = 1.0;
        regD.m128_f32[2] = 1.0;
        regD.m128_f32[3] = 1.0;

        // Load the constant 0
        regC.m128_f32[0] = 0.0;
        regC.m128_f32[1] = 0.0;
        regC.m128_f32[2] = 0.0;
        regC.m128_f32[3] = 0.0;

        // +--------------------------------------------------------------------+
        // | Compute 0.8 * exp(4*d)                                             |
        // +--------------------------------------------------------------------+

        // Compute -1 * distance * ( 4 / (128 * stdDev * stdDev) )
        regE.m128 = _mm_mul_ps(regH.m128,regA.m128);

        // Compute 1 + -1 * distance * ( 4 / (128 * stdDev * stdDev) )
        regE.m128 = _mm_add_ps(regD.m128,regE.m128);

        // If 1 + -1 * distance * ( 4 / (128 * stdDev * stdDev) ) < 0 then set to 0
        regF.m128 = _mm_cmpgt_ps(regE.m128,regC.m128);
        regE.m128 = _mm_and_ps(regE.m128,regF.m128);

        // Compute ( 1 + -1 * distance * ( 4 / (128 * stdDev * stdDev) ) ) ^ 2
        regE.m128 = _mm_mul_ps(regE.m128,regE.m128);

        // Compute ( 1 + -1 * distance * ( 4 / (128 * stdDev * stdDev) ) ) ^ 4
        regE.m128 = _mm_mul_ps(regE.m128,regE.m128);

        // Compute ( 1 + -1 * distance * ( 4 / (128 * stdDev * stdDev) ) ) ^ 8
        regE.m128 = _mm_mul_ps(regE.m128,regE.m128);

        // Compute ( 1 + -1 * distance * ( 4 / (128 * stdDev * stdDev) ) ) ^ 16
        regE.m128 = _mm_mul_ps(regE.m128,regE.m128);

        // Compute ( 1 + -1 * distance * ( 4 / (128 * stdDev * stdDev) ) ) ^ 32
        regE.m128 = _mm_mul_ps(regE.m128,regE.m128);

        // Compute ( 1 + -1 * distance * ( 4 / (128 * stdDev * stdDev) ) ) ^ 64
        regE.m128 = _mm_mul_ps(regE.m128,regE.m128);

        // Compute ( 1 + -1 * distance * ( 4 / (128 * stdDev * stdDev) ) ) ^ 128
        regE.m128 = _mm_mul_ps(regE.m128,regE.m128);

        // Load the constant 0.8
        regF.m128_f32[0] = 0.8;
        regF.m128_f32[1] = 0.8;
        regF.m128_f32[2] = 0.8;
        regF.m128_f32[3] = 0.8;

        // Compute 0.8 * ( 1 + -1 * distance * ( 4 / (128 * stdDev * stdDev) ) ) ^ 128
        regG.m128 = _mm_mul_ps(regF.m128,regE.m128);

        // +--------------------------------------------------------------------+
        // | Compute 0.18 * exp(0.4*d)                                          |
        // +--------------------------------------------------------------------+

        // Compute -1 * distance * ( 0.4 / (128 * stdDev * stdDev) )
        regE.m128 = _mm_mul_ps(regH.m128,regB.m128);

        // Compute 1 + -1 * distance * ( 0.4 / (128 * stdDev * stdDev) )
        regE.m128 = _mm_add_ps(regD.m128,regE.m128);

        // If 1 + -1 * distance * ( 0.4 / (128 * stdDev * stdDev) ) < 0 then set to 0
        regF.m128 = _mm_cmpgt_ps(regE.m128,regC.m128);
        regE.m128 = _mm_and_ps(regE.m128,regF.m128);

        // Compute ( 1 + -1 * distance * ( 0.4 / (128 * stdDev * stdDev) ) ) ^ 2
        regE.m128 = _mm_mul_ps(regE.m128,regE.m128);

        // Compute ( 1 + -1 * distance * ( 0.4 / (128 * stdDev * stdDev) ) ) ^ 4
        regE.m128 = _mm_mul_ps(regE.m128,regE.m128);

        // Compute ( 1 + -1 * distance * ( 0.4 / (128 * stdDev * stdDev) ) ) ^ 8
        regE.m128 = _mm_mul_ps(regE.m128,regE.m128);

        // Compute ( 1 + -1 * distance * ( 0.4 / (128 * stdDev * stdDev) ) ) ^ 16
        regE.m128 = _mm_mul_ps(regE.m128,regE.m128);

        // Compute ( 1 + -1 * distance * ( 0.4 / (128 * stdDev * stdDev) ) ) ^ 32
        regE.m128 = _mm_mul_ps(regE.m128,regE.m128);

        // Compute ( 1 + -1 * distance * ( 0.4 / (128 * stdDev * stdDev) ) ) ^ 64
        regE.m128 = _mm_mul_ps(regE.m128,regE.m128);

        // Compute ( 1 + -1 * distance * ( 0.4 / (128 * stdDev * stdDev) ) ) ^ 128
        regE.m128 = _mm_mul_ps(regE.m128,regE.m128);

        // Load the constant 0.8
        regF.m128_f32[0] = 0.18;
        regF.m128_f32[1] = 0.18;
        regF.m128_f32[2] = 0.18;
        regF.m128_f32[3] = 0.18;

        // Compute 0.18 * ( 1 + -1 * distance * ( 0.4 / (128 * stdDev * stdDev) ) ) ^ 128
        regF.m128 = _mm_mul_ps(regF.m128,regE.m128);

        // Compute 0.8 * exp(4*d) + 0.18 * exp(0.4*d)
        regG.m128 = _mm_add_ps(regG.m128,regF.m128);

        // +--------------------------------------------------------------------+
        // | Compute 0.02 * exp(0.1*d)                                          |
        // +--------------------------------------------------------------------+

        // Compute -1 * distance * ( 0.1 / (128 * stdDev * stdDev) )
        regE.m128 = _mm_mul_ps(regH.m128,regB.m128);

        // Compute 1 + -1 * distance * ( 0.1 / (128 * stdDev * stdDev) )
        regE.m128 = _mm_add_ps(regD.m128,regE.m128);

        // If 1 + -1 * distance * ( 0.1 / (128 * stdDev * stdDev) ) < 0 then set to 0
        regF.m128 = _mm_cmpgt_ps(regE.m128,regC.m128);
        regE.m128 = _mm_and_ps(regE.m128,regF.m128);

        // Compute ( 1 + -1 * distance * ( 0.1 / (128 * stdDev * stdDev) ) ) ^ 2
        regE.m128 = _mm_mul_ps(regE.m128,regE.m128);

        // Compute ( 1 + -1 * distance * ( 0.1 / (128 * stdDev * stdDev) ) ) ^ 4
        regE.m128 = _mm_mul_ps(regE.m128,regE.m128);

        // Compute ( 1 + -1 * distance * ( 0.1 / (128 * stdDev * stdDev) ) ) ^ 8
        regE.m128 = _mm_mul_ps(regE.m128,regE.m128);

        // Compute ( 1 + -1 * distance * ( 0.1 / (128 * stdDev * stdDev) ) ) ^ 16
        regE.m128 = _mm_mul_ps(regE.m128,regE.m128);

        // Compute ( 1 + -1 * distance * ( 0.1 / (128 * stdDev * stdDev) ) ) ^ 32
        regE.m128 = _mm_mul_ps(regE.m128,regE.m128);

        // Compute ( 1 + -1 * distance * ( 0.1 / (128 * stdDev * stdDev) ) ) ^ 64
        regE.m128 = _mm_mul_ps(regE.m128,regE.m128);

        // Compute ( 1 + -1 * distance * ( 0.1 / (128 * stdDev * stdDev) ) ) ^ 128
        regE.m128 = _mm_mul_ps(regE.m128,regE.m128);

        // Load the constant 0.02
        regF.m128_f32[0] = 0.02;
        regF.m128_f32[1] = 0.02;
        regF.m128_f32[2] = 0.02;
        regF.m128_f32[3] = 0.02;

        // Compute 0.02 * ( 1 + -1 * distance * ( 0.1 / (128 * stdDev * stdDev) ) ) ^ 128
        regF.m128 = _mm_mul_ps(regF.m128,regE.m128);

        // Compute 0.8 * exp(4*d) + 0.18 * exp(0.4*d) + 0.02 * exp(0.1*d)
        regG.m128 = _mm_add_ps(regG.m128,regF.m128);

        // Save in the array of individual results
        individualProb[indexParticle] = regG.m128_f32[0];
        individualProb[indexParticle+1] = regG.m128_f32[1];
        individualProb[indexParticle+2] = regG.m128_f32[2];
        individualProb[indexParticle+3] = regG.m128_f32[3];

    }

    if (indexParticle > myFilter->FILTER_NBPARTICLES)
    {
        indexParticle -= 4;
    }

    for (indexParticle=0; indexParticle < myFilter->FILTER_NBPARTICLES; indexParticle++)
    {

        // Compute the equation: p(O_q|x_j,i) = 0.8 * exp(4*d) + 0.18 * exp(0.4*d) + 0.02 * exp(0.1*d)
        // where d = -[(x-x')^2 + (y-y')^2 + (z-z')^2]/sigma^2

        // Get the distance: (x-x')^2 + (y-y')^2 + (z-z')^2
        distanceX = positionX - myFilter->position[indexParticle][0];
        distanceY = positionY - myFilter->position[indexParticle][1];
        distanceZ = positionZ - myFilter->position[indexParticle][2];

        distance = distanceX * distanceX + distanceY * distanceY + distanceZ * distanceZ;

        // Compute the exponent: d = -1*distance/sigma^2 = -[(x-x')^2 + (y-y')^2 + (z-z')^2]/sigma^2
        d = -1.0 * distance / (myFilter->FILTER_STDDEVIATION * myFilter->FILTER_STDDEVIATION);

        // Compute the probability: P = 0.8 * exp(4*d) + 0.18 * exp(0.4*d) + 0.02 * exp(0.1*d)
        p_Oq_xji = 0.8 * expest(4*d) + 0.18 * expest(0.4*d) + 0.02 * expest(0.1*d);


        // Save in the array of individual results
        individualProb[indexParticle] = p_Oq_xji;

    }

#endif

}

/*******************************************************************************
 * filterUpdateAPriori                                                         *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:        myFilter    The filter object used to update the a priori    *
 *                            probabilities of the filter                      *
 *                Pqj_tm1     An array that contains the probability that      *
 *                            the current source j corresponded to the         *
 *                            potential source q. The array has the            *
 *                            following format:                                *
 *                                                                             *
 *                                     +-----------+                           *
 *                            q = 0    | Pj,0(t-1) |                           *
 *                                     +-----------+                           *
 *                            q = 1    | Pj,1(t-1) |                           *
 *                                     +-----------+                           *
 *                              ...    |    ...    |                           *
 *                                     +-----------+                           *
 *                            q = Q-1  |Pj,Q-1(t-1)|                           *
 *                                     +-----------+                           *
 *                                                                             *
 * Outputs:       myFilter    The filter object with its updated               *
 *                            probabilities. The probability                   *
 *                            P(Obsj(t)|Ov(t-1)) is computed.                  *
 *                                                                             *
 * Description: This function computes the a priori probability of the filter  *
 *                                                                             *
 ******************************************************************************/

void filterUpdateAPriori(struct objFilter *myFilter, float *Pqj_tm1)
{

    // Index to browse for each value of q
    unsigned int indexQ;

    // Pj(t-1)
    float P_j_tm1;

    // P(Ej|Ov(t-1))
    float P_Ej__Ov_tm1;

    // P(Ej|Ov(t-2))
    float P_Ej__Ov_tm2;

    // P(Aj(t-1)|Ov(t-2))
    float P_Aj_tm1__Ov_tm2;

    // P(Aj(t-1)|O(t-1))
    float P_Aj_tm1__O_tm1;

    // P(Aj(t-1)|Ov(t-1))
    float P_Aj_tm1__Ov_tm1;

    // P(Aj(t)|Ov(t-1))
    float P_Aj_t__Ov_tm1;

    // P(Obsj(t)|Ov(t-1))
    float P_Obsj_t__Ov_tm1;

    /***************************************************************************
    * Step 1: Load parameters                                                  *
    ***************************************************************************/

    // The old P(Ej|Ov(t-1)) now becomes P(Ej|Ov(t-2))
    P_Ej__Ov_tm2 = myFilter->P_Ej__Ov_tm1;

    // The old P(Aj(t)|Ov(t-1)) now becomes P(Aj(t-1)|Ov(t-2))
    P_Aj_tm1__Ov_tm2 = myFilter->P_Aj_t__Ov_tm1;

    /***************************************************************************
    * Step 2: Compute the probability that the current source is observed      *
    ***************************************************************************/

    // Since we have Pq,j(t-1) for the j that corresponds to this filter, we can
    // obtain Pj(t-1) by computing the sum Pq,j(t-1) for q = 0, 1, ..., Q-1
    // where Q is the number of potential sources from the beamformer

    P_j_tm1 = 0;

    for (indexQ = 0; indexQ < myFilter->BF_MAXSOURCES; indexQ++)
    {

        P_j_tm1 += Pqj_tm1[indexQ];

    }

    /***************************************************************************
    * Step 3: Compute the apriori probabilities                                *
    ***************************************************************************/

        // +-------------------------------------------------------------------+
        // | Step A: Compute the probability that the source exists            |
        // +-------------------------------------------------------------------+

        // Apply the following equation: P(Ej|Ov(t-1))
        // = Pj(t-1) + (1 - Pj(t-1)) * (P0*P(Ej|Ov(t-2)))/(1 - (1-P0)*P(Ej|Ov(t-2)))
        P_Ej__Ov_tm1 = P_j_tm1 + (1 - P_j_tm1) * (myFilter->FILTER_P0 * P_Ej__Ov_tm2) / (1 - (1 - myFilter->FILTER_P0) * P_Ej__Ov_tm2);

        // +-------------------------------------------------------------------+
        // | Step B: Compute the prob that the source was active from cur prob |
        // +-------------------------------------------------------------------+

        // Apply the following equation: P(Aj(t-1)|O(t-1))
        // = 0.15 + 0.85 * Pj(t-1)
        P_Aj_tm1__O_tm1 = 0.15f + 0.85f * P_j_tm1;

        // +-------------------------------------------------------------------+
        // | Step C: Compute the probability that the source was active        |
        // +-------------------------------------------------------------------+

        // Apply the following equation: P(Aj(t-1)|Ov(t-1))
        // = 1 / [ 1 + [1 - P(Aj(t-1)|Ov(t-2))][1 - P(Aj(t-1)|O(t-1))] ] / [ P(Aj(t-1)|Ov(t-2)) * P(Aj(t-1)|O(t-1)) ]
        if ((P_Aj_tm1__Ov_tm2 * P_Aj_tm1__O_tm1) < 1E-6)
        {
            P_Aj_tm1__Ov_tm1 = 0.0f;
        }
        else
        {
            if (( 1.0f + ((1.0f - P_Aj_tm1__Ov_tm2) * (1.0f - P_Aj_tm1__O_tm1))/(P_Aj_tm1__Ov_tm2 * P_Aj_tm1__O_tm1)) < 1E-6f)
            {
                P_Aj_tm1__Ov_tm1 = 1.0f;
            }
            else
            {
                P_Aj_tm1__Ov_tm1 = 1.0f / ( 1.0f + ((1.0f - P_Aj_tm1__Ov_tm2) * (1.0f - P_Aj_tm1__O_tm1))/(P_Aj_tm1__Ov_tm2 * P_Aj_tm1__O_tm1));
            }
        }

        // +-------------------------------------------------------------------+
        // | Step D: Compute the probability that the source becomes active    |
        // +-------------------------------------------------------------------+

        // Apply the following equation: P(Aj(t)|Ov(t-1))
        // = P(Aj(t)|Aj(t-1)) * P(Aj(t-1)|Ov(t-1)) + P(Aj(t)|¬Aj(t-1)) * [ 1 - P(Aj(t-1)|Ov(t-1)) ]
        P_Aj_t__Ov_tm1 = myFilter->FILTER_AJT_AJTM1 * P_Aj_tm1__Ov_tm1 + myFilter->FILTER_AJT_NOTAJTM1 * (1.0f - P_Aj_tm1__Ov_tm1);

        // +-------------------------------------------------------------------+
        // | Step E: Compute the probability that the source is observable     |
        // +-------------------------------------------------------------------+

        // Apply the following equation:
        // P(Obsj(t)|Ov(t-1)) = P(Ej|Ov(t-1)) * P(Aj(t)|Ov(t-1))
        P_Obsj_t__Ov_tm1 = P_Ej__Ov_tm1 * P_Aj_t__Ov_tm1;

    /***************************************************************************
    * Step 4: Save parameters                                                  *
    ***************************************************************************/

    // Save P(Ej|Ov(t-1))
    myFilter->P_Ej__Ov_tm1 = P_Ej__Ov_tm1;

    // Save P(Aj(t)|Ov(t-1))
    myFilter->P_Aj_t__Ov_tm1 = P_Aj_t__Ov_tm1;

    // Save P(Obsj(t)|Ov(t-1))
    myFilter->P_Obsj_t__Ov_tm1 = P_Obsj_t__Ov_tm1;

}

/*******************************************************************************
 * filterUpdateWeights                                                         *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:    myFilter        The filter object which weights will be          *
 *                            updated                                          *
 *            Pqj_t           An array that contains the probability that      *
 *                            the current source j corresponds to the          *
 *                            potential source q. The array has the            *
 *                            following format:                                *
 *                                                                             *
 *                                     +-----------+                           *
 *                            q = 0    |  Pj,0(t)  |                           *
 *                                     +-----------+                           *
 *                            q = 1    |  Pj,1(t)  |                           *
 *                                     +-----------+                           *
 *                              ...    |    ...    |                           *
 *                                     +-----------+                           *
 *                          q = Q-1    | Pj,Q-1(t) |                           *
 *                                     +-----------+                           *
 *                                                                             *
 *            myPotentialSources                                               *
 *                                                                             *
 *                            A structure that contains the potential sources  *
 *                                                                             *
 * Outputs:   myFilter        The filter object is updated with new weights    *
 *                                                                             *
 * Description: This function updates the weight of each particle in the       *
 *              filter                                                         *
 *                                                                             *
 ******************************************************************************/

void filterUpdateWeights(struct objFilter *myFilter, float *Pqj_t, struct objPotentialSources *myPotentialSources)
{

    // Index to loop for each value of q
    unsigned int indexQ;

    // Index to loop for each particle
    unsigned int indexParticle;

    // Pj(t)
    float Pj_t;

    // sum_q=0_Q-1(Pq,j(t) * p(Oq(t)|xvj,i(t)))
    float sum_wrt_q_Pqj_p_Oq_t__xv_jy_t;

    // sum_q=0_Q-1(sum_i=0_N-1(Pq,j(t) * p(Oq(t)|xvj,i(t))))
    float sum_wrt_iq_Pqj_p_Oq_t__xv_jy_t;

    // sum_i=0_N-1(p(xvj,i(t)|O(t) * wj,i(t-1))
    float sum_wrt_i_p_xvji_t__O_t;

    /***************************************************************************
    * Step 1: Compute the probability that the current source is observed      *
    ***************************************************************************/

    // Since we have Pq,j(t) for the j that corresponds to this filter, we can
    // obtain Pj(t) by computing the sum Pq,j(t) for q = 0, 1, ..., Q-1
    // where Q is the number of potential sources from the beamformer

    Pj_t = 0;

    for (indexQ = 0; indexQ < myFilter->BF_MAXSOURCES; indexQ++)
    {

        Pj_t += Pqj_t[indexQ];

    }

    /***************************************************************************
    * Step 2: Compute the probability that the particles match the observation *
    ***************************************************************************/

        // +-------------------------------------------------------------------+
        // | Step A: Compute p(Oq|xvj,i) for all q and i                       |
        // +-------------------------------------------------------------------+

        // Loop for each potential source: q = 0, 1, ... , Q-1

        for (indexQ = 0; indexQ < myFilter->BF_MAXSOURCES; indexQ++)
        {

            // Compute the probability
            // The probability p(Oq(t)|xvj,i(t)) is computed for i = 0, 1, ... N-1
            // and is stored in the array p_Oq_t__xv_jy_t
            filterProb(myFilter, myPotentialSources->sourcesPosition[indexQ][0], myPotentialSources->sourcesPosition[indexQ][1], myPotentialSources->sourcesPosition[indexQ][2], &myFilter->p_Oq_t__xv_jy_t[indexQ][0]);

        }

        // +-------------------------------------------------------------------+
        // | Step B: Sum_i=0_N-1(sum_q=0_Q-1(Pq,j(t) * p(Oq(t)|xvj,i(t))))     |
        // +-------------------------------------------------------------------+

        sum_wrt_iq_Pqj_p_Oq_t__xv_jy_t = 0;

        for (indexParticle = 0; indexParticle < myFilter->FILTER_NBPARTICLES; indexParticle++)
        {

            for (indexQ = 0; indexQ < myFilter->BF_MAXSOURCES; indexQ++)
            {

                sum_wrt_iq_Pqj_p_Oq_t__xv_jy_t += Pqj_t[indexQ] * myFilter->p_Oq_t__xv_jy_t[indexQ][indexParticle];

            }

        }

        // +-------------------------------------------------------------------+
        // | Step C: Compute p(xvj,i(t)|O(t))                                  |
        // +-------------------------------------------------------------------+

        // Compute p(xvj,i(t)|O(t)) for i = 0, 1, ..., N-1
        for (indexParticle = 0; indexParticle < myFilter->FILTER_NBPARTICLES; indexParticle++)
        {

            // Compute sum_q=1_Q(Pq,j(t) * p(Oq(t)|xvj,i(t)))
            sum_wrt_q_Pqj_p_Oq_t__xv_jy_t = 0;

            for (indexQ = 0; indexQ < myFilter->BF_MAXSOURCES; indexQ++)
            {

                sum_wrt_q_Pqj_p_Oq_t__xv_jy_t += Pqj_t[indexQ] * myFilter->p_Oq_t__xv_jy_t[indexQ][indexParticle];

            }

            // If Pq,j(t) is 0 for all values of q, then so is Pj
            // (since it is the sum of all Pq,j(t) for q = 0, ... , Q-1
            // In this case, we get sum_q=1_Q( Pq,j(t) * p(Oq(t)|xvj,i(t)) = 0,
            // sum_i=1_N(sum_q=1_Q( Pq,j(t) * p(Oq(t)|xvj,i(t)) )) = 0.
            // So we would have 0 * (0/0), which gives a NaN according to
            // the software. However, we define it here as 0 since there is a
            // null probability.
            if (Pj_t == 0)
            {
                myFilter->p_xvji_t__O_t[indexParticle] = myFilter->FILTER_NBPARTICLESINV;
            }

            // Otherwise, apply the equation as normal
            else
            {
                // Apply the equation: p(xvj,i(t)|O(t))
                // = (1 - Pj(t))(1/Np) + Pj * [sum_q=1_Q( Pq,j(t) * p(Oq(t)|xvj,i(t))] / [sum_i=1_N(sum_q=1_Q( Pq,j(t) * p(Oq(t)|xvj,i(t)) ))]
                //
                // If the sum is too small, make it equal to 0
                if (sum_wrt_iq_Pqj_p_Oq_t__xv_jy_t > 1E-6)
                {
                    myFilter->p_xvji_t__O_t[indexParticle] = (1 - Pj_t) * myFilter->FILTER_NBPARTICLESINV + Pj_t * (sum_wrt_q_Pqj_p_Oq_t__xv_jy_t/sum_wrt_iq_Pqj_p_Oq_t__xv_jy_t);
                }
                else
                {
                    myFilter->p_xvji_t__O_t[indexParticle] = (1 - Pj_t) * myFilter->FILTER_NBPARTICLESINV;
                }
            }

        }

    /***************************************************************************
    * Step 3: Update the weights                                               *
    ***************************************************************************/

        // +-------------------------------------------------------------------+
        // | Step A: Compute the sum of all p(xvj,i(t)|O(t)) times their weight|
        // +-------------------------------------------------------------------+

        sum_wrt_i_p_xvji_t__O_t = 0.0f;

        for (indexParticle = 0; indexParticle < myFilter->FILTER_NBPARTICLES; indexParticle++)
        {

            // Compute sum_i=0_N-1(p(xvj,i(t)|O(t))*wj,i(t-1))
            sum_wrt_i_p_xvji_t__O_t += myFilter->p_xvji_t__O_t[indexParticle] * myFilter->weight[indexParticle];

        }


        // +-------------------------------------------------------------------+
        // | Step B: Update the weight values                                  |
        // +-------------------------------------------------------------------+

        for (indexParticle = 0; indexParticle < myFilter->FILTER_NBPARTICLES; indexParticle++)
        {

            // Compute the equation: wj,t(t)
            // = p(xvj,i(t)|O(t))*wj,i(t-1) / sum_i=0_N-1(p(xvj,i(t)|O(t))*wj,i(t-1))
            //
            // If the sum is too small, then set to zero
            if (sum_wrt_i_p_xvji_t__O_t > 1E-6f)
            {
                myFilter->weight[indexParticle] = (myFilter->p_xvji_t__O_t[indexParticle] * myFilter->weight[indexParticle]) / sum_wrt_i_p_xvji_t__O_t;
            }
            else
            {
                myFilter->weight[indexParticle] = 0.0f;
            }

        }

}

/*******************************************************************************
 * filterResample                                                              *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:    myFilter        The filter object which particles need to be     *
 *                            resampled                                        *
 *                                                                             *
 * Outputs:   myFilter        The filter object with its particles resampled   *
 *                                                                             *
 * Description: This function resamples the particles by selecting mostly      *
 *              particles with the greatest weight                             *
 *                                                                             *
 ******************************************************************************/

void filterResample(struct objFilter *myFilter)
{

    // Cumulative sum
    float CDFSum;

    // Random value
    float rndValue;

    // Particle index
    unsigned int indexParticle;

    // Pointer to find the index
    unsigned int pointer;

    // Low bound for the pointer
    unsigned int lowBound;

    // High bound for the pointer
    unsigned int highBound;

    /***************************************************************************
    * Step 1: Copy the particles in a temporary variable (will be used later)  *
    ***************************************************************************/

        // +-------------------------------------------------------------------+
        // | Step A: Copy the positions                                        |
        // +-------------------------------------------------------------------+

        for (indexParticle = 0; indexParticle < myFilter->FILTER_NBPARTICLES; indexParticle++)
        {

            myFilter->oldPosition[indexParticle][0] = myFilter->position[indexParticle][0];
            myFilter->oldPosition[indexParticle][1] = myFilter->position[indexParticle][1];
            myFilter->oldPosition[indexParticle][2] = myFilter->position[indexParticle][2];

        }

        // +-------------------------------------------------------------------+
        // | Step B: Copy the velocities                                       |
        // +-------------------------------------------------------------------+

        for (indexParticle = 0; indexParticle < myFilter->FILTER_NBPARTICLES; indexParticle++)
        {

            myFilter->oldVelocity[indexParticle][0] = myFilter->velocity[indexParticle][0];
            myFilter->oldVelocity[indexParticle][1] = myFilter->velocity[indexParticle][1];
            myFilter->oldVelocity[indexParticle][2] = myFilter->velocity[indexParticle][2];

        }

        // +-------------------------------------------------------------------+
        // | Step C: Copy the states                                           |
        // +-------------------------------------------------------------------+

        for (indexParticle = 0; indexParticle < myFilter->FILTER_NBPARTICLES; indexParticle++)
        {

            myFilter->oldState[indexParticle] = myFilter->state[indexParticle];

        }

    /***************************************************************************
    * Step 2: Compute the cumulative distribution function                     *
    ***************************************************************************/

    CDFSum = 0;

    for (indexParticle = 0; indexParticle < myFilter->FILTER_NBPARTICLES; indexParticle++)
    {

        // Add the current probability to the sum of probabilities
        CDFSum += myFilter->weight[indexParticle];

        // Save as the CDF for this index
        myFilter->CDF[indexParticle] = CDFSum;

    }

    /***************************************************************************
    * Step 3: Resample the particles                                           *
    ***************************************************************************/

    for (indexParticle = 0; indexParticle < myFilter->FILTER_NBPARTICLES; indexParticle++)
    {

        // +-------------------------------------------------------------------+
        // | Step A: Generate a random number                                  |
        // +-------------------------------------------------------------------+

        rndValue = randu();

        // +-------------------------------------------------------------------+
        // | Step B: Find the corresponding index                              |
        // +-------------------------------------------------------------------+

        // Define initial low bound
        lowBound = 0;

        // Define initial high bound
        highBound = myFilter->FILTER_NBPARTICLES - 1;

        // Set the pointer in the middle of the range
        pointer = (unsigned int) (floorf((lowBound + highBound) / 2.0f));

        // Loop until an index is found
        while(1)
        {

            // If the current value is bigger or equal
            if (rndValue <= myFilter->CDF[pointer])
            {

                // Check if we reached the bottom of the array
                if (pointer == lowBound)
                {
                    // We found the good index, so stop the iteration
                    break;
                }

                // Check if the value just before is smaller than or equal to the
                // current value
                if (rndValue > myFilter->CDF[pointer - 1])
                {

                    // We found the good index, so stop the iteration
                    break;

                }

                // Define the new high bound
                highBound = pointer - 1;

                // Set the pointer in the middle of the range
                pointer = (unsigned int) (floorf((lowBound + highBound) / 2.0f));

            }

            // If the current value is greater or equal
            else
            {

                // Check if we reached the top of the array
                if (pointer == highBound)
                {
                    // We found the good index, so stop the iteration
                    break;
                }

                // Define the new low bound
                lowBound = pointer;

                // Set the pointer in the middle of the range
                pointer = (unsigned int) (floorf((lowBound + highBound + 1) / 2.0f));


            }

        }

        // +-------------------------------------------------------------------+
        // | Step C: Copy the old position to the new position                 |
        // +-------------------------------------------------------------------+

        myFilter->position[indexParticle][0] = myFilter->oldPosition[pointer][0];
        myFilter->position[indexParticle][1] = myFilter->oldPosition[pointer][1];
        myFilter->position[indexParticle][2] = myFilter->oldPosition[pointer][2];

        // +-------------------------------------------------------------------+
        // | Step D: Copy the old velocity to the new velocity                 |
        // +-------------------------------------------------------------------+

        myFilter->velocity[indexParticle][0] = myFilter->oldVelocity[pointer][0];
        myFilter->velocity[indexParticle][1] = myFilter->oldVelocity[pointer][1];
        myFilter->velocity[indexParticle][2] = myFilter->oldVelocity[pointer][2];

        // +-------------------------------------------------------------------+
        // | Step E: Copy the old state to the new state                       |
        // +-------------------------------------------------------------------+

        myFilter->state[indexParticle] = myFilter->oldState[pointer];

        // +-------------------------------------------------------------------+
        // | Step F: Reset the weight                                          |
        // +-------------------------------------------------------------------+

        myFilter->weight[indexParticle] = myFilter->FILTER_NBPARTICLESINV;

    }

}

/*******************************************************************************
 * filterResampleIfNeeded                                                      *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myFilter        The filter object which particles may need to  *
 *                              be resampled                                   *
 *                                                                             *
 * Outputs:     myFilter        The filter object with its particles resampled *
 *                              or not                                         *
 *              state           Returns 1 if the filter has been resampled, or *
 *                              0 otherwise                                    *
 *                                                                             *
 * Description: This function resamples the particles by selecting mostly      *
 *              particles with the greatest weight if the resampling condition *
 *              is satisfied                                                   *
 *                                                                             *
 ******************************************************************************/

unsigned char filterResampleIfNeeded(struct objFilter *myFilter)
{

    // Neff
    float Neff;

    // The sum of the weights squared
    float sumWeightsSquared;

    // Index to loop through particles
    unsigned int indexParticle;

    /***************************************************************************
    * Step 1: Compute the inverse of the sum of weights squared (Neff)         *
    ***************************************************************************/

    sumWeightsSquared = 0;

    for (indexParticle = 0; indexParticle < myFilter->FILTER_NBPARTICLES; indexParticle++)
    {

        sumWeightsSquared += (myFilter->weight[indexParticle]) * (myFilter->weight[indexParticle]);

    }

    if (sumWeightsSquared < 1E-10f)
    {
        Neff = (float) (1.0f / (1E-10f));
    }
    else
    {
        Neff = (float) (1.0f / sumWeightsSquared);
    }

    /***************************************************************************
    * Step 2: Check if there is a need to resample; if so, do it               *
    ***************************************************************************/

//    printf("x: %f y:%f z:%f\n", myFilter->position[0][0],myFilter->position[0][1],myFilter->position[0][2]);
//    printf("%f-",myFilter->weight[0]);
//    printf("%f-",myFilter->weight[1]);
//    printf("%f-",myFilter->weight[2]);
//    printf("%f-",myFilter->weight[3]);
//    printf("%f-",myFilter->weight[4]);
//    printf("%f-",myFilter->weight[5]);
//    printf("%f-",myFilter->weight[6]);
//    printf("%f-",myFilter->weight[7]);
//    printf("%f-",myFilter->weight[8]);
//    printf("%f-",myFilter->weight[9]);
//
//    printf("\n\n");

    if (Neff < myFilter->FILTER_RSNMIN)
    {

        // ---
//        resamplingCounter++;
//        printf("%i\n",resamplingCounter);
        // ---

        filterResample(myFilter);

        return 1;

    }

    return 0;

}
