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


#include <pharos/hal/cpu/cpu.h>
#include <pharos/hal/cpu/clint.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/hal/cpu/sections.h>


/**
 * Interrupt stack
 */
KERNEL_SECTION_USS uint8_t pharosCpuInterruptStack[PHAROS_NUMBER_CORES][PHAROS_CPU_INTERRUPT_STACK_SIZE] PHAROS_ALIGN_KERNEL_STACK;


void pharosCpuMulticoreClearPending()
{
    /* clear the pending interrupt */
    pharosCpuMulticoreClearInterrupt(pharosICoreRunning());
}


void pharosCpuMulticoreEnableInt()
{
    /* set the MIE register MSIE bit */
    pharosCpuMieSetBit(PHAROS_CPU_MIE_MSIE);
}


void pharosCpuInterruptStackGet(uint8_t **interruptStackStart , uint8_t **interrruptStackEnd)
{
    /* set the start address */
    *interruptStackStart = pharosCpuInterruptStack[pharosICoreRunning()];

    /* set the end address */
    *interrruptStackEnd = pharosCpuInterruptStack[pharosICoreRunning()] + PHAROS_CPU_INTERRUPT_STACK_SIZE;
}
