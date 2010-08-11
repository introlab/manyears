/*******************************************************************************
 * ManyEars: Hardware configuration - Header                                   *
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

#ifndef HARDWARE_H
#define HARDWARE_H

// =============================================================================

/*******************************************************************************
 * Hardware acceleration                                                       *
 ******************************************************************************/

// This "patch" is required since the type m128 is not always recognized
// in all environments.

#ifdef USE_SIMD

#include <xmmintrin.h>
#include <sys/types.h>

#ifdef __GNUC__

#ifndef WIN32
#define __int64 long
#define __int8 char
#define __int16 short
#define __int32 int
#define __int64 long
#endif

typedef union {
    __m128              m128;
    float               m128_f32[4];
    unsigned __int64    m128_u64[2];
    __int8              m128_i8[16];
    __int16             m128_i16[8];
    __int32             m128_i32[4];
    __int64             m128_i64[2];
    unsigned __int8     m128_u8[16];
    unsigned __int16    m128_u16[8];
    unsigned __int32    m128_u32[4];
} __m128_mod __attribute__ ((aligned (16)));

#else
    typedef __m128 __m128_mod;
#endif

#endif //USE_SIMD

// =============================================================================


#ifdef __GNUC__
    #define MSVC_ALIGN_PREFIX
    #define GCC_ALIGN_SUFFIX  __attribute__ ((aligned (16)))
#else
    #define MSVC_ALIGN_PREFIX __declspec(align(16))
    #define GCC_ALIGN_SUFFIX
#endif

#endif
