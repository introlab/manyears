/*******************************************************************************
 * ManyEars: Overall context - Header                                          *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Author: Francois Grondin                                                    *
 * Original Code: Jean-Marc Valin                                              *
 * Modified Code: Simon Briere                                                 *
 * Version: 1.1.0                                                              *
 * Date: July 1st, 2010                                                        *
 *******************************************************************************                               
 * LICENSE                                                                     *
 * --------------------------------------------------------------------------- *
 * This file is part of ManyEars. ManyEars is free software: you can           *
 * redistribute it and/or modify it under the terms of the GNU General Public  *
 * License as published by the Free Software Foundation, either version 3 of   *
 * the License, or (at your option) any later version. ManyEars is distributed *
 * in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even  *
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR         *
 * PURPOSE. See the GNU General Public License for more details. You should    *
 * have received a copy of the GNU General Public License along with           *
 * ManyEars. If not, see http://www.gnu.org/licenses/.                         *
 *                                                                             *
 *******************************************************************************
 *                                                                             *
 * ManyEars has been created and developped at:                                *
 *                                                                             *
 * IntroLab, Universite de Sherbrooke, Sherbrooke, Quebec, Canada              *
 * http://introlab.gel.usherbrooke.ca                                          *
 *                                                                             *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * The following articles relate to the ManyEars project:                      *
 *                                                                             *
 * S. Briere, J.-M. Valin, F. Michaud, Dominic Letourneau, Embedded Auditory   *
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

#ifndef OVERALLCONTEXT_H
#define OVERALLCONTEXT_H

// =============================================================================

#ifdef __cplusplus
extern "C" {
#endif

#include "hardware.h"
#include "parameters.h"
#include "Geometry/microphones.h"
#include "Preprocessing/preprocessor.h"
#include "Localisation/beamformer.h"
#include "Localisation/potentialSources.h"
#include "Tracking/mixture.h"
#include "Tracking/trackedSources.h"
#include "Separation/gss.h"
#include "Separation/separatedSources.h"
#include "Separation/postfilter.h"
#include "Separation/postfilteredSources.h"
#include "Postprocessing/postprocessor.h"
#include "Output/output.h"
#include "Output/outputChunk.h"


/*******************************************************************************
 * Structures                                                                  *
 ******************************************************************************/

    struct objOverall
    {

        // +-------------------------------------------------------------------+
        // | Objects                                                           |
        // +-------------------------------------------------------------------+

        struct ParametersStruct* myParameters;

        struct objMicrophones* myMicrophones;
        struct objPreprocessor* myPreprocessor;
        struct objBeamformer* myBeamformer;
        struct objMixture* myMixture;
        struct objGSS* myGSS;
        struct objPostfilter* myPostfilter;
        struct objPostprocessor* myPostprocessorSeparated;
        struct objPostprocessor* myPostprocessorPostfiltered;

        struct objPotentialSources* myPotentialSources;
        struct objTrackedSources* myTrackedSources;
        struct objSeparatedSources* mySeparatedSources;
        struct objPostfilteredSources* myPostfilteredSources;

        struct objOutput* myOutputSeparated;
        struct objOutput* myOutputPostfiltered;

        struct objOutputChunk* myOutputChunkSeparated;
        struct objOutputChunk* myOutputChunkPostfiltered;

    };

/*******************************************************************************
 * Prototypes                                                                  *
 ******************************************************************************/

    struct objOverall createEmptyOverallContext();

    void deleteOverallContext(struct objOverall myContext);

// =============================================================================


#ifdef __cplusplus
}
#endif

#endif
