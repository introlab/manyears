/******************************************************************************* 
 * ManyEars: File Inputs/Outputs - Header                                      *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Author: François Grondin                                                    *
 * Original Code: Jean-Marc Valin                                              *
 * Modified Code: Simon Brière                                                 *
 * Version: 1.0.0                                                              *
 * Date: February 1st, 2010                                                    *
 *                                                                             *
 * Disclaimer: This software is provided "as is". Use it at your own risk.     *
 ******************************************************************************/

#ifndef IODISK_H
#define IODISK_H

// =============================================================================

#include "beamformer.h"
#include "mixture.h"

/*******************************************************************************
 * Constant                                                                    *
 ******************************************************************************/

    // Define the path of the file to be read
    #define IODISK_READPATH       "C:/Tmp/Lab/inputfile.raw"

    // Define the path of the file to be written
    #define IODISK_WRITEPATH      "C:/Tmp/Lab/outputfile.out"

    // Define the buffer size for a signal
    #define IODISK_BUFFERSIGNAL	  10000000

    // Define the buffer size for sources
    #define IODISK_BUFFERSOURCES  20000

/*******************************************************************************
 * Structures                                                                  *
 ******************************************************************************/

//        struct objSignal
//		{
//
//			// Signal can hold a maximum of samples
//			float signal[BF_MICSNUMBER][IODISK_BUFFERSIGNAL];
//
//			// Signal end
//			unsigned long length;
//
//			// Number of frames
//			unsigned int numberFrames;
//
//		};
//
//		struct objAllSources
//		{
//
//			// Object can hold a maximum of sources
//			struct objSources listSources[IODISK_BUFFERSOURCES];
//
//			// Number of sources
//			unsigned long numberFrames;
//
//		};

/*******************************************************************************
 * Prototypes                                                                  *
 ******************************************************************************/

//		void readSignal(struct objSignal *mySignal, char *filePath);
//
//		void writeAllSources(struct objAllSources *mySources, char *filePath);

// =============================================================================

#endif
