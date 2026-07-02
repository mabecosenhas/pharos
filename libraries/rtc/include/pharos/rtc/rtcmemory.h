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


#ifndef PHAROS_RTC_MEMORY_H
#define PHAROS_RTC_MEMORY_H

#include <pharos/kernel/object/declarations.h>


/**
 * Copies the specified byte to the memory location for the desired number of bytes.
 * 
 * This function is similar to C Standard Library "memset" function, but takes an "uint8_t" instead of an "int" type.
 * 
 * @param m array where to place the byte
 * @param c byte to place
 * @param n number of bytes to copy
 */
void rtcMemSet8(void *m , uint8_t c , size_t n);


/**
 * Zeros the specified array
 * 
 * This function is similar to C Standard Library "bzero" function
 * 
 * @param m array where to place the byte
 * @param n number of bytes to zero-out
 */
INLINE void rtcMemZeros(void *m , size_t n);


/**
 * Searches in array for the specified byte until the maximum length of the array.
 * 
 * This function is similar to C Standard Library "memchr" function, but takes an "uint8_t" instead of an "int" type.
 *
 * @param array array to search in
 * @param c byte to search for
 * @param length maximum number of bytes to search for in the array
 * 
 * @return returns the position found in the array or NULL if the byte was not found.
 */
void *rtcMemSearch8(const void *array , uint8_t c , size_t length);


/**
 * Searches in array for the specified byte until the maximum length of the array, in reverse order.
 * 
 * This function is similar to C Standard Library "memrchr" function, but takes an "uint8_t" instead of an "int" type.
 *
 * @param array array to search in
 * @param c byte to search for
 * @param length maximum number of bytes to search for in the array
 * 
 * @return returns the position found in the array or NULL if the byte was not found.
 */
void *rtcMemSearchReverse8(const void *array , uint8_t c , size_t length);


/**
 * Reads from the same source position ("src") the number of times specified in "times"
 * 
 * @param dest destination array
 * @param src source address (is not incremented)
 * @param times number of times to read. The amount of bytes read is "times * 4"
 * 
 * @return returns dest + times
 */
uint32_t *rtcMemReadFixed32(uint32_t *dest , volatile uint32_t *src , size_t times);


/**
 * Reads from the same source position ("src") the number of times specified in "times"
 * 
 * @param dest destination array
 * @param src source address (is not incremented)
 * @param times number of times to read. The amount of bytes read is "times * 2"
 * 
 * @return returns dest + times
 */
uint16_t * rtcMemReadFixed16(uint16_t *dest , volatile uint16_t *src , size_t times);


/**
 * Reads from the same source position ("src") the number of times specified in "times"
 * 
 * @param dest destination array
 * @param src source address (is not incremented)
 * @param times number of times to read. The amount of bytes read is "times"
 * 
 * @return returns dest + times
 */
uint8_t *rtcMemReadFixed8(uint8_t *dest , volatile uint8_t *src , size_t times);


/**
 * Copy the memory from the source array to the destination array, with at most n bytes
 * 
 * This function is similar to C Standard Library "memcpy" function, but does not return anything
 * 
 * @param dest destination array
 * @param src source array
 * @param n number of bytes to copy
 */
INLINE void rtcMemCopyArray(void *dest , const void *src , size_t n);


/**
 * swaps the bytes of the 2-byte number
 * 
 * @param number 2-byte number
 * 
 * @return returns the bytes swapped
 */
INLINE uint16_t rtcMemSwap16(uint16_t number);


/**
 * Compares the content of two arrays for the specified number of bytes
 * 
 * This function is similar to C Standard Library "memcmp" function, but does returns an "intptr_t" instead of an "int"
 * 
 * @param a first address to compare
 * @param b second address to compare
 * @param size number of bytes to compare on each address
 * 
 * @return returns bigger than 0 if the content at memory a is larger than b. 
 * Returns smaller than 0 if the content of a is smaller than b.
 * Returns 0 if both contents are equal
 */
INLINE intptr_t rtcMemCompareArray(void *dest , const void *src , size_t n);


/**
 * Compares the content of two addresses for the specified number of bytes and checks if they are equal.
 * Similar to  rtcMemCompareArray but returns a bool
 * 
 * @param a first address to compare
 * @param b second address to compare
 * @param size number of bytes to compare on each address
 * 
 * @return returns TRUE if they are equal and FALSE otherwise
 */
INLINE bool rtcMemCompareEqualArray(void *s1 , const void *s2 , size_t n);


/**
 * Compares two unsigned 16-bit variables
 * 
 * @param a first value
 * @param b second value 
 * 
 * @return returns bigger than 0 if the a > b, returns 0 if a==b and returns smaller than 0 if b>a
 */
INLINE intptr_t rtcMemCompareU16(uint16_t *a , uint16_t *b);


/**
 * Compares two unsigned 32-bit variables
 * 
 * @param a first value
 * @param b second value 
 * 
 * @return returns bigger than 0 if the a > b, returns 0 if a==b and returns smaller than 0 if b>a
 */
INLINE intptr_t rtcMemCompareU32(uint32_t *a , uint32_t *b);


/**
 * Compares two unsigned 64-bit variables
 * 
 * @param a first value
 * @param b second value 
 * 
 * @return returns bigger than 0 if the a > b, returns 0 if a==b and returns smaller than 0 if b>a
 */
INLINE intptr_t rtcMemCompareU64(uint64_t *a , uint64_t *b);


/**
 * Compares two intptr_t variables
 * 
 * @param a first value
 * @param b second value 
 * 
 * @return returns 1 if the a > b, returns 0 if a==b and returns -1 if b>a
 */
INLINE intptr_t rtcMemCompareIptr(intptr_t a , intptr_t b);


INLINE void rtcMemZeros(void *m , size_t n)
{
    /* use the rtcMemSet8 function to zero-out the array */
    rtcMemSet8(m , 0 , n);
}


INLINE void rtcMemCopyArray(void *dest , const void *src , size_t n)
{
    /* use memcpy */
    memcpy(dest , src , n);
}


INLINE intptr_t rtcMemCompareArray(void *s1 , const void *s2 , size_t n)
{
    /* use memcmp */
    return memcmp(s1 , s2 , n);
}


INLINE bool rtcMemCompareEqualArray(void *s1 , const void *s2 , size_t n)
{
    /* use memcmp */
    return memcmp(s1 , s2 , n) == 0 ? TRUE : FALSE;
}


INLINE intptr_t rtcMemCompareU16(uint16_t *a , uint16_t *b)
{
    /* convert to larger signed integer and return the difference */
    return(intptr_t) *a - (intptr_t) * b;
}


INLINE intptr_t rtcMemCompareU32(uint32_t *a , uint32_t *b)
{
    /* convert to larger signed integer and return the difference */
    return(int64_t) *a - (int64_t) * b;
}


INLINE intptr_t rtcMemCompareU64(uint64_t *a , uint64_t *b)
{
    /* compare the numbers */
    return COMPARE(*a , *b);
}


INLINE uint16_t rtcMemSwap16(uint16_t number)
{
    /* swap the number */
    return __builtin_bswap16(number);
}


INLINE intptr_t rtcMemCompareIptr(intptr_t a , intptr_t b)
{
    /* compare the numbers */
    return COMPARE(a , b);
}

#endif /* PHAROS_RTC_MEMORY_H */
