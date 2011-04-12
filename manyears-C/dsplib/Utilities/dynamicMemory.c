/*******************************************************************************
 * ManyEars: Dynamic Memory Manager - Source code                              *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Author: François Grondin                                                    *
 * Original Code: Jean-Marc Valin                                              *
 * Modified Code: Simon Brière                                                 *
 * Version: 1.1.0                                                              *
 * Date: June 29th, 2010                                                       *
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

#include "dynamicMemory.h"

/*******************************************************************************
 * newTable1D                                                                  *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      len             The length of the array                        *
 *              sizeOneElement  The size of each element in bytes              *
 *                                                                             *
 * Outputs:     void*           Returns a pointer to the array                 *
 *                                                                             *
 * Description: This function allocates memory for an array of one dimension.  *
 *              The array is aligned on 16 bytes in order to allow SSE         *
 *              operations.                                                    *
 *                                                                             *
 *              To free the memory, call the function deleteTable1D and not    *
 *              the function free.                                             *
 *                                                                             *
 ******************************************************************************/

void* newTable1D(int len, int sizeOneElement)
{

    // Declare the table pointer
    char* tablePtr = NULL;

    // Declare the memory size
    int sizeTable = 0;

    // Padding
    char padding = 0;

    // Round up the size of the table such
    // that it can fit an alignment to 16 bytes
    sizeTable = sizeOneElement * len + 16;

    // Allocate memory
    tablePtr = (char *) malloc(sizeTable);

    // Compute the padding required
    padding = (char) (16 - (((size_t) tablePtr) & 0x0000000F));

    *((char*) (tablePtr + padding - 1)) = padding;

    // Return the pointer to the beginning of the table
    return ((void*) (tablePtr + padding));

}

/*******************************************************************************
 * newTable2D                                                                  *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      firstDim        Number of elements in the first dimension      *
 *              secondDim       Number of elements in the second dimension     *
 *              sizeOneElement  The size of each element in bytes              *
 *                                                                             *
 * Outputs:     void**           Returns a pointer to the array                *
 *                                                                             *
 * Description: This function allocates memory for an array of two dimensions. *
 *              The array is aligned on 16 bytes in order to allow SSE         *
 *              operations.                                                    *
 *                                                                             *
 *              To free the memory, call the function deleteTable2D and not    *
 *              the function free.                                             *
 *                                                                             *
 ******************************************************************************/

void** newTable2D(int firstDim, int secondDim, int sizeOneElement)
{

    // Index
    int index = 0;

    // Size of each row
    int sizeRow = 0;

    // Declare the table pointer
    char** tablePtr = NULL;

    // Create the list of pointers
    tablePtr = (char **) newTable1D(firstDim, sizeof(char*));

    // Size of each row in the second dimension
    sizeRow = sizeOneElement * secondDim;

    if ((sizeRow/16)*16 != sizeRow)
    {
        sizeRow += 16;
        sizeRow /= 16;
        sizeRow *= 16;
    }

    // Initialize next dimension
    tablePtr[0] = (char *) newTable1D(firstDim * sizeRow, sizeof(char));

    // Then connect each pointer to an element of the memory
    for (index = 1; index < firstDim; index++)
    {
        tablePtr[index] = tablePtr[index - 1] + sizeRow;
    }

    return ((void**) tablePtr);

}

/*******************************************************************************
 * newTable3D                                                                  *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      firstDim        Number of elements in the first dimension      *
 *              secondDim       Number of elements in the second dimension     *
 *              thirdDim        Number of elements in the third dimension      *
 *              sizeOneElement  The size of each element in bytes              *
 *                                                                             *
 * Outputs:     void***         Returns a pointer to the array                 *
 *                                                                             *
 * Description: This function allocates memory for an array of three           *
 *              dimensions. The array is aligned on 16 bytes in order to allow *
 *              SSE operations.                                                *
 *                                                                             *
 *              To free the memory, call the function deleteTable3D and not    *
 *              the function free.                                             *
 *                                                                             *
 ******************************************************************************/

void*** newTable3D(int firstDim, int secondDim, int thirdDim, int sizeOneElement)
{

    // Index
    int index1 = 0;
    int index2 = 0;
    int prevIndex1 = 0;
    int prevIndex2 = 0;

    // Size of each row
    int sizeRow = 0;

    // Declare the table pointer
    char*** tablePtr = NULL;

    // Create the list of pointers
    tablePtr = (char ***) newTable1D(firstDim, sizeof(char**));

    // Create the second list of pointers
    tablePtr[0] = (char **) newTable1D(firstDim * secondDim, sizeof(char*));

    // Then connect each pointer to a list of pointer
    for (index1 = 1; index1 < firstDim; index1++)
    {
        tablePtr[index1] = tablePtr[index1 - 1] + secondDim;
    }

    // Size of each row in the third dimension
    sizeRow = sizeOneElement * thirdDim;

    if ((sizeRow/16)*16 != sizeRow)
    {
        sizeRow += 16;
        sizeRow /= 16;
        sizeRow *= 16;
    }

    // Create the list of values
    tablePtr[0][0] = (char *) newTable1D(firstDim * secondDim * sizeRow, sizeof(char));

    prevIndex1 = 0;
    prevIndex2 = 0;

    // Then connect each pointer to an element of the memory
    for (index1 = 0; index1 < firstDim; index1++)
    {
        for (index2 = 0; index2 < secondDim; index2++)
        {

            if (!((index1 == 0) && (index2 == 0)))
            {

                if (index2 == 0)
                {
                    prevIndex1 = index1 - 1;
                    prevIndex2 = secondDim - 1;
                }
                else
                {
                    prevIndex1 = index1;
                    prevIndex2 = index2 - 1;
                }

                tablePtr[index1][index2] = tablePtr[prevIndex1][prevIndex2] + sizeRow;

            }

        }       

    }

    return ((void ***) tablePtr);

}

/*******************************************************************************
 * deleteTable1D                                                               *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      tablePtr        Pointer to the 1-D array that was previously   *
 *                              created                                        *
 *                                                                             *
 * Outputs:     (none)                                                         *
 *                                                                             *
 * Description: This function frees memory for the allocated 1-D array.        *
 *                                                                             *
 ******************************************************************************/

void deleteTable1D(void* tablePtr)
{

    // Padding
    char padding;

    // Beginning of the allocated memory
    char* allocatedMemory;

    // Get the padding
    padding = *(((char*) tablePtr) - 1);

    // Get the pointer
    allocatedMemory = (void*) (((char*) tablePtr) - padding);

    // Free
    free(allocatedMemory);

}

/*******************************************************************************
 * deleteTable2D                                                               *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      tablePtr        Pointer to the 2-D array that was previously   *
 *                              created                                        *
 *                                                                             *
 * Outputs:     (none)                                                         *
 *                                                                             *
 * Description: This function frees memory for the allocated 2-D array.        *
 *                                                                             *
 ******************************************************************************/

void deleteTable2D(void** tablePtr)
{

    // Free memory
    deleteTable1D(tablePtr[0]);
    deleteTable1D(tablePtr);

}

/*******************************************************************************
 * deleteTable3D                                                               *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      tablePtr        Pointer to the 3-D array that was previously   *
 *                              created                                        *
 *                                                                             *
 * Outputs:     (none)                                                         *
 *                                                                             *
 * Description: This function frees memory for the allocated 3-D array.        *
 *                                                                             *
 ******************************************************************************/

void deleteTable3D(void*** tablePtr)
{

    // Free memory
    deleteTable1D(tablePtr[0][0]);
    deleteTable1D(tablePtr[0]);
    deleteTable1D(tablePtr);

}
