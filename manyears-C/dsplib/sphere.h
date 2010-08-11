/******************************************************************************* 
 * ManyEars: Sphere - Header                                                   *
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

#ifndef SPHERE_H
#define SPHERE_H

#include "dynamicMemory.h"

// =============================================================================

/*******************************************************************************
 * Structures                                                                  *
 ******************************************************************************/

    struct objSphere
    {

        // +-------------------------------------------------------------------+
        // | Parameters                                                        |
        // +-------------------------------------------------------------------+

        // Number of recursive levels to generate the points on the sphere
        int SPHERE_NUMBERLEVELS;

        // Number of triangles according to the number of levels
        // SPHERE_NUMBERTRIANGLES = 20 * 4^(SPHERE_NUBMERLEVELS)
        int SPHERE_NUMBERTRIANGLES;

        // Number of points according to the number of levels
        // SPHERE_NUMBERPOINTS = SPHERE_NUMBERTRIANGLES / 2 + 2
        int SPHERE_NUMBERPOINTS;

        // +-------------------------------------------------------------------+
        // | Variables                                                         |
        // +-------------------------------------------------------------------+

        // Points on the sphere: [SPHERE_NUMBERPOINTS][3]
        float** spherePoints;

    };

/*******************************************************************************
 * Prototypes                                                                  *
 ******************************************************************************/

    void sphereInit(struct objSphere *mySphere, int numberLevels);

    void sphereTerminate(struct objSphere *mySphere);

// =============================================================================

#endif
