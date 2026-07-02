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


/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunSoft, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 * ====================================================
 */

/*! \addtogroup PHAROS-RTM
 * 
 * @{
 */

#include <pharos/rtm/rtm.h>


PHAROS_WEAK float64_t rtmLog10f64(float64_t x)
{
    /* __ieee754_log10(x)
     * Return the base 10 logarithm of x
     * 
     * Method :
     *	Let log10_2hi = leading 40 bits of log10(2) and
     *	    log10_2lo = log10(2) - log10_2hi,
     *	    ivln10   = 1/log(10) rounded.
     *	Then
     *		n = ilogb(x), 
     *		if(n<0)  n = n+1;
     *		x = scalbn(x,-n);
     *		log10(x) := n*log10_2hi + (n*log10_2lo + ivln10*log(x))
     *
     * Note 1:
     *	To guarantee log10(10**n)=n, where 10**n is normal, the rounding 
     *	mode must set to Round-to-Nearest.
     * Note 2:
     *	[1/log(10)] rounded to 53 bits has error  .198   ulps;
     *	log10 is monotonic at all binary break points.
     *
     * Special cases:
     *	log10(x) is NaN with signal if x < 0; 
     *	log10(+INF) is +INF with no signal; log10(0) is -INF with signal;
     *	log10(NaN) is that NaN with no signal;
     *	log10(10**N) = N  for N=0,1,...,22.
     *
     * Constants:
     * The hexadecimal values are the intended ones for the following constants.
     * The decimal values may be used, provided that the compiler will convert
     * from decimal to binary accurately enough to produce the hexadecimal values
     * shown.
     */

    float64_t y , z;

    /* offset of the power. this allows calculating for very small or very big numbers */
    int32_t k = 0;

    /* sign of k */
    int32_t i;

    /* high part (32 bits) of x */
    int32_t hx;

    /* low part (32 bits) of x */
    uint32_t lx;


    /* high word of x */
    hx = rtmGetHighf64(x);

    /* low word of x */
    lx = rtmGetLowf64(x);

    /* if x < 2^-1022  */
    if(hx < 0x00100000)
    {
        /* if x = +-0 */
        if(((hx & RTM_FLOAT64_HI_UNMASK_SIGN) | lx) == 0)
        {
            /* log(+-0)=-inf */
            return -RTM_CONSTANT_POWER_2_54 / RTM_CONSTANT_ZERO;
        }

        /* if x < 0 */
        if(hx < 0)
        {
            /* log(-#) = NaN */
            return (x - x) / RTM_CONSTANT_ZERO;
        }

        k -= 54;

        /* subnormal number, scale up x */
        x *= RTM_CONSTANT_POWER_2_54;

        /* high word of x */
        hx = rtmGetHighf64(x);
    }

    /* if x is too high (Inf) */
    if(hx >= RTM_FLOAT64_HI_MASK_EXPONENT)
    {
        /* return Inf */
        return x + x;
    }

    k += (hx >> 20) - 1023;

    /* calculate the sign of k */
    i = ((uint32_t) k & RTM_FLOAT64_HI_MASK_SIGN) >> 31;
    hx = (hx & RTM_FLOAT64_HI_MASK_FRAC) | ((0x3ff - i) << 20);
    y = (float64_t) (k + i);

    rtmSetHighf64(&x , hx);

    /* calculate log10 using ln */
    z = y * RTM_CONSTANT_LOG10_2_LOW + RTM_CONSTANT_1_DIV_LN_10 * rtmLogf64(x);

    return z + y * RTM_CONSTANT_LOG10_2_HI;
}

/*! @} */
