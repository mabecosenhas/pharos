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


PHAROS_WEAK float64_t rtmScalbf64(float64_t x , float64_t fn)
{
    /* return value */
    float64_t result;


    /* if x or fn is NaN */
    if(rtmIsNanf64(x) || rtmIsNanf64(fn))
    {
        /* return NaN */
        result = x * fn;
    }
        /* if fn is not a number (that is, if fn is +-Inf) */
    else if(rtmIsFinitef64(fn) == 0)
    {
        /* if fn is +Inf */
        if(fn > 0.0)
        {
            /* return +-Inf (x can be a finite or +-Inf) */
            result = x * fn;
        }
            /* else, then fn is -Inf */
        else
        {
            /* return NaN if x is +-Inf, and +-0 if x is finite */
            result = x / (-fn);
        }
    }
        /* if fn is not an integer */
    else if(rtmRintf64(fn) != fn)
    {
        /* return NaN */
        result = (fn - fn) / (fn - fn);
    }
        /* if fn is too large */
    else if(fn > 65000.0)
    {
        /* limit the calculation */
        result = rtmScalbnf64(x , 65000);
    }
        /* if fn is too small */
    else if(-fn > 65000.0)
    {
        /* limit the calculation */
        result = rtmScalbnf64(x , -65000);
    }
        /* if fn is within normal parameters */
    else
    {
        /* calculate it */
        result = rtmScalbnf64(x , (int32_t) fn);
    }

    /* return the calculated value */
    return result;
}

/*! @} */
