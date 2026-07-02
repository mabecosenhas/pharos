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


#ifndef PHAROS_THREAD_RTTHREAD_H
#define PHAROS_THREAD_RTTHREAD_H

#include <pharos/kernel/thread/declarations.h>
#include <pharos/kernel/thread/thread.h>

struct RtThread
{
    /**
     * thread information
     * 
     * @note application must initialize this field
     */
    ThreadInfo thread;

    /**
     * RT deadline. When the deadline is reached, the health monitoring invokes an application method
     * 
     * @note application must define this value. A value of PHAROS_DEADLINE_IGNORED means there is no deadline
     */
    Deadline deadline;

    /**
     * node on the thread on the deadline tick handler
     */
    LinkedListNode deadlineTickNode;

    /**
     * time tick handler used to be invoked for the deadline of a thread
     */
    TimeTickHandler deadlineTickHandler;

    /**
     * Current deadline handler
     */
    ptrTimeTickHandler currentDeadlineHandler;

    /**
     * Instant when the next deadline will occur
     */
    ClockTick nextDeadline;
};


/**
 * Initialize a real-time thread (called at Pharos startup)
 * 
 * @param thread thread to initialize
 * @param isPeriodic flag indicating if the thread is periodic
 * @param isSporadic flag indicating if the thread is sporadic
 * @param kernelEntryPoint kernel mode entry point
 * @param userEntryPoint user mode entry point
 */
void pharosIThreadRtInitialize(ptrRtThread thread , ptrPartition partition , bool isPeriodic , bool isSporadic ,
                               ThreadNumber number , uintptr_t kernelEntryPoint , uintptr_t userEntryPoint ,
                               bool restart);


#endif /* RTTHREAD_H */
