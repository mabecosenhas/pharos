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


#include <pharos/rtc/rtc.h>
#include <pharos/rtc/rtcinternal.h>
#include <pharos/declarations.h>


size_t rtcStringPrint(char *str , const char *format , ...)
{
    /* number of bytes written to the string */
    size_t result;
    
    /* variable argument list */
    va_list ap; 

    
    /* start the variable arguments */
    va_start(ap , format); 
    
    /* use the strprintk function */
    result = rtcPrintkInternal(NULL , str , format , ap);
    
    /* clean up the variable arguments */
    va_end(ap);
    
    /* return the number of bytes written */
    return result;
}
