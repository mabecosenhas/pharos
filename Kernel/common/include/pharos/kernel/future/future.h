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


#ifndef PHAROS_KERNEL_FUTURE_FUTURE_H
#define PHAROS_KERNEL_FUTURE_FUTURE_H

#include <pharos/declarations.h>
#include <pharos/kernel/future/declarations.h>
#include <pharos/kernel/thread/declarations.h>
#include <pharos/kernel/collection/simplelist.h>
#include <pharos/kernel/time/declarations.h>
#include <pharos/kernel/collection/linkedlist.h>
#include <pharos/kernel/partition/declarations.h>
#include <pharos/kernel/core/coreconf.h>
#include <pharos/kernel/channel/channel.h>

struct FutureId
{
    /**
     * Partition restart number. Useful for futures to see if they have the correct version.
     * If not, that means that the partition has since restarted and the future is no longer valid
     */
    PartitionRestartNumber partitionRestartNumber;

    /**
     * Index of the future inside the partition 
     */
    uint32_t number;

    /**
     * Partition where the future belongs to 
     */
    PartitionNumber partition;
};

struct Future
{
    /**
     * node on the partition free Future list
     */
    SimpleListNode next;

    /**
     * thread future ready queue or the thread future waiting queue  
     */
    LinkedListNode queueNode;

    /**
     * node on the partition list of external futures
     */
    LinkedListNode partitionNode;

    /**
     * Thread blocked on the future (or NULL if none)
     */
    ptrThreadInfo blockedThread;

    /**
     * Argument when the future is ready
     * Must be volatile to make sure the argument is written first than the isReady flag
     */
    volatile FutureArg argument;

    /**
     * index on the future table
     */
    FutureId index;

    /**
     * Partition that owns the future (needed in multi-core)
     */
    ptrPartition owner;

    /**
     * Additional information for the core (multi-core or single-core)
     */
    PharosFutureCore rmp;

    /**
     * Bulk associated with the future
     */
    ChannelBulk bulk;

    struct
    {
        /**
         * Flag indicating if it is ready.
         * Must be volatile to make sure the argument is written first than the isReady flag
         */
        volatile uintptr_t isReady;

        /**
         * Auxiliary flag that indicates that a thread is making the future ready and therefore a second ready function
         * will return error
         */
        volatile uintptr_t makingReady;

        /**
         * Flag indicating if it is on a thread queue.
         */
        volatile uintptr_t onQueue;

        /**
         * Flag indicating that the future is free
         */
        volatile uintptr_t isFree;

        /**
         * Flag indicating that the future has a different holder than the future->owner
         */
        volatile uintptr_t hasDifferentHolder;
    } flags;

    /**
     * Temporary partition that can have access to the future (used in case of hwqueue or channel)
     */
    ptrPartition tempPartition;
};


/**
 * Get the future with the specified number on the running partition
 * 
 * @param futureId future id to get
 * 
 * @return returns NULL if the number is invalid or its address otherwise
 */
ptrFuture pharosIFutureGet(ptrFutureId futureId);


/**
 * Get the future on the running partition
 * 
 * @param futureId future id to get
 * 
 * @return returns NULL if the number is invalid or its address otherwise
 */
ptrFuture pharosIFutureGetLocal(ptrFutureId futureId);


/**
 * Get the future on the running core
 * 
 * @param futureId future id to get
 * 
 * @return returns NULL if the number is invalid or its address otherwise
 */
ptrFuture pharosIFutureGetGlobal(ptrFutureId number);


/**
 * get a future from the partition future queue 
 * 
 * @param futureId pointer to the location of the future number
 * 
 * @return returns success or the error (e.g. invalid future number)
 */
PharosFutureGetR pharosSFutureGet(ptrFutureId futureId);


/**
 * frees a future so that it can be used again
 * 
 * @param futureId number of the future to free
 * 
 * @return returns success or the error (e.g. invalid future number)
 */
PharosFutureFreeR pharosSFutureFree(ptrFutureId futureId , ptrChannelBulk bulk);


/**
 * Waits for a future with a timeout
 * 
 * @param futureId number of the future to wait for
 * @param arg address of future argument that will be filled with the argument if the future is made ready
 * @param timeout timeout information
 * @return returns success or the error (e.g. invalid future number)
 */
PharosFutureWaitR pharosSFutureWait(ptrFutureId futureId , ptrFutureArg arg , ptrChannelBulk bulk , Timeout timeout);


/**
 * "Unblocks" a future by making it ready. A thread blocked on the future will be unblocked.
 * 
 * @param futureId number of the future to make ready
 * @param arg address of the future argument that will be copied into the thread blocked on the future (pharosFutureWait)
 *  
 * @return returns success or the error (e.g. invalid future number)
 */
PharosFutureReadyR pharosSFutureReady(ptrFutureId futureId , ptrFutureArg arg , ptrChannelBulk bulk);


/**
 * Determines if the specified partition holds the future 
 * 
 * @constraint assumes that future and partition are valid pointers 
 * 
 * @param future future to determine the holder 
 * @param partition partition to check
 * 
 * @return returns TRUE if the partition holds the future and FALSE otherwise
 */
bool pharosIFutureIsHolder(ptrFuture future , ptrPartition partition);


/**
 * Remove access to the to the future 
 * 
 * @param future future to remove access
 */
void pharosIFutureRemoveAccess(ptrFuture future);


/**
 * Updates the future holder
 * 
 * @constraint assumes that future and newHolder are valid pointers 
 * @note does not assume that the running core must be the newHolder core (or the future core)
 * 
 * @param future future to update
 * @param newHolder new partition holder
 */
void pharosIFutureUpdateHolder(ptrFuture future , ptrPartition newHolder);


/**
 * Make the future ready
 * 
 * @param future future to ready
 * @param arg argument to the future
 */
void pharosIFutureReady(ptrFuture future , ptrFutureArg arg);


/**
 * Make the future ready on the same running core 
 * 
 * @constraint must execute this function in the future owner core 
 * 
 * @param future future to ready
 * @param arg argument to the future
 */
void pharosIFutureReadyOnOwnerCore(ptrFuture future , ptrFutureArg arg);


/**
 * Reset a future
 * 
 * @param future future to reset
 */
INLINE void pharosIFutureReset(ptrFuture future);


/**
 * Free a future (assumes that all pre-validation have been made)
 * 
 * @param f future to free
 * 
 * @return returns PHAROS_FUTURE_FREE_IN_USE or PHAROS_FUTURE_FREE_SUCCESS
 */
PharosFutureFreeR pharosIFutureFree(ptrFuture f);


/**
 * Copy the argument and bulk of the future to the specified arguments 
 * 
 * @constraint must run on the receiver core so it can add MMU permissions to it 
 * 
 * @param future future from where to get the argument and bulk from
 * @param arg place where to write the argument 
 * @param bulk address where to write the bulk (if not NULL and the future has a valid bulk)
 * 
 * @return returns success or the invalid MMU pages
 */
PharosFutureWaitR pharosIFutureCopyResult(ptrPartition receiver , ptrFuture future , ptrFutureArg arg , ptrChannelBulk bulk);


/**
 * Determines if a specified future id is valid or not
 * 
 * @contraint id must be a pointer to a valid FutureId address
 * 
 * @param id the future id
 * 
 * @return returns TRUE if the FutureId is valid and FALSE otherwise
 */
INLINE bool pharosIFutureIdIsValid(ptrFutureId id);


#endif /* PHAROS_KERNEL_FUTURE_FUTURE_H */
