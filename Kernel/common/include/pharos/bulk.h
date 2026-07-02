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


#ifndef PHAROS_API_BULK_H
#define PHAROS_API_BULK_H


#include <pharos/declarations.h>
#include <pharos/kernel/bulk/bulk.h>
#include <pharos/kernel/object/declarations.h>
#include <pharos/kernel/time/declarations.h>


/**
 * Gets the bulk for the application to use.
 * If the specified bulk does not have any more bulks left the function returns unavailable (does not block).
 * 
 * @param id bulk id
 * @param bulk address of a pointer that will be initialized with the bulk address. This variable should not be used directly and is only useful when giving the bulk back (pharosBulkGive)
 * @param buffer address of buffer that will be initialized with the address and length of the buffer. The length of the buffer is the maximum size possible. You can override it and when it is given back (pharosBulkGive) it will be reset to the maximum size again
 * @param timeout time to wait for a bulk to be available
 * 
 * @return return success, invalid or unavailable
 */
PharosBulkGetR pharosBulkGet(BulkQueueNumber id , ptrBulk *bulk , ptrBuffer buffer , Timeout timeout);


/**
 * Gives back the bulk for PHAROS to be later accessible.
 * 
 * @param bulk bulk to give 
 * 
 * @return returns the result of the method
 */
PharosBulkGiveR pharosBulkGive(ptrBulk bulk);


#endif /* BULK_H */
