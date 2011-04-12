/******************************************************************************* 
 * ManyEars: MCRA noise estimator - Header                                     *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Author: François Grondin                                                    *
 * Original Code: Jean-Marc Valin                                              *
 * Modified Code: Simon Brière                                                 *
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

#ifndef MCRA_H
#define MCRA_H

#include <math.h>
#include <stdio.h>

#include "../hardware.h"
#include "../parameters.h"
#include "../Utilities/dynamicMemory.h"

/*******************************************************************************
 * Structures                                                                  *
 ******************************************************************************/

struct objMcra
{

    // +-------------------------------------------------------------------+
    // | Parameters                                                        |
    // +-------------------------------------------------------------------+

    // MCRA: Size of the window to smooth spectrum in frequency
    unsigned int MCRA_BSIZE;

    // MCRA: Window to smooth spectrum in frequency
    float* MCRA_WINDOW;

    // MCRA: Coefficient to smooth the spectrum in time (alphaS)
    float MCRA_ALPHAS;

    // MCRA: Probability smoothing parameter (alphaP)
    float MCRA_ALPHAP;

    // MCRA: Constant smoothing parameter (alphaD)
    float MCRA_ALPHAD;

    // MCRA: Maximum number of frames to refresh (L)
    unsigned int MCRA_L;

    // MCRA: Decision parameter (delta)
    float MCRA_DELTA;

    // Number of samples per frame (need to be a power of 2)
    unsigned int MICST_FRAMESIZE;

    // Half the number of samples per frame
    unsigned int MICST_HALFFRAMESIZE;

    // Long frame size
    unsigned int MICST_LONGFRAMESIZE;

    // Half the number of samples per long frame size
    unsigned int MICST_HALFLONGFRAMESIZE;


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

/*******************************************************************************
 * Prototypes                                                                  *
 ******************************************************************************/

void mcraInit(struct objMcra *myMCRA, struct ParametersStruct *myParameters, unsigned int size);

void mcraTerminate(struct objMcra *myMCRA);

void mcraClone(struct objMcra* myMCRADest, struct objMcra* myMCRASource);

void mcraProcessFrame(struct objMcra *myMCRA, float *xPower, float *sigma);

#endif
