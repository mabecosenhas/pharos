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


#ifndef PHAROS_KERNELAPI_TIME_H
#define PHAROS_KERNELAPI_TIME_H

#include <pharos/kernel/time/declarations.h>
#include <pharos/kernel/thread/declarations.h>
#include <pharos/kernel/core/coreconf.h>
#include <pharos/kernel/collection/treemap.h>
#include <pharos/kernel/collection/linkedlist.h>
#include <pharos/kernel/collection/simplelist.h>
#include <pharos/kernel/timer/declarations.h>
#include <pharos/kernel/thread/queue.h>
#include <pharos/hal/hal.h>
#include <pharos/hal/board.h>
#include <pharos/kernel/scheduler/declarations.h>

/**
 * Time tick handler for "internal" threads and timers. This is used to awaken threads on a specific flat scheduler
 */
struct TimeTickHandler
{
    /**
     * awake time
     */
    ClockTick awakeTime;

    /**
     * node on the tree map that contains all time tick handlers and their awake time
     */
    TreeNode node;

    /**
     * node on the free list
     */
    LinkedListNode usedNode;

    /**
     * map containing all threads that awake at the same instant
     */
    PriorityQueue threadQueue;

    /**
     * list of timers to execute at this instant
     */
    LinkedList timers;

    /**
     * list of threads whose deadline occurs at this instant
     */
    LinkedList deadlines;

    struct
    {
        uint32_t isOnFreeList : 1;
    } flags;
};


/**
 * Initialize a time tick handler
 * 
 * @constraint interrupts must be disabled when calling this function
 * 
 * @param handler handler to initialize
 * @param awakeTime time to execute the handler
 */
void pharosITimeInitializeTickHandler(ptrFlatScheduler scheduler , ptrTimeTickHandler handler , ClockTick awakeTime);


/**
 * Compares two time ticks
 *
 * Protection: none
 *
 * @param tickA tick A to compare
 * @param tickB tick B to compare
 *
 * @return returns 1 if tickA is larger than tickB, 0 if they are equal and -1 otherwise
 */
intptr_t pharosIClockTickHandlerComp(const ptrClockTick a , const ptrClockTick b);


/**
 * Add a time tick handler to the time environment (to be used later on on timeouts, sleeps, timers, etc)
 *
 * Protection: none
 *
 * @param scheduler scheduler where to add the handler
 * @param handler time tick to add
 */
void pharosITimeTickAdd(ptrFlatScheduler scheduler , ptrTimeTickHandler handler);


/**
 * Add a time tick handler to a thread. The thread is now blocked (only) by time.
 * That is, if a given time is elapsed the thread will be become ready
 * (of course, it could be unblocked earlier in case it was blocking on a queue, simple semaphore, etc that was released and the timeout did not occur)
 *
 * @constraint must run on the same core as the scheduler
 * @constraint interrupts must be disabled when calling this function
 *
 * @param thread thread to add
 * @param awakeTime instant when the thread should awake
 * @param type type of timeout
 * @param scheduler flat scheduler which will awake the time tick handler
 */
void pharosITimeTickAddThread(ptrThreadInfo thread , ClockTick awakeTime , TimeTickAwakenType type ,
                              ptrFlatScheduler scheduler);


/**
 * Add a time tick handler to a timer.
 *
 * @constraint interrupts must be disabled when calling this function
 *
 * @param scheduler scheduler where to add the time tick handler
 * @param timer timer to add
 * @param awakeTime instant when the timer should awake
 * 
 */
void pharosITimeTickAddTimer(ptrFlatScheduler scheduler , ptrTimer timer , ClockTick awakeTime);


/**
 * Remove a time tick handler associated with the thread
 *
 * @param thread thread to remove
 */
void pharosITimeTickRemoveThread(ptrThreadInfo thread);


/**
 * Removes the timer from the time tick tree map
 * 
 * @param timer timer to remove
 */
void pharosITimeTickRemoveTimer(ptrTimer timer);


/**
 * awake next highest priority thread that should have awaken at the same instant as the thread
 * 
 * @param thread thread
 */
void pharosITimeTickAwakeNext(ptrThreadInfo thread);


/**
 * Clock tick Interrupt Service Routine (in nominal state)
 *
 * Announces that another clock tick elapsed. Updates any thread that became ready
 *
 * Protection: none (assumes is being called inside interrupt)
 */
void pharosITimeTickIsr(void);




/**
 * ISR handler called when the WCET of the running thread has expired
 * 
 * @param number interrupt number
 */
void pharosIWcetHandlerIsr(const uint8_t *interruptedAddress);


/**
 * Get the timer that is currently executing
 * 
 * This function should only be called inside a software timer routine otherwise it will always return NULL
 */
ptrTimer pharosITimerGetExecuting(void);


/**
 * Adds a deadline to a RT thread
 *
 * @constraint interrupts must be disabled when calling this function
 * 
 * @param rTthread real time thread
 * @param deadlineInstant deadline instant
 */
void pharosITimeTickAddDeadline(ptrRtThread rTthread , ClockTick deadlineInstant);


/**
 * Removes a deadline handler from the thread
 * 
 * Constraint: when calling this function interrupts must be disabled
 * 
 * @param rTthread thread to remove the handler from
 * @param deadlineHandler handler which contains the deadline
 */
void pharosITimeTickRemoveDeadline(ptrRtThread rTthread);


/**
 * Determines if the type of awake is a timeout 
 * 
 * @param type type to check
 * 
 * @return returns TRUE if the awake type is timeout and false otherwise
 */
bool pharosITimeTickAwakenIsTimeout(TimeTickAwakenType type);


/**
 * Reset the "next time tick" of the scheduler for internal threads/timers/deadlines
 * Calculates (the "hard way" the next clock tick to execute)
 * 
 * @param scheduler flat scheduler to reset
 */
void pharosITimeResetNextTimeTick(ptrFlatScheduler scheduler);


/**
 * Update the next clock tick to execute if the tick handler that was just removed was the one active
 * 
 * @param scheduler scheduler to update
 * @param tick clock tick that was changed
 */
void pharosITimeUpdateNextClockTick(ptrFlatScheduler scheduler , ClockTick tick);


/**
 * Activate a single thread that was waiting to be activated on the core where it is currently blocked on
 * 
 * @param thread thread to activate
 */
void pharosITimeActivateThreadRmp(ptrThreadInfo thread);


/**
 * Activate a thread that was waiting to be activated on the running core
 * 
 * @param thread thread to activate
 */
void pharosITimeActivateThread(ptrThreadInfo thread);


/**
 * Determines if the time tick handler has nothing left to process
 * 
 * @param handler time tick handler 
 * 
 * @return returns TRUE if there is nothing left to process and FALSE if there is something left to process
 */
bool pharosITimeTickHandlerIsEmpty(ptrTimeTickHandler handler);


/**
 * Check if the timeout is a finite number
 * 
 * @param timeout timeout to check
 * 
 * @return returns TRUE if the timeout is a finite number and FALSE if it infinite or zero
 */
INLINE bool pharosITimeoutIsFinite(Timeout timeout);


#endif /* TIME_H */
