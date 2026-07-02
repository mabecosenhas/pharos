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


#include <pharos/kernel/future/future.h>
#include <pharos/kernel/partition/partition.h>
#include <pharos/kernel/thread/dispatcher.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/kernel/core/coreinlines.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/kernel/scheduler/schedulerinline.h>
#include <pharos/kernel/assert/assert.h>
#include <pharos/kernel/channel/channel.h>


PharosFutureGetR pharosSFutureGet(ptrFutureId id)
{
    /* result of the function */
    PharosFutureGetR result;

    /* future got from the list */
    ptrFuture future;

    /* interrupt level */
    PharosInterruptLevel interruptLevel;

    /* get the running partition */
    ptrPartition partition = pharosIPartitionGetRunning();

    /* get the free futures of the running partition */
    ptrSimpleList freeFutures = &partition->freeFutures;


    /* check if the number address is valid */
    if(pharosCpuBufferIsInsideProtection((uint8_t *) id , sizeof (FutureId) , pharosIPartitionRunningMemProt()) == FALSE)
    {
        /* return invalid address */
        result = PHAROS_FUTURE_GET_INVALID_ADDRESS;
    }
    else
    {
        /* don't allow anyone inside threads or interrupts to change the simple list */
        pharosCpuDisableInterrupts(&interruptLevel);

        /* get the first free future */
        future = simpleListRemoveFirst(freeFutures);

        /* interrupts can now be enabled */
        pharosCpuEnableInterrupts(&interruptLevel);

        /* if the list is empty */
        if(future == NULL)
        {
            /* there are no more futures */
            result = PHAROS_FUTURE_GET_EMPTY;
        }
        else
        {
            /* reset the making ready flag */
            future->flags.makingReady = FALSE;

            /* reset the blocked thread */
            future->blockedThread = NULL;

            /* reset the isReady */
            future->flags.isReady = FALSE;

            /* future no longer on a queue */
            future->flags.onQueue = FALSE;

            /* set the future as not free */
            future->flags.isFree = FALSE;

            /* at startup the future owner is the future->owner */
            future->flags.hasDifferentHolder = FALSE;

            /* copy the restart number */
            future->index.partitionRestartNumber = partition->restartNumber;

            /* mark the bulk as invalid */
            pharosChannelBulkSetInvalid(&future->bulk);

            /* copy the number */
            *id = future->index;

            /* reset the partition that can have access to it */
            future->tempPartition = NULL;

            /* return success */
            result = PHAROS_FUTURE_GET_SUCCESS;
        }
    }

    /* return result */
    return result;
}


PharosFutureWaitR pharosSFutureWait(ptrFutureId number , ptrFutureArg arg , ptrChannelBulk bulk , Timeout timeout)
{
    /* result of the function */
    PharosFutureWaitR result;

    /* copy of the future number (so another thread cannot change it) */
    FutureId myId;

    /* future got from the list */
    ptrFuture future;

    /* interrupt level */
    PharosInterruptLevel interruptLevel;

    /* get the calling thread */
    ptrThreadInfo thread = pharosISchedGetRunningThread();

    /* get the running partition (important if we are inside an interrupt) */
    ptrPartition runningPartition = pharosIPartitionGetRunning();

    /* running partition memory environment */
    ptrEnvMemoryProtection env = pharosIPartitionRunningMemProt();


    /* check if the id address is valid */
    if(pharosCpuBufferIsInsideProtection((uint8_t *) number , sizeof (FutureId) , env) == FALSE)
    {
        /* return invalid address */
        result = PHAROS_FUTURE_WAIT_INVALID_ADDRESS;
    }
        /* if running inside interrupt with a timeout */
    else if((pharosIInterruptIsRunning() == TRUE) && (timeout != PHAROS_TIMEOUT_ZERO))
    {
        /* cannot add a future inside an interrupt */
        result = PHAROS_FUTURE_WAIT_INSIDE_INTERRUPT;
    }
        /* check if the argument is in a valid address */
    else if(pharosCpuBufferIsInsideProtection((uint8_t *) arg , sizeof (FutureArg) , env) == FALSE)
    {
        /* return invalid argument */
        result = PHAROS_FUTURE_WAIT_INVALID_ARG;
    }
        /* check if the bulk address is valid */
    else if((bulk != NULL) &&
            (pharosCpuBufferIsInsideProtection((uint8_t *) bulk , sizeof (ChannelBulk) , env) == FALSE))
    {
        /* return invalid address */
        result = PHAROS_FUTURE_WAIT_INVALID_BULK_ADDRESS;
    }
    else
    {
        /* create a copy of the future number */
        myId = *number;

        /* get the future (now we know that "number" is a valid address) */
        future = pharosIFutureGetLocal(&myId);

        /* don't allow anyone inside threads or interrupts to change the simple list */
        pharosCpuDisableInterrupts(&interruptLevel);

        /* if there is no future with such number on the running partition */
        if(future == NULL)
        {
            /* interrupts can now be enabled */
            pharosCpuEnableInterrupts(&interruptLevel);

            /* return invalid */
            result = PHAROS_FUTURE_WAIT_INVALID;
        }
            /* if the future is not being used */
        else if(future->flags.isFree == TRUE)
        {
            /* interrupts can now be enabled */
            pharosCpuEnableInterrupts(&interruptLevel);

            /* return invalid */
            result = PHAROS_FUTURE_WAIT_INVALID;
        }
            /* check to see if there is a thread blocked on the future */
        else if(future->blockedThread != NULL)
        {
            /* interrupts can now be enabled */
            pharosCpuEnableInterrupts(&interruptLevel);

            /* there is, say it is in use */
            result = PHAROS_FUTURE_WAIT_IS_WAITING;
        }
            /* if the future is already ready */
        else if(future->flags.isReady == TRUE)
        {
            /* then copy the arguments and bulk */
            result = pharosIFutureCopyResult(runningPartition , future , arg , bulk);

            /* interrupts can now be enabled */
            pharosCpuEnableInterrupts(&interruptLevel);
        }
        else if(timeout == PHAROS_TIMEOUT_ZERO)
        {
            /* re-enable interrupts*/
            pharosCpuEnableInterrupts(&interruptLevel);

            /* no timeout selected, return  */
            result = PHAROS_FUTURE_WAIT_UNAVAILABLE;
        }
            /* if wait without timeout */
        else if(timeout == PHAROS_TIMEOUT_INFINITE)
        {
            /* update the state of the thread */
            pharosIThreadStateBlockFuture(&thread->state);

            /* thread is now blocked on the future */
            future->blockedThread = thread;

            /* state that this thread is blocked on the future */
            thread->blockedFuture = future;

            /* remove the thread from the list of ready threads */
            pharosISchedRemoveReadyThread(thread);

            /* re-enable interrupts*/
            pharosCpuEnableInterrupts(&interruptLevel);

            /* dispatch to highest priority ready thread */
            pharosIDispatchThread();

            /* if we reached here it means that the future was unblocked and the future argument is on the thread */

            /* state that this thread is no longer blocked on a future */
            thread->blockedFuture = NULL;

            /* if we got here then a future was received */
            result = thread->unblockInfo.future.result;

            /* if it was a success */
            if(result == PHAROS_FUTURE_WAIT_SUCCESS)
            {
                /* then copy the arguments and bulk */
                result = pharosIFutureCopyResult(runningPartition , future , arg , bulk);
            }
        }
        else
        {
            /* thread is now blocked on the future */
            future->blockedThread = thread;

            /* state that this thread is blocked on the future */
            thread->blockedFuture = future;

            /* update the state of the thread to blocked */
            pharosIThreadStateBlockFuture(&thread->state);

            /* update state to waiting on queue */
            pharosIThreadStateWaitFuture(&thread->state);

            /* remove the thread from the list of ready threads */
            pharosISchedRemoveReadyThread(thread);

            /* add the time handler */
            pharosITimeTickAddThread(thread , timeout + pharosIClockTicksGet(thread->scheduler) ,
                                     PHAROS_TIME_TICK_AWAKEN_THREAD_FUTURE_WAIT_TIMEOUT , thread->scheduler);

            /* re-enable interrupts*/
            pharosCpuEnableInterrupts(&interruptLevel);

            /* dispatch to highest priority ready thread */
            pharosIDispatchThread();

            /* someone could have made a futureReady between the timeout clock tick and this position */

            /* get the result of the timeout (future was made ready?) */
            if(future->flags.isReady == FALSE)
            {
                /* there is no blocked thread now */
                future->blockedThread = NULL;

                /* there was a timeout */
                result = PHAROS_FUTURE_WAIT_TIMEOUT;
            }
            else
            {
                /* if we reached here it means that the future was unblocked and the future argument and bulk are 
                 * already set on the future */

                /* get the result */
                result = thread->unblockInfo.future.result;

                /* if the result was a success */
                if(result == PHAROS_FUTURE_WAIT_SUCCESS)
                {
                    /* then copy the arguments and bulk */
                    result = pharosIFutureCopyResult(runningPartition , future , arg , bulk);
                }
            }

            /* state that this thread is no longer blocked on a future */
            thread->blockedFuture = NULL;
        }
    }

    /* return the result (success or error) */
    return result;
}


ptrFuture pharosIFutureGet(ptrFutureId number)
{
    /* result of the function */
    ptrFuture result;

    /* future table of the partition */
    ptrFutureTable table;

    /* object table of the partition */
    ptrObjectTable objects;


    /* get the object table of the partition */
    objects = pharosIPartitionGetObjectsTable(number->partition);

    /* if the object is valid */
    if(objects != NULL)
    {
        /* get the table of queues of the queue environment */
        table = &objects->futureTable;


        /* if the id is too large */
        if(number->number >= table->size)
        {
            /* return invalid */
            result = NULL;
        }
            /* if the id is valid */
        else
        {
            /* return the pointer to the future table */
            result = &table->futures[number->number];

            /* check to see if the running partition has access to the future */
            if(pharosIFutureIsHolder(result , pharosIPartitionGetRunning()) == FALSE)
            {
                /* if it does not belong to the partition or to the temporary partition then does not have access to the future */
                result = NULL;
            }
        }
    }
        /* if the partition is invalid */
    else
    {
        /* invalid partition */
        result = NULL;
    }

    /* return result */
    return result;
}


ptrFuture pharosIFutureGetLocal(ptrFutureId number)
{
    /* result of the function */
    ptrFuture result;

    /* get the running partition future table */
    ptrFutureTable table = &pharosIPartitionRunObjectTable()->futureTable;


    /* if the partition is not correct or the id is too large */
    if((number->partition != pharosIPartitionGetRunning()->id) || (number->number >= table->size))
    {
        /* return invalid */
        result = NULL;
    }
        /* if the id is valid */
    else
    {
        /* return the pointer to the future table */
        result = &table->futures[number->number];
    }


    /* return result */
    return result;
}


ptrFuture pharosIFutureGetGlobal(ptrFutureId id)
{
    /* result of the function */
    ptrFuture result;

    /* future table of the partition */
    ptrFutureTable table;

    /* object table of the partition */
    ptrObjectTable objects;


    /* get the object table of the partition */
    objects = pharosIPartitionGetObjectsTable(id->partition);

    /* if the object is valid */
    if(objects != NULL)
    {
        /* get the table of queues of the queue environment */
        table = &objects->futureTable;

        /* if the id is too large */
        if(id->number >= table->size)
        {
            /* return invalid */
            result = NULL;
        }
            /* if the id is valid */
        else
        {
            /* return the pointer to the future table */
            result = &table->futures[id->number];
        }
    }
        /* if the partition is invalid */
    else
    {
        /* invalid partition */
        result = NULL;
    }

    /* return result */
    return result;
}


void pharosIFutureUpdateHolder(ptrFuture future , ptrPartition newHolder)
{
    /* make sure there is a future */
    if(future != NULL)
    {
        /* if the new holder is the owner of the future */
        if(future->owner == newHolder)
        {
            /* then reset the flag */
            future->flags.hasDifferentHolder = FALSE;

            /* and reset the temporary partition */
            future->tempPartition = NULL;
        }
            /* if the new holder is not the owner of the future */
        else
        {
            /* set the temporary partition */
            future->tempPartition = newHolder;

            /* set that the future has a different holder than the owner */
            future->flags.hasDifferentHolder = TRUE;
        }
    }
}


void pharosIFutureRemoveAccess(ptrFuture future)
{
    /* state that it has a different holder */
    future->flags.hasDifferentHolder = TRUE;

    /* but we don't know which it is yet */
    future->tempPartition = NULL;
}


bool pharosIFutureIsHolder(ptrFuture future , ptrPartition partition)
{
    /* return value of this method */
    bool result;

    /* current holder of the future */
    ptrPartition holder;


    /* if the future does not have different holder */
    if(future->flags.hasDifferentHolder == FALSE)
    {
        /* get the future owner */
        holder = future->owner;
    }
        /* if the future has a different holder */
    else
    {
        /* get the temporary partition that holds the future */
        holder = future->tempPartition;
    }

    /* if the holder is the partition */
    if(holder == partition)
    {
        /* then it is the holder */
        result = TRUE;
    }
        /* otherwise */
    else
    {
        /* it is not the holder */
        result = FALSE;
    }

    /* returns TRUE if the partition holds the future and FALSE otherwise */
    return result;
}


PharosFutureWaitR pharosIFutureCopyResult(ptrPartition receiver , ptrFuture future , ptrFutureArg destination , ptrChannelBulk bulk)
{
    /* return by default success */
    PharosFutureWaitR result = PHAROS_FUTURE_WAIT_SUCCESS;


    /* check if we are running in the correct core */
    PHAROS_ASSERT_CORE(receiver->core->number , "Incorrect core");

    /* then just copy the arguments */
    *destination = future->argument;

    /* if there is bulk to copy to */
    if(bulk != NULL)
    {
        /* if the future has a bulk */
        if(pharosChannelBulkIsValid(&future->bulk) == TRUE)
        {
            /* if the receiver runs in user mode then try to add the MMU page to the bulk */
            if((pharosIPartitionIsSupervisor(receiver) == FALSE) &&
               (pharosCpuChannelBulkAdd(future->bulk.kernelBulk->queue , future->bulk.kernelBulk , receiver) == 0U))
            {
                /* then we have to free the channel bulk (the receiver partition can't have it) */
                pharosIChannelBulkFree(future->bulk.kernelBulk);

                /* it could not add the bulk */
                result = PHAROS_FUTURE_WAIT_INVALID_MMU;
            }
            else
            {
                /* copy the future bulk */
                *bulk = future->bulk;
            }
        }
            /* if the future does not have a bulk */
        else
        {
            /* then mark the caller bulk as invalid so it knows not to use it */
            pharosChannelBulkSetInvalid(bulk);
        }
    }
        /* otherwise, the receiver does not want the bulk */
    else
    {
        /* if the future has a bulk */
        if(pharosChannelBulkIsValid(&future->bulk) == TRUE)
        {
            /* then we have to free the channel bulk (no one else will) */
            pharosIChannelBulkFree(future->bulk.kernelBulk);
        }
    }

    /* return success or the MMU tables error */
    return result;
}

