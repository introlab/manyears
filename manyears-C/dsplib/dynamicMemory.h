/*******************************************************************************
 * ManyEars: Dynamic Memory Manager - Header                                   *
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

#ifndef DYNAMICMEMORY_H
#define DYNAMICMEMORY_H

#include <stdlib.h>

// =============================================================================

/*******************************************************************************
 * Prototypes                                                                  *
 ******************************************************************************/

void* newTable1D(int len, int sizeOneElement);

void** newTable2D(int firstDim, int secondDim, int sizeOneElement);

void*** newTable3D(int firstDim, int secondDim, int thirdDim, int sizeOneElement);

void deleteTable1D(void* tablePtr);

void deleteTable2D(void** tablePtr);

void deleteTable3D(void*** tablePtr);

// =============================================================================

#endif
