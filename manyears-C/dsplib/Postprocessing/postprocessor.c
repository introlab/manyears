/*******************************************************************************
 * ManyEars: Postprocessor - Source code                                       *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Author: François Grondin                                                    *
 * Original Code: Jean-Marc Valin                                              *
 * Modified Code: Simon Brière                                                 *
 * Version: 1.1.0                                                              *
 * Date: September 6th, 2010                                                   *
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

#include "Postprocessing/postprocessor.h"

/*******************************************************************************
 * postprocessorInit                                                           *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myPostprocessor Object to be initialized                       *
 *              myParameters    List of parameters that is used to configure   *
 *                              the object.                                    *
 *                                                                             *
 * Outputs:     myPostprocessor Initialized object                             *
 *                                                                             *
 * Description: This function initializes the postprocessor object and         *
 *              allocates memory that will be used later.                      *
 *                                                                             *
 ******************************************************************************/

void postprocessorInit(struct objPostprocessor *myPostprocessor, struct ParametersStruct *myParameters)
{

    unsigned int indexSource;
    unsigned int indexBuffer;

    /***************************************************************************
    * Step 1: Load parameters                                                  *
    ***************************************************************************/

    myPostprocessor->PP_FRAMESIZE = GLOBAL_FRAMESIZE;
    myPostprocessor->PP_HOPSIZE = (unsigned int) (GLOBAL_FRAMESIZE * GLOBAL_OVERLAP);
    myPostprocessor->PP_BUFFERSIZE = myPostprocessor->PP_FRAMESIZE + myPostprocessor->PP_HOPSIZE;
    myPostprocessor->PP_NSOURCES = myParameters->P_GEN_DYNSOURCES;

    /***************************************************************************
    * Step 2: Initialize the FFT module                                        *
    ***************************************************************************/

    myPostprocessor->myFFT = (struct objFFT*) malloc(sizeof(struct objFFT));
    fftInit(myPostprocessor->myFFT, myParameters, myPostprocessor->PP_FRAMESIZE);

    /***************************************************************************
    * Step 3: Generate the window                                              *
    ***************************************************************************/

    myPostprocessor->window = (float*) newTable1D(myPostprocessor->PP_FRAMESIZE,sizeof(float));
    generatePowerComplementaryWindow(myPostprocessor->window,myPostprocessor->PP_FRAMESIZE);

    /***************************************************************************
    * Step 4: Allocate space for the arrays                                    *
    ***************************************************************************/

    myPostprocessor->sourcesID = (ID_TYPE*) newTable1D(myPostprocessor->PP_NSOURCES,sizeof(ID_TYPE));
    myPostprocessor->sourcesPosition = (float**) newTable2D(myPostprocessor->PP_NSOURCES, 3, sizeof(float));
    myPostprocessor->circularBuffer = (float**) newTable2D(myPostprocessor->PP_NSOURCES,myPostprocessor->PP_BUFFERSIZE,sizeof(float));
    myPostprocessor->frameTime = (float**) newTable2D(myPostprocessor->PP_NSOURCES,myPostprocessor->PP_FRAMESIZE,sizeof(float));
    myPostprocessor->frameTimeWindowed = (float**) newTable2D(myPostprocessor->PP_NSOURCES,myPostprocessor->PP_FRAMESIZE,sizeof(float));
    myPostprocessor->frameFrequencyReal = (float**) newTable2D(myPostprocessor->PP_NSOURCES,myPostprocessor->PP_FRAMESIZE,sizeof(float));
    myPostprocessor->frameFrequencyImag = (float**) newTable2D(myPostprocessor->PP_NSOURCES,myPostprocessor->PP_FRAMESIZE,sizeof(float));

    /***************************************************************************
    * Step 5: Initialize values                                                *
    ***************************************************************************/

    myPostprocessor->circularBufferIndexRead = 0;
    myPostprocessor->circularBufferIndexWrite = 0;

    for (indexSource = 0; indexSource < myPostprocessor->PP_NSOURCES; indexSource++)
    {

        // Set ID to no sources
        myPostprocessor->sourcesID[indexSource] = GSS_NOSOURCE;

        // Set all values of the circular buffer to zeros
        for (indexBuffer = 0; indexBuffer < myPostprocessor->PP_BUFFERSIZE; indexBuffer++)
        {
            myPostprocessor->circularBuffer[indexSource][indexBuffer] = 0.0f;
        }

    }

    myPostprocessor->nActiveSources = 0;

}

/*******************************************************************************
 * postprocessorTerminate                                                      *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myPostprocessor The object to be terminated                    *
 *                                                                             *
 * Outputs:     (none)                                                         *
 *                                                                             *
 * Description: This function frees the dynamic memory that was allocated in   *
 *              the initialization of this object                              *
 *                                                                             *
 ******************************************************************************/

void postprocessorTerminate(struct objPostprocessor *myPostprocessor)
{

    /***************************************************************************
    * Step 1: Terminate the FFT module                                         *
    ***************************************************************************/

    fftTerminate(myPostprocessor->myFFT);
    free(myPostprocessor->myFFT);

    /***************************************************************************
    * Step 2: Delete the window                                                *
    ***************************************************************************/

    deleteTable1D((void*) myPostprocessor->window);

    /***************************************************************************
    * Step 3: Delete arrays                                                    *
    ***************************************************************************/

    deleteTable1D((void*) myPostprocessor->sourcesID);
    deleteTable2D((void**) myPostprocessor->sourcesPosition);
    deleteTable2D((void**) myPostprocessor->circularBuffer);
    deleteTable2D((void**) myPostprocessor->frameTime);
    deleteTable2D((void**) myPostprocessor->frameTimeWindowed);
    deleteTable2D((void**) myPostprocessor->frameFrequencyReal);
    deleteTable2D((void**) myPostprocessor->frameFrequencyImag);

}

/*******************************************************************************
 * postprocessorProcessFrameSeparated                                          *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myPostprocessor     The object used to store data              *
 *              myTrackedSources    The tracked sources for the current frame  *
 *              mySeparatedSources  The separated sources for the current      *
 *                                  frame                                      *
 *                                                                             *
 * Outputs:     myPostprocessor     The updated object with new data           *
 *                                                                             *
 * Description: This function adds the new frame in the postprocessor object   *
 *              and converts it to overlapped time-domain frame ready to be    *
 *              exported.                                                      *
 *                                                                             *
 ******************************************************************************/

void postprocessorProcessFrameSeparated(struct objPostprocessor *myPostprocessor, struct objTrackedSources* myTrackedSources, struct objSeparatedSources* mySeparatedSources)
{

    // Index to loop through each source
    unsigned int indexSource;
    unsigned int indexSource2;

    ID_TYPE firstSourceID;
    unsigned int firstSourceIndex;
    ID_TYPE secondSourceID;
    unsigned int secondSourceIndex;

    ID_TYPE oldSourceID;
    ID_TYPE newSourceID;

    char IDMatches;

    // Index to loop through buffer
    unsigned int indexBuffer;

    // Index to loop through frames
    unsigned int k;

    /***************************************************************************
    * Step 1: Update list of sources and buffers                               *
    ***************************************************************************/

    // +-----------------------------------------------------------------------+
    // | Step A: Check if old sources are still here                           |
    // +-----------------------------------------------------------------------+

    for (indexSource = 0; indexSource < myPostprocessor->PP_NSOURCES; indexSource++)
    {
        oldSourceID = myPostprocessor->sourcesID[indexSource];

        // If there is a source at this spot
        if (oldSourceID != GSS_NOSOURCE)
        {

            IDMatches = 0;

            // Check if the source is in the list
            for (indexSource2 = 0; indexSource2 < idListGetNElements(&mySeparatedSources->sourcesID); indexSource2++)
            {
                newSourceID = idListGetID(&mySeparatedSources->sourcesID, indexSource2);

                if (newSourceID == oldSourceID)
                {
                    IDMatches = 1;
                    break;
                }

            }

            // If the source no longuer exists, then free the spot
            if (IDMatches == 0)
            {
                // Free spot in list
                myPostprocessor->sourcesID[indexSource] = GSS_NOSOURCE;

                // Reset the buffer
                for (indexBuffer = 0; indexBuffer < myPostprocessor->PP_BUFFERSIZE; indexBuffer++)
                {
                    myPostprocessor->circularBuffer[indexSource][indexBuffer] = 0.0f;
                }

            }

        }

    }

    // +-----------------------------------------------------------------------+
    // | Step B: Check if new sources need to be give a spot                   |
    // +-----------------------------------------------------------------------+

    for (indexSource = 0; indexSource < idListGetNElements(&mySeparatedSources->sourcesID); indexSource++)
    {
        newSourceID = idListGetID(&mySeparatedSources->sourcesID,indexSource);

        IDMatches = 0;

        for (indexSource2 = 0; indexSource2 < myPostprocessor->PP_NSOURCES; indexSource2++)
        {
            oldSourceID = myPostprocessor->sourcesID[indexSource2];

            if (oldSourceID != GSS_NOSOURCE)
            {
                if (newSourceID == oldSourceID)
                {
                    IDMatches = 1;
                    break;
                }
            }

        }

        // If the source is new, give it a spot
        if (IDMatches == 0)
        {
            for (indexSource2 = 0; indexSource2 < myPostprocessor->PP_NSOURCES; indexSource2++)
            {
                oldSourceID = myPostprocessor->sourcesID[indexSource2];

                if (oldSourceID == GSS_NOSOURCE)
                {
                    myPostprocessor->sourcesID[indexSource2] = newSourceID;
                    break;
                }

            }
        }

    }

    /***************************************************************************
    * Step 2: Copy positions of sources                                        *
    ***************************************************************************/

    for (indexSource = 0; indexSource < idListGetNElements(&myTrackedSources->sourcesID); indexSource++)
    {
        for (indexSource2 = 0; indexSource2 < myPostprocessor->PP_NSOURCES; indexSource2++)
        {
            if (idListGetID(&myTrackedSources->sourcesID,indexSource) == myPostprocessor->sourcesID[indexSource2])
            {
                myPostprocessor->sourcesPosition[indexSource2][0] = myTrackedSources->sourcesPosition[indexSource][0];
                myPostprocessor->sourcesPosition[indexSource2][1] = myTrackedSources->sourcesPosition[indexSource][1];
                myPostprocessor->sourcesPosition[indexSource2][2] = myTrackedSources->sourcesPosition[indexSource][2];
                break;
            }
        }
    }

    /***************************************************************************
    * Step 2: Copy frames                                                      *
    ***************************************************************************/

    myPostprocessor->nActiveSources = 0;

    for (indexSource = 0; indexSource < idListGetNElements(&mySeparatedSources->sourcesID); indexSource++)
    {
        newSourceID = idListGetID(&mySeparatedSources->sourcesID, indexSource);

        for (indexSource2 = 0; indexSource2 < myPostprocessor->PP_NSOURCES; indexSource2++)
        {
            oldSourceID = myPostprocessor->sourcesID[indexSource2];

            if (newSourceID == oldSourceID)
            {

                myPostprocessor->nActiveSources++;

                for (k = 0; k < myPostprocessor->PP_FRAMESIZE; k++)
                {
                    myPostprocessor->frameFrequencyReal[indexSource2][k] = mySeparatedSources->sourcesFramesReal[indexSource][k];
                    myPostprocessor->frameFrequencyImag[indexSource2][k] = mySeparatedSources->sourcesFramesImag[indexSource][k];
                }

                break;
            }

        }

    }


    /***************************************************************************
    * Step 3: Compute the IFFT                                                 *
    ***************************************************************************/

    firstSourceID = GSS_NOSOURCE;
    secondSourceID = GSS_NOSOURCE;

    for (indexSource = 0; indexSource <  myPostprocessor->PP_NSOURCES; indexSource++)
    {
        if (myPostprocessor->sourcesID[indexSource] != GSS_NOSOURCE)
        {
            if (firstSourceID == GSS_NOSOURCE)
            {
                // Save first source information

                firstSourceID = myPostprocessor->sourcesID[indexSource];
                firstSourceIndex = indexSource;
            }
            else if (secondSourceID == GSS_NOSOURCE)
            {
                // Save second source information

                secondSourceID = myPostprocessor->sourcesID[indexSource];
                secondSourceIndex = indexSource;

                // Process

                ifftComputeTwice(myPostprocessor->myFFT,
                                 myPostprocessor->frameFrequencyReal[firstSourceIndex],
                                 myPostprocessor->frameFrequencyImag[firstSourceIndex],
                                 myPostprocessor->frameFrequencyReal[secondSourceIndex],
                                 myPostprocessor->frameFrequencyImag[secondSourceIndex],
                                 myPostprocessor->frameTime[firstSourceIndex],
                                 myPostprocessor->frameTime[secondSourceIndex]);

                // Reset flags
                firstSourceID = GSS_NOSOURCE;
                secondSourceID = GSS_NOSOURCE;

            }
        }
    }

    // If one source has not been processed, then do it
    if (firstSourceID != GSS_NOSOURCE)
    {
        ifftComputeOnce(myPostprocessor->myFFT,
                        myPostprocessor->frameFrequencyReal[firstSourceIndex],
                        myPostprocessor->frameFrequencyImag[firstSourceIndex],
                        myPostprocessor->frameTime[firstSourceIndex]);
    }

    /***************************************************************************
    * Step 4: Window the frames                                                *
    ***************************************************************************/

    for (indexSource = 0; indexSource < myPostprocessor->PP_NSOURCES; indexSource++)
    {
        if (myPostprocessor->sourcesID[indexSource] != GSS_NOSOURCE)
        {
            for (k = 0; k < myPostprocessor->PP_FRAMESIZE; k++)
            {
                myPostprocessor->frameTimeWindowed[indexSource][k] = myPostprocessor->frameTime[indexSource][k] * myPostprocessor->window[k];
            }
        }
    }

    /***************************************************************************
    * Step 5: Overlap-add the frame                                            *
    ***************************************************************************/

    // +-------------------------------------------------------------------+
    // | Step A: Update circular buffer                                    |
    // +-------------------------------------------------------------------+

    for (indexSource = 0; indexSource < myPostprocessor->PP_NSOURCES; indexSource++)
    {

        if (myPostprocessor->sourcesID[indexSource] != GSS_NOSOURCE)
        {

            indexBuffer = myPostprocessor->circularBufferIndexWrite;

            // +-------------------------------------------------------------------+
            // | Step i: Add the new frame to the buffer                           |
            // +-------------------------------------------------------------------+

            for (k = 0; k < myPostprocessor->PP_FRAMESIZE; k++)
            {

                myPostprocessor->circularBuffer[indexSource][indexBuffer] += myPostprocessor->frameTimeWindowed[indexSource][k];
                indexBuffer++;

                if (indexBuffer == myPostprocessor->PP_BUFFERSIZE)
                {
                    indexBuffer = 0;
                }

            }

            // +-------------------------------------------------------------------+
            // | Step ii: Set all values of the next hop to zero                   |
            // +-------------------------------------------------------------------+

            for (k = 0; k < myPostprocessor->PP_HOPSIZE; k++)
            {

                myPostprocessor->circularBuffer[indexSource][indexBuffer] = 0.0f;

                indexBuffer++;

                if (indexBuffer == myPostprocessor->PP_BUFFERSIZE)
                {
                    indexBuffer = 0;
                }

            }

        }

    }

    // +-------------------------------------------------------------------+
    // | Step B: Update the read/write pointer                             |
    // +-------------------------------------------------------------------+

    // Increase pointer
    myPostprocessor->circularBufferIndexRead = myPostprocessor->circularBufferIndexWrite;

    // Increase pointer
    myPostprocessor->circularBufferIndexWrite += myPostprocessor->PP_HOPSIZE;

    // Wrap around if needed
    if (myPostprocessor->circularBufferIndexWrite >= myPostprocessor->PP_BUFFERSIZE)
    {
        myPostprocessor->circularBufferIndexWrite -= myPostprocessor->PP_BUFFERSIZE;
    }

}

/*******************************************************************************
 * postprocessorProcessFramePostfiltered                                       *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myPostprocessor     The object used to store data              *
 *              myTrackedSources    The tracked sources for the current frame  *
 *              myPostfilteredSources                                          *
 *                                  The postfiltered sources for the current   *
 *                                  frame                                      *
 *                                                                             *
 * Outputs:     myPostprocessor     The updated object with new data           *
 *                                                                             *
 * Description: This function adds the new frame in the postprocessor object   *
 *              and converts it to overlapped time-domain frame ready to be    *
 *              exported.                                                      *
 *                                                                             *
 ******************************************************************************/

void postprocessorProcessFramePostfiltered(struct objPostprocessor *myPostprocessor, struct objTrackedSources* myTrackedSources, struct objPostfilteredSources* myPostfilteredSources)
{

    // Index to loop through each source
    unsigned int indexSource;
    unsigned int indexSource2;

    ID_TYPE firstSourceID;
    unsigned int firstSourceIndex;
    ID_TYPE secondSourceID;
    unsigned int secondSourceIndex;

    ID_TYPE oldSourceID;
    ID_TYPE newSourceID;

    char IDMatches;

    // Index to loop through buffer
    unsigned int indexBuffer;

    // Index to loop through frames
    unsigned int k;

    /***************************************************************************
    * Step 1: Update list of sources and buffers                               *
    ***************************************************************************/

    // +-----------------------------------------------------------------------+
    // | Step A: Check if old sources are still here                           |
    // +-----------------------------------------------------------------------+

    for (indexSource = 0; indexSource < myPostprocessor->PP_NSOURCES; indexSource++)
    {
        oldSourceID = myPostprocessor->sourcesID[indexSource];

        // If there is a source at this spot
        if (oldSourceID != GSS_NOSOURCE)
        {

            IDMatches = 0;

            // Check if the source is in the list
            for (indexSource2 = 0; indexSource2 < idListGetNElements(&myPostfilteredSources->sourcesID); indexSource2++)
            {
                newSourceID = idListGetID(&myPostfilteredSources->sourcesID, indexSource2);

                if (newSourceID == oldSourceID)
                {
                    IDMatches = 1;
                    break;
                }

            }

            // If the source no longuer exists, then free the spot
            if (IDMatches == 0)
            {
                // Free spot in list
                myPostprocessor->sourcesID[indexSource] = GSS_NOSOURCE;

                // Reset the buffer
                for (indexBuffer = 0; indexBuffer < myPostprocessor->PP_BUFFERSIZE; indexBuffer++)
                {
                    myPostprocessor->circularBuffer[indexSource][indexBuffer] = 0.0;
                }

            }

        }

    }

    // +-----------------------------------------------------------------------+
    // | Step B: Check if new sources need to be given a spot                  |
    // +-----------------------------------------------------------------------+

    for (indexSource = 0; indexSource < idListGetNElements(&myPostfilteredSources->sourcesID); indexSource++)
    {
        newSourceID = idListGetID(&myPostfilteredSources->sourcesID,indexSource);

        IDMatches = 0;

        for (indexSource2 = 0; indexSource2 < myPostprocessor->PP_NSOURCES; indexSource2++)
        {
            oldSourceID = myPostprocessor->sourcesID[indexSource2];

            if (oldSourceID != GSS_NOSOURCE)
            {
                if (newSourceID == oldSourceID)
                {
                    IDMatches = 1;
                    break;
                }
            }

        }

        // If the source is new, give it a spot
        if (IDMatches == 0)
        {
            for (indexSource2 = 0; indexSource2 < myPostprocessor->PP_NSOURCES; indexSource2++)
            {
                oldSourceID = myPostprocessor->sourcesID[indexSource2];

                if (oldSourceID == GSS_NOSOURCE)
                {
                    myPostprocessor->sourcesID[indexSource2] = newSourceID;
                    break;
                }

            }
        }

    }

    /***************************************************************************
    * Step 2: Copy positions of sources                                        *
    ***************************************************************************/

    for (indexSource = 0; indexSource < idListGetNElements(&myTrackedSources->sourcesID); indexSource++)
    {
        for (indexSource2 = 0; indexSource2 < myPostprocessor->PP_NSOURCES; indexSource2++)
        {
            if (idListGetID(&myTrackedSources->sourcesID,indexSource) == myPostprocessor->sourcesID[indexSource2])
            {
                myPostprocessor->sourcesPosition[indexSource2][0] = myTrackedSources->sourcesPosition[indexSource][0];
                myPostprocessor->sourcesPosition[indexSource2][1] = myTrackedSources->sourcesPosition[indexSource][1];
                myPostprocessor->sourcesPosition[indexSource2][2] = myTrackedSources->sourcesPosition[indexSource][2];
                break;
            }
        }
    }

    /***************************************************************************
    * Step 2: Copy frames                                                      *
    ***************************************************************************/

    myPostprocessor->nActiveSources = 0;

    for (indexSource = 0; indexSource < idListGetNElements(&myPostfilteredSources->sourcesID); indexSource++)
    {
        newSourceID = idListGetID(&myPostfilteredSources->sourcesID, indexSource);

        for (indexSource2 = 0; indexSource2 < myPostprocessor->PP_NSOURCES; indexSource2++)
        {
            oldSourceID = myPostprocessor->sourcesID[indexSource2];

            if (newSourceID == oldSourceID)
            {

                myPostprocessor->nActiveSources++;

                for (k = 0; k < myPostprocessor->PP_FRAMESIZE; k++)
                {
                    myPostprocessor->frameFrequencyReal[indexSource2][k] = myPostfilteredSources->sourcesFramesReal[indexSource][k];
                    myPostprocessor->frameFrequencyImag[indexSource2][k] = myPostfilteredSources->sourcesFramesImag[indexSource][k];
                }

                break;
            }

        }

    }


    /***************************************************************************
    * Step 3: Compute the IFFT                                                 *
    ***************************************************************************/

    firstSourceID = GSS_NOSOURCE;
    secondSourceID = GSS_NOSOURCE;

    for (indexSource = 0; indexSource <  myPostprocessor->PP_NSOURCES; indexSource++)
    {
        if (myPostprocessor->sourcesID[indexSource] != GSS_NOSOURCE)
        {
            if (firstSourceID == GSS_NOSOURCE)
            {
                // Save first source information

                firstSourceID = myPostprocessor->sourcesID[indexSource];
                firstSourceIndex = indexSource;
            }
            else if (secondSourceID == GSS_NOSOURCE)
            {
                // Save second source information

                secondSourceID = myPostprocessor->sourcesID[indexSource];
                secondSourceIndex = indexSource;

                // Process

                ifftComputeTwice(myPostprocessor->myFFT,
                                 myPostprocessor->frameFrequencyReal[firstSourceIndex],
                                 myPostprocessor->frameFrequencyImag[firstSourceIndex],
                                 myPostprocessor->frameFrequencyReal[secondSourceIndex],
                                 myPostprocessor->frameFrequencyImag[secondSourceIndex],
                                 myPostprocessor->frameTime[firstSourceIndex],
                                 myPostprocessor->frameTime[secondSourceIndex]);

                // Reset flags
                firstSourceID = GSS_NOSOURCE;
                secondSourceID = GSS_NOSOURCE;

            }
        }
    }

    // If one source has not been processed, then do it
    if (firstSourceID != GSS_NOSOURCE)
    {
        ifftComputeOnce(myPostprocessor->myFFT,
                        myPostprocessor->frameFrequencyReal[firstSourceIndex],
                        myPostprocessor->frameFrequencyImag[firstSourceIndex],
                        myPostprocessor->frameTime[firstSourceIndex]);
    }

    /***************************************************************************
    * Step 4: Window the frames                                                *
    ***************************************************************************/

    for (indexSource = 0; indexSource < myPostprocessor->PP_NSOURCES; indexSource++)
    {
        if (myPostprocessor->sourcesID[indexSource] != GSS_NOSOURCE)
        {
            for (k = 0; k < myPostprocessor->PP_FRAMESIZE; k++)
            {
                myPostprocessor->frameTimeWindowed[indexSource][k] = myPostprocessor->frameTime[indexSource][k] * myPostprocessor->window[k];
            }
        }
    }

    /***************************************************************************
    * Step 5: Overlap-add the frame                                            *
    ***************************************************************************/

    // +-------------------------------------------------------------------+
    // | Step A: Update circular buffer                                    |
    // +-------------------------------------------------------------------+

    for (indexSource = 0; indexSource < myPostprocessor->PP_NSOURCES; indexSource++)
    {

        if (myPostprocessor->sourcesID[indexSource] != GSS_NOSOURCE)
        {

            indexBuffer = myPostprocessor->circularBufferIndexWrite;

            // +-------------------------------------------------------------------+
            // | Step i: Add the new frame to the buffer                           |
            // +-------------------------------------------------------------------+

            for (k = 0; k < myPostprocessor->PP_FRAMESIZE; k++)
            {

                myPostprocessor->circularBuffer[indexSource][indexBuffer] += myPostprocessor->frameTimeWindowed[indexSource][k];
                indexBuffer++;

                if (indexBuffer == myPostprocessor->PP_BUFFERSIZE)
                {
                    indexBuffer = 0;
                }

            }

            // +-------------------------------------------------------------------+
            // | Step ii: Set all values of the next hop to zero                   |
            // +-------------------------------------------------------------------+

            for (k = 0; k < myPostprocessor->PP_HOPSIZE; k++)
            {

                myPostprocessor->circularBuffer[indexSource][indexBuffer] = 0.0;

                indexBuffer++;

                if (indexBuffer == myPostprocessor->PP_BUFFERSIZE)
                {
                    indexBuffer = 0;
                }

            }

        }

    }

    // +-------------------------------------------------------------------+
    // | Step B: Update the read/write pointer                             |
    // +-------------------------------------------------------------------+

    // Increase pointer
    myPostprocessor->circularBufferIndexRead = myPostprocessor->circularBufferIndexWrite;

    // Increase pointer
    myPostprocessor->circularBufferIndexWrite += myPostprocessor->PP_HOPSIZE;

    // Wrap around if needed
    if (myPostprocessor->circularBufferIndexWrite >= myPostprocessor->PP_BUFFERSIZE)
    {
        myPostprocessor->circularBufferIndexWrite -= myPostprocessor->PP_BUFFERSIZE;
    }

}

/*******************************************************************************
 * postprocessorExtractHop                                                     *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myPostprocessor     The object used to store data              *
 *              sourcesID           Id of the source to be exported            *
 *                                                                             *
 * Outputs:     outputArray         Array that receives the new exported hop   *
 *                                                                             *
 * Description: This function exports the new hop that can be concatenated to  *
 *              a time-domain stream.                                          *
 *                                                                             *
 ******************************************************************************/

void postprocessorExtractHop(struct objPostprocessor *myPostprocessor, ID_TYPE sourcesID, float* outputArray)
{

    unsigned int indexBuffer;
    unsigned int k;
    signed int indexID;
    unsigned int indexSource;

    /***************************************************************************
    * Step 1: Get the id index                                                 *
    ***************************************************************************/

    indexID = GSS_NOSOURCE;

    for (indexSource = 0; indexSource < myPostprocessor->PP_NSOURCES; indexSource++)
    {
        if (myPostprocessor->sourcesID[indexSource] == sourcesID)
        {
            indexID = (signed int) indexSource;
            break;
        }
    }

    /***************************************************************************
    * Step 2: Get the hop which is ready                                       *
    ***************************************************************************/

    if (indexID != GSS_NOSOURCE)
    {

        indexBuffer = myPostprocessor->circularBufferIndexRead;

        for (k = 0; k < myPostprocessor->PP_HOPSIZE; k++)
        {

            outputArray[k] = myPostprocessor->circularBuffer[indexID][indexBuffer];

            indexBuffer++;

            if (indexBuffer == myPostprocessor->PP_BUFFERSIZE)
            {
                indexBuffer = 0;
            }

        }

    }



}
