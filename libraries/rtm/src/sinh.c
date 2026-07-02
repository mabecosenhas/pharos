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


PHAROS_WEAK float64_t rtmSinhf64(float64_t x)
{
    /*  Method : 
     * mathematically sinh(x) if defined to be (exp(x)-exp(-x))/2
     *	1. Replace x by |x| (sinh(-x) = -sinh(x)). 
     *	2. 
     *		                                    E + E/(E+1)
     *	    0        <= x <= 22     :  sinh(x) := --------------, E=expm1(x)
     *			       			        2
     *
     *	    22       <= x <= lnovft :  sinh(x) := exp(x)/2 
     *	    lnovft   <= x <= ln2ovft:  sinh(x) := exp(x/2)/2 * exp(x/2)
     *	    ln2ovft  <  x	    :  sinh(x) := x*shuge (overflow)
     *
     * Special cases:
     *	sinh(x) is |x| if x is +INF, -INF, or NaN.
     *	only sinh(0)=0 is exact for finite x.
     */

    float64_t t , w , h;

    /* High word of |x| */
    int32_t ix;

    /* High word of x */
    int32_t jx;


    /* High word of x */
    jx = rtmGetHighf64(x);

    /* High word of |x| */
    ix = jx & RTM_FLOAT64_HI_UNMASK_SIGN;

    /* x is INF or NaN */
    if(ix >= RTM_FLOAT64_HI_MASK_EXPONENT)
    {
        return x + x;
    }

    h = RTM_CONSTANT_HALF;

    if(jx < 0)
    {
        h = -h;
    }

    /* if |x| in [0,22], return sign(x)*0.5*(E+E/(E+1))) */

    /* if |x| < 22 */
    if(ix < 0x40360000)
    {
        /* if |x| < 2^-28 */
        if(ix < RTM_FLOAT64_2_EXP_M28)
        {
            /* sinh(tiny) = tiny with inexact */
            return x;
        }

        t = rtmExpm1f64(rtmAbsf64(x));

        /* if |x| < 1 */
        if(ix < RTM_FLOAT64_HI_ONE)
        {
            return h * (2.0 * t - t * t / (t + RTM_CONSTANT_ONE));
        }

        return h * (t + t / (t + RTM_CONSTANT_ONE));
    }

    /* if |x| in [22, log(max double)] */
    if(ix < 0x40862E42)
    {
        /* return 0.5 * exp(|x|) */
        return h * rtmExpf64(rtmAbsf64(x));
    }

    /* |x| in [log(max double), overflow threshold] */
    if(ix < 0x408633CE)
    {
        w = rtmExpf64(RTM_CONSTANT_HALF * rtmAbsf64(x));
        t = h * w;
        return t * w;
    }

    /* |x| > overflow threshold, sinh(x) overflow */
    return x * RTM_CONSTANT_HUGE;
}

/*! @} */
