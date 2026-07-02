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


PHAROS_WEAK float64_t rtmRemainderf64(float64_t x , float64_t p)
{
    /* Method : Based on fmod() return x-[x/p]chopped*p exactlp */

    /* high word of x */
    int32_t hx;

    /* high word of p */
    int32_t hp;

    /* sign of x */
    uint32_t sx;

    /* low word of x */
    uint32_t lx;

    /* low  word of p */
    uint32_t lp;

    /* aux variable that stores 0.5 * p */
    float64_t p_half;


    /* high word of x */
    hx = rtmGetHighf64(x);

    /* low  word of x */
    lx = rtmGetLowf64(x);

    /* high word of p */
    hp = rtmGetHighf64(p);

    /* low  word of p */
    lp = rtmGetLowf64(p);

    /* get the sign */
    sx = hx & RTM_FLOAT64_HI_MASK_SIGN;

    /* high word of |p| */
    hp &= RTM_FLOAT64_HI_UNMASK_SIGN;

    /* high word of |x| */
    hx &= RTM_FLOAT64_HI_UNMASK_SIGN;

    /* if p = 0 */
    if((hp | lp) == 0)
    {
        /* if x is NaN, return NaN, otherwise return NaN with division by 0 */
        x = (x * p) / (x * p);
    }
        /* if x not finite or p is NaN */
    else if((hx >= RTM_FLOAT64_HI_MASK_EXPONENT) ||
            ((hp >= RTM_FLOAT64_HI_MASK_EXPONENT)&&
             (((hp - RTM_FLOAT64_HI_MASK_EXPONENT) | lp) != 0)))
    {
        x = (x * p) / (x * p);
    }
    else
    {
        if(hp <= 0x7fdfffff)
        {
            /* now x < 2p */
            x = rtmFmodf64(x , p + p);
        }

        /* if x = p */
        if(((hx - hp) | (lx - lp)) == 0)
        {
            /* return +-0 (according to the sign of x) */
            x = RTM_CONSTANT_ZERO * x;
        }
        else
        {
            /* calculate |x| */
            x = rtmAbsf64(x);

            /* calculate |p| */
            p = rtmAbsf64(p);

            if(hp < 0x00200000)
            {
                if(x + x > p)
                {
                    x -= p;

                    if(x + x >= p)
                    {
                        x -= p;
                    }
                }
            }
            else
            {
                p_half = RTM_CONSTANT_HALF * p;

                if(x > p_half)
                {
                    x -= p;

                    if(x >= p_half)
                    {
                        x -= p;
                    }
                }
            }

            /* correct the signal of x */
            rtmSetHighf64(&x , rtmGetHighf64(x) ^ sx);
        }
    }

    /* return the calculated value */
    return x;
}

/*! @} */
