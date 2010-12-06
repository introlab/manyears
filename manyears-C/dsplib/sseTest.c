#include "hardware.h"
#include "stdio.h"

int main (int argc, char* argv[])
{

#ifdef USE_SIMD

    // SIMD registers
    __m128_mod regA, regB, regC;
    int i;

    regA.m128_f32[0] = 1.0;
    regA.m128_f32[1] = 2.0;
    regA.m128_f32[2] = 3.0;
    regA.m128_f32[3] = 4.0;

    regB.m128_f32[0] = 10.0;
    regB.m128_f32[1] = 20.0;
    regB.m128_f32[2] = 30.0;
    regB.m128_f32[3] = 40.0;

    regC.m128 = _mm_add_ps(regA.m128,regB.m128);

    for (i = 0; i < 4; i++)
    {
        printf("%f\n",regC.m128_f32[i]);
    }

#endif

    return 0;

}
