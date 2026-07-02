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


PHAROS_WEAK float64_t rtmModf64(float64_t x , float64_t *iptr)
{
    /* Method: Bit twiddling */

    /* high x */
    int32_t hx;

    /* low  x */
    int32_t lx;

    /* exponent of x minux 0x3ff */
    int32_t j0;

    /* aux variable */
    uint32_t i;


    /* high x */
    hx = rtmGetHighf64(x);

    /* low  x */
    lx = rtmGetLowf64(x);

    /* exponent of x minux 0x3ff */
    j0 = ((hx >> 20) & 0x7ff) - 0x3ff;

    if(j0 < 20)
    {
        /* integer part in high x */

        /* if |x| < 1 */
        if(j0 < 0)
        {
            /* *iptr = +-0 */

            /* keep the sign and clear everything else */
            rtmSetHighLowf64(iptr , hx & RTM_FLOAT64_HI_MASK_SIGN , 0);
        }
        else
        {
            i = (RTM_FLOAT64_HI_MASK_FRAC) >> j0;

            /* if x is integral */
            if(((hx & i) | lx) == 0)
            {
                /* integral part is the x itself */
                *iptr = x;

                /* keep the sign and clear everything else */
                rtmSetHighLowf64(&x , rtmGetHighf64(x) & RTM_FLOAT64_HI_MASK_SIGN , 0);
            }
            else
            {
                rtmSetHighLowf64(iptr , hx & (~i) , 0);

                /* place the return value */
                x = x - *iptr;
            }
        }
    }
    else if(j0 > 51)
    {
        /* if x is NaN or +-Inf */
        if(j0 == 0x400)
        {
            *iptr = x;

            /* return NaN */
            x = 0.0 / x;
        }
        else
        {
            /* no fraction part */
            *iptr = x * RTM_CONSTANT_ONE;

            /* keep the sign and clear everything else */
            rtmSetHighf64(&x , rtmGetHighf64(x) & RTM_FLOAT64_HI_MASK_SIGN);

            /* place 0 */
            rtmSetLowf64(&x , 0);

            /* return +-0 */
        }
    }
    else
    {
        /* fraction part in low x */
        i = ((uint32_t) (0xffffffff)) >> (j0 - 20);

        /* if x is integral */
        if((lx & i) == 0)
        {
            /* copy x */
            *iptr = x;

            /* keep only the sign of x and reset everything else (place +-0) */
            rtmSetHighLowf64(&x , rtmGetHighf64(x) & RTM_FLOAT64_HI_MASK_SIGN , 0);

            /* return +-0 */
        }
        else
        {
            /* set the high and low part of iptr */
            rtmSetHighLowf64(iptr , hx , lx & (~i));

            /* place the return value */
            x = x - *iptr;
        }
    }

    /* return the calculated value */
    return x;
}

/*! @} */
