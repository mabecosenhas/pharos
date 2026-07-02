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


#include <pharos/kernel/core/coreconf.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/kernel/core/rsp.h>
#include <pharos/hal/cpu/sections.h>
#include <pharos/hal/cpu/linker.h>
#include <pharos/kernel/object/declarations.h>


/**
 * interrupt nesting level
 */
KERNEL_SECTION uintptr_t pharosVInterruptNestingLevel = 0U;


/**
 * Running thread
 */
volatile KERNEL_SECTION STRUCTURE_FIELD_ALIGNMENT_8 ptrThreadInfo pharosVRunningThread = NULL;


/**
 * Context switch needed flag
 */
volatile KERNEL_SECTION uint32_t pharosVNeedsThreadContextSwitch = FALSE;


/**
 * dispatch is disabled by default
 */
volatile KERNEL_SECTION uint32_t pharosVDispatchLevel = 0U;


/**
 * Initialization stack (also corresponds to the idle stack)
 * 
 * Note that this stack MUST not be on the BSS section. If it was, then when it was zeroying out the BSS it would be also
 * zeroying its own stack and unpredictable behavior would occur.
 */
KERNEL_SECTION_USS uint8_t pharosVStartupStack[PHAROS_CPU_STARTUP_STACK_SIZE] VARIABLE_ALIGNMENT_X(PHAROS_STACK_ALIGNMENT);


void pharosSingleCoreInitialize()
{
    /* initialize the data */
    pharosIDataInitialize();

    /* initialize the partitions */
    pharosIPartitionsInitializeMinimal(&pharosVCoreTable.cores[0U] , 0U);

    /* kernel boot */
    pharosCBoot(&pharosVCoreTable.cores[0U]);
}


void pharosIDataInitialize()
{
    /* initialize the kernel data */
    pharosIKernelDataInitialize();

    /* initialize the shared data */
    pharosISharedDataInitialize();

    /* copy the inter-partition calls data */
    pharosIIpCallDataInitialize();
}


uint8_t *pharosCpuThreadIdleStack(void)
{
    /* return the stack address */
    return pharosVStartupStack;
}


void pharosCoreConfInitialize(ptrPharosCoreConfiguration conf)
{
    /* nothing to do */
}


void pharosICoreSynchronize(void)
{
    /* do nothing since there is only one core */
}
