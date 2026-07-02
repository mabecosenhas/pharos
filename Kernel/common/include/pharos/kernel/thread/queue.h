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


#ifndef PHAROS_THREAD_QUEUE_H
#define PHAROS_THREAD_QUEUE_H


#include <pharos/kernel/thread/declarations.h>
#include <pharos/kernel/collection/treemap.h>
#include <pharos/kernel/collection/linkedlist.h>
#include <pharos/kernel/collection/simplelist.h>
#include <pharos/kernel/semaphore/declarations.h>
#include <pharos/kernel/thread/priority.h>
#include <pharos/kernel/partition/declarations.h>
#include <pharos/hal/cpu/conf.h>

struct PriorityQueueNode
{
    /**
     * Node on the list of objects with the same priority
     */
    LinkedListNode usedNode;

    /**
     * Linked list that will contain the objects (threads, semaphore ceiling) with the same priority
     */
    LinkedList list;

    /**
     * node on the map of the PriorityQueue
     */
    TreeNode mapNode;
};

struct PriorityQueue
{
    /**
     * Map of items sorted by priority Map<Priority , PriorityQueueNode>
     * 
     * Each entry on this map is a list of items
     */
    TreeMap items;
};

struct FifoQueueNode
{
    LinkedListNode node;
};

struct ThreadQueueNode
{
    /**
     * Node on the list of free thread queue nodes
     */
    SimpleListNode freeNode;

    union
    {
        FifoQueueNode fifo;
        PriorityQueueNode priority;
    } node;
};

struct FifoQueue
{
    LinkedList items;
};

struct ThreadQueue
{
    bool isFifo;

    union
    {
        PriorityQueue priority;
        FifoQueue fifo;
    } queue;
};


/**
 * Initialize a thread queue
 * 
 * @param queue thread queue to initialize
 */
void pharosIThreadQueueInitialize(ptrThreadQueue queue , bool isFifo);



/**
 * Initializes all the queues
 */
void pharosIThreadQueuesInitialize(void);


/**
 * Initialize the priority queue 
 * 
 * @param queue priority queue
 */
void pharosIPqInitialize(ptrPriorityQueue queue);

/**
 * Initialize a FIFO queue
 * 
 * @param queue FIFO queue to initialize
 */
void pharosIFifoQueueInitialize(ptrFifoQueue queue);


/**
 * Initializes the queue node to be used
 */
void pharosIThreadQueueInitThreadBlocked(ptrThreadQueueNode node);


void pharosIPqInitThreadWait(ptrThreadQueueNode node);


void pharosIThreadQueueInitSemCeil(ptrThreadQueueNode node);


/**
 * Determine if the thread queue is empty
 * 
 * @param queue thread queue to check
 * 
 * @return returns TRUE if the thread queue is empty and FALSE otherwise
 */
bool pharosIThreadQueueIsEmpty(ptrThreadQueue queue);


/**
 * determines if the thread queue has no more threads
 * 
 * @param queue queue to analyze
 * 
 * @return returns TRUE if the priority queue is empty and FALSE otherwise
 */
bool pharosIPqIsEmpty(ptrPriorityQueue queue);


/**
 * adds a semaphore ceiling to the queue 
 * 
 * @constraint interrupts must be disabled when calling this function
 *
 * @param queue queue where to add the semaphore
 * @param semaphore semaphore to add
 */
void pharosIPqAddSemCeil(ptrPriorityQueue queue , ptrSemaphoreCeiling semaphore);


/**
 * removes a semaphore ceiling from the queue of semaphores
 * 
 * @constraint interrupts must be disabled when calling this function
 * 
 * @param queue queue where to remove the semaphore
 * @param semaphore semaphore to remove
 */
void pharosIPqRemoveSemCeil(ptrPriorityQueue queue , ptrSemaphoreCeiling semaphore);


/**
 * gets the highest ceiling semaphore on the queue
 * 
 * @param queue queue to analyze
 * 
 * @return returns the semaphore ceiling with highest ceiling or NULL if there is no semaphore on the queue
 */
ptrSemaphoreCeiling pharosIPqGetHighestSemCeil(ptrPriorityQueue queue);


/**
 * Add a thread to the thread queue (thread is blocked on the thread queue)
 * 
 * @constraint interrupts must be disabled when calling this method
 * 
 * @param queue thread queue where to add the thread to
 * @param thread thread to block on the thread queue
 */
void pharosIThreadQueueAddBlocked(ptrThreadQueue queue , ptrThreadInfo thread);


/**
 * adds a thread to the queue of waiting threads
 * 
 * @constraint interrupts must be disabled
 * 
 * @param queue queue where to add the thread
 * @param queuePointer pointer to the queue of the thread (will be updated to indicate the thread is now on this queue. Will be important to remove the thread from this queue)
 */
void pharosIPqAddWait(ptrPriorityQueue queue , ptrThreadInfo thread);


/**
 * Removes the thread from the blocked queue (thread->blockedQueue) if the thread is blocked on any queue
 * 
 * @constraint interrupts should be disabled
 * @constraint method must be executed on the core of the current scheduler of the thread
 * 
 * @param thread thread to remove from its blocked queue
 * @param wasBlocked address where to place the return value (TRUE if it was blocked and FALSE otherwise)
 */
ptrThreadQueue pharosIThreadQueueRemoveBlocked(ptrThreadInfo thread);


/**
 * Remove the thread from its waiting queue
 * 
 * @param thread thread to remove
 * 
 * @return returns the previous queue
 */
void pharosIPqRemoveWait(ptrThreadInfo thread);


/**
 * Get the highest priority thread from the queue
 * 
 * @param queue thread queue from which to get the highest priority thread
 * 
 * @return returns a pointer to the thread with highest priority or NULL if the thread queue is empty
 */
ptrThreadInfo pharosIPqGetHighest(ptrPriorityQueue queue);


/**
 * Get the highest "priority" of the FIFO queue
 * 
 * @param queue FIFO queue
 * 
 * @return returns the thread that was placed first in the queue
 */
ptrThreadInfo pharosIFifoQueueGetHighest(ptrFifoQueue queue);


/**
 * Get the highest priority on the priority queue
 * 
 * @param queue priority queue
 * 
 * @return returns the thread with the highest priority on the queue
 */
ptrThreadInfo pharosIThreadQueueGetHighest(ptrThreadQueue queue);


/**
 * Update the thread priority (even if needs to update on a remote core)
 * 
 * @param thread thread to update its priority
 * @param newPriority new priority
 */
void pharosIThreadQueueUpdateThreadPrio(ptrThreadInfo thread , ThreadPriority newPriority);


/**
 * Performs the thread priority update operation on the core that runs the thread
 * 
 * Updates the thread on the ready list
 * 
 * @constraint must be executed on the core where the thread executes
 * 
 * @param thread thread to update its priority
 * @param newPriority new priority
 */
void pharosIThreadQueueUpdateThreadPrioOnThreadCore(ptrThreadInfo thread , ThreadPriority newPriority);


/**
 * Performs the thread priority update operation on the core that is currently scheduling the thread
 * 
 * @constraint must be executed on the core of the thread current scheduler 
 * 
 * @param thread thread to update its priority
 * @param newPriority new priority
 */
void pharosIThreadQueueUpdateThreadPrioOnSchedulerCore(ptrThreadInfo thread , ThreadPriority newPriority);


/**
 * Removes all threads from the thread queue 
 * 
 * @param queue thread queue to remove the threads
 */
void pharosIThreadQueueRemoveAllThreads(ptrThreadQueue queue);




/**
 * Remove the highest priority thread waiting on the queue
 * 
 * @param queue priority queue
 * 
 * @return returns the highest priority thread on the queue or NULL if the queue is empty
 */
ptrThreadInfo pharosIPqRemoveHighest(ptrPriorityQueue queue);


/**
 * Removes a thread from the priority queue
 * 
 * @constraint interrupts must be disabled
 * 
 * @param queue queue where to remove the thread from
 * @param thread thread to remove
 */
void pharosIPqRemoveThread(ptrPriorityQueue queue , ptrThreadInfo thread);


void pharosIFifoQueueAddBlocked(ptrFifoQueue queue , ptrThreadInfo thread);

/**
 * adds a thread to the queue of blocked threads
 * 
 * 
 * @param queue queue where to add the thread
 * @param thread thread to add to the queue
 */
void pharosIPqAddBlocked(ptrPriorityQueue queue , ptrThreadInfo thread);


/**
 * Removes a thread from the blocked queue
 * 
 * @constraint interrupts must be disabled
 * 
 * @param queue queue where to remove the thread from
 * @param thread thread to remove
 */
void pharosIFifoQueueRemoveThread(ptrFifoQueue queue , ptrThreadInfo thread);


/**
 * Determine if the FIFO queue is empty
 * 
 * @param queue FIFO queue to check
 * 
 * @return returns TRUE if the queue is empty and FALSE otherwise
 */
bool pharosIFifoQueueIsEmpty(ptrFifoQueue queue);


/**
 * Get a free node of the thread queue
 * 
 * @constraint interrupts must be disabled
 * 
 * @return returns a free node
 */
ptrThreadQueueNode pharosIThreadQueueGetFree(void);


/**
 * Free a node of the thread queue 
 * 
 * @constraint interrupts must be disabled
 * 
 * @param node node to free
 */
void pharosIThreadQueueReturnFree(ptrThreadQueueNode node);


#endif /* QUEUE_H */
