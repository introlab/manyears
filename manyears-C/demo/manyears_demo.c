/**



  */
//Standard C includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Manyears includes
#include "overallContext.h"

// -- To be tested
#include "Output/outputSingleStream.h"
// --

#define SAMPLES_PER_FRAME 512
#define NB_MICROPHONES 8
#define RAW_BUFFER_SIZE (SAMPLES_PER_FRAME * NB_MICROPHONES)

void setup_microphone_positions_and_gains(struct ParametersStruct *parametersStruct, struct objMicrophones* myMicrophones)
{

    // Set the number of microphones
    microphonesInit(myMicrophones, 8);

    // Add microphone 1...
    microphonesAdd(myMicrophones,
                   0,
                   parametersStruct->P_GEO_MICS_MIC1_X,
                   parametersStruct->P_GEO_MICS_MIC1_Y,
                   parametersStruct->P_GEO_MICS_MIC1_Z,
                   parametersStruct->P_GEO_MICS_MIC1_GAIN
                   );

    // Add microphone 2...
    microphonesAdd(myMicrophones,
                   1,
                   parametersStruct->P_GEO_MICS_MIC2_X,
                   parametersStruct->P_GEO_MICS_MIC2_Y,
                   parametersStruct->P_GEO_MICS_MIC2_Z,
                   parametersStruct->P_GEO_MICS_MIC2_GAIN
                   );

    // Add microphone 3...
    microphonesAdd(myMicrophones,
                   2,
                   parametersStruct->P_GEO_MICS_MIC3_X,
                   parametersStruct->P_GEO_MICS_MIC3_Y,
                   parametersStruct->P_GEO_MICS_MIC3_Z,
                   parametersStruct->P_GEO_MICS_MIC3_GAIN
                   );

    // Add microphone 4...
    microphonesAdd(myMicrophones,
                   3,
                   parametersStruct->P_GEO_MICS_MIC4_X,
                   parametersStruct->P_GEO_MICS_MIC4_Y,
                   parametersStruct->P_GEO_MICS_MIC4_Z,
                   parametersStruct->P_GEO_MICS_MIC4_GAIN
                   );

    // Add microphone 5...
    microphonesAdd(myMicrophones,
                   4,
                   parametersStruct->P_GEO_MICS_MIC5_X,
                   parametersStruct->P_GEO_MICS_MIC5_Y,
                   parametersStruct->P_GEO_MICS_MIC5_Z,
                   parametersStruct->P_GEO_MICS_MIC5_GAIN
                   );

    // Add microphone 6...
    microphonesAdd(myMicrophones,
                   5,
                   parametersStruct->P_GEO_MICS_MIC6_X,
                   parametersStruct->P_GEO_MICS_MIC6_Y,
                   parametersStruct->P_GEO_MICS_MIC6_Z,
                   parametersStruct->P_GEO_MICS_MIC6_GAIN
                   );

    // Add microphone 7...
    microphonesAdd(myMicrophones,
                   6,
                   parametersStruct->P_GEO_MICS_MIC7_X,
                   parametersStruct->P_GEO_MICS_MIC7_Y,
                   parametersStruct->P_GEO_MICS_MIC7_Z,
                   parametersStruct->P_GEO_MICS_MIC7_GAIN
                   );

    // Add microphone 8...
    microphonesAdd(myMicrophones,
                   7,
                   parametersStruct->P_GEO_MICS_MIC8_X,
                   parametersStruct->P_GEO_MICS_MIC8_Y,
                   parametersStruct->P_GEO_MICS_MIC8_Z,
                   parametersStruct->P_GEO_MICS_MIC8_GAIN
                   );


}





int main (int argc, char* argv[])
{
    int file_index = 1;
    int frame_number = 0;
    unsigned char channel = 0;
    int frame_index = 0;
    int source_index = 0;
    int source_id;
    float x,y,z;

    short audio_raw_data[RAW_BUFFER_SIZE];
    float audio_float_data[NB_MICROPHONES][SAMPLES_PER_FRAME];

    struct objOverall workspace;

    const char* sepFileName = "separated*****.wav";
    const char* postFileName = "postfiltered*****.wav";
    const char* trackFileName = "tracked.dat";
    const char* separator = "-";
    char* sepPath;
    char* postPath;
    char* trackPath;
    FILE* filePtr = NULL;
    FILE* trackFilePtr = NULL;

    // Create the workspace
    workspace = createEmptyOverallContext();

    //Default parameters
    ParametersLoadDefault(workspace.myParameters);

    if (argc != 3)
    {
        printf("Usage %s ''raw files''\n",argv[0]);
        return -1;
    }

    if (argv[1][1] == 'r')
    {

        // Some fine tunings of the parameters for the robot
        workspace.myParameters->P_BF_ET = 450;
        workspace.myParameters->P_MIXTURE_NEWTHRESHOLD = 0.3;
        workspace.myParameters->P_MIXTURE_CUMULATIVETIME1 = 150;
        workspace.myParameters->P_MIXTURE_TOBS1 = 0.2;
        workspace.myParameters->P_MIXTURE_CUMULATIVETIME2 = 400;
        workspace.myParameters->P_MIXTURE_TOBS2 = 0.99;
        workspace.myParameters->P_GSS_MU = 0.02;
        workspace.myParameters->P_OUT_GAIN = 10;
        workspace.myParameters->P_FILTER_INERTIAX = 0.5;
        workspace.myParameters->P_FILTER_INERTIAY = 0.5;
        workspace.myParameters->P_FILTER_INERTIAZ = 0.05;
        workspace.myParameters->P_FILTER_NEWSTOP = 0.50;
        workspace.myParameters->P_FILTER_NEWCONST = 0.40;
        workspace.myParameters->P_FILTER_NEWEXC = 0.10;
        workspace.myParameters->P_GEN_DYNSOURCES = 2;

        workspace.myParameters->P_GEO_MICS_MIC1_X = 0.02999999933;
        workspace.myParameters->P_GEO_MICS_MIC1_Y = -0.1000000015;
        workspace.myParameters->P_GEO_MICS_MIC1_Z = -0.01999999955;

        workspace.myParameters->P_GEO_MICS_MIC2_X = 0.1000000015;
        workspace.myParameters->P_GEO_MICS_MIC2_Y = 0;
        workspace.myParameters->P_GEO_MICS_MIC2_Z = -0.200000003;

        workspace.myParameters->P_GEO_MICS_MIC3_X = 0.1199999973;
        workspace.myParameters->P_GEO_MICS_MIC3_Y = 0;
        workspace.myParameters->P_GEO_MICS_MIC3_Z = -0.6899999976;

        workspace.myParameters->P_GEO_MICS_MIC4_X = 0.02999999933;
        workspace.myParameters->P_GEO_MICS_MIC4_Y = 0.1000000015;
        workspace.myParameters->P_GEO_MICS_MIC4_Z = -0.01999999955;

        workspace.myParameters->P_GEO_MICS_MIC5_X = -0.1700000018;
        workspace.myParameters->P_GEO_MICS_MIC5_Y = 0.1099999994;
        workspace.myParameters->P_GEO_MICS_MIC5_Z = -0.2700000107;

        workspace.myParameters->P_GEO_MICS_MIC6_X = -0.1700000018;
        workspace.myParameters->P_GEO_MICS_MIC6_Y = 0.1099999994;
        workspace.myParameters->P_GEO_MICS_MIC6_Z = -0.7200000286;

        workspace.myParameters->P_GEO_MICS_MIC7_X = -0.1700000018;
        workspace.myParameters->P_GEO_MICS_MIC7_Y = -0.1099999994;
        workspace.myParameters->P_GEO_MICS_MIC7_Z = -0.7200000286;

        workspace.myParameters->P_GEO_MICS_MIC8_X = -0.1700000018;
        workspace.myParameters->P_GEO_MICS_MIC8_Y = -0.1099999994;
        workspace.myParameters->P_GEO_MICS_MIC8_Z = -0.2700000107;

    }
    else
    {

        // Some fine tunings of the parameters for the cube
        workspace.myParameters->P_BF_ET = 600;
        workspace.myParameters->P_MIXTURE_NEWTHRESHOLD = 0.1;
        workspace.myParameters->P_MIXTURE_CUMULATIVETIME1 = 150;
        workspace.myParameters->P_MIXTURE_TOBS1 = 0.2;
        workspace.myParameters->P_MIXTURE_CUMULATIVETIME2 = 200;
        workspace.myParameters->P_MIXTURE_TOBS2 = 0.99;
        workspace.myParameters->P_GSS_MU = 0.02;
        workspace.myParameters->P_OUT_GAIN = 10;
        workspace.myParameters->P_FILTER_INERTIAX = 0.5;
        workspace.myParameters->P_FILTER_INERTIAY = 0.5;
        workspace.myParameters->P_FILTER_INERTIAZ = 0.05;
        workspace.myParameters->P_FILTER_NEWSTOP = 0.50;
        workspace.myParameters->P_FILTER_NEWCONST = 0.40;
        workspace.myParameters->P_FILTER_NEWEXC = 0.10;
        workspace.myParameters->P_GEN_DYNSOURCES = 2;

    }

    // Initialize the microphones
    microphonesInit(workspace.myMicrophones, NB_MICROPHONES);

    //Change your microphone positions here...
    //This is a copy of what is in ParametersLoadDefault right now...
    setup_microphone_positions_and_gains(workspace.myParameters, workspace.myMicrophones);

    file_index = 2;

    printf("Processing : %s \n",argv[file_index]);

    //Open file in binary mode
    filePtr = fopen(argv[file_index], "rb");

    if (filePtr == NULL)
    {

        printf("Invalid file %s \n",argv[file_index]);
        return -1;
    }

    // Generate the paths for the separated and postfiltered sources
    sepPath = (char*) malloc(sizeof(char) * 1024);
    postPath = (char*) malloc(sizeof(char) * 1024);
    trackPath = (char*) malloc(sizeof(char) * 1024);

    strcpy(sepPath, argv[file_index]);
    strcat(sepPath, separator);
    strcat(sepPath, sepFileName);
    strcpy(postPath, argv[file_index]);
    strcat(postPath, separator);
    strcat(postPath, postFileName);
    strcpy(trackPath, argv[file_index]);
    strcat(trackPath, separator);
    strcat(trackPath, trackFileName);

    // Initialize the preprocessor
    preprocessorInit(workspace.myPreprocessor, workspace.myParameters, workspace.myMicrophones);

    // Initialize the beamformer
    beamformerInit(workspace.myBeamformer, workspace.myParameters, workspace.myMicrophones);

    // Initialize the mixture
    mixtureInit(workspace.myMixture, workspace.myParameters);

    // Initialize the gss
    gssInit(workspace.myGSS, workspace.myParameters, workspace.myMicrophones);

    // Initialize the postfilter
    postfilterInit(workspace.myPostfilter, workspace.myParameters);

    // Initialize the postprocessor
    postprocessorInit(workspace.myPostprocessorSeparated, workspace.myParameters);
    postprocessorInit(workspace.myPostprocessorPostfiltered, workspace.myParameters);

    // Initialize the potential sources
    potentialSourcesInit(workspace.myPotentialSources, workspace.myParameters);

    // Initialize the tracked sources
    trackedSourcesInit(workspace.myTrackedSources, workspace.myParameters);

    // Initialize the separated sources
    separatedSourcesInit(workspace.mySeparatedSources, workspace.myParameters);

    // Initialize the postfiltered sources
    postfilteredSourcesInit(workspace.myPostfilteredSources, workspace.myParameters);

    // Initialize the output
    outputInit(workspace.myOutputSeparated, workspace.myParameters, NULL, NULL, sepPath, '*');
    outputInit(workspace.myOutputPostfiltered, workspace.myParameters, NULL, NULL, postPath, '*');

    //reset frame number
    frame_number = 0;

    //set data to zero
    memset(audio_raw_data,0,RAW_BUFFER_SIZE * sizeof(short));

    // Create file with tracked sources
    trackFilePtr = fopen(trackPath, "wb");
    fclose(trackFilePtr);

    while(fread(audio_raw_data,sizeof(short),RAW_BUFFER_SIZE,filePtr) > 0)
    {

        printf("Processing frame %i:\n",frame_number);

        //#1 - Let's create the float data for processing
        for (channel = 0; channel < NB_MICROPHONES; channel++)
        {
            for (frame_index = 0; frame_index < SAMPLES_PER_FRAME; frame_index++)
            {
                audio_float_data[channel][frame_index] = ((float) audio_raw_data[channel + (NB_MICROPHONES * frame_index)]) / 32768.0;
            }

            // Copy frames to the beamformer frames, will do 50% overlap internally
            preprocessorPushFrames(workspace.myPreprocessor, SAMPLES_PER_FRAME, channel);
            preprocessorAddFrame(workspace.myPreprocessor, &audio_float_data[channel][0], channel, SAMPLES_PER_FRAME);
        }

        //#2 Preprocess
        preprocessorProcessFrame(workspace.myPreprocessor);

        //#3 Find potential sources from the beamformer
        beamformerFindMaxima(workspace.myBeamformer, workspace.myPreprocessor, workspace.myPotentialSources);

        //#4 Track Sources
        mixtureUpdate(workspace.myMixture, workspace.myTrackedSources, workspace.myPotentialSources);

        //#5 Separate sources
        gssProcess(workspace.myGSS, workspace.myPreprocessor, workspace.myTrackedSources, workspace.mySeparatedSources);
        postfilterProcess(workspace.myPostfilter, workspace.mySeparatedSources, workspace.myPreprocessor, workspace.myPostfilteredSources);

        //#6 Postprocess
        postprocessorProcessFrameSeparated(workspace.myPostprocessorSeparated, workspace.myTrackedSources, workspace.mySeparatedSources);
        postprocessorProcessFramePostfiltered(workspace.myPostprocessorPostfiltered, workspace.myTrackedSources, workspace.myPostfilteredSources);

        //#7 Output results in files
        outputProcess(workspace.myOutputSeparated, workspace.myPostprocessorSeparated);
        outputProcess(workspace.myOutputPostfiltered, workspace.myPostprocessorPostfiltered);

        //#8 Output results in terminal and file
        trackFilePtr = fopen(trackPath, "ab");

        for (source_index = 0; source_index < trackedSourcesGetMaxNumberSources(workspace.myTrackedSources); source_index++)
        {
            source_id = trackedSourcesGetID(workspace.myTrackedSources,source_index);

            if (source_id != ID_NOSOURCE)
            {
                x = trackedSourcesGetX(workspace.myTrackedSources,source_index);
                y = trackedSourcesGetY(workspace.myTrackedSources,source_index);
                z = trackedSourcesGetZ(workspace.myTrackedSources,source_index);
            }
            else
            {
                x = 0.0;
                y = 0.0;
                z = 0.0;
            }

            printf("Source ID: %i           x: %f y: %f z: %f\n",source_id,x,y,z);
            fprintf(trackFilePtr, "%i %f %f %f\n", source_id, x, y, z);

        }

        fprintf(trackFilePtr, "\n");
        fclose(trackFilePtr);

        printf("\n");

        //next frame
        frame_number++;
    }

    //terminate
    preprocessorTerminate(workspace.myPreprocessor);
    beamformerTerminate(workspace.myBeamformer);
    mixtureTerminate(workspace.myMixture);
    gssTerminate(workspace.myGSS);
    postfilterTerminate(workspace.myPostfilter);
    postprocessorTerminate(workspace.myPostprocessorSeparated);
    postprocessorTerminate(workspace.myPostprocessorPostfiltered);

    potentialSourcesTerminate(workspace.myPotentialSources);
    trackedSourcesTerminate(workspace.myTrackedSources);
    separatedSourcesTerminate(workspace.mySeparatedSources);
    postfilteredSourcesTerminate(workspace.myPostfilteredSources);

    outputTerminate(workspace.myOutputSeparated);
    outputTerminate(workspace.myOutputPostfiltered);

    //Close file (will cleanup memory)
    fclose(filePtr);

    microphonesTerminate(workspace.myMicrophones);

    return 0;
}
