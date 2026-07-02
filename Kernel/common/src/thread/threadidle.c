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


#include <pharos/kernel/thread/idle.h>
#include <pharos/kernel/partition/partition.h>
#include <pharos/kernel/thread/dispatcher.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/hal/cpu/sections.h>



/* global idle thread (only used in this function) - one per core */
PHAROS_WEAK KERNEL_SECTION AperiodicThread pharosVIdleThread[PHAROS_NUMBER_CORES] = {

    /* initialize all cores with same idle thread content */
    [0 ... (PHAROS_NUMBER_CORES - 1)] =
    {
        /* entry point */
        .body = pharosIIdleThreadBody ,
        .info =
        {
            /* set the name of the idle thread (setting the "name" field directly is not possible) */
            .nameInternal = "PharosIdle" ,

            /* initialize the unblock info */
            .threadFlags.unblockDueToTimeout = FALSE ,

            /* lowest priority */
            .priority = PHAROS_PRIORITY_LOWEST ,

            /* is aperiodic thread */
            .threadId.flags.isAperiodic = TRUE ,

            /* is not a periodic thread */
            .threadId.flags.isPeriodic = FALSE ,

            /* is not a sporadic thread */
            .threadId.flags.isSporadic = FALSE ,

            /* no WCET */
            .wcet = PHAROS_WCET_UNLIMITED ,

            /* set its stack */
            .applicationStack =
            {
                /* initialize the stack pointer */
                .stack = NULL ,

                /* initialize the stack size */
                .size = PHAROS_CPU_STARTUP_STACK_SIZE ,
            } ,

            /* set its kernel stack */
            .kernelStack =
            {
                /* initialize the kernel stack pointer */
                .stack = NULL ,

                /* initialize the kernel stack size */
                .size = PHAROS_CPU_STARTUP_STACK_SIZE ,
            }
        }
    }
};


PHAROS_WEAK void pharosCpuIdleJobBody(void)
{
    /* default implementation of idle thread job does nothing */

    /* other implementations could set the CPU to low power mode, deep power mode and so on (depending on the CPU) */
}


void pharosIIdleThreadBody(void *data)
{
    /* starts up executing in supervisor mode and interrupts enabled */
    /* initialize all partitions that have lazy load */
    pharosIPartitionInitLazyLoad();

    /* force a dispatch (even though the "lazy" threads were made ready, a dispatch could have not occurred) */
    pharosIDispatchThread();

    /* make sure the loop never ends */
    for(;;)
    {
        /* restart any pending partition */
        pharosIPartitionRestartPending();

        /* call the job body */
        pharosCpuIdleJobBody();
    }
}


void pharosIThreadIdleInitialize(void)
{
    /* get the running core */
    CoreNumber runningCore = pharosICoreRunning();


    /* initialize the stack pointer according to the core being run */
    pharosVIdleThread[runningCore].info.applicationStack.stack = pharosCpuThreadIdleStack();

    /* initialize the stack pointer according to the core being run */
    pharosVIdleThread[runningCore].info.kernelStack.stack = pharosCpuThreadIdleStack();

    /* update the running thread for the scheduler to be able to compare priorities */
    pharosISchedUpdateRunningThread(&pharosVIdleThread[runningCore].info);

    /* for the idle thread the entry point is not the aperiodic thread body but the idle thread itself */
    pharosIThreadInitialize(&pharosVIdleThread[runningCore].info , NULL , NULL , FALSE , FALSE , TRUE ,
                            PHAROS_OBJECT_NUMBER_INVALID , (uintptr_t) pharosVIdleThread[runningCore].body ,
                            (uintptr_t) pharosVIdleThread[runningCore].body , FALSE);

    /* make the thread ready */
    pharosISchedIdleThreadSet(runningCore , &pharosVIdleThread[runningCore].info);
}
