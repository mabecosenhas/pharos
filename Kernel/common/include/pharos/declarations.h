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


#ifndef PHAROS_API_DECLARATIONS_H
#define PHAROS_API_DECLARATIONS_H


#include <pharos/hal/hal.h>


/******************************************************************/
/* List of errors (that could be recovered from)                  */
/******************************************************************/


/**
 * Illegal instruction access error 
 * 
 * Occurs when trying to execute an assembly instruction that the processor does not know what it means. 
 * Normally occurs when attempting to execute code from the data section (invalid function pointer)
 */
#define PHAROS_ERR_ILLEGAL_INSTR                    (0x2U)

/**
 * Unaligned memory access error
 * 
 * Occurs when an attempt is made to read/write to a memory region which is not aligned. For example, in some CPUs
 * it is not possible to access a uint64_t variable if it is not aligned to 64 bits.
 */
#define PHAROS_ERR_UNALIGNED_MEM_ACCESS             (0x3U)

/**
 * Invalid syscall
 * 
 */
#define PHAROS_ERR_INVALID_SYSCALL                  (0x5U)

/**
 * I/O memory is overlapping with normal memory
 * 
 * Occurs if defined for that a partition has an I/O memory that has the same address that the normal 
 * partition memory
 */
#define PHAROS_ERR_I0_MEM_OVERLAPPING               (0x6U)

/**
 * Aperiodic thread has invalid replenishment period (1). 
 * It must be at least 2 since the precision of the clock is of one clock tick
 */
#define PHAROS_ERR_APERIODIC_INVALID_PERIOD         (0X7U)

/**
 * First number of the CPU specific errors
 * 
 * For example, the CPU could state that a watchdog reset is an error and should be dealt with at CPU startup.
 */
#define PHAROS_ERR_CPU_START                        (0x10000000U)


/**
 * First number of the Pharos general fatal errors
 * 
 * For example, no threads are defined
 */
#define PHAROS_FATAL_START                          (0x20000000U)    


/**
 * First number of the CPU specific fatal errors
 * 
 * For example, the CPU could state that an interrupt number/priority is invalid and abort the initialization
 */
#define PHAROS_FATAL_CPU_START                      (0x30000000U)


/**
 * First number of the application specific errors
 * 
 * For example, the application could define a set of valid states to be in and, if it detects somehow that it is in
 * a invalid state, the application could choose to raise a fatal error
 */
#define PHAROS_FATAL_APP_START                      (0x40000000U)    

/******************************************************************/
/* List of fatal errors                                           */
/******************************************************************/

/**
 * No threads
 * 
 * The application has not defined a single thread (on any partition)
 */
#define PHAROS_FATAL_NO_THREADS                     (PHAROS_FATAL_START + 0x0U)

/**
 * Hierarchical scheduling is defined but the partition does not have a flat scheduler
 */
#define PHAROS_FATAL_PARTITION_WITHOUT_SCHEDULER    (PHAROS_FATAL_START + 0x1U)

/**
 * Pharos internal error detected. Pharos is somehow in a wrong state. Perhaps the best solution is to restart.
 */
#define PHAROS_FATAL_INTERNAL_ERROR_DETECTED        (PHAROS_FATAL_START + 0x2U)

/**
 * Valid errors are the "#define" that start with "PHAROS_ERR_"
 */
typedef uint32_t PharosError , *ptrPharosError;


/**
 * Valid fatal errors are the "#define" that start with "PHAROS_FATAL_"
 */
typedef uint32_t PharosFatalError , *ptrPharosFatalError;


/**
 * Priority in Pharos
 */
typedef uint64_t PharosPriority , *ptrPharosPriority;

/**
 * Return type of the pharosWait function
 */
typedef enum
{
    /**
     * successfully waited
     */
    PHAROS_WAIT_SUCCESS = 0 ,

    /**
     * call to pharosWait is made inside interrupt and is not valid
     */
    PHAROS_WAIT_INSIDE_INTERRUPT = 1
} PharosWaitR;

/**
 * Return type of the pharosThreadSuspend function
 */
typedef enum
{
    PHAROS_THREAD_SUSPEND_SUCCESS = 0 ,

    /**
     * rightNow flag set to FALSE and the thread is currently in inter-partition call. 
     * This return value is also considered to be a success
     */
    PHAROS_THREAD_SUSPEND_DELAY = 1 ,
    PHAROS_THREAD_SUSPEND_INVALID_ID = 2 ,
    PHAROS_THREAD_SUSPEND_ALREADY_SUSPENDED = 3 ,
    PHAROS_THREAD_SUSPEND_INVALID_ID_ADDRESS = 4 ,

    /**
     * rightNow flag set to TRUE and the thread is currently in inter-partition call
     */
    PHAROS_THREAD_SUSPEND_INTER_PARTITION = 5 ,
} PharosThreadSuspendR;

/**
 * Return type of the pharosThreadSuspendSelf function
 */
typedef enum
{
    /**
     * Success
     */
    PHAROS_THREAD_SUSPEND_SELF_SUCCESS = 0 ,

    /**
     * rightNow flag set to FALSE and the thread is currently in inter-partition call
     * This return value is also considered to be a success
     */
    PHAROS_THREAD_SELF_SUSPEND_DELAY = PHAROS_THREAD_SUSPEND_DELAY ,

    /**
     * rightNow flag set to TRUE and the thread is currently in inter-partition call
     */
    PHAROS_THREAD_SUSPEND_SELF_INTER_PARTITION = PHAROS_THREAD_SUSPEND_INTER_PARTITION ,


    PHAROS_THREAD_SUSPEND_SELF_INSIDE_INTERRUPT = PHAROS_THREAD_SELF_SUSPEND_DELAY + 1 ,

} PharosThreadSuspendSelfR;

/**
 * Return type of the pharosThreadResume function
 */
typedef enum
{
    /**
     * thread resume was successful
     */
    PHAROS_THREAD_RESUME_SUCCESS = 0 ,
    PHAROS_THREAD_RESUME_INVALID_ID = 1 ,
    PHAROS_THREAD_RESUME_NOT_SUSPENDED = 2 ,
    PHAROS_THREAD_RESUME_INVALID_ID_ADDRESS = 3 ,
} PharosThreadResumeR;

/**
 * Return type of the pharosThreadGetIdSelf function
 */
typedef enum
{
    /**
     * Success
     */
    PHAROS_THREAD_GET_ID_SELF_SUCCESS = 0 ,
    PHAROS_THREAD_GET_ID_SELF_INVALID_ID = 1 ,
    PHAROS_THREAD_GET_ID_SELF_INSIDE_INTERRUPT = 2 ,
} PharosThreadGetIdSelfR;

typedef enum
{
    /**
     * Success
     */
    PHAROS_THREAD_APERIODIC_CREATE_SUCCESS = 0 ,

    /**
     * Address of properties does not belong to the calling partition memory space
     */
    PHAROS_THREAD_APERIODIC_CREATE_INVALID_PROPERTIES = 1 ,

    /**
     * Requested user stack is too small
     */
    PHAROS_THREAD_APERIODIC_CREATE_INVALID_USER_STACK = 2 ,

    /**
     * Requested kernel stack is too small
     */
    PHAROS_THREAD_APERIODIC_CREATE_INVALID_KERNEL_STACK = 3 ,

    /**
     * Requested shared stack is too small
     */
    PHAROS_THREAD_APERIODIC_CREATE_INVALID_SHARED_STACK = 4 ,
    PHAROS_THREAD_APERIODIC_CREATE_INVALID_PERIOD = 5 ,
    PHAROS_THREAD_APERIODIC_CREATE_INVALID_NAME = 6 ,
    PHAROS_THREAD_APERIODIC_CREATE_ALREADY_CREATED = 7 ,

    /**
     * Calls are only allowed when using Hierarchical scheduler or the priority is lower than the Core.maxDynamicPriority
     */
    PHAROS_THREAD_APERIODIC_CREATE_INVALID_PRIORITY = 8 ,

    /**
     * User dynamic memory space is insufficient to allocate the requested user stack 
     */
    PHAROS_THREAD_APERIODIC_CREATE_NO_USER_MEMORY = 9 ,

    /**
     * Kernel dynamic memory space is insufficient to allocate the requested kernel stack or the requested 
     * kernel stack is smaller than the minimum allowed
     */
    PHAROS_THREAD_APERIODIC_CREATE_NO_KERNEL_MEMORY = 10 ,

    /**
     * Shared stack dynamic memory space is insufficient to allocate the requested shared stack
     */
    PHAROS_THREAD_APERIODIC_CREATE_NO_SHARED_STACK_MEMORY = 11 ,
    PHAROS_THREAD_APERIODIC_CREATE_INVALID_ID = 12 ,
} PharosThreadAperiodicCreateR;

typedef enum
{
    /**
     * Success
     */
    PHAROS_THREAD_PERIODIC_CREATE_SUCCESS = PHAROS_THREAD_APERIODIC_CREATE_SUCCESS ,
    PHAROS_THREAD_PERIODIC_CREATE_INVALID_PROPERTIES = PHAROS_THREAD_APERIODIC_CREATE_INVALID_PROPERTIES ,
    PHAROS_THREAD_PERIODIC_CREATE_INVALID_USER_STACK = PHAROS_THREAD_APERIODIC_CREATE_INVALID_USER_STACK ,
    PHAROS_THREAD_PERIODIC_CREATE_INVALID_KERNEL_STACK = PHAROS_THREAD_APERIODIC_CREATE_INVALID_KERNEL_STACK ,
    PHAROS_THREAD_PERIODIC_CREATE_INVALID_SHARED_STACK = PHAROS_THREAD_APERIODIC_CREATE_INVALID_SHARED_STACK ,
    PHAROS_THREAD_PERIODIC_CREATE_INVALID_PERIOD = PHAROS_THREAD_APERIODIC_CREATE_INVALID_PERIOD ,
    PHAROS_THREAD_PERIODIC_CREATE_INVALID_NAME = PHAROS_THREAD_APERIODIC_CREATE_INVALID_NAME ,
    PHAROS_THREAD_PERIODIC_CREATE_ALREADY_CREATED = PHAROS_THREAD_APERIODIC_CREATE_ALREADY_CREATED ,
    /**
     * Calls are only allowed when using Hierarchical scheduler or the priority is lower than the Core.maxDynamicPriority
     */
    PHAROS_THREAD_PERIODIC_CREATE_INVALID_PRIORITY = PHAROS_THREAD_APERIODIC_CREATE_INVALID_PRIORITY ,
    PHAROS_THREAD_PERIODIC_CREATE_NO_USER_MEMORY = PHAROS_THREAD_APERIODIC_CREATE_NO_USER_MEMORY ,
    PHAROS_THREAD_PERIODIC_CREATE_NO_KERNEL_MEMORY = PHAROS_THREAD_APERIODIC_CREATE_NO_KERNEL_MEMORY ,
    PHAROS_THREAD_PERIODIC_CREATE_NO_SHARED_STACK_MEMORY = PHAROS_THREAD_APERIODIC_CREATE_NO_SHARED_STACK_MEMORY ,

    PHAROS_THREAD_PERIODIC_CREATE_INVALID_ID = PHAROS_THREAD_APERIODIC_CREATE_INVALID_ID ,

} PharosThreadPeriodicCreateR;

typedef enum
{
    /**
     * Success
     */
    PHAROS_THREAD_SPORADIC_CREATE_SUCCESS = PHAROS_THREAD_APERIODIC_CREATE_SUCCESS ,
    PHAROS_THREAD_SPORADIC_CREATE_INVALID_PROPERTIES = PHAROS_THREAD_APERIODIC_CREATE_INVALID_PROPERTIES ,
    PHAROS_THREAD_SPORADIC_CREATE_INVALID_USER_STACK = PHAROS_THREAD_APERIODIC_CREATE_INVALID_USER_STACK ,
    PHAROS_THREAD_SPORADIC_CREATE_INVALID_KERNEL_STACK = PHAROS_THREAD_APERIODIC_CREATE_INVALID_KERNEL_STACK ,
    PHAROS_THREAD_SPORADIC_CREATE_INVALID_SHARED_STACK = PHAROS_THREAD_APERIODIC_CREATE_INVALID_SHARED_STACK ,
    PHAROS_THREAD_SPORADIC_CREATE_INVALID_PERIOD = PHAROS_THREAD_APERIODIC_CREATE_INVALID_PERIOD ,
    PHAROS_THREAD_SPORADIC_CREATE_INVALID_NAME = PHAROS_THREAD_APERIODIC_CREATE_INVALID_NAME ,
    PHAROS_THREAD_SPORADIC_CREATE_ALREADY_CREATED = PHAROS_THREAD_APERIODIC_CREATE_ALREADY_CREATED ,
    /**
     * Calls are only allowed when using Hierarchical scheduler or the priority is lower than the Core.maxDynamicPriority
     */
    PHAROS_THREAD_SPORADIC_CREATE_INVALID_PRIORITY = PHAROS_THREAD_APERIODIC_CREATE_INVALID_PRIORITY ,
    PHAROS_THREAD_SPORADIC_CREATE_NO_USER_MEMORY = PHAROS_THREAD_APERIODIC_CREATE_NO_USER_MEMORY ,
    PHAROS_THREAD_SPORADIC_CREATE_NO_KERNEL_MEMORY = PHAROS_THREAD_APERIODIC_CREATE_NO_KERNEL_MEMORY ,
    PHAROS_THREAD_SPORADIC_CREATE_NO_SHARED_STACK_MEMORY = PHAROS_THREAD_APERIODIC_CREATE_NO_SHARED_STACK_MEMORY ,
    PHAROS_THREAD_SPORADIC_CREATE_INVALID_ID = PHAROS_THREAD_APERIODIC_CREATE_INVALID_ID ,

    /**
     * Invalid activation properties
     */
    PHAROS_THREAD_SPORADIC_CREATE_INVALID_ACTIVATION = 13 ,

} PharosThreadSporadicCreateR;

/**
 * Return type of the pharosThreadStart function
 */
typedef enum
{
    /**
     * Success
     */
    PHAROS_THREAD_START_SUCCESS = 0 ,
    PHAROS_THREAD_START_INVALID_ID = 1 ,
    PHAROS_THREAD_START_ALREADY_STARTED = 2 ,
    PHAROS_THREAD_START_INVALID_ID_ADDRESS = 3 ,
} PharosThreadStartR;

/**
 * Return type of the pharosThreadStartDelay function
 */
typedef enum
{
    /**
     * Success
     */
    PHAROS_THREAD_START_DELAY_SUCCESS = PHAROS_THREAD_START_SUCCESS ,
    PHAROS_THREAD_START_DELAY_INVALID_ID = PHAROS_THREAD_START_INVALID_ID ,
    PHAROS_THREAD_START_DELAY_ALREADY_STARTED = PHAROS_THREAD_START_ALREADY_STARTED ,
    PHAROS_THREAD_START_DELAY_INVALID_ID_ADDRESS = PHAROS_THREAD_START_INVALID_ID_ADDRESS ,
} PharosThreadStartDelayR;

/**
 * Return type of the pharosThreadGetId function
 */
typedef enum
{
    /**
     * Success
     */
    PHAROS_THREAD_GET_ID_SUCCESS = 0 ,
    PHAROS_THREAD_GET_ID_INVALID_NAME = 1 ,
    PHAROS_THREAD_GET_ID_UNKNOWN = 2 ,
    PHAROS_THREAD_GET_ID_INVALID_ID = 3 ,
} PharosThreadGetIdR;

/**
 * Return type of the pharosThreadGetStatus function
 */
typedef enum
{
    /**
     * Success
     */
    PHAROS_THREAD_GET_STATUS_SUCCESS = 0 ,
    PHAROS_THREAD_GET_STATUS_INVALID_ID = 1 ,
    PHAROS_THREAD_GET_STATUS_INVALID_STATUS = 2 ,
    PHAROS_THREAD_GET_STATUS_UNKNOWN = 3 ,
} PharosThreadGetStatusR;

/**
 * Return type of the pharosThreadPrioritySet function
 */
typedef enum
{
    /**
     * Successfully updated the thread priority
     */
    PHAROS_THREAD_PRIORITY_SET_SUCESS = 0 ,

    /**
     * Priority is higher (smaller in value) than the original priority - only used for flat schedulers.
     * With an hierarchical scheduler this value will never be returned.
     */
    PHAROS_THREAD_PRIORITY_SET_TOO_HIGH = 1 ,

    /**
     * Invalid thread id address
     */
    PHAROS_THREAD_PRIORITY_SET_INVALID_ID = 2 ,

    /**
     * Unknown id
     */
    PHAROS_THREAD_SET_PRIORITY_UNKNOWN = 3 ,
} PharosThreadPrioritySetR;

typedef enum
{
    /**
     * Successfully restarted the thread
     */
    PHAROS_THREAD_RESTART_SUCESS = 0 ,

    /**
     * Thread cannot restart while accessing an inter-partition object
     */
    PHAROS_THREAD_RESTART_USING_INTER_PARTITION = 1 ,

    /**
     * Invalid thread id
     */
    PHAROS_THREAD_RESTART_INVALID_ID = 2 ,

    /**
     * Invalid thread id address
     */
    PHAROS_THREAD_RESTART_INVALID_ID_ADDRESS = 3 ,

} PharosThreadRestartR;

typedef enum
{
    /**
     * Successfully stopped the thread (pharosThreadStopSelf does not actually return this value)
     */
    PHAROS_THREAD_STOP_SELF_SUCESS = 0 ,

    /**
     * Thread cannot stop right now and will be stopped once it returns from the resource or inter-partition call 
     * or filter
     */
    PHAROS_THREAD_STOP_SELF_DELAY = 1 ,

    /**
     * Requested to stop right now and thread cannot stop while doing an inter-partition call/resource/filter
     */
    PHAROS_THREAD_STOP_SELF_INTER_PARTITION = 2 ,

    /**
     * Running inside an interrupt 
     */
    PHAROS_THREAD_STOP_SELF_INSIDE_INTERRUPT = 3 ,

} PharosThreadStopSelfR;

typedef enum
{
    /**
     * Successfully stopped the thread
     */
    PHAROS_THREAD_STOP_SUCESS = PHAROS_THREAD_STOP_SELF_SUCESS ,

    /**
     * Thread cannot stop right now and will be stopped once it returns from the resource or inter-partition call 
     * or filter
     */
    PHAROS_THREAD_STOP_DELAY = PHAROS_THREAD_STOP_SELF_DELAY ,

    /**
     * Requested to stop right now and thread cannot stop while doing an inter-partition call
     */
    PHAROS_THREAD_STOP_INTER_PARTITION = PHAROS_THREAD_STOP_SELF_INTER_PARTITION ,

    /**
     * Invalid thread id
     */
    PHAROS_THREAD_STOP_INVALID_ID = PHAROS_THREAD_STOP_SELF_DELAY + 1 ,

    /**
     * Invalid thread id
     */
    PHAROS_THREAD_STOP_INVALID_ID_ADDRESS = PHAROS_THREAD_STOP_INVALID_ID + 1 ,

    /**
     * Thread did not yet start, so cannot be stopped
     */
    PHAROS_THREAD_STOP_NOT_STARTED = PHAROS_THREAD_STOP_INVALID_ID_ADDRESS + 1 ,

} PharosThreadStopR;

/**
 * Return type of the pharosThreadStack function
 */
typedef enum
{
    /**
     * Success
     */
    PHAROS_THREAD_STACK_SUCCESS = 0 ,
    PHAROS_THREAD_STACK_INVALID_ID = 1 ,
    PHAROS_THREAD_STACK_INVALID_ID_ADDRESS = 2 ,
    PHAROS_THREAD_STACK_INVALID_USER_STACK = 3 ,
    PHAROS_THREAD_STACK_INVALID_KERNEL_STACK = 4 ,
    PHAROS_THREAD_STACK_INVALID_SHARED_STACK = 5 ,
} PharosThreadStackR;

/**
 * Return type of the pharosSemCeilObtain function
 */
typedef enum
{
    /**
     * Success
     */
    PHAROS_SEMAPHORE_CEILING_OBTAIN_SUCCESS = 0 ,
    PHAROS_SEMAPHORE_CEILING_OBTAIN_ALREADY_HOLDER = 1 ,
    PHAROS_SEMAPHORE_CEILING_OBTAIN_INVALID = 2 ,
    PHAROS_SEMAPHORE_CEILING_OBTAIN_INVALID_CEILING_PRIORITY = 3 ,
    PHAROS_SEMAPHORE_CEILING_OBTAIN_INSIDE_INTERRUPT = 4 ,
    PHAROS_SEMAPHORE_CEILING_OBTAIN_NOT_AVAILABLE = 5 ,
    PHAROS_SEMAPHORE_CEILING_OBTAIN_TIMEOUT = 6 ,
} PharosSemCeilObtainR;

/**
 * Return type of the pharosSemCeilRelease function
 */
typedef enum
{
    /**
     * Success
     */
    PHAROS_SEMAPHORE_CEILING_RELEASE_SUCCESS = 0 ,
    PHAROS_SEMAPHORE_CEILING_RELEASE_INVALID = 1 ,
    PHAROS_SEMAPHORE_CEILING_RELEASE_NOT_HOLDER = 2 ,
    PHAROS_SEMAPHORE_CEILING_RELEASE_INSIDE_INTERRUPT = 3 ,
} PharosSemCeilReleaseR;

/**
 * Return type of the pharosSemMutexObtainTimeout function
 */
typedef enum
{
    /**
     * Success
     */
    PHAROS_SEMAPHORE_MUTEX_OBTAIN_TIMEOUT_SUCCESS = 0 ,
    PHAROS_SEMAPHORE_MUTEX_OBTAIN_TIMEOUT_INVALID = 1 ,
    PHAROS_SEMAPHORE_MUTEX_OBTAIN_TIMEOUT_TIMEOUT = 2 ,
    PHAROS_SEMAPHORE_MUTEX_OBTAIN_TIMEOUT_UNAVAILABLE = 3 ,
    PHAROS_SEMAPHORE_MUTEX_OBTAIN_TIMEOUT_INSIDE_INTERRUPT = 4 ,
} PharosSemMutexObtainTimeoutR;

/**
 * Return type of the pharosSemMutexObtain function
 */
typedef enum
{
    /**
     * Success
     */
    PHAROS_SEMAPHORE_MUTEX_OBTAIN_SUCCESS = 0 ,
    PHAROS_SEMAPHORE_MUTEX_OBTAIN_INVALID = 1 ,
    PHAROS_SEMAPHORE_MUTEX_OBTAIN_UNAVAILABLE = 2 ,
} PharosSemMutexObtainR;

/**
 * Return type of the pharosSemMutexRelease function
 */
typedef enum
{
    /**
     * Success
     */
    PHAROS_SEMAPHORE_MUTEX_RELEASE_SUCCESS = 0 ,
    PHAROS_SEMAPHORE_MUTEX_RELEASE_INVALID = 1 ,
    PHAROS_SEMAPHORE_MUTEX_RELEASE_ALREADY_RELEASED = 2
} PharosSemMutexReleaseR;

/**
 * Return type of the pharosSemCountObtain function
 */
typedef enum
{
    /**
     * Success
     */
    PHAROS_SEMAPHORE_COUNTING_OBTAIN_SUCCESS = 0 ,

    /**
     * Invalid counting semaphore number
     */
    PHAROS_SEMAPHORE_COUNTING_OBTAIN_INVALID = 1 ,

    /**
     * Counting semaphore is not available
     */
    PHAROS_SEMAPHORE_COUNTING_OBTAIN_UNAVAILABLE = 2
} PharosSemCountObtainR;

/**
 * Return type of the pharosSemCountObtainTimeout function
 */
typedef enum
{
    /**
     * Success
     */
    PHAROS_SEMAPHORE_COUNTING_OBTAIN_TIMEOUT_SUCCESS = 0 ,

    /**
     * Invalid counting semaphore number
     */
    PHAROS_SEMAPHORE_COUNTING_OBTAIN_TIMEOUT_INVALID = 1 ,

    /**
     * Timeout occurred while attempting to obtain a counting semaphore
     */
    PHAROS_SEMAPHORE_COUNTING_OBTAIN_TIMEOUT_TIMEOUT = 2 ,

    /**
     * Zero-timeout was specified and counting semaphore is not available
     */
    PHAROS_SEMAPHORE_COUNTING_OBTAIN_TIMEOUT_UNAVAILABLE = 3 ,

    /**
     * Attempting to obtain a counting semaphore with a non-zero-timeout inside an interrupt (caller not allowed to do 
     * that)
     */
    PHAROS_SEMAPHORE_COUNTING_OBTAIN_TIMEOUT_INSIDE_INTERRUPT = 4 ,

} PharosSemCountObtainTimeoutR;

/**
 * Return type of the pharosSemCountRelease function
 */
typedef enum
{
    /**
     * Success
     */
    PHAROS_SEMAPHORE_COUNTING_RELEASE_SUCCESS = 0 ,

    /**
     * Invalid counting semaphore number
     */
    PHAROS_SEMAPHORE_COUNTING_RELEASE_INVALID = 1 ,

    /**
     * Counting semaphore maximum count reached
     */
    PHAROS_SEMAPHORE_COUNTING_RELEASE_MAX_REACHED = 2 ,

} PharosSemCountReleaseR;

/**
 * Return type of the pharosTimerStart function
 */
typedef enum
{
    /**
     * Success
     */
    PHAROS_TIMER_START_SUCCESS = 0 ,

    /**
     * Invalid timer number
     */
    PHAROS_TIMER_START_INVALID = 1 ,

    /**
     * Timer already started and is running
     */
    PHAROS_TIMER_START_ALREADY_STARTED = 2 ,

    /**
     * Period is smaller than the configured period. Period can only be equal to or larger than the configured period
     */
    PHAROS_TIMER_START_INVALID_PERIOD = 3 ,

} PharosTimerStartR;

/**
 * Return type of the pharosTimerStop function
 */
typedef enum
{
    /**
     * Success
     */
    PHAROS_TIMER_STOP_SUCCESS = 0 ,

    /**
     * Invalid timer number
     */
    PHAROS_TIMER_STOP_INVALID = 1 ,

    /**
     * Timer already stopped
     */
    PHAROS_TIMER_STOP_ALREADY_STOPPED = 2 ,

} PharosTimerStopR;

/**
 * Return type of the pharosBulkGet function
 */
typedef enum
{
    /**
     * Success
     */
    PHAROS_BULK_GET_SUCCESS = 0 ,
    PHAROS_BULK_GET_INVALID = 1 ,
    PHAROS_BULK_GET_UNAVAILABLE = 2 ,
    PHAROS_BULK_GET_INVALID_BULK = 3 ,
    PHAROS_BULK_GET_INVALID_BUFFER = 4 ,
    PHAROS_BULK_GET_INSIDE_INTERRUPT = 5 ,
    PHAROS_BULK_GET_TIMEOUT = 6 ,
} PharosBulkGetR;

typedef enum
{
    /**
     * Success
     */
    PHAROS_BULK_GIVE_SUCCESS = 0 ,
    PHAROS_BULK_GIVE_INVALID_BULK = 1 ,
} PharosBulkGiveR;

/**
 * Return type of the pharosClockGet function
 */
typedef enum
{
    /**
     * Success
     */
    PHAROS_CLOCK_GET_DATE_SUCCESS = 0 ,
    PHAROS_CLOCK_GET_DATE_CLOCK_NOT_SET = 1 ,
    PHAROS_CLOCK_GET_DATE_INVALID = 2 ,
} PharosClockGetR;

/**
 * Return type of the pharosClockSet function
 */
typedef enum
{
    /**
     * Success
     */
    PHAROS_CLOCK_SET_DATE_SUCCESS = 0 ,
    PHAROS_CLOCK_SET_DATE_INVALID = 1 ,
} PharosClockSetR;

/**
 * Return type of the pharosEventSend function
 */
typedef enum
{
    /**
     * Success
     */
    PHAROS_EVENT_SEND_SUCCESS = 0 ,
    PHAROS_EVENT_SEND_INVALID = 1 ,
    PHAROS_EVENT_SEND_THREAD_ID_INVALID_ADDRESS = 2 ,
} PharosEventSendR;

/**
 * Return type of the pharosEventReceive function
 */
typedef enum
{
    /**
     * Success
     */
    PHAROS_EVENT_RECEIVE_SUCCESS = 0 ,
    PHAROS_EVENT_RECEIVE_EVENT_INVALID = 1 ,
    PHAROS_EVENT_RECEIVE_UNAVAILABLE = 2 ,
    PHAROS_EVENT_RECEIVE_TIMEOUT = 3 ,
    PHAROS_EVENT_RECEIVE_INSIDE_INTERRUPT = 4 ,
} PharosEventReceiveR;

/**
 * Return type of the pharosLwQueueSend function
 */
typedef enum
{
    /**
     * Success
     */
    PHAROS_LIGHTWEIGHT_QUEUE_SEND_SUCCESS = 0 ,
    PHAROS_LIGHTWEIGHT_QUEUE_SEND_INVALID = 1 ,
    PHAROS_LIGHTWEIGHT_QUEUE_SEND_INVALID_BULK = 2 ,
} PharosLwQueueSendR;

/**
 * Return type of the pharosLwQueueReceive function
 */
typedef enum
{
    /**
     * Success
     */
    PHAROS_LIGHTWEIGHT_QUEUE_RECEIVE_SUCCESS = 0 ,
    PHAROS_LIGHTWEIGHT_QUEUE_RECEIVE_INVALID = 1 ,
    PHAROS_LIGHTWEIGHT_QUEUE_RECEIVE_NO_MESSAGE = 2 ,
} PharosLwQueueReceiveR;

/**
 * Return type of the pharosLwQueueReceiveTimeout function
 */
typedef enum
{
    /**
     * Success
     */
    PHAROS_LIGHTWEIGHT_QUEUE_RECEIVE_TIMEOUT_SUCCESS = 0 ,
    PHAROS_LIGHTWEIGHT_QUEUE_RECEIVE_TIMEOUT_INVALID = 1 ,
    PHAROS_LIGHTWEIGHT_QUEUE_RECEIVE_TIMEOUT_TIMEOUT = 2 ,
    PHAROS_LIGHTWEIGHT_QUEUE_RECEIVE_TIMEOUT_NO_MESSAGE = 3 ,
    PHAROS_LIGHTWEIGHT_QUEUE_RECEIVE_TIMEOUT_INSIDE_INTERRUPT = 4 ,
} PharosLwQueueReceiveTimeoutR;

/**
 * Return type of the pharosResourceAccess function
 */
typedef enum
{
    /**
     * Successfully executed the resource
     */
    PHAROS_RESOURCE_ACCESS_SUCCESS = 0 ,

    /***********************************/
    /* Errors from the semaphore       */
    /***********************************/

    /**
     * Thread that is trying to obtain the semaphore ceiling has a priority higher than the ceiling
     */
    PHAROS_RESOURCE_ACCESS_INVALID_CEILING_PRIORITY = PHAROS_SEMAPHORE_CEILING_OBTAIN_INVALID_CEILING_PRIORITY , /* 3 */

    /**
     * Trying to execute the resource while running in the context of an interrupt
     */
    PHAROS_RESOURCE_ACCESS_INSIDE_INTERRUPT = PHAROS_SEMAPHORE_CEILING_OBTAIN_INSIDE_INTERRUPT , /* 4 */

    /**
     * Timeout waiting for the semaphore ceiling of the resource
     */
    PHAROS_RESOURCE_ACCESS_TIMEOUT = PHAROS_SEMAPHORE_CEILING_OBTAIN_TIMEOUT , /* 6 */

    /**
     * Invalid Resource Id. Start after the last semaphore obtain error
     */
    PHAROS_RESOURCE_ACCESS_INVALID = PHAROS_RESOURCE_ACCESS_TIMEOUT + 1 , /* 7 */

    /**
     * resource not granted because the periodic or sporadic caller thread cannot guarantee it will finish its time before the resource is freed
     */
    PHAROS_RESOURCE_ACCESS_NOT_GRANTED_WCET = PHAROS_RESOURCE_ACCESS_INVALID + 1 , /* 8 */

    /**
     * resource or partition that owns the resource have not been initialized yet or is restarting
     */
    PHAROS_RESOURCE_ACCESS_UNINITIALIZED = PHAROS_RESOURCE_ACCESS_NOT_GRANTED_WCET + 1 , /* 9 */

    /***********************************/
    /* Errors from the access          */
    /***********************************/

    PHAROS_RESOURCE_ACCESS_INVALID_OUTPUT = PHAROS_RESOURCE_ACCESS_UNINITIALIZED + 1 , /* 10 */

    /***********************************/
    /* Errors from the filter          */
    /***********************************/

    /**
     * Filter refused access to resource due to invalid partition/driver calling
     */
    PHAROS_RESOURCE_ACCESS_FILTER_INVALID_ENVIRONMENT = PHAROS_RESOURCE_ACCESS_INVALID_OUTPUT + 1 , /* 11 */

    /**
     * Filter refused access to resource due to the caller making calls to close together (overloading the resource)
     */
    PHAROS_RESOURCE_ACCESS_FILTER_TOO_CLOSE = PHAROS_RESOURCE_ACCESS_FILTER_INVALID_ENVIRONMENT + 1 , /* 12 */

    /**
     * Filter refused access to resource
     */
    PHAROS_RESOURCE_ACCESS_FILTER_REFUSED = PHAROS_RESOURCE_ACCESS_FILTER_TOO_CLOSE + 1 , /* 13 */

    /**
     * Filter refused due to invalid input
     */
    PHAROS_RESOURCE_ACCESS_FILTER_INVALID_INPUT = PHAROS_RESOURCE_ACCESS_FILTER_REFUSED + 1 , /* 14 */

    /**
     * Access was granted but the input was invalid and hence was refused
     */
    PHAROS_RESOURCE_ACCESS_INVALID_INPUT = PHAROS_RESOURCE_ACCESS_FILTER_INVALID_INPUT + 1 , /* 15 */

    /**
     * Running thread does not have a valid stack to access the shared object
     */
    PHAROS_RESOURCE_ACCESS_RUNNING_THREAD_INVALID_SHARED_STACK = PHAROS_RESOURCE_ACCESS_INVALID_INPUT + 1 , /* 16 */
} PharosResourceAccessR;

/**
 * Return type of the resource filter function
 */
typedef enum
{
    /**
     * Success
     */
    PHAROS_RESOURCE_FILTER_SUCCESS = PHAROS_RESOURCE_ACCESS_SUCCESS ,
    PHAROS_RESOURCE_FILTER_INVALID_ENVIRONMENT = PHAROS_RESOURCE_ACCESS_FILTER_INVALID_ENVIRONMENT ,
    PHAROS_RESOURCE_FILTER_TOO_CLOSE = PHAROS_RESOURCE_ACCESS_FILTER_TOO_CLOSE ,
    PHAROS_RESOURCE_FILTER_REFUSED = PHAROS_RESOURCE_ACCESS_FILTER_REFUSED ,
    PHAROS_RESOURCE_FILTER_INVALID_INPUT = PHAROS_RESOURCE_ACCESS_FILTER_INVALID_INPUT ,
} PharosResourceFilterR;

/**
 * Return type of the pharosChannelSend function
 */
typedef enum
{
    /**
     * Success
     */
    PHAROS_CHANNEL_SEND_SUCCESS = 0 ,
    PHAROS_CHANNEL_SEND_INVALID_CHANNEL_ADDRESS = 1 ,
    PHAROS_CHANNEL_SEND_INVALID_CHANNEL = 2 ,
    PHAROS_CHANNEL_SEND_INVALID_BULK = 3 ,
    PHAROS_CHANNEL_SEND_INVALID_BULK_ADDRESS = 5 ,
    PHAROS_CHANNEL_SEND_INVALID_BULK_BUFFER = 6 ,
    PHAROS_CHANNEL_SEND_INVALID_BULK_EXTRA_BUFFER = 7 ,
    PHAROS_CHANNEL_SEND_INVALID_FUTURE_ADDRESS = 10 ,
    PHAROS_CHANNEL_SEND_INVALID_FUTURE_HOLDER = 11 ,
    PHAROS_CHANNEL_SEND_INVALID_SHARED_STACK = 12 ,
    PHAROS_CHANNEL_SEND_UNINITIALIZED = 13 ,
    PHAROS_CHANNEL_SEND_FUTURE_IS_READY = 14 ,
    PHAROS_CHANNEL_SEND_PARTITION_RESTART = 15 ,

} PharosChannelSendR;

/**
 * Return type of the channel send filter function
 */
typedef enum
{
    /**
     * Success
     */
    PHAROS_CHANNEL_SEND_FILTER_SUCCESS = PHAROS_CHANNEL_SEND_SUCCESS ,
    PHAROS_CHANNEL_SEND_FILTER_REFUSED = PHAROS_CHANNEL_SEND_PARTITION_RESTART + 1 ,
    PHAROS_CHANNEL_SEND_FILTER_INVALID_SENDER = PHAROS_CHANNEL_SEND_PARTITION_RESTART + 2 ,
    PHAROS_CHANNEL_SEND_FILTER_TOO_CLOSE = PHAROS_CHANNEL_SEND_PARTITION_RESTART + 3 ,
    PHAROS_CHANNEL_SEND_FILTER_REFUSED_PRIORITY = PHAROS_CHANNEL_SEND_PARTITION_RESTART + 4
} PharosChannelFilterSendR;

/**
 * Return type of the pharosChannelReceiveLocal function
 */
typedef enum
{
    /**
     * Success
     */
    PHAROS_CHANNEL_RECEIVE_SUCCESS = 0 ,

    PHAROS_CHANNEL_RECEIVE_INVALID_CHANNEL_ADDRESS = 1 ,
    PHAROS_CHANNEL_RECEIVE_INVALID_CHANNEL = 2 ,
    PHAROS_CHANNEL_RECEIVE_INSIDE_INTERRUPT = 3 ,
    PHAROS_CHANNEL_RECEIVE_INVALID_BULK_ADDRESS = 4 ,
    PHAROS_CHANNEL_RECEIVE_INVALID_FUTURE_ADDRESS = 7 ,
    PHAROS_CHANNEL_RECEIVE_INVALID_SENDER_ADDRESS = 8 ,
    PHAROS_CHANNEL_RECEIVE_EMPTY = 9 ,
    PHAROS_CHANNEL_RECEIVE_INVALID_SHARED_STACK = 10 ,
    PHAROS_CHANNEL_RECEIVE_UNINITIALIZED = 11 ,
    PHAROS_CHANNEL_RECEIVE_INVALID_PRIORITY_ADDRESS = 12 ,

    /**
     * Trying to receive a bulk while another thread with a minimum priority with other thread already trying to receive 
     * it
     */
    PHAROS_CHANNEL_RECEIVE_QUEUE_THREAD_WAITING_WITH_MIN_PRIORITY = 14 ,
    /**
     * Cannot receive a channel bulk from a channel on a remote core inside an interrupt
     */
    PHAROS_CHANNEL_RECEIVE_REMOTE_INSIDE_INTERRUPT = 15 ,

    /**
     * The configuration of the channel bulks is incorrect (more MMU tables are required)
     */
    PHAROS_CHANNEL_RECEIVE_INVALID_MMU = 16 ,
    PHAROS_CHANNEL_RECEIVE_TIMEOUT = 17 ,
} PharosChannelReceiveR;

/**
 * Return type of the pharosChannelReceiveLocal function
 */
typedef enum
{
    /**
     * Success
     */
    PHAROS_CHANNEL_RECEIVE_LOCAL_SUCCESS = 0 ,
    PHAROS_CHANNEL_RECEIVE_LOCAL_INVALID_CHANNEL = PHAROS_CHANNEL_RECEIVE_INVALID_CHANNEL ,
    PHAROS_CHANNEL_RECEIVE_LOCAL_INSIDE_INTERRUPT = PHAROS_CHANNEL_RECEIVE_INSIDE_INTERRUPT ,
    PHAROS_CHANNEL_RECEIVE_LOCAL_INVALID_BULK = PHAROS_CHANNEL_RECEIVE_INVALID_BULK_ADDRESS ,
    PHAROS_CHANNEL_RECEIVE_LOCAL_INVALID_FUTURE = PHAROS_CHANNEL_RECEIVE_INVALID_FUTURE_ADDRESS ,
    PHAROS_CHANNEL_RECEIVE_LOCAL_INVALID_SENDER_ADDRESS = PHAROS_CHANNEL_RECEIVE_INVALID_SENDER_ADDRESS ,
    PHAROS_CHANNEL_RECEIVE_LOCAL_EMPTY = PHAROS_CHANNEL_RECEIVE_EMPTY ,
    PHAROS_CHANNEL_RECEIVE_LOCAL_UNINITIALIZED = PHAROS_CHANNEL_RECEIVE_UNINITIALIZED ,
    PHAROS_CHANNEL_RECEIVE_LOCAL_INVALID_PRIORITY_ADDRESS = PHAROS_CHANNEL_RECEIVE_INVALID_PRIORITY_ADDRESS ,
    PHAROS_CHANNEL_RECEIVE_LOCAL_QUEUE_THREAD_WAITING_WITH_MIN_PRIORITY = PHAROS_CHANNEL_RECEIVE_QUEUE_THREAD_WAITING_WITH_MIN_PRIORITY ,

    /**
     * The configuration of the channel bulks is incorrect (more MMU tables are required)
     */
    PHAROS_CHANNEL_RECEIVE_LOCAL_INVALID_MMU = PHAROS_CHANNEL_RECEIVE_INVALID_MMU ,
    PHAROS_CHANNEL_RECEIVE_LOCAL_TIMEOUT = PHAROS_CHANNEL_RECEIVE_TIMEOUT ,

} PharosChannelReceiveLocalR;

/**
 * Return type of the channel receive filter function
 */
typedef enum
{
    /**
     * Success
     */
    PHAROS_CHANNEL_RECEIVE_FILTER_SUCCESS = PHAROS_CHANNEL_RECEIVE_SUCCESS ,
    PHAROS_CHANNEL_RECEIVE_FILTER_REFUSED = PHAROS_CHANNEL_RECEIVE_TIMEOUT + 1 ,
    PHAROS_CHANNEL_RECEIVE_FILTER_INVALID_RECEIVER = PHAROS_CHANNEL_RECEIVE_TIMEOUT + 2 ,
    PHAROS_CHANNEL_RECEIVE_FILTER_TOO_CLOSE = PHAROS_CHANNEL_RECEIVE_TIMEOUT + 3 ,

} PharosChannelFilterReceiveR;

/**
 * Return type of the pharosChannelBulkFree function
 */
typedef enum
{
    /**
     * Success
     */
    PHAROS_CHANNEL_BULK_FREE_SUCCESS = 0 ,
    PHAROS_CHANNEL_BULK_FREE_INVALID_BULK = 1 ,
    PHAROS_CHANNEL_BULK_FREE_INVALID_BULK_ADDRESS = 2 ,
} PharosChannelBulkFreeR;

/**
 * Return type of the pharosChannelBulkQueueGet function
 */
typedef enum
{
    /**
     * Success
     */
    PHAROS_CHANNEL_BULK_QUEUE_GET_SUCCESS = 0 ,
    PHAROS_CHANNEL_BULK_QUEUE_GET_INVALID = 1 ,
    PHAROS_CHANNEL_BULK_QUEUE_GET_INVALID_BULK = 2 ,
    PHAROS_CHANNEL_BULK_QUEUE_GET_EMPTY = 4 ,
    /**
     * MMU configuration requires more MMU pages to be able to map the channel bulk
     */
    PHAROS_CHANNEL_BULK_QUEUE_GET_MMU_INVALID = 5 ,

    /**
     * Getting a channel bulk inside an interrupt with a timeout different from PHAROS_TIMEOUT_ZERO
     */
    PHAROS_CHANNEL_BULK_QUEUE_GET_INSIDE_INTERRUPT = 6 ,

    /**
     * Timeout while waiting for a channel bulk on the channel bulk queue
     */
    PHAROS_CHANNEL_BULK_QUEUE_GET_TIMEOUT = 7 ,
} PharosChannelBulkQueueGetR;

/**
 * Heavy weight queue send method return values
 */
typedef enum
{
    /**
     * Success
     */
    PHAROS_HEAVY_WEIGHT_SEND_SUCCESS = 0 ,
    PHAROS_HEAVY_WEIGHT_SEND_INVALID = 1 ,
    PHAROS_HEAVY_WEIGHT_SEND_INVALID_SIZE = 2 ,
    PHAROS_HEAVY_WEIGHT_SEND_INVALID_BUFFER = 3 ,
    PHAROS_HEAVY_WEIGHT_SEND_FULL = 4 ,
    PHAROS_HEAVY_WEIGHT_SEND_INVALID_SHARED_STACK = 5 ,
    PHAROS_HEAVY_WEIGHT_SEND_INVALID_FUTURE = 6 ,
    PHAROS_HEAVY_WEIGHT_SEND_FUTURE_IN_USE = 7 ,
    PHAROS_HEAVY_WEIGHT_SEND_UNINITIALIZED = 8 ,
    PHAROS_HEAVY_WEIGHT_SEND_FUTURE_IS_READY = 9 ,

    /**
     * Trying to send a hw message inside an interrupt with a timeout
     */
    PHAROS_HEAVY_WEIGHT_SEND_TIMEOUT_INSIDE_INTERRUPT = 10 ,

    /**
     * Trying to send a hw message with a timeout but the timeout has elapsed and no messages are available
     */
    PHAROS_HEAVY_WEIGHT_SEND_TIMEOUT = 11 ,

    PHAROS_HEAVY_WEIGHT_SEND_PARTITION_RESTART = 12 ,

    PHAROS_HEAVY_WEIGHT_SEND_FILTER_INVALID_SENDER = 13 ,
    PHAROS_HEAVY_WEIGHT_SEND_FILTER_TOO_CLOSE = 14 ,
    PHAROS_HEAVY_WEIGHT_SEND_FILTER_REFUSED = 15 ,
    PHAROS_HEAVY_WEIGHT_SEND_FILTER_INVALID_MESSAGE_POINTER = 16 ,
    PHAROS_HEAVY_WEIGHT_SEND_FILTER_INVALID_MESSAGE_SIZE = 17 ,
    PHAROS_HEAVY_WEIGHT_SEND_FILTER_INVALID_MESSAGE_PRIORITY = 18 ,
} PharosHwQueueSendR;

/**
 * Heavy weight queue filter method return values
 */
typedef enum
{
    /**
     * Success
     */
    PHAROS_HEAVY_WEIGHT_FILTER_SUCCESS = PHAROS_HEAVY_WEIGHT_SEND_SUCCESS ,
    PHAROS_HEAVY_WEIGHT_FILTER_INVALID_SENDER = PHAROS_HEAVY_WEIGHT_SEND_FILTER_INVALID_SENDER ,
    PHAROS_HEAVY_WEIGHT_FILTER_TOO_CLOSE = PHAROS_HEAVY_WEIGHT_SEND_FILTER_TOO_CLOSE ,
    PHAROS_HEAVY_WEIGHT_FILTER_REFUSED = PHAROS_HEAVY_WEIGHT_SEND_FILTER_REFUSED ,
    PHAROS_HEAVY_WEIGHT_FILTER_INVALID_MESSAGE_POINTER = PHAROS_HEAVY_WEIGHT_SEND_FILTER_INVALID_MESSAGE_POINTER ,
    PHAROS_HEAVY_WEIGHT_FILTER_INVALID_MESSAGE_SIZE = PHAROS_HEAVY_WEIGHT_SEND_FILTER_INVALID_MESSAGE_SIZE ,
    PHAROS_HEAVY_WEIGHT_FILTER_INVALID_MESSAGE_PRIORITY = PHAROS_HEAVY_WEIGHT_SEND_FILTER_INVALID_MESSAGE_PRIORITY ,
} PharosHwQueueFilterR;

/**
 * Heavy weight queue receive method return values
 */
typedef enum
{
    /**
     * Heavy weight queue received a message
     */
    PHAROS_HEAVY_WEIGHT_RECEIVE_SUCCESS = 0 ,

    /**
     * Invalid heavy weight queue 
     */
    PHAROS_HEAVY_WEIGHT_RECEIVE_INVALID = 1 ,

    /**
     * Heavy weight queue is empty
     */
    PHAROS_HEAVY_WEIGHT_RECEIVE_EMPTY = 2 ,

    /**
     * Queue receive invoked inside an interrupt
     */
    PHAROS_HEAVY_WEIGHT_RECEIVE_INSIDE_INTERRUPT = 3 ,

    /**
     * Invalid buffer to where to place the message to.
     * 
     * The buffer must have enough size to fit the complete message and must be fully inside the calling thread environment. Otherwise, it could be trying 
     * to overwrite another partition/driver memory space
     */
    PHAROS_HEAVY_WEIGHT_RECEIVE_INVALID_BUFFER = 4 ,

    /**
     * A timeout occurs while waiting for the message
     */
    PHAROS_HEAVY_WEIGHT_RECEIVE_TIMEOUT = 5 ,

    /**
     * Invalid priority address
     */
    PHAROS_HEAVY_WEIGHT_RECEIVE_INVALID_PRIORITY = 6 ,

    /**
     * Invalid size address
     */
    PHAROS_HEAVY_WEIGHT_RECEIVE_INVALID_SIZE = 7 ,

    /**
     * Invalid size future (is not NULL and is at an invalid address)
     */
    PHAROS_HEAVY_WEIGHT_RECEIVE_INVALID_FUTURE = 8 ,
} PharosHwQueueReceiveR;

/**
 * Interrupt install method return values
 */
typedef enum
{
    /**
     * Success
     */
    PHAROS_INTERRUPT_INSTALL_SUCCESS = 0 ,
    PHAROS_INTERRUPT_INSTALL_INVALID_INDEX = 1 ,
    PHAROS_INTERRUPT_INSTALL_INTERRUPT_INVALID = 2 ,
    PHAROS_INTERRUPT_INSTALL_ALREADY_INSTALLED = 3
} PharosInterruptInstallR;

/**
 * Interrupt remove method return values
 */
typedef enum
{
    /**
     * Success
     */
    PHAROS_INTERRUPT_REMOVE_SUCCESS = 0 ,
    PHAROS_INTERRUPT_REMOVE_INVALID_INDEX = 1 ,
    PHAROS_INTERRUPT_REMOVE_NOT_INSTALLED = 2
} PharosInterruptRemoveR;

/**
 * Critical section execute method return values
 */
typedef enum
{
    /**
     * Success
     */
    PHAROS_CRITICAL_SECTION_EXECUTE_SUCCESS = 0 ,
    PHAROS_CRITICAL_SECTION_EXECUTE_INVALID_INDEX = 1 ,
    PHAROS_CRITICAL_SECTION_EXECUTE_INVALID_OUTPUT = 2 ,
} PharosCsExecuteR;

/**
 * Inter-partition call return values
 */
typedef enum
{
    /**
     * Success
     */
    PHAROS_IPC_RESULT_SUCCESS = 0 ,
    PHAROS_IPC_RESULT_INVALID_CALL = 1 ,
    PHAROS_IPC_RESULT_INVALID_INPUT = 2 ,
    PHAROS_IPC_RESULT_INVALID_OUTPUT = 3 ,
    PHAROS_IPC_RESULT_INVALID_SHARED_STACK = 4 ,
    PHAROS_IPC_RESULT_SHARED_STACK_SMALL = 5 ,
    PHAROS_IPC_RESULT_UNINITIALIZED = 6 ,
    PHAROS_IPC_FILTER_RESULT_INVALID_CALLER = 7 ,
    PHAROS_IPC_FILTER_RESULT_OVERLOAD = 8 ,
    PHAROS_IPC_FILTER_RESULT_INVALID_INPUT = 9 ,
    PHAROS_IPC_FILTER_RESULT_REFUSED = 10 ,

    /**
     * Custom return values should start from this value
     */
    PHAROS_IPC_RESULT_LAST = 10000U ,
} PharosIpCallR;

typedef enum
{
    /**
     * Success
     */
    PHAROS_FUTURE_GET_SUCCESS = 0 ,
    PHAROS_FUTURE_GET_INVALID_ADDRESS = 1 ,
    PHAROS_FUTURE_GET_EMPTY = 2 ,
} PharosFutureGetR;

typedef enum
{
    /**
     * Success
     */
    PHAROS_FUTURE_FREE_SUCCESS = 0 ,
    PHAROS_FUTURE_FREE_INVALID = 1 ,
    PHAROS_FUTURE_FREE_INVALID_ADDRESS = 1 ,
    PHAROS_FUTURE_FREE_IN_USE = 2 ,
    PHAROS_FUTURE_FREE_ALREADY_FREE = 3 ,
    PHAROS_FUTURE_FREE_INVALID_BULK_ADDRESS = 4 ,

    /**
     * can only free the bulk associated with the future
     */
    PHAROS_FUTURE_FREE_INVALID_BULK = 5 ,
} PharosFutureFreeR;

typedef enum
{
    /**
     * Success
     */
    PHAROS_FUTURE_WAIT_SUCCESS = 0 ,
    PHAROS_FUTURE_WAIT_INVALID = 1 ,
    PHAROS_FUTURE_WAIT_INVALID_ARG = 2 ,
    PHAROS_FUTURE_WAIT_IS_WAITING = 3 ,
    PHAROS_FUTURE_WAIT_TIMEOUT = 4 ,
    PHAROS_FUTURE_WAIT_UNAVAILABLE = 5 ,
    PHAROS_FUTURE_WAIT_INSIDE_INTERRUPT = 6 ,

    /**
     * Partition that held the future did not process it correctly. For example, it was restarted or if the future
     * was sent together with a channel bulk and there was an error in the channel reception 
     * (PHAROS_CHANNEL_RECEIVE_INVALID_MMU).
     */
    PHAROS_FUTURE_WAIT_PARTITION_ERROR = 7 ,

    PHAROS_FUTURE_WAIT_INVALID_ADDRESS = 8 ,

    PHAROS_FUTURE_WAIT_INVALID_BULK_ADDRESS = 9 ,

    /**
     * Cannot add the bulk to the receiver since the MMU requires additional tables
     */
    PHAROS_FUTURE_WAIT_INVALID_MMU = 10 ,
} PharosFutureWaitR;

typedef enum
{
    /**
     * Success
     */
    PHAROS_FUTURE_QUEUE_WAIT_SUCCESS = 0 ,
    PHAROS_FUTURE_QUEUE_WAIT_INVALID_ARGUMENT_ADDRESS = 1 ,
    PHAROS_FUTURE_QUEUE_WAIT_INVALID_FUTURE_ADDRESS = 2 ,
    PHAROS_FUTURE_QUEUE_WAIT_UNAVAILABLE = 3 ,
    PHAROS_FUTURE_QUEUE_WAIT_TIMEOUT = 4 ,
    PHAROS_FUTURE_QUEUE_WAIT_EMPTY = 5 ,
    PHAROS_FUTURE_QUEUE_WAIT_INSIDE_INTERRUPT = 6 ,
    PHAROS_FUTURE_QUEUE_WAIT_INVALID_BULK_ADDRESS = 7 ,
    PHAROS_FUTURE_QUEUE_WAIT_INVALID_MMU = PHAROS_FUTURE_WAIT_INVALID_MMU ,
} PharosFutureQueueWaitR;

typedef enum
{
    /**
     * Ready was successful
     */
    PHAROS_FUTURE_READY_SUCCESS = 0 ,

    /**
     * Partition owner of the future has restarted, so the future cannot be made ready
     */
    PHAROS_FUTURE_READY_PARTITION_RESTART = 1 ,

    /**
     * Invalid future id
     */
    PHAROS_FUTURE_READY_INVALID = 2 ,

    /**
     * Invalid ready argument
     */
    PHAROS_FUTURE_READY_INVALID_ARG = 3 ,

    /**
     * Future was already made ready
     */
    PHAROS_FUTURE_READY_ALREADY_READY = 4 ,

    /**
     * Future id address is invalid 
     */
    PHAROS_FUTURE_READY_INVALID_ADDRESS = 5 ,

    /**
     * Bulk address was specified (that is, it is not NULL) but invalid (does not belong to the memory area that
     * the calling partition can access)
     */
    PHAROS_FUTURE_READY_INVALID_BULK_ADDRESS = 6 ,

    /**
     * Bulk address was specified (that is, it is not NULL) but invalid (partition does not have permissions to 
     * the bulk)
     */
    PHAROS_FUTURE_READY_INVALID_BULK = 7 ,

} PharosFutureReadyR;

typedef enum
{
    /**
     * Success
     */
    PHAROS_FUTURE_QUEUE_ADD_SUCCESS = 0 ,
    PHAROS_FUTURE_QUEUE_ADD_INVALID = 1 ,
    PHAROS_FUTURE_QUEUE_ADD_IS_WAITING = 2 ,
    PHAROS_FUTURE_QUEUE_ADD_INSIDE_INTERRUPT = 3 ,
    PHAROS_FUTURE_QUEUE_ADD_INVALID_ADDRESS = 4 ,

} PharosFutureQueueAddR;

typedef enum
{
    /**
     * Success
     */
    PHAROS_FUTURE_QUEUE_REMOVE_SUCCESS = 0 ,
    PHAROS_FUTURE_QUEUE_REMOVE_INVALID = 1 ,
    PHAROS_FUTURE_QUEUE_REMOVE_NOT_HOLDER = 2 ,
    PHAROS_FUTURE_QUEUE_REMOVE_INSIDE_INTERRUPT = 3 ,
    PHAROS_FUTURE_QUEUE_REMOVE_INVALID_ADDRESS = 4 ,
} PharosFutureQueueRemoveR;

typedef enum
{
    /**
     * No permissions to shutdown (on success the shutdown does not return) 
     */
    PHAROS_SHUTDOWN_NO_PERMISSIONS = 1 ,
} PharosShutdownR;

typedef enum
{
    /**
     * No permissions to restart (on success the restart does not return) 
     */
    PHAROS_RESTART_NO_PERMISSIONS = 1 ,
} PharosRestartR;

typedef enum
{
    /**
     * Will restart the partition 
     */
    PHAROS_PARTITION_RESTART_SUCCESS = 0 ,

    /**
     * Trying to restart an invalid partition
     */
    PHAROS_PARTITION_RESTART_INVALID = 1 ,

    /**
     * No sufficient permissions to restart the partition
     */
    PHAROS_PARTITION_RESTART_NOT_ALLOWED = 2 ,

} PharosPartitionRestartR;


/**
 * Outputs a char a debug UART
 * 
 * @param c char to output
 */
void outputChar(char_t c);


/**
 * Compares two priorities
 * 
 * @note this is not a syscall
 * 
 * @constraint assumes the specified addresses of the priorities are valid addresses
 * 
 * @param pa address of first priority
 * @param pb address of second priority
 * 
 * @return returns positive, negative or 0 if a > b, a < b or a == b (respectively) 
 */
intptr_t pharosPriorityComparator(const ptrPharosPriority pa , const ptrPharosPriority pb);


#endif /* DECLARATIONS_H */
