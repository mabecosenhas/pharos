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
 *
 */

/*! \addtogroup PHAROS-RTM
 * 
 * @{
 */

#include <pharos/rtm/rtm.h>


PHAROS_WEAK float64_t rtmCbrtf64(float64_t x)
{
    /* B1 = (682-0.03306235651)*2**20 */
    static const uint32_t B1 = 715094163;

    /* B2 = (664-0.03306235651)*2**20 */
    static const uint32_t B2 = 696219795;

    /* 19/35     = 0x3FE15F15, 0xF15F15F1 */
    static const float64_t C = 5.42857142857142815906e-01;

    /* -864/1225 = 0xBFE691DE, 0x2532C834 */
    static const float64_t D = -7.05306122448979611050e-01;

    /* 99/70     = 0x3FF6A0EA, 0x0EA0EA0F */
    static const float64_t E = 1.41428571428571436819e+00;

    /* 45/28     = 0x3FF9B6DB, 0x6DB6DB6E */
    static const float64_t F = 1.60714285714285720630e+00;

    /* 5/14      = 0x3FD6DB6D, 0xB6DB6DB7 */
    static const float64_t G = 3.57142857142857150787e-01;

    /* high word of x */
    int32_t hx;

    /* temporary value */
    float64_t r;

    /* temporary value */
    float64_t s;

    /* temporary value */
    float64_t w;

    /* temporary value */
    float64_t t = 0.0;

    /* return value of this function */
    float64_t result;

    /* sign of x */
    uint32_t sign;


    /* high word of x */
    hx = rtmGetHighf64(x);

    /* sign= sign(x) */
    sign = hx & RTM_FLOAT64_HI_MASK_SIGN;

    /* get the high word of |x| */
    hx ^= sign;

    /* if x is Inf or NaN */
    if(hx >= RTM_FLOAT64_HI_MASK_EXPONENT)
    {
        /* cbrt(NaN,INF) is itself */
        result = x + x;
    }
        /* check if x is 0 */
    else if((hx | rtmGetLowf64(x)) == 0)
    {
        /* cbrt(0) is itself */
        result = x;
    }
    else
    {
        /* x <- |x| */
        rtmSetHighf64(&x , hx);

        /* rough cbrt to 5 bits */
        if(hx < 0x00100000)
        {
            /* subnormal number */

            /* set t= 2^54 */
            rtmSetHighf64(&t , 0x43500000);

            t *= x;

            rtmSetHighf64(&t , rtmGetHighf64(t) / 3 + B2);
        }
        else
        {
            rtmSetHighf64(&t , hx / 3 + B1);
        }

        /* new cbrt to 23 bits, may be implemented in single precision */
        r = t * t / x;
        s = C + r * t;
        t *= G + F / (s + E + D / s);

        /* chopped to 20 bits and make it larger than cbrt(x) */
        rtmSetLowf64(&t , 0);
        rtmSetHighf64(&t , rtmGetHighf64(t) + 0x00000001);


        /* one step newton iteration to 53 bits with error less than 0.667 ulps */

        /* t * t is exact */
        s = t * t;
        r = x / s;
        w = t + t;

        /* r - s is exact */
        r = (r - t) / (w + r);

        result = t + t * r;

        /* restore the sign bit */
        rtmSetHighf64(&result , rtmGetHighf64(result) | sign);
    }

    /* return the calculated value */
    return result;
}

/*! @} */
