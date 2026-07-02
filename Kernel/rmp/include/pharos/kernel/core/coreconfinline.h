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
#include <pharos/kernel/core/coreconf.h>
#include <pharos/kernel/core/multi.h>
#include <pharos/kernel/clock/clock.h>
#include <pharos/kernel/assert/assert.h>
#include <pharos/kernel/thread/dispatcher.h>


/**
 * Get the running core
 * 
 * @return returns the number of the running core
 */
CoreNumber pharosICoreRunning(void) __attribute__((const));


/**
 * Get the pointer to the Core running
 * 
 * @return returns the pointer to the core 
 */
INLINE ptrCore pharosICore(void)
{
    return &pharosVCoreTable.cores[pharosICoreRunning()];
}


/**
 * Get the pointer to the specified core number
 * 
 * @param core core number
 * 
 * @return returns the pointer to the core 
 */
INLINE ptrCore pharosICoreGet(CoreNumber core)
{
    return &pharosVCoreTable.cores[core];
}


INLINE ptrPartitionTable pharosICorePartitionTable(void)
{
    return &pharosVCoreTable.cores[pharosICoreRunning()].partitions;
}


INLINE ptrPartition pharosIPartitionGetOnRunCore(PartitionNumber id)
{
    /* get the partition */
    ptrPartition p = pharosIPartitionGet(id);


    /* if the core is different */
    if(p != NULL && p->core->number != pharosICoreRunning())
    {
        /* then return NULL */
        p = NULL;
    }

    /* return the partition or NULL if the partition is on another core */
    return p;
}


/**
 * Set the thread context switch need
 * 
 * @param context flag indicating if context switch is needed
 */
INLINE void pharosIThreadContextSwitchNeeded(bool context)
{
    /* set the thread context swith needed for the running core */
    pharosVRmpCore[pharosICoreRunning()].contextSwitchNeeded = context;
}


/**
 * Get the thread context switch need for the current core
 * 
 * @return returns the thread context switch need for the current core
 */
INLINE bool pharosINeedsThreadContextSwitch(void)
{
    /* get the thread context swith needed for the running core */
    return pharosVRmpCore[pharosICoreRunning()].contextSwitchNeeded;
}


/**
 * Update the running thread 
 * 
 * @param thread thread that is now running
 */
INLINE void pharosISchedUpdateRunningThread(ptrThreadInfo thread)
{
    /* update the running thread */
    pharosVRmpCore[pharosICoreRunning()].runningThread = thread;
}


INLINE ptrThreadInfo pharosISchedGetRunningThread(void)
{
    /* return the running thread on the running core */
    return pharosVRmpCore[pharosICoreRunning()].runningThread;
}


/**
 * Get the scheduler on the running core
 * 
 * @return returns the scheduler on the running core
 */
INLINE ptrScheduler pharosIScheduler(void)
{
    return pharosVCoreTable.cores[pharosICoreRunning()].scheduler;
}


INLINE bool pharosIIsDispatchingEnabled(void)
{
    /* dispatching is only enable if the dispatch level is 0 */
    return pharosVRmpCore[pharosICoreRunning()].dispatchLevel == 0 ? TRUE : FALSE;
}


INLINE void pharosIDispatchDisable(void)
{
    /* increment the level */
    pharosVRmpCore[pharosICoreRunning()].dispatchLevel++;
}


INLINE void pharosIDispatchEnable(void)
{
    /* decrement the level */
    pharosVRmpCore[pharosICoreRunning()].dispatchLevel--;
    
    /* check if we just enabled dispatching */
    if(pharosVRmpCore[pharosICoreRunning()].dispatchLevel == 0U)
    {
        /* make any dispatch if needed */
        pharosIDispatchThread();
    }
}


INLINE bool pharosIInterruptIsRunning(void)
{
    /* return if the interrupt is running based on the level */
    return pharosVRmpCore[pharosICoreRunning()].interruptLevel != 0U ? TRUE : FALSE;
}


INLINE uintptr_t pharosIInterruptNest(void)
{
    return pharosVRmpCore[pharosICoreRunning()].interruptLevel;
}


INLINE void pharosIRmpIncDateOneClockTick(ptrCore core)
{
    /* only core 0 can update the date */
    if(core->number == 0U)
    {
        /* increment the date on the current core */
        pharosIIncDateOneClockTick();
    }
}

/* PHAROS_COREINLINE_H */
#endif 
