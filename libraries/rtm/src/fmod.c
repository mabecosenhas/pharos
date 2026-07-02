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


PHAROS_WEAK float64_t rtmFmodf64(float64_t x , float64_t y)
{
    /* Method: shift and subtract */

    static const float64_t Zero[] = {0.0 , -0.0};

    int32_t n;

    /* high word of |x| */
    int32_t hx;

    /* high word of |y| */
    int32_t hy;

    /* difference of hx and hy */
    int32_t hz;

    /* ix = ilogb(x) */
    int32_t ix;

    /* iy = ilogb(y) */
    int32_t iy;

    /* sign of x */
    int32_t sx;

    /* low  word of x */
    uint32_t lx;

    /* low  word of y */
    uint32_t ly;

    /* difference in the low parts of x and y */
    uint32_t lz;


    /* high word of x */
    hx = rtmGetHighf64(x);

    /* low  word of x */
    lx = rtmGetLowf64(x);

    /* high word of |y| */
    hy = rtmGetHighf64(y) & RTM_FLOAT64_HI_UNMASK_SIGN;

    /* low  word of y */
    ly = rtmGetLowf64(y);

    /* sign of x */
    sx = hx & RTM_FLOAT64_HI_MASK_SIGN;

    /* |x| */
    hx ^= sx;

    /* if y=0,or x not finite or y is NaN */
    if((hy | ly) == 0 || (hx >= RTM_FLOAT64_HI_MASK_EXPONENT) ||
       ((hy | ((ly | -ly) >> 31)) > RTM_FLOAT64_HI_MASK_EXPONENT))
    {
        /* purge off exception values */
        return (x * y) / (x * y);
    }

    /* if the hi(x) is smaller than hi(y) */
    if(hx <= hy)
    {
        /* if |x| < |y| */
        if((hx < hy) || (lx < ly))
        {
            /* return x */
            return x;
        }

        /* if |x| = |y| */
        if(lx == ly)
        {
            /* return x * +-0 */
            return Zero[(uint32_t) sx >> 31];
        }
        /* else, then |x| > |y| and we have to calculate it */
    }

    /* if we are here then |x| > |y| */

    /* determine ix = ilogb(x) */
    ix = rtmInternalIlogbf64(hx , lx);

    /* determine iy = ilogb(y) */
    iy = rtmInternalIlogbf64(hy , ly);

    /* set up {hx,lx}, {hy,ly} and align y to x */
    if(ix >= -1022)
    {
        hx = 0x00100000 | (RTM_FLOAT64_HI_MASK_FRAC & hx);
    }
        /* subnormal x, shift x to normal */
    else
    {
        n = -1022 - ix;

        if(n <= 31)
        {
            hx = (hx << n) | (lx >> (32 - n));
            lx <<= n;
        }
        else
        {
            hx = lx << (n - 32);
            lx = 0;
        }
    }
    if(iy >= -1022)
    {
        hy = 0x00100000 | (RTM_FLOAT64_HI_MASK_FRAC & hy);
    }
        /* subnormal y, shift y to normal */
    else
    {
        n = -1022 - iy;

        if(n <= 31)
        {
            hy = (hy << n) | (ly >> (32 - n));
            ly <<= n;
        }
        else
        {
            hy = ly << (n - 32);
            ly = 0;
        }
    }

    /* fix point fmod */
    n = ix - iy;

    while(n--)
    {
        hz = hx - hy;
        lz = lx - ly;

        if(lx < ly)
        {
            hz -= 1;
        }

        /* if y > x */
        if(hz < 0)
        {
            hx = hx + hx + (lx >> 31);
            lx = lx + lx;
        }
        else
        {
            /* if x = y */
            if((hz | lz) == 0)
            {
                /* return sign(x)*0 */
                return Zero[(uint32_t) sx >> 31];
            }

            hx = hz + hz + (lz >> 31);
            lx = lz + lz;
        }
    }

    hz = hx - hy;
    lz = lx - ly;

    if(lx < ly)
    {
        hz -= 1;
    }

    if(hz >= 0)
    {
        hx = hz;
        lx = lz;
    }

    /* convert back to floating value and restore the sign */
    if((hx | lx) == 0)
    {
        /* return sign(x)*0 */
        return Zero[(uint32_t) sx >> 31];
    }

    /* normalize x */
    while(hx < 0x00100000)
    {
        hx = hx + hx + (lx >> 31);
        lx = lx + lx;
        iy -= 1;
    }

    if(iy >= -1022)
    {
        /* normalize output */
        hx = ((hx - 0x00100000) | ((iy + 1023) << 20));

        /* set the high part (and keep the sign) and the low part */
        rtmSetHighLowf64(&x , hx | sx , lx);
    }
    else
    {
        /* subnormal output */
        n = -1022 - iy;

        if(n <= 20)
        {
            lx = (lx >> n) | ((uint32_t) hx << (32 - n));
            hx >>= n;
        }
        else if(n <= 31)
        {
            lx = (hx << (32 - n)) | (lx >> n);
            hx = sx;
        }
        else
        {
            lx = hx >> (n - 32);
            hx = sx;
        }

        /* set the high part (and keep the sign) and the low part */
        rtmSetHighLowf64(&x , hx | sx , lx);

        /* create necessary signal */
        x *= RTM_CONSTANT_ONE;
    }

    /* exact output */
    return x;
}

/*! @} */
