/*******************************************************************************
 * ManyEars: Postprocessor - Header                                            *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Author: François Grondin                                                    *
 * Original Code: Jean-Marc Valin                                              *
 * Modified Code: Simon Brière                                                 *
 * Version: 1.1.0                                                              *
 * Date: September 6th, 2010                                                   *
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

#ifndef POSTPROCESSOR_H
#define POSTPROCESSOR_H

#include "../hardware.h"
#include "../parameters.h"
#include "../Tracking/trackedSources.h"
#include "../Separation/postfilteredSources.h"
#include "../Separation/separatedSources.h"
#include "../Utilities/dynamicMemory.h"
#include "../Utilities/fft.h"
#include "../Utilities/idManager.h"
#include "../Utilities/window.h"


/*******************************************************************************
 * Structures                                                                  *
 ******************************************************************************/

struct objPostprocessor
{

    // +-------------------------------------------------------------------+
    // | Parameters                                                        |
    // +-------------------------------------------------------------------+

    // Size of a frame
    unsigned int PP_FRAMESIZE;

    // Hop size
    unsigned int PP_HOPSIZE;

    // Size of the circular buffer
    unsigned int PP_BUFFERSIZE;

    // Maximum number of separated sources
    unsigned int PP_NSOURCES;

    // +-------------------------------------------------------------------+
    // | Variables                                                         |
    // +-------------------------------------------------------------------+

    // Create a FFT object
    struct objFFT* myFFT;

    // Window
    float* window;

    // Source IDs
    ID_TYPE* sourcesID;

    // Source position
    float** sourcesPosition;

    // Number of active sources
    unsigned int nActiveSources;

    // Circular buffer to perform overlap-add operations
    float** circularBuffer;
    unsigned int circularBufferIndexWrite;
    unsigned int circularBufferIndexRead;

    // Frames in frequency domain
    float** frameFrequencyReal;
    float** frameFrequencyImag;

    // Frames in time domain
    float** frameTime;

    // Windowed frame in time domain
    float** frameTimeWindowed;

};

/*******************************************************************************
 * Prototypes                                                                  *
 ******************************************************************************/

    void postprocessorInit(struct objPostprocessor *myPreprocessor, struct ParametersStruct *myParameters);

    void postprocessorTerminate(struct objPostprocessor *myPostprocessor);

    void postprocessorProcessFrameSeparated(struct objPostprocessor *myPostprocessor, struct objTrackedSources* myTrackedSources, struct objSeparatedSources* mySeparatedSources);

    void postprocessorProcessFramePostfiltered(struct objPostprocessor *myPostprocessor, struct objTrackedSources* myTrackedSources, struct objPostfilteredSources* myPostfilteredSources);

    void postprocessorExtractHop(struct objPostprocessor *myPostprocessor, signed int sourceID, float* outputArray);

#endif
