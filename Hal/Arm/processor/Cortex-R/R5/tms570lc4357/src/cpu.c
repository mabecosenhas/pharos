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


#include <pharos/hal/hal.h>
#include <pharos/hal/cpu/system.h>
#include <pharos/hal/arm.h>


PharosCpuResetSource pharosCpuResetSource(void)
{
    PharosCpuResetSource result;

    /* get the reset status from the CPU (removing the MPU mode) */
    uint32_t status = PHAROS_SYSTEM_PRIMARY->systemExceptionStatus & PHAROS_CPU_SYS1_SYSESR_MPU_UNMASK;

    /* if there is no reset source */
    if(status == 0U)
    {
        /* then return no reset */
        result = PHAROS_CPU_RST_SRC_NO_RESET;
    }
    else
    {
        /* read the exception status register (removing the MPU mode) by getting the highest bit */
        result = armbit2Integer(status);
    }

    /* return the reset source */
    return result;
}


void pharosCpuResetSourceClear(void)
{
    /* clear all reset sources */
    PHAROS_SYSTEM_PRIMARY->systemExceptionStatus |= PHAROS_CPU_SYS1_SYSESR_MPU_UNMASK;
}
