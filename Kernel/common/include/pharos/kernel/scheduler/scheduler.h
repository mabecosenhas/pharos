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


#ifndef PHAROS_SCHEDULER_H
#define PHAROS_SCHEDULER_H


#include <pharos/kernel/scheduler/declarations.h>
#include <pharos/kernel/thread/declarations.h>
#include <pharos/kernel/thread/thread.h>
#include <pharos/kernel/partition/declarations.h>

typedef enum
{
    PHAROS_SCHEDULER_FLAT = 0 ,
    PHAROS_SCHEDULER_HIERARCHICAL = 1 ,
} PharosSchedulerType;

struct Scheduler
{
    /**
     * type of scheduler (flat or hierarchical)
     */
    PharosSchedulerType type;

    /**
     * Core where the scheduler is running
     */
    ptrCore core;
};

struct FlatScheduler
{
    /**
     * Scheduler. Must be the first field in the structure so that a cast from ptrScheduler to 
     * ptrFlatScheduler can be made
     */
    Scheduler scheduler;

    /**
     * @brief array containing all ready threads
     * 
     * Each entry of the table contains a thread. No two threads can have the same priority hence the maximum number of threads allowed is 256
     * Since we "only" support semaphores with priority ceiling protocol at most there can be one other thread in the same priority (the one holding a semaphore with priority ceiling equal to the priority of the thread)
     */
    LinkedList pharosVThreadsReady[MAXIMUM_NUMBER_PRIORITIES];

    /**
     * Priority bit map
     */
    PriorityBitMap pharosVbitMap;

    /**
     * Tree map containing all tick handlers.
     *
     * @constraint protection against interrupts
     */
    TreeMap handlers;

    /**
     * next clock tick to be activated on internal threads/timers
     */
    ClockTick nextTicks;

    /**
     * List of free time tick handlers that can be used (for a timeout of a thread, deadline handling, etc)
     * 
     * @constraint protection against interrupts
     */
    LinkedList freeTimeTickHandlers;

    /**
     * List of installed interrupts 
     */
    LinkedList interruptsInstalled;

    /**
     * number of clock ticks that were processed on this flat scheduler 
     * 
     * Corresponds to the clock ticks since the system was reboot if a syscall is made to check them
     *
     * @constraint protection against interrupts
     */
    volatile ClockTick ticks;

    /**
     * last tick processed by the scheduler. Need for hierarchical scheduling
     */
    ClockTick lastProcessedTick;

    /**
     * Extra tick handler per flat scheduler
     * This is required since every thread has its own tick handler and every timer has its own tick handler. However,
     * When re-scheduling a timer a temporary new tick handler is required. This will correspond to this tick handler.
     */
    TimeTickHandler tickHandler;
};

struct HierarchicalSchedulerMajorFrame
{
    /**
     * Array of partitions. Each position in the array points to the partition that should start to execute at the same
     * index as in the ticks array.
     * 
     * @note initialized by the application
     */
    ptrPartition *partitions;

    /**
     * Array of duration (measured in clock ticks) for how long the partition is supposed to execute. 
     * 
     * @note initialized by the application
     */
    ClockTick *duration;

    /**
     * Number of items in partitions and ticks 
     * 
     * @note initialized by the application
     */
    uint32_t size;

    /**
     * Next tick to switch partition
     */
    ClockTick nextTick;

    /**
     * Next index to be executed
     */
    uint32_t indexNext;
};

struct HierarchicalScheduler
{
    /**
     * Scheduler. Must be the first field in the structure so that a cast from ptrScheduler to 
     * ptrHierarchicalScheduler can be made
     */
    Scheduler scheduler;

    /**
     * Major frame that determines which partition to execute at each instant in time
     * 
     * @note initialized by the application
     */
    HierarchicalSchedulerMajorFrame maf;

    struct
    {
        /**
         * Flag indicating if only the running partition interrupts are accepted while running running each partition.
         * 
         * Set to FALSE if all interrupts are accepted always
         * Set to TRUE if only the interrupts installed by the running partition are accepted
         * 
         * @note initialized by the application
         */
        uint32_t acceptOnlyPartitionInterrupts : 1;
    } flags;
};


/**
 * array containing 256 positions and for each position gives the lowest bit with a 1.
 * For example:
 *  index = 0000 0101 -> 0
 *  index = 0000 1100 -> 2
 *  index = 0101 0000 -> 4
 */
extern const uint8_t pharosVhighestBit[];


/**
 * initialize the scheduler for the specified core
 * 
 * @param core core in which the hierarchical scheduler is running on
 * @param scheduler scheduler to initialize
 */
void pharosISchedHierarchicalInit(ptrCore core , ptrHierarchicalScheduler scheduler);


/**
 * initialize the scheduler for the specified core
 * 
 * @param scheduler scheduler to initialize
 * @param core core in which the scheduler is executed
 */
void pharosISchedFlatInit(ptrFlatScheduler scheduler , ptrCore core);


/**
 * Adds a thread to the list of ready threads
 * 
 * @constraint must execute on the core of the thread->currentScheduler
 * 
 * @param thread thread to add
 */
void pharosISchedAddReadyThread(ptrThreadInfo thread);


/**
 * Adds a thread to the list of ready threads only if the thread belongs to running core
 * 
 * @note can be executed from any core
 * 
 * @param thread thread to add
 */
void pharosISchedAddReadyThreadRmp(ptrThreadInfo thread);


/**
 * Remove a ready thread from the list of ready threads.
 * 
 * @constraint must execute on the core of the scheduler
 * 
 * @param scheduler scheduler where to add the thread
 * @param thread thread to remove from the list of ready threads
 */
void pharosISchedRemoveReadyThread(ptrThreadInfo thread);


/**
 * Adds the idle thread for the running core 
 * 
 * @param idle idle thread to set
 */
void pharosISchedIdleThreadSet(CoreNumber runningCore , ptrThreadInfo idle);


/**
 * Get the highest ready thread.
 * 
 * Assumes there is always one ready thread, even if it is the idle thread
 * 
 * @constraint must execute on the core of the scheduler owner
 * @constraint interrupts must be disabled
 * 
 * @param scheduler flat scheduler from which to get the highest priority thread from 
 * 
 * @return returns the highest ready thread
 */
ptrThreadInfo pharosISchedGetHighestReady(ptrFlatScheduler scheduler);


/**
 * updates the thread priority on the ready list of threads.
 * The calling thread is updating the priority of itself
 * 
 * Protection: none required (protected internally) 
 * 
 * @param thread thread to update
 * @param newPriority new priority of the thread
 */
void pharosISchedUpdateThreadPrio(ptrThreadInfo thread , ThreadPriority newPriority);


/**
 * Update the hierarchical scheduler 
 * 
 * @param core running core
 * @param scheduler scheduler to update
 * @param currentTick current clock tick
 * 
 * @return returns the number of clock ticks to execute in the context of the new partition that was just switched (or not)
 */
ClockTick pharosISchedHierarchicalUpdate(ptrCore core , ptrHierarchicalScheduler scheduler , ClockTick currentTick);


/**
 * Gets an unused time tick handler
 *
 * @constraint interrupts must be disabled
 *
 * @return returns a free time tick handler pointer
 */
ptrTimeTickHandler pharosITimeTickHandlerGetNew(ptrFlatScheduler scheduler);


/**
 * Free a time tick handler (place it on the free list) 
 * 
 * @constraint interrupts must be disabled
 *
 * @param scheduler scheduler where to add the time tick handler
 * @param handler handler to free
 */
void pharosITimeTickHandlerFree(ptrFlatScheduler scheduler , ptrTimeTickHandler handler);


/**
 * Executes a single clock tick in the specified flat scheduler
 * 
 * @param scheduler scheduler where to execute a clock tick
 */
void pharosITimeTickFlatScheduler(ptrFlatScheduler scheduler);


/**
 * activate the highest priority thread waiting to be activated at the specified time tick
 * 
 * @param handler time tick
 */
ptrThreadInfo pharosIFlatSchedulerTimeActivateHighestPriorityThread(ptrTimeTickHandler handler);


/**
 * Remove a time tick handler (only if there is nothing on the handler left to execute)
 * 
 * @param scheduler scheduler to remove the handler from
 * @param thread that holds the handler
 * @param handler time tick handler that was updated (removed a thread, deadline, etc)
 */
bool pharosIFlatSchedulerTimeTickHandlerRemove(ptrFlatScheduler scheduler , ptrThreadInfo thread , ptrTimeTickHandler handler);


/**
 * Give a time tick handler to the scheduler
 * 
 * @param scheduler scheduler to use the handler 
 * @param handler time tick handler to be used 
 */
void pharosIFlatSchedulerTimeTickInit(ptrFlatScheduler scheduler , ptrTimeTickHandler handler);


/**
 * Get the map of clock tick handlers of internal threads/timers of the specified scheduler
 * 
 * @param scheduler flat scheduler
 * 
 * @return returns the map of clock tick handlers
 */
INLINE ptrTreeMap pharosIClockTickHandlersGet(ptrFlatScheduler scheduler);


/**
 * Get the map of clock tick handlers for remote threads of the specified scheduler
 * 
 * @param scheduler flat scheduler
 * 
 * @return returns the map of clock tick handlers
 */
INLINE ptrTreeMap pharosIClockTickRemoteHandlersGet(ptrFlatScheduler scheduler);


/**
 * Get the next clock tick of internal threads to be executed on the specified flat scheduler
 * 
 * @param scheduler flat scheduler
 * 
 * @return returns the next clock tick 
 */
INLINE ClockTick pharosIClockTickNextGet(ptrFlatScheduler scheduler);


/**
 * Set the next clock tick to execute on the specified flat scheduler
 * 
 * @param scheduler flat scheduler
 * @param nextTick next clock tick to execute
 */
INLINE void pharosIClockTickNextSet(ptrFlatScheduler scheduler , ClockTick nextTick);


/**
 * Get the next clock tick of remote threads to be executed on the specified flat scheduler
 * 
 * @param scheduler flat scheduler
 * 
 * @return returns the next clock tick 
 */
INLINE ClockTick pharosIClockTickNextRemoteGet(ptrFlatScheduler scheduler);


/**
 * Set the next clock tick of remote threads to execute on the specified flat scheduler
 * 
 * @param scheduler flat scheduler
 * @param nextTick next clock tick to execute
 */
INLINE void pharosIClockTickNextRemoteSet(ptrFlatScheduler scheduler , ClockTick nextTick);


/**
 * Increment the clock tick of the current scheduler
 * 
 * @param increment number of clock ticks to increment
 * @param core running core
 * 
 * @return returns the current clock ticks
 */
INLINE ClockTick pharosIClockTicksInc(ClockTick increment , ptrCore core);


/**
 * Determines if the scheduler running on the local core is hierarchical
 * 
 * @return returns TRUE if the scheduler is hierarchical and FALSE otherwise
 */
INLINE bool pharosISchedulerIsHierarchical(void);


/**
 * Get the number of clock ticks of the specified scheduler 
 * 
 * @param scheduler scheduler to get the number of clock ticks since boot from
 * 
 * @return returns the number of clock ticks
 */
INLINE ClockTick pharosIClockTicksGet(ptrFlatScheduler scheduler);


/**
 * Updates the running thread.
 * 
 * @constraint interrupts must be disabled
 * 
 * @param thread thread to update
 */
INLINE void pharosISchedUpdateRunningThread(ptrThreadInfo thread);


/**
 * determines if a context switch is needed (a higher priority thread is ready)
 * 
 * @return returns TRUE if a context switch is needed and FALSE otherwise
 */
INLINE bool pharosINeedsThreadContextSwitch(void);


#endif /* PHAROS_SCHEDULER_H */
