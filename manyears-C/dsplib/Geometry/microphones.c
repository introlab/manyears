/*******************************************************************************
 * ManyEars: Microphones - Source code                                         *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Author: François Grondin                                                    *
 * Original Code: Jean-Marc Valin                                              *
 * Modified Code: Simon Brière                                                 *
 * Version: 1.2.0                                                              *
 * Date: November 10th, 2010                                                   *
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

#include "Geometry/microphones.h"

/*******************************************************************************
 * microphonesInit                                                             *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myMicrophones   Object to be initialized                       *
 *              nMics           Number of microphones in the array             *
 *                                                                             *
 * Outputs:     myMicrophones   Initialized object                             *
 *                                                                             *
 * Description: This function initializes the microphones object and allocates *
 *              memory that will be used later.                                *
 *                                                                             *
 ******************************************************************************/

void microphonesInit(struct objMicrophones* myMicrophones, unsigned int nMics)
{

    unsigned int indexMic1;
    unsigned int indexMic2;

    unsigned int indexPair;

    /***************************************************************************
    * Step 1: Load parameters                                                  *
    ***************************************************************************/

    myMicrophones->nMics = nMics;
    myMicrophones->nPairs = (myMicrophones->nMics * (myMicrophones->nMics - 1)) / 2;

    /***************************************************************************
    * Step 2: Create matrix                                                    *
    ***************************************************************************/

    // +-----------------------------------------------------------------------+
    // | Step A: Microphone positions                                          |
    // +-----------------------------------------------------------------------+

    myMicrophones->micsPosition = (float**) newTable2D(myMicrophones->nMics, 3, sizeof(float));

    // +-----------------------------------------------------------------------+
    // | Step B: Microphone gains                                              |
    // +-----------------------------------------------------------------------+

    myMicrophones->micsGain = (float*) newTable1D(myMicrophones->nMics, sizeof(float));

    // +-----------------------------------------------------------------------+
    // | Step C: Forward matrix                                                |
    // +-----------------------------------------------------------------------+

    myMicrophones->mappingMatrix = (signed int**) newTable2D(myMicrophones->nMics, myMicrophones->nMics, sizeof(signed int));

    indexPair = 0;

    for (indexMic1 = 0; indexMic1 < myMicrophones->nMics; indexMic1++)
    {
        for (indexMic2 = (indexMic1 + 1); indexMic2 < myMicrophones->nMics; indexMic2++)
        {

            myMicrophones->mappingMatrix[indexMic1][indexMic2] = indexPair;
            myMicrophones->mappingMatrix[indexMic2][indexMic1] = indexPair;
            indexPair++;

        }

        myMicrophones->mappingMatrix[indexMic1][indexMic1] = -1;

    }

    // +-----------------------------------------------------------------------+
    // | Step D: Inverse matrix                                                |
    // +-----------------------------------------------------------------------+

    myMicrophones->inverseMappingMatrix = (signed int**) newTable2D(myMicrophones->nPairs, 2, sizeof(signed int));

    indexPair = 0;

    for (indexMic1 = 0; indexMic1 < myMicrophones->nMics; indexMic1++)
    {
        for (indexMic2 = (indexMic1 + 1); indexMic2 < myMicrophones->nMics; indexMic2++)
        {

            myMicrophones->inverseMappingMatrix[indexPair][0] = indexMic1;
            myMicrophones->inverseMappingMatrix[indexPair][1] = indexMic2;

            indexPair++;

        }
    }

}

/*******************************************************************************
 * microphonesTerminate                                                        *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myMicrophones       The microphones object to be terminated    *
 *                                                                             *
 * Outputs:     (none)                                                         *
 *                                                                             *
 * Description: This function frees the dynamic memory that was allocated in   *
 *              the initialization of this object                              *
 *                                                                             *
 ******************************************************************************/

void microphonesTerminate(struct objMicrophones* myMicrophones)
{

    // Free
    deleteTable2D((void**) myMicrophones->micsPosition);
    deleteTable1D((void*) myMicrophones->micsGain);
    deleteTable2D((void**) myMicrophones->mappingMatrix);
    deleteTable2D((void**) myMicrophones->inverseMappingMatrix);

}

/*******************************************************************************
 * microphonesAdd                                                              *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myMicrophones       The microphones object to be used          *
 *              indexMic            The index of the microphone to be added    *
 *              x                   The x-coordinate position of the mic       *
 *              y                   The y-coordinate position of the mic       *
 *              z                   The z-coordinate position of the mic       *
 *              gain                The gain to be applied on the mic          *
 *                                                                             *
 * Outputs:     myMicrophones       The updated microphones object             *
 *                                                                             *
 * Description: This function adds a microphone in the microphone array. This  *
 *              function must be called prior to using this object with other  *
 *              components of the system.                                      *
 *                                                                             *
 ******************************************************************************/

void microphonesAdd(struct objMicrophones* myMicrophones, unsigned int indexMic, float x, float y, float z, float gain)
{

    myMicrophones->micsPosition[indexMic][0] = x;
    myMicrophones->micsPosition[indexMic][1] = y;
    myMicrophones->micsPosition[indexMic][2] = z;
    myMicrophones->micsGain[indexMic] = gain;

}

/*******************************************************************************
 * microphonesClone                                                            *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myMicrophonesSource The source microphones object              *
 *                                                                             *
 * Outputs:     myMicrophonesDest   The destination microphones object         *
 *                                                                             *
 * Description: This function clones the source microphones object in the      *
 *              destination microphones object.                                *
 *                                                                             *
 ******************************************************************************/

void microphonesClone(struct objMicrophones* myMicrophonesSource, struct objMicrophones* myMicrophonesDest)
{


    unsigned int indexMic1;
    unsigned int indexMic2;
    unsigned int indexPair;

    /***************************************************************************
    * Step 1: Copy parameters                                                  *
    ***************************************************************************/

    myMicrophonesDest->nMics = myMicrophonesSource->nMics;
    myMicrophonesDest->nPairs = myMicrophonesSource->nPairs;

    /***************************************************************************
    * Step 2: Copy matrix                                                      *
    ***************************************************************************/

    // +-----------------------------------------------------------------------+
    // | Step A: Microphone positions                                          |
    // +-----------------------------------------------------------------------+

    myMicrophonesDest->micsPosition = (float**) newTable2D(myMicrophonesDest->nMics, 3, sizeof(float));

    for (indexMic1 = 0; indexMic1 < myMicrophonesDest->nMics; indexMic1++)
    {
        myMicrophonesDest->micsPosition[indexMic1][0] = myMicrophonesSource->micsPosition[indexMic1][0];
        myMicrophonesDest->micsPosition[indexMic1][1] = myMicrophonesSource->micsPosition[indexMic1][1];
        myMicrophonesDest->micsPosition[indexMic1][2] = myMicrophonesSource->micsPosition[indexMic1][2];
    }

    // +-----------------------------------------------------------------------+
    // | Step B: Microphone gains                                              |
    // +-----------------------------------------------------------------------+

    myMicrophonesDest->micsGain = (float*) newTable1D(myMicrophonesDest->nMics, sizeof(float));

    for (indexMic1 = 0; indexMic1 < myMicrophonesDest->nMics; indexMic1++)
    {
        myMicrophonesDest->micsGain[indexMic1] = myMicrophonesSource->micsGain[indexMic1];
    }

    // +-----------------------------------------------------------------------+
    // | Step C: Forward matrix                                                |
    // +-----------------------------------------------------------------------+

    myMicrophonesDest->mappingMatrix = (signed int**) newTable2D(myMicrophonesDest->nMics, myMicrophonesDest->nMics, sizeof(float));

    for (indexMic1 = 0; indexMic1 < myMicrophonesDest->nMics; indexMic1++)
    {
        for (indexMic2 = 0; indexMic2 < myMicrophonesDest->nMics; indexMic2++)
        {
            myMicrophonesDest->mappingMatrix[indexMic1][indexMic2] = myMicrophonesSource->mappingMatrix[indexMic1][indexMic2];
        }
    }

    // +-----------------------------------------------------------------------+
    // | Step C: Inverse matrix                                                |
    // +-----------------------------------------------------------------------+

    myMicrophonesDest->inverseMappingMatrix = (signed int**) newTable2D(myMicrophonesDest->nPairs, 2, sizeof(signed int));

    for (indexPair = 0; indexPair < myMicrophonesDest->nPairs; indexPair++)
    {
        myMicrophonesDest->inverseMappingMatrix[indexPair][0] = myMicrophonesSource->inverseMappingMatrix[indexPair][0];
        myMicrophonesDest->inverseMappingMatrix[indexPair][1] = myMicrophonesSource->inverseMappingMatrix[indexPair][1];
    }

}

/*******************************************************************************
 * microphonesGetPair                                                          *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myMicrophones       The microphones object to be used          *
 *              indexMic1           The index of the first microphone          *
 *              indexMic2           The index of the second microphone         *
 *                                                                             *
 * Outputs:     (signed int)        The pair index                             *
 *                                                                             *
 * Description: This function returns the pair index that matches the two      *
 *              microphones indexes provided.                                  *
 *                                                                             *
 *              Ex:                                                            *
 *                                                                             *
 *              For 4 microphones                                              *
 *                                                                             *
 *              Mic1    Mic2    Pair                                           *
 *              0       1       0                                              *
 *              0       2       1                                              *
 *              0       3       2                                              *
 *              1       2       3                                              *
 *              1       3       4                                              *
 *              2       3       5                                              *
 *                                                                             *
 *              For instance, if indexMic1 = 0, indexMic2 = 3, then returns 2  *
 *                                                                             *
 ******************************************************************************/

INLINE_PREFIX signed int microphonesGetPair(struct objMicrophones* myMicrophones, unsigned int indexMic1, unsigned int indexMic2)
{
    return myMicrophones->mappingMatrix[indexMic1][indexMic2];
}

/*******************************************************************************
 * microphonesGetMic                                                           *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myMicrophones       The microphones object to be used          *
 *              indexPair           The index of the pair                      *
 *              pairElement         The microphone to be returned (0 = first,  *
 *                                  1 = second)                                *
 *                                                                             *
 * Outputs:     (signed int)        The microphone index                       *
 *                                                                             *
 * Description: This function returns the microphone index that comes from the *
 *              pair index provided.                                           *
 *                                                                             *
 *              Ex:                                                            *
 *                                                                             *
 *              For 4 microphones                                              *
 *                                                                             *
 *              Mic1    Mic2    Pair                                           *
 *              0       1       0                                              *
 *              0       2       1                                              *
 *              0       3       2                                              *
 *              1       2       3                                              *
 *              1       3       4                                              *
 *              2       3       5                                              *
 *                                                                             *
 *              For instance, if indexPair = 4, pairElement = 1, then          *
 *              returns 3                                                      *
 *                                                                             *
 ******************************************************************************/

INLINE_PREFIX signed int microphonesGetMic(struct objMicrophones* myMicrophones, unsigned int indexPair, unsigned int pairElement)
{
    return myMicrophones->inverseMappingMatrix[indexPair][pairElement];
}

/*******************************************************************************
 * microphonesGetPosition                                                      *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myMicrophones       The microphones object to be used          *
 *              indexMic            The index of the microphone                *
 *                                                                             *
 * Outputs:     (float*)            The pointer to the array with position     *
 *                                                                             *
 * Description: This function returns the pointer to the array that contains   *
 *              the x,y and z position of the microphone which index is        *
 *              indexMic                                                       *
 ******************************************************************************/

INLINE_PREFIX float* microphonesGetPosition(struct objMicrophones* myMicrophones, unsigned int indexMic)
{
    return myMicrophones->micsPosition[indexMic];
}

/*******************************************************************************
 * microphonesGetPositionX                                                     *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myMicrophones       The microphones object to be used          *
 *              indexMic            The index of the microphone                *
 *                                                                             *
 * Outputs:     (float)             The x position of the microphone           *
 *                                                                             *
 * Description: This function returns the x of the microphone which index is   *
 *              indexMic                                                       *
 ******************************************************************************/

INLINE_PREFIX float microphonesGetPositionX(struct objMicrophones* myMicrophones, unsigned int indexMic)
{
    return myMicrophones->micsPosition[indexMic][0];
}

/*******************************************************************************
 * microphonesGetPositionY                                                     *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myMicrophones       The microphones object to be used          *
 *              indexMic            The index of the microphone                *
 *                                                                             *
 * Outputs:     (float)             The y position of the microphone           *
 *                                                                             *
 * Description: This function returns the y of the microphone which index is   *
 *              indexMic                                                       *
 ******************************************************************************/

INLINE_PREFIX float microphonesGetPositionY(struct objMicrophones* myMicrophones, unsigned int indexMic)
{
    return myMicrophones->micsPosition[indexMic][1];
}

/*******************************************************************************
 * microphonesGetPositionZ                                                     *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myMicrophones       The microphones object to be used          *
 *              indexMic            The index of the microphone                *
 *                                                                             *
 * Outputs:     (float)             The z position of the microphone           *
 *                                                                             *
 * Description: This function returns the z of the microphone which index is   *
 *              indexMic                                                       *
 ******************************************************************************/

INLINE_PREFIX float microphonesGetPositionZ(struct objMicrophones* myMicrophones, unsigned int indexMic)
{
    return myMicrophones->micsPosition[indexMic][2];
}

/*******************************************************************************
 * microphonesGetPositionGain                                                  *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myMicrophones       The microphones object to be used          *
 *              indexMic            The index of the microphone                *
 *                                                                             *
 * Outputs:     (float)             The gain of the microphone                 *
 *                                                                             *
 * Description: This function returns the gain of the microphone which index   *
 *              is indexMic                                                    *
 ******************************************************************************/

INLINE_PREFIX float microphonesGetGain(struct objMicrophones* myMicrophones, unsigned int indexMic)
{
    return myMicrophones->micsGain[indexMic];
}
