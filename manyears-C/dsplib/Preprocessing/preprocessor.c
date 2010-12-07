/*******************************************************************************
 * ManyEars: Preprocessor - Source Code                                        *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Author: François Grondin                                                    *
 * Original Code: Jean-Marc Valin                                              *
 * Modified Code: Simon Brière                                                 *
 * Version: 1.1.0                                                              *
 * Date: August 19th, 2010                                                     *
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

#include "Preprocessing/preprocessor.h"

/*******************************************************************************
 * preprocessorInit                                                            *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myPreprocessor  Object to be initialized                       *
 *              myParameters    List of parameters that is used to configure   *
 *                              the object.                                    *
 *                                                                             *
 * Outputs:     myPreprocessor  Initialized object                             *
 *                                                                             *
 * Description: This function initializes the preprocessor object and          *
 *              allocates memory that will be used later.                      *
 *                                                                             *
 ******************************************************************************/

void preprocessorInit(struct objPreprocessor *myPreprocessor, struct ParametersStruct *myParameters, struct objMicrophones* myMicrophones)
{

    unsigned int indexMic;

    /***************************************************************************
    * Step 1: Load parameters                                                  *
    ***************************************************************************/

        // +-------------------------------------------------------------------+
        // | General                                                           |
        // +-------------------------------------------------------------------+

        // Define the size of the frame
        myPreprocessor->PP_FRAMESIZE = GLOBAL_FRAMESIZE;

    /***************************************************************************
    * Step 2: Initialize context                                               *
    ***************************************************************************/

        // +-------------------------------------------------------------------+
        // | Step A: Microphones                                               |
        // +-------------------------------------------------------------------+

        myPreprocessor->myMicrophones = (struct objMicrophones*) malloc(sizeof(struct objMicrophones));
        microphonesClone(myMicrophones, myPreprocessor->myMicrophones);

        // +-------------------------------------------------------------------+
        // | Step B: Create arrays                                             |
        // +-------------------------------------------------------------------+

        myPreprocessor->micArray = (struct objMicST**) newTable2D(myPreprocessor->myMicrophones->nMics, 1, sizeof(struct objMicST));

        myPreprocessor->myFFT = (struct objFFT*) malloc(sizeof(struct objFFT));

        myPreprocessor->window = (float*) newTable1D(myPreprocessor->PP_FRAMESIZE, sizeof(float));

        myPreprocessor->workingArray1Real = (float*) newTable1D(myPreprocessor->PP_FRAMESIZE, sizeof(float));
        myPreprocessor->workingArray1Imag = (float*) newTable1D(myPreprocessor->PP_FRAMESIZE, sizeof(float));
        myPreprocessor->workingArray2Real = (float*) newTable1D(myPreprocessor->PP_FRAMESIZE, sizeof(float));
        myPreprocessor->workingArray2Imag = (float*) newTable1D(myPreprocessor->PP_FRAMESIZE, sizeof(float));

        // +-------------------------------------------------------------------+
        // | Step C: Initialize the FFT object                                 |
        // +-------------------------------------------------------------------+

        fftInit(myPreprocessor->myFFT, myParameters, myPreprocessor->PP_FRAMESIZE);

        // +-------------------------------------------------------------------+
        // | Step D: Initialize window                                         |
        // +-------------------------------------------------------------------+

        generatePowerComplementaryWindow(myPreprocessor->window, myPreprocessor->PP_FRAMESIZE);

        // +-------------------------------------------------------------------+
        // | Step E: Initialize the sound track for each microphone            |
        // +-------------------------------------------------------------------+

        for (indexMic = 0; indexMic < myPreprocessor->myMicrophones->nMics; indexMic++)
        {
            micstInit(myPreprocessor->micArray[indexMic], myParameters,
                      microphonesGetPosition(myPreprocessor->myMicrophones,indexMic),
                      microphonesGetGain(myPreprocessor->myMicrophones,indexMic));
        }

}

/*******************************************************************************
 * preprocessorTerminate                                                       *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myPreprocessor  The object to be terminated                    *
 *                                                                             *
 * Outputs:     (none)                                                         *
 *                                                                             *
 * Description: This function frees the dynamic memory that was allocated in   *
 *              the initialization of this object                              *
 *                                                                             *
 ******************************************************************************/

void preprocessorTerminate(struct objPreprocessor *myPreprocessor)
{

    unsigned int indexMic;

    /***************************************************************************
    * Step 1: Free memory                                                      *
    ***************************************************************************/

        // +-------------------------------------------------------------------+
        // | Step A: Sound tracks                                              |
        // +-------------------------------------------------------------------+

        for (indexMic = 0; indexMic < myPreprocessor->myMicrophones->nMics; indexMic++)
        {
            micstTerminate(myPreprocessor->micArray[indexMic]);
        }

        deleteTable2D((void**) myPreprocessor->micArray);

        // +-------------------------------------------------------------------+
        // | Step B: Window                                                    |
        // +-------------------------------------------------------------------+

        deleteTable1D((void*) myPreprocessor->window);

        // +-------------------------------------------------------------------+
        // | Step C: FFT object                                                |
        // +-------------------------------------------------------------------+

        fftTerminate(myPreprocessor->myFFT);
        free(myPreprocessor->myFFT);

        // +-------------------------------------------------------------------+
        // | Step D: Arrays                                                    |
        // +-------------------------------------------------------------------+

        deleteTable1D((void*) myPreprocessor->workingArray1Real);
        deleteTable1D((void*) myPreprocessor->workingArray1Imag);
        deleteTable1D((void*) myPreprocessor->workingArray2Real);
        deleteTable1D((void*) myPreprocessor->workingArray2Imag);

        // +-------------------------------------------------------------------+
        // | Step E: Microphones                                               |
        // +-------------------------------------------------------------------+

        microphonesTerminate(myPreprocessor->myMicrophones);
        free((void*) myPreprocessor->myMicrophones);

}

/*******************************************************************************
 * preprocessorPushFrames                                                      *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myPreprocessor      The beamformer object which frames will be *
 *                                  shifted                                    *
 *              shiftSize           The number of shifted samples              *
 *              micIndex            The index of the microphone to which the   *
 *                                  frame corresponds                          *
 *                                                                             *
 * Outputs:     myPreprocessor      The updated preprocessor                   *
 *                                                                             *
 * Description: This function pushes the current samples by a given shift      *
 *              shift amount.                                                  *
 *                                                                             *
 *              For instance, say we have a frame of 10 samples for microphone *
 *              #4 and the following call is made:                             *
 *                                                                             *
 *              preprocessorPushFrames(&currentObj, 5, 4)                      *
 *                                                                             *
 *              We get the following transformation:                           *
 *                                                                             *
 *                Initial                      Final                           *
 *                                                                             *
 *              +----------+                +----------+                       *
 *              | Sample A |                | Sample F |                       *
 *              +----------+                +----------+                       *
 *              | Sample B |                | Sample G |                       *
 *              +----------+                +----------+                       *
 *              | Sample C |                | Sample H |                       *
 *              +----------+                +----------+                       *
 *              | Sample D |                | Sample I |                       *
 *              +----------+                +----------+                       *
 *              | Sample E |                | Sample J |                       *
 *              +----------+       =>       +----------+                       *
 *              | Sample F |                | (empty)  |                       *
 *              +----------+                +----------+                       *
 *              | Sample G |                | (empty)  |                       *
 *              +----------+                +----------+                       *
 *              | Sample H |                | (empty)  |                       *
 *              +----------+                +----------+                       *
 *              | Sample I |                | (empty)  |                       *
 *              +----------+                +----------+                       *
 *              | Sample J |                | (empty)  |                       *
 *              +----------+                +----------+                       *
 *                                                                             *
 ******************************************************************************/

void preprocessorPushFrames(struct objPreprocessor *myPreprocessor, unsigned int shiftSize, unsigned int micIndex)
{

    // Index to loop throught the frame
    unsigned int k;

    // Pointers
    unsigned int indexSource;
    unsigned int indexDestination;

    indexDestination = 0;
    indexSource = shiftSize;

    // Move the samples
    for (k = 0; k < (myPreprocessor->PP_FRAMESIZE - shiftSize); k++)
    {
        myPreprocessor->micArray[micIndex]->xtime[indexDestination] = myPreprocessor->micArray[micIndex]->xtime[indexSource];
        indexSource++;
        indexDestination++;
    }

}

/*******************************************************************************
 * preprocessorAddFrame                                                        *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myPreprocessor      The preprocessor object in which the       *
 *                                  frames                                     *
 *                                  are to be loaded                           *
 *              newFrame            A pointer to the beginning of the array to *
 *                                  be loaded                                  *
 *              micIndex            The index of the microphone to which the   *
 *                                  frame corresponds                          *
 *              sizeFrame           The size of the array to be loaded         *
 *                                                                             *
 * Outputs:     myPreprocessor      The updated object                         *
 *                                                                             *
 * Description: This function loads the new array inside the frame             *
 *                                                                             *
 *              For instance, say we have a frame of 10 samples for microphone *
 *              #4 and the following call is made:                             *
 *                                                                             *
 *              preprocessorAddFrame(&currentObj, &currentArray, 4, 5)         *
 *                                                                             *
 *              We get the following transformation:                           *
 *                                                                             *
 *                     Array            Initial                  Final         *
 *                                                                             *
 *                  +----------+      +----------+            +----------+     *
 * &currentArray -> | Sample K |      | Sample F |            | Sample F |     *
 *                  +----------+      +----------+            +----------+     *
 *                  | Sample L |      | Sample G |            | Sample G |     *
 *                  +----------+      +----------+            +----------+     *
 *                  | Sample M |      | Sample H |            | Sample H |     *
 *                  +----------+      +----------+            +----------+     *
 *                  | Sample N |      | Sample I |            | Sample I |     *
 *                  +----------+      +----------+            +----------+     *
 *                  | Sample O |      | Sample J |            | Sample J |     *
 *                  +----------+      +----------+     =>     +----------+     *
 *                                    | (empty)  |            | Sample K |     *
 *                                    +----------+            +----------+     *
 *                                    | (empty)  |            | Sample L |     *
 *                                    +----------+            +----------+     *
 *                                    | (empty)  |            | Sample M |     *
 *                                    +----------+            +----------+     *
 *                                    | (empty)  |            | Sample N |     *
 *                                    +----------+            +----------+     *
 *                                    | (empty)  |            | Sample O |     *
 *                                    +----------+            +----------+     *
 *                                                                             *
 ******************************************************************************/

void preprocessorAddFrame(struct objPreprocessor *myPreprocessor, float *newFrame, unsigned char micIndex, unsigned int sizeFrame)
{

    // Index to loop throught the frame
    unsigned int k;

    // Pointers
    unsigned int indexSource;
    unsigned int indexDestination;

    // Gain of this microphone
    float gain;

    indexDestination = myPreprocessor->PP_FRAMESIZE - sizeFrame;
    indexSource = 0;

    gain = microphonesGetGain(myPreprocessor->myMicrophones, micIndex);

    // Copy the samples
    for (k = 0; k < sizeFrame; k++)
    {
        myPreprocessor->micArray[micIndex]->xtime[indexDestination] = gain * newFrame[indexSource];
        indexSource++;
        indexDestination++;
    }

}

/*******************************************************************************
 * preprocessorProcessFrame                                                    *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myPreprocessor  The object used to process                     *
 *                                                                             *
 * Outputs:     (none)                                                         *
 *                                                                             *
 * Description: This processes the frames                                      *
 *                                                                             *
 ******************************************************************************/

void preprocessorProcessFrame(struct objPreprocessor *myPreprocessor)
{

    // Index to loop through sound track
    unsigned int micIndex;

    // Index to loop through each frame
    unsigned int k;

    /***************************************************************************
    * Step 1: Window                                                           *
    ***************************************************************************/

    for (micIndex = 0; micIndex < myPreprocessor->myMicrophones->nMics; micIndex++)
    {

        for (k = 0; k < ((unsigned int) myPreprocessor->PP_FRAMESIZE); k++)
        {

            myPreprocessor->micArray[micIndex]->xtime_windowed[k] = myPreprocessor->micArray[micIndex]->xtime[k] * myPreprocessor->window[k];

        }

    }

    /***************************************************************************
    * Step 2: Compute the FFT of each microphone sound track frame             *
    ***************************************************************************/

    // For each pair of frames, we can compute both FFTs in parallel with only
    // one FFT since the frame signals are always real

    for (micIndex = 0; micIndex < myPreprocessor->myMicrophones->nMics; micIndex+=2)
    {
        fftComputeTwice(myPreprocessor->myFFT, myPreprocessor->micArray[micIndex]->xtime_windowed, myPreprocessor->micArray[micIndex+1]->xtime_windowed, myPreprocessor->micArray[micIndex]->xfreqReal, myPreprocessor->micArray[micIndex]->xfreqImag, myPreprocessor->micArray[micIndex+1]->xfreqReal, myPreprocessor->micArray[micIndex+1]->xfreqImag);
    }

    // If there was an odd number of microphone sound tracks, get the last FFT
    if ((myPreprocessor->myMicrophones->nMics / 2.0) != floor(myPreprocessor->myMicrophones->nMics/2.0))
    {
        fftComputeOnce(myPreprocessor->myFFT, myPreprocessor->micArray[myPreprocessor->myMicrophones->nMics-1]->xtime_windowed, myPreprocessor->micArray[myPreprocessor->myMicrophones->nMics-1]->xfreqReal, myPreprocessor->micArray[myPreprocessor->myMicrophones->nMics-1]->xfreqImag);
    }

    /***************************************************************************
    * Step 3: Get the weighted expression for each frame (zeta(k)*X(k)/|X(k)|) *
    ***************************************************************************/

    for (micIndex = 0; micIndex < myPreprocessor->myMicrophones->nMics; micIndex++)
    {
        micstProcessFrame(myPreprocessor->micArray[micIndex]);
    }

}
