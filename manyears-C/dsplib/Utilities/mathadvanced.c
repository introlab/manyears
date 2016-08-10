#include "Utilities/mathadvanced.h"

float expest(float _arg)
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

