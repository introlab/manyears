/*******************************************************************************
 * ManyEars: Output - Header                                                   *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Author: François Grondin                                                    *
 * Original Code: Jean-Marc Valin                                              *
 * Modified Code: Simon Brière                                                 *
 * Version: 1.1.0                                                              *
 * Date: June 19th, 2013                                                       *
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

#ifndef OUTPUTCHUNK_H
#define OUTPUTCHUNK_H

#include <stdio.h>
#include <string.h>

#include "../parameters.h"
#include "../Utilities/dynamicMemory.h"
#include "../Utilities/idList.h"
#include "../Postprocessing/postprocessor.h"


/*******************************************************************************
 * Types (Do not edit)                                                         *
 ******************************************************************************/

    // Define the value for an ID that represents no source
    #define      OUTPUTCHUNK_NOSOURCE              -1

/*******************************************************************************
 * Structure                                                                   *
 ******************************************************************************/

struct objOutputChunk
{

    // +-------------------------------------------------------------------+
    // | Parameters                                                        |
    // +-------------------------------------------------------------------+

    // Maximum number of sources
    unsigned int OUT_NBSOURCES;

    // Hop size
    unsigned int OUT_HOPSIZE;

    // Sample rate
    unsigned int OUT_SAMPLERATE;

    // Gain
    float OUT_GAIN;

    // Size of the wave header
    int OUT_WAVEHEADERSIZE;

    // +-------------------------------------------------------------------+
    // | Variables                                                         |
    // +-------------------------------------------------------------------+

    // Path of the file name for tracking
    char* pathTracking;

    // Path of the file name for separation
    char* pathSeparation;

    // Path of the file name for separation wave
    char* pathSeparationWave;

    // Wave file header
    char* waveHeader;

    // Min duration
    unsigned int minDuration;
    unsigned int intervalDuration;

    // Flag to output tracking
    char outputTracking;

    // Flag to output separation
    char outputSeparation;

    // List of file names
    char** listFilenameTracking;
    char** listFilenameSeparation;
    char* filenameSeparation;
    char* filenameSeparationWave;

    // List of IDs
    signed int* listID;

    // List of duration
    unsigned int* listDuration;
    unsigned int* listParts;

    // List of file pointers
    FILE** listPointersTracking;
    FILE** listPointersSeparation;

    // Wildcard character
    char wildcardChar;
    char wildcardChar2;

    // Temporary array
    float* tmpArray;

    // Flag
    char deleteSeparated;

};


/*******************************************************************************
 * Prototypes                                                                  *
 ******************************************************************************/

void outputChunkInit(struct objOutputChunk* myOutputChunk, struct ParametersStruct* myParameters, char* myPathTracking, char* myPathSeparation, char* myPathSeparationWave, char wildcardChar, char wildcardChar2);

void outputChunkTerminate(struct objOutputChunk* myOutputChunk);

void outputChunkProcess(struct objOutputChunk* myOutputChunk, struct objPostprocessor* myPostprocessor);

void outputChunkGeneratePathTemp(struct objOutputChunk* myOutputChunk, const char* model, char* result, ID_TYPE id);

void outputChunkGeneratePathWave(struct objOutputChunk* myOutputChunk, const char* model, char* result, ID_TYPE id, unsigned int partNumber);

void outputChunkGenerateWave(struct objOutputChunk* myOutputChunk, ID_TYPE id, unsigned int partNumber);

#endif
