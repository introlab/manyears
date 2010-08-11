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
 ******************************************************************************/

#include <math.h>

#include "beamformer.h"
#include "micSoundTrack.h"
#include "sphere.h"
#include "fft.h"
#include "randGen.h"
#include "hardware.h"

#include "stdio.h"

/*******************************************************************************
 * Compatibility issues                                                        *
 ******************************************************************************/

// In some environment, the constant PI is not define with the math.h library
#ifndef M_PI
	#define M_PI	3.1415926535897932384626433832795
#endif


/*******************************************************************************
 * beamformerInit                                                              *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myBeamformer    Object to be initialized                       *
 *              myParameters    List of parameters that is used to configure   *
 *                              the object.                                    *
 *                                                                             *
 * Outputs:     myBeamformer    Initialized object                             *
 *                                                                             *
 * Description: This function initializes the beamformer object and allocates  *
 *              memory that will be used later.                                *
 *                                                                             *
 ******************************************************************************/

void beamformerInit(struct objBeamformer *myBeamformer, struct ParametersStruct *myParameters)
{

    // Index to loop through microphones
    unsigned char indexMic1;

    // Index to loop through microphones
    unsigned char indexMic2;

    // Index to count the number of pairs
    unsigned char indexPair;

    // Index to loop through points on the sphere
    unsigned int indexPoint;

    // Current delay (can be negative)
    signed short currentDelay;

    /***************************************************************************
    * Step 1: Load parameters                                                  *
    ***************************************************************************/

        // +-------------------------------------------------------------------+
        // | Microphone position                                               |
        // +-------------------------------------------------------------------+

        myBeamformer->BF_MICSPOSITIONS_1_X = myParameters->P_BF_MICSPOSITIONS_1_X;
        myBeamformer->BF_MICSPOSITIONS_1_Y = myParameters->P_BF_MICSPOSITIONS_1_Y;
        myBeamformer->BF_MICSPOSITIONS_1_Z = myParameters->P_BF_MICSPOSITIONS_1_Z;
        myBeamformer->BF_MICSPOSITIONS_2_X = myParameters->P_BF_MICSPOSITIONS_2_X;
        myBeamformer->BF_MICSPOSITIONS_2_Y = myParameters->P_BF_MICSPOSITIONS_2_Y;
        myBeamformer->BF_MICSPOSITIONS_2_Z = myParameters->P_BF_MICSPOSITIONS_2_Z;
        myBeamformer->BF_MICSPOSITIONS_3_X = myParameters->P_BF_MICSPOSITIONS_3_X;
        myBeamformer->BF_MICSPOSITIONS_3_Y = myParameters->P_BF_MICSPOSITIONS_3_Y;
        myBeamformer->BF_MICSPOSITIONS_3_Z = myParameters->P_BF_MICSPOSITIONS_3_Z;
        myBeamformer->BF_MICSPOSITIONS_4_X = myParameters->P_BF_MICSPOSITIONS_4_X;
        myBeamformer->BF_MICSPOSITIONS_4_Y = myParameters->P_BF_MICSPOSITIONS_4_Y;
        myBeamformer->BF_MICSPOSITIONS_4_Z = myParameters->P_BF_MICSPOSITIONS_4_Z;
        myBeamformer->BF_MICSPOSITIONS_5_X = myParameters->P_BF_MICSPOSITIONS_5_X;
        myBeamformer->BF_MICSPOSITIONS_5_Y = myParameters->P_BF_MICSPOSITIONS_5_Y;
        myBeamformer->BF_MICSPOSITIONS_5_Z = myParameters->P_BF_MICSPOSITIONS_5_Z;
        myBeamformer->BF_MICSPOSITIONS_6_X = myParameters->P_BF_MICSPOSITIONS_6_X;
        myBeamformer->BF_MICSPOSITIONS_6_Y = myParameters->P_BF_MICSPOSITIONS_6_Y;
        myBeamformer->BF_MICSPOSITIONS_6_Z = myParameters->P_BF_MICSPOSITIONS_6_Z;
        myBeamformer->BF_MICSPOSITIONS_7_X = myParameters->P_BF_MICSPOSITIONS_7_X;
        myBeamformer->BF_MICSPOSITIONS_7_Y = myParameters->P_BF_MICSPOSITIONS_7_Y;
        myBeamformer->BF_MICSPOSITIONS_7_Z = myParameters->P_BF_MICSPOSITIONS_7_Z;
        myBeamformer->BF_MICSPOSITIONS_8_X = myParameters->P_BF_MICSPOSITIONS_8_X;
        myBeamformer->BF_MICSPOSITIONS_8_Y = myParameters->P_BF_MICSPOSITIONS_8_Y;
        myBeamformer->BF_MICSPOSITIONS_8_Z = myParameters->P_BF_MICSPOSITIONS_8_Z;

        // +-------------------------------------------------------------------+
        // | Microphone gain                                                   |
        // +-------------------------------------------------------------------+

        myBeamformer->BF_MICSGAIN_1 = myParameters->P_BF_MICSGAIN_1;
        myBeamformer->BF_MICSGAIN_2 = myParameters->P_BF_MICSGAIN_2;
        myBeamformer->BF_MICSGAIN_3 = myParameters->P_BF_MICSGAIN_3;
        myBeamformer->BF_MICSGAIN_4 = myParameters->P_BF_MICSGAIN_4;
        myBeamformer->BF_MICSGAIN_5 = myParameters->P_BF_MICSGAIN_5;
        myBeamformer->BF_MICSGAIN_6 = myParameters->P_BF_MICSGAIN_6;
        myBeamformer->BF_MICSGAIN_7 = myParameters->P_BF_MICSGAIN_7;
        myBeamformer->BF_MICSGAIN_8 = myParameters->P_BF_MICSGAIN_8;

        // +-------------------------------------------------------------------+
        // | General                                                           |
        // +-------------------------------------------------------------------+

        // Define the number of microphones
        myBeamformer->BF_MICSNUMBER = GLOBAL_MICSNUMBER;

        // Define the maximum number of sources that can be found
        myBeamformer->BF_MAXSOURCES = myParameters->P_BF_MAXSOURCES;

        // Define the number of maxima to keep in memory in order to find
        // the one that best fits
        myBeamformer->BF_NUMBERMAX = myParameters->P_BF_NUMBERMAX;

        // Define the value threshold for the maxima comparison
        myBeamformer->BF_VALUETS = myParameters->P_BF_VALUETS;

        // Define the horizontal angle tolerance for first source
        myBeamformer->BF_HANGLETOL = myParameters->P_BF_HANGLETOL;

        // Define the horizontal angle tolerance for next sources
        myBeamformer->BF_HANGLETOLNEXT = myParameters->P_BF_HANGLETOLNEXT;

        // Define the vertical angle tolerance
        myBeamformer->BF_VANGLETOL = myParameters->P_BF_VANGLETOL;

        // Define the range where the neighbour delays are used to refine
        // the result
        myBeamformer->BF_RANGE = myParameters->P_BF_RANGE;

        // Define the number of delays next to the main delay to set to zero
        // to find the peaks after the first one
        myBeamformer->BF_RIJDELAYS = myParameters->P_BF_RIJDELAYS;

        // Calculate the number of possibilities given the number of microphones
        myBeamformer->BF_CROSSNUMBER = (myBeamformer->BF_MICSNUMBER) * (myBeamformer->BF_MICSNUMBER - 1) / 2;

        // Size of a frame
        myBeamformer->MICST_FRAMESIZE = GLOBAL_FRAMESIZE;

        // Number of levels on the sphere
        myBeamformer->SPHERE_NUMBERLEVELS = GLOBAL_SPHERE_NUMBERLEVELS;

        // Number of points on the sphere
        myBeamformer->SPHERE_NUMBERPOINTS = 20 * pow(4, myBeamformer->SPHERE_NUMBERLEVELS) / 2 + 2;

        // Sampling rate
        myBeamformer->MICST_FS = GLOBAL_FS;

        // Speed of sound
        myBeamformer->MICST_C = GLOBAL_C;

    /***************************************************************************
    * Step 2: Initialize context                                               *
    ***************************************************************************/

        // +-------------------------------------------------------------------+
        // | Step A: Create arrays                                             |
        // +-------------------------------------------------------------------+

        myBeamformer->micArray = (struct objMicST**) newTable2D(myBeamformer->BF_MICSNUMBER, 1, sizeof(struct objMicST));

        myBeamformer->matrixPairs = (unsigned char**) newTable2D(myBeamformer->BF_CROSSNUMBER, 2, sizeof(unsigned char));

        myBeamformer->Rij = (float**) newTable2D(myBeamformer->BF_CROSSNUMBER, myBeamformer->MICST_FRAMESIZE, sizeof(float));

        myBeamformer->mySphere = (struct objSphere*) malloc(sizeof(struct objSphere));

        myBeamformer->myFFT = (struct objFFT*) malloc(sizeof(struct objFFT));

        myBeamformer->delaysSphere = (unsigned short**) newTable2D(myBeamformer->BF_CROSSNUMBER, myBeamformer->SPHERE_NUMBERPOINTS, sizeof(unsigned short));

        myBeamformer->listMaximaIndexes = (unsigned int*) newTable1D(myBeamformer->BF_NUMBERMAX, sizeof(unsigned int));
        myBeamformer->listMaximaValues = (float*) newTable1D(myBeamformer->BF_NUMBERMAX, sizeof(float));

        myBeamformer->hanningWindow = (float*) newTable1D(myBeamformer->MICST_FRAMESIZE, sizeof(float));

        myBeamformer->positions = (float**) newTable2D(myBeamformer->BF_MICSNUMBER, 3, sizeof(float));
        myBeamformer->gains = (float*) newTable1D(myBeamformer->BF_MICSNUMBER, sizeof(float));

        myBeamformer->workingArray1Real = (float*) newTable1D(myBeamformer->MICST_FRAMESIZE, sizeof(float));
        myBeamformer->workingArray1Imag = (float*) newTable1D(myBeamformer->MICST_FRAMESIZE, sizeof(float));
        myBeamformer->workingArray2Real = (float*) newTable1D(myBeamformer->MICST_FRAMESIZE, sizeof(float));
        myBeamformer->workingArray2Imag = (float*) newTable1D(myBeamformer->MICST_FRAMESIZE, sizeof(float));

        myBeamformer->prevAngleH = (float*) newTable1D(myBeamformer->BF_MAXSOURCES, sizeof(float));
        myBeamformer->prevAngleV = (float*) newTable1D(myBeamformer->BF_MAXSOURCES, sizeof(float));

        // +-------------------------------------------------------------------+
        // | Step B: Initialize the position of microphones                    |
        // +-------------------------------------------------------------------+

        myBeamformer->positions[0][0] = myBeamformer->BF_MICSPOSITIONS_1_X;
        myBeamformer->positions[0][1] = myBeamformer->BF_MICSPOSITIONS_1_Y;
        myBeamformer->positions[0][2] = myBeamformer->BF_MICSPOSITIONS_1_Z;
        myBeamformer->positions[1][0] = myBeamformer->BF_MICSPOSITIONS_2_X;
        myBeamformer->positions[1][1] = myBeamformer->BF_MICSPOSITIONS_2_Y;
        myBeamformer->positions[1][2] = myBeamformer->BF_MICSPOSITIONS_2_Z;
        myBeamformer->positions[2][0] = myBeamformer->BF_MICSPOSITIONS_3_X;
        myBeamformer->positions[2][1] = myBeamformer->BF_MICSPOSITIONS_3_Y;
        myBeamformer->positions[2][2] = myBeamformer->BF_MICSPOSITIONS_3_Z;
        myBeamformer->positions[3][0] = myBeamformer->BF_MICSPOSITIONS_4_X;
        myBeamformer->positions[3][1] = myBeamformer->BF_MICSPOSITIONS_4_Y;
        myBeamformer->positions[3][2] = myBeamformer->BF_MICSPOSITIONS_4_Z;
        myBeamformer->positions[4][0] = myBeamformer->BF_MICSPOSITIONS_5_X;
        myBeamformer->positions[4][1] = myBeamformer->BF_MICSPOSITIONS_5_Y;
        myBeamformer->positions[4][2] = myBeamformer->BF_MICSPOSITIONS_5_Z;
        myBeamformer->positions[5][0] = myBeamformer->BF_MICSPOSITIONS_6_X;
        myBeamformer->positions[5][1] = myBeamformer->BF_MICSPOSITIONS_6_Y;
        myBeamformer->positions[5][2] = myBeamformer->BF_MICSPOSITIONS_6_Z;
        myBeamformer->positions[6][0] = myBeamformer->BF_MICSPOSITIONS_7_X;
        myBeamformer->positions[6][1] = myBeamformer->BF_MICSPOSITIONS_7_Y;
        myBeamformer->positions[6][2] = myBeamformer->BF_MICSPOSITIONS_7_Z;
        myBeamformer->positions[7][0] = myBeamformer->BF_MICSPOSITIONS_8_X;
        myBeamformer->positions[7][1] = myBeamformer->BF_MICSPOSITIONS_8_Y;
        myBeamformer->positions[7][2] = myBeamformer->BF_MICSPOSITIONS_8_Z;

        // +-------------------------------------------------------------------+
        // | Step C: Initialize the gain of microphones                        |
        // +-------------------------------------------------------------------+

        myBeamformer->gains[0] = myBeamformer->BF_MICSGAIN_1;
        myBeamformer->gains[1] = myBeamformer->BF_MICSGAIN_2;
        myBeamformer->gains[2] = myBeamformer->BF_MICSGAIN_3;
        myBeamformer->gains[3] = myBeamformer->BF_MICSGAIN_4;
        myBeamformer->gains[4] = myBeamformer->BF_MICSGAIN_5;
        myBeamformer->gains[5] = myBeamformer->BF_MICSGAIN_6;
        myBeamformer->gains[6] = myBeamformer->BF_MICSGAIN_7;
        myBeamformer->gains[7] = myBeamformer->BF_MICSGAIN_8;

        // +-------------------------------------------------------------------+
        // | Step D: Initialize the pairs of microphones                       |
        // +-------------------------------------------------------------------+

        indexPair = 0;

        for (indexMic1 = 0; indexMic1 < myBeamformer->BF_MICSNUMBER; indexMic1++)
        {
            for (indexMic2 = (indexMic1 + 1); indexMic2 < myBeamformer->BF_MICSNUMBER; indexMic2++)
            {
                myBeamformer->matrixPairs[indexPair][0] = indexMic1;
                myBeamformer->matrixPairs[indexPair][1] = indexMic2;

                indexPair++;
            }
        }

        // +-------------------------------------------------------------------+
        // | Step E: Initialize the sound track for each microphone            |
        // +-------------------------------------------------------------------+

        for (indexMic1 = 0; indexMic1 < myBeamformer->BF_MICSNUMBER; indexMic1++)
        {
            micstInit(myBeamformer->micArray[indexMic1], myParameters, myBeamformer->positions[indexMic1], myBeamformer->gains[indexMic1]);
        }

        // +-------------------------------------------------------------------+
        // | Step F: Initialize the unit sphere                                |
        // +-------------------------------------------------------------------+

        sphereInit(myBeamformer->mySphere, myBeamformer->SPHERE_NUMBERLEVELS);

        // +-------------------------------------------------------------------+
        // | Step G: Initialize the FFT object                                 |
        // +-------------------------------------------------------------------+

        fftInit(myBeamformer->myFFT, myParameters);

        // +-------------------------------------------------------------------+
        // | Step H: Compute the delays and the decision factor for each point |
        // +-------------------------------------------------------------------+

        // Compute the delays for each point on the sphere
        for (indexPoint = 0; indexPoint < myBeamformer->SPHERE_NUMBERPOINTS; indexPoint++)
        {

            // Compute the delays with each pair of microphones
            for (indexPair = 0; indexPair < myBeamformer->BF_CROSSNUMBER; indexPair++)
            {

                // Get the index of each microphone
                indexMic1 = myBeamformer->matrixPairs[indexPair][0];
                indexMic2 = myBeamformer->matrixPairs[indexPair][1];

                // Get the delay
                // delay = (Fs/c) * ( (pi_x - pj_x) * u_x + (pi_y - pj_y) * u_y + (pi_z - pj_z) * u_z)
                currentDelay = -1 * floor(((((float) myBeamformer->MICST_FS) / ((float) myBeamformer->MICST_C)) * ( (myBeamformer->micArray[indexMic1]->position[0] - myBeamformer->micArray[indexMic2]->position[0]) * myBeamformer->mySphere->spherePoints[indexPoint][0] + (myBeamformer->micArray[indexMic1]->position[1] - myBeamformer->micArray[indexMic2]->position[1]) * myBeamformer->mySphere->spherePoints[indexPoint][1] + (myBeamformer->micArray[indexMic1]->position[2] - myBeamformer->micArray[indexMic2]->position[2]) * myBeamformer->mySphere->spherePoints[indexPoint][2])) + 0.5);

                // If the delay is negative, then wrap around to match the indexes of the frame
                if (currentDelay < 0)
                {
                    myBeamformer->delaysSphere[indexPair][indexPoint] = (unsigned short) (currentDelay + myBeamformer->MICST_FRAMESIZE);
                }
                else
                {
                    myBeamformer->delaysSphere[indexPair][indexPoint] = (unsigned short) currentDelay;
                }

            }

        }

        // +-------------------------------------------------------------------+
        // | Step I: Initialize window                                         |
        // +-------------------------------------------------------------------+

        generateWindow(myBeamformer->hanningWindow, myBeamformer->MICST_FRAMESIZE);

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

    int indexMic;

    // Free memory   

    for (indexMic = 0; indexMic < myBeamformer->BF_MICSNUMBER; indexMic++)
    {
        micstTerminate(myBeamformer->micArray[indexMic]);
    }

    deleteTable2D((void**) myBeamformer->micArray);

    deleteTable2D((void**) myBeamformer->matrixPairs);;

    deleteTable2D((void**) myBeamformer->Rij);

    sphereTerminate(myBeamformer->mySphere);
    free(myBeamformer->mySphere);

    fftTerminate(myBeamformer->myFFT);
    free(myBeamformer->myFFT);

    deleteTable2D((void**) myBeamformer->delaysSphere);

    deleteTable1D((void*) myBeamformer->listMaximaIndexes);
    deleteTable1D((void*) myBeamformer->listMaximaValues);

    deleteTable1D((void*) myBeamformer->hanningWindow);

    deleteTable2D((void**) myBeamformer->positions);
    deleteTable1D((void*) myBeamformer->gains);

    deleteTable1D((void*) myBeamformer->workingArray1Real);
    deleteTable1D((void*) myBeamformer->workingArray1Imag);
    deleteTable1D((void*) myBeamformer->workingArray2Real);
    deleteTable1D((void*) myBeamformer->workingArray2Imag);

    deleteTable1D((void*) myBeamformer->prevAngleH);
    deleteTable1D((void*) myBeamformer->prevAngleV);

}

/*******************************************************************************
 * beamformerPushFrames                                                        *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myBeamformer        The beamformer object which frames will be *
 *                                  shifted                                    *
 *              shiftSize           The number of shifted samples              *
 *              micIndex            The index of the microphone to which the   *
 *                                  frame corresponds                          *
 *                                                                             *
 * Outputs:     myBeamformer        The updated beamformer                     *
 *                                                                             *
 * Description: This function pushes the current samples by a given shift      *
 *              shift amount.                                                  *
 *                                                                             *
 *              For instance, say we have a frame of 10 samples for microphone *
 *              #4 and the following call is made:                             *
 *                                                                             *
 *              beamformerPushFrames(&currentObj, 5, 4)                        *
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

void beamformerPushFrames(struct objBeamformer *myBeamformer, unsigned int shiftSize, unsigned int micIndex)
{

    // Index to loop throught the frame
    unsigned int k;

    // Pointers
    unsigned int indexSource;
    unsigned int indexDestination;

    indexDestination = 0;
    indexSource = shiftSize;

    // Move the samples
    for (k = 0; k < (myBeamformer->MICST_FRAMESIZE - shiftSize); k++)
    {
        myBeamformer->micArray[micIndex]->xtime[indexDestination] = myBeamformer->micArray[micIndex]->xtime[indexSource];
        indexSource++;
        indexDestination++;
    }

}

/*******************************************************************************
 * beamformerAddFrame                                                          *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myBeamformer        The beamformer object in which the frames  *
 *                                  are to be loaded                           *
 *              newFrame            A pointer to the beginning of the array to *
 *                                  be loaded                                  *
 *              micIndex            The index of the microphone to which the   *
 *                                  frame corresponds                          *
 *              sizeFrame           The size of the array to be loaded         *
 *                                                                             *
 * Outputs:     myBeamformer        The updated beamformer                     *
 *                                                                             *
 * Description: This function loads the new array inside the frame             *
 *                                                                             *
 *              For instance, say we have a frame of 10 samples for microphone *
 *              #4 and the following call is made:                             *
 *                                                                             *
 *              beamformerAddFrame(&currentObj, &currentArray, 4, 5)           *
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

void beamformerAddFrame(struct objBeamformer *myBeamformer, float *newFrame, unsigned char micIndex, unsigned int sizeFrame)
{

    // Index to loop throught the frame
    unsigned int k;

    // Pointers
    unsigned int indexSource;
    unsigned int indexDestination;

    indexDestination = myBeamformer->MICST_FRAMESIZE - sizeFrame;
    indexSource = 0;

    // Copy the samples
    for (k = 0; k < sizeFrame; k++)
    {
        myBeamformer->micArray[micIndex]->xtime[indexDestination] = newFrame[indexSource];
        indexSource++;
        indexDestination++;
    }

}

/*******************************************************************************
 * beamformerComputeRij                                                        *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myBeamformer    Beamformer object in use.                      *
 *                                                                             *
 * Outputs:     myBeamformer    Updated beamformer object in use.              *
 *                                                                             *
 * Description: This function refreshes the matrix Rij inside the beamformer   *
 *              object.                                                        *
 *                                                                             *
 ******************************************************************************/

void beamformerComputeRij(struct objBeamformer *myBeamformer)
{

    // Index to loop through sound track
    unsigned int micIndex;
    unsigned int micIndexPrev;

    // Second index to loop through sound track
    unsigned int micIndex2;
    unsigned int micIndex2Prev;

    // Index to loop through each combinations
    unsigned int crossIndex;

    // Index to loop through each frame
    unsigned int k;

    /***************************************************************************
    * Step 0: Window                                                           *
    ***************************************************************************/

    for (micIndex = 0; micIndex < myBeamformer->BF_MICSNUMBER; micIndex++)
    {

        for (k = 0; k < myBeamformer->MICST_FRAMESIZE; k++)
        {

            myBeamformer->micArray[micIndex]->xtime_windowed[k] = myBeamformer->micArray[micIndex]->xtime[k] * myBeamformer->hanningWindow[k];

        }

    }

    /***************************************************************************
    * Step 1: Compute the FFT of each microphone sound track frame             *
    ***************************************************************************/

    // For each pair of frames, we can compute both FFTs in parallel with only
    // one FFT since the frame signals are always real

    for (micIndex = 0; micIndex < myBeamformer->BF_MICSNUMBER; micIndex+=2)
    {
        fftComputeTwice(myBeamformer->myFFT, myBeamformer->micArray[micIndex]->xtime_windowed, myBeamformer->micArray[micIndex+1]->xtime_windowed, myBeamformer->micArray[micIndex]->xfreqReal, myBeamformer->micArray[micIndex]->xfreqImag, myBeamformer->micArray[micIndex+1]->xfreqReal, myBeamformer->micArray[micIndex+1]->xfreqImag);
    }

    // If there was an odd number of microphone sound tracks, get the last FFT
    if ((myBeamformer->BF_MICSNUMBER / 2.0) != floor(myBeamformer->BF_MICSNUMBER/2.0))
    {
        fftComputeOnce(myBeamformer->myFFT, myBeamformer->micArray[myBeamformer->BF_MICSNUMBER-1]->xtime_windowed, myBeamformer->micArray[myBeamformer->BF_MICSNUMBER-1]->xfreqReal, myBeamformer->micArray[myBeamformer->BF_MICSNUMBER-1]->xfreqImag);
    }

    /***************************************************************************
    * Step 2: Get the weighted expression for each frame (zeta(k)*X(k)/|X(k)|) *
    ***************************************************************************/

    for (micIndex = 0; micIndex < myBeamformer->BF_MICSNUMBER; micIndex++)
    {
        micstProcessFrame(myBeamformer->micArray[micIndex]);
    }

    /***************************************************************************
    * Step 3: Multiply the cross terms                                         *
    ***************************************************************************/

    crossIndex = 0;

    for (crossIndex = 0; crossIndex < myBeamformer->BF_CROSSNUMBER; crossIndex++)
    {

        // If there is a pair of frames to process
        if (((crossIndex+1)/2.0) == floor((crossIndex+1)/2.0))
        {

            micIndex = myBeamformer->matrixPairs[crossIndex][0];
            micIndex2 = myBeamformer->matrixPairs[crossIndex][1];
            micIndexPrev = myBeamformer->matrixPairs[crossIndex-1][0];
            micIndex2Prev = myBeamformer->matrixPairs[crossIndex-1][1];

            // Loop for each element in the array
            for (k = 0; k < myBeamformer->MICST_FRAMESIZE; k++)
            {

                // Multiply each cross correlation and conjugate the imaginary part
                // (zeta_i[k] * X_i[k] / |X_i[k]|) * (zeta_j[k] * X_j[k]* / |X_j[k]|):
                // -> Re{(zeta_i[k] * X_i[k] / |X_i[k]|)} * Re{(zeta_j[k] * X_j[k] / |X_j[k]|)} + Im{(zeta_i[k] * X_i[k] / |X_i[k]|)} * Im{(zeta_j[k] * X_j[k] / |X_j[k]|)}
                // -> Re{(zeta_i[k] * X_i[k] / |X_i[k]|)} * Im{(zeta_j[k] * X_j[k] / |X_j[k]|)} - Im{(zeta_i[k] * X_i[k] / |X_i[k]|)} * Re{(zeta_j[k] * X_j[k] / |X_j[k]|)}
                //
                // A scaling factor N (the frame size) is used to match the result with the equation
                // R(w)_ij(tau) = sum_k=0_L-1{ (zeta_i(k) * X_i(k) * zeta_j(k) * X_j(k)* / (|X_i(k)| * |X_j(k)|)) * * exp( j * 2 * pi * k * tau / L)

                myBeamformer->workingArray1Real[k] = myBeamformer->MICST_FRAMESIZE * (myBeamformer->micArray[micIndexPrev]->weightedResultReal[k] * myBeamformer->micArray[micIndex2Prev]->weightedResultReal[k] + myBeamformer->micArray[micIndexPrev]->weightedResultImag[k] * myBeamformer->micArray[micIndex2Prev]->weightedResultImag[k]);
                myBeamformer->workingArray1Imag[k] = myBeamformer->MICST_FRAMESIZE * (myBeamformer->micArray[micIndexPrev]->weightedResultImag[k] * myBeamformer->micArray[micIndex2Prev]->weightedResultReal[k] - myBeamformer->micArray[micIndexPrev]->weightedResultReal[k] * myBeamformer->micArray[micIndex2Prev]->weightedResultImag[k]);
                myBeamformer->workingArray2Real[k] = myBeamformer->MICST_FRAMESIZE * (myBeamformer->micArray[micIndex]->weightedResultReal[k] * myBeamformer->micArray[micIndex2]->weightedResultReal[k] + myBeamformer->micArray[micIndex]->weightedResultImag[k] * myBeamformer->micArray[micIndex2]->weightedResultImag[k]);
                myBeamformer->workingArray2Imag[k] = myBeamformer->MICST_FRAMESIZE * (myBeamformer->micArray[micIndex]->weightedResultImag[k] * myBeamformer->micArray[micIndex2]->weightedResultReal[k] - myBeamformer->micArray[micIndex]->weightedResultReal[k] * myBeamformer->micArray[micIndex2]->weightedResultImag[k]);

            }

            ifftComputeTwice(myBeamformer->myFFT, myBeamformer->workingArray1Real, myBeamformer->workingArray1Imag, myBeamformer->workingArray2Real, myBeamformer->workingArray2Imag, &myBeamformer->Rij[crossIndex-1][0], &myBeamformer->Rij[crossIndex][0]);

        }

    }

    // Check if there is an IFFT left to process
    if (floor(myBeamformer->BF_CROSSNUMBER / 2) != (myBeamformer->BF_CROSSNUMBER / 2))
    {

        // Loop for each element in the array
        for (k = 0; k < myBeamformer->MICST_FRAMESIZE; k++)
        {

            // Multiply each cross correlation and conjugate the imaginary part
            // (zeta_i[k] * X_i[k] / |X_i[k]|) * (zeta_j[k] * X_j[k]* / |X_j[k]|):
            // -> Re{(zeta_i[k] * X_i[k] / |X_i[k]|)} * Re{(zeta_j[k] * X_j[k] / |X_j[k]|)} - Im{(zeta_i[k] * X_i[k] / |X_i[k]|)} * Im{(zeta_j[k] * X_j[k] / |X_j[k]|)}
            // -> Re{(zeta_i[k] * X_i[k] / |X_i[k]|)} * Im{(zeta_j[k] * X_j[k] / |X_j[k]|)} + Im{(zeta_i[k] * X_i[k] / |X_i[k]|)} * Re{(zeta_j[k] * X_j[k] / |X_j[k]|)}

            myBeamformer->workingArray1Real[k] = myBeamformer->MICST_FRAMESIZE * myBeamformer->micArray[myBeamformer->BF_CROSSNUMBER - 1]->weightedResultReal[k] * myBeamformer->micArray[myBeamformer->BF_CROSSNUMBER]->weightedResultReal[k] + myBeamformer->micArray[myBeamformer->BF_CROSSNUMBER - 1]->weightedResultImag[k] * myBeamformer->micArray[myBeamformer->BF_CROSSNUMBER]->weightedResultImag[k];
            myBeamformer->workingArray1Imag[k] = myBeamformer->MICST_FRAMESIZE * myBeamformer->micArray[myBeamformer->BF_CROSSNUMBER - 1]->weightedResultReal[k] * myBeamformer->micArray[myBeamformer->BF_CROSSNUMBER]->weightedResultImag[k] + myBeamformer->micArray[myBeamformer->BF_CROSSNUMBER - 1]->weightedResultImag[k] * myBeamformer->micArray[myBeamformer->BF_CROSSNUMBER]->weightedResultReal[k];

        }


        // Compute the IFFT
        ifftComputeOnce(myBeamformer->myFFT, myBeamformer->workingArray1Real, myBeamformer->workingArray1Imag, &myBeamformer->Rij[myBeamformer->BF_CROSSNUMBER - 1][0]);

    }

}

/*******************************************************************************
 * beamformerFindMaxima                                                        *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myBeamformer    Beamformer object in use.                      *
 *                                                                             *
 * Outputs:     mySources       The structure that holds the potential sources *
 *                              that are found with the beamformer for the     *
 *                              current frame                                  *
 *                                                                             *
 * Description: This function returns the potential sources                    *
 *                                                                             *
 ******************************************************************************/

void beamformerFindMaxima(struct objBeamformer *myBeamformer, struct objPotentialSources *mySources)
{

    // Index to loop for each source to find
    unsigned char indexSource;
    unsigned char indexSource2;

    // Index to loop through each point
    unsigned int indexPoint;

    // Index to count the number of pairs
    unsigned char indexPair;

    // Index to go through the delays
    unsigned int indexDelay;
    unsigned int indexDelayWrapped;

    // Index to go through the maxima
    signed int indexMaxima;
    signed int indexMaxima2;

    // Accumulator for the cross-correlation
    float crossCorrelationSum;
    float crossCorrelationPair;

    // Current delay for the selected point on the sphere
    unsigned short currentDelay;

    // Overall maximum angles
    float maxAngleH;
    float maxAngleV;

    // Current maximum angles
    float currentAngleH;
    float currentAngleV;
    float x;
    float y;
    float z;

    // Temporary varible
    float angleDiff;
    char maxFound;

    /***************************************************************************
    * Step 1: Compute the matrix Rij(tau)                                      *
    ***************************************************************************/

    beamformerComputeRij(myBeamformer);

    /***************************************************************************
    * Step 2: Loop for each source to be found                                 *
    ***************************************************************************/

    for (indexSource = 0; indexSource < myBeamformer->BF_MAXSOURCES; indexSource++)
    {

        /***********************************************************************
        * Step A: Find the maximum cross correlation                           *
        ***********************************************************************/

        // Init all maxima
        for (indexMaxima = 0; indexMaxima < myBeamformer->BF_NUMBERMAX; indexMaxima++)
        {

            myBeamformer->listMaximaValues[indexMaxima] = BF_NOMAX;

        }               

        // Loop for each point on the sphere
        for (indexPoint = 0; indexPoint < myBeamformer->SPHERE_NUMBERPOINTS; indexPoint++)
        {           

            // +---------------------------------------------------------------+
            // | Step I: Find the next maximum                                 |
            // +---------------------------------------------------------------+

            // Set the sum to a minimum
            crossCorrelationSum = 0;

            // Loop for each pair of microphone
            for (indexPair = 0; indexPair < myBeamformer->BF_CROSSNUMBER; indexPair++)
            {

                // Get the delay for this point on the sphere
                currentDelay = myBeamformer->delaysSphere[indexPair][indexPoint];

                // Get the cross-correlation for this delay and add to the sum
                crossCorrelationSum += myBeamformer->Rij[indexPair][currentDelay];

            }

            // +---------------------------------------------------------------+
            // | Step II: Add to the list of maxima if it is one of them       |
            // +---------------------------------------------------------------+

            for (indexMaxima = 0; indexMaxima < myBeamformer->BF_NUMBERMAX; indexMaxima++)
            {

                // Check if this maximum is smaller than one of these
                if (crossCorrelationSum < myBeamformer->listMaximaValues[indexMaxima])
                {

                    // If so then check if this maximum can be added to the list
                    if (indexMaxima > 0)
                    {

                        // Shift all previous maxima
                        for (indexMaxima2 = 0; indexMaxima2 < (indexMaxima - 1); indexMaxima2++)
                        {

                            myBeamformer->listMaximaValues[indexMaxima2] = myBeamformer->listMaximaValues[indexMaxima2 + 1];
                            myBeamformer->listMaximaIndexes[indexMaxima2] = myBeamformer->listMaximaIndexes[indexMaxima2 + 1];

                        }

                        // Copy the new maximum
                        myBeamformer->listMaximaValues[indexMaxima - 1] = crossCorrelationSum;
                        myBeamformer->listMaximaIndexes[indexMaxima - 1] = indexPoint;

                    }

                    // Then exit the loop
                    break;

                }

            }

            // Check if this beats the greatest maximum
            if (crossCorrelationSum > myBeamformer->listMaximaValues[myBeamformer->BF_NUMBERMAX-1])
            {
                myBeamformer->listMaximaValues[myBeamformer->BF_NUMBERMAX-1] = crossCorrelationSum;
                myBeamformer->listMaximaIndexes[myBeamformer->BF_NUMBERMAX-1] = indexPoint;
            }

        }

        /***********************************************************************
        * Step C: Find the best point                                          *
        ***********************************************************************/

        // This correction applies only if this is the first source
        if (indexSource == 0)
        {

            for (indexMaxima = myBeamformer->BF_NUMBERMAX-2; indexMaxima >= 0; indexMaxima--)
            {

                // Compute the position of the initial max
                x = myBeamformer->mySphere->spherePoints[myBeamformer->listMaximaIndexes[myBeamformer->BF_NUMBERMAX - 1]][0];
                y = myBeamformer->mySphere->spherePoints[myBeamformer->listMaximaIndexes[myBeamformer->BF_NUMBERMAX - 1]][1];
                z = myBeamformer->mySphere->spherePoints[myBeamformer->listMaximaIndexes[myBeamformer->BF_NUMBERMAX - 1]][2];

                // Compute the angles
                maxAngleH = atan2(y,x);
                maxAngleV = asin(z);

                // Check if there is a maximum at this place
                if (myBeamformer->listMaximaValues[indexMaxima] != BF_NOMAX)
                {

                    // Check if the point is at least a given ratio of the maximum
                    // value found
                    if (myBeamformer->listMaximaValues[indexMaxima] > (myBeamformer->BF_VALUETS * myBeamformer->listMaximaValues[myBeamformer->BF_NUMBERMAX-1]))
                    {

                        // Compute the position
                        x = myBeamformer->mySphere->spherePoints[myBeamformer->listMaximaIndexes[indexMaxima]][0];
                        y = myBeamformer->mySphere->spherePoints[myBeamformer->listMaximaIndexes[indexMaxima]][1];
                        z = myBeamformer->mySphere->spherePoints[myBeamformer->listMaximaIndexes[indexMaxima]][2];

                        // Compute the angles
                        currentAngleH = atan2(y,x);
                        currentAngleV = asin(z);

                        // Check if the difference between the horizontal angles is
                        // below a given threshold
                        if ((fabs(maxAngleH - currentAngleH)) < myBeamformer->BF_HANGLETOL)
                        {

                                // Check if the difference between the vertical angles
                                // is below a given threshold
                                if ((fabs(currentAngleV) + myBeamformer->BF_VANGLETOL) < fabs(maxAngleV))
                                {

                                        // Update the maximum
                                        myBeamformer->listMaximaValues[myBeamformer->BF_NUMBERMAX-1] = myBeamformer->listMaximaValues[indexMaxima];
                                        myBeamformer->listMaximaIndexes[myBeamformer->BF_NUMBERMAX-1] = myBeamformer->listMaximaIndexes[indexMaxima];

                                        // Update the angles
                                        maxAngleH = currentAngleH;
                                        maxAngleV = currentAngleV;

                                }

                        }

                    }

                }

            }

        }

        else
        {

            for (indexMaxima = myBeamformer->BF_NUMBERMAX-1; indexMaxima >= 0; indexMaxima--)
            {

                // Check if there is a maximum at this place
                if (myBeamformer->listMaximaValues[indexMaxima] != BF_NOMAX)
                {

                    // Compute the position
                    x = myBeamformer->mySphere->spherePoints[myBeamformer->listMaximaIndexes[indexMaxima]][0];
                    y = myBeamformer->mySphere->spherePoints[myBeamformer->listMaximaIndexes[indexMaxima]][1];
                    z = myBeamformer->mySphere->spherePoints[myBeamformer->listMaximaIndexes[indexMaxima]][2];

                    // Compute the angles
                    currentAngleH = atan2(y,x);

                    // Loop for each previous source
                    for (indexSource2 = 0; indexSource2 < indexSource; indexSource2++)
                    {

                        // Compute the angle difference
                        angleDiff = fabs(myBeamformer->prevAngleH[indexSource2] - currentAngleH);

                        // Wrap around if needed
                        if (angleDiff > M_PI)
                        {
                            angleDiff = 2*M_PI - angleDiff;
                        }

                        // Check if the difference between the horizontal angles is
                        // above a given threshold
                        if (angleDiff < myBeamformer->BF_HANGLETOLNEXT)
                        {

                            // If not eliminate this point
                            myBeamformer->listMaximaValues[indexMaxima] = BF_NOMAX;

                        }

                    }

                }

            }

            // Find first valid maxima

            maxFound = 0;

            for (indexMaxima = myBeamformer->BF_NUMBERMAX-1; indexMaxima >= 0; indexMaxima--)
            {

                // Check if there is a maximum at this place
                if (myBeamformer->listMaximaValues[indexMaxima] != BF_NOMAX)
                {

                    // Compute the position of the initial max
                    x = myBeamformer->mySphere->spherePoints[myBeamformer->listMaximaIndexes[indexMaxima]][0];
                    y = myBeamformer->mySphere->spherePoints[myBeamformer->listMaximaIndexes[indexMaxima]][1];
                    z = myBeamformer->mySphere->spherePoints[myBeamformer->listMaximaIndexes[indexMaxima]][2];

                    // Compute the angles
                    maxAngleH = atan2(y,x);
                    maxAngleV = asin(z);

                    maxFound = 1;

                    break;

                }

            }

            if (maxFound == 0)
            {

                // Compute the position of the initial max
                x = myBeamformer->mySphere->spherePoints[myBeamformer->listMaximaIndexes[myBeamformer->BF_NUMBERMAX - 1]][0];
                y = myBeamformer->mySphere->spherePoints[myBeamformer->listMaximaIndexes[myBeamformer->BF_NUMBERMAX - 1]][1];
                z = myBeamformer->mySphere->spherePoints[myBeamformer->listMaximaIndexes[myBeamformer->BF_NUMBERMAX - 1]][2];

                // Compute the angles
                maxAngleH = atan2(y,x);
                maxAngleV = asin(z);

            }

            for (indexMaxima = myBeamformer->BF_NUMBERMAX-2; indexMaxima >= 0; indexMaxima--)
            {

                // Check if there is a maximum at this place
                if (myBeamformer->listMaximaValues[indexMaxima] != BF_NOMAX)
                {

                    // Check if the point is at least a given ratio of the maximum
                    // value found
                    if (myBeamformer->listMaximaValues[indexMaxima] > (myBeamformer->BF_VALUETS * myBeamformer->listMaximaValues[myBeamformer->BF_NUMBERMAX-1]))
                    {

                        // Compute the position
                        x = myBeamformer->mySphere->spherePoints[myBeamformer->listMaximaIndexes[indexMaxima]][0];
                        y = myBeamformer->mySphere->spherePoints[myBeamformer->listMaximaIndexes[indexMaxima]][1];
                        z = myBeamformer->mySphere->spherePoints[myBeamformer->listMaximaIndexes[indexMaxima]][2];

                        // Compute the angles
                        currentAngleH = atan2(y,x);
                        currentAngleV = asin(z);

                        // Check if the difference between the vertical angles
                        // is below a given threshold
                        if ((fabs(currentAngleV) + myBeamformer->BF_VANGLETOL) < fabs(maxAngleV))
                        {

                            // Update the maximum
                            myBeamformer->listMaximaValues[myBeamformer->BF_NUMBERMAX-1] = myBeamformer->listMaximaValues[indexMaxima];
                            myBeamformer->listMaximaIndexes[myBeamformer->BF_NUMBERMAX-1] = myBeamformer->listMaximaIndexes[indexMaxima];

                            // Update the angles
                            maxAngleH = currentAngleH;
                            maxAngleV = currentAngleV;

                        }

                    }

                }

            }

        }

        // Update previous angles
        myBeamformer->prevAngleH[indexSource] = maxAngleH;
        myBeamformer->prevAngleV[indexSource] = maxAngleV;

        /***********************************************************************
        * Step D: Refine the search for the maximum                            *
        ***********************************************************************/

        // Set the sum to a minimum
        crossCorrelationSum = 0;

        // Loop for each pair
        for (indexPair = 0; indexPair < myBeamformer->BF_CROSSNUMBER; indexPair++)
        {

            // Set the sum to a minimum
            crossCorrelationPair = 0;

            // Get the delay for this point on the sphere
            currentDelay = myBeamformer->delaysSphere[indexPair][myBeamformer->listMaximaIndexes[myBeamformer->BF_NUMBERMAX-1]];

            for (indexDelay = (currentDelay - myBeamformer->BF_RANGE); indexDelay <= (currentDelay + myBeamformer->BF_RANGE); indexDelay++)
            {
                    // Wrap around if needed
                    if (indexDelay < 0)
                    {
                            indexDelayWrapped = indexDelay + myBeamformer->MICST_FRAMESIZE;
                    }
                    else if (indexDelay >= myBeamformer->MICST_FRAMESIZE)
                    {
                            indexDelayWrapped = indexDelay - myBeamformer->MICST_FRAMESIZE;
                    }
                    else
                    {
                            indexDelayWrapped = indexDelay;
                    }

                    // Keep maximum
                    if (myBeamformer->Rij[indexPair][indexDelayWrapped] > crossCorrelationPair)
                    {
                            crossCorrelationPair = myBeamformer->Rij[indexPair][indexDelayWrapped];
                    }

            }

            // Add to the sum
            crossCorrelationSum += crossCorrelationPair;

        }

        // Save this point (corrected or not)
        mySources->sourcesPosition[indexSource][0] = myBeamformer->mySphere->spherePoints[myBeamformer->listMaximaIndexes[myBeamformer->BF_NUMBERMAX - 1]][0];
        mySources->sourcesPosition[indexSource][1] = myBeamformer->mySphere->spherePoints[myBeamformer->listMaximaIndexes[myBeamformer->BF_NUMBERMAX - 1]][1];
        mySources->sourcesPosition[indexSource][2] = myBeamformer->mySphere->spherePoints[myBeamformer->listMaximaIndexes[myBeamformer->BF_NUMBERMAX - 1]][2];
        mySources->sourcesEnergy[indexSource] = crossCorrelationSum;

        /***********************************************************************
        * Step E: Set the maximum cross correlation to zero                    *
        ***********************************************************************/

        // Set the value of the corresponding delays to zero for next pass
        for (indexPair = 0; indexPair < myBeamformer->BF_CROSSNUMBER; indexPair++)
        {

            // Get the delay for this point on the sphere
            currentDelay = myBeamformer->delaysSphere[indexPair][myBeamformer->listMaximaIndexes[myBeamformer->BF_NUMBERMAX-1]];

            for (indexDelay = (currentDelay - myBeamformer->BF_RIJDELAYS); indexDelay <= (currentDelay + myBeamformer->BF_RIJDELAYS); indexDelay++)
            {
                // Wrap around if needed
                if (indexDelay < 0)
                {
                    indexDelayWrapped = indexDelay + myBeamformer->MICST_FRAMESIZE;
                }
                else if (indexDelay >= myBeamformer->MICST_FRAMESIZE)
                {
                    indexDelayWrapped = indexDelay - myBeamformer->MICST_FRAMESIZE;
                }
                else
                {
                    indexDelayWrapped = indexDelay;
                }

                // Set this correlation to zero
                myBeamformer->Rij[indexPair][indexDelayWrapped] = 0;

            }

        }

    }

}

/*******************************************************************************
 * generateWindow                                                              *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      window      Pointer to the array that will contain the window  *
 *              len         Size of the window                                 *
 *                                                                             *
 * Outputs:     window      This array is filled with the correct window       *
 *                          elements                                           *
 *                                                                             *
 * Description: This function generate an hanning window with the following    *
 *              shape:                                                         *
 *                                                                             *
 *              w[n] = 0.5 * (1 - cos (2*pi*n/(N-1))                           *
 *                                                                             *
 ******************************************************************************/

void generateWindow(float *window, int len)
{

    int k;

    float lenf;

    lenf = ((float) len) - 1.0;

    for (k = 0; k < len; k++)
    {
        window[k] = 0.5 * ( 1.0 - cos( (2 * M_PI * k) / lenf ) );
    }

}
