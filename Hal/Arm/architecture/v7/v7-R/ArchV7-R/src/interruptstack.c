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


#include <pharos/hal/boardApi.h>
#include <pharos/hal/arm.h>


void pharosCpuInterruptNestedEnable(void)
{
    /* check if the interrupt nest level is not too deep */
    if(pharosCpuSpGet() > pharosIrqSupervisorStack + PHAROS_CPU_INTERRUPT_STACK_MIN_SIZE)
    {
        /* make sure the compiler does not place previous operations after the interrupt enable */
        PHAROS_SW_MEMORY_BARRIER();

        /* enable interrupts if we have stack left */
        pharosCpuInterruptForceEnable();
    }

    /* otherwise we don't allow more nesting levels */
}


void pharosCpuInterruptNestedDisable(void)
{
    /* disable the interrupts */
    pharosCpuInterruptForceDisable();
}
