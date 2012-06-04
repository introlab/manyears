/*******************************************************************************
 * ManyEars: OutputSingleStream - Source code                                  *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Author: François Grondin                                                    *
 * Original Code: Jean-Marc Valin                                              *
 * Modified Code: Simon Brière                                                 *
 * Version: 1.1.0                                                              *
 * Date: November 16th, 2010                                                   *
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

#include "Output/outputSingleStream.h"

/*******************************************************************************
 * outputSingleStreamInit                                                      *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      mySingleStreamOutput    Object to be initialized               *
 *              myParameters            List of parameters that is used to     *
 *                                      configure the object.                  *
 *              myPathStreamWave        The model path for created wave files. *
 *                                      It is set to NULL if no file needs to  *
 *                                      be created.                            *
 *                                                                             *
 * Outputs:     mySingleStreamOutput    Initialized object                     *
 *                                                                             *
 * Description: This function initializes the output object and allocates      *
 *              memory that will be used later.                                *
 *                                                                             *
 ******************************************************************************/

void outputSingleStreamInit(struct objOutputSingleStream* myOutputSingleStream, struct ParametersStruct* myParameters, char* myPathStreamData, char* myPathStreamWave)
{

    unsigned int lenPathWave;
    unsigned int lenPathData;

    // *************************************************************************
    // * STEP 1: Load parameters                                               *
    // *************************************************************************

    myOutputSingleStream->OUT_HOPSIZE = (unsigned int) (GLOBAL_FRAMESIZE * GLOBAL_OVERLAP);
    myOutputSingleStream->OUT_SAMPLERATE = GLOBAL_FS;
    myOutputSingleStream->OUT_GAIN = myParameters->P_OUT_GAIN;
    myOutputSingleStream->OUT_WAVEHEADERSIZE = myParameters->P_OUT_WAVEHEADERSIZE;

    // *************************************************************************
    // * STEP 2: Save strings                                                  *
    // *************************************************************************

    lenPathWave = strlen(myPathStreamWave);
    myOutputSingleStream->pathStreamWave = (char*) newTable1D(lenPathWave+1, sizeof(char));
    strcpy(myOutputSingleStream->pathStreamWave,myPathStreamWave);

    lenPathData = strlen(myPathStreamData);
    myOutputSingleStream->pathStreamData = (char*) newTable1D(lenPathData+1, sizeof(char));
    strcpy(myOutputSingleStream->pathStreamData,myPathStreamData);

    // *************************************************************************
    // * STEP 3: Allocate string for wave file header                          *
    // *************************************************************************

    myOutputSingleStream->waveHeader = (char*) newTable1D(myOutputSingleStream->OUT_WAVEHEADERSIZE, sizeof(char));

    // *************************************************************************
    // * STEP 4: Allocate temporary array                                      *
    // *************************************************************************

    myOutputSingleStream->tmpArray = (float*) newTable1D(myOutputSingleStream->OUT_HOPSIZE, sizeof(float));

    // *************************************************************************
    // * STEP 5: Allocate watermark arrays                                     *
    // *************************************************************************

    myOutputSingleStream->noWatermarkArray = (signed short*) newTable1D(myOutputSingleStream->OUT_HOPSIZE, sizeof(signed short));
    myOutputSingleStream->watermarkArray = (signed short*) newTable1D(myOutputSingleStream->OUT_HOPSIZE, sizeof(signed short));
    myOutputSingleStream->watermarkElementsArray = (float*) newTable1D(3, sizeof(float));

}

/*******************************************************************************
 * outputSingleStreamTerminate                                                 *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      mySingleStreamOutput    The output object to be terminated     *
 *                                                                             *
 * Outputs:     (none)                                                         *
 *                                                                             *
 * Description: This function frees the dynamic memory that was allocated in   *
 *              the initialization of this object                              *
 *                                                                             *
 ******************************************************************************/

void outputSingleStreamTerminate(struct objOutputSingleStream* myOutputSingleStream)
{

    // *************************************************************************
    // * STEP 1: Delete strings                                                *
    // *************************************************************************

    deleteTable1D((void*) myOutputSingleStream->pathStreamWave);

    // *************************************************************************
    // * STEP 2: Free string for wave file header                              *
    // *************************************************************************

    deleteTable1D((void*) myOutputSingleStream->waveHeader);

    // *************************************************************************
    // * STEP 3: Free temporary array                                          *
    // *************************************************************************

    deleteTable1D((void*) myOutputSingleStream->tmpArray);

    // *************************************************************************
    // * STEP 4: Free watermark arrays                                         *
    // *************************************************************************

    deleteTable1D((void*) myOutputSingleStream->noWatermarkArray);
    deleteTable1D((void*) myOutputSingleStream->watermarkArray);
    deleteTable1D((void*) myOutputSingleStream->watermarkElementsArray);

}

void outputSingleStreamStart(struct objOutputSingleStream* myOutputSingleStream)
{

    // *************************************************************************
    // * STEP 1: Open the data file                                            *
    // *************************************************************************

    myOutputSingleStream->pointerStreamData = fopen(myOutputSingleStream->pathStreamData, "wb");

}

void outputSingleStreamStop(struct objOutputSingleStream* myOutputSingleStream)
{

    // *************************************************************************
    // * STEP 1: Close the data file                                           *
    // *************************************************************************

    fclose(myOutputSingleStream->pointerStreamData);

    // *************************************************************************
    // * STEP 2: Generate the wave file                                        *
    // *************************************************************************

    outputSingleStreamGenerateWave(myOutputSingleStream);

    // *************************************************************************
    // * STEP 3: Delete the data file                                          *
    // *************************************************************************

    remove(myOutputSingleStream->pathStreamData);

}

/*******************************************************************************
 * outputSingleStreamProcess                                                   *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myOutput            The output object to be used               *
 *              myPostprocessor     The postprocessor object which contains    *
 *                                  the data to be written to files.           *
 *                                                                             *
 * Outputs:     (none but files will be written)                               *
 *                                                                             *
 * Description: This function writes the data in files.                        *
 *                                                                             *
 ******************************************************************************/

void outputSingleStreamProcess(struct objOutputSingleStream* myOutputSingleStream, struct objPostprocessor* myPostprocessor)
{

    unsigned int indexSource;
    unsigned int indexSample;
    signed short currentSample;
    float x,y,z;
    char exists = 0;

    // *************************************************************************
    // * STEP 1: Write in wave file                                            *
    // *************************************************************************

    // Take the first source for now

    for (indexSource = 0; indexSource < myPostprocessor->PP_NSOURCES; indexSource++)
    {

        // If a source is found
        if (myPostprocessor->sourcesID[indexSource] != ID_NOSOURCE)
        {

            // Position
            x = myPostprocessor->sourcesPosition[indexSource][0];
            y = myPostprocessor->sourcesPosition[indexSource][1];
            z = myPostprocessor->sourcesPosition[indexSource][2];

            // Extract the frame
            postprocessorExtractHop(myPostprocessor, myPostprocessor->sourcesID[indexSource], myOutputSingleStream->tmpArray);

            // No Watermark
            for (indexSample = 0; indexSample < myOutputSingleStream->OUT_HOPSIZE; indexSample++)
            {
                currentSample = (signed short) floor((myOutputSingleStream->tmpArray[indexSample] * myOutputSingleStream->OUT_GAIN) * 32768.0 + 0.5);
                myOutputSingleStream->noWatermarkArray[indexSample] = currentSample;
            }

            // Apply watermark
            outputSingleStreamWatermarkXYZ(myOutputSingleStream, x, y, z);

            // Write watermark
            for (indexSample = 0; indexSample < myOutputSingleStream->OUT_HOPSIZE; indexSample++)
            {
                currentSample = myOutputSingleStream->watermarkArray[indexSample];
                fwrite(&currentSample, 1, sizeof(signed short), myOutputSingleStream->pointerStreamData);
            }

            // Flag it
            exists = 1;

            break;

        }
    }

    // If no source found, then fill this part of the stream with zeros
    if (exists == 0)
    {

        // Write zeros
        for (indexSample = 0; indexSample < myOutputSingleStream->OUT_HOPSIZE; indexSample++)
        {
            currentSample = 0;
            fwrite(&currentSample, 1, sizeof(signed short), myOutputSingleStream->pointerStreamData);
        }

    }


}

/*******************************************************************************
 * outputSingleStreamGenerateWave                                              *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myOutput            The output object to be used               *
 *              id                  The id of the current source               *
 *                                                                             *
 * Outputs:     (none but wave file is created)                                *
 *                                                                             *
 * Description: This creates a WAV file based on the output of the id.         *
 *                                                                             *
 ******************************************************************************/

void outputSingleStreamGenerateWave(struct objOutputSingleStream* myOutputSingleStream)
{

    FILE* ptrData;
    unsigned int indexData;
    char tmpChar;
    signed short tmpSample;

    FILE* ptrWave;

    unsigned int sizeSample;
    unsigned int chunkSize;
    unsigned int subChunk1Size;
    unsigned int subChunk2Size;
    unsigned short nChannels;
    unsigned int sampleRate;
    unsigned short audioFormat;
    unsigned int byteRate;
    unsigned short blockAlign;
    unsigned short bitsPerSample;

    // *************************************************************************
    // * STEP 1: Open data to check the length                                 *
    // *************************************************************************

    ptrData = fopen(myOutputSingleStream->pathStreamData, "rb");

    indexData = 0;
    while(!feof(ptrData))
    {
        fread(&tmpChar,sizeof(char),1,ptrData);
        indexData++;
    }

    indexData--;

    fclose(ptrData);

    // *************************************************************************
    // * STEP 1: Compute parameters                                            *
    // *************************************************************************

    // Size of each sample in bytes
    sizeSample = sizeof(tmpSample);

    // Only one channel
    nChannels = 1;

    // Sample rate
    sampleRate = myOutputSingleStream->OUT_SAMPLERATE;

    // subChunk1Size = 16 for PCM
    subChunk1Size = 16;

    // subChunk2Size = nSamples * nChannels * sizeof(signed int)
    subChunk2Size = indexData * nChannels * sizeSample;

    // chunkSize = 4 + (8 + SubChunk1Size) + (8 + SubChunk2Size)
    chunkSize = 4 + (8 + subChunk1Size) + (8 + subChunk2Size);

    // Audio format = 1 for PCM
    audioFormat = 1;

    // Byte rate = SampleRate * nChannels * sizeof(signed int)
    byteRate = sampleRate * nChannels * sizeSample;

    // Block align = nChannels * sizeof(signed int)
    blockAlign = nChannels * sizeSample;

    // Bits per sample = 8 * sizeof(signed int)
    bitsPerSample = 8 * sizeSample;

    // *************************************************************************
    // * STEP 2: Write header                                                  *
    // *************************************************************************

    ptrWave = fopen(myOutputSingleStream->pathStreamWave, "wb");
    ptrData = fopen(myOutputSingleStream->pathStreamData, "rb");

    // +-----------------------------------------------------------------------+
    // | Step A: Write the RIFF chunk descriptor                               |
    // +-----------------------------------------------------------------------+

        // +-------------------------------------------------------------------+
        // | Step i: ChunkID                                                   |
        // +-------------------------------------------------------------------+

        // Characters "RIFF"

        tmpChar = 'R';
        fwrite(&tmpChar, 1, sizeof(tmpChar), ptrWave);
        tmpChar = 'I';
        fwrite(&tmpChar, 1, sizeof(tmpChar), ptrWave);
        tmpChar = 'F';
        fwrite(&tmpChar, 1, sizeof(tmpChar), ptrWave);
        tmpChar = 'F';
        fwrite(&tmpChar, 1, sizeof(tmpChar), ptrWave);

        // +-------------------------------------------------------------------+
        // | Step ii: ChunkSize                                                |
        // +-------------------------------------------------------------------+

        // Write chunk size with least significant bytes first

        fwrite(&chunkSize, 1, sizeof(chunkSize), ptrWave);

        // +-------------------------------------------------------------------+
        // | Step iii: Format                                                  |
        // +-------------------------------------------------------------------+

        // Characters WAVE

        tmpChar = 'W';
        fwrite(&tmpChar, 1, sizeof(tmpChar), ptrWave);
        tmpChar = 'A';
        fwrite(&tmpChar, 1, sizeof(tmpChar), ptrWave);
        tmpChar = 'V';
        fwrite(&tmpChar, 1, sizeof(tmpChar), ptrWave);
        tmpChar = 'E';
        fwrite(&tmpChar, 1, sizeof(tmpChar), ptrWave);

    // +-----------------------------------------------------------------------+
    // | Step B: Write the fmt sub-chunk                                       |
    // +-----------------------------------------------------------------------+

        // +-------------------------------------------------------------------+
        // | Step i: Subchunk1ID                                               |
        // +-------------------------------------------------------------------+

        // Characters "fmt "

        tmpChar = 'f';
        fwrite(&tmpChar, 1, sizeof(tmpChar), ptrWave);
        tmpChar = 'm';
        fwrite(&tmpChar, 1, sizeof(tmpChar), ptrWave);
        tmpChar = 't';
        fwrite(&tmpChar, 1, sizeof(tmpChar), ptrWave);
        tmpChar = ' ';
        fwrite(&tmpChar, 1, sizeof(tmpChar), ptrWave);

        // +-------------------------------------------------------------------+
        // | Step ii: Subchunk1Size                                            |
        // +-------------------------------------------------------------------+

        // Write sub chunk size with least significant bytes first
        fwrite(&subChunk1Size, 1, sizeof(subChunk1Size), ptrWave);

        // +-------------------------------------------------------------------+
        // | Step iii: AudioFormat                                             |
        // +-------------------------------------------------------------------+

        // Write audioformat with least significant bytes first
        fwrite(&audioFormat, 1, sizeof(audioFormat), ptrWave);

        // +-------------------------------------------------------------------+
        // | Step iv: NumberChannels                                           |
        // +-------------------------------------------------------------------+

        // Write numberchannels with least significant bytes first
        fwrite(&nChannels, 1, sizeof(nChannels), ptrWave);

        // +-------------------------------------------------------------------+
        // | Step v: SampleRate                                                |
        // +-------------------------------------------------------------------+

        // Write sampleRate with least significant bytes first
        fwrite(&sampleRate, 1, sizeof(sampleRate), ptrWave);

        // +-------------------------------------------------------------------+
        // | Step vi: ByteRate                                                 |
        // +-------------------------------------------------------------------+

        // Write byteRate with least significant bytes first
        fwrite(&byteRate, 1, sizeof(byteRate), ptrWave);

        // +-------------------------------------------------------------------+
        // | Step vii: BlockAlign                                              |
        // +-------------------------------------------------------------------+

        // Write blockAlign with least significant bytes first
        fwrite(&blockAlign, 1, sizeof(blockAlign), ptrWave);

        // +-------------------------------------------------------------------+
        // | Step viii: BitsPerSample                                          |
        // +-------------------------------------------------------------------+

        // Write bitsPerSample with least significant bytes first
        fwrite(&bitsPerSample, 1, sizeof(bitsPerSample), ptrWave);

    // +-----------------------------------------------------------------------+
    // | Step C: Write the data sub-chunk                                      |
    // +-----------------------------------------------------------------------+

        // +-------------------------------------------------------------------+
        // | Step i: Subchunk2ID                                               |
        // +-------------------------------------------------------------------+

        // Characters "data"

        tmpChar = 'd';
        fwrite(&tmpChar, 1, sizeof(tmpChar), ptrWave);
        tmpChar = 'a';
        fwrite(&tmpChar, 1, sizeof(tmpChar), ptrWave);
        tmpChar = 't';
        fwrite(&tmpChar, 1, sizeof(tmpChar), ptrWave);
        tmpChar = 'a';
        fwrite(&tmpChar, 1, sizeof(tmpChar), ptrWave);

        // +-------------------------------------------------------------------+
        // | Step ii: Subchunk2Size                                            |
        // +-------------------------------------------------------------------+

        // Write sub chunk size with least significant bytes first
        fwrite(&subChunk2Size, 1, sizeof(subChunk2Size), ptrWave);

    // *************************************************************************
    // * STEP 3: Write data                                                    *
    // *************************************************************************

    while(!feof(ptrData))
    {
        fread(&tmpSample, 1, sizeof(tmpSample), ptrData);
        fwrite(&tmpSample, 1, sizeof(tmpSample), ptrWave);
    }

    fclose(ptrData);
    fclose(ptrWave);

}

/*******************************************************************************
 * outputSingleStreamWatermarkXYZ                                              *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myOutputSingleStream    The stream object                      *
 *              x                       x-position of the source               *
 *              y                       y-position of the source               *
 *              z                       z-position of the source               *
 *                                                                             *
 * Outputs:     myOutputSingleStream    The stream object                      *
 *                                                                             *
 * Description: This applies a watermark with the source position              *
 *                                                                             *
 ******************************************************************************/

void outputSingleStreamWatermarkXYZ(struct objOutputSingleStream* myOutputSingleStream, float x, float y, float z)
{

    myOutputSingleStream->watermarkElementsArray[0] = x;
    myOutputSingleStream->watermarkElementsArray[1] = y;
    myOutputSingleStream->watermarkElementsArray[2] = z;

    outputSingleStreamWatermark(myOutputSingleStream->noWatermarkArray, myOutputSingleStream->watermarkArray, 3, myOutputSingleStream->watermarkElementsArray, myOutputSingleStream->OUT_HOPSIZE);

}

/*******************************************************************************
 * outputSingleStreamReverseWatermarkXYZ                                       *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myOutputSingleStream    The stream object                      *
 *                                                                             *
 * Outputs:     myOutputSingleStream    The stream object                      *
 *              x                       x-position of the source               *
 *              y                       y-position of the source               *
 *              z                       z-position of the source               *
 *                                                                             *
 * Description: This extracts a watermark with the source position             *
 *                                                                             *
 ******************************************************************************/

void outputSingleStreamReverseWatermarkXYZ(struct objOutputSingleStream* myOutputSingleStream, float* x, float* y, float* z)
{

    outputSingleStreamReverseWatermark(myOutputSingleStream->noWatermarkArray, myOutputSingleStream->watermarkArray, 3, myOutputSingleStream->watermarkElementsArray, myOutputSingleStream->OUT_HOPSIZE);

    *x = myOutputSingleStream->watermarkElementsArray[0];
    *y = myOutputSingleStream->watermarkElementsArray[1];
    *z = myOutputSingleStream->watermarkElementsArray[2];

}

/*******************************************************************************
 * outputSingleStreamWatermark                                                 *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      noWatermarkArray    The input without watermark                *
 *              nElements           The number of elements to watermark        *
 *              elementArray        The array with the elements                *
 *              nSamples            The number of samples                      *
 *                                                                             *
 * Outputs:     watermarkArray      The output with watermark                  *
 *                                                                             *
 * Description: This applies a watermark                                       *
 *                                                                             *
 ******************************************************************************/

void outputSingleStreamWatermark(signed short* noWatermarkArray, signed short* watermarkArray, unsigned int nElements, float* elementArray, unsigned int nSamples)
{

    unsigned int iSample;

    unsigned int iElement;

    unsigned int nBits;
    unsigned int iBit;

    float elementFloat;
    unsigned int elementInt;
    short signed elementShort;

    nBits = 32;

    iSample = 0;

    for (iElement = 0; iElement < nElements; iElement++)
    {

        elementFloat = elementArray[iElement];
        elementInt = *((int*) &elementFloat);

        for (iBit = 0; iBit < nBits; iBit++)
        {

            elementShort = (signed short) (elementInt & 0x0001);

            elementInt >>= 1;

            watermarkArray[iSample] = (noWatermarkArray[iSample] & 0xFFFE) + elementShort;

            iSample++;

        }

    }

    while (iSample < nSamples)
    {
        watermarkArray[iSample] = noWatermarkArray[iSample];
        iSample++;
    }

}

/*******************************************************************************
 * outputSingleStreamReverseWatermark                                          *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      watermarkArray      The input with watermark                   *
 *              nElements           The number of elements to watermark        *
 *              elementArray        The array with the elements                *
 *              nSamples            The number of samples                      *
 *                                                                             *
 * Outputs:     noWatermarkArray    The output with watermark                  *
 *                                                                             *
 * Description: This extracts a watermark                                      *
 *                                                                             *
 ******************************************************************************/

void outputSingleStreamReverseWatermark(signed short* noWatermarkArray, signed short* watermarkArray, unsigned int nElements, float* elementArray, unsigned int nSamples)
{

    unsigned int iSample;

    unsigned int iElement;

    unsigned int nBits;
    unsigned int iBit;

    float elementFloat;
    unsigned int elementInt;
    short signed elementShort;

    nBits = 32;

    iSample = 0;

    for (iElement = 0; iElement < nElements; iElement++)
    {

        elementInt = 0x0000;

        for (iBit = 0; iBit < nBits; iBit++)
        {

            elementShort = (watermarkArray[iSample] & 0x0001);

            elementInt >>= 1;
            elementInt += 0x80000000 * ((unsigned int) (elementShort));

            noWatermarkArray[iSample] = (watermarkArray[iSample] & 0xFFFE);

            iSample++;

        }

        elementFloat = *((float*) &elementInt);
        elementArray[iElement] = elementFloat;

    }

    while (iSample < nSamples)
    {
        noWatermarkArray[iSample] = (watermarkArray[iSample] & 0xFFFE);
        iSample++;
    }

}
