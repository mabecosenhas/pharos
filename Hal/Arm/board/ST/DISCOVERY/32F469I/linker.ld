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


OUTPUT_ARCH(arm)

/*
 * The memory map looks like this:
 * +-----------------------+ <- low memory
 * | .text                 |
 * |   pharosStartText     |
 * |   pharosEndText       |
 * +-----------------------+
 * | .kernel               | initialized data goes here
 * |   pharosKernelStart   |
 * |   pharosKernelEnd     |
 * +-----------------------+
 * | .partition0           | initialized data goes here
 * |        partition data |
 * |                       |
 * +-----------------------+
 * | .partition1           | initialized data goes here
 * |        partition data |
 * |                       |
 * +-----------------------+
 *            .
 *            .
 *            .
 * +-----------------------+
 * | .partition63          | initialized data goes here
 * |        partition data |
 * +-----------------------+
 * | .ipCall               | 
 * |    pharosIpCallStart  |
 * |    pharosIpCallEnd    |
 * +-----------------------+ <- high memory
 */


/* Specify the memory areas */
MEMORY
{
  /* Flash memory */
  flash   (rx)      : ORIGIN = 0x08000000 , LENGTH = 2M

  /* high speed SRAM */
  ccm     (rwx)     : ORIGIN = 0x10000000 , LENGTH = 64K

  /* standard SRAM 1 */
  sram1     (rwx)   : ORIGIN = 0x20000000 , LENGTH = 160K

  /* standard SRAM 2 */
  sram2     (rwx)   : ORIGIN = 0x20028000 , LENGTH = 32K

  /* standard SRAM 3 */
  sram3     (rwx)   : ORIGIN = 0x20030000 , LENGTH = 128K

  /* SRAM for backup */
  sramBackup (rwx)  : ORIGIN = 0x40024000 , LENGTH = 4K

  /* slow SDRAM */
  sdram   (rwx)     : ORIGIN = 0xC0000000 , LENGTH = 16M
}

ROM_START = 0x08000000;
ROM_END   = 0x08200000;

CHANNEL_BULK_SIZE = 128;

/**
 * On this CPU the minimum MPU alignment is 32 bytes, but this requires using sub-regions. 
 * It is much simpler not to use sub-regions and allow this wasted space 
 */
REGION_MINIMUM_ALIGNMENT = 256;
REGION_MINIMUM_ALIGNMENT_LOG2 = 8;

/* calculate the log2 of the text section size, used to initialize the MPU size register */

/* calculate the size of the text section (note that it includes the ROM area) */
pharosCpuTextSizeLog2   = MAX(REGION_MINIMUM_ALIGNMENT_LOG2 , LOG2CEIL(ROM_END - ROM_START));
pharosCpuIpCallSizeLog2 = MAX(REGION_MINIMUM_ALIGNMENT_LOG2 , LOG2CEIL(pharosIpCallEnd - pharosIpCallStart));

/* alignment for kernel data */
KERNEL_ALIGNMENT = REGION_MINIMUM_ALIGNMENT;

/* alignment for shared stack data */
SHARED_STACK_ALIGNMENT = REGION_MINIMUM_ALIGNMENT;

/* alignment for shared data */
SHARED_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosSharedCcmEnd - pharosSharedDataCcmStart));
SHARED_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosSharedSram1End - pharosSharedDataSram1Start));
SHARED_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosSharedSram2End - pharosSharedDataSram2Start));
SHARED_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosSharedSram3End - pharosSharedDataSram3Start ));
SHARED_SRAM_BACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosSharedSramBackupEnd - pharosSharedDataSramBackupStart));
SHARED_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosSharedSdramEnd - pharosSharedDataSdramStart));



/* alignment for inter-partition calls data */
IP_CALL_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosIpCallEnd - pharosIpCallStart));




/* definition of the partition 0 alignment for each memory region */
PARTITION_0_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition0CcmEnd - pharosPartition0DataCcmStart));
PARTITION_0_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition0Sram1End - pharosPartition0DataSram1Start));
PARTITION_0_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition0Sram2End - pharosPartition0DataSram2Start));
PARTITION_0_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition0Sram3End - pharosPartition0DataSram3Start));
PARTITION_0_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition0SramBackupEnd - pharosPartition0DataSramBackupStart));
PARTITION_0_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition0SdramEnd - pharosPartition0DataSdramStart));

/* definition of the partition 1 alignment for each memory region */
PARTITION_1_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition1CcmEnd - pharosPartition1DataCcmStart));
PARTITION_1_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition1Sram1End - pharosPartition1DataSram1Start));
PARTITION_1_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition1Sram2End - pharosPartition1DataSram2Start));
PARTITION_1_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition1Sram3End - pharosPartition1DataSram3Start));
PARTITION_1_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition1SramBackupEnd - pharosPartition1DataSramBackupStart));
PARTITION_1_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition1SdramEnd - pharosPartition1DataSdramStart));

/* definition of the partition 2 alignment for each memory region */
PARTITION_2_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition2CcmEnd - pharosPartition2DataCcmStart));
PARTITION_2_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition2Sram1End - pharosPartition2DataSram1Start));
PARTITION_2_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition2Sram2End - pharosPartition2DataSram2Start));
PARTITION_2_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition2Sram3End - pharosPartition2DataSram3Start));
PARTITION_2_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition2SramBackupEnd - pharosPartition2DataSramBackupStart));
PARTITION_2_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition2SdramEnd - pharosPartition2DataSdramStart));

/* definition of the partition 3 alignment for each memory region */
PARTITION_3_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition3CcmEnd - pharosPartition3DataCcmStart));
PARTITION_3_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition3Sram1End - pharosPartition3DataSram1Start));
PARTITION_3_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition3Sram2End - pharosPartition3DataSram2Start));
PARTITION_3_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition3Sram3End - pharosPartition3DataSram3Start));
PARTITION_3_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition3SramBackupEnd - pharosPartition3DataSramBackupStart));
PARTITION_3_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition3SdramEnd - pharosPartition3DataSdramStart));

/* definition of the partition 4 alignment for each memory region */
PARTITION_4_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition4CcmEnd - pharosPartition4DataCcmStart));
PARTITION_4_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition4Sram1End - pharosPartition4DataSram1Start));
PARTITION_4_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition4Sram2End - pharosPartition4DataSram2Start));
PARTITION_4_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition4Sram3End - pharosPartition4DataSram3Start));
PARTITION_4_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition4SramBackupEnd - pharosPartition4DataSramBackupStart));
PARTITION_4_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition4SdramEnd - pharosPartition4DataSdramStart));

/* definition of the partition 5 alignment for each memory region */
PARTITION_5_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition5CcmEnd - pharosPartition5DataCcmStart));
PARTITION_5_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition5Sram1End - pharosPartition5DataSram1Start));
PARTITION_5_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition5Sram2End - pharosPartition5DataSram2Start));
PARTITION_5_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition5Sram3End - pharosPartition5DataSram3Start));
PARTITION_5_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition5SramBackupEnd - pharosPartition5DataSramBackupStart));
PARTITION_5_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition5SdramEnd - pharosPartition5DataSdramStart));

/* definition of the partition 6 alignment for each memory region */
PARTITION_6_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition6CcmEnd - pharosPartition6DataCcmStart));
PARTITION_6_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition6Sram1End - pharosPartition6DataSram1Start));
PARTITION_6_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition6Sram2End - pharosPartition6DataSram2Start));
PARTITION_6_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition6Sram3End - pharosPartition6DataSram3Start));
PARTITION_6_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition6SramBackupEnd - pharosPartition6DataSramBackupStart));
PARTITION_6_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition6SdramEnd - pharosPartition6DataSdramStart));

/* definition of the partition 7 alignment for each memory region */
PARTITION_7_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition7CcmEnd - pharosPartition7DataCcmStart));
PARTITION_7_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition7Sram1End - pharosPartition7DataSram1Start));
PARTITION_7_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition7Sram2End - pharosPartition7DataSram2Start));
PARTITION_7_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition7Sram3End - pharosPartition7DataSram3Start));
PARTITION_7_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition7SramBackupEnd - pharosPartition7DataSramBackupStart));
PARTITION_7_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition7SdramEnd - pharosPartition7DataSdramStart));

/* definition of the partition 8 alignment for each memory region */
PARTITION_8_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition8CcmEnd - pharosPartition8DataCcmStart));
PARTITION_8_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition8Sram1End - pharosPartition8DataSram1Start));
PARTITION_8_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition8Sram2End - pharosPartition8DataSram2Start));
PARTITION_8_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition8Sram3End - pharosPartition8DataSram3Start));
PARTITION_8_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition8SramBackupEnd - pharosPartition8DataSramBackupStart));
PARTITION_8_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition8SdramEnd - pharosPartition8DataSdramStart));

/* definition of the partition 9 alignment for each memory region */
PARTITION_9_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition9CcmEnd - pharosPartition9DataCcmStart));
PARTITION_9_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition9Sram1End - pharosPartition9DataSram1Start));
PARTITION_9_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition9Sram2End - pharosPartition9DataSram2Start));
PARTITION_9_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition9Sram3End - pharosPartition9DataSram3Start));
PARTITION_9_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition9SramBackupEnd - pharosPartition9DataSramBackupStart));
PARTITION_9_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition9SdramEnd - pharosPartition9DataSdramStart));

/* definition of the partition 10 alignment for each memory region */
PARTITION_10_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition10CcmEnd - pharosPartition10DataCcmStart));
PARTITION_10_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition10Sram1End - pharosPartition10DataSram1Start));
PARTITION_10_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition10Sram2End - pharosPartition10DataSram2Start));
PARTITION_10_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition10Sram3End - pharosPartition10DataSram3Start));
PARTITION_10_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition10SramBackupEnd - pharosPartition10DataSramBackupStart));
PARTITION_10_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition10SdramEnd - pharosPartition10DataSdramStart));

/* definition of the partition 11 alignment for each memory region */
PARTITION_11_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition11CcmEnd - pharosPartition11DataCcmStart));
PARTITION_11_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition11Sram1End - pharosPartition11DataSram1Start));
PARTITION_11_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition11Sram2End - pharosPartition11DataSram2Start));
PARTITION_11_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition11Sram3End - pharosPartition11DataSram3Start));
PARTITION_11_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition11SramBackupEnd - pharosPartition11DataSramBackupStart));
PARTITION_11_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition11SdramEnd - pharosPartition11DataSdramStart));

/* definition of the partition 12 alignment for each memory region */
PARTITION_12_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition12CcmEnd - pharosPartition12DataCcmStart));
PARTITION_12_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition12Sram1End - pharosPartition12DataSram1Start));
PARTITION_12_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition12Sram2End - pharosPartition12DataSram2Start));
PARTITION_12_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition12Sram3End - pharosPartition12DataSram3Start));
PARTITION_12_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition12SramBackupEnd - pharosPartition12DataSramBackupStart));
PARTITION_12_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition12SdramEnd - pharosPartition12DataSdramStart));

/* definition of the partition 13 alignment for each memory region */
PARTITION_13_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition13CcmEnd - pharosPartition13DataCcmStart));
PARTITION_13_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition13Sram1End - pharosPartition13DataSram1Start));
PARTITION_13_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition13Sram2End - pharosPartition13DataSram2Start));
PARTITION_13_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition13Sram3End - pharosPartition13DataSram3Start));
PARTITION_13_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition13SramBackupEnd - pharosPartition13DataSramBackupStart));
PARTITION_13_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition13SdramEnd - pharosPartition13DataSdramStart));

/* definition of the partition 14 alignment for each memory region */
PARTITION_14_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition14CcmEnd - pharosPartition14DataCcmStart));
PARTITION_14_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition14Sram1End - pharosPartition14DataSram1Start));
PARTITION_14_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition14Sram2End - pharosPartition14DataSram2Start));
PARTITION_14_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition14Sram3End - pharosPartition14DataSram3Start));
PARTITION_14_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition14SramBackupEnd - pharosPartition14DataSramBackupStart));
PARTITION_14_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition14SdramEnd - pharosPartition14DataSdramStart));

/* definition of the partition 15 alignment for each memory region */
PARTITION_15_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition15CcmEnd - pharosPartition15DataCcmStart));
PARTITION_15_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition15Sram1End - pharosPartition15DataSram1Start));
PARTITION_15_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition15Sram2End - pharosPartition15DataSram2Start));
PARTITION_15_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition15Sram3End - pharosPartition15DataSram3Start));
PARTITION_15_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition15SramBackupEnd - pharosPartition15DataSramBackupStart));
PARTITION_15_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition15SdramEnd - pharosPartition15DataSdramStart));

/* definition of the partition 16 alignment for each memory region */
PARTITION_16_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition16CcmEnd - pharosPartition16DataCcmStart));
PARTITION_16_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition16Sram1End - pharosPartition16DataSram1Start));
PARTITION_16_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition16Sram2End - pharosPartition16DataSram2Start));
PARTITION_16_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition16Sram3End - pharosPartition16DataSram3Start));
PARTITION_16_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition16SramBackupEnd - pharosPartition16DataSramBackupStart));
PARTITION_16_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition16SdramEnd - pharosPartition16DataSdramStart));

/* definition of the partition 17 alignment for each memory region */
PARTITION_17_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition17CcmEnd - pharosPartition17DataCcmStart));
PARTITION_17_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition17Sram1End - pharosPartition17DataSram1Start));
PARTITION_17_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition17Sram2End - pharosPartition17DataSram2Start));
PARTITION_17_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition17Sram3End - pharosPartition17DataSram3Start));
PARTITION_17_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition17SramBackupEnd - pharosPartition17DataSramBackupStart));
PARTITION_17_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition17SdramEnd - pharosPartition17DataSdramStart));

/* definition of the partition 18 alignment for each memory region */
PARTITION_18_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition18CcmEnd - pharosPartition18DataCcmStart));
PARTITION_18_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition18Sram1End - pharosPartition18DataSram1Start));
PARTITION_18_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition18Sram2End - pharosPartition18DataSram2Start));
PARTITION_18_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition18Sram3End - pharosPartition18DataSram3Start));
PARTITION_18_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition18SramBackupEnd - pharosPartition18DataSramBackupStart));
PARTITION_18_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition18SdramEnd - pharosPartition18DataSdramStart));

/* definition of the partition 19 alignment for each memory region */
PARTITION_19_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition19CcmEnd - pharosPartition19DataCcmStart));
PARTITION_19_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition19Sram1End - pharosPartition19DataSram1Start));
PARTITION_19_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition19Sram2End - pharosPartition19DataSram2Start));
PARTITION_19_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition19Sram3End - pharosPartition19DataSram3Start));
PARTITION_19_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition19SramBackupEnd - pharosPartition19DataSramBackupStart));
PARTITION_19_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition19SdramEnd - pharosPartition19DataSdramStart));

/* definition of the partition 20 alignment for each memory region */
PARTITION_20_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition20CcmEnd - pharosPartition20DataCcmStart));
PARTITION_20_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition20Sram1End - pharosPartition20DataSram1Start));
PARTITION_20_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition20Sram2End - pharosPartition20DataSram2Start));
PARTITION_20_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition20Sram3End - pharosPartition20DataSram3Start));
PARTITION_20_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition20SramBackupEnd - pharosPartition20DataSramBackupStart));
PARTITION_20_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition20SdramEnd - pharosPartition20DataSdramStart));

/* definition of the partition 21 alignment for each memory region */
PARTITION_21_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition21CcmEnd - pharosPartition21DataCcmStart));
PARTITION_21_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition21Sram1End - pharosPartition21DataSram1Start));
PARTITION_21_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition21Sram2End - pharosPartition21DataSram2Start));
PARTITION_21_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition21Sram3End - pharosPartition21DataSram3Start));
PARTITION_21_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition21SramBackupEnd - pharosPartition21DataSramBackupStart));
PARTITION_21_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition21SdramEnd - pharosPartition21DataSdramStart));

/* definition of the partition 22 alignment for each memory region */
PARTITION_22_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition22CcmEnd - pharosPartition22DataCcmStart));
PARTITION_22_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition22Sram1End - pharosPartition22DataSram1Start));
PARTITION_22_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition22Sram2End - pharosPartition22DataSram2Start));
PARTITION_22_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition22Sram3End - pharosPartition22DataSram3Start));
PARTITION_22_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition22SramBackupEnd - pharosPartition22DataSramBackupStart));
PARTITION_22_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition22SdramEnd - pharosPartition22DataSdramStart));

/* definition of the partition 23 alignment for each memory region */
PARTITION_23_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition23CcmEnd - pharosPartition23DataCcmStart));
PARTITION_23_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition23Sram1End - pharosPartition23DataSram1Start));
PARTITION_23_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition23Sram2End - pharosPartition23DataSram2Start));
PARTITION_23_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition23Sram3End - pharosPartition23DataSram3Start));
PARTITION_23_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition23SramBackupEnd - pharosPartition23DataSramBackupStart));
PARTITION_23_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition23SdramEnd - pharosPartition23DataSdramStart));

/* definition of the partition 24 alignment for each memory region */
PARTITION_24_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition24CcmEnd - pharosPartition24DataCcmStart));
PARTITION_24_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition24Sram1End - pharosPartition24DataSram1Start));
PARTITION_24_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition24Sram2End - pharosPartition24DataSram2Start));
PARTITION_24_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition24Sram3End - pharosPartition24DataSram3Start));
PARTITION_24_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition24SramBackupEnd - pharosPartition24DataSramBackupStart));
PARTITION_24_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition24SdramEnd - pharosPartition24DataSdramStart));

/* definition of the partition 25 alignment for each memory region */
PARTITION_25_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition25CcmEnd - pharosPartition25DataCcmStart));
PARTITION_25_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition25Sram1End - pharosPartition25DataSram1Start));
PARTITION_25_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition25Sram2End - pharosPartition25DataSram2Start));
PARTITION_25_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition25Sram3End - pharosPartition25DataSram3Start));
PARTITION_25_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition25SramBackupEnd - pharosPartition25DataSramBackupStart));
PARTITION_25_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition25SdramEnd - pharosPartition25DataSdramStart));

/* definition of the partition 26 alignment for each memory region */
PARTITION_26_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition26CcmEnd - pharosPartition26DataCcmStart));
PARTITION_26_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition26Sram1End - pharosPartition26DataSram1Start));
PARTITION_26_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition26Sram2End - pharosPartition26DataSram2Start));
PARTITION_26_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition26Sram3End - pharosPartition26DataSram3Start));
PARTITION_26_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition26SramBackupEnd - pharosPartition26DataSramBackupStart));
PARTITION_26_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition26SdramEnd - pharosPartition26DataSdramStart));

/* definition of the partition 27 alignment for each memory region */
PARTITION_27_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition27CcmEnd - pharosPartition27DataCcmStart));
PARTITION_27_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition27Sram1End - pharosPartition27DataSram1Start));
PARTITION_27_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition27Sram2End - pharosPartition27DataSram2Start));
PARTITION_27_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition27Sram3End - pharosPartition27DataSram3Start));
PARTITION_27_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition27SramBackupEnd - pharosPartition27DataSramBackupStart));
PARTITION_27_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition27SdramEnd - pharosPartition27DataSdramStart));

/* definition of the partition 28 alignment for each memory region */
PARTITION_28_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition28CcmEnd - pharosPartition28DataCcmStart));
PARTITION_28_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition28Sram1End - pharosPartition28DataSram1Start));
PARTITION_28_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition28Sram2End - pharosPartition28DataSram2Start));
PARTITION_28_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition28Sram3End - pharosPartition28DataSram3Start));
PARTITION_28_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition28SramBackupEnd - pharosPartition28DataSramBackupStart));
PARTITION_28_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition28SdramEnd - pharosPartition28DataSdramStart));

/* definition of the partition 29 alignment for each memory region */
PARTITION_29_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition29CcmEnd - pharosPartition29DataCcmStart));
PARTITION_29_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition29Sram1End - pharosPartition29DataSram1Start));
PARTITION_29_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition29Sram2End - pharosPartition29DataSram2Start));
PARTITION_29_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition29Sram3End - pharosPartition29DataSram3Start));
PARTITION_29_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition29SramBackupEnd - pharosPartition29DataSramBackupStart));
PARTITION_29_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition29SdramEnd - pharosPartition29DataSdramStart));

/* definition of the partition 30 alignment for each memory region */
PARTITION_30_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition30CcmEnd - pharosPartition30DataCcmStart));
PARTITION_30_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition30Sram1End - pharosPartition30DataSram1Start));
PARTITION_30_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition30Sram2End - pharosPartition30DataSram2Start));
PARTITION_30_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition30Sram3End - pharosPartition30DataSram3Start));
PARTITION_30_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition30SramBackupEnd - pharosPartition30DataSramBackupStart));
PARTITION_30_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition30SdramEnd - pharosPartition30DataSdramStart));

/* definition of the partition 31 alignment for each memory region */
PARTITION_31_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition31CcmEnd - pharosPartition31DataCcmStart));
PARTITION_31_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition31Sram1End - pharosPartition31DataSram1Start));
PARTITION_31_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition31Sram2End - pharosPartition31DataSram2Start));
PARTITION_31_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition31Sram3End - pharosPartition31DataSram3Start));
PARTITION_31_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition31SramBackupEnd - pharosPartition31DataSramBackupStart));
PARTITION_31_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition31SdramEnd - pharosPartition31DataSdramStart));

/* definition of the partition 32 alignment for each memory region */
PARTITION_32_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition32CcmEnd - pharosPartition32DataCcmStart));
PARTITION_32_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition32Sram1End - pharosPartition32DataSram1Start));
PARTITION_32_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition32Sram2End - pharosPartition32DataSram2Start));
PARTITION_32_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition32Sram3End - pharosPartition32DataSram3Start));
PARTITION_32_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition32SramBackupEnd - pharosPartition32DataSramBackupStart));
PARTITION_32_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition32SdramEnd - pharosPartition32DataSdramStart));

/* definition of the partition 33 alignment for each memory region */
PARTITION_33_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition33CcmEnd - pharosPartition33DataCcmStart));
PARTITION_33_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition33Sram1End - pharosPartition33DataSram1Start));
PARTITION_33_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition33Sram2End - pharosPartition33DataSram2Start));
PARTITION_33_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition33Sram3End - pharosPartition33DataSram3Start));
PARTITION_33_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition33SramBackupEnd - pharosPartition33DataSramBackupStart));
PARTITION_33_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition33SdramEnd - pharosPartition33DataSdramStart));

/* definition of the partition 34 alignment for each memory region */
PARTITION_34_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition34CcmEnd - pharosPartition34DataCcmStart));
PARTITION_34_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition34Sram1End - pharosPartition34DataSram1Start));
PARTITION_34_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition34Sram2End - pharosPartition34DataSram2Start));
PARTITION_34_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition34Sram3End - pharosPartition34DataSram3Start));
PARTITION_34_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition34SramBackupEnd - pharosPartition34DataSramBackupStart));
PARTITION_34_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition34SdramEnd - pharosPartition34DataSdramStart));

/* definition of the partition 35 alignment for each memory region */
PARTITION_35_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition35CcmEnd - pharosPartition35DataCcmStart));
PARTITION_35_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition35Sram1End - pharosPartition35DataSram1Start));
PARTITION_35_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition35Sram2End - pharosPartition35DataSram2Start));
PARTITION_35_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition35Sram3End - pharosPartition35DataSram3Start));
PARTITION_35_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition35SramBackupEnd - pharosPartition35DataSramBackupStart));
PARTITION_35_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition35SdramEnd - pharosPartition35DataSdramStart));

/* definition of the partition 36 alignment for each memory region */
PARTITION_36_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition36CcmEnd - pharosPartition36DataCcmStart));
PARTITION_36_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition36Sram1End - pharosPartition36DataSram1Start));
PARTITION_36_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition36Sram2End - pharosPartition36DataSram2Start));
PARTITION_36_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition36Sram3End - pharosPartition36DataSram3Start));
PARTITION_36_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition36SramBackupEnd - pharosPartition36DataSramBackupStart));
PARTITION_36_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition36SdramEnd - pharosPartition36DataSdramStart));

/* definition of the partition 37 alignment for each memory region */
PARTITION_37_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition37CcmEnd - pharosPartition37DataCcmStart));
PARTITION_37_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition37Sram1End - pharosPartition37DataSram1Start));
PARTITION_37_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition37Sram2End - pharosPartition37DataSram2Start));
PARTITION_37_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition37Sram3End - pharosPartition37DataSram3Start));
PARTITION_37_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition37SramBackupEnd - pharosPartition37DataSramBackupStart));
PARTITION_37_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition37SdramEnd - pharosPartition37DataSdramStart));

/* definition of the partition 38 alignment for each memory region */
PARTITION_38_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition38CcmEnd - pharosPartition38DataCcmStart));
PARTITION_38_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition38Sram1End - pharosPartition38DataSram1Start));
PARTITION_38_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition38Sram2End - pharosPartition38DataSram2Start));
PARTITION_38_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition38Sram3End - pharosPartition38DataSram3Start));
PARTITION_38_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition38SramBackupEnd - pharosPartition38DataSramBackupStart));
PARTITION_38_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition38SdramEnd - pharosPartition38DataSdramStart));

/* definition of the partition 39 alignment for each memory region */
PARTITION_39_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition39CcmEnd - pharosPartition39DataCcmStart));
PARTITION_39_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition39Sram1End - pharosPartition39DataSram1Start));
PARTITION_39_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition39Sram2End - pharosPartition39DataSram2Start));
PARTITION_39_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition39Sram3End - pharosPartition39DataSram3Start));
PARTITION_39_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition39SramBackupEnd - pharosPartition39DataSramBackupStart));
PARTITION_39_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition39SdramEnd - pharosPartition39DataSdramStart));

/* definition of the partition 40 alignment for each memory region */
PARTITION_40_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition40CcmEnd - pharosPartition40DataCcmStart));
PARTITION_40_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition40Sram1End - pharosPartition40DataSram1Start));
PARTITION_40_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition40Sram2End - pharosPartition40DataSram2Start));
PARTITION_40_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition40Sram3End - pharosPartition40DataSram3Start));
PARTITION_40_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition40SramBackupEnd - pharosPartition40DataSramBackupStart));
PARTITION_40_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition40SdramEnd - pharosPartition40DataSdramStart));

/* definition of the partition 41 alignment for each memory region */
PARTITION_41_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition41CcmEnd - pharosPartition41DataCcmStart));
PARTITION_41_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition41Sram1End - pharosPartition41DataSram1Start));
PARTITION_41_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition41Sram2End - pharosPartition41DataSram2Start));
PARTITION_41_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition41Sram3End - pharosPartition41DataSram3Start));
PARTITION_41_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition41SramBackupEnd - pharosPartition41DataSramBackupStart));
PARTITION_41_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition41SdramEnd - pharosPartition41DataSdramStart));

/* definition of the partition 42 alignment for each memory region */
PARTITION_42_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition42CcmEnd - pharosPartition42DataCcmStart));
PARTITION_42_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition42Sram1End - pharosPartition42DataSram1Start));
PARTITION_42_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition42Sram2End - pharosPartition42DataSram2Start));
PARTITION_42_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition42Sram3End - pharosPartition42DataSram3Start));
PARTITION_42_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition42SramBackupEnd - pharosPartition42DataSramBackupStart));
PARTITION_42_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition42SdramEnd - pharosPartition42DataSdramStart));

/* definition of the partition 43 alignment for each memory region */
PARTITION_43_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition43CcmEnd - pharosPartition43DataCcmStart));
PARTITION_43_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition43Sram1End - pharosPartition43DataSram1Start));
PARTITION_43_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition43Sram2End - pharosPartition43DataSram2Start));
PARTITION_43_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition43Sram3End - pharosPartition43DataSram3Start));
PARTITION_43_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition43SramBackupEnd - pharosPartition43DataSramBackupStart));
PARTITION_43_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition43SdramEnd - pharosPartition43DataSdramStart));

/* definition of the partition 44 alignment for each memory region */
PARTITION_44_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition44CcmEnd - pharosPartition44DataCcmStart));
PARTITION_44_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition44Sram1End - pharosPartition44DataSram1Start));
PARTITION_44_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition44Sram2End - pharosPartition44DataSram2Start));
PARTITION_44_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition44Sram3End - pharosPartition44DataSram3Start));
PARTITION_44_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition44SramBackupEnd - pharosPartition44DataSramBackupStart));
PARTITION_44_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition44SdramEnd - pharosPartition44DataSdramStart));

/* definition of the partition 45 alignment for each memory region */
PARTITION_45_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition45CcmEnd - pharosPartition45DataCcmStart));
PARTITION_45_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition45Sram1End - pharosPartition45DataSram1Start));
PARTITION_45_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition45Sram2End - pharosPartition45DataSram2Start));
PARTITION_45_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition45Sram3End - pharosPartition45DataSram3Start));
PARTITION_45_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition45SramBackupEnd - pharosPartition45DataSramBackupStart));
PARTITION_45_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition45SdramEnd - pharosPartition45DataSdramStart));

/* definition of the partition 46 alignment for each memory region */
PARTITION_46_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition46CcmEnd - pharosPartition46DataCcmStart));
PARTITION_46_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition46Sram1End - pharosPartition46DataSram1Start));
PARTITION_46_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition46Sram2End - pharosPartition46DataSram2Start));
PARTITION_46_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition46Sram3End - pharosPartition46DataSram3Start));
PARTITION_46_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition46SramBackupEnd - pharosPartition46DataSramBackupStart));
PARTITION_46_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition46SdramEnd - pharosPartition46DataSdramStart));

/* definition of the partition 47 alignment for each memory region */
PARTITION_47_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition47CcmEnd - pharosPartition47DataCcmStart));
PARTITION_47_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition47Sram1End - pharosPartition47DataSram1Start));
PARTITION_47_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition47Sram2End - pharosPartition47DataSram2Start));
PARTITION_47_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition47Sram3End - pharosPartition47DataSram3Start));
PARTITION_47_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition47SramBackupEnd - pharosPartition47DataSramBackupStart));
PARTITION_47_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition47SdramEnd - pharosPartition47DataSdramStart));

/* definition of the partition 48 alignment for each memory region */
PARTITION_48_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition48CcmEnd - pharosPartition48DataCcmStart));
PARTITION_48_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition48Sram1End - pharosPartition48DataSram1Start));
PARTITION_48_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition48Sram2End - pharosPartition48DataSram2Start));
PARTITION_48_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition48Sram3End - pharosPartition48DataSram3Start));
PARTITION_48_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition48SramBackupEnd - pharosPartition48DataSramBackupStart));
PARTITION_48_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition48SdramEnd - pharosPartition48DataSdramStart));

/* definition of the partition 49 alignment for each memory region */
PARTITION_49_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition49CcmEnd - pharosPartition49DataCcmStart));
PARTITION_49_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition49Sram1End - pharosPartition49DataSram1Start));
PARTITION_49_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition49Sram2End - pharosPartition49DataSram2Start));
PARTITION_49_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition49Sram3End - pharosPartition49DataSram3Start));
PARTITION_49_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition49SramBackupEnd - pharosPartition49DataSramBackupStart));
PARTITION_49_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition49SdramEnd - pharosPartition49DataSdramStart));

/* definition of the partition 50 alignment for each memory region */
PARTITION_50_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition50CcmEnd - pharosPartition50DataCcmStart));
PARTITION_50_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition50Sram1End - pharosPartition50DataSram1Start));
PARTITION_50_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition50Sram2End - pharosPartition50DataSram2Start));
PARTITION_50_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition50Sram3End - pharosPartition50DataSram3Start));
PARTITION_50_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition50SramBackupEnd - pharosPartition50DataSramBackupStart));
PARTITION_50_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition50SdramEnd - pharosPartition50DataSdramStart));

/* definition of the partition 51 alignment for each memory region */
PARTITION_51_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition51CcmEnd - pharosPartition51DataCcmStart));
PARTITION_51_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition51Sram1End - pharosPartition51DataSram1Start));
PARTITION_51_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition51Sram2End - pharosPartition51DataSram2Start));
PARTITION_51_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition51Sram3End - pharosPartition51DataSram3Start));
PARTITION_51_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition51SramBackupEnd - pharosPartition51DataSramBackupStart));
PARTITION_51_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition51SdramEnd - pharosPartition51DataSdramStart));

/* definition of the partition 52 alignment for each memory region */
PARTITION_52_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition52CcmEnd - pharosPartition52DataCcmStart));
PARTITION_52_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition52Sram1End - pharosPartition52DataSram1Start));
PARTITION_52_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition52Sram2End - pharosPartition52DataSram2Start));
PARTITION_52_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition52Sram3End - pharosPartition52DataSram3Start));
PARTITION_52_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition52SramBackupEnd - pharosPartition52DataSramBackupStart));
PARTITION_52_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition52SdramEnd - pharosPartition52DataSdramStart));

/* definition of the partition 53 alignment for each memory region */
PARTITION_53_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition53CcmEnd - pharosPartition53DataCcmStart));
PARTITION_53_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition53Sram1End - pharosPartition53DataSram1Start));
PARTITION_53_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition53Sram2End - pharosPartition53DataSram2Start));
PARTITION_53_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition53Sram3End - pharosPartition53DataSram3Start));
PARTITION_53_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition53SramBackupEnd - pharosPartition53DataSramBackupStart));
PARTITION_53_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition53SdramEnd - pharosPartition53DataSdramStart));

/* definition of the partition 54 alignment for each memory region */
PARTITION_54_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition54CcmEnd - pharosPartition54DataCcmStart));
PARTITION_54_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition54Sram1End - pharosPartition54DataSram1Start));
PARTITION_54_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition54Sram2End - pharosPartition54DataSram2Start));
PARTITION_54_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition54Sram3End - pharosPartition54DataSram3Start));
PARTITION_54_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition54SramBackupEnd - pharosPartition54DataSramBackupStart));
PARTITION_54_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition54SdramEnd - pharosPartition54DataSdramStart));

/* definition of the partition 55 alignment for each memory region */
PARTITION_55_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition55CcmEnd - pharosPartition55DataCcmStart));
PARTITION_55_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition55Sram1End - pharosPartition55DataSram1Start));
PARTITION_55_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition55Sram2End - pharosPartition55DataSram2Start));
PARTITION_55_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition55Sram3End - pharosPartition55DataSram3Start));
PARTITION_55_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition55SramBackupEnd - pharosPartition55DataSramBackupStart));
PARTITION_55_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition55SdramEnd - pharosPartition55DataSdramStart));

/* definition of the partition 56 alignment for each memory region */
PARTITION_56_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition56CcmEnd - pharosPartition56DataCcmStart));
PARTITION_56_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition56Sram1End - pharosPartition56DataSram1Start));
PARTITION_56_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition56Sram2End - pharosPartition56DataSram2Start));
PARTITION_56_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition56Sram3End - pharosPartition56DataSram3Start));
PARTITION_56_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition56SramBackupEnd - pharosPartition56DataSramBackupStart));
PARTITION_56_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition56SdramEnd - pharosPartition56DataSdramStart));

/* definition of the partition 57 alignment for each memory region */
PARTITION_57_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition57CcmEnd - pharosPartition57DataCcmStart));
PARTITION_57_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition57Sram1End - pharosPartition57DataSram1Start));
PARTITION_57_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition57Sram2End - pharosPartition57DataSram2Start));
PARTITION_57_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition57Sram3End - pharosPartition57DataSram3Start));
PARTITION_57_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition57SramBackupEnd - pharosPartition57DataSramBackupStart));
PARTITION_57_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition57SdramEnd - pharosPartition57DataSdramStart));

/* definition of the partition 58 alignment for each memory region */
PARTITION_58_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition58CcmEnd - pharosPartition58DataCcmStart));
PARTITION_58_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition58Sram1End - pharosPartition58DataSram1Start));
PARTITION_58_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition58Sram2End - pharosPartition58DataSram2Start));
PARTITION_58_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition58Sram3End - pharosPartition58DataSram3Start));
PARTITION_58_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition58SramBackupEnd - pharosPartition58DataSramBackupStart));
PARTITION_58_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition58SdramEnd - pharosPartition58DataSdramStart));

/* definition of the partition 59 alignment for each memory region */
PARTITION_59_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition59CcmEnd - pharosPartition59DataCcmStart));
PARTITION_59_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition59Sram1End - pharosPartition59DataSram1Start));
PARTITION_59_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition59Sram2End - pharosPartition59DataSram2Start));
PARTITION_59_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition59Sram3End - pharosPartition59DataSram3Start));
PARTITION_59_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition59SramBackupEnd - pharosPartition59DataSramBackupStart));
PARTITION_59_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition59SdramEnd - pharosPartition59DataSdramStart));

/* definition of the partition 60 alignment for each memory region */
PARTITION_60_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition60CcmEnd - pharosPartition60DataCcmStart));
PARTITION_60_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition60Sram1End - pharosPartition60DataSram1Start));
PARTITION_60_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition60Sram2End - pharosPartition60DataSram2Start));
PARTITION_60_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition60Sram3End - pharosPartition60DataSram3Start));
PARTITION_60_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition60SramBackupEnd - pharosPartition60DataSramBackupStart));
PARTITION_60_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition60SdramEnd - pharosPartition60DataSdramStart));

/* definition of the partition 61 alignment for each memory region */
PARTITION_61_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition61CcmEnd - pharosPartition61DataCcmStart));
PARTITION_61_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition61Sram1End - pharosPartition61DataSram1Start));
PARTITION_61_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition61Sram2End - pharosPartition61DataSram2Start));
PARTITION_61_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition61Sram3End - pharosPartition61DataSram3Start));
PARTITION_61_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition61SramBackupEnd - pharosPartition61DataSramBackupStart));
PARTITION_61_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition61SdramEnd - pharosPartition61DataSdramStart));

/* definition of the partition 62 alignment for each memory region */
PARTITION_62_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition62CcmEnd - pharosPartition62DataCcmStart));
PARTITION_62_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition62Sram1End - pharosPartition62DataSram1Start));
PARTITION_62_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition62Sram2End - pharosPartition62DataSram2Start));
PARTITION_62_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition62Sram3End - pharosPartition62DataSram3Start));
PARTITION_62_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition62SramBackupEnd - pharosPartition62DataSramBackupStart));
PARTITION_62_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition62SdramEnd - pharosPartition62DataSdramStart));

/* definition of the partition 63 alignment for each memory region */
PARTITION_63_CCM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition63CcmEnd - pharosPartition63DataCcmStart));
PARTITION_63_SRAM1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition63Sram1End - pharosPartition63DataSram1Start));
PARTITION_63_SRAM2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition63Sram2End - pharosPartition63DataSram2Start));
PARTITION_63_SRAM3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition63Sram3End - pharosPartition63DataSram3Start));
PARTITION_63_SRAMBACKUP_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition63SramBackupEnd - pharosPartition63DataSramBackupStart));
PARTITION_63_SDRAM_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosPartition63SdramEnd - pharosPartition63DataSdramStart));


ENTRY(pharosCpuStart)

/* Define output sections */
SECTIONS
{
  /******************************************************************/
  /*              Start of text and read-only data area             */
  /******************************************************************/

  .text :
  {
    /* source code is in text section */
    pharosStartText = .;

    KEEP(*(.vectorTable .vectorTable.*))
    *(.text)
    *(.text.*)

    /* read-only data is placed in text */
    . = ALIGN (0x10);
    pharosRodataStart = . ;
    *(.rodata)
    *(.rodata.*)
    *(.srodata)
    *(.srodata.*)
    pharosRodataEnd = .;

    /* variable to check the end of the text section */
    . = ALIGN (0x8);
    
    pharosEndText = ROM_END;

  } > flash = 0x00

  /******************************************************************/
  /*                 End of text area and read-only data            */
  /******************************************************************/


  /******************************************************************/
  /*                     Start of kernel area                       */
  /******************************************************************/

  /* address in the flash of the kernel data to copy it to RAM */
  pharosKernelDataCcmLoadAddr        = LOADADDR(.kernelDataCcm);
  pharosKernelDataSram1LoadAddr      = LOADADDR(.kernelDataSram1);
  pharosKernelDataSram2LoadAddr      = LOADADDR(.kernelDataSram2);
  pharosKernelDataSram3LoadAddr      = LOADADDR(.kernelDataSram3);
  pharosKernelDataSramBackupLoadAddr = LOADADDR(.kernelDataSramBackup);
  pharosKernelDataSdramLoadAddr      = LOADADDR(.kernelDataSdram);

  pharosKernelDataCcmSize       = (pharosKernelDataCcmEnd - pharosKernelDataCcmStart) / 8;
  pharosKernelDataSram1Size     = (pharosKernelDataSram1End - pharosKernelDataSram1Start) / 8;
  pharosKernelDataSram2Size     = (pharosKernelDataSram2End - pharosKernelDataSram2Start) / 8;
  pharosKernelDataSram3Size     = (pharosKernelDataSram3End - pharosKernelDataSram3Start) / 8;
  pharosKernelDataSramBackupSize = (pharosKernelDataSramBackupEnd - pharosKernelDataSramBackupStart) / 8;
  pharosKernelDataSdramSize     = (pharosKernelDataSdramEnd - pharosKernelDataSdramStart) / 8;
  

  pharosKernelBssCcmSize       = (pharosKernelBssCcmEnd - pharosKernelBssCcmStart) / 8;
  pharosKernelBssSram1Size     = (pharosKernelBssSram1End - pharosKernelBssSram1Start) / 8;
  pharosKernelBssSram2Size     = (pharosKernelBssSram2End - pharosKernelBssSram2Start) / 8;
  pharosKernelBssSram3Size     = (pharosKernelBssSram3End - pharosKernelBssSram3Start) / 8;
  pharosKernelBssSramBackupSize = (pharosKernelBssSramBackupEnd - pharosKernelBssSramBackupStart) / 8;
  pharosKernelBssSdramSize     = (pharosKernelBssSdramEnd - pharosKernelBssSdramStart) / 8;
  

  /****************/
  /*   CCM area   */
  /****************/

  /* Data section on CCM */
  .kernelDataCcm : ALIGN(8)
  {
     CREATE_OBJECT_SYMBOLS

    pharosKernelCcmStart = .;
   
    . = ALIGN(KERNEL_ALIGNMENT);

    pharosKernelDataCcmStart = .;

    *(.kernelDataCcm)  
    *(.kernelDataCcm.*)

    . = ALIGN(8);
    pharosKernelDataCcmEnd = .;
  } > ccm AT > flash = 0xff

    
  /* BSS and USS sections on CCM */
  .kernelBssUssCcm (NOLOAD) : ALIGN(8)
  {
    . = ALIGN(8);

    pharosKernelBssCcmStart = .;

    *(.kernelBssCcm)  
    *(.kernelBssCcm.*)

    . = ALIGN(8);
    pharosKernelBssCcmEnd = .;

    pharosKernelUssCcmStart = .;

    *(.kernelUssCcm)  
    *(.kernelUssCcm.*)

    pharosKernelUssCcmEnd = .;
    . = ALIGN(8);

    pharosKernelCcmEnd = .;
  } > ccm 

  /****************/
  /*  SRAM1 area  */
  /****************/

  /* Data section on SRAM1 */
  .kernelDataSram1 : ALIGN(8)
  {
     CREATE_OBJECT_SYMBOLS

    pharosKernelSram1Start = .;
   
    . = ALIGN(KERNEL_ALIGNMENT);

    pharosKernelDataSram1Start = .;

    *(.kernelDataSram1)  
    *(.kernelDataSram1.*)

    . = ALIGN(8);
    pharosKernelDataSram1End = .;
  } > sram1 AT > flash = 0xff

    
  /* BSS and USS sections on SRAM1 */
  .kernelBssUssSram1 (NOLOAD) : ALIGN(8)
  {
    . = ALIGN(8);

    pharosKernelBssSram1Start = .;

    *(.kernelBssSram1)  
    *(.kernelBssSram1.*)

    . = ALIGN(8);
    pharosKernelBssSram1End = .;

    pharosKernelUssSram1Start = .;

    *(.kernelUssSram1)  
    *(.kernelUssSram1.*)

    pharosKernelUssSram1End = .;
    . = ALIGN(8);

    pharosKernelSram1End = .;
  } > sram1 

  /****************/
  /*  SRAM2 area  */
  /****************/

  /* Data section on SRAM2 */
  .kernelDataSram2 : ALIGN(8)
  {
     CREATE_OBJECT_SYMBOLS

    pharosKernelSram2Start = .;
   
    . = ALIGN(KERNEL_ALIGNMENT);

    pharosKernelDataSram2Start = .;

    *(.kernelDataSram2)  
    *(.kernelDataSram2.*)

    . = ALIGN(8);
    pharosKernelDataSram2End = .;
  } > sram2 AT > flash = 0xff

    
  /* BSS and USS sections on SRAM2 */
  .kernelBssUssSram2 (NOLOAD) : ALIGN(8)
  {
    . = ALIGN(8);

    pharosKernelBssSram2Start = .;

    *(.kernelBssSram2)  
    *(.kernelBssSram2.*)

    . = ALIGN(8);
    pharosKernelBssSram2End = .;

    pharosKernelUssSram2Start = .;

    *(.kernelUssSram2)  
    *(.kernelUssSram2.*)

    pharosKernelUssSram2End = .;
    . = ALIGN(8);

    pharosKernelSram2End = .;
  } > sram2 


  /****************/
  /*  SRAM3 area  */
  /****************/

  /* Data section on SRAM3 */
  .kernelDataSram3 : ALIGN(8)
  {
     CREATE_OBJECT_SYMBOLS

    pharosKernelSram3Start = .;
   
    . = ALIGN(KERNEL_ALIGNMENT);

    pharosKernelDataSram3Start = .;

    *(.kernelDataSram3)  
    *(.kernelDataSram3.*)

    . = ALIGN(8);
    pharosKernelDataSram3End = .;
  } > sram3 AT > flash = 0xff

    
  /* BSS and USS sections on SRAM3 */
  .kernelBssUssSram3 (NOLOAD) : ALIGN(8)
  {
    . = ALIGN(8);

    pharosKernelBssSram3Start = .;

    *(.kernelBssSram3)  
    *(.kernelBssSram3.*)

    . = ALIGN(8);
    pharosKernelBssSram3End = .;

    pharosKernelUssSram3Start = .;

    *(.kernelUssSram3)  
    *(.kernelUssSram3.*)

    pharosKernelUssSram3End = .;
    . = ALIGN(8);

    pharosKernelSram3End = .;
  } > sram3 

  /*****************/
  /*SRAMBackup area*/
  /*****************/

  /* Data section on SRAM Backup */
  .kernelDataSramBackup : ALIGN(8)
  {
     CREATE_OBJECT_SYMBOLS

    pharosKernelSramBackupStart = .;
   
    . = ALIGN(KERNEL_ALIGNMENT);

    pharosKernelDataSramBackupStart = .;

    *(.kernelDataSramBackup)  
    *(.kernelDataSramBackup.*)

    . = ALIGN(8);
    pharosKernelDataSramBackupEnd = .;
  } > sramBackup AT > flash = 0xff

    
  /* BSS and USS sections on SRAM Backup */
  .kernelBssUssSramBackup (NOLOAD) : ALIGN(8)
  {
    . = ALIGN(8);

    pharosKernelBssSramBackupStart = .;

    *(.kernelBssSramBackup)  
    *(.kernelBssSramBackup.*)

    . = ALIGN(8);
    pharosKernelBssSramBackupEnd = .;

    pharosKernelUssSramBackupStart = .;

    *(.kernelUssSramBackup)  
    *(.kernelUssSramBackup.*)


    pharosKernelUssSramBackupEnd = .;
    . = ALIGN(8);

    pharosKernelSramBackupEnd = .;
  } > sramBackup 

  /****************/
  /*  SDRAM area  */
  /****************/

  /* Data section on SDRAM */
  .kernelDataSdram : ALIGN(8)
  {
     CREATE_OBJECT_SYMBOLS

    pharosKernelSdramStart = .;
   
    . = ALIGN(KERNEL_ALIGNMENT);

    pharosKernelDataSdramStart = .;

    *(.kernelDataSdram)  
    *(.kernelDataSdram.*)

    . = ALIGN(8);
    pharosKernelDataSdramEnd = .;
  } > sdram AT > flash = 0xff

    
  /* BSS and USS sections on SDRAM */
  .kernelBssUssSdram (NOLOAD) : ALIGN(8)
  {
    . = ALIGN(8);

    pharosKernelBssSdramStart = .;

    *(.kernelBssSdram)  
    *(.kernelBssSdram.*)

    . = ALIGN(8);
    pharosKernelBssSdramEnd = .;

    pharosKernelUssSdramStart = .;

    *(.kernelUssSdram)  
    *(.kernelUssSdram.*)

    pharosKernelUssSdramEnd = .;
    . = ALIGN(8);

    pharosKernelSdramEnd = .;
  } > sdram


  /****************/
  /*  Bulks       */
  /****************/
 
  .kernelBulksCcm (NOLOAD) : 
  {
    CREATE_OBJECT_SYMBOLS
    
    /* need to align the bulks to their size at least */
    . = ALIGN(CHANNEL_BULK_SIZE);

    pharosBulkCcmStart = .;

    *(.kernelBulksCcm)  
    *(.kernelBulksCcm.*)

    pharosBulkCcmEnd = .;

    pharosKernelCcmEnd = .;

    /*
     * note that there may be overloap (we are not aligning here to KERNEL_ALIGNMENT) 
     * from this kernel section to the next section, but that is not an issue 
     * because the kernel already has access to all sections 
     */
  } > ccm

  .kernelBulksSram1 (NOLOAD) : 
  {
    CREATE_OBJECT_SYMBOLS
    
    /* need to align the bulks to their size at least */
    . = ALIGN(CHANNEL_BULK_SIZE);

    pharosBulkSram1Start = .;

    *(.kernelBulksSram1)  
    *(.kernelBulksSram1.*)

    pharosBulkSram1End = .;

    pharosKernelSram1End = .;

    /*
     * note that there may be overloap (we are not aligning here to KERNEL_ALIGNMENT) 
     * from this kernel section to the next section, but that is not an issue 
     * because the kernel already has access to all sections 
     */
  } > sram1
 
  .kernelBulksSram2 (NOLOAD) : 
  {
    CREATE_OBJECT_SYMBOLS
    
    /* need to align the bulks to their size at least */
    . = ALIGN(CHANNEL_BULK_SIZE);

    pharosBulkSram2Start = .;

    *(.kernelBulksSram2)  
    *(.kernelBulksSram2.*)

    pharosBulkSram2End = .;

    pharosKernelSram2End = .;

    /*
     * note that there may be overloap (we are not aligning here to KERNEL_ALIGNMENT) 
     * from this kernel section to the next section, but that is not an issue 
     * because the kernel already has access to all sections 
     */
  } > sram2
 
  .kernelBulksSram3 (NOLOAD) : 
  {
    CREATE_OBJECT_SYMBOLS
    
    /* need to align the bulks to their size at least */
    . = ALIGN(CHANNEL_BULK_SIZE);

    pharosBulkSram3Start = .;

    *(.kernelBulksSram3)  
    *(.kernelBulksSram3.*)

    pharosBulkSram3End = .;

    pharosKernelSram3End = .;

    /*
     * note that there may be overloap (we are not aligning here to KERNEL_ALIGNMENT) 
     * from this kernel section to the next section, but that is not an issue 
     * because the kernel already has access to all sections 
     */
  } > sram3
 
  .kernelBulksSramBackup (NOLOAD) : 
  {
    CREATE_OBJECT_SYMBOLS
    
    /* need to align the bulks to their size at least */
    . = ALIGN(CHANNEL_BULK_SIZE);

    pharosBulkSramBackupStart = .;

    *(.kernelBulksSramBackup)  
    *(.kernelBulksSramBackup.*)

    pharosBulkSramBackupEnd = .;

    pharosKernelSramBackupEnd = .;

    /*
     * note that there may be overloap (we are not aligning here to KERNEL_ALIGNMENT) 
     * from this kernel section to the next section, but that is not an issue 
     * because the kernel already has access to all sections 
     */
  } > sramBackup
 
  .kernelBulksSdram (NOLOAD) : 
  {
    CREATE_OBJECT_SYMBOLS
    
    /* need to align the bulks to their size at least */
    . = ALIGN(CHANNEL_BULK_SIZE);

    pharosBulkSdramStart = .;

    *(.kernelBulksSdram)  
    *(.kernelBulksSdram.*)

    pharosBulkSdramEnd = .;

    pharosKernelSdramEnd = .;

    /*
     * note that there may be overloap (we are not aligning here to KERNEL_ALIGNMENT) 
     * from this kernel section to the next section, but that is not an issue 
     * because the kernel already has access to all sections 
     */
  } > sdram
 

  /******************************************************************/
  /*                  Start of shared stack area                    */
  /******************************************************************/

  .pharosSharedStackCcm (NOLOAD) : 
  {
    CREATE_OBJECT_SYMBOLS

    pharosSharedStackCcmStart = .;

    /* make sure the next region does not start before the MPU region of this one ends */
    . = ALIGN(SHARED_STACK_ALIGNMENT);

    /* address of the shared stack section start in RAM */
    pharosSharedStackCcmStart = .;

    /* each shared stack must be aligned to its size (e.g. stack of 1KiB must be aligned to 1KiB) */
    *(.sharedStackCcm)
    *(.sharedStackCcm.*)

    . = ALIGN(SHARED_STACK_ALIGNMENT);

    pharosSharedStackCcmEnd = .;
   
  } > ccm


  .pharosSharedStackSram1 (NOLOAD) : 
  {
    CREATE_OBJECT_SYMBOLS

    pharosSharedStackSram1Start = .;

    /* make sure the next region does not start before the MPU region of this one ends */
    . = ALIGN(SHARED_STACK_ALIGNMENT);

    /* address of the shared stack section start in RAM */
    pharosSharedStackSram1Start = .;

    /* each shared stack must be aligned to its size (e.g. stack of 1KiB must be aligned to 1KiB) */
    *(.sharedStackSram1)
    *(.sharedStackSram1.*)

    . = ALIGN(SHARED_STACK_ALIGNMENT);

    pharosSharedStackSram1End = .;
   
  } > sram1

  .pharosSharedStackSram2 (NOLOAD) : 
  {
    CREATE_OBJECT_SYMBOLS

    pharosSharedStackSram2Start = .;

    /* make sure the next region does not start before the MPU region of this one ends */
    . = ALIGN(SHARED_STACK_ALIGNMENT);

    /* address of the shared stack section start in RAM */
    pharosSharedStackSram2Start = .;

    /* each shared stack must be aligned to its size (e.g. stack of 1KiB must be aligned to 1KiB) */
    *(.sharedStackSram2)
    *(.sharedStackSram2.*)

    . = ALIGN(SHARED_STACK_ALIGNMENT);

    pharosSharedStackSram2End = .;
   
  } > sram2

  .pharosSharedStackSram3 (NOLOAD) : 
  {
    CREATE_OBJECT_SYMBOLS

    pharosSharedStackSram3Start = .;

    /* make sure the next region does not start before the MPU region of this one ends */
    . = ALIGN(SHARED_STACK_ALIGNMENT);

    /* address of the shared stack section start in RAM */
    pharosSharedStackSram3Start = .;

    /* each shared stack must be aligned to its size (e.g. stack of 1KiB must be aligned to 1KiB) */
    *(.sharedStackSram3)
    *(.sharedStackSram3.*)

    . = ALIGN(SHARED_STACK_ALIGNMENT);

    pharosSharedStackSram3End = .;
   
  } > sram3

  .pharosSharedStackSramBackup (NOLOAD) : 
  {
    CREATE_OBJECT_SYMBOLS

    pharosSharedStackSramBackupStart = .;

    /* make sure the next region does not start before the MPU region of this one ends */
    . = ALIGN(SHARED_STACK_ALIGNMENT);

    /* address of the shared stack section start in RAM */
    pharosSharedStackSramBackupStart = .;

    /* each shared stack must be aligned to its size (e.g. stack of 1KiB must be aligned to 1KiB) */
    *(.sharedStackSramBackup)
    *(.sharedStackSramBackup.*)

    . = ALIGN(SHARED_STACK_ALIGNMENT);

    pharosSharedStackSramBackupEnd = .;
   
  } > sramBackup


  .pharosSharedStackSdram (NOLOAD) : 
  {
    CREATE_OBJECT_SYMBOLS

    pharosSharedStackSdramStart = .;

    /* make sure the next region does not start before the MPU region of this one ends */
    . = ALIGN(SHARED_STACK_ALIGNMENT);

    /* address of the shared stack section start in RAM */
    pharosSharedStackSdramStart = .;

    /* each shared stack must be aligned to its size (e.g. stack of 1KiB must be aligned to 1KiB) */
    *(.sharedStackSdram)
    *(.sharedStackSdram.*)

    . = ALIGN(SHARED_STACK_ALIGNMENT);

    pharosSharedStackSdramEnd = .;
   
  } > sdram


  /******************************************************************/
  /*                        End of kernel space                     */
  /******************************************************************/

  /******************************************************************/
  /*                     Start of Shared data area                  */
  /******************************************************************/

  /* address in the flash of the shared data to copy it to RAM */
  pharosSharedDataCcmLoadAddr        = LOADADDR(.sharedDataCcm);
  pharosSharedDataSram1LoadAddr      = LOADADDR(.sharedDataSram1);
  pharosSharedDataSram2LoadAddr      = LOADADDR(.sharedDataSram2);
  pharosSharedDataSram3LoadAddr      = LOADADDR(.sharedDataSram3);
  pharosSharedDataSramBackupLoadAddr = LOADADDR(.sharedDataSramBackup);
  pharosSharedDataSdramLoadAddr      = LOADADDR(.sharedDataSdram);
  
  pharosSharedDataCcmSize = (pharosSharedDataCcmEnd - pharosSharedCcmStart) / 8;
  pharosSharedDataSram1Size = (pharosSharedDataSram1End - pharosSharedSram1Start) / 8;
  pharosSharedDataSram2Size = (pharosSharedDataSram2End - pharosSharedSram2Start) / 8;
  pharosSharedDataSram3Size = (pharosSharedDataSram3End - pharosSharedSram3Start) / 8;
  pharosSharedDataSramBackupSize = (pharosSharedDataSramBackupEnd - pharosSharedSramBackupStart) / 8;
  pharosSharedDataSdramSize = (pharosSharedDataSdramEnd - pharosSharedSdramStart) / 8;

  pharosSharedBssCcmSize = (pharosSharedBssCcmEnd - pharosSharedBssCcmStart) / 8;
  pharosSharedBssSram1Size = (pharosSharedBssSram1End - pharosSharedBssSram1Start) / 8;
  pharosSharedBssSram2Size = (pharosSharedBssSram2End - pharosSharedBssSram2Start) / 8;
  pharosSharedBssSram3Size = (pharosSharedBssSram3End - pharosSharedBssSram3Start) / 8;
  pharosSharedBssSramBackupSize = (pharosSharedBssSramBackupEnd - pharosSharedBssSramBackupStart) / 8;
  pharosSharedBssSdramSize = (pharosSharedBssSdramEnd - pharosSharedBssSdramStart) / 8;


  /****************/
  /*   CCM area   */
  /****************/

  /* Data section on CCM */
  .sharedDataCcm : ALIGN(8)
  {
     CREATE_OBJECT_SYMBOLS

    pharosSharedCcmStart = .;
   
    . = ALIGN(SHARED_CCM_ALIGNMENT);

    pharosSharedDataCcmStart = .;

    *(.sharedDataCcm)  
    *(.sharedDataCcm.*)

    . = ALIGN(8);
    pharosSharedDataCcmEnd = .;
  } > ccm AT > flash = 0xff

    
  /* BSS and USS sections on CCM */
  .sharedBssUssCcm (NOLOAD) : ALIGN(8)
  {
    . = ALIGN(8);

    pharosSharedBssCcmStart = .;

    *(.sharedBssCcm)  
    *(.sharedBssCcm.*)

    . = ALIGN(8);
    pharosSharedBssCcmEnd = .;

    pharosSharedUssCcmStart = .;

    *(.sharedUssCcm)  
    *(.sharedUssCcm.*)

    pharosSharedUssCcmEnd = .;

    . = ALIGN(SHARED_CCM_ALIGNMENT);

    pharosSharedCcmEnd = .;
  } > ccm 

  /****************/
  /*  SRAM1 area  */
  /****************/

  /* Data section on SRAM1 */
  .sharedDataSram1 : ALIGN(8)
  {
     CREATE_OBJECT_SYMBOLS

    pharosSharedSram1Start = .;
   
    . = ALIGN(SHARED_SRAM1_ALIGNMENT);

    pharosSharedDataSram1Start = .;

    *(.sharedDataSram1)  
    *(.sharedDataSram1.*)

    . = ALIGN(8);
    pharosSharedDataSram1End = .;
  } > sram1 AT > flash = 0xff

    
  /* BSS and USS sections on SRAM1 */
  .sharedBssUssSram1 (NOLOAD) : ALIGN(8)
  {
    . = ALIGN(8);

    pharosSharedBssSram1Start = .;

    *(.sharedBssSram1)  
    *(.sharedBssSram1.*)

    . = ALIGN(8);
    pharosSharedBssSram1End = .;

    pharosSharedUssSram1Start = .;

    *(.sharedUssSram1)  
    *(.sharedUssSram1.*)

    pharosSharedUssSram1End = .;

    . = ALIGN(SHARED_SRAM1_ALIGNMENT);

    pharosSharedSram1End = .;
  } > sram1 

  /****************/
  /*  SRAM2 area  */
  /****************/

  /* Data section on SRAM2 */
  .sharedDataSram2 : ALIGN(8)
  {
     CREATE_OBJECT_SYMBOLS

    pharosSharedSram2Start = .;
   
    . = ALIGN(SHARED_SRAM2_ALIGNMENT);

    pharosSharedDataSram2Start = .;

    *(.sharedDataSram2)  
    *(.sharedDataSram2.*)

    . = ALIGN(8);
    pharosSharedDataSram2End = .;
  } > sram2 AT > flash = 0xff

    
  /* BSS and USS sections on SRAM2 */
  .sharedBssUssSram2 (NOLOAD) : ALIGN(8)
  {
    . = ALIGN(8);

    pharosSharedBssSram2Start = .;

    *(.sharedBssSram2)  
    *(.sharedBssSram2.*)

    . = ALIGN(8);
    pharosSharedBssSram2End = .;

    pharosSharedUssSram2Start = .;

    *(.sharedUssSram2)  
    *(.sharedUssSram2.*)

    pharosSharedUssSram2End = .;

    . = ALIGN(SHARED_SRAM2_ALIGNMENT);

    pharosSharedSram2End = .;
  } > sram2 


  /****************/
  /*  SRAM3 area  */
  /****************/

  /* Data section on SRAM3 */
  .sharedDataSram3 : ALIGN(8)
  {
     CREATE_OBJECT_SYMBOLS

    pharosSharedSram3Start = .;
   
    . = ALIGN(SHARED_SRAM2_ALIGNMENT);

    pharosSharedDataSram3Start = .;

    *(.sharedDataSram3)  
    *(.sharedDataSram3.*)

    . = ALIGN(8);
    pharosSharedDataSram3End = .;
  } > sram3 AT > flash = 0xff

    
  /* BSS and USS sections on SRAM3 */
  .sharedBssUssSram3 (NOLOAD) : ALIGN(8)
  {
    . = ALIGN(8);

    pharosSharedBssSram3Start = .;

    *(.sharedBssSram3)  
    *(.sharedBssSram3.*)

    . = ALIGN(8);
    pharosSharedBssSram3End = .;

    pharosSharedUssSram3Start = .;

    *(.sharedUssSram3)  
    *(.sharedUssSram3.*)

    pharosSharedUssSram3End = .;

    . = ALIGN(SHARED_SRAM3_ALIGNMENT);

    pharosSharedSram3End = .;
  } > sram3 

  /*****************/
  /*SRAMBackup area*/
  /*****************/

  /* Data section on SRAM Backup */
  .sharedDataSramBackup : ALIGN(8)
  {
     CREATE_OBJECT_SYMBOLS

    pharosSharedSramBackupStart = .;
   
    . = ALIGN(SHARED_SRAM_BACKUP_ALIGNMENT);

    pharosSharedDataSramBackupStart = .;

    *(.sharedDataSramBackup)  
    *(.sharedDataSramBackup.*)

    . = ALIGN(8);
    pharosSharedDataSramBackupEnd = .;
  } > sramBackup AT > flash = 0xff

    
  /* BSS and USS sections on SRAM Backup */
  .sharedBssUssSramBackup (NOLOAD) : ALIGN(8)
  {
    . = ALIGN(8);

    pharosSharedBssSramBackupStart = .;

    *(.sharedBssSramBackup)  
    *(.sharedBssSramBackup.*)

    . = ALIGN(8);
    pharosSharedBssSramBackupEnd = .;

    pharosSharedUssSramBackupStart = .;

    *(.sharedUssSramBackup)  
    *(.sharedUssSramBackup.*)

    pharosSharedUssSramBackupEnd = .;

    . = ALIGN(SHARED_SRAM_BACKUP_ALIGNMENT);

    pharosSharedSramBackupEnd = .;
  } > sramBackup 

  /****************/
  /*  SDRAM area  */
  /****************/

  /* Data section on SDRAM */
  .sharedDataSdram : ALIGN(8)
  {
     CREATE_OBJECT_SYMBOLS

    pharosSharedSdramStart = .;
   
    . = ALIGN(SHARED_SDRAM_ALIGNMENT);

    pharosSharedDataSdramStart = .;

    *(.sharedDataSdram)  
    *(.sharedDataSdram.*)

    . = ALIGN(8);
    pharosSharedDataSdramEnd = .;
  } > sdram AT > flash = 0xff

    
  /* BSS and USS sections on SDRAM */
  .sharedBssUssSdram (NOLOAD) : ALIGN(8)
  {
    . = ALIGN(8);

    pharosSharedBssSdramStart = .;

    *(.sharedBssSdram)  
    *(.sharedBssSdram.*)

    . = ALIGN(8);
    pharosSharedBssSdramEnd = .;

    pharosSharedUssSdramStart = .;

    *(.sharedUssSdram)  
    *(.sharedUssSdram.*)

    pharosSharedUssSdramEnd = .;

    . = ALIGN(SHARED_SDRAM_ALIGNMENT);

    pharosSharedSdramEnd = .;
  } > sdram


  /******************************************************************/
  /*                        End of shared data                      */
  /******************************************************************/

 /******************************************************************/
  /* Separate the partitions data space to enable memory protection */
  /******************************************************************/

  /* determine the size of the data in 64 bit variables */
  pharosPartition0DataCcmSize = (pharosPartition0DataCcmEnd - pharosPartition0CcmStart) / 8;
  pharosPartition1DataCcmSize = (pharosPartition1DataCcmEnd - pharosPartition1CcmStart) / 8;
  pharosPartition2DataCcmSize = (pharosPartition2DataCcmEnd - pharosPartition2CcmStart) / 8;
  pharosPartition3DataCcmSize = (pharosPartition3DataCcmEnd - pharosPartition3CcmStart) / 8;
  pharosPartition4DataCcmSize = (pharosPartition4DataCcmEnd - pharosPartition4CcmStart) / 8;
  pharosPartition5DataCcmSize = (pharosPartition5DataCcmEnd - pharosPartition5CcmStart) / 8;
  pharosPartition6DataCcmSize = (pharosPartition6DataCcmEnd - pharosPartition6CcmStart) / 8;
  pharosPartition7DataCcmSize = (pharosPartition7DataCcmEnd - pharosPartition7CcmStart) / 8;
  pharosPartition8DataCcmSize = (pharosPartition8DataCcmEnd - pharosPartition8CcmStart) / 8;
  pharosPartition9DataCcmSize = (pharosPartition9DataCcmEnd - pharosPartition9CcmStart) / 8;
  pharosPartition10DataCcmSize = (pharosPartition10DataCcmEnd - pharosPartition10CcmStart) / 8;
  pharosPartition11DataCcmSize = (pharosPartition11DataCcmEnd - pharosPartition11CcmStart) / 8;
  pharosPartition12DataCcmSize = (pharosPartition12DataCcmEnd - pharosPartition12CcmStart) / 8;
  pharosPartition13DataCcmSize = (pharosPartition13DataCcmEnd - pharosPartition13CcmStart) / 8;
  pharosPartition14DataCcmSize = (pharosPartition14DataCcmEnd - pharosPartition14CcmStart) / 8;
  pharosPartition15DataCcmSize = (pharosPartition15DataCcmEnd - pharosPartition15CcmStart) / 8;
  pharosPartition16DataCcmSize = (pharosPartition16DataCcmEnd - pharosPartition16CcmStart) / 8;
  pharosPartition17DataCcmSize = (pharosPartition17DataCcmEnd - pharosPartition17CcmStart) / 8;
  pharosPartition18DataCcmSize = (pharosPartition18DataCcmEnd - pharosPartition18CcmStart) / 8;
  pharosPartition19DataCcmSize = (pharosPartition19DataCcmEnd - pharosPartition19CcmStart) / 8;
  pharosPartition20DataCcmSize = (pharosPartition20DataCcmEnd - pharosPartition20CcmStart) / 8;
  pharosPartition21DataCcmSize = (pharosPartition21DataCcmEnd - pharosPartition21CcmStart) / 8;
  pharosPartition22DataCcmSize = (pharosPartition22DataCcmEnd - pharosPartition22CcmStart) / 8;
  pharosPartition23DataCcmSize = (pharosPartition23DataCcmEnd - pharosPartition23CcmStart) / 8;
  pharosPartition24DataCcmSize = (pharosPartition24DataCcmEnd - pharosPartition24CcmStart) / 8;
  pharosPartition25DataCcmSize = (pharosPartition25DataCcmEnd - pharosPartition25CcmStart) / 8;
  pharosPartition26DataCcmSize = (pharosPartition26DataCcmEnd - pharosPartition26CcmStart) / 8;
  pharosPartition27DataCcmSize = (pharosPartition27DataCcmEnd - pharosPartition27CcmStart) / 8;
  pharosPartition28DataCcmSize = (pharosPartition28DataCcmEnd - pharosPartition28CcmStart) / 8;
  pharosPartition29DataCcmSize = (pharosPartition29DataCcmEnd - pharosPartition29CcmStart) / 8;
  pharosPartition30DataCcmSize = (pharosPartition30DataCcmEnd - pharosPartition30CcmStart) / 8;
  pharosPartition31DataCcmSize = (pharosPartition31DataCcmEnd - pharosPartition31CcmStart) / 8;
  pharosPartition32DataCcmSize = (pharosPartition32DataCcmEnd - pharosPartition32CcmStart) / 8;
  pharosPartition33DataCcmSize = (pharosPartition33DataCcmEnd - pharosPartition33CcmStart) / 8;
  pharosPartition34DataCcmSize = (pharosPartition34DataCcmEnd - pharosPartition34CcmStart) / 8;
  pharosPartition35DataCcmSize = (pharosPartition35DataCcmEnd - pharosPartition35CcmStart) / 8;
  pharosPartition36DataCcmSize = (pharosPartition36DataCcmEnd - pharosPartition36CcmStart) / 8;
  pharosPartition37DataCcmSize = (pharosPartition37DataCcmEnd - pharosPartition37CcmStart) / 8;
  pharosPartition38DataCcmSize = (pharosPartition38DataCcmEnd - pharosPartition38CcmStart) / 8;
  pharosPartition39DataCcmSize = (pharosPartition39DataCcmEnd - pharosPartition39CcmStart) / 8;
  pharosPartition40DataCcmSize = (pharosPartition40DataCcmEnd - pharosPartition40CcmStart) / 8;
  pharosPartition41DataCcmSize = (pharosPartition41DataCcmEnd - pharosPartition41CcmStart) / 8;
  pharosPartition42DataCcmSize = (pharosPartition42DataCcmEnd - pharosPartition42CcmStart) / 8;
  pharosPartition43DataCcmSize = (pharosPartition43DataCcmEnd - pharosPartition43CcmStart) / 8;
  pharosPartition44DataCcmSize = (pharosPartition44DataCcmEnd - pharosPartition44CcmStart) / 8;
  pharosPartition45DataCcmSize = (pharosPartition45DataCcmEnd - pharosPartition45CcmStart) / 8;
  pharosPartition46DataCcmSize = (pharosPartition46DataCcmEnd - pharosPartition46CcmStart) / 8;
  pharosPartition47DataCcmSize = (pharosPartition47DataCcmEnd - pharosPartition47CcmStart) / 8;
  pharosPartition48DataCcmSize = (pharosPartition48DataCcmEnd - pharosPartition48CcmStart) / 8;
  pharosPartition49DataCcmSize = (pharosPartition49DataCcmEnd - pharosPartition49CcmStart) / 8;
  pharosPartition50DataCcmSize = (pharosPartition50DataCcmEnd - pharosPartition50CcmStart) / 8;
  pharosPartition51DataCcmSize = (pharosPartition51DataCcmEnd - pharosPartition51CcmStart) / 8;
  pharosPartition52DataCcmSize = (pharosPartition52DataCcmEnd - pharosPartition52CcmStart) / 8;
  pharosPartition53DataCcmSize = (pharosPartition53DataCcmEnd - pharosPartition53CcmStart) / 8;
  pharosPartition54DataCcmSize = (pharosPartition54DataCcmEnd - pharosPartition54CcmStart) / 8;
  pharosPartition55DataCcmSize = (pharosPartition55DataCcmEnd - pharosPartition55CcmStart) / 8;
  pharosPartition56DataCcmSize = (pharosPartition56DataCcmEnd - pharosPartition56CcmStart) / 8;
  pharosPartition57DataCcmSize = (pharosPartition57DataCcmEnd - pharosPartition57CcmStart) / 8;
  pharosPartition58DataCcmSize = (pharosPartition58DataCcmEnd - pharosPartition58CcmStart) / 8;
  pharosPartition59DataCcmSize = (pharosPartition59DataCcmEnd - pharosPartition59CcmStart) / 8;
  pharosPartition60DataCcmSize = (pharosPartition60DataCcmEnd - pharosPartition60CcmStart) / 8;
  pharosPartition61DataCcmSize = (pharosPartition61DataCcmEnd - pharosPartition61CcmStart) / 8;
  pharosPartition62DataCcmSize = (pharosPartition62DataCcmEnd - pharosPartition62CcmStart) / 8;
  pharosPartition63DataCcmSize = (pharosPartition63DataCcmEnd - pharosPartition63CcmStart) / 8;

  pharosPartition0DataSram1Size = (pharosPartition0DataSram1End - pharosPartition0Sram1Start) / 8;
  pharosPartition1DataSram1Size = (pharosPartition1DataSram1End - pharosPartition1Sram1Start) / 8;
  pharosPartition2DataSram1Size = (pharosPartition2DataSram1End - pharosPartition2Sram1Start) / 8;
  pharosPartition3DataSram1Size = (pharosPartition3DataSram1End - pharosPartition3Sram1Start) / 8;
  pharosPartition4DataSram1Size = (pharosPartition4DataSram1End - pharosPartition4Sram1Start) / 8;
  pharosPartition5DataSram1Size = (pharosPartition5DataSram1End - pharosPartition5Sram1Start) / 8;
  pharosPartition6DataSram1Size = (pharosPartition6DataSram1End - pharosPartition6Sram1Start) / 8;
  pharosPartition7DataSram1Size = (pharosPartition7DataSram1End - pharosPartition7Sram1Start) / 8;
  pharosPartition8DataSram1Size = (pharosPartition8DataSram1End - pharosPartition8Sram1Start) / 8;
  pharosPartition9DataSram1Size = (pharosPartition9DataSram1End - pharosPartition9Sram1Start) / 8;
  pharosPartition10DataSram1Size = (pharosPartition10DataSram1End - pharosPartition10Sram1Start) / 8;
  pharosPartition11DataSram1Size = (pharosPartition11DataSram1End - pharosPartition11Sram1Start) / 8;
  pharosPartition12DataSram1Size = (pharosPartition12DataSram1End - pharosPartition12Sram1Start) / 8;
  pharosPartition13DataSram1Size = (pharosPartition13DataSram1End - pharosPartition13Sram1Start) / 8;
  pharosPartition14DataSram1Size = (pharosPartition14DataSram1End - pharosPartition14Sram1Start) / 8;
  pharosPartition15DataSram1Size = (pharosPartition15DataSram1End - pharosPartition15Sram1Start) / 8;
  pharosPartition16DataSram1Size = (pharosPartition16DataSram1End - pharosPartition16Sram1Start) / 8;
  pharosPartition17DataSram1Size = (pharosPartition17DataSram1End - pharosPartition17Sram1Start) / 8;
  pharosPartition18DataSram1Size = (pharosPartition18DataSram1End - pharosPartition18Sram1Start) / 8;
  pharosPartition19DataSram1Size = (pharosPartition19DataSram1End - pharosPartition19Sram1Start) / 8;
  pharosPartition20DataSram1Size = (pharosPartition20DataSram1End - pharosPartition20Sram1Start) / 8;
  pharosPartition21DataSram1Size = (pharosPartition21DataSram1End - pharosPartition21Sram1Start) / 8;
  pharosPartition22DataSram1Size = (pharosPartition22DataSram1End - pharosPartition22Sram1Start) / 8;
  pharosPartition23DataSram1Size = (pharosPartition23DataSram1End - pharosPartition23Sram1Start) / 8;
  pharosPartition24DataSram1Size = (pharosPartition24DataSram1End - pharosPartition24Sram1Start) / 8;
  pharosPartition25DataSram1Size = (pharosPartition25DataSram1End - pharosPartition25Sram1Start) / 8;
  pharosPartition26DataSram1Size = (pharosPartition26DataSram1End - pharosPartition26Sram1Start) / 8;
  pharosPartition27DataSram1Size = (pharosPartition27DataSram1End - pharosPartition27Sram1Start) / 8;
  pharosPartition28DataSram1Size = (pharosPartition28DataSram1End - pharosPartition28Sram1Start) / 8;
  pharosPartition29DataSram1Size = (pharosPartition29DataSram1End - pharosPartition29Sram1Start) / 8;
  pharosPartition30DataSram1Size = (pharosPartition30DataSram1End - pharosPartition30Sram1Start) / 8;
  pharosPartition31DataSram1Size = (pharosPartition31DataSram1End - pharosPartition31Sram1Start) / 8;
  pharosPartition32DataSram1Size = (pharosPartition32DataSram1End - pharosPartition32Sram1Start) / 8;
  pharosPartition33DataSram1Size = (pharosPartition33DataSram1End - pharosPartition33Sram1Start) / 8;
  pharosPartition34DataSram1Size = (pharosPartition34DataSram1End - pharosPartition34Sram1Start) / 8;
  pharosPartition35DataSram1Size = (pharosPartition35DataSram1End - pharosPartition35Sram1Start) / 8;
  pharosPartition36DataSram1Size = (pharosPartition36DataSram1End - pharosPartition36Sram1Start) / 8;
  pharosPartition37DataSram1Size = (pharosPartition37DataSram1End - pharosPartition37Sram1Start) / 8;
  pharosPartition38DataSram1Size = (pharosPartition38DataSram1End - pharosPartition38Sram1Start) / 8;
  pharosPartition39DataSram1Size = (pharosPartition39DataSram1End - pharosPartition39Sram1Start) / 8;
  pharosPartition40DataSram1Size = (pharosPartition40DataSram1End - pharosPartition40Sram1Start) / 8;
  pharosPartition41DataSram1Size = (pharosPartition41DataSram1End - pharosPartition41Sram1Start) / 8;
  pharosPartition42DataSram1Size = (pharosPartition42DataSram1End - pharosPartition42Sram1Start) / 8;
  pharosPartition43DataSram1Size = (pharosPartition43DataSram1End - pharosPartition43Sram1Start) / 8;
  pharosPartition44DataSram1Size = (pharosPartition44DataSram1End - pharosPartition44Sram1Start) / 8;
  pharosPartition45DataSram1Size = (pharosPartition45DataSram1End - pharosPartition45Sram1Start) / 8;
  pharosPartition46DataSram1Size = (pharosPartition46DataSram1End - pharosPartition46Sram1Start) / 8;
  pharosPartition47DataSram1Size = (pharosPartition47DataSram1End - pharosPartition47Sram1Start) / 8;
  pharosPartition48DataSram1Size = (pharosPartition48DataSram1End - pharosPartition48Sram1Start) / 8;
  pharosPartition49DataSram1Size = (pharosPartition49DataSram1End - pharosPartition49Sram1Start) / 8;
  pharosPartition50DataSram1Size = (pharosPartition50DataSram1End - pharosPartition50Sram1Start) / 8;
  pharosPartition51DataSram1Size = (pharosPartition51DataSram1End - pharosPartition51Sram1Start) / 8;
  pharosPartition52DataSram1Size = (pharosPartition52DataSram1End - pharosPartition52Sram1Start) / 8;
  pharosPartition53DataSram1Size = (pharosPartition53DataSram1End - pharosPartition53Sram1Start) / 8;
  pharosPartition54DataSram1Size = (pharosPartition54DataSram1End - pharosPartition54Sram1Start) / 8;
  pharosPartition55DataSram1Size = (pharosPartition55DataSram1End - pharosPartition55Sram1Start) / 8;
  pharosPartition56DataSram1Size = (pharosPartition56DataSram1End - pharosPartition56Sram1Start) / 8;
  pharosPartition57DataSram1Size = (pharosPartition57DataSram1End - pharosPartition57Sram1Start) / 8;
  pharosPartition58DataSram1Size = (pharosPartition58DataSram1End - pharosPartition58Sram1Start) / 8;
  pharosPartition59DataSram1Size = (pharosPartition59DataSram1End - pharosPartition59Sram1Start) / 8;
  pharosPartition60DataSram1Size = (pharosPartition60DataSram1End - pharosPartition60Sram1Start) / 8;
  pharosPartition61DataSram1Size = (pharosPartition61DataSram1End - pharosPartition61Sram1Start) / 8;
  pharosPartition62DataSram1Size = (pharosPartition62DataSram1End - pharosPartition62Sram1Start) / 8;
  pharosPartition63DataSram1Size = (pharosPartition63DataSram1End - pharosPartition63Sram1Start) / 8;

  pharosPartition0DataSram2Size = (pharosPartition0DataSram2End - pharosPartition0Sram2Start) / 8;
  pharosPartition1DataSram2Size = (pharosPartition1DataSram2End - pharosPartition1Sram2Start) / 8;
  pharosPartition2DataSram2Size = (pharosPartition2DataSram2End - pharosPartition2Sram2Start) / 8;
  pharosPartition3DataSram2Size = (pharosPartition3DataSram2End - pharosPartition3Sram2Start) / 8;
  pharosPartition4DataSram2Size = (pharosPartition4DataSram2End - pharosPartition4Sram2Start) / 8;
  pharosPartition5DataSram2Size = (pharosPartition5DataSram2End - pharosPartition5Sram2Start) / 8;
  pharosPartition6DataSram2Size = (pharosPartition6DataSram2End - pharosPartition6Sram2Start) / 8;
  pharosPartition7DataSram2Size = (pharosPartition7DataSram2End - pharosPartition7Sram2Start) / 8;
  pharosPartition8DataSram2Size = (pharosPartition8DataSram2End - pharosPartition8Sram2Start) / 8;
  pharosPartition9DataSram2Size = (pharosPartition9DataSram2End - pharosPartition9Sram2Start) / 8;
  pharosPartition10DataSram2Size = (pharosPartition10DataSram2End - pharosPartition10Sram2Start) / 8;
  pharosPartition11DataSram2Size = (pharosPartition11DataSram2End - pharosPartition11Sram2Start) / 8;
  pharosPartition12DataSram2Size = (pharosPartition12DataSram2End - pharosPartition12Sram2Start) / 8;
  pharosPartition13DataSram2Size = (pharosPartition13DataSram2End - pharosPartition13Sram2Start) / 8;
  pharosPartition14DataSram2Size = (pharosPartition14DataSram2End - pharosPartition14Sram2Start) / 8;
  pharosPartition15DataSram2Size = (pharosPartition15DataSram2End - pharosPartition15Sram2Start) / 8;
  pharosPartition16DataSram2Size = (pharosPartition16DataSram2End - pharosPartition16Sram2Start) / 8;
  pharosPartition17DataSram2Size = (pharosPartition17DataSram2End - pharosPartition17Sram2Start) / 8;
  pharosPartition18DataSram2Size = (pharosPartition18DataSram2End - pharosPartition18Sram2Start) / 8;
  pharosPartition19DataSram2Size = (pharosPartition19DataSram2End - pharosPartition19Sram2Start) / 8;
  pharosPartition20DataSram2Size = (pharosPartition20DataSram2End - pharosPartition20Sram2Start) / 8;
  pharosPartition21DataSram2Size = (pharosPartition21DataSram2End - pharosPartition21Sram2Start) / 8;
  pharosPartition22DataSram2Size = (pharosPartition22DataSram2End - pharosPartition22Sram2Start) / 8;
  pharosPartition23DataSram2Size = (pharosPartition23DataSram2End - pharosPartition23Sram2Start) / 8;
  pharosPartition24DataSram2Size = (pharosPartition24DataSram2End - pharosPartition24Sram2Start) / 8;
  pharosPartition25DataSram2Size = (pharosPartition25DataSram2End - pharosPartition25Sram2Start) / 8;
  pharosPartition26DataSram2Size = (pharosPartition26DataSram2End - pharosPartition26Sram2Start) / 8;
  pharosPartition27DataSram2Size = (pharosPartition27DataSram2End - pharosPartition27Sram2Start) / 8;
  pharosPartition28DataSram2Size = (pharosPartition28DataSram2End - pharosPartition28Sram2Start) / 8;
  pharosPartition29DataSram2Size = (pharosPartition29DataSram2End - pharosPartition29Sram2Start) / 8;
  pharosPartition30DataSram2Size = (pharosPartition30DataSram2End - pharosPartition30Sram2Start) / 8;
  pharosPartition31DataSram2Size = (pharosPartition31DataSram2End - pharosPartition31Sram2Start) / 8;
  pharosPartition32DataSram2Size = (pharosPartition32DataSram2End - pharosPartition32Sram2Start) / 8;
  pharosPartition33DataSram2Size = (pharosPartition33DataSram2End - pharosPartition33Sram2Start) / 8;
  pharosPartition34DataSram2Size = (pharosPartition34DataSram2End - pharosPartition34Sram2Start) / 8;
  pharosPartition35DataSram2Size = (pharosPartition35DataSram2End - pharosPartition35Sram2Start) / 8;
  pharosPartition36DataSram2Size = (pharosPartition36DataSram2End - pharosPartition36Sram2Start) / 8;
  pharosPartition37DataSram2Size = (pharosPartition37DataSram2End - pharosPartition37Sram2Start) / 8;
  pharosPartition38DataSram2Size = (pharosPartition38DataSram2End - pharosPartition38Sram2Start) / 8;
  pharosPartition39DataSram2Size = (pharosPartition39DataSram2End - pharosPartition39Sram2Start) / 8;
  pharosPartition40DataSram2Size = (pharosPartition40DataSram2End - pharosPartition40Sram2Start) / 8;
  pharosPartition41DataSram2Size = (pharosPartition41DataSram2End - pharosPartition41Sram2Start) / 8;
  pharosPartition42DataSram2Size = (pharosPartition42DataSram2End - pharosPartition42Sram2Start) / 8;
  pharosPartition43DataSram2Size = (pharosPartition43DataSram2End - pharosPartition43Sram2Start) / 8;
  pharosPartition44DataSram2Size = (pharosPartition44DataSram2End - pharosPartition44Sram2Start) / 8;
  pharosPartition45DataSram2Size = (pharosPartition45DataSram2End - pharosPartition45Sram2Start) / 8;
  pharosPartition46DataSram2Size = (pharosPartition46DataSram2End - pharosPartition46Sram2Start) / 8;
  pharosPartition47DataSram2Size = (pharosPartition47DataSram2End - pharosPartition47Sram2Start) / 8;
  pharosPartition48DataSram2Size = (pharosPartition48DataSram2End - pharosPartition48Sram2Start) / 8;
  pharosPartition49DataSram2Size = (pharosPartition49DataSram2End - pharosPartition49Sram2Start) / 8;
  pharosPartition50DataSram2Size = (pharosPartition50DataSram2End - pharosPartition50Sram2Start) / 8;
  pharosPartition51DataSram2Size = (pharosPartition51DataSram2End - pharosPartition51Sram2Start) / 8;
  pharosPartition52DataSram2Size = (pharosPartition52DataSram2End - pharosPartition52Sram2Start) / 8;
  pharosPartition53DataSram2Size = (pharosPartition53DataSram2End - pharosPartition53Sram2Start) / 8;
  pharosPartition54DataSram2Size = (pharosPartition54DataSram2End - pharosPartition54Sram2Start) / 8;
  pharosPartition55DataSram2Size = (pharosPartition55DataSram2End - pharosPartition55Sram2Start) / 8;
  pharosPartition56DataSram2Size = (pharosPartition56DataSram2End - pharosPartition56Sram2Start) / 8;
  pharosPartition57DataSram2Size = (pharosPartition57DataSram2End - pharosPartition57Sram2Start) / 8;
  pharosPartition58DataSram2Size = (pharosPartition58DataSram2End - pharosPartition58Sram2Start) / 8;
  pharosPartition59DataSram2Size = (pharosPartition59DataSram2End - pharosPartition59Sram2Start) / 8;
  pharosPartition60DataSram2Size = (pharosPartition60DataSram2End - pharosPartition60Sram2Start) / 8;
  pharosPartition61DataSram2Size = (pharosPartition61DataSram2End - pharosPartition61Sram2Start) / 8;
  pharosPartition62DataSram2Size = (pharosPartition62DataSram2End - pharosPartition62Sram2Start) / 8;
  pharosPartition63DataSram2Size = (pharosPartition63DataSram2End - pharosPartition63Sram2Start) / 8;

  pharosPartition0DataSram3Size = (pharosPartition0DataSram3End - pharosPartition0Sram3Start) / 8;
  pharosPartition1DataSram3Size = (pharosPartition1DataSram3End - pharosPartition1Sram3Start) / 8;
  pharosPartition2DataSram3Size = (pharosPartition2DataSram3End - pharosPartition2Sram3Start) / 8;
  pharosPartition3DataSram3Size = (pharosPartition3DataSram3End - pharosPartition3Sram3Start) / 8;
  pharosPartition4DataSram3Size = (pharosPartition4DataSram3End - pharosPartition4Sram3Start) / 8;
  pharosPartition5DataSram3Size = (pharosPartition5DataSram3End - pharosPartition5Sram3Start) / 8;
  pharosPartition6DataSram3Size = (pharosPartition6DataSram3End - pharosPartition6Sram3Start) / 8;
  pharosPartition7DataSram3Size = (pharosPartition7DataSram3End - pharosPartition7Sram3Start) / 8;
  pharosPartition8DataSram3Size = (pharosPartition8DataSram3End - pharosPartition8Sram3Start) / 8;
  pharosPartition9DataSram3Size = (pharosPartition9DataSram3End - pharosPartition9Sram3Start) / 8;
  pharosPartition10DataSram3Size = (pharosPartition10DataSram3End - pharosPartition10Sram3Start) / 8;
  pharosPartition11DataSram3Size = (pharosPartition11DataSram3End - pharosPartition11Sram3Start) / 8;
  pharosPartition12DataSram3Size = (pharosPartition12DataSram3End - pharosPartition12Sram3Start) / 8;
  pharosPartition13DataSram3Size = (pharosPartition13DataSram3End - pharosPartition13Sram3Start) / 8;
  pharosPartition14DataSram3Size = (pharosPartition14DataSram3End - pharosPartition14Sram3Start) / 8;
  pharosPartition15DataSram3Size = (pharosPartition15DataSram3End - pharosPartition15Sram3Start) / 8;
  pharosPartition16DataSram3Size = (pharosPartition16DataSram3End - pharosPartition16Sram3Start) / 8;
  pharosPartition17DataSram3Size = (pharosPartition17DataSram3End - pharosPartition17Sram3Start) / 8;
  pharosPartition18DataSram3Size = (pharosPartition18DataSram3End - pharosPartition18Sram3Start) / 8;
  pharosPartition19DataSram3Size = (pharosPartition19DataSram3End - pharosPartition19Sram3Start) / 8;
  pharosPartition20DataSram3Size = (pharosPartition20DataSram3End - pharosPartition20Sram3Start) / 8;
  pharosPartition21DataSram3Size = (pharosPartition21DataSram3End - pharosPartition21Sram3Start) / 8;
  pharosPartition22DataSram3Size = (pharosPartition22DataSram3End - pharosPartition22Sram3Start) / 8;
  pharosPartition23DataSram3Size = (pharosPartition23DataSram3End - pharosPartition23Sram3Start) / 8;
  pharosPartition24DataSram3Size = (pharosPartition24DataSram3End - pharosPartition24Sram3Start) / 8;
  pharosPartition25DataSram3Size = (pharosPartition25DataSram3End - pharosPartition25Sram3Start) / 8;
  pharosPartition26DataSram3Size = (pharosPartition26DataSram3End - pharosPartition26Sram3Start) / 8;
  pharosPartition27DataSram3Size = (pharosPartition27DataSram3End - pharosPartition27Sram3Start) / 8;
  pharosPartition28DataSram3Size = (pharosPartition28DataSram3End - pharosPartition28Sram3Start) / 8;
  pharosPartition29DataSram3Size = (pharosPartition29DataSram3End - pharosPartition29Sram3Start) / 8;
  pharosPartition30DataSram3Size = (pharosPartition30DataSram3End - pharosPartition30Sram3Start) / 8;
  pharosPartition31DataSram3Size = (pharosPartition31DataSram3End - pharosPartition31Sram3Start) / 8;
  pharosPartition32DataSram3Size = (pharosPartition32DataSram3End - pharosPartition32Sram3Start) / 8;
  pharosPartition33DataSram3Size = (pharosPartition33DataSram3End - pharosPartition33Sram3Start) / 8;
  pharosPartition34DataSram3Size = (pharosPartition34DataSram3End - pharosPartition34Sram3Start) / 8;
  pharosPartition35DataSram3Size = (pharosPartition35DataSram3End - pharosPartition35Sram3Start) / 8;
  pharosPartition36DataSram3Size = (pharosPartition36DataSram3End - pharosPartition36Sram3Start) / 8;
  pharosPartition37DataSram3Size = (pharosPartition37DataSram3End - pharosPartition37Sram3Start) / 8;
  pharosPartition38DataSram3Size = (pharosPartition38DataSram3End - pharosPartition38Sram3Start) / 8;
  pharosPartition39DataSram3Size = (pharosPartition39DataSram3End - pharosPartition39Sram3Start) / 8;
  pharosPartition40DataSram3Size = (pharosPartition40DataSram3End - pharosPartition40Sram3Start) / 8;
  pharosPartition41DataSram3Size = (pharosPartition41DataSram3End - pharosPartition41Sram3Start) / 8;
  pharosPartition42DataSram3Size = (pharosPartition42DataSram3End - pharosPartition42Sram3Start) / 8;
  pharosPartition43DataSram3Size = (pharosPartition43DataSram3End - pharosPartition43Sram3Start) / 8;
  pharosPartition44DataSram3Size = (pharosPartition44DataSram3End - pharosPartition44Sram3Start) / 8;
  pharosPartition45DataSram3Size = (pharosPartition45DataSram3End - pharosPartition45Sram3Start) / 8;
  pharosPartition46DataSram3Size = (pharosPartition46DataSram3End - pharosPartition46Sram3Start) / 8;
  pharosPartition47DataSram3Size = (pharosPartition47DataSram3End - pharosPartition47Sram3Start) / 8;
  pharosPartition48DataSram3Size = (pharosPartition48DataSram3End - pharosPartition48Sram3Start) / 8;
  pharosPartition49DataSram3Size = (pharosPartition49DataSram3End - pharosPartition49Sram3Start) / 8;
  pharosPartition50DataSram3Size = (pharosPartition50DataSram3End - pharosPartition50Sram3Start) / 8;
  pharosPartition51DataSram3Size = (pharosPartition51DataSram3End - pharosPartition51Sram3Start) / 8;
  pharosPartition52DataSram3Size = (pharosPartition52DataSram3End - pharosPartition52Sram3Start) / 8;
  pharosPartition53DataSram3Size = (pharosPartition53DataSram3End - pharosPartition53Sram3Start) / 8;
  pharosPartition54DataSram3Size = (pharosPartition54DataSram3End - pharosPartition54Sram3Start) / 8;
  pharosPartition55DataSram3Size = (pharosPartition55DataSram3End - pharosPartition55Sram3Start) / 8;
  pharosPartition56DataSram3Size = (pharosPartition56DataSram3End - pharosPartition56Sram3Start) / 8;
  pharosPartition57DataSram3Size = (pharosPartition57DataSram3End - pharosPartition57Sram3Start) / 8;
  pharosPartition58DataSram3Size = (pharosPartition58DataSram3End - pharosPartition58Sram3Start) / 8;
  pharosPartition59DataSram3Size = (pharosPartition59DataSram3End - pharosPartition59Sram3Start) / 8;
  pharosPartition60DataSram3Size = (pharosPartition60DataSram3End - pharosPartition60Sram3Start) / 8;
  pharosPartition61DataSram3Size = (pharosPartition61DataSram3End - pharosPartition61Sram3Start) / 8;
  pharosPartition62DataSram3Size = (pharosPartition62DataSram3End - pharosPartition62Sram3Start) / 8;
  pharosPartition63DataSram3Size = (pharosPartition63DataSram3End - pharosPartition63Sram3Start) / 8;

  pharosPartition0DataSramBackupSize = (pharosPartition0DataSramBackupEnd - pharosPartition0SramBackupStart) / 8;
  pharosPartition1DataSramBackupSize = (pharosPartition1DataSramBackupEnd - pharosPartition1SramBackupStart) / 8;
  pharosPartition2DataSramBackupSize = (pharosPartition2DataSramBackupEnd - pharosPartition2SramBackupStart) / 8;
  pharosPartition3DataSramBackupSize = (pharosPartition3DataSramBackupEnd - pharosPartition3SramBackupStart) / 8;
  pharosPartition4DataSramBackupSize = (pharosPartition4DataSramBackupEnd - pharosPartition4SramBackupStart) / 8;
  pharosPartition5DataSramBackupSize = (pharosPartition5DataSramBackupEnd - pharosPartition5SramBackupStart) / 8;
  pharosPartition6DataSramBackupSize = (pharosPartition6DataSramBackupEnd - pharosPartition6SramBackupStart) / 8;
  pharosPartition7DataSramBackupSize = (pharosPartition7DataSramBackupEnd - pharosPartition7SramBackupStart) / 8;
  pharosPartition8DataSramBackupSize = (pharosPartition8DataSramBackupEnd - pharosPartition8SramBackupStart) / 8;
  pharosPartition9DataSramBackupSize = (pharosPartition9DataSramBackupEnd - pharosPartition9SramBackupStart) / 8;
  pharosPartition10DataSramBackupSize = (pharosPartition10DataSramBackupEnd - pharosPartition10SramBackupStart) / 8;
  pharosPartition11DataSramBackupSize = (pharosPartition11DataSramBackupEnd - pharosPartition11SramBackupStart) / 8;
  pharosPartition12DataSramBackupSize = (pharosPartition12DataSramBackupEnd - pharosPartition12SramBackupStart) / 8;
  pharosPartition13DataSramBackupSize = (pharosPartition13DataSramBackupEnd - pharosPartition13SramBackupStart) / 8;
  pharosPartition14DataSramBackupSize = (pharosPartition14DataSramBackupEnd - pharosPartition14SramBackupStart) / 8;
  pharosPartition15DataSramBackupSize = (pharosPartition15DataSramBackupEnd - pharosPartition15SramBackupStart) / 8;
  pharosPartition16DataSramBackupSize = (pharosPartition16DataSramBackupEnd - pharosPartition16SramBackupStart) / 8;
  pharosPartition17DataSramBackupSize = (pharosPartition17DataSramBackupEnd - pharosPartition17SramBackupStart) / 8;
  pharosPartition18DataSramBackupSize = (pharosPartition18DataSramBackupEnd - pharosPartition18SramBackupStart) / 8;
  pharosPartition19DataSramBackupSize = (pharosPartition19DataSramBackupEnd - pharosPartition19SramBackupStart) / 8;
  pharosPartition20DataSramBackupSize = (pharosPartition20DataSramBackupEnd - pharosPartition20SramBackupStart) / 8;
  pharosPartition21DataSramBackupSize = (pharosPartition21DataSramBackupEnd - pharosPartition21SramBackupStart) / 8;
  pharosPartition22DataSramBackupSize = (pharosPartition22DataSramBackupEnd - pharosPartition22SramBackupStart) / 8;
  pharosPartition23DataSramBackupSize = (pharosPartition23DataSramBackupEnd - pharosPartition23SramBackupStart) / 8;
  pharosPartition24DataSramBackupSize = (pharosPartition24DataSramBackupEnd - pharosPartition24SramBackupStart) / 8;
  pharosPartition25DataSramBackupSize = (pharosPartition25DataSramBackupEnd - pharosPartition25SramBackupStart) / 8;
  pharosPartition26DataSramBackupSize = (pharosPartition26DataSramBackupEnd - pharosPartition26SramBackupStart) / 8;
  pharosPartition27DataSramBackupSize = (pharosPartition27DataSramBackupEnd - pharosPartition27SramBackupStart) / 8;
  pharosPartition28DataSramBackupSize = (pharosPartition28DataSramBackupEnd - pharosPartition28SramBackupStart) / 8;
  pharosPartition29DataSramBackupSize = (pharosPartition29DataSramBackupEnd - pharosPartition29SramBackupStart) / 8;
  pharosPartition30DataSramBackupSize = (pharosPartition30DataSramBackupEnd - pharosPartition30SramBackupStart) / 8;
  pharosPartition31DataSramBackupSize = (pharosPartition31DataSramBackupEnd - pharosPartition31SramBackupStart) / 8;
  pharosPartition32DataSramBackupSize = (pharosPartition32DataSramBackupEnd - pharosPartition32SramBackupStart) / 8;
  pharosPartition33DataSramBackupSize = (pharosPartition33DataSramBackupEnd - pharosPartition33SramBackupStart) / 8;
  pharosPartition34DataSramBackupSize = (pharosPartition34DataSramBackupEnd - pharosPartition34SramBackupStart) / 8;
  pharosPartition35DataSramBackupSize = (pharosPartition35DataSramBackupEnd - pharosPartition35SramBackupStart) / 8;
  pharosPartition36DataSramBackupSize = (pharosPartition36DataSramBackupEnd - pharosPartition36SramBackupStart) / 8;
  pharosPartition37DataSramBackupSize = (pharosPartition37DataSramBackupEnd - pharosPartition37SramBackupStart) / 8;
  pharosPartition38DataSramBackupSize = (pharosPartition38DataSramBackupEnd - pharosPartition38SramBackupStart) / 8;
  pharosPartition39DataSramBackupSize = (pharosPartition39DataSramBackupEnd - pharosPartition39SramBackupStart) / 8;
  pharosPartition40DataSramBackupSize = (pharosPartition40DataSramBackupEnd - pharosPartition40SramBackupStart) / 8;
  pharosPartition41DataSramBackupSize = (pharosPartition41DataSramBackupEnd - pharosPartition41SramBackupStart) / 8;
  pharosPartition42DataSramBackupSize = (pharosPartition42DataSramBackupEnd - pharosPartition42SramBackupStart) / 8;
  pharosPartition43DataSramBackupSize = (pharosPartition43DataSramBackupEnd - pharosPartition43SramBackupStart) / 8;
  pharosPartition44DataSramBackupSize = (pharosPartition44DataSramBackupEnd - pharosPartition44SramBackupStart) / 8;
  pharosPartition45DataSramBackupSize = (pharosPartition45DataSramBackupEnd - pharosPartition45SramBackupStart) / 8;
  pharosPartition46DataSramBackupSize = (pharosPartition46DataSramBackupEnd - pharosPartition46SramBackupStart) / 8;
  pharosPartition47DataSramBackupSize = (pharosPartition47DataSramBackupEnd - pharosPartition47SramBackupStart) / 8;
  pharosPartition48DataSramBackupSize = (pharosPartition48DataSramBackupEnd - pharosPartition48SramBackupStart) / 8;
  pharosPartition49DataSramBackupSize = (pharosPartition49DataSramBackupEnd - pharosPartition49SramBackupStart) / 8;
  pharosPartition50DataSramBackupSize = (pharosPartition50DataSramBackupEnd - pharosPartition50SramBackupStart) / 8;
  pharosPartition51DataSramBackupSize = (pharosPartition51DataSramBackupEnd - pharosPartition51SramBackupStart) / 8;
  pharosPartition52DataSramBackupSize = (pharosPartition52DataSramBackupEnd - pharosPartition52SramBackupStart) / 8;
  pharosPartition53DataSramBackupSize = (pharosPartition53DataSramBackupEnd - pharosPartition53SramBackupStart) / 8;
  pharosPartition54DataSramBackupSize = (pharosPartition54DataSramBackupEnd - pharosPartition54SramBackupStart) / 8;
  pharosPartition55DataSramBackupSize = (pharosPartition55DataSramBackupEnd - pharosPartition55SramBackupStart) / 8;
  pharosPartition56DataSramBackupSize = (pharosPartition56DataSramBackupEnd - pharosPartition56SramBackupStart) / 8;
  pharosPartition57DataSramBackupSize = (pharosPartition57DataSramBackupEnd - pharosPartition57SramBackupStart) / 8;
  pharosPartition58DataSramBackupSize = (pharosPartition58DataSramBackupEnd - pharosPartition58SramBackupStart) / 8;
  pharosPartition59DataSramBackupSize = (pharosPartition59DataSramBackupEnd - pharosPartition59SramBackupStart) / 8;
  pharosPartition60DataSramBackupSize = (pharosPartition60DataSramBackupEnd - pharosPartition60SramBackupStart) / 8;
  pharosPartition61DataSramBackupSize = (pharosPartition61DataSramBackupEnd - pharosPartition61SramBackupStart) / 8;
  pharosPartition62DataSramBackupSize = (pharosPartition62DataSramBackupEnd - pharosPartition62SramBackupStart) / 8;
  pharosPartition63DataSramBackupSize = (pharosPartition63DataSramBackupEnd - pharosPartition63SramBackupStart) / 8;

  pharosPartition0DataSdramSize = (pharosPartition0DataSdramEnd - pharosPartition0SdramStart) / 8;
  pharosPartition1DataSdramSize = (pharosPartition1DataSdramEnd - pharosPartition1SdramStart) / 8;
  pharosPartition2DataSdramSize = (pharosPartition2DataSdramEnd - pharosPartition2SdramStart) / 8;
  pharosPartition3DataSdramSize = (pharosPartition3DataSdramEnd - pharosPartition3SdramStart) / 8;
  pharosPartition4DataSdramSize = (pharosPartition4DataSdramEnd - pharosPartition4SdramStart) / 8;
  pharosPartition5DataSdramSize = (pharosPartition5DataSdramEnd - pharosPartition5SdramStart) / 8;
  pharosPartition6DataSdramSize = (pharosPartition6DataSdramEnd - pharosPartition6SdramStart) / 8;
  pharosPartition7DataSdramSize = (pharosPartition7DataSdramEnd - pharosPartition7SdramStart) / 8;
  pharosPartition8DataSdramSize = (pharosPartition8DataSdramEnd - pharosPartition8SdramStart) / 8;
  pharosPartition9DataSdramSize = (pharosPartition9DataSdramEnd - pharosPartition9SdramStart) / 8;
  pharosPartition10DataSdramSize = (pharosPartition10DataSdramEnd - pharosPartition10SdramStart) / 8;
  pharosPartition11DataSdramSize = (pharosPartition11DataSdramEnd - pharosPartition11SdramStart) / 8;
  pharosPartition12DataSdramSize = (pharosPartition12DataSdramEnd - pharosPartition12SdramStart) / 8;
  pharosPartition13DataSdramSize = (pharosPartition13DataSdramEnd - pharosPartition13SdramStart) / 8;
  pharosPartition14DataSdramSize = (pharosPartition14DataSdramEnd - pharosPartition14SdramStart) / 8;
  pharosPartition15DataSdramSize = (pharosPartition15DataSdramEnd - pharosPartition15SdramStart) / 8;
  pharosPartition16DataSdramSize = (pharosPartition16DataSdramEnd - pharosPartition16SdramStart) / 8;
  pharosPartition17DataSdramSize = (pharosPartition17DataSdramEnd - pharosPartition17SdramStart) / 8;
  pharosPartition18DataSdramSize = (pharosPartition18DataSdramEnd - pharosPartition18SdramStart) / 8;
  pharosPartition19DataSdramSize = (pharosPartition19DataSdramEnd - pharosPartition19SdramStart) / 8;
  pharosPartition20DataSdramSize = (pharosPartition20DataSdramEnd - pharosPartition20SdramStart) / 8;
  pharosPartition21DataSdramSize = (pharosPartition21DataSdramEnd - pharosPartition21SdramStart) / 8;
  pharosPartition22DataSdramSize = (pharosPartition22DataSdramEnd - pharosPartition22SdramStart) / 8;
  pharosPartition23DataSdramSize = (pharosPartition23DataSdramEnd - pharosPartition23SdramStart) / 8;
  pharosPartition24DataSdramSize = (pharosPartition24DataSdramEnd - pharosPartition24SdramStart) / 8;
  pharosPartition25DataSdramSize = (pharosPartition25DataSdramEnd - pharosPartition25SdramStart) / 8;
  pharosPartition26DataSdramSize = (pharosPartition26DataSdramEnd - pharosPartition26SdramStart) / 8;
  pharosPartition27DataSdramSize = (pharosPartition27DataSdramEnd - pharosPartition27SdramStart) / 8;
  pharosPartition28DataSdramSize = (pharosPartition28DataSdramEnd - pharosPartition28SdramStart) / 8;
  pharosPartition29DataSdramSize = (pharosPartition29DataSdramEnd - pharosPartition29SdramStart) / 8;
  pharosPartition30DataSdramSize = (pharosPartition30DataSdramEnd - pharosPartition30SdramStart) / 8;
  pharosPartition31DataSdramSize = (pharosPartition31DataSdramEnd - pharosPartition31SdramStart) / 8;
  pharosPartition32DataSdramSize = (pharosPartition32DataSdramEnd - pharosPartition32SdramStart) / 8;
  pharosPartition33DataSdramSize = (pharosPartition33DataSdramEnd - pharosPartition33SdramStart) / 8;
  pharosPartition34DataSdramSize = (pharosPartition34DataSdramEnd - pharosPartition34SdramStart) / 8;
  pharosPartition35DataSdramSize = (pharosPartition35DataSdramEnd - pharosPartition35SdramStart) / 8;
  pharosPartition36DataSdramSize = (pharosPartition36DataSdramEnd - pharosPartition36SdramStart) / 8;
  pharosPartition37DataSdramSize = (pharosPartition37DataSdramEnd - pharosPartition37SdramStart) / 8;
  pharosPartition38DataSdramSize = (pharosPartition38DataSdramEnd - pharosPartition38SdramStart) / 8;
  pharosPartition39DataSdramSize = (pharosPartition39DataSdramEnd - pharosPartition39SdramStart) / 8;
  pharosPartition40DataSdramSize = (pharosPartition40DataSdramEnd - pharosPartition40SdramStart) / 8;
  pharosPartition41DataSdramSize = (pharosPartition41DataSdramEnd - pharosPartition41SdramStart) / 8;
  pharosPartition42DataSdramSize = (pharosPartition42DataSdramEnd - pharosPartition42SdramStart) / 8;
  pharosPartition43DataSdramSize = (pharosPartition43DataSdramEnd - pharosPartition43SdramStart) / 8;
  pharosPartition44DataSdramSize = (pharosPartition44DataSdramEnd - pharosPartition44SdramStart) / 8;
  pharosPartition45DataSdramSize = (pharosPartition45DataSdramEnd - pharosPartition45SdramStart) / 8;
  pharosPartition46DataSdramSize = (pharosPartition46DataSdramEnd - pharosPartition46SdramStart) / 8;
  pharosPartition47DataSdramSize = (pharosPartition47DataSdramEnd - pharosPartition47SdramStart) / 8;
  pharosPartition48DataSdramSize = (pharosPartition48DataSdramEnd - pharosPartition48SdramStart) / 8;
  pharosPartition49DataSdramSize = (pharosPartition49DataSdramEnd - pharosPartition49SdramStart) / 8;
  pharosPartition50DataSdramSize = (pharosPartition50DataSdramEnd - pharosPartition50SdramStart) / 8;
  pharosPartition51DataSdramSize = (pharosPartition51DataSdramEnd - pharosPartition51SdramStart) / 8;
  pharosPartition52DataSdramSize = (pharosPartition52DataSdramEnd - pharosPartition52SdramStart) / 8;
  pharosPartition53DataSdramSize = (pharosPartition53DataSdramEnd - pharosPartition53SdramStart) / 8;
  pharosPartition54DataSdramSize = (pharosPartition54DataSdramEnd - pharosPartition54SdramStart) / 8;
  pharosPartition55DataSdramSize = (pharosPartition55DataSdramEnd - pharosPartition55SdramStart) / 8;
  pharosPartition56DataSdramSize = (pharosPartition56DataSdramEnd - pharosPartition56SdramStart) / 8;
  pharosPartition57DataSdramSize = (pharosPartition57DataSdramEnd - pharosPartition57SdramStart) / 8;
  pharosPartition58DataSdramSize = (pharosPartition58DataSdramEnd - pharosPartition58SdramStart) / 8;
  pharosPartition59DataSdramSize = (pharosPartition59DataSdramEnd - pharosPartition59SdramStart) / 8;
  pharosPartition60DataSdramSize = (pharosPartition60DataSdramEnd - pharosPartition60SdramStart) / 8;
  pharosPartition61DataSdramSize = (pharosPartition61DataSdramEnd - pharosPartition61SdramStart) / 8;
  pharosPartition62DataSdramSize = (pharosPartition62DataSdramEnd - pharosPartition62SdramStart) / 8;
  pharosPartition63DataSdramSize = (pharosPartition63DataSdramEnd - pharosPartition63SdramStart) / 8;

  /* determine the size of the bss in 64 bit variables */
  pharosPartition0BssCcmSize = (pharosPartition0BssCcmEnd - pharosPartition0BssCcmStart) / 8;
  pharosPartition1BssCcmSize = (pharosPartition1BssCcmEnd - pharosPartition1BssCcmStart) / 8;
  pharosPartition2BssCcmSize = (pharosPartition2BssCcmEnd - pharosPartition2BssCcmStart) / 8;
  pharosPartition3BssCcmSize = (pharosPartition3BssCcmEnd - pharosPartition3BssCcmStart) / 8;
  pharosPartition4BssCcmSize = (pharosPartition4BssCcmEnd - pharosPartition4BssCcmStart) / 8;
  pharosPartition5BssCcmSize = (pharosPartition5BssCcmEnd - pharosPartition5BssCcmStart) / 8;
  pharosPartition6BssCcmSize = (pharosPartition6BssCcmEnd - pharosPartition6BssCcmStart) / 8;
  pharosPartition7BssCcmSize = (pharosPartition7BssCcmEnd - pharosPartition7BssCcmStart) / 8;
  pharosPartition8BssCcmSize = (pharosPartition8BssCcmEnd - pharosPartition8BssCcmStart) / 8;
  pharosPartition9BssCcmSize = (pharosPartition9BssCcmEnd - pharosPartition9BssCcmStart) / 8;
  pharosPartition10BssCcmSize = (pharosPartition10BssCcmEnd - pharosPartition10BssCcmStart) / 8;
  pharosPartition11BssCcmSize = (pharosPartition11BssCcmEnd - pharosPartition11BssCcmStart) / 8;
  pharosPartition12BssCcmSize = (pharosPartition12BssCcmEnd - pharosPartition12BssCcmStart) / 8;
  pharosPartition13BssCcmSize = (pharosPartition13BssCcmEnd - pharosPartition13BssCcmStart) / 8;
  pharosPartition14BssCcmSize = (pharosPartition14BssCcmEnd - pharosPartition14BssCcmStart) / 8;
  pharosPartition15BssCcmSize = (pharosPartition15BssCcmEnd - pharosPartition15BssCcmStart) / 8;
  pharosPartition16BssCcmSize = (pharosPartition16BssCcmEnd - pharosPartition16BssCcmStart) / 8;
  pharosPartition17BssCcmSize = (pharosPartition17BssCcmEnd - pharosPartition17BssCcmStart) / 8;
  pharosPartition18BssCcmSize = (pharosPartition18BssCcmEnd - pharosPartition18BssCcmStart) / 8;
  pharosPartition19BssCcmSize = (pharosPartition19BssCcmEnd - pharosPartition19BssCcmStart) / 8;
  pharosPartition20BssCcmSize = (pharosPartition20BssCcmEnd - pharosPartition20BssCcmStart) / 8;
  pharosPartition21BssCcmSize = (pharosPartition21BssCcmEnd - pharosPartition21BssCcmStart) / 8;
  pharosPartition22BssCcmSize = (pharosPartition22BssCcmEnd - pharosPartition22BssCcmStart) / 8;
  pharosPartition23BssCcmSize = (pharosPartition23BssCcmEnd - pharosPartition23BssCcmStart) / 8;
  pharosPartition24BssCcmSize = (pharosPartition24BssCcmEnd - pharosPartition24BssCcmStart) / 8;
  pharosPartition25BssCcmSize = (pharosPartition25BssCcmEnd - pharosPartition25BssCcmStart) / 8;
  pharosPartition26BssCcmSize = (pharosPartition26BssCcmEnd - pharosPartition26BssCcmStart) / 8;
  pharosPartition27BssCcmSize = (pharosPartition27BssCcmEnd - pharosPartition27BssCcmStart) / 8;
  pharosPartition28BssCcmSize = (pharosPartition28BssCcmEnd - pharosPartition28BssCcmStart) / 8;
  pharosPartition29BssCcmSize = (pharosPartition29BssCcmEnd - pharosPartition29BssCcmStart) / 8;
  pharosPartition30BssCcmSize = (pharosPartition30BssCcmEnd - pharosPartition30BssCcmStart) / 8;
  pharosPartition31BssCcmSize = (pharosPartition31BssCcmEnd - pharosPartition31BssCcmStart) / 8;
  pharosPartition32BssCcmSize = (pharosPartition32BssCcmEnd - pharosPartition32BssCcmStart) / 8;
  pharosPartition33BssCcmSize = (pharosPartition33BssCcmEnd - pharosPartition33BssCcmStart) / 8;
  pharosPartition34BssCcmSize = (pharosPartition34BssCcmEnd - pharosPartition34BssCcmStart) / 8;
  pharosPartition35BssCcmSize = (pharosPartition35BssCcmEnd - pharosPartition35BssCcmStart) / 8;
  pharosPartition36BssCcmSize = (pharosPartition36BssCcmEnd - pharosPartition36BssCcmStart) / 8;
  pharosPartition37BssCcmSize = (pharosPartition37BssCcmEnd - pharosPartition37BssCcmStart) / 8;
  pharosPartition38BssCcmSize = (pharosPartition38BssCcmEnd - pharosPartition38BssCcmStart) / 8;
  pharosPartition39BssCcmSize = (pharosPartition39BssCcmEnd - pharosPartition39BssCcmStart) / 8;
  pharosPartition40BssCcmSize = (pharosPartition40BssCcmEnd - pharosPartition40BssCcmStart) / 8;
  pharosPartition41BssCcmSize = (pharosPartition41BssCcmEnd - pharosPartition41BssCcmStart) / 8;
  pharosPartition42BssCcmSize = (pharosPartition42BssCcmEnd - pharosPartition42BssCcmStart) / 8;
  pharosPartition43BssCcmSize = (pharosPartition43BssCcmEnd - pharosPartition43BssCcmStart) / 8;
  pharosPartition44BssCcmSize = (pharosPartition44BssCcmEnd - pharosPartition44BssCcmStart) / 8;
  pharosPartition45BssCcmSize = (pharosPartition45BssCcmEnd - pharosPartition45BssCcmStart) / 8;
  pharosPartition46BssCcmSize = (pharosPartition46BssCcmEnd - pharosPartition46BssCcmStart) / 8;
  pharosPartition47BssCcmSize = (pharosPartition47BssCcmEnd - pharosPartition47BssCcmStart) / 8;
  pharosPartition48BssCcmSize = (pharosPartition48BssCcmEnd - pharosPartition48BssCcmStart) / 8;
  pharosPartition49BssCcmSize = (pharosPartition49BssCcmEnd - pharosPartition49BssCcmStart) / 8;
  pharosPartition50BssCcmSize = (pharosPartition50BssCcmEnd - pharosPartition50BssCcmStart) / 8;
  pharosPartition51BssCcmSize = (pharosPartition51BssCcmEnd - pharosPartition51BssCcmStart) / 8;
  pharosPartition52BssCcmSize = (pharosPartition52BssCcmEnd - pharosPartition52BssCcmStart) / 8;
  pharosPartition53BssCcmSize = (pharosPartition53BssCcmEnd - pharosPartition53BssCcmStart) / 8;
  pharosPartition54BssCcmSize = (pharosPartition54BssCcmEnd - pharosPartition54BssCcmStart) / 8;
  pharosPartition55BssCcmSize = (pharosPartition55BssCcmEnd - pharosPartition55BssCcmStart) / 8;
  pharosPartition56BssCcmSize = (pharosPartition56BssCcmEnd - pharosPartition56BssCcmStart) / 8;
  pharosPartition57BssCcmSize = (pharosPartition57BssCcmEnd - pharosPartition57BssCcmStart) / 8;
  pharosPartition58BssCcmSize = (pharosPartition58BssCcmEnd - pharosPartition58BssCcmStart) / 8;
  pharosPartition59BssCcmSize = (pharosPartition59BssCcmEnd - pharosPartition59BssCcmStart) / 8;
  pharosPartition60BssCcmSize = (pharosPartition60BssCcmEnd - pharosPartition60BssCcmStart) / 8;
  pharosPartition61BssCcmSize = (pharosPartition61BssCcmEnd - pharosPartition61BssCcmStart) / 8;
  pharosPartition62BssCcmSize = (pharosPartition62BssCcmEnd - pharosPartition62BssCcmStart) / 8;
  pharosPartition63BssCcmSize = (pharosPartition63BssCcmEnd - pharosPartition63BssCcmStart) / 8;

  pharosPartition0BssSram1Size = (pharosPartition0BssSram1End - pharosPartition0BssSram1Start) / 8;
  pharosPartition1BssSram1Size = (pharosPartition1BssSram1End - pharosPartition1BssSram1Start) / 8;
  pharosPartition2BssSram1Size = (pharosPartition2BssSram1End - pharosPartition2BssSram1Start) / 8;
  pharosPartition3BssSram1Size = (pharosPartition3BssSram1End - pharosPartition3BssSram1Start) / 8;
  pharosPartition4BssSram1Size = (pharosPartition4BssSram1End - pharosPartition4BssSram1Start) / 8;
  pharosPartition5BssSram1Size = (pharosPartition5BssSram1End - pharosPartition5BssSram1Start) / 8;
  pharosPartition6BssSram1Size = (pharosPartition6BssSram1End - pharosPartition6BssSram1Start) / 8;
  pharosPartition7BssSram1Size = (pharosPartition7BssSram1End - pharosPartition7BssSram1Start) / 8;
  pharosPartition8BssSram1Size = (pharosPartition8BssSram1End - pharosPartition8BssSram1Start) / 8;
  pharosPartition9BssSram1Size = (pharosPartition9BssSram1End - pharosPartition9BssSram1Start) / 8;
  pharosPartition10BssSram1Size = (pharosPartition10BssSram1End - pharosPartition10BssSram1Start) / 8;
  pharosPartition11BssSram1Size = (pharosPartition11BssSram1End - pharosPartition11BssSram1Start) / 8;
  pharosPartition12BssSram1Size = (pharosPartition12BssSram1End - pharosPartition12BssSram1Start) / 8;
  pharosPartition13BssSram1Size = (pharosPartition13BssSram1End - pharosPartition13BssSram1Start) / 8;
  pharosPartition14BssSram1Size = (pharosPartition14BssSram1End - pharosPartition14BssSram1Start) / 8;
  pharosPartition15BssSram1Size = (pharosPartition15BssSram1End - pharosPartition15BssSram1Start) / 8;
  pharosPartition16BssSram1Size = (pharosPartition16BssSram1End - pharosPartition16BssSram1Start) / 8;
  pharosPartition17BssSram1Size = (pharosPartition17BssSram1End - pharosPartition17BssSram1Start) / 8;
  pharosPartition18BssSram1Size = (pharosPartition18BssSram1End - pharosPartition18BssSram1Start) / 8;
  pharosPartition19BssSram1Size = (pharosPartition19BssSram1End - pharosPartition19BssSram1Start) / 8;
  pharosPartition20BssSram1Size = (pharosPartition20BssSram1End - pharosPartition20BssSram1Start) / 8;
  pharosPartition21BssSram1Size = (pharosPartition21BssSram1End - pharosPartition21BssSram1Start) / 8;
  pharosPartition22BssSram1Size = (pharosPartition22BssSram1End - pharosPartition22BssSram1Start) / 8;
  pharosPartition23BssSram1Size = (pharosPartition23BssSram1End - pharosPartition23BssSram1Start) / 8;
  pharosPartition24BssSram1Size = (pharosPartition24BssSram1End - pharosPartition24BssSram1Start) / 8;
  pharosPartition25BssSram1Size = (pharosPartition25BssSram1End - pharosPartition25BssSram1Start) / 8;
  pharosPartition26BssSram1Size = (pharosPartition26BssSram1End - pharosPartition26BssSram1Start) / 8;
  pharosPartition27BssSram1Size = (pharosPartition27BssSram1End - pharosPartition27BssSram1Start) / 8;
  pharosPartition28BssSram1Size = (pharosPartition28BssSram1End - pharosPartition28BssSram1Start) / 8;
  pharosPartition29BssSram1Size = (pharosPartition29BssSram1End - pharosPartition29BssSram1Start) / 8;
  pharosPartition30BssSram1Size = (pharosPartition30BssSram1End - pharosPartition30BssSram1Start) / 8;
  pharosPartition31BssSram1Size = (pharosPartition31BssSram1End - pharosPartition31BssSram1Start) / 8;
  pharosPartition32BssSram1Size = (pharosPartition32BssSram1End - pharosPartition32BssSram1Start) / 8;
  pharosPartition33BssSram1Size = (pharosPartition33BssSram1End - pharosPartition33BssSram1Start) / 8;
  pharosPartition34BssSram1Size = (pharosPartition34BssSram1End - pharosPartition34BssSram1Start) / 8;
  pharosPartition35BssSram1Size = (pharosPartition35BssSram1End - pharosPartition35BssSram1Start) / 8;
  pharosPartition36BssSram1Size = (pharosPartition36BssSram1End - pharosPartition36BssSram1Start) / 8;
  pharosPartition37BssSram1Size = (pharosPartition37BssSram1End - pharosPartition37BssSram1Start) / 8;
  pharosPartition38BssSram1Size = (pharosPartition38BssSram1End - pharosPartition38BssSram1Start) / 8;
  pharosPartition39BssSram1Size = (pharosPartition39BssSram1End - pharosPartition39BssSram1Start) / 8;
  pharosPartition40BssSram1Size = (pharosPartition40BssSram1End - pharosPartition40BssSram1Start) / 8;
  pharosPartition41BssSram1Size = (pharosPartition41BssSram1End - pharosPartition41BssSram1Start) / 8;
  pharosPartition42BssSram1Size = (pharosPartition42BssSram1End - pharosPartition42BssSram1Start) / 8;
  pharosPartition43BssSram1Size = (pharosPartition43BssSram1End - pharosPartition43BssSram1Start) / 8;
  pharosPartition44BssSram1Size = (pharosPartition44BssSram1End - pharosPartition44BssSram1Start) / 8;
  pharosPartition45BssSram1Size = (pharosPartition45BssSram1End - pharosPartition45BssSram1Start) / 8;
  pharosPartition46BssSram1Size = (pharosPartition46BssSram1End - pharosPartition46BssSram1Start) / 8;
  pharosPartition47BssSram1Size = (pharosPartition47BssSram1End - pharosPartition47BssSram1Start) / 8;
  pharosPartition48BssSram1Size = (pharosPartition48BssSram1End - pharosPartition48BssSram1Start) / 8;
  pharosPartition49BssSram1Size = (pharosPartition49BssSram1End - pharosPartition49BssSram1Start) / 8;
  pharosPartition50BssSram1Size = (pharosPartition50BssSram1End - pharosPartition50BssSram1Start) / 8;
  pharosPartition51BssSram1Size = (pharosPartition51BssSram1End - pharosPartition51BssSram1Start) / 8;
  pharosPartition52BssSram1Size = (pharosPartition52BssSram1End - pharosPartition52BssSram1Start) / 8;
  pharosPartition53BssSram1Size = (pharosPartition53BssSram1End - pharosPartition53BssSram1Start) / 8;
  pharosPartition54BssSram1Size = (pharosPartition54BssSram1End - pharosPartition54BssSram1Start) / 8;
  pharosPartition55BssSram1Size = (pharosPartition55BssSram1End - pharosPartition55BssSram1Start) / 8;
  pharosPartition56BssSram1Size = (pharosPartition56BssSram1End - pharosPartition56BssSram1Start) / 8;
  pharosPartition57BssSram1Size = (pharosPartition57BssSram1End - pharosPartition57BssSram1Start) / 8;
  pharosPartition58BssSram1Size = (pharosPartition58BssSram1End - pharosPartition58BssSram1Start) / 8;
  pharosPartition59BssSram1Size = (pharosPartition59BssSram1End - pharosPartition59BssSram1Start) / 8;
  pharosPartition60BssSram1Size = (pharosPartition60BssSram1End - pharosPartition60BssSram1Start) / 8;
  pharosPartition61BssSram1Size = (pharosPartition61BssSram1End - pharosPartition61BssSram1Start) / 8;
  pharosPartition62BssSram1Size = (pharosPartition62BssSram1End - pharosPartition62BssSram1Start) / 8;
  pharosPartition63BssSram1Size = (pharosPartition63BssSram1End - pharosPartition63BssSram1Start) / 8;

  pharosPartition0BssSram2Size = (pharosPartition0BssSram2End - pharosPartition0BssSram2Start) / 8;
  pharosPartition1BssSram2Size = (pharosPartition1BssSram2End - pharosPartition1BssSram2Start) / 8;
  pharosPartition2BssSram2Size = (pharosPartition2BssSram2End - pharosPartition2BssSram2Start) / 8;
  pharosPartition3BssSram2Size = (pharosPartition3BssSram2End - pharosPartition3BssSram2Start) / 8;
  pharosPartition4BssSram2Size = (pharosPartition4BssSram2End - pharosPartition4BssSram2Start) / 8;
  pharosPartition5BssSram2Size = (pharosPartition5BssSram2End - pharosPartition5BssSram2Start) / 8;
  pharosPartition6BssSram2Size = (pharosPartition6BssSram2End - pharosPartition6BssSram2Start) / 8;
  pharosPartition7BssSram2Size = (pharosPartition7BssSram2End - pharosPartition7BssSram2Start) / 8;
  pharosPartition8BssSram2Size = (pharosPartition8BssSram2End - pharosPartition8BssSram2Start) / 8;
  pharosPartition9BssSram2Size = (pharosPartition9BssSram2End - pharosPartition9BssSram2Start) / 8;
  pharosPartition10BssSram2Size = (pharosPartition10BssSram2End - pharosPartition10BssSram2Start) / 8;
  pharosPartition11BssSram2Size = (pharosPartition11BssSram2End - pharosPartition11BssSram2Start) / 8;
  pharosPartition12BssSram2Size = (pharosPartition12BssSram2End - pharosPartition12BssSram2Start) / 8;
  pharosPartition13BssSram2Size = (pharosPartition13BssSram2End - pharosPartition13BssSram2Start) / 8;
  pharosPartition14BssSram2Size = (pharosPartition14BssSram2End - pharosPartition14BssSram2Start) / 8;
  pharosPartition15BssSram2Size = (pharosPartition15BssSram2End - pharosPartition15BssSram2Start) / 8;
  pharosPartition16BssSram2Size = (pharosPartition16BssSram2End - pharosPartition16BssSram2Start) / 8;
  pharosPartition17BssSram2Size = (pharosPartition17BssSram2End - pharosPartition17BssSram2Start) / 8;
  pharosPartition18BssSram2Size = (pharosPartition18BssSram2End - pharosPartition18BssSram2Start) / 8;
  pharosPartition19BssSram2Size = (pharosPartition19BssSram2End - pharosPartition19BssSram2Start) / 8;
  pharosPartition20BssSram2Size = (pharosPartition20BssSram2End - pharosPartition20BssSram2Start) / 8;
  pharosPartition21BssSram2Size = (pharosPartition21BssSram2End - pharosPartition21BssSram2Start) / 8;
  pharosPartition22BssSram2Size = (pharosPartition22BssSram2End - pharosPartition22BssSram2Start) / 8;
  pharosPartition23BssSram2Size = (pharosPartition23BssSram2End - pharosPartition23BssSram2Start) / 8;
  pharosPartition24BssSram2Size = (pharosPartition24BssSram2End - pharosPartition24BssSram2Start) / 8;
  pharosPartition25BssSram2Size = (pharosPartition25BssSram2End - pharosPartition25BssSram2Start) / 8;
  pharosPartition26BssSram2Size = (pharosPartition26BssSram2End - pharosPartition26BssSram2Start) / 8;
  pharosPartition27BssSram2Size = (pharosPartition27BssSram2End - pharosPartition27BssSram2Start) / 8;
  pharosPartition28BssSram2Size = (pharosPartition28BssSram2End - pharosPartition28BssSram2Start) / 8;
  pharosPartition29BssSram2Size = (pharosPartition29BssSram2End - pharosPartition29BssSram2Start) / 8;
  pharosPartition30BssSram2Size = (pharosPartition30BssSram2End - pharosPartition30BssSram2Start) / 8;
  pharosPartition31BssSram2Size = (pharosPartition31BssSram2End - pharosPartition31BssSram2Start) / 8;
  pharosPartition32BssSram2Size = (pharosPartition32BssSram2End - pharosPartition32BssSram2Start) / 8;
  pharosPartition33BssSram2Size = (pharosPartition33BssSram2End - pharosPartition33BssSram2Start) / 8;
  pharosPartition34BssSram2Size = (pharosPartition34BssSram2End - pharosPartition34BssSram2Start) / 8;
  pharosPartition35BssSram2Size = (pharosPartition35BssSram2End - pharosPartition35BssSram2Start) / 8;
  pharosPartition36BssSram2Size = (pharosPartition36BssSram2End - pharosPartition36BssSram2Start) / 8;
  pharosPartition37BssSram2Size = (pharosPartition37BssSram2End - pharosPartition37BssSram2Start) / 8;
  pharosPartition38BssSram2Size = (pharosPartition38BssSram2End - pharosPartition38BssSram2Start) / 8;
  pharosPartition39BssSram2Size = (pharosPartition39BssSram2End - pharosPartition39BssSram2Start) / 8;
  pharosPartition40BssSram2Size = (pharosPartition40BssSram2End - pharosPartition40BssSram2Start) / 8;
  pharosPartition41BssSram2Size = (pharosPartition41BssSram2End - pharosPartition41BssSram2Start) / 8;
  pharosPartition42BssSram2Size = (pharosPartition42BssSram2End - pharosPartition42BssSram2Start) / 8;
  pharosPartition43BssSram2Size = (pharosPartition43BssSram2End - pharosPartition43BssSram2Start) / 8;
  pharosPartition44BssSram2Size = (pharosPartition44BssSram2End - pharosPartition44BssSram2Start) / 8;
  pharosPartition45BssSram2Size = (pharosPartition45BssSram2End - pharosPartition45BssSram2Start) / 8;
  pharosPartition46BssSram2Size = (pharosPartition46BssSram2End - pharosPartition46BssSram2Start) / 8;
  pharosPartition47BssSram2Size = (pharosPartition47BssSram2End - pharosPartition47BssSram2Start) / 8;
  pharosPartition48BssSram2Size = (pharosPartition48BssSram2End - pharosPartition48BssSram2Start) / 8;
  pharosPartition49BssSram2Size = (pharosPartition49BssSram2End - pharosPartition49BssSram2Start) / 8;
  pharosPartition50BssSram2Size = (pharosPartition50BssSram2End - pharosPartition50BssSram2Start) / 8;
  pharosPartition51BssSram2Size = (pharosPartition51BssSram2End - pharosPartition51BssSram2Start) / 8;
  pharosPartition52BssSram2Size = (pharosPartition52BssSram2End - pharosPartition52BssSram2Start) / 8;
  pharosPartition53BssSram2Size = (pharosPartition53BssSram2End - pharosPartition53BssSram2Start) / 8;
  pharosPartition54BssSram2Size = (pharosPartition54BssSram2End - pharosPartition54BssSram2Start) / 8;
  pharosPartition55BssSram2Size = (pharosPartition55BssSram2End - pharosPartition55BssSram2Start) / 8;
  pharosPartition56BssSram2Size = (pharosPartition56BssSram2End - pharosPartition56BssSram2Start) / 8;
  pharosPartition57BssSram2Size = (pharosPartition57BssSram2End - pharosPartition57BssSram2Start) / 8;
  pharosPartition58BssSram2Size = (pharosPartition58BssSram2End - pharosPartition58BssSram2Start) / 8;
  pharosPartition59BssSram2Size = (pharosPartition59BssSram2End - pharosPartition59BssSram2Start) / 8;
  pharosPartition60BssSram2Size = (pharosPartition60BssSram2End - pharosPartition60BssSram2Start) / 8;
  pharosPartition61BssSram2Size = (pharosPartition61BssSram2End - pharosPartition61BssSram2Start) / 8;
  pharosPartition62BssSram2Size = (pharosPartition62BssSram2End - pharosPartition62BssSram2Start) / 8;
  pharosPartition63BssSram2Size = (pharosPartition63BssSram2End - pharosPartition63BssSram2Start) / 8;

  pharosPartition0BssSram3Size = (pharosPartition0BssSram3End - pharosPartition0BssSram3Start) / 8;
  pharosPartition1BssSram3Size = (pharosPartition1BssSram3End - pharosPartition1BssSram3Start) / 8;
  pharosPartition2BssSram3Size = (pharosPartition2BssSram3End - pharosPartition2BssSram3Start) / 8;
  pharosPartition3BssSram3Size = (pharosPartition3BssSram3End - pharosPartition3BssSram3Start) / 8;
  pharosPartition4BssSram3Size = (pharosPartition4BssSram3End - pharosPartition4BssSram3Start) / 8;
  pharosPartition5BssSram3Size = (pharosPartition5BssSram3End - pharosPartition5BssSram3Start) / 8;
  pharosPartition6BssSram3Size = (pharosPartition6BssSram3End - pharosPartition6BssSram3Start) / 8;
  pharosPartition7BssSram3Size = (pharosPartition7BssSram3End - pharosPartition7BssSram3Start) / 8;
  pharosPartition8BssSram3Size = (pharosPartition8BssSram3End - pharosPartition8BssSram3Start) / 8;
  pharosPartition9BssSram3Size = (pharosPartition9BssSram3End - pharosPartition9BssSram3Start) / 8;
  pharosPartition10BssSram3Size = (pharosPartition10BssSram3End - pharosPartition10BssSram3Start) / 8;
  pharosPartition11BssSram3Size = (pharosPartition11BssSram3End - pharosPartition11BssSram3Start) / 8;
  pharosPartition12BssSram3Size = (pharosPartition12BssSram3End - pharosPartition12BssSram3Start) / 8;
  pharosPartition13BssSram3Size = (pharosPartition13BssSram3End - pharosPartition13BssSram3Start) / 8;
  pharosPartition14BssSram3Size = (pharosPartition14BssSram3End - pharosPartition14BssSram3Start) / 8;
  pharosPartition15BssSram3Size = (pharosPartition15BssSram3End - pharosPartition15BssSram3Start) / 8;
  pharosPartition16BssSram3Size = (pharosPartition16BssSram3End - pharosPartition16BssSram3Start) / 8;
  pharosPartition17BssSram3Size = (pharosPartition17BssSram3End - pharosPartition17BssSram3Start) / 8;
  pharosPartition18BssSram3Size = (pharosPartition18BssSram3End - pharosPartition18BssSram3Start) / 8;
  pharosPartition19BssSram3Size = (pharosPartition19BssSram3End - pharosPartition19BssSram3Start) / 8;
  pharosPartition20BssSram3Size = (pharosPartition20BssSram3End - pharosPartition20BssSram3Start) / 8;
  pharosPartition21BssSram3Size = (pharosPartition21BssSram3End - pharosPartition21BssSram3Start) / 8;
  pharosPartition22BssSram3Size = (pharosPartition22BssSram3End - pharosPartition22BssSram3Start) / 8;
  pharosPartition23BssSram3Size = (pharosPartition23BssSram3End - pharosPartition23BssSram3Start) / 8;
  pharosPartition24BssSram3Size = (pharosPartition24BssSram3End - pharosPartition24BssSram3Start) / 8;
  pharosPartition25BssSram3Size = (pharosPartition25BssSram3End - pharosPartition25BssSram3Start) / 8;
  pharosPartition26BssSram3Size = (pharosPartition26BssSram3End - pharosPartition26BssSram3Start) / 8;
  pharosPartition27BssSram3Size = (pharosPartition27BssSram3End - pharosPartition27BssSram3Start) / 8;
  pharosPartition28BssSram3Size = (pharosPartition28BssSram3End - pharosPartition28BssSram3Start) / 8;
  pharosPartition29BssSram3Size = (pharosPartition29BssSram3End - pharosPartition29BssSram3Start) / 8;
  pharosPartition30BssSram3Size = (pharosPartition30BssSram3End - pharosPartition30BssSram3Start) / 8;
  pharosPartition31BssSram3Size = (pharosPartition31BssSram3End - pharosPartition31BssSram3Start) / 8;
  pharosPartition32BssSram3Size = (pharosPartition32BssSram3End - pharosPartition32BssSram3Start) / 8;
  pharosPartition33BssSram3Size = (pharosPartition33BssSram3End - pharosPartition33BssSram3Start) / 8;
  pharosPartition34BssSram3Size = (pharosPartition34BssSram3End - pharosPartition34BssSram3Start) / 8;
  pharosPartition35BssSram3Size = (pharosPartition35BssSram3End - pharosPartition35BssSram3Start) / 8;
  pharosPartition36BssSram3Size = (pharosPartition36BssSram3End - pharosPartition36BssSram3Start) / 8;
  pharosPartition37BssSram3Size = (pharosPartition37BssSram3End - pharosPartition37BssSram3Start) / 8;
  pharosPartition38BssSram3Size = (pharosPartition38BssSram3End - pharosPartition38BssSram3Start) / 8;
  pharosPartition39BssSram3Size = (pharosPartition39BssSram3End - pharosPartition39BssSram3Start) / 8;
  pharosPartition40BssSram3Size = (pharosPartition40BssSram3End - pharosPartition40BssSram3Start) / 8;
  pharosPartition41BssSram3Size = (pharosPartition41BssSram3End - pharosPartition41BssSram3Start) / 8;
  pharosPartition42BssSram3Size = (pharosPartition42BssSram3End - pharosPartition42BssSram3Start) / 8;
  pharosPartition43BssSram3Size = (pharosPartition43BssSram3End - pharosPartition43BssSram3Start) / 8;
  pharosPartition44BssSram3Size = (pharosPartition44BssSram3End - pharosPartition44BssSram3Start) / 8;
  pharosPartition45BssSram3Size = (pharosPartition45BssSram3End - pharosPartition45BssSram3Start) / 8;
  pharosPartition46BssSram3Size = (pharosPartition46BssSram3End - pharosPartition46BssSram3Start) / 8;
  pharosPartition47BssSram3Size = (pharosPartition47BssSram3End - pharosPartition47BssSram3Start) / 8;
  pharosPartition48BssSram3Size = (pharosPartition48BssSram3End - pharosPartition48BssSram3Start) / 8;
  pharosPartition49BssSram3Size = (pharosPartition49BssSram3End - pharosPartition49BssSram3Start) / 8;
  pharosPartition50BssSram3Size = (pharosPartition50BssSram3End - pharosPartition50BssSram3Start) / 8;
  pharosPartition51BssSram3Size = (pharosPartition51BssSram3End - pharosPartition51BssSram3Start) / 8;
  pharosPartition52BssSram3Size = (pharosPartition52BssSram3End - pharosPartition52BssSram3Start) / 8;
  pharosPartition53BssSram3Size = (pharosPartition53BssSram3End - pharosPartition53BssSram3Start) / 8;
  pharosPartition54BssSram3Size = (pharosPartition54BssSram3End - pharosPartition54BssSram3Start) / 8;
  pharosPartition55BssSram3Size = (pharosPartition55BssSram3End - pharosPartition55BssSram3Start) / 8;
  pharosPartition56BssSram3Size = (pharosPartition56BssSram3End - pharosPartition56BssSram3Start) / 8;
  pharosPartition57BssSram3Size = (pharosPartition57BssSram3End - pharosPartition57BssSram3Start) / 8;
  pharosPartition58BssSram3Size = (pharosPartition58BssSram3End - pharosPartition58BssSram3Start) / 8;
  pharosPartition59BssSram3Size = (pharosPartition59BssSram3End - pharosPartition59BssSram3Start) / 8;
  pharosPartition60BssSram3Size = (pharosPartition60BssSram3End - pharosPartition60BssSram3Start) / 8;
  pharosPartition61BssSram3Size = (pharosPartition61BssSram3End - pharosPartition61BssSram3Start) / 8;
  pharosPartition62BssSram3Size = (pharosPartition62BssSram3End - pharosPartition62BssSram3Start) / 8;
  pharosPartition63BssSram3Size = (pharosPartition63BssSram3End - pharosPartition63BssSram3Start) / 8;

  pharosPartition0BssSramBackupSize = (pharosPartition0BssSramBackupEnd - pharosPartition0BssSramBackupStart) / 8;
  pharosPartition1BssSramBackupSize = (pharosPartition1BssSramBackupEnd - pharosPartition1BssSramBackupStart) / 8;
  pharosPartition2BssSramBackupSize = (pharosPartition2BssSramBackupEnd - pharosPartition2BssSramBackupStart) / 8;
  pharosPartition3BssSramBackupSize = (pharosPartition3BssSramBackupEnd - pharosPartition3BssSramBackupStart) / 8;
  pharosPartition4BssSramBackupSize = (pharosPartition4BssSramBackupEnd - pharosPartition4BssSramBackupStart) / 8;
  pharosPartition5BssSramBackupSize = (pharosPartition5BssSramBackupEnd - pharosPartition5BssSramBackupStart) / 8;
  pharosPartition6BssSramBackupSize = (pharosPartition6BssSramBackupEnd - pharosPartition6BssSramBackupStart) / 8;
  pharosPartition7BssSramBackupSize = (pharosPartition7BssSramBackupEnd - pharosPartition7BssSramBackupStart) / 8;
  pharosPartition8BssSramBackupSize = (pharosPartition8BssSramBackupEnd - pharosPartition8BssSramBackupStart) / 8;
  pharosPartition9BssSramBackupSize = (pharosPartition9BssSramBackupEnd - pharosPartition9BssSramBackupStart) / 8;
  pharosPartition10BssSramBackupSize = (pharosPartition10BssSramBackupEnd - pharosPartition10BssSramBackupStart) / 8;
  pharosPartition11BssSramBackupSize = (pharosPartition11BssSramBackupEnd - pharosPartition11BssSramBackupStart) / 8;
  pharosPartition12BssSramBackupSize = (pharosPartition12BssSramBackupEnd - pharosPartition12BssSramBackupStart) / 8;
  pharosPartition13BssSramBackupSize = (pharosPartition13BssSramBackupEnd - pharosPartition13BssSramBackupStart) / 8;
  pharosPartition14BssSramBackupSize = (pharosPartition14BssSramBackupEnd - pharosPartition14BssSramBackupStart) / 8;
  pharosPartition15BssSramBackupSize = (pharosPartition15BssSramBackupEnd - pharosPartition15BssSramBackupStart) / 8;
  pharosPartition16BssSramBackupSize = (pharosPartition16BssSramBackupEnd - pharosPartition16BssSramBackupStart) / 8;
  pharosPartition17BssSramBackupSize = (pharosPartition17BssSramBackupEnd - pharosPartition17BssSramBackupStart) / 8;
  pharosPartition18BssSramBackupSize = (pharosPartition18BssSramBackupEnd - pharosPartition18BssSramBackupStart) / 8;
  pharosPartition19BssSramBackupSize = (pharosPartition19BssSramBackupEnd - pharosPartition19BssSramBackupStart) / 8;
  pharosPartition20BssSramBackupSize = (pharosPartition20BssSramBackupEnd - pharosPartition20BssSramBackupStart) / 8;
  pharosPartition21BssSramBackupSize = (pharosPartition21BssSramBackupEnd - pharosPartition21BssSramBackupStart) / 8;
  pharosPartition22BssSramBackupSize = (pharosPartition22BssSramBackupEnd - pharosPartition22BssSramBackupStart) / 8;
  pharosPartition23BssSramBackupSize = (pharosPartition23BssSramBackupEnd - pharosPartition23BssSramBackupStart) / 8;
  pharosPartition24BssSramBackupSize = (pharosPartition24BssSramBackupEnd - pharosPartition24BssSramBackupStart) / 8;
  pharosPartition25BssSramBackupSize = (pharosPartition25BssSramBackupEnd - pharosPartition25BssSramBackupStart) / 8;
  pharosPartition26BssSramBackupSize = (pharosPartition26BssSramBackupEnd - pharosPartition26BssSramBackupStart) / 8;
  pharosPartition27BssSramBackupSize = (pharosPartition27BssSramBackupEnd - pharosPartition27BssSramBackupStart) / 8;
  pharosPartition28BssSramBackupSize = (pharosPartition28BssSramBackupEnd - pharosPartition28BssSramBackupStart) / 8;
  pharosPartition29BssSramBackupSize = (pharosPartition29BssSramBackupEnd - pharosPartition29BssSramBackupStart) / 8;
  pharosPartition30BssSramBackupSize = (pharosPartition30BssSramBackupEnd - pharosPartition30BssSramBackupStart) / 8;
  pharosPartition31BssSramBackupSize = (pharosPartition31BssSramBackupEnd - pharosPartition31BssSramBackupStart) / 8;
  pharosPartition32BssSramBackupSize = (pharosPartition32BssSramBackupEnd - pharosPartition32BssSramBackupStart) / 8;
  pharosPartition33BssSramBackupSize = (pharosPartition33BssSramBackupEnd - pharosPartition33BssSramBackupStart) / 8;
  pharosPartition34BssSramBackupSize = (pharosPartition34BssSramBackupEnd - pharosPartition34BssSramBackupStart) / 8;
  pharosPartition35BssSramBackupSize = (pharosPartition35BssSramBackupEnd - pharosPartition35BssSramBackupStart) / 8;
  pharosPartition36BssSramBackupSize = (pharosPartition36BssSramBackupEnd - pharosPartition36BssSramBackupStart) / 8;
  pharosPartition37BssSramBackupSize = (pharosPartition37BssSramBackupEnd - pharosPartition37BssSramBackupStart) / 8;
  pharosPartition38BssSramBackupSize = (pharosPartition38BssSramBackupEnd - pharosPartition38BssSramBackupStart) / 8;
  pharosPartition39BssSramBackupSize = (pharosPartition39BssSramBackupEnd - pharosPartition39BssSramBackupStart) / 8;
  pharosPartition40BssSramBackupSize = (pharosPartition40BssSramBackupEnd - pharosPartition40BssSramBackupStart) / 8;
  pharosPartition41BssSramBackupSize = (pharosPartition41BssSramBackupEnd - pharosPartition41BssSramBackupStart) / 8;
  pharosPartition42BssSramBackupSize = (pharosPartition42BssSramBackupEnd - pharosPartition42BssSramBackupStart) / 8;
  pharosPartition43BssSramBackupSize = (pharosPartition43BssSramBackupEnd - pharosPartition43BssSramBackupStart) / 8;
  pharosPartition44BssSramBackupSize = (pharosPartition44BssSramBackupEnd - pharosPartition44BssSramBackupStart) / 8;
  pharosPartition45BssSramBackupSize = (pharosPartition45BssSramBackupEnd - pharosPartition45BssSramBackupStart) / 8;
  pharosPartition46BssSramBackupSize = (pharosPartition46BssSramBackupEnd - pharosPartition46BssSramBackupStart) / 8;
  pharosPartition47BssSramBackupSize = (pharosPartition47BssSramBackupEnd - pharosPartition47BssSramBackupStart) / 8;
  pharosPartition48BssSramBackupSize = (pharosPartition48BssSramBackupEnd - pharosPartition48BssSramBackupStart) / 8;
  pharosPartition49BssSramBackupSize = (pharosPartition49BssSramBackupEnd - pharosPartition49BssSramBackupStart) / 8;
  pharosPartition50BssSramBackupSize = (pharosPartition50BssSramBackupEnd - pharosPartition50BssSramBackupStart) / 8;
  pharosPartition51BssSramBackupSize = (pharosPartition51BssSramBackupEnd - pharosPartition51BssSramBackupStart) / 8;
  pharosPartition52BssSramBackupSize = (pharosPartition52BssSramBackupEnd - pharosPartition52BssSramBackupStart) / 8;
  pharosPartition53BssSramBackupSize = (pharosPartition53BssSramBackupEnd - pharosPartition53BssSramBackupStart) / 8;
  pharosPartition54BssSramBackupSize = (pharosPartition54BssSramBackupEnd - pharosPartition54BssSramBackupStart) / 8;
  pharosPartition55BssSramBackupSize = (pharosPartition55BssSramBackupEnd - pharosPartition55BssSramBackupStart) / 8;
  pharosPartition56BssSramBackupSize = (pharosPartition56BssSramBackupEnd - pharosPartition56BssSramBackupStart) / 8;
  pharosPartition57BssSramBackupSize = (pharosPartition57BssSramBackupEnd - pharosPartition57BssSramBackupStart) / 8;
  pharosPartition58BssSramBackupSize = (pharosPartition58BssSramBackupEnd - pharosPartition58BssSramBackupStart) / 8;
  pharosPartition59BssSramBackupSize = (pharosPartition59BssSramBackupEnd - pharosPartition59BssSramBackupStart) / 8;
  pharosPartition60BssSramBackupSize = (pharosPartition60BssSramBackupEnd - pharosPartition60BssSramBackupStart) / 8;
  pharosPartition61BssSramBackupSize = (pharosPartition61BssSramBackupEnd - pharosPartition61BssSramBackupStart) / 8;
  pharosPartition62BssSramBackupSize = (pharosPartition62BssSramBackupEnd - pharosPartition62BssSramBackupStart) / 8;
  pharosPartition63BssSramBackupSize = (pharosPartition63BssSramBackupEnd - pharosPartition63BssSramBackupStart) / 8;

  pharosPartition0BssSdramSize = (pharosPartition0BssSdramEnd - pharosPartition0BssSdramStart) / 8;
  pharosPartition1BssSdramSize = (pharosPartition1BssSdramEnd - pharosPartition1BssSdramStart) / 8;
  pharosPartition2BssSdramSize = (pharosPartition2BssSdramEnd - pharosPartition2BssSdramStart) / 8;
  pharosPartition3BssSdramSize = (pharosPartition3BssSdramEnd - pharosPartition3BssSdramStart) / 8;
  pharosPartition4BssSdramSize = (pharosPartition4BssSdramEnd - pharosPartition4BssSdramStart) / 8;
  pharosPartition5BssSdramSize = (pharosPartition5BssSdramEnd - pharosPartition5BssSdramStart) / 8;
  pharosPartition6BssSdramSize = (pharosPartition6BssSdramEnd - pharosPartition6BssSdramStart) / 8;
  pharosPartition7BssSdramSize = (pharosPartition7BssSdramEnd - pharosPartition7BssSdramStart) / 8;
  pharosPartition8BssSdramSize = (pharosPartition8BssSdramEnd - pharosPartition8BssSdramStart) / 8;
  pharosPartition9BssSdramSize = (pharosPartition9BssSdramEnd - pharosPartition9BssSdramStart) / 8;
  pharosPartition10BssSdramSize = (pharosPartition10BssSdramEnd - pharosPartition10BssSdramStart) / 8;
  pharosPartition11BssSdramSize = (pharosPartition11BssSdramEnd - pharosPartition11BssSdramStart) / 8;
  pharosPartition12BssSdramSize = (pharosPartition12BssSdramEnd - pharosPartition12BssSdramStart) / 8;
  pharosPartition13BssSdramSize = (pharosPartition13BssSdramEnd - pharosPartition13BssSdramStart) / 8;
  pharosPartition14BssSdramSize = (pharosPartition14BssSdramEnd - pharosPartition14BssSdramStart) / 8;
  pharosPartition15BssSdramSize = (pharosPartition15BssSdramEnd - pharosPartition15BssSdramStart) / 8;
  pharosPartition16BssSdramSize = (pharosPartition16BssSdramEnd - pharosPartition16BssSdramStart) / 8;
  pharosPartition17BssSdramSize = (pharosPartition17BssSdramEnd - pharosPartition17BssSdramStart) / 8;
  pharosPartition18BssSdramSize = (pharosPartition18BssSdramEnd - pharosPartition18BssSdramStart) / 8;
  pharosPartition19BssSdramSize = (pharosPartition19BssSdramEnd - pharosPartition19BssSdramStart) / 8;
  pharosPartition20BssSdramSize = (pharosPartition20BssSdramEnd - pharosPartition20BssSdramStart) / 8;
  pharosPartition21BssSdramSize = (pharosPartition21BssSdramEnd - pharosPartition21BssSdramStart) / 8;
  pharosPartition22BssSdramSize = (pharosPartition22BssSdramEnd - pharosPartition22BssSdramStart) / 8;
  pharosPartition23BssSdramSize = (pharosPartition23BssSdramEnd - pharosPartition23BssSdramStart) / 8;
  pharosPartition24BssSdramSize = (pharosPartition24BssSdramEnd - pharosPartition24BssSdramStart) / 8;
  pharosPartition25BssSdramSize = (pharosPartition25BssSdramEnd - pharosPartition25BssSdramStart) / 8;
  pharosPartition26BssSdramSize = (pharosPartition26BssSdramEnd - pharosPartition26BssSdramStart) / 8;
  pharosPartition27BssSdramSize = (pharosPartition27BssSdramEnd - pharosPartition27BssSdramStart) / 8;
  pharosPartition28BssSdramSize = (pharosPartition28BssSdramEnd - pharosPartition28BssSdramStart) / 8;
  pharosPartition29BssSdramSize = (pharosPartition29BssSdramEnd - pharosPartition29BssSdramStart) / 8;
  pharosPartition30BssSdramSize = (pharosPartition30BssSdramEnd - pharosPartition30BssSdramStart) / 8;
  pharosPartition31BssSdramSize = (pharosPartition31BssSdramEnd - pharosPartition31BssSdramStart) / 8;
  pharosPartition32BssSdramSize = (pharosPartition32BssSdramEnd - pharosPartition32BssSdramStart) / 8;
  pharosPartition33BssSdramSize = (pharosPartition33BssSdramEnd - pharosPartition33BssSdramStart) / 8;
  pharosPartition34BssSdramSize = (pharosPartition34BssSdramEnd - pharosPartition34BssSdramStart) / 8;
  pharosPartition35BssSdramSize = (pharosPartition35BssSdramEnd - pharosPartition35BssSdramStart) / 8;
  pharosPartition36BssSdramSize = (pharosPartition36BssSdramEnd - pharosPartition36BssSdramStart) / 8;
  pharosPartition37BssSdramSize = (pharosPartition37BssSdramEnd - pharosPartition37BssSdramStart) / 8;
  pharosPartition38BssSdramSize = (pharosPartition38BssSdramEnd - pharosPartition38BssSdramStart) / 8;
  pharosPartition39BssSdramSize = (pharosPartition39BssSdramEnd - pharosPartition39BssSdramStart) / 8;
  pharosPartition40BssSdramSize = (pharosPartition40BssSdramEnd - pharosPartition40BssSdramStart) / 8;
  pharosPartition41BssSdramSize = (pharosPartition41BssSdramEnd - pharosPartition41BssSdramStart) / 8;
  pharosPartition42BssSdramSize = (pharosPartition42BssSdramEnd - pharosPartition42BssSdramStart) / 8;
  pharosPartition43BssSdramSize = (pharosPartition43BssSdramEnd - pharosPartition43BssSdramStart) / 8;
  pharosPartition44BssSdramSize = (pharosPartition44BssSdramEnd - pharosPartition44BssSdramStart) / 8;
  pharosPartition45BssSdramSize = (pharosPartition45BssSdramEnd - pharosPartition45BssSdramStart) / 8;
  pharosPartition46BssSdramSize = (pharosPartition46BssSdramEnd - pharosPartition46BssSdramStart) / 8;
  pharosPartition47BssSdramSize = (pharosPartition47BssSdramEnd - pharosPartition47BssSdramStart) / 8;
  pharosPartition48BssSdramSize = (pharosPartition48BssSdramEnd - pharosPartition48BssSdramStart) / 8;
  pharosPartition49BssSdramSize = (pharosPartition49BssSdramEnd - pharosPartition49BssSdramStart) / 8;
  pharosPartition50BssSdramSize = (pharosPartition50BssSdramEnd - pharosPartition50BssSdramStart) / 8;
  pharosPartition51BssSdramSize = (pharosPartition51BssSdramEnd - pharosPartition51BssSdramStart) / 8;
  pharosPartition52BssSdramSize = (pharosPartition52BssSdramEnd - pharosPartition52BssSdramStart) / 8;
  pharosPartition53BssSdramSize = (pharosPartition53BssSdramEnd - pharosPartition53BssSdramStart) / 8;
  pharosPartition54BssSdramSize = (pharosPartition54BssSdramEnd - pharosPartition54BssSdramStart) / 8;
  pharosPartition55BssSdramSize = (pharosPartition55BssSdramEnd - pharosPartition55BssSdramStart) / 8;
  pharosPartition56BssSdramSize = (pharosPartition56BssSdramEnd - pharosPartition56BssSdramStart) / 8;
  pharosPartition57BssSdramSize = (pharosPartition57BssSdramEnd - pharosPartition57BssSdramStart) / 8;
  pharosPartition58BssSdramSize = (pharosPartition58BssSdramEnd - pharosPartition58BssSdramStart) / 8;
  pharosPartition59BssSdramSize = (pharosPartition59BssSdramEnd - pharosPartition59BssSdramStart) / 8;
  pharosPartition60BssSdramSize = (pharosPartition60BssSdramEnd - pharosPartition60BssSdramStart) / 8;
  pharosPartition61BssSdramSize = (pharosPartition61BssSdramEnd - pharosPartition61BssSdramStart) / 8;
  pharosPartition62BssSdramSize = (pharosPartition62BssSdramEnd - pharosPartition62BssSdramStart) / 8;
  pharosPartition63BssSdramSize = (pharosPartition63BssSdramEnd - pharosPartition63BssSdramStart) / 8;



  /******************************************************************/
  /* Partition 0                                                    */
  /******************************************************************/

  /* address in the flash of the partition 0 data to copy it to RAM */
  pharosPartition0DataCcmLoadAddr = LOADADDR(.partition0DataCcm);

  /* address in the flash of the partition 0 data to copy it to RAM */
  pharosPartition0DataSram1LoadAddr = LOADADDR(.partition0DataSram1);

  /* address in the flash of the partition 0 data to copy it to RAM */
  pharosPartition0DataSram2LoadAddr = LOADADDR(.partition0DataSram2);

  /* address in the flash of the partition 0 data to copy it to RAM */
  pharosPartition0DataSram3LoadAddr = LOADADDR(.partition0DataSram3);

  /* address in the flash of the partition 0 data to copy it to RAM */
  pharosPartition0DataSramBackupLoadAddr = LOADADDR(.partition0DataSramBackup);

  /* address in the flash of the partition 0 data to copy it to RAM */
  pharosPartition0DataSdramLoadAddr = LOADADDR(.partition0DataSdram);

  /* data of partition in Ccm */
  .partition0DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition0CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_0_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition0DataCcmStart = ABSOLUTE(.);

    *(.partition0DataCcm)
    *(.partition0DataCcm.*)

    /* by default, the data section will go to partition 0 and to CCM */
    *(.data)
    *(.data.*)
    *(.sdata)
    *(.sdata.*)

    /* address of the partition 0 data section end in RAM */
    . = ALIGN(8);
    pharosPartition0DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 0 in Ccm */
  .pharosPartition0BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 0 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition0BssCcmStart = .;

    *(.partition0BssCcm)
    *(.partition0BssCcm.*)

     /* by default, the bss section will go to partition 0 and to CCM */
     *(.bss)
     *(.bss.*)
     *(.sbss)
     *(.sbss.*)
     *(COMMON)
     *(COMMON.*)
     *(SCOMMON)
     *(SCOMMON.*)

    /* address of the partition 0 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition0BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition0UssCcmStart = .;

    *(.partition0UssCcm)
    *(.partition0UssCcm.*)

    pharosPartition0UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_0_CCM_ALIGNMENT);

    /* address of the partition 0 sections end in Ccm */
    pharosPartition0CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition0DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition0Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_0_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition0DataSram1Start = ABSOLUTE(.);

    *(.partition0DataSram1)
    *(.partition0DataSram1.*)

    /* address of the partition 0 data section end in RAM */
    . = ALIGN(8);
    pharosPartition0DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 0 in Sram1 */
  .pharosPartition0BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 0 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition0BssSram1Start = .;

    *(.partition0BssSram1)
    *(.partition0BssSram1.*)

    /* address of the partition 0 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition0BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition0UssSram1Start = .;

    *(.partition0UssSram1)
    *(.partition0UssSram1.*)

    pharosPartition0UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_0_SRAM1_ALIGNMENT);

    /* address of the partition 0 sections end in Sram1 */
    pharosPartition0Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition0DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition0Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_0_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition0DataSram2Start = ABSOLUTE(.);

    *(.partition0DataSram2)
    *(.partition0DataSram2.*)

    /* address of the partition 0 data section end in RAM */
    . = ALIGN(8);
    pharosPartition0DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 0 in Sram2 */
  .pharosPartition0BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 0 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition0BssSram2Start = .;

    *(.partition0BssSram2)
    *(.partition0BssSram2.*)

    /* address of the partition 0 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition0BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition0UssSram2Start = .;

    *(.partition0UssSram2)
    *(.partition0UssSram2.*)

    pharosPartition0UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_0_SRAM2_ALIGNMENT);

    /* address of the partition 0 sections end in Sram2 */
    pharosPartition0Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition0DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition0Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_0_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition0DataSram3Start = ABSOLUTE(.);

    *(.partition0DataSram3)
    *(.partition0DataSram3.*)

    /* address of the partition 0 data section end in RAM */
    . = ALIGN(8);
    pharosPartition0DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 0 in Sram3 */
  .pharosPartition0BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 0 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition0BssSram3Start = .;

    *(.partition0BssSram3)
    *(.partition0BssSram3.*)

    /* address of the partition 0 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition0BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition0UssSram3Start = .;

    *(.partition0UssSram3)
    *(.partition0UssSram3.*)

    pharosPartition0UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_0_SRAM3_ALIGNMENT);

    /* address of the partition 0 sections end in Sram3 */
    pharosPartition0Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition0DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition0SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_0_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition0DataSramBackupStart = ABSOLUTE(.);

    *(.partition0DataSramBackup)
    *(.partition0DataSramBackup.*)

    /* address of the partition 0 data section end in RAM */
    . = ALIGN(8);
    pharosPartition0DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 0 in SramBackup */
  .pharosPartition0BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 0 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition0BssSramBackupStart = .;

    *(.partition0BssSramBackup)
    *(.partition0BssSramBackup.*)

    /* address of the partition 0 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition0BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition0UssSramBackupStart = .;

    *(.partition0UssSramBackup)
    *(.partition0UssSramBackup.*)

    pharosPartition0UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_0_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 0 sections end in SramBackup */
    pharosPartition0SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition0DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition0SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_0_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition0DataSdramStart = ABSOLUTE(.);

    *(.partition0DataSdram)
    *(.partition0DataSdram.*)

    /* address of the partition 0 data section end in RAM */
    . = ALIGN(8);
    pharosPartition0DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 0 in Sdram */
  .pharosPartition0BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 0 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition0BssSdramStart = .;

    *(.partition0BssSdram)
    *(.partition0BssSdram.*)

    /* address of the partition 0 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition0BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition0UssSdramStart = .;

    *(.partition0UssSdram)
    *(.partition0UssSdram.*)

    pharosPartition0UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_0_SDRAM_ALIGNMENT);

    /* address of the partition 0 sections end in Sdram */
    pharosPartition0SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 1                                                    */
  /******************************************************************/

  /* address in the flash of the partition 1 data to copy it to RAM */
  pharosPartition1DataCcmLoadAddr = LOADADDR(.partition1DataCcm);

  /* address in the flash of the partition 1 data to copy it to RAM */
  pharosPartition1DataSram1LoadAddr = LOADADDR(.partition1DataSram1);

  /* address in the flash of the partition 1 data to copy it to RAM */
  pharosPartition1DataSram2LoadAddr = LOADADDR(.partition1DataSram2);

  /* address in the flash of the partition 1 data to copy it to RAM */
  pharosPartition1DataSram3LoadAddr = LOADADDR(.partition1DataSram3);

  /* address in the flash of the partition 1 data to copy it to RAM */
  pharosPartition1DataSramBackupLoadAddr = LOADADDR(.partition1DataSramBackup);

  /* address in the flash of the partition 1 data to copy it to RAM */
  pharosPartition1DataSdramLoadAddr = LOADADDR(.partition1DataSdram);

  /* data of partition in Ccm */
  .partition1DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition1CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_1_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition1DataCcmStart = ABSOLUTE(.);

    *(.partition1DataCcm)
    *(.partition1DataCcm.*)

    /* address of the partition 1 data section end in RAM */
    . = ALIGN(8);
    pharosPartition1DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 1 in Ccm */
  .pharosPartition1BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 1 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition1BssCcmStart = .;

    *(.partition1BssCcm)
    *(.partition1BssCcm.*)

    /* address of the partition 1 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition1BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition1UssCcmStart = .;

    *(.partition1UssCcm)
    *(.partition1UssCcm.*)

    pharosPartition1UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_1_CCM_ALIGNMENT);

    /* address of the partition 1 sections end in Ccm */
    pharosPartition1CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition1DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition1Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_1_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition1DataSram1Start = ABSOLUTE(.);

    *(.partition1DataSram1)
    *(.partition1DataSram1.*)

    /* address of the partition 1 data section end in RAM */
    . = ALIGN(8);
    pharosPartition1DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 1 in Sram1 */
  .pharosPartition1BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 1 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition1BssSram1Start = .;

    *(.partition1BssSram1)
    *(.partition1BssSram1.*)

    /* address of the partition 1 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition1BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition1UssSram1Start = .;

    *(.partition1UssSram1)
    *(.partition1UssSram1.*)

    pharosPartition1UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_1_SRAM1_ALIGNMENT);

    /* address of the partition 1 sections end in Sram1 */
    pharosPartition1Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition1DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition1Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_1_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition1DataSram2Start = ABSOLUTE(.);

    *(.partition1DataSram2)
    *(.partition1DataSram2.*)

    /* address of the partition 1 data section end in RAM */
    . = ALIGN(8);
    pharosPartition1DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 1 in Sram2 */
  .pharosPartition1BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 1 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition1BssSram2Start = .;

    *(.partition1BssSram2)
    *(.partition1BssSram2.*)

    /* address of the partition 1 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition1BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition1UssSram2Start = .;

    *(.partition1UssSram2)
    *(.partition1UssSram2.*)

    pharosPartition1UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_1_SRAM2_ALIGNMENT);

    /* address of the partition 1 sections end in Sram2 */
    pharosPartition1Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition1DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition1Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_1_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition1DataSram3Start = ABSOLUTE(.);

    *(.partition1DataSram3)
    *(.partition1DataSram3.*)

    /* address of the partition 1 data section end in RAM */
    . = ALIGN(8);
    pharosPartition1DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 1 in Sram3 */
  .pharosPartition1BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 1 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition1BssSram3Start = .;

    *(.partition1BssSram3)
    *(.partition1BssSram3.*)

    /* address of the partition 1 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition1BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition1UssSram3Start = .;

    *(.partition1UssSram3)
    *(.partition1UssSram3.*)

    pharosPartition1UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_1_SRAM3_ALIGNMENT);

    /* address of the partition 1 sections end in Sram3 */
    pharosPartition1Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition1DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition1SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_1_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition1DataSramBackupStart = ABSOLUTE(.);

    *(.partition1DataSramBackup)
    *(.partition1DataSramBackup.*)

    /* address of the partition 1 data section end in RAM */
    . = ALIGN(8);
    pharosPartition1DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 1 in SramBackup */
  .pharosPartition1BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 1 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition1BssSramBackupStart = .;

    *(.partition1BssSramBackup)
    *(.partition1BssSramBackup.*)

    /* address of the partition 1 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition1BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition1UssSramBackupStart = .;

    *(.partition1UssSramBackup)
    *(.partition1UssSramBackup.*)

    pharosPartition1UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_1_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 1 sections end in SramBackup */
    pharosPartition1SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition1DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition1SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_1_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition1DataSdramStart = ABSOLUTE(.);

    *(.partition1DataSdram)
    *(.partition1DataSdram.*)

    /* address of the partition 1 data section end in RAM */
    . = ALIGN(8);
    pharosPartition1DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 1 in Sdram */
  .pharosPartition1BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 1 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition1BssSdramStart = .;

    *(.partition1BssSdram)
    *(.partition1BssSdram.*)

    /* address of the partition 1 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition1BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition1UssSdramStart = .;

    *(.partition1UssSdram)
    *(.partition1UssSdram.*)

    pharosPartition1UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_1_SDRAM_ALIGNMENT);

    /* address of the partition 1 sections end in Sdram */
    pharosPartition1SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 2                                                    */
  /******************************************************************/

  /* address in the flash of the partition 2 data to copy it to RAM */
  pharosPartition2DataCcmLoadAddr = LOADADDR(.partition2DataCcm);

  /* address in the flash of the partition 2 data to copy it to RAM */
  pharosPartition2DataSram1LoadAddr = LOADADDR(.partition2DataSram1);

  /* address in the flash of the partition 2 data to copy it to RAM */
  pharosPartition2DataSram2LoadAddr = LOADADDR(.partition2DataSram2);

  /* address in the flash of the partition 2 data to copy it to RAM */
  pharosPartition2DataSram3LoadAddr = LOADADDR(.partition2DataSram3);

  /* address in the flash of the partition 2 data to copy it to RAM */
  pharosPartition2DataSramBackupLoadAddr = LOADADDR(.partition2DataSramBackup);

  /* address in the flash of the partition 2 data to copy it to RAM */
  pharosPartition2DataSdramLoadAddr = LOADADDR(.partition2DataSdram);

  /* data of partition in Ccm */
  .partition2DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition2CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_2_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition2DataCcmStart = ABSOLUTE(.);

    *(.partition2DataCcm)
    *(.partition2DataCcm.*)

    /* address of the partition 2 data section end in RAM */
    . = ALIGN(8);
    pharosPartition2DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 2 in Ccm */
  .pharosPartition2BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 2 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition2BssCcmStart = .;

    *(.partition2BssCcm)
    *(.partition2BssCcm.*)

    /* address of the partition 2 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition2BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition2UssCcmStart = .;

    *(.partition2UssCcm)
    *(.partition2UssCcm.*)

    pharosPartition2UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_2_CCM_ALIGNMENT);

    /* address of the partition 2 sections end in Ccm */
    pharosPartition2CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition2DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition2Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_2_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition2DataSram1Start = ABSOLUTE(.);

    *(.partition2DataSram1)
    *(.partition2DataSram1.*)

    /* address of the partition 2 data section end in RAM */
    . = ALIGN(8);
    pharosPartition2DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 2 in Sram1 */
  .pharosPartition2BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 2 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition2BssSram1Start = .;

    *(.partition2BssSram1)
    *(.partition2BssSram1.*)

    /* address of the partition 2 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition2BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition2UssSram1Start = .;

    *(.partition2UssSram1)
    *(.partition2UssSram1.*)

    pharosPartition2UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_2_SRAM1_ALIGNMENT);

    /* address of the partition 2 sections end in Sram1 */
    pharosPartition2Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition2DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition2Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_2_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition2DataSram2Start = ABSOLUTE(.);

    *(.partition2DataSram2)
    *(.partition2DataSram2.*)

    /* address of the partition 2 data section end in RAM */
    . = ALIGN(8);
    pharosPartition2DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 2 in Sram2 */
  .pharosPartition2BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 2 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition2BssSram2Start = .;

    *(.partition2BssSram2)
    *(.partition2BssSram2.*)

    /* address of the partition 2 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition2BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition2UssSram2Start = .;

    *(.partition2UssSram2)
    *(.partition2UssSram2.*)

    pharosPartition2UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_2_SRAM2_ALIGNMENT);

    /* address of the partition 2 sections end in Sram2 */
    pharosPartition2Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition2DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition2Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_2_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition2DataSram3Start = ABSOLUTE(.);

    *(.partition2DataSram3)
    *(.partition2DataSram3.*)

    /* address of the partition 2 data section end in RAM */
    . = ALIGN(8);
    pharosPartition2DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 2 in Sram3 */
  .pharosPartition2BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 2 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition2BssSram3Start = .;

    *(.partition2BssSram3)
    *(.partition2BssSram3.*)

    /* address of the partition 2 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition2BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition2UssSram3Start = .;

    *(.partition2UssSram3)
    *(.partition2UssSram3.*)

    pharosPartition2UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_2_SRAM3_ALIGNMENT);

    /* address of the partition 2 sections end in Sram3 */
    pharosPartition2Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition2DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition2SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_2_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition2DataSramBackupStart = ABSOLUTE(.);

    *(.partition2DataSramBackup)
    *(.partition2DataSramBackup.*)

    /* address of the partition 2 data section end in RAM */
    . = ALIGN(8);
    pharosPartition2DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 2 in SramBackup */
  .pharosPartition2BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 2 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition2BssSramBackupStart = .;

    *(.partition2BssSramBackup)
    *(.partition2BssSramBackup.*)

    /* address of the partition 2 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition2BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition2UssSramBackupStart = .;

    *(.partition2UssSramBackup)
    *(.partition2UssSramBackup.*)

    pharosPartition2UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_2_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 2 sections end in SramBackup */
    pharosPartition2SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition2DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition2SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_2_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition2DataSdramStart = ABSOLUTE(.);

    *(.partition2DataSdram)
    *(.partition2DataSdram.*)

    /* address of the partition 2 data section end in RAM */
    . = ALIGN(8);
    pharosPartition2DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 2 in Sdram */
  .pharosPartition2BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 2 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition2BssSdramStart = .;

    *(.partition2BssSdram)
    *(.partition2BssSdram.*)

    /* address of the partition 2 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition2BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition2UssSdramStart = .;

    *(.partition2UssSdram)
    *(.partition2UssSdram.*)

    pharosPartition2UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_2_SDRAM_ALIGNMENT);

    /* address of the partition 2 sections end in Sdram */
    pharosPartition2SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 3                                                    */
  /******************************************************************/

  /* address in the flash of the partition 3 data to copy it to RAM */
  pharosPartition3DataCcmLoadAddr = LOADADDR(.partition3DataCcm);

  /* address in the flash of the partition 3 data to copy it to RAM */
  pharosPartition3DataSram1LoadAddr = LOADADDR(.partition3DataSram1);

  /* address in the flash of the partition 3 data to copy it to RAM */
  pharosPartition3DataSram2LoadAddr = LOADADDR(.partition3DataSram2);

  /* address in the flash of the partition 3 data to copy it to RAM */
  pharosPartition3DataSram3LoadAddr = LOADADDR(.partition3DataSram3);

  /* address in the flash of the partition 3 data to copy it to RAM */
  pharosPartition3DataSramBackupLoadAddr = LOADADDR(.partition3DataSramBackup);

  /* address in the flash of the partition 3 data to copy it to RAM */
  pharosPartition3DataSdramLoadAddr = LOADADDR(.partition3DataSdram);

  /* data of partition in Ccm */
  .partition3DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition3CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_3_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition3DataCcmStart = ABSOLUTE(.);

    *(.partition3DataCcm)
    *(.partition3DataCcm.*)

    /* address of the partition 3 data section end in RAM */
    . = ALIGN(8);
    pharosPartition3DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 3 in Ccm */
  .pharosPartition3BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 3 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition3BssCcmStart = .;

    *(.partition3BssCcm)
    *(.partition3BssCcm.*)

    /* address of the partition 3 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition3BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition3UssCcmStart = .;

    *(.partition3UssCcm)
    *(.partition3UssCcm.*)

    pharosPartition3UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_3_CCM_ALIGNMENT);

    /* address of the partition 3 sections end in Ccm */
    pharosPartition3CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition3DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition3Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_3_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition3DataSram1Start = ABSOLUTE(.);

    *(.partition3DataSram1)
    *(.partition3DataSram1.*)

    /* address of the partition 3 data section end in RAM */
    . = ALIGN(8);
    pharosPartition3DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 3 in Sram1 */
  .pharosPartition3BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 3 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition3BssSram1Start = .;

    *(.partition3BssSram1)
    *(.partition3BssSram1.*)

    /* address of the partition 3 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition3BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition3UssSram1Start = .;

    *(.partition3UssSram1)
    *(.partition3UssSram1.*)

    pharosPartition3UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_3_SRAM1_ALIGNMENT);

    /* address of the partition 3 sections end in Sram1 */
    pharosPartition3Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition3DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition3Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_3_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition3DataSram2Start = ABSOLUTE(.);

    *(.partition3DataSram2)
    *(.partition3DataSram2.*)

    /* address of the partition 3 data section end in RAM */
    . = ALIGN(8);
    pharosPartition3DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 3 in Sram2 */
  .pharosPartition3BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 3 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition3BssSram2Start = .;

    *(.partition3BssSram2)
    *(.partition3BssSram2.*)

    /* address of the partition 3 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition3BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition3UssSram2Start = .;

    *(.partition3UssSram2)
    *(.partition3UssSram2.*)

    pharosPartition3UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_3_SRAM2_ALIGNMENT);

    /* address of the partition 3 sections end in Sram2 */
    pharosPartition3Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition3DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition3Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_3_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition3DataSram3Start = ABSOLUTE(.);

    *(.partition3DataSram3)
    *(.partition3DataSram3.*)

    /* address of the partition 3 data section end in RAM */
    . = ALIGN(8);
    pharosPartition3DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 3 in Sram3 */
  .pharosPartition3BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 3 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition3BssSram3Start = .;

    *(.partition3BssSram3)
    *(.partition3BssSram3.*)

    /* address of the partition 3 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition3BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition3UssSram3Start = .;

    *(.partition3UssSram3)
    *(.partition3UssSram3.*)

    pharosPartition3UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_3_SRAM3_ALIGNMENT);

    /* address of the partition 3 sections end in Sram3 */
    pharosPartition3Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition3DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition3SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_3_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition3DataSramBackupStart = ABSOLUTE(.);

    *(.partition3DataSramBackup)
    *(.partition3DataSramBackup.*)

    /* address of the partition 3 data section end in RAM */
    . = ALIGN(8);
    pharosPartition3DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 3 in SramBackup */
  .pharosPartition3BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 3 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition3BssSramBackupStart = .;

    *(.partition3BssSramBackup)
    *(.partition3BssSramBackup.*)

    /* address of the partition 3 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition3BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition3UssSramBackupStart = .;

    *(.partition3UssSramBackup)
    *(.partition3UssSramBackup.*)

    pharosPartition3UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_3_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 3 sections end in SramBackup */
    pharosPartition3SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition3DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition3SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_3_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition3DataSdramStart = ABSOLUTE(.);

    *(.partition3DataSdram)
    *(.partition3DataSdram.*)

    /* address of the partition 3 data section end in RAM */
    . = ALIGN(8);
    pharosPartition3DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 3 in Sdram */
  .pharosPartition3BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 3 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition3BssSdramStart = .;

    *(.partition3BssSdram)
    *(.partition3BssSdram.*)

    /* address of the partition 3 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition3BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition3UssSdramStart = .;

    *(.partition3UssSdram)
    *(.partition3UssSdram.*)

    pharosPartition3UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_3_SDRAM_ALIGNMENT);

    /* address of the partition 3 sections end in Sdram */
    pharosPartition3SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 4                                                    */
  /******************************************************************/

  /* address in the flash of the partition 4 data to copy it to RAM */
  pharosPartition4DataCcmLoadAddr = LOADADDR(.partition4DataCcm);

  /* address in the flash of the partition 4 data to copy it to RAM */
  pharosPartition4DataSram1LoadAddr = LOADADDR(.partition4DataSram1);

  /* address in the flash of the partition 4 data to copy it to RAM */
  pharosPartition4DataSram2LoadAddr = LOADADDR(.partition4DataSram2);

  /* address in the flash of the partition 4 data to copy it to RAM */
  pharosPartition4DataSram3LoadAddr = LOADADDR(.partition4DataSram3);

  /* address in the flash of the partition 4 data to copy it to RAM */
  pharosPartition4DataSramBackupLoadAddr = LOADADDR(.partition4DataSramBackup);

  /* address in the flash of the partition 4 data to copy it to RAM */
  pharosPartition4DataSdramLoadAddr = LOADADDR(.partition4DataSdram);

  /* data of partition in Ccm */
  .partition4DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition4CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_4_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition4DataCcmStart = ABSOLUTE(.);

    *(.partition4DataCcm)
    *(.partition4DataCcm.*)

    /* address of the partition 4 data section end in RAM */
    . = ALIGN(8);
    pharosPartition4DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 4 in Ccm */
  .pharosPartition4BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 4 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition4BssCcmStart = .;

    *(.partition4BssCcm)
    *(.partition4BssCcm.*)

    /* address of the partition 4 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition4BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition4UssCcmStart = .;

    *(.partition4UssCcm)
    *(.partition4UssCcm.*)

    pharosPartition4UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_4_CCM_ALIGNMENT);

    /* address of the partition 4 sections end in Ccm */
    pharosPartition4CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition4DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition4Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_4_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition4DataSram1Start = ABSOLUTE(.);

    *(.partition4DataSram1)
    *(.partition4DataSram1.*)

    /* address of the partition 4 data section end in RAM */
    . = ALIGN(8);
    pharosPartition4DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 4 in Sram1 */
  .pharosPartition4BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 4 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition4BssSram1Start = .;

    *(.partition4BssSram1)
    *(.partition4BssSram1.*)

    /* address of the partition 4 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition4BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition4UssSram1Start = .;

    *(.partition4UssSram1)
    *(.partition4UssSram1.*)

    pharosPartition4UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_4_SRAM1_ALIGNMENT);

    /* address of the partition 4 sections end in Sram1 */
    pharosPartition4Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition4DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition4Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_4_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition4DataSram2Start = ABSOLUTE(.);

    *(.partition4DataSram2)
    *(.partition4DataSram2.*)

    /* address of the partition 4 data section end in RAM */
    . = ALIGN(8);
    pharosPartition4DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 4 in Sram2 */
  .pharosPartition4BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 4 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition4BssSram2Start = .;

    *(.partition4BssSram2)
    *(.partition4BssSram2.*)

    /* address of the partition 4 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition4BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition4UssSram2Start = .;

    *(.partition4UssSram2)
    *(.partition4UssSram2.*)

    pharosPartition4UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_4_SRAM2_ALIGNMENT);

    /* address of the partition 4 sections end in Sram2 */
    pharosPartition4Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition4DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition4Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_4_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition4DataSram3Start = ABSOLUTE(.);

    *(.partition4DataSram3)
    *(.partition4DataSram3.*)

    /* address of the partition 4 data section end in RAM */
    . = ALIGN(8);
    pharosPartition4DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 4 in Sram3 */
  .pharosPartition4BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 4 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition4BssSram3Start = .;

    *(.partition4BssSram3)
    *(.partition4BssSram3.*)

    /* address of the partition 4 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition4BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition4UssSram3Start = .;

    *(.partition4UssSram3)
    *(.partition4UssSram3.*)

    pharosPartition4UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_4_SRAM3_ALIGNMENT);

    /* address of the partition 4 sections end in Sram3 */
    pharosPartition4Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition4DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition4SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_4_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition4DataSramBackupStart = ABSOLUTE(.);

    *(.partition4DataSramBackup)
    *(.partition4DataSramBackup.*)

    /* address of the partition 4 data section end in RAM */
    . = ALIGN(8);
    pharosPartition4DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 4 in SramBackup */
  .pharosPartition4BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 4 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition4BssSramBackupStart = .;

    *(.partition4BssSramBackup)
    *(.partition4BssSramBackup.*)

    /* address of the partition 4 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition4BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition4UssSramBackupStart = .;

    *(.partition4UssSramBackup)
    *(.partition4UssSramBackup.*)

    pharosPartition4UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_4_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 4 sections end in SramBackup */
    pharosPartition4SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition4DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition4SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_4_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition4DataSdramStart = ABSOLUTE(.);

    *(.partition4DataSdram)
    *(.partition4DataSdram.*)

    /* address of the partition 4 data section end in RAM */
    . = ALIGN(8);
    pharosPartition4DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 4 in Sdram */
  .pharosPartition4BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 4 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition4BssSdramStart = .;

    *(.partition4BssSdram)
    *(.partition4BssSdram.*)

    /* address of the partition 4 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition4BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition4UssSdramStart = .;

    *(.partition4UssSdram)
    *(.partition4UssSdram.*)

    pharosPartition4UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_4_SDRAM_ALIGNMENT);

    /* address of the partition 4 sections end in Sdram */
    pharosPartition4SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 5                                                    */
  /******************************************************************/

  /* address in the flash of the partition 5 data to copy it to RAM */
  pharosPartition5DataCcmLoadAddr = LOADADDR(.partition5DataCcm);

  /* address in the flash of the partition 5 data to copy it to RAM */
  pharosPartition5DataSram1LoadAddr = LOADADDR(.partition5DataSram1);

  /* address in the flash of the partition 5 data to copy it to RAM */
  pharosPartition5DataSram2LoadAddr = LOADADDR(.partition5DataSram2);

  /* address in the flash of the partition 5 data to copy it to RAM */
  pharosPartition5DataSram3LoadAddr = LOADADDR(.partition5DataSram3);

  /* address in the flash of the partition 5 data to copy it to RAM */
  pharosPartition5DataSramBackupLoadAddr = LOADADDR(.partition5DataSramBackup);

  /* address in the flash of the partition 5 data to copy it to RAM */
  pharosPartition5DataSdramLoadAddr = LOADADDR(.partition5DataSdram);

  /* data of partition in Ccm */
  .partition5DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition5CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_5_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition5DataCcmStart = ABSOLUTE(.);

    *(.partition5DataCcm)
    *(.partition5DataCcm.*)

    /* address of the partition 5 data section end in RAM */
    . = ALIGN(8);
    pharosPartition5DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 5 in Ccm */
  .pharosPartition5BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 5 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition5BssCcmStart = .;

    *(.partition5BssCcm)
    *(.partition5BssCcm.*)

    /* address of the partition 5 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition5BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition5UssCcmStart = .;

    *(.partition5UssCcm)
    *(.partition5UssCcm.*)

    pharosPartition5UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_5_CCM_ALIGNMENT);

    /* address of the partition 5 sections end in Ccm */
    pharosPartition5CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition5DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition5Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_5_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition5DataSram1Start = ABSOLUTE(.);

    *(.partition5DataSram1)
    *(.partition5DataSram1.*)

    /* address of the partition 5 data section end in RAM */
    . = ALIGN(8);
    pharosPartition5DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 5 in Sram1 */
  .pharosPartition5BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 5 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition5BssSram1Start = .;

    *(.partition5BssSram1)
    *(.partition5BssSram1.*)

    /* address of the partition 5 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition5BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition5UssSram1Start = .;

    *(.partition5UssSram1)
    *(.partition5UssSram1.*)

    pharosPartition5UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_5_SRAM1_ALIGNMENT);

    /* address of the partition 5 sections end in Sram1 */
    pharosPartition5Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition5DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition5Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_5_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition5DataSram2Start = ABSOLUTE(.);

    *(.partition5DataSram2)
    *(.partition5DataSram2.*)

    /* address of the partition 5 data section end in RAM */
    . = ALIGN(8);
    pharosPartition5DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 5 in Sram2 */
  .pharosPartition5BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 5 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition5BssSram2Start = .;

    *(.partition5BssSram2)
    *(.partition5BssSram2.*)

    /* address of the partition 5 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition5BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition5UssSram2Start = .;

    *(.partition5UssSram2)
    *(.partition5UssSram2.*)

    pharosPartition5UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_5_SRAM2_ALIGNMENT);

    /* address of the partition 5 sections end in Sram2 */
    pharosPartition5Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition5DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition5Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_5_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition5DataSram3Start = ABSOLUTE(.);

    *(.partition5DataSram3)
    *(.partition5DataSram3.*)

    /* address of the partition 5 data section end in RAM */
    . = ALIGN(8);
    pharosPartition5DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 5 in Sram3 */
  .pharosPartition5BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 5 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition5BssSram3Start = .;

    *(.partition5BssSram3)
    *(.partition5BssSram3.*)

    /* address of the partition 5 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition5BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition5UssSram3Start = .;

    *(.partition5UssSram3)
    *(.partition5UssSram3.*)

    pharosPartition5UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_5_SRAM3_ALIGNMENT);

    /* address of the partition 5 sections end in Sram3 */
    pharosPartition5Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition5DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition5SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_5_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition5DataSramBackupStart = ABSOLUTE(.);

    *(.partition5DataSramBackup)
    *(.partition5DataSramBackup.*)

    /* address of the partition 5 data section end in RAM */
    . = ALIGN(8);
    pharosPartition5DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 5 in SramBackup */
  .pharosPartition5BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 5 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition5BssSramBackupStart = .;

    *(.partition5BssSramBackup)
    *(.partition5BssSramBackup.*)

    /* address of the partition 5 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition5BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition5UssSramBackupStart = .;

    *(.partition5UssSramBackup)
    *(.partition5UssSramBackup.*)

    pharosPartition5UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_5_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 5 sections end in SramBackup */
    pharosPartition5SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition5DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition5SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_5_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition5DataSdramStart = ABSOLUTE(.);

    *(.partition5DataSdram)
    *(.partition5DataSdram.*)

    /* address of the partition 5 data section end in RAM */
    . = ALIGN(8);
    pharosPartition5DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 5 in Sdram */
  .pharosPartition5BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 5 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition5BssSdramStart = .;

    *(.partition5BssSdram)
    *(.partition5BssSdram.*)

    /* address of the partition 5 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition5BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition5UssSdramStart = .;

    *(.partition5UssSdram)
    *(.partition5UssSdram.*)

    pharosPartition5UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_5_SDRAM_ALIGNMENT);

    /* address of the partition 5 sections end in Sdram */
    pharosPartition5SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 6                                                    */
  /******************************************************************/

  /* address in the flash of the partition 6 data to copy it to RAM */
  pharosPartition6DataCcmLoadAddr = LOADADDR(.partition6DataCcm);

  /* address in the flash of the partition 6 data to copy it to RAM */
  pharosPartition6DataSram1LoadAddr = LOADADDR(.partition6DataSram1);

  /* address in the flash of the partition 6 data to copy it to RAM */
  pharosPartition6DataSram2LoadAddr = LOADADDR(.partition6DataSram2);

  /* address in the flash of the partition 6 data to copy it to RAM */
  pharosPartition6DataSram3LoadAddr = LOADADDR(.partition6DataSram3);

  /* address in the flash of the partition 6 data to copy it to RAM */
  pharosPartition6DataSramBackupLoadAddr = LOADADDR(.partition6DataSramBackup);

  /* address in the flash of the partition 6 data to copy it to RAM */
  pharosPartition6DataSdramLoadAddr = LOADADDR(.partition6DataSdram);

  /* data of partition in Ccm */
  .partition6DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition6CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_6_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition6DataCcmStart = ABSOLUTE(.);

    *(.partition6DataCcm)
    *(.partition6DataCcm.*)

    /* address of the partition 6 data section end in RAM */
    . = ALIGN(8);
    pharosPartition6DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 6 in Ccm */
  .pharosPartition6BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 6 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition6BssCcmStart = .;

    *(.partition6BssCcm)
    *(.partition6BssCcm.*)

    /* address of the partition 6 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition6BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition6UssCcmStart = .;

    *(.partition6UssCcm)
    *(.partition6UssCcm.*)

    pharosPartition6UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_6_CCM_ALIGNMENT);

    /* address of the partition 6 sections end in Ccm */
    pharosPartition6CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition6DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition6Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_6_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition6DataSram1Start = ABSOLUTE(.);

    *(.partition6DataSram1)
    *(.partition6DataSram1.*)

    /* address of the partition 6 data section end in RAM */
    . = ALIGN(8);
    pharosPartition6DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 6 in Sram1 */
  .pharosPartition6BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 6 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition6BssSram1Start = .;

    *(.partition6BssSram1)
    *(.partition6BssSram1.*)

    /* address of the partition 6 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition6BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition6UssSram1Start = .;

    *(.partition6UssSram1)
    *(.partition6UssSram1.*)

    pharosPartition6UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_6_SRAM1_ALIGNMENT);

    /* address of the partition 6 sections end in Sram1 */
    pharosPartition6Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition6DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition6Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_6_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition6DataSram2Start = ABSOLUTE(.);

    *(.partition6DataSram2)
    *(.partition6DataSram2.*)

    /* address of the partition 6 data section end in RAM */
    . = ALIGN(8);
    pharosPartition6DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 6 in Sram2 */
  .pharosPartition6BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 6 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition6BssSram2Start = .;

    *(.partition6BssSram2)
    *(.partition6BssSram2.*)

    /* address of the partition 6 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition6BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition6UssSram2Start = .;

    *(.partition6UssSram2)
    *(.partition6UssSram2.*)

    pharosPartition6UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_6_SRAM2_ALIGNMENT);

    /* address of the partition 6 sections end in Sram2 */
    pharosPartition6Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition6DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition6Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_6_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition6DataSram3Start = ABSOLUTE(.);

    *(.partition6DataSram3)
    *(.partition6DataSram3.*)

    /* address of the partition 6 data section end in RAM */
    . = ALIGN(8);
    pharosPartition6DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 6 in Sram3 */
  .pharosPartition6BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 6 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition6BssSram3Start = .;

    *(.partition6BssSram3)
    *(.partition6BssSram3.*)

    /* address of the partition 6 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition6BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition6UssSram3Start = .;

    *(.partition6UssSram3)
    *(.partition6UssSram3.*)

    pharosPartition6UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_6_SRAM3_ALIGNMENT);

    /* address of the partition 6 sections end in Sram3 */
    pharosPartition6Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition6DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition6SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_6_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition6DataSramBackupStart = ABSOLUTE(.);

    *(.partition6DataSramBackup)
    *(.partition6DataSramBackup.*)

    /* address of the partition 6 data section end in RAM */
    . = ALIGN(8);
    pharosPartition6DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 6 in SramBackup */
  .pharosPartition6BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 6 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition6BssSramBackupStart = .;

    *(.partition6BssSramBackup)
    *(.partition6BssSramBackup.*)

    /* address of the partition 6 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition6BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition6UssSramBackupStart = .;

    *(.partition6UssSramBackup)
    *(.partition6UssSramBackup.*)

    pharosPartition6UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_6_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 6 sections end in SramBackup */
    pharosPartition6SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition6DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition6SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_6_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition6DataSdramStart = ABSOLUTE(.);

    *(.partition6DataSdram)
    *(.partition6DataSdram.*)

    /* address of the partition 6 data section end in RAM */
    . = ALIGN(8);
    pharosPartition6DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 6 in Sdram */
  .pharosPartition6BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 6 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition6BssSdramStart = .;

    *(.partition6BssSdram)
    *(.partition6BssSdram.*)

    /* address of the partition 6 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition6BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition6UssSdramStart = .;

    *(.partition6UssSdram)
    *(.partition6UssSdram.*)

    pharosPartition6UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_6_SDRAM_ALIGNMENT);

    /* address of the partition 6 sections end in Sdram */
    pharosPartition6SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 7                                                    */
  /******************************************************************/

  /* address in the flash of the partition 7 data to copy it to RAM */
  pharosPartition7DataCcmLoadAddr = LOADADDR(.partition7DataCcm);

  /* address in the flash of the partition 7 data to copy it to RAM */
  pharosPartition7DataSram1LoadAddr = LOADADDR(.partition7DataSram1);

  /* address in the flash of the partition 7 data to copy it to RAM */
  pharosPartition7DataSram2LoadAddr = LOADADDR(.partition7DataSram2);

  /* address in the flash of the partition 7 data to copy it to RAM */
  pharosPartition7DataSram3LoadAddr = LOADADDR(.partition7DataSram3);

  /* address in the flash of the partition 7 data to copy it to RAM */
  pharosPartition7DataSramBackupLoadAddr = LOADADDR(.partition7DataSramBackup);

  /* address in the flash of the partition 7 data to copy it to RAM */
  pharosPartition7DataSdramLoadAddr = LOADADDR(.partition7DataSdram);

  /* data of partition in Ccm */
  .partition7DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition7CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_7_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition7DataCcmStart = ABSOLUTE(.);

    *(.partition7DataCcm)
    *(.partition7DataCcm.*)

    /* address of the partition 7 data section end in RAM */
    . = ALIGN(8);
    pharosPartition7DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 7 in Ccm */
  .pharosPartition7BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 7 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition7BssCcmStart = .;

    *(.partition7BssCcm)
    *(.partition7BssCcm.*)

    /* address of the partition 7 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition7BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition7UssCcmStart = .;

    *(.partition7UssCcm)
    *(.partition7UssCcm.*)

    pharosPartition7UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_7_CCM_ALIGNMENT);

    /* address of the partition 7 sections end in Ccm */
    pharosPartition7CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition7DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition7Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_7_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition7DataSram1Start = ABSOLUTE(.);

    *(.partition7DataSram1)
    *(.partition7DataSram1.*)

    /* address of the partition 7 data section end in RAM */
    . = ALIGN(8);
    pharosPartition7DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 7 in Sram1 */
  .pharosPartition7BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 7 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition7BssSram1Start = .;

    *(.partition7BssSram1)
    *(.partition7BssSram1.*)

    /* address of the partition 7 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition7BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition7UssSram1Start = .;

    *(.partition7UssSram1)
    *(.partition7UssSram1.*)

    pharosPartition7UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_7_SRAM1_ALIGNMENT);

    /* address of the partition 7 sections end in Sram1 */
    pharosPartition7Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition7DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition7Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_7_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition7DataSram2Start = ABSOLUTE(.);

    *(.partition7DataSram2)
    *(.partition7DataSram2.*)

    /* address of the partition 7 data section end in RAM */
    . = ALIGN(8);
    pharosPartition7DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 7 in Sram2 */
  .pharosPartition7BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 7 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition7BssSram2Start = .;

    *(.partition7BssSram2)
    *(.partition7BssSram2.*)

    /* address of the partition 7 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition7BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition7UssSram2Start = .;

    *(.partition7UssSram2)
    *(.partition7UssSram2.*)

    pharosPartition7UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_7_SRAM2_ALIGNMENT);

    /* address of the partition 7 sections end in Sram2 */
    pharosPartition7Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition7DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition7Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_7_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition7DataSram3Start = ABSOLUTE(.);

    *(.partition7DataSram3)
    *(.partition7DataSram3.*)

    /* address of the partition 7 data section end in RAM */
    . = ALIGN(8);
    pharosPartition7DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 7 in Sram3 */
  .pharosPartition7BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 7 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition7BssSram3Start = .;

    *(.partition7BssSram3)
    *(.partition7BssSram3.*)

    /* address of the partition 7 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition7BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition7UssSram3Start = .;

    *(.partition7UssSram3)
    *(.partition7UssSram3.*)

    pharosPartition7UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_7_SRAM3_ALIGNMENT);

    /* address of the partition 7 sections end in Sram3 */
    pharosPartition7Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition7DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition7SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_7_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition7DataSramBackupStart = ABSOLUTE(.);

    *(.partition7DataSramBackup)
    *(.partition7DataSramBackup.*)

    /* address of the partition 7 data section end in RAM */
    . = ALIGN(8);
    pharosPartition7DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 7 in SramBackup */
  .pharosPartition7BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 7 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition7BssSramBackupStart = .;

    *(.partition7BssSramBackup)
    *(.partition7BssSramBackup.*)

    /* address of the partition 7 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition7BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition7UssSramBackupStart = .;

    *(.partition7UssSramBackup)
    *(.partition7UssSramBackup.*)

    pharosPartition7UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_7_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 7 sections end in SramBackup */
    pharosPartition7SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition7DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition7SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_7_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition7DataSdramStart = ABSOLUTE(.);

    *(.partition7DataSdram)
    *(.partition7DataSdram.*)

    /* address of the partition 7 data section end in RAM */
    . = ALIGN(8);
    pharosPartition7DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 7 in Sdram */
  .pharosPartition7BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 7 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition7BssSdramStart = .;

    *(.partition7BssSdram)
    *(.partition7BssSdram.*)

    /* address of the partition 7 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition7BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition7UssSdramStart = .;

    *(.partition7UssSdram)
    *(.partition7UssSdram.*)

    pharosPartition7UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_7_SDRAM_ALIGNMENT);

    /* address of the partition 7 sections end in Sdram */
    pharosPartition7SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 8                                                    */
  /******************************************************************/

  /* address in the flash of the partition 8 data to copy it to RAM */
  pharosPartition8DataCcmLoadAddr = LOADADDR(.partition8DataCcm);

  /* address in the flash of the partition 8 data to copy it to RAM */
  pharosPartition8DataSram1LoadAddr = LOADADDR(.partition8DataSram1);

  /* address in the flash of the partition 8 data to copy it to RAM */
  pharosPartition8DataSram2LoadAddr = LOADADDR(.partition8DataSram2);

  /* address in the flash of the partition 8 data to copy it to RAM */
  pharosPartition8DataSram3LoadAddr = LOADADDR(.partition8DataSram3);

  /* address in the flash of the partition 8 data to copy it to RAM */
  pharosPartition8DataSramBackupLoadAddr = LOADADDR(.partition8DataSramBackup);

  /* address in the flash of the partition 8 data to copy it to RAM */
  pharosPartition8DataSdramLoadAddr = LOADADDR(.partition8DataSdram);

  /* data of partition in Ccm */
  .partition8DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition8CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_8_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition8DataCcmStart = ABSOLUTE(.);

    *(.partition8DataCcm)
    *(.partition8DataCcm.*)

    /* address of the partition 8 data section end in RAM */
    . = ALIGN(8);
    pharosPartition8DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 8 in Ccm */
  .pharosPartition8BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 8 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition8BssCcmStart = .;

    *(.partition8BssCcm)
    *(.partition8BssCcm.*)

    /* address of the partition 8 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition8BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition8UssCcmStart = .;

    *(.partition8UssCcm)
    *(.partition8UssCcm.*)

    pharosPartition8UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_8_CCM_ALIGNMENT);

    /* address of the partition 8 sections end in Ccm */
    pharosPartition8CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition8DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition8Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_8_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition8DataSram1Start = ABSOLUTE(.);

    *(.partition8DataSram1)
    *(.partition8DataSram1.*)

    /* address of the partition 8 data section end in RAM */
    . = ALIGN(8);
    pharosPartition8DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 8 in Sram1 */
  .pharosPartition8BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 8 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition8BssSram1Start = .;

    *(.partition8BssSram1)
    *(.partition8BssSram1.*)

    /* address of the partition 8 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition8BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition8UssSram1Start = .;

    *(.partition8UssSram1)
    *(.partition8UssSram1.*)

    pharosPartition8UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_8_SRAM1_ALIGNMENT);

    /* address of the partition 8 sections end in Sram1 */
    pharosPartition8Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition8DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition8Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_8_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition8DataSram2Start = ABSOLUTE(.);

    *(.partition8DataSram2)
    *(.partition8DataSram2.*)

    /* address of the partition 8 data section end in RAM */
    . = ALIGN(8);
    pharosPartition8DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 8 in Sram2 */
  .pharosPartition8BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 8 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition8BssSram2Start = .;

    *(.partition8BssSram2)
    *(.partition8BssSram2.*)

    /* address of the partition 8 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition8BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition8UssSram2Start = .;

    *(.partition8UssSram2)
    *(.partition8UssSram2.*)

    pharosPartition8UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_8_SRAM2_ALIGNMENT);

    /* address of the partition 8 sections end in Sram2 */
    pharosPartition8Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition8DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition8Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_8_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition8DataSram3Start = ABSOLUTE(.);

    *(.partition8DataSram3)
    *(.partition8DataSram3.*)

    /* address of the partition 8 data section end in RAM */
    . = ALIGN(8);
    pharosPartition8DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 8 in Sram3 */
  .pharosPartition8BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 8 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition8BssSram3Start = .;

    *(.partition8BssSram3)
    *(.partition8BssSram3.*)

    /* address of the partition 8 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition8BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition8UssSram3Start = .;

    *(.partition8UssSram3)
    *(.partition8UssSram3.*)

    pharosPartition8UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_8_SRAM3_ALIGNMENT);

    /* address of the partition 8 sections end in Sram3 */
    pharosPartition8Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition8DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition8SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_8_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition8DataSramBackupStart = ABSOLUTE(.);

    *(.partition8DataSramBackup)
    *(.partition8DataSramBackup.*)

    /* address of the partition 8 data section end in RAM */
    . = ALIGN(8);
    pharosPartition8DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 8 in SramBackup */
  .pharosPartition8BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 8 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition8BssSramBackupStart = .;

    *(.partition8BssSramBackup)
    *(.partition8BssSramBackup.*)

    /* address of the partition 8 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition8BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition8UssSramBackupStart = .;

    *(.partition8UssSramBackup)
    *(.partition8UssSramBackup.*)

    pharosPartition8UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_8_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 8 sections end in SramBackup */
    pharosPartition8SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition8DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition8SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_8_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition8DataSdramStart = ABSOLUTE(.);

    *(.partition8DataSdram)
    *(.partition8DataSdram.*)

    /* address of the partition 8 data section end in RAM */
    . = ALIGN(8);
    pharosPartition8DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 8 in Sdram */
  .pharosPartition8BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 8 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition8BssSdramStart = .;

    *(.partition8BssSdram)
    *(.partition8BssSdram.*)

    /* address of the partition 8 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition8BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition8UssSdramStart = .;

    *(.partition8UssSdram)
    *(.partition8UssSdram.*)

    pharosPartition8UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_8_SDRAM_ALIGNMENT);

    /* address of the partition 8 sections end in Sdram */
    pharosPartition8SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 9                                                    */
  /******************************************************************/

  /* address in the flash of the partition 9 data to copy it to RAM */
  pharosPartition9DataCcmLoadAddr = LOADADDR(.partition9DataCcm);

  /* address in the flash of the partition 9 data to copy it to RAM */
  pharosPartition9DataSram1LoadAddr = LOADADDR(.partition9DataSram1);

  /* address in the flash of the partition 9 data to copy it to RAM */
  pharosPartition9DataSram2LoadAddr = LOADADDR(.partition9DataSram2);

  /* address in the flash of the partition 9 data to copy it to RAM */
  pharosPartition9DataSram3LoadAddr = LOADADDR(.partition9DataSram3);

  /* address in the flash of the partition 9 data to copy it to RAM */
  pharosPartition9DataSramBackupLoadAddr = LOADADDR(.partition9DataSramBackup);

  /* address in the flash of the partition 9 data to copy it to RAM */
  pharosPartition9DataSdramLoadAddr = LOADADDR(.partition9DataSdram);

  /* data of partition in Ccm */
  .partition9DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition9CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_9_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition9DataCcmStart = ABSOLUTE(.);

    *(.partition9DataCcm)
    *(.partition9DataCcm.*)

    /* address of the partition 9 data section end in RAM */
    . = ALIGN(8);
    pharosPartition9DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 9 in Ccm */
  .pharosPartition9BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 9 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition9BssCcmStart = .;

    *(.partition9BssCcm)
    *(.partition9BssCcm.*)

    /* address of the partition 9 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition9BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition9UssCcmStart = .;

    *(.partition9UssCcm)
    *(.partition9UssCcm.*)

    pharosPartition9UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_9_CCM_ALIGNMENT);

    /* address of the partition 9 sections end in Ccm */
    pharosPartition9CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition9DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition9Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_9_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition9DataSram1Start = ABSOLUTE(.);

    *(.partition9DataSram1)
    *(.partition9DataSram1.*)

    /* address of the partition 9 data section end in RAM */
    . = ALIGN(8);
    pharosPartition9DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 9 in Sram1 */
  .pharosPartition9BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 9 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition9BssSram1Start = .;

    *(.partition9BssSram1)
    *(.partition9BssSram1.*)

    /* address of the partition 9 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition9BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition9UssSram1Start = .;

    *(.partition9UssSram1)
    *(.partition9UssSram1.*)

    pharosPartition9UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_9_SRAM1_ALIGNMENT);

    /* address of the partition 9 sections end in Sram1 */
    pharosPartition9Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition9DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition9Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_9_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition9DataSram2Start = ABSOLUTE(.);

    *(.partition9DataSram2)
    *(.partition9DataSram2.*)

    /* address of the partition 9 data section end in RAM */
    . = ALIGN(8);
    pharosPartition9DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 9 in Sram2 */
  .pharosPartition9BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 9 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition9BssSram2Start = .;

    *(.partition9BssSram2)
    *(.partition9BssSram2.*)

    /* address of the partition 9 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition9BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition9UssSram2Start = .;

    *(.partition9UssSram2)
    *(.partition9UssSram2.*)

    pharosPartition9UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_9_SRAM2_ALIGNMENT);

    /* address of the partition 9 sections end in Sram2 */
    pharosPartition9Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition9DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition9Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_9_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition9DataSram3Start = ABSOLUTE(.);

    *(.partition9DataSram3)
    *(.partition9DataSram3.*)

    /* address of the partition 9 data section end in RAM */
    . = ALIGN(8);
    pharosPartition9DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 9 in Sram3 */
  .pharosPartition9BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 9 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition9BssSram3Start = .;

    *(.partition9BssSram3)
    *(.partition9BssSram3.*)

    /* address of the partition 9 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition9BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition9UssSram3Start = .;

    *(.partition9UssSram3)
    *(.partition9UssSram3.*)

    pharosPartition9UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_9_SRAM3_ALIGNMENT);

    /* address of the partition 9 sections end in Sram3 */
    pharosPartition9Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition9DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition9SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_9_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition9DataSramBackupStart = ABSOLUTE(.);

    *(.partition9DataSramBackup)
    *(.partition9DataSramBackup.*)

    /* address of the partition 9 data section end in RAM */
    . = ALIGN(8);
    pharosPartition9DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 9 in SramBackup */
  .pharosPartition9BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 9 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition9BssSramBackupStart = .;

    *(.partition9BssSramBackup)
    *(.partition9BssSramBackup.*)

    /* address of the partition 9 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition9BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition9UssSramBackupStart = .;

    *(.partition9UssSramBackup)
    *(.partition9UssSramBackup.*)

    pharosPartition9UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_9_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 9 sections end in SramBackup */
    pharosPartition9SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition9DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition9SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_9_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition9DataSdramStart = ABSOLUTE(.);

    *(.partition9DataSdram)
    *(.partition9DataSdram.*)

    /* address of the partition 9 data section end in RAM */
    . = ALIGN(8);
    pharosPartition9DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 9 in Sdram */
  .pharosPartition9BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 9 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition9BssSdramStart = .;

    *(.partition9BssSdram)
    *(.partition9BssSdram.*)

    /* address of the partition 9 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition9BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition9UssSdramStart = .;

    *(.partition9UssSdram)
    *(.partition9UssSdram.*)

    pharosPartition9UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_9_SDRAM_ALIGNMENT);

    /* address of the partition 9 sections end in Sdram */
    pharosPartition9SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 10                                                   */
  /******************************************************************/

  /* address in the flash of the partition 10 data to copy it to RAM */
  pharosPartition10DataCcmLoadAddr = LOADADDR(.partition10DataCcm);

  /* address in the flash of the partition 10 data to copy it to RAM */
  pharosPartition10DataSram1LoadAddr = LOADADDR(.partition10DataSram1);

  /* address in the flash of the partition 10 data to copy it to RAM */
  pharosPartition10DataSram2LoadAddr = LOADADDR(.partition10DataSram2);

  /* address in the flash of the partition 10 data to copy it to RAM */
  pharosPartition10DataSram3LoadAddr = LOADADDR(.partition10DataSram3);

  /* address in the flash of the partition 10 data to copy it to RAM */
  pharosPartition10DataSramBackupLoadAddr = LOADADDR(.partition10DataSramBackup);

  /* address in the flash of the partition 10 data to copy it to RAM */
  pharosPartition10DataSdramLoadAddr = LOADADDR(.partition10DataSdram);

  /* data of partition in Ccm */
  .partition10DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition10CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_10_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition10DataCcmStart = ABSOLUTE(.);

    *(.partition10DataCcm)
    *(.partition10DataCcm.*)

    /* address of the partition 10 data section end in RAM */
    . = ALIGN(8);
    pharosPartition10DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 10 in Ccm */
  .pharosPartition10BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 10 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition10BssCcmStart = .;

    *(.partition10BssCcm)
    *(.partition10BssCcm.*)

    /* address of the partition 10 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition10BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition10UssCcmStart = .;

    *(.partition10UssCcm)
    *(.partition10UssCcm.*)

    pharosPartition10UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_10_CCM_ALIGNMENT);

    /* address of the partition 10 sections end in Ccm */
    pharosPartition10CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition10DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition10Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_10_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition10DataSram1Start = ABSOLUTE(.);

    *(.partition10DataSram1)
    *(.partition10DataSram1.*)

    /* address of the partition 10 data section end in RAM */
    . = ALIGN(8);
    pharosPartition10DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 10 in Sram1 */
  .pharosPartition10BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 10 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition10BssSram1Start = .;

    *(.partition10BssSram1)
    *(.partition10BssSram1.*)

    /* address of the partition 10 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition10BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition10UssSram1Start = .;

    *(.partition10UssSram1)
    *(.partition10UssSram1.*)

    pharosPartition10UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_10_SRAM1_ALIGNMENT);

    /* address of the partition 10 sections end in Sram1 */
    pharosPartition10Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition10DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition10Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_10_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition10DataSram2Start = ABSOLUTE(.);

    *(.partition10DataSram2)
    *(.partition10DataSram2.*)

    /* address of the partition 10 data section end in RAM */
    . = ALIGN(8);
    pharosPartition10DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 10 in Sram2 */
  .pharosPartition10BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 10 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition10BssSram2Start = .;

    *(.partition10BssSram2)
    *(.partition10BssSram2.*)

    /* address of the partition 10 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition10BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition10UssSram2Start = .;

    *(.partition10UssSram2)
    *(.partition10UssSram2.*)

    pharosPartition10UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_10_SRAM2_ALIGNMENT);

    /* address of the partition 10 sections end in Sram2 */
    pharosPartition10Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition10DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition10Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_10_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition10DataSram3Start = ABSOLUTE(.);

    *(.partition10DataSram3)
    *(.partition10DataSram3.*)

    /* address of the partition 10 data section end in RAM */
    . = ALIGN(8);
    pharosPartition10DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 10 in Sram3 */
  .pharosPartition10BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 10 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition10BssSram3Start = .;

    *(.partition10BssSram3)
    *(.partition10BssSram3.*)

    /* address of the partition 10 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition10BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition10UssSram3Start = .;

    *(.partition10UssSram3)
    *(.partition10UssSram3.*)

    pharosPartition10UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_10_SRAM3_ALIGNMENT);

    /* address of the partition 10 sections end in Sram3 */
    pharosPartition10Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition10DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition10SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_10_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition10DataSramBackupStart = ABSOLUTE(.);

    *(.partition10DataSramBackup)
    *(.partition10DataSramBackup.*)

    /* address of the partition 10 data section end in RAM */
    . = ALIGN(8);
    pharosPartition10DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 10 in SramBackup */
  .pharosPartition10BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 10 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition10BssSramBackupStart = .;

    *(.partition10BssSramBackup)
    *(.partition10BssSramBackup.*)

    /* address of the partition 10 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition10BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition10UssSramBackupStart = .;

    *(.partition10UssSramBackup)
    *(.partition10UssSramBackup.*)

    pharosPartition10UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_10_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 10 sections end in SramBackup */
    pharosPartition10SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition10DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition10SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_10_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition10DataSdramStart = ABSOLUTE(.);

    *(.partition10DataSdram)
    *(.partition10DataSdram.*)

    /* address of the partition 10 data section end in RAM */
    . = ALIGN(8);
    pharosPartition10DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 10 in Sdram */
  .pharosPartition10BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 10 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition10BssSdramStart = .;

    *(.partition10BssSdram)
    *(.partition10BssSdram.*)

    /* address of the partition 10 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition10BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition10UssSdramStart = .;

    *(.partition10UssSdram)
    *(.partition10UssSdram.*)

    pharosPartition10UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_10_SDRAM_ALIGNMENT);

    /* address of the partition 10 sections end in Sdram */
    pharosPartition10SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 11                                                   */
  /******************************************************************/

  /* address in the flash of the partition 11 data to copy it to RAM */
  pharosPartition11DataCcmLoadAddr = LOADADDR(.partition11DataCcm);

  /* address in the flash of the partition 11 data to copy it to RAM */
  pharosPartition11DataSram1LoadAddr = LOADADDR(.partition11DataSram1);

  /* address in the flash of the partition 11 data to copy it to RAM */
  pharosPartition11DataSram2LoadAddr = LOADADDR(.partition11DataSram2);

  /* address in the flash of the partition 11 data to copy it to RAM */
  pharosPartition11DataSram3LoadAddr = LOADADDR(.partition11DataSram3);

  /* address in the flash of the partition 11 data to copy it to RAM */
  pharosPartition11DataSramBackupLoadAddr = LOADADDR(.partition11DataSramBackup);

  /* address in the flash of the partition 11 data to copy it to RAM */
  pharosPartition11DataSdramLoadAddr = LOADADDR(.partition11DataSdram);

  /* data of partition in Ccm */
  .partition11DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition11CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_11_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition11DataCcmStart = ABSOLUTE(.);

    *(.partition11DataCcm)
    *(.partition11DataCcm.*)

    /* address of the partition 11 data section end in RAM */
    . = ALIGN(8);
    pharosPartition11DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 11 in Ccm */
  .pharosPartition11BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 11 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition11BssCcmStart = .;

    *(.partition11BssCcm)
    *(.partition11BssCcm.*)

    /* address of the partition 11 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition11BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition11UssCcmStart = .;

    *(.partition11UssCcm)
    *(.partition11UssCcm.*)

    pharosPartition11UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_11_CCM_ALIGNMENT);

    /* address of the partition 11 sections end in Ccm */
    pharosPartition11CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition11DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition11Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_11_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition11DataSram1Start = ABSOLUTE(.);

    *(.partition11DataSram1)
    *(.partition11DataSram1.*)

    /* address of the partition 11 data section end in RAM */
    . = ALIGN(8);
    pharosPartition11DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 11 in Sram1 */
  .pharosPartition11BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 11 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition11BssSram1Start = .;

    *(.partition11BssSram1)
    *(.partition11BssSram1.*)

    /* address of the partition 11 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition11BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition11UssSram1Start = .;

    *(.partition11UssSram1)
    *(.partition11UssSram1.*)

    pharosPartition11UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_11_SRAM1_ALIGNMENT);

    /* address of the partition 11 sections end in Sram1 */
    pharosPartition11Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition11DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition11Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_11_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition11DataSram2Start = ABSOLUTE(.);

    *(.partition11DataSram2)
    *(.partition11DataSram2.*)

    /* address of the partition 11 data section end in RAM */
    . = ALIGN(8);
    pharosPartition11DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 11 in Sram2 */
  .pharosPartition11BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 11 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition11BssSram2Start = .;

    *(.partition11BssSram2)
    *(.partition11BssSram2.*)

    /* address of the partition 11 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition11BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition11UssSram2Start = .;

    *(.partition11UssSram2)
    *(.partition11UssSram2.*)

    pharosPartition11UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_11_SRAM2_ALIGNMENT);

    /* address of the partition 11 sections end in Sram2 */
    pharosPartition11Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition11DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition11Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_11_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition11DataSram3Start = ABSOLUTE(.);

    *(.partition11DataSram3)
    *(.partition11DataSram3.*)

    /* address of the partition 11 data section end in RAM */
    . = ALIGN(8);
    pharosPartition11DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 11 in Sram3 */
  .pharosPartition11BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 11 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition11BssSram3Start = .;

    *(.partition11BssSram3)
    *(.partition11BssSram3.*)

    /* address of the partition 11 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition11BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition11UssSram3Start = .;

    *(.partition11UssSram3)
    *(.partition11UssSram3.*)

    pharosPartition11UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_11_SRAM3_ALIGNMENT);

    /* address of the partition 11 sections end in Sram3 */
    pharosPartition11Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition11DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition11SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_11_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition11DataSramBackupStart = ABSOLUTE(.);

    *(.partition11DataSramBackup)
    *(.partition11DataSramBackup.*)

    /* address of the partition 11 data section end in RAM */
    . = ALIGN(8);
    pharosPartition11DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 11 in SramBackup */
  .pharosPartition11BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 11 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition11BssSramBackupStart = .;

    *(.partition11BssSramBackup)
    *(.partition11BssSramBackup.*)

    /* address of the partition 11 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition11BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition11UssSramBackupStart = .;

    *(.partition11UssSramBackup)
    *(.partition11UssSramBackup.*)

    pharosPartition11UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_11_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 11 sections end in SramBackup */
    pharosPartition11SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition11DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition11SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_11_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition11DataSdramStart = ABSOLUTE(.);

    *(.partition11DataSdram)
    *(.partition11DataSdram.*)

    /* address of the partition 11 data section end in RAM */
    . = ALIGN(8);
    pharosPartition11DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 11 in Sdram */
  .pharosPartition11BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 11 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition11BssSdramStart = .;

    *(.partition11BssSdram)
    *(.partition11BssSdram.*)

    /* address of the partition 11 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition11BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition11UssSdramStart = .;

    *(.partition11UssSdram)
    *(.partition11UssSdram.*)

    pharosPartition11UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_11_SDRAM_ALIGNMENT);

    /* address of the partition 11 sections end in Sdram */
    pharosPartition11SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 12                                                   */
  /******************************************************************/

  /* address in the flash of the partition 12 data to copy it to RAM */
  pharosPartition12DataCcmLoadAddr = LOADADDR(.partition12DataCcm);

  /* address in the flash of the partition 12 data to copy it to RAM */
  pharosPartition12DataSram1LoadAddr = LOADADDR(.partition12DataSram1);

  /* address in the flash of the partition 12 data to copy it to RAM */
  pharosPartition12DataSram2LoadAddr = LOADADDR(.partition12DataSram2);

  /* address in the flash of the partition 12 data to copy it to RAM */
  pharosPartition12DataSram3LoadAddr = LOADADDR(.partition12DataSram3);

  /* address in the flash of the partition 12 data to copy it to RAM */
  pharosPartition12DataSramBackupLoadAddr = LOADADDR(.partition12DataSramBackup);

  /* address in the flash of the partition 12 data to copy it to RAM */
  pharosPartition12DataSdramLoadAddr = LOADADDR(.partition12DataSdram);

  /* data of partition in Ccm */
  .partition12DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition12CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_12_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition12DataCcmStart = ABSOLUTE(.);

    *(.partition12DataCcm)
    *(.partition12DataCcm.*)

    /* address of the partition 12 data section end in RAM */
    . = ALIGN(8);
    pharosPartition12DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 12 in Ccm */
  .pharosPartition12BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 12 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition12BssCcmStart = .;

    *(.partition12BssCcm)
    *(.partition12BssCcm.*)

    /* address of the partition 12 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition12BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition12UssCcmStart = .;

    *(.partition12UssCcm)
    *(.partition12UssCcm.*)

    pharosPartition12UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_12_CCM_ALIGNMENT);

    /* address of the partition 12 sections end in Ccm */
    pharosPartition12CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition12DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition12Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_12_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition12DataSram1Start = ABSOLUTE(.);

    *(.partition12DataSram1)
    *(.partition12DataSram1.*)

    /* address of the partition 12 data section end in RAM */
    . = ALIGN(8);
    pharosPartition12DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 12 in Sram1 */
  .pharosPartition12BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 12 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition12BssSram1Start = .;

    *(.partition12BssSram1)
    *(.partition12BssSram1.*)

    /* address of the partition 12 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition12BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition12UssSram1Start = .;

    *(.partition12UssSram1)
    *(.partition12UssSram1.*)

    pharosPartition12UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_12_SRAM1_ALIGNMENT);

    /* address of the partition 12 sections end in Sram1 */
    pharosPartition12Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition12DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition12Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_12_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition12DataSram2Start = ABSOLUTE(.);

    *(.partition12DataSram2)
    *(.partition12DataSram2.*)

    /* address of the partition 12 data section end in RAM */
    . = ALIGN(8);
    pharosPartition12DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 12 in Sram2 */
  .pharosPartition12BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 12 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition12BssSram2Start = .;

    *(.partition12BssSram2)
    *(.partition12BssSram2.*)

    /* address of the partition 12 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition12BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition12UssSram2Start = .;

    *(.partition12UssSram2)
    *(.partition12UssSram2.*)

    pharosPartition12UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_12_SRAM2_ALIGNMENT);

    /* address of the partition 12 sections end in Sram2 */
    pharosPartition12Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition12DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition12Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_12_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition12DataSram3Start = ABSOLUTE(.);

    *(.partition12DataSram3)
    *(.partition12DataSram3.*)

    /* address of the partition 12 data section end in RAM */
    . = ALIGN(8);
    pharosPartition12DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 12 in Sram3 */
  .pharosPartition12BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 12 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition12BssSram3Start = .;

    *(.partition12BssSram3)
    *(.partition12BssSram3.*)

    /* address of the partition 12 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition12BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition12UssSram3Start = .;

    *(.partition12UssSram3)
    *(.partition12UssSram3.*)

    pharosPartition12UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_12_SRAM3_ALIGNMENT);

    /* address of the partition 12 sections end in Sram3 */
    pharosPartition12Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition12DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition12SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_12_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition12DataSramBackupStart = ABSOLUTE(.);

    *(.partition12DataSramBackup)
    *(.partition12DataSramBackup.*)

    /* address of the partition 12 data section end in RAM */
    . = ALIGN(8);
    pharosPartition12DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 12 in SramBackup */
  .pharosPartition12BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 12 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition12BssSramBackupStart = .;

    *(.partition12BssSramBackup)
    *(.partition12BssSramBackup.*)

    /* address of the partition 12 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition12BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition12UssSramBackupStart = .;

    *(.partition12UssSramBackup)
    *(.partition12UssSramBackup.*)

    pharosPartition12UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_12_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 12 sections end in SramBackup */
    pharosPartition12SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition12DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition12SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_12_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition12DataSdramStart = ABSOLUTE(.);

    *(.partition12DataSdram)
    *(.partition12DataSdram.*)

    /* address of the partition 12 data section end in RAM */
    . = ALIGN(8);
    pharosPartition12DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 12 in Sdram */
  .pharosPartition12BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 12 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition12BssSdramStart = .;

    *(.partition12BssSdram)
    *(.partition12BssSdram.*)

    /* address of the partition 12 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition12BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition12UssSdramStart = .;

    *(.partition12UssSdram)
    *(.partition12UssSdram.*)

    pharosPartition12UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_12_SDRAM_ALIGNMENT);

    /* address of the partition 12 sections end in Sdram */
    pharosPartition12SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 13                                                   */
  /******************************************************************/

  /* address in the flash of the partition 13 data to copy it to RAM */
  pharosPartition13DataCcmLoadAddr = LOADADDR(.partition13DataCcm);

  /* address in the flash of the partition 13 data to copy it to RAM */
  pharosPartition13DataSram1LoadAddr = LOADADDR(.partition13DataSram1);

  /* address in the flash of the partition 13 data to copy it to RAM */
  pharosPartition13DataSram2LoadAddr = LOADADDR(.partition13DataSram2);

  /* address in the flash of the partition 13 data to copy it to RAM */
  pharosPartition13DataSram3LoadAddr = LOADADDR(.partition13DataSram3);

  /* address in the flash of the partition 13 data to copy it to RAM */
  pharosPartition13DataSramBackupLoadAddr = LOADADDR(.partition13DataSramBackup);

  /* address in the flash of the partition 13 data to copy it to RAM */
  pharosPartition13DataSdramLoadAddr = LOADADDR(.partition13DataSdram);

  /* data of partition in Ccm */
  .partition13DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition13CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_13_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition13DataCcmStart = ABSOLUTE(.);

    *(.partition13DataCcm)
    *(.partition13DataCcm.*)

    /* address of the partition 13 data section end in RAM */
    . = ALIGN(8);
    pharosPartition13DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 13 in Ccm */
  .pharosPartition13BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 13 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition13BssCcmStart = .;

    *(.partition13BssCcm)
    *(.partition13BssCcm.*)

    /* address of the partition 13 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition13BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition13UssCcmStart = .;

    *(.partition13UssCcm)
    *(.partition13UssCcm.*)

    pharosPartition13UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_13_CCM_ALIGNMENT);

    /* address of the partition 13 sections end in Ccm */
    pharosPartition13CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition13DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition13Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_13_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition13DataSram1Start = ABSOLUTE(.);

    *(.partition13DataSram1)
    *(.partition13DataSram1.*)

    /* address of the partition 13 data section end in RAM */
    . = ALIGN(8);
    pharosPartition13DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 13 in Sram1 */
  .pharosPartition13BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 13 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition13BssSram1Start = .;

    *(.partition13BssSram1)
    *(.partition13BssSram1.*)

    /* address of the partition 13 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition13BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition13UssSram1Start = .;

    *(.partition13UssSram1)
    *(.partition13UssSram1.*)

    pharosPartition13UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_13_SRAM1_ALIGNMENT);

    /* address of the partition 13 sections end in Sram1 */
    pharosPartition13Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition13DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition13Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_13_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition13DataSram2Start = ABSOLUTE(.);

    *(.partition13DataSram2)
    *(.partition13DataSram2.*)

    /* address of the partition 13 data section end in RAM */
    . = ALIGN(8);
    pharosPartition13DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 13 in Sram2 */
  .pharosPartition13BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 13 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition13BssSram2Start = .;

    *(.partition13BssSram2)
    *(.partition13BssSram2.*)

    /* address of the partition 13 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition13BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition13UssSram2Start = .;

    *(.partition13UssSram2)
    *(.partition13UssSram2.*)

    pharosPartition13UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_13_SRAM2_ALIGNMENT);

    /* address of the partition 13 sections end in Sram2 */
    pharosPartition13Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition13DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition13Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_13_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition13DataSram3Start = ABSOLUTE(.);

    *(.partition13DataSram3)
    *(.partition13DataSram3.*)

    /* address of the partition 13 data section end in RAM */
    . = ALIGN(8);
    pharosPartition13DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 13 in Sram3 */
  .pharosPartition13BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 13 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition13BssSram3Start = .;

    *(.partition13BssSram3)
    *(.partition13BssSram3.*)

    /* address of the partition 13 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition13BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition13UssSram3Start = .;

    *(.partition13UssSram3)
    *(.partition13UssSram3.*)

    pharosPartition13UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_13_SRAM3_ALIGNMENT);

    /* address of the partition 13 sections end in Sram3 */
    pharosPartition13Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition13DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition13SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_13_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition13DataSramBackupStart = ABSOLUTE(.);

    *(.partition13DataSramBackup)
    *(.partition13DataSramBackup.*)

    /* address of the partition 13 data section end in RAM */
    . = ALIGN(8);
    pharosPartition13DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 13 in SramBackup */
  .pharosPartition13BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 13 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition13BssSramBackupStart = .;

    *(.partition13BssSramBackup)
    *(.partition13BssSramBackup.*)

    /* address of the partition 13 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition13BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition13UssSramBackupStart = .;

    *(.partition13UssSramBackup)
    *(.partition13UssSramBackup.*)

    pharosPartition13UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_13_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 13 sections end in SramBackup */
    pharosPartition13SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition13DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition13SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_13_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition13DataSdramStart = ABSOLUTE(.);

    *(.partition13DataSdram)
    *(.partition13DataSdram.*)

    /* address of the partition 13 data section end in RAM */
    . = ALIGN(8);
    pharosPartition13DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 13 in Sdram */
  .pharosPartition13BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 13 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition13BssSdramStart = .;

    *(.partition13BssSdram)
    *(.partition13BssSdram.*)

    /* address of the partition 13 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition13BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition13UssSdramStart = .;

    *(.partition13UssSdram)
    *(.partition13UssSdram.*)

    pharosPartition13UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_13_SDRAM_ALIGNMENT);

    /* address of the partition 13 sections end in Sdram */
    pharosPartition13SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 14                                                   */
  /******************************************************************/

  /* address in the flash of the partition 14 data to copy it to RAM */
  pharosPartition14DataCcmLoadAddr = LOADADDR(.partition14DataCcm);

  /* address in the flash of the partition 14 data to copy it to RAM */
  pharosPartition14DataSram1LoadAddr = LOADADDR(.partition14DataSram1);

  /* address in the flash of the partition 14 data to copy it to RAM */
  pharosPartition14DataSram2LoadAddr = LOADADDR(.partition14DataSram2);

  /* address in the flash of the partition 14 data to copy it to RAM */
  pharosPartition14DataSram3LoadAddr = LOADADDR(.partition14DataSram3);

  /* address in the flash of the partition 14 data to copy it to RAM */
  pharosPartition14DataSramBackupLoadAddr = LOADADDR(.partition14DataSramBackup);

  /* address in the flash of the partition 14 data to copy it to RAM */
  pharosPartition14DataSdramLoadAddr = LOADADDR(.partition14DataSdram);

  /* data of partition in Ccm */
  .partition14DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition14CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_14_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition14DataCcmStart = ABSOLUTE(.);

    *(.partition14DataCcm)
    *(.partition14DataCcm.*)

    /* address of the partition 14 data section end in RAM */
    . = ALIGN(8);
    pharosPartition14DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 14 in Ccm */
  .pharosPartition14BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 14 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition14BssCcmStart = .;

    *(.partition14BssCcm)
    *(.partition14BssCcm.*)

    /* address of the partition 14 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition14BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition14UssCcmStart = .;

    *(.partition14UssCcm)
    *(.partition14UssCcm.*)

    pharosPartition14UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_14_CCM_ALIGNMENT);

    /* address of the partition 14 sections end in Ccm */
    pharosPartition14CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition14DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition14Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_14_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition14DataSram1Start = ABSOLUTE(.);

    *(.partition14DataSram1)
    *(.partition14DataSram1.*)

    /* address of the partition 14 data section end in RAM */
    . = ALIGN(8);
    pharosPartition14DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 14 in Sram1 */
  .pharosPartition14BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 14 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition14BssSram1Start = .;

    *(.partition14BssSram1)
    *(.partition14BssSram1.*)

    /* address of the partition 14 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition14BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition14UssSram1Start = .;

    *(.partition14UssSram1)
    *(.partition14UssSram1.*)

    pharosPartition14UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_14_SRAM1_ALIGNMENT);

    /* address of the partition 14 sections end in Sram1 */
    pharosPartition14Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition14DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition14Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_14_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition14DataSram2Start = ABSOLUTE(.);

    *(.partition14DataSram2)
    *(.partition14DataSram2.*)

    /* address of the partition 14 data section end in RAM */
    . = ALIGN(8);
    pharosPartition14DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 14 in Sram2 */
  .pharosPartition14BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 14 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition14BssSram2Start = .;

    *(.partition14BssSram2)
    *(.partition14BssSram2.*)

    /* address of the partition 14 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition14BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition14UssSram2Start = .;

    *(.partition14UssSram2)
    *(.partition14UssSram2.*)

    pharosPartition14UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_14_SRAM2_ALIGNMENT);

    /* address of the partition 14 sections end in Sram2 */
    pharosPartition14Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition14DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition14Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_14_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition14DataSram3Start = ABSOLUTE(.);

    *(.partition14DataSram3)
    *(.partition14DataSram3.*)

    /* address of the partition 14 data section end in RAM */
    . = ALIGN(8);
    pharosPartition14DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 14 in Sram3 */
  .pharosPartition14BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 14 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition14BssSram3Start = .;

    *(.partition14BssSram3)
    *(.partition14BssSram3.*)

    /* address of the partition 14 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition14BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition14UssSram3Start = .;

    *(.partition14UssSram3)
    *(.partition14UssSram3.*)

    pharosPartition14UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_14_SRAM3_ALIGNMENT);

    /* address of the partition 14 sections end in Sram3 */
    pharosPartition14Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition14DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition14SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_14_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition14DataSramBackupStart = ABSOLUTE(.);

    *(.partition14DataSramBackup)
    *(.partition14DataSramBackup.*)

    /* address of the partition 14 data section end in RAM */
    . = ALIGN(8);
    pharosPartition14DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 14 in SramBackup */
  .pharosPartition14BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 14 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition14BssSramBackupStart = .;

    *(.partition14BssSramBackup)
    *(.partition14BssSramBackup.*)

    /* address of the partition 14 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition14BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition14UssSramBackupStart = .;

    *(.partition14UssSramBackup)
    *(.partition14UssSramBackup.*)

    pharosPartition14UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_14_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 14 sections end in SramBackup */
    pharosPartition14SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition14DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition14SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_14_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition14DataSdramStart = ABSOLUTE(.);

    *(.partition14DataSdram)
    *(.partition14DataSdram.*)

    /* address of the partition 14 data section end in RAM */
    . = ALIGN(8);
    pharosPartition14DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 14 in Sdram */
  .pharosPartition14BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 14 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition14BssSdramStart = .;

    *(.partition14BssSdram)
    *(.partition14BssSdram.*)

    /* address of the partition 14 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition14BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition14UssSdramStart = .;

    *(.partition14UssSdram)
    *(.partition14UssSdram.*)

    pharosPartition14UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_14_SDRAM_ALIGNMENT);

    /* address of the partition 14 sections end in Sdram */
    pharosPartition14SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 15                                                   */
  /******************************************************************/

  /* address in the flash of the partition 15 data to copy it to RAM */
  pharosPartition15DataCcmLoadAddr = LOADADDR(.partition15DataCcm);

  /* address in the flash of the partition 15 data to copy it to RAM */
  pharosPartition15DataSram1LoadAddr = LOADADDR(.partition15DataSram1);

  /* address in the flash of the partition 15 data to copy it to RAM */
  pharosPartition15DataSram2LoadAddr = LOADADDR(.partition15DataSram2);

  /* address in the flash of the partition 15 data to copy it to RAM */
  pharosPartition15DataSram3LoadAddr = LOADADDR(.partition15DataSram3);

  /* address in the flash of the partition 15 data to copy it to RAM */
  pharosPartition15DataSramBackupLoadAddr = LOADADDR(.partition15DataSramBackup);

  /* address in the flash of the partition 15 data to copy it to RAM */
  pharosPartition15DataSdramLoadAddr = LOADADDR(.partition15DataSdram);

  /* data of partition in Ccm */
  .partition15DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition15CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_15_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition15DataCcmStart = ABSOLUTE(.);

    *(.partition15DataCcm)
    *(.partition15DataCcm.*)

    /* address of the partition 15 data section end in RAM */
    . = ALIGN(8);
    pharosPartition15DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 15 in Ccm */
  .pharosPartition15BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 15 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition15BssCcmStart = .;

    *(.partition15BssCcm)
    *(.partition15BssCcm.*)

    /* address of the partition 15 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition15BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition15UssCcmStart = .;

    *(.partition15UssCcm)
    *(.partition15UssCcm.*)

    pharosPartition15UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_15_CCM_ALIGNMENT);

    /* address of the partition 15 sections end in Ccm */
    pharosPartition15CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition15DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition15Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_15_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition15DataSram1Start = ABSOLUTE(.);

    *(.partition15DataSram1)
    *(.partition15DataSram1.*)

    /* address of the partition 15 data section end in RAM */
    . = ALIGN(8);
    pharosPartition15DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 15 in Sram1 */
  .pharosPartition15BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 15 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition15BssSram1Start = .;

    *(.partition15BssSram1)
    *(.partition15BssSram1.*)

    /* address of the partition 15 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition15BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition15UssSram1Start = .;

    *(.partition15UssSram1)
    *(.partition15UssSram1.*)

    pharosPartition15UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_15_SRAM1_ALIGNMENT);

    /* address of the partition 15 sections end in Sram1 */
    pharosPartition15Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition15DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition15Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_15_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition15DataSram2Start = ABSOLUTE(.);

    *(.partition15DataSram2)
    *(.partition15DataSram2.*)

    /* address of the partition 15 data section end in RAM */
    . = ALIGN(8);
    pharosPartition15DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 15 in Sram2 */
  .pharosPartition15BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 15 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition15BssSram2Start = .;

    *(.partition15BssSram2)
    *(.partition15BssSram2.*)

    /* address of the partition 15 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition15BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition15UssSram2Start = .;

    *(.partition15UssSram2)
    *(.partition15UssSram2.*)

    pharosPartition15UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_15_SRAM2_ALIGNMENT);

    /* address of the partition 15 sections end in Sram2 */
    pharosPartition15Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition15DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition15Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_15_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition15DataSram3Start = ABSOLUTE(.);

    *(.partition15DataSram3)
    *(.partition15DataSram3.*)

    /* address of the partition 15 data section end in RAM */
    . = ALIGN(8);
    pharosPartition15DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 15 in Sram3 */
  .pharosPartition15BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 15 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition15BssSram3Start = .;

    *(.partition15BssSram3)
    *(.partition15BssSram3.*)

    /* address of the partition 15 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition15BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition15UssSram3Start = .;

    *(.partition15UssSram3)
    *(.partition15UssSram3.*)

    pharosPartition15UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_15_SRAM3_ALIGNMENT);

    /* address of the partition 15 sections end in Sram3 */
    pharosPartition15Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition15DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition15SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_15_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition15DataSramBackupStart = ABSOLUTE(.);

    *(.partition15DataSramBackup)
    *(.partition15DataSramBackup.*)

    /* address of the partition 15 data section end in RAM */
    . = ALIGN(8);
    pharosPartition15DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 15 in SramBackup */
  .pharosPartition15BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 15 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition15BssSramBackupStart = .;

    *(.partition15BssSramBackup)
    *(.partition15BssSramBackup.*)

    /* address of the partition 15 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition15BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition15UssSramBackupStart = .;

    *(.partition15UssSramBackup)
    *(.partition15UssSramBackup.*)

    pharosPartition15UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_15_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 15 sections end in SramBackup */
    pharosPartition15SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition15DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition15SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_15_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition15DataSdramStart = ABSOLUTE(.);

    *(.partition15DataSdram)
    *(.partition15DataSdram.*)

    /* address of the partition 15 data section end in RAM */
    . = ALIGN(8);
    pharosPartition15DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 15 in Sdram */
  .pharosPartition15BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 15 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition15BssSdramStart = .;

    *(.partition15BssSdram)
    *(.partition15BssSdram.*)

    /* address of the partition 15 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition15BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition15UssSdramStart = .;

    *(.partition15UssSdram)
    *(.partition15UssSdram.*)

    pharosPartition15UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_15_SDRAM_ALIGNMENT);

    /* address of the partition 15 sections end in Sdram */
    pharosPartition15SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 16                                                   */
  /******************************************************************/

  /* address in the flash of the partition 16 data to copy it to RAM */
  pharosPartition16DataCcmLoadAddr = LOADADDR(.partition16DataCcm);

  /* address in the flash of the partition 16 data to copy it to RAM */
  pharosPartition16DataSram1LoadAddr = LOADADDR(.partition16DataSram1);

  /* address in the flash of the partition 16 data to copy it to RAM */
  pharosPartition16DataSram2LoadAddr = LOADADDR(.partition16DataSram2);

  /* address in the flash of the partition 16 data to copy it to RAM */
  pharosPartition16DataSram3LoadAddr = LOADADDR(.partition16DataSram3);

  /* address in the flash of the partition 16 data to copy it to RAM */
  pharosPartition16DataSramBackupLoadAddr = LOADADDR(.partition16DataSramBackup);

  /* address in the flash of the partition 16 data to copy it to RAM */
  pharosPartition16DataSdramLoadAddr = LOADADDR(.partition16DataSdram);

  /* data of partition in Ccm */
  .partition16DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition16CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_16_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition16DataCcmStart = ABSOLUTE(.);

    *(.partition16DataCcm)
    *(.partition16DataCcm.*)

    /* address of the partition 16 data section end in RAM */
    . = ALIGN(8);
    pharosPartition16DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 16 in Ccm */
  .pharosPartition16BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 16 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition16BssCcmStart = .;

    *(.partition16BssCcm)
    *(.partition16BssCcm.*)

    /* address of the partition 16 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition16BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition16UssCcmStart = .;

    *(.partition16UssCcm)
    *(.partition16UssCcm.*)

    pharosPartition16UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_16_CCM_ALIGNMENT);

    /* address of the partition 16 sections end in Ccm */
    pharosPartition16CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition16DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition16Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_16_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition16DataSram1Start = ABSOLUTE(.);

    *(.partition16DataSram1)
    *(.partition16DataSram1.*)

    /* address of the partition 16 data section end in RAM */
    . = ALIGN(8);
    pharosPartition16DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 16 in Sram1 */
  .pharosPartition16BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 16 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition16BssSram1Start = .;

    *(.partition16BssSram1)
    *(.partition16BssSram1.*)

    /* address of the partition 16 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition16BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition16UssSram1Start = .;

    *(.partition16UssSram1)
    *(.partition16UssSram1.*)

    pharosPartition16UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_16_SRAM1_ALIGNMENT);

    /* address of the partition 16 sections end in Sram1 */
    pharosPartition16Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition16DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition16Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_16_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition16DataSram2Start = ABSOLUTE(.);

    *(.partition16DataSram2)
    *(.partition16DataSram2.*)

    /* address of the partition 16 data section end in RAM */
    . = ALIGN(8);
    pharosPartition16DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 16 in Sram2 */
  .pharosPartition16BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 16 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition16BssSram2Start = .;

    *(.partition16BssSram2)
    *(.partition16BssSram2.*)

    /* address of the partition 16 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition16BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition16UssSram2Start = .;

    *(.partition16UssSram2)
    *(.partition16UssSram2.*)

    pharosPartition16UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_16_SRAM2_ALIGNMENT);

    /* address of the partition 16 sections end in Sram2 */
    pharosPartition16Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition16DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition16Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_16_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition16DataSram3Start = ABSOLUTE(.);

    *(.partition16DataSram3)
    *(.partition16DataSram3.*)

    /* address of the partition 16 data section end in RAM */
    . = ALIGN(8);
    pharosPartition16DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 16 in Sram3 */
  .pharosPartition16BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 16 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition16BssSram3Start = .;

    *(.partition16BssSram3)
    *(.partition16BssSram3.*)

    /* address of the partition 16 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition16BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition16UssSram3Start = .;

    *(.partition16UssSram3)
    *(.partition16UssSram3.*)

    pharosPartition16UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_16_SRAM3_ALIGNMENT);

    /* address of the partition 16 sections end in Sram3 */
    pharosPartition16Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition16DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition16SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_16_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition16DataSramBackupStart = ABSOLUTE(.);

    *(.partition16DataSramBackup)
    *(.partition16DataSramBackup.*)

    /* address of the partition 16 data section end in RAM */
    . = ALIGN(8);
    pharosPartition16DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 16 in SramBackup */
  .pharosPartition16BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 16 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition16BssSramBackupStart = .;

    *(.partition16BssSramBackup)
    *(.partition16BssSramBackup.*)

    /* address of the partition 16 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition16BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition16UssSramBackupStart = .;

    *(.partition16UssSramBackup)
    *(.partition16UssSramBackup.*)

    pharosPartition16UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_16_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 16 sections end in SramBackup */
    pharosPartition16SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition16DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition16SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_16_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition16DataSdramStart = ABSOLUTE(.);

    *(.partition16DataSdram)
    *(.partition16DataSdram.*)

    /* address of the partition 16 data section end in RAM */
    . = ALIGN(8);
    pharosPartition16DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 16 in Sdram */
  .pharosPartition16BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 16 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition16BssSdramStart = .;

    *(.partition16BssSdram)
    *(.partition16BssSdram.*)

    /* address of the partition 16 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition16BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition16UssSdramStart = .;

    *(.partition16UssSdram)
    *(.partition16UssSdram.*)

    pharosPartition16UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_16_SDRAM_ALIGNMENT);

    /* address of the partition 16 sections end in Sdram */
    pharosPartition16SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 17                                                   */
  /******************************************************************/

  /* address in the flash of the partition 17 data to copy it to RAM */
  pharosPartition17DataCcmLoadAddr = LOADADDR(.partition17DataCcm);

  /* address in the flash of the partition 17 data to copy it to RAM */
  pharosPartition17DataSram1LoadAddr = LOADADDR(.partition17DataSram1);

  /* address in the flash of the partition 17 data to copy it to RAM */
  pharosPartition17DataSram2LoadAddr = LOADADDR(.partition17DataSram2);

  /* address in the flash of the partition 17 data to copy it to RAM */
  pharosPartition17DataSram3LoadAddr = LOADADDR(.partition17DataSram3);

  /* address in the flash of the partition 17 data to copy it to RAM */
  pharosPartition17DataSramBackupLoadAddr = LOADADDR(.partition17DataSramBackup);

  /* address in the flash of the partition 17 data to copy it to RAM */
  pharosPartition17DataSdramLoadAddr = LOADADDR(.partition17DataSdram);

  /* data of partition in Ccm */
  .partition17DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition17CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_17_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition17DataCcmStart = ABSOLUTE(.);

    *(.partition17DataCcm)
    *(.partition17DataCcm.*)

    /* address of the partition 17 data section end in RAM */
    . = ALIGN(8);
    pharosPartition17DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 17 in Ccm */
  .pharosPartition17BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 17 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition17BssCcmStart = .;

    *(.partition17BssCcm)
    *(.partition17BssCcm.*)

    /* address of the partition 17 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition17BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition17UssCcmStart = .;

    *(.partition17UssCcm)
    *(.partition17UssCcm.*)

    pharosPartition17UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_17_CCM_ALIGNMENT);

    /* address of the partition 17 sections end in Ccm */
    pharosPartition17CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition17DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition17Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_17_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition17DataSram1Start = ABSOLUTE(.);

    *(.partition17DataSram1)
    *(.partition17DataSram1.*)

    /* address of the partition 17 data section end in RAM */
    . = ALIGN(8);
    pharosPartition17DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 17 in Sram1 */
  .pharosPartition17BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 17 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition17BssSram1Start = .;

    *(.partition17BssSram1)
    *(.partition17BssSram1.*)

    /* address of the partition 17 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition17BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition17UssSram1Start = .;

    *(.partition17UssSram1)
    *(.partition17UssSram1.*)

    pharosPartition17UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_17_SRAM1_ALIGNMENT);

    /* address of the partition 17 sections end in Sram1 */
    pharosPartition17Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition17DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition17Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_17_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition17DataSram2Start = ABSOLUTE(.);

    *(.partition17DataSram2)
    *(.partition17DataSram2.*)

    /* address of the partition 17 data section end in RAM */
    . = ALIGN(8);
    pharosPartition17DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 17 in Sram2 */
  .pharosPartition17BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 17 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition17BssSram2Start = .;

    *(.partition17BssSram2)
    *(.partition17BssSram2.*)

    /* address of the partition 17 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition17BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition17UssSram2Start = .;

    *(.partition17UssSram2)
    *(.partition17UssSram2.*)

    pharosPartition17UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_17_SRAM2_ALIGNMENT);

    /* address of the partition 17 sections end in Sram2 */
    pharosPartition17Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition17DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition17Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_17_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition17DataSram3Start = ABSOLUTE(.);

    *(.partition17DataSram3)
    *(.partition17DataSram3.*)

    /* address of the partition 17 data section end in RAM */
    . = ALIGN(8);
    pharosPartition17DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 17 in Sram3 */
  .pharosPartition17BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 17 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition17BssSram3Start = .;

    *(.partition17BssSram3)
    *(.partition17BssSram3.*)

    /* address of the partition 17 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition17BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition17UssSram3Start = .;

    *(.partition17UssSram3)
    *(.partition17UssSram3.*)

    pharosPartition17UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_17_SRAM3_ALIGNMENT);

    /* address of the partition 17 sections end in Sram3 */
    pharosPartition17Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition17DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition17SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_17_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition17DataSramBackupStart = ABSOLUTE(.);

    *(.partition17DataSramBackup)
    *(.partition17DataSramBackup.*)

    /* address of the partition 17 data section end in RAM */
    . = ALIGN(8);
    pharosPartition17DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 17 in SramBackup */
  .pharosPartition17BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 17 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition17BssSramBackupStart = .;

    *(.partition17BssSramBackup)
    *(.partition17BssSramBackup.*)

    /* address of the partition 17 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition17BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition17UssSramBackupStart = .;

    *(.partition17UssSramBackup)
    *(.partition17UssSramBackup.*)

    pharosPartition17UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_17_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 17 sections end in SramBackup */
    pharosPartition17SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition17DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition17SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_17_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition17DataSdramStart = ABSOLUTE(.);

    *(.partition17DataSdram)
    *(.partition17DataSdram.*)

    /* address of the partition 17 data section end in RAM */
    . = ALIGN(8);
    pharosPartition17DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 17 in Sdram */
  .pharosPartition17BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 17 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition17BssSdramStart = .;

    *(.partition17BssSdram)
    *(.partition17BssSdram.*)

    /* address of the partition 17 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition17BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition17UssSdramStart = .;

    *(.partition17UssSdram)
    *(.partition17UssSdram.*)

    pharosPartition17UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_17_SDRAM_ALIGNMENT);

    /* address of the partition 17 sections end in Sdram */
    pharosPartition17SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 18                                                   */
  /******************************************************************/

  /* address in the flash of the partition 18 data to copy it to RAM */
  pharosPartition18DataCcmLoadAddr = LOADADDR(.partition18DataCcm);

  /* address in the flash of the partition 18 data to copy it to RAM */
  pharosPartition18DataSram1LoadAddr = LOADADDR(.partition18DataSram1);

  /* address in the flash of the partition 18 data to copy it to RAM */
  pharosPartition18DataSram2LoadAddr = LOADADDR(.partition18DataSram2);

  /* address in the flash of the partition 18 data to copy it to RAM */
  pharosPartition18DataSram3LoadAddr = LOADADDR(.partition18DataSram3);

  /* address in the flash of the partition 18 data to copy it to RAM */
  pharosPartition18DataSramBackupLoadAddr = LOADADDR(.partition18DataSramBackup);

  /* address in the flash of the partition 18 data to copy it to RAM */
  pharosPartition18DataSdramLoadAddr = LOADADDR(.partition18DataSdram);

  /* data of partition in Ccm */
  .partition18DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition18CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_18_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition18DataCcmStart = ABSOLUTE(.);

    *(.partition18DataCcm)
    *(.partition18DataCcm.*)

    /* address of the partition 18 data section end in RAM */
    . = ALIGN(8);
    pharosPartition18DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 18 in Ccm */
  .pharosPartition18BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 18 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition18BssCcmStart = .;

    *(.partition18BssCcm)
    *(.partition18BssCcm.*)

    /* address of the partition 18 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition18BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition18UssCcmStart = .;

    *(.partition18UssCcm)
    *(.partition18UssCcm.*)

    pharosPartition18UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_18_CCM_ALIGNMENT);

    /* address of the partition 18 sections end in Ccm */
    pharosPartition18CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition18DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition18Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_18_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition18DataSram1Start = ABSOLUTE(.);

    *(.partition18DataSram1)
    *(.partition18DataSram1.*)

    /* address of the partition 18 data section end in RAM */
    . = ALIGN(8);
    pharosPartition18DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 18 in Sram1 */
  .pharosPartition18BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 18 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition18BssSram1Start = .;

    *(.partition18BssSram1)
    *(.partition18BssSram1.*)

    /* address of the partition 18 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition18BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition18UssSram1Start = .;

    *(.partition18UssSram1)
    *(.partition18UssSram1.*)

    pharosPartition18UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_18_SRAM1_ALIGNMENT);

    /* address of the partition 18 sections end in Sram1 */
    pharosPartition18Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition18DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition18Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_18_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition18DataSram2Start = ABSOLUTE(.);

    *(.partition18DataSram2)
    *(.partition18DataSram2.*)

    /* address of the partition 18 data section end in RAM */
    . = ALIGN(8);
    pharosPartition18DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 18 in Sram2 */
  .pharosPartition18BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 18 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition18BssSram2Start = .;

    *(.partition18BssSram2)
    *(.partition18BssSram2.*)

    /* address of the partition 18 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition18BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition18UssSram2Start = .;

    *(.partition18UssSram2)
    *(.partition18UssSram2.*)

    pharosPartition18UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_18_SRAM2_ALIGNMENT);

    /* address of the partition 18 sections end in Sram2 */
    pharosPartition18Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition18DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition18Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_18_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition18DataSram3Start = ABSOLUTE(.);

    *(.partition18DataSram3)
    *(.partition18DataSram3.*)

    /* address of the partition 18 data section end in RAM */
    . = ALIGN(8);
    pharosPartition18DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 18 in Sram3 */
  .pharosPartition18BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 18 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition18BssSram3Start = .;

    *(.partition18BssSram3)
    *(.partition18BssSram3.*)

    /* address of the partition 18 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition18BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition18UssSram3Start = .;

    *(.partition18UssSram3)
    *(.partition18UssSram3.*)

    pharosPartition18UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_18_SRAM3_ALIGNMENT);

    /* address of the partition 18 sections end in Sram3 */
    pharosPartition18Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition18DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition18SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_18_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition18DataSramBackupStart = ABSOLUTE(.);

    *(.partition18DataSramBackup)
    *(.partition18DataSramBackup.*)

    /* address of the partition 18 data section end in RAM */
    . = ALIGN(8);
    pharosPartition18DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 18 in SramBackup */
  .pharosPartition18BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 18 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition18BssSramBackupStart = .;

    *(.partition18BssSramBackup)
    *(.partition18BssSramBackup.*)

    /* address of the partition 18 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition18BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition18UssSramBackupStart = .;

    *(.partition18UssSramBackup)
    *(.partition18UssSramBackup.*)

    pharosPartition18UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_18_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 18 sections end in SramBackup */
    pharosPartition18SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition18DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition18SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_18_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition18DataSdramStart = ABSOLUTE(.);

    *(.partition18DataSdram)
    *(.partition18DataSdram.*)

    /* address of the partition 18 data section end in RAM */
    . = ALIGN(8);
    pharosPartition18DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 18 in Sdram */
  .pharosPartition18BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 18 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition18BssSdramStart = .;

    *(.partition18BssSdram)
    *(.partition18BssSdram.*)

    /* address of the partition 18 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition18BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition18UssSdramStart = .;

    *(.partition18UssSdram)
    *(.partition18UssSdram.*)

    pharosPartition18UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_18_SDRAM_ALIGNMENT);

    /* address of the partition 18 sections end in Sdram */
    pharosPartition18SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 19                                                   */
  /******************************************************************/

  /* address in the flash of the partition 19 data to copy it to RAM */
  pharosPartition19DataCcmLoadAddr = LOADADDR(.partition19DataCcm);

  /* address in the flash of the partition 19 data to copy it to RAM */
  pharosPartition19DataSram1LoadAddr = LOADADDR(.partition19DataSram1);

  /* address in the flash of the partition 19 data to copy it to RAM */
  pharosPartition19DataSram2LoadAddr = LOADADDR(.partition19DataSram2);

  /* address in the flash of the partition 19 data to copy it to RAM */
  pharosPartition19DataSram3LoadAddr = LOADADDR(.partition19DataSram3);

  /* address in the flash of the partition 19 data to copy it to RAM */
  pharosPartition19DataSramBackupLoadAddr = LOADADDR(.partition19DataSramBackup);

  /* address in the flash of the partition 19 data to copy it to RAM */
  pharosPartition19DataSdramLoadAddr = LOADADDR(.partition19DataSdram);

  /* data of partition in Ccm */
  .partition19DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition19CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_19_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition19DataCcmStart = ABSOLUTE(.);

    *(.partition19DataCcm)
    *(.partition19DataCcm.*)

    /* address of the partition 19 data section end in RAM */
    . = ALIGN(8);
    pharosPartition19DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 19 in Ccm */
  .pharosPartition19BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 19 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition19BssCcmStart = .;

    *(.partition19BssCcm)
    *(.partition19BssCcm.*)

    /* address of the partition 19 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition19BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition19UssCcmStart = .;

    *(.partition19UssCcm)
    *(.partition19UssCcm.*)

    pharosPartition19UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_19_CCM_ALIGNMENT);

    /* address of the partition 19 sections end in Ccm */
    pharosPartition19CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition19DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition19Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_19_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition19DataSram1Start = ABSOLUTE(.);

    *(.partition19DataSram1)
    *(.partition19DataSram1.*)

    /* address of the partition 19 data section end in RAM */
    . = ALIGN(8);
    pharosPartition19DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 19 in Sram1 */
  .pharosPartition19BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 19 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition19BssSram1Start = .;

    *(.partition19BssSram1)
    *(.partition19BssSram1.*)

    /* address of the partition 19 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition19BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition19UssSram1Start = .;

    *(.partition19UssSram1)
    *(.partition19UssSram1.*)

    pharosPartition19UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_19_SRAM1_ALIGNMENT);

    /* address of the partition 19 sections end in Sram1 */
    pharosPartition19Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition19DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition19Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_19_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition19DataSram2Start = ABSOLUTE(.);

    *(.partition19DataSram2)
    *(.partition19DataSram2.*)

    /* address of the partition 19 data section end in RAM */
    . = ALIGN(8);
    pharosPartition19DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 19 in Sram2 */
  .pharosPartition19BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 19 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition19BssSram2Start = .;

    *(.partition19BssSram2)
    *(.partition19BssSram2.*)

    /* address of the partition 19 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition19BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition19UssSram2Start = .;

    *(.partition19UssSram2)
    *(.partition19UssSram2.*)

    pharosPartition19UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_19_SRAM2_ALIGNMENT);

    /* address of the partition 19 sections end in Sram2 */
    pharosPartition19Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition19DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition19Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_19_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition19DataSram3Start = ABSOLUTE(.);

    *(.partition19DataSram3)
    *(.partition19DataSram3.*)

    /* address of the partition 19 data section end in RAM */
    . = ALIGN(8);
    pharosPartition19DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 19 in Sram3 */
  .pharosPartition19BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 19 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition19BssSram3Start = .;

    *(.partition19BssSram3)
    *(.partition19BssSram3.*)

    /* address of the partition 19 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition19BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition19UssSram3Start = .;

    *(.partition19UssSram3)
    *(.partition19UssSram3.*)

    pharosPartition19UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_19_SRAM3_ALIGNMENT);

    /* address of the partition 19 sections end in Sram3 */
    pharosPartition19Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition19DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition19SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_19_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition19DataSramBackupStart = ABSOLUTE(.);

    *(.partition19DataSramBackup)
    *(.partition19DataSramBackup.*)

    /* address of the partition 19 data section end in RAM */
    . = ALIGN(8);
    pharosPartition19DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 19 in SramBackup */
  .pharosPartition19BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 19 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition19BssSramBackupStart = .;

    *(.partition19BssSramBackup)
    *(.partition19BssSramBackup.*)

    /* address of the partition 19 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition19BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition19UssSramBackupStart = .;

    *(.partition19UssSramBackup)
    *(.partition19UssSramBackup.*)

    pharosPartition19UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_19_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 19 sections end in SramBackup */
    pharosPartition19SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition19DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition19SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_19_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition19DataSdramStart = ABSOLUTE(.);

    *(.partition19DataSdram)
    *(.partition19DataSdram.*)

    /* address of the partition 19 data section end in RAM */
    . = ALIGN(8);
    pharosPartition19DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 19 in Sdram */
  .pharosPartition19BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 19 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition19BssSdramStart = .;

    *(.partition19BssSdram)
    *(.partition19BssSdram.*)

    /* address of the partition 19 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition19BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition19UssSdramStart = .;

    *(.partition19UssSdram)
    *(.partition19UssSdram.*)

    pharosPartition19UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_19_SDRAM_ALIGNMENT);

    /* address of the partition 19 sections end in Sdram */
    pharosPartition19SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 20                                                   */
  /******************************************************************/

  /* address in the flash of the partition 20 data to copy it to RAM */
  pharosPartition20DataCcmLoadAddr = LOADADDR(.partition20DataCcm);

  /* address in the flash of the partition 20 data to copy it to RAM */
  pharosPartition20DataSram1LoadAddr = LOADADDR(.partition20DataSram1);

  /* address in the flash of the partition 20 data to copy it to RAM */
  pharosPartition20DataSram2LoadAddr = LOADADDR(.partition20DataSram2);

  /* address in the flash of the partition 20 data to copy it to RAM */
  pharosPartition20DataSram3LoadAddr = LOADADDR(.partition20DataSram3);

  /* address in the flash of the partition 20 data to copy it to RAM */
  pharosPartition20DataSramBackupLoadAddr = LOADADDR(.partition20DataSramBackup);

  /* address in the flash of the partition 20 data to copy it to RAM */
  pharosPartition20DataSdramLoadAddr = LOADADDR(.partition20DataSdram);

  /* data of partition in Ccm */
  .partition20DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition20CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_20_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition20DataCcmStart = ABSOLUTE(.);

    *(.partition20DataCcm)
    *(.partition20DataCcm.*)

    /* address of the partition 20 data section end in RAM */
    . = ALIGN(8);
    pharosPartition20DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 20 in Ccm */
  .pharosPartition20BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 20 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition20BssCcmStart = .;

    *(.partition20BssCcm)
    *(.partition20BssCcm.*)

    /* address of the partition 20 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition20BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition20UssCcmStart = .;

    *(.partition20UssCcm)
    *(.partition20UssCcm.*)

    pharosPartition20UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_20_CCM_ALIGNMENT);

    /* address of the partition 20 sections end in Ccm */
    pharosPartition20CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition20DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition20Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_20_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition20DataSram1Start = ABSOLUTE(.);

    *(.partition20DataSram1)
    *(.partition20DataSram1.*)

    /* address of the partition 20 data section end in RAM */
    . = ALIGN(8);
    pharosPartition20DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 20 in Sram1 */
  .pharosPartition20BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 20 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition20BssSram1Start = .;

    *(.partition20BssSram1)
    *(.partition20BssSram1.*)

    /* address of the partition 20 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition20BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition20UssSram1Start = .;

    *(.partition20UssSram1)
    *(.partition20UssSram1.*)

    pharosPartition20UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_20_SRAM1_ALIGNMENT);

    /* address of the partition 20 sections end in Sram1 */
    pharosPartition20Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition20DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition20Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_20_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition20DataSram2Start = ABSOLUTE(.);

    *(.partition20DataSram2)
    *(.partition20DataSram2.*)

    /* address of the partition 20 data section end in RAM */
    . = ALIGN(8);
    pharosPartition20DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 20 in Sram2 */
  .pharosPartition20BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 20 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition20BssSram2Start = .;

    *(.partition20BssSram2)
    *(.partition20BssSram2.*)

    /* address of the partition 20 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition20BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition20UssSram2Start = .;

    *(.partition20UssSram2)
    *(.partition20UssSram2.*)

    pharosPartition20UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_20_SRAM2_ALIGNMENT);

    /* address of the partition 20 sections end in Sram2 */
    pharosPartition20Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition20DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition20Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_20_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition20DataSram3Start = ABSOLUTE(.);

    *(.partition20DataSram3)
    *(.partition20DataSram3.*)

    /* address of the partition 20 data section end in RAM */
    . = ALIGN(8);
    pharosPartition20DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 20 in Sram3 */
  .pharosPartition20BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 20 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition20BssSram3Start = .;

    *(.partition20BssSram3)
    *(.partition20BssSram3.*)

    /* address of the partition 20 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition20BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition20UssSram3Start = .;

    *(.partition20UssSram3)
    *(.partition20UssSram3.*)

    pharosPartition20UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_20_SRAM3_ALIGNMENT);

    /* address of the partition 20 sections end in Sram3 */
    pharosPartition20Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition20DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition20SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_20_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition20DataSramBackupStart = ABSOLUTE(.);

    *(.partition20DataSramBackup)
    *(.partition20DataSramBackup.*)

    /* address of the partition 20 data section end in RAM */
    . = ALIGN(8);
    pharosPartition20DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 20 in SramBackup */
  .pharosPartition20BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 20 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition20BssSramBackupStart = .;

    *(.partition20BssSramBackup)
    *(.partition20BssSramBackup.*)

    /* address of the partition 20 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition20BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition20UssSramBackupStart = .;

    *(.partition20UssSramBackup)
    *(.partition20UssSramBackup.*)

    pharosPartition20UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_20_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 20 sections end in SramBackup */
    pharosPartition20SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition20DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition20SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_20_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition20DataSdramStart = ABSOLUTE(.);

    *(.partition20DataSdram)
    *(.partition20DataSdram.*)

    /* address of the partition 20 data section end in RAM */
    . = ALIGN(8);
    pharosPartition20DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 20 in Sdram */
  .pharosPartition20BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 20 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition20BssSdramStart = .;

    *(.partition20BssSdram)
    *(.partition20BssSdram.*)

    /* address of the partition 20 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition20BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition20UssSdramStart = .;

    *(.partition20UssSdram)
    *(.partition20UssSdram.*)

    pharosPartition20UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_20_SDRAM_ALIGNMENT);

    /* address of the partition 20 sections end in Sdram */
    pharosPartition20SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 21                                                   */
  /******************************************************************/

  /* address in the flash of the partition 21 data to copy it to RAM */
  pharosPartition21DataCcmLoadAddr = LOADADDR(.partition21DataCcm);

  /* address in the flash of the partition 21 data to copy it to RAM */
  pharosPartition21DataSram1LoadAddr = LOADADDR(.partition21DataSram1);

  /* address in the flash of the partition 21 data to copy it to RAM */
  pharosPartition21DataSram2LoadAddr = LOADADDR(.partition21DataSram2);

  /* address in the flash of the partition 21 data to copy it to RAM */
  pharosPartition21DataSram3LoadAddr = LOADADDR(.partition21DataSram3);

  /* address in the flash of the partition 21 data to copy it to RAM */
  pharosPartition21DataSramBackupLoadAddr = LOADADDR(.partition21DataSramBackup);

  /* address in the flash of the partition 21 data to copy it to RAM */
  pharosPartition21DataSdramLoadAddr = LOADADDR(.partition21DataSdram);

  /* data of partition in Ccm */
  .partition21DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition21CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_21_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition21DataCcmStart = ABSOLUTE(.);

    *(.partition21DataCcm)
    *(.partition21DataCcm.*)

    /* address of the partition 21 data section end in RAM */
    . = ALIGN(8);
    pharosPartition21DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 21 in Ccm */
  .pharosPartition21BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 21 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition21BssCcmStart = .;

    *(.partition21BssCcm)
    *(.partition21BssCcm.*)

    /* address of the partition 21 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition21BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition21UssCcmStart = .;

    *(.partition21UssCcm)
    *(.partition21UssCcm.*)

    pharosPartition21UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_21_CCM_ALIGNMENT);

    /* address of the partition 21 sections end in Ccm */
    pharosPartition21CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition21DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition21Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_21_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition21DataSram1Start = ABSOLUTE(.);

    *(.partition21DataSram1)
    *(.partition21DataSram1.*)

    /* address of the partition 21 data section end in RAM */
    . = ALIGN(8);
    pharosPartition21DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 21 in Sram1 */
  .pharosPartition21BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 21 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition21BssSram1Start = .;

    *(.partition21BssSram1)
    *(.partition21BssSram1.*)

    /* address of the partition 21 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition21BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition21UssSram1Start = .;

    *(.partition21UssSram1)
    *(.partition21UssSram1.*)

    pharosPartition21UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_21_SRAM1_ALIGNMENT);

    /* address of the partition 21 sections end in Sram1 */
    pharosPartition21Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition21DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition21Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_21_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition21DataSram2Start = ABSOLUTE(.);

    *(.partition21DataSram2)
    *(.partition21DataSram2.*)

    /* address of the partition 21 data section end in RAM */
    . = ALIGN(8);
    pharosPartition21DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 21 in Sram2 */
  .pharosPartition21BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 21 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition21BssSram2Start = .;

    *(.partition21BssSram2)
    *(.partition21BssSram2.*)

    /* address of the partition 21 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition21BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition21UssSram2Start = .;

    *(.partition21UssSram2)
    *(.partition21UssSram2.*)

    pharosPartition21UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_21_SRAM2_ALIGNMENT);

    /* address of the partition 21 sections end in Sram2 */
    pharosPartition21Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition21DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition21Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_21_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition21DataSram3Start = ABSOLUTE(.);

    *(.partition21DataSram3)
    *(.partition21DataSram3.*)

    /* address of the partition 21 data section end in RAM */
    . = ALIGN(8);
    pharosPartition21DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 21 in Sram3 */
  .pharosPartition21BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 21 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition21BssSram3Start = .;

    *(.partition21BssSram3)
    *(.partition21BssSram3.*)

    /* address of the partition 21 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition21BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition21UssSram3Start = .;

    *(.partition21UssSram3)
    *(.partition21UssSram3.*)

    pharosPartition21UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_21_SRAM3_ALIGNMENT);

    /* address of the partition 21 sections end in Sram3 */
    pharosPartition21Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition21DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition21SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_21_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition21DataSramBackupStart = ABSOLUTE(.);

    *(.partition21DataSramBackup)
    *(.partition21DataSramBackup.*)

    /* address of the partition 21 data section end in RAM */
    . = ALIGN(8);
    pharosPartition21DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 21 in SramBackup */
  .pharosPartition21BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 21 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition21BssSramBackupStart = .;

    *(.partition21BssSramBackup)
    *(.partition21BssSramBackup.*)

    /* address of the partition 21 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition21BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition21UssSramBackupStart = .;

    *(.partition21UssSramBackup)
    *(.partition21UssSramBackup.*)

    pharosPartition21UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_21_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 21 sections end in SramBackup */
    pharosPartition21SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition21DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition21SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_21_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition21DataSdramStart = ABSOLUTE(.);

    *(.partition21DataSdram)
    *(.partition21DataSdram.*)

    /* address of the partition 21 data section end in RAM */
    . = ALIGN(8);
    pharosPartition21DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 21 in Sdram */
  .pharosPartition21BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 21 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition21BssSdramStart = .;

    *(.partition21BssSdram)
    *(.partition21BssSdram.*)

    /* address of the partition 21 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition21BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition21UssSdramStart = .;

    *(.partition21UssSdram)
    *(.partition21UssSdram.*)

    pharosPartition21UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_21_SDRAM_ALIGNMENT);

    /* address of the partition 21 sections end in Sdram */
    pharosPartition21SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 22                                                   */
  /******************************************************************/

  /* address in the flash of the partition 22 data to copy it to RAM */
  pharosPartition22DataCcmLoadAddr = LOADADDR(.partition22DataCcm);

  /* address in the flash of the partition 22 data to copy it to RAM */
  pharosPartition22DataSram1LoadAddr = LOADADDR(.partition22DataSram1);

  /* address in the flash of the partition 22 data to copy it to RAM */
  pharosPartition22DataSram2LoadAddr = LOADADDR(.partition22DataSram2);

  /* address in the flash of the partition 22 data to copy it to RAM */
  pharosPartition22DataSram3LoadAddr = LOADADDR(.partition22DataSram3);

  /* address in the flash of the partition 22 data to copy it to RAM */
  pharosPartition22DataSramBackupLoadAddr = LOADADDR(.partition22DataSramBackup);

  /* address in the flash of the partition 22 data to copy it to RAM */
  pharosPartition22DataSdramLoadAddr = LOADADDR(.partition22DataSdram);

  /* data of partition in Ccm */
  .partition22DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition22CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_22_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition22DataCcmStart = ABSOLUTE(.);

    *(.partition22DataCcm)
    *(.partition22DataCcm.*)

    /* address of the partition 22 data section end in RAM */
    . = ALIGN(8);
    pharosPartition22DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 22 in Ccm */
  .pharosPartition22BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 22 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition22BssCcmStart = .;

    *(.partition22BssCcm)
    *(.partition22BssCcm.*)

    /* address of the partition 22 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition22BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition22UssCcmStart = .;

    *(.partition22UssCcm)
    *(.partition22UssCcm.*)

    pharosPartition22UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_22_CCM_ALIGNMENT);

    /* address of the partition 22 sections end in Ccm */
    pharosPartition22CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition22DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition22Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_22_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition22DataSram1Start = ABSOLUTE(.);

    *(.partition22DataSram1)
    *(.partition22DataSram1.*)

    /* address of the partition 22 data section end in RAM */
    . = ALIGN(8);
    pharosPartition22DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 22 in Sram1 */
  .pharosPartition22BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 22 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition22BssSram1Start = .;

    *(.partition22BssSram1)
    *(.partition22BssSram1.*)

    /* address of the partition 22 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition22BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition22UssSram1Start = .;

    *(.partition22UssSram1)
    *(.partition22UssSram1.*)

    pharosPartition22UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_22_SRAM1_ALIGNMENT);

    /* address of the partition 22 sections end in Sram1 */
    pharosPartition22Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition22DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition22Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_22_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition22DataSram2Start = ABSOLUTE(.);

    *(.partition22DataSram2)
    *(.partition22DataSram2.*)

    /* address of the partition 22 data section end in RAM */
    . = ALIGN(8);
    pharosPartition22DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 22 in Sram2 */
  .pharosPartition22BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 22 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition22BssSram2Start = .;

    *(.partition22BssSram2)
    *(.partition22BssSram2.*)

    /* address of the partition 22 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition22BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition22UssSram2Start = .;

    *(.partition22UssSram2)
    *(.partition22UssSram2.*)

    pharosPartition22UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_22_SRAM2_ALIGNMENT);

    /* address of the partition 22 sections end in Sram2 */
    pharosPartition22Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition22DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition22Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_22_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition22DataSram3Start = ABSOLUTE(.);

    *(.partition22DataSram3)
    *(.partition22DataSram3.*)

    /* address of the partition 22 data section end in RAM */
    . = ALIGN(8);
    pharosPartition22DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 22 in Sram3 */
  .pharosPartition22BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 22 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition22BssSram3Start = .;

    *(.partition22BssSram3)
    *(.partition22BssSram3.*)

    /* address of the partition 22 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition22BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition22UssSram3Start = .;

    *(.partition22UssSram3)
    *(.partition22UssSram3.*)

    pharosPartition22UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_22_SRAM3_ALIGNMENT);

    /* address of the partition 22 sections end in Sram3 */
    pharosPartition22Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition22DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition22SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_22_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition22DataSramBackupStart = ABSOLUTE(.);

    *(.partition22DataSramBackup)
    *(.partition22DataSramBackup.*)

    /* address of the partition 22 data section end in RAM */
    . = ALIGN(8);
    pharosPartition22DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 22 in SramBackup */
  .pharosPartition22BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 22 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition22BssSramBackupStart = .;

    *(.partition22BssSramBackup)
    *(.partition22BssSramBackup.*)

    /* address of the partition 22 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition22BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition22UssSramBackupStart = .;

    *(.partition22UssSramBackup)
    *(.partition22UssSramBackup.*)

    pharosPartition22UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_22_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 22 sections end in SramBackup */
    pharosPartition22SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition22DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition22SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_22_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition22DataSdramStart = ABSOLUTE(.);

    *(.partition22DataSdram)
    *(.partition22DataSdram.*)

    /* address of the partition 22 data section end in RAM */
    . = ALIGN(8);
    pharosPartition22DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 22 in Sdram */
  .pharosPartition22BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 22 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition22BssSdramStart = .;

    *(.partition22BssSdram)
    *(.partition22BssSdram.*)

    /* address of the partition 22 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition22BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition22UssSdramStart = .;

    *(.partition22UssSdram)
    *(.partition22UssSdram.*)

    pharosPartition22UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_22_SDRAM_ALIGNMENT);

    /* address of the partition 22 sections end in Sdram */
    pharosPartition22SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 23                                                   */
  /******************************************************************/

  /* address in the flash of the partition 23 data to copy it to RAM */
  pharosPartition23DataCcmLoadAddr = LOADADDR(.partition23DataCcm);

  /* address in the flash of the partition 23 data to copy it to RAM */
  pharosPartition23DataSram1LoadAddr = LOADADDR(.partition23DataSram1);

  /* address in the flash of the partition 23 data to copy it to RAM */
  pharosPartition23DataSram2LoadAddr = LOADADDR(.partition23DataSram2);

  /* address in the flash of the partition 23 data to copy it to RAM */
  pharosPartition23DataSram3LoadAddr = LOADADDR(.partition23DataSram3);

  /* address in the flash of the partition 23 data to copy it to RAM */
  pharosPartition23DataSramBackupLoadAddr = LOADADDR(.partition23DataSramBackup);

  /* address in the flash of the partition 23 data to copy it to RAM */
  pharosPartition23DataSdramLoadAddr = LOADADDR(.partition23DataSdram);

  /* data of partition in Ccm */
  .partition23DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition23CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_23_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition23DataCcmStart = ABSOLUTE(.);

    *(.partition23DataCcm)
    *(.partition23DataCcm.*)

    /* address of the partition 23 data section end in RAM */
    . = ALIGN(8);
    pharosPartition23DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 23 in Ccm */
  .pharosPartition23BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 23 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition23BssCcmStart = .;

    *(.partition23BssCcm)
    *(.partition23BssCcm.*)

    /* address of the partition 23 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition23BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition23UssCcmStart = .;

    *(.partition23UssCcm)
    *(.partition23UssCcm.*)

    pharosPartition23UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_23_CCM_ALIGNMENT);

    /* address of the partition 23 sections end in Ccm */
    pharosPartition23CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition23DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition23Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_23_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition23DataSram1Start = ABSOLUTE(.);

    *(.partition23DataSram1)
    *(.partition23DataSram1.*)

    /* address of the partition 23 data section end in RAM */
    . = ALIGN(8);
    pharosPartition23DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 23 in Sram1 */
  .pharosPartition23BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 23 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition23BssSram1Start = .;

    *(.partition23BssSram1)
    *(.partition23BssSram1.*)

    /* address of the partition 23 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition23BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition23UssSram1Start = .;

    *(.partition23UssSram1)
    *(.partition23UssSram1.*)

    pharosPartition23UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_23_SRAM1_ALIGNMENT);

    /* address of the partition 23 sections end in Sram1 */
    pharosPartition23Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition23DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition23Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_23_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition23DataSram2Start = ABSOLUTE(.);

    *(.partition23DataSram2)
    *(.partition23DataSram2.*)

    /* address of the partition 23 data section end in RAM */
    . = ALIGN(8);
    pharosPartition23DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 23 in Sram2 */
  .pharosPartition23BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 23 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition23BssSram2Start = .;

    *(.partition23BssSram2)
    *(.partition23BssSram2.*)

    /* address of the partition 23 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition23BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition23UssSram2Start = .;

    *(.partition23UssSram2)
    *(.partition23UssSram2.*)

    pharosPartition23UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_23_SRAM2_ALIGNMENT);

    /* address of the partition 23 sections end in Sram2 */
    pharosPartition23Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition23DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition23Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_23_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition23DataSram3Start = ABSOLUTE(.);

    *(.partition23DataSram3)
    *(.partition23DataSram3.*)

    /* address of the partition 23 data section end in RAM */
    . = ALIGN(8);
    pharosPartition23DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 23 in Sram3 */
  .pharosPartition23BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 23 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition23BssSram3Start = .;

    *(.partition23BssSram3)
    *(.partition23BssSram3.*)

    /* address of the partition 23 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition23BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition23UssSram3Start = .;

    *(.partition23UssSram3)
    *(.partition23UssSram3.*)

    pharosPartition23UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_23_SRAM3_ALIGNMENT);

    /* address of the partition 23 sections end in Sram3 */
    pharosPartition23Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition23DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition23SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_23_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition23DataSramBackupStart = ABSOLUTE(.);

    *(.partition23DataSramBackup)
    *(.partition23DataSramBackup.*)

    /* address of the partition 23 data section end in RAM */
    . = ALIGN(8);
    pharosPartition23DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 23 in SramBackup */
  .pharosPartition23BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 23 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition23BssSramBackupStart = .;

    *(.partition23BssSramBackup)
    *(.partition23BssSramBackup.*)

    /* address of the partition 23 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition23BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition23UssSramBackupStart = .;

    *(.partition23UssSramBackup)
    *(.partition23UssSramBackup.*)

    pharosPartition23UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_23_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 23 sections end in SramBackup */
    pharosPartition23SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition23DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition23SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_23_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition23DataSdramStart = ABSOLUTE(.);

    *(.partition23DataSdram)
    *(.partition23DataSdram.*)

    /* address of the partition 23 data section end in RAM */
    . = ALIGN(8);
    pharosPartition23DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 23 in Sdram */
  .pharosPartition23BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 23 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition23BssSdramStart = .;

    *(.partition23BssSdram)
    *(.partition23BssSdram.*)

    /* address of the partition 23 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition23BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition23UssSdramStart = .;

    *(.partition23UssSdram)
    *(.partition23UssSdram.*)

    pharosPartition23UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_23_SDRAM_ALIGNMENT);

    /* address of the partition 23 sections end in Sdram */
    pharosPartition23SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 24                                                   */
  /******************************************************************/

  /* address in the flash of the partition 24 data to copy it to RAM */
  pharosPartition24DataCcmLoadAddr = LOADADDR(.partition24DataCcm);

  /* address in the flash of the partition 24 data to copy it to RAM */
  pharosPartition24DataSram1LoadAddr = LOADADDR(.partition24DataSram1);

  /* address in the flash of the partition 24 data to copy it to RAM */
  pharosPartition24DataSram2LoadAddr = LOADADDR(.partition24DataSram2);

  /* address in the flash of the partition 24 data to copy it to RAM */
  pharosPartition24DataSram3LoadAddr = LOADADDR(.partition24DataSram3);

  /* address in the flash of the partition 24 data to copy it to RAM */
  pharosPartition24DataSramBackupLoadAddr = LOADADDR(.partition24DataSramBackup);

  /* address in the flash of the partition 24 data to copy it to RAM */
  pharosPartition24DataSdramLoadAddr = LOADADDR(.partition24DataSdram);

  /* data of partition in Ccm */
  .partition24DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition24CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_24_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition24DataCcmStart = ABSOLUTE(.);

    *(.partition24DataCcm)
    *(.partition24DataCcm.*)

    /* address of the partition 24 data section end in RAM */
    . = ALIGN(8);
    pharosPartition24DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 24 in Ccm */
  .pharosPartition24BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 24 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition24BssCcmStart = .;

    *(.partition24BssCcm)
    *(.partition24BssCcm.*)

    /* address of the partition 24 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition24BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition24UssCcmStart = .;

    *(.partition24UssCcm)
    *(.partition24UssCcm.*)

    pharosPartition24UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_24_CCM_ALIGNMENT);

    /* address of the partition 24 sections end in Ccm */
    pharosPartition24CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition24DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition24Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_24_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition24DataSram1Start = ABSOLUTE(.);

    *(.partition24DataSram1)
    *(.partition24DataSram1.*)

    /* address of the partition 24 data section end in RAM */
    . = ALIGN(8);
    pharosPartition24DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 24 in Sram1 */
  .pharosPartition24BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 24 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition24BssSram1Start = .;

    *(.partition24BssSram1)
    *(.partition24BssSram1.*)

    /* address of the partition 24 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition24BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition24UssSram1Start = .;

    *(.partition24UssSram1)
    *(.partition24UssSram1.*)

    pharosPartition24UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_24_SRAM1_ALIGNMENT);

    /* address of the partition 24 sections end in Sram1 */
    pharosPartition24Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition24DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition24Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_24_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition24DataSram2Start = ABSOLUTE(.);

    *(.partition24DataSram2)
    *(.partition24DataSram2.*)

    /* address of the partition 24 data section end in RAM */
    . = ALIGN(8);
    pharosPartition24DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 24 in Sram2 */
  .pharosPartition24BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 24 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition24BssSram2Start = .;

    *(.partition24BssSram2)
    *(.partition24BssSram2.*)

    /* address of the partition 24 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition24BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition24UssSram2Start = .;

    *(.partition24UssSram2)
    *(.partition24UssSram2.*)

    pharosPartition24UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_24_SRAM2_ALIGNMENT);

    /* address of the partition 24 sections end in Sram2 */
    pharosPartition24Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition24DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition24Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_24_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition24DataSram3Start = ABSOLUTE(.);

    *(.partition24DataSram3)
    *(.partition24DataSram3.*)

    /* address of the partition 24 data section end in RAM */
    . = ALIGN(8);
    pharosPartition24DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 24 in Sram3 */
  .pharosPartition24BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 24 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition24BssSram3Start = .;

    *(.partition24BssSram3)
    *(.partition24BssSram3.*)

    /* address of the partition 24 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition24BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition24UssSram3Start = .;

    *(.partition24UssSram3)
    *(.partition24UssSram3.*)

    pharosPartition24UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_24_SRAM3_ALIGNMENT);

    /* address of the partition 24 sections end in Sram3 */
    pharosPartition24Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition24DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition24SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_24_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition24DataSramBackupStart = ABSOLUTE(.);

    *(.partition24DataSramBackup)
    *(.partition24DataSramBackup.*)

    /* address of the partition 24 data section end in RAM */
    . = ALIGN(8);
    pharosPartition24DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 24 in SramBackup */
  .pharosPartition24BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 24 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition24BssSramBackupStart = .;

    *(.partition24BssSramBackup)
    *(.partition24BssSramBackup.*)

    /* address of the partition 24 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition24BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition24UssSramBackupStart = .;

    *(.partition24UssSramBackup)
    *(.partition24UssSramBackup.*)

    pharosPartition24UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_24_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 24 sections end in SramBackup */
    pharosPartition24SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition24DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition24SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_24_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition24DataSdramStart = ABSOLUTE(.);

    *(.partition24DataSdram)
    *(.partition24DataSdram.*)

    /* address of the partition 24 data section end in RAM */
    . = ALIGN(8);
    pharosPartition24DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 24 in Sdram */
  .pharosPartition24BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 24 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition24BssSdramStart = .;

    *(.partition24BssSdram)
    *(.partition24BssSdram.*)

    /* address of the partition 24 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition24BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition24UssSdramStart = .;

    *(.partition24UssSdram)
    *(.partition24UssSdram.*)

    pharosPartition24UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_24_SDRAM_ALIGNMENT);

    /* address of the partition 24 sections end in Sdram */
    pharosPartition24SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 25                                                   */
  /******************************************************************/

  /* address in the flash of the partition 25 data to copy it to RAM */
  pharosPartition25DataCcmLoadAddr = LOADADDR(.partition25DataCcm);

  /* address in the flash of the partition 25 data to copy it to RAM */
  pharosPartition25DataSram1LoadAddr = LOADADDR(.partition25DataSram1);

  /* address in the flash of the partition 25 data to copy it to RAM */
  pharosPartition25DataSram2LoadAddr = LOADADDR(.partition25DataSram2);

  /* address in the flash of the partition 25 data to copy it to RAM */
  pharosPartition25DataSram3LoadAddr = LOADADDR(.partition25DataSram3);

  /* address in the flash of the partition 25 data to copy it to RAM */
  pharosPartition25DataSramBackupLoadAddr = LOADADDR(.partition25DataSramBackup);

  /* address in the flash of the partition 25 data to copy it to RAM */
  pharosPartition25DataSdramLoadAddr = LOADADDR(.partition25DataSdram);

  /* data of partition in Ccm */
  .partition25DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition25CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_25_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition25DataCcmStart = ABSOLUTE(.);

    *(.partition25DataCcm)
    *(.partition25DataCcm.*)

    /* address of the partition 25 data section end in RAM */
    . = ALIGN(8);
    pharosPartition25DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 25 in Ccm */
  .pharosPartition25BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 25 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition25BssCcmStart = .;

    *(.partition25BssCcm)
    *(.partition25BssCcm.*)

    /* address of the partition 25 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition25BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition25UssCcmStart = .;

    *(.partition25UssCcm)
    *(.partition25UssCcm.*)

    pharosPartition25UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_25_CCM_ALIGNMENT);

    /* address of the partition 25 sections end in Ccm */
    pharosPartition25CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition25DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition25Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_25_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition25DataSram1Start = ABSOLUTE(.);

    *(.partition25DataSram1)
    *(.partition25DataSram1.*)

    /* address of the partition 25 data section end in RAM */
    . = ALIGN(8);
    pharosPartition25DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 25 in Sram1 */
  .pharosPartition25BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 25 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition25BssSram1Start = .;

    *(.partition25BssSram1)
    *(.partition25BssSram1.*)

    /* address of the partition 25 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition25BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition25UssSram1Start = .;

    *(.partition25UssSram1)
    *(.partition25UssSram1.*)

    pharosPartition25UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_25_SRAM1_ALIGNMENT);

    /* address of the partition 25 sections end in Sram1 */
    pharosPartition25Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition25DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition25Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_25_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition25DataSram2Start = ABSOLUTE(.);

    *(.partition25DataSram2)
    *(.partition25DataSram2.*)

    /* address of the partition 25 data section end in RAM */
    . = ALIGN(8);
    pharosPartition25DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 25 in Sram2 */
  .pharosPartition25BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 25 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition25BssSram2Start = .;

    *(.partition25BssSram2)
    *(.partition25BssSram2.*)

    /* address of the partition 25 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition25BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition25UssSram2Start = .;

    *(.partition25UssSram2)
    *(.partition25UssSram2.*)

    pharosPartition25UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_25_SRAM2_ALIGNMENT);

    /* address of the partition 25 sections end in Sram2 */
    pharosPartition25Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition25DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition25Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_25_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition25DataSram3Start = ABSOLUTE(.);

    *(.partition25DataSram3)
    *(.partition25DataSram3.*)

    /* address of the partition 25 data section end in RAM */
    . = ALIGN(8);
    pharosPartition25DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 25 in Sram3 */
  .pharosPartition25BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 25 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition25BssSram3Start = .;

    *(.partition25BssSram3)
    *(.partition25BssSram3.*)

    /* address of the partition 25 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition25BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition25UssSram3Start = .;

    *(.partition25UssSram3)
    *(.partition25UssSram3.*)

    pharosPartition25UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_25_SRAM3_ALIGNMENT);

    /* address of the partition 25 sections end in Sram3 */
    pharosPartition25Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition25DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition25SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_25_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition25DataSramBackupStart = ABSOLUTE(.);

    *(.partition25DataSramBackup)
    *(.partition25DataSramBackup.*)

    /* address of the partition 25 data section end in RAM */
    . = ALIGN(8);
    pharosPartition25DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 25 in SramBackup */
  .pharosPartition25BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 25 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition25BssSramBackupStart = .;

    *(.partition25BssSramBackup)
    *(.partition25BssSramBackup.*)

    /* address of the partition 25 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition25BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition25UssSramBackupStart = .;

    *(.partition25UssSramBackup)
    *(.partition25UssSramBackup.*)

    pharosPartition25UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_25_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 25 sections end in SramBackup */
    pharosPartition25SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition25DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition25SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_25_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition25DataSdramStart = ABSOLUTE(.);

    *(.partition25DataSdram)
    *(.partition25DataSdram.*)

    /* address of the partition 25 data section end in RAM */
    . = ALIGN(8);
    pharosPartition25DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 25 in Sdram */
  .pharosPartition25BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 25 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition25BssSdramStart = .;

    *(.partition25BssSdram)
    *(.partition25BssSdram.*)

    /* address of the partition 25 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition25BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition25UssSdramStart = .;

    *(.partition25UssSdram)
    *(.partition25UssSdram.*)

    pharosPartition25UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_25_SDRAM_ALIGNMENT);

    /* address of the partition 25 sections end in Sdram */
    pharosPartition25SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 26                                                   */
  /******************************************************************/

  /* address in the flash of the partition 26 data to copy it to RAM */
  pharosPartition26DataCcmLoadAddr = LOADADDR(.partition26DataCcm);

  /* address in the flash of the partition 26 data to copy it to RAM */
  pharosPartition26DataSram1LoadAddr = LOADADDR(.partition26DataSram1);

  /* address in the flash of the partition 26 data to copy it to RAM */
  pharosPartition26DataSram2LoadAddr = LOADADDR(.partition26DataSram2);

  /* address in the flash of the partition 26 data to copy it to RAM */
  pharosPartition26DataSram3LoadAddr = LOADADDR(.partition26DataSram3);

  /* address in the flash of the partition 26 data to copy it to RAM */
  pharosPartition26DataSramBackupLoadAddr = LOADADDR(.partition26DataSramBackup);

  /* address in the flash of the partition 26 data to copy it to RAM */
  pharosPartition26DataSdramLoadAddr = LOADADDR(.partition26DataSdram);

  /* data of partition in Ccm */
  .partition26DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition26CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_26_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition26DataCcmStart = ABSOLUTE(.);

    *(.partition26DataCcm)
    *(.partition26DataCcm.*)

    /* address of the partition 26 data section end in RAM */
    . = ALIGN(8);
    pharosPartition26DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 26 in Ccm */
  .pharosPartition26BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 26 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition26BssCcmStart = .;

    *(.partition26BssCcm)
    *(.partition26BssCcm.*)

    /* address of the partition 26 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition26BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition26UssCcmStart = .;

    *(.partition26UssCcm)
    *(.partition26UssCcm.*)

    pharosPartition26UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_26_CCM_ALIGNMENT);

    /* address of the partition 26 sections end in Ccm */
    pharosPartition26CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition26DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition26Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_26_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition26DataSram1Start = ABSOLUTE(.);

    *(.partition26DataSram1)
    *(.partition26DataSram1.*)

    /* address of the partition 26 data section end in RAM */
    . = ALIGN(8);
    pharosPartition26DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 26 in Sram1 */
  .pharosPartition26BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 26 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition26BssSram1Start = .;

    *(.partition26BssSram1)
    *(.partition26BssSram1.*)

    /* address of the partition 26 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition26BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition26UssSram1Start = .;

    *(.partition26UssSram1)
    *(.partition26UssSram1.*)

    pharosPartition26UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_26_SRAM1_ALIGNMENT);

    /* address of the partition 26 sections end in Sram1 */
    pharosPartition26Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition26DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition26Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_26_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition26DataSram2Start = ABSOLUTE(.);

    *(.partition26DataSram2)
    *(.partition26DataSram2.*)

    /* address of the partition 26 data section end in RAM */
    . = ALIGN(8);
    pharosPartition26DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 26 in Sram2 */
  .pharosPartition26BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 26 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition26BssSram2Start = .;

    *(.partition26BssSram2)
    *(.partition26BssSram2.*)

    /* address of the partition 26 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition26BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition26UssSram2Start = .;

    *(.partition26UssSram2)
    *(.partition26UssSram2.*)

    pharosPartition26UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_26_SRAM2_ALIGNMENT);

    /* address of the partition 26 sections end in Sram2 */
    pharosPartition26Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition26DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition26Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_26_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition26DataSram3Start = ABSOLUTE(.);

    *(.partition26DataSram3)
    *(.partition26DataSram3.*)

    /* address of the partition 26 data section end in RAM */
    . = ALIGN(8);
    pharosPartition26DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 26 in Sram3 */
  .pharosPartition26BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 26 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition26BssSram3Start = .;

    *(.partition26BssSram3)
    *(.partition26BssSram3.*)

    /* address of the partition 26 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition26BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition26UssSram3Start = .;

    *(.partition26UssSram3)
    *(.partition26UssSram3.*)

    pharosPartition26UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_26_SRAM3_ALIGNMENT);

    /* address of the partition 26 sections end in Sram3 */
    pharosPartition26Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition26DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition26SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_26_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition26DataSramBackupStart = ABSOLUTE(.);

    *(.partition26DataSramBackup)
    *(.partition26DataSramBackup.*)

    /* address of the partition 26 data section end in RAM */
    . = ALIGN(8);
    pharosPartition26DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 26 in SramBackup */
  .pharosPartition26BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 26 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition26BssSramBackupStart = .;

    *(.partition26BssSramBackup)
    *(.partition26BssSramBackup.*)

    /* address of the partition 26 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition26BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition26UssSramBackupStart = .;

    *(.partition26UssSramBackup)
    *(.partition26UssSramBackup.*)

    pharosPartition26UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_26_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 26 sections end in SramBackup */
    pharosPartition26SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition26DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition26SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_26_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition26DataSdramStart = ABSOLUTE(.);

    *(.partition26DataSdram)
    *(.partition26DataSdram.*)

    /* address of the partition 26 data section end in RAM */
    . = ALIGN(8);
    pharosPartition26DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 26 in Sdram */
  .pharosPartition26BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 26 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition26BssSdramStart = .;

    *(.partition26BssSdram)
    *(.partition26BssSdram.*)

    /* address of the partition 26 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition26BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition26UssSdramStart = .;

    *(.partition26UssSdram)
    *(.partition26UssSdram.*)

    pharosPartition26UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_26_SDRAM_ALIGNMENT);

    /* address of the partition 26 sections end in Sdram */
    pharosPartition26SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 27                                                   */
  /******************************************************************/

  /* address in the flash of the partition 27 data to copy it to RAM */
  pharosPartition27DataCcmLoadAddr = LOADADDR(.partition27DataCcm);

  /* address in the flash of the partition 27 data to copy it to RAM */
  pharosPartition27DataSram1LoadAddr = LOADADDR(.partition27DataSram1);

  /* address in the flash of the partition 27 data to copy it to RAM */
  pharosPartition27DataSram2LoadAddr = LOADADDR(.partition27DataSram2);

  /* address in the flash of the partition 27 data to copy it to RAM */
  pharosPartition27DataSram3LoadAddr = LOADADDR(.partition27DataSram3);

  /* address in the flash of the partition 27 data to copy it to RAM */
  pharosPartition27DataSramBackupLoadAddr = LOADADDR(.partition27DataSramBackup);

  /* address in the flash of the partition 27 data to copy it to RAM */
  pharosPartition27DataSdramLoadAddr = LOADADDR(.partition27DataSdram);

  /* data of partition in Ccm */
  .partition27DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition27CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_27_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition27DataCcmStart = ABSOLUTE(.);

    *(.partition27DataCcm)
    *(.partition27DataCcm.*)

    /* address of the partition 27 data section end in RAM */
    . = ALIGN(8);
    pharosPartition27DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 27 in Ccm */
  .pharosPartition27BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 27 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition27BssCcmStart = .;

    *(.partition27BssCcm)
    *(.partition27BssCcm.*)

    /* address of the partition 27 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition27BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition27UssCcmStart = .;

    *(.partition27UssCcm)
    *(.partition27UssCcm.*)

    pharosPartition27UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_27_CCM_ALIGNMENT);

    /* address of the partition 27 sections end in Ccm */
    pharosPartition27CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition27DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition27Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_27_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition27DataSram1Start = ABSOLUTE(.);

    *(.partition27DataSram1)
    *(.partition27DataSram1.*)

    /* address of the partition 27 data section end in RAM */
    . = ALIGN(8);
    pharosPartition27DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 27 in Sram1 */
  .pharosPartition27BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 27 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition27BssSram1Start = .;

    *(.partition27BssSram1)
    *(.partition27BssSram1.*)

    /* address of the partition 27 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition27BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition27UssSram1Start = .;

    *(.partition27UssSram1)
    *(.partition27UssSram1.*)

    pharosPartition27UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_27_SRAM1_ALIGNMENT);

    /* address of the partition 27 sections end in Sram1 */
    pharosPartition27Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition27DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition27Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_27_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition27DataSram2Start = ABSOLUTE(.);

    *(.partition27DataSram2)
    *(.partition27DataSram2.*)

    /* address of the partition 27 data section end in RAM */
    . = ALIGN(8);
    pharosPartition27DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 27 in Sram2 */
  .pharosPartition27BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 27 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition27BssSram2Start = .;

    *(.partition27BssSram2)
    *(.partition27BssSram2.*)

    /* address of the partition 27 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition27BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition27UssSram2Start = .;

    *(.partition27UssSram2)
    *(.partition27UssSram2.*)

    pharosPartition27UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_27_SRAM2_ALIGNMENT);

    /* address of the partition 27 sections end in Sram2 */
    pharosPartition27Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition27DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition27Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_27_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition27DataSram3Start = ABSOLUTE(.);

    *(.partition27DataSram3)
    *(.partition27DataSram3.*)

    /* address of the partition 27 data section end in RAM */
    . = ALIGN(8);
    pharosPartition27DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 27 in Sram3 */
  .pharosPartition27BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 27 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition27BssSram3Start = .;

    *(.partition27BssSram3)
    *(.partition27BssSram3.*)

    /* address of the partition 27 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition27BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition27UssSram3Start = .;

    *(.partition27UssSram3)
    *(.partition27UssSram3.*)

    pharosPartition27UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_27_SRAM3_ALIGNMENT);

    /* address of the partition 27 sections end in Sram3 */
    pharosPartition27Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition27DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition27SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_27_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition27DataSramBackupStart = ABSOLUTE(.);

    *(.partition27DataSramBackup)
    *(.partition27DataSramBackup.*)

    /* address of the partition 27 data section end in RAM */
    . = ALIGN(8);
    pharosPartition27DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 27 in SramBackup */
  .pharosPartition27BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 27 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition27BssSramBackupStart = .;

    *(.partition27BssSramBackup)
    *(.partition27BssSramBackup.*)

    /* address of the partition 27 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition27BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition27UssSramBackupStart = .;

    *(.partition27UssSramBackup)
    *(.partition27UssSramBackup.*)

    pharosPartition27UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_27_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 27 sections end in SramBackup */
    pharosPartition27SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition27DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition27SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_27_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition27DataSdramStart = ABSOLUTE(.);

    *(.partition27DataSdram)
    *(.partition27DataSdram.*)

    /* address of the partition 27 data section end in RAM */
    . = ALIGN(8);
    pharosPartition27DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 27 in Sdram */
  .pharosPartition27BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 27 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition27BssSdramStart = .;

    *(.partition27BssSdram)
    *(.partition27BssSdram.*)

    /* address of the partition 27 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition27BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition27UssSdramStart = .;

    *(.partition27UssSdram)
    *(.partition27UssSdram.*)

    pharosPartition27UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_27_SDRAM_ALIGNMENT);

    /* address of the partition 27 sections end in Sdram */
    pharosPartition27SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 28                                                   */
  /******************************************************************/

  /* address in the flash of the partition 28 data to copy it to RAM */
  pharosPartition28DataCcmLoadAddr = LOADADDR(.partition28DataCcm);

  /* address in the flash of the partition 28 data to copy it to RAM */
  pharosPartition28DataSram1LoadAddr = LOADADDR(.partition28DataSram1);

  /* address in the flash of the partition 28 data to copy it to RAM */
  pharosPartition28DataSram2LoadAddr = LOADADDR(.partition28DataSram2);

  /* address in the flash of the partition 28 data to copy it to RAM */
  pharosPartition28DataSram3LoadAddr = LOADADDR(.partition28DataSram3);

  /* address in the flash of the partition 28 data to copy it to RAM */
  pharosPartition28DataSramBackupLoadAddr = LOADADDR(.partition28DataSramBackup);

  /* address in the flash of the partition 28 data to copy it to RAM */
  pharosPartition28DataSdramLoadAddr = LOADADDR(.partition28DataSdram);

  /* data of partition in Ccm */
  .partition28DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition28CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_28_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition28DataCcmStart = ABSOLUTE(.);

    *(.partition28DataCcm)
    *(.partition28DataCcm.*)

    /* address of the partition 28 data section end in RAM */
    . = ALIGN(8);
    pharosPartition28DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 28 in Ccm */
  .pharosPartition28BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 28 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition28BssCcmStart = .;

    *(.partition28BssCcm)
    *(.partition28BssCcm.*)

    /* address of the partition 28 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition28BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition28UssCcmStart = .;

    *(.partition28UssCcm)
    *(.partition28UssCcm.*)

    pharosPartition28UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_28_CCM_ALIGNMENT);

    /* address of the partition 28 sections end in Ccm */
    pharosPartition28CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition28DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition28Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_28_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition28DataSram1Start = ABSOLUTE(.);

    *(.partition28DataSram1)
    *(.partition28DataSram1.*)

    /* address of the partition 28 data section end in RAM */
    . = ALIGN(8);
    pharosPartition28DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 28 in Sram1 */
  .pharosPartition28BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 28 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition28BssSram1Start = .;

    *(.partition28BssSram1)
    *(.partition28BssSram1.*)

    /* address of the partition 28 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition28BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition28UssSram1Start = .;

    *(.partition28UssSram1)
    *(.partition28UssSram1.*)

    pharosPartition28UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_28_SRAM1_ALIGNMENT);

    /* address of the partition 28 sections end in Sram1 */
    pharosPartition28Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition28DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition28Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_28_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition28DataSram2Start = ABSOLUTE(.);

    *(.partition28DataSram2)
    *(.partition28DataSram2.*)

    /* address of the partition 28 data section end in RAM */
    . = ALIGN(8);
    pharosPartition28DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 28 in Sram2 */
  .pharosPartition28BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 28 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition28BssSram2Start = .;

    *(.partition28BssSram2)
    *(.partition28BssSram2.*)

    /* address of the partition 28 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition28BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition28UssSram2Start = .;

    *(.partition28UssSram2)
    *(.partition28UssSram2.*)

    pharosPartition28UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_28_SRAM2_ALIGNMENT);

    /* address of the partition 28 sections end in Sram2 */
    pharosPartition28Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition28DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition28Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_28_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition28DataSram3Start = ABSOLUTE(.);

    *(.partition28DataSram3)
    *(.partition28DataSram3.*)

    /* address of the partition 28 data section end in RAM */
    . = ALIGN(8);
    pharosPartition28DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 28 in Sram3 */
  .pharosPartition28BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 28 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition28BssSram3Start = .;

    *(.partition28BssSram3)
    *(.partition28BssSram3.*)

    /* address of the partition 28 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition28BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition28UssSram3Start = .;

    *(.partition28UssSram3)
    *(.partition28UssSram3.*)

    pharosPartition28UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_28_SRAM3_ALIGNMENT);

    /* address of the partition 28 sections end in Sram3 */
    pharosPartition28Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition28DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition28SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_28_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition28DataSramBackupStart = ABSOLUTE(.);

    *(.partition28DataSramBackup)
    *(.partition28DataSramBackup.*)

    /* address of the partition 28 data section end in RAM */
    . = ALIGN(8);
    pharosPartition28DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 28 in SramBackup */
  .pharosPartition28BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 28 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition28BssSramBackupStart = .;

    *(.partition28BssSramBackup)
    *(.partition28BssSramBackup.*)

    /* address of the partition 28 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition28BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition28UssSramBackupStart = .;

    *(.partition28UssSramBackup)
    *(.partition28UssSramBackup.*)

    pharosPartition28UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_28_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 28 sections end in SramBackup */
    pharosPartition28SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition28DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition28SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_28_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition28DataSdramStart = ABSOLUTE(.);

    *(.partition28DataSdram)
    *(.partition28DataSdram.*)

    /* address of the partition 28 data section end in RAM */
    . = ALIGN(8);
    pharosPartition28DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 28 in Sdram */
  .pharosPartition28BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 28 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition28BssSdramStart = .;

    *(.partition28BssSdram)
    *(.partition28BssSdram.*)

    /* address of the partition 28 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition28BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition28UssSdramStart = .;

    *(.partition28UssSdram)
    *(.partition28UssSdram.*)

    pharosPartition28UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_28_SDRAM_ALIGNMENT);

    /* address of the partition 28 sections end in Sdram */
    pharosPartition28SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 29                                                   */
  /******************************************************************/

  /* address in the flash of the partition 29 data to copy it to RAM */
  pharosPartition29DataCcmLoadAddr = LOADADDR(.partition29DataCcm);

  /* address in the flash of the partition 29 data to copy it to RAM */
  pharosPartition29DataSram1LoadAddr = LOADADDR(.partition29DataSram1);

  /* address in the flash of the partition 29 data to copy it to RAM */
  pharosPartition29DataSram2LoadAddr = LOADADDR(.partition29DataSram2);

  /* address in the flash of the partition 29 data to copy it to RAM */
  pharosPartition29DataSram3LoadAddr = LOADADDR(.partition29DataSram3);

  /* address in the flash of the partition 29 data to copy it to RAM */
  pharosPartition29DataSramBackupLoadAddr = LOADADDR(.partition29DataSramBackup);

  /* address in the flash of the partition 29 data to copy it to RAM */
  pharosPartition29DataSdramLoadAddr = LOADADDR(.partition29DataSdram);

  /* data of partition in Ccm */
  .partition29DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition29CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_29_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition29DataCcmStart = ABSOLUTE(.);

    *(.partition29DataCcm)
    *(.partition29DataCcm.*)

    /* address of the partition 29 data section end in RAM */
    . = ALIGN(8);
    pharosPartition29DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 29 in Ccm */
  .pharosPartition29BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 29 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition29BssCcmStart = .;

    *(.partition29BssCcm)
    *(.partition29BssCcm.*)

    /* address of the partition 29 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition29BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition29UssCcmStart = .;

    *(.partition29UssCcm)
    *(.partition29UssCcm.*)

    pharosPartition29UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_29_CCM_ALIGNMENT);

    /* address of the partition 29 sections end in Ccm */
    pharosPartition29CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition29DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition29Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_29_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition29DataSram1Start = ABSOLUTE(.);

    *(.partition29DataSram1)
    *(.partition29DataSram1.*)

    /* address of the partition 29 data section end in RAM */
    . = ALIGN(8);
    pharosPartition29DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 29 in Sram1 */
  .pharosPartition29BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 29 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition29BssSram1Start = .;

    *(.partition29BssSram1)
    *(.partition29BssSram1.*)

    /* address of the partition 29 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition29BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition29UssSram1Start = .;

    *(.partition29UssSram1)
    *(.partition29UssSram1.*)

    pharosPartition29UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_29_SRAM1_ALIGNMENT);

    /* address of the partition 29 sections end in Sram1 */
    pharosPartition29Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition29DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition29Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_29_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition29DataSram2Start = ABSOLUTE(.);

    *(.partition29DataSram2)
    *(.partition29DataSram2.*)

    /* address of the partition 29 data section end in RAM */
    . = ALIGN(8);
    pharosPartition29DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 29 in Sram2 */
  .pharosPartition29BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 29 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition29BssSram2Start = .;

    *(.partition29BssSram2)
    *(.partition29BssSram2.*)

    /* address of the partition 29 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition29BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition29UssSram2Start = .;

    *(.partition29UssSram2)
    *(.partition29UssSram2.*)

    pharosPartition29UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_29_SRAM2_ALIGNMENT);

    /* address of the partition 29 sections end in Sram2 */
    pharosPartition29Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition29DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition29Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_29_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition29DataSram3Start = ABSOLUTE(.);

    *(.partition29DataSram3)
    *(.partition29DataSram3.*)

    /* address of the partition 29 data section end in RAM */
    . = ALIGN(8);
    pharosPartition29DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 29 in Sram3 */
  .pharosPartition29BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 29 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition29BssSram3Start = .;

    *(.partition29BssSram3)
    *(.partition29BssSram3.*)

    /* address of the partition 29 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition29BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition29UssSram3Start = .;

    *(.partition29UssSram3)
    *(.partition29UssSram3.*)

    pharosPartition29UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_29_SRAM3_ALIGNMENT);

    /* address of the partition 29 sections end in Sram3 */
    pharosPartition29Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition29DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition29SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_29_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition29DataSramBackupStart = ABSOLUTE(.);

    *(.partition29DataSramBackup)
    *(.partition29DataSramBackup.*)

    /* address of the partition 29 data section end in RAM */
    . = ALIGN(8);
    pharosPartition29DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 29 in SramBackup */
  .pharosPartition29BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 29 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition29BssSramBackupStart = .;

    *(.partition29BssSramBackup)
    *(.partition29BssSramBackup.*)

    /* address of the partition 29 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition29BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition29UssSramBackupStart = .;

    *(.partition29UssSramBackup)
    *(.partition29UssSramBackup.*)

    pharosPartition29UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_29_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 29 sections end in SramBackup */
    pharosPartition29SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition29DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition29SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_29_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition29DataSdramStart = ABSOLUTE(.);

    *(.partition29DataSdram)
    *(.partition29DataSdram.*)

    /* address of the partition 29 data section end in RAM */
    . = ALIGN(8);
    pharosPartition29DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 29 in Sdram */
  .pharosPartition29BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 29 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition29BssSdramStart = .;

    *(.partition29BssSdram)
    *(.partition29BssSdram.*)

    /* address of the partition 29 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition29BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition29UssSdramStart = .;

    *(.partition29UssSdram)
    *(.partition29UssSdram.*)

    pharosPartition29UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_29_SDRAM_ALIGNMENT);

    /* address of the partition 29 sections end in Sdram */
    pharosPartition29SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 30                                                   */
  /******************************************************************/

  /* address in the flash of the partition 30 data to copy it to RAM */
  pharosPartition30DataCcmLoadAddr = LOADADDR(.partition30DataCcm);

  /* address in the flash of the partition 30 data to copy it to RAM */
  pharosPartition30DataSram1LoadAddr = LOADADDR(.partition30DataSram1);

  /* address in the flash of the partition 30 data to copy it to RAM */
  pharosPartition30DataSram2LoadAddr = LOADADDR(.partition30DataSram2);

  /* address in the flash of the partition 30 data to copy it to RAM */
  pharosPartition30DataSram3LoadAddr = LOADADDR(.partition30DataSram3);

  /* address in the flash of the partition 30 data to copy it to RAM */
  pharosPartition30DataSramBackupLoadAddr = LOADADDR(.partition30DataSramBackup);

  /* address in the flash of the partition 30 data to copy it to RAM */
  pharosPartition30DataSdramLoadAddr = LOADADDR(.partition30DataSdram);

  /* data of partition in Ccm */
  .partition30DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition30CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_30_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition30DataCcmStart = ABSOLUTE(.);

    *(.partition30DataCcm)
    *(.partition30DataCcm.*)

    /* address of the partition 30 data section end in RAM */
    . = ALIGN(8);
    pharosPartition30DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 30 in Ccm */
  .pharosPartition30BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 30 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition30BssCcmStart = .;

    *(.partition30BssCcm)
    *(.partition30BssCcm.*)

    /* address of the partition 30 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition30BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition30UssCcmStart = .;

    *(.partition30UssCcm)
    *(.partition30UssCcm.*)

    pharosPartition30UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_30_CCM_ALIGNMENT);

    /* address of the partition 30 sections end in Ccm */
    pharosPartition30CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition30DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition30Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_30_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition30DataSram1Start = ABSOLUTE(.);

    *(.partition30DataSram1)
    *(.partition30DataSram1.*)

    /* address of the partition 30 data section end in RAM */
    . = ALIGN(8);
    pharosPartition30DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 30 in Sram1 */
  .pharosPartition30BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 30 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition30BssSram1Start = .;

    *(.partition30BssSram1)
    *(.partition30BssSram1.*)

    /* address of the partition 30 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition30BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition30UssSram1Start = .;

    *(.partition30UssSram1)
    *(.partition30UssSram1.*)

    pharosPartition30UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_30_SRAM1_ALIGNMENT);

    /* address of the partition 30 sections end in Sram1 */
    pharosPartition30Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition30DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition30Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_30_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition30DataSram2Start = ABSOLUTE(.);

    *(.partition30DataSram2)
    *(.partition30DataSram2.*)

    /* address of the partition 30 data section end in RAM */
    . = ALIGN(8);
    pharosPartition30DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 30 in Sram2 */
  .pharosPartition30BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 30 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition30BssSram2Start = .;

    *(.partition30BssSram2)
    *(.partition30BssSram2.*)

    /* address of the partition 30 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition30BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition30UssSram2Start = .;

    *(.partition30UssSram2)
    *(.partition30UssSram2.*)

    pharosPartition30UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_30_SRAM2_ALIGNMENT);

    /* address of the partition 30 sections end in Sram2 */
    pharosPartition30Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition30DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition30Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_30_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition30DataSram3Start = ABSOLUTE(.);

    *(.partition30DataSram3)
    *(.partition30DataSram3.*)

    /* address of the partition 30 data section end in RAM */
    . = ALIGN(8);
    pharosPartition30DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 30 in Sram3 */
  .pharosPartition30BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 30 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition30BssSram3Start = .;

    *(.partition30BssSram3)
    *(.partition30BssSram3.*)

    /* address of the partition 30 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition30BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition30UssSram3Start = .;

    *(.partition30UssSram3)
    *(.partition30UssSram3.*)

    pharosPartition30UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_30_SRAM3_ALIGNMENT);

    /* address of the partition 30 sections end in Sram3 */
    pharosPartition30Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition30DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition30SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_30_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition30DataSramBackupStart = ABSOLUTE(.);

    *(.partition30DataSramBackup)
    *(.partition30DataSramBackup.*)

    /* address of the partition 30 data section end in RAM */
    . = ALIGN(8);
    pharosPartition30DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 30 in SramBackup */
  .pharosPartition30BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 30 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition30BssSramBackupStart = .;

    *(.partition30BssSramBackup)
    *(.partition30BssSramBackup.*)

    /* address of the partition 30 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition30BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition30UssSramBackupStart = .;

    *(.partition30UssSramBackup)
    *(.partition30UssSramBackup.*)

    pharosPartition30UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_30_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 30 sections end in SramBackup */
    pharosPartition30SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition30DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition30SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_30_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition30DataSdramStart = ABSOLUTE(.);

    *(.partition30DataSdram)
    *(.partition30DataSdram.*)

    /* address of the partition 30 data section end in RAM */
    . = ALIGN(8);
    pharosPartition30DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 30 in Sdram */
  .pharosPartition30BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 30 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition30BssSdramStart = .;

    *(.partition30BssSdram)
    *(.partition30BssSdram.*)

    /* address of the partition 30 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition30BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition30UssSdramStart = .;

    *(.partition30UssSdram)
    *(.partition30UssSdram.*)

    pharosPartition30UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_30_SDRAM_ALIGNMENT);

    /* address of the partition 30 sections end in Sdram */
    pharosPartition30SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 31                                                   */
  /******************************************************************/

  /* address in the flash of the partition 31 data to copy it to RAM */
  pharosPartition31DataCcmLoadAddr = LOADADDR(.partition31DataCcm);

  /* address in the flash of the partition 31 data to copy it to RAM */
  pharosPartition31DataSram1LoadAddr = LOADADDR(.partition31DataSram1);

  /* address in the flash of the partition 31 data to copy it to RAM */
  pharosPartition31DataSram2LoadAddr = LOADADDR(.partition31DataSram2);

  /* address in the flash of the partition 31 data to copy it to RAM */
  pharosPartition31DataSram3LoadAddr = LOADADDR(.partition31DataSram3);

  /* address in the flash of the partition 31 data to copy it to RAM */
  pharosPartition31DataSramBackupLoadAddr = LOADADDR(.partition31DataSramBackup);

  /* address in the flash of the partition 31 data to copy it to RAM */
  pharosPartition31DataSdramLoadAddr = LOADADDR(.partition31DataSdram);

  /* data of partition in Ccm */
  .partition31DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition31CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_31_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition31DataCcmStart = ABSOLUTE(.);

    *(.partition31DataCcm)
    *(.partition31DataCcm.*)

    /* address of the partition 31 data section end in RAM */
    . = ALIGN(8);
    pharosPartition31DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 31 in Ccm */
  .pharosPartition31BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 31 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition31BssCcmStart = .;

    *(.partition31BssCcm)
    *(.partition31BssCcm.*)

    /* address of the partition 31 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition31BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition31UssCcmStart = .;

    *(.partition31UssCcm)
    *(.partition31UssCcm.*)

    pharosPartition31UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_31_CCM_ALIGNMENT);

    /* address of the partition 31 sections end in Ccm */
    pharosPartition31CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition31DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition31Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_31_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition31DataSram1Start = ABSOLUTE(.);

    *(.partition31DataSram1)
    *(.partition31DataSram1.*)

    /* address of the partition 31 data section end in RAM */
    . = ALIGN(8);
    pharosPartition31DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 31 in Sram1 */
  .pharosPartition31BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 31 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition31BssSram1Start = .;

    *(.partition31BssSram1)
    *(.partition31BssSram1.*)

    /* address of the partition 31 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition31BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition31UssSram1Start = .;

    *(.partition31UssSram1)
    *(.partition31UssSram1.*)

    pharosPartition31UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_31_SRAM1_ALIGNMENT);

    /* address of the partition 31 sections end in Sram1 */
    pharosPartition31Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition31DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition31Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_31_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition31DataSram2Start = ABSOLUTE(.);

    *(.partition31DataSram2)
    *(.partition31DataSram2.*)

    /* address of the partition 31 data section end in RAM */
    . = ALIGN(8);
    pharosPartition31DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 31 in Sram2 */
  .pharosPartition31BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 31 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition31BssSram2Start = .;

    *(.partition31BssSram2)
    *(.partition31BssSram2.*)

    /* address of the partition 31 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition31BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition31UssSram2Start = .;

    *(.partition31UssSram2)
    *(.partition31UssSram2.*)

    pharosPartition31UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_31_SRAM2_ALIGNMENT);

    /* address of the partition 31 sections end in Sram2 */
    pharosPartition31Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition31DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition31Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_31_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition31DataSram3Start = ABSOLUTE(.);

    *(.partition31DataSram3)
    *(.partition31DataSram3.*)

    /* address of the partition 31 data section end in RAM */
    . = ALIGN(8);
    pharosPartition31DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 31 in Sram3 */
  .pharosPartition31BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 31 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition31BssSram3Start = .;

    *(.partition31BssSram3)
    *(.partition31BssSram3.*)

    /* address of the partition 31 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition31BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition31UssSram3Start = .;

    *(.partition31UssSram3)
    *(.partition31UssSram3.*)

    pharosPartition31UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_31_SRAM3_ALIGNMENT);

    /* address of the partition 31 sections end in Sram3 */
    pharosPartition31Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition31DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition31SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_31_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition31DataSramBackupStart = ABSOLUTE(.);

    *(.partition31DataSramBackup)
    *(.partition31DataSramBackup.*)

    /* address of the partition 31 data section end in RAM */
    . = ALIGN(8);
    pharosPartition31DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 31 in SramBackup */
  .pharosPartition31BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 31 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition31BssSramBackupStart = .;

    *(.partition31BssSramBackup)
    *(.partition31BssSramBackup.*)

    /* address of the partition 31 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition31BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition31UssSramBackupStart = .;

    *(.partition31UssSramBackup)
    *(.partition31UssSramBackup.*)

    pharosPartition31UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_31_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 31 sections end in SramBackup */
    pharosPartition31SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition31DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition31SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_31_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition31DataSdramStart = ABSOLUTE(.);

    *(.partition31DataSdram)
    *(.partition31DataSdram.*)

    /* address of the partition 31 data section end in RAM */
    . = ALIGN(8);
    pharosPartition31DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 31 in Sdram */
  .pharosPartition31BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 31 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition31BssSdramStart = .;

    *(.partition31BssSdram)
    *(.partition31BssSdram.*)

    /* address of the partition 31 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition31BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition31UssSdramStart = .;

    *(.partition31UssSdram)
    *(.partition31UssSdram.*)

    pharosPartition31UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_31_SDRAM_ALIGNMENT);

    /* address of the partition 31 sections end in Sdram */
    pharosPartition31SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 32                                                   */
  /******************************************************************/

  /* address in the flash of the partition 32 data to copy it to RAM */
  pharosPartition32DataCcmLoadAddr = LOADADDR(.partition32DataCcm);

  /* address in the flash of the partition 32 data to copy it to RAM */
  pharosPartition32DataSram1LoadAddr = LOADADDR(.partition32DataSram1);

  /* address in the flash of the partition 32 data to copy it to RAM */
  pharosPartition32DataSram2LoadAddr = LOADADDR(.partition32DataSram2);

  /* address in the flash of the partition 32 data to copy it to RAM */
  pharosPartition32DataSram3LoadAddr = LOADADDR(.partition32DataSram3);

  /* address in the flash of the partition 32 data to copy it to RAM */
  pharosPartition32DataSramBackupLoadAddr = LOADADDR(.partition32DataSramBackup);

  /* address in the flash of the partition 32 data to copy it to RAM */
  pharosPartition32DataSdramLoadAddr = LOADADDR(.partition32DataSdram);

  /* data of partition in Ccm */
  .partition32DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition32CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_32_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition32DataCcmStart = ABSOLUTE(.);

    *(.partition32DataCcm)
    *(.partition32DataCcm.*)

    /* address of the partition 32 data section end in RAM */
    . = ALIGN(8);
    pharosPartition32DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 32 in Ccm */
  .pharosPartition32BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 32 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition32BssCcmStart = .;

    *(.partition32BssCcm)
    *(.partition32BssCcm.*)

    /* address of the partition 32 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition32BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition32UssCcmStart = .;

    *(.partition32UssCcm)
    *(.partition32UssCcm.*)

    pharosPartition32UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_32_CCM_ALIGNMENT);

    /* address of the partition 32 sections end in Ccm */
    pharosPartition32CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition32DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition32Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_32_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition32DataSram1Start = ABSOLUTE(.);

    *(.partition32DataSram1)
    *(.partition32DataSram1.*)

    /* address of the partition 32 data section end in RAM */
    . = ALIGN(8);
    pharosPartition32DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 32 in Sram1 */
  .pharosPartition32BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 32 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition32BssSram1Start = .;

    *(.partition32BssSram1)
    *(.partition32BssSram1.*)

    /* address of the partition 32 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition32BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition32UssSram1Start = .;

    *(.partition32UssSram1)
    *(.partition32UssSram1.*)

    pharosPartition32UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_32_SRAM1_ALIGNMENT);

    /* address of the partition 32 sections end in Sram1 */
    pharosPartition32Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition32DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition32Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_32_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition32DataSram2Start = ABSOLUTE(.);

    *(.partition32DataSram2)
    *(.partition32DataSram2.*)

    /* address of the partition 32 data section end in RAM */
    . = ALIGN(8);
    pharosPartition32DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 32 in Sram2 */
  .pharosPartition32BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 32 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition32BssSram2Start = .;

    *(.partition32BssSram2)
    *(.partition32BssSram2.*)

    /* address of the partition 32 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition32BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition32UssSram2Start = .;

    *(.partition32UssSram2)
    *(.partition32UssSram2.*)

    pharosPartition32UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_32_SRAM2_ALIGNMENT);

    /* address of the partition 32 sections end in Sram2 */
    pharosPartition32Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition32DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition32Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_32_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition32DataSram3Start = ABSOLUTE(.);

    *(.partition32DataSram3)
    *(.partition32DataSram3.*)

    /* address of the partition 32 data section end in RAM */
    . = ALIGN(8);
    pharosPartition32DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 32 in Sram3 */
  .pharosPartition32BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 32 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition32BssSram3Start = .;

    *(.partition32BssSram3)
    *(.partition32BssSram3.*)

    /* address of the partition 32 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition32BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition32UssSram3Start = .;

    *(.partition32UssSram3)
    *(.partition32UssSram3.*)

    pharosPartition32UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_32_SRAM3_ALIGNMENT);

    /* address of the partition 32 sections end in Sram3 */
    pharosPartition32Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition32DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition32SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_32_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition32DataSramBackupStart = ABSOLUTE(.);

    *(.partition32DataSramBackup)
    *(.partition32DataSramBackup.*)

    /* address of the partition 32 data section end in RAM */
    . = ALIGN(8);
    pharosPartition32DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 32 in SramBackup */
  .pharosPartition32BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 32 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition32BssSramBackupStart = .;

    *(.partition32BssSramBackup)
    *(.partition32BssSramBackup.*)

    /* address of the partition 32 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition32BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition32UssSramBackupStart = .;

    *(.partition32UssSramBackup)
    *(.partition32UssSramBackup.*)

    pharosPartition32UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_32_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 32 sections end in SramBackup */
    pharosPartition32SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition32DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition32SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_32_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition32DataSdramStart = ABSOLUTE(.);

    *(.partition32DataSdram)
    *(.partition32DataSdram.*)

    /* address of the partition 32 data section end in RAM */
    . = ALIGN(8);
    pharosPartition32DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 32 in Sdram */
  .pharosPartition32BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 32 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition32BssSdramStart = .;

    *(.partition32BssSdram)
    *(.partition32BssSdram.*)

    /* address of the partition 32 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition32BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition32UssSdramStart = .;

    *(.partition32UssSdram)
    *(.partition32UssSdram.*)

    pharosPartition32UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_32_SDRAM_ALIGNMENT);

    /* address of the partition 32 sections end in Sdram */
    pharosPartition32SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 33                                                   */
  /******************************************************************/

  /* address in the flash of the partition 33 data to copy it to RAM */
  pharosPartition33DataCcmLoadAddr = LOADADDR(.partition33DataCcm);

  /* address in the flash of the partition 33 data to copy it to RAM */
  pharosPartition33DataSram1LoadAddr = LOADADDR(.partition33DataSram1);

  /* address in the flash of the partition 33 data to copy it to RAM */
  pharosPartition33DataSram2LoadAddr = LOADADDR(.partition33DataSram2);

  /* address in the flash of the partition 33 data to copy it to RAM */
  pharosPartition33DataSram3LoadAddr = LOADADDR(.partition33DataSram3);

  /* address in the flash of the partition 33 data to copy it to RAM */
  pharosPartition33DataSramBackupLoadAddr = LOADADDR(.partition33DataSramBackup);

  /* address in the flash of the partition 33 data to copy it to RAM */
  pharosPartition33DataSdramLoadAddr = LOADADDR(.partition33DataSdram);

  /* data of partition in Ccm */
  .partition33DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition33CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_33_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition33DataCcmStart = ABSOLUTE(.);

    *(.partition33DataCcm)
    *(.partition33DataCcm.*)

    /* address of the partition 33 data section end in RAM */
    . = ALIGN(8);
    pharosPartition33DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 33 in Ccm */
  .pharosPartition33BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 33 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition33BssCcmStart = .;

    *(.partition33BssCcm)
    *(.partition33BssCcm.*)

    /* address of the partition 33 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition33BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition33UssCcmStart = .;

    *(.partition33UssCcm)
    *(.partition33UssCcm.*)

    pharosPartition33UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_33_CCM_ALIGNMENT);

    /* address of the partition 33 sections end in Ccm */
    pharosPartition33CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition33DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition33Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_33_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition33DataSram1Start = ABSOLUTE(.);

    *(.partition33DataSram1)
    *(.partition33DataSram1.*)

    /* address of the partition 33 data section end in RAM */
    . = ALIGN(8);
    pharosPartition33DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 33 in Sram1 */
  .pharosPartition33BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 33 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition33BssSram1Start = .;

    *(.partition33BssSram1)
    *(.partition33BssSram1.*)

    /* address of the partition 33 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition33BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition33UssSram1Start = .;

    *(.partition33UssSram1)
    *(.partition33UssSram1.*)

    pharosPartition33UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_33_SRAM1_ALIGNMENT);

    /* address of the partition 33 sections end in Sram1 */
    pharosPartition33Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition33DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition33Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_33_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition33DataSram2Start = ABSOLUTE(.);

    *(.partition33DataSram2)
    *(.partition33DataSram2.*)

    /* address of the partition 33 data section end in RAM */
    . = ALIGN(8);
    pharosPartition33DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 33 in Sram2 */
  .pharosPartition33BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 33 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition33BssSram2Start = .;

    *(.partition33BssSram2)
    *(.partition33BssSram2.*)

    /* address of the partition 33 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition33BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition33UssSram2Start = .;

    *(.partition33UssSram2)
    *(.partition33UssSram2.*)

    pharosPartition33UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_33_SRAM2_ALIGNMENT);

    /* address of the partition 33 sections end in Sram2 */
    pharosPartition33Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition33DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition33Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_33_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition33DataSram3Start = ABSOLUTE(.);

    *(.partition33DataSram3)
    *(.partition33DataSram3.*)

    /* address of the partition 33 data section end in RAM */
    . = ALIGN(8);
    pharosPartition33DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 33 in Sram3 */
  .pharosPartition33BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 33 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition33BssSram3Start = .;

    *(.partition33BssSram3)
    *(.partition33BssSram3.*)

    /* address of the partition 33 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition33BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition33UssSram3Start = .;

    *(.partition33UssSram3)
    *(.partition33UssSram3.*)

    pharosPartition33UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_33_SRAM3_ALIGNMENT);

    /* address of the partition 33 sections end in Sram3 */
    pharosPartition33Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition33DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition33SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_33_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition33DataSramBackupStart = ABSOLUTE(.);

    *(.partition33DataSramBackup)
    *(.partition33DataSramBackup.*)

    /* address of the partition 33 data section end in RAM */
    . = ALIGN(8);
    pharosPartition33DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 33 in SramBackup */
  .pharosPartition33BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 33 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition33BssSramBackupStart = .;

    *(.partition33BssSramBackup)
    *(.partition33BssSramBackup.*)

    /* address of the partition 33 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition33BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition33UssSramBackupStart = .;

    *(.partition33UssSramBackup)
    *(.partition33UssSramBackup.*)

    pharosPartition33UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_33_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 33 sections end in SramBackup */
    pharosPartition33SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition33DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition33SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_33_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition33DataSdramStart = ABSOLUTE(.);

    *(.partition33DataSdram)
    *(.partition33DataSdram.*)

    /* address of the partition 33 data section end in RAM */
    . = ALIGN(8);
    pharosPartition33DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 33 in Sdram */
  .pharosPartition33BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 33 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition33BssSdramStart = .;

    *(.partition33BssSdram)
    *(.partition33BssSdram.*)

    /* address of the partition 33 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition33BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition33UssSdramStart = .;

    *(.partition33UssSdram)
    *(.partition33UssSdram.*)

    pharosPartition33UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_33_SDRAM_ALIGNMENT);

    /* address of the partition 33 sections end in Sdram */
    pharosPartition33SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 34                                                   */
  /******************************************************************/

  /* address in the flash of the partition 34 data to copy it to RAM */
  pharosPartition34DataCcmLoadAddr = LOADADDR(.partition34DataCcm);

  /* address in the flash of the partition 34 data to copy it to RAM */
  pharosPartition34DataSram1LoadAddr = LOADADDR(.partition34DataSram1);

  /* address in the flash of the partition 34 data to copy it to RAM */
  pharosPartition34DataSram2LoadAddr = LOADADDR(.partition34DataSram2);

  /* address in the flash of the partition 34 data to copy it to RAM */
  pharosPartition34DataSram3LoadAddr = LOADADDR(.partition34DataSram3);

  /* address in the flash of the partition 34 data to copy it to RAM */
  pharosPartition34DataSramBackupLoadAddr = LOADADDR(.partition34DataSramBackup);

  /* address in the flash of the partition 34 data to copy it to RAM */
  pharosPartition34DataSdramLoadAddr = LOADADDR(.partition34DataSdram);

  /* data of partition in Ccm */
  .partition34DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition34CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_34_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition34DataCcmStart = ABSOLUTE(.);

    *(.partition34DataCcm)
    *(.partition34DataCcm.*)

    /* address of the partition 34 data section end in RAM */
    . = ALIGN(8);
    pharosPartition34DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 34 in Ccm */
  .pharosPartition34BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 34 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition34BssCcmStart = .;

    *(.partition34BssCcm)
    *(.partition34BssCcm.*)

    /* address of the partition 34 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition34BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition34UssCcmStart = .;

    *(.partition34UssCcm)
    *(.partition34UssCcm.*)

    pharosPartition34UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_34_CCM_ALIGNMENT);

    /* address of the partition 34 sections end in Ccm */
    pharosPartition34CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition34DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition34Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_34_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition34DataSram1Start = ABSOLUTE(.);

    *(.partition34DataSram1)
    *(.partition34DataSram1.*)

    /* address of the partition 34 data section end in RAM */
    . = ALIGN(8);
    pharosPartition34DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 34 in Sram1 */
  .pharosPartition34BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 34 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition34BssSram1Start = .;

    *(.partition34BssSram1)
    *(.partition34BssSram1.*)

    /* address of the partition 34 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition34BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition34UssSram1Start = .;

    *(.partition34UssSram1)
    *(.partition34UssSram1.*)

    pharosPartition34UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_34_SRAM1_ALIGNMENT);

    /* address of the partition 34 sections end in Sram1 */
    pharosPartition34Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition34DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition34Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_34_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition34DataSram2Start = ABSOLUTE(.);

    *(.partition34DataSram2)
    *(.partition34DataSram2.*)

    /* address of the partition 34 data section end in RAM */
    . = ALIGN(8);
    pharosPartition34DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 34 in Sram2 */
  .pharosPartition34BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 34 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition34BssSram2Start = .;

    *(.partition34BssSram2)
    *(.partition34BssSram2.*)

    /* address of the partition 34 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition34BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition34UssSram2Start = .;

    *(.partition34UssSram2)
    *(.partition34UssSram2.*)

    pharosPartition34UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_34_SRAM2_ALIGNMENT);

    /* address of the partition 34 sections end in Sram2 */
    pharosPartition34Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition34DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition34Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_34_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition34DataSram3Start = ABSOLUTE(.);

    *(.partition34DataSram3)
    *(.partition34DataSram3.*)

    /* address of the partition 34 data section end in RAM */
    . = ALIGN(8);
    pharosPartition34DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 34 in Sram3 */
  .pharosPartition34BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 34 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition34BssSram3Start = .;

    *(.partition34BssSram3)
    *(.partition34BssSram3.*)

    /* address of the partition 34 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition34BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition34UssSram3Start = .;

    *(.partition34UssSram3)
    *(.partition34UssSram3.*)

    pharosPartition34UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_34_SRAM3_ALIGNMENT);

    /* address of the partition 34 sections end in Sram3 */
    pharosPartition34Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition34DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition34SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_34_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition34DataSramBackupStart = ABSOLUTE(.);

    *(.partition34DataSramBackup)
    *(.partition34DataSramBackup.*)

    /* address of the partition 34 data section end in RAM */
    . = ALIGN(8);
    pharosPartition34DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 34 in SramBackup */
  .pharosPartition34BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 34 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition34BssSramBackupStart = .;

    *(.partition34BssSramBackup)
    *(.partition34BssSramBackup.*)

    /* address of the partition 34 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition34BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition34UssSramBackupStart = .;

    *(.partition34UssSramBackup)
    *(.partition34UssSramBackup.*)

    pharosPartition34UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_34_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 34 sections end in SramBackup */
    pharosPartition34SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition34DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition34SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_34_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition34DataSdramStart = ABSOLUTE(.);

    *(.partition34DataSdram)
    *(.partition34DataSdram.*)

    /* address of the partition 34 data section end in RAM */
    . = ALIGN(8);
    pharosPartition34DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 34 in Sdram */
  .pharosPartition34BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 34 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition34BssSdramStart = .;

    *(.partition34BssSdram)
    *(.partition34BssSdram.*)

    /* address of the partition 34 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition34BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition34UssSdramStart = .;

    *(.partition34UssSdram)
    *(.partition34UssSdram.*)

    pharosPartition34UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_34_SDRAM_ALIGNMENT);

    /* address of the partition 34 sections end in Sdram */
    pharosPartition34SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 35                                                   */
  /******************************************************************/

  /* address in the flash of the partition 35 data to copy it to RAM */
  pharosPartition35DataCcmLoadAddr = LOADADDR(.partition35DataCcm);

  /* address in the flash of the partition 35 data to copy it to RAM */
  pharosPartition35DataSram1LoadAddr = LOADADDR(.partition35DataSram1);

  /* address in the flash of the partition 35 data to copy it to RAM */
  pharosPartition35DataSram2LoadAddr = LOADADDR(.partition35DataSram2);

  /* address in the flash of the partition 35 data to copy it to RAM */
  pharosPartition35DataSram3LoadAddr = LOADADDR(.partition35DataSram3);

  /* address in the flash of the partition 35 data to copy it to RAM */
  pharosPartition35DataSramBackupLoadAddr = LOADADDR(.partition35DataSramBackup);

  /* address in the flash of the partition 35 data to copy it to RAM */
  pharosPartition35DataSdramLoadAddr = LOADADDR(.partition35DataSdram);

  /* data of partition in Ccm */
  .partition35DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition35CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_35_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition35DataCcmStart = ABSOLUTE(.);

    *(.partition35DataCcm)
    *(.partition35DataCcm.*)

    /* address of the partition 35 data section end in RAM */
    . = ALIGN(8);
    pharosPartition35DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 35 in Ccm */
  .pharosPartition35BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 35 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition35BssCcmStart = .;

    *(.partition35BssCcm)
    *(.partition35BssCcm.*)

    /* address of the partition 35 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition35BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition35UssCcmStart = .;

    *(.partition35UssCcm)
    *(.partition35UssCcm.*)

    pharosPartition35UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_35_CCM_ALIGNMENT);

    /* address of the partition 35 sections end in Ccm */
    pharosPartition35CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition35DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition35Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_35_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition35DataSram1Start = ABSOLUTE(.);

    *(.partition35DataSram1)
    *(.partition35DataSram1.*)

    /* address of the partition 35 data section end in RAM */
    . = ALIGN(8);
    pharosPartition35DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 35 in Sram1 */
  .pharosPartition35BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 35 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition35BssSram1Start = .;

    *(.partition35BssSram1)
    *(.partition35BssSram1.*)

    /* address of the partition 35 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition35BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition35UssSram1Start = .;

    *(.partition35UssSram1)
    *(.partition35UssSram1.*)

    pharosPartition35UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_35_SRAM1_ALIGNMENT);

    /* address of the partition 35 sections end in Sram1 */
    pharosPartition35Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition35DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition35Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_35_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition35DataSram2Start = ABSOLUTE(.);

    *(.partition35DataSram2)
    *(.partition35DataSram2.*)

    /* address of the partition 35 data section end in RAM */
    . = ALIGN(8);
    pharosPartition35DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 35 in Sram2 */
  .pharosPartition35BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 35 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition35BssSram2Start = .;

    *(.partition35BssSram2)
    *(.partition35BssSram2.*)

    /* address of the partition 35 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition35BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition35UssSram2Start = .;

    *(.partition35UssSram2)
    *(.partition35UssSram2.*)

    pharosPartition35UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_35_SRAM2_ALIGNMENT);

    /* address of the partition 35 sections end in Sram2 */
    pharosPartition35Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition35DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition35Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_35_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition35DataSram3Start = ABSOLUTE(.);

    *(.partition35DataSram3)
    *(.partition35DataSram3.*)

    /* address of the partition 35 data section end in RAM */
    . = ALIGN(8);
    pharosPartition35DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 35 in Sram3 */
  .pharosPartition35BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 35 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition35BssSram3Start = .;

    *(.partition35BssSram3)
    *(.partition35BssSram3.*)

    /* address of the partition 35 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition35BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition35UssSram3Start = .;

    *(.partition35UssSram3)
    *(.partition35UssSram3.*)

    pharosPartition35UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_35_SRAM3_ALIGNMENT);

    /* address of the partition 35 sections end in Sram3 */
    pharosPartition35Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition35DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition35SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_35_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition35DataSramBackupStart = ABSOLUTE(.);

    *(.partition35DataSramBackup)
    *(.partition35DataSramBackup.*)

    /* address of the partition 35 data section end in RAM */
    . = ALIGN(8);
    pharosPartition35DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 35 in SramBackup */
  .pharosPartition35BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 35 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition35BssSramBackupStart = .;

    *(.partition35BssSramBackup)
    *(.partition35BssSramBackup.*)

    /* address of the partition 35 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition35BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition35UssSramBackupStart = .;

    *(.partition35UssSramBackup)
    *(.partition35UssSramBackup.*)

    pharosPartition35UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_35_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 35 sections end in SramBackup */
    pharosPartition35SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition35DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition35SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_35_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition35DataSdramStart = ABSOLUTE(.);

    *(.partition35DataSdram)
    *(.partition35DataSdram.*)

    /* address of the partition 35 data section end in RAM */
    . = ALIGN(8);
    pharosPartition35DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 35 in Sdram */
  .pharosPartition35BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 35 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition35BssSdramStart = .;

    *(.partition35BssSdram)
    *(.partition35BssSdram.*)

    /* address of the partition 35 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition35BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition35UssSdramStart = .;

    *(.partition35UssSdram)
    *(.partition35UssSdram.*)

    pharosPartition35UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_35_SDRAM_ALIGNMENT);

    /* address of the partition 35 sections end in Sdram */
    pharosPartition35SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 36                                                   */
  /******************************************************************/

  /* address in the flash of the partition 36 data to copy it to RAM */
  pharosPartition36DataCcmLoadAddr = LOADADDR(.partition36DataCcm);

  /* address in the flash of the partition 36 data to copy it to RAM */
  pharosPartition36DataSram1LoadAddr = LOADADDR(.partition36DataSram1);

  /* address in the flash of the partition 36 data to copy it to RAM */
  pharosPartition36DataSram2LoadAddr = LOADADDR(.partition36DataSram2);

  /* address in the flash of the partition 36 data to copy it to RAM */
  pharosPartition36DataSram3LoadAddr = LOADADDR(.partition36DataSram3);

  /* address in the flash of the partition 36 data to copy it to RAM */
  pharosPartition36DataSramBackupLoadAddr = LOADADDR(.partition36DataSramBackup);

  /* address in the flash of the partition 36 data to copy it to RAM */
  pharosPartition36DataSdramLoadAddr = LOADADDR(.partition36DataSdram);

  /* data of partition in Ccm */
  .partition36DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition36CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_36_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition36DataCcmStart = ABSOLUTE(.);

    *(.partition36DataCcm)
    *(.partition36DataCcm.*)

    /* address of the partition 36 data section end in RAM */
    . = ALIGN(8);
    pharosPartition36DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 36 in Ccm */
  .pharosPartition36BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 36 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition36BssCcmStart = .;

    *(.partition36BssCcm)
    *(.partition36BssCcm.*)

    /* address of the partition 36 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition36BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition36UssCcmStart = .;

    *(.partition36UssCcm)
    *(.partition36UssCcm.*)

    pharosPartition36UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_36_CCM_ALIGNMENT);

    /* address of the partition 36 sections end in Ccm */
    pharosPartition36CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition36DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition36Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_36_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition36DataSram1Start = ABSOLUTE(.);

    *(.partition36DataSram1)
    *(.partition36DataSram1.*)

    /* address of the partition 36 data section end in RAM */
    . = ALIGN(8);
    pharosPartition36DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 36 in Sram1 */
  .pharosPartition36BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 36 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition36BssSram1Start = .;

    *(.partition36BssSram1)
    *(.partition36BssSram1.*)

    /* address of the partition 36 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition36BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition36UssSram1Start = .;

    *(.partition36UssSram1)
    *(.partition36UssSram1.*)

    pharosPartition36UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_36_SRAM1_ALIGNMENT);

    /* address of the partition 36 sections end in Sram1 */
    pharosPartition36Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition36DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition36Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_36_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition36DataSram2Start = ABSOLUTE(.);

    *(.partition36DataSram2)
    *(.partition36DataSram2.*)

    /* address of the partition 36 data section end in RAM */
    . = ALIGN(8);
    pharosPartition36DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 36 in Sram2 */
  .pharosPartition36BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 36 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition36BssSram2Start = .;

    *(.partition36BssSram2)
    *(.partition36BssSram2.*)

    /* address of the partition 36 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition36BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition36UssSram2Start = .;

    *(.partition36UssSram2)
    *(.partition36UssSram2.*)

    pharosPartition36UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_36_SRAM2_ALIGNMENT);

    /* address of the partition 36 sections end in Sram2 */
    pharosPartition36Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition36DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition36Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_36_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition36DataSram3Start = ABSOLUTE(.);

    *(.partition36DataSram3)
    *(.partition36DataSram3.*)

    /* address of the partition 36 data section end in RAM */
    . = ALIGN(8);
    pharosPartition36DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 36 in Sram3 */
  .pharosPartition36BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 36 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition36BssSram3Start = .;

    *(.partition36BssSram3)
    *(.partition36BssSram3.*)

    /* address of the partition 36 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition36BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition36UssSram3Start = .;

    *(.partition36UssSram3)
    *(.partition36UssSram3.*)

    pharosPartition36UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_36_SRAM3_ALIGNMENT);

    /* address of the partition 36 sections end in Sram3 */
    pharosPartition36Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition36DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition36SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_36_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition36DataSramBackupStart = ABSOLUTE(.);

    *(.partition36DataSramBackup)
    *(.partition36DataSramBackup.*)

    /* address of the partition 36 data section end in RAM */
    . = ALIGN(8);
    pharosPartition36DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 36 in SramBackup */
  .pharosPartition36BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 36 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition36BssSramBackupStart = .;

    *(.partition36BssSramBackup)
    *(.partition36BssSramBackup.*)

    /* address of the partition 36 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition36BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition36UssSramBackupStart = .;

    *(.partition36UssSramBackup)
    *(.partition36UssSramBackup.*)

    pharosPartition36UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_36_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 36 sections end in SramBackup */
    pharosPartition36SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition36DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition36SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_36_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition36DataSdramStart = ABSOLUTE(.);

    *(.partition36DataSdram)
    *(.partition36DataSdram.*)

    /* address of the partition 36 data section end in RAM */
    . = ALIGN(8);
    pharosPartition36DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 36 in Sdram */
  .pharosPartition36BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 36 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition36BssSdramStart = .;

    *(.partition36BssSdram)
    *(.partition36BssSdram.*)

    /* address of the partition 36 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition36BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition36UssSdramStart = .;

    *(.partition36UssSdram)
    *(.partition36UssSdram.*)

    pharosPartition36UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_36_SDRAM_ALIGNMENT);

    /* address of the partition 36 sections end in Sdram */
    pharosPartition36SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 37                                                   */
  /******************************************************************/

  /* address in the flash of the partition 37 data to copy it to RAM */
  pharosPartition37DataCcmLoadAddr = LOADADDR(.partition37DataCcm);

  /* address in the flash of the partition 37 data to copy it to RAM */
  pharosPartition37DataSram1LoadAddr = LOADADDR(.partition37DataSram1);

  /* address in the flash of the partition 37 data to copy it to RAM */
  pharosPartition37DataSram2LoadAddr = LOADADDR(.partition37DataSram2);

  /* address in the flash of the partition 37 data to copy it to RAM */
  pharosPartition37DataSram3LoadAddr = LOADADDR(.partition37DataSram3);

  /* address in the flash of the partition 37 data to copy it to RAM */
  pharosPartition37DataSramBackupLoadAddr = LOADADDR(.partition37DataSramBackup);

  /* address in the flash of the partition 37 data to copy it to RAM */
  pharosPartition37DataSdramLoadAddr = LOADADDR(.partition37DataSdram);

  /* data of partition in Ccm */
  .partition37DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition37CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_37_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition37DataCcmStart = ABSOLUTE(.);

    *(.partition37DataCcm)
    *(.partition37DataCcm.*)

    /* address of the partition 37 data section end in RAM */
    . = ALIGN(8);
    pharosPartition37DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 37 in Ccm */
  .pharosPartition37BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 37 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition37BssCcmStart = .;

    *(.partition37BssCcm)
    *(.partition37BssCcm.*)

    /* address of the partition 37 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition37BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition37UssCcmStart = .;

    *(.partition37UssCcm)
    *(.partition37UssCcm.*)

    pharosPartition37UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_37_CCM_ALIGNMENT);

    /* address of the partition 37 sections end in Ccm */
    pharosPartition37CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition37DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition37Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_37_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition37DataSram1Start = ABSOLUTE(.);

    *(.partition37DataSram1)
    *(.partition37DataSram1.*)

    /* address of the partition 37 data section end in RAM */
    . = ALIGN(8);
    pharosPartition37DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 37 in Sram1 */
  .pharosPartition37BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 37 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition37BssSram1Start = .;

    *(.partition37BssSram1)
    *(.partition37BssSram1.*)

    /* address of the partition 37 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition37BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition37UssSram1Start = .;

    *(.partition37UssSram1)
    *(.partition37UssSram1.*)

    pharosPartition37UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_37_SRAM1_ALIGNMENT);

    /* address of the partition 37 sections end in Sram1 */
    pharosPartition37Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition37DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition37Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_37_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition37DataSram2Start = ABSOLUTE(.);

    *(.partition37DataSram2)
    *(.partition37DataSram2.*)

    /* address of the partition 37 data section end in RAM */
    . = ALIGN(8);
    pharosPartition37DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 37 in Sram2 */
  .pharosPartition37BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 37 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition37BssSram2Start = .;

    *(.partition37BssSram2)
    *(.partition37BssSram2.*)

    /* address of the partition 37 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition37BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition37UssSram2Start = .;

    *(.partition37UssSram2)
    *(.partition37UssSram2.*)

    pharosPartition37UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_37_SRAM2_ALIGNMENT);

    /* address of the partition 37 sections end in Sram2 */
    pharosPartition37Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition37DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition37Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_37_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition37DataSram3Start = ABSOLUTE(.);

    *(.partition37DataSram3)
    *(.partition37DataSram3.*)

    /* address of the partition 37 data section end in RAM */
    . = ALIGN(8);
    pharosPartition37DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 37 in Sram3 */
  .pharosPartition37BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 37 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition37BssSram3Start = .;

    *(.partition37BssSram3)
    *(.partition37BssSram3.*)

    /* address of the partition 37 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition37BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition37UssSram3Start = .;

    *(.partition37UssSram3)
    *(.partition37UssSram3.*)

    pharosPartition37UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_37_SRAM3_ALIGNMENT);

    /* address of the partition 37 sections end in Sram3 */
    pharosPartition37Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition37DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition37SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_37_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition37DataSramBackupStart = ABSOLUTE(.);

    *(.partition37DataSramBackup)
    *(.partition37DataSramBackup.*)

    /* address of the partition 37 data section end in RAM */
    . = ALIGN(8);
    pharosPartition37DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 37 in SramBackup */
  .pharosPartition37BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 37 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition37BssSramBackupStart = .;

    *(.partition37BssSramBackup)
    *(.partition37BssSramBackup.*)

    /* address of the partition 37 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition37BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition37UssSramBackupStart = .;

    *(.partition37UssSramBackup)
    *(.partition37UssSramBackup.*)

    pharosPartition37UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_37_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 37 sections end in SramBackup */
    pharosPartition37SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition37DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition37SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_37_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition37DataSdramStart = ABSOLUTE(.);

    *(.partition37DataSdram)
    *(.partition37DataSdram.*)

    /* address of the partition 37 data section end in RAM */
    . = ALIGN(8);
    pharosPartition37DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 37 in Sdram */
  .pharosPartition37BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 37 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition37BssSdramStart = .;

    *(.partition37BssSdram)
    *(.partition37BssSdram.*)

    /* address of the partition 37 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition37BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition37UssSdramStart = .;

    *(.partition37UssSdram)
    *(.partition37UssSdram.*)

    pharosPartition37UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_37_SDRAM_ALIGNMENT);

    /* address of the partition 37 sections end in Sdram */
    pharosPartition37SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 38                                                   */
  /******************************************************************/

  /* address in the flash of the partition 38 data to copy it to RAM */
  pharosPartition38DataCcmLoadAddr = LOADADDR(.partition38DataCcm);

  /* address in the flash of the partition 38 data to copy it to RAM */
  pharosPartition38DataSram1LoadAddr = LOADADDR(.partition38DataSram1);

  /* address in the flash of the partition 38 data to copy it to RAM */
  pharosPartition38DataSram2LoadAddr = LOADADDR(.partition38DataSram2);

  /* address in the flash of the partition 38 data to copy it to RAM */
  pharosPartition38DataSram3LoadAddr = LOADADDR(.partition38DataSram3);

  /* address in the flash of the partition 38 data to copy it to RAM */
  pharosPartition38DataSramBackupLoadAddr = LOADADDR(.partition38DataSramBackup);

  /* address in the flash of the partition 38 data to copy it to RAM */
  pharosPartition38DataSdramLoadAddr = LOADADDR(.partition38DataSdram);

  /* data of partition in Ccm */
  .partition38DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition38CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_38_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition38DataCcmStart = ABSOLUTE(.);

    *(.partition38DataCcm)
    *(.partition38DataCcm.*)

    /* address of the partition 38 data section end in RAM */
    . = ALIGN(8);
    pharosPartition38DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 38 in Ccm */
  .pharosPartition38BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 38 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition38BssCcmStart = .;

    *(.partition38BssCcm)
    *(.partition38BssCcm.*)

    /* address of the partition 38 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition38BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition38UssCcmStart = .;

    *(.partition38UssCcm)
    *(.partition38UssCcm.*)

    pharosPartition38UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_38_CCM_ALIGNMENT);

    /* address of the partition 38 sections end in Ccm */
    pharosPartition38CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition38DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition38Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_38_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition38DataSram1Start = ABSOLUTE(.);

    *(.partition38DataSram1)
    *(.partition38DataSram1.*)

    /* address of the partition 38 data section end in RAM */
    . = ALIGN(8);
    pharosPartition38DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 38 in Sram1 */
  .pharosPartition38BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 38 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition38BssSram1Start = .;

    *(.partition38BssSram1)
    *(.partition38BssSram1.*)

    /* address of the partition 38 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition38BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition38UssSram1Start = .;

    *(.partition38UssSram1)
    *(.partition38UssSram1.*)

    pharosPartition38UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_38_SRAM1_ALIGNMENT);

    /* address of the partition 38 sections end in Sram1 */
    pharosPartition38Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition38DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition38Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_38_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition38DataSram2Start = ABSOLUTE(.);

    *(.partition38DataSram2)
    *(.partition38DataSram2.*)

    /* address of the partition 38 data section end in RAM */
    . = ALIGN(8);
    pharosPartition38DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 38 in Sram2 */
  .pharosPartition38BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 38 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition38BssSram2Start = .;

    *(.partition38BssSram2)
    *(.partition38BssSram2.*)

    /* address of the partition 38 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition38BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition38UssSram2Start = .;

    *(.partition38UssSram2)
    *(.partition38UssSram2.*)

    pharosPartition38UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_38_SRAM2_ALIGNMENT);

    /* address of the partition 38 sections end in Sram2 */
    pharosPartition38Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition38DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition38Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_38_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition38DataSram3Start = ABSOLUTE(.);

    *(.partition38DataSram3)
    *(.partition38DataSram3.*)

    /* address of the partition 38 data section end in RAM */
    . = ALIGN(8);
    pharosPartition38DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 38 in Sram3 */
  .pharosPartition38BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 38 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition38BssSram3Start = .;

    *(.partition38BssSram3)
    *(.partition38BssSram3.*)

    /* address of the partition 38 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition38BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition38UssSram3Start = .;

    *(.partition38UssSram3)
    *(.partition38UssSram3.*)

    pharosPartition38UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_38_SRAM3_ALIGNMENT);

    /* address of the partition 38 sections end in Sram3 */
    pharosPartition38Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition38DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition38SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_38_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition38DataSramBackupStart = ABSOLUTE(.);

    *(.partition38DataSramBackup)
    *(.partition38DataSramBackup.*)

    /* address of the partition 38 data section end in RAM */
    . = ALIGN(8);
    pharosPartition38DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 38 in SramBackup */
  .pharosPartition38BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 38 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition38BssSramBackupStart = .;

    *(.partition38BssSramBackup)
    *(.partition38BssSramBackup.*)

    /* address of the partition 38 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition38BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition38UssSramBackupStart = .;

    *(.partition38UssSramBackup)
    *(.partition38UssSramBackup.*)

    pharosPartition38UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_38_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 38 sections end in SramBackup */
    pharosPartition38SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition38DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition38SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_38_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition38DataSdramStart = ABSOLUTE(.);

    *(.partition38DataSdram)
    *(.partition38DataSdram.*)

    /* address of the partition 38 data section end in RAM */
    . = ALIGN(8);
    pharosPartition38DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 38 in Sdram */
  .pharosPartition38BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 38 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition38BssSdramStart = .;

    *(.partition38BssSdram)
    *(.partition38BssSdram.*)

    /* address of the partition 38 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition38BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition38UssSdramStart = .;

    *(.partition38UssSdram)
    *(.partition38UssSdram.*)

    pharosPartition38UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_38_SDRAM_ALIGNMENT);

    /* address of the partition 38 sections end in Sdram */
    pharosPartition38SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 39                                                   */
  /******************************************************************/

  /* address in the flash of the partition 39 data to copy it to RAM */
  pharosPartition39DataCcmLoadAddr = LOADADDR(.partition39DataCcm);

  /* address in the flash of the partition 39 data to copy it to RAM */
  pharosPartition39DataSram1LoadAddr = LOADADDR(.partition39DataSram1);

  /* address in the flash of the partition 39 data to copy it to RAM */
  pharosPartition39DataSram2LoadAddr = LOADADDR(.partition39DataSram2);

  /* address in the flash of the partition 39 data to copy it to RAM */
  pharosPartition39DataSram3LoadAddr = LOADADDR(.partition39DataSram3);

  /* address in the flash of the partition 39 data to copy it to RAM */
  pharosPartition39DataSramBackupLoadAddr = LOADADDR(.partition39DataSramBackup);

  /* address in the flash of the partition 39 data to copy it to RAM */
  pharosPartition39DataSdramLoadAddr = LOADADDR(.partition39DataSdram);

  /* data of partition in Ccm */
  .partition39DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition39CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_39_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition39DataCcmStart = ABSOLUTE(.);

    *(.partition39DataCcm)
    *(.partition39DataCcm.*)

    /* address of the partition 39 data section end in RAM */
    . = ALIGN(8);
    pharosPartition39DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 39 in Ccm */
  .pharosPartition39BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 39 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition39BssCcmStart = .;

    *(.partition39BssCcm)
    *(.partition39BssCcm.*)

    /* address of the partition 39 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition39BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition39UssCcmStart = .;

    *(.partition39UssCcm)
    *(.partition39UssCcm.*)

    pharosPartition39UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_39_CCM_ALIGNMENT);

    /* address of the partition 39 sections end in Ccm */
    pharosPartition39CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition39DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition39Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_39_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition39DataSram1Start = ABSOLUTE(.);

    *(.partition39DataSram1)
    *(.partition39DataSram1.*)

    /* address of the partition 39 data section end in RAM */
    . = ALIGN(8);
    pharosPartition39DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 39 in Sram1 */
  .pharosPartition39BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 39 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition39BssSram1Start = .;

    *(.partition39BssSram1)
    *(.partition39BssSram1.*)

    /* address of the partition 39 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition39BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition39UssSram1Start = .;

    *(.partition39UssSram1)
    *(.partition39UssSram1.*)

    pharosPartition39UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_39_SRAM1_ALIGNMENT);

    /* address of the partition 39 sections end in Sram1 */
    pharosPartition39Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition39DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition39Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_39_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition39DataSram2Start = ABSOLUTE(.);

    *(.partition39DataSram2)
    *(.partition39DataSram2.*)

    /* address of the partition 39 data section end in RAM */
    . = ALIGN(8);
    pharosPartition39DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 39 in Sram2 */
  .pharosPartition39BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 39 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition39BssSram2Start = .;

    *(.partition39BssSram2)
    *(.partition39BssSram2.*)

    /* address of the partition 39 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition39BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition39UssSram2Start = .;

    *(.partition39UssSram2)
    *(.partition39UssSram2.*)

    pharosPartition39UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_39_SRAM2_ALIGNMENT);

    /* address of the partition 39 sections end in Sram2 */
    pharosPartition39Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition39DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition39Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_39_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition39DataSram3Start = ABSOLUTE(.);

    *(.partition39DataSram3)
    *(.partition39DataSram3.*)

    /* address of the partition 39 data section end in RAM */
    . = ALIGN(8);
    pharosPartition39DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 39 in Sram3 */
  .pharosPartition39BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 39 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition39BssSram3Start = .;

    *(.partition39BssSram3)
    *(.partition39BssSram3.*)

    /* address of the partition 39 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition39BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition39UssSram3Start = .;

    *(.partition39UssSram3)
    *(.partition39UssSram3.*)

    pharosPartition39UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_39_SRAM3_ALIGNMENT);

    /* address of the partition 39 sections end in Sram3 */
    pharosPartition39Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition39DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition39SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_39_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition39DataSramBackupStart = ABSOLUTE(.);

    *(.partition39DataSramBackup)
    *(.partition39DataSramBackup.*)

    /* address of the partition 39 data section end in RAM */
    . = ALIGN(8);
    pharosPartition39DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 39 in SramBackup */
  .pharosPartition39BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 39 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition39BssSramBackupStart = .;

    *(.partition39BssSramBackup)
    *(.partition39BssSramBackup.*)

    /* address of the partition 39 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition39BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition39UssSramBackupStart = .;

    *(.partition39UssSramBackup)
    *(.partition39UssSramBackup.*)

    pharosPartition39UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_39_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 39 sections end in SramBackup */
    pharosPartition39SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition39DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition39SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_39_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition39DataSdramStart = ABSOLUTE(.);

    *(.partition39DataSdram)
    *(.partition39DataSdram.*)

    /* address of the partition 39 data section end in RAM */
    . = ALIGN(8);
    pharosPartition39DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 39 in Sdram */
  .pharosPartition39BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 39 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition39BssSdramStart = .;

    *(.partition39BssSdram)
    *(.partition39BssSdram.*)

    /* address of the partition 39 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition39BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition39UssSdramStart = .;

    *(.partition39UssSdram)
    *(.partition39UssSdram.*)

    pharosPartition39UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_39_SDRAM_ALIGNMENT);

    /* address of the partition 39 sections end in Sdram */
    pharosPartition39SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 40                                                   */
  /******************************************************************/

  /* address in the flash of the partition 40 data to copy it to RAM */
  pharosPartition40DataCcmLoadAddr = LOADADDR(.partition40DataCcm);

  /* address in the flash of the partition 40 data to copy it to RAM */
  pharosPartition40DataSram1LoadAddr = LOADADDR(.partition40DataSram1);

  /* address in the flash of the partition 40 data to copy it to RAM */
  pharosPartition40DataSram2LoadAddr = LOADADDR(.partition40DataSram2);

  /* address in the flash of the partition 40 data to copy it to RAM */
  pharosPartition40DataSram3LoadAddr = LOADADDR(.partition40DataSram3);

  /* address in the flash of the partition 40 data to copy it to RAM */
  pharosPartition40DataSramBackupLoadAddr = LOADADDR(.partition40DataSramBackup);

  /* address in the flash of the partition 40 data to copy it to RAM */
  pharosPartition40DataSdramLoadAddr = LOADADDR(.partition40DataSdram);

  /* data of partition in Ccm */
  .partition40DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition40CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_40_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition40DataCcmStart = ABSOLUTE(.);

    *(.partition40DataCcm)
    *(.partition40DataCcm.*)

    /* address of the partition 40 data section end in RAM */
    . = ALIGN(8);
    pharosPartition40DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 40 in Ccm */
  .pharosPartition40BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 40 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition40BssCcmStart = .;

    *(.partition40BssCcm)
    *(.partition40BssCcm.*)

    /* address of the partition 40 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition40BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition40UssCcmStart = .;

    *(.partition40UssCcm)
    *(.partition40UssCcm.*)

    pharosPartition40UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_40_CCM_ALIGNMENT);

    /* address of the partition 40 sections end in Ccm */
    pharosPartition40CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition40DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition40Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_40_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition40DataSram1Start = ABSOLUTE(.);

    *(.partition40DataSram1)
    *(.partition40DataSram1.*)

    /* address of the partition 40 data section end in RAM */
    . = ALIGN(8);
    pharosPartition40DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 40 in Sram1 */
  .pharosPartition40BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 40 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition40BssSram1Start = .;

    *(.partition40BssSram1)
    *(.partition40BssSram1.*)

    /* address of the partition 40 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition40BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition40UssSram1Start = .;

    *(.partition40UssSram1)
    *(.partition40UssSram1.*)

    pharosPartition40UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_40_SRAM1_ALIGNMENT);

    /* address of the partition 40 sections end in Sram1 */
    pharosPartition40Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition40DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition40Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_40_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition40DataSram2Start = ABSOLUTE(.);

    *(.partition40DataSram2)
    *(.partition40DataSram2.*)

    /* address of the partition 40 data section end in RAM */
    . = ALIGN(8);
    pharosPartition40DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 40 in Sram2 */
  .pharosPartition40BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 40 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition40BssSram2Start = .;

    *(.partition40BssSram2)
    *(.partition40BssSram2.*)

    /* address of the partition 40 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition40BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition40UssSram2Start = .;

    *(.partition40UssSram2)
    *(.partition40UssSram2.*)

    pharosPartition40UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_40_SRAM2_ALIGNMENT);

    /* address of the partition 40 sections end in Sram2 */
    pharosPartition40Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition40DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition40Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_40_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition40DataSram3Start = ABSOLUTE(.);

    *(.partition40DataSram3)
    *(.partition40DataSram3.*)

    /* address of the partition 40 data section end in RAM */
    . = ALIGN(8);
    pharosPartition40DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 40 in Sram3 */
  .pharosPartition40BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 40 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition40BssSram3Start = .;

    *(.partition40BssSram3)
    *(.partition40BssSram3.*)

    /* address of the partition 40 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition40BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition40UssSram3Start = .;

    *(.partition40UssSram3)
    *(.partition40UssSram3.*)

    pharosPartition40UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_40_SRAM3_ALIGNMENT);

    /* address of the partition 40 sections end in Sram3 */
    pharosPartition40Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition40DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition40SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_40_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition40DataSramBackupStart = ABSOLUTE(.);

    *(.partition40DataSramBackup)
    *(.partition40DataSramBackup.*)

    /* address of the partition 40 data section end in RAM */
    . = ALIGN(8);
    pharosPartition40DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 40 in SramBackup */
  .pharosPartition40BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 40 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition40BssSramBackupStart = .;

    *(.partition40BssSramBackup)
    *(.partition40BssSramBackup.*)

    /* address of the partition 40 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition40BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition40UssSramBackupStart = .;

    *(.partition40UssSramBackup)
    *(.partition40UssSramBackup.*)

    pharosPartition40UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_40_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 40 sections end in SramBackup */
    pharosPartition40SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition40DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition40SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_40_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition40DataSdramStart = ABSOLUTE(.);

    *(.partition40DataSdram)
    *(.partition40DataSdram.*)

    /* address of the partition 40 data section end in RAM */
    . = ALIGN(8);
    pharosPartition40DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 40 in Sdram */
  .pharosPartition40BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 40 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition40BssSdramStart = .;

    *(.partition40BssSdram)
    *(.partition40BssSdram.*)

    /* address of the partition 40 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition40BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition40UssSdramStart = .;

    *(.partition40UssSdram)
    *(.partition40UssSdram.*)

    pharosPartition40UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_40_SDRAM_ALIGNMENT);

    /* address of the partition 40 sections end in Sdram */
    pharosPartition40SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 41                                                   */
  /******************************************************************/

  /* address in the flash of the partition 41 data to copy it to RAM */
  pharosPartition41DataCcmLoadAddr = LOADADDR(.partition41DataCcm);

  /* address in the flash of the partition 41 data to copy it to RAM */
  pharosPartition41DataSram1LoadAddr = LOADADDR(.partition41DataSram1);

  /* address in the flash of the partition 41 data to copy it to RAM */
  pharosPartition41DataSram2LoadAddr = LOADADDR(.partition41DataSram2);

  /* address in the flash of the partition 41 data to copy it to RAM */
  pharosPartition41DataSram3LoadAddr = LOADADDR(.partition41DataSram3);

  /* address in the flash of the partition 41 data to copy it to RAM */
  pharosPartition41DataSramBackupLoadAddr = LOADADDR(.partition41DataSramBackup);

  /* address in the flash of the partition 41 data to copy it to RAM */
  pharosPartition41DataSdramLoadAddr = LOADADDR(.partition41DataSdram);

  /* data of partition in Ccm */
  .partition41DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition41CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_41_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition41DataCcmStart = ABSOLUTE(.);

    *(.partition41DataCcm)
    *(.partition41DataCcm.*)

    /* address of the partition 41 data section end in RAM */
    . = ALIGN(8);
    pharosPartition41DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 41 in Ccm */
  .pharosPartition41BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 41 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition41BssCcmStart = .;

    *(.partition41BssCcm)
    *(.partition41BssCcm.*)

    /* address of the partition 41 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition41BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition41UssCcmStart = .;

    *(.partition41UssCcm)
    *(.partition41UssCcm.*)

    pharosPartition41UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_41_CCM_ALIGNMENT);

    /* address of the partition 41 sections end in Ccm */
    pharosPartition41CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition41DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition41Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_41_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition41DataSram1Start = ABSOLUTE(.);

    *(.partition41DataSram1)
    *(.partition41DataSram1.*)

    /* address of the partition 41 data section end in RAM */
    . = ALIGN(8);
    pharosPartition41DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 41 in Sram1 */
  .pharosPartition41BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 41 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition41BssSram1Start = .;

    *(.partition41BssSram1)
    *(.partition41BssSram1.*)

    /* address of the partition 41 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition41BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition41UssSram1Start = .;

    *(.partition41UssSram1)
    *(.partition41UssSram1.*)

    pharosPartition41UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_41_SRAM1_ALIGNMENT);

    /* address of the partition 41 sections end in Sram1 */
    pharosPartition41Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition41DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition41Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_41_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition41DataSram2Start = ABSOLUTE(.);

    *(.partition41DataSram2)
    *(.partition41DataSram2.*)

    /* address of the partition 41 data section end in RAM */
    . = ALIGN(8);
    pharosPartition41DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 41 in Sram2 */
  .pharosPartition41BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 41 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition41BssSram2Start = .;

    *(.partition41BssSram2)
    *(.partition41BssSram2.*)

    /* address of the partition 41 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition41BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition41UssSram2Start = .;

    *(.partition41UssSram2)
    *(.partition41UssSram2.*)

    pharosPartition41UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_41_SRAM2_ALIGNMENT);

    /* address of the partition 41 sections end in Sram2 */
    pharosPartition41Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition41DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition41Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_41_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition41DataSram3Start = ABSOLUTE(.);

    *(.partition41DataSram3)
    *(.partition41DataSram3.*)

    /* address of the partition 41 data section end in RAM */
    . = ALIGN(8);
    pharosPartition41DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 41 in Sram3 */
  .pharosPartition41BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 41 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition41BssSram3Start = .;

    *(.partition41BssSram3)
    *(.partition41BssSram3.*)

    /* address of the partition 41 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition41BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition41UssSram3Start = .;

    *(.partition41UssSram3)
    *(.partition41UssSram3.*)

    pharosPartition41UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_41_SRAM3_ALIGNMENT);

    /* address of the partition 41 sections end in Sram3 */
    pharosPartition41Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition41DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition41SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_41_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition41DataSramBackupStart = ABSOLUTE(.);

    *(.partition41DataSramBackup)
    *(.partition41DataSramBackup.*)

    /* address of the partition 41 data section end in RAM */
    . = ALIGN(8);
    pharosPartition41DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 41 in SramBackup */
  .pharosPartition41BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 41 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition41BssSramBackupStart = .;

    *(.partition41BssSramBackup)
    *(.partition41BssSramBackup.*)

    /* address of the partition 41 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition41BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition41UssSramBackupStart = .;

    *(.partition41UssSramBackup)
    *(.partition41UssSramBackup.*)

    pharosPartition41UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_41_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 41 sections end in SramBackup */
    pharosPartition41SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition41DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition41SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_41_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition41DataSdramStart = ABSOLUTE(.);

    *(.partition41DataSdram)
    *(.partition41DataSdram.*)

    /* address of the partition 41 data section end in RAM */
    . = ALIGN(8);
    pharosPartition41DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 41 in Sdram */
  .pharosPartition41BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 41 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition41BssSdramStart = .;

    *(.partition41BssSdram)
    *(.partition41BssSdram.*)

    /* address of the partition 41 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition41BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition41UssSdramStart = .;

    *(.partition41UssSdram)
    *(.partition41UssSdram.*)

    pharosPartition41UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_41_SDRAM_ALIGNMENT);

    /* address of the partition 41 sections end in Sdram */
    pharosPartition41SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 42                                                   */
  /******************************************************************/

  /* address in the flash of the partition 42 data to copy it to RAM */
  pharosPartition42DataCcmLoadAddr = LOADADDR(.partition42DataCcm);

  /* address in the flash of the partition 42 data to copy it to RAM */
  pharosPartition42DataSram1LoadAddr = LOADADDR(.partition42DataSram1);

  /* address in the flash of the partition 42 data to copy it to RAM */
  pharosPartition42DataSram2LoadAddr = LOADADDR(.partition42DataSram2);

  /* address in the flash of the partition 42 data to copy it to RAM */
  pharosPartition42DataSram3LoadAddr = LOADADDR(.partition42DataSram3);

  /* address in the flash of the partition 42 data to copy it to RAM */
  pharosPartition42DataSramBackupLoadAddr = LOADADDR(.partition42DataSramBackup);

  /* address in the flash of the partition 42 data to copy it to RAM */
  pharosPartition42DataSdramLoadAddr = LOADADDR(.partition42DataSdram);

  /* data of partition in Ccm */
  .partition42DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition42CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_42_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition42DataCcmStart = ABSOLUTE(.);

    *(.partition42DataCcm)
    *(.partition42DataCcm.*)

    /* address of the partition 42 data section end in RAM */
    . = ALIGN(8);
    pharosPartition42DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 42 in Ccm */
  .pharosPartition42BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 42 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition42BssCcmStart = .;

    *(.partition42BssCcm)
    *(.partition42BssCcm.*)

    /* address of the partition 42 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition42BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition42UssCcmStart = .;

    *(.partition42UssCcm)
    *(.partition42UssCcm.*)

    pharosPartition42UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_42_CCM_ALIGNMENT);

    /* address of the partition 42 sections end in Ccm */
    pharosPartition42CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition42DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition42Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_42_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition42DataSram1Start = ABSOLUTE(.);

    *(.partition42DataSram1)
    *(.partition42DataSram1.*)

    /* address of the partition 42 data section end in RAM */
    . = ALIGN(8);
    pharosPartition42DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 42 in Sram1 */
  .pharosPartition42BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 42 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition42BssSram1Start = .;

    *(.partition42BssSram1)
    *(.partition42BssSram1.*)

    /* address of the partition 42 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition42BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition42UssSram1Start = .;

    *(.partition42UssSram1)
    *(.partition42UssSram1.*)

    pharosPartition42UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_42_SRAM1_ALIGNMENT);

    /* address of the partition 42 sections end in Sram1 */
    pharosPartition42Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition42DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition42Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_42_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition42DataSram2Start = ABSOLUTE(.);

    *(.partition42DataSram2)
    *(.partition42DataSram2.*)

    /* address of the partition 42 data section end in RAM */
    . = ALIGN(8);
    pharosPartition42DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 42 in Sram2 */
  .pharosPartition42BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 42 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition42BssSram2Start = .;

    *(.partition42BssSram2)
    *(.partition42BssSram2.*)

    /* address of the partition 42 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition42BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition42UssSram2Start = .;

    *(.partition42UssSram2)
    *(.partition42UssSram2.*)

    pharosPartition42UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_42_SRAM2_ALIGNMENT);

    /* address of the partition 42 sections end in Sram2 */
    pharosPartition42Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition42DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition42Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_42_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition42DataSram3Start = ABSOLUTE(.);

    *(.partition42DataSram3)
    *(.partition42DataSram3.*)

    /* address of the partition 42 data section end in RAM */
    . = ALIGN(8);
    pharosPartition42DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 42 in Sram3 */
  .pharosPartition42BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 42 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition42BssSram3Start = .;

    *(.partition42BssSram3)
    *(.partition42BssSram3.*)

    /* address of the partition 42 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition42BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition42UssSram3Start = .;

    *(.partition42UssSram3)
    *(.partition42UssSram3.*)

    pharosPartition42UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_42_SRAM3_ALIGNMENT);

    /* address of the partition 42 sections end in Sram3 */
    pharosPartition42Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition42DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition42SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_42_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition42DataSramBackupStart = ABSOLUTE(.);

    *(.partition42DataSramBackup)
    *(.partition42DataSramBackup.*)

    /* address of the partition 42 data section end in RAM */
    . = ALIGN(8);
    pharosPartition42DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 42 in SramBackup */
  .pharosPartition42BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 42 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition42BssSramBackupStart = .;

    *(.partition42BssSramBackup)
    *(.partition42BssSramBackup.*)

    /* address of the partition 42 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition42BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition42UssSramBackupStart = .;

    *(.partition42UssSramBackup)
    *(.partition42UssSramBackup.*)

    pharosPartition42UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_42_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 42 sections end in SramBackup */
    pharosPartition42SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition42DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition42SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_42_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition42DataSdramStart = ABSOLUTE(.);

    *(.partition42DataSdram)
    *(.partition42DataSdram.*)

    /* address of the partition 42 data section end in RAM */
    . = ALIGN(8);
    pharosPartition42DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 42 in Sdram */
  .pharosPartition42BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 42 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition42BssSdramStart = .;

    *(.partition42BssSdram)
    *(.partition42BssSdram.*)

    /* address of the partition 42 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition42BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition42UssSdramStart = .;

    *(.partition42UssSdram)
    *(.partition42UssSdram.*)

    pharosPartition42UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_42_SDRAM_ALIGNMENT);

    /* address of the partition 42 sections end in Sdram */
    pharosPartition42SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 43                                                   */
  /******************************************************************/

  /* address in the flash of the partition 43 data to copy it to RAM */
  pharosPartition43DataCcmLoadAddr = LOADADDR(.partition43DataCcm);

  /* address in the flash of the partition 43 data to copy it to RAM */
  pharosPartition43DataSram1LoadAddr = LOADADDR(.partition43DataSram1);

  /* address in the flash of the partition 43 data to copy it to RAM */
  pharosPartition43DataSram2LoadAddr = LOADADDR(.partition43DataSram2);

  /* address in the flash of the partition 43 data to copy it to RAM */
  pharosPartition43DataSram3LoadAddr = LOADADDR(.partition43DataSram3);

  /* address in the flash of the partition 43 data to copy it to RAM */
  pharosPartition43DataSramBackupLoadAddr = LOADADDR(.partition43DataSramBackup);

  /* address in the flash of the partition 43 data to copy it to RAM */
  pharosPartition43DataSdramLoadAddr = LOADADDR(.partition43DataSdram);

  /* data of partition in Ccm */
  .partition43DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition43CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_43_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition43DataCcmStart = ABSOLUTE(.);

    *(.partition43DataCcm)
    *(.partition43DataCcm.*)

    /* address of the partition 43 data section end in RAM */
    . = ALIGN(8);
    pharosPartition43DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 43 in Ccm */
  .pharosPartition43BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 43 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition43BssCcmStart = .;

    *(.partition43BssCcm)
    *(.partition43BssCcm.*)

    /* address of the partition 43 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition43BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition43UssCcmStart = .;

    *(.partition43UssCcm)
    *(.partition43UssCcm.*)

    pharosPartition43UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_43_CCM_ALIGNMENT);

    /* address of the partition 43 sections end in Ccm */
    pharosPartition43CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition43DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition43Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_43_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition43DataSram1Start = ABSOLUTE(.);

    *(.partition43DataSram1)
    *(.partition43DataSram1.*)

    /* address of the partition 43 data section end in RAM */
    . = ALIGN(8);
    pharosPartition43DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 43 in Sram1 */
  .pharosPartition43BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 43 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition43BssSram1Start = .;

    *(.partition43BssSram1)
    *(.partition43BssSram1.*)

    /* address of the partition 43 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition43BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition43UssSram1Start = .;

    *(.partition43UssSram1)
    *(.partition43UssSram1.*)

    pharosPartition43UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_43_SRAM1_ALIGNMENT);

    /* address of the partition 43 sections end in Sram1 */
    pharosPartition43Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition43DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition43Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_43_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition43DataSram2Start = ABSOLUTE(.);

    *(.partition43DataSram2)
    *(.partition43DataSram2.*)

    /* address of the partition 43 data section end in RAM */
    . = ALIGN(8);
    pharosPartition43DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 43 in Sram2 */
  .pharosPartition43BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 43 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition43BssSram2Start = .;

    *(.partition43BssSram2)
    *(.partition43BssSram2.*)

    /* address of the partition 43 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition43BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition43UssSram2Start = .;

    *(.partition43UssSram2)
    *(.partition43UssSram2.*)

    pharosPartition43UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_43_SRAM2_ALIGNMENT);

    /* address of the partition 43 sections end in Sram2 */
    pharosPartition43Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition43DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition43Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_43_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition43DataSram3Start = ABSOLUTE(.);

    *(.partition43DataSram3)
    *(.partition43DataSram3.*)

    /* address of the partition 43 data section end in RAM */
    . = ALIGN(8);
    pharosPartition43DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 43 in Sram3 */
  .pharosPartition43BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 43 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition43BssSram3Start = .;

    *(.partition43BssSram3)
    *(.partition43BssSram3.*)

    /* address of the partition 43 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition43BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition43UssSram3Start = .;

    *(.partition43UssSram3)
    *(.partition43UssSram3.*)

    pharosPartition43UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_43_SRAM3_ALIGNMENT);

    /* address of the partition 43 sections end in Sram3 */
    pharosPartition43Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition43DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition43SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_43_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition43DataSramBackupStart = ABSOLUTE(.);

    *(.partition43DataSramBackup)
    *(.partition43DataSramBackup.*)

    /* address of the partition 43 data section end in RAM */
    . = ALIGN(8);
    pharosPartition43DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 43 in SramBackup */
  .pharosPartition43BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 43 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition43BssSramBackupStart = .;

    *(.partition43BssSramBackup)
    *(.partition43BssSramBackup.*)

    /* address of the partition 43 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition43BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition43UssSramBackupStart = .;

    *(.partition43UssSramBackup)
    *(.partition43UssSramBackup.*)

    pharosPartition43UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_43_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 43 sections end in SramBackup */
    pharosPartition43SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition43DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition43SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_43_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition43DataSdramStart = ABSOLUTE(.);

    *(.partition43DataSdram)
    *(.partition43DataSdram.*)

    /* address of the partition 43 data section end in RAM */
    . = ALIGN(8);
    pharosPartition43DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 43 in Sdram */
  .pharosPartition43BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 43 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition43BssSdramStart = .;

    *(.partition43BssSdram)
    *(.partition43BssSdram.*)

    /* address of the partition 43 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition43BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition43UssSdramStart = .;

    *(.partition43UssSdram)
    *(.partition43UssSdram.*)

    pharosPartition43UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_43_SDRAM_ALIGNMENT);

    /* address of the partition 43 sections end in Sdram */
    pharosPartition43SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 44                                                   */
  /******************************************************************/

  /* address in the flash of the partition 44 data to copy it to RAM */
  pharosPartition44DataCcmLoadAddr = LOADADDR(.partition44DataCcm);

  /* address in the flash of the partition 44 data to copy it to RAM */
  pharosPartition44DataSram1LoadAddr = LOADADDR(.partition44DataSram1);

  /* address in the flash of the partition 44 data to copy it to RAM */
  pharosPartition44DataSram2LoadAddr = LOADADDR(.partition44DataSram2);

  /* address in the flash of the partition 44 data to copy it to RAM */
  pharosPartition44DataSram3LoadAddr = LOADADDR(.partition44DataSram3);

  /* address in the flash of the partition 44 data to copy it to RAM */
  pharosPartition44DataSramBackupLoadAddr = LOADADDR(.partition44DataSramBackup);

  /* address in the flash of the partition 44 data to copy it to RAM */
  pharosPartition44DataSdramLoadAddr = LOADADDR(.partition44DataSdram);

  /* data of partition in Ccm */
  .partition44DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition44CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_44_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition44DataCcmStart = ABSOLUTE(.);

    *(.partition44DataCcm)
    *(.partition44DataCcm.*)

    /* address of the partition 44 data section end in RAM */
    . = ALIGN(8);
    pharosPartition44DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 44 in Ccm */
  .pharosPartition44BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 44 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition44BssCcmStart = .;

    *(.partition44BssCcm)
    *(.partition44BssCcm.*)

    /* address of the partition 44 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition44BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition44UssCcmStart = .;

    *(.partition44UssCcm)
    *(.partition44UssCcm.*)

    pharosPartition44UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_44_CCM_ALIGNMENT);

    /* address of the partition 44 sections end in Ccm */
    pharosPartition44CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition44DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition44Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_44_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition44DataSram1Start = ABSOLUTE(.);

    *(.partition44DataSram1)
    *(.partition44DataSram1.*)

    /* address of the partition 44 data section end in RAM */
    . = ALIGN(8);
    pharosPartition44DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 44 in Sram1 */
  .pharosPartition44BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 44 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition44BssSram1Start = .;

    *(.partition44BssSram1)
    *(.partition44BssSram1.*)

    /* address of the partition 44 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition44BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition44UssSram1Start = .;

    *(.partition44UssSram1)
    *(.partition44UssSram1.*)

    pharosPartition44UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_44_SRAM1_ALIGNMENT);

    /* address of the partition 44 sections end in Sram1 */
    pharosPartition44Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition44DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition44Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_44_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition44DataSram2Start = ABSOLUTE(.);

    *(.partition44DataSram2)
    *(.partition44DataSram2.*)

    /* address of the partition 44 data section end in RAM */
    . = ALIGN(8);
    pharosPartition44DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 44 in Sram2 */
  .pharosPartition44BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 44 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition44BssSram2Start = .;

    *(.partition44BssSram2)
    *(.partition44BssSram2.*)

    /* address of the partition 44 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition44BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition44UssSram2Start = .;

    *(.partition44UssSram2)
    *(.partition44UssSram2.*)

    pharosPartition44UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_44_SRAM2_ALIGNMENT);

    /* address of the partition 44 sections end in Sram2 */
    pharosPartition44Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition44DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition44Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_44_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition44DataSram3Start = ABSOLUTE(.);

    *(.partition44DataSram3)
    *(.partition44DataSram3.*)

    /* address of the partition 44 data section end in RAM */
    . = ALIGN(8);
    pharosPartition44DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 44 in Sram3 */
  .pharosPartition44BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 44 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition44BssSram3Start = .;

    *(.partition44BssSram3)
    *(.partition44BssSram3.*)

    /* address of the partition 44 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition44BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition44UssSram3Start = .;

    *(.partition44UssSram3)
    *(.partition44UssSram3.*)

    pharosPartition44UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_44_SRAM3_ALIGNMENT);

    /* address of the partition 44 sections end in Sram3 */
    pharosPartition44Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition44DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition44SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_44_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition44DataSramBackupStart = ABSOLUTE(.);

    *(.partition44DataSramBackup)
    *(.partition44DataSramBackup.*)

    /* address of the partition 44 data section end in RAM */
    . = ALIGN(8);
    pharosPartition44DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 44 in SramBackup */
  .pharosPartition44BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 44 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition44BssSramBackupStart = .;

    *(.partition44BssSramBackup)
    *(.partition44BssSramBackup.*)

    /* address of the partition 44 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition44BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition44UssSramBackupStart = .;

    *(.partition44UssSramBackup)
    *(.partition44UssSramBackup.*)

    pharosPartition44UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_44_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 44 sections end in SramBackup */
    pharosPartition44SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition44DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition44SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_44_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition44DataSdramStart = ABSOLUTE(.);

    *(.partition44DataSdram)
    *(.partition44DataSdram.*)

    /* address of the partition 44 data section end in RAM */
    . = ALIGN(8);
    pharosPartition44DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 44 in Sdram */
  .pharosPartition44BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 44 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition44BssSdramStart = .;

    *(.partition44BssSdram)
    *(.partition44BssSdram.*)

    /* address of the partition 44 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition44BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition44UssSdramStart = .;

    *(.partition44UssSdram)
    *(.partition44UssSdram.*)

    pharosPartition44UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_44_SDRAM_ALIGNMENT);

    /* address of the partition 44 sections end in Sdram */
    pharosPartition44SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 45                                                   */
  /******************************************************************/

  /* address in the flash of the partition 45 data to copy it to RAM */
  pharosPartition45DataCcmLoadAddr = LOADADDR(.partition45DataCcm);

  /* address in the flash of the partition 45 data to copy it to RAM */
  pharosPartition45DataSram1LoadAddr = LOADADDR(.partition45DataSram1);

  /* address in the flash of the partition 45 data to copy it to RAM */
  pharosPartition45DataSram2LoadAddr = LOADADDR(.partition45DataSram2);

  /* address in the flash of the partition 45 data to copy it to RAM */
  pharosPartition45DataSram3LoadAddr = LOADADDR(.partition45DataSram3);

  /* address in the flash of the partition 45 data to copy it to RAM */
  pharosPartition45DataSramBackupLoadAddr = LOADADDR(.partition45DataSramBackup);

  /* address in the flash of the partition 45 data to copy it to RAM */
  pharosPartition45DataSdramLoadAddr = LOADADDR(.partition45DataSdram);

  /* data of partition in Ccm */
  .partition45DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition45CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_45_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition45DataCcmStart = ABSOLUTE(.);

    *(.partition45DataCcm)
    *(.partition45DataCcm.*)

    /* address of the partition 45 data section end in RAM */
    . = ALIGN(8);
    pharosPartition45DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 45 in Ccm */
  .pharosPartition45BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 45 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition45BssCcmStart = .;

    *(.partition45BssCcm)
    *(.partition45BssCcm.*)

    /* address of the partition 45 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition45BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition45UssCcmStart = .;

    *(.partition45UssCcm)
    *(.partition45UssCcm.*)

    pharosPartition45UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_45_CCM_ALIGNMENT);

    /* address of the partition 45 sections end in Ccm */
    pharosPartition45CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition45DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition45Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_45_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition45DataSram1Start = ABSOLUTE(.);

    *(.partition45DataSram1)
    *(.partition45DataSram1.*)

    /* address of the partition 45 data section end in RAM */
    . = ALIGN(8);
    pharosPartition45DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 45 in Sram1 */
  .pharosPartition45BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 45 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition45BssSram1Start = .;

    *(.partition45BssSram1)
    *(.partition45BssSram1.*)

    /* address of the partition 45 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition45BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition45UssSram1Start = .;

    *(.partition45UssSram1)
    *(.partition45UssSram1.*)

    pharosPartition45UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_45_SRAM1_ALIGNMENT);

    /* address of the partition 45 sections end in Sram1 */
    pharosPartition45Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition45DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition45Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_45_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition45DataSram2Start = ABSOLUTE(.);

    *(.partition45DataSram2)
    *(.partition45DataSram2.*)

    /* address of the partition 45 data section end in RAM */
    . = ALIGN(8);
    pharosPartition45DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 45 in Sram2 */
  .pharosPartition45BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 45 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition45BssSram2Start = .;

    *(.partition45BssSram2)
    *(.partition45BssSram2.*)

    /* address of the partition 45 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition45BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition45UssSram2Start = .;

    *(.partition45UssSram2)
    *(.partition45UssSram2.*)

    pharosPartition45UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_45_SRAM2_ALIGNMENT);

    /* address of the partition 45 sections end in Sram2 */
    pharosPartition45Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition45DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition45Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_45_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition45DataSram3Start = ABSOLUTE(.);

    *(.partition45DataSram3)
    *(.partition45DataSram3.*)

    /* address of the partition 45 data section end in RAM */
    . = ALIGN(8);
    pharosPartition45DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 45 in Sram3 */
  .pharosPartition45BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 45 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition45BssSram3Start = .;

    *(.partition45BssSram3)
    *(.partition45BssSram3.*)

    /* address of the partition 45 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition45BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition45UssSram3Start = .;

    *(.partition45UssSram3)
    *(.partition45UssSram3.*)

    pharosPartition45UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_45_SRAM3_ALIGNMENT);

    /* address of the partition 45 sections end in Sram3 */
    pharosPartition45Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition45DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition45SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_45_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition45DataSramBackupStart = ABSOLUTE(.);

    *(.partition45DataSramBackup)
    *(.partition45DataSramBackup.*)

    /* address of the partition 45 data section end in RAM */
    . = ALIGN(8);
    pharosPartition45DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 45 in SramBackup */
  .pharosPartition45BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 45 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition45BssSramBackupStart = .;

    *(.partition45BssSramBackup)
    *(.partition45BssSramBackup.*)

    /* address of the partition 45 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition45BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition45UssSramBackupStart = .;

    *(.partition45UssSramBackup)
    *(.partition45UssSramBackup.*)

    pharosPartition45UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_45_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 45 sections end in SramBackup */
    pharosPartition45SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition45DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition45SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_45_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition45DataSdramStart = ABSOLUTE(.);

    *(.partition45DataSdram)
    *(.partition45DataSdram.*)

    /* address of the partition 45 data section end in RAM */
    . = ALIGN(8);
    pharosPartition45DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 45 in Sdram */
  .pharosPartition45BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 45 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition45BssSdramStart = .;

    *(.partition45BssSdram)
    *(.partition45BssSdram.*)

    /* address of the partition 45 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition45BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition45UssSdramStart = .;

    *(.partition45UssSdram)
    *(.partition45UssSdram.*)

    pharosPartition45UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_45_SDRAM_ALIGNMENT);

    /* address of the partition 45 sections end in Sdram */
    pharosPartition45SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 46                                                   */
  /******************************************************************/

  /* address in the flash of the partition 46 data to copy it to RAM */
  pharosPartition46DataCcmLoadAddr = LOADADDR(.partition46DataCcm);

  /* address in the flash of the partition 46 data to copy it to RAM */
  pharosPartition46DataSram1LoadAddr = LOADADDR(.partition46DataSram1);

  /* address in the flash of the partition 46 data to copy it to RAM */
  pharosPartition46DataSram2LoadAddr = LOADADDR(.partition46DataSram2);

  /* address in the flash of the partition 46 data to copy it to RAM */
  pharosPartition46DataSram3LoadAddr = LOADADDR(.partition46DataSram3);

  /* address in the flash of the partition 46 data to copy it to RAM */
  pharosPartition46DataSramBackupLoadAddr = LOADADDR(.partition46DataSramBackup);

  /* address in the flash of the partition 46 data to copy it to RAM */
  pharosPartition46DataSdramLoadAddr = LOADADDR(.partition46DataSdram);

  /* data of partition in Ccm */
  .partition46DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition46CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_46_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition46DataCcmStart = ABSOLUTE(.);

    *(.partition46DataCcm)
    *(.partition46DataCcm.*)

    /* address of the partition 46 data section end in RAM */
    . = ALIGN(8);
    pharosPartition46DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 46 in Ccm */
  .pharosPartition46BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 46 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition46BssCcmStart = .;

    *(.partition46BssCcm)
    *(.partition46BssCcm.*)

    /* address of the partition 46 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition46BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition46UssCcmStart = .;

    *(.partition46UssCcm)
    *(.partition46UssCcm.*)

    pharosPartition46UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_46_CCM_ALIGNMENT);

    /* address of the partition 46 sections end in Ccm */
    pharosPartition46CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition46DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition46Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_46_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition46DataSram1Start = ABSOLUTE(.);

    *(.partition46DataSram1)
    *(.partition46DataSram1.*)

    /* address of the partition 46 data section end in RAM */
    . = ALIGN(8);
    pharosPartition46DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 46 in Sram1 */
  .pharosPartition46BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 46 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition46BssSram1Start = .;

    *(.partition46BssSram1)
    *(.partition46BssSram1.*)

    /* address of the partition 46 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition46BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition46UssSram1Start = .;

    *(.partition46UssSram1)
    *(.partition46UssSram1.*)

    pharosPartition46UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_46_SRAM1_ALIGNMENT);

    /* address of the partition 46 sections end in Sram1 */
    pharosPartition46Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition46DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition46Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_46_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition46DataSram2Start = ABSOLUTE(.);

    *(.partition46DataSram2)
    *(.partition46DataSram2.*)

    /* address of the partition 46 data section end in RAM */
    . = ALIGN(8);
    pharosPartition46DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 46 in Sram2 */
  .pharosPartition46BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 46 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition46BssSram2Start = .;

    *(.partition46BssSram2)
    *(.partition46BssSram2.*)

    /* address of the partition 46 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition46BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition46UssSram2Start = .;

    *(.partition46UssSram2)
    *(.partition46UssSram2.*)

    pharosPartition46UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_46_SRAM2_ALIGNMENT);

    /* address of the partition 46 sections end in Sram2 */
    pharosPartition46Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition46DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition46Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_46_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition46DataSram3Start = ABSOLUTE(.);

    *(.partition46DataSram3)
    *(.partition46DataSram3.*)

    /* address of the partition 46 data section end in RAM */
    . = ALIGN(8);
    pharosPartition46DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 46 in Sram3 */
  .pharosPartition46BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 46 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition46BssSram3Start = .;

    *(.partition46BssSram3)
    *(.partition46BssSram3.*)

    /* address of the partition 46 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition46BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition46UssSram3Start = .;

    *(.partition46UssSram3)
    *(.partition46UssSram3.*)

    pharosPartition46UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_46_SRAM3_ALIGNMENT);

    /* address of the partition 46 sections end in Sram3 */
    pharosPartition46Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition46DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition46SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_46_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition46DataSramBackupStart = ABSOLUTE(.);

    *(.partition46DataSramBackup)
    *(.partition46DataSramBackup.*)

    /* address of the partition 46 data section end in RAM */
    . = ALIGN(8);
    pharosPartition46DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 46 in SramBackup */
  .pharosPartition46BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 46 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition46BssSramBackupStart = .;

    *(.partition46BssSramBackup)
    *(.partition46BssSramBackup.*)

    /* address of the partition 46 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition46BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition46UssSramBackupStart = .;

    *(.partition46UssSramBackup)
    *(.partition46UssSramBackup.*)

    pharosPartition46UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_46_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 46 sections end in SramBackup */
    pharosPartition46SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition46DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition46SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_46_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition46DataSdramStart = ABSOLUTE(.);

    *(.partition46DataSdram)
    *(.partition46DataSdram.*)

    /* address of the partition 46 data section end in RAM */
    . = ALIGN(8);
    pharosPartition46DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 46 in Sdram */
  .pharosPartition46BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 46 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition46BssSdramStart = .;

    *(.partition46BssSdram)
    *(.partition46BssSdram.*)

    /* address of the partition 46 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition46BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition46UssSdramStart = .;

    *(.partition46UssSdram)
    *(.partition46UssSdram.*)

    pharosPartition46UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_46_SDRAM_ALIGNMENT);

    /* address of the partition 46 sections end in Sdram */
    pharosPartition46SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 47                                                   */
  /******************************************************************/

  /* address in the flash of the partition 47 data to copy it to RAM */
  pharosPartition47DataCcmLoadAddr = LOADADDR(.partition47DataCcm);

  /* address in the flash of the partition 47 data to copy it to RAM */
  pharosPartition47DataSram1LoadAddr = LOADADDR(.partition47DataSram1);

  /* address in the flash of the partition 47 data to copy it to RAM */
  pharosPartition47DataSram2LoadAddr = LOADADDR(.partition47DataSram2);

  /* address in the flash of the partition 47 data to copy it to RAM */
  pharosPartition47DataSram3LoadAddr = LOADADDR(.partition47DataSram3);

  /* address in the flash of the partition 47 data to copy it to RAM */
  pharosPartition47DataSramBackupLoadAddr = LOADADDR(.partition47DataSramBackup);

  /* address in the flash of the partition 47 data to copy it to RAM */
  pharosPartition47DataSdramLoadAddr = LOADADDR(.partition47DataSdram);

  /* data of partition in Ccm */
  .partition47DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition47CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_47_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition47DataCcmStart = ABSOLUTE(.);

    *(.partition47DataCcm)
    *(.partition47DataCcm.*)

    /* address of the partition 47 data section end in RAM */
    . = ALIGN(8);
    pharosPartition47DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 47 in Ccm */
  .pharosPartition47BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 47 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition47BssCcmStart = .;

    *(.partition47BssCcm)
    *(.partition47BssCcm.*)

    /* address of the partition 47 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition47BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition47UssCcmStart = .;

    *(.partition47UssCcm)
    *(.partition47UssCcm.*)

    pharosPartition47UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_47_CCM_ALIGNMENT);

    /* address of the partition 47 sections end in Ccm */
    pharosPartition47CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition47DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition47Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_47_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition47DataSram1Start = ABSOLUTE(.);

    *(.partition47DataSram1)
    *(.partition47DataSram1.*)

    /* address of the partition 47 data section end in RAM */
    . = ALIGN(8);
    pharosPartition47DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 47 in Sram1 */
  .pharosPartition47BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 47 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition47BssSram1Start = .;

    *(.partition47BssSram1)
    *(.partition47BssSram1.*)

    /* address of the partition 47 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition47BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition47UssSram1Start = .;

    *(.partition47UssSram1)
    *(.partition47UssSram1.*)

    pharosPartition47UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_47_SRAM1_ALIGNMENT);

    /* address of the partition 47 sections end in Sram1 */
    pharosPartition47Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition47DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition47Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_47_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition47DataSram2Start = ABSOLUTE(.);

    *(.partition47DataSram2)
    *(.partition47DataSram2.*)

    /* address of the partition 47 data section end in RAM */
    . = ALIGN(8);
    pharosPartition47DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 47 in Sram2 */
  .pharosPartition47BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 47 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition47BssSram2Start = .;

    *(.partition47BssSram2)
    *(.partition47BssSram2.*)

    /* address of the partition 47 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition47BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition47UssSram2Start = .;

    *(.partition47UssSram2)
    *(.partition47UssSram2.*)

    pharosPartition47UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_47_SRAM2_ALIGNMENT);

    /* address of the partition 47 sections end in Sram2 */
    pharosPartition47Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition47DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition47Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_47_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition47DataSram3Start = ABSOLUTE(.);

    *(.partition47DataSram3)
    *(.partition47DataSram3.*)

    /* address of the partition 47 data section end in RAM */
    . = ALIGN(8);
    pharosPartition47DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 47 in Sram3 */
  .pharosPartition47BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 47 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition47BssSram3Start = .;

    *(.partition47BssSram3)
    *(.partition47BssSram3.*)

    /* address of the partition 47 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition47BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition47UssSram3Start = .;

    *(.partition47UssSram3)
    *(.partition47UssSram3.*)

    pharosPartition47UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_47_SRAM3_ALIGNMENT);

    /* address of the partition 47 sections end in Sram3 */
    pharosPartition47Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition47DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition47SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_47_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition47DataSramBackupStart = ABSOLUTE(.);

    *(.partition47DataSramBackup)
    *(.partition47DataSramBackup.*)

    /* address of the partition 47 data section end in RAM */
    . = ALIGN(8);
    pharosPartition47DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 47 in SramBackup */
  .pharosPartition47BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 47 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition47BssSramBackupStart = .;

    *(.partition47BssSramBackup)
    *(.partition47BssSramBackup.*)

    /* address of the partition 47 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition47BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition47UssSramBackupStart = .;

    *(.partition47UssSramBackup)
    *(.partition47UssSramBackup.*)

    pharosPartition47UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_47_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 47 sections end in SramBackup */
    pharosPartition47SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition47DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition47SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_47_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition47DataSdramStart = ABSOLUTE(.);

    *(.partition47DataSdram)
    *(.partition47DataSdram.*)

    /* address of the partition 47 data section end in RAM */
    . = ALIGN(8);
    pharosPartition47DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 47 in Sdram */
  .pharosPartition47BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 47 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition47BssSdramStart = .;

    *(.partition47BssSdram)
    *(.partition47BssSdram.*)

    /* address of the partition 47 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition47BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition47UssSdramStart = .;

    *(.partition47UssSdram)
    *(.partition47UssSdram.*)

    pharosPartition47UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_47_SDRAM_ALIGNMENT);

    /* address of the partition 47 sections end in Sdram */
    pharosPartition47SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 48                                                   */
  /******************************************************************/

  /* address in the flash of the partition 48 data to copy it to RAM */
  pharosPartition48DataCcmLoadAddr = LOADADDR(.partition48DataCcm);

  /* address in the flash of the partition 48 data to copy it to RAM */
  pharosPartition48DataSram1LoadAddr = LOADADDR(.partition48DataSram1);

  /* address in the flash of the partition 48 data to copy it to RAM */
  pharosPartition48DataSram2LoadAddr = LOADADDR(.partition48DataSram2);

  /* address in the flash of the partition 48 data to copy it to RAM */
  pharosPartition48DataSram3LoadAddr = LOADADDR(.partition48DataSram3);

  /* address in the flash of the partition 48 data to copy it to RAM */
  pharosPartition48DataSramBackupLoadAddr = LOADADDR(.partition48DataSramBackup);

  /* address in the flash of the partition 48 data to copy it to RAM */
  pharosPartition48DataSdramLoadAddr = LOADADDR(.partition48DataSdram);

  /* data of partition in Ccm */
  .partition48DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition48CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_48_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition48DataCcmStart = ABSOLUTE(.);

    *(.partition48DataCcm)
    *(.partition48DataCcm.*)

    /* address of the partition 48 data section end in RAM */
    . = ALIGN(8);
    pharosPartition48DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 48 in Ccm */
  .pharosPartition48BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 48 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition48BssCcmStart = .;

    *(.partition48BssCcm)
    *(.partition48BssCcm.*)

    /* address of the partition 48 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition48BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition48UssCcmStart = .;

    *(.partition48UssCcm)
    *(.partition48UssCcm.*)

    pharosPartition48UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_48_CCM_ALIGNMENT);

    /* address of the partition 48 sections end in Ccm */
    pharosPartition48CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition48DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition48Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_48_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition48DataSram1Start = ABSOLUTE(.);

    *(.partition48DataSram1)
    *(.partition48DataSram1.*)

    /* address of the partition 48 data section end in RAM */
    . = ALIGN(8);
    pharosPartition48DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 48 in Sram1 */
  .pharosPartition48BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 48 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition48BssSram1Start = .;

    *(.partition48BssSram1)
    *(.partition48BssSram1.*)

    /* address of the partition 48 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition48BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition48UssSram1Start = .;

    *(.partition48UssSram1)
    *(.partition48UssSram1.*)

    pharosPartition48UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_48_SRAM1_ALIGNMENT);

    /* address of the partition 48 sections end in Sram1 */
    pharosPartition48Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition48DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition48Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_48_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition48DataSram2Start = ABSOLUTE(.);

    *(.partition48DataSram2)
    *(.partition48DataSram2.*)

    /* address of the partition 48 data section end in RAM */
    . = ALIGN(8);
    pharosPartition48DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 48 in Sram2 */
  .pharosPartition48BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 48 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition48BssSram2Start = .;

    *(.partition48BssSram2)
    *(.partition48BssSram2.*)

    /* address of the partition 48 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition48BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition48UssSram2Start = .;

    *(.partition48UssSram2)
    *(.partition48UssSram2.*)

    pharosPartition48UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_48_SRAM2_ALIGNMENT);

    /* address of the partition 48 sections end in Sram2 */
    pharosPartition48Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition48DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition48Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_48_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition48DataSram3Start = ABSOLUTE(.);

    *(.partition48DataSram3)
    *(.partition48DataSram3.*)

    /* address of the partition 48 data section end in RAM */
    . = ALIGN(8);
    pharosPartition48DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 48 in Sram3 */
  .pharosPartition48BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 48 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition48BssSram3Start = .;

    *(.partition48BssSram3)
    *(.partition48BssSram3.*)

    /* address of the partition 48 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition48BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition48UssSram3Start = .;

    *(.partition48UssSram3)
    *(.partition48UssSram3.*)

    pharosPartition48UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_48_SRAM3_ALIGNMENT);

    /* address of the partition 48 sections end in Sram3 */
    pharosPartition48Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition48DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition48SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_48_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition48DataSramBackupStart = ABSOLUTE(.);

    *(.partition48DataSramBackup)
    *(.partition48DataSramBackup.*)

    /* address of the partition 48 data section end in RAM */
    . = ALIGN(8);
    pharosPartition48DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 48 in SramBackup */
  .pharosPartition48BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 48 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition48BssSramBackupStart = .;

    *(.partition48BssSramBackup)
    *(.partition48BssSramBackup.*)

    /* address of the partition 48 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition48BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition48UssSramBackupStart = .;

    *(.partition48UssSramBackup)
    *(.partition48UssSramBackup.*)

    pharosPartition48UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_48_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 48 sections end in SramBackup */
    pharosPartition48SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition48DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition48SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_48_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition48DataSdramStart = ABSOLUTE(.);

    *(.partition48DataSdram)
    *(.partition48DataSdram.*)

    /* address of the partition 48 data section end in RAM */
    . = ALIGN(8);
    pharosPartition48DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 48 in Sdram */
  .pharosPartition48BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 48 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition48BssSdramStart = .;

    *(.partition48BssSdram)
    *(.partition48BssSdram.*)

    /* address of the partition 48 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition48BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition48UssSdramStart = .;

    *(.partition48UssSdram)
    *(.partition48UssSdram.*)

    pharosPartition48UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_48_SDRAM_ALIGNMENT);

    /* address of the partition 48 sections end in Sdram */
    pharosPartition48SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 49                                                   */
  /******************************************************************/

  /* address in the flash of the partition 49 data to copy it to RAM */
  pharosPartition49DataCcmLoadAddr = LOADADDR(.partition49DataCcm);

  /* address in the flash of the partition 49 data to copy it to RAM */
  pharosPartition49DataSram1LoadAddr = LOADADDR(.partition49DataSram1);

  /* address in the flash of the partition 49 data to copy it to RAM */
  pharosPartition49DataSram2LoadAddr = LOADADDR(.partition49DataSram2);

  /* address in the flash of the partition 49 data to copy it to RAM */
  pharosPartition49DataSram3LoadAddr = LOADADDR(.partition49DataSram3);

  /* address in the flash of the partition 49 data to copy it to RAM */
  pharosPartition49DataSramBackupLoadAddr = LOADADDR(.partition49DataSramBackup);

  /* address in the flash of the partition 49 data to copy it to RAM */
  pharosPartition49DataSdramLoadAddr = LOADADDR(.partition49DataSdram);

  /* data of partition in Ccm */
  .partition49DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition49CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_49_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition49DataCcmStart = ABSOLUTE(.);

    *(.partition49DataCcm)
    *(.partition49DataCcm.*)

    /* address of the partition 49 data section end in RAM */
    . = ALIGN(8);
    pharosPartition49DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 49 in Ccm */
  .pharosPartition49BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 49 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition49BssCcmStart = .;

    *(.partition49BssCcm)
    *(.partition49BssCcm.*)

    /* address of the partition 49 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition49BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition49UssCcmStart = .;

    *(.partition49UssCcm)
    *(.partition49UssCcm.*)

    pharosPartition49UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_49_CCM_ALIGNMENT);

    /* address of the partition 49 sections end in Ccm */
    pharosPartition49CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition49DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition49Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_49_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition49DataSram1Start = ABSOLUTE(.);

    *(.partition49DataSram1)
    *(.partition49DataSram1.*)

    /* address of the partition 49 data section end in RAM */
    . = ALIGN(8);
    pharosPartition49DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 49 in Sram1 */
  .pharosPartition49BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 49 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition49BssSram1Start = .;

    *(.partition49BssSram1)
    *(.partition49BssSram1.*)

    /* address of the partition 49 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition49BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition49UssSram1Start = .;

    *(.partition49UssSram1)
    *(.partition49UssSram1.*)

    pharosPartition49UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_49_SRAM1_ALIGNMENT);

    /* address of the partition 49 sections end in Sram1 */
    pharosPartition49Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition49DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition49Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_49_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition49DataSram2Start = ABSOLUTE(.);

    *(.partition49DataSram2)
    *(.partition49DataSram2.*)

    /* address of the partition 49 data section end in RAM */
    . = ALIGN(8);
    pharosPartition49DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 49 in Sram2 */
  .pharosPartition49BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 49 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition49BssSram2Start = .;

    *(.partition49BssSram2)
    *(.partition49BssSram2.*)

    /* address of the partition 49 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition49BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition49UssSram2Start = .;

    *(.partition49UssSram2)
    *(.partition49UssSram2.*)

    pharosPartition49UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_49_SRAM2_ALIGNMENT);

    /* address of the partition 49 sections end in Sram2 */
    pharosPartition49Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition49DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition49Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_49_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition49DataSram3Start = ABSOLUTE(.);

    *(.partition49DataSram3)
    *(.partition49DataSram3.*)

    /* address of the partition 49 data section end in RAM */
    . = ALIGN(8);
    pharosPartition49DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 49 in Sram3 */
  .pharosPartition49BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 49 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition49BssSram3Start = .;

    *(.partition49BssSram3)
    *(.partition49BssSram3.*)

    /* address of the partition 49 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition49BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition49UssSram3Start = .;

    *(.partition49UssSram3)
    *(.partition49UssSram3.*)

    pharosPartition49UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_49_SRAM3_ALIGNMENT);

    /* address of the partition 49 sections end in Sram3 */
    pharosPartition49Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition49DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition49SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_49_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition49DataSramBackupStart = ABSOLUTE(.);

    *(.partition49DataSramBackup)
    *(.partition49DataSramBackup.*)

    /* address of the partition 49 data section end in RAM */
    . = ALIGN(8);
    pharosPartition49DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 49 in SramBackup */
  .pharosPartition49BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 49 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition49BssSramBackupStart = .;

    *(.partition49BssSramBackup)
    *(.partition49BssSramBackup.*)

    /* address of the partition 49 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition49BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition49UssSramBackupStart = .;

    *(.partition49UssSramBackup)
    *(.partition49UssSramBackup.*)

    pharosPartition49UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_49_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 49 sections end in SramBackup */
    pharosPartition49SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition49DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition49SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_49_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition49DataSdramStart = ABSOLUTE(.);

    *(.partition49DataSdram)
    *(.partition49DataSdram.*)

    /* address of the partition 49 data section end in RAM */
    . = ALIGN(8);
    pharosPartition49DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 49 in Sdram */
  .pharosPartition49BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 49 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition49BssSdramStart = .;

    *(.partition49BssSdram)
    *(.partition49BssSdram.*)

    /* address of the partition 49 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition49BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition49UssSdramStart = .;

    *(.partition49UssSdram)
    *(.partition49UssSdram.*)

    pharosPartition49UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_49_SDRAM_ALIGNMENT);

    /* address of the partition 49 sections end in Sdram */
    pharosPartition49SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 50                                                   */
  /******************************************************************/

  /* address in the flash of the partition 50 data to copy it to RAM */
  pharosPartition50DataCcmLoadAddr = LOADADDR(.partition50DataCcm);

  /* address in the flash of the partition 50 data to copy it to RAM */
  pharosPartition50DataSram1LoadAddr = LOADADDR(.partition50DataSram1);

  /* address in the flash of the partition 50 data to copy it to RAM */
  pharosPartition50DataSram2LoadAddr = LOADADDR(.partition50DataSram2);

  /* address in the flash of the partition 50 data to copy it to RAM */
  pharosPartition50DataSram3LoadAddr = LOADADDR(.partition50DataSram3);

  /* address in the flash of the partition 50 data to copy it to RAM */
  pharosPartition50DataSramBackupLoadAddr = LOADADDR(.partition50DataSramBackup);

  /* address in the flash of the partition 50 data to copy it to RAM */
  pharosPartition50DataSdramLoadAddr = LOADADDR(.partition50DataSdram);

  /* data of partition in Ccm */
  .partition50DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition50CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_50_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition50DataCcmStart = ABSOLUTE(.);

    *(.partition50DataCcm)
    *(.partition50DataCcm.*)

    /* address of the partition 50 data section end in RAM */
    . = ALIGN(8);
    pharosPartition50DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 50 in Ccm */
  .pharosPartition50BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 50 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition50BssCcmStart = .;

    *(.partition50BssCcm)
    *(.partition50BssCcm.*)

    /* address of the partition 50 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition50BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition50UssCcmStart = .;

    *(.partition50UssCcm)
    *(.partition50UssCcm.*)

    pharosPartition50UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_50_CCM_ALIGNMENT);

    /* address of the partition 50 sections end in Ccm */
    pharosPartition50CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition50DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition50Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_50_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition50DataSram1Start = ABSOLUTE(.);

    *(.partition50DataSram1)
    *(.partition50DataSram1.*)

    /* address of the partition 50 data section end in RAM */
    . = ALIGN(8);
    pharosPartition50DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 50 in Sram1 */
  .pharosPartition50BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 50 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition50BssSram1Start = .;

    *(.partition50BssSram1)
    *(.partition50BssSram1.*)

    /* address of the partition 50 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition50BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition50UssSram1Start = .;

    *(.partition50UssSram1)
    *(.partition50UssSram1.*)

    pharosPartition50UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_50_SRAM1_ALIGNMENT);

    /* address of the partition 50 sections end in Sram1 */
    pharosPartition50Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition50DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition50Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_50_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition50DataSram2Start = ABSOLUTE(.);

    *(.partition50DataSram2)
    *(.partition50DataSram2.*)

    /* address of the partition 50 data section end in RAM */
    . = ALIGN(8);
    pharosPartition50DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 50 in Sram2 */
  .pharosPartition50BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 50 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition50BssSram2Start = .;

    *(.partition50BssSram2)
    *(.partition50BssSram2.*)

    /* address of the partition 50 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition50BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition50UssSram2Start = .;

    *(.partition50UssSram2)
    *(.partition50UssSram2.*)

    pharosPartition50UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_50_SRAM2_ALIGNMENT);

    /* address of the partition 50 sections end in Sram2 */
    pharosPartition50Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition50DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition50Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_50_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition50DataSram3Start = ABSOLUTE(.);

    *(.partition50DataSram3)
    *(.partition50DataSram3.*)

    /* address of the partition 50 data section end in RAM */
    . = ALIGN(8);
    pharosPartition50DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 50 in Sram3 */
  .pharosPartition50BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 50 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition50BssSram3Start = .;

    *(.partition50BssSram3)
    *(.partition50BssSram3.*)

    /* address of the partition 50 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition50BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition50UssSram3Start = .;

    *(.partition50UssSram3)
    *(.partition50UssSram3.*)

    pharosPartition50UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_50_SRAM3_ALIGNMENT);

    /* address of the partition 50 sections end in Sram3 */
    pharosPartition50Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition50DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition50SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_50_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition50DataSramBackupStart = ABSOLUTE(.);

    *(.partition50DataSramBackup)
    *(.partition50DataSramBackup.*)

    /* address of the partition 50 data section end in RAM */
    . = ALIGN(8);
    pharosPartition50DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 50 in SramBackup */
  .pharosPartition50BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 50 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition50BssSramBackupStart = .;

    *(.partition50BssSramBackup)
    *(.partition50BssSramBackup.*)

    /* address of the partition 50 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition50BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition50UssSramBackupStart = .;

    *(.partition50UssSramBackup)
    *(.partition50UssSramBackup.*)

    pharosPartition50UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_50_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 50 sections end in SramBackup */
    pharosPartition50SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition50DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition50SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_50_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition50DataSdramStart = ABSOLUTE(.);

    *(.partition50DataSdram)
    *(.partition50DataSdram.*)

    /* address of the partition 50 data section end in RAM */
    . = ALIGN(8);
    pharosPartition50DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 50 in Sdram */
  .pharosPartition50BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 50 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition50BssSdramStart = .;

    *(.partition50BssSdram)
    *(.partition50BssSdram.*)

    /* address of the partition 50 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition50BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition50UssSdramStart = .;

    *(.partition50UssSdram)
    *(.partition50UssSdram.*)

    pharosPartition50UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_50_SDRAM_ALIGNMENT);

    /* address of the partition 50 sections end in Sdram */
    pharosPartition50SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 51                                                   */
  /******************************************************************/

  /* address in the flash of the partition 51 data to copy it to RAM */
  pharosPartition51DataCcmLoadAddr = LOADADDR(.partition51DataCcm);

  /* address in the flash of the partition 51 data to copy it to RAM */
  pharosPartition51DataSram1LoadAddr = LOADADDR(.partition51DataSram1);

  /* address in the flash of the partition 51 data to copy it to RAM */
  pharosPartition51DataSram2LoadAddr = LOADADDR(.partition51DataSram2);

  /* address in the flash of the partition 51 data to copy it to RAM */
  pharosPartition51DataSram3LoadAddr = LOADADDR(.partition51DataSram3);

  /* address in the flash of the partition 51 data to copy it to RAM */
  pharosPartition51DataSramBackupLoadAddr = LOADADDR(.partition51DataSramBackup);

  /* address in the flash of the partition 51 data to copy it to RAM */
  pharosPartition51DataSdramLoadAddr = LOADADDR(.partition51DataSdram);

  /* data of partition in Ccm */
  .partition51DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition51CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_51_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition51DataCcmStart = ABSOLUTE(.);

    *(.partition51DataCcm)
    *(.partition51DataCcm.*)

    /* address of the partition 51 data section end in RAM */
    . = ALIGN(8);
    pharosPartition51DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 51 in Ccm */
  .pharosPartition51BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 51 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition51BssCcmStart = .;

    *(.partition51BssCcm)
    *(.partition51BssCcm.*)

    /* address of the partition 51 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition51BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition51UssCcmStart = .;

    *(.partition51UssCcm)
    *(.partition51UssCcm.*)

    pharosPartition51UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_51_CCM_ALIGNMENT);

    /* address of the partition 51 sections end in Ccm */
    pharosPartition51CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition51DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition51Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_51_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition51DataSram1Start = ABSOLUTE(.);

    *(.partition51DataSram1)
    *(.partition51DataSram1.*)

    /* address of the partition 51 data section end in RAM */
    . = ALIGN(8);
    pharosPartition51DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 51 in Sram1 */
  .pharosPartition51BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 51 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition51BssSram1Start = .;

    *(.partition51BssSram1)
    *(.partition51BssSram1.*)

    /* address of the partition 51 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition51BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition51UssSram1Start = .;

    *(.partition51UssSram1)
    *(.partition51UssSram1.*)

    pharosPartition51UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_51_SRAM1_ALIGNMENT);

    /* address of the partition 51 sections end in Sram1 */
    pharosPartition51Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition51DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition51Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_51_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition51DataSram2Start = ABSOLUTE(.);

    *(.partition51DataSram2)
    *(.partition51DataSram2.*)

    /* address of the partition 51 data section end in RAM */
    . = ALIGN(8);
    pharosPartition51DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 51 in Sram2 */
  .pharosPartition51BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 51 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition51BssSram2Start = .;

    *(.partition51BssSram2)
    *(.partition51BssSram2.*)

    /* address of the partition 51 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition51BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition51UssSram2Start = .;

    *(.partition51UssSram2)
    *(.partition51UssSram2.*)

    pharosPartition51UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_51_SRAM2_ALIGNMENT);

    /* address of the partition 51 sections end in Sram2 */
    pharosPartition51Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition51DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition51Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_51_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition51DataSram3Start = ABSOLUTE(.);

    *(.partition51DataSram3)
    *(.partition51DataSram3.*)

    /* address of the partition 51 data section end in RAM */
    . = ALIGN(8);
    pharosPartition51DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 51 in Sram3 */
  .pharosPartition51BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 51 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition51BssSram3Start = .;

    *(.partition51BssSram3)
    *(.partition51BssSram3.*)

    /* address of the partition 51 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition51BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition51UssSram3Start = .;

    *(.partition51UssSram3)
    *(.partition51UssSram3.*)

    pharosPartition51UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_51_SRAM3_ALIGNMENT);

    /* address of the partition 51 sections end in Sram3 */
    pharosPartition51Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition51DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition51SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_51_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition51DataSramBackupStart = ABSOLUTE(.);

    *(.partition51DataSramBackup)
    *(.partition51DataSramBackup.*)

    /* address of the partition 51 data section end in RAM */
    . = ALIGN(8);
    pharosPartition51DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 51 in SramBackup */
  .pharosPartition51BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 51 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition51BssSramBackupStart = .;

    *(.partition51BssSramBackup)
    *(.partition51BssSramBackup.*)

    /* address of the partition 51 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition51BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition51UssSramBackupStart = .;

    *(.partition51UssSramBackup)
    *(.partition51UssSramBackup.*)

    pharosPartition51UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_51_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 51 sections end in SramBackup */
    pharosPartition51SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition51DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition51SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_51_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition51DataSdramStart = ABSOLUTE(.);

    *(.partition51DataSdram)
    *(.partition51DataSdram.*)

    /* address of the partition 51 data section end in RAM */
    . = ALIGN(8);
    pharosPartition51DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 51 in Sdram */
  .pharosPartition51BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 51 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition51BssSdramStart = .;

    *(.partition51BssSdram)
    *(.partition51BssSdram.*)

    /* address of the partition 51 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition51BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition51UssSdramStart = .;

    *(.partition51UssSdram)
    *(.partition51UssSdram.*)

    pharosPartition51UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_51_SDRAM_ALIGNMENT);

    /* address of the partition 51 sections end in Sdram */
    pharosPartition51SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 52                                                   */
  /******************************************************************/

  /* address in the flash of the partition 52 data to copy it to RAM */
  pharosPartition52DataCcmLoadAddr = LOADADDR(.partition52DataCcm);

  /* address in the flash of the partition 52 data to copy it to RAM */
  pharosPartition52DataSram1LoadAddr = LOADADDR(.partition52DataSram1);

  /* address in the flash of the partition 52 data to copy it to RAM */
  pharosPartition52DataSram2LoadAddr = LOADADDR(.partition52DataSram2);

  /* address in the flash of the partition 52 data to copy it to RAM */
  pharosPartition52DataSram3LoadAddr = LOADADDR(.partition52DataSram3);

  /* address in the flash of the partition 52 data to copy it to RAM */
  pharosPartition52DataSramBackupLoadAddr = LOADADDR(.partition52DataSramBackup);

  /* address in the flash of the partition 52 data to copy it to RAM */
  pharosPartition52DataSdramLoadAddr = LOADADDR(.partition52DataSdram);

  /* data of partition in Ccm */
  .partition52DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition52CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_52_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition52DataCcmStart = ABSOLUTE(.);

    *(.partition52DataCcm)
    *(.partition52DataCcm.*)

    /* address of the partition 52 data section end in RAM */
    . = ALIGN(8);
    pharosPartition52DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 52 in Ccm */
  .pharosPartition52BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 52 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition52BssCcmStart = .;

    *(.partition52BssCcm)
    *(.partition52BssCcm.*)

    /* address of the partition 52 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition52BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition52UssCcmStart = .;

    *(.partition52UssCcm)
    *(.partition52UssCcm.*)

    pharosPartition52UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_52_CCM_ALIGNMENT);

    /* address of the partition 52 sections end in Ccm */
    pharosPartition52CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition52DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition52Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_52_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition52DataSram1Start = ABSOLUTE(.);

    *(.partition52DataSram1)
    *(.partition52DataSram1.*)

    /* address of the partition 52 data section end in RAM */
    . = ALIGN(8);
    pharosPartition52DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 52 in Sram1 */
  .pharosPartition52BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 52 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition52BssSram1Start = .;

    *(.partition52BssSram1)
    *(.partition52BssSram1.*)

    /* address of the partition 52 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition52BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition52UssSram1Start = .;

    *(.partition52UssSram1)
    *(.partition52UssSram1.*)

    pharosPartition52UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_52_SRAM1_ALIGNMENT);

    /* address of the partition 52 sections end in Sram1 */
    pharosPartition52Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition52DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition52Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_52_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition52DataSram2Start = ABSOLUTE(.);

    *(.partition52DataSram2)
    *(.partition52DataSram2.*)

    /* address of the partition 52 data section end in RAM */
    . = ALIGN(8);
    pharosPartition52DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 52 in Sram2 */
  .pharosPartition52BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 52 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition52BssSram2Start = .;

    *(.partition52BssSram2)
    *(.partition52BssSram2.*)

    /* address of the partition 52 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition52BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition52UssSram2Start = .;

    *(.partition52UssSram2)
    *(.partition52UssSram2.*)

    pharosPartition52UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_52_SRAM2_ALIGNMENT);

    /* address of the partition 52 sections end in Sram2 */
    pharosPartition52Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition52DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition52Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_52_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition52DataSram3Start = ABSOLUTE(.);

    *(.partition52DataSram3)
    *(.partition52DataSram3.*)

    /* address of the partition 52 data section end in RAM */
    . = ALIGN(8);
    pharosPartition52DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 52 in Sram3 */
  .pharosPartition52BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 52 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition52BssSram3Start = .;

    *(.partition52BssSram3)
    *(.partition52BssSram3.*)

    /* address of the partition 52 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition52BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition52UssSram3Start = .;

    *(.partition52UssSram3)
    *(.partition52UssSram3.*)

    pharosPartition52UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_52_SRAM3_ALIGNMENT);

    /* address of the partition 52 sections end in Sram3 */
    pharosPartition52Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition52DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition52SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_52_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition52DataSramBackupStart = ABSOLUTE(.);

    *(.partition52DataSramBackup)
    *(.partition52DataSramBackup.*)

    /* address of the partition 52 data section end in RAM */
    . = ALIGN(8);
    pharosPartition52DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 52 in SramBackup */
  .pharosPartition52BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 52 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition52BssSramBackupStart = .;

    *(.partition52BssSramBackup)
    *(.partition52BssSramBackup.*)

    /* address of the partition 52 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition52BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition52UssSramBackupStart = .;

    *(.partition52UssSramBackup)
    *(.partition52UssSramBackup.*)

    pharosPartition52UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_52_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 52 sections end in SramBackup */
    pharosPartition52SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition52DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition52SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_52_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition52DataSdramStart = ABSOLUTE(.);

    *(.partition52DataSdram)
    *(.partition52DataSdram.*)

    /* address of the partition 52 data section end in RAM */
    . = ALIGN(8);
    pharosPartition52DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 52 in Sdram */
  .pharosPartition52BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 52 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition52BssSdramStart = .;

    *(.partition52BssSdram)
    *(.partition52BssSdram.*)

    /* address of the partition 52 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition52BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition52UssSdramStart = .;

    *(.partition52UssSdram)
    *(.partition52UssSdram.*)

    pharosPartition52UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_52_SDRAM_ALIGNMENT);

    /* address of the partition 52 sections end in Sdram */
    pharosPartition52SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 53                                                   */
  /******************************************************************/

  /* address in the flash of the partition 53 data to copy it to RAM */
  pharosPartition53DataCcmLoadAddr = LOADADDR(.partition53DataCcm);

  /* address in the flash of the partition 53 data to copy it to RAM */
  pharosPartition53DataSram1LoadAddr = LOADADDR(.partition53DataSram1);

  /* address in the flash of the partition 53 data to copy it to RAM */
  pharosPartition53DataSram2LoadAddr = LOADADDR(.partition53DataSram2);

  /* address in the flash of the partition 53 data to copy it to RAM */
  pharosPartition53DataSram3LoadAddr = LOADADDR(.partition53DataSram3);

  /* address in the flash of the partition 53 data to copy it to RAM */
  pharosPartition53DataSramBackupLoadAddr = LOADADDR(.partition53DataSramBackup);

  /* address in the flash of the partition 53 data to copy it to RAM */
  pharosPartition53DataSdramLoadAddr = LOADADDR(.partition53DataSdram);

  /* data of partition in Ccm */
  .partition53DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition53CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_53_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition53DataCcmStart = ABSOLUTE(.);

    *(.partition53DataCcm)
    *(.partition53DataCcm.*)

    /* address of the partition 53 data section end in RAM */
    . = ALIGN(8);
    pharosPartition53DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 53 in Ccm */
  .pharosPartition53BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 53 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition53BssCcmStart = .;

    *(.partition53BssCcm)
    *(.partition53BssCcm.*)

    /* address of the partition 53 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition53BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition53UssCcmStart = .;

    *(.partition53UssCcm)
    *(.partition53UssCcm.*)

    pharosPartition53UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_53_CCM_ALIGNMENT);

    /* address of the partition 53 sections end in Ccm */
    pharosPartition53CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition53DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition53Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_53_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition53DataSram1Start = ABSOLUTE(.);

    *(.partition53DataSram1)
    *(.partition53DataSram1.*)

    /* address of the partition 53 data section end in RAM */
    . = ALIGN(8);
    pharosPartition53DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 53 in Sram1 */
  .pharosPartition53BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 53 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition53BssSram1Start = .;

    *(.partition53BssSram1)
    *(.partition53BssSram1.*)

    /* address of the partition 53 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition53BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition53UssSram1Start = .;

    *(.partition53UssSram1)
    *(.partition53UssSram1.*)

    pharosPartition53UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_53_SRAM1_ALIGNMENT);

    /* address of the partition 53 sections end in Sram1 */
    pharosPartition53Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition53DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition53Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_53_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition53DataSram2Start = ABSOLUTE(.);

    *(.partition53DataSram2)
    *(.partition53DataSram2.*)

    /* address of the partition 53 data section end in RAM */
    . = ALIGN(8);
    pharosPartition53DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 53 in Sram2 */
  .pharosPartition53BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 53 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition53BssSram2Start = .;

    *(.partition53BssSram2)
    *(.partition53BssSram2.*)

    /* address of the partition 53 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition53BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition53UssSram2Start = .;

    *(.partition53UssSram2)
    *(.partition53UssSram2.*)

    pharosPartition53UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_53_SRAM2_ALIGNMENT);

    /* address of the partition 53 sections end in Sram2 */
    pharosPartition53Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition53DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition53Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_53_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition53DataSram3Start = ABSOLUTE(.);

    *(.partition53DataSram3)
    *(.partition53DataSram3.*)

    /* address of the partition 53 data section end in RAM */
    . = ALIGN(8);
    pharosPartition53DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 53 in Sram3 */
  .pharosPartition53BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 53 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition53BssSram3Start = .;

    *(.partition53BssSram3)
    *(.partition53BssSram3.*)

    /* address of the partition 53 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition53BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition53UssSram3Start = .;

    *(.partition53UssSram3)
    *(.partition53UssSram3.*)

    pharosPartition53UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_53_SRAM3_ALIGNMENT);

    /* address of the partition 53 sections end in Sram3 */
    pharosPartition53Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition53DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition53SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_53_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition53DataSramBackupStart = ABSOLUTE(.);

    *(.partition53DataSramBackup)
    *(.partition53DataSramBackup.*)

    /* address of the partition 53 data section end in RAM */
    . = ALIGN(8);
    pharosPartition53DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 53 in SramBackup */
  .pharosPartition53BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 53 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition53BssSramBackupStart = .;

    *(.partition53BssSramBackup)
    *(.partition53BssSramBackup.*)

    /* address of the partition 53 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition53BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition53UssSramBackupStart = .;

    *(.partition53UssSramBackup)
    *(.partition53UssSramBackup.*)

    pharosPartition53UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_53_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 53 sections end in SramBackup */
    pharosPartition53SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition53DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition53SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_53_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition53DataSdramStart = ABSOLUTE(.);

    *(.partition53DataSdram)
    *(.partition53DataSdram.*)

    /* address of the partition 53 data section end in RAM */
    . = ALIGN(8);
    pharosPartition53DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 53 in Sdram */
  .pharosPartition53BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 53 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition53BssSdramStart = .;

    *(.partition53BssSdram)
    *(.partition53BssSdram.*)

    /* address of the partition 53 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition53BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition53UssSdramStart = .;

    *(.partition53UssSdram)
    *(.partition53UssSdram.*)

    pharosPartition53UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_53_SDRAM_ALIGNMENT);

    /* address of the partition 53 sections end in Sdram */
    pharosPartition53SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 54                                                   */
  /******************************************************************/

  /* address in the flash of the partition 54 data to copy it to RAM */
  pharosPartition54DataCcmLoadAddr = LOADADDR(.partition54DataCcm);

  /* address in the flash of the partition 54 data to copy it to RAM */
  pharosPartition54DataSram1LoadAddr = LOADADDR(.partition54DataSram1);

  /* address in the flash of the partition 54 data to copy it to RAM */
  pharosPartition54DataSram2LoadAddr = LOADADDR(.partition54DataSram2);

  /* address in the flash of the partition 54 data to copy it to RAM */
  pharosPartition54DataSram3LoadAddr = LOADADDR(.partition54DataSram3);

  /* address in the flash of the partition 54 data to copy it to RAM */
  pharosPartition54DataSramBackupLoadAddr = LOADADDR(.partition54DataSramBackup);

  /* address in the flash of the partition 54 data to copy it to RAM */
  pharosPartition54DataSdramLoadAddr = LOADADDR(.partition54DataSdram);

  /* data of partition in Ccm */
  .partition54DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition54CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_54_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition54DataCcmStart = ABSOLUTE(.);

    *(.partition54DataCcm)
    *(.partition54DataCcm.*)

    /* address of the partition 54 data section end in RAM */
    . = ALIGN(8);
    pharosPartition54DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 54 in Ccm */
  .pharosPartition54BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 54 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition54BssCcmStart = .;

    *(.partition54BssCcm)
    *(.partition54BssCcm.*)

    /* address of the partition 54 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition54BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition54UssCcmStart = .;

    *(.partition54UssCcm)
    *(.partition54UssCcm.*)

    pharosPartition54UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_54_CCM_ALIGNMENT);

    /* address of the partition 54 sections end in Ccm */
    pharosPartition54CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition54DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition54Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_54_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition54DataSram1Start = ABSOLUTE(.);

    *(.partition54DataSram1)
    *(.partition54DataSram1.*)

    /* address of the partition 54 data section end in RAM */
    . = ALIGN(8);
    pharosPartition54DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 54 in Sram1 */
  .pharosPartition54BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 54 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition54BssSram1Start = .;

    *(.partition54BssSram1)
    *(.partition54BssSram1.*)

    /* address of the partition 54 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition54BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition54UssSram1Start = .;

    *(.partition54UssSram1)
    *(.partition54UssSram1.*)

    pharosPartition54UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_54_SRAM1_ALIGNMENT);

    /* address of the partition 54 sections end in Sram1 */
    pharosPartition54Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition54DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition54Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_54_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition54DataSram2Start = ABSOLUTE(.);

    *(.partition54DataSram2)
    *(.partition54DataSram2.*)

    /* address of the partition 54 data section end in RAM */
    . = ALIGN(8);
    pharosPartition54DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 54 in Sram2 */
  .pharosPartition54BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 54 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition54BssSram2Start = .;

    *(.partition54BssSram2)
    *(.partition54BssSram2.*)

    /* address of the partition 54 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition54BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition54UssSram2Start = .;

    *(.partition54UssSram2)
    *(.partition54UssSram2.*)

    pharosPartition54UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_54_SRAM2_ALIGNMENT);

    /* address of the partition 54 sections end in Sram2 */
    pharosPartition54Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition54DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition54Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_54_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition54DataSram3Start = ABSOLUTE(.);

    *(.partition54DataSram3)
    *(.partition54DataSram3.*)

    /* address of the partition 54 data section end in RAM */
    . = ALIGN(8);
    pharosPartition54DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 54 in Sram3 */
  .pharosPartition54BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 54 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition54BssSram3Start = .;

    *(.partition54BssSram3)
    *(.partition54BssSram3.*)

    /* address of the partition 54 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition54BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition54UssSram3Start = .;

    *(.partition54UssSram3)
    *(.partition54UssSram3.*)

    pharosPartition54UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_54_SRAM3_ALIGNMENT);

    /* address of the partition 54 sections end in Sram3 */
    pharosPartition54Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition54DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition54SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_54_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition54DataSramBackupStart = ABSOLUTE(.);

    *(.partition54DataSramBackup)
    *(.partition54DataSramBackup.*)

    /* address of the partition 54 data section end in RAM */
    . = ALIGN(8);
    pharosPartition54DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 54 in SramBackup */
  .pharosPartition54BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 54 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition54BssSramBackupStart = .;

    *(.partition54BssSramBackup)
    *(.partition54BssSramBackup.*)

    /* address of the partition 54 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition54BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition54UssSramBackupStart = .;

    *(.partition54UssSramBackup)
    *(.partition54UssSramBackup.*)

    pharosPartition54UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_54_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 54 sections end in SramBackup */
    pharosPartition54SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition54DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition54SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_54_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition54DataSdramStart = ABSOLUTE(.);

    *(.partition54DataSdram)
    *(.partition54DataSdram.*)

    /* address of the partition 54 data section end in RAM */
    . = ALIGN(8);
    pharosPartition54DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 54 in Sdram */
  .pharosPartition54BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 54 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition54BssSdramStart = .;

    *(.partition54BssSdram)
    *(.partition54BssSdram.*)

    /* address of the partition 54 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition54BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition54UssSdramStart = .;

    *(.partition54UssSdram)
    *(.partition54UssSdram.*)

    pharosPartition54UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_54_SDRAM_ALIGNMENT);

    /* address of the partition 54 sections end in Sdram */
    pharosPartition54SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 55                                                   */
  /******************************************************************/

  /* address in the flash of the partition 55 data to copy it to RAM */
  pharosPartition55DataCcmLoadAddr = LOADADDR(.partition55DataCcm);

  /* address in the flash of the partition 55 data to copy it to RAM */
  pharosPartition55DataSram1LoadAddr = LOADADDR(.partition55DataSram1);

  /* address in the flash of the partition 55 data to copy it to RAM */
  pharosPartition55DataSram2LoadAddr = LOADADDR(.partition55DataSram2);

  /* address in the flash of the partition 55 data to copy it to RAM */
  pharosPartition55DataSram3LoadAddr = LOADADDR(.partition55DataSram3);

  /* address in the flash of the partition 55 data to copy it to RAM */
  pharosPartition55DataSramBackupLoadAddr = LOADADDR(.partition55DataSramBackup);

  /* address in the flash of the partition 55 data to copy it to RAM */
  pharosPartition55DataSdramLoadAddr = LOADADDR(.partition55DataSdram);

  /* data of partition in Ccm */
  .partition55DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition55CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_55_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition55DataCcmStart = ABSOLUTE(.);

    *(.partition55DataCcm)
    *(.partition55DataCcm.*)

    /* address of the partition 55 data section end in RAM */
    . = ALIGN(8);
    pharosPartition55DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 55 in Ccm */
  .pharosPartition55BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 55 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition55BssCcmStart = .;

    *(.partition55BssCcm)
    *(.partition55BssCcm.*)

    /* address of the partition 55 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition55BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition55UssCcmStart = .;

    *(.partition55UssCcm)
    *(.partition55UssCcm.*)

    pharosPartition55UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_55_CCM_ALIGNMENT);

    /* address of the partition 55 sections end in Ccm */
    pharosPartition55CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition55DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition55Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_55_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition55DataSram1Start = ABSOLUTE(.);

    *(.partition55DataSram1)
    *(.partition55DataSram1.*)

    /* address of the partition 55 data section end in RAM */
    . = ALIGN(8);
    pharosPartition55DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 55 in Sram1 */
  .pharosPartition55BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 55 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition55BssSram1Start = .;

    *(.partition55BssSram1)
    *(.partition55BssSram1.*)

    /* address of the partition 55 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition55BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition55UssSram1Start = .;

    *(.partition55UssSram1)
    *(.partition55UssSram1.*)

    pharosPartition55UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_55_SRAM1_ALIGNMENT);

    /* address of the partition 55 sections end in Sram1 */
    pharosPartition55Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition55DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition55Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_55_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition55DataSram2Start = ABSOLUTE(.);

    *(.partition55DataSram2)
    *(.partition55DataSram2.*)

    /* address of the partition 55 data section end in RAM */
    . = ALIGN(8);
    pharosPartition55DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 55 in Sram2 */
  .pharosPartition55BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 55 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition55BssSram2Start = .;

    *(.partition55BssSram2)
    *(.partition55BssSram2.*)

    /* address of the partition 55 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition55BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition55UssSram2Start = .;

    *(.partition55UssSram2)
    *(.partition55UssSram2.*)

    pharosPartition55UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_55_SRAM2_ALIGNMENT);

    /* address of the partition 55 sections end in Sram2 */
    pharosPartition55Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition55DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition55Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_55_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition55DataSram3Start = ABSOLUTE(.);

    *(.partition55DataSram3)
    *(.partition55DataSram3.*)

    /* address of the partition 55 data section end in RAM */
    . = ALIGN(8);
    pharosPartition55DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 55 in Sram3 */
  .pharosPartition55BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 55 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition55BssSram3Start = .;

    *(.partition55BssSram3)
    *(.partition55BssSram3.*)

    /* address of the partition 55 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition55BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition55UssSram3Start = .;

    *(.partition55UssSram3)
    *(.partition55UssSram3.*)

    pharosPartition55UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_55_SRAM3_ALIGNMENT);

    /* address of the partition 55 sections end in Sram3 */
    pharosPartition55Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition55DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition55SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_55_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition55DataSramBackupStart = ABSOLUTE(.);

    *(.partition55DataSramBackup)
    *(.partition55DataSramBackup.*)

    /* address of the partition 55 data section end in RAM */
    . = ALIGN(8);
    pharosPartition55DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 55 in SramBackup */
  .pharosPartition55BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 55 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition55BssSramBackupStart = .;

    *(.partition55BssSramBackup)
    *(.partition55BssSramBackup.*)

    /* address of the partition 55 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition55BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition55UssSramBackupStart = .;

    *(.partition55UssSramBackup)
    *(.partition55UssSramBackup.*)

    pharosPartition55UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_55_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 55 sections end in SramBackup */
    pharosPartition55SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition55DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition55SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_55_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition55DataSdramStart = ABSOLUTE(.);

    *(.partition55DataSdram)
    *(.partition55DataSdram.*)

    /* address of the partition 55 data section end in RAM */
    . = ALIGN(8);
    pharosPartition55DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 55 in Sdram */
  .pharosPartition55BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 55 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition55BssSdramStart = .;

    *(.partition55BssSdram)
    *(.partition55BssSdram.*)

    /* address of the partition 55 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition55BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition55UssSdramStart = .;

    *(.partition55UssSdram)
    *(.partition55UssSdram.*)

    pharosPartition55UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_55_SDRAM_ALIGNMENT);

    /* address of the partition 55 sections end in Sdram */
    pharosPartition55SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 56                                                   */
  /******************************************************************/

  /* address in the flash of the partition 56 data to copy it to RAM */
  pharosPartition56DataCcmLoadAddr = LOADADDR(.partition56DataCcm);

  /* address in the flash of the partition 56 data to copy it to RAM */
  pharosPartition56DataSram1LoadAddr = LOADADDR(.partition56DataSram1);

  /* address in the flash of the partition 56 data to copy it to RAM */
  pharosPartition56DataSram2LoadAddr = LOADADDR(.partition56DataSram2);

  /* address in the flash of the partition 56 data to copy it to RAM */
  pharosPartition56DataSram3LoadAddr = LOADADDR(.partition56DataSram3);

  /* address in the flash of the partition 56 data to copy it to RAM */
  pharosPartition56DataSramBackupLoadAddr = LOADADDR(.partition56DataSramBackup);

  /* address in the flash of the partition 56 data to copy it to RAM */
  pharosPartition56DataSdramLoadAddr = LOADADDR(.partition56DataSdram);

  /* data of partition in Ccm */
  .partition56DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition56CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_56_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition56DataCcmStart = ABSOLUTE(.);

    *(.partition56DataCcm)
    *(.partition56DataCcm.*)

    /* address of the partition 56 data section end in RAM */
    . = ALIGN(8);
    pharosPartition56DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 56 in Ccm */
  .pharosPartition56BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 56 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition56BssCcmStart = .;

    *(.partition56BssCcm)
    *(.partition56BssCcm.*)

    /* address of the partition 56 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition56BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition56UssCcmStart = .;

    *(.partition56UssCcm)
    *(.partition56UssCcm.*)

    pharosPartition56UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_56_CCM_ALIGNMENT);

    /* address of the partition 56 sections end in Ccm */
    pharosPartition56CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition56DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition56Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_56_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition56DataSram1Start = ABSOLUTE(.);

    *(.partition56DataSram1)
    *(.partition56DataSram1.*)

    /* address of the partition 56 data section end in RAM */
    . = ALIGN(8);
    pharosPartition56DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 56 in Sram1 */
  .pharosPartition56BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 56 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition56BssSram1Start = .;

    *(.partition56BssSram1)
    *(.partition56BssSram1.*)

    /* address of the partition 56 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition56BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition56UssSram1Start = .;

    *(.partition56UssSram1)
    *(.partition56UssSram1.*)

    pharosPartition56UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_56_SRAM1_ALIGNMENT);

    /* address of the partition 56 sections end in Sram1 */
    pharosPartition56Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition56DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition56Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_56_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition56DataSram2Start = ABSOLUTE(.);

    *(.partition56DataSram2)
    *(.partition56DataSram2.*)

    /* address of the partition 56 data section end in RAM */
    . = ALIGN(8);
    pharosPartition56DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 56 in Sram2 */
  .pharosPartition56BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 56 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition56BssSram2Start = .;

    *(.partition56BssSram2)
    *(.partition56BssSram2.*)

    /* address of the partition 56 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition56BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition56UssSram2Start = .;

    *(.partition56UssSram2)
    *(.partition56UssSram2.*)

    pharosPartition56UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_56_SRAM2_ALIGNMENT);

    /* address of the partition 56 sections end in Sram2 */
    pharosPartition56Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition56DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition56Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_56_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition56DataSram3Start = ABSOLUTE(.);

    *(.partition56DataSram3)
    *(.partition56DataSram3.*)

    /* address of the partition 56 data section end in RAM */
    . = ALIGN(8);
    pharosPartition56DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 56 in Sram3 */
  .pharosPartition56BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 56 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition56BssSram3Start = .;

    *(.partition56BssSram3)
    *(.partition56BssSram3.*)

    /* address of the partition 56 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition56BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition56UssSram3Start = .;

    *(.partition56UssSram3)
    *(.partition56UssSram3.*)

    pharosPartition56UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_56_SRAM3_ALIGNMENT);

    /* address of the partition 56 sections end in Sram3 */
    pharosPartition56Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition56DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition56SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_56_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition56DataSramBackupStart = ABSOLUTE(.);

    *(.partition56DataSramBackup)
    *(.partition56DataSramBackup.*)

    /* address of the partition 56 data section end in RAM */
    . = ALIGN(8);
    pharosPartition56DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 56 in SramBackup */
  .pharosPartition56BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 56 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition56BssSramBackupStart = .;

    *(.partition56BssSramBackup)
    *(.partition56BssSramBackup.*)

    /* address of the partition 56 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition56BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition56UssSramBackupStart = .;

    *(.partition56UssSramBackup)
    *(.partition56UssSramBackup.*)

    pharosPartition56UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_56_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 56 sections end in SramBackup */
    pharosPartition56SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition56DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition56SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_56_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition56DataSdramStart = ABSOLUTE(.);

    *(.partition56DataSdram)
    *(.partition56DataSdram.*)

    /* address of the partition 56 data section end in RAM */
    . = ALIGN(8);
    pharosPartition56DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 56 in Sdram */
  .pharosPartition56BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 56 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition56BssSdramStart = .;

    *(.partition56BssSdram)
    *(.partition56BssSdram.*)

    /* address of the partition 56 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition56BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition56UssSdramStart = .;

    *(.partition56UssSdram)
    *(.partition56UssSdram.*)

    pharosPartition56UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_56_SDRAM_ALIGNMENT);

    /* address of the partition 56 sections end in Sdram */
    pharosPartition56SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 57                                                   */
  /******************************************************************/

  /* address in the flash of the partition 57 data to copy it to RAM */
  pharosPartition57DataCcmLoadAddr = LOADADDR(.partition57DataCcm);

  /* address in the flash of the partition 57 data to copy it to RAM */
  pharosPartition57DataSram1LoadAddr = LOADADDR(.partition57DataSram1);

  /* address in the flash of the partition 57 data to copy it to RAM */
  pharosPartition57DataSram2LoadAddr = LOADADDR(.partition57DataSram2);

  /* address in the flash of the partition 57 data to copy it to RAM */
  pharosPartition57DataSram3LoadAddr = LOADADDR(.partition57DataSram3);

  /* address in the flash of the partition 57 data to copy it to RAM */
  pharosPartition57DataSramBackupLoadAddr = LOADADDR(.partition57DataSramBackup);

  /* address in the flash of the partition 57 data to copy it to RAM */
  pharosPartition57DataSdramLoadAddr = LOADADDR(.partition57DataSdram);

  /* data of partition in Ccm */
  .partition57DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition57CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_57_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition57DataCcmStart = ABSOLUTE(.);

    *(.partition57DataCcm)
    *(.partition57DataCcm.*)

    /* address of the partition 57 data section end in RAM */
    . = ALIGN(8);
    pharosPartition57DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 57 in Ccm */
  .pharosPartition57BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 57 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition57BssCcmStart = .;

    *(.partition57BssCcm)
    *(.partition57BssCcm.*)

    /* address of the partition 57 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition57BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition57UssCcmStart = .;

    *(.partition57UssCcm)
    *(.partition57UssCcm.*)

    pharosPartition57UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_57_CCM_ALIGNMENT);

    /* address of the partition 57 sections end in Ccm */
    pharosPartition57CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition57DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition57Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_57_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition57DataSram1Start = ABSOLUTE(.);

    *(.partition57DataSram1)
    *(.partition57DataSram1.*)

    /* address of the partition 57 data section end in RAM */
    . = ALIGN(8);
    pharosPartition57DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 57 in Sram1 */
  .pharosPartition57BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 57 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition57BssSram1Start = .;

    *(.partition57BssSram1)
    *(.partition57BssSram1.*)

    /* address of the partition 57 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition57BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition57UssSram1Start = .;

    *(.partition57UssSram1)
    *(.partition57UssSram1.*)

    pharosPartition57UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_57_SRAM1_ALIGNMENT);

    /* address of the partition 57 sections end in Sram1 */
    pharosPartition57Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition57DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition57Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_57_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition57DataSram2Start = ABSOLUTE(.);

    *(.partition57DataSram2)
    *(.partition57DataSram2.*)

    /* address of the partition 57 data section end in RAM */
    . = ALIGN(8);
    pharosPartition57DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 57 in Sram2 */
  .pharosPartition57BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 57 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition57BssSram2Start = .;

    *(.partition57BssSram2)
    *(.partition57BssSram2.*)

    /* address of the partition 57 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition57BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition57UssSram2Start = .;

    *(.partition57UssSram2)
    *(.partition57UssSram2.*)

    pharosPartition57UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_57_SRAM2_ALIGNMENT);

    /* address of the partition 57 sections end in Sram2 */
    pharosPartition57Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition57DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition57Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_57_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition57DataSram3Start = ABSOLUTE(.);

    *(.partition57DataSram3)
    *(.partition57DataSram3.*)

    /* address of the partition 57 data section end in RAM */
    . = ALIGN(8);
    pharosPartition57DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 57 in Sram3 */
  .pharosPartition57BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 57 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition57BssSram3Start = .;

    *(.partition57BssSram3)
    *(.partition57BssSram3.*)

    /* address of the partition 57 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition57BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition57UssSram3Start = .;

    *(.partition57UssSram3)
    *(.partition57UssSram3.*)

    pharosPartition57UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_57_SRAM3_ALIGNMENT);

    /* address of the partition 57 sections end in Sram3 */
    pharosPartition57Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition57DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition57SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_57_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition57DataSramBackupStart = ABSOLUTE(.);

    *(.partition57DataSramBackup)
    *(.partition57DataSramBackup.*)

    /* address of the partition 57 data section end in RAM */
    . = ALIGN(8);
    pharosPartition57DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 57 in SramBackup */
  .pharosPartition57BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 57 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition57BssSramBackupStart = .;

    *(.partition57BssSramBackup)
    *(.partition57BssSramBackup.*)

    /* address of the partition 57 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition57BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition57UssSramBackupStart = .;

    *(.partition57UssSramBackup)
    *(.partition57UssSramBackup.*)

    pharosPartition57UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_57_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 57 sections end in SramBackup */
    pharosPartition57SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition57DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition57SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_57_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition57DataSdramStart = ABSOLUTE(.);

    *(.partition57DataSdram)
    *(.partition57DataSdram.*)

    /* address of the partition 57 data section end in RAM */
    . = ALIGN(8);
    pharosPartition57DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 57 in Sdram */
  .pharosPartition57BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 57 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition57BssSdramStart = .;

    *(.partition57BssSdram)
    *(.partition57BssSdram.*)

    /* address of the partition 57 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition57BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition57UssSdramStart = .;

    *(.partition57UssSdram)
    *(.partition57UssSdram.*)

    pharosPartition57UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_57_SDRAM_ALIGNMENT);

    /* address of the partition 57 sections end in Sdram */
    pharosPartition57SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 58                                                   */
  /******************************************************************/

  /* address in the flash of the partition 58 data to copy it to RAM */
  pharosPartition58DataCcmLoadAddr = LOADADDR(.partition58DataCcm);

  /* address in the flash of the partition 58 data to copy it to RAM */
  pharosPartition58DataSram1LoadAddr = LOADADDR(.partition58DataSram1);

  /* address in the flash of the partition 58 data to copy it to RAM */
  pharosPartition58DataSram2LoadAddr = LOADADDR(.partition58DataSram2);

  /* address in the flash of the partition 58 data to copy it to RAM */
  pharosPartition58DataSram3LoadAddr = LOADADDR(.partition58DataSram3);

  /* address in the flash of the partition 58 data to copy it to RAM */
  pharosPartition58DataSramBackupLoadAddr = LOADADDR(.partition58DataSramBackup);

  /* address in the flash of the partition 58 data to copy it to RAM */
  pharosPartition58DataSdramLoadAddr = LOADADDR(.partition58DataSdram);

  /* data of partition in Ccm */
  .partition58DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition58CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_58_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition58DataCcmStart = ABSOLUTE(.);

    *(.partition58DataCcm)
    *(.partition58DataCcm.*)

    /* address of the partition 58 data section end in RAM */
    . = ALIGN(8);
    pharosPartition58DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 58 in Ccm */
  .pharosPartition58BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 58 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition58BssCcmStart = .;

    *(.partition58BssCcm)
    *(.partition58BssCcm.*)

    /* address of the partition 58 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition58BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition58UssCcmStart = .;

    *(.partition58UssCcm)
    *(.partition58UssCcm.*)

    pharosPartition58UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_58_CCM_ALIGNMENT);

    /* address of the partition 58 sections end in Ccm */
    pharosPartition58CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition58DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition58Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_58_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition58DataSram1Start = ABSOLUTE(.);

    *(.partition58DataSram1)
    *(.partition58DataSram1.*)

    /* address of the partition 58 data section end in RAM */
    . = ALIGN(8);
    pharosPartition58DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 58 in Sram1 */
  .pharosPartition58BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 58 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition58BssSram1Start = .;

    *(.partition58BssSram1)
    *(.partition58BssSram1.*)

    /* address of the partition 58 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition58BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition58UssSram1Start = .;

    *(.partition58UssSram1)
    *(.partition58UssSram1.*)

    pharosPartition58UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_58_SRAM1_ALIGNMENT);

    /* address of the partition 58 sections end in Sram1 */
    pharosPartition58Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition58DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition58Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_58_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition58DataSram2Start = ABSOLUTE(.);

    *(.partition58DataSram2)
    *(.partition58DataSram2.*)

    /* address of the partition 58 data section end in RAM */
    . = ALIGN(8);
    pharosPartition58DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 58 in Sram2 */
  .pharosPartition58BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 58 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition58BssSram2Start = .;

    *(.partition58BssSram2)
    *(.partition58BssSram2.*)

    /* address of the partition 58 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition58BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition58UssSram2Start = .;

    *(.partition58UssSram2)
    *(.partition58UssSram2.*)

    pharosPartition58UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_58_SRAM2_ALIGNMENT);

    /* address of the partition 58 sections end in Sram2 */
    pharosPartition58Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition58DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition58Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_58_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition58DataSram3Start = ABSOLUTE(.);

    *(.partition58DataSram3)
    *(.partition58DataSram3.*)

    /* address of the partition 58 data section end in RAM */
    . = ALIGN(8);
    pharosPartition58DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 58 in Sram3 */
  .pharosPartition58BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 58 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition58BssSram3Start = .;

    *(.partition58BssSram3)
    *(.partition58BssSram3.*)

    /* address of the partition 58 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition58BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition58UssSram3Start = .;

    *(.partition58UssSram3)
    *(.partition58UssSram3.*)

    pharosPartition58UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_58_SRAM3_ALIGNMENT);

    /* address of the partition 58 sections end in Sram3 */
    pharosPartition58Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition58DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition58SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_58_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition58DataSramBackupStart = ABSOLUTE(.);

    *(.partition58DataSramBackup)
    *(.partition58DataSramBackup.*)

    /* address of the partition 58 data section end in RAM */
    . = ALIGN(8);
    pharosPartition58DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 58 in SramBackup */
  .pharosPartition58BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 58 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition58BssSramBackupStart = .;

    *(.partition58BssSramBackup)
    *(.partition58BssSramBackup.*)

    /* address of the partition 58 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition58BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition58UssSramBackupStart = .;

    *(.partition58UssSramBackup)
    *(.partition58UssSramBackup.*)

    pharosPartition58UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_58_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 58 sections end in SramBackup */
    pharosPartition58SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition58DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition58SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_58_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition58DataSdramStart = ABSOLUTE(.);

    *(.partition58DataSdram)
    *(.partition58DataSdram.*)

    /* address of the partition 58 data section end in RAM */
    . = ALIGN(8);
    pharosPartition58DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 58 in Sdram */
  .pharosPartition58BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 58 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition58BssSdramStart = .;

    *(.partition58BssSdram)
    *(.partition58BssSdram.*)

    /* address of the partition 58 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition58BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition58UssSdramStart = .;

    *(.partition58UssSdram)
    *(.partition58UssSdram.*)

    pharosPartition58UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_58_SDRAM_ALIGNMENT);

    /* address of the partition 58 sections end in Sdram */
    pharosPartition58SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 59                                                   */
  /******************************************************************/

  /* address in the flash of the partition 59 data to copy it to RAM */
  pharosPartition59DataCcmLoadAddr = LOADADDR(.partition59DataCcm);

  /* address in the flash of the partition 59 data to copy it to RAM */
  pharosPartition59DataSram1LoadAddr = LOADADDR(.partition59DataSram1);

  /* address in the flash of the partition 59 data to copy it to RAM */
  pharosPartition59DataSram2LoadAddr = LOADADDR(.partition59DataSram2);

  /* address in the flash of the partition 59 data to copy it to RAM */
  pharosPartition59DataSram3LoadAddr = LOADADDR(.partition59DataSram3);

  /* address in the flash of the partition 59 data to copy it to RAM */
  pharosPartition59DataSramBackupLoadAddr = LOADADDR(.partition59DataSramBackup);

  /* address in the flash of the partition 59 data to copy it to RAM */
  pharosPartition59DataSdramLoadAddr = LOADADDR(.partition59DataSdram);

  /* data of partition in Ccm */
  .partition59DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition59CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_59_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition59DataCcmStart = ABSOLUTE(.);

    *(.partition59DataCcm)
    *(.partition59DataCcm.*)

    /* address of the partition 59 data section end in RAM */
    . = ALIGN(8);
    pharosPartition59DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 59 in Ccm */
  .pharosPartition59BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 59 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition59BssCcmStart = .;

    *(.partition59BssCcm)
    *(.partition59BssCcm.*)

    /* address of the partition 59 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition59BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition59UssCcmStart = .;

    *(.partition59UssCcm)
    *(.partition59UssCcm.*)

    pharosPartition59UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_59_CCM_ALIGNMENT);

    /* address of the partition 59 sections end in Ccm */
    pharosPartition59CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition59DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition59Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_59_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition59DataSram1Start = ABSOLUTE(.);

    *(.partition59DataSram1)
    *(.partition59DataSram1.*)

    /* address of the partition 59 data section end in RAM */
    . = ALIGN(8);
    pharosPartition59DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 59 in Sram1 */
  .pharosPartition59BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 59 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition59BssSram1Start = .;

    *(.partition59BssSram1)
    *(.partition59BssSram1.*)

    /* address of the partition 59 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition59BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition59UssSram1Start = .;

    *(.partition59UssSram1)
    *(.partition59UssSram1.*)

    pharosPartition59UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_59_SRAM1_ALIGNMENT);

    /* address of the partition 59 sections end in Sram1 */
    pharosPartition59Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition59DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition59Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_59_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition59DataSram2Start = ABSOLUTE(.);

    *(.partition59DataSram2)
    *(.partition59DataSram2.*)

    /* address of the partition 59 data section end in RAM */
    . = ALIGN(8);
    pharosPartition59DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 59 in Sram2 */
  .pharosPartition59BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 59 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition59BssSram2Start = .;

    *(.partition59BssSram2)
    *(.partition59BssSram2.*)

    /* address of the partition 59 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition59BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition59UssSram2Start = .;

    *(.partition59UssSram2)
    *(.partition59UssSram2.*)

    pharosPartition59UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_59_SRAM2_ALIGNMENT);

    /* address of the partition 59 sections end in Sram2 */
    pharosPartition59Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition59DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition59Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_59_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition59DataSram3Start = ABSOLUTE(.);

    *(.partition59DataSram3)
    *(.partition59DataSram3.*)

    /* address of the partition 59 data section end in RAM */
    . = ALIGN(8);
    pharosPartition59DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 59 in Sram3 */
  .pharosPartition59BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 59 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition59BssSram3Start = .;

    *(.partition59BssSram3)
    *(.partition59BssSram3.*)

    /* address of the partition 59 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition59BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition59UssSram3Start = .;

    *(.partition59UssSram3)
    *(.partition59UssSram3.*)

    pharosPartition59UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_59_SRAM3_ALIGNMENT);

    /* address of the partition 59 sections end in Sram3 */
    pharosPartition59Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition59DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition59SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_59_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition59DataSramBackupStart = ABSOLUTE(.);

    *(.partition59DataSramBackup)
    *(.partition59DataSramBackup.*)

    /* address of the partition 59 data section end in RAM */
    . = ALIGN(8);
    pharosPartition59DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 59 in SramBackup */
  .pharosPartition59BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 59 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition59BssSramBackupStart = .;

    *(.partition59BssSramBackup)
    *(.partition59BssSramBackup.*)

    /* address of the partition 59 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition59BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition59UssSramBackupStart = .;

    *(.partition59UssSramBackup)
    *(.partition59UssSramBackup.*)

    pharosPartition59UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_59_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 59 sections end in SramBackup */
    pharosPartition59SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition59DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition59SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_59_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition59DataSdramStart = ABSOLUTE(.);

    *(.partition59DataSdram)
    *(.partition59DataSdram.*)

    /* address of the partition 59 data section end in RAM */
    . = ALIGN(8);
    pharosPartition59DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 59 in Sdram */
  .pharosPartition59BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 59 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition59BssSdramStart = .;

    *(.partition59BssSdram)
    *(.partition59BssSdram.*)

    /* address of the partition 59 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition59BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition59UssSdramStart = .;

    *(.partition59UssSdram)
    *(.partition59UssSdram.*)

    pharosPartition59UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_59_SDRAM_ALIGNMENT);

    /* address of the partition 59 sections end in Sdram */
    pharosPartition59SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 60                                                   */
  /******************************************************************/

  /* address in the flash of the partition 60 data to copy it to RAM */
  pharosPartition60DataCcmLoadAddr = LOADADDR(.partition60DataCcm);

  /* address in the flash of the partition 60 data to copy it to RAM */
  pharosPartition60DataSram1LoadAddr = LOADADDR(.partition60DataSram1);

  /* address in the flash of the partition 60 data to copy it to RAM */
  pharosPartition60DataSram2LoadAddr = LOADADDR(.partition60DataSram2);

  /* address in the flash of the partition 60 data to copy it to RAM */
  pharosPartition60DataSram3LoadAddr = LOADADDR(.partition60DataSram3);

  /* address in the flash of the partition 60 data to copy it to RAM */
  pharosPartition60DataSramBackupLoadAddr = LOADADDR(.partition60DataSramBackup);

  /* address in the flash of the partition 60 data to copy it to RAM */
  pharosPartition60DataSdramLoadAddr = LOADADDR(.partition60DataSdram);

  /* data of partition in Ccm */
  .partition60DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition60CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_60_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition60DataCcmStart = ABSOLUTE(.);

    *(.partition60DataCcm)
    *(.partition60DataCcm.*)

    /* address of the partition 60 data section end in RAM */
    . = ALIGN(8);
    pharosPartition60DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 60 in Ccm */
  .pharosPartition60BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 60 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition60BssCcmStart = .;

    *(.partition60BssCcm)
    *(.partition60BssCcm.*)

    /* address of the partition 60 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition60BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition60UssCcmStart = .;

    *(.partition60UssCcm)
    *(.partition60UssCcm.*)

    pharosPartition60UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_60_CCM_ALIGNMENT);

    /* address of the partition 60 sections end in Ccm */
    pharosPartition60CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition60DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition60Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_60_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition60DataSram1Start = ABSOLUTE(.);

    *(.partition60DataSram1)
    *(.partition60DataSram1.*)

    /* address of the partition 60 data section end in RAM */
    . = ALIGN(8);
    pharosPartition60DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 60 in Sram1 */
  .pharosPartition60BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 60 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition60BssSram1Start = .;

    *(.partition60BssSram1)
    *(.partition60BssSram1.*)

    /* address of the partition 60 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition60BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition60UssSram1Start = .;

    *(.partition60UssSram1)
    *(.partition60UssSram1.*)

    pharosPartition60UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_60_SRAM1_ALIGNMENT);

    /* address of the partition 60 sections end in Sram1 */
    pharosPartition60Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition60DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition60Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_60_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition60DataSram2Start = ABSOLUTE(.);

    *(.partition60DataSram2)
    *(.partition60DataSram2.*)

    /* address of the partition 60 data section end in RAM */
    . = ALIGN(8);
    pharosPartition60DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 60 in Sram2 */
  .pharosPartition60BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 60 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition60BssSram2Start = .;

    *(.partition60BssSram2)
    *(.partition60BssSram2.*)

    /* address of the partition 60 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition60BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition60UssSram2Start = .;

    *(.partition60UssSram2)
    *(.partition60UssSram2.*)

    pharosPartition60UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_60_SRAM2_ALIGNMENT);

    /* address of the partition 60 sections end in Sram2 */
    pharosPartition60Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition60DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition60Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_60_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition60DataSram3Start = ABSOLUTE(.);

    *(.partition60DataSram3)
    *(.partition60DataSram3.*)

    /* address of the partition 60 data section end in RAM */
    . = ALIGN(8);
    pharosPartition60DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 60 in Sram3 */
  .pharosPartition60BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 60 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition60BssSram3Start = .;

    *(.partition60BssSram3)
    *(.partition60BssSram3.*)

    /* address of the partition 60 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition60BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition60UssSram3Start = .;

    *(.partition60UssSram3)
    *(.partition60UssSram3.*)

    pharosPartition60UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_60_SRAM3_ALIGNMENT);

    /* address of the partition 60 sections end in Sram3 */
    pharosPartition60Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition60DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition60SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_60_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition60DataSramBackupStart = ABSOLUTE(.);

    *(.partition60DataSramBackup)
    *(.partition60DataSramBackup.*)

    /* address of the partition 60 data section end in RAM */
    . = ALIGN(8);
    pharosPartition60DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 60 in SramBackup */
  .pharosPartition60BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 60 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition60BssSramBackupStart = .;

    *(.partition60BssSramBackup)
    *(.partition60BssSramBackup.*)

    /* address of the partition 60 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition60BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition60UssSramBackupStart = .;

    *(.partition60UssSramBackup)
    *(.partition60UssSramBackup.*)

    pharosPartition60UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_60_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 60 sections end in SramBackup */
    pharosPartition60SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition60DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition60SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_60_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition60DataSdramStart = ABSOLUTE(.);

    *(.partition60DataSdram)
    *(.partition60DataSdram.*)

    /* address of the partition 60 data section end in RAM */
    . = ALIGN(8);
    pharosPartition60DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 60 in Sdram */
  .pharosPartition60BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 60 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition60BssSdramStart = .;

    *(.partition60BssSdram)
    *(.partition60BssSdram.*)

    /* address of the partition 60 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition60BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition60UssSdramStart = .;

    *(.partition60UssSdram)
    *(.partition60UssSdram.*)

    pharosPartition60UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_60_SDRAM_ALIGNMENT);

    /* address of the partition 60 sections end in Sdram */
    pharosPartition60SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 61                                                   */
  /******************************************************************/

  /* address in the flash of the partition 61 data to copy it to RAM */
  pharosPartition61DataCcmLoadAddr = LOADADDR(.partition61DataCcm);

  /* address in the flash of the partition 61 data to copy it to RAM */
  pharosPartition61DataSram1LoadAddr = LOADADDR(.partition61DataSram1);

  /* address in the flash of the partition 61 data to copy it to RAM */
  pharosPartition61DataSram2LoadAddr = LOADADDR(.partition61DataSram2);

  /* address in the flash of the partition 61 data to copy it to RAM */
  pharosPartition61DataSram3LoadAddr = LOADADDR(.partition61DataSram3);

  /* address in the flash of the partition 61 data to copy it to RAM */
  pharosPartition61DataSramBackupLoadAddr = LOADADDR(.partition61DataSramBackup);

  /* address in the flash of the partition 61 data to copy it to RAM */
  pharosPartition61DataSdramLoadAddr = LOADADDR(.partition61DataSdram);

  /* data of partition in Ccm */
  .partition61DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition61CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_61_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition61DataCcmStart = ABSOLUTE(.);

    *(.partition61DataCcm)
    *(.partition61DataCcm.*)

    /* address of the partition 61 data section end in RAM */
    . = ALIGN(8);
    pharosPartition61DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 61 in Ccm */
  .pharosPartition61BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 61 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition61BssCcmStart = .;

    *(.partition61BssCcm)
    *(.partition61BssCcm.*)

    /* address of the partition 61 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition61BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition61UssCcmStart = .;

    *(.partition61UssCcm)
    *(.partition61UssCcm.*)

    pharosPartition61UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_61_CCM_ALIGNMENT);

    /* address of the partition 61 sections end in Ccm */
    pharosPartition61CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition61DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition61Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_61_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition61DataSram1Start = ABSOLUTE(.);

    *(.partition61DataSram1)
    *(.partition61DataSram1.*)

    /* address of the partition 61 data section end in RAM */
    . = ALIGN(8);
    pharosPartition61DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 61 in Sram1 */
  .pharosPartition61BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 61 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition61BssSram1Start = .;

    *(.partition61BssSram1)
    *(.partition61BssSram1.*)

    /* address of the partition 61 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition61BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition61UssSram1Start = .;

    *(.partition61UssSram1)
    *(.partition61UssSram1.*)

    pharosPartition61UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_61_SRAM1_ALIGNMENT);

    /* address of the partition 61 sections end in Sram1 */
    pharosPartition61Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition61DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition61Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_61_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition61DataSram2Start = ABSOLUTE(.);

    *(.partition61DataSram2)
    *(.partition61DataSram2.*)

    /* address of the partition 61 data section end in RAM */
    . = ALIGN(8);
    pharosPartition61DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 61 in Sram2 */
  .pharosPartition61BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 61 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition61BssSram2Start = .;

    *(.partition61BssSram2)
    *(.partition61BssSram2.*)

    /* address of the partition 61 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition61BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition61UssSram2Start = .;

    *(.partition61UssSram2)
    *(.partition61UssSram2.*)

    pharosPartition61UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_61_SRAM2_ALIGNMENT);

    /* address of the partition 61 sections end in Sram2 */
    pharosPartition61Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition61DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition61Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_61_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition61DataSram3Start = ABSOLUTE(.);

    *(.partition61DataSram3)
    *(.partition61DataSram3.*)

    /* address of the partition 61 data section end in RAM */
    . = ALIGN(8);
    pharosPartition61DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 61 in Sram3 */
  .pharosPartition61BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 61 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition61BssSram3Start = .;

    *(.partition61BssSram3)
    *(.partition61BssSram3.*)

    /* address of the partition 61 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition61BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition61UssSram3Start = .;

    *(.partition61UssSram3)
    *(.partition61UssSram3.*)

    pharosPartition61UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_61_SRAM3_ALIGNMENT);

    /* address of the partition 61 sections end in Sram3 */
    pharosPartition61Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition61DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition61SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_61_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition61DataSramBackupStart = ABSOLUTE(.);

    *(.partition61DataSramBackup)
    *(.partition61DataSramBackup.*)

    /* address of the partition 61 data section end in RAM */
    . = ALIGN(8);
    pharosPartition61DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 61 in SramBackup */
  .pharosPartition61BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 61 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition61BssSramBackupStart = .;

    *(.partition61BssSramBackup)
    *(.partition61BssSramBackup.*)

    /* address of the partition 61 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition61BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition61UssSramBackupStart = .;

    *(.partition61UssSramBackup)
    *(.partition61UssSramBackup.*)

    pharosPartition61UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_61_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 61 sections end in SramBackup */
    pharosPartition61SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition61DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition61SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_61_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition61DataSdramStart = ABSOLUTE(.);

    *(.partition61DataSdram)
    *(.partition61DataSdram.*)

    /* address of the partition 61 data section end in RAM */
    . = ALIGN(8);
    pharosPartition61DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 61 in Sdram */
  .pharosPartition61BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 61 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition61BssSdramStart = .;

    *(.partition61BssSdram)
    *(.partition61BssSdram.*)

    /* address of the partition 61 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition61BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition61UssSdramStart = .;

    *(.partition61UssSdram)
    *(.partition61UssSdram.*)

    pharosPartition61UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_61_SDRAM_ALIGNMENT);

    /* address of the partition 61 sections end in Sdram */
    pharosPartition61SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 62                                                   */
  /******************************************************************/

  /* address in the flash of the partition 62 data to copy it to RAM */
  pharosPartition62DataCcmLoadAddr = LOADADDR(.partition62DataCcm);

  /* address in the flash of the partition 62 data to copy it to RAM */
  pharosPartition62DataSram1LoadAddr = LOADADDR(.partition62DataSram1);

  /* address in the flash of the partition 62 data to copy it to RAM */
  pharosPartition62DataSram2LoadAddr = LOADADDR(.partition62DataSram2);

  /* address in the flash of the partition 62 data to copy it to RAM */
  pharosPartition62DataSram3LoadAddr = LOADADDR(.partition62DataSram3);

  /* address in the flash of the partition 62 data to copy it to RAM */
  pharosPartition62DataSramBackupLoadAddr = LOADADDR(.partition62DataSramBackup);

  /* address in the flash of the partition 62 data to copy it to RAM */
  pharosPartition62DataSdramLoadAddr = LOADADDR(.partition62DataSdram);

  /* data of partition in Ccm */
  .partition62DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition62CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_62_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition62DataCcmStart = ABSOLUTE(.);

    *(.partition62DataCcm)
    *(.partition62DataCcm.*)

    /* address of the partition 62 data section end in RAM */
    . = ALIGN(8);
    pharosPartition62DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 62 in Ccm */
  .pharosPartition62BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 62 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition62BssCcmStart = .;

    *(.partition62BssCcm)
    *(.partition62BssCcm.*)

    /* address of the partition 62 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition62BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition62UssCcmStart = .;

    *(.partition62UssCcm)
    *(.partition62UssCcm.*)

    pharosPartition62UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_62_CCM_ALIGNMENT);

    /* address of the partition 62 sections end in Ccm */
    pharosPartition62CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition62DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition62Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_62_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition62DataSram1Start = ABSOLUTE(.);

    *(.partition62DataSram1)
    *(.partition62DataSram1.*)

    /* address of the partition 62 data section end in RAM */
    . = ALIGN(8);
    pharosPartition62DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 62 in Sram1 */
  .pharosPartition62BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 62 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition62BssSram1Start = .;

    *(.partition62BssSram1)
    *(.partition62BssSram1.*)

    /* address of the partition 62 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition62BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition62UssSram1Start = .;

    *(.partition62UssSram1)
    *(.partition62UssSram1.*)

    pharosPartition62UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_62_SRAM1_ALIGNMENT);

    /* address of the partition 62 sections end in Sram1 */
    pharosPartition62Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition62DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition62Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_62_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition62DataSram2Start = ABSOLUTE(.);

    *(.partition62DataSram2)
    *(.partition62DataSram2.*)

    /* address of the partition 62 data section end in RAM */
    . = ALIGN(8);
    pharosPartition62DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 62 in Sram2 */
  .pharosPartition62BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 62 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition62BssSram2Start = .;

    *(.partition62BssSram2)
    *(.partition62BssSram2.*)

    /* address of the partition 62 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition62BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition62UssSram2Start = .;

    *(.partition62UssSram2)
    *(.partition62UssSram2.*)

    pharosPartition62UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_62_SRAM2_ALIGNMENT);

    /* address of the partition 62 sections end in Sram2 */
    pharosPartition62Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition62DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition62Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_62_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition62DataSram3Start = ABSOLUTE(.);

    *(.partition62DataSram3)
    *(.partition62DataSram3.*)

    /* address of the partition 62 data section end in RAM */
    . = ALIGN(8);
    pharosPartition62DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 62 in Sram3 */
  .pharosPartition62BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 62 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition62BssSram3Start = .;

    *(.partition62BssSram3)
    *(.partition62BssSram3.*)

    /* address of the partition 62 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition62BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition62UssSram3Start = .;

    *(.partition62UssSram3)
    *(.partition62UssSram3.*)

    pharosPartition62UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_62_SRAM3_ALIGNMENT);

    /* address of the partition 62 sections end in Sram3 */
    pharosPartition62Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition62DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition62SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_62_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition62DataSramBackupStart = ABSOLUTE(.);

    *(.partition62DataSramBackup)
    *(.partition62DataSramBackup.*)

    /* address of the partition 62 data section end in RAM */
    . = ALIGN(8);
    pharosPartition62DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 62 in SramBackup */
  .pharosPartition62BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 62 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition62BssSramBackupStart = .;

    *(.partition62BssSramBackup)
    *(.partition62BssSramBackup.*)

    /* address of the partition 62 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition62BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition62UssSramBackupStart = .;

    *(.partition62UssSramBackup)
    *(.partition62UssSramBackup.*)

    pharosPartition62UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_62_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 62 sections end in SramBackup */
    pharosPartition62SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition62DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition62SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_62_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition62DataSdramStart = ABSOLUTE(.);

    *(.partition62DataSdram)
    *(.partition62DataSdram.*)

    /* address of the partition 62 data section end in RAM */
    . = ALIGN(8);
    pharosPartition62DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 62 in Sdram */
  .pharosPartition62BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 62 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition62BssSdramStart = .;

    *(.partition62BssSdram)
    *(.partition62BssSdram.*)

    /* address of the partition 62 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition62BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition62UssSdramStart = .;

    *(.partition62UssSdram)
    *(.partition62UssSdram.*)

    pharosPartition62UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_62_SDRAM_ALIGNMENT);

    /* address of the partition 62 sections end in Sdram */
    pharosPartition62SdramEnd = .;

  } > sdram

  /******************************************************************/
  /* Partition 63                                                   */
  /******************************************************************/

  /* address in the flash of the partition 63 data to copy it to RAM */
  pharosPartition63DataCcmLoadAddr = LOADADDR(.partition63DataCcm);

  /* address in the flash of the partition 63 data to copy it to RAM */
  pharosPartition63DataSram1LoadAddr = LOADADDR(.partition63DataSram1);

  /* address in the flash of the partition 63 data to copy it to RAM */
  pharosPartition63DataSram2LoadAddr = LOADADDR(.partition63DataSram2);

  /* address in the flash of the partition 63 data to copy it to RAM */
  pharosPartition63DataSram3LoadAddr = LOADADDR(.partition63DataSram3);

  /* address in the flash of the partition 63 data to copy it to RAM */
  pharosPartition63DataSramBackupLoadAddr = LOADADDR(.partition63DataSramBackup);

  /* address in the flash of the partition 63 data to copy it to RAM */
  pharosPartition63DataSdramLoadAddr = LOADADDR(.partition63DataSdram);

  /* data of partition in Ccm */
  .partition63DataCcm : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition63CcmStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_63_CCM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition63DataCcmStart = ABSOLUTE(.);

    *(.partition63DataCcm)
    *(.partition63DataCcm.*)

    /* address of the partition 63 data section end in RAM */
    . = ALIGN(8);
    pharosPartition63DataCcmEnd = .;

  } > ccm AT > flash = 0xff


  /* bss of partition 63 in Ccm */
  .pharosPartition63BssCcm (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 63 bss section start in Ccm */
    . = ALIGN(8);
    pharosPartition63BssCcmStart = .;

    *(.partition63BssCcm)
    *(.partition63BssCcm.*)

    /* address of the partition 63 bss section end in Ccm */
    . = ALIGN(8);
    pharosPartition63BssCcmEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition63UssCcmStart = .;

    *(.partition63UssCcm)
    *(.partition63UssCcm.*)

    pharosPartition63UssCcmEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_63_CCM_ALIGNMENT);

    /* address of the partition 63 sections end in Ccm */
    pharosPartition63CcmEnd = .;

  } > ccm

  /* data of partition in Sram1 */
  .partition63DataSram1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition63Sram1Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_63_SRAM1_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition63DataSram1Start = ABSOLUTE(.);

    *(.partition63DataSram1)
    *(.partition63DataSram1.*)

    /* address of the partition 63 data section end in RAM */
    . = ALIGN(8);
    pharosPartition63DataSram1End = .;

  } > sram1 AT > flash = 0xff


  /* bss of partition 63 in Sram1 */
  .pharosPartition63BssSram1 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 63 bss section start in Sram1 */
    . = ALIGN(8);
    pharosPartition63BssSram1Start = .;

    *(.partition63BssSram1)
    *(.partition63BssSram1.*)

    /* address of the partition 63 bss section end in Sram1 */
    . = ALIGN(8);
    pharosPartition63BssSram1End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition63UssSram1Start = .;

    *(.partition63UssSram1)
    *(.partition63UssSram1.*)

    pharosPartition63UssSram1End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_63_SRAM1_ALIGNMENT);

    /* address of the partition 63 sections end in Sram1 */
    pharosPartition63Sram1End = .;

  } > sram1

  /* data of partition in Sram2 */
  .partition63DataSram2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition63Sram2Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_63_SRAM2_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition63DataSram2Start = ABSOLUTE(.);

    *(.partition63DataSram2)
    *(.partition63DataSram2.*)

    /* address of the partition 63 data section end in RAM */
    . = ALIGN(8);
    pharosPartition63DataSram2End = .;

  } > sram2 AT > flash = 0xff


  /* bss of partition 63 in Sram2 */
  .pharosPartition63BssSram2 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 63 bss section start in Sram2 */
    . = ALIGN(8);
    pharosPartition63BssSram2Start = .;

    *(.partition63BssSram2)
    *(.partition63BssSram2.*)

    /* address of the partition 63 bss section end in Sram2 */
    . = ALIGN(8);
    pharosPartition63BssSram2End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition63UssSram2Start = .;

    *(.partition63UssSram2)
    *(.partition63UssSram2.*)

    pharosPartition63UssSram2End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_63_SRAM2_ALIGNMENT);

    /* address of the partition 63 sections end in Sram2 */
    pharosPartition63Sram2End = .;

  } > sram2

  /* data of partition in Sram3 */
  .partition63DataSram3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition63Sram3Start = ABSOLUTE(.); 

    . = ALIGN(PARTITION_63_SRAM3_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition63DataSram3Start = ABSOLUTE(.);

    *(.partition63DataSram3)
    *(.partition63DataSram3.*)

    /* address of the partition 63 data section end in RAM */
    . = ALIGN(8);
    pharosPartition63DataSram3End = .;

  } > sram3 AT > flash = 0xff


  /* bss of partition 63 in Sram3 */
  .pharosPartition63BssSram3 (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 63 bss section start in Sram3 */
    . = ALIGN(8);
    pharosPartition63BssSram3Start = .;

    *(.partition63BssSram3)
    *(.partition63BssSram3.*)

    /* address of the partition 63 bss section end in Sram3 */
    . = ALIGN(8);
    pharosPartition63BssSram3End = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition63UssSram3Start = .;

    *(.partition63UssSram3)
    *(.partition63UssSram3.*)

    pharosPartition63UssSram3End = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_63_SRAM3_ALIGNMENT);

    /* address of the partition 63 sections end in Sram3 */
    pharosPartition63Sram3End = .;

  } > sram3

  /* data of partition in SramBackup */
  .partition63DataSramBackup : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition63SramBackupStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_63_SRAMBACKUP_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition63DataSramBackupStart = ABSOLUTE(.);

    *(.partition63DataSramBackup)
    *(.partition63DataSramBackup.*)

    /* address of the partition 63 data section end in RAM */
    . = ALIGN(8);
    pharosPartition63DataSramBackupEnd = .;

  } > sramBackup AT > flash = 0xff


  /* bss of partition 63 in SramBackup */
  .pharosPartition63BssSramBackup (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 63 bss section start in SramBackup */
    . = ALIGN(8);
    pharosPartition63BssSramBackupStart = .;

    *(.partition63BssSramBackup)
    *(.partition63BssSramBackup.*)

    /* address of the partition 63 bss section end in SramBackup */
    . = ALIGN(8);
    pharosPartition63BssSramBackupEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition63UssSramBackupStart = .;

    *(.partition63UssSramBackup)
    *(.partition63UssSramBackup.*)

    pharosPartition63UssSramBackupEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_63_SRAMBACKUP_ALIGNMENT);

    /* address of the partition 63 sections end in SramBackup */
    pharosPartition63SramBackupEnd = .;

  } > sramBackup

  /* data of partition in Sdram */
  .partition63DataSdram : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    pharosPartition63SdramStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_63_SDRAM_ALIGNMENT);

    /* address where the MPU shall be initialized to (correctly aligned) */
    pharosPartition63DataSdramStart = ABSOLUTE(.);

    *(.partition63DataSdram)
    *(.partition63DataSdram.*)

    /* address of the partition 63 data section end in RAM */
    . = ALIGN(8);
    pharosPartition63DataSdramEnd = .;

  } > sdram AT > flash = 0xff


  /* bss of partition 63 in Sdram */
  .pharosPartition63BssSdram (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 63 bss section start in Sdram */
    . = ALIGN(8);
    pharosPartition63BssSdramStart = .;

    *(.partition63BssSdram)
    *(.partition63BssSdram.*)

    /* address of the partition 63 bss section end in Sdram */
    . = ALIGN(8);
    pharosPartition63BssSdramEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    pharosPartition63UssSdramStart = .;

    *(.partition63UssSdram)
    *(.partition63UssSdram.*)

    pharosPartition63UssSdramEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_63_SDRAM_ALIGNMENT);

    /* address of the partition 63 sections end in Sdram */
    pharosPartition63SdramEnd = .;

  } > sdram


  /******************************************************************/
  /*                     End of partitions                          */
  /******************************************************************/

  /******************************************************************/
  /*             Start of Inter-partition calls area                */
  /******************************************************************/

  /* place this section last because it usually takes very little space and therefore has low alignment constraints */
  pharosIpCallDataLoadAddr = LOADADDR(.ipCall);
  pharosIpCallDataSize = (pharosIpCallEnd - pharosIpCallSectionStart) / 8;

  /* this area will be read-only for the user application but must be written in supervisor mode at initialization */
  .ipCall : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    pharosIpCallSectionStart = ABSOLUTE(.);
    
    . = ALIGN(IP_CALL_ALIGNMENT);

    /* address of the shared section start in RAM */
    pharosIpCallStart = ABSOLUTE(.);
    *(.ipCall)  
    *(.ipCall.*)  

    . = ALIGN(8);

    /* address of the shared section end in RAM */
    pharosIpCallEnd = ABSOLUTE(.);

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(IP_CALL_ALIGNMENT);

  } > sram2 AT > flash = 0xff



}