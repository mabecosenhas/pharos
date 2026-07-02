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


#include <pharos/kernel/semaphore/mutex.h>
#include <pharos/semaphore.h>
#include <pharos/hal/boardApi.h>
#include <pharos/kernel/interrupt/interrupt.h>
#include <pharos/kernel/thread/sporadicthread.h>
#include <pharos/kernel/thread/dispatcher.h>
#include <pharos/kernel/thread/aperiodicthread.h>
#include <pharos/clock.h>
#include <pharos/kernel/scheduler/scheduler.h>
#include <pharos/kernel/semaphore/mutexTable.h>
#include <pharos/kernel/object/object.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/kernel/scheduler/schedulerinline.h>


/**
 * Releases the semaphore
 * 
 * @constraint assumes the semaphore is a valid mutex 
 * 
 * @param semaphore semaphore to release
 * 
 * @return returns the result of the release (already release or success)
 */
static PharosSemMutexReleaseR pharosIMutexRelease(ptrSemaphoreMutex semaphore);


PharosSemMutexObtainR pharosSSemMutexObtain(SemMutexNumber number)
{
    /* return value of the function */
    PharosSemMutexObtainTimeoutR result;

    /* interrupt level */
    PharosInterruptLevel interrupt;

    /* semaphore to obtain */
    ptrSemaphoreMutex semaphore;


    /* get the semaphore */
    semaphore = pharosIGetSemaphoreMutex(number);

    /* if the semaphore id is invalid */
    if(semaphore == NULL)
    {
        /* return invalid */
        result = PHAROS_SEMAPHORE_MUTEX_OBTAIN_INVALID;
    }
    else
    {
        /* disable interrupts so nothing can change the value of the global variables */
        pharosCpuDisableInterrupts(&interrupt);

        /* check if the mutex semaphore is available */
        if(semaphore->mutexFlags.count == 1U)
        {
            /* decrease the count */
            semaphore->mutexFlags.count = 0U;

            /* re-enable interrupts */
            pharosCpuEnableInterrupts(&interrupt);

            result = PHAROS_SEMAPHORE_MUTEX_OBTAIN_SUCCESS;
        }
            /* mutex semaphore is unavailable */
        else
        {
            /* re-enable interrupts */
            pharosCpuEnableInterrupts(&interrupt);

            /* no timeout, return mutex semaphore unavailable */
            result = PHAROS_SEMAPHORE_MUTEX_OBTAIN_UNAVAILABLE;
        }
    }

    /* return result */
    return result;
}


PharosSemMutexObtainTimeoutR pharosISemMutexObtainTimeout(ptrThreadInfo thread , ptrSemaphoreMutex semaphore ,
                                                          Timeout timeout)
{
    /* return value of the method */
    PharosSemMutexObtainTimeoutR result;

    /* interrupt level */
    PharosInterruptLevel interrupt;


    /* disable interrupts so nothing can change the value of the global variables */
    pharosCpuDisableInterrupts(&interrupt);

    /* check if the semaphore is available */
    if(semaphore->mutexFlags.count == 1U)
    {
        /* decrease the count */
        semaphore->mutexFlags.count = 0U;

        /* update the activation of the sporadic thread */
        pharosIThreadSporadicActivate(thread);

        /* re-enable interrupts */
        pharosCpuEnableInterrupts(&interrupt);

        /* return success */
        result = PHAROS_SEMAPHORE_MUTEX_OBTAIN_SUCCESS;
    }
        /* mutex semaphore is unavailable */
    else
    {
        /* mutex semaphore is unavailable, check if we should wait for a timeout */
        if(timeout == PHAROS_TIMEOUT_ZERO)
        {
            /* re-enable interrupts */
            pharosCpuEnableInterrupts(&interrupt);

            /* no timeout, return mutex unavailable */
            result = PHAROS_SEMAPHORE_MUTEX_OBTAIN_TIMEOUT_UNAVAILABLE;
        }
            /* if waiting without timeout */
        else if(timeout == PHAROS_TIMEOUT_INFINITE)
        {
            /* going to block until the semaphore is released to this thread */
            pharosIThreadQueueAddBlocked(&semaphore->mutexBlockedQueue , thread);

            /* block the thread on the mutex semaphore */
            pharosIThreadStateBlockMutex(&thread->state);

            /* remove the thread from the ready list */
            pharosISchedRemoveReadyThread(thread);

            /* re-enable interrupts */
            pharosCpuEnableInterrupts(&interrupt);

            /* dispatch to highest priority ready thread */
            pharosIDispatchThread();

            /* since the timeout is infinite, if we get here then the semaphore was obtained */
            result = PHAROS_SEMAPHORE_MUTEX_OBTAIN_TIMEOUT_SUCCESS;
        }
            /* waiting with timeout */
        else
        {
            /* going to block until the semaphore is released to this thread */
            pharosIThreadQueueAddBlocked(&semaphore->mutexBlockedQueue , thread);

            /* block the thread on the semaphore */
            pharosIThreadStateBlockMutex(&thread->state);

            /* block the thread on a timeout */
            pharosIThreadStateWaitMutex(&thread->state);

            /* remove the thread from the ready list */
            pharosISchedRemoveReadyThread(thread);

            /* reset the unblock information */
            thread->threadFlags.unblockDueToTimeout = FALSE;

            /* add a tick handler at the timeout instant */
            pharosITimeTickAddThread(thread , timeout + pharosIClockTicksGet(thread->scheduler) ,
                                     PHAROS_TIME_TICK_AWAKEN_THREAD_SEMAPHORE_MUTEX_OBTAIN_TIMEOUT ,
                                     thread->scheduler);

            pharosCpuEnableInterrupts(&interrupt);

            /* dispatch to highest priority ready thread */
            pharosIDispatchThread();

            /* get the result of the timeout (semaphore was released or timeout occurred?) */
            if(thread->threadFlags.unblockDueToTimeout == TRUE)
            {
                /* there was a timeout */
                result = PHAROS_SEMAPHORE_MUTEX_OBTAIN_TIMEOUT_TIMEOUT;
            }
            else
            {
                /* mutex semaphore was released and this thread obtained it before the timeout expired */
                result = PHAROS_SEMAPHORE_MUTEX_OBTAIN_TIMEOUT_SUCCESS;
            }
        }
    }

    /* return success or the error condition */
    return result;
}


PharosSemMutexObtainTimeoutR pharosSSemMutexObtainTimeout(SemMutexNumber number , Timeout timeout)
{
    /* method return value */
    PharosSemMutexObtainTimeoutR result;

    /* semaphore to obtain */
    ptrSemaphoreMutex semaphore;

    /* calling thread */
    ptrThreadInfo runningThread;


    /* get the semaphore */
    semaphore = pharosIGetSemaphoreMutex(number);

    /* get the running thread */
    runningThread = pharosISchedGetRunningThread();

    /* if the semaphore id is invalid */
    if(semaphore == NULL)
    {
        /* return invalid */
        result = PHAROS_SEMAPHORE_MUTEX_OBTAIN_TIMEOUT_INVALID;
    }
        /* check if is running inside an interrupt */
    else if(pharosIInterruptIsRunning() == TRUE)
    {
        /* inside an interrupt cannot obtain semaphores */
        result = PHAROS_SEMAPHORE_MUTEX_OBTAIN_TIMEOUT_INSIDE_INTERRUPT;
    }
    else
    {
        /* get the semaphore result */
        result = pharosISemMutexObtainTimeout(runningThread , semaphore , timeout);
    }

    /* return success or the error condition */
    return result;
}


PharosSemMutexReleaseR pharosSSemMutexRelease(SemMutexNumber number)
{
    /* result of the method */
    PharosSemMutexReleaseR result;

    /* get the mutex */
    ptrSemaphoreMutex semaphore = pharosIGetSemaphoreMutex(number);


    /* if the semaphore id is invalid */
    if(semaphore == NULL)
    {
        /* return invalid */
        result = PHAROS_SEMAPHORE_MUTEX_RELEASE_INVALID;
    }
    else
    {
        /* release the mutex */
        result = pharosIMutexRelease(semaphore);
    }

    /* return the result of the mutex release */
    return result;
}


PharosSemMutexReleaseR pharosIMutexRelease(ptrSemaphoreMutex semaphore)
{
    /* return value of the method */
    PharosSemMutexReleaseR result;

    /* interrupt level */
    PharosInterruptLevel interrupt;

    /* highest thread that was blocked on the mutex semaphore */
    ptrThreadInfo highestThread;


    /* disable interrupts so nothing can change the value of the global variables */
    pharosCpuDisableInterrupts(&interrupt);

    /* if semaphore is already released */
    if(semaphore->mutexFlags.count == 1U)
    {
        /* can re-enable interrupts */
        pharosCpuEnableInterrupts(&interrupt);

        /* return that the semaphore was already released */
        result = PHAROS_SEMAPHORE_MUTEX_RELEASE_ALREADY_RELEASED;
    }
    else
    {
        /*get the highest priority thread blocked on the semaphore */
        highestThread = pharosIThreadQueueGetHighest(&semaphore->mutexBlockedQueue);

        /* if there is no other higher priority thread blocked on the semaphore */
        if(highestThread == NULL)
        {
            /* then set the semaphore internal count to 1 */
            semaphore->mutexFlags.count = 1U;

            /* can re-enable interrupts now */
            pharosCpuEnableInterrupts(&interrupt);
        }
        else
        {
            /* remove the highest thread from the queue of threads blocked on the semaphore */
            pharosIThreadQueueRemoveBlocked(highestThread);

            /* update the activation (if needed) */
            pharosIThreadSporadicActivate(highestThread);

            /* remove the thread timeout time tick (if needed). Sporadic threads waiting for
             * activation don't have time tick handler */
            pharosITimeTickRemoveThread(highestThread);

            /* unblock the thread state */
            pharosIThreadStateUnblock(&highestThread->state);

            /* remove the wait state from the thread state */
            pharosIThreadStateUnwait(&highestThread->state);

            /* activate the thread */
            pharosISchedAddReadyThread(highestThread);

            /* can re-enable interrupts now */
            pharosCpuEnableInterrupts(&interrupt);

            /* dispatch */
            pharosIDispatchThread();
        }

        /* result is success */
        result = PHAROS_SEMAPHORE_MUTEX_RELEASE_SUCCESS;
    }

    /* return success or the error */
    return result;
}


void pharosISemsMutexInitialize(const ptrSemaphoreMutexTable table)
{
    /* iterator */
    ObjectNumber i;


    /* loop through all the mutex semaphores */
    for(i = 0U; i < table->size; i++)
    {
        /* get the semaphore to initialize */
        ptrSemaphoreMutex semaphore = &table->mutexSemaphores[i];

        /* initialize the queue of threads blocked on the mutex */
        pharosIThreadQueueInitialize(&semaphore->mutexBlockedQueue , semaphore->mutexFlags.isFifo);
    }
}


ptrSemaphoreMutex pharosIGetSemaphoreMutex(SemMutexNumber number)
{
    /* mutex semaphore to return */
    ptrSemaphoreMutex result;

    ptrSemaphoreMutexTable table = &pharosIPartitionRunObjectTable()->mutexSemaphoreTable;


    /* if the id is too great */
    if(number >= table->size)
    {
        /* return invalid */
        result = NULL;
    }
    else
    {
        /* get the mutex semaphore address */
        result = &table->mutexSemaphores[number];
    }

    /* return the mutex semaphore got */
    return result;
}
