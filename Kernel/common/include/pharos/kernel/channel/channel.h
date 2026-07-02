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


#ifndef PHAROS_CHANNEL_H
#define PHAROS_CHANNEL_H


#include <pharos/kernel/channel/declarations.h>
#include <pharos/channel.h>
#include <pharos/kernel/collection/treemap.h>
#include <pharos/kernel/partition/declarations.h>
#include <pharos/kernel/time/declarations.h>
#include <pharos/kernel/thread/queue.h>
#include <pharos/kernel/buffer/declarations.h>
#include <pharos/kernel/future/declarations.h>

struct Channel
{
    /**
     * Filter method when sending a bulk
     * 
     * Note: initialized by the application! Place NULL if every sender is allowed to send messages
     */
    ChannelFilterSendMethod sendFilter;

    /**
     * Filter method when receiving a bulk
     * 
     * Note: initialized by the application! Place NULL if every sender is allowed to send messages
     */
    ChannelFilterReceiveMethod receiveFilter;

    /**
     * Queue of threads waiting to receive a channel bulk
     */
    ThreadQueue blockedThreads;

    /**
     * Messages (bulks) 
     * Map<Priority , List<ChannelBulk>>
     */
    TreeMap messages;

    /**
     * owner of the channel
     */
    ptrPartition owner;

    /**
     * number of the channel
     */
    ChannelId id;

    /**
     * If the channel is configured with a minimum priority (isWaitWithMinimumPriority = TRUE) the this field
     * contains the minimum priority of the thread that is blocked on the channel
     */
    PharosPriority minimumPriority;

    struct
    {
        /**
         * Flag indicating that the channel has been initialized
         * 
         * Note: initialized by PHAROS
         */
        uint32_t isInitialized : 1;

        /**
         * Flag indicating if the queue is by priority (FALSE) or FIFO order (TRUE)
         * 
         * Note: initialized by the application
         */
        uint32_t isFifo : 1;

        /**
         * Flag indicating that the channel will only allow one thread to receive from the channel and that the 
         * priority of the pharosChannelReceive/pharosChannelReceiveLocal is interpreted as the minimum priority
         * 
         * Note: initialized by the application
         */
        uint32_t isWaitWithMinimumPriority : 1;
    } flags;
};


/**
 * Syscall for pharosChannelReceiveLocal
 * Receives a bulk through a channel
 * 
 * @param channel channel to receive the bulk from
 * @param bulk pointer where the bulk will be placed in. The application can use this to resend the bulk to another channel
 * @param buffer pointer to the buffer where the message will be copied into (just the initialization of the pointer and size)
 * @param priority priority of the message received
 * @param future future that the receiver can make ready with a specified argument to that the sender has a confirmation and result of the send (if NULL then the sender did not want to receive a notification via future)
 * @param timeout timeout while receiving
 * 
 * @return returns success or error in receiving a bulk
 */
PharosChannelReceiveLocalR pharosSChannelReceiveLocal(ChannelNumber id , ptrChannelBulk bulk ,
                                                      ptrPharosPriority priority ,
                                                      ptrFutureId future , ptrPartitionNumber sender , Timeout timeout);


/**
 * Syscall for pharosChannelReceive
 * Receives a bulk through a channel on the specified channel id (could be on any partition, on any core)
 * 
 * @param id id of the channel
 * @param bulk pointer where the bulk will be placed in. The application can use this to resend the bulk to another channel
 * @param bulkMessage pointer to the buffer where the message will be copied into (just the initialization of the 
 * pointer and size) with a zero-copy transfer
 * @param dataMessage pointer to the buffer where the additional data will be copied into (2-way copy)
 * @param priority priority of the message received
 * @param future future that the receiver can make ready with a specified argument to that the sender has a confirmation and result of the send (if NULL then the sender did not want to receive a notification via future)
 * @param timeout timeout while receiving
 * 
 * @return returns success or error in receiving a bulk
 */
PharosChannelReceiveR pharosSChannelReceive(ptrChannelId id , ptrChannelBulk bulk , ptrPharosPriority priority ,
                                            ptrFutureId future , ptrPartitionNumber sender ,
                                            Timeout timeout);


/**
 * Syscall for the function pharosChannelSend.
 * 
 * Sends a channel bulk through a channel
 * 
 * @param partitionNumber partition that owns the channel where to send the bulk to
 * @param channelNumber channel to send the bulk to
 * @param bulk bulk to send
 * @param offset offset inside the bulk to send
 * @param size size of the bulk (number of bytes) 
 * @param priority priority with which to send the bulk
 * @param future future number passed to the receiver to send a notification that the send has been processed (pass NULL if no future is to be received)
 * 
 * @return returns success or the error while sending the bulk
 */
PharosChannelSendR pharosSChannelSend(ptrChannelId id , ptrChannelBulk bulk , PharosPriority priority ,
                                      ptrFutureId future);


/**
 * Send a channel bulk to a channel (on any core)
 * 
 * @constraint bulk is valid
 * 
 * @param channel channel where to send the bulk
 * @param bulk bulk to send
 */
PharosChannelSendR pharosIChannelSend(ptrChannel channel , ptrChannelBulkKernel bulk);


/**
 * Send a channel bulk to a channel on the channel core
 * 
 * @constraint must execute on the core of the channel owner
 * @constraint thread dispatch must be disabled and this function will re-enable thread dispatch 
 * 
 * @param channel channel where to send the bulk
 * @param bulk bulk to send
 * 
 * @return returns the error or success
 */
PharosChannelSendR pharosIChannelSendOnChannelCore(ptrChannel channel , ptrChannelBulkKernel bulk);


/**
 * Minimal initialization of the channels of the partition 
 * 
 * @param p partition to initialize
 */
void pharosIChannelsInitializeMinimal(ptrPartition p);


/**
 * initializes all channels inside the table
 *
 * @param p partition that contains the channels
 * @param table channel table containing all channels to initialize
 */
void pharosIChannelsInitialize(ptrPartition p , const ptrChannelTable table);


/**
 * Get the channel with the specified id
 * 
 * @param id id of the channel to get
 * 
 * @return returns the pointer to the channel or NULL if the specified channel does not exist
 */
ptrChannel pharosIChannelGet(ptrChannelId id);


/**
 * Get the channel on the running partition with the specified number
 * 
 * @param number number of the channel on the running partition 
 * 
 * @return returns the pointer to the channel or NULL if the specified channel does not exist
 */
ptrChannel pharosIChannelGetLocal(ChannelNumber number);


/**
 * Initializes a table of channel bulk queues of the specified environment and table
 * 
 * @param id partition on which the table is
 * @param table table to initialize
 */
void pharosIChannelBulkQueuesInit(const ptrPartition partition , ptrChannelBulkQueueTable table);


/**
 * Receives a message from a channel with the specified timeout
 * 
 * @param channel pointer to a channel
 * @param bulkKernel bulk to be initialized with the message received
 * @param bulk buffer to be initialized with the message received
 * @param dataMessage buffer to be copied into with the sender message
 * @param priority priority to be initialized with the priority with which the message was sent
 * @param timeout timeout for the wait
 * @param future address future id where to place the future id of the future that was sent
 * 
 * @return returns success or the error
 */
PharosChannelReceiveR pharosIChannelReceive(ptrChannel channel , ptrChannelBulk bulk ,
                                            ptrPharosPriority priority , Timeout timeout ,
                                            ptrFutureId future);


/**
 * Receives a message from a channel with the specified timeout
 * 
 * @constraint must execute on the core of the thread owner
 * @constraint must execute on the core of the channel owner
 * 
 * @param channel pointer to a channel
 * @param bulk bulk to be initialized with the message received
 * @param buffer buffer to be initialized with the message received
 * @param dataMessage buffer to be copied into with the sender message
 * @param priority priority to be initialized with the priority with which the message was sent
 * @param timeout timeout for the wait
 * @param future address future id where to place the future id of the future that was sent 
 * 
 * @return returns success or the error
 */
PharosChannelReceiveR pharosIChannelReceiveOnThreadAndChannelCore(ptrChannel channel ,
                                                                  ptrChannelBulk bulk ,
                                                                  ptrPharosPriority priority , Timeout timeout ,
                                                                  ptrFutureId future);


/**
 * If on a multi-core system, sends a rmp message to the thread owner core stating that the thread is to be awaken
 * (either timeout or a channel bulk was received) or just calls pharosIChannelReceiveReturnLocal
 * 
 * @constraint thread dispatch must be disabled. This function will always enable thread dispatch
 * 
 * @param thread thread to awaken on the remote core
 * @param channel channel where the thread tries to obtain a bulk from
 * @param bulk bulk sent
 * @param priority priority of the bulk sent
 */
void pharosIChannelReceiveReturn(ptrThreadInfo thread , ptrChannel channel , ptrChannelBulkKernel bulk ,
                                 PharosChannelReceiveR returnValue);


/**
 * Gives the channel bulk to the unblocked thread on the core of the unblocked thread.
 * 
 * @constraint This method must run on the core of the unblocked thread and on the core of the channel
 * 
 * @param unblocked thread that is unblocked to get the channel bulk
 * @param bulkKernel channel bulk to be received by the unblocked thread
 * @param returnValue return value so far
 */
void pharosIChannelReceiveReturnOnThreadCore(ptrThreadInfo unblocked , ptrChannelBulkKernel bulkKernel ,
                                             PharosChannelReceiveR returnValue);


/**
 * Copies the bulk future (if it exists) to the future number (if it exists)
 *  
 * @param number address where to place the future number of the bulk
 * @param bulkKernel bulk that contains the future
 * @param bulk application bulk
 * @param partition partition which will hold the channel bulk
 * 
 * @return returns error if the bufferMessage does not have the adequate size
 */
PharosChannelReceiveR pharosIChannelBulkCopy(ptrFutureId number ,
                                             ptrChannelBulk bulk ,
                                             ptrPharosPriority priority , ptrPartition partition);


/**
 * Check if the channel is empty to defined priority (if the channel is set to wait with a minimum priority)
 * 
 * @param channel channel
 * @param priority minimum priority (if the channel is set to wait with a minimum priority)
 * 
 * @return returns TRUE if the channel is empty to the defined priority level and FALSE otherwise
 */
bool pharosIChannelIsEmpty(ptrChannel channel , ptrPharosPriority priority);


/**
 * Get the highest priority bulk of the channel
 * 
 * @constraint assumes there is a message inside the queue 
 * @constraint assumes interrupts are disabled 
 * 
 * @param queue channel to get the bulk from
 * @param priority address where to place the priority (could be NULL)
 * 
 * @return returns the bulk got
 */
ptrChannelBulkKernel pharosIChannelGetHighestPriorityBulk(ptrChannel queue , ptrPharosPriority priority);


/**
 * Put a channel bulk inside the map of bulks of the channel
 * 
 * @constraint assumes interrupts are disabled 
 * 
 * @param channel channel where to add the channel bulk
 * @param bulk channel bulk to add
 */
void pharosIChannelBulkPut(ptrChannel channel , ptrChannelBulkKernel bulk);


/**
 * Initialize a channel bulk
 * 
 * @param bulk bulk to initialize
 * @param bulkKernel kernel bulk to initialize the bulk with
 */
void pharosIChannelBulkInitialize(ptrChannelBulk bulk , ptrChannelBulkKernel bulkKernel);



/**
 * Determines if a given channel bulk is valid (for the running thread partition)
 * 
 * @param partition partition where the bulk should belong to
 * @param bulk bulk to determine
 * 
 * @return returns TRUE if the channel bulk is valid and FALSE otherwise
 */
bool pharosIChannelBulkKernelValid(ptrPartition partition , ptrChannelBulkKernel bulk);


/**
 * Check if the buffer is inside the bulk buffer 
 * 
 * @constraint assumes the bulk is a valid bulk
 * 
 * @param bulk bulk to check
 * @param buffer buffer to check
 * 
 * @return returns TRUE if the buffer is all inside the bulk buffer and FALSE otherwise
 */
bool pharosIChannelBulkBufferValid(ptrChannelBulkKernel bulk , ptrBuffer buffer);


/**
 * Checks if the channel bulk is valid
 * 
 * @constraint assumes the bulk has a valid kernel bulk
 * @constraint assumes the bulk is on the kernel stack (that is, cannot be changed by any thread)
 * 
 * @param bulk bulk to analyse
 * 
 * @return returns TRUE if the bulk has valid buffers and FALSE otherwise
 */
bool pharosIChannelBulkValid(ptrChannelBulk bulk);


/**
 * Free a bulk (on RMP or RSP)
 * 
 * @param bulk bulk to free
 */
void pharosIChannelBulkFree(ptrChannelBulkKernel bulk);


/**
 * Free a bulk on the bulk queue core 
 * 
 * @constraint method must be invoked on the core of the channel bulk queue partition
 * 
 * @param bulk bulk to free
 */
void pharosIChannelBulkFreeOnQueueCore(ptrChannelBulkKernel bulk);


/**
 * Frees all bulks of the partition
 * 
 * @constraint must run on the core of the partition
 * 
 * @param partition partition that should free all bulks
 */
void pharosIPartitionChannelBulksReset(ptrPartition partition);


/**
 * Executes the receive filter, saves the context of the running thread and if "restarted", the function will jump back 
 * to here and return PHAROS_CHANNEL_RECEIVE_LOCAL_UNINITIALIZED
 * 
 * @param channel channel where the bulk is being received
 * 
 * @return returns the filter success or error
 */
PharosChannelFilterReceiveR pharosIChannelFilterReceive(ptrChannel channel);


/**
 * Executes the receive filter in the separate stack and with the queue owner memory permissions
 * 
 * @param runningThread running thread
 * @param channel channel 
 * @param partitionId running partition number
 * @param sharedStack shared stack in which to place the thread stack
 * @param wasInside flag indicating if the thread is already inside the running partition list of external threads 
 * @param original original partition of the running thread
 * @param originalInterPartitionContext context of this thread before this call was made
 * 
 * @return returns the filter result
 */
PharosChannelFilterReceiveR pharosIChannelFilterReceiveExecute(ptrThreadInfo runningThread , ptrChannel channel ,
                                                               PartitionNumber partitionId , ptrStack sharedStack ,
                                                               ptrPartition original , bool *wasInside ,
                                                               ptrPharosCpuInterPartitionContext originalInterPartitionContext);


/**
 * Executes the filter in the separate stack and with the queue owner memory permissions
 * 
 * @param channel channel where to send the bulk
 * @param priority priority of the bulk
 * @param bulk bulk to send
 * 
 * @return returns success if the filter says to continue and the filter error otherwise
 */
PharosChannelFilterSendR pharosIChannelFilterSend(ptrChannel channel , PharosPriority priority , ptrChannelBulk bulk);


/**
 * Executes the send filter in the separate stack and with the queue owner memory permissions
 * 
 * @param runningThread running thread
 * @param channel channel 
 * @param partitionId running partition number
 * @param bulk bulk to send
 * @param sharedStack shared stack in which to place the thread stack
 * @param original original partition of the running thread
 * @param wasInside flag indicating if the thread is already inside the running partition list of external threads 
 * @param originalInterPartitionContext context of this thread before this call was made
 * @param priority priority of the bulk
 * 
 * @return returns the filter result
 */
PharosChannelFilterSendR pharosIChannelFilterSendExecute(ptrThreadInfo runningThread , ptrChannel channel ,
                                                         PartitionNumber partitionId , ptrChannelBulk bulk ,
                                                         ptrStack sharedStack , ptrPartition original ,
                                                         bool *wasInside ,
                                                         ptrPharosCpuInterPartitionContext originalInterPartitionContext ,
                                                         PharosPriority priority);


/**
 * Set the bulk as invalid 
 * 
 * @param bulk bulk to set
 */
INLINE void pharosChannelBulkSetInvalid(ptrChannelBulk bulk);


#endif /* CHANNEL_H */
