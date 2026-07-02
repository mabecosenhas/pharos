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
#include <pharos/hal/cpu/conf.h>
#include <pharos/hal/performance/performance.h>
#include <pharos/hal/cpu/system.h>


void pharosCpuMemoryInitialize(void)
{
    /* initialize the MINITGCR register to enable the hardware initialization key */
    PHAROS_SYSTEM_PRIMARY->memHwInitializationGlobalCtr = PHAROS_CPU_MINITGCR_KEY;
    
    /* initialize the MSINENA register to enable MBIST controller (initialize SRAM) */
    PHAROS_SYSTEM_PRIMARY->memSelfTestInitEnable = PHAROS_CPU_MSINENA_SELFTEST;
    
    /* wait until the memory self-test is completed */
    while(PHAROS_SYSTEM_PRIMARY->memHwInitStatus != PHAROS_CPU_MSTCGSTAT_MEM_DONE);

    /* write anything to the MINITGCR register to disable the hardware initialization key */
    PHAROS_SYSTEM_PRIMARY->memHwInitializationGlobalCtr = !PHAROS_CPU_MINITGCR_KEY;
}
