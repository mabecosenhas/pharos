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


#ifndef PHAROS_TIMEINLINE_H
#define PHAROS_TIMEINLINE_H

#include <pharos/kernel/time/time.h>



INLINE bool pharosITimeoutIsFinite(Timeout timeout)
{
    /* return value of the function */
    bool result;
    
    
    /* check if timeout is zero or infinite */
    if((timeout == PHAROS_TIMEOUT_ZERO) || (timeout == PHAROS_TIMEOUT_INFINITE))
    {
        /* so it is not a number */
        result = FALSE;
    }
    else
    {
        /* it is a number */
        result = TRUE;
    }
    
    /* return the result */
    return result;
}


#endif /* PHAROS_TIMEINLINE_H */
