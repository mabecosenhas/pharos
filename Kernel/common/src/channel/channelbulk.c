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


#include <pharos/channel.h>
#include <pharos/buffer.h>
#include <pharos/kernel/channel/channel.h>
#include <pharos/kernel/channel/channelbulkkernel.h>
#include <pharos/kernel/channel/channelbulk.h>
#include <pharos/kernel/scheduler/scheduler.h>
#include <pharos/kernel/scheduler/schedulerinline.h>
#include <pharos/kernel/thread/dispatcher.h>
#include <pharos/kernel/thread/sporadicthread.h>
#include <pharos/kernel/object/object.h>
#include <pharos/kernel/object/protection.h>
#include <pharos/kernel/interrupt/interrupt.h>
#include <pharos/kernel/partition/partition.h>
#include <pharos/kernel/core/coreinlines.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/kernel/assert/assert.h>
#include <pharos/hal/cpu/sections.h>
#include <pharos/hal/boardApi.h>



/**
 * Gets the channel bulk queue from the running partition
 * 
 * @param number number of the channel bulk queue on the running partition
 * 
 * @return returns a pointer to the channel bulk queue or NULL if invalid
 */
static ptrChannelBulkQueue getBulkQueueLocal(ChannelBulkQueueNumber number);


/**
 * Initialize the bulk 
 * 
 * @constraint calling thread is assumed to be on the specified partition
 * 
 * @param queue queue where the bulk belongs to
 * @param bulkKernel bulk kernel to initialize
 * @param bulk application bulk to initialize
 * @param partition partition that will own the bulk (if initialization goes well)
 * @return returns PHAROS_CHANNEL_BULK_QUEUE_GET_MMU_INVALID or PHAROS_CHANNEL_BULK_QUEUE_GET_SUCCESS depending if an
 * error occurs or everything goes well
 */
static PharosChannelBulkQueueGetR bulkInit(ptrChannelBulkQueue queue , ptrChannelBulkKernel bulkKernel ,
                                           ptrChannelBulk bulk , ptrPartition partition);


void pharosIPartitionChannelBulksReset(ptrPartition p)
{
    /* interrupt level */
    PharosInterruptLevel level;

    /* bulk iterator */
    ptrChannelBulkKernel bulk;

    /* bulk queue iterator */
    ChannelBulkQueueNumber i;

    /* bulk iterator */
    uint32_t j;

    ptrChannelBulkQueue queue;


    /* for every bulk queue of this partition */
    for(i = 0; i < p->objects.channelBulkTable.size; i++)
    {
        /* get the queue */
        queue = &p->objects.channelBulkTable.channelBulkQueues[i];

        /* clear all the list */
        simpleListClear(&queue->freeBulks);

        /* for every bulk */
        for(j = 0; j < queue->numberBulks; j++)
        {
            /* get the bulk */
            bulk = &queue->bulks[j];

            /* if the bulk is held by the partition */
            if((bulk->holder == p) || (bulk->holder == NULL))
            {
                /* disable interrupts to keep coherency between the list of bulks held by the partition 
                 * (could happen that while the partition is restarting a bulk was sent by another core) */
                pharosCpuDisableInterrupts(&level);

                /* free the bulk by placing it on the list of free bulks of its queue */
                simpleListAppendLast(&queue->freeBulks , bulk);

                /* re-enable interrupts */
                pharosCpuEnableInterrupts(&level);

                /* reset the holder */
                bulk->holder = NULL;
            }

            /* else, the bulk is either own by no one (already on the freeBulks list) or is held by another partition, 
             * in which case we let it remain there */
        }
    }

    /* disable interrupts to keep coherency between the list of bulks held by the partition 
     * (could happen that while the partition is restarting a bulk was sent by another core) */
    pharosCpuDisableInterrupts(&level);

    /* get the first bulk currently held by the partition */
    bulk = (ptrChannelBulkKernel) linkedListGetFirst(&p->externalChannelBulksHold);

    /* re-enable interrupts */
    pharosCpuEnableInterrupts(&level);

    /* while there are bulks held by this partition */
    while(bulk != NULL)
    {
        /* free the bulk (return it to the owner partition) */
        pharosIChannelBulkFree(bulk);

        /* disable interrupts to keep coherency between the list of bulks held by the partition 
         * (could happen that while the partition is restarting a bulk was sent by another core) */
        pharosCpuDisableInterrupts(&level);

        /* get the next bulk */
        bulk = (ptrChannelBulkKernel) linkedListGetFirst(&p->externalChannelBulksHold);

        /* re-enable interrupts */
        pharosCpuEnableInterrupts(&level);
    }

    /* at this point we have free any external bulk that this partition has */

    /* now we have to free the bulks that this partition holds and their are their own */
}


void pharosIChannelBulkQueuesInit(const ptrPartition partition , ptrChannelBulkQueueTable table)
{
    /* buffer iterator */
    uint8_t *bufferIterator;

    /* iterator of the bulk queue */
    ChannelBulkQueueNumber i;

    /* iterator of the bulk number */
    uint32_t j;

    /* bulk size */
    uint32_t bulkSize;

    /* bulk to initialize */
    ptrChannelBulkKernel bulk;


    /* for each bulk queue */
    for(i = 0U; i < table->size; i++)
    {
        /* get the channel */
        ptrChannelBulkQueue queue = &table->channelBulkQueues[i];

        /* initialize the list of free bulks inside the queue */
        simpleListInitialize(&queue->freeBulks , OFFSETOF(ChannelBulkKernel , next));

        /* initialize the channel bulk queue */
        bufferIterator = pharosCpuChannelBulkQueueInit(queue , &bulkSize);

        /* initialize the map of blocked threads on the simple semaphore */
        pharosIThreadQueueInitialize(&queue->blockedQueue , queue->flags.isFifo);

        /* set the partition where the queue belongs to */
        queue->owner = partition;

        /* for every bulk */
        for(j = 0U; j < queue->numberBulks; j++)
        {
            /* get the bulk address */
            bulk = &queue->bulks[j];

            /* check that the bulk configuration is correct */
            PHAROS_ASSERT(pharosCpuChannelBulkAddressValid(bulk) == TRUE ,
                          "Bulk queue configuration for partition %d, queue %d is incorrect.\r\n"
                          "Please check if the bulks are in the BULK_SECTION section and/or the number of bulks is correct" , partition->id , i);

            /* initialize the buffer pointer of the bulk */
            bulk->bulkFullBuffer.buffer = bufferIterator;

            /* initialize the size */
            bulk->bulkFullBuffer.size = bulkSize;

            /* initialize the queue */
            bulk->queue = queue;

            /* there is no filter */
            bulk->future = NULL;

            /* reset the next bulk with the same priority */
            bulk->nextPriority = NULL;

            /* add the bulk to the list of free bulks */
            simpleListAppendFirst(&queue->freeBulks , bulk);

            /* initialize the holder to none (no one can use the bulk until it is got or received) */
            bulk->holder = NULL;

            /* increase the buffer iterator to point to the next page */
            bufferIterator += bulkSize;
        }
    }
}


void pharosIChannelBulkInitialize(ptrChannelBulk bulk , ptrChannelBulkKernel bulkKernel)
{
    /* set the address of the kernel bulk */
    bulk->kernelBulk = bulkKernel;

    /* initialize the buffer to the start of the bulk */
    bulk->bufferFull = bulkKernel->bufferMessage;

    /* fill the message buffer to occupy the full bulk buffer */
    bulk->bufferMessage = bulkKernel->bufferMessage;

    /* make the extra buffer point at the end */
    bulk->bufferExtra.buffer = bulk->bufferFull.buffer + bulk->bufferFull.size;

    /* and with zero-size */
    bulk->bufferExtra.size = 0;
}


PharosChannelBulkQueueGetR bulkInit(ptrChannelBulkQueue queue , ptrChannelBulkKernel bulkKernel , ptrChannelBulk bulk ,
                                    ptrPartition partition)
{
    /* interrupt level */
    PharosInterruptLevel level;

    /* return value of this method */
    PharosChannelBulkQueueGetR result;


    /* check that the calling thread belongs to the partition */
    PHAROS_ASSERT(pharosIPartitionGetRunning() == partition , "Invalid partition");

    /* disable interrupts to keep coherency between the list of bulks held by the partition and the bulk itself 
     * (when doing a partition restart they should match) */
    pharosCpuDisableInterrupts(&level);

    /* if not running on a partition with supervisor permissions then add the channel bulk */
    if((pharosIPartitionHasSupervisor() == FALSE) && (pharosCpuChannelBulkAdd(queue , bulkKernel , partition) == 0U))
    {
        /* return the bulk since it cannot be used */

        /* free the bulk by placing it on the list of free bulks of its queue */
        simpleListAppendLast(&queue->freeBulks , bulkKernel);

        /* re-enable interrupts */
        pharosCpuEnableInterrupts(&level);

        /* return error */
        result = PHAROS_CHANNEL_BULK_QUEUE_GET_MMU_INVALID;
    }
        /* if could add the bulk */
    else
    {
        /* copy the full buffer to the buffer */
        bulkKernel->bufferMessage = bulkKernel->bulkFullBuffer;

        /* reset the next bulk with the same priority */
        bulkKernel->nextPriority = NULL;

        /* reset the flag */
        bulkKernel->flags.responseIsReady = FALSE;

        /* reset the flag */
        bulkKernel->flags.isResponse = FALSE;

        /* reset the flag */
        bulkKernel->flags.partitionRestart = FALSE;

        /* reset the response thread waiting (to none) */
        bulkKernel->responseWaiter = NULL;

        /* reset the future */
        bulkKernel->future = NULL;

        /* initialize the buffers of the bulk */
        pharosIChannelBulkInitialize(bulk , bulkKernel);

        /* add the bulk to the partition */
        pharosIChannelBulkAdd2PartitionOnPartitionCore(partition , bulkKernel);

        /* re-enable interrupts */
        pharosCpuEnableInterrupts(&level);

        /* return success */
        result = PHAROS_CHANNEL_BULK_QUEUE_GET_SUCCESS;
    }

    /* return success or invalid MMU tables */
    return result;
}


void pharosIChannelBulkAdd2PartitionOnPartitionCore(ptrPartition partition , ptrChannelBulkKernel bulk)
{
    /* get the future */
    ptrFuture future = bulk->future;


    /* check that the bulk is valid */
    PHAROS_ASSERT(bulk != NULL , "Bulk is invalid");

    /* check that the partition is valid */
    PHAROS_ASSERT(partition != NULL , "Partition is invalid");

    /* check if the running core is correct */
    PHAROS_ASSERT_CORE(partition->core->number , "Running on different core");

    /* only for external bulks */
    if(partition != bulk->queue->owner)
    {
        /* add the bulk to the receiver partition */
        linkedListAppendFirst(&partition->externalChannelBulksHold , bulk);
    }

    /* if there is a future */
    if(future != NULL)
    {
        /* remove access to the future (no partition at this point can access the future) */
        pharosIFutureUpdateHolder(future , partition);

        if(partition != future->owner)
        {
            /* add the future to the channel owner partition */
            linkedListAppendFirst(&partition->externalFuturesHold , future);
        }
    }

    /* update the holder */
    bulk->holder = partition;
}


void pharosIChannelBulkRemoveFromHolderOnPartitionCore(ptrChannelBulkKernel bulk)
{
    /* get the future of the bulk*/
    ptrFuture future = bulk->future;


    /* check that the bulk is valid */
    PHAROS_ASSERT(bulk != NULL , "Bulk is invalid");

    /* if the partition that currently holds the bulk is valid */
    PHAROS_ASSERT(bulk->holder != NULL , "Bulk holder is invalid");

    /* check that running core is correct */
    PHAROS_ASSERT_CORE(bulk->holder->core->number , "Removing bulk from different core");

    /* make sure the holder is not the owner */
    if(bulk->holder != bulk->queue->owner)
    {
        /* remove the bulk from its current holder list and add it to new holder */
        linkedListRemoveObject(&bulk->holder->externalChannelBulksHold , bulk);
    }

    /* if there is a future */
    if(future != NULL)
    {
        if(bulk->holder != future->owner)
        {
            /* remove the future from the running partition list of external futures */
            linkedListRemoveObject(&bulk->holder->externalFuturesHold , future);
        }
    }

    /* right now no one has the bulk */
    bulk->holder = NULL;
}


PharosChannelBulkQueueGetR pharosSChannelBulkQueueGet(ChannelBulkQueueNumber number , ptrChannelBulk bulk ,
                                                      Timeout timeout)
{
    /* result of the method */
    PharosChannelBulkQueueGetR result;

    /* interrupt level */
    PharosInterruptLevel level;

    /* bulk queue to get the bulk from */
    ptrChannelBulkQueue queue;

    /* bulk on the kernel */
    ptrChannelBulkKernel bulkKernel;

    /* running partition */
    ptrPartition partitionRunning = pharosIPartitionGetRunning();

    /* calling thread */
    ptrThreadInfo runningThread = pharosISchedGetRunningThread();

    /* get the running protection environment */
    ptrEnvMemoryProtection prot = pharosIPartitionRunningMemProt();


    /* get the queue of bulks on the running thread partition */
    queue = getBulkQueueLocal(number);

    /* if the queue is invalid */
    if(queue == NULL)
    {
        /* return invalid queue */
        result = PHAROS_CHANNEL_BULK_QUEUE_GET_INVALID;
    }
        /* if the bulk is not inside the running thread partition */
    else if(pharosCpuBufferIsInsideProtection((uint8_t *) bulk , sizeof (ChannelBulk) , prot) == FALSE)
    {
        /* return invalid bulk */
        result = PHAROS_CHANNEL_BULK_QUEUE_GET_INVALID_BULK;
    }
        /* if there is a timeout (could block) and running inside an interrupt */
    else if(timeout != PHAROS_TIMEOUT_ZERO && pharosIInterruptIsRunning() == TRUE)
    {
        /* return error */
        result = PHAROS_CHANNEL_BULK_QUEUE_GET_INSIDE_INTERRUPT;
    }
    else
    {
        /* disable interrupts so no one can change the list of free bulks */
        pharosCpuDisableInterrupts(&level);

        /* get the last bulk on the channel */
        bulkKernel = (ptrChannelBulkKernel) simpleListRemoveFirst(&queue->freeBulks);

        /* if a bulk is available */
        if(bulkKernel != NULL)
        {
            /* re-enable interrupts */
            pharosCpuEnableInterrupts(&level);

            /* set the bulk */
            result = bulkInit(queue , bulkKernel , bulk , partitionRunning);
        }
            /* if not waiting */
        else if(timeout == PHAROS_TIMEOUT_ZERO)
        {
            /* re-enable interrupts */
            pharosCpuEnableInterrupts(&level);

            /* the queue is empty */
            result = PHAROS_CHANNEL_BULK_QUEUE_GET_EMPTY;
        }
            /* if waiting forever */
        else if(timeout == PHAROS_TIMEOUT_INFINITE)
        {
            /* going to block until the semaphore is released to this thread */
            pharosIThreadQueueAddBlocked(&queue->blockedQueue , runningThread);

            /* block the thread on the counting semaphore */
            pharosIThreadStateBlockChannelBulk(&runningThread->state);

            /* remove the thread from the ready list */
            pharosISchedRemoveReadyThread(runningThread);

            /* re-enable interrupts */
            pharosCpuEnableInterrupts(&level);

            /* dispatch to highest priority ready thread */
            pharosIDispatchThread();

            /* since the timeout is infinite, if we get here then the bulk was obtained */

            /* initialize the bulk */
            bulkKernel = runningThread->unblockInfo.channelBulk;

            /* set the bulk */
            bulkInit(queue , bulkKernel , bulk , partitionRunning);

            /* return success */
            result = PHAROS_CHANNEL_BULK_QUEUE_GET_SUCCESS;
        }
            /* if waiting with a timeout */
        else
        {
            /* going to block until the semaphore is released to this thread */
            pharosIThreadQueueAddBlocked(&queue->blockedQueue , runningThread);

            /* block the thread on the counting semaphore */
            pharosIThreadStateBlockChannelBulk(&runningThread->state);

            /* block the thread on a timeout */
            pharosIThreadStateWaitChannelBulk(&runningThread->state);

            /* remove the thread from the ready list */
            pharosISchedRemoveReadyThread(runningThread);

            /* reset the unblock information */
            runningThread->threadFlags.unblockDueToTimeout = FALSE;

            /* add a tick handler at the timeout instant */
            pharosITimeTickAddThread(runningThread , timeout + pharosIClockTicksGet(runningThread->scheduler) ,
                                     PHAROS_TIME_TICK_AWAKEN_THREAD_CHANNEL_BULK_GET_TIMEOUT , runningThread->scheduler);

            /* re-enable interrupts*/
            pharosCpuEnableInterrupts(&level);

            /* dispatch to highest priority ready thread */
            pharosIDispatchThread();

            /* get the result of the timeout (bulk was free and given to this thread?) */
            if(runningThread->threadFlags.unblockDueToTimeout == TRUE)
            {
                /* there was a timeout */
                result = PHAROS_CHANNEL_BULK_QUEUE_GET_TIMEOUT;
            }
            else
            {
                /* initialize the bulk */
                bulkKernel = runningThread->unblockInfo.channelBulk;

                /* set the bulk */
                bulkInit(queue , bulkKernel , bulk , partitionRunning);

                /* thread got a bulk before the timeout expired */
                result = PHAROS_CHANNEL_BULK_QUEUE_GET_SUCCESS;
            }
        }
    }

    /* return the method result */
    return result;
}


void pharosIChannelBulkFreeOnQueueCore(ptrChannelBulkKernel bulk)
{
    /* interrupt level */
    PharosInterruptLevel level;

    /* highest thread that was blocked on the counting semaphore */
    ptrThreadInfo highestThread;

    /* get the bulk queue */
    ptrChannelBulkQueue queue = bulk->queue;


    /* check that the bulk is valid */
    PHAROS_ASSERT(bulk != NULL , "Bulk is invalid");

    /* check the bulk queue */
    PHAROS_ASSERT(bulk->queue != NULL , "Bulk queue is invalid");

    /* check that running core is correct 
     * note: the bulk core is also the highestThread core so we can ready the thread on this function */
    PHAROS_ASSERT_CORE(bulk->queue->owner->core->number , "Invalid core");

    /* disable interrupts so no one can change the list of free bulks */
    pharosCpuDisableInterrupts(&level);

    /* if the partition that holds the bulk queue is working normally */
    if(queue->owner->state == PHAROS_PARTITION_STATE_NOMINAL)
    {
        /* remove the highest priority blocked thread on the queue */
        highestThread = pharosIThreadQueueGetHighest(&queue->blockedQueue);

        /* reset the future of the bulk */
        bulk->future = NULL;

        /* if there is no thread waiting for the bulk */
        if(highestThread == NULL)
        {
            bulk->holder = NULL;

            /* free the bulk by placing it on the list of free bulks of its queue */
            simpleListAppendLast(&queue->freeBulks , bulk);

            /* interrupts can now be enabled */
            pharosCpuEnableInterrupts(&level);
        }
        else
        {
            /* check that the thread belongs to the running core */
            PHAROS_ASSERT_CORE(highestThread->partition->core->number , "Invalid core");

            /* set the new holder */
            bulk->holder = highestThread->partition;

            /* give the bulk to the unblocked thread */
            highestThread->unblockInfo.channelBulk = bulk;

            /* remove the highest thread from the queue of threads blocked on the semaphore */
            pharosIThreadQueueRemoveBlocked(highestThread);

            /* remove the thread timeout time tick (if needed) */
            pharosITimeTickRemoveThread(highestThread);

            /* unblock the thread state */
            pharosIThreadStateUnblock(&highestThread->state);

            /* remove the wait state from the thread state */
            pharosIThreadStateUnwait(&highestThread->state);

            /* activate the thread */
            pharosISchedAddReadyThread(highestThread);

            /* can re-enable interrupts now */
            pharosCpuEnableInterrupts(&level);

            /* dispatch */
            pharosIDispatchThread();
        }
    }
        /* if the partition that holds the bulk queue is not working normally (restarting, stopped or starting up) */
    else
    {
        /* then we will not give the bulk to the highest priority waiting thread and instead we will place it on the 
         * free bulk list */

        /* free the bulk by placing it on the list of free bulks of its queue */
        simpleListAppendLast(&queue->freeBulks , bulk);

        /* interrupts can now be enabled */
        pharosCpuEnableInterrupts(&level);
    }
}


PharosChannelBulkFreeR pharosSChannelBulkFree(ptrChannelBulk bulk)
{
    /* result of the method */
    PharosChannelBulkFreeR result;

    /* interrupt level */
    PharosInterruptLevel level;

    /* running protection environment */
    ptrEnvMemoryProtection prot;

    /* get the running partition */
    ptrPartition runningPartition = pharosIPartitionGetRunning();


    PHAROS_ASSERT(runningPartition != NULL , "Running partition is invalid");

    /* get the running protection environment */
    prot = pharosIPartitionMemoryProt(runningPartition);

    /* if the bulk is not inside the running thread partition */
    if(pharosCpuBufferIsInsideProtection((uint8_t *) bulk , sizeof (ChannelBulk) , prot) == FALSE)
    {
        /* return invalid bulk */
        result = PHAROS_CHANNEL_BULK_FREE_INVALID_BULK_ADDRESS;
    }
        /* check if the bulk trying to be free is valid (at a valid address and owned by the partition) */
    else if(pharosIChannelBulkKernelValid(runningPartition , bulk->kernelBulk) == FALSE)
    {
        /* bulk is invalid */
        result = PHAROS_CHANNEL_BULK_FREE_INVALID_BULK;
    }
    else
    {
        /* if not running on a partition with supervisor permissions */
        if(pharosIPartitionHasSupervisor() == FALSE)
        {
            /* disable interrupts so no one can change the list of channel bulks held */
            pharosCpuDisableInterrupts(&level);

            /* remove the permissions to the bulk */
            pharosCpuChannelBulkRemove(bulk->kernelBulk , runningPartition);

            /* can re-enable interrupts now */
            pharosCpuEnableInterrupts(&level);
        }

        /* free the bulk on this core or another */
        pharosIChannelBulkFree(bulk->kernelBulk);

        /* return success */
        result = PHAROS_CHANNEL_BULK_FREE_SUCCESS;
    }

    /* return the method result */
    return result;
}


ptrChannelBulkQueue getBulkQueueLocal(ChannelBulkQueueNumber number)
{
    /* result of the method */
    ptrChannelBulkQueue result;

    /* get the table of channels of the channel environment */
    ptrChannelBulkQueueTable table = &pharosIPartitionRunObjectTable()->channelBulkTable;


    /* if the channel bulk queue number is too high */
    if((table == NULL) || (number >= table->size))
    {
        /* return invalid */
        result = NULL;
    }
        /* if the number is OK */
    else
    {
        /* return the pointer to the channel bulk queue */
        result = &table->channelBulkQueues[number];
    }

    /* return the result */
    return result;
}


bool pharosIChannelBulkBufferValid(ptrChannelBulkKernel bulkKernel , ptrBuffer buffer)
{
    /* return value of this method */
    bool result;

    /* calculate the end address of the bulk page */
    uint8_t *endBulk = bulkKernel->bulkFullBuffer.buffer + bulkKernel->bulkFullBuffer.size;


    /* check if the buffer is all inside the bulk page */
    if((buffer->buffer >= bulkKernel->bulkFullBuffer.buffer) && (buffer->buffer + buffer->size <= endBulk))
    {
        /* it is */
        result = TRUE;
    }
    else
    {
        /* it isn't */
        result = FALSE;
    }

    /* return the result */
    return result;
}


bool pharosIChannelBulkKernelValid(ptrPartition partition , ptrChannelBulkKernel bulk)
{
    /* method result */
    bool result;

    /* interrupt level */
    PharosInterruptLevel level;


    /* disable interrupts so no one can change the holder of the bulk */
    pharosCpuDisableInterrupts(&level);

    /* if the bulk is not inside the bulks area */
    if(pharosCpuChannelBulkAddressValid(bulk) == FALSE)
    {
        /* bulk must be inside the kernel and address correctly aligned */
        result = FALSE;
    }
        /* if the bulk does not belong to the holder */
    else if(partition != bulk->holder)
    {
        /* then cannot send the bulk */
        result = FALSE;
    }
    else
    {
        /* bulk is valid */
        result = TRUE;
    }

    /* re-enable interrupts */
    pharosCpuEnableInterrupts(&level);

    /* return TRUE if the bulk is valid and FALSE otherwise */
    return result;
}


bool pharosIChannelBulkValid(ptrChannelBulk bulk)
{
    /* return value */
    bool result;


    /* check that the full buffer matches the bulk full buffer */
    if(pharosBufferCompare(&bulk->bufferFull , &bulk->kernelBulk->bulkFullBuffer) != 0)
    {
        /* the bulk is not valid */
        result = FALSE;
    }
        /* check that the message buffer is inside the full buffer */
    else if(pharosBufferIsInside(&bulk->bufferFull , &bulk->bufferMessage) == FALSE)
    {
        /* the bulk is not valid */
        result = FALSE;
    }
        /* check that the message buffer is inside the full buffer */
    else if(pharosBufferIsInside(&bulk->bufferFull , &bulk->bufferExtra) == FALSE)
    {
        /* the bulk is not valid */
        result = FALSE;
    }
    else
    {
        /* the bulk is valid */
        result = TRUE;
    }

    /* return TRUE if the bulk is valid and FALSE otherwise */
    return result;
}
