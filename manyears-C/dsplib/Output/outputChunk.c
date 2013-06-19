/*******************************************************************************
 * ManyEars: Output - Source code                                              *
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

#include "Output/outputChunk.h"

/*******************************************************************************
 * outputInit                                                                  *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myOutput        Object to be initialized                       *
 *              myParameters    List of parameters that is used to configure   *
 *                              the object.                                    *
 *              myPathTracking  The model path for creating tracking files. It *
 *                              is set to NULL if no file needs to be created. *
 *              myPathSeparation                                               *
 *                              The model path for creating separated files.   *
 *                              It is set to NULL if no file needs to be       *
 *                              created.                                       *
 *              myPathSeparationWave                                           *
 *                              The model path for created wave files. It is   *
 *                              set to NULL if no file needs to be created.    *
 *              wildcardChar    The wildcard character                         *
 *                                                                             *
 * Outputs:     myOutput        Initialized object                             *
 *                                                                             *
 * Description: This function initializes the output object and allocates      *
 *              memory that will be used later.                                *
 *                                                                             *
 ******************************************************************************/

void outputChunkInit(struct objOutputChunk* myOutputChunk, struct ParametersStruct* myParameters, char* myPathTracking, char* myPathSeparation, char* myPathSeparationWave, char wildcardChar, char wildcardChar2)
{

    unsigned int lenPathTracking;
    unsigned int lenPathSeparation;
    unsigned int lenPathSeparationWave;

    unsigned int indexSource;

    const char* temp = "temp";

    // *************************************************************************
    // * STEP 1: Load parameters                                               *
    // *************************************************************************

    myOutputChunk->OUT_NBSOURCES = myParameters->P_GEN_DYNSOURCES;
    myOutputChunk->OUT_HOPSIZE = (unsigned int) (GLOBAL_FRAMESIZE * GLOBAL_OVERLAP);
    myOutputChunk->OUT_SAMPLERATE = GLOBAL_FS;
    myOutputChunk->OUT_GAIN = myParameters->P_OUT_GAIN;
    myOutputChunk->OUT_WAVEHEADERSIZE = myParameters->P_OUT_WAVEHEADERSIZE;

    // *************************************************************************
    // * STEP 2: Save strings                                                  *
    // *************************************************************************

    if (myPathTracking != NULL)
    {
        lenPathTracking = strlen(myPathTracking);
        myOutputChunk->pathTracking = (char*) newTable1D(lenPathTracking+1, sizeof(char));
        strcpy(myOutputChunk->pathTracking,myPathTracking);
    }
    else
    {
        myOutputChunk->pathTracking = NULL;
        lenPathTracking = 0;
    }

    if (myPathSeparation != NULL)
    {
        lenPathSeparation = strlen(myPathSeparation);
        myOutputChunk->pathSeparation = (char*) newTable1D(lenPathSeparation+1, sizeof(char));
        strcpy(myOutputChunk->pathSeparation, myPathSeparation);
        myOutputChunk->deleteSeparated = 0;
    }
    else
    {

        if (myPathSeparationWave != NULL)
        {

            //lenPathSeparation = strlen(myPathSeparationWave);
            //myOutput->pathSeparation = (char*) newTable1D(lenPathSeparation + strlen(temp) + 1, sizeof(char));
            lenPathSeparation = strlen(myPathSeparationWave) + strlen(temp);
            myOutputChunk->pathSeparation = (char*) newTable1D(lenPathSeparation + 1, sizeof(char));
            strcpy(myOutputChunk->pathSeparation, myPathSeparationWave);
            strcat(myOutputChunk->pathSeparation, temp);
            myOutputChunk->deleteSeparated = 1;

        }
        else
        {
            myOutputChunk->pathSeparation = NULL;
            myOutputChunk->deleteSeparated = 1;
            lenPathSeparation = 0;
        }

    }

    if (myPathSeparationWave != NULL)
    {
        lenPathSeparationWave = strlen(myPathSeparationWave);
        myOutputChunk->pathSeparationWave = newTable1D(lenPathSeparationWave+1, sizeof(char));
        strcpy(myOutputChunk->pathSeparationWave, myPathSeparationWave);
    }
    else
    {
        myOutputChunk->pathSeparationWave = NULL;
        lenPathSeparationWave = 0;
    }

    // *************************************************************************
    // * STEP 3: Allocate string paths                                         *
    // *************************************************************************

    myOutputChunk->listFilenameTracking = (char**) newTable2D(myOutputChunk->OUT_NBSOURCES, lenPathTracking, sizeof(char));
    myOutputChunk->listFilenameSeparation = (char**) newTable2D(myOutputChunk->OUT_NBSOURCES, lenPathSeparation, sizeof(char));
    myOutputChunk->filenameSeparation = (char*) newTable1D(lenPathSeparation, sizeof(char));
    myOutputChunk->filenameSeparationWave = (char*) newTable1D(lenPathSeparationWave, sizeof(char));

    // *************************************************************************
    // * STEP 4: Allocate file pointers                                        *
    // *************************************************************************

    myOutputChunk->listPointersTracking = (FILE**) newTable1D(myOutputChunk->OUT_NBSOURCES, sizeof(FILE*));
    myOutputChunk->listPointersSeparation = (FILE**) newTable1D(myOutputChunk->OUT_NBSOURCES, sizeof(FILE*));

    // *************************************************************************
    // * STEP 5: Allocate IDs                                                  *
    // *************************************************************************

    myOutputChunk->listID = (signed int*) newTable1D(myOutputChunk->OUT_NBSOURCES, sizeof(signed int));

    for (indexSource = 0; indexSource < myOutputChunk->OUT_NBSOURCES; indexSource++)
    {
        myOutputChunk->listID[indexSource] = OUTPUTCHUNK_NOSOURCE;
    }

    // *************************************************************************
    // * STEP 6: Allocate durations                                            *
    // *************************************************************************

    myOutputChunk->listDuration = (unsigned int*) newTable1D(myOutputChunk->OUT_NBSOURCES, sizeof(unsigned int));
    myOutputChunk->listParts = (unsigned int*) newTable1D(myOutputChunk->OUT_NBSOURCES, sizeof(unsigned int));

    // *************************************************************************
    // * STEP 7: Save wildcard character                                       *
    // *************************************************************************

    myOutputChunk->wildcardChar = wildcardChar;
    myOutputChunk->wildcardChar2 = wildcardChar2;

    // *************************************************************************
    // * STEP 8: Save duration                                                 *
    // *************************************************************************

    myOutputChunk->minDuration = myParameters->P_OUT_MINDURATION;
    myOutputChunk->intervalDuration = myParameters->P_OUT_INTERVALDURATION;

    // *************************************************************************
    // * STEP 9: Allocate temporary array                                      *
    // *************************************************************************

    myOutputChunk->tmpArray = (float*) newTable1D(myOutputChunk->OUT_HOPSIZE, sizeof(float));

    // *************************************************************************
    // * STEP 10: Allocate string for wave file header                         *
    // *************************************************************************

    myOutputChunk->waveHeader = (char*) newTable1D(myOutputChunk->OUT_WAVEHEADERSIZE, sizeof(char));

}

/*******************************************************************************
 * outputTerminate                                                             *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myOutput            The output object to be terminated         *
 *                                                                             *
 * Outputs:     (none)                                                         *
 *                                                                             *
 * Description: This function frees the dynamic memory that was allocated in   *
 *              the initialization of this object                              *
 *                                                                             *
 ******************************************************************************/

void outputChunkTerminate(struct objOutputChunk* myOutputChunk)
{

    unsigned int indexSource;

    // *************************************************************************
    // * STEP 1: Close files that correspond to remaining IDs                  *
    // *************************************************************************

    for (indexSource = 0; indexSource < myOutputChunk->OUT_NBSOURCES; indexSource++)
    {
        if (myOutputChunk->listID[indexSource] != OUTPUTCHUNK_NOSOURCE)
        {

            if (myOutputChunk->listPointersTracking[indexSource] != NULL)
            {
                fclose(myOutputChunk->listPointersTracking[indexSource]);
            }
            if (myOutputChunk->listPointersSeparation[indexSource] != NULL)
            {
                fclose(myOutputChunk->listPointersSeparation[indexSource]);
            }
            if (myOutputChunk->pathSeparationWave != NULL)
            {
                //outputGenerateWave(myOutputChunk, myOutputChunk->listID[indexSource]);

                if (myOutputChunk->deleteSeparated == 1)
                {
                    remove(myOutputChunk->listFilenameSeparation[indexSource]);
                }

            }

            myOutputChunk->listID[indexSource] = OUTPUTCHUNK_NOSOURCE;

        }
    }

}

/*******************************************************************************
 * outputProcess                                                               *
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

void outputChunkProcess(struct objOutputChunk* myOutputChunk, struct objPostprocessor* myPostprocessor)
{

    unsigned int indexSource;
    unsigned int indexSource2;
    unsigned int indexSample;

    unsigned int durationTs;

    signed short currentSample;

    char exists = 0;

    // *************************************************************************
    // * STEP 1: Close files that correspond to IDs that no longer exist       *
    // *************************************************************************

    for (indexSource = 0; indexSource < myOutputChunk->OUT_NBSOURCES; indexSource++)
    {
        if (myOutputChunk->listID[indexSource] != OUTPUTCHUNK_NOSOURCE)
        {

            exists = 0;

            for (indexSource2 = 0; indexSource2 < myPostprocessor->PP_NSOURCES; indexSource2++)
            {
                if (myOutputChunk->listID[indexSource] == myPostprocessor->sourcesID[indexSource2])
                {
                    exists = 1;
                    break;
                }
            }

            if (exists == 0)
            {

                if (myOutputChunk->listPointersTracking[indexSource] != NULL)
                {
                    fclose(myOutputChunk->listPointersTracking[indexSource]);
                }
                if (myOutputChunk->listPointersSeparation[indexSource] != NULL)
                {
                    fclose(myOutputChunk->listPointersSeparation[indexSource]);
                }
                if (myOutputChunk->pathSeparationWave != NULL)
                {
                    //outputChunkGenerateWave(myOutputChunk, myOutputChunk->listID[indexSource]);

                    if (myOutputChunk->deleteSeparated == 1)
                    {
                        remove(myOutputChunk->listFilenameSeparation[indexSource]);
                    }

                }

                myOutputChunk->listID[indexSource] = OUTPUTCHUNK_NOSOURCE;
            }
        }
    }

    // *************************************************************************
    // * STEP 2: Open files that correspond to new IDs                         *
    // *************************************************************************

    for (indexSource = 0; indexSource < myPostprocessor->PP_NSOURCES; indexSource++)
    {
        if (myPostprocessor->sourcesID[indexSource] != GSS_NOSOURCE)
        {

            exists = 0;

            for (indexSource2 = 0; indexSource2 < myOutputChunk->OUT_NBSOURCES; indexSource2++)
            {
                if (myPostprocessor->sourcesID[indexSource] == myOutputChunk->listID[indexSource2])
                {
                    exists = 1;
                    break;
                }
            }

            if (exists == 0)
            {

                for (indexSource2 = 0; indexSource2 < myOutputChunk->OUT_NBSOURCES; indexSource2++)
                {
                    if (myOutputChunk->listID[indexSource2] == OUTPUTCHUNK_NOSOURCE)
                    {

                        if (myOutputChunk->pathTracking != NULL)
                        {
                            outputChunkGeneratePathTemp(myOutputChunk, myOutputChunk->pathTracking, myOutputChunk->listFilenameTracking[indexSource2], myPostprocessor->sourcesID[indexSource]);
                            myOutputChunk->listPointersTracking[indexSource2] = fopen(myOutputChunk->listFilenameTracking[indexSource2], "wb");
                        }
                        else
                        {
                            myOutputChunk->listPointersTracking[indexSource2] = NULL;
                        }

                        if (myOutputChunk->pathSeparation != NULL)
                        {
                            outputChunkGeneratePathTemp(myOutputChunk, myOutputChunk->pathSeparation, myOutputChunk->listFilenameSeparation[indexSource2], myPostprocessor->sourcesID[indexSource]);
                            myOutputChunk->listPointersSeparation[indexSource2] = fopen(myOutputChunk->listFilenameSeparation[indexSource2], "wb");
                        }
                        else
                        {
                            myOutputChunk->listPointersSeparation[indexSource2] = NULL;
                        }

                        myOutputChunk->listID[indexSource2] = myPostprocessor->sourcesID[indexSource];
                        myOutputChunk->listDuration[indexSource2] = 0;
                        myOutputChunk->listParts[indexSource2] = 0;

                        break;

                    }
                }


            }
        }
    }

    // *************************************************************************
    // * STEP 3: Write in files that correspond to active IDs                  *
    // *************************************************************************

    for (indexSource = 0; indexSource < myOutputChunk->OUT_NBSOURCES; indexSource++)
    {
        if (myOutputChunk->listID[indexSource] != OUTPUTCHUNK_NOSOURCE)
        {
            postprocessorExtractHop(myPostprocessor, myOutputChunk->listID[indexSource], myOutputChunk->tmpArray);

            if (myOutputChunk->listPointersSeparation[indexSource] != NULL)
            {

                for (indexSample = 0; indexSample < myOutputChunk->OUT_HOPSIZE; indexSample++)
                {
                    currentSample = (signed short) floor((myOutputChunk->tmpArray[indexSample] * myOutputChunk->OUT_GAIN) * 32768.0 + 0.5);
                    fwrite(&currentSample, 1, sizeof(signed short), myOutputChunk->listPointersSeparation[indexSource]);
                }

            }

            if (myOutputChunk->listPointersTracking[indexSource] != NULL)
            {

                for (indexSource2 = 0; indexSource2 < myPostprocessor->PP_NSOURCES; indexSource2++)
                {
                    if (myPostprocessor->sourcesID[indexSource2] == myOutputChunk->listID[indexSource])
                    {
                        fwrite(&myPostprocessor->sourcesPosition[indexSource2][0], 1, sizeof(float), myOutputChunk->listPointersTracking[indexSource]);
                        fwrite(&myPostprocessor->sourcesPosition[indexSource2][1], 1, sizeof(float), myOutputChunk->listPointersTracking[indexSource]);
                        fwrite(&myPostprocessor->sourcesPosition[indexSource2][2], 1, sizeof(float), myOutputChunk->listPointersTracking[indexSource]);
                        break;
                    }
                }

            }

            myOutputChunk->listDuration[indexSource] += myOutputChunk->OUT_HOPSIZE;

            if (myOutputChunk->listParts[indexSource] == 0)
            {
                durationTs = myOutputChunk->minDuration;
            }
            else
            {
                durationTs = myOutputChunk->minDuration + myOutputChunk->listParts[indexSource] * myOutputChunk->intervalDuration;
            }

            if (myOutputChunk->listDuration[indexSource] >= durationTs)
            {

                myOutputChunk->listParts[indexSource]++;
                outputChunkGenerateWave(myOutputChunk, myOutputChunk->listID[indexSource], myOutputChunk->listParts[indexSource]);

            }

        }
    }

}

void outputChunkGeneratePathTemp(struct objOutputChunk* myOutputChunk, const char* model, char* result, ID_TYPE id)
{

    signed int indexChar;
    unsigned int lenModel;

    char modelChar;
    char newChar;
    signed int idAcc;
    signed int idRem;

    lenModel = strlen(model);
    idAcc = id;

    for (indexChar = lenModel - 1; indexChar >= 0; indexChar--)
    {
        modelChar = model[indexChar];

        // If this is a wildcar character, then remplace
        if (modelChar == myOutputChunk->wildcardChar)
        {
            // Get last digit
            idRem = idAcc - ((idAcc / 10) * 10);
            idAcc /= 10;

            // In ASCII, 0 = 48, 1 = 49, 2 = 50, ...
            newChar = ((char) (idRem) + 48);
        }
        else if (modelChar == myOutputChunk->wildcardChar2)
        {
            newChar = '0';
        }
        else
        {
            newChar = modelChar;
        }

        // Remplace
        result[indexChar] = newChar;

    }

    // Set NULL character at the end of new string
    result[lenModel] = 0x0;

}

/*******************************************************************************
 * outputGeneratePath                                                          *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myOutput            The output object to be used               *
 *              model               The string used as a model path            *
 *              id                  The id of the current source               *
 *                                                                             *
 * Outputs:     result              The string which contains the generated    *
 *                                  path                                       *
 *                                                                             *
 * Description: This generates a path related to the provided ID.              *
 *                                                                             *
 ******************************************************************************/

void outputChunkGeneratePathWave(struct objOutputChunk* myOutputChunk, const char* model, char* result, ID_TYPE id, unsigned int partNumber)
{

    signed int indexChar;
    unsigned int lenModel;

    char modelChar;
    char newChar;
    signed int idAcc;
    signed int idRem;
    signed int partAcc;
    signed int partRem;

    lenModel = strlen(model);
    idAcc = id;
    partAcc = (signed int) partNumber;

    for (indexChar = lenModel - 1; indexChar >= 0; indexChar--)
    {
        modelChar = model[indexChar];

        // If this is a wildcar character, then remplace
        if (modelChar == myOutputChunk->wildcardChar)
        {
            // Get last digit
            idRem = idAcc - ((idAcc / 10) * 10);
            idAcc /= 10;

            // In ASCII, 0 = 48, 1 = 49, 2 = 50, ...
            newChar = ((char) (idRem) + 48);
        }
        // If this is a wildcar character, then remplace
        else if (modelChar == myOutputChunk->wildcardChar2)
        {
            // Get last digit
            partRem = partAcc - ((partAcc / 10) * 10);
            partAcc /= 10;

            // In ASCII, 0 = 48, 1 = 49, 2 = 50, ...
            newChar = ((char) (partRem) + 48);
        }
        else
        {
            newChar = modelChar;
        }

        // Remplace
        result[indexChar] = newChar;

    }

    // Set NULL character at the end of new string
    result[lenModel] = 0x0;

}

/*******************************************************************************
 * outputGenerateWave                                                          *
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

void outputChunkGenerateWave(struct objOutputChunk* myOutputChunk, ID_TYPE id, unsigned int partNumber)
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

    outputChunkGeneratePathTemp(myOutputChunk, myOutputChunk->pathSeparation, myOutputChunk->filenameSeparation, id);
    ptrData = fopen(myOutputChunk->filenameSeparation, "rb");

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
    sampleRate = myOutputChunk->OUT_SAMPLERATE;

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

    outputChunkGeneratePathWave(myOutputChunk, myOutputChunk->pathSeparationWave, myOutputChunk->filenameSeparationWave, id, partNumber);
    ptrWave = fopen(myOutputChunk->filenameSeparationWave, "wb");
    ptrData = fopen(myOutputChunk->filenameSeparation, "rb");

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
