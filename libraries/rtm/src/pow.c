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


PHAROS_WEAK float64_t rtmPowf64(float64_t x , float64_t y)
{
    /* Method:  Let x = 2 * (1+f)
     *      1. Compute and return log2(x) in two pieces:
     *         log2(x) = w1 + w2,
     *         where w1 has 53-24 = 29 bit trailing zeros.
     *      2. Perform y*log2(x) = n+y' by simulating muti-precision 
     *         arithmetic, where |y'|<=0.5.
     *      3. Return x^y = 2^n*exp(y'*log2)
     *
     * Special cases:
     *      1.  (anything) ** 0  is 1
     *      2.  (anything) ** 1  is itself
     *      3.  (anything) ** NAN is NAN
     *      4.  NAN ** (anything except 0) is NAN
     *      5.  +-(|x| > 1) **  +INF is +INF
     *      6.  +-(|x| > 1) **  -INF is +0
     *      7.  +-(|x| < 1) **  +INF is +0
     *      8.  +-(|x| < 1) **  -INF is +INF
     *      9.  +-1         ** +-INF is NAN
     *      10. +0 ** (+anything except 0, NAN)               is +0
     *      11. -0 ** (+anything except 0, NAN, odd integer)  is +0
     *      12. +0 ** (-anything except 0, NAN)               is +INF
     *      13. -0 ** (-anything except 0, NAN, odd integer)  is +INF
     *      14. -0 ** (odd integer) = -( +0 ** (odd integer) )
     *      15. +INF ** (+anything except 0,NAN) is +INF
     *      16. +INF ** (-anything except 0,NAN) is +0
     *      17. -INF ** (anything)  = -0 ** (-anything)
     *      18. (-anything) ** (integer) is (-1)**(integer)*(+anything**integer)
     *      19. (-anything except 0 and inf) ** (non-integer) is NAN
     *
     * Accuracy:
     *      pow(x,y) returns x^y nearly rounded. In particular
     *	            pow(integer,integer)
     *      always returns the correct integer provided it is 
     *      representable.
     *
     * Constants :
     * The hexadecimal values are the intended ones for the following 
     * constants. The decimal values may be used, provided that the 
     * compiler will convert from decimal to binary accurately enough 
     * to produce the hexadecimal values shown.
     */

    static const float64_t bp[] = {RTM_CONSTANT_ONE , 1.5};

    /* 0x3FE2B803, 0x40000000 */
    static const float64_t dp_h[] = {0.0 , 5.84962487220764160156e-01};

    /* 0x3E4CFDEB, 0x43CFD006 */
    static const float64_t dp_l[] = {0.0 , 1.35003920212974897128e-08};

    /* poly coefs for (3/2)*(log(x)-2s-2/3*s**3 */

    /* 0x3FE33333, 0x33333303 */
    static const float64_t L1 = 5.99999999999994648725e-01;

    /* 0x3FDB6DB6, 0xDB6FABFF */
    static const float64_t L2 = 4.28571428578550184252e-01;

    /* 0x3FD55555, 0x518F264D */
    static const float64_t L3 = 3.33333329818377432918e-01;

    /* 0x3FD17460, 0xA91D4101 */
    static const float64_t L4 = 2.72728123808534006489e-01;

    /* 0x3FCD864A, 0x93C9DB65 */
    static const float64_t L5 = 2.30660745775561754067e-01;

    /* 0x3FCA7E28, 0x4A454EEF */
    static const float64_t L6 = 2.06975017800338417784e-01;

    /* 0x3FC55555, 0x5555553E */
    static const float64_t P1 = 1.66666666666666019037e-01;

    /* 0xBF66C16C, 0x16BEBD93 */
    static const float64_t P2 = -2.77777777770155933842e-03;

    /* 0x3F11566A, 0xAF25DE2C */
    static const float64_t P3 = 6.61375632143793436117e-05;

    /* 0xBEBBBD41, 0xC5D26BF1 */
    static const float64_t P4 = -1.65339022054652515390e-06;

    /* 0x3E663769, 0x72BEA4D0 */
    static const float64_t P5 = 4.13813679705723846039e-08;

    /* 0x3FE62E42, 0xFEFA39EF */
    static const float64_t lg2 = 6.93147180559945286227e-01;

    /* 0x3FE62E43, 0x00000000 */
    static const float64_t lg2_h = 6.93147182464599609375e-01;

    /* 0xBE205C61, 0x0CA86C39 */
    static const float64_t lg2_l = -1.90465429995776804525e-09;

    /* -(1024-log2(ovfl+.5ulp)) */
    static const float64_t ovt = 8.0085662595372944372e-0017;

    /* 0x3FEEC709, 0xDC3A03FD =2/(3ln2) */
    static const float64_t cp = 9.61796693925975554329e-01;

    /* 0x3FEEC709, 0xE0000000 =(float)cp */
    static const float64_t cp_h = 9.61796700954437255859e-01;

    /* 0xBE3E2FE0, 0x145B01F5 =tail of cp_h*/
    static const float64_t cp_l = -7.02846165095275826516e-09;

    float64_t z , ax , z_h , z_l , p_h , p_l;
    float64_t y1 , t1 , t2 , r , s , t , u , v , w;
    int32_t i , j , k , yisint , n;

    /* high word of x */
    int32_t hx;

    /* high word of y */
    int32_t hy;

    /* high word of |x| */
    int32_t ix;

    /* high word of |y| */
    int32_t iy;

    /* low part of x */
    uint32_t lx;

    /* low part of y */
    uint32_t ly;


    /* high word of x */
    hx = rtmGetHighf64(x);

    /* low word of x */
    lx = rtmGetLowf64(x);

    /* high word of y */
    hy = rtmGetHighf64(y);

    /* low word of y */
    ly = rtmGetLowf64(y);

    /* high word of |x| */
    ix = hx & RTM_FLOAT64_HI_UNMASK_SIGN;

    /* high word of |y| */
    iy = hy & RTM_FLOAT64_HI_UNMASK_SIGN;

    /* if y == 0 */
    if((iy | ly) == 0)
    {
        /* then x^0 = 1 */
        return RTM_CONSTANT_ONE;
    }

    /* check the special case where x is +1 (even if y is NaN) */
    if(((hx - RTM_FLOAT64_HI_ONE) | lx) == 0)
    {
        /* 1^whatever = 1 */
        return RTM_CONSTANT_ONE;
    }

    /* if x or y is +-NaN */
    if(ix > RTM_FLOAT64_HI_MASK_EXPONENT || ((ix == RTM_FLOAT64_HI_MASK_EXPONENT)&&(lx != 0)) ||
       iy > RTM_FLOAT64_HI_MASK_EXPONENT || ((iy == RTM_FLOAT64_HI_MASK_EXPONENT)&&(ly != 0)))
    {
        /* return x+y */
        return x + y;
    }

    /* determine if y is an odd int when x < 0
     * yisint = 0	... y is not an integer
     * yisint = 1	... y is an odd int
     * yisint = 2	... y is an even int */
    yisint = 0;

    if(hx < 0)
    {
        if(iy >= 0x43400000)
        {
            /* even integer y */
            yisint = 2;
        }
        else
        {
            if(iy >= RTM_FLOAT64_HI_ONE)
            {
                /* exponent */
                k = (iy >> 20) - 0x3ff;

                if(k > 20)
                {
                    j = ly >> (52 - k);

                    if((uint32_t) (j << (52 - k)) == ly)
                    {
                        yisint = 2 - (j & 1);
                    }
                }
                else
                {
                    if(ly == 0)
                    {
                        j = iy >> (20 - k);

                        if((j << (20 - k)) == iy)
                        {
                            yisint = 2 - (j & 1);
                        }
                    }
                }
            }
        }
    }

    /* special value of y */
    if(ly == 0)
    {
        /* if y is +-inf */
        if(iy == RTM_FLOAT64_HI_MASK_EXPONENT)
        {
            /* if x is -1 (the x = +1 has been dealt with before) */
            if(((ix - RTM_FLOAT64_HI_ONE) | lx) == 0)
            {
                /* (-1)^inf is NaN */
                return y - y;

            }
                /* if (|x|>1)**+-inf = inf,0 */
            else if(ix >= RTM_FLOAT64_HI_ONE)
            {
                return (hy >= 0) ? y : RTM_CONSTANT_ZERO;
            }
                /* (|x|<1)**-,+inf = inf,0 */
            else
            {
                return (hy < 0) ? -y : RTM_CONSTANT_ZERO;
            }
        }
        /* if y is  +-1 */
        if(iy == RTM_FLOAT64_HI_ONE)
        {
            if(hy < 0)
            {
                return RTM_CONSTANT_ONE / x;
            }
            else
            {
                return x;
            }
        }

        /* if y is  2 */
        if(hy == 0x40000000)
        {
            return x * x;
        }

        /* y is  0.5 */
        if(hy == 0x3fe00000)
        {
            /* if x >= +0 */
            if(hx >= 0)
            {
                return rtmSqrtf64(x);
            }
        }
    }

    /* calculate |x| */
    ax = rtmAbsf64(x);

    /* special value of x */
    if(lx == 0)
    {
        /* if x is +-0 or +-inf or +-1 */
        if(ix == RTM_FLOAT64_HI_MASK_EXPONENT || ix == 0 || ix == RTM_FLOAT64_HI_ONE)
        {

            z = ax;

            if(hy < 0)
            {
                /* z = (1/|x|) */
                z = RTM_CONSTANT_ONE / z;
            }

            if(hx < 0)
            {
                if(((ix - RTM_FLOAT64_HI_ONE) | yisint) == 0)
                {
                    /* (-1)**non-int is NaN */
                    z = (z - z) / (z - z);
                }
                else if(yisint == 1)
                {
                    /* (x<0)**odd = -(|x|**odd) */
                    z = -z;
                }
            }
            return z;
        }
    }

    n = ((uint32_t) hx >> 31) - 1;

    /* (x < 0)**(non-int) is NaN */
    if((n | yisint) == 0)
    {
        return (x - x) / (x - x);
    }

    s = RTM_CONSTANT_ONE;

    /* s (sign of result -ve**odd) = -1 else = 1 */
    if((n | (yisint - 1)) == 0)
    {
        /* (-ve)**(odd int) */
        s = -RTM_CONSTANT_ONE;
    }

    /* |y| is huge */

    /* if |y| > 2^31 */
    if(iy > 0x41e00000)
    {
        /* if |y| > 2^64, must overflow or underflow */
        if(iy > 0x43f00000)
        {
            if(ix <= 0x3fefffff)
            {
                return (hy < 0) ? RTM_CONSTANT_HUGE * RTM_CONSTANT_HUGE : RTM_CONSTANT_TINY * RTM_CONSTANT_TINY;
            }

            if(ix >= RTM_FLOAT64_HI_ONE)
            {
                return (hy > 0) ? RTM_CONSTANT_HUGE * RTM_CONSTANT_HUGE : RTM_CONSTANT_TINY * RTM_CONSTANT_TINY;
            }
        }

        /* over/underflow if x is not close to one */
        if(ix < 0x3fefffff)
        {
            return (hy < 0) ? s * RTM_CONSTANT_HUGE * RTM_CONSTANT_HUGE : s * RTM_CONSTANT_TINY * RTM_CONSTANT_TINY;
        }

        if(ix > RTM_FLOAT64_HI_ONE)
        {
            return (hy > 0) ? s * RTM_CONSTANT_HUGE * RTM_CONSTANT_HUGE : s * RTM_CONSTANT_TINY * RTM_CONSTANT_TINY;
        }

        /* now |1-x| is tiny <= 2^-20, suffice to compute log(x) by x-x^2/2+x^3/3-x^4/4 */
        t = ax - RTM_CONSTANT_ONE; /* t has 20 trailing zeros */
        w = (t * t) * (RTM_CONSTANT_HALF - t * (0.3333333333333333333333 - t * 0.25));

        /* ivln2_h has 21 sig. bits */
        u = RTM_CONSTANT_1_DIV_LN2_HI * t;
        v = t * RTM_CONSTANT_1_DIV_LN2_LOW - w * RTM_CONSTANT_1_DIV_LN2;
        t1 = u + v;

        rtmSetLowf64(&t1 , 0);

        t2 = v - (t1 - u);
    }
    else
    {
        float64_t ss , s2 , s_h , s_l , t_h , t_l;
        n = 0;

        /* take care subnormal number */
        if(ix < 0x00100000)
        {
            ax *= RTM_CONSTANT_POWER_2_53;
            n -= 53;
            ix = rtmGetHighf64(ax);
        }

        n += ((ix) >> 20) - 0x3ff;
        j = ix & RTM_FLOAT64_HI_MASK_FRAC;

        /* determine interval */

        /* normalize ix */
        ix = j | RTM_FLOAT64_HI_ONE;

        /* if |x| < sqrt(3/2) */
        if(j <= 0x3988E)
        {
            k = 0;
        }
            /* if |x| < sqrt(3) */
        else if(j < 0xBB67A)
        {
            k = 1;
        }
        else
        {
            k = 0;
            n += 1;
            ix -= 0x00100000;
        }

        rtmSetHighf64(&ax , ix);

        /* compute ss = s_h+s_l = (x-1)/(x+1) or (x-1.5)/(x+1.5) */

        /* bp[0]=1.0, bp[1]=1.5 */
        u = ax - bp[k];
        v = RTM_CONSTANT_ONE / (ax + bp[k]);
        ss = u*v;
        s_h = ss;

        rtmSetLowf64(&s_h , 0);

        /* t_h=ax+bp[k] High */
        t_h = RTM_CONSTANT_ZERO;
        rtmSetHighf64(&t_h , ((ix >> 1) | 0x20000000) + 0x00080000 + (k << 18));

        t_l = ax - (t_h - bp[k]);
        s_l = v * ((u - s_h * t_h) - s_h * t_l);

        /* compute log(ax) */
        s2 = ss*ss;
        r = s2 * s2 * (L1 + s2 * (L2 + s2 * (L3 + s2 * (L4 + s2 * (L5 + s2 * L6)))));
        r += s_l * (s_h + ss);
        s2 = s_h*s_h;
        t_h = 3.0 + s2 + r;

        rtmSetLowf64(&t_h , 0);

        t_l = r - ((t_h - 3.0) - s2);

        /* u+v = ss*(1+...) */
        u = s_h*t_h;
        v = s_l * t_h + t_l*ss;

        /* 2/(3log2)*(ss+...) */
        p_h = u + v;

        rtmSetLowf64(&p_h , 0);

        p_l = v - (p_h - u);
        z_h = cp_h*p_h; /* cp_h+cp_l = 2/(3*log2) */
        z_l = cp_l * p_h + p_l * cp + dp_l[k];

        /* log2(ax) = (ss+..)*2/(3*log2) = n + dp_h + z_h + z_l */
        t = (float64_t) n;
        t1 = (((z_h + z_l) + dp_h[k]) + t);

        rtmSetLowf64(&t1 , 0);

        t2 = z_l - (((t1 - t) - dp_h[k]) - z_h);
    }

    /* split up y into y1+y2 and compute (y1+y2)*(t1+t2) */
    y1 = y;

    rtmSetLowf64(&y1 , 0);

    p_l = (y - y1) * t1 + y*t2;
    p_h = y1 * t1;
    z = p_l + p_h;

    /* get the high word of z */
    j = rtmGetHighf64(z);

    /* get the low word of z */
    i = rtmGetLowf64(z);

    /* z >= 1024 */
    if(j >= 0x40900000)
    {
        /* if z > 1024 */
        if(((j - 0x40900000) | i) != 0)
        {
            /* overflow */
            return s * RTM_CONSTANT_HUGE * RTM_CONSTANT_HUGE;
        }
        else
        {
            if(p_l + ovt > z - p_h)
            {
                /* overflow */
                return s * RTM_CONSTANT_HUGE * RTM_CONSTANT_HUGE;
            }
        }
    }
    else if((j & RTM_FLOAT64_HI_UNMASK_SIGN) >= 0x4090cc00)
    {
        /* z <= -1075 */
        if(((j - 0xc090cc00) | i) != 0) /* z < -1075 */
        {
            /* underflow */
            return s * RTM_CONSTANT_TINY * RTM_CONSTANT_TINY;
        }
        else
        {
            if(p_l <= z - p_h)
            {
                /* underflow */
                return s * RTM_CONSTANT_TINY * RTM_CONSTANT_TINY;
            }
        }
    }

    /* compute 2^(p_h+p_l) */
    i = j & RTM_FLOAT64_HI_UNMASK_SIGN;
    k = (i >> 20) - 0x3ff;
    n = 0;

    /* if |z| > 0.5, set n = [z+0.5] */
    if(i > RTM_FLOAT64_HI_1_DIV_2)
    {
        n = j + (0x00100000 >> (k + 1));

        /* new k for n */
        k = ((n & RTM_FLOAT64_HI_UNMASK_SIGN) >> 20) - 0x3ff;
        t = RTM_CONSTANT_ZERO;

        rtmSetHighf64(&t , (n&~(RTM_FLOAT64_HI_MASK_FRAC >> k)));
        n = ((n & RTM_FLOAT64_HI_MASK_FRAC) | 0x00100000) >> (20 - k);
        if(j < 0)
        {
            n = -n;
        }
        p_h -= t;
    }

    t = p_l + p_h;

    rtmSetLowf64(&t , 0);

    u = t*lg2_h;
    v = (p_l - (t - p_h)) * lg2 + t*lg2_l;
    z = u + v;
    w = v - (z - u);
    t = z * z;
    t1 = z - t * (P1 + t * (P2 + t * (P3 + t * (P4 + t * P5))));
    r = (z * t1) / (t1 - RTM_CONSTANT_TWO)-(w + z * w);
    z = RTM_CONSTANT_ONE - (r - z);
    j = rtmGetHighf64(z);
    j += (n << 20);

    if((j >> 20) <= 0)
    {
        /* subnormal output */
        z = rtmScalbnf64(z , n);
    }
    else
    {
        rtmSetHighf64(&z , rtmGetHighf64(z) + (n << 20));
    }

    return s * z;
}

/*! @} */
