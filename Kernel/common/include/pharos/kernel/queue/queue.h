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


#ifndef PHAROS_QUEUE_H
#define PHAROS_QUEUE_H


#include <pharos/kernel/queue/declarations.h>
#include <pharos/kernel/collection/linkedlist.h>
#include <pharos/kernel/collection/simplelist.h>
#include <pharos/kernel/collection/treemap.h>
#include <pharos/queue.h>
#include <pharos/kernel/bulk/declarations.h>
#include <pharos/kernel/object/declarations.h>
#include <pharos/kernel/buffer/buffer.h>
#include <pharos/kernel/object/declarations.h>
#include <pharos/kernel/time/declarations.h>
#include <pharos/kernel/thread/queue.h>
#include <pharos/kernel/partition/declarations.h>
#include <pharos/hal/cpu/conf.h>
#include <pharos/kernel/core/declarations.h>
#include <pharos/kernel/core/coreconf.h>

/**
 * @brief High-speed queue (zero-copy) used for communication
 * 
 * Light-weight zero-copy queue where the sender thread sends a buffer pointer to a receiver thread that reads directly from the buffer.
 * 
 * It can be used inside the same partition/driver for a very fast communication. For example, it can be used to communicate a message from an interrupt ISR to 
 * a partition thread. It can be used from partition to partition if the message being sent is on the SHARED_MEMORY section. In this case, every partition/driver
 * can access to this memory and corrupt it so be careful using this mechanism in this way.
 */
struct LightWeightQueue
{
    /**
     * list of messages that have been sent to the queue
     * 
     * Note: initialized by PHAROS
     */
    SimpleList messagesSent;

    /**
     * list of threads blocked waiting for a message on this queue
     * 
     * Note: initialized by PHAROS
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
 * @brief Low-speed queue (two-copy) used for inter-partition/driver communication.
 *
 * Sender thread copies the message to a buffer inside the queue, which is then copied to the receiver thread buffer.
 * This way it is assured that one partition does not affect another (memory is separated). Also, since the filter 
 * method is executed in the context of the calling thread (but with memory access to the queue partition/driver) 
 * there is also temporal protection from one partition to another (an erroneous sender partition trying to overflood 
 * the queue with messages will be filtered and the filter is executed on the temporal slot of the calling partition).
 * 
 * Heavy weight queues are commonly used to communicate from different partitions with small messages that don't take 
 * too long to copy to the internal buffer.
 * 
 * For fast communication with long messages between partitions consider using Channels.
 */
struct HeavyWeightQueue
{
    /**
     * Map of messages that were sent to the queue and are available to be received
     * 
     * Note: initialized by PHAROS
     */
    TreeMap mapMessagesSent;

    /**
     * list of free messages 
     * 
     * Note: initialized by PHAROS
     */
    SimpleList freeMessages[PHAROS_NUMBER_CORES];

    /**
     * Buffer that is used to store the messages content
     * 
     * Note: initialized by the application! Buffer array should be in KERNEL_SECTION
     */
    Buffer fullBuffer[PHAROS_NUMBER_CORES];

    /**
     * Pointer to an array of HeavyWeightQueueMessage
     * 
     * Note: initialized by the application! Should point to an array of messages. The size of this array corresponds 
     * to the maximum number of messages that can be placed on the queue without it being full
     */
    ptrHeavyWeightQueueMessage messages[PHAROS_NUMBER_CORES];

    /**
     * Number of elements in the messages array. 
     * This number will correspond to the number of times in which the fullBuffer will be divided into. Each message 
     * in the messages array will be initialized with the corresponding pointer in the array and the message size
     * is messageSize.
     * 
     * Note: initialized by the application!
     */
    uint32_t numberMessages[PHAROS_NUMBER_CORES];

    /**
     * Size of each message in which the buffer will be divided into
     * 
     * Note: initialized by the application!
     */
    uint32_t messageSize;

    /**
     * list of threads blocked waiting for a message on this queue
     * 
     * Note: initialized by PHAROS
     */
    ThreadQueue receiveBlockedQueue;

    /**
     * list of threads blocked waiting for a message to be free to be able to sent it on each core
     * 
     * Note: initialized by PHAROS
     */
    ThreadQueue sendBlockedQueue[PHAROS_NUMBER_CORES];

    /**
     * owner of the environment 
     * 
     * Note: initialized by PHAROS
     */
    ptrPartition owner;

    /**
     * Filter method
     * 
     * Note: initialized by the application! Place NULL if every sender is allowed to send messages
     */
    HeavyWeightFilterMethod filter;

    /**
     * Id of the queue
     * 
     * Note: initialized by PHAROS
     */
    HwQueueNumber id;

    struct
    {
        /**
         * Flag indicating that the resource has been initialized
         * 
         * Note: initialized by PHAROS
         */
        uint32_t isInitialized : 1;

        /**
         * Flag indicating if the receiving queue is by priority (FALSE) or FIFO order (TRUE)
         * 
         * Note: initialized by the application
         */
        uint32_t isReceivingFifo : 1;

        /**
         * Flag indicating if the sending queue is by priority (FALSE) or FIFO order (TRUE).
         * Threads are on this queue if the queue is empty and threads are blocked waiting for an available message 
         * to be sent
         * 
         * Note: initialized by the application
         */
        uint32_t isSendingFifo : 1;
    } flags;
};

struct HeavyWeightQueueMessage
{
    /**
     * node used on the list of free messages 
     */
    SimpleListNode next;

    /**
     * node used on the map of sent messages
     */
    TreeNode node;

    /**
     * buffer containing the message and message size. Is reset each time it is used with the maximum size available
     */
    Buffer buffer;

    /**
     * Associated future to the message (NULL if none)
     */
    ptrFuture future;
    
    /**
     * priority with which the message was sent
     */
    PharosPriority priority;

    
    /**
     * Pointer to the next bulk with the same priority on the same channel
     */
    ptrHeavyWeightQueueMessage nextPriority;

    /**
     * Pointer to the last bulk with the same priority on the same channel
     */
    ptrHeavyWeightQueueMessage lastPriority;

    /**
     * Additional information for the core (multi-core or single-core)
     */
    PharosHwQueueMessageCore rmp;
};


/**
 * initializes all light weight queues inside the table
 *
 * Protection:
 */
void pharosILwQueuesInitialize(ptrLightWeightQueueTable table);


/**
 * Minimal initialization of the hw queues of the partition 
 * 
 * @param p partition to initialize
 */
void pharosIHwQueuesInitializeMinimal(ptrPartition p);


/**
 * initializes all heavy weight queues inside the table
 *
 * Protection:
 */
void pharosIHwQueuesInitialize(ptrPartition partition , const ptrHeavyWeightQueueTable table);


/**
 * receive a message from a lightweight queue
 * 
 * @param thread running thread
 * @param queue queue to get the message from
 * @param message pointer where to place the message
 * @param buffer buffer that contains the message payload. It is initialized by this function in order to prevent 
 * switches to kernel and user space modes. Otherwise, the user could receive a message but since it is protected 
 * inside the kernel space memory, if the user wanted the buffer contained inside the message it would required another 
 * switch to kernel space in order to read the content of the message
 * @param timeout timeout of the receive
 * 
 * @return returns success or the error
 */
PharosLwQueueReceiveTimeoutR pharosILwQueueReceiveTimeout(ptrThreadInfo thread , ptrLightWeightQueue queue ,
                                                          ptrBulk *message , ptrBuffer buffer , Timeout timeout);

/**
 * Get the queue for the specified id
 *
 * Protection: none required (queue table is not changed after initialization)
 *
 * @param id queue id
 *
 * @return returns the queue pointer or NULL if the id is invalid
 */
ptrLightWeightQueue pharosILwQueueGet(LwQueueNumber number);


/**
 * Get the queue for the specified id on the running partition
 * 
 * Protection: none required (queue table is not changed after initialization)
 * 
 * @param number queue number on the running partition
 * 
 * @return returns the queue pointer or NULL if the id is invalid
 */
ptrHeavyWeightQueue pharosIHwQueueGet(HwQueueNumber number);


/**
 * receives a heavy weight queue 
 * 
 * @param queue queue to get the message from
 * @param buffer buffer to be initialized with the message contents
 * @param size pointer to a variable that will contain the size of the message
 * @param priority pointer to a variable that will contain the priority of the message sent
 * @param timeout timeout to wait for a message
 * @param future valid future number to receive
 * 
 * @return returns success or the error 
 */
PharosHwQueueReceiveR pharosIHwQueueReceive(ptrHeavyWeightQueue queue , uint8_t *buffer , uint32_t *size , ptrPharosPriority priority , Timeout timeout , ptrFutureId future);


/**
 * Free a message on the running core
 * 
 * @param queue queue to which to give the message
 * @param message message to give 
 */
void pharosIHwQueueFreeMessageLocal(ptrHeavyWeightQueue queue , ptrHeavyWeightQueueMessage message);


/**
 * Syscall for pharosLwQueueReceiveTimeout 
 * Receives a message from a queue while blocking if the queue has no messages.
 *
 * @param number number of the lightweight queue to receive from (corresponds to the index on the lightweight queue table)
 * @param message pointer where to place the message received
 * @param buffer pointer to an address where to fill the buffer variable with the buffer that was received
 * @param timeout timeout of the receive
 * @return returns success or the error that occurred
 */
PharosLwQueueReceiveTimeoutR pharosSLwQueueReceiveTimeout(LwQueueNumber number , ptrBulk *message , ptrBuffer buffer , Timeout timeout);


/**
 * Syscall for pharosLwQueueReceive 
 * Receives a message from a queue without blocking. Can be used inside interrupt routines since it is not blocking.
 *
 * @param number number of the lightweight queue to receive from (corresponds to the index on the lightweight queue table)
 * @param message pointer where to place the message received
 * @param buffer pointer to an address where to fill the buffer variable with the buffer that was received
 * 
 * @return returns success or the error that occurred
 */
PharosLwQueueReceiveR pharosSLwQueueReceive(LwQueueNumber number , ptrBulk *message , ptrBuffer buffer);


/**
 * Syscall for pharosLwQueueSend 
 * Sends a message to a queue.
 * 
 * @param number number of the lightweight queue to send to (corresponds to the index on the lightweight queue table)
 * @param message bulk message to send
 * @param size number of bytes sent
 * 
 * @return returns success or the error that occurred
 */
PharosLwQueueSendR pharosSLwQueueSend(LwQueueNumber number , ptrBulk message , uint32_t size);


/**
 * Syscall for pharosHwQueueSend 
 * Sends a heavy weight message to the specified queue
 * 
 * @param partition number of the partition to send to
 * @param number number of the heavy weight queue inside the partition to send to
 * @param buffer buffer to send (must not be NULL)
 * @param size size of the buffer
 * @param priority priority with which to the send the message
 * @param future future number passed to the receiver to send a notification that the send has been processed (pass NULL if no future is to be received)
 * @param timeout timeout to wait for a free message
 * 
 * @return returns the result of the send, being successful or an error
 */
PharosHwQueueSendR pharosSHwQueueSend(PartitionNumber partition , HwQueueNumber number , const uint8_t *buffer , uint32_t size , PharosPriority priority , ptrFutureId future , Timeout timeout);


/**
 * Syscall for pharosHwQueueReceive
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
PharosHwQueueReceiveR pharosSHwQueueReceive(HwQueueNumber number , uint8_t *buffer , uint32_t *size , ptrPharosPriority priority , ptrFutureId future , Timeout timeout);


/**
 * Copies the message future (if it exists) to the future number (if it exists)
 *  
 * @param number address where to place the future number of the bulk
 * @param message message that contains the future
 */
void pharosICopyFuture(ptrFutureId number , ptrHeavyWeightQueueMessage bulk);


/**
 * Free a message on the local core
 * 
 * @param queue queue where the message is
 * @param message message to free
 */
void pharosIHwQueueFreeMessage(ptrHeavyWeightQueue queue , ptrHeavyWeightQueueMessage message);


/**
 * Sends a message to a hw queue
 * 
 * @param queue hw queue to send the message to 
 * @param message message to send
 * @param priority priority of the message
 * 
 * @return returns success or error
 */
PharosHwQueueSendR pharosIHwQueueSend(ptrHeavyWeightQueue queue , ptrHeavyWeightQueueMessage message , PharosPriority priority);


/**
 * Sends a message to a hw queue on the local core after having a valid queue and message
 * 
 * @constraint must be executed on the core of the queue->owner
 * 
 * @param queue hw queue
 * @param message message to send
 * @param priority priority of the message to send
 * 
 * @return returns success or error
 */
PharosHwQueueSendR pharosIHwQueueSendOnQueueCore(ptrHeavyWeightQueue queue , ptrHeavyWeightQueueMessage message , PharosPriority priority);


/**
 * Executes the filter function. Saves the context of the thread so if the HW queue partition is restarted, 
 * then this thread will go back to this function just before the call to the filter and return 
 * PHAROS_HEAVY_WEIGHT_SEND_UNINITIALIZED
 * 
 * @param queue queue to which to execute the filter
 * @param size size of the message being sent
 * @param priority priority of the message being sent
 * 
 * @return returns the result of the filter (success or error)
 */
PharosHwQueueFilterR pharosIHwQueueFilter(ptrHeavyWeightQueue queue , uint32_t size , PharosPriority priority);


/**
 * Executes the filter in the separate stack and with the queue owner memory permissions
 * 
 * @param runningThread running thread
 * @param queue hw queue to send to
 * @param size size of the message to send
 * @param priority priority of the message to send
 * @param sharedStack shared stack in which to place the thread stack
 * @param wasInside flag indicating if the thread is already inside the running partition list of external threads 
 * @param original original partition of the running thread
 * @param originalInterPartitionContext context of this thread before this call was made
 * 
 * @return returns the filter result
 */
PharosHwQueueFilterR pharosIHwQueueFilterExecute(ptrThreadInfo runningThread , ptrHeavyWeightQueue queue ,
                                                 uint32_t size , PharosPriority priority , ptrStack sharedStack ,
                                                 bool *wasInside , ptrPartition caller , ptrPartition original ,
                                                 ptrPharosCpuInterPartitionContext originalInterPartitionContext);

#endif /* QUEUE_H */
