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


#include <pharos/kernel/queue/queue.h>
#include <pharos/queue.h>
#include <pharos/kernel/thread/aperiodicthread.h>
#include <pharos/kernel/thread/dispatcher.h>
#include <pharos/kernel/thread/sporadicthread.h>
#include <pharos/kernel/partition/partition.h>
#include <pharos/hal/boardApi.h>
#include <pharos/buffer.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/kernel/scheduler/schedulerinline.h>


/**
 * Sends a bulk message to the lightweight queue 
 * 
 * @param queue queue where to send the message to
 * @param message bulk message to send
 * @param size size of the message
 * 
 * @return returns the result of the message send (invalid or sucess)
 */
static PharosLwQueueSendR queueSend(ptrLightWeightQueue queue , ptrBulk message , uint32_t size);


PharosLwQueueSendR queueSend(ptrLightWeightQueue queue , ptrBulk message , uint32_t size)
{
    /* result of the method */
    PharosLwQueueSendR result;

    /* interrupt level */
    PharosInterruptLevel interrupt;

    /* thread unblocked by this message (if any) */
    ptrThreadInfo unblocked;


    /* if queue is invalid */
    if(queue == NULL)
    {
        /* return invalid */
        result = PHAROS_LIGHTWEIGHT_QUEUE_SEND_INVALID;
    }
    else
    {
        /* set the message size */
        message->bulk.buffer.size = size;

        /* disable interrupts so nothing can change the value of the global variables */
        pharosCpuDisableInterrupts(&interrupt);

        /* remove the highest priority blocked thread on the queue */
        unblocked = pharosIThreadQueueGetHighest(&queue->blockedQueue);

        /* check if unblocked a thread */
        if(unblocked != NULL)
        {
            /* remove the thread from the map of blocked threads on this queue */
            pharosIThreadQueueRemoveBlocked(unblocked);

            /* update the activation of the sporadic thread */
            pharosIThreadSporadicActivate(unblocked);

            /* update the thread queue receive result */
            unblocked->threadFlags.unblockDueToTimeout = FALSE;

            /* remove the thread timeout time tick (if needed) */
            pharosITimeTickRemoveThread(unblocked);

            /* update the state of the thread to ready */
            pharosIThreadStateUnblock(&unblocked->state);

            /* remove the wait state (if was placed) */
            pharosIThreadStateUnwait(&unblocked->state);

            /* copy the message pointer */
            unblocked->unblockInfo.message = message;

            /* add the thread to ready thread list */
            pharosISchedAddReadyThread(unblocked);

            /* re-enable interrupts*/
            pharosCpuEnableInterrupts(&interrupt);

            /* maybe necessary to dispatch to new thread */
            pharosIDispatchThread();
        }
            /* if no thread is waiting on the queue */
        else
        {
            /* add message to queue */
            simpleListAppendLast(&queue->messagesSent , message);

            /* re-enable interrupts*/
            pharosCpuEnableInterrupts(&interrupt);
        }

        /* return success */
        result = PHAROS_LIGHTWEIGHT_QUEUE_SEND_SUCCESS;
    }


    /* return the method result */
    return result;
}


PharosLwQueueSendR pharosSLwQueueSend(LwQueueNumber number , ptrBulk message , uint32_t size)
{
    /* queue where to send the message */
    ptrLightWeightQueue queue;

    /* result of the function */
    PharosLwQueueSendR result;


    if(message == NULL)
    {
        result = PHAROS_LIGHTWEIGHT_QUEUE_SEND_INVALID_BULK;
    }
    else
    {
        /* queue to where to send the message */
        queue = pharosILwQueueGet(number);

        /* send the message */
        result = queueSend(queue , message , size);
    }

    /* return the send result */
    return result;
}


PharosLwQueueReceiveR pharosSLwQueueReceive(LwQueueNumber number , ptrBulk *message , ptrBuffer buffer)
{
    /* return value of the method */
    PharosLwQueueReceiveR result;

    /* interrupt level */
    PharosInterruptLevel interrupt;

    /* queue to receive from */
    ptrLightWeightQueue queue;


    /* get the queue*/
    queue = pharosILwQueueGet(number);

    /* if the queue id is invalid */
    if(queue == NULL)
    {
        /* return invalid id */
        result = PHAROS_LIGHTWEIGHT_QUEUE_RECEIVE_INVALID;
    }
        /* if the queue and message are correct */
    else
    {
        /* disable interrupts so nothing can change the value of the global variables */
        pharosCpuDisableInterrupts(&interrupt);

        /* check if there is any message on the list */
        if(simpleListIsEmpty(&queue->messagesSent) == TRUE)
        {
            /* re-enable interrupts*/
            pharosCpuEnableInterrupts(&interrupt);

            /* this method does not block for a message so just return unavailable */
            result = PHAROS_LIGHTWEIGHT_QUEUE_RECEIVE_NO_MESSAGE;
        }
            /* there is a message available */
        else
        {
            /* get the first available message */
            *message = (ptrBulk) simpleListRemoveFirst(&queue->messagesSent);

            /* re-enable interrupts*/
            pharosCpuEnableInterrupts(&interrupt);

            /* copy the buffer */
            *buffer = (*message)->bulk.buffer;

            /* return success */
            result = PHAROS_LIGHTWEIGHT_QUEUE_RECEIVE_SUCCESS;
        }
    }

    /* return the method result */
    return result;
}


PharosLwQueueReceiveTimeoutR pharosILwQueueReceiveTimeout(ptrThreadInfo thread , ptrLightWeightQueue queue ,
                                                          ptrBulk *message , ptrBuffer buffer , Timeout timeout)
{
    /* interrupt level */
    PharosInterruptLevel interrupt;

    /* return value of the method */
    PharosLwQueueReceiveTimeoutR result;


    /* disable interrupts so nothing can change the value of the global variables */
    pharosCpuDisableInterrupts(&interrupt);

    /* if running inside an interrupt */
    if(pharosIInterruptIsRunning() == TRUE)
    {
        /* re-enable interrupts*/
        pharosCpuEnableInterrupts(&interrupt);

        /* return invalid receive */
        result = PHAROS_LIGHTWEIGHT_QUEUE_RECEIVE_TIMEOUT_INSIDE_INTERRUPT;
    }
        /* check if there is any message on the list */
    else if(simpleListIsEmpty(&queue->messagesSent) == FALSE)
    {
        /* get the first available message */
        *message = (ptrBulk) simpleListRemoveFirst(&queue->messagesSent);

        /* copy the buffer */
        *buffer = (*message)->bulk.buffer;

        /* re-enable interrupts*/
        pharosCpuEnableInterrupts(&interrupt);

        /* update the activation of the sporadic thread */
        pharosIThreadSporadicActivate(thread);

        /* return success */
        result = PHAROS_LIGHTWEIGHT_QUEUE_RECEIVE_TIMEOUT_SUCCESS;
    }
        /* there is no message available */
    else if(timeout == PHAROS_TIMEOUT_ZERO)
    {
        /* re-enable interrupts*/
        pharosCpuEnableInterrupts(&interrupt);

        /* no timeout selected, return unavailable */
        result = PHAROS_LIGHTWEIGHT_QUEUE_RECEIVE_TIMEOUT_NO_MESSAGE;
    }
        /* if wait without timeout */
    else if(timeout == PHAROS_TIMEOUT_INFINITE)
    {
        /* update the state of the thread */
        pharosIThreadStateBlockLwQueue(&thread->state);

        /* add the calling thread to the list of blocked threads on the queue */
        pharosIThreadQueueAddBlocked(&queue->blockedQueue , thread);

        /* remove the thread from the list of ready threads */
        pharosISchedRemoveReadyThread(thread);

        /* re-enable interrupts*/
        pharosCpuEnableInterrupts(&interrupt);

        /* dispatch to highest priority ready thread */
        pharosIDispatchThread();

        /* copy the message pointer */
        *message = thread->unblockInfo.message;

        /* copy the buffer */
        *buffer = (*message)->bulk.buffer;

        /* if we got here then a message was sent and we got it */
        result = PHAROS_LIGHTWEIGHT_QUEUE_RECEIVE_TIMEOUT_SUCCESS;
    }
        /* else is wait with timeout */
    else
    {
        /* update the state of the thread to blocked */
        pharosIThreadStateBlockLwQueue(&thread->state);

        /* update state to waiting on queue */
        pharosIThreadStateWaitLwQueue(&thread->state);

        /* add the calling thread to the list of blocked threads on the queue */
        pharosIThreadQueueAddBlocked(&queue->blockedQueue , thread);

        /* remove the thread from the list of ready threads */
        pharosISchedRemoveReadyThread(thread);

        /* reset the flag */
        thread->threadFlags.unblockDueToTimeout = FALSE;

        /* add the time handler */
        pharosITimeTickAddThread(thread , timeout + pharosIClockTicksGet(thread->scheduler) ,
                                 PHAROS_TIME_TICK_AWAKEN_THREAD_LIGHTWEIGHT_QUEUE_RECEIVE_TIMEOUT ,
                                 thread->scheduler);

        /* re-enable interrupts*/
        pharosCpuEnableInterrupts(&interrupt);

        /* dispatch to highest priority ready thread */
        pharosIDispatchThread();

        /* get the result of the timeout (message was received or timeout occurred?) */
        if(thread->threadFlags.unblockDueToTimeout == TRUE)
        {
            /* there was a timeout */
            result = PHAROS_LIGHTWEIGHT_QUEUE_RECEIVE_TIMEOUT_TIMEOUT;
        }
        else
        {
            /* copy the message pointer */
            *message = thread->unblockInfo.message;

            /* copy the buffer */
            *buffer = (*message)->bulk.buffer;

            /* simple semaphore was released and this thread obtained it before the timeout expired */
            result = PHAROS_LIGHTWEIGHT_QUEUE_RECEIVE_TIMEOUT_SUCCESS;
        }
    }

    /* return success or the error */
    return result;
}


PharosLwQueueReceiveTimeoutR pharosSLwQueueReceiveTimeout(LwQueueNumber number , ptrBulk *message , ptrBuffer buffer , Timeout timeout)
{
    /* return value of the method */
    PharosLwQueueReceiveR result;

    /* queue to receive from */
    ptrLightWeightQueue queue;

    /* calling thread */
    ptrThreadInfo thread;


    /* get the queue */
    queue = pharosILwQueueGet(number);

    /* if queue is invalid */
    if(queue == NULL)
    {
        /* queue is invalid */
        result = PHAROS_LIGHTWEIGHT_QUEUE_RECEIVE_TIMEOUT_INVALID;
    }
    else
    {
        /* get the calling thread */
        thread = pharosISchedGetRunningThread();

        /* receive the message */
        result = pharosILwQueueReceiveTimeout(thread , queue , message , buffer , timeout);
    }

    /* return the method result */
    return result;
}


void pharosILwQueuesInitialize(ptrLightWeightQueueTable table)
{
    /* iterator of queues */
    LwQueueNumber i;


    /* loop through all the queues */
    for(i = 0; i < table->size; i++)
    {
        /* get the queue to initialize */
        ptrLightWeightQueue queue = &table->lwQueues[i];

        /* initialize the list of messages sent */
        simpleListInitialize(&queue->messagesSent , OFFSETOF(Bulk , bulk.usedNode));

        /* initialize the map of blocked threads on the queue */
        pharosIThreadQueueInitialize(&queue->blockedQueue , queue->flags.isFifo);
    }

    /* queues are now ready to be used */
}


ptrLightWeightQueue pharosILwQueueGet(LwQueueNumber number)
{
    /* result of the method */
    ptrLightWeightQueue result;

    /* get the table of queues in the current partition */
    ptrLightWeightQueueTable table = &pharosIPartitionRunObjectTable()->lwQueueTable;


    /* if the queue id is too high */
    if(table == NULL || number >= table->size)
    {
        /* return invalid */
        result = NULL;
    }
        /* if the id is OK */
    else
    {
        /* return the pointer to the queue */
        result = &table->lwQueues[number];
    }

    /* return the result */
    return result;
}
