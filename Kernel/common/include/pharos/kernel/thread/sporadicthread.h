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


#ifndef PHAROS_SPORADICTHREAD_H
#define PHAROS_SPORADICTHREAD_H

#include <pharos/kernel/thread/declarations.h>
#include <pharos/kernel/clock/clock.h>
#include <pharos/kernel/event/event.h>
#include <pharos/kernel/semaphore/mutex.h>    
#include <pharos/kernel/queue/queue.h>
#include <pharos/kernel/thread/thread.h>
#include <pharos/kernel/channel/declarations.h>
#include <pharos/kernel/thread/rtthread.h>

/**
 * Prototype of a sporadic thread that is activated by an event
 */
typedef void (* SporadicThreadJobEventBody)(void *partitionData , Event event);

/**
 * Prototype of a sporadic thread that is activated by a mutex
 */
typedef void (* SporadicThreadJobMutexBody)(void *partitionData , SemMutexNumber ssId);

/**
 * Prototype of a sporadic thread that is activated by a lightweight queue
 */
typedef void (* SporadicThreadJobLwQueueBody)(void *partitionData , LwQueueNumber number , ptrBulk message ,
                                              uint8_t *buffer , uint32_t size);

/**
 * Prototype of a sporadic thread that is activated by a heavyweight queue
 */
typedef void (* SporadicThreadJobHwQueueBody)(void *partitionData , HwQueueNumber queueId , uint8_t *buffer ,
                                              uint32_t size , PharosPriority priority , ptrFutureId future);

/**
 * Prototype of a sporadic thread that is activated by a channel
 */
typedef void (* SporadicThreadJobChannelBody)(void *partitionData , ptrChannelBulk bulk ,
                                              ChannelId queueId , PharosPriority priority , ptrFutureId future);

/**
 * Type of activation of a sporadic thread (activation means the start of the sporadic MIT timeout)
 */
typedef enum
{
    /**
     * Sporadic thread activated by a mutex
     */
    PHAROS_SPORADIC_THREAD_ACTIVATION_BY_MUTEX = 0 ,

    /**
     * Sporadic thread activated by an event
     */
    PHAROS_SPORADIC_THREAD_ACTIVATION_BY_EVENT = 1 ,

    /**
     * Sporadic thread activated by a lightweight queue
     */
    PHAROS_SPORADIC_THREAD_ACTIVATION_BY_LIGHTWEIGHT_QUEUE = 2 ,

    /**
     * Sporadic thread activated by a heavyweight queue
     */
    PHAROS_SPORADIC_THREAD_ACTIVATION_BY_HEAVYWEIGHT_QUEUE = 3 ,

    /**
     * Sporadic thread activated by a channel
     */
    PHAROS_SPORADIC_THREAD_ACTIVATION_BY_CHANNEL = 4 ,
} SporadicThreadActivationType;

struct PharosSporadicThreadActivation
{
    /**
     * activation type. Is associated with the union type
     */
    SporadicThreadActivationType type;

    /**
     * activation information
     */
    union
    {

        /**
         * simple semaphore activation information
         */
        struct
        {
            /**
             * sporadic thread awakes for a simple semaphore release
             * 
             * @note application must initialize this field (if type is PHAROS_SPORADIC_THREAD_ACTIVATION_BY_MUTEX)
             */
            SemMutexNumber mutexNumber;

            /**
             * body of thread to execute in case of activation by simple semaphore
             * 
             * @note application must initialize this field (if type is PHAROS_SPORADIC_THREAD_ACTIVATION_BY_MUTEX)
             */
            SporadicThreadJobMutexBody mutexBody;
        } mutex;

        /**
         *  body of the thread to execute in case of activation by event
         * 
         * @note application must initialize this field (if type is PHAROS_SPORADIC_THREAD_ACTIVATION_BY_EVENT)
         */
        SporadicThreadJobEventBody eventBody;

        /**
         * lightweight queue activation information
         */
        struct
        {
            /**
             * sporadic thread awakes for a lightweight queue message
             * 
             * @note application must initialize this field (if type is PHAROS_SPORADIC_THREAD_ACTIVATION_BY_LIGHTWEIGHT_QUEUE)
             */
            LwQueueNumber lwQueueId;

            /**
             * entry point
             * 
             * @note application must initialize this field (if type is PHAROS_SPORADIC_THREAD_ACTIVATION_BY_LIGHTWEIGHT_QUEUE)
             */
            SporadicThreadJobLwQueueBody lwBody;
        } lwQueue;

        /**
         * heavy weight queue activation information
         */
        struct
        {
            /**
             * Heavy weight queue message where the thread will wait on
             * 
             * @note application must initialize this field (if type is PHAROS_SPORADIC_THREAD_ACTIVATION_BY_HEAVYWEIGHT_QUEUE)
             */
            HwQueueNumber hwQueueNumber;

            /**
             * entry point
             * 
             * @note application must initialize this field (if type is PHAROS_SPORADIC_THREAD_ACTIVATION_BY_HEAVYWEIGHT_QUEUE)
             */
            SporadicThreadJobHwQueueBody hwBody;

            /**
             * Must point to a buffer whose size must be the size of a message on queue
             * 
             * @note application must initialize this field (if type is PHAROS_SPORADIC_THREAD_ACTIVATION_BY_HEAVYWEIGHT_QUEUE)
             */
            uint8_t *buffer;
        } hwQueue;

        /**
         * channel activation information
         */
        struct
        {
            /**
             * Channel where the sporadic threads waits on (even if in another partition)
             * 
             * @note application must initialize this field (if type is PHAROS_SPORADIC_THREAD_ACTIVATION_BY_CHANNEL)
             */
            ChannelId channelId;

            /**
             * Entry point
             * 
             * @note application must initialize this field (if type is PHAROS_SPORADIC_THREAD_ACTIVATION_BY_CHANNEL)
             */
            SporadicThreadJobChannelBody channelBody;
        } channel;

    } entry;
};

struct PharosThreadSporadicProperties
{
    PharosThreadProperties basic;

    ClockTick deadline;

    PharosSporadicThreadActivation activation;
};

/**
 * Sporadic thread structure
 */
struct SporadicThread
{
    /**
     * real-time thread 
     */
    RtThread rtThread;

    /**
     * Activation of the sporadic thread
     * 
     * @note application must initialize this structure to indicate how the sporadic thread is activated 
     * (e.g. by a mutex, lw message queue, etc)
     */
    PharosSporadicThreadActivation activation;
};


/**
 * Initializes a sporadic thread
 * 
 * @param thread thread to initialize
 * @param env environment of the thread 
 * @param number thread number inside the environment
 */
void pharosISporadicThreadInitialize(ptrSporadicThread thread , ptrPartition partition , ThreadNumber number , bool restart);


/**
 * Activates the sporadic thread by checking if it is in the waiting state. 
 * If it is then updates the last release instant.
 * Also activates the deadline handler.
 * 
 * @note the thread does not need to be a sporadic thread. This method checks if the thread is sporadic and only does
 * anything if it is.
 * 
 * @param thread thread to activate
 */
void pharosIThreadSporadicActivate(ptrThreadInfo thread);


/**
 * Announce that a sporadic job has ended
 * 
 * @param address that triggered the syscall. If not correct, this function will not do anything.
 */
CpuModeChangeContext pharosISporadicThreadEnded(const uint8_t * returnAddress , CpuModeChangeContext context);


/**
 * Create an sporadic thread 
 * 
 * @param properties sporadic thread properties
 * 
 * @return returns success or the corresponding error
 */
PharosThreadSporadicCreateR pharosSThreadSporadicCreate(const ptrPharosThreadSporadicProperties properties , ptrThreadId id);


#endif /* SPORADICTHREAD_H */
