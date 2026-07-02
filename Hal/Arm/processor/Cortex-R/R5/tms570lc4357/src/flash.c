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


#include <pharos/hal/cpu/flash.h>


void pharosCpuFlashInitialize(void)
{
    /* setup flash read mode, address wait states and data wait states */
    PHAROS_CPU_L2FMC->flashReadControl = PHAROS_CPU_L2FMC_FRDCNTL_RWAIT(3U) | PHAROS_CPU_L2FMC_FRDCNTL_PFUENA | PHAROS_CPU_L2FMC_FRDCNTL_PFUENB;

    /* enable write access to the flash state machine */
    PHAROS_CPU_L2FMC->fsmWriteEnable = PHAROS_CPU_L2FMC_FSM_WR_ENA_EN;

    /* setup flash access wait states for bank 7 */
    PHAROS_CPU_L2FMC->eepromEmulationConf = PHAROS_CPU_L2FMC_EE_CONF_EWAIT(9U) | 0x00000002U;

    /* disable write access to flash state machine registers */
    PHAROS_CPU_L2FMC->fsmWriteEnable = PHAROS_CPU_L2FMC_FSM_WR_ENA_DIS;

    /* set the flash bank 0,1 and 7 to active */
    PHAROS_CPU_L2FMC->flashBankPowerMode = PHAROS_CPU_L2FMC_FBPWRMODE_B7_A | PHAROS_CPU_L2FMC_FBPWRMODE_B1_A |
        PHAROS_CPU_L2FMC_FBPWRMODE_B0_A;
}
