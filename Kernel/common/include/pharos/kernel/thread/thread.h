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


#ifndef PHAROS_KERNELAPI_THREAD_H
#define PHAROS_KERNELAPI_THREAD_H


#include <pharos/hal/hal.h>
#include <pharos/hal/board.h>
#include <pharos/kernel/thread/declarations.h>
#include <pharos/kernel/partition/declarations.h>
#include <pharos/kernel/collection/linkedlist.h>
#include <pharos/kernel/collection/treemap.h>
#include <pharos/kernel/thread/priority.h>
#include <pharos/kernel/thread/state.h>
#include <pharos/kernel/time/time.h>
#include <pharos/kernel/event/event.h>
#include <pharos/kernel/queue/queue.h>
#include <pharos/kernel/buffer/declarations.h>
#include <pharos/kernel/thread/queue.h>
#include <pharos/kernel/bulk/declarations.h>
#include <pharos/kernel/channel/declarations.h>
#include <pharos/kernel/thread/stack.h>
#include <pharos/kernel/resource/declarations.h>
#include <pharos/kernel/thread/id.h>
#include <pharos/hal/cpu/conf.h>
#include <pharos/kernel/future/future.h>
#include <pharos/kernel/future/futureQueue.h>    
#include <pharos/kernel/scheduler/declarations.h>

struct PharosThreadProperties
{
    /**
     * Base priority of the thread
     */
    ThreadPriority basePriority;

    /**
     * size (in bytes) of the user stack
     */
    uint32_t userStackSize;

    /**
     * size (in bytes) of the kernel stack
     */
    uint32_t kernelStackSize;

    /**
     * size (in bytes) of the shared stack
     */
    uint32_t sharedStackSize;

    /**
     * stack pattern (8 bytes)
     */
    uint64_t stackPattern;

    /**
     * execution time budget
     */
    Wcet budget;

    /**
     * replenishment period (in clock ticks)
     */
    ClockTick replenishmentPeriod;

    /**
     * name of the thread
     */
    char *name;
};

struct ThreadStatus
{
    /**
     * next deadline
     */
    ClockTick deadline;

    /**
     * current thread priority
     */
    ThreadPriority currentPriority;

    /**
     * Size of the user stack
     */
    uintptr_t userStackSize;

    /**
     * Size of the kernel stack
     */
    uintptr_t kernelStackSize;

    /**
     * Size of the shared stack
     */
    uintptr_t sharedStackSize;

    /**
     * current thread state
     */
    ThreadState state;
};

struct ThreadInfo
{
    /**
     * CPU context of the thread
     * 
     * @note initialized by Pharos
     */
    ThreadCpuContext cpuContext;

    /**
     * node of the thread on the priority queue when the thread is blocked 
     * (e.g. is blocked on a semaphore then this is the node on the semaphore priority queue)
     * 
     * @note initialized by Pharos
     */
    ThreadQueueNode blockedThreadQueueNode;

    /**
     * Id of the thread
     * 
     * @note initialized by Pharos
     */
    ThreadId threadId;

    /**
     * Stack used when running the application code
     * 
     * @note application should initialize this field with a stack buffer in the section of the partition/driver of 
     * the thread
     */
    Stack applicationStack;

    /**
     * Number of bytes to allocate on the user stack 
     * 
     * @note initialized by Pharos
     */
    size_t userStackAllocateSize;

    /**
     * Address of the user stack that was allocated
     * 
     * @note initialized by Pharos
     */
    void *userStackAllocateAddress;

    /**
     * Stack used when running the kernel code
     * 
     * @note application should initialize this field with a stack buffer inside the kernel section
     */
    Stack kernelStack;

    /**
     * Stack used when running the application code
     * 
     * @note application should define this pointer (to NULL if there is no access to shared objects 
     * such as resources, heavy weight queues or channels)
     */
    ptrStackShared sharedAccessStack;

    /**
     * Additional information for the core (multi-core or single-core)
     */
    PharosThreadCore rmp;

    /**
     * node on the thread ready list
     */
    LinkedListNode readyNode;

    /**
     * Pointer to the partition that owns the thread
     * 
     * If NULL means that belongs to the kernel
     */
    ptrPartition partition;

    /**
     * Pointer to the partition the thread is currently executing in. For example, when executing a filter or an 
     * IP call the running partition may be different that the partition that owns the thread
     */
    ptrPartition runningPartition;

    /**
     * Current context of the inter-partition call
     */
    ptrPharosCpuInterPartitionContext activeContext;

    /**
     * Pointer to the flat scheduler that schedules this thread (always the same and equal to the partition->scheduler)
     * A thread, when ready, is always placed on this scheduler
     */
    ptrFlatScheduler scheduler;

    /**
     * time tick handler used to awake this thread (and potentially others as well)
     */
    TimeTickHandler tickHandler;

    /**
     * base thread priority. Also corresponds to the maximum priority of the thread
     */
    ThreadPriority priority;

    /**
     * base thread priority
     */
    ThreadPriority currentPriority;

    /**
     * state of the thread
     */
    ThreadState state;

    /**
     * Queue of ceiling semaphores hold. The semaphore with highest ceiling will be used to determine the threads 
     * current priority
     */
    ThreadQueue ceilingSemaphoresHold;

    /**
     * node on the map
     */
    TreeNode awakeSameTimeNode;

    /**
     * node of the thread on the wait priority queue when the thread is waiting for a clock tick to be unblocked 
     * (e.g. timeout of a semaphore obtain or a thread sleep)
     */
    ThreadQueueNode waitThreadQueueNode;

    /**
     * pointer to the queue where the thread is blocked (or NULL if not blocked)
     */
    ptrThreadQueue blockedQueue;

    /**
     * pointer to the priority queue where the thread is waiting (or NULL if not waiting)
     */
    ptrPriorityQueue waitQueue;

    /**
     * Priority of the thread with which it was blocked
     * 
     * @note required since the pharosIThreadQueueUpdateThreadPrioOnThreadCore and
     * pharosIThreadQueueUpdateThreadPrioOnSchedulerCore methods could be running on different cores/partitions and 
     * also concurrently
     */
    ThreadPriority blockedPriority;

    /**
     * pointer to the suspended blocked queue (in case the thread was suspended and blocked, this allows to resume)
     */
    ptrThreadQueue suspendedBlockedQueue;

    /**
     * Clock tick that the thread was waiting on (if suspendedWaitQueue is != null) 
     */
    ClockTick suspendedAwakeTime;

    /**
     * User mode entry point of the thread
     */
    uintptr_t userEntryPoint;

    /**
     * Kernel mode entry point of the thread
     */
    uintptr_t kernelEntryPoint;

    /**
     * type of awake
     */
    TimeTickAwakenType type;

    /**
     * pointer to the time tick handler (if in use)
     */
    ptrTimeTickHandler timeTickHandler;

    /**
     * Future queue which contains the futures that this thread holds and are ready
     */
    LinkedList futureReadyQueue;

    /**
     * Future queue which contains the futures that this thread is blocked on
     */
    LinkedList futureBlockedQueue;

    /**
     * List of resources being held
     */
    LinkedList resourcesHold;

    /**
     * Future this thread is blocked on (if any)
     */
    ptrFuture blockedFuture;

    /**
     * worst execution time [us]
     */
    Wcet wcet;

    /**
     * execution time at the moment of the previous dispatch [us]
     */
    Wcet executionTime;

    /**
     * last dispatch instant [us]
     */
    uint64_t lastDispatchTime;

    /**
     * period in which the budget is replenished [clock tick]
     * 
     * For periodic threads corresponds to the thread period
     * For sporadic threads corresponds to the MIT
     * For aperiodic threads corresponds to the period in which the budget is replenished
     * 
     * @note application must initialize this field if the wcet field is initialized with a value different than
     * PHAROS_WCET_UNLIMITED
     */
    uint32_t replenishBudgetInterval;

    /**
     * last instant when the thread budget was restored [clock tick]
     */
    ClockTick lastReplenishmentInstant;

    /**
     * Name of the thread
     * 
     * @note application should initialize this field to place a name so that syscall pharosThreadGetId can get it.
     * Can be placed at NULL if pharosThreadGetId is not needed.
     */
    const char *name;

    /**
     * Name of the thread internally
     */
    char nameInternal[PHAROS_THREAD_NAME_MAX];

    /**
     * Thread node on the partition list of external threads
     */
    LinkedListNode externalThreadNode;

    /**
     * Thread node on the channel/hw queue/etc list of threads waiting for the channel/hw queue/etc to be initialized
     */
    LinkedListNode waitingStartNode;

    /**
     * Thread node on the partition list of created threads
     */
    SimpleListNode createdNode;

    /**
     * Node of this thread on the map of threads sorted by name
     */
    TreeNode nameNode;

    /**
     * Node of this thread on the map of threads sorted by thread id number
     */
    TreeNode idNode;

    /**
     * boolean flags
     */
    struct
    {
        /**
         * indicating if thread is waiting on an event
         */
        uint32_t isEventWaiting : 1;

        /**
         * indicates if the time tick has already elapsed
         */
        uint32_t hasTimeTickElapsed : 1;

        /**
         * indicates if the thread is blocked while reading a channel packet on a byte (as in opposition to a packet)
         */
        uint32_t isChannelPacketBlockedByte : 1;

        /**
         * result of the semaphore obtain/queue receive etc when a timeout is selected 
         * 
         * (if the timeout occurred then the timeout value will be placed, if the semaphore was released then the SUCCESS will be placed here)
         */
        uint32_t unblockDueToTimeout : 1;

        /**
         * Flag indicating if the thread is on the ready list (TRUE is on the list, FALSE otherwise)
         */
        uint32_t isOnReadyList : 1;

        /**
         * Flag indicating if the sporadic thread has already been activated once
         */
        uint32_t hasActivated : 1;
    } threadFlags;

    /**
     * contains the element that unblocked the thread.
     * For example, if the thread was waiting for an event, then this will have the event that unblocked the thread
     */
    union
    {
        /**
         * last event sent
         */
        Event event;

        /**
         * message received
         */
        ptrBulk message;

        /**
         * Pointer to the heavy weight queue message
         */
        struct
        {
            /**
             * Pointer to the heavy weight queue message
             */
            ptrHeavyWeightQueueMessage hwMessage;
        } hwq;

        struct
        {
            /**
             * channel bulk
             */
            ptrChannelBulkKernel channelBulk;

            /**
             * Return value
             */
            PharosChannelReceiveR returnValue;
        } channelReceive;

        /**
         * Bulk
         */
        ptrBulk bulk;

        /**
         * Channel bulk
         */
        ptrChannelBulkKernel channelBulk;

        struct
        {
            /**
             * Result of the future
             */
            PharosFutureWaitR result;
        } future;

    } unblockInfo;
};


/**
 * Internal Pharos function that performs the pharosWait function with supervisor privileges
 * 
 * @param clockTicks
 * 
 * @return 
 */
PharosWaitR pharosSWait(ClockTick clockTicks);


/**
 * Syscall for pharosThreadSuspend
 * 
 * @param id thread id to suspend
 * 
 * @return returns the result of the suspension
 */
PharosThreadSuspendR pharosSThreadSuspend(ptrThreadId id , bool rightNow);


/**
 * Syscall for pharosThreadSuspendSelf
 * 
 * @return returns the result of the suspension
 */
PharosThreadSuspendSelfR pharosSThreadSuspendSelf(bool rightNow);


/**
 * Syscall for pharosThreadResume
 * 
 * @param id thread id to resume
 * 
 * @return returns the result of the resume
 */
PharosThreadResumeR pharosSThreadResume(ptrThreadId id);


/**
 * Gets the current priority of a thread
 *
 * Protection: none required since the priority is a 8 bit variable and changes to it are atomic
 *
 * @param thread thread to get the priority
 *
 * @return returns the thread priority
 */
INLINE ThreadPriority pharosIThreadGetPriority(const ptrThreadInfo thread);


/**
 * Starts a thread 
 * 
 * @param id id of the thread to start
 * 
 * @return returns success or the corresponding error
 */
PharosThreadStartR pharosSThreadStart(ptrThreadId id);


/**
 * Starts a thread with a specified delay
 * 
 * @param id id of the thread to start
 * @param delay number of clock ticks from which to start the start
 * 
 * @return returns success or the corresponding error
 */
PharosThreadStartDelayR pharosSThreadStartDelay(ptrThreadId id , ClockTick delay);

/**
 * Syscall to get the name of the running thread
 * 
 * @param id address where to place the thread id
 * 
 * @return returns success or the error
 */
PharosThreadGetIdSelfR pharosSThreadGetIdSelf(ptrThreadId id);


/**
 * Syscall to get the thread id associated with the thread name
 * 
 * @param name name of the thread to look for
 * @param id address where to place the thread id
 * 
 * @return returns success or the error
 */
PharosThreadGetIdR pharosSThreadGetId(const char *name , ptrThreadId id);


/**
 * Syscall to get the thread status associated to the thread id
 * 
 * @param id id of the thread to get the status from
 * @param status address where to place the status 
 * 
 * @return returns success or the error 
 */
PharosThreadGetStatusR pharosSThreadGetStatus(ptrThreadId id , ptrThreadStatus status);


/**
 * Syscall to set the thread priority of the specified thread id
 * 
 * @param id id of the thread to update the priority
 * @param newPriority new priority to set. Must not be higher than the maximum allowed priority
 * 
 * @return returns success or the error
 */
PharosThreadPrioritySetR pharosSThreadSetPriority(ptrThreadId id , ThreadPriority newPriority);


/**
 * Syscall to restart the thread 
 * 
 * @param id id of the thread to restart 
 * 
 * @return returns success or the error
 */
PharosThreadRestartR pharosSThreadRestart(ptrThreadId id);


/**
 * Syscall to stop self the thread 
 * 
 * @return returns success or the error
 */
PharosThreadStopSelfR pharosSThreadStopSelf(bool rightNow);


/**
 * Syscall to stop the thread 
 * 
 * @param id id of the thread to stop 
 * 
 * @return returns success or the error
 */
PharosThreadStopR pharosSThreadStop(ptrThreadId id , bool rightNow);


/**
 * Determines the stack usage of the specified thread
 * 
 * @param id thread to calculate the stack usage of
 * @param exactMethod TRUE value to calculate the stack usage with an exact (and slow) method, FALSE value to 
 *        calculate an approximate (actual value can be bigger) stack usage using a faster method O(log(N)).
 * @param pattern pattern used to initialize the stack
 * @param userStackUsage pointer where to place the user stack usage (in bytes)
 * @param kernelStackUsage pointer where to place the kernel stack usage (in bytes)
 * @param sharedStackUsage pointer where to place the shared stack usage (in bytes)
 * 
 * @return returns success or the error
 */
PharosThreadStackR pharosSThreadStack(ptrThreadId id , bool exactMethod , uintptr_t *userStackUsage ,
                                      uintptr_t *kernelStackUsage , uintptr_t *sharedStackUsage);


/**
 * Adds the semaphore to the list of semaphores that the thread holds
 *
 * @constraint interrupts must be disabled when calling this function
 * 
 * @param thread thread to where to add the semaphore
 * @param semaphore semaphore to add
 */
INLINE void pharosIThreadAddCeiling(const ptrThreadInfo thread , ptrSemaphoreCeiling semaphore);


/**
 * Removes the semaphore to the list of semaphores the thread holds
 *
 * @constraint interrupts must be disabled when calling this function
 * 
 * @param thread thread to where to remove the semaphore
 * @param semaphore semaphore to remove
 */
INLINE void pharosIThreadRemoveCeiling(const ptrThreadInfo thread , ptrSemaphoreCeiling semaphore);


/**
 * Gets the ceiling semaphore with the highest ceiling that the thread holds
 *
 * @param thread thread holding semaphores
 *
 * @return returns a pointer to the last semaphore hold (or NULL if none)
 */
INLINE ptrSemaphoreCeiling pharosIThreadGetHighestCeiling(const ptrThreadInfo thread);


/**
 * determines if the thread has enough time to execute WCET microseconds
 * 
 * @param wcet worst execution time yet to perform
 * 
 * @return returns TRUE if the running thread has time left and FALSE otherwise
 */
bool pharosIThreadHasEnoughTime(ptrThreadInfo thread , Wcet wcet);

/**
 * Get a pointer to the thread specified by its id
 *
 * Protection: none required (no internal changes are performed and values read are never changed)
 *
 * @param threadId thread id
 *
 * @return returns NULL if there is no such thread
 */
ptrThreadInfo pharosIThreadGet(ptrThreadId threadId);


/**
 * determines if the thread is awake.
 * 
 * This method checks if the thread is waiting for a clock tick and if so, checks if that clock tick already elapsed or not.
 * 
 * If it already elapsed that means that other higher priority threads where activated at the same clock instant and this thread state has not yet been updated 
 * 
 * @param thread thread to check
 * 
 * @return returns TRUE if the thread is awake and FALSE otherwise
 */
bool pharosIThreadIsAwake(ptrThreadInfo thread);


/**
 * Initialize all threads (called at Pharos startup)
 *
 * Protection: none required since the method is only called at startup
 * 
 * @param restart restart flag
 */
void pharosIThreadsInitialize(ptrThreadTable threads , ptrPartition partition , bool restart);


/**
 * Initialize the idle thread
 */
void pharosIThreadIdleInitialize(void);


/**
 * Initialize a thread info (called at Pharos startup)
 *
 * @note a thread can only be periodic, sporadic or aperiodic. This method does not test if these arguments are correct.
 * It is the responsibility of the caller to pass the correct arguments.
 *
 * Protection: none required since the method is only called at startup
 *
 * @param thread thread to initialize
 * @param isPeriodic flag indicating if the thread is periodic
 * @param isSporadic flag indicating if the thread is sporadic
 * @param isAperiodic flag indicating if the thread is aperiodic
 * @param kernelEntryPoint kernel mode entry point
 * @param userEntryPoint user mode entry point
 * @param restart restart flag
 */
void pharosIThreadInitialize(ptrThreadInfo thread , ptrPartition partition , ptrFlatScheduler scheduler ,
                             bool isPeriodic , bool isSporadic , bool isAperiodic ,
                             ThreadNumber number , uintptr_t kernelEntryPoint , uintptr_t userEntryPoint , bool restart);


/**
 * Initialize the RMP information of the thread 
 * 
 * @param thread thread to initialize
 */
void pharosIThreadInitRmp(ptrThreadInfo thread);


/**
 * Restores the thread priority to its original value
 *
 * Protection: none required (protects inside)
 *
 * @param thread thread to restore the base priority
 */
INLINE void pharosIThreadRestorePriority(const ptrThreadInfo thread);


/**
 * Increments the running thread resource count (thread entered a resource access method)
 */
INLINE void pharosIThreadResourceAdd(ptrThreadInfo thread , ptrResource resource);


/**
 Decrements the running thread resource count
 */
INLINE void pharosIThreadResourceRemove(ptrThreadInfo thread , ptrResource resource);


/**
 * Checks if the thread is running inside a resource
 * 
 * @param thread to check
 * 
 * @return returns TRUE if the thread is running inside a resource and FALSE otherwise
 */
INLINE bool pharosIThreadIsInsideResource(ptrThreadInfo thread);


/**
 * Gets the last resource to be used by the thread 
 * 
 * @param thread thread 
 * 
 * @return returns the last resource to be used by the thread 
 */
INLINE ptrResource pharosIThreadResourceLast(ptrThreadInfo thread);

/**
 * determine if the thread is periodic 
 * 
 * @param thread thread to determine 
 * 
 * @return returns TRUE of the thread is periodic and FALSE otherwise
 */
INLINE bool pharosIThreadIsPeriodic(const ptrThreadInfo thread);


/**
 * determine if the thread is aperiodic 
 * 
 * @param thread thread to determine 
 * 
 * @return returns TRUE of the thread is aperiodic and FALSE otherwise
 */
INLINE bool pharosIThreadIsAperiodic(const ptrThreadInfo thread);


/**
 * Determine if the thread is RT (periodic or sporadic)
 * 
 * @param thread thread to check
 * 
 * @return returns TRUE if the thread is RT and FALSE otherwise
 */
INLINE bool pharosIThreadIsRt(const ptrThreadInfo thread);


/**
 * determine if the thread is sporadic 
 * 
 * @param thread thread to determine 
 * 
 * @return returns TRUE of the thread is sporadic and FALSE otherwise
 */
INLINE bool pharosIThreadIsSporadic(const ptrThreadInfo thread);


/**
 * Get the partition that owns the thread
 * 
 * @param thread thread to get the partition
 * 
 * @return returns the partition of the thread
 */
INLINE ptrPartition pharosIThreadGetPartition(const ptrThreadInfo thread);


/**
 * Create a new thread from dynamically allocated memory (only for hierarchical scheduling)
 * 
 * @param thread address where to place the thread
 * @param sizeofTcb size of the thread TCB (aperiodic, periodic or sporadic)
 * @param properties properties of thread (stack size, priority, etc)
 * @param userStackAllocateSize size of the user stack to allocate
 * 
 * @return returns success or the memory allocation error
 */
PharosThreadPeriodicCreateR pharosIThreadCreate(ptrThreadInfo *thread , uint32_t sizeofTcb ,
                                                const ptrPharosThreadProperties properties , size_t userStackAllocateSize);



/**
 * Validate the properties and id of a thread when attempting to create it
 * 
 * @param properties properties to validate
 * @param id thread id to validate
 * 
 * @return returns success or error if an error occurred during the validation
 */
PharosThreadPeriodicCreateR pharosIThreadPropertiesValidate(ptrPharosThreadProperties properties , ptrThreadId id , uint32_t sizeofTcb);


/**
 * Starts the thread assuming all previous checks have been made 
 * 
 * @param thread thread to start
 * @param delay delay with which to start the thread
 * @param start address where to place the instant when the thread started 
 * @param restart TRUE if the thread is being restarted
 */
void pharosIThreadStart(ptrThreadInfo thread , ClockTick delay , ClockTick *start);


/**
 * Stop a thread. Thread will not be scheduled after this call
 * 
 * @param thread thread to stop
 */
void pharosIThreadStop(ptrThreadInfo thread);


/**
 * Stop the thread on the current thread scheduler core
 * 
 * @constraint must be executed on the current thread scheduler core
 * 
 * @param thread thread to stop
 */
void pharosIThreadStopOnSchedulerCore(ptrThreadInfo thread);


/**
 * Suspend a thread (on any core). 
 * 
 * Removes the thread from the ready state on the core it executes, removes it from the
 * blocking queue and waiting queue
 * 
 * @param thread thread to suspend
 */
void pharosIThreadSuspend(ptrThreadInfo thread);


/**
 * Performs the suspend operation on the core that currently schedules the thread. 
 * 
 * If the thread is on a blocking queue and waiting queue, removes the thread from it and saves the state so when it is 
 * resumed it can go back
 * 
 * @param thread thread to suspend
 */
void pharosIThreadSuspendOnSchedulerCore(ptrThreadInfo thread);


/**
 * Resumes the thread. Calls the necessary operations on the thread and scheduler core(s)
 * 
 * @param thread thread to resume
 */
void pharosIThreadResume(ptrThreadInfo thread);


/**
 * Performs the thread resume operation on the core which is scheduling the thread 
 * 
 * If the thread was on a blocking and waiting queue, 
 * 
 * @param thread thread to resume
 */
void pharosIThreadResumeOnSchedulerCore(ptrThreadInfo thread);


/**
 * Update the core of the thread.
 * 
 * Used when a thread switches to another core
 * 
 * @param thread thread to update
 * @param core new core where the thread is blocked on
 */
void pharosIThreadUpdateCore(ptrThreadInfo thread , ptrCore core);


/**
 * Restores the core of the thread.
 * 
 * Used when a thread switches back from another core
 * 
 * @param thread thread to update
 */
void pharosIThreadRestoreCore(ptrThreadInfo thread);


/**
 * Allocates the stack (up or down - depending on the architecture), maintaining the required stack alignment
 * Changes the stack according to the size
 * 
 * @param stack stack where to allocate and modify
 * @param size number of bytes to allocate inside the stack 
 * 
 * @return returns the stack address where it is safe to place additional elements 
 */
void *pharosIStackAllocate(ptrStack stack , size_t size);


/**
 * Reserves space on the stack. The actual code of this function depends on whether the stack is ascending or descending
 * 
 * @param stack stack
 * @param size number of bytes to reserve on the stack
 */
void pharosStackReserve(const ptrStack stack , uint32_t size);


/**
 * Frees space on the stack. The actual code of this function depends on whether the stack is ascending or descending
 * 
 * @param stack stack
 * @param size number of bytes to free on the stack
 */
void pharosStackFree(const ptrStack stack , uint32_t size);


/**
 * Determines if the thread is in the started state (this includes checking if the thread partition is restarting)
 * 
 * @param thread thread to check
 * 
 * @return returns TRUE if the thread is started (and therefore could execute if in the ready state) and FALSE otherwise
 */
INLINE bool pharosIThreadIsStarted(ptrThreadInfo thread);


#endif /* THREAD_H */
