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


#include <pharos/kernel/thread/thread.h>
#include <pharos/kernel/partition/partition.h>
#include <pharos/hal/hal.h>
#include <pharos/kernel/bulk/bulk.h>
#include <pharos/hal/board.h>
#include <pharos/hal/boardApi.h>
#include <pharos/kernel/future/futureTable.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/kernel/thread/aperiodicthread.h>
#include <pharos/hal/cpu/sections.h>
#include <pharos/kernel/thread/periodicthread.h>
#include <pharos/kernel/thread/sporadicthread.h>
#include <pharos/kernel/channel/channel.h>
#include <pharos/kernel/thread/dispatcher.h>
#include <pharos/kernel/thread/threadinline.h>
#include <pharos/kernel/interrupt/interruptinline.h>
#include <pharos/kernel/channel/channelbulkkernel.h>


/**
 * Table with all partitions
 * This table is only initialized once.
 *
 * @note note that this is on the BSS section, meaning that is automatically initialized by Pharos with "NULL" in every position
 */
KERNEL_SECTION_BSS static ptrPartition pharosVPartitions[PHAROS_PARTITION_MAXIMUM_NUMBER];


static intptr_t stringComparator(const void *a , const void* b);


static intptr_t threadNumberComparator(const void *a , const void* b);


static void pharosIPartitionInitFunction(ptrPartition p);


ptrPartition pharosIPartitionGet(PartitionNumber id)
{
    /* result of the method */
    ptrPartition result;


    /* if the id is invalid */
    if(id >= PHAROS_PARTITION_MAXIMUM_NUMBER)
    {
        /* return invalid */
        result = NULL;
    }
        /* id is valid */
    else
    {
        /* return the partition pointer */
        result = pharosVPartitions[id];
    }

    /* return the partition pointer or NULL if invalid */
    return result;
}


uint32_t pharosIPartitionGetNumber(void)
{
    /* get the number of partitions on the running core */
    return pharosICorePartitionTable()->size;
}


PartitionNumber pharosIPartitionStart(CoreNumber core)
{
    /* index of the partitions on the running core */
    PartitionNumber start = 0;

    /* core iterator */
    CoreNumber iterator;


    /* for every core with the number inferior to the running core */
    for(iterator = 0; iterator < core; iterator++)
    {
        /* increment it by the number of partitions on the core */
        start += pharosVCoreTable.cores[iterator].partitions.size;
    }

    /* return the index of the partitions on the running core */
    return start;
}


void pharosIPartitionsInitializeMinimal(ptrCore core , CoreNumber coreNumber)
{
    /* iterator of the partitions on this core */
    PartitionNumber i;

    /* partition to initialize */
    ptrPartition p;

    /* get the running core partition table */
    ptrPartitionTable table = &core->partitions;

    /* get the number of the first partition that runs on this core */
    PartitionNumber start = pharosIPartitionStart(coreNumber);


    /* set the core number */
    core->number = coreNumber;

    /* for each partition on the core */
    for(i = 0U; i < table->size; i++)
    {
        /* get the partition to initialize */
        p = &table->partitions[i];

        /* set all partitions as not initialized */
        p->partitionFlags.isInitialized = FALSE;

        /* set the number of the partition */
        p->id = i + start;

        /* initialize the partitions table */
        pharosVPartitions[p->id] = p;

        /* initialize hw queues */
        pharosIHwQueuesInitializeMinimal(p);

        /* initialize resources */
        pharosIResourcesInitializeMinimal(p);

        /* initialize channels */
        pharosIChannelsInitializeMinimal(p);

        /* initialize the inter-partition calls of this partition */
        pharosIIpCallsInitializeMinimal(p);
    }
}


void pharosIPartitionsInitialize(ptrCore core)
{
    /* iterator of the partitions on this core */
    PartitionNumber i;

    /* partition to initialize */
    ptrPartition p;

    /* get the running core partition table */
    ptrPartitionTable table = pharosICorePartitionTable();


    /* for each partition on the core */
    for(i = 0U; i < table->size; i++)
    {
        /* get the partition to initialize */
        p = &table->partitions[i];

        /* initialize the mem protection of the partition */
        pharosCpuMemProtectionInit(&p->memProtection , p->id);

        /* initialize the MMU info of the partition */
        pharosCpuPartitionMmuInit(&p->context , &p->memProtection);

        /* initialize the CPU specific information regarding the partition */
        pharosCpuPartitionInitialize(p);

        /* initialize the pointer to the core */
        p->core = &pharosVCoreTable.cores[pharosICoreRunning()];

        /* initialize the interrupts of the partition */
        pharosIInterruptPartitionInitialize(p , &p->interruptConfigurationTable);

        /* initialize the table of futures */
        pharosIFutureTableInitialize(p , &p->objects.futureTable);
    }
}


void pharosIPartitionReinit(ptrPartition p)
{
    ThreadNumber originalAperiodicIterator = p->aperiodicIterator;


    /* set to initializing. In this mode the threads of this partition can be dispatched to */
    p->state = PHAROS_PARTITION_STATE_INITIALIZING;

    /* initialize the data and BSS of the partition */
    pharosIPartitionDataInitialize(p->id);

    /* initialize the map that holds the threads sorted by name */
    treeMapInitialize(&p->threadsByName , stringComparator , OFFSETOF(ThreadInfo , nameNode));

    /* initialize the aperiodic threads by id map */
    treeMapInitialize(&p->aperiodicById , threadNumberComparator , OFFSETOF(ThreadInfo , idNode));

    /* initialize the periodic threads by id map */
    treeMapInitialize(&p->periodicById , threadNumberComparator , OFFSETOF(ThreadInfo , idNode));

    /* initialize the sporadic threads by id map */
    treeMapInitialize(&p->sporadicById , threadNumberComparator , OFFSETOF(ThreadInfo , idNode));

    /* initialize the dynamic memory */
    pharosIDynamicMemoryInitialize(&p->dynamicMemory);

    /* initialize the dynamic memory */
    pharosIDynamicMemoryInitialize(&p->kernelDynamicMemory);

    /* initialize the dynamic memory */
    pharosIDynamicMemoryInitialize(&p->sharedStackDynamicMemory);

    /* initialize the partition objects including the threads */
    pharosIObjectTableReinitialize(&p->objects , p);

    /* reset the time tick of the partition scheduler */
    pharosITimeResetNextTimeTick(p->scheduler);

    /* restore the aperiodic iterator (because of the restart thread we need to place exactly the same value) */
    p->aperiodicIterator = originalAperiodicIterator;

    /* install the interrupts */
    pharosIInterruptPartitionInitialize(p , &p->interruptConfigurationTable);

    /* call the partition init function */
    pharosIPartitionInitFunction(p);

    /* reset the restart flag */
    p->partitionFlags.readyRestart = FALSE;

    /* partition has now been initialized */
    p->partitionFlags.isInitialized = TRUE;

    /* partition is in nominal state */
    p->state = PHAROS_PARTITION_STATE_NOMINAL;
}


void pharosIPartitionInit(ptrPartition p)
{
    CoreNumber coreIterator;

    /* thread number of the new thread */
    ThreadNumber number;


    /* set to initializing */
    p->state = PHAROS_PARTITION_STATE_INITIALIZING;

    /* initialize the data and BSS of the partition */
    pharosIPartitionDataInitialize(p->id);

    /* initialize the list of dynamically created threads */
    simpleListInitialize(&p->createdThreads , OFFSETOF(ThreadInfo , createdNode));

    /* initialize the map that holds the threads sorted by name */
    treeMapInitialize(&p->threadsByName , stringComparator , OFFSETOF(ThreadInfo , nameNode));

    /* initialize the aperiodic threads by id map */
    treeMapInitialize(&p->aperiodicById , threadNumberComparator , OFFSETOF(ThreadInfo , idNode));

    /* initialize the periodic threads by id map */
    treeMapInitialize(&p->periodicById , threadNumberComparator , OFFSETOF(ThreadInfo , idNode));

    /* initialize the sporadic threads by id map */
    treeMapInitialize(&p->sporadicById , threadNumberComparator , OFFSETOF(ThreadInfo , idNode));

    /* for each core */
    for(coreIterator = 0; coreIterator < PHAROS_NUMBER_CORES; coreIterator++)
    {
        /* initialize the list of external threads that are inside the context of this partition */
        linkedListInitialize(&p->externalThreads[coreIterator] , OFFSETOF(ThreadInfo , externalThreadNode));
    }

    /* initialize the list of channel bulks hold by the partition */
    linkedListInitialize(&p->externalChannelBulksHold , OFFSETOF(ChannelBulkKernel , node));

    /* initialize the list of external futures hold by the partition */
    linkedListInitialize(&p->externalFuturesHold , OFFSETOF(Future , partitionNode));

    /* initialize the partition */
    pharosIObjectTableInitialize(&p->objects , p);

    /* initialize the dynamic memory */
    pharosIDynamicMemoryInitialize(&p->dynamicMemory);

    /* initialize the dynamic memory */
    pharosIDynamicMemoryInitialize(&p->kernelDynamicMemory);

    /* initialize the dynamic memory */
    pharosIDynamicMemoryInitialize(&p->sharedStackDynamicMemory);

    /* reset the number of inner threads using inter-partition */
    p->interPartitionNestLevel = 0U;

    /* initialize the restart number */
    p->restartNumber = 0U;

    /* if there is a restart thread */
    if(p->restartThread != NULL)
    {
        /* get the thread number to be used */
        number = p->aperiodicIterator;

        /* increment the aperiodic iterator for the next thread to be created */
        p->aperiodicIterator++;

        /* initialize common thread data */
        pharosIThreadInitialize(&p->restartThread->info , p , p->scheduler , FALSE , FALSE , TRUE , number ,
                                (uintptr_t) & pharosIPartitionRestartThreadBody , (uintptr_t) pharosIPartitionRestartThreadBody , FALSE);

        /* set the thread as started */
        pharosIThreadStateSetStarted(&p->restartThread->info.state);
    }

    /* invoke the init handler (with supervisor permissions) */
    pharosIPartitionInitFunction(p);

    /* reset the restart flag */
    p->partitionFlags.readyRestart = FALSE;

    /* partition has now been initialized */
    p->partitionFlags.isInitialized = TRUE;

    /* partition is in nominal state */
    p->state = PHAROS_PARTITION_STATE_NOMINAL;
}


void pharosIPartitionsObjectsInit(ptrCore core)
{
    /* iterator on the table of partitions */
    PartitionNumber i;

    /* partition iterator */
    ptrPartition p;


    /* initialize the idle thread (do it first so we can call syscalls with a running thread) */
    pharosIThreadIdleInitialize();

    /* initialize the flat scheduler associated to the core (in case of a global flat scheduler is used or there is a 
     * time window without a specific partition to run */
    pharosISchedFlatInit(&core->flatScheduler , core);

    /* for each partition */
    for(i = 0U; i < pharosICorePartitionTable()->size; i++)
    {
        /* get the partition iterator */
        p = &pharosICorePartitionTable()->partitions[i];

        /* if the core is scheduled with an hierarchical scheduler */
        if(core->scheduler->type == PHAROS_SCHEDULER_HIERARCHICAL)
        {
            /* if there is hierarchical scheduling */
            if(p->scheduler != NULL)
            {
                /* initialize the flat scheduler associated to the partition */
                pharosISchedFlatInit(p->scheduler , core);
            }
            else
            {
                /* use the same flat scheduler as the core */
                p->scheduler = &core->flatScheduler;
            }
        }
        else
        {
            /* use the same flat scheduler as the core */
            p->scheduler = &core->flatScheduler;
        }

        /* only initialize now the partitions that are not lazy load and if creation should not be at runtime */
        if(p->partitionFlags.isLazyLoad == FALSE)
        {
            /* initialize the partition */
            pharosIPartitionInit(p);
        }
    }
}


void pharosIPartitionInitLazyLoad(void)
{
    /* iterator on the table of partitions */
    PartitionNumber i;

    /* partition iterator */
    ptrPartition p;


    /* for each partition */
    for(i = 0U; i < pharosICorePartitionTable()->size; i++)
    {
        /* get the partition iterator */
        p = &pharosICorePartitionTable()->partitions[i];

        /* only initialize the partitions that are lazy load */
        if(p->partitionFlags.isLazyLoad == TRUE)
        {
            /* initialize the partition */
            pharosIPartitionInit(p);
        }
    }
}


ptrPartition pharosIPartitionGetRunning(void)
{
    /* result of the method (partition running) */
    ptrPartition result;


    /* check if it running inside an interrupt */
    if(pharosIInterruptIsRunning() == TRUE)
    {
        /* if there is a timer running */
        if(pharosITimerRunningPartition() != NULL)
        {
            /* get the environment that owns the timer */
            result = pharosITimerRunningPartition();
        }
        else
        {
            /* check the partition which owns the interrupt running */
            result = pharosIInterruptRunningEnvGet();
        }
    }
        /* check the running core is using a partition */
    else if(pharosICore()->runningPartition != NULL)
    {
        /* return that partition (e.g. init handler) */
        result = pharosICore()->runningPartition;
    }
        /* check if there is a running thread */
    else
    {
        /* return the running thread environment */
        result = pharosISchedGetRunningThread()->runningPartition;
    }

    /* return the pointer to the running environment */
    return result;
}


bool pharosIAddressInsideMemoryArea(ptrEnvMemoryProtection memProtection , uint8_t *address)
{
    /* memory area iterator */
    uint32_t i;


    /* for each memory area */
    for(i = 0; i < PHAROS_NUMBER_MEMORY_AREAS; i++)
    {
        /* check if the address is inside it */
        if(memProtection->startAddress[i] <= address && address < memProtection->endAddress[i])
        {
            /* it yes */
            return TRUE;
        }
    }

    /* looped through all the memory areas and could not find it */
    return FALSE;
}


bool pharosIPartitionIsInitialized(ptrPartition partition)
{
    /* return the initialized flag */
    return partition->partitionFlags.isInitialized;
}


intptr_t threadNumberComparator(const void *a , const void* b)
{
    /* cast a to a thread number */
    /* MISRA2004 11.4: This function must receive a ptrThreadNumber as argument a */
    const ptrThreadNumber na = (ptrThreadNumber) a;

    /* cast b to a thread number */
    /* MISRA2004 11.4: This function must receive a ptrThreadNumber as argument b */
    const ptrThreadNumber nb = (ptrThreadNumber) b;

    return (intptr_t) *na - (intptr_t) * nb;
}


intptr_t stringComparator(const void *a , const void* b)
{
    /* cast a to a char * */
    /* MISRA2004 11.4: This function must receive a (char *) as argument a */
    const char *s1 = (char *) a;

    /* cast b to a char * */
    /* MISRA2004 11.4: This function must receive a (char *) as argument b */
    const char * s2 = (char *) b;

    /* iterator on the first string */
    char c1;

    /* iterator on the second string */
    char c2;


    do
    {
        /* get the first char to compare */
        c1 = *s1;

        /* get the second char to compare */
        c2 = *s2;

        /* if we have reached the end of the first string */
        if(c1 == '\0')
        {
            /* stop searching */
            break;
        }

        /* increment the pointer to search for */
        s1++;

        /* increment the pointer to search for */
        s2++;
    }
        /* if the chars don't match stop searching */
    while(c1 == c2);

    /* return the result */
    return (intptr_t) c1 - (intptr_t) c2;
}


void partitionAddAperiodicThread(ptrPartition partition , ptrAperiodicThread thread)
{
    /* interrupt level */
    PharosInterruptLevel level;

    /* disable interrupts: cannot allow an interrupt or another thread to change the _threadsReady at the same time */
    pharosCpuDisableInterrupts(&level);

    /* append the thread to the list of dynamically created threads */
    simpleListAppendLast(&partition->createdThreads , &thread->info);

    /* add the thread to the map of aperiodic threads */
    treeMapPut(&partition->aperiodicById , &thread->info.threadId.number , thread);

    /* re-enable interrupts */
    pharosCpuEnableInterrupts(&level);
}


void partitionAddPeriodicThread(ptrPartition partition , ptrPeriodicThread thread)
{
    /* interrupt level */
    PharosInterruptLevel level;

    /* disable interrupts: cannot allow an interrupt or another thread to change the _threadsReady at the same time */
    pharosCpuDisableInterrupts(&level);

    /* append the thread to the list of dynamically created threads */
    simpleListAppendLast(&partition->createdThreads , &thread->rtThread.thread);

    /* add the thread to the map of periodic threads */
    treeMapPut(&partition->periodicById , &thread->rtThread.thread.threadId.number , thread);

    /* re-enable interrupts */
    pharosCpuEnableInterrupts(&level);
}


void partitionAddSporadicThread(ptrPartition partition , ptrSporadicThread thread)
{
    /* interrupt level */
    PharosInterruptLevel level;

    /* disable interrupts: cannot allow an interrupt or another thread to change the _threadsReady at the same time */
    pharosCpuDisableInterrupts(&level);

    /* append the thread to the list of dynamically created threads */
    simpleListAppendLast(&partition->createdThreads , &thread->rtThread.thread);

    /* add the thread to the map of sporadic threads */
    treeMapPut(&partition->sporadicById , &thread->rtThread.thread.threadId.number , thread);

    /* re-enable interrupts */
    pharosCpuEnableInterrupts(&level);
}


void pharosIPartitionThreadReset(ptrThreadInfo thread)
{
    /* if the thread is not running an IP-call or resource */
    if(pharosIThreadStateIsInterPartition(thread) == FALSE)
    {
        /* then we can reset it */

        /* set the state of the thread as stopped (this will prevent deadlines from executing) */
        pharosIThreadStateSetStopped(&thread->state);

        /* if the thread is waiting on something we have to remove the time ticker */
        pharosITimeTickRemoveThread(thread);

        /* check if the thread is RT (that is, could have a deadline). 
         * This must be done after removing the time tick */
        if(pharosIThreadIsRt(thread) == TRUE)
        {
            /* remove the deadline handler */
            /* MISRA: can cast to ptrRtThread since we checked that the thread is RT */
            pharosITimeTickRemoveDeadline((ptrRtThread) thread);
        }

        /* if the thread is on the ready list remove it */
        pharosISchedRemoveReadyThread(thread);
    }
}


void pharosIPartitionResetThreads(ptrPartition partition)
{
    ObjectNumber i;

    /* no need to disable interrupts since at this point threads can no longer be created */

    ptrThreadInfo thread = simpleListRemoveFirst(&partition->createdThreads);

    /* reset dynamically created threads */
    while(thread != NULL)
    {
        /* reset the thread */
        pharosIPartitionThreadReset(thread);

        /* no need to de-allocate memory since the dynamic memory will be reset afterwards */

        /* remove next */
        thread = simpleListRemoveFirst(&partition->createdThreads);
    }

    /* remove statically created threads from the ready list */

    /* remove aperiodic threads */
    for(i = 0; i < partition->objects.threadTable.sizeAperiodicThreads; i++)
    {
        /* get the thread */
        thread = &partition->objects.threadTable.aperiodicThreads[i].info;

        /* reset the thread */
        pharosIPartitionThreadReset(thread);
    }

    /* remove periodic threads */
    for(i = 0; i < partition->objects.threadTable.sizePeriodicThreads; i++)
    {
        /* get the thread */
        thread = &partition->objects.threadTable.periodicThreads[i].rtThread.thread;

        /* reset the thread */
        pharosIPartitionThreadReset(thread);
    }

    /* remove sporadic threads */
    for(i = 0; i < partition->objects.threadTable.sizeSporadicThreads; i++)
    {
        /* get the thread */
        thread = &partition->objects.threadTable.sporadicThreads[i].rtThread.thread;

        /* reset the thread */
        pharosIPartitionThreadReset(thread);
    }

    /* for all external threads that are inside the partition filters/IP calls, stop them (leave the filter and return error) */
    pharosIPartitionExternalThreadsStop(partition);

}


void pharosIPartitionExternalThreadsStopOnPartitionCore(CoreNumber runningCore , ptrPartition partition)
{
    ptrThreadInfo thread;

    /* remove the first external thread accessing the partition being restarted */
    thread = linkedListRemoveFirst(&partition->externalThreads[runningCore]);

    /* for every external thread */
    while(thread != NULL)
    {
        /* have to make the thread return an error (uninitialized) from its filter/IP call */
        pharosCpuThreadFinishCall(thread , thread->activeContext);

        /* decrement the nest level of inter-partition calls */
        pharosIThreadStateInterPartitionDec(thread);

        /* clear any blocking or waiting state of the thread */
        pharosIThreadStateClear(&thread->state);

        /* add the thread to the ready list */
        pharosISchedAddReadyThread(thread);

        /* get the next external thread */
        thread = linkedListRemoveFirst(&partition->externalThreads[runningCore]);
    }

    pharosIDispatchThread();
}


void pharosIThreadInterPartitionRestore(ptrThreadInfo thread , ptrPartition original , bool *wasInside ,
                                        ptrPharosCpuInterPartitionContext originalContext)
{
    /* interrupt level */
    PharosInterruptLevel level;

    /* get the running core */
    CoreNumber core = pharosICoreRunning();


    /* disable interrupts: cannot allow a restart while we are changing */
    pharosCpuDisableInterrupts(&level);

    /* restore the original context */
    thread->activeContext = originalContext;

    /* remove it from the list of external threads */
    linkedListRemoveObject(&thread->runningPartition->externalThreads[core] , thread);

    if((*wasInside) == TRUE)
    {
        /* restore it */

        /* add it to the list of the new partition external threads */
        linkedListAppendLast(&original->externalThreads[core] , thread);
    }

    /* restore the thread running partition */
    thread->runningPartition = original;

    /* decrement the thread inter partition nesting level */
    pharosIThreadStateInterPartitionDec(thread);

    /* re-enable interrupts */
    pharosCpuEnableInterrupts(&level);
}


void pharosIPartitionInitFunction(ptrPartition p)
{
    /* set the core running partition */
    pharosICore()->runningPartition = p;

    /* if there is a init handler */
    if(p->initHandler != NULL)
    {
        /* then invoke the init handler (with supervisor permissions) */
        p->initHandler(p->id , p->data);
    }

    /* reset the core running partition */
    pharosICore()->runningPartition = NULL;
}
