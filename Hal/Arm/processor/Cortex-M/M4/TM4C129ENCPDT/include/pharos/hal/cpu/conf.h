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


#ifndef PHAROS_CONF_H
#define	PHAROS_CONF_H

#include <pharos/hal/v7m/conf.h>


/**
 * CPU only has one core
 */
#define PHAROS_NUMBER_CORES                     (1U)

/**
 * CPU only has one memory area (SRAM)
 */
#define PHAROS_NUMBER_MEMORY_AREAS              (1U)

/**
 *  @brief ensure that the compiler writes memory in correct order (does not change the order of certain operations)
 */
#define PHAROS_CPU_AND_COMP_MEM_BARRIER()       __asm volatile("" ::: "memory")

/**
 * Cache line size
 */
#define PHAROS_CPU_CACHE_LINE_SIZE              (64U)

/**
 * CPU clock frequency (once the PLL is started)
 */
#define PHAROS_CPU_FREQ                         (120000000U)

/**
 * Maximum number of microseconds of systick without overloading the systick register (only has 24 bits)
 * 2^24 / 120 = 139810,13 -> the maximum "standard" multiple is 100 ms
 */
#define PHAROS_CPU_SYSTICK_MAX                  (VALUE_100K)

/**
 * Number of interrupts this CPU supports
 */
#define PHAROS_CPU_NUMBER_INTERRUPTS            (128U)

/**
 * Lowest priority on this CPU
 */
#define PHAROS_INTERRUPT_PRIORITY_LOWEST        (0x7U)

/**
 * Number of bits that define an interrupt priority
 */
#define PHAROS_CPU_INTERRUPT_PRIO_BITS          (0x3U)

/**
 * Priority bits that should be maintained
 */
#define PHAROS_CPU_INTERRUPT_PRIO_MASK          (0x1FU)

/**
 * The TM4C129ENCPDT is used always with FPU
 */
#define PHAROS_CPU_HAS_VFP

/**
 * The TM4C129ENCPDT is used always with MPU
 */
#define CPU_HAS_MPU

/**
 * The TM4C129ENCPDT number of MPU regions
 */
#define PHAROS_CPU_MPU_NUMBER_REGIONS           (8U)


/***********************************************************
 * Definition of the address for peripherals               *
 ***********************************************************/

#define PHAROS_CPU_WATCHDOG0_BASE               (0x40000000U) 
#define PHAROS_CPU_WATCHDOG1_BASE               (0x40001000U)  
#define PHAROS_CPU_GPIO_PORTA_BASE              (0x40004000U)  
#define PHAROS_CPU_GPIO_PORTB_BASE              (0x40005000U)  
#define PHAROS_CPU_GPIO_PORTC_BASE              (0x40006000U)  
#define PHAROS_CPU_GPIO_PORTD_BASE              (0x40007000U) 
#define PHAROS_CPU_SSI0_BASE                    (0x40008000U) 
#define PHAROS_CPU_SSI1_BASE                    (0x40009000U)  
#define PHAROS_CPU_SSI2_BASE                    (0x4000A000U)  
#define PHAROS_CPU_SSI3_BASE                    (0x4000B000U)  
#define PHAROS_CPU_UART0_BASE                   (0x4000C000U)  
#define PHAROS_CPU_UART1_BASE                   (0x4000D000U) 
#define PHAROS_CPU_UART2_BASE                   (0x4000E000U)  
#define PHAROS_CPU_UART3_BASE                   (0x4000F000U)  
#define PHAROS_CPU_UART4_BASE                   (0x40010000U) 
#define PHAROS_CPU_UART5_BASE                   (0x40011000U)  
#define PHAROS_CPU_UART6_BASE                   (0x40012000U)  
#define PHAROS_CPU_UART7_BASE                   (0x40013000U)  
#define PHAROS_CPU_I2C0_BASE                    (0x40020000U)  
#define PHAROS_CPU_I2C1_BASE                    (0x40021000U)  
#define PHAROS_CPU_I2C2_BASE                    (0x40022000U) 
#define PHAROS_CPU_I2C3_BASE                    (0x40023000U)  
#define PHAROS_CPU_GPIO_PORTE_BASE              (0x40024000U)  
#define PHAROS_CPU_GPIO_PORTF_BASE              (0x40025000U)  
#define PHAROS_CPU_GPIO_PORTG_BASE              (0x40026000U)  
#define PHAROS_CPU_GPIO_PORTH_BASE              (0x40027000U)  
#define PHAROS_CPU_PWM0_BASE                    (0x40028000U)  
#define PHAROS_CPU_PWM1_BASE                    (0x40029000U)  
#define PHAROS_CPU_QEI0_BASE                    (0x4002C000U)  
#define PHAROS_CPU_QEI1_BASE                    (0x4002D000U)  
#define PHAROS_CPU_WTIMER0_BASE                 (0x40036000U)  
#define PHAROS_CPU_WTIMER1_BASE                 (0x40037000U)  
#define PHAROS_CPU_ADC0_BASE                    (0x40038000U) 
#define PHAROS_CPU_ADC1_BASE                    (0x40039000U)  
#define PHAROS_CPU_COMP_BASE                    (0x4003C000U)  
#define PHAROS_CPU_GPIO_PORTJ_BASE              (0x4003D000U)  
#define PHAROS_CPU_CAN0_BASE                    (0x40040000U) 
#define PHAROS_CPU_CAN1_BASE                    (0x40041000U) 
#define PHAROS_CPU_WTIMER2_BASE                 (0x4004C000U) 
#define PHAROS_CPU_WTIMER3_BASE                 (0x4004D000U)
#define PHAROS_CPU_WTIMER4_BASE                 (0x4004E000U) 
#define PHAROS_CPU_WTIMER5_BASE                 (0x4004F000U)  
#define PHAROS_CPU_USB0_BASE                    (0x40050000U)  
#define PHAROS_CPU_GPIO_PORTA_AHB_BASE          (0x40058000U)  
#define PHAROS_CPU_GPIO_PORTB_AHB_BASE          (0x40059000U)  
#define PHAROS_CPU_GPIO_PORTC_AHB_BASE          (0x4005A000U)
#define PHAROS_CPU_GPIO_PORTD_AHB_BASE          (0x4005B000U)
#define PHAROS_CPU_GPIO_PORTE_AHB_BASE          (0x4005C000U)
#define PHAROS_CPU_GPIO_PORTF_AHB_BASE          (0x4005D000U)  
#define PHAROS_CPU_GPIO_PORTG_AHB_BASE          (0x4005E000U)  
#define PHAROS_CPU_GPIO_PORTH_AHB_BASE          (0x4005F000U)  
#define PHAROS_CPU_GPIO_PORTJ_AHB_BASE          (0x40060000U)  
#define PHAROS_CPU_GPIO_PORTK_BASE              (0x40061000U)
#define PHAROS_CPU_GPIO_PORTL_BASE              (0x40062000U)
#define PHAROS_CPU_GPIO_PORTM_BASE              (0x40063000U)
#define PHAROS_CPU_GPIO_PORTN_BASE              (0x40064000U) 
#define PHAROS_CPU_GPIO_PORTP_BASE              (0x40065000U)
#define PHAROS_CPU_GPIO_PORTQ_BASE              (0x40066000U) 
#define PHAROS_CPU_GPIO_PORTR_BASE              (0x40067000U) 
#define PHAROS_CPU_GPIO_PORTS_BASE              (0x40068000U)  
#define PHAROS_CPU_GPIO_PORTT_BASE              (0x40069000U) 
#define PHAROS_CPU_EEPROM_BASE                  (0x400AF000U) 
#define PHAROS_CPU_ONEWIRE0_BASE                (0x400B6000U) 
#define PHAROS_CPU_I2C8_BASE                    (0x400B8000U) 
#define PHAROS_CPU_I2C9_BASE                    (0x400B9000U)  
#define PHAROS_CPU_I2C4_BASE                    (0x400C0000U)
#define PHAROS_CPU_I2C5_BASE                    (0x400C1000U) 
#define PHAROS_CPU_I2C6_BASE                    (0x400C2000U)  
#define PHAROS_CPU_I2C7_BASE                    (0x400C3000U) 
#define PHAROS_CPU_EPI0_BASE                    (0x400D0000U)  
#define PHAROS_CPU_EMAC0_BASE                   (0x400EC000U)  
#define PHAROS_CPU_SYSEXC_BASE                  (0x400F9000U) 
#define PHAROS_CPU_HIB_BASE                     (0x400FC000U)  
#define PHAROS_CPU_FLASH_CTRL_BASE              (0x400FD000U)  
#define PHAROS_CPU_SYSCTL_BASE                  (0x400FE000U)  
#define PHAROS_CPU_UDMA_BASE                    (0x400FF000U)  
#define PHAROS_CPU_CCM0_BASE                    (0x44030000U) 
#define PHAROS_CPU_SHAMD5_BASE                  (0x44034000U) 
#define PHAROS_CPU_AES_BASE                     (0x44036000U) 
#define PHAROS_CPU_DES_BASE                     (0x44038000U)
#define PHAROS_CPU_LCD0_BASE                    (0x44050000U) 
#define PHAROS_CPU_ITM_BASE                     (0xE0000000U)  
#define PHAROS_CPU_DWT_BASE                     (0xE0001000U)  
#define PHAROS_CPU_FPB_BASE                     (0xE0002000U)  
#define PHAROS_CPU_TPIU_BASE                    (0xE0040000U)

#endif	/* CONF_H */
