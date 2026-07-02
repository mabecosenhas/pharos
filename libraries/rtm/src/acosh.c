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


PHAROS_WEAK float64_t rtmAcoshf64(float64_t x)
{
    /* Method :
     * Based on 
     *    acosh(x) = log [ x + sqrt(x*x-1) ]
     *    
     * we have
     *    acosh(x) := log(x) + ln2,             if x is large; else
     *	  acosh(x) := log(2x-1/(sqrt(x*x-1)+x)) if x > 2; else
     *	  acosh(x) := log1p(t+sqrt(2.0*t+t*t)); where t = x-1.
     *
     * Special cases:
     *	acosh(x) is NaN with signal if x < 1.
     *	acosh(NaN) is NaN without signal.
     */

    /* auxiliary variable to store temporary values */
    float64_t t;

    /* high word of x */
    int32_t hx;


    /* high word of x */
    hx = rtmGetHighf64(x);

    /* if x < 1 */
    if(hx < RTM_FLOAT64_HI_ONE)
    {
        /* return NaN */
        return (x - x) / (x - x);
    }
        /* if x > 2^28 */
    else if(hx >= 0x41b00000)
    {
        /* if x is Inf of NaN */
        if(hx >= RTM_FLOAT64_HI_MASK_EXPONENT)
        {
            /* if Inf, return Inf. If NaN, return NaN */
            return x + x;
        }
            /* if x > 2^28 (and not inf or NaN */
        else
        {
            /* acosh(huge) = log(x) + ln2 */
            return rtmLogf64(x) + RTM_CONSTANT_LN2;
        }
    }
        /* if special case of x = 1 */
    else if(((hx - RTM_FLOAT64_HI_ONE) | rtmGetLowf64(x)) == 0)
    {
        /* acosh(1) = 0 */
        return 0.0;
    }
        /* if 2^28 > x > 2 */
    else if(hx > 0x40000000)
    {
        /* aux variable to store x^2 */
        t = x*x;

        /* return log(2x-1/(x+sqrt(x^2-1))) */
        return rtmLogf64(2.0 * x - RTM_CONSTANT_ONE / (x + rtmSqrtf64(t - RTM_CONSTANT_ONE)));
    }
        /* 1 <= x < 2 */
    else
    {
        /* aux variable to store x-1 */
        t = x - RTM_CONSTANT_ONE;

        /* return log1p(t+sqrt(2.0*t+t*t)) where t = x - 1 */
        return rtmLog1f64(t + rtmSqrtf64(2.0 * t + t * t));
    }
}

/*! @} */
