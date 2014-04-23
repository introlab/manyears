/******************************************************************************* 
 * ManyEars: Sphere - Source Code                                              *
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

#include "Localisation/sphere.h"

/*******************************************************************************
 * Compatibility issues                                                        *
 ******************************************************************************/

// In some environment, the constant PI is not define with the math.h library
#ifndef M_PI
	#define M_PI	3.1415926535897932384626433832795
#endif

/*******************************************************************************
 * sphereInit                                                                  *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      mySphere        The sphere object to be initialized            *
 *              numberLevels    The number of levels of iteration to generate  *
 *                              the sphere                                     *
 *                                                                             *
 * Outputs:     mySphere        The sphere initialized object                  *
 *                                                                             *
 * Description: This function generates the points that the sphere is made of  *
  *                                                                            *
 ******************************************************************************/

void sphereInit(struct objSphere *mySphere, unsigned int numberLevels)
{

    // Define an array to store all triangles
    // Each triangle is represented by a row of 9 points:
    //
    // +----------+----------+----------+
    // | x1 y1 z1 | x2 y2 z2 | x3 y3 z3 |
    // +----------+----------+----------+
    //
    /*
    // Where each set (x,y,z) represents one end of the triangle:
    //
    //               o (x1,y1,z1)
    //              / \
    //  (x2,y2,z2) o---o (x3,y3,z3)
    //
    */
    // This array is created dynamically since the number of
    // triangles may change
    float** sphereTriangles;

    // Define an array to store initial points for level 0
    // Each point is represented by a row of 3 points:
    //
    // +---------+
    // | x  y  z |
    // +---------+
    //
    // Define the initial points for level 0
    // There are 20 triangles, and thus 12 points
    float level0Points[12][3];

    // Define the step size when the array of triangles is being filled
    // For instance, say there are 4 levels, and thus 5120 triangles
    // For level 0, each triangle is spaced by 256 rows in the array
    // New triangles are then generated for level 1, and each new triangle is
    // then spaced by 64 rows
    // New triangles are then generated for level 2, and each new triangle is
    // then spaced by 16 rows
    // New triangles are then generated for level 3, and each new triangle is
    // then spaced by 4 rows
    // New triangles are then generated for level 4, and there is no more space
    // between triangles since the array is now full
    unsigned int stepSize;

    // To loop for each level, use the following index
    unsigned char indexLevel;

    // When the array of triangles is being filled, use the following index
    unsigned int indexRow;

    // To loop for each point in the sphere, use the following index
    unsigned int indexPointSphere;

    // To loop for each point in the triangle, use the following index
    unsigned int indexPointTriangle;

    // Use the following variable to keep track of the number of points
    unsigned int numberPoints;

    // Use the following variable to load the current point to be added
    float currentPoint[3];

    // Use the following variable to flag if the point is already in the list
    unsigned char pointAlreadyInList;

    /*
    // When the array of triangles is being filled, use the following variables
    // to generate new triangles:
    //
    //                  o Point 1
    //                 /A\
    //    Point 12--> x---x <--Point 13
    //               /B\C/D\
    //      Point 2 o---x---o Point 3
    //                  ^
    //                  |
    //              Point 23
    //
    */
    float point1[3];
    float point2[3];
    float point3[3];
    float point12[3];
    float point23[3];
    float point13[3];
    float mag12;
    float mag23;
    float mag13;
    unsigned int indexTriangleA;
    unsigned int indexTriangleB;
    unsigned int indexTriangleC;
    unsigned int indexTriangleD;

    // Define variable to store parameters to generate the triangles
    float sint;
    float cost;

    // *************************************************************************
    // * STEP 1: Initialize context                                            *
    // *************************************************************************

    // Number of recursive levels to generate the points on the sphere
    mySphere->SPHERE_NUMBERLEVELS = numberLevels;

    // Number of triangles according to the number of levels
    // SPHERE_NUMBERTRIANGLES = 20 * 4^(SPHERE_NUMBERLEVELS)
    mySphere->SPHERE_NUMBERTRIANGLES = (unsigned int) (20 * powf(4.0f, (float) mySphere->SPHERE_NUMBERLEVELS));

    // Number of points according to the number of levels
    // SPHERE_NUMBERPOINTS = SPHERE_NUMBERTRIANGLES / 2 + 2
    mySphere->SPHERE_NUMBERPOINTS = mySphere->SPHERE_NUMBERTRIANGLES / 2 + 2;

    // Sphere
    mySphere->spherePoints = (float **) newTable2D(mySphere->SPHERE_NUMBERPOINTS,3,sizeof(float));

    // *************************************************************************
    // * STEP 2: Create dynamic arrays used only in this function              *
    // *************************************************************************

    sphereTriangles = (float **) newTable2D(mySphere->SPHERE_NUMBERTRIANGLES,9,sizeof(float));

    // *************************************************************************
    // * STEP 3: Compute parameters                                            *
    // *************************************************************************

    // Define variable to store parameters to generate the triangles
    sint = 0.4472136f;
    cost = 0.8944272f;

    // *************************************************************************
    // * STEP 4: Generate the 12 points and 20 triangles for level 0           *
    // *************************************************************************

        // Point at the top of the sphere
        level0Points[0][0] = 0;
        level0Points[0][1] = 0;
        level0Points[0][2] = 1;

        // Point at the bottom of the sphere
        level0Points[11][0] = 0;
        level0Points[11][1] = 0;
        level0Points[11][2] = -1;

        // Now create two circles on each hemisphere that will be divided in 5
        // points

        // Circle on top hemisphere
        level0Points[1][0] = cost * sinf(2.0f * M_PI * 0.0f / 5.0f);
        level0Points[1][1] = cost * cosf(2.0f * M_PI * 0.0f / 5.0f);
        level0Points[1][2] = sint;
        level0Points[2][0] = cost * sinf(2.0f * M_PI * 1.0f / 5.0f);
        level0Points[2][1] = cost * cosf(2.0f * M_PI * 1.0f / 5.0f);
        level0Points[2][2] = sint;
        level0Points[3][0] = cost * sinf(2.0f * M_PI * 2.0f / 5.0f);
        level0Points[3][1] = cost * cosf(2.0f * M_PI * 2.0f / 5.0f);
        level0Points[3][2] = sint;
        level0Points[4][0] = cost * sinf(2.0f * M_PI * 3.0f / 5.0f);
        level0Points[4][1] = cost * cosf(2.0f * M_PI * 3.0f / 5.0f);
        level0Points[4][2] = sint;
        level0Points[5][0] = cost * sinf(2.0f * M_PI * 4.0f / 5.0f);
        level0Points[5][1] = cost * cosf(2.0f * M_PI * 4.0f / 5.0f);
        level0Points[5][2] = sint;

        // Circle on bottom hemisphere
        level0Points[6][0] = -1.0f * cost * sinf(2.0f * M_PI * 0.0f / 5.0f);
        level0Points[6][1] = -1.0f * cost * cosf(2.0f * M_PI * 0.0f / 5.0f);
        level0Points[6][2] = -1.0f * sint;
        level0Points[7][0] = -1.0f * cost * sinf(2.0f * M_PI * 1.0f / 5.0f);
        level0Points[7][1] = -1.0f * cost * cosf(2.0f * M_PI * 1.0f / 5.0f);
        level0Points[7][2] = -1.0f * sint;
        level0Points[8][0] = -1.0f * cost * sinf(2.0f * M_PI * 2.0f / 5.0f);
        level0Points[8][1] = -1.0f * cost * cosf(2.0f * M_PI * 2.0f / 5.0f);
        level0Points[8][2] = -1.0f * sint;
        level0Points[9][0] = -1.0f * cost * sinf(2.0f * M_PI * 3.0f / 5.0f);
        level0Points[9][1] = -1.0f * cost * cosf(2.0f * M_PI * 3.0f / 5.0f);
        level0Points[9][2] = -1.0f * sint;
        level0Points[10][0] = -1.0f * cost * sinf(2.0f * M_PI * 4.0f / 5.0f);
        level0Points[10][1] = -1.0f * cost * cosf(2.0f * M_PI * 4.0f / 5.0f);
        level0Points[10][2] = -1.0f * sint;

    // Use these points to define the 20 triangles for level 0

        // First define the step size to fill the array
        stepSize = mySphere->SPHERE_NUMBERTRIANGLES / 20;

        // Fill the array

        sphereTriangles[0*stepSize][0] = level0Points[0][0];
        sphereTriangles[0*stepSize][1] = level0Points[0][1];
        sphereTriangles[0*stepSize][2] = level0Points[0][2];
        sphereTriangles[0*stepSize][3] = level0Points[2][0];
        sphereTriangles[0*stepSize][4] = level0Points[2][1];
        sphereTriangles[0*stepSize][5] = level0Points[2][2];
        sphereTriangles[0*stepSize][6] = level0Points[1][0];
        sphereTriangles[0*stepSize][7] = level0Points[1][1];
        sphereTriangles[0*stepSize][8] = level0Points[1][2];

        sphereTriangles[1*stepSize][0] = level0Points[0][0];
        sphereTriangles[1*stepSize][1] = level0Points[0][1];
        sphereTriangles[1*stepSize][2] = level0Points[0][2];
        sphereTriangles[1*stepSize][3] = level0Points[3][0];
        sphereTriangles[1*stepSize][4] = level0Points[3][1];
        sphereTriangles[1*stepSize][5] = level0Points[3][2];
        sphereTriangles[1*stepSize][6] = level0Points[2][0];
        sphereTriangles[1*stepSize][7] = level0Points[2][1];
        sphereTriangles[1*stepSize][8] = level0Points[2][2];

        sphereTriangles[2*stepSize][0] = level0Points[0][0];
        sphereTriangles[2*stepSize][1] = level0Points[0][1];
        sphereTriangles[2*stepSize][2] = level0Points[0][2];
        sphereTriangles[2*stepSize][3] = level0Points[4][0];
        sphereTriangles[2*stepSize][4] = level0Points[4][1];
        sphereTriangles[2*stepSize][5] = level0Points[4][2];
        sphereTriangles[2*stepSize][6] = level0Points[3][0];
        sphereTriangles[2*stepSize][7] = level0Points[3][1];
        sphereTriangles[2*stepSize][8] = level0Points[3][2];

        sphereTriangles[3*stepSize][0] = level0Points[0][0];
        sphereTriangles[3*stepSize][1] = level0Points[0][1];
        sphereTriangles[3*stepSize][2] = level0Points[0][2];
        sphereTriangles[3*stepSize][3] = level0Points[5][0];
        sphereTriangles[3*stepSize][4] = level0Points[5][1];
        sphereTriangles[3*stepSize][5] = level0Points[5][2];
        sphereTriangles[3*stepSize][6] = level0Points[4][0];
        sphereTriangles[3*stepSize][7] = level0Points[4][1];
        sphereTriangles[3*stepSize][8] = level0Points[4][2];

        sphereTriangles[4*stepSize][0] = level0Points[0][0];
        sphereTriangles[4*stepSize][1] = level0Points[0][1];
        sphereTriangles[4*stepSize][2] = level0Points[0][2];
        sphereTriangles[4*stepSize][3] = level0Points[1][0];
        sphereTriangles[4*stepSize][4] = level0Points[1][1];
        sphereTriangles[4*stepSize][5] = level0Points[1][2];
        sphereTriangles[4*stepSize][6] = level0Points[5][0];
        sphereTriangles[4*stepSize][7] = level0Points[5][1];
        sphereTriangles[4*stepSize][8] = level0Points[5][2];

        sphereTriangles[5*stepSize][0] = level0Points[9][0];
        sphereTriangles[5*stepSize][1] = level0Points[9][1];
        sphereTriangles[5*stepSize][2] = level0Points[9][2];
        sphereTriangles[5*stepSize][3] = level0Points[1][0];
        sphereTriangles[5*stepSize][4] = level0Points[1][1];
        sphereTriangles[5*stepSize][5] = level0Points[1][2];
        sphereTriangles[5*stepSize][6] = level0Points[2][0];
        sphereTriangles[5*stepSize][7] = level0Points[2][1];
        sphereTriangles[5*stepSize][8] = level0Points[2][2];

        sphereTriangles[6*stepSize][0] = level0Points[10][0];
        sphereTriangles[6*stepSize][1] = level0Points[10][1];
        sphereTriangles[6*stepSize][2] = level0Points[10][2];
        sphereTriangles[6*stepSize][3] = level0Points[2][0];
        sphereTriangles[6*stepSize][4] = level0Points[2][1];
        sphereTriangles[6*stepSize][5] = level0Points[2][2];
        sphereTriangles[6*stepSize][6] = level0Points[3][0];
        sphereTriangles[6*stepSize][7] = level0Points[3][1];
        sphereTriangles[6*stepSize][8] = level0Points[3][2];

        sphereTriangles[7*stepSize][0] = level0Points[6][0];
        sphereTriangles[7*stepSize][1] = level0Points[6][1];
        sphereTriangles[7*stepSize][2] = level0Points[6][2];
        sphereTriangles[7*stepSize][3] = level0Points[3][0];
        sphereTriangles[7*stepSize][4] = level0Points[3][1];
        sphereTriangles[7*stepSize][5] = level0Points[3][2];
        sphereTriangles[7*stepSize][6] = level0Points[4][0];
        sphereTriangles[7*stepSize][7] = level0Points[4][1];
        sphereTriangles[7*stepSize][8] = level0Points[4][2];

        sphereTriangles[8*stepSize][0] = level0Points[7][0];
        sphereTriangles[8*stepSize][1] = level0Points[7][1];
        sphereTriangles[8*stepSize][2] = level0Points[7][2];
        sphereTriangles[8*stepSize][3] = level0Points[4][0];
        sphereTriangles[8*stepSize][4] = level0Points[4][1];
        sphereTriangles[8*stepSize][5] = level0Points[4][2];
        sphereTriangles[8*stepSize][6] = level0Points[5][0];
        sphereTriangles[8*stepSize][7] = level0Points[5][1];
        sphereTriangles[8*stepSize][8] = level0Points[5][2];

        sphereTriangles[9*stepSize][0] = level0Points[8][0];
        sphereTriangles[9*stepSize][1] = level0Points[8][1];
        sphereTriangles[9*stepSize][2] = level0Points[8][2];
        sphereTriangles[9*stepSize][3] = level0Points[5][0];
        sphereTriangles[9*stepSize][4] = level0Points[5][1];
        sphereTriangles[9*stepSize][5] = level0Points[5][2];
        sphereTriangles[9*stepSize][6] = level0Points[1][0];
        sphereTriangles[9*stepSize][7] = level0Points[1][1];
        sphereTriangles[9*stepSize][8] = level0Points[1][2];

        sphereTriangles[10*stepSize][0] = level0Points[4][0];
        sphereTriangles[10*stepSize][1] = level0Points[4][1];
        sphereTriangles[10*stepSize][2] = level0Points[4][2];
        sphereTriangles[10*stepSize][3] = level0Points[7][0];
        sphereTriangles[10*stepSize][4] = level0Points[7][1];
        sphereTriangles[10*stepSize][5] = level0Points[7][2];
        sphereTriangles[10*stepSize][6] = level0Points[6][0];
        sphereTriangles[10*stepSize][7] = level0Points[6][1];
        sphereTriangles[10*stepSize][8] = level0Points[6][2];

        sphereTriangles[11*stepSize][0] = level0Points[5][0];
        sphereTriangles[11*stepSize][1] = level0Points[5][1];
        sphereTriangles[11*stepSize][2] = level0Points[5][2];
        sphereTriangles[11*stepSize][3] = level0Points[8][0];
        sphereTriangles[11*stepSize][4] = level0Points[8][1];
        sphereTriangles[11*stepSize][5] = level0Points[8][2];
        sphereTriangles[11*stepSize][6] = level0Points[7][0];
        sphereTriangles[11*stepSize][7] = level0Points[7][1];
        sphereTriangles[11*stepSize][8] = level0Points[7][2];

        sphereTriangles[12*stepSize][0] = level0Points[1][0];
        sphereTriangles[12*stepSize][1] = level0Points[1][1];
        sphereTriangles[12*stepSize][2] = level0Points[1][2];
        sphereTriangles[12*stepSize][3] = level0Points[9][0];
        sphereTriangles[12*stepSize][4] = level0Points[9][1];
        sphereTriangles[12*stepSize][5] = level0Points[9][2];
        sphereTriangles[12*stepSize][6] = level0Points[8][0];
        sphereTriangles[12*stepSize][7] = level0Points[8][1];
        sphereTriangles[12*stepSize][8] = level0Points[8][2];

        sphereTriangles[13*stepSize][0] = level0Points[2][0];
        sphereTriangles[13*stepSize][1] = level0Points[2][1];
        sphereTriangles[13*stepSize][2] = level0Points[2][2];
        sphereTriangles[13*stepSize][3] = level0Points[10][0];
        sphereTriangles[13*stepSize][4] = level0Points[10][1];
        sphereTriangles[13*stepSize][5] = level0Points[10][2];
        sphereTriangles[13*stepSize][6] = level0Points[9][0];
        sphereTriangles[13*stepSize][7] = level0Points[9][1];
        sphereTriangles[13*stepSize][8] = level0Points[9][2];

        sphereTriangles[14*stepSize][0] = level0Points[3][0];
        sphereTriangles[14*stepSize][1] = level0Points[3][1];
        sphereTriangles[14*stepSize][2] = level0Points[3][2];
        sphereTriangles[14*stepSize][3] = level0Points[6][0];
        sphereTriangles[14*stepSize][4] = level0Points[6][1];
        sphereTriangles[14*stepSize][5] = level0Points[6][2];
        sphereTriangles[14*stepSize][6] = level0Points[10][0];
        sphereTriangles[14*stepSize][7] = level0Points[10][1];
        sphereTriangles[14*stepSize][8] = level0Points[10][2];

        sphereTriangles[15*stepSize][0] = level0Points[11][0];
        sphereTriangles[15*stepSize][1] = level0Points[11][1];
        sphereTriangles[15*stepSize][2] = level0Points[11][2];
        sphereTriangles[15*stepSize][3] = level0Points[6][0];
        sphereTriangles[15*stepSize][4] = level0Points[6][1];
        sphereTriangles[15*stepSize][5] = level0Points[6][2];
        sphereTriangles[15*stepSize][6] = level0Points[7][0];
        sphereTriangles[15*stepSize][7] = level0Points[7][1];
        sphereTriangles[15*stepSize][8] = level0Points[7][2];

        sphereTriangles[16*stepSize][0] = level0Points[11][0];
        sphereTriangles[16*stepSize][1] = level0Points[11][1];
        sphereTriangles[16*stepSize][2] = level0Points[11][2];
        sphereTriangles[16*stepSize][3] = level0Points[7][0];
        sphereTriangles[16*stepSize][4] = level0Points[7][1];
        sphereTriangles[16*stepSize][5] = level0Points[7][2];
        sphereTriangles[16*stepSize][6] = level0Points[8][0];
        sphereTriangles[16*stepSize][7] = level0Points[8][1];
        sphereTriangles[16*stepSize][8] = level0Points[8][2];

        sphereTriangles[17*stepSize][0] = level0Points[11][0];
        sphereTriangles[17*stepSize][1] = level0Points[11][1];
        sphereTriangles[17*stepSize][2] = level0Points[11][2];
        sphereTriangles[17*stepSize][3] = level0Points[8][0];
        sphereTriangles[17*stepSize][4] = level0Points[8][1];
        sphereTriangles[17*stepSize][5] = level0Points[8][2];
        sphereTriangles[17*stepSize][6] = level0Points[9][0];
        sphereTriangles[17*stepSize][7] = level0Points[9][1];
        sphereTriangles[17*stepSize][8] = level0Points[9][2];

        sphereTriangles[18*stepSize][0] = level0Points[11][0];
        sphereTriangles[18*stepSize][1] = level0Points[11][1];
        sphereTriangles[18*stepSize][2] = level0Points[11][2];
        sphereTriangles[18*stepSize][3] = level0Points[9][0];
        sphereTriangles[18*stepSize][4] = level0Points[9][1];
        sphereTriangles[18*stepSize][5] = level0Points[9][2];
        sphereTriangles[18*stepSize][6] = level0Points[10][0];
        sphereTriangles[18*stepSize][7] = level0Points[10][1];
        sphereTriangles[18*stepSize][8] = level0Points[10][2];

        sphereTriangles[19*stepSize][0] = level0Points[11][0];
        sphereTriangles[19*stepSize][1] = level0Points[11][1];
        sphereTriangles[19*stepSize][2] = level0Points[11][2];
        sphereTriangles[19*stepSize][3] = level0Points[10][0];
        sphereTriangles[19*stepSize][4] = level0Points[10][1];
        sphereTriangles[19*stepSize][5] = level0Points[10][2];
        sphereTriangles[19*stepSize][6] = level0Points[6][0];
        sphereTriangles[19*stepSize][7] = level0Points[6][1];
        sphereTriangles[19*stepSize][8] = level0Points[6][2];

    // *************************************************************************
    // * STEP 5: Generate each subsequent level                                *
    // *************************************************************************

        // Loop for each level starting at 1
        for (indexLevel = 1; indexLevel <= mySphere->SPHERE_NUMBERLEVELS; indexLevel++)
        {
            // Loop for each row to be filled in the array
            for (indexRow = 0; indexRow < mySphere->SPHERE_NUMBERTRIANGLES; indexRow += stepSize)
            {
                // Get the current triangle
                point1[0] = sphereTriangles[indexRow][0];
                point1[1] = sphereTriangles[indexRow][1];
                point1[2] = sphereTriangles[indexRow][2];
                point2[0] = sphereTriangles[indexRow][3];
                point2[1] = sphereTriangles[indexRow][4];
                point2[2] = sphereTriangles[indexRow][5];
                point3[0] = sphereTriangles[indexRow][6];
                point3[1] = sphereTriangles[indexRow][7];
                point3[2] = sphereTriangles[indexRow][8];

                // Find the point that would lie on the sphere between points 1 & 2
                point12[0] = point1[0] + point2[0];
                point12[1] = point1[1] + point2[1];
                point12[2] = point1[2] + point2[2];
                mag12 = sqrtf(point12[0] * point12[0] + point12[1] * point12[1] + point12[2] * point12[2]);
                point12[0] /= mag12;
                point12[1] /= mag12;
                point12[2] /= mag12;

                // Find the point that would lie on the sphere between points 2 & 3
                point23[0] = point2[0] + point3[0];
                point23[1] = point2[1] + point3[1];
                point23[2] = point2[2] + point3[2];
                mag23 = sqrtf(point23[0] * point23[0] + point23[1] * point23[1] + point23[2] * point23[2]);
                point23[0] /= mag23;
                point23[1] /= mag23;
                point23[2] /= mag23;

                // Find the point that would lie on the sphere between points 1 & 3
                point13[0] = point1[0] + point3[0];
                point13[1] = point1[1] + point3[1];
                point13[2] = point1[2] + point3[2];
                mag13 = sqrtf(point13[0] * point13[0] + point13[1] * point13[1] + point13[2] * point13[2]);
                point13[0] /= mag13;
                point13[1] /= mag13;
                point13[2] /= mag13;

                // Create new triangles index in the array
                // Note that the big triangle that is splitted in four triangles will
                // be replaced in the list by triangle A
                indexTriangleA = indexRow + 0 * stepSize / 4;
                indexTriangleB = indexRow + 1 * stepSize / 4;
                indexTriangleC = indexRow + 2 * stepSize / 4;
                indexTriangleD = indexRow + 3 * stepSize / 4;

                // Copy the new values of triangle A
                sphereTriangles[indexTriangleA][0] = point1[0];
                sphereTriangles[indexTriangleA][1] = point1[1];
                sphereTriangles[indexTriangleA][2] = point1[2];
                sphereTriangles[indexTriangleA][3] = point12[0];
                sphereTriangles[indexTriangleA][4] = point12[1];
                sphereTriangles[indexTriangleA][5] = point12[2];
                sphereTriangles[indexTriangleA][6] = point13[0];
                sphereTriangles[indexTriangleA][7] = point13[1];
                sphereTriangles[indexTriangleA][8] = point13[2];

                // Copy the new values of triangle B
                sphereTriangles[indexTriangleB][0] = point12[0];
                sphereTriangles[indexTriangleB][1] = point12[1];
                sphereTriangles[indexTriangleB][2] = point12[2];
                sphereTriangles[indexTriangleB][3] = point2[0];
                sphereTriangles[indexTriangleB][4] = point2[1];
                sphereTriangles[indexTriangleB][5] = point2[2];
                sphereTriangles[indexTriangleB][6] = point23[0];
                sphereTriangles[indexTriangleB][7] = point23[1];
                sphereTriangles[indexTriangleB][8] = point23[2];

                // Copy the new values of triangle C
                sphereTriangles[indexTriangleC][0] = point12[0];
                sphereTriangles[indexTriangleC][1] = point12[1];
                sphereTriangles[indexTriangleC][2] = point12[2];
                sphereTriangles[indexTriangleC][3] = point23[0];
                sphereTriangles[indexTriangleC][4] = point23[1];
                sphereTriangles[indexTriangleC][5] = point23[2];
                sphereTriangles[indexTriangleC][6] = point13[0];
                sphereTriangles[indexTriangleC][7] = point13[1];
                sphereTriangles[indexTriangleC][8] = point13[2];

                // Copy the new values of triangle D
                sphereTriangles[indexTriangleD][0] = point13[0];
                sphereTriangles[indexTriangleD][1] = point13[1];
                sphereTriangles[indexTriangleD][2] = point13[2];
                sphereTriangles[indexTriangleD][3] = point23[0];
                sphereTriangles[indexTriangleD][4] = point23[1];
                sphereTriangles[indexTriangleD][5] = point23[2];
                sphereTriangles[indexTriangleD][6] = point3[0];
                sphereTriangles[indexTriangleD][7] = point3[1];
                sphereTriangles[indexTriangleD][8] = point3[2];

            }

            // Divide the step size by four for the next level
            stepSize /= 4;

        }

    // *************************************************************************
    // * STEP 6: Extract points and make sure each point does not appear twice *
    // *************************************************************************

        // When the array is being filled, there is no point at first
        numberPoints = 0;

        // Loop for each triangle in the list (each triangle has 3 points)
        for (indexRow = 0; indexRow < mySphere->SPHERE_NUMBERTRIANGLES; indexRow++)
        {

            // Loop for each point in the triangle (there are 3 points, thus
            // 9 values). We loop at the beginning of each point (0, 3 and 6)
            for (indexPointTriangle = 0; indexPointTriangle <= 6; indexPointTriangle += 3)
            {

                // Load the point to be added in the list
                currentPoint[0] = sphereTriangles[indexRow][indexPointTriangle + 0];
                currentPoint[1] = sphereTriangles[indexRow][indexPointTriangle + 1];
                currentPoint[2] = sphereTriangles[indexRow][indexPointTriangle + 2];

                // Initially, we assume the point is not in the list
                pointAlreadyInList = 0;

                // Check for each point already added
                for (indexPointSphere = 0; indexPointSphere < numberPoints; indexPointSphere++)
                {
                    // Check if the point to be added matches the point in the list
                    if ((fabs((currentPoint[0] - mySphere->spherePoints[indexPointSphere][0])) < 1E-5) &&
                        (fabs((currentPoint[1] - mySphere->spherePoints[indexPointSphere][1])) < 1E-5) &&
                        (fabs((currentPoint[2] - mySphere->spherePoints[indexPointSphere][2])) < 1E-5))
                    {

                        // If it does, then the point already exists: exit loop
                        pointAlreadyInList = 1;
                        break;

                    }
                }

                // If the point is not already in the list, then add it
                if (pointAlreadyInList == 0)
                {
                    mySphere->spherePoints[numberPoints][0] = currentPoint[0];
                    mySphere->spherePoints[numberPoints][1] = currentPoint[1];
                    mySphere->spherePoints[numberPoints][2] = currentPoint[2];
                    numberPoints++;

                }

            }

        }

    // *************************************************************************
    // * STEP 7: Free memory                                                   *
    // *************************************************************************

        deleteTable2D((void**) sphereTriangles);

}

void sphereArcInit(struct objSphere *mySphere, float angleInDegreeStart, float angleInDegreeStop, unsigned int nPoints) {
    
    unsigned int indexPoint;
    
    float angleInDegreeStartMod;
    float angleInDegreeStopMod;
    float theta;
    float x,y,z;
    
    // *************************************************************************
    // * STEP 1: Initialize context                                            *
    // *************************************************************************

    // Number of recursive levels to generate the points on the sphere
    // We set it to zero here because it is a special case
    mySphere->SPHERE_NUMBERLEVELS = 0;

    // Number of triangles according to the number of levels
    // No triangle in this specific case
    mySphere->SPHERE_NUMBERTRIANGLES = 0;

    // Number of points on the arc
    mySphere->SPHERE_NUMBERPOINTS = nPoints;

    // Sphere
    mySphere->spherePoints = (float **) newTable2D(mySphere->SPHERE_NUMBERPOINTS,3,sizeof(float));    
    
    // *************************************************************************
    // * STEP 2: Generate points                                               *
    // *************************************************************************    
    
    angleInDegreeStartMod = angleInDegreeStart - floorf(angleInDegreeStart / 360.0f) * 360.0f;
    angleInDegreeStopMod = angleInDegreeStop - floorf(angleInDegreeStop / 360.0f) * 360.0f;
    
    if (angleInDegreeStopMod <= angleInDegreeStartMod) {
        angleInDegreeStopMod = angleInDegreeStop + 360.0f;
    }
    
    for (indexPoint = 0; indexPoint < mySphere->SPHERE_NUMBERPOINTS; indexPoint++) {
        
        theta = angleInDegreeStartMod + indexPoint * (angleInDegreeStopMod - angleInDegreeStartMod) / (mySphere->SPHERE_NUMBERPOINTS - 1);
        
        x = cosf(2*M_PI*theta/360.0f);
        y = sinf(2*M_PI*theta/360.0f);
        z = 0;
        
        mySphere->spherePoints[indexPoint][0] = x;
        mySphere->spherePoints[indexPoint][1] = y;
        mySphere->spherePoints[indexPoint][2] = z;
        
        printf("%u: %f %f %f\n",indexPoint,x,y,z);
        
    }
    
}

/*******************************************************************************
 * sphereClone                                                                 *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      sourceSphere    The sphere object to clone                     *
 *                                                                             *
 * Outputs:     destSphere      The sphere object that becomes the clone       *
 *                                                                             *
 * Description: This function clones the sphere into a new object              *
  *                                                                            *
 ******************************************************************************/

void sphereClone(struct objSphere *sourceSphere, struct objSphere *destSphere)
{

    unsigned int indexPoint;

    // *************************************************************************
    // * STEP 1: Copy parameters                                               *
    // *************************************************************************

    destSphere->SPHERE_NUMBERLEVELS = sourceSphere->SPHERE_NUMBERLEVELS;
    destSphere->SPHERE_NUMBERPOINTS = sourceSphere->SPHERE_NUMBERPOINTS;
    destSphere->SPHERE_NUMBERTRIANGLES = sourceSphere->SPHERE_NUMBERTRIANGLES;

    // *************************************************************************
    // * STEP 2: Create arrays                                                 *
    // *************************************************************************

    destSphere->spherePoints = (float **) newTable2D(sourceSphere->SPHERE_NUMBERPOINTS,3,sizeof(float));

    // *************************************************************************
    // * STEP 3: Copy points                                                   *
    // *************************************************************************

    for (indexPoint = 0; indexPoint < sourceSphere->SPHERE_NUMBERPOINTS; indexPoint++)
    {

        destSphere->spherePoints[indexPoint][0] = sourceSphere->spherePoints[indexPoint][0];
        destSphere->spherePoints[indexPoint][1] = sourceSphere->spherePoints[indexPoint][1];
        destSphere->spherePoints[indexPoint][2] = sourceSphere->spherePoints[indexPoint][2];

    }

}

/*******************************************************************************
 * sphereTerminate                                                             *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      mySphere        The sphere object to be terminated             *
 *                                                                             *
 * Outputs:     (none)                                                         *
 *                                                                             *
 * Description: This function terminates the sphere object                     *
  *                                                                            *
 ******************************************************************************/
void sphereTerminate(struct objSphere *mySphere)
{

    // Free memory
    deleteTable2D((void**) mySphere->spherePoints);

}

INLINE_PREFIX float sphereGetX(struct objSphere* mySphere, unsigned int indexPoint)
{
    return (mySphere->spherePoints[indexPoint][0]);
}

INLINE_PREFIX float sphereGetY(struct objSphere* mySphere, unsigned int indexPoint)
{
    return (mySphere->spherePoints[indexPoint][1]);
}

INLINE_PREFIX float sphereGetZ(struct objSphere* mySphere, unsigned int indexPoint)
{
    return (mySphere->spherePoints[indexPoint][2]);
}
