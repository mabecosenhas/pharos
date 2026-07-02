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


#include <pharos/kernel/core/multithreading.h>
#include <pharos/kernel/thread/thread.h>
#include <pharos/hal/hal.h>
#include <pharos/system.h>
#include <pharos/kernel/partition/partition.h>
#include <pharos/kernel/scheduler/scheduler.h>
#include <pharos/hal/boardApi.h>
#include <pharos/kernel/core/coreinlines.h>
#include <pharos/kernel/clock/clock.h>
#include <pharos/kernel/queue/hwtable.h>
#include <pharos/kernel/core/core.h>
#include <pharos/kernel/core/coreconfinline.h>


void pharosCBoot(ptrCore core)
{
    /* interrupt level */
    PharosInterruptLevel level;

    
    /* first C code to be invoked inside the kernel */

    /* disable interrupts so the clock interrupt does not read the permissions of the running thread and does not allow the 
     * context switch to the first thread (because it needs to read from the kernel ThreadInfo structure). 
     * Interrupts will be re-enabled when the context switch is performed */
    pharosCpuDisableInterrupts(&level);

    /* set that the state is initializing */
    pharosICoreSetState(core , PHAROS_CORE_STATE_INITIALIZING);

    /* reset the running thread */
    pharosISchedUpdateRunningThread(NULL);

    /* initialize the interrupt configurations */
    pharosIInterruptInit(core);

    /* initialize the CPU core */
    pharosCpuCoreInitialize(core);

    /* initialize the core configuration */
    pharosCoreConfInitialize(&core->rmp);

    /* initialize the thread queues */
    pharosIThreadQueuesInitialize();

    /* initialize (always) the core scheduler. 
     * For flat scheduling it is the one used. 
     * For hierarchical scheduling it could also be used if one (or more) of the time slots are NULL */
    pharosISchedFlatInit(&core->flatScheduler , core);

    /* initialize the clock */
    pharosCpuClockInitialize();

    /* check if the scheduler is not placed */
    if(core->scheduler == NULL)
    {
        /* use the default flat scheduler */
        core->scheduler = &core->flatScheduler.scheduler;

        /* set the type */
        core->flatScheduler.scheduler.type = PHAROS_SCHEDULER_FLAT;

        /* set the core */
        core->flatScheduler.scheduler.core = core;

        /* set the running flat scheduler */
        core->runningFlatScheduler = &core->flatScheduler;

        /* initialize all partitions with memory addresses and initialize the inter-partition calls */
        pharosIPartitionsInitialize(core);

        /* initialize the MMU 
         * (can initialize now the MMU since the pharosIPartitionsInitialize has initialized the partitions memory addresses) */
        pharosCpuMmuInitialize();

        /* initialize the partition objects (all at startup under this scheduler unless specified lazy load) */
        pharosIPartitionsObjectsInit(core);
    }
        /* else, initialize the hierarchical scheduler */
    else
    {
        ptrHierarchicalScheduler scheduler = (ptrHierarchicalScheduler) core->scheduler;

        /* set the scheduler type to hierarchical */
        scheduler->scheduler.type = PHAROS_SCHEDULER_HIERARCHICAL;

        /* set the core */
        scheduler->scheduler.core = core;

        /* initialize all partitions with memory addresses and initialize the inter-partition calls */
        pharosIPartitionsInitialize(core);

        /* initialize the MMU 
         * (can initialize now the MMU since the pharosIPartitionsInitialize has initialized the partitions memory addresses) */
        pharosCpuMmuInitialize();

        /* initialize the running flat scheduler as the first partition to run scheduler */

        /* initialize the hierarchical scheduler */
        pharosISchedHierarchicalInit(core , scheduler);
    }

    /* start multi-threading */
    pharosIInitializeMultitasking(core);
}


PharosShutdownR pharosSShutdown(void)
{
    /* check if the running partition can shutdown the CPU */
    if(pharosIPartitionHasSupervisor() == TRUE)
    {
        /* shutdown the CPU */
        pharosCpuShutdown();
    }
    else
    {
        /* return error */
        return PHAROS_SHUTDOWN_NO_PERMISSIONS;
    }
}


PharosRestartR pharosSRestart(void)
{
    /* check if the running partition can shutdown the CPU */
    if(pharosIPartitionHasSupervisor() == TRUE)
    {
        /* restart the CPU */
        pharosCpuRestart();
    }
    else
    {
        /* return error */
        return PHAROS_RESTART_NO_PERMISSIONS;
    }
}
