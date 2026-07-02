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
#include <pharos/hal/v7m/declarations.h>
#include <pharos/hal/v7m/isr.h>
#include <pharos/hal/cpu/cpu.h>
#include <pharos/hal/thumb.h>
#include <pharos/hal/v7m/systick.h>
#include <pharos/hal/cpu/context.h>
#include <pharos/kernel/thread/stack.h>
#include <pharos/hal/v7m/mpu.h>
#include <pharos/hal/v7m/cache.h>
#include <pharos/hal/v7m/fpu.h>
#include <pharos/hal/memory/mpuV7.h>
#include <pharos/hal/cpu/conf.h>
#include <pharos/kernel/object/protection.h>
#include <pharos/kernel/core/core.h>
#include <pharos/kernel/clock/clock.h>
#include <pharos/hal/v7m/nvic.h>
#include <pharos/hal/cpu/timer.h>
#include <pharos/hal/cpu/rom.h>


void pharosCpuOscInit(void)
{
    uint32_t i;


    /* clear the main oscillator power down, range setting and no cristal */
    PHAROS_CPU_SYSTEM_CONTROL->mainOscillatorControl &= ~(PHAROS_CPU_SYSCTL_MOSCCTL_OSCRNG | PHAROS_CPU_SYSCTL_MOSCCTL_PWRDN | PHAROS_CPU_SYSCTL_MOSCCTL_NOXTAL);

    /* set the best maximum frequency below the 25 MHz */
    PHAROS_CPU_SYSTEM_CONTROL->memTimParFlashEeprom0 = PHAROS_CPU_SYSCTL_MEMTIM0_FBCHT_1_5 |
        (1 << PHAROS_CPU_SYSCTL_MEMTIM0_FWS_S) | PHAROS_CPU_SYSCTL_MEMTIM0_EBCHT_1_5 | (1 << PHAROS_CPU_SYSCTL_MEMTIM0_EWS_S) | PHAROS_CPU_SYSCTL_MEMTIM0_MB1;

    /* clear the previous PLL divider and source and update the memory timings */
    PHAROS_CPU_SYSTEM_CONTROL->runSleepModeConf = (PHAROS_CPU_SYSTEM_CONTROL->runSleepModeConf &
                                                   ~(PHAROS_CPU_SYSCTL_RSCLKCFG_PSYSDIV_MASK | PHAROS_CPU_SYSCTL_RSCLKCFG_OSCSRC_M |
                                                     PHAROS_CPU_SYSCTL_RSCLKCFG_PLLSRC_M | PHAROS_CPU_SYSCTL_RSCLKCFG_USEPLL)) | PHAROS_CPU_SYSCTL_RSCLKCFG_MEMTIMU;

    /* select the MOSC as the PLL source */
    PHAROS_CPU_SYSTEM_CONTROL->runSleepModeConf |= PHAROS_CPU_SYSCTL_RSCLKCFG_OSCSRC_MOSC | PHAROS_CPU_SYSCTL_RSCLKCFG_PLLSRC_MOSC;

    /* set the N to 4 */
    PHAROS_CPU_SYSTEM_CONTROL->pllFreq1 = 4;
    PHAROS_CPU_SYSTEM_CONTROL->pllFreq1 |= 256;

    /* since we know that we need 120 MHz and have a clock input of 25 MHz we can calculate the right M */
    PHAROS_CPU_SYSTEM_CONTROL->pllFreq0 = 96;

    /* Set the Flash and EEPROM timing values for 120 MHz */
    PHAROS_CPU_SYSTEM_CONTROL->memTimParFlashEeprom0 = (PHAROS_CPU_SYSCTL_MEMTIM0_FBCHT_3_5 | (5 << PHAROS_CPU_SYSCTL_MEMTIM0_FWS_S) |
                                                        PHAROS_CPU_SYSCTL_MEMTIM0_EBCHT_3_5 | (5 << PHAROS_CPU_SYSCTL_MEMTIM0_EWS_S) |
                                                        PHAROS_CPU_SYSCTL_MEMTIM0_MB1);

    /* power on the PLL */
    PHAROS_CPU_SYSTEM_CONTROL->pllFreq0 |= PHAROS_CPU_SYSCTL_PLLFREQ0_PWR;

    /* loop waiting for the PLL to lock. Wait at most 32 KiB iterations */
    for(i = 0; (i < VALUE_32KiB) & ((PHAROS_CPU_SYSTEM_CONTROL->pllStat & PHAROS_CPU_SYSCTL_PLLSTAT_LOCK) != 0U); i++);

    /* if could lock the PLL before the number of max iterations has elapsed */
    if(i == VALUE_32KiB)
    {
        /* could not set the timeout */
        pharosFatalErrorAnnounce(PHAROS_FATAL_CPU_PLL_NOT_LOCKED , __FILE__ , __func__ , __LINE__);

        /* if recovered from the PLL just leave, give a change for the application to continue with lower clock */
    }
    else
    {
        /* set the clock configuration to use the PLL (now that it is locked) */
        PHAROS_CPU_SYSTEM_CONTROL->runSleepModeConf |= (1 << PHAROS_CPU_SYSCTL_RSCLKCFG_PSYSDIV_OFFSET) |
            PHAROS_CPU_SYSCTL_RSCLKCFG_OSCSRC_MOSC | PHAROS_CPU_SYSCTL_RSCLKCFG_PLLSRC_MOSC | PHAROS_CPU_SYSCTL_RSCLKCFG_USEPLL | PHAROS_CPU_SYSCTL_RSCLKCFG_MEMTIMU;

        /* chance to the oscillator source */
        PHAROS_CPU_SYSTEM_CONTROL->runSleepModeConf &= ~(PHAROS_CPU_SYSCTL_RSCLKCFG_OSCSRC_M);
    }
}


void pharosCpuInitialize(void)
{
    /* initialize the floating point */
    pharosCpuFpuInit();

    /* initialize the i-cache */
    pharosCpuCacheIEnable();

    /* initialize the d-cache */
    pharosCpuCacheDEnable();
}


void pharosCpuCoreInitialize(ptrCore core)
{
    /* initialize the interrupts */
    pharosCpuNvicInitialize(&core->interrupts);
}
