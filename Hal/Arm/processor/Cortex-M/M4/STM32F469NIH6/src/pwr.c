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


#include <pharos/hal/cpu/pwr.h>

#define PHAROS_CPU_PWR_CR_DBP           (1 << 8)
#define PHAROS_CPU_PWR_CR_ODEN          (1<<16)
#define PHAROS_CPU_PWR_CR_ODSWEN        (1<<17)

#define PHAROS_CPU_PWR_CSR_ODRDY        (1<<16)
#define PHAROS_CPU_PWR_CSR_ODSWRDY      (1<<17)


void pharosCpuPwrDisBackupWriteProt()
{
    /* check if the DBP (Disable backup write protection is set */
    if((PHAROS_CPU_PWR->control & PHAROS_CPU_PWR_CR_DBP) == 0)
    {
        /* it is set, that means we came from a reset and to protect against parasitic writes, the CPU has disabled 
         * writing to the backup SRAM. We have to enable it */
        PHAROS_CPU_PWR->control |= PHAROS_CPU_PWR_CR_DBP;

        /* wait until the control flag has been set */
    }
}


void pharosCpuPwrEnableOverDrive(void)
{
    /* enable the over-drive bit */
    PHAROS_CPU_PWR->control |= PHAROS_CPU_PWR_CR_ODEN;

    /* wait until the over drive is ready */
    while((PHAROS_CPU_PWR->controlStatus & PHAROS_CPU_PWR_CSR_ODRDY) != 0);

    /* enable the over-drive switching */
    PHAROS_CPU_PWR->control |= PHAROS_CPU_PWR_CR_ODSWEN;

    /* wait until the over drive is ready */
    while((PHAROS_CPU_PWR->controlStatus & PHAROS_CPU_PWR_CSR_ODSWRDY) != 0);
}
