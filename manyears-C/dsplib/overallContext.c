/*******************************************************************************
 * ManyEars: Overall context - Header                                          *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Author: François Grondin                                                    *
 * Original Code: Jean-Marc Valin                                              *
 * Modified Code: Simon Brière                                                 *
 * Version: 1.1.0                                                              *
 * Date: July 1st, 2010                                                        *
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

#include "overallContext.h"

/*******************************************************************************
 * createEmptyOverallContext                                                   *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      (none)                                                         *
 *                                                                             *
 * Outputs:     (objOverall)    Structure with all the allocated objects for   *
 *                              processing                                     *
 *                                                                             *
 * Description: This function creates a structure with all the objects that    *
 *              need to be used to perform operations in the library.          *
 *                                                                             *
 ******************************************************************************/

struct objOverall createEmptyOverallContext()
{

    struct objOverall tmp;

    tmp.myMicrophones = (struct objMicrophones*) malloc(sizeof(struct objMicrophones));
    tmp.myPreprocessor = (struct objPreprocessor*) malloc(sizeof(struct objPreprocessor));
    tmp.myBeamformer = (struct objBeamformer*) malloc(sizeof(struct objBeamformer));
    tmp.myMixture = (struct objMixture*) malloc(sizeof(struct objMixture));
    tmp.myGSS = (struct objGSS*) malloc(sizeof(struct objGSS));
    tmp.myPostfilter = (struct objPostfilter*) malloc(sizeof(struct objPostfilter));
    tmp.myPostprocessorSeparated = (struct objPostprocessor*) malloc(sizeof(struct objPostprocessor));
    tmp.myPostprocessorPostfiltered = (struct objPostprocessor*) malloc(sizeof(struct objPostprocessor));

    tmp.myPotentialSources = (struct objPotentialSources*) malloc(sizeof(struct objPotentialSources));
    tmp.myTrackedSources = (struct objTrackedSources*) malloc(sizeof(struct objTrackedSources));
    tmp.mySeparatedSources = (struct objSeparatedSources*) malloc(sizeof(struct objSeparatedSources));
    tmp.myPostfilteredSources = (struct objPostfilteredSources*) malloc(sizeof(struct objPostfilteredSources));

    tmp.myOutputSeparated = (struct objOutput*) malloc(sizeof(struct objOutput));
    tmp.myOutputPostfiltered = (struct objOutput*) malloc(sizeof(struct objOutput));

    tmp.myOutputChunkSeparated = (struct objOutputChunk*) malloc(sizeof(struct objOutputChunk));
    tmp.myOutputChunkPostfiltered = (struct objOutputChunk*) malloc(sizeof(struct objOutputChunk));

    tmp.myParameters = (struct ParametersStruct*) malloc(sizeof(struct ParametersStruct));

    return tmp;

}

/*******************************************************************************
 * deleteOverallContext                                                        *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myContext       The context to be deleted                      *
 *                                                                             *
 * Outputs:     (none)                                                         *
 *                                                                             *
 * Description: This function frees the memory used by the objects.            *
 *                                                                             *
 ******************************************************************************/

void deleteOverallContext(struct objOverall myContext)
{

    free((void*) myContext.myMicrophones);
    free((void*) myContext.myPreprocessor);
    free((void*) myContext.myBeamformer);
    free((void*) myContext.myMixture);
    free((void*) myContext.myGSS);
    free((void*) myContext.myPostfilter);
    free((void*) myContext.myPostprocessorSeparated);
    free((void*) myContext.myPostprocessorPostfiltered);

    free((void*) myContext.myPotentialSources);
    free((void*) myContext.myTrackedSources);
    free((void*) myContext.mySeparatedSources);
    free((void*) myContext.myPostfilteredSources);

    free((void*) myContext.myOutputSeparated);
    free((void*) myContext.myOutputPostfiltered);

    free((void*) myContext.myOutputChunkSeparated);
    free((void*) myContext.myOutputChunkPostfiltered);

    free((void*) myContext.myParameters);

}
