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


#include <pharos/hal/cpu/cpu.h>
#include <pharos/hal/cpu/timer.h>
#include <pharos/hal/cpu/gptimer.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/hal/cpu/sections.h>

static void pharosCpuMulticoreInterruptHandler(void *argument , uintptr_t preInterruptData , PharosInterruptNumber number ,
                                        const uint8_t *interruptedAddress);


void pharosCpuInitialize(void)
{

}


uintptr_t pharosCpuMulticoreInterruptClearHandler(void *argument)
{
    /* clear the interrupt on mailbox 1 (write high to clear) */
    PHAROS_CPU_REGISTERS->coreMailboxesRead[pharosICoreRunning()][1] = 1;
    
    return 0;
}


void pharosCpuMulticoreInterruptHandler(void *argument , uintptr_t preInterruptData , PharosInterruptNumber number ,
                                        const uint8_t *interruptedAddress)
{
    pharosIMulticoreInterruptHandler();
}


void pharosCpuMulticoreTriggerInterrupt(CoreNumber number)
{
    /* trigger the interrupt on the specified core by writing to its mailbox 1 */
    PHAROS_CPU_REGISTERS->coreMailboxesWriteSet[number][1] = 1;
}


bool pharosCpuMulticoreIntIsPending(void)
{
    /* check if the interrupt is pending */
    return (PHAROS_CPU_REGISTERS->coreMailboxesRead[pharosICoreRunning()][1] == 1) ? TRUE : FALSE;
}


void pharosCpuMulticoreClearPending()
{
    /* clear interrupt */
    pharosCpuMulticoreInterruptClearHandler(NULL);
}


void pharosCpuMulticoreEnableInt()
{
    /* nothing to be done since interrupts will be enable afterwards */
}


void pharosCpuCoreInitialize(ptrCore core)
{
    static KERNEL_SECTION_USS PartitionIsrConf partConf[PHAROS_NUMBER_CORES];

    static KERNEL_SECTION_USS InterruptConfiguration interruptConf[PHAROS_NUMBER_CORES];

    /* initialize the vector interrupt controller */
    pharosCpuIcInitialize(&core->interrupts);

    partConf[core->number].isrHandler = pharosCpuMulticoreInterruptHandler;

    /* timer1 is not from a partition */
    partConf[core->number].partition = NULL;

    /* set the interrupt configuration */
    partConf[core->number].interruptConf = &interruptConf[core->number];

    /* set the clear interrupt */
    interruptConf[core->number].preInterruptHandler = pharosCpuMulticoreInterruptClearHandler;

    /* set the mailbox 1 as the inter-core interrupt */
    interruptConf[core->number].number = PHAROS_CPU_ISR_MAILBOX1;

    /* set IRQ priority */
    interruptConf[core->number].cpu.priority = PHAROS_CPU_INTERRUPT_IRQ;
 
    /* install the interrupt */
    pharosIInterruptInstallInit(&interruptConf[core->number] , &partConf[core->number]);
}
