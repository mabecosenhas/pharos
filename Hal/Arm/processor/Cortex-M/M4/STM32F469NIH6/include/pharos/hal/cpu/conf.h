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


#ifndef PHAROS_CPU_CONF_H
#define	PHAROS_CPU_CONF_H

#include <pharos/hal/v7m/conf.h>


/**
 * CPU only has one core
 */
#define PHAROS_NUMBER_CORES                     (1U)

/**
 * CPU has 6 different memory areas (CCM, SRAM1, SRAM2, SRAM3, SRAM backup and SDRAM)
 */
#define PHAROS_NUMBER_MEMORY_AREAS              (6U)

/**
 * Cache line size
 */
#define PHAROS_CPU_CACHE_LINE_SIZE              (64U)

/**
 * CPU clock frequency (once the PLL is started) is 180 MHz
 */
#define PHAROS_CPU_FREQ                         (180U * VALUE_1M)

/**
 * AHB clock frequency
 */
#define PHAROS_CPU_AHB_FREQ_MHZ                 (180U)

/**
 * APB1 clock frequency
 */
#define PHAROS_CPU_APB1_FREQ_MHZ                (PHAROS_CPU_AHB_FREQ_MHZ / 4U)

/**
 * APB2 clock frequency
 */
#define PHAROS_CPU_APB2_FREQ_MHZ                (PHAROS_CPU_AHB_FREQ_MHZ / 2U)

/**
 * Maximum number of microseconds of systick without overloading the systick register (only has 24 bits)
 * 2^24 / 180 = 93206,7 -> the maximum "standard" multiple is 10 ms
 */
#define PHAROS_CPU_SYSTICK_MAX                  (VALUE_10K)

/**
 * Number of MPU regions supported by the CPU
 */
#define PHAROS_CPU_MPU_NUMBER_REGIONS           (8U)

/**
 * The STM32F469NIH6 is used always with FPU
 */
#define PHAROS_CPU_HAS_VFP

/**
 * Number of interrupts this CPU supports (does not account for exceptions)
 */
#define PHAROS_CPU_NUMBER_INTERRUPTS            (93U)

/**
 * Lowest priority on this CPU
 */
#define PHAROS_INTERRUPT_PRIORITY_LOWEST        (0xfU)

/**
 * Number of bits that define an interrupt priority
 */
#define PHAROS_CPU_INTERRUPT_PRIO_BITS          (0x4U)

/**
 * Priority bits that should be maintained
 */
#define PHAROS_CPU_INTERRUPT_PRIO_MASK          (0x0FU)


/************************************************************
 * CPU registers addresses                                  *
 ************************************************************/

/**
 * Address for FMC registers
 */
#define PHAROS_CPU_FMC_ADDRESS                  (0xA0000000U)

/**
 * Address for QUADSPI registers
 */
#define PHAROS_CPU_QUADSPI_ADDRESS              (0xA0001000U)

/**
 * Address for RNG registers
 */
#define PHAROS_CPU_RNG_ADDRESS                  (0x50060800U)

/**
 * Address for HASH registers
 */
#define PHAROS_CPU_HASH_ADDRESS                 (0x50060400U)

/**
 * Address for cRYP registers
 */
#define PHAROS_CPU_CRYP_ADDRESS                 (0x50060000U)

/**
 * Address for DCMI registers
 */
#define PHAROS_CPU_DCMI_ADDRESS                 (0x50050000U)

/**
 * Address for USB OTG FS registers
 */
#define PHAROS_CPU_USB_OTG_FS_ADDRESS           (0x50000000U)

/**
 * Address for USB OTG HS registers
 */
#define PHAROS_CPU_USB_OTG_HS_ADDRESS           (0x40040000U)

/**
 * Address for DMA2D registers
 */
#define PHAROS_CPU_DMA2D_ADDRESS                (0x4002B000U)

/**
 * Address for Ethernet registers
 */
#define PHAROS_CPU_ETHERNET_ADDRESS             (0x40028000U)

/**
 * Address for DMA2 registers
 */
#define PHAROS_CPU_DMA2_ADDRESS                 (0x40026400U)

/**
 * Address for DMA1 registers
 */
#define PHAROS_CPU_DMA1_ADDRESS                 (0x40026000U)

/**
 * Address for BKPSRAM registers
 */
#define PHAROS_CPU_BKPSRAM_ADDRESS              (0x40024000U)

/**
 * Address for FLASH registers
 */
#define PHAROS_CPU_FLASH_ADDRESS                (0x40023C00U)

/**
 * Address for RCC registers
 */
#define PHAROS_CPU_RCC_ADDRESS                  (0x40023800U)

/**
 * Address for CRC registers
 */
#define PHAROS_CPU_CRC_ADDRESS                  (0x40023000U)

/**
 * Address for GPIOK registers
 */
#define PHAROS_CPU_GPIOK_ADDRESS                (0x40022800U)

/**
 * Address for GPIOJ registers
 */
#define PHAROS_CPU_GPIOJ_ADDRESS                (0x40022400U)

/**
 * Address for GPIOI registers
 */
#define PHAROS_CPU_GPIOI_ADDRESS                (0x40022000U)

/**
 * Address for GPIOH registers
 */
#define PHAROS_CPU_GPIOH_ADDRESS                (0x40021C00U)

/**
 * Address for GPIOG registers
 */
#define PHAROS_CPU_GPIOG_ADDRESS                (0x40021800U)

/**
 * Address for GPIOF registers
 */
#define PHAROS_CPU_GPIOF_ADDRESS                (0x40021400U)
                                                   
/**
 * Address for GPIOE registers
 */
#define PHAROS_CPU_GPIOE_ADDRESS                (0x40021000U)

/**
 * Address for GPIOD registers
 */
#define PHAROS_CPU_GPIOD_ADDRESS                (0x40020C00U)

/**
 * Address for GPIOC registers
 */
#define PHAROS_CPU_GPIOC_ADDRESS                (0x40020800U)

/**
 * Address for GPIOB registers
 */
#define PHAROS_CPU_GPIOB_ADDRESS                (0x40020400U)
                                                   
/**
 * Address for GPIOA registers
 */
#define PHAROS_CPU_GPIOA_ADDRESS                (0x40020000U)

/**
 * Address for DSI Host registers
 */
#define PHAROS_CPU_DSI_HOST_ADDRESS             (0x40016c00U)

/**
 * Address for LCD TFT registers
 */
#define PHAROS_CPU_LCD_TFT_ADDRESS              (0x40016800U)

/**
 * Address for SAI1 registers
 */
#define PHAROS_CPU_SAI1_ADDRESS                 (0x40015800U)

/**
 * Address for SPI6 registers
 */
#define PHAROS_CPU_SPI6_ADDRESS                 (0x40015400U)

/**
 * Address for SPI5 registers
 */
#define PHAROS_CPU_SPI5_ADDRESS                 (0x40015000U)

/**
 * Address for  registers
 */
#define PHAROS_CPU_TIM11_ADDRESS                (0x40014800U)

/**
 * Address for Timer 10 registers
 */
#define PHAROS_CPU_TIM10_ADDRESS                (0x40014400U)

/**
 * Address for Timer 9 registers
 */
#define PHAROS_CPU_TIM9_ADDRESS                 (0x40014000U)

/**
 * Address for EXTI registers
 */
#define PHAROS_CPU_EXTI_ADDRESS                 (0x40013c00U)

/**
 * Address for SYSCFG registers
 */
#define PHAROS_CPU_SYSCFG_ADDRESS               (0x40013800U)

/**
 * Address for SPI4 registers
 */
#define PHAROS_CPU_SPI4_ADDRESS                 (0x40013400U)

/**
 * Address for SPI1 registers
 */
#define PHAROS_CPU_SPI1_ADDRESS                 (0x40013000U)

/**
 * Address for SDIO registers
 */
#define PHAROS_CPU_SDIO_ADDRESS                 (0x40012c00U)

/**
 * Address for ADC1 and ADC2 and ADC3 registers
 */
#define PHAROS_CPU_ADC1_ADC2_ADC3_ADDRESS       (0x40012000U)

/**
 * Address for USART6 registers
 */
#define PHAROS_CPU_USART6_ADDRESS               (0x40011400U)

/**
 * Address for USART1 registers
 */
#define PHAROS_CPU_USART1_ADDRESS               (0x40011000U)

/**
 * Address for Timer 8 registers
 */
#define PHAROS_CPU_TIM8_ADDRESS                 (0x40010400U)

/**
 * Address for Timer 1 registers
 */
#define PHAROS_CPU_TIM1_ADDRESS                 (0x40010000U)

/**
 * Address for UART 8 registers
 */
#define PHAROS_CPU_UART8_ADDRESS                (0x40007c00U)

/**
 * Address for UART 7 registers
 */
#define PHAROS_CPU_UART7_ADDRESS                (0x40007800U)

/**
 * Address for DAC registers
 */
#define PHAROS_CPU_DAC_ADDRESS                  (0x40007400U)

/**
 * Address for PWR registers
 */
#define PHAROS_CPU_PWR_ADDRESS                  (0x40007000U)

/**
 * Address for CAN2 registers
 */
#define PHAROS_CPU_CAN2_ADDRESS                 (0x40006800U)

/**
 * Address for CAN1 registers
 */
#define PHAROS_CPU_CAN1_ADDRESS                 (0x40006400U)

/**
 * Address for I2C3 registers
 */
#define PHAROS_CPU_I2C3_ADDRESS                 (0x40005c00U)

/**
 * Address for I2C2 registers
 */
#define PHAROS_CPU_I2C2_ADDRESS                 (0x40005800U)

/**
 * Address for I2C1 registers
 */
#define PHAROS_CPU_I2C1_ADDRESS                 (0x40005400U)

/**
 * Address for UART 5 registers
 */
#define PHAROS_CPU_UART5_ADDRESS                (0x40005000U)

/**
 * Address for UART 4 registers
 */
#define PHAROS_CPU_UART4_ADDRESS                (0x40004c00U)

/**
 * Address for USART 3 registers
 */
#define PHAROS_CPU_USART3_ADDRESS               (0x40004800U)

/**
 * Address for USART 2 registers
 */
#define PHAROS_CPU_USART2_ADDRESS               (0x40004400U)

/**
 * Address for I2S3 registers
 */
#define PHAROS_CPU_I2S3_EXT_ADDRESS             (0x40004000U)

/**
 * Address for SPI3 and I2S3 registers
 */
#define PHAROS_CPU_SPI3_I2S3_ADDRESS            (0x40003c00U)

/**
 * Address for SPI2 and I2S2 registers
 */
#define PHAROS_CPU_SPI2_I2S2_ADDRESS            (0x40003800U)

/**
 * Address for I2S2 registers
 */
#define PHAROS_CPU_I2S2_EXT_ADDRESS             (0x40003400U)

/**
 * Address for IWDG registers
 */
#define PHAROS_CPU_IWDG_ADDRESS                 (0x40003000U)

/**
 * Address for WWDG registers
 */
#define PHAROS_CPU_WWDG_ADDRESS                 (0x40002c00U)

/**
 * Address for RTC Backup registers
 */
#define PHAROS_CPU_RTC_BKP_ADDRESS              (0x40002800U)

/**
 * Address for Timer 14 registers
 */
#define PHAROS_CPU_TIM14_ADDRESS                (0x40002000U)

/**
 * Address for Timer 13 registers
 */
#define PHAROS_CPU_TIM13_ADDRESS                (0x40001c00U)

/**
 * Address for Timer 12 registers
 */
#define PHAROS_CPU_TIM12_ADDRESS                (0x40001800U)

/**
 * Address for Timer 7 registers
 */
#define PHAROS_CPU_TIM7_ADDRESS                 (0x40001400U)

/**
 * Address for Timer 6 registers
 */
#define PHAROS_CPU_TIM6_ADDRESS                 (0x40001000U)

/**
 * Address for Timer 5 registers
 */
#define PHAROS_CPU_TIM5_ADDRESS                 (0x40000c00U)

/**
 * Address for Timer 4 registers
 */
#define PHAROS_CPU_TIM4_ADDRESS                 (0x40000800U)

/**
 * Address for Timer 3 registers
 */
#define PHAROS_CPU_TIM3_ADDRESS                 (0x40000400U)

/**
 * Address for Timer 2 registers
 */
#define PHAROS_CPU_TIM2_ADDRESS                 (0x40000000U)


/**
 * Kernel sections for different memory areas
 */
#define KERNEL_SECTION_CCM                      __attribute__ ((section (".kernelDataCcm"))) 
#define KERNEL_SECTION_CCM_BSS                  __attribute__ ((section (".kernelBssCcm"))) 
#define KERNEL_SECTION_CCM_USS                  __attribute__ ((section (".kernelUssCcm"))) 
#define KERNEL_SECTION_SRAM1                    __attribute__ ((section (".kernelDataSram1"))) 
#define KERNEL_SECTION_SRAM1_BSS                __attribute__ ((section (".kernelBssSram1"))) 
#define KERNEL_SECTION_SRAM1_USS                __attribute__ ((section (".kernelUssSram1"))) 
#define KERNEL_SECTION_SRAM2                    __attribute__ ((section (".kernelDataSram2"))) 
#define KERNEL_SECTION_SRAM2_BSS                __attribute__ ((section (".kernelBssSram2"))) 
#define KERNEL_SECTION_SRAM2_USS                __attribute__ ((section (".kernelUssSram2"))) 
#define KERNEL_SECTION_SRAM3                    __attribute__ ((section (".kernelDataSram3"))) 
#define KERNEL_SECTION_SRAM3_BSS                __attribute__ ((section (".kernelBssSram3"))) 
#define KERNEL_SECTION_SRAM3_USS                __attribute__ ((section (".kernelUssSram3"))) 
#define KERNEL_SECTION_SRAM_BCK                 __attribute__ ((section (".kernelDataSramBackup"))) 
#define KERNEL_SECTION_SRAM_BCK_BSS             __attribute__ ((section (".kernelBssSramBackup"))) 
#define KERNEL_SECTION_SRAM_BCK_USS             __attribute__ ((section (".kernelUssSramBackup"))) 
#define KERNEL_SECTION_SDRAM                    __attribute__ ((section (".kernelDataSdram"))) 
#define KERNEL_SECTION_SDRAM_BSS                __attribute__ ((section (".kernelBssSdram"))) 
#define KERNEL_SECTION_SDRAM_USS                __attribute__ ((section (".kernelUssSdram"))) 

/**
 * Bulk section for different memory areas
 */
#define BULK_SECTION_CCM                        __attribute__ ((section (".kernelBulksCcm"))) 
#define BULK_SECTION_SRAM1                      __attribute__ ((section (".kernelBulksSram1"))) 
#define BULK_SECTION_SRAM2                      __attribute__ ((section (".kernelBulksSram2"))) 
#define BULK_SECTION_SRAM3                      __attribute__ ((section (".kernelBulksSram3"))) 
#define BULK_SECTION_SRAM_BCK                   __attribute__ ((section (".kernelBulksSramBackup"))) 
#define BULK_SECTION_SDRAM                      __attribute__ ((section (".kernelBulksSdram"))) 

/**
 * Shared sections for different memory areas
 */
#define SHARED_SECTION_CCM                      __attribute__ ((section (".sharedDataCcm"))) 
#define SHARED_SECTION_CCM_BSS                  __attribute__ ((section (".sharedBssCcm"))) 
#define SHARED_SECTION_CCM_USS                  __attribute__ ((section (".sharedUssCcm"))) 
#define SHARED_SECTION_SRAM1                    __attribute__ ((section (".sharedDataSram1"))) 
#define SHARED_SECTION_SRAM1_BSS                __attribute__ ((section (".sharedBssSram1"))) 
#define SHARED_SECTION_SRAM1_USS                __attribute__ ((section (".sharedUssSram1"))) 
#define SHARED_SECTION_SRAM2                    __attribute__ ((section (".sharedDataSram2"))) 
#define SHARED_SECTION_SRAM2_BSS                __attribute__ ((section (".sharedBssSram2"))) 
#define SHARED_SECTION_SRAM2_USS                __attribute__ ((section (".sharedUssSram2"))) 
#define SHARED_SECTION_SRAM3                    __attribute__ ((section (".sharedDataSram3"))) 
#define SHARED_SECTION_SRAM3_BSS                __attribute__ ((section (".sharedBssSram3"))) 
#define SHARED_SECTION_SRAM3_USS                __attribute__ ((section (".sharedUssSram3"))) 
#define SHARED_SECTION_SRAM_BCK                 __attribute__ ((section (".sharedDataSramBackup"))) 
#define SHARED_SECTION_SRAM_BCK_BSS             __attribute__ ((section (".sharedBssSramBackup"))) 
#define SHARED_SECTION_SRAM_BCK_USS             __attribute__ ((section (".sharedUssSramBackup"))) 
#define SHARED_SECTION_SDRAM                    __attribute__ ((section (".sharedDataSdram"))) 
#define SHARED_SECTION_SDRAM_BSS                __attribute__ ((section (".sharedBssSdram"))) 
#define SHARED_SECTION_SDRAM_USS                __attribute__ ((section (".sharedUssSdram"))) 


/**
 * Shared sections for different memory areas
 */
#define SHARED_STACK_SECTION_CCM                __attribute__ ((section (".sharedStackCcm"))) 
#define SHARED_STACK_SECTION_SRAM1              __attribute__ ((section (".sharedStackSram1"))) 
#define SHARED_STACK_SECTION_SRAM2              __attribute__ ((section (".sharedStackSram2"))) 
#define SHARED_STACK_SECTION_SRAM3              __attribute__ ((section (".sharedStackSram3"))) 
#define SHARED_STACK_SECTION_SRAM_BCK           __attribute__ ((section (".sharedStackSramBackup"))) 
#define SHARED_STACK_SECTION_SDRAM              __attribute__ ((section (".sharedStackSdram"))) 


/**
 * Default Inter-partition calls section (text area)
 */
#define IP_CALL_SECTION                         __attribute__ ((section (".ipCall"))) 


/**
 * Partitions section for different memory areas
 */
#define PARTITION_0_SECTION_CCM                 __attribute__((section(".partition0DataCcm"))) 
#define PARTITION_0_SECTION_CCM_BSS             __attribute__((section(".partition0BssCcm"))) 
#define PARTITION_0_SECTION_CCM_USS             __attribute__((section(".partition0UssCcm"))) 
#define PARTITION_0_SECTION_SRAM1               __attribute__((section(".partition0DataSram1"))) 
#define PARTITION_0_SECTION_SRAM1_BSS           __attribute__((section(".partition0BssSram1"))) 
#define PARTITION_0_SECTION_SRAM1_USS           __attribute__((section(".partition0UssSram1"))) 
#define PARTITION_0_SECTION_SRAM2               __attribute__((section(".partition0DataSram2"))) 
#define PARTITION_0_SECTION_SRAM2_BSS           __attribute__((section(".partition0BssSram2"))) 
#define PARTITION_0_SECTION_SRAM2_USS           __attribute__((section(".partition0UssSram2"))) 
#define PARTITION_0_SECTION_SRAM3               __attribute__((section(".partition0DataSram3"))) 
#define PARTITION_0_SECTION_SRAM3_BSS           __attribute__((section(".partition0BssSram3"))) 
#define PARTITION_0_SECTION_SRAM3_USS           __attribute__((section(".partition0UssSram3"))) 
#define PARTITION_0_SECTION_SRAM_BCK            __attribute__((section(".partition0DataSramBackup"))) 
#define PARTITION_0_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition0BssSramBackup"))) 
#define PARTITION_0_SECTION_SRAM_BCK_USS        __attribute__((section(".partition0UssSramBackup"))) 
#define PARTITION_0_SECTION_SDRAM               __attribute__((section(".partition0DataSdram"))) 
#define PARTITION_0_SECTION_SDRAM_BSS           __attribute__((section(".partition0BssSdram"))) 
#define PARTITION_0_SECTION_SDRAM_USS           __attribute__((section(".partition0UssSdram"))) 


#define PARTITION_1_SECTION_CCM                 __attribute__((section(".partition1DataCcm"))) 
#define PARTITION_1_SECTION_CCM_BSS             __attribute__((section(".partition1BssCcm"))) 
#define PARTITION_1_SECTION_CCM_USS             __attribute__((section(".partition1UssCcm"))) 
#define PARTITION_1_SECTION_SRAM1               __attribute__((section(".partition1DataSram1"))) 
#define PARTITION_1_SECTION_SRAM1_BSS           __attribute__((section(".partition1BssSram1"))) 
#define PARTITION_1_SECTION_SRAM1_USS           __attribute__((section(".partition1UssSram1"))) 
#define PARTITION_1_SECTION_SRAM2               __attribute__((section(".partition1DataSram2"))) 
#define PARTITION_1_SECTION_SRAM2_BSS           __attribute__((section(".partition1BssSram2"))) 
#define PARTITION_1_SECTION_SRAM2_USS           __attribute__((section(".partition1UssSram2"))) 
#define PARTITION_1_SECTION_SRAM3               __attribute__((section(".partition1DataSram3"))) 
#define PARTITION_1_SECTION_SRAM3_BSS           __attribute__((section(".partition1BssSram3"))) 
#define PARTITION_1_SECTION_SRAM3_USS           __attribute__((section(".partition1UssSram3"))) 
#define PARTITION_1_SECTION_SRAM_BCK            __attribute__((section(".partition1DataSramBackup"))) 
#define PARTITION_1_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition1BssSramBackup"))) 
#define PARTITION_1_SECTION_SRAM_BCK_USS        __attribute__((section(".partition1UssSramBackup"))) 
#define PARTITION_1_SECTION_SDRAM               __attribute__((section(".partition1DataSdram"))) 
#define PARTITION_1_SECTION_SDRAM_BSS           __attribute__((section(".partition1BssSdram"))) 
#define PARTITION_1_SECTION_SDRAM_USS           __attribute__((section(".partition1UssSdram"))) 


#define PARTITION_2_SECTION_CCM                 __attribute__((section(".partition2DataCcm"))) 
#define PARTITION_2_SECTION_CCM_BSS             __attribute__((section(".partition2BssCcm"))) 
#define PARTITION_2_SECTION_CCM_USS             __attribute__((section(".partition2UssCcm"))) 
#define PARTITION_2_SECTION_SRAM1               __attribute__((section(".partition2DataSram1"))) 
#define PARTITION_2_SECTION_SRAM1_BSS           __attribute__((section(".partition2BssSram1"))) 
#define PARTITION_2_SECTION_SRAM1_USS           __attribute__((section(".partition2UssSram1"))) 
#define PARTITION_2_SECTION_SRAM2               __attribute__((section(".partition2DataSram2"))) 
#define PARTITION_2_SECTION_SRAM2_BSS           __attribute__((section(".partition2BssSram2"))) 
#define PARTITION_2_SECTION_SRAM2_USS           __attribute__((section(".partition2UssSram2"))) 
#define PARTITION_2_SECTION_SRAM3               __attribute__((section(".partition2DataSram3"))) 
#define PARTITION_2_SECTION_SRAM3_BSS           __attribute__((section(".partition2BssSram3"))) 
#define PARTITION_2_SECTION_SRAM3_USS           __attribute__((section(".partition2UssSram3"))) 
#define PARTITION_2_SECTION_SRAM_BCK            __attribute__((section(".partition2DataSramBackup"))) 
#define PARTITION_2_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition2BssSramBackup"))) 
#define PARTITION_2_SECTION_SRAM_BCK_USS        __attribute__((section(".partition2UssSramBackup"))) 
#define PARTITION_2_SECTION_SDRAM               __attribute__((section(".partition2DataSdram"))) 
#define PARTITION_2_SECTION_SDRAM_BSS           __attribute__((section(".partition2BssSdram"))) 
#define PARTITION_2_SECTION_SDRAM_USS           __attribute__((section(".partition2UssSdram"))) 


#define PARTITION_3_SECTION_CCM                 __attribute__((section(".partition3DataCcm"))) 
#define PARTITION_3_SECTION_CCM_BSS             __attribute__((section(".partition3BssCcm"))) 
#define PARTITION_3_SECTION_CCM_USS             __attribute__((section(".partition3UssCcm"))) 
#define PARTITION_3_SECTION_SRAM1               __attribute__((section(".partition3DataSram1"))) 
#define PARTITION_3_SECTION_SRAM1_BSS           __attribute__((section(".partition3BssSram1"))) 
#define PARTITION_3_SECTION_SRAM1_USS           __attribute__((section(".partition3UssSram1"))) 
#define PARTITION_3_SECTION_SRAM2               __attribute__((section(".partition3DataSram2"))) 
#define PARTITION_3_SECTION_SRAM2_BSS           __attribute__((section(".partition3BssSram2"))) 
#define PARTITION_3_SECTION_SRAM2_USS           __attribute__((section(".partition3UssSram2"))) 
#define PARTITION_3_SECTION_SRAM3               __attribute__((section(".partition3DataSram3"))) 
#define PARTITION_3_SECTION_SRAM3_BSS           __attribute__((section(".partition3BssSram3"))) 
#define PARTITION_3_SECTION_SRAM3_USS           __attribute__((section(".partition3UssSram3"))) 
#define PARTITION_3_SECTION_SRAM_BCK            __attribute__((section(".partition3DataSramBackup"))) 
#define PARTITION_3_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition3BssSramBackup"))) 
#define PARTITION_3_SECTION_SRAM_BCK_USS        __attribute__((section(".partition3UssSramBackup"))) 
#define PARTITION_3_SECTION_SDRAM               __attribute__((section(".partition3DataSdram"))) 
#define PARTITION_3_SECTION_SDRAM_BSS           __attribute__((section(".partition3BssSdram"))) 
#define PARTITION_3_SECTION_SDRAM_USS           __attribute__((section(".partition3UssSdram"))) 


#define PARTITION_4_SECTION_CCM                 __attribute__((section(".partition4DataCcm"))) 
#define PARTITION_4_SECTION_CCM_BSS             __attribute__((section(".partition4BssCcm"))) 
#define PARTITION_4_SECTION_CCM_USS             __attribute__((section(".partition4UssCcm"))) 
#define PARTITION_4_SECTION_SRAM1               __attribute__((section(".partition4DataSram1"))) 
#define PARTITION_4_SECTION_SRAM1_BSS           __attribute__((section(".partition4BssSram1"))) 
#define PARTITION_4_SECTION_SRAM1_USS           __attribute__((section(".partition4UssSram1"))) 
#define PARTITION_4_SECTION_SRAM2               __attribute__((section(".partition4DataSram2"))) 
#define PARTITION_4_SECTION_SRAM2_BSS           __attribute__((section(".partition4BssSram2"))) 
#define PARTITION_4_SECTION_SRAM2_USS           __attribute__((section(".partition4UssSram2"))) 
#define PARTITION_4_SECTION_SRAM3               __attribute__((section(".partition4DataSram3"))) 
#define PARTITION_4_SECTION_SRAM3_BSS           __attribute__((section(".partition4BssSram3"))) 
#define PARTITION_4_SECTION_SRAM3_USS           __attribute__((section(".partition4UssSram3"))) 
#define PARTITION_4_SECTION_SRAM_BCK            __attribute__((section(".partition4DataSramBackup"))) 
#define PARTITION_4_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition4BssSramBackup"))) 
#define PARTITION_4_SECTION_SRAM_BCK_USS        __attribute__((section(".partition4UssSramBackup"))) 
#define PARTITION_4_SECTION_SDRAM               __attribute__((section(".partition4DataSdram"))) 
#define PARTITION_4_SECTION_SDRAM_BSS           __attribute__((section(".partition4BssSdram"))) 
#define PARTITION_4_SECTION_SDRAM_USS           __attribute__((section(".partition4UssSdram"))) 


#define PARTITION_5_SECTION_CCM                 __attribute__((section(".partition5DataCcm"))) 
#define PARTITION_5_SECTION_CCM_BSS             __attribute__((section(".partition5BssCcm"))) 
#define PARTITION_5_SECTION_CCM_USS             __attribute__((section(".partition5UssCcm"))) 
#define PARTITION_5_SECTION_SRAM1               __attribute__((section(".partition5DataSram1"))) 
#define PARTITION_5_SECTION_SRAM1_BSS           __attribute__((section(".partition5BssSram1"))) 
#define PARTITION_5_SECTION_SRAM1_USS           __attribute__((section(".partition5UssSram1"))) 
#define PARTITION_5_SECTION_SRAM2               __attribute__((section(".partition5DataSram2"))) 
#define PARTITION_5_SECTION_SRAM2_BSS           __attribute__((section(".partition5BssSram2"))) 
#define PARTITION_5_SECTION_SRAM2_USS           __attribute__((section(".partition5UssSram2"))) 
#define PARTITION_5_SECTION_SRAM3               __attribute__((section(".partition5DataSram3"))) 
#define PARTITION_5_SECTION_SRAM3_BSS           __attribute__((section(".partition5BssSram3"))) 
#define PARTITION_5_SECTION_SRAM3_USS           __attribute__((section(".partition5UssSram3"))) 
#define PARTITION_5_SECTION_SRAM_BCK            __attribute__((section(".partition5DataSramBackup"))) 
#define PARTITION_5_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition5BssSramBackup"))) 
#define PARTITION_5_SECTION_SRAM_BCK_USS        __attribute__((section(".partition5UssSramBackup"))) 
#define PARTITION_5_SECTION_SDRAM               __attribute__((section(".partition5DataSdram"))) 
#define PARTITION_5_SECTION_SDRAM_BSS           __attribute__((section(".partition5BssSdram"))) 
#define PARTITION_5_SECTION_SDRAM_USS           __attribute__((section(".partition5UssSdram"))) 


#define PARTITION_6_SECTION_CCM                 __attribute__((section(".partition6DataCcm"))) 
#define PARTITION_6_SECTION_CCM_BSS             __attribute__((section(".partition6BssCcm"))) 
#define PARTITION_6_SECTION_CCM_USS             __attribute__((section(".partition6UssCcm"))) 
#define PARTITION_6_SECTION_SRAM1               __attribute__((section(".partition6DataSram1"))) 
#define PARTITION_6_SECTION_SRAM1_BSS           __attribute__((section(".partition6BssSram1"))) 
#define PARTITION_6_SECTION_SRAM1_USS           __attribute__((section(".partition6UssSram1"))) 
#define PARTITION_6_SECTION_SRAM2               __attribute__((section(".partition6DataSram2"))) 
#define PARTITION_6_SECTION_SRAM2_BSS           __attribute__((section(".partition6BssSram2"))) 
#define PARTITION_6_SECTION_SRAM2_USS           __attribute__((section(".partition6UssSram2"))) 
#define PARTITION_6_SECTION_SRAM3               __attribute__((section(".partition6DataSram3"))) 
#define PARTITION_6_SECTION_SRAM3_BSS           __attribute__((section(".partition6BssSram3"))) 
#define PARTITION_6_SECTION_SRAM3_USS           __attribute__((section(".partition6UssSram3"))) 
#define PARTITION_6_SECTION_SRAM_BCK            __attribute__((section(".partition6DataSramBackup"))) 
#define PARTITION_6_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition6BssSramBackup"))) 
#define PARTITION_6_SECTION_SRAM_BCK_USS        __attribute__((section(".partition6UssSramBackup"))) 
#define PARTITION_6_SECTION_SDRAM               __attribute__((section(".partition6DataSdram"))) 
#define PARTITION_6_SECTION_SDRAM_BSS           __attribute__((section(".partition6BssSdram"))) 
#define PARTITION_6_SECTION_SDRAM_USS           __attribute__((section(".partition6UssSdram"))) 


#define PARTITION_7_SECTION_CCM                 __attribute__((section(".partition7DataCcm"))) 
#define PARTITION_7_SECTION_CCM_BSS             __attribute__((section(".partition7BssCcm"))) 
#define PARTITION_7_SECTION_CCM_USS             __attribute__((section(".partition7UssCcm"))) 
#define PARTITION_7_SECTION_SRAM1               __attribute__((section(".partition7DataSram1"))) 
#define PARTITION_7_SECTION_SRAM1_BSS           __attribute__((section(".partition7BssSram1"))) 
#define PARTITION_7_SECTION_SRAM1_USS           __attribute__((section(".partition7UssSram1"))) 
#define PARTITION_7_SECTION_SRAM2               __attribute__((section(".partition7DataSram2"))) 
#define PARTITION_7_SECTION_SRAM2_BSS           __attribute__((section(".partition7BssSram2"))) 
#define PARTITION_7_SECTION_SRAM2_USS           __attribute__((section(".partition7UssSram2"))) 
#define PARTITION_7_SECTION_SRAM3               __attribute__((section(".partition7DataSram3"))) 
#define PARTITION_7_SECTION_SRAM3_BSS           __attribute__((section(".partition7BssSram3"))) 
#define PARTITION_7_SECTION_SRAM3_USS           __attribute__((section(".partition7UssSram3"))) 
#define PARTITION_7_SECTION_SRAM_BCK            __attribute__((section(".partition7DataSramBackup"))) 
#define PARTITION_7_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition7BssSramBackup"))) 
#define PARTITION_7_SECTION_SRAM_BCK_USS        __attribute__((section(".partition7UssSramBackup"))) 
#define PARTITION_7_SECTION_SDRAM               __attribute__((section(".partition7DataSdram"))) 
#define PARTITION_7_SECTION_SDRAM_BSS           __attribute__((section(".partition7BssSdram"))) 
#define PARTITION_7_SECTION_SDRAM_USS           __attribute__((section(".partition7UssSdram"))) 


#define PARTITION_8_SECTION_CCM                 __attribute__((section(".partition8DataCcm"))) 
#define PARTITION_8_SECTION_CCM_BSS             __attribute__((section(".partition8BssCcm"))) 
#define PARTITION_8_SECTION_CCM_USS             __attribute__((section(".partition8UssCcm"))) 
#define PARTITION_8_SECTION_SRAM1               __attribute__((section(".partition8DataSram1"))) 
#define PARTITION_8_SECTION_SRAM1_BSS           __attribute__((section(".partition8BssSram1"))) 
#define PARTITION_8_SECTION_SRAM1_USS           __attribute__((section(".partition8UssSram1"))) 
#define PARTITION_8_SECTION_SRAM2               __attribute__((section(".partition8DataSram2"))) 
#define PARTITION_8_SECTION_SRAM2_BSS           __attribute__((section(".partition8BssSram2"))) 
#define PARTITION_8_SECTION_SRAM2_USS           __attribute__((section(".partition8UssSram2"))) 
#define PARTITION_8_SECTION_SRAM3               __attribute__((section(".partition8DataSram3"))) 
#define PARTITION_8_SECTION_SRAM3_BSS           __attribute__((section(".partition8BssSram3"))) 
#define PARTITION_8_SECTION_SRAM3_USS           __attribute__((section(".partition8UssSram3"))) 
#define PARTITION_8_SECTION_SRAM_BCK            __attribute__((section(".partition8DataSramBackup"))) 
#define PARTITION_8_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition8BssSramBackup"))) 
#define PARTITION_8_SECTION_SRAM_BCK_USS        __attribute__((section(".partition8UssSramBackup"))) 
#define PARTITION_8_SECTION_SDRAM               __attribute__((section(".partition8DataSdram"))) 
#define PARTITION_8_SECTION_SDRAM_BSS           __attribute__((section(".partition8BssSdram"))) 
#define PARTITION_8_SECTION_SDRAM_USS           __attribute__((section(".partition8UssSdram"))) 


#define PARTITION_9_SECTION_CCM                 __attribute__((section(".partition9DataCcm"))) 
#define PARTITION_9_SECTION_CCM_BSS             __attribute__((section(".partition9BssCcm"))) 
#define PARTITION_9_SECTION_CCM_USS             __attribute__((section(".partition9UssCcm"))) 
#define PARTITION_9_SECTION_SRAM1               __attribute__((section(".partition9DataSram1"))) 
#define PARTITION_9_SECTION_SRAM1_BSS           __attribute__((section(".partition9BssSram1"))) 
#define PARTITION_9_SECTION_SRAM1_USS           __attribute__((section(".partition9UssSram1"))) 
#define PARTITION_9_SECTION_SRAM2               __attribute__((section(".partition9DataSram2"))) 
#define PARTITION_9_SECTION_SRAM2_BSS           __attribute__((section(".partition9BssSram2"))) 
#define PARTITION_9_SECTION_SRAM2_USS           __attribute__((section(".partition9UssSram2"))) 
#define PARTITION_9_SECTION_SRAM3               __attribute__((section(".partition9DataSram3"))) 
#define PARTITION_9_SECTION_SRAM3_BSS           __attribute__((section(".partition9BssSram3"))) 
#define PARTITION_9_SECTION_SRAM3_USS           __attribute__((section(".partition9UssSram3"))) 
#define PARTITION_9_SECTION_SRAM_BCK            __attribute__((section(".partition9DataSramBackup"))) 
#define PARTITION_9_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition9BssSramBackup"))) 
#define PARTITION_9_SECTION_SRAM_BCK_USS        __attribute__((section(".partition9UssSramBackup"))) 
#define PARTITION_9_SECTION_SDRAM               __attribute__((section(".partition9DataSdram"))) 
#define PARTITION_9_SECTION_SDRAM_BSS           __attribute__((section(".partition9BssSdram"))) 
#define PARTITION_9_SECTION_SDRAM_USS           __attribute__((section(".partition9UssSdram"))) 


#define PARTITION_10_SECTION_CCM                 __attribute__((section(".partition10DataCcm"))) 
#define PARTITION_10_SECTION_CCM_BSS             __attribute__((section(".partition10BssCcm"))) 
#define PARTITION_10_SECTION_CCM_USS             __attribute__((section(".partition10UssCcm"))) 
#define PARTITION_10_SECTION_SRAM1               __attribute__((section(".partition10DataSram1"))) 
#define PARTITION_10_SECTION_SRAM1_BSS           __attribute__((section(".partition10BssSram1"))) 
#define PARTITION_10_SECTION_SRAM1_USS           __attribute__((section(".partition10UssSram1"))) 
#define PARTITION_10_SECTION_SRAM2               __attribute__((section(".partition10DataSram2"))) 
#define PARTITION_10_SECTION_SRAM2_BSS           __attribute__((section(".partition10BssSram2"))) 
#define PARTITION_10_SECTION_SRAM2_USS           __attribute__((section(".partition10UssSram2"))) 
#define PARTITION_10_SECTION_SRAM3               __attribute__((section(".partition10DataSram3"))) 
#define PARTITION_10_SECTION_SRAM3_BSS           __attribute__((section(".partition10BssSram3"))) 
#define PARTITION_10_SECTION_SRAM3_USS           __attribute__((section(".partition10UssSram3"))) 
#define PARTITION_10_SECTION_SRAM_BCK            __attribute__((section(".partition10DataSramBackup"))) 
#define PARTITION_10_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition10BssSramBackup"))) 
#define PARTITION_10_SECTION_SRAM_BCK_USS        __attribute__((section(".partition10UssSramBackup"))) 
#define PARTITION_10_SECTION_SDRAM               __attribute__((section(".partition10DataSdram"))) 
#define PARTITION_10_SECTION_SDRAM_BSS           __attribute__((section(".partition10BssSdram"))) 
#define PARTITION_10_SECTION_SDRAM_USS           __attribute__((section(".partition10UssSdram"))) 


#define PARTITION_11_SECTION_CCM                 __attribute__((section(".partition11DataCcm"))) 
#define PARTITION_11_SECTION_CCM_BSS             __attribute__((section(".partition11BssCcm"))) 
#define PARTITION_11_SECTION_CCM_USS             __attribute__((section(".partition11UssCcm"))) 
#define PARTITION_11_SECTION_SRAM1               __attribute__((section(".partition11DataSram1"))) 
#define PARTITION_11_SECTION_SRAM1_BSS           __attribute__((section(".partition11BssSram1"))) 
#define PARTITION_11_SECTION_SRAM1_USS           __attribute__((section(".partition11UssSram1"))) 
#define PARTITION_11_SECTION_SRAM2               __attribute__((section(".partition11DataSram2"))) 
#define PARTITION_11_SECTION_SRAM2_BSS           __attribute__((section(".partition11BssSram2"))) 
#define PARTITION_11_SECTION_SRAM2_USS           __attribute__((section(".partition11UssSram2"))) 
#define PARTITION_11_SECTION_SRAM3               __attribute__((section(".partition11DataSram3"))) 
#define PARTITION_11_SECTION_SRAM3_BSS           __attribute__((section(".partition11BssSram3"))) 
#define PARTITION_11_SECTION_SRAM3_USS           __attribute__((section(".partition11UssSram3"))) 
#define PARTITION_11_SECTION_SRAM_BCK            __attribute__((section(".partition11DataSramBackup"))) 
#define PARTITION_11_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition11BssSramBackup"))) 
#define PARTITION_11_SECTION_SRAM_BCK_USS        __attribute__((section(".partition11UssSramBackup"))) 
#define PARTITION_11_SECTION_SDRAM               __attribute__((section(".partition11DataSdram"))) 
#define PARTITION_11_SECTION_SDRAM_BSS           __attribute__((section(".partition11BssSdram"))) 
#define PARTITION_11_SECTION_SDRAM_USS           __attribute__((section(".partition11UssSdram"))) 


#define PARTITION_12_SECTION_CCM                 __attribute__((section(".partition12DataCcm"))) 
#define PARTITION_12_SECTION_CCM_BSS             __attribute__((section(".partition12BssCcm"))) 
#define PARTITION_12_SECTION_CCM_USS             __attribute__((section(".partition12UssCcm"))) 
#define PARTITION_12_SECTION_SRAM1               __attribute__((section(".partition12DataSram1"))) 
#define PARTITION_12_SECTION_SRAM1_BSS           __attribute__((section(".partition12BssSram1"))) 
#define PARTITION_12_SECTION_SRAM1_USS           __attribute__((section(".partition12UssSram1"))) 
#define PARTITION_12_SECTION_SRAM2               __attribute__((section(".partition12DataSram2"))) 
#define PARTITION_12_SECTION_SRAM2_BSS           __attribute__((section(".partition12BssSram2"))) 
#define PARTITION_12_SECTION_SRAM2_USS           __attribute__((section(".partition12UssSram2"))) 
#define PARTITION_12_SECTION_SRAM3               __attribute__((section(".partition12DataSram3"))) 
#define PARTITION_12_SECTION_SRAM3_BSS           __attribute__((section(".partition12BssSram3"))) 
#define PARTITION_12_SECTION_SRAM3_USS           __attribute__((section(".partition12UssSram3"))) 
#define PARTITION_12_SECTION_SRAM_BCK            __attribute__((section(".partition12DataSramBackup"))) 
#define PARTITION_12_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition12BssSramBackup"))) 
#define PARTITION_12_SECTION_SRAM_BCK_USS        __attribute__((section(".partition12UssSramBackup"))) 
#define PARTITION_12_SECTION_SDRAM               __attribute__((section(".partition12DataSdram"))) 
#define PARTITION_12_SECTION_SDRAM_BSS           __attribute__((section(".partition12BssSdram"))) 
#define PARTITION_12_SECTION_SDRAM_USS           __attribute__((section(".partition12UssSdram"))) 


#define PARTITION_13_SECTION_CCM                 __attribute__((section(".partition13DataCcm"))) 
#define PARTITION_13_SECTION_CCM_BSS             __attribute__((section(".partition13BssCcm"))) 
#define PARTITION_13_SECTION_CCM_USS             __attribute__((section(".partition13UssCcm"))) 
#define PARTITION_13_SECTION_SRAM1               __attribute__((section(".partition13DataSram1"))) 
#define PARTITION_13_SECTION_SRAM1_BSS           __attribute__((section(".partition13BssSram1"))) 
#define PARTITION_13_SECTION_SRAM1_USS           __attribute__((section(".partition13UssSram1"))) 
#define PARTITION_13_SECTION_SRAM2               __attribute__((section(".partition13DataSram2"))) 
#define PARTITION_13_SECTION_SRAM2_BSS           __attribute__((section(".partition13BssSram2"))) 
#define PARTITION_13_SECTION_SRAM2_USS           __attribute__((section(".partition13UssSram2"))) 
#define PARTITION_13_SECTION_SRAM3               __attribute__((section(".partition13DataSram3"))) 
#define PARTITION_13_SECTION_SRAM3_BSS           __attribute__((section(".partition13BssSram3"))) 
#define PARTITION_13_SECTION_SRAM3_USS           __attribute__((section(".partition13UssSram3"))) 
#define PARTITION_13_SECTION_SRAM_BCK            __attribute__((section(".partition13DataSramBackup"))) 
#define PARTITION_13_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition13BssSramBackup"))) 
#define PARTITION_13_SECTION_SRAM_BCK_USS        __attribute__((section(".partition13UssSramBackup"))) 
#define PARTITION_13_SECTION_SDRAM               __attribute__((section(".partition13DataSdram"))) 
#define PARTITION_13_SECTION_SDRAM_BSS           __attribute__((section(".partition13BssSdram"))) 
#define PARTITION_13_SECTION_SDRAM_USS           __attribute__((section(".partition13UssSdram"))) 


#define PARTITION_14_SECTION_CCM                 __attribute__((section(".partition14DataCcm"))) 
#define PARTITION_14_SECTION_CCM_BSS             __attribute__((section(".partition14BssCcm"))) 
#define PARTITION_14_SECTION_CCM_USS             __attribute__((section(".partition14UssCcm"))) 
#define PARTITION_14_SECTION_SRAM1               __attribute__((section(".partition14DataSram1"))) 
#define PARTITION_14_SECTION_SRAM1_BSS           __attribute__((section(".partition14BssSram1"))) 
#define PARTITION_14_SECTION_SRAM1_USS           __attribute__((section(".partition14UssSram1"))) 
#define PARTITION_14_SECTION_SRAM2               __attribute__((section(".partition14DataSram2"))) 
#define PARTITION_14_SECTION_SRAM2_BSS           __attribute__((section(".partition14BssSram2"))) 
#define PARTITION_14_SECTION_SRAM2_USS           __attribute__((section(".partition14UssSram2"))) 
#define PARTITION_14_SECTION_SRAM3               __attribute__((section(".partition14DataSram3"))) 
#define PARTITION_14_SECTION_SRAM3_BSS           __attribute__((section(".partition14BssSram3"))) 
#define PARTITION_14_SECTION_SRAM3_USS           __attribute__((section(".partition14UssSram3"))) 
#define PARTITION_14_SECTION_SRAM_BCK            __attribute__((section(".partition14DataSramBackup"))) 
#define PARTITION_14_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition14BssSramBackup"))) 
#define PARTITION_14_SECTION_SRAM_BCK_USS        __attribute__((section(".partition14UssSramBackup"))) 
#define PARTITION_14_SECTION_SDRAM               __attribute__((section(".partition14DataSdram"))) 
#define PARTITION_14_SECTION_SDRAM_BSS           __attribute__((section(".partition14BssSdram"))) 
#define PARTITION_14_SECTION_SDRAM_USS           __attribute__((section(".partition14UssSdram"))) 


#define PARTITION_15_SECTION_CCM                 __attribute__((section(".partition15DataCcm"))) 
#define PARTITION_15_SECTION_CCM_BSS             __attribute__((section(".partition15BssCcm"))) 
#define PARTITION_15_SECTION_CCM_USS             __attribute__((section(".partition15UssCcm"))) 
#define PARTITION_15_SECTION_SRAM1               __attribute__((section(".partition15DataSram1"))) 
#define PARTITION_15_SECTION_SRAM1_BSS           __attribute__((section(".partition15BssSram1"))) 
#define PARTITION_15_SECTION_SRAM1_USS           __attribute__((section(".partition15UssSram1"))) 
#define PARTITION_15_SECTION_SRAM2               __attribute__((section(".partition15DataSram2"))) 
#define PARTITION_15_SECTION_SRAM2_BSS           __attribute__((section(".partition15BssSram2"))) 
#define PARTITION_15_SECTION_SRAM2_USS           __attribute__((section(".partition15UssSram2"))) 
#define PARTITION_15_SECTION_SRAM3               __attribute__((section(".partition15DataSram3"))) 
#define PARTITION_15_SECTION_SRAM3_BSS           __attribute__((section(".partition15BssSram3"))) 
#define PARTITION_15_SECTION_SRAM3_USS           __attribute__((section(".partition15UssSram3"))) 
#define PARTITION_15_SECTION_SRAM_BCK            __attribute__((section(".partition15DataSramBackup"))) 
#define PARTITION_15_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition15BssSramBackup"))) 
#define PARTITION_15_SECTION_SRAM_BCK_USS        __attribute__((section(".partition15UssSramBackup"))) 
#define PARTITION_15_SECTION_SDRAM               __attribute__((section(".partition15DataSdram"))) 
#define PARTITION_15_SECTION_SDRAM_BSS           __attribute__((section(".partition15BssSdram"))) 
#define PARTITION_15_SECTION_SDRAM_USS           __attribute__((section(".partition15UssSdram"))) 


#define PARTITION_16_SECTION_CCM                 __attribute__((section(".partition16DataCcm"))) 
#define PARTITION_16_SECTION_CCM_BSS             __attribute__((section(".partition16BssCcm"))) 
#define PARTITION_16_SECTION_CCM_USS             __attribute__((section(".partition16UssCcm"))) 
#define PARTITION_16_SECTION_SRAM1               __attribute__((section(".partition16DataSram1"))) 
#define PARTITION_16_SECTION_SRAM1_BSS           __attribute__((section(".partition16BssSram1"))) 
#define PARTITION_16_SECTION_SRAM1_USS           __attribute__((section(".partition16UssSram1"))) 
#define PARTITION_16_SECTION_SRAM2               __attribute__((section(".partition16DataSram2"))) 
#define PARTITION_16_SECTION_SRAM2_BSS           __attribute__((section(".partition16BssSram2"))) 
#define PARTITION_16_SECTION_SRAM2_USS           __attribute__((section(".partition16UssSram2"))) 
#define PARTITION_16_SECTION_SRAM3               __attribute__((section(".partition16DataSram3"))) 
#define PARTITION_16_SECTION_SRAM3_BSS           __attribute__((section(".partition16BssSram3"))) 
#define PARTITION_16_SECTION_SRAM3_USS           __attribute__((section(".partition16UssSram3"))) 
#define PARTITION_16_SECTION_SRAM_BCK            __attribute__((section(".partition16DataSramBackup"))) 
#define PARTITION_16_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition16BssSramBackup"))) 
#define PARTITION_16_SECTION_SRAM_BCK_USS        __attribute__((section(".partition16UssSramBackup"))) 
#define PARTITION_16_SECTION_SDRAM               __attribute__((section(".partition16DataSdram"))) 
#define PARTITION_16_SECTION_SDRAM_BSS           __attribute__((section(".partition16BssSdram"))) 
#define PARTITION_16_SECTION_SDRAM_USS           __attribute__((section(".partition16UssSdram"))) 


#define PARTITION_17_SECTION_CCM                 __attribute__((section(".partition17DataCcm"))) 
#define PARTITION_17_SECTION_CCM_BSS             __attribute__((section(".partition17BssCcm"))) 
#define PARTITION_17_SECTION_CCM_USS             __attribute__((section(".partition17UssCcm"))) 
#define PARTITION_17_SECTION_SRAM1               __attribute__((section(".partition17DataSram1"))) 
#define PARTITION_17_SECTION_SRAM1_BSS           __attribute__((section(".partition17BssSram1"))) 
#define PARTITION_17_SECTION_SRAM1_USS           __attribute__((section(".partition17UssSram1"))) 
#define PARTITION_17_SECTION_SRAM2               __attribute__((section(".partition17DataSram2"))) 
#define PARTITION_17_SECTION_SRAM2_BSS           __attribute__((section(".partition17BssSram2"))) 
#define PARTITION_17_SECTION_SRAM2_USS           __attribute__((section(".partition17UssSram2"))) 
#define PARTITION_17_SECTION_SRAM3               __attribute__((section(".partition17DataSram3"))) 
#define PARTITION_17_SECTION_SRAM3_BSS           __attribute__((section(".partition17BssSram3"))) 
#define PARTITION_17_SECTION_SRAM3_USS           __attribute__((section(".partition17UssSram3"))) 
#define PARTITION_17_SECTION_SRAM_BCK            __attribute__((section(".partition17DataSramBackup"))) 
#define PARTITION_17_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition17BssSramBackup"))) 
#define PARTITION_17_SECTION_SRAM_BCK_USS        __attribute__((section(".partition17UssSramBackup"))) 
#define PARTITION_17_SECTION_SDRAM               __attribute__((section(".partition17DataSdram"))) 
#define PARTITION_17_SECTION_SDRAM_BSS           __attribute__((section(".partition17BssSdram"))) 
#define PARTITION_17_SECTION_SDRAM_USS           __attribute__((section(".partition17UssSdram"))) 


#define PARTITION_18_SECTION_CCM                 __attribute__((section(".partition18DataCcm"))) 
#define PARTITION_18_SECTION_CCM_BSS             __attribute__((section(".partition18BssCcm"))) 
#define PARTITION_18_SECTION_CCM_USS             __attribute__((section(".partition18UssCcm"))) 
#define PARTITION_18_SECTION_SRAM1               __attribute__((section(".partition18DataSram1"))) 
#define PARTITION_18_SECTION_SRAM1_BSS           __attribute__((section(".partition18BssSram1"))) 
#define PARTITION_18_SECTION_SRAM1_USS           __attribute__((section(".partition18UssSram1"))) 
#define PARTITION_18_SECTION_SRAM2               __attribute__((section(".partition18DataSram2"))) 
#define PARTITION_18_SECTION_SRAM2_BSS           __attribute__((section(".partition18BssSram2"))) 
#define PARTITION_18_SECTION_SRAM2_USS           __attribute__((section(".partition18UssSram2"))) 
#define PARTITION_18_SECTION_SRAM3               __attribute__((section(".partition18DataSram3"))) 
#define PARTITION_18_SECTION_SRAM3_BSS           __attribute__((section(".partition18BssSram3"))) 
#define PARTITION_18_SECTION_SRAM3_USS           __attribute__((section(".partition18UssSram3"))) 
#define PARTITION_18_SECTION_SRAM_BCK            __attribute__((section(".partition18DataSramBackup"))) 
#define PARTITION_18_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition18BssSramBackup"))) 
#define PARTITION_18_SECTION_SRAM_BCK_USS        __attribute__((section(".partition18UssSramBackup"))) 
#define PARTITION_18_SECTION_SDRAM               __attribute__((section(".partition18DataSdram"))) 
#define PARTITION_18_SECTION_SDRAM_BSS           __attribute__((section(".partition18BssSdram"))) 
#define PARTITION_18_SECTION_SDRAM_USS           __attribute__((section(".partition18UssSdram"))) 


#define PARTITION_19_SECTION_CCM                 __attribute__((section(".partition19DataCcm"))) 
#define PARTITION_19_SECTION_CCM_BSS             __attribute__((section(".partition19BssCcm"))) 
#define PARTITION_19_SECTION_CCM_USS             __attribute__((section(".partition19UssCcm"))) 
#define PARTITION_19_SECTION_SRAM1               __attribute__((section(".partition19DataSram1"))) 
#define PARTITION_19_SECTION_SRAM1_BSS           __attribute__((section(".partition19BssSram1"))) 
#define PARTITION_19_SECTION_SRAM1_USS           __attribute__((section(".partition19UssSram1"))) 
#define PARTITION_19_SECTION_SRAM2               __attribute__((section(".partition19DataSram2"))) 
#define PARTITION_19_SECTION_SRAM2_BSS           __attribute__((section(".partition19BssSram2"))) 
#define PARTITION_19_SECTION_SRAM2_USS           __attribute__((section(".partition19UssSram2"))) 
#define PARTITION_19_SECTION_SRAM3               __attribute__((section(".partition19DataSram3"))) 
#define PARTITION_19_SECTION_SRAM3_BSS           __attribute__((section(".partition19BssSram3"))) 
#define PARTITION_19_SECTION_SRAM3_USS           __attribute__((section(".partition19UssSram3"))) 
#define PARTITION_19_SECTION_SRAM_BCK            __attribute__((section(".partition19DataSramBackup"))) 
#define PARTITION_19_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition19BssSramBackup"))) 
#define PARTITION_19_SECTION_SRAM_BCK_USS        __attribute__((section(".partition19UssSramBackup"))) 
#define PARTITION_19_SECTION_SDRAM               __attribute__((section(".partition19DataSdram"))) 
#define PARTITION_19_SECTION_SDRAM_BSS           __attribute__((section(".partition19BssSdram"))) 
#define PARTITION_19_SECTION_SDRAM_USS           __attribute__((section(".partition19UssSdram"))) 


#define PARTITION_20_SECTION_CCM                 __attribute__((section(".partition20DataCcm"))) 
#define PARTITION_20_SECTION_CCM_BSS             __attribute__((section(".partition20BssCcm"))) 
#define PARTITION_20_SECTION_CCM_USS             __attribute__((section(".partition20UssCcm"))) 
#define PARTITION_20_SECTION_SRAM1               __attribute__((section(".partition20DataSram1"))) 
#define PARTITION_20_SECTION_SRAM1_BSS           __attribute__((section(".partition20BssSram1"))) 
#define PARTITION_20_SECTION_SRAM1_USS           __attribute__((section(".partition20UssSram1"))) 
#define PARTITION_20_SECTION_SRAM2               __attribute__((section(".partition20DataSram2"))) 
#define PARTITION_20_SECTION_SRAM2_BSS           __attribute__((section(".partition20BssSram2"))) 
#define PARTITION_20_SECTION_SRAM2_USS           __attribute__((section(".partition20UssSram2"))) 
#define PARTITION_20_SECTION_SRAM3               __attribute__((section(".partition20DataSram3"))) 
#define PARTITION_20_SECTION_SRAM3_BSS           __attribute__((section(".partition20BssSram3"))) 
#define PARTITION_20_SECTION_SRAM3_USS           __attribute__((section(".partition20UssSram3"))) 
#define PARTITION_20_SECTION_SRAM_BCK            __attribute__((section(".partition20DataSramBackup"))) 
#define PARTITION_20_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition20BssSramBackup"))) 
#define PARTITION_20_SECTION_SRAM_BCK_USS        __attribute__((section(".partition20UssSramBackup"))) 
#define PARTITION_20_SECTION_SDRAM               __attribute__((section(".partition20DataSdram"))) 
#define PARTITION_20_SECTION_SDRAM_BSS           __attribute__((section(".partition20BssSdram"))) 
#define PARTITION_20_SECTION_SDRAM_USS           __attribute__((section(".partition20UssSdram"))) 


#define PARTITION_21_SECTION_CCM                 __attribute__((section(".partition21DataCcm"))) 
#define PARTITION_21_SECTION_CCM_BSS             __attribute__((section(".partition21BssCcm"))) 
#define PARTITION_21_SECTION_CCM_USS             __attribute__((section(".partition21UssCcm"))) 
#define PARTITION_21_SECTION_SRAM1               __attribute__((section(".partition21DataSram1"))) 
#define PARTITION_21_SECTION_SRAM1_BSS           __attribute__((section(".partition21BssSram1"))) 
#define PARTITION_21_SECTION_SRAM1_USS           __attribute__((section(".partition21UssSram1"))) 
#define PARTITION_21_SECTION_SRAM2               __attribute__((section(".partition21DataSram2"))) 
#define PARTITION_21_SECTION_SRAM2_BSS           __attribute__((section(".partition21BssSram2"))) 
#define PARTITION_21_SECTION_SRAM2_USS           __attribute__((section(".partition21UssSram2"))) 
#define PARTITION_21_SECTION_SRAM3               __attribute__((section(".partition21DataSram3"))) 
#define PARTITION_21_SECTION_SRAM3_BSS           __attribute__((section(".partition21BssSram3"))) 
#define PARTITION_21_SECTION_SRAM3_USS           __attribute__((section(".partition21UssSram3"))) 
#define PARTITION_21_SECTION_SRAM_BCK            __attribute__((section(".partition21DataSramBackup"))) 
#define PARTITION_21_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition21BssSramBackup"))) 
#define PARTITION_21_SECTION_SRAM_BCK_USS        __attribute__((section(".partition21UssSramBackup"))) 
#define PARTITION_21_SECTION_SDRAM               __attribute__((section(".partition21DataSdram"))) 
#define PARTITION_21_SECTION_SDRAM_BSS           __attribute__((section(".partition21BssSdram"))) 
#define PARTITION_21_SECTION_SDRAM_USS           __attribute__((section(".partition21UssSdram"))) 


#define PARTITION_22_SECTION_CCM                 __attribute__((section(".partition22DataCcm"))) 
#define PARTITION_22_SECTION_CCM_BSS             __attribute__((section(".partition22BssCcm"))) 
#define PARTITION_22_SECTION_CCM_USS             __attribute__((section(".partition22UssCcm"))) 
#define PARTITION_22_SECTION_SRAM1               __attribute__((section(".partition22DataSram1"))) 
#define PARTITION_22_SECTION_SRAM1_BSS           __attribute__((section(".partition22BssSram1"))) 
#define PARTITION_22_SECTION_SRAM1_USS           __attribute__((section(".partition22UssSram1"))) 
#define PARTITION_22_SECTION_SRAM2               __attribute__((section(".partition22DataSram2"))) 
#define PARTITION_22_SECTION_SRAM2_BSS           __attribute__((section(".partition22BssSram2"))) 
#define PARTITION_22_SECTION_SRAM2_USS           __attribute__((section(".partition22UssSram2"))) 
#define PARTITION_22_SECTION_SRAM3               __attribute__((section(".partition22DataSram3"))) 
#define PARTITION_22_SECTION_SRAM3_BSS           __attribute__((section(".partition22BssSram3"))) 
#define PARTITION_22_SECTION_SRAM3_USS           __attribute__((section(".partition22UssSram3"))) 
#define PARTITION_22_SECTION_SRAM_BCK            __attribute__((section(".partition22DataSramBackup"))) 
#define PARTITION_22_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition22BssSramBackup"))) 
#define PARTITION_22_SECTION_SRAM_BCK_USS        __attribute__((section(".partition22UssSramBackup"))) 
#define PARTITION_22_SECTION_SDRAM               __attribute__((section(".partition22DataSdram"))) 
#define PARTITION_22_SECTION_SDRAM_BSS           __attribute__((section(".partition22BssSdram"))) 
#define PARTITION_22_SECTION_SDRAM_USS           __attribute__((section(".partition22UssSdram"))) 


#define PARTITION_23_SECTION_CCM                 __attribute__((section(".partition23DataCcm"))) 
#define PARTITION_23_SECTION_CCM_BSS             __attribute__((section(".partition23BssCcm"))) 
#define PARTITION_23_SECTION_CCM_USS             __attribute__((section(".partition23UssCcm"))) 
#define PARTITION_23_SECTION_SRAM1               __attribute__((section(".partition23DataSram1"))) 
#define PARTITION_23_SECTION_SRAM1_BSS           __attribute__((section(".partition23BssSram1"))) 
#define PARTITION_23_SECTION_SRAM1_USS           __attribute__((section(".partition23UssSram1"))) 
#define PARTITION_23_SECTION_SRAM2               __attribute__((section(".partition23DataSram2"))) 
#define PARTITION_23_SECTION_SRAM2_BSS           __attribute__((section(".partition23BssSram2"))) 
#define PARTITION_23_SECTION_SRAM2_USS           __attribute__((section(".partition23UssSram2"))) 
#define PARTITION_23_SECTION_SRAM3               __attribute__((section(".partition23DataSram3"))) 
#define PARTITION_23_SECTION_SRAM3_BSS           __attribute__((section(".partition23BssSram3"))) 
#define PARTITION_23_SECTION_SRAM3_USS           __attribute__((section(".partition23UssSram3"))) 
#define PARTITION_23_SECTION_SRAM_BCK            __attribute__((section(".partition23DataSramBackup"))) 
#define PARTITION_23_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition23BssSramBackup"))) 
#define PARTITION_23_SECTION_SRAM_BCK_USS        __attribute__((section(".partition23UssSramBackup"))) 
#define PARTITION_23_SECTION_SDRAM               __attribute__((section(".partition23DataSdram"))) 
#define PARTITION_23_SECTION_SDRAM_BSS           __attribute__((section(".partition23BssSdram"))) 
#define PARTITION_23_SECTION_SDRAM_USS           __attribute__((section(".partition23UssSdram"))) 


#define PARTITION_24_SECTION_CCM                 __attribute__((section(".partition24DataCcm"))) 
#define PARTITION_24_SECTION_CCM_BSS             __attribute__((section(".partition24BssCcm"))) 
#define PARTITION_24_SECTION_CCM_USS             __attribute__((section(".partition24UssCcm"))) 
#define PARTITION_24_SECTION_SRAM1               __attribute__((section(".partition24DataSram1"))) 
#define PARTITION_24_SECTION_SRAM1_BSS           __attribute__((section(".partition24BssSram1"))) 
#define PARTITION_24_SECTION_SRAM1_USS           __attribute__((section(".partition24UssSram1"))) 
#define PARTITION_24_SECTION_SRAM2               __attribute__((section(".partition24DataSram2"))) 
#define PARTITION_24_SECTION_SRAM2_BSS           __attribute__((section(".partition24BssSram2"))) 
#define PARTITION_24_SECTION_SRAM2_USS           __attribute__((section(".partition24UssSram2"))) 
#define PARTITION_24_SECTION_SRAM3               __attribute__((section(".partition24DataSram3"))) 
#define PARTITION_24_SECTION_SRAM3_BSS           __attribute__((section(".partition24BssSram3"))) 
#define PARTITION_24_SECTION_SRAM3_USS           __attribute__((section(".partition24UssSram3"))) 
#define PARTITION_24_SECTION_SRAM_BCK            __attribute__((section(".partition24DataSramBackup"))) 
#define PARTITION_24_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition24BssSramBackup"))) 
#define PARTITION_24_SECTION_SRAM_BCK_USS        __attribute__((section(".partition24UssSramBackup"))) 
#define PARTITION_24_SECTION_SDRAM               __attribute__((section(".partition24DataSdram"))) 
#define PARTITION_24_SECTION_SDRAM_BSS           __attribute__((section(".partition24BssSdram"))) 
#define PARTITION_24_SECTION_SDRAM_USS           __attribute__((section(".partition24UssSdram"))) 


#define PARTITION_25_SECTION_CCM                 __attribute__((section(".partition25DataCcm"))) 
#define PARTITION_25_SECTION_CCM_BSS             __attribute__((section(".partition25BssCcm"))) 
#define PARTITION_25_SECTION_CCM_USS             __attribute__((section(".partition25UssCcm"))) 
#define PARTITION_25_SECTION_SRAM1               __attribute__((section(".partition25DataSram1"))) 
#define PARTITION_25_SECTION_SRAM1_BSS           __attribute__((section(".partition25BssSram1"))) 
#define PARTITION_25_SECTION_SRAM1_USS           __attribute__((section(".partition25UssSram1"))) 
#define PARTITION_25_SECTION_SRAM2               __attribute__((section(".partition25DataSram2"))) 
#define PARTITION_25_SECTION_SRAM2_BSS           __attribute__((section(".partition25BssSram2"))) 
#define PARTITION_25_SECTION_SRAM2_USS           __attribute__((section(".partition25UssSram2"))) 
#define PARTITION_25_SECTION_SRAM3               __attribute__((section(".partition25DataSram3"))) 
#define PARTITION_25_SECTION_SRAM3_BSS           __attribute__((section(".partition25BssSram3"))) 
#define PARTITION_25_SECTION_SRAM3_USS           __attribute__((section(".partition25UssSram3"))) 
#define PARTITION_25_SECTION_SRAM_BCK            __attribute__((section(".partition25DataSramBackup"))) 
#define PARTITION_25_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition25BssSramBackup"))) 
#define PARTITION_25_SECTION_SRAM_BCK_USS        __attribute__((section(".partition25UssSramBackup"))) 
#define PARTITION_25_SECTION_SDRAM               __attribute__((section(".partition25DataSdram"))) 
#define PARTITION_25_SECTION_SDRAM_BSS           __attribute__((section(".partition25BssSdram"))) 
#define PARTITION_25_SECTION_SDRAM_USS           __attribute__((section(".partition25UssSdram"))) 


#define PARTITION_26_SECTION_CCM                 __attribute__((section(".partition26DataCcm"))) 
#define PARTITION_26_SECTION_CCM_BSS             __attribute__((section(".partition26BssCcm"))) 
#define PARTITION_26_SECTION_CCM_USS             __attribute__((section(".partition26UssCcm"))) 
#define PARTITION_26_SECTION_SRAM1               __attribute__((section(".partition26DataSram1"))) 
#define PARTITION_26_SECTION_SRAM1_BSS           __attribute__((section(".partition26BssSram1"))) 
#define PARTITION_26_SECTION_SRAM1_USS           __attribute__((section(".partition26UssSram1"))) 
#define PARTITION_26_SECTION_SRAM2               __attribute__((section(".partition26DataSram2"))) 
#define PARTITION_26_SECTION_SRAM2_BSS           __attribute__((section(".partition26BssSram2"))) 
#define PARTITION_26_SECTION_SRAM2_USS           __attribute__((section(".partition26UssSram2"))) 
#define PARTITION_26_SECTION_SRAM3               __attribute__((section(".partition26DataSram3"))) 
#define PARTITION_26_SECTION_SRAM3_BSS           __attribute__((section(".partition26BssSram3"))) 
#define PARTITION_26_SECTION_SRAM3_USS           __attribute__((section(".partition26UssSram3"))) 
#define PARTITION_26_SECTION_SRAM_BCK            __attribute__((section(".partition26DataSramBackup"))) 
#define PARTITION_26_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition26BssSramBackup"))) 
#define PARTITION_26_SECTION_SRAM_BCK_USS        __attribute__((section(".partition26UssSramBackup"))) 
#define PARTITION_26_SECTION_SDRAM               __attribute__((section(".partition26DataSdram"))) 
#define PARTITION_26_SECTION_SDRAM_BSS           __attribute__((section(".partition26BssSdram"))) 
#define PARTITION_26_SECTION_SDRAM_USS           __attribute__((section(".partition26UssSdram"))) 


#define PARTITION_27_SECTION_CCM                 __attribute__((section(".partition27DataCcm"))) 
#define PARTITION_27_SECTION_CCM_BSS             __attribute__((section(".partition27BssCcm"))) 
#define PARTITION_27_SECTION_CCM_USS             __attribute__((section(".partition27UssCcm"))) 
#define PARTITION_27_SECTION_SRAM1               __attribute__((section(".partition27DataSram1"))) 
#define PARTITION_27_SECTION_SRAM1_BSS           __attribute__((section(".partition27BssSram1"))) 
#define PARTITION_27_SECTION_SRAM1_USS           __attribute__((section(".partition27UssSram1"))) 
#define PARTITION_27_SECTION_SRAM2               __attribute__((section(".partition27DataSram2"))) 
#define PARTITION_27_SECTION_SRAM2_BSS           __attribute__((section(".partition27BssSram2"))) 
#define PARTITION_27_SECTION_SRAM2_USS           __attribute__((section(".partition27UssSram2"))) 
#define PARTITION_27_SECTION_SRAM3               __attribute__((section(".partition27DataSram3"))) 
#define PARTITION_27_SECTION_SRAM3_BSS           __attribute__((section(".partition27BssSram3"))) 
#define PARTITION_27_SECTION_SRAM3_USS           __attribute__((section(".partition27UssSram3"))) 
#define PARTITION_27_SECTION_SRAM_BCK            __attribute__((section(".partition27DataSramBackup"))) 
#define PARTITION_27_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition27BssSramBackup"))) 
#define PARTITION_27_SECTION_SRAM_BCK_USS        __attribute__((section(".partition27UssSramBackup"))) 
#define PARTITION_27_SECTION_SDRAM               __attribute__((section(".partition27DataSdram"))) 
#define PARTITION_27_SECTION_SDRAM_BSS           __attribute__((section(".partition27BssSdram"))) 
#define PARTITION_27_SECTION_SDRAM_USS           __attribute__((section(".partition27UssSdram"))) 


#define PARTITION_28_SECTION_CCM                 __attribute__((section(".partition28DataCcm"))) 
#define PARTITION_28_SECTION_CCM_BSS             __attribute__((section(".partition28BssCcm"))) 
#define PARTITION_28_SECTION_CCM_USS             __attribute__((section(".partition28UssCcm"))) 
#define PARTITION_28_SECTION_SRAM1               __attribute__((section(".partition28DataSram1"))) 
#define PARTITION_28_SECTION_SRAM1_BSS           __attribute__((section(".partition28BssSram1"))) 
#define PARTITION_28_SECTION_SRAM1_USS           __attribute__((section(".partition28UssSram1"))) 
#define PARTITION_28_SECTION_SRAM2               __attribute__((section(".partition28DataSram2"))) 
#define PARTITION_28_SECTION_SRAM2_BSS           __attribute__((section(".partition28BssSram2"))) 
#define PARTITION_28_SECTION_SRAM2_USS           __attribute__((section(".partition28UssSram2"))) 
#define PARTITION_28_SECTION_SRAM3               __attribute__((section(".partition28DataSram3"))) 
#define PARTITION_28_SECTION_SRAM3_BSS           __attribute__((section(".partition28BssSram3"))) 
#define PARTITION_28_SECTION_SRAM3_USS           __attribute__((section(".partition28UssSram3"))) 
#define PARTITION_28_SECTION_SRAM_BCK            __attribute__((section(".partition28DataSramBackup"))) 
#define PARTITION_28_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition28BssSramBackup"))) 
#define PARTITION_28_SECTION_SRAM_BCK_USS        __attribute__((section(".partition28UssSramBackup"))) 
#define PARTITION_28_SECTION_SDRAM               __attribute__((section(".partition28DataSdram"))) 
#define PARTITION_28_SECTION_SDRAM_BSS           __attribute__((section(".partition28BssSdram"))) 
#define PARTITION_28_SECTION_SDRAM_USS           __attribute__((section(".partition28UssSdram"))) 


#define PARTITION_29_SECTION_CCM                 __attribute__((section(".partition29DataCcm"))) 
#define PARTITION_29_SECTION_CCM_BSS             __attribute__((section(".partition29BssCcm"))) 
#define PARTITION_29_SECTION_CCM_USS             __attribute__((section(".partition29UssCcm"))) 
#define PARTITION_29_SECTION_SRAM1               __attribute__((section(".partition29DataSram1"))) 
#define PARTITION_29_SECTION_SRAM1_BSS           __attribute__((section(".partition29BssSram1"))) 
#define PARTITION_29_SECTION_SRAM1_USS           __attribute__((section(".partition29UssSram1"))) 
#define PARTITION_29_SECTION_SRAM2               __attribute__((section(".partition29DataSram2"))) 
#define PARTITION_29_SECTION_SRAM2_BSS           __attribute__((section(".partition29BssSram2"))) 
#define PARTITION_29_SECTION_SRAM2_USS           __attribute__((section(".partition29UssSram2"))) 
#define PARTITION_29_SECTION_SRAM3               __attribute__((section(".partition29DataSram3"))) 
#define PARTITION_29_SECTION_SRAM3_BSS           __attribute__((section(".partition29BssSram3"))) 
#define PARTITION_29_SECTION_SRAM3_USS           __attribute__((section(".partition29UssSram3"))) 
#define PARTITION_29_SECTION_SRAM_BCK            __attribute__((section(".partition29DataSramBackup"))) 
#define PARTITION_29_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition29BssSramBackup"))) 
#define PARTITION_29_SECTION_SRAM_BCK_USS        __attribute__((section(".partition29UssSramBackup"))) 
#define PARTITION_29_SECTION_SDRAM               __attribute__((section(".partition29DataSdram"))) 
#define PARTITION_29_SECTION_SDRAM_BSS           __attribute__((section(".partition29BssSdram"))) 
#define PARTITION_29_SECTION_SDRAM_USS           __attribute__((section(".partition29UssSdram"))) 


#define PARTITION_30_SECTION_CCM                 __attribute__((section(".partition30DataCcm"))) 
#define PARTITION_30_SECTION_CCM_BSS             __attribute__((section(".partition30BssCcm"))) 
#define PARTITION_30_SECTION_CCM_USS             __attribute__((section(".partition30UssCcm"))) 
#define PARTITION_30_SECTION_SRAM1               __attribute__((section(".partition30DataSram1"))) 
#define PARTITION_30_SECTION_SRAM1_BSS           __attribute__((section(".partition30BssSram1"))) 
#define PARTITION_30_SECTION_SRAM1_USS           __attribute__((section(".partition30UssSram1"))) 
#define PARTITION_30_SECTION_SRAM2               __attribute__((section(".partition30DataSram2"))) 
#define PARTITION_30_SECTION_SRAM2_BSS           __attribute__((section(".partition30BssSram2"))) 
#define PARTITION_30_SECTION_SRAM2_USS           __attribute__((section(".partition30UssSram2"))) 
#define PARTITION_30_SECTION_SRAM3               __attribute__((section(".partition30DataSram3"))) 
#define PARTITION_30_SECTION_SRAM3_BSS           __attribute__((section(".partition30BssSram3"))) 
#define PARTITION_30_SECTION_SRAM3_USS           __attribute__((section(".partition30UssSram3"))) 
#define PARTITION_30_SECTION_SRAM_BCK            __attribute__((section(".partition30DataSramBackup"))) 
#define PARTITION_30_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition30BssSramBackup"))) 
#define PARTITION_30_SECTION_SRAM_BCK_USS        __attribute__((section(".partition30UssSramBackup"))) 
#define PARTITION_30_SECTION_SDRAM               __attribute__((section(".partition30DataSdram"))) 
#define PARTITION_30_SECTION_SDRAM_BSS           __attribute__((section(".partition30BssSdram"))) 
#define PARTITION_30_SECTION_SDRAM_USS           __attribute__((section(".partition30UssSdram"))) 


#define PARTITION_31_SECTION_CCM                 __attribute__((section(".partition31DataCcm"))) 
#define PARTITION_31_SECTION_CCM_BSS             __attribute__((section(".partition31BssCcm"))) 
#define PARTITION_31_SECTION_CCM_USS             __attribute__((section(".partition31UssCcm"))) 
#define PARTITION_31_SECTION_SRAM1               __attribute__((section(".partition31DataSram1"))) 
#define PARTITION_31_SECTION_SRAM1_BSS           __attribute__((section(".partition31BssSram1"))) 
#define PARTITION_31_SECTION_SRAM1_USS           __attribute__((section(".partition31UssSram1"))) 
#define PARTITION_31_SECTION_SRAM2               __attribute__((section(".partition31DataSram2"))) 
#define PARTITION_31_SECTION_SRAM2_BSS           __attribute__((section(".partition31BssSram2"))) 
#define PARTITION_31_SECTION_SRAM2_USS           __attribute__((section(".partition31UssSram2"))) 
#define PARTITION_31_SECTION_SRAM3               __attribute__((section(".partition31DataSram3"))) 
#define PARTITION_31_SECTION_SRAM3_BSS           __attribute__((section(".partition31BssSram3"))) 
#define PARTITION_31_SECTION_SRAM3_USS           __attribute__((section(".partition31UssSram3"))) 
#define PARTITION_31_SECTION_SRAM_BCK            __attribute__((section(".partition31DataSramBackup"))) 
#define PARTITION_31_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition31BssSramBackup"))) 
#define PARTITION_31_SECTION_SRAM_BCK_USS        __attribute__((section(".partition31UssSramBackup"))) 
#define PARTITION_31_SECTION_SDRAM               __attribute__((section(".partition31DataSdram"))) 
#define PARTITION_31_SECTION_SDRAM_BSS           __attribute__((section(".partition31BssSdram"))) 
#define PARTITION_31_SECTION_SDRAM_USS           __attribute__((section(".partition31UssSdram"))) 


#define PARTITION_32_SECTION_CCM                 __attribute__((section(".partition32DataCcm"))) 
#define PARTITION_32_SECTION_CCM_BSS             __attribute__((section(".partition32BssCcm"))) 
#define PARTITION_32_SECTION_CCM_USS             __attribute__((section(".partition32UssCcm"))) 
#define PARTITION_32_SECTION_SRAM1               __attribute__((section(".partition32DataSram1"))) 
#define PARTITION_32_SECTION_SRAM1_BSS           __attribute__((section(".partition32BssSram1"))) 
#define PARTITION_32_SECTION_SRAM1_USS           __attribute__((section(".partition32UssSram1"))) 
#define PARTITION_32_SECTION_SRAM2               __attribute__((section(".partition32DataSram2"))) 
#define PARTITION_32_SECTION_SRAM2_BSS           __attribute__((section(".partition32BssSram2"))) 
#define PARTITION_32_SECTION_SRAM2_USS           __attribute__((section(".partition32UssSram2"))) 
#define PARTITION_32_SECTION_SRAM3               __attribute__((section(".partition32DataSram3"))) 
#define PARTITION_32_SECTION_SRAM3_BSS           __attribute__((section(".partition32BssSram3"))) 
#define PARTITION_32_SECTION_SRAM3_USS           __attribute__((section(".partition32UssSram3"))) 
#define PARTITION_32_SECTION_SRAM_BCK            __attribute__((section(".partition32DataSramBackup"))) 
#define PARTITION_32_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition32BssSramBackup"))) 
#define PARTITION_32_SECTION_SRAM_BCK_USS        __attribute__((section(".partition32UssSramBackup"))) 
#define PARTITION_32_SECTION_SDRAM               __attribute__((section(".partition32DataSdram"))) 
#define PARTITION_32_SECTION_SDRAM_BSS           __attribute__((section(".partition32BssSdram"))) 
#define PARTITION_32_SECTION_SDRAM_USS           __attribute__((section(".partition32UssSdram"))) 


#define PARTITION_33_SECTION_CCM                 __attribute__((section(".partition33DataCcm"))) 
#define PARTITION_33_SECTION_CCM_BSS             __attribute__((section(".partition33BssCcm"))) 
#define PARTITION_33_SECTION_CCM_USS             __attribute__((section(".partition33UssCcm"))) 
#define PARTITION_33_SECTION_SRAM1               __attribute__((section(".partition33DataSram1"))) 
#define PARTITION_33_SECTION_SRAM1_BSS           __attribute__((section(".partition33BssSram1"))) 
#define PARTITION_33_SECTION_SRAM1_USS           __attribute__((section(".partition33UssSram1"))) 
#define PARTITION_33_SECTION_SRAM2               __attribute__((section(".partition33DataSram2"))) 
#define PARTITION_33_SECTION_SRAM2_BSS           __attribute__((section(".partition33BssSram2"))) 
#define PARTITION_33_SECTION_SRAM2_USS           __attribute__((section(".partition33UssSram2"))) 
#define PARTITION_33_SECTION_SRAM3               __attribute__((section(".partition33DataSram3"))) 
#define PARTITION_33_SECTION_SRAM3_BSS           __attribute__((section(".partition33BssSram3"))) 
#define PARTITION_33_SECTION_SRAM3_USS           __attribute__((section(".partition33UssSram3"))) 
#define PARTITION_33_SECTION_SRAM_BCK            __attribute__((section(".partition33DataSramBackup"))) 
#define PARTITION_33_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition33BssSramBackup"))) 
#define PARTITION_33_SECTION_SRAM_BCK_USS        __attribute__((section(".partition33UssSramBackup"))) 
#define PARTITION_33_SECTION_SDRAM               __attribute__((section(".partition33DataSdram"))) 
#define PARTITION_33_SECTION_SDRAM_BSS           __attribute__((section(".partition33BssSdram"))) 
#define PARTITION_33_SECTION_SDRAM_USS           __attribute__((section(".partition33UssSdram"))) 


#define PARTITION_34_SECTION_CCM                 __attribute__((section(".partition34DataCcm"))) 
#define PARTITION_34_SECTION_CCM_BSS             __attribute__((section(".partition34BssCcm"))) 
#define PARTITION_34_SECTION_CCM_USS             __attribute__((section(".partition34UssCcm"))) 
#define PARTITION_34_SECTION_SRAM1               __attribute__((section(".partition34DataSram1"))) 
#define PARTITION_34_SECTION_SRAM1_BSS           __attribute__((section(".partition34BssSram1"))) 
#define PARTITION_34_SECTION_SRAM1_USS           __attribute__((section(".partition34UssSram1"))) 
#define PARTITION_34_SECTION_SRAM2               __attribute__((section(".partition34DataSram2"))) 
#define PARTITION_34_SECTION_SRAM2_BSS           __attribute__((section(".partition34BssSram2"))) 
#define PARTITION_34_SECTION_SRAM2_USS           __attribute__((section(".partition34UssSram2"))) 
#define PARTITION_34_SECTION_SRAM3               __attribute__((section(".partition34DataSram3"))) 
#define PARTITION_34_SECTION_SRAM3_BSS           __attribute__((section(".partition34BssSram3"))) 
#define PARTITION_34_SECTION_SRAM3_USS           __attribute__((section(".partition34UssSram3"))) 
#define PARTITION_34_SECTION_SRAM_BCK            __attribute__((section(".partition34DataSramBackup"))) 
#define PARTITION_34_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition34BssSramBackup"))) 
#define PARTITION_34_SECTION_SRAM_BCK_USS        __attribute__((section(".partition34UssSramBackup"))) 
#define PARTITION_34_SECTION_SDRAM               __attribute__((section(".partition34DataSdram"))) 
#define PARTITION_34_SECTION_SDRAM_BSS           __attribute__((section(".partition34BssSdram"))) 
#define PARTITION_34_SECTION_SDRAM_USS           __attribute__((section(".partition34UssSdram"))) 


#define PARTITION_35_SECTION_CCM                 __attribute__((section(".partition35DataCcm"))) 
#define PARTITION_35_SECTION_CCM_BSS             __attribute__((section(".partition35BssCcm"))) 
#define PARTITION_35_SECTION_CCM_USS             __attribute__((section(".partition35UssCcm"))) 
#define PARTITION_35_SECTION_SRAM1               __attribute__((section(".partition35DataSram1"))) 
#define PARTITION_35_SECTION_SRAM1_BSS           __attribute__((section(".partition35BssSram1"))) 
#define PARTITION_35_SECTION_SRAM1_USS           __attribute__((section(".partition35UssSram1"))) 
#define PARTITION_35_SECTION_SRAM2               __attribute__((section(".partition35DataSram2"))) 
#define PARTITION_35_SECTION_SRAM2_BSS           __attribute__((section(".partition35BssSram2"))) 
#define PARTITION_35_SECTION_SRAM2_USS           __attribute__((section(".partition35UssSram2"))) 
#define PARTITION_35_SECTION_SRAM3               __attribute__((section(".partition35DataSram3"))) 
#define PARTITION_35_SECTION_SRAM3_BSS           __attribute__((section(".partition35BssSram3"))) 
#define PARTITION_35_SECTION_SRAM3_USS           __attribute__((section(".partition35UssSram3"))) 
#define PARTITION_35_SECTION_SRAM_BCK            __attribute__((section(".partition35DataSramBackup"))) 
#define PARTITION_35_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition35BssSramBackup"))) 
#define PARTITION_35_SECTION_SRAM_BCK_USS        __attribute__((section(".partition35UssSramBackup"))) 
#define PARTITION_35_SECTION_SDRAM               __attribute__((section(".partition35DataSdram"))) 
#define PARTITION_35_SECTION_SDRAM_BSS           __attribute__((section(".partition35BssSdram"))) 
#define PARTITION_35_SECTION_SDRAM_USS           __attribute__((section(".partition35UssSdram"))) 


#define PARTITION_36_SECTION_CCM                 __attribute__((section(".partition36DataCcm"))) 
#define PARTITION_36_SECTION_CCM_BSS             __attribute__((section(".partition36BssCcm"))) 
#define PARTITION_36_SECTION_CCM_USS             __attribute__((section(".partition36UssCcm"))) 
#define PARTITION_36_SECTION_SRAM1               __attribute__((section(".partition36DataSram1"))) 
#define PARTITION_36_SECTION_SRAM1_BSS           __attribute__((section(".partition36BssSram1"))) 
#define PARTITION_36_SECTION_SRAM1_USS           __attribute__((section(".partition36UssSram1"))) 
#define PARTITION_36_SECTION_SRAM2               __attribute__((section(".partition36DataSram2"))) 
#define PARTITION_36_SECTION_SRAM2_BSS           __attribute__((section(".partition36BssSram2"))) 
#define PARTITION_36_SECTION_SRAM2_USS           __attribute__((section(".partition36UssSram2"))) 
#define PARTITION_36_SECTION_SRAM3               __attribute__((section(".partition36DataSram3"))) 
#define PARTITION_36_SECTION_SRAM3_BSS           __attribute__((section(".partition36BssSram3"))) 
#define PARTITION_36_SECTION_SRAM3_USS           __attribute__((section(".partition36UssSram3"))) 
#define PARTITION_36_SECTION_SRAM_BCK            __attribute__((section(".partition36DataSramBackup"))) 
#define PARTITION_36_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition36BssSramBackup"))) 
#define PARTITION_36_SECTION_SRAM_BCK_USS        __attribute__((section(".partition36UssSramBackup"))) 
#define PARTITION_36_SECTION_SDRAM               __attribute__((section(".partition36DataSdram"))) 
#define PARTITION_36_SECTION_SDRAM_BSS           __attribute__((section(".partition36BssSdram"))) 
#define PARTITION_36_SECTION_SDRAM_USS           __attribute__((section(".partition36UssSdram"))) 


#define PARTITION_37_SECTION_CCM                 __attribute__((section(".partition37DataCcm"))) 
#define PARTITION_37_SECTION_CCM_BSS             __attribute__((section(".partition37BssCcm"))) 
#define PARTITION_37_SECTION_CCM_USS             __attribute__((section(".partition37UssCcm"))) 
#define PARTITION_37_SECTION_SRAM1               __attribute__((section(".partition37DataSram1"))) 
#define PARTITION_37_SECTION_SRAM1_BSS           __attribute__((section(".partition37BssSram1"))) 
#define PARTITION_37_SECTION_SRAM1_USS           __attribute__((section(".partition37UssSram1"))) 
#define PARTITION_37_SECTION_SRAM2               __attribute__((section(".partition37DataSram2"))) 
#define PARTITION_37_SECTION_SRAM2_BSS           __attribute__((section(".partition37BssSram2"))) 
#define PARTITION_37_SECTION_SRAM2_USS           __attribute__((section(".partition37UssSram2"))) 
#define PARTITION_37_SECTION_SRAM3               __attribute__((section(".partition37DataSram3"))) 
#define PARTITION_37_SECTION_SRAM3_BSS           __attribute__((section(".partition37BssSram3"))) 
#define PARTITION_37_SECTION_SRAM3_USS           __attribute__((section(".partition37UssSram3"))) 
#define PARTITION_37_SECTION_SRAM_BCK            __attribute__((section(".partition37DataSramBackup"))) 
#define PARTITION_37_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition37BssSramBackup"))) 
#define PARTITION_37_SECTION_SRAM_BCK_USS        __attribute__((section(".partition37UssSramBackup"))) 
#define PARTITION_37_SECTION_SDRAM               __attribute__((section(".partition37DataSdram"))) 
#define PARTITION_37_SECTION_SDRAM_BSS           __attribute__((section(".partition37BssSdram"))) 
#define PARTITION_37_SECTION_SDRAM_USS           __attribute__((section(".partition37UssSdram"))) 


#define PARTITION_38_SECTION_CCM                 __attribute__((section(".partition38DataCcm"))) 
#define PARTITION_38_SECTION_CCM_BSS             __attribute__((section(".partition38BssCcm"))) 
#define PARTITION_38_SECTION_CCM_USS             __attribute__((section(".partition38UssCcm"))) 
#define PARTITION_38_SECTION_SRAM1               __attribute__((section(".partition38DataSram1"))) 
#define PARTITION_38_SECTION_SRAM1_BSS           __attribute__((section(".partition38BssSram1"))) 
#define PARTITION_38_SECTION_SRAM1_USS           __attribute__((section(".partition38UssSram1"))) 
#define PARTITION_38_SECTION_SRAM2               __attribute__((section(".partition38DataSram2"))) 
#define PARTITION_38_SECTION_SRAM2_BSS           __attribute__((section(".partition38BssSram2"))) 
#define PARTITION_38_SECTION_SRAM2_USS           __attribute__((section(".partition38UssSram2"))) 
#define PARTITION_38_SECTION_SRAM3               __attribute__((section(".partition38DataSram3"))) 
#define PARTITION_38_SECTION_SRAM3_BSS           __attribute__((section(".partition38BssSram3"))) 
#define PARTITION_38_SECTION_SRAM3_USS           __attribute__((section(".partition38UssSram3"))) 
#define PARTITION_38_SECTION_SRAM_BCK            __attribute__((section(".partition38DataSramBackup"))) 
#define PARTITION_38_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition38BssSramBackup"))) 
#define PARTITION_38_SECTION_SRAM_BCK_USS        __attribute__((section(".partition38UssSramBackup"))) 
#define PARTITION_38_SECTION_SDRAM               __attribute__((section(".partition38DataSdram"))) 
#define PARTITION_38_SECTION_SDRAM_BSS           __attribute__((section(".partition38BssSdram"))) 
#define PARTITION_38_SECTION_SDRAM_USS           __attribute__((section(".partition38UssSdram"))) 


#define PARTITION_39_SECTION_CCM                 __attribute__((section(".partition39DataCcm"))) 
#define PARTITION_39_SECTION_CCM_BSS             __attribute__((section(".partition39BssCcm"))) 
#define PARTITION_39_SECTION_CCM_USS             __attribute__((section(".partition39UssCcm"))) 
#define PARTITION_39_SECTION_SRAM1               __attribute__((section(".partition39DataSram1"))) 
#define PARTITION_39_SECTION_SRAM1_BSS           __attribute__((section(".partition39BssSram1"))) 
#define PARTITION_39_SECTION_SRAM1_USS           __attribute__((section(".partition39UssSram1"))) 
#define PARTITION_39_SECTION_SRAM2               __attribute__((section(".partition39DataSram2"))) 
#define PARTITION_39_SECTION_SRAM2_BSS           __attribute__((section(".partition39BssSram2"))) 
#define PARTITION_39_SECTION_SRAM2_USS           __attribute__((section(".partition39UssSram2"))) 
#define PARTITION_39_SECTION_SRAM3               __attribute__((section(".partition39DataSram3"))) 
#define PARTITION_39_SECTION_SRAM3_BSS           __attribute__((section(".partition39BssSram3"))) 
#define PARTITION_39_SECTION_SRAM3_USS           __attribute__((section(".partition39UssSram3"))) 
#define PARTITION_39_SECTION_SRAM_BCK            __attribute__((section(".partition39DataSramBackup"))) 
#define PARTITION_39_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition39BssSramBackup"))) 
#define PARTITION_39_SECTION_SRAM_BCK_USS        __attribute__((section(".partition39UssSramBackup"))) 
#define PARTITION_39_SECTION_SDRAM               __attribute__((section(".partition39DataSdram"))) 
#define PARTITION_39_SECTION_SDRAM_BSS           __attribute__((section(".partition39BssSdram"))) 
#define PARTITION_39_SECTION_SDRAM_USS           __attribute__((section(".partition39UssSdram"))) 


#define PARTITION_40_SECTION_CCM                 __attribute__((section(".partition40DataCcm"))) 
#define PARTITION_40_SECTION_CCM_BSS             __attribute__((section(".partition40BssCcm"))) 
#define PARTITION_40_SECTION_CCM_USS             __attribute__((section(".partition40UssCcm"))) 
#define PARTITION_40_SECTION_SRAM1               __attribute__((section(".partition40DataSram1"))) 
#define PARTITION_40_SECTION_SRAM1_BSS           __attribute__((section(".partition40BssSram1"))) 
#define PARTITION_40_SECTION_SRAM1_USS           __attribute__((section(".partition40UssSram1"))) 
#define PARTITION_40_SECTION_SRAM2               __attribute__((section(".partition40DataSram2"))) 
#define PARTITION_40_SECTION_SRAM2_BSS           __attribute__((section(".partition40BssSram2"))) 
#define PARTITION_40_SECTION_SRAM2_USS           __attribute__((section(".partition40UssSram2"))) 
#define PARTITION_40_SECTION_SRAM3               __attribute__((section(".partition40DataSram3"))) 
#define PARTITION_40_SECTION_SRAM3_BSS           __attribute__((section(".partition40BssSram3"))) 
#define PARTITION_40_SECTION_SRAM3_USS           __attribute__((section(".partition40UssSram3"))) 
#define PARTITION_40_SECTION_SRAM_BCK            __attribute__((section(".partition40DataSramBackup"))) 
#define PARTITION_40_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition40BssSramBackup"))) 
#define PARTITION_40_SECTION_SRAM_BCK_USS        __attribute__((section(".partition40UssSramBackup"))) 
#define PARTITION_40_SECTION_SDRAM               __attribute__((section(".partition40DataSdram"))) 
#define PARTITION_40_SECTION_SDRAM_BSS           __attribute__((section(".partition40BssSdram"))) 
#define PARTITION_40_SECTION_SDRAM_USS           __attribute__((section(".partition40UssSdram"))) 


#define PARTITION_41_SECTION_CCM                 __attribute__((section(".partition41DataCcm"))) 
#define PARTITION_41_SECTION_CCM_BSS             __attribute__((section(".partition41BssCcm"))) 
#define PARTITION_41_SECTION_CCM_USS             __attribute__((section(".partition41UssCcm"))) 
#define PARTITION_41_SECTION_SRAM1               __attribute__((section(".partition41DataSram1"))) 
#define PARTITION_41_SECTION_SRAM1_BSS           __attribute__((section(".partition41BssSram1"))) 
#define PARTITION_41_SECTION_SRAM1_USS           __attribute__((section(".partition41UssSram1"))) 
#define PARTITION_41_SECTION_SRAM2               __attribute__((section(".partition41DataSram2"))) 
#define PARTITION_41_SECTION_SRAM2_BSS           __attribute__((section(".partition41BssSram2"))) 
#define PARTITION_41_SECTION_SRAM2_USS           __attribute__((section(".partition41UssSram2"))) 
#define PARTITION_41_SECTION_SRAM3               __attribute__((section(".partition41DataSram3"))) 
#define PARTITION_41_SECTION_SRAM3_BSS           __attribute__((section(".partition41BssSram3"))) 
#define PARTITION_41_SECTION_SRAM3_USS           __attribute__((section(".partition41UssSram3"))) 
#define PARTITION_41_SECTION_SRAM_BCK            __attribute__((section(".partition41DataSramBackup"))) 
#define PARTITION_41_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition41BssSramBackup"))) 
#define PARTITION_41_SECTION_SRAM_BCK_USS        __attribute__((section(".partition41UssSramBackup"))) 
#define PARTITION_41_SECTION_SDRAM               __attribute__((section(".partition41DataSdram"))) 
#define PARTITION_41_SECTION_SDRAM_BSS           __attribute__((section(".partition41BssSdram"))) 
#define PARTITION_41_SECTION_SDRAM_USS           __attribute__((section(".partition41UssSdram"))) 


#define PARTITION_42_SECTION_CCM                 __attribute__((section(".partition42DataCcm"))) 
#define PARTITION_42_SECTION_CCM_BSS             __attribute__((section(".partition42BssCcm"))) 
#define PARTITION_42_SECTION_CCM_USS             __attribute__((section(".partition42UssCcm"))) 
#define PARTITION_42_SECTION_SRAM1               __attribute__((section(".partition42DataSram1"))) 
#define PARTITION_42_SECTION_SRAM1_BSS           __attribute__((section(".partition42BssSram1"))) 
#define PARTITION_42_SECTION_SRAM1_USS           __attribute__((section(".partition42UssSram1"))) 
#define PARTITION_42_SECTION_SRAM2               __attribute__((section(".partition42DataSram2"))) 
#define PARTITION_42_SECTION_SRAM2_BSS           __attribute__((section(".partition42BssSram2"))) 
#define PARTITION_42_SECTION_SRAM2_USS           __attribute__((section(".partition42UssSram2"))) 
#define PARTITION_42_SECTION_SRAM3               __attribute__((section(".partition42DataSram3"))) 
#define PARTITION_42_SECTION_SRAM3_BSS           __attribute__((section(".partition42BssSram3"))) 
#define PARTITION_42_SECTION_SRAM3_USS           __attribute__((section(".partition42UssSram3"))) 
#define PARTITION_42_SECTION_SRAM_BCK            __attribute__((section(".partition42DataSramBackup"))) 
#define PARTITION_42_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition42BssSramBackup"))) 
#define PARTITION_42_SECTION_SRAM_BCK_USS        __attribute__((section(".partition42UssSramBackup"))) 
#define PARTITION_42_SECTION_SDRAM               __attribute__((section(".partition42DataSdram"))) 
#define PARTITION_42_SECTION_SDRAM_BSS           __attribute__((section(".partition42BssSdram"))) 
#define PARTITION_42_SECTION_SDRAM_USS           __attribute__((section(".partition42UssSdram"))) 


#define PARTITION_43_SECTION_CCM                 __attribute__((section(".partition43DataCcm"))) 
#define PARTITION_43_SECTION_CCM_BSS             __attribute__((section(".partition43BssCcm"))) 
#define PARTITION_43_SECTION_CCM_USS             __attribute__((section(".partition43UssCcm"))) 
#define PARTITION_43_SECTION_SRAM1               __attribute__((section(".partition43DataSram1"))) 
#define PARTITION_43_SECTION_SRAM1_BSS           __attribute__((section(".partition43BssSram1"))) 
#define PARTITION_43_SECTION_SRAM1_USS           __attribute__((section(".partition43UssSram1"))) 
#define PARTITION_43_SECTION_SRAM2               __attribute__((section(".partition43DataSram2"))) 
#define PARTITION_43_SECTION_SRAM2_BSS           __attribute__((section(".partition43BssSram2"))) 
#define PARTITION_43_SECTION_SRAM2_USS           __attribute__((section(".partition43UssSram2"))) 
#define PARTITION_43_SECTION_SRAM3               __attribute__((section(".partition43DataSram3"))) 
#define PARTITION_43_SECTION_SRAM3_BSS           __attribute__((section(".partition43BssSram3"))) 
#define PARTITION_43_SECTION_SRAM3_USS           __attribute__((section(".partition43UssSram3"))) 
#define PARTITION_43_SECTION_SRAM_BCK            __attribute__((section(".partition43DataSramBackup"))) 
#define PARTITION_43_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition43BssSramBackup"))) 
#define PARTITION_43_SECTION_SRAM_BCK_USS        __attribute__((section(".partition43UssSramBackup"))) 
#define PARTITION_43_SECTION_SDRAM               __attribute__((section(".partition43DataSdram"))) 
#define PARTITION_43_SECTION_SDRAM_BSS           __attribute__((section(".partition43BssSdram"))) 
#define PARTITION_43_SECTION_SDRAM_USS           __attribute__((section(".partition43UssSdram"))) 


#define PARTITION_44_SECTION_CCM                 __attribute__((section(".partition44DataCcm"))) 
#define PARTITION_44_SECTION_CCM_BSS             __attribute__((section(".partition44BssCcm"))) 
#define PARTITION_44_SECTION_CCM_USS             __attribute__((section(".partition44UssCcm"))) 
#define PARTITION_44_SECTION_SRAM1               __attribute__((section(".partition44DataSram1"))) 
#define PARTITION_44_SECTION_SRAM1_BSS           __attribute__((section(".partition44BssSram1"))) 
#define PARTITION_44_SECTION_SRAM1_USS           __attribute__((section(".partition44UssSram1"))) 
#define PARTITION_44_SECTION_SRAM2               __attribute__((section(".partition44DataSram2"))) 
#define PARTITION_44_SECTION_SRAM2_BSS           __attribute__((section(".partition44BssSram2"))) 
#define PARTITION_44_SECTION_SRAM2_USS           __attribute__((section(".partition44UssSram2"))) 
#define PARTITION_44_SECTION_SRAM3               __attribute__((section(".partition44DataSram3"))) 
#define PARTITION_44_SECTION_SRAM3_BSS           __attribute__((section(".partition44BssSram3"))) 
#define PARTITION_44_SECTION_SRAM3_USS           __attribute__((section(".partition44UssSram3"))) 
#define PARTITION_44_SECTION_SRAM_BCK            __attribute__((section(".partition44DataSramBackup"))) 
#define PARTITION_44_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition44BssSramBackup"))) 
#define PARTITION_44_SECTION_SRAM_BCK_USS        __attribute__((section(".partition44UssSramBackup"))) 
#define PARTITION_44_SECTION_SDRAM               __attribute__((section(".partition44DataSdram"))) 
#define PARTITION_44_SECTION_SDRAM_BSS           __attribute__((section(".partition44BssSdram"))) 
#define PARTITION_44_SECTION_SDRAM_USS           __attribute__((section(".partition44UssSdram"))) 


#define PARTITION_45_SECTION_CCM                 __attribute__((section(".partition45DataCcm"))) 
#define PARTITION_45_SECTION_CCM_BSS             __attribute__((section(".partition45BssCcm"))) 
#define PARTITION_45_SECTION_CCM_USS             __attribute__((section(".partition45UssCcm"))) 
#define PARTITION_45_SECTION_SRAM1               __attribute__((section(".partition45DataSram1"))) 
#define PARTITION_45_SECTION_SRAM1_BSS           __attribute__((section(".partition45BssSram1"))) 
#define PARTITION_45_SECTION_SRAM1_USS           __attribute__((section(".partition45UssSram1"))) 
#define PARTITION_45_SECTION_SRAM2               __attribute__((section(".partition45DataSram2"))) 
#define PARTITION_45_SECTION_SRAM2_BSS           __attribute__((section(".partition45BssSram2"))) 
#define PARTITION_45_SECTION_SRAM2_USS           __attribute__((section(".partition45UssSram2"))) 
#define PARTITION_45_SECTION_SRAM3               __attribute__((section(".partition45DataSram3"))) 
#define PARTITION_45_SECTION_SRAM3_BSS           __attribute__((section(".partition45BssSram3"))) 
#define PARTITION_45_SECTION_SRAM3_USS           __attribute__((section(".partition45UssSram3"))) 
#define PARTITION_45_SECTION_SRAM_BCK            __attribute__((section(".partition45DataSramBackup"))) 
#define PARTITION_45_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition45BssSramBackup"))) 
#define PARTITION_45_SECTION_SRAM_BCK_USS        __attribute__((section(".partition45UssSramBackup"))) 
#define PARTITION_45_SECTION_SDRAM               __attribute__((section(".partition45DataSdram"))) 
#define PARTITION_45_SECTION_SDRAM_BSS           __attribute__((section(".partition45BssSdram"))) 
#define PARTITION_45_SECTION_SDRAM_USS           __attribute__((section(".partition45UssSdram"))) 


#define PARTITION_46_SECTION_CCM                 __attribute__((section(".partition46DataCcm"))) 
#define PARTITION_46_SECTION_CCM_BSS             __attribute__((section(".partition46BssCcm"))) 
#define PARTITION_46_SECTION_CCM_USS             __attribute__((section(".partition46UssCcm"))) 
#define PARTITION_46_SECTION_SRAM1               __attribute__((section(".partition46DataSram1"))) 
#define PARTITION_46_SECTION_SRAM1_BSS           __attribute__((section(".partition46BssSram1"))) 
#define PARTITION_46_SECTION_SRAM1_USS           __attribute__((section(".partition46UssSram1"))) 
#define PARTITION_46_SECTION_SRAM2               __attribute__((section(".partition46DataSram2"))) 
#define PARTITION_46_SECTION_SRAM2_BSS           __attribute__((section(".partition46BssSram2"))) 
#define PARTITION_46_SECTION_SRAM2_USS           __attribute__((section(".partition46UssSram2"))) 
#define PARTITION_46_SECTION_SRAM3               __attribute__((section(".partition46DataSram3"))) 
#define PARTITION_46_SECTION_SRAM3_BSS           __attribute__((section(".partition46BssSram3"))) 
#define PARTITION_46_SECTION_SRAM3_USS           __attribute__((section(".partition46UssSram3"))) 
#define PARTITION_46_SECTION_SRAM_BCK            __attribute__((section(".partition46DataSramBackup"))) 
#define PARTITION_46_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition46BssSramBackup"))) 
#define PARTITION_46_SECTION_SRAM_BCK_USS        __attribute__((section(".partition46UssSramBackup"))) 
#define PARTITION_46_SECTION_SDRAM               __attribute__((section(".partition46DataSdram"))) 
#define PARTITION_46_SECTION_SDRAM_BSS           __attribute__((section(".partition46BssSdram"))) 
#define PARTITION_46_SECTION_SDRAM_USS           __attribute__((section(".partition46UssSdram"))) 


#define PARTITION_47_SECTION_CCM                 __attribute__((section(".partition47DataCcm"))) 
#define PARTITION_47_SECTION_CCM_BSS             __attribute__((section(".partition47BssCcm"))) 
#define PARTITION_47_SECTION_CCM_USS             __attribute__((section(".partition47UssCcm"))) 
#define PARTITION_47_SECTION_SRAM1               __attribute__((section(".partition47DataSram1"))) 
#define PARTITION_47_SECTION_SRAM1_BSS           __attribute__((section(".partition47BssSram1"))) 
#define PARTITION_47_SECTION_SRAM1_USS           __attribute__((section(".partition47UssSram1"))) 
#define PARTITION_47_SECTION_SRAM2               __attribute__((section(".partition47DataSram2"))) 
#define PARTITION_47_SECTION_SRAM2_BSS           __attribute__((section(".partition47BssSram2"))) 
#define PARTITION_47_SECTION_SRAM2_USS           __attribute__((section(".partition47UssSram2"))) 
#define PARTITION_47_SECTION_SRAM3               __attribute__((section(".partition47DataSram3"))) 
#define PARTITION_47_SECTION_SRAM3_BSS           __attribute__((section(".partition47BssSram3"))) 
#define PARTITION_47_SECTION_SRAM3_USS           __attribute__((section(".partition47UssSram3"))) 
#define PARTITION_47_SECTION_SRAM_BCK            __attribute__((section(".partition47DataSramBackup"))) 
#define PARTITION_47_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition47BssSramBackup"))) 
#define PARTITION_47_SECTION_SRAM_BCK_USS        __attribute__((section(".partition47UssSramBackup"))) 
#define PARTITION_47_SECTION_SDRAM               __attribute__((section(".partition47DataSdram"))) 
#define PARTITION_47_SECTION_SDRAM_BSS           __attribute__((section(".partition47BssSdram"))) 
#define PARTITION_47_SECTION_SDRAM_USS           __attribute__((section(".partition47UssSdram"))) 


#define PARTITION_48_SECTION_CCM                 __attribute__((section(".partition48DataCcm"))) 
#define PARTITION_48_SECTION_CCM_BSS             __attribute__((section(".partition48BssCcm"))) 
#define PARTITION_48_SECTION_CCM_USS             __attribute__((section(".partition48UssCcm"))) 
#define PARTITION_48_SECTION_SRAM1               __attribute__((section(".partition48DataSram1"))) 
#define PARTITION_48_SECTION_SRAM1_BSS           __attribute__((section(".partition48BssSram1"))) 
#define PARTITION_48_SECTION_SRAM1_USS           __attribute__((section(".partition48UssSram1"))) 
#define PARTITION_48_SECTION_SRAM2               __attribute__((section(".partition48DataSram2"))) 
#define PARTITION_48_SECTION_SRAM2_BSS           __attribute__((section(".partition48BssSram2"))) 
#define PARTITION_48_SECTION_SRAM2_USS           __attribute__((section(".partition48UssSram2"))) 
#define PARTITION_48_SECTION_SRAM3               __attribute__((section(".partition48DataSram3"))) 
#define PARTITION_48_SECTION_SRAM3_BSS           __attribute__((section(".partition48BssSram3"))) 
#define PARTITION_48_SECTION_SRAM3_USS           __attribute__((section(".partition48UssSram3"))) 
#define PARTITION_48_SECTION_SRAM_BCK            __attribute__((section(".partition48DataSramBackup"))) 
#define PARTITION_48_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition48BssSramBackup"))) 
#define PARTITION_48_SECTION_SRAM_BCK_USS        __attribute__((section(".partition48UssSramBackup"))) 
#define PARTITION_48_SECTION_SDRAM               __attribute__((section(".partition48DataSdram"))) 
#define PARTITION_48_SECTION_SDRAM_BSS           __attribute__((section(".partition48BssSdram"))) 
#define PARTITION_48_SECTION_SDRAM_USS           __attribute__((section(".partition48UssSdram"))) 


#define PARTITION_49_SECTION_CCM                 __attribute__((section(".partition49DataCcm"))) 
#define PARTITION_49_SECTION_CCM_BSS             __attribute__((section(".partition49BssCcm"))) 
#define PARTITION_49_SECTION_CCM_USS             __attribute__((section(".partition49UssCcm"))) 
#define PARTITION_49_SECTION_SRAM1               __attribute__((section(".partition49DataSram1"))) 
#define PARTITION_49_SECTION_SRAM1_BSS           __attribute__((section(".partition49BssSram1"))) 
#define PARTITION_49_SECTION_SRAM1_USS           __attribute__((section(".partition49UssSram1"))) 
#define PARTITION_49_SECTION_SRAM2               __attribute__((section(".partition49DataSram2"))) 
#define PARTITION_49_SECTION_SRAM2_BSS           __attribute__((section(".partition49BssSram2"))) 
#define PARTITION_49_SECTION_SRAM2_USS           __attribute__((section(".partition49UssSram2"))) 
#define PARTITION_49_SECTION_SRAM3               __attribute__((section(".partition49DataSram3"))) 
#define PARTITION_49_SECTION_SRAM3_BSS           __attribute__((section(".partition49BssSram3"))) 
#define PARTITION_49_SECTION_SRAM3_USS           __attribute__((section(".partition49UssSram3"))) 
#define PARTITION_49_SECTION_SRAM_BCK            __attribute__((section(".partition49DataSramBackup"))) 
#define PARTITION_49_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition49BssSramBackup"))) 
#define PARTITION_49_SECTION_SRAM_BCK_USS        __attribute__((section(".partition49UssSramBackup"))) 
#define PARTITION_49_SECTION_SDRAM               __attribute__((section(".partition49DataSdram"))) 
#define PARTITION_49_SECTION_SDRAM_BSS           __attribute__((section(".partition49BssSdram"))) 
#define PARTITION_49_SECTION_SDRAM_USS           __attribute__((section(".partition49UssSdram"))) 


#define PARTITION_50_SECTION_CCM                 __attribute__((section(".partition50DataCcm"))) 
#define PARTITION_50_SECTION_CCM_BSS             __attribute__((section(".partition50BssCcm"))) 
#define PARTITION_50_SECTION_CCM_USS             __attribute__((section(".partition50UssCcm"))) 
#define PARTITION_50_SECTION_SRAM1               __attribute__((section(".partition50DataSram1"))) 
#define PARTITION_50_SECTION_SRAM1_BSS           __attribute__((section(".partition50BssSram1"))) 
#define PARTITION_50_SECTION_SRAM1_USS           __attribute__((section(".partition50UssSram1"))) 
#define PARTITION_50_SECTION_SRAM2               __attribute__((section(".partition50DataSram2"))) 
#define PARTITION_50_SECTION_SRAM2_BSS           __attribute__((section(".partition50BssSram2"))) 
#define PARTITION_50_SECTION_SRAM2_USS           __attribute__((section(".partition50UssSram2"))) 
#define PARTITION_50_SECTION_SRAM3               __attribute__((section(".partition50DataSram3"))) 
#define PARTITION_50_SECTION_SRAM3_BSS           __attribute__((section(".partition50BssSram3"))) 
#define PARTITION_50_SECTION_SRAM3_USS           __attribute__((section(".partition50UssSram3"))) 
#define PARTITION_50_SECTION_SRAM_BCK            __attribute__((section(".partition50DataSramBackup"))) 
#define PARTITION_50_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition50BssSramBackup"))) 
#define PARTITION_50_SECTION_SRAM_BCK_USS        __attribute__((section(".partition50UssSramBackup"))) 
#define PARTITION_50_SECTION_SDRAM               __attribute__((section(".partition50DataSdram"))) 
#define PARTITION_50_SECTION_SDRAM_BSS           __attribute__((section(".partition50BssSdram"))) 
#define PARTITION_50_SECTION_SDRAM_USS           __attribute__((section(".partition50UssSdram"))) 


#define PARTITION_51_SECTION_CCM                 __attribute__((section(".partition51DataCcm"))) 
#define PARTITION_51_SECTION_CCM_BSS             __attribute__((section(".partition51BssCcm"))) 
#define PARTITION_51_SECTION_CCM_USS             __attribute__((section(".partition51UssCcm"))) 
#define PARTITION_51_SECTION_SRAM1               __attribute__((section(".partition51DataSram1"))) 
#define PARTITION_51_SECTION_SRAM1_BSS           __attribute__((section(".partition51BssSram1"))) 
#define PARTITION_51_SECTION_SRAM1_USS           __attribute__((section(".partition51UssSram1"))) 
#define PARTITION_51_SECTION_SRAM2               __attribute__((section(".partition51DataSram2"))) 
#define PARTITION_51_SECTION_SRAM2_BSS           __attribute__((section(".partition51BssSram2"))) 
#define PARTITION_51_SECTION_SRAM2_USS           __attribute__((section(".partition51UssSram2"))) 
#define PARTITION_51_SECTION_SRAM3               __attribute__((section(".partition51DataSram3"))) 
#define PARTITION_51_SECTION_SRAM3_BSS           __attribute__((section(".partition51BssSram3"))) 
#define PARTITION_51_SECTION_SRAM3_USS           __attribute__((section(".partition51UssSram3"))) 
#define PARTITION_51_SECTION_SRAM_BCK            __attribute__((section(".partition51DataSramBackup"))) 
#define PARTITION_51_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition51BssSramBackup"))) 
#define PARTITION_51_SECTION_SRAM_BCK_USS        __attribute__((section(".partition51UssSramBackup"))) 
#define PARTITION_51_SECTION_SDRAM               __attribute__((section(".partition51DataSdram"))) 
#define PARTITION_51_SECTION_SDRAM_BSS           __attribute__((section(".partition51BssSdram"))) 
#define PARTITION_51_SECTION_SDRAM_USS           __attribute__((section(".partition51UssSdram"))) 


#define PARTITION_52_SECTION_CCM                 __attribute__((section(".partition52DataCcm"))) 
#define PARTITION_52_SECTION_CCM_BSS             __attribute__((section(".partition52BssCcm"))) 
#define PARTITION_52_SECTION_CCM_USS             __attribute__((section(".partition52UssCcm"))) 
#define PARTITION_52_SECTION_SRAM1               __attribute__((section(".partition52DataSram1"))) 
#define PARTITION_52_SECTION_SRAM1_BSS           __attribute__((section(".partition52BssSram1"))) 
#define PARTITION_52_SECTION_SRAM1_USS           __attribute__((section(".partition52UssSram1"))) 
#define PARTITION_52_SECTION_SRAM2               __attribute__((section(".partition52DataSram2"))) 
#define PARTITION_52_SECTION_SRAM2_BSS           __attribute__((section(".partition52BssSram2"))) 
#define PARTITION_52_SECTION_SRAM2_USS           __attribute__((section(".partition52UssSram2"))) 
#define PARTITION_52_SECTION_SRAM3               __attribute__((section(".partition52DataSram3"))) 
#define PARTITION_52_SECTION_SRAM3_BSS           __attribute__((section(".partition52BssSram3"))) 
#define PARTITION_52_SECTION_SRAM3_USS           __attribute__((section(".partition52UssSram3"))) 
#define PARTITION_52_SECTION_SRAM_BCK            __attribute__((section(".partition52DataSramBackup"))) 
#define PARTITION_52_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition52BssSramBackup"))) 
#define PARTITION_52_SECTION_SRAM_BCK_USS        __attribute__((section(".partition52UssSramBackup"))) 
#define PARTITION_52_SECTION_SDRAM               __attribute__((section(".partition52DataSdram"))) 
#define PARTITION_52_SECTION_SDRAM_BSS           __attribute__((section(".partition52BssSdram"))) 
#define PARTITION_52_SECTION_SDRAM_USS           __attribute__((section(".partition52UssSdram"))) 


#define PARTITION_53_SECTION_CCM                 __attribute__((section(".partition53DataCcm"))) 
#define PARTITION_53_SECTION_CCM_BSS             __attribute__((section(".partition53BssCcm"))) 
#define PARTITION_53_SECTION_CCM_USS             __attribute__((section(".partition53UssCcm"))) 
#define PARTITION_53_SECTION_SRAM1               __attribute__((section(".partition53DataSram1"))) 
#define PARTITION_53_SECTION_SRAM1_BSS           __attribute__((section(".partition53BssSram1"))) 
#define PARTITION_53_SECTION_SRAM1_USS           __attribute__((section(".partition53UssSram1"))) 
#define PARTITION_53_SECTION_SRAM2               __attribute__((section(".partition53DataSram2"))) 
#define PARTITION_53_SECTION_SRAM2_BSS           __attribute__((section(".partition53BssSram2"))) 
#define PARTITION_53_SECTION_SRAM2_USS           __attribute__((section(".partition53UssSram2"))) 
#define PARTITION_53_SECTION_SRAM3               __attribute__((section(".partition53DataSram3"))) 
#define PARTITION_53_SECTION_SRAM3_BSS           __attribute__((section(".partition53BssSram3"))) 
#define PARTITION_53_SECTION_SRAM3_USS           __attribute__((section(".partition53UssSram3"))) 
#define PARTITION_53_SECTION_SRAM_BCK            __attribute__((section(".partition53DataSramBackup"))) 
#define PARTITION_53_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition53BssSramBackup"))) 
#define PARTITION_53_SECTION_SRAM_BCK_USS        __attribute__((section(".partition53UssSramBackup"))) 
#define PARTITION_53_SECTION_SDRAM               __attribute__((section(".partition53DataSdram"))) 
#define PARTITION_53_SECTION_SDRAM_BSS           __attribute__((section(".partition53BssSdram"))) 
#define PARTITION_53_SECTION_SDRAM_USS           __attribute__((section(".partition53UssSdram"))) 


#define PARTITION_54_SECTION_CCM                 __attribute__((section(".partition54DataCcm"))) 
#define PARTITION_54_SECTION_CCM_BSS             __attribute__((section(".partition54BssCcm"))) 
#define PARTITION_54_SECTION_CCM_USS             __attribute__((section(".partition54UssCcm"))) 
#define PARTITION_54_SECTION_SRAM1               __attribute__((section(".partition54DataSram1"))) 
#define PARTITION_54_SECTION_SRAM1_BSS           __attribute__((section(".partition54BssSram1"))) 
#define PARTITION_54_SECTION_SRAM1_USS           __attribute__((section(".partition54UssSram1"))) 
#define PARTITION_54_SECTION_SRAM2               __attribute__((section(".partition54DataSram2"))) 
#define PARTITION_54_SECTION_SRAM2_BSS           __attribute__((section(".partition54BssSram2"))) 
#define PARTITION_54_SECTION_SRAM2_USS           __attribute__((section(".partition54UssSram2"))) 
#define PARTITION_54_SECTION_SRAM3               __attribute__((section(".partition54DataSram3"))) 
#define PARTITION_54_SECTION_SRAM3_BSS           __attribute__((section(".partition54BssSram3"))) 
#define PARTITION_54_SECTION_SRAM3_USS           __attribute__((section(".partition54UssSram3"))) 
#define PARTITION_54_SECTION_SRAM_BCK            __attribute__((section(".partition54DataSramBackup"))) 
#define PARTITION_54_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition54BssSramBackup"))) 
#define PARTITION_54_SECTION_SRAM_BCK_USS        __attribute__((section(".partition54UssSramBackup"))) 
#define PARTITION_54_SECTION_SDRAM               __attribute__((section(".partition54DataSdram"))) 
#define PARTITION_54_SECTION_SDRAM_BSS           __attribute__((section(".partition54BssSdram"))) 
#define PARTITION_54_SECTION_SDRAM_USS           __attribute__((section(".partition54UssSdram"))) 


#define PARTITION_55_SECTION_CCM                 __attribute__((section(".partition55DataCcm"))) 
#define PARTITION_55_SECTION_CCM_BSS             __attribute__((section(".partition55BssCcm"))) 
#define PARTITION_55_SECTION_CCM_USS             __attribute__((section(".partition55UssCcm"))) 
#define PARTITION_55_SECTION_SRAM1               __attribute__((section(".partition55DataSram1"))) 
#define PARTITION_55_SECTION_SRAM1_BSS           __attribute__((section(".partition55BssSram1"))) 
#define PARTITION_55_SECTION_SRAM1_USS           __attribute__((section(".partition55UssSram1"))) 
#define PARTITION_55_SECTION_SRAM2               __attribute__((section(".partition55DataSram2"))) 
#define PARTITION_55_SECTION_SRAM2_BSS           __attribute__((section(".partition55BssSram2"))) 
#define PARTITION_55_SECTION_SRAM2_USS           __attribute__((section(".partition55UssSram2"))) 
#define PARTITION_55_SECTION_SRAM3               __attribute__((section(".partition55DataSram3"))) 
#define PARTITION_55_SECTION_SRAM3_BSS           __attribute__((section(".partition55BssSram3"))) 
#define PARTITION_55_SECTION_SRAM3_USS           __attribute__((section(".partition55UssSram3"))) 
#define PARTITION_55_SECTION_SRAM_BCK            __attribute__((section(".partition55DataSramBackup"))) 
#define PARTITION_55_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition55BssSramBackup"))) 
#define PARTITION_55_SECTION_SRAM_BCK_USS        __attribute__((section(".partition55UssSramBackup"))) 
#define PARTITION_55_SECTION_SDRAM               __attribute__((section(".partition55DataSdram"))) 
#define PARTITION_55_SECTION_SDRAM_BSS           __attribute__((section(".partition55BssSdram"))) 
#define PARTITION_55_SECTION_SDRAM_USS           __attribute__((section(".partition55UssSdram"))) 


#define PARTITION_56_SECTION_CCM                 __attribute__((section(".partition56DataCcm"))) 
#define PARTITION_56_SECTION_CCM_BSS             __attribute__((section(".partition56BssCcm"))) 
#define PARTITION_56_SECTION_CCM_USS             __attribute__((section(".partition56UssCcm"))) 
#define PARTITION_56_SECTION_SRAM1               __attribute__((section(".partition56DataSram1"))) 
#define PARTITION_56_SECTION_SRAM1_BSS           __attribute__((section(".partition56BssSram1"))) 
#define PARTITION_56_SECTION_SRAM1_USS           __attribute__((section(".partition56UssSram1"))) 
#define PARTITION_56_SECTION_SRAM2               __attribute__((section(".partition56DataSram2"))) 
#define PARTITION_56_SECTION_SRAM2_BSS           __attribute__((section(".partition56BssSram2"))) 
#define PARTITION_56_SECTION_SRAM2_USS           __attribute__((section(".partition56UssSram2"))) 
#define PARTITION_56_SECTION_SRAM3               __attribute__((section(".partition56DataSram3"))) 
#define PARTITION_56_SECTION_SRAM3_BSS           __attribute__((section(".partition56BssSram3"))) 
#define PARTITION_56_SECTION_SRAM3_USS           __attribute__((section(".partition56UssSram3"))) 
#define PARTITION_56_SECTION_SRAM_BCK            __attribute__((section(".partition56DataSramBackup"))) 
#define PARTITION_56_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition56BssSramBackup"))) 
#define PARTITION_56_SECTION_SRAM_BCK_USS        __attribute__((section(".partition56UssSramBackup"))) 
#define PARTITION_56_SECTION_SDRAM               __attribute__((section(".partition56DataSdram"))) 
#define PARTITION_56_SECTION_SDRAM_BSS           __attribute__((section(".partition56BssSdram"))) 
#define PARTITION_56_SECTION_SDRAM_USS           __attribute__((section(".partition56UssSdram"))) 


#define PARTITION_57_SECTION_CCM                 __attribute__((section(".partition57DataCcm"))) 
#define PARTITION_57_SECTION_CCM_BSS             __attribute__((section(".partition57BssCcm"))) 
#define PARTITION_57_SECTION_CCM_USS             __attribute__((section(".partition57UssCcm"))) 
#define PARTITION_57_SECTION_SRAM1               __attribute__((section(".partition57DataSram1"))) 
#define PARTITION_57_SECTION_SRAM1_BSS           __attribute__((section(".partition57BssSram1"))) 
#define PARTITION_57_SECTION_SRAM1_USS           __attribute__((section(".partition57UssSram1"))) 
#define PARTITION_57_SECTION_SRAM2               __attribute__((section(".partition57DataSram2"))) 
#define PARTITION_57_SECTION_SRAM2_BSS           __attribute__((section(".partition57BssSram2"))) 
#define PARTITION_57_SECTION_SRAM2_USS           __attribute__((section(".partition57UssSram2"))) 
#define PARTITION_57_SECTION_SRAM3               __attribute__((section(".partition57DataSram3"))) 
#define PARTITION_57_SECTION_SRAM3_BSS           __attribute__((section(".partition57BssSram3"))) 
#define PARTITION_57_SECTION_SRAM3_USS           __attribute__((section(".partition57UssSram3"))) 
#define PARTITION_57_SECTION_SRAM_BCK            __attribute__((section(".partition57DataSramBackup"))) 
#define PARTITION_57_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition57BssSramBackup"))) 
#define PARTITION_57_SECTION_SRAM_BCK_USS        __attribute__((section(".partition57UssSramBackup"))) 
#define PARTITION_57_SECTION_SDRAM               __attribute__((section(".partition57DataSdram"))) 
#define PARTITION_57_SECTION_SDRAM_BSS           __attribute__((section(".partition57BssSdram"))) 
#define PARTITION_57_SECTION_SDRAM_USS           __attribute__((section(".partition57UssSdram"))) 


#define PARTITION_58_SECTION_CCM                 __attribute__((section(".partition58DataCcm"))) 
#define PARTITION_58_SECTION_CCM_BSS             __attribute__((section(".partition58BssCcm"))) 
#define PARTITION_58_SECTION_CCM_USS             __attribute__((section(".partition58UssCcm"))) 
#define PARTITION_58_SECTION_SRAM1               __attribute__((section(".partition58DataSram1"))) 
#define PARTITION_58_SECTION_SRAM1_BSS           __attribute__((section(".partition58BssSram1"))) 
#define PARTITION_58_SECTION_SRAM1_USS           __attribute__((section(".partition58UssSram1"))) 
#define PARTITION_58_SECTION_SRAM2               __attribute__((section(".partition58DataSram2"))) 
#define PARTITION_58_SECTION_SRAM2_BSS           __attribute__((section(".partition58BssSram2"))) 
#define PARTITION_58_SECTION_SRAM2_USS           __attribute__((section(".partition58UssSram2"))) 
#define PARTITION_58_SECTION_SRAM3               __attribute__((section(".partition58DataSram3"))) 
#define PARTITION_58_SECTION_SRAM3_BSS           __attribute__((section(".partition58BssSram3"))) 
#define PARTITION_58_SECTION_SRAM3_USS           __attribute__((section(".partition58UssSram3"))) 
#define PARTITION_58_SECTION_SRAM_BCK            __attribute__((section(".partition58DataSramBackup"))) 
#define PARTITION_58_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition58BssSramBackup"))) 
#define PARTITION_58_SECTION_SRAM_BCK_USS        __attribute__((section(".partition58UssSramBackup"))) 
#define PARTITION_58_SECTION_SDRAM               __attribute__((section(".partition58DataSdram"))) 
#define PARTITION_58_SECTION_SDRAM_BSS           __attribute__((section(".partition58BssSdram"))) 
#define PARTITION_58_SECTION_SDRAM_USS           __attribute__((section(".partition58UssSdram"))) 


#define PARTITION_59_SECTION_CCM                 __attribute__((section(".partition59DataCcm"))) 
#define PARTITION_59_SECTION_CCM_BSS             __attribute__((section(".partition59BssCcm"))) 
#define PARTITION_59_SECTION_CCM_USS             __attribute__((section(".partition59UssCcm"))) 
#define PARTITION_59_SECTION_SRAM1               __attribute__((section(".partition59DataSram1"))) 
#define PARTITION_59_SECTION_SRAM1_BSS           __attribute__((section(".partition59BssSram1"))) 
#define PARTITION_59_SECTION_SRAM1_USS           __attribute__((section(".partition59UssSram1"))) 
#define PARTITION_59_SECTION_SRAM2               __attribute__((section(".partition59DataSram2"))) 
#define PARTITION_59_SECTION_SRAM2_BSS           __attribute__((section(".partition59BssSram2"))) 
#define PARTITION_59_SECTION_SRAM2_USS           __attribute__((section(".partition59UssSram2"))) 
#define PARTITION_59_SECTION_SRAM3               __attribute__((section(".partition59DataSram3"))) 
#define PARTITION_59_SECTION_SRAM3_BSS           __attribute__((section(".partition59BssSram3"))) 
#define PARTITION_59_SECTION_SRAM3_USS           __attribute__((section(".partition59UssSram3"))) 
#define PARTITION_59_SECTION_SRAM_BCK            __attribute__((section(".partition59DataSramBackup"))) 
#define PARTITION_59_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition59BssSramBackup"))) 
#define PARTITION_59_SECTION_SRAM_BCK_USS        __attribute__((section(".partition59UssSramBackup"))) 
#define PARTITION_59_SECTION_SDRAM               __attribute__((section(".partition59DataSdram"))) 
#define PARTITION_59_SECTION_SDRAM_BSS           __attribute__((section(".partition59BssSdram"))) 
#define PARTITION_59_SECTION_SDRAM_USS           __attribute__((section(".partition59UssSdram"))) 


#define PARTITION_60_SECTION_CCM                 __attribute__((section(".partition60DataCcm"))) 
#define PARTITION_60_SECTION_CCM_BSS             __attribute__((section(".partition60BssCcm"))) 
#define PARTITION_60_SECTION_CCM_USS             __attribute__((section(".partition60UssCcm"))) 
#define PARTITION_60_SECTION_SRAM1               __attribute__((section(".partition60DataSram1"))) 
#define PARTITION_60_SECTION_SRAM1_BSS           __attribute__((section(".partition60BssSram1"))) 
#define PARTITION_60_SECTION_SRAM1_USS           __attribute__((section(".partition60UssSram1"))) 
#define PARTITION_60_SECTION_SRAM2               __attribute__((section(".partition60DataSram2"))) 
#define PARTITION_60_SECTION_SRAM2_BSS           __attribute__((section(".partition60BssSram2"))) 
#define PARTITION_60_SECTION_SRAM2_USS           __attribute__((section(".partition60UssSram2"))) 
#define PARTITION_60_SECTION_SRAM3               __attribute__((section(".partition60DataSram3"))) 
#define PARTITION_60_SECTION_SRAM3_BSS           __attribute__((section(".partition60BssSram3"))) 
#define PARTITION_60_SECTION_SRAM3_USS           __attribute__((section(".partition60UssSram3"))) 
#define PARTITION_60_SECTION_SRAM_BCK            __attribute__((section(".partition60DataSramBackup"))) 
#define PARTITION_60_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition60BssSramBackup"))) 
#define PARTITION_60_SECTION_SRAM_BCK_USS        __attribute__((section(".partition60UssSramBackup"))) 
#define PARTITION_60_SECTION_SDRAM               __attribute__((section(".partition60DataSdram"))) 
#define PARTITION_60_SECTION_SDRAM_BSS           __attribute__((section(".partition60BssSdram"))) 
#define PARTITION_60_SECTION_SDRAM_USS           __attribute__((section(".partition60UssSdram"))) 


#define PARTITION_61_SECTION_CCM                 __attribute__((section(".partition61DataCcm"))) 
#define PARTITION_61_SECTION_CCM_BSS             __attribute__((section(".partition61BssCcm"))) 
#define PARTITION_61_SECTION_CCM_USS             __attribute__((section(".partition61UssCcm"))) 
#define PARTITION_61_SECTION_SRAM1               __attribute__((section(".partition61DataSram1"))) 
#define PARTITION_61_SECTION_SRAM1_BSS           __attribute__((section(".partition61BssSram1"))) 
#define PARTITION_61_SECTION_SRAM1_USS           __attribute__((section(".partition61UssSram1"))) 
#define PARTITION_61_SECTION_SRAM2               __attribute__((section(".partition61DataSram2"))) 
#define PARTITION_61_SECTION_SRAM2_BSS           __attribute__((section(".partition61BssSram2"))) 
#define PARTITION_61_SECTION_SRAM2_USS           __attribute__((section(".partition61UssSram2"))) 
#define PARTITION_61_SECTION_SRAM3               __attribute__((section(".partition61DataSram3"))) 
#define PARTITION_61_SECTION_SRAM3_BSS           __attribute__((section(".partition61BssSram3"))) 
#define PARTITION_61_SECTION_SRAM3_USS           __attribute__((section(".partition61UssSram3"))) 
#define PARTITION_61_SECTION_SRAM_BCK            __attribute__((section(".partition61DataSramBackup"))) 
#define PARTITION_61_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition61BssSramBackup"))) 
#define PARTITION_61_SECTION_SRAM_BCK_USS        __attribute__((section(".partition61UssSramBackup"))) 
#define PARTITION_61_SECTION_SDRAM               __attribute__((section(".partition61DataSdram"))) 
#define PARTITION_61_SECTION_SDRAM_BSS           __attribute__((section(".partition61BssSdram"))) 
#define PARTITION_61_SECTION_SDRAM_USS           __attribute__((section(".partition61UssSdram"))) 


#define PARTITION_62_SECTION_CCM                 __attribute__((section(".partition62DataCcm"))) 
#define PARTITION_62_SECTION_CCM_BSS             __attribute__((section(".partition62BssCcm"))) 
#define PARTITION_62_SECTION_CCM_USS             __attribute__((section(".partition62UssCcm"))) 
#define PARTITION_62_SECTION_SRAM1               __attribute__((section(".partition62DataSram1"))) 
#define PARTITION_62_SECTION_SRAM1_BSS           __attribute__((section(".partition62BssSram1"))) 
#define PARTITION_62_SECTION_SRAM1_USS           __attribute__((section(".partition62UssSram1"))) 
#define PARTITION_62_SECTION_SRAM2               __attribute__((section(".partition62DataSram2"))) 
#define PARTITION_62_SECTION_SRAM2_BSS           __attribute__((section(".partition62BssSram2"))) 
#define PARTITION_62_SECTION_SRAM2_USS           __attribute__((section(".partition62UssSram2"))) 
#define PARTITION_62_SECTION_SRAM3               __attribute__((section(".partition62DataSram3"))) 
#define PARTITION_62_SECTION_SRAM3_BSS           __attribute__((section(".partition62BssSram3"))) 
#define PARTITION_62_SECTION_SRAM3_USS           __attribute__((section(".partition62UssSram3"))) 
#define PARTITION_62_SECTION_SRAM_BCK            __attribute__((section(".partition62DataSramBackup"))) 
#define PARTITION_62_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition62BssSramBackup"))) 
#define PARTITION_62_SECTION_SRAM_BCK_USS        __attribute__((section(".partition62UssSramBackup"))) 
#define PARTITION_62_SECTION_SDRAM               __attribute__((section(".partition62DataSdram"))) 
#define PARTITION_62_SECTION_SDRAM_BSS           __attribute__((section(".partition62BssSdram"))) 
#define PARTITION_62_SECTION_SDRAM_USS           __attribute__((section(".partition62UssSdram"))) 


#define PARTITION_63_SECTION_CCM                 __attribute__((section(".partition63DataCcm"))) 
#define PARTITION_63_SECTION_CCM_BSS             __attribute__((section(".partition63BssCcm"))) 
#define PARTITION_63_SECTION_CCM_USS             __attribute__((section(".partition63UssCcm"))) 
#define PARTITION_63_SECTION_SRAM1               __attribute__((section(".partition63DataSram1"))) 
#define PARTITION_63_SECTION_SRAM1_BSS           __attribute__((section(".partition63BssSram1"))) 
#define PARTITION_63_SECTION_SRAM1_USS           __attribute__((section(".partition63UssSram1"))) 
#define PARTITION_63_SECTION_SRAM2               __attribute__((section(".partition63DataSram2"))) 
#define PARTITION_63_SECTION_SRAM2_BSS           __attribute__((section(".partition63BssSram2"))) 
#define PARTITION_63_SECTION_SRAM2_USS           __attribute__((section(".partition63UssSram2"))) 
#define PARTITION_63_SECTION_SRAM3               __attribute__((section(".partition63DataSram3"))) 
#define PARTITION_63_SECTION_SRAM3_BSS           __attribute__((section(".partition63BssSram3"))) 
#define PARTITION_63_SECTION_SRAM3_USS           __attribute__((section(".partition63UssSram3"))) 
#define PARTITION_63_SECTION_SRAM_BCK            __attribute__((section(".partition63DataSramBackup"))) 
#define PARTITION_63_SECTION_SRAM_BCK_BSS        __attribute__((section(".partition63BssSramBackup"))) 
#define PARTITION_63_SECTION_SRAM_BCK_USS        __attribute__((section(".partition63UssSramBackup"))) 
#define PARTITION_63_SECTION_SDRAM               __attribute__((section(".partition63DataSdram"))) 
#define PARTITION_63_SECTION_SDRAM_BSS           __attribute__((section(".partition63BssSdram"))) 
#define PARTITION_63_SECTION_SDRAM_USS           __attribute__((section(".partition63UssSdram"))) 


/**
 * Default Kernel data section (for initialized data)
 */
#define KERNEL_SECTION              KERNEL_SECTION_SRAM1

/**
 * Default Kernel data section (for initialized data to zero)
 */
#define KERNEL_SECTION_BSS          KERNEL_SECTION_SRAM1_BSS

/**
 * Default Kernel data section (for non-initialized data)
 */
#define KERNEL_SECTION_USS          KERNEL_SECTION_SRAM1_USS 

/**
 * Bulk section. Channel bulks should be placed on this section to ensure that no partition writes to them
 */
#define BULK_SECTION                BULK_SECTION_SRAM1

/**
 * Default Shared data section (for initialized data)
 */
#define SHARED_SECTION              SHARED_SECTION_SRAM1

/**
 * Default Shared data section (for initialized data to zero)
 */
#define SHARED_SECTION_BSS          SHARED_SECTION_SRAM1_BSS

/**
 * Default Shared data section (for non-initialized data)
 */
#define SHARED_SECTION_USS          SHARED_SECTION_SRAM1_USS



/**
 * Partitions data section
 */
/* Default section for partition 0 data section (on SRAM1) */
#define PARTITION_0_SECTION         PARTITION_0_SECTION_SRAM1
/* Default section for partition 0 bss section (on SRAM1) */
#define PARTITION_0_SECTION_BSS     PARTITION_0_SECTION_SRAM1_BSS
/* Default section for partition 0 uss section (on SRAM1) */
#define PARTITION_0_SECTION_USS     PARTITION_0_SECTION_SRAM1_USS

/* Default section for partition 1 data section (on SRAM1) */
#define PARTITION_1_SECTION         PARTITION_1_SECTION_SRAM1
/* Default section for partition 1 bss section (on SRAM1) */
#define PARTITION_1_SECTION_BSS     PARTITION_1_SECTION_SRAM1_BSS
/* Default section for partition 1 uss section (on SRAM1) */
#define PARTITION_1_SECTION_USS     PARTITION_1_SECTION_SRAM1_USS

/* Default section for partition 2 data section (on SRAM1) */
#define PARTITION_2_SECTION         PARTITION_2_SECTION_SRAM1
/* Default section for partition 2 bss section (on SRAM1) */
#define PARTITION_2_SECTION_BSS     PARTITION_2_SECTION_SRAM1_BSS
/* Default section for partition 2 uss section (on SRAM1) */
#define PARTITION_2_SECTION_USS     PARTITION_2_SECTION_SRAM1_USS

/* Default section for partition 3 data section (on SRAM1) */
#define PARTITION_3_SECTION         PARTITION_3_SECTION_SRAM1
/* Default section for partition 3 bss section (on SRAM1) */
#define PARTITION_3_SECTION_BSS     PARTITION_3_SECTION_SRAM1_BSS
/* Default section for partition 3 uss section (on SRAM1) */
#define PARTITION_3_SECTION_USS     PARTITION_3_SECTION_SRAM1_USS

/* Default section for partition 4 data section (on SRAM1) */
#define PARTITION_4_SECTION         PARTITION_4_SECTION_SRAM1
/* Default section for partition 4 bss section (on SRAM1) */
#define PARTITION_4_SECTION_BSS     PARTITION_4_SECTION_SRAM1_BSS
/* Default section for partition 4 uss section (on SRAM1) */
#define PARTITION_4_SECTION_USS     PARTITION_4_SECTION_SRAM1_USS

/* Default section for partition 5 data section (on SRAM1) */
#define PARTITION_5_SECTION         PARTITION_5_SECTION_SRAM1
/* Default section for partition 5 bss section (on SRAM1) */
#define PARTITION_5_SECTION_BSS     PARTITION_5_SECTION_SRAM1_BSS
/* Default section for partition 5 uss section (on SRAM1) */
#define PARTITION_5_SECTION_USS     PARTITION_5_SECTION_SRAM1_USS

/* Default section for partition 6 data section (on SRAM1) */
#define PARTITION_6_SECTION         PARTITION_6_SECTION_SRAM1
/* Default section for partition 6 bss section (on SRAM1) */
#define PARTITION_6_SECTION_BSS     PARTITION_6_SECTION_SRAM1_BSS
/* Default section for partition 6 uss section (on SRAM1) */
#define PARTITION_6_SECTION_USS     PARTITION_6_SECTION_SRAM1_USS

/* Default section for partition 7 data section (on SRAM1) */
#define PARTITION_7_SECTION         PARTITION_7_SECTION_SRAM1
/* Default section for partition 7 bss section (on SRAM1) */
#define PARTITION_7_SECTION_BSS     PARTITION_7_SECTION_SRAM1_BSS
/* Default section for partition 7 uss section (on SRAM1) */
#define PARTITION_7_SECTION_USS     PARTITION_7_SECTION_SRAM1_USS

/* Default section for partition 8 data section (on SRAM1) */
#define PARTITION_8_SECTION         PARTITION_8_SECTION_SRAM1
/* Default section for partition 8 bss section (on SRAM1) */
#define PARTITION_8_SECTION_BSS     PARTITION_8_SECTION_SRAM1_BSS
/* Default section for partition 8 uss section (on SRAM1) */
#define PARTITION_8_SECTION_USS     PARTITION_8_SECTION_SRAM1_USS

/* Default section for partition 9 data section (on SRAM1) */
#define PARTITION_9_SECTION         PARTITION_9_SECTION_SRAM1
/* Default section for partition 9 bss section (on SRAM1) */
#define PARTITION_9_SECTION_BSS     PARTITION_9_SECTION_SRAM1_BSS
/* Default section for partition 9 uss section (on SRAM1) */
#define PARTITION_9_SECTION_USS     PARTITION_9_SECTION_SRAM1_USS

/* Default section for partition 10 data section (on SRAM1) */
#define PARTITION_10_SECTION         PARTITION_10_SECTION_SRAM1
/* Default section for partition 10 bss section (on SRAM1) */
#define PARTITION_10_SECTION_BSS     PARTITION_10_SECTION_SRAM1_BSS
/* Default section for partition 10 uss section (on SRAM1) */
#define PARTITION_10_SECTION_USS     PARTITION_10_SECTION_SRAM1_USS

/* Default section for partition 11 data section (on SRAM1) */
#define PARTITION_11_SECTION         PARTITION_11_SECTION_SRAM1
/* Default section for partition 11 bss section (on SRAM1) */
#define PARTITION_11_SECTION_BSS     PARTITION_11_SECTION_SRAM1_BSS
/* Default section for partition 11 uss section (on SRAM1) */
#define PARTITION_11_SECTION_USS     PARTITION_11_SECTION_SRAM1_USS

/* Default section for partition 12 data section (on SRAM1) */
#define PARTITION_12_SECTION         PARTITION_12_SECTION_SRAM1
/* Default section for partition 12 bss section (on SRAM1) */
#define PARTITION_12_SECTION_BSS     PARTITION_12_SECTION_SRAM1_BSS
/* Default section for partition 12 uss section (on SRAM1) */
#define PARTITION_12_SECTION_USS     PARTITION_12_SECTION_SRAM1_USS

/* Default section for partition 13 data section (on SRAM1) */
#define PARTITION_13_SECTION         PARTITION_13_SECTION_SRAM1
/* Default section for partition 13 bss section (on SRAM1) */
#define PARTITION_13_SECTION_BSS     PARTITION_13_SECTION_SRAM1_BSS
/* Default section for partition 13 uss section (on SRAM1) */
#define PARTITION_13_SECTION_USS     PARTITION_13_SECTION_SRAM1_USS

/* Default section for partition 14 data section (on SRAM1) */
#define PARTITION_14_SECTION         PARTITION_14_SECTION_SRAM1
/* Default section for partition 14 bss section (on SRAM1) */
#define PARTITION_14_SECTION_BSS     PARTITION_14_SECTION_SRAM1_BSS
/* Default section for partition 14 uss section (on SRAM1) */
#define PARTITION_14_SECTION_USS     PARTITION_14_SECTION_SRAM1_USS

/* Default section for partition 15 data section (on SRAM1) */
#define PARTITION_15_SECTION         PARTITION_15_SECTION_SRAM1
/* Default section for partition 15 bss section (on SRAM1) */
#define PARTITION_15_SECTION_BSS     PARTITION_15_SECTION_SRAM1_BSS
/* Default section for partition 15 uss section (on SRAM1) */
#define PARTITION_15_SECTION_USS     PARTITION_15_SECTION_SRAM1_USS

/* Default section for partition 16 data section (on SRAM1) */
#define PARTITION_16_SECTION         PARTITION_16_SECTION_SRAM1
/* Default section for partition 16 bss section (on SRAM1) */
#define PARTITION_16_SECTION_BSS     PARTITION_16_SECTION_SRAM1_BSS
/* Default section for partition 16 uss section (on SRAM1) */
#define PARTITION_16_SECTION_USS     PARTITION_16_SECTION_SRAM1_USS

/* Default section for partition 17 data section (on SRAM1) */
#define PARTITION_17_SECTION         PARTITION_17_SECTION_SRAM1
/* Default section for partition 17 bss section (on SRAM1) */
#define PARTITION_17_SECTION_BSS     PARTITION_17_SECTION_SRAM1_BSS
/* Default section for partition 17 uss section (on SRAM1) */
#define PARTITION_17_SECTION_USS     PARTITION_17_SECTION_SRAM1_USS

/* Default section for partition 18 data section (on SRAM1) */
#define PARTITION_18_SECTION         PARTITION_18_SECTION_SRAM1
/* Default section for partition 18 bss section (on SRAM1) */
#define PARTITION_18_SECTION_BSS     PARTITION_18_SECTION_SRAM1_BSS
/* Default section for partition 18 uss section (on SRAM1) */
#define PARTITION_18_SECTION_USS     PARTITION_18_SECTION_SRAM1_USS

/* Default section for partition 19 data section (on SRAM1) */
#define PARTITION_19_SECTION         PARTITION_19_SECTION_SRAM1
/* Default section for partition 19 bss section (on SRAM1) */
#define PARTITION_19_SECTION_BSS     PARTITION_19_SECTION_SRAM1_BSS
/* Default section for partition 19 uss section (on SRAM1) */
#define PARTITION_19_SECTION_USS     PARTITION_19_SECTION_SRAM1_USS

/* Default section for partition 20 data section (on SRAM1) */
#define PARTITION_20_SECTION         PARTITION_20_SECTION_SRAM1
/* Default section for partition 20 bss section (on SRAM1) */
#define PARTITION_20_SECTION_BSS     PARTITION_20_SECTION_SRAM1_BSS
/* Default section for partition 20 uss section (on SRAM1) */
#define PARTITION_20_SECTION_USS     PARTITION_20_SECTION_SRAM1_USS

/* Default section for partition 21 data section (on SRAM1) */
#define PARTITION_21_SECTION         PARTITION_21_SECTION_SRAM1
/* Default section for partition 21 bss section (on SRAM1) */
#define PARTITION_21_SECTION_BSS     PARTITION_21_SECTION_SRAM1_BSS
/* Default section for partition 21 uss section (on SRAM1) */
#define PARTITION_21_SECTION_USS     PARTITION_21_SECTION_SRAM1_USS

/* Default section for partition 22 data section (on SRAM1) */
#define PARTITION_22_SECTION         PARTITION_22_SECTION_SRAM1
/* Default section for partition 22 bss section (on SRAM1) */
#define PARTITION_22_SECTION_BSS     PARTITION_22_SECTION_SRAM1_BSS
/* Default section for partition 22 uss section (on SRAM1) */
#define PARTITION_22_SECTION_USS     PARTITION_22_SECTION_SRAM1_USS

/* Default section for partition 23 data section (on SRAM1) */
#define PARTITION_23_SECTION         PARTITION_23_SECTION_SRAM1
/* Default section for partition 23 bss section (on SRAM1) */
#define PARTITION_23_SECTION_BSS     PARTITION_23_SECTION_SRAM1_BSS
/* Default section for partition 23 uss section (on SRAM1) */
#define PARTITION_23_SECTION_USS     PARTITION_23_SECTION_SRAM1_USS

/* Default section for partition 24 data section (on SRAM1) */
#define PARTITION_24_SECTION         PARTITION_24_SECTION_SRAM1
/* Default section for partition 24 bss section (on SRAM1) */
#define PARTITION_24_SECTION_BSS     PARTITION_24_SECTION_SRAM1_BSS
/* Default section for partition 24 uss section (on SRAM1) */
#define PARTITION_24_SECTION_USS     PARTITION_24_SECTION_SRAM1_USS

/* Default section for partition 25 data section (on SRAM1) */
#define PARTITION_25_SECTION         PARTITION_25_SECTION_SRAM1
/* Default section for partition 25 bss section (on SRAM1) */
#define PARTITION_25_SECTION_BSS     PARTITION_25_SECTION_SRAM1_BSS
/* Default section for partition 25 uss section (on SRAM1) */
#define PARTITION_25_SECTION_USS     PARTITION_25_SECTION_SRAM1_USS

/* Default section for partition 26 data section (on SRAM1) */
#define PARTITION_26_SECTION         PARTITION_26_SECTION_SRAM1
/* Default section for partition 26 bss section (on SRAM1) */
#define PARTITION_26_SECTION_BSS     PARTITION_26_SECTION_SRAM1_BSS
/* Default section for partition 26 uss section (on SRAM1) */
#define PARTITION_26_SECTION_USS     PARTITION_26_SECTION_SRAM1_USS

/* Default section for partition 27 data section (on SRAM1) */
#define PARTITION_27_SECTION         PARTITION_27_SECTION_SRAM1
/* Default section for partition 27 bss section (on SRAM1) */
#define PARTITION_27_SECTION_BSS     PARTITION_27_SECTION_SRAM1_BSS
/* Default section for partition 27 uss section (on SRAM1) */
#define PARTITION_27_SECTION_USS     PARTITION_27_SECTION_SRAM1_USS

/* Default section for partition 28 data section (on SRAM1) */
#define PARTITION_28_SECTION         PARTITION_28_SECTION_SRAM1
/* Default section for partition 28 bss section (on SRAM1) */
#define PARTITION_28_SECTION_BSS     PARTITION_28_SECTION_SRAM1_BSS
/* Default section for partition 28 uss section (on SRAM1) */
#define PARTITION_28_SECTION_USS     PARTITION_28_SECTION_SRAM1_USS

/* Default section for partition 29 data section (on SRAM1) */
#define PARTITION_29_SECTION         PARTITION_29_SECTION_SRAM1
/* Default section for partition 29 bss section (on SRAM1) */
#define PARTITION_29_SECTION_BSS     PARTITION_29_SECTION_SRAM1_BSS
/* Default section for partition 29 uss section (on SRAM1) */
#define PARTITION_29_SECTION_USS     PARTITION_29_SECTION_SRAM1_USS

/* Default section for partition 30 data section (on SRAM1) */
#define PARTITION_30_SECTION         PARTITION_30_SECTION_SRAM1
/* Default section for partition 30 bss section (on SRAM1) */
#define PARTITION_30_SECTION_BSS     PARTITION_30_SECTION_SRAM1_BSS
/* Default section for partition 30 uss section (on SRAM1) */
#define PARTITION_30_SECTION_USS     PARTITION_30_SECTION_SRAM1_USS

/* Default section for partition 31 data section (on SRAM1) */
#define PARTITION_31_SECTION         PARTITION_31_SECTION_SRAM1
/* Default section for partition 31 bss section (on SRAM1) */
#define PARTITION_31_SECTION_BSS     PARTITION_31_SECTION_SRAM1_BSS
/* Default section for partition 31 uss section (on SRAM1) */
#define PARTITION_31_SECTION_USS     PARTITION_31_SECTION_SRAM1_USS

/* Default section for partition 32 data section (on SRAM1) */
#define PARTITION_32_SECTION         PARTITION_32_SECTION_SRAM1
/* Default section for partition 32 bss section (on SRAM1) */
#define PARTITION_32_SECTION_BSS     PARTITION_32_SECTION_SRAM1_BSS
/* Default section for partition 32 uss section (on SRAM1) */
#define PARTITION_32_SECTION_USS     PARTITION_32_SECTION_SRAM1_USS

/* Default section for partition 33 data section (on SRAM1) */
#define PARTITION_33_SECTION         PARTITION_33_SECTION_SRAM1
/* Default section for partition 33 bss section (on SRAM1) */
#define PARTITION_33_SECTION_BSS     PARTITION_33_SECTION_SRAM1_BSS
/* Default section for partition 33 uss section (on SRAM1) */
#define PARTITION_33_SECTION_USS     PARTITION_33_SECTION_SRAM1_USS

/* Default section for partition 34 data section (on SRAM1) */
#define PARTITION_34_SECTION         PARTITION_34_SECTION_SRAM1
/* Default section for partition 34 bss section (on SRAM1) */
#define PARTITION_34_SECTION_BSS     PARTITION_34_SECTION_SRAM1_BSS
/* Default section for partition 34 uss section (on SRAM1) */
#define PARTITION_34_SECTION_USS     PARTITION_34_SECTION_SRAM1_USS

/* Default section for partition 35 data section (on SRAM1) */
#define PARTITION_35_SECTION         PARTITION_35_SECTION_SRAM1
/* Default section for partition 35 bss section (on SRAM1) */
#define PARTITION_35_SECTION_BSS     PARTITION_35_SECTION_SRAM1_BSS
/* Default section for partition 35 uss section (on SRAM1) */
#define PARTITION_35_SECTION_USS     PARTITION_35_SECTION_SRAM1_USS

/* Default section for partition 36 data section (on SRAM1) */
#define PARTITION_36_SECTION         PARTITION_36_SECTION_SRAM1
/* Default section for partition 36 bss section (on SRAM1) */
#define PARTITION_36_SECTION_BSS     PARTITION_36_SECTION_SRAM1_BSS
/* Default section for partition 36 uss section (on SRAM1) */
#define PARTITION_36_SECTION_USS     PARTITION_36_SECTION_SRAM1_USS

/* Default section for partition 37 data section (on SRAM1) */
#define PARTITION_37_SECTION         PARTITION_37_SECTION_SRAM1
/* Default section for partition 37 bss section (on SRAM1) */
#define PARTITION_37_SECTION_BSS     PARTITION_37_SECTION_SRAM1_BSS
/* Default section for partition 37 uss section (on SRAM1) */
#define PARTITION_37_SECTION_USS     PARTITION_37_SECTION_SRAM1_USS

/* Default section for partition 38 data section (on SRAM1) */
#define PARTITION_38_SECTION         PARTITION_38_SECTION_SRAM1
/* Default section for partition 38 bss section (on SRAM1) */
#define PARTITION_38_SECTION_BSS     PARTITION_38_SECTION_SRAM1_BSS
/* Default section for partition 38 uss section (on SRAM1) */
#define PARTITION_38_SECTION_USS     PARTITION_38_SECTION_SRAM1_USS

/* Default section for partition 39 data section (on SRAM1) */
#define PARTITION_39_SECTION         PARTITION_39_SECTION_SRAM1
/* Default section for partition 39 bss section (on SRAM1) */
#define PARTITION_39_SECTION_BSS     PARTITION_39_SECTION_SRAM1_BSS
/* Default section for partition 39 uss section (on SRAM1) */
#define PARTITION_39_SECTION_USS     PARTITION_39_SECTION_SRAM1_USS

/* Default section for partition 40 data section (on SRAM1) */
#define PARTITION_40_SECTION         PARTITION_40_SECTION_SRAM1
/* Default section for partition 40 bss section (on SRAM1) */
#define PARTITION_40_SECTION_BSS     PARTITION_40_SECTION_SRAM1_BSS
/* Default section for partition 40 uss section (on SRAM1) */
#define PARTITION_40_SECTION_USS     PARTITION_40_SECTION_SRAM1_USS

/* Default section for partition 41 data section (on SRAM1) */
#define PARTITION_41_SECTION         PARTITION_41_SECTION_SRAM1
/* Default section for partition 41 bss section (on SRAM1) */
#define PARTITION_41_SECTION_BSS     PARTITION_41_SECTION_SRAM1_BSS
/* Default section for partition 41 uss section (on SRAM1) */
#define PARTITION_41_SECTION_USS     PARTITION_41_SECTION_SRAM1_USS

/* Default section for partition 42 data section (on SRAM1) */
#define PARTITION_42_SECTION         PARTITION_42_SECTION_SRAM1
/* Default section for partition 42 bss section (on SRAM1) */
#define PARTITION_42_SECTION_BSS     PARTITION_42_SECTION_SRAM1_BSS
/* Default section for partition 42 uss section (on SRAM1) */
#define PARTITION_42_SECTION_USS     PARTITION_42_SECTION_SRAM1_USS

/* Default section for partition 43 data section (on SRAM1) */
#define PARTITION_43_SECTION         PARTITION_43_SECTION_SRAM1
/* Default section for partition 43 bss section (on SRAM1) */
#define PARTITION_43_SECTION_BSS     PARTITION_43_SECTION_SRAM1_BSS
/* Default section for partition 43 uss section (on SRAM1) */
#define PARTITION_43_SECTION_USS     PARTITION_43_SECTION_SRAM1_USS

/* Default section for partition 44 data section (on SRAM1) */
#define PARTITION_44_SECTION         PARTITION_44_SECTION_SRAM1
/* Default section for partition 44 bss section (on SRAM1) */
#define PARTITION_44_SECTION_BSS     PARTITION_44_SECTION_SRAM1_BSS
/* Default section for partition 44 uss section (on SRAM1) */
#define PARTITION_44_SECTION_USS     PARTITION_44_SECTION_SRAM1_USS

/* Default section for partition 45 data section (on SRAM1) */
#define PARTITION_45_SECTION         PARTITION_45_SECTION_SRAM1
/* Default section for partition 45 bss section (on SRAM1) */
#define PARTITION_45_SECTION_BSS     PARTITION_45_SECTION_SRAM1_BSS
/* Default section for partition 45 uss section (on SRAM1) */
#define PARTITION_45_SECTION_USS     PARTITION_45_SECTION_SRAM1_USS

/* Default section for partition 46 data section (on SRAM1) */
#define PARTITION_46_SECTION         PARTITION_46_SECTION_SRAM1
/* Default section for partition 46 bss section (on SRAM1) */
#define PARTITION_46_SECTION_BSS     PARTITION_46_SECTION_SRAM1_BSS
/* Default section for partition 46 uss section (on SRAM1) */
#define PARTITION_46_SECTION_USS     PARTITION_46_SECTION_SRAM1_USS

/* Default section for partition 47 data section (on SRAM1) */
#define PARTITION_47_SECTION         PARTITION_47_SECTION_SRAM1
/* Default section for partition 47 bss section (on SRAM1) */
#define PARTITION_47_SECTION_BSS     PARTITION_47_SECTION_SRAM1_BSS
/* Default section for partition 47 uss section (on SRAM1) */
#define PARTITION_47_SECTION_USS     PARTITION_47_SECTION_SRAM1_USS

/* Default section for partition 48 data section (on SRAM1) */
#define PARTITION_48_SECTION         PARTITION_48_SECTION_SRAM1
/* Default section for partition 48 bss section (on SRAM1) */
#define PARTITION_48_SECTION_BSS     PARTITION_48_SECTION_SRAM1_BSS
/* Default section for partition 48 uss section (on SRAM1) */
#define PARTITION_48_SECTION_USS     PARTITION_48_SECTION_SRAM1_USS

/* Default section for partition 49 data section (on SRAM1) */
#define PARTITION_49_SECTION         PARTITION_49_SECTION_SRAM1
/* Default section for partition 49 bss section (on SRAM1) */
#define PARTITION_49_SECTION_BSS     PARTITION_49_SECTION_SRAM1_BSS
/* Default section for partition 49 uss section (on SRAM1) */
#define PARTITION_49_SECTION_USS     PARTITION_49_SECTION_SRAM1_USS

/* Default section for partition 50 data section (on SRAM1) */
#define PARTITION_50_SECTION         PARTITION_50_SECTION_SRAM1
/* Default section for partition 50 bss section (on SRAM1) */
#define PARTITION_50_SECTION_BSS     PARTITION_50_SECTION_SRAM1_BSS
/* Default section for partition 50 uss section (on SRAM1) */
#define PARTITION_50_SECTION_USS     PARTITION_50_SECTION_SRAM1_USS

/* Default section for partition 51 data section (on SRAM1) */
#define PARTITION_51_SECTION         PARTITION_51_SECTION_SRAM1
/* Default section for partition 51 bss section (on SRAM1) */
#define PARTITION_51_SECTION_BSS     PARTITION_51_SECTION_SRAM1_BSS
/* Default section for partition 51 uss section (on SRAM1) */
#define PARTITION_51_SECTION_USS     PARTITION_51_SECTION_SRAM1_USS

/* Default section for partition 52 data section (on SRAM1) */
#define PARTITION_52_SECTION         PARTITION_52_SECTION_SRAM1
/* Default section for partition 52 bss section (on SRAM1) */
#define PARTITION_52_SECTION_BSS     PARTITION_52_SECTION_SRAM1_BSS
/* Default section for partition 52 uss section (on SRAM1) */
#define PARTITION_52_SECTION_USS     PARTITION_52_SECTION_SRAM1_USS

/* Default section for partition 53 data section (on SRAM1) */
#define PARTITION_53_SECTION         PARTITION_53_SECTION_SRAM1
/* Default section for partition 53 bss section (on SRAM1) */
#define PARTITION_53_SECTION_BSS     PARTITION_53_SECTION_SRAM1_BSS
/* Default section for partition 53 uss section (on SRAM1) */
#define PARTITION_53_SECTION_USS     PARTITION_53_SECTION_SRAM1_USS

/* Default section for partition 54 data section (on SRAM1) */
#define PARTITION_54_SECTION         PARTITION_54_SECTION_SRAM1
/* Default section for partition 54 bss section (on SRAM1) */
#define PARTITION_54_SECTION_BSS     PARTITION_54_SECTION_SRAM1_BSS
/* Default section for partition 54 uss section (on SRAM1) */
#define PARTITION_54_SECTION_USS     PARTITION_54_SECTION_SRAM1_USS

/* Default section for partition 55 data section (on SRAM1) */
#define PARTITION_55_SECTION         PARTITION_55_SECTION_SRAM1
/* Default section for partition 55 bss section (on SRAM1) */
#define PARTITION_55_SECTION_BSS     PARTITION_55_SECTION_SRAM1_BSS
/* Default section for partition 55 uss section (on SRAM1) */
#define PARTITION_55_SECTION_USS     PARTITION_55_SECTION_SRAM1_USS

/* Default section for partition 56 data section (on SRAM1) */
#define PARTITION_56_SECTION         PARTITION_56_SECTION_SRAM1
/* Default section for partition 56 bss section (on SRAM1) */
#define PARTITION_56_SECTION_BSS     PARTITION_56_SECTION_SRAM1_BSS
/* Default section for partition 56 uss section (on SRAM1) */
#define PARTITION_56_SECTION_USS     PARTITION_56_SECTION_SRAM1_USS

/* Default section for partition 57 data section (on SRAM1) */
#define PARTITION_57_SECTION         PARTITION_57_SECTION_SRAM1
/* Default section for partition 57 bss section (on SRAM1) */
#define PARTITION_57_SECTION_BSS     PARTITION_57_SECTION_SRAM1_BSS
/* Default section for partition 57 uss section (on SRAM1) */
#define PARTITION_57_SECTION_USS     PARTITION_57_SECTION_SRAM1_USS

/* Default section for partition 58 data section (on SRAM1) */
#define PARTITION_58_SECTION         PARTITION_58_SECTION_SRAM1
/* Default section for partition 58 bss section (on SRAM1) */
#define PARTITION_58_SECTION_BSS     PARTITION_58_SECTION_SRAM1_BSS
/* Default section for partition 58 uss section (on SRAM1) */
#define PARTITION_58_SECTION_USS     PARTITION_58_SECTION_SRAM1_USS

/* Default section for partition 59 data section (on SRAM1) */
#define PARTITION_59_SECTION         PARTITION_59_SECTION_SRAM1
/* Default section for partition 59 bss section (on SRAM1) */
#define PARTITION_59_SECTION_BSS     PARTITION_59_SECTION_SRAM1_BSS
/* Default section for partition 59 uss section (on SRAM1) */
#define PARTITION_59_SECTION_USS     PARTITION_59_SECTION_SRAM1_USS

/* Default section for partition 60 data section (on SRAM1) */
#define PARTITION_60_SECTION         PARTITION_60_SECTION_SRAM1
/* Default section for partition 60 bss section (on SRAM1) */
#define PARTITION_60_SECTION_BSS     PARTITION_60_SECTION_SRAM1_BSS
/* Default section for partition 60 uss section (on SRAM1) */
#define PARTITION_60_SECTION_USS     PARTITION_60_SECTION_SRAM1_USS

/* Default section for partition 61 data section (on SRAM1) */
#define PARTITION_61_SECTION         PARTITION_61_SECTION_SRAM1
/* Default section for partition 61 bss section (on SRAM1) */
#define PARTITION_61_SECTION_BSS     PARTITION_61_SECTION_SRAM1_BSS
/* Default section for partition 61 uss section (on SRAM1) */
#define PARTITION_61_SECTION_USS     PARTITION_61_SECTION_SRAM1_USS

/* Default section for partition 62 data section (on SRAM1) */
#define PARTITION_62_SECTION         PARTITION_62_SECTION_SRAM1
/* Default section for partition 62 bss section (on SRAM1) */
#define PARTITION_62_SECTION_BSS     PARTITION_62_SECTION_SRAM1_BSS
/* Default section for partition 62 uss section (on SRAM1) */
#define PARTITION_62_SECTION_USS     PARTITION_62_SECTION_SRAM1_USS

/* Default section for partition 63 data section (on SRAM1) */
#define PARTITION_63_SECTION         PARTITION_63_SECTION_SRAM1
/* Default section for partition 63 bss section (on SRAM1) */
#define PARTITION_63_SECTION_BSS     PARTITION_63_SECTION_SRAM1_BSS
/* Default section for partition 63 uss section (on SRAM1) */
#define PARTITION_63_SECTION_USS     PARTITION_63_SECTION_SRAM1_USS


#define SHARED_STACK_SECTION        SHARED_STACK_SECTION_SRAM1


/* PHAROS_CPU_CONF_H */
#endif	
