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


PHAROS_WEAK float64_t rtmLogbf64(float64_t x)
{
    /* return value of this function */
    float64_t result;

    /* high part of |x| */
    int32_t lx;

    /* high part of x */
    int32_t ix;


    /* high |x| */
    ix = (rtmGetHighf64(x)) & RTM_FLOAT64_HI_UNMASK_SIGN;

    /* low x */
    lx = rtmGetLowf64(x);

    /* if x = +-0 */
    if((ix | lx) == 0)
    {
        /* return -1 / |x| -> -Inf */
        result = -RTM_CONSTANT_ONE / rtmAbsf64(x);
    }
        /* if x is NaN or +-Inf */
    else if(ix >= RTM_FLOAT64_HI_MASK_EXPONENT)
    {
        /* return NaN or +Inf */
        result = x * x;
    }
        /* x is a "good value" */
    else
    {
        /* get the exponent bits of x */
        ix >>= 20;

        /* if x is too small -> exponent (e) is 0 then */
        if(ix == 0)
        {
            /* x = fraction * 2^-1023, but we already know that x != 0 (was checked earlier) so the exponent is -1022 */
            result = -1022.0;
        }
        else
        {
            result = (float64_t) (ix - 1023);
        }
    }

    /* return the calculated value */
    return result;
}

/*! @} */
