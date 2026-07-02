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
#include <pharos/kernel/scheduler/scheduler.h>
#include <pharos/kernel/thread/dispatcher.h>
#include <pharos/kernel/thread/aperiodicthread.h>
#include <pharos/kernel/interrupt/interrupt.h>
#include <pharos/semaphore.h>
#include <pharos/kernel/thread/sporadicthread.h>
#include <pharos/kernel/semaphore/ceiling.h>
#include <pharos/kernel/partition/partition.h>
#include <pharos/kernel/thread/queue.h>
#include <pharos/hal/boardApi.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/kernel/thread/priority.h>
#include <pharos/kernel/thread/threadinline.h>
#include <pharos/kernel/scheduler/schedulerinline.h>


/**
 * get a semaphore given its id
 * 
 * @param number semaphore number on the running partition
 * 
 * @return returns the semaphore or NULL if the number is invalid
 */
static ptrSemaphoreCeiling pharosISemCeilGet(SemCeilNumber number);


PharosSemCeilObtainR pharosISemCeilObtain(ptrSemaphoreCeiling semaphore , Timeout timeout , ptrResource resource)
{
    /* running thread */
    ptrThreadInfo thread;

    /* interrupt level */
    PharosInterruptLevel level;

    /* result of this function */
    PharosSemCeilObtainR result;


    /* get the running thread */
    thread = pharosISchedGetRunningThread();

    /* check if is running inside an interrupt */
    if(pharosIInterruptIsRunning() == TRUE)
    {
        /* inside an interrupt cannot obtain semaphores */
        result = PHAROS_SEMAPHORE_CEILING_OBTAIN_INSIDE_INTERRUPT;
    }
        /* else, we can try to get the valid semaphore call */
    else
    {
        /* disable interrupts so nothing can change the value of the global variables */
        pharosCpuDisableInterrupts(&level);

        /* if the ceiling is too low */
        if(thread->currentPriority < semaphore->ceiling)
        {
            /* re-enable interrupts */
            pharosCpuEnableInterrupts(&level);

            /* then there is an error (no thread can obtain a semaphore with a ceiling with higher priority than the ceiling) */
            result = PHAROS_SEMAPHORE_CEILING_OBTAIN_INVALID_CEILING_PRIORITY;
        }
        else
        {
            /* check if the semaphore has already been obtained */
            if(semaphore->holder != NULL)
            {
                /* if the holder is the calling thread */
                if(semaphore->holder == thread)
                {
                    /* re-enable interrupts */
                    pharosCpuEnableInterrupts(&level);

                    /* return that the calling thread already holds the semaphore */
                    result = PHAROS_SEMAPHORE_CEILING_OBTAIN_ALREADY_HOLDER;
                }
                else
                {
                    /* semaphore is not available, check if have to wait */
                    if(timeout == PHAROS_TIMEOUT_ZERO)
                    {
                        /* re-enable interrupts */
                        pharosCpuEnableInterrupts(&level);

                        /* return the that the semaphore is not available */
                        result = PHAROS_SEMAPHORE_CEILING_OBTAIN_NOT_AVAILABLE;
                    }
                    else if(timeout == PHAROS_TIMEOUT_INFINITE)
                    {
                        /* going to block until the semaphore is released to this thread */
                        pharosIThreadQueueAddBlocked(&semaphore->blockedQueue , thread);

                        /* block the thread on the counting semaphore */
                        pharosIThreadStateBlockCeil(&thread->state);

                        /* remove the thread from the ready list */
                        pharosISchedRemoveReadyThread(thread);

                        /* re-enable interrupts */
                        pharosCpuEnableInterrupts(&level);

                        /* dispatch to highest priority ready thread */
                        pharosIDispatchThread();

                        /* since the timeout is infinite, if we get here then the semaphore was obtained */
                        result = PHAROS_SEMAPHORE_CEILING_OBTAIN_SUCCESS;
                    }
                    else
                    {
                        /* going to block until the semaphore is released to this thread */
                        pharosIThreadQueueAddBlocked(&semaphore->blockedQueue , thread);

                        /* block the thread on the counting semaphore */
                        pharosIThreadStateBlockCeil(&thread->state);

                        /* block the thread on a timeout */
                        pharosIThreadStateWaitCeil(&thread->state);

                        /* remove the thread from the ready list */
                        pharosISchedRemoveReadyThread(thread);

                        /* reset the unblock information */
                        thread->threadFlags.unblockDueToTimeout = FALSE;

                        /* add a tick handler at the timeout instant */
                        pharosITimeTickAddThread(thread , timeout + pharosIClockTicksGet(thread->scheduler) ,
                                                 PHAROS_TIME_TICK_AWAKEN_THREAD_SEMAPHORE_CEILING_OBTAIN_TIMEOUT ,
                                                 thread->scheduler);

                        /* re-enable interrupts */
                        pharosCpuEnableInterrupts(&level);

                        /* dispatch to highest priority ready thread */
                        pharosIDispatchThread();

                        /* get the result of the timeout (semaphore was released or timeout occurred?) */
                        if(thread->threadFlags.unblockDueToTimeout == TRUE)
                        {
                            /* there was a timeout */
                            result = PHAROS_SEMAPHORE_CEILING_OBTAIN_TIMEOUT;
                        }
                        else
                        {
                            /* semaphore was released and this thread obtained it before the timeout expired */
                            result = PHAROS_SEMAPHORE_CEILING_OBTAIN_SUCCESS;
                        }
                    }
                }
            }
                /* else, semaphore is available */
            else
            {
                /* add the resource */
                pharosIThreadResourceAdd(thread , resource);

                /* if the thread has lower priority than the ceiling */
                if(thread->currentPriority > semaphore->ceiling)
                {
                    /* then raise the priority of the thread */
                    pharosISchedUpdateThreadPrio(thread , semaphore->ceiling);
                }
                /* else, priorities are equal so no need to change the thread priority */

                /* update the semaphore holder (only the holder can release it) */
                semaphore->holder = thread;

                /* add the semaphore to the running thread semaphore hold list */
                pharosIThreadAddCeiling(thread , semaphore);

                /* re-enable interrupts */
                pharosCpuEnableInterrupts(&level);

                /* return success */
                result = PHAROS_SEMAPHORE_CEILING_OBTAIN_SUCCESS;
            }
        }

    }

    return result;
}


PharosSemCeilObtainR pharosSSemCeilObtain(SemCeilNumber number , Timeout timeout)
{

    /* result of this function */
    PharosSemCeilObtainR result;

    /* semaphore to obtain */
    ptrSemaphoreCeiling semaphore;


    /* get the semaphore */
    semaphore = pharosISemCeilGet(number);

    /* if current thread priority is lower than the ceiling */
    if(semaphore == NULL)
    {
        /* it isn't valid, return error */
        result = PHAROS_SEMAPHORE_CEILING_OBTAIN_INVALID;
    }
    else
    {
        /* obtain the semaphore */
        result = pharosISemCeilObtain(semaphore , timeout , NULL);
    }

    /* return result of function */
    return result;
}


void pharosISemCeilInitialize(ptrPartition id , ptrSemaphoreCeiling semaphore , bool isResource)
{
    /* reset the holder */
    semaphore->holder = NULL;

    /* initialize the thread queue */
    pharosIThreadQueueInitialize(&semaphore->blockedQueue , semaphore->ceilingFlags.isFifo);

    /* initialize the list of semaphores with same priority which a thread holds */
    pharosIThreadQueueInitSemCeil(&semaphore->blockedQueueNode);

    /* update the owner */
    semaphore->owner = id;
}


void pharosISemsCeilInitialize(ptrPartition id , const ptrSemaphoreTable table)
{
    /* iterator */
    ObjectNumber i;


    /* loop through all the semaphores */
    for(i = 0U; i < table->size; i++)
    {
        /* get the semaphore to initialize */
        ptrSemaphoreCeiling semaphore = &table->ceilingSemaphores[i];

        /* initialize the semaphore */
        pharosISemCeilInitialize(id , semaphore , FALSE);
    }
}


PharosSemCeilReleaseR pharosISemCeilRelease(ptrSemaphoreCeiling semaphore , ptrThreadInfo runningThread , ptrResource resource)
{
    /* result of this function */
    PharosSemCeilReleaseR result;

    /* highest priority thread that is blocked on the semaphore and will get it */
    ptrThreadInfo highestThread;

    /* interrupt level */
    PharosInterruptLevel level;


    /* disable interrupts so nothing can change the value of the global variables */
    pharosCpuDisableInterrupts(&level);

    /* if the calling thread is not the holder */
    if(runningThread != semaphore->holder)
    {
        /* re-enable interrupts */
        pharosCpuEnableInterrupts(&level);

        /* then cannot release it */
        result = PHAROS_SEMAPHORE_CEILING_RELEASE_NOT_HOLDER;
    }
    else
    {
        /* remove the resource from the list of resources hold by the running thread */
        pharosIThreadResourceRemove(runningThread , resource);

        /* check if there is another thread blocked on the semaphore */
        highestThread = pharosIThreadQueueGetHighest(&semaphore->blockedQueue);

        /* remove the semaphore hold */
        pharosIThreadRemoveCeiling(runningThread , semaphore);

        /* if there is no other higher priority thread blocked on the semaphore */
        if(highestThread == NULL)
        {
            /* reset the holder thread (no thread holds the semaphore) */
            semaphore->holder = NULL;
        }
        else
        {
            /* add the resource to the thread */
            pharosIThreadResourceAdd(highestThread , resource);

            /* remove the highest thread from the queue of threads blocked on the semaphore */
            pharosIThreadQueueRemoveBlocked(highestThread);

            /* update the thread that holds the semaphore */
            semaphore->holder = highestThread;

            /* remove the thread timeout time tick (if needed). Sporadic threads waiting for activation don't have time tick handler */
            pharosITimeTickRemoveThread(highestThread);

            /* unblock the thread state */
            pharosIThreadStateUnblock(&highestThread->state);

            /* remove the wait state from the thread state */
            pharosIThreadStateUnwait(&highestThread->state);

            /* add the semaphore to the running thread semaphore hold list */
            pharosIThreadAddCeiling(highestThread , semaphore);

            /* activate the thread */
            pharosISchedAddReadyThread(highestThread);
        }

        /* update the priority of the thread to the highest ceiling or the base priority of the thread */
        semaphore = pharosIThreadGetHighestCeiling(runningThread);

        /* if there is no other semaphore hold */
        if(semaphore == NULL)
        {
            /* no semaphores hold by the thread, lets restore it to the base priority */
            pharosIThreadRestorePriority(runningThread);
        }
        else
        {
            /* update the thread priority to the highest ceiling */
            pharosISchedUpdateThreadPrio(runningThread , semaphore->ceiling);
        }

        /* re-enable interrupts */
        pharosCpuEnableInterrupts(&level);

        /* make sure to make a thread dispatch check */
        pharosIThreadContextSwitchNeeded(TRUE);

        /* dispatch to any higher priority thread */
        pharosIDispatchThread();

        /* release was successful */
        result = PHAROS_SEMAPHORE_CEILING_RELEASE_SUCCESS;
    }

    return result;
}


PharosSemCeilReleaseR pharosSSemCeilRelease(SemCeilNumber number)
{
    /* result of this function */
    PharosSemCeilReleaseR result;

    /* get the semaphore pointer */
    ptrSemaphoreCeiling semaphore;

    /* running thread that is attempting to release the semaphore */
    ptrThreadInfo runningThread;


    /* get the semaphore */
    semaphore = pharosISemCeilGet(number);

    /* get the running thread */
    runningThread = pharosISchedGetRunningThread();

    /* check if the semaphore is valid */
    if(semaphore == NULL)
    {
        /* it isn't valid, return error */
        result = PHAROS_SEMAPHORE_CEILING_RELEASE_INVALID;
    }

        /* check if is running inside an interrupt */
    else if(pharosIInterruptIsRunning() == TRUE)
    {
        /* inside an interrupt cannot release semaphores with priority ceiling */
        result = PHAROS_SEMAPHORE_CEILING_RELEASE_INSIDE_INTERRUPT;
    }
    else
    {
        /* release the semaphore */
        result = pharosISemCeilRelease(semaphore , runningThread , NULL);

    }

    /* return result of function */
    return result;
}


ptrSemaphoreCeiling pharosISemCeilGet(SemCeilNumber number)
{
    /* semaphore to return */
    ptrSemaphoreCeiling semaphore;

    ptrSemaphoreTable table = &pharosIPartitionRunObjectTable()->ceilingSemaphoreTable;

    /* if the id is too great */
    if(number >= table->size)
    {
        /* return invalid */
        semaphore = NULL;
    }
    else
    {
        /* get the semaphore address */
        semaphore = &table->ceilingSemaphores[number];
    }

    /* return the semaphore got */
    return semaphore;
}
