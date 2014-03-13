/**



  */
//Standard C includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <RtAudio.h>
#include <unistd.h>

#define SAMPLING_RATE 48000
#define SAMPLES_PER_FRAME 512
#define NB_MICROPHONES 8
#define RAW_BUFFER_SIZE (SAMPLES_PER_FRAME * NB_MICROPHONES)



using namespace std;

//Manyears includes
extern "C"
{
    #include "overallContext.h"
}

//prototype
int record( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
             double streamTime, RtAudioStreamStatus status, void *userData );

class AudioCapture
{
public:
    AudioCapture()
        : rtaudio_(NULL), workspaceReady_(false)
    {
        if (initSoundCard())
        {
            initWorkspace();

            //Start stream
            try
            {
                rtaudio_->startStream();
            }
            catch (RtError& e)
            {
                e.printMessage();
            }
        }
    }

    ~AudioCapture()
    {
        if (rtaudio_)
        {
            try
            {
                rtaudio_->stopStream();
            }
            catch (RtError& e)
            {
                e.printMessage();
            }

            delete rtaudio_;
            rtaudio_ = NULL;


            //Cleanup workspace
            destroyWorkspace();
        }
    }

    void destroyWorkspace()
    {
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
        microphonesTerminate(workspace.myMicrophones);

        cerr << "Workspace terminated"<<endl;
        workspaceReady_ = false;
    }

    bool processFrame(double streamTime, short *frames, unsigned int frame_size, int nb_channels)
    {
        if ((frames != NULL) &&
                (frame_size == SAMPLES_PER_FRAME) &&
                (nb_channels == NB_MICROPHONES) &&
                workspaceReady_ == true)
        {
            cout << streamTime<< " processing..." << endl;
            static float audio_float_data[NB_MICROPHONES][SAMPLES_PER_FRAME];

            //#1 - Let's create the float data for processing
            for (unsigned int channel = 0; channel < nb_channels; channel++)
            {
                for (unsigned int frame_index = 0; frame_index < frame_size; frame_index++)
                {
                    audio_float_data[channel][frame_index] = ((float) frames[channel + (nb_channels * frame_index)]) / 32768.0;
                }

                // Copy frames to the beamformer frames, will do 50% overlap internally
                preprocessorPushFrames(workspace.myPreprocessor, frame_size, channel);
                preprocessorAddFrame(workspace.myPreprocessor, &audio_float_data[channel][0], channel, frame_size);
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

            //Print result
            for (unsigned int source_index = 0; source_index < trackedSourcesGetMaxNumberSources(workspace.myTrackedSources); source_index++)
            {
                int source_id = trackedSourcesGetID(workspace.myTrackedSources,source_index);

                float x,y,z = 0;

                if (source_id != ID_NOSOURCE)
                {
                    x = trackedSourcesGetX(workspace.myTrackedSources,source_index);
                    y = trackedSourcesGetY(workspace.myTrackedSources,source_index);
                    z = trackedSourcesGetZ(workspace.myTrackedSources,source_index);
                    
                    printf("Source ID: %i           x: %f y: %f z: %f\n",source_id,x,y,z);
                }
            }

            return true;
        }
        else
        {
            //Problems
            cerr << "Process error" << endl;
            cerr << "frames : " << frames << endl;
            cerr << "frame_size : " << frame_size << endl;
            cerr << "nb_channels : " << nb_channels << endl;
            return false;
        }


    }

    void initWorkspace()
    {
        // Create the workspace
        workspace = createEmptyOverallContext();

        //Default parameters
        ParametersLoadDefault(workspace.myParameters);

        // Initialize the microphones
        microphonesInit(workspace.myMicrophones, NB_MICROPHONES);
        setup_microphone_positions_and_gains(workspace.myParameters, workspace.myMicrophones);

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

        cout << "Workspace created" << endl;
        workspaceReady_  = true;

    }

    bool initSoundCard()
    {
        rtaudio_ = new RtAudio();
        if ( rtaudio_->getDeviceCount() < 1 )
        {
            cerr << "No sound card available" << endl;
            delete rtaudio_;
            rtaudio_ = NULL;
            return false;
        }

        //Let's find the first device with at least 8 inputs

        // Variable to stock the approriate device
        int selected_device = -1;

        // Scan through devices for various capabilities
        RtAudio::DeviceInfo info;

        for (unsigned int indexDevice = 0; indexDevice < rtaudio_->getDeviceCount(); indexDevice++ )
        {
            info = rtaudio_->getDeviceInfo(indexDevice);
            if ( info.probed == true )
            {

                //Take the device with 8 output channels
                if( info.inputChannels >= NB_MICROPHONES)
                {
                    cout << "Using device : " << info.name << " nb Inputs: " << info.inputChannels << endl;
                    selected_device = indexDevice;
                    break;
                }
            }
        }

        RtAudio::StreamParameters iparameters;
        iparameters.deviceId = selected_device;
        iparameters.nChannels = NB_MICROPHONES;
        iparameters.firstChannel = 0;
        RtAudio::StreamOptions audioOptions;
        int sampleRate = SAMPLING_RATE;
        unsigned int bufferFrames = SAMPLES_PER_FRAME;


        try
        {
            //Open the device
            rtaudio_->openStream( NULL, &iparameters, RTAUDIO_SINT16, sampleRate, &bufferFrames, &record, this, &audioOptions);

        }
        catch ( RtError& e ) {
            e.printMessage();
            delete rtaudio_;
            rtaudio_ = NULL;
            return false;
        }


        return true;
    }

protected:

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


    RtAudio *rtaudio_;
    struct objOverall workspace;
    bool workspaceReady_ ;
};


//Record callback
int record( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
             double streamTime, RtAudioStreamStatus status, void *userData )
{
    /*
    cout << "record time : " << streamTime << endl;
    cout << "record outputBuffer " << outputBuffer << endl;
    cout << "record inputBuffer " << inputBuffer << endl;
    cout << "record nBufferFrames " << nBufferFrames << endl;
    cout << "record status " << status << endl;
    */

    AudioCapture *capture = reinterpret_cast<AudioCapture*>(userData);

    if (capture && inputBuffer)
    {
        if (nBufferFrames == SAMPLES_PER_FRAME)
        {
            capture->processFrame(streamTime, (short*)inputBuffer,nBufferFrames, NB_MICROPHONES);
        }
        else
        {
            cerr << "Buffer frame mismatch, got "<<nBufferFrames<<" asked " << SAMPLES_PER_FRAME << endl;
            return -1;
        }
    }

    return 0;

}









int main (int argc, char* argv[])
{
    AudioCapture capture;

    while(1)
    {
        //100ms
        usleep(100000);
    }


    return 0;
}
