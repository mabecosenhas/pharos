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


#include <pharos/hal/vicPl190.h>
#include <pharos/hal/halapi.h>
#include <pharos/hal/cpuapi/arm.h>
#include <pharos/kernel/interrupt/interrupt.h>
#include <pharos/hal/boardApi.h>
#include <pharos/hal/cpu/sections.h>
#include <pharos/hal/interrupt.h>


void armVicPl190Initialize(ptrArmVicPl190 pl190)
{
    uint32_t i;

    /* set the default vector address as the last index of the pharosIsrTable */
    pl190->defaultVectorAddress = 0U;

    for(i = 0U; i < 16U; i++)
    {
        /* reset the address */
        pl190->vectorAddressRegisters[i] = 0U;
    }
}


void armVicPl190UnmaskInterrupt(PharosInterruptNumber number , ptrArmVicPl190 pl190)
{
    /* unmask the corresponding interrupt vector number */
    pl190->interruptEnable = 1U << number;
}


void armVicPl190MaskInterrupt(PharosInterruptNumber number , ptrArmVicPl190 pl190)
{
    /* mask the corresponding interrupt vector number */
    pl190->interruptEnableClear = 1U << number;
}


#define VIC_PL190_VICVECTCNTL_ENABLE            (1U << 5U)


void pharosCpuPl190InterruptInstall(ptrInterruptConfiguration conf , ptrArmVicPl190 pl190)
{
    /* if the interrupt configuration has a priority (other than default) */
    if(conf->cpu.priority != PHAROS_INTERRUPT_PRIORITY_DEFAULT)
    {
        /* 5th bit must be set to enable the slot (see the register definition above) */
        pl190->vectorControlRegisters[conf->cpu.priority] = (uint32_t) (VIC_PL190_VICVECTCNTL_ENABLE | conf->number);

        /* install the vector address with the priority 
         * because the control register only allows one interrupt number, each priority can only be associated with one interrupt number */
        pl190->vectorAddressRegisters[conf->cpu.priority] = (uint32_t) (1U << conf->number);
    }
        /* in case of an interrupt with default priority */
    else
    {
        /* add the interrupt number to the default address 
         * the default address can handle multiple interrupts */
        pl190->defaultVectorAddress |= (uint32_t) (1U << conf->number);
    }

    /* flush instruction after installing an interrupt vector */
    pharosCpuInvalidateICache();

    /* flush data cache */
    pharosCpuFlushDataCache();

    /* activate the interrupt */
    armVicPl190UnmaskInterrupt(conf->number , pl190);
}


void pharosCpuPl190InterruptRemove(ptrInterruptConfiguration conf , ptrArmVicPl190 pl190)
{
    /* if the interrupt configuration has a priority (other than default) */
    if(conf->cpu.priority != PHAROS_INTERRUPT_PRIORITY_DEFAULT)
    {
        /* disable the interrupt priority */
        pl190->vectorControlRegisters[conf->cpu.priority] = 0U;
    }
    else
    {
        /* remove the interrupt number from the default vector address */
        pl190->defaultVectorAddress &= ~(1U << conf->number);
    }

    /* only when there is no handler can de-activate the interrupt */
    armVicPl190MaskInterrupt(conf->number , pl190);

    /* flush instruction and data after removing an interrupt vector */
    pharosCpuInvalidateICache();

    /* flush data cache */
    pharosCpuFlushDataCache();
}
