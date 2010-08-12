#ifndef SPHEREOBJ_H
#define SPHEREOBJ_H

#include "math.h"

class Sphere
{

public:

    /***********************************************************
    * Constructor                                              *
    ***********************************************************/

    Sphere(int _numberLevels, float _radius);

    /***********************************************************
    * Destructor                                               *
    ***********************************************************/

    ~Sphere();

    /***********************************************************
    * Accessors                                                *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Get the number of triangles                           |
    // +-------------------------------------------------------+

    int getNumberTriangles();

    // +-------------------------------------------------------+
    // | Get the number of levels                              |
    // +-------------------------------------------------------+

    int getNumberLevels();

    // +-------------------------------------------------------+
    // | Get the vertices of a given triangle                  |
    // +-------------------------------------------------------+

    void getTriangleVertices(int _triangleIndex, float *_x1, float *_y1, float *_z1, float *_x2, float *_y2, float *_z2, float *_x3, float *_y3, float *_z3);

    // +-------------------------------------------------------+
    // | Get the radius                                        |
    // +-------------------------------------------------------+

    float getRadius();


private:

    /***********************************************************
    * Computations                                             *
    ***********************************************************/

    // +-------------------------------------------------------+
    // | Initialization of the sphere triangles                |
    // +-------------------------------------------------------+

    void initSphere(float _radius);

    /***********************************************************
    * Private fields                                           *
    ***********************************************************/

    // Array of vertices
    float **arrayTriangles;

    // Number of triangles
    int numberTriangles;

    // Number of levels
    int numberLevels;

    // Radius of the sphere
    float radius;

    // Position of the sphere
    float x, y, z;

};

#endif // SPHERE_H
