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


#include <pharos/kernel/core/coreconf.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/hal/halapi.h>
#include <pharos/kernel/channel/channel.h>
#include <pharos/hal/cpu/cache.h>
#include <pharos/hal/cpu/sections.h>
#include <pharos/kernel/object/declarations.h>
#include <pharos/kernel/core/rmpscheduler.h>


KERNEL_SECTION_USS PharosRmpCore pharosVRmpCore[PHAROS_NUMBER_CORES];


#define PHAROS_CORE_IS_READY        (0xdf321321U)


KERNEL_SECTION_BSS volatile CoreNumber partition2Core[PHAROS_PARTITION_MAXIMUM_NUMBER];

KERNEL_SECTION_BSS static volatile uint32_t coreIsReady[PHAROS_NUMBER_CORES];




/**
 * Initialization stack (also corresponds to the idle stack)
 * This contains all of the stacks of each core
 * 
 * Note that this stack MUST not be on the BSS section. If it was, then when it was zeroying out the BSS it would be also
 * zeroying its own stack and unpredictable behavior would occur.
 */
KERNEL_SECTION_USS uint8_t pharosVStartupStack[PHAROS_NUMBER_CORES * PHAROS_CPU_STARTUP_STACK_SIZE] VARIABLE_ALIGNMENT_X(PHAROS_STACK_ALIGNMENT);


/**
 * Initialize the partition2Core variable 
 */
static void pharosIPartition2CoreInitialize(void);


uint8_t *pharosCpuThreadIdleStack(void)
{
    /* return the stack address of the running core */
    return &pharosVStartupStack[pharosICoreRunning() * PHAROS_CPU_STARTUP_STACK_SIZE];
}


void pharosMulticoreInitialize(CoreNumber core)
{
    /* core iterator */
    CoreNumber iterator;

    /* core address iterator */
    ptrCore c;


    /* if running on the main core */
    if(core == 0)
    {
        /* initialize the kernel data */

        /* copy the kernel data */
        pharosIKernelDataInitialize();

        /* copy the inter-partition calls data */
        pharosIIpCallDataInitialize();

        /* flush the cache */
        pharosCpuFlushDataCache();

        /* make sure the pharosCpuStartFlag is set before initializing the data (so that the other cores can continue) */
        __atomic_thread_fence(__ATOMIC_RELEASE);

        /* initialize the shared data */
        pharosISharedDataInitialize();

        /* initialize the kernel bulks */
        pharosIKernelBulksInitialize();

        /* initialize the partition2Core variable */
        pharosIPartition2CoreInitialize();

        /* make sure the pharosCpuStartFlag is set before initializing the data (so that the other cores can continue) */
        __atomic_thread_fence(__ATOMIC_RELEASE);

        /* for all cores */
        for(iterator = 0U; iterator < pharosVCoreTable.size; iterator++)
        {
            /* signal that the core is not ready */
            coreIsReady[iterator] = 0U;

            /* get the address of the core */
            c = &pharosVCoreTable.cores[iterator];

            /* initialize all partitions with minimum information so that inter-partition calls from multiple cores 
             * will return an error (for non-initialized partitions) but will not access uninitialized data (and
             * possibly "explode" the whole system) */
            pharosIPartitionsInitializeMinimal(c , iterator);
        }

        /* flush the cache */
        pharosCpuFlushDataCache();

        /* make sure the everything has been done so far before before allowing the other cores to start */
        __atomic_thread_fence(__ATOMIC_RELEASE);

        /* allow the other cores to start initializing their data, bss, etc */
        for(iterator = 1U; iterator < PHAROS_NUMBER_CORES; iterator++)
        {
            pharosCpuMulticoreTriggerInterrupt(iterator);
        }

        /* make sure the pharosCpuStartFlag is set before initializing the data (so that the other cores can continue) */
        __atomic_thread_fence(__ATOMIC_RELEASE);
    }
    else
    {
        /* wait for the kernel data to be copied (this contains the partitionsDataStart, partitionsDataAddr, etc that are 
         * needed to initialize the data of the each partition */
        while(pharosCpuMulticoreIntIsPending() == FALSE);

        /* now clear the pending interrupt */
        pharosCpuMulticoreClearPending();
    }

    /* check if the core is used by the application */
    if(core < pharosVCoreTable.size)
    {
        /* initialize the MPSC queue of the running core */
        pharosIMpscQueueInitialize(&pharosICore()->rmp.queue);

        /* enable the multicore interrupt on this core */
        pharosCpuMulticoreEnableInt();

        /* initialize the core */
        pharosCBoot(&pharosVCoreTable.cores[core]);
    }
        /* if the core is not used */
    else
    {
        /* just signal that the core is ready */
        coreIsReady[pharosICoreRunning()] = PHAROS_CORE_IS_READY;

        /* halt the running core (application is not using it) */
        pharosCpuCoreHalt();
    }
}


void pharosCoreConfInitialize(ptrPharosCoreConfiguration conf)
{
    /* core iterator */
    CoreNumber core = pharosICoreRunning();


    /* reset the running thread */
    pharosVRmpCore[core].runningThread = NULL;

    /* initialize the context switch need */
    pharosVRmpCore[core].contextSwitchNeeded = TRUE;

    /* initialize the interrupt nesting level */
    pharosVRmpCore[core].interruptLevel = 0U;

    /* reset the thread context disable level */
    pharosVRmpCore[core].dispatchLevel = 0U;
}


void pharosIPartition2CoreInitialize(void)
{
    /* iterator of the core */
    CoreNumber i;

    /* iterator of the overall partitions */
    PartitionNumber iterator = 0;

    /* iterator of the partition inside one core */
    PartitionNumber j;


    /* for each core */
    for(i = 0; i < pharosVCoreTable.size; i++)
    {
        /* for each partition inside the core */
        for(j = 0; j < pharosVCoreTable.cores[i].partitions.size; j++)
        {
            /* set the partition has belonging to the core */
            partition2Core[iterator] = i;

            /* increment the partition */
            iterator++;
        }
    }
}


void pharosICoreSynchronize(void)
{
    /* core iterator */
    CoreNumber iterator;


    /* signal that this core is ready */
    coreIsReady[pharosICoreRunning()] = PHAROS_CORE_IS_READY;

    /* wait for each core */
    for(iterator = 0; iterator < PHAROS_NUMBER_CORES; iterator++)
    {
        /* wait until the core is ready */
        while(coreIsReady[iterator] != PHAROS_CORE_IS_READY);
    }
}


void pharosIMulticoreInterruptHandler(void)
{
    /* node */
    ptrPharosMpscNode node;

    /* get the RMP of the core */
    ptrPharosCoreConfiguration conf = &pharosICore()->rmp;

    /* get the MPSC queue */
    ptrPharosMpscQueue queue = &conf->queue;


    /* get the first node from the queue */
    node = pharosIMpscQueuePop(queue);

    /* while there are messages to process */
    while(node != NULL)
    {
        /* check if it is a hw queue send message type */
        if(node->type == PHAROS_RMP_MESSAGE_TYPE_HWQUEUE_SEND)
        {
            /* get the message sent */
            ptrPharosRmpMessageHwQueueSend m = node->message.hwqueuesend;

            /* send the message on the local core */
            pharosIHwQueueSendOnQueueCore(m->queue , m->message , m->priority);
        }
            /* check if it is a future ready message type */
        else if(node->type == PHAROS_RMP_MESSAGE_TYPE_FUTURE_READY)
        {
            /* get the message sent */
            ptrPharosRmpMessageFutureReady m = node->message.futureReady;

            /* check if ready is actually a reset */
            if(m->isReset == FALSE)
            {
                /* ready the future on the local core */
                pharosIFutureReadyOnOwnerCore(m->future , &m->arg);
            }
            /* it is a reset */
            else
            {
                /* so ready the future on the local core with a NULL argument */
                pharosIFutureReadyOnOwnerCore(m->future , NULL);
            }
        }
            /* check if it is a hw queue message free type */
        else if(node->type == PHAROS_RMP_MESSAGE_TYPE_HWQUEUE_FREE)
        {
            /* get the message sent */
            ptrPharosRmpMessageHwQueueFree m = node->message.hwqueueFree;

            /* free the message on the local core */
            pharosIHwQueueFreeMessageLocal(m->queue , m->message);
        }
            /* check if it is a channel send */
        else if(node->type == PHAROS_RMP_MESSAGE_TYPE_CHANNEL_SEND)
        {
            /* get the message sent */
            ptrPharosRmpMessageChannelSend m = node->message.channelSend;

            /* send the channel message in the local core */
            pharosIChannelSendRmp(m->channel , m->bulk);
        }
            /* check if a partition restart signal */
        else if(node->type == PHAROS_RMP_MESSAGE_TYPE_PARTITION_RESTART_SIGNAL)
        {
            /* get the message sent */
            ptrPharosRmpMessagePartitionRestartSignal m = node->message.partitionRestartSignal;

            /* announce to this core that a remote partition is being restarted */
            pharosIPartitionRestartSignalOnPartitionCore(m->partition);
        }
            /* check if a stop all partition external threads */
        else if(node->type == PHAROS_RMP_MESSAGE_TYPE_PARTITION_EXTERNAL_THREADS_STOP)
        {
            /* get the message sent */
            ptrPharosRmpMessagePartitionExternalThreadsStop m = node->message.partitionExternalThreadsStop;

            /* announce to this core that a partition restart was made in another core so this core must stop any thread accessing it */
            pharosIPartitionExternalThreadsStopOnPartitionCore(pharosICoreRunning() , m->partition);
        }
            /* if a channel receive message */
        else if(node->type == PHAROS_RMP_MESSAGE_TYPE_CHANNEL_RECEIVE)
        {
            /* get the message sent */
            ptrPharosRmpMessageChannelReceive m = node->message.channelReceive;

            /* announce to this core that a thread on a different core is waiting for a channel bulk with no "force
             * activation" since it must be done on the thread core (clock tick on different cores may be completely 
             * different) */
            pharosIChannelReceiveOnChannelCore(m->thread , m->channel , m->priority , m->timeout);
        }
            /* if a channel receive return message */
        else if(node->type == PHAROS_RMP_MESSAGE_TYPE_CHANNEL_RECEIVE_RETURN)
        {
            /* get the message sent */
            ptrPharosRmpMessageChannelReceiveReturn m = node->message.channelReceiveReturn;

            /* channel receive remote has ended */
            pharosIChannelReceiveReturnOnReceiverCore(m->thread , m->bulk , m->returnValue);
        }
        else if(node->type == PHAROS_RMP_MESSAGE_TYPE_SCHEDULER_THREAD_READY)
        {
            /* get the message sent */
            ptrPharosRmpMessageThreadReady m = node->message.threadReady;

            /* ready the thread */
            pharosISchedAddReadyThread(m->thread);
        }
        else if(node->type == PHAROS_RMP_MESSAGE_TYPE_SCHEDULER_THREAD_UPDATE_PRIORITY)
        {
            /* get the message sent */
            ptrPharosRmpMessageSchedulerThreadUpdatePriority m = node->message.threadPriority;

            /* ready the thread */
            pharosIThreadQueueUpdateThreadPrioOnSchedulerCore(m->thread , m->newPriority);
        }
        else if(node->type == PHAROS_RMP_MESSAGE_TYPE_THREAD_ACTIVATE)
        {
            /* get the message sent */
            ptrPharosRmpMessageThreadActivate m = node->message.threadActivate;

            /* ready the thread */
            pharosITimeActivateThreadRemote(m->thread);
        }
            /* if not anything else, must be a channel bulk free */
        else
        {
            /* get the message sent */
            ptrPharosRmpMessageChannelBulkFree m = node->message.channelFree;

            /* send the channel bulk free message in the local core */
            pharosIChannelBulkFreeOnQueueCore(m->bulk);
        }

        /* get the next message to process (if any) */
        node = pharosIMpscQueuePop(queue);
    }
}
