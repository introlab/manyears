/******************************************************************************* 
 * ManyEars: Beamformer - Source code                                          *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Author: François Grondin                                                    *
 * Original Code: Jean-Marc Valin                                              *
 * Modified Code: Simon Brière                                                 *
 * Version: 1.1.0                                                              *
 * Date: June 29th, 2010                                                       *
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

#include "Localisation/beamformer.h"

/*******************************************************************************
 * Compatibility issues                                                        *
 ******************************************************************************/

// In some environment, the constant PI is not define with the math.h library
#ifndef M_PI
	#define M_PI	3.1415926535897932384626433832795f
#endif


/*******************************************************************************
 * beamformerInit                                                              *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myBeamformer    Object to be initialized                       *
 *              myParameters    List of parameters that is used to configure   *
 *                              the object.                                    *
 *              myMicrophones   The microphones configuration                  *
 *                                                                             *
 * Outputs:     myBeamformer    Initialized object                             *
 *                                                                             *
 * Description: This function initializes the beamformer object and allocates  *
 *              memory that will be used later.                                *
 *                                                                             *
 ******************************************************************************/

void beamformerInit(struct objBeamformer *myBeamformer, struct ParametersStruct *myParameters, struct objMicrophones* myMicrophones)
{

    /***************************************************************************
    * Step 1: Save parameters                                                  *
    ***************************************************************************/

    myBeamformer->BF_SPHERENBLEVELS = GLOBAL_SPHERE_NUMBERLEVELS;
    myBeamformer->BF_MAXSOURCES = myParameters->P_BF_MAXSOURCES;
    myBeamformer->BF_FILTERRANGE = myParameters->P_BF_FILTERRANGE;
    myBeamformer->BF_RESETRANGE = myParameters->P_BF_RESETRANGE;
    myBeamformer->BF_ET = myParameters->P_BF_ET;

    /***************************************************************************
    * Step 2: Create objects                                                   *
    ***************************************************************************/

    // +-----------------------------------------------------------------------+
    // | Step A: Microphones                                                   |
    // +-----------------------------------------------------------------------+

    myBeamformer->myMicrophones = (struct objMicrophones*) malloc(sizeof(struct objMicrophones));
    microphonesClone(myMicrophones, myBeamformer->myMicrophones);

    // +-----------------------------------------------------------------------+
    // | Step B: Sphere                                                        |
    // +-----------------------------------------------------------------------+

    myBeamformer->mySphere = (struct objSphere*) malloc(sizeof(struct objSphere));
    sphereInit(myBeamformer->mySphere, myBeamformer->BF_SPHERENBLEVELS);

    // +-----------------------------------------------------------------------+
    // | Step C: Delays                                                        |
    // +-----------------------------------------------------------------------+

    myBeamformer->myDelays = (struct objDelays*) malloc(sizeof(struct objDelays));
    delaysInit(myBeamformer->myDelays,myBeamformer->myMicrophones, myBeamformer->mySphere, GLOBAL_C, GLOBAL_FS, 1.5);

    // +-----------------------------------------------------------------------+
    // | Step D: Rij                                                           |
    // +-----------------------------------------------------------------------+

    myBeamformer->myRij = (struct objRij*) malloc(sizeof(struct objRij));
    rijInit(myBeamformer->myRij, myParameters, myBeamformer->myMicrophones, myBeamformer->myDelays, GLOBAL_FRAMESIZE, myParameters->P_BF_FILTERRANGE, myParameters->P_BF_RESETRANGE);

    /***************************************************************************
    * Step 3: Create arrays                                                    *
    ***************************************************************************/

    myBeamformer->maxValues = (float*) newTable1D(myBeamformer->BF_MAXSOURCES, sizeof(float));
    myBeamformer->maxIndexes = (signed int*) newTable1D(myBeamformer->BF_MAXSOURCES, sizeof(signed int));

}

/*******************************************************************************
 * beamformerTerminate                                                         *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myBeamformer        The beamformer object to be terminated     *
 *                                                                             *
 * Outputs:     (none)                                                         *
 *                                                                             *
 * Description: This function frees the dynamic memory that was allocated in   *
 *              the initialization of this object                              *
 *                                                                             *
 ******************************************************************************/

void beamformerTerminate(struct objBeamformer *myBeamformer)
{

    /***************************************************************************
    * Step 1: Delete objects                                                   *
    ***************************************************************************/

    // +-----------------------------------------------------------------------+
    // | Step A: Microphones                                                   |
    // +-----------------------------------------------------------------------+

    microphonesTerminate(myBeamformer->myMicrophones);
    free((void*) myBeamformer->myMicrophones);

    // +-----------------------------------------------------------------------+
    // | Step B: Sphere                                                        |
    // +-----------------------------------------------------------------------+

    sphereTerminate(myBeamformer->mySphere);
    free((void*) myBeamformer->mySphere);

    // +-----------------------------------------------------------------------+
    // | Step C: Delays                                                        |
    // +-----------------------------------------------------------------------+

    delaysTerminate(myBeamformer->myDelays);
    free((void*) myBeamformer->myDelays);

    // +-----------------------------------------------------------------------+
    // | Step D: Rij                                                           |
    // +-----------------------------------------------------------------------+

    rijTerminate(myBeamformer->myRij);
    free((void*) myBeamformer->myRij);

    /***************************************************************************
    * Step 2: Create arrays                                                    *
    ***************************************************************************/

    deleteTable1D((void*) myBeamformer->maxValues);
    deleteTable1D((void*) myBeamformer->maxIndexes);

}


/*******************************************************************************
 * beamformerFindMaxima                                                        *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myBeamformer    Beamformer object in use.                      *
 *              myPreprocessor  Preprocessor object to get frames              *
 *                                                                             *
 * Outputs:     mySources       The structure that holds the potential sources *
 *                              that are found with the beamformer for the     *
 *                              current frame                                  *
 *                                                                             *
 * Description: This function returns the potential sources                    *
 *                                                                             *
 ******************************************************************************/

void beamformerFindMaxima(struct objBeamformer *myBeamformer, struct objPreprocessor *myPreprocessor, struct objPotentialSources *mySources)
{

    unsigned int indexPotential;
    unsigned int indexMic;
    unsigned int indexPair;
    unsigned int indexPoint;

    signed int delay;

    float sum;
    float maxValue;
    signed int maxIndex;

    float x,y,z;
    float energy;
    float v;
    float probability;

    /***************************************************************************
    * Step 1: Rij object                                                       *
    ***************************************************************************/

    // +-----------------------------------------------------------------------+
    // | Step A: Load weighted spectra                                         |
    // +-----------------------------------------------------------------------+

    for (indexMic = 0; indexMic < myBeamformer->myMicrophones->nMics; indexMic++)
    {
        rijLoadFrame(myBeamformer->myRij, indexMic, myPreprocessor->micArray[indexMic]->weightedResultReal, myPreprocessor->micArray[indexMic]->weightedResultImag);
    }

    // +-----------------------------------------------------------------------+
    // | Step B: Compute cross-correlations                                    |
    // +-----------------------------------------------------------------------+

    rijProcess(myBeamformer->myRij);

    /***************************************************************************
    * Step 2: Find potential sources                                           *
    ***************************************************************************/

    for (indexPotential = 0; indexPotential < myBeamformer->BF_MAXSOURCES; indexPotential++)
    {

        // +-------------------------------------------------------------------+
        // | Step A: Find the maximum                                          |
        // +-------------------------------------------------------------------+

        maxValue = -1E30f;
        maxIndex = -1;

        for (indexPoint = 0; indexPoint < myBeamformer->myDelays->nPoints; indexPoint++)
        {

            // +---------------------------------------------------------------+
            // | Step i: Compute the energy for the current point              |
            // +---------------------------------------------------------------+

            sum = 0.0f;

            for (indexPair = 0; indexPair < myBeamformer->myDelays->nPairs; indexPair++)
            {

                // Get the associated delay
                delay = delaysGetFromPair(myBeamformer->myDelays, indexPoint, indexPair);

                // Add this to the sum of energy for the current point
                sum += rijGetEnergyFromPair(myBeamformer->myRij, indexPair, delay);

            }

            // +---------------------------------------------------------------+
            // | Step ii: Check if this is now the new maximum                 |
            // +---------------------------------------------------------------+

            if (sum > maxValue)
            {
                maxValue = sum;
                maxIndex = indexPoint;
            }

        }

        // Save the result
        myBeamformer->maxValues[indexPotential] = maxValue;
        myBeamformer->maxIndexes[indexPotential] = maxIndex;

        // +-------------------------------------------------------------------+
        // | Step B: Reset the delays associated with the maximum              |
        // +-------------------------------------------------------------------+

        // First verify that this was not the last iteration so that delays
        // are reset only if needed

        if (indexPotential != (myBeamformer->BF_MAXSOURCES - 1))
        {

            // Reset
            rijRemoveSource(myBeamformer->myRij, myBeamformer->myDelays, maxIndex);

        }

    }

    /***************************************************************************
    * Step 3: Generate potential sources and probabilities                     *
    ***************************************************************************/

    for (indexPotential = 0; indexPotential < myBeamformer->BF_MAXSOURCES; indexPotential++)
    {

        // +-------------------------------------------------------------------+
        // | Step A: Get positions                                             |
        // +-------------------------------------------------------------------+

        indexPoint = myBeamformer->maxIndexes[indexPotential];

        x = sphereGetX(myBeamformer->mySphere, indexPoint);
        y = sphereGetY(myBeamformer->mySphere, indexPoint);
        z = sphereGetZ(myBeamformer->mySphere, indexPoint);

        mySources->sourcesPosition[indexPotential][0] = x;
        mySources->sourcesPosition[indexPotential][1] = y;
        mySources->sourcesPosition[indexPotential][2] = z;

        // +-------------------------------------------------------------------+
        // | Step B: Get probability                                           |
        // +-------------------------------------------------------------------+

        energy = myBeamformer->maxValues[indexPotential];

        switch (indexPotential)
        {

        case 0:

            v = energy / myBeamformer->BF_ET;

            if (v <= 1.0f)
            {
                probability = v * v / 2.0f;
            }
            else
            {
                probability = 1.0f - (1.0f / (v * v * 2.0f));
            }
            break;

        case 1:
            probability = 0.3f;
            break;

        case 2:
            probability = 0.16f;
            break;

        case 3:
            probability = 0.03f;
            break;

        default:
            probability = 0.01f;
            break;

        }

        mySources->sourcesProbability[indexPotential] = probability;

    }

}
