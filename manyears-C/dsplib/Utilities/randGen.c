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
#include <stdio.h>
#include <math.h>

#include "randGen.h"

unsigned int counter = 0;

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
                    x1 = 2.0f * randu() - 1.0f;
                    x2 = 2.0f * randu() - 1.0f;

                    // Get the radius squared
                    w = x1 * x1 + x2 * x2;

		} while (w >= 1.0);

		// Apply equations
		w = sqrtf( (-2.0f * logf( w ) ) / w );
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
	tmp /= 32768.0f;

        counter++;

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

//        printf("%i\n",counter);

	return (normalDistribution[index]);

}
