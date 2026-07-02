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


PHAROS_WEAK float64_t rtmNextafterf64(float64_t x , float64_t y)
{
    /* high word of x */
    int32_t hx;

    /* high word of y */
    int32_t hy;

    /* high word of |x| */
    int32_t ix;

    /* high word of |y| */
    int32_t iy;

    /* low  word of x */
    uint32_t lx;

    /* low  word of y */
    uint32_t ly;


    /* high word of x */
    hx = rtmGetHighf64(x);

    /* low  word of x */
    lx = rtmGetLowf64(x);

    /* high word of y */
    hy = rtmGetHighf64(y);

    /* low  word of y */
    ly = rtmGetLowf64(y);

    /* |x| */
    ix = hx & RTM_FLOAT64_HI_UNMASK_SIGN;

    /* |y| */
    iy = hy & RTM_FLOAT64_HI_UNMASK_SIGN;

    /* x is nan or y is nan */
    if(((ix >= RTM_FLOAT64_HI_MASK_EXPONENT) && ((ix - RTM_FLOAT64_HI_MASK_EXPONENT) | lx) != 0) ||
       ((iy >= RTM_FLOAT64_HI_MASK_EXPONENT) && ((iy - RTM_FLOAT64_HI_MASK_EXPONENT) | ly) != 0))
    {
        x = x + y;
    }
        /* if x = y */
    else if(x == y)
    {
        /* return y (this for the case of +-0 for x and y */
        x = y;
    }
        /*if x == 0 */
    else if((ix | lx) == 0)
    {
        /* copy the sign of y and place in x the minimum float64_t value above/below 0 (+-2^-1074) */
        rtmSetHighLowf64(&x , hy & RTM_FLOAT64_HI_MASK_SIGN , 1);
    }
        /* simple cases have been solved, now we have to calculate the hard way */
    else
    {
        /* if x > 0 */
        if(hx >= 0)
        {
            /* x > y, x -= ulp */
            if(hx > hy || ((hx == hy) && (lx > ly)))
            {
                if(lx == 0)
                {
                    hx -= 1;
                }

                lx -= 1;
            }
                /* x < y, x += ulp */
            else
            {
                lx += 1;

                if(lx == 0)
                {
                    hx += 1;
                }
            }
        }
            /* x < 0 */
        else
        {
            /* x < y, x -= ulp */
            if(hy >= 0 || hx > hy || ((hx == hy)&&(lx > ly)))
            {
                if(lx == 0)
                {
                    hx -= 1;
                }

                lx -= 1;
            }
                /* x > y, x += ulp */
            else
            {
                lx += 1;

                if(lx == 0)
                {
                    hx += 1;
                }
            }
        }

        /* copy the exponent of x to y */
        hy = hx & RTM_FLOAT64_HI_MASK_EXPONENT;

        /* if overflow  */
        if(hy >= RTM_FLOAT64_HI_MASK_EXPONENT)
        {
            /* the exponent is to great, this will trigger an overflow */
            x = x + x;
        }
        else
        {
            /* if underflow */
            if(hy < 0x00100000)
            {
                /* update x */
                rtmSetHighLowf64(&x , hx , lx);
            }
        }
    }

    /* return the calculated value */
    return x;
}

/*! @} */
