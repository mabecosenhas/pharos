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


#include <pharos/hal/cpu/asm.h>
#include <pharos/hal/hal.h>
#include <pharos/hal/cpu/cpu.h>


struct PharosCpuGic
{
    /**
     * Interrupt Controller Active Priorities Register (0,0) (ICC_AP0R0_EL1)
     * offset: 0x0
     */
    uint32_t activePrioritiesRegister0_0;
    
    /**
     * Interrupt Controller Active Priorities Register (0,0) (ICC_AP0R0_EL1)
     * offset: 0x0
     */
    uint32_t activePrioritiesRegister0_1;
    
    
};


KERNEL_SECTION_USS uint8_t pharosCpuInterruptStack[PHAROS_CPU_INTERRUPT_STACK_SIZE];


void pharosCInterruptHandler(PharosInterruptNumber interrupt)
{
    
}
