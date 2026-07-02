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


#include <pharos/kernel/event/event.h>
#include <pharos/event.h>
#include <pharos/kernel/thread/dispatcher.h>
#include <pharos/kernel/interrupt/interrupt.h>
#include <pharos/kernel/thread/sporadicthread.h>
#include <pharos/kernel/thread/thread.h>
#include <pharos/hal/board.h>
#include <pharos/kernel/partition/partition.h>
#include <pharos/hal/boardApi.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/kernel/scheduler/schedulerinline.h>


/**
 * Send an event to a thread 
 * 
 * @param thread thread to send the event to
 * @param event event to send
 * 
 * @return returns the result of the send
 */
static void eventSend(ptrThreadInfo thread , Event event);


PharosEventSendR pharosSEventSend(ptrThreadId threadId , Event event)
{
    /* result of the function */
    PharosEventSendR result;

    /* get the thread to where to send the event */
    ptrThreadInfo thread;


    /* check if the address of the thread id is valid */
    if(pharosCpuBufferIsInsideProtection((uint8_t *) threadId , sizeof (ThreadId) , pharosIPartitionRunningMemProt()) == FALSE)
    {
        /* return invalid address */
        result = PHAROS_EVENT_SEND_THREAD_ID_INVALID_ADDRESS;
    }
    else
    {
        /* get the thread associated to the id */
        thread = pharosIThreadGet(threadId);

        /* if there is no such thread */
        if(thread == NULL)
        {
            /* return invalid thread */
            result = PHAROS_EVENT_SEND_INVALID;
        }
            /* if the thread is valid */
        else
        {
            /* send the event to the thread */
            eventSend(thread , event);

            /* return success */
            result = PHAROS_EVENT_SEND_SUCCESS;
        }
    }

    /* return the result of the event send */
    return result;
}


PharosEventReceiveR pharosSEventReceive(ptrEvent event , Timeout timeout)
{
    /* result of the method */
    PharosEventReceiveR result;


    /* check if we are inside an interrupt */
    if(pharosIInterruptIsRunning() == TRUE)
    {
        /* called blocking call inside interrupt */
        result = PHAROS_EVENT_RECEIVE_INSIDE_INTERRUPT;
    }
        /* check if event is in a valid address */
    else if(pharosCpuBufferIsInsideProtection((uint8_t *) event , sizeof (Event) , pharosIPartitionRunningMemProt()) == FALSE)
    {
        /* it is not, return invalid */
        result = PHAROS_EVENT_RECEIVE_EVENT_INVALID;
    }
        /* else, everything is fine */
    else
    {
        /* preliminary checks are OK, proceed with the reception */
        result = pharosIEventReceiveUnprotected(timeout , event);
    }

    /* return the result */
    return result;
}


void eventSend(ptrThreadInfo thread , Event event)
{
    /* interrupt level */
    PharosInterruptLevel level;


    /* disable interrupts to protect against events sent inside interrupts */
    pharosCpuDisableInterrupts(&level);

    /* store the event on the thread (replace any previous event) */
    thread->unblockInfo.event = event;

    /* update the activation of the sporadic thread (if the thread is sporadic and if needed) */
    pharosIThreadSporadicActivate(thread);

    /* check if the thread is waiting for an event and is awake 
     * Why is important the "awake" part: 
     *  imagine the scenario where the thread is waiting for an event with a timeout. Suppose the timeout already
     *  elapsed but due other higher priority threads, the thread waiting for the event was not yet awaken (and its 
     *  state updated).
     *  In this case, the thread shall not consume the event because at the instant it was given the event the timeout 
     *  had already occurred.
     */
    if((pharosIThreadStateIsWaitEvent(&thread->state) == TRUE) && (pharosIThreadIsAwake(thread) == FALSE))
    {
        /* unblock the thread state */
        pharosIThreadStateUnblock(&thread->state);

        /* remove the wait state from the thread state */
        pharosIThreadStateUnwait(&thread->state);

        /* remove any timeout that the thread may be on */
        pharosITimeTickRemoveThread(thread);

        /* add the thread to the ready list */
        pharosISchedAddReadyThread(thread);

        /* thread just consumed the event */
        thread->threadFlags.isEventWaiting = FALSE;

        /* re-enable interrupts */
        pharosCpuEnableInterrupts(&level);

        /* dispatch to next thread (if needed) */
        pharosIDispatchThread();
    }
        /* thread was not waiting for event, leave it there */
    else
    {
        /* set that there is an event waiting to be processed */
        thread->threadFlags.isEventWaiting = TRUE;

        /* re-enable interrupts */
        pharosCpuEnableInterrupts(&level);
    }
}


PharosEventReceiveR pharosIEventReceiveUnprotected(Timeout timeout , ptrEvent event)
{
    /* result of the method */
    PharosEventReceiveR result;

    /* interrupt level */
    PharosInterruptLevel level;

    /* running thread */
    ptrThreadInfo thread;


    /* get the running thread */
    thread = pharosISchedGetRunningThread();

    /* disable interrupts to protect against sends inside interrupts */
    pharosCpuDisableInterrupts(&level);

    /* check if there is an event waiting to be processed */
    if(thread->threadFlags.isEventWaiting == TRUE)
    {
        /* reset the waiting flag since the event is to be processed right now */
        thread->threadFlags.isEventWaiting = FALSE;

        /* copy the event */
        *event = thread->unblockInfo.event;

        /* re-enable interrupts */
        pharosCpuEnableInterrupts(&level);

        /* return success */
        result = PHAROS_EVENT_RECEIVE_SUCCESS;
    }
        /* event was not available, see if there is a timeout */
    else if(timeout == PHAROS_TIMEOUT_ZERO)
    {
        /* no timeout and return right away */
        pharosCpuEnableInterrupts(&level);

        /* event not available */
        result = PHAROS_EVENT_RECEIVE_UNAVAILABLE;
    }
        /* check if should wait forever */
    else if(timeout == PHAROS_TIMEOUT_INFINITE)
    {
        /* wait forever until the event is received */
        pharosIThreadStateBlockEvent(&thread->state);

        /* thread is waiting for event */
        pharosIThreadStateWaitEvent(&thread->state);

        /* thread no longer ready */
        pharosISchedRemoveReadyThread(thread);

        /* re-enable interrupts */
        pharosCpuEnableInterrupts(&level);

        /* dispatch to highest priority ready thread */
        pharosIDispatchThread();

        /* copy the event */
        *event = thread->unblockInfo.event;

        /* return success */
        result = PHAROS_EVENT_RECEIVE_SUCCESS;
    }
        /* else must be wait with a timeout */
    else
    {
        /* set the thread is waiting on event */
        thread->threadFlags.isEventWaiting = TRUE;

        /* set the thread is blocked on event */
        pharosIThreadStateBlockEvent(&thread->state);

        /* thread is waiting for event */
        pharosIThreadStateWaitEvent(&thread->state);

        /* thread no longer ready */
        pharosISchedRemoveReadyThread(thread);

        /* add the time handler */
        pharosITimeTickAddThread(thread , timeout + pharosIClockTicksGet(thread->scheduler) ,
                                 PHAROS_TIME_TICK_AWAKEN_THREAD_EVENT_RECEIVE_TIMEOUT , thread->scheduler);

        /* can re-enable interrupts now */
        pharosCpuEnableInterrupts(&level);

        /* dispatch to highest priority ready thread */
        pharosIDispatchThread();

        /* if no longer waiting for an event */
        if(thread->threadFlags.isEventWaiting == FALSE)
        {
            /* copy the event */
            *event = thread->unblockInfo.event;

            /* return success */
            result = PHAROS_EVENT_RECEIVE_SUCCESS;
        }
        else
        {
            /* set that the thread is no longer waiting for an event */
            thread->threadFlags.isEventWaiting = FALSE;

            /* return success */
            result = PHAROS_EVENT_RECEIVE_TIMEOUT;
        }
    }

    /* return the result */
    return result;
}
