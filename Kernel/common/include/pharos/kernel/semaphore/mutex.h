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


#ifndef MUTEX_H
#define MUTEX_H


#include <pharos/kernel/semaphore/declarations.h>
#include <pharos/kernel/thread/priority.h>
#include <pharos/kernel/thread/declarations.h>
#include <pharos/kernel/time/declarations.h>
#include <pharos/kernel/thread/queue.h>
#include <pharos/semaphore.h>
#include <pharos/kernel/partition/declarations.h>

/**
 * @brief Semaphore mutex
 */
struct SemaphoreMutex
{

    struct
    {
        /**
         * mutex internal count (between 1 and 0)
         */
        uint32_t count : 1;

        /**
         * Flag indicating if the queue is by priority (FALSE) or FIFO order (TRUE)
         * 
         * Note: initialized by the application
         */
        uint32_t isFifo : 1;

    } mutexFlags;

    /**
     * queue of threads blocked on this mutex
     */
    ThreadQueue mutexBlockedQueue;
};


/**
 * Initializes the simple semaphores 
 * 
 * @param table table with the semaphores
 */
void pharosISemsMutexInitialize(const ptrSemaphoreMutexTable table);

/**
 * get the mutex semaphore unprotected (means that it does not makes the checks to see if running on an aperiodic thread, interrupt, etc)
 * 
 * Protection: none required
 * 
 * @param runningThread running thread
 * @param semaphore semaphore to get
 * @param timeout timeout
 * 
 * @return returns success or the error value
 */
PharosSemMutexObtainTimeoutR pharosISemMutexObtainTimeout(ptrThreadInfo runningThread , ptrSemaphoreMutex semaphore ,
                                                          Timeout timeout);

/**
 * get the mutex semaphore from the running partition and its id
 * 
 * @param id id of the semaphore on the running partition
 * 
 * @return returns the semaphore pointer or NULL if the id is invalid
 */
ptrSemaphoreMutex pharosIGetSemaphoreMutex(SemMutexNumber number);



PharosSemMutexObtainR pharosSSemMutexObtain(SemMutexNumber number);

PharosSemMutexObtainTimeoutR pharosSSemMutexObtainTimeout(SemMutexNumber number , Timeout timeout);

PharosSemMutexReleaseR pharosSSemMutexRelease(SemMutexNumber number);

#endif /* MUTEX_H */
