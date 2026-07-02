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


PHAROS_WEAK int32_t rtmIlogbf64(float64_t x)
{
    /* high word of |x| */
    int32_t hx;

    /* low word of x */
    int32_t lx;

    /* function return value */
    int32_t result;


    /* high word of |x| */
    hx = (rtmGetHighf64(x)) & RTM_FLOAT64_HI_UNMASK_SIGN;

    if(hx < 0x00100000)
    {
        lx = rtmGetLowf64(x);

        /* if x is 0 */
        if((hx | lx) == 0)
        {
            /* ilogb(0) = 0x80000001 (the lowest integer number = -2147483647) */
            result = -__INT32_MAX__;
        }
            /* subnormal x */
        else
        {
            result = rtmInternalIlogbSmallf64(hx , lx);
        }

    }
    else if(hx < RTM_FLOAT64_HI_MASK_EXPONENT)
    {
        result = (hx >> 20) - 1023;
    }
        /* else, x is too big, x is NaN or x is +-Inf */
    else
    {
        /* return the highest possible number, even if x is NaN */
        result = __INT32_MAX__;
    }

    /* return the calculated value */
    return result;
}

/*! @} */
