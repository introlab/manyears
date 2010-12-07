/******************************************************************************* 
 * ManyEars: Microphone Sound Track - Source Code                              *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Author: Francois Grondin                                                    *
 * Original Code: Jean-Marc Valin                                              *
 * Modified Code: Simon Briere                                                 *
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

#include "Preprocessing/micst.h"

/*******************************************************************************
 * micstInitNormal                                                             *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myMicST         The micST object to be initialized             *
 *              myParameters    List of parameters that is used to configure   *
 *                              the object                                     *
 *              position        The microphone position on the cube            *
 *                              (must be an array made of 3 elements, where    *
 *                              1st element = x, 2nd = y and 3rd = z)          *
 *              gain            Gain of the microphone                         *
 *                                                                             *
 * Outputs:     myMicST         The micST initialized object                   *
 *                                                                             *
 * Description: This function initializes the micST object                     *
 *                                                                             *
 ******************************************************************************/

void micstInit(struct objMicST *myMicST, struct ParametersStruct *myParameters, float *position, float gain)
{

    // Index to fill the arrays
    unsigned int k;

    // *************************************************************************
    // * STEP 1: Load parameters                                               *
    // *************************************************************************

    myMicST->MICST_ALPHAD = myParameters->P_MICST_ALPHAD;
    myMicST->MICST_C = GLOBAL_C;
    myMicST->MICST_DELTA = myParameters->P_MICST_DELTA;
    myMicST->MICST_FRAMESIZE = GLOBAL_FRAMESIZE;
    myMicST->MICST_HALFFRAMESIZE = myMicST->MICST_FRAMESIZE / 2;
    myMicST->MICST_FS = GLOBAL_FS;
    myMicST->MICST_GAMMA = myParameters->P_MICST_GAMMA;
    myMicST->MICST_OVERLAP = GLOBAL_OVERLAP;

    // *************************************************************************
    // * STEP 2: Initialize context                                            *
    // *************************************************************************

    myMicST->gain = gain;
    myMicST->position[0] = position[0];
    myMicST->position[1] = position[1];
    myMicST->position[2] = position[2];

    myMicST->ksi = (float*) newTable1D(myMicST->MICST_FRAMESIZE,sizeof(float));
    myMicST->lambda = (float*) newTable1D(myMicST->MICST_FRAMESIZE,sizeof(float));
    myMicST->lambda_prev = (float*) newTable1D(myMicST->MICST_FRAMESIZE,sizeof(float));
    myMicST->sigma = (float*) newTable1D(myMicST->MICST_FRAMESIZE,sizeof(float));
    myMicST->weightedResultImag = (float*) newTable1D(myMicST->MICST_FRAMESIZE,sizeof(float));
    myMicST->weightedResultReal = (float*) newTable1D(myMicST->MICST_FRAMESIZE,sizeof(float));
    myMicST->xfreqImag = (float*) newTable1D(myMicST->MICST_FRAMESIZE,sizeof(float));
    myMicST->xfreqReal = (float*) newTable1D(myMicST->MICST_FRAMESIZE,sizeof(float));
    myMicST->xpower = (float*) newTable1D(myMicST->MICST_FRAMESIZE,sizeof(float));
    myMicST->xpower_prev = (float*) newTable1D(myMicST->MICST_FRAMESIZE,sizeof(float));
    myMicST->xtime = (float*) newTable1D(myMicST->MICST_FRAMESIZE,sizeof(float));
    myMicST->xtime_windowed = (float*) newTable1D(myMicST->MICST_FRAMESIZE,sizeof(float));
    myMicST->zeta = (float*) newTable1D(myMicST->MICST_FRAMESIZE,sizeof(float));
    myMicST->zeta_prev = (float*) newTable1D(myMicST->MICST_FRAMESIZE,sizeof(float));

    myMicST->noiseEstimator = (struct objMcra*) malloc(sizeof(struct objMcra));

    // *************************************************************************
    // * STEP 3: Initialize the arrays                                         *
    // *************************************************************************

    for (k = 0; k < myMicST->MICST_FRAMESIZE; k++)
    {

        myMicST->ksi[k] = 0;
        myMicST->lambda[k] = 0;
        myMicST->lambda_prev[k] = 0;
        myMicST->sigma[k] = 0;
        myMicST->xfreqImag[k] = 0;
        myMicST->xfreqReal[k] = 0;
        myMicST->xpower[k] = 0;
        myMicST->xpower_prev[k] = 0;
        myMicST->xtime[k] = 0;
        myMicST->xtime_windowed[k] = 0;
        myMicST->zeta[k] = 0;
        myMicST->zeta_prev[k] = 0;
        myMicST->weightedResultImag[k] = 0;
        myMicST->weightedResultReal[k] = 0;

    }

    mcraInit(myMicST->noiseEstimator, myParameters, myMicST->MICST_FRAMESIZE);

}

/*******************************************************************************
 * micstTerminate                                                              *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myMicST         The micst object to be terminated              *
 *                                                                             *
 * Outputs:     (none)                                                         *
 *                                                                             *
 * Description: This function terminates the micst object                      *
 *                                                                             *
 ******************************************************************************/

void micstTerminate(struct objMicST *myMicST)
{

    // Free memory
    deleteTable1D((void*) myMicST->ksi);
    deleteTable1D((void*) myMicST->lambda);
    deleteTable1D((void*) myMicST->lambda_prev);
    deleteTable1D((void*) myMicST->sigma);
    deleteTable1D((void*) myMicST->weightedResultImag);
    deleteTable1D((void*) myMicST->weightedResultReal);
    deleteTable1D((void*) myMicST->xfreqImag);
    deleteTable1D((void*) myMicST->xfreqReal);
    deleteTable1D((void*) myMicST->xpower);
    deleteTable1D((void*) myMicST->xpower_prev);
    deleteTable1D((void*) myMicST->xtime);
    deleteTable1D((void*) myMicST->xtime_windowed);
    deleteTable1D((void*) myMicST->zeta);
    deleteTable1D((void*) myMicST->zeta_prev);

    mcraTerminate(myMicST->noiseEstimator);
    free(myMicST->noiseEstimator);

}

/*******************************************************************************
 * micstProcessFrame		                                               *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myMicST         The micST object to use to process the frame   *
 *                                                                             *
 * Outputs:     myMicST         The updatd micST object                        *
 *                                                                             *
 * Description: This function processes the new frame and returns the          *
 *              cross correlation terms to be used by the beamformer           *
 *                                                                             *
 ******************************************************************************/

void micstProcessFrame(struct objMicST *myMicST)
{

    // Index to go through each array
    unsigned int k;

    // Temporary variable to store the square root of Xpower[k] for a given k
    float tmpSqrt;

    // Temporary variable to store the posteriori information
    float post;

#ifdef USE_SIMD
    // SIMD registers
    __m128_mod regA, regB, regC, regD, regE, regF, regG, regH;
#endif

    /***************************************************************************
     * Step 0: Set DC to zero                                                  *
     **************************************************************************/

    myMicST->xfreqReal[0] = 0.0f;
    myMicST->xfreqImag[0] = 0.0f;

    /***************************************************************************
     * Step 1: Compute the power                                               *
     **************************************************************************/

#ifndef USE_SIMD

    // +-----------------------------------------------------------------------+
    // | No hardware acceleration                                              |
    // +-----------------------------------------------------------------------+

    myMicST->xpower[0] = myMicST->xfreqReal[0] * myMicST->xfreqReal[0] + myMicST->xfreqImag[0] * myMicST->xfreqImag[0];
    myMicST->xpower[myMicST->MICST_HALFFRAMESIZE] = myMicST->xfreqReal[myMicST->MICST_HALFFRAMESIZE] * myMicST->xfreqReal[myMicST->MICST_HALFFRAMESIZE] + myMicST->xfreqImag[myMicST->MICST_HALFFRAMESIZE] * myMicST->xfreqImag[myMicST->MICST_HALFFRAMESIZE];

    for (k = 1; k < myMicST->MICST_HALFFRAMESIZE; k++)
    {

        myMicST->xpower[k] = myMicST->xfreqReal[k] * myMicST->xfreqReal[k] + myMicST->xfreqImag[k] * myMicST->xfreqImag[k];
        myMicST->xpower[myMicST->MICST_FRAMESIZE - k] = myMicST->xpower[k];

    }

#else

    // +-----------------------------------------------------------------------+
    // | With SSE instructions                                                 |
    // +-----------------------------------------------------------------------+

    for (k = 0; k < myMicST->MICST_FRAMESIZE; k+=4)
    {
        // Load xfreqReal in regA
        regA.m128 = _mm_load_ps(&myMicST->xfreqReal[k]);
        // Load xfreqImag in regB
        regB.m128 = _mm_load_ps(&myMicST->xfreqImag[k]);

        // xfreqReal * xfreqReal
        regC.m128 = _mm_mul_ps(regA.m128,regA.m128);
        // xfreqImag * xfreqImag
        regD.m128 = _mm_mul_ps(regB.m128,regB.m128);

        // xfreqReal * xfreqReal + xfreqImag * xfreqImag
        regE.m128 = _mm_add_ps(regC.m128,regD.m128);

        // Store in xpower
        _mm_store_ps(&myMicST->xpower[k],regE.m128);

    }

#endif

    /***************************************************************************
     * Step 2: Compute noise with MCRA                                         *
     **************************************************************************/

    // sigma_2_i[k]
    mcraProcessFrame(myMicST->noiseEstimator, myMicST->xpower, myMicST->sigma);

    /***************************************************************************
     * Step 3: Compute the simple reverberation model                          *
     **************************************************************************/

#ifndef USE_SIMD

    // +-----------------------------------------------------------------------+
    // | No hardware acceleration                                              |
    // +-----------------------------------------------------------------------+

    // lambda_rev_n,i[k] = gamma * lambda_rev_n-1,i[k] + (1 - gamma) * delta * | zeta_n-1_i[k] * X_n-1_i[k] |^2

    myMicST->lambda[0] = myMicST->MICST_GAMMA * myMicST->lambda_prev[0] + (1.0f - myMicST->MICST_GAMMA) * myMicST->MICST_DELTA * fabsf(myMicST->zeta_prev[0]) * fabsf(myMicST->zeta_prev[0]) * myMicST->xpower_prev[0];
    myMicST->lambda[myMicST->MICST_HALFFRAMESIZE] = myMicST->MICST_GAMMA * myMicST->lambda_prev[myMicST->MICST_HALFFRAMESIZE] + (1.0f - myMicST->MICST_GAMMA) * myMicST->MICST_DELTA * fabsf(myMicST->zeta_prev[myMicST->MICST_HALFFRAMESIZE]) * fabsf(myMicST->zeta_prev[myMicST->MICST_HALFFRAMESIZE]) * myMicST->xpower_prev[myMicST->MICST_HALFFRAMESIZE];

    for (k = 1; k < myMicST->MICST_HALFFRAMESIZE; k++)
    {

        myMicST->lambda[k] = myMicST->MICST_GAMMA * myMicST->lambda_prev[k] + (1.0f - myMicST->MICST_GAMMA) * myMicST->MICST_DELTA * fabsf(myMicST->zeta_prev[k]) * fabsf(myMicST->zeta_prev[k]) * myMicST->xpower_prev[k];
        myMicST->lambda[myMicST->MICST_FRAMESIZE - k] = myMicST->lambda[k];

    }

#else

    // +-----------------------------------------------------------------------+
    // | With SSE instructions                                                 |
    // +-----------------------------------------------------------------------+

    // lambda_rev_n,i[k] = gamma * lambda_rev_n-1,i[k] + (1 - gamma) * delta * | zeta_n-1_i[k] * X_n-1_i[k] |^2

    // Load the constant gamma in regA
    regA.m128_f32[0] = myMicST->MICST_GAMMA;
    regA.m128_f32[1] = myMicST->MICST_GAMMA;
    regA.m128_f32[2] = myMicST->MICST_GAMMA;
    regA.m128_f32[3] = myMicST->MICST_GAMMA;

    // Load the constant (1-gamma)*delta in regB
    regB.m128_f32[0] = (1 - myMicST->MICST_GAMMA) * myMicST->MICST_DELTA;
    regB.m128_f32[1] = (1 - myMicST->MICST_GAMMA) * myMicST->MICST_DELTA;
    regB.m128_f32[2] = (1 - myMicST->MICST_GAMMA) * myMicST->MICST_DELTA;
    regB.m128_f32[3] = (1 - myMicST->MICST_GAMMA) * myMicST->MICST_DELTA;

    for (k = 0; k < myMicST->MICST_HALFFRAMESIZE; k+=4)
    {
        // Load lambda_rev_n-1,i[k] in regC
        regC.m128 = _mm_load_ps(&myMicST->lambda_prev[k]);
        // Load zeta_n-1_i[k] in regD
        regD.m128 = _mm_load_ps(&myMicST->zeta_prev[k]);
        // Load |X_n-1_i[k]|^2 in regE
        regE.m128 = _mm_load_ps(&myMicST->xpower_prev[k]);

        // gamma * lambda_rev_n-1,i[k]
        regF.m128 = _mm_mul_ps(regA.m128,regC.m128);

        // |zeta_n-1_i[k]|^2
        regD.m128 = _mm_mul_ps(regD.m128,regD.m128);

        // |zeta_n-1_i[k]|^2 * |X_n-1_i[k]|^2
        regD.m128 = _mm_mul_ps(regD.m128,regE.m128);

        // (1 - gamma) * delta * |zeta_n-1_i[k]|^2 * |X_n-1_i[k]|^2
        regD.m128 = _mm_mul_ps(regB.m128,regD.m128);

        // gamma * lambda_rev_n-1,i[k] + (1 - gamma) * delta * |zeta_n-1_i[k]|^2 * |X_n-1_i[k]|^2
        regD.m128 = _mm_add_ps(regF.m128,regD.m128);

        // Store result
        _mm_store_ps(&myMicST->lambda[k],regD.m128);

    }

    myMicST->lambda[myMicST->MICST_HALFFRAMESIZE] = myMicST->MICST_GAMMA * myMicST->lambda_prev[myMicST->MICST_HALFFRAMESIZE] + (1 - myMicST->MICST_GAMMA) * myMicST->MICST_DELTA * fabs(myMicST->zeta_prev[myMicST->MICST_HALFFRAMESIZE]) * fabs(myMicST->zeta_prev[myMicST->MICST_HALFFRAMESIZE]) * myMicST->xpower_prev[myMicST->MICST_HALFFRAMESIZE];

#endif

    /***************************************************************************
     * Step 4: Compute the estimate of the a priori SNR                        *
     **************************************************************************/

#ifndef USE_SIMD

    // +-----------------------------------------------------------------------+
    // | No hardware acceleration                                              |
    // +-----------------------------------------------------------------------+

    // post[k] = | X_n_i[k] |^2 - sigma_2_i[k] - lambda_rev_n,i[k]
    // ksi_n_i[k] = {(1 - alphaD) * [ zeta_n-1_i[k] ]^2 * | X_n-1_i[k] |^2 + alphaD * post[k]} / (sigma_2_i[k] + lambda_rev_n,i[k] + 1E-20)

    post = myMicST->xpower[0] - myMicST->sigma[0] - myMicST->lambda[0];
    if (post < 0.0f)
    {
        post = 0.0f;
    }
    myMicST->ksi[0] = ((1 - myMicST->MICST_ALPHAD) * fabsf(myMicST->zeta_prev[0]) * fabsf(myMicST->zeta_prev[0]) * myMicST->xpower_prev[0] + myMicST->MICST_ALPHAD * post) / (myMicST->sigma[0] + myMicST->lambda[0] + 1E-20f);

    post = myMicST->xpower[myMicST->MICST_HALFFRAMESIZE] - myMicST->sigma[myMicST->MICST_HALFFRAMESIZE] - myMicST->lambda[myMicST->MICST_HALFFRAMESIZE];
    if (post < 0.0f)
    {
        post = 0.0f;
    }
    myMicST->ksi[myMicST->MICST_HALFFRAMESIZE] = ((1 - myMicST->MICST_ALPHAD) * fabsf(myMicST->zeta_prev[myMicST->MICST_HALFFRAMESIZE]) * fabsf(myMicST->zeta_prev[myMicST->MICST_HALFFRAMESIZE]) * myMicST->xpower_prev[myMicST->MICST_HALFFRAMESIZE] + myMicST->MICST_ALPHAD * post) / (myMicST->sigma[myMicST->MICST_HALFFRAMESIZE] + myMicST->lambda[myMicST->MICST_HALFFRAMESIZE] + 1E-20f);

    for (k = 1; k < myMicST->MICST_HALFFRAMESIZE; k++)
    {
        post = myMicST->xpower[k] - myMicST->sigma[k] - myMicST->lambda[k];
        if (post < 0.0f)
        {
            post = 0.0f;
        }
        myMicST->ksi[k] = ((1 - myMicST->MICST_ALPHAD) * fabsf(myMicST->zeta_prev[k]) * fabsf(myMicST->zeta_prev[k]) * myMicST->xpower_prev[k] + myMicST->MICST_ALPHAD * post) / (myMicST->sigma[k] + myMicST->lambda[k] + 1E-20f);

        myMicST->ksi[myMicST->MICST_FRAMESIZE - k] = myMicST->ksi[k];
    }

#else

    // +-----------------------------------------------------------------------+
    // | With SSE instructions                                                 |
    // +-----------------------------------------------------------------------+

    // post[k] = | X_n_i[k] |^2 - sigma_2_i[k] - lambda_rev_n,i[k]
    // ksi_n_i[k] = {(1 - alphaD) * [ zeta_n-1_i[k] ]^2 * | X_n-1_i[k] |^2 + alphaD * post[k]} / (sigma_2_i[k] + lambda_rev_n,i[k] + 1E-20)

    // Load the constant 1 - alphaD in regB
    regB.m128_f32[0] = 1 - myMicST->MICST_ALPHAD;
    regB.m128_f32[1] = 1 - myMicST->MICST_ALPHAD;
    regB.m128_f32[2] = 1 - myMicST->MICST_ALPHAD;
    regB.m128_f32[3] = 1 - myMicST->MICST_ALPHAD;

    // Load the constant 1E-20 in regC
    regC.m128_f32[0] = 1E-20;
    regC.m128_f32[1] = 1E-20;
    regC.m128_f32[2] = 1E-20;
    regC.m128_f32[3] = 1E-20;

    for (k = 0; k < myMicST->MICST_HALFFRAMESIZE; k+=4)
    {

        // Load the constant alphaD in regA
        regA.m128_f32[0] = myMicST->MICST_ALPHAD;
        regA.m128_f32[1] = myMicST->MICST_ALPHAD;
        regA.m128_f32[2] = myMicST->MICST_ALPHAD;
        regA.m128_f32[3] = myMicST->MICST_ALPHAD;

        // Load zeta_n-1_i[k] in regD
        regD.m128 = _mm_load_ps(&myMicST->zeta_prev[k]);
        // Load | X_n-1_i[k] |^2 in regE
        regE.m128 = _mm_load_ps(&myMicST->xpower_prev[k]);
        // Load | X_n_i[k] |^2 in regF
        regF.m128 = _mm_load_ps(&myMicST->xpower[k]);
        // Load sigma_2_i[k] in regG
        regG.m128 = _mm_load_ps(&myMicST->sigma[k]);
        // Load lambda_rev_n,i[k] in regH
        regH.m128 = _mm_load_ps(&myMicST->lambda[k]);

        // [ zeta_n-1_i[k] ]^2
        regD.m128 = _mm_mul_ps(regD.m128,regD.m128);

        // [ zeta_n-1_i[k] ]^2 * | X_n-1_i[k] |^2
        regD.m128 = _mm_mul_ps(regD.m128, regE.m128);

        // (1 - alphaD) * [ zeta_n-1_i[k] ]^2 * | X_n-1_i[k] |^2
        regD.m128 = _mm_mul_ps(regB.m128,regD.m128);

        // | X_n_i[k] |^2 - sigma_2_i[k]
        regF.m128 = _mm_sub_ps(regF.m128, regG.m128);

        // | X_n_i[k] |^2 - sigma_2_i[k] - lambda_rev_n,i[k]
        regF.m128 = _mm_sub_ps(regF.m128, regH.m128);

        // alphaD * (| X_n_i[k] |^2 - sigma_2_i[k] - lambda_rev_n,i[k])
        regF.m128 = _mm_mul_ps(regA.m128, regF.m128);

        // Load the constant 0 in regA
        regA.m128_f32[0] = 0;
        regA.m128_f32[1] = 0;
        regA.m128_f32[2] = 0;
        regA.m128_f32[3] = 0;

        // If alphaD * (| X_n_i[k] |^2 - sigma_2_i[k] - lambda_rev_n,i[k]) < 0
        // then = 0
        regA.m128 = _mm_cmpgt_ps(regF.m128, regA.m128);
        regF.m128 = _mm_and_ps(regF.m128, regA.m128);

        // (1 - alphaD) * [ zeta_n-1_i[k] ]^2 * | X_n-1_i[k] |^2 + alphaD * (| X_n_i[k] |^2 - sigma_2_i[k] - lambda_rev_n,i[k])
        regF.m128 = _mm_add_ps(regF.m128, regD.m128);

        // sigma_2_i[k] + lambda_rev_n,i[k]
        regH.m128 = _mm_add_ps(regG.m128, regH.m128);

        // sigma_2_i[k] + lambda_rev_n,i[k] + 1E-20
        regH.m128 = _mm_add_ps(regC.m128, regH.m128);

        // {(1 - alphaD) * [ zeta_n-1_i[k] ]^2 * | X_n-1_i[k] |^2 + alphaD * (| X_n_i[k] |^2 - sigma_2_i[k] - lambda_rev_n,i[k])} / (sigma_2_i[k] + lambda_rev_n,i[k] + 1E-20)
        regH.m128 = _mm_div_ps(regF.m128, regH.m128);

        // Store result in ksi_n_i[k]
        _mm_store_ps(&myMicST->ksi[k], regH.m128);

    }

    post = myMicST->xpower[myMicST->MICST_HALFFRAMESIZE] - myMicST->sigma[myMicST->MICST_HALFFRAMESIZE] - myMicST->lambda[myMicST->MICST_HALFFRAMESIZE];
    if (post < 0.0)
    {
        post = 0.0;
    }
    myMicST->ksi[myMicST->MICST_HALFFRAMESIZE] = ((1 - myMicST->MICST_ALPHAD) * fabs(myMicST->zeta_prev[myMicST->MICST_HALFFRAMESIZE]) * fabs(myMicST->zeta_prev[myMicST->MICST_HALFFRAMESIZE]) * myMicST->xpower_prev[myMicST->MICST_HALFFRAMESIZE] + myMicST->MICST_ALPHAD * post) / (myMicST->sigma[myMicST->MICST_HALFFRAMESIZE] + myMicST->lambda[myMicST->MICST_HALFFRAMESIZE] + 1E-20);

#endif

    /***************************************************************************
     * Step 5: Compute the weighting function                                  *
     **************************************************************************/

#ifndef USE_SIMD

    // +-----------------------------------------------------------------------+
    // | No hardware acceleration                                              |
    // +-----------------------------------------------------------------------+

    // zeta_n_i[k] = ksi_n_i[k] / (ksi_n_i[k] + 1)

    myMicST->zeta[0] = myMicST->ksi[0] / (myMicST->ksi[0] + 1.0f);
    myMicST->zeta[myMicST->MICST_HALFFRAMESIZE] = myMicST->ksi[myMicST->MICST_HALFFRAMESIZE] / (myMicST->ksi[myMicST->MICST_HALFFRAMESIZE] + 1.0f);

    for (k = 1; k < myMicST->MICST_HALFFRAMESIZE; k++)
    {

        myMicST->zeta[k] = myMicST->ksi[k] / (myMicST->ksi[k] + 1.0f);
        myMicST->zeta[myMicST->MICST_FRAMESIZE - k] = myMicST->zeta[k];

    }

#else

    // +-----------------------------------------------------------------------+
    // | With SSE instructions                                                 |
    // +-----------------------------------------------------------------------+

    // zeta_n_i[k] = ksi_n_i[k] / (ksi_n_i[k] + 1)

    // Load the constant 1 in regA
    regA.m128_f32[0] = 1;
    regA.m128_f32[1] = 1;
    regA.m128_f32[2] = 1;
    regA.m128_f32[3] = 1;

    for (k = 0; k < myMicST->MICST_HALFFRAMESIZE; k+=4)
    {
        // Load ksi_n_i[k] in regB
        regB.m128 = _mm_load_ps(&myMicST->ksi[k]);

        // ksi_n_i[k] + 1
        regC.m128 = _mm_add_ps(regB.m128, regA.m128);

        // ksi_n_i[k] / (ksi_n_i[k] + 1)
        regD.m128 = _mm_div_ps(regB.m128,regC.m128);

        // Store in zeta_n_i[k]
        _mm_store_ps(&myMicST->zeta[k], regD.m128);

    }

    myMicST->zeta[myMicST->MICST_HALFFRAMESIZE] = myMicST->ksi[myMicST->MICST_HALFFRAMESIZE] / (myMicST->ksi[myMicST->MICST_HALFFRAMESIZE] + 1);

#endif

    /***************************************************************************
     * Step 6: Compute the weighted spectrum                                   *
     **************************************************************************/

#ifndef USE_SIMD

    // +-----------------------------------------------------------------------+
    // | No hardware acceleration                                              |
    // +-----------------------------------------------------------------------+

    // zeta_i[k] * X_i[k] / | X_i[k] |

    tmpSqrt = sqrtf(myMicST->xpower[0]);

    myMicST->weightedResultReal[0] = myMicST->zeta[0] * myMicST->xfreqReal[0] / (tmpSqrt + 1E-20f);
    myMicST->weightedResultImag[0] = myMicST->zeta[0] * myMicST->xfreqImag[0] / (tmpSqrt + 1E-20f);


    tmpSqrt = sqrtf(myMicST->xpower[myMicST->MICST_HALFFRAMESIZE]);

    myMicST->weightedResultReal[myMicST->MICST_HALFFRAMESIZE] = myMicST->zeta[myMicST->MICST_HALFFRAMESIZE] * myMicST->xfreqReal[myMicST->MICST_HALFFRAMESIZE] / (tmpSqrt + 1E-20f);
    myMicST->weightedResultImag[myMicST->MICST_HALFFRAMESIZE] = myMicST->zeta[myMicST->MICST_HALFFRAMESIZE] * myMicST->xfreqImag[myMicST->MICST_HALFFRAMESIZE] / (tmpSqrt + 1E-20f);

    for (k = 1; k < myMicST->MICST_HALFFRAMESIZE; k++)
    {
        tmpSqrt = sqrtf(myMicST->xpower[k]);

        myMicST->weightedResultReal[k] = myMicST->zeta[k] * myMicST->xfreqReal[k] / (tmpSqrt + 1E-20f);
        myMicST->weightedResultImag[k] = myMicST->zeta[k] * myMicST->xfreqImag[k] / (tmpSqrt + 1E-20f);

        myMicST->weightedResultReal[myMicST->MICST_FRAMESIZE - k] = myMicST->weightedResultReal[k];
        myMicST->weightedResultImag[myMicST->MICST_FRAMESIZE - k] = -1.0f * myMicST->weightedResultImag[k];

    }

#else

    // +-----------------------------------------------------------------------+
    // | With SSE instructions                                                 |
    // +-----------------------------------------------------------------------+

    // | zeta_i[k] | / | X_i[k] |

    // Load the constant 1E-20 in regA
    regA.m128_f32[0] = 1E-20;
    regA.m128_f32[1] = 1E-20;
    regA.m128_f32[2] = 1E-20;
    regA.m128_f32[3] = 1E-20;

    for (k = 0; k < myMicST->MICST_HALFFRAMESIZE; k+=4)
    {
        // Load zeta_i[k] in regC
        regC.m128 = _mm_load_ps(&myMicST->zeta[k]);
        // Load | X_i[k] |^2 in regD
        regD.m128 = _mm_load_ps(&myMicST->xpower[k]);
        // Load Re{X_i[k]} in regG
        regG.m128 = _mm_load_ps(&myMicST->xfreqReal[k]);
        // Load Im{X_i[k]} in regH
        regH.m128 = _mm_load_ps(&myMicST->xfreqImag[k]);

        // | X_i[k] |
        regE.m128 = _mm_sqrt_ps(regD.m128);
        regE.m128 = _mm_add_ps(regE.m128,regA.m128);

        // zeta_i[k] / | X_i[k] |
        regF.m128 = _mm_div_ps(regC.m128,regE.m128);

        // zeta_i[k] * X_i[k] / | X_i[k] |
        regG.m128 = _mm_mul_ps(regG.m128,regF.m128);
        regH.m128 = _mm_mul_ps(regH.m128,regF.m128);

        // Store in weightedResult
        _mm_store_ps(&myMicST->weightedResultReal[k],regG.m128);
        _mm_store_ps(&myMicST->weightedResultImag[k],regH.m128);

    }

    tmpSqrt = sqrt(myMicST->xpower[myMicST->MICST_HALFFRAMESIZE]);

    myMicST->weightedResultReal[myMicST->MICST_HALFFRAMESIZE] = myMicST->zeta[myMicST->MICST_HALFFRAMESIZE] * myMicST->xfreqReal[myMicST->MICST_HALFFRAMESIZE] / (tmpSqrt + 1E-20);
    myMicST->weightedResultImag[myMicST->MICST_HALFFRAMESIZE] = myMicST->zeta[myMicST->MICST_HALFFRAMESIZE] * myMicST->xfreqImag[myMicST->MICST_HALFFRAMESIZE] / (tmpSqrt + 1E-20);

    for (k = 1; k < myMicST->MICST_HALFFRAMESIZE; k++)
    {
        myMicST->weightedResultReal[myMicST->MICST_FRAMESIZE - k] = myMicST->weightedResultReal[k];
        myMicST->weightedResultImag[myMicST->MICST_FRAMESIZE - k] = -1.0 * myMicST->weightedResultImag[k];
    }

#endif


    /***************************************************************************
     * Step 7: Update parameters                                               *
     **************************************************************************/

    for (k = 0; k < myMicST->MICST_FRAMESIZE; k++)
    {
        myMicST->lambda_prev[k] = myMicST->lambda[k];
        myMicST->xpower_prev[k] = myMicST->xpower[k];
        myMicST->zeta_prev[k] = myMicST->zeta[k];
    }

}
