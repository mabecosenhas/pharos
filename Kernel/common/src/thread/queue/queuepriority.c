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
 * Adds the thread to the priority queue
 *
 * @constraint interrupts must be disabled
 * 
 * @param queue queue where to add the thread
 * @param thread thread to add
 * @param offset offset of the list to add
 */
static void priorityQueueAdd(ptrPriorityQueue queue , ptrThreadInfo thread , int16_t offset);


void pharosIPqInitialize(ptrPriorityQueue queue)
{
    /* initialize the map of threads where each position of the map is a list of threads with the same priority 
     * (in Java -> TreeMap<ThreadPriority, List<Thread>>) */
    treeMapInitialize(&queue->items , (ComparatorMethod) pharosIThreadPriorityComparator ,
                      OFFSETOF(ThreadQueueNode , node.priority.mapNode));
}


bool pharosIPqIsEmpty(ptrPriorityQueue queue)
{
    /* check queue is valid */
    PHAROS_ASSERT(queue != NULL , "Queue is invalid");

    /* if the map is empty then the queue is empty */
    return treeMapIsEmpty(&queue->items);
}


void pharosIPqRemoveWait(ptrThreadInfo thread)
{
    /* check that the thread is valid */
    PHAROS_ASSERT(thread != NULL , "Thread is invalid");

    /* make sure the thread is still waiting on the queue */
    if(thread->waitQueue != NULL)
    {
        /* remove the highest thread from the queue of threads wait for an instant */
        pharosIPqRemoveThread(thread->waitQueue , thread);

        /* reset the wait queue */
        thread->waitQueue = NULL;
    }
}


ptrThreadInfo pharosIPqGetHighest(ptrPriorityQueue queue)
{
    ptrThreadInfo result;

    /* list containing the highest priority threads */
    ptrLinkedList list;

    /* get the list of threads with the same priority */
    ptrThreadQueueNode node = treeMapGetFirst(&queue->items);


    /* check that the thread is valid */
    PHAROS_ASSERT(queue != NULL , "Queue is invalid");

    /* if there is an item on the list */
    if(node != NULL)
    {
        /* get the list */
        list = &node->node.priority.list;

        /* get the first item of the list */
        result = linkedListGetFirst(list);
    }
    else
    {
        /* there are no items on the queue */
        result = NULL;
    }

    /* return the highest priority thread on the queue */
    return result;
}


ptrThreadInfo pharosIPqRemoveHighest(ptrPriorityQueue queue)
{
    /* returned thread */
    ptrThreadInfo result;

    /* list containing the highest priority threads */
    ptrLinkedList list;

    /* get the list of threads with the same priority */
    ptrThreadQueueNode node = treeMapGetFirst(&queue->items);


    /* check that the thread is valid */
    PHAROS_ASSERT(queue != NULL , "Queue is invalid");

    /* if there is an item on the list */
    if(node != NULL)
    {
        /* get the list */
        list = &node->node.priority.list;

        /* get the first item of the list */
        result = linkedListRemoveFirst(list);

        /* check that the list was non-empty */
        PHAROS_ASSERT(result != NULL , "List of threads is empty");

        /* check if the list is empty now */
        if(linkedListIsEmpty(list) == TRUE)
        {
            /* then remove the first item on the map */
            treeMapRemoveFirst(&queue->items);

            /* add the list to the free lists */
            pharosIThreadQueueReturnFree(node);
        }
    }
    else
    {
        /* there are no items on the queue */
        result = NULL;
    }

    /* return the highest priority thread on the queue */
    return result;
}


void pharosIPqRemoveThread(ptrPriorityQueue queue , ptrThreadInfo thread)
{
    /* queue node */
    ptrThreadQueueNode node;


    /* check queue is valid */
    PHAROS_ASSERT(queue != NULL , "Queue is invalid");

    /* check that the queue map has been initialized */
    PHAROS_ASSERT(queue->items.comparatorMethod != NULL , "Queue map is not initialized");

    /* get the list on the queue corresponding to the thread (this cannot be NULL since the queuePointer was initialized) */
    node = (ptrThreadQueueNode) treeMapGet(&queue->items , &thread->blockedPriority);

    /* check that the node is valid */
    PHAROS_ASSERT(node != NULL , "Cannot remove thread from priority queue");

    /* remove the node of the thread from the list */
    linkedListRemoveObject(&node->node.priority.list , thread);

    /* if there is no other thread with the same priority */
    if(linkedListIsEmpty(&node->node.priority.list) == TRUE)
    {
        /* then remove the linked list from the map */
        treeMapRemove(&queue->items , &thread->blockedPriority);

        /* add the list to the free list */
        pharosIThreadQueueReturnFree(node);
    }
}


void pharosIPqInitThreadWait(ptrThreadQueueNode node)
{
    /* add the node to the list of free nodes */
    pharosIThreadQueueReturnFree(node);
}


void pharosIPqAddBlocked(ptrPriorityQueue queue , ptrThreadInfo thread)
{
    /* check that the thread is valid */
    PHAROS_ASSERT(thread != NULL , "Thread is invalid");

    /* check that the queue is valid */
    PHAROS_ASSERT(queue != NULL , "Queue is invalid");

    /* adds the thread to the queue of blocked threads */
    priorityQueueAdd(queue , thread , OFFSETOF(ThreadInfo , blockedThreadQueueNode.node.priority.usedNode));
}


void pharosIPqAddWait(ptrPriorityQueue queue , ptrThreadInfo thread)
{
    /* check that the thread is valid */
    PHAROS_ASSERT(thread != NULL , "Thread is invalid");

    /* check that the queue is valid */
    PHAROS_ASSERT(queue != NULL , "Queue is invalid");

    /* update the queue where the thread is waiting on */
    thread->waitQueue = queue;

    /* adds the thread to the queue of threads waiting (for timeout, sleep, ...) */
    priorityQueueAdd(queue , thread , OFFSETOF(ThreadInfo , waitThreadQueueNode.node.priority.usedNode));
}


void pharosIPqAddSemCeil(ptrPriorityQueue queue , ptrSemaphoreCeiling semaphore)
{
    /* get the list of semaphores ceiling with the same ceiling */
    ptrThreadQueueNode node = (ptrThreadQueueNode) treeMapGet(&queue->items , &semaphore->ceiling);


    /* check queue is valid */
    PHAROS_ASSERT(queue != NULL , "Queue is invalid");

    /* check semaphore is valid */
    PHAROS_ASSERT(semaphore != NULL , "Semaphore is invalid");

    /* if there are no semaphore with the same ceiling */
    if(node == NULL)
    {
        /* get a free node */
        node = pharosIThreadQueueGetFree();

        /* initialize the list of nodes */
        linkedListInitialize(&node->node.priority.list , OFFSETOF(SemaphoreCeiling , blockedQueueNode.node.priority.usedNode));

        /* append the semaphore to the free list */
        linkedListAppendLast(&node->node.priority.list , semaphore);

        /* put the node on the map */
        treeMapPut(&queue->items , &semaphore->ceiling , node);
    }
    else
    {
        /* use the existing list and append the thread to the last position */
        linkedListAppendLast(&node->node.priority.list , semaphore);
    }
}


void pharosIPqRemoveSemCeil(ptrPriorityQueue queue , ptrSemaphoreCeiling semaphore)
{
    /* get the list on the queue corresponding to the thread (this must be not NULL since the queuePointer was initialized) */
    ptrThreadQueueNode node = (ptrThreadQueueNode) treeMapGet(&queue->items , &semaphore->ceiling);


    /* check queue is valid */
    PHAROS_ASSERT(queue != NULL , "Queue is invalid");

    /* check semaphore is valid */
    PHAROS_ASSERT(semaphore != NULL , "Semaphore is invalid");

    /* remove the thread from the list */
    linkedListRemoveObject(&node->node.priority.list , semaphore);

    /* if there is no other thread with the same priority */
    if(linkedListIsEmpty(&node->node.priority.list) == TRUE)
    {
        /* then remove the linked list from the map */
        treeMapRemove(&queue->items , &semaphore->ceiling);

        /* add the node to the free lists */
        pharosIThreadQueueReturnFree(node);
    }
}


ptrSemaphoreCeiling pharosIPqGetHighestSemCeil(ptrPriorityQueue queue)
{
    /* return value */
    ptrSemaphoreCeiling result;

    /* get the list of semaphore with the same ceiling */
    ptrThreadQueueNode node = (ptrThreadQueueNode) treeMapGetFirst(&queue->items);

    /* list containing the highest priority ceiling semaphores */
    ptrLinkedList list;


    /* if there any semaphore (linked list) on the map containing the semaphores */
    if(node != NULL)
    {
        /* then get the list */
        list = &node->node.priority.list;

        /* get the first item of the list */
        result = linkedListGetFirst(list);
    }
    else
    {
        /* there are no items on the queue */
        result = NULL;
    }

    /* return the highest ceiling semaphore on the queue (or NULL if none) */
    return result;
}


void priorityQueueAdd(ptrPriorityQueue queue , ptrThreadInfo thread , int16_t offset)
{
    /* get the list of threads with the same priority */
    ptrThreadQueueNode node = treeMapGet(&queue->items , &thread->blockedPriority);


    /* if there are no threads with the same priority */
    if(node == NULL)
    {
        /* get a free node */
        node = pharosIThreadQueueGetFree();

        /* re-initialize the list with the correct offset */
        linkedListInitialize(&node->node.priority.list , offset);

        /* append the thread to the free list */
        linkedListAppendLast(&node->node.priority.list , thread);

        /* put the list on the map  */
        treeMapPut(&queue->items , &thread->blockedPriority , node);
    }
    else
    {
        /* use the existing list and append the thread to the last position */
        linkedListAppendLast(&node->node.priority.list , thread);
    }
}
