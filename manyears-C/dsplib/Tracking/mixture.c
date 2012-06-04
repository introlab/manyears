/******************************************************************************* 
 * ManyEars: Mixture - Source Code                                             *
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

#include "Tracking/mixture.h"

/*******************************************************************************
 * Compatibility issues                                                        *
 ******************************************************************************/

// In some environment, the constant PI is not define with the math.h library
#ifndef M_PI
    #define M_PI    3.1415926535897932384626433832795f
#endif

/*******************************************************************************
 * mixtureInit                                                                 *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myMixture       The mixture object to be initalized            *
 *              myParameters    List of parameters that is used to configure   *
 *                              the object                                     *
 *                                                                             *
 * Outputs:     myMixture       The initialized mixture object                 *
 *                                                                             *
 * Description: This function initializes the mixture object                   *
 *                                                                             *
 ******************************************************************************/

void mixtureInit(struct objMixture *myMixture, struct ParametersStruct *myParameters)
{

    // Index to loop through each filter spot
    unsigned char indexFilter;

    /***************************************************************************
    * Step 1: Load parameters                                                  *
    ***************************************************************************/

    // +-----------------------------------------------------------------------+
    // | General                                                               |
    // +-----------------------------------------------------------------------+

    // Maximum number of filter
    myMixture->MIXTURE_NBFILTERS = myParameters->P_GEN_DYNSOURCES;

    // +-----------------------------------------------------------------------+
    // | Probability                                                           |
    // +-----------------------------------------------------------------------+

    // Probability that a new source appears: Pnew
    myMixture->MIXTURE_PNEW = myParameters->P_MIXTURE_PNEW;

    // Probability that a false detection occurs
    myMixture->MIXTURE_PFALSE = myParameters->P_MIXTURE_PFALSE;

    // +-----------------------------------------------------------------------+
    // | Source tracking                                                       |
    // +-----------------------------------------------------------------------+

    // Threshold to reach in order to track a new source
    myMixture->MIXTURE_NEWTHRESHOLD = myParameters->P_MIXTURE_NEWTHRESHOLD;

    // +-----------------------------------------------------------------------+
    // | Source addition/removing                                              |
    // +-----------------------------------------------------------------------+

    // Threshold to reach in order to confirm a source really exists
    myMixture->MIXTURE_CONFIRMEXISTS = myParameters->P_MIXTURE_CONFIRMEXISTS;

    // Threshold to count a source as existing
    myMixture->MIXTURE_CONFIRMCOUNTTS = myParameters->P_MIXTURE_CONFIRMCOUNTTS;

    // Number of times the threshold must be reached
    myMixture->MIXTURE_CONFIRMCOUNT = myParameters->P_MIXTURE_CONFIRMCOUNT;

    // Maximum number of frames while the source has not been tracked
    // in order to delete this tracking for probation time
    myMixture->MIXTURE_CUMULATIVETIMEPROB = myParameters->P_MIXTURE_CUMULATIVETIMEPROB;

    // Maximum number of frames while the source has not been tracked
    // in order to delete this tracking for time 1
    myMixture->MIXTURE_CUMULATIVETIME1 = myParameters->P_MIXTURE_CUMULATIVETIME1;

    // Maximum number of frames while the source has not been tracked
    // in order to delete this tracking for time 2
    myMixture->MIXTURE_CUMULATIVETIME2 = myParameters->P_MIXTURE_CUMULATIVETIME2;

    // Minimum value to consider to say that source is not observed for
    // probation time
    myMixture->MIXTURE_TOBSPROB = myParameters->P_MIXTURE_TOBSPROB;

    // Minimum value to consider to say that source is not observed for
    // time 1
    myMixture->MIXTURE_TOBS1 = myParameters->P_MIXTURE_TOBS1;

    // Minimum value to consider to say that source is not observed for
    // time 2
    myMixture->MIXTURE_TOBS2 = myParameters->P_MIXTURE_TOBS2;

    // Minimum horizontal angle difference that the new source needs to
    // have with the already existing filters (rad)
    myMixture->MIXTURE_NEWANGLE = myParameters->P_MIXTURE_NEWANGLE;

    // +-----------------------------------------------------------------------+
    // | Others                                                                |
    // +-----------------------------------------------------------------------+

    // Maximum number of potential sources
    myMixture->BF_MAXSOURCES = myParameters->P_BF_MAXSOURCES;

    // Number of particles in filters
    myMixture->FILTER_NBPARTICLES = GLOBAL_NBPARTICLES;

    // Standard deviation
    myMixture->FILTER_STDDEVIATION = myParameters->P_FILTER_STDDEVIATION;

    // Mixture base for f
    myMixture->MIXTURE_BASEF = myMixture->MIXTURE_NBFILTERS + 2;

    /***************************************************************************
    * Step 2: Create arrays                                                    *
    ***************************************************************************/

    // +-----------------------------------------------------------------------+
    // | Variables                                                             |
    // +-----------------------------------------------------------------------+

    myMixture->listFilters = (struct objFilter*) newTable1D(myMixture->MIXTURE_NBFILTERS, sizeof(struct objFilter));
    myMixture->filterID = (ID_TYPE*) newTable1D(myMixture->MIXTURE_NBFILTERS, sizeof(ID_TYPE));
    myMixture->confirmSourceExists = (float*) newTable1D(myMixture->MIXTURE_NBFILTERS, sizeof(float));
    myMixture->sumProbSourceExists = (float*) newTable1D(myMixture->MIXTURE_NBFILTERS, sizeof(float));
    myMixture->flagSourceExists = (unsigned char*) newTable1D(myMixture->MIXTURE_NBFILTERS, sizeof(unsigned char));
    myMixture->countSourceExists = (unsigned int*) newTable1D(myMixture->MIXTURE_NBFILTERS, sizeof(unsigned int));
    myMixture->cumulativeTimeProb = (unsigned int*) newTable1D(myMixture->MIXTURE_NBFILTERS, sizeof(unsigned int));
    myMixture->cumulativeTime1 = (unsigned int*) newTable1D(myMixture->MIXTURE_NBFILTERS, sizeof(unsigned int));
    myMixture->cumulativeTime2 = (unsigned int*) newTable1D(myMixture->MIXTURE_NBFILTERS, sizeof(unsigned int));

    // +-------------------------------------------------------------------+
    // | Temporary variables                                               |
    // +-------------------------------------------------------------------+

    myMixture->Pq = (float*) newTable1D(myMixture->BF_MAXSOURCES, sizeof(float));
    myMixture->Pfq = (float**) newTable2D((myMixture->MIXTURE_NBFILTERS + 2), myMixture->BF_MAXSOURCES, sizeof(float));
    myMixture->POq_fq = (float**) newTable2D((myMixture->MIXTURE_NBFILTERS + 2), myMixture->BF_MAXSOURCES, sizeof(float));
    myMixture->pOq_xvji = (float*) newTable1D(myMixture->FILTER_NBPARTICLES, sizeof(float));
    myMixture->Pqj_t = (float**) newTable2D(myMixture->FILTER_NBPARTICLES, myMixture->BF_MAXSOURCES, sizeof(float));
    myMixture->Pq_t_H0 = (float*) newTable1D(myMixture->BF_MAXSOURCES, sizeof(float));
    myMixture->Pq_t_H2 = (float*) newTable1D(myMixture->BF_MAXSOURCES, sizeof(float));
    myMixture->fq = (unsigned char*) newTable1D(myMixture->BF_MAXSOURCES, sizeof(unsigned char));
    myMixture->filterConflict = (unsigned char*) newTable1D(myMixture->MIXTURE_NBFILTERS, sizeof(unsigned char));

    /***************************************************************************
    * Step 3: Initialize values                                                *
    ***************************************************************************/

    // Initialize each filter
    for (indexFilter = 0; indexFilter < myMixture->MIXTURE_NBFILTERS; indexFilter++)
    {

        // Initialize the filter
        filterInit(&myMixture->listFilters[indexFilter],myParameters);

        // Set the filter ID to no filter
        myMixture->filterID[indexFilter] = MIXTURE_NOFILTER;

        // Initialize the probability that the filter really exists
        myMixture->confirmSourceExists[indexFilter] = 0;

        // Initialize the flag that the source exists
        myMixture->flagSourceExists[indexFilter] = MIXTURE_SOURCEDOESNOTEXIST;

        // Initialize the counts for a filter to exist
        myMixture->countSourceExists[indexFilter] = 0;
        myMixture->sumProbSourceExists[indexFilter] = 0;

        // Initialize the filter cumulative times to 0
        myMixture->cumulativeTimeProb[indexFilter] = 0;
        myMixture->cumulativeTime1[indexFilter] = 0;
        myMixture->cumulativeTime2[indexFilter] = 0;


    }

    // Initially there is no source being tracked
    myMixture->numberTrackingInProgress = 0;

    /***************************************************************************
    * Step 4: Initialize random module                                         *
    ***************************************************************************/

    randInit();

    /***************************************************************************
    * Step 5: Initialize ID manager module                                     *
    ***************************************************************************/

    idManagerInit(&myMixture->idManager);

}

/*******************************************************************************
 * mixtureTerminate                                                            *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myMixture       The mixture object to be terminated            *
 *                                                                             *
 * Outputs:     (none)                                                         *
 *                                                                             *
 * Description: This function terminates the mixture object                    *
 *                                                                             *
 ******************************************************************************/

void mixtureTerminate(struct objMixture *myMixture)
{

    unsigned int indexFilter;

    // Free memory

    for (indexFilter = 0; indexFilter < myMixture->MIXTURE_NBFILTERS; indexFilter++)
    {
        filterTerminate(&myMixture->listFilters[indexFilter]);
    }

    deleteTable1D((void*) myMixture->listFilters);
    deleteTable1D((void*) myMixture->filterID);
    deleteTable1D((void*) myMixture->confirmSourceExists);
    deleteTable1D((void*) myMixture->sumProbSourceExists);
    deleteTable1D((void*) myMixture->flagSourceExists);
    deleteTable1D((void*) myMixture->countSourceExists);
    deleteTable1D((void*) myMixture->cumulativeTimeProb);
    deleteTable1D((void*) myMixture->cumulativeTime1);
    deleteTable1D((void*) myMixture->cumulativeTime2);

    deleteTable1D((void*) myMixture->Pq);
    deleteTable2D((void**)myMixture->Pfq);
    deleteTable2D((void**)myMixture->POq_fq);
    deleteTable1D((void*) myMixture->pOq_xvji);
    deleteTable2D((void**) myMixture->Pqj_t);
    deleteTable1D((void*) myMixture->Pq_t_H0);
    deleteTable1D((void*) myMixture->Pq_t_H2);
    deleteTable1D((void*) myMixture->fq);
    deleteTable1D((void*) myMixture->filterConflict);

    // Terminate id module
    idManagerTerminate(&myMixture->idManager);

}

/*******************************************************************************
 * mixtureUpdate                                                               *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:    myMixture             The mixture object to be used              *
 *            objPotentialSources   The current potential sources to be used   *
 *                                                                             *
 * Outputs:   myMixture             The mixture object will be updated         *
 *            objTrackedSources     The tracked sources                        *
 *                                                                             *
 * Description: This function tracks sources based on the potential sources    *
 *              obtained from the beamformer.                                  *
  *                                                                             *
 ******************************************************************************/

void mixtureUpdate(struct objMixture *myMixture, struct objTrackedSources *myTrackedSources, struct objPotentialSources *myPotentialSources)
{

    // Index to loop for each source
    unsigned char indexQ;

    // Loop for each filter
    unsigned char indexFilter;

    // Loop for each source
    unsigned char indexSource;

    // Index of a specific ID
    unsigned int indexID;

    // Loop for each particle within the filter
    unsigned int indexParticle;

    // sum_i_0_N-1(wf(q),i * p(Oq|xvj,i))
    float sum_wfqi_pOq_xvji;

    // P(f)
    float Pf;

    // P(O|f);
    float POf;

    // P(f|O)
    float PfO;

    // Sum of all Pq,j(t), Pq(t)(H0) and Pq(t)(H2)
    float sum_Pqj_PqH0_PqH2;

    // Number of possibilities for f
    unsigned int fPossibilities;

    // Index for each possibility of f
    unsigned char indexF;

    // Conflit indicator
    unsigned char conflict;

    // Pj(t)
    float Pj_t;

    // Check if a new source can be added if there is no other source near it
    float potentialX;
    float potentialY;
    float filterX;
    float filterY;
    float filterZ;
    float angle;
    char cancelNewSource;

#ifdef USE_SIMD

    // SIMD registers
    __m128_mod regA, regB, regC, regD;

    // Variables needed for SSE computations
    float* weightArrayPtr;
    float* pOq_xvjiArrayPtr;

#endif

    /***************************************************************************
    * Step 1: Predict the state svj(t) from svj(t-1) for each source j         *
    ***************************************************************************/

    // Loop for each filter
    for (indexFilter = 0; indexFilter < myMixture->MIXTURE_NBFILTERS; indexFilter++)
    {

        // Check if a filter exists at this location
        if (myMixture->filterID[indexFilter] != MIXTURE_NOFILTER)
        {

            // Predict the state of the particles in this filter
            filterPredict(&myMixture->listFilters[indexFilter]);

        }

    }


    /***************************************************************************
    * Step 2: Compute instantaneous direction probabilities                    *
    ***************************************************************************/

    // +-------------------------------------------------------------------+
    // | Step A: Compute Pq                                                |
    // +-------------------------------------------------------------------+

    for (indexQ = 0; indexQ < myMixture->BF_MAXSOURCES; indexQ++)
    {
        myMixture->Pq[indexQ] = myPotentialSources->sourcesProbability[indexQ];
    }

    // +-------------------------------------------------------------------+
    // | Step B: Compute the a priori probability for each assignment      |
    // +-------------------------------------------------------------------+

    // Loop for each potential source q = 0, 1, ... Q-1
    for (indexQ = 0; indexQ < myMixture->BF_MAXSOURCES; indexQ++)
    {

        // +---------------------------------------------------------------+
        // | Step I: Probability this source is assigned to a false detect |
        // +---------------------------------------------------------------+

        // (1 - Pq) * Pfalse
        myMixture->Pfq[0][indexQ] = (1.0f - myMixture->Pq[indexQ]) * myMixture->MIXTURE_PFALSE;

        // +---------------------------------------------------------------+
        // | Step II: Probability this source is a new source              |
        // +---------------------------------------------------------------+

        // Pq * Pnew
        myMixture->Pfq[1][indexQ] = myMixture->Pq[indexQ] * myMixture->MIXTURE_PNEW;

        // +---------------------------------------------------------------+
        // | Step III: Probability this source is assigned to given filter |
        // +---------------------------------------------------------------+

        // Loop for each filter
        for (indexFilter = 0; indexFilter < myMixture->MIXTURE_NBFILTERS; indexFilter++)
        {

            // Check if a filter exists at this location
            if (myMixture->filterID[indexFilter] != MIXTURE_NOFILTER)
            {

                // Pq * P(Obsj(t)|Ov(t-1))
                myMixture->Pfq[2 + indexFilter][indexQ] = myMixture->Pq[indexQ] * (myMixture->listFilters[indexFilter]).P_Obsj_t__Ov_tm1;

            }

            // If it is not the case
            else
            {
                // Then set the probability to zero
                myMixture->Pfq[2 + indexFilter][indexQ] = 0.0f;
            }

        }

    }

    // +-------------------------------------------------------------------+
    // | Step C: Compute the probability according to current observation  |
    // +-------------------------------------------------------------------+

    // Loop for each potential source q = 0, 1, ... Q-1
    for (indexQ = 0; indexQ < myMixture->BF_MAXSOURCES; indexQ++)
    {

        // +---------------------------------------------------------------+
        // | Step I: Probability this source is assigned to a false detect |
        // +---------------------------------------------------------------+

        // 1/4pi
        myMixture->POq_fq[0][indexQ] = 1/(4.0f*M_PI);

        // +---------------------------------------------------------------+
        // | Step II: Probability this source is a new source              |
        // +---------------------------------------------------------------+

        // 1/4pi
        myMixture->POq_fq[1][indexQ] = 1/(4.0f*M_PI);

        // +---------------------------------------------------------------+
        // | Step III: Probability this source is assigned to given filter |
        // +---------------------------------------------------------------+

        // Loop for each filter
        for (indexFilter = 0; indexFilter < myMixture->MIXTURE_NBFILTERS; indexFilter++)
        {

            // Check if a filter exists at this location
            if (myMixture->filterID[indexFilter] != MIXTURE_NOFILTER)
            {

                // Compute p(Oq|xvj,i) for i = 0, 1, ... , N-1
                filterProb(&myMixture->listFilters[indexFilter], myPotentialSources->sourcesPosition[indexQ][0], myPotentialSources->sourcesPosition[indexQ][1], myPotentialSources->sourcesPosition[indexQ][2], myMixture->pOq_xvji);

                // Compute sum_i_0_N-1(wf(q),i * p(Oq|xvj,i))

                sum_wfqi_pOq_xvji = 0.0f;

#ifndef USE_SIMD

                // +--------------------------------------------------------+
                // | No hardware acceleration                               |
                // +--------------------------------------------------------+

                for (indexParticle = 0; indexParticle < myMixture->FILTER_NBPARTICLES; indexParticle++)
                {

                    sum_wfqi_pOq_xvji += myMixture->listFilters[indexFilter].weight[indexParticle] * myMixture->pOq_xvji[indexParticle];

                }

#else

                // +--------------------------------------------------------+
                // | With SSE instructions                                  |
                // +--------------------------------------------------------+

                // Save the pointer of the weight array
                weightArrayPtr = myMixture->listFilters[indexFilter].weight;

                // Save the pointer of the pOq_xvji array
                pOq_xvjiArrayPtr = myMixture->pOq_xvji;

                // Set regA to 0 everywhere
                regA.m128_f32[0] = 0.0;
                regA.m128_f32[1] = 0.0;
                regA.m128_f32[2] = 0.0;
                regA.m128_f32[3] = 0.0;

                for (indexParticle = 0; (indexParticle + 3) < myMixture->FILTER_NBPARTICLES; indexParticle+=4)
                {

                    // Load weight
                    regB.m128 = _mm_load_ps(&weightArrayPtr[indexParticle]);

                    // Load pOq_xvji
                    regC.m128 = _mm_load_ps(&pOq_xvjiArrayPtr[indexParticle]);

                    // weight * pOq_xvji
                    regD.m128 = _mm_mul_ps(regB.m128, regC.m128);

                    // Add to accumulator
                    regA.m128 = _mm_add_ps(regA.m128, regD.m128);

                }

                // Add all four floats to get overall sum
                sum_wfqi_pOq_xvji = regA.m128_f32[0] + regA.m128_f32[1] + regA.m128_f32[2] + regA.m128_f32[3];

                // Complete for particles left
                if (indexParticle > myMixture->FILTER_NBPARTICLES)
                {
                    indexParticle -= 4;
                }

                for (indexParticle=0; indexParticle < myMixture->FILTER_NBPARTICLES; indexParticle++)
                {
                    sum_wfqi_pOq_xvji += myMixture->listFilters[indexFilter].weight[indexParticle] * myMixture->pOq_xvji[indexParticle];
                }

#endif

                // p(Oq|f(q)) = sum_i_0_N-1(wf(q),i * p(Oq|xvj,i))
                myMixture->POq_fq[2 + indexFilter][indexQ] = sum_wfqi_pOq_xvji;


            }

            // If it is not the case
            else
            {

                // Then set the probability to zero
                myMixture->POq_fq[2 + indexFilter][indexQ] = 0.0f;

            }

        }

    }

    /***************************************************************************
    * Step 3: Compute Pq,j(t), Pq(t)(H0) and Pq(t)(H2)                         *
    ***************************************************************************/

    // Calculate the number of possibilities for f:
    // base ^ Q
    fPossibilities = (unsigned int) pow(myMixture->MIXTURE_BASEF, myMixture->BF_MAXSOURCES);

    // Set the number of possibilities to initial state
    for (indexQ = 0; indexQ < myMixture->BF_MAXSOURCES; indexQ++)
    {
        myMixture->fq[indexQ] = myMixture->MIXTURE_BASEF - 1;
    }

    // Initialize Pq,j(t), Pq(t)(H0) and Pq(t)(H2)
    for (indexQ = 0; indexQ < myMixture->BF_MAXSOURCES; indexQ++)
    {

        myMixture->Pq_t_H0[indexQ] = 0.0f;
        myMixture->Pq_t_H2[indexQ] = 0.0f;

        for (indexFilter = 0; indexFilter < myMixture->MIXTURE_NBFILTERS; indexFilter++)
        {

            myMixture->Pqj_t[indexFilter][indexQ] = 0.0f;

        }

    }

    // Calculate p(O|f) for each possibility
    for (indexF = 0; indexF < fPossibilities; indexF++)
    {

        // +-------------------------------------------------------------------+
        // | Step A: Increment f(q)                                            |
        // +-------------------------------------------------------------------+

        // For this part, we need to increment f with a specific base
        // For instance, if there are 4 sources and 3 filters, we get
        // a base 5 (2+3; the 2 comes from the false detection and new
        // sources) number with 4 digits:
        //
        // +---------+-------+-------+-------+-------+
        // |  indexF | f(3)  | f(2)  | f(1)  | f(0)  |
        // +---------+-------+-------+-------+-------+
        // |       0 |   0   |   0   |   0   |   0   |
        // +---------+-------+-------+-------+-------+
        // |       1 |   0   |   0   |   0   |   1   |
        // +---------+-------+-------+-------+-------+
        // |       2 |   0   |   0   |   0   |   2   |
        // +---------+-------+-------+-------+-------+
        // |       3 |   0   |   0   |   0   |   3   |
        // +---------+-------+-------+-------+-------+
        // |       4 |   0   |   0   |   0   |   4   |
        // +---------+-------+-------+-------+-------+
        // |       5 |   0   |   0   |   1   |   0   |
        // +---------+-------+-------+-------+-------+
        // |       6 |   0   |   0   |   1   |   1   |
        // +---------+-------+-------+-------+-------+
        // |       7 |   0   |   0   |   1   |   2   |
        // +---------+-------+-------+-------+-------+
        // |       8 |   0   |   0   |   1   |   3   |
        // +---------+-------+-------+-------+-------+
        // |       9 |   0   |   0   |   1   |   4   |
        // +---------+-------+-------+-------+-------+
        // |      10 |   0   |   0   |   2   |   0   |
        // +---------+-------+-------+-------+-------+
        // |   ...   |  ...  |  ...  |  ...  |  ...  |
        // +---------+-------+-------+-------+-------+
        // |     625 |   4   |   4   |   4   |   4   |
        // +---------+-------+-------+-------+-------+
        //
        // In order to do so, we increment the least significant digit
        // If it reaches the base value, then a carry is sent to the
        // next digit, and so on.

        // Increment f
        myMixture->fq[0]++;

        // Check for carry
        for (indexQ = 0; indexQ < myMixture->BF_MAXSOURCES; indexQ++)
        {
            // If there is a carry
            if (myMixture->fq[indexQ] == myMixture->MIXTURE_BASEF)
            {
                // Then increment the next digit if not the last one
                if (indexQ != (myMixture->BF_MAXSOURCES - 1))
                {
                    myMixture->fq[indexQ+1]++;
                }

                // Reset the current digit
                myMixture->fq[indexQ] = 0;
            }
        }

        // +-------------------------------------------------------------------+
        // | Step B: Check if there is no conflict                             |
        // +-------------------------------------------------------------------+

        // Initially starts with no conflict
        conflict = MIXTURE_NOCONFLICT;

        for (indexFilter = 0; indexFilter < myMixture->MIXTURE_NBFILTERS; indexFilter++)
        {
            myMixture->filterConflict[indexFilter] = MIXTURE_NOCONFLICT;
        }

        for (indexQ = 0; indexQ < myMixture->BF_MAXSOURCES; indexQ++)
        {
            // Check if there is already a source attributed to this filter
            //
            // f(q) = {0, 1, 2, ... , (number of filters - 1) + 2}
            //
            // Take f(q) and subtract 2:
            //
            // (f(q) - 2) = {-2, -1, 0, ..., (number of filters - 1)}
            //
            // Then only consider the results >= 0
            //
            // filterConflict(0) = CONFLIT/NO CONFLICT
            // filterConflict(1) = CONFLIT/NO CONFLICT
            // ...
            // filterConflict(number of filters - 1) = CONFLIT/NO CONFLICT
            //
            // If there is already the value CONFLICT at a given index, then
            // it means another source has been mapped to this filter and
            // thus a general conflict occurs for this f

            // Consider only positive index for filters
            if ((myMixture->fq[indexQ]-2) >= 0)
            {

                // Check if there is already another source mapped to this filter
                if (myMixture->filterConflict[myMixture->fq[indexQ]-2] == MIXTURE_CONFLICT)
                {
                    // Then this combination has a conflict
                    conflict = MIXTURE_CONFLICT;

                    // Exit loop
                    break;

                }
                else
                {
                    // Otherwise mark this spot as being already taken
                    myMixture->filterConflict[myMixture->fq[indexQ]-2] = MIXTURE_CONFLICT;
                }

            }

        }

        // If there is a conflict, break
        if (conflict == MIXTURE_CONFLICT)
        {
            break;
        }

        // +-------------------------------------------------------------------+
        // | Step C: Compute P(f)                                              |
        // +-------------------------------------------------------------------+

        Pf = 1.0f;

        for (indexQ = 0; indexQ < myMixture->BF_MAXSOURCES; indexQ++)
        {
            Pf *= myMixture->Pfq[myMixture->fq[indexQ]][indexQ];
        }

        // +-------------------------------------------------------------------+
        // | Step D: Compute P(O|f)                                            |
        // +-------------------------------------------------------------------+

        POf = 1.0f;

        for (indexQ = 0; indexQ < myMixture->BF_MAXSOURCES; indexQ++)
        {
            POf *= myMixture->POq_fq[myMixture->fq[indexQ]][indexQ];
        }

        // +-------------------------------------------------------------------+
        // | Step E: Compute P(f|O)                                            |
        // +-------------------------------------------------------------------+

        PfO = Pf * POf;

        // +-------------------------------------------------------------------+
        // | Step F: Compute Pq(t)(H0), Pq(t)(H2) and Pq,j(t)                  |
        // +-------------------------------------------------------------------+

        for (indexQ = 0; indexQ < myMixture->BF_MAXSOURCES; indexQ++)
        {

            // Pq(t)(H0)
            if (myMixture->fq[indexQ] == 0)
            {
                myMixture->Pq_t_H0[indexQ] += PfO;
            }

            // Pq(t)(H2)
            else if (myMixture->fq[indexQ] == 1)
            {
                myMixture->Pq_t_H2[indexQ] += PfO;
            }

            // Pq,j(t)
            else
            {
                myMixture->Pqj_t[myMixture->fq[indexQ]-2][indexQ] += PfO;
            }

        }

    }

        // +-------------------------------------------------------------------+
        // | Step G: Normalize Pq,j(t), Pq(t)(H0) and Pq(t)(H2)                |
        // +-------------------------------------------------------------------+

        for (indexQ = 0; indexQ < myMixture->BF_MAXSOURCES; indexQ++)
        {

            // +---------------------------------------------------------------+
            // | Step I: Compute the sum of probability for the current source |
            // +---------------------------------------------------------------+

            sum_Pqj_PqH0_PqH2 = 0.0f;

            sum_Pqj_PqH0_PqH2 += myMixture->Pq_t_H0[indexQ];
            sum_Pqj_PqH0_PqH2 += myMixture->Pq_t_H2[indexQ];

            for (indexFilter = 0; indexFilter < myMixture->MIXTURE_NBFILTERS; indexFilter++)
            {

                sum_Pqj_PqH0_PqH2 += myMixture->Pqj_t[indexFilter][indexQ];

            }

            // +---------------------------------------------------------------+
            // | Step II: Normalize for the current source                     |
            // +---------------------------------------------------------------+

            // If the sum is smaller than a threshold, then just set everything
            // to zero otherwise there will be an overflow

            if (sum_Pqj_PqH0_PqH2 > 1E-20f)
            {

                myMixture->Pq_t_H0[indexQ] /= sum_Pqj_PqH0_PqH2;
                myMixture->Pq_t_H2[indexQ] /= sum_Pqj_PqH0_PqH2;

                for (indexFilter = 0; indexFilter < myMixture->MIXTURE_NBFILTERS; indexFilter++)
                {

                        myMixture->Pqj_t[indexFilter][indexQ] /= sum_Pqj_PqH0_PqH2;

                }

            }
            else
            {

                myMixture->Pq_t_H0[indexQ] = 0.0f;
                myMixture->Pq_t_H2[indexQ] = 0.0f;

                for (indexFilter = 0; indexFilter < myMixture->MIXTURE_NBFILTERS; indexFilter++)
                {

                        myMixture->Pqj_t[indexFilter][indexQ] = 0.0f;

                }


            }

        }


    /***************************************************************************
    * Step 4: Compute updated particle weights                                 *
    ***************************************************************************/

    for (indexFilter = 0; indexFilter < myMixture->MIXTURE_NBFILTERS; indexFilter++)
    {

        // Check if there is a filter at this location
        if (myMixture->filterID[indexFilter] != MIXTURE_NOFILTER)
        {

            // +---------------------------------------------------------------+
            // | Step A: Update the weights of particles                       |
            // +---------------------------------------------------------------+

            filterUpdateWeights(&myMixture->listFilters[indexFilter], &myMixture->Pqj_t[indexFilter][0], myPotentialSources);

            // +---------------------------------------------------------------+
            // | Step B: Update a priori probabilities                         |
            // +---------------------------------------------------------------+

            filterUpdateAPriori(&myMixture->listFilters[indexFilter], &myMixture->Pqj_t[indexFilter][0]);

            // +---------------------------------------------------------------+
            // | Step C: Check if the source really exists                     |
            // +---------------------------------------------------------------+

            // In order to do so, the maximum probability that the source exists
            // P(Ej|Ov(t-1)) is kept in memory. When this value exceeds a given
            // threshold, the probability is set to 1 such that the source
            // exists for now on.

            if (myMixture->countSourceExists[indexFilter] < myMixture->MIXTURE_CONFIRMCOUNT)
            {

                myMixture->countSourceExists[indexFilter]++;
                myMixture->sumProbSourceExists[indexFilter] += myMixture->listFilters[indexFilter].P_Ej__Ov_tm1;

            }

            if (myMixture->listFilters[indexFilter].P_Ej__Ov_tm1 > myMixture->confirmSourceExists[indexFilter])
            {
                myMixture->confirmSourceExists[indexFilter] = myMixture->listFilters[indexFilter].P_Ej__Ov_tm1;
            }

            if ((myMixture->confirmSourceExists[indexFilter] > myMixture->MIXTURE_CONFIRMEXISTS) && (myMixture->countSourceExists[indexFilter] == myMixture->MIXTURE_CONFIRMCOUNT))
            {
                if ((myMixture->sumProbSourceExists[indexFilter]/myMixture->MIXTURE_CONFIRMCOUNT) > myMixture->MIXTURE_CONFIRMCOUNTTS)
                {

                    myMixture->confirmSourceExists[indexFilter] = 1;
                    myMixture->flagSourceExists[indexFilter] = MIXTURE_SOURCEEXISTS;

                }

                else
                {

                    myMixture->countSourceExists[indexFilter] = 0;
                    myMixture->sumProbSourceExists[indexFilter] = 0;

                }
            }

            // +---------------------------------------------------------------+
            // | Step D: Compute Pj(t)                                         |
            // +---------------------------------------------------------------+

            Pj_t = 0.0f;

            for (indexQ = 0; indexQ < myMixture->BF_MAXSOURCES; indexQ++)
            {

                Pj_t += myMixture->Pqj_t[indexFilter][indexQ];

            }

            // +---------------------------------------------------------------+
            // | Step E: Check if the source has not been observed             |
            // +---------------------------------------------------------------+

            if (myMixture->flagSourceExists[indexFilter] == MIXTURE_SOURCEDOESNOTEXIST)
            {

                if (Pj_t < myMixture->MIXTURE_TOBSPROB)
                {
                        myMixture->cumulativeTimeProb[indexFilter]++;
                }
                else
                {
                        myMixture->cumulativeTimeProb[indexFilter] = 0;
                }

            }
            else
            {
                if (Pj_t < myMixture->MIXTURE_TOBS1)
                {
                        myMixture->cumulativeTime1[indexFilter]++;
                }
                else
                {
                        myMixture->cumulativeTime1[indexFilter] = 0;
                }

                if (Pj_t < myMixture->MIXTURE_TOBS2)
                {
                        myMixture->cumulativeTime2[indexFilter]++;
                }
                else
                {
                        myMixture->cumulativeTime2[indexFilter] = 0;
                }
            }

        }

    }

    /***************************************************************************
    * Step 5: Add or remove sources if necessary                               *
    ***************************************************************************/

        // +-------------------------------------------------------------------+
        // | Step A: Track a new source if needed                              |
        // +-------------------------------------------------------------------+

        for (indexQ = 0; indexQ < myMixture->BF_MAXSOURCES; indexQ++)
        {

            // Check if there is a need to track a new source
            if (myMixture->Pq_t_H2[indexQ] > myMixture->MIXTURE_NEWTHRESHOLD)
            {

                // Before adding, check if there is no other source near in the
                // horizontal plane

                cancelNewSource = 0;

                for (indexFilter = 0; indexFilter < myMixture->MIXTURE_NBFILTERS; indexFilter++)
                {

                    // If there is a filter
                    if (myMixture->filterID[indexFilter] != MIXTURE_NOFILTER)
                    {

                        // Get the filter position
                        filterEstimatePosition(&myMixture->listFilters[indexFilter],&filterX,&filterY,&filterZ);

                        // Get the potential source position
                        potentialX = myPotentialSources->sourcesPosition[indexQ][0];
                        potentialY = myPotentialSources->sourcesPosition[indexQ][1];

                        // Get the horizontal angle between the two
                        angle = fabsf(atan2(filterY,filterX) - atan2(potentialY,potentialX));

                        if (angle > M_PI)
                        {
                            angle -= M_PI;
                        }

					}

                }

                // Then if the source can be added, proceed
                if (cancelNewSource == 0)
                {

                    // Check if there is some room to track a new source
                    if (myMixture->numberTrackingInProgress < myMixture->MIXTURE_NBFILTERS)
                    {

                        // Increment the number of sources being tracked
                        myMixture->numberTrackingInProgress++;

                        // Find a free spot
                        for (indexFilter = 0; indexFilter < myMixture->MIXTURE_NBFILTERS; indexFilter++)
                        {

                            // If this is a free spot
                            if (myMixture->filterID[indexFilter] == MIXTURE_NOFILTER)
                            {

                                // Track the source with this filter
                                myMixture->filterID[indexFilter] = idManagerReserveID(&myMixture->idManager);

                                // Reinitialize the filter for this new position
                                // The probability that the source really exists is given by the
                                // value of Pq(t)(H2)
                                filterActivate(&myMixture->listFilters[indexFilter], myPotentialSources->sourcesPosition[indexQ][0], myPotentialSources->sourcesPosition[indexQ][1], myPotentialSources->sourcesPosition[indexQ][2], myMixture->Pq_t_H2[indexQ]);

                                // Exit the loop
                                break;

                            }

                        }

                    }

                }

            }

        }

        // +-------------------------------------------------------------------+
        // | Step B: Check if there is a need to stop tracking a source        |
        // +-------------------------------------------------------------------+

        for (indexFilter = 0; indexFilter < myMixture->MIXTURE_NBFILTERS; indexFilter++)
        {

            // Check if there is a filter at this location
            if (myMixture->filterID[indexFilter] != MIXTURE_NOFILTER)
            {

                if (myMixture->flagSourceExists[indexFilter] == MIXTURE_SOURCEDOESNOTEXIST)
                {

                    // Check if the cumulative time of inactivity is exceeded when source
                    // is in probation
                    if (myMixture->cumulativeTimeProb[indexFilter] >= myMixture->MIXTURE_CUMULATIVETIMEPROB)
                    {

                        // Release the ID
                        idManagerRestoreID(&myMixture->idManager, myMixture->filterID[indexFilter]);

                        // Remove this filter
                        myMixture->filterID[indexFilter] = MIXTURE_NOFILTER;
                        myMixture->confirmSourceExists[indexFilter] = 0;
                        myMixture->flagSourceExists[indexFilter] = MIXTURE_SOURCEDOESNOTEXIST;
                        myMixture->countSourceExists[indexFilter] = 0;
                        myMixture->cumulativeTimeProb[indexFilter] = 0;
                        myMixture->cumulativeTime1[indexFilter] = 0;
                        myMixture->cumulativeTime2[indexFilter] = 0;
                        myMixture->numberTrackingInProgress--;

                    }

                }

                else
                {

                    // Check if the first cumulative time of inactivity is exceeded
                    if (myMixture->cumulativeTime1[indexFilter] >= myMixture->MIXTURE_CUMULATIVETIME1)
                    {

                        // Release the ID
                        idManagerRestoreID(&myMixture->idManager, myMixture->filterID[indexFilter]);

                        // Remove this filter
                        myMixture->filterID[indexFilter] = MIXTURE_NOFILTER;
                        myMixture->confirmSourceExists[indexFilter] = 0;
                        myMixture->flagSourceExists[indexFilter] = MIXTURE_SOURCEDOESNOTEXIST;
                        myMixture->countSourceExists[indexFilter] = 0;
                        myMixture->cumulativeTimeProb[indexFilter] = 0;
                        myMixture->cumulativeTime1[indexFilter] = 0;
                        myMixture->cumulativeTime2[indexFilter] = 0;
                        myMixture->numberTrackingInProgress--;

                    }

                    // Check if the second cumulative time of inactivity is exceeded
                    else if (myMixture->cumulativeTime2[indexFilter] >= myMixture->MIXTURE_CUMULATIVETIME2)
                    {

                        // Release the ID
                        idManagerRestoreID(&myMixture->idManager, myMixture->filterID[indexFilter]);

                        // Remove this filter
                        myMixture->filterID[indexFilter] = MIXTURE_NOFILTER;
                        myMixture->confirmSourceExists[indexFilter] = 0;
                        myMixture->flagSourceExists[indexFilter] = MIXTURE_SOURCEDOESNOTEXIST;
                        myMixture->countSourceExists[indexFilter] = 0;
                        myMixture->cumulativeTimeProb[indexFilter] = 0;
                        myMixture->cumulativeTime1[indexFilter] = 0;
                        myMixture->cumulativeTime2[indexFilter] = 0;
                        myMixture->numberTrackingInProgress--;

                    }

                }


            }

        }

    /***************************************************************************
    * Step 6: Compute source localisation estimate xv_j(t) for each source     *
    ***************************************************************************/

        // Reset the number of sources
        idListReset(&myTrackedSources->sourcesID);

        // Only estimate the position of the sources that really exists
        for (indexSource = 0; indexSource < myMixture->MIXTURE_NBFILTERS; indexSource++)
        {

            // Check if there is a filter at this location
            if (myMixture->filterID[indexSource] != MIXTURE_NOFILTER)
            {

                // Check if the source was confirmed as an existing source
                if (myMixture->flagSourceExists[indexSource] == MIXTURE_SOURCEEXISTS)
                {

                    // Check if the buffer of the filter has been filled at
                    // least once. If not, do not add any source for now.
                    if (myMixture->listFilters[indexSource].bufferWasFull == FILTER_BUFFERFULL)
                    {

                        // Then add it to the list of sources
                        indexID = idListAdd(&myTrackedSources->sourcesID, myMixture->filterID[indexSource]);

                        // Remove this ID from the list of available/used IDs such that is
                        // is not used anymore to ensure the singularity of the source
                        idManagerDeleteID(&myMixture->idManager, myMixture->filterID[indexSource]);

                        // Get the position for this source
                        filterEstimatePosition(&myMixture->listFilters[indexSource], &myTrackedSources->sourcesPosition[indexID][0], &myTrackedSources->sourcesPosition[indexID][1], &myTrackedSources->sourcesPosition[indexID][2]);

                    }

                }

            }

        }


    /***************************************************************************
    * Step 7: Resample particles for each source if necessary                  *
    ***************************************************************************/

    for (indexFilter = 0; indexFilter < myMixture->MIXTURE_NBFILTERS; indexFilter++)
    {

        // Check if there is a filter at this location
        if (myMixture->filterID[indexFilter] != MIXTURE_NOFILTER)
        {

            filterResampleIfNeeded(&myMixture->listFilters[indexFilter]);

        }

    }


}
