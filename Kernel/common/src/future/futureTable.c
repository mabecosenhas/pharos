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


#include <pharos/kernel/future/future.h>
#include <pharos/kernel/future/futureTable.h>
#include <pharos/kernel/partition/partition.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/kernel/future/futureinline.h>


void pharosIFutureTableReset(ptrPartition partition , ptrFutureTable table)
{
    /* iterator of the future on the future table */
    uint32_t i;

    /* future iterator */
    ptrFuture future;


    /* unblock all threads waiting for the future (on other partitions) */

    /* for each future on the partition */
    for(i = 0U; i < table->size; i++)
    {
        /* get the future */
        future = &table->futures[i];

        /* reset the future */
        pharosIFutureReset(future);
    }
}


void pharosIPartitionFuturesReset(ptrPartition p)
{

    /* interrupt level */
    PharosInterruptLevel level;

    /* future iterator */
    ptrFuture future;


    /* disable interrupts to keep coherency between the list of futures held by the partition 
     * (could happen that while the partition is restarting a future was sent by another core) */
    pharosCpuDisableInterrupts(&level);

    /* get the first future currently held by the partition */
    future = (ptrFuture) linkedListRemoveFirst(&p->externalFuturesHold);

    /* re-enable interrupts */
    pharosCpuEnableInterrupts(&level);

    /* while there are bulks held by this partition */
    while(future != NULL)
    {
        /* free the bulk (return it to the owner partition) */
        pharosIFutureReset(future);

        /* get the next bulk */
        future = (ptrFuture) linkedListRemoveFirst(&p->externalFuturesHold);
    }
}


void pharosIFutureTableInitialize(ptrPartition partition , ptrFutureTable table)
{
    /* iterator of the future on the future table */
    uint32_t i;


    /* initialize the partition list of free futures */
    simpleListInitialize(&partition->freeFutures , OFFSETOF(Future , next));

    /* for each future on the partition */
    for(i = 0U; i < table->size; i++)
    {
        /* get the next future */
        ptrFuture future = &table->futures[i];

        /* reset the blocked thread */
        future->blockedThread = NULL;

        /* reset the isReady */
        future->flags.isReady = FALSE;

        /* initialize the index on the table */
        future->index.number = i;

        /* copy the partition number */
        future->index.partition = partition->id;

        /* copy the restart number */
        future->index.partitionRestartNumber = partition->restartNumber;

        /* set the partition owner */
        future->owner = partition;

        /* not on a queue */
        future->flags.onQueue = FALSE;

        /* start the future as free */
        future->flags.isFree = TRUE;

        /* add the future to the free list */
        simpleListAppendLast(&partition->freeFutures , future);
    }
}
