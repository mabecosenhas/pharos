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


#include <pharos/hal/cpu/rcc.h>
#include <pharos/hal/cpu/flash.h>
#include <pharos/hal/cpu/pwr.h>


void pharosCpuInitHse(void)
{
    /* enable the HSE clock */
    PHAROS_CPU_RCC->clockControl |= PHAROS_CPU_RCC_CR_HSE_ON;

    /* wait until HSE is ready */
    while((PHAROS_CPU_RCC->clockControl & PHAROS_CPU_RCC_CR_HSE_RDY) == 0);
}


void pharosCpuInitPll(void)
{
    uint32_t i;


    /* first disable the PLL */
    PHAROS_CPU_RCC->clockControl &= ~PHAROS_CPU_RCC_CR_PLL_ON;

    /* wait until PLL is ready */
    for(i = 0; i < 10000U; i++)
    {
        if(((PHAROS_CPU_RCC->clockControl & PHAROS_CPU_RCC_CR_PLL_RDY) != 0))
        {
            break;
        }
    }
    /* set the PLL to:  (f_output = f_VCO/P  =  360/2 = 180)
     *  - use the HSE   (f_HSE = 8 MHz)
     *  - P = 2
     *  - M = 4         (f_VCO = f_HSE*N/M = 8 * 180/4 = 360)
     *  - N = 180       
     *  - Q = 6         (f_USB = f_SDMMC = f_VCO/Q = 360/6 = 60)
     *  - R = 6         (f_DSI = f_VCO/4 = 360/6 = 60) 
     */
    PHAROS_CPU_RCC->pllConfiguration = PHAROS_CPU_RCC_PLLCFGR_HSE |
            PHAROS_CPU_RCC_PLLCFGR_M(4U) |
            PHAROS_CPU_RCC_PLLCFGR_N(180U) |
            PHAROS_CPU_RCC_PLLCFGR_P(2U) |
            PHAROS_CPU_RCC_PLLCFGR_Q(6U) |
            PHAROS_CPU_RCC_PLLCFGR_R(6U);

    /* enable the PLL */
    PHAROS_CPU_RCC->clockControl |= PHAROS_CPU_RCC_CR_PLL_ON;

    /* wait until PLL is ready */
    for(i = 0; i < 10000U; i++)
    {
        if(((PHAROS_CPU_RCC->clockControl & PHAROS_CPU_RCC_CR_PLL_RDY) != 0))
        {
            break;
        }
    }
}


void pharosCpuOscInit(void)
{
    /* we may have come from a reset so we need to check this case and disable the protection against writes 
     * in the backup SRAM */
    pharosCpuPwrDisBackupWriteProt();

    /* configure the HSE clock oscillator to 8 MHz */
    pharosCpuInitHse();

    /* enable the power interface clock to enable access to the RTC and RTC backup registers (see section 5.1.4 of the
     * "STM32F469xx and STM32F479xx advanced Arm ® -based 32-bit MCUs" */
    PHAROS_CPU_RCC->apb1PeripheralClockEnable |= PHAROS_CPU_RCC_APB1ENR_PWR_EN;

    /* set the output voltage to the default value (and enable access to backup SRAM) */
    PHAROS_CPU_PWR->control |= PHAROS_CPU_PWR_CR_VOS_SCALE1 | PHAROS_CPU_PWR_CR_DBP_EN;

    /* initialize the PLL to give an output of 360 MHz (f_VCO) and the clock frequency of the CPU is divided by 2 so it is 180 MHz */
    pharosCpuInitPll();

    /* activate the over-drive so that the CPU frequency can reach 180 MHz */
    pharosCpuPwrEnableOverDrive();

    /* 5 wait cycles for flash under 180 MHz */
    pharosCpuFlashSetLatency(5);

    /* set AHB to 180 MHz / 1 and set the APB1 to AHB/4 = 45 MHz and APB2 to AHB/2 = 90 MHz */
    PHAROS_CPU_RCC->clockConfiguration |=
            PHAROS_CPU_RCC_CFGR_PPRE1(PHAROS_CPU_RCC_CFGR_PPRE1_DIV_4) |
            PHAROS_CPU_RCC_CFGR_PPRE2(PHAROS_CPU_RCC_CFGR_PPRE2_DIV_2) |
            PHAROS_CPU_RCC_CFGR_HPRE(PHAROS_CPU_RCC_CFGR_HPRE_DIV_1);

    /* activate the PLL as source of clock */
    PHAROS_CPU_RCC->clockConfiguration |= PHAROS_CPU_RCC_CFGR_SW(PHAROS_CPU_RCC_CFGR_SW_PLL_P);
}
