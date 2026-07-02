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
#include <pharos/hal/vicPl190.h>
#include <pharos/hal/cpu/cpu.h>
#include <pharos/kernel/partition/partition.h>
#include <pharos/hal/boardApi.h>
#include <pharos/kernel/core/core.h>
#include <pharos/hal/fpu.h>
#include <pharos/kernel/core/rsp.h>
#include <pharos/hal/board/conf.h>
#include <pharos/hal/interrupt.h>


#define SYS_LOCK_ADDRESS        (uint32_t *) 0x10000020
#define SYS_RESETCTL_ADDRESS    (uint32_t *) 0x10000040
#define SYS_LOCK_UNLOCK_MAGIC   0xA05F

static volatile uint32_t * const UART0DR = (uint32_t *) PHAROS_VERSATILE_PB_UART0_ADDRESS;

extern uint8_t pharosCpuVectorsStart;
extern uint8_t pharosCpuVectorsEnd;
extern uint8_t pharosCpuVectorLocation;


void pharosCpuCopyIsrVector(void)
{
    /* use memcpy to copy the vectors (copying them by hand seemed to trigger the optimization of gcc and nothing was done) */
    memcpy(&pharosCpuVectorLocation , &pharosCpuVectorsStart , &pharosCpuVectorsEnd - &pharosCpuVectorsStart);
}


void pharosCpuBoot()
{
    /* initialize the FPU */
    pharosCpuFpuInitialize();

    /* initialize Pharos in single core */
    pharosSingleCoreInitialize();
}


void pharosCpuCoreInitialize(ptrCore core)
{
    /* initialize the vector interrupt controller */
    pharosCpuInterruptControllerInitialize(&core->interrupts);
}


void pharosCpuShutdown(void)
{
    PharosInterruptLevel level;


    /* disable all interrupts from here on out */
    pharosCpuDisableInterrupts(&level);

    /* unlock registers */
    *(SYS_LOCK_ADDRESS) = (*(SYS_LOCK_ADDRESS)) | SYS_LOCK_UNLOCK_MAGIC;

    /* set the reset */
    *(SYS_RESETCTL_ADDRESS) = (*(SYS_RESETCTL_ADDRESS)) | 0x86;


    volatile uint32_t *sys_lock = (volatile uint32_t *) 0x10000020;
    volatile uint32_t *sys_resetctl = (volatile uint32_t *) 0x10000040;

    while(1)
    {
        *sys_lock = 0xa05f;
        *sys_resetctl = 0xf4;
    }
}


void outputSChar(char c)
{
    /* use channel serial port 0 */
    *UART0DR = (uint32_t) c;
}
