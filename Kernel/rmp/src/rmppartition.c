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


#include <pharos/kernel/scheduler/scheduler.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/kernel/partition/partition.h>


void pharosIPartitionRestartSignal(ptrPartition partition)
{
    /* get the running core */
    CoreNumber runningCore = pharosICoreRunning();

    /* get the thread core */
    CoreNumber partitionCore = partition->core->number;

    /* node to send the message to the other core with */
    ptrPharosMpscNode node;

    /* queue on the other core to send */
    ptrPharosMpscQueue mpscQueue;

    /* message to send */
    ptrPharosRmpMessagePartitionRestartSignal messageMpsc;


    /* if running on the same core */
    if(runningCore == partitionCore)
    {
        /* restart the local partition */
        pharosIPartitionRestartSignalOnPartitionCore(partition);
    }
        /* else, must be running on a different core */
    else
    {
        /* message to send to the other core */
        node = &partition->rmp.mpscPartitionRestartSignal[partitionCore];

        /* get the MPSC queue to send to on the channel owner core */
        mpscQueue = &pharosICoreGet(partitionCore)->rmp.queue;

        /* get the correct node to use to send the message */
        node = pharosIMpscQueueGetMessage(mpscQueue , node);

        /* get the message */
        messageMpsc = &partition->rmp.partitionRestartMessageSignal;

        /* set the message */
        node->message.partitionRestartSignal = messageMpsc;

        /* create the multi-core message */
        node->type = PHAROS_RMP_MESSAGE_TYPE_PARTITION_RESTART_SIGNAL;

        messageMpsc->partition = partition;

        /* make sure that every memory store is done before sending the message */
        __atomic_thread_fence(__ATOMIC_RELEASE);

        /* push the message to the MPSC queue */
        pharosIMpscQueuePush(mpscQueue , node);

        /* make sure that the message has been placed in the queue before triggering the interrupt */
        __atomic_thread_fence(__ATOMIC_RELEASE);

        /* send an IPI call to the core queue owner */
        pharosCpuMulticoreTriggerInterrupt(partitionCore);
    }
}


void pharosIPartitionExternalThreadsStop(ptrPartition partition)
{
    /* get the running core */
    CoreNumber runningCore = pharosICoreRunning();

    /* get the thread core */
    CoreNumber partitionCore = partition->core->number;

    /* node to send the message to the other core with */
    ptrPharosMpscNode node;

    /* queue on the other core to send */
    ptrPharosMpscQueue mpscQueue;

    /* message to send */
    ptrPharosRmpMessagePartitionExternalThreadsStop messageMpsc;


    /* if running on the same core */
    if(runningCore == partitionCore)
    {
        /* stop external threads on the running core */
        pharosIPartitionExternalThreadsStopOnPartitionCore(runningCore , partition);
    }
        /* else, must be running on a different core */
    else
    {
        /* message to send to the other core */
        node = &partition->rmp.mpscPartitionExternalThreadsStop[partitionCore];

        /* get the MPSC queue to send to on the channel owner core */
        mpscQueue = &pharosICoreGet(partitionCore)->rmp.queue;

        /* get the correct node to use to send the message */
        node = pharosIMpscQueueGetMessage(mpscQueue , node);

        /* get the message */
        messageMpsc = &partition->rmp.partitionExternalThreadsStop;

        /* set the message */
        node->message.partitionExternalThreadsStop = messageMpsc;

        /* create the multi-core message */
        node->type = PHAROS_RMP_MESSAGE_TYPE_PARTITION_EXTERNAL_THREADS_STOP;

        messageMpsc->partition = partition;

        /* make sure that every memory store is done before sending the message */
        __atomic_thread_fence(__ATOMIC_RELEASE);

        /* push the message to the MPSC queue */
        pharosIMpscQueuePush(mpscQueue , node);

        /* make sure that the message has been placed in the queue before triggering the interrupt */
        __atomic_thread_fence(__ATOMIC_RELEASE);

        /* send an IPI call to the core queue owner */
        pharosCpuMulticoreTriggerInterrupt(partitionCore);
    }
}
