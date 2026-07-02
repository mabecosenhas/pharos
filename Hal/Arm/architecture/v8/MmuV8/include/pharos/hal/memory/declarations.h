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


#ifndef PHAROS_MMUV8_DECLARATIONS2_H
#define	PHAROS_MMUV8_DECLARATIONS2_H

#include <pharos/hal/hal.h>


typedef enum
{
    PHAROS_PAGE_TABLE_4K = 0 ,
    PHAROS_PAGE_TABLE_2M = 1 ,
} PharosPageTableType;


typedef enum
{
    PHAROS_CHANNEL_BULK_QUEUE_4K = VALUE_4KiB ,
    PHAROS_CHANNEL_BULK_QUEUE_64K = VALUE_64KiB ,
    PHAROS_CHANNEL_BULK_QUEUE_2M = VALUE_2MiB ,
} PharosChannelBulkQueueSize;


typedef struct PharosCpuMmuLevel2Table PharosCpuMmuLevel2Table , *ptrPharosCpuMmuLevel2Table;
typedef struct PharosCpuMmuLevel3Table PharosCpuMmuLevel3Table , *ptrPharosCpuMmuLevel3Table;

typedef PharosCpuMmuLevel3Table PageTable4K , *ptrPageTable4K;

typedef struct PharosPage4K PharosPage4K , *ptrPharosPage4K;
typedef struct PharosPage64K PharosPage64K , *ptrPharosPage64K;
typedef struct PharosPage2M PharosPage2M , *ptrPharosPage2M;
typedef struct PharosCpuMmuBlock1Descriptor PharosCpuMmuBlock1Descriptor , *ptrPharosCpuMmuBlock1Descriptor;

typedef struct PharosCpuMmuBlock2Descriptor PharosCpuMmuBlock2Descriptor , *ptrPharosCpuMmuBlock2Descriptor;

typedef struct PharosCpuMmuTableDescriptor PharosCpuMmuTableDescriptor , *ptrPharosCpuMmuTableDescriptor;
typedef struct PharosCpuMmuInvalidDescriptor PharosCpuMmuInvalidDescriptor , *ptrPharosCpuMmuInvalidDescriptor;

typedef struct PharosCpuMmuPage4KDescriptor PharosCpuMmuPage4KDescriptor , *ptrPharosCpuMmuPage4KDescriptor;

/* PHAROS_MMUV8_DECLARATIONS2_H */
#endif	
