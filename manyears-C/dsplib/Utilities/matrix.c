/*******************************************************************************
 * ManyEars: Matrix - Source code                                              *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Author: François Grondin                                                    *
 * Original Code: Jean-Marc Valin                                              *
 * Modified Code: Simon Brière                                                 *
 * Version: 1.1.0                                                              *
 * Date: August 31st, 2010                                                     *
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

#include "Utilities/matrix.h"

/*******************************************************************************
 * matrixCreate     		                                               *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      nRows           Number of rows                                 *
 *              nCols           Number of columns                              *
 *              nFrames         Number of frames                               *
 *                                                                             *
 * Outputs:     objMatrix       The new matrix object                          *
 *                                                                             *
 * Description: This function creates a 2D matrix for each of the nFrames. All *
 *              values are set to zero. Memory is dynamically allocated.       *
 *                                                                             *
 ******************************************************************************/

struct objMatrix* matrixCreate(unsigned int nRows, unsigned int nCols, unsigned int nFrames)
{

    struct objMatrix* newMatrix;

    // Create the object
    newMatrix = (struct objMatrix*) malloc(sizeof(struct objMatrix));

    // Initialize its fields
    matrixInit(newMatrix,nRows,nCols,nFrames);

    return newMatrix;

}

/*******************************************************************************
 * matrixDelete     		                                               *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      matrix          The matrix object                              *
 *                                                                             *
 * Outputs:     (none)                                                         *
 *                                                                             *
 * Description: This function deletes the matrix and frees the memory.         *
 *                                                                             *
 ******************************************************************************/

void matrixDelete(struct objMatrix* matrix)
{

    // Terminate its fields
    matrixTerminate(matrix);

    // Delete the object
    free((void*) matrix);

}

/*******************************************************************************
 * matrixInit     		                                               *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      nRows           Number of rows                                 *
 *              nCols           Number of columns                              *
 *              nFrames         Number of frames                               *
 *                                                                             *
 * Outputs:     matrix          The matrix object                              *
 *                                                                             *
 * Description: Memory is dynamically allocated.                               *
 *                                                                             *
 ******************************************************************************/

void matrixInit(struct objMatrix* matrix, unsigned int nRows, unsigned int nCols, unsigned int nFrames)
{

    unsigned int indexRow;
    unsigned int indexCol;
    unsigned int k;

#ifdef USE_SIMD

    // SIMD registers
    __m128_mod regA;

#endif

    // Set dimensions
    matrix->nRows = nRows;
    matrix->nCols = nCols;
    matrix->nFrames = nFrames;

    if ((matrix->nRows > 0) && (matrix->nCols > 0) && (matrix->nFrames > 0))
    {

        // Then create a dynamic array that will contain each element
        matrix->valueReal = (float***) newTable3D(nRows,nCols,nFrames,sizeof(float));
        matrix->valueImag = (float***) newTable3D(nRows,nCols,nFrames,sizeof(float));

        // Fill the matrix with zeros everywhere
        for (indexRow = 0; indexRow < matrix->nRows; indexRow++)
        {

            for (indexCol = 0; indexCol < matrix->nCols; indexCol++)
            {

#ifndef USE_SIMD

                for (k = 0; k < matrix->nFrames; k++)
                {

                    matrix->valueReal[indexRow][indexCol][k] = 0.0;
                    matrix->valueImag[indexRow][indexCol][k] = 0.0;

                }

#else

                // Load the constant zero
                regA.m128_f32[0] = 0.0;
                regA.m128_f32[1] = 0.0;
                regA.m128_f32[2] = 0.0;
                regA.m128_f32[3] = 0.0;

                for (k = 0; k < matrix->nFrames; k+=4)
                {
                    // Copy the constant in memory
                     _mm_store_ps(&matrix->valueReal[indexRow][indexCol][k],regA.m128);
                     _mm_store_ps(&matrix->valueImag[indexRow][indexCol][k],regA.m128);

                }

                for (k = k - 4; k < matrix->nFrames; k++)
                {
                    matrix->valueReal[indexRow][indexCol][k] = 0.0;
                    matrix->valueImag[indexRow][indexCol][k] = 0.0;
                }

#endif

            }

        }

    }

}

/*******************************************************************************
 * matrixTerminate     		                                               *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      matrix          The matrix object                              *
 *                                                                             *
 * Outputs:     (none)                                                         *
 *                                                                             *
 * Description: This function frees the memory.                                *
 *                                                                             *
 ******************************************************************************/

void matrixTerminate(struct objMatrix* matrix)
{

    // Delete old arrays if no dimension was null
    if ((matrix->nRows > 0) && (matrix->nCols > 0) && (matrix->nFrames > 0))
    {
        deleteTable3D((void***) matrix->valueReal);
        deleteTable3D((void***) matrix->valueImag);
    }

}

/*******************************************************************************
 * matrixCopy     		                                               *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      matrixSource    The matrix object that is copied               *
 *                                                                             *
 * Outputs:     matrixDest      The matrix object that receives the copy       *
 *                                                                             *
 * Description: This function copies the dimensions and the content of a       *
 *              matrix inside another matrix                                   *
 *                                                                             *
 ******************************************************************************/

void matrixCopy(struct objMatrix* matrixSource, struct objMatrix* matrixDest)
{

    unsigned int indexRow;
    unsigned int indexCol;
    unsigned int k;

#ifdef USE_SIMD

    // SIMD registers
    __m128_mod regA, regB;

#endif

    if ((matrixSource->nCols == matrixDest->nCols) &&
        (matrixSource->nRows == matrixDest->nRows) &&
        (matrixSource->nFrames == matrixDest->nFrames))
        {

        // Copy the content
        for (indexRow = 0; indexRow < matrixSource->nRows; indexRow++)
        {
            for (indexCol = 0; indexCol < matrixSource->nCols; indexCol++)
            {

    #ifndef USE_SIMD

                for (k = 0; k < matrixSource->nFrames; k++)
                {
                    matrixSetReal(matrixDest,indexRow,indexCol,k,matrixGetReal(matrixSource,indexRow,indexCol,k));
                    matrixSetImag(matrixDest,indexRow,indexCol,k,matrixGetImag(matrixSource,indexRow,indexCol,k));
                }

    #else

                for (k = 0; k < matrixSource->nFrames; k+=4)
                {

                    // Load values
                    regA.m128 = _mm_load_ps(&matrixSource->valueReal[indexRow][indexCol][k]);
                    regB.m128 = _mm_load_ps(&matrixSource->valueImag[indexRow][indexCol][k]);

                    // Store values
                     _mm_store_ps(&matrixDest->valueReal[indexRow][indexCol][k],regA.m128);
                     _mm_store_ps(&matrixDest->valueImag[indexRow][indexCol][k],regB.m128);

                }
                for (k = k - 4; k < matrixSource->nFrames; k++)
                {
                    matrixSetReal(matrixDest,indexRow,indexCol,k,matrixGetReal(matrixSource,indexRow,indexCol,k));
                    matrixSetImag(matrixDest,indexRow,indexCol,k,matrixGetImag(matrixSource,indexRow,indexCol,k));
                }
    #endif

            }
        }
    }
}

/*******************************************************************************
 * matrixGetReal     		                                               *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      matrix          The matrix object                              *
 *              indexRow        The index of the row                           *
 *              indexCol        The index of the column                        *
 *              k               The index of the frame                         *
 *                                                                             *
 * Outputs:     (returned value)                                               *
 *                                                                             *
 * Description: This function returns the real value of the kth 2D matrix at   *
 *              a given row and column                                         *
 *                                                                             *
 ******************************************************************************/

float matrixGetReal(struct objMatrix* matrix, unsigned int indexRow, unsigned int indexCol, unsigned int k)
{
    return matrix->valueReal[indexRow][indexCol][k];
}

/*******************************************************************************
 * matrixGetImag     		                                               *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      matrix          The matrix object                              *
 *              indexRow        The index of the row                           *
 *              indexCol        The index of the column                        *
 *              k               The index of the frame                         *
 *                                                                             *
 * Outputs:     (returned value)                                               *
 *                                                                             *
 * Description: This function returns the imaginary value of the kth 2D matrix *
 *              at a given row and column                                      *
 *                                                                             *
 ******************************************************************************/

float matrixGetImag(struct objMatrix* matrix, unsigned int indexRow, unsigned int indexCol, unsigned int k)
{
    return matrix->valueImag[indexRow][indexCol][k];
}

/*******************************************************************************
 * matrixSetReal     		                                               *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      matrix          The matrix object                              *
 *              indexRow        The index of the row                           *
 *              indexCol        The index of the column                        *
 *              k               The index of the frame                         *
 *              value           The value to load                              *
 *                                                                             *
 * Outputs:     (none)                                                         *
 *                                                                             *
 * Description: This function sets the real value of the kth 2D matrix at      *
 *              a given row and column to the given value                      *
 *                                                                             *
 ******************************************************************************/

void matrixSetReal(struct objMatrix* matrix, unsigned int indexRow, unsigned int indexCol, unsigned int k, float value)
{
    matrix->valueReal[indexRow][indexCol][k] = value;
}

/*******************************************************************************
 * matrixSetImag     		                                               *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      matrix          The matrix object                              *
 *              indexRow        The index of the row                           *
 *              indexCol        The index of the column                        *
 *              k               The index of the frame                         *
 *              value           The value to load                              *
 *                                                                             *
 * Outputs:     (none)                                                         *
 *                                                                             *
 * Description: This function sets the imaginary value of the kth 2D matrix at *
 *              a given row and column to the given value                      *
 *                                                                             *
 ******************************************************************************/

void matrixSetImag(struct objMatrix* matrix, unsigned int indexRow, unsigned int indexCol, unsigned int k, float value)
{
    matrix->valueImag[indexRow][indexCol][k] = value;
}

/*******************************************************************************
 * matrixInsertRow     		                                               *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      matrix          The matrix object                              *
 *              newRowIndex     The index of the row                           *
 *                                                                             *
 * Outputs:     matrix          The matrix object                              *
 *                                                                             *
 * Description: This function inserts a new row at the specified index.        *
 *                                                                             *
 ******************************************************************************/

void matrixInsertRow(struct objMatrix* matrix, unsigned int newRowIndex)
{

    struct objMatrix* oldMatrix;

    unsigned int indexRow;
    unsigned int indexCol;
    unsigned int k;

#ifdef USE_SIMD

    // SIMD registers
    __m128_mod regA, regB;

#endif

    // Copy old matrix
    oldMatrix = matrixCreate(matrix->nRows, matrix->nCols, matrix->nFrames);
    matrixCopy(matrix,oldMatrix);

    // Resize the matrix
    matrixResize(matrix,matrix->nRows+1,matrix->nCols,matrix->nFrames);

    // Copy the old content
    for (indexRow = 0; indexRow < newRowIndex; indexRow++)
    {
        for (indexCol = 0; indexCol < oldMatrix->nCols; indexCol++)
        {

#ifndef USE_SIMD

            for (k = 0; k < oldMatrix->nFrames; k++)
            {
                matrixSetReal(matrix, indexRow, indexCol, k, matrixGetReal(oldMatrix, indexRow, indexCol, k));
                matrixSetImag(matrix, indexRow, indexCol, k, matrixGetImag(oldMatrix, indexRow, indexCol, k));
            }

#else

            for (k = 0; k < oldMatrix->nFrames; k+=4)
            {

                // Load values
                regA.m128 = _mm_load_ps(&oldMatrix->valueReal[indexRow][indexCol][k]);
                regB.m128 = _mm_load_ps(&oldMatrix->valueImag[indexRow][indexCol][k]);

                // Store values
                 _mm_store_ps(&matrix->valueReal[indexRow][indexCol][k],regA.m128);
                 _mm_store_ps(&matrix->valueImag[indexRow][indexCol][k],regB.m128);

            }
            for (k = k - 4; k < oldMatrix->nFrames; k++)
            {
                matrixSetReal(matrix, indexRow, indexCol, k, matrixGetReal(oldMatrix, indexRow, indexCol, k));
                matrixSetImag(matrix, indexRow, indexCol, k, matrixGetImag(oldMatrix, indexRow, indexCol, k));
            }
#endif

        }
    }
    for (indexRow = newRowIndex; indexRow < oldMatrix->nRows; indexRow++)
    {
        for (indexCol = 0; indexCol < oldMatrix->nCols; indexCol++)
        {

#ifndef USE_SIMD

            for (k = 0; k < oldMatrix->nFrames; k++)
            {
                matrixSetReal(matrix, indexRow+1, indexCol, k, matrixGetReal(oldMatrix, indexRow, indexCol, k));
                matrixSetImag(matrix, indexRow+1, indexCol, k, matrixGetImag(oldMatrix, indexRow, indexCol, k));
            }

#else

            for (k = 0; k < oldMatrix->nFrames; k+=4)
            {

                // Load values
                regA.m128 = _mm_load_ps(&oldMatrix->valueReal[indexRow][indexCol][k]);
                regB.m128 = _mm_load_ps(&oldMatrix->valueImag[indexRow][indexCol][k]);

                // Store values
                 _mm_store_ps(&matrix->valueReal[indexRow+1][indexCol][k],regA.m128);
                 _mm_store_ps(&matrix->valueImag[indexRow+1][indexCol][k],regB.m128);

            }
            for (k = k - 4; k < oldMatrix->nFrames; k++)
            {
                matrixSetReal(matrix, indexRow+1, indexCol, k, matrixGetReal(oldMatrix, indexRow, indexCol, k));
                matrixSetImag(matrix, indexRow+1, indexCol, k, matrixGetImag(oldMatrix, indexRow, indexCol, k));
            }
#endif

        }
    }

    matrixDelete(oldMatrix);

}

/*******************************************************************************
 * matrixInsertCol     		                                               *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      matrix          The matrix object                              *
 *              newColIndex     The index of the col                           *
 *                                                                             *
 * Outputs:     matrix          The matrix object                              *
 *                                                                             *
 * Description: This function inserts a new column at the specified index.     *
 *                                                                             *
 ******************************************************************************/

void matrixInsertCol(struct objMatrix* matrix, unsigned int newColIndex)
{

    struct objMatrix* oldMatrix;

    unsigned int indexRow;
    unsigned int indexCol;
    unsigned int k;

#ifdef USE_SIMD

    // SIMD registers
    __m128_mod regA, regB;

#endif

    // Copy old matrix
    oldMatrix = matrixCreate(0, 0, 0);
    matrixCopy(matrix,oldMatrix);

    // Resize the matrix
    matrixResize(matrix,matrix->nRows,matrix->nCols+1,matrix->nFrames);

    // Copy the old content
    for (indexCol = 0; indexCol < newColIndex; indexCol++)
    {
        for (indexRow = 0; indexRow < oldMatrix->nRows; indexRow++)
        {

#ifndef USE_SIMD

            for (k = 0; k < oldMatrix->nFrames; k++)
            {
                matrixSetReal(matrix, indexRow, indexCol, k, matrixGetReal(oldMatrix, indexRow, indexCol, k));
                matrixSetImag(matrix, indexRow, indexCol, k, matrixGetImag(oldMatrix, indexRow, indexCol, k));
            }

#else

            for (k = 0; k < oldMatrix->nFrames; k+=4)
            {

                // Load values
                regA.m128 = _mm_load_ps(&oldMatrix->valueReal[indexRow][indexCol][k]);
                regB.m128 = _mm_load_ps(&oldMatrix->valueImag[indexRow][indexCol][k]);

                // Store values
                 _mm_store_ps(&matrix->valueReal[indexRow][indexCol][k],regA.m128);
                 _mm_store_ps(&matrix->valueImag[indexRow][indexCol][k],regB.m128);

            }
            for (k = k - 4; k < oldMatrix->nFrames; k++)
            {
                matrixSetReal(matrix, indexRow, indexCol, k, matrixGetReal(oldMatrix, indexRow, indexCol, k));
                matrixSetImag(matrix, indexRow, indexCol, k, matrixGetImag(oldMatrix, indexRow, indexCol, k));
            }

#endif

        }
    }
    for (indexCol = newColIndex; indexCol < oldMatrix->nCols; indexCol++)
    {
        for (indexRow = 0; indexRow < oldMatrix->nRows; indexRow++)
        {

#ifndef  USE_SIMD

            for (k = 0; k < oldMatrix->nFrames; k++)
            {
                matrixSetReal(matrix, indexRow, indexCol+1, k, matrixGetReal(oldMatrix, indexRow, indexCol, k));
                matrixSetImag(matrix, indexRow, indexCol+1, k, matrixGetImag(oldMatrix, indexRow, indexCol, k));
            }

#else

            for (k = 0; k < oldMatrix->nFrames; k+=4)
            {

                // Load values
                regA.m128 = _mm_load_ps(&oldMatrix->valueReal[indexRow][indexCol][k]);
                regB.m128 = _mm_load_ps(&oldMatrix->valueImag[indexRow][indexCol][k]);

                // Store values
                 _mm_store_ps(&matrix->valueReal[indexRow][indexCol+1][k],regA.m128);
                 _mm_store_ps(&matrix->valueImag[indexRow][indexCol+1][k],regB.m128);

            }
            for (k = k - 4; k < oldMatrix->nFrames; k++)
            {
                matrixSetReal(matrix, indexRow, indexCol+1, k, matrixGetReal(oldMatrix, indexRow, indexCol, k));
                matrixSetImag(matrix, indexRow, indexCol+1, k, matrixGetImag(oldMatrix, indexRow, indexCol, k));
            }

#endif

        }
    }

    matrixDelete(oldMatrix);

}

/*******************************************************************************
 * matrixDeleteRow     		                                               *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      matrix          The matrix object                              *
 *              deleteRowIndex  The index of the row                           *
 *                                                                             *
 * Outputs:     matrix          The matrix object                              *
 *                                                                             *
 * Description: This function deletes the row at the specified index.          *
 *                                                                             *
 ******************************************************************************/

void matrixDeleteRow(struct objMatrix* matrix, unsigned int deleteRowIndex)
{

    struct objMatrix* oldMatrix;

    unsigned int indexRow;
    unsigned int indexCol;
    unsigned int k;

#ifdef USE_SIMD

    // SIMD registers
    __m128_mod regA, regB;

#endif

    if (matrix->nRows > 0)
    {

        // Copy old matrix
        oldMatrix = matrixCreate(matrix->nRows, matrix->nCols, matrix->nFrames);
        matrixCopy(matrix,oldMatrix);

        // Resize the matrix
        matrixResize(matrix,matrix->nRows-1,matrix->nCols,matrix->nFrames);

        // Copy the old content
        for (indexRow = 0; indexRow < deleteRowIndex; indexRow++)
        {
            for (indexCol = 0; indexCol < oldMatrix->nCols; indexCol++)
            {

#ifndef USE_SIMD

                for (k = 0; k < oldMatrix->nFrames; k++)
                {
                    matrixSetReal(matrix, indexRow, indexCol, k, matrixGetReal(oldMatrix, indexRow, indexCol, k));
                    matrixSetImag(matrix, indexRow, indexCol, k, matrixGetImag(oldMatrix, indexRow, indexCol, k));
                }

#else

                for (k = 0; k < oldMatrix->nFrames; k+=4)
                {

                    // Load values
                    regA.m128 = _mm_load_ps(&oldMatrix->valueReal[indexRow][indexCol][k]);
                    regB.m128 = _mm_load_ps(&oldMatrix->valueImag[indexRow][indexCol][k]);

                    // Store values
                     _mm_store_ps(&matrix->valueReal[indexRow][indexCol][k],regA.m128);
                     _mm_store_ps(&matrix->valueImag[indexRow][indexCol][k],regB.m128);

                }
                for (k = k - 4; k < oldMatrix->nFrames; k++)
                {
                    matrixSetReal(matrix, indexRow, indexCol, k, matrixGetReal(oldMatrix, indexRow, indexCol, k));
                    matrixSetImag(matrix, indexRow, indexCol, k, matrixGetImag(oldMatrix, indexRow, indexCol, k));
                }

#endif

            }
        }
        for (indexRow = (deleteRowIndex+1); indexRow < oldMatrix->nRows; indexRow++)
        {
            for (indexCol = 0; indexCol < oldMatrix->nCols; indexCol++)
            {

#ifndef USE_SIMD

                for (k = 0; k < oldMatrix->nFrames; k++)
                {
                    matrixSetReal(matrix, indexRow-1, indexCol, k, matrixGetReal(oldMatrix, indexRow, indexCol, k));
                    matrixSetImag(matrix, indexRow-1, indexCol, k, matrixGetImag(oldMatrix, indexRow, indexCol, k));
                }

#else

                for (k = 0; k < oldMatrix->nFrames; k+=4)
                {

                    // Load values
                    regA.m128 = _mm_load_ps(&oldMatrix->valueReal[indexRow][indexCol][k]);
                    regB.m128 = _mm_load_ps(&oldMatrix->valueImag[indexRow][indexCol][k]);

                    // Store values
                     _mm_store_ps(&matrix->valueReal[indexRow-1][indexCol][k],regA.m128);
                     _mm_store_ps(&matrix->valueImag[indexRow-1][indexCol][k],regB.m128);

                }
                for (k = k - 4; k < oldMatrix->nFrames; k++)
                {
                    matrixSetReal(matrix, indexRow-1, indexCol, k, matrixGetReal(oldMatrix, indexRow, indexCol, k));
                    matrixSetImag(matrix, indexRow-1, indexCol, k, matrixGetImag(oldMatrix, indexRow, indexCol, k));
                }

#endif

            }
        }

    }

    matrixDelete(oldMatrix);

}

/*******************************************************************************
 * matrixDeleteCol     		                                               *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      matrix          The matrix object                              *
 *              deleteColIndex  The index of the col                           *
 *                                                                             *
 * Outputs:     matrix          The matrix object                              *
 *                                                                             *
 * Description: This function deletes the column at the specified index.       *
 *                                                                             *
 ******************************************************************************/

void matrixDeleteCol(struct objMatrix* matrix, unsigned int deleteColIndex)
{

    struct objMatrix* oldMatrix;

    unsigned int indexRow;
    unsigned int indexCol;
    unsigned int k;

#ifdef USE_SIMD

    // SIMD registers
    __m128_mod regA, regB;

#endif

    if (matrix->nCols > 0)
    {

        // Copy old matrix
        oldMatrix = matrixCreate(matrix->nRows, matrix->nCols, matrix->nFrames);
        matrixCopy(matrix,oldMatrix);

        // Resize the matrix
        matrixResize(matrix,matrix->nRows,matrix->nCols-1,matrix->nFrames);

        // Copy the old content
        for (indexCol = 0; indexCol < deleteColIndex; indexCol++)
        {
            for (indexRow = 0; indexRow < oldMatrix->nRows; indexRow++)
            {

#ifndef USE_SIMD

                for (k = 0; k < oldMatrix->nFrames; k++)
                {
                    matrixSetReal(matrix, indexRow, indexCol, k, matrixGetReal(oldMatrix, indexRow, indexCol, k));
                    matrixSetImag(matrix, indexRow, indexCol, k, matrixGetImag(oldMatrix, indexRow, indexCol, k));
                }

#else

                for (k = 0; k < oldMatrix->nFrames; k+=4)
                {

                    // Load values
                    regA.m128 = _mm_load_ps(&oldMatrix->valueReal[indexRow][indexCol][k]);
                    regB.m128 = _mm_load_ps(&oldMatrix->valueImag[indexRow][indexCol][k]);

                    // Store values
                     _mm_store_ps(&matrix->valueReal[indexRow][indexCol][k],regA.m128);
                     _mm_store_ps(&matrix->valueImag[indexRow][indexCol][k],regB.m128);

                }
                for (k = k - 4; k < oldMatrix->nFrames; k++)
                {
                    matrixSetReal(matrix, indexRow, indexCol, k, matrixGetReal(oldMatrix, indexRow, indexCol, k));
                    matrixSetImag(matrix, indexRow, indexCol, k, matrixGetImag(oldMatrix, indexRow, indexCol, k));
                }

#endif

            }
        }
        for (indexCol = (deleteColIndex+1); indexCol < oldMatrix->nCols; indexCol++)
        {
            for (indexRow = 0; indexRow < oldMatrix->nRows; indexRow++)
            {

#ifndef USE_SIMD

                for (k = 0; k < oldMatrix->nFrames; k++)
                {
                    matrixSetReal(matrix, indexRow, indexCol-1, k, matrixGetReal(oldMatrix, indexRow, indexCol, k));
                    matrixSetImag(matrix, indexRow, indexCol-1, k, matrixGetImag(oldMatrix, indexRow, indexCol, k));
                }

#else

                for (k = 0; k < oldMatrix->nFrames; k+=4)
                {

                    // Load values
                    regA.m128 = _mm_load_ps(&oldMatrix->valueReal[indexRow][indexCol][k]);
                    regB.m128 = _mm_load_ps(&oldMatrix->valueImag[indexRow][indexCol][k]);

                    // Store values
                     _mm_store_ps(&matrix->valueReal[indexRow][indexCol-1][k],regA.m128);
                     _mm_store_ps(&matrix->valueImag[indexRow][indexCol-1][k],regB.m128);

                }
                for (k = k - 4; k < oldMatrix->nFrames; k++)
                {
                    matrixSetReal(matrix, indexRow, indexCol-1, k, matrixGetReal(oldMatrix, indexRow, indexCol, k));
                    matrixSetImag(matrix, indexRow, indexCol-1, k, matrixGetImag(oldMatrix, indexRow, indexCol, k));
                }

#endif
            }
        }

    }

    matrixDelete(oldMatrix);
}

/*******************************************************************************
 * matrixResize     		                                               *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      matrix          The matrix object                              *
 *              nRows           Number of rows                                 *
 *              nCols           Number of columns                              *
 *              nFrames         Number of frames                               *
 *                                                                             *
 * Outputs:     matrix          The matrix object                              *
 *                                                                             *
 * Description: This function resizes the matrix according to the new          *
 *              dimensions provided. All elements are set to zero.             *
 *                                                                             *
 ******************************************************************************/

void matrixResize(struct objMatrix* matrix, unsigned int nRows, unsigned int nCols, unsigned int nFrames)
{

    // Terminate
    matrixTerminate(matrix);

    // And initialize again with new parameters
    matrixInit(matrix,nRows,nCols,nFrames);

}

/*******************************************************************************
 * matrixMultMatrix                                                            *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      matrixA         The matrix object which is the first operand   *
 *              matrixB         The matrix object which is the second operand  *
 *                                                                             *
 * Outputs:     matrixResult    The matrix object which holds the result       *
 *                                                                             *
 * Description: This function multiplies matrixA by matrixB for all frames.    *
 *              The result is stored in matrixResult.                          *
 *                                                                             *
 *      R(0) = A(0) * B(0)                                                     *
 *      R(1) = A(1) * B(1)                                                     *
 *      ...        ...                                                         *
 *      R(k) = A(k) * B(k)                                                     *
 *      ...        ...                                                         *
 *      R(K) = A(K) * B(K)                                                     *
 *                                                                             *
 ******************************************************************************/

void matrixMultMatrix(struct objMatrix* matrixA, struct objMatrix* matrixB, struct objMatrix* matrixResult)
{

    unsigned int indexRow;
    unsigned int indexCol;
    unsigned int indexLine;
    unsigned int k;

    unsigned int newNRows;
    unsigned int newNCols;
    unsigned int newNLines;
    unsigned int newNFrames;

    float elementAReal;
    float elementAImag;
    float elementBReal;
    float elementBImag;

#ifdef USE_SIMD

    // SIMD registers
    __m128_mod regA, regB, regC, regD, regE, regF, regG, regH;

#endif

    // Check if dimensions match
    if ((matrixA->nCols == matrixB->nRows) && (matrixA->nFrames == matrixB->nFrames))
    {

        newNRows = matrixA->nRows;
        newNCols = matrixB->nCols;
        newNLines = matrixA->nCols;
        newNFrames = matrixA->nFrames;

        // Resize the matrix that will contain the result if needed
        if ((matrixResult->nRows != newNRows) || (matrixResult->nCols != newNCols) || (matrixResult->nFrames != newNFrames))
        {
            matrixResize(matrixResult,newNRows,newNCols,newNFrames);
        }

#ifdef USE_SIMD

        // Load the constant zero
        regA.m128_f32[0] = 0.0;
        regA.m128_f32[1] = 0.0;
        regA.m128_f32[2] = 0.0;
        regA.m128_f32[3] = 0.0;

#endif

        // Loop for each element
        for (indexRow = 0; indexRow < newNRows; indexRow++)
        {
            for (indexCol = 0; indexCol < newNCols; indexCol++)
            {

#ifndef USE_SIMD

                for (k = 0; k < newNFrames; k++)
                {                 

                    matrixResult->valueReal[indexRow][indexCol][k] = 0.0;
                    matrixResult->valueImag[indexRow][indexCol][k] = 0.0;
                }

#else

                for (k = 0; k < newNFrames; k+=4)
                {

                    _mm_store_ps(&matrixResult->valueReal[indexRow][indexCol][k],regA.m128);
                    _mm_store_ps(&matrixResult->valueImag[indexRow][indexCol][k],regA.m128);

                }
                for (k = k - 4; k < newNFrames; k++)
                {

                    matrixResult->valueReal[indexRow][indexCol][k] = 0.0;
                    matrixResult->valueImag[indexRow][indexCol][k] = 0.0;
                }

#endif

                for (indexLine = 0; indexLine < newNLines; indexLine++)
                {

#ifndef USE_SIMD

                    for (k = 0; k < newNFrames; k++)
                    {

                        elementAReal = matrixGetReal(matrixA,indexRow,indexLine,k);
                        elementAImag = matrixGetImag(matrixA,indexRow,indexLine,k);
                        elementBReal = matrixGetReal(matrixB,indexLine,indexCol,k);
                        elementBImag = matrixGetImag(matrixB,indexLine,indexCol,k);

                        matrixResult->valueReal[indexRow][indexCol][k] += elementAReal * elementBReal - elementAImag * elementBImag;
                        matrixResult->valueImag[indexRow][indexCol][k] += elementAReal * elementBImag + elementAImag * elementBReal;

                    }

#else

                    for (k = 0; k < newNFrames; k+=4)
                    {

                        // Load values for the element A
                        regB.m128 = _mm_load_ps(&matrixA->valueReal[indexRow][indexLine][k]);
                        regC.m128 = _mm_load_ps(&matrixA->valueImag[indexRow][indexLine][k]);

                        // Load values for the element B
                        regD.m128 = _mm_load_ps(&matrixB->valueReal[indexLine][indexCol][k]);
                        regE.m128 = _mm_load_ps(&matrixB->valueImag[indexLine][indexCol][k]);

                        // Re{Result} = Re{A}*Re{B} - Im{A}*Im{B}
                        regF.m128 = _mm_mul_ps(regB.m128,regD.m128);
                        regG.m128 = _mm_mul_ps(regC.m128,regE.m128);
                        regH.m128 = _mm_sub_ps(regF.m128,regG.m128);

                        // Load accumulator value
                        regF.m128 = _mm_load_ps(&matrixResult->valueReal[indexRow][indexCol][k]);

                        // Add to accumulator
                        regF.m128 = _mm_add_ps(regF.m128,regH.m128);

                        // Store result
                        _mm_store_ps(&matrixResult->valueReal[indexRow][indexCol][k],regF.m128);

                        // Im{Result} = Re{A}*Im{B} + Im{A}*Re{B}
                        regF.m128 = _mm_mul_ps(regB.m128,regE.m128);
                        regG.m128 = _mm_mul_ps(regC.m128,regD.m128);
                        regH.m128 = _mm_add_ps(regF.m128,regG.m128);

                        // Load accumulator value
                        regF.m128 = _mm_load_ps(&matrixResult->valueImag[indexRow][indexCol][k]);

                        // Add to accumulator
                        regF.m128 = _mm_add_ps(regF.m128,regH.m128);

                        // Store result
                        _mm_store_ps(&matrixResult->valueImag[indexRow][indexCol][k],regF.m128);

                    }
                    for (k = k - 4; k < newNFrames; k++)
                    {

                        elementAReal = matrixGetReal(matrixA,indexRow,indexLine,k);
                        elementAImag = matrixGetImag(matrixA,indexRow,indexLine,k);
                        elementBReal = matrixGetReal(matrixB,indexLine,indexCol,k);
                        elementBImag = matrixGetImag(matrixB,indexLine,indexCol,k);

                        matrixResult->valueReal[indexRow][indexCol][k] += elementAReal * elementBReal - elementAImag * elementBImag;
                        matrixResult->valueImag[indexRow][indexCol][k] += elementAReal * elementBImag + elementAImag * elementBReal;

                    }

#endif

                }

            }
        }

    }
    else
    {
        // Create an empty matrix
        matrixResult = matrixCreate(0,0,0);
    }

}

/*******************************************************************************
 * matrixMultScalar                                                            *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      matrixSource    The matrix object which is the first operand   *
 *              scalar          The scalar value which is the second operand   *
 *                                                                             *
 * Outputs:     matrixResult    The matrix object which holds the result       *
 *                                                                             *
 * Description: This function multiplies matrixA by the scalar for all frames. *
 *              The result is stored in matrixResult.                          *
 *                                                                             *
 *      R(0) = scalar * A(0)                                                   *
 *      R(1) = scalar * A(1)                                                   *
 *      ...        ...                                                         *
 *      R(k) = scalar * A(k)                                                   *
 *      ...        ...                                                         *
 *      R(K) = scalar * A(K)                                                   *
 *                                                                             *
 ******************************************************************************/

void matrixMultScalar(struct objMatrix* matrixSource, float scalar, struct objMatrix* matrixResult)
{

    unsigned int indexRow;
    unsigned int indexCol;
    unsigned int k;

    unsigned int newNRows;
    unsigned int newNCols;
    unsigned int newNFrames;

#ifdef USE_SIMD

    // SIMD registers
    __m128_mod regA, regB, regC;

#endif

    newNRows = matrixSource->nRows;
    newNCols = matrixSource->nCols;
    newNFrames = matrixSource->nFrames;

    // Resize the matrix that will contain the result if needed
    if ((matrixResult->nRows != newNRows) || (matrixResult->nCols != newNCols) || (matrixResult->nFrames != newNFrames))
    {
        matrixResize(matrixResult,newNRows,newNCols,newNFrames);
    }

#ifdef USE_SIMD

    // Load the scalar
    regA.m128_f32[0] = scalar;
    regA.m128_f32[1] = scalar;
    regA.m128_f32[2] = scalar;
    regA.m128_f32[3] = scalar;

#endif

    // Loop for each element
    for (indexRow = 0; indexRow < newNRows; indexRow++)
    {
        for (indexCol = 0; indexCol < newNCols; indexCol++)
        {

#ifndef USE_SIMD

            for (k = 0; k < newNFrames; k++)
            {

                matrixResult->valueReal[indexRow][indexCol][k] = matrixSource->valueReal[indexRow][indexCol][k] * scalar;
                matrixResult->valueImag[indexRow][indexCol][k] = matrixSource->valueImag[indexRow][indexCol][k] * scalar;

            }

#else

            for (k = 0; k < newNFrames; k+=4)
            {

                // Load values
                regB.m128 = _mm_load_ps(&matrixSource->valueReal[indexRow][indexCol][k]);
                regC.m128 = _mm_load_ps(&matrixSource->valueImag[indexRow][indexCol][k]);

                // Multiply
                regB.m128 = _mm_mul_ps(regA.m128,regB.m128);
                regC.m128 = _mm_mul_ps(regA.m128,regC.m128);

                // Store values
                _mm_store_ps(&matrixResult->valueReal[indexRow][indexCol][k],regB.m128);
                _mm_store_ps(&matrixResult->valueImag[indexRow][indexCol][k],regC.m128);

            }
            for (k = k - 4; k < newNFrames; k++)
            {

                matrixResult->valueReal[indexRow][indexCol][k] = matrixSource->valueReal[indexRow][indexCol][k] * scalar;
                matrixResult->valueImag[indexRow][indexCol][k] = matrixSource->valueImag[indexRow][indexCol][k] * scalar;

            }

#endif

        }
    }

}

/*******************************************************************************
 * matrixRemoveIdentity                                                        *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      matrix          The matrix object                              *
 *                                                                             *
 * Outputs:     matrixResult    The matrix object which holds the result       *
 *                                                                             *
 * Description: This function removes the identity matrix for each frame.      *
 *              The result is stored in matrixResult.                          *
 *                                                                             *
 *      R(0) = A(0) - I                                                        *
 *      R(1) = A(1) - I                                                        *
 *      ...        ...                                                         *
 *      R(k) = A(k) - I                                                        *
 *      ...        ...                                                         *
 *      R(K) = A(K) - I                                                        *
 *                                                                             *
 ******************************************************************************/

void matrixRemoveIdentity(struct objMatrix* matrix)
{

    unsigned int indexRowCol;
    unsigned int k;

    unsigned int nRowsCols;

#ifdef USE_SIMD

    // SIMD registers
    __m128_mod regA, regB;

#endif

    // Make sure the matrix is square
    if (matrix->nRows == matrix->nCols)
    {

        nRowsCols = matrix->nRows;

#ifdef USE_SIMD

    // Load the constant -1
    regA.m128_f32[0] = -1.0;
    regA.m128_f32[1] = -1.0;
    regA.m128_f32[2] = -1.0;
    regA.m128_f32[3] = -1.0;

#endif

        for (indexRowCol = 0; indexRowCol < nRowsCols; indexRowCol++)
        {

#ifndef USE_SIMD

            for (k = 0; k < matrix->nFrames; k++)
            {
                matrix->valueReal[indexRowCol][indexRowCol][k] -= 1.0f;
            }

#else

            for (k = 0; k < matrix->nFrames; k+=4)
            {
                regB.m128 = _mm_load_ps(&matrix->valueReal[indexRowCol][indexRowCol][k]);
                regB.m128 = _mm_add_ps(regA.m128,regB.m128);
                _mm_store_ps(&matrix->valueReal[indexRowCol][indexRowCol][k], regB.m128);
            }
            for (k = k - 4; k < matrix->nFrames; k++)
            {
                matrix->valueReal[indexRowCol][indexRowCol][k] -= 1.0;
            }

#endif

        }

    }

}

/*******************************************************************************
 * matrixRemoveIdentity                                                        *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      matrix          The matrix object                              *
 *                                                                             *
 * Outputs:     matrixResult    The matrix object which holds the result       *
 *                                                                             *
 * Description: This function sets the diagonal term to zero for each frame.   *
 *              The result is stored in matrixResult.                          *
 *                                                                             *
 *      R(0) = A(0) & (!I)                                                     *
 *      R(1) = A(1) & (!I)                                                     *
 *      ...        ...                                                         *
 *      R(k) = A(k) & (!I)                                                     *
 *      ...        ...                                                         *
 *      R(K) = A(K) & (!I)                                                     *
 *                                                                             *
 ******************************************************************************/

void matrixRemoveDiagonal(struct objMatrix* matrix)
{

    unsigned int indexRowCol;
    unsigned int k;

    unsigned int nRowsCols;

#ifdef USE_SIMD

    // SIMD registers
    __m128_mod regA;

#endif

    // Make sure the matrix is square
    if (matrix->nRows == matrix->nCols)
    {

        nRowsCols = matrix->nRows;

#ifdef USE_SIMD

    // Load the constant 0
    regA.m128_f32[0] = 0.0;
    regA.m128_f32[1] = 0.0;
    regA.m128_f32[2] = 0.0;
    regA.m128_f32[3] = 0.0;

#endif

        for (indexRowCol = 0; indexRowCol < nRowsCols; indexRowCol++)
        {

#ifndef USE_SIMD

            for (k = 0; k < matrix->nFrames; k++)
            {
                matrix->valueReal[indexRowCol][indexRowCol][k] = 0.0f;
                matrix->valueImag[indexRowCol][indexRowCol][k] = 0.0f;
            }

#else

            for (k = 0; k < matrix->nFrames; k+=4)
            {
                _mm_store_ps(&matrix->valueReal[indexRowCol][indexRowCol][k],regA.m128);
                _mm_store_ps(&matrix->valueImag[indexRowCol][indexRowCol][k],regA.m128);
            }
            for (k = k - 4; k < matrix->nFrames; k++)
            {
                matrix->valueReal[indexRowCol][indexRowCol][k] = 0.0;
                matrix->valueImag[indexRowCol][indexRowCol][k] = 0.0;
            }

#endif

        }

    }

}

/*******************************************************************************
 * matrixHermitian     		                                               *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      matrixSource    The matrix object used                         *
  *                                                                            *
 * Outputs:     matrixDest      The matrix object that holds the result        *
 *                                                                             *
 * Description: This function computes the hermitian matrix.                   *
 *                                                                             *
 ******************************************************************************/

void matrixHermitian(struct objMatrix* matrixSource, struct objMatrix* matrixDest)
{

    unsigned int newNRows;
    unsigned int newNCols;
    unsigned int newNFrames;

    unsigned int indexRow;
    unsigned int indexCol;
    unsigned int k;

#ifdef USE_SIMD

    // SIMD registers
    __m128_mod regA, regB, regC;

#endif

    newNRows = matrixSource->nCols;
    newNCols = matrixSource->nRows;
    newNFrames = matrixSource->nFrames;

    // Resize the matrix that will contain the result if needed
    if ((matrixDest->nRows != newNRows) || (matrixDest->nCols != newNCols) || (matrixDest->nFrames != newNFrames))
    {
        matrixResize(matrixDest,newNRows,newNCols,newNFrames);
    }

#ifdef USE_SIMD

    // Load the constant -1
    regA.m128_f32[0] = -1.0;
    regA.m128_f32[1] = -1.0;
    regA.m128_f32[2] = -1.0;
    regA.m128_f32[3] = -1.0;

#endif

    // Move content
    for (indexRow = 0; indexRow < matrixSource->nRows; indexRow++)
    {
        for (indexCol = 0; indexCol < matrixSource->nCols; indexCol++)
        {

#ifndef USE_SIMD

            for (k = 0; k < matrixSource->nFrames; k++)
            {
                matrixSetReal(matrixDest,indexCol,indexRow,k,matrixGetReal(matrixSource,indexRow,indexCol,k));
                matrixSetImag(matrixDest,indexCol,indexRow,k,-1.0f * matrixGetImag(matrixSource,indexRow,indexCol,k));
            }

#else

            for (k = 0; k < matrixSource->nFrames; k+=4)
            {

                // Load the values
                regB.m128 = _mm_load_ps(&matrixSource->valueReal[indexRow][indexCol][k]);
                regC.m128 = _mm_load_ps(&matrixSource->valueImag[indexRow][indexCol][k]);

                // Compute the complexe conjugate
                regC.m128 = _mm_mul_ps(regC.m128,regA.m128);

                // Save the values
                _mm_store_ps(&matrixDest->valueReal[indexCol][indexRow][k],regB.m128);
                _mm_store_ps(&matrixDest->valueImag[indexCol][indexRow][k],regC.m128);

            }
            for (k = k - 4; k < matrixSource->nFrames; k++)
            {
                matrixSetReal(matrixDest,indexCol,indexRow,k,matrixGetReal(matrixSource,indexRow,indexCol,k));
                matrixSetImag(matrixDest,indexCol,indexRow,k,-1.0 * matrixGetImag(matrixSource,indexRow,indexCol,k));
            }

#endif

        }
    }

}

void matrixMultMatrixPerElement(struct objMatrix* matrixA, struct objMatrix* matrixB, struct objMatrix* matrixResult)
{

    unsigned int indexRow;
    unsigned int indexCol;
    unsigned int k;

    unsigned int nRows;
    unsigned int nCols;
    unsigned int nFrames;

    float elementAReal;
    float elementAImag;
    float elementBReal;
    float elementBImag;
    float resultReal;
    float resultImag;

#ifdef USE_SIMD

    // SIMD registers
    __m128_mod regA, regB, regC, regD, regE, regF, regG, regH;

#endif

    if ((matrixA->nCols == matrixB->nCols) && (matrixA->nRows == matrixB->nRows))
    {

        nRows = matrixA->nRows;
        nCols = matrixA->nCols;
        nFrames = matrixA->nFrames;

        if ((matrixResult->nRows != nRows) || (matrixResult->nCols != nCols) || (matrixResult->nFrames != nFrames))
        {
            matrixResize(matrixResult,nRows,nCols,nFrames);
        }

        for (indexRow = 0; indexRow < nRows; indexRow++)
        {

            for (indexCol = 0; indexCol < nCols; indexCol++)
            {

#ifndef USE_SIMD

                for (k = 0; k < nFrames; k++)
                {

                    elementAReal = matrixGetReal(matrixA,indexRow,indexCol,k);
                    elementAImag = matrixGetImag(matrixA,indexRow,indexCol,k);
                    elementBReal = matrixGetReal(matrixB,indexRow,indexCol,k);
                    elementBImag = matrixGetImag(matrixB,indexRow,indexCol,k);

                    // Re{A*B} = Re{A}*Re{B} - Im{A}*Im{B}
                    resultReal = elementAReal * elementBReal - elementAImag * elementBImag;

                    // Im{A*B} = Re{A}*Im{B} + Im{A}*Re{B}
                    resultImag = elementAReal * elementBImag + elementAImag * elementBReal;

                    matrixSetReal(matrixResult,indexRow,indexCol,k,resultReal);
                    matrixSetImag(matrixResult,indexRow,indexCol,k,resultImag);

                }

#else

                for (k = 0; k < nFrames; k+=4)
                {

                    regA.m128 = _mm_load_ps(&matrixA->valueReal[indexRow][indexCol][k]);
                    regB.m128 = _mm_load_ps(&matrixA->valueImag[indexRow][indexCol][k]);
                    regC.m128 = _mm_load_ps(&matrixB->valueReal[indexRow][indexCol][k]);
                    regD.m128 = _mm_load_ps(&matrixB->valueImag[indexRow][indexCol][k]);

                    // Re{A*B} = Re{A}*Re{B} - Im{A}*Im{B}
                    regE.m128 = _mm_mul_ps(regA.m128,regC.m128);
                    regF.m128 = _mm_mul_ps(regB.m128,regD.m128);
                    regG.m128 = _mm_sub_ps(regE.m128,regF.m128);

                    // Im{A*B} = Re{A}*Im{B} + Im{A}*Re{B}
                    regE.m128 = _mm_mul_ps(regA.m128,regD.m128);
                    regF.m128 = _mm_mul_ps(regB.m128,regC.m128);
                    regH.m128 = _mm_add_ps(regE.m128,regF.m128);

                    _mm_store_ps(&matrixResult->valueReal[indexRow][indexCol][k],regG.m128);
                    _mm_store_ps(&matrixResult->valueImag[indexRow][indexCol][k],regH.m128);

                }
                for (k = k - 4; k < nFrames; k++)
                {

                    elementAReal = matrixGetReal(matrixA,indexRow,indexCol,k);
                    elementAImag = matrixGetImag(matrixA,indexRow,indexCol,k);
                    elementBReal = matrixGetReal(matrixB,indexRow,indexCol,k);
                    elementBImag = matrixGetImag(matrixB,indexRow,indexCol,k);

                    // Re{A*B} = Re{A}*Re{B} - Im{A}*Im{B}
                    resultReal = elementAReal * elementBReal - elementAImag * elementBImag;

                    // Im{A*B} = Re{A}*Im{B} + Im{A}*Re{B}
                    resultImag = elementAReal * elementBImag + elementAImag * elementBReal;

                    matrixSetReal(matrixResult,indexRow,indexCol,k,resultReal);
                    matrixSetImag(matrixResult,indexRow,indexCol,k,resultImag);

                }

#endif

            }

        }

    }

}

void matrixDividePerElement(struct objMatrix* matrixA, struct objMatrix* matrixB, struct objMatrix* matrixResult)
{

    unsigned int indexRow;
    unsigned int indexCol;
    unsigned int k;

    unsigned int nRows;
    unsigned int nCols;
    unsigned int nFrames;

    float elementAReal;
    float elementAImag;
    float elementBReal;
    float elementBImag;
    float resultReal;
    float resultImag;

#ifdef USE_SIMD

    // SIMD registers
    __m128_mod regA, regB, regC, regD, regE, regF, regG, regH;

#endif

    if ((matrixA->nCols == matrixB->nCols) && (matrixA->nRows == matrixB->nRows))
    {

        nRows = matrixA->nRows;
        nCols = matrixA->nCols;
        nFrames = matrixA->nFrames;

        if ((matrixResult->nRows != nRows) || (matrixResult->nCols != nCols) || (matrixResult->nFrames != nFrames))
        {
            matrixResize(matrixResult,nRows,nCols,nFrames);
        }

        for (indexRow = 0; indexRow < nRows; indexRow++)
        {

            for (indexCol = 0; indexCol < nCols; indexCol++)
            {

#ifndef USE_SIMD

                for (k = 0; k < nFrames; k++)
                {

                    elementAReal = matrixGetReal(matrixA,indexRow,indexCol,k);
                    elementAImag = matrixGetImag(matrixA,indexRow,indexCol,k);
                    elementBReal = matrixGetReal(matrixB,indexRow,indexCol,k);
                    elementBImag = matrixGetImag(matrixB,indexRow,indexCol,k);

                    // Re{A*B} = (Re{A}*Re{B} + Im{A}*Im{B})/(Re{B}*Re{B} + Im{B}*Im{B})
                    resultReal = (elementAReal * elementBReal + elementAImag * elementBImag) /
                                 (elementBReal * elementBReal + elementBImag * elementBImag);

                    // Im{A*B} = (Im{A}*Re{B} - Re{A}*Im{B})/(Re{B}*Re{B} + Im{B}*Im{B})
                    resultImag = (elementAImag * elementBReal - elementAReal * elementBImag) /
                                 (elementBReal * elementBReal + elementBImag * elementBImag);

                    matrixSetReal(matrixResult,indexRow,indexCol,k,resultReal);
                    matrixSetImag(matrixResult,indexRow,indexCol,k,resultImag);

                }

#else

                for (k = 0; k < nFrames; k+=4)
                {

                    regA.m128 = _mm_load_ps(&matrixA->valueReal[indexRow][indexCol][k]);
                    regB.m128 = _mm_load_ps(&matrixA->valueImag[indexRow][indexCol][k]);
                    regC.m128 = _mm_load_ps(&matrixB->valueReal[indexRow][indexCol][k]);
                    regD.m128 = _mm_load_ps(&matrixB->valueImag[indexRow][indexCol][k]);

                    // (Re{B}*Re{B} + Im{B}*Im{B})
                    regE.m128 = _mm_mul_ps(regC.m128,regC.m128);
                    regF.m128 = _mm_mul_ps(regD.m128,regD.m128);
                    regH.m128 = _mm_add_ps(regE.m128,regF.m128);

                    // (Re{A}*Re{B} + Im{A}*Im{B})
                    regE.m128 = _mm_mul_ps(regA.m128,regC.m128);
                    regF.m128 = _mm_mul_ps(regB.m128,regD.m128);
                    regG.m128 = _mm_add_ps(regE.m128,regF.m128);

                    // Re{A*B} = (Re{A}*Re{B} + Im{A}*Im{B})/(Re{B}*Re{B} + Im{B}*Im{B})
                    regE.m128 = _mm_div_ps(regG.m128,regH.m128);
                    _mm_store_ps(&matrixResult->valueReal[indexRow][indexCol][k],regE.m128);

                    // (Im{A}*Re{B} - Re{A}*Im{B})
                    regE.m128 = _mm_mul_ps(regB.m128,regC.m128);
                    regF.m128 = _mm_mul_ps(regA.m128,regD.m128);
                    regG.m128 = _mm_sub_ps(regE.m128,regF.m128);

                    // Im{A*B} = (Im{A}*Re{B} - Re{A}*Im{B})/(Re{B}*Re{B} + Im{B}*Im{B})
                    regE.m128 = _mm_div_ps(regG.m128,regH.m128);
                    _mm_store_ps(&matrixResult->valueImag[indexRow][indexCol][k],regE.m128);

                }
                for (k = k - 4; k < nFrames; k++)
                {

                    elementAReal = matrixGetReal(matrixA,indexRow,indexCol,k);
                    elementAImag = matrixGetImag(matrixA,indexRow,indexCol,k);
                    elementBReal = matrixGetReal(matrixB,indexRow,indexCol,k);
                    elementBImag = matrixGetImag(matrixB,indexRow,indexCol,k);

                    // Re{A*B} = (Re{A}*Re{B} + Im{A}*Im{B})/(Re{B}*Re{B} + Im{B}*Im{B})
                    resultReal = (elementAReal * elementBReal + elementAImag * elementBImag) /
                                 (elementBReal * elementBReal + elementBImag * elementBImag);

                    // Im{A*B} = (Im{A}*Re{B} - Re{A}*Im{B})/(Re{B}*Re{B} + Im{B}*Im{B})
                    resultImag = (elementAImag * elementBReal - elementAReal * elementBImag) /
                                 (elementBReal * elementBReal + elementBImag * elementBImag);

                    matrixSetReal(matrixResult,indexRow,indexCol,k,resultReal);
                    matrixSetImag(matrixResult,indexRow,indexCol,k,resultImag);

                }

#endif

            }

        }

    }

}

void matrixInvRealPerElement(struct objMatrix* matrixSource, struct objMatrix* matrixResult)
{

    unsigned int indexRow;
    unsigned int indexCol;
    unsigned int k;

    unsigned int nRows;
    unsigned int nCols;
    unsigned int nFrames;

    float element;
    float result;

#ifdef USE_SIMD

    // SIMD registers
    __m128_mod regA, regB, regD, regF;

#endif

    nRows = matrixSource->nRows;
    nCols = matrixSource->nCols;
    nFrames = matrixSource->nFrames;

#ifdef USE_SIMD

    // Load the constant 1
    regA.m128_f32[0] = 1.0;
    regA.m128_f32[1] = 1.0;
    regA.m128_f32[2] = 1.0;
    regA.m128_f32[3] = 1.0;

    // Load the constant 0
    regB.m128_f32[0] = 0.0;
    regB.m128_f32[1] = 0.0;
    regB.m128_f32[2] = 0.0;
    regB.m128_f32[3] = 0.0;

#endif

    if ((matrixResult->nRows != nRows) || (matrixResult->nCols != nCols) || (matrixResult->nFrames != nFrames))
    {
        matrixResize(matrixResult,nRows,nCols,nFrames);
    }

    for (indexRow = 0; indexRow < nRows; indexRow++)
    {

        for (indexCol = 0; indexCol < nCols; indexCol++)
        {

#ifndef USE_SIMD

            for (k = 0; k < nFrames; k++)
            {

                element = matrixGetReal(matrixSource,indexRow,indexCol,k);

                result = (float) (1.0/(element));

                matrixSetReal(matrixResult,indexRow,indexCol,k,result);

            }

#else

            for (k = 0; k < nFrames; k+=4)
            {

                regD.m128 = _mm_load_ps(&matrixSource->valueReal[indexRow][indexCol][k]);

                regF.m128 = _mm_div_ps(regA.m128,regD.m128);

                _mm_store_ps(&matrixResult->valueReal[indexRow][indexCol][k],regF.m128);
                _mm_store_ps(&matrixResult->valueImag[indexRow][indexCol][k],regB.m128);

            }
            for (k = k - 4; k < nFrames; k++)
            {

                element = matrixGetReal(matrixSource,indexRow,indexCol,k);

                result = 1.0/(element);

                matrixSetReal(matrixResult,indexRow,indexCol,k,result);

            }

#endif

        }

    }



}

void matrixMultScalarPerFrame(struct objMatrix* matrixSource, struct objMatrix* matrixFrames, struct objMatrix* matrixResult)
{

    unsigned int nRows;
    unsigned int nCols;
    unsigned int nFrames;

    unsigned int indexRow;
    unsigned int indexCol;
    unsigned int k;

    float elementReal;
    float elementImag;
    float scalar;
    float resultReal;
    float resultImag;

#ifdef USE_SIMD

    // SIMD registers
    __m128_mod regA, regB, regC, regD, regE;

#endif

    nRows = matrixSource->nRows;
    nCols = matrixSource->nCols;
    nFrames = matrixSource->nFrames;   

    if ((matrixResult->nRows != nRows) || (matrixResult->nCols != nCols) || (matrixResult->nFrames != nFrames))
    {
        matrixResize(matrixResult,nRows,nCols,nFrames);
    }

    for (indexRow = 0; indexRow < nRows; indexRow++)
    {

        for (indexCol = 0; indexCol < nCols; indexCol++)
        {

#ifndef USE_SIMD

            for (k = 0; k < nFrames; k++)
            {

                scalar = matrixGetReal(matrixFrames,0,0,k);

                elementReal = matrixGetReal(matrixSource,indexRow,indexCol,k);
                elementImag = matrixGetImag(matrixSource,indexRow,indexCol,k);

                resultReal = scalar * elementReal;
                resultImag = scalar * elementImag;

                matrixSetReal(matrixResult,indexRow,indexCol,k,resultReal);
                matrixSetImag(matrixResult,indexRow,indexCol,k,resultImag);

            }

#else

            for (k = 0; k < nFrames; k+=4)
            {

                regA.m128 = _mm_load_ps(&matrixFrames->valueReal[0][0][k]);

                regB.m128 = _mm_load_ps(&matrixSource->valueReal[indexRow][indexCol][k]);
                regC.m128 = _mm_load_ps(&matrixSource->valueImag[indexRow][indexCol][k]);

                regD.m128 = _mm_mul_ps(regA.m128,regB.m128);
                regE.m128 = _mm_mul_ps(regA.m128,regC.m128);

                _mm_store_ps(&matrixResult->valueReal[indexRow][indexCol][k],regD.m128);
                _mm_store_ps(&matrixResult->valueImag[indexRow][indexCol][k],regE.m128);

            }
            for (k = k - 4; k < nFrames; k++)
            {

                scalar = matrixGetReal(matrixFrames,0,0,k);

                elementReal = matrixGetReal(matrixSource,indexRow,indexCol,k);
                elementImag = matrixGetImag(matrixSource,indexRow,indexCol,k);

                resultReal = scalar * elementReal;
                resultImag = scalar * elementImag;

                matrixSetReal(matrixResult,indexRow,indexCol,k,resultReal);
                matrixSetImag(matrixResult,indexRow,indexCol,k,resultImag);

            }

#endif

        }

    }
}

void matrixAddMatrix(struct objMatrix* matrixA, struct objMatrix* matrixB, struct objMatrix* matrixResult)
{

    unsigned int nRows;
    unsigned int nCols;
    unsigned int nFrames;

    unsigned int indexRow;
    unsigned int indexCol;
    unsigned int k;

    float elementAReal;
    float elementAImag;
    float elementBReal;
    float elementBImag;
    float resultReal;
    float resultImag;

#ifdef USE_SIMD

    // SIMD registers
    __m128_mod regA, regB, regC, regD, regE, regF;

#endif

    if ((matrixA->nRows == matrixB->nRows) && (matrixA->nCols == matrixB->nCols) && (matrixA->nFrames == matrixB->nFrames))
    {

        nRows = matrixA->nRows;
        nCols = matrixA->nCols;
        nFrames = matrixA->nFrames;

        if ((matrixA->nRows != nRows) || (matrixA->nCols != nCols) || (matrixA->nFrames != nFrames))
        {
            matrixResize(matrixResult,nRows,nCols,nFrames);
        }

        for (indexRow = 0; indexRow < nRows; indexRow++)
        {
            for (indexCol = 0; indexCol < nCols; indexCol++)
            {

#ifndef USE_SIMD

                for (k = 0; k < nFrames; k++)
                {
                    elementAReal = matrixGetReal(matrixA,indexRow,indexCol,k);
                    elementAImag = matrixGetImag(matrixA,indexRow,indexCol,k);
                    elementBReal = matrixGetReal(matrixB,indexRow,indexCol,k);
                    elementBImag = matrixGetImag(matrixB,indexRow,indexCol,k);

                    // Re{A+B} = Re{A} + Re{B}
                    resultReal = elementAReal + elementBReal;
                    // Im{A+B} = Im{A} + Im{B}
                    resultImag = elementAImag + elementBImag;

                    matrixSetReal(matrixResult,indexRow,indexCol,k,resultReal);
                    matrixSetImag(matrixResult,indexRow,indexCol,k,resultImag);
                }

#else

                for (k = 0; k < nFrames; k+=4)
                {

                    regA.m128 = _mm_load_ps(&matrixA->valueReal[indexRow][indexCol][k]);
                    regB.m128 = _mm_load_ps(&matrixA->valueImag[indexRow][indexCol][k]);
                    regC.m128 = _mm_load_ps(&matrixB->valueReal[indexRow][indexCol][k]);
                    regD.m128 = _mm_load_ps(&matrixB->valueImag[indexRow][indexCol][k]);

                    // Re{A+B} = Re{A} + Re{B}
                    regE.m128 = _mm_add_ps(regA.m128,regC.m128);

                    // Im{A+B} = Im{A} + Im{B}
                    regF.m128 = _mm_add_ps(regB.m128,regD.m128);

                    _mm_store_ps(&matrixResult->valueReal[indexRow][indexCol][k],regE.m128);
                    _mm_store_ps(&matrixResult->valueImag[indexRow][indexCol][k],regF.m128);

                }
                for (k = k - 4; k < nFrames; k++)
                {
                    elementAReal = matrixGetReal(matrixA,indexRow,indexCol,k);
                    elementAImag = matrixGetImag(matrixA,indexRow,indexCol,k);
                    elementBReal = matrixGetReal(matrixB,indexRow,indexCol,k);
                    elementBImag = matrixGetImag(matrixB,indexRow,indexCol,k);

                    // Re{A+B} = Re{A} + Re{B}
                    resultReal = elementAReal + elementBReal;
                    // Im{A+B} = Im{A} + Im{B}
                    resultImag = elementAImag + elementBImag;

                    matrixSetReal(matrixResult,indexRow,indexCol,k,resultReal);
                    matrixSetImag(matrixResult,indexRow,indexCol,k,resultImag);
                }

#endif

            }
        }

    }


}

void matrixMakeNonZero(struct objMatrix* matrix)
{

    unsigned int nRows;
    unsigned int nCols;
    unsigned int nFrames;

    unsigned int indexRow;
    unsigned int indexCol;
    unsigned int k;

    float minValue = (float) (1E-10);

    nRows = matrix->nRows;
    nCols = matrix->nCols;
    nFrames = matrix->nFrames;

    for (indexRow = 0; indexRow < nRows; indexRow++)
    {
        for (indexCol = 0; indexCol < nCols; indexCol++)
        {

            for (k = 0; k < nFrames; k++)
            {
                if ((matrixGetReal(matrix,indexRow,indexCol,k) == 0) && (matrixGetImag(matrix,indexRow,indexCol,k)==0))
                {
                    matrixSetReal(matrix,indexRow,indexCol,k,minValue);
                }
            }

        }
    }
}

/*******************************************************************************
 * matrixPrint     		                                               *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      matrix          The matrix object used                         *
  *                                                                            *
 * Outputs:     (console)                                                      *
 *                                                                             *
 * Description: This function prints the content of a matrix.                  *
 *                                                                             *
 ******************************************************************************/

void matrixPrint(struct objMatrix* matrix)
{

    unsigned int indexRow;
    unsigned int indexCol;
    unsigned int k;

    if ((matrix->nRows == 0) || (matrix->nCols == 0) || (matrix->nFrames == 0))
    {
        printf("(Empty)\n");
    }
    else
    {
        for (k = 0; k < matrix->nFrames; k++)
        {

            printf("\nFrame: %i\n\n",k);

            for (indexRow = 0; indexRow < matrix->nRows; indexRow++)
            {

                for (indexCol = 0; indexCol < matrix->nCols; indexCol++)
                {

                    printf("%1.2E + j %1.2E     ",matrixGetReal(matrix,indexRow,indexCol,k),matrixGetImag(matrix,indexRow,indexCol,k));

                }

                printf("\n");
            }

        }

        printf("\n");

    }
}

void matrixPrintOneFrame(struct objMatrix* matrix, unsigned int k)
{

    unsigned int indexRow;
    unsigned int indexCol;

    if ((matrix->nRows == 0) || (matrix->nCols == 0) || (matrix->nFrames == 0))
    {
        printf("(Empty)\n");
    }
    else
    {

        printf("\nFrame: %i\n\n",k);

        for (indexRow = 0; indexRow < matrix->nRows; indexRow++)
        {

            for (indexCol = 0; indexCol < matrix->nCols; indexCol++)
            {

                printf("%1.2E + j %1.2E     ",matrixGetReal(matrix,indexRow,indexCol,k),matrixGetImag(matrix,indexRow,indexCol,k));

            }

            printf("\n");
        }

        printf("\n");
    }
}
