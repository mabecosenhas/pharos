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


#include <pharos/kernel/thread/queue.h>
#include <pharos/kernel/thread/thread.h>
#include <pharos/kernel/partition/partition.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/hal/cpu/sections.h>
#include <pharos/kernel/core/core.h>
#include <pharos/kernel/assert/assert.h>






void pharosIFifoQueueInitialize(ptrFifoQueue queue)
{
    linkedListInitialize(&queue->items , OFFSETOF(ThreadInfo , blockedThreadQueueNode.node.fifo.node));
}


bool pharosIFifoQueueIsEmpty(ptrFifoQueue queue)
{
    /* check queue is valid */
    PHAROS_ASSERT(queue != NULL , "Queue is invalid");

    /* return if the linked list is empty */
    return linkedListIsEmpty(&queue->items);
}


void pharosIFifoQueueAddBlocked(ptrFifoQueue queue , ptrThreadInfo thread)
{
    /* check that the thread is valid */
    PHAROS_ASSERT(thread != NULL , "Thread is invalid");

    /* check that the queue is valid */
    PHAROS_ASSERT(queue != NULL , "Queue is invalid");

    /* append the thread to the last position */
    linkedListAppendLast(&queue->items , thread);
}

void pharosIFifoQueueRemoveThread(ptrFifoQueue queue , ptrThreadInfo thread)
{
    /* check that the thread is valid */
    PHAROS_ASSERT(thread != NULL , "Thread is invalid");

    /* check that the queue is valid */
    PHAROS_ASSERT(queue != NULL , "Queue is invalid");

    /* remove the thread from the FIFO queue */
    linkedListRemoveObject(&queue->items , thread);
}



ptrThreadInfo pharosIFifoQueueGetHighest(ptrFifoQueue queue)
{
    /* return the first element on the list */
    return linkedListGetFirst(&queue->items);
}


ptrThreadInfo pharosIThreadQueueGetHighest(ptrThreadQueue queue)
{
    /* if the queue is the FIFO type */
    if(queue->isFifo == TRUE)
    {
        /* then get the highest of the FIFO */
        return pharosIFifoQueueGetHighest(&queue->queue.fifo);
    }
        /* else, is a priority queue */
    else
    {
        /* get the highest priority thread of the priority queue */
        return pharosIPqGetHighest(&queue->queue.priority);
    }
}
