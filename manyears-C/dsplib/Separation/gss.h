/*******************************************************************************
 * ManyEars: GSS - Header                                                      *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Author: François Grondin                                                    *
 * Original Code: Jean-Marc Valin                                              *
 * Modified Code: Simon Brière                                                 *
 * Version: 1.1.0                                                              *
 * Date: September 15th, 2010                                                  *
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

#ifndef GSS_H
#define GSS_H

#include <math.h>

#include "../hardware.h"
#include "../parameters.h"
#include "../Geometry/microphones.h"
#include "../Preprocessing/preprocessor.h"
#include "../Tracking/trackedSources.h"
#include "../Separation/separatedSources.h"
#include "../Utilities/dynamicMemory.h"
#include "../Utilities/idList.h"
#include "../Utilities/matrix.h"
#include "../Localisation/sphere.h"


/*******************************************************************************
 * Structure                                                                   *
 ******************************************************************************/

struct objGSS
{

    // +-------------------------------------------------------------------+
    // | Parameters                                                        |
    // +-------------------------------------------------------------------+

    // Distance of the source from the center of the cube (in meters)
    float GSS_SOURCEDISTANCE;

    // Sampling rate
    unsigned int GSS_FS;

    // Speed of sound
    float GSS_C;

    // Maximum number of separated sources
    unsigned int GSS_NBSOURCES;

    // Number of samples per frame
    unsigned int GSS_NFRAMES;

    // Half number of samples per frame
    unsigned int GSS_HALFNFRAMES;

    // Half number of samples per frame plus one
    unsigned int GSS_HALFNFRAMESPLUSONE;

    // Lambda
    float GSS_LAMBDA;

    // Mu
    float GSS_MU;

    // +-------------------------------------------------------------------+
    // | Variables                                                         |
    // +-------------------------------------------------------------------+

        // +---------------------------------------------------------------+
        // | General                                                       |
        // +---------------------------------------------------------------+

        // Defines the smallest delay from a source to a microphone
        // (this amount is removed from all delays to avoid delaying
        // signals by too much)
        unsigned int delayOffset;

        // Defines the range of delays
        unsigned int delayMinimum;
        unsigned int delayMaximum;

        // +---------------------------------------------------------------+
        // | Microphones                                                   |
        // +---------------------------------------------------------------+

        // Object for the microphones configuration
        struct objMicrophones* myMicrophones;

        // +---------------------------------------------------------------+
        // | Sources                                                       |
        // +---------------------------------------------------------------+

        // Current active sources
        struct objIdList sourcesIDNow;

        // Position of the active sources
        float** sourcesPosition;

        // Sources added since last frame
        struct objIdList sourcesIDAdded;

        // Sources deleted since last frame
        struct objIdList sourcesIDDeleted;

        // +---------------------------------------------------------------+
        // | Matrices                                                      |
        // +---------------------------------------------------------------+

        // x(k)
        struct objMatrix* x;

        // xH(k)
        struct objMatrix* xH;

        // y(k)
        struct objMatrix* y;
        struct objMatrix* yFull;

        // yH(k)
        struct objMatrix* yH;

        // Ryy(k)
        struct objMatrix* Ryy_E;

        // A(k)
        struct objMatrix* A;

        // AH(k)
        struct objMatrix* AH;

        // Alpha(k)
        struct objMatrix* alpha;
        struct objMatrix* alphatmp;

        // W^n(k)
        struct objMatrix* Wn;
        struct objMatrix* Wntmp1;

        // dJ1(k)
        struct objMatrix* dJ1;
        struct objMatrix* dJ1tmp1;
        struct objMatrix* dJ1tmp2;
        struct objMatrix* dJ1tmp3;
        struct objMatrix* dJ1tmp4;

        // dJ2(k)
        struct objMatrix* dJ2;
        struct objMatrix* dJ2tmp1;
        struct objMatrix* dJ2tmp2;

        // dJR'(k)
        struct objMatrix* dJR;

        // +---------------------------------------------------------------+
        // | Math acceleration                                             |
        // +---------------------------------------------------------------+

        float** cosTable;
        float** sinTable;

};

/*******************************************************************************
 * Prototypes                                                                  *
 ******************************************************************************/

void gssInit(struct objGSS* myGSS, struct ParametersStruct* myParameters, struct objMicrophones* myMicrophones);

void gssTerminate(struct objGSS* myGSS);

void gssAddSource(struct objGSS* myGSS, ID_TYPE newID, float positionX, float positionY, float positionZ);

void gssDeleteSource(struct objGSS* myGSS, ID_TYPE deleteID);

void gssUpdateSource(struct objGSS* myGSS, ID_TYPE sourceID, float positionX, float positionY, float positionZ);

void gssRefreshSources(struct objGSS* myGSS, struct objTrackedSources* myTrackedSources);

void gssProcess(struct objGSS* myGSS, struct objPreprocessor* myPreprocessor, struct objTrackedSources* myTrackedSources, struct objSeparatedSources* mySeparatedSources);

#endif
