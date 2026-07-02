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


#ifndef PHAROS_KERNEL_FUTURE_FUTUREQUEUE_H
#define PHAROS_KERNEL_FUTURE_FUTUREQUEUE_H


#include <pharos/declarations.h>
#include <pharos/kernel/future/declarations.h>
#include <pharos/kernel/collection/linkedlist.h>
#include <pharos/kernel/time/declarations.h>



/**
 * Add a future to the running thread future queue 
 * 
 * @param number number of the future to add
 * 
 * @return returns success or the error (e.g. invalid future number)
 */
PharosFutureQueueAddR pharosSFutureQueueAdd(ptrFutureId number);


/**
 * Removes a future from the running thread future queue
 * 
 * @param number number of the future to remove
 * 
 * @return returns success or the error (e.g. invalid future number)
 */
PharosFutureQueueRemoveR pharosSFutureQueueRemove(ptrFutureId number);


/**
 * Waits for the first future to become ready on the running thread thread queue.
 * The number of the future will be placed in the futureDone argument.
 * 
 * @param futureDone address where to place the future number of the first future that was made ready
 * @param arg argument of the future that was ready
 * @param bulk additional (optional) bulk of the future. If set to NULL and the the future is made ready with a bulk,
 * then the bulk is automatically free with this call
 * @param timeout timeout information
 * 
 * @return returns success or the error (e.g. invalid future number)
 */
PharosFutureQueueWaitR pharosSFutureQueueWait(ptrFutureId futureDone , ptrFutureArg arg , ptrChannelBulk bulk ,
                                              Timeout timeout);


#endif /* PHAROS_KERNEL_FUTURE_FUTUREQUEUE_H */
