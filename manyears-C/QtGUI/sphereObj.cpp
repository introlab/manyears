#include "sphereObj.h"

/***********************************************************
* Constructor                                              *
***********************************************************/

Sphere::Sphere(int _numberLevels, float _radius)
{
    this->numberLevels = _numberLevels;
    this->numberTriangles = 20 * pow(4.0, (double) this->numberLevels);

    // Create an array dynamically to store the triangles
    this->arrayTriangles = new float*[(this->numberTriangles)];

    for (int indexTriangle = 0; indexTriangle < this->numberTriangles; indexTriangle++)
    {
        this->arrayTriangles[indexTriangle] = new float[9];
    }

    this->initSphere(_radius);

}

/***********************************************************
* Destructor                                               *
***********************************************************/

Sphere::~Sphere()
{

    // Delete the dynamic array
    for (int indexTriangle = 0; indexTriangle < this->numberTriangles; indexTriangle++)
    {
        delete[] this->arrayTriangles[indexTriangle];
    }

    delete[] (this->arrayTriangles);

}

/***********************************************************
* Accessors                                                *
***********************************************************/

// +-------------------------------------------------------+
// | Get the number of triangles                           |
// +-------------------------------------------------------+

int Sphere::getNumberTriangles()
{
    return (this->numberTriangles);
}

// +-------------------------------------------------------+
// | Get the number of levels                              |
// +-------------------------------------------------------+

int Sphere::getNumberLevels()
{
    return (this->numberLevels);
}

// +-------------------------------------------------------+
// | Get the vertices of a given triangle                  |
// +-------------------------------------------------------+

void Sphere::getTriangleVertices(int _triangleIndex, float *_x1, float *_y1, float *_z1, float *_x2, float *_y2, float *_z2, float *_x3, float *_y3, float *_z3)
{

    *_x1 = this->arrayTriangles[_triangleIndex][0];
    *_y1 = this->arrayTriangles[_triangleIndex][1];
    *_z1 = this->arrayTriangles[_triangleIndex][2];
    *_x2 = this->arrayTriangles[_triangleIndex][3];
    *_y2 = this->arrayTriangles[_triangleIndex][4];
    *_z2 = this->arrayTriangles[_triangleIndex][5];
    *_x3 = this->arrayTriangles[_triangleIndex][6];
    *_y3 = this->arrayTriangles[_triangleIndex][7];
    *_z3 = this->arrayTriangles[_triangleIndex][8];

}

// +-------------------------------------------------------+
// | Get the radius                                        |
// +-------------------------------------------------------+

float Sphere::getRadius()
{
    return (this->radius);
}

/***********************************************************
* Computations                                             *
***********************************************************/

// +-------------------------------------------------------+
// | Initialization of the sphere                          |
// +-------------------------------------------------------+

void Sphere::initSphere(float _radius)
{

    // Each triangle is represented by a row of 9 points:
    //
    // +----------+----------+----------+
    // | x1 y1 z1 | x2 y2 z2 | x3 y3 z3 |
    // +----------+----------+----------+
    //
    // Where each set (x,y,z) represents one end of the triangle:
    //
    //               o (x1,y1,z1)
    //              / \
    //  (x2,y2,z2) o---o (x3,y3,z3)
    //
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
    // * STEP 0: Compute parameters                                            *
    // *************************************************************************

    // Define variable to store parameters to generate the triangles
    sint = 0.4472136;
    cost = 0.8944272;

    // Update the radius and center of the sphere
    this->radius = _radius;

    // *************************************************************************
    // * STEP 1: Generate the 12 points and 20 triangles for level 0           *
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
        level0Points[1][0] = cost * sin(2 * M_PI * 0 / 5);
        level0Points[1][1] = cost * cos(2 * M_PI * 0 / 5);
        level0Points[1][2] = sint;
        level0Points[2][0] = cost * sin(2 * M_PI * 1 / 5);
        level0Points[2][1] = cost * cos(2 * M_PI * 1 / 5);
        level0Points[2][2] = sint;
        level0Points[3][0] = cost * sin(2 * M_PI * 2 / 5);
        level0Points[3][1] = cost * cos(2 * M_PI * 2 / 5);
        level0Points[3][2] = sint;
        level0Points[4][0] = cost * sin(2 * M_PI * 3 / 5);
        level0Points[4][1] = cost * cos(2 * M_PI * 3 / 5);
        level0Points[4][2] = sint;
        level0Points[5][0] = cost * sin(2 * M_PI * 4 / 5);
        level0Points[5][1] = cost * cos(2 * M_PI * 4 / 5);
        level0Points[5][2] = sint;

        // Circle on bottom hemisphere
        level0Points[6][0] = -cost * sin(2 * M_PI * 0 / 5);
        level0Points[6][1] = -cost * cos(2 * M_PI * 0 / 5);
        level0Points[6][2] = -sint;
        level0Points[7][0] = -cost * sin(2 * M_PI * 1 / 5);
        level0Points[7][1] = -cost * cos(2 * M_PI * 1 / 5);
        level0Points[7][2] = -sint;
        level0Points[8][0] = -cost * sin(2 * M_PI * 2 / 5);
        level0Points[8][1] = -cost * cos(2 * M_PI * 2 / 5);
        level0Points[8][2] = -sint;
        level0Points[9][0] = -cost * sin(2 * M_PI * 3 / 5);
        level0Points[9][1] = -cost * cos(2 * M_PI * 3 / 5);
        level0Points[9][2] = -sint;
        level0Points[10][0] = -cost * sin(2 * M_PI * 4 / 5);
        level0Points[10][1] = -cost * cos(2 * M_PI * 4 / 5);
        level0Points[10][2] = -sint;

        // Scale and translate the points according to radius and center
        for (int indexPoint = 0; indexPoint < 12; indexPoint++)
        {

            level0Points[indexPoint][0] = level0Points[indexPoint][0] * this->radius;
            level0Points[indexPoint][1] = level0Points[indexPoint][1] * this->radius;
            level0Points[indexPoint][2] = level0Points[indexPoint][2] * this->radius;

        }

        // Use these points to define the 20 triangles for level 0
        // Make sure points are attributed to a triangle in
        // COUNTERCLOCKWISE direction. This is important for later
        // as openGL renders triangle with this orientation.

        // First define the step size to fill the array
        stepSize = this->numberTriangles / 20;

        // Fill the array

        // Triangle: 0-2-1
        this->arrayTriangles[0*stepSize][0] = level0Points[0][0];
        this->arrayTriangles[0*stepSize][1] = level0Points[0][1];
        this->arrayTriangles[0*stepSize][2] = level0Points[0][2];
        this->arrayTriangles[0*stepSize][3] = level0Points[2][0];
        this->arrayTriangles[0*stepSize][4] = level0Points[2][1];
        this->arrayTriangles[0*stepSize][5] = level0Points[2][2];
        this->arrayTriangles[0*stepSize][6] = level0Points[1][0];
        this->arrayTriangles[0*stepSize][7] = level0Points[1][1];
        this->arrayTriangles[0*stepSize][8] = level0Points[1][2];

        // Triangle: 0-3-2
        this->arrayTriangles[1*stepSize][0] = level0Points[0][0];
        this->arrayTriangles[1*stepSize][1] = level0Points[0][1];
        this->arrayTriangles[1*stepSize][2] = level0Points[0][2];
        this->arrayTriangles[1*stepSize][3] = level0Points[3][0];
        this->arrayTriangles[1*stepSize][4] = level0Points[3][1];
        this->arrayTriangles[1*stepSize][5] = level0Points[3][2];
        this->arrayTriangles[1*stepSize][6] = level0Points[2][0];
        this->arrayTriangles[1*stepSize][7] = level0Points[2][1];
        this->arrayTriangles[1*stepSize][8] = level0Points[2][2];

        // Triangle: 0-4-3
        this->arrayTriangles[2*stepSize][0] = level0Points[0][0];
        this->arrayTriangles[2*stepSize][1] = level0Points[0][1];
        this->arrayTriangles[2*stepSize][2] = level0Points[0][2];
        this->arrayTriangles[2*stepSize][3] = level0Points[4][0];
        this->arrayTriangles[2*stepSize][4] = level0Points[4][1];
        this->arrayTriangles[2*stepSize][5] = level0Points[4][2];
        this->arrayTriangles[2*stepSize][6] = level0Points[3][0];
        this->arrayTriangles[2*stepSize][7] = level0Points[3][1];
        this->arrayTriangles[2*stepSize][8] = level0Points[3][2];

        // Triangle: 0-5-4
        this->arrayTriangles[3*stepSize][0] = level0Points[0][0];
        this->arrayTriangles[3*stepSize][1] = level0Points[0][1];
        this->arrayTriangles[3*stepSize][2] = level0Points[0][2];
        this->arrayTriangles[3*stepSize][3] = level0Points[5][0];
        this->arrayTriangles[3*stepSize][4] = level0Points[5][1];
        this->arrayTriangles[3*stepSize][5] = level0Points[5][2];
        this->arrayTriangles[3*stepSize][6] = level0Points[4][0];
        this->arrayTriangles[3*stepSize][7] = level0Points[4][1];
        this->arrayTriangles[3*stepSize][8] = level0Points[4][2];

        // Triangle: 0-1-5
        this->arrayTriangles[4*stepSize][0] = level0Points[0][0];
        this->arrayTriangles[4*stepSize][1] = level0Points[0][1];
        this->arrayTriangles[4*stepSize][2] = level0Points[0][2];
        this->arrayTriangles[4*stepSize][3] = level0Points[1][0];
        this->arrayTriangles[4*stepSize][4] = level0Points[1][1];
        this->arrayTriangles[4*stepSize][5] = level0Points[1][2];
        this->arrayTriangles[4*stepSize][6] = level0Points[5][0];
        this->arrayTriangles[4*stepSize][7] = level0Points[5][1];
        this->arrayTriangles[4*stepSize][8] = level0Points[5][2];

        // Triangle: 9-1-2
        this->arrayTriangles[5*stepSize][0] = level0Points[9][0];
        this->arrayTriangles[5*stepSize][1] = level0Points[9][1];
        this->arrayTriangles[5*stepSize][2] = level0Points[9][2];
        this->arrayTriangles[5*stepSize][3] = level0Points[1][0];
        this->arrayTriangles[5*stepSize][4] = level0Points[1][1];
        this->arrayTriangles[5*stepSize][5] = level0Points[1][2];
        this->arrayTriangles[5*stepSize][6] = level0Points[2][0];
        this->arrayTriangles[5*stepSize][7] = level0Points[2][1];
        this->arrayTriangles[5*stepSize][8] = level0Points[2][2];

        // Triangle: 10-2-3
        this->arrayTriangles[6*stepSize][0] = level0Points[10][0];
        this->arrayTriangles[6*stepSize][1] = level0Points[10][1];
        this->arrayTriangles[6*stepSize][2] = level0Points[10][2];
        this->arrayTriangles[6*stepSize][3] = level0Points[2][0];
        this->arrayTriangles[6*stepSize][4] = level0Points[2][1];
        this->arrayTriangles[6*stepSize][5] = level0Points[2][2];
        this->arrayTriangles[6*stepSize][6] = level0Points[3][0];
        this->arrayTriangles[6*stepSize][7] = level0Points[3][1];
        this->arrayTriangles[6*stepSize][8] = level0Points[3][2];

        // Triangle: 6-3-4
        this->arrayTriangles[7*stepSize][0] = level0Points[6][0];
        this->arrayTriangles[7*stepSize][1] = level0Points[6][1];
        this->arrayTriangles[7*stepSize][2] = level0Points[6][2];
        this->arrayTriangles[7*stepSize][3] = level0Points[3][0];
        this->arrayTriangles[7*stepSize][4] = level0Points[3][1];
        this->arrayTriangles[7*stepSize][5] = level0Points[3][2];
        this->arrayTriangles[7*stepSize][6] = level0Points[4][0];
        this->arrayTriangles[7*stepSize][7] = level0Points[4][1];
        this->arrayTriangles[7*stepSize][8] = level0Points[4][2];

        // Triangle: 7-4-5
        this->arrayTriangles[8*stepSize][0] = level0Points[7][0];
        this->arrayTriangles[8*stepSize][1] = level0Points[7][1];
        this->arrayTriangles[8*stepSize][2] = level0Points[7][2];
        this->arrayTriangles[8*stepSize][3] = level0Points[4][0];
        this->arrayTriangles[8*stepSize][4] = level0Points[4][1];
        this->arrayTriangles[8*stepSize][5] = level0Points[4][2];
        this->arrayTriangles[8*stepSize][6] = level0Points[5][0];
        this->arrayTriangles[8*stepSize][7] = level0Points[5][1];
        this->arrayTriangles[8*stepSize][8] = level0Points[5][2];

        // Triangle: 8-5-1
        this->arrayTriangles[9*stepSize][0] = level0Points[8][0];
        this->arrayTriangles[9*stepSize][1] = level0Points[8][1];
        this->arrayTriangles[9*stepSize][2] = level0Points[8][2];
        this->arrayTriangles[9*stepSize][3] = level0Points[5][0];
        this->arrayTriangles[9*stepSize][4] = level0Points[5][1];
        this->arrayTriangles[9*stepSize][5] = level0Points[5][2];
        this->arrayTriangles[9*stepSize][6] = level0Points[1][0];
        this->arrayTriangles[9*stepSize][7] = level0Points[1][1];
        this->arrayTriangles[9*stepSize][8] = level0Points[1][2];

        // Triangle: 4-7-6
        this->arrayTriangles[10*stepSize][0] = level0Points[4][0];
        this->arrayTriangles[10*stepSize][1] = level0Points[4][1];
        this->arrayTriangles[10*stepSize][2] = level0Points[4][2];
        this->arrayTriangles[10*stepSize][3] = level0Points[7][0];
        this->arrayTriangles[10*stepSize][4] = level0Points[7][1];
        this->arrayTriangles[10*stepSize][5] = level0Points[7][2];
        this->arrayTriangles[10*stepSize][6] = level0Points[6][0];
        this->arrayTriangles[10*stepSize][7] = level0Points[6][1];
        this->arrayTriangles[10*stepSize][8] = level0Points[6][2];

        // Triangle: 5-8-7
        this->arrayTriangles[11*stepSize][0] = level0Points[5][0];
        this->arrayTriangles[11*stepSize][1] = level0Points[5][1];
        this->arrayTriangles[11*stepSize][2] = level0Points[5][2];
        this->arrayTriangles[11*stepSize][3] = level0Points[8][0];
        this->arrayTriangles[11*stepSize][4] = level0Points[8][1];
        this->arrayTriangles[11*stepSize][5] = level0Points[8][2];
        this->arrayTriangles[11*stepSize][6] = level0Points[7][0];
        this->arrayTriangles[11*stepSize][7] = level0Points[7][1];
        this->arrayTriangles[11*stepSize][8] = level0Points[7][2];

        // Triangle: 1-9-8
        this->arrayTriangles[12*stepSize][0] = level0Points[1][0];
        this->arrayTriangles[12*stepSize][1] = level0Points[1][1];
        this->arrayTriangles[12*stepSize][2] = level0Points[1][2];
        this->arrayTriangles[12*stepSize][3] = level0Points[9][0];
        this->arrayTriangles[12*stepSize][4] = level0Points[9][1];
        this->arrayTriangles[12*stepSize][5] = level0Points[9][2];
        this->arrayTriangles[12*stepSize][6] = level0Points[8][0];
        this->arrayTriangles[12*stepSize][7] = level0Points[8][1];
        this->arrayTriangles[12*stepSize][8] = level0Points[8][2];

        // Triangle: 2-10-9
        this->arrayTriangles[13*stepSize][0] = level0Points[2][0];
        this->arrayTriangles[13*stepSize][1] = level0Points[2][1];
        this->arrayTriangles[13*stepSize][2] = level0Points[2][2];
        this->arrayTriangles[13*stepSize][3] = level0Points[10][0];
        this->arrayTriangles[13*stepSize][4] = level0Points[10][1];
        this->arrayTriangles[13*stepSize][5] = level0Points[10][2];
        this->arrayTriangles[13*stepSize][6] = level0Points[9][0];
        this->arrayTriangles[13*stepSize][7] = level0Points[9][1];
        this->arrayTriangles[13*stepSize][8] = level0Points[9][2];

        // Triangle: 3-6-10
        this->arrayTriangles[14*stepSize][0] = level0Points[3][0];
        this->arrayTriangles[14*stepSize][1] = level0Points[3][1];
        this->arrayTriangles[14*stepSize][2] = level0Points[3][2];
        this->arrayTriangles[14*stepSize][3] = level0Points[6][0];
        this->arrayTriangles[14*stepSize][4] = level0Points[6][1];
        this->arrayTriangles[14*stepSize][5] = level0Points[6][2];
        this->arrayTriangles[14*stepSize][6] = level0Points[10][0];
        this->arrayTriangles[14*stepSize][7] = level0Points[10][1];
        this->arrayTriangles[14*stepSize][8] = level0Points[10][2];

        // Triangle: 11-6-7
        this->arrayTriangles[15*stepSize][0] = level0Points[11][0];
        this->arrayTriangles[15*stepSize][1] = level0Points[11][1];
        this->arrayTriangles[15*stepSize][2] = level0Points[11][2];
        this->arrayTriangles[15*stepSize][3] = level0Points[6][0];
        this->arrayTriangles[15*stepSize][4] = level0Points[6][1];
        this->arrayTriangles[15*stepSize][5] = level0Points[6][2];
        this->arrayTriangles[15*stepSize][6] = level0Points[7][0];
        this->arrayTriangles[15*stepSize][7] = level0Points[7][1];
        this->arrayTriangles[15*stepSize][8] = level0Points[7][2];

        // Triangle: 11-7-8
        this->arrayTriangles[16*stepSize][0] = level0Points[11][0];
        this->arrayTriangles[16*stepSize][1] = level0Points[11][1];
        this->arrayTriangles[16*stepSize][2] = level0Points[11][2];
        this->arrayTriangles[16*stepSize][3] = level0Points[7][0];
        this->arrayTriangles[16*stepSize][4] = level0Points[7][1];
        this->arrayTriangles[16*stepSize][5] = level0Points[7][2];
        this->arrayTriangles[16*stepSize][6] = level0Points[8][0];
        this->arrayTriangles[16*stepSize][7] = level0Points[8][1];
        this->arrayTriangles[16*stepSize][8] = level0Points[8][2];

        // Triangle: 11-8-9
        this->arrayTriangles[17*stepSize][0] = level0Points[11][0];
        this->arrayTriangles[17*stepSize][1] = level0Points[11][1];
        this->arrayTriangles[17*stepSize][2] = level0Points[11][2];
        this->arrayTriangles[17*stepSize][3] = level0Points[8][0];
        this->arrayTriangles[17*stepSize][4] = level0Points[8][1];
        this->arrayTriangles[17*stepSize][5] = level0Points[8][2];
        this->arrayTriangles[17*stepSize][6] = level0Points[9][0];
        this->arrayTriangles[17*stepSize][7] = level0Points[9][1];
        this->arrayTriangles[17*stepSize][8] = level0Points[9][2];

        // Triangle: 11-9-10
        this->arrayTriangles[18*stepSize][0] = level0Points[11][0];
        this->arrayTriangles[18*stepSize][1] = level0Points[11][1];
        this->arrayTriangles[18*stepSize][2] = level0Points[11][2];
        this->arrayTriangles[18*stepSize][3] = level0Points[9][0];
        this->arrayTriangles[18*stepSize][4] = level0Points[9][1];
        this->arrayTriangles[18*stepSize][5] = level0Points[9][2];
        this->arrayTriangles[18*stepSize][6] = level0Points[10][0];
        this->arrayTriangles[18*stepSize][7] = level0Points[10][1];
        this->arrayTriangles[18*stepSize][8] = level0Points[10][2];

        // Triangle: 11-10-6
        this->arrayTriangles[19*stepSize][0] = level0Points[11][0];
        this->arrayTriangles[19*stepSize][1] = level0Points[11][1];
        this->arrayTriangles[19*stepSize][2] = level0Points[11][2];
        this->arrayTriangles[19*stepSize][3] = level0Points[10][0];
        this->arrayTriangles[19*stepSize][4] = level0Points[10][1];
        this->arrayTriangles[19*stepSize][5] = level0Points[10][2];
        this->arrayTriangles[19*stepSize][6] = level0Points[6][0];
        this->arrayTriangles[19*stepSize][7] = level0Points[6][1];
        this->arrayTriangles[19*stepSize][8] = level0Points[6][2];

    // *************************************************************************
    // * STEP 2: Generate each subsequent level                                *
    // *************************************************************************

        // Loop for each level starting at 1
        for (int indexLevel = 1; indexLevel <= this->numberLevels; indexLevel++)
        {
            // Loop for each row to be filled in the array
            for (int indexRow = 0; indexRow < this->numberTriangles; indexRow += stepSize)
            {
                // Get the current triangle
                point1[0] = this->arrayTriangles[indexRow][0];
                point1[1] = this->arrayTriangles[indexRow][1];
                point1[2] = this->arrayTriangles[indexRow][2];
                point2[0] = this->arrayTriangles[indexRow][3];
                point2[1] = this->arrayTriangles[indexRow][4];
                point2[2] = this->arrayTriangles[indexRow][5];
                point3[0] = this->arrayTriangles[indexRow][6];
                point3[1] = this->arrayTriangles[indexRow][7];
                point3[2] = this->arrayTriangles[indexRow][8];

                // Find the point that would lie on the sphere between points 1 & 2
                point12[0] = point1[0] + point2[0];
                point12[1] = point1[1] + point2[1];
                point12[2] = point1[2] + point2[2];
                mag12 = sqrt(point12[0] * point12[0] + point12[1] * point12[1] + point12[2] * point12[2]);
                point12[0] /= mag12;
                point12[1] /= mag12;
                point12[2] /= mag12;
                point12[0] *= this->radius;
                point12[1] *= this->radius;
                point12[2] *= this->radius;


                // Find the point that would lie on the sphere between points 2 & 3
                point23[0] = point2[0] + point3[0];
                point23[1] = point2[1] + point3[1];
                point23[2] = point2[2] + point3[2];
                mag23 = sqrt(point23[0] * point23[0] + point23[1] * point23[1] + point23[2] * point23[2]);
                point23[0] /= mag23;
                point23[1] /= mag23;
                point23[2] /= mag23;
                point23[0] *= this->radius;
                point23[1] *= this->radius;
                point23[2] *= this->radius;

                // Find the point that would lie on the sphere between points 1 & 3
                point13[0] = point1[0] + point3[0];
                point13[1] = point1[1] + point3[1];
                point13[2] = point1[2] + point3[2];
                mag13 = sqrt(point13[0] * point13[0] + point13[1] * point13[1] + point13[2] * point13[2]);
                point13[0] /= mag13;
                point13[1] /= mag13;
                point13[2] /= mag13;
                point13[0] *= this->radius;
                point13[1] *= this->radius;
                point13[2] *= this->radius;

                // Create new triangles index in the array
                // Note that the big triangle that is splitted in four triangles will
                // be replaced in the list by triangle A
                indexTriangleA = indexRow + 0 * stepSize / 4;
                indexTriangleB = indexRow + 1 * stepSize / 4;
                indexTriangleC = indexRow + 2 * stepSize / 4;
                indexTriangleD = indexRow + 3 * stepSize / 4;

                // Copy the new values of triangle A: 1-12-13
                this->arrayTriangles[indexTriangleA][0] = point1[0];
                this->arrayTriangles[indexTriangleA][1] = point1[1];
                this->arrayTriangles[indexTriangleA][2] = point1[2];
                this->arrayTriangles[indexTriangleA][3] = point12[0];
                this->arrayTriangles[indexTriangleA][4] = point12[1];
                this->arrayTriangles[indexTriangleA][5] = point12[2];
                this->arrayTriangles[indexTriangleA][6] = point13[0];
                this->arrayTriangles[indexTriangleA][7] = point13[1];
                this->arrayTriangles[indexTriangleA][8] = point13[2];

                // Copy the new values of triangle B: 12-2-23
                this->arrayTriangles[indexTriangleB][0] = point12[0];
                this->arrayTriangles[indexTriangleB][1] = point12[1];
                this->arrayTriangles[indexTriangleB][2] = point12[2];
                this->arrayTriangles[indexTriangleB][3] = point2[0];
                this->arrayTriangles[indexTriangleB][4] = point2[1];
                this->arrayTriangles[indexTriangleB][5] = point2[2];
                this->arrayTriangles[indexTriangleB][6] = point23[0];
                this->arrayTriangles[indexTriangleB][7] = point23[1];
                this->arrayTriangles[indexTriangleB][8] = point23[2];

                // Copy the new values of triangle C: 12-23-13
                this->arrayTriangles[indexTriangleC][0] = point12[0];
                this->arrayTriangles[indexTriangleC][1] = point12[1];
                this->arrayTriangles[indexTriangleC][2] = point12[2];
                this->arrayTriangles[indexTriangleC][3] = point23[0];
                this->arrayTriangles[indexTriangleC][4] = point23[1];
                this->arrayTriangles[indexTriangleC][5] = point23[2];
                this->arrayTriangles[indexTriangleC][6] = point13[0];
                this->arrayTriangles[indexTriangleC][7] = point13[1];
                this->arrayTriangles[indexTriangleC][8] = point13[2];

                // Copy the new values of triangle D: 13-23-3
                this->arrayTriangles[indexTriangleD][0] = point13[0];
                this->arrayTriangles[indexTriangleD][1] = point13[1];
                this->arrayTriangles[indexTriangleD][2] = point13[2];
                this->arrayTriangles[indexTriangleD][3] = point23[0];
                this->arrayTriangles[indexTriangleD][4] = point23[1];
                this->arrayTriangles[indexTriangleD][5] = point23[2];
                this->arrayTriangles[indexTriangleD][6] = point3[0];
                this->arrayTriangles[indexTriangleD][7] = point3[1];
                this->arrayTriangles[indexTriangleD][8] = point3[2];

            }

            // Divide the step size by four for the next level
            stepSize /= 4;

        }

}
