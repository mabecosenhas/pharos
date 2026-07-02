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


#include <pharos/hal/board.h>
#include <pharos/hal/hal.h>
#include <pharos/hal/cpu/cpu.h>
#include <pharos/hal/boardApi.h>
#include <pharos/kernel/thread/stack.h>
#include <pharos/hal/cpu/asm.h>
#include <pharos/kernel/core/syscall.h>
#include <pharos/kernel/partition/ipcall.h>
#include <pharos/hal/fpu.h>
#include <pharos/kernel/object/protection.h>
#include <pharos/kernel/core/core.h>
#include <pharos/hal/cpu/context.h>
#include <pharos/kernel/assert/assert.h>


void pharosCpuContextSwitch(ptrThreadInfo current , ptrThreadInfo heir)
{
    /* if not running in idle thread */
    if(current->partition != NULL)
    {
        /* save the context (shared stack) */
        pharosCpuMpuSave(current);
    }

    /* if not switching to an idle thread */
    if(heir->partition != NULL)
    {
        /* restore the MPU registers of the heir thread */
        pharosCpuMpuRestore(heir);
    }

    /* perform the context switch (low level registers) */
    pharosCpuContextSwitchLow(&current->cpuContext , &heir->cpuContext);
}


void pharosCpuRestoreHeir(ptrThreadInfo heir)
{
    /* if not starting with an idle thread */
    if(heir->partition != NULL)
    {
        /* restore the MPU registers of the heir thread */
        pharosCpuMpuRestore(heir);
    }

    /* perform the context switch (low level registers) */
    pharosCpuRestoreHeirLow(&heir->cpuContext);
}
