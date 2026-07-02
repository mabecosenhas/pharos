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
#include <pharos/hal/boardApi.h>
#include <pharos/kernel/interrupt/interrupt.h>
#include <pharos/kernel/object/protection.h>
#include <pharos/kernel/object/object.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/kernel/clock/clock.h>
#include <pharos/kernel/thread/rtthread.h>
#include <pharos/kernel/thread/dispatcher.h>
#include <pharos/kernel/thread/periodicthread.h>
#include <pharos/kernel/thread/sporadicthread.h>
#include <pharos/kernel/thread/aperiodicthread.h>
#include <pharos/kernel/thread/threadinline.h>
#include <pharos/kernel/scheduler/schedulerinline.h>


/**
 * Prepare the thread to be started. Initializes the state, deadlines, etc
 * 
 * @param id thread id 
 * @param thread address where to place the thread pointer
 * @param delay delay 
 * @param start address where to place the clock tick when the thread should start
 * 
 * @return returns the result of the start
 */
static PharosThreadStartR pharosIThreadStartPrepare(ptrThreadId id , ptrThreadInfo *thread , ClockTick delay , ClockTick *start);


void pharosIThreadStart(ptrThreadInfo thread , ClockTick delay , ClockTick *start)
{
    /* interrupt level */
    PharosInterruptLevel interrupt;


    /* reset the state */
    pharosIThreadStateInitialize(&thread->state);

    /* set the thread state as been started, so it cannot be started again */
    pharosIThreadStateSetStarted(&thread->state);

    /* re-initialize the application thread */
    pharosCpuThreadContextReInit(thread);

    /* disable interrupts so nothing can change the value of the current clock tick */
    pharosCpuDisableInterrupts(&interrupt);

    /* mark the release as now */
    *start = pharosIClockTicksGet(thread->scheduler);

    /* mark the last replenishment when the thread actually starts */
    thread->lastReplenishmentInstant = (*start) + delay;

    /* check if the thread isn't aperiodic (it is a RT thread otherwise) */
    if(pharosIThreadIsPeriodic(thread) == TRUE)
    {
        /* get the RT thread pointer */
        /* MISRA: can convert to a RtThread structure since it is not an aperiodic thread */
        ptrPeriodicThread periodic = (ptrPeriodicThread) thread;


        /* increment by the release offset */
        thread->lastReplenishmentInstant += periodic->release;

        /* if the thread has a deadline */
        if(periodic->rtThread.deadline != PHAROS_DEADLINE_IGNORED)
        {
            /* set the deadline timer */
            pharosITimeTickAddDeadline(&periodic->rtThread ,
                                       periodic->rtThread.thread.lastReplenishmentInstant + periodic->rtThread.deadline);
        }
    }

    /* re-enable interrupts*/
    pharosCpuEnableInterrupts(&interrupt);

    /* check if starting a sporadic thread */
    if(pharosIThreadIsSporadic(thread) == TRUE)
    {
        /* set the state to wait for activation */
        pharosIThreadStateWaitActivation(&thread->state);
    }

    /* disable interrupts so nothing can change the value of the current clock tick */
    pharosCpuDisableInterrupts(&interrupt);

    /* check if the thread activation instant already elapsed */
    if(thread->lastReplenishmentInstant <= pharosIClockTicksGet(thread->scheduler))
    {
        /* it did, so make the thread ready */
        pharosISchedAddReadyThread(thread);

        /* re-enable interrupts*/
        pharosCpuEnableInterrupts(&interrupt);

        /* if the thread started (restarted) is the running thread */
        if(pharosISchedGetRunningThread() == thread)
        {
            /* dispatch to another thread without saving the context of this thread 
             * (since it was just re-initialized in the TCB) */
            pharosIDispatchHeirThread();
        }
        else
        {
            /* a dispatch may be required since the restart restores the thread priority and state */
            pharosIDispatchThread();
        }
    }
    else
    {
        /* add a time tick handler to the thread */
        pharosITimeTickAddThread(thread , thread->lastReplenishmentInstant , PHAROS_TIME_TICK_AWAKEN_SLEEP , thread->scheduler);

        /* re-enable interrupts*/
        pharosCpuEnableInterrupts(&interrupt);
    }
}


PharosThreadStartR pharosIThreadStartPrepare(ptrThreadId id , ptrThreadInfo *thread , ClockTick delay , ClockTick *start)
{
    /* return value of this method */
    PharosThreadStartR result;

    /* interrupt level */
    PharosInterruptLevel interrupt;


    /* check that the id is at a valid address */
    if(pharosCpuBufferIsInsideProtection((uint8_t *) id , sizeof (ThreadId) , pharosIPartitionRunningMemProt()) == FALSE)
    {
        /* return invalid id */
        result = PHAROS_THREAD_START_INVALID_ID_ADDRESS;
    }
    else
    {
        /* get the thread with the specified id from the partition thread map */
        *thread = pharosIThreadGet(id);

        /* if found the thread */
        if((*thread) != NULL)
        {
            /* disable interrupts so nothing can change the value of the thread state */
            pharosCpuDisableInterrupts(&interrupt);

            /* check if the thread has already started */
            if(pharosIThreadStateIsStarted(&(*thread)->state) == TRUE)
            {
                /* re-enable interrupts*/
                pharosCpuEnableInterrupts(&interrupt);

                /* return error (already started) */
                result = PHAROS_THREAD_START_ALREADY_STARTED;
            }
            else
            {
                /* re-enable interrupts*/
                pharosCpuEnableInterrupts(&interrupt);

                /* start the thread */
                pharosIThreadStart(*thread , delay , start);

                /* return success */
                result = PHAROS_THREAD_START_SUCCESS;
            }
        }
        else
        {
            /* return invalid id */
            result = PHAROS_THREAD_START_INVALID_ID;
        }
    }

    /* return the result */
    return result;
}


PharosThreadStartR pharosSThreadStart(ptrThreadId id)
{
    /* return value of this method */
    PharosThreadStartR result;

    /* get the thread with the specified id from the partition thread map */
    ptrThreadInfo thread;

    /* clock tick when the thread starts (unused on this function) */
    ClockTick start;


    /* prepare the start of the thread (e.g. change the state to "isStarted") */
    result = pharosIThreadStartPrepare(id , &thread , 0U , &start);

    /* if everything went OK */
    if(result == PHAROS_THREAD_START_SUCCESS)
    {
        /* make the thread ready */
        pharosISchedAddReadyThread(thread);

        /* make any dispatch (if needed) */
        pharosIDispatchThread();
    }

    /* return the result */
    return result;
}


PharosThreadStartDelayR pharosSThreadStartDelay(ptrThreadId id , ClockTick delay)
{
    /* return value of this method */
    PharosThreadStartR result;

    /* thread with the specified id from the partition thread map */
    ptrThreadInfo thread;

    /* interrupt level */
    PharosInterruptLevel interrupt;

    /* clock tick when the thread starts (unused on this function) */
    ClockTick start;


    /* check if the delay is 0 */
    if(delay == 0U)
    {
        /* special case */
        result = pharosSThreadStart(id);
    }
    else
    {
        /* prepare the start of the thread (e.g. change the state to "isStarted") */
        result = pharosIThreadStartPrepare(id , &thread , delay , &start);
        
        /* if everything went OK */
        if(result == (PharosThreadStartR) PHAROS_THREAD_START_DELAY_SUCCESS)
        {
            /* disable interrupts so nothing can change the value of the global variables */
            pharosCpuDisableInterrupts(&interrupt);

            /* if the delay has not yet elapsed (interrupts were enabled so a dispatch could have occurred) */
            if(start + delay > pharosIClockTicksGet(thread->scheduler))
            {
                /* update state to waiting on sleep */
                pharosIThreadStateWaitSleep(&thread->state);

                /* add a time tick handler to the thread */
                pharosITimeTickAddThread(thread , start + delay , PHAROS_TIME_TICK_AWAKEN_SLEEP , thread->scheduler);

                /* re-enable interrupts*/
                pharosCpuEnableInterrupts(&interrupt);
            }
            else
            {
                /* re-enable interrupts (before adding the thread to the ready list */
                pharosCpuEnableInterrupts(&interrupt);

                /* then just place the thread in the ready state */
                pharosISchedAddReadyThread(thread);

                /* the new thread may have a higher priority, so make a dispatch (if needed) */
                pharosIDispatchThread();
            }
        }
    }

    /* return the result */
    return result;
}
