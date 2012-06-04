/******************************************************************************* 
 * ManyEars: MCRA noise estimator - Source Code                                *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Author: Francois Grondin                                                    *
 * Original Code: Jean-Marc Valin                                              *
 * Modified Code: Simon Briere                                                 *
 * Version: 1.1.0                                                              *
 * Date: August 12th, 2010                                                     *
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

#include "Preprocessing/mcra.h"

/*******************************************************************************
 * mcraInit                                                                    *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myMCRA          The mcra object to be initialized              *
 *              myParameters    List of parameters that is used to configure   *
 *                              the object                                     *
 *                                                                             *
 * Outputs:     myMCRA          The mcra initialized object                    *
 *                                                                             *
 * Description: This function initializes the mcra object                      *
 *                                                                             *
 ******************************************************************************/

void mcraInit(struct objMcra *myMCRA, struct ParametersStruct *myParameters, unsigned int size)
{

    // Index to go through each element of the window
    unsigned int i;

    // Index to go through each element of the arrays
    unsigned int k;

    // *************************************************************************
    // * STEP 1: Load parameters                                               *
    // *************************************************************************

    myMCRA->MCRA_ALPHAD = myParameters->P_MCRA_ALPHAD;
    myMCRA->MCRA_ALPHAP = myParameters->P_MCRA_ALPHAP;
    myMCRA->MCRA_ALPHAS = myParameters->P_MCRA_ALPHAS;
    myMCRA->MCRA_DELTA = myParameters->P_MCRA_DELTA;
    myMCRA->MCRA_L = myParameters->P_MCRA_L;

    myMCRA->MICST_FRAMESIZE = size;
    myMCRA->MICST_HALFFRAMESIZE = myMCRA->MICST_FRAMESIZE / 2;

    myMCRA->MCRA_BSIZE = GLOBAL_BSIZE;
    myMCRA->MCRA_WINDOW = newTable1D(3, sizeof(float));
    myMCRA->MCRA_WINDOW[0] = GLOBAL_WINDOW0;
    myMCRA->MCRA_WINDOW[1] = GLOBAL_WINDOW1;
    myMCRA->MCRA_WINDOW[2] = GLOBAL_WINDOW2;

    // *************************************************************************
    // * STEP 2: Initialize context                                            *
    // *************************************************************************

    // Flag the fact that this object has just been initialized
    myMCRA->firstProcessing = 1;

    // Reset the frame counter to 1
    myMCRA->l = 1;

    // Initialize the arrays
    myMCRA->b = (float*) newTable1D(myMCRA->MCRA_BSIZE, sizeof(float));
    myMCRA->S = (float*) newTable1D(myMCRA->MICST_FRAMESIZE, sizeof(float));
    myMCRA->S_min = (float*) newTable1D(myMCRA->MICST_FRAMESIZE, sizeof(float));
    myMCRA->S_min_prev = (float*) newTable1D(myMCRA->MICST_FRAMESIZE, sizeof(float));
    myMCRA->S_prev = (float*) newTable1D(myMCRA->MICST_FRAMESIZE, sizeof(float));
    myMCRA->S_tmp = (float*) newTable1D(myMCRA->MICST_FRAMESIZE, sizeof(float));
    myMCRA->S_tmp_prev = (float*) newTable1D(myMCRA->MICST_FRAMESIZE, sizeof(float));
    myMCRA->Sf = (float*) newTable1D(myMCRA->MICST_FRAMESIZE, sizeof(float));
    myMCRA->lambdaD = (float*) newTable1D(myMCRA->MICST_FRAMESIZE, sizeof(float));
    myMCRA->lambdaD_next = (float*) newTable1D(myMCRA->MICST_FRAMESIZE, sizeof(float));

    // Copy values of the windows b in the object
    for (i = 0; i < myMCRA->MCRA_BSIZE; i++)
    {
        myMCRA->b[i] = myMCRA->MCRA_WINDOW[i];
    }

    // Initialize all arrays to 0
    for (k = 0; k < myMCRA->MICST_FRAMESIZE; k++)
    {

        myMCRA->S[k] = 0;
        myMCRA->S_min[k] = 0;
        myMCRA->S_min_prev[k] = 0;
        myMCRA->S_prev[k] = 0;
        myMCRA->S_tmp[k] = 0;
        myMCRA->S_tmp_prev[k] = 0;
        myMCRA->Sf[k] = 0;
        myMCRA->lambdaD[k] = 0;
        myMCRA->lambdaD_next[k] = 0;

    }

}


/*******************************************************************************
 * mcraTerminate                                                               *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myMCRA          The mcra object to be terminated               *
 *                                                                             *
 * Outputs:     (none)                                                         *
 *                                                                             *
 * Description: This function terminates the mcra object                       *
 *                                                                             *
 ******************************************************************************/

void mcraTerminate(struct objMcra *myMCRA)
{

    // Free memory
    deleteTable1D((void*) myMCRA->b);
    deleteTable1D((void*) myMCRA->S);
    deleteTable1D((void*) myMCRA->S_min);
    deleteTable1D((void*) myMCRA->S_min_prev);
    deleteTable1D((void*) myMCRA->S_prev);
    deleteTable1D((void*) myMCRA->S_tmp);
    deleteTable1D((void*) myMCRA->S_tmp_prev);
    deleteTable1D((void*) myMCRA->Sf);
    deleteTable1D((void*) myMCRA->lambdaD);
    deleteTable1D((void*) myMCRA->lambdaD_next);
    deleteTable1D((void*) myMCRA->MCRA_WINDOW);

}

/*******************************************************************************
 * mcraClone                                                                   *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myMCRASource        The mcra object to provide values          *
 *                                                                             *
 * Outputs:     myMCRADest          The mcra object to receive new values      *
 *                                                                             *
 * Description: This function copies the values from myMCRASource to           *
 *              myMCRADest.                                                    *
 *                                                                             *
 ******************************************************************************/

void mcraClone(struct objMcra* myMCRADest, struct objMcra* myMCRASource)
{

    unsigned int k;

#ifdef USE_SIMD
    // SIMD registers
    __m128_mod regA;
#endif

    // Make sure both have the same frame size
    if (myMCRADest->MICST_FRAMESIZE == myMCRASource->MICST_FRAMESIZE)
    {

#ifndef USE_SIMD

        // Then copy all arrays
        for (k = 0; k < myMCRADest->MICST_FRAMESIZE; k++)
        {

            myMCRADest->S[k] = myMCRASource->S[k];
            myMCRADest->S_min[k] = myMCRASource->S_min[k];
            myMCRADest->S_min_prev[k] = myMCRASource->S_min_prev[k];
            myMCRADest->S_prev[k] = myMCRASource->S_prev[k];
            myMCRADest->S_tmp[k] = myMCRASource->S_tmp[k];
            myMCRADest->S_tmp_prev[k] = myMCRASource->S_tmp_prev[k];
            myMCRADest->Sf[k] = myMCRASource->Sf[k];
            myMCRADest->lambdaD[k] = myMCRASource->lambdaD[k];
            myMCRADest->lambdaD_next[k] = myMCRASource->lambdaD_next[k];

        }

#else

        // Then copy all arrays
        for (k = 0; k < myMCRADest->MICST_FRAMESIZE; k+=4)
        {

            regA.m128 = _mm_load_ps(&myMCRASource->S[k]);
            _mm_store_ps(&myMCRADest->S[k], regA.m128);

            regA.m128 = _mm_load_ps(&myMCRASource->S_min[k]);
            _mm_store_ps(&myMCRADest->S_min[k], regA.m128);

            regA.m128 = _mm_load_ps(&myMCRASource->S_min_prev[k]);
            _mm_store_ps(&myMCRADest->S_min_prev[k], regA.m128);

            regA.m128 = _mm_load_ps(&myMCRASource->S_prev[k]);
            _mm_store_ps(&myMCRADest->S_prev[k], regA.m128);

            regA.m128 = _mm_load_ps(&myMCRASource->S_tmp[k]);
            _mm_store_ps(&myMCRADest->S_tmp[k], regA.m128);

            regA.m128 = _mm_load_ps(&myMCRASource->S_tmp_prev[k]);
            _mm_store_ps(&myMCRADest->S_tmp_prev[k], regA.m128);

            regA.m128 = _mm_load_ps(&myMCRASource->Sf[k]);
            _mm_store_ps(&myMCRADest->Sf[k], regA.m128);

            regA.m128 = _mm_load_ps(&myMCRASource->lambdaD[k]);
            _mm_store_ps(&myMCRADest->lambdaD[k], regA.m128);

            regA.m128 = _mm_load_ps(&myMCRASource->lambdaD_next[k]);
            _mm_store_ps(&myMCRADest->lambdaD_next[k], regA.m128);

        }

#endif

    }

}

/*******************************************************************************
 * mcraProcessFrame                                                            *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myMCRA          The mcra object to use to process the frame    *
 *              xPower          The array where lies the power of the          *
 *                              frequency representation of the current frame  *
 *                              ( |X(k)|^2 for all k )                         *
 *                                                                             *
 * Outputs:     myMCRA          The mcra object with some values updated       *
 *              sigma           The estimated noise in an array in the         *
 *                              frequency domain                               *
 *                                                                             *
 * Description: This function processes the new frame and returns the          *
 *              estimated noise using the MCRA technique                       *
 *                                                                             *
 ******************************************************************************/

void mcraProcessFrame(struct objMcra *myMCRA, float *xPower, float *sigma)
{

    // Index to go through each array
    unsigned int k;

    // Index to go through window b
    signed int indexB;

#ifdef USE_SIMD
    // SIMD registers
    __m128_mod regA, regB, regC, regD, regE, regF, regG, regH;
#endif

    /***************************************************************************
     * Step 1: Smooth the spectrum in frequency with window b                  *
     **************************************************************************/

    // Apply a linear convolution in the frequency-domain

    for (k = 0; k < myMCRA->MICST_FRAMESIZE; k++)
    {

        myMCRA->Sf[k] = 0;

        for (indexB = -1 * ((myMCRA->MCRA_BSIZE-1)/2); indexB <= ((signed int) ((myMCRA->MCRA_BSIZE-1)/2)); indexB++)
        {
            // Check if the index is out of bound
            if ( ((k + indexB) > 0) && ((k + indexB) < myMCRA->MICST_FRAMESIZE) )
            {

                myMCRA->Sf[k] += myMCRA->b[indexB + ((myMCRA->MCRA_BSIZE-1)/2)] * xPower[k + indexB];

            }

        }

    }

    /***************************************************************************
     * Step 2: Smooth the spectrum in time                                     *
     **************************************************************************/

#ifndef USE_SIMD

    // +-----------------------------------------------------------------------+
    // | No hardware acceleration                                              |
    // +-----------------------------------------------------------------------+

    // Apply equation S = alphaS * S_previous + (1 - alphaS) * Sf

    myMCRA->S[0] = myMCRA->MCRA_ALPHAS * myMCRA->S_prev[0] + (1.0f - myMCRA->MCRA_ALPHAS) * myMCRA->Sf[0];
    myMCRA->S[myMCRA->MICST_HALFFRAMESIZE] = myMCRA->MCRA_ALPHAS * myMCRA->S_prev[myMCRA->MICST_HALFFRAMESIZE] + (1.0f - myMCRA->MCRA_ALPHAS) * myMCRA->Sf[myMCRA->MICST_HALFFRAMESIZE];

    for (k = 1; k < myMCRA->MICST_HALFFRAMESIZE; k++)
    {

        myMCRA->S[k] = myMCRA->MCRA_ALPHAS * myMCRA->S_prev[k] + (1.0f - myMCRA->MCRA_ALPHAS) * myMCRA->Sf[k];
        myMCRA->S[myMCRA->MICST_FRAMESIZE - k] = myMCRA->S[k];

    }

#else

    // +-----------------------------------------------------------------------+
    // | With SSE instructions                                                 |
    // +-----------------------------------------------------------------------+

    // Apply equation S = alphaS * S_previous + (1 - alphaS) * Sf

    myMCRA->S[myMCRA->MICST_HALFFRAMESIZE] = myMCRA->MCRA_ALPHAS * myMCRA->S_prev[myMCRA->MICST_HALFFRAMESIZE] + (1 - myMCRA->MCRA_ALPHAS) * myMCRA->Sf[myMCRA->MICST_HALFFRAMESIZE];

    // Load the constant alphaS in regA
    regA.m128_f32[0] = myMCRA->MCRA_ALPHAS;
    regA.m128_f32[1] = myMCRA->MCRA_ALPHAS;
    regA.m128_f32[2] = myMCRA->MCRA_ALPHAS;
    regA.m128_f32[3] = myMCRA->MCRA_ALPHAS;

    // Load the constant (1-alphaS) in regB
    regB.m128_f32[0] = 1 - myMCRA->MCRA_ALPHAS;
    regB.m128_f32[1] = 1 - myMCRA->MCRA_ALPHAS;
    regB.m128_f32[2] = 1 - myMCRA->MCRA_ALPHAS;
    regB.m128_f32[3] = 1 - myMCRA->MCRA_ALPHAS;

    for (k = 0; k < myMCRA->MICST_HALFFRAMESIZE; k+= 4)
    {
        // Load S_prev[k] in regC
        regC.m128 = _mm_load_ps(&myMCRA->S_prev[k]);

        // Load Sf[k] in regD
        regD.m128 = _mm_load_ps(&myMCRA->Sf[k]);

        // alphaS * S_prev[k]
        regE.m128 = _mm_mul_ps(regA.m128,regC.m128);

        // (1 - alphaS) * Sf[k]
        regF.m128 = _mm_mul_ps(regB.m128,regD.m128);

        // alphaS * S_prev[k] + (1 - alphaS) * Sf[k]
        regG.m128 = _mm_add_ps(regE.m128,regF.m128);

        // Store results in S[k]
        _mm_store_ps(&myMCRA->S[k],regG.m128);

    }

#endif

    /***************************************************************************
     * Step 3: Make a samplewise comparison and find minimum                   *
     **************************************************************************/

#ifndef USE_SIMD

    // +-----------------------------------------------------------------------+
    // | No hardware acceleration                                              |
    // +-----------------------------------------------------------------------+

    // Check if the maximum number of frames is reached
    if (myMCRA->l == myMCRA->MCRA_L)
    {

        // Smin(k,l) = min{Stmp(k,l-1),S(k,l)}
        // Stmp(k,l) = S(k,l)

        myMCRA->S_min[0] = (myMCRA->S_tmp_prev[0] <= myMCRA->S[0]) * myMCRA->S_tmp_prev[0] + (myMCRA->S_tmp_prev[0] > myMCRA->S[0]) * myMCRA->S[0];
        myMCRA->S_tmp[0] = myMCRA->S[0];
        myMCRA->S_min[myMCRA->MICST_HALFFRAMESIZE] = (myMCRA->S_tmp_prev[myMCRA->MICST_HALFFRAMESIZE] <= myMCRA->S[myMCRA->MICST_HALFFRAMESIZE]) * myMCRA->S_tmp_prev[myMCRA->MICST_HALFFRAMESIZE] + (myMCRA->S_tmp_prev[myMCRA->MICST_HALFFRAMESIZE] > myMCRA->S[myMCRA->MICST_HALFFRAMESIZE]) * myMCRA->S[myMCRA->MICST_HALFFRAMESIZE];
        myMCRA->S_tmp[myMCRA->MICST_HALFFRAMESIZE] = myMCRA->S[myMCRA->MICST_HALFFRAMESIZE];

        for (k = 1; k < myMCRA->MICST_HALFFRAMESIZE; k++)
        {
            myMCRA->S_min[k] = (myMCRA->S_tmp_prev[k] <= myMCRA->S[k]) * myMCRA->S_tmp_prev[k] + (myMCRA->S_tmp_prev[k] > myMCRA->S[k]) * myMCRA->S[k];
            myMCRA->S_tmp[k] = myMCRA->S[k];

            myMCRA->S_min[myMCRA->MICST_FRAMESIZE - k] = myMCRA->S_min[k];
            myMCRA->S_tmp[myMCRA->MICST_FRAMESIZE - k] = myMCRA->S_tmp[k];

        }

    }

    else
    {

        // Smin(k,l) = min{Smin(k,l-1),S(k,l)}
        // Stmp(k,l) = min(Stmp(k,l-1),S(k,l)}

        myMCRA->S_min[0] = (myMCRA->S_min_prev[0] <= myMCRA->S[0]) * myMCRA->S_min_prev[0] + (myMCRA->S_min_prev[0] > myMCRA->S[0]) * myMCRA->S[0];
        myMCRA->S_tmp[0] = (myMCRA->S_tmp_prev[0] <= myMCRA->S[0]) * myMCRA->S_tmp_prev[0] + (myMCRA->S_tmp_prev[0] > myMCRA->S[0]) * myMCRA->S[0];
        myMCRA->S_min[myMCRA->MICST_HALFFRAMESIZE] = (myMCRA->S_min_prev[myMCRA->MICST_HALFFRAMESIZE] <= myMCRA->S[myMCRA->MICST_HALFFRAMESIZE]) * myMCRA->S_min_prev[myMCRA->MICST_HALFFRAMESIZE] + (myMCRA->S_min_prev[myMCRA->MICST_HALFFRAMESIZE] > myMCRA->S[myMCRA->MICST_HALFFRAMESIZE]) * myMCRA->S[myMCRA->MICST_HALFFRAMESIZE];
        myMCRA->S_tmp[myMCRA->MICST_HALFFRAMESIZE] = (myMCRA->S_tmp_prev[myMCRA->MICST_HALFFRAMESIZE] <= myMCRA->S[myMCRA->MICST_HALFFRAMESIZE]) * myMCRA->S_tmp_prev[myMCRA->MICST_HALFFRAMESIZE] + (myMCRA->S_tmp_prev[myMCRA->MICST_HALFFRAMESIZE] > myMCRA->S[myMCRA->MICST_HALFFRAMESIZE]) * myMCRA->S[myMCRA->MICST_HALFFRAMESIZE];

        for (k = 1; k < myMCRA->MICST_HALFFRAMESIZE; k++)
        {
            myMCRA->S_min[k] = (myMCRA->S_min_prev[k] <= myMCRA->S[k]) * myMCRA->S_min_prev[k] + (myMCRA->S_min_prev[k] > myMCRA->S[k]) * myMCRA->S[k];
            myMCRA->S_tmp[k] = (myMCRA->S_tmp_prev[k] <= myMCRA->S[k]) * myMCRA->S_tmp_prev[k] + (myMCRA->S_tmp_prev[k] > myMCRA->S[k]) * myMCRA->S[k];

            myMCRA->S_min[myMCRA->MICST_FRAMESIZE - k] = myMCRA->S_min[k];
            myMCRA->S_tmp[myMCRA->MICST_FRAMESIZE - k] = myMCRA->S_tmp[k];

        }

    }

#else

    // +-----------------------------------------------------------------------+
    // | With SSE instructions                                                 |
    // +-----------------------------------------------------------------------+

    // Check if the maximum number of frames is reached
    if (myMCRA->l == myMCRA->MCRA_L)
    {

        // Smin(k,l) = min{Stmp(k,l-1),S(k,l)}
        // Stmp(k,l) = S(k,l)

        myMCRA->S_min[myMCRA->MICST_HALFFRAMESIZE] = (myMCRA->S_tmp_prev[myMCRA->MICST_HALFFRAMESIZE] <= myMCRA->S[myMCRA->MICST_HALFFRAMESIZE]) * myMCRA->S_tmp_prev[myMCRA->MICST_HALFFRAMESIZE] + (myMCRA->S_tmp_prev[myMCRA->MICST_HALFFRAMESIZE] > myMCRA->S[myMCRA->MICST_HALFFRAMESIZE]) * myMCRA->S[myMCRA->MICST_HALFFRAMESIZE];
        myMCRA->S_tmp[myMCRA->MICST_HALFFRAMESIZE] = myMCRA->S[myMCRA->MICST_HALFFRAMESIZE];

        for (k = 0; k < myMCRA->MICST_HALFFRAMESIZE; k+= 4)
        {

            // Load Stmp(k,l-1) in regA
            regA.m128 = _mm_load_ps(&myMCRA->S_tmp_prev[k]);

            // Load S(k,l) in regB
            regB.m128 = _mm_load_ps(&myMCRA->S[k]);

            // Stmp(k,l-1) <= S(k,l)
            regC.m128 = _mm_cmple_ps(regA.m128, regB.m128);

            // (Stmp(k,l-1) <= S(k,l)) & Stmp(k,l-1)
            regD.m128 = _mm_and_ps(regC.m128, regA.m128);

            // Stmp(k,l-1) > S(k,l)
            regC.m128 = _mm_cmpgt_ps(regA.m128, regB.m128);

            // (Stmp(k,l-1) > S(k,l)) & S(k,l)
            regE.m128 = _mm_and_ps(regC.m128, regB.m128);

            // min{Stmp(k,l-1),S(k,l)} = (Stmp(k,l-1) <= S(k,l)) & Stmp(k,l-1) + (Stmp(k,l-1) > S(k,l)) & S(k,l)
            regF.m128 = _mm_add_ps(regD.m128,regE.m128);

            // Smin(k,l) = min{Stmp(k,l-1),S(k,l)}
            _mm_store_ps(&myMCRA->S_min[k], regF.m128);

            // Stmp(k,l) = S(k,l)
            _mm_store_ps(&myMCRA->S_tmp[k], regB.m128);

        }

    }

    else
    {

        // Smin(k,l) = min{Smin(k,l-1),S(k,l)}
        // Stmp(k,l) = min(Stmp(k,l-1),S(k,l)}

        myMCRA->S_min[myMCRA->MICST_HALFFRAMESIZE] = (myMCRA->S_min_prev[myMCRA->MICST_HALFFRAMESIZE] <= myMCRA->S[myMCRA->MICST_HALFFRAMESIZE]) * myMCRA->S_min_prev[myMCRA->MICST_HALFFRAMESIZE] + (myMCRA->S_min_prev[myMCRA->MICST_HALFFRAMESIZE] > myMCRA->S[myMCRA->MICST_HALFFRAMESIZE]) * myMCRA->S[myMCRA->MICST_HALFFRAMESIZE];
        myMCRA->S_tmp[myMCRA->MICST_HALFFRAMESIZE] = (myMCRA->S_tmp_prev[myMCRA->MICST_HALFFRAMESIZE] <= myMCRA->S[myMCRA->MICST_HALFFRAMESIZE]) * myMCRA->S_tmp_prev[myMCRA->MICST_HALFFRAMESIZE] + (myMCRA->S_tmp_prev[myMCRA->MICST_HALFFRAMESIZE] > myMCRA->S[myMCRA->MICST_HALFFRAMESIZE]) * myMCRA->S[myMCRA->MICST_HALFFRAMESIZE];

        for (k = 0; k < myMCRA->MICST_HALFFRAMESIZE; k+= 4)
        {

            // Load Stmp(k,l-1) in regA
            regA.m128 = _mm_load_ps(&myMCRA->S_tmp_prev[k]);

            // Load S(k,l) in regB
            regB.m128 = _mm_load_ps(&myMCRA->S[k]);

            // Load Smin(k,l-1) in regC
            regC.m128 = _mm_load_ps(&myMCRA->S_min_prev[k]);

            // Smin(k,l-1) <= S(k,l)
            regD.m128 = _mm_cmple_ps(regC.m128, regB.m128);

            // (Smin(k,l-1) <= S(k,l)) & Smin(k,l-1)
            regE.m128 = _mm_and_ps(regD.m128, regC.m128);

            // Smin(k,l-1) > S(k,l)
            regD.m128 = _mm_cmpgt_ps(regC.m128, regB.m128);

            // (Smin(k,l-1) > S(k,l)) & S(k,l)
            regF.m128 = _mm_and_ps(regD.m128, regB.m128);

            // min{Smin(k,l-1),S(k,l)} = (Smin(k,l-1) <= S(k,l)) & Smin(k,l-1) + (Smin(k,l-1) > S(k,l)) & S(k,l)
            regG.m128 = _mm_add_ps(regE.m128,regF.m128);

            // Stmp(k,l-1) <= S(k,l)
            regD.m128 = _mm_cmple_ps(regA.m128, regB.m128);

            // (Stmp(k,l-1) <= S(k,l)) & Stmp(k,l-1)
            regE.m128 = _mm_and_ps(regD.m128, regA.m128);

            // Stmp(k,l-1) > S(k,l)
            regD.m128 = _mm_cmpgt_ps(regA.m128, regB.m128);

            // (Stmp(k,l-1) > S(k,l)) & S(k,l)
            regF.m128 = _mm_and_ps(regD.m128, regB.m128);

            // min{Stmp(k,l-1),S(k,l)} = (Stmp(k,l-1) <= S(k,l)) & Stmp(k,l-1) + (Stmp(k,l-1) > S(k,l)) & S(k,l)
            regH.m128 = _mm_add_ps(regE.m128,regF.m128);

            // Smin(k,l) = min{Stmp(k,l-1),S(k,l)}
            _mm_store_ps(&myMCRA->S_min[k], regG.m128);

            // Stmp(k,l) = min(Stmp(k,l-1),S(k,l)}
            _mm_store_ps(&myMCRA->S_tmp[k], regH.m128);

        }

    }

#endif

    /***************************************************************************
     * Step 4: Update the noise, if needed                                     *
     **************************************************************************/

    // The noise is only updated if one of the following conditions is
    // reached (these conditions are matched for each values of k, except for
    // condition 1 which applies to all values of k)
    //
    // 1) It is the first group of L frames to be processed
    // 2) S(k,l) < (Smin(k,l) * delta)
    // 3) lambdaD(k,l) > |Y(k,l)|^2

#ifndef USE_SIMD

    // +-----------------------------------------------------------------------+
    // | No hardware acceleration                                              |
    // +-----------------------------------------------------------------------+

    for (k = 0; k < myMCRA->MICST_FRAMESIZE; k++)
    {

        // Verify the previous conditions
        if ((myMCRA->firstProcessing == 1) || (myMCRA->S[k] < (myMCRA->MCRA_DELTA * myMCRA->S_min[k])) || (myMCRA->lambdaD[k] > xPower[k]))
        {

            if (((myMCRA->firstProcessing == 1) && ((1.0/myMCRA->l) > myMCRA->MCRA_ALPHAD)))
            {

                // lambdaD(k,l+1) = (1/l) * lambdaD(k,l) + (1 - 1/l) * |Y(k,l)|^2
                myMCRA->lambdaD_next[k] = (float) ((1.0/myMCRA->l) * myMCRA->lambdaD[k] + (1.0f - (1.0f/myMCRA->l)) * xPower[k]);

            }
            else
            {

                // lambdaD(k,l+1) = alphaD * lambdaD(k,l) + (1 - alphaD) * |Y(k,l)|^2
                myMCRA->lambdaD_next[k] = myMCRA->MCRA_ALPHAD * myMCRA->lambdaD[k] + (1.0f - myMCRA->MCRA_ALPHAD) * xPower[k];

            }
        }

        // Otherwise keep the same noise
        else
        {
            myMCRA->lambdaD_next[k] = myMCRA->lambdaD[k];
        }

    }

#else

    if (myMCRA->firstProcessing != 1)
    {

        // Load MCRA_DELTA in regA
        regA.m128_f32[0] = myMCRA->MCRA_DELTA;
        regA.m128_f32[1] = myMCRA->MCRA_DELTA;
        regA.m128_f32[2] = myMCRA->MCRA_DELTA;
        regA.m128_f32[3] = myMCRA->MCRA_DELTA;

        // Load MCRA_ALPHAD in regB
        regB.m128_f32[0] = myMCRA->MCRA_ALPHAD;
        regB.m128_f32[1] = myMCRA->MCRA_ALPHAD;
        regB.m128_f32[2] = myMCRA->MCRA_ALPHAD;
        regB.m128_f32[3] = myMCRA->MCRA_ALPHAD;

        // Load 1 - MCRA_ALPHAD in regC
        regC.m128_f32[0] = 1 - myMCRA->MCRA_ALPHAD;
        regC.m128_f32[1] = 1 - myMCRA->MCRA_ALPHAD;
        regC.m128_f32[2] = 1 - myMCRA->MCRA_ALPHAD;
        regC.m128_f32[3] = 1 - myMCRA->MCRA_ALPHAD;

        for (k = 0; k < myMCRA->MICST_HALFFRAMESIZE; k+= 4)
        {

            // Load S(k,l) in regD
            regD.m128 = _mm_load_ps(&myMCRA->S[k]);

            // Load Smin(k,l) in regE
            regE.m128 = _mm_load_ps(&myMCRA->S_min[k]);

            // Load lambdaD(k,l) in regF
            regF.m128 = _mm_load_ps(&myMCRA->lambdaD[k]);

            // Load |Y(k,l)|^2 in regG
            regG.m128 = _mm_load_ps(&xPower[k]);

            // (Smin(k,l) * delta)
            regH.m128 = _mm_mul_ps(regE.m128,regA.m128);

            // S(k,l) < (Smin(k,l) * delta)
            regH.m128 = _mm_cmplt_ps(regE.m128,regH.m128);

            // lambdaD(k,l) > |Y(k,l)|^2
            regD.m128 = _mm_cmpgt_ps(regF.m128, regG.m128);

            // [S(k,l) < (Smin(k,l) * delta)] || [lambdaD(k,l) > |Y(k,l)|^2]
            regH.m128 = _mm_or_ps(regH.m128,regD.m128);

            // (1 - alphaD) * |Y(k,l)|^2
            regG.m128 = _mm_mul_ps(regC.m128,regG.m128);

            // alphaD * lambdaD(k,l)
            regD.m128 = _mm_mul_ps(regB.m128, regF.m128);

            // alphaD * lambdaD(k,l) + (1 - alphaD) * |Y(k,l)|^2
            regD.m128 = _mm_add_ps(regD.m128, regG.m128);

            // If conditions matched: lambdaD(k,l+1) = alphaD * lambdaD(k,l) + (1 - alphaD) * |Y(k,l)|^2
            // else: lambdaD(k,l+1) = lambdaD(k,l)
            regD.m128 = _mm_and_ps(regD.m128, regH.m128);
            regH.m128 = _mm_andnot_ps(regH.m128, regF.m128);
            regH.m128 = _mm_add_ps(regD.m128,regH.m128);

            // Store the noise
            _mm_store_ps(&myMCRA->lambdaD_next[k], regH.m128);

            // Store in lambdaD as well for next iteration
            //_mm_store_ps(&myMCRA->lambdaD[k], regG.m128);

        }

    }

    else
    {

        if (1.0/myMCRA->l > myMCRA->MCRA_ALPHAD)
        {

            // Load (1.0 / l) in regA
            regA.m128_f32[0] = (1.0 / myMCRA->l);
            regA.m128_f32[1] = (1.0 / myMCRA->l);
            regA.m128_f32[2] = (1.0 / myMCRA->l);
            regA.m128_f32[3] = (1.0 / myMCRA->l);

            // Load 1 - (1.0 / l) in regB
            regB.m128_f32[0] = 1 - (1.0 / myMCRA->l);
            regB.m128_f32[1] = 1 - (1.0 / myMCRA->l);
            regB.m128_f32[2] = 1 - (1.0 / myMCRA->l);
            regB.m128_f32[3] = 1 - (1.0 / myMCRA->l);

        }
        else
        {

            // Load MCRA_ALPHAD in regA
            regA.m128_f32[0] = myMCRA->MCRA_ALPHAD;
            regA.m128_f32[1] = myMCRA->MCRA_ALPHAD;
            regA.m128_f32[2] = myMCRA->MCRA_ALPHAD;
            regA.m128_f32[3] = myMCRA->MCRA_ALPHAD;

            // Load 1 - MCRA_ALPHAD in regB
            regB.m128_f32[0] = 1 - myMCRA->MCRA_ALPHAD;
            regB.m128_f32[1] = 1 - myMCRA->MCRA_ALPHAD;
            regB.m128_f32[2] = 1 - myMCRA->MCRA_ALPHAD;
            regB.m128_f32[3] = 1 - myMCRA->MCRA_ALPHAD;

        }

        for (k = 0; k < myMCRA->MICST_HALFFRAMESIZE; k+= 4)
        {

            // Load lambdaD(k,l) in regC
            regC.m128 = _mm_load_ps(&myMCRA->lambdaD[k]);

            // Load |Y(k,l)|^2 in regD
            regD.m128 = _mm_load_ps(&xPower[k]);

            // (1 - alphaD) * |Y(k,l)|^2
            regE.m128 = _mm_mul_ps(regB.m128,regD.m128);

            // alphaD * lambdaD(k,l)
            regF.m128 = _mm_mul_ps(regA.m128, regC.m128);

            // alphaD * lambdaD(k,l) + (1 - alphaD) * |Y(k,l)|^2
            regG.m128 = _mm_add_ps(regF.m128, regE.m128);

            // Store the noise
            _mm_store_ps(&myMCRA->lambdaD_next[k], regG.m128);

            // Store in lambdaD as well for next iteration
            _mm_store_ps(&myMCRA->lambdaD[k], regG.m128);

        }

    }

    // Compute the last one that has not been computed yet

    // Verify the previous conditions
    if ((myMCRA->firstProcessing == 1) || (myMCRA->S[myMCRA->MICST_HALFFRAMESIZE] < (myMCRA->MCRA_DELTA * myMCRA->S_min[myMCRA->MICST_HALFFRAMESIZE])) || (myMCRA->lambdaD[myMCRA->MICST_HALFFRAMESIZE] > xPower[myMCRA->MICST_HALFFRAMESIZE]))
    {
        if (((myMCRA->firstProcessing == 1) && ((1.0/myMCRA->l) > myMCRA->MCRA_ALPHAD)))
        {
            // lambdaD(k,l+1) = (1/l) * lambdaD(k,l) + (1 - 1/l) * |Y(k,l)|^2
            myMCRA->lambdaD_next[myMCRA->MICST_HALFFRAMESIZE] = (1.0/myMCRA->l) * myMCRA->lambdaD[myMCRA->MICST_HALFFRAMESIZE] + (1 - (1.0/myMCRA->l)) * xPower[myMCRA->MICST_HALFFRAMESIZE];
        }
        else
        {
            // lambdaD(k,l+1) = alphaD * lambdaD(k,l) + (1 - alphaD) * |Y(k,l)|^2
            myMCRA->lambdaD_next[myMCRA->MICST_HALFFRAMESIZE] = myMCRA->MCRA_ALPHAD * myMCRA->lambdaD[myMCRA->MICST_HALFFRAMESIZE] + (1 - myMCRA->MCRA_ALPHAD) * xPower[myMCRA->MICST_HALFFRAMESIZE];
        }
    }

    // Otherwise keep the same noise
    else
    {
        myMCRA->lambdaD_next[myMCRA->MICST_HALFFRAMESIZE] = myMCRA->lambdaD[myMCRA->MICST_HALFFRAMESIZE];
    }


#endif


    /***************************************************************************
     * Step 5: Update parameters                                               *
     **************************************************************************/

#ifndef USE_SIMD

    // +-----------------------------------------------------------------------+
    // | No hardware acceleration                                              |
    // +-----------------------------------------------------------------------+

    // Increment the frame counter and wrap around if needed
    (myMCRA->l)++;

    if (myMCRA->l > myMCRA->MCRA_L)
    {
        // Remove the flag for the first group of frames to be processed
        myMCRA->firstProcessing = 0;

        // Wrap around
        myMCRA->l = 1;
    }

    // Update arrays;
    for (k = 0; k < myMCRA->MICST_FRAMESIZE; k++)
    {

        myMCRA->S_min_prev[k] = myMCRA->S_min[k];
        myMCRA->S_prev[k] = myMCRA->S[k];
        myMCRA->S_tmp_prev[k] = myMCRA->S_tmp[k];
        myMCRA->lambdaD[k] = myMCRA->lambdaD_next[k];
        sigma[k] = myMCRA->lambdaD_next[k];

    }

#else

    // +-----------------------------------------------------------------------+
    // | With SSE instructions                                                 |
    // +-----------------------------------------------------------------------+

    // Increment the frame counter and wrap around if needed
    (myMCRA->l)++;

    if (myMCRA->l > myMCRA->MCRA_L)
    {
        // Remove the flag for the first group of frames to be processed
        myMCRA->firstProcessing = 0;

        // Wrap around
        myMCRA->l = 1;
    }

    // Here lambdaD is not updated since it has been updated in step 4 before
    // the shift one one was done on lambdaD_next

    // Update arrays;
    for (k = 0; k < myMCRA->MICST_HALFFRAMESIZE; k+= 4)
    {

        // Load Smin(k,l) in regA
        regA.m128 = _mm_load_ps(&myMCRA->S_min[k]);
        // Store regA in Smin(k,l-1)
        _mm_store_ps(&myMCRA->S_min_prev[k], regA.m128);

        // Load S(k,l) in regA
        regA.m128 = _mm_load_ps(&myMCRA->S[k]);
        // Store regA in  S(k,l-1)
        _mm_store_ps(&myMCRA->S_prev[k], regA.m128);

        // Load Stmp(k,l) in regA
        regA.m128 = _mm_load_ps(&myMCRA->S_tmp[k]);
        // Store regA in Stmp(k,l-1)
        _mm_store_ps(&myMCRA->S_tmp_prev[k], regA.m128);

        // Load lambdaD(k,l+1) in regA
        regA.m128 = _mm_load_ps(&myMCRA->lambdaD_next[k]);
        // Store regA in lambdaD(k,l)
        _mm_store_ps(&myMCRA->lambdaD[k], regA.m128);

    }

    myMCRA->S_min_prev[myMCRA->MICST_HALFFRAMESIZE] = myMCRA->S_min[myMCRA->MICST_HALFFRAMESIZE];
    myMCRA->S_prev[myMCRA->MICST_HALFFRAMESIZE] = myMCRA->S[myMCRA->MICST_HALFFRAMESIZE];
    myMCRA->S_tmp_prev[myMCRA->MICST_HALFFRAMESIZE] = myMCRA->S_tmp[myMCRA->MICST_HALFFRAMESIZE];
    myMCRA->lambdaD[myMCRA->MICST_HALFFRAMESIZE] = myMCRA->lambdaD_next[myMCRA->MICST_HALFFRAMESIZE];

    sigma[0] = myMCRA->lambdaD_next[0];
    sigma[myMCRA->MICST_HALFFRAMESIZE] = myMCRA->lambdaD_next[myMCRA->MICST_HALFFRAMESIZE];

    for (k = 1; k < myMCRA->MICST_HALFFRAMESIZE; k++)
    {
        sigma[k] = myMCRA->lambdaD_next[k];
        sigma[myMCRA->MICST_FRAMESIZE - k] = sigma[k];
    }

#endif

}
