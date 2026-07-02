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


PHAROS_WEAK float64_t rtmTanf64(float64_t x)
{
    /* Method:
     *  Let S,C and T denote the sin, cos and tan respectively on 
     *  [-PI/4, +PI/4]. Reduce the argument x to y1+y2 = x-k*pi/2 
     *  in [-pi/4 , +pi/4], and let n = k mod 4.
     *  We have
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
     * TRIG(x) returns trig(x) nearly rounded 
     */
    float64_t y[2];
    int32_t n;

    /* High word of |x| */
    int32_t ix;


    /* High word of |x| */
    ix = rtmGetHighf64(x) & RTM_FLOAT64_HI_UNMASK_SIGN;

    /* if |x| ~< pi/4 */
    if(ix <= 0x3fe921fb)
    {
        /* calculate tan (x is already inside the correct interval */
        x = rtmKernelTanf64(x , 0.0 , 1);
    }
        /* tan(+-Inf or NaN) is NaN */
    else if(ix >= RTM_FLOAT64_HI_MASK_EXPONENT)
    {
        /* return NaN */
        x = x - x;
    }
        /* argument reduction needed */
    else
    {
        n = 1 - ((rtmRemPio2f64(x , y) & 1) << 1);

        /*   1 -- n even -1 -- n odd */
        x = rtmKernelTanf64(y[0] , y[1] , n);
    }

    /* return the calculated value */
    return x;
}


float64_t rtmKernelTanf64(float64_t x , float64_t y , int32_t iy)
{

    /* Algorithm
     *  1. Since tan(-x) = -tan(x), we need only to consider positive x.
     *  2. if x < 2^-28 (hx<0x3e300000 0), return x with inexact if x!=0.
     *  3. tan(x) is approximated by a odd polynomial of degree 27 on
     *	   [0,0.67434]
     *		  	         3             27
     *	   	tan(x) ~ x + T1*x + ... + T13*x
     *	   where
     *
     * 	        |tan(x)         2     4            26   |     -59.2
     * 	        |----- - (1+T1*x +T2*x +.... +T13*x    )| <= 2
     * 	        |  x 					|
     *
     *	   Note: tan(x+y) = tan(x) + tan'(x)*y
     *		          ~ tan(x) + (1+x*x)*y
     *	   Therefore, for better accuracy in computing tan(x+y), let
     *		     3      2      2       2       2
     *		r = x *(T2+x *(T3+x *(...+x *(T12+x *T13))))
     *	   then
     *		 		    3    2
     *		tan(x+y) = x + (T1*x + (x *(r+y)+y))
     *
     *      4. For x in [0.67434,pi/4],  let y = pi/4 - x, then
     *		tan(x) = tan(pi/4-y) = (1-tan(y))/(1+tan(y))
     *		       = 1 - 2*(tan(y) - (tan(y)^2)/(1+tan(y)))
     */

#define one xxx[13]
#define pio4 xxx[14]
#define pio4lo xxx[15]
#define T xxx


    static const float64_t xxx[] = {
        3.33333333333334091986e-01 , /* 3FD55555, 55555563 */
        1.33333333333201242699e-01 , /* 3FC11111, 1110FE7A */
        5.39682539762260521377e-02 , /* 3FABA1BA, 1BB341FE */
        2.18694882948595424599e-02 , /* 3F9664F4, 8406D637 */
        8.86323982359930005737e-03 , /* 3F8226E3, E96E8493 */
        3.59207910759131235356e-03 , /* 3F6D6D22, C9560328 */
        1.45620945432529025516e-03 , /* 3F57DBC8, FEE08315 */
        5.88041240820264096874e-04 , /* 3F4344D8, F2F26501 */
        2.46463134818469906812e-04 , /* 3F3026F7, 1A8D1068 */
        7.81794442939557092300e-05 , /* 3F147E88, A03792A6 */
        7.14072491382608190305e-05 , /* 3F12B80F, 32F0A7E9 */
        -1.85586374855275456654e-05 , /* BEF375CB, DB605373 */
        2.59073051863633712884e-05 , /* 3EFB2A70, 74BF7AD4 */
        /* one */ 1.00000000000000000000e+00 , /* 3FF00000, 00000000 */
        /* pio4 */ 7.85398163397448278999e-01 , /* 3FE921FB, 54442D18 */
        /* pio4lo */ 3.06161699786838301793e-17 /* 3C81A626, 33145C07 */
    };

    /* calculate x^2 */
    float64_t z;

    /* aux variable */
    float64_t r;

    /* aux variable */
    float64_t v;

    /* calculate x^4 */
    float64_t w;

    /* calculate x^3 */
    float64_t s;

    /* high word of |x| */
    int32_t ix;

    /* high word of x */
    int32_t hx;


    /* high word of x */
    hx = rtmGetHighf64(x);

    /* high word of |x| */
    ix = hx & RTM_FLOAT64_HI_UNMASK_SIGN;

    /* |x| < 2^-28 */
    if(ix < RTM_FLOAT64_2_EXP_M28)
    {
        /* generate inexact if x != 0 */
        if((int32_t) x == 0)
        {
            /* if x = 0 and k = -1 (that means that we should return -1/tan(x)) */
            if(((ix | rtmGetLowf64(x)) | (iy + 1)) == 0)
            {
                return one / rtmAbsf64(x);
            }
            else
            {
                /* if calculating tan(x) */
                if(iy == 1)
                {
                    /* approximate tan(x) = x for |x| < 2^28 */
                    return x;
                }
                    /* else, we are calculating -1/tan(x) */
                else
                {
                    float64_t a , t;

                    /* compute -1 / (x+y) carefully */
                    z = w = x + y;

                    rtmSetLowf64(&z , 0);

                    v = y - (z - x);
                    t = a = -one / w;

                    rtmSetLowf64(&t , 0);

                    s = one + t * z;

                    return t + a * (s + t * v);
                }
            }
        }
    }

    /* |x| >= 0.6744 */
    if(ix >= RTM_FLOAT64_HI_0D6744)
    {
        if(hx < 0)
        {
            x = -x;
            y = -y;
        }

        z = pio4 - x;
        w = pio4lo - y;
        x = z + w;
        y = 0.0;
    }

    /* calculate x^2 */
    z = x * x;

    /* calculate x^4 */
    w = z * z;

    /* calculate x^3 */
    s = z * x;

    /* break x^5*(T[1]+x^2*T[2]+...) into x^5(T[1]+x^4*T[3]+...+x^20*T[11]) + x^5(x^2*(T[2]+x^4*T[4]+...+x^22*[T12])) */
    r = T[1] + w * (T[3] + w * (T[5] + w * (T[7] + w * (T[9] + w * T[11]))));
    v = z * (T[2] + w * (T[4] + w * (T[6] + w * (T[8] + w * (T[10] + w * T[12])))));
    r = y + z * (s * (r + v) + y);
    r += T[0] * s;
    w = x + r;

    /* if |x| >= 0.67434 */
    if(ix >= RTM_FLOAT64_HI_0D6744)
    {
        v = (float64_t) iy;
        return (float64_t) (1 - ((hx >> 30) & 2)) * (v - 2.0 * (x - (w * w / (w + v) - r)));
    }

    else if(iy == 1)
    {
        return w;
    }
        /* else, we have to calculate -1/tan(x) and |x| < 0.67434 */
    else
    {
        /*
         * if allow error up to 2 ulp, simply return
         * -1.0 / (x+r) here
         */

        /* compute -1.0 / (x+r) accurately */
        float64_t a , t;

        z = w;

        rtmSetLowf64(&z , 0);


        /* z + v = r + x */
        v = r - (z - x);

        /* a = -1.0/w */
        t = a = -RTM_CONSTANT_ONE / w;

        rtmSetLowf64(&t , 0);

        s = RTM_CONSTANT_ONE + t * z;

        return t + a * (s + t * v);
    }
}


/*! @} */
