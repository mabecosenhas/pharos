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


#ifndef PHAROS_API_CHANNEL_H
#define PHAROS_API_CHANNEL_H


#include <pharos/declarations.h>
#include <pharos/kernel/channel/channel.h>
#include <pharos/kernel/time/declarations.h>
#include <pharos/kernel/buffer/declarations.h>
#include <pharos/kernel/object/declarations.h>
#include <pharos/kernel/channel/declarations.h>
#include <pharos/kernel/partition/declarations.h>
#include <pharos/kernel/channel/channelbulk.h>
#include <pharos/kernel/future/declarations.h>
#include <pharos/kernel/channel/channelinline.h>
#include <pharos/kernel/channel/channelbulkkernel.h>


/**
 * Initialize the channel id given its partition and channel
 * 
 * @constraint id address must be a valid address since this function is executed in user mode (i.e. this is NOT 
 * a syscall)
 * 
 * @param id address of the channel id to initialize
 * @param partition partition number of the channel which owns the channel
 * @param channel channel number inside the partition
 */
INLINE void pharosChannelIdInitialize(ptrChannelId id , PartitionNumber partition , ChannelNumber channel);


/**
 * Determines if the channel bulk is valid
 * Useful for checking if a response (using pharosChannelReceiveLocal or pharosChannelReceive) is valid and should be 
 * used (the caller expects a response)
 * 
 * @param bulk bulk to check
 * 
 * @return returns TRUE if the bulk is valid and FALSE otherwise
 */
INLINE bool pharosChannelBulkIsValid(ptrChannelBulk bulk);


/**
 * Get the bulk buffer.
 * 
 * A channel bulk contains:
 *  - a buffer (which this method returns) containing the body of the message being sent
 *  - a bulk buffer containing the full size of the channel bulk (can be used for reference and should not be changed)
 *  - an extra buffer which may contain additional information
 * 
 * The buffer could be used for example, to transmit information to a TCP partition containing the body of the message 
 * to be sent, while the extra buffer could contain some other information, such as the information of which socket
 * to transmit the information to.
 * 
 * @param bulk bulk
 * 
 * @return returns the buffer of the bulk that contains the body of the message being sent
 */
INLINE ptrBuffer pharosChannelBulkBuffer(ptrChannelBulk bulk);


/**
 * Get the bulk extra buffer
 * 
 * @param bulk bulk
 * 
 * @return returns the extra buffer of the bulk that contains the body of the message being sent
 */
INLINE ptrBuffer pharosChannelBulkBufferExtra(ptrChannelBulk bulk);


/**
 * Initializes the extra buffer of the bulk with the specified size (places at the end of the bulk and decreases the 
 * size of the bulk "normal" buffer)
 * 
 * @param bulk bulk to initialize
 * @param size size of the extra buffer
 * 
 * @return returns the extra buffer
 */
INLINE ptrBuffer pharosChannelBulkBufferExtraInit(ptrChannelBulk bulk , size_t size);


/**
 * Auxiliary function that copies an array to the bulk extra buffer 
 * 
 * @constraint assumes that the size is such that the extra buffer does NOT overlap the message buffer
 * 
 * @param bulk bulk to initialize 
 * @param array array to copy from
 * @param size size of the extra buffer
 * 
 * @return returns the extra buffer
 */
INLINE ptrBuffer pharosChannelBulkBufferExtraCopy(ptrChannelBulk bulk , const uint8_t *array , size_t size);


/**
 * Initializes the buffer and extra buffer sizes and pointers of the bulk and copies the array to the extra buffer
 * 
 * @constraint assumes the array and corresponding size fit into the bulk 
 * 
 * @param bulk channel bulk to initialize
 * @param array array to copy to the extra buffer
 * @param size number of bytes of the array to copy
 */
INLINE void pharosChannelBulkBuffersInit(ptrChannelBulk bulk , const uint8_t *array , size_t size);


/**
 * Auxiliary function that copies an array to the bulk buffer
 * 
 * @constraint assumes that the size is such that the extra buffer does NOT overlap the message buffer
 * 
 * @param bulk bulk to initialize 
 * @param array array to copy from
 * @param size size of the bulk buffer
 * 
 * @return returns the bulk buffer
 */
INLINE ptrBuffer pharosChannelBulkBufferCopy(ptrChannelBulk bulk , const uint8_t *array , size_t size);


/**
 * Sends a bulk through a channel.
 * 
 * A special case is interesting: a thread can receive a bulk, process it and send it to another channel. 
 * In this case, it could also be necessary to send only part of the bulkBuffer. In this case, you can change the 
 * buffer to "start later" and "have a smaller size" by changing the buffer and size of the bulkBuffer.
 * 
 * @param id channel to send the bulk to
 * @param bulk bulk to send 
 * @param priority priority of the message to send
 * @param future future number passed to the receiver to send a notification that the send has been processed (pass 
 * NULL if no future is to be received)
 * 
 * @return returns success or error in sending the bulk. Note that the return value only respects to the sending of the
 * bulk, not to the receiving.
 * That is, if this method returns success that may not mean that the bulk is received by the receiver partition.
 * For example, the receiver partition could be restarted between the bulk being sent and the bulk being received. 
 * To make sure that the bulk was received and processed use the future
 */
PharosChannelSendR pharosChannelSend(ptrChannelId id , ptrChannelBulk bulk , PharosPriority priority ,
                                     ptrFutureId future);


/**
 * Receives a bulk through a channel in the running partition
 * 
 * @param channel channel to receive the bulk from
 * @param bulk pointer where the bulk will be placed in. The application can use this to resend the bulk to another 
 * channel
 * @param priority address where to place the priority of the message received. If the channel is configured to wait 
 * with a minimum priority then this argument is also interpreted as the minimum priority (if NULL will allow all 
 * priorities)
 * @param future future that the receiver can make ready with a specified argument to that the sender has a 
 * confirmation and result of the send (if NULL then the sender did not want to receive a notification via future)
 * @param response bulk where to place the response to the processing of the channel bulk (NULL if none)
 * @param sender address where to place the partition that sent the bulk (if no error occurred)
 * @param timeout timeout while receiving
 * 
 * @return returns success or error in receiving a bulk
 */
PharosChannelReceiveLocalR pharosChannelReceiveLocal(ChannelNumber id , ptrChannelBulk bulk ,
                                                     ptrPharosPriority priority ,
                                                     ptrFutureId future , ptrPartitionNumber sender , Timeout timeout);


/**
 * Receives a bulk through a channel on the specified channel id (could be on any partition, on any core)
 * 
 * @param id id of the channel
 * @param bulk pointer where the bulk will be placed in. The application can use this to resend the bulk to another 
 * channel
 * @param bulkMessage pointer to the buffer where the message will be copied into (just the initialization of the 
 * pointer and size) with a zero-copy transfer
 * @param dataMessage pointer to the buffer where the additional data will be copied into (2-way copy)
 * @param priority address where to place the priority of the message received. If the channel is configured to wait 
 * with a minimum priority then this argument is also interpreted as the minimum priority (if NULL will allow all 
 * priorities)
 * @param future future that the receiver can make ready with a specified argument to that the sender has a
 * confirmation and result of the send (if NULL then the sender did not want to receive a notification via future)
 * @param sender address where to place the partition that sent the bulk (if no error occurred)
 * @param response bulk where to place the response to the processing of the channel bulk (NULL if none)
 * @param timeout timeout while receiving
 * 
 * @return returns success or error in receiving a bulk
 */
PharosChannelReceiveR pharosChannelReceive(ptrChannelId id , ptrChannelBulk bulk , ptrPharosPriority priority ,
                                           ptrFutureId future , ptrPartitionNumber sender ,
                                           Timeout timeout);


/**
 * Gets a channel bulk. The bulk is initialized with a buffer equal to its full buffer and an empty extra buffer.
 * 
 * @param number[in] number of the channel bulk
 * @param bulk[out] address where to place the bulk
 * @param buffer[in/out] address of a buffer to initialize with a pointer to the memory location of the buffer where 
 * the calling thread can write to
 * @param timeout maximum time while getting a bulk
 * 
 * @return returns success or error in getting a bulk
 */
PharosChannelBulkQueueGetR pharosChannelBulkQueueGet(ChannelBulkQueueNumber number , ptrChannelBulk bulk ,
                                                     Timeout timeout);


/**
 * Frees the oldest channel bulk that has been received and not yet free
 * 
 * @param bulk bulk MUST be the same as returned by the pharosChannelReceiveLocal method. Otherwise will return an error
 * 
 * @return returns success or error in freeing he bulk
 */
PharosChannelBulkFreeR pharosChannelBulkFree(ptrChannelBulk bulk);



#endif /* CHANNEL_H */
