/**



  */
//Standard C includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Manyears includes
#include "beamformer.h"
#include "mixture.h"

#define SAMPLES_PER_FRAME 512
#define NB_MICROPHONES 8
#define RAW_BUFFER_SIZE (SAMPLES_PER_FRAME * NB_MICROPHONES)


void setup_microphone_positions_and_gains(struct ParametersStruct *parametersStruct)
{
	// +-------------------------------------------------------+
	// | i. Microphone 1                                       |
	// +-------------------------------------------------------+
	
	// Define the gain of each microphone to equalize
	parametersStruct->P_BF_MICSGAIN_1 = 1.0476;
	
	// +-------------------------------------------+
	// | a. Position                               |
	// +-------------------------------------------+
	
	// Define the position of each microphone on the cube (in meters)
	// The center of the cube is at the origin
	parametersStruct->P_BF_MICSPOSITIONS_1_X = -0.215;
	parametersStruct->P_BF_MICSPOSITIONS_1_Y = +0.1875;
	parametersStruct->P_BF_MICSPOSITIONS_1_Z = -0.15;
	
	// +-------------------------------------------------------+
	// | ii. Microphone 2                                      |
	// +-------------------------------------------------------+
	
	// Define the gain of each microphone to equalize
	parametersStruct->P_BF_MICSGAIN_2 = 1.0000;
	
	// +-------------------------------------------+
	// | a. Position                               |
	// +-------------------------------------------+
	
	// Define the position of each microphone on the cube (in meters)
	// The center of the cube is at the origin
	parametersStruct->P_BF_MICSPOSITIONS_2_X = -0.215;
	parametersStruct->P_BF_MICSPOSITIONS_2_Y = +0.1875;
	parametersStruct->P_BF_MICSPOSITIONS_2_Z = +0.15;
	
	// +-------------------------------------------------------+
	// | iii. Microphone 3                                     |
	// +-------------------------------------------------------+
	
	// Define the gain of each microphone to equalize
	parametersStruct->P_BF_MICSGAIN_3 = 2.2288;
	
	// +-------------------------------------------+
	// | a. Position                               |
	// +-------------------------------------------+
	
	// Define the position of each microphone on the cube (in meters)
	// The center of the cube is at the origin
	parametersStruct->P_BF_MICSPOSITIONS_3_X = +0.215;
	parametersStruct->P_BF_MICSPOSITIONS_3_Y = +0.1875;
	parametersStruct->P_BF_MICSPOSITIONS_3_Z = -0.15;
	
	// +-------------------------------------------------------+
	// | iv. Microphone 4                                      |
	// +-------------------------------------------------------+
	
	// Define the gain of each microphone to equalize
	parametersStruct->P_BF_MICSGAIN_4 = 1.5242;
	
	// +-------------------------------------------+
	// | a. Position                               |
	// +-------------------------------------------+
	
	// Define the position of each microphone on the cube (in meters)
	// The center of the cube is at the origin
	parametersStruct->P_BF_MICSPOSITIONS_4_X = +0.215;
	parametersStruct->P_BF_MICSPOSITIONS_4_Y = +0.1875;
	parametersStruct->P_BF_MICSPOSITIONS_4_Z = +0.15;
	
	// +-------------------------------------------------------+
	// | v. Microphone 5                                       |
	// +-------------------------------------------------------+
	
	// Define the gain of each microphone to equalize
	parametersStruct->P_BF_MICSGAIN_5 = 1.3265;
	
	// +-------------------------------------------+
	// | a. Position                               |
	// +-------------------------------------------+
	
	// Define the position of each microphone on the cube (in meters)
	// The center of the cube is at the origin
	parametersStruct->P_BF_MICSPOSITIONS_5_X = -0.215;
	parametersStruct->P_BF_MICSPOSITIONS_5_Y = -0.1875;
	parametersStruct->P_BF_MICSPOSITIONS_5_Z = -0.15;
	
	// +-------------------------------------------------------+
	// | vi. Microphone 6                                      |
	// +-------------------------------------------------------+
	
	// Define the gain of each microphone to equalize
	parametersStruct->P_BF_MICSGAIN_6 = 1.9480;
	
	// +-------------------------------------------+
	// | a. Position                               |
	// +-------------------------------------------+
	
	// Define the position of each microphone on the cube (in meters)
	// The center of the cube is at the origin
	parametersStruct->P_BF_MICSPOSITIONS_6_X = -0.215;
	parametersStruct->P_BF_MICSPOSITIONS_6_Y = -0.1875;
	parametersStruct->P_BF_MICSPOSITIONS_6_Z = +0.15;
	
	// +-------------------------------------------------------+
	// | vii. Microphone 7                                     |
	// +-------------------------------------------------------+
	
	// Define the gain of each microphone to equalize
	parametersStruct->P_BF_MICSGAIN_7 = 1.0350;
	
	// +-------------------------------------------+
	// | a. Position                               |
	// +-------------------------------------------+
	
	// Define the position of each microphone on the cube (in meters)
	// The center of the cube is at the origin
	parametersStruct->P_BF_MICSPOSITIONS_7_X = +0.215;
	parametersStruct->P_BF_MICSPOSITIONS_7_Y = -0.1875;
	parametersStruct->P_BF_MICSPOSITIONS_7_Z = -0.15;
	
	// +-------------------------------------------------------+
	// | viii. Microphone 8                                    |
	// +-------------------------------------------------------+
	
	// Define the gain of each microphone to equalize
	parametersStruct->P_BF_MICSGAIN_8 = 1.9145;
	
	// +-------------------------------------------+
	// | a. Position                               |
	// +-------------------------------------------+
	
	// Define the position of each microphone on the cube (in meters)
	// The center of the cube is at the origin
	parametersStruct->P_BF_MICSPOSITIONS_8_X = +0.215;
	parametersStruct->P_BF_MICSPOSITIONS_8_Y = -0.1875;
	parametersStruct->P_BF_MICSPOSITIONS_8_Z = +0.15;
	
	
}





int main (int argc, char* argv[])
{
    int file_index = 1;
    int frame_number = 0;
    unsigned char channel = 0;
    int frame_index = 0;
    int source_index = 0;

    short audio_raw_data[RAW_BUFFER_SIZE];
    float audio_float_data[NB_MICROPHONES][SAMPLES_PER_FRAME];

    struct objBeamformer currentBeamformer;
    struct objMixture currentMixture;
    struct objPotentialSources currentPotentialSources;
    struct objTrackedSources currentTrackedSources;

    struct ParametersStruct currentParameters;

    //Default parameters
    ParametersLoadDefault(&currentParameters);
	
	//Change your microphone positions here...
	//This is a copy of what is in ParametersLoadDefault right now...
	setup_microphone_positions_and_gains(&currentParameters);
	
	
	
    if (argc < 2)
    {
        printf("Usage %s ''raw files''\n",argv[0]);
        return -1;
    }
	
    for (file_index = 1; file_index < argc; file_index++)
    {
        printf("Processing : %s \n",argv[file_index]);

        //Open file in binary mode
        FILE* filePtr = fopen(argv[file_index], "rb");

        if (filePtr == NULL)
        {

            printf("Invalid file %s \n",argv[file_index]);
            continue;
        }

        // Initialize the beamformer
        beamformerInit(&currentBeamformer, &currentParameters);

        // Initialize the mixture
        mixtureInit(&currentMixture, &currentParameters);

        // Initialize the potential sources
        potentialSourcesInit(&currentPotentialSources, &currentParameters);

        //Initialize the sources
        trackedSourcesInit(&currentTrackedSources, &currentParameters);

        //reset frame number
        frame_number = 0;

        //set data to zero
        memset(audio_raw_data,0,RAW_BUFFER_SIZE * sizeof(short));

        //Read & process all data
        while(fread(audio_raw_data,sizeof(short),RAW_BUFFER_SIZE,filePtr) > 0)
        {
            printf("Processing frame : %i \n",frame_number);

            //#1 - Let's create the float data for processing
            for (channel = 0; channel < NB_MICROPHONES; channel++)
            {
                for (frame_index = 0; frame_index < SAMPLES_PER_FRAME; frame_index++)
                {
                    audio_float_data[channel][frame_index] = (float) audio_raw_data[channel + (NB_MICROPHONES * frame_index)];
                }

                // Copy frames to the beamformer frames, will do 50% overlap internally
                beamformerPushFrames(&currentBeamformer, SAMPLES_PER_FRAME, channel);
                beamformerAddFrame(&currentBeamformer, &audio_float_data[channel][0], channel, SAMPLES_PER_FRAME);
            }

            //#2 Find potential sources from the beamformer
            beamformerFindMaxima(&currentBeamformer, &currentPotentialSources);

            //#3 Track Sources
            mixtureUpdate(&currentMixture, &currentTrackedSources, &currentPotentialSources);

            //#4 Output result
            for (source_index = 0; source_index < currentParameters.P_MIXTURE_NBFILTERS; source_index++)
            {

                /*printf("source index : %i X:%f, Y:%f, Z:%f, id: %i \n",source_index,
                       currentTrackedSources.sourcesPosition[source_index][0],
                       currentTrackedSources.sourcesPosition[source_index][1],
                       currentTrackedSources.sourcesPosition[source_index][2],
                       currentTrackedSources.sourcesID[source_index]);*/
                printf("source index bis : %i X:%f, Y:%f, Z:%f, id: %i \n",source_index,
                       trackedSourcesGetX(&currentTrackedSources, source_index),
                       trackedSourcesGetY(&currentTrackedSources, source_index),
                       trackedSourcesGetZ(&currentTrackedSources, source_index),
                       currentTrackedSources.sourcesID[source_index]);

            }



            //next frame
            frame_number++;
        }

        //terminate
        beamformerTerminate(&currentBeamformer);
        mixtureTerminate(&currentMixture);
        potentialSourcesTerminate(&currentPotentialSources);
        trackedSourcesTerminate(&currentTrackedSources);

        //Close file (will cleanup memory)
        fclose(filePtr);

    }

    return 0;
}
