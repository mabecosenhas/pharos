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


PHAROS_WEAK float64_t rtmCoshf64(float64_t x)
{
    /* Method : 
     * mathematically cosh(x) if defined to be (exp(x)+exp(-x))/2
     *	1. Replace x by |x| (cosh(x) = cosh(-x)). 
     *	2. 
     *		                                        [ exp(x) - 1 ]^2 
     *	    0        <= x <= ln2/2  :  cosh(x) := 1 + -------------------
     *			       			           2*exp(x)
     *
     *		                                  exp(x) +  1/exp(x)
     *	    ln2/2    <= x <= 22     :  cosh(x) := -------------------
     *			       			          2
     *	    22       <= x <= lnovft :  cosh(x) := exp(x)/2 
     *	    lnovft   <= x <= ln2ovft:  cosh(x) := exp(x/2)/2 * exp(x/2)
     *	    ln2ovft  <  x	    :  cosh(x) := huge*huge (overflow)
     *
     * Special cases:
     *	cosh(x) is |x| if x is +INF, -INF, or NaN.
     *	only cosh(0)=1 is exact for finite x.
     */

    /* aux variable */
    float64_t t;

    /* aux variable */
    float64_t w;

    /* high word of |x| */
    int32_t ix;


    /* High word of |x|. */
    ix = rtmHighAbsf64(x);

    /* x is +-Inf or NaN */
    if(ix >= RTM_FLOAT64_HI_MASK_EXPONENT)
    {
        /* return +Inf or NaN */
        x = x * x;
    }
        /* if |x| in [0,0.5*ln(2)],  */
    else if(ix < 0x3fd62e43)
    {
        /* calculate exp(|x|) - 1 */
        t = rtmExpm1f64(rtmAbsf64(x));

        /* calculate exp(|x|) */
        w = RTM_CONSTANT_ONE + t;

        /* if x is tiny (2^-55) */
        if(ix < RTM_FLOAT_2_EXP_M55)
        {
            /* cosh(tiny) = 1 */
            x = w;
        }
        else
        {
            /* return 1+expm1(|x|)^2 / (2*exp(|x|)) */
            x = RTM_CONSTANT_ONE + (t * t) / (w + w);
        }
    }
        /* |x| in [0.5*ln2,22],  */
    else if(ix < 0x40360000)
    {
        /* calculate exp(|x|) */
        t = rtmExpf64(rtmAbsf64(x));

        /* return (exp(|x|)+1/exp(|x|)/2; */
        x = RTM_CONSTANT_HALF * t + RTM_CONSTANT_HALF / t;
    }
        /* |x| in [22, log(max float64_t)] */
    else if(ix < 0x40862E42)
    {
        /* return half*exp(|x|) -> exp(-|x|) is too little to be worth to calculate */
        x = RTM_CONSTANT_HALF * rtmExpf64(rtmAbsf64(x));
    }
        /* else, |x| is in [log(max float64_t) */
    else
    {
        /* overflow threshold] */

        /* check if can still calculate without giving an overflow */
        if(ix < 0x408633CE)
        {
            w = rtmExpf64(RTM_CONSTANT_HALF * rtmAbsf64(x));
            t = RTM_CONSTANT_HALF * w;

            x = t * w;
        }
        else
        {
            /* |x| > overflow threshold, cosh(x) overflow */
            x = RTM_CONSTANT_HUGE * RTM_CONSTANT_HUGE;
        }
    }

    /* return the calculated value */
    return x;
}

/*! @} */
