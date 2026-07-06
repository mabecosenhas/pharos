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


PHAROS_WEAK float64_t rtmAcosf64(float64_t x)
{
    /* 
     * Method :                  
     *	acos(x)  = pi/2 - asin(x)
     *	acos(-x) = pi/2 + asin(x)
     * For |x|<=0.5
     *	acos(x) = pi/2 - (x + x*x^2*R(x^2))	(see asin.c)
     * For x>0.5
     * 	acos(x) = pi/2 - (pi/2 - 2asin(sqrt((1-x)/2)))
     *		= 2asin(sqrt((1-x)/2))  
     *		= 2s + 2s*z*R(z) 	...z=(1-x)/2, s=sqrt(z)
     *		= 2f + (2c + 2s*z*R(z))
     *     where f=hi part of s, and c = (z-f*f)/(s+f) is the correction term
     *     for f so that f+c ~ sqrt(z).
     * For x<-0.5
     *	acos(x) = pi - 2asin(sqrt((1-|x|)/2))
     *		= pi - 0.5*(s+s*z*R(z)), where z=(1-|x|)/2,s=sqrt(z)
     *
     * Special cases:
     *	if x is NaN, return x itself;
     *	if |x|>1, return NaN with invalid signal.
     *
     * Function needed: sqrt
     */

    /* 0x3FC55555, 0x55555555 */
    static const float64_t pS0 = 1.66666666666666657415e-01;

    /* 0xBFD4D612, 0x03EB6F7D */
    static const float64_t pS1 = -3.25565818622400915405e-01;

    /* 0x3FC9C155, 0x0E884455 */
    static const float64_t pS2 = 2.01212532134862925881e-01;

    /* 0xBFA48228, 0xB5688F3B */
    static const float64_t pS3 = -4.00555345006794114027e-02;

    /* 0x3F49EFE0, 0x7501B288 */
    static const float64_t pS4 = 7.91534994289814532176e-04;

    /* 0x3F023DE1, 0x0DFDF709 */
    static const float64_t pS5 = 3.47933107596021167570e-05;

    /* 0xC0033A27, 0x1C8A2D4B */
    static const float64_t qS1 = -2.40339491173441421878e+00;

    /* 0x40002AE5, 0x9C598AC8 */
    static const float64_t qS2 = 2.02094576023350569471e+00;

    /* 0xBFE6066C, 0x1B8D0159 */
    static const float64_t qS3 = -6.88283971605453293030e-01;

    /* 0x3FB3B8C5, 0xB12E9282 */
    static const float64_t qS4 = 7.70381505559019352791e-02;

    /* value of the calculated result */
    float64_t result;

    float64_t z;
    float64_t p;
    float64_t q;
    float64_t r;
    float64_t w;
    float64_t s;
    float64_t c;
    float64_t df;

    /* high word of x */
    int32_t hx;

    /* high word of |x| */
    int32_t ix;

    /* low word of x for bit manipulation */
    int32_t lx;


    /* high word of x */
    hx = rtmGetHighf64(x);

    /* high word of |x| */
    ix = hx & RTM_FLOAT64_HI_UNMASK_SIGN;

    /* low word of x */
    lx = rtmGetLowf64(x);

    /* if |x| >= 1 */
    if (ix >= RTM_FLOAT64_HI_ONE)
    {
        /* if |x| == 1 */
        if (((ix - RTM_FLOAT64_HI_ONE) | lx) == 0)
        {
            /* if calculating acos(1) */
            if (hx > 0)
            {
                result = 0.0;
            }
            else
            {
                /* acos(-1) = pi */
                result = RTM_CONSTANT_PI + (2.0 * RTM_CONSTANT_PI_DIV_2_LOW);
            }
        }
        else
        {
            /* acos(|x|>1) is NaN - result will be NaN */
            result = 0.0;
            result = result / result;
        }
    }
    /* if |x| < 0.5 */
    else if (ix < 0x3fe00000)
    {
        /* if |x| < 2^-57 */
        if (ix <= 0x3c600000)
        {
            /* return PI / 2 */
            result = RTM_CONSTANT_PI_DIV_2_HI + RTM_CONSTANT_PI_DIV_2_LOW;
        }
        else
        {
            /* calculate through pi/2 - (x + x*x^2*R(x^2)) */

            /* calculate x^2 */
            z = x * x;

            p = z * (pS0 + (z * (pS1 + (z * (pS2 + (z * (pS3 + (z * (pS4 + (z * pS5))))))))));
            q = RTM_CONSTANT_ONE + (z * (qS1 + (z * (qS2 + (z * (qS3 + (z * qS4)))))));

            r = p / q;

            result = RTM_CONSTANT_PI_DIV_2_HI - (x - (RTM_CONSTANT_PI_DIV_2_LOW - (x * r)));
        }
    }
    /* if -1 < x <= -0.5 */
    else if (hx < 0)
    {
        /* calculate z = (1 + x) / 2 and since x is negative, this is equal to z = (1-|x|)/2 */
        z = (RTM_CONSTANT_ONE + x) * RTM_CONSTANT_HALF;

        p = z * (pS0 + (z * (pS1 + (z * (pS2 + (z * (pS3 + (z * (pS4 + (z * pS5))))))))));
        q = RTM_CONSTANT_ONE + (z * (qS1 + (z * (qS2 + (z * (qS3 + (z * qS4)))))));

        /* calculate s = sqrt( (1-|x|)/2 ) */
        s = rtmSqrtf64(z);
        r = p / q;
        w = (r * s) - RTM_CONSTANT_PI_DIV_2_LOW;

        /* return pi - 2asin(sqrt((1-|x|)/2)) */
        result = RTM_CONSTANT_PI - (2.0 * (s + w));
    }
    /* else, then 1 > x >= 0.5 */
    else
    {
        /* assign z = (1-x) / 2 */
        z = (RTM_CONSTANT_ONE - x) * RTM_CONSTANT_HALF;

        /* assign s = sqrt( (1-x) / 2 ) */
        s = rtmSqrtf64(z);

        /* assign df = sqrt( (1-x) / 2 ) */
        df = s;

        /* reset the lower fractional bits */
        rtmSetLowf64(&df, 0);

        c = (z - (df * df)) / (s + df);
        p = z * (pS0 + (z * (pS1 + (z * (pS2 + (z * (pS3 + (z * (pS4 + (z * pS5))))))))));
        q = RTM_CONSTANT_ONE + (z * (qS1 + (z * (qS2 + (z * (qS3 + (z * qS4)))))));
        r = p / q;
        w = (r * s) + c;

        /* return 2asin(sqrt((1-x)/2)) */
        result = 2.0 * (df + w);
    }

    /* return the calculated value */
    return result;
}

/*! @} */
