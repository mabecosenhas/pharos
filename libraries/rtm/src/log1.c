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


PHAROS_WEAK float64_t rtmLog1f64(float64_t x)
{
    /* Method :                  
     *   1. Argument Reduction: find k and f such that 
     *			1+x = 2^k * (1+f), 
     *	   where  sqrt(2)/2 < 1+f < sqrt(2) .
     *
     *      Note. If k=0, then f=x is exact. However, if k!=0, then f
     *	may not be representable exactly. In that case, a correction
     *	term is need. Let u=1+x rounded. Let c = (1+x)-u, then
     *	log(1+x) - log(u) ~ c/u. Thus, we proceed to compute log(u),
     *	and add back the correction term c/u.
     *	(Note: when x > 2**53, one can simply return log(x))
     *
     *   2. Approximation of log1p(f).
     *	Let s = f/(2+f) ; based on log(1+f) = log(1+s) - log(1-s)
     *		 = 2s + 2/3 s**3 + 2/5 s**5 + .....,
     *	     	 = 2s + s*R
     *      We use a special Reme algorithm on [0,0.1716] to generate 
     * 	a polynomial of degree 14 to approximate R The maximum error 
     *	of this polynomial approximation is bounded by 2**-58.45. In
     *	other words,
     *		        2      4      6      8      10      12      14
     *	    R(z) ~ Lp1*s +Lp2*s +Lp3*s +Lp4*s +Lp5*s  +Lp6*s  +Lp7*s
     *  	(the values of Lp1 to Lp7 are listed in the program)
     *	and
     *	    |      2          14          |     -58.45
     *	    | Lp1*s +...+Lp7*s    -  R(z) | <= 2 
     *	    |                             |
     *	Note that 2s = f - s*f = f - hfsq + s*hfsq, where hfsq = f*f/2.
     *	In order to guarantee error in log below 1ulp, we compute log
     *	by
     *		log1p(f) = f - (hfsq - s*(hfsq+R)).
     *	
     *	3. Finally, log1p(x) = k*ln2 + log1p(f).  
     *		 	     = k*ln2_hi+(f-(hfsq-(s*(hfsq+R)+k*ln2_lo)))
     *	   Here ln2 is split into two floating point number: 
     *			ln2_hi + ln2_lo,
     *	   where n*ln2_hi is always exact for |n| < 2000.
     *
     * Special cases:
     *	log1p(x) is NaN with signal if x < -1 (including -INF) ; 
     *	log1p(+INF) is +INF; log1p(-1) is -INF with signal;
     *	log1p(NaN) is that NaN with no signal.
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
     *
     * Note: Assuming log() return accurate answer, the following
     * 	 algorithm can be used to compute log1p(x) to within a few ULP:
     *	
     *		u = 1+x;
     *		if(u==1.0) return x ; else
     *			   return log(u)*(x/(u-1.0));
     *
     *	 See HP-15C Advanced Functions Handbook, p.193.
     */

    /* high(ln(2)) 3fe62e42 fee00000 */
    static const float64_t ln2_hi = RTM_CONSTANT_LN2_HI;

    /* low(ln(2)) 3dea39ef 35793c76 */
    static const float64_t ln2_lo = RTM_CONSTANT_LN2_LOW;

    /* 2^54 43500000 00000000 */
    static const float64_t two54 = RTM_CONSTANT_POWER_2_54;

    /* 3FE55555 55555593 */
    static const float64_t Lp1 = 6.666666666666735130e-01;

    /* 3FD99999 9997FA04 */
    static const float64_t Lp2 = 3.999999999940941908e-01;

    /* 3FD24924 94229359 */
    static const float64_t Lp3 = 2.857142874366239149e-01;

    /* 3FCC71C5 1D8E78AF */
    static const float64_t Lp4 = 2.222219843214978396e-01;

    /* 3FC74664 96CB03DE */
    static const float64_t Lp5 = 1.818357216161805012e-01;

    /* 3FC39A09 D078C69F */
    static const float64_t Lp6 = 1.531383769920937332e-01;

    /* 3FC2F112 DF3E5244 */
    static const float64_t Lp7 = 1.479819860511658591e-01;

    float64_t hfsq , f , c , s , z , R , u;

    /* aux variable */
    int32_t k;

    /* high word of x */
    int32_t hx;

    /* aux variable with the high word of u */
    int32_t hu;

    /* high word of |x| */
    int32_t ax;


    /* high word of x */
    hx = rtmGetHighf64(x);

    /* high word of |x| */
    ax = hx & RTM_FLOAT64_HI_UNMASK_SIGN;

    k = 1;

    /* if x < 0.41422  */
    if(hx < 0x3FDA827A)
    {
        /* if x <= -1.0 */
        if(ax >= RTM_FLOAT64_HI_ONE)
        {
            if(x == -RTM_CONSTANT_ONE)
            {
                /* log1p(-1)=+inf */
                return -two54 / RTM_CONSTANT_ZERO;
            }
            else
            {
                /* log1p(x<-1)=NaN */
                return (x - x) / (x - x);
            }
        }

        /* if |x| < 2^-29 */
        if(ax < 0x3e200000)
        {
            /* raise inexact if x != 0 and check if |x| < 2^-54 */
            if((RTM_CONSTANT_POWER_2_54 + x > RTM_CONSTANT_ZERO) && (ax < 0x3c900000))
            {
                /* log1p(very small) ~ very small */
                return x;
            }
                /* else, we have 2^-29 > |x| > 2^-54*/
            else
            {
                /* log1p(small) ~ small - small^2/2 */
                return x - x * x * RTM_CONSTANT_HALF;
            }
        }

        /* -0.2929 < x < 0.41422 */
        if(hx > 0 || hx <= ((int32_t) 0xbfd2bec3))
        {
            k = 0;
            f = x;
            hu = 1;
        }
    }

    /* if x is NaN or +-Inf */
    if(hx >= RTM_FLOAT64_HI_MASK_EXPONENT)
    {
        /* return itself, raise overflow exception if +-Inf */
        return x + x;
    }

    if(k != 0)
    {
        if(hx < 0x43400000)
        {
            u = RTM_CONSTANT_ONE + x;

            /* high word of u */
            hu = rtmGetHighf64(u);

            k = (hu >> 20) - 1023;

            /* correction term */
            c = (k > 0) ? RTM_CONSTANT_ONE - (u - x) : x - (u - RTM_CONSTANT_ONE);

            c /= u;
        }
        else
        {
            u = x;

            /* high word of u */
            hu = rtmGetHighf64(u);
            k = (hu >> 20) - 1023;
            c = 0;
        }

        hu &= RTM_FLOAT64_HI_MASK_FRAC;

        if(hu < 0x6a09e)
        {
            /* normalise u */
            rtmSetHighf64(&u , hu | RTM_FLOAT64_HI_ONE);
        }
        else
        {
            k += 1;

            /* normalise u/2 */
            rtmSetHighf64(&u , hu | 0x3fe00000);
            hu = (0x00100000 - hu) >> 2;
        }

        f = u - RTM_CONSTANT_ONE;
    }

    hfsq = RTM_CONSTANT_HALF * f*f;

    /* if |f| < 2^-20 */
    if(hu == 0)
    {
        if(f == RTM_CONSTANT_ZERO)
        {
            if(k == 0)
            {
                return RTM_CONSTANT_ZERO;
            }
            else
            {
                c += k*ln2_lo;
                return k * ln2_hi + c;
            }
        }

        R = hfsq * (RTM_CONSTANT_ONE - 0.66666666666666666 * f);

        if(k == 0)
        {
            return f - R;
        }
        else
        {
            return k * ln2_hi - ((R - (k * ln2_lo + c)) - f);
        }
    }

    s = f / (2.0 + f);
    z = s*s;
    R = z * (Lp1 + z * (Lp2 + z * (Lp3 + z * (Lp4 + z * (Lp5 + z * (Lp6 + z * Lp7))))));

    if(k == 0)
    {
        return f - (hfsq - s * (hfsq + R));
    }
    else
    {
        return k * ln2_hi - ((hfsq - (s * (hfsq + R)+(k * ln2_lo + c))) - f);
    }
}

/*! @} */
