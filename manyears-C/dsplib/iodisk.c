/******************************************************************************* 
 * ManyEars: File Inputs/Outputs - Source Code								   *
 * --------------------------------------------------------------------------- *
 *																			   *
 * Author: François Grondin													   *
 * Original Code: Jean-Marc Valin											   *
 * Modified Code: Simon Brière												   *
 * Version: 1.0.0															   *
 * Date: February 1st, 2010													   *
 *																			   *
 * Disclaimer: This software is provided "as is". Use it at your own risk.	   *
 ******************************************************************************/

#include <stdio.h>
#include <math.h>

#include "iodisk.h"
#include "mixture.h"
#include "beamformer.h"
#include "micSoundTrack.h"

/*******************************************************************************
 * readSignal			                                                       *
 * --------------------------------------------------------------------------- *
 *																			   *
 * Inputs:		filePath		Complete file path of the file to be opened	   *
 *																			   *
 * Outputs:		mySignal		Object in which the data are loaded			   *
 *																			   *
 * Description: This function opens a file that contains the raw data from	   *
 *				each microphone	and copies the samples in objSignal			   *
 *                                                                             *
 ******************************************************************************/

void readSignal(struct objSignal *mySignal, char *filePath)
{

	// Define the index of the samples
	unsigned long indexSample;

	// Define the index of microphone
	unsigned char indexMic;

	// Define the index of padding
	unsigned int indexPad;

	// Temporary variable for conversion
	unsigned short tmpShort;
	float tmpFloat;

	// Define a file pointer
    FILE *fp;

	// Open the file
    fp = fopen(filePath, "rb");

	// Reset the index
    indexSample = 0;

	// Loop until the end of the file or the maximum of the signal buffer
    while ((!feof(fp)) || (indexSample == IODISK_BUFFERSIGNAL))
    {

		// For each microphone
        for (indexMic = 0; indexMic < BF_MICSNUMBER; indexMic++)
        {

			// Loop the corresponding sample
			fread(&tmpShort, sizeof(short), 1, fp);

			// Convert
			tmpFloat = (float) tmpShort;

            if (tmpFloat >= 32768 )
            {
                tmpFloat -= 65536;
            }

            // Normalize
			tmpFloat /= 32768;

			// Get the current sample 
			mySignal->signal[indexMic][indexSample] = tmpFloat;

        }

		// Increment for the next sample
        indexSample++;

    }

	indexSample--;

	// If the number of samples does not match the heap size, pad with zeros
	if ((fmod(indexSample,MICST_HEAPSIZE)) != 0)
	{
		for (indexPad = 0; indexPad < (MICST_HEAPSIZE - fmod(indexSample,MICST_HEAPSIZE)); indexPad++)
		{

			// For each microphone
			for (indexMic = 0; indexMic < BF_MICSNUMBER; indexMic++)
			{
			
				mySignal->signal[indexMic][indexSample + indexPad + 1] = 0;

			}

		}

		indexSample += (MICST_HEAPSIZE - fmod(indexSample,MICST_HEAPSIZE));
	}

	// Close the file
    fclose(fp);

	// Save the length of the signal for each microphone
	mySignal->length = indexSample;

	// Save the number of frames
	mySignal->numberFrames = indexSample / MICST_HEAPSIZE;

}

/*******************************************************************************
 * writeAllSources			                                                   *
 * --------------------------------------------------------------------------- *
 *																			   *
 * Inputs:		mySources		Object in which the tracked sources are stored *
 *																			   *
 * Outputs:		filePath		File where the sources are written			   *
 *																			   *
 * Description: This function writes the tracked sources to a file			   *
 *                                                                             *
 ******************************************************************************/

void writeAllSources(struct objAllSources *mySources, char *filePath)
{

	// Define the file pointer
	FILE *fp;

	// Define the index of the source
	unsigned char indexSource;

	// Define the index of the frame
	unsigned long indexFrame;

	// Temporary variable for conversion
	float tmp;

	// Open the file
	fp = fopen(filePath, "wb");

	// Write numer of frames
	tmp = (float) mySources->numberFrames;

	fwrite(&tmp, sizeof(float), 1, fp);

	// Loop for each frame
	for (indexFrame = 0; indexFrame < mySources->numberFrames; indexFrame++)
	{

		// Loop for each source in the frame
		for (indexSource = 0; indexSource < MIXTURE_NBFILTERS; indexSource++)
		{

			// Get the source ID and convert to float
			tmp = (float) mySources->listSources[indexFrame].sourcesID[indexSource];
			fwrite(&tmp, sizeof(float), 1, fp);

			// Write the position of the source
			fwrite(&mySources->listSources[indexFrame].sourcesPosition[indexSource][0], sizeof(float), 1, fp);
			fwrite(&mySources->listSources[indexFrame].sourcesPosition[indexSource][1], sizeof(float), 1, fp);
			fwrite(&mySources->listSources[indexFrame].sourcesPosition[indexSource][2], sizeof(float), 1, fp);

		}

	}

	// Close the file
	fclose(fp);

}
