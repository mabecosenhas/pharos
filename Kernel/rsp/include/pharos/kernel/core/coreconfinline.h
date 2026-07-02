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


#ifndef PHAROS_COREINLINE_H
#define PHAROS_COREINLINE_H

#include <pharos/kernel/core/core.h>
#include <pharos/kernel/core/multi.h>
#include <pharos/kernel/clock/clock.h>
#include <pharos/kernel/assert/assert.h>
#include <pharos/kernel/thread/dispatcher.h>


INLINE void pharosIRmpIncDateOneClockTick(ptrCore core)
{
    /* increment the date on the current core */
    pharosIIncDateOneClockTick();
}


INLINE CoreNumber pharosICoreRunning(void)
{
    /* only has one core */
    return 0U;
}


INLINE ptrCore pharosICore(void)
{
    return &pharosVCoreTable.cores[0];
}


/**
 * Get the specified core 
 * 
 * @param core core number
 * 
 * @return returns the pointer to the core
 */
INLINE ptrCore pharosICoreGet(CoreNumber core)
{
    return &pharosVCoreTable.cores[0];
}


INLINE ptrPartitionTable pharosICorePartitionTable(void)
{
    return &pharosVCoreTable.cores[0].partitions;
}


INLINE ptrPartition pharosIPartitionGetOnRunCore(PartitionNumber id)
{
    /* get the partition (there is only one core) */
    return pharosIPartitionGet(id);
}


INLINE uintptr_t pharosIInterruptNest(void)
{
    return pharosVInterruptNestingLevel;
}


/**
 * Get the scheduler on the running core
 * 
 * @return returns the scheduler on the running core
 */
INLINE ptrScheduler pharosIScheduler(void)
{
    return pharosVCoreTable.cores[0].scheduler;
}


/**
 * get the running thread
 * 
 * Protection: none
 * 
 * @return returns the running thread
 */
INLINE ptrThreadInfo pharosISchedGetRunningThread(void)
{
    /* get the running thread */
    return pharosVRunningThread;
}


/**
 * Set the thread context switch need
 * 
 * @param context flag indicating if context switch is needed
 */
INLINE void pharosIThreadContextSwitchNeeded(bool context)
{
    pharosVNeedsThreadContextSwitch = context;
}


INLINE bool pharosINeedsThreadContextSwitch(void)
{
    return pharosVNeedsThreadContextSwitch;
}


/**
 * Update the running thread 
 * 
 * @param thread thread that is now running
 */
INLINE void pharosISchedUpdateRunningThread(ptrThreadInfo thread)
{
    /* update the running thread */
    pharosVRunningThread = thread;
}


INLINE bool pharosIIsDispatchingEnabled(void)
{
    /* dispatching is only enable if the dispatch level is 0 */
    return pharosVDispatchLevel > 0U ? FALSE : TRUE;
}


INLINE void pharosIDispatchDisable(void)
{
    /* increment the level */
    pharosVDispatchLevel++;
}


INLINE void pharosIDispatchEnable(void)
{
    /* decrement the level */
    pharosVDispatchLevel--;

    /* check if we just enabled dispatching */
    if(pharosVDispatchLevel == 0U)
    {
        /* make any dispatch if needed */
        pharosIDispatchThread();
    }
}


INLINE bool pharosIInterruptIsRunning(void)
{
    /* return if the interrupt is running based on the level */
    return pharosVInterruptNestingLevel != 0U ? TRUE : FALSE;
}



/* PHAROS_COREINLINE_H */
#endif 
