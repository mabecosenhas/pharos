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


PHAROS_WEAK float64_t rtmRintf64(float64_t x)
{
    /*  Method: Using floating addition */

    /* table containing 2^52 and -2^52*/
    static const float64_t TWO52[2] = {

        /* 0x43300000, 0x00000000 */
        RTM_CONSTANT_POWER_2_52 ,

        /* 0xC3300000, 0x00000000 */
        -RTM_CONSTANT_POWER_2_52 ,
    };

    /* high word of x */
    int32_t hx;

    int32_t j0;

    /* sign (1 or 0) of x */
    int32_t sx;

    uint32_t i , i1;
    float64_t w , t;


    /* high word of x */
    hx = rtmGetHighf64(x);

    /* get the sign (1 or 0 for - or +) */
    sx = (hx >> 31) & 1;

    /* low  word of x */
    i1 = rtmGetLowf64(x);

    /* get exponent of x - 0x3ff */
    j0 = ((hx >> 20) & 0x7ff) - 0x3ff;

    if(j0 < 20)
    {
        if(j0 < 0)
        {
            /* if x = 0 */
            if(((hx & RTM_FLOAT64_HI_UNMASK_SIGN) | i1) == 0)
            {
                /* rint(0) = 0 */
                return x;
            }
            else
            {
                i1 |= (hx & 0x0fffff);
                hx &= 0xfffe0000;
                hx |= ((i1 | -i1) >> 12)&0x80000;

                rtmSetHighf64(&x , hx);

                w = TWO52[sx] + x;
                t = w - TWO52[sx];
                hx = rtmGetHighf64(t);

                rtmSetHighf64(&t , (hx & RTM_FLOAT64_HI_UNMASK_SIGN) | (sx << 31));

                return t;
            }
        }
        else
        {
            i = (RTM_FLOAT64_HI_MASK_FRAC) >> j0;

            /* if x is integral */
            if(((hx & i) | i1) == 0)
            {
                /* return itself */
                return x;
            }

            i >>= 1;

            if(((hx & i) | i1) != 0)
            {
                if(j0 == 19)
                {
                    i1 = 0x40000000;
                }
                else
                {
                    hx = (hx & (~i)) | ((0x20000) >> j0);
                }
            }
        }
    }
        /* if exp > 51 + 0x3ff = 1074 */
    else if(j0 > 51)
    {
        /* if x is either NaN or Inf -> exponent is 0x7ff (0x3ff + 0x400 = 0x7ff) */
        if(j0 == 0x400)
        {
            /* rint(NaN) = NaN , rint(+-Inf) = +-Inf */
            return x + x;
        }
            /* means that exponent is > 1074 (but not NaN nor Inf) */
        else
        {
            /* x is integral (exponent is too big for fractional part to have a ",") */
            return x;
        }
    }
    else
    {
        i = ((uint32_t) (0xffffffffU)) >> (j0 - 20);

        /* if x is integral */
        if((i1 & i) == 0)
        {
            /* return itself */
            return x;
        }

        i >>= 1;

        if((i1 & i) != 0)
        {
            i1 = (i1 & (~i)) | ((0x40000000) >> (j0 - 20));
        }
    }

    rtmSetHighf64(&x , hx);
    rtmSetLowf64(&x , i1);

    w = TWO52[sx] + x;

    return w - TWO52[sx];
}

/*! @} */
