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


#ifndef PHAROS_API_FUTURE_H
#define PHAROS_API_FUTURE_H

#include <pharos/declarations.h>
#include <pharos/kernel/future/future.h>


/**
 * get a future from the partition future queue 
 * 
 * @param number pointer to the location of the future number
 * 
 * @return returns success or the error (e.g. invalid future number)
 */
PharosFutureGetR pharosFutureGet(ptrFutureId number);


/**
 * frees a future so that it can be used again
 * 
 * @param number number of the future to free
 * @param bulk optional bulk that can also be free (bulk must be associated with the future in order to be free)
 * 
 * @return returns success or the error (e.g. invalid future number)
 */
PharosFutureFreeR pharosFutureFree(ptrFutureId number , ptrChannelBulk bulk);


/**
 * Waits for a future with a timeout
 * 
 * @param number number of the future to wait for
 * @param arg address of future argument that will be filled with the argument if the future is made ready
 * @param timeout timeout information
 * @param bulk additional (optional) bulk of the future. If set to NULL and the the future is made ready with a bulk,
 * then the bulk is automatically free with this call
 * 
 * @return returns success or the error (e.g. invalid future number)
 */
PharosFutureWaitR pharosFutureWait(ptrFutureId number , ptrFutureArg arg , ptrChannelBulk bulk , Timeout timeout);


/**
 * "Unblocks" a future by making it ready. A thread blocked on the future will be unblocked.
 * 
 * @param number number of the future to make ready
 * @param arg address of the future argument that will be copied into the thread blocked on the future (
 * pharosFutureWait)
 * @param bulk additional (optional) bulk of the future
 *  
 * @return returns success or the error (e.g. invalid future number)
 */
PharosFutureReadyR pharosFutureReady(ptrFutureId number , ptrFutureArg arg , ptrChannelBulk bulk);


/**
 * Add a future to the running thread future queue 
 * 
 * @param number number of the future to add
 * 
 * @return returns success or the error (e.g. invalid future number)
 */
PharosFutureQueueAddR pharosFutureQueueAdd(ptrFutureId number);


/**
 * Removes a future from the running thread future queue
 * 
 * @param number number of the future to remove
 * 
 * @return returns success or the error (e.g. invalid future number)
 */
PharosFutureQueueRemoveR pharosFutureQueueRemove(ptrFutureId number);


/**
 * Waits for the first future to become ready on the running thread thread queue.
 * The number of the future will be placed in the futureDone argument.
 * 
 * @param futureDone address where to place the future number of the first future that was made ready
 * @param arg argument of the future that was ready
 * @param timeout timeout information
 * @param bulk additional (optional) bulk of the future. If set to NULL and the the future is made ready with a bulk,
 * then the bulk is automatically free with this call
 * 
 * @return returns success or the error (e.g. invalid future number)
 */
PharosFutureQueueWaitR pharosFutureQueueWait(ptrFutureId futureDone , ptrFutureArg arg , ptrChannelBulk bulk ,
                                             Timeout timeout);


/**
 * Determines if the future ID is a valid future ID
 * 
 * @param future future ID to determine
 * 
 * @return returns TRUE if the future ID is valid and FALSE otherwise
 */
INLINE bool pharosFutureIsValid(ptrFutureId future)
{
    /* return value */
    bool result;


    /* check if the future is right away invalid */
    if(future != NULL)
    {
        /* check if the number is valid */
        result = future->number == PHAROS_FUTURE_NUMBER_INVALID ? FALSE : TRUE;
    }
    else
    {
        /* it is invalid */
        result = FALSE;
    }

    /* return TRUE or FALSE */
    return result;
}


#endif /* PHAROS_API_FUTURE_H */
