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
#include <pharos/kernel/assert/assert.h>


void pharosIFutureReady(ptrFuture future , ptrFutureArg arg)
{
    /* get the running core */
    CoreNumber core = pharosICoreRunning();

    /* get the core that owns the queue */
    CoreNumber owner = future->owner->core->number;

    /* node to send the message to the other core with */
    ptrPharosMpscNode node;

    /* message to send */
    ptrPharosRmpMessageFutureReady message;

    /* queue on the other core to send */
    ptrPharosMpscQueue queue;

    /* interrupt level */
    PharosInterruptLevel level;


    /* check that the function is called with a valid future */
    PHAROS_ASSERT(future != NULL , "future is invalid");

    /* check if the future is being ready on the same core as the owner */
    if(core == owner)
    {
        /* disable interrupts so nothing can change the value of the futures held */
        pharosCpuDisableInterrupts(&level);

        /* if the running partition is not the future holder */
        if((future->tempPartition != NULL) && (future->tempPartition != future->owner))
        {
            /* then remove the future from the list of external futures held by the partition */
            linkedListRemoveObject(&future->tempPartition->externalFuturesHold , future);
        }

        /* re-enable interrupts */
        pharosCpuEnableInterrupts(&level);

        /* then do the ready on the future owner core */
        pharosIFutureReadyOnOwnerCore(future , arg);
    }
    else
    {
        /* message to send to the other core */
        node = &future->rmp.mpscReadyNode[owner];

        /* get the MPSC queue to send to on the channel owner core */
        queue = &pharosICoreGet(owner)->rmp.queue;

        /* get the correct node to use to send the message */
        node = pharosIMpscQueueGetMessage(queue , node);

        /* get the message */
        message = &future->rmp.readyMessage;

        /* set the message */
        node->message.futureReady = message;

        /* create the multi-core message */
        node->type = PHAROS_RMP_MESSAGE_TYPE_FUTURE_READY;
        message->future = future;

        if(arg != NULL)
        {
            message->arg = *arg;
            message->isReset = FALSE;
        }
        else
        {
            message->isReset = TRUE;
        }

        /* disable interrupts so nothing can change the value of the channel bulks held */
        pharosCpuDisableInterrupts(&level);

        /* if the running partition is not the future holder */
        if((future->tempPartition != NULL) && (future->tempPartition != future->owner))
        {
            /* then remove the future from the list of external futures held by the partition */
            linkedListRemoveObject(&future->tempPartition->externalFuturesHold , future);
        }

        /* make sure that every memory store is done before sending the message */
        __atomic_thread_fence(__ATOMIC_RELEASE);

        /* push the message to the MPSC queue */
        pharosIMpscQueuePush(queue , node);

        /* make sure that the message has been placed in the queue before triggering the interrupt */
        __atomic_thread_fence(__ATOMIC_RELEASE);

        /* send an IPI call to the core queue owner */
        pharosCpuMulticoreTriggerInterrupt(owner);

        /* re-enable interrupts */
        pharosCpuEnableInterrupts(&level);
    }
}
