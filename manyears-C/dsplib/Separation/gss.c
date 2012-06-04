/*******************************************************************************
 * ManyEars: GSS - Source Code                                                 *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Author: François Grondin                                                    *
 * Original Code: Jean-Marc Valin                                              *
 * Modified Code: Simon Brière                                                 *
 * Version: 1.1.0                                                              *
 * Date: September 15th, 2010                                                  *
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

#include "Separation/gss.h"

/*******************************************************************************
 * Compatibility issues                                                        *
 ******************************************************************************/

/*******************************************************************************
 * gssInit                                                                     *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:  myGSS           The gss object used to be initialized              *
 *          myParameters    The parameters used for initialization             *
 *                                                                             *
 * Outputs: myGSS           The initialized gss object                         *
 *                                                                             *
 * Description: This function initializes the gss object.                      *
 *                                                                             *
 ******************************************************************************/

void gssInit(struct objGSS* myGSS, struct ParametersStruct* myParameters, struct objMicrophones* myMicrophones)
{

    unsigned int indexMicrophone;

    unsigned int indexPoint;

    float x, y, z;

    float delay;
    float delayMin;
    float delayMax;

    float distanceX;
    float distanceY;
    float distanceZ;
    float distance;

    struct objSphere* mySphere;

    unsigned int k;
    unsigned int indexDelay;

    unsigned int nSources;

    // *************************************************************************
    // * STEP 1: Load parameters                                               *
    // *************************************************************************

    myGSS->GSS_SOURCEDISTANCE = (float) myParameters->P_GSS_SOURCEDISTANCE;
    myGSS->GSS_FS = (unsigned int) GLOBAL_FS;
    myGSS->GSS_C = (float) GLOBAL_C;
    myGSS->GSS_NFRAMES = (unsigned int) GLOBAL_FRAMESIZE;
    myGSS->GSS_HALFNFRAMES = myGSS->GSS_NFRAMES / 2;
    myGSS->GSS_HALFNFRAMESPLUSONE = myGSS->GSS_HALFNFRAMES + 1;

    myGSS->GSS_LAMBDA = myParameters->P_GSS_LAMBDA;
    myGSS->GSS_MU = myParameters->P_GSS_MU;

    myGSS->GSS_NBSOURCES = (unsigned int) myParameters->P_GEN_DYNSOURCES;

    // *************************************************************************
    // * STEP 2: Initialize microphones variables                              *
    // *************************************************************************

    myGSS->myMicrophones = (struct objMicrophones*) malloc(sizeof(struct objMicrophones));
    microphonesClone(myMicrophones, myGSS->myMicrophones);

    // *************************************************************************
    // * STEP 3: Initialize general variables                                  *
    // *************************************************************************

    myGSS->delayOffset = 0;

    // *************************************************************************
    // * STEP 4: Initialize source variables                                   *
    // *************************************************************************

    idListInit(&myGSS->sourcesIDNow, myGSS->GSS_NBSOURCES);
    idListInit(&myGSS->sourcesIDAdded, myGSS->GSS_NBSOURCES);
    idListInit(&myGSS->sourcesIDDeleted, myGSS->GSS_NBSOURCES);
    myGSS->sourcesPosition = (float**) newTable2D(myGSS->GSS_NBSOURCES, 3, sizeof(float));
    nSources = 0;

    // *************************************************************************
    // * STEP 5: Initialize matrices                                           *
    // *************************************************************************

    // Initialize the matrix x(k): nMics x 1
    myGSS->x = matrixCreate(myGSS->myMicrophones->nMics,1,myGSS->GSS_HALFNFRAMESPLUSONE);

    // Initialize the matrix xH(k): 1 x nMics
    myGSS->xH = matrixCreate(1,myGSS->myMicrophones->nMics,myGSS->GSS_HALFNFRAMESPLUSONE);

    // Initialize the matrix y(k): nSources x 1
    myGSS->y = matrixCreate(nSources,1,myGSS->GSS_HALFNFRAMESPLUSONE);
    myGSS->yFull = matrixCreate(nSources,1,myGSS->GSS_NFRAMES);

    // Initialize the matrix yH(k): 1 x nSources
    myGSS->yH = matrixCreate(1,nSources,myGSS->GSS_HALFNFRAMESPLUSONE);

    // Initialize the matrix Ryy_E(k): nSources x nSources
    myGSS->Ryy_E = matrixCreate(nSources,nSources,myGSS->GSS_HALFNFRAMESPLUSONE);

    // Initialize the matrix A(k): nMics x nSources
    myGSS->A = matrixCreate(myGSS->myMicrophones->nMics,nSources,myGSS->GSS_HALFNFRAMESPLUSONE);

    // Initialize the matrix AH(k): nMics x nSources
    myGSS->AH = matrixCreate(nSources,myGSS->myMicrophones->nMics,myGSS->GSS_HALFNFRAMESPLUSONE);

    // Initialize the matrix Alpha(k): 1x1
    myGSS->alpha = matrixCreate(1,1,myGSS->GSS_HALFNFRAMESPLUSONE);
    myGSS->alphatmp = matrixCreate(1,1,myGSS->GSS_HALFNFRAMESPLUSONE);

    // Initialize the matrix W^n(k): nSources x nMics
    myGSS->Wn = matrixCreate(nSources,myGSS->myMicrophones->nMics,myGSS->GSS_HALFNFRAMESPLUSONE);
    // Tmp1: nSources x nMics
    myGSS->Wntmp1 = matrixCreate(nSources,myGSS->myMicrophones->nMics,myGSS->GSS_HALFNFRAMESPLUSONE);

    // Initialize the matrix dJ1(k): nSources x nMics
    myGSS->dJ1 = matrixCreate(nSources,myGSS->myMicrophones->nMics,myGSS->GSS_HALFNFRAMESPLUSONE);
    // Tmp1: nSources x nSources
    myGSS->dJ1tmp1 = matrixCreate(nSources,nSources,myGSS->GSS_HALFNFRAMESPLUSONE);
    // Tmp2: nSources x nMics
    myGSS->dJ1tmp2 = matrixCreate(nSources,myGSS->myMicrophones->nMics,myGSS->GSS_HALFNFRAMESPLUSONE);
    // Tmp3: nMics x 1
    myGSS->dJ1tmp3 = matrixCreate(myGSS->myMicrophones->nMics,1,myGSS->GSS_HALFNFRAMESPLUSONE);
    // Tmp4: nSources x nMics
    myGSS->dJ1tmp4 = matrixCreate(nSources,myGSS->myMicrophones->nMics,myGSS->GSS_HALFNFRAMESPLUSONE);

    // Initialize the matrix dJ2(k): nSources x nMics
    myGSS->dJ2 = matrixCreate(nSources,myGSS->myMicrophones->nMics,myGSS->GSS_HALFNFRAMESPLUSONE);
    // Tmp1: nSources x nSources
    myGSS->dJ2tmp1 = matrixCreate(nSources,nSources,myGSS->GSS_HALFNFRAMESPLUSONE);
    // Tmp2: nSources x nMics
    myGSS->dJ2tmp2 = matrixCreate(nSources,myGSS->myMicrophones->nMics,myGSS->GSS_HALFNFRAMESPLUSONE);

    // Initialize the matrix dJR'(k); nSources x nMics
    myGSS->dJR = matrixCreate(nSources,myGSS->myMicrophones->nMics,myGSS->GSS_HALFNFRAMESPLUSONE);

    // *************************************************************************
    // * STEP 6: Find the range of delays                                      *
    // *************************************************************************

    // +-----------------------------------------------------------------------+
    // | Step A: Create a unit sphere                                          |
    // +-----------------------------------------------------------------------+

    mySphere = (struct objSphere*) malloc(sizeof(struct objSphere));
    sphereInit(mySphere,4);

    // +-----------------------------------------------------------------------+
    // | Step B: Test each point                                               |
    // +-----------------------------------------------------------------------+

    delayMin = INFINITY;
    delayMax = 0.0f;

    for (indexPoint = 0; indexPoint < mySphere->SPHERE_NUMBERPOINTS; indexPoint++)
    {

        x = myGSS->GSS_SOURCEDISTANCE * mySphere->spherePoints[indexPoint][0];
        y = myGSS->GSS_SOURCEDISTANCE * mySphere->spherePoints[indexPoint][1];
        z = myGSS->GSS_SOURCEDISTANCE * mySphere->spherePoints[indexPoint][2];

        for (indexMicrophone = 0; indexMicrophone < myGSS->myMicrophones->nMics; indexMicrophone++)
        {

            distanceX = microphonesGetPositionX(myGSS->myMicrophones, indexMicrophone) - x;
            distanceY = microphonesGetPositionY(myGSS->myMicrophones, indexMicrophone) - y;
            distanceZ = microphonesGetPositionZ(myGSS->myMicrophones, indexMicrophone) - z;

            distance = sqrtf(distanceX * distanceX + distanceY * distanceY + distanceZ * distanceZ);

            delay = (myGSS->GSS_FS / myGSS->GSS_C) * distance;

            if (delay < delayMin)
            {
                delayMin = delay;
            }
            if (delay > delayMax)
            {
                delayMax = delay;
            }

        }

    }

    // Save the delay
    myGSS->delayOffset = (unsigned int) (floor(delayMin));

    // Save the range
    myGSS->delayMinimum = (unsigned int) 0;
    myGSS->delayMaximum = (unsigned int) (floor(delayMax + 1.0));

    // Clear sphere
    sphereTerminate(mySphere);
    free((void*) mySphere);

    // +---------------------------------------------------------------+
    // | Step C: Create arrays for math acceleration                   |
    // +---------------------------------------------------------------+

    myGSS->cosTable = (float**) newTable2D(myGSS->delayMaximum, myGSS->GSS_HALFNFRAMESPLUSONE, sizeof(float));
    myGSS->sinTable = (float**) newTable2D(myGSS->delayMaximum, myGSS->GSS_HALFNFRAMESPLUSONE, sizeof(float));

    for (indexDelay = 0; indexDelay < myGSS->delayMaximum; indexDelay++)
    {
        for (k = 0; k < myGSS->GSS_HALFNFRAMESPLUSONE; k++)
        {

            // Compute aij(k)   = exp(-j*2*pi*k*delay/N)
            //                  = cos(2*pi*k*delay/N) - j * sin(2*pi*k*delay/N)
            myGSS->cosTable[indexDelay][k] = cosf(2.0f * M_PI * k * indexDelay / myGSS->GSS_NFRAMES);
            myGSS->sinTable[indexDelay][k] = -1.0f * sinf(2.0f * M_PI * k * indexDelay / myGSS->GSS_NFRAMES);

        }
    }

}

/*******************************************************************************
 * gssTerminate                                                                *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:  myGSS           The gss object used to be initialized              *
 *                                                                             *
 * Outputs: (none)                                                             *
 *                                                                             *
 * Description: This function terminates the gss object.                       *
 *                                                                             *
 ******************************************************************************/

void gssTerminate(struct objGSS* myGSS)
{

    // *************************************************************************
    // * STEP 1: Terminate microphones variables                               *
    // *************************************************************************

    microphonesTerminate(myGSS->myMicrophones);
    free((void*) myGSS->myMicrophones);

    // *************************************************************************
    // * STEP 2: Terminate sources variables                                   *
    // *************************************************************************

    idListTerminate(&myGSS->sourcesIDAdded);
    idListTerminate(&myGSS->sourcesIDDeleted);
    idListTerminate(&myGSS->sourcesIDNow);
    deleteTable2D((void**) myGSS->sourcesPosition);

    // *************************************************************************
    // * STEP 3: Terminate matrices                                            *
    // *************************************************************************

    // Delete the matrix x(k)
    matrixDelete(myGSS->x);

    // Delete the matrix xH(k)
    matrixDelete(myGSS->xH);

    // Delete the matrix y(k)
    matrixDelete(myGSS->y);

    // Delete the matrix yH(k)
    matrixDelete(myGSS->yH);

    // Delete the matrix Ryy_E(k)
    matrixDelete(myGSS->Ryy_E);

    // Delete the matrix A(k)
    matrixDelete(myGSS->A);

    // Delete the matrix AH(k)
    matrixDelete(myGSS->AH);

    // Delete the matrix Alpha(k)
    matrixDelete(myGSS->alpha);
    matrixDelete(myGSS->alphatmp);

    // Delete the matrix W^n(k)
    matrixDelete(myGSS->Wn);
    // Tmp1: nSources x nMics
    matrixDelete(myGSS->Wntmp1);

    // Delete the matrix dJ1(k)
    matrixDelete(myGSS->dJ1);
    // Tmp1: nSources x nSources
    matrixDelete(myGSS->dJ1tmp1);
    // Tmp2: nSources x nMics
    matrixDelete(myGSS->dJ1tmp2);
    // Tmp3: nMics x 1
    matrixDelete(myGSS->dJ1tmp3);
    // Tmp4: nSources x nMics
    matrixDelete(myGSS->dJ1tmp4);

    // Delete the matrix dJ2(k)
    matrixDelete(myGSS->dJ2);
    // Tmp1: nSources x nSources
    matrixDelete(myGSS->dJ2tmp1);
    // Tmp2: nSources x nMics
    matrixDelete(myGSS->dJ2tmp2);

    // Delete the matrix dJR'(k)
    matrixDelete(myGSS->dJR);

    // *************************************************************************
    // * STEP 4: Terminate math acceleration tables                            *
    // *************************************************************************

    deleteTable2D((void**) myGSS->cosTable);
    deleteTable2D((void**) myGSS->sinTable);

}

/*******************************************************************************
 * gssAddSource                                                                *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:  myGSS           The gss object used in which a source is added     *
 *                                                                             *
 * Outputs: myGSS           The gss object with the source added               *
 *                                                                             *
 * Description: This function adds a new source to the gss object.             *
 *                                                                             *
 ******************************************************************************/

void gssAddSource(struct objGSS* myGSS, ID_TYPE newID, float positionX, float positionY, float positionZ)
{

    signed int indexID;

    unsigned int newNSources;
    unsigned int oldNSources;

    float x,y,z;
    float r;
    float xNotNorm,yNotNorm,zNotNorm;
    float xNorm,yNorm,zNorm;
    float distanceX,distanceY,distanceZ;
    float distance;

    unsigned int delay;
    unsigned int delayNorm;

    float aijReal;
    float aijImag;

    float wjiReal;
    float wjiImag;

    unsigned int indexMicrophone;
    unsigned int k;

    // *************************************************************************
    // * STEP 1: Get new number of sources                                     *
    // *************************************************************************

    oldNSources = idListGetNElements(&myGSS->sourcesIDNow);
    newNSources = oldNSources + 1;

    // *************************************************************************
    // * STEP 2: Change dimensions of matrices                                 *
    // *************************************************************************

    // +-----------------------------------------------------------------------+
    // | Step A: Insert row/column in recursive matrices                       |
    // +-----------------------------------------------------------------------+

    // Since these matrices are recursive, the old content needs to be
    // preserved. This is the reason why insert operations are performed.

    // Insert a new row in y(k)
    matrixInsertRow(myGSS->y,oldNSources);
    matrixInsertRow(myGSS->yFull,oldNSources);

    // Insert a new row in W^n(k)
    matrixInsertRow(myGSS->Wn,oldNSources);

    // +-----------------------------------------------------------------------+
    // | Step B: Resize non-recursive matrices                                 |
    // +-----------------------------------------------------------------------+

    // Since these matrices are NOT recursive, the old content can be discarded.
    // For this reason, and simple resize operation is performed. This operation
    // is faster than an insert operation.

    // Resize the matrix yH(k): 1 x nSources
    matrixResize(myGSS->yH,1,newNSources,myGSS->GSS_HALFNFRAMESPLUSONE);

    // Resize the matrix Ryy_E(k): nSources x nSources
    matrixResize(myGSS->Ryy_E,newNSources,newNSources,myGSS->GSS_HALFNFRAMESPLUSONE);

    // Resize the matrix A(k): nMics x nSources
    matrixResize(myGSS->A,myGSS->myMicrophones->nMics,newNSources,myGSS->GSS_HALFNFRAMESPLUSONE);

    // Resize the matrix AH(k): nMics x nSources
    matrixResize(myGSS->AH,newNSources,myGSS->myMicrophones->nMics,myGSS->GSS_HALFNFRAMESPLUSONE);

    // Resize the matrix W^n(k): nSources x nMics
    // Tmp1: nSources x nMics
    matrixResize(myGSS->Wntmp1,newNSources,myGSS->myMicrophones->nMics,myGSS->GSS_HALFNFRAMESPLUSONE);

    // Resize the matrix dJ1(k): nSources x nMics
    matrixResize(myGSS->dJ1,newNSources,myGSS->myMicrophones->nMics,myGSS->GSS_HALFNFRAMESPLUSONE);
    // Tmp1: nSources x nSources
    matrixResize(myGSS->dJ1tmp1,newNSources,newNSources,myGSS->GSS_HALFNFRAMESPLUSONE);
    // Tmp2: nSources x nMics
    matrixResize(myGSS->dJ1tmp2,newNSources,myGSS->myMicrophones->nMics,myGSS->GSS_HALFNFRAMESPLUSONE);
    // Tmp3: nMics x 1
    matrixResize(myGSS->dJ1tmp3,myGSS->myMicrophones->nMics,1,myGSS->GSS_HALFNFRAMESPLUSONE);
    // Tmp4: nSources x nMics
    matrixResize(myGSS->dJ1tmp4,newNSources,myGSS->myMicrophones->nMics,myGSS->GSS_HALFNFRAMESPLUSONE);

    // Resize the matrix dJ2(k): nSources x nMics
    matrixResize(myGSS->dJ2,newNSources,myGSS->myMicrophones->nMics,myGSS->GSS_HALFNFRAMESPLUSONE);
    // Tmp1: nSources x nSources
    matrixResize(myGSS->dJ2tmp1,newNSources,newNSources,myGSS->GSS_HALFNFRAMESPLUSONE);
    // Tmp2: nSources x nMics
    matrixResize(myGSS->dJ2tmp2,newNSources,myGSS->myMicrophones->nMics,myGSS->GSS_HALFNFRAMESPLUSONE);

    // Resize the matrix dJR'(k); nSources x nMics
    matrixResize(myGSS->dJR,newNSources,myGSS->myMicrophones->nMics,myGSS->GSS_HALFNFRAMESPLUSONE);

    // *************************************************************************
    // * STEP 3: Update list of IDs and positions                              *
    // *************************************************************************

    // Add IDs to list
    indexID = idListAdd(&myGSS->sourcesIDNow, newID);
    if (indexID != -1)
    {
        myGSS->sourcesPosition[indexID][0] = positionX;
        myGSS->sourcesPosition[indexID][1] = positionY;
        myGSS->sourcesPosition[indexID][2] = positionZ;
    }

    // *************************************************************************
    // * STEP 4: Update the matrix Wn(k)                                       *
    // *************************************************************************

    // Normalize the position of the source

    xNotNorm = myGSS->sourcesPosition[indexID][0];
    yNotNorm = myGSS->sourcesPosition[indexID][1];
    zNotNorm = myGSS->sourcesPosition[indexID][2];

    r = sqrtf(xNotNorm * xNotNorm + yNotNorm * yNotNorm + zNotNorm * zNotNorm);

    xNorm = xNotNorm / (r + 1E-10f);
    yNorm = yNotNorm / (r + 1E-10f);
    zNorm = zNotNorm / (r + 1E-10f);

    // Set at the defined distance

    x = myGSS->GSS_SOURCEDISTANCE * xNorm;
    y = myGSS->GSS_SOURCEDISTANCE * yNorm;
    z = myGSS->GSS_SOURCEDISTANCE * zNorm;

    for (indexMicrophone = 0; indexMicrophone < myGSS->myMicrophones->nMics; indexMicrophone++)
    {

        // Compute the distance between the source and the microphone
        distanceX = microphonesGetPositionX(myGSS->myMicrophones,indexMicrophone) - x;
        distanceY = microphonesGetPositionY(myGSS->myMicrophones,indexMicrophone) - y;
        distanceZ = microphonesGetPositionZ(myGSS->myMicrophones,indexMicrophone) - z;

        distance = sqrtf(distanceX * distanceX + distanceY * distanceY + distanceZ * distanceZ);

        // Compute the delay for sound to reach the microphone from the source
        delay = (unsigned int) floor(((myGSS->GSS_FS / myGSS->GSS_C) * distance) + 0.5);

        // Remove the offset to avoid delaying the samples by too much if not needed
        delayNorm = delay - myGSS->delayOffset;

        for (k = 0; k < myGSS->GSS_HALFNFRAMESPLUSONE; k++)
        {

            // Compute aij(k)   = exp(-j * 2 * pi * k * delay / nFrames)
            //                  = cos(2 * pi * k * delay / nFrames) - j * sin(2 * pi * k * delay / nFrames)
            aijReal = cosf(2.0f * M_PI * k * delayNorm / myGSS->GSS_NFRAMES);
            aijImag = -1.0f * sinf(2.0f * M_PI * k * delayNorm / myGSS->GSS_NFRAMES);

            // Compute wji(k) = aij*(k) / nMics
            wjiReal = aijReal / myGSS->myMicrophones->nMics;
            wjiImag = -1.0f * aijImag / myGSS->myMicrophones->nMics;

            // Save in the matrix Wn(k)
            matrixSetReal(myGSS->Wn,oldNSources,indexMicrophone,k,wjiReal);
            matrixSetImag(myGSS->Wn,oldNSources,indexMicrophone,k,wjiImag);

        }

    }

}

/*******************************************************************************
 * gssDeleteSource                                                             *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:  myGSS           The gss object used in which a source is deleted   *
 *                                                                             *
 * Outputs: myGSS           The gss object with the source deleted             *
 *                                                                             *
 * Description: This function removes a new source from the gss object.        *
 *                                                                             *
 ******************************************************************************/

void gssDeleteSource(struct objGSS* myGSS, ID_TYPE deleteID)
{

    signed int indexID;

    unsigned int indexElement;
    unsigned int newNSources;

    // *************************************************************************
    // * STEP 1: Delete the ID                                                 *
    // *************************************************************************

    indexID = idListDelete(&myGSS->sourcesIDNow, deleteID);
    newNSources = idListGetMaxNElements(&myGSS->sourcesIDNow);

    // *************************************************************************
    // * STEP 2: Update positions                                              *
    // *************************************************************************

    for (indexElement = (unsigned int) indexID; indexElement < (newNSources - 1); indexElement++)
    {
        myGSS->sourcesPosition[indexElement][0] = myGSS->sourcesPosition[indexElement + 1][0];
        myGSS->sourcesPosition[indexElement][1] = myGSS->sourcesPosition[indexElement + 1][1];
        myGSS->sourcesPosition[indexElement][2] = myGSS->sourcesPosition[indexElement + 1][2];
    }

    // *************************************************************************
    // * STEP 3: Change dimensions of matrices                                 *
    // *************************************************************************

    // +-----------------------------------------------------------------------+
    // | Step A: Delete row/column in recursive matrices                       |
    // +-----------------------------------------------------------------------+

    // Since these matrices are recursive, the old content needs to be
    // preserved. This is the reason why delete operations are performed.

    // Remove a row from y(k)
    matrixDeleteRow(myGSS->y,indexID);
    matrixDeleteRow(myGSS->yFull,indexID);

    // Remove a row from W^n(k)
    matrixDeleteRow(myGSS->Wn,indexID);

    // +-----------------------------------------------------------------------+
    // | Step B: Resize non-recursive matrices                                 |
    // +-----------------------------------------------------------------------+

    // Since these matrices are NOT recursive, the old content can be discarded.
    // For this reason, and simple resize operation is performed. This operation
    // is faster than a delete operation.

    // Resize the matrix yH(k): nSources x 1
    matrixResize(myGSS->yH,1,newNSources,myGSS->GSS_HALFNFRAMESPLUSONE);

    // Resize the matrix Ryy(k): nSources x nSources
    matrixResize(myGSS->Ryy_E,newNSources,newNSources,myGSS->GSS_HALFNFRAMESPLUSONE);

    // Resize the matrix A(k): nMics x nSources
    matrixResize(myGSS->A,myGSS->myMicrophones->nMics,newNSources,myGSS->GSS_HALFNFRAMESPLUSONE);

    // Resize the matrix AH(k): nMics x nSources
    matrixResize(myGSS->AH,newNSources,myGSS->myMicrophones->nMics,myGSS->GSS_HALFNFRAMESPLUSONE);

    // Resize the matrix W^n(k): nSources x nMics
    // Tmp1: nSources x nMics
    matrixResize(myGSS->Wntmp1,newNSources,myGSS->myMicrophones->nMics,myGSS->GSS_HALFNFRAMESPLUSONE);

    // Resize the matrix dJ1(k): nSources x nMics
    matrixResize(myGSS->dJ1,newNSources,myGSS->myMicrophones->nMics,myGSS->GSS_HALFNFRAMESPLUSONE);
    // Tmp1: nSources x nSources
    matrixResize(myGSS->dJ1tmp1,newNSources,newNSources,myGSS->GSS_HALFNFRAMESPLUSONE);
    // Tmp2: nSources x nMics
    matrixResize(myGSS->dJ1tmp2,newNSources,myGSS->myMicrophones->nMics,myGSS->GSS_HALFNFRAMESPLUSONE);
    // Tmp3: nMics x 1
    matrixResize(myGSS->dJ1tmp3,myGSS->myMicrophones->nMics,1,myGSS->GSS_HALFNFRAMESPLUSONE);
    // Tmp4: nSources x nMics
    matrixResize(myGSS->dJ1tmp4,newNSources,myGSS->myMicrophones->nMics,myGSS->GSS_HALFNFRAMESPLUSONE);

    // Resize the matrix dJ2(k): nSources x nMics
    matrixResize(myGSS->dJ2,newNSources,myGSS->myMicrophones->nMics,myGSS->GSS_HALFNFRAMESPLUSONE);
    // Tmp1: nSources x nSources
    matrixResize(myGSS->dJ2tmp1,newNSources,newNSources,myGSS->GSS_HALFNFRAMESPLUSONE);
    // Tmp2: nSources x nMics
    matrixResize(myGSS->dJ2tmp2,newNSources,myGSS->myMicrophones->nMics,myGSS->GSS_HALFNFRAMESPLUSONE);

}

/*******************************************************************************
 * gssUpdateSource                                                             *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:  myGSS           The gss object used in which a source position is  *
 *                          updated                                            *
 *                                                                             *
 * Outputs: myGSS           The gss object with the updated source position    *
 *                                                                             *
 * Description: This function updates the position of a source.                *
 *                                                                             *
 ******************************************************************************/

void gssUpdateSource(struct objGSS* myGSS, ID_TYPE sourceID, float positionX, float positionY, float positionZ)
{

    signed int indexElement;

    // *************************************************************************
    // * STEP 1: Find the position that matches the ID                         *
    // *************************************************************************

    indexElement = idListGetIndex(&myGSS->sourcesIDNow, sourceID);

    // *************************************************************************
    // * STEP 2: Update the position of the corresponding source               *
    // *************************************************************************

    if (indexElement != ID_NOTFOUND)
    {

        // Update the position values
        myGSS->sourcesPosition[indexElement][0] = positionX;
        myGSS->sourcesPosition[indexElement][1] = positionY;
        myGSS->sourcesPosition[indexElement][2] = positionZ;

    }

}

/*******************************************************************************
 * gssRefreshSources                                                           *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:  myGSS            The gss object which sources need to be refreshed *
 *          myTrackedSources The tracked sources                               *
 *                                                                             *
 * Outputs: myGSS            The gss object with the source deleted            *
 *                                                                             *
 * Description: This function removes a new source from the gss object.        *
 *                                                                             *
 ******************************************************************************/

void gssRefreshSources(struct objGSS* myGSS, struct objTrackedSources* myTrackedSources)
{

    unsigned int indexSource;

    ID_TYPE currentID;
    unsigned int currentIndex;

    // *************************************************************************
    // * STEP 1: Find sources to be deleted and added                          *
    // *************************************************************************

    idListCompare(&myGSS->sourcesIDNow, &myTrackedSources->sourcesID, &myGSS->sourcesIDAdded, &myGSS->sourcesIDDeleted);

    // *************************************************************************
    // * STEP 2: Delete sources                                                *
    // *************************************************************************

    for (indexSource = 0; indexSource < idListGetNElements(&myGSS->sourcesIDDeleted); indexSource++)
    {       
        currentID = idListGetID(&myGSS->sourcesIDDeleted, indexSource);
        gssDeleteSource(myGSS, currentID);
    }

    // *************************************************************************
    // * STEP 3: Add sources                                                   *
    // *************************************************************************

    for (indexSource = 0; indexSource < idListGetNElements(&myGSS->sourcesIDAdded); indexSource++)
    {
        currentID = idListGetID(&myGSS->sourcesIDAdded, indexSource);
        currentIndex = idListGetIndex(&myTrackedSources->sourcesID, currentID);

        gssAddSource(myGSS, currentID,
                     myTrackedSources->sourcesPosition[currentIndex][0],
                     myTrackedSources->sourcesPosition[currentIndex][1],
                     myTrackedSources->sourcesPosition[currentIndex][2]);
    }

    // *************************************************************************
    // * STEP 4: Update positions                                              *
    // *************************************************************************

    for (indexSource = 0; indexSource < idListGetNElements(&myTrackedSources->sourcesID); indexSource++)
    {
        currentID = idListGetID(&myTrackedSources->sourcesID, indexSource);
        currentIndex = idListGetIndex(&myTrackedSources->sourcesID, currentID);

        gssUpdateSource(myGSS, currentID,
                        myTrackedSources->sourcesPosition[currentIndex][0],
                        myTrackedSources->sourcesPosition[currentIndex][1],
                        myTrackedSources->sourcesPosition[currentIndex][2]);

    }
}

/*******************************************************************************
 * gssProcess                                                                  *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:  myGSS           The gss object used to perform separation          *
 *          myPreprocessor  The preprocessor object which contains frames that *
 *                          are used for this operation                        *
 *                                                                             *
 * Outputs: mySeparatedSources  The object that contains the separated sources *
 *                                                                             *
 * Description: This function performs the separation operation                *
 *                                                                             *
 ******************************************************************************/

void gssProcess(struct objGSS* myGSS, struct objPreprocessor* myPreprocessor, struct objTrackedSources* myTrackedSources, struct objSeparatedSources* mySeparatedSources)
{

    unsigned int indexSource;
    signed int indexID;
    unsigned int indexMicrophone;
    unsigned int k;

    float x,y,z;
    float r;
    float xNotNorm,yNotNorm,zNotNorm;
    float xNorm,yNorm,zNorm;
    float distanceX,distanceY,distanceZ;
    float distance;

    unsigned int delay;
    unsigned int delayNorm;

    float aijReal;
    float aijImag;

    float scalar;

    unsigned int nSources;

#ifdef USE_SIMD

    // SIMD registers
    __m128_mod regA, regB, regC;

#endif

    // *************************************************************************
    // * STEP 0: Load tracked sources                                          *
    // *************************************************************************

    gssRefreshSources(myGSS, myTrackedSources);

    nSources = idListGetNElements(&myGSS->sourcesIDNow);

    // *************************************************************************
    // * STEP 1: Load frames in x(k)                                           *
    // *************************************************************************

    for (indexMicrophone = 0; indexMicrophone < myGSS->myMicrophones->nMics; indexMicrophone++)
    {

#ifndef USE_SIMD

        for (k = 0; k < myGSS->GSS_HALFNFRAMESPLUSONE; k++)
        {
            // Load x(k)
            matrixSetReal(myGSS->x, indexMicrophone, 0, k, myPreprocessor->micArray[indexMicrophone]->xfreqReal[k]);
            matrixSetImag(myGSS->x, indexMicrophone, 0, k, myPreprocessor->micArray[indexMicrophone]->xfreqImag[k]);
        }

#else

        for (k = 0; k < myGSS->GSS_HALFNFRAMES; k+=4)
        {
            // Load x(k)
            regA.m128 = _mm_load_ps(&myPreprocessor->micArray[indexMicrophone]->xfreqReal[k]);
            regB.m128 = _mm_load_ps(&myPreprocessor->micArray[indexMicrophone]->xfreqImag[k]);
            _mm_store_ps(&myGSS->x->valueReal[indexMicrophone][0][k], regA.m128);
            _mm_store_ps(&myGSS->x->valueImag[indexMicrophone][0][k], regB.m128);

        }

        // Load x(k)
        matrixSetReal(myGSS->x, indexMicrophone, 0, myGSS->GSS_HALFNFRAMES, myPreprocessor->micArray[indexMicrophone]->xfreqReal[myGSS->GSS_HALFNFRAMES]);
        matrixSetImag(myGSS->x, indexMicrophone, 0, myGSS->GSS_HALFNFRAMES, myPreprocessor->micArray[indexMicrophone]->xfreqImag[myGSS->GSS_HALFNFRAMES]);


#endif

    }

    // *************************************************************************
    // * STEP 2: Compute matrices                                              *
    // *************************************************************************

    if (nSources > 0)
    {

        // +-----------------------------------------------------------------------+
        // | Step A: Compute x(k)                                                  |
        // +-----------------------------------------------------------------------+

        matrixHermitian(myGSS->x,myGSS->xH);

        // +-----------------------------------------------------------------------+
        // | Step B: Compute yH(k)                                                 |
        // +-----------------------------------------------------------------------+

        matrixHermitian(myGSS->y,myGSS->yH);

        // +-----------------------------------------------------------------------+
        // | Step C: Compute Ryy(k)                                                |
        // +-----------------------------------------------------------------------+

        matrixMultMatrix(myGSS->y,myGSS->yH,myGSS->Ryy_E);

        // +-----------------------------------------------------------------------+
        // | Step D: Compute A(k) and AH(k)                                        |
        // +-----------------------------------------------------------------------+

    #ifdef USE_SIMD

        // Load the constant -1.0
        regA.m128_f32[0] = -1.0;
        regA.m128_f32[1] = -1.0;
        regA.m128_f32[2] = -1.0;
        regA.m128_f32[3] = -1.0;

    #endif

        for (indexSource = 0; indexSource < nSources; indexSource++)
        {

            // Normalize the position of the source

            xNotNorm = myGSS->sourcesPosition[indexSource][0];
            yNotNorm = myGSS->sourcesPosition[indexSource][1];
            zNotNorm = myGSS->sourcesPosition[indexSource][2];

            r = sqrtf(xNotNorm * xNotNorm + yNotNorm * yNotNorm + zNotNorm * zNotNorm);

            xNorm = xNotNorm / (r + 1E-10f);
            yNorm = yNotNorm / (r + 1E-10f);
            zNorm = zNotNorm / (r + 1E-10f);

            // Set at the defined distance

            x = myGSS->GSS_SOURCEDISTANCE * xNorm;
            y = myGSS->GSS_SOURCEDISTANCE * yNorm;
            z = myGSS->GSS_SOURCEDISTANCE * zNorm;

            for (indexMicrophone = 0; indexMicrophone < myGSS->myMicrophones->nMics; indexMicrophone++)
            {

                // Compute the distance between the source and the microphone
                distanceX = microphonesGetPositionX(myGSS->myMicrophones,indexMicrophone) - x;
                distanceY = microphonesGetPositionY(myGSS->myMicrophones,indexMicrophone) - y;
                distanceZ = microphonesGetPositionZ(myGSS->myMicrophones,indexMicrophone) - z;

                distance = sqrtf(distanceX * distanceX + distanceY * distanceY + distanceZ * distanceZ);

                // Compute the delay for sound to reach the microphone from the source
                delay = (unsigned int) floor(((myGSS->GSS_FS / myGSS->GSS_C) * distance) + 0.5);

                // Remove the offset to avoid delaying the samples by too much if not needed
                delayNorm = delay - myGSS->delayOffset;

    #ifndef USE_SIMD

                for (k = 0; k < myGSS->GSS_HALFNFRAMESPLUSONE; k++)
                {

                    // Compute aij(k)   = exp(-j*2*pi*k*delay/N)
                    //                  = cos(2*pi*k*delay/N) - j * sin(2*pi*k*delay/N)
                    aijReal = myGSS->cosTable[delayNorm][k];
                    aijImag = myGSS->sinTable[delayNorm][k];

                    // Save in the matrix A(k)
                    matrixSetReal(myGSS->A,indexMicrophone,indexSource,k,aijReal);
                    matrixSetImag(myGSS->A,indexMicrophone,indexSource,k,aijImag);

                    // Save in the matrix AH(k)
                    matrixSetReal(myGSS->AH,indexSource,indexMicrophone,k,aijReal);
                    matrixSetImag(myGSS->AH,indexSource,indexMicrophone,k,-1.0f * aijImag);

                }

    #else

                for (k = 0; k < myGSS->GSS_HALFNFRAMES; k+=4)
                {

                    // Compute aij(k)   = exp(-j*2*pi*k*delay/N)
                    //                  = cos(2*pi*k*delay/N) - j * sin(2*pi*k*delay/N)
                    regB.m128 = _mm_load_ps(&myGSS->cosTable[delayNorm][k]);
                    regC.m128 = _mm_load_ps(&myGSS->sinTable[delayNorm][k]);

                    // Save in the matrix A(k)
                    _mm_store_ps(&myGSS->A->valueReal[indexMicrophone][indexSource][k], regB.m128);
                    _mm_store_ps(&myGSS->A->valueImag[indexMicrophone][indexSource][k], regC.m128);

                    // Compute complex conjugate
                    regC.m128 = _mm_mul_ps(regA.m128,regC.m128);

                    // Save in the matrix AH(k)
                    _mm_store_ps(&myGSS->AH->valueReal[indexSource][indexMicrophone][k], regB.m128);
                    _mm_store_ps(&myGSS->AH->valueImag[indexSource][indexMicrophone][k], regC.m128);

                }

                // Compute aij(k)   = exp(-j*2*pi*k*delay/N)
                //                  = cos(2*pi*k*delay/N) - j * sin(2*pi*k*delay/N)
                aijReal = myGSS->cosTable[delayNorm][myGSS->GSS_HALFNFRAMES];
                aijImag = myGSS->sinTable[delayNorm][myGSS->GSS_HALFNFRAMES];

                // Save in the matrix A(k)
                matrixSetReal(myGSS->A,indexMicrophone,indexSource,myGSS->GSS_HALFNFRAMES,aijReal);
                matrixSetImag(myGSS->A,indexMicrophone,indexSource,myGSS->GSS_HALFNFRAMES,aijImag);

                // Save in the matrix AH(k)
                matrixSetReal(myGSS->AH,indexSource,indexMicrophone,myGSS->GSS_HALFNFRAMES,aijReal);
                matrixSetImag(myGSS->AH,indexSource,indexMicrophone,myGSS->GSS_HALFNFRAMES,-1.0 * aijImag);

    #endif

            }

        }

        // +-----------------------------------------------------------------------+
        // | Step E: Compute E(k)                                                  |
        // +-----------------------------------------------------------------------+

        // E = Ryy - diag(Ryy)
        matrixRemoveDiagonal(myGSS->Ryy_E);

        // +-----------------------------------------------------------------------+
        // | Step F: Compute dJ1(k)                                                |
        // +-----------------------------------------------------------------------+

            // +-------------------------------------------------------------------+
            // | Step i: Compute alpha(k)                                          |
            // +-------------------------------------------------------------------+

            matrixMultMatrix(myGSS->xH,myGSS->x,myGSS->alpha);
            matrixMultMatrixPerElement(myGSS->alpha,myGSS->alpha,myGSS->alphatmp);
            matrixMakeNonZero(myGSS->alphatmp);
            matrixInvRealPerElement(myGSS->alphatmp,myGSS->alpha);

            // +-------------------------------------------------------------------+
            // | Step ii: Compute matrices                                         |
            // +-------------------------------------------------------------------+

            // -4 * nSources * mu
            scalar = -4.0f * nSources * myGSS->GSS_MU;

            // -4 * nSources * mu * E(k)
            matrixMultScalar(myGSS->Ryy_E,scalar,myGSS->dJ1tmp1);

            // (-4 * nSources * mu * E(k)) * W(k)
            matrixMultMatrix(myGSS->dJ1tmp1,myGSS->Wn,myGSS->dJ1tmp2);

            // ((-4 * nSources * mu * E(k)) * W(k)) * x(k)
            matrixMultMatrix(myGSS->dJ1tmp2,myGSS->x,myGSS->dJ1tmp3);

            // (((-4 * nSources * mu * E(k)) * W(k)) * x(k)) * xH(k)
            matrixMultMatrix(myGSS->dJ1tmp3,myGSS->xH,myGSS->dJ1tmp4);

            // alpha(k) * [(((-4 * nSources * mu * E(k)) * W(k)) * x(k)) * xH(k)]
            matrixMultScalarPerFrame(myGSS->dJ1tmp4,myGSS->alpha,myGSS->dJ1);

        // +-----------------------------------------------------------------------+
        // | Step G: Compute dJ2(k)                                                |
        // +-----------------------------------------------------------------------+

        // W(k) * A(k)
        matrixMultMatrix(myGSS->Wn,myGSS->A,myGSS->dJ2tmp1);

        // W(k) * A(k) - I
        matrixRemoveIdentity(myGSS->dJ2tmp1);

        // (W(k) * A(k) - I) * AH(k)
        matrixMultMatrix(myGSS->dJ2tmp1,myGSS->AH,myGSS->dJ2tmp2);

        // -2 * (1/nSources) * mu
        scalar = -2.0f * (1.0f/((float) nSources)) * myGSS->GSS_MU;

        // 2 * (1/nSources) * mu * (W(k) * A(k) - I) * AH(k)
        matrixMultScalar(myGSS->dJ2tmp2,scalar,myGSS->dJ2);

        // +-----------------------------------------------------------------------+
        // | Step H: Compute dJR'(k)                                               |
        // +-----------------------------------------------------------------------+

        // (1 - lambda * mu)
        scalar = 1.0f - myGSS->GSS_LAMBDA * myGSS->GSS_MU;

        // (1 - lambda * mu) * Wn(k)
        matrixMultScalar(myGSS->Wn,scalar,myGSS->dJR);

        // +-----------------------------------------------------------------------+
        // | Step I: Compute Wn+1(k)                                               |
        // +-----------------------------------------------------------------------+

        // dJ1 + dJ2
        matrixAddMatrix(myGSS->dJ1,myGSS->dJ2,myGSS->Wntmp1);

        // dJ1 + dJ2 + dJR'
        matrixAddMatrix(myGSS->Wntmp1,myGSS->dJR,myGSS->Wn);

        // +-----------------------------------------------------------------------+
        // | Step J: Compute y(k)                                                  |
        // +-----------------------------------------------------------------------+

        // W(k) * x(k)
        matrixMultMatrix(myGSS->Wn,myGSS->x,myGSS->y);

    }

    // *************************************************************************
    // * STEP 3: Compute y(k) with all k's                                     *
    // *************************************************************************

    for (indexSource = 0; indexSource < nSources; indexSource++)
    {

        for (k = 1; k < myGSS->GSS_HALFNFRAMES; k++)
        {
            matrixSetReal(myGSS->yFull,indexSource, 0, k, matrixGetReal(myGSS->y, indexSource, 0, k));
            matrixSetImag(myGSS->yFull,indexSource, 0, k, matrixGetImag(myGSS->y, indexSource, 0, k));
            matrixSetReal(myGSS->yFull,indexSource, 0, (myGSS->GSS_NFRAMES - k), matrixGetReal(myGSS->y, indexSource, 0, k));
            matrixSetImag(myGSS->yFull,indexSource, 0, (myGSS->GSS_NFRAMES - k), -1.0f * matrixGetImag(myGSS->y, indexSource, 0, k));
        }

        matrixSetReal(myGSS->yFull, indexSource, 0, 0, matrixGetReal(myGSS->y, indexSource, 0, 0));
        matrixSetImag(myGSS->yFull, indexSource, 0, 0, matrixGetImag(myGSS->y, indexSource, 0, 0));

        matrixSetReal(myGSS->yFull, indexSource, 0, myGSS->GSS_HALFNFRAMES, matrixGetReal(myGSS->y, indexSource, 0, myGSS->GSS_HALFNFRAMES));
        matrixSetImag(myGSS->yFull, indexSource, 0, myGSS->GSS_HALFNFRAMES, matrixGetImag(myGSS->y, indexSource, 0, myGSS->GSS_HALFNFRAMES));


    }

    // *************************************************************************
    // * STEP 4: Export frames                                                 *
    // *************************************************************************

    // Reset list of IDs
    idListReset(&mySeparatedSources->sourcesID);

    // Then load sources
    for (indexSource = 0; indexSource < nSources; indexSource++)
    {

        indexID = idListAdd(&mySeparatedSources->sourcesID, idListGetID(&myGSS->sourcesIDNow, indexSource));

#ifndef USE_SIMD

        for (k = 0; k < myGSS->GSS_NFRAMES; k++)
        {

            // Export y(k)
            mySeparatedSources->sourcesFramesReal[indexID][k] = matrixGetReal(myGSS->yFull, indexID, 0, k);
            mySeparatedSources->sourcesFramesImag[indexID][k] = matrixGetImag(myGSS->yFull, indexID, 0, k);

        }

#else

		/*
		//TODO : IS THIS USEFUL FRANCOIS?

        tmp = 0;

        for (k = 0; k < myGSS->GSS_NFRAMES; k++)
        {
            tmp += mySeparatedSources->sourcesFramesReal[indexID][k] * mySeparatedSources->sourcesFramesReal[indexID][k];
            tmp += mySeparatedSources->sourcesFramesImag[indexID][k] * mySeparatedSources->sourcesFramesImag[indexID][k];
        }

        if (tmp == 0)
        {
            tmp = 0;
        }
		*/

        for (k = 0; k < myGSS->GSS_NFRAMES; k+=4)
        {

            // Export y(k)

            regA.m128 = _mm_load_ps(&myGSS->yFull->valueReal[indexID][0][k]);
            regB.m128 = _mm_load_ps(&myGSS->yFull->valueImag[indexID][0][k]);
            _mm_store_ps(&mySeparatedSources->sourcesFramesReal[indexID][k], regA.m128);
            _mm_store_ps(&mySeparatedSources->sourcesFramesImag[indexID][k], regB.m128);

        }
        for (k = k - 4; k < myGSS->GSS_NFRAMES; k++)
        {

            // Export y(k)
            mySeparatedSources->sourcesFramesReal[indexID][k] = matrixGetReal(myGSS->yFull, indexID, 0, k);
            mySeparatedSources->sourcesFramesImag[indexID][k] = matrixGetImag(myGSS->yFull, indexID, 0, k);

        }
#endif

    }

}

