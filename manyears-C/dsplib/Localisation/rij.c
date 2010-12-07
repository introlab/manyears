/*******************************************************************************
 * ManyEars: Rij - Source code                                                 *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Author: François Grondin                                                    *
 * Original Code: Jean-Marc Valin                                              *
 * Modified Code: Simon Brière                                                 *
 * Version: 1.2.0                                                              *
 * Date: November 10th, 2010                                                   *
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

#include "Localisation/rij.h"

/*******************************************************************************
 * rijInit                                                                     *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myRij           Object to be initialized                       *
 *              myParameters    Parameters                                     *
 *              myMicrophones   Object to get configuration of the microphones *
 *              myDelays        Object to get delays                           *
 *              frameSize       Parameter: size of a frame                     *
 *              filterRange     Parameter: range to get maximum energy         *
 *              resetRange      Parameter: range to reset energy               *
 *                                                                             *
 * Outputs:     myRij           Initialized object                             *
 *                                                                             *
 * Description: This function initializes the rij object and allocates         *
 *              memory that will be used later.                                *
 *                                                                             *
 ******************************************************************************/

void rijInit(struct objRij* myRij, struct ParametersStruct* myParameters, struct objMicrophones* myMicrophones, struct objDelays* myDelays, unsigned int frameSize, unsigned int filterRange, unsigned int resetRange)
{

    /***************************************************************************
    * Step 1: Load parameters                                                  *
    ***************************************************************************/

    myRij->RIJ_FRAMESIZE = frameSize;
    myRij->RIJ_FILTERRANGE = (signed int) filterRange;
    myRij->RIJ_RESETRANGE = (signed int) resetRange;

    /***************************************************************************
    * Step 2: Initialize delays range                                          *
    ***************************************************************************/

    myRij->delayMin = delaysGetMin(myDelays);
    myRij->delayMax = delaysGetMax(myDelays);

    /***************************************************************************
    * Step 3: Initialize modules                                               *
    ***************************************************************************/

    // +-----------------------------------------------------------------------+
    // | Step A: FFT                                                           |
    // +-----------------------------------------------------------------------+

    myRij->myFFT = (struct objFFT*) malloc(sizeof(struct objFFT));
    fftInit(myRij->myFFT, myParameters, myRij->RIJ_FRAMESIZE);

    // +-----------------------------------------------------------------------+
    // | Step B: Microphones                                                   |
    // +-----------------------------------------------------------------------+

    myRij->myMicrophones = (struct objMicrophones*) malloc(sizeof(struct objMicrophones));
    microphonesClone(myMicrophones, myRij->myMicrophones);

    /***************************************************************************
    * Step 4: Create arrays                                                    *
    ***************************************************************************/

    // +-----------------------------------------------------------------------+
    // | Step A: Microphone weighted spectrum                                  |
    // +-----------------------------------------------------------------------+

    myRij->freqReal = (float**) newTable2D(myRij->myMicrophones->nMics, myRij->RIJ_FRAMESIZE, sizeof(float));
    myRij->freqImag = (float**) newTable2D(myRij->myMicrophones->nMics, myRij->RIJ_FRAMESIZE, sizeof(float));

    // +-----------------------------------------------------------------------+
    // | Step B: Cross-correlation for each pair of microphones                |
    // +-----------------------------------------------------------------------+

    myRij->crossCorr = (float**) newTable2D(myRij->myMicrophones->nPairs, myRij->RIJ_FRAMESIZE, sizeof(float));
    myRij->crossCorrFiltered = (float**) newTable2D(myRij->myMicrophones->nPairs, (myRij->delayMax - myRij->delayMin + 1), sizeof(float));

    // +-----------------------------------------------------------------------+
    // | Step C: Working arrays                                                |
    // +-----------------------------------------------------------------------+

    myRij->workingArray1Real = (float*) newTable1D(myRij->RIJ_FRAMESIZE, sizeof(float));
    myRij->workingArray1Imag = (float*) newTable1D(myRij->RIJ_FRAMESIZE, sizeof(float));
    myRij->workingArray2Real = (float*) newTable1D(myRij->RIJ_FRAMESIZE, sizeof(float));
    myRij->workingArray2Imag = (float*) newTable1D(myRij->RIJ_FRAMESIZE, sizeof(float));

}

/*******************************************************************************
 * rijTerminate                                                                *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myRij           The Rij object to be terminated                *
 *                                                                             *
 * Outputs:     (none)                                                         *
 *                                                                             *
 * Description: This function frees the dynamic memory that was allocated in   *
 *              the initialization of this object                              *
 *                                                                             *
 ******************************************************************************/

void rijTerminate(struct objRij* myRij)
{

    // Free

    deleteTable2D((void**) myRij->freqReal);
    deleteTable2D((void**) myRij->freqImag);
    deleteTable2D((void**) myRij->crossCorr);
    deleteTable2D((void**) myRij->crossCorrFiltered);
    deleteTable1D((void*) myRij->workingArray1Real);
    deleteTable1D((void*) myRij->workingArray1Imag);
    deleteTable1D((void*) myRij->workingArray2Real);
    deleteTable1D((void*) myRij->workingArray2Imag);

    fftTerminate(myRij->myFFT);
    free((void*) myRij->myFFT);

    microphonesTerminate(myRij->myMicrophones);
    free((void*) myRij->myMicrophones);

}

/*******************************************************************************
 * rijLoadFrame                                                                *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myRij           The Rij object to be used                      *
 *              indexMic        Index of the microphone to be used             *
 *              frameReal       The pointer to the real part of the frame      *
 *              frameImag       The pointer to the imaginary part of the frame *
 *                                                                             *
 * Outputs:     myRij           The Rij object loaded with the new frame       *
 *                                                                             *
 * Description: This function loads the frame associated with the microphone   *
 *              which index is given by indexMic.                              *
 *                                                                             *
 ******************************************************************************/

void rijLoadFrame(struct objRij* myRij, unsigned int indexMic, float* frameReal, float* frameImag)
{

    unsigned int indexSample;

    for (indexSample = 0; indexSample < myRij->RIJ_FRAMESIZE; indexSample++)
    {
        myRij->freqReal[indexMic][indexSample] = frameReal[indexSample];
        myRij->freqImag[indexMic][indexSample] = frameImag[indexSample];
    }

}

/*******************************************************************************
 * rijProcess                                                                  *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myRij           The Rij object to be used                      *
 *                                                                             *
 * Outputs:     myRij           The Rij object with updated cross-correlations *
 *                                                                             *
 * Description: This function updates the cross-correlation for each pair of   *
 *              microphones.                                                   *
 *                                                                             *
 ******************************************************************************/

void rijProcess(struct objRij* myRij)
{

    unsigned int indexPair;

    unsigned int indexMic1;
    unsigned int indexMic2;
    unsigned int indexMic1Prev;
    unsigned int indexMic2Prev;

    unsigned int k;

    signed int indexDelay;
    signed int indexDelay2;

    signed int indexDelayCorrected;

    unsigned int indexCrossCorrFiltered;

    float max;

    /***************************************************************************
    * Step 1: Multiply the cross terms                                         *
    ***************************************************************************/

    for (indexPair = 0; indexPair < myRij->myMicrophones->nPairs; indexPair++)
    {

        // If there is a pair of frames to process
        if (((indexPair+1)/2.0) == floor((indexPair+1)/2.0))
        {

            indexMic1Prev = microphonesGetMic(myRij->myMicrophones,(indexPair-1),0);
            indexMic2Prev = microphonesGetMic(myRij->myMicrophones,(indexPair-1),1);
            indexMic1 = microphonesGetMic(myRij->myMicrophones,(indexPair),0);
            indexMic2 = microphonesGetMic(myRij->myMicrophones,(indexPair),1);

            // Loop for each element in the array
            for (k = 0; k < myRij->RIJ_FRAMESIZE; k++)
            {

                // Multiply each cross correlation and conjugate the imaginary part
                // (zeta_i[k] * X_i[k] / |X_i[k]|) * (zeta_j[k] * X_j[k]* / |X_j[k]|):
                // -> Re{(zeta_i[k] * X_i[k] / |X_i[k]|)} * Re{(zeta_j[k] * X_j[k] / |X_j[k]|)} + Im{(zeta_i[k] * X_i[k] / |X_i[k]|)} * Im{(zeta_j[k] * X_j[k] / |X_j[k]|)}
                // -> Re{(zeta_i[k] * X_i[k] / |X_i[k]|)} * Im{(zeta_j[k] * X_j[k] / |X_j[k]|)} - Im{(zeta_i[k] * X_i[k] / |X_i[k]|)} * Re{(zeta_j[k] * X_j[k] / |X_j[k]|)}
                //
                // A scaling factor N (the frame size) is used to match the result with the equation
                // R(w)_ij(tau) = sum_k=0_L-1{ (zeta_i(k) * X_i(k) * zeta_j(k) * X_j(k)* / (|X_i(k)| * |X_j(k)|)) * * exp( j * 2 * pi * k * tau / L)

                myRij->workingArray1Real[k] = myRij->RIJ_FRAMESIZE * (myRij->freqReal[indexMic1Prev][k] * myRij->freqReal[indexMic2Prev][k] + myRij->freqImag[indexMic1Prev][k] * myRij->freqImag[indexMic2Prev][k]);
                myRij->workingArray1Imag[k] = myRij->RIJ_FRAMESIZE * (myRij->freqImag[indexMic1Prev][k] * myRij->freqReal[indexMic2Prev][k] - myRij->freqReal[indexMic1Prev][k] * myRij->freqImag[indexMic2Prev][k]);
                myRij->workingArray2Real[k] = myRij->RIJ_FRAMESIZE * (myRij->freqReal[indexMic1][k] * myRij->freqReal[indexMic2][k] + myRij->freqImag[indexMic1][k] * myRij->freqImag[indexMic2][k]);
                myRij->workingArray2Imag[k] = myRij->RIJ_FRAMESIZE * (myRij->freqImag[indexMic1][k] * myRij->freqReal[indexMic2][k] - myRij->freqReal[indexMic1][k] * myRij->freqImag[indexMic2][k]);

            }

            ifftComputeTwice(myRij->myFFT, myRij->workingArray1Real, myRij->workingArray1Imag, myRij->workingArray2Real, myRij->workingArray2Imag, myRij->crossCorr[indexPair-1], myRij->crossCorr[indexPair]);

        }

    }

    // Check if there is an IFFT left to process
    if (floorf(myRij->myMicrophones->nPairs / 2) != (myRij->myMicrophones->nPairs / 2))
    {

        indexMic1 = microphonesGetMic(myRij->myMicrophones,(myRij->myMicrophones->nPairs - 1),0);
        indexMic2 = microphonesGetMic(myRij->myMicrophones,(myRij->myMicrophones->nPairs - 1),1);

        // Loop for each element in the array
        for (k = 0; k < myRij->RIJ_FRAMESIZE; k++)
        {

            // Multiply each cross correlation and conjugate the imaginary part
            // (zeta_i[k] * X_i[k] / |X_i[k]|) * (zeta_j[k] * X_j[k]* / |X_j[k]|):
            // -> Re{(zeta_i[k] * X_i[k] / |X_i[k]|)} * Re{(zeta_j[k] * X_j[k] / |X_j[k]|)} - Im{(zeta_i[k] * X_i[k] / |X_i[k]|)} * Im{(zeta_j[k] * X_j[k] / |X_j[k]|)}
            // -> Re{(zeta_i[k] * X_i[k] / |X_i[k]|)} * Im{(zeta_j[k] * X_j[k] / |X_j[k]|)} + Im{(zeta_i[k] * X_i[k] / |X_i[k]|)} * Re{(zeta_j[k] * X_j[k] / |X_j[k]|)}

            myRij->workingArray1Real[k] = myRij->RIJ_FRAMESIZE * (myRij->freqReal[indexMic1][k] * myRij->freqReal[indexMic2][k] + myRij->freqImag[indexMic1][k] * myRij->freqImag[indexMic2][k]);
            myRij->workingArray1Imag[k] = myRij->RIJ_FRAMESIZE * (myRij->freqReal[indexMic1][k] * myRij->freqImag[indexMic2][k] + myRij->freqImag[indexMic1][k] * myRij->freqReal[indexMic2][k]);

        }


        // Compute the IFFT
        ifftComputeOnce(myRij->myFFT, myRij->workingArray1Real, myRij->workingArray1Imag, myRij->crossCorr[myRij->myMicrophones->nPairs - 1]);

    }

    /***************************************************************************
    * Step 2: Filter the cross-correlation terms                               *
    ***************************************************************************/

    for (indexPair = 0; indexPair < myRij->myMicrophones->nPairs; indexPair++)
    {

        indexCrossCorrFiltered = 0;

        for (indexDelay = myRij->delayMin; indexDelay <= myRij->delayMax; indexDelay++)
        {

            indexDelayCorrected = (indexDelay - ((signed int) myRij->RIJ_FILTERRANGE));

            if (indexDelayCorrected < 0)
            {
                indexDelayCorrected += myRij->RIJ_FRAMESIZE;
            }

            max = myRij->crossCorr[indexPair][indexDelayCorrected];

            for (indexDelay2 = (indexDelay - myRij->RIJ_FILTERRANGE); indexDelay2 <= (indexDelay + myRij->RIJ_FILTERRANGE); indexDelay2++)
            {

                indexDelayCorrected = indexDelay2;

                if (indexDelayCorrected < 0)
                {
                    indexDelayCorrected += myRij->RIJ_FRAMESIZE;
                }

                if (myRij->crossCorr[indexPair][indexDelayCorrected] > max)
                {
                    max = myRij->crossCorr[indexPair][indexDelayCorrected];
                }

            }

            myRij->crossCorrFiltered[indexPair][indexCrossCorrFiltered] = max;
            indexCrossCorrFiltered++;

        }

    }

}

/*******************************************************************************
 * rijRemoveSource                                                             *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myRij           The Rij object to be used                      *
 *              myDelays        The delays object to be used                   *
 *              indexPoint      The index of the point on the sphere to be     *
 *                              removed                                        *
 *                                                                             *
 * Outputs:     myRij           The Rij object with updated cross-correlations *
 *                                                                             *
 * Description: This function updates the cross-correlation for each pair of   *
 *              microphones in order to remove the contribution of a given     *
 *              point with index is indexPoint.                                *
 *                                                                             *
 ******************************************************************************/

void rijRemoveSource(struct objRij* myRij, struct objDelays* myDelays, unsigned int indexPoint)
{

    unsigned int indexPair;

    signed int delay;
    signed int delayMin;
    signed int delayMax;

    /***************************************************************************
    * Step 1: Reset the delays                                                 *
    ***************************************************************************/

    for (indexPair = 0; indexPair < myRij->myMicrophones->nPairs; indexPair++)
    {

        // +-------------------------------------------------------------------+
        // | Step A: Get delay for this pair and point                         |
        // +-------------------------------------------------------------------+

        delay = myDelays->lookup[indexPoint][indexPair];

        // +-------------------------------------------------------------------+
        // | Step B: Get the range that needs to be reset                      |
        // +-------------------------------------------------------------------+

        delayMin = delay - myRij->RIJ_RESETRANGE;

        if (delayMin < myRij->delayMin)
        {
            delayMin = myRij->delayMin;
        }

        delayMax = delay + myRij->RIJ_RESETRANGE;
        if (delayMax > myRij->delayMax)
        {
            delayMax = myRij->delayMax;
        }

        // +-------------------------------------------------------------------+
        // | Step C: Reset                                                     |
        // +-------------------------------------------------------------------+

        for (delay = delayMin; delay <= delayMax; delay++)
        {
            myRij->crossCorrFiltered[indexPair][delay - myRij->delayMin] = 0.0;
        }

    }

}

/*******************************************************************************
 * rijGetEnergyFromMics                                                        *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myRij           The Rij object to be used                      *
 *              iMic            The first microphone in the pair               *
 *              jMic            The second microphone in the pair              *
 *              delay           The delay observed                             *
 *                                                                             *
 * Outputs:     (float)         The energy for this pair of microphones        *
 *                                                                             *
 * Description: This function returns the energy associated with this pair of  *
 *              microphones and the associated delay                           *
 *                                                                             *
 ******************************************************************************/

INLINE_PREFIX float rijGetEnergyFromMics(struct objRij* myRij, unsigned int iMic, unsigned int jMic, signed int delay)
{

    unsigned int indexPair;

    indexPair = microphonesGetPair(myRij->myMicrophones, iMic, jMic);

    return myRij->crossCorrFiltered[indexPair][delay - myRij->delayMin];

}

/*******************************************************************************
 * rijGetEnergyFromPair                                                        *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myRij           The Rij object to be used                      *
 *              indexPair       The index of the pair of microphones           *
 *              delay           The delay observed                             *
 *                                                                             *
 * Outputs:     (float)         The energy for this pair of microphones        *
 *                                                                             *
 * Description: This function returns the energy associated with this pair of  *
 *              microphones and the associated delay                           *
 *                                                                             *
 ******************************************************************************/

INLINE_PREFIX float rijGetEnergyFromPair(struct objRij* myRij, unsigned int indexPair, signed int delay)
{

    return myRij->crossCorrFiltered[indexPair][delay - myRij->delayMin];

}
