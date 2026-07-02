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


PHAROS_WEAK float64_t rtmAtanhf64(float64_t x)
{
    /* Method :
     *    1.Reduced x to positive by atanh(-x) = -atanh(x)
     *    2.For x>=0.5
     *              1              2x                          x
     *	atanh(x) = --- * log(1 + -------) = 0.5 * log1p(2 * --------)
     *              2             1 - x                      1 - x
     *	
     * 	For x < 0.5
     *	atanh(x) = 0.5*log1p(2x+2x*x/(1-x))
     *
     * Special cases:
     *  atanh(x) is NaN if |x| > 1 with signal
     *  atanh(NaN) is that NaN with no signal
     *  atanh(+-1) is +-INF with signal
     */

    /* auxiliary variable to store temporary value and also result of the function */
    float64_t t;

    /* high word of x */
    int32_t hx;

    /* low word of x */
    uint32_t lx;

    /* high word of |x| */
    int32_t ix;


    /* high word */
    hx = rtmGetHighf64(x);

    /* low word */
    lx = rtmGetLowf64(x);

    /* get the 32 high bits of |x| */
    ix = hx & RTM_FLOAT64_HI_UNMASK_SIGN;

    /* if |x|>1 */
    if((ix | ((lx | (-lx)) >> 31)) > RTM_FLOAT64_HI_ONE)
    {
        t = (x - x) / (x - x);
    }
        /* if |x| == 1 */
    else if(ix == RTM_FLOAT64_HI_ONE)
    {
        /* return +- Inf (x is +-1) */
        t = x / RTM_CONSTANT_ZERO;
    }
        /* if x < 2^-28 */
    else if((ix < RTM_FLOAT64_2_EXP_M28) &&
            ((RTM_CONSTANT_HUGE + x) > RTM_CONSTANT_ZERO))
    {
        /* atanh(x) = x for very small x */
        t = x;
    }
    else
    {
        /* x <- |x| */
        rtmSetHighf64(&x , ix);

        /* if x < 0.5 */
        if(ix < RTM_FLOAT64_HI_1_DIV_2)
        {
            /* auxiliary variable to store 2x */
            t = x + x;

            /* calculate atanh(x) = 0.5*log1p(2x+2x*x/(1-x)) */
            t = RTM_CONSTANT_HALF * rtmLog1f64(t + t * x / (RTM_CONSTANT_ONE - x));
        }
            /* else 1 > x >= 0.5 */
        else
        {
            /* return (except for the sign) atanh given by the formula 
             *                           2x
             * atanh(x) = 0.5 * log1p(--------)
             *                         1 - x
             */
            t = RTM_CONSTANT_HALF * rtmLog1f64((x + x) / (RTM_CONSTANT_ONE - x));
        }

        /* correct based on sign atanh(-x) = -atanh(x) */

        /* if x < 0 */
        if(hx < 0)
        {
            /* then correct the sign */
            t = -t;
        }
    }

    /* return the calculated value */
    return t;
}

/*! @} */
