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


#include <pharos/kernel/bulk/bulk.h>
#include <pharos/kernel/thread/thread.h>
#include <pharos/bulk.h>
#include <pharos/hal/board.h>
#include <pharos/kernel/partition/partition.h>
#include <pharos/hal/hal.h>
#include <pharos/hal/boardApi.h>
#include <pharos/buffer.h>
#include <pharos/kernel/bulk/bulk.h>
#include <pharos/kernel/thread/dispatcher.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/kernel/scheduler/schedulerinline.h>


/**
 * Gets the bulk queue given its id
 * 
 * @param id bulk queue id 
 * 
 * @return returns a pointer to the queue (or NULL if none)
 */
static ptrBulkQueue getBulkQueue(BulkQueueNumber number);


/**
 * Check if the address of the specified bulk is a valid bulk inside the specified queue
 * 
 * @param queue queue to check
 * @param bulk bulk to analyze
 * 
 * @return returns TRUE if the bulk is valid and FALSE otherwise
 */
static bool pharosIBulkInsideQueue(ptrBulkQueue queue , ptrBulk bulk);


PharosBulkGetR pharosSBulkGet(BulkQueueNumber number , ptrBulk *bulkDestination , ptrBuffer buffer , Timeout timeout)
{
    /* result of the function */
    PharosBulkGetR result;

    /* interrupt level */
    PharosInterruptLevel level;

    /* queue pointer */
    ptrBulkQueue queue;

    /* calling thread */
    ptrThreadInfo runningThread = pharosISchedGetRunningThread();

    
    /* get the queue */
    queue = getBulkQueue(number);

    /* if the queue is not valid */
    if(queue == NULL)
    {
        /* return invalid */
        result = PHAROS_BULK_GET_INVALID;
    }
        /* if there is a timeout (could block) and running inside an interrupt */
    else if(timeout != PHAROS_TIMEOUT_ZERO && pharosIInterruptIsRunning() == TRUE)
    {
        /* return error */
        result = PHAROS_BULK_GET_INSIDE_INTERRUPT;
    }
        /* check if the bulk address can be written on */
    else if(pharosCpuBufferIsInsideProtection((uint8_t *) bulkDestination , sizeof (ptrBulk) , pharosIPartitionRunningMemProt()) == FALSE)
    {
        /* bulk is not part of the calling thread memory area */
        result = PHAROS_BULK_GET_INVALID_BULK;
    }
        /* check if the buffer address can be written on */
    else if(pharosCpuBufferIsInsideProtection((uint8_t *) buffer , sizeof (Buffer) , pharosIPartitionRunningMemProt()) == FALSE)
    {
        /* buffer is not part of the calling thread memory area */
        result = PHAROS_BULK_GET_INVALID_BUFFER;
    }
        /* queue is valid */
    else
    {
        /* don't allow anyone inside threads or interrupts to change the simple list */
        pharosCpuDisableInterrupts(&level);

        /* get the first item of the unused bulks */
        ptrBulk bulk = (ptrBulk) simpleListRemoveFirst(&queue->freeBulks);

        /* check if there was a bulk available */
        if(bulk != NULL)
        {
            /* interrupts can now be enabled */
            pharosCpuEnableInterrupts(&level);

            /* initialize the bulk */
            *bulkDestination = bulk;

            /* re-initialize the bulk size since it could have been re-written when using lw queues for example 
             * (with the message size instead of the bulk size) */
            (*bulkDestination)->bulk.buffer.size = queue->bulkSize;

            /* copy the buffer so the user can use it */
            *buffer = (*bulkDestination)->bulk.buffer;

            /* return success */
            result = PHAROS_BULK_GET_SUCCESS;
        }
            /* if not waiting */
        else if(timeout == PHAROS_TIMEOUT_ZERO)
        {
            /* interrupts can now be enabled */
            pharosCpuEnableInterrupts(&level);

            /* bulk not available */
            result = PHAROS_BULK_GET_UNAVAILABLE;
        }
            /* if waiting forever */
        else if(timeout == PHAROS_TIMEOUT_INFINITE)
        {
            /* going to block until the semaphore is released to this thread */
            pharosIThreadQueueAddBlocked(&queue->blockedQueue , runningThread);

            /* block the thread on the counting semaphore */
            pharosIThreadStateBlockBulk(&runningThread->state);

            /* remove the thread from the ready list */
            pharosISchedRemoveReadyThread(runningThread);

            /* re-enable interrupts */
            pharosCpuEnableInterrupts(&level);

            /* dispatch to highest priority ready thread */
            pharosIDispatchThread();

            /* since the timeout is infinite, if we get here then the bulk was obtained */

            /* initialize the bulk */
            *bulkDestination = runningThread->unblockInfo.bulk;

            /* copy the buffer so the user can use it */
            *buffer = (*bulkDestination)->bulk.buffer;

            /* re-initialize the bulk size since it could have been re-written when using lw queues for example 
             * (with the message size instead of the bulk size) */
            (*bulkDestination)->bulk.buffer.size = queue->bulkSize;

            /* return success */
            result = PHAROS_BULK_GET_SUCCESS;
        }
            /* if waiting with a timeout */
        else
        {
            /* going to block until the semaphore is released to this thread */
            pharosIThreadQueueAddBlocked(&queue->blockedQueue , runningThread);

            /* block the thread on the counting semaphore */
            pharosIThreadStateBlockBulk(&runningThread->state);

            /* block the thread on a timeout */
            pharosIThreadStateWaitBulk(&runningThread->state);

            /* remove the thread from the ready list */
            pharosISchedRemoveReadyThread(runningThread);

            /* reset the unblock information */
            runningThread->threadFlags.unblockDueToTimeout = FALSE;

            /* add a tick handler at the timeout instant */
            pharosITimeTickAddThread(runningThread , timeout + pharosIClockTicksGet(runningThread->scheduler) ,
                                     PHAROS_TIME_TICK_AWAKEN_THREAD_BULK_TIMEOUT , runningThread->scheduler);

            /* re-enable interrupts*/
            pharosCpuEnableInterrupts(&level);

            /* dispatch to highest priority ready thread */
            pharosIDispatchThread();

            /* get the result of the timeout (bulk was free and given to this thread?) */
            if(runningThread->threadFlags.unblockDueToTimeout == TRUE)
            {
                /* there was a timeout */
                result = PHAROS_BULK_GET_TIMEOUT;
            }
            else
            {
                /* initialize the bulk */
                *bulkDestination = runningThread->unblockInfo.bulk;

                /* copy the buffer so the user can use it */
                *buffer = (*bulkDestination)->bulk.buffer;

                /* re-initialize the bulk size since it could have been re-written when using lw queues for example 
                 * (with the message size instead of the bulk size) */
                (*bulkDestination)->bulk.buffer.size = queue->bulkSize;

                /* thread got a bulk before the timeout expired */
                result = PHAROS_BULK_GET_SUCCESS;
            }
        }
    }


    /* return the result of the function */
    return result;
}


PharosBulkGiveR pharosSBulkGive(ptrBulk bulk)
{
    /* result of the function */
    PharosBulkGetR result;

    /* interrupt level */
    PharosInterruptLevel level;

    /* queue pointer */
    ptrBulkQueue queue;

    /* highest thread that was blocked on the counting semaphore */
    ptrThreadInfo highestThread;


    /* if the address of the bulk is not inside the kernel */
    if(pharosCpuAddressInsideKernel((uint8_t *) bulk) == FALSE)
    {
        /* return invalid bulk */
        result = PHAROS_BULK_GIVE_INVALID_BULK;
    }
    else
    {
        /* get the queue from the bulk (at this point we only know that the bulk is inside the kernel memory area so 
         * at least bulk->bulk.number has a valid address that we can check */
        queue = getBulkQueue(bulk->bulk.number);

        /* check if the queue is not valid */
        if(queue == NULL)
        {
            /* return invalid */
            result = PHAROS_BULK_GIVE_INVALID_BULK;
        }
            /* if the bulk is not valid. At this point we check that the queue/bulk have a good match.
             * if there was an "attack" and by some change the bulk has a correct queue number, now we check if the bulk
             * is in fact a valid bulk on that queue */
        else if((bulk == NULL) || pharosIBulkInsideQueue(queue , bulk) == FALSE)
        {
            /* return invalid bulk */
            result = PHAROS_BULK_GIVE_INVALID_BULK;
        }
        else
        {
            /* disable interrupts so nothing can change the value of the global variables */
            pharosCpuDisableInterrupts(&level);

            /* remove the highest priority blocked thread on the queue */
            highestThread = pharosIThreadQueueGetHighest(&queue->blockedQueue);

            /* if semaphore is not blocking any thread */
            if(highestThread == NULL)
            {
                /* add the item to the unused bulks list */
                simpleListAppendLast(&queue->freeBulks , bulk);

                /* interrupts can now be enabled */
                pharosCpuEnableInterrupts(&level);
            }
            else
            {
                /* give the bulk to the unblocked thread */
                highestThread->unblockInfo.bulk = bulk;

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

            /* return success */
            result = PHAROS_BULK_GIVE_SUCCESS;
        }
    }

    /* return the result of the function */
    return result;
}


void pharosIBulksInitialize(const ptrBulkTable table)
{
    /* iterator of the bulk queue */
    BulkQueueNumber i;

    /* iterator of the bulk inside the bulk queue */
    uint32_t bulkIterator;

    /* pointer to the bulk queue buffer */
    uint8_t *tableBuffer;

    /* index on the table buffer where each message buffer begins */
    uint32_t bufferIterator;


    /* for each bulk queue */
    for(i = 0U; i < table->size; i++)
    {
        /* get the bulk queue */
        ptrBulkQueue queue = &table->bulkQueues[i];

        /* re-initialize the buffer index iterator for this queue */
        bufferIterator = 0U;

        /* update the pointer to the queue buffer */
        tableBuffer = queue->bulkBuffer.buffer;

        /* initialize the list of the free bulks */
        simpleListInitialize(&queue->freeBulks , OFFSETOF(Bulk , bulk.usedNode));

        /* initialize the map of blocked threads on the simple semaphore */
        pharosIThreadQueueInitialize(&queue->blockedQueue , queue->flags.isFifo);

        /* loop for each bulk inside the queue */
        for(bulkIterator = 0U; bulkIterator < queue->numberBulks; bulkIterator++)
        {
            /* get the bulk */
            ptrBulk bulk = &queue->queue[bulkIterator];

            /* initialize the bulk id */
            bulk->bulk.number = i;

            /* initialize the buffer pointer and size */
            pharosBufferInitialize(&bulk->bulk.buffer , &tableBuffer[bufferIterator] , queue->bulkSize);

            /* increase the position of the next message buffer */
            bufferIterator += queue->bulkSize;

            /* add the bulk to the free list */
            simpleListAppendLast(&queue->freeBulks , bulk);
        }
    }
}


static ptrBulkQueue getBulkQueue(BulkQueueNumber number)
{
    /* result of the function */
    ptrBulkQueue result;

    /* get the running bulk table */
    ptrBulkTable table = &pharosIPartitionRunObjectTable()->bulkTable;


    /* if the id is too large */
    if(number >= table->size)
    {
        /* return invalid */
        result = NULL;
    }
        /* if the id is valid */
    else
    {
        /* return the pointer to the bulk queue */
        result = &table->bulkQueues[number];
    }

    /* return result */
    return result;
}


bool pharosIBulkInsideQueue(ptrBulkQueue queue , ptrBulk bulk)
{
    /* result of this method */
    bool result = FALSE;

    /* start address of the bulks inside the queue */
    uint8_t *start = (uint8_t *) & queue->queue[0];

    /* address of the bulk */
    uint8_t *b = (uint8_t *) bulk;


    /* check if the bulk is at an address inside the queue */
    if(&queue->queue[0] <= bulk && bulk <= &queue->queue[queue->numberBulks - 1U])
    {
        /* check if the bulk is correctly aligned */
        if((b - start) % sizeof (Bulk) == 0U)
        {
            /* everything is correct */
            result = TRUE;
        }
        /* else, not correctly aligned */
    }
    /* else, not inside the queue array of bulks */

    return result;
}
