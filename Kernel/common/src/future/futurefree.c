/****************************************************************************
 * Pharos - A Real-Time Secure Operating System                             *
 * Copyright 2019 Pedro Macara and Filipe Monteiro                          *
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
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/kernel/core/coreinlines.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/kernel/scheduler/schedulerinline.h>


PharosFutureFreeR pharosSFutureFree(ptrFutureId number , ptrChannelBulk bulk)
{
    /* result of the function */
    PharosFutureFreeR result;

    /* future got from the list (can only free on the running partition) */
    ptrFuture future;

    /* copy of the future number (so another thread cannot change it) */
    FutureId myId;

    /* running partition memory environment */
    ptrEnvMemoryProtection env = pharosIPartitionRunningMemProt();

    volatile ptrChannelBulkKernel argumentBulkKernel = NULL;

    volatile ptrChannelBulkKernel futureBulkKernel = NULL;


    /* check if the id address is valid */
    if(pharosCpuBufferIsInsideProtection((uint8_t *) number , sizeof (FutureId) , env) == FALSE)
    {
        /* return invalid address */
        result = PHAROS_FUTURE_FREE_INVALID_ADDRESS;
    }
        /* check if the bulk address is valid */
    else if((bulk != NULL) &&
            (pharosCpuBufferIsInsideProtection((uint8_t *) bulk , sizeof (ChannelBulk) , env) == FALSE))
    {
        /* return invalid address */
        result = PHAROS_FUTURE_FREE_INVALID_BULK_ADDRESS;
    }
    else
    {
        /* create a copy of the future number */
        myId = *number;

        /* get the future (now we know that "number" is a valid address) */
        future = pharosIFutureGetLocal(&myId);

        /* if specified a bulk */
        if(bulk != NULL)
        {
            /* get its kernel bulk (so application cannot change it) */
            argumentBulkKernel = bulk->kernelBulk;
        }

        /* if there is no future with such number on the running partition */
        if(future == NULL)
        {
            /* return invalid */
            result = PHAROS_FUTURE_FREE_INVALID;
        }
            /* if the future is already free */
        else if(future->flags.isFree == TRUE)
        {
            /* return the future is already free */
            result = PHAROS_FUTURE_FREE_ALREADY_FREE;
        }
        else
        {
            /* get its kernel bulk */
            futureBulkKernel = future->bulk.kernelBulk;

            /* if the bulk kernel does not match the future bulk */
            if((argumentBulkKernel != NULL) && (pharosChannelBulkIsValid(bulk) == TRUE) && (argumentBulkKernel != futureBulkKernel))
            {
                /* return invalid bulk (can only free the bulk associated with the future) */
                result = PHAROS_FUTURE_FREE_INVALID_BULK;
            }
            else
            {
                /* if specified to free the bulk and the bulk is valid */
                if((argumentBulkKernel != NULL) && (argumentBulkKernel == futureBulkKernel))
                {
                    /* then free the bulk */
                    pharosIChannelBulkFree(futureBulkKernel);
                }

                /* free the future */
                result = pharosIFutureFree(future);
            }
        }
    }

    /* return the result (success or error) */
    return result;
}


PharosFutureFreeR pharosIFutureFree(ptrFuture future)
{
    /* return value */
    PharosFutureFreeR result;

    /* interrupt level */
    PharosInterruptLevel interruptLevel;

    /* get the free futures of the running partition */
    ptrSimpleList freeFutures;


    /* check we are running in the right core */
    PHAROS_ASSERT_CORE(future->owner->core->number , "Freeing future in invalid core");

    /* get the partition list of free futures */
    freeFutures = &future->owner->freeFutures;

    /* don't allow anyone inside threads or interrupts to change the simple list */
    pharosCpuDisableInterrupts(&interruptLevel);

    /* check to see if there is a thread blocked on the future */
    if(future->blockedThread != NULL)
    {
        /* interrupts can now be enabled */
        pharosCpuEnableInterrupts(&interruptLevel);

        /* there is, say it is in use */
        result = PHAROS_FUTURE_FREE_IN_USE;
    }
    else
    {
        /* if the future is currently on a queue */
        if(future->flags.onQueue == TRUE)
        {
            /* if the future is ready */
            if(future->flags.isReady == TRUE)
            {
                /* remove the future from the ready queue */
                linkedListRemoveObject(&future->blockedThread->futureReadyQueue , future);
            }
                /* if the future is not ready */
            else
            {
                /* remove the future from the blocked queue */
                linkedListRemoveObject(&future->blockedThread->futureBlockedQueue , future);
            }
        }

        /* set the future as free */
        future->flags.isFree = TRUE;

        /* at startup the future owner is the future->owner */
        future->flags.hasDifferentHolder = FALSE;

        /* append the future to the list of free futures of the running partition */
        simpleListAppendLast(freeFutures , future);

        /* interrupts can now be enabled */
        pharosCpuEnableInterrupts(&interruptLevel);

        /* return success */
        result = PHAROS_FUTURE_FREE_SUCCESS;
    }

    return result;
}

