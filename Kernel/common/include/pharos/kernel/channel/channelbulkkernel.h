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


#ifndef PHAROS_KERNEL_CHANNEL_CHANNELBULKKERNEL_H
#define PHAROS_KERNEL_CHANNEL_CHANNELBULKKERNEL_H

#include <pharos/kernel/channel/declarations.h>
#include <pharos/channel.h>
#include <pharos/kernel/collection/treemap.h>
#include <pharos/kernel/buffer/buffer.h>
#include <pharos/kernel/channel/channel.h>
#include <pharos/kernel/collection/simplelist.h>
#include <pharos/kernel/partition/declarations.h>
#include <pharos/hal/cpu/cpu.h>
#include <pharos/kernel/future/declarations.h>
#include <pharos/kernel/core/coreconf.h>
#include <pharos/kernel/thread/queue.h>

struct ChannelBulkKernel
{
    /**
     * tree node on the map of bulks sent (sorted by priority)
     */
    TreeNode treeNode;

    /**
     * Pointer to the next bulk with the same priority on the same channel
     */
    ptrChannelBulkKernel nextPriority;

    /**
     * Pointer to the last bulk with the same priority on the same channel
     */
    ptrChannelBulkKernel lastPriority;

    /**
     * node on the list of free bulks 
     */
    SimpleListNode next;

    /**
     * message of the bulk.
     * The start address of the message might not be the same as the page and the number of bytes
     * transmitted might be smaller than the full page size
     */
    Buffer bufferMessage;

    /**
     * buffer containing the extra message and message size
     */
    Buffer bufferExtra;

    /**
     * full buffer of the bulk. Should not be changed after being initialized
     */
    Buffer bulkFullBuffer;

    /**
     * Thread waiting for a response
     */
    ptrThreadInfo responseWaiter;

    /**
     * Channel bulk queue where this bulk belongs to
     */
    ptrChannelBulkQueue queue;

    /**
     * Current holder of the bulk
     */
    ptrPartition holder;

    /**
     * Partition that sent the bulk
     */
    ptrPartition sender;

    /**
     * Additional information that maybe required for the CPU
     * For example, CPUs with MPU need to know which bulks belong to a partition
     */
    PharosCpuChannelBulkInfo cpuInfo;

    /**
     * Additional information for the core (multi-core or single-core)
     */
    PharosChannelBulkCore rmp;

    /**
     * Associated future to the channel bulk (NULL if none)
     */
    ptrFuture future;

    /**
     * Node on the partition externalChannelBulksHold
     */
    LinkedListNode node;

    /**
     * Partition restart number. Useful for channel bulks and futures to see if they have the correct version.
     * If not, that means that the partition has since restarted.
     */
    PartitionRestartNumber restartNumber;

    /**
     * Priority with which the bulk was sent
     */
    PharosPriority priority;

    struct
    {
        uint32_t isResponse : 1;
        uint32_t responseIsReady : 1;

        /**
         * Bulk is a response and the partition to where the bulk was sent to is restarting
         */
        uint32_t partitionRestart : 1;
    } flags;
};

/**
 * Channel Bulk Queue where the application can get bulks and send them to channels.
 * 
 * Example of a possible configuration of 1000 pages of 4KiB each and the partition can send these bulks to 
 * 3 receivers
 * 
 * declare 1000 pages of 4KiB each. Must declare on this section to be properly aligned to 4K.
 * PAGE_SECTION_4K PharosPage4K pages4K[1000];
 * 
 * declare 1000 bulks (one for each page). Notice that the bulks must be on a different on their own section
 * BULK_SECTION ChannelBulk bulks[1000];
 * 
 * declare the additional MMU pages required 
 * KERNEL_SECTION PageTable4K tables4K[PHAROS_NUMBER_PAGE_TABLE_4K(3 , 1000)];
 * 
 * initialize the ChannelBulkQueue with the CPU specific MMU properties. Check the User Manual for more information.
 */
struct ChannelBulkQueue
{
    /**
     * Start of the buffer of the bulk queue
     */
    uint8_t *start;

    /**
     * queue of channel bulks
     * 
     * Note: application must initialize this! The array of the channel bulks should be inside the KERNEL_SECTION
     */
    ptrChannelBulkKernel bulks;

    /**
     * number of channel bulks inside the queue
     * 
     * Note: application must initialize this!
     */
    uint32_t numberBulks;

    /**
     * Additional information for the bulk queue based on the CPU.
     * For CPUs with MMU some information is required, whereas CPUs with MPU require other type of information
     */
    PharosCpuChannelBulkQueueInf cpuInfo;

    /**
     * list of free bulks
     */
    SimpleList freeBulks;

    /**
     * Partition where the queue belongs to
     */
    ptrPartition owner;

    /**
     * queue of threads blocked waiting for a bulk on this queue
     */
    ThreadQueue blockedQueue;

    struct
    {
        /**
         * Flag indicating if the queue is by priority (FALSE) or FIFO order (TRUE)
         * 
         * Note: initialized by the application
         */
        uint32_t isFifo : 1;
    } flags;
};



/**
 * Syscall for pharosChannelBulkQueueGet
 * 
 * @param number[in] number of the channel bulk
 * @param bulk[out] address where to place the bulk
 * @param buffer[in/out] address of a buffer to initialize with a pointer to the memory location of the buffer where 
 * the calling thread can write to
 * @param timeout maximum time while getting a bulk
 * 
 * @return returns success or error in getting a bulk
 */
PharosChannelBulkQueueGetR pharosSChannelBulkQueueGet(ChannelBulkQueueNumber number , ptrChannelBulk bulk , Timeout timeout);


/**
 * Syscall for pharosChannelBulkFree
 * 
 * @param bulk bulk MUST be the same as returned by the pharosChannelReceiveLocal method. Otherwise will return an error
 * 
 * @return returns success or error in freeing he bulk
 */
PharosChannelBulkFreeR pharosSChannelBulkFree(ptrChannelBulk number);


/**
 * Adds the bulk and the associated future to the partition (bulk could be NULL, in which case this function does 
 * nothing)
 * 
 * @constraint bulk must be valid
 * @constraint this method must be invoked with interrupts disabled
 * @constraint must run on the core of the partition 
 * 
 * @param partition partition where to add the bulk and future
 * @param bulk external bulk to add to the partition
 */
void pharosIChannelBulkAdd2PartitionOnPartitionCore(ptrPartition partition , ptrChannelBulkKernel bulk);


/**
 * Remove the bulk and future (if needed) 
 * 
 * @constraint bulk must be valid
 * @constraint this method must be invoked with interrupts disabled
 * @constraint this method must be invoked with on the bulk holder core (and the bulk must have a holder)
 * 
 * @param bulk bulk to remove
 * @param runningPartition running partition to remove the external bulk and future from
 */
void pharosIChannelBulkRemoveFromHolderOnPartitionCore(ptrChannelBulkKernel bulk);


#endif /* PHAROS_KERNEL_CHANNEL_CHANNELBULKKERNEL_H */
