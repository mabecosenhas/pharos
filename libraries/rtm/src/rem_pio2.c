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


/*
 * Table of constants for 2/pi, 396 Hex digits (476 decimal) of 2/pi 
 */
static const int32_t rtmTwoOverPi[] = {

    0xA2F983 , 0x6E4E44 , 0x1529FC , 0x2757D1 , 0xF534DD , 0xC0DB62 ,
    0x95993C , 0x439041 , 0xFE5163 , 0xABDEBB , 0xC561B7 , 0x246E3A ,
    0x424DD2 , 0xE00649 , 0x2EEA09 , 0xD1921C , 0xFE1DEB , 0x1CB129 ,
    0xA73EE8 , 0x8235F5 , 0x2EBB44 , 0x84E99C , 0x7026B4 , 0x5F7E41 ,
    0x3991D6 , 0x398353 , 0x39F49C , 0x845F8B , 0xBDF928 , 0x3B1FF8 ,
    0x97FFDE , 0x05980F , 0xEF2F11 , 0x8B5A0A , 0x6D1F6D , 0x367ECF ,
    0x27CB09 , 0xB74F46 , 0x3F669E , 0x5FEA2D , 0x7527BA , 0xC7EBE5 ,
    0xF17B3D , 0x0739F7 , 0x8A5292 , 0xEA6BFB , 0x5FB11F , 0x8D5D08 ,
    0x560330 , 0x46FC7B , 0x6BABF0 , 0xCFBC20 , 0x9AF436 , 0x1DA9E3 ,
    0x91615E , 0xE61B08 , 0x659985 , 0x5F14A0 , 0x68408D , 0xFFD880 ,
    0x4D7327 , 0x310606 , 0x1556CA , 0x73A8C9 , 0x60E27B , 0xC08C6B ,
};


PHAROS_WEAK int32_t rtmRemPio2f64(float64_t x , float64_t *y)
{
    /* The method is to compute the integer (mod 8) and fraction parts of 
     * (2/pi)*x without doing the full multiplication. In general we
     * skip the part of the product that are known to be a huge integer (
     * more accurately, = 0 mod 8 ). Thus the number of operations are
     * independent of the exponent of the input.
     *
     * (2/pi) is represented by an array of 24-bit integers in ipio2[].
     *
     * Input parameters:
     * 	x[]	The input value (must be positive) is broken into nx 
     *		pieces of 24-bit integers in float64_t precision format.
     *		x[i] will be the i-th 24 bit of x. The scaled exponent 
     *		of x[0] is given in input parameter e0 (i.e., x[0]*2^e0 
     *		match x's up to 24 bits.
     *
     *		Example of breaking a float64_t positive z into x[0]+x[1]+x[2]:
     *			e0 = ilogb(z)-23
     *			z  = scalbn(z,-e0)
     *		for i = 0,1,2
     *			x[i] = floor(z)
     *			z    = (z-x[i])*2**24
     *
     *
     *	y[]	ouput result in an array of float64_t precision numbers.
     *		The dimension of y[] is:
     *			24-bit  precision	1
     *			53-bit  precision	2
     *			64-bit  precision	2
     *			113-bit precision	3
     *		The actual value is the sum of them. Thus for 113-bit
     *		precison, one may have to do something like:
     *
     *		long float64_t t,w,r_head, r_tail;
     *		t = (long float64_t)y[2] + (long float64_t)y[1];
     *		w = (long float64_t)y[0];
     *		r_head = t+w;
     *		r_tail = w - (r_head - t);
     *
     *	e0	The exponent of x[0]
     *
     *	nx	dimension of x[]
     *
     *  	prec	an integer indicating the precision:
     *			0	24  bits (single)
     *			1	53  bits (float64_t)
     *			2	64  bits (extended)
     *			3	113 bits (quad)
     *
     *	ipio2[]
     *		integer array, contains the (24*i)-th to (24*i+23)-th 
     *		bit of 2/pi after binary point. The corresponding 
     *		floating value is
     *
     *			ipio2[i] * 2^(-24(i+1)).
     *
     * External function:
     *	float64_t scalbn(), floor();
     *
     *
     * Here is the description of some local variables:
     *
     * 	jk	jk+1 is the initial number of terms of ipio2[] needed
     *		in the computation. The recommended value is 2,3,4,
     *		6 for single, float64_t, extended,and quad.
     *
     * 	jz	local integer variable indicating the number of 
     *		terms of ipio2[] used. 
     *
     *	jx	nx - 1
     *
     *	jv	index for pointing to the suitable ipio2[] for the
     *		computation. In general, we want
     *			( 2^e0*x[0] * ipio2[jv-1]*2^(-24jv) )/8
     *		is an integer. Thus
     *			e0-3-24*jv >= 0 or (e0-3)/24 >= jv
     *		Hence jv = max(0,(e0-3)/24).
     *
     *	jp	jp+1 is the number of terms in PIo2[] needed, jp = jk.
     *
     * 	q[]	float64_t array with integral value, representing the
     *		24-bits chunk of the product of x and 2/pi.
     *
     *	q0	the corresponding exponent of q[0]. Note that the
     *		exponent for q[i] would be q0-24*i.
     *
     *	PIo2[]	float64_t precision array, obtained by cutting pi/2
     *		into 24 bits chunks. 
     *
     *	f[]	ipio2[] in floating point 
     *
     *	iq[]	integer array by breaking up q[] in 24-bits chunk.
     *
     *	fq[]	final product of x*(2/pi) in fq[0],..,fq[jk]
     *
     *	ih	integer. If >0 it indicates q[] is >= 0.5, hence
     *		it also indicates the *sign* of the result.
     *
     */
    static const int32_t npio2_hw[] = {

        0x3FF921FB , 0x400921FB , 0x4012D97C , 0x401921FB , 0x401F6A7A , 0x4022D97C ,
        0x4025FDBB , 0x402921FB , 0x402C463A , 0x402F6A7A , 0x4031475C , 0x4032D97C ,
        0x40346B9C , 0x4035FDBB , 0x40378FDB , 0x403921FB , 0x403AB41B , 0x403C463A ,
        0x403DD85A , 0x403F6A7A , 0x40407E4C , 0x4041475C , 0x4042106C , 0x4042D97C ,
        0x4043A28C , 0x40446B9C , 0x404534AC , 0x4045FDBB , 0x4046C6CB , 0x40478FDB ,
        0x404858EB , 0x404921FB ,
    };

    /* 53 bits of 2/pi (0x3FE45F30, 0x6DC9C883) */
    static const float64_t invpio2 = RTM_CONSTANT_2_DIV_PI;

    /* first 33 bit of pi/2 (0x3FF921FB, 0x54400000) */
    static const float64_t pio2_1 = 1.57079632673412561417e+00;

    /* pi/2 - pio2_1 (0x3DD0B461, 0x1A626331) */
    static const float64_t pio2_1t = 6.07710050650619224932e-11;

    /* second 33 bit of pi/2 (0x3DD0B461, 0x1A600000) */
    static const float64_t pio2_2 = 6.07710050630396597660e-11;

    /* pi/2 - (pio2_1+pio2_2) (0x3BA3198A, 0x2E037073) */
    static const float64_t pio2_2t = 2.02226624879595063154e-21;

    /* third  33 bit of pi/2 (0x3BA3198A, 0x2E000000) */
    static const float64_t pio2_3 = 2.02226624871116645580e-21;

    /* pi/2 - (pio2_1+pio2_2+pio2_3) (0x397B839A, 0x252049C1) */
    static const float64_t pio2_3t = 8.47842766036889956997e-32;


    float64_t z , w , t , r , fn;
    float64_t tx[3];
    int32_t e0 , i , j , nx , n , ix , hx;

    /* result of this function */
    int32_t result;


    /* high word of x */
    hx = rtmGetHighf64(x);

    /* high word of |x| */
    ix = hx & RTM_FLOAT64_HI_UNMASK_SIGN;

    /* if |x| ~<= pi/4 */
    if(ix <= 0x3fe921fb)
    {
        /* no need for reduction */
        y[0] = x;
        y[1] = 0;

        result = 0;
    }
        /* if |x| < 3pi/4 */
    else if(ix < 0x4002d97c)
    {
        /* special case with n=+-1 */

        /* if 3pi/4 > x > 0 */
        if(hx > 0)
        {
            z = x - pio2_1;

            /* check if x is not close to pi/2 */
            if(ix != 0x3ff921fb)
            {
                /* is not close to pi/2, so 33+53 bit pi is good enough */
                y[0] = z - pio2_1t;
                y[1] = (z - y[0]) - pio2_1t;
            }
                /* else, it is near pi/2 */
            else
            {
                /* near pi/2, use 33+33+53 bit pi */
                z -= pio2_2;
                y[0] = z - pio2_2t;
                y[1] = (z - y[0]) - pio2_2t;
            }

            result = 1;
        }
            /* else, -3pi/4 < x < 0 */
        else
        {
            /* negative x */
            z = x + pio2_1;

            if(ix != 0x3ff921fb)
            {
                /* 33+53 bit pi is good enough */
                y[0] = z + pio2_1t;
                y[1] = (z - y[0]) + pio2_1t;
            }
            else
            {
                /* near pi/2, use 33+33+53 bit pi */
                z += pio2_2;
                y[0] = z + pio2_2t;
                y[1] = (z - y[0]) + pio2_2t;
            }

            result = -1;
        }
    }
        /* |x| ~<= 2^19*(pi/2), medium size */
    else if(ix <= 0x413921fb)
    {
        /* calculate |x| */
        t = rtmAbsf64(x);

        /* calculate integer part |x| / (pi/2) + 0.5 */
        n = (int32_t) (t * invpio2 + RTM_CONSTANT_HALF);

        /* convert it to float */
        fn = (float64_t) n;

        /* calculate the difference between the original float and the integer approximation split by pi/2 */
        r = t - fn*pio2_1;

        /* 1st round good to 85 bit */
        w = fn*pio2_1t;

        if(n < 32 && ix != npio2_hw[n - 1])
        {
            /* quick check no cancellation */
            y[0] = r - w;
        }
        else
        {
            j = ix >> 20;
            y[0] = r - w;
            i = j - (((rtmGetHighf64(y[0])) >> 20)&0x7ff);

            if(i > 16)
            {
                /* 2nd iteration needed, good to 118 */
                t = r;
                w = fn*pio2_2;
                r = t - w;
                w = fn * pio2_2t - ((t - r) - w);
                y[0] = r - w;
                i = j - (((rtmGetHighf64(y[0])) >> 20)&0x7ff);
                if(i > 49)
                {
                    /* 3rd iteration need, 151 bits acc */

                    /* will cover all possible cases */
                    t = r;
                    w = fn*pio2_3;
                    r = t - w;
                    w = fn * pio2_3t - ((t - r) - w);
                    y[0] = r - w;
                }
            }
        }

        y[1] = (r - y[0]) - w;

        if(hx < 0)
        {
            y[0] = -y[0];
            y[1] = -y[1];

            result = -n;
        }
        else
        {
            result = n;
        }
    }
        /* x is inf or NaN */
    else if(ix >= RTM_FLOAT64_HI_MASK_EXPONENT)
    {
        y[0] = x - x;
        y[1] = y[0];

        result = 0;
    }
        /* all other (large) arguments */
    else
    {
        /* e0 = ilogb(z)-23; */
        e0 = (ix >> 20) - 1046;

        /* set z = scalbn(|x|,ilogb(x)-23) */
        rtmSetHighLowf64(&z , ix - (e0 << 20) , rtmGetLowf64(x));

        for(i = 0; i < 2; i++)
        {
            tx[i] = (float64_t) ((int32_t) (z));
            z = (z - tx[i]) * RTM_CONSTANT_POWER_2_24;
        }

        tx[2] = z;
        nx = 3;

        /* skip zero term */
        while(tx[nx - 1] == RTM_CONSTANT_ZERO)
        {
            nx--;
        }

        n = rtmKernelRemPio2f64(tx , y , e0 , nx);

        if(hx < 0)
        {
            y[0] = -y[0];
            y[1] = -y[1];

            result = -n;
        }
        else
        {
            result = n;
        }
    }

    return result;
}


PHAROS_WEAK int32_t rtmKernelRemPio2f64(float64_t *x , float64_t *y , int32_t e0 , int32_t nx)
{
    /* Constants:
     * The hexadecimal values are the intended ones for the following 
     * constants. The decimal values may be used, provided that the 
     * compiler will convert from decimal to binary accurately enough 
     * to produce the hexadecimal values shown.
     */

    static const float64_t PIo2[] = {

        1.57079625129699707031e+00 , /* 0x3FF921FB, 0x40000000 */
        7.54978941586159635335e-08 , /* 0x3E74442D, 0x00000000 */
        5.39030252995776476554e-15 , /* 0x3CF84698, 0x80000000 */
        3.28200341580791294123e-22 , /* 0x3B78CC51, 0x60000000 */
        1.27065575308067607349e-29 , /* 0x39F01B83, 0x80000000 */
        1.22933308981111328932e-36 , /* 0x387A2520, 0x40000000 */
        2.73370053816464559624e-44 , /* 0x36E38222, 0x80000000 */
        2.16741683877804819444e-51 , /* 0x3569F31D, 0x00000000 */
    };


    int32_t jz , jx , jv , jp , jk , carry , n , iq[20] , i , j , k , m , q0 , ih;
    float64_t z , fw , f[20] , fq[20] , q[20];

    /* initialize jk */
    jk = 4;
    jp = jk;

    /* determine jx,jv,q0, note that 3>q0 */
    jx = nx - 1;
    jv = (e0 - 3) / 24;

    if(jv < 0)
    {
        jv = 0;
    }

    q0 = e0 - 24 * (jv + 1);

    /* set up f[0] to f[jx+jk] where f[jx+jk] = ipio2[jv+jk] */
    j = jv - jx;
    m = jx + jk;

    for(i = 0; i <= m; i++ , j++)
    {
        f[i] = (j < 0) ? RTM_CONSTANT_ZERO : (float64_t) rtmTwoOverPi[j];
    }

    /* compute q[0],q[1],...q[jk] */
    for(i = 0; i <= jk; i++)
    {
        for(j = 0 , fw = 0.0; j <= jx; j++)
        {
            fw += x[j] * f[jx + i - j];
        }

        q[i] = fw;
    }

    jz = jk;

    /* cycle until no more recomputations are needed */
    while(1)
    {
        /* distill q[] into iq[] reversingly */
        for(i = 0 , j = jz , z = q[jz]; j > 0; i++ , j--)
        {
            fw = (float64_t) ((int32_t) (RTM_CONSTANT_POWER_2_M24 * z));
            iq[i] = (int32_t) (z - RTM_CONSTANT_POWER_2_24 * fw);
            z = q[j - 1] + fw;
        }

        /* compute n */

        /* actual value of z */
        z = rtmScalbnf64(z , q0);

        /* trim off integer >= 8 */
        z -= 8.0 * rtmFloorf64(z * 0.125);
        n = (int32_t) z;
        z -= (float64_t) n;
        ih = 0;

        if(q0 > 0)
        {
            /* need iq[jz-1] to determine n */
            i = (iq[jz - 1]>>(24 - q0));
            n += i;
            iq[jz - 1] -= i << (24 - q0);
            ih = iq[jz - 1]>>(23 - q0);
        }
        else if(q0 == 0)
        {
            ih = iq[jz - 1] >> 23;
        }
        else if(z >= RTM_CONSTANT_HALF)
        {
            ih = 2;
        }

        if(ih > 0)
        {
            /* q > 0.5 */
            n += 1;
            carry = 0;

            for(i = 0; i < jz; i++)
            {
                /* compute 1-q */
                j = iq[i];

                if(carry == 0)
                {
                    if(j != 0)
                    {
                        carry = 1;
                        iq[i] = 0x1000000 - j;
                    }
                }
                else
                {
                    iq[i] = 0xffffff - j;
                }
            }

            if(q0 > 0)
            {
                /* rare case: chance is 1 in 12 */
                switch(q0)
                {
                case 1:
                    iq[jz - 1] &= 0x7fffff;
                    break;
                case 2:
                    iq[jz - 1] &= 0x3fffff;
                    break;
                }
            }

            if(ih == 2)
            {
                z = RTM_CONSTANT_ONE - z;
                if(carry != 0)
                {
                    z -= rtmScalbnf64(RTM_CONSTANT_ONE , q0);
                }
            }
        }

        /* check if recomputation is needed */
        if(z == RTM_CONSTANT_ZERO)
        {
            j = 0;

            for(i = jz - 1; i >= jk; i--)
            {
                j |= iq[i];
            }

            if(j == 0)
            {
                /* need recomputation */

                /* k = no. of terms needed */
                for(k = 1; iq[jk - k] == 0; k++);

                for(i = jz + 1; i <= jz + k; i++)
                {
                    /* add q[jz+1] to q[jz+k] */
                    f[jx + i] = (float64_t) rtmTwoOverPi[jv + i];

                    for(j = 0 , fw = 0.0; j <= jx; j++)
                    {
                        fw += x[j] * f[jx + i - j];
                    }

                    q[i] = fw;
                }

                jz += k;

                /* recompute */
                continue;
            }
        }

        break;
    }

    /* chop off zero terms */
    if(z == 0.0)
    {
        jz -= 1;
        q0 -= 24;

        while(iq[jz] == 0)
        {
            jz--;
            q0 -= 24;
        }
    }
    else
    {
        /* break z into 24-bit if necessary */
        z = rtmScalbnf64(z , -q0);

        if(z >= RTM_CONSTANT_POWER_2_24)
        {
            fw = (float64_t) ((int32_t) (RTM_CONSTANT_POWER_2_M24 * z));
            iq[jz] = (int32_t) (z - RTM_CONSTANT_POWER_2_24 * fw);
            jz += 1;
            q0 += 24;
            iq[jz] = (int32_t) fw;
        }
        else
        {
            iq[jz] = (int32_t) z;
        }
    }

    /* convert integer "bit" chunk to floating-point value */
    fw = rtmScalbnf64(RTM_CONSTANT_ONE , q0);

    for(i = jz; i >= 0; i--)
    {
        q[i] = fw * (float64_t) iq[i];
        fw *= RTM_CONSTANT_POWER_2_M24;
    }

    /* compute PIo2[0,...,jp]*q[jz,...,0] */
    for(i = jz; i >= 0; i--)
    {
        for(fw = 0.0 , k = 0; k <= jp && k <= jz - i; k++)
        {
            fw += PIo2[k] * q[i + k];
        }

        fq[jz - i] = fw;
    }

    /* compress fq[] into y[] */


    fw = 0.0;

    for(i = jz; i >= 0; i--)
    {
        fw += fq[i];
    }

    y[0] = (ih == 0) ? fw : -fw;
    fw = fq[0] - fw;

    for(i = 1; i <= jz; i++)
    {
        fw += fq[i];
    }

    y[1] = (ih == 0) ? fw : -fw;

    return n & 7;
}

/*! @} */
