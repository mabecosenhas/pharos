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


#ifndef PHAROS_API_QUEUE_H
#define PHAROS_API_QUEUE_H


#include <pharos/declarations.h>
#include <pharos/kernel/queue/declarations.h>
#include <pharos/kernel/time/declarations.h>
#include <pharos/kernel/buffer/buffer.h>
#include <pharos/kernel/bulk/declarations.h>
#include <pharos/kernel/partition/declarations.h>
#include <pharos/kernel/future/declarations.h>


/**
 * Sends a message to a queue.
 * 
 * @param number number of the lightweight queue to send to (corresponds to the index on the lightweight queue table)
 * @param message bulk message to send
 * @param size number of bytes sent
 * 
 * @return returns success or the error that occurred
 */
PharosLwQueueSendR pharosLwQueueSend(LwQueueNumber number , ptrBulk message , uint32_t size);


/**
 * Receives a message from a queue without blocking. Can be used inside interrupt routines since it is not blocking.
 *
 * @param number number of the lightweight queue to receive from (corresponds to the index on the lightweight queue table)
 * @param message pointer where to place the message received
 * @param buffer pointer to an address where to fill the buffer variable with the buffer that was received
 * 
 * @return returns success or the error that occurred
 */
PharosLwQueueReceiveR pharosLwQueueReceive(LwQueueNumber number , ptrBulk *message , ptrBuffer buffer);


/**
 * Receives a message from a queue while blocking if the queue has no messages.
 *
 * @param number number of the lightweight queue to receive from (corresponds to the index on the lightweight queue table)
 * @param message pointer where to place the message received
 * @param buffer pointer to an address where to fill the buffer variable with the buffer that was received
 * @param timeout timeout to receive a message
 * 
 * @return returns success or the error that occurred
 */
PharosLwQueueReceiveTimeoutR pharosLwQueueReceiveTimeout(LwQueueNumber number , ptrBulk *message , ptrBuffer buffer , Timeout timeout);


/**
 * Sends a heavy weight message to the specified queue
 * 
 * @note the Heavy Queue uses (internally) the memcpy function to copy the message from the user memory space (the
 * variable buffer) to the kernel memory, and then when it is received from the kernel memory back to the 
 * receiving partition memory space (argument buffer of pharosHwQueueReceive function). The memcpy is much more
 * efficient is the memory addresses are aligned to make an 8-byte copy possible. To enforce that this happens, 
 * you can aligned your vectors to "PHAROS_CPU_COPY8_ALIGN". You can use, for example:
 * 
 * uint8_t array[16] __attribute__ ((aligned(PHAROS_CPU_COPY8_ALIGN)));
 * 
 * Note that at least the kernel memory should be aligned. Doing this should be quite easy by configuring your 
 * buffer to be aligned, for example:
 * 
 * KERNEL_SECTION uint8_t hwQueueBuffer[HW_QUEUE_NUMBER_MESSAGES * HW_QUEUE_SIZE_MESSAGE] __attribute__ ((aligned(PHAROS_CPU_COPY8_ALIGN)));
 * 
 * and then making sure that the size of each message (in this example corresponds to HW_QUEUE_SIZE_MESSAGE) is 
 * a multiple of PHAROS_CPU_COPY8_ALIGN bytes.
 * 
 * Final note: the memcpy requires BOTH the destination and source arrays to be aligned. It is not sufficient that
 * only one is aligned. Check memcpy.c file to better understand the code.
 * 
 * @param partition number of the partition to send to
 * @param number number of the heavy weight queue inside the partition to send to
 * @param buffer buffer to send (must not be NULL)
 * @param size size of the buffer
 * @param future future number passed to the receiver to send a notification that the send has been processed (pass NULL if no future is to be received)
 * @param priority priority with which to the send the message
 * @param timeout timeout to wait for a free message
 * 
 * @return returns the result of the send, being successful or an error. Note that the return value only respects to the sending of the message, not to the receiving.
 * That is, if this method returns success that may not mean that the message is received by the receiver partition. For example, the receiver partition
 * could be restarted between the message being sent and the message being received. To make sure that the message was received and processed use the future
 */
PharosHwQueueSendR pharosHwQueueSend(PartitionNumber partition , HwQueueNumber number , const uint8_t *buffer , uint32_t size , PharosPriority priority , ptrFutureId future , Timeout timeout);


/**
 * Receive a heavy weight message from the specified queue number inside the running thread partition/driver. Will not block waiting for the message 
 * 
 * @param number number of the queue inside the running thread partition/driver
 * @param buffer buffer where the message will be copied to. The buffer must point to an array with sufficient size to accommodate the full message sent. The size of the buffer will be updated to size of the message sent
 * @param size address where the size will be stored
 * @param priority address where to place the priority of the message received
 * @param future future that the receiver can make ready with a specified argument to that the sender has a confirmation and result of the send (if NULL then the sender did not want to receive a notification via future)
 * @param timeout timeout of the receive
 * 
 * @return returns success or the associated error
 */
PharosHwQueueReceiveR pharosHwQueueReceive(HwQueueNumber number , uint8_t *buffer , uint32_t *size , ptrPharosPriority priority , ptrFutureId future , Timeout timeout);


#endif /* QUEUE_H */
