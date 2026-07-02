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


PHAROS_WEAK float64_t rtmAsinhf64(float64_t x)
{
    /* Method: Based on 
     *    asinh(x) = sign(x) * log [ |x| + sqrt(x*x+1) ]
     *
     * we have
     *    asinh(x) := x  if  1+x*x=1,
     *             := sign(x)*(log(x)+ln2)) for large |x|, else
     *             := sign(x)*log(2|x|+1/(|x|+sqrt(x*x+1))) if|x|>2, else
     *             := sign(x)*log1p(|x| + x^2/(1 + sqrt(1+x^2)))  
     */

    float64_t t , w;

    /* high word of x */
    int32_t hx;

    /* low word of x */
    int32_t ix;


    /* high word of x */
    hx = rtmGetHighf64(x);

    /* high word of |x| */
    ix = hx & RTM_FLOAT64_HI_UNMASK_SIGN;

    /* x is inf or NaN */
    if(ix >= RTM_FLOAT64_HI_MASK_EXPONENT)
    {
        return x + x;
    }

    /* if |x| < 2^-28 */
    if(ix < RTM_FLOAT64_2_EXP_M28)
    {
        /* return x inexact except 0 */
        return x;
    }

    /* if |x| > 2^28 */
    if(ix > 0x41b00000)
    {
        w = rtmLogf64(rtmAbsf64(x)) + RTM_CONSTANT_LN2;
    }
        /* if 2^28 > |x| > 2.0 */
    else if(ix > 0x40000000)
    {
        t = rtmAbsf64(x);
        w = rtmLogf64(2.0 * t + RTM_CONSTANT_ONE / (rtmSqrtf64(x * x + RTM_CONSTANT_ONE) + t));
    }
        /* if 2.0 > |x| > 2^-28 */
    else
    {
        t = x*x;
        w = rtmLog1f64(rtmAbsf64(x) + t / (RTM_CONSTANT_ONE + rtmSqrtf64(RTM_CONSTANT_ONE + t)));
    }

    if(hx > 0)
    {
        return w;
    }
    else
    {
        return -w;
    }
}

/*! @} */
