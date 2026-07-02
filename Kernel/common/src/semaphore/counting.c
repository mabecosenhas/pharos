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


#include <pharos/kernel/semaphore/declarations.h>
#include <pharos/kernel/object/object.h>
#include <pharos/kernel/partition/partition.h>
#include <pharos/kernel/thread/dispatcher.h>
#include <pharos/kernel/scheduler/scheduler.h>
#include <pharos/kernel/interrupt/interrupt.h>
#include <pharos/kernel/thread/aperiodicthread.h>
#include <pharos/semaphore.h>
#include <pharos/kernel/semaphore/counting.h>
#include <pharos/hal/boardApi.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/kernel/scheduler/schedulerinline.h>


/**
 * Get the counting semaphore on the running partition
 * 
 * @param number counting semaphore number
 * 
 * @return returns the pointer to the counting semaphore or NULL if invalid
 */
static ptrSemaphoreCounting pharosISemCountGet(SemCountNumber number);


/**
 * Release the counting semaphore
 * 
 * @param semaphore counting semaphore to release
 * 
 * @return returns the release result (always returns success)
 */
static PharosSemCountReleaseR pharosISemCountRelease(ptrSemaphoreCounting semaphore);


/**
 * Obtains the counting semaphore with a timeout 
 * 
 * @param runningThread running thread that is trying to obtain the counting semaphore
 * @param semaphore counting semaphore to obtain
 * @param timeout timeout of the obtain
 * 
 * @return returns success, timeout or invalid
 */
static PharosSemCountObtainTimeoutR pharosISemCountObtainTimeoutUnprotected(ptrThreadInfo runningThread , ptrSemaphoreCounting semaphore , Timeout timeout);


/**
 * Initialize a counting semaphore 
 * 
 * @param semaphore semaphore to initialize
 * @param resetCounter flag indicating if should initialize with the counter at 0 or leave it as is
 */
static void pharosISemCountInitialize(ptrSemaphoreCounting semaphore);


void pharosISemsCountInitialize(const ptrSemaphoreCountingTable table)
{
    /* iterator */
    ObjectNumber i;


    /* loop through all the simple semaphores */
    for(i = 0U; i < table->size; i++)
    {
        /* get the counting semaphore to initialize */
        ptrSemaphoreCounting semaphore = &table->countingSemaphores[i];

        /* initialize the semaphore and don't force a reset on the counter */
        pharosISemCountInitialize(semaphore);
    }

    /* counting semaphores are now ready to be used */
}


PharosSemCountObtainR pharosSSemCountObtain(SemCountNumber number)
{
    /* return value of the function */
    PharosSemCountObtainR result;

    /* interrupt level */
    PharosInterruptLevel level;

    /* semaphore to obtain */
    ptrSemaphoreCounting semaphore;


    /* get the semaphore */
    semaphore = pharosISemCountGet(number);

    /* if the semaphore id is invalid */
    if(semaphore == NULL)
    {
        /* return invalid */
        result = PHAROS_SEMAPHORE_COUNTING_OBTAIN_INVALID;
    }
    else
    {
        /* disable interrupts so nothing can change the value of the global variables */
        pharosCpuDisableInterrupts(&level);

        /* check if the counting semaphore is available */
        if(semaphore->count > 0U)
        {
            /* decrease the count */
            semaphore->count--;

            /* re-enable interrupts */
            pharosCpuEnableInterrupts(&level);

            result = PHAROS_SEMAPHORE_COUNTING_OBTAIN_SUCCESS;
        }
            /* counting semaphore is unavailable */
        else
        {
            /* re-enable interrupts */
            pharosCpuEnableInterrupts(&level);

            /* no timeout, return counting semaphore unavailable */
            result = PHAROS_SEMAPHORE_COUNTING_OBTAIN_UNAVAILABLE;
        }
    }

    /* return result */
    return result;
}


PharosSemCountObtainTimeoutR pharosISemCountObtainTimeoutUnprotected(ptrThreadInfo thread , ptrSemaphoreCounting semaphore ,
                                                                     Timeout timeout)
{
    /* return value of the method */
    PharosSemCountObtainTimeoutR result;

    /* interrupt level */
    PharosInterruptLevel level;


    /* disable interrupts so nothing can change the value of the global variables */
    pharosCpuDisableInterrupts(&level);

    /* check if the semaphore is available */
    if(semaphore->count > 0U)
    {
        /* decrease the count */
        semaphore->count--;

        /* re-enable interrupts */
        pharosCpuEnableInterrupts(&level);

        /* return success */
        result = PHAROS_SEMAPHORE_COUNTING_OBTAIN_SUCCESS;
    }
        /* counting semaphore is unavailable */
    else
    {
        /* counting semaphore is unavailable, check if we should wait for a timeout */
        if(timeout == PHAROS_TIMEOUT_ZERO)
        {
            /* no timeout, return counting semaphore unavailable */
            result = PHAROS_SEMAPHORE_COUNTING_OBTAIN_TIMEOUT_UNAVAILABLE;

            /* re-enable interrupts */
            pharosCpuEnableInterrupts(&level);
        }
            /* if waiting without timeout */
        else if(timeout == PHAROS_TIMEOUT_INFINITE)
        {
            /* going to block until the semaphore is released to this thread */
            pharosIThreadQueueAddBlocked(&semaphore->blockedQueue , thread);

            /* block the thread on the counting semaphore */
            pharosIThreadStateBlockCount(&thread->state);

            /* remove the thread from the ready list */
            pharosISchedRemoveReadyThread(thread);

            /* re-enable interrupts */
            pharosCpuEnableInterrupts(&level);

            /* dispatch to highest priority ready thread */
            pharosIDispatchThread();

            /* since the timeout is infinite, if we get here then the semaphore was obtained */
            result = PHAROS_SEMAPHORE_COUNTING_OBTAIN_TIMEOUT_SUCCESS;
        }
            /* waiting with timeout */
        else
        {
            /* going to block until the semaphore is released to this thread */
            pharosIThreadQueueAddBlocked(&semaphore->blockedQueue , thread);

            /* block the thread on the counting semaphore */
            pharosIThreadStateBlockCount(&thread->state);

            /* block the thread on a timeout */
            pharosIThreadStateWaitCount(&thread->state);

            /* remove the thread from the ready list */
            pharosISchedRemoveReadyThread(thread);

            /* reset the unblock information */
            thread->threadFlags.unblockDueToTimeout = FALSE;

            /* add a tick handler at the timeout instant */
            pharosITimeTickAddThread(thread , timeout + pharosIClockTicksGet(thread->scheduler) ,
                                     PHAROS_TIME_TICK_AWAKEN_THREAD_SEMAPHORE_COUNTING_OBTAIN_TIMEOUT ,
                                     thread->scheduler);

            /* re-enable interrupts*/
            pharosCpuEnableInterrupts(&level);

            /* dispatch to highest priority ready thread */
            pharosIDispatchThread();

            /* get the result of the timeout (semaphore was released or timeout occurred?) */
            if(thread->threadFlags.unblockDueToTimeout == TRUE)
            {
                /* there was a timeout */
                result = PHAROS_SEMAPHORE_COUNTING_OBTAIN_TIMEOUT_TIMEOUT;
            }
            else
            {
                /* counting semaphore was released and this thread obtained it before the timeout expired */
                result = PHAROS_SEMAPHORE_COUNTING_OBTAIN_TIMEOUT_SUCCESS;
            }
        }
    }

    /* return success or the error condition */
    return result;
}


PharosSemCountObtainTimeoutR pharosSSemCountObtainTimeout(SemCountNumber number , Timeout timeout)
{
    /* method return value */
    PharosSemCountObtainTimeoutR result;

    /* semaphore to obtain */
    ptrSemaphoreCounting semaphore;

    /* calling thread */
    ptrThreadInfo runningThread;


    /* get the semaphore */
    semaphore = pharosISemCountGet(number);

    /* get the running thread */
    runningThread = pharosISchedGetRunningThread();

    /* if the semaphore id is invalid */
    if(semaphore == NULL)
    {
        /* return invalid */
        result = PHAROS_SEMAPHORE_COUNTING_OBTAIN_TIMEOUT_INVALID;
    }
        /* check if is running inside an interrupt */
    else if(pharosIInterruptIsRunning() == TRUE)
    {
        /* inside an interrupt cannot obtain semaphores */
        result = PHAROS_SEMAPHORE_COUNTING_OBTAIN_TIMEOUT_INSIDE_INTERRUPT;
    }
    else
    {
        /* get the counting semaphore result */
        result = pharosISemCountObtainTimeoutUnprotected(runningThread , semaphore , timeout);
    }

    /* return success or the error condition */
    return result;
}


PharosSemCountReleaseR pharosISemCountRelease(ptrSemaphoreCounting semaphore)
{
    /* interrupt level */
    PharosInterruptLevel level;

    /* highest thread that was blocked on the counting semaphore */
    ptrThreadInfo highestThread;

    /* return value of the method */
    PharosSemCountReleaseR result;


    /* disable interrupts so nothing can change the value of the global variables */
    pharosCpuDisableInterrupts(&level);

    /* remove the highest priority blocked thread on the queue */
    highestThread = pharosIThreadQueueGetHighest(&semaphore->blockedQueue);

    /* if semaphore is not blocking any thread */
    if(highestThread == NULL)
    {
        /* if the semaphore count is already at maximum */
        if(semaphore->count == semaphore->maximumCount)
        {
            /* can re-enable interrupts */
            pharosCpuEnableInterrupts(&level);

            /* don't let it increase any more */
            result = PHAROS_SEMAPHORE_COUNTING_RELEASE_MAX_REACHED;
        }
        else
        {
            /* increment the internal count */
            semaphore->count++;

            /* can re-enable interrupts */
            pharosCpuEnableInterrupts(&level);

            /* return success */
            result = PHAROS_SEMAPHORE_COUNTING_RELEASE_SUCCESS;
        }
    }
    else
    {
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

        /* result is success */
        result = PHAROS_SEMAPHORE_COUNTING_RELEASE_SUCCESS;
    }

    return result;
}


PharosSemCountReleaseR pharosSSemCountRelease(SemCountNumber number)
{
    /* return value of the method */
    PharosSemCountReleaseR result;

    /* semaphore */
    ptrSemaphoreCounting semaphore = pharosISemCountGet(number);


    /* if the semaphore id is invalid */
    if(semaphore == NULL)
    {
        /* return invalid */
        result = PHAROS_SEMAPHORE_COUNTING_RELEASE_INVALID;
    }
    else
    {
        result = pharosISemCountRelease(semaphore);
    }

    /* return success or the error */
    return result;
}


ptrSemaphoreCounting pharosISemCountGet(SemCountNumber number)
{
    /* counting semaphore to return */
    ptrSemaphoreCounting result;

    /* get the table */
    ptrSemaphoreCountingTable table = &pharosIPartitionRunObjectTable()->countingSemaphoreTable;

    /* if the id is too great */
    if(number >= table->size)
    {
        /* return invalid */
        result = NULL;
    }
    else
    {
        /* get the counting semaphore address */
        result = &table->countingSemaphores[number];
    }

    /* return the counting semaphore got */
    return result;
}


void pharosISemCountInitialize(ptrSemaphoreCounting semaphore)
{
    /* initialize the map of blocked threads on the simple semaphore */
    pharosIThreadQueueInitialize(&semaphore->blockedQueue , semaphore->flags.isFifo);

    /* if the application did not define a maximum count */
    if(semaphore->maximumCount == 0)
    {
        /* then we place it as the largest possible value */
        semaphore->maximumCount = UINT64_MAX;
    }
}
