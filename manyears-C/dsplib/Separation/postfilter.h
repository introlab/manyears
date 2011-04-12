/*******************************************************************************
 * ManyEars: PostFilter - Header                                               *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Author: François Grondin                                                    *
 * Original Code: Jean-Marc Valin                                              *
 * Modified Code: Simon Brière                                                 *
 * Version: 1.1.0                                                              *
 * Date: September 16th, 2010                                                  *
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

#ifndef POSTFILTER_H
#define POSTFILTER_H

#include <stdio.h>

#include "../hardware.h"
#include "../parameters.h"
#include "../Preprocessing/mcra.h"
#include "../Preprocessing/preprocessor.h"
#include "../Separation/gss.h"
#include "../Separation/postfilteredSources.h"
#include "../Utilities/idList.h"
#include "../Utilities/linearCorrelation.h"
#include "../Utilities/transcendental.h"
#include "../Utilities/window.h"

/*******************************************************************************
 * Structure                                                                   *
 ******************************************************************************/

struct objPostfilter
{

    // +-------------------------------------------------------------------+
    // | Parameters                                                        |
    // +-------------------------------------------------------------------+

        // +---------------------------------------------------------------+
        // | General                                                       |
        // +---------------------------------------------------------------+

        // Maximum number of separated sources
        unsigned int POSTFILTER_NBSOURCES;

        // Number of samples per frame
        unsigned int POSTFILTER_NFRAMES;

        // Half the number of samples per frame
        unsigned int POSTFILTER_HALFNFRAMES;

        // Half the number of samples per frame plus one
        unsigned int POSTFILTER_HALFNFRAMESPLUSONE;

        // +---------------------------------------------------------------+
        // | Noise                                                         |
        // +---------------------------------------------------------------+

            // +-----------------------------------------------------------+
            // | Leakage                                                   |
            // +-----------------------------------------------------------+

            // AlphaS: smoothing
            float POSTFILTER_ALPHAS;

            // Eta: reducing factor
            float POSTFILTER_ETA;

            // +-----------------------------------------------------------+
            // | Reverberation                                             |
            // +-----------------------------------------------------------+

            // Gamma: Reverberation time
            float POSTFILTER_GAMMA;

            // Delta: Signal-to-reverberation ratio
            float POSTFILTER_DELTA;

        // +---------------------------------------------------------------+
        // | Speech presence                                               |
        // +---------------------------------------------------------------+

        // AlphapMin
        float POSTFILTER_ALPHAPMIN;

        // +---------------------------------------------------------------+
        // | Speech presence gain                                          |
        // +---------------------------------------------------------------+

        // Teta: Local
        float POSTFILTER_TETA_LOCAL;

        // Teta: Global
        float POSTFILTER_TETA_GLOBAL;

        // Teta: Frame
        float POSTFILTER_TETA_FRAME;

        // Alpha_zeta
        float POSTFILTER_ALPHAZETA;

        // Maximum a priori probability of speech absence
        float POSTFILTER_MAXQ;

        // Minimum gain allowed when speech is absent
        float POSTFILTER_GMIN;

        // Size of the local window
        unsigned int POSTFILTER_LOCALWINDOWSIZE;

        // Size of the global window
        unsigned int POSTFILTER_GLOBALWINDOWSIZE;

        // Size of the frame window
        int POSTFILTER_FRAMEWINDOWSIZE;

    // +-------------------------------------------------------------------+
    // | Variables                                                         |
    // +-------------------------------------------------------------------+

        // +---------------------------------------------------------------+
        // | Sources                                                       |
        // +---------------------------------------------------------------+

            // Separated sources
            float** YReal;
            float** YImag;
            float** YPower;

            // Filtered sources
            float** SReal;
            float** SImag;
            float** SPower;

            // IDs of sources
            struct objIdList sourcesIDNow;

            ID_TYPE* sourcesID;

            // Sources to be added
            struct objIdList sourcesIDAdded;

            // Sources to be deleted
            struct objIdList sourcesIDDeleted;

        // +---------------------------------------------------------------+
        // | Noise                                                         |
        // +---------------------------------------------------------------+

            // +-----------------------------------------------------------+
            // | Static                                                    |
            // +-----------------------------------------------------------+

            struct objMcra* mcra;
            float** lambdaStat;

            // +-----------------------------------------------------------+
            // | Leakage                                                   |
            // +-----------------------------------------------------------+

            float** Z;
            float** lambdaLeak;

            // +-----------------------------------------------------------+
            // | Reverberation                                             |
            // +-----------------------------------------------------------+

            float** lambdaRev;

            // +-----------------------------------------------------------+
            // | Total                                                     |
            // +-----------------------------------------------------------+

            float** lambda;

        // +---------------------------------------------------------------+
        // | Speech presence gain                                          |
        // +---------------------------------------------------------------+

            // Transcendental
            struct objTranscendental transcendental;

            // gamma(k,l)
            float** gamma;

            // xi(k,l)
            float** xi;

            // alphaP(k,l)
            float** alphaP;

            // v(k,l)
            float** v;

            // G_H1(k,l)
            float** GH1;

        // +---------------------------------------------------------------+
        // | Speech presence uncertainty                                   |
        // +---------------------------------------------------------------+

            // +-----------------------------------------------------------+
            // | Windows                                                   |
            // +-----------------------------------------------------------+

            // Local
            struct objLinearCorrelation localWindow;

            // Global
            struct objLinearCorrelation globalWindow;

            // Frame
            struct objLinearCorrelation frameWindow;

            // +-----------------------------------------------------------+
            // | Result after windowing                                    |
            // +-----------------------------------------------------------+

            // Local
            float* localResultCropped;

            // Global
            float* globalResultCropped;

            // Frame
            float* frameResultCropped;

            // +-----------------------------------------------------------+
            // | A priori SNR                                              |
            // +-----------------------------------------------------------+

            // Local
            float** localZeta;

            // Global
            float** globalZeta;

            // Frame
            float** frameZeta;

            // +-----------------------------------------------------------+
            // | Speech presence probabilities                             |
            // +-----------------------------------------------------------+

            // Local
            float** localP;

            // Global
            float** globalP;

            // Frame
            float** frameP;

            // A priori probability of speech presence
            float** p;

            // A priori probability of speech absence
            float** q;

            // +-----------------------------------------------------------+
            // | Gain                                                      |
            // +-----------------------------------------------------------+

            // Gain
            float** G;

};

void postfilterInit(struct objPostfilter* myPostfilter, struct ParametersStruct* myParameters);

void postfilterTerminate(struct objPostfilter* myPostfilter);

void postfilterAddSource(struct objPostfilter* myPostfilter, struct objPreprocessor* myPreprocessor, ID_TYPE newID);

void postfilterDeleteSource(struct objPostfilter* myPostfilter, ID_TYPE deleteID);

void postfilterProcess(struct objPostfilter* myPostfilter, struct objSeparatedSources* mySeparatedSources, struct objPreprocessor* myPreprocessor, struct objPostfilteredSources* myPostfilteredSources);

void postfilterAppendStream(struct objPostfilter* myPostfilter, FILE** filePtr);

#endif
