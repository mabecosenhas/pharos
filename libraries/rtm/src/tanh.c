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


PHAROS_WEAK float64_t rtmTanhf64(float64_t x)
{
    /* Method:
     *				       x    -x
     *				      e  - e
     *	0. tanh(x) is defined to be -----------
     *				       x    -x
     *				      e  + e
     *	1. reduce x to non-negative by tanh(-x) = -tanh(x).
     *	2.      0  <= x <= 2^-55  : tanh(x) := x*(one+x)
     *					        -t
     *	     2^-55 <  x <= 1      : tanh(x) := -----; t = expm1(-2x)
     *					       t + 2
     *						      2
     *	         1 <= x <= 22.0   : tanh(x) := 1 -  ----- ; t=expm1(2x)
     *						    t + 2
     *	      22.0 <  x <= INF    : tanh(x) := 1.
     *
     * Special cases:
     *	tanh(NaN) is NaN;
     *	only tanh(0)=0 is exact for finite argument.
     */
    static const float64_t one = RTM_CONSTANT_ONE;

    /* aux variable (see method above) */
    float64_t t;

    /* aux variable and also the result of the function */
    float64_t z;

    /* high word of x */
    int32_t hx;

    /* High word of |x| */
    int32_t ix;


    /* High word of x */
    hx = rtmGetHighf64(x);

    /* High word of |x| */
    ix = hx & RTM_FLOAT64_HI_UNMASK_SIGN;

    /* x is Inf or NaN */
    if(ix >= RTM_FLOAT64_HI_MASK_EXPONENT)
    {
        /* if x is +Inf or NaN */
        if(hx >= 0)
        {
            /* tanh(+Inf)=+1 and  tanh(NaN) = NaN */
            z = RTM_CONSTANT_ONE / x + RTM_CONSTANT_ONE;
        }
        else
        {
            /* tanh(-Inf)=-1 */
            z = RTM_CONSTANT_ONE / x - RTM_CONSTANT_ONE;
        }
    }
        /* if |x| < 2^-55 */
    else if(ix < RTM_FLOAT_2_EXP_M55)
    {
        /* tanh(small) = small (and raise inexact exception if x != 0) */
        z = x * (RTM_CONSTANT_ONE + x);
    }
    else
    {
        /* if 2^-55 <= |x| < 22 */
        if(ix < 0x40360000)
        {
            /* if |x| >= 1  */
            if(ix >= RTM_FLOAT64_HI_ONE)
            {
                /*                  2
                 * tanh(x) := 1 - ----- ; t=expm1(2x)
                 *                t + 2
                 */

                /* calculate t = expm1(2 * |x|) in the end we correct the sign */
                t = rtmExpm1f64(RTM_CONSTANT_TWO * rtmAbsf64(x));

                /* calculate the result */
                z = RTM_CONSTANT_ONE - RTM_CONSTANT_TWO / (t + RTM_CONSTANT_TWO);
            }
                /* else, 2^-55 <= |x| < 1*/
            else
            {

                /*             -t
                 * tanh(x) := -----; t = expm1(-2x)
                 *            t + 2
                 */

                /* calculate t = expm1(-2 * |x|) in the end we correct the sign */
                t = rtmExpm1f64(-RTM_CONSTANT_TWO * rtmAbsf64(x));

                /* calculate the result */
                z = -t / (t + RTM_CONSTANT_TWO);
            }
        }
            /* |x| > 22, return +-1 */
        else
        {
            /* result is 1 with inexact flag */
            z = one - RTM_CONSTANT_TINY;
        }

        /* if x < 1 */
        if(hx < 0)
        {
            /* then change the signal of z -> remember that tanh(x) = -tanh(-x) */
            z = -z;
        }
    }

    /* return the calculated value */
    return z;
}


/*! @} */
