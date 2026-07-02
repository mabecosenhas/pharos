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


PHAROS_WEAK float64_t rtmJnf64(int32_t n , float64_t x)
{
    /*
     * Special cases:
     *	y0(0)=y1(0)=yn(n,0) = -inf with division by zero signal;
     *	y0(-ve)=y1(-ve)=yn(n,-ve) are NaN with invalid signal.
     * Note 2. About jn(n,x), yn(n,x)
     *	For n=0, j0(x) is called,
     *	for n=1, j1(x) is called,
     *	for n<x, forward recursion us used starting
     *	from values of j0(x) and j1(x).
     *	for n>x, a continued fraction approximation to
     *	j(n,x)/j(n-1,x) is evaluated and then backward
     *	recursion is used starting from a supposed value
     *	for j(n,x). The resulting value of j(0,x) is
     *	compared with the actual value to correct the
     *	supposed value of j(n,x).
     *
     *	yn(n,x) is similar in all respects, except
     *	that forward recursion is used for all
     *	values of n>1.
     */

    /* iterator variable */
    int32_t i;

    /* high word of x */
    int32_t hx;

    /* high word of |x| */
    int32_t ix;

    /* low word of x */
    int32_t lx;

    /* sign of x */
    int32_t sgn;

    float64_t a , b , temp , di;
    float64_t z , w;


    /* J(-n,x) = (-1)^n * J(n, x), J(n, -x) = (-1)^n * J(n, x)
     * Thus, J(-n,x) = J(n,-x) */

    /* high word of x */
    hx = rtmGetHighf64(x);

    /* high word of |x| */
    ix = hx & RTM_FLOAT64_HI_UNMASK_SIGN;

    /* low word of x */
    lx = rtmGetLowf64(x);

    /* if J(n,NaN) */
    if((ix | ((uint32_t) (lx | -lx)) >> 31) > RTM_FLOAT64_HI_MASK_EXPONENT)
    {
        /* then return NaN */
        return x + x;
    }

    if(n < 0)
    {
        n = -n;
        x = -x;

        /* flip the sign */
        hx ^= RTM_FLOAT64_HI_MASK_SIGN;
    }

    if(n == 0)
    {
        return (rtmJ0f64(x));
    }

    if(n == 1)
    {
        return (rtmJ1f64(x));
    }

    /* even n -- 0, odd n -- sign(x) */
    sgn = (n & 1)&(hx >> 31);

    /* calculate |x| */
    x = rtmAbsf64(x);

    /* if x is 0 or inf */
    if((ix | lx) == 0 || ix >= RTM_FLOAT64_HI_MASK_EXPONENT)
    {
        b = RTM_CONSTANT_ZERO;
    }
    else if((float64_t) n <= x)
    {
        /* Safe to use J(n+1,x)=2n/x *J(n,x)-J(n-1,x) */

        /* if x > 2^302 */
        if(ix >= 0x52D00000)
        {
            /* (x >> n^2) 
             *	    Jn(x) = cos(x-(2n+1)*pi/4)*sqrt(2/x*pi)
             *	    Yn(x) = sin(x-(2n+1)*pi/4)*sqrt(2/x*pi)
             *	    Let s=sin(x), c=cos(x), 
             *		xn=x-(2n+1)*pi/4, sqt2 = sqrt(2),then
             *
             *		   n	sin(xn)*sqt2	cos(xn)*sqt2
             *		----------------------------------
             *		   0	 s-c		 c+s
             *		   1	-s-c 		-c+s
             *		   2	-s+c		-c-s
             *		   3	 s+c		 c-s
             */
            switch(n & 3)
            {
            case 0: temp = rtmCosf64(x) + rtmSinf64(x);
                break;
            case 1: temp = -rtmCosf64(x) + rtmSinf64(x);
                break;
            case 2: temp = -rtmCosf64(x) - rtmSinf64(x);
                break;
            case 3: temp = rtmCosf64(x) - rtmSinf64(x);
                break;
            }

            b = RTM_CONSTANT_PI_INV_SQRT * temp / rtmSqrtf64(x);
        }
        else
        {
            a = rtmJ0f64(x);
            b = rtmJ1f64(x);

            for(i = 1; i < n; i++)
            {
                temp = b;
                b = b * ((float64_t) (i + i) / x) - a; /* avoid underflow */
                a = temp;
            }
        }
    }
    else
    {
        /* if x < 2^-29 */
        if(ix < 0x3e100000)
        {
            /* x is tiny, return the first Taylor expansion of J(n,x) 
             * J(n,x) = 1/n!*(x/2)^n  - ...
             */
            if(n > 33)
            {
                /* underflow */
                b = RTM_CONSTANT_ZERO;
            }
            else
            {
                temp = x * RTM_CONSTANT_HALF;
                b = temp;

                for(a = RTM_CONSTANT_ONE , i = 2; i <= n; i++)
                {
                    /* a = n! */
                    a *= (float64_t) i;

                    /* b = (x/2)^n */
                    b *= temp;
                }

                b = b / a;
            }
        }
        else
        {
            /* use backward recurrence */
            /* 			x      x^2      x^2       
             *  J(n,x)/J(n-1,x) =  ----   ------   ------   .....
             *			2n  - 2(n+1) - 2(n+2)
             *
             * 			1      1        1       
             *  (for large x)   =  ----  ------   ------   .....
             *			2n   2(n+1)   2(n+2)
             *			-- - ------ - ------ - 
             *			 x     x         x
             *
             * Let w = 2n/x and h=2/x, then the above quotient
             * is equal to the continued fraction:
             *		    1
             *	= -----------------------
             *		       1
             *	   w - -----------------
             *			  1
             * 	        w+h - ---------
             *		       w+2h - ...
             *
             * To determine how many terms needed, let
             * Q(0) = w, Q(1) = w(w+h) - 1,
             * Q(k) = (w+k*h)*Q(k-1) - Q(k-2),
             * When Q(k) > 1e4	good for single 
             * When Q(k) > 1e9	good for float64_t 
             * When Q(k) > 1e17	good for quadruple 
             */
            /* determine k */
            float64_t t , v;
            float64_t q0 , q1 , h , tmp;
            int32_t k , m;

            w = (n + n) / (float64_t) x;
            h = 2.0 / (float64_t) x;
            q0 = w;
            z = w + h;
            q1 = w * z - RTM_CONSTANT_ONE;
            k = 1;

            while(q1 < 1.0e9)
            {
                k += 1;
                z += h;
                tmp = z * q1 - q0;
                q0 = q1;
                q1 = tmp;
            }

            m = n + n;

            for(t = RTM_CONSTANT_ZERO , i = 2 * (n + k); i >= m; i -= 2)
            {
                t = RTM_CONSTANT_ONE / (i / x - t);
            }

            a = t;
            b = RTM_CONSTANT_ONE;

            /*  estimate log((2/x)^n*n!) = n*log(2/x)+n*ln(n)
             *  Hence, if n*(log(2n/x)) > ...
             *  single 8.8722839355e+01
             *  float64_t 7.09782712893383973096e+02
             *  long float64_t 1.1356523406294143949491931077970765006170e+04
             *  then recurrent value may overflow and the result is 
             *  likely underflow to zero
             */
            tmp = n;
            v = RTM_CONSTANT_TWO / x;
            tmp = tmp * rtmLogf64(rtmAbsf64(v * tmp));

            if(tmp < 7.09782712893383973096e+02)
            {
                for(i = n - 1 , di = (float64_t) (i + i); i > 0; i--)
                {
                    temp = b;
                    b *= di;
                    b = b / x - a;
                    a = temp;
                    di -= RTM_CONSTANT_TWO;
                }
            }
            else
            {
                for(i = n - 1 , di = (float64_t) (i + i); i > 0; i--)
                {
                    temp = b;
                    b *= di;
                    b = b / x - a;
                    a = temp;
                    di -= RTM_CONSTANT_TWO;

                    /* scale b to avoid spurious overflow */
                    if(b > 1e100)
                    {
                        a /= b;
                        t /= b;
                        b = RTM_CONSTANT_ONE;
                    }
                }
            }
            b = (t * rtmJ0f64(x) / b);
        }
    }

    if(sgn == 1)
    {
        return -b;
    }
    else
    {
        return b;
    }
}


PHAROS_WEAK float64_t rtmYnf64(int32_t n , float64_t x)
{
    int32_t i , hx , ix , lx;
    int32_t sign;
    float64_t a , b , temp;

    /* return value of the function */
    float64_t result;


    /* high word of x */
    hx = rtmGetHighf64(x);

    /* high word of |x| */
    ix = hx & RTM_FLOAT64_HI_UNMASK_SIGN;

    /* low word of x */
    lx = rtmGetLowf64(x);


    /* if x is NaN or Inf  */
    if((ix | ((uint32_t) (lx | -lx)) >> 31) > RTM_FLOAT64_HI_MASK_EXPONENT)
    {
        /* Y(n,NaN) is NaN */
        result = x + x;
    }
        /* if x is 0 */
    else if((ix | lx) == 0)
    {
        result = -RTM_CONSTANT_ONE / RTM_CONSTANT_ZERO;
    }
        /* if x is negative */
    else if(hx < 0)
    {
        result = RTM_CONSTANT_ZERO / RTM_CONSTANT_ZERO;
    }
    else
    {
        sign = 1;

        if(n < 0)
        {
            n = -n;
            sign = 1 - ((n & 1) << 1);
        }

        if(n == 0)
        {
            result = (rtmY0f64(x));
        }

        else if(n == 1)
        {
            result = (sign * rtmY1f64(x));
        }

        else if(ix == RTM_FLOAT64_HI_MASK_EXPONENT)
        {
            result = RTM_CONSTANT_ZERO;
        }
        else
        {
            /* if x > 2^302 */
            if(ix >= 0x52D00000)
            {
                /* (x >> n**2) 
                 *	    Jn(x) = cos(x-(2n+1)*pi/4)*sqrt(2/x*pi)
                 *	    Yn(x) = sin(x-(2n+1)*pi/4)*sqrt(2/x*pi)
                 *	    Let s=sin(x), c=cos(x), 
                 *		xn=x-(2n+1)*pi/4, sqt2 = sqrt(2),then
                 *
                 *		   n	sin(xn)*sqt2	cos(xn)*sqt2
                 *		----------------------------------
                 *		   0	 s-c		 c+s
                 *		   1	-s-c 		-c+s
                 *		   2	-s+c		-c-s
                 *		   3	 s+c		 c-s
                 */
                switch(n & 3)
                {
                case 0: temp = rtmSinf64(x) - rtmCosf64(x);
                    break;
                case 1: temp = -rtmSinf64(x) - rtmCosf64(x);
                    break;
                case 2: temp = -rtmSinf64(x) + rtmCosf64(x);
                    break;
                case 3: temp = rtmSinf64(x) + rtmCosf64(x);
                    break;
                }

                b = RTM_CONSTANT_PI_INV_SQRT * temp / rtmSqrtf64(x);
            }
            else
            {
                a = rtmY0f64(x);
                b = rtmY1f64(x);

                /* quit if b is -inf */
                for(i = 1; (i < n) && (rtmGetHighf64(b) != (int32_t) 0xfff00000); i++)
                {
                    temp = b;
                    b = ((float64_t) (i + i) / x) * b - a;
                    a = temp;
                }
            }

            if(sign > 0)
            {
                result = b;
            }
            else
            {
                result = -b;
            }
        }
    }

    /* return the calculated value */
    return result;
}

/*! @} */
