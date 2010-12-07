/*******************************************************************************
 * ManyEars: transcendental - Source Code                                      *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Author: François Grondin                                                    *
 * Original Code: Jean-Marc Valin                                              *
 * Modified Code: Simon Brière                                                 *
 * Version: 1.1.0                                                              *
 * Date: November 1st , 2010                                                   *
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

#include "Utilities/transcendental.h"

/*******************************************************************************
 * transcendentalInit                                                          *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myTranscendental        The object to be initialized           *
 *                                                                             *
 * Outputs:     myTranscendental        The initialized object                 *
 *                                                                             *
 * Description: This function initializes the objTranscendental object         *
 *                                                                             *
 ******************************************************************************/

void transcendentalInit(struct objTranscendental* myTranscendental)
{

    float numberElements;
    unsigned int numberElementsRounded;

    unsigned int index;
    float v;
    float intPart;
    float fracPart;
    float value;

    const float table[21] = {0.75008f,0.93640f,1.11688f,1.28852f,1.45013f,1.60184f,1.74422f,
                             1.87812f,2.00448f,2.12417f,2.23799f,2.34656f,2.45053f,2.55031f,
                             2.64646f,2.73921f,2.82895f,2.91602f,3.00049f,3.08265f,3.16270f};

    // Save parameters
    myTranscendental->interval = (float) TRANSCENDENTAL_INTERVAL;
    myTranscendental->minValue = (float) TRANSCENDENTAL_MINVALUE;
    myTranscendental->maxValue = (float) TRANSCENDENTAL_MAXVALUE;

    // Generate the array
    numberElements = (myTranscendental->maxValue - myTranscendental->minValue) / myTranscendental->interval;
    numberElementsRounded = (unsigned int) floor(numberElements);
    myTranscendental->numberElements = numberElementsRounded;
    myTranscendental->results = (float*) newTable1D(myTranscendental->numberElements, sizeof(float));

    // Fill the array
    v = 0.0f;

    for (index = 0; index < myTranscendental->numberElements; index++)
    {

        if (v <= 9.5f)
        {
            intPart = floorf(2.0f*v);
            fracPart = 2.0f * v - intPart;

            value = ((1-fracPart) * table[(unsigned int) intPart] +
                    fracPart * table[((unsigned int) intPart) + 1]) /
                    sqrtf(v + 0.0001f);

        }
        else
        {
            value = 1.0f;
        }

        myTranscendental->results[index] = value;
        v += myTranscendental->interval;

    }

}

/*******************************************************************************
 * transcendentalTerminate                                                     *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myTranscendental        The object to be terminated            *
 *                                                                             *
 * Outputs:     (none)                                                         *
 *                                                                             *
 * Description: This function terminates the objTranscendental object          *
 *                                                                             *
 ******************************************************************************/

void transcendentalTerminate(struct objTranscendental* myTranscendental)
{

    // Free memory
    deleteTable1D((void*) myTranscendental->results);

}

/*******************************************************************************
 * transcendentalEvaluate                                                      *
 * --------------------------------------------------------------------------- *
 *                                                                             *
 * Inputs:      myTranscendental        The object to be used                  *
 *              value                   The argument of the transcendental     *
 *                                      equation                               *
 *                                                                             *
 * Outputs:     (float)                 The transcendental value               *
 *                                                                             *
 * Description: This function evaluates a hardcoded transcendental function    *
 *                                                                             *
 ******************************************************************************/

float transcendentalEvaluate(struct objTranscendental* myTranscendental, float value)
{

    unsigned int index;

    index = ((unsigned int) floor((value / myTranscendental->interval))) + 1;

    if (index < 0)
    {
        index = 0;
    }
    if (index > (myTranscendental->numberElements - 1))
    {
        index = myTranscendental->numberElements - 1;
    }

    return myTranscendental->results[index];

}
