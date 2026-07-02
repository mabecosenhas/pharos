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


PHAROS_WEAK float64_t rtmFrexpf64(float64_t x , int32_t *eptr)
{
    /* and the corresponding binary exponent "exp". That is arg = x*2^exp.
     * If arg is inf, 0.0, or NaN, then frexp(arg,&exp) returns arg with *exp=0 */

    /* high part of x */
    int32_t hx;

    /* high part of |x| */
    int32_t ix;

    /* low part of x */
    int32_t lx;


    /* high part of x */
    hx = rtmGetHighf64(x);

    /* high part of |x| */
    ix = hx & RTM_FLOAT64_HI_UNMASK_SIGN;

    /* low part of x */
    lx = rtmGetLowf64(x);

    /* initialise eptr */
    *eptr = 0;

    /* if x is a "normal" number (not 0, Inf nor NaN) */
    if(ix < RTM_FLOAT64_HI_MASK_EXPONENT && ((ix | lx) != 0))
    {
        /* if x is subnormal */
        if(ix < 0x00100000)
        {
            /* update x (x = x * 2^54) */
            x *= RTM_CONSTANT_POWER_2_54;

            /* get again the new high part of x */
            hx = rtmGetHighf64(x);

            /* high part of |x| */
            ix = hx & RTM_FLOAT64_HI_UNMASK_SIGN;

            /* decrease the exponent */
            *eptr = -54;
        }

        /* update the exponent by low part of x */
        *eptr += (ix >> 20) - 1022;

        /* update the high part to keep the sign and the integer part */
        rtmSetHighf64(&x , (hx & RTM_FLOAT64_HI_UNMASK_EXPONENT) | RTM_FLOAT64_HI_1_DIV_2);
    }
    /* else, if x is 0 or Inf or NaN then return itself */

    /* x was already updated with the result (or left alone in case of x being 0, Inf or NaN */
    return x;
}

/*! @} */
