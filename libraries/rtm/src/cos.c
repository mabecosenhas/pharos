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


PHAROS_WEAK float64_t rtmCosf64(float64_t x)
{
    /* uses kernel functions:
     *	__kernel_sin		... sine function on [-pi/4,pi/4]
     *	__kernel_cos		... cosine function on [-pi/4,pi/4]
     *	__ieee754_rem_pio2	... argument reduction routine
     *
     * Method.
     *      Let S,C and T denote the sin, cos and tan respectively on 
     *	[-PI/4, +PI/4]. Reduce the argument x to y1+y2 = x-k*pi/2 
     *	in [-pi/4 , +pi/4], and let n = k mod 4.
     *	We have
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
     * Accuracy:
     *	TRIG(x) returns trig(x) nearly rounded 
     */

    /* remainder of x in pi/4 */
    float64_t y[2];

    /* remainder of x in pi/4 (the quadrant) */
    int32_t n;

    /* high word of |x| */
    int32_t ix;


    /* High word of x */
    ix = rtmGetHighf64(x);

    /* get the high word of |x| */
    ix &= RTM_FLOAT64_HI_UNMASK_SIGN;

    /* if |x| ~< pi/4 */
    if(ix <= 0x3fe921fb)
    {
        /* calculate using the kernel function */
        x = rtmKernelCosf64(x , 0.0);
    }
        /* cos(Inf or NaN) is NaN */
    else if(ix >= RTM_FLOAT64_HI_MASK_EXPONENT)
    {
        /* return error and raise exceptions */
        x = x - x;
    }
        /* argument reduction needed */
    else
    {
        /* reduce x by calculating x rem pi/2 */
        n = rtmRemPio2f64(x , y) & 3;

        /* first quadrant */
        if(n == 0)
        {
            /* calculate using cos */
            x = rtmKernelCosf64(y[0] , y[1]);
        }
            /* second quadrant */
        else if(n == 1)
        {
            /* calculate using -sin */
            x = -rtmKernelSinf64(y[0] , y[1] , 1);
        }
            /* third quadrant */
        else if(n == 2)
        {
            /* calculate using -cos */
            x = -rtmKernelCosf64(y[0] , y[1]);
        }
            /* fourth quadrant */
        else
        {
            /* calculate using sin */
            x = rtmKernelSinf64(y[0] , y[1] , 1);
        }
    }

    /* return the calculated value */
    return x;
}


float64_t rtmKernelCosf64(float64_t x , float64_t y)
{
    /* Algorithm
     *  1. Since cos(-x) = cos(x), we need only to consider positive x.
     *  2. if x < 2^-27 (hx<0x3e400000 0), return 1 with inexact if x!=0.
     *  3. cos(x) is approximated by a polynomial of degree 14 on
     *	   [0,pi/4]
     *		  	      2        4            14
     *	   	cos(x) ~ 1 - x/2 + C1*x + ... + C6*x
     *	   where the remez error is
     *	
     * 	|              2     4     6     8     10    12     14 |     -58
     * 	|cos(x)-(1-.5*x +C1*x +C2*x +C3*x +C4*x +C5*x  +C6*x  )| <= 2
     * 	|    					               | 
     * 
     * 	               4     6     8     10    12     14 
     *	4. let r = C1*x +C2*x +C3*x +C4*x +C5*x  +C6*x  , then
     *         cos(x) = 1 - x*x/2 + r
     *     since cos(x+y) ~ cos(x) - sin(x)*y 
     *                    ~ cos(x) - x*y,
     *     a correction term is necessary in cos(x) and hence
     *	       cos(x+y) = 1 - (x*x/2 - (r - x*y))
     *	   For better accuracy when x > 0.3, let qx = |x|/4 with
     *	   the last 32 bits mask off, and if x > 0.78125, let qx = 0.28125.
     *	   Then
     *	       cos(x+y) = (1-qx) - ((x*x/2-qx) - (r-x*y)).
     *     Note that 1-qx and (x*x/2-qx) is EXACT here, and the
     *     magnitude of the latter is at least a quarter of x*x/2,
     *     thus, reducing the rounding error in the subtraction.
     */

    /* 0x3FA55555, 0x5555554C */
    static const float64_t C1 = 4.16666666666666019037e-02;

    /* 0xBF56C16C, 0x16C15177 */
    static const float64_t C2 = -1.38888888888741095749e-03;

    /* 0x3EFA01A0, 0x19CB1590 */
    static const float64_t C3 = 2.48015872894767294178e-05;

    /* 0xBE927E4F, 0x809C52AD */
    static const float64_t C4 = -2.75573143513906633035e-07;

    /* 0x3E21EE9E, 0xBDB4B1C4 */
    static const float64_t C5 = 2.08757232129817482790e-09;

    /* 0xBDA8FAE9, 0xBE8838D4 */
    static const float64_t C6 = -1.13596475577881948265e-11;

    /* used to store x^2 */
    float64_t z;

    /* r has given in the formula above */
    float64_t r;

    /* qx as described above */
    float64_t qx;

    /* aux variable to store 1 - qx */
    float64_t a;

    /* aux variable to store 0.5 * z - qx */
    float64_t hz;

    /* high word of |x| */
    int32_t ix;


    /* ix = |x|'s high word */
    ix = rtmGetHighf64(x) & RTM_FLOAT64_HI_UNMASK_SIGN;

    /* if |x| < 2^27 */
    if(ix < RTM_FLOAT64_2_EXP_M27)
    {
        /* cos(0) = 1 */
        x = RTM_CONSTANT_ONE;
    }
    else
    {
        /* calculate x^2 (the polynomial that calculates r has only powers of 2) */
        z = x * x;

        /* calculate r (see above the formula) */
        r = z * (C1 + z * (C2 + z * (C3 + z * (C4 + z * (C5 + z * C6)))));

        /* if |x| < 0.3 */
        if(ix < 0x3FD33333)
        {
            /* calculate the cos */
            x = RTM_CONSTANT_ONE - (RTM_CONSTANT_HALF * z - (z * r - x * y));
        }
            /* else, |x| >= 0.3 */
        else
        {
            /* x > 0.78125 */
            if(ix > 0x3fe90000)
            {
                qx = 0.28125;
            }
            else
            {
                /* x/4 */
                rtmSetHighLowf64(&qx , ix - 0x00200000 , 0);
            }

            hz = RTM_CONSTANT_HALF * z - qx;
            a = RTM_CONSTANT_ONE - qx;

            /* calculate the cos */
            x = a - (hz - (z * r - x * y));
        }
    }

    /* return the calculated value */
    return x;
}

/*! @} */
