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


#include <pharos/kernel/core/multi.h>
#include <pharos/hal/cpu/clint.h>
#include <pharos/kernel/core/coreconfinline.h>


void pharosCpuMulticoreTriggerInterrupt(CoreNumber number)
{
    /* trigger the interrupt by writing to the core MSIP register */
    PHAROS_CPU_CLINT->msip[number] = 1U;
}


void pharosCpuMulticoreClearInterrupt(CoreNumber number)
{
    /* clear the interrupt */
    PHAROS_CPU_CLINT->msip[number] = 0U;
}


bool pharosCpuMulticoreIntIsPending(void)
{
    /* check if there is any bit written in the core */
    return (PHAROS_CPU_CLINT->msip[pharosICoreRunning()] == 0U) ? FALSE : TRUE;
}
