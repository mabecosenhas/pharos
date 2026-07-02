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


#ifndef PHAROS_CPU_MEMORY_H
#define	PHAROS_CPU_MEMORY_H

#include <pharos/hal/hal.h>


#define PHAROS_MEMORY_AREA_CCM          (0U)
#define PHAROS_MEMORY_AREA_SRAM1        (1U)
#define PHAROS_MEMORY_AREA_SRAM2        (2U)
#define PHAROS_MEMORY_AREA_SRAM3        (3U)
#define PHAROS_MEMORY_AREA_SDRAM        (4U)
#define PHAROS_MEMORY_AREA_SRAM_BACKUP  (5U)


extern uint8_t pharosStartText;
extern uint8_t pharosEndText;

/**
 * Linker script variable whose address indicates the start of the read-only data
 */
extern uint8_t pharosRodataStart;

/**
 * Linker script variable whose address indicates the end of the read-only data
 */
extern uint8_t pharosRodataEnd;



/**
 * Linker script variable whose address indicates the start of the kernel section
 */
extern uint64_t pharosKernelDataCcmLoadAddr;
extern uint64_t pharosKernelDataSram1LoadAddr;
extern uint64_t pharosKernelDataSram2LoadAddr;
extern uint64_t pharosKernelDataSram3LoadAddr;
extern uint64_t pharosKernelDataSramBackupLoadAddr;
extern uint64_t pharosKernelDataSdramLoadAddr;

extern uint8_t pharosKernelDataCcmSize;
extern uint8_t pharosKernelDataSram1Size;
extern uint8_t pharosKernelDataSram2Size;
extern uint8_t pharosKernelDataSram3Size;
extern uint8_t pharosKernelDataSramBackupSize;
extern uint8_t pharosKernelDataSdramSize;

extern uint8_t pharosKernelBssCcmSize;
extern uint8_t pharosKernelBssSram1Size;
extern uint8_t pharosKernelBssSram2Size;
extern uint8_t pharosKernelBssSram3Size;
extern uint8_t pharosKernelBssSramBackupSize;
extern uint8_t pharosKernelBssSdramSize;

extern uint64_t pharosKernelCcmStart;
extern uint64_t pharosKernelSram1Start;
extern uint64_t pharosKernelSram2Start;
extern uint64_t pharosKernelSram3Start;
extern uint64_t pharosKernelSramBackupStart;
extern uint64_t pharosKernelSdramStart;

extern uint64_t pharosKernelDataCcmStart;
extern uint64_t pharosKernelDataCcmEnd;
extern uint64_t pharosKernelDataSram1Start;
extern uint64_t pharosKernelDataSram1End;
extern uint64_t pharosKernelDataSram2Start;
extern uint64_t pharosKernelDataSram2End;
extern uint64_t pharosKernelDataSram3Start;
extern uint64_t pharosKernelDataSram3End;
extern uint64_t pharosKernelDataSramBackupStart;
extern uint64_t pharosKernelDataSramBackupEnd;
extern uint64_t pharosKernelDataSdramStart;
extern uint64_t pharosKernelDataSdramEnd;

extern uint64_t pharosKernelBssCcmStart;
extern uint64_t pharosKernelBssCcmEnd;
extern uint64_t pharosKernelBssSram1Start;
extern uint64_t pharosKernelBssSram1End;
extern uint64_t pharosKernelBssSram2Start;
extern uint64_t pharosKernelBssSram2End;
extern uint64_t pharosKernelBssSram3Start;
extern uint64_t pharosKernelBssSram3End;
extern uint64_t pharosKernelBssSramBackupStart;
extern uint64_t pharosKernelBssSramBackupEnd;
extern uint64_t pharosKernelBssSdramStart;
extern uint64_t pharosKernelBssSdramEnd;


/**
 * Linker script variable whose address indicates the start of the shared section
 */
extern uint64_t pharosSharedCcmStart;
extern uint64_t pharosSharedCcmEnd;
extern uint64_t pharosSharedSram1Start;
extern uint64_t pharosSharedSram1End;
extern uint64_t pharosSharedSram2Start;
extern uint64_t pharosSharedSram2End;
extern uint64_t pharosSharedSram3Start;
extern uint64_t pharosSharedSram3End;
extern uint64_t pharosSharedSramBackupStart;
extern uint64_t pharosSharedSramBackupEnd;
extern uint64_t pharosSharedSdramStart;
extern uint64_t pharosSharedSdramEnd;

extern uint64_t pharosSharedDataCcmStart;
extern uint64_t pharosSharedDataCcmEnd;
extern uint64_t pharosSharedDataSram1Start;
extern uint64_t pharosSharedDataSram1End;
extern uint64_t pharosSharedDataSram2Start;
extern uint64_t pharosSharedDataSram2End;
extern uint64_t pharosSharedDataSram3Start;
extern uint64_t pharosSharedDataSram3End;
extern uint64_t pharosSharedDataSramBackupStart;
extern uint64_t pharosSharedDataSramBackupEnd;
extern uint64_t pharosSharedDataSdramStart;
extern uint64_t pharosSharedDataSdramEnd;

extern uint64_t pharosSharedBssCcmStart;
extern uint64_t pharosSharedBssCcmEnd;
extern uint64_t pharosSharedBssSram1Start;
extern uint64_t pharosSharedBssSram1End;
extern uint64_t pharosSharedBssSram2Start;
extern uint64_t pharosSharedBssSram2End;
extern uint64_t pharosSharedBssSram3Start;
extern uint64_t pharosSharedBssSram3End;
extern uint64_t pharosSharedBssSramBackupStart;
extern uint64_t pharosSharedBssSramBackupEnd;
extern uint64_t pharosSharedBssSdramStart;
extern uint64_t pharosSharedBssSdramEnd;

extern uint64_t pharosSharedDataCcmLoadAddr;
extern uint64_t pharosSharedDataSram1LoadAddr;
extern uint64_t pharosSharedDataSram2LoadAddr;
extern uint64_t pharosSharedDataSram3LoadAddr;
extern uint64_t pharosSharedDataSramBackupLoadAddr;
extern uint64_t pharosSharedDataSdramLoadAddr;

extern uint64_t pharosSharedDataCcmSize;
extern uint64_t pharosSharedDataSram1Size;
extern uint64_t pharosSharedDataSram2Size;
extern uint64_t pharosSharedDataSram3Size;
extern uint64_t pharosSharedDataSramBackupSize;
extern uint64_t pharosSharedDataSdramSize;

extern uint64_t pharosSharedBssCcmSize;
extern uint64_t pharosSharedBssSram1Size;
extern uint64_t pharosSharedBssSram2Size;
extern uint64_t pharosSharedBssSram3Size;
extern uint64_t pharosSharedBssSramBackupSize;
extern uint64_t pharosSharedBssSdramSize;

/**
 * Linker script variable whose address indicates the start of the shared stack section
 */
extern uint64_t pharosSharedStackCcmStart;
extern uint64_t pharosSharedStackCcmEnd;
extern uint64_t pharosSharedStackSram1Start;
extern uint64_t pharosSharedStackSram1End;
extern uint64_t pharosSharedStackSram2Start;
extern uint64_t pharosSharedStackSram2End;
extern uint64_t pharosSharedStackSram3Start;
extern uint64_t pharosSharedStackSram3End;
extern uint64_t pharosSharedStackSramBackupStart;
extern uint64_t pharosSharedStackSramBackupEnd;
extern uint64_t pharosSharedStackSdramStart;
extern uint64_t pharosSharedStackSdramEnd;






/**
 * Linker script variable associated with the IPC section
 */
extern uint64_t pharosIpCallDataLoadAddr;
extern uint8_t pharosIpCallDataSize;
extern uint8_t pharosIpCallStart;
extern uint8_t pharosIpCallEnd;
extern uint64_t pharosIpCallSectionStart;

/**
 * Linker script variable whose address indicates the start of the bulks data
 */
extern uint8_t pharosBulkCcmStart;
extern uint8_t pharosBulkSram1Start;
extern uint8_t pharosBulkSram2Start;
extern uint8_t pharosBulkSram3Start;
extern uint8_t pharosBulkSramBackupStart;
extern uint8_t pharosBulkSdramStart;

extern uint8_t pharosBulkCcmEnd;
extern uint8_t pharosBulkSram1End;
extern uint8_t pharosBulkSram2End;
extern uint8_t pharosBulkSram3End;
extern uint8_t pharosBulkSramBackupEnd;
extern uint8_t pharosBulkSdramEnd;

extern uint64_t pharosKernelCcmEnd;
extern uint64_t pharosKernelSram1End;
extern uint64_t pharosKernelSram2End;
extern uint64_t pharosKernelSram3End;
extern uint64_t pharosKernelSramBackupEnd;
extern uint64_t pharosKernelSdramEnd;


extern uint64_t pharosPartition0DataCcmLoadAddr;
extern uint8_t pharosPartition0DataCcmSize;
extern uint8_t pharosPartition0BssCcmSize;
extern uint64_t pharosPartition0CcmStart;
extern uint64_t pharosPartition0DataCcmStart;
extern uint64_t pharosPartition0BssCcmStart;
extern uint64_t pharosPartition0CcmEnd;
extern uint64_t pharosPartition0DataCcmEnd;
extern uint64_t pharosPartition0BssCcmEnd;

extern uint64_t pharosPartition0DataSram1LoadAddr;
extern uint8_t pharosPartition0DataSram1Size;
extern uint8_t pharosPartition0BssSram1Size;
extern uint64_t pharosPartition0Sram1Start;
extern uint64_t pharosPartition0DataSram1Start;
extern uint64_t pharosPartition0BssSram1Start;
extern uint64_t pharosPartition0Sram1End;
extern uint64_t pharosPartition0DataSram1End;
extern uint64_t pharosPartition0BssSram1End;

extern uint64_t pharosPartition0DataSram2LoadAddr;
extern uint8_t pharosPartition0DataSram2Size;
extern uint8_t pharosPartition0BssSram2Size;
extern uint64_t pharosPartition0Sram2Start;
extern uint64_t pharosPartition0DataSram2Start;
extern uint64_t pharosPartition0BssSram2Start;
extern uint64_t pharosPartition0Sram2End;
extern uint64_t pharosPartition0DataSram2End;
extern uint64_t pharosPartition0BssSram2End;

extern uint64_t pharosPartition0DataSram3LoadAddr;
extern uint8_t pharosPartition0DataSram3Size;
extern uint8_t pharosPartition0BssSram3Size;
extern uint64_t pharosPartition0Sram3Start;
extern uint64_t pharosPartition0DataSram3Start;
extern uint64_t pharosPartition0BssSram3Start;
extern uint64_t pharosPartition0Sram3End;
extern uint64_t pharosPartition0DataSram3End;
extern uint64_t pharosPartition0BssSram3End;

extern uint64_t pharosPartition0DataSramBackupLoadAddr;
extern uint8_t pharosPartition0DataSramBackupSize;
extern uint8_t pharosPartition0BssSramBackupSize;
extern uint64_t pharosPartition0SramBackupStart;
extern uint64_t pharosPartition0DataSramBackupStart;
extern uint64_t pharosPartition0BssSramBackupStart;
extern uint64_t pharosPartition0SramBackupEnd;
extern uint64_t pharosPartition0DataSramBackupEnd;
extern uint64_t pharosPartition0BssSramBackupEnd;

extern uint64_t pharosPartition0DataSdramLoadAddr;
extern uint8_t pharosPartition0DataSdramSize;
extern uint8_t pharosPartition0BssSdramSize;
extern uint64_t pharosPartition0SdramStart;
extern uint64_t pharosPartition0DataSdramStart;
extern uint64_t pharosPartition0BssSdramStart;
extern uint64_t pharosPartition0SdramEnd;
extern uint64_t pharosPartition0DataSdramEnd;
extern uint64_t pharosPartition0BssSdramEnd;

extern uint64_t pharosPartition1DataCcmLoadAddr;
extern uint8_t pharosPartition1DataCcmSize;
extern uint8_t pharosPartition1BssCcmSize;
extern uint64_t pharosPartition1CcmStart;
extern uint64_t pharosPartition1DataCcmStart;
extern uint64_t pharosPartition1BssCcmStart;
extern uint64_t pharosPartition1CcmEnd;
extern uint64_t pharosPartition1DataCcmEnd;
extern uint64_t pharosPartition1BssCcmEnd;

extern uint64_t pharosPartition1DataSram1LoadAddr;
extern uint8_t pharosPartition1DataSram1Size;
extern uint8_t pharosPartition1BssSram1Size;
extern uint64_t pharosPartition1Sram1Start;
extern uint64_t pharosPartition1DataSram1Start;
extern uint64_t pharosPartition1BssSram1Start;
extern uint64_t pharosPartition1Sram1End;
extern uint64_t pharosPartition1DataSram1End;
extern uint64_t pharosPartition1BssSram1End;

extern uint64_t pharosPartition1DataSram2LoadAddr;
extern uint8_t pharosPartition1DataSram2Size;
extern uint8_t pharosPartition1BssSram2Size;
extern uint64_t pharosPartition1Sram2Start;
extern uint64_t pharosPartition1DataSram2Start;
extern uint64_t pharosPartition1BssSram2Start;
extern uint64_t pharosPartition1Sram2End;
extern uint64_t pharosPartition1DataSram2End;
extern uint64_t pharosPartition1BssSram2End;

extern uint64_t pharosPartition1DataSram3LoadAddr;
extern uint8_t pharosPartition1DataSram3Size;
extern uint8_t pharosPartition1BssSram3Size;
extern uint64_t pharosPartition1Sram3Start;
extern uint64_t pharosPartition1DataSram3Start;
extern uint64_t pharosPartition1BssSram3Start;
extern uint64_t pharosPartition1Sram3End;
extern uint64_t pharosPartition1DataSram3End;
extern uint64_t pharosPartition1BssSram3End;

extern uint64_t pharosPartition1DataSramBackupLoadAddr;
extern uint8_t pharosPartition1DataSramBackupSize;
extern uint8_t pharosPartition1BssSramBackupSize;
extern uint64_t pharosPartition1SramBackupStart;
extern uint64_t pharosPartition1DataSramBackupStart;
extern uint64_t pharosPartition1BssSramBackupStart;
extern uint64_t pharosPartition1SramBackupEnd;
extern uint64_t pharosPartition1DataSramBackupEnd;
extern uint64_t pharosPartition1BssSramBackupEnd;

extern uint64_t pharosPartition1DataSdramLoadAddr;
extern uint8_t pharosPartition1DataSdramSize;
extern uint8_t pharosPartition1BssSdramSize;
extern uint64_t pharosPartition1SdramStart;
extern uint64_t pharosPartition1DataSdramStart;
extern uint64_t pharosPartition1BssSdramStart;
extern uint64_t pharosPartition1SdramEnd;
extern uint64_t pharosPartition1DataSdramEnd;
extern uint64_t pharosPartition1BssSdramEnd;

extern uint64_t pharosPartition2DataCcmLoadAddr;
extern uint8_t pharosPartition2DataCcmSize;
extern uint8_t pharosPartition2BssCcmSize;
extern uint64_t pharosPartition2CcmStart;
extern uint64_t pharosPartition2DataCcmStart;
extern uint64_t pharosPartition2BssCcmStart;
extern uint64_t pharosPartition2CcmEnd;
extern uint64_t pharosPartition2DataCcmEnd;
extern uint64_t pharosPartition2BssCcmEnd;

extern uint64_t pharosPartition2DataSram1LoadAddr;
extern uint8_t pharosPartition2DataSram1Size;
extern uint8_t pharosPartition2BssSram1Size;
extern uint64_t pharosPartition2Sram1Start;
extern uint64_t pharosPartition2DataSram1Start;
extern uint64_t pharosPartition2BssSram1Start;
extern uint64_t pharosPartition2Sram1End;
extern uint64_t pharosPartition2DataSram1End;
extern uint64_t pharosPartition2BssSram1End;

extern uint64_t pharosPartition2DataSram2LoadAddr;
extern uint8_t pharosPartition2DataSram2Size;
extern uint8_t pharosPartition2BssSram2Size;
extern uint64_t pharosPartition2Sram2Start;
extern uint64_t pharosPartition2DataSram2Start;
extern uint64_t pharosPartition2BssSram2Start;
extern uint64_t pharosPartition2Sram2End;
extern uint64_t pharosPartition2DataSram2End;
extern uint64_t pharosPartition2BssSram2End;

extern uint64_t pharosPartition2DataSram3LoadAddr;
extern uint8_t pharosPartition2DataSram3Size;
extern uint8_t pharosPartition2BssSram3Size;
extern uint64_t pharosPartition2Sram3Start;
extern uint64_t pharosPartition2DataSram3Start;
extern uint64_t pharosPartition2BssSram3Start;
extern uint64_t pharosPartition2Sram3End;
extern uint64_t pharosPartition2DataSram3End;
extern uint64_t pharosPartition2BssSram3End;

extern uint64_t pharosPartition2DataSramBackupLoadAddr;
extern uint8_t pharosPartition2DataSramBackupSize;
extern uint8_t pharosPartition2BssSramBackupSize;
extern uint64_t pharosPartition2SramBackupStart;
extern uint64_t pharosPartition2DataSramBackupStart;
extern uint64_t pharosPartition2BssSramBackupStart;
extern uint64_t pharosPartition2SramBackupEnd;
extern uint64_t pharosPartition2DataSramBackupEnd;
extern uint64_t pharosPartition2BssSramBackupEnd;

extern uint64_t pharosPartition2DataSdramLoadAddr;
extern uint8_t pharosPartition2DataSdramSize;
extern uint8_t pharosPartition2BssSdramSize;
extern uint64_t pharosPartition2SdramStart;
extern uint64_t pharosPartition2DataSdramStart;
extern uint64_t pharosPartition2BssSdramStart;
extern uint64_t pharosPartition2SdramEnd;
extern uint64_t pharosPartition2DataSdramEnd;
extern uint64_t pharosPartition2BssSdramEnd;

extern uint64_t pharosPartition3DataCcmLoadAddr;
extern uint8_t pharosPartition3DataCcmSize;
extern uint8_t pharosPartition3BssCcmSize;
extern uint64_t pharosPartition3CcmStart;
extern uint64_t pharosPartition3DataCcmStart;
extern uint64_t pharosPartition3BssCcmStart;
extern uint64_t pharosPartition3CcmEnd;
extern uint64_t pharosPartition3DataCcmEnd;
extern uint64_t pharosPartition3BssCcmEnd;

extern uint64_t pharosPartition3DataSram1LoadAddr;
extern uint8_t pharosPartition3DataSram1Size;
extern uint8_t pharosPartition3BssSram1Size;
extern uint64_t pharosPartition3Sram1Start;
extern uint64_t pharosPartition3DataSram1Start;
extern uint64_t pharosPartition3BssSram1Start;
extern uint64_t pharosPartition3Sram1End;
extern uint64_t pharosPartition3DataSram1End;
extern uint64_t pharosPartition3BssSram1End;

extern uint64_t pharosPartition3DataSram2LoadAddr;
extern uint8_t pharosPartition3DataSram2Size;
extern uint8_t pharosPartition3BssSram2Size;
extern uint64_t pharosPartition3Sram2Start;
extern uint64_t pharosPartition3DataSram2Start;
extern uint64_t pharosPartition3BssSram2Start;
extern uint64_t pharosPartition3Sram2End;
extern uint64_t pharosPartition3DataSram2End;
extern uint64_t pharosPartition3BssSram2End;

extern uint64_t pharosPartition3DataSram3LoadAddr;
extern uint8_t pharosPartition3DataSram3Size;
extern uint8_t pharosPartition3BssSram3Size;
extern uint64_t pharosPartition3Sram3Start;
extern uint64_t pharosPartition3DataSram3Start;
extern uint64_t pharosPartition3BssSram3Start;
extern uint64_t pharosPartition3Sram3End;
extern uint64_t pharosPartition3DataSram3End;
extern uint64_t pharosPartition3BssSram3End;

extern uint64_t pharosPartition3DataSramBackupLoadAddr;
extern uint8_t pharosPartition3DataSramBackupSize;
extern uint8_t pharosPartition3BssSramBackupSize;
extern uint64_t pharosPartition3SramBackupStart;
extern uint64_t pharosPartition3DataSramBackupStart;
extern uint64_t pharosPartition3BssSramBackupStart;
extern uint64_t pharosPartition3SramBackupEnd;
extern uint64_t pharosPartition3DataSramBackupEnd;
extern uint64_t pharosPartition3BssSramBackupEnd;

extern uint64_t pharosPartition3DataSdramLoadAddr;
extern uint8_t pharosPartition3DataSdramSize;
extern uint8_t pharosPartition3BssSdramSize;
extern uint64_t pharosPartition3SdramStart;
extern uint64_t pharosPartition3DataSdramStart;
extern uint64_t pharosPartition3BssSdramStart;
extern uint64_t pharosPartition3SdramEnd;
extern uint64_t pharosPartition3DataSdramEnd;
extern uint64_t pharosPartition3BssSdramEnd;

extern uint64_t pharosPartition4DataCcmLoadAddr;
extern uint8_t pharosPartition4DataCcmSize;
extern uint8_t pharosPartition4BssCcmSize;
extern uint64_t pharosPartition4CcmStart;
extern uint64_t pharosPartition4DataCcmStart;
extern uint64_t pharosPartition4BssCcmStart;
extern uint64_t pharosPartition4CcmEnd;
extern uint64_t pharosPartition4DataCcmEnd;
extern uint64_t pharosPartition4BssCcmEnd;

extern uint64_t pharosPartition4DataSram1LoadAddr;
extern uint8_t pharosPartition4DataSram1Size;
extern uint8_t pharosPartition4BssSram1Size;
extern uint64_t pharosPartition4Sram1Start;
extern uint64_t pharosPartition4DataSram1Start;
extern uint64_t pharosPartition4BssSram1Start;
extern uint64_t pharosPartition4Sram1End;
extern uint64_t pharosPartition4DataSram1End;
extern uint64_t pharosPartition4BssSram1End;

extern uint64_t pharosPartition4DataSram2LoadAddr;
extern uint8_t pharosPartition4DataSram2Size;
extern uint8_t pharosPartition4BssSram2Size;
extern uint64_t pharosPartition4Sram2Start;
extern uint64_t pharosPartition4DataSram2Start;
extern uint64_t pharosPartition4BssSram2Start;
extern uint64_t pharosPartition4Sram2End;
extern uint64_t pharosPartition4DataSram2End;
extern uint64_t pharosPartition4BssSram2End;

extern uint64_t pharosPartition4DataSram3LoadAddr;
extern uint8_t pharosPartition4DataSram3Size;
extern uint8_t pharosPartition4BssSram3Size;
extern uint64_t pharosPartition4Sram3Start;
extern uint64_t pharosPartition4DataSram3Start;
extern uint64_t pharosPartition4BssSram3Start;
extern uint64_t pharosPartition4Sram3End;
extern uint64_t pharosPartition4DataSram3End;
extern uint64_t pharosPartition4BssSram3End;

extern uint64_t pharosPartition4DataSramBackupLoadAddr;
extern uint8_t pharosPartition4DataSramBackupSize;
extern uint8_t pharosPartition4BssSramBackupSize;
extern uint64_t pharosPartition4SramBackupStart;
extern uint64_t pharosPartition4DataSramBackupStart;
extern uint64_t pharosPartition4BssSramBackupStart;
extern uint64_t pharosPartition4SramBackupEnd;
extern uint64_t pharosPartition4DataSramBackupEnd;
extern uint64_t pharosPartition4BssSramBackupEnd;

extern uint64_t pharosPartition4DataSdramLoadAddr;
extern uint8_t pharosPartition4DataSdramSize;
extern uint8_t pharosPartition4BssSdramSize;
extern uint64_t pharosPartition4SdramStart;
extern uint64_t pharosPartition4DataSdramStart;
extern uint64_t pharosPartition4BssSdramStart;
extern uint64_t pharosPartition4SdramEnd;
extern uint64_t pharosPartition4DataSdramEnd;
extern uint64_t pharosPartition4BssSdramEnd;

extern uint64_t pharosPartition5DataCcmLoadAddr;
extern uint8_t pharosPartition5DataCcmSize;
extern uint8_t pharosPartition5BssCcmSize;
extern uint64_t pharosPartition5CcmStart;
extern uint64_t pharosPartition5DataCcmStart;
extern uint64_t pharosPartition5BssCcmStart;
extern uint64_t pharosPartition5CcmEnd;
extern uint64_t pharosPartition5DataCcmEnd;
extern uint64_t pharosPartition5BssCcmEnd;

extern uint64_t pharosPartition5DataSram1LoadAddr;
extern uint8_t pharosPartition5DataSram1Size;
extern uint8_t pharosPartition5BssSram1Size;
extern uint64_t pharosPartition5Sram1Start;
extern uint64_t pharosPartition5DataSram1Start;
extern uint64_t pharosPartition5BssSram1Start;
extern uint64_t pharosPartition5Sram1End;
extern uint64_t pharosPartition5DataSram1End;
extern uint64_t pharosPartition5BssSram1End;

extern uint64_t pharosPartition5DataSram2LoadAddr;
extern uint8_t pharosPartition5DataSram2Size;
extern uint8_t pharosPartition5BssSram2Size;
extern uint64_t pharosPartition5Sram2Start;
extern uint64_t pharosPartition5DataSram2Start;
extern uint64_t pharosPartition5BssSram2Start;
extern uint64_t pharosPartition5Sram2End;
extern uint64_t pharosPartition5DataSram2End;
extern uint64_t pharosPartition5BssSram2End;

extern uint64_t pharosPartition5DataSram3LoadAddr;
extern uint8_t pharosPartition5DataSram3Size;
extern uint8_t pharosPartition5BssSram3Size;
extern uint64_t pharosPartition5Sram3Start;
extern uint64_t pharosPartition5DataSram3Start;
extern uint64_t pharosPartition5BssSram3Start;
extern uint64_t pharosPartition5Sram3End;
extern uint64_t pharosPartition5DataSram3End;
extern uint64_t pharosPartition5BssSram3End;

extern uint64_t pharosPartition5DataSramBackupLoadAddr;
extern uint8_t pharosPartition5DataSramBackupSize;
extern uint8_t pharosPartition5BssSramBackupSize;
extern uint64_t pharosPartition5SramBackupStart;
extern uint64_t pharosPartition5DataSramBackupStart;
extern uint64_t pharosPartition5BssSramBackupStart;
extern uint64_t pharosPartition5SramBackupEnd;
extern uint64_t pharosPartition5DataSramBackupEnd;
extern uint64_t pharosPartition5BssSramBackupEnd;

extern uint64_t pharosPartition5DataSdramLoadAddr;
extern uint8_t pharosPartition5DataSdramSize;
extern uint8_t pharosPartition5BssSdramSize;
extern uint64_t pharosPartition5SdramStart;
extern uint64_t pharosPartition5DataSdramStart;
extern uint64_t pharosPartition5BssSdramStart;
extern uint64_t pharosPartition5SdramEnd;
extern uint64_t pharosPartition5DataSdramEnd;
extern uint64_t pharosPartition5BssSdramEnd;

extern uint64_t pharosPartition6DataCcmLoadAddr;
extern uint8_t pharosPartition6DataCcmSize;
extern uint8_t pharosPartition6BssCcmSize;
extern uint64_t pharosPartition6CcmStart;
extern uint64_t pharosPartition6DataCcmStart;
extern uint64_t pharosPartition6BssCcmStart;
extern uint64_t pharosPartition6CcmEnd;
extern uint64_t pharosPartition6DataCcmEnd;
extern uint64_t pharosPartition6BssCcmEnd;

extern uint64_t pharosPartition6DataSram1LoadAddr;
extern uint8_t pharosPartition6DataSram1Size;
extern uint8_t pharosPartition6BssSram1Size;
extern uint64_t pharosPartition6Sram1Start;
extern uint64_t pharosPartition6DataSram1Start;
extern uint64_t pharosPartition6BssSram1Start;
extern uint64_t pharosPartition6Sram1End;
extern uint64_t pharosPartition6DataSram1End;
extern uint64_t pharosPartition6BssSram1End;

extern uint64_t pharosPartition6DataSram2LoadAddr;
extern uint8_t pharosPartition6DataSram2Size;
extern uint8_t pharosPartition6BssSram2Size;
extern uint64_t pharosPartition6Sram2Start;
extern uint64_t pharosPartition6DataSram2Start;
extern uint64_t pharosPartition6BssSram2Start;
extern uint64_t pharosPartition6Sram2End;
extern uint64_t pharosPartition6DataSram2End;
extern uint64_t pharosPartition6BssSram2End;

extern uint64_t pharosPartition6DataSram3LoadAddr;
extern uint8_t pharosPartition6DataSram3Size;
extern uint8_t pharosPartition6BssSram3Size;
extern uint64_t pharosPartition6Sram3Start;
extern uint64_t pharosPartition6DataSram3Start;
extern uint64_t pharosPartition6BssSram3Start;
extern uint64_t pharosPartition6Sram3End;
extern uint64_t pharosPartition6DataSram3End;
extern uint64_t pharosPartition6BssSram3End;

extern uint64_t pharosPartition6DataSramBackupLoadAddr;
extern uint8_t pharosPartition6DataSramBackupSize;
extern uint8_t pharosPartition6BssSramBackupSize;
extern uint64_t pharosPartition6SramBackupStart;
extern uint64_t pharosPartition6DataSramBackupStart;
extern uint64_t pharosPartition6BssSramBackupStart;
extern uint64_t pharosPartition6SramBackupEnd;
extern uint64_t pharosPartition6DataSramBackupEnd;
extern uint64_t pharosPartition6BssSramBackupEnd;

extern uint64_t pharosPartition6DataSdramLoadAddr;
extern uint8_t pharosPartition6DataSdramSize;
extern uint8_t pharosPartition6BssSdramSize;
extern uint64_t pharosPartition6SdramStart;
extern uint64_t pharosPartition6DataSdramStart;
extern uint64_t pharosPartition6BssSdramStart;
extern uint64_t pharosPartition6SdramEnd;
extern uint64_t pharosPartition6DataSdramEnd;
extern uint64_t pharosPartition6BssSdramEnd;

extern uint64_t pharosPartition7DataCcmLoadAddr;
extern uint8_t pharosPartition7DataCcmSize;
extern uint8_t pharosPartition7BssCcmSize;
extern uint64_t pharosPartition7CcmStart;
extern uint64_t pharosPartition7DataCcmStart;
extern uint64_t pharosPartition7BssCcmStart;
extern uint64_t pharosPartition7CcmEnd;
extern uint64_t pharosPartition7DataCcmEnd;
extern uint64_t pharosPartition7BssCcmEnd;

extern uint64_t pharosPartition7DataSram1LoadAddr;
extern uint8_t pharosPartition7DataSram1Size;
extern uint8_t pharosPartition7BssSram1Size;
extern uint64_t pharosPartition7Sram1Start;
extern uint64_t pharosPartition7DataSram1Start;
extern uint64_t pharosPartition7BssSram1Start;
extern uint64_t pharosPartition7Sram1End;
extern uint64_t pharosPartition7DataSram1End;
extern uint64_t pharosPartition7BssSram1End;

extern uint64_t pharosPartition7DataSram2LoadAddr;
extern uint8_t pharosPartition7DataSram2Size;
extern uint8_t pharosPartition7BssSram2Size;
extern uint64_t pharosPartition7Sram2Start;
extern uint64_t pharosPartition7DataSram2Start;
extern uint64_t pharosPartition7BssSram2Start;
extern uint64_t pharosPartition7Sram2End;
extern uint64_t pharosPartition7DataSram2End;
extern uint64_t pharosPartition7BssSram2End;

extern uint64_t pharosPartition7DataSram3LoadAddr;
extern uint8_t pharosPartition7DataSram3Size;
extern uint8_t pharosPartition7BssSram3Size;
extern uint64_t pharosPartition7Sram3Start;
extern uint64_t pharosPartition7DataSram3Start;
extern uint64_t pharosPartition7BssSram3Start;
extern uint64_t pharosPartition7Sram3End;
extern uint64_t pharosPartition7DataSram3End;
extern uint64_t pharosPartition7BssSram3End;

extern uint64_t pharosPartition7DataSramBackupLoadAddr;
extern uint8_t pharosPartition7DataSramBackupSize;
extern uint8_t pharosPartition7BssSramBackupSize;
extern uint64_t pharosPartition7SramBackupStart;
extern uint64_t pharosPartition7DataSramBackupStart;
extern uint64_t pharosPartition7BssSramBackupStart;
extern uint64_t pharosPartition7SramBackupEnd;
extern uint64_t pharosPartition7DataSramBackupEnd;
extern uint64_t pharosPartition7BssSramBackupEnd;

extern uint64_t pharosPartition7DataSdramLoadAddr;
extern uint8_t pharosPartition7DataSdramSize;
extern uint8_t pharosPartition7BssSdramSize;
extern uint64_t pharosPartition7SdramStart;
extern uint64_t pharosPartition7DataSdramStart;
extern uint64_t pharosPartition7BssSdramStart;
extern uint64_t pharosPartition7SdramEnd;
extern uint64_t pharosPartition7DataSdramEnd;
extern uint64_t pharosPartition7BssSdramEnd;

extern uint64_t pharosPartition8DataCcmLoadAddr;
extern uint8_t pharosPartition8DataCcmSize;
extern uint8_t pharosPartition8BssCcmSize;
extern uint64_t pharosPartition8CcmStart;
extern uint64_t pharosPartition8DataCcmStart;
extern uint64_t pharosPartition8BssCcmStart;
extern uint64_t pharosPartition8CcmEnd;
extern uint64_t pharosPartition8DataCcmEnd;
extern uint64_t pharosPartition8BssCcmEnd;

extern uint64_t pharosPartition8DataSram1LoadAddr;
extern uint8_t pharosPartition8DataSram1Size;
extern uint8_t pharosPartition8BssSram1Size;
extern uint64_t pharosPartition8Sram1Start;
extern uint64_t pharosPartition8DataSram1Start;
extern uint64_t pharosPartition8BssSram1Start;
extern uint64_t pharosPartition8Sram1End;
extern uint64_t pharosPartition8DataSram1End;
extern uint64_t pharosPartition8BssSram1End;

extern uint64_t pharosPartition8DataSram2LoadAddr;
extern uint8_t pharosPartition8DataSram2Size;
extern uint8_t pharosPartition8BssSram2Size;
extern uint64_t pharosPartition8Sram2Start;
extern uint64_t pharosPartition8DataSram2Start;
extern uint64_t pharosPartition8BssSram2Start;
extern uint64_t pharosPartition8Sram2End;
extern uint64_t pharosPartition8DataSram2End;
extern uint64_t pharosPartition8BssSram2End;

extern uint64_t pharosPartition8DataSram3LoadAddr;
extern uint8_t pharosPartition8DataSram3Size;
extern uint8_t pharosPartition8BssSram3Size;
extern uint64_t pharosPartition8Sram3Start;
extern uint64_t pharosPartition8DataSram3Start;
extern uint64_t pharosPartition8BssSram3Start;
extern uint64_t pharosPartition8Sram3End;
extern uint64_t pharosPartition8DataSram3End;
extern uint64_t pharosPartition8BssSram3End;

extern uint64_t pharosPartition8DataSramBackupLoadAddr;
extern uint8_t pharosPartition8DataSramBackupSize;
extern uint8_t pharosPartition8BssSramBackupSize;
extern uint64_t pharosPartition8SramBackupStart;
extern uint64_t pharosPartition8DataSramBackupStart;
extern uint64_t pharosPartition8BssSramBackupStart;
extern uint64_t pharosPartition8SramBackupEnd;
extern uint64_t pharosPartition8DataSramBackupEnd;
extern uint64_t pharosPartition8BssSramBackupEnd;

extern uint64_t pharosPartition8DataSdramLoadAddr;
extern uint8_t pharosPartition8DataSdramSize;
extern uint8_t pharosPartition8BssSdramSize;
extern uint64_t pharosPartition8SdramStart;
extern uint64_t pharosPartition8DataSdramStart;
extern uint64_t pharosPartition8BssSdramStart;
extern uint64_t pharosPartition8SdramEnd;
extern uint64_t pharosPartition8DataSdramEnd;
extern uint64_t pharosPartition8BssSdramEnd;

extern uint64_t pharosPartition9DataCcmLoadAddr;
extern uint8_t pharosPartition9DataCcmSize;
extern uint8_t pharosPartition9BssCcmSize;
extern uint64_t pharosPartition9CcmStart;
extern uint64_t pharosPartition9DataCcmStart;
extern uint64_t pharosPartition9BssCcmStart;
extern uint64_t pharosPartition9CcmEnd;
extern uint64_t pharosPartition9DataCcmEnd;
extern uint64_t pharosPartition9BssCcmEnd;

extern uint64_t pharosPartition9DataSram1LoadAddr;
extern uint8_t pharosPartition9DataSram1Size;
extern uint8_t pharosPartition9BssSram1Size;
extern uint64_t pharosPartition9Sram1Start;
extern uint64_t pharosPartition9DataSram1Start;
extern uint64_t pharosPartition9BssSram1Start;
extern uint64_t pharosPartition9Sram1End;
extern uint64_t pharosPartition9DataSram1End;
extern uint64_t pharosPartition9BssSram1End;

extern uint64_t pharosPartition9DataSram2LoadAddr;
extern uint8_t pharosPartition9DataSram2Size;
extern uint8_t pharosPartition9BssSram2Size;
extern uint64_t pharosPartition9Sram2Start;
extern uint64_t pharosPartition9DataSram2Start;
extern uint64_t pharosPartition9BssSram2Start;
extern uint64_t pharosPartition9Sram2End;
extern uint64_t pharosPartition9DataSram2End;
extern uint64_t pharosPartition9BssSram2End;

extern uint64_t pharosPartition9DataSram3LoadAddr;
extern uint8_t pharosPartition9DataSram3Size;
extern uint8_t pharosPartition9BssSram3Size;
extern uint64_t pharosPartition9Sram3Start;
extern uint64_t pharosPartition9DataSram3Start;
extern uint64_t pharosPartition9BssSram3Start;
extern uint64_t pharosPartition9Sram3End;
extern uint64_t pharosPartition9DataSram3End;
extern uint64_t pharosPartition9BssSram3End;

extern uint64_t pharosPartition9DataSramBackupLoadAddr;
extern uint8_t pharosPartition9DataSramBackupSize;
extern uint8_t pharosPartition9BssSramBackupSize;
extern uint64_t pharosPartition9SramBackupStart;
extern uint64_t pharosPartition9DataSramBackupStart;
extern uint64_t pharosPartition9BssSramBackupStart;
extern uint64_t pharosPartition9SramBackupEnd;
extern uint64_t pharosPartition9DataSramBackupEnd;
extern uint64_t pharosPartition9BssSramBackupEnd;

extern uint64_t pharosPartition9DataSdramLoadAddr;
extern uint8_t pharosPartition9DataSdramSize;
extern uint8_t pharosPartition9BssSdramSize;
extern uint64_t pharosPartition9SdramStart;
extern uint64_t pharosPartition9DataSdramStart;
extern uint64_t pharosPartition9BssSdramStart;
extern uint64_t pharosPartition9SdramEnd;
extern uint64_t pharosPartition9DataSdramEnd;
extern uint64_t pharosPartition9BssSdramEnd;

extern uint64_t pharosPartition10DataCcmLoadAddr;
extern uint8_t pharosPartition10DataCcmSize;
extern uint8_t pharosPartition10BssCcmSize;
extern uint64_t pharosPartition10CcmStart;
extern uint64_t pharosPartition10DataCcmStart;
extern uint64_t pharosPartition10BssCcmStart;
extern uint64_t pharosPartition10CcmEnd;
extern uint64_t pharosPartition10DataCcmEnd;
extern uint64_t pharosPartition10BssCcmEnd;

extern uint64_t pharosPartition10DataSram1LoadAddr;
extern uint8_t pharosPartition10DataSram1Size;
extern uint8_t pharosPartition10BssSram1Size;
extern uint64_t pharosPartition10Sram1Start;
extern uint64_t pharosPartition10DataSram1Start;
extern uint64_t pharosPartition10BssSram1Start;
extern uint64_t pharosPartition10Sram1End;
extern uint64_t pharosPartition10DataSram1End;
extern uint64_t pharosPartition10BssSram1End;

extern uint64_t pharosPartition10DataSram2LoadAddr;
extern uint8_t pharosPartition10DataSram2Size;
extern uint8_t pharosPartition10BssSram2Size;
extern uint64_t pharosPartition10Sram2Start;
extern uint64_t pharosPartition10DataSram2Start;
extern uint64_t pharosPartition10BssSram2Start;
extern uint64_t pharosPartition10Sram2End;
extern uint64_t pharosPartition10DataSram2End;
extern uint64_t pharosPartition10BssSram2End;

extern uint64_t pharosPartition10DataSram3LoadAddr;
extern uint8_t pharosPartition10DataSram3Size;
extern uint8_t pharosPartition10BssSram3Size;
extern uint64_t pharosPartition10Sram3Start;
extern uint64_t pharosPartition10DataSram3Start;
extern uint64_t pharosPartition10BssSram3Start;
extern uint64_t pharosPartition10Sram3End;
extern uint64_t pharosPartition10DataSram3End;
extern uint64_t pharosPartition10BssSram3End;

extern uint64_t pharosPartition10DataSramBackupLoadAddr;
extern uint8_t pharosPartition10DataSramBackupSize;
extern uint8_t pharosPartition10BssSramBackupSize;
extern uint64_t pharosPartition10SramBackupStart;
extern uint64_t pharosPartition10DataSramBackupStart;
extern uint64_t pharosPartition10BssSramBackupStart;
extern uint64_t pharosPartition10SramBackupEnd;
extern uint64_t pharosPartition10DataSramBackupEnd;
extern uint64_t pharosPartition10BssSramBackupEnd;

extern uint64_t pharosPartition10DataSdramLoadAddr;
extern uint8_t pharosPartition10DataSdramSize;
extern uint8_t pharosPartition10BssSdramSize;
extern uint64_t pharosPartition10SdramStart;
extern uint64_t pharosPartition10DataSdramStart;
extern uint64_t pharosPartition10BssSdramStart;
extern uint64_t pharosPartition10SdramEnd;
extern uint64_t pharosPartition10DataSdramEnd;
extern uint64_t pharosPartition10BssSdramEnd;

extern uint64_t pharosPartition11DataCcmLoadAddr;
extern uint8_t pharosPartition11DataCcmSize;
extern uint8_t pharosPartition11BssCcmSize;
extern uint64_t pharosPartition11CcmStart;
extern uint64_t pharosPartition11DataCcmStart;
extern uint64_t pharosPartition11BssCcmStart;
extern uint64_t pharosPartition11CcmEnd;
extern uint64_t pharosPartition11DataCcmEnd;
extern uint64_t pharosPartition11BssCcmEnd;

extern uint64_t pharosPartition11DataSram1LoadAddr;
extern uint8_t pharosPartition11DataSram1Size;
extern uint8_t pharosPartition11BssSram1Size;
extern uint64_t pharosPartition11Sram1Start;
extern uint64_t pharosPartition11DataSram1Start;
extern uint64_t pharosPartition11BssSram1Start;
extern uint64_t pharosPartition11Sram1End;
extern uint64_t pharosPartition11DataSram1End;
extern uint64_t pharosPartition11BssSram1End;

extern uint64_t pharosPartition11DataSram2LoadAddr;
extern uint8_t pharosPartition11DataSram2Size;
extern uint8_t pharosPartition11BssSram2Size;
extern uint64_t pharosPartition11Sram2Start;
extern uint64_t pharosPartition11DataSram2Start;
extern uint64_t pharosPartition11BssSram2Start;
extern uint64_t pharosPartition11Sram2End;
extern uint64_t pharosPartition11DataSram2End;
extern uint64_t pharosPartition11BssSram2End;

extern uint64_t pharosPartition11DataSram3LoadAddr;
extern uint8_t pharosPartition11DataSram3Size;
extern uint8_t pharosPartition11BssSram3Size;
extern uint64_t pharosPartition11Sram3Start;
extern uint64_t pharosPartition11DataSram3Start;
extern uint64_t pharosPartition11BssSram3Start;
extern uint64_t pharosPartition11Sram3End;
extern uint64_t pharosPartition11DataSram3End;
extern uint64_t pharosPartition11BssSram3End;

extern uint64_t pharosPartition11DataSramBackupLoadAddr;
extern uint8_t pharosPartition11DataSramBackupSize;
extern uint8_t pharosPartition11BssSramBackupSize;
extern uint64_t pharosPartition11SramBackupStart;
extern uint64_t pharosPartition11DataSramBackupStart;
extern uint64_t pharosPartition11BssSramBackupStart;
extern uint64_t pharosPartition11SramBackupEnd;
extern uint64_t pharosPartition11DataSramBackupEnd;
extern uint64_t pharosPartition11BssSramBackupEnd;

extern uint64_t pharosPartition11DataSdramLoadAddr;
extern uint8_t pharosPartition11DataSdramSize;
extern uint8_t pharosPartition11BssSdramSize;
extern uint64_t pharosPartition11SdramStart;
extern uint64_t pharosPartition11DataSdramStart;
extern uint64_t pharosPartition11BssSdramStart;
extern uint64_t pharosPartition11SdramEnd;
extern uint64_t pharosPartition11DataSdramEnd;
extern uint64_t pharosPartition11BssSdramEnd;

extern uint64_t pharosPartition12DataCcmLoadAddr;
extern uint8_t pharosPartition12DataCcmSize;
extern uint8_t pharosPartition12BssCcmSize;
extern uint64_t pharosPartition12CcmStart;
extern uint64_t pharosPartition12DataCcmStart;
extern uint64_t pharosPartition12BssCcmStart;
extern uint64_t pharosPartition12CcmEnd;
extern uint64_t pharosPartition12DataCcmEnd;
extern uint64_t pharosPartition12BssCcmEnd;

extern uint64_t pharosPartition12DataSram1LoadAddr;
extern uint8_t pharosPartition12DataSram1Size;
extern uint8_t pharosPartition12BssSram1Size;
extern uint64_t pharosPartition12Sram1Start;
extern uint64_t pharosPartition12DataSram1Start;
extern uint64_t pharosPartition12BssSram1Start;
extern uint64_t pharosPartition12Sram1End;
extern uint64_t pharosPartition12DataSram1End;
extern uint64_t pharosPartition12BssSram1End;

extern uint64_t pharosPartition12DataSram2LoadAddr;
extern uint8_t pharosPartition12DataSram2Size;
extern uint8_t pharosPartition12BssSram2Size;
extern uint64_t pharosPartition12Sram2Start;
extern uint64_t pharosPartition12DataSram2Start;
extern uint64_t pharosPartition12BssSram2Start;
extern uint64_t pharosPartition12Sram2End;
extern uint64_t pharosPartition12DataSram2End;
extern uint64_t pharosPartition12BssSram2End;

extern uint64_t pharosPartition12DataSram3LoadAddr;
extern uint8_t pharosPartition12DataSram3Size;
extern uint8_t pharosPartition12BssSram3Size;
extern uint64_t pharosPartition12Sram3Start;
extern uint64_t pharosPartition12DataSram3Start;
extern uint64_t pharosPartition12BssSram3Start;
extern uint64_t pharosPartition12Sram3End;
extern uint64_t pharosPartition12DataSram3End;
extern uint64_t pharosPartition12BssSram3End;

extern uint64_t pharosPartition12DataSramBackupLoadAddr;
extern uint8_t pharosPartition12DataSramBackupSize;
extern uint8_t pharosPartition12BssSramBackupSize;
extern uint64_t pharosPartition12SramBackupStart;
extern uint64_t pharosPartition12DataSramBackupStart;
extern uint64_t pharosPartition12BssSramBackupStart;
extern uint64_t pharosPartition12SramBackupEnd;
extern uint64_t pharosPartition12DataSramBackupEnd;
extern uint64_t pharosPartition12BssSramBackupEnd;

extern uint64_t pharosPartition12DataSdramLoadAddr;
extern uint8_t pharosPartition12DataSdramSize;
extern uint8_t pharosPartition12BssSdramSize;
extern uint64_t pharosPartition12SdramStart;
extern uint64_t pharosPartition12DataSdramStart;
extern uint64_t pharosPartition12BssSdramStart;
extern uint64_t pharosPartition12SdramEnd;
extern uint64_t pharosPartition12DataSdramEnd;
extern uint64_t pharosPartition12BssSdramEnd;

extern uint64_t pharosPartition13DataCcmLoadAddr;
extern uint8_t pharosPartition13DataCcmSize;
extern uint8_t pharosPartition13BssCcmSize;
extern uint64_t pharosPartition13CcmStart;
extern uint64_t pharosPartition13DataCcmStart;
extern uint64_t pharosPartition13BssCcmStart;
extern uint64_t pharosPartition13CcmEnd;
extern uint64_t pharosPartition13DataCcmEnd;
extern uint64_t pharosPartition13BssCcmEnd;

extern uint64_t pharosPartition13DataSram1LoadAddr;
extern uint8_t pharosPartition13DataSram1Size;
extern uint8_t pharosPartition13BssSram1Size;
extern uint64_t pharosPartition13Sram1Start;
extern uint64_t pharosPartition13DataSram1Start;
extern uint64_t pharosPartition13BssSram1Start;
extern uint64_t pharosPartition13Sram1End;
extern uint64_t pharosPartition13DataSram1End;
extern uint64_t pharosPartition13BssSram1End;

extern uint64_t pharosPartition13DataSram2LoadAddr;
extern uint8_t pharosPartition13DataSram2Size;
extern uint8_t pharosPartition13BssSram2Size;
extern uint64_t pharosPartition13Sram2Start;
extern uint64_t pharosPartition13DataSram2Start;
extern uint64_t pharosPartition13BssSram2Start;
extern uint64_t pharosPartition13Sram2End;
extern uint64_t pharosPartition13DataSram2End;
extern uint64_t pharosPartition13BssSram2End;

extern uint64_t pharosPartition13DataSram3LoadAddr;
extern uint8_t pharosPartition13DataSram3Size;
extern uint8_t pharosPartition13BssSram3Size;
extern uint64_t pharosPartition13Sram3Start;
extern uint64_t pharosPartition13DataSram3Start;
extern uint64_t pharosPartition13BssSram3Start;
extern uint64_t pharosPartition13Sram3End;
extern uint64_t pharosPartition13DataSram3End;
extern uint64_t pharosPartition13BssSram3End;

extern uint64_t pharosPartition13DataSramBackupLoadAddr;
extern uint8_t pharosPartition13DataSramBackupSize;
extern uint8_t pharosPartition13BssSramBackupSize;
extern uint64_t pharosPartition13SramBackupStart;
extern uint64_t pharosPartition13DataSramBackupStart;
extern uint64_t pharosPartition13BssSramBackupStart;
extern uint64_t pharosPartition13SramBackupEnd;
extern uint64_t pharosPartition13DataSramBackupEnd;
extern uint64_t pharosPartition13BssSramBackupEnd;

extern uint64_t pharosPartition13DataSdramLoadAddr;
extern uint8_t pharosPartition13DataSdramSize;
extern uint8_t pharosPartition13BssSdramSize;
extern uint64_t pharosPartition13SdramStart;
extern uint64_t pharosPartition13DataSdramStart;
extern uint64_t pharosPartition13BssSdramStart;
extern uint64_t pharosPartition13SdramEnd;
extern uint64_t pharosPartition13DataSdramEnd;
extern uint64_t pharosPartition13BssSdramEnd;

extern uint64_t pharosPartition14DataCcmLoadAddr;
extern uint8_t pharosPartition14DataCcmSize;
extern uint8_t pharosPartition14BssCcmSize;
extern uint64_t pharosPartition14CcmStart;
extern uint64_t pharosPartition14DataCcmStart;
extern uint64_t pharosPartition14BssCcmStart;
extern uint64_t pharosPartition14CcmEnd;
extern uint64_t pharosPartition14DataCcmEnd;
extern uint64_t pharosPartition14BssCcmEnd;

extern uint64_t pharosPartition14DataSram1LoadAddr;
extern uint8_t pharosPartition14DataSram1Size;
extern uint8_t pharosPartition14BssSram1Size;
extern uint64_t pharosPartition14Sram1Start;
extern uint64_t pharosPartition14DataSram1Start;
extern uint64_t pharosPartition14BssSram1Start;
extern uint64_t pharosPartition14Sram1End;
extern uint64_t pharosPartition14DataSram1End;
extern uint64_t pharosPartition14BssSram1End;

extern uint64_t pharosPartition14DataSram2LoadAddr;
extern uint8_t pharosPartition14DataSram2Size;
extern uint8_t pharosPartition14BssSram2Size;
extern uint64_t pharosPartition14Sram2Start;
extern uint64_t pharosPartition14DataSram2Start;
extern uint64_t pharosPartition14BssSram2Start;
extern uint64_t pharosPartition14Sram2End;
extern uint64_t pharosPartition14DataSram2End;
extern uint64_t pharosPartition14BssSram2End;

extern uint64_t pharosPartition14DataSram3LoadAddr;
extern uint8_t pharosPartition14DataSram3Size;
extern uint8_t pharosPartition14BssSram3Size;
extern uint64_t pharosPartition14Sram3Start;
extern uint64_t pharosPartition14DataSram3Start;
extern uint64_t pharosPartition14BssSram3Start;
extern uint64_t pharosPartition14Sram3End;
extern uint64_t pharosPartition14DataSram3End;
extern uint64_t pharosPartition14BssSram3End;

extern uint64_t pharosPartition14DataSramBackupLoadAddr;
extern uint8_t pharosPartition14DataSramBackupSize;
extern uint8_t pharosPartition14BssSramBackupSize;
extern uint64_t pharosPartition14SramBackupStart;
extern uint64_t pharosPartition14DataSramBackupStart;
extern uint64_t pharosPartition14BssSramBackupStart;
extern uint64_t pharosPartition14SramBackupEnd;
extern uint64_t pharosPartition14DataSramBackupEnd;
extern uint64_t pharosPartition14BssSramBackupEnd;

extern uint64_t pharosPartition14DataSdramLoadAddr;
extern uint8_t pharosPartition14DataSdramSize;
extern uint8_t pharosPartition14BssSdramSize;
extern uint64_t pharosPartition14SdramStart;
extern uint64_t pharosPartition14DataSdramStart;
extern uint64_t pharosPartition14BssSdramStart;
extern uint64_t pharosPartition14SdramEnd;
extern uint64_t pharosPartition14DataSdramEnd;
extern uint64_t pharosPartition14BssSdramEnd;

extern uint64_t pharosPartition15DataCcmLoadAddr;
extern uint8_t pharosPartition15DataCcmSize;
extern uint8_t pharosPartition15BssCcmSize;
extern uint64_t pharosPartition15CcmStart;
extern uint64_t pharosPartition15DataCcmStart;
extern uint64_t pharosPartition15BssCcmStart;
extern uint64_t pharosPartition15CcmEnd;
extern uint64_t pharosPartition15DataCcmEnd;
extern uint64_t pharosPartition15BssCcmEnd;

extern uint64_t pharosPartition15DataSram1LoadAddr;
extern uint8_t pharosPartition15DataSram1Size;
extern uint8_t pharosPartition15BssSram1Size;
extern uint64_t pharosPartition15Sram1Start;
extern uint64_t pharosPartition15DataSram1Start;
extern uint64_t pharosPartition15BssSram1Start;
extern uint64_t pharosPartition15Sram1End;
extern uint64_t pharosPartition15DataSram1End;
extern uint64_t pharosPartition15BssSram1End;

extern uint64_t pharosPartition15DataSram2LoadAddr;
extern uint8_t pharosPartition15DataSram2Size;
extern uint8_t pharosPartition15BssSram2Size;
extern uint64_t pharosPartition15Sram2Start;
extern uint64_t pharosPartition15DataSram2Start;
extern uint64_t pharosPartition15BssSram2Start;
extern uint64_t pharosPartition15Sram2End;
extern uint64_t pharosPartition15DataSram2End;
extern uint64_t pharosPartition15BssSram2End;

extern uint64_t pharosPartition15DataSram3LoadAddr;
extern uint8_t pharosPartition15DataSram3Size;
extern uint8_t pharosPartition15BssSram3Size;
extern uint64_t pharosPartition15Sram3Start;
extern uint64_t pharosPartition15DataSram3Start;
extern uint64_t pharosPartition15BssSram3Start;
extern uint64_t pharosPartition15Sram3End;
extern uint64_t pharosPartition15DataSram3End;
extern uint64_t pharosPartition15BssSram3End;

extern uint64_t pharosPartition15DataSramBackupLoadAddr;
extern uint8_t pharosPartition15DataSramBackupSize;
extern uint8_t pharosPartition15BssSramBackupSize;
extern uint64_t pharosPartition15SramBackupStart;
extern uint64_t pharosPartition15DataSramBackupStart;
extern uint64_t pharosPartition15BssSramBackupStart;
extern uint64_t pharosPartition15SramBackupEnd;
extern uint64_t pharosPartition15DataSramBackupEnd;
extern uint64_t pharosPartition15BssSramBackupEnd;

extern uint64_t pharosPartition15DataSdramLoadAddr;
extern uint8_t pharosPartition15DataSdramSize;
extern uint8_t pharosPartition15BssSdramSize;
extern uint64_t pharosPartition15SdramStart;
extern uint64_t pharosPartition15DataSdramStart;
extern uint64_t pharosPartition15BssSdramStart;
extern uint64_t pharosPartition15SdramEnd;
extern uint64_t pharosPartition15DataSdramEnd;
extern uint64_t pharosPartition15BssSdramEnd;

extern uint64_t pharosPartition16DataCcmLoadAddr;
extern uint8_t pharosPartition16DataCcmSize;
extern uint8_t pharosPartition16BssCcmSize;
extern uint64_t pharosPartition16CcmStart;
extern uint64_t pharosPartition16DataCcmStart;
extern uint64_t pharosPartition16BssCcmStart;
extern uint64_t pharosPartition16CcmEnd;
extern uint64_t pharosPartition16DataCcmEnd;
extern uint64_t pharosPartition16BssCcmEnd;

extern uint64_t pharosPartition16DataSram1LoadAddr;
extern uint8_t pharosPartition16DataSram1Size;
extern uint8_t pharosPartition16BssSram1Size;
extern uint64_t pharosPartition16Sram1Start;
extern uint64_t pharosPartition16DataSram1Start;
extern uint64_t pharosPartition16BssSram1Start;
extern uint64_t pharosPartition16Sram1End;
extern uint64_t pharosPartition16DataSram1End;
extern uint64_t pharosPartition16BssSram1End;

extern uint64_t pharosPartition16DataSram2LoadAddr;
extern uint8_t pharosPartition16DataSram2Size;
extern uint8_t pharosPartition16BssSram2Size;
extern uint64_t pharosPartition16Sram2Start;
extern uint64_t pharosPartition16DataSram2Start;
extern uint64_t pharosPartition16BssSram2Start;
extern uint64_t pharosPartition16Sram2End;
extern uint64_t pharosPartition16DataSram2End;
extern uint64_t pharosPartition16BssSram2End;

extern uint64_t pharosPartition16DataSram3LoadAddr;
extern uint8_t pharosPartition16DataSram3Size;
extern uint8_t pharosPartition16BssSram3Size;
extern uint64_t pharosPartition16Sram3Start;
extern uint64_t pharosPartition16DataSram3Start;
extern uint64_t pharosPartition16BssSram3Start;
extern uint64_t pharosPartition16Sram3End;
extern uint64_t pharosPartition16DataSram3End;
extern uint64_t pharosPartition16BssSram3End;

extern uint64_t pharosPartition16DataSramBackupLoadAddr;
extern uint8_t pharosPartition16DataSramBackupSize;
extern uint8_t pharosPartition16BssSramBackupSize;
extern uint64_t pharosPartition16SramBackupStart;
extern uint64_t pharosPartition16DataSramBackupStart;
extern uint64_t pharosPartition16BssSramBackupStart;
extern uint64_t pharosPartition16SramBackupEnd;
extern uint64_t pharosPartition16DataSramBackupEnd;
extern uint64_t pharosPartition16BssSramBackupEnd;

extern uint64_t pharosPartition16DataSdramLoadAddr;
extern uint8_t pharosPartition16DataSdramSize;
extern uint8_t pharosPartition16BssSdramSize;
extern uint64_t pharosPartition16SdramStart;
extern uint64_t pharosPartition16DataSdramStart;
extern uint64_t pharosPartition16BssSdramStart;
extern uint64_t pharosPartition16SdramEnd;
extern uint64_t pharosPartition16DataSdramEnd;
extern uint64_t pharosPartition16BssSdramEnd;

extern uint64_t pharosPartition17DataCcmLoadAddr;
extern uint8_t pharosPartition17DataCcmSize;
extern uint8_t pharosPartition17BssCcmSize;
extern uint64_t pharosPartition17CcmStart;
extern uint64_t pharosPartition17DataCcmStart;
extern uint64_t pharosPartition17BssCcmStart;
extern uint64_t pharosPartition17CcmEnd;
extern uint64_t pharosPartition17DataCcmEnd;
extern uint64_t pharosPartition17BssCcmEnd;

extern uint64_t pharosPartition17DataSram1LoadAddr;
extern uint8_t pharosPartition17DataSram1Size;
extern uint8_t pharosPartition17BssSram1Size;
extern uint64_t pharosPartition17Sram1Start;
extern uint64_t pharosPartition17DataSram1Start;
extern uint64_t pharosPartition17BssSram1Start;
extern uint64_t pharosPartition17Sram1End;
extern uint64_t pharosPartition17DataSram1End;
extern uint64_t pharosPartition17BssSram1End;

extern uint64_t pharosPartition17DataSram2LoadAddr;
extern uint8_t pharosPartition17DataSram2Size;
extern uint8_t pharosPartition17BssSram2Size;
extern uint64_t pharosPartition17Sram2Start;
extern uint64_t pharosPartition17DataSram2Start;
extern uint64_t pharosPartition17BssSram2Start;
extern uint64_t pharosPartition17Sram2End;
extern uint64_t pharosPartition17DataSram2End;
extern uint64_t pharosPartition17BssSram2End;

extern uint64_t pharosPartition17DataSram3LoadAddr;
extern uint8_t pharosPartition17DataSram3Size;
extern uint8_t pharosPartition17BssSram3Size;
extern uint64_t pharosPartition17Sram3Start;
extern uint64_t pharosPartition17DataSram3Start;
extern uint64_t pharosPartition17BssSram3Start;
extern uint64_t pharosPartition17Sram3End;
extern uint64_t pharosPartition17DataSram3End;
extern uint64_t pharosPartition17BssSram3End;

extern uint64_t pharosPartition17DataSramBackupLoadAddr;
extern uint8_t pharosPartition17DataSramBackupSize;
extern uint8_t pharosPartition17BssSramBackupSize;
extern uint64_t pharosPartition17SramBackupStart;
extern uint64_t pharosPartition17DataSramBackupStart;
extern uint64_t pharosPartition17BssSramBackupStart;
extern uint64_t pharosPartition17SramBackupEnd;
extern uint64_t pharosPartition17DataSramBackupEnd;
extern uint64_t pharosPartition17BssSramBackupEnd;

extern uint64_t pharosPartition17DataSdramLoadAddr;
extern uint8_t pharosPartition17DataSdramSize;
extern uint8_t pharosPartition17BssSdramSize;
extern uint64_t pharosPartition17SdramStart;
extern uint64_t pharosPartition17DataSdramStart;
extern uint64_t pharosPartition17BssSdramStart;
extern uint64_t pharosPartition17SdramEnd;
extern uint64_t pharosPartition17DataSdramEnd;
extern uint64_t pharosPartition17BssSdramEnd;

extern uint64_t pharosPartition18DataCcmLoadAddr;
extern uint8_t pharosPartition18DataCcmSize;
extern uint8_t pharosPartition18BssCcmSize;
extern uint64_t pharosPartition18CcmStart;
extern uint64_t pharosPartition18DataCcmStart;
extern uint64_t pharosPartition18BssCcmStart;
extern uint64_t pharosPartition18CcmEnd;
extern uint64_t pharosPartition18DataCcmEnd;
extern uint64_t pharosPartition18BssCcmEnd;

extern uint64_t pharosPartition18DataSram1LoadAddr;
extern uint8_t pharosPartition18DataSram1Size;
extern uint8_t pharosPartition18BssSram1Size;
extern uint64_t pharosPartition18Sram1Start;
extern uint64_t pharosPartition18DataSram1Start;
extern uint64_t pharosPartition18BssSram1Start;
extern uint64_t pharosPartition18Sram1End;
extern uint64_t pharosPartition18DataSram1End;
extern uint64_t pharosPartition18BssSram1End;

extern uint64_t pharosPartition18DataSram2LoadAddr;
extern uint8_t pharosPartition18DataSram2Size;
extern uint8_t pharosPartition18BssSram2Size;
extern uint64_t pharosPartition18Sram2Start;
extern uint64_t pharosPartition18DataSram2Start;
extern uint64_t pharosPartition18BssSram2Start;
extern uint64_t pharosPartition18Sram2End;
extern uint64_t pharosPartition18DataSram2End;
extern uint64_t pharosPartition18BssSram2End;

extern uint64_t pharosPartition18DataSram3LoadAddr;
extern uint8_t pharosPartition18DataSram3Size;
extern uint8_t pharosPartition18BssSram3Size;
extern uint64_t pharosPartition18Sram3Start;
extern uint64_t pharosPartition18DataSram3Start;
extern uint64_t pharosPartition18BssSram3Start;
extern uint64_t pharosPartition18Sram3End;
extern uint64_t pharosPartition18DataSram3End;
extern uint64_t pharosPartition18BssSram3End;

extern uint64_t pharosPartition18DataSramBackupLoadAddr;
extern uint8_t pharosPartition18DataSramBackupSize;
extern uint8_t pharosPartition18BssSramBackupSize;
extern uint64_t pharosPartition18SramBackupStart;
extern uint64_t pharosPartition18DataSramBackupStart;
extern uint64_t pharosPartition18BssSramBackupStart;
extern uint64_t pharosPartition18SramBackupEnd;
extern uint64_t pharosPartition18DataSramBackupEnd;
extern uint64_t pharosPartition18BssSramBackupEnd;

extern uint64_t pharosPartition18DataSdramLoadAddr;
extern uint8_t pharosPartition18DataSdramSize;
extern uint8_t pharosPartition18BssSdramSize;
extern uint64_t pharosPartition18SdramStart;
extern uint64_t pharosPartition18DataSdramStart;
extern uint64_t pharosPartition18BssSdramStart;
extern uint64_t pharosPartition18SdramEnd;
extern uint64_t pharosPartition18DataSdramEnd;
extern uint64_t pharosPartition18BssSdramEnd;

extern uint64_t pharosPartition19DataCcmLoadAddr;
extern uint8_t pharosPartition19DataCcmSize;
extern uint8_t pharosPartition19BssCcmSize;
extern uint64_t pharosPartition19CcmStart;
extern uint64_t pharosPartition19DataCcmStart;
extern uint64_t pharosPartition19BssCcmStart;
extern uint64_t pharosPartition19CcmEnd;
extern uint64_t pharosPartition19DataCcmEnd;
extern uint64_t pharosPartition19BssCcmEnd;

extern uint64_t pharosPartition19DataSram1LoadAddr;
extern uint8_t pharosPartition19DataSram1Size;
extern uint8_t pharosPartition19BssSram1Size;
extern uint64_t pharosPartition19Sram1Start;
extern uint64_t pharosPartition19DataSram1Start;
extern uint64_t pharosPartition19BssSram1Start;
extern uint64_t pharosPartition19Sram1End;
extern uint64_t pharosPartition19DataSram1End;
extern uint64_t pharosPartition19BssSram1End;

extern uint64_t pharosPartition19DataSram2LoadAddr;
extern uint8_t pharosPartition19DataSram2Size;
extern uint8_t pharosPartition19BssSram2Size;
extern uint64_t pharosPartition19Sram2Start;
extern uint64_t pharosPartition19DataSram2Start;
extern uint64_t pharosPartition19BssSram2Start;
extern uint64_t pharosPartition19Sram2End;
extern uint64_t pharosPartition19DataSram2End;
extern uint64_t pharosPartition19BssSram2End;

extern uint64_t pharosPartition19DataSram3LoadAddr;
extern uint8_t pharosPartition19DataSram3Size;
extern uint8_t pharosPartition19BssSram3Size;
extern uint64_t pharosPartition19Sram3Start;
extern uint64_t pharosPartition19DataSram3Start;
extern uint64_t pharosPartition19BssSram3Start;
extern uint64_t pharosPartition19Sram3End;
extern uint64_t pharosPartition19DataSram3End;
extern uint64_t pharosPartition19BssSram3End;

extern uint64_t pharosPartition19DataSramBackupLoadAddr;
extern uint8_t pharosPartition19DataSramBackupSize;
extern uint8_t pharosPartition19BssSramBackupSize;
extern uint64_t pharosPartition19SramBackupStart;
extern uint64_t pharosPartition19DataSramBackupStart;
extern uint64_t pharosPartition19BssSramBackupStart;
extern uint64_t pharosPartition19SramBackupEnd;
extern uint64_t pharosPartition19DataSramBackupEnd;
extern uint64_t pharosPartition19BssSramBackupEnd;

extern uint64_t pharosPartition19DataSdramLoadAddr;
extern uint8_t pharosPartition19DataSdramSize;
extern uint8_t pharosPartition19BssSdramSize;
extern uint64_t pharosPartition19SdramStart;
extern uint64_t pharosPartition19DataSdramStart;
extern uint64_t pharosPartition19BssSdramStart;
extern uint64_t pharosPartition19SdramEnd;
extern uint64_t pharosPartition19DataSdramEnd;
extern uint64_t pharosPartition19BssSdramEnd;

extern uint64_t pharosPartition20DataCcmLoadAddr;
extern uint8_t pharosPartition20DataCcmSize;
extern uint8_t pharosPartition20BssCcmSize;
extern uint64_t pharosPartition20CcmStart;
extern uint64_t pharosPartition20DataCcmStart;
extern uint64_t pharosPartition20BssCcmStart;
extern uint64_t pharosPartition20CcmEnd;
extern uint64_t pharosPartition20DataCcmEnd;
extern uint64_t pharosPartition20BssCcmEnd;

extern uint64_t pharosPartition20DataSram1LoadAddr;
extern uint8_t pharosPartition20DataSram1Size;
extern uint8_t pharosPartition20BssSram1Size;
extern uint64_t pharosPartition20Sram1Start;
extern uint64_t pharosPartition20DataSram1Start;
extern uint64_t pharosPartition20BssSram1Start;
extern uint64_t pharosPartition20Sram1End;
extern uint64_t pharosPartition20DataSram1End;
extern uint64_t pharosPartition20BssSram1End;

extern uint64_t pharosPartition20DataSram2LoadAddr;
extern uint8_t pharosPartition20DataSram2Size;
extern uint8_t pharosPartition20BssSram2Size;
extern uint64_t pharosPartition20Sram2Start;
extern uint64_t pharosPartition20DataSram2Start;
extern uint64_t pharosPartition20BssSram2Start;
extern uint64_t pharosPartition20Sram2End;
extern uint64_t pharosPartition20DataSram2End;
extern uint64_t pharosPartition20BssSram2End;

extern uint64_t pharosPartition20DataSram3LoadAddr;
extern uint8_t pharosPartition20DataSram3Size;
extern uint8_t pharosPartition20BssSram3Size;
extern uint64_t pharosPartition20Sram3Start;
extern uint64_t pharosPartition20DataSram3Start;
extern uint64_t pharosPartition20BssSram3Start;
extern uint64_t pharosPartition20Sram3End;
extern uint64_t pharosPartition20DataSram3End;
extern uint64_t pharosPartition20BssSram3End;

extern uint64_t pharosPartition20DataSramBackupLoadAddr;
extern uint8_t pharosPartition20DataSramBackupSize;
extern uint8_t pharosPartition20BssSramBackupSize;
extern uint64_t pharosPartition20SramBackupStart;
extern uint64_t pharosPartition20DataSramBackupStart;
extern uint64_t pharosPartition20BssSramBackupStart;
extern uint64_t pharosPartition20SramBackupEnd;
extern uint64_t pharosPartition20DataSramBackupEnd;
extern uint64_t pharosPartition20BssSramBackupEnd;

extern uint64_t pharosPartition20DataSdramLoadAddr;
extern uint8_t pharosPartition20DataSdramSize;
extern uint8_t pharosPartition20BssSdramSize;
extern uint64_t pharosPartition20SdramStart;
extern uint64_t pharosPartition20DataSdramStart;
extern uint64_t pharosPartition20BssSdramStart;
extern uint64_t pharosPartition20SdramEnd;
extern uint64_t pharosPartition20DataSdramEnd;
extern uint64_t pharosPartition20BssSdramEnd;

extern uint64_t pharosPartition21DataCcmLoadAddr;
extern uint8_t pharosPartition21DataCcmSize;
extern uint8_t pharosPartition21BssCcmSize;
extern uint64_t pharosPartition21CcmStart;
extern uint64_t pharosPartition21DataCcmStart;
extern uint64_t pharosPartition21BssCcmStart;
extern uint64_t pharosPartition21CcmEnd;
extern uint64_t pharosPartition21DataCcmEnd;
extern uint64_t pharosPartition21BssCcmEnd;

extern uint64_t pharosPartition21DataSram1LoadAddr;
extern uint8_t pharosPartition21DataSram1Size;
extern uint8_t pharosPartition21BssSram1Size;
extern uint64_t pharosPartition21Sram1Start;
extern uint64_t pharosPartition21DataSram1Start;
extern uint64_t pharosPartition21BssSram1Start;
extern uint64_t pharosPartition21Sram1End;
extern uint64_t pharosPartition21DataSram1End;
extern uint64_t pharosPartition21BssSram1End;

extern uint64_t pharosPartition21DataSram2LoadAddr;
extern uint8_t pharosPartition21DataSram2Size;
extern uint8_t pharosPartition21BssSram2Size;
extern uint64_t pharosPartition21Sram2Start;
extern uint64_t pharosPartition21DataSram2Start;
extern uint64_t pharosPartition21BssSram2Start;
extern uint64_t pharosPartition21Sram2End;
extern uint64_t pharosPartition21DataSram2End;
extern uint64_t pharosPartition21BssSram2End;

extern uint64_t pharosPartition21DataSram3LoadAddr;
extern uint8_t pharosPartition21DataSram3Size;
extern uint8_t pharosPartition21BssSram3Size;
extern uint64_t pharosPartition21Sram3Start;
extern uint64_t pharosPartition21DataSram3Start;
extern uint64_t pharosPartition21BssSram3Start;
extern uint64_t pharosPartition21Sram3End;
extern uint64_t pharosPartition21DataSram3End;
extern uint64_t pharosPartition21BssSram3End;

extern uint64_t pharosPartition21DataSramBackupLoadAddr;
extern uint8_t pharosPartition21DataSramBackupSize;
extern uint8_t pharosPartition21BssSramBackupSize;
extern uint64_t pharosPartition21SramBackupStart;
extern uint64_t pharosPartition21DataSramBackupStart;
extern uint64_t pharosPartition21BssSramBackupStart;
extern uint64_t pharosPartition21SramBackupEnd;
extern uint64_t pharosPartition21DataSramBackupEnd;
extern uint64_t pharosPartition21BssSramBackupEnd;

extern uint64_t pharosPartition21DataSdramLoadAddr;
extern uint8_t pharosPartition21DataSdramSize;
extern uint8_t pharosPartition21BssSdramSize;
extern uint64_t pharosPartition21SdramStart;
extern uint64_t pharosPartition21DataSdramStart;
extern uint64_t pharosPartition21BssSdramStart;
extern uint64_t pharosPartition21SdramEnd;
extern uint64_t pharosPartition21DataSdramEnd;
extern uint64_t pharosPartition21BssSdramEnd;

extern uint64_t pharosPartition22DataCcmLoadAddr;
extern uint8_t pharosPartition22DataCcmSize;
extern uint8_t pharosPartition22BssCcmSize;
extern uint64_t pharosPartition22CcmStart;
extern uint64_t pharosPartition22DataCcmStart;
extern uint64_t pharosPartition22BssCcmStart;
extern uint64_t pharosPartition22CcmEnd;
extern uint64_t pharosPartition22DataCcmEnd;
extern uint64_t pharosPartition22BssCcmEnd;

extern uint64_t pharosPartition22DataSram1LoadAddr;
extern uint8_t pharosPartition22DataSram1Size;
extern uint8_t pharosPartition22BssSram1Size;
extern uint64_t pharosPartition22Sram1Start;
extern uint64_t pharosPartition22DataSram1Start;
extern uint64_t pharosPartition22BssSram1Start;
extern uint64_t pharosPartition22Sram1End;
extern uint64_t pharosPartition22DataSram1End;
extern uint64_t pharosPartition22BssSram1End;

extern uint64_t pharosPartition22DataSram2LoadAddr;
extern uint8_t pharosPartition22DataSram2Size;
extern uint8_t pharosPartition22BssSram2Size;
extern uint64_t pharosPartition22Sram2Start;
extern uint64_t pharosPartition22DataSram2Start;
extern uint64_t pharosPartition22BssSram2Start;
extern uint64_t pharosPartition22Sram2End;
extern uint64_t pharosPartition22DataSram2End;
extern uint64_t pharosPartition22BssSram2End;

extern uint64_t pharosPartition22DataSram3LoadAddr;
extern uint8_t pharosPartition22DataSram3Size;
extern uint8_t pharosPartition22BssSram3Size;
extern uint64_t pharosPartition22Sram3Start;
extern uint64_t pharosPartition22DataSram3Start;
extern uint64_t pharosPartition22BssSram3Start;
extern uint64_t pharosPartition22Sram3End;
extern uint64_t pharosPartition22DataSram3End;
extern uint64_t pharosPartition22BssSram3End;

extern uint64_t pharosPartition22DataSramBackupLoadAddr;
extern uint8_t pharosPartition22DataSramBackupSize;
extern uint8_t pharosPartition22BssSramBackupSize;
extern uint64_t pharosPartition22SramBackupStart;
extern uint64_t pharosPartition22DataSramBackupStart;
extern uint64_t pharosPartition22BssSramBackupStart;
extern uint64_t pharosPartition22SramBackupEnd;
extern uint64_t pharosPartition22DataSramBackupEnd;
extern uint64_t pharosPartition22BssSramBackupEnd;

extern uint64_t pharosPartition22DataSdramLoadAddr;
extern uint8_t pharosPartition22DataSdramSize;
extern uint8_t pharosPartition22BssSdramSize;
extern uint64_t pharosPartition22SdramStart;
extern uint64_t pharosPartition22DataSdramStart;
extern uint64_t pharosPartition22BssSdramStart;
extern uint64_t pharosPartition22SdramEnd;
extern uint64_t pharosPartition22DataSdramEnd;
extern uint64_t pharosPartition22BssSdramEnd;

extern uint64_t pharosPartition23DataCcmLoadAddr;
extern uint8_t pharosPartition23DataCcmSize;
extern uint8_t pharosPartition23BssCcmSize;
extern uint64_t pharosPartition23CcmStart;
extern uint64_t pharosPartition23DataCcmStart;
extern uint64_t pharosPartition23BssCcmStart;
extern uint64_t pharosPartition23CcmEnd;
extern uint64_t pharosPartition23DataCcmEnd;
extern uint64_t pharosPartition23BssCcmEnd;

extern uint64_t pharosPartition23DataSram1LoadAddr;
extern uint8_t pharosPartition23DataSram1Size;
extern uint8_t pharosPartition23BssSram1Size;
extern uint64_t pharosPartition23Sram1Start;
extern uint64_t pharosPartition23DataSram1Start;
extern uint64_t pharosPartition23BssSram1Start;
extern uint64_t pharosPartition23Sram1End;
extern uint64_t pharosPartition23DataSram1End;
extern uint64_t pharosPartition23BssSram1End;

extern uint64_t pharosPartition23DataSram2LoadAddr;
extern uint8_t pharosPartition23DataSram2Size;
extern uint8_t pharosPartition23BssSram2Size;
extern uint64_t pharosPartition23Sram2Start;
extern uint64_t pharosPartition23DataSram2Start;
extern uint64_t pharosPartition23BssSram2Start;
extern uint64_t pharosPartition23Sram2End;
extern uint64_t pharosPartition23DataSram2End;
extern uint64_t pharosPartition23BssSram2End;

extern uint64_t pharosPartition23DataSram3LoadAddr;
extern uint8_t pharosPartition23DataSram3Size;
extern uint8_t pharosPartition23BssSram3Size;
extern uint64_t pharosPartition23Sram3Start;
extern uint64_t pharosPartition23DataSram3Start;
extern uint64_t pharosPartition23BssSram3Start;
extern uint64_t pharosPartition23Sram3End;
extern uint64_t pharosPartition23DataSram3End;
extern uint64_t pharosPartition23BssSram3End;

extern uint64_t pharosPartition23DataSramBackupLoadAddr;
extern uint8_t pharosPartition23DataSramBackupSize;
extern uint8_t pharosPartition23BssSramBackupSize;
extern uint64_t pharosPartition23SramBackupStart;
extern uint64_t pharosPartition23DataSramBackupStart;
extern uint64_t pharosPartition23BssSramBackupStart;
extern uint64_t pharosPartition23SramBackupEnd;
extern uint64_t pharosPartition23DataSramBackupEnd;
extern uint64_t pharosPartition23BssSramBackupEnd;

extern uint64_t pharosPartition23DataSdramLoadAddr;
extern uint8_t pharosPartition23DataSdramSize;
extern uint8_t pharosPartition23BssSdramSize;
extern uint64_t pharosPartition23SdramStart;
extern uint64_t pharosPartition23DataSdramStart;
extern uint64_t pharosPartition23BssSdramStart;
extern uint64_t pharosPartition23SdramEnd;
extern uint64_t pharosPartition23DataSdramEnd;
extern uint64_t pharosPartition23BssSdramEnd;

extern uint64_t pharosPartition24DataCcmLoadAddr;
extern uint8_t pharosPartition24DataCcmSize;
extern uint8_t pharosPartition24BssCcmSize;
extern uint64_t pharosPartition24CcmStart;
extern uint64_t pharosPartition24DataCcmStart;
extern uint64_t pharosPartition24BssCcmStart;
extern uint64_t pharosPartition24CcmEnd;
extern uint64_t pharosPartition24DataCcmEnd;
extern uint64_t pharosPartition24BssCcmEnd;

extern uint64_t pharosPartition24DataSram1LoadAddr;
extern uint8_t pharosPartition24DataSram1Size;
extern uint8_t pharosPartition24BssSram1Size;
extern uint64_t pharosPartition24Sram1Start;
extern uint64_t pharosPartition24DataSram1Start;
extern uint64_t pharosPartition24BssSram1Start;
extern uint64_t pharosPartition24Sram1End;
extern uint64_t pharosPartition24DataSram1End;
extern uint64_t pharosPartition24BssSram1End;

extern uint64_t pharosPartition24DataSram2LoadAddr;
extern uint8_t pharosPartition24DataSram2Size;
extern uint8_t pharosPartition24BssSram2Size;
extern uint64_t pharosPartition24Sram2Start;
extern uint64_t pharosPartition24DataSram2Start;
extern uint64_t pharosPartition24BssSram2Start;
extern uint64_t pharosPartition24Sram2End;
extern uint64_t pharosPartition24DataSram2End;
extern uint64_t pharosPartition24BssSram2End;

extern uint64_t pharosPartition24DataSram3LoadAddr;
extern uint8_t pharosPartition24DataSram3Size;
extern uint8_t pharosPartition24BssSram3Size;
extern uint64_t pharosPartition24Sram3Start;
extern uint64_t pharosPartition24DataSram3Start;
extern uint64_t pharosPartition24BssSram3Start;
extern uint64_t pharosPartition24Sram3End;
extern uint64_t pharosPartition24DataSram3End;
extern uint64_t pharosPartition24BssSram3End;

extern uint64_t pharosPartition24DataSramBackupLoadAddr;
extern uint8_t pharosPartition24DataSramBackupSize;
extern uint8_t pharosPartition24BssSramBackupSize;
extern uint64_t pharosPartition24SramBackupStart;
extern uint64_t pharosPartition24DataSramBackupStart;
extern uint64_t pharosPartition24BssSramBackupStart;
extern uint64_t pharosPartition24SramBackupEnd;
extern uint64_t pharosPartition24DataSramBackupEnd;
extern uint64_t pharosPartition24BssSramBackupEnd;

extern uint64_t pharosPartition24DataSdramLoadAddr;
extern uint8_t pharosPartition24DataSdramSize;
extern uint8_t pharosPartition24BssSdramSize;
extern uint64_t pharosPartition24SdramStart;
extern uint64_t pharosPartition24DataSdramStart;
extern uint64_t pharosPartition24BssSdramStart;
extern uint64_t pharosPartition24SdramEnd;
extern uint64_t pharosPartition24DataSdramEnd;
extern uint64_t pharosPartition24BssSdramEnd;

extern uint64_t pharosPartition25DataCcmLoadAddr;
extern uint8_t pharosPartition25DataCcmSize;
extern uint8_t pharosPartition25BssCcmSize;
extern uint64_t pharosPartition25CcmStart;
extern uint64_t pharosPartition25DataCcmStart;
extern uint64_t pharosPartition25BssCcmStart;
extern uint64_t pharosPartition25CcmEnd;
extern uint64_t pharosPartition25DataCcmEnd;
extern uint64_t pharosPartition25BssCcmEnd;

extern uint64_t pharosPartition25DataSram1LoadAddr;
extern uint8_t pharosPartition25DataSram1Size;
extern uint8_t pharosPartition25BssSram1Size;
extern uint64_t pharosPartition25Sram1Start;
extern uint64_t pharosPartition25DataSram1Start;
extern uint64_t pharosPartition25BssSram1Start;
extern uint64_t pharosPartition25Sram1End;
extern uint64_t pharosPartition25DataSram1End;
extern uint64_t pharosPartition25BssSram1End;

extern uint64_t pharosPartition25DataSram2LoadAddr;
extern uint8_t pharosPartition25DataSram2Size;
extern uint8_t pharosPartition25BssSram2Size;
extern uint64_t pharosPartition25Sram2Start;
extern uint64_t pharosPartition25DataSram2Start;
extern uint64_t pharosPartition25BssSram2Start;
extern uint64_t pharosPartition25Sram2End;
extern uint64_t pharosPartition25DataSram2End;
extern uint64_t pharosPartition25BssSram2End;

extern uint64_t pharosPartition25DataSram3LoadAddr;
extern uint8_t pharosPartition25DataSram3Size;
extern uint8_t pharosPartition25BssSram3Size;
extern uint64_t pharosPartition25Sram3Start;
extern uint64_t pharosPartition25DataSram3Start;
extern uint64_t pharosPartition25BssSram3Start;
extern uint64_t pharosPartition25Sram3End;
extern uint64_t pharosPartition25DataSram3End;
extern uint64_t pharosPartition25BssSram3End;

extern uint64_t pharosPartition25DataSramBackupLoadAddr;
extern uint8_t pharosPartition25DataSramBackupSize;
extern uint8_t pharosPartition25BssSramBackupSize;
extern uint64_t pharosPartition25SramBackupStart;
extern uint64_t pharosPartition25DataSramBackupStart;
extern uint64_t pharosPartition25BssSramBackupStart;
extern uint64_t pharosPartition25SramBackupEnd;
extern uint64_t pharosPartition25DataSramBackupEnd;
extern uint64_t pharosPartition25BssSramBackupEnd;

extern uint64_t pharosPartition25DataSdramLoadAddr;
extern uint8_t pharosPartition25DataSdramSize;
extern uint8_t pharosPartition25BssSdramSize;
extern uint64_t pharosPartition25SdramStart;
extern uint64_t pharosPartition25DataSdramStart;
extern uint64_t pharosPartition25BssSdramStart;
extern uint64_t pharosPartition25SdramEnd;
extern uint64_t pharosPartition25DataSdramEnd;
extern uint64_t pharosPartition25BssSdramEnd;

extern uint64_t pharosPartition26DataCcmLoadAddr;
extern uint8_t pharosPartition26DataCcmSize;
extern uint8_t pharosPartition26BssCcmSize;
extern uint64_t pharosPartition26CcmStart;
extern uint64_t pharosPartition26DataCcmStart;
extern uint64_t pharosPartition26BssCcmStart;
extern uint64_t pharosPartition26CcmEnd;
extern uint64_t pharosPartition26DataCcmEnd;
extern uint64_t pharosPartition26BssCcmEnd;

extern uint64_t pharosPartition26DataSram1LoadAddr;
extern uint8_t pharosPartition26DataSram1Size;
extern uint8_t pharosPartition26BssSram1Size;
extern uint64_t pharosPartition26Sram1Start;
extern uint64_t pharosPartition26DataSram1Start;
extern uint64_t pharosPartition26BssSram1Start;
extern uint64_t pharosPartition26Sram1End;
extern uint64_t pharosPartition26DataSram1End;
extern uint64_t pharosPartition26BssSram1End;

extern uint64_t pharosPartition26DataSram2LoadAddr;
extern uint8_t pharosPartition26DataSram2Size;
extern uint8_t pharosPartition26BssSram2Size;
extern uint64_t pharosPartition26Sram2Start;
extern uint64_t pharosPartition26DataSram2Start;
extern uint64_t pharosPartition26BssSram2Start;
extern uint64_t pharosPartition26Sram2End;
extern uint64_t pharosPartition26DataSram2End;
extern uint64_t pharosPartition26BssSram2End;

extern uint64_t pharosPartition26DataSram3LoadAddr;
extern uint8_t pharosPartition26DataSram3Size;
extern uint8_t pharosPartition26BssSram3Size;
extern uint64_t pharosPartition26Sram3Start;
extern uint64_t pharosPartition26DataSram3Start;
extern uint64_t pharosPartition26BssSram3Start;
extern uint64_t pharosPartition26Sram3End;
extern uint64_t pharosPartition26DataSram3End;
extern uint64_t pharosPartition26BssSram3End;

extern uint64_t pharosPartition26DataSramBackupLoadAddr;
extern uint8_t pharosPartition26DataSramBackupSize;
extern uint8_t pharosPartition26BssSramBackupSize;
extern uint64_t pharosPartition26SramBackupStart;
extern uint64_t pharosPartition26DataSramBackupStart;
extern uint64_t pharosPartition26BssSramBackupStart;
extern uint64_t pharosPartition26SramBackupEnd;
extern uint64_t pharosPartition26DataSramBackupEnd;
extern uint64_t pharosPartition26BssSramBackupEnd;

extern uint64_t pharosPartition26DataSdramLoadAddr;
extern uint8_t pharosPartition26DataSdramSize;
extern uint8_t pharosPartition26BssSdramSize;
extern uint64_t pharosPartition26SdramStart;
extern uint64_t pharosPartition26DataSdramStart;
extern uint64_t pharosPartition26BssSdramStart;
extern uint64_t pharosPartition26SdramEnd;
extern uint64_t pharosPartition26DataSdramEnd;
extern uint64_t pharosPartition26BssSdramEnd;

extern uint64_t pharosPartition27DataCcmLoadAddr;
extern uint8_t pharosPartition27DataCcmSize;
extern uint8_t pharosPartition27BssCcmSize;
extern uint64_t pharosPartition27CcmStart;
extern uint64_t pharosPartition27DataCcmStart;
extern uint64_t pharosPartition27BssCcmStart;
extern uint64_t pharosPartition27CcmEnd;
extern uint64_t pharosPartition27DataCcmEnd;
extern uint64_t pharosPartition27BssCcmEnd;

extern uint64_t pharosPartition27DataSram1LoadAddr;
extern uint8_t pharosPartition27DataSram1Size;
extern uint8_t pharosPartition27BssSram1Size;
extern uint64_t pharosPartition27Sram1Start;
extern uint64_t pharosPartition27DataSram1Start;
extern uint64_t pharosPartition27BssSram1Start;
extern uint64_t pharosPartition27Sram1End;
extern uint64_t pharosPartition27DataSram1End;
extern uint64_t pharosPartition27BssSram1End;

extern uint64_t pharosPartition27DataSram2LoadAddr;
extern uint8_t pharosPartition27DataSram2Size;
extern uint8_t pharosPartition27BssSram2Size;
extern uint64_t pharosPartition27Sram2Start;
extern uint64_t pharosPartition27DataSram2Start;
extern uint64_t pharosPartition27BssSram2Start;
extern uint64_t pharosPartition27Sram2End;
extern uint64_t pharosPartition27DataSram2End;
extern uint64_t pharosPartition27BssSram2End;

extern uint64_t pharosPartition27DataSram3LoadAddr;
extern uint8_t pharosPartition27DataSram3Size;
extern uint8_t pharosPartition27BssSram3Size;
extern uint64_t pharosPartition27Sram3Start;
extern uint64_t pharosPartition27DataSram3Start;
extern uint64_t pharosPartition27BssSram3Start;
extern uint64_t pharosPartition27Sram3End;
extern uint64_t pharosPartition27DataSram3End;
extern uint64_t pharosPartition27BssSram3End;

extern uint64_t pharosPartition27DataSramBackupLoadAddr;
extern uint8_t pharosPartition27DataSramBackupSize;
extern uint8_t pharosPartition27BssSramBackupSize;
extern uint64_t pharosPartition27SramBackupStart;
extern uint64_t pharosPartition27DataSramBackupStart;
extern uint64_t pharosPartition27BssSramBackupStart;
extern uint64_t pharosPartition27SramBackupEnd;
extern uint64_t pharosPartition27DataSramBackupEnd;
extern uint64_t pharosPartition27BssSramBackupEnd;

extern uint64_t pharosPartition27DataSdramLoadAddr;
extern uint8_t pharosPartition27DataSdramSize;
extern uint8_t pharosPartition27BssSdramSize;
extern uint64_t pharosPartition27SdramStart;
extern uint64_t pharosPartition27DataSdramStart;
extern uint64_t pharosPartition27BssSdramStart;
extern uint64_t pharosPartition27SdramEnd;
extern uint64_t pharosPartition27DataSdramEnd;
extern uint64_t pharosPartition27BssSdramEnd;

extern uint64_t pharosPartition28DataCcmLoadAddr;
extern uint8_t pharosPartition28DataCcmSize;
extern uint8_t pharosPartition28BssCcmSize;
extern uint64_t pharosPartition28CcmStart;
extern uint64_t pharosPartition28DataCcmStart;
extern uint64_t pharosPartition28BssCcmStart;
extern uint64_t pharosPartition28CcmEnd;
extern uint64_t pharosPartition28DataCcmEnd;
extern uint64_t pharosPartition28BssCcmEnd;

extern uint64_t pharosPartition28DataSram1LoadAddr;
extern uint8_t pharosPartition28DataSram1Size;
extern uint8_t pharosPartition28BssSram1Size;
extern uint64_t pharosPartition28Sram1Start;
extern uint64_t pharosPartition28DataSram1Start;
extern uint64_t pharosPartition28BssSram1Start;
extern uint64_t pharosPartition28Sram1End;
extern uint64_t pharosPartition28DataSram1End;
extern uint64_t pharosPartition28BssSram1End;

extern uint64_t pharosPartition28DataSram2LoadAddr;
extern uint8_t pharosPartition28DataSram2Size;
extern uint8_t pharosPartition28BssSram2Size;
extern uint64_t pharosPartition28Sram2Start;
extern uint64_t pharosPartition28DataSram2Start;
extern uint64_t pharosPartition28BssSram2Start;
extern uint64_t pharosPartition28Sram2End;
extern uint64_t pharosPartition28DataSram2End;
extern uint64_t pharosPartition28BssSram2End;

extern uint64_t pharosPartition28DataSram3LoadAddr;
extern uint8_t pharosPartition28DataSram3Size;
extern uint8_t pharosPartition28BssSram3Size;
extern uint64_t pharosPartition28Sram3Start;
extern uint64_t pharosPartition28DataSram3Start;
extern uint64_t pharosPartition28BssSram3Start;
extern uint64_t pharosPartition28Sram3End;
extern uint64_t pharosPartition28DataSram3End;
extern uint64_t pharosPartition28BssSram3End;

extern uint64_t pharosPartition28DataSramBackupLoadAddr;
extern uint8_t pharosPartition28DataSramBackupSize;
extern uint8_t pharosPartition28BssSramBackupSize;
extern uint64_t pharosPartition28SramBackupStart;
extern uint64_t pharosPartition28DataSramBackupStart;
extern uint64_t pharosPartition28BssSramBackupStart;
extern uint64_t pharosPartition28SramBackupEnd;
extern uint64_t pharosPartition28DataSramBackupEnd;
extern uint64_t pharosPartition28BssSramBackupEnd;

extern uint64_t pharosPartition28DataSdramLoadAddr;
extern uint8_t pharosPartition28DataSdramSize;
extern uint8_t pharosPartition28BssSdramSize;
extern uint64_t pharosPartition28SdramStart;
extern uint64_t pharosPartition28DataSdramStart;
extern uint64_t pharosPartition28BssSdramStart;
extern uint64_t pharosPartition28SdramEnd;
extern uint64_t pharosPartition28DataSdramEnd;
extern uint64_t pharosPartition28BssSdramEnd;

extern uint64_t pharosPartition29DataCcmLoadAddr;
extern uint8_t pharosPartition29DataCcmSize;
extern uint8_t pharosPartition29BssCcmSize;
extern uint64_t pharosPartition29CcmStart;
extern uint64_t pharosPartition29DataCcmStart;
extern uint64_t pharosPartition29BssCcmStart;
extern uint64_t pharosPartition29CcmEnd;
extern uint64_t pharosPartition29DataCcmEnd;
extern uint64_t pharosPartition29BssCcmEnd;

extern uint64_t pharosPartition29DataSram1LoadAddr;
extern uint8_t pharosPartition29DataSram1Size;
extern uint8_t pharosPartition29BssSram1Size;
extern uint64_t pharosPartition29Sram1Start;
extern uint64_t pharosPartition29DataSram1Start;
extern uint64_t pharosPartition29BssSram1Start;
extern uint64_t pharosPartition29Sram1End;
extern uint64_t pharosPartition29DataSram1End;
extern uint64_t pharosPartition29BssSram1End;

extern uint64_t pharosPartition29DataSram2LoadAddr;
extern uint8_t pharosPartition29DataSram2Size;
extern uint8_t pharosPartition29BssSram2Size;
extern uint64_t pharosPartition29Sram2Start;
extern uint64_t pharosPartition29DataSram2Start;
extern uint64_t pharosPartition29BssSram2Start;
extern uint64_t pharosPartition29Sram2End;
extern uint64_t pharosPartition29DataSram2End;
extern uint64_t pharosPartition29BssSram2End;

extern uint64_t pharosPartition29DataSram3LoadAddr;
extern uint8_t pharosPartition29DataSram3Size;
extern uint8_t pharosPartition29BssSram3Size;
extern uint64_t pharosPartition29Sram3Start;
extern uint64_t pharosPartition29DataSram3Start;
extern uint64_t pharosPartition29BssSram3Start;
extern uint64_t pharosPartition29Sram3End;
extern uint64_t pharosPartition29DataSram3End;
extern uint64_t pharosPartition29BssSram3End;

extern uint64_t pharosPartition29DataSramBackupLoadAddr;
extern uint8_t pharosPartition29DataSramBackupSize;
extern uint8_t pharosPartition29BssSramBackupSize;
extern uint64_t pharosPartition29SramBackupStart;
extern uint64_t pharosPartition29DataSramBackupStart;
extern uint64_t pharosPartition29BssSramBackupStart;
extern uint64_t pharosPartition29SramBackupEnd;
extern uint64_t pharosPartition29DataSramBackupEnd;
extern uint64_t pharosPartition29BssSramBackupEnd;

extern uint64_t pharosPartition29DataSdramLoadAddr;
extern uint8_t pharosPartition29DataSdramSize;
extern uint8_t pharosPartition29BssSdramSize;
extern uint64_t pharosPartition29SdramStart;
extern uint64_t pharosPartition29DataSdramStart;
extern uint64_t pharosPartition29BssSdramStart;
extern uint64_t pharosPartition29SdramEnd;
extern uint64_t pharosPartition29DataSdramEnd;
extern uint64_t pharosPartition29BssSdramEnd;

extern uint64_t pharosPartition30DataCcmLoadAddr;
extern uint8_t pharosPartition30DataCcmSize;
extern uint8_t pharosPartition30BssCcmSize;
extern uint64_t pharosPartition30CcmStart;
extern uint64_t pharosPartition30DataCcmStart;
extern uint64_t pharosPartition30BssCcmStart;
extern uint64_t pharosPartition30CcmEnd;
extern uint64_t pharosPartition30DataCcmEnd;
extern uint64_t pharosPartition30BssCcmEnd;

extern uint64_t pharosPartition30DataSram1LoadAddr;
extern uint8_t pharosPartition30DataSram1Size;
extern uint8_t pharosPartition30BssSram1Size;
extern uint64_t pharosPartition30Sram1Start;
extern uint64_t pharosPartition30DataSram1Start;
extern uint64_t pharosPartition30BssSram1Start;
extern uint64_t pharosPartition30Sram1End;
extern uint64_t pharosPartition30DataSram1End;
extern uint64_t pharosPartition30BssSram1End;

extern uint64_t pharosPartition30DataSram2LoadAddr;
extern uint8_t pharosPartition30DataSram2Size;
extern uint8_t pharosPartition30BssSram2Size;
extern uint64_t pharosPartition30Sram2Start;
extern uint64_t pharosPartition30DataSram2Start;
extern uint64_t pharosPartition30BssSram2Start;
extern uint64_t pharosPartition30Sram2End;
extern uint64_t pharosPartition30DataSram2End;
extern uint64_t pharosPartition30BssSram2End;

extern uint64_t pharosPartition30DataSram3LoadAddr;
extern uint8_t pharosPartition30DataSram3Size;
extern uint8_t pharosPartition30BssSram3Size;
extern uint64_t pharosPartition30Sram3Start;
extern uint64_t pharosPartition30DataSram3Start;
extern uint64_t pharosPartition30BssSram3Start;
extern uint64_t pharosPartition30Sram3End;
extern uint64_t pharosPartition30DataSram3End;
extern uint64_t pharosPartition30BssSram3End;

extern uint64_t pharosPartition30DataSramBackupLoadAddr;
extern uint8_t pharosPartition30DataSramBackupSize;
extern uint8_t pharosPartition30BssSramBackupSize;
extern uint64_t pharosPartition30SramBackupStart;
extern uint64_t pharosPartition30DataSramBackupStart;
extern uint64_t pharosPartition30BssSramBackupStart;
extern uint64_t pharosPartition30SramBackupEnd;
extern uint64_t pharosPartition30DataSramBackupEnd;
extern uint64_t pharosPartition30BssSramBackupEnd;

extern uint64_t pharosPartition30DataSdramLoadAddr;
extern uint8_t pharosPartition30DataSdramSize;
extern uint8_t pharosPartition30BssSdramSize;
extern uint64_t pharosPartition30SdramStart;
extern uint64_t pharosPartition30DataSdramStart;
extern uint64_t pharosPartition30BssSdramStart;
extern uint64_t pharosPartition30SdramEnd;
extern uint64_t pharosPartition30DataSdramEnd;
extern uint64_t pharosPartition30BssSdramEnd;

extern uint64_t pharosPartition31DataCcmLoadAddr;
extern uint8_t pharosPartition31DataCcmSize;
extern uint8_t pharosPartition31BssCcmSize;
extern uint64_t pharosPartition31CcmStart;
extern uint64_t pharosPartition31DataCcmStart;
extern uint64_t pharosPartition31BssCcmStart;
extern uint64_t pharosPartition31CcmEnd;
extern uint64_t pharosPartition31DataCcmEnd;
extern uint64_t pharosPartition31BssCcmEnd;

extern uint64_t pharosPartition31DataSram1LoadAddr;
extern uint8_t pharosPartition31DataSram1Size;
extern uint8_t pharosPartition31BssSram1Size;
extern uint64_t pharosPartition31Sram1Start;
extern uint64_t pharosPartition31DataSram1Start;
extern uint64_t pharosPartition31BssSram1Start;
extern uint64_t pharosPartition31Sram1End;
extern uint64_t pharosPartition31DataSram1End;
extern uint64_t pharosPartition31BssSram1End;

extern uint64_t pharosPartition31DataSram2LoadAddr;
extern uint8_t pharosPartition31DataSram2Size;
extern uint8_t pharosPartition31BssSram2Size;
extern uint64_t pharosPartition31Sram2Start;
extern uint64_t pharosPartition31DataSram2Start;
extern uint64_t pharosPartition31BssSram2Start;
extern uint64_t pharosPartition31Sram2End;
extern uint64_t pharosPartition31DataSram2End;
extern uint64_t pharosPartition31BssSram2End;

extern uint64_t pharosPartition31DataSram3LoadAddr;
extern uint8_t pharosPartition31DataSram3Size;
extern uint8_t pharosPartition31BssSram3Size;
extern uint64_t pharosPartition31Sram3Start;
extern uint64_t pharosPartition31DataSram3Start;
extern uint64_t pharosPartition31BssSram3Start;
extern uint64_t pharosPartition31Sram3End;
extern uint64_t pharosPartition31DataSram3End;
extern uint64_t pharosPartition31BssSram3End;

extern uint64_t pharosPartition31DataSramBackupLoadAddr;
extern uint8_t pharosPartition31DataSramBackupSize;
extern uint8_t pharosPartition31BssSramBackupSize;
extern uint64_t pharosPartition31SramBackupStart;
extern uint64_t pharosPartition31DataSramBackupStart;
extern uint64_t pharosPartition31BssSramBackupStart;
extern uint64_t pharosPartition31SramBackupEnd;
extern uint64_t pharosPartition31DataSramBackupEnd;
extern uint64_t pharosPartition31BssSramBackupEnd;

extern uint64_t pharosPartition31DataSdramLoadAddr;
extern uint8_t pharosPartition31DataSdramSize;
extern uint8_t pharosPartition31BssSdramSize;
extern uint64_t pharosPartition31SdramStart;
extern uint64_t pharosPartition31DataSdramStart;
extern uint64_t pharosPartition31BssSdramStart;
extern uint64_t pharosPartition31SdramEnd;
extern uint64_t pharosPartition31DataSdramEnd;
extern uint64_t pharosPartition31BssSdramEnd;

extern uint64_t pharosPartition32DataCcmLoadAddr;
extern uint8_t pharosPartition32DataCcmSize;
extern uint8_t pharosPartition32BssCcmSize;
extern uint64_t pharosPartition32CcmStart;
extern uint64_t pharosPartition32DataCcmStart;
extern uint64_t pharosPartition32BssCcmStart;
extern uint64_t pharosPartition32CcmEnd;
extern uint64_t pharosPartition32DataCcmEnd;
extern uint64_t pharosPartition32BssCcmEnd;

extern uint64_t pharosPartition32DataSram1LoadAddr;
extern uint8_t pharosPartition32DataSram1Size;
extern uint8_t pharosPartition32BssSram1Size;
extern uint64_t pharosPartition32Sram1Start;
extern uint64_t pharosPartition32DataSram1Start;
extern uint64_t pharosPartition32BssSram1Start;
extern uint64_t pharosPartition32Sram1End;
extern uint64_t pharosPartition32DataSram1End;
extern uint64_t pharosPartition32BssSram1End;

extern uint64_t pharosPartition32DataSram2LoadAddr;
extern uint8_t pharosPartition32DataSram2Size;
extern uint8_t pharosPartition32BssSram2Size;
extern uint64_t pharosPartition32Sram2Start;
extern uint64_t pharosPartition32DataSram2Start;
extern uint64_t pharosPartition32BssSram2Start;
extern uint64_t pharosPartition32Sram2End;
extern uint64_t pharosPartition32DataSram2End;
extern uint64_t pharosPartition32BssSram2End;

extern uint64_t pharosPartition32DataSram3LoadAddr;
extern uint8_t pharosPartition32DataSram3Size;
extern uint8_t pharosPartition32BssSram3Size;
extern uint64_t pharosPartition32Sram3Start;
extern uint64_t pharosPartition32DataSram3Start;
extern uint64_t pharosPartition32BssSram3Start;
extern uint64_t pharosPartition32Sram3End;
extern uint64_t pharosPartition32DataSram3End;
extern uint64_t pharosPartition32BssSram3End;

extern uint64_t pharosPartition32DataSramBackupLoadAddr;
extern uint8_t pharosPartition32DataSramBackupSize;
extern uint8_t pharosPartition32BssSramBackupSize;
extern uint64_t pharosPartition32SramBackupStart;
extern uint64_t pharosPartition32DataSramBackupStart;
extern uint64_t pharosPartition32BssSramBackupStart;
extern uint64_t pharosPartition32SramBackupEnd;
extern uint64_t pharosPartition32DataSramBackupEnd;
extern uint64_t pharosPartition32BssSramBackupEnd;

extern uint64_t pharosPartition32DataSdramLoadAddr;
extern uint8_t pharosPartition32DataSdramSize;
extern uint8_t pharosPartition32BssSdramSize;
extern uint64_t pharosPartition32SdramStart;
extern uint64_t pharosPartition32DataSdramStart;
extern uint64_t pharosPartition32BssSdramStart;
extern uint64_t pharosPartition32SdramEnd;
extern uint64_t pharosPartition32DataSdramEnd;
extern uint64_t pharosPartition32BssSdramEnd;

extern uint64_t pharosPartition33DataCcmLoadAddr;
extern uint8_t pharosPartition33DataCcmSize;
extern uint8_t pharosPartition33BssCcmSize;
extern uint64_t pharosPartition33CcmStart;
extern uint64_t pharosPartition33DataCcmStart;
extern uint64_t pharosPartition33BssCcmStart;
extern uint64_t pharosPartition33CcmEnd;
extern uint64_t pharosPartition33DataCcmEnd;
extern uint64_t pharosPartition33BssCcmEnd;

extern uint64_t pharosPartition33DataSram1LoadAddr;
extern uint8_t pharosPartition33DataSram1Size;
extern uint8_t pharosPartition33BssSram1Size;
extern uint64_t pharosPartition33Sram1Start;
extern uint64_t pharosPartition33DataSram1Start;
extern uint64_t pharosPartition33BssSram1Start;
extern uint64_t pharosPartition33Sram1End;
extern uint64_t pharosPartition33DataSram1End;
extern uint64_t pharosPartition33BssSram1End;

extern uint64_t pharosPartition33DataSram2LoadAddr;
extern uint8_t pharosPartition33DataSram2Size;
extern uint8_t pharosPartition33BssSram2Size;
extern uint64_t pharosPartition33Sram2Start;
extern uint64_t pharosPartition33DataSram2Start;
extern uint64_t pharosPartition33BssSram2Start;
extern uint64_t pharosPartition33Sram2End;
extern uint64_t pharosPartition33DataSram2End;
extern uint64_t pharosPartition33BssSram2End;

extern uint64_t pharosPartition33DataSram3LoadAddr;
extern uint8_t pharosPartition33DataSram3Size;
extern uint8_t pharosPartition33BssSram3Size;
extern uint64_t pharosPartition33Sram3Start;
extern uint64_t pharosPartition33DataSram3Start;
extern uint64_t pharosPartition33BssSram3Start;
extern uint64_t pharosPartition33Sram3End;
extern uint64_t pharosPartition33DataSram3End;
extern uint64_t pharosPartition33BssSram3End;

extern uint64_t pharosPartition33DataSramBackupLoadAddr;
extern uint8_t pharosPartition33DataSramBackupSize;
extern uint8_t pharosPartition33BssSramBackupSize;
extern uint64_t pharosPartition33SramBackupStart;
extern uint64_t pharosPartition33DataSramBackupStart;
extern uint64_t pharosPartition33BssSramBackupStart;
extern uint64_t pharosPartition33SramBackupEnd;
extern uint64_t pharosPartition33DataSramBackupEnd;
extern uint64_t pharosPartition33BssSramBackupEnd;

extern uint64_t pharosPartition33DataSdramLoadAddr;
extern uint8_t pharosPartition33DataSdramSize;
extern uint8_t pharosPartition33BssSdramSize;
extern uint64_t pharosPartition33SdramStart;
extern uint64_t pharosPartition33DataSdramStart;
extern uint64_t pharosPartition33BssSdramStart;
extern uint64_t pharosPartition33SdramEnd;
extern uint64_t pharosPartition33DataSdramEnd;
extern uint64_t pharosPartition33BssSdramEnd;

extern uint64_t pharosPartition34DataCcmLoadAddr;
extern uint8_t pharosPartition34DataCcmSize;
extern uint8_t pharosPartition34BssCcmSize;
extern uint64_t pharosPartition34CcmStart;
extern uint64_t pharosPartition34DataCcmStart;
extern uint64_t pharosPartition34BssCcmStart;
extern uint64_t pharosPartition34CcmEnd;
extern uint64_t pharosPartition34DataCcmEnd;
extern uint64_t pharosPartition34BssCcmEnd;

extern uint64_t pharosPartition34DataSram1LoadAddr;
extern uint8_t pharosPartition34DataSram1Size;
extern uint8_t pharosPartition34BssSram1Size;
extern uint64_t pharosPartition34Sram1Start;
extern uint64_t pharosPartition34DataSram1Start;
extern uint64_t pharosPartition34BssSram1Start;
extern uint64_t pharosPartition34Sram1End;
extern uint64_t pharosPartition34DataSram1End;
extern uint64_t pharosPartition34BssSram1End;

extern uint64_t pharosPartition34DataSram2LoadAddr;
extern uint8_t pharosPartition34DataSram2Size;
extern uint8_t pharosPartition34BssSram2Size;
extern uint64_t pharosPartition34Sram2Start;
extern uint64_t pharosPartition34DataSram2Start;
extern uint64_t pharosPartition34BssSram2Start;
extern uint64_t pharosPartition34Sram2End;
extern uint64_t pharosPartition34DataSram2End;
extern uint64_t pharosPartition34BssSram2End;

extern uint64_t pharosPartition34DataSram3LoadAddr;
extern uint8_t pharosPartition34DataSram3Size;
extern uint8_t pharosPartition34BssSram3Size;
extern uint64_t pharosPartition34Sram3Start;
extern uint64_t pharosPartition34DataSram3Start;
extern uint64_t pharosPartition34BssSram3Start;
extern uint64_t pharosPartition34Sram3End;
extern uint64_t pharosPartition34DataSram3End;
extern uint64_t pharosPartition34BssSram3End;

extern uint64_t pharosPartition34DataSramBackupLoadAddr;
extern uint8_t pharosPartition34DataSramBackupSize;
extern uint8_t pharosPartition34BssSramBackupSize;
extern uint64_t pharosPartition34SramBackupStart;
extern uint64_t pharosPartition34DataSramBackupStart;
extern uint64_t pharosPartition34BssSramBackupStart;
extern uint64_t pharosPartition34SramBackupEnd;
extern uint64_t pharosPartition34DataSramBackupEnd;
extern uint64_t pharosPartition34BssSramBackupEnd;

extern uint64_t pharosPartition34DataSdramLoadAddr;
extern uint8_t pharosPartition34DataSdramSize;
extern uint8_t pharosPartition34BssSdramSize;
extern uint64_t pharosPartition34SdramStart;
extern uint64_t pharosPartition34DataSdramStart;
extern uint64_t pharosPartition34BssSdramStart;
extern uint64_t pharosPartition34SdramEnd;
extern uint64_t pharosPartition34DataSdramEnd;
extern uint64_t pharosPartition34BssSdramEnd;

extern uint64_t pharosPartition35DataCcmLoadAddr;
extern uint8_t pharosPartition35DataCcmSize;
extern uint8_t pharosPartition35BssCcmSize;
extern uint64_t pharosPartition35CcmStart;
extern uint64_t pharosPartition35DataCcmStart;
extern uint64_t pharosPartition35BssCcmStart;
extern uint64_t pharosPartition35CcmEnd;
extern uint64_t pharosPartition35DataCcmEnd;
extern uint64_t pharosPartition35BssCcmEnd;

extern uint64_t pharosPartition35DataSram1LoadAddr;
extern uint8_t pharosPartition35DataSram1Size;
extern uint8_t pharosPartition35BssSram1Size;
extern uint64_t pharosPartition35Sram1Start;
extern uint64_t pharosPartition35DataSram1Start;
extern uint64_t pharosPartition35BssSram1Start;
extern uint64_t pharosPartition35Sram1End;
extern uint64_t pharosPartition35DataSram1End;
extern uint64_t pharosPartition35BssSram1End;

extern uint64_t pharosPartition35DataSram2LoadAddr;
extern uint8_t pharosPartition35DataSram2Size;
extern uint8_t pharosPartition35BssSram2Size;
extern uint64_t pharosPartition35Sram2Start;
extern uint64_t pharosPartition35DataSram2Start;
extern uint64_t pharosPartition35BssSram2Start;
extern uint64_t pharosPartition35Sram2End;
extern uint64_t pharosPartition35DataSram2End;
extern uint64_t pharosPartition35BssSram2End;

extern uint64_t pharosPartition35DataSram3LoadAddr;
extern uint8_t pharosPartition35DataSram3Size;
extern uint8_t pharosPartition35BssSram3Size;
extern uint64_t pharosPartition35Sram3Start;
extern uint64_t pharosPartition35DataSram3Start;
extern uint64_t pharosPartition35BssSram3Start;
extern uint64_t pharosPartition35Sram3End;
extern uint64_t pharosPartition35DataSram3End;
extern uint64_t pharosPartition35BssSram3End;

extern uint64_t pharosPartition35DataSramBackupLoadAddr;
extern uint8_t pharosPartition35DataSramBackupSize;
extern uint8_t pharosPartition35BssSramBackupSize;
extern uint64_t pharosPartition35SramBackupStart;
extern uint64_t pharosPartition35DataSramBackupStart;
extern uint64_t pharosPartition35BssSramBackupStart;
extern uint64_t pharosPartition35SramBackupEnd;
extern uint64_t pharosPartition35DataSramBackupEnd;
extern uint64_t pharosPartition35BssSramBackupEnd;

extern uint64_t pharosPartition35DataSdramLoadAddr;
extern uint8_t pharosPartition35DataSdramSize;
extern uint8_t pharosPartition35BssSdramSize;
extern uint64_t pharosPartition35SdramStart;
extern uint64_t pharosPartition35DataSdramStart;
extern uint64_t pharosPartition35BssSdramStart;
extern uint64_t pharosPartition35SdramEnd;
extern uint64_t pharosPartition35DataSdramEnd;
extern uint64_t pharosPartition35BssSdramEnd;

extern uint64_t pharosPartition36DataCcmLoadAddr;
extern uint8_t pharosPartition36DataCcmSize;
extern uint8_t pharosPartition36BssCcmSize;
extern uint64_t pharosPartition36CcmStart;
extern uint64_t pharosPartition36DataCcmStart;
extern uint64_t pharosPartition36BssCcmStart;
extern uint64_t pharosPartition36CcmEnd;
extern uint64_t pharosPartition36DataCcmEnd;
extern uint64_t pharosPartition36BssCcmEnd;

extern uint64_t pharosPartition36DataSram1LoadAddr;
extern uint8_t pharosPartition36DataSram1Size;
extern uint8_t pharosPartition36BssSram1Size;
extern uint64_t pharosPartition36Sram1Start;
extern uint64_t pharosPartition36DataSram1Start;
extern uint64_t pharosPartition36BssSram1Start;
extern uint64_t pharosPartition36Sram1End;
extern uint64_t pharosPartition36DataSram1End;
extern uint64_t pharosPartition36BssSram1End;

extern uint64_t pharosPartition36DataSram2LoadAddr;
extern uint8_t pharosPartition36DataSram2Size;
extern uint8_t pharosPartition36BssSram2Size;
extern uint64_t pharosPartition36Sram2Start;
extern uint64_t pharosPartition36DataSram2Start;
extern uint64_t pharosPartition36BssSram2Start;
extern uint64_t pharosPartition36Sram2End;
extern uint64_t pharosPartition36DataSram2End;
extern uint64_t pharosPartition36BssSram2End;

extern uint64_t pharosPartition36DataSram3LoadAddr;
extern uint8_t pharosPartition36DataSram3Size;
extern uint8_t pharosPartition36BssSram3Size;
extern uint64_t pharosPartition36Sram3Start;
extern uint64_t pharosPartition36DataSram3Start;
extern uint64_t pharosPartition36BssSram3Start;
extern uint64_t pharosPartition36Sram3End;
extern uint64_t pharosPartition36DataSram3End;
extern uint64_t pharosPartition36BssSram3End;

extern uint64_t pharosPartition36DataSramBackupLoadAddr;
extern uint8_t pharosPartition36DataSramBackupSize;
extern uint8_t pharosPartition36BssSramBackupSize;
extern uint64_t pharosPartition36SramBackupStart;
extern uint64_t pharosPartition36DataSramBackupStart;
extern uint64_t pharosPartition36BssSramBackupStart;
extern uint64_t pharosPartition36SramBackupEnd;
extern uint64_t pharosPartition36DataSramBackupEnd;
extern uint64_t pharosPartition36BssSramBackupEnd;

extern uint64_t pharosPartition36DataSdramLoadAddr;
extern uint8_t pharosPartition36DataSdramSize;
extern uint8_t pharosPartition36BssSdramSize;
extern uint64_t pharosPartition36SdramStart;
extern uint64_t pharosPartition36DataSdramStart;
extern uint64_t pharosPartition36BssSdramStart;
extern uint64_t pharosPartition36SdramEnd;
extern uint64_t pharosPartition36DataSdramEnd;
extern uint64_t pharosPartition36BssSdramEnd;

extern uint64_t pharosPartition37DataCcmLoadAddr;
extern uint8_t pharosPartition37DataCcmSize;
extern uint8_t pharosPartition37BssCcmSize;
extern uint64_t pharosPartition37CcmStart;
extern uint64_t pharosPartition37DataCcmStart;
extern uint64_t pharosPartition37BssCcmStart;
extern uint64_t pharosPartition37CcmEnd;
extern uint64_t pharosPartition37DataCcmEnd;
extern uint64_t pharosPartition37BssCcmEnd;

extern uint64_t pharosPartition37DataSram1LoadAddr;
extern uint8_t pharosPartition37DataSram1Size;
extern uint8_t pharosPartition37BssSram1Size;
extern uint64_t pharosPartition37Sram1Start;
extern uint64_t pharosPartition37DataSram1Start;
extern uint64_t pharosPartition37BssSram1Start;
extern uint64_t pharosPartition37Sram1End;
extern uint64_t pharosPartition37DataSram1End;
extern uint64_t pharosPartition37BssSram1End;

extern uint64_t pharosPartition37DataSram2LoadAddr;
extern uint8_t pharosPartition37DataSram2Size;
extern uint8_t pharosPartition37BssSram2Size;
extern uint64_t pharosPartition37Sram2Start;
extern uint64_t pharosPartition37DataSram2Start;
extern uint64_t pharosPartition37BssSram2Start;
extern uint64_t pharosPartition37Sram2End;
extern uint64_t pharosPartition37DataSram2End;
extern uint64_t pharosPartition37BssSram2End;

extern uint64_t pharosPartition37DataSram3LoadAddr;
extern uint8_t pharosPartition37DataSram3Size;
extern uint8_t pharosPartition37BssSram3Size;
extern uint64_t pharosPartition37Sram3Start;
extern uint64_t pharosPartition37DataSram3Start;
extern uint64_t pharosPartition37BssSram3Start;
extern uint64_t pharosPartition37Sram3End;
extern uint64_t pharosPartition37DataSram3End;
extern uint64_t pharosPartition37BssSram3End;

extern uint64_t pharosPartition37DataSramBackupLoadAddr;
extern uint8_t pharosPartition37DataSramBackupSize;
extern uint8_t pharosPartition37BssSramBackupSize;
extern uint64_t pharosPartition37SramBackupStart;
extern uint64_t pharosPartition37DataSramBackupStart;
extern uint64_t pharosPartition37BssSramBackupStart;
extern uint64_t pharosPartition37SramBackupEnd;
extern uint64_t pharosPartition37DataSramBackupEnd;
extern uint64_t pharosPartition37BssSramBackupEnd;

extern uint64_t pharosPartition37DataSdramLoadAddr;
extern uint8_t pharosPartition37DataSdramSize;
extern uint8_t pharosPartition37BssSdramSize;
extern uint64_t pharosPartition37SdramStart;
extern uint64_t pharosPartition37DataSdramStart;
extern uint64_t pharosPartition37BssSdramStart;
extern uint64_t pharosPartition37SdramEnd;
extern uint64_t pharosPartition37DataSdramEnd;
extern uint64_t pharosPartition37BssSdramEnd;

extern uint64_t pharosPartition38DataCcmLoadAddr;
extern uint8_t pharosPartition38DataCcmSize;
extern uint8_t pharosPartition38BssCcmSize;
extern uint64_t pharosPartition38CcmStart;
extern uint64_t pharosPartition38DataCcmStart;
extern uint64_t pharosPartition38BssCcmStart;
extern uint64_t pharosPartition38CcmEnd;
extern uint64_t pharosPartition38DataCcmEnd;
extern uint64_t pharosPartition38BssCcmEnd;

extern uint64_t pharosPartition38DataSram1LoadAddr;
extern uint8_t pharosPartition38DataSram1Size;
extern uint8_t pharosPartition38BssSram1Size;
extern uint64_t pharosPartition38Sram1Start;
extern uint64_t pharosPartition38DataSram1Start;
extern uint64_t pharosPartition38BssSram1Start;
extern uint64_t pharosPartition38Sram1End;
extern uint64_t pharosPartition38DataSram1End;
extern uint64_t pharosPartition38BssSram1End;

extern uint64_t pharosPartition38DataSram2LoadAddr;
extern uint8_t pharosPartition38DataSram2Size;
extern uint8_t pharosPartition38BssSram2Size;
extern uint64_t pharosPartition38Sram2Start;
extern uint64_t pharosPartition38DataSram2Start;
extern uint64_t pharosPartition38BssSram2Start;
extern uint64_t pharosPartition38Sram2End;
extern uint64_t pharosPartition38DataSram2End;
extern uint64_t pharosPartition38BssSram2End;

extern uint64_t pharosPartition38DataSram3LoadAddr;
extern uint8_t pharosPartition38DataSram3Size;
extern uint8_t pharosPartition38BssSram3Size;
extern uint64_t pharosPartition38Sram3Start;
extern uint64_t pharosPartition38DataSram3Start;
extern uint64_t pharosPartition38BssSram3Start;
extern uint64_t pharosPartition38Sram3End;
extern uint64_t pharosPartition38DataSram3End;
extern uint64_t pharosPartition38BssSram3End;

extern uint64_t pharosPartition38DataSramBackupLoadAddr;
extern uint8_t pharosPartition38DataSramBackupSize;
extern uint8_t pharosPartition38BssSramBackupSize;
extern uint64_t pharosPartition38SramBackupStart;
extern uint64_t pharosPartition38DataSramBackupStart;
extern uint64_t pharosPartition38BssSramBackupStart;
extern uint64_t pharosPartition38SramBackupEnd;
extern uint64_t pharosPartition38DataSramBackupEnd;
extern uint64_t pharosPartition38BssSramBackupEnd;

extern uint64_t pharosPartition38DataSdramLoadAddr;
extern uint8_t pharosPartition38DataSdramSize;
extern uint8_t pharosPartition38BssSdramSize;
extern uint64_t pharosPartition38SdramStart;
extern uint64_t pharosPartition38DataSdramStart;
extern uint64_t pharosPartition38BssSdramStart;
extern uint64_t pharosPartition38SdramEnd;
extern uint64_t pharosPartition38DataSdramEnd;
extern uint64_t pharosPartition38BssSdramEnd;

extern uint64_t pharosPartition39DataCcmLoadAddr;
extern uint8_t pharosPartition39DataCcmSize;
extern uint8_t pharosPartition39BssCcmSize;
extern uint64_t pharosPartition39CcmStart;
extern uint64_t pharosPartition39DataCcmStart;
extern uint64_t pharosPartition39BssCcmStart;
extern uint64_t pharosPartition39CcmEnd;
extern uint64_t pharosPartition39DataCcmEnd;
extern uint64_t pharosPartition39BssCcmEnd;

extern uint64_t pharosPartition39DataSram1LoadAddr;
extern uint8_t pharosPartition39DataSram1Size;
extern uint8_t pharosPartition39BssSram1Size;
extern uint64_t pharosPartition39Sram1Start;
extern uint64_t pharosPartition39DataSram1Start;
extern uint64_t pharosPartition39BssSram1Start;
extern uint64_t pharosPartition39Sram1End;
extern uint64_t pharosPartition39DataSram1End;
extern uint64_t pharosPartition39BssSram1End;

extern uint64_t pharosPartition39DataSram2LoadAddr;
extern uint8_t pharosPartition39DataSram2Size;
extern uint8_t pharosPartition39BssSram2Size;
extern uint64_t pharosPartition39Sram2Start;
extern uint64_t pharosPartition39DataSram2Start;
extern uint64_t pharosPartition39BssSram2Start;
extern uint64_t pharosPartition39Sram2End;
extern uint64_t pharosPartition39DataSram2End;
extern uint64_t pharosPartition39BssSram2End;

extern uint64_t pharosPartition39DataSram3LoadAddr;
extern uint8_t pharosPartition39DataSram3Size;
extern uint8_t pharosPartition39BssSram3Size;
extern uint64_t pharosPartition39Sram3Start;
extern uint64_t pharosPartition39DataSram3Start;
extern uint64_t pharosPartition39BssSram3Start;
extern uint64_t pharosPartition39Sram3End;
extern uint64_t pharosPartition39DataSram3End;
extern uint64_t pharosPartition39BssSram3End;

extern uint64_t pharosPartition39DataSramBackupLoadAddr;
extern uint8_t pharosPartition39DataSramBackupSize;
extern uint8_t pharosPartition39BssSramBackupSize;
extern uint64_t pharosPartition39SramBackupStart;
extern uint64_t pharosPartition39DataSramBackupStart;
extern uint64_t pharosPartition39BssSramBackupStart;
extern uint64_t pharosPartition39SramBackupEnd;
extern uint64_t pharosPartition39DataSramBackupEnd;
extern uint64_t pharosPartition39BssSramBackupEnd;

extern uint64_t pharosPartition39DataSdramLoadAddr;
extern uint8_t pharosPartition39DataSdramSize;
extern uint8_t pharosPartition39BssSdramSize;
extern uint64_t pharosPartition39SdramStart;
extern uint64_t pharosPartition39DataSdramStart;
extern uint64_t pharosPartition39BssSdramStart;
extern uint64_t pharosPartition39SdramEnd;
extern uint64_t pharosPartition39DataSdramEnd;
extern uint64_t pharosPartition39BssSdramEnd;

extern uint64_t pharosPartition40DataCcmLoadAddr;
extern uint8_t pharosPartition40DataCcmSize;
extern uint8_t pharosPartition40BssCcmSize;
extern uint64_t pharosPartition40CcmStart;
extern uint64_t pharosPartition40DataCcmStart;
extern uint64_t pharosPartition40BssCcmStart;
extern uint64_t pharosPartition40CcmEnd;
extern uint64_t pharosPartition40DataCcmEnd;
extern uint64_t pharosPartition40BssCcmEnd;

extern uint64_t pharosPartition40DataSram1LoadAddr;
extern uint8_t pharosPartition40DataSram1Size;
extern uint8_t pharosPartition40BssSram1Size;
extern uint64_t pharosPartition40Sram1Start;
extern uint64_t pharosPartition40DataSram1Start;
extern uint64_t pharosPartition40BssSram1Start;
extern uint64_t pharosPartition40Sram1End;
extern uint64_t pharosPartition40DataSram1End;
extern uint64_t pharosPartition40BssSram1End;

extern uint64_t pharosPartition40DataSram2LoadAddr;
extern uint8_t pharosPartition40DataSram2Size;
extern uint8_t pharosPartition40BssSram2Size;
extern uint64_t pharosPartition40Sram2Start;
extern uint64_t pharosPartition40DataSram2Start;
extern uint64_t pharosPartition40BssSram2Start;
extern uint64_t pharosPartition40Sram2End;
extern uint64_t pharosPartition40DataSram2End;
extern uint64_t pharosPartition40BssSram2End;

extern uint64_t pharosPartition40DataSram3LoadAddr;
extern uint8_t pharosPartition40DataSram3Size;
extern uint8_t pharosPartition40BssSram3Size;
extern uint64_t pharosPartition40Sram3Start;
extern uint64_t pharosPartition40DataSram3Start;
extern uint64_t pharosPartition40BssSram3Start;
extern uint64_t pharosPartition40Sram3End;
extern uint64_t pharosPartition40DataSram3End;
extern uint64_t pharosPartition40BssSram3End;

extern uint64_t pharosPartition40DataSramBackupLoadAddr;
extern uint8_t pharosPartition40DataSramBackupSize;
extern uint8_t pharosPartition40BssSramBackupSize;
extern uint64_t pharosPartition40SramBackupStart;
extern uint64_t pharosPartition40DataSramBackupStart;
extern uint64_t pharosPartition40BssSramBackupStart;
extern uint64_t pharosPartition40SramBackupEnd;
extern uint64_t pharosPartition40DataSramBackupEnd;
extern uint64_t pharosPartition40BssSramBackupEnd;

extern uint64_t pharosPartition40DataSdramLoadAddr;
extern uint8_t pharosPartition40DataSdramSize;
extern uint8_t pharosPartition40BssSdramSize;
extern uint64_t pharosPartition40SdramStart;
extern uint64_t pharosPartition40DataSdramStart;
extern uint64_t pharosPartition40BssSdramStart;
extern uint64_t pharosPartition40SdramEnd;
extern uint64_t pharosPartition40DataSdramEnd;
extern uint64_t pharosPartition40BssSdramEnd;

extern uint64_t pharosPartition41DataCcmLoadAddr;
extern uint8_t pharosPartition41DataCcmSize;
extern uint8_t pharosPartition41BssCcmSize;
extern uint64_t pharosPartition41CcmStart;
extern uint64_t pharosPartition41DataCcmStart;
extern uint64_t pharosPartition41BssCcmStart;
extern uint64_t pharosPartition41CcmEnd;
extern uint64_t pharosPartition41DataCcmEnd;
extern uint64_t pharosPartition41BssCcmEnd;

extern uint64_t pharosPartition41DataSram1LoadAddr;
extern uint8_t pharosPartition41DataSram1Size;
extern uint8_t pharosPartition41BssSram1Size;
extern uint64_t pharosPartition41Sram1Start;
extern uint64_t pharosPartition41DataSram1Start;
extern uint64_t pharosPartition41BssSram1Start;
extern uint64_t pharosPartition41Sram1End;
extern uint64_t pharosPartition41DataSram1End;
extern uint64_t pharosPartition41BssSram1End;

extern uint64_t pharosPartition41DataSram2LoadAddr;
extern uint8_t pharosPartition41DataSram2Size;
extern uint8_t pharosPartition41BssSram2Size;
extern uint64_t pharosPartition41Sram2Start;
extern uint64_t pharosPartition41DataSram2Start;
extern uint64_t pharosPartition41BssSram2Start;
extern uint64_t pharosPartition41Sram2End;
extern uint64_t pharosPartition41DataSram2End;
extern uint64_t pharosPartition41BssSram2End;

extern uint64_t pharosPartition41DataSram3LoadAddr;
extern uint8_t pharosPartition41DataSram3Size;
extern uint8_t pharosPartition41BssSram3Size;
extern uint64_t pharosPartition41Sram3Start;
extern uint64_t pharosPartition41DataSram3Start;
extern uint64_t pharosPartition41BssSram3Start;
extern uint64_t pharosPartition41Sram3End;
extern uint64_t pharosPartition41DataSram3End;
extern uint64_t pharosPartition41BssSram3End;

extern uint64_t pharosPartition41DataSramBackupLoadAddr;
extern uint8_t pharosPartition41DataSramBackupSize;
extern uint8_t pharosPartition41BssSramBackupSize;
extern uint64_t pharosPartition41SramBackupStart;
extern uint64_t pharosPartition41DataSramBackupStart;
extern uint64_t pharosPartition41BssSramBackupStart;
extern uint64_t pharosPartition41SramBackupEnd;
extern uint64_t pharosPartition41DataSramBackupEnd;
extern uint64_t pharosPartition41BssSramBackupEnd;

extern uint64_t pharosPartition41DataSdramLoadAddr;
extern uint8_t pharosPartition41DataSdramSize;
extern uint8_t pharosPartition41BssSdramSize;
extern uint64_t pharosPartition41SdramStart;
extern uint64_t pharosPartition41DataSdramStart;
extern uint64_t pharosPartition41BssSdramStart;
extern uint64_t pharosPartition41SdramEnd;
extern uint64_t pharosPartition41DataSdramEnd;
extern uint64_t pharosPartition41BssSdramEnd;

extern uint64_t pharosPartition42DataCcmLoadAddr;
extern uint8_t pharosPartition42DataCcmSize;
extern uint8_t pharosPartition42BssCcmSize;
extern uint64_t pharosPartition42CcmStart;
extern uint64_t pharosPartition42DataCcmStart;
extern uint64_t pharosPartition42BssCcmStart;
extern uint64_t pharosPartition42CcmEnd;
extern uint64_t pharosPartition42DataCcmEnd;
extern uint64_t pharosPartition42BssCcmEnd;

extern uint64_t pharosPartition42DataSram1LoadAddr;
extern uint8_t pharosPartition42DataSram1Size;
extern uint8_t pharosPartition42BssSram1Size;
extern uint64_t pharosPartition42Sram1Start;
extern uint64_t pharosPartition42DataSram1Start;
extern uint64_t pharosPartition42BssSram1Start;
extern uint64_t pharosPartition42Sram1End;
extern uint64_t pharosPartition42DataSram1End;
extern uint64_t pharosPartition42BssSram1End;

extern uint64_t pharosPartition42DataSram2LoadAddr;
extern uint8_t pharosPartition42DataSram2Size;
extern uint8_t pharosPartition42BssSram2Size;
extern uint64_t pharosPartition42Sram2Start;
extern uint64_t pharosPartition42DataSram2Start;
extern uint64_t pharosPartition42BssSram2Start;
extern uint64_t pharosPartition42Sram2End;
extern uint64_t pharosPartition42DataSram2End;
extern uint64_t pharosPartition42BssSram2End;

extern uint64_t pharosPartition42DataSram3LoadAddr;
extern uint8_t pharosPartition42DataSram3Size;
extern uint8_t pharosPartition42BssSram3Size;
extern uint64_t pharosPartition42Sram3Start;
extern uint64_t pharosPartition42DataSram3Start;
extern uint64_t pharosPartition42BssSram3Start;
extern uint64_t pharosPartition42Sram3End;
extern uint64_t pharosPartition42DataSram3End;
extern uint64_t pharosPartition42BssSram3End;

extern uint64_t pharosPartition42DataSramBackupLoadAddr;
extern uint8_t pharosPartition42DataSramBackupSize;
extern uint8_t pharosPartition42BssSramBackupSize;
extern uint64_t pharosPartition42SramBackupStart;
extern uint64_t pharosPartition42DataSramBackupStart;
extern uint64_t pharosPartition42BssSramBackupStart;
extern uint64_t pharosPartition42SramBackupEnd;
extern uint64_t pharosPartition42DataSramBackupEnd;
extern uint64_t pharosPartition42BssSramBackupEnd;

extern uint64_t pharosPartition42DataSdramLoadAddr;
extern uint8_t pharosPartition42DataSdramSize;
extern uint8_t pharosPartition42BssSdramSize;
extern uint64_t pharosPartition42SdramStart;
extern uint64_t pharosPartition42DataSdramStart;
extern uint64_t pharosPartition42BssSdramStart;
extern uint64_t pharosPartition42SdramEnd;
extern uint64_t pharosPartition42DataSdramEnd;
extern uint64_t pharosPartition42BssSdramEnd;

extern uint64_t pharosPartition43DataCcmLoadAddr;
extern uint8_t pharosPartition43DataCcmSize;
extern uint8_t pharosPartition43BssCcmSize;
extern uint64_t pharosPartition43CcmStart;
extern uint64_t pharosPartition43DataCcmStart;
extern uint64_t pharosPartition43BssCcmStart;
extern uint64_t pharosPartition43CcmEnd;
extern uint64_t pharosPartition43DataCcmEnd;
extern uint64_t pharosPartition43BssCcmEnd;

extern uint64_t pharosPartition43DataSram1LoadAddr;
extern uint8_t pharosPartition43DataSram1Size;
extern uint8_t pharosPartition43BssSram1Size;
extern uint64_t pharosPartition43Sram1Start;
extern uint64_t pharosPartition43DataSram1Start;
extern uint64_t pharosPartition43BssSram1Start;
extern uint64_t pharosPartition43Sram1End;
extern uint64_t pharosPartition43DataSram1End;
extern uint64_t pharosPartition43BssSram1End;

extern uint64_t pharosPartition43DataSram2LoadAddr;
extern uint8_t pharosPartition43DataSram2Size;
extern uint8_t pharosPartition43BssSram2Size;
extern uint64_t pharosPartition43Sram2Start;
extern uint64_t pharosPartition43DataSram2Start;
extern uint64_t pharosPartition43BssSram2Start;
extern uint64_t pharosPartition43Sram2End;
extern uint64_t pharosPartition43DataSram2End;
extern uint64_t pharosPartition43BssSram2End;

extern uint64_t pharosPartition43DataSram3LoadAddr;
extern uint8_t pharosPartition43DataSram3Size;
extern uint8_t pharosPartition43BssSram3Size;
extern uint64_t pharosPartition43Sram3Start;
extern uint64_t pharosPartition43DataSram3Start;
extern uint64_t pharosPartition43BssSram3Start;
extern uint64_t pharosPartition43Sram3End;
extern uint64_t pharosPartition43DataSram3End;
extern uint64_t pharosPartition43BssSram3End;

extern uint64_t pharosPartition43DataSramBackupLoadAddr;
extern uint8_t pharosPartition43DataSramBackupSize;
extern uint8_t pharosPartition43BssSramBackupSize;
extern uint64_t pharosPartition43SramBackupStart;
extern uint64_t pharosPartition43DataSramBackupStart;
extern uint64_t pharosPartition43BssSramBackupStart;
extern uint64_t pharosPartition43SramBackupEnd;
extern uint64_t pharosPartition43DataSramBackupEnd;
extern uint64_t pharosPartition43BssSramBackupEnd;

extern uint64_t pharosPartition43DataSdramLoadAddr;
extern uint8_t pharosPartition43DataSdramSize;
extern uint8_t pharosPartition43BssSdramSize;
extern uint64_t pharosPartition43SdramStart;
extern uint64_t pharosPartition43DataSdramStart;
extern uint64_t pharosPartition43BssSdramStart;
extern uint64_t pharosPartition43SdramEnd;
extern uint64_t pharosPartition43DataSdramEnd;
extern uint64_t pharosPartition43BssSdramEnd;

extern uint64_t pharosPartition44DataCcmLoadAddr;
extern uint8_t pharosPartition44DataCcmSize;
extern uint8_t pharosPartition44BssCcmSize;
extern uint64_t pharosPartition44CcmStart;
extern uint64_t pharosPartition44DataCcmStart;
extern uint64_t pharosPartition44BssCcmStart;
extern uint64_t pharosPartition44CcmEnd;
extern uint64_t pharosPartition44DataCcmEnd;
extern uint64_t pharosPartition44BssCcmEnd;

extern uint64_t pharosPartition44DataSram1LoadAddr;
extern uint8_t pharosPartition44DataSram1Size;
extern uint8_t pharosPartition44BssSram1Size;
extern uint64_t pharosPartition44Sram1Start;
extern uint64_t pharosPartition44DataSram1Start;
extern uint64_t pharosPartition44BssSram1Start;
extern uint64_t pharosPartition44Sram1End;
extern uint64_t pharosPartition44DataSram1End;
extern uint64_t pharosPartition44BssSram1End;

extern uint64_t pharosPartition44DataSram2LoadAddr;
extern uint8_t pharosPartition44DataSram2Size;
extern uint8_t pharosPartition44BssSram2Size;
extern uint64_t pharosPartition44Sram2Start;
extern uint64_t pharosPartition44DataSram2Start;
extern uint64_t pharosPartition44BssSram2Start;
extern uint64_t pharosPartition44Sram2End;
extern uint64_t pharosPartition44DataSram2End;
extern uint64_t pharosPartition44BssSram2End;

extern uint64_t pharosPartition44DataSram3LoadAddr;
extern uint8_t pharosPartition44DataSram3Size;
extern uint8_t pharosPartition44BssSram3Size;
extern uint64_t pharosPartition44Sram3Start;
extern uint64_t pharosPartition44DataSram3Start;
extern uint64_t pharosPartition44BssSram3Start;
extern uint64_t pharosPartition44Sram3End;
extern uint64_t pharosPartition44DataSram3End;
extern uint64_t pharosPartition44BssSram3End;

extern uint64_t pharosPartition44DataSramBackupLoadAddr;
extern uint8_t pharosPartition44DataSramBackupSize;
extern uint8_t pharosPartition44BssSramBackupSize;
extern uint64_t pharosPartition44SramBackupStart;
extern uint64_t pharosPartition44DataSramBackupStart;
extern uint64_t pharosPartition44BssSramBackupStart;
extern uint64_t pharosPartition44SramBackupEnd;
extern uint64_t pharosPartition44DataSramBackupEnd;
extern uint64_t pharosPartition44BssSramBackupEnd;

extern uint64_t pharosPartition44DataSdramLoadAddr;
extern uint8_t pharosPartition44DataSdramSize;
extern uint8_t pharosPartition44BssSdramSize;
extern uint64_t pharosPartition44SdramStart;
extern uint64_t pharosPartition44DataSdramStart;
extern uint64_t pharosPartition44BssSdramStart;
extern uint64_t pharosPartition44SdramEnd;
extern uint64_t pharosPartition44DataSdramEnd;
extern uint64_t pharosPartition44BssSdramEnd;

extern uint64_t pharosPartition45DataCcmLoadAddr;
extern uint8_t pharosPartition45DataCcmSize;
extern uint8_t pharosPartition45BssCcmSize;
extern uint64_t pharosPartition45CcmStart;
extern uint64_t pharosPartition45DataCcmStart;
extern uint64_t pharosPartition45BssCcmStart;
extern uint64_t pharosPartition45CcmEnd;
extern uint64_t pharosPartition45DataCcmEnd;
extern uint64_t pharosPartition45BssCcmEnd;

extern uint64_t pharosPartition45DataSram1LoadAddr;
extern uint8_t pharosPartition45DataSram1Size;
extern uint8_t pharosPartition45BssSram1Size;
extern uint64_t pharosPartition45Sram1Start;
extern uint64_t pharosPartition45DataSram1Start;
extern uint64_t pharosPartition45BssSram1Start;
extern uint64_t pharosPartition45Sram1End;
extern uint64_t pharosPartition45DataSram1End;
extern uint64_t pharosPartition45BssSram1End;

extern uint64_t pharosPartition45DataSram2LoadAddr;
extern uint8_t pharosPartition45DataSram2Size;
extern uint8_t pharosPartition45BssSram2Size;
extern uint64_t pharosPartition45Sram2Start;
extern uint64_t pharosPartition45DataSram2Start;
extern uint64_t pharosPartition45BssSram2Start;
extern uint64_t pharosPartition45Sram2End;
extern uint64_t pharosPartition45DataSram2End;
extern uint64_t pharosPartition45BssSram2End;

extern uint64_t pharosPartition45DataSram3LoadAddr;
extern uint8_t pharosPartition45DataSram3Size;
extern uint8_t pharosPartition45BssSram3Size;
extern uint64_t pharosPartition45Sram3Start;
extern uint64_t pharosPartition45DataSram3Start;
extern uint64_t pharosPartition45BssSram3Start;
extern uint64_t pharosPartition45Sram3End;
extern uint64_t pharosPartition45DataSram3End;
extern uint64_t pharosPartition45BssSram3End;

extern uint64_t pharosPartition45DataSramBackupLoadAddr;
extern uint8_t pharosPartition45DataSramBackupSize;
extern uint8_t pharosPartition45BssSramBackupSize;
extern uint64_t pharosPartition45SramBackupStart;
extern uint64_t pharosPartition45DataSramBackupStart;
extern uint64_t pharosPartition45BssSramBackupStart;
extern uint64_t pharosPartition45SramBackupEnd;
extern uint64_t pharosPartition45DataSramBackupEnd;
extern uint64_t pharosPartition45BssSramBackupEnd;

extern uint64_t pharosPartition45DataSdramLoadAddr;
extern uint8_t pharosPartition45DataSdramSize;
extern uint8_t pharosPartition45BssSdramSize;
extern uint64_t pharosPartition45SdramStart;
extern uint64_t pharosPartition45DataSdramStart;
extern uint64_t pharosPartition45BssSdramStart;
extern uint64_t pharosPartition45SdramEnd;
extern uint64_t pharosPartition45DataSdramEnd;
extern uint64_t pharosPartition45BssSdramEnd;

extern uint64_t pharosPartition46DataCcmLoadAddr;
extern uint8_t pharosPartition46DataCcmSize;
extern uint8_t pharosPartition46BssCcmSize;
extern uint64_t pharosPartition46CcmStart;
extern uint64_t pharosPartition46DataCcmStart;
extern uint64_t pharosPartition46BssCcmStart;
extern uint64_t pharosPartition46CcmEnd;
extern uint64_t pharosPartition46DataCcmEnd;
extern uint64_t pharosPartition46BssCcmEnd;

extern uint64_t pharosPartition46DataSram1LoadAddr;
extern uint8_t pharosPartition46DataSram1Size;
extern uint8_t pharosPartition46BssSram1Size;
extern uint64_t pharosPartition46Sram1Start;
extern uint64_t pharosPartition46DataSram1Start;
extern uint64_t pharosPartition46BssSram1Start;
extern uint64_t pharosPartition46Sram1End;
extern uint64_t pharosPartition46DataSram1End;
extern uint64_t pharosPartition46BssSram1End;

extern uint64_t pharosPartition46DataSram2LoadAddr;
extern uint8_t pharosPartition46DataSram2Size;
extern uint8_t pharosPartition46BssSram2Size;
extern uint64_t pharosPartition46Sram2Start;
extern uint64_t pharosPartition46DataSram2Start;
extern uint64_t pharosPartition46BssSram2Start;
extern uint64_t pharosPartition46Sram2End;
extern uint64_t pharosPartition46DataSram2End;
extern uint64_t pharosPartition46BssSram2End;

extern uint64_t pharosPartition46DataSram3LoadAddr;
extern uint8_t pharosPartition46DataSram3Size;
extern uint8_t pharosPartition46BssSram3Size;
extern uint64_t pharosPartition46Sram3Start;
extern uint64_t pharosPartition46DataSram3Start;
extern uint64_t pharosPartition46BssSram3Start;
extern uint64_t pharosPartition46Sram3End;
extern uint64_t pharosPartition46DataSram3End;
extern uint64_t pharosPartition46BssSram3End;

extern uint64_t pharosPartition46DataSramBackupLoadAddr;
extern uint8_t pharosPartition46DataSramBackupSize;
extern uint8_t pharosPartition46BssSramBackupSize;
extern uint64_t pharosPartition46SramBackupStart;
extern uint64_t pharosPartition46DataSramBackupStart;
extern uint64_t pharosPartition46BssSramBackupStart;
extern uint64_t pharosPartition46SramBackupEnd;
extern uint64_t pharosPartition46DataSramBackupEnd;
extern uint64_t pharosPartition46BssSramBackupEnd;

extern uint64_t pharosPartition46DataSdramLoadAddr;
extern uint8_t pharosPartition46DataSdramSize;
extern uint8_t pharosPartition46BssSdramSize;
extern uint64_t pharosPartition46SdramStart;
extern uint64_t pharosPartition46DataSdramStart;
extern uint64_t pharosPartition46BssSdramStart;
extern uint64_t pharosPartition46SdramEnd;
extern uint64_t pharosPartition46DataSdramEnd;
extern uint64_t pharosPartition46BssSdramEnd;

extern uint64_t pharosPartition47DataCcmLoadAddr;
extern uint8_t pharosPartition47DataCcmSize;
extern uint8_t pharosPartition47BssCcmSize;
extern uint64_t pharosPartition47CcmStart;
extern uint64_t pharosPartition47DataCcmStart;
extern uint64_t pharosPartition47BssCcmStart;
extern uint64_t pharosPartition47CcmEnd;
extern uint64_t pharosPartition47DataCcmEnd;
extern uint64_t pharosPartition47BssCcmEnd;

extern uint64_t pharosPartition47DataSram1LoadAddr;
extern uint8_t pharosPartition47DataSram1Size;
extern uint8_t pharosPartition47BssSram1Size;
extern uint64_t pharosPartition47Sram1Start;
extern uint64_t pharosPartition47DataSram1Start;
extern uint64_t pharosPartition47BssSram1Start;
extern uint64_t pharosPartition47Sram1End;
extern uint64_t pharosPartition47DataSram1End;
extern uint64_t pharosPartition47BssSram1End;

extern uint64_t pharosPartition47DataSram2LoadAddr;
extern uint8_t pharosPartition47DataSram2Size;
extern uint8_t pharosPartition47BssSram2Size;
extern uint64_t pharosPartition47Sram2Start;
extern uint64_t pharosPartition47DataSram2Start;
extern uint64_t pharosPartition47BssSram2Start;
extern uint64_t pharosPartition47Sram2End;
extern uint64_t pharosPartition47DataSram2End;
extern uint64_t pharosPartition47BssSram2End;

extern uint64_t pharosPartition47DataSram3LoadAddr;
extern uint8_t pharosPartition47DataSram3Size;
extern uint8_t pharosPartition47BssSram3Size;
extern uint64_t pharosPartition47Sram3Start;
extern uint64_t pharosPartition47DataSram3Start;
extern uint64_t pharosPartition47BssSram3Start;
extern uint64_t pharosPartition47Sram3End;
extern uint64_t pharosPartition47DataSram3End;
extern uint64_t pharosPartition47BssSram3End;

extern uint64_t pharosPartition47DataSramBackupLoadAddr;
extern uint8_t pharosPartition47DataSramBackupSize;
extern uint8_t pharosPartition47BssSramBackupSize;
extern uint64_t pharosPartition47SramBackupStart;
extern uint64_t pharosPartition47DataSramBackupStart;
extern uint64_t pharosPartition47BssSramBackupStart;
extern uint64_t pharosPartition47SramBackupEnd;
extern uint64_t pharosPartition47DataSramBackupEnd;
extern uint64_t pharosPartition47BssSramBackupEnd;

extern uint64_t pharosPartition47DataSdramLoadAddr;
extern uint8_t pharosPartition47DataSdramSize;
extern uint8_t pharosPartition47BssSdramSize;
extern uint64_t pharosPartition47SdramStart;
extern uint64_t pharosPartition47DataSdramStart;
extern uint64_t pharosPartition47BssSdramStart;
extern uint64_t pharosPartition47SdramEnd;
extern uint64_t pharosPartition47DataSdramEnd;
extern uint64_t pharosPartition47BssSdramEnd;

extern uint64_t pharosPartition48DataCcmLoadAddr;
extern uint8_t pharosPartition48DataCcmSize;
extern uint8_t pharosPartition48BssCcmSize;
extern uint64_t pharosPartition48CcmStart;
extern uint64_t pharosPartition48DataCcmStart;
extern uint64_t pharosPartition48BssCcmStart;
extern uint64_t pharosPartition48CcmEnd;
extern uint64_t pharosPartition48DataCcmEnd;
extern uint64_t pharosPartition48BssCcmEnd;

extern uint64_t pharosPartition48DataSram1LoadAddr;
extern uint8_t pharosPartition48DataSram1Size;
extern uint8_t pharosPartition48BssSram1Size;
extern uint64_t pharosPartition48Sram1Start;
extern uint64_t pharosPartition48DataSram1Start;
extern uint64_t pharosPartition48BssSram1Start;
extern uint64_t pharosPartition48Sram1End;
extern uint64_t pharosPartition48DataSram1End;
extern uint64_t pharosPartition48BssSram1End;

extern uint64_t pharosPartition48DataSram2LoadAddr;
extern uint8_t pharosPartition48DataSram2Size;
extern uint8_t pharosPartition48BssSram2Size;
extern uint64_t pharosPartition48Sram2Start;
extern uint64_t pharosPartition48DataSram2Start;
extern uint64_t pharosPartition48BssSram2Start;
extern uint64_t pharosPartition48Sram2End;
extern uint64_t pharosPartition48DataSram2End;
extern uint64_t pharosPartition48BssSram2End;

extern uint64_t pharosPartition48DataSram3LoadAddr;
extern uint8_t pharosPartition48DataSram3Size;
extern uint8_t pharosPartition48BssSram3Size;
extern uint64_t pharosPartition48Sram3Start;
extern uint64_t pharosPartition48DataSram3Start;
extern uint64_t pharosPartition48BssSram3Start;
extern uint64_t pharosPartition48Sram3End;
extern uint64_t pharosPartition48DataSram3End;
extern uint64_t pharosPartition48BssSram3End;

extern uint64_t pharosPartition48DataSramBackupLoadAddr;
extern uint8_t pharosPartition48DataSramBackupSize;
extern uint8_t pharosPartition48BssSramBackupSize;
extern uint64_t pharosPartition48SramBackupStart;
extern uint64_t pharosPartition48DataSramBackupStart;
extern uint64_t pharosPartition48BssSramBackupStart;
extern uint64_t pharosPartition48SramBackupEnd;
extern uint64_t pharosPartition48DataSramBackupEnd;
extern uint64_t pharosPartition48BssSramBackupEnd;

extern uint64_t pharosPartition48DataSdramLoadAddr;
extern uint8_t pharosPartition48DataSdramSize;
extern uint8_t pharosPartition48BssSdramSize;
extern uint64_t pharosPartition48SdramStart;
extern uint64_t pharosPartition48DataSdramStart;
extern uint64_t pharosPartition48BssSdramStart;
extern uint64_t pharosPartition48SdramEnd;
extern uint64_t pharosPartition48DataSdramEnd;
extern uint64_t pharosPartition48BssSdramEnd;

extern uint64_t pharosPartition49DataCcmLoadAddr;
extern uint8_t pharosPartition49DataCcmSize;
extern uint8_t pharosPartition49BssCcmSize;
extern uint64_t pharosPartition49CcmStart;
extern uint64_t pharosPartition49DataCcmStart;
extern uint64_t pharosPartition49BssCcmStart;
extern uint64_t pharosPartition49CcmEnd;
extern uint64_t pharosPartition49DataCcmEnd;
extern uint64_t pharosPartition49BssCcmEnd;

extern uint64_t pharosPartition49DataSram1LoadAddr;
extern uint8_t pharosPartition49DataSram1Size;
extern uint8_t pharosPartition49BssSram1Size;
extern uint64_t pharosPartition49Sram1Start;
extern uint64_t pharosPartition49DataSram1Start;
extern uint64_t pharosPartition49BssSram1Start;
extern uint64_t pharosPartition49Sram1End;
extern uint64_t pharosPartition49DataSram1End;
extern uint64_t pharosPartition49BssSram1End;

extern uint64_t pharosPartition49DataSram2LoadAddr;
extern uint8_t pharosPartition49DataSram2Size;
extern uint8_t pharosPartition49BssSram2Size;
extern uint64_t pharosPartition49Sram2Start;
extern uint64_t pharosPartition49DataSram2Start;
extern uint64_t pharosPartition49BssSram2Start;
extern uint64_t pharosPartition49Sram2End;
extern uint64_t pharosPartition49DataSram2End;
extern uint64_t pharosPartition49BssSram2End;

extern uint64_t pharosPartition49DataSram3LoadAddr;
extern uint8_t pharosPartition49DataSram3Size;
extern uint8_t pharosPartition49BssSram3Size;
extern uint64_t pharosPartition49Sram3Start;
extern uint64_t pharosPartition49DataSram3Start;
extern uint64_t pharosPartition49BssSram3Start;
extern uint64_t pharosPartition49Sram3End;
extern uint64_t pharosPartition49DataSram3End;
extern uint64_t pharosPartition49BssSram3End;

extern uint64_t pharosPartition49DataSramBackupLoadAddr;
extern uint8_t pharosPartition49DataSramBackupSize;
extern uint8_t pharosPartition49BssSramBackupSize;
extern uint64_t pharosPartition49SramBackupStart;
extern uint64_t pharosPartition49DataSramBackupStart;
extern uint64_t pharosPartition49BssSramBackupStart;
extern uint64_t pharosPartition49SramBackupEnd;
extern uint64_t pharosPartition49DataSramBackupEnd;
extern uint64_t pharosPartition49BssSramBackupEnd;

extern uint64_t pharosPartition49DataSdramLoadAddr;
extern uint8_t pharosPartition49DataSdramSize;
extern uint8_t pharosPartition49BssSdramSize;
extern uint64_t pharosPartition49SdramStart;
extern uint64_t pharosPartition49DataSdramStart;
extern uint64_t pharosPartition49BssSdramStart;
extern uint64_t pharosPartition49SdramEnd;
extern uint64_t pharosPartition49DataSdramEnd;
extern uint64_t pharosPartition49BssSdramEnd;

extern uint64_t pharosPartition50DataCcmLoadAddr;
extern uint8_t pharosPartition50DataCcmSize;
extern uint8_t pharosPartition50BssCcmSize;
extern uint64_t pharosPartition50CcmStart;
extern uint64_t pharosPartition50DataCcmStart;
extern uint64_t pharosPartition50BssCcmStart;
extern uint64_t pharosPartition50CcmEnd;
extern uint64_t pharosPartition50DataCcmEnd;
extern uint64_t pharosPartition50BssCcmEnd;

extern uint64_t pharosPartition50DataSram1LoadAddr;
extern uint8_t pharosPartition50DataSram1Size;
extern uint8_t pharosPartition50BssSram1Size;
extern uint64_t pharosPartition50Sram1Start;
extern uint64_t pharosPartition50DataSram1Start;
extern uint64_t pharosPartition50BssSram1Start;
extern uint64_t pharosPartition50Sram1End;
extern uint64_t pharosPartition50DataSram1End;
extern uint64_t pharosPartition50BssSram1End;

extern uint64_t pharosPartition50DataSram2LoadAddr;
extern uint8_t pharosPartition50DataSram2Size;
extern uint8_t pharosPartition50BssSram2Size;
extern uint64_t pharosPartition50Sram2Start;
extern uint64_t pharosPartition50DataSram2Start;
extern uint64_t pharosPartition50BssSram2Start;
extern uint64_t pharosPartition50Sram2End;
extern uint64_t pharosPartition50DataSram2End;
extern uint64_t pharosPartition50BssSram2End;

extern uint64_t pharosPartition50DataSram3LoadAddr;
extern uint8_t pharosPartition50DataSram3Size;
extern uint8_t pharosPartition50BssSram3Size;
extern uint64_t pharosPartition50Sram3Start;
extern uint64_t pharosPartition50DataSram3Start;
extern uint64_t pharosPartition50BssSram3Start;
extern uint64_t pharosPartition50Sram3End;
extern uint64_t pharosPartition50DataSram3End;
extern uint64_t pharosPartition50BssSram3End;

extern uint64_t pharosPartition50DataSramBackupLoadAddr;
extern uint8_t pharosPartition50DataSramBackupSize;
extern uint8_t pharosPartition50BssSramBackupSize;
extern uint64_t pharosPartition50SramBackupStart;
extern uint64_t pharosPartition50DataSramBackupStart;
extern uint64_t pharosPartition50BssSramBackupStart;
extern uint64_t pharosPartition50SramBackupEnd;
extern uint64_t pharosPartition50DataSramBackupEnd;
extern uint64_t pharosPartition50BssSramBackupEnd;

extern uint64_t pharosPartition50DataSdramLoadAddr;
extern uint8_t pharosPartition50DataSdramSize;
extern uint8_t pharosPartition50BssSdramSize;
extern uint64_t pharosPartition50SdramStart;
extern uint64_t pharosPartition50DataSdramStart;
extern uint64_t pharosPartition50BssSdramStart;
extern uint64_t pharosPartition50SdramEnd;
extern uint64_t pharosPartition50DataSdramEnd;
extern uint64_t pharosPartition50BssSdramEnd;

extern uint64_t pharosPartition51DataCcmLoadAddr;
extern uint8_t pharosPartition51DataCcmSize;
extern uint8_t pharosPartition51BssCcmSize;
extern uint64_t pharosPartition51CcmStart;
extern uint64_t pharosPartition51DataCcmStart;
extern uint64_t pharosPartition51BssCcmStart;
extern uint64_t pharosPartition51CcmEnd;
extern uint64_t pharosPartition51DataCcmEnd;
extern uint64_t pharosPartition51BssCcmEnd;

extern uint64_t pharosPartition51DataSram1LoadAddr;
extern uint8_t pharosPartition51DataSram1Size;
extern uint8_t pharosPartition51BssSram1Size;
extern uint64_t pharosPartition51Sram1Start;
extern uint64_t pharosPartition51DataSram1Start;
extern uint64_t pharosPartition51BssSram1Start;
extern uint64_t pharosPartition51Sram1End;
extern uint64_t pharosPartition51DataSram1End;
extern uint64_t pharosPartition51BssSram1End;

extern uint64_t pharosPartition51DataSram2LoadAddr;
extern uint8_t pharosPartition51DataSram2Size;
extern uint8_t pharosPartition51BssSram2Size;
extern uint64_t pharosPartition51Sram2Start;
extern uint64_t pharosPartition51DataSram2Start;
extern uint64_t pharosPartition51BssSram2Start;
extern uint64_t pharosPartition51Sram2End;
extern uint64_t pharosPartition51DataSram2End;
extern uint64_t pharosPartition51BssSram2End;

extern uint64_t pharosPartition51DataSram3LoadAddr;
extern uint8_t pharosPartition51DataSram3Size;
extern uint8_t pharosPartition51BssSram3Size;
extern uint64_t pharosPartition51Sram3Start;
extern uint64_t pharosPartition51DataSram3Start;
extern uint64_t pharosPartition51BssSram3Start;
extern uint64_t pharosPartition51Sram3End;
extern uint64_t pharosPartition51DataSram3End;
extern uint64_t pharosPartition51BssSram3End;

extern uint64_t pharosPartition51DataSramBackupLoadAddr;
extern uint8_t pharosPartition51DataSramBackupSize;
extern uint8_t pharosPartition51BssSramBackupSize;
extern uint64_t pharosPartition51SramBackupStart;
extern uint64_t pharosPartition51DataSramBackupStart;
extern uint64_t pharosPartition51BssSramBackupStart;
extern uint64_t pharosPartition51SramBackupEnd;
extern uint64_t pharosPartition51DataSramBackupEnd;
extern uint64_t pharosPartition51BssSramBackupEnd;

extern uint64_t pharosPartition51DataSdramLoadAddr;
extern uint8_t pharosPartition51DataSdramSize;
extern uint8_t pharosPartition51BssSdramSize;
extern uint64_t pharosPartition51SdramStart;
extern uint64_t pharosPartition51DataSdramStart;
extern uint64_t pharosPartition51BssSdramStart;
extern uint64_t pharosPartition51SdramEnd;
extern uint64_t pharosPartition51DataSdramEnd;
extern uint64_t pharosPartition51BssSdramEnd;

extern uint64_t pharosPartition52DataCcmLoadAddr;
extern uint8_t pharosPartition52DataCcmSize;
extern uint8_t pharosPartition52BssCcmSize;
extern uint64_t pharosPartition52CcmStart;
extern uint64_t pharosPartition52DataCcmStart;
extern uint64_t pharosPartition52BssCcmStart;
extern uint64_t pharosPartition52CcmEnd;
extern uint64_t pharosPartition52DataCcmEnd;
extern uint64_t pharosPartition52BssCcmEnd;

extern uint64_t pharosPartition52DataSram1LoadAddr;
extern uint8_t pharosPartition52DataSram1Size;
extern uint8_t pharosPartition52BssSram1Size;
extern uint64_t pharosPartition52Sram1Start;
extern uint64_t pharosPartition52DataSram1Start;
extern uint64_t pharosPartition52BssSram1Start;
extern uint64_t pharosPartition52Sram1End;
extern uint64_t pharosPartition52DataSram1End;
extern uint64_t pharosPartition52BssSram1End;

extern uint64_t pharosPartition52DataSram2LoadAddr;
extern uint8_t pharosPartition52DataSram2Size;
extern uint8_t pharosPartition52BssSram2Size;
extern uint64_t pharosPartition52Sram2Start;
extern uint64_t pharosPartition52DataSram2Start;
extern uint64_t pharosPartition52BssSram2Start;
extern uint64_t pharosPartition52Sram2End;
extern uint64_t pharosPartition52DataSram2End;
extern uint64_t pharosPartition52BssSram2End;

extern uint64_t pharosPartition52DataSram3LoadAddr;
extern uint8_t pharosPartition52DataSram3Size;
extern uint8_t pharosPartition52BssSram3Size;
extern uint64_t pharosPartition52Sram3Start;
extern uint64_t pharosPartition52DataSram3Start;
extern uint64_t pharosPartition52BssSram3Start;
extern uint64_t pharosPartition52Sram3End;
extern uint64_t pharosPartition52DataSram3End;
extern uint64_t pharosPartition52BssSram3End;

extern uint64_t pharosPartition52DataSramBackupLoadAddr;
extern uint8_t pharosPartition52DataSramBackupSize;
extern uint8_t pharosPartition52BssSramBackupSize;
extern uint64_t pharosPartition52SramBackupStart;
extern uint64_t pharosPartition52DataSramBackupStart;
extern uint64_t pharosPartition52BssSramBackupStart;
extern uint64_t pharosPartition52SramBackupEnd;
extern uint64_t pharosPartition52DataSramBackupEnd;
extern uint64_t pharosPartition52BssSramBackupEnd;

extern uint64_t pharosPartition52DataSdramLoadAddr;
extern uint8_t pharosPartition52DataSdramSize;
extern uint8_t pharosPartition52BssSdramSize;
extern uint64_t pharosPartition52SdramStart;
extern uint64_t pharosPartition52DataSdramStart;
extern uint64_t pharosPartition52BssSdramStart;
extern uint64_t pharosPartition52SdramEnd;
extern uint64_t pharosPartition52DataSdramEnd;
extern uint64_t pharosPartition52BssSdramEnd;

extern uint64_t pharosPartition53DataCcmLoadAddr;
extern uint8_t pharosPartition53DataCcmSize;
extern uint8_t pharosPartition53BssCcmSize;
extern uint64_t pharosPartition53CcmStart;
extern uint64_t pharosPartition53DataCcmStart;
extern uint64_t pharosPartition53BssCcmStart;
extern uint64_t pharosPartition53CcmEnd;
extern uint64_t pharosPartition53DataCcmEnd;
extern uint64_t pharosPartition53BssCcmEnd;

extern uint64_t pharosPartition53DataSram1LoadAddr;
extern uint8_t pharosPartition53DataSram1Size;
extern uint8_t pharosPartition53BssSram1Size;
extern uint64_t pharosPartition53Sram1Start;
extern uint64_t pharosPartition53DataSram1Start;
extern uint64_t pharosPartition53BssSram1Start;
extern uint64_t pharosPartition53Sram1End;
extern uint64_t pharosPartition53DataSram1End;
extern uint64_t pharosPartition53BssSram1End;

extern uint64_t pharosPartition53DataSram2LoadAddr;
extern uint8_t pharosPartition53DataSram2Size;
extern uint8_t pharosPartition53BssSram2Size;
extern uint64_t pharosPartition53Sram2Start;
extern uint64_t pharosPartition53DataSram2Start;
extern uint64_t pharosPartition53BssSram2Start;
extern uint64_t pharosPartition53Sram2End;
extern uint64_t pharosPartition53DataSram2End;
extern uint64_t pharosPartition53BssSram2End;

extern uint64_t pharosPartition53DataSram3LoadAddr;
extern uint8_t pharosPartition53DataSram3Size;
extern uint8_t pharosPartition53BssSram3Size;
extern uint64_t pharosPartition53Sram3Start;
extern uint64_t pharosPartition53DataSram3Start;
extern uint64_t pharosPartition53BssSram3Start;
extern uint64_t pharosPartition53Sram3End;
extern uint64_t pharosPartition53DataSram3End;
extern uint64_t pharosPartition53BssSram3End;

extern uint64_t pharosPartition53DataSramBackupLoadAddr;
extern uint8_t pharosPartition53DataSramBackupSize;
extern uint8_t pharosPartition53BssSramBackupSize;
extern uint64_t pharosPartition53SramBackupStart;
extern uint64_t pharosPartition53DataSramBackupStart;
extern uint64_t pharosPartition53BssSramBackupStart;
extern uint64_t pharosPartition53SramBackupEnd;
extern uint64_t pharosPartition53DataSramBackupEnd;
extern uint64_t pharosPartition53BssSramBackupEnd;

extern uint64_t pharosPartition53DataSdramLoadAddr;
extern uint8_t pharosPartition53DataSdramSize;
extern uint8_t pharosPartition53BssSdramSize;
extern uint64_t pharosPartition53SdramStart;
extern uint64_t pharosPartition53DataSdramStart;
extern uint64_t pharosPartition53BssSdramStart;
extern uint64_t pharosPartition53SdramEnd;
extern uint64_t pharosPartition53DataSdramEnd;
extern uint64_t pharosPartition53BssSdramEnd;

extern uint64_t pharosPartition54DataCcmLoadAddr;
extern uint8_t pharosPartition54DataCcmSize;
extern uint8_t pharosPartition54BssCcmSize;
extern uint64_t pharosPartition54CcmStart;
extern uint64_t pharosPartition54DataCcmStart;
extern uint64_t pharosPartition54BssCcmStart;
extern uint64_t pharosPartition54CcmEnd;
extern uint64_t pharosPartition54DataCcmEnd;
extern uint64_t pharosPartition54BssCcmEnd;

extern uint64_t pharosPartition54DataSram1LoadAddr;
extern uint8_t pharosPartition54DataSram1Size;
extern uint8_t pharosPartition54BssSram1Size;
extern uint64_t pharosPartition54Sram1Start;
extern uint64_t pharosPartition54DataSram1Start;
extern uint64_t pharosPartition54BssSram1Start;
extern uint64_t pharosPartition54Sram1End;
extern uint64_t pharosPartition54DataSram1End;
extern uint64_t pharosPartition54BssSram1End;

extern uint64_t pharosPartition54DataSram2LoadAddr;
extern uint8_t pharosPartition54DataSram2Size;
extern uint8_t pharosPartition54BssSram2Size;
extern uint64_t pharosPartition54Sram2Start;
extern uint64_t pharosPartition54DataSram2Start;
extern uint64_t pharosPartition54BssSram2Start;
extern uint64_t pharosPartition54Sram2End;
extern uint64_t pharosPartition54DataSram2End;
extern uint64_t pharosPartition54BssSram2End;

extern uint64_t pharosPartition54DataSram3LoadAddr;
extern uint8_t pharosPartition54DataSram3Size;
extern uint8_t pharosPartition54BssSram3Size;
extern uint64_t pharosPartition54Sram3Start;
extern uint64_t pharosPartition54DataSram3Start;
extern uint64_t pharosPartition54BssSram3Start;
extern uint64_t pharosPartition54Sram3End;
extern uint64_t pharosPartition54DataSram3End;
extern uint64_t pharosPartition54BssSram3End;

extern uint64_t pharosPartition54DataSramBackupLoadAddr;
extern uint8_t pharosPartition54DataSramBackupSize;
extern uint8_t pharosPartition54BssSramBackupSize;
extern uint64_t pharosPartition54SramBackupStart;
extern uint64_t pharosPartition54DataSramBackupStart;
extern uint64_t pharosPartition54BssSramBackupStart;
extern uint64_t pharosPartition54SramBackupEnd;
extern uint64_t pharosPartition54DataSramBackupEnd;
extern uint64_t pharosPartition54BssSramBackupEnd;

extern uint64_t pharosPartition54DataSdramLoadAddr;
extern uint8_t pharosPartition54DataSdramSize;
extern uint8_t pharosPartition54BssSdramSize;
extern uint64_t pharosPartition54SdramStart;
extern uint64_t pharosPartition54DataSdramStart;
extern uint64_t pharosPartition54BssSdramStart;
extern uint64_t pharosPartition54SdramEnd;
extern uint64_t pharosPartition54DataSdramEnd;
extern uint64_t pharosPartition54BssSdramEnd;

extern uint64_t pharosPartition55DataCcmLoadAddr;
extern uint8_t pharosPartition55DataCcmSize;
extern uint8_t pharosPartition55BssCcmSize;
extern uint64_t pharosPartition55CcmStart;
extern uint64_t pharosPartition55DataCcmStart;
extern uint64_t pharosPartition55BssCcmStart;
extern uint64_t pharosPartition55CcmEnd;
extern uint64_t pharosPartition55DataCcmEnd;
extern uint64_t pharosPartition55BssCcmEnd;

extern uint64_t pharosPartition55DataSram1LoadAddr;
extern uint8_t pharosPartition55DataSram1Size;
extern uint8_t pharosPartition55BssSram1Size;
extern uint64_t pharosPartition55Sram1Start;
extern uint64_t pharosPartition55DataSram1Start;
extern uint64_t pharosPartition55BssSram1Start;
extern uint64_t pharosPartition55Sram1End;
extern uint64_t pharosPartition55DataSram1End;
extern uint64_t pharosPartition55BssSram1End;

extern uint64_t pharosPartition55DataSram2LoadAddr;
extern uint8_t pharosPartition55DataSram2Size;
extern uint8_t pharosPartition55BssSram2Size;
extern uint64_t pharosPartition55Sram2Start;
extern uint64_t pharosPartition55DataSram2Start;
extern uint64_t pharosPartition55BssSram2Start;
extern uint64_t pharosPartition55Sram2End;
extern uint64_t pharosPartition55DataSram2End;
extern uint64_t pharosPartition55BssSram2End;

extern uint64_t pharosPartition55DataSram3LoadAddr;
extern uint8_t pharosPartition55DataSram3Size;
extern uint8_t pharosPartition55BssSram3Size;
extern uint64_t pharosPartition55Sram3Start;
extern uint64_t pharosPartition55DataSram3Start;
extern uint64_t pharosPartition55BssSram3Start;
extern uint64_t pharosPartition55Sram3End;
extern uint64_t pharosPartition55DataSram3End;
extern uint64_t pharosPartition55BssSram3End;

extern uint64_t pharosPartition55DataSramBackupLoadAddr;
extern uint8_t pharosPartition55DataSramBackupSize;
extern uint8_t pharosPartition55BssSramBackupSize;
extern uint64_t pharosPartition55SramBackupStart;
extern uint64_t pharosPartition55DataSramBackupStart;
extern uint64_t pharosPartition55BssSramBackupStart;
extern uint64_t pharosPartition55SramBackupEnd;
extern uint64_t pharosPartition55DataSramBackupEnd;
extern uint64_t pharosPartition55BssSramBackupEnd;

extern uint64_t pharosPartition55DataSdramLoadAddr;
extern uint8_t pharosPartition55DataSdramSize;
extern uint8_t pharosPartition55BssSdramSize;
extern uint64_t pharosPartition55SdramStart;
extern uint64_t pharosPartition55DataSdramStart;
extern uint64_t pharosPartition55BssSdramStart;
extern uint64_t pharosPartition55SdramEnd;
extern uint64_t pharosPartition55DataSdramEnd;
extern uint64_t pharosPartition55BssSdramEnd;

extern uint64_t pharosPartition56DataCcmLoadAddr;
extern uint8_t pharosPartition56DataCcmSize;
extern uint8_t pharosPartition56BssCcmSize;
extern uint64_t pharosPartition56CcmStart;
extern uint64_t pharosPartition56DataCcmStart;
extern uint64_t pharosPartition56BssCcmStart;
extern uint64_t pharosPartition56CcmEnd;
extern uint64_t pharosPartition56DataCcmEnd;
extern uint64_t pharosPartition56BssCcmEnd;

extern uint64_t pharosPartition56DataSram1LoadAddr;
extern uint8_t pharosPartition56DataSram1Size;
extern uint8_t pharosPartition56BssSram1Size;
extern uint64_t pharosPartition56Sram1Start;
extern uint64_t pharosPartition56DataSram1Start;
extern uint64_t pharosPartition56BssSram1Start;
extern uint64_t pharosPartition56Sram1End;
extern uint64_t pharosPartition56DataSram1End;
extern uint64_t pharosPartition56BssSram1End;

extern uint64_t pharosPartition56DataSram2LoadAddr;
extern uint8_t pharosPartition56DataSram2Size;
extern uint8_t pharosPartition56BssSram2Size;
extern uint64_t pharosPartition56Sram2Start;
extern uint64_t pharosPartition56DataSram2Start;
extern uint64_t pharosPartition56BssSram2Start;
extern uint64_t pharosPartition56Sram2End;
extern uint64_t pharosPartition56DataSram2End;
extern uint64_t pharosPartition56BssSram2End;

extern uint64_t pharosPartition56DataSram3LoadAddr;
extern uint8_t pharosPartition56DataSram3Size;
extern uint8_t pharosPartition56BssSram3Size;
extern uint64_t pharosPartition56Sram3Start;
extern uint64_t pharosPartition56DataSram3Start;
extern uint64_t pharosPartition56BssSram3Start;
extern uint64_t pharosPartition56Sram3End;
extern uint64_t pharosPartition56DataSram3End;
extern uint64_t pharosPartition56BssSram3End;

extern uint64_t pharosPartition56DataSramBackupLoadAddr;
extern uint8_t pharosPartition56DataSramBackupSize;
extern uint8_t pharosPartition56BssSramBackupSize;
extern uint64_t pharosPartition56SramBackupStart;
extern uint64_t pharosPartition56DataSramBackupStart;
extern uint64_t pharosPartition56BssSramBackupStart;
extern uint64_t pharosPartition56SramBackupEnd;
extern uint64_t pharosPartition56DataSramBackupEnd;
extern uint64_t pharosPartition56BssSramBackupEnd;

extern uint64_t pharosPartition56DataSdramLoadAddr;
extern uint8_t pharosPartition56DataSdramSize;
extern uint8_t pharosPartition56BssSdramSize;
extern uint64_t pharosPartition56SdramStart;
extern uint64_t pharosPartition56DataSdramStart;
extern uint64_t pharosPartition56BssSdramStart;
extern uint64_t pharosPartition56SdramEnd;
extern uint64_t pharosPartition56DataSdramEnd;
extern uint64_t pharosPartition56BssSdramEnd;

extern uint64_t pharosPartition57DataCcmLoadAddr;
extern uint8_t pharosPartition57DataCcmSize;
extern uint8_t pharosPartition57BssCcmSize;
extern uint64_t pharosPartition57CcmStart;
extern uint64_t pharosPartition57DataCcmStart;
extern uint64_t pharosPartition57BssCcmStart;
extern uint64_t pharosPartition57CcmEnd;
extern uint64_t pharosPartition57DataCcmEnd;
extern uint64_t pharosPartition57BssCcmEnd;

extern uint64_t pharosPartition57DataSram1LoadAddr;
extern uint8_t pharosPartition57DataSram1Size;
extern uint8_t pharosPartition57BssSram1Size;
extern uint64_t pharosPartition57Sram1Start;
extern uint64_t pharosPartition57DataSram1Start;
extern uint64_t pharosPartition57BssSram1Start;
extern uint64_t pharosPartition57Sram1End;
extern uint64_t pharosPartition57DataSram1End;
extern uint64_t pharosPartition57BssSram1End;

extern uint64_t pharosPartition57DataSram2LoadAddr;
extern uint8_t pharosPartition57DataSram2Size;
extern uint8_t pharosPartition57BssSram2Size;
extern uint64_t pharosPartition57Sram2Start;
extern uint64_t pharosPartition57DataSram2Start;
extern uint64_t pharosPartition57BssSram2Start;
extern uint64_t pharosPartition57Sram2End;
extern uint64_t pharosPartition57DataSram2End;
extern uint64_t pharosPartition57BssSram2End;

extern uint64_t pharosPartition57DataSram3LoadAddr;
extern uint8_t pharosPartition57DataSram3Size;
extern uint8_t pharosPartition57BssSram3Size;
extern uint64_t pharosPartition57Sram3Start;
extern uint64_t pharosPartition57DataSram3Start;
extern uint64_t pharosPartition57BssSram3Start;
extern uint64_t pharosPartition57Sram3End;
extern uint64_t pharosPartition57DataSram3End;
extern uint64_t pharosPartition57BssSram3End;

extern uint64_t pharosPartition57DataSramBackupLoadAddr;
extern uint8_t pharosPartition57DataSramBackupSize;
extern uint8_t pharosPartition57BssSramBackupSize;
extern uint64_t pharosPartition57SramBackupStart;
extern uint64_t pharosPartition57DataSramBackupStart;
extern uint64_t pharosPartition57BssSramBackupStart;
extern uint64_t pharosPartition57SramBackupEnd;
extern uint64_t pharosPartition57DataSramBackupEnd;
extern uint64_t pharosPartition57BssSramBackupEnd;

extern uint64_t pharosPartition57DataSdramLoadAddr;
extern uint8_t pharosPartition57DataSdramSize;
extern uint8_t pharosPartition57BssSdramSize;
extern uint64_t pharosPartition57SdramStart;
extern uint64_t pharosPartition57DataSdramStart;
extern uint64_t pharosPartition57BssSdramStart;
extern uint64_t pharosPartition57SdramEnd;
extern uint64_t pharosPartition57DataSdramEnd;
extern uint64_t pharosPartition57BssSdramEnd;

extern uint64_t pharosPartition58DataCcmLoadAddr;
extern uint8_t pharosPartition58DataCcmSize;
extern uint8_t pharosPartition58BssCcmSize;
extern uint64_t pharosPartition58CcmStart;
extern uint64_t pharosPartition58DataCcmStart;
extern uint64_t pharosPartition58BssCcmStart;
extern uint64_t pharosPartition58CcmEnd;
extern uint64_t pharosPartition58DataCcmEnd;
extern uint64_t pharosPartition58BssCcmEnd;

extern uint64_t pharosPartition58DataSram1LoadAddr;
extern uint8_t pharosPartition58DataSram1Size;
extern uint8_t pharosPartition58BssSram1Size;
extern uint64_t pharosPartition58Sram1Start;
extern uint64_t pharosPartition58DataSram1Start;
extern uint64_t pharosPartition58BssSram1Start;
extern uint64_t pharosPartition58Sram1End;
extern uint64_t pharosPartition58DataSram1End;
extern uint64_t pharosPartition58BssSram1End;

extern uint64_t pharosPartition58DataSram2LoadAddr;
extern uint8_t pharosPartition58DataSram2Size;
extern uint8_t pharosPartition58BssSram2Size;
extern uint64_t pharosPartition58Sram2Start;
extern uint64_t pharosPartition58DataSram2Start;
extern uint64_t pharosPartition58BssSram2Start;
extern uint64_t pharosPartition58Sram2End;
extern uint64_t pharosPartition58DataSram2End;
extern uint64_t pharosPartition58BssSram2End;

extern uint64_t pharosPartition58DataSram3LoadAddr;
extern uint8_t pharosPartition58DataSram3Size;
extern uint8_t pharosPartition58BssSram3Size;
extern uint64_t pharosPartition58Sram3Start;
extern uint64_t pharosPartition58DataSram3Start;
extern uint64_t pharosPartition58BssSram3Start;
extern uint64_t pharosPartition58Sram3End;
extern uint64_t pharosPartition58DataSram3End;
extern uint64_t pharosPartition58BssSram3End;

extern uint64_t pharosPartition58DataSramBackupLoadAddr;
extern uint8_t pharosPartition58DataSramBackupSize;
extern uint8_t pharosPartition58BssSramBackupSize;
extern uint64_t pharosPartition58SramBackupStart;
extern uint64_t pharosPartition58DataSramBackupStart;
extern uint64_t pharosPartition58BssSramBackupStart;
extern uint64_t pharosPartition58SramBackupEnd;
extern uint64_t pharosPartition58DataSramBackupEnd;
extern uint64_t pharosPartition58BssSramBackupEnd;

extern uint64_t pharosPartition58DataSdramLoadAddr;
extern uint8_t pharosPartition58DataSdramSize;
extern uint8_t pharosPartition58BssSdramSize;
extern uint64_t pharosPartition58SdramStart;
extern uint64_t pharosPartition58DataSdramStart;
extern uint64_t pharosPartition58BssSdramStart;
extern uint64_t pharosPartition58SdramEnd;
extern uint64_t pharosPartition58DataSdramEnd;
extern uint64_t pharosPartition58BssSdramEnd;

extern uint64_t pharosPartition59DataCcmLoadAddr;
extern uint8_t pharosPartition59DataCcmSize;
extern uint8_t pharosPartition59BssCcmSize;
extern uint64_t pharosPartition59CcmStart;
extern uint64_t pharosPartition59DataCcmStart;
extern uint64_t pharosPartition59BssCcmStart;
extern uint64_t pharosPartition59CcmEnd;
extern uint64_t pharosPartition59DataCcmEnd;
extern uint64_t pharosPartition59BssCcmEnd;

extern uint64_t pharosPartition59DataSram1LoadAddr;
extern uint8_t pharosPartition59DataSram1Size;
extern uint8_t pharosPartition59BssSram1Size;
extern uint64_t pharosPartition59Sram1Start;
extern uint64_t pharosPartition59DataSram1Start;
extern uint64_t pharosPartition59BssSram1Start;
extern uint64_t pharosPartition59Sram1End;
extern uint64_t pharosPartition59DataSram1End;
extern uint64_t pharosPartition59BssSram1End;

extern uint64_t pharosPartition59DataSram2LoadAddr;
extern uint8_t pharosPartition59DataSram2Size;
extern uint8_t pharosPartition59BssSram2Size;
extern uint64_t pharosPartition59Sram2Start;
extern uint64_t pharosPartition59DataSram2Start;
extern uint64_t pharosPartition59BssSram2Start;
extern uint64_t pharosPartition59Sram2End;
extern uint64_t pharosPartition59DataSram2End;
extern uint64_t pharosPartition59BssSram2End;

extern uint64_t pharosPartition59DataSram3LoadAddr;
extern uint8_t pharosPartition59DataSram3Size;
extern uint8_t pharosPartition59BssSram3Size;
extern uint64_t pharosPartition59Sram3Start;
extern uint64_t pharosPartition59DataSram3Start;
extern uint64_t pharosPartition59BssSram3Start;
extern uint64_t pharosPartition59Sram3End;
extern uint64_t pharosPartition59DataSram3End;
extern uint64_t pharosPartition59BssSram3End;

extern uint64_t pharosPartition59DataSramBackupLoadAddr;
extern uint8_t pharosPartition59DataSramBackupSize;
extern uint8_t pharosPartition59BssSramBackupSize;
extern uint64_t pharosPartition59SramBackupStart;
extern uint64_t pharosPartition59DataSramBackupStart;
extern uint64_t pharosPartition59BssSramBackupStart;
extern uint64_t pharosPartition59SramBackupEnd;
extern uint64_t pharosPartition59DataSramBackupEnd;
extern uint64_t pharosPartition59BssSramBackupEnd;

extern uint64_t pharosPartition59DataSdramLoadAddr;
extern uint8_t pharosPartition59DataSdramSize;
extern uint8_t pharosPartition59BssSdramSize;
extern uint64_t pharosPartition59SdramStart;
extern uint64_t pharosPartition59DataSdramStart;
extern uint64_t pharosPartition59BssSdramStart;
extern uint64_t pharosPartition59SdramEnd;
extern uint64_t pharosPartition59DataSdramEnd;
extern uint64_t pharosPartition59BssSdramEnd;

extern uint64_t pharosPartition60DataCcmLoadAddr;
extern uint8_t pharosPartition60DataCcmSize;
extern uint8_t pharosPartition60BssCcmSize;
extern uint64_t pharosPartition60CcmStart;
extern uint64_t pharosPartition60DataCcmStart;
extern uint64_t pharosPartition60BssCcmStart;
extern uint64_t pharosPartition60CcmEnd;
extern uint64_t pharosPartition60DataCcmEnd;
extern uint64_t pharosPartition60BssCcmEnd;

extern uint64_t pharosPartition60DataSram1LoadAddr;
extern uint8_t pharosPartition60DataSram1Size;
extern uint8_t pharosPartition60BssSram1Size;
extern uint64_t pharosPartition60Sram1Start;
extern uint64_t pharosPartition60DataSram1Start;
extern uint64_t pharosPartition60BssSram1Start;
extern uint64_t pharosPartition60Sram1End;
extern uint64_t pharosPartition60DataSram1End;
extern uint64_t pharosPartition60BssSram1End;

extern uint64_t pharosPartition60DataSram2LoadAddr;
extern uint8_t pharosPartition60DataSram2Size;
extern uint8_t pharosPartition60BssSram2Size;
extern uint64_t pharosPartition60Sram2Start;
extern uint64_t pharosPartition60DataSram2Start;
extern uint64_t pharosPartition60BssSram2Start;
extern uint64_t pharosPartition60Sram2End;
extern uint64_t pharosPartition60DataSram2End;
extern uint64_t pharosPartition60BssSram2End;

extern uint64_t pharosPartition60DataSram3LoadAddr;
extern uint8_t pharosPartition60DataSram3Size;
extern uint8_t pharosPartition60BssSram3Size;
extern uint64_t pharosPartition60Sram3Start;
extern uint64_t pharosPartition60DataSram3Start;
extern uint64_t pharosPartition60BssSram3Start;
extern uint64_t pharosPartition60Sram3End;
extern uint64_t pharosPartition60DataSram3End;
extern uint64_t pharosPartition60BssSram3End;

extern uint64_t pharosPartition60DataSramBackupLoadAddr;
extern uint8_t pharosPartition60DataSramBackupSize;
extern uint8_t pharosPartition60BssSramBackupSize;
extern uint64_t pharosPartition60SramBackupStart;
extern uint64_t pharosPartition60DataSramBackupStart;
extern uint64_t pharosPartition60BssSramBackupStart;
extern uint64_t pharosPartition60SramBackupEnd;
extern uint64_t pharosPartition60DataSramBackupEnd;
extern uint64_t pharosPartition60BssSramBackupEnd;

extern uint64_t pharosPartition60DataSdramLoadAddr;
extern uint8_t pharosPartition60DataSdramSize;
extern uint8_t pharosPartition60BssSdramSize;
extern uint64_t pharosPartition60SdramStart;
extern uint64_t pharosPartition60DataSdramStart;
extern uint64_t pharosPartition60BssSdramStart;
extern uint64_t pharosPartition60SdramEnd;
extern uint64_t pharosPartition60DataSdramEnd;
extern uint64_t pharosPartition60BssSdramEnd;

extern uint64_t pharosPartition61DataCcmLoadAddr;
extern uint8_t pharosPartition61DataCcmSize;
extern uint8_t pharosPartition61BssCcmSize;
extern uint64_t pharosPartition61CcmStart;
extern uint64_t pharosPartition61DataCcmStart;
extern uint64_t pharosPartition61BssCcmStart;
extern uint64_t pharosPartition61CcmEnd;
extern uint64_t pharosPartition61DataCcmEnd;
extern uint64_t pharosPartition61BssCcmEnd;

extern uint64_t pharosPartition61DataSram1LoadAddr;
extern uint8_t pharosPartition61DataSram1Size;
extern uint8_t pharosPartition61BssSram1Size;
extern uint64_t pharosPartition61Sram1Start;
extern uint64_t pharosPartition61DataSram1Start;
extern uint64_t pharosPartition61BssSram1Start;
extern uint64_t pharosPartition61Sram1End;
extern uint64_t pharosPartition61DataSram1End;
extern uint64_t pharosPartition61BssSram1End;

extern uint64_t pharosPartition61DataSram2LoadAddr;
extern uint8_t pharosPartition61DataSram2Size;
extern uint8_t pharosPartition61BssSram2Size;
extern uint64_t pharosPartition61Sram2Start;
extern uint64_t pharosPartition61DataSram2Start;
extern uint64_t pharosPartition61BssSram2Start;
extern uint64_t pharosPartition61Sram2End;
extern uint64_t pharosPartition61DataSram2End;
extern uint64_t pharosPartition61BssSram2End;

extern uint64_t pharosPartition61DataSram3LoadAddr;
extern uint8_t pharosPartition61DataSram3Size;
extern uint8_t pharosPartition61BssSram3Size;
extern uint64_t pharosPartition61Sram3Start;
extern uint64_t pharosPartition61DataSram3Start;
extern uint64_t pharosPartition61BssSram3Start;
extern uint64_t pharosPartition61Sram3End;
extern uint64_t pharosPartition61DataSram3End;
extern uint64_t pharosPartition61BssSram3End;

extern uint64_t pharosPartition61DataSramBackupLoadAddr;
extern uint8_t pharosPartition61DataSramBackupSize;
extern uint8_t pharosPartition61BssSramBackupSize;
extern uint64_t pharosPartition61SramBackupStart;
extern uint64_t pharosPartition61DataSramBackupStart;
extern uint64_t pharosPartition61BssSramBackupStart;
extern uint64_t pharosPartition61SramBackupEnd;
extern uint64_t pharosPartition61DataSramBackupEnd;
extern uint64_t pharosPartition61BssSramBackupEnd;

extern uint64_t pharosPartition61DataSdramLoadAddr;
extern uint8_t pharosPartition61DataSdramSize;
extern uint8_t pharosPartition61BssSdramSize;
extern uint64_t pharosPartition61SdramStart;
extern uint64_t pharosPartition61DataSdramStart;
extern uint64_t pharosPartition61BssSdramStart;
extern uint64_t pharosPartition61SdramEnd;
extern uint64_t pharosPartition61DataSdramEnd;
extern uint64_t pharosPartition61BssSdramEnd;

extern uint64_t pharosPartition62DataCcmLoadAddr;
extern uint8_t pharosPartition62DataCcmSize;
extern uint8_t pharosPartition62BssCcmSize;
extern uint64_t pharosPartition62CcmStart;
extern uint64_t pharosPartition62DataCcmStart;
extern uint64_t pharosPartition62BssCcmStart;
extern uint64_t pharosPartition62CcmEnd;
extern uint64_t pharosPartition62DataCcmEnd;
extern uint64_t pharosPartition62BssCcmEnd;

extern uint64_t pharosPartition62DataSram1LoadAddr;
extern uint8_t pharosPartition62DataSram1Size;
extern uint8_t pharosPartition62BssSram1Size;
extern uint64_t pharosPartition62Sram1Start;
extern uint64_t pharosPartition62DataSram1Start;
extern uint64_t pharosPartition62BssSram1Start;
extern uint64_t pharosPartition62Sram1End;
extern uint64_t pharosPartition62DataSram1End;
extern uint64_t pharosPartition62BssSram1End;

extern uint64_t pharosPartition62DataSram2LoadAddr;
extern uint8_t pharosPartition62DataSram2Size;
extern uint8_t pharosPartition62BssSram2Size;
extern uint64_t pharosPartition62Sram2Start;
extern uint64_t pharosPartition62DataSram2Start;
extern uint64_t pharosPartition62BssSram2Start;
extern uint64_t pharosPartition62Sram2End;
extern uint64_t pharosPartition62DataSram2End;
extern uint64_t pharosPartition62BssSram2End;

extern uint64_t pharosPartition62DataSram3LoadAddr;
extern uint8_t pharosPartition62DataSram3Size;
extern uint8_t pharosPartition62BssSram3Size;
extern uint64_t pharosPartition62Sram3Start;
extern uint64_t pharosPartition62DataSram3Start;
extern uint64_t pharosPartition62BssSram3Start;
extern uint64_t pharosPartition62Sram3End;
extern uint64_t pharosPartition62DataSram3End;
extern uint64_t pharosPartition62BssSram3End;

extern uint64_t pharosPartition62DataSramBackupLoadAddr;
extern uint8_t pharosPartition62DataSramBackupSize;
extern uint8_t pharosPartition62BssSramBackupSize;
extern uint64_t pharosPartition62SramBackupStart;
extern uint64_t pharosPartition62DataSramBackupStart;
extern uint64_t pharosPartition62BssSramBackupStart;
extern uint64_t pharosPartition62SramBackupEnd;
extern uint64_t pharosPartition62DataSramBackupEnd;
extern uint64_t pharosPartition62BssSramBackupEnd;

extern uint64_t pharosPartition62DataSdramLoadAddr;
extern uint8_t pharosPartition62DataSdramSize;
extern uint8_t pharosPartition62BssSdramSize;
extern uint64_t pharosPartition62SdramStart;
extern uint64_t pharosPartition62DataSdramStart;
extern uint64_t pharosPartition62BssSdramStart;
extern uint64_t pharosPartition62SdramEnd;
extern uint64_t pharosPartition62DataSdramEnd;
extern uint64_t pharosPartition62BssSdramEnd;

extern uint64_t pharosPartition63DataCcmLoadAddr;
extern uint8_t pharosPartition63DataCcmSize;
extern uint8_t pharosPartition63BssCcmSize;
extern uint64_t pharosPartition63CcmStart;
extern uint64_t pharosPartition63DataCcmStart;
extern uint64_t pharosPartition63BssCcmStart;
extern uint64_t pharosPartition63CcmEnd;
extern uint64_t pharosPartition63DataCcmEnd;
extern uint64_t pharosPartition63BssCcmEnd;

extern uint64_t pharosPartition63DataSram1LoadAddr;
extern uint8_t pharosPartition63DataSram1Size;
extern uint8_t pharosPartition63BssSram1Size;
extern uint64_t pharosPartition63Sram1Start;
extern uint64_t pharosPartition63DataSram1Start;
extern uint64_t pharosPartition63BssSram1Start;
extern uint64_t pharosPartition63Sram1End;
extern uint64_t pharosPartition63DataSram1End;
extern uint64_t pharosPartition63BssSram1End;

extern uint64_t pharosPartition63DataSram2LoadAddr;
extern uint8_t pharosPartition63DataSram2Size;
extern uint8_t pharosPartition63BssSram2Size;
extern uint64_t pharosPartition63Sram2Start;
extern uint64_t pharosPartition63DataSram2Start;
extern uint64_t pharosPartition63BssSram2Start;
extern uint64_t pharosPartition63Sram2End;
extern uint64_t pharosPartition63DataSram2End;
extern uint64_t pharosPartition63BssSram2End;

extern uint64_t pharosPartition63DataSram3LoadAddr;
extern uint8_t pharosPartition63DataSram3Size;
extern uint8_t pharosPartition63BssSram3Size;
extern uint64_t pharosPartition63Sram3Start;
extern uint64_t pharosPartition63DataSram3Start;
extern uint64_t pharosPartition63BssSram3Start;
extern uint64_t pharosPartition63Sram3End;
extern uint64_t pharosPartition63DataSram3End;
extern uint64_t pharosPartition63BssSram3End;

extern uint64_t pharosPartition63DataSramBackupLoadAddr;
extern uint8_t pharosPartition63DataSramBackupSize;
extern uint8_t pharosPartition63BssSramBackupSize;
extern uint64_t pharosPartition63SramBackupStart;
extern uint64_t pharosPartition63DataSramBackupStart;
extern uint64_t pharosPartition63BssSramBackupStart;
extern uint64_t pharosPartition63SramBackupEnd;
extern uint64_t pharosPartition63DataSramBackupEnd;
extern uint64_t pharosPartition63BssSramBackupEnd;

extern uint64_t pharosPartition63DataSdramLoadAddr;
extern uint8_t pharosPartition63DataSdramSize;
extern uint8_t pharosPartition63BssSdramSize;
extern uint64_t pharosPartition63SdramStart;
extern uint64_t pharosPartition63DataSdramStart;
extern uint64_t pharosPartition63BssSdramStart;
extern uint64_t pharosPartition63SdramEnd;
extern uint64_t pharosPartition63DataSdramEnd;
extern uint64_t pharosPartition63BssSdramEnd;


/* PHAROS_CPU_MEMORY_H */
#endif	
