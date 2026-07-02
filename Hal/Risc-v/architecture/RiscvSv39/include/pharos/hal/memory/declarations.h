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


#ifndef PHAROS_DECLARATIONS_H
#define	PHAROS_DECLARATIONS_H

typedef struct PharosCpuMmuLevelTable PharosCpuMmuLevelTable , *ptrPharosCpuMmuLevelTable;
typedef struct PharosPage4K PharosPage4K , *ptrPharosPage4K;
typedef struct PharosPage2M PharosPage2M , *ptrPharosPage2M;
typedef struct PharosPage1G PharosPage1G , *ptrPharosPage1G;



/**
 * Error caused by incorrect configuration of the channel bulk queue number of MMU tables of level 2 (insufficient)
 */
#define PHAROS_CPU_ERROR_CHANNEL_LEVEL2_TABLES_EMPTY        (PHAROS_ERR_CPU_START + 0U)

/**
 * Error caused by incorrect configuration of the channel bulk queue number of MMU tables of level 1 (insufficient)
 */
#define PHAROS_CPU_ERROR_CHANNEL_LEVEL1_TABLES_EMPTY        (PHAROS_ERR_CPU_START + 1U)


typedef enum
{
    PHAROS_PAGE_TABLE_4K = 0 ,
    PHAROS_PAGE_TABLE_2M = 1 ,
    PHAROS_PAGE_TABLE_1G = 2 ,
} PharosPageTableType;


typedef enum
{
    PHAROS_CHANNEL_BULK_QUEUE_4K = VALUE_4KiB ,
    PHAROS_CHANNEL_BULK_QUEUE_2M = VALUE_2MiB ,
    PHAROS_CHANNEL_BULK_QUEUE_1G = VALUE_1GiB ,
} PharosChannelBulkQueueSize;


/* PHAROS_DECLARATIONS_H */
#endif	
