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


#include <pharos/kernel/object/declarations.h>
#include <pharos/kernel/object/protection.h>
#include <pharos/hal/boardApi.h>
#include <pharos/hal/cpu/linker.h>
#include <pharos/channel.h>
#include <pharos/kernel/interrupt/interrupt.h>
#include <pharos/kernel/partition/partition.h>
#include <pharos/kernel/core/coreconfinline.h>


const EnvMemoryProtection pharosVKernelMemProtection = {
    /* initialize the kernel memory protection information */
    .startAddress[PHAROS_MEMORY_AREA_CCM] = (uint8_t *) & pharosKernelCcmStart ,
    .startAddress[PHAROS_MEMORY_AREA_SRAM1] = (uint8_t *) & pharosKernelSram1Start ,
    .startAddress[PHAROS_MEMORY_AREA_SRAM2] = (uint8_t *) & pharosKernelSram2Start ,
    .startAddress[PHAROS_MEMORY_AREA_SRAM3] = (uint8_t *) & pharosKernelSram3Start ,
    .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uint8_t *) & pharosKernelSramBackupStart ,
    .startAddress[PHAROS_MEMORY_AREA_SDRAM] = (uint8_t *) & pharosKernelSdramStart ,

    .endAddress[PHAROS_MEMORY_AREA_CCM] = (uint8_t *) & pharosKernelCcmEnd ,
    .endAddress[PHAROS_MEMORY_AREA_SRAM1] = (uint8_t *) & pharosKernelSram1End ,
    .endAddress[PHAROS_MEMORY_AREA_SRAM2] = (uint8_t *) & pharosKernelSram2End ,
    .endAddress[PHAROS_MEMORY_AREA_SRAM3] = (uint8_t *) & pharosKernelSram3End ,
    .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uint8_t *) & pharosKernelSramBackupEnd ,
    .endAddress[PHAROS_MEMORY_AREA_SDRAM] = (uint8_t *) & pharosKernelSdramEnd ,
};

const EnvMemoryProtection pharosVSharedMemProtection = {
    /* initialize the shared memory protection information */
    .startAddress[PHAROS_MEMORY_AREA_CCM] = (uint8_t *) & pharosSharedDataCcmStart ,
    .startAddress[PHAROS_MEMORY_AREA_SRAM1] = (uint8_t *) & pharosSharedDataSram1Start ,
    .startAddress[PHAROS_MEMORY_AREA_SRAM2] = (uint8_t *) & pharosSharedDataSram2Start ,
    .startAddress[PHAROS_MEMORY_AREA_SRAM3] = (uint8_t *) & pharosSharedDataSram3Start ,
    .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uint8_t *) & pharosSharedDataSramBackupStart ,
    .startAddress[PHAROS_MEMORY_AREA_SDRAM] = (uint8_t *) & pharosSharedDataSdramStart ,

    .endAddress[PHAROS_MEMORY_AREA_CCM] = (uint8_t *) & pharosSharedCcmEnd ,
    .endAddress[PHAROS_MEMORY_AREA_SRAM1] = (uint8_t *) & pharosSharedSram1End ,
    .endAddress[PHAROS_MEMORY_AREA_SRAM2] = (uint8_t *) & pharosSharedSram2End ,
    .endAddress[PHAROS_MEMORY_AREA_SRAM3] = (uint8_t *) & pharosSharedSram3End ,
    .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uint8_t *) & pharosSharedSramBackupEnd ,
    .endAddress[PHAROS_MEMORY_AREA_SDRAM] = (uint8_t *) & pharosSharedSdramEnd ,
};

const EnvMemoryProtection pharosVBulkMemProtection = {
    /* initialize the shared memory protection information */
    .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosBulkCcmStart ,
    .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosBulkSram1Start ,
    .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosBulkSram2Start ,
    .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosBulkSram3Start ,
    .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosBulkSramBackupStart ,
    .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosBulkSdramStart ,

    .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosBulkCcmEnd ,
    .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosBulkSram1End ,
    .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosBulkSram2End ,
    .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosBulkSram3End ,
    .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosBulkSramBackupEnd ,
    .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosBulkSdramEnd ,
};


const PharosMemoryArea pharosCpuSharedMemoryArea = {
    /* Ccm area */
    .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosSharedCcmStart ,
    .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosSharedCcmEnd ,
    .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosSharedDataCcmLoadAddr ,
    .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosSharedDataCcmStart ,
    .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosSharedDataCcmSize ,
    .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosSharedBssCcmStart ,
    .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosSharedBssCcmSize ,

    /* Sram1 area */
    .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosSharedSram1Start  ,
    .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosSharedSram1End ,
    .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosSharedDataSram1LoadAddr ,
    .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = (uint64_t *) &pharosSharedDataSram1Start ,
    .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosSharedDataSram1Size ,
    .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosSharedBssSram1Start ,
    .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosSharedBssSram1Size ,

    /* Sram2 area */
    .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosSharedSram2Start ,
    .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosSharedSram2End ,
    .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosSharedDataSram2LoadAddr ,
    .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosSharedDataSram2Start ,
    .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosSharedDataSram2Size ,
    .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosSharedBssSram2Start ,
    .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosSharedBssSram2Size ,

    /* Sram3 area */
    .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosSharedSram3Start ,
    .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosSharedSram3End ,
    .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosSharedDataSram3LoadAddr ,
    .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosSharedDataSram3Start ,
    .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosSharedDataSram3Size ,
    .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosSharedBssSram3Start ,
    .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosSharedBssSram3Size ,

    /* SramBackup area */
    .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosSharedSramBackupStart ,
    .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosSharedSramBackupEnd ,
    .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosSharedDataSramBackupLoadAddr ,
    .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosSharedDataSramBackupStart ,
    .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosSharedDataSramBackupSize ,
    .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosSharedBssSramBackupStart ,
    .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosSharedBssSramBackupSize ,

    /* Sdram area */
    .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosSharedSdramStart ,
    .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosSharedSdramEnd ,
    .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosSharedDataSdramLoadAddr ,
    .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosSharedDataSdramStart ,
    .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosSharedDataSdramSize ,
    .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosSharedBssSdramStart ,
    .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosSharedBssSdramSize ,
};

const PharosMemoryArea pharosCpuKernelMemoryArea = {
    /* Ccm area */
    .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosKernelCcmStart ,
    .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosKernelCcmEnd ,
    .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosKernelDataCcmLoadAddr ,
    .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosKernelDataCcmStart ,
    .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosKernelDataCcmSize ,
    .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosKernelBssCcmStart ,
    .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosKernelBssCcmSize ,

    /* Sram1 area */
    .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosKernelSram1Start ,
    .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosKernelSram1End ,
    .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosKernelDataSram1LoadAddr ,
    .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosKernelDataSram1Start ,
    .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosKernelDataSram1Size ,
    .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosKernelBssSram1Start ,
    .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosKernelBssSram1Size ,

    /* Sram2 area */
    .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosKernelSram2Start ,
    .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosKernelSram2End ,
    .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosKernelDataSram2LoadAddr ,
    .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosKernelDataSram2Start ,
    .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosKernelDataSram2Size ,
    .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosKernelBssSram2Start ,
    .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosKernelBssSram2Size ,

    /* Sram3 area */
    .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosKernelSram3Start ,
    .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosKernelSram3End ,
    .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosKernelDataSram3LoadAddr ,
    .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosKernelDataSram3Start ,
    .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosKernelDataSram3Size ,
    .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosKernelBssSram3Start ,
    .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosKernelBssSram3Size ,

    /* SramBackup area */
    .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosKernelSramBackupStart ,
    .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosKernelSramBackupEnd ,
    .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosKernelDataSramBackupLoadAddr ,
    .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosKernelDataSramBackupStart ,
    .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosKernelDataSramBackupSize ,
    .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosKernelBssSramBackupStart ,
    .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosKernelBssSramBackupSize ,

    /* Sdram area */
    .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosKernelSdramStart ,
    .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosKernelSdramEnd ,
    .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosKernelDataSdramLoadAddr ,
    .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosKernelDataSdramStart ,
    .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosKernelDataSdramSize ,
    .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosKernelBssSdramStart ,
    .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosKernelBssSdramSize ,
};

const PharosMemoryArea pharosCpuPartitionsMemoryArea[MAXIMUM_NUMBER_PARTITIONS] = {
    /* partition 0 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition0CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition0CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition0DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition0DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition0DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition0BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition0BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition0Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition0Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition0DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition0DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition0DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition0BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition0BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition0Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition0Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition0DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition0DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition0DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition0BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition0BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition0Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition0Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition0DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition0DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition0DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition0BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition0BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition0SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition0SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition0DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition0DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition0DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition0BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition0BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition0SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition0SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition0DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition0DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition0DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition0BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition0BssSdramSize ,

    } ,
    /* partition 1 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition1CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition1CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition1DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition1DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition1DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition1BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition1BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition1Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition1Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition1DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition1DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition1DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition1BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition1BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition1Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition1Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition1DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition1DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition1DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition1BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition1BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition1Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition1Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition1DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition1DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition1DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition1BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition1BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition1SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition1SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition1DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition1DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition1DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition1BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition1BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition1SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition1SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition1DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition1DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition1DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition1BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition1BssSdramSize ,

    } ,
    /* partition 2 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition2CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition2CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition2DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition2DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition2DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition2BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition2BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition2Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition2Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition2DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition2DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition2DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition2BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition2BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition2Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition2Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition2DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition2DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition2DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition2BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition2BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition2Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition2Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition2DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition2DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition2DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition2BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition2BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition2SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition2SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition2DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition2DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition2DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition2BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition2BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition2SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition2SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition2DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition2DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition2DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition2BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition2BssSdramSize ,

    } ,
    /* partition 3 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition3CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition3CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition3DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition3DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition3DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition3BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition3BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition3Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition3Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition3DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition3DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition3DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition3BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition3BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition3Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition3Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition3DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition3DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition3DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition3BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition3BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition3Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition3Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition3DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition3DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition3DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition3BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition3BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition3SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition3SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition3DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition3DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition3DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition3BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition3BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition3SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition3SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition3DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition3DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition3DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition3BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition3BssSdramSize ,

    } ,
    /* partition 4 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition4CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition4CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition4DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition4DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition4DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition4BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition4BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition4Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition4Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition4DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition4DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition4DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition4BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition4BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition4Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition4Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition4DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition4DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition4DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition4BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition4BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition4Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition4Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition4DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition4DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition4DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition4BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition4BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition4SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition4SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition4DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition4DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition4DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition4BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition4BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition4SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition4SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition4DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition4DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition4DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition4BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition4BssSdramSize ,

    } ,
    /* partition 5 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition5CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition5CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition5DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition5DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition5DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition5BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition5BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition5Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition5Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition5DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition5DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition5DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition5BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition5BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition5Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition5Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition5DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition5DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition5DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition5BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition5BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition5Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition5Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition5DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition5DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition5DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition5BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition5BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition5SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition5SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition5DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition5DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition5DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition5BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition5BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition5SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition5SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition5DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition5DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition5DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition5BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition5BssSdramSize ,

    } ,
    /* partition 6 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition6CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition6CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition6DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition6DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition6DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition6BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition6BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition6Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition6Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition6DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition6DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition6DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition6BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition6BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition6Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition6Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition6DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition6DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition6DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition6BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition6BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition6Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition6Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition6DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition6DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition6DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition6BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition6BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition6SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition6SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition6DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition6DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition6DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition6BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition6BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition6SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition6SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition6DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition6DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition6DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition6BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition6BssSdramSize ,

    } ,
    /* partition 7 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition7CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition7CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition7DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition7DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition7DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition7BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition7BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition7Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition7Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition7DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition7DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition7DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition7BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition7BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition7Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition7Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition7DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition7DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition7DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition7BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition7BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition7Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition7Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition7DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition7DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition7DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition7BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition7BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition7SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition7SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition7DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition7DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition7DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition7BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition7BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition7SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition7SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition7DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition7DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition7DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition7BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition7BssSdramSize ,

    } ,
    /* partition 8 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition8CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition8CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition8DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition8DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition8DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition8BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition8BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition8Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition8Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition8DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition8DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition8DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition8BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition8BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition8Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition8Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition8DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition8DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition8DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition8BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition8BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition8Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition8Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition8DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition8DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition8DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition8BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition8BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition8SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition8SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition8DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition8DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition8DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition8BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition8BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition8SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition8SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition8DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition8DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition8DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition8BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition8BssSdramSize ,

    } ,
    /* partition 9 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition9CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition9CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition9DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition9DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition9DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition9BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition9BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition9Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition9Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition9DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition9DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition9DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition9BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition9BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition9Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition9Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition9DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition9DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition9DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition9BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition9BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition9Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition9Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition9DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition9DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition9DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition9BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition9BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition9SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition9SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition9DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition9DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition9DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition9BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition9BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition9SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition9SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition9DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition9DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition9DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition9BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition9BssSdramSize ,

    } ,
    /* partition 10 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition10CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition10CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition10DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition10DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition10DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition10BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition10BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition10Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition10Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition10DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition10DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition10DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition10BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition10BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition10Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition10Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition10DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition10DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition10DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition10BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition10BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition10Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition10Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition10DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition10DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition10DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition10BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition10BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition10SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition10SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition10DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition10DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition10DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition10BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition10BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition10SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition10SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition10DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition10DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition10DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition10BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition10BssSdramSize ,

    } ,
    /* partition 11 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition11CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition11CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition11DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition11DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition11DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition11BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition11BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition11Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition11Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition11DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition11DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition11DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition11BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition11BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition11Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition11Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition11DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition11DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition11DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition11BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition11BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition11Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition11Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition11DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition11DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition11DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition11BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition11BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition11SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition11SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition11DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition11DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition11DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition11BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition11BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition11SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition11SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition11DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition11DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition11DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition11BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition11BssSdramSize ,

    } ,
    /* partition 12 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition12CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition12CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition12DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition12DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition12DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition12BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition12BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition12Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition12Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition12DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition12DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition12DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition12BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition12BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition12Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition12Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition12DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition12DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition12DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition12BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition12BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition12Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition12Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition12DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition12DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition12DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition12BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition12BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition12SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition12SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition12DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition12DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition12DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition12BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition12BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition12SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition12SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition12DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition12DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition12DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition12BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition12BssSdramSize ,

    } ,
    /* partition 13 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition13CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition13CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition13DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition13DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition13DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition13BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition13BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition13Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition13Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition13DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition13DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition13DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition13BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition13BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition13Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition13Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition13DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition13DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition13DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition13BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition13BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition13Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition13Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition13DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition13DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition13DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition13BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition13BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition13SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition13SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition13DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition13DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition13DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition13BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition13BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition13SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition13SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition13DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition13DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition13DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition13BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition13BssSdramSize ,

    } ,
    /* partition 14 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition14CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition14CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition14DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition14DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition14DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition14BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition14BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition14Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition14Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition14DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition14DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition14DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition14BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition14BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition14Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition14Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition14DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition14DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition14DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition14BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition14BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition14Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition14Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition14DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition14DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition14DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition14BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition14BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition14SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition14SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition14DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition14DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition14DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition14BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition14BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition14SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition14SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition14DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition14DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition14DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition14BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition14BssSdramSize ,

    } ,
    /* partition 15 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition15CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition15CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition15DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition15DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition15DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition15BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition15BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition15Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition15Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition15DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition15DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition15DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition15BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition15BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition15Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition15Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition15DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition15DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition15DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition15BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition15BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition15Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition15Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition15DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition15DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition15DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition15BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition15BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition15SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition15SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition15DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition15DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition15DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition15BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition15BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition15SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition15SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition15DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition15DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition15DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition15BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition15BssSdramSize ,

    } ,
    /* partition 16 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition16CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition16CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition16DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition16DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition16DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition16BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition16BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition16Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition16Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition16DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition16DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition16DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition16BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition16BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition16Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition16Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition16DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition16DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition16DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition16BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition16BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition16Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition16Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition16DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition16DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition16DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition16BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition16BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition16SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition16SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition16DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition16DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition16DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition16BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition16BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition16SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition16SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition16DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition16DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition16DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition16BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition16BssSdramSize ,

    } ,
    /* partition 17 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition17CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition17CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition17DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition17DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition17DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition17BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition17BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition17Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition17Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition17DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition17DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition17DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition17BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition17BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition17Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition17Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition17DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition17DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition17DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition17BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition17BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition17Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition17Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition17DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition17DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition17DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition17BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition17BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition17SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition17SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition17DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition17DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition17DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition17BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition17BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition17SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition17SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition17DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition17DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition17DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition17BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition17BssSdramSize ,

    } ,
    /* partition 18 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition18CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition18CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition18DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition18DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition18DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition18BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition18BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition18Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition18Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition18DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition18DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition18DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition18BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition18BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition18Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition18Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition18DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition18DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition18DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition18BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition18BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition18Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition18Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition18DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition18DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition18DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition18BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition18BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition18SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition18SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition18DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition18DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition18DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition18BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition18BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition18SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition18SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition18DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition18DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition18DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition18BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition18BssSdramSize ,

    } ,
    /* partition 19 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition19CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition19CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition19DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition19DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition19DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition19BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition19BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition19Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition19Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition19DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition19DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition19DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition19BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition19BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition19Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition19Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition19DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition19DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition19DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition19BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition19BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition19Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition19Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition19DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition19DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition19DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition19BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition19BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition19SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition19SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition19DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition19DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition19DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition19BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition19BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition19SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition19SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition19DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition19DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition19DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition19BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition19BssSdramSize ,

    } ,
    /* partition 20 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition20CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition20CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition20DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition20DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition20DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition20BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition20BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition20Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition20Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition20DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition20DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition20DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition20BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition20BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition20Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition20Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition20DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition20DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition20DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition20BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition20BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition20Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition20Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition20DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition20DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition20DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition20BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition20BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition20SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition20SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition20DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition20DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition20DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition20BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition20BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition20SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition20SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition20DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition20DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition20DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition20BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition20BssSdramSize ,

    } ,
    /* partition 21 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition21CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition21CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition21DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition21DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition21DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition21BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition21BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition21Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition21Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition21DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition21DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition21DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition21BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition21BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition21Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition21Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition21DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition21DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition21DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition21BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition21BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition21Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition21Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition21DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition21DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition21DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition21BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition21BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition21SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition21SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition21DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition21DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition21DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition21BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition21BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition21SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition21SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition21DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition21DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition21DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition21BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition21BssSdramSize ,

    } ,
    /* partition 22 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition22CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition22CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition22DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition22DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition22DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition22BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition22BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition22Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition22Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition22DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition22DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition22DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition22BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition22BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition22Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition22Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition22DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition22DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition22DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition22BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition22BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition22Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition22Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition22DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition22DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition22DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition22BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition22BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition22SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition22SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition22DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition22DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition22DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition22BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition22BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition22SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition22SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition22DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition22DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition22DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition22BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition22BssSdramSize ,

    } ,
    /* partition 23 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition23CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition23CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition23DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition23DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition23DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition23BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition23BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition23Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition23Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition23DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition23DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition23DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition23BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition23BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition23Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition23Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition23DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition23DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition23DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition23BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition23BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition23Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition23Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition23DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition23DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition23DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition23BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition23BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition23SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition23SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition23DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition23DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition23DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition23BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition23BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition23SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition23SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition23DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition23DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition23DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition23BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition23BssSdramSize ,

    } ,
    /* partition 24 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition24CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition24CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition24DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition24DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition24DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition24BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition24BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition24Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition24Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition24DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition24DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition24DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition24BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition24BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition24Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition24Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition24DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition24DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition24DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition24BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition24BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition24Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition24Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition24DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition24DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition24DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition24BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition24BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition24SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition24SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition24DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition24DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition24DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition24BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition24BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition24SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition24SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition24DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition24DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition24DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition24BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition24BssSdramSize ,

    } ,
    /* partition 25 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition25CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition25CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition25DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition25DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition25DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition25BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition25BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition25Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition25Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition25DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition25DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition25DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition25BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition25BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition25Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition25Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition25DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition25DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition25DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition25BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition25BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition25Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition25Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition25DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition25DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition25DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition25BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition25BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition25SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition25SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition25DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition25DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition25DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition25BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition25BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition25SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition25SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition25DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition25DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition25DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition25BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition25BssSdramSize ,

    } ,
    /* partition 26 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition26CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition26CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition26DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition26DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition26DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition26BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition26BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition26Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition26Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition26DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition26DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition26DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition26BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition26BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition26Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition26Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition26DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition26DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition26DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition26BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition26BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition26Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition26Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition26DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition26DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition26DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition26BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition26BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition26SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition26SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition26DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition26DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition26DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition26BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition26BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition26SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition26SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition26DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition26DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition26DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition26BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition26BssSdramSize ,

    } ,
    /* partition 27 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition27CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition27CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition27DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition27DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition27DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition27BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition27BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition27Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition27Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition27DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition27DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition27DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition27BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition27BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition27Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition27Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition27DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition27DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition27DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition27BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition27BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition27Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition27Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition27DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition27DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition27DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition27BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition27BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition27SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition27SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition27DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition27DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition27DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition27BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition27BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition27SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition27SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition27DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition27DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition27DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition27BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition27BssSdramSize ,

    } ,
    /* partition 28 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition28CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition28CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition28DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition28DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition28DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition28BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition28BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition28Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition28Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition28DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition28DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition28DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition28BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition28BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition28Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition28Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition28DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition28DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition28DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition28BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition28BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition28Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition28Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition28DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition28DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition28DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition28BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition28BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition28SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition28SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition28DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition28DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition28DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition28BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition28BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition28SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition28SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition28DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition28DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition28DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition28BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition28BssSdramSize ,

    } ,
    /* partition 29 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition29CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition29CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition29DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition29DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition29DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition29BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition29BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition29Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition29Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition29DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition29DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition29DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition29BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition29BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition29Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition29Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition29DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition29DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition29DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition29BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition29BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition29Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition29Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition29DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition29DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition29DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition29BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition29BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition29SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition29SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition29DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition29DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition29DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition29BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition29BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition29SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition29SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition29DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition29DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition29DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition29BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition29BssSdramSize ,

    } ,
    /* partition 30 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition30CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition30CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition30DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition30DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition30DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition30BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition30BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition30Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition30Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition30DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition30DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition30DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition30BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition30BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition30Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition30Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition30DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition30DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition30DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition30BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition30BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition30Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition30Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition30DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition30DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition30DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition30BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition30BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition30SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition30SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition30DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition30DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition30DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition30BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition30BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition30SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition30SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition30DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition30DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition30DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition30BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition30BssSdramSize ,

    } ,
    /* partition 31 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition31CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition31CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition31DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition31DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition31DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition31BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition31BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition31Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition31Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition31DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition31DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition31DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition31BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition31BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition31Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition31Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition31DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition31DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition31DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition31BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition31BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition31Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition31Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition31DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition31DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition31DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition31BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition31BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition31SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition31SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition31DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition31DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition31DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition31BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition31BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition31SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition31SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition31DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition31DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition31DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition31BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition31BssSdramSize ,

    } ,
    /* partition 32 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition32CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition32CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition32DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition32DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition32DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition32BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition32BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition32Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition32Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition32DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition32DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition32DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition32BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition32BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition32Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition32Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition32DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition32DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition32DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition32BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition32BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition32Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition32Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition32DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition32DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition32DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition32BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition32BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition32SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition32SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition32DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition32DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition32DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition32BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition32BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition32SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition32SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition32DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition32DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition32DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition32BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition32BssSdramSize ,

    } ,
    /* partition 33 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition33CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition33CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition33DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition33DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition33DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition33BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition33BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition33Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition33Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition33DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition33DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition33DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition33BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition33BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition33Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition33Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition33DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition33DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition33DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition33BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition33BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition33Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition33Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition33DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition33DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition33DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition33BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition33BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition33SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition33SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition33DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition33DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition33DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition33BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition33BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition33SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition33SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition33DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition33DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition33DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition33BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition33BssSdramSize ,

    } ,
    /* partition 34 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition34CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition34CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition34DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition34DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition34DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition34BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition34BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition34Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition34Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition34DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition34DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition34DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition34BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition34BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition34Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition34Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition34DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition34DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition34DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition34BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition34BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition34Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition34Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition34DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition34DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition34DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition34BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition34BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition34SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition34SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition34DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition34DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition34DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition34BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition34BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition34SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition34SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition34DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition34DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition34DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition34BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition34BssSdramSize ,

    } ,
    /* partition 35 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition35CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition35CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition35DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition35DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition35DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition35BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition35BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition35Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition35Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition35DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition35DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition35DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition35BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition35BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition35Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition35Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition35DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition35DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition35DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition35BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition35BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition35Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition35Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition35DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition35DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition35DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition35BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition35BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition35SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition35SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition35DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition35DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition35DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition35BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition35BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition35SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition35SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition35DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition35DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition35DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition35BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition35BssSdramSize ,

    } ,
    /* partition 36 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition36CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition36CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition36DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition36DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition36DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition36BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition36BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition36Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition36Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition36DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition36DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition36DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition36BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition36BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition36Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition36Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition36DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition36DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition36DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition36BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition36BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition36Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition36Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition36DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition36DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition36DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition36BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition36BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition36SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition36SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition36DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition36DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition36DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition36BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition36BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition36SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition36SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition36DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition36DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition36DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition36BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition36BssSdramSize ,

    } ,
    /* partition 37 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition37CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition37CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition37DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition37DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition37DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition37BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition37BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition37Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition37Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition37DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition37DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition37DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition37BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition37BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition37Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition37Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition37DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition37DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition37DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition37BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition37BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition37Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition37Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition37DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition37DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition37DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition37BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition37BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition37SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition37SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition37DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition37DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition37DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition37BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition37BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition37SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition37SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition37DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition37DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition37DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition37BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition37BssSdramSize ,

    } ,
    /* partition 38 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition38CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition38CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition38DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition38DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition38DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition38BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition38BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition38Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition38Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition38DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition38DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition38DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition38BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition38BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition38Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition38Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition38DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition38DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition38DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition38BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition38BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition38Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition38Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition38DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition38DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition38DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition38BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition38BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition38SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition38SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition38DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition38DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition38DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition38BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition38BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition38SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition38SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition38DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition38DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition38DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition38BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition38BssSdramSize ,

    } ,
    /* partition 39 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition39CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition39CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition39DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition39DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition39DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition39BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition39BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition39Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition39Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition39DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition39DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition39DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition39BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition39BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition39Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition39Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition39DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition39DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition39DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition39BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition39BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition39Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition39Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition39DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition39DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition39DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition39BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition39BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition39SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition39SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition39DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition39DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition39DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition39BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition39BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition39SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition39SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition39DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition39DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition39DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition39BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition39BssSdramSize ,

    } ,
    /* partition 40 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition40CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition40CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition40DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition40DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition40DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition40BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition40BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition40Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition40Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition40DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition40DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition40DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition40BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition40BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition40Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition40Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition40DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition40DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition40DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition40BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition40BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition40Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition40Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition40DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition40DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition40DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition40BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition40BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition40SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition40SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition40DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition40DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition40DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition40BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition40BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition40SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition40SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition40DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition40DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition40DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition40BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition40BssSdramSize ,

    } ,
    /* partition 41 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition41CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition41CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition41DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition41DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition41DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition41BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition41BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition41Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition41Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition41DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition41DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition41DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition41BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition41BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition41Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition41Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition41DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition41DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition41DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition41BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition41BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition41Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition41Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition41DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition41DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition41DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition41BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition41BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition41SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition41SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition41DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition41DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition41DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition41BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition41BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition41SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition41SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition41DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition41DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition41DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition41BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition41BssSdramSize ,

    } ,
    /* partition 42 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition42CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition42CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition42DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition42DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition42DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition42BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition42BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition42Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition42Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition42DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition42DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition42DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition42BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition42BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition42Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition42Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition42DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition42DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition42DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition42BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition42BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition42Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition42Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition42DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition42DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition42DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition42BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition42BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition42SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition42SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition42DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition42DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition42DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition42BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition42BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition42SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition42SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition42DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition42DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition42DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition42BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition42BssSdramSize ,

    } ,
    /* partition 43 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition43CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition43CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition43DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition43DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition43DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition43BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition43BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition43Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition43Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition43DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition43DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition43DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition43BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition43BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition43Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition43Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition43DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition43DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition43DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition43BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition43BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition43Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition43Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition43DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition43DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition43DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition43BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition43BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition43SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition43SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition43DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition43DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition43DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition43BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition43BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition43SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition43SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition43DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition43DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition43DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition43BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition43BssSdramSize ,

    } ,
    /* partition 44 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition44CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition44CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition44DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition44DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition44DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition44BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition44BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition44Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition44Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition44DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition44DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition44DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition44BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition44BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition44Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition44Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition44DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition44DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition44DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition44BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition44BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition44Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition44Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition44DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition44DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition44DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition44BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition44BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition44SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition44SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition44DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition44DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition44DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition44BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition44BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition44SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition44SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition44DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition44DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition44DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition44BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition44BssSdramSize ,

    } ,
    /* partition 45 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition45CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition45CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition45DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition45DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition45DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition45BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition45BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition45Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition45Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition45DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition45DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition45DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition45BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition45BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition45Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition45Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition45DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition45DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition45DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition45BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition45BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition45Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition45Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition45DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition45DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition45DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition45BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition45BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition45SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition45SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition45DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition45DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition45DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition45BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition45BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition45SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition45SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition45DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition45DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition45DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition45BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition45BssSdramSize ,

    } ,
    /* partition 46 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition46CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition46CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition46DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition46DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition46DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition46BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition46BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition46Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition46Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition46DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition46DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition46DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition46BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition46BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition46Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition46Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition46DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition46DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition46DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition46BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition46BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition46Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition46Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition46DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition46DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition46DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition46BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition46BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition46SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition46SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition46DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition46DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition46DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition46BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition46BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition46SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition46SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition46DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition46DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition46DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition46BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition46BssSdramSize ,

    } ,
    /* partition 47 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition47CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition47CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition47DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition47DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition47DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition47BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition47BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition47Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition47Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition47DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition47DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition47DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition47BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition47BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition47Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition47Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition47DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition47DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition47DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition47BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition47BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition47Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition47Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition47DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition47DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition47DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition47BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition47BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition47SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition47SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition47DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition47DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition47DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition47BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition47BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition47SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition47SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition47DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition47DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition47DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition47BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition47BssSdramSize ,

    } ,
    /* partition 48 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition48CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition48CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition48DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition48DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition48DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition48BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition48BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition48Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition48Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition48DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition48DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition48DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition48BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition48BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition48Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition48Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition48DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition48DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition48DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition48BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition48BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition48Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition48Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition48DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition48DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition48DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition48BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition48BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition48SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition48SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition48DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition48DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition48DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition48BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition48BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition48SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition48SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition48DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition48DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition48DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition48BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition48BssSdramSize ,

    } ,
    /* partition 49 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition49CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition49CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition49DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition49DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition49DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition49BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition49BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition49Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition49Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition49DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition49DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition49DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition49BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition49BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition49Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition49Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition49DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition49DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition49DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition49BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition49BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition49Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition49Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition49DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition49DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition49DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition49BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition49BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition49SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition49SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition49DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition49DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition49DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition49BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition49BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition49SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition49SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition49DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition49DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition49DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition49BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition49BssSdramSize ,

    } ,
    /* partition 50 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition50CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition50CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition50DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition50DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition50DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition50BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition50BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition50Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition50Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition50DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition50DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition50DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition50BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition50BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition50Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition50Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition50DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition50DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition50DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition50BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition50BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition50Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition50Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition50DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition50DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition50DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition50BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition50BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition50SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition50SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition50DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition50DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition50DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition50BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition50BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition50SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition50SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition50DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition50DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition50DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition50BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition50BssSdramSize ,

    } ,
    /* partition 51 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition51CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition51CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition51DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition51DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition51DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition51BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition51BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition51Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition51Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition51DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition51DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition51DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition51BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition51BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition51Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition51Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition51DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition51DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition51DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition51BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition51BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition51Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition51Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition51DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition51DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition51DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition51BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition51BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition51SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition51SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition51DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition51DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition51DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition51BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition51BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition51SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition51SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition51DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition51DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition51DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition51BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition51BssSdramSize ,

    } ,
    /* partition 52 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition52CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition52CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition52DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition52DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition52DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition52BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition52BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition52Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition52Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition52DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition52DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition52DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition52BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition52BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition52Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition52Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition52DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition52DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition52DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition52BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition52BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition52Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition52Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition52DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition52DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition52DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition52BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition52BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition52SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition52SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition52DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition52DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition52DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition52BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition52BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition52SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition52SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition52DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition52DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition52DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition52BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition52BssSdramSize ,

    } ,
    /* partition 53 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition53CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition53CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition53DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition53DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition53DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition53BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition53BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition53Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition53Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition53DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition53DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition53DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition53BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition53BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition53Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition53Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition53DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition53DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition53DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition53BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition53BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition53Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition53Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition53DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition53DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition53DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition53BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition53BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition53SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition53SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition53DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition53DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition53DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition53BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition53BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition53SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition53SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition53DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition53DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition53DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition53BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition53BssSdramSize ,

    } ,
    /* partition 54 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition54CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition54CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition54DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition54DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition54DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition54BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition54BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition54Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition54Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition54DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition54DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition54DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition54BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition54BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition54Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition54Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition54DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition54DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition54DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition54BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition54BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition54Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition54Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition54DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition54DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition54DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition54BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition54BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition54SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition54SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition54DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition54DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition54DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition54BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition54BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition54SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition54SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition54DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition54DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition54DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition54BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition54BssSdramSize ,

    } ,
    /* partition 55 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition55CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition55CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition55DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition55DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition55DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition55BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition55BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition55Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition55Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition55DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition55DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition55DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition55BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition55BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition55Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition55Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition55DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition55DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition55DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition55BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition55BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition55Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition55Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition55DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition55DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition55DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition55BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition55BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition55SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition55SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition55DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition55DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition55DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition55BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition55BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition55SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition55SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition55DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition55DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition55DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition55BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition55BssSdramSize ,

    } ,
    /* partition 56 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition56CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition56CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition56DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition56DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition56DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition56BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition56BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition56Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition56Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition56DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition56DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition56DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition56BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition56BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition56Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition56Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition56DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition56DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition56DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition56BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition56BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition56Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition56Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition56DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition56DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition56DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition56BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition56BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition56SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition56SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition56DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition56DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition56DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition56BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition56BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition56SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition56SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition56DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition56DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition56DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition56BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition56BssSdramSize ,

    } ,
    /* partition 57 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition57CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition57CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition57DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition57DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition57DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition57BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition57BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition57Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition57Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition57DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition57DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition57DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition57BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition57BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition57Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition57Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition57DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition57DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition57DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition57BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition57BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition57Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition57Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition57DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition57DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition57DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition57BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition57BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition57SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition57SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition57DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition57DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition57DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition57BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition57BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition57SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition57SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition57DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition57DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition57DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition57BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition57BssSdramSize ,

    } ,
    /* partition 58 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition58CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition58CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition58DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition58DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition58DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition58BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition58BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition58Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition58Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition58DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition58DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition58DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition58BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition58BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition58Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition58Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition58DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition58DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition58DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition58BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition58BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition58Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition58Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition58DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition58DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition58DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition58BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition58BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition58SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition58SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition58DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition58DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition58DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition58BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition58BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition58SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition58SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition58DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition58DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition58DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition58BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition58BssSdramSize ,

    } ,
    /* partition 59 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition59CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition59CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition59DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition59DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition59DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition59BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition59BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition59Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition59Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition59DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition59DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition59DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition59BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition59BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition59Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition59Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition59DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition59DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition59DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition59BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition59BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition59Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition59Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition59DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition59DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition59DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition59BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition59BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition59SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition59SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition59DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition59DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition59DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition59BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition59BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition59SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition59SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition59DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition59DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition59DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition59BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition59BssSdramSize ,

    } ,
    /* partition 60 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition60CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition60CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition60DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition60DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition60DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition60BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition60BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition60Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition60Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition60DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition60DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition60DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition60BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition60BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition60Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition60Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition60DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition60DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition60DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition60BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition60BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition60Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition60Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition60DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition60DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition60DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition60BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition60BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition60SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition60SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition60DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition60DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition60DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition60BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition60BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition60SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition60SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition60DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition60DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition60DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition60BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition60BssSdramSize ,

    } ,
    /* partition 61 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition61CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition61CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition61DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition61DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition61DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition61BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition61BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition61Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition61Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition61DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition61DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition61DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition61BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition61BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition61Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition61Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition61DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition61DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition61DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition61BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition61BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition61Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition61Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition61DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition61DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition61DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition61BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition61BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition61SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition61SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition61DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition61DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition61DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition61BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition61BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition61SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition61SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition61DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition61DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition61DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition61BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition61BssSdramSize ,

    } ,
    /* partition 62 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition62CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition62CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition62DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition62DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition62DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition62BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition62BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition62Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition62Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition62DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition62DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition62DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition62BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition62BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition62Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition62Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition62DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition62DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition62DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition62BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition62BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition62Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition62Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition62DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition62DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition62DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition62BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition62BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition62SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition62SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition62DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition62DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition62DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition62BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition62BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition62SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition62SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition62DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition62DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition62DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition62BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition62BssSdramSize ,

    } ,
    /* partition 63 memory area */
    {
        /* Ccm area */
        .startAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition63CcmStart ,
        .endAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition63CcmEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition63DataCcmLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition63DataCcmStart ,
        .dataSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition63DataCcmSize ,
        .bssAddress[PHAROS_MEMORY_AREA_CCM] = &pharosPartition63BssCcmStart ,
        .bssSize[PHAROS_MEMORY_AREA_CCM] = (uintptr_t) & pharosPartition63BssCcmSize ,

        /* Sram1 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition63Sram1Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition63Sram1End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition63DataSram1LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition63DataSram1Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition63DataSram1Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM1] = &pharosPartition63BssSram1Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM1] = (uintptr_t) & pharosPartition63BssSram1Size ,

        /* Sram2 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition63Sram2Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition63Sram2End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition63DataSram2LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition63DataSram2Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition63DataSram2Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM2] = &pharosPartition63BssSram2Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM2] = (uintptr_t) & pharosPartition63BssSram2Size ,

        /* Sram3 area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition63Sram3Start ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition63Sram3End ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition63DataSram3LoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition63DataSram3Start ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition63DataSram3Size ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM3] = &pharosPartition63BssSram3Start ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM3] = (uintptr_t) & pharosPartition63BssSram3Size ,

        /* SramBackup area */
        .startAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition63SramBackupStart ,
        .endAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition63SramBackupEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition63DataSramBackupLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition63DataSramBackupStart ,
        .dataSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition63DataSramBackupSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SRAM_BACKUP] = &pharosPartition63BssSramBackupStart ,
        .bssSize[PHAROS_MEMORY_AREA_SRAM_BACKUP] = (uintptr_t) & pharosPartition63BssSramBackupSize ,

        /* Sdram area */
        .startAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition63SdramStart ,
        .endAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition63SdramEnd ,
        .dataLoadAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition63DataSdramLoadAddr ,
        .dataRamAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition63DataSdramStart ,
        .dataSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition63DataSdramSize ,
        .bssAddress[PHAROS_MEMORY_AREA_SDRAM] = &pharosPartition63BssSdramStart ,
        .bssSize[PHAROS_MEMORY_AREA_SDRAM] = (uintptr_t) & pharosPartition63BssSdramSize ,

    } ,

};
