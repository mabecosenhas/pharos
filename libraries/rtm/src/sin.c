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


PHAROS_WEAK float64_t rtmSinf64(float64_t x)
{
    /* Let S,C and T denote the sin, cos and tan respectively on [-PI/4, +PI/4].
     * Reduce the argument x to y1+y2 = x-k*pi/2 
     * in [-pi/4 , +pi/4], and let n = k mod 4.
     * We have
     *
     *          n        sin(x)      cos(x)        tan(x)
     *     ----------------------------------------------------------
     *	    0	       S	   C		 T
     *	    1	       C	  -S		-1/T
     *	    2	      -S	  -C		 T
     *	    3	      -C	   S		-1/T
     *     ----------------------------------------------------------
     *
     * Special cases:
     *      Let trig be any of sin, cos, or tan.
     *      trig(+-INF)  is NaN, with signals;
     *      trig(NaN)    is that NaN;
     *
     * Accuracy: TRIG(x) returns trig(x) nearly rounded 
     */
    float64_t y[2] , z = 0.0;
    int32_t n;

    /* High word of |x| */
    int32_t ix;



    /* High word of |x| */
    ix = rtmGetHighf64(x) & RTM_FLOAT64_HI_UNMASK_SIGN;

    /* if |x| ~< pi/4 */
    if(ix <= 0x3fe921fb)
    {
        /* calculate using sin */
        x = rtmKernelSinf64(x , z , 0);
    }
        /* sin(Inf or NaN) is NaN */
    else if(ix >= RTM_FLOAT64_HI_MASK_EXPONENT)
    {
        x = x - x;
    }
        /* argument reduction needed */
    else
    {
        /* find out in which quadrant x is */
        n = rtmRemPio2f64(x , y) & 3;

        /* according to the quadrant */

        /* first quadrant */
        if(n == 0)
        {
            /* calculate using sin */
            x = rtmKernelSinf64(y[0] , y[1] , 1);
        }
            /* second quadrant */
        else if(n == 1)
        {
            /* calculate using cos */
            x = rtmKernelCosf64(y[0] , y[1]);
        }
            /* third quadrant */
        else if(n == 2)
        {
            /* calculate using -sin */
            x = -rtmKernelSinf64(y[0] , y[1] , 1);
        }
            /* fourth quadrant */
        else
        {
            /* calculate using -cos */
            x = -rtmKernelCosf64(y[0] , y[1]);
        }
    }

    /* return the calculated value */
    return x;
}


float64_t rtmKernelSinf64(float64_t x , float64_t y , int32_t iy)
{
    /* Algorithm
     *	1. Since sin(-x) = -sin(x), we need only to consider positive x. 
     *	2. if x < 2^-27 (hx<0x3e400000 0), return x with inexact if x!=0.
     *	3. sin(x) is approximated by a polynomial of degree 13 on
     *	   [0,pi/4]
     *		  	         3            13
     *	   	sin(x) ~ x + S1*x + ... + S6*x
     *	   where
     *	
     * 	|sin(x)         2     4     6     8     10     12  |     -58
     * 	|----- - (1+S1*x +S2*x +S3*x +S4*x +S5*x  +S6*x   )| <= 2
     * 	|  x 					           | 
     * 
     *	4. sin(x+y) = sin(x) + sin'(x')*y
     *		    ~ sin(x) + (1-x*x/2)*y
     *	   For better accuracy, let 
     *		     3      2      2      2      2
     *		r = x *(S2+x *(S3+x *(S4+x *(S5+x *S6))))
     *	   then                   3    2
     *		sin(x) = x + (S1*x + (x *(r-y/2)+y))
     */

    static const float64_t

    S1 = -1.66666666666666324348e-01 , /* 0xBFC55555, 0x55555549 */
            S2 = 8.33333333332248946124e-03 , /* 0x3F811111, 0x1110F8A6 */
            S3 = -1.98412698298579493134e-04 , /* 0xBF2A01A0, 0x19C161D5 */
            S4 = 2.75573137070700676789e-06 , /* 0x3EC71DE3, 0x57B1FE7D */
            S5 = -2.50507602534068634195e-08 , /* 0xBE5AE5E6, 0x8A2B9CEB */
            S6 = 1.58969099521155010221e-10; /* 0x3DE5D93A, 0x5ACFD57C */


    /* value of r (see formula above) */
    float64_t r;

    /* aux variable with x^2 */
    float64_t z;

    /* aux variable with x^3 */
    float64_t v;

    /* high word of |x| */
    int32_t ix;


    /* high word of |x| */
    ix = rtmGetHighf64(x) & RTM_FLOAT64_HI_UNMASK_SIGN;

    /* |x| < 2^-27 */
    if(ix < RTM_FLOAT64_2_EXP_M27)
    {
        /* sin(small) ~ small */
        return x;
    }

    /* calculate x^2 */
    z = x*x;

    /* calculate x^3 */
    v = z*x;

    /* calculate r (see formula above) */
    r = S2 + z * (S3 + z * (S4 + z * (S5 + z * S6)));

    /* if iy is 0 then y is also 0, simplifies the math */
    if(iy == 0)
    {
        return x + v * (S1 + z * r);
    }
        /* the low part of x is inside y, so use it */
    else
    {
        return x - ((z * (RTM_CONSTANT_HALF * y - v * r) - y) - v * S1);
    }
}

/*! @} */
