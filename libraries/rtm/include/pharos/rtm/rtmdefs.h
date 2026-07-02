/****************************************************************************
 * Pharos - A Real-Time Secure Operating System                             *
 * Copyright 2020 Pedro Macara and Filipe Monteiro                          *
 *                                                                          *
 * Licensed under the Apache License, Version 2.0 (the "License");          *
 * you may not use this file except in compliance with the License.         *
 * You may obtain a copy of the License at                                  *
 *                                                                          *
 *     http://www.apache.org/licenses/LICENSE-2.0                           *
 *                                                                          *
 * Unless required by applicable law or agreed to in writing, software      *
 * distributed under the License is distributed on an "AS IS" BASIS,        *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. *
 * See the License for the specific language governing permissions and      *
 * limitations under the License.                                           *
 *                                                                          *
 ****************************************************************************/


/*! \addtogroup PHAROS-RTM
 * 
 * @{
 */

#ifndef RTM_DEFS_H
#define RTM_DEFS_H


#include <pharos/hal/hal.h>

/**
 * Zero
 */
#define RTM_CONSTANT_ZERO                                   (0.0)

/**
 * 1/2 
 */
#define RTM_CONSTANT_HALF                                   (0.50000000000000000000e+00)

/**
 * 1 (0x3FF00000, 0x00000000)
 */
#define RTM_CONSTANT_ONE                                    (1.00000000000000000000e+00)

/**
 * 2 (0x40000000, 0x00000000)
 */
#define RTM_CONSTANT_TWO                                    (2.00000000000000000000e+00)

/**
 * PI (0x400921FB, 0x54442D18) 
 */
#define RTM_CONSTANT_PI                                     (3.14159265358979311600e+00)

/**
 * low(PI) (0x3CA1A626, 0x33145C07 )
 */
#define RTM_CONSTANT_PI_LOW                                 (1.2246467991473531772E-16)

/**
 * high(PI/2) (0x3FF921FB, 0x54442D18) 
 */
#define RTM_CONSTANT_PI_DIV_2_HI                            (1.57079632679489655800e+00)

/**
 * low(PI/2) (0x3C91A626, 0x33145C07)
 */
#define RTM_CONSTANT_PI_DIV_2_LOW                           (6.12323399573676603587e-17)

/**
 * PI/2 (0x3FF921FB, 0x54442D18) 
 */
#define RTM_CONSTANT_PI_DIV_2                               (1.5707963267948965580E+00)

/**
 * PI/4 (0x3FE921FB, 0x54442D18) 
 */
#define RTM_CONSTANT_PI_DIV_4                               (7.8539816339744827900E-01)

/**
 * PI/6
 */
#define RTM_CONSTANT_PI_DIV_6                               (.52359877559829887307710723054658383L)

/**
 * high(PI/4) (0x3FE921FB, 0x54442D18) 
 */
#define RTM_CONSTANT_PI_DIV_4_HI                            (7.85398163397448278999e-01)

/**
 * sqrt(1/PI) (0x3FE20DD7, 0x50429B6D) 
 */
#define RTM_CONSTANT_PI_INV_SQRT                            (5.64189583547756279280e-01)

/**
 * Huge number (1e300)
 */
#define RTM_CONSTANT_HUGE                                   (1e300)

/**
 * Tiny number (1e-300)
 */
#define RTM_CONSTANT_TINY                                   (1e-300)

/**
 * Neper number
 */
#define RTM_CONSTANT_E                                      (2.7182818284590452353602874713526625L) 

/**
 * ln(2) (0x3FE62E42, 0xFEFA39EF)
 */
#define RTM_CONSTANT_LN2                                    (6.93147180559945286227e-01)

/**
 * high(ln(2)) (0x3fe62e42 0xfee00000)
 */
#define RTM_CONSTANT_LN2_HI                                 (6.93147180369123816490e-01)

/**
 * low(ln(2)) (0x3dea39ef 0x35793c76)
 */
#define RTM_CONSTANT_LN2_LOW                                (1.90821492927058770002e-10)

/**
 * 1/ln(2) (0x3FF71547, 0x652B82FE)
 */
#define RTM_CONSTANT_1_DIV_LN2                              (1.44269504088896338700e+00)

/**
 * high(1/ln(2)) (0x3FF71547, 0x60000000)
 */
#define RTM_CONSTANT_1_DIV_LN2_HI                           (1.44269502162933349609e+00)

/**
 * low(1/ln(2)) (0x3E54AE0B, 0xF85DDF44)
 */
#define RTM_CONSTANT_1_DIV_LN2_LOW                          (1.92596299112661746887e-08)

/**
 * 2/pi (0x3FE45F30, 0x6DC9C883)
 */
#define RTM_CONSTANT_2_DIV_PI                               (6.36619772367581382433e-01)

/**
 * erx (0x3FEB0AC1, 0x60000000)
 */
#define RTM_CONSTANT_ERX                                    (8.45062911510467529297e-01)

/**
 * efx (0x3FC06EBA, 0x8214DB69)
 */
#define RTM_CONSTANT_EFX                                    (1.28379167095512586316e-01)

/**
 * efx8 (0x3FF06EBA, 0x8214DB69)
 */
#define RTM_CONSTANT_EFX8                                   (1.02703333676410069053e+00)

/**
 * 2^24 (0x41700000, 0x00000000) 
 */
#define RTM_CONSTANT_POWER_2_24                             (1.67772160000000000000e+07)

/**
 * 2^-24 (0x3E700000, 0x00000000) 
 */
#define RTM_CONSTANT_POWER_2_M24                            (5.96046447753906250000e-08)

/**
 * 2^52 (0x43300000, 0x00000000) 
 */
#define RTM_CONSTANT_POWER_2_52                             (4.50359962737049600000e+15)

/**
 * 2^53 (0x43400000, 0x00000000) 
 */
#define RTM_CONSTANT_POWER_2_53                             (9007199254740992.0)

/**
 * 2^54 (0x43500000 0x00000000) 
 */
#define RTM_CONSTANT_POWER_2_54                             (1.80143985094819840000e+16)

/**
 * low part of 2^54 (0x3C900000, 0x00000000 )
 */
#define RTM_CONSTANT_POWER_2_54_LOW                         (5.55111512312578270212e-17)

/**
 * high(log10(2) (0x3FD34413, 0x509F6000)
 */
#define RTM_CONSTANT_LOG10_2_HI                             (3.01029995663611771306e-01)

/**
 * low(log10(2) (0x3D59FEF3, 0x11F12B36)
 */
#define RTM_CONSTANT_LOG10_2_LOW                            (3.69423907715893078616e-13)

/**
 * 1/(ln(10)  (0x3FDBCB7B, 0x1526E50E)
 */
#define RTM_CONSTANT_1_DIV_LN_10                            (4.34294481903251816668e-01)

/**
 * Exponent mask for the high part of a float64_t
 */
#define RTM_FLOAT64_HI_MASK_EXPONENT                        (0x7ff00000)

/**
 * Mask for the sign bit on a float64_t word
 */
#define RTM_FLOAT64_HI_MASK_SIGN                            (0x80000000)

/**
 * Mask for the sign bit on a float64_t word
 */
#define RTM_FLOAT64_HI_MASK_FRAC                            (0x000fffff)

/**
 * Unmask for the sign bit on a float64_t word
 */
#define RTM_FLOAT64_HI_UNMASK_SIGN                          (0x7fffffff)

/**
 * Mask for the sign and the value of the high part of a float64_t leaving only the exponent
 */
#define RTM_FLOAT64_HI_UNMASK_EXPONENT                      (0x800fffff)

/**
 * High word value for 1
 */
#define RTM_FLOAT64_HI_ONE                                  (0x3ff00000)

/**
 * High word value for 0.5
 */
#define RTM_FLOAT64_HI_1_DIV_2                              (0x3fe00000)

/**
 * High word value for 2^-13
 */
#define RTM_FLOAT64_2_EXP_M13                               (0x3f200000)

/**
 * High word value for 2^-27
 */
#define RTM_FLOAT64_2_EXP_M27                               (0x3e400000)

/**
 * High word value for 2^-28
 */
#define RTM_FLOAT64_2_EXP_M28                               (0x3e300000)

/**
 * High word value for 2^-55
 */
#define RTM_FLOAT_2_EXP_M55                                 (0x3c800000)

/**
 * Offset (number of bits) of the exponent on the high word
 */
#define RTM_FLOAT64_HI_EXP_OFFSET                           (20)

/**
 * High word value for 0.6744
 */
#define RTM_FLOAT64_HI_0D6744                               (0x3FE59428)

/**
 * Offset (number of bits) of the sign on the high word
 */
#define RTM_FLOAT64_HI_SIGN_OFFSET                          (31)

/**
 * Float NaN
 */
#define RTM_FLOAT64_NaN                                     ((1.0-1.0) / (1.0-1.0))

/**
 * Float64 +Inf
 */
#define RTM_FLOAT64_INFINITY_PLUS                           (__builtin_huge_val())

/**
 * Float64 -Inf
 */
#define RTM_FLOAT64_INFINITY_MINUS                          (-__builtin_huge_val())


typedef union RtmFloat64 RtmFloat64 , *ptrRtmFloat64;


#if __FLOAT_WORD_ORDER__ == __ORDER_LITTLE_ENDIAN__

/**
 * Compact the size of the I/O registers
 */
#pragma pack(push, 1)

/*********************************************************/
/* Float 64 bits                                         */
/* High word corresponds to                              */
/* s eeeeeeeeeee ffffffffffffffffffff                    */
/* 1 sign bit                                            */
/* 11 exponent bits                                      */
/* 20 fractional bits                                    */
/* The low word is just 32 bits of fractional bits       */

/*********************************************************/

union RtmFloat64
{
    float64_t x;

    struct
    {
        int32_t low;
        int32_t high;
    } bytes;

};

/**
 * restore the previous pack definition
 */
#pragma pack(pop)

#else

/**
 * Compact the size of the I/O registers
 */
#pragma pack(push, 1)

union RtmFloat64
{
    float64_t x;

    struct
    {
        int32_t high;
        int32_t low;
    } bytes;

};

/**
 * restore the previous pack definition
 */
#pragma pack(pop)

#endif

/* RTM_DEFS_H */
#endif 

/*! @} */
