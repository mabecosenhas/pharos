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


#ifndef PHAROS_RTCINTERNAL_H
#define PHAROS_RTCINTERNAL_H

#include <pharos/rtc/rtcio.h>

typedef __builtin_va_list __gnuc_va_list;

typedef __gnuc_va_list va_list;

#define va_arg(v,l) __builtin_va_arg(v,l)

#define va_start(v,l) __builtin_va_start(v,l)

#define va_end(v) __builtin_va_end(v)


/**
 * Prints to the handler and/or to the array the string fmt with the format of the printk function
 * 
 * @param handler handler to print with (or NULL if none)
 * @param array array to print (or NULL if none)
 * @param fmt format of the string to print
 * @param ap variable argument list
 * 
 * @return returns the number of chars printed to the handler and/or to the array
 */
size_t rtcPrintkInternal(RtcOutputCharHandler handler , char *array , const char *fmt , va_list ap);


/**
 * Compares two strings that are unaligned (have to compare char by char) until the specified size
 * 
 * @param s1 first string to compare
 * @param s2 second string to compare
 * @param size maximum address of maximum number of bytes to compare
 * @param ended if strings are equal within the size or  different, the ended is set to TRUE (otherwise is FALSE)
 * 
 * @return return 0 if the strings are equal, smaller than 0 if s1 < s2 and larger than zero if s1 > s2
 */
intptr_t rtcStringCompareUnalignedSize(const char *s1 , const char *s2 , size_t size , bool *ended);


/* PHAROS_RTCINTERNAL_H */
#endif 
