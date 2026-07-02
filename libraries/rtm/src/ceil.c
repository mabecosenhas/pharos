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


PHAROS_WEAK float64_t rtmCeilf64(float64_t x)
{
    /* Method: Bit twiddling */

    /* high part of x */
    int32_t hx;

    /* low part of x */
    int32_t lx;

    /* exponent of x minus 0x3ff */
    int32_t j0;

    /* aux variable */
    uint32_t i;

    /* aux variable */
    uint32_t j;


    /* get the high part of x */
    hx = rtmGetHighf64(x);

    /* get the low part of x */
    lx = rtmGetLowf64(x);

    /* get the exponent minus 0x3ff */
    j0 = ((hx >> 20) & 0x7ff) - 0x3ff;

    if(j0 < 20)
    {
        /* if j0 is less than 0 than means that the exponent is larger than 1 */
        if(j0 < 0)
        {
            /* return 0*sign(x) if |x| < 1 */

            /* if x < 0 */
            if(hx < 0)
            {
                /* place the minus signal */
                hx = RTM_FLOAT64_HI_MASK_SIGN;

                /* reset the low part (return -0) */
                lx = 0;
            }
                /* if x is not 0 */
            else if((hx | lx) != 0)
            {
                /* return 1 */

                /* place the high part with one */
                hx = RTM_FLOAT64_HI_ONE;

                /* place the low part has "1" too */
                lx = 0;
            }
        }
        else
        {
            i = (RTM_FLOAT64_HI_MASK_FRAC) >> j0;

            /* if x is integral */
            if(((hx & i) | lx) == 0)
            {
                /* return it (and don't raise inexact exception) */
                return x;
            }

            /* if x > 0 */
            if(hx > 0)
            {
                /* add one unit */
                hx += (0x00100000) >> j0;
            }

            /* clear fractional part in hx */
            hx &= (~i);

            /* clear fractional part in lx */
            lx = 0;
        }
    }
    else if(j0 > 51)
    {
        if(j0 == 0x400)
        {
            /* inf or NaN */
            return x + x;
        }
        else
        {
            /* x is integral */
            return x;
        }
    }
        /* else means 20 < j0 < 51 */
    else
    {
        i = ((uint32_t) (0xffffffff)) >> (j0 - 20);

        /* if x is integral */
        if((lx & i) == 0)
        {
            /* return itself */
            return x;
        }

        /* if x > 0 */
        if(hx > 0)
        {
            if(j0 == 20)
            {
                /* add one unit */
                hx += 1;
            }
            else
            {
                j = lx + (1 << (52 - j0));

                /* got a carry */
                if(j < (uint32_t) lx)
                {
                    /* add one unit */
                    hx += 1;
                }

                lx = j;
            }
        }

        /* clear fractional part in lx */
        lx &= (~i);
    }

    /* set the high (32 bits) part */
    rtmSetHighf64(&x , hx);

    /* set the low (32 bits) part */
    rtmSetLowf64(&x , lx);

    /* return the calculated value */
    return x;
}

/*! @} */
