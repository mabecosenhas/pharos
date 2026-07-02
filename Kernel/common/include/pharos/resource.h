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


#ifndef PHAROS_API_RESOURCE_H
#define PHAROS_API_RESOURCE_H


#include <pharos/declarations.h>
#include <pharos/kernel/resource/declarations.h>
#include <pharos/kernel/time/declarations.h>


/**
 * Access an application resource. Invokes the resource filter (if any) if successful, invokes the resource access method protected by its semaphore ceiling.
 * 
 * @param partitionNumber partition number (index on the table of partitions) that owns the resource that will be accessed
 * @param resourceNumber resource number (index on the table of resources) inside the partition that owns the resource
 * @param input input data
 * @param output pointer to the data that will be written with the output of the resource access method
 * @param timeout timeout for the access (timeout waiting for the semaphore ceiling)
 * 
 * @return returns success or the corresponding error
 */
PharosResourceAccessR pharosResourceAccess(PartitionNumber partitionNumber , ResourceNumber resourceNumber , uint64_t input , uint64_t *output , Timeout timeout);


#endif /* RESOURCE_H */
