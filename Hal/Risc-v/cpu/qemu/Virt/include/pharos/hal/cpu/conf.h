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
#define PHAROS_CPU_CONF_H



#include <pharos/hal/def.h>
#include <pharos/hal/cpu/archconf.h>

/**
 * CPU Frequency (1 GHz) in MHz
 */
#define PHAROS_CPU_FREQ                         (VALUE_1K)


/**
 * Mtimer Frequency in MHz (10 MHz)
 */
#define PHAROS_CPU_MTIMER_FREQ                  (10U)


/**
 * Macro to convert the microseconds into number of cycles of the mtimer
 */
#define PHAROS_CPU_MICRO_2_TIMER(x)             ((x) * PHAROS_CPU_MTIMER_FREQ)


/**
 * This CPU has floating point
 */
#define PHAROS_CPU_FP


/**
 * CPU has a working MMU
 */
#define PHAROS_CPU_HAS_MMU


/**
 * Number of bytes per cache line
 */
#define PHAROS_CPU_CACHE_LINE_SIZE              (64U)


/**
 * Size of the PharosRmpCore structure
 */
#define PHAROS_CPU_CORE_RMP_SIZE                (PHAROS_CPU_CACHE_LINE_SIZE)


/**
 * Log 2 of the size of the PharosRmpCore structure
 */
#define PHAROS_CPU_CORE_RMP_SIZE_LOG2           (6U)


/**
 * Number of cores in the CPU
 */
#define PHAROS_NUMBER_CORES                     (4U)


/**
 * Number of memory areas of this CPU
 */
#define PHAROS_NUMBER_MEMORY_AREAS              (1U)


/**
 * Number of interrupts that the Pharos Kernel will have (on its interrupt table pharosVIsrTable)
 * accounts for the PHAROS_CPU_ISR_UART0
 */
#define PHAROS_NUMBER_INTERRUPTS                (0x35U)


/**
 * Number of different priorities of the interrupts
 */
#define PHAROS_NUMBER_INTERRUPT_PRIORITIES      (7U)


/**
 * Separated "big" interrupt stack (per hart) to support nested interrupts without stack overrun
 * Must be a power of 2 so that PHAROS_CPU_INTERRUPT_STACK_SIZE_LOG2 gives a "correct" value
 * If you change this value be sure to also change PHAROS_CPU_INTERRUPT_STACK_SIZE_LOG2.
 */
#define PHAROS_CPU_INTERRUPT_STACK_SIZE         (VALUE_64KiB)


/**
 * Logarithm value of PHAROS_CPU_INTERRUPT_STACK_SIZE. Needed to easily multiply by the stack size using only a left shift
 */
#define PHAROS_CPU_INTERRUPT_STACK_SIZE_LOG2    (16U)


/**
 * Minimum stack size on the interrupt to allow another interrupt to occur
 */
#define PHAROS_CPU_INTERRUPT_STACK_MIN_SIZE     (VALUE_1KiB)


#define PHAROS_CPU_CLINT_ADDRESS                (0x02000000U)


#define PHAROS_CPU_NS16656_ADDRESS              (0x10000000U)


#define PHAROS_CPU_PLIC_ADDRESS                 (0x0C000000U)


#define PHAROS_CPU_IO_ADDRESS                   (0x10001000)


#define PHAROS_CPU_FLASH_ADDRESS                (0x20000000)


#define PHAROS_CPU_DRAM_ADDRESS                 (0x80000000)


#define PHAROS_CPU_PCIE_MMIO_ADDRESS            (0x40000000)


#define PHAROS_CPU_PCIE_PIO_ADDRESS             (0x03000000)


#define PHAROS_CPU_PCIE_ECAM_ADDRESS            (0x30000000)


/* PHAROS_CPU_CONF_H */
#endif
