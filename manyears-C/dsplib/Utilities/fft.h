/******************************************************************************* 
 * ManyEars: FFT - Header                                                      *
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

#ifndef FFT_H
#define FFT_H

#include <math.h>

#include "../hardware.h"
#include "../parameters.h"
#include "../Utilities/dynamicMemory.h"

/*******************************************************************************
 * Structure                                                                   *
 ******************************************************************************/

struct objFFT
{

    // +-------------------------------------------------------------------+
    // | Parameters                                                        |
    // +-------------------------------------------------------------------+

    // Number of points in the FFT (must be a power of 2)
    unsigned int FFT_SIZE;

    // Number of levels in the FFT (number of poinst = 2^(number of levels)
    unsigned int FFT_NBLEVELS;

    // Half the number of points in the FFT
    unsigned int FFT_HALFSIZE;

    // Inverse of the number of points in the FFT
    float FFT_SIZE_INV;

    // Number of elements in the array for SIMD group optimization
    unsigned int FFT_SIMD_GROUP;

    // Number of elements in the array for SIMD individual optimization
    unsigned int FFT_SIMD_INDIVIDUAL;

    // +-------------------------------------------------------------------+
    // | Variables                                                         |
    // +-------------------------------------------------------------------+

    // Define the FFT factor
    // The FFT factor is Wn(r) = exp(-j*2*pi*r/N) for r = 0 ... (N/2 - 1)
    // It is precomputed when the FFT module is initialized. Once this is done,
    // the results will be used for each FFT to be computed
    //
    // Size: [FFT_HALFSIZE]
    float* WnReal;
    float* WnImag;
    // Size: [FFT_SIMD_GROUP*4]
    float* simdWnReal;
    float* simdWnImag;

    // Define working array
    // This array is used during the FFT computations. It is set as a global
    // variable to avoid stacking and unstacking all these values when there is
    // a function call. This will speed up the computations.
    //
    // Size: [FFT_SIZE]
    float* workingArrayReal;
    float* workingArrayImag;

    // Define arrays to compute 2 FFTs with only one FFT for real signals
    //
    // Size: [FFT_SIZE]
    float* fftTwiceReal;
    float* fftTwiceRealFlipped;
    float* fftTwiceImag;
    float* fftTwiceImagFlipped;

    // Define array to compute 1 FFT for real signals
    //
    // Size: [FFT_SIZE]
    float* emptyArray;

    // Define array to trash unused result from IFFT
    //
    // Size: [FFT_SIZE]
    float* trashArray;

    // Define array to store the reverse bit order addressing
    //
    // Size: [FFT_SIZE]
    unsigned int* revBitOrderArray;

    // Define arrays to store the a indexes for SIMD optimization
    //
    // Size: [FFT_SIMD_GROUP]
    float** simdARealGroups;
    float** simdAImagGroups;
    float** simdBRealGroups;
    float** simdBImagGroups;
    float** simdRRealGroups;
    float** simdRImagGroups;
    // Size: [FFT_SIMD_INDIVIDUAL]
    float** simdAIndividual;
    float** simdBIndividual;

};

/*******************************************************************************
 * Prototypes                                                                  *
 ******************************************************************************/

void fftInit(struct objFFT* myFFT, struct ParametersStruct* myParameters, unsigned int size);

void fftTerminate(struct objFFT* myFFT);

void fftCompute(struct objFFT* myFFT, float* sourceArrayReal, float* sourceArrayImag, float* destArrayReal, float* destArrayImag);

void fftComputeOnce(struct objFFT* myFFT, float *sourceArray, float *destArrayReal, float *destArrayImag);

void fftComputeTwice(struct objFFT* myFFT, float *sourceArray1, float *sourceArray2, float *destArray1Real, float *destArray1Imag, float *destArray2Real, float *destArray2Imag);

void ifftCompute(struct objFFT* myFFT, float *sourceArrayReal, float *sourceArrayImag, float *destArrayReal, float *destArrayImag);

void ifftComputeOnce(struct objFFT* myFFT, float *sourceArray1Real, float *sourceArray1Imag, float *destArray1);

void ifftComputeTwice(struct objFFT* myFFT, float *sourceArray1Real, float *sourceArray1Imag, float *sourceArray2Real, float *sourceArray2Imag, float *destArray1, float *destArray2);

#endif
