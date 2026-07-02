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


#include <pharos/kernel/core/multi.h>
#include <pharos/kernel/thread/dispatcher.h>
#include <pharos/kernel/thread/sporadicthread.h>
#include <pharos/kernel/queue/queue.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/kernel/core/mpsc.h>


PharosHwQueueSendR pharosIHwQueueSend(ptrHeavyWeightQueue queue , ptrHeavyWeightQueueMessage message , PharosPriority priority)
{
    PharosHwQueueSendR result;
    
    /* get the running core */
    CoreNumber core = pharosICoreRunning();

    /* get the core that owns the queue */
    CoreNumber queueOwner = queue->owner->core->number;

    /* node to send the message to the other core with */
    ptrPharosMpscNode node;

    /* message to send */
    ptrPharosRmpMessageHwQueueSend messageMpsc;

    /* queue on the other core to send */
    ptrPharosMpscQueue mpscQueue;


    /* set the sender of the message */
    message->rmp.sender = core;
    
    /* check the queue is valid */
    PHAROS_ASSERT(queue != NULL , "Invalid queue");

    /* check the message is valid */
    PHAROS_ASSERT(message != NULL , "Invalid message");

    /* check if we are sending the message to the same core */
    if(queueOwner == core)
    {
        /* then use the same core function to send the message */
        result = pharosIHwQueueSendOnQueueCore(queue , message , priority);
    }
    else
    {
        /* message to send to the other core */
        node = &message->rmp.mpscSendNode[queueOwner];
       
        /* get the MPSC queue to send to on the channel owner core */
        mpscQueue = &pharosICoreGet(queueOwner)->rmp.queue;

        /* get the correct node to use to send the message */
        node = pharosIMpscQueueGetMessage(mpscQueue , node);

        /* get the message */
        messageMpsc = &message->rmp.sendMessage;

        /* set the message */
        node->message.hwqueuesend = messageMpsc;

        /* create the multi-core message */
        node->type = PHAROS_RMP_MESSAGE_TYPE_HWQUEUE_SEND;

        messageMpsc->queue = queue;
        messageMpsc->message = message;
        messageMpsc->priority = priority;

        /* make sure that every memory store is done before sending the message */
        __atomic_thread_fence(__ATOMIC_RELEASE);

        /* push the message to the MPSC queue */
        pharosIMpscQueuePush(mpscQueue , node);

        /* make sure that the message has been placed in the queue before triggering the interrupt */
        __atomic_thread_fence(__ATOMIC_RELEASE);

        /* send an IPI call to the core queue owner */
        pharosCpuMulticoreTriggerInterrupt(queueOwner);
        
        /* return success */
        result = PHAROS_HEAVY_WEIGHT_SEND_SUCCESS;
    }
    
        /* return success or the partition restart */
    return result;
}


void pharosIHwQueueFreeMessage(ptrHeavyWeightQueue queue , ptrHeavyWeightQueueMessage message)
{
    /* get the running core */
    CoreNumber core = pharosICoreRunning();

    /* node to send the message to the other core with */
    ptrPharosMpscNode node;

    /* message to send */
    ptrPharosRmpMessageHwQueueFree messageMpsc;

    /* queue on the other core to send */
    ptrPharosMpscQueue mpscQueue;

    /* sender core */
    CoreNumber sender = message->rmp.sender;


    /* check if we are sending the message in the same core */
    if(sender == core)
    {
        /* then use the same core function to free the message */
        pharosIHwQueueFreeMessageLocal(queue , message);
    }
    else
    {
        /* fast forward message to send to the other core */
        node = &message->rmp.mpscFreeNode[sender];

        /* get the MPSC queue to send to on the channel owner core */
        mpscQueue = &pharosICoreGet(sender)->rmp.queue;

        /* get the correct node to use to send the message */
        node = pharosIMpscQueueGetMessage(mpscQueue , node);

        /* get the message */
        messageMpsc = &message->rmp.freeMessage;

        /* set the message */
        node->message.hwqueueFree = messageMpsc;

        /* create the multi-core message */
        node->type = PHAROS_RMP_MESSAGE_TYPE_HWQUEUE_FREE;
        
        /* fill the multi-core message */
        messageMpsc->queue = queue;
        messageMpsc->message = message;

        /* make sure that every memory store is done before sending the message */
        __atomic_thread_fence(__ATOMIC_RELEASE);

        /* push the message to the MPSC queue */
        pharosIMpscQueuePush(mpscQueue , node);

        /* make sure that the message has been placed in the queue before triggering the interrupt */
        __atomic_thread_fence(__ATOMIC_RELEASE);

        /* send an IPI call to the core queue owner */
        pharosCpuMulticoreTriggerInterrupt(sender);
    }
}
