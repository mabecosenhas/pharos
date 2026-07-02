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


#ifndef PHAROS_CORECONF_H
#define PHAROS_CORECONF_H

#include <pharos/kernel/core/declarations.h>
#include <pharos/kernel/thread/declarations.h>
#include <pharos/hal/cpu/conf.h>
#include <pharos/kernel/collection/treemap.h>
#include <pharos/kernel/collection/simplelist.h>
#include <pharos/kernel/time/declarations.h>
#include <pharos/kernel/core/mpsc.h>
#include <pharos/kernel/time/time.h>
#include <pharos/kernel/thread/queue.h>


/**
 * Make sure the queue and all nodes are aligned to a pointer size (many CPUs require the __atomic_exchange_n operation
 * to use aligned addresses
 */
#pragma pack(push , __SIZEOF_POINTER__)

struct PharosThreadCore
{
    PharosMpscNode mpscChannelReceiveNode[PHAROS_NUMBER_CORES];
    PharosMpscNode mpscChannelReceiveReturnNode[PHAROS_NUMBER_CORES];
    PharosMpscNode mpscThreadReadyNode[PHAROS_NUMBER_CORES];
    PharosMpscNode mpscThreadSchedulerPriorityUpdate[PHAROS_NUMBER_CORES];
    PharosMpscNode mpscThreadActivate[PHAROS_NUMBER_CORES];

    PharosRmpMessageChannelReceive channelReceiveMessage;
    PharosRmpMessageChannelReceiveReturn channelReceiveReturnMessage;
    PharosRmpMessageThreadReady threadReadyMessage;
    PharosRmpMessageSchedulerThreadUpdatePriority threadPriorityMessage;
    PharosRmpMessageThreadSuspend threadSuspendMessage;
    PharosRmpMessageThreadResume threadResumeMessage;
    PharosRmpMessageThreadStop threadStopMessage;
    PharosRmpMessageThreadActivate threadActivateMessage;

    /**
     * node of the thread on the wait priority queue when the thread is waiting for a clock tick to be unblocked 
     * (e.g. timeout of a semaphore obtain or a thread sleep)
     * 
     * Used, for example, when a thread in core A is receiving a channel in core B. In core B, the thread could be 
     * blocked on the channel and with a timeout (hence 2 nodes are needed)
     * 
     * @note this could be optimized (in terms of space) to only need 2 nodes (instead of 2*PHAROS_NUMBER_CORES), but 
     * this will decrease speed (need to add/remove the nodes to the cores, creating copies, etc).
     */
    ThreadQueueNode remoteBlockedThreadQueueNode[PHAROS_NUMBER_CORES];
    ThreadQueueNode remoteWaitThreadQueueNode[PHAROS_NUMBER_CORES];

    /**
     * Flag stating it the nodes have already been added (per core)
     */
    bool alreadyAddedThreadQueueNodes[PHAROS_NUMBER_CORES];

    /**
     * Core where the thread is current blocked on 
     * (this is NOT the core in which the thread is running -> the thread always executes on the same core)
     */
    ptrCore runningCore;
};

struct PharosChannelBulkCore
{
    PharosMpscNode mpscSendNode[PHAROS_NUMBER_CORES];
    PharosMpscNode mpscFreeNode[PHAROS_NUMBER_CORES];
    PharosMpscNode mpscRespondNode[PHAROS_NUMBER_CORES];

    PharosRmpMessageChannelSend sendMessage;
    PharosRmpMessageChannelBulkFree freeMessage;
};

struct PharosHwQueueMessageCore
{
    /**
     * Core that sent the message
     */
    CoreNumber sender;

    PharosMpscNode mpscSendNode[PHAROS_NUMBER_CORES];
    PharosMpscNode mpscFreeNode[PHAROS_NUMBER_CORES];

    PharosRmpMessageHwQueueSend sendMessage;
    PharosRmpMessageHwQueueFree freeMessage;
};

struct PharosFutureCore
{
    PharosMpscNode mpscReadyNode[PHAROS_NUMBER_CORES];
    PharosRmpMessageFutureReady readyMessage;
};

struct PharosCoreConfiguration
{
    /**pharosIChannelSend
     * A multi-producer single-consumer queue
     */
    PharosMpscQueue queue;
};

struct PharosPartitionCore
{
    PharosMpscNode mpscPartitionRestartSignal[PHAROS_NUMBER_CORES];
    PharosMpscNode mpscPartitionExternalThreadsStop[PHAROS_NUMBER_CORES];

    PharosRmpMessagePartitionRestartSignal partitionRestartMessageSignal;
    PharosRmpMessagePartitionExternalThreadsStop partitionExternalThreadsStop;
};

/**
 * restore the pack definition
 */
#pragma pack(pop)


/**
 * Make the PharosRmpCore is packed so the assembly code matches
 */
#pragma pack(push , 1)

struct PharosRmpCore
{
    /**
     * Running thread on the core
     */
    ptrThreadInfo runningThread;

    /**
     * Dispatch level on the core
     */
    uintptr_t dispatchLevel;

    /**
     * Interrupt level on the core
     */
    uintptr_t interruptLevel;

    /**
     * Context switch flag on the core
     */
    uintptr_t contextSwitchNeeded;
    
    /**
     * Clock ticks since boot on the core
     */
    ClockTick ticksSinceBoot;
 
    uint8_t padding[24];
    
    /* Make sure that each Core has a size multiple of the cache line to ensure that the structure is not accessed by 
     * a core different from the running core to avoid expensive cache communication (snooping) */
};

/**
 * restore the pack definition
 */
#pragma pack(pop)


/**
 * Table of cores
 */
extern PharosRmpCore pharosVRmpCore[PHAROS_NUMBER_CORES];


/**
 * get the running thread
 * 
 * Protection: none
 * 
 * @return returns the running thread
 */
INLINE ptrThreadInfo pharosISchedGetRunningThread(void);


/**
 * Initialize the multicore
 */
void pharosMulticoreInitialize(CoreNumber core) __attribute__((noreturn));


/**
 * Halt the running core
 */
void pharosCpuCoreHalt(void) __attribute__((noreturn));


/* PHAROS_CORE_H */
#endif 
