/*******************************************************************************
 * ManyEars: linearCorrelation - Source Code                                   *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Author: François Grondin                                                    *
 * Original Code: Jean-Marc Valin                                              *
 * Modified Code: Simon Brière                                                 *
 * Version: 1.1.0                                                              *
 * Date: November 1st , 2010                                                   *
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

#include "Utilities/linearCorrelation.h"

/*******************************************************************************
 * linearCorrelationInit                                                       *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myLinearCorrelation     The object to be initialized           *
 *              vectorASize             The size of the first vector           *
 *              vectorBSize             The size of the second vector          *
 *                                                                             *
 * Outputs:     myLinearCorrelation     The initialized object                 *
 *                                                                             *
 * Description: This function initializes the objLinearCorrelation object      *
 *                                                                             *
 ******************************************************************************/

void linearCorrelationInit(struct objLinearCorrelation* myLinearCorrelation, unsigned int vectorASize, unsigned int vectorBSize)
{

    // *************************************************************************
    // * STEP 1: Save parameters                                               *
    // *************************************************************************

    myLinearCorrelation->vectorASize = vectorASize;
    myLinearCorrelation->vectorBSize = vectorBSize;

    // *************************************************************************
    // * STEP 2: Adjust the size such that SIMD operations can be performed    *
    // *************************************************************************

    // +-----------------------------------------------------------------------+
    // | Step A: Vector A                                                      |
    // +-----------------------------------------------------------------------+

        // Vector A stands for the SMALLEST vector
        //
        // Different size must be dealt with:
        //
        // CASE 1:  Multiple of 4: Add a block of 4 samples filled with zeros
        // ------
        //
        //              0               4
        //              +---+---+---+---+
        // 4 samples:   | S | S | S | S |
        //              +---+---+---+---+
        //
        // becomes
        //
        //              0               4               8
        //              +---+---+---+---+---+---+---+---+
        // 8 samples:   | S | S | S | S | 0 | 0 | 0 | 0 |
        //              +--------------------------------
        //
        // CASE 2:  Remainder of 1, 2 or 3: Padd & add a block of 4 samples
        // ------   filled with zeros
        //
        //              0               4   5
        //              +---+---+---+---+---+
        // 5 samples:   | S | S | S | S | S |
        //              +---+---+---+---+---+
        //
        // becomes
        //
        //              0               4               8              12
        //              +---+---+---+---+---+---+---+---+---+---+---+---+
        // 12 samples:  | S | S | S | S | S | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
        //              +-----------------------------------------------+
        //
        //
        //
        //              0               4       6
        //              +---+---+---+---+---+---+
        // 6 samples:   | S | S | S | S | S | S |
        //              +---+---+---+---+---+---+
        //
        // becomes
        //
        //              0               4               8              12
        //              +---+---+---+---+---+---+---+---+---+---+---+---+
        // 12 samples:  | S | S | S | S | S | S | 0 | 0 | 0 | 0 | 0 | 0 |
        //              +-----------------------------------------------+
        //
        //
        //
        //              0               4           7
        //              +---+---+---+---+---+---+---+
        // 7 samples:   | S | S | S | S | S | S | S |
        //              +---+---+---+---+---+---+---+
        //
        // becomes
        //
        //              0               4               8              12
        //              +---+---+---+---+---+---+---+---+---+---+---+---+
        // 12 samples:  | S | S | S | S | S | S | S | 0 | 0 | 0 | 0 | 0 |
        //              +-----------------------------------------------+

        if (myLinearCorrelation->vectorASize == (myLinearCorrelation->vectorASize / 4) * 4)
        {
            myLinearCorrelation->vectorASizeExt = myLinearCorrelation->vectorASize + 4;
            myLinearCorrelation->vectorAnBlocks = myLinearCorrelation->vectorASizeExt / 4;
            myLinearCorrelation->vectorABlockPadding = 1;
            myLinearCorrelation->vectorAZeroPadding = 0;
        }
        else
        {
            myLinearCorrelation->vectorASizeExt = ((myLinearCorrelation->vectorASize / 4) * 4) + 8;
            myLinearCorrelation->vectorAnBlocks = myLinearCorrelation->vectorASizeExt / 4;
            myLinearCorrelation->vectorABlockPadding = 1;
            myLinearCorrelation->vectorAZeroPadding = 4 - (myLinearCorrelation->vectorASize - (myLinearCorrelation->vectorASize / 4) * 4);
        }

    // +-----------------------------------------------------------------------+
    // | Step B: Vector B                                                      |
    // +-----------------------------------------------------------------------+

        // Vector B stands for the LARGEST vector
        //
        // (n - 1) blocks of 4 samples must be added before and after the vector
        // once it has been padded with zeros to have a multiple of 4 as the
        // number of samples, where n stands for the number of blocks in vector A
        //
        // Consider the following vector A:
        //
        //              0               4               8              12
        //              +---+---+---+---+---+---+---+---+---+---+---+---+
        // 12 samples:  | S | S | S | S | S | S | S | S | 0 | 0 | 0 | 0 |
        //              +---+---+---+---+---+---+---+---+---+---+---+---+
        //
        // This vector can be shifted as follows:
        //
        //              0               4               8              12
        //              +---+---+---+---+---+---+---+---+---+---+---+---+
        // 12 samples:  | 0 | S | S | S | S | S | S | S | S | 0 | 0 | 0 |
        //              +---+---+---+---+---+---+---+---+---+---+---+---+
        //
        //              0               4               8              12
        //              +---+---+---+---+---+---+---+---+---+---+---+---+
        // 12 samples:  | 0 | 0 | S | S | S | S | S | S | S | S | 0 | 0 |
        //              +---+---+---+---+---+---+---+---+---+---+---+---+
        //
        //              0               4               8              12
        //              +---+---+---+---+---+---+---+---+---+---+---+---+
        // 12 samples:  | 0 | 0 | 0 | S | S | S | S | S | S | S | S | 0 |
        //              +---+---+---+---+---+---+---+---+---+---+---+---+
        //
        // Consider the following vector B:
        //
        //              0              ...             48
        //              +---+---+---+---+---+---+---+---+
        // 48 samples:  | S | S | S |  ...  | S | S | S |
        //              +---+---+---+---+---+---+---+---+
        //
        // For the worst case in vector A, the following last computation must
        // be made:
        //                                               0              ...             48
        //                                              +---+---+---+---+---+---+---+---+
        //                                 48 samples:  | S | S | S |  ...  | S | S | S |
        //                                              +---+---+---+---+---+---+---+---+
        //                                                |
        //                              X                 |
        //                                                v
        //              0               4               8              12
        //              +---+---+---+---+---+---+---+---+---+---+---+---+
        // 12 samples:  | 0 | S | S | S | S | S | S | S | S | 0 | 0 | 0 |
        //              +---+---+---+---+---+---+---+---+---+---+---+---+
        //
        // Therefore there needs to be additional blocks to pad. In this case, n = 3
        // (12 / 4) and thus need to add n - 1 = 2 blocks:
        //
        //              -8              -4              0              ...             48
        //              +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
        // 48 samples:  | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | S | S | S |  ...  | S | S | S |
        //              +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
        //                                                |
        //                              X                 |
        //                                                v
        //              0               4               8              12
        //              +---+---+---+---+---+---+---+---+---+---+---+---+
        // 12 samples:  | 0 | S | S | S | S | S | S | S | S | 0 | 0 | 0 |
        //              +---+---+---+---+---+---+---+---+---+---+---+---+
        //
        //              0              ...             48
        //              +---+---+---+---+---+---+---+---+
        // 48 samples:  | S | S | S |  ...  | S | S | S |
        //              +---+---+---+---+---+---+---+---+
        //                                            |
        //                              X             |
        //                                            v
        //                              0               4               8              12
        //                              +---+---+---+---+---+---+---+---+---+---+---+---+
        //                 12 samples:  | 0 | 0 | 0 | S | S | S | S | S | S | S | S | 0 |
        //                              +---+---+---+---+---+---+---+---+---+---+---+---+
        //
        // Therefore there needs to be additional blocks to pad. In this case, n = 3
        // (12 / 4) and thus need to add n - 1 = 2 blocks:
        //
        //
        //              0              ...             48              52              56
        //              +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
        // 48 samples:  | S | S | S |  ...  | S | S | S | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
        //              +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
        //
        //                              X
        //
        //                              0               4               8              12
        //                              +---+---+---+---+---+---+---+---+---+---+---+---+
        //                 12 samples:  | 0 | 0 | 0 | S | S | S | S | S | S | S | S | 0 |
        //                              +---+---+---+---+---+---+---+---+---+---+---+---+

        myLinearCorrelation->vectorBBlockPaddingBefore = myLinearCorrelation->vectorAnBlocks - 1;
        myLinearCorrelation->vectorBBlockPaddingAfter = myLinearCorrelation->vectorAnBlocks - 1;

        if (myLinearCorrelation->vectorBSize == (myLinearCorrelation->vectorBSize / 4) * 4)
        {
            myLinearCorrelation->vectorBSizeExt = 4 * myLinearCorrelation->vectorBBlockPaddingBefore +
                                                  ((myLinearCorrelation->vectorBSize / 4) * 4) +
                                                  4 * myLinearCorrelation->vectorBBlockPaddingAfter;
            myLinearCorrelation->vectorBZeroPadding = 0;
        }
        else
        {
            myLinearCorrelation->vectorBSizeExt = 4 * myLinearCorrelation->vectorBBlockPaddingBefore +
                                                  ((myLinearCorrelation->vectorBSize / 4) * 4 + 4) +
                                                  4 * myLinearCorrelation->vectorBBlockPaddingAfter;
            myLinearCorrelation->vectorBZeroPadding = myLinearCorrelation->vectorBSize - ((myLinearCorrelation->vectorBSize / 4) * 4);
        }

        myLinearCorrelation->vectorBnBlocks = myLinearCorrelation->vectorBSizeExt / 4;

        // +-----------------------------------------------------------------------+
        // | Step C: Vector Result                                                 |
        // +-----------------------------------------------------------------------+

        // Linear convolution between a vector of size m and a vector of size n
        // gives a vector of size (n+m-1)

        myLinearCorrelation->vectorResultSize = myLinearCorrelation->vectorASize +
                                                myLinearCorrelation->vectorBSize -
                                                1;

        // Number of blocks: all blocks of vector A - those of vectors A + 1
        myLinearCorrelation->vectorResultnBlocks = myLinearCorrelation->vectorBnBlocks -
                                                   myLinearCorrelation->vectorAnBlocks +
                                                   1;

        myLinearCorrelation->vectorResultSizeExt = myLinearCorrelation->vectorResultnBlocks * 4;

        // Look at what the padding does:
        //
        // CASE 1: No zero padding in vector A
        // ------
        //
        // Vector B:
        //
        //              -4              0              ...             48
        //              +---+---+---+---+---+---+---+---+---+---+---+---+
        //              | 0 | 0 | 0 | 0 | S | S | S |  ...  | S | S | S |
        //              +---+---+---+---+---+---+---+---+---+---+---+---+
        //
        // Vector A:                    X
        //
        //              0               4               8
        //              +---+---+---+---+---+---+---+---+
        // No shift:    | S | S | S | S | 0 | 0 | 0 | 0 |   Result == 0
        //              +--------------------------------
        //
        //              0               4               8
        //              +---+---+---+---+---+---+---+---+
        // Shift by 1:  | 0 | S | S | S | S | 0 | 0 | 0 |   Result != 0
        //              +--------------------------------
        //
        // TOTAL ZEROS PRODUCED: 1
        //
        //
        // CASE 2: Three zeros added in vector A
        // ------
        //
        //              -8              -4              0              ...             48
        //              +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
        // Vector B:    | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | S | S | S |  ...  | S | S | S |
        //              +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
        //
        // Vector A:                    X
        //
        //              0               4               8              12
        //              +---+---+---+---+---+---+---+---+---+---+---+---+
        // No shift:    | S | S | S | S | S | 0 | 0 | 0 | 0 | 0 | 0 | 0 |   Result == 0
        //              +---+---+---+---+---+---+---+---+---+---+---+---+
        //
        //              0               4               8              12
        //              +---+---+---+---+---+---+---+---+---+---+---+---+
        // Shift by 1:  | 0 | S | S | S | S | S | 0 | 0 | 0 | 0 | 0 | 0 |   Result == 0
        //              +---+---+---+---+---+---+---+---+---+---+---+---+
        //
        //              0               4               8              12
        //              +---+---+---+---+---+---+---+---+---+---+---+---+
        // Shift by 2:  | 0 | 0 | S | S | S | S | S | 0 | 0 | 0 | 0 | 0 |   Result == 0
        //              +---+---+---+---+---+---+---+---+---+---+---+---+
        //
        //              0               4               8              12
        //              +---+---+---+---+---+---+---+---+---+---+---+---+
        // Shift by 3:  | 0 | 0 | 0 | S | S | S | S | S | 0 | 0 | 0 | 0 |   Result == 0
        //              +---+---+---+---+---+---+---+---+---+---+---+---+
        //
        //              0               4               8              12
        //              +---+---+---+---+---+---+---+---+---+---+---+---+
        // Shift by 4:  | 0 | 0 | 0 | 0 | S | S | S | S | S | 0 | 0 | 0 |   Result != 0
        //              +---+---+---+---+---+---+---+---+---+---+---+---+
        //
        // TOTAL ZEROS PRODUCED: 4
        //
        //
        // CASE 3: Two zeros added in vector A
        // ------
        //
        //              -8              -4              0              ...             48
        //              +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
        // Vector B:    | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | S | S | S |  ...  | S | S | S |
        //              +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
        //
        // Vector A:                    X
        //
        //              0               4               8              12
        //              +---+---+---+---+---+---+---+---+---+---+---+---+
        // No shift:    | S | S | S | S | S | S | 0 | 0 | 0 | 0 | 0 | 0 |   Result == 0
        //              +---+---+---+---+---+---+---+---+---+---+---+---+
        //
        //              0               4               8              12
        //              +---+---+---+---+---+---+---+---+---+---+---+---+
        // Shift by 1:  | 0 | S | S | S | S | S | S | 0 | 0 | 0 | 0 | 0 |   Result == 0
        //              +---+---+---+---+---+---+---+---+---+---+---+---+
        //
        //              0               4               8              12
        //              +---+---+---+---+---+---+---+---+---+---+---+---+
        // Shift by 2:  | 0 | 0 | S | S | S | S | S | S | 0 | 0 | 0 | 0 |   Result == 0
        //              +---+---+---+---+---+---+---+---+---+---+---+---+
        //
        //              0               4               8              12
        //              +---+---+---+---+---+---+---+---+---+---+---+---+
        // Shift by 3:  | 0 | 0 | 0 | S | S | S | S | S | S | 0 | 0 | 0 |   Result != 0
        //              +---+---+---+---+---+---+---+---+---+---+---+---+
        //
        // TOTAL ZEROS PRODUCED: 3
        //
        //
        // CASE 4: One zero added in vector A
        // ------
        //
        //              -8              -4              0              ...             48
        //              +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
        // Vector B:    | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | S | S | S |  ...  | S | S | S |
        //              +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
        //
        // Vector A:                    X
        //
        //              0               4               8              12
        //              +---+---+---+---+---+---+---+---+---+---+---+---+
        // No shift:    | S | S | S | S | S | S | S | 0 | 0 | 0 | 0 | 0 |   Result == 0
        //              +---+---+---+---+---+---+---+---+---+---+---+---+
        //
        //              0               4               8              12
        //              +---+---+---+---+---+---+---+---+---+---+---+---+
        // Shift by 1:  | 0 | S | S | S | S | S | S | S | 0 | 0 | 0 | 0 |   Result == 0
        //              +---+---+---+---+---+---+---+---+---+---+---+---+
        //
        //              0               4               8              12
        //              +---+---+---+---+---+---+---+---+---+---+---+---+
        // Shift by 2:  | 0 | 0 | S | S | S | S | S | S | S | 0 | 0 | 0 |   Result != 0
        //              +---+---+---+---+---+---+---+---+---+---+---+---+
        //
        // TOTAL ZEROS PRODUCED: 2
        //

        myLinearCorrelation->vectorResultZerosBefore = myLinearCorrelation->vectorAZeroPadding + 1;

        // Look at what the padding does:
        //
        // CASE 1: No zero padding in vector B
        // ------
        //
        // Vector B:
        //
        //              0      ...      44              48              52
        //              +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
        //              | S |  ...  | S | S | S | S | S | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
        //              +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
        //
        // Vector A:                    X
        //
        //                              0               4               8
        //                              +---+---+---+---+---+---+---+---+
        //              No shift:       | S | S | S | S | 0 | 0 | 0 | 0 |   Result != 0
        //                              +---+---+---+---+---+---+---+---+
        //
        //                              0               4               8
        //                              +---+---+---+---+---+---+---+---+
        //              Shift by 1:     | 0 | S | S | S | S | 0 | 0 | 0 |   Result != 0
        //                              +---+---+---+---+---+---+---+---+
        //
        //                              0               4               8
        //                              +---+---+---+---+---+---+---+---+
        //              Shift by 2:     | 0 | 0 | S | S | S | S | 0 | 0 |   Result != 0
        //                              +---+---+---+---+---+---+---+---+
        //
        //                              0               4               8
        //                              +---+---+---+---+---+---+---+---+
        //              Shift by 3:     | 0 | 0 | 0 | S | S | S | S | 0 |   Result != 0
        //                              +---+---+---+---+---+---+---+---+
        //
        //
        // TOTAL ZEROS PRODUCED: 0
        //
        //
        // CASE 2: One zero added in vector B
        // ------
        //
        // Vector B:
        //
        //              0      ...      44              48              52
        //              +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
        //              | S |  ...  | S | S | S | S | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
        //              +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
        //
        // Vector A:                    X
        //
        //                              0               4               8
        //                              +---+---+---+---+---+---+---+---+---+---+---+---+
        //              No shift:       | S | S | S | S | S | 0 | 0 | 0 | 0 | 0 | 0 | 0 |   Result != 0
        //                              +---+---+---+---+---+---+---+---+---+---+---+---+
        //
        //                              0               4               8
        //                              +---+---+---+---+---+---+---+---+---+---+---+---+
        //              No shift:       | 0 | S | S | S | S | S | 0 | 0 | 0 | 0 | 0 | 0 |   Result != 0
        //                              +---+---+---+---+---+---+---+---+---+---+---+---+
        //
        //                              0               4               8
        //                              +---+---+---+---+---+---+---+---+---+---+---+---+
        //              No shift:       | 0 | 0 | S | S | S | S | S | 0 | 0 | 0 | 0 | 0 |   Result != 0
        //                              +---+---+---+---+---+---+---+---+---+---+---+---+
        //
        //                              0               4               8
        //                              +---+---+---+---+---+---+---+---+---+---+---+---+
        //              No shift:       | 0 | 0 | 0 | S | S | S | S | S | 0 | 0 | 0 | 0 |   Result == 0
        //                              +---+---+---+---+---+---+---+---+---+---+---+---+
        //
        //
        // TOTAL ZEROS PRODUCED: 1
        //
        //
        // CASE 3: Two zeros added in vector B
        // ------
        //
        // Vector B:
        //
        //              0      ...      44              48              52
        //              +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
        //              | S |  ...  | S | S | S | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
        //              +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
        //
        // Vector A:                    X
        //
        //                              0               4               8
        //                              +---+---+---+---+---+---+---+---+---+---+---+---+
        //              No shift:       | S | S | S | S | S | 0 | 0 | 0 | 0 | 0 | 0 | 0 |   Result != 0
        //                              +---+---+---+---+---+---+---+---+---+---+---+---+
        //
        //                              0               4               8
        //                              +---+---+---+---+---+---+---+---+---+---+---+---+
        //              No shift:       | 0 | S | S | S | S | S | 0 | 0 | 0 | 0 | 0 | 0 |   Result != 0
        //                              +---+---+---+---+---+---+---+---+---+---+---+---+
        //
        //                              0               4               8
        //                              +---+---+---+---+---+---+---+---+---+---+---+---+
        //              No shift:       | 0 | 0 | S | S | S | S | S | 0 | 0 | 0 | 0 | 0 |   Result == 0
        //                              +---+---+---+---+---+---+---+---+---+---+---+---+
        //
        //                              0               4               8
        //                              +---+---+---+---+---+---+---+---+---+---+---+---+
        //              No shift:       | 0 | 0 | 0 | S | S | S | S | S | 0 | 0 | 0 | 0 |   Result == 0
        //                              +---+---+---+---+---+---+---+---+---+---+---+---+
        //
        //
        // TOTAL ZEROS PRODUCED: 2
        //
        //
        // CASE 3: Three zeros added in vector B
        // ------
        //
        // Vector B:
        //
        //              0      ...      44              48              52
        //              +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
        //              | S |  ...  | S | S | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
        //              +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
        //
        // Vector A:                    X
        //
        //                              0               4               8
        //                              +---+---+---+---+---+---+---+---+---+---+---+---+
        //              No shift:       | S | S | S | S | S | S | S | 0 | 0 | 0 | 0 | 0 |   Result != 0
        //                              +---+---+---+---+---+---+---+---+---+---+---+---+
        //
        //                              0               4               8
        //                              +---+---+---+---+---+---+---+---+---+---+---+---+
        //              No shift:       | 0 | S | S | S | S | S | S | S | 0 | 0 | 0 | 0 |   Result == 0
        //                              +---+---+---+---+---+---+---+---+---+---+---+---+
        //
        //                              0               4               8
        //                              +---+---+---+---+---+---+---+---+---+---+---+---+
        //              No shift:       | 0 | 0 | S | S | S | S | S | S | S | 0 | 0 | 0 |   Result == 0
        //                              +---+---+---+---+---+---+---+---+---+---+---+---+
        //
        //                              0               4               8
        //                              +---+---+---+---+---+---+---+---+---+---+---+---+
        //              No shift:       | 0 | 0 | 0 | S | S | S | S | S | S | S | 0 | 0 |   Result == 0
        //                              +---+---+---+---+---+---+---+---+---+---+---+---+
        //
        //
        // TOTAL ZEROS PRODUCED: 3

        myLinearCorrelation->vectorResultZerosAfter = myLinearCorrelation->vectorBZeroPadding;


    // *************************************************************************
    // * STEP 3: Create arrays                                                 *
    // *************************************************************************

        // Create 4 rows for vector A because shifting is done by choosing
        // the correct row (memory needs to stay aligned)
        //
        //              0               4               8              12
        //              +---+---+---+---+---+---+---+---+---+---+---+---+
        //      Row 0:  | S | S | S | S | S | S | S | S | 0 | 0 | 0 | 0 |
        //              +---+---+---+---+---+---+---+---+---+---+---+---+
        //      Row 1:  | 0 | S | S | S | S | S | S | S | S | 0 | 0 | 0 |
        //              +---+---+---+---+---+---+---+---+---+---+---+---+
        //      Row 2:  | 0 | 0 | S | S | S | S | S | S | S | S | 0 | 0 |
        //              +---+---+---+---+---+---+---+---+---+---+---+---+
        //      Row 3:  | 0 | 0 | 0 | S | S | S | S | S | S | S | S | 0 |
        //              +---+---+---+---+---+---+---+---+---+---+---+---+

        myLinearCorrelation->vectorA = (float**) newTable2D(4, myLinearCorrelation->vectorASizeExt, sizeof(float));

        // Create a single row for vector B
        myLinearCorrelation->vectorB = (float*) newTable1D(myLinearCorrelation->vectorBSizeExt, sizeof(float));

        // Create a single row for vector Result
        myLinearCorrelation->vectorResult = (float*) newTable1D(myLinearCorrelation->vectorResultSizeExt, sizeof(float));

}

/*******************************************************************************
 * linearCorrelationTerminate                                                  *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myLinearCorrelation     The object to be terminated            *
 *                                                                             *
 * Outputs:     (none)                                                         *
 *                                                                             *
 * Description: This function terminates the objLinearCorrelation object       *
 *                                                                             *
 ******************************************************************************/

void linearCorrelationTerminate(struct objLinearCorrelation* myLinearCorrelation)
{

    // Free
    deleteTable2D((void**) myLinearCorrelation->vectorA);
    deleteTable1D((void*) myLinearCorrelation->vectorB);
    deleteTable1D((void*) myLinearCorrelation->vectorResult);

}

/*******************************************************************************
 * linearCorrelationLoadVectorA                                                *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myLinearCorrelation     The object to be used                  *
 *              vectorA                 The vector to be loaded                *
 *                                                                             *
 * Outputs:     (none)                                                         *
 *                                                                             *
 * Description: This function loads the first vector in the                    *
 *              objLinearCorrelation object                                    *
 *                                                                             *
 ******************************************************************************/

void linearCorrelationLoadVectorA(struct objLinearCorrelation* myLinearCorrelation, float* vectorA)
{

    unsigned int index;

    for (index = 0; index < myLinearCorrelation->vectorASizeExt; index++)
    {
        myLinearCorrelation->vectorA[0][index] = 0.0;
        myLinearCorrelation->vectorA[1][index] = 0.0;
        myLinearCorrelation->vectorA[2][index] = 0.0;
        myLinearCorrelation->vectorA[3][index] = 0.0;
    }

    for (index = 0; index < myLinearCorrelation->vectorASize; index++)
    {
        myLinearCorrelation->vectorA[0][index] = vectorA[index];
        myLinearCorrelation->vectorA[1][index+1] = vectorA[index];
        myLinearCorrelation->vectorA[2][index+2] = vectorA[index];
        myLinearCorrelation->vectorA[3][index+3] = vectorA[index];
    }

}

/*******************************************************************************
 * linearCorrelationLoadVectorB                                                *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myLinearCorrelation     The object to be used                  *
 *              vectorB                 The vector to be loaded                *
 *                                                                             *
 * Outputs:     (none)                                                         *
 *                                                                             *
 * Description: This function loads the second vector in the                   *
 *              objLinearCorrelation object                                    *
 *                                                                             *
 ******************************************************************************/

void linearCorrelationLoadVectorB(struct objLinearCorrelation* myLinearCorrelation, float* vectorB)
{

    unsigned int indexArray;
    unsigned int indexVector;

    for (indexArray = 0; indexArray < myLinearCorrelation->vectorBBlockPaddingBefore * 4; indexArray++)
    {
        myLinearCorrelation->vectorB[indexArray] = 0.0;
    }

    indexVector = 0;
    for (indexArray = (myLinearCorrelation->vectorBBlockPaddingBefore * 4); indexArray < (myLinearCorrelation->vectorBSize + (myLinearCorrelation->vectorBBlockPaddingBefore * 4)); indexArray++)
    {
        myLinearCorrelation->vectorB[indexArray] = vectorB[indexVector];
        indexVector++;
    }

    for (indexArray = (myLinearCorrelation->vectorBSize + (myLinearCorrelation->vectorBBlockPaddingBefore * 4)); indexArray < myLinearCorrelation->vectorBSizeExt; indexArray++)
    {
        myLinearCorrelation->vectorB[indexArray] = 0.0;
    }

}

/*******************************************************************************
 * linearCorrelationCompute                                                    *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myLinearCorrelation     The object to be used                  *
 *                                                                             *
 * Outputs:     (none)                                                         *
 *                                                                             *
 * Description: This function computes the correlation between both vectors.   *
 *                                                                             *
 ******************************************************************************/

void linearCorrelationCompute(struct objLinearCorrelation* myLinearCorrelation)
{

    unsigned int indexBlockAB;
    unsigned int indexBlockB;
    unsigned int indexElementB;
    unsigned int indexElementResult;


#ifdef USE_SIMD

    // SIMD registers
    __m128_mod regA, regB, regC, regD;
#else
    float sum=0;
    unsigned int indexElementAB = 0;
    float elementA,elementB;
    float elementResult;
#endif



    indexElementResult = 0;

    // Loop for all elements in vector B
    for (indexBlockB = 0; indexBlockB < (myLinearCorrelation->vectorBnBlocks - myLinearCorrelation->vectorAnBlocks + 1); indexBlockB++)
    {
        for (indexElementB = 0; indexElementB < 4; indexElementB++)
        {

#ifndef USE_SIMD

            // Loop for all elements in vector A
            sum = 0.0;

            for (indexBlockAB = 0; indexBlockAB < myLinearCorrelation->vectorAnBlocks; indexBlockAB++)
            {
                for (indexElementAB = 0; indexElementAB < 4; indexElementAB++)
                {
                    elementA = myLinearCorrelation->vectorA[3 - indexElementAB][3 + indexBlockAB * 4];
                    elementB = myLinearCorrelation->vectorB[indexBlockB * 4 + indexElementB + indexBlockAB * 4 + indexElementAB];
                    elementResult = elementA * elementB;

                    sum += elementResult;                  

                }
            }

            myLinearCorrelation->vectorResult[indexElementResult] = sum;

#else

            // Loop for all elements in vector A

            // Load the constant 0.0
            regD.m128_f32[0] = 0.0;
            regD.m128_f32[1] = 0.0;
            regD.m128_f32[2] = 0.0;
            regD.m128_f32[3] = 0.0;

            for (indexBlockAB = 0; indexBlockAB < myLinearCorrelation->vectorAnBlocks; indexBlockAB++)
            {

                    regA.m128 = _mm_load_ps(&myLinearCorrelation->vectorA[indexElementB][indexBlockAB*4]);
                    regB.m128 = _mm_load_ps(&myLinearCorrelation->vectorB[(indexBlockB + indexBlockAB)*4]);
                    regC.m128 = _mm_mul_ps(regA.m128,regB.m128);
                    regD.m128 = _mm_add_ps(regC.m128,regD.m128);
            }

            myLinearCorrelation->vectorResult[indexElementResult] = regD.m128_f32[0] +
                                                                    regD.m128_f32[1] +
                                                                    regD.m128_f32[2] +
                                                                    regD.m128_f32[3];

#endif

            indexElementResult++;

        }
    }

}

/*******************************************************************************
 * linearCorrelationStoreVectorResult                                          *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myLinearCorrelation     The object to be used                  *
 *                                                                             *
 * Outputs:     vectorResult            The full size resulting vector         *
 *                                                                             *
 * Description: This function stores the result vector in vectorResult. The    *
 *              size correponds to the size of a linear correlation (N+M-1)    *
 *                                                                             *
 ******************************************************************************/

void linearCorrelationStoreVectorResult(struct objLinearCorrelation* myLinearCorrelation, float* vectorResult)
{

    unsigned int indexArray;
    unsigned int indexVector;

    indexVector = 0;

    for (indexArray = myLinearCorrelation->vectorResultZerosBefore; indexArray < (myLinearCorrelation->vectorResultZerosBefore + myLinearCorrelation->vectorResultSize); indexArray++)
    {
        vectorResult[indexVector] = myLinearCorrelation->vectorResult[indexArray];
        indexVector++;
    }

}

/*******************************************************************************
 * linearCorrelationStoreVectorResultCropped                                   *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myLinearCorrelation     The object to be used                  *
 *                                                                             *
 * Outputs:     vectorResult            The resized resulting vector           *
 *                                                                             *
 * Description: This function stores the result vector in vectorResult. The    *
 *              size correponds to the size of vectorA (result is cropped)     *
 *                                                                             *
 ******************************************************************************/

void linearCorrelationStoreVectorResultCropped(struct objLinearCorrelation* myLinearCorrelation, float* vectorResultCropped)
{

    unsigned int indexArray;
    unsigned int indexStart;
    unsigned int indexVector;

    indexStart = myLinearCorrelation->vectorASize/2;
    indexVector = 0;

    for (indexArray = (myLinearCorrelation->vectorResultZerosBefore + indexStart); indexArray < (myLinearCorrelation->vectorResultZerosBefore + indexStart + myLinearCorrelation->vectorBSize); indexArray++)
    {
        vectorResultCropped[indexVector] = myLinearCorrelation->vectorResult[indexArray];
        indexVector++;
    }


}
