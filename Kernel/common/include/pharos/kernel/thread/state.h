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


#ifndef PHAROS_STATE_H
#define PHAROS_STATE_H


#include <pharos/kernel/thread/declarations.h>
#include <pharos/kernel/partition/declarations.h>


#define THREAD_READY_STATE                                          (0U)
#define THREAD_RUNNING_STATE                                        (1U)

#define PHAROS_THREAD_STATE_UNBLOCKED                               (0U)
#define PHAROS_THREAD_STATE_BLOCKED_SEMAPHORE_MUTEX                 (1U)
#define PHAROS_THREAD_STATE_BLOCKED_EVENT                           (2U)
#define PHAROS_THREAD_STATE_BLOCKED_LIGHTWEIGHT_QUEUE               (3U)
#define PHAROS_THREAD_STATE_BLOCKED_HW_QUEUE_RECEIVE                (4U)
#define PHAROS_THREAD_STATE_BLOCKED_HW_QUEUE_SEND                   (5U)
#define PHAROS_THREAD_STATE_BLOCKED_SEMAPHORE_COUNTING              (6U)
#define PHAROS_THREAD_STATE_BLOCKED_CHANNEL                         (7U)
#define PHAROS_THREAD_STATE_BLOCKED_SEMAPHORE_CEILING               (8U)
#define PHAROS_THREAD_STATE_BLOCKED_FUTURE                          (9U)
#define PHAROS_THREAD_STATE_BLOCKED_FUTURE_QUEUE                    (10U)
#define PHAROS_THREAD_STATE_BLOCKED_BULK                            (11U)
#define PHAROS_THREAD_STATE_BLOCKED_CHANNEL_BULK                    (12U)
#define PHAROS_THREAD_STATE_BLOCKED_CHANNEL_RESPONSE           (13U)

#define PHAROS_THREAD_STATE_NOT_WAITING                             (0U) 
#define PHAROS_THREAD_STATE_WAITING_TIMEOUT_SEMAPHORE_MUTEX         (1U)
#define PHAROS_THREAD_STATE_WAITING_TIMEOUT_EVENT                   (2U)
#define PHAROS_THREAD_STATE_WAITING_TIMEOUT_LIGHTWEIGHT_QUEUE       (3U)
#define PHAROS_THREAD_STATE_WAITING_TIMEOUT_HW_QUEUE_RECEIVE        (4U)
#define PHAROS_THREAD_STATE_WAITING_TIMEOUT_HW_QUEUE_SEND           (5U)
#define PHAROS_THREAD_STATE_WAITING_SLEEP                           (6U)
#define PHAROS_THREAD_STATE_WAITING_TIMEOUT_SEMAPHORE_COUNTING      (7U)
#define PHAROS_THREAD_STATE_WAITING_TIMEOUT_SEMAPHORE_CEILING       (8U)
#define PHAROS_THREAD_STATE_WAITING_TIMEOUT_CHANNEL                 (9U)
#define PHAROS_THREAD_STATE_WAITING_TIMEOUT_FUTURE                  (10U)
#define PHAROS_THREAD_STATE_WAITING_TIMEOUT_FUTURE_QUEUE            (11U)
#define PHAROS_THREAD_STATE_WAITING_TIMEOUT_BULK                    (12U)
#define PHAROS_THREAD_STATE_WAITING_TIMEOUT_CHANNEL_BULK            (13U)
#define PHAROS_THREAD_STATE_WAITING_TIMEOUT_CHANNEL_BULK_RESPONSE   (14U)


struct ThreadState
{

    struct
    {
        /******************************************************************************************************/
        /* ready flags                                                                                        */
        /******************************************************************************************************/

        /**
         * blocked state information (e.g. blocked on simple semaphore, on event, etc)
         */
        uint32_t blocked;

        /**
         * wait state information (e.g. waiting for activation, timeout on simple semaphore, sleep, etc)
         */
        uint32_t wait;

        /**
         * flag indicating if the thread is suspended
         */
        uint32_t isSuspended : 1;

        /**
         * flag indicating if the thread is deleted (TRUE for deleted)
         */
        uint32_t isDeleted : 1;

        /**
         * flag indicating if the thread is created (TRUE for created)
         */
        uint32_t isCreated : 1;

        /**
         * flag indicating if the thread was started (TRUE for started)
         */
        uint32_t isStarted : 1;

        /******************************************************************************************************/
        /* other flags                                                                                        */
        /******************************************************************************************************/

        /**
         * flag indicating if the thread is waiting to be stopped (once it can)
         */
        uint32_t isWaitingStop : 1;

        /**
         * flag indicating if the thread is waiting to be stopped (once it can)
         */
        uint32_t isWaitingSuspend : 1;

        /**
         * flag indicating if the thread is waiting for an activation (for periodic and sporadic threads)
         */
        uint32_t isWaitingActivation : 1;

    } flags;

    /**
     * Nest level of inter-partition calls
     */
    uint32_t interPartitionNest;
};

/**
 * Increment the thread inter partition level 
 * 
 * @param thread thread to increment
 */
void pharosIThreadStateInterPartitionInc(ptrThreadInfo thread);


/**
 * Decrement the thread inter partition level. If the thread partition is restarting then signal a restart if
 * no other threads of the same partition are also not inside another partition context.
 * 
 * @param thread thread to increment
 */
void pharosIThreadStateInterPartitionDec(ptrThreadInfo thread);


/**
 * Determine if the thread state is inside another partition 
 * 
 * @param thread thread to check
 * 
 * @return returns TRUE if the threads state is inside another partition and FALSE otherwise
 */
bool pharosIThreadStateIsInterPartition(ptrThreadInfo thread);

/**
 * Determine if the thread state is waiting 
 * 
 * @param state thread state 
 * 
 * @return returns TRUE if the thread state is waiting and FALSE otherwise
 */
bool pharosIThreadIsWaiting(ThreadState state);


/**
 * Determine if the thread is blocked on a future
 * 
 * @param state thread state
 * 
 * @return returns TRUE if the thread state is blocked on a future and FALSE otherwise
 */
bool pharosIThreadStateIsBlockedFuture(ThreadState state);


/**
 * Determine if the thread is blocked on a future queue
 * 
 * @param state thread state
 * 
 * @return returns TRUE if the thread state is blocked on a future queue and FALSE otherwise
 */
bool pharosIThreadStateIsBlockedFutureQueue(ThreadState state);


/**
 * sets the thread is blocked on a mutex
 *
 * @param state state of the thread to change
 */
void pharosIThreadStateBlockMutex(const ptrThreadState state);


/**
 * sets the thread state as deleted
 * 
 * @param state state of the thread to change
 */
void pharosIThreadStateDelete(const ptrThreadState state);


/**
 * sets the thread is blocked on a counting semaphore 
 * 
 * @param state state of the thread
 */
void pharosIThreadStateBlockCount(const ptrThreadState state);


/**
 * sets the thread is blocked on a bulk
 * 
 * @param state state of the thread
 */
void pharosIThreadStateBlockBulk(const ptrThreadState state);


/**
 * sets the thread is blocked on a channel bulk
 * 
 * @param state state of the thread
 */
void pharosIThreadStateBlockChannelBulk(const ptrThreadState state);


/**
 * sets the thread is blocked on a ceiling semaphore 
 * 
 * @param state state of the thread
 */
void pharosIThreadStateBlockCeil(const ptrThreadState state);


/**
 * sets the thread is blocked on an event
 *
 * @param state state of the thread to update
 */
void pharosIThreadStateBlockEvent(const ptrThreadState state);


/**
 * sets the thread is blocked on a future
 *
 * @param state state of the thread to update
 */
void pharosIThreadStateBlockFuture(const ptrThreadState state);


/**
 * sets the thread is blocked on a future queue
 *
 * @param state state of the thread to update
 */
void pharosIThreadStateBlockFutureQueue(const ptrThreadState state);


/**
 * sets the thread state to not suspended (resumed)
 * 
 * @param state state of the thread to update
 */
void pharosIThreadStateSetResume(const ptrThreadState state);


/**
 * sets the thread state to suspended
 * 
 * @param state state of the thread to update
 */
void pharosIThreadStateSetSuspend(const ptrThreadState state);


/**
 * determines if the thread state is suspended 
 * 
 * @param state state to determine
 * 
 * @return returns TRUE if the state is suspended and FALSE otherwise
 */
bool pharosIThreadStateIsSuspended(const ptrThreadState state);


/**
 * sets the thread is blocked on a activation (valid for periodic and sporadic threads)
 *
 * @param state state of the thread
 */
void pharosIThreadStateWaitActivation(const ptrThreadState state);


/**
 * Clears the thread waiting activation
 * 
 * @param state state of the thread
 */
void pharosIThreadStateClearWaitActivation(const ptrThreadState state);


/**
 * sets the thread is blocked on a lightweight queue
 *
 * @param state state of the thread
 */
void pharosIThreadStateBlockLwQueue(const ptrThreadState state);


/**
 * sets the thread is blocked on a heavy weight queue receive
 *
 * @param state state of the thread
 */
void pharosIThreadStateBlockHwQueueReceive(const ptrThreadState state);


/**
 * sets the thread is blocked on a heavy weight queue send
 *
 * @param state state of the thread
 */
void pharosIThreadStateBlockHwQueueSend(const ptrThreadState state);


/**
 * sets the thread is blocked on a channel receive
 *
 * @param state state of the thread
 */
void pharosIThreadStateBlockChannel(const ptrThreadState state);


/**
 * sets the thread is blocked on a channel response
 *
 * @param state state of the thread
 */
void pharosIThreadStateBlockChannelResponse(const ptrThreadState state);


/**
 * Determine if the thread state is ready
 * 
 * @param state thread state
 * @return returns TRUE if the thread state is ready and FALSE otherwise
 */
bool pharosIThreadStateIsReady(const ptrThreadState state);


/**
 * sets the thread is unblocked
 *
 * @param state state of the thread
 */
void pharosIThreadStateUnblock(const ptrThreadState state);


/**
 * sets the thread is waiting on a counting semaphore
 *
 * @param state state of the thread
 */
void pharosIThreadStateWaitCount(const ptrThreadState state);


/**
 * sets the thread is waiting on a bulk
 *
 * @param state state of the thread
 */
void pharosIThreadStateWaitBulk(const ptrThreadState state);


/**
 * sets the thread is waiting to receive a channel bulk
 *
 * @param state state of the thread
 */
void pharosIThreadStateWaitChannelBulk(const ptrThreadState state);


/**
 * sets the thread is waiting on a channel bulk response
 * 
 * @param state state of the thread
 */
void pharosIThreadStateWaitChannelResponse(const ptrThreadState state);


/**
 * sets the thread is waiting on a ceiling semaphore
 *
 * @param state state of the thread
 */
void pharosIThreadStateWaitCeil(const ptrThreadState state);


/**
 * sets the thread is waiting on a mutex
 *
 * @param state state of the thread
 */
void pharosIThreadStateWaitMutex(const ptrThreadState state);


/**
 * sets the thread is waiting on an event
 *
 * @param state state of the thread
 */
void pharosIThreadStateWaitEvent(const ptrThreadState state);


/**
 * sets the thread is waiting on a lw queue
 *
 * @param state state of the thread
 */
void pharosIThreadStateWaitLwQueue(const ptrThreadState state);


/**
 * sets the thread is waiting on a hw queue
 *
 * @param state state of the thread
 */
void pharosIThreadStateWaitHwQueue(const ptrThreadState state);


/**
 * sets the thread is waiting on a channel
 *
 * @param state state of the thread
 */
void pharosIThreadStateWaitChannel(const ptrThreadState state);


/**
 * sets the thread is waiting on a future
 *
 * @param state state of the thread
 */
void pharosIThreadStateWaitFuture(const ptrThreadState state);


/**
 * sets the thread is waiting on a future queue
 *
 * @param state state of the thread
 */
void pharosIThreadStateWaitFutureQueue(const ptrThreadState state);


/**
 * sets the thread is waiting on a sleep operation
 *
 * @param state state of the thread
 */
void pharosIThreadStateWaitSleep(const ptrThreadState state);


/**
 * sets the thread is not waiting on anything
 *
 * @param state state of the thread
 */
void pharosIThreadStateUnwait(const ptrThreadState state);


/**
 * determines if the thread state is waiting for an event
 * 
 * @param state state to determine
 * 
 * @return returns TRUE if the thread state is waiting for an event and FALSE otherwise
 */
bool pharosIThreadStateIsWaitEvent(const ptrThreadState state);


/**
 * determines if the thread state is waiting
 * 
 * @param state state to determine
 * 
 * @return returns TRUE if the thread state is waiting and FALSE otherwise
 */
bool pharosIThreadStateIsWaiting(const ptrThreadState state);


/**
 * determines if the thread state is waiting for an activation (useful for periodic or sporadic threads) 
 * 
 * @param state state to determine
 * 
 * @return returns TRUE if the thread state is waiting for an activation and FALSE otherwise
 */
bool pharosIThreadStateIsWaitAct(const ptrThreadState state);


/**
 * Initialize the thread state
 * 
 * @param state thread state
 */
void pharosIThreadStateInitialize(const ptrThreadState state);


/**
 * Set the thread as been created
 * 
 * @param state state to update
 */
void pharosIThreadStateSetCreated(const ptrThreadState state);


/**
 * Determine if the thread has been created
 * 
 * @param state state to check
 * 
 * @return returns TRUE if the state has been created and FALSE otherwise
 */
bool pharosIThreadStateIsCreated(const ptrThreadState state);


/**
 * Set the thread as been started
 * 
 * @param state state to update
 */
void pharosIThreadStateSetStarted(const ptrThreadState state);


/**
 * Set the thread is stopped (means that it is not started)
 * 
 * @param state state to update
 */
void pharosIThreadStateSetStopped(const ptrThreadState state);


void pharosIThreadStateStopWaitingSet(const ptrThreadState state);
void pharosIThreadStateStopWaitingClear(const ptrThreadState state);
bool pharosIThreadStateIsStopWaiting(const ptrThreadState state);


void pharosIThreadStateSuspendWaitingSet(const ptrThreadState state);
bool pharosIThreadStateIsSuspendWaiting(const ptrThreadState state);
void pharosIThreadStateSuspendWaitingClear(const ptrThreadState state);

/**
 * Determine if the thread has been started
 * 
 * @param state state to check
 * 
 * @return returns TRUE if the state has been started and FALSE otherwise
 */
bool pharosIThreadStateIsStarted(const ptrThreadState state);


/**
 * Clears the thread state 
 * 
 * @param state state of the thread to clear
 */
void pharosIThreadStateClear(const ptrThreadState state);


#endif /* STATE_H */
