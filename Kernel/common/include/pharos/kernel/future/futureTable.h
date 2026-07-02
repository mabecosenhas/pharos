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


#ifndef PHAROS_KERNEL_FUTURE_FUTURETABLE_H
#define PHAROS_KERNEL_FUTURE_FUTURETABLE_H

#include <pharos/kernel/future/declarations.h>
#include <pharos/kernel/partition/declarations.h>
#include <pharos/kernel/future/future.h>

/**
 * @brief Table of futures
 */
struct FutureTable
{
    ptrFuture futures;

    uint32_t size;
};


/**
 * Initialize the future table
 * 
 * @param partition partition that holds the futures
 * @param table future table
 */
void pharosIFutureTableInitialize(ptrPartition partition , ptrFutureTable table);


/**
 * Re-initialize the future table. Unblocks any thread waiting for the current futures and returns an error.
 * 
 * @param partition partition that holds the futures
 * @param table future table
 */
void pharosIFutureTableReset(ptrPartition partition , ptrFutureTable table);


/**
 * Reset any external futures hold by the partition 
 * 
 * @constraint only the core that owns the partition can invoke this method
 * 
 * @param p partition to reset futures
 */
void pharosIPartitionFuturesReset(ptrPartition p);


#endif /* PHAROS_KERNEL_FUTURE_PHAROS_FUTURETABLE_H */
