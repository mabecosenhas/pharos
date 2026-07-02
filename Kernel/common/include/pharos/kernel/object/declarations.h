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


#ifndef PHAROS_OBJECT_DECLARATIONS_H
#define PHAROS_OBJECT_DECLARATIONS_H

#include <pharos/hal/hal.h>




#define PHAROS_OBJECT_NUMBER_INVALID        (0xffffffffU)



typedef struct LightWeightQueueTable LightWeightQueueTable , *ptrLightWeightQueueTable;

typedef struct HeavyWeightQueueTable HeavyWeightQueueTable , *ptrHeavyWeightQueueTable;

typedef struct ThreadTable ThreadTable , *ptrThreadTable;

typedef struct BulkQueueTable BulkQueueTable , *ptrBulkTable;

typedef struct ObjectId ObjectId VARIABLE_ALIGNMENT_X(8) , *ptrObjectId;

typedef uint32_t ObjectNumber , *ptrObjectNumber;

typedef uint8_t KernelObjectNumber , *ptrKernelObjectNumber;

typedef struct ObjectTable ObjectTable , *ptrObjectTable;


/****************************************************************************************************************/
/* The following functions where placed inside Pharos so that Pharos does not have external dependencies        */
/* (on RTC library, for example)                                                                                */
/****************************************************************************************************************/


/**
 * Copies a memory area from src to dest. 
 * 
 * This function allows that the dest and src arrays may overlap.
 * 
 * @param dest destination address
 * @param src source address
 * @param n number of bytes to copy 
 * 
 * @return returns the original destination (dest)
 */
void *memmove(void *dest , const void *src , size_t n);


/**
 * copies the content of the source to the destination for the specified number of bytes.
 * 
 * This destination and source arrays must NOT overlap.
 * 
 * @param dest destination address
 * @param src source address
 * @param size number of bytes to copy
 */
void *memcpy(void *dest , const void *src , size_t size);


/**
 * Compares the content of two addresses for the specified number of bytes
 * 
 * @param a first address to compare
 * @param b second address to compare
 * @param size number of bytes to compare on each address
 * 
 * @return returns 1 if the content at memory a is larger than b. Returns -1 if the content of a is smaller than b.
 * Returns 0 if both contents are equal
 */
int memcmp(const void *a , const void *b , size_t size);


/**
 * Calculate the size of a string with a maximum size
 * 
 * @param string string to determine the size of
 * @param maxSize maximum size (in bytes)
 * 
 * @return returns the length of the string of maxSize if the string is larger than maxSize
 */
size_t strnlen(const char *string , size_t maxSize);


/**
 * Fills the array with the specified byte for the given amount of bytes
 * 
 * @param s array to fill
 * @param c byte to place
 * @param n number of bytes to fill the array
 * 
 * @return returns the pointer to s
 */
void *memset(void *s , int c , size_t n);


/**
 * Copy the memory from the source array to the destination array for n bytes
 * 
 * The memory areas may overleap
 * 
 * This function is similar to C Standard Library "memmove" function, but does not return anything.
 * 
 * @param dest destination array
 * @param src source array 
 * @param n number of bytes to copy
 */
void rtcMemMove8(void *dest , const void *src , size_t n);


/**
 * Copy the memory from the source array to the destination array, with at most n bytes, in reverse order
 * 
 * @param dest destination array
 * @param src source array
 * @param n number of bytes to copy
 */
void rtcMemCopyReverse8(void *dest , const void *src , size_t n);


#endif /* DECLARATIONS_H */
