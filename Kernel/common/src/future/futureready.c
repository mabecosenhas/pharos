/****************************************************************************
 * Pharos - A Real-Time Secure Operating System                             *
 * Copyright 2019 Pedro Macara and Filipe Monteiro                          *
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
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/kernel/core/coreinlines.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/kernel/scheduler/schedulerinline.h>


static void pharosIFutureBulkRemovePermission(ptrFuture future);


PharosFutureReadyR pharosSFutureReady(ptrFutureId id , ptrFutureArg arg , ptrChannelBulk bulk)
{
    /* result of the function */
    PharosFutureReadyR result;

    /* future got from the list (the ready is the only operation that could be performed outside the running partition) */
    ptrFuture future;

    /* get the future without protections */
    ptrFuture unprotected;

    /* interrupt level */
    PharosInterruptLevel level;

    /* get the running partition */
    ptrPartition runningPartition = pharosIPartitionGetRunning();

    /* running partition memory environment */
    ptrEnvMemoryProtection env = pharosIPartitionRunningMemProt();


    /* check if the id address is valid */
    if(pharosCpuBufferIsInsideProtection((uint8_t *) id , sizeof (FutureId) , env) == FALSE)
    {
        /* return invalid address */
        result = PHAROS_FUTURE_READY_INVALID_ADDRESS;
    }
        /* check if the bulk address is valid */
    else if((bulk != NULL) &&
            (pharosCpuBufferIsInsideProtection((uint8_t *) bulk , sizeof (ChannelBulk) , env) == FALSE))
    {
        /* return invalid address */
        result = PHAROS_FUTURE_READY_INVALID_BULK_ADDRESS;
    }
        /* check if the caller has permissions to the bulk */
    else if((bulk != NULL) && pharosIChannelBulkKernelValid(runningPartition , bulk->kernelBulk) == FALSE)
    {
        /* return invalid bulk (can only ready a bulk that the belongs to the running partition) */
        result = PHAROS_FUTURE_READY_INVALID_BULK;
    }
        /* check if the argument is in a valid address */
    else if(pharosCpuBufferIsInsideProtection((uint8_t *) arg , sizeof (FutureArg) , env) == FALSE)
    {
        /* return invalid argument */
        result = PHAROS_FUTURE_READY_INVALID_ARG;
    }
    else
    {
        /* future got from the list
         * (the ready is the only operation that could be performed outside the running partition) */
        future = pharosIFutureGet(id);

        /* get the future without protections */
        unprotected = pharosIFutureGetGlobal(id);

        /* don't allow anyone inside threads or interrupts to access the future */
        pharosCpuDisableInterrupts(&level);

        /* if there is no future with such number on the running partition */
        if(unprotected == NULL)
        {
            /* re-enable interrupts*/
            pharosCpuEnableInterrupts(&level);

            /* return invalid */
            result = PHAROS_FUTURE_READY_INVALID;
        }
            /* if the restart number is incorrect */
        else if(id->partitionRestartNumber != unprotected->owner->restartNumber)
        {
            /* re-enable interrupts*/
            pharosCpuEnableInterrupts(&level);

            /* return that the partition restarted, so nothing needs to be done */
            result = PHAROS_FUTURE_READY_PARTITION_RESTART;
        }
            /* if there is no future with such number on the running partition */
        else if(future == NULL)
        {
            /* re-enable interrupts*/
            pharosCpuEnableInterrupts(&level);

            /* return invalid */
            result = PHAROS_FUTURE_READY_INVALID;
        }
            /* if the running partition does not hold the future */
        else if(pharosIFutureIsHolder(future , runningPartition) == FALSE)
        {
            /* re-enable interrupts*/
            pharosCpuEnableInterrupts(&level);

            /* invalid future */
            result = PHAROS_FUTURE_READY_INVALID;
        }
        else
        {
            /* if the future is already ready */
            if((future->flags.isReady == TRUE) || (future->flags.makingReady == TRUE))
            {
                /* re-enable interrupts*/
                pharosCpuEnableInterrupts(&level);

                /* future already ready */
                result = PHAROS_FUTURE_READY_ALREADY_READY;
            }
            else
            {
                /* future is making ready -> this makes sure that if a thread has started to ready the future then no
                 * other thread ready it */
                future->flags.makingReady = TRUE;

                /* re-enable interrupts*/
                pharosCpuEnableInterrupts(&level);

                /* disable the thread dispatching. This is essential since in 
                 * single-core -> we are removing the bulk from the future holder to the future owner 
                 * multi-core  -> we are removing the bulk from the future holder to the future owner in another partition 
                 * if a partition restart or a thread stop/restart occurs, then we have to make sure that this operation finishes,
                 * that is, there is a moment when the future bulk will not have an owner. We cannot let this thread stop in the 
                 * middle of this process. In multi-core, we make sure that the interrupt to the other core is triggered so it is the 
                 * responsability of the other core to finish this operation */
                pharosIDispatchDisable();

                /* if there is a bulk */
                if(bulk != NULL)
                {
                    /* then copy the bulk */
                    future->bulk = *bulk;

                    /* remove the MMU permissions to the future bulk (if needed) */
                    pharosIFutureBulkRemovePermission(future);
                }
                else
                {
                    /* otherwise mark the bulk as invalid */
                    pharosChannelBulkSetInvalid(&future->bulk);
                }

                /* ready the future */
                pharosIFutureReady(future , arg);

                /* re-enable dispatching */
                pharosIDispatchEnable();

                /* return success */
                result = PHAROS_FUTURE_READY_SUCCESS;
            }
        }
    }

    /* return the result (success or error) */
    return result;
}


void pharosIFutureReadyOnOwnerCore(ptrFuture future , ptrFutureArg arg)
{
    /* interrupt level */
    PharosInterruptLevel interruptLevel;

    /* thread return value (if a thread was blocked on this future it will return the value on this variable) */
    PharosFutureWaitR result;


    /* check we are running in the future owner core */
    PHAROS_ASSERT_CORE(future->owner->core->number , "Incorrect core");

    /* don't allow anyone inside threads or interrupts to access the future */
    pharosCpuDisableInterrupts(&interruptLevel);

    /* check for the special case of a future reset */
    if(arg != NULL)
    {
        /* copy the arguments */
        future->argument = *arg;

        /* if readying the future with a bulk */
        if(pharosChannelBulkIsValid(&future->bulk) == TRUE)
        {
            /* then add the bulk to the partition */
            pharosIChannelBulkAdd2PartitionOnPartitionCore(future->owner , future->bulk.kernelBulk);
        }

        /* thread will return success */
        result = PHAROS_FUTURE_WAIT_SUCCESS;
    }
    else
    {
        /* thread will return partition restarted or an error occurred 
         * (e.g. MMU error on the channel receiver partition) */
        result = PHAROS_FUTURE_WAIT_PARTITION_ERROR;
    }


    /* make the future ready */
    future->flags.isReady = TRUE;

    /* if there is no holder */
    if(future->blockedThread == NULL)
    {
        /* if the future does not have a blocked thread then it cannot be on a queue */
        PHAROS_ASSERT(future->flags.onQueue == FALSE , "Flag is incorrect");

        /* re-enable interrupts */
        pharosCpuEnableInterrupts(&interruptLevel);
    }
    else
    {
        /* check that we are running on the blocked thread core */
        PHAROS_ASSERT_CORE(future->blockedThread->partition->core->number , "Incorrect core");

        /* if the future is on a queue */
        if(future->flags.onQueue == TRUE)
        {
            /* remove the future from the blocked queue */
            linkedListRemoveObject(&future->blockedThread->futureBlockedQueue , future);

            /* add the future to the ready queue */
            linkedListAppendLast(&future->blockedThread->futureReadyQueue , future);
        }

        /* if future blocked thread is waiting on the future we are readying or on the future queue */
        if((future == future->blockedThread->blockedFuture) ||
           (pharosIThreadStateIsBlockedFutureQueue(future->blockedThread->state) == TRUE))
        {
            PHAROS_ASSERT((pharosIThreadStateIsBlockedFuture(future->blockedThread->state) == TRUE) ||
                          (pharosIThreadStateIsBlockedFutureQueue(future->blockedThread->state) == TRUE) , "incorrect state");

            /* update the thread queue receive result */
            future->blockedThread->threadFlags.unblockDueToTimeout = FALSE;

            /* remove the thread timeout time tick (if needed). 
             * Sporadic threads waiting for activation don't have time tick handler (no timeout) */
            pharosITimeTickRemoveThread(future->blockedThread);

            /* update the state of the thread to ready */
            pharosIThreadStateUnblock(&future->blockedThread->state);

            /* remove the wait state (if was placed) */
            pharosIThreadStateUnwait(&future->blockedThread->state);

            /* add the thread to ready thread list */
            pharosISchedAddReadyThread(future->blockedThread);

            /* make the waiting thread return success */
            future->blockedThread->unblockInfo.future.result = result;

            /* there is no blocked thread now */
            future->blockedThread = NULL;

            /* re-enable interrupts*/
            pharosCpuEnableInterrupts(&interruptLevel);

            /* maybe necessary to dispatch to thread we just made ready */
            pharosIDispatchThread();
        }
        else
        {
            /* re-enable interrupts*/
            pharosCpuEnableInterrupts(&interruptLevel);
        }
    }

    /* if there was an error on the partition */
    if(result == PHAROS_FUTURE_WAIT_PARTITION_ERROR)
    {
        /* then free the future so that it can be re-used */
        pharosIFutureFree(future);
    }
}


void pharosIFutureBulkRemovePermission(ptrFuture future)
{
    /* kernel bulk */
    ptrChannelBulkKernel bulkKernel;

    /* interrupt level */
    PharosInterruptLevel level;


    /* get the future bulk kernel */
    bulkKernel = future->bulk.kernelBulk;

    PHAROS_ASSERT((bulkKernel->holder == future->owner) || (bulkKernel->holder == future->tempPartition) , "invalid holder");

    /* don't allow anyone inside threads or interrupts to access the future */
    pharosCpuDisableInterrupts(&level);

    /* if running in user mode */
    if(pharosIPartitionHasSupervisor() == FALSE)
    {
        /* remove permissions on the bulk */
        pharosCpuChannelBulkRemove(bulkKernel , bulkKernel->holder);
    }

    /* at this point bulk belongs to this channel, we must remove it */
    pharosIChannelBulkRemoveFromHolderOnPartitionCore(bulkKernel);

    /* re-enable interrupts */
    pharosCpuEnableInterrupts(&level);
}

