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


#ifndef PHAROS_THREAD_DECLARATIONS_H
#define PHAROS_THREAD_DECLARATIONS_H

#include <pharos/hal/hal.h>
#include <pharos/kernel/object/declarations.h>
#include <pharos/kernel/time/declarations.h>

#define KERNEL_IDLE_THREAD_NUMBER           (0U)

/**
 * @brief maximum number of thread priorities (from 0 to 255)
 */
#define MAXIMUM_NUMBER_PRIORITIES           (256U)

#define PRIORITY_BIT_MAP_NUMBER_BITS        (64U)

#define NUMBER_BIT_MAPS                     (MAXIMUM_NUMBER_PRIORITIES / PRIORITY_BIT_MAP_NUMBER_BITS)


/**
 * unlimited WCET
 */
#define PHAROS_WCET_UNLIMITED               (0U)


/**
 * ignored deadline
 */
#define PHAROS_DEADLINE_IGNORED             (0U)

/**
 * Maximum thread name (for newly allocated threads)
 */
#define PHAROS_THREAD_NAME_MAX              (32U)


/**
 * Forward declaration of structures 
 */
typedef struct ThreadInfo ThreadInfo , *ptrThreadInfo;
typedef struct PeriodicThread PeriodicThread , *ptrPeriodicThread;
typedef struct SporadicThread SporadicThread , *ptrSporadicThread;
typedef struct AperiodicThread AperiodicThread , *ptrAperiodicThread;

typedef struct ThreadState ThreadState , *ptrThreadState;

typedef struct ThreadId ThreadId , *ptrThreadId;

typedef struct RtThread RtThread , *ptrRtThread;

typedef struct FifoQueue FifoQueue , *ptrFifoQueue;
typedef struct FifoQueueNode FifoQueueNode , *ptrFifoQueueNode;
typedef struct ThreadQueue ThreadQueue , *ptrThreadQueue;
typedef struct ThreadQueueNode ThreadQueueNode , *ptrThreadQueueNode;

typedef struct PriorityQueue PriorityQueue , *ptrPriorityQueue;

typedef struct PriorityQueueNode PriorityQueueNode , *ptrPriorityQueueNode;

typedef struct ThreadStatus ThreadStatus , *ptrThreadStatus;

typedef struct Stack Stack , *ptrStack;

typedef ObjectNumber ThreadNumber , *ptrThreadNumber;

typedef struct StackShared StackShared , *ptrStackShared;

typedef struct PharosSporadicThreadActivation PharosSporadicThreadActivation , *ptrPharosSporadicThreadActivation;
typedef struct PharosThreadProperties PharosThreadProperties , *ptrPharosThreadProperties;

typedef struct PharosThreadAperiodicProperties PharosThreadAperiodicProperties , *ptrPharosThreadAperiodicProperties;
typedef struct PharosThreadSporadicProperties PharosThreadSporadicProperties , *ptrPharosThreadSporadicProperties;
typedef struct PharosThreadPeriodicProperties PharosThreadPeriodicProperties , *ptrPharosThreadPeriodicProperties;

typedef struct PharosThreadCore PharosThreadCore , *ptrPharosThreadCore;

typedef struct ThreadLocalStorage ThreadLocalStorage , *ptrThreadLocalStorage;

typedef struct ThreadRegularStackUserSpace ThreadRegularStackUserSpace , *ptrThreadRegularStackUserSpace;

typedef struct ThreadSporadicStackUserSpace ThreadSporadicStackUserSpace , *ptrThreadSporadicStackUserSpace;

/**
 * @brief worst execution time for a periodic or sporadic task in slices of 1us.
 *
 * Supports WCET up to 4294 seconds
 */
typedef uint32_t Wcet;


/**
 * Number of clock ticks until the real-time thread deadline is reached
 */
typedef ClockTick Deadline;


#endif /* DECLARATIONS_H */
