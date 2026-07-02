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


#ifndef PHAROS_KERNEL_RMP_MPSC_H
#define PHAROS_KERNEL_RMP_MPSC_H

#include <pharos/hal/hal.h>
#include <pharos/kernel/queue/declarations.h>
#include <pharos/kernel/future/declarations.h>
#include <pharos/kernel/channel/declarations.h>
#include <pharos/kernel/collection/simplelist.h>
#include <pharos/kernel/thread/declarations.h>
#include <pharos/kernel/buffer/declarations.h>
#include <pharos/kernel/time/declarations.h>
#include <pharos/kernel/thread/priority.h>
#include <pharos/kernel/channel/channelbulk.h>


typedef struct PharosMpscNode PharosMpscNode , *ptrPharosMpscNode;

typedef struct PharosMpscQueue PharosMpscQueue , *ptrPharosMpscQueue;

typedef struct PharosRmpMessageHwQueueSend PharosRmpMessageHwQueueSend , *ptrPharosRmpMessageHwQueueSend;

typedef struct PharosRmpMessageFutureReady PharosRmpMessageFutureReady , *ptrPharosRmpMessageFutureReady;

typedef struct PharosRmpMessageHwQueueFree PharosRmpMessageHwQueueFree , *ptrPharosRmpMessageHwQueueFree;

typedef struct PharosRmpMessageChannelSend PharosRmpMessageChannelSend , *ptrPharosRmpMessageChannelSend;

typedef struct PharosRmpMessageChannelReceive PharosRmpMessageChannelReceive , *ptrPharosRmpMessageChannelReceive;

typedef struct PharosRmpMessageChannelReceiveReturn PharosRmpMessageChannelReceiveReturn , *ptrPharosRmpMessageChannelReceiveReturn;

typedef struct PharosRmpMessageChannelBulkFree PharosRmpMessageChannelBulkFree , *ptrPharosRmpMessageChannelBulkFree;

typedef struct PharosRmpMessagePartitionRestartSignal PharosRmpMessagePartitionRestartSignal , *ptrPharosRmpMessagePartitionRestartSignal;

typedef struct PharosRmpMessagePartitionExternalThreadsStop PharosRmpMessagePartitionExternalThreadsStop , *ptrPharosRmpMessagePartitionExternalThreadsStop;

typedef struct PharosRmpMessageThreadReady PharosRmpMessageThreadReady , *ptrPharosRmpMessageThreadReady;

typedef struct PharosRmpMessageSchedulerThreadUpdatePriority PharosRmpMessageSchedulerThreadUpdatePriority , *ptrPharosRmpMessageSchedulerThreadUpdatePriority;

typedef struct PharosRmpMessageThreadStop PharosRmpMessageThreadStop , *ptrPharosRmpMessageThreadStop;

typedef struct PharosRmpMessageThreadSuspend PharosRmpMessageThreadSuspend , *ptrPharosRmpMessageThreadSuspend;

typedef struct PharosRmpMessageThreadResume PharosRmpMessageThreadResume , *ptrPharosRmpMessageThreadResume;

typedef struct PharosRmpMessageThreadActivate PharosRmpMessageThreadActivate , *ptrPharosRmpMessageThreadActivate;



typedef union PharosMpscMessage PharosMpscMessage , *ptrPharosMpscMessage;

struct PharosRmpMessageHwQueueSend
{
    ptrHeavyWeightQueue queue;
    ptrHeavyWeightQueueMessage message;
    PharosPriority priority;
};

struct PharosRmpMessageFutureReady
{
    ptrFuture future;
    FutureArg arg;
    bool isReset;
};

struct PharosRmpMessageHwQueueFree
{
    ptrHeavyWeightQueue queue;
    ptrHeavyWeightQueueMessage message;
};

struct PharosRmpMessageChannelSend
{
    ptrChannel channel;
    ptrChannelBulkKernel bulk;
};

struct PharosRmpMessageChannelReceive
{
    /**
     * Channel to receive from
     */
    ptrChannel channel;

    /**
     * Thread that wants to receive from the channel
     */
    ptrThreadInfo thread;

    /**
     * Timeout
     */
    Timeout timeout;

    /**
     * Address of the priority argument
     */
    ptrPharosPriority priority;
};

struct PharosRmpMessageChannelReceiveReturn
{
    /**
     * bulk received
     */
    ptrChannelBulkKernel bulk;

    /**
     * Thread that received the bulk
     */
    ptrThreadInfo thread;

    PharosChannelReceiveR returnValue;
};

struct PharosRmpMessageChannelBulkFree
{
    ptrChannelBulkKernel bulk;
};

struct PharosRmpMessageChannelBulkRespond
{
    ptrChannelBulk response;
};

struct PharosRmpMessageChannelBulkResponseFree
{
    ptrChannelBulkKernel response;
};

struct PharosRmpMessagePartitionRestartSignal
{
    ptrPartition partition;
};

struct PharosRmpMessagePartitionExternalThreadsStop
{
    ptrPartition partition;
};

struct PharosRmpMessageThreadReady
{
    ptrThreadInfo thread;
};

struct PharosRmpMessageSchedulerThreadUpdatePriority
{
    ptrThreadInfo thread;
    ThreadPriority newPriority;
};

struct PharosRmpMessageThreadStop
{
    ptrThreadInfo thread;
};

struct PharosRmpMessageThreadActivate
{
    ptrThreadInfo thread;
};

struct PharosRmpMessageThreadSuspend
{
    ptrThreadInfo thread;
};

struct PharosRmpMessageThreadResume
{
    ptrThreadInfo thread;
};

typedef enum
{
    PHAROS_RMP_MESSAGE_TYPE_HWQUEUE_SEND = 0U ,
    PHAROS_RMP_MESSAGE_TYPE_HWQUEUE_FREE = 1U ,
    PHAROS_RMP_MESSAGE_TYPE_FUTURE_READY = 2U ,
    PHAROS_RMP_MESSAGE_TYPE_CHANNEL_SEND = 3U ,
    PHAROS_RMP_MESSAGE_TYPE_CHANNEL_RECEIVE = 4U ,
    PHAROS_RMP_MESSAGE_TYPE_CHANNEL_RECEIVE_RETURN = 5U ,
    PHAROS_RMP_MESSAGE_TYPE_CHANNEL_BULK_FREE = 6U ,
    PHAROS_RMP_MESSAGE_TYPE_CHANNEL_BULK_RESPOND = 7U ,
    PHAROS_RMP_MESSAGE_TYPE_PARTITION_RESTART_SIGNAL = 8U ,
    PHAROS_RMP_MESSAGE_TYPE_PARTITION_EXTERNAL_THREADS_STOP = 9U ,
    PHAROS_RMP_MESSAGE_TYPE_SCHEDULER_THREAD_READY = 10U ,
    PHAROS_RMP_MESSAGE_TYPE_SCHEDULER_THREAD_UPDATE_PRIORITY = 11U ,
    PHAROS_RMP_MESSAGE_TYPE_THREAD_SUSPEND = 12U ,
    PHAROS_RMP_MESSAGE_TYPE_THREAD_RESUME = 13U ,
    PHAROS_RMP_MESSAGE_TYPE_THREAD_STOP = 14U ,
    PHAROS_RMP_MESSAGE_TYPE_THREAD_ACTIVATE = 15U ,
} PharosMulticoreMessageType;

/**
 * Make sure the queue and all nodes are aligned to a pointer size (many CPUs require the __atomic_exchange_n operation
 * to use aligned addresses
 */
#pragma pack(push , __SIZEOF_POINTER__)

union PharosMpscMessage
{
    ptrPharosRmpMessageHwQueueSend hwqueuesend;
    ptrPharosRmpMessageFutureReady futureReady;
    ptrPharosRmpMessageHwQueueFree hwqueueFree;
    ptrPharosRmpMessageChannelSend channelSend;
    ptrPharosRmpMessageChannelReceive channelReceive;
    ptrPharosRmpMessageChannelReceiveReturn channelReceiveReturn;
    ptrPharosRmpMessageChannelBulkFree channelFree;
    ptrPharosRmpMessagePartitionRestartSignal partitionRestartSignal;
    ptrPharosRmpMessagePartitionExternalThreadsStop partitionExternalThreadsStop;
    ptrPharosRmpMessageThreadReady threadReady;
    ptrPharosRmpMessageSchedulerThreadUpdatePriority threadPriority;
    ptrPharosRmpMessageThreadStop threadStop;
    ptrPharosRmpMessageThreadSuspend threadSuspend;
    ptrPharosRmpMessageThreadResume threadResume;
    ptrPharosRmpMessageThreadActivate threadActivate;
};

struct PharosMpscNode
{
    /**
     * Pointer to the next node on the queue
     */
    ptrPharosMpscNode next;

    /**
     * Type of message that the node contains
     */
    PharosMulticoreMessageType type;

    /**
     * Message that the node contains (actually is a pointer)
     */
    PharosMpscMessage message;
};

struct PharosMpscQueue
{
    /**
     * Head of the queue where the new queued items are placed
     */
    volatile ptrPharosMpscNode head;

    /**
     * Tail of the queue where the popped items are got from
     */
    ptrPharosMpscNode tail;

    /**
     * Empty node on the queue with nothing (diminishes the complexity of the implementation)
     */
    PharosMpscNode stub;
};


/**
 * restore the pack definition
 */
#pragma pack(pop)


/**
 * Initialize the MPSC (Multi-Producer Single-Consumer) queue
 * 
 * @param queue queue to initialize
 */
void pharosIMpscQueueInitialize(ptrPharosMpscQueue queue);


/**
 * Push an item to the MPSC queue 
 * 
 * @param queue MPSC queue
 * @param node node to push
 */
void pharosIMpscQueuePush(ptrPharosMpscQueue queue , ptrPharosMpscNode node);


/**
 * Pop a node from the MPSC queue
 * 
 * @param queue MPSC queue
 * 
 * @return returns the popped or NULL if empty
 */
ptrPharosMpscNode pharosIMpscQueuePop(ptrPharosMpscQueue queue);


/**
 * Get the correct node to place in the MPSC queue associated to the original node.
 * 
 * @param queue MPSC queue
 * @param node original node 
 * 
 * @return returns the pointer to the node 
 */
ptrPharosMpscNode pharosIMpscQueueGetMessage(ptrPharosMpscQueue queue , ptrPharosMpscNode node);


/* PHAROS_KERNEL_RMP_MPSC_H */
#endif 
