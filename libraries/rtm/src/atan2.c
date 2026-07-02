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
 *
 */

/*! \addtogroup PHAROS-RTM
 * 
 * @{
 */


#include <pharos/rtm/rtm.h>


PHAROS_WEAK float64_t rtmAtan2f64(float64_t y , float64_t x)
{
    /* Method :
     *	1. Reduce y to positive by atan2(y,x)=-atan2(-y,x).
     *	2. Reduce x to positive by (if x and y are unexceptional): 
     *	        ARG (x+iy) = arctan(y/x)   	   ... if x > 0,
     *	        ARG (x+iy) = pi - arctan[y/(-x)]   ... if x < 0,
     *
     * Special cases:
     *
     *  ATAN2((anything), NaN ) is NaN;
     *  ATAN2(NAN , (anything) ) is NaN;
     *  ATAN2(+-0, +(anything but NaN)) is +-0  ;
     *  ATAN2(+-0, -(anything but NaN)) is +-pi ;
     *  ATAN2(+-(anything but 0 and NaN), 0) is +-pi/2;
     *  ATAN2(+-(anything but INF and NaN), +INF) is +-0 ;
     *  ATAN2(+-(anything but INF and NaN), -INF) is +-pi;
     *  ATAN2(+-INF,+INF ) is +-pi/4 ;
     *  ATAN2(+-INF,-INF ) is +-3pi/4;
     *  ATAN2(+-INF, (anything but,0,NaN, and INF)) is +-pi/2;
     *
     * Constants:
     * The hexadecimal values are the intended ones for the following 
     * constants. The decimal values may be used, provided that the 
     * compiler will convert from decimal to binary accurately enough 
     * to produce the hexadecimal values shown.
     */

    float64_t z;
    int32_t k , m;

    /* high part of x */
    int32_t hx;

    /* high part of y */
    int32_t hy;

    /* high part of |x| */
    int32_t ix;

    /* high part of |y| */
    int32_t iy;

    /* lower part of x */
    uint32_t lx;

    /* lower part of y */
    uint32_t ly;

    /* return value */
    float64_t result;


    /* get the 32 high bits of x */
    hx = rtmGetHighf64(x);

    /* get the 32 high bits of |x| */
    ix = hx & RTM_FLOAT64_HI_UNMASK_SIGN;

    /* get the 32 lower bits of x */
    lx = rtmGetLowf64(x);

    /* get the 32 high bits of y */
    hy = rtmGetHighf64(y);

    /* get the 32 high bits of |y| */
    iy = hy & RTM_FLOAT64_HI_UNMASK_SIGN;

    /* get the 32 lower bits of x */
    ly = rtmGetLowf64(y);

    /* x or y is NaN */
    if(((ix | ((lx | -lx) >> 31)) > RTM_FLOAT64_HI_MASK_EXPONENT) ||
       ((iy | ((ly | -ly) >> 31)) > RTM_FLOAT64_HI_MASK_EXPONENT))
    {
        result = x + y;
    }
        /* check if x = 1.0 */
    else if(((hx - RTM_FLOAT64_HI_ONE) | lx) == 0)
    {
        /* calculate the atan of y */
        result = rtmAtanf64(y);
    }
    else
    {
        /*
        int temp = (hx >> 30);
        int temp2 = (hy >> 31);

        temp = temp2;
        temp2 = temp;
         */

        /* place the sign of both x and y in m -> 2*sign(x) + sign(y) */
        m = (((uint32_t) hx >> 30) & 2) | ((uint32_t) hy >> 31);

        /* if y = 0 */
        if((iy | ly) == 0)
        {
            /* m = 0 -> x is positive, y is positive */
            /* m = 1 -> x is positive, y is negative */
            if(m <= 1)
            {
                /* atan(+-0,+anything)=+-0 */
                result = y;

            }
                /* m = 2 -> x is negative, y is positive */
            else if(m == 2)
            {
                /* atan(+0,-anything) = pi */
                result = RTM_CONSTANT_PI + RTM_CONSTANT_TINY;
            }
                /* m = 3 -> x is negative, y is negative */
            else
            {
                /* atan(-0,-anything) =-pi */
                result = -RTM_CONSTANT_PI - RTM_CONSTANT_TINY;
            }
        }
            /* when x = 0 */
        else if((ix | lx) == 0)
        {
            /* check the sign to return -Pi/2 or Pi/2 */
            result = (hy < 0) ? -RTM_CONSTANT_PI_DIV_2 - RTM_CONSTANT_TINY : RTM_CONSTANT_PI_DIV_2 + RTM_CONSTANT_TINY;
        }
            /* if x is +-Inf */
        else if(ix == RTM_FLOAT64_HI_MASK_EXPONENT)
        {
            /* if y is +-Inf */
            if(iy == RTM_FLOAT64_HI_MASK_EXPONENT)
            {
                /* m = 0 -> x is positive, y is positive */
                if(m == 0)
                {
                    /* atan(+INF,+INF) */
                    result = RTM_CONSTANT_PI_DIV_4 + RTM_CONSTANT_TINY;
                }
                    /* m = 1 -> x is positive, y is negative */
                else if(m == 1)
                {
                    /* atan(-INF,+INF) */
                    result = -RTM_CONSTANT_PI_DIV_4 - RTM_CONSTANT_TINY;
                }
                    /* m = 2 -> x is negative, y is positive */
                else if(m == 2)
                {
                    /* atan(+INF,-INF) */
                    result = 3.0 * RTM_CONSTANT_PI_DIV_4 + RTM_CONSTANT_TINY;
                }
                    /* m = 3 -> x is negative, y is negative */
                else
                {
                    /* atan(-INF,-INF) */
                    result = -3.0 * RTM_CONSTANT_PI_DIV_4 - RTM_CONSTANT_TINY;
                }
            }
            else
            {
                /* m = 0 -> x is positive, y is positive */
                if(m == 0)
                {
                    /* atan(+...,+INF) */
                    result = RTM_CONSTANT_ZERO;
                }
                    /* m = 1 -> x is positive, y is negative */
                else if(m == 1)
                {
                    /* atan(-...,+INF) */
                    result = -RTM_CONSTANT_ZERO;
                }
                    /* m = 2 -> x is negative, y is positive */
                else if(m == 2)
                {
                    /* atan(+...,-INF) */
                    result = RTM_CONSTANT_PI + RTM_CONSTANT_TINY;
                }
                    /* m = 3 -> x is negative, y is negative */
                else
                {
                    /* atan(-...,-INF) */
                    result = -RTM_CONSTANT_PI - RTM_CONSTANT_TINY;
                }
            }
        }
            /* when y is INF (we already checked if y is NaN) */
        else if(iy == RTM_FLOAT64_HI_MASK_EXPONENT)
        {
            /* return -PI/2 or PI/2 depending on the sign of y */
            result = (hy < 0) ? -RTM_CONSTANT_PI_DIV_2 - RTM_CONSTANT_TINY : RTM_CONSTANT_PI_DIV_2 + RTM_CONSTANT_TINY;
        }
        else
        {
            /* compute y/x */
            k = (iy - ix) >> 20;

            if(k > 60)
            {
                /* |y/x| >  2^60 */
                z = RTM_CONSTANT_PI_DIV_2 + RTM_CONSTANT_HALF * RTM_CONSTANT_PI_LOW;
            }
            else if(hx < 0 && k<-60)
            {
                /* |y|/x < -2^60 */
                z = 0.0;
            }
            else
            {
                /* safe to do y/x */
                z = rtmAtanf64(rtmAbsf64(y / x));
            }

            /* m = 0 -> x is positive, y is positive */
            if(m == 0)
            {
                /* atan(+,+) */
                result = z;
            }
                /* m = 1 -> x is positive, y is negative */
            else if(m == 1)
            {
                /* atan(-,+) */
                /* flip the sign */
                rtmSetHighf64(&z , rtmGetHighf64(z) ^ RTM_FLOAT64_HI_MASK_SIGN);

                result = z;
            }
                /* m = 2 -> x is negative, y is positive */
            else if(m == 2)
            {
                /* atan(+,-) */
                result = RTM_CONSTANT_PI - (z - RTM_CONSTANT_PI_LOW);

            }
                /* m = 3 -> x is negative, y is negative */
            else
            {
                /* atan(-,-) */
                result = (z - RTM_CONSTANT_PI_LOW) - RTM_CONSTANT_PI;
            }
        }
    }

    /* return the calculated value */
    return result;
}

/*! @} */
