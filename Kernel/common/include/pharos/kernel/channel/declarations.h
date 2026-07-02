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


#ifndef PHAROS_CHANNEL_DECLARATIONS_H
#define PHAROS_CHANNEL_DECLARATIONS_H


#include <pharos/hal/hal.h>
#include <pharos/kernel/object/declarations.h>
#include <pharos/kernel/partition/declarations.h>
#include <pharos/declarations.h>

#define CHANNEL_NUMBER_PRIORITIES               PHAROS_NUMBER_PRIORITIES

/**
 * Minimum priority to receive all bulks (basically is ignored)
 */
#define PHAROS_CHANNEL_PRIORITY_MINIMUM_ALL     UINT64_MAX


typedef struct Channel Channel , *ptrChannel;

typedef struct ChannelBulk ChannelBulk , *ptrChannelBulk;

typedef struct ChannelBulkKernel ChannelBulkKernel , *ptrChannelBulkKernel;

typedef struct PharosCpuChannelBulkInfo PharosCpuChannelBulkInfo , *ptrPharosCpuChannelBulkInfo;

typedef struct PharosCpuChannelBulkQueueInf PharosCpuChannelBulkQueueInf , *ptrPharosCpuChannelBulkQueueInf;

typedef struct ChannelBulkQueue ChannelBulkQueue , *ptrChannelBulkQueue;

typedef ObjectNumber ChannelNumber;

typedef ObjectNumber ChannelBulkQueueNumber;

typedef struct ChannelTable ChannelTable , *ptrChannelTable;

typedef struct ChannelBulkQueueTable ChannelBulkQueueTable , *ptrChannelBulkQueueTable;

typedef struct ChannelId ChannelId , *ptrChannelId;

typedef PharosChannelFilterSendR(*ChannelFilterSendMethod)(void *partitionData , ChannelNumber id ,
                                                           PartitionNumber sender , PharosPriority priority ,
                                                           ptrChannelBulk bulk);

typedef PharosChannelFilterReceiveR(*ChannelFilterReceiveMethod)(void *partitionData , PartitionNumber receiver ,
                                                                 ChannelId id);

typedef struct PharosChannelBulkCore PharosChannelBulkCore , *ptrPharosChannelBulkCore;

/**
 * Make sure the channel id takes a fixed length (some assembly code is dependents on it)
 */
#pragma pack(push ,4)

struct ChannelId
{
    /**
     * Partition where the channel belongs to
     */
    PartitionNumber partition;

    /**
     * Number of the channel
     */
    ChannelNumber channel;
};

/**
 * restore the previous pack definition
 */
#pragma pack(pop)


#endif /* DECLARATIONS_H */
