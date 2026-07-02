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
#include <pharos/kernel/future/futureQueue.h>
#include <pharos/kernel/thread/dispatcher.h>
#include <pharos/kernel/partition/partition.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/kernel/scheduler/schedulerinline.h>


PharosFutureQueueAddR pharosSFutureQueueAdd(ptrFutureId number)
{
    /* result of the function */
    PharosFutureQueueAddR result;

    /* interrupt level */
    PharosInterruptLevel interruptLevel;

    /* copy of the future number (so another thread cannot change it) */
    FutureId myId;

    /* future got from the list */
    ptrFuture future;

    /* calling thread */
    ptrThreadInfo thread;


    /* if running inside interrupt */
    if(pharosIInterruptIsRunning() == TRUE)
    {
        /* cannot add a future inside an interrupt */
        result = PHAROS_FUTURE_QUEUE_ADD_INSIDE_INTERRUPT;
    }
        /* check if the number address is valid */
    else if(pharosCpuBufferIsInsideProtection((uint8_t *) number , sizeof (FutureId) , pharosIPartitionRunningMemProt()) == FALSE)
    {
        /* return invalid address */
        result = PHAROS_FUTURE_QUEUE_ADD_INVALID_ADDRESS;
    }
    else
    {
        /* create a copy of the future number */
        myId = *number;

        /* get the future (now we know that "number" is a valid address) */
        future = pharosIFutureGetLocal(&myId);

        /* get the calling thread */
        thread = pharosISchedGetRunningThread();

        /* if there is no future with such number on the running partition */
        if(future == NULL)
        {
            /* return invalid */
            result = PHAROS_FUTURE_QUEUE_ADD_INVALID;
        }
        else
        {
            /* don't allow anyone inside threads or interrupts to access the future */
            pharosCpuDisableInterrupts(&interruptLevel);

            /* check if there is another thread blocked on the future */
            if(future->blockedThread != NULL)
            {
                /* interrupts can now be enabled */
                pharosCpuEnableInterrupts(&interruptLevel);

                /* there cannot be more than one thread blocked on the future */
                result = PHAROS_FUTURE_QUEUE_ADD_IS_WAITING;
            }
            else
            {
                /* future is now on a thread queue */
                future->flags.onQueue = TRUE;

                /* if the future is ready */
                if(future->flags.isReady == TRUE)
                {
                    /* add the future to the running thread list of futures that are ready */
                    linkedListAppendLast(&thread->futureReadyQueue , future);
                }
                    /* else, the future is not yet ready */
                else
                {
                    /* make the future blocked on the running thread. This way no other thread can access it */
                    future->blockedThread = thread;

                    /* add the future to the running thread list of futures to wait for */
                    linkedListAppendLast(&thread->futureBlockedQueue , future);
                }

                /* interrupts can now be enabled */
                pharosCpuEnableInterrupts(&interruptLevel);

                /* return success */
                result = PHAROS_FUTURE_QUEUE_ADD_SUCCESS;
            }
        }
    }

    /* return the result (success or error) */
    return result;
}


PharosFutureQueueRemoveR pharosSFutureQueueRemove(ptrFutureId number)
{
    /* result of the function */
    PharosFutureQueueRemoveR result;

    /* interrupt level */
    PharosInterruptLevel interruptLevel;

    /* future got from the list */
    ptrFuture future;

    /* copy of the future number (so another thread cannot change it) */
    FutureId myId;

    /* calling thread */
    ptrThreadInfo thread;


    /* if running inside interrupt */
    if(pharosIInterruptIsRunning() == TRUE)
    {
        /* cannot add a future inside an interrupt */
        result = PHAROS_FUTURE_QUEUE_REMOVE_INSIDE_INTERRUPT;
    }
        /* check if the number address is valid */
    else if(pharosCpuBufferIsInsideProtection((uint8_t *) number , sizeof (FutureId) , pharosIPartitionRunningMemProt()) == FALSE)
    {
        /* return invalid address */
        result = PHAROS_FUTURE_QUEUE_REMOVE_INVALID_ADDRESS;
    }
    else
    {
        /* create a copy of the future number */
        myId = *number;

        /* get the future (now we know that "number" is a valid address) */
        future = pharosIFutureGetLocal(&myId);

        /* get the calling thread */
        thread = pharosISchedGetRunningThread();

        /* if there is no future with such number on the running partition */
        if(future == NULL)
        {
            /* return invalid */
            result = PHAROS_FUTURE_QUEUE_REMOVE_INVALID;
        }
        else
        {
            /* don't allow anyone inside threads or interrupts to access the future */
            pharosCpuDisableInterrupts(&interruptLevel);

            /* check if the running thread has the future */
            if(future->blockedThread != thread)
            {
                /* interrupts can now be enabled */
                pharosCpuEnableInterrupts(&interruptLevel);

                /* cannot remove a future that does not belong to it */
                result = PHAROS_FUTURE_QUEUE_REMOVE_NOT_HOLDER;
            }
            else
            {
                /* future is now not on a thread queue */
                future->flags.onQueue = FALSE;

                /* remove the blocked thread */
                future->blockedThread = NULL;

                /* if the future is ready */
                if(future->flags.isReady == TRUE)
                {
                    /* remove the future from the the running thread list of futures to wait for */
                    linkedListRemoveObject(&thread->futureReadyQueue , future);
                }
                else
                {
                    /* remove the future from the the running thread list of futures to wait for */
                    linkedListRemoveObject(&thread->futureBlockedQueue , future);
                }

                /* interrupts can now be enabled */
                pharosCpuEnableInterrupts(&interruptLevel);

                /* return success */
                result = PHAROS_FUTURE_QUEUE_REMOVE_SUCCESS;
            }
        }
    }

    /* return the result (success or error) */
    return result;
}


PharosFutureQueueWaitR pharosSFutureQueueWait(ptrFutureId futureDone , ptrFutureArg arg , ptrChannelBulk bulk ,
                                              Timeout timeout)
{
    /* result of the function */
    PharosFutureQueueAddR result;

    /* interrupt level */
    PharosInterruptLevel interruptLevel;

    /* future */
    ptrFuture future;

    /* get the calling thread */
    ptrThreadInfo thread = pharosISchedGetRunningThread();

    /* running partition memory environment */
    ptrEnvMemoryProtection env = pharosIPartitionRunningMemProt();


    /* if running inside interrupt */
    if(pharosIInterruptIsRunning() == TRUE)
    {
        /* cannot add a future inside an interrupt */
        result = PHAROS_FUTURE_QUEUE_WAIT_INSIDE_INTERRUPT;
    }
        /* check if the future is in a valid address */
    else if(pharosCpuBufferIsInsideProtection((uint8_t *) futureDone , sizeof (FutureId) , env) == FALSE)
    {
        /* return invalid future */
        result = PHAROS_FUTURE_QUEUE_WAIT_INVALID_FUTURE_ADDRESS;
    }
        /* check if the argument is in a valid address */
    else if(pharosCpuBufferIsInsideProtection((uint8_t *) arg , sizeof (FutureArg) , env) == FALSE)
    {
        /* return invalid argument */
        result = PHAROS_FUTURE_QUEUE_WAIT_INVALID_ARGUMENT_ADDRESS;
    }
        /* check if the bulk address is valid */
    else if((bulk != NULL) &&
            (pharosCpuBufferIsInsideProtection((uint8_t *) bulk , sizeof (ChannelBulk) , env) == FALSE))
    {
        /* return invalid address */
        result = PHAROS_FUTURE_QUEUE_WAIT_INVALID_BULK_ADDRESS;
    }
    else
    {
        /* don't allow anyone inside threads or interrupts to access the future */
        pharosCpuDisableInterrupts(&interruptLevel);

        /* check if the thread future queue is empty (thread has not added any future) */
        if(linkedListIsEmpty(&thread->futureReadyQueue) == TRUE && linkedListIsEmpty(&thread->futureBlockedQueue) == TRUE)
        {
            /* re-enable interrupts*/
            pharosCpuEnableInterrupts(&interruptLevel);

            /* return empty */
            result = PHAROS_FUTURE_QUEUE_WAIT_EMPTY;
        }
        else
        {
            /* get the first ready future */
            future = linkedListRemoveFirst(&thread->futureReadyQueue);

            /* if there are futures ready */
            if(future != NULL)
            {
                /* copy the index */
                *futureDone = future->index;

                /* then copy the arguments and bulk */
                result = pharosIFutureCopyResult(thread->partition , future , arg , bulk);

                /* future not on queue anymore */
                future->flags.onQueue = FALSE;

                /* remove the blocked thread */
                future->blockedThread = NULL;

                /* interrupts can now be enabled */
                pharosCpuEnableInterrupts(&interruptLevel);
            }
                /* if the caller does not want to wait */
            else if(timeout == PHAROS_TIMEOUT_ZERO)
            {
                /* re-enable interrupts*/
                pharosCpuEnableInterrupts(&interruptLevel);

                /* no timeout selected, return  */
                result = PHAROS_FUTURE_QUEUE_WAIT_UNAVAILABLE;
            }
                /* if wait without timeout */
            else if(timeout == PHAROS_TIMEOUT_INFINITE)
            {
                /* update the state of the thread */
                pharosIThreadStateBlockFutureQueue(&thread->state);

                /* remove the thread from the list of ready threads */
                pharosISchedRemoveReadyThread(thread);

                /* re-enable interrupts*/
                pharosCpuEnableInterrupts(&interruptLevel);

                /* dispatch to highest priority ready thread */
                pharosIDispatchThread();

                /* if we reached here it means that the future was unblocked and the future argument is on the thread */

                /* don't allow anyone inside threads or interrupts to access the future */
                pharosCpuDisableInterrupts(&interruptLevel);

                /* get the first ready future */
                future = linkedListRemoveFirst(&thread->futureReadyQueue);

                /* check that we got a valid future */
                PHAROS_ASSERT(future != NULL , "future is invalid");

                /* future is not on a thread queue anymore */
                future->flags.onQueue = FALSE;

                /* no thread is blocked on the future */
                future->blockedThread = NULL;

                /* copy the arguments and bulk */
                result = pharosIFutureCopyResult(thread->partition , future , arg , bulk);

                /* re-enable interrupts*/
                pharosCpuEnableInterrupts(&interruptLevel);

                /* copy the future (with interrupts enable since the index is always the same) */
                *futureDone = future->index;
            }
            else
            {
                /* update the state of the thread to blocked */
                pharosIThreadStateBlockFutureQueue(&thread->state);

                /* update state to waiting on queue */
                pharosIThreadStateWaitFutureQueue(&thread->state);

                /* remove the thread from the list of ready threads */
                pharosISchedRemoveReadyThread(thread);

                /* add the time handler */
                pharosITimeTickAddThread(thread , timeout + pharosIClockTicksGet(thread->scheduler) ,
                                         PHAROS_TIME_TICK_AWAKEN_THREAD_FUTURE_QUEUE_WAIT_TIMEOUT ,
                                         thread->scheduler);

                /* re-enable interrupts*/
                pharosCpuEnableInterrupts(&interruptLevel);

                /* dispatch to highest priority ready thread */
                pharosIDispatchThread();

                /* don't allow anyone inside threads or interrupts to access the future */
                pharosCpuDisableInterrupts(&interruptLevel);

                /* get the future from the ready queue */
                future = linkedListRemoveFirst(&thread->futureReadyQueue);

                /* if there is a future on the ready queue */
                if(future != NULL)
                {
                    /* if we reached here it means that the future was unblocked and the future argument is on the thread */

                    /* copy the arguments and bulk */
                    result = pharosIFutureCopyResult(thread->partition , future , arg , bulk);

                    /* future is not on a thread queue anymore */
                    future->flags.onQueue = FALSE;

                    /* remove the blocked thread */
                    future->blockedThread = NULL;

                    /* re-enable interrupts*/
                    pharosCpuEnableInterrupts(&interruptLevel);

                    /* copy the future */
                    *futureDone = future->index;
                }
                    /* else, there is no future received within the timeout */
                else
                {
                    /* re-enable interrupts*/
                    pharosCpuEnableInterrupts(&interruptLevel);

                    /* return timeout */
                    result = PHAROS_FUTURE_QUEUE_WAIT_TIMEOUT;
                }
            }
        }
    }

    /* return the result (success or error) */
    return result;
}
