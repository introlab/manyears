/******************************************************************************* 
 * ManyEars: Random Number Generator - Source Code                             *
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

#include <math.h>

#include "randGen.h"

/*******************************************************************************
 * Compatibility issues                                                        *
 ******************************************************************************/

// In some environment, the constant PI is not define with the math.h library
#ifndef M_PI
	#define M_PI	3.1415926535897932384626433832795
#endif

/*******************************************************************************
 * Static variables                                                            *
 ******************************************************************************/

	// Accumulator for the random generator
	unsigned long randAcc;

	// Array with the normal distribution
	float normalDistribution[RAND_NORMALSIZE];

/*******************************************************************************
 * initLCG                                                                     *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      (none)                                                         *
 *                                                                             *
 * Outputs:     randAcc     The random accumulator is initialized              *
 *                                                                             *
 * Description: This function initializes the Linear Congruential Generator    *
 ******************************************************************************/

void initLCG(void)
{
	randAcc = 1;
}

/*******************************************************************************
 * LCG                                                                         *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      (none)                                                         *
 *                                                                             *
 * Outputs:     randAcc             The random accumulator is refreshed        *
 *                                                                             *
 * Description: This function refreshes the Linear Congruential Generator      *
 ******************************************************************************/

void LCG(void)
{
	randAcc = RAND_LCG_A * randAcc + RAND_LCG_C;
}

/*******************************************************************************
 * randInit                                                                    *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      (none)                                                         *
 *                                                                             *
 * Outputs:     randAcc             The random accumulator is initialized      *
 *              normalDistribution  The normalDistribution array to be filled  *
 *                                  with samples for a normal distribution     *
 *                                                                             *
 * Description: This function initializes the Linear Congruential Generator    *
 *              and generates a normal distribution with the Box-Muller        *
 *              transformation in cartesian form                               *
 ******************************************************************************/

void randInit(void)
{
	
	unsigned int index;
	float x1, x2;
	float w;
	float y1, y2;

	float tmp;

	// Initialize the LCG
	initLCG();

	// Generate the normal distribution with the Box-Muller transformation
	// in cartesian form
	for (index = 0; index < RAND_NORMALSIZE; index+=2)
	{
	
		// Loop until the point is inside a unit circle
		do
		{
			
			// Generate the point coordinates
			x1 = 2.0 * randu() - 1.0;
			x2 = 2.0 * randu() - 1.0;

			// Get the radius squared
			w = x1 * x1 + x2 * x2;

		} while (w >= 1.0);

		// Apply equations
		w = sqrt( (-2.0 * log( w ) ) / w );
        y1 = x1 * w;
        y2 = x2 * w;

		// Store new random variables
		normalDistribution[index] = y1;
		normalDistribution[index + 1] = y2;

	}

}

/*******************************************************************************
 * randu                                                                       *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      (none)                                                         *
 *                                                                             *
 * Outputs:     Random Number   Uniformly distributed random number            *
 *                                                                             *
 * Description: This function returns a uniformly distributed random number    *
 *              between 0 and 1                                                *
 ******************************************************************************/

float randu(void)
{
	float tmp;

	// Refresh the random accumulator
	LCG();
	
	// Take bits 30 to 16 only
	tmp = ((float) ((0x7FFF0000 & randAcc) >> 16));
	
	// Normalize
	tmp /= 32768.0;

	return  tmp;
}

/*******************************************************************************
 * randn                                                                       *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      (none)                                                         *
 *                                                                             *
 * Outputs:     Random Number   Normally distributed random number             *
 *                                                                             *
 * Description: This function returns a normally distributed random number     *
 *              (average = 0, std = 1)                                         *
 ******************************************************************************/

float randn(void)
{

	unsigned int index;

	// Select a random index
	index = (unsigned int) (floor(randu() * RAND_NORMALSIZE));

	return (normalDistribution[index]);

}
