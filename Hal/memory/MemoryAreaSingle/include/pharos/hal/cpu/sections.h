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


#ifndef PHAROS_SECTIONS_H
#define	PHAROS_SECTIONS_H


/**
 * Due to some GCC bug, some variables we have to specifically state that they belong to the read-only data. The "const"
 * modifier should have taken care of it, but it doesn't.
 */
#define RONLY_SECTION               __attribute__ ((section (".rodata"))) 


/**
 * Kernel data section (for initialized data)
 */
#define KERNEL_SECTION              __attribute__ ((section (".kernel"))) 

/**
 * Kernel data section (for initialized data to zero)
 */
#define KERNEL_SECTION_BSS          __attribute__ ((section (".kernelBss"))) 

/**
 * Kernel data section (for non-initialized data)
 */
#define KERNEL_SECTION_USS          __attribute__ ((section (".kernelUss"))) 

/**
 * Bulk section. Channel bulks should be placed on this section to ensure that no partition writes to them
 */
#define BULK_SECTION                __attribute__ ((section (".kernelBulks"))) 

/**
 * Shared data section (for initialized data)
 */
#define SHARED_SECTION              __attribute__ ((section (".shared"))) 

/**
 * Shared data section (for initialized data to zero)
 */
#define SHARED_SECTION_BSS          __attribute__ ((section (".sharedBss"))) 

/**
 * Shared data section (for non-initialized data)
 */
#define SHARED_SECTION_USS          __attribute__ ((section (".sharedUss"))) 

/**
 * Inter-partition calls section (text area)
 */
#define IP_CALL_SECTION             __attribute__ ((section (".ipCall")))


/**
 * Partitions data section
 */
/* definition of partition 0 data section (initialized data) */
#define PARTITION_0_SECTION         __attribute__((section(".partition0"))) 

/* definition of partition 1 data section (initialized data) */
#define PARTITION_1_SECTION         __attribute__((section(".partition1"))) 

/* definition of partition 2 data section (initialized data) */
#define PARTITION_2_SECTION         __attribute__((section(".partition2"))) 

/* definition of partition 3 data section (initialized data) */
#define PARTITION_3_SECTION         __attribute__((section(".partition3"))) 

/* definition of partition 4 data section (initialized data) */
#define PARTITION_4_SECTION         __attribute__((section(".partition4"))) 

/* definition of partition 5 data section (initialized data) */
#define PARTITION_5_SECTION         __attribute__((section(".partition5"))) 

/* definition of partition 6 data section (initialized data) */
#define PARTITION_6_SECTION         __attribute__((section(".partition6"))) 

/* definition of partition 7 data section (initialized data) */
#define PARTITION_7_SECTION         __attribute__((section(".partition7"))) 

/* definition of partition 8 data section (initialized data) */
#define PARTITION_8_SECTION         __attribute__((section(".partition8"))) 

/* definition of partition 9 data section (initialized data) */
#define PARTITION_9_SECTION         __attribute__((section(".partition9"))) 

/* definition of partition 10 data section (initialized data) */
#define PARTITION_10_SECTION        __attribute__((section(".partition10"))) 

/* definition of partition 11 data section (initialized data) */
#define PARTITION_11_SECTION        __attribute__((section(".partition11"))) 

/* definition of partition 12 data section (initialized data) */
#define PARTITION_12_SECTION        __attribute__((section(".partition12"))) 

/* definition of partition 13 data section (initialized data) */
#define PARTITION_13_SECTION        __attribute__((section(".partition13"))) 

/* definition of partition 14 data section (initialized data) */
#define PARTITION_14_SECTION        __attribute__((section(".partition14"))) 

/* definition of partition 15 data section (initialized data) */
#define PARTITION_15_SECTION        __attribute__((section(".partition15"))) 

/* definition of partition 16 data section (initialized data) */
#define PARTITION_16_SECTION        __attribute__((section(".partition16"))) 

/* definition of partition 17 data section (initialized data) */
#define PARTITION_17_SECTION        __attribute__((section(".partition17"))) 

/* definition of partition 18 data section (initialized data) */
#define PARTITION_18_SECTION        __attribute__((section(".partition18"))) 

/* definition of partition 19 data section (initialized data) */
#define PARTITION_19_SECTION        __attribute__((section(".partition19"))) 

/* definition of partition 20 data section (initialized data) */
#define PARTITION_20_SECTION        __attribute__((section(".partition20"))) 

/* definition of partition 21 data section (initialized data) */
#define PARTITION_21_SECTION        __attribute__((section(".partition21"))) 

/* definition of partition 22 data section (initialized data) */
#define PARTITION_22_SECTION        __attribute__((section(".partition22"))) 

/* definition of partition 23 data section (initialized data) */
#define PARTITION_23_SECTION        __attribute__((section(".partition23"))) 

/* definition of partition 24 data section (initialized data) */
#define PARTITION_24_SECTION        __attribute__((section(".partition24"))) 

/* definition of partition 25 data section (initialized data) */
#define PARTITION_25_SECTION        __attribute__((section(".partition25"))) 

/* definition of partition 26 data section (initialized data) */
#define PARTITION_26_SECTION        __attribute__((section(".partition26"))) 

/* definition of partition 27 data section (initialized data) */
#define PARTITION_27_SECTION        __attribute__((section(".partition27"))) 

/* definition of partition 28 data section (initialized data) */
#define PARTITION_28_SECTION        __attribute__((section(".partition28"))) 

/* definition of partition 29 data section (initialized data) */
#define PARTITION_29_SECTION        __attribute__((section(".partition29"))) 

/* definition of partition 30 data section (initialized data) */
#define PARTITION_30_SECTION        __attribute__((section(".partition30"))) 

/* definition of partition 31 data section (initialized data) */
#define PARTITION_31_SECTION        __attribute__((section(".partition31"))) 

/* definition of partition 32 data section (initialized data) */
#define PARTITION_32_SECTION        __attribute__((section(".partition32"))) 

/* definition of partition 33 data section (initialized data) */
#define PARTITION_33_SECTION        __attribute__((section(".partition33"))) 

/* definition of partition 34 data section (initialized data) */
#define PARTITION_34_SECTION        __attribute__((section(".partition34"))) 

/* definition of partition 35 data section (initialized data) */
#define PARTITION_35_SECTION        __attribute__((section(".partition35"))) 

/* definition of partition 36 data section (initialized data) */
#define PARTITION_36_SECTION        __attribute__((section(".partition36"))) 

/* definition of partition 37 data section (initialized data) */
#define PARTITION_37_SECTION        __attribute__((section(".partition37"))) 

/* definition of partition 38 data section (initialized data) */
#define PARTITION_38_SECTION        __attribute__((section(".partition38"))) 

/* definition of partition 39 data section (initialized data) */
#define PARTITION_39_SECTION        __attribute__((section(".partition39"))) 

/* definition of partition 40 data section (initialized data) */
#define PARTITION_40_SECTION        __attribute__((section(".partition40"))) 

/* definition of partition 41 data section (initialized data) */
#define PARTITION_41_SECTION        __attribute__((section(".partition41"))) 

/* definition of partition 42 data section (initialized data) */
#define PARTITION_42_SECTION        __attribute__((section(".partition42"))) 

/* definition of partition 43 data section (initialized data) */
#define PARTITION_43_SECTION        __attribute__((section(".partition43"))) 

/* definition of partition 44 data section (initialized data) */
#define PARTITION_44_SECTION        __attribute__((section(".partition44"))) 

/* definition of partition 45 data section (initialized data) */
#define PARTITION_45_SECTION        __attribute__((section(".partition45"))) 

/* definition of partition 46 data section (initialized data) */
#define PARTITION_46_SECTION        __attribute__((section(".partition46"))) 

/* definition of partition 47 data section (initialized data) */
#define PARTITION_47_SECTION        __attribute__((section(".partition47"))) 

/* definition of partition 48 data section (initialized data) */
#define PARTITION_48_SECTION        __attribute__((section(".partition48"))) 

/* definition of partition 49 data section (initialized data) */
#define PARTITION_49_SECTION        __attribute__((section(".partition49"))) 

/* definition of partition 50 data section (initialized data) */
#define PARTITION_50_SECTION        __attribute__((section(".partition50"))) 

/* definition of partition 51 data section (initialized data) */
#define PARTITION_51_SECTION        __attribute__((section(".partition51"))) 

/* definition of partition 52 data section (initialized data) */
#define PARTITION_52_SECTION        __attribute__((section(".partition52"))) 

/* definition of partition 53 data section (initialized data) */
#define PARTITION_53_SECTION        __attribute__((section(".partition53"))) 

/* definition of partition 54 data section (initialized data) */
#define PARTITION_54_SECTION        __attribute__((section(".partition54"))) 

/* definition of partition 55 data section (initialized data) */
#define PARTITION_55_SECTION        __attribute__((section(".partition55"))) 

/* definition of partition 56 data section (initialized data) */
#define PARTITION_56_SECTION        __attribute__((section(".partition56"))) 

/* definition of partition 57 data section (initialized data) */
#define PARTITION_57_SECTION        __attribute__((section(".partition57"))) 

/* definition of partition 58 data section (initialized data) */
#define PARTITION_58_SECTION        __attribute__((section(".partition58"))) 

/* definition of partition 59 data section (initialized data) */
#define PARTITION_59_SECTION        __attribute__((section(".partition59"))) 

/* definition of partition 60 data section (initialized data) */
#define PARTITION_60_SECTION        __attribute__((section(".partition60"))) 

/* definition of partition 61 data section (initialized data) */
#define PARTITION_61_SECTION        __attribute__((section(".partition61"))) 

/* definition of partition 62 data section (initialized data) */
#define PARTITION_62_SECTION        __attribute__((section(".partition62"))) 

/* definition of partition 63 data section (initialized data) */
#define PARTITION_63_SECTION        __attribute__((section(".partition63"))) 



/* definition of partition 0 BSS section (initialized data with zeros) */
#define PARTITION_0_SECTION_BSS     __attribute__((section(".partition0Bss"))) 

/* definition of partition 1 BSS section (initialized data with zeros) */
#define PARTITION_1_SECTION_BSS     __attribute__((section(".partition1Bss"))) 

/* definition of partition 2 BSS section (initialized data with zeros) */
#define PARTITION_2_SECTION_BSS     __attribute__((section(".partition2Bss"))) 

/* definition of partition 3 BSS section (initialized data with zeros) */
#define PARTITION_3_SECTION_BSS     __attribute__((section(".partition3Bss"))) 

/* definition of partition 4 BSS section (initialized data with zeros) */
#define PARTITION_4_SECTION_BSS     __attribute__((section(".partition4Bss"))) 

/* definition of partition 5 BSS section (initialized data with zeros) */
#define PARTITION_5_SECTION_BSS     __attribute__((section(".partition5Bss"))) 

/* definition of partition 6 BSS section (initialized data with zeros) */
#define PARTITION_6_SECTION_BSS     __attribute__((section(".partition6Bss"))) 

/* definition of partition 7 BSS section (initialized data with zeros) */
#define PARTITION_7_SECTION_BSS     __attribute__((section(".partition7Bss"))) 

/* definition of partition 8 BSS section (initialized data with zeros) */
#define PARTITION_8_SECTION_BSS     __attribute__((section(".partition8Bss"))) 

/* definition of partition 9 BSS section (initialized data with zeros) */
#define PARTITION_9_SECTION_BSS     __attribute__((section(".partition9Bss"))) 

/* definition of partition 10 BSS section (initialized data with zeros) */
#define PARTITION_10_SECTION_BSS     __attribute__((section(".partition10Bss"))) 

/* definition of partition 11 BSS section (initialized data with zeros) */
#define PARTITION_11_SECTION_BSS     __attribute__((section(".partition11Bss"))) 

/* definition of partition 12 BSS section (initialized data with zeros) */
#define PARTITION_12_SECTION_BSS     __attribute__((section(".partition12Bss"))) 

/* definition of partition 13 BSS section (initialized data with zeros) */
#define PARTITION_13_SECTION_BSS     __attribute__((section(".partition13Bss"))) 

/* definition of partition 14 BSS section (initialized data with zeros) */
#define PARTITION_14_SECTION_BSS     __attribute__((section(".partition14Bss"))) 

/* definition of partition 15 BSS section (initialized data with zeros) */
#define PARTITION_15_SECTION_BSS     __attribute__((section(".partition15Bss"))) 

/* definition of partition 16 BSS section (initialized data with zeros) */
#define PARTITION_16_SECTION_BSS     __attribute__((section(".partition16Bss"))) 

/* definition of partition 17 BSS section (initialized data with zeros) */
#define PARTITION_17_SECTION_BSS     __attribute__((section(".partition17Bss"))) 

/* definition of partition 18 BSS section (initialized data with zeros) */
#define PARTITION_18_SECTION_BSS     __attribute__((section(".partition18Bss"))) 

/* definition of partition 19 BSS section (initialized data with zeros) */
#define PARTITION_19_SECTION_BSS     __attribute__((section(".partition19Bss"))) 

/* definition of partition 20 BSS section (initialized data with zeros) */
#define PARTITION_20_SECTION_BSS     __attribute__((section(".partition20Bss"))) 

/* definition of partition 21 BSS section (initialized data with zeros) */
#define PARTITION_21_SECTION_BSS     __attribute__((section(".partition21Bss"))) 

/* definition of partition 22 BSS section (initialized data with zeros) */
#define PARTITION_22_SECTION_BSS     __attribute__((section(".partition22Bss"))) 

/* definition of partition 23 BSS section (initialized data with zeros) */
#define PARTITION_23_SECTION_BSS     __attribute__((section(".partition23Bss"))) 

/* definition of partition 24 BSS section (initialized data with zeros) */
#define PARTITION_24_SECTION_BSS     __attribute__((section(".partition24Bss"))) 

/* definition of partition 25 BSS section (initialized data with zeros) */
#define PARTITION_25_SECTION_BSS     __attribute__((section(".partition25Bss"))) 

/* definition of partition 26 BSS section (initialized data with zeros) */
#define PARTITION_26_SECTION_BSS     __attribute__((section(".partition26Bss"))) 

/* definition of partition 27 BSS section (initialized data with zeros) */
#define PARTITION_27_SECTION_BSS     __attribute__((section(".partition27Bss"))) 

/* definition of partition 28 BSS section (initialized data with zeros) */
#define PARTITION_28_SECTION_BSS     __attribute__((section(".partition28Bss"))) 

/* definition of partition 29 BSS section (initialized data with zeros) */
#define PARTITION_29_SECTION_BSS     __attribute__((section(".partition29Bss"))) 

/* definition of partition 30 BSS section (initialized data with zeros) */
#define PARTITION_30_SECTION_BSS     __attribute__((section(".partition30Bss"))) 

/* definition of partition 31 BSS section (initialized data with zeros) */
#define PARTITION_31_SECTION_BSS     __attribute__((section(".partition31Bss"))) 

/* definition of partition 32 BSS section (initialized data with zeros) */
#define PARTITION_32_SECTION_BSS     __attribute__((section(".partition32Bss"))) 

/* definition of partition 33 BSS section (initialized data with zeros) */
#define PARTITION_33_SECTION_BSS     __attribute__((section(".partition33Bss"))) 

/* definition of partition 34 BSS section (initialized data with zeros) */
#define PARTITION_34_SECTION_BSS     __attribute__((section(".partition34Bss"))) 

/* definition of partition 35 BSS section (initialized data with zeros) */
#define PARTITION_35_SECTION_BSS     __attribute__((section(".partition35Bss"))) 

/* definition of partition 36 BSS section (initialized data with zeros) */
#define PARTITION_36_SECTION_BSS     __attribute__((section(".partition36Bss"))) 

/* definition of partition 37 BSS section (initialized data with zeros) */
#define PARTITION_37_SECTION_BSS     __attribute__((section(".partition37Bss"))) 

/* definition of partition 38 BSS section (initialized data with zeros) */
#define PARTITION_38_SECTION_BSS     __attribute__((section(".partition38Bss"))) 

/* definition of partition 39 BSS section (initialized data with zeros) */
#define PARTITION_39_SECTION_BSS     __attribute__((section(".partition39Bss"))) 

/* definition of partition 40 BSS section (initialized data with zeros) */
#define PARTITION_40_SECTION_BSS     __attribute__((section(".partition40Bss"))) 

/* definition of partition 41 BSS section (initialized data with zeros) */
#define PARTITION_41_SECTION_BSS     __attribute__((section(".partition41Bss"))) 

/* definition of partition 42 BSS section (initialized data with zeros) */
#define PARTITION_42_SECTION_BSS     __attribute__((section(".partition42Bss"))) 

/* definition of partition 43 BSS section (initialized data with zeros) */
#define PARTITION_43_SECTION_BSS     __attribute__((section(".partition43Bss"))) 

/* definition of partition 44 BSS section (initialized data with zeros) */
#define PARTITION_44_SECTION_BSS     __attribute__((section(".partition44Bss"))) 

/* definition of partition 45 BSS section (initialized data with zeros) */
#define PARTITION_45_SECTION_BSS     __attribute__((section(".partition45Bss"))) 

/* definition of partition 46 BSS section (initialized data with zeros) */
#define PARTITION_46_SECTION_BSS     __attribute__((section(".partition46Bss"))) 

/* definition of partition 47 BSS section (initialized data with zeros) */
#define PARTITION_47_SECTION_BSS     __attribute__((section(".partition47Bss"))) 

/* definition of partition 48 BSS section (initialized data with zeros) */
#define PARTITION_48_SECTION_BSS     __attribute__((section(".partition48Bss"))) 

/* definition of partition 49 BSS section (initialized data with zeros) */
#define PARTITION_49_SECTION_BSS     __attribute__((section(".partition49Bss"))) 

/* definition of partition 50 BSS section (initialized data with zeros) */
#define PARTITION_50_SECTION_BSS     __attribute__((section(".partition50Bss"))) 

/* definition of partition 51 BSS section (initialized data with zeros) */
#define PARTITION_51_SECTION_BSS     __attribute__((section(".partition51Bss"))) 

/* definition of partition 52 BSS section (initialized data with zeros) */
#define PARTITION_52_SECTION_BSS     __attribute__((section(".partition52Bss"))) 

/* definition of partition 53 BSS section (initialized data with zeros) */
#define PARTITION_53_SECTION_BSS     __attribute__((section(".partition53Bss"))) 

/* definition of partition 54 BSS section (initialized data with zeros) */
#define PARTITION_54_SECTION_BSS     __attribute__((section(".partition54Bss"))) 

/* definition of partition 55 BSS section (initialized data with zeros) */
#define PARTITION_55_SECTION_BSS     __attribute__((section(".partition55Bss"))) 

/* definition of partition 56 BSS section (initialized data with zeros) */
#define PARTITION_56_SECTION_BSS     __attribute__((section(".partition56Bss"))) 

/* definition of partition 57 BSS section (initialized data with zeros) */
#define PARTITION_57_SECTION_BSS     __attribute__((section(".partition57Bss"))) 

/* definition of partition 58 BSS section (initialized data with zeros) */
#define PARTITION_58_SECTION_BSS     __attribute__((section(".partition58Bss"))) 

/* definition of partition 59 BSS section (initialized data with zeros) */
#define PARTITION_59_SECTION_BSS     __attribute__((section(".partition59Bss"))) 

/* definition of partition 60 BSS section (initialized data with zeros) */
#define PARTITION_60_SECTION_BSS     __attribute__((section(".partition60Bss"))) 

/* definition of partition 61 BSS section (initialized data with zeros) */
#define PARTITION_61_SECTION_BSS     __attribute__((section(".partition61Bss"))) 

/* definition of partition 62 BSS section (initialized data with zeros) */
#define PARTITION_62_SECTION_BSS     __attribute__((section(".partition62Bss"))) 

/* definition of partition 63 BSS section (initialized data with zeros) */
#define PARTITION_63_SECTION_BSS     __attribute__((section(".partition63Bss"))) 



/* definition of partition 0 USS section (non initialized data) */
#define PARTITION_0_SECTION_USS     __attribute__ ((section (".partition0Uss")))

/* definition of partition 1 USS section (non initialized data) */
#define PARTITION_1_SECTION_USS     __attribute__ ((section (".partition1Uss")))

/* definition of partition 2 USS section (non initialized data) */
#define PARTITION_2_SECTION_USS     __attribute__ ((section (".partition2Uss")))

/* definition of partition 3 USS section (non initialized data) */
#define PARTITION_3_SECTION_USS     __attribute__ ((section (".partition3Uss")))

/* definition of partition 4 USS section (non initialized data) */
#define PARTITION_4_SECTION_USS     __attribute__ ((section (".partition4Uss")))

/* definition of partition 5 USS section (non initialized data) */
#define PARTITION_5_SECTION_USS     __attribute__ ((section (".partition5Uss")))

/* definition of partition 6 USS section (non initialized data) */
#define PARTITION_6_SECTION_USS     __attribute__ ((section (".partition6Uss")))

/* definition of partition 7 USS section (non initialized data) */
#define PARTITION_7_SECTION_USS     __attribute__ ((section (".partition7Uss")))

/* definition of partition 8 USS section (non initialized data) */
#define PARTITION_8_SECTION_USS     __attribute__ ((section (".partition8Uss")))

/* definition of partition 9 USS section (non initialized data) */
#define PARTITION_9_SECTION_USS     __attribute__ ((section (".partition9Uss")))

/* definition of partition 10 USS section (non initialized data) */
#define PARTITION_10_SECTION_USS     __attribute__ ((section (".partition10Uss")))

/* definition of partition 11 USS section (non initialized data) */
#define PARTITION_11_SECTION_USS     __attribute__ ((section (".partition11Uss")))

/* definition of partition 12 USS section (non initialized data) */
#define PARTITION_12_SECTION_USS     __attribute__ ((section (".partition12Uss")))

/* definition of partition 13 USS section (non initialized data) */
#define PARTITION_13_SECTION_USS     __attribute__ ((section (".partition13Uss")))

/* definition of partition 14 USS section (non initialized data) */
#define PARTITION_14_SECTION_USS     __attribute__ ((section (".partition14Uss")))

/* definition of partition 15 USS section (non initialized data) */
#define PARTITION_15_SECTION_USS     __attribute__ ((section (".partition15Uss")))

/* definition of partition 16 USS section (non initialized data) */
#define PARTITION_16_SECTION_USS     __attribute__ ((section (".partition16Uss")))

/* definition of partition 17 USS section (non initialized data) */
#define PARTITION_17_SECTION_USS     __attribute__ ((section (".partition17Uss")))

/* definition of partition 18 USS section (non initialized data) */
#define PARTITION_18_SECTION_USS     __attribute__ ((section (".partition18Uss")))

/* definition of partition 19 USS section (non initialized data) */
#define PARTITION_19_SECTION_USS     __attribute__ ((section (".partition19Uss")))

/* definition of partition 20 USS section (non initialized data) */
#define PARTITION_20_SECTION_USS     __attribute__ ((section (".partition20Uss")))

/* definition of partition 21 USS section (non initialized data) */
#define PARTITION_21_SECTION_USS     __attribute__ ((section (".partition21Uss")))

/* definition of partition 22 USS section (non initialized data) */
#define PARTITION_22_SECTION_USS     __attribute__ ((section (".partition22Uss")))

/* definition of partition 23 USS section (non initialized data) */
#define PARTITION_23_SECTION_USS     __attribute__ ((section (".partition23Uss")))

/* definition of partition 24 USS section (non initialized data) */
#define PARTITION_24_SECTION_USS     __attribute__ ((section (".partition24Uss")))

/* definition of partition 25 USS section (non initialized data) */
#define PARTITION_25_SECTION_USS     __attribute__ ((section (".partition25Uss")))

/* definition of partition 26 USS section (non initialized data) */
#define PARTITION_26_SECTION_USS     __attribute__ ((section (".partition26Uss")))

/* definition of partition 27 USS section (non initialized data) */
#define PARTITION_27_SECTION_USS     __attribute__ ((section (".partition27Uss")))

/* definition of partition 28 USS section (non initialized data) */
#define PARTITION_28_SECTION_USS     __attribute__ ((section (".partition28Uss")))

/* definition of partition 29 USS section (non initialized data) */
#define PARTITION_29_SECTION_USS     __attribute__ ((section (".partition29Uss")))

/* definition of partition 30 USS section (non initialized data) */
#define PARTITION_30_SECTION_USS     __attribute__ ((section (".partition30Uss")))

/* definition of partition 31 USS section (non initialized data) */
#define PARTITION_31_SECTION_USS     __attribute__ ((section (".partition31Uss")))

/* definition of partition 32 USS section (non initialized data) */
#define PARTITION_32_SECTION_USS     __attribute__ ((section (".partition32Uss")))

/* definition of partition 33 USS section (non initialized data) */
#define PARTITION_33_SECTION_USS     __attribute__ ((section (".partition33Uss")))

/* definition of partition 34 USS section (non initialized data) */
#define PARTITION_34_SECTION_USS     __attribute__ ((section (".partition34Uss")))

/* definition of partition 35 USS section (non initialized data) */
#define PARTITION_35_SECTION_USS     __attribute__ ((section (".partition35Uss")))

/* definition of partition 36 USS section (non initialized data) */
#define PARTITION_36_SECTION_USS     __attribute__ ((section (".partition36Uss")))

/* definition of partition 37 USS section (non initialized data) */
#define PARTITION_37_SECTION_USS     __attribute__ ((section (".partition37Uss")))

/* definition of partition 38 USS section (non initialized data) */
#define PARTITION_38_SECTION_USS     __attribute__ ((section (".partition38Uss")))

/* definition of partition 39 USS section (non initialized data) */
#define PARTITION_39_SECTION_USS     __attribute__ ((section (".partition39Uss")))

/* definition of partition 40 USS section (non initialized data) */
#define PARTITION_40_SECTION_USS     __attribute__ ((section (".partition40Uss")))

/* definition of partition 41 USS section (non initialized data) */
#define PARTITION_41_SECTION_USS     __attribute__ ((section (".partition41Uss")))

/* definition of partition 42 USS section (non initialized data) */
#define PARTITION_42_SECTION_USS     __attribute__ ((section (".partition42Uss")))

/* definition of partition 43 USS section (non initialized data) */
#define PARTITION_43_SECTION_USS     __attribute__ ((section (".partition43Uss")))

/* definition of partition 44 USS section (non initialized data) */
#define PARTITION_44_SECTION_USS     __attribute__ ((section (".partition44Uss")))

/* definition of partition 45 USS section (non initialized data) */
#define PARTITION_45_SECTION_USS     __attribute__ ((section (".partition45Uss")))

/* definition of partition 46 USS section (non initialized data) */
#define PARTITION_46_SECTION_USS     __attribute__ ((section (".partition46Uss")))

/* definition of partition 47 USS section (non initialized data) */
#define PARTITION_47_SECTION_USS     __attribute__ ((section (".partition47Uss")))

/* definition of partition 48 USS section (non initialized data) */
#define PARTITION_48_SECTION_USS     __attribute__ ((section (".partition48Uss")))

/* definition of partition 49 USS section (non initialized data) */
#define PARTITION_49_SECTION_USS     __attribute__ ((section (".partition49Uss")))

/* definition of partition 50 USS section (non initialized data) */
#define PARTITION_50_SECTION_USS     __attribute__ ((section (".partition50Uss")))

/* definition of partition 51 USS section (non initialized data) */
#define PARTITION_51_SECTION_USS     __attribute__ ((section (".partition51Uss")))

/* definition of partition 52 USS section (non initialized data) */
#define PARTITION_52_SECTION_USS     __attribute__ ((section (".partition52Uss")))

/* definition of partition 53 USS section (non initialized data) */
#define PARTITION_53_SECTION_USS     __attribute__ ((section (".partition53Uss")))

/* definition of partition 54 USS section (non initialized data) */
#define PARTITION_54_SECTION_USS     __attribute__ ((section (".partition54Uss")))

/* definition of partition 55 USS section (non initialized data) */
#define PARTITION_55_SECTION_USS     __attribute__ ((section (".partition55Uss")))

/* definition of partition 56 USS section (non initialized data) */
#define PARTITION_56_SECTION_USS     __attribute__ ((section (".partition56Uss")))

/* definition of partition 57 USS section (non initialized data) */
#define PARTITION_57_SECTION_USS     __attribute__ ((section (".partition57Uss")))

/* definition of partition 58 USS section (non initialized data) */
#define PARTITION_58_SECTION_USS     __attribute__ ((section (".partition58Uss")))

/* definition of partition 59 USS section (non initialized data) */
#define PARTITION_59_SECTION_USS     __attribute__ ((section (".partition59Uss")))

/* definition of partition 60 USS section (non initialized data) */
#define PARTITION_60_SECTION_USS     __attribute__ ((section (".partition60Uss")))

/* definition of partition 61 USS section (non initialized data) */
#define PARTITION_61_SECTION_USS     __attribute__ ((section (".partition61Uss")))

/* definition of partition 62 USS section (non initialized data) */
#define PARTITION_62_SECTION_USS     __attribute__ ((section (".partition62Uss")))

/* definition of partition 63 USS section (non initialized data) */
#define PARTITION_63_SECTION_USS     __attribute__ ((section (".partition63Uss")))



#define SHARED_STACK_SECTION        __attribute__ ((section (".pharosSharedStack"))) 


/* PHAROS_SECTIONS_H */
#endif	
