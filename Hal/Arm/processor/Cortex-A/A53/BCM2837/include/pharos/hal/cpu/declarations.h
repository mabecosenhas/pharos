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


#ifndef PHAROS_HAL_CPU_DECLARATIONS_H
#define	PHAROS_HAL_CPU_DECLARATIONS_H

#include <pharos/hal/hal.h>

typedef struct PharosCpuGpio PharosCpuGpio , *ptrPharosCpuGpio;
typedef struct PharosCpuAuxiliaryPerMap PharosCpuAuxiliaryPerMap , *ptrPharosCpuAuxiliaryPerMap;
typedef struct CpuModeChangeContext CpuModeChangeContext , *ptrCpuModeChangeContext;
typedef struct PharosInterruptController PharosInterruptController , *ptrPharosInterruptController;
typedef struct PharosCpuSystemTimer PharosCpuSystemTimer , *ptrPharosCpuSystemTimer;
typedef struct PharosCpuTimer PharosCpuTimer , *ptrPharosCpuTimer;
typedef struct PharosCpuRegisters PharosCpuRegisters , *ptrPharosCpuRegisters;
typedef struct PharosCpuPl011 PharosCpuPl011 , *ptrPharosCpuPl011;

typedef uint32_t PharosInterruptLevel VARIABLE_ALIGNMENT_X(8);


typedef PharosInterruptLevel *ptrPharosInterruptLevel;

extern const uint32_t pharosVCpuSyscallStackSize[];


/**
 * offset of the pendable 2 register 
 */
#define PHAROS_CPU_ISR_PENDABLE2_OFFSET     (32U)


/**
 * Interrupts for the BCM2837
 * 
 * Note that the CPU supports more interrupts. Currently they are not shown since Pharos is currently used 
 * together with Raspberry PI3 board in which the GPU uses the remaining interrupts and errors could occur if the 
 * CPU also enabled those interrupts. For precaution, there are not placed here.
 * If in the future more boards use the BCM2837, all the interrupts should be placed here.
 */
typedef enum
{
    PHAROS_CPU_ISR_CNTPS = 0U ,
    PHAROS_CPU_ISR_CNTPN = 1U ,
    PHAROS_CPU_ISR_CNTHP = 2U ,
    PHAROS_CPU_ISR_CNTV = 3U ,
    PHAROS_CPU_ISR_MAILBOX0 = 4U ,
    PHAROS_CPU_ISR_MAILBOX1 = 5U ,
    PHAROS_CPU_ISR_MAILBOX2 = 6U ,
    PHAROS_CPU_ISR_MAILBOX3 = 7U ,
    PHAROS_CPU_ISR_GPU = 8U ,
    PHAROS_CPU_ISR_PMU = 9U,
    PHAROS_CPU_ISR_AXI = 10U,
    PHAROS_CPU_ISR_LOCAL_TIMER = 11U,

    /**
     * Contains the number of ISRs 
     */
    PHAROS_CPU_ISR_LAST = PHAROS_CPU_ISR_LOCAL_TIMER + 1U ,

} PharosInterruptNumber;


typedef enum
{
    /**
     * Normal interrupt priority
     */
    PHAROS_CPU_INTERRUPT_IRQ = 0 ,

    /**
     * Urgent interrupt priority.
     * At most only one interrupt can be FIQ
     */
    PHAROS_CPU_INTERRUPT_FIQ = 1 ,
} PharosInterruptPriority;


/* PHAROS_HAL_CPU_DECLARATIONS_H */
#endif	
