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


PHAROS_WEAK float64_t rtmScalbnf64(float64_t x , int32_t n)
{
    /* exponent part of x */
    int32_t k;

    /* high word of x */
    int32_t hx;

    /* low word of x */
    int32_t lx;


    /* high word of x */
    hx = rtmGetHighf64(x);

    /* low  word of x */
    lx = rtmGetLowf64(x);

    /* extract exponent */
    k = (hx & RTM_FLOAT64_HI_MASK_EXPONENT) >> RTM_FLOAT64_HI_EXP_OFFSET;

    /* if x is NaN or Inf */
    if(k == 0x7ff)
    {
        /* NaN or Inf */
        return x + x;
    }

    /* if there is no exponent */
    if(k == 0)
    {
        /* if x = +-0 */
        if((lx | (hx & RTM_FLOAT64_HI_UNMASK_SIGN)) == 0)
        {
            /* +-0 */
            return x;
        }
        else
        {
            /* x = x * 2^54 */
            x *= RTM_CONSTANT_POWER_2_54;

            /* get the updated high part of x */
            hx = rtmGetHighf64(x);

            /* calculate again the exponent of x minus 54 (which we just added) */
            k = ((hx & RTM_FLOAT64_HI_MASK_EXPONENT) >> RTM_FLOAT64_HI_EXP_OFFSET) - 54;

            if(n < -50000)
            {
                /* raise underflow exception */
                return RTM_CONSTANT_TINY * x;
            }
        }
    }

    k = k + n;

    /* exponent of x and n are too big */
    if(k > 0x7fe)
    {
        /* overflow  */
        return RTM_CONSTANT_HUGE * rtmCopySignf64(RTM_CONSTANT_HUGE , x);
    }
        /* if cumulative exponent are bigger than 0 (but not too big) */
    else if(k > 0)
    {
        /* normal result */

        /* replace the exponent by the cumulative exponent */
        rtmSetHighf64(&x , (hx & RTM_FLOAT64_HI_UNMASK_EXPONENT) | (k << RTM_FLOAT64_HI_EXP_OFFSET));

        return x;
    }

    else if(k <= -54)
    {
        /* in case integer overflow in n + k */
        if(n > 50000)
        {
            /*overflow*/
            return RTM_CONSTANT_HUGE * rtmCopySignf64(RTM_CONSTANT_HUGE , x);
        }
        else
        {
            /* underflow */
            return RTM_CONSTANT_TINY * rtmCopySignf64(RTM_CONSTANT_TINY , x);
        }
    }
    else
    {
        /* subnormal result */
        k += 54;

        /* get the sign and the value part of hx */
        rtmSetHighf64(&x , (hx & RTM_FLOAT64_HI_UNMASK_EXPONENT) | (k << 20));

        return x * RTM_CONSTANT_POWER_2_54_LOW;
    }
}

/*! @} */
