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


#ifndef PHAROS_BULK_H
#define PHAROS_BULK_H


#include <pharos/kernel/bulk/declarations.h>
#include <pharos/kernel/collection/simplelist.h>
#include <pharos/kernel/buffer/buffer.h>
#include <pharos/kernel/object/declarations.h>
#include <pharos/bulk.h>
#include <pharos/kernel/thread/queue.h>
#include <pharos/kernel/time/declarations.h>

struct BulkQueue
{
    /**
     * queue of bulks
     */
    ptrBulk queue;

    /**
     * number of bulks inside the queue
     */
    uint32_t numberBulks;

    /**
     * Number of bytes inside each bulk
     */
    uint32_t bulkSize;

    /**
     * list of free bulks
     */
    SimpleList freeBulks;

    /**
     * Buffer to be distributed through all the bulks
     */
    Buffer bulkBuffer;

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

struct BulkInternal
{
    /**
     * Node on the list of used nodes
     */
    SimpleListNode usedNode;

    /**
     * Id of the bulk queue. This ensures that the caller does not need to know to where the bulk belongs and just return it
     */
    BulkQueueNumber number;

    /**
     * buffer of the bulk
     */
    Buffer buffer;
};

/**
 * Bulk inside a bulk queue. A bulk contains a byte buffer than can be got and given from a to a queue.
 */
union Bulk
{
    BulkInternal bulk;

    /* make sure the Bulk size is a power of 2 (because of the "%" operation in pharosIBulkInsideQueue) */
    uint8_t padding [PHAROS_NEXT_POWER_2(sizeof(BulkInternal))];
};


/**
 * Initialize all bulks
 */
void pharosIBulksInitialize(const ptrBulkTable table);


/**
 * Internal Pharos function that performs the pharosBulkGive function with supervisor privileges
 * 
 * @param bulk
 * 
 * @return 
 */
PharosBulkGiveR pharosSBulkGive(ptrBulk bulk);

/**
 * Internal Pharos function that performs the pharosBulkGet function with supervisor privileges
 * 
 * @param id bulk id
 * @param bulkDestination address of a pointer that will be initialized with the bulk address. This variable should not be used directly and is only useful when giving the bulk back (pharosBulkGive)
 * @param buffer address of buffer that will be initialized with the address and length of the buffer. The length of the buffer is the maximum size possible. You can override it and when it is given back (pharosBulkGive) it will be reset to the maximum size again
 * @param timeout time to wait for a bulk to be available
 * 
 * @return return success, invalid or unavailable
 */
PharosBulkGetR pharosSBulkGet(BulkQueueNumber id , ptrBulk *bulkDestination , ptrBuffer buffer , Timeout timeout);



#endif /* BULK_H */
