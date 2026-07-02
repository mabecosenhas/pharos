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


#ifndef PHAROS_KERNEL_SEMAPHORE_COUNTING_H
#define PHAROS_KERNEL_SEMAPHORE_COUNTING_H


#include <pharos/kernel/semaphore/declarations.h>
#include <pharos/kernel/thread/queue.h>
#include <pharos/kernel/object/declarations.h>

struct SemaphoreCounting
{
    /**
     * internal counter of the semaphore 
     * 
     * note: application should initialize this with the initial count of the semaphore
     */
    uint64_t count;

    /**
     * internal maximum counter of the semaphore. If 0, then there is no maximum count (basically is 2^64-1)
     * 
     * note: application should initialize this with the semaphore maximum count
     */
    uint64_t maximumCount;

    /**
     * queue of threads blocked on this counting semaphore
     */
    ThreadQueue blockedQueue;

    struct
    {
        /**
         * Flag indicating if the queue is by priority (FALSE) or FIFO order (TRUE)
         * 
         * Note: initialized by the application
         */
        uint32_t isFifo : 1;
    } flags;
};


/**
 * Initializes the counting semaphores 
 * 
 * @param table table with the semaphores
 */
void pharosISemsCountInitialize(const ptrSemaphoreCountingTable table);



PharosSemCountObtainR pharosSSemCountObtain(SemCountNumber number);

PharosSemCountObtainTimeoutR pharosSSemCountObtainTimeout(SemCountNumber number , Timeout timeout);

PharosSemCountReleaseR pharosSSemCountRelease(SemCountNumber number);


#endif /* COUNTING_H */
