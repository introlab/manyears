/*******************************************************************************
 * ManyEars: Math Extented Operations - Header                                 *
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

#ifndef MATHADVANCED_H
#define MATHADVANCED_H

#include "../hardware.h"

/*******************************************************************************
 * INLINE_PREFIX functions                                                            *
 ******************************************************************************/

// +---------------------------------------------------------------------------+
// | Exponential                                                               |
// +---------------------------------------------------------------------------+

static INLINE_PREFIX float expest(float _arg)
{

    // Compute the exponential with the limit definition:
    //
    // exp(x) = (1 + x/n)^n
    //
    // where n is the number of iteration (needs to go to
    // infinity to get exact result)

    // Here n = 128;
    //
    // Only good for range x = [-128,0]

    float sum;

    sum = (float) (1.0 + _arg / 128.0);

    if (sum < 0.0)
    {
        sum = 0.0;
    }

    sum *= sum;
    sum *= sum;
    sum *= sum;
    sum *= sum;
    sum *= sum;
    sum *= sum;
    sum *= sum;

    return sum;

}

#endif
