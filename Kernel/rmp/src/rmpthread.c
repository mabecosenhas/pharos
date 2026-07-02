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
#include <pharos/kernel/partition/partition.h>
#include <pharos/kernel/core/core.h>
#include <pharos/kernel/core/mpsc.h>
#include <pharos/kernel/core/coreconf.h>
#include <pharos/kernel/core/coreconfinline.h>


void pharosIThreadInitRmp(ptrThreadInfo thread)
{
    /* set the thread running core */
    thread->rmp.runningCore = pharosICore();
}


void pharosIThreadSuspend(ptrThreadInfo thread)
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
    ptrPharosRmpMessageThreadSuspend messageMpsc;


    /* if running on the same core */
    if(runningCore == threadCore)
    {
        /* just call the suspend on the local core */
        pharosIThreadSuspendOnSchedulerCore(thread);
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
        messageMpsc = &thread->rmp.threadSuspendMessage;

        /* set the message */
        node->message.threadSuspend = messageMpsc;

        /* create the multi-core message */
        node->type = PHAROS_RMP_MESSAGE_TYPE_THREAD_SUSPEND;

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


void pharosIThreadResume(ptrThreadInfo thread)
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
    ptrPharosRmpMessageThreadResume messageMpsc;


    /* if running on the same core */
    if(runningCore == threadCore)
    {
        /* just call the resume on the local core */
        pharosIThreadResumeOnSchedulerCore(thread);
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
        messageMpsc = &thread->rmp.threadResumeMessage;

        /* set the message */
        node->message.threadResume = messageMpsc;

        /* create the multi-core message */
        node->type = PHAROS_RMP_MESSAGE_TYPE_THREAD_RESUME;

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


void pharosIThreadStop(ptrThreadInfo thread)
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
    ptrPharosRmpMessageThreadStop messageMpsc;


    /* if running on the same core */
    if(runningCore == threadCore)
    {
        /* just call the stop on the local core */
        pharosIThreadStopOnSchedulerCore(thread);
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
        messageMpsc = &thread->rmp.threadStopMessage;

        /* set the message */
        node->message.threadStop = messageMpsc;

        /* create the multi-core message */
        node->type = PHAROS_RMP_MESSAGE_TYPE_THREAD_STOP;

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


void pharosIThreadUpdateCore(ptrThreadInfo thread , ptrCore core)
{
    /* update the scheduler of the running thread */
    thread->rmp.runningCore = core;
}


void pharosIThreadRestoreCore(ptrThreadInfo thread)
{
    /* restore the original scheduler */
    thread->rmp.runningCore = thread->partition->core;
}
