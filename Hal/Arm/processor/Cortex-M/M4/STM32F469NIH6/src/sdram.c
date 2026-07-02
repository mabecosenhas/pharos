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


#include <pharos/hal/cpu/fmc.h>


/**
 * Wait until a command is possible to be sent to the SDRAM controller
 */
static void pharosCpuSdramWait();



void pharosCpuSdramClockEnable()
{
    /* build the clock enable command for the bank1 of the SDRAM */
    uint32_t command = PHAROS_CPU_FMC_SDCMR_CTB1 | PHAROS_CPU_FMC_SDCMR_MODE_CLOCK_ENABLE;

    /* wait till we can write to the SDRAM command mode register */
    pharosCpuSdramWait();

    /* send the command */
    PHAROS_CPU_FMC->sdramCommandMode = command;
}


void pharosCpuSdramPall()
{
    /* build the PALL command for the bank1 of the SDRAM */
    uint32_t command = PHAROS_CPU_FMC_SDCMR_CTB1 | PHAROS_CPU_FMC_SDCMR_MODE_PALL;

    /* wait till we can write to the SDRAM command mode register */
    pharosCpuSdramWait();

    /* send the command */
    PHAROS_CPU_FMC->sdramCommandMode = command;
}


void pharosCpuSdramAutorefresh(uint32_t numberAutoRefresh)
{
    /* build the PALL command for the bank1 of the SDRAM */
    uint32_t command = PHAROS_CPU_FMC_SDCMR_CTB1 | PHAROS_CPU_FMC_SDCMR_MODE_AUTO_REFRESH | PHAROS_CPUFMC_SDCMR_NRFS(numberAutoRefresh);

    /* wait till we can write to the SDRAM command mode register */
    pharosCpuSdramWait();

    /* send the command */
    PHAROS_CPU_FMC->sdramCommandMode = command;
}


void pharosCpuSdramLoadCommand(uint32_t mrd)
{
    /* build the PALL command for the bank1 of the SDRAM */
    uint32_t command = PHAROS_CPU_FMC_SDCMR_CTB1 | PHAROS_CPU_FMC_SDCMR_MODE_LOAD_MODE | PHAROS_CPUFMC_SDCMR_MRD(mrd);

    /* wait till we can write to the SDRAM command mode register */
    pharosCpuSdramWait();

    /* send the command */
    PHAROS_CPU_FMC->sdramCommandMode = command;
}


void pharosCpuSdramWait()
{
    /* wait while the busy flag is set */
    while((PHAROS_CPU_FMC->sdramStatus & PHAROS_CPU_FMC_SDSR_BUSY) != 0);
}
