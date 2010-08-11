#include "compensator.h"

/*******************************************************************************
 * Compatibility issues                                                        *
 ******************************************************************************/

// In some environment, the constant PI is not define with the math.h library
#ifndef M_PI
        #define M_PI	3.1415926535897932384626433832795
#endif

void compensatorUpdate(struct objCompensator* myCompensator, float x, float y, float z, float E0)
{

    // Horizontal angle
    float horizontalAngle;

    // First loop for computation
    int index1;

    // Second loop for computation
    int index2;

    // Linear distance between two angles
    float distance;

    // Similitude between two angles
    float similitude;

    // Similitude between new angle and others
    float currentSimilitude;

    // Sum of all similitudes
    float sumSimilitude;

    /***************************************************************************
    * Step 1: Update the pointer                                               *
    ***************************************************************************/

    // Increment
    myCompensator->pointer++;

    // Wrap around if needed
    if (myCompensator->pointer >= COMP_BUFFERSIZE)
    {
        myCompensator->pointer = 0;
    }

    /***************************************************************************
    * Step 2: Save the current angle and energy                                *
    ***************************************************************************/

    horizontalAngle = (atan2(y,x) / (2*M_PI)) * 360.0;
    myCompensator->maximaAngle[myCompensator->pointer] = horizontalAngle;
    myCompensator->maximaWeight[myCompensator->pointer] = E0;

    /***************************************************************************
    * Step 3: Compute the dispersion for the current data                      *
    ***************************************************************************/

    // Initialize the sum
    sumSimilitude = 0.0;
    currentSimilitude = 0.0;

    // First loop
    for (index1 = 0; index1 < (COMP_BUFFERSIZE - 1); index1++)
    {
        // Second loop
        for (index2 = (index1 + 1); index2 < COMP_BUFFERSIZE; index2++)
        {

            // +-----------------------------------------------------------+
            // | Step I: Compute the distance between the pair of angles   |
            // +-----------------------------------------------------------+

            // Get the distance
            distance = myCompensator->maximaAngle[index1] - myCompensator->maximaAngle[index2];

            // Make sure it is positive
            if (distance < 0)
            {
                distance *= -1;
            }

            // Make sure it lies between 0 and 180
            if (distance > 180)
            {
                distance = 360 - distance;
            }

            // +-----------------------------------------------------------+
            // | Step II: Compute the similitude between this pair         |
            // +-----------------------------------------------------------+

            //similitude = myCompensator->maximaWeight[index1] * myCompensator->maximaWeight[index2] * exp(-1 * COMP_SPREAD1 * distance);
            similitude = exp(-1 * COMP_SPREAD1 * distance);

            sumSimilitude += similitude;

            // +-----------------------------------------------------------+
            // | Step III: Compute the current similitude                  |
            // +-----------------------------------------------------------+

            if ((index1 == myCompensator->pointer) || (index2 == myCompensator->pointer))
            {
                //currentSimilitude += myCompensator->maximaWeight[index1] * myCompensator->maximaWeight[index2] * exp(-1 * COMP_SPREAD2 * distance);
                currentSimilitude += exp(-1 * COMP_SPREAD2 * distance);
            }           

        }
    }

    /***************************************************************************
    * Step 4: Compute the gain for the current maximum                         *
    ***************************************************************************/

    myCompensator->gain = (sumSimilitude * currentSimilitude);

}

void compensatorInit(struct objCompensator* myCompensator)
{

    int indexBuffer;

    /***************************************************************************
    * Step 1: Clear the buffer                                                 *
    ***************************************************************************/

    for (indexBuffer = 0; indexBuffer < COMP_BUFFERSIZE; indexBuffer++)
    {
        myCompensator->maximaAngle[indexBuffer] = 0.0;
        myCompensator->maximaWeight[indexBuffer] = 0.0;
    }

    /***************************************************************************
    * Step 2: Position pointer                                                 *
    ***************************************************************************/

    myCompensator->pointer = COMP_BUFFERSIZE - 1;

    /***************************************************************************
    * Step 3: Init gain                                                        *
    ***************************************************************************/

    myCompensator->gain = 1.0;

}
