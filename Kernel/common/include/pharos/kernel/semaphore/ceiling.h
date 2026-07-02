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


#ifndef CEILING_H
#define CEILING_H


#include <pharos/kernel/semaphore/declarations.h>
#include <pharos/kernel/thread/priority.h>
#include <pharos/kernel/thread/declarations.h>
#include <pharos/kernel/time/declarations.h>
#include <pharos/kernel/thread/queue.h>
#include <pharos/semaphore.h>
#include <pharos/kernel/partition/declarations.h>
#include <pharos/kernel/resource/declarations.h>

/**
 * @brief Semaphore with priority ceiling
 */
struct SemaphoreCeiling
{
    /**
     * @brief holder of the semaphore (or NULL if none)
     */
    ptrThreadInfo holder;

    /**
     * queue of all the threads blocked on this semaphore
     */
    ThreadQueue blockedQueue;

    /**
     * queue node of all the threads blocked on this semaphore
     */
    ThreadQueueNode blockedQueueNode;

    /**
     * @brief priority ceiling value
     * 
     * @note application must define this value
     */
    ThreadPriority ceiling;

    struct
    {
        /**
         * @brief flag indicating if the semaphore can update the ceiling according to the threads that use it
         * (or attempt to use it)
         * 
         * @note not used (reserved for future use)
         */
        uint32_t updateCeiling : 1;

        /**
         * Flag indicating if the queue is by priority (FALSE) or FIFO order (TRUE)
         * 
         * @note initialized by the application
         * @note if schedulability is important, then this flag should ALWAYS be set to FALSE 
         * (or left uninitialized -> the compiler will automatically place FALSE)
         */
        uint32_t isFifo : 1;

    } ceilingFlags;

    /**
     * Environment of the semaphore
     */
    ptrPartition owner;
};



/**
 * Initializes the ceiling semaphores
 * 
 * @param table table with the semaphores
 */
void pharosISemsCeilInitialize(ptrPartition id , const ptrSemaphoreTable table);

/**
 * Initialize a single semaphore
 * 
 * @param semaphore semaphore to initialize 
 */
void pharosISemCeilInitialize(ptrPartition id , ptrSemaphoreCeiling semaphore , bool isResource);

/**
 * release a semaphore 
 * 
 * @param semaphore semaphore pointer to release
 * @param runningThread running thread
 * @param resource resource if the ceiling semaphore belongs to a resource (otherwise should be NULL)
 * 
 * @return returns the result of the release (success or error)
 */
PharosSemCeilReleaseR pharosISemCeilRelease(ptrSemaphoreCeiling semaphore , ptrThreadInfo runningThread , ptrResource resource);

/**
 * obtains a semaphore 
 * 
 * @param semaphore semaphore to obtain
 * @param timeout timeout for the obtain
 * @param resource resource that uses the semaphore (or NULL if none)
 * 
 * @return returns the result of the obtain (success or error)
 */
PharosSemCeilObtainR pharosISemCeilObtain(ptrSemaphoreCeiling semaphore , Timeout timeout , ptrResource resource);


PharosSemCeilObtainR pharosSSemCeilObtain(SemCeilNumber number , Timeout timeout);


PharosSemCeilReleaseR pharosSSemCeilRelease(SemCeilNumber number);


#endif /* CEILING_H */
