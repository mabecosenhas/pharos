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


#ifndef PHAROS_INTERRUPT_H
#define PHAROS_INTERRUPT_H

#include <pharos/hal/hal.h>
#include <pharos/hal/cpu/asm.h>
#include <pharos/hal/cpu/conf.h>
#include <pharos/hal/cpu/cpu.h>


typedef uint64_t PharosInterruptLevel VARIABLE_ALIGNMENT_X(8);


typedef PharosInterruptLevel *ptrPharosInterruptLevel;


extern uint8_t pharosCpuInterruptStack[PHAROS_NUMBER_CORES][PHAROS_CPU_INTERRUPT_STACK_SIZE];


/**
 * disable interrupts (written in assembly)
 */
uint64_t pharosCpuInterruptDisable(void);

/**
 * enable interrupts (written in assembly)
 */
void pharosCpuInterruptEnable(uint64_t level);


/**
 * disable interrupts. Increase interrupt level
 * 
 * @contraint number pointer must point to a valid address
 * 
 * @param number pointer where to place the previous interrupt level
 */
INLINE void pharosCpuDisableInterrupts(ptrPharosInterruptLevel number)
{
    /* make sure all memory operations are completed here */
    PHAROS_SW_MEMORY_BARRIER();

    /* disable interrupts */
    *number = pharosCpuInterruptDisable();
}


/**
 * enable interrupts. Decreases interrupt level
 * 
 * @param level interrupt level to set
 */
INLINE void pharosCpuEnableInterrupts(ptrPharosInterruptLevel number)
{
    /* make sure all memory operations are completed here */
    PHAROS_SW_MEMORY_BARRIER();

    /* enable interrupts */
    pharosCpuInterruptEnable(*number);
}


/**
 * Disable the timer interrupt 
 */
void pharosCpuMTimerInterruptDisable(void);


/**
 * Enable the timer interrupt
 */
void pharosCpuMTimerInterruptEnable(void);


/* PHAROS_INTERRUPT_H */
#endif 
