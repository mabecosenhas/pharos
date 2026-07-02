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


#ifndef PHAROS_QUEUE_DECLARATIONS_H
#define PHAROS_QUEUE_DECLARATIONS_H


#include <pharos/hal/hal.h>
#include <pharos/kernel/object/declarations.h>
#include <pharos/kernel/partition/declarations.h>
#include <pharos/declarations.h>


typedef struct LightWeightQueue LightWeightQueue , *ptrLightWeightQueue;

typedef struct HeavyWeightQueue HeavyWeightQueue , *ptrHeavyWeightQueue;

typedef struct LightWeightQueueTable LightWeightQueueTable , *ptrLightWeightQueueTable;

typedef struct HeavyWeightQueueTable HeavyWeightQueueTable , *ptrHeavytWeightQueueTable;

typedef ObjectNumber LwQueueNumber;

typedef ObjectNumber HwQueueNumber;

typedef struct HeavyWeightQueueMessage HeavyWeightQueueMessage , *ptrHeavyWeightQueueMessage;

typedef PharosHwQueueFilterR(*HeavyWeightFilterMethod)(void *partitionData , HwQueueNumber id , PartitionNumber sender , PharosPriority priority , uint32_t messageSize);

typedef struct PharosHwQueueMessageCore PharosHwQueueMessageCore , *ptrPharosHwQueueMessageCore;


#endif /* DECLARATIONS_H */
