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


#include <pharos/hal/cpu/system.h>
#include <pharos/hal/cpu/peripheral.h>
#include <pharos/hal/cpu/system.h>

void pharosCpuPeripheralsInitialize(void)
{
    /* disable peripherals before peripheral powerup */
    PHAROS_SYSTEM_PRIMARY->clockControl &= 0xfffffeffU;

    /* release peripherals from reset and enable clocks to all peripherals */
    
    /* powerup all peripherals */
    PHAROS_CPU_PCR_1->PSPWRDWNCLR0 = 0xffffffffU;
    PHAROS_CPU_PCR_1->PSPWRDWNCLR1 = 0xffffffffU;
    PHAROS_CPU_PCR_1->PSPWRDWNCLR2 = 0xffffffffU;
    PHAROS_CPU_PCR_1->PSPWRDWNCLR3 = 0xffffffffU;

    PHAROS_CPU_PCR_2->PSPWRDWNCLR0 = 0xffffffffU;
    PHAROS_CPU_PCR_2->PSPWRDWNCLR1 = 0xffffffffU;
    PHAROS_CPU_PCR_2->PSPWRDWNCLR2 = 0xffffffffU;
    PHAROS_CPU_PCR_2->PSPWRDWNCLR3 = 0xffffffffU;

    PHAROS_CPU_PCR_3->PSPWRDWNCLR0 = 0xffffffffU;
    PHAROS_CPU_PCR_3->PSPWRDWNCLR1 = 0xffffffffU;
    PHAROS_CPU_PCR_3->PSPWRDWNCLR2 = 0xffffffffU;
    PHAROS_CPU_PCR_3->PSPWRDWNCLR3 = 0xffffffffU;

    /* enable peripherals */
    PHAROS_SYSTEM_PRIMARY->clockControl |= 0x00000100U;
}
