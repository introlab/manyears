/******************************************************************************* 
 * ManyEars: Random Number Generator - Header                                  *
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

#ifndef RANDGEN_H
#define RANDGEN_H

// =============================================================================

/*******************************************************************************
 * Constants                                                                   *
 ******************************************************************************/

    // Linear Congruential Generator: X_(n+1) = (a * X_n + c) mod (2^32)
    #define     RAND_LCG_A          214013
    #define     RAND_LCG_C          2531011

    // Define the number of points in the normal distribution function
    // Must be an even number
    #define     RAND_NORMALSIZE     10000

/*******************************************************************************
 * Prototypes                                                                  *
 ******************************************************************************/

    void initLCG(void);

    void LCG(void);

    void randInit(void);

    float randu(void);

    float randn(void);

// =============================================================================

#endif
