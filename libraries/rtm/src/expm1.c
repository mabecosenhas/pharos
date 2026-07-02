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
 * Copyright (C) 2004 by Sun Microsystems, Inc. All rights reserved.
 *
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


PHAROS_WEAK float64_t rtmExpm1f64(float64_t x)
{
    /* Method
     *   1. Argument reduction:
     *	Given x, find r and integer k such that
     *
     *               x = k*ln2 + r,  |r| <= 0.5*ln2 ~ 0.34658  
     *
     *      Here a correction term c will be computed to compensate 
     *	the error in r when rounded to a floating-point number.
     *
     *   2. Approximating expm1(r) by a special rational function on
     *	the interval [0,0.34658]:
     *	Since
     *	    r*(exp(r)+1)/(exp(r)-1) = 2+ r^2/6 - r^4/360 + ...
     *	we define R1(r*r) by
     *	    r*(exp(r)+1)/(exp(r)-1) = 2+ r^2/6 * R1(r*r)
     *	That is,
     *	    R1(r**2) = 6/r *((exp(r)+1)/(exp(r)-1) - 2/r)
     *	             = 6/r * ( 1 + 2.0*(1/(exp(r)-1) - 1/r))
     *	             = 1 - r^2/60 + r^4/2520 - r^6/100800 + ...
     *      We use a special Remes algorithm on [0,0.347] to generate 
     * 	a polynomial of degree 5 in r*r to approximate R1. The 
     *	maximum error of this polynomial approximation is bounded 
     *	by 2**-61. In other words,
     *	    R1(z) ~ 1.0 + Q1*z + Q2*z**2 + Q3*z**3 + Q4*z**4 + Q5*z**5
     *	where 	Q1  =  -1.6666666666666567384E-2,
     * 	        Q2  =   3.9682539681370365873E-4,
     * 	        Q3  =  -9.9206344733435987357E-6,
     * 	        Q4  =   2.5051361420808517002E-7,
     * 	        Q5  =  -6.2843505682382617102E-9;
     *          (where z=r*r, and the values of Q1 to Q5 are listed below)
     *	with error bounded by
     *	    |                  5           |     -61
     *	    | 1.0+Q1*z+...+Q5*z   -  R1(z) | <= 2 
     *	    |                              |
     *	
     *	expm1(r) = exp(r)-1 is then computed by the following 
     * 	specific way which minimize the accumulation rounding error: 
     *			       2     3
     *			      r     r    [ 3 - (R1 + R1*r/2)  ]
     *	      expm1(r) = r + --- + --- * [--------------------]
     *		              2     2    [ 6 - r*(3 - R1*r/2) ]
     *	
     *	To compensate the error in the argument reduction, we use
     *		expm1(r+c) = expm1(r) + c + expm1(r)*c 
     *			   ~ expm1(r) + c + r*c 
     *	Thus c+r*c will be added in as the correction terms for
     *	expm1(r+c). Now rearrange the term to avoid optimization 
     * 	screw up:
     *		        (      2                                    2 )
     *		        ({  ( r    [ R1 -  (3 - R1*r/2) ]  )  }    r  )
     *	 expm1(r+c)~r - ({r*(--- * [--------------------]-c)-c} - --- )
     *	                ({  ( 2    [ 6 - r*(3 - R1*r/2) ]  )  }    2  )
     *                      (                                             )
     *    	
     *		   = r - E
     *   3. Scale back to obtain expm1(x):
     *	From step 1, we have
     *	   expm1(x) = either 2^k*[expm1(r)+1] - 1
     *		    = or     2^k*[expm1(r) + (1-2^-k)]
     *   4. Implementation notes:
     *	(A). To save one multiplication, we scale the coefficient Qi
     *	     to Qi*2^i, and replace z by (x^2)/2.
     *	(B). To achieve maximum accuracy, we compute expm1(x) by
     *	  (i)   if x < -56*ln2, return -1.0, (raise inexact if x!=inf)
     *	  (ii)  if k=0, return r-E
     *	  (iii) if k=-1, return 0.5*(r-E)-0.5
     *        (iv)	if k=1 if r < -0.25, return 2*((r+0.5)- E)
     *	       	       else	     return  1.0+2.0*(r-E);
     *	  (v)   if (k<-2||k>56) return 2^k(1-(E-r)) - 1 (or exp(x)-1)
     *	  (vi)  if k <= 20, return 2^k((1-2^-k)-(E-r)), else
     *	  (vii) return 2^k(1-((E+2^-k)-r)) 
     *
     * Special cases:
     *	expm1(INF) is INF, expm1(NaN) is NaN;
     *	expm1(-INF) is -1, and
     *	for finite argument, only expm1(0)=0 is exact.
     *
     * Accuracy:
     *	according to an error analysis, the error is always less than
     *	1 ulp (unit in the last place).
     *
     * Misc. info.
     *	For IEEE float64_t 
     *	    if x >  7.09782712893383973096e+02 then expm1(x) overflow
     *
     * Constants:
     * The hexadecimal values are the intended ones for the following 
     * constants. The decimal values may be used, provided that the 
     * compiler will convert from decimal to binary accurately enough
     * to produce the hexadecimal values shown.
     */

    /* 0x40862E42, 0xFEFA39EF */
    static const float64_t o_threshold = 7.09782712893383973096e+02;
    /* scaled coefficients related to expm1 */

    /* BFA11111 111110F4 */
    static const float64_t Q1 = -3.33333333333331316428e-02;

    /* 3F5A01A0 19FE5585 */
    static const float64_t Q2 = 1.58730158725481460165e-03;

    /* BF14CE19 9EAADBB7 */
    static const float64_t Q3 = -7.93650757867487942473e-05;

    /* 3ED0CFCA 86E65239 */
    static const float64_t Q4 = 4.00821782732936239552e-06;



    /* BE8AFDB7 6E09C32D */
    static const float64_t Q5 = -2.01099218183624371326e-07;

    float64_t y , hi , lo , c , t , e , hxs , hfx , r1;
    int32_t k , xsb;

    /* high word of x */
    int32_t hx;

    /* high word of |x| */
    uint32_t ix;


    /* high word of x */
    hx = rtmGetHighf64(x);

    /* sign bit of x */
    xsb = hx & RTM_FLOAT64_HI_MASK_SIGN;

    /* y = |x| */
    y = rtmAbsf64(x);

    /* high word of |x| */
    ix = hx & RTM_FLOAT64_HI_UNMASK_SIGN;

    /* filter out huge and non-finite argument */

    /* if |x| >= 56*ln2 */
    if(ix >= 0x4043687A)
    {
        /* if |x| >= 709.78... */
        if(ix >= 0x40862E42)
        {
            /* if x is NaN or Inf */
            if(ix >= RTM_FLOAT64_HI_MASK_EXPONENT)
            {
                /* if x is NaN */
                if(((ix & 0xfffff) | rtmGetLowf64(x)) != 0)
                {
                    /* return NaN */
                    return x + x;
                }
                    /* else, x is +-Inf */
                else
                {
                    /* exp(+Inf)-1 = Inf and exp(-Inf)-1 = -1*/
                    return (xsb == 0) ? x : -RTM_CONSTANT_ONE;
                }
            }

            if(x > o_threshold)
            {
                /* overflow */
                return RTM_CONSTANT_HUGE * RTM_CONSTANT_HUGE;
            }
        }

        /* if x < -56*ln2, return -1.0 with inexact */
        if(xsb != 0)
        {
            /* raise inexact */
            if(x + RTM_CONSTANT_TINY < 0.0)
            {
                /* return -1 */
                return RTM_CONSTANT_TINY - RTM_CONSTANT_ONE;
            }
        }
    }

    /* argument reduction */

    /* if  |x| > 0.5 ln2 */
    if(ix > 0x3fd62e42)
    {
        /* and |x| < 1.5 ln2 */
        if(ix < 0x3FF0A2B2)
        {
            /* check the special case where x is +1 */
            if(((hx - RTM_FLOAT64_HI_ONE) | rtmGetLowf64(x)) == 0)
            {
                /* e^1 - 1 = e - 1 */
                return RTM_CONSTANT_E - 1.0;
            }
            else if(xsb == 0)
            {
                hi = x - RTM_CONSTANT_LN2_HI;
                lo = RTM_CONSTANT_LN2_LOW;
                k = 1;
            }
            else
            {
                hi = x + RTM_CONSTANT_LN2_HI;
                lo = -RTM_CONSTANT_LN2_LOW;
                k = -1;
            }
        }
        else
        {
            k = RTM_CONSTANT_1_DIV_LN2 * x + ((xsb == 0) ? RTM_CONSTANT_HALF : -RTM_CONSTANT_HALF);
            t = k;
            hi = x - t*RTM_CONSTANT_LN2_HI; /* t*ln2_hi is exact here */
            lo = t*RTM_CONSTANT_LN2_LOW;
        }
        x = hi - lo;
        c = (hi - x) - lo;
    }
        /* if |x| < 2^-54 */
    else if(ix < 0x3c900000)
    {
        /* return x */

        /* return x with inexact flags when x!=0 */
        t = RTM_CONSTANT_HUGE + x;

        return x - (t - (RTM_CONSTANT_HUGE + x));
    }
    else
    {
        k = 0;
    }

    /* x is now in primary range */
    hfx = RTM_CONSTANT_HALF * x;
    hxs = x * hfx;
    r1 = RTM_CONSTANT_ONE + hxs * (Q1 + hxs * (Q2 + hxs * (Q3 + hxs * (Q4 + hxs * Q5))));
    t = 3.0 - r1*hfx;
    e = hxs * ((r1 - t) / (6.0 - x * t));

    if(k == 0)
    {
        /* c is 0 */
        return x - (x * e - hxs);
    }
    else
    {
        e = (x * (e - c) - c);
        e -= hxs;

        if(k == -1)
        {
            y = RTM_CONSTANT_HALF * (x - e) - RTM_CONSTANT_HALF;
        }
        else if(k == 1)
        {
            if(x < -0.25)
            {
                y = -2.0 * (e - (x + RTM_CONSTANT_HALF));
            }
            else
            {
                y = RTM_CONSTANT_ONE + 2.0 * (x - e);
            }
        }

        else if(k <= -2 || k > 56)
        {
            /* suffice to return exp(x)-1 */
            y = RTM_CONSTANT_ONE - (e - x);

            /* add k to y's exponent */
            rtmSetHighf64(&y , rtmGetHighf64(y) + (k << 20));

            y = y - RTM_CONSTANT_ONE;
        }
        else
        {
            t = RTM_CONSTANT_ONE;

            if(k < 20)
            {
                /* t = 1 - 2^-k */
                rtmSetHighf64(&t , RTM_FLOAT64_HI_ONE - (0x200000 >> k));

                y = t - (e - x);

                /* add k to y's exponent */
                rtmSetHighf64(&y , rtmGetHighf64(y) + (k << 20));
            }
            else
            {
                /* 2^-k */
                rtmSetHighf64(&t , ((0x3ff - k) << 20));

                y = x - (e + t);
                y += RTM_CONSTANT_ONE;

                /* add k to y's exponent */
                rtmSetHighf64(&y , rtmGetHighf64(y) + (k << 20));
            }
        }
    }

    /* return the calculated value */
    return y;
}

/*! @} */
