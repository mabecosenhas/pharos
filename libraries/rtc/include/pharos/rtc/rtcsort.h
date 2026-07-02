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


#ifndef PHAROS_RTC_SORT_H
#define PHAROS_RTC_SORT_H


#include <pharos/rtc/rtc.h>


/**
 * Sorts the specified array (base) with "size" number of elements on the array. 
 * The "handler" function is able to compare two distinct elements of the 
 * array and sort them. The "arg" will be passed to the "handler" function. This could be used for any reason
 * of the "handler" function (or pass NULL if you don't need it).
 * 
 * The heap sort has the advantage of being O(N log(N)) in the best/average/worst case and using O(1) memory.
 * On the other hand, it is not stable (might change the order of elements with the same key) and does unnecessary 
 * swaps.
 * 
 * In order to use the compiler optimization procedures this method is implemented as a Macro. In short, by doing this
 * each time the array ("base") is accessed (see below, for example: &base[index]) the compiler calculates the 
 * position in memory of the index being accessed. If it was implemented as a inline function (or a regular function)
 * then this address would need to be calculated by software. This would not only increase the complexity of the 
 * function but fundamentally would loose the compilation optimizations for accessing, for example, arrays where each
 * element is an "int" or "double" (that is, elements whose size is a power of 2).
 * 
 * The prototype is as follows:
 * 
 * void RTC_HEAP_SORT(void *base , const size_t size , HeapSortCompareMethod handler , void *arg);
 * 
 * where the HeapSortCompareMethod should be the type:
 * 
 * typedef intptr_t(*HeapSortCompareMethod)(const void *a , const void *b , void *argument);
 * 
 * 
 * For example, sort the following array:
 * 
 * // array to sort 
 * uint32_t u32Array1[] = {10 , 20 , 15 , 17 , 9 , 21};
 * 
 * // function to compare the unsigned integer (does not use the argument). Since the function is so simple it can be 
 * // inline
 * INLINE intptr_t u32Compare(const uint32_t *a , const uint32_t *b , void *argument)
 * {
 *    return (intptr_t) (*a) - (intptr_t) (*b);
 * }
 * 
 * // this will sort the array
 * RTC_HEAP_SORT(u32Array1 , sizeof (u32Array1) / sizeof (uint32_t) , u32Compare , NULL);
 * 
 * @param base start of the array
 * @param size number of elements in the array
 * @param handler handler method to compare two elements
 * @param arg optional argument to pass to the handler function
 */
#define RTC_HEAP_SORT(base , size , handler , arg)                                                          \
    do {                                                                                                    \
        /* iterator */                                                                                      \
        intptr_t i , index , j;                                                                             \
                                                                                                            \
                                                                                                            \
        /* build the heap */                                                                                \
        RTC_HEAP_BUILD_MAX_HEAP(base , size , handler , arg);                                               \
                                                                                                            \
        /* for each element of the array (starting at the top) */                                           \
        for(i = size - 1; i > 0; i--)                                                                       \
        {                                                                                                   \
            /* swap value of first indexed with last indexed */                                             \
            RTC_HEAP_SWAP(base, 0 , i);                                                                     \
                                                                                                            \
            /* maintaining heap property after each swapping */                                             \
            j = 0;                                                                                          \
                                                                                                            \
            do                                                                                              \
            {                                                                                               \
                index = (2 * j + 1);                                                                        \
                                                                                                            \
                /* check if the left child is smaller than the right child */                               \
                if((index < i - 1) && (handler(&base[index] , &base[index + 1] , arg) < 0))                 \
                {                                                                                           \
                    /* make index point to the right child */                                               \
                    index++;                                                                                \
                }                                                                                           \
                                                                                                            \
                /* check if the parent is smaller than the child */                                         \
                if(((index < i) && handler(&base[j] , &base[index] , arg) < 0))                             \
                {                                                                                           \
                    /* if they are then swap them */                                                        \
                    RTC_HEAP_SWAP(base, j , index);                                                         \
                }                                                                                           \
                                                                                                            \
                j = index;                                                                                  \
                                                                                                            \
            } while(index < i);                                                                             \
        }                                                                                                   \
    } while(0);                                                                           


/**
 * Helper macro for the heap sort. This macro just swaps two elements of the array, indicated by "i" and "j" indexes
 */
#define RTC_HEAP_SWAP(_heapArray , i , j)                                                                   \
    do{                                                                                                     \
        /* store the index i to a temporary variable */                                                     \
        __typeof__(*_heapArray) temp = _heapArray[i];                                                       \
                                                                                                            \
                                                                                                            \
        /* swap the position at i */                                                                        \
        _heapArray[i] = _heapArray[j];                                                                      \
                                                                                                            \
        /* swap the position at j */                                                                        \
        _heapArray[j] = temp;                                                                               \
    }while(0);


/**
 * Helper macro for the heap sort. This macro builds the first iteration of the heap sort, commonly known as "heapify".
 */
#define RTC_HEAP_BUILD_MAX_HEAP(_heapArray , size , handler , arg)                                          \
    do{                                                                                                     \
        size_t i;                                                                                           \
        intptr_t j;                                                                                         \
        size_t _size = size;                                                                                \
                                                                                                            \
                                                                                                            \
        /* for every element on the array */                                                                \
        for(i = 1U; i < _size; i++)                                                                         \
        {                                                                                                   \
            /* if the child is bigger than the parent */                                                    \
            if(handler(&_heapArray[i] , &_heapArray[(i - 1) / 2] , arg) > 0L)                               \
            {                                                                                               \
                j = i;                                                                                      \
                                                                                                            \
                /* swap child and parent until parent is smaller */                                         \
                while(handler(&_heapArray[j] , &_heapArray[(j - 1) / 2] , arg) > 0L)                        \
                {                                                                                           \
                    /* swap them */                                                                         \
                    RTC_HEAP_SWAP(_heapArray , j , (j - 1) / 2);                                            \
                                                                                                            \
                    /* go to the parent */                                                                  \
                    j = (j - 1) / 2;                                                                        \
                                                                                                            \
                }                                                                                           \
            }                                                                                               \
        }                                                                                                   \
    }while(0);




#endif /* PHAROS_RTC_SORT_H */
