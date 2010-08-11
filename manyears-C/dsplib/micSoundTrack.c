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
 ******************************************************************************/

#include "stdio.h"

#include <math.h>
#ifdef USE_SIMD
#include "fft.h"
#include <xmmintrin.h>
#endif

#include "micSoundTrack.h"
#include "hardware.h"

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

void mcraInit(struct objMcra *myMCRA, struct ParametersStruct *myParameters)
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

    myMCRA->MICST_FRAMESIZE = GLOBAL_FRAMESIZE;
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

        for (indexB = -1 * ((myMCRA->MCRA_BSIZE-1)/2); indexB <= ((myMCRA->MCRA_BSIZE-1)/2); indexB++)
        {
            // Check if the index is out of bound
            if ( (((signed int) k + indexB) > 0) && (((signed int) k + indexB) < myMCRA->MICST_FRAMESIZE) )
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

    myMCRA->S[0] = myMCRA->MCRA_ALPHAS * myMCRA->S_prev[0] + (1 - myMCRA->MCRA_ALPHAS) * myMCRA->Sf[0];
    myMCRA->S[myMCRA->MICST_HALFFRAMESIZE] = myMCRA->MCRA_ALPHAS * myMCRA->S_prev[myMCRA->MICST_HALFFRAMESIZE] + (1 - myMCRA->MCRA_ALPHAS) * myMCRA->Sf[myMCRA->MICST_HALFFRAMESIZE];

    for (k = 1; k < myMCRA->MICST_HALFFRAMESIZE; k++)
    {

        myMCRA->S[k] = myMCRA->MCRA_ALPHAS * myMCRA->S_prev[k] + (1 - myMCRA->MCRA_ALPHAS) * myMCRA->Sf[k];
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
        myMCRA->S_min[myMCRA->MICST_HALFFRAMESIZE2] = (myMCRA->S_tmp_prev[myMCRA->MICST_HALFFRAMESIZE2] <= myMCRA->S[myMCRA->MICST_HALFFRAMESIZE2]) * myMCRA->S_tmp_prev[myMCRA->MICST_HALFFRAMESIZE2] + (myMCRA->S_tmp_prev[myMCRA->MICST_HALFFRAMESIZE2] > myMCRA->S[myMCRA->MICST_HALFFRAMESIZE2]) * myMCRA->S[myMCRA->MICST_HALFFRAMESIZE2];
        myMCRA->S_tmp[myMCRA->MICST_HALFFRAMESIZE2] = myMCRA->S[myMCRA->MICST_HALFFRAMESIZE2];

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
                myMCRA->lambdaD_next[k] = (1.0/myMCRA->l) * myMCRA->lambdaD[k] + (1 - (1.0/myMCRA->l)) * xPower[k];

            }
            else
            {

                // lambdaD(k,l+1) = alphaD * lambdaD(k,l) + (1 - alphaD) * |Y(k,l)|^2
                myMCRA->lambdaD_next[k] = myMCRA->MCRA_ALPHAD * myMCRA->lambdaD[k] + (1 - myMCRA->MCRA_ALPHAD) * xPower[k];

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

    myMicST->ksi = newTable1D(myMicST->MICST_FRAMESIZE,sizeof(float));
    myMicST->lambda = newTable1D(myMicST->MICST_FRAMESIZE,sizeof(float));
    myMicST->lambda_prev = newTable1D(myMicST->MICST_FRAMESIZE,sizeof(float));
    myMicST->sigma = newTable1D(myMicST->MICST_FRAMESIZE,sizeof(float));
    myMicST->weightedResultImag = newTable1D(myMicST->MICST_FRAMESIZE,sizeof(float));
    myMicST->weightedResultReal = newTable1D(myMicST->MICST_FRAMESIZE,sizeof(float));
    myMicST->xfreqImag = newTable1D(myMicST->MICST_FRAMESIZE,sizeof(float));
    myMicST->xfreqReal = newTable1D(myMicST->MICST_FRAMESIZE,sizeof(float));
    myMicST->xpower = newTable1D(myMicST->MICST_FRAMESIZE,sizeof(float));
    myMicST->xpower_prev = newTable1D(myMicST->MICST_FRAMESIZE,sizeof(float));
    myMicST->xtime = newTable1D(myMicST->MICST_FRAMESIZE,sizeof(float));
    myMicST->xtime_windowed = newTable1D(myMicST->MICST_FRAMESIZE,sizeof(float));
    myMicST->zeta = newTable1D(myMicST->MICST_FRAMESIZE,sizeof(float));
    myMicST->zeta_prev = newTable1D(myMicST->MICST_FRAMESIZE,sizeof(float));

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

    mcraInit(myMicST->noiseEstimator, myParameters);

}

/*******************************************************************************
 * mcraTerminate                                                               *
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

    myMicST->xfreqReal[0] = 0;
    myMicST->xfreqImag[0] = 0;

    /***************************************************************************
     * Step 1: Compute the power                                               *
     **************************************************************************/

#ifndef USE_SIMD

    // +-----------------------------------------------------------------------+
    // | No hardware acceleration                                              |
    // +-----------------------------------------------------------------------+

    myMicST->xpower[0] = myMicST->xfreqReal[0] * myMicST->xfreqReal[0] + myMicST->xfreqImag[0] * myMicST->xfreqImag[0];
    myMicST->xpower[myMicST->MICST_HALFFRAMESIZE2] = myMicST->xfreqReal[myMicST->MICST_HALFFRAMESIZE2] * myMicST->xfreqReal[myMicST->MICST_HALFFRAMESIZE2] + myMicST->xfreqImag[myMicST->MICST_HALFFRAMESIZE2] * myMicST->xfreqImag[myMicST->MICST_HALFFRAMESIZE2];

    for (k = 1; k < myMicST->MICST_HALFFRAMESIZE2; k++)
    {
        myMicST->xpower[k] = myMicST->xfreqReal[k] * myMicST->xfreqReal[k] + myMicST->xfreqImag[k] * myMicST->xfreqImag[k];
        myMicST->xpower[myMicST->MICST_FRAMESIZE2 - k] = myMicST->xpower[k];
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

    myMicST->lambda[0] = myMicST->MICST_GAMMA2 * myMicST->lambda_prev[0] + (1 - myMicST->MICST_GAMMA2) * myMicST->MICST_DELTA2 * fabs(myMicST->zeta_prev[0]) * fabs(myMicST->zeta_prev[0]) * myMicST->xpower_prev[0];
    myMicST->lambda[myMicST->MICST_HALFFRAMESIZE2] = myMicST->MICST_GAMMA2 * myMicST->lambda_prev[myMicST->MICST_HALFFRAMESIZE2] + (1 - myMicST->MICST_GAMMA2) * myMicST->MICST_DELTA2 * fabs(myMicST->zeta_prev[myMicST->MICST_HALFFRAMESIZE2]) * fabs(myMicST->zeta_prev[myMicST->MICST_HALFFRAMESIZE2]) * myMicST->xpower_prev[myMicST->MICST_HALFFRAMESIZE2];

    for (k = 1; k < myMicST->MICST_HALFFRAMESIZE2; k++)
    {
        myMicST->lambda[k] = myMicST->MICST_GAMMA2 * myMicST->lambda_prev[k] + (1 - myMicST->MICST_GAMMA2) * myMicST->MICST_DELTA2 * fabs(myMicST->zeta_prev[k]) * fabs(myMicST->zeta_prev[k]) * myMicST->xpower_prev[k];
        myMicST->lambda[myMicST->MICST_FRAMESIZE2 - k] = myMicST->lambda[k];
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
    if (post < 0.0)
    {
        post = 0.0;
    }
    myMicST->ksi[0] = ((1 - myMicST->MICST_ALPHAD) * fabs(myMicST->zeta_prev[0]) * fabs(myMicST->zeta_prev[0]) * myMicST->xpower_prev[0] + myMicST->MICST_ALPHAD * post) / (myMicST->sigma[0] + myMicST->lambda[0] + 1E-20);

    post = myMicST->xpower[myMicST->MICST_HALFFRAMESIZE] - myMicST->sigma[myMicST->MICST_HALFFRAMESIZE] - myMicST->lambda[myMicST->MICST_HALFFRAMESIZE];
    if (post < 0.0)
    {
        post = 0.0;
    }
    myMicST->ksi[myMicST->MICST_HALFFRAMESIZE] = ((1 - myMicST->MICST_ALPHAD) * fabs(myMicST->zeta_prev[myMicST->MICST_HALFFRAMESIZE]) * fabs(myMicST->zeta_prev[myMicST->MICST_HALFFRAMESIZE]) * myMicST->xpower_prev[myMicST->MICST_HALFFRAMESIZE] + myMicST->MICST_ALPHAD * post) / (myMicST->sigma[myMicST->MICST_HALFFRAMESIZE] + myMicST->lambda[myMicST->MICST_HALFFRAMESIZE] + 1E-20);

    for (k = 1; k < myMicST->MICST_HALFFRAMESIZE; k++)
    {
        post = myMicST->xpower[k] - myMicST->sigma[k] - myMicST->lambda[k];
        if (post < 0.0)
        {
            post = 0.0;
        }
        myMicST->ksi[k] = ((1 - myMicST->MICST_ALPHAD) * fabs(myMicST->zeta_prev[k]) * fabs(myMicST->zeta_prev[k]) * myMicST->xpower_prev[k] + myMicST->MICST_ALPHAD * post) / (myMicST->sigma[k] + myMicST->lambda[k] + 1E-20);

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

    myMicST->zeta[0] = myMicST->ksi[0] / (myMicST->ksi[0] + 1);
    myMicST->zeta[myMicST->MICST_HALFFRAMESIZE2] = myMicST->ksi[myMicST->MICST_HALFFRAMESIZE2] / (myMicST->ksi[myMicST->MICST_HALFFRAMESIZE2] + 1);

    for (k = 1; k < myMicST->MICST_HALFFRAMESIZE2; k++)
    {
        myMicST->zeta[k] = myMicST->ksi[k] / (myMicST->ksi[k] + 1);
        myMicST->zeta[myMicST->MICST_FRAMESIZE2 - k] = myMicST->zeta[k];
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

    tmpSqrt = sqrt(myMicST->xpower[0]);

    myMicST->weightedResultReal[0] = myMicST->zeta[0] * myMicST->xfreqReal[0] / (tmpSqrt + 1E-20);
    myMicST->weightedResultImag[0] = myMicST->zeta[0] * myMicST->xfreqImag[0] / (tmpSqrt + 1E-20);

    tmpSqrt = sqrt(myMicST->xpower[myMicST->MICST_HALFFRAMESIZE2]);

    myMicST->weightedResultReal[myMicST->MICST_HALFFRAMESIZE2] = myMicST->zeta[myMicST->MICST_HALFFRAMESIZE2] * myMicST->xfreqReal[myMicST->MICST_HALFFRAMESIZE2] / (tmpSqrt + 1E-20);
    myMicST->weightedResultImag[myMicST->MICST_HALFFRAMESIZE2] = myMicST->zeta[myMicST->MICST_HALFFRAMESIZE2] * myMicST->xfreqImag[myMicST->MICST_HALFFRAMESIZE2] / (tmpSqrt + 1E-20);

    for (k = 1; k < myMicST->MICST_HALFFRAMESIZE2; k++)
    {
        tmpSqrt = sqrt(myMicST->xpower[k]);

        myMicST->weightedResultReal[k] = myMicST->zeta[k] * myMicST->xfreqReal[k] / (tmpSqrt + 1E-20);
        myMicST->weightedResultImag[k] = myMicST->zeta[k] * myMicST->xfreqImag[k] / (tmpSqrt + 1E-20);

        myMicST->weightedResultReal[myMicST->MICST_FRAMESIZE2 - k] = myMicST->weightedResultReal[k];
        myMicST->weightedResultImag[myMicST->MICST_FRAMESIZE2 - k] = -1.0 * myMicST->weightedResultImag[k];
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
