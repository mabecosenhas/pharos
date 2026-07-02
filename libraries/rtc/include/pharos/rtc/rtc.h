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


/*! \defgroup PHAROS-RTC PHAROS-RTC
 *
 * General purpose library RTC, aimed to replace the C Standard Library (e.g. C90) in embedded systems that do not have
 * the full capabilities of a "fully capable" Operating System (e.g. filesystem, dynamic memory allocation, etc).
 * 
 * The library provides some of the most commonly used functions (e.g. rtcStringCompare) that are useful in 
 * embedded applications. Some functions (e.g rtcStringCompareN) use the standard interface while others 
 * (e.g. rtcStringSearchReverse) have changed their prototype slightly to reflect the proper way of 
 * implementing the function.
 * 
 * To keep compatability with previous existing libraries (e.g. C Standard Library), the RTC library has all of its 
 * functions begin with an "rtc" prefix, with the notable exception of printk (which is not inside the C Standard 
 * Library so there is no conflict).
 * 
 * Although RTC uses the interface of some commonly used functions, this library implementation is not based on any 
 * existing source code. In fact, many functions differs from their C standard library counter-part. For 
 * example, rtcStringConcatN differs from strncat in the sense that it does not return the pointer to the original 
 * destination string. This makes it usable to concatenate several strings. Nor does it add the series of '\0' as 
 * stpncpy does. It is thus, much more efficient, especially for small strings with large "n".
 * 
 * All of the functions are thread-safe. None of them uses any TLS variables (Thread Local Storage).
 *
 * @{
 */

#ifndef PHAROS_RTC_H
#define PHAROS_RTC_H

#include <pharos/hal/hal.h>
#include <pharos/rtc/rtcsort.h>
#include <pharos/rtc/rtcmemory.h>
#include <pharos/rtc/rtcinternal.h>
#include <pharos/rtc/rtcstring.h>
#include <pharos/rtc/rtcendian.h>
#include <pharos/rtc/rtcio.h>
#include <pharos/rtc/rtcinline.h>


/* PHAROS_RTC_H */
#endif 

/*! @} */
