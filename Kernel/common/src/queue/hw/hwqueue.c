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
#include <pharos/kernel/object/protection.h>
#include <pharos/kernel/interrupt/interrupt.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/kernel/partition/partitioninline.h>
#include <pharos/kernel/future/futureinline.h>
#include <pharos/kernel/scheduler/schedulerinline.h>


/**
 * Gets a hw queue on a specific partition
 * 
 * @param partition partition to get the queue from
 * @param number number of the queue on the partition
 * 
 * @return returns the hw queue or NULL if invalid
 */
static ptrHeavyWeightQueue queueGetGlobal(PartitionNumber partition , HwQueueNumber number);


/**
 * Get the highest priority message from the queue
 * 
 * @contraint assumes there is at least one message on the queue
 * 
 * @param queue hw queue to get the message from
 * 
 * @return returns the highest priority message 
 */
static ptrHeavyWeightQueueMessage getHighestPriorityMessage(ptrHeavyWeightQueue queue);


/**
 * Place the message in the queue
 * 
 * @param queue queue to place the message
 * @param message message to send
 */
static void placeMessageQueue(ptrHeavyWeightQueue queue , ptrHeavyWeightQueueMessage message);


/**
 * Copy the message contents to the buffer, size and future
 * 
 * @param queue queue to which the message was sent
 * @param message message to copy
 * @param buffer buffer to copy to 
 * @param size address where to place the size
 * @param future address where to place the future (if not NULL)
 * @param priority address where to place the priority (or NULL if not wanted)
 */
static void copyMessageContent(ptrHeavyWeightQueue queue , ptrHeavyWeightQueueMessage message , uint8_t *buffer , uint32_t *size , ptrFutureId future , ptrPharosPriority priority);


void pharosICopyFuture(ptrFutureId future , ptrHeavyWeightQueueMessage message)
{
    /* if there is a future number */
    if(future != NULL)
    {
        /* if there is no future */
        if(message->future == NULL)
        {
            /* then place invalid number */
            future->number = PHAROS_FUTURE_NUMBER_INVALID;

            /* and invalid partition */
            future->partition = PHAROS_PARTITION_INVALID;
        }
            /* if there is a future to copy */
        else
        {
            /* then copy it */
            *future = message->future->index;
        }
    }
}


PharosHwQueueSendR getFreeMessage(ptrHeavyWeightQueue queue , ptrHeavyWeightQueueMessage *message , Timeout timeout)
{
    /* result of this function */
    PharosHwQueueSendR result;

    /* interrupt level */
    PharosInterruptLevel interrupt;

    /* calling thread */
    ptrThreadInfo runningThread;

    /* running core */
    CoreNumber core = pharosICoreRunning();


    /* get the calling thread */
    runningThread = pharosISchedGetRunningThread();

    /* disable interrupts so nothing can change the value of the global variables */
    pharosCpuDisableInterrupts(&interrupt);

    /* get a free message from the queue if there are threads blocked on the queue that means there is at least one message free on the queue and hence this will never return NULL here */
    *message = simpleListRemoveFirst(&queue->freeMessages[core]);

    /* check if there are free messages */
    if(*message != NULL)
    {
        /* re-enable interrupts */
        pharosCpuEnableInterrupts(&interrupt);

        /* return success, got a message to send */
        result = PHAROS_HEAVY_WEIGHT_SEND_SUCCESS;
    }
        /* no free messages, check if we should not wait */
    else if(timeout == PHAROS_TIMEOUT_ZERO)
    {
        /* re-enable interrupts */
        pharosCpuEnableInterrupts(&interrupt);

        /* return error (the queue is full) */
        result = PHAROS_HEAVY_WEIGHT_SEND_FULL;
    }
        /* no free messages, check if we must wait until a message is ready */
    else if(timeout == PHAROS_TIMEOUT_INFINITE)
    {
        /* update the state of the thread */
        pharosIThreadStateBlockHwQueueSend(&runningThread->state);

        /* add the calling thread to the list of blocked threads on the queue */
        pharosIThreadQueueAddBlocked(&queue->sendBlockedQueue[core] , runningThread);

        /* remove the thread from the list of ready threads */
        pharosISchedRemoveReadyThread(runningThread);

        /* re-enable interrupts*/
        pharosCpuEnableInterrupts(&interrupt);

        /* dispatch to highest priority ready thread */
        pharosIDispatchThread();

        /* if we reached here it means that a message was got */
        *message = runningThread->unblockInfo.hwq.hwMessage;

        /* check the message is valid */
        PHAROS_ASSERT(*message != NULL , "invalid message");

        /* return success, got a message to send */
        result = PHAROS_HEAVY_WEIGHT_SEND_SUCCESS;
    }
        /* no free messages, check if we must wait for a specified timeout */
    else
    {
        /* update the state of the thread to blocked */
        pharosIThreadStateBlockHwQueueSend(&runningThread->state);

        /* update state to waiting on queue */
        pharosIThreadStateWaitHwQueue(&runningThread->state);

        /* add the calling thread to the list of blocked threads on the queue */
        pharosIThreadQueueAddBlocked(&queue->sendBlockedQueue[core] , runningThread);

        /* remove the thread from the list of ready threads */
        pharosISchedRemoveReadyThread(runningThread);

        /* reset the flag before blocking the current thread */
        runningThread->threadFlags.unblockDueToTimeout = FALSE;

        /* add the time handler */
        pharosITimeTickAddThread(runningThread , timeout + pharosIClockTicksGet(runningThread->scheduler) ,
                                 PHAROS_TIME_TICK_AWAKEN_THREAD_HEAVYWEIGHT_QUEUE_SEND_TIMEOUT ,
                                 runningThread->scheduler);

        /* re-enable interrupts*/
        pharosCpuEnableInterrupts(&interrupt);

        /* dispatch to highest priority ready thread */
        pharosIDispatchThread();

        /* get the result of the timeout (message was received or timeout occurred?) */
        if(runningThread->threadFlags.unblockDueToTimeout == TRUE)
        {
            /* there was a timeout */
            result = PHAROS_HEAVY_WEIGHT_SEND_TIMEOUT;
        }
        else
        {
            /* if we reached here it means that a message was got */
            *message = runningThread->unblockInfo.hwq.hwMessage;

            /* check the message is valid */
            PHAROS_ASSERT(*message != NULL , "invalid message");

            /* return success, got a message to send */
            result = PHAROS_HEAVY_WEIGHT_SEND_SUCCESS;
        }
    }

    /* return the send result */
    return result;
}


PharosHwQueueSendR pharosSHwQueueSend(PartitionNumber partition , HwQueueNumber number , const uint8_t *buffer , uint32_t size , PharosPriority priority , ptrFutureId future , Timeout timeout)
{
    /* message where the buffer content will be copied into */
    ptrHeavyWeightQueueMessage message;

    /* queue where to send the message */
    ptrHeavyWeightQueue queue;

    /* result of the function */
    PharosHwQueueSendR result;

    /* future */
    ptrFuture f = NULL;

    /* copy of the future number (so another thread cannot change it) */
    FutureId myId;

    /* get the partition to send to */
    ptrPartition p = pharosIPartitionGet(partition);

    /* memory protection of the running partition */
    ptrEnvMemoryProtection env = pharosIPartitionRunningMemProt();


    /* check if timeout is appropriate inside an interrupt */
    if((timeout != PHAROS_TIMEOUT_ZERO) && (pharosIInterruptIsRunning() == TRUE))
    {
        /* cannot send the message with a timeout inside an interrupt */
        result = PHAROS_HEAVY_WEIGHT_SEND_TIMEOUT_INSIDE_INTERRUPT;
    }
        /* check if the future is outside the protection */
    else if((future != NULL) && (pharosCpuBufferIsInsideProtection((uint8_t *) future , sizeof (FutureId) , env) == FALSE))
    {
        /* return invalid future */
        result = PHAROS_HEAVY_WEIGHT_SEND_INVALID_FUTURE;
    }
    else
    {
        /* if there is a future to get */
        if(future != NULL)
        {
            /* create a copy of the future number */
            myId = *future;

            /* get the future (now we know that "number" is a valid address) */
            f = pharosIFutureGetLocal(&myId);
        }

        /* get the queue */
        queue = queueGetGlobal(partition , number);

        /* if could not find the queue */
        if(queue == NULL)
        {
            /* return invalid queue */
            result = PHAROS_HEAVY_WEIGHT_SEND_INVALID;
        }
            /* if the future is not valid */
        else if((future != NULL) && (f == NULL))
        {
            /* return invalid future */
            result = PHAROS_HEAVY_WEIGHT_SEND_INVALID_FUTURE;
        }
            /* check if the future cannot be used by the running partition */
        else if((f != NULL) && (pharosIFutureIsHolder(f , pharosIPartitionGetRunning()) == FALSE))
        {
            /* return future is in use */
            result = PHAROS_HEAVY_WEIGHT_SEND_FUTURE_IN_USE;
        }
            /* if the future is ready or in the progress of making it ready */
        else if((f != NULL) && (f->flags.isReady == TRUE || f->flags.makingReady == TRUE))
        {
            /* cannot send this future since it is already ready */
            result = PHAROS_HEAVY_WEIGHT_SEND_FUTURE_IS_READY;
        }
            /* check if the partition that owns the channel has started */
        else if(pharosIPartitionIsStarted(p) == FALSE)
        {
            /* then return uninitialized */
            result = PHAROS_HEAVY_WEIGHT_SEND_UNINITIALIZED;
        }
            /* if the message to be sent is too big */
        else if(queue->messageSize < size)
        {
            /* return invalid size */
            result = PHAROS_HEAVY_WEIGHT_SEND_INVALID_SIZE;
        }
            /* if buffer is invalid */
        else if(pharosCpuBufferIsInsideProtection(buffer , size , env) == FALSE)
        {
            /* return invalid buffer */
            result = PHAROS_HEAVY_WEIGHT_SEND_INVALID_BUFFER;
        }
            /* all preliminary checks went OK */
        else
        {
            /* execute the filter */
            result = pharosIHwQueueFilter(queue , size , priority);

            /* check again if the partition has restarted (check after the filter function) */
            if(pharosIPartitionIsStarted(p) == FALSE)
            {
                /* then return uninitialized */
                result = PHAROS_HEAVY_WEIGHT_SEND_UNINITIALIZED;
            }
                /* if filter gave success */
            else if(result == (PharosHwQueueSendR) PHAROS_HEAVY_WEIGHT_FILTER_SUCCESS)
            {
                /* get a free message */
                result = getFreeMessage(queue , &message , timeout);

                /* check the message is valid */
                PHAROS_ASSERT((result != PHAROS_HEAVY_WEIGHT_SEND_SUCCESS) || (message != NULL) , "Invalid free message");

                /* if there is no free message */
                if(result == PHAROS_HEAVY_WEIGHT_SEND_SUCCESS)
                {
                    /* copy the message */
                    /* MISRA2004 20.3: buffer was confirmed previously to be a valid address */
                    memcpy(message->buffer.buffer , buffer , size);

                    /* copy the buffer size */
                    message->buffer.size = size;

                    /* copy the future */
                    message->future = f;

                    /* set the priority */
                    message->priority = priority;

                    /* then send the message to the queue */
                    pharosIHwQueueSend(queue , message , priority);

                    /* result is success (redundant since PHAROS_HEAVY_WEIGHT_SEND_SUCCESS is the same as PHAROS_HEAVY_WEIGHT_FILTER_SUCCESS) */
                    result = PHAROS_HEAVY_WEIGHT_SEND_SUCCESS;
                }
                /* else, just return the result of the get free message */
            }

        }
    }

    /* return the send result */
    return result;
}


PharosHwQueueReceiveR pharosSHwQueueReceive(HwQueueNumber number , uint8_t *buffer , uint32_t *size , ptrPharosPriority priority , ptrFutureId future , Timeout timeout)
{
    /* return value of the method */
    PharosHwQueueReceiveR result;

    /* queue to receive from */
    ptrHeavyWeightQueue queue;

    /* auxiliary variable to store the size correctly inside the kernel memory area */
    uint32_t mySize;

    /* auxiliary variable to store the priority correctly inside the kernel memory area */
    PharosPriority myPriority;


    /* get the queue */
    queue = pharosIHwQueueGet(number);

    /* check if we are inside an interrupt */
    if(pharosIInterruptIsRunning() == TRUE)
    {
        /* called blocking call inside interrupt */
        result = PHAROS_HEAVY_WEIGHT_RECEIVE_INSIDE_INTERRUPT;
    }
        /* check if the priority address is valid */
    else if(priority != NULL && pharosCpuBufferIsInsideProtection((uint8_t *) priority , sizeof (PharosPriority) , pharosIPartitionRunningMemProt()) == FALSE)
    {
        /* return invalid priority address */
        result = PHAROS_HEAVY_WEIGHT_RECEIVE_INVALID_PRIORITY;
    }
        /* check if size address is valid */
    else if(size != NULL && pharosCpuBufferIsInsideProtection((uint8_t *) size , sizeof (uint32_t) , pharosIPartitionRunningMemProt()) == FALSE)
    {
        /* return invalid size address */
        result = PHAROS_HEAVY_WEIGHT_RECEIVE_INVALID_SIZE;
    }
        /* if queue is invalid */
    else if(queue == NULL)
    {
        /* queue is invalid */
        result = PHAROS_HEAVY_WEIGHT_RECEIVE_INVALID;
    }
        /* check the if the buffer was enough size for any message to be received */
    else if(pharosCpuBufferIsInsideProtection(buffer , queue->messageSize , pharosIPartitionRunningMemProt()) == FALSE)
    {
        /* return invalid buffer */
        result = PHAROS_HEAVY_WEIGHT_RECEIVE_INVALID_BUFFER;
    }
        /* check if the future is outside the protection */
    else if((future != NULL) && (pharosCpuBufferIsInsideProtection((uint8_t *) future , sizeof (FutureId) , pharosIPartitionRunningMemProt()) == FALSE))
    {
        /* return invalid future */
        result = PHAROS_HEAVY_WEIGHT_RECEIVE_INVALID_FUTURE;
    }
    else
    {
        /* receive the message */
        result = pharosIHwQueueReceive(queue , buffer , &mySize , &myPriority , timeout , future);

        /* if to copy the size */
        if(size != NULL)
        {
            /* copy the size */
            *size = mySize;
        }

        /* if to copy the priority */
        if(priority != NULL)
        {
            /* copy the priority */
            *priority = myPriority;
        }
    }

    /* return the method result */
    return result;
}


void pharosIHwQueueFreeMessageLocal(ptrHeavyWeightQueue queue , ptrHeavyWeightQueueMessage message)
{
    /* interrupt level */
    PharosInterruptLevel interrupt;

    /* thread unblocked by this message (if any) */
    ptrThreadInfo unblocked;

    /* get the running core */
    CoreNumber core = pharosICoreRunning();


    /* disable interrupts so we can safely add to the free messages list */
    pharosCpuDisableInterrupts(&interrupt);

    /* remove the highest priority blocked thread on the queue */
    unblocked = pharosIThreadQueueGetHighest(&queue->sendBlockedQueue[core]);

    /* if there is a thread waiting for the message */
    if(unblocked != NULL)
    {
        /* remove the thread from the map of blocked threads on this queue */
        pharosIThreadQueueRemoveBlocked(unblocked);

        /* update the thread queue receive result */
        unblocked->threadFlags.unblockDueToTimeout = FALSE;

        /* remove the thread timeout time tick (if needed). Sporadic threads waiting for activation don't have time tick handler */
        pharosITimeTickRemoveThread(unblocked);

        /* update the state of the thread to ready */
        pharosIThreadStateUnblock(&unblocked->state);

        /* remove the wait state (if was placed) */
        pharosIThreadStateUnwait(&unblocked->state);

        /* copy the message pointer */
        unblocked->unblockInfo.hwq.hwMessage = message;

        /* add the thread to ready thread list */
        pharosISchedAddReadyThread(unblocked);

        /* re-enable interrupts (we can do this since the unblocked thread as at this point already removed the time tick handler and hence cannot be awaken by a clock tick */
        pharosCpuEnableInterrupts(&interrupt);

        /* maybe necessary to dispatch to new thread */
        pharosIDispatchThread();
    }
    else
    {
        /* append the message to the free message list so it can be used again */
        simpleListAppendFirst(&queue->freeMessages[core] , message);

        /* re-enable interrupts*/
        pharosCpuEnableInterrupts(&interrupt);
    }
}


PharosHwQueueReceiveR pharosIHwQueueReceive(ptrHeavyWeightQueue queue , uint8_t *buffer , uint32_t *size ,
                                            ptrPharosPriority priority , Timeout timeout , ptrFutureId future)
{
    /* interrupt level */
    PharosInterruptLevel interrupt;

    /* return value of the method */
    PharosHwQueueReceiveR result;

    /* calling thread */
    ptrThreadInfo thread;

    /* message to receive */
    ptrHeavyWeightQueueMessage message;


    /* get the calling thread */
    thread = pharosISchedGetRunningThread();

    /* disable interrupts so nothing can change the value of the global variables */
    pharosCpuDisableInterrupts(&interrupt);

    /* check if there is any message on any list through the priority bit map */
    if(treeMapIsEmpty(&queue->mapMessagesSent) == FALSE)
    {
        /* get the highest priority message on the queue (and update the priority map) */
        message = getHighestPriorityMessage(queue);

        /* check the message is valid */
        PHAROS_ASSERT(message != NULL , "Invalid message");

        /* re-enable interrupts*/
        pharosCpuEnableInterrupts(&interrupt);

        /* copy the message */
        copyMessageContent(queue , message , buffer , size , future , priority);

        /* update the activation of the sporadic thread */
        pharosIThreadSporadicActivate(thread);

        /* return success */
        result = PHAROS_HEAVY_WEIGHT_RECEIVE_SUCCESS;
    }
        /* there is no message available */
    else if(timeout == PHAROS_TIMEOUT_ZERO)
    {
        /* re-enable interrupts*/
        pharosCpuEnableInterrupts(&interrupt);

        /* no timeout selected, return empty */
        result = PHAROS_HEAVY_WEIGHT_RECEIVE_EMPTY;
    }
        /* if wait without timeout */
    else if(timeout == PHAROS_TIMEOUT_INFINITE)
    {
        /* update the state of the thread */
        pharosIThreadStateBlockHwQueueReceive(&thread->state);

        /* add the calling thread to the list of blocked threads on the queue */
        pharosIThreadQueueAddBlocked(&queue->receiveBlockedQueue , thread);

        /* remove the thread from the list of ready threads */
        pharosISchedRemoveReadyThread(thread);

        /* re-enable interrupts*/
        pharosCpuEnableInterrupts(&interrupt);

        /* dispatch to highest priority ready thread */
        pharosIDispatchThread();

        /* get the message received */
        message = thread->unblockInfo.hwq.hwMessage;

        /* copy the message */
        copyMessageContent(queue , message , buffer , size , future , priority);

        /* if we got here then a message was sent and we got it */
        result = PHAROS_HEAVY_WEIGHT_RECEIVE_SUCCESS;
    }
        /* else is wait with timeout */
    else
    {
        /* update the state of the thread to blocked */
        pharosIThreadStateBlockHwQueueReceive(&thread->state);

        /* update state to waiting on queue */
        pharosIThreadStateWaitHwQueue(&thread->state);

        /* add the calling thread to the list of blocked threads on the queue */
        pharosIThreadQueueAddBlocked(&queue->receiveBlockedQueue , thread);

        /* remove the thread from the list of ready threads */
        pharosISchedRemoveReadyThread(thread);

        /* reset the flag */
        thread->threadFlags.unblockDueToTimeout = FALSE;

        /* add the time handler */
        pharosITimeTickAddThread(thread , timeout + pharosIClockTicksGet(thread->scheduler) ,
                                 PHAROS_TIME_TICK_AWAKEN_THREAD_HEAVYWEIGHT_QUEUE_RECEIVE_TIMEOUT ,
                                 thread->scheduler);

        /* re-enable interrupts*/
        pharosCpuEnableInterrupts(&interrupt);

        /* dispatch to highest priority ready thread */
        pharosIDispatchThread();

        /* disable interrupts so nothing can change the value of the global variables */
        pharosCpuDisableInterrupts(&interrupt);

        /* remove the thread from the list of blocked threads whether a timeout occurred or not */
        pharosIThreadQueueRemoveBlocked(thread);

        /* get the result of the timeout (message was received or timeout occurred?) */
        if(thread->threadFlags.unblockDueToTimeout == TRUE)
        {
            /* re-enable interrupts*/
            pharosCpuEnableInterrupts(&interrupt);

            /* there was a timeout */
            result = PHAROS_HEAVY_WEIGHT_RECEIVE_TIMEOUT;
        }
        else
        {
            /* re-enable interrupts*/
            pharosCpuEnableInterrupts(&interrupt);

            /* get the message received */
            message = thread->unblockInfo.hwq.hwMessage;

            /* copy the message */
            copyMessageContent(queue , message , buffer , size , future , priority);

            /* simple semaphore was released and this thread obtained it before the timeout expired */
            result = PHAROS_HEAVY_WEIGHT_RECEIVE_SUCCESS;
        }
    }

    /* return success or the error */
    return result;
}


ptrHeavyWeightQueue pharosIHwQueueGet(HwQueueNumber number)
{
    /* result of the method */
    ptrHeavyWeightQueue result;

    /* get the table of queues in the current environment */
    ptrHeavyWeightQueueTable table = &pharosIPartitionRunObjectTable()->hwQueueTable;

    /* if the queue number is too high */
    if(number >= table->size)
    {
        /* return invalid */
        result = NULL;
    }
        /* if the number is OK */
    else
    {
        /* return the pointer to the queue */
        result = &table->hwQueues[number];
    }

    /* return the result */
    return result;
}


PharosHwQueueSendR pharosIHwQueueSendOnQueueCore(ptrHeavyWeightQueue queue , ptrHeavyWeightQueueMessage message ,
                                                 PharosPriority priority)
{
    /* return value of this method */
    PharosHwQueueSendR result;

    /* interrupt level */
    PharosInterruptLevel interrupt;

    /* thread unblocked by this message (if any) */
    ptrThreadInfo unblocked;

    /* get the future */
    ptrFuture f = message->future;


    /* check for invalid queue */
    PHAROS_ASSERT(queue != NULL , "Invalid queue");

    /* check for invalid message */
    PHAROS_ASSERT(message != NULL , "Invalid message");

    /* if there is a future */
    if(f != NULL)
    {
        /* the queue owner now has access to the future */
        pharosIFutureUpdateHolder(f , queue->owner);
    }

    /* disable interrupts so we can add the thread to the ready list */
    pharosCpuDisableInterrupts(&interrupt);

    /* check if the queue partition is restarting (if it is, then we can't send the message (the queue 
     * receiveBlockedQueue could be incoherent).
     * The sender can only make sure that the message was processed by waiting on the future and checking that it 
     * was successful */
    if(queue->owner->state == PHAROS_PARTITION_STATE_RESTARTING)
    {
        /* if there is a future */
        if(f != NULL)
        {
            /* reset the future so it will return "partition restarting" when the sender tries to wait for it */
            pharosIFutureReset(f);
        }

        /* re-enable interrupts */
        pharosCpuEnableInterrupts(&interrupt);

        /* return partition restart */
        result = PHAROS_HEAVY_WEIGHT_SEND_PARTITION_RESTART;
    }
        /* if the queue owner is not restarting */
    else
    {
        /* if there is a future */
        if(f != NULL)
        {
            /* append to the list of external futures being held by the partition */
            linkedListAppendFirst(&queue->owner->externalFuturesHold , f);
        }

        /* remove the highest priority blocked thread on the queue */
        unblocked = pharosIThreadQueueGetHighest(&queue->receiveBlockedQueue);

        /* if there was a thread blocked on the queue that thread will receive the message */
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
            unblocked->unblockInfo.hwq.hwMessage = message;

            /* re-enable interrupts (we can do this since the unblocked thread as at this point already removed the time 
             * tick handler and hence cannot be awaken by a clock tick */
            pharosCpuEnableInterrupts(&interrupt);

            /* add the thread to ready thread list */
            pharosISchedAddReadyThread(unblocked);

            /* maybe necessary to dispatch to new thread */
            pharosIDispatchThread();
        }
            /* there is no thread being blocked */
        else
        {

            /* place the message in the queue */
            placeMessageQueue(queue , message);

            /* re-enable interrupts*/
            pharosCpuEnableInterrupts(&interrupt);
        }

        /* return success */
        result = PHAROS_HEAVY_WEIGHT_SEND_SUCCESS;
    }

    /* return success or partition restart */
    return result;
}


void pharosIHwQueuesInitializeMinimal(ptrPartition p)
{
    /* iterator of queues */
    HwQueueNumber i;

    /* queue iterator */
    ptrHeavyWeightQueue queue;

    /* hw queue table */
    ptrHeavyWeightQueueTable table = &p->objects.hwQueueTable;


    /* loop through all the queues */
    for(i = 0U; i < table->size; i++)
    {
        /* get the queue to initialize */
        queue = &table->hwQueues[i];

        /* initialize the owner of the queue */
        queue->owner = p;

        /* mark the queue has not been initialized */
        queue->flags.isInitialized = FALSE;
    }

    /* queues are now ready to be used */
}


void pharosIHwQueuesInitialize(ptrPartition partition , const ptrHeavyWeightQueueTable table)
{
    /* iterator of queues */
    HwQueueNumber i;

    /* iterator for priorities and messages */
    uint32_t k;

    /* core iterator */
    CoreNumber core;

    /* iterator on the full buffer for each message */
    uint32_t index;

    /* message iterator */
    ptrHeavyWeightQueueMessage message;

    /* queue iterator */
    ptrHeavyWeightQueue queue;


    /* loop through all the queues */
    for(i = 0U; i < table->size; i++)
    {
        /* get the queue to initialize */
        queue = &table->hwQueues[i];

        /* initialize the map of messages sent */
        treeMapInitialize(&queue->mapMessagesSent , (ComparatorMethod) pharosPriorityComparator , OFFSETOF(HeavyWeightQueueMessage , node));

        /* for each core */
        for(core = 0U; core < PHAROS_NUMBER_CORES; core++)
        {
            /* get the message array to initialize for the core */
            ptrHeavyWeightQueueMessage messages = queue->messages[core];

            /* initialize the list of free messages */
            simpleListInitialize(&queue->freeMessages[core] , OFFSETOF(HeavyWeightQueueMessage , next));

            /* reset the index on the fullBuffer */
            index = 0U;

            /* for every message */
            for(k = 0U; k < queue->numberMessages[core]; k++)
            {
                /* get the message to initialize */
                message = &messages[k];

                /* initialize the message buffer pointer to index iterator */
                message->buffer.buffer = &queue->fullBuffer[core].buffer[index];

                /* initialize the message size to the maximum size of each message */
                message->buffer.size = queue->messageSize;

                /* increment the index to the next message */
                index += queue->messageSize;

                /* add a free message */
                simpleListAppendFirst(&queue->freeMessages[core] , message);
            }

            /* initialize the queue id */
            queue->id = i;

            /* initialize the map of blocked threads to send on the queue */
            pharosIThreadQueueInitialize(&queue->sendBlockedQueue[core] , queue->flags.isSendingFifo);
        }

        /* initialize the map of blocked threads to receive on the queue */
        pharosIThreadQueueInitialize(&queue->receiveBlockedQueue , queue->flags.isReceivingFifo);

        /* mark the queue has been initialized */
        queue->flags.isInitialized = TRUE;
    }

    /* queues are now ready to be used */
}


ptrHeavyWeightQueueMessage getHighestPriorityMessage(ptrHeavyWeightQueue queue)
{
    /* return value */
    ptrHeavyWeightQueueMessage result;

    ptrHeavyWeightQueueMessage aux;


    /* get the highest message from the map */
    result = (ptrHeavyWeightQueueMessage) treeMapRemoveFirst(&queue->mapMessagesSent);

    /* check the message is correct */
    PHAROS_ASSERT(result != NULL , "invalid message");

    /* if there is another bulk with the same priority */
    if(result->nextPriority != NULL)
    {
        /* get the next element */
        aux = result->nextPriority;

        /* copy the last pointer */
        aux->lastPriority = result->lastPriority;

        /* place the next one again in the map */
        treeMapPut(&queue->mapMessagesSent , &aux->priority , aux);
    }

    /* return the highest priority message (or null if there is none) */
    return result;
}


void copyMessageContent(ptrHeavyWeightQueue queue , ptrHeavyWeightQueueMessage message , uint8_t *buffer , uint32_t *size , ptrFutureId future , ptrPharosPriority priority)
{
    /* copy the message */
    /* MISRA2004 20.3: buffer was checked in function pharosSHwQueueReceive */
    memcpy(buffer , message->buffer.buffer , message->buffer.size);

    /* set the message size */
    *size = message->buffer.size;

    /* if there is a priority to copy to */
    if(priority != NULL)
    {
        /* then copy the priority */
        *priority = message->priority;
    }

    /* copy the future */
    pharosICopyFuture(future , message);

    /* free the message so that it can be re-used again */
    pharosIHwQueueFreeMessage(queue , message);
}


ptrHeavyWeightQueue queueGetGlobal(PartitionNumber partition , HwQueueNumber number)
{
    /* result of the method */
    ptrHeavyWeightQueue result;

    /* get the desired partition (on any core) */
    ptrPartition p = pharosIPartitionGet(partition);


    /* if there is such a partition */
    if(p != NULL)
    {
        ptrHeavyWeightQueueTable table = &p->objects.hwQueueTable;

        /* if the queue number is too high */
        if((table == NULL) || (number >= table->size))
        {
            /* return invalid */
            result = NULL;
        }
            /* if the number is OK */
        else
        {
            /* return the pointer to the queue */
            result = &table->hwQueues[number];
        }
    }
        /* if there is no such partition */
    else
    {
        /* return invalid */
        result = NULL;
    }


    /* return the result */
    return result;
}


void placeMessageQueue(ptrHeavyWeightQueue queue , ptrHeavyWeightQueueMessage message)
{
    /* aux message */
    ptrHeavyWeightQueueMessage aux;


    /* search a bulk with the same priority */
    aux = treeMapGet(&queue->mapMessagesSent , &message->priority);

    /* the bulk will be added to the end of the list */
    message->nextPriority = NULL;

    /* if there is another message with the same priority */
    if(aux != NULL)
    {
        /* if there are already two (or more) elements on the list */
        if(aux->lastPriority != NULL)
        {
            /* update the last one to point to the new bulk */
            aux->lastPriority->nextPriority = message;
        }
            /* list has only one element */
        else
        {
            /* update the next */
            aux->nextPriority = message;
        }

        /* update the last pointer */
        aux->lastPriority = message;
    }
    else
    {
        /* place the bulk on the list */
        treeMapPut(&queue->mapMessagesSent , &message->priority , message);

        /* there are no other bulks with the same priority */
        message->lastPriority = NULL;
    }
}
