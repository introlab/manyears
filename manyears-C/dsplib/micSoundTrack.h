/******************************************************************************* 
 * ManyEars: Microphone Sound Track - Header                                   *
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

#ifndef MICSOUNDTRACK_H
#define MICSOUNDTRACK_H

// =============================================================================

#include "parameters.h"
#include "hardware.h"
#include "dynamicMemory.h"

/*******************************************************************************
 * Structures                                                                  *
 ******************************************************************************/

struct objMcra
{

    // +-------------------------------------------------------------------+
    // | Parameters                                                        |
    // +-------------------------------------------------------------------+

    // MCRA: Size of the window to smooth spectrum in frequency
    int MCRA_BSIZE;

    // MCRA: Window to smooth spectrum in frequency
    float* MCRA_WINDOW;

    // MCRA: Coefficient to smooth the spectrum in time (alphaS)
    float MCRA_ALPHAS;

    // MCRA: Probability smoothing parameter (alphaP)
    float MCRA_ALPHAP;

    // MCRA: Constant smoothing parameter (alphaD)
    float MCRA_ALPHAD;

    // MCRA: Maximum number of frames to refresh (L)
    int MCRA_L;

    // MCRA: Decision parameter (delta)
    float MCRA_DELTA;

    // Number of samples per frame (need to be a power of 2)
    int MICST_FRAMESIZE;

    // Half the number of samples per frame
    int MICST_HALFFRAMESIZE;

    // +-------------------------------------------------------------------+
    // | Variables                                                         |
    // +-------------------------------------------------------------------+

    // Window to smooth the spectrum in frequency
    // Size: MCRA_BSIZE
    float* b;                                // b(i) for all i

    // Power spectrum which is smoothed in frequency
    // Size: MICST_FRAMESIZE
    float* Sf;                              // Sf(k,l) for all k

    // Power spectrum which is smoothed in time
    // Size: MICST_FRAMESIZE
    float* S;                               // S(k,l) for all k

    // Previous power spectrum which was smooted in time
    // Size: MICST_FRAMESIZE
    float* S_prev;                          // S(k,l-1) for all k

    // Minimum power spectrum
    // Size: MICST_FRAMESIZE
    float* S_min;                           // Smin(k,l) for all k

    // Previous minimum power spectrum
    // Size: MICST_FRAMESIZE
    float* S_min_prev;                      // Smin(k,l-1) for all k

    // Temporary power spectrum
    // Size: MICST_FRAMESIZE
    float* S_tmp;                           // Stmp(k,l) for all k

    // Previous temporary power spectrum
    // Size: MICST_FRAMESIZE
    float* S_tmp_prev;                      // Stmp(k,l-1) for all k

    // Noise
    // Size: MICST_FRAMESIZE
    float* lambdaD_next;                    // lambdaD(k,l+1) for all k

    // Previous noise
    // Size: MICST_FRAMESIZE
    float* lambdaD;                         // lambdaD(k,l) for all k

    // Counts the number of frame
    unsigned int l;                         // l

    // Flag if this is the first group of frames to be processed
    unsigned char firstProcessing;

};

struct objMicST
{

    // +-------------------------------------------------------------------+
    // | Parameters                                                        |
    // +-------------------------------------------------------------------+

    // Number of samples per frame (need to be a power of 2)
    int MICST_FRAMESIZE;

    // Half the number of samples per frame
    int MICST_HALFFRAMESIZE;

    // Frame overlap (1 = 100%, 0.5 = 50%, 0.25 = 25%, ...)
    float MICST_OVERLAP;

    // Speed of sound (in meters / sec)
    float MICST_C;

    // Sampling rate (in samples / sec)
    int MICST_FS;

    // Adaptation rate (alphaD)
    float MICST_ALPHAD;

    // Reverberation decay (gamma)
    float MICST_GAMMA;

    // Level of reverberation (delta)
    float MICST_DELTA;

    // +-------------------------------------------------------------------+
    // | Variables                                                         |
    // +-------------------------------------------------------------------+

    // Microphone position on the cube (in meter)
    // Index 0 : x-coord, Index 1: y-coord, Index 2: z-coord
    float position[3];

    // Microphone current frame not windowed (in time domain)
    // Size: MICST_FRAMESIZE
    float* xtime;                                     // x_nw[n]

    // Microphone current frame windowed (in time domain)
    // Size: MICST_FRAMESIZE
    float* xtime_windowed;                            // x[n] = x_nw[n] * w[n]

    // Microphone current frame (in frequency domain)
    // Size: MICST_FRAMESIZE
    float* xfreqReal;                                 // Re{X[k]} for all k
    float* xfreqImag;                                 // Im{X[k]} for all k

    // Microphone current frame power (in frequency domain)
    // Size: MICST_FRAMESIZE
    float* xpower;                                    // |X[k]|^2 for all k

    // Microphone current frame half power (in frequency domain)
    // Size: MICST_HALFFRAMESIZE
    float *xhalfpower;                                // |X[k]|^2 for k = 1,...,N/2

    // Microphone previous frame power (in frequency domain)
    // Size: MICST_FRAMESIZE
    float* xpower_prev;                               // |X[k]_n-1|^2 for all k

    // Microphone previous frame half power (in frequency domain)
    // Size: MICST_HALFFRAMESIZE
    float *xhalfpower_prev;                           // |X[k]_n-1|^2 for k = 1,...,N/2

    // Weighting function
    // Size: MICST_FRAMESIZE
    float* zeta;                                      // zeta_n_i(k) for all k

    // Previous weighting function
    // Size: MICST_FRAMESIZE
    float* zeta_prev;                                 // zeta_n-1_i(k) for all k

    // Estimate of the a priori SNR
    // Size: MICST_FRAMESIZE
    float* ksi;                                       // ksi_n_i(k) for all k

    // Noise estimated with the MCRA method
    // Size: MICST_FRAMESIZE
    float* sigma;                                     // sigma_i_2(k) for all k

    // Reverberation term
    // Size: MICST_FRAMESIZE
    float* lambda;                                    // lambda_rev_n,i(k) for all k

    // Previous reverberation term
    // Size: MICST_FRAMESIZE
    float* lambda_prev;                               // lambda_rev_n-1,i(k) for all k

    // Weighted result (real part)
    // Size: MICST_FRAMESIZE
    float* weightedResultReal;                        // Real{ zeta(k) * X(k) / |X(k)| } fpr all k
    float* weightedResultImag;                        // Im{ zeta(k) * X(k) / |X(k)| } for all k

    // Gain to equalize microphones
    float gain;

    // Include the mcra object to estimate noise
    struct objMcra* noiseEstimator;

};

/*******************************************************************************
 * Prototypes                                                                  *
 ******************************************************************************/

void mcraInit(struct objMcra *myMCRA, struct ParametersStruct *myParameters);

void mcraTerminate(struct objMcra *myMCRA);

void mcraProcessFrame(struct objMcra *myMCRA, float *xPower, float *sigma);

void micstInit(struct objMicST *myMicST, struct ParametersStruct *myParameters, float *position, float gain);

void micstTerminate(struct objMicST *myMicST);

void micstProcessFrame(struct objMicST *myMicST);

// =============================================================================

#endif
