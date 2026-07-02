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


PHAROS_WEAK float64_t rtmHypotf64(float64_t x , float64_t y)
{
    /* _Method :                  
     * If (assume round-to-nearest) z=x*x+y*y 
     * has error less than sqrt(2)/2 ulp, than 
     * sqrt(z) has error less than 1 ulp (exercise).
     *
     * So, compute sqrt(x*x+y*y) with some care as 
     * follows to get the error below 1 ulp:
     *
     * Assume x>y>0;
     * (if possible, set rounding to round-to-nearest)
     * 1. if x > 2y  use x1*x1+(y*y+(x2*(x+x1))) for x*x+y*y
     * where x1 = x with lower 32 bits cleared, x2 = x-x1; else
     * 2. if x <= 2y use t1*y1+((x-y)*(x-y)+(t1*y2+t2*y))
     * where t1 = 2x with lower 32 bits cleared, t2 = 2x-t1, 
     * y1= y with lower 32 bits chopped, y2 = y-y1.
     *	
     * NOTE: scaling may be necessary if some argument is too large or too tiny
     *
     * Special cases:
     * if x or y is +-Inf => hypot(x,y) is +Inf 
     * if x or y is NaN   => hypot(x,y) is NaN 
     *
     * Accuracy:
     * hypot(x,y) returns sqrt(x^2+y^2) with error less than 1 ulps (units in the last place) 
     */

    /* holds the number (x or y) that has the bigger high word */
    float64_t a;

    /* holds the number (x or y) that has the smaller  high word */
    float64_t b;

    float64_t t1;

    float64_t t2;

    float64_t y1;

    float64_t y2;

    float64_t w;

    /* aux variable */
    int32_t j;

    int32_t k;

    int32_t ha;

    int32_t hb;


    /* high word of |x| */
    ha = rtmGetHighf64(x) & RTM_FLOAT64_HI_UNMASK_SIGN;

    /* high word of |y| */
    hb = rtmGetHighf64(y) & RTM_FLOAT64_HI_UNMASK_SIGN;

    /* if |y| > |x| (except for the low word) */
    if(hb > ha)
    {
        /* place in a the bigger word */
        a = y;

        /* place in a the smaller word */
        b = x;

        /* switch ha with hb */
        j = ha;
        ha = hb;
        hb = j;
    }
    else
    {
        /* place in a the bigger word */
        a = x;

        /* place in a the smaller word */
        b = y;
    }

    /* a <- |a| */
    rtmSetHighf64(&a , ha);

    /* b <- |b| */
    rtmSetHighf64(&b , hb);

    /* if x/y > 2^60 or y/x > 2^60 */
    if((ha - hb) > 0x3c00000)
    {
        /* the difference between x and y is too great, so sqrt(x^2+y^2) ~ x + y */
        return a + b;
    }

    k = 0;

    /* if a > 2^500 */
    if(ha > 0x5f300000)
    {
        /* if Inf or NaN */
        if(ha >= RTM_FLOAT64_HI_MASK_EXPONENT)
        {
            /* for sNaN */
            w = a + b;

            if(((ha & 0xfffff) | rtmGetLowf64(a)) == 0)
            {
                w = a;
            }

            if(((hb^RTM_FLOAT64_HI_MASK_EXPONENT) | rtmGetLowf64(b)) == 0)
            {
                w = b;
            }

            return w;
        }
        else
        {
            /* scale a and b by 2^-600 */
            ha -= 0x25800000;
            hb -= 0x25800000;
            k += 600;

            rtmSetHighf64(&a , ha);
            rtmSetHighf64(&b , hb);
        }
    }

    /* if b < 2^-500 */
    if(hb < 0x20b00000)
    {
        /* if subnormal b or 0 */
        if(hb <= RTM_FLOAT64_HI_MASK_FRAC)
        {
            /* if b = 0 */
            if((hb | (rtmGetLowf64(b))) == 0)
            {
                return a;
            }
            else
            {
                t1 = 0;

                /* t1=2^1022 */
                rtmSetHighf64(&t1 , 0x7fd00000);

                b *= t1;
                a *= t1;
                k -= 1022;
            }
        }
        else
        {
            /* scale a and b by 2^600 */

            /* a *= 2^600 */
            ha += 0x25800000;

            /* b *= 2^600 */
            hb += 0x25800000;
            k -= 600;

            rtmSetHighf64(&a , ha);
            rtmSetHighf64(&b , hb);
        }
    }

    /* medium size a and b */
    w = a - b;

    if(w > b)
    {
        t1 = 0;
        rtmSetHighf64(&t1 , ha);
        t2 = a - t1;
        w = rtmSqrtf64(t1 * t1 - (b * (-b) - t2 * (a + t1)));
    }
    else
    {
        a = a + a;
        y1 = 0;
        rtmSetHighf64(&y1 , hb);

        y2 = b - y1;
        t1 = 0;
        rtmSetHighf64(&t1 , ha + 0x00100000);

        t2 = a - t1;
        w = rtmSqrtf64(t1 * y1 - (w * (-w)-(t1 * y2 + t2 * b)));
    }

    if(k != 0)
    {
        t1 = RTM_CONSTANT_ONE;

        rtmSetHighf64(&t1 , rtmGetHighf64(t1) + (k << 20));

        return t1 * w;
    }
    else
    {
        return w;
    }
}

/*! @} */
