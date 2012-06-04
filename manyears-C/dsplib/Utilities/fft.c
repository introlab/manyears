/******************************************************************************* 
 * ManyEars: FFT - Source Code                                                 *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Author: François Grondin                                                    *
 * Original Code: Jean-Marc Valin                                              *
 * Modified Code: Simon Brière                                                 *
 * Version: 1.1.0                                                              *
 * Date: June 29th , 2010                                                      *
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

#include "fft.h"

/*******************************************************************************
 * Compatibility issues                                                        *
 ******************************************************************************/

// In some environment, the constant PI is not define with the math.h library
#ifndef M_PI
	#define M_PI	3.1415926535897932384626433832795f
#endif

/*******************************************************************************
 * fftInit                                                                     *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myFFT               The FFT object to be initialized           *
 *                                                                             *
 * Outputs:     myFFT               The initialized FFT object                 *
 *                                                                             *
 * Description: This function initializes the FFT object.                      *
 *                                                                             *
 ******************************************************************************/

void fftInit(struct objFFT* myFFT, struct ParametersStruct* myParameters, unsigned int size)
{

    // Temporary variable
    unsigned int tmpFrameSize;

    // Temporary variable
    unsigned int tmpNumberLevels;

    // Define the index to generate Wn(r)
    unsigned int r;

    // Define the index to generate the reverse bit order array
    unsigned int indexRevBitOrder;

    // Define the index to generate the empty array
    unsigned int emptyIndex;

    // Define the INDEX of the input parameter a
    unsigned int a;
    // Define the INDEX of the input parameter b
    unsigned int b;

    // Define accumulator to compute the index of parameters a and b
    unsigned int accumulatorA;
    // Define accumulator to compute the index of parameter r
    unsigned int accumulatorR;

    // Define the nubmer of groups in the current level
    unsigned int numberGroups;
    // Define the number of points per group
    unsigned int numberSubGroups;

    // Define the index of the level
    unsigned int indexLevel;
    // Define the index of the group
    unsigned int indexGroup;
    // Define the index of the point inside the group
    unsigned int indexSubGroup;

    // Define the index of the twiddle-factor in memory
    unsigned int indexTwiddle;

    // Define the index of the simd array for a with groups
    unsigned int simdAIndexGroup;
    // Define the index of the simd array for b with groups
    unsigned int simdBIndexGroup;
    // Define the index of the simd array for r with groups
    unsigned int simdRIndexGroup;
    // Define the index of the simd array for a with individual elements
    unsigned int simdAIndexIndividual;
    // Define the index of the simd array for b with individual elements
    unsigned int simdBIndexIndividual;

    // *************************************************************************
    // * STEP 1: Load parameters                                               *
    // *************************************************************************

    myFFT->FFT_SIZE = size;

    tmpFrameSize = myFFT->FFT_SIZE;
    tmpNumberLevels = 0;

    while(tmpFrameSize > 1)
    {
        tmpNumberLevels++;
        tmpFrameSize /= 2;
    }

    myFFT->FFT_NBLEVELS = tmpNumberLevels;
    myFFT->FFT_HALFSIZE = myFFT->FFT_SIZE / 2;
    myFFT->FFT_SIZE_INV = (1.0f / myFFT->FFT_SIZE);
    myFFT->FFT_SIMD_GROUP = ((myFFT->FFT_SIZE/2) * (myFFT->FFT_NBLEVELS-2) / 4);
    myFFT->FFT_SIMD_INDIVIDUAL = ((myFFT->FFT_SIZE/2) * 2);

    // *************************************************************************
    // * STEP 2: Initialize context                                            *
    // *************************************************************************

        // +-------------------------------------------------------------------+
        // | Step A: Create arrays                                             |
        // +-------------------------------------------------------------------+

        myFFT->WnReal = (float*) newTable1D(myFFT->FFT_HALFSIZE, sizeof(float));
        myFFT->WnImag = (float*) newTable1D(myFFT->FFT_HALFSIZE, sizeof(float));
        myFFT->simdWnReal = (float*) newTable1D(myFFT->FFT_SIMD_GROUP*4, sizeof(float));
        myFFT->simdWnImag = (float*) newTable1D(myFFT->FFT_SIMD_GROUP*4, sizeof(float));
        myFFT->workingArrayReal = (float*) newTable1D(myFFT->FFT_SIZE, sizeof(float));
        myFFT->workingArrayImag = (float*) newTable1D(myFFT->FFT_SIZE, sizeof(float));
        myFFT->fftTwiceReal = (float*) newTable1D(myFFT->FFT_SIZE, sizeof(float));
        myFFT->fftTwiceRealFlipped = (float*) newTable1D(myFFT->FFT_SIZE, sizeof(float));
        myFFT->fftTwiceImag = (float*) newTable1D(myFFT->FFT_SIZE, sizeof(float));
        myFFT->fftTwiceImagFlipped = (float*) newTable1D(myFFT->FFT_SIZE, sizeof(float));
        myFFT->emptyArray = (float*) newTable1D(myFFT->FFT_SIZE, sizeof(float));
        myFFT->trashArray = (float*) newTable1D(myFFT->FFT_SIZE, sizeof(float));
        myFFT->revBitOrderArray = (unsigned int*) newTable1D(myFFT->FFT_SIZE, sizeof(unsigned int));
        myFFT->simdARealGroups = (float**) newTable1D(myFFT->FFT_SIMD_GROUP, sizeof(float*));
        myFFT->simdAImagGroups = (float**) newTable1D(myFFT->FFT_SIMD_GROUP, sizeof(float*));
        myFFT->simdBRealGroups = (float**) newTable1D(myFFT->FFT_SIMD_GROUP, sizeof(float*));
        myFFT->simdBImagGroups = (float**) newTable1D(myFFT->FFT_SIMD_GROUP, sizeof(float*));
        myFFT->simdRRealGroups = (float**) newTable1D(myFFT->FFT_SIMD_GROUP, sizeof(float*));
        myFFT->simdRImagGroups = (float**) newTable1D(myFFT->FFT_SIMD_GROUP, sizeof(float*));
        myFFT->simdAIndividual = (float**) newTable1D(myFFT->FFT_SIMD_INDIVIDUAL, sizeof(float*));
        myFFT->simdBIndividual = (float**) newTable1D(myFFT->FFT_SIMD_INDIVIDUAL, sizeof(float*));

        // +-------------------------------------------------------------------+
        // | Step B: Generate the FFT factors Wn(r)                            |
        // +-------------------------------------------------------------------+

        // Generate Wn(r) = exp(-j*2*pi*r/N) for r = 0 ... (N/2 - 1)
        for (r = 0; r < myFFT->FFT_HALFSIZE; r++)
        {

            myFFT->WnReal[r] = cosf(2.0f * M_PI * r / myFFT->FFT_SIZE);
            myFFT->WnImag[r] = -1.0f * sinf(2.0f * M_PI * r / myFFT->FFT_SIZE);

        }

        // +-------------------------------------------------------------------+
        // | Step C: Generate an array with reverse-bit indexes                |
        // +-------------------------------------------------------------------+

        // Generate an array of reverse bit order
        for (indexRevBitOrder = 0; indexRevBitOrder < myFFT->FFT_SIZE; indexRevBitOrder++)
        {

            myFFT->revBitOrderArray[indexRevBitOrder] = (indexRevBitOrder & 0x0001) << 15;
            myFFT->revBitOrderArray[indexRevBitOrder] += (indexRevBitOrder & 0x0002) << 13;
            myFFT->revBitOrderArray[indexRevBitOrder] += (indexRevBitOrder & 0x0004) << 11;
            myFFT->revBitOrderArray[indexRevBitOrder] += (indexRevBitOrder & 0x0008) << 9;
            myFFT->revBitOrderArray[indexRevBitOrder] += (indexRevBitOrder & 0x0010) << 7;
            myFFT->revBitOrderArray[indexRevBitOrder] += (indexRevBitOrder & 0x0020) << 5;
            myFFT->revBitOrderArray[indexRevBitOrder] += (indexRevBitOrder & 0x0040) << 3;
            myFFT->revBitOrderArray[indexRevBitOrder] += (indexRevBitOrder & 0x0080) << 1;
            myFFT->revBitOrderArray[indexRevBitOrder] += (indexRevBitOrder & 0x0100) >> 1;
            myFFT->revBitOrderArray[indexRevBitOrder] += (indexRevBitOrder & 0x0200) >> 3;
            myFFT->revBitOrderArray[indexRevBitOrder] += (indexRevBitOrder & 0x0400) >> 5;
            myFFT->revBitOrderArray[indexRevBitOrder] += (indexRevBitOrder & 0x0800) >> 7;
            myFFT->revBitOrderArray[indexRevBitOrder] += (indexRevBitOrder & 0x1000) >> 9;
            myFFT->revBitOrderArray[indexRevBitOrder] += (indexRevBitOrder & 0x2000) >> 11;
            myFFT->revBitOrderArray[indexRevBitOrder] += (indexRevBitOrder & 0x4000) >> 13;
            myFFT->revBitOrderArray[indexRevBitOrder] += (indexRevBitOrder & 0x8000) >> 15;

            myFFT->revBitOrderArray[indexRevBitOrder] >>= (16 - myFFT->FFT_NBLEVELS);

        }

        // +-------------------------------------------------------------------+
        // | Step D: Generate an empty array                                   |
        // +-------------------------------------------------------------------+

        // Generate an empty array (will be used as a dummy array for the imaginary
        // part when the FFT of a single real signal is computed)
        for (emptyIndex = 0; emptyIndex < myFFT->FFT_SIZE; emptyIndex++)
        {
            myFFT->emptyArray[emptyIndex] = 0;
        }

        // +-------------------------------------------------------------------+
        // | Step E: SIMD: Compute the indexes used for memory accesses        |
        // +-------------------------------------------------------------------+

        // Load parameters
        numberGroups = 1;
        numberSubGroups = myFFT->FFT_HALFSIZE;

        // Initialize pointers
        simdAIndexGroup = 0;
        simdBIndexGroup = 0;
        simdRIndexGroup = 0;
        simdAIndexIndividual = 0;
        simdBIndexIndividual = 0;
        indexTwiddle = 0;

        // Loop for each level
        for (indexLevel = 0; indexLevel < myFFT->FFT_NBLEVELS; indexLevel++)
        {

            accumulatorA = 0;
            accumulatorR = 0;

            // Loop for each group in the current level
            for (indexGroup = 0; indexGroup < numberGroups; indexGroup++)
            {

                // Loop for each element of the group
                for (indexSubGroup = 0; indexSubGroup < numberSubGroups; indexSubGroup++)
                {

                    // Calculate the indexes
                    a = accumulatorA;
                    b = accumulatorA + numberSubGroups;
                    r = accumulatorR;
                    accumulatorA++;
                    accumulatorR += numberGroups;

                    // Check if there are groups of at least 4 elements
                    if (numberSubGroups >= 4)
                    {

                        // Copy corresponding twiddle factor
                        myFFT->simdWnReal[indexTwiddle] = myFFT->WnReal[r];
                        myFFT->simdWnImag[indexTwiddle] = myFFT->WnImag[r];
                        indexTwiddle++;

                        // Check if a is a multiple of 4
                        if ((a / 4.0f) == floorf(a/4.0f))
                        {

                            myFFT->simdARealGroups[simdAIndexGroup] = &myFFT->workingArrayReal[a];
                            myFFT->simdAImagGroups[simdAIndexGroup] = &myFFT->workingArrayImag[a];
                            myFFT->simdBRealGroups[simdBIndexGroup] = &myFFT->workingArrayReal[b];
                            myFFT->simdBImagGroups[simdBIndexGroup] = &myFFT->workingArrayImag[b];
                            myFFT->simdRRealGroups[simdRIndexGroup] = &myFFT->simdWnReal[indexTwiddle - 1];
                            myFFT->simdRImagGroups[simdRIndexGroup] = &myFFT->simdWnImag[indexTwiddle - 1];

                            simdAIndexGroup++;
                            simdBIndexGroup++;
                            simdRIndexGroup++;

                        }

                    }
                    else
                    {

                        myFFT->simdAIndividual[simdAIndexIndividual++] = &myFFT->workingArrayReal[a];
                        myFFT->simdBIndividual[simdBIndexIndividual++] = &myFFT->workingArrayReal[b];

                    }

                }

                // Update accumulators
                accumulatorA += numberSubGroups;
                accumulatorR = 0;

            }

            // Divide the number of points by group by 2 for the next level
            numberSubGroups >>= 1;
            // Multiply the number of groups by 2 for the next level
            numberGroups <<= 1;

        }

}

/*******************************************************************************
 * fftTerminate                                                                *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myFFT               The FFT object to be terminated            *
 *                                                                             *
 * Outputs:     (none)                                                         *
 *                                                                             *
 * Description: This function terminates the FFT object.                       *
 *                                                                             *
 ******************************************************************************/

void fftTerminate(struct objFFT* myFFT)
{

    // Free memory
    deleteTable1D((void*) myFFT->WnReal);
    deleteTable1D((void*) myFFT->WnImag);
    deleteTable1D((void*) myFFT->simdWnReal);
    deleteTable1D((void*) myFFT->simdWnImag);
    deleteTable1D((void*) myFFT->workingArrayReal);
    deleteTable1D((void*) myFFT->workingArrayImag);
    deleteTable1D((void*) myFFT->fftTwiceReal);
    deleteTable1D((void*) myFFT->fftTwiceRealFlipped);
    deleteTable1D((void*) myFFT->fftTwiceImag);
    deleteTable1D((void*) myFFT->fftTwiceImagFlipped);
    deleteTable1D((void*) myFFT->emptyArray);
    deleteTable1D((void*) myFFT->trashArray);
    deleteTable1D((void*) myFFT->revBitOrderArray);
    deleteTable1D((void*) myFFT->simdARealGroups);
    deleteTable1D((void*) myFFT->simdAImagGroups);
    deleteTable1D((void*) myFFT->simdBRealGroups);
    deleteTable1D((void*) myFFT->simdBImagGroups);
    deleteTable1D((void*) myFFT->simdRRealGroups);
    deleteTable1D((void*) myFFT->simdRImagGroups);
    deleteTable1D((void*) myFFT->simdAIndividual);
    deleteTable1D((void*) myFFT->simdBIndividual);

}

/*******************************************************************************
 * fftCompute                                                                  *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myFFT               The FFT object used for this computation   *
 *              sourceArrayReal     Pointer to the first element of the array  *
 *                                  that contains the real part of the signal  *
 *                                  to be transformed with the FFT             *
 *              sourceArrayImag     Pointer to the first element of the array  *
 *                                  that contains the imaginary part of the    *
 *                                  signal to be transformed with the FFT      *
 *                                                                             *
 * Outputs:     destArrayReal       Pointer to the first element of the array  *
 *                                  that will receive the real part of the     *
 *                                  result of the FFT                          *
 *              destArrayImag       Pointer to the first element of the array  *
 *                                  that will receive the imaginary part of    *
 *                                  the result of the FFT                      *
 *                                                                             *
 *                                                                             *
 * Description: This function performs the FFT computation using the           *
 *              radix-2 decimation-in-frequency FFT algorithm                  *
 *                                                                             *
 ******************************************************************************/

void fftCompute(struct objFFT* myFFT, float* sourceArrayReal, float* sourceArrayImag, float* destArrayReal, float* destArrayImag)
{

#ifndef USE_SIMD

    unsigned int indexArray;
/*
    // The basic structure in this algorithm is a butterfly as follows:
    //
    // a o---o-->--o--->---o A = a + b
    //        \   /
    //         \ /
    //          X
    //         / \
    //        /   \  Wn(r)
    // b o---o-->--o--->---o B = (a - b) Wn(r)
    //         -1
*/
    // Define the INDEX of the input parameter a
    unsigned int a;
    // Define the INDEX of the input parameter b
    unsigned int b;
    // Define the parameter r
    unsigned int r;

    // Define the real part of the input parameter a
    float valueAReal;
    // Define the imaginary part of the input parameter a
    float valueAImag;
    // Define the real part of the input parameter b
    float valueBReal;
    // Define the imaginary part of the input parameter b
    float valueBImag;
    // Define the real part of the output parameter A
    float newValueAReal;
    // Define the imaginary part of the output parameter A
    float newValueAImag;
    // Define the real part of the output parameter B
    float newValueBReal;
    // Define the imaginary part of the output parameter B
    float newValueBImag;

    // Define accumulator to compute the index of parameters a and b
    unsigned int accumulatorA;
    // Define accumulator to compute the index of parameter r
    unsigned int accumulatorR;

    // Define the nubmer of groups in the current level
    unsigned int numberGroups;
    // Define the number of points per group
    unsigned int numberSubGroups;

    // Define the index of the level
    unsigned int indexLevel;
    // Define the index of the group
    unsigned int indexGroup;
    // Define the index of the point inside the group
    unsigned int indexSubGroup;

    // Define the index to generate the reverse bit order array
    unsigned int indexRevBitOrder;

    // Temporary variables to speed up butterfly computation
    float diffABReal;
    float diffABImag;
    float curWnReal;
    float curWnImag;

    // Load parameters
    numberGroups = 1;
    numberSubGroups = myFFT->FFT_HALFSIZE;

    // Copy all elements from the source array in the working array
    for (indexArray = 0; indexArray < myFFT->FFT_SIZE; indexArray++)
    {
        myFFT->workingArrayReal[indexArray] = sourceArrayReal[indexArray];
        myFFT->workingArrayImag[indexArray] = sourceArrayImag[indexArray];
    }

    // Loop for each level
    for (indexLevel = 0; indexLevel < myFFT->FFT_NBLEVELS; indexLevel++)
    {

        accumulatorA = 0;
        accumulatorR = 0;

        // Loop for each group in the current level
        for (indexGroup = 0; indexGroup < numberGroups; indexGroup++)
        {

            // Loop for each element of the group
            for (indexSubGroup = 0; indexSubGroup < numberSubGroups; indexSubGroup++)
            {

                // Calculate the indexes
                a = accumulatorA;
                b = accumulatorA + numberSubGroups;
                r = accumulatorR;
                accumulatorA++;
                accumulatorR += numberGroups;

                // Load the values a and b (these are complex values)
                valueAReal = myFFT->workingArrayReal[a];
                valueAImag = myFFT->workingArrayImag[a];
                valueBReal = myFFT->workingArrayReal[b];
                valueBImag = myFFT->workingArrayImag[b];

                // Apply A = a + b from the butterfly
                newValueAReal = valueAReal + valueBReal;
                newValueAImag = valueAImag + valueBImag;

                // Apply B = (a - b) * Wn(r) from the butterfly
                diffABReal = valueAReal - valueBReal;
                diffABImag = valueAImag - valueBImag;
                curWnReal = myFFT->WnReal[r];
                curWnImag = myFFT->WnImag[r];
                newValueBReal = diffABReal * curWnReal - diffABImag * curWnImag;
                newValueBImag = diffABReal * curWnImag + diffABImag * curWnReal;

                // Save results at the same place as the initial values
                myFFT->workingArrayReal[a] = newValueAReal;
                myFFT->workingArrayImag[a] = newValueAImag;
                myFFT->workingArrayReal[b] = newValueBReal;
                myFFT->workingArrayImag[b] = newValueBImag;

            }

            // Update accumulators
            accumulatorA += numberSubGroups;
            accumulatorR = 0;

        }

        // Divide the number of points by group by 2 for the next level
        numberSubGroups >>= 1;
        // Multiply the number of groups by 2 for the next level
        numberGroups <<= 1;

    }

    // Reorder result (it is actually in reverse bit order) and copy to destination array
    for (indexRevBitOrder = 0; indexRevBitOrder < myFFT->FFT_SIZE; indexRevBitOrder++)
    {

        destArrayReal[indexRevBitOrder] = myFFT->workingArrayReal[myFFT->revBitOrderArray[indexRevBitOrder]];
        destArrayImag[indexRevBitOrder] = myFFT->workingArrayImag[myFFT->revBitOrderArray[indexRevBitOrder]];

    }

#else

    // Array index
    unsigned int indexGroup;
    unsigned int indexLevel;
    unsigned int indexArray;

    // Define variables for the last two levels
    float valueAReal;
    float valueAImag;
    float valueBReal;
    float valueBImag;
    float newValueAReal;
    float newValueAImag;
    float newValueBReal;
    float newValueBImag;
    unsigned int a;
    unsigned int b;
    unsigned int accumulatorA;

    // Define the index to generate the reverse bit order array
    unsigned int indexRevBitOrder;

    // SIMD registers
    __m128_mod regA, regB, regC, regD, regE, regF, regG;

    // *************************************************************************
    // * STEP 0: Copy source                                                   *
    // *************************************************************************

    // Copy all elements from the source array in the working array
    for (indexArray = 0; indexArray < myFFT->FFT_SIZE; indexArray+=4)
    {

        // Load sourceArrayReal[k] in regA
        regA.m128 = _mm_load_ps(&sourceArrayReal[indexArray]);

        // Load sourceArrayImag[k] in regB
        regB.m128 = _mm_load_ps(&sourceArrayImag[indexArray]);

        // Copy regA in workingArrayReal[k]
        _mm_store_ps(&myFFT->workingArrayReal[indexArray], regA.m128);

        // Copy regB in workingArrayImag[k]
        _mm_store_ps(&myFFT->workingArrayImag[indexArray], regB.m128);

    }

    // *************************************************************************
    // * STEP 1: Perform computations for all levels except two last one       *
    // *************************************************************************

        // Loop for the groups
        indexGroup = 0;

        for (indexLevel = 0; indexLevel < (myFFT->FFT_NBLEVELS - 2); indexLevel++)
        {

            for (indexArray = 0; indexArray < (myFFT->FFT_SIZE/8); indexArray++)
            {

                // Load arguments aReal, aImag, bReal and bImag
                regA.m128 = _mm_load_ps(myFFT->simdARealGroups[indexGroup]);
                regB.m128 = _mm_load_ps(myFFT->simdAImagGroups[indexGroup]);
                regC.m128 = _mm_load_ps(myFFT->simdBRealGroups[indexGroup]);
                regD.m128 = _mm_load_ps(myFFT->simdBImagGroups[indexGroup]);

                // First addition: (aReal + bReal), (aImag + bImag)
                regE.m128 = _mm_add_ps(regA.m128,regC.m128);
                regF.m128 = _mm_add_ps(regB.m128,regD.m128);

                // Store A = (aReal + bReal) + j(aImag + bImag)
                _mm_store_ps(myFFT->simdARealGroups[indexGroup], regE.m128);
                _mm_store_ps(myFFT->simdAImagGroups[indexGroup], regF.m128);

                // Second addition: B = (aReal - bReal), (aImag - bImag)
                regE.m128 = _mm_sub_ps(regA.m128,regC.m128);
                regF.m128 = _mm_sub_ps(regB.m128,regD.m128);

                // Load twiddle factor WnReal and WnImag
                regA.m128 = _mm_load_ps(myFFT->simdRRealGroups[indexGroup]);
                regB.m128 = _mm_load_ps(myFFT->simdRImagGroups[indexGroup]);

                // Multiplications

                // (E*A - F*B)
                regC.m128 = _mm_mul_ps(regE.m128,regA.m128);
                regD.m128 = _mm_mul_ps(regF.m128,regB.m128);
                regG.m128 = _mm_sub_ps(regC.m128,regD.m128);

                // (F*A + E*B)
                regC.m128 = _mm_mul_ps(regF.m128,regA.m128);
                regD.m128 = _mm_mul_ps(regE.m128,regB.m128);
                regA.m128 = _mm_add_ps(regC.m128,regD.m128);

                // Store B = (aReal - bReal) * WnReal - (aImag - bImag) * WnImag
                //         + j[ (aImag - bImag) * WnReal + (aReal - bReal) * WnImag ]

                _mm_store_ps(myFFT->simdBRealGroups[indexGroup], regG.m128);
                _mm_store_ps(myFFT->simdBImagGroups[indexGroup], regA.m128);

                // Increment the counter
                indexGroup++;

            }

        }

    // *************************************************************************
    // * STEP 2: Perform computations for level 1                              *
    // *************************************************************************

    accumulatorA = 0;

    // Loop for each group in the current level
    for (indexGroup = 0; indexGroup < myFFT->FFT_SIZE/4; indexGroup++)
    {

        // Calculate the indexes
        a = accumulatorA;
        b = accumulatorA + 2;
        accumulatorA++;

        // Load the values a and b (these are complex values)
        valueAReal = myFFT->workingArrayReal[a];
        valueAImag = myFFT->workingArrayImag[a];
        valueBReal = myFFT->workingArrayReal[b];
        valueBImag = myFFT->workingArrayImag[b];

        // Apply A = a + b
        newValueAReal = valueAReal + valueBReal;
        newValueAImag = valueAImag + valueBImag;

        // Apply B = a - b
        newValueBReal = valueAReal - valueBReal;
        newValueBImag = valueAImag - valueBImag;

        // Save results at the same place as the initial values
        myFFT->workingArrayReal[a] = newValueAReal;
        myFFT->workingArrayImag[a] = newValueAImag;
        myFFT->workingArrayReal[b] = newValueBReal;
        myFFT->workingArrayImag[b] = newValueBImag;

        // Calculate the indexes
        a = accumulatorA;
        b = accumulatorA + 2;
        accumulatorA+=3;

        // Load the values a and b (these are complex values)
        valueAReal = myFFT->workingArrayReal[a];
        valueAImag = myFFT->workingArrayImag[a];
        valueBReal = myFFT->workingArrayReal[b];
        valueBImag = myFFT->workingArrayImag[b];

        // Apply A = a + b
        newValueAReal = valueAReal + valueBReal;
        newValueAImag = valueAImag + valueBImag;

        // Apply B = (a - b) * -j = [(aReal - bReal) + j * (aImag - bImag)] * -j =
        //                          (aImag - bImag) + j * (bReal - aReal)
        newValueBReal = valueAImag - valueBImag;
        newValueBImag = valueBReal - valueAReal;

        // Save results at the same place as the initial values
        myFFT->workingArrayReal[a] = newValueAReal;
        myFFT->workingArrayImag[a] = newValueAImag;
        myFFT->workingArrayReal[b] = newValueBReal;
        myFFT->workingArrayImag[b] = newValueBImag;

    }

    // *************************************************************************
    // * STEP 3: Perform computations for level 0                              *
    // *************************************************************************

    accumulatorA = 0;

    // Loop for each group in the current level
    for (indexGroup = 0; indexGroup < myFFT->FFT_SIZE/2; indexGroup++)
    {

        // Calculate the indexes
        a = accumulatorA;
        b = accumulatorA + 1;
        accumulatorA+=2;

        // Load the values a and b (these are complex values)
        valueAReal = myFFT->workingArrayReal[a];
        valueAImag = myFFT->workingArrayImag[a];
        valueBReal = myFFT->workingArrayReal[b];
        valueBImag = myFFT->workingArrayImag[b];

        // Apply A = a + b
        newValueAReal = valueAReal + valueBReal;
        newValueAImag = valueAImag + valueBImag;

        // Apply B = a - b
        newValueBReal = valueAReal - valueBReal;
        newValueBImag = valueAImag - valueBImag;

        // Save results at the same place as the initial values
        myFFT->workingArrayReal[a] = newValueAReal;
        myFFT->workingArrayImag[a] = newValueAImag;
        myFFT->workingArrayReal[b] = newValueBReal;
        myFFT->workingArrayImag[b] = newValueBImag;

    }

    // *************************************************************************
    // * STEP 4: Copy result                                                   *
    // *************************************************************************

    // Reorder result (it is actually in reverse bit order) and copy to destination array
    for (indexRevBitOrder = 0; indexRevBitOrder < myFFT->FFT_SIZE; indexRevBitOrder++)
    {

        destArrayReal[indexRevBitOrder] = myFFT->workingArrayReal[myFFT->revBitOrderArray[indexRevBitOrder]];
        destArrayImag[indexRevBitOrder] = myFFT->workingArrayImag[myFFT->revBitOrderArray[indexRevBitOrder]];

    }

#endif

}

/*******************************************************************************
 * fftComputeOnce                                                              *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myFFT               The FFT object used for this computation   *
 *              sourceArray         Pointer to the first element of the array  *
 *                                  that contains the signal to be transformed *
 *                                  with the FFT (signal is real)              *
 *                                                                             *
 * Outputs:     destArrayReal       Pointer to the first element of the array  *
 *                                  that will receive the real part of the     *
 *                                  result of the FFT of the signal            *
 *              destArrayImag       Pointer to the first element of the array  *
 *                                  that will receive the imaginary part of    *
 *                                  the result of the FFT of the signal        *
 *                                                                             *
 * Description: This function computes the FFT for a real signal               *
 *                                                                             *
 ******************************************************************************/

void fftComputeOnce(struct objFFT* myFFT, float *sourceArray, float *destArrayReal, float *destArrayImag)
{
    fftCompute(myFFT, sourceArray, myFFT->emptyArray, destArrayReal, destArrayImag);
}

/*******************************************************************************
 * fftComputeTwice                                                             *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myFFT               The FFT object used for this computation   *
 *              sourceArray1        Pointer to the first element of the array  *
 *                                  that contains the signal to be transformed *
 *                                  with the FFT (signal is real)              *
 *              sourceArray2        Pointer to the first element of the array  *
 *                                  that contains the signal to be transformed *
 *                                  with the FFT (signal is real)              *
 *                                                                             *
 * Outputs:     destArray1Real      Pointer to the first element of the array  *
 *                                  that will receive the real part of the     *
 *                                  result of the FFT of the first signal      *
 *              destArray1Imag      Pointer to the first element of the array  *
 *                                  that will receive the imaginary part of    *
 *                                  the result of the FFT of the first signal  *
 *              destArray2Real      Pointer to the first element of the array  *
 *                                  that will receive the real part of the     *
 *                                  result of the FFT of the second signal     *
 *              destArray2Imag      Pointer to the first element of the array  *
 *                                  that will receive the imaginary part of    *
 *                                  the result of the FFT of the second signal *
 *                                                                             *
 * Description: This function performs two FFT computations using the          *
 *              radix-2 decimation-in-frequency FFT algorithm                  *
 *                                                                             *
 ******************************************************************************/

void fftComputeTwice(struct objFFT* myFFT, float *sourceArray1, float *sourceArray2, float *destArray1Real, float *destArray1Imag, float *destArray2Real, float *destArray2Imag)
{

    // Index to loop throught the array
    unsigned int k;

    // Compute the FFT
    fftCompute(myFFT, sourceArray1, sourceArray2, myFFT->fftTwiceReal, myFFT->fftTwiceImag);

    // Real part of the first FFT = (twiceReal + twiceReal flipped) / 2
    // Imag part of the first FFT = (twiceImag - twiceImag flipped) / 2
    // Real part of the second FFT = (twiceImag + twiceImag flipped) / 2
    // Imag part of the second FFT = (twiceReal flipped - twiceReal) / 2

     // Except for DC that stays the same
    destArray1Real[0] = myFFT->fftTwiceReal[0];
    destArray1Imag[0] = 0;
    destArray2Real[0] = myFFT->fftTwiceImag[0];
    destArray2Imag[0] = 0;

    for (k = 1; k < myFFT->FFT_SIZE; k++)
    {
        destArray1Real[k] = (myFFT->fftTwiceReal[k] + myFFT->fftTwiceReal[myFFT->FFT_SIZE - k]) / 2;
        destArray1Imag[k] = (myFFT->fftTwiceImag[k] - myFFT->fftTwiceImag[myFFT->FFT_SIZE - k]) / 2;
        destArray2Real[k] = (myFFT->fftTwiceImag[k] + myFFT->fftTwiceImag[myFFT->FFT_SIZE - k]) / 2;
        destArray2Imag[k] = (myFFT->fftTwiceReal[myFFT->FFT_SIZE - k] - myFFT->fftTwiceReal[k]) / 2;
    }

}

/*******************************************************************************
 * ifftCompute                                                                 *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myFFT               The FFT object used for this computation   *
 *              sourceArrayReal     Pointer to the first element of the array  *
 *                                  that contains the real part of the signal  *
 *                                  to be transformed with the inverse FFT     *
 *              sourceArrayImag     Pointer to the first element of the array  *
 *                                  that contains the imaginary part of the    *
 *                                  signal to be transformed with the inverse  *
 *                                  FFT                                        *
 *                                                                             *
 * Outputs:     destArrayReal       Pointer to the first element of the array  *
 *                                  that will receive the real part of the     *
 *                                  result of the inverse FFT                  *
 *              destArrayImag       Pointer to the first element of the array  *
 *                                  that will receive the imaginary part of    *
 *                                  the result of the inverse FFT              *
 *                                                                             *
 *                                                                             *
 * Description: This function performs the inverse FFT computation using the   *
 *              radix-2 decimation-in-frequency FFT algorithm with some        *
 *		preprocessing and postprocessing:                              *
 *                                                                             *
 *              X1[k] for k = 0 , 1 , ... , N is the signal in freq domain     *
 *                                                                             *
 *              Preprocessing:	X2[k] = X1[k]* for k = 0 , 1 , ... , N         *
 *              Processing:     X3[k] = FFT(X1[k]) for k = 0 , 1 , ... , N     *
 *              Postprocessing:	X4[k] = (1/N) * X3[k]* for k = 0 , 1 , ... , N *
 *                                                                             *
 *              X4[k] for k = 0 , 1 , ... , N is the result of the inverse     *
 *              Fourier transform                                              *
 *                                                                             *
 ******************************************************************************/

void ifftCompute(struct objFFT* myFFT, float *sourceArrayReal, float *sourceArrayImag, float *destArrayReal, float *destArrayImag)
{

#ifndef USE_SIMD

    // Index to go through each element
    unsigned int k;

    // Apply prefiltering
    for (k = 0; k < myFFT->FFT_SIZE; k++)
    {
        destArrayImag[k] = -1.0f * sourceArrayImag[k];
    }

    // Compute FFT
    fftCompute(myFFT, sourceArrayReal, destArrayImag, destArrayReal, destArrayImag);

    // Apply post filtering
    for (k = 0; k < myFFT->FFT_SIZE; k++)
    {
        destArrayReal[k] = myFFT->FFT_SIZE_INV * destArrayReal[k];
        destArrayImag[k] = -1.0f * myFFT->FFT_SIZE_INV * destArrayImag[k];
    }

#else

    // Index to go through each element
    unsigned int k;

    // SIMD registers
    __m128_mod regA, regB, regC, regD, regE, regF;

    regA.m128_f32[0] = -1;
    regA.m128_f32[1] = -1;
    regA.m128_f32[2] = -1;
    regA.m128_f32[3] = -1;

    // Apply prefiltering
    for (k = 0; k < myFFT->FFT_SIZE; k+=4)
    {
        regB.m128 = _mm_load_ps(&sourceArrayImag[k]);
        regC.m128 = _mm_mul_ps(regA.m128,regB.m128);
        _mm_store_ps(&destArrayImag[k],regC.m128);
    }

    // Compute FFT
    fftCompute(myFFT, sourceArrayReal, destArrayImag, destArrayReal, destArrayImag);

    regA.m128_f32[0] = myFFT->FFT_SIZE_INV;
    regA.m128_f32[1] = myFFT->FFT_SIZE_INV;
    regA.m128_f32[2] = myFFT->FFT_SIZE_INV;
    regA.m128_f32[3] = myFFT->FFT_SIZE_INV;
    regB.m128_f32[0] = -1 * myFFT->FFT_SIZE_INV;
    regB.m128_f32[1] = -1 * myFFT->FFT_SIZE_INV;
    regB.m128_f32[2] = -1 * myFFT->FFT_SIZE_INV;
    regB.m128_f32[3] = -1 * myFFT->FFT_SIZE_INV;

    // Apply post filtering
    for (k = 0; k < myFFT->FFT_SIZE; k+= 4)
    {
        regC.m128 = _mm_load_ps(&destArrayReal[k]);
        regD.m128 = _mm_load_ps(&destArrayImag[k]);
        regE.m128 = _mm_mul_ps(regC.m128,regA.m128);
        regF.m128 = _mm_mul_ps(regD.m128,regB.m128);
        _mm_store_ps(&destArrayReal[k],regE.m128);
        _mm_store_ps(&destArrayImag[k],regF.m128);
    }

#endif

}

/*******************************************************************************
 * ifftComputeOnce                                                             *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myFFT               The FFT object used for this computation   *
 *              sourceArray1Real    Pointer to the first element of the array  *
 *                                  that contains the real part of the signal  *
 *                                  to be transformed with the inverse FFT     *
 *                                  (signal is even)                           *
 *              sourceArray1Imag    Pointer to the first element of the array  *
 *                                  that contains the imaginary part of the    *
 *                                  signal to be transformed with the inverse  *
 *                                  FFT (signal is even)                       *
 *                                                                             *
 * Outputs:     destArray1Real      Pointer to the first element of the array  *
 *                                  that will receive the real part of the     *
 *                                  result of the FFT of the signal            *
 *                                                                             *
 * Description: This function computes the inverse FFT for a real signal       *
 *                                                                             *
 ******************************************************************************/

void ifftComputeOnce(struct objFFT* myFFT, float *sourceArray1Real, float *sourceArray1Imag, float *destArray1)
{
    ifftCompute(myFFT, sourceArray1Real, sourceArray1Imag, destArray1, myFFT->trashArray);
}

/*******************************************************************************
 * ifftComputeTwice                                                            *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myFFT               The FFT object used for this computation   *
 *              sourceArray1Real    Pointer to the first element of the array  *
 *                                  that contains the real part of the signal  *
 *                                  to be transformed with the inverse FFT     *
 *                                  (signal is even)                           *
 *              sourceArray1Imag    Pointer to the first element of the array  *
 *                                  that contains the imaginary part of the    *
 *                                  signal to be transformed with the inverse  *
 *                                  FFT (signal is even)                       *
 *              sourceArray2Real    Pointer to the first element of the array  *
 *                                  that contains the real part of the signal  *
 *                                  to be transformed with the inverse FFT     *
 *                                  (signal is even)                           *
 *              sourceArray2Imag    Pointer to the first element of the array  *
 *                                  that contains the imaginary part of the    *
 *                                  signal to be transformed with the inverse  *
 *                                  FFT (signal is even)                       *
 *                                                                             *
 * Outputs:     destArray1Real      Pointer to the first element of the array  *
 *                                  that will receive the real part of the     *
 *                                  result of the FFT of the first signal      *
 *              destArray2Real      Pointer to the first element of the array  *
 *                                  that will receive the real part of the     *
 *                                  result of the FFT of the second signal     *
 *                                                                             *
 * Description: This function performs two inverse FFT computations using the  *
 *              radix-2 decimation-in-frequency FFT algorithm.                 *
 *                                                                             *
 ******************************************************************************/

void ifftComputeTwice(struct objFFT* myFFT, float *sourceArray1Real, float *sourceArray1Imag, float *sourceArray2Real, float *sourceArray2Imag, float *destArray1, float *destArray2)
{

    // Index to loop throught the array
    unsigned int k;

    // Real part of the FFT = firstReal - secondImag
    // Real part of the flipped FFT = firstReal + secondImag
    // Imaginary part of the FFT = firstImag + secondReal
    // Imaginary part of the FFT = secondReal - firstImag

    // Use destArray1 to store the real part of the FFT to be inversed
    // Use destArray2 to store the imaginary part of the FFT to be inversed

    // Re {Y_0} = Re {X1_0}
    // Im {Y_0} = Re {X2_0}
    destArray1[0] = sourceArray1Real[0];
    destArray2[0] = sourceArray2Real[0];

    // Re {Y_(N/2)} = Re{X1_(N/2)}
    // Im {Y_(N/2)} = Re{X2_(N/2)}
    destArray1[myFFT->FFT_HALFSIZE] = sourceArray1Real[myFFT->FFT_HALFSIZE];
    destArray2[myFFT->FFT_HALFSIZE] = sourceArray2Real[myFFT->FFT_HALFSIZE];

    // Re {Y_k} = Re{X1_k} - Im{X2_k} for k = 1 ... (N/2) - 1
    // Im {Y_k} = Im{X1_k} + Re{X2_k} for k = 1 ... (N/2) - 1
    // Re (Y_-k} = Re{X1_k} + Im{X2_k} for k = 1 ... (N/2) - 1
    // Im {Y_-k} = Re{X2_k} - Im{X1_k} for k = 1 ... (N/2) - 1
    for (k = 1; k < myFFT->FFT_HALFSIZE; k++)
    {
        destArray1[k] = sourceArray1Real[k] - sourceArray2Imag[k];
        destArray2[k] = sourceArray1Imag[k] + sourceArray2Real[k];
        destArray1[myFFT->FFT_SIZE - k] = sourceArray1Real[k] + sourceArray2Imag[k];
        destArray2[myFFT->FFT_SIZE - k] = sourceArray2Real[k] - sourceArray1Imag[k];
    }

    // Compute IFFT
    ifftCompute(myFFT, &destArray1[0], &destArray2[0], &destArray1[0], &destArray2[0]);

}

