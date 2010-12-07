/*******************************************************************************
 * ManyEars: Delays - Source code                                              *
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

#include "Localisation/delays.h"

/*******************************************************************************
 * delaysInit                                                                  *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myDelays        Object to be initialized                       *
 *              myMicrophones   Object to get configuration of the microphones *
 *              mySphere        Object to get observation points               *
 *              c               Parameter: speed of sound                      *
 *              Fs              Parameter: sampling rate                       *
 *              radius          Parameter: radius of the sphere (near-field)   *
 *                                                                             *
 * Outputs:     myMicrophones   Initialized object                             *
 *                                                                             *
 * Description: This function initializes the delays object and allocates      *
 *              memory that will be used later. The delays are generated in    *
 *              this function.                                                 *
 *                                                                             *
 ******************************************************************************/

void delaysInit(struct objDelays* myDelays, struct objMicrophones* myMicrophones, struct objSphere* mySphere, float c, unsigned int Fs, float radius)
{

    unsigned int indexPoint;
    unsigned int indexMic1;
    unsigned int indexMic2;
    unsigned int indexPair;

    float pointX, pointY, pointZ;
    float micX, micY, micZ;
    float distX, distY, distZ;
    float distance;
    float diffDist;

    float* distanceArray;

    /***************************************************************************
    * Step 1: Save parameters                                                  *
    ***************************************************************************/

    myDelays->DELAYS_RADIUS = radius;
    myDelays->C = c;
    myDelays->FS = Fs;

    /***************************************************************************
    * Step 2: Initialize objects                                               *
    ***************************************************************************/

    // +-------------------------------------------------------------------+
    // | Step A: Initialize the unit sphere                                |
    // +-------------------------------------------------------------------+

    myDelays->mySphere = (struct objSphere*) malloc(sizeof(struct objSphere));
    sphereClone(mySphere, myDelays->mySphere);

    // +-------------------------------------------------------------------+
    // | Step B: Initialize the mics pair                                  |
    // +-------------------------------------------------------------------+

    myDelays->myMicrophones = (struct objMicrophones*) malloc(sizeof(struct objMicrophones));
    microphonesClone(myMicrophones, myDelays->myMicrophones);

    /***************************************************************************
    * Step 3: Compute delays                                                   *
    ***************************************************************************/

    myDelays->nPoints = myDelays->mySphere->SPHERE_NUMBERPOINTS;
    myDelays->nMics = myDelays->myMicrophones->nMics;
    myDelays->nPairs = myDelays->myMicrophones->nPairs;
    myDelays->lookup = (signed int**) newTable2D(myDelays->nPoints, myDelays->nPairs, sizeof(signed int));

    distanceArray = (float*) newTable1D(myDelays->nMics, sizeof(float));

    for (indexPoint = 0; indexPoint < myDelays->nPoints; indexPoint++)
    {

        pointX = myDelays->DELAYS_RADIUS * myDelays->mySphere->spherePoints[indexPoint][0];
        pointY = myDelays->DELAYS_RADIUS * myDelays->mySphere->spherePoints[indexPoint][1];
        pointZ = myDelays->DELAYS_RADIUS * myDelays->mySphere->spherePoints[indexPoint][2];

        for (indexMic1 = 0; indexMic1 < myDelays->nMics; indexMic1++)
        {

            micX = microphonesGetPositionX(myDelays->myMicrophones, indexMic1);
            micY = microphonesGetPositionY(myDelays->myMicrophones, indexMic1);
            micZ = microphonesGetPositionZ(myDelays->myMicrophones, indexMic1);

            distX = pointX - micX;
            distY = pointY - micY;
            distZ = pointZ - micZ;

            distance = sqrtf(distX * distX + distY * distY + distZ * distZ);

            distanceArray[indexMic1] = distance;

        }

        for (indexMic1 = 0; indexMic1 < myDelays->nMics; indexMic1++)
        {
            for (indexMic2 = (indexMic1+1); indexMic2 < myDelays->nMics; indexMic2++)
            {

                indexPair = microphonesGetPair(myDelays->myMicrophones, indexMic1, indexMic2);

                diffDist = distanceArray[indexMic1] - distanceArray[indexMic2];

                myDelays->lookup[indexPoint][indexPair] = (int) (floor((myDelays->FS / myDelays->C) * diffDist + 0.5));

            }
        }

    }

    deleteTable1D((void*) distanceArray);

}

/*******************************************************************************
 * microphonesTerminate                                                        *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myDelays        The microphones object to be terminated        *
 *                                                                             *
 * Outputs:     (none)                                                         *
 *                                                                             *
 * Description: This function frees the dynamic memory that was allocated in   *
 *              the initialization of this object                              *
 *                                                                             *
 ******************************************************************************/

void delaysTerminate(struct objDelays* myDelays)
{

    // Free
    sphereTerminate(myDelays->mySphere);
    free((void*) myDelays->mySphere);
    microphonesTerminate(myDelays->myMicrophones);
    free((void*) myDelays->myMicrophones);
    deleteTable2D((void**) myDelays->lookup);

}

/*******************************************************************************
 * delaysGetMin                                                                *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myDelays        The microphones object to be used              *
 *                                                                             *
 * Outputs:     (signed int)    Returns the minimum delay value                *
 *                                                                             *
 * Description: This function returns the minimum delay value among all delays *
 *              generated.                                                     *
 *                                                                             *
 ******************************************************************************/

signed int delaysGetMin(struct objDelays* myDelays)
{

    int min;

    unsigned int indexPoint;
    unsigned int indexPair;

    min = myDelays->lookup[0][0];

    for (indexPoint = 0; indexPoint < myDelays->nPoints; indexPoint++)
    {
        for (indexPair = 0; indexPair < myDelays->nPairs; indexPair++)
        {

            if (myDelays->lookup[indexPoint][indexPair] < min)
            {
                min = myDelays->lookup[indexPoint][indexPair];
            }

        }
    }

    return min;

}

/*******************************************************************************
 * delaysGetMax                                                                *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myDelays        The microphones object to be used              *
 *                                                                             *
 * Outputs:     (signed int)    Returns the maximum delay value                *
 *                                                                             *
 * Description: This function returns the maximum delay value among all delays *
 *              generated.                                                     *
 *                                                                             *
 ******************************************************************************/

signed int delaysGetMax(struct objDelays* myDelays)
{

    int max;

    unsigned int indexPoint;
    unsigned int indexPair;

    max = myDelays->lookup[0][0];

    for (indexPoint = 0; indexPoint < myDelays->nPoints; indexPoint++)
    {
        for (indexPair = 0; indexPair < myDelays->nPairs; indexPair++)
        {

            if (myDelays->lookup[indexPoint][indexPair] > max)
            {
                max = myDelays->lookup[indexPoint][indexPair];
            }

        }
    }

    return max;

}

/*******************************************************************************
 * delaysGetFromPair                                                           *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myDelays        The microphones object to be used              *
 *              indexPoint      Index of the point on the sphere               *
 *              indexPair       Index of the pair of microphones               *
 *                                                                             *
 * Outputs:     (signed int)    Returns the delays that match the indexes of   *
 *                              the point and pair.                            *
 *                                                                             *
 * Description: This function returns the delays corresponding to the point    *
 *              and pair.                                                      *
 *                                                                             *
 ******************************************************************************/

INLINE_PREFIX signed int delaysGetFromPair(struct objDelays* myDelays, unsigned int indexPoint, unsigned int indexPair)
{
    return (myDelays->lookup[indexPoint][indexPair]);
}

/*******************************************************************************
 * delaysGetFromMics                                                           *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myDelays        The microphones object to be used              *
 *              indexPoint      Index of the point on the sphere               *
 *              indexMic1       Index of the first microphone                  *
 *              indexMic2       Index of the second microphone                 *
 *                                                                             *
 * Outputs:     (signed int)    Returns the delays that match the indexes of   *
 *                              the point and the microphone indexes.          *
 *                                                                             *
 * Description: This function returns the delays corresponding to the point    *
 *              and microphone indexes.                                        *
 *                                                                             *
 ******************************************************************************/

INLINE_PREFIX signed int delaysGetFromMics(struct objDelays* myDelays, unsigned int indexPoint, unsigned int indexMic1, unsigned int indexMic2)
{
    return (myDelays->lookup[indexPoint][microphonesGetPair(myDelays->myMicrophones, indexMic1, indexMic2)]);
}
