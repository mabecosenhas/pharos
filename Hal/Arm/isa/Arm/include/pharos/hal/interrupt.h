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


/* 
 * File:   interrupt.h
 *
 * Created on September 25, 2017
 */

#ifndef PHAROS_ARM_INTERRUPT_H
#define PHAROS_ARM_INTERRUPT_H

#include <pharos/hal/hal.h>
#include <pharos/hal/arm/declarations.h>
#include <pharos/hal/cpu/conf.h>


/**
 * disable interrupts (written in assembly)
 */
uint32_t pharosArmDisableInterrupts(void);

/**
 * enable interrupts (written in assembly)
 */
void pharosArmEnableInterrupts(uint32_t level);


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
    *number = pharosArmDisableInterrupts();
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
    pharosArmEnableInterrupts(*number);
}

#endif /* INTERRUPT_H */
