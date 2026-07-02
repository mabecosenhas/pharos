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


#ifndef PHAROS_HAL_H
#define PHAROS_HAL_H

#include <pharos/hal/def.h>


#define PHAROS_SECTION(x)                   __attribute__ ((section (x))) 


/**
 * Default Read-only section 
 */
#define PHAROS_RODATA_SECTION               PHAROS_SECTION(".rodata")


/**
 * Defines the inline function prefix
 */
#define INLINE                              static __inline__

/**
 * The attribute to make a function implementation weak (when the same function is implemented twice, the implementation
 * with the weak attribute is discarded)
 */
#define PHAROS_WEAK                         __attribute__((weak)) 


#ifdef __MSVC__
#define FORCE_INLINE                        __forceinline
#else
#define FORCE_INLINE                        INLINE __attribute__((always_inline))
#endif


/**
 * Helpful macro to disable compiler optimizations to a function. 
 * Could be useful, for example, in a delay function that does an active wait for some time
 */
#define PHAROS_OPTIMIZE_O0                  __attribute__((optimize("-O0")))

/**
 * If the OFFSETOF macro is not defined then define it 
 * (should ignore if the tool that statically analyze the source code cannot handle the __builtin_offsetof function)
 */
#ifndef OFFSETOF
/**
 * define offsetof function as the builtin GCC offset function
 */
#define OFFSETOF(st, m)                     __builtin_offsetof( st, m )
#else
/**
 * Ignore the offsetof macro
 */
#define OFFSETOF(st,m)                      ( ((uint32_t)((char *)&((s *)0)->memb - (char *)0)) )
#endif


/**
 * Maximum number of supported partitions
 */
#define PHAROS_PARTITION_MAXIMUM_NUMBER     (64U)


/*****************************************
 * Some helpful math macros
 *****************************************/

/**
 * Helpful macro to determine the maximum of two numbers
 */
#define MAX(a,b)                    \
({                                  \
  __typeof__ (a) _a = (a);          \
  __typeof__ (b) _b = (b);          \
  _a > _b ? _a : _b;                \
})


/**
 * Helpful macro to determine the maximum of two numbers. Cannot be used by other Macros (use MAX_MACRO instead)
 */
#define MIN(a,b)                    \
({                                  \
  __typeof__ (a) _a = (a);          \
  __typeof__ (b) _b = (b);          \
  _a > _b ? _b : _a;                \
})


#define ABS(x)                      \
({                                  \
  __typeof__ (x) _x = (x);          \
  _x > 0 ? _x : -_x;                \
})


/**
 * Helpful macro to compare two variables. Cannot be used by other Macros
 * Returns bigger than 0 if a>b
 * Returns smaller than 0 if b>a
 * Returns 0 if a==b
 */
#define COMPARE(a,b)                \
({                                  \
  __typeof__ (a) _a = (a);          \
  __typeof__ (b) _b = (b);          \
  _a > _b ? 1 : (_a==_b ? 0 : -1);  \
})

/**
 * Helpful macro to determine the maximum of two numbers. To be used by Macros 
 * (inside functions use the MAX instead of MAX_MACRO)
 */
#define MAX_MACRO(a,b)                              (a > b ? a : b)


/**
 * Helpful macro to determine the minimum of two numbers. To be used by Macros 
 * (inside functions use the MIN instead of MIN_MACRO)
 */
#define MIN_MACRO(a,b)                              (a < b ? a : b)


/**
 * Determine the ceiling between two numbers
 * Basically, the ceiling of how many times the "x" fits entirely in "y".
 * For example:
 * PHAROS_CEILING(4095, 4096) = 1
 * PHAROS_CEILING(4096, 4096) = 1
 * PHAROS_CEILING(4097, 4096) = 2
 */
#define PHAROS_CEILING(x,y)                         (( (x) + (y) - 1U) / (y) )


/**
 * Calculate the LOG2 of a number
 */
#define PHAROS_LOG2(x)                              (uint32_t) (8U*sizeof(unsigned long long) - __builtin_clzll((x)) - 1U)


/**
 * Macro that determines the previous power of 2 of specified size. For example:
 *   PHAROS_PREVIOUS_POWER_2(2KiB)   = 2KiB
 *   PHAROS_PREVIOUS_POWER_2(1.5KiB) = 1KiB
 *   PHAROS_PREVIOUS_POWER_2(1.1KiB) = 1KiB
 *   PHAROS_PREVIOUS_POWER_2(1KiB)   = 1KiB
 */
#define PHAROS_PREVIOUS_POWER_2(x)                  (1U << (PHAROS_LOG2(x)))


/**
 * Macro that determines the next power of 2 of the available space. For example:
 *   PHAROS_NEXT_POWER_2(2KiB)   = 2KiB
 *   PHAROS_NEXT_POWER_2(1.5KiB) = 2KiB
 *   PHAROS_NEXT_POWER_2(1.1KiB) = 2KiB
 *   PHAROS_NEXT_POWER_2(1KiB)   = 1KiB
 */
#define PHAROS_NEXT_POWER_2(x)                      ((uint64_t)( (PHAROS_PREVIOUS_POWER_2(x) == (x)) ? (x) : (PHAROS_PREVIOUS_POWER_2(x) * 2U)))



/*****************************************
 * Alignment macros
 *****************************************/

#define STRUCTURE_FIELD_ALIGNMENT_64                __attribute__ ((aligned (64)))
#define STRUCTURE_FIELD_ALIGNMENT_32                __attribute__ ((aligned (32)))
#define STRUCTURE_FIELD_ALIGNMENT_16                __attribute__ ((aligned (16)))
#define STRUCTURE_FIELD_ALIGNMENT_8                 __attribute__ ((aligned (8)))
#define STRUCTURE_FIELD_ALIGNMENT_4                 __attribute__ ((aligned (4)))
#define STRUCTURE_FIELD_ALIGNMENT_2                 __attribute__ ((aligned (2)))

#define VARIABLE_ALIGNMENT_X(X)                     __attribute__ ((aligned (X)))

#define PHAROS_IPC_ALIGN                            VARIABLE_ALIGNMENT_X(64U)

#define PHAROS_ALIGN_2MiB                           __attribute__ ((aligned (VALUE_2MiB)))
#define PHAROS_ALIGN_1MiB                           __attribute__ ((aligned (VALUE_1MiB)))
#define PHAROS_ALIGN_16KiB                          __attribute__ ((aligned (VALUE_16KiB)))
#define PHAROS_ALIGN_4KiB                           __attribute__ ((aligned (VALUE_4KiB)))

/**
 * Alignment for the stacks. Size argument only used in some CPUs. 
 * Normally in CPUs with MMU, it is fixed to 4 KiB. 
 * In CPUs with MPU is the maximum between 256 (minimum possible in Pharos) and the next power of 2 of the size.
 */
#define PHAROS_ALIGN_STACK(size)                    VARIABLE_ALIGNMENT_X(PHAROS_DYNAMIC_STACK_ALIGN(size))


/**
 * The kernel stack does not require a special alignment since it is not guarded by MMU or MPU regions
 * (when inside the kernel mode the kernel has permissions to every kernel region)
 */
#define PHAROS_ALIGN_KERNEL_STACK                   VARIABLE_ALIGNMENT_X(PHAROS_STACK_ALIGNMENT)


/**
 * Compiler memory barrier
 */
#define PHAROS_SW_MEMORY_BARRIER()                  __asm__ volatile("": : :"memory")


/**
 * NULL pointer definition
 */
#define NULL                                        ((void *) 0)


/**
 * Boolean definition
 */
typedef enum
{
    BOOLEAN_FALSE = FALSE ,
    BOOLEAN_TRUE = TRUE
} bool;


/**
 * Single precision float
 */
typedef float float32_t;

/**
 * Double precision float
 */
typedef double float64_t;


/**
 * If compiling on a 32 bit system
 */
#ifndef __LP64__

/**
 * @brief Single byte character
 */
typedef char char_t;

/**
 * @brief signed 8 bit integer
 **/
typedef signed char int8_t;

/**
 * @brief unsigned 8 bit integer
 **/
typedef unsigned char uint8_t;

/**
 * @brief signed 16 bit integer
 **/
typedef signed short int16_t;

/**
 * @brief unsigned 16 bit integer
 **/
typedef unsigned short uint16_t;

/**
 * @brief signed 32 bit integer
 **/
typedef signed int int32_t;

/**
 * @brief unsigned 32 bit integer
 **/
typedef unsigned int uint32_t;

/**
 * @brief signed 64 bit integer
 **/
typedef signed long long int64_t STRUCTURE_FIELD_ALIGNMENT_8;

/**
 * @brief unsigned 64 bit integer
 **/
typedef unsigned long long uint64_t STRUCTURE_FIELD_ALIGNMENT_8;


/**
 * If compiling on a 64 bit machine
 */
#else

/**
 * @brief Single byte character
 */
typedef char char_t;

/**
 * @brief signed 8 bit integer
 **/
typedef signed char int8_t;

/**
 * @brief unsigned 8 bit integer
 **/
typedef unsigned char uint8_t;

/**
 * @brief signed 16 bit integer
 **/
typedef signed short int16_t;

/**
 * @brief unsigned 16 bit integer
 **/
typedef unsigned short uint16_t;

/**
 * @brief signed 32 bit integer
 **/
typedef signed int int32_t;

/**
 * @brief unsigned 32 bit integer
 **/
typedef unsigned int uint32_t;

/**
 * @brief signed 64 bit integer
 **/
typedef signed long int64_t STRUCTURE_FIELD_ALIGNMENT_8;

/**
 * @brief unsigned 64 bit integer
 **/
typedef unsigned long uint64_t STRUCTURE_FIELD_ALIGNMENT_8;

/**
 * @brief unsigned 128 bit integer
 **/
typedef __uint128_t uint128_t;

/**
 * @brief signed 128 bit integer
 **/
typedef __int128_t int128_t;


#endif


#if __SIZEOF_POINTER__ == 8

#define PHAROS_MEMORY_END   (0xffffffffffffffffU)

#define SIZE_MAX            (0xffffffffffffffffU)

/**
 * @brief pointer in this system is a 64 bit unsigned integer
 */
typedef uint64_t uintptr_t;

/**
 * @brief pointer in this system is a 64 bit signed integer
 */
typedef int64_t intptr_t;

#else 

#define PHAROS_MEMORY_END   (0xffffffffU)

#define SIZE_MAX            (0xffffffffU)

/**
 * @brief pointer in this system is a 32 bit unsigned integer
 */
typedef uint32_t uintptr_t;

/**
 * @brief pointer in this system is a 32 bit signed integer
 */
typedef int32_t intptr_t;

#endif 

typedef uint8_t *StackPointer;


typedef uintptr_t size_t;


/**
 * Declaration of the structure of the CPU context per Thread. The concrete definition of the structure is 
 * CPU-dependent since the CPU registers to be saved/restored is different from each CPU. The definition is located 
 * on each CPU specific code.
 */
typedef struct ThreadCpuContext ThreadCpuContext , *ptrThreadCpuContext;

typedef struct PharosCpuInterPartitionContext PharosCpuInterPartitionContext , *ptrPharosCpuInterPartitionContext;

typedef struct ThreadCpuBaseContext ThreadCpuBaseContext , *ptrThreadCpuBaseContext;


#endif /* HAL_H */
