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

#include <pharos/hal/def.h>
#include <pharos/hal/v8a/conf.h>


/**
 * The CPU is used always with FPU
 */
#define PHAROS_CPU_HAS_VFP


#define PHAROS_CPU_IO_BASE                      (0x3F000000)

/**
 * Number of bytes per cache line
 */
#define PHAROS_CPU_CACHE_LINE_SIZE              (32U)

/**
 * Size of the PharosRmpCore structure
 */
#define PHAROS_CPU_CORE_RMP_SIZE                (2U * PHAROS_CPU_CACHE_LINE_SIZE)

/**
 * Log 2 of the size of the PharosRmpCore structure
 */
#define PHAROS_CPU_CORE_RMP_SIZE_LOG2           (6U)

/**
 * Number of cores in the BCM2837
 */
#define PHAROS_NUMBER_CORES                     (4U)

/**
 * Number of memory areas of this CPU (only SDRAM)
 */
#define PHAROS_NUMBER_MEMORY_AREAS              (1U)

/**
 * Address of the I/O system timer
 */
#define PHAROS_CPU_SYSTEM_TIMER_ADDRESS         (PHAROS_CPU_IO_BASE + 0x00003000U)


/**
 * Address of the interrupt controller
 */
#define PHAROS_CPU_INT_CTRL_ADDRESS             (PHAROS_CPU_IO_BASE + 0x0000b200U)


/**
 * Address of the I/O ARM timer
 */
#define PHAROS_CPU_TIMER_ADDRESS                (PHAROS_CPU_IO_BASE + 0x0000b400U)


/**
 * Address of the I/O GPIO registers
 */
#define PHAROS_CPU_GPIO_ADDRESS                 (PHAROS_CPU_IO_BASE + 0x00200000U)


/**
 * Address of the PL011 registers
 */
#define PHAROS_CPU_PL011_ADDRESS                (PHAROS_CPU_IO_BASE + 0x00201000U)


/**
 * Address of the I/O auxiliary peripheral map
 */
#define PHAROS_CPU_AUX_PER_MAP_ADDRESS          (PHAROS_CPU_IO_BASE + 0x00215000U)


#define PHAROS_CPU_REGISTERS_ADDRESS            (0x40000000U)


/**
 * Number of interrupts that the Pharos Kernel will have (on its interrupt table pharosVIsrTable)
 * goes up to PHAROS_CPU_ISR_LOCAL_TIMER
 */
#define PHAROS_NUMBER_INTERRUPTS                (12U)



/* PHAROS_CONF_H */
#endif	
