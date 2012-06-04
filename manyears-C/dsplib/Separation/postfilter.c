/*******************************************************************************
 * ManyEars: PostFilter - Source Code                                          *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Author: François Grondin                                                    *
 * Original Code: Jean-Marc Valin                                              *
 * Modified Code: Simon Brière                                                 *
 * Version: 1.1.0                                                              *
 * Date: September 16th, 2010                                                  *
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

#include "Separation/postfilter.h"

/*******************************************************************************
 * postfilterInit                                                              *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:  myPostfilter    The postfilter object used to be initialized       *
 *          myParameters    The parameters used for initialization             *
 *                                                                             *
 * Outputs: myPostfilter    The initialized postfilter object                  *
 *                                                                             *
 * Description: This function initializes the postfilter object.               *
 *                                                                             *
 ******************************************************************************/

void postfilterInit(struct objPostfilter* myPostfilter, struct ParametersStruct* myParameters)
{

    unsigned int indexSource;
    unsigned int k;

    float* tmpArray;

    // *************************************************************************
    // * STEP 1: Load parameters                                               *
    // *************************************************************************

    myPostfilter->POSTFILTER_ALPHAS = myParameters->P_POSTFILTER_ALPHAS;
    myPostfilter->POSTFILTER_ETA = myParameters->P_POSTFILTER_ETA;
    myPostfilter->POSTFILTER_GAMMA = myParameters->P_POSTFILTER_GAMMA;
    myPostfilter->POSTFILTER_DELTA = myParameters->P_POSTFILTER_DELTA;
    myPostfilter->POSTFILTER_NBSOURCES = myParameters->P_GEN_DYNSOURCES;
    myPostfilter->POSTFILTER_NFRAMES = GLOBAL_FRAMESIZE;
    myPostfilter->POSTFILTER_HALFNFRAMES = myPostfilter->POSTFILTER_NFRAMES / 2;
    myPostfilter->POSTFILTER_HALFNFRAMESPLUSONE = myPostfilter->POSTFILTER_HALFNFRAMES + 1;
    myPostfilter->POSTFILTER_TETA_LOCAL = myParameters->P_POSTFILTER_TETA_LOCAL;
    myPostfilter->POSTFILTER_TETA_GLOBAL = myParameters->P_POSTFILTER_TETA_GLOBAL;
    myPostfilter->POSTFILTER_TETA_FRAME = myParameters->P_POSTFILTER_TETA_FRAME;
    myPostfilter->POSTFILTER_ALPHAZETA = myParameters->P_POSTFILTER_ALPHAZETA;
    myPostfilter->POSTFILTER_MAXQ = myParameters->P_POSTFILTER_MAXQ;
    myPostfilter->POSTFILTER_GMIN = myParameters->P_POSTFILTER_GMIN;
    myPostfilter->POSTFILTER_LOCALWINDOWSIZE = myParameters->P_POSTFILTER_LOCALWINDOWSIZE;
    myPostfilter->POSTFILTER_GLOBALWINDOWSIZE = myParameters->P_POSTFILTER_GLOBALWINDOWSIZE;
    myPostfilter->POSTFILTER_FRAMEWINDOWSIZE = myPostfilter->POSTFILTER_NFRAMES;
    myPostfilter->POSTFILTER_ALPHAPMIN = myParameters->P_POSTFILTER_ALPHAPMIN;

    // *************************************************************************
    // * STEP 2: Create arrays                                                 *
    // *************************************************************************

    // +-----------------------------------------------------------------------+
    // | Step A: Create MCRA objects                                           |
    // +-----------------------------------------------------------------------+

    myPostfilter->mcra = (struct objMcra*) newTable1D(myPostfilter->POSTFILTER_NBSOURCES, sizeof(struct objMcra));

    // +-----------------------------------------------------------------------+
    // | Step B: Create sources arrays                                         |
    // +-----------------------------------------------------------------------+

    idListInit(&myPostfilter->sourcesIDNow, myPostfilter->POSTFILTER_NBSOURCES);
    idListInit(&myPostfilter->sourcesIDAdded, myPostfilter->POSTFILTER_NBSOURCES);
    idListInit(&myPostfilter->sourcesIDDeleted, myPostfilter->POSTFILTER_NBSOURCES);

    myPostfilter->sourcesID = (ID_TYPE*) newTable1D(myPostfilter->POSTFILTER_NBSOURCES, sizeof(ID_TYPE));

    // +-----------------------------------------------------------------------+
    // | Step C: Create input and output arrays                                |
    // +-----------------------------------------------------------------------+

    myPostfilter->YReal = (float**) newTable2D(myPostfilter->POSTFILTER_NBSOURCES, myPostfilter->POSTFILTER_NFRAMES, sizeof(float));
    myPostfilter->YImag = (float**) newTable2D(myPostfilter->POSTFILTER_NBSOURCES, myPostfilter->POSTFILTER_NFRAMES, sizeof(float));
    myPostfilter->YPower = (float**) newTable2D(myPostfilter->POSTFILTER_NBSOURCES, myPostfilter->POSTFILTER_NFRAMES, sizeof(float));
    myPostfilter->SReal = (float**) newTable2D(myPostfilter->POSTFILTER_NBSOURCES, myPostfilter->POSTFILTER_NFRAMES, sizeof(float));
    myPostfilter->SImag = (float**) newTable2D(myPostfilter->POSTFILTER_NBSOURCES, myPostfilter->POSTFILTER_NFRAMES, sizeof(float));
    myPostfilter->SPower = (float**) newTable2D(myPostfilter->POSTFILTER_NBSOURCES, myPostfilter->POSTFILTER_NFRAMES, sizeof(float));

    // +-----------------------------------------------------------------------+
    // | Step D: Create noise arrays                                           |
    // +-----------------------------------------------------------------------+

        // +-------------------------------------------------------------------+
        // | Step i: Static noise                                              |
        // +-------------------------------------------------------------------+

        myPostfilter->lambdaStat = (float**) newTable2D(myPostfilter->POSTFILTER_NBSOURCES, myPostfilter->POSTFILTER_NFRAMES, sizeof(float));

        // +-------------------------------------------------------------------+
        // | Step ii: Leakage noise                                            |
        // +-------------------------------------------------------------------+

        myPostfilter->Z = (float**) newTable2D(myPostfilter->POSTFILTER_NBSOURCES, myPostfilter->POSTFILTER_NFRAMES, sizeof(float));
        myPostfilter->lambdaLeak = (float**) newTable2D(myPostfilter->POSTFILTER_NBSOURCES, myPostfilter->POSTFILTER_NFRAMES, sizeof(float));

        // +-------------------------------------------------------------------+
        // | Step iii: Reverberation noise                                     |
        // +-------------------------------------------------------------------+

        myPostfilter->lambdaRev = (float**) newTable2D(myPostfilter->POSTFILTER_NBSOURCES, myPostfilter->POSTFILTER_NFRAMES, sizeof(float));

        // +-------------------------------------------------------------------+
        // | Step iv: Total noise                                              |
        // +-------------------------------------------------------------------+

        myPostfilter->lambda = (float**) newTable2D(myPostfilter->POSTFILTER_NBSOURCES, myPostfilter->POSTFILTER_NFRAMES, sizeof(float));

    // +-----------------------------------------------------------------------+
    // | Step E: Create speech presence arrays                                 |
    // +-----------------------------------------------------------------------+

        myPostfilter->gamma = (float**) newTable2D(myPostfilter->POSTFILTER_NBSOURCES, myPostfilter->POSTFILTER_NFRAMES, sizeof(float));
        myPostfilter->xi = (float**) newTable2D(myPostfilter->POSTFILTER_NBSOURCES, myPostfilter->POSTFILTER_NFRAMES, sizeof(float));
        myPostfilter->alphaP = (float**) newTable2D(myPostfilter->POSTFILTER_NBSOURCES, myPostfilter->POSTFILTER_NFRAMES, sizeof(float));
        myPostfilter->v = (float**) newTable2D(myPostfilter->POSTFILTER_NBSOURCES, myPostfilter->POSTFILTER_NFRAMES, sizeof(float));
        myPostfilter->GH1 = (float**) newTable2D(myPostfilter->POSTFILTER_NBSOURCES, myPostfilter->POSTFILTER_NFRAMES, sizeof(float));

    // +-----------------------------------------------------------------------+
    // | Step F: Create speech presence gain arrays                            |
    // +-----------------------------------------------------------------------+

        // +-------------------------------------------------------------------+
        // | Step i: Result after windowing                                    |
        // +-------------------------------------------------------------------+

            // +---------------------------------------------------------------+
            // | Step a: Local window                                          |
            // +---------------------------------------------------------------+

                myPostfilter->localResultCropped = (float*) newTable1D(myPostfilter->POSTFILTER_NFRAMES, sizeof(float));

            // +---------------------------------------------------------------+
            // | Step b: Global window                                         |
            // +---------------------------------------------------------------+

                myPostfilter->globalResultCropped = (float*) newTable1D(myPostfilter->POSTFILTER_NFRAMES, sizeof(float));

            // +---------------------------------------------------------------+
            // | Step c: Frame window                                          |
            // +---------------------------------------------------------------+

                myPostfilter->frameResultCropped = (float*) newTable1D(myPostfilter->POSTFILTER_NFRAMES, sizeof(float));

        // +-------------------------------------------------------------------+
        // | Step ii: A priori SNR                                             |
        // +-------------------------------------------------------------------+

            // +---------------------------------------------------------------+
            // | Step a: Local window                                          |
            // +---------------------------------------------------------------+

                myPostfilter->localZeta = (float**) newTable2D(myPostfilter->POSTFILTER_NBSOURCES, myPostfilter->POSTFILTER_NFRAMES, sizeof(float));

            // +---------------------------------------------------------------+
            // | Step b: Global window                                         |
            // +---------------------------------------------------------------+

                myPostfilter->globalZeta = (float**) newTable2D(myPostfilter->POSTFILTER_NBSOURCES, myPostfilter->POSTFILTER_NFRAMES, sizeof(float));

            // +---------------------------------------------------------------+
            // | Step c: Frame window                                          |
            // +---------------------------------------------------------------+

                myPostfilter->frameZeta = (float**) newTable2D(myPostfilter->POSTFILTER_NBSOURCES, myPostfilter->POSTFILTER_NFRAMES, sizeof(float));

        // +-------------------------------------------------------------------+
        // | Step iii: Speech presence probabilities                           |
        // +-------------------------------------------------------------------+

            // +---------------------------------------------------------------+
            // | Step a: Local window                                          |
            // +---------------------------------------------------------------+

                myPostfilter->localP = (float**) newTable2D(myPostfilter->POSTFILTER_NBSOURCES, myPostfilter->POSTFILTER_NFRAMES, sizeof(float));

            // +---------------------------------------------------------------+
            // | Step b: Global window                                         |
            // +---------------------------------------------------------------+

                myPostfilter->globalP = (float**) newTable2D(myPostfilter->POSTFILTER_NBSOURCES, myPostfilter->POSTFILTER_NFRAMES, sizeof(float));

            // +---------------------------------------------------------------+
            // | Step c: Frame window                                          |
            // +---------------------------------------------------------------+

                myPostfilter->frameP = (float**) newTable2D(myPostfilter->POSTFILTER_NBSOURCES, myPostfilter->POSTFILTER_NFRAMES, sizeof(float));

            // +---------------------------------------------------------------+
            // | Step d: A priori probability of speech presence               |
            // +---------------------------------------------------------------+

                myPostfilter->p = (float**) newTable2D(myPostfilter->POSTFILTER_NBSOURCES, myPostfilter->POSTFILTER_NFRAMES, sizeof(float));

            // +---------------------------------------------------------------+
            // | Step e: Probability of speech presence                        |
            // +---------------------------------------------------------------+

            myPostfilter->q = (float**) newTable2D(myPostfilter->POSTFILTER_NBSOURCES, myPostfilter->POSTFILTER_NFRAMES, sizeof(float));

        // +-------------------------------------------------------------------+
        // | Step iv: Gain                                                     |
        // +-------------------------------------------------------------------+

        // Gain
        myPostfilter->G = (float**) newTable2D(myPostfilter->POSTFILTER_NBSOURCES, myPostfilter->POSTFILTER_NFRAMES, sizeof(float));

    // *************************************************************************
    // * STEP 3: Initialize values                                             *
    // *************************************************************************

    // +-----------------------------------------------------------------------+
    // | Step A: Initialize sources                                            |
    // +-----------------------------------------------------------------------+

        for (indexSource = 0; indexSource < myPostfilter->POSTFILTER_NBSOURCES; indexSource++)
        {
            myPostfilter->sourcesID[indexSource] = ID_NOSOURCE;
        }

        idListInit(&myPostfilter->sourcesIDNow, myPostfilter->POSTFILTER_NBSOURCES);
        idListInit(&myPostfilter->sourcesIDAdded, myPostfilter->POSTFILTER_NBSOURCES);
        idListInit(&myPostfilter->sourcesIDDeleted, myPostfilter->POSTFILTER_NBSOURCES);

    // +-----------------------------------------------------------------------+
    // | Step B: Initialize MCRA objects                                       |
    // +-----------------------------------------------------------------------+

        for (indexSource = 0; indexSource < myPostfilter->POSTFILTER_NBSOURCES; indexSource++)
        {
            mcraInit(&myPostfilter->mcra[indexSource], myParameters, myPostfilter->POSTFILTER_NFRAMES);
        }

    // +-----------------------------------------------------------------------+
    // | Step C: Initialize transcendental objects                             |
    // +-----------------------------------------------------------------------+

        transcendentalInit(&myPostfilter->transcendental);

    // +-----------------------------------------------------------------------+
    // | Step D: Initialize windows                                            |
    // +-----------------------------------------------------------------------+

        // +-------------------------------------------------------------------+
        // | Step i: Local window                                              |
        // +-------------------------------------------------------------------+

        linearCorrelationInit(&myPostfilter->localWindow, myPostfilter->POSTFILTER_LOCALWINDOWSIZE, myPostfilter->POSTFILTER_NFRAMES);
        tmpArray = (float*) newTable1D(myPostfilter->POSTFILTER_LOCALWINDOWSIZE, sizeof(float));
        generateHanningWindow(tmpArray, myPostfilter->POSTFILTER_LOCALWINDOWSIZE);
        linearCorrelationLoadVectorA(&myPostfilter->localWindow, tmpArray);
        deleteTable1D((void*) tmpArray);

        // +-------------------------------------------------------------------+
        // | Step ii: Global window                                            |
        // +-------------------------------------------------------------------+

        linearCorrelationInit(&myPostfilter->globalWindow, myPostfilter->POSTFILTER_GLOBALWINDOWSIZE, myPostfilter->POSTFILTER_NFRAMES);
        tmpArray = (float*) newTable1D(myPostfilter->POSTFILTER_GLOBALWINDOWSIZE, sizeof(float));
        generateHanningWindow(tmpArray, myPostfilter->POSTFILTER_GLOBALWINDOWSIZE);
        linearCorrelationLoadVectorA(&myPostfilter->globalWindow, tmpArray);
        deleteTable1D((void*) tmpArray);

        // +-------------------------------------------------------------------+
        // | Step iii: Frame window                                            |
        // +-------------------------------------------------------------------+

        linearCorrelationInit(&myPostfilter->frameWindow, myPostfilter->POSTFILTER_FRAMEWINDOWSIZE, myPostfilter->POSTFILTER_NFRAMES);
        tmpArray = (float*) newTable1D(myPostfilter->POSTFILTER_FRAMEWINDOWSIZE, sizeof(float));
        generateHanningWindow(tmpArray, myPostfilter->POSTFILTER_FRAMEWINDOWSIZE);
        linearCorrelationLoadVectorA(&myPostfilter->frameWindow, tmpArray);
        deleteTable1D((void*) tmpArray);

    // +-----------------------------------------------------------------------+
    // | Step E: Set all other arrays to zero                                  |
    // +-----------------------------------------------------------------------+

        for (indexSource = 0; indexSource < myPostfilter->POSTFILTER_NBSOURCES; indexSource++)
        {
            for (k = 0; k < myPostfilter->POSTFILTER_NFRAMES; k++)
            {
                myPostfilter->YReal[indexSource][k] = 0.0f;
                myPostfilter->YImag[indexSource][k] = 0.0f;
                myPostfilter->YPower[indexSource][k] = 0.0f;
                myPostfilter->SReal[indexSource][k] = 0.0f;
                myPostfilter->SImag[indexSource][k] = 0.0f;
                myPostfilter->SPower[indexSource][k] = 0.0f;
                myPostfilter->lambdaStat[indexSource][k] = 0.0f;
                myPostfilter->Z[indexSource][k] = 0.0f;
                myPostfilter->lambdaLeak[indexSource][k] = 0.0f;
                myPostfilter->lambdaRev[indexSource][k] = 0.0f;
                myPostfilter->lambda[indexSource][k] = 0.0f;
                myPostfilter->gamma[indexSource][k] = 0.0f;
                myPostfilter->xi[indexSource][k] = 0.0f;
                myPostfilter->alphaP[indexSource][k] = 0.0f;
                myPostfilter->v[indexSource][k] = 0.0f;
                myPostfilter->GH1[indexSource][k] = 0.0f;
                myPostfilter->localZeta[indexSource][k] = 0.0f;
                myPostfilter->globalZeta[indexSource][k] = 0.0f;
                myPostfilter->frameZeta[indexSource][k] = 0.0f;
                myPostfilter->localP[indexSource][k] = 0.0f;
                myPostfilter->globalP[indexSource][k] = 0.0f;
                myPostfilter->frameP[indexSource][k] = 0.0f;
                myPostfilter->p[indexSource][k] = 0.0f;
                myPostfilter->q[indexSource][k] = 0.0f;
                myPostfilter->G[indexSource][k] = 0.0f;

            }
        }

        for (k = 0; k < myPostfilter->POSTFILTER_NBSOURCES; k++)
        {
            myPostfilter->localResultCropped[k] = 0.0f;
            myPostfilter->globalResultCropped[k] = 0.0f;
            myPostfilter->frameResultCropped[k] = 0.0f;
        }

}

/*******************************************************************************
 * postfilterTerminate                                                         *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:  myPostfilter    The postfilter object used to be terminated        *
 *                                                                             *
 * Outputs: (none)                                                             *
 *                                                                             *
 * Description: This function terminates the postfilter object.                *
 *                                                                             *
 ******************************************************************************/

void postfilterTerminate(struct objPostfilter* myPostfilter)
{

    unsigned int indexSource;

    // *************************************************************************
    // * STEP 1: Free memory                                                   *
    // *************************************************************************

    // +-----------------------------------------------------------------------+
    // | Step A: Terminate MCRA objects                                        |
    // +-----------------------------------------------------------------------+

        for (indexSource = 0; indexSource < myPostfilter->POSTFILTER_NBSOURCES; indexSource++)
        {
            mcraTerminate(&myPostfilter->mcra[indexSource]);
        }

        deleteTable1D((void*) myPostfilter->mcra);

    // +-----------------------------------------------------------------------+
    // | Step B: Delete sources arrays                                         |
    // +-----------------------------------------------------------------------+

        idListTerminate(&myPostfilter->sourcesIDNow);
        idListTerminate(&myPostfilter->sourcesIDAdded);
        idListTerminate(&myPostfilter->sourcesIDDeleted);

        deleteTable1D((void*) myPostfilter->sourcesID);

    // +-----------------------------------------------------------------------+
    // | Step C: Delete input and output arrays                                |
    // +-----------------------------------------------------------------------+

        deleteTable2D((void**) myPostfilter->YReal);
        deleteTable2D((void**) myPostfilter->YImag);
        deleteTable2D((void**) myPostfilter->YPower);
        deleteTable2D((void**) myPostfilter->SReal);
        deleteTable2D((void**) myPostfilter->SImag);
        deleteTable2D((void**) myPostfilter->SPower);

    // +-----------------------------------------------------------------------+
    // | Step D: Delete noise arrays                                           |
    // +-----------------------------------------------------------------------+

        // +-------------------------------------------------------------------+
        // | Step i: Static noise                                              |
        // +-------------------------------------------------------------------+

            deleteTable2D((void**) myPostfilter->lambdaStat);

        // +-------------------------------------------------------------------+
        // | Step ii: Leakage noise                                            |
        // +-------------------------------------------------------------------+

            deleteTable2D((void**) myPostfilter->Z);
            deleteTable2D((void**) myPostfilter->lambdaLeak);

        // +-------------------------------------------------------------------+
        // | Step iii: Reverberation noise                                     |
        // +-------------------------------------------------------------------+

            deleteTable2D((void**) myPostfilter->lambdaRev);

        // +-------------------------------------------------------------------+
        // | Step iv: Total noise                                              |
        // +-------------------------------------------------------------------+

            deleteTable2D((void**) myPostfilter->lambda);

    // +-----------------------------------------------------------------------+
    // | Step E: Terminate transcendental object                               |
    // +-----------------------------------------------------------------------+

        transcendentalTerminate(&myPostfilter->transcendental);

    // +-----------------------------------------------------------------------+
    // | Step F: Delete speech presence arrays                                 |
    // +-----------------------------------------------------------------------+

        deleteTable2D((void**) myPostfilter->gamma);
        deleteTable2D((void**) myPostfilter->xi);
        deleteTable2D((void**) myPostfilter->alphaP);
        deleteTable2D((void**) myPostfilter->v);
        deleteTable2D((void**) myPostfilter->GH1);

    // +-----------------------------------------------------------------------+
    // | Step G: Delete speech presence gain arrays                            |
    // +-----------------------------------------------------------------------+

        // +-------------------------------------------------------------------+
        // | Step i: Terminate windows                                         |
        // +-------------------------------------------------------------------+

            // +---------------------------------------------------------------+
            // | Step a: Local window                                          |
            // +---------------------------------------------------------------+

                linearCorrelationTerminate(&myPostfilter->localWindow);

            // +---------------------------------------------------------------+
            // | Step b: Global window                                         |
            // +---------------------------------------------------------------+

                linearCorrelationTerminate(&myPostfilter->globalWindow);

            // +---------------------------------------------------------------+
            // | Step c: Frame window                                          |
            // +---------------------------------------------------------------+

                linearCorrelationTerminate(&myPostfilter->frameWindow);

        // +-------------------------------------------------------------------+
        // | Step ii: Result after windowing                                   |
        // +-------------------------------------------------------------------+

            // +---------------------------------------------------------------+
            // | Step a: Local window                                          |
            // +---------------------------------------------------------------+

                deleteTable1D((void*) myPostfilter->localResultCropped);

            // +---------------------------------------------------------------+
            // | Step b: Global window                                         |
            // +---------------------------------------------------------------+

                deleteTable1D((void*) myPostfilter->globalResultCropped);

            // +---------------------------------------------------------------+
            // | Step c: Frame window                                          |
            // +---------------------------------------------------------------+

                deleteTable1D((void*) myPostfilter->frameResultCropped);

        // +-------------------------------------------------------------------+
        // | Step iii: A priori SNR                                            |
        // +-------------------------------------------------------------------+

            // +---------------------------------------------------------------+
            // | Step a: Local window                                          |
            // +---------------------------------------------------------------+

                deleteTable2D((void**) myPostfilter->localZeta);

            // +---------------------------------------------------------------+
            // | Step b: Global window                                         |
            // +---------------------------------------------------------------+

                deleteTable2D((void**) myPostfilter->globalZeta);

            // +---------------------------------------------------------------+
            // | Step c: Frame window                                          |
            // +---------------------------------------------------------------+

                deleteTable2D((void**) myPostfilter->frameZeta);

        // +-------------------------------------------------------------------+
        // | Step iv: Speech presence probabilities                            |
        // +-------------------------------------------------------------------+

            // +---------------------------------------------------------------+
            // | Step a: Local window                                          |
            // +---------------------------------------------------------------+

                deleteTable2D((void**) myPostfilter->localP);

            // +---------------------------------------------------------------+
            // | Step b: Global window                                         |
            // +---------------------------------------------------------------+

                deleteTable2D((void**) myPostfilter->globalP);

            // +---------------------------------------------------------------+
            // | Step c: Frame window                                          |
            // +---------------------------------------------------------------+

                deleteTable2D((void**) myPostfilter->frameP);

            // +---------------------------------------------------------------+
            // | Step d: A priori probability of speech presence               |
            // +---------------------------------------------------------------+

                deleteTable2D((void**) myPostfilter->p);

            // +---------------------------------------------------------------+
            // | Step e: Probability of speech presence                        |
            // +---------------------------------------------------------------+

                deleteTable2D((void**) myPostfilter->q);

        // +-----------------------------------------------------------+
        // | Step v: Gain                                              |
        // +-----------------------------------------------------------+

            // Gain
            deleteTable2D((void**) myPostfilter->G);

}

/*******************************************************************************
 * postfilterAddSource                                                         *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:  myPostfilter    The postfilter object in which the source is added *
 *          myPreprocessor  The preprocessor object used to init the source    *
 *          newID           The ID of the new source                           *
 *                                                                             *
 * Outputs: myPostfilter    The updated postfilter object                      *
 *                                                                             *
 * Description: This function adds a source to the postfilter object.          *
 *                                                                             *
 ******************************************************************************/

void postfilterAddSource(struct objPostfilter* myPostfilter, struct objPreprocessor* myPreprocessor, ID_TYPE newID)
{

    unsigned int indexID;

    unsigned int k;

    // Add ID to list
    idListAdd(&myPostfilter->sourcesIDNow, newID);

    // Find a free spot
    for (indexID = 0; indexID < myPostfilter->POSTFILTER_NBSOURCES; indexID++)
    {
        if (myPostfilter->sourcesID[indexID] == ID_NOSOURCE)
        {

            // Copy the ID
            myPostfilter->sourcesID[indexID] = newID;

            // Copy the static noise (which needs to be initiated from
            // the static noise of the microphones)
            mcraClone(&myPostfilter->mcra[indexID],myPreprocessor->micArray[0]->noiseEstimator);

            // Reset the arrays at this spot
            for (k = 0; k < myPostfilter->POSTFILTER_NFRAMES; k++)
            {
                myPostfilter->YReal[indexID][k] = 0.0f;
                myPostfilter->YImag[indexID][k] = 0.0f;
                myPostfilter->YPower[indexID][k] = 0.0f;
                myPostfilter->SReal[indexID][k] = 0.0f;
                myPostfilter->SImag[indexID][k] = 0.0f;
                myPostfilter->SPower[indexID][k] = 0.0f;
                myPostfilter->lambdaStat[indexID][k] = 0.0f;
                myPostfilter->Z[indexID][k] = 0.0f;
                myPostfilter->lambdaLeak[indexID][k] = 0.0f;
                myPostfilter->lambdaRev[indexID][k] = 0.0f;
                myPostfilter->lambda[indexID][k] = 0.0f;
                myPostfilter->gamma[indexID][k] = 0.0f;
                myPostfilter->xi[indexID][k] = 0.0f;
                myPostfilter->alphaP[indexID][k] = 0.0f;
                myPostfilter->v[indexID][k] = 0.0f;
                myPostfilter->GH1[indexID][k] = 0.0f;
                myPostfilter->localZeta[indexID][k] = 0.0f;
                myPostfilter->globalZeta[indexID][k] = 0.0f;
                myPostfilter->frameZeta[indexID][k] = 0.0f;
                myPostfilter->localP[indexID][k] = 0.0f;
                myPostfilter->globalP[indexID][k] = 0.0f;
                myPostfilter->frameP[indexID][k] = 0.0f;
                myPostfilter->p[indexID][k] = 0.0f;
                myPostfilter->q[indexID][k] = 0.0f;
                myPostfilter->G[indexID][k] = 0.0f;
            }

            break;

        }
    }

}

/*******************************************************************************
 * postfilterDeleteSource                                                      *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:  myPostfilter    The postfilter object to delete a source           *
 *          deleteID        The ID of the source to be deleted                 *
 *                                                                             *
 * Outputs: myPostfilter    The updated postfilter object                      *
 *                                                                             *
 * Description: This function deletes a source from the postfilter object.     *
 *                                                                             *
 ******************************************************************************/

void postfilterDeleteSource(struct objPostfilter* myPostfilter, ID_TYPE deleteID)
{

    unsigned int indexID;

    // Delete ID from list
    idListDelete(&myPostfilter->sourcesIDNow, deleteID);

    // Free the associated spot
    for (indexID = 0; indexID < myPostfilter->POSTFILTER_NBSOURCES; indexID++)
    {
        if (myPostfilter->sourcesID[indexID] == deleteID)
        {

            // Free the spot
            myPostfilter->sourcesID[indexID] = ID_NOSOURCE;

            break;

        }
    }

}

/*******************************************************************************
 * postfilterRefreshSources                                                    *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:  myPostfilter    The postfilter object used                         *
 *          mySeparatedSources                                                 *
 *                          The separated sources object used                  *
 *          myPreprocessor  The preprocessor object used                       *
 *                                                                             *
 * Outputs: myPostfilter    The updated postfilter object                      *
 *                                                                             *
 * Description: This function updates the sources in the postfilter object.    *
 *                                                                             *
 ******************************************************************************/

void postfilterRefreshSources(struct objPostfilter* myPostfilter, struct objSeparatedSources* mySeparatedSources, struct objPreprocessor* myPreprocessor)
{

    unsigned int indexSource;
    unsigned int k;
    unsigned int indexID;
    ID_TYPE currentID;

#ifdef USE_SIMD

    // SIMD registers
    __m128_mod regA, regB, regC, regD, regE;

#endif

    // *************************************************************************
    // * STEP 1: Find sources to be deleted and added                          *
    // *************************************************************************

        idListCompare(&myPostfilter->sourcesIDNow, &mySeparatedSources->sourcesID, &myPostfilter->sourcesIDAdded, &myPostfilter->sourcesIDDeleted);

    // *************************************************************************
    // * STEP 2: Delete sources                                                *
    // *************************************************************************

        for (indexSource = 0; indexSource < idListGetNElements(&myPostfilter->sourcesIDDeleted); indexSource++)
        {
            currentID = idListGetID(&myPostfilter->sourcesIDDeleted, indexSource);
            postfilterDeleteSource(myPostfilter, currentID);
        }

    // *************************************************************************
    // * STEP 3: Add sources                                                   *
    // *************************************************************************

        for (indexSource = 0; indexSource < idListGetNElements(&myPostfilter->sourcesIDAdded); indexSource++)
        {
            currentID = idListGetID(&myPostfilter->sourcesIDAdded, indexSource);
            postfilterAddSource(myPostfilter, myPreprocessor, currentID);
        }

    // *************************************************************************
    // * STEP 1: Load separated sources                                        *
    // *************************************************************************

        // Load frames
        for (indexID = 0; indexID < idListGetNElements(&mySeparatedSources->sourcesID); indexID++)
        {

            currentID = idListGetID(&mySeparatedSources->sourcesID, indexID);

            for (indexSource = 0; indexSource < myPostfilter->POSTFILTER_NBSOURCES; indexSource++)
            {

                if (myPostfilter->sourcesID[indexSource] == currentID)
                {

#ifndef USE_SIMD

                    for (k = 0; k < myPostfilter->POSTFILTER_NFRAMES; k++)
                    {

                        myPostfilter->YReal[indexSource][k] = mySeparatedSources->sourcesFramesReal[indexID][k];
                        myPostfilter->YImag[indexSource][k] = mySeparatedSources->sourcesFramesImag[indexID][k];
                        myPostfilter->YPower[indexSource][k] = myPostfilter->YReal[indexSource][k] * myPostfilter->YReal[indexSource][k] +
                                                               myPostfilter->YImag[indexSource][k] * myPostfilter->YImag[indexSource][k];

                    }

#else

                    for (k = 0; k < myPostfilter->POSTFILTER_NFRAMES; k+=4)
                    {

                        regA.m128 = _mm_load_ps(&mySeparatedSources->sourcesFramesReal[indexID][k]);
                        regB.m128 = _mm_load_ps(&mySeparatedSources->sourcesFramesImag[indexID][k]);

                        regC.m128 = _mm_mul_ps(regA.m128,regA.m128);
                        regD.m128 = _mm_mul_ps(regB.m128,regB.m128);
                        regE.m128 = _mm_add_ps(regC.m128,regD.m128);

                        _mm_store_ps(&myPostfilter->YReal[indexSource][k], regA.m128);
                        _mm_store_ps(&myPostfilter->YImag[indexSource][k], regB.m128);
                        _mm_store_ps(&myPostfilter->YPower[indexSource][k], regE.m128);

                    }
                    for (k = k - 4; k < myPostfilter->POSTFILTER_NFRAMES; k++)
                    {

                        myPostfilter->YReal[indexSource][k] = mySeparatedSources->sourcesFramesReal[indexID][k];
                        myPostfilter->YImag[indexSource][k] = mySeparatedSources->sourcesFramesImag[indexID][k];
                        myPostfilter->YPower[indexSource][k] = myPostfilter->YReal[indexSource][k] * myPostfilter->YReal[indexSource][k] +
                                                               myPostfilter->YImag[indexSource][k] * myPostfilter->YImag[indexSource][k];
                    }
#endif

                }

            }

        }


}

/*******************************************************************************
 * postfilterProcess                                                           *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:  myPostfilter    The postfilter object used                         *
 *          mySeparatedSources                                                 *
 *                          The separated sources object used                  *
 *          myPreprocessor  The preprocessor object used                       *
 *                                                                             *
 * Outputs: myPostfilter    The updated postfilter object                      *
 *          myPostfilteredSources                                              *
 *                          The postfiltered sources                           *
 *                                                                             *
 * Description: This function uses the separated sources to produce frames for *
 *              the postfiltered sources.                                      *
 *                                                                             *
 ******************************************************************************/

void postfilterProcess(struct objPostfilter* myPostfilter, struct objSeparatedSources* mySeparatedSources, struct objPreprocessor* myPreprocessor, struct objPostfilteredSources* myPostfilteredSources)
{

    int indexID;
    unsigned int indexSource;
    unsigned int indexSource2;
    unsigned int k;

    float tmp;

#ifdef USE_SIMD

    // SIMD registers
    __m128_mod regA, regB, regC, regD, regE, regF, regG, regH;

#endif

    // *************************************************************************
    // * STEP 1: Load separated sources                                        *
    // *************************************************************************

    postfilterRefreshSources(myPostfilter, mySeparatedSources, myPreprocessor);

    // *************************************************************************
    // * STEP 2: Noise updates                                                 *
    // *************************************************************************

    // +-----------------------------------------------------------------------+
    // | Step A: Static noise                                                  |
    // +-----------------------------------------------------------------------+

    for (indexSource = 0; indexSource < myPostfilter->POSTFILTER_NBSOURCES; indexSource++)
    {
        if (myPostfilter->sourcesID[indexSource] != ID_NOSOURCE)
        {

            // Update the MCRA objects
            mcraProcessFrame(&myPostfilter->mcra[indexSource], myPostfilter->YPower[indexSource], myPostfilter->lambdaStat[indexSource]);

        }
    }

    // +-----------------------------------------------------------------------+
    // | Step B: Leakage noise                                                 |
    // +-----------------------------------------------------------------------+

        // +-------------------------------------------------------------------+
        // | Step i: Compute Z                                                 |
        // +-------------------------------------------------------------------+

        for (indexSource = 0; indexSource < myPostfilter->POSTFILTER_NBSOURCES; indexSource++)
        {
            if (myPostfilter->sourcesID[indexSource] != ID_NOSOURCE)
            {

    #ifndef USE_SIMD

                for (k = 0; k < myPostfilter->POSTFILTER_NFRAMES; k++)
                {

                    // Update Z: Z_m(k,l) = alphaS * Z_m,(k,l-1) + (1 - alphaS) * |Y_m(k,l)|^2
                    myPostfilter->Z[indexSource][k] = myPostfilter->POSTFILTER_ALPHAS * myPostfilter->Z[indexSource][k] +
                                                      (1.0f - myPostfilter->POSTFILTER_ALPHAS) * myPostfilter->YPower[indexSource][k];

                }

    #else

                // Load the constant alphaS
                regA.m128_f32[0] = myPostfilter->POSTFILTER_ALPHAS;
                regA.m128_f32[1] = myPostfilter->POSTFILTER_ALPHAS;
                regA.m128_f32[2] = myPostfilter->POSTFILTER_ALPHAS;
                regA.m128_f32[3] = myPostfilter->POSTFILTER_ALPHAS;

                // Load the constant (1 - alphaS)
                regB.m128_f32[0] = 1 - myPostfilter->POSTFILTER_ALPHAS;
                regB.m128_f32[1] = 1 - myPostfilter->POSTFILTER_ALPHAS;
                regB.m128_f32[2] = 1 - myPostfilter->POSTFILTER_ALPHAS;
                regB.m128_f32[3] = 1 - myPostfilter->POSTFILTER_ALPHAS;

                for (k = 0; k < myPostfilter->POSTFILTER_HALFNFRAMES; k+=4)
                {

                    // Update Z: Z_m(k,l) = alphaS * Z_m,(k,l-1) + (1 - alphaS) * |Y_m(k,l)|^2

                    regC.m128 = _mm_load_ps(&myPostfilter->Z[indexSource][k]);
                    regD.m128 = _mm_load_ps(&myPostfilter->YPower[indexSource][k]);

                    regE.m128 = _mm_mul_ps(regA.m128, regC.m128);
                    regF.m128 = _mm_mul_ps(regB.m128, regD.m128);
                    regG.m128 = _mm_add_ps(regE.m128, regF.m128);

                    _mm_store_ps(&myPostfilter->Z[indexSource][k], regG.m128);

                }

                // Update Z: Z_m(k,l) = alphaS * Z_m,(k,l-1) + (1 - alphaS) * |Y_m(k,l)|^2
                myPostfilter->Z[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] = myPostfilter->POSTFILTER_ALPHAS * myPostfilter->Z[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] +
                                                                                     (1 - myPostfilter->POSTFILTER_ALPHAS) * myPostfilter->YPower[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES];

    #endif

            }
        }

        // +-------------------------------------------------------------------+
        // | Step ii: Compute lambda_leak                                      |
        // +-------------------------------------------------------------------+

        for (indexSource = 0; indexSource < myPostfilter->POSTFILTER_NBSOURCES; indexSource++)
        {

            if (myPostfilter->sourcesID[indexSource] != ID_NOSOURCE)
            {

#ifdef USE_SIMD

                // Load the constant eta
                regA.m128_f32[0] = myPostfilter->POSTFILTER_ETA;
                regA.m128_f32[1] = myPostfilter->POSTFILTER_ETA;
                regA.m128_f32[2] = myPostfilter->POSTFILTER_ETA;
                regA.m128_f32[3] = myPostfilter->POSTFILTER_ETA;

#endif

#ifndef USE_SIMD

                // Set lambda_leak to 0 everywhere
                for (k = 0; k < myPostfilter->POSTFILTER_NFRAMES; k++)
                {
                    myPostfilter->lambdaLeak[indexSource][k] = 0.0f;
                }

#else

                // Load the constant 0
                regB.m128_f32[0] = 0.0;
                regB.m128_f32[1] = 0.0;
                regB.m128_f32[2] = 0.0;
                regB.m128_f32[3] = 0.0;

                // Set lambda_leak to 0 everywhere
                for (k = 0; k < myPostfilter->POSTFILTER_HALFNFRAMES; k+=4)
                {
                    _mm_store_ps(&myPostfilter->lambdaLeak[indexSource][k], regB.m128);
                }

                myPostfilter->lambdaLeak[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] = 0.0;

#endif

                for (indexSource2 = 0; indexSource2 < myPostfilter->POSTFILTER_NBSOURCES; indexSource2++)
                {

                    if (myPostfilter->sourcesID[indexSource2] != ID_NOSOURCE)
                    {

                        if (indexSource != indexSource2)
                        {

#ifndef USE_SIMD

                            for (k = 0; k < myPostfilter->POSTFILTER_NFRAMES; k++)
                            {

                                // Update: lambda_leak_m(k,l) += Z_i(k,l)
                                myPostfilter->lambdaLeak[indexSource][k] += myPostfilter->Z[indexSource2][k];

                            }

#else

                            for (k = 0; k < myPostfilter->POSTFILTER_HALFNFRAMES; k+=4)
                            {

                                // Update: lambda_leak_m(k,l) += Z_i(k,l)

                                regC.m128 = _mm_load_ps(&myPostfilter->Z[indexSource2][k]);
                                regD.m128 = _mm_load_ps(&myPostfilter->lambdaLeak[indexSource][k]);
                                regE.m128 = _mm_add_ps(regC.m128,regD.m128);
                                _mm_store_ps(&myPostfilter->lambdaLeak[indexSource][k], regE.m128);

                            }

                            // Update: lambda_leak_m(k,l) += Z_i(k,l)
                            myPostfilter->lambdaLeak[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] += myPostfilter->Z[indexSource2][myPostfilter->POSTFILTER_HALFNFRAMES];

#endif

                        }

                    }

                }

            }

        }

        for (indexSource = 0; indexSource < myPostfilter->POSTFILTER_NBSOURCES; indexSource++)
        {

            if (myPostfilter->sourcesID[indexSource] != ID_NOSOURCE)
            {


#ifndef USE_SIMD

                // Update: lambda_leak_m(k,l) *= eta
                for (k = 0; k < myPostfilter->POSTFILTER_NFRAMES; k++)
                {
                    myPostfilter->lambdaLeak[indexSource][k] *= myPostfilter->POSTFILTER_ETA;
                }

#else

                // Update: lambda_leak_m(k,l) *= eta
                for (k = 0; k < myPostfilter->POSTFILTER_HALFNFRAMES; k+=4)
                {

                    regB.m128 = _mm_load_ps(&myPostfilter->lambdaLeak[indexSource][k]);
                    regC.m128 = _mm_mul_ps(regA.m128,regB.m128);

                    _mm_store_ps(&myPostfilter->lambdaLeak[indexSource][k], regC.m128);

                }

                myPostfilter->lambdaLeak[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] *= myPostfilter->POSTFILTER_ETA;

#endif

            }

        }

    // +-----------------------------------------------------------------------+
    // | Step C: Reverberation noise                                           |
    // +-----------------------------------------------------------------------+

    for (indexSource = 0; indexSource < myPostfilter->POSTFILTER_NBSOURCES; indexSource++)
    {

        if (myPostfilter->sourcesID[indexSource] != ID_NOSOURCE)
        {

#ifndef USE_SIMD

            for (k = 0; k < myPostfilter->POSTFILTER_NFRAMES; k++)
            {

                // Update: lambda_rev(k,l) = gamma * lambda_rev(k,l-1) + ((1-gamma)/delta) * |Si(k,l-1)|^2
//                myPostfilter->lambdaRev[indexSource][k] = myPostfilter->POSTFILTER_GAMMA * myPostfilter->lambdaRev[indexSource][k] +
//                                                          ((1.0f - myPostfilter->POSTFILTER_GAMMA)/myPostfilter->POSTFILTER_DELTA) * (myPostfilter->SPower[indexSource][k]);
                myPostfilter->lambdaRev[indexSource][k] = myPostfilter->POSTFILTER_GAMMA * myPostfilter->lambdaRev[indexSource][k] +
                                                          ((1.0f - myPostfilter->POSTFILTER_GAMMA)/myPostfilter->POSTFILTER_DELTA) * (myPostfilter->YPower[indexSource][k]);

            }

#else

            // Load the constant gamma
            regA.m128_f32[0] = myPostfilter->POSTFILTER_GAMMA;
            regA.m128_f32[1] = myPostfilter->POSTFILTER_GAMMA;
            regA.m128_f32[2] = myPostfilter->POSTFILTER_GAMMA;
            regA.m128_f32[3] = myPostfilter->POSTFILTER_GAMMA;

            // Load the constant (1-gamma)/delta
            regB.m128_f32[0] = ((1 - myPostfilter->POSTFILTER_GAMMA)/myPostfilter->POSTFILTER_DELTA);
            regB.m128_f32[1] = ((1 - myPostfilter->POSTFILTER_GAMMA)/myPostfilter->POSTFILTER_DELTA);
            regB.m128_f32[2] = ((1 - myPostfilter->POSTFILTER_GAMMA)/myPostfilter->POSTFILTER_DELTA);
            regB.m128_f32[3] = ((1 - myPostfilter->POSTFILTER_GAMMA)/myPostfilter->POSTFILTER_DELTA);

            for (k = 0; k < myPostfilter->POSTFILTER_HALFNFRAMES; k+=4)
            {

                // Update: lambda_rev(k,l) = gamma * lambda_rev(k,l-1) + ((1-gamma)/delta) * |Si(k,l-1)|^2

                regC.m128 = _mm_load_ps(&myPostfilter->lambdaRev[indexSource][k]);
                //regD.m128 = _mm_load_ps(&myPostfilter->SPower[indexSource][k]);
                regD.m128 = _mm_load_ps(&myPostfilter->YPower[indexSource][k]);
                regE.m128 = _mm_mul_ps(regA.m128,regC.m128);
                regF.m128 = _mm_mul_ps(regB.m128,regD.m128);
                regG.m128 = _mm_add_ps(regE.m128,regF.m128);
                _mm_store_ps(&myPostfilter->lambdaRev[indexSource][k],regG.m128);

            }

            // Update: lambda_rev(k,l) = gamma * lambda_rev(k,l-1) + ((1-gamma)/delta) * |Si(k,l-1)|^2
//            myPostfilter->lambdaRev[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] = myPostfilter->POSTFILTER_GAMMA * myPostfilter->lambdaRev[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] +
//                                                                                         ((1 - myPostfilter->POSTFILTER_GAMMA)/myPostfilter->POSTFILTER_DELTA) * (myPostfilter->SPower[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES]);
            myPostfilter->lambdaRev[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] = myPostfilter->POSTFILTER_GAMMA * myPostfilter->lambdaRev[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] +
                                                                                         ((1 - myPostfilter->POSTFILTER_GAMMA)/myPostfilter->POSTFILTER_DELTA) * (myPostfilter->YPower[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES]);

#endif

        }

    }

    // +-----------------------------------------------------------------------+
    // | Step D: Total noise                                                   |
    // +-----------------------------------------------------------------------+

    for (indexSource = 0; indexSource < myPostfilter->POSTFILTER_NBSOURCES; indexSource++)
    {

        if (myPostfilter->sourcesID[indexSource] != ID_NOSOURCE)
        {

#ifndef USE_SIMD

            for (k = 0; k < myPostfilter->POSTFILTER_NFRAMES; k++)
            {

                // Update: lambda(k,l) = lambda_stat(k,l) + lambda_leak(k,l)
                myPostfilter->lambda[indexSource][k] = myPostfilter->lambdaStat[indexSource][k] +
                                                       myPostfilter->lambdaLeak[indexSource][k];

            }

#else

            for (k = 0; k < myPostfilter->POSTFILTER_HALFNFRAMES; k+=4)
            {

                // Update: lambda(k,l) = lambda_stat(k,l) + lambda_leak(k,l)
                regA.m128 = _mm_load_ps(&myPostfilter->lambdaStat[indexSource][k]);
                regB.m128 = _mm_load_ps(&myPostfilter->lambdaLeak[indexSource][k]);
                regC.m128 = _mm_add_ps(regA.m128,regB.m128);
                _mm_store_ps(&myPostfilter->lambda[indexSource][k],regC.m128);

            }

            // Update: lambda(k,l) = lambda_stat(k,l) + lambda_leak(k,l)
            myPostfilter->lambda[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] = myPostfilter->lambdaStat[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] +
                                                                                      myPostfilter->lambdaLeak[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES];

#endif

            for (indexSource2 = 0; indexSource2 < myPostfilter->POSTFILTER_NBSOURCES; indexSource2++)
            {

                if (myPostfilter->sourcesID[indexSource2] != ID_NOSOURCE)
                {

#ifndef USE_SIMD

                    for (k = 0; k < myPostfilter->POSTFILTER_NFRAMES; k++)
                    {
                        myPostfilter->lambda[indexSource][k] += myPostfilter->lambdaRev[indexSource2][k];
                    }

#else

                    for (k = 0; k < myPostfilter->POSTFILTER_HALFNFRAMES; k+=4)
                    {

                        // Update: lambda(k,l) += lambda_rev(k,l)
                        regA.m128 = _mm_load_ps(&myPostfilter->lambda[indexSource][k]);
                        regB.m128 = _mm_load_ps(&myPostfilter->lambdaRev[indexSource2][k]);
                        regC.m128 = _mm_add_ps(regA.m128,regB.m128);
                        _mm_store_ps(&myPostfilter->lambda[indexSource][k], regC.m128);

                    }

                    myPostfilter->lambda[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] += myPostfilter->lambdaRev[indexSource2][myPostfilter->POSTFILTER_HALFNFRAMES];

#endif

                }

            }

        }

    }

    // *************************************************************************
    // * STEP 3: Speech presence                                               *
    // *************************************************************************

        for (indexSource = 0; indexSource < myPostfilter->POSTFILTER_NBSOURCES; indexSource++)
        {

            if (myPostfilter->sourcesID[indexSource] != ID_NOSOURCE)
            {

                // +-----------------------------------------------------------+
                // | Step A: Compute gamma(k,l)                                |
                // +-----------------------------------------------------------+

#ifndef USE_SIMD

                for (k = 0; k < myPostfilter->POSTFILTER_NFRAMES; k++)
                {

                    // Update: gamma(k) = |Y(k)|^2 / lambda(k)
                    myPostfilter->gamma[indexSource][k] = myPostfilter->YPower[indexSource][k]
													      / (myPostfilter->lambda[indexSource][k] + 1E-10f);
                }

#else

                // Load the constant 1E-10
                regA.m128_f32[0] = 1E-10;
                regA.m128_f32[1] = 1E-10;
                regA.m128_f32[2] = 1E-10;
                regA.m128_f32[3] = 1E-10;

                for (k = 0; k < myPostfilter->POSTFILTER_HALFNFRAMES; k+=4)
                {

                    // Update: gamma(k) = |Y(k)|^2 / lambda(k)
                    regC.m128 = _mm_load_ps(&myPostfilter->YPower[indexSource][k]);
                    regD.m128 = _mm_load_ps(&myPostfilter->lambda[indexSource][k]);
                    regE.m128 = _mm_add_ps(regA.m128,regD.m128);
                    regF.m128 = _mm_div_ps(regC.m128,regE.m128);
                    _mm_store_ps(&myPostfilter->gamma[indexSource][k],regF.m128);

                }
                // Update: gamma(k) = |Y(k)|^2 / lambda(k)
                myPostfilter->gamma[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] = myPostfilter->YPower[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES]
                                                                                         / (myPostfilter->lambda[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] + 1E-10);

#endif

                // +-----------------------------------------------------------+
                // | Step B: Compute alphaP(k,l)                               |
                // +-----------------------------------------------------------+

#ifndef USE_SIMD

                for (k = 0; k < myPostfilter->POSTFILTER_NFRAMES; k++)
                {
                    // alphaP(k,l) = (xi(k,l) / (xi(k,l) + 1))^2 + alphaPmin
                    myPostfilter->alphaP[indexSource][k] = (myPostfilter->xi[indexSource][k] / (myPostfilter->xi[indexSource][k] + 1.0f)) *
                                                           (myPostfilter->xi[indexSource][k] / (myPostfilter->xi[indexSource][k] + 1.0f)) +
                                                           myPostfilter->POSTFILTER_ALPHAPMIN;

                    // Make sure alphaP(k,l) does not exceed 1
                    if (myPostfilter->alphaP[indexSource][k] > 1.0f)
                    {
                        myPostfilter->alphaP[indexSource][k] = 1.0f;
                    }
                }

#else

                // Load the constant 1
                regA.m128_f32[0] = 1.0;
                regA.m128_f32[1] = 1.0;
                regA.m128_f32[2] = 1.0;
                regA.m128_f32[3] = 1.0;

                // Load the constant alphaPmin
                regB.m128_f32[0] = myPostfilter->POSTFILTER_ALPHAPMIN;
                regB.m128_f32[1] = myPostfilter->POSTFILTER_ALPHAPMIN;
                regB.m128_f32[2] = myPostfilter->POSTFILTER_ALPHAPMIN;
                regB.m128_f32[3] = myPostfilter->POSTFILTER_ALPHAPMIN;

                // Load the constant 0xFFFFFFFF
                regC.m128_u32[0] = 0xFFFFFFFF;
                regC.m128_u32[1] = 0xFFFFFFFF;
                regC.m128_u32[2] = 0xFFFFFFFF;
                regC.m128_u32[3] = 0xFFFFFFFF;

                for (k = 0; k < myPostfilter->POSTFILTER_HALFNFRAMES; k+=4)
                {
                    // alphaP(k,l) = (xi(k,l) / (xi(k,l) + 1))^2 + alphaPmin

                    // Load xi(k,l)
                    regD.m128 = _mm_load_ps(&myPostfilter->xi[indexSource][k]);

                    // xi(k,l) + 1.0
                    regE.m128 = _mm_add_ps(regA.m128,regD.m128);

                    // (xi(k,l) / (xi(k,l) + 1))
                    regF.m128 = _mm_div_ps(regD.m128,regE.m128);

                    // (xi(k,l) / (xi(k,l) + 1))^2
                    regF.m128 = _mm_mul_ps(regF.m128,regF.m128);

                    // (xi(k,l) / (xi(k,l) + 1))^2 + alphaPmin
                    regG.m128 = _mm_add_ps(regF.m128,regB.m128);

                    // Make sure alphaP does not exceed 1
                    regH.m128 = _mm_cmple_ps(regG.m128,regA.m128);
                    regG.m128 = _mm_and_ps(regG.m128,regH.m128);
                    regH.m128 = _mm_andnot_ps(regH.m128,regC.m128);
                    regH.m128 = _mm_and_ps(regH.m128,regA.m128);
                    regG.m128 = _mm_add_ps(regH.m128,regG.m128);

                    // Store
                    _mm_store_ps(&myPostfilter->alphaP[indexSource][k],regG.m128);
                }

                // alphaP(k,l) = (xi(k,l) / (xi(k,l) + 1))^2 + alphaPmin
                myPostfilter->alphaP[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] = (myPostfilter->xi[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] / (myPostfilter->xi[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] + 1.0)) *
                                                                                          (myPostfilter->xi[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] / (myPostfilter->xi[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] + 1.0)) +
                                                                                          myPostfilter->POSTFILTER_ALPHAPMIN;

                // Make sure alphaP(k,l) does not exceed 1
                if (myPostfilter->alphaP[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] > 1.0)
                {
                    myPostfilter->alphaP[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] = 1.0;
                }

#endif

                // +-----------------------------------------------------------+
                // | Step C: Compute xi(k,l)                                   |
                // +-----------------------------------------------------------+

#ifndef USE_SIMD

                for (k = 0; k < myPostfilter->POSTFILTER_NFRAMES; k++)
                {

                    // tmp = max{gamma(k,l)-1,0}

                    if ((myPostfilter->gamma[indexSource][k] - 1) < 0.0f)
                    {
                        tmp = 0.0f;
                    }
                    else
                    {
                        tmp = myPostfilter->gamma[indexSource][k] - 1.0f;
                    }


                    // (1 - alphaP(k,l)) * G_H1^2(k,l-1) * gamma(k,l) + alphaP(k,l) * max{gamma(k,l)-1,0}
                    myPostfilter->xi[indexSource][k] = (1 - myPostfilter->alphaP[indexSource][k]) *
                                                       myPostfilter->GH1[indexSource][k] * myPostfilter->GH1[indexSource][k] * myPostfilter->gamma[indexSource][k] +
                                                       myPostfilter->alphaP[indexSource][k] * tmp;
                }

#else

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

                for (k = 0; k < myPostfilter->POSTFILTER_HALFNFRAMES; k+=4)
                {

                    // Load gamma(k,l)
                    regC.m128 = _mm_load_ps(&myPostfilter->gamma[indexSource][k]);

                    // gamma(k,l) - 1
                    regD.m128 = _mm_sub_ps(regC.m128,regA.m128);

                    // max{(gamma(k,l) - 1,0}
                    regE.m128 = _mm_cmple_ps(regB.m128,regD.m128);
                    regE.m128 = _mm_and_ps(regE.m128,regD.m128);

                    // Load alphaP(k,l))
                    regD.m128 = _mm_load_ps(&myPostfilter->alphaP[indexSource][k]);

                    // Load (1 - alphaP(k,l))
                    regF.m128 = _mm_sub_ps(regA.m128,regD.m128);

                    // (1 - alphaP(k,l)) * G_H1^2(k,l-1)
                    regG.m128 = _mm_load_ps(&myPostfilter->GH1[indexSource][k]);
                    regG.m128 = _mm_mul_ps(regG.m128,regG.m128);
                    regG.m128 = _mm_mul_ps(regF.m128,regG.m128);

                    // (1 - alphaP(k,l)) * G_H1^2(k,l-1) * gamma(k,l)
                    regG.m128 = _mm_mul_ps(regG.m128,regC.m128);

                    // alphaP(k,l) * max{gamma(k,l)-1,0}
                    regH.m128 = _mm_mul_ps(regD.m128,regE.m128);

                    // (1 - alphaP(k,l)) * G_H1^2(k,l-1) * gamma(k,l) + alphaP(k,l) * max{gamma(k,l)-1,0}
                    regH.m128 = _mm_add_ps(regG.m128,regH.m128);

                    // Store
                    _mm_store_ps(&myPostfilter->xi[indexSource][k], regH.m128);

                }

                // tmp = max{gamma(k,l)-1,0}

                if ((myPostfilter->gamma[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] - 1) < 0.0)
                {
                    tmp = 0.0;
                }
                else
                {
                    tmp = myPostfilter->gamma[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] - 1;
                }


                // (1 - alphaP(k,l)) * G_H1^2(k,l-1) * gamma(k,l) + alphaP(k,l) * max{gamma(k,l)-1,0}
                myPostfilter->xi[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] = (1 - myPostfilter->alphaP[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES]) *
                                                                                       myPostfilter->GH1[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] * myPostfilter->GH1[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] * myPostfilter->gamma[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] +
                                                                                       myPostfilter->alphaP[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] * tmp;

                // Copy the other half frame
                for (k = 1; k < myPostfilter->POSTFILTER_HALFNFRAMES; k++)
                {
                    myPostfilter->xi[indexSource][myPostfilter->POSTFILTER_NFRAMES - k] = myPostfilter->xi[indexSource][k];
                }

#endif

                // +-----------------------------------------------------------+
                // | Step D: Compute v(k,l)                                    |
                // +-----------------------------------------------------------+

#ifndef USE_SIMD

                for (k = 0; k < myPostfilter->POSTFILTER_NFRAMES; k++)
                {
                    // v(k,l) = gamma(k,l) * xi(k,l) / (xi(k,l) + 1)
                    myPostfilter->v[indexSource][k] = myPostfilter->gamma[indexSource][k] *
                                                      myPostfilter->xi[indexSource][k] /
                                                      (myPostfilter->xi[indexSource][k] + 1.0f);
                }

#else

                // Load the constant 1
                regA.m128_f32[0] = 1.0;
                regA.m128_f32[1] = 1.0;
                regA.m128_f32[2] = 1.0;
                regA.m128_f32[3] = 1.0;

                for (k = 0; k < myPostfilter->POSTFILTER_HALFNFRAMES; k+=4)
                {

                    // Load xi(k,l)
                    regB.m128 = _mm_load_ps(&myPostfilter->xi[indexSource][k]);

                    // xi(k,l) + 1
                    regC.m128 = _mm_add_ps(regB.m128,regA.m128);

                    // xi(k,l) / (xi(k,l) + 1)
                    regD.m128 = _mm_div_ps(regB.m128,regC.m128);

                    // Load gamma(k,l)
                    regE.m128 = _mm_load_ps(&myPostfilter->gamma[indexSource][k]);

                    // gamma(k,l) * xi(k,l) / (xi(k,l) + 1)
                    regF.m128 = _mm_mul_ps(regE.m128,regD.m128);

                    // Store
                    _mm_store_ps(&myPostfilter->v[indexSource][k],regF.m128);

                }

                // v(k,l) = gamma(k,l) * xi(k,l) / (xi(k,l) + 1)
                myPostfilter->v[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] = myPostfilter->gamma[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] *
                                                                                     myPostfilter->xi[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] /
                                                                                     (myPostfilter->xi[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] + 1.0);

#endif

                // +-----------------------------------------------------------+
                // | Step E: Compute GH1(k,l)                                  |
                // +-----------------------------------------------------------+

                for (k = 0; k < myPostfilter->POSTFILTER_HALFNFRAMES; k++)
                {

                    tmp = transcendentalEvaluate(&myPostfilter->transcendental, myPostfilter->v[indexSource][k]);

                    myPostfilter->GH1[indexSource][k] = (myPostfilter->xi[indexSource][k] /
                                                        (myPostfilter->xi[indexSource][k] + 1.0f)) *
                                                        tmp;

                }
                
                tmp = transcendentalEvaluate(&myPostfilter->transcendental, myPostfilter->v[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES]);

                myPostfilter->GH1[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] = (myPostfilter->xi[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] /
                                                                                       (myPostfilter->xi[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] + 1.0f)) *
                                                                                       tmp;

            }

        }

    // *************************************************************************
    // * STEP 3: Speech presence probabilities                                 *
    // *************************************************************************

        for (indexSource = 0; indexSource < myPostfilter->POSTFILTER_NBSOURCES; indexSource++)
        {

            if (myPostfilter->sourcesID[indexSource] != ID_NOSOURCE)
            {

                // +-----------------------------------------------------------+
                // | Step A: Compute the speech presence probability           |
                // +-----------------------------------------------------------+

                    // +-------------------------------------------------------+
                    // | Step i: Compute the linear correlation with windows   |
                    // +-------------------------------------------------------+

                        // +---------------------------------------------------+
                        // | Step a: Local                                     |
                        // +---------------------------------------------------+

                            // Load the long vector
                            linearCorrelationLoadVectorB(&myPostfilter->localWindow, myPostfilter->xi[indexSource]);

                            // Compute
                            linearCorrelationCompute(&myPostfilter->localWindow);

                            // Store the result
                            linearCorrelationStoreVectorResultCropped(&myPostfilter->localWindow, myPostfilter->localResultCropped);

                            // Mirror
                            for (k = 1; k < myPostfilter->POSTFILTER_HALFNFRAMES; k++)
                            {
                                myPostfilter->localResultCropped[myPostfilter->POSTFILTER_NFRAMES - k] = myPostfilter->localResultCropped[k];
                            }

                        // +---------------------------------------------------+
                        // | Step b: Global                                    |
                        // +---------------------------------------------------+

                            // Load the long vector
                            linearCorrelationLoadVectorB(&myPostfilter->globalWindow, myPostfilter->xi[indexSource]);

                            // Compute
                            linearCorrelationCompute(&myPostfilter->globalWindow);

                            // Store the result
                            linearCorrelationStoreVectorResultCropped(&myPostfilter->globalWindow, myPostfilter->globalResultCropped);

                            // Mirror
                            for (k = 1; k < myPostfilter->POSTFILTER_HALFNFRAMES; k++)
                            {
                                myPostfilter->globalResultCropped[myPostfilter->POSTFILTER_NFRAMES - k] = myPostfilter->globalResultCropped[k];
                            }

                        // +---------------------------------------------------+
                        // | Step c: Frame                                     |
                        // +---------------------------------------------------+

                            // Load the long vector
                            linearCorrelationLoadVectorB(&myPostfilter->frameWindow, myPostfilter->xi[indexSource]);

                            // Compute
                            linearCorrelationCompute(&myPostfilter->frameWindow);

                            // Store the result
                            linearCorrelationStoreVectorResultCropped(&myPostfilter->frameWindow, myPostfilter->frameResultCropped);

                            // Mirror
                            for (k = 1; k < myPostfilter->POSTFILTER_HALFNFRAMES; k++)
                            {
                                myPostfilter->frameResultCropped[myPostfilter->POSTFILTER_NFRAMES - k] = myPostfilter->frameResultCropped[k];
                            }

                    // +-------------------------------------------------------+
                    // | Step ii: Compute the linear correlation with windows  |
                    // +-------------------------------------------------------+                       

                        // +---------------------------------------------------+
                        // | Step a: Local                                     |
                        // +---------------------------------------------------+

#ifndef USE_SIMD

                            for (k = 0; k < myPostfilter->POSTFILTER_NFRAMES; k++)
                            {
                                // zeta_local(k,l) = (1 - alpha_zeta) * zeta_local(k,l-1) + alpha_zeta * sum_j=-w1..w2[h_local(j)*xi(k+j)]
                                myPostfilter->localZeta[indexSource][k] = (1.0f - myPostfilter->POSTFILTER_ALPHAZETA) * myPostfilter->localZeta[indexSource][k] +
                                                                          myPostfilter->POSTFILTER_ALPHAZETA * myPostfilter->localResultCropped[k];
                            }

#else

                            // Load the constant alpha_zeta
                            regA.m128_f32[0] = myPostfilter->POSTFILTER_ALPHAZETA;
                            regA.m128_f32[1] = myPostfilter->POSTFILTER_ALPHAZETA;
                            regA.m128_f32[2] = myPostfilter->POSTFILTER_ALPHAZETA;
                            regA.m128_f32[3] = myPostfilter->POSTFILTER_ALPHAZETA;

                            // Load the constant 1 - alpha_zeta
                            regB.m128_f32[0] = 1.0 - myPostfilter->POSTFILTER_ALPHAZETA;
                            regB.m128_f32[1] = 1.0 - myPostfilter->POSTFILTER_ALPHAZETA;
                            regB.m128_f32[2] = 1.0 - myPostfilter->POSTFILTER_ALPHAZETA;
                            regB.m128_f32[3] = 1.0 - myPostfilter->POSTFILTER_ALPHAZETA;

                            for (k = 0; k < myPostfilter->POSTFILTER_HALFNFRAMES; k+=4)
                            {
                                // zeta_local(k,l) = (1 - alpha_zeta) * zeta_local(k,l-1) + alpha_zeta * sum_j=-w1..w2[h_local(j)*xi(k+j)]

                                // Load zeta_local(k,l-1)
                                regC.m128 = _mm_load_ps(&myPostfilter->localZeta[indexSource][k]);
                                // Load sum_j=-w1..w2[h_local(j)*xi(k+j)
                                regD.m128 = _mm_load_ps(&myPostfilter->localResultCropped[k]);

                                // (1 - alpha_zeta) * zeta_local(k,l-1)
                                regE.m128 = _mm_mul_ps(regC.m128, regB.m128);
                                // alpha_zeta * sum_j=-w1..w2[h_local(j)*xi(k+j)
                                regF.m128 = _mm_mul_ps(regD.m128, regA.m128);

                                // (1 - alpha_zeta) * zeta_local(k,l-1) + alpha_zeta * sum_j=-w1..w2[h_local(j)*xi(k+j)]
                                regG.m128 = _mm_add_ps(regE.m128, regF.m128);

                                // Store
                                _mm_store_ps(&myPostfilter->localZeta[indexSource][k], regG.m128);
                            }

                            // zeta_local(k,l) = (1 - alpha_zeta) * zeta_local(k,l-1) + alpha_zeta * sum_j=-w1..w2[h_local(j)*xi(k+j)]
                            myPostfilter->localZeta[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] = (1 - myPostfilter->POSTFILTER_ALPHAZETA) * myPostfilter->localZeta[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] +
                                                                                                         myPostfilter->POSTFILTER_ALPHAZETA * myPostfilter->localResultCropped[myPostfilter->POSTFILTER_HALFNFRAMES];

#endif

                        // +---------------------------------------------------+
                        // | Step b: Global                                    |
                        // +---------------------------------------------------+

#ifndef USE_SIMD

                            for (k = 0; k < myPostfilter->POSTFILTER_NFRAMES; k++)
                            {
                                // zeta_global(k,l) = (1 - alpha_zeta) * zeta_global(k,l-1) + alpha_zeta * sum_j=-w1..w2[h_global(j)*xi(k+j)]
                                myPostfilter->globalZeta[indexSource][k] = (1.0f - myPostfilter->POSTFILTER_ALPHAZETA) * myPostfilter->globalZeta[indexSource][k] +
                                                                            myPostfilter->POSTFILTER_ALPHAZETA * myPostfilter->globalResultCropped[k];
                            }

#else

                            // Load the constant alpha_zeta
                            regA.m128_f32[0] = myPostfilter->POSTFILTER_ALPHAZETA;
                            regA.m128_f32[1] = myPostfilter->POSTFILTER_ALPHAZETA;
                            regA.m128_f32[2] = myPostfilter->POSTFILTER_ALPHAZETA;
                            regA.m128_f32[3] = myPostfilter->POSTFILTER_ALPHAZETA;

                            // Load the constant 1 - alpha_zeta
                            regB.m128_f32[0] = 1.0 - myPostfilter->POSTFILTER_ALPHAZETA;
                            regB.m128_f32[1] = 1.0 - myPostfilter->POSTFILTER_ALPHAZETA;
                            regB.m128_f32[2] = 1.0 - myPostfilter->POSTFILTER_ALPHAZETA;
                            regB.m128_f32[3] = 1.0 - myPostfilter->POSTFILTER_ALPHAZETA;

                            for (k = 0; k < myPostfilter->POSTFILTER_HALFNFRAMES; k+=4)
                            {
                                // zeta_global(k,l) = (1 - alpha_zeta) * zeta_global(k,l-1) + alpha_zeta * sum_j=-w1..w2[h_global(j)*xi(k+j)]

                                // Load zeta_global(k,l-1)
                                regC.m128 = _mm_load_ps(&myPostfilter->globalZeta[indexSource][k]);
                                // Load sum_j=-w1..w2[h_global(j)*xi(k+j)
                                regD.m128 = _mm_load_ps(&myPostfilter->globalResultCropped[k]);

                                // (1 - alpha_zeta) * zeta_global(k,l-1)
                                regE.m128 = _mm_mul_ps(regC.m128, regB.m128);
                                // alpha_zeta * sum_j=-w1..w2[h_global(j)*xi(k+j)
                                regF.m128 = _mm_mul_ps(regD.m128, regA.m128);

                                // (1 - alpha_zeta) * zeta_global(k,l-1) + alpha_zeta * sum_j=-w1..w2[h_global(j)*xi(k+j)]
                                regG.m128 = _mm_add_ps(regE.m128, regF.m128);

                                // Store
                                _mm_store_ps(&myPostfilter->globalZeta[indexSource][k], regG.m128);

                            }

                            // zeta_global(k,l) = (1 - alpha_zeta) * zeta_global(k,l-1) + alpha_zeta * sum_j=-w1..w2[h_global(j)*xi(k+j)]
                            myPostfilter->globalZeta[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] = (1 - myPostfilter->POSTFILTER_ALPHAZETA) * myPostfilter->globalZeta[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] +
                                                                                                          myPostfilter->POSTFILTER_ALPHAZETA * myPostfilter->globalResultCropped[myPostfilter->POSTFILTER_HALFNFRAMES];

#endif

                        // +---------------------------------------------------+
                        // | Step c: Frame                                     |
                        // +---------------------------------------------------+

#ifndef USE_SIMD

                            for (k = 0; k < myPostfilter->POSTFILTER_NFRAMES; k++)
                            {
                                // zeta_frame(k,l) = (1 - alpha_zeta) * zeta_frame(k,l-1) + alpha_zeta * sum_j=-w1..w2[h_frame(j)*xi(k+j)]
                                myPostfilter->frameZeta[indexSource][k] = (1.0f - myPostfilter->POSTFILTER_ALPHAZETA) * myPostfilter->frameZeta[indexSource][k] +
                                                                          myPostfilter->POSTFILTER_ALPHAZETA * myPostfilter->frameResultCropped[k];
                            }

#else

                            // Load the constant alpha_zeta
                            regA.m128_f32[0] = myPostfilter->POSTFILTER_ALPHAZETA;
                            regA.m128_f32[1] = myPostfilter->POSTFILTER_ALPHAZETA;
                            regA.m128_f32[2] = myPostfilter->POSTFILTER_ALPHAZETA;
                            regA.m128_f32[3] = myPostfilter->POSTFILTER_ALPHAZETA;

                            // Load the constant 1 - alpha_zeta
                            regB.m128_f32[0] = 1.0 - myPostfilter->POSTFILTER_ALPHAZETA;
                            regB.m128_f32[1] = 1.0 - myPostfilter->POSTFILTER_ALPHAZETA;
                            regB.m128_f32[2] = 1.0 - myPostfilter->POSTFILTER_ALPHAZETA;
                            regB.m128_f32[3] = 1.0 - myPostfilter->POSTFILTER_ALPHAZETA;

                            for (k = 0; k < myPostfilter->POSTFILTER_HALFNFRAMES; k+=4)
                            {
                                // zeta_frame(k,l) = (1 - alpha_zeta) * zeta_frame(k,l-1) + alpha_zeta * sum_j=-w1..w2[h_frame(j)*xi(k+j)]

                                // Load zeta_frame(k,l-1)
                                regC.m128 = _mm_load_ps(&myPostfilter->frameZeta[indexSource][k]);
                                // Load sum_j=-w1..w2[h_frame(j)*xi(k+j)
                                regD.m128 = _mm_load_ps(&myPostfilter->frameResultCropped[k]);

                                // (1 - alpha_zeta) * zeta_frame(k,l-1)
                                regE.m128 = _mm_mul_ps(regC.m128, regB.m128);
                                // alpha_zeta * sum_j=-w1..w2[h_frame(j)*xi(k+j)
                                regF.m128 = _mm_mul_ps(regD.m128, regA.m128);

                                // (1 - alpha_zeta) * zeta_frame(k,l-1) + alpha_zeta * sum_j=-w1..w2[h_frame(j)*xi(k+j)]
                                regG.m128 = _mm_add_ps(regE.m128, regF.m128);

                                // Store
                                _mm_store_ps(&myPostfilter->frameZeta[indexSource][k], regG.m128);
                            }

                            // zeta_frame(k,l) = (1 - alpha_zeta) * zeta_frame(k,l-1) + alpha_zeta * sum_j=-w1..w2[h_frame(j)*xi(k+j)]
                            myPostfilter->frameZeta[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] = (1 - myPostfilter->POSTFILTER_ALPHAZETA) * myPostfilter->frameZeta[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] +
                                                                                                         myPostfilter->POSTFILTER_ALPHAZETA * myPostfilter->frameResultCropped[myPostfilter->POSTFILTER_HALFNFRAMES];

#endif

                    // +-------------------------------------------------------+
                    // | Step iii: Compute the speech presence probabilities   |
                    // +-------------------------------------------------------+

                            // +-----------------------------------------------+
                            // | Step a: Local                                 |
                            // +-----------------------------------------------+

#ifndef USE_SIMD

                            for (k = 0; k < myPostfilter->POSTFILTER_NFRAMES; k++)
                            {
                                // P_local(k,l) = 1 / (1 + ( teta / zeta_local(k,l) ) ^ 2)
                                //              = (zeta_local(k,l))^2 / ( (zeta_local(k,l))^2 + teta^2 )
                                tmp = myPostfilter->localZeta[indexSource][k] * myPostfilter->localZeta[indexSource][k];
                                myPostfilter->localP[indexSource][k] = tmp / (tmp + myPostfilter->POSTFILTER_TETA_LOCAL * myPostfilter->POSTFILTER_TETA_LOCAL);
                            }

#else

                            // Load the constant 1
                            regA.m128_f32[0] = 1.0;
                            regA.m128_f32[1] = 1.0;
                            regA.m128_f32[2] = 1.0;
                            regA.m128_f32[3] = 1.0;

                            // Load the constant teta^2
                            regB.m128_f32[0] = myPostfilter->POSTFILTER_TETA_LOCAL * myPostfilter->POSTFILTER_TETA_LOCAL;
                            regB.m128_f32[1] = myPostfilter->POSTFILTER_TETA_LOCAL * myPostfilter->POSTFILTER_TETA_LOCAL;
                            regB.m128_f32[2] = myPostfilter->POSTFILTER_TETA_LOCAL * myPostfilter->POSTFILTER_TETA_LOCAL;
                            regB.m128_f32[3] = myPostfilter->POSTFILTER_TETA_LOCAL * myPostfilter->POSTFILTER_TETA_LOCAL;

                            for (k = 0; k < myPostfilter->POSTFILTER_HALFNFRAMES; k+=4)
                            {
                                // P_local(k,l) = 1 / (1 + ( teta / zeta_local(k,l) ) ^ 2)
                                //              = (zeta_local(k,l))^2 / ( (zeta_local(k,l))^2 + teta^2 )

                                // Load zeta_local(k,l)
                                regC.m128 = _mm_load_ps(&myPostfilter->localZeta[indexSource][k]);

                                // Compute zeta_local(k,l) ^ 2
                                regD.m128 = _mm_mul_ps(regC.m128,regC.m128);

                                // Compute ( (zeta_local(k,l))^2 + teta^2 )
                                regE.m128 = _mm_add_ps(regD.m128,regB.m128);

                                // Compute (zeta_local(k,l))^2 / ( (zeta_local(k,l))^2 + teta^2 )
                                regF.m128 = _mm_div_ps(regD.m128,regE.m128);

                                // Store
                                _mm_store_ps(&myPostfilter->localP[indexSource][k], regF.m128);
                            }

                            // P_local(k,l) = 1 / (1 + ( teta / zeta_local(k,l) ) ^ 2)
                            //              = (zeta_local(k,l))^2 / ( (zeta_local(k,l))^2 + teta^2 )
                            tmp = myPostfilter->localZeta[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] * myPostfilter->localZeta[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES];
                            myPostfilter->localP[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] = tmp / (tmp + myPostfilter->POSTFILTER_TETA_LOCAL * myPostfilter->POSTFILTER_TETA_LOCAL);

#endif

                            // +-----------------------------------------------+
                            // | Step b: Global                                |
                            // +-----------------------------------------------+

#ifndef USE_SIMD

                            for (k = 0; k < myPostfilter->POSTFILTER_NFRAMES; k++)
                            {
                                // P_global(k,l) = 1 / (1 + ( teta / zeta_global(k,l) ) ^ 2)
                                //               = (zeta_global(k,l))^2 / ( (zeta_global(k,l))^2 + teta^2 )
                                tmp = myPostfilter->globalZeta[indexSource][k] * myPostfilter->globalZeta[indexSource][k];
                                myPostfilter->globalP[indexSource][k] = tmp / (tmp + myPostfilter->POSTFILTER_TETA_GLOBAL * myPostfilter->POSTFILTER_TETA_GLOBAL);
                            }

#else


                            // Load the constant 1
                            regA.m128_f32[0] = 1.0;
                            regA.m128_f32[1] = 1.0;
                            regA.m128_f32[2] = 1.0;
                            regA.m128_f32[3] = 1.0;

                            // Load the constant teta^2
                            regB.m128_f32[0] = myPostfilter->POSTFILTER_TETA_GLOBAL * myPostfilter->POSTFILTER_TETA_GLOBAL;
                            regB.m128_f32[1] = myPostfilter->POSTFILTER_TETA_GLOBAL * myPostfilter->POSTFILTER_TETA_GLOBAL;
                            regB.m128_f32[2] = myPostfilter->POSTFILTER_TETA_GLOBAL * myPostfilter->POSTFILTER_TETA_GLOBAL;
                            regB.m128_f32[3] = myPostfilter->POSTFILTER_TETA_GLOBAL * myPostfilter->POSTFILTER_TETA_GLOBAL;

                            for (k = 0; k < myPostfilter->POSTFILTER_HALFNFRAMES; k+=4)
                            {
                                // P_global(k,l) = 1 / (1 + ( teta / zeta_global(k,l) ) ^ 2)
                                //               = (zeta_global(k,l))^2 / ( (zeta_global(k,l))^2 + teta^2 )

                                // Load zeta_global(k,l)
                                regC.m128 = _mm_load_ps(&myPostfilter->globalZeta[indexSource][k]);

                                // Compute zeta_global(k,l) ^ 2
                                regD.m128 = _mm_mul_ps(regC.m128,regC.m128);

                                // Compute ( (zeta_global(k,l))^2 + teta^2 )
                                regE.m128 = _mm_add_ps(regD.m128,regB.m128);

                                // Compute (zeta_global(k,l))^2 / ( (zeta_global(k,l))^2 + teta^2 )
                                regF.m128 = _mm_div_ps(regD.m128,regE.m128);

                                // Store
                                _mm_store_ps(&myPostfilter->globalP[indexSource][k], regF.m128);
                            }

                            // P_global(k,l) = 1 / (1 + ( teta / zeta_global(k,l) ) ^ 2)
                            //               = (zeta_global(k,l))^2 / ( (zeta_global(k,l))^2 + teta^2 )
                            tmp = myPostfilter->globalZeta[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] * myPostfilter->globalZeta[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES];
                            myPostfilter->globalP[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] = tmp / (tmp + myPostfilter->POSTFILTER_TETA_GLOBAL * myPostfilter->POSTFILTER_TETA_GLOBAL);

#endif


                            // +-----------------------------------------------+
                            // | Step c: Frame                                 |
                            // +-----------------------------------------------+

#ifndef USE_SIMD

                            for (k = 0; k < myPostfilter->POSTFILTER_NFRAMES; k++)
                            {
                                // P_frame(k,l) = 1 / (1 + ( teta / zeta_frame(k,l) ) ^ 2)
                                //              = (zeta_frame(k,l))^2 / ( (zeta_frame(k,l))^2 + teta^2 )
                                tmp = myPostfilter->frameZeta[indexSource][k] * myPostfilter->frameZeta[indexSource][k];
                                myPostfilter->frameP[indexSource][k] = tmp / (tmp + myPostfilter->POSTFILTER_TETA_FRAME * myPostfilter->POSTFILTER_TETA_FRAME);
                            }

#else


                            // Load the constant 1
                            regA.m128_f32[0] = 1.0;
                            regA.m128_f32[1] = 1.0;
                            regA.m128_f32[2] = 1.0;
                            regA.m128_f32[3] = 1.0;

                            // Load the constant teta^2
                            regB.m128_f32[0] = myPostfilter->POSTFILTER_TETA_FRAME * myPostfilter->POSTFILTER_TETA_FRAME;
                            regB.m128_f32[1] = myPostfilter->POSTFILTER_TETA_FRAME * myPostfilter->POSTFILTER_TETA_FRAME;
                            regB.m128_f32[2] = myPostfilter->POSTFILTER_TETA_FRAME * myPostfilter->POSTFILTER_TETA_FRAME;
                            regB.m128_f32[3] = myPostfilter->POSTFILTER_TETA_FRAME * myPostfilter->POSTFILTER_TETA_FRAME;

                            for (k = 0; k < myPostfilter->POSTFILTER_HALFNFRAMES; k+=4)
                            {
                                // P_local(k,l) = 1 / (1 + ( teta / zeta_frame(k,l) ) ^ 2)
                                //              = (zeta_frame(k,l))^2 / ( (zeta_frame(k,l))^2 + teta^2 )

                                // Load zeta_frame(k,l)
                                regC.m128 = _mm_load_ps(&myPostfilter->frameZeta[indexSource][k]);

                                // Compute zeta_frame(k,l) ^ 2
                                regD.m128 = _mm_mul_ps(regC.m128,regC.m128);

                                // Compute ( (zeta_frame(k,l))^2 + teta^2 )
                                regE.m128 = _mm_add_ps(regD.m128,regB.m128);

                                // Compute (zeta_frame(k,l))^2 / ( (zeta_frame(k,l))^2 + teta^2 )
                                regF.m128 = _mm_div_ps(regD.m128,regE.m128);

                                // Store
                                _mm_store_ps(&myPostfilter->frameP[indexSource][k], regF.m128);
                            }

                            // P_frame(k,l) = 1 / (1 + ( teta / zeta_frame(k,l) ) ^ 2)
                            //              = (zeta_frame(k,l))^2 / ( (zeta_frame(k,l))^2 + teta^2 )
                            tmp = myPostfilter->frameZeta[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] * myPostfilter->frameZeta[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES];
                            myPostfilter->frameP[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] = tmp / (tmp + myPostfilter->POSTFILTER_TETA_FRAME * myPostfilter->POSTFILTER_TETA_FRAME);

#endif

                // +-----------------------------------------------------------+
                // | Step B: Compute the gain                                  |
                // +-----------------------------------------------------------+

                    // +-------------------------------------------------------+
                    // | Step i: Compute the speech absence probabilities      |
                    // +-------------------------------------------------------+

#ifndef USE_SIMD

                        for (k = 0; k < myPostfilter->POSTFILTER_NFRAMES; k++)
                        {

                            // q(k,l) = min(1 - P_local(k,l) * P_global(k,l) * P_frame(k,l), maxQ)

                            myPostfilter->q[indexSource][k] = 1.0f -
															  myPostfilter->localP[indexSource][k] *
                                                              myPostfilter->globalP[indexSource][k] *
                                                              myPostfilter->frameP[indexSource][k];

                            if (myPostfilter->q[indexSource][k] > myPostfilter->POSTFILTER_MAXQ)
                            {
                                myPostfilter->q[indexSource][k] = myPostfilter->POSTFILTER_MAXQ;
                            }

                        }

#else

                        // Load the constant 1
                        regA.m128_f32[0] = 1.0;
                        regA.m128_f32[1] = 1.0;
                        regA.m128_f32[2] = 1.0;
                        regA.m128_f32[3] = 1.0;

                        // Load the constant maxQ
                        regB.m128_f32[0] = myPostfilter->POSTFILTER_MAXQ;
                        regB.m128_f32[1] = myPostfilter->POSTFILTER_MAXQ;
                        regB.m128_f32[2] = myPostfilter->POSTFILTER_MAXQ;
                        regB.m128_f32[3] = myPostfilter->POSTFILTER_MAXQ;

                        // Load the constant 0xFFFFFFFF
                        regC.m128_u32[0] = 0xFFFFFFFF;
                        regC.m128_u32[1] = 0xFFFFFFFF;
                        regC.m128_u32[2] = 0xFFFFFFFF;
                        regC.m128_u32[3] = 0xFFFFFFFF;

                        for (k = 0; k < myPostfilter->POSTFILTER_HALFNFRAMES; k+=4)
                        {

                            // q(k,l) = min(1 - P_local(k,l) * P_global(k,l) * P_frame(k,l), maxQ)

                            // Load P_local(k,l)
                            regD.m128 = _mm_load_ps(&myPostfilter->localP[indexSource][k]);

                            // Load P_global(k,l)
                            regE.m128 = _mm_load_ps(&myPostfilter->globalP[indexSource][k]);

                            // Load P_frame(k,l)
                            regF.m128 = _mm_load_ps(&myPostfilter->frameP[indexSource][k]);

                            // P_local(k,l) * P_global(k,l) * P_frame(k,l)
                            regG.m128 = _mm_mul_ps(regD.m128,regE.m128);
                            regG.m128 = _mm_mul_ps(regG.m128,regF.m128);

                            // 1 - P_local(k,l) * P_global(k,l) * P_frame(k,l)
                            regG.m128 = _mm_sub_ps(regA.m128,regG.m128);

                            // min(1 - P_local(k,l) * P_global(k,l) * P_frame(k,l), maxQ)
                            regE.m128 = _mm_cmple_ps(regB.m128,regG.m128);
                            regD.m128 = _mm_andnot_ps(regE.m128,regC.m128);
                            regH.m128 = _mm_and_ps(regD.m128, regG.m128);
                            regE.m128 = _mm_and_ps(regE.m128, regB.m128);
                            regH.m128 = _mm_add_ps(regE.m128, regH.m128);

                            // Store
                            _mm_store_ps(&myPostfilter->q[indexSource][k], regH.m128);

                        }

                        // q(k,l) = min(1 - P_local(k,l) * P_global(k,l) * P_frame(k,l), maxQ)

                        myPostfilter->q[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] = 1.0 -
                                                                                             myPostfilter->localP[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] *
                                                                                             myPostfilter->globalP[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] *
                                                                                             myPostfilter->frameP[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES];

                        if (myPostfilter->q[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] > myPostfilter->POSTFILTER_MAXQ)
                        {
                            myPostfilter->q[indexSource][myPostfilter->POSTFILTER_HALFNFRAMES] = myPostfilter->POSTFILTER_MAXQ;
                        }

#endif

                    // +-------------------------------------------------------+
                    // | Step ii: Compute the speech presence probabilities    |
                    // +-------------------------------------------------------+

                        for (k = 0; k < myPostfilter->POSTFILTER_HALFNFRAMESPLUSONE; k++)
                        {
                            // q(k,l) / (1 - q(k,l))
                            tmp = myPostfilter->q[indexSource][k] / (1.0f - myPostfilter->q[indexSource][k]);

                            // [q(k,l) / (1 - q(k,l))] * (1 + xi(k,l))
                            myPostfilter->p[indexSource][k] = tmp * (1.0f + myPostfilter->xi[indexSource][k]);

                            // [q(k,l) / (1 - q(k,l))] * (1 + xi(k,l)) * exp(-v(k))
                            myPostfilter->p[indexSource][k] *= expf(-1.0 * myPostfilter->v[indexSource][k]);

                            // 1 + [q(k,l) / (1 - q(k,l))] * (1 + xi(k,l)) * exp(-v(k))
                            myPostfilter->p[indexSource][k] += 1.0f;

                            // 1 / (1 + [q(k,l) / (1 - q(k,l))] * (1 + xi(k,l)) * exp(-v(k)))
                            myPostfilter->p[indexSource][k] = 1.0f / myPostfilter->p[indexSource][k];
                        }

                    // +-------------------------------------------------------+
                    // | Step iii: Compute the gain                            |
                    // +-------------------------------------------------------+

                        for (k = 0; k < myPostfilter->POSTFILTER_HALFNFRAMESPLUSONE; k++)
                        {

                            // G(k,l) = [{G_H1(k,l)}^p(k,l)] * [{G_min}^(1-p(k,l))]
                            myPostfilter->G[indexSource][k] = powf(myPostfilter->GH1[indexSource][k], myPostfilter->p[indexSource][k]) *
                                                              powf(myPostfilter->POSTFILTER_GMIN, (1.0f - myPostfilter->p[indexSource][k]));
                        }
                        // Copy the other half frame
                        for (k = 1; k < myPostfilter->POSTFILTER_HALFNFRAMES; k++)
                        {
                            myPostfilter->G[indexSource][myPostfilter->POSTFILTER_NFRAMES - k] = myPostfilter->G[indexSource][k];
                        }

            }

        }

        // *************************************************************************
        // * STEP 4: Apply the gain                                                *
        // *************************************************************************

        // Reset list of IDs
        idListReset(&myPostfilteredSources->sourcesID);

        for (indexSource = 0; indexSource < myPostfilter->POSTFILTER_NBSOURCES; indexSource++)
        {

            if (myPostfilter->sourcesID[indexSource] != ID_NOSOURCE)
            {

                indexID = idListAdd(&myPostfilteredSources->sourcesID, myPostfilter->sourcesID[indexSource]);

                for (k = 0; k < myPostfilter->POSTFILTER_NFRAMES; k++)
                {
                    // Re{S(k,l)} = G(k,l) * Re{Y(k,l)}
                    myPostfilter->SReal[indexSource][k] = myPostfilter->G[indexSource][k] * myPostfilter->YReal[indexSource][k];
                    myPostfilteredSources->sourcesFramesReal[indexID][k] = myPostfilter->SReal[indexSource][k];

                    // Im{S(k,l)} = G(k,l) * Im{Y(k,l)}
                    myPostfilter->SImag[indexSource][k] = myPostfilter->G[indexSource][k] * myPostfilter->YImag[indexSource][k];
                    myPostfilteredSources->sourcesFramesImag[indexID][k] = myPostfilter->SImag[indexSource][k];

                    // |S(k,l)|^2 = (G(k,l)^2) * |Y(k,l)|^2
                    myPostfilter->SPower[indexSource][k] = myPostfilter->G[indexSource][k] * myPostfilter->G[indexSource][k] * myPostfilter->YPower[indexSource][k];

                }

            }

        }

}

void postfilterAppendStream(struct objPostfilter* myPostfilter, FILE** filePtr)
{

    unsigned int indexSource;
    unsigned int indexSample;

    // +-------------------------------------------------------------------+
    // | Parameters                                                        |
    // +-------------------------------------------------------------------+

        // +---------------------------------------------------------------+
        // | General                                                       |
        // +---------------------------------------------------------------+

        // Maximum number of separated sources
        fwrite(&myPostfilter->POSTFILTER_NBSOURCES, 1, sizeof(unsigned int), *filePtr);

        // Number of samples per frame
        fwrite(&myPostfilter->POSTFILTER_NFRAMES, 1, sizeof(unsigned int), *filePtr);

        // Half the number of samples per frame
        fwrite(&myPostfilter->POSTFILTER_HALFNFRAMES, 1, sizeof(unsigned int), *filePtr);

        // Half the number of samples per frame plus one
        fwrite(&myPostfilter->POSTFILTER_HALFNFRAMESPLUSONE, 1, sizeof(unsigned int), *filePtr);

        // +---------------------------------------------------------------+
        // | Noise                                                         |
        // +---------------------------------------------------------------+

            // +-----------------------------------------------------------+
            // | Leakage                                                   |
            // +-----------------------------------------------------------+

            // AlphaS: smoothing
            fwrite(&myPostfilter->POSTFILTER_ALPHAS, 1, sizeof(float), *filePtr);

            // Eta: reducing factor
            fwrite(&myPostfilter->POSTFILTER_ETA, 1, sizeof(float), *filePtr);

            // +-----------------------------------------------------------+
            // | Reverberation                                             |
            // +-----------------------------------------------------------+

            // Gamma: Reverberation time
            fwrite(&myPostfilter->POSTFILTER_GAMMA, 1, sizeof(float), *filePtr);

            // Delta: Signal-to-reverberation ratio
            fwrite(&myPostfilter->POSTFILTER_DELTA, 1, sizeof(float), *filePtr);

        // +---------------------------------------------------------------+
        // | Speech presence                                               |
        // +---------------------------------------------------------------+

        // AlphapMin
        fwrite(&myPostfilter->POSTFILTER_ALPHAPMIN, 1, sizeof(float), *filePtr);

        // +---------------------------------------------------------------+
        // | Speech presence gain                                          |
        // +---------------------------------------------------------------+

        // Teta
        fwrite(&myPostfilter->POSTFILTER_TETA_LOCAL, 1, sizeof(float), *filePtr);
        fwrite(&myPostfilter->POSTFILTER_TETA_GLOBAL, 1, sizeof(float), *filePtr);
        fwrite(&myPostfilter->POSTFILTER_TETA_FRAME, 1, sizeof(float), *filePtr);

        // Alpha_zeta
        fwrite(&myPostfilter->POSTFILTER_ALPHAZETA, 1, sizeof(float), *filePtr);

        // Maximum a priori probability of speech absence
        fwrite(&myPostfilter->POSTFILTER_MAXQ, 1, sizeof(float), *filePtr);

        // Minimum gain allowed when speech is absent
        fwrite(&myPostfilter->POSTFILTER_GMIN, 1, sizeof(float), *filePtr);

        // Size of the local window
        fwrite(&myPostfilter->POSTFILTER_LOCALWINDOWSIZE, 1, sizeof(unsigned int), *filePtr);


        // Size of the global window
        fwrite(&myPostfilter->POSTFILTER_GLOBALWINDOWSIZE, 1, sizeof(unsigned int), *filePtr);

        // Size of the frame window
        fwrite(&myPostfilter->POSTFILTER_FRAMEWINDOWSIZE, 1, sizeof(int), *filePtr);

    // +-------------------------------------------------------------------+
    // | Variables                                                         |
    // +-------------------------------------------------------------------+

        // +---------------------------------------------------------------+
        // | Sources                                                       |
        // +---------------------------------------------------------------+

            // Separated sources
            for (indexSource = 0; indexSource < myPostfilter->POSTFILTER_NBSOURCES; indexSource++)
            {
                for (indexSample = 0; indexSample < myPostfilter->POSTFILTER_NFRAMES; indexSample++)
                {
                    fwrite(&myPostfilter->YReal[indexSource][indexSample], 1, sizeof(float), *filePtr);
                }
            }
            for (indexSource = 0; indexSource < myPostfilter->POSTFILTER_NBSOURCES; indexSource++)
            {
                for (indexSample = 0; indexSample < myPostfilter->POSTFILTER_NFRAMES; indexSample++)
                {
                    fwrite(&myPostfilter->YImag[indexSource][indexSample], 1, sizeof(float), *filePtr);
                }
            }
            for (indexSource = 0; indexSource < myPostfilter->POSTFILTER_NBSOURCES; indexSource++)
            {
                for (indexSample = 0; indexSample < myPostfilter->POSTFILTER_NFRAMES; indexSample++)
                {
                    fwrite(&myPostfilter->YPower[indexSource][indexSample], 1, sizeof(float), *filePtr);
                }
            }

            // Filtered sources
            for (indexSource = 0; indexSource < myPostfilter->POSTFILTER_NBSOURCES; indexSource++)
            {
                for (indexSample = 0; indexSample < myPostfilter->POSTFILTER_NFRAMES; indexSample++)
                {
                    fwrite(&myPostfilter->SReal[indexSource][indexSample], 1, sizeof(float), *filePtr);
                }
            }
            for (indexSource = 0; indexSource < myPostfilter->POSTFILTER_NBSOURCES; indexSource++)
            {
                for (indexSample = 0; indexSample < myPostfilter->POSTFILTER_NFRAMES; indexSample++)
                {
                    fwrite(&myPostfilter->SImag[indexSource][indexSample], 1, sizeof(float), *filePtr);
                }
            }
            for (indexSource = 0; indexSource < myPostfilter->POSTFILTER_NBSOURCES; indexSource++)
            {
                for (indexSample = 0; indexSample < myPostfilter->POSTFILTER_NFRAMES; indexSample++)
                {
                    fwrite(&myPostfilter->SPower[indexSource][indexSample], 1, sizeof(float), *filePtr);
                }
            }


            for (indexSource = 0; indexSource < myPostfilter->POSTFILTER_NBSOURCES; indexSource++)
            {
                fwrite(&myPostfilter->sourcesID[indexSource], 1, sizeof(ID_TYPE), *filePtr);
            }

        // +---------------------------------------------------------------+
        // | Noise                                                         |
        // +---------------------------------------------------------------+

            // +-----------------------------------------------------------+
            // | Static                                                    |
            // +-----------------------------------------------------------+

            for (indexSource = 0; indexSource < myPostfilter->POSTFILTER_NBSOURCES; indexSource++)
            {
                for (indexSample = 0; indexSample < myPostfilter->POSTFILTER_NFRAMES; indexSample++)
                {
                    fwrite(&myPostfilter->lambdaStat[indexSource][indexSample], 1, sizeof(float), *filePtr);
                }
            }

            // +-----------------------------------------------------------+
            // | Leakage                                                   |
            // +-----------------------------------------------------------+

            for (indexSource = 0; indexSource < myPostfilter->POSTFILTER_NBSOURCES; indexSource++)
            {
                for (indexSample = 0; indexSample < myPostfilter->POSTFILTER_NFRAMES; indexSample++)
                {
                    fwrite(&myPostfilter->Z[indexSource][indexSample], 1, sizeof(float), *filePtr);
                }
            }
            for (indexSource = 0; indexSource < myPostfilter->POSTFILTER_NBSOURCES; indexSource++)
            {
                for (indexSample = 0; indexSample < myPostfilter->POSTFILTER_NFRAMES; indexSample++)
                {
                    fwrite(&myPostfilter->lambdaLeak[indexSource][indexSample], 1, sizeof(float), *filePtr);
                }
            }

            // +-----------------------------------------------------------+
            // | Reverberation                                             |
            // +-----------------------------------------------------------+

            for (indexSource = 0; indexSource < myPostfilter->POSTFILTER_NBSOURCES; indexSource++)
            {
                for (indexSample = 0; indexSample < myPostfilter->POSTFILTER_NFRAMES; indexSample++)
                {
                    fwrite(&myPostfilter->lambdaRev[indexSource][indexSample], 1, sizeof(float), *filePtr);
                }
            }

            // +-----------------------------------------------------------+
            // | Total                                                     |
            // +-----------------------------------------------------------+

            for (indexSource = 0; indexSource < myPostfilter->POSTFILTER_NBSOURCES; indexSource++)
            {
                for (indexSample = 0; indexSample < myPostfilter->POSTFILTER_NFRAMES; indexSample++)
                {
                    fwrite(&myPostfilter->lambda[indexSource][indexSample], 1, sizeof(float), *filePtr);
                }
            }

        // +---------------------------------------------------------------+
        // | Speech presence gain                                          |
        // +---------------------------------------------------------------+

            // gamma(k,l)
            for (indexSource = 0; indexSource < myPostfilter->POSTFILTER_NBSOURCES; indexSource++)
            {
                for (indexSample = 0; indexSample < myPostfilter->POSTFILTER_NFRAMES; indexSample++)
                {
                    fwrite(&myPostfilter->gamma[indexSource][indexSample], 1, sizeof(float), *filePtr);
                }
            }

            // xi(k,l)
            for (indexSource = 0; indexSource < myPostfilter->POSTFILTER_NBSOURCES; indexSource++)
            {
                for (indexSample = 0; indexSample < myPostfilter->POSTFILTER_NFRAMES; indexSample++)
                {
                    fwrite(&myPostfilter->xi[indexSource][indexSample], 1, sizeof(float), *filePtr);
                }
            }

            // alphaP(k,l)
            for (indexSource = 0; indexSource < myPostfilter->POSTFILTER_NBSOURCES; indexSource++)
            {
                for (indexSample = 0; indexSample < myPostfilter->POSTFILTER_NFRAMES; indexSample++)
                {
                    fwrite(&myPostfilter->alphaP[indexSource][indexSample], 1, sizeof(float), *filePtr);
                }
            }

            // v(k,l)
            for (indexSource = 0; indexSource < myPostfilter->POSTFILTER_NBSOURCES; indexSource++)
            {
                for (indexSample = 0; indexSample < myPostfilter->POSTFILTER_NFRAMES; indexSample++)
                {
                    fwrite(&myPostfilter->v[indexSource][indexSample], 1, sizeof(float), *filePtr);
                }
            }

            // G_H1(k,l)
            for (indexSource = 0; indexSource < myPostfilter->POSTFILTER_NBSOURCES; indexSource++)
            {
                for (indexSample = 0; indexSample < myPostfilter->POSTFILTER_NFRAMES; indexSample++)
                {
                    fwrite(&myPostfilter->GH1[indexSource][indexSample], 1, sizeof(float), *filePtr);
                }
            }

        // +---------------------------------------------------------------+
        // | Speech presence uncertainty                                   |
        // +---------------------------------------------------------------+

            // +-----------------------------------------------------------+
            // | A priori SNR                                              |
            // +-----------------------------------------------------------+

            // Local
            for (indexSource = 0; indexSource < myPostfilter->POSTFILTER_NBSOURCES; indexSource++)
            {
                for (indexSample = 0; indexSample < myPostfilter->POSTFILTER_NFRAMES; indexSample++)
                {
                    fwrite(&myPostfilter->localZeta[indexSource][indexSample], 1, sizeof(float), *filePtr);
                }
            }

            // Global
            for (indexSource = 0; indexSource < myPostfilter->POSTFILTER_NBSOURCES; indexSource++)
            {
                for (indexSample = 0; indexSample < myPostfilter->POSTFILTER_NFRAMES; indexSample++)
                {
                    fwrite(&myPostfilter->globalZeta[indexSource][indexSample], 1, sizeof(float), *filePtr);
                }
            }

            // Frame
            for (indexSource = 0; indexSource < myPostfilter->POSTFILTER_NBSOURCES; indexSource++)
            {
                for (indexSample = 0; indexSample < myPostfilter->POSTFILTER_NFRAMES; indexSample++)
                {
                    fwrite(&myPostfilter->frameZeta[indexSource][indexSample], 1, sizeof(float), *filePtr);
                }
            }

            // +-----------------------------------------------------------+
            // | Speech presence probabilities                             |
            // +-----------------------------------------------------------+

            // Local
            for (indexSource = 0; indexSource < myPostfilter->POSTFILTER_NBSOURCES; indexSource++)
            {
                for (indexSample = 0; indexSample < myPostfilter->POSTFILTER_NFRAMES; indexSample++)
                {
                    fwrite(&myPostfilter->localP[indexSource][indexSample], 1, sizeof(float), *filePtr);
                }
            }

            // Global
            for (indexSource = 0; indexSource < myPostfilter->POSTFILTER_NBSOURCES; indexSource++)
            {
                for (indexSample = 0; indexSample < myPostfilter->POSTFILTER_NFRAMES; indexSample++)
                {
                    fwrite(&myPostfilter->globalP[indexSource][indexSample], 1, sizeof(float), *filePtr);
                }
            }

            // Frame
            for (indexSource = 0; indexSource < myPostfilter->POSTFILTER_NBSOURCES; indexSource++)
            {
                for (indexSample = 0; indexSample < myPostfilter->POSTFILTER_NFRAMES; indexSample++)
                {
                    fwrite(&myPostfilter->frameP[indexSource][indexSample], 1, sizeof(float), *filePtr);
                }
            }

            // A priori probability of speech presence
            for (indexSource = 0; indexSource < myPostfilter->POSTFILTER_NBSOURCES; indexSource++)
            {
                for (indexSample = 0; indexSample < myPostfilter->POSTFILTER_NFRAMES; indexSample++)
                {
                    fwrite(&myPostfilter->p[indexSource][indexSample], 1, sizeof(float), *filePtr);
                }
            }

            // A priori probability of speech absence
            for (indexSource = 0; indexSource < myPostfilter->POSTFILTER_NBSOURCES; indexSource++)
            {
                for (indexSample = 0; indexSample < myPostfilter->POSTFILTER_NFRAMES; indexSample++)
                {
                    fwrite(&myPostfilter->q[indexSource][indexSample], 1, sizeof(float), *filePtr);
                }
            }

            // +-----------------------------------------------------------+
            // | Gain                                                      |
            // +-----------------------------------------------------------+

            // Gain
            for (indexSource = 0; indexSource < myPostfilter->POSTFILTER_NBSOURCES; indexSource++)
            {
                for (indexSample = 0; indexSample < myPostfilter->POSTFILTER_NFRAMES; indexSample++)
                {
                    fwrite(&myPostfilter->G[indexSource][indexSample], 1, sizeof(float), *filePtr);
                }
            }

}
