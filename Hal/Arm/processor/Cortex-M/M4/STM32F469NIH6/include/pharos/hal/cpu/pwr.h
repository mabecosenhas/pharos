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


#ifndef PHAROS_PWR_H
#define	PHAROS_PWR_H

#include <pharos/hal/cpu/declarations.h>
#include <pharos/hal/cpu/conf.h>


#define PHAROS_CPU_PWR                      ((ptrPharosCpuPwr) PHAROS_CPU_PWR_ADDRESS)


#define PHAROS_CPU_PWR_CR_VOS_SCALE1        (0b11 << 14)
#define PHAROS_CPU_PWR_CR_DBP_EN            (1U << 8U)

struct PharosCpuPwr
{
    /**
     * PWR power control register (PWR_CR)
     * 
     * offset:      0x00
     * Reset value: 0x0000 C000 (reset by wakeup from Standby mode)
     */
    volatile uint32_t control;

    /**
     * PWR power control/status register (PWR_CSR)
     * 
     * offset:      0x004
     * Reset value: 0x0000 0000 (not reset by wakeup from Standby mode)
     */
    volatile uint32_t controlStatus;

};

void pharosCpuPwrDisBackupWriteProt(void);


void pharosCpuPwrEnableOverDrive(void);


/* PHAROS_PWR_H */
#endif	
