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


#ifndef PHAROS_PRIORITY_H
#define PHAROS_PRIORITY_H


#include <pharos/kernel/thread/declarations.h>



/**
 *@brief Highest priority 
 */
#define PHAROS_PRIORITY_HIGHEST 0U

/**
 *@brief Lowest priority
 */
#define PHAROS_PRIORITY_LOWEST  255U

typedef union
{
    uint64_t map64[NUMBER_BIT_MAPS];
    uint32_t map32[2 * NUMBER_BIT_MAPS];
    uint32_t map16[4 * NUMBER_BIT_MAPS];
    uint32_t map8[8 * NUMBER_BIT_MAPS];
} PriorityBitMap STRUCTURE_FIELD_ALIGNMENT_8 , *ptrPriorityBitMap;


/**
 *@brief thread priority from 0 (highest priority) to 255 (lowest priority)
 */
typedef uint8_t ThreadPriority , *ptrThreadPriority;


/**
 * compares priority of a and b
 * 
 * @param a
 * @param b
 * @return returns 0 if they are equal, 1 is a is higher priority than b (but lower in value) and -1 otherwise
 */
intptr_t pharosIThreadPriorityComparator(const ptrThreadPriority a , const ptrThreadPriority b);


/**
 * Initialize the priority bit map
 * 
 * @param bitMap priority bit map to initialize
 */
void pharosIPriorityBitMapInit(ptrPriorityBitMap bitMap);


#endif /* PRIORITY_H */
