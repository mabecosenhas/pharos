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
 * Copyright (C) 2004 by Sun Microsystems, Inc. All rights reserved.
 *
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


PHAROS_WEAK float64_t rtmExpf64(float64_t x)
{
    /* Method
     *   1. Argument reduction:
     *      Reduce x to an r so that |r| <= 0.5*ln2 ~ 0.34658.
     *	Given x, find r and integer k such that
     *
     *               x = k*ln2 + r,  |r| <= 0.5*ln2.  
     *
     *  Here r will be represented as r = hi-lo for better accuracy.
     *
     *   2. Approximation of exp(r) by a special rational function on
     *	the interval [0,0.34658]:
     *	Write
     *	    R(r**2) = r*(exp(r)+1)/(exp(r)-1) = 2 + r*r/6 - r**4/360 + ...
     *      We use a special Remes algorithm on [0,0.34658] to generate 
     * 	a polynomial of degree 5 to approximate R. The maximum error 
     *	of this polynomial approximation is bounded by 2**-59. In
     *	other words,
     *	    R(z) ~ 2.0 + P1*z + P2*z**2 + P3*z**3 + P4*z**4 + P5*z**5
     *  	(where z=r*r, and the values of P1 to P5 are listed below)
     *	and
     *	    |                  5          |     -59
     *	    | 2.0+P1*z+...+P5*z   -  R(z) | <= 2 
     *	    |                             |
     *	The computation of exp(r) thus becomes
     *                             2*r
     *	         exp(r) = 1 + -------
     *		              R - r
     *                              r*R1(r)	
     *		        = 1 + r + ----------- (for better accuracy)
     *		                  2 - R1(r)
     *	where
     *	                         2       4             10
     *	        R1(r) = r - (P1*r  + P2*r  + ... + P5*r  )
     *	
     *   3. Scale back to obtain exp(x):
     *	From step 1, we have
     *	   exp(x) = 2^k * exp(r)
     *
     * Special cases:
     *	exp(INF) is INF, exp(NaN) is NaN;
     *	exp(-INF) is 0, and
     *	for finite argument, only exp(0)=1 is exact.
     *
     * Accuracy:
     *	according to an error analysis, the error is always less than
     *	1 ulp (unit in the last place).
     *
     * Misc. info.
     *	For IEEE float64_t 
     *	    if x >  7.09782712893383973096e+02 then exp(x) overflow
     *	    if x < -7.45133219101941108420e+02 then exp(x) underflow
     *
     * Constants:
     * The hexadecimal values are the intended ones for the following 
     * constants. The decimal values may be used, provided that the 
     * compiler will convert from decimal to binary accurately enough
     * to produce the hexadecimal values shown.
     */

    static const float64_t halF[2] = {RTM_CONSTANT_HALF , -RTM_CONSTANT_HALF};

    /* 2**-1000=0x01700000,0*/
    static const float64_t twom1000 = 9.33263618503218878990e-302;

    /* 0x40862E42, 0xFEFA39EF */
    static const float64_t o_threshold = 7.09782712893383973096e+02;

    /* 0xc0874910, 0xD52D3051 */
    static const float64_t u_threshold = -7.45133219101941108420e+02;

    /* high(ln(2)) */
    static const float64_t ln2HI[2] = {
        /* 0x3fe62e42, 0xfee00000 */
        6.93147180369123816490e-01 ,

        /* 0xbfe62e42, 0xfee00000 */
        -6.93147180369123816490e-01
    };

    /* low(ln(2)) */
    static const float64_t ln2LO[2] = {

        /* 0x3dea39ef, 0x35793c76 */
        1.90821492927058770002e-10 ,

        /* 0xbdea39ef, 0x35793c76 */
        -1.90821492927058770002e-10
    };

    /* 0x3FC55555, 0x5555553E */
    static const float64_t P1 = 1.66666666666666019037e-01;

    /* 0xBF66C16C, 0x16BEBD93 */
    static const float64_t P2 = -2.77777777770155933842e-03;

    /* 0x3F11566A, 0xAF25DE2C */
    static const float64_t P3 = 6.61375632143793436117e-05;

    /* 0xBEBBBD41, 0xC5D26BF1 */
    static const float64_t P4 = -1.65339022054652515390e-06;

    /* 0x3E663769, 0x72BEA4D0 */
    static const float64_t P5 = 4.13813679705723846039e-08;

    float64_t y , hi , lo , c , t;
    int32_t k , xsb;

    /* high word of x */
    int32_t hx;

    /* high word of |x| */
    uint32_t ix;


    /* high word of x */
    hx = rtmGetHighf64(x);

    /* sign bit of x */
    xsb = (hx >> 31) & 1;

    /* high word of |x| */
    ix = hx & RTM_FLOAT64_HI_UNMASK_SIGN;

    /* filter out non-finite argument */

    /* if |x| >= 709.78... */
    if(ix >= 0x40862E42)
    {
        /* if is nan or +-Inf */
        if(ix >= RTM_FLOAT64_HI_MASK_EXPONENT)
        {
            /* check for NaN */
            if(((ix & 0xfffff) | rtmGetLowf64(x)) != 0)
            {
                /* NaN */
                return x + x;
            }
                /* if +Inf */
            else if(xsb == 0)
            {

                /* return +Inf */
                return x;
            }
                /* if -Inf */
            else
            {
                /* return 0.0 */
                return 0.0;
            }
        }
        else
        {
            /* if x is too big */
            if(x > o_threshold)
            {
                /* overflow */
                return RTM_CONSTANT_HUGE * RTM_CONSTANT_HUGE;
            }

            /* if x is too small (x < -709) */
            if(x < u_threshold)
            {
                /* underflow */
                return twom1000 * twom1000;
            }
        }
    }

    /* argument reduction */

    /* if  |x| > 0.5 ln2 */
    if(ix > 0x3fd62e42)
    {
        /* if |x| < 1.5 ln2 */
        if(ix < 0x3FF0A2B2)
        {
            /* check the special case where x is +1 */
            if(((hx - RTM_FLOAT64_HI_ONE) | rtmGetLowf64(x)) == 0)
            {
                /* e^1 = e */
                return RTM_CONSTANT_E;
            }
            else
            {
                hi = x - ln2HI[xsb];
                lo = ln2LO[xsb];
                k = 1 - xsb - xsb;
            }
        }
            /* if |x| >= 1.5 ln2 */
        else
        {
            k = (int32_t) (RTM_CONSTANT_1_DIV_LN2 * x + halF[xsb]);

            /* t * ln2HI is exact here */
            hi = x - k * ln2HI[0];
            lo = k * ln2LO[0];
        }

        x = hi - lo;

        /* x is now in primary range */

        /* calculate x^2 */
        t = x * x;

        /* calculate c */
        c = x - t * (P1 + t * (P2 + t * (P3 + t * (P4 + t * P5))));

        y = RTM_CONSTANT_ONE - ((lo - (x * c) / (2.0 - c)) - hi);

        if(k >= -1021)
        {
            /* add k to y's exponent */
            rtmSetHighf64(&y , rtmGetHighf64(y) + (k << 20));

            return y;
        }
        else
        {
            /* add k to y's exponent */
            rtmSetHighf64(&y , rtmGetHighf64(y) + ((k + 1000) << 20));

            return y*twom1000;
        }
    }
        /* if |x| < 2^-28 */
    else if(ix < RTM_FLOAT64_2_EXP_M28)
    {
        /* return 1 + x */
        return RTM_CONSTANT_ONE + x;
    }
        /* if 2^-28 <= |x| <= 0.5 ln2 */
    else
    {
        /* x is now in primary range */
        t = x*x;

        /* calculate c */
        c = x - t * (P1 + t * (P2 + t * (P3 + t * (P4 + t * P5))));

        return RTM_CONSTANT_ONE - ((x * c) / (c - 2.0) - x);
    }
}

/*! @} */
