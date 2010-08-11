/******************************************************************************* 
 * ManyEars: Beamformer - Header                                               *
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

#ifndef BEAMFORMER_H
#define BEAMFORMER_H

#include <stdlib.h>

#include "parameters.h"
#include "sphere.h"
#include "potentialSources.h"

// =============================================================================

/*******************************************************************************
 * Types (Do not edit)                                                         *
 ******************************************************************************/

        // Maximum state
        #define     BF_NOMAX           -1     // Indicates there is no max

/*******************************************************************************
 * Structures                                                                  *
 ******************************************************************************/

struct objBeamformer
{
    // +-------------------------------------------------------------------+
    // | Parameters                                                        |
    // +-------------------------------------------------------------------+

    // Define the number of microphones
    int BF_MICSNUMBER;

    // Define the position of each microphone on the cube (in meters)
    // The center of the cube is at the origin
    float BF_MICSPOSITIONS_1_X;
    float BF_MICSPOSITIONS_1_Y;
    float BF_MICSPOSITIONS_1_Z;
    float BF_MICSPOSITIONS_2_X;
    float BF_MICSPOSITIONS_2_Y;
    float BF_MICSPOSITIONS_2_Z;
    float BF_MICSPOSITIONS_3_X;
    float BF_MICSPOSITIONS_3_Y;
    float BF_MICSPOSITIONS_3_Z;
    float BF_MICSPOSITIONS_4_X;
    float BF_MICSPOSITIONS_4_Y;
    float BF_MICSPOSITIONS_4_Z;
    float BF_MICSPOSITIONS_5_X;
    float BF_MICSPOSITIONS_5_Y;
    float BF_MICSPOSITIONS_5_Z;
    float BF_MICSPOSITIONS_6_X;
    float BF_MICSPOSITIONS_6_Y;
    float BF_MICSPOSITIONS_6_Z;
    float BF_MICSPOSITIONS_7_X;
    float BF_MICSPOSITIONS_7_Y;
    float BF_MICSPOSITIONS_7_Z;
    float BF_MICSPOSITIONS_8_X;
    float BF_MICSPOSITIONS_8_Y;
    float BF_MICSPOSITIONS_8_Z;

    // Define the gain of each microphone to equalize
    float BF_MICSGAIN_1;
    float BF_MICSGAIN_2;
    float BF_MICSGAIN_3;
    float BF_MICSGAIN_4;
    float BF_MICSGAIN_5;
    float BF_MICSGAIN_6;
    float BF_MICSGAIN_7;
    float BF_MICSGAIN_8;

    // Define the maximum number of sources that can be found
    int BF_MAXSOURCES;

    // Define the number of maxima to keep in memory in order to find
    // the one that best fits
    int BF_NUMBERMAX;

    // Define the value threshold for the maxima comparison
    float BF_VALUETS;

    // Define the horizontal angle tolerance for first source
    float BF_HANGLETOL;

    // Define the horizontal angle tolerance for next sources
    float BF_HANGLETOLNEXT;

    // Define the vertical angle tolerance
    float BF_VANGLETOL;

    // Define the range where the neighbour delays are used to refine
    // the result
    int BF_RANGE;

    // Define the number of delays next to the main delay to set to zero
    // to find the peaks after the first one
    int BF_RIJDELAYS;

    // Calculate the number of possibilities given the number of microphones
    int BF_CROSSNUMBER;

    // Size of a frame
    int MICST_FRAMESIZE;

    // Number of levels on the sphere
    int SPHERE_NUMBERLEVELS;

    // Number of points on the sphere
    int SPHERE_NUMBERPOINTS;

    // Sampling rate
    int MICST_FS;

    // Speed of sound
    float MICST_C;

    // +-------------------------------------------------------------------+
    // | Variables                                                         |
    // +-------------------------------------------------------------------+

    // Microphone sound tracks
    struct objMicST** micArray;

    // Define the matrix to match each pair of microphones
    // Size: [BF_CROSSNUMBER][2]
    unsigned char** matrixPairs;

    // Define the matrix Rij
    // Size: [BF_CROSSNUMBER][MICST_FRAMESIZE]
    float** Rij;

    // Create a unit sphere
    struct objSphere* mySphere;

    // Create a FFT object
    struct objFFT* myFFT;

    // Define the delays for each point on the sphere
    // Size: [BF_CROSSNUMBER][SPHERE_NUMBERPOINTS]
    unsigned short** delaysSphere;

    // List of maxima
    unsigned int* listMaximaIndexes;
    float* listMaximaValues;

    // Window
    // Size: [MICST_FRAMESIZE]
    float* hanningWindow;

    // Positions
    float** positions;

    // Gains
    float* gains;  

    // Arrays to manipulate frames
    // Size: [MICST_FRAMESIZE]
    float* workingArray1Real;
    // Size: [MICST_FRAMESIZE]
    float* workingArray1Imag;
    // Size: [MICST_FRAMESIZE]
    float* workingArray2Real;
    // Size: [MICST_FRAMESIZE]
    float* workingArray2Imag;

    // Previous source angles
    // Size: BF_MAXSOURCES
    float* prevAngleH;
    // Size: BF_MAXSOURCES
    float* prevAngleV;

};

/*******************************************************************************
 * Prototypes                                                                  *
 ******************************************************************************/

        void beamformerInit(struct objBeamformer *myBeamformer, struct ParametersStruct *myParameters);

        void beamformerTerminate(struct objBeamformer *myBeamformer);

        void beamformerPushFrames(struct objBeamformer *myBeamformer, unsigned int shiftSize, unsigned int micIndex);

        void beamformerAddFrame(struct objBeamformer *myBeamformer, float *newFrame, unsigned char micIndex, unsigned int sizeFrame);

        void beamformerComputeRij(struct objBeamformer *myBeamformer);

        void beamformerFindMaxima(struct objBeamformer *myBeamformer, struct objPotentialSources *mySources);

        void generateWindow(float *window, int len);

// =============================================================================

#endif
