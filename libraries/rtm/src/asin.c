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


PHAROS_WEAK float64_t rtmAsinf64(float64_t x)
{
    /* Since  asin(x) = x + x^3/6 + x^5*3/40 + x^7*15/336 + ...
     *	we approximate asin(x) on [0,0.5] by
     *	        asin(x) = x + x*x^2*R(x^2)
     *	where
     *          R(x^2) is a rational approximation of (asin(x)-x)/x^3 
     *	and its remez error is bounded by
     *          |(asin(x)-x)/x^3 - R(x^2)| < 2^(-58.75)
     *
     *	For x in [0.5,1]
     *	         asin(x) = pi/2-2*asin(sqrt((1-x)/2))
     *	Let y = (1-x), z = y/2, s := sqrt(z), and pio2_hi+pio2_lo=pi/2;
     *	then for x > 0.98
     *	         asin(x) = pi/2 - 2*(s+s*z*R(z))
     *	                 = pio2_hi - (2*(s+s*z*R(z)) - pio2_lo)
     *	For x<=0.98, let pio4_hi = pio2_hi/2, then
     *	        f = hi part of s;
     *	        c = sqrt(z) - f = (z-f*f)/(s+f) ... f + c = sqrt(z)
     *	and
     *	        asin(x) = pi/2 - 2*(s+s*z*R(z))
     *	                = pio4_hi+(pio4-2s)-(2s*z*R(z)-pio2_lo)
     *	                = pio4_hi+(pio4-2f)-(2s*z*R(z)-(pio2_lo+2c))
     *
     * Special cases:
     *	if x is NaN, return x itself;
     *	if |x|>1, return NaN with invalid signal.
     *
     */

    /* coefficients for R(x^2) */

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

    float64_t t , w , p , q , c , r , s;

    /* high part of x */
    int32_t hx;

    /* high part of |x| */
    int32_t ix;


    /* get the 32 high bits */
    hx = rtmGetHighf64(x);

    /* clear the signal */
    ix = hx & RTM_FLOAT64_HI_UNMASK_SIGN;


    /* |x| >= 1 */
    if(ix >= RTM_FLOAT64_HI_ONE)
    {
        /* if |x| = 1 */
        if(((ix - RTM_FLOAT64_HI_ONE) | rtmGetLowf64(x)) == 0)
        {
            /* asin(1) = +-pi/2 with inexact */
            return x * RTM_CONSTANT_PI_DIV_2_HI + x * RTM_CONSTANT_PI_DIV_2_LOW;
        }
            /* else, |x| > 1 */
        else
        {
            /* asin(|x| > 1) is NaN */
            return (x - x) / (x - x);
        }
    }
        /* |x| < 0.5 */
    else if(ix < RTM_FLOAT64_HI_1_DIV_2)
    {

        /* if |x| < 2^-27 */
        if(ix < RTM_FLOAT64_2_EXP_M27)
        {
            /* return x */
            return x;
        }

        t = x * x;
        p = t * (pS0 + t * (pS1 + t * (pS2 + t * (pS3 + t * (pS4 + t * pS5)))));
        q = RTM_CONSTANT_ONE + t * (qS1 + t * (qS2 + t * (qS3 + t * qS4)));
        w = p / q;

        return x + x*w;
    }
        /* 1 > |x| >= 0.5 */
    else
    {
        w = RTM_CONSTANT_ONE - rtmAbsf64(x);
        t = w * RTM_CONSTANT_HALF;
        p = t * (pS0 + t * (pS1 + t * (pS2 + t * (pS3 + t * (pS4 + t * pS5)))));
        q = RTM_CONSTANT_ONE + t * (qS1 + t * (qS2 + t * (qS3 + t * qS4)));
        s = rtmSqrtf64(t);

        /* if |x| > 0.975 */
        if(ix >= 0x3FEF3333)
        {
            w = p / q;
            t = RTM_CONSTANT_PI_DIV_2_HI - (2.0 * (s + s * w) - RTM_CONSTANT_PI_DIV_2_LOW);
        }
            /* else 0.5 < |x| < 0.975 */
        else
        {
            w = s;

            rtmSetLowf64(&w , 0);

            c = (t - w * w) / (s + w);
            r = p / q;
            p = 2.0 * s * r - (RTM_CONSTANT_PI_DIV_2_LOW - 2.0 * c);
            q = RTM_CONSTANT_PI_DIV_4_HI - 2.0 * w;
            t = RTM_CONSTANT_PI_DIV_4_HI - (p - q);
        }

        /* check the signal */
        if(hx > 0)
        {
            return t;
        }
        else
        {
            return -t;
        }
    }
}

/*! @} */
