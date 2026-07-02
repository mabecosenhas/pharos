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


/*! \addtogroup PHAROS-RTM
 * 
 * @{
 */

#ifndef RTM_INLINE_H
#define RTM_INLINE_H

#include <pharos/rtm/rtmdefs.h>


INLINE int32_t rtmGetHighf64(float64_t x)
{
    /* create a temporary float64 structure */
    RtmFloat64 f;


    /* assign the double */
    f.x = x;

    /* return the high bits */
    return f.bytes.high;
}


INLINE int32_t rtmGetLowf64(float64_t x)
{
    /* create a temporary float64 structure */
    RtmFloat64 f;


    /* assign the double */
    f.x = x;

    /* return the low bits */
    return f.bytes.low;
}


INLINE void rtmGetBitsf64(float64_t x , int32_t *high , int32_t *low)
{
    /* create a temporary float64 structure */
    RtmFloat64 f;


    /* assign the double */
    f.x = x;

    /* copy the high bits */
    *high = f.bytes.high;

    /* copy the low bits */
    *low = f.bytes.low;
}


INLINE void rtmSetHighf64(float64_t *x , int32_t h)
{
    /* create a temporary float64 structure */
    RtmFloat64 f;


    /* assign the double */
    f.x = *x;

    /* change the bits */
    f.bytes.high = h;

    /* update x */
    *x = f.x;
}


INLINE void rtmSetLowf64(float64_t *x , int32_t l)
{
    /* create a temporary float64 structure */
    RtmFloat64 f;


    /* assign the double */
    f.x = *x;

    /* change the bits */
    f.bytes.low = l;

    /* update x */
    *x = f.x;
}


INLINE void rtmSetHighLowf64(float64_t *x , int32_t h , int32_t l)
{
    /* create a temporary float64 structure */
    RtmFloat64 f;


    /* change the high bits */
    f.bytes.high = h;

    /* change the low bits */
    f.bytes.low = l;

    /* update x */
    *x = f.x;
}


INLINE int32_t rtmHighAbsf64(float64_t x)
{
    /* get the high word */
    int32_t hx = rtmGetHighf64(x);


    /* remove the sign (make the abs) and return it */
    return hx & RTM_FLOAT64_HI_UNMASK_SIGN;
}


INLINE int32_t rtmIsFinitef64(float64_t x)
{
    /* get the high word */
    int32_t hx = rtmGetHighf64(x);


    /* first remove the sign bit, then subtract the mask of the exponent. 
     * If hx has all mask bits set then the subtraction will give 0 and 1 otherwise */
    return(int32_t) ((hx & RTM_FLOAT64_HI_UNMASK_SIGN) - RTM_FLOAT64_HI_MASK_EXPONENT) >> 31;
}


INLINE int32_t rtmInternalIlogbSmallf64(int32_t hx , int32_t lx)
{
    int32_t ix;


    if(hx == 0)
    {
        for(ix = -1043; lx > 0; lx <<= 1)
        {
            ix -= 1;
        }
    }
    else
    {
        for(ix = -1022 , hx <<= 11; hx > 0; hx <<= 1)
        {
            ix -= 1;
        }
    }

    return ix;
}


INLINE int32_t rtmInternalIlogbf64(int32_t hx , int32_t lx)
{
    int32_t ix;


    if(hx < 0x00100000)
    {
        ix = rtmInternalIlogbSmallf64(hx , lx);
    }
    else
    {
        ix = (hx >> 20) - 1023;
    }

    return ix;
}


INLINE int32_t rtmIsInfinitef64(float64_t x)
{
    /* high part (32 bits) of x */
    int32_t hx;

    /* low part (32 bits) of x */
    int32_t lx;


    /* get the high part of |x| */
    hx = (rtmGetHighf64(x) & RTM_FLOAT64_HI_UNMASK_SIGN);

    /* get the low part of x */
    lx = rtmGetLowf64(x);

    /* subtract the exponent part */
    hx -= RTM_FLOAT64_HI_MASK_EXPONENT;

    /* if the exponent is != 7fff or the fraction is different than 0 then return 0 (not infinite) */
    return !(hx | lx);
}


INLINE int32_t rtmIsNanf64(float64_t x)
{
    /* high part (32 bits) of x */
    int32_t hx;

    /* low part (32 bits) of x */
    int32_t lx;


    /* get the high part of |x| */
    hx = (rtmGetHighf64(x) & RTM_FLOAT64_HI_UNMASK_SIGN);

    /* get the low part of x */
    lx = rtmGetLowf64(x);

    /* x is NaN if the fraction of x is != 0 and the exponent is 0x7ff */

    hx |= (uint32_t) (lx | (-lx)) >> 31;
    hx = RTM_FLOAT64_HI_MASK_EXPONENT - hx;

    return((uint32_t) (hx)) >> 31;
}


INLINE int32_t rtmIsZerof64(float64_t x)
{
    /* high part (32 bits) of x */
    int32_t hx;

    /* low part (32 bits) of x */
    int32_t lx;


    /* get the high part of |x| */
    hx = (rtmGetHighf64(x)) & RTM_FLOAT64_HI_UNMASK_SIGN;

    /* get the low part of x */
    lx = rtmGetLowf64(x);

    /* if x is 0 then return != 0, if x is not 0, then return 0 */
    return !(hx | lx);
}


INLINE float64_t rtmCopySignf64(float64_t x , float64_t y)
{
    /* remove the sign of x and place the sign of y on the high word of x */
    int32_t hx = rtmGetHighf64(x);
    int32_t hy = rtmGetHighf64(y);

    rtmSetHighf64(&x , (hx & RTM_FLOAT64_HI_UNMASK_SIGN) | (hy & RTM_FLOAT64_HI_MASK_SIGN));

    /* x has been updated with the sign of y */
    return x;
}


/* RTM_INLINE_H */
#endif 

/*! @} */
