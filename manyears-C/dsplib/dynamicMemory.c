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
