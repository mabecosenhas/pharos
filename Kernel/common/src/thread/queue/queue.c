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


/**
 * List of free ThreadQueueNode for threads
 */
static KERNEL_SECTION_USS SimpleList freeNodes[PHAROS_NUMBER_CORES];



void pharosIThreadQueueInitialize(ptrThreadQueue queue , bool isFifo)
{
    queue->isFifo = isFifo;

    if(isFifo == TRUE)
    {
        pharosIFifoQueueInitialize(&queue->queue.fifo);
    }
    else
    {
        pharosIPqInitialize(&queue->queue.priority);
    }
}


void pharosIThreadQueuesInitialize(void)
{
    /* initialize the list that contains the free priority queue nodes */
    simpleListInitialize(&freeNodes[pharosICoreRunning()] , OFFSETOF(ThreadQueueNode , freeNode));
}


void pharosIThreadQueueInitThreadBlocked(ptrThreadQueueNode node)
{
    /* add the node to the list of free nodes */
    pharosIThreadQueueReturnFree(node);
}


void pharosIThreadQueueInitSemCeil(ptrThreadQueueNode node)
{
    /* add the node to the list of free nodes */
    pharosIThreadQueueReturnFree(node);
}


void pharosIThreadQueueAddBlocked(ptrThreadQueue queue , ptrThreadInfo thread)
{
    /* check that the thread is valid */
    PHAROS_ASSERT(thread != NULL , "Thread is invalid");

    /* update the queue where the thread is blocked in */
    thread->blockedQueue = queue;

    /* save the priority with which the thread was blocked */
    thread->blockedPriority = thread->currentPriority;

    /* if the queue is FIFO */
    if(queue->isFifo == TRUE)
    {
        /* then add to the FIFO queue */
        pharosIFifoQueueAddBlocked(&queue->queue.fifo , thread);
    }
        /* else it is a priority queue */
    else
    {
        /* then add to the priority queue */
        pharosIPqAddBlocked(&queue->queue.priority , thread);
    }
}


ptrThreadQueue pharosIThreadQueueRemoveBlocked(ptrThreadInfo thread)
{
    /* thread blocked queue */
    ptrThreadQueue queue;


    /* check that the thread is valid */
    PHAROS_ASSERT(thread != NULL , "Thread is invalid");

    /* get the blocked queue */
    queue = thread->blockedQueue;

    /* if there is a queue pointer */
    if(queue != NULL)
    {
        /* if the queue is FIFO */
        if(queue->isFifo == TRUE)
        {
            /* then remove from the FIFO queue */
            pharosIFifoQueueRemoveThread(&queue->queue.fifo , thread);
        }
            /* else it is a priority queue */
        else
        {
            /* then remove from the priority queue */
            pharosIPqRemoveThread(&queue->queue.priority , thread);
        }

        /* thread no longer blocked on any queue */
        thread->blockedQueue = NULL;
    }
    /* else, thread was not blocked so do nothing */

    /* return the queue where the thread was blocked (or NULL if none) */
    return queue;
}


bool pharosIThreadQueueIsEmpty(ptrThreadQueue queue)
{
    /* return of this method*/
    bool result;


    /* check queue is valid */
    PHAROS_ASSERT(queue != NULL , "Queue is invalid");

    /* if the queue is priority */
    if(queue->isFifo == FALSE)
    {
        /* then determine if the priority queue is empty */
        result = pharosIPqIsEmpty(&queue->queue.priority);
    }
        /* if it is FIFO queue */
    else
    {
        /* then determine if the FIFO queue is empty */
        result = pharosIFifoQueueIsEmpty(&queue->queue.fifo);
    }

    /* return the result found */
    return result;
}


void pharosIThreadQueueUpdateThreadPrioOnThreadCore(ptrThreadInfo thread , ThreadPriority newPriority)
{
    /* interrupt level */
    PharosInterruptLevel interrupt;


    PHAROS_ASSERT(pharosICoreRunning() == thread->partition->core->number , "Invalid core");

    /* disable interrupts so we can update the thread priority */
    pharosCpuDisableInterrupts(&interrupt);

    /* update the current priority of the thread on the scheduler */
    pharosISchedUpdateThreadPrio(thread , newPriority);

    /* re-enable interrupts */
    pharosCpuEnableInterrupts(&interrupt);
}


void pharosIThreadQueueUpdateThreadPrioOnSchedulerCore(ptrThreadInfo thread , ThreadPriority newPriority)
{
    /* thread current blocked queue */
    ptrThreadQueue blockedQueue;

    /* thread current wait queue */
    ptrPriorityQueue waitQueue;

    /* interrupt level */
    PharosInterruptLevel interrupt;


    /* disable interrupts so we can update the thread priority */
    pharosCpuDisableInterrupts(&interrupt);

    /* check that we are really changing the priority */
    if(thread->blockedPriority != newPriority)
    {
        /* thread current blocked queue */
        blockedQueue = thread->blockedQueue;

        /* thread current wait queue */
        waitQueue = thread->waitQueue;

        /* if the thread is blocked on a priority queue */
        if((blockedQueue != NULL) && (blockedQueue->isFifo == FALSE))
        {
            /* remove the thread from the current priority queue */
            pharosIPqRemoveThread(&blockedQueue->queue.priority , thread);
        }

        /* if the thread is on a wait queue */
        if(waitQueue != NULL)
        {
            /* remove the thread from the current wait queue (no need to remove the tick handler just to add it again) */
            pharosIPqRemoveWait(thread);
        }

        /* set the new priority (after having removed from the queue) */
        thread->blockedPriority = newPriority;

        /* if the thread is blocked on a priority queue */
        if((blockedQueue != NULL) && (blockedQueue->isFifo == FALSE))
        {
            /* restore the blocked queue */
            thread->blockedQueue = blockedQueue;

            /* add the thread with the new priority */
            pharosIPqAddBlocked(&blockedQueue->queue.priority , thread);
        }

        /* if the thread is on a wait queue */
        if(waitQueue != NULL)
        {
            /* restore the wait queue */
            thread->waitQueue = waitQueue;

            /* add the thread again with the new priority */
            pharosIPqAddWait(thread->waitQueue , thread);
        }
    }
    /* else, no need to do anything since it already has the correct priority */

    /* re-enable interrupts */
    pharosCpuEnableInterrupts(&interrupt);
}


void pharosIThreadQueueRemoveAllThreads(ptrThreadQueue queue)
{
    /* interrupt level */
    PharosInterruptLevel level;

    /* thread iterator */
    ptrThreadInfo thread;


    /* have to disable interrupts because even though the partition is currently uninitialized,
     * sporadic threads from other partitions could be added to the blocked threads map */
    pharosCpuDisableInterrupts(&level);

    /* start by getting the highest priority thread waiting on the channel */
    thread = pharosIThreadQueueGetHighest(queue);

    /* while there are still threads to be taken out of the blocked threads */
    while(thread != NULL)
    {
        /* remove the thread from the queue */
        pharosIThreadQueueRemoveBlocked(thread);

        /* re-enable interrupts */
        pharosCpuEnableInterrupts(&level);

        /* have to disable interrupts again to access the thread queue */
        pharosCpuDisableInterrupts(&level);

        /* get the next thread */
        thread = pharosIThreadQueueGetHighest(queue);
    }

    /* re-enable interrupts */
    pharosCpuEnableInterrupts(&level);
}


ptrThreadQueueNode pharosIThreadQueueGetFree(void)
{
    /* return value */
    ptrThreadQueueNode node;


    /* on a single core there are no remote threads incoming so we can store all nodes in a single list */
    node = simpleListRemoveFirst(&freeNodes[pharosICoreRunning()]);

    /* make sure the node is not NULL (in debug mode) */
    PHAROS_ASSERT(node != NULL , "Free list is returning NULL node");

    return node;
}


void pharosIThreadQueueReturnFree(ptrThreadQueueNode node)
{
    /* on a single core we can use a single list to store all nodes, so we just return the node to that list */
    simpleListAppendLast(&freeNodes[pharosICoreRunning()] , node);
}
