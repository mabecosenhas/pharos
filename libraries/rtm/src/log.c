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


PHAROS_WEAK float64_t rtmLogf64(float64_t x)
{
    /*Method :                  
     *   1. Argument Reduction: find k and f such that 
     *			x = 2^k * (1+f), 
     *	   where  sqrt(2)/2 < 1+f < sqrt(2) .
     *
     *   2. Approximation of log(1+f).
     *	Let s = f/(2+f) ; based on log(1+f) = log(1+s) - log(1-s)
     *		 = 2s + 2/3 s**3 + 2/5 s**5 + .....,
     *	     	 = 2s + s*R
     *      We use a special Reme algorithm on [0,0.1716] to generate 
     * 	a polynomial of degree 14 to approximate R The maximum error 
     *	of this polynomial approximation is bounded by 2**-58.45. In
     *	other words,
     *		        2      4      6      8      10      12      14
     *	    R(z) ~ Lg1*s +Lg2*s +Lg3*s +Lg4*s +Lg5*s  +Lg6*s  +Lg7*s
     *  	(the values of Lg1 to Lg7 are listed in the program)
     *	and
     *	    |      2          14          |     -58.45
     *	    | Lg1*s +...+Lg7*s    -  R(z) | <= 2 
     *	    |                             |
     *	Note that 2s = f - s*f = f - hfsq + s*hfsq, where hfsq = f*f/2.
     *	In order to guarantee error in log below 1ulp, we compute log
     *	by
     *		log(1+f) = f - s*(f - R)	(if f is not too large)
     *		log(1+f) = f - (hfsq - s*(hfsq+R)).	(better accuracy)
     *	
     *	3. Finally,  log(x) = k*ln2 + log(1+f).  
     *			    = k*ln2_hi+(f-(hfsq-(s*(hfsq+R)+k*ln2_lo)))
     *	   Here ln2 is split into two floating point number: 
     *			ln2_hi + ln2_lo,
     *	   where n*ln2_hi is always exact for |n| < 2000.
     *
     * Special cases:
     *	log(x) is NaN with signal if x < 0 (including -INF) ; 
     *	log(+INF) is +INF; log(0) is -INF with signal;
     *	log(NaN) is that NaN with no signal.
     *
     * Accuracy:
     *	according to an error analysis, the error is always less than
     *	1 ulp (unit in the last place).
     *
     * Constants:
     * The hexadecimal values are the intended ones for the following 
     * constants. The decimal values may be used, provided that the 
     * compiler will convert from decimal to binary accurately enough 
     * to produce the hexadecimal values shown.
     */


    /* 3FE55555 55555593 */
    static const float64_t Lg1 = 6.666666666666735130e-01;

    /* 3FD99999 9997FA04 */
    static const float64_t Lg2 = 3.999999999940941908e-01;

    /* 3FD24924 94229359 */
    static const float64_t Lg3 = 2.857142874366239149e-01;

    /* 3FCC71C5 1D8E78AF */
    static const float64_t Lg4 = 2.222219843214978396e-01;

    /* 3FC74664 96CB03DE */
    static const float64_t Lg5 = 1.818357216161805012e-01;

    /* 3FC39A09 D078C69F */
    static const float64_t Lg6 = 1.531383769920937332e-01;

    /* 3FC2F112 DF3E5244 */
    static const float64_t Lg7 = 1.479819860511658591e-01;

    float64_t hfsq , f , s , z , R , w , t1 , t2 , dk;
    int32_t k , hx , i , j;
    uint32_t lx;


    /* high word of x */
    hx = rtmGetHighf64(x);

    /* low  word of x */
    lx = rtmGetLowf64(x);

    k = 0;

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
            /* log(negative) = NaN */
            return (x - x) / RTM_CONSTANT_ZERO;
        }

        k -= 54;

        /* subnormal number, scale up x */
        x *= RTM_CONSTANT_POWER_2_54;

        /* high word of x */
        hx = rtmGetHighf64(x);
    }

    if(hx >= RTM_FLOAT64_HI_MASK_EXPONENT)
    {
        return x + x;
    }

    k += (hx >> 20) - 1023;
    hx &= RTM_FLOAT64_HI_MASK_FRAC;
    i = (hx + 0x95f64)&0x100000;

    /* normalize x or x/2 */

    rtmSetHighf64(&x , hx | (i^RTM_FLOAT64_HI_ONE));
    k += (i >> 20);
    f = x - RTM_CONSTANT_ONE;

    /* if |f| < 2^-20 */
    if((RTM_FLOAT64_HI_MASK_FRAC & (2 + hx)) < 3)
    {

        if(f == RTM_CONSTANT_ZERO)
        {
            if(k == 0)
            {
                return RTM_CONSTANT_ZERO;
            }
            else
            {
                dk = (float64_t) k;
                return dk * RTM_CONSTANT_LN2_HI + dk*RTM_CONSTANT_LN2_LOW;
            }
        }

        R = f * f * (RTM_CONSTANT_HALF - 0.33333333333333333 * f);
        if(k == 0)
        {
            return f - R;
        }
        else
        {
            dk = (float64_t) k;
            return dk * RTM_CONSTANT_LN2_HI - ((R - dk * RTM_CONSTANT_LN2_LOW) - f);
        }
    }

    s = f / (2.0 + f);
    dk = (float64_t) k;
    z = s * s;
    i = hx - 0x6147a;
    w = z * z;
    j = 0x6b851 - hx;
    t1 = w * (Lg2 + w * (Lg4 + w * Lg6));
    t2 = z * (Lg1 + w * (Lg3 + w * (Lg5 + w * Lg7)));
    i |= j;
    R = t2 + t1;

    if(i > 0)
    {
        hfsq = RTM_CONSTANT_HALF * f * f;

        if(k == 0)
        {
            return f - (hfsq - s * (hfsq + R));
        }
        else
        {
            return dk * RTM_CONSTANT_LN2_HI - ((hfsq - (s * (hfsq + R) + dk * RTM_CONSTANT_LN2_LOW)) - f);
        }
    }
    else
    {
        if(k == 0)
        {
            return f - s * (f - R);
        }
        else
        {
            return dk * RTM_CONSTANT_LN2_HI - ((s * (f - R) - dk * RTM_CONSTANT_LN2_LOW) - f);
        }
    }
}

/*! @} */
