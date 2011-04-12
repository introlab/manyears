/*******************************************************************************
 * ManyEars: Separated Sources - Source Code                                   *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Author: François Grondin                                                    *
 * Original Code: Jean-Marc Valin                                              *
 * Modified Code: Simon Brière                                                 *
 * Version: 1.1.0                                                              *
 * Date: September 9th, 2010                                                   *
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

#include "separatedSources.h"

/*******************************************************************************
 * separatedSourcesInit                                                        *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      mySeparatedSources                                             *
 *                              The separated sources object to be initialized *
 *              myParameters    List of parameters that is used to configure   *
 *                              the object                                     *
 *                                                                             *
 * Outputs:     mySeparatedSources                                             *
 *                              The separated sources object to be initialized *
 *                                                                             *
 * Description: This function initializes the separated sources object         *
 *                                                                             *
 ******************************************************************************/

void separatedSourcesInit(struct objSeparatedSources *mySeparatedSources, struct ParametersStruct *myParameters)
{

    /***************************************************************************
    * Step 1: Load parameters                                                  *
    ***************************************************************************/

    mySeparatedSources->GSS_NBSOURCES = (unsigned int) myParameters->P_GEN_DYNSOURCES;
    mySeparatedSources->GSS_LONGFRAMESIZE = (unsigned int) GLOBAL_LONGFRAMESIZE;

    /***************************************************************************
    * Step 2: Create arrays                                                    *
    ***************************************************************************/

    idListInit(&mySeparatedSources->sourcesID, mySeparatedSources->GSS_NBSOURCES);

    mySeparatedSources->sourcesFramesReal = (float**) newTable2D(mySeparatedSources->GSS_NBSOURCES,mySeparatedSources->GSS_LONGFRAMESIZE,sizeof(float));
    mySeparatedSources->sourcesFramesImag = (float**) newTable2D(mySeparatedSources->GSS_NBSOURCES,mySeparatedSources->GSS_LONGFRAMESIZE,sizeof(float));   

}

/*******************************************************************************
 * separatedSourcesGetMaxNumberSources                                         *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      mySeparatedSources                                             *
 *                              The separated sources object to be used        *
 *                                                                             *
 * Outputs:     unsigned int    The max number of separated sources            *
 *                                                                             *
 * Description: This function returns the max number of separated sources      *
 *                                                                             *
 ******************************************************************************/

unsigned int separatedSourcesGetMaxNumberSources(struct objSeparatedSources *mySeparatedSources)
{
    return (idListGetMaxNElements(&mySeparatedSources->sourcesID));
}

/*******************************************************************************
 * separatedSourcesGetNumberSeparatedSources                                   *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      mySeparatedSources                                             *
 *                              The separated sources object to be used        *
 *                                                                             *
 * Outputs:     unsigned int    The number of sources currently separated      *
 *                                                                             *
 * Description: This function returns the number of sources currently          *
 *              separated                                                      *
 *                                                                             *
 ******************************************************************************/

unsigned int separatedSourcesGetNumberSeparatedSources(struct objSeparatedSources *mySeparatedSources)
{
    return (idListGetNElements(&mySeparatedSources->sourcesID));
}

/*******************************************************************************
 * separatedSourcesGetID                                                       *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      separatedSourcesGetID                                          *
 *                              The separated sources object to be used        *
 *              sourceIndex     The source index                               *
 *                                                                             *
 * Outputs:     signed int      The value of the ID of the source              *
 *                              which index is given                           *
 *                                                                             *
 * Description: This function returns the ID of the source which index         *
 *              is given by sourceIndex                                        *
 *                                                                             *
 ******************************************************************************/

ID_TYPE separatedSourcesGetID(struct objSeparatedSources *mySeparatedSources, unsigned int sourceIndex)
{
    return (idListGetID(&mySeparatedSources->sourcesID, sourceIndex));
}

/*******************************************************************************
 * separatedSourcesGetFrameReal                                                *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      mySeparatedSources                                             *
 *                              The separated sources object to be used        *
 *              sourceIndex     The source index                               *
 *              k               The frame index                                *
 *                                                                             *
 * Outputs:     float           The real value of the kth element of the frame *
 *                              which source index is given                    *
 *                                                                             *
 * Description: This function returns the real value of the kth element of the *
 *              frame which source index is given                              *
 *                                                                             *
 ******************************************************************************/

float separatedSourcesGetFrameReal(struct objSeparatedSources *mySeparatedSources, unsigned int sourceIndex, unsigned int k)
{
    return (mySeparatedSources->sourcesFramesReal[sourceIndex][k]);
}

/*******************************************************************************
 * separatedSourcesGetFrameImag                                                *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      mySeparatedSources                                             *
 *                              The separated sources object to be used        *
 *              sourceIndex     The source index                               *
 *              k               The frame index                                *
 *                                                                             *
 * Outputs:     float           The imag value of the kth element of the frame *
 *                              which source index is given                    *
 *                                                                             *
 * Description: This function returns the imag value of the kth element of the *
 *              frame which source index is given                              *
 *                                                                             *
 ******************************************************************************/

float separatedSourcesGetFrameImag(struct objSeparatedSources *mySeparatedSources, unsigned int sourceIndex, unsigned int k)
{
    return (mySeparatedSources->sourcesFramesImag[sourceIndex][k]);
}

float separatedSourcesGetFrameRealId(struct objSeparatedSources *mySeparatedSources, ID_TYPE id, unsigned int k)
{

    unsigned int sourceIndex;

    sourceIndex = idListGetIndex(&mySeparatedSources->sourcesID, id);

    return (mySeparatedSources->sourcesFramesReal[sourceIndex][k]);

}

float separatedSourcesGetFrameImagId(struct objSeparatedSources *mySeparatedSources, ID_TYPE id, unsigned int k)
{

    unsigned int sourceIndex;

    sourceIndex = idListGetIndex(&mySeparatedSources->sourcesID, id);

    return (mySeparatedSources->sourcesFramesImag[sourceIndex][k]);

}

/*******************************************************************************
 * separatedSourcesTerminate                                                   *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      mySeparatedSources                                             *
 *                             The separated sources object to be terminated   *
 *                                                                             *
 * Outputs:     (none)                                                         *
 *                                                                             *
 * Description: This function terminates the separated sources object          *
 *                                                                             *
 ******************************************************************************/

void separatedSourcesTerminate(struct objSeparatedSources *mySeparatedSources)
{
    // Free memory
    idListTerminate(&mySeparatedSources->sourcesID);
    deleteTable2D((void**) mySeparatedSources->sourcesFramesReal);
    deleteTable2D((void**) mySeparatedSources->sourcesFramesImag);
}
