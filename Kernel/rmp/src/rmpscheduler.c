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


#include <pharos/kernel/scheduler/scheduler.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/kernel/core/rmpscheduler.h>
#include <pharos/kernel/assert/assert.h>


void pharosISchedAddReadyThreadRmp(ptrThreadInfo thread)
{
    /* get the running core */
    CoreNumber runningCore = pharosICoreRunning();

    /* get the thread core */
    CoreNumber threadCore = thread->partition->core->number;

    /* node to send the message to the other core with */
    ptrPharosMpscNode node;

    /* queue on the other core to send */
    ptrPharosMpscQueue mpscQueue;

    /* message to send */
    ptrPharosRmpMessageThreadReady messageMpsc;


    /* if running on the same core */
    if(runningCore == threadCore)
    {
        /* just call the ready thread */
        pharosISchedAddReadyThread(thread);
    }
        /* else, must be running on a different core */
    else
    {
        /* message to send to the other core */
        node = &thread->rmp.mpscThreadReadyNode[threadCore];

        /* get the MPSC queue to send to on the channel owner core */
        mpscQueue = &pharosICoreGet(threadCore)->rmp.queue;

        /* get the correct node to use to send the message */
        node = pharosIMpscQueueGetMessage(mpscQueue , node);

        /* get the message */
        messageMpsc = &thread->rmp.threadReadyMessage;

        /* set the message */
        node->message.threadReady = messageMpsc;

        /* create the multi-core message */
        node->type = PHAROS_RMP_MESSAGE_TYPE_SCHEDULER_THREAD_READY;

        messageMpsc->thread = thread;

        /* make sure that every memory store is done before sending the message */
        __atomic_thread_fence(__ATOMIC_RELEASE);

        /* push the message to the MPSC queue */
        pharosIMpscQueuePush(mpscQueue , node);

        /* make sure that the message has been placed in the queue before triggering the interrupt */
        __atomic_thread_fence(__ATOMIC_RELEASE);

        /* send an IPI call to the core queue owner */
        pharosCpuMulticoreTriggerInterrupt(threadCore);
    }
}


void pharosIThreadQueueUpdateThreadPrio(ptrThreadInfo thread , ThreadPriority newPriority)
{
    /* get the running core */
    CoreNumber runningCore = pharosICoreRunning();

    /* get the thread core */
    CoreNumber threadCore = thread->rmp.runningCore->number;

    /* node to send the message to the other core with */
    ptrPharosMpscNode node;

    /* queue on the other core to send */
    ptrPharosMpscQueue mpscQueue;

    /* message to send */
    ptrPharosRmpMessageSchedulerThreadUpdatePriority messageMpsc;


    /* if running on the same core */
    if(runningCore == threadCore)
    {
        /* update the thread priority on the scheduler core (this one, since there is only one) */
        pharosIThreadQueueUpdateThreadPrioOnSchedulerCore(thread , newPriority);

        /* update the thread priority on the thread core */
        pharosIThreadQueueUpdateThreadPrioOnThreadCore(thread , newPriority);
    }
        /* else, must be running on a different core */
    else
    {
        /* update the thread priority on the thread core */
        pharosIThreadQueueUpdateThreadPrioOnThreadCore(thread , newPriority);

        /* message to send to the other core */
        node = &thread->rmp.mpscThreadSchedulerPriorityUpdate[threadCore];

        /* get the MPSC queue to send to on the channel owner core */
        mpscQueue = &pharosICoreGet(threadCore)->rmp.queue;

        /* get the correct node to use to send the message */
        node = pharosIMpscQueueGetMessage(mpscQueue , node);

        /* get the message */
        messageMpsc = &thread->rmp.threadPriorityMessage;

        /* set the message */
        node->message.threadPriority = messageMpsc;

        /* create the multi-core message */
        node->type = PHAROS_RMP_MESSAGE_TYPE_SCHEDULER_THREAD_UPDATE_PRIORITY;

        messageMpsc->thread = thread;
        messageMpsc->newPriority = newPriority;

        /* make sure that every memory store is done before sending the message */
        __atomic_thread_fence(__ATOMIC_RELEASE);

        /* push the message to the MPSC queue */
        pharosIMpscQueuePush(mpscQueue , node);

        /* make sure that the message has been placed in the queue before triggering the interrupt */
        __atomic_thread_fence(__ATOMIC_RELEASE);

        /* send an IPI call to the core queue owner */
        pharosCpuMulticoreTriggerInterrupt(threadCore);
    }
}


void pharosITimeActivateThreadRemote(ptrThreadInfo thread)
{
    /* interrupt level */
    PharosInterruptLevel level;


    /* check if running in the correct core */
    PHAROS_ASSERT_CORE(thread->rmp.runningCore->number , "Invalid core");

    /* disable interrupts so nothing can change the value of the global variables */
    pharosCpuDisableInterrupts(&level);

    /* check if the thread actually got the bulk */
    if(thread->blockedQueue == NULL)
    {
        /* thread already no longer blocked on an object of the core, so can be made ready without timeout */
        thread->threadFlags.unblockDueToTimeout = FALSE;

        /* restore the thread core */
        pharosIThreadRestoreCore(thread);
    }
    else
    {
        /* timeout occurred while the thread was blocked */
        thread->threadFlags.unblockDueToTimeout = TRUE;

        /* thread is still blocked on a queue, so we have to remove it */
        pharosIThreadQueueRemoveBlocked(thread);

        /* unblock the thread (thread is no longer blocked on whatever item it was blocked on - semaphore, event, queue, etc) */
        pharosIThreadStateUnblock(&thread->state);
    }

    /* re-enable interrupts*/
    pharosCpuEnableInterrupts(&level);

    /* make the thread ready on the running core */
    pharosISchedAddReadyThreadRmp(thread);
}


void pharosITimeActivateThreadRmp(ptrThreadInfo thread)
{
    /* get the running core */
    CoreNumber runningCore = pharosICoreRunning();

    /* get the thread core */
    CoreNumber threadCore = thread->rmp.runningCore->number;

    /* node to send the message to the other core with */
    ptrPharosMpscNode node;

    /* queue on the other core to send */
    ptrPharosMpscQueue mpscQueue;

    /* message to send */
    ptrPharosRmpMessageThreadActivate messageMpsc;


    /* if running on the same core */
    if(runningCore == threadCore)
    {
        /* activate the thread on this core */
        pharosITimeActivateThread(thread);
    }
        /* else, must be running on a different core */
    else
    {
        /* thread is no longer waiting */
        pharosIThreadStateUnwait(&thread->state);

        /* message to send to the other core */
        node = &thread->rmp.mpscThreadActivate[threadCore];

        /* get the MPSC queue to send to on the channel owner core */
        mpscQueue = &pharosICoreGet(threadCore)->rmp.queue;

        /* get the correct node to use to send the message */
        node = pharosIMpscQueueGetMessage(mpscQueue , node);

        /* get the message */
        messageMpsc = &thread->rmp.threadActivateMessage;

        /* set the message */
        node->message.threadActivate = messageMpsc;

        /* create the multi-core message */
        node->type = PHAROS_RMP_MESSAGE_TYPE_THREAD_ACTIVATE;

        messageMpsc->thread = thread;

        /* make sure that every memory store is done before sending the message */
        __atomic_thread_fence(__ATOMIC_RELEASE);

        /* push the message to the MPSC queue */
        pharosIMpscQueuePush(mpscQueue , node);

        /* make sure that the message has been placed in the queue before triggering the interrupt */
        __atomic_thread_fence(__ATOMIC_RELEASE);

        /* send an IPI call to the core queue owner */
        pharosCpuMulticoreTriggerInterrupt(threadCore);
    }
}
