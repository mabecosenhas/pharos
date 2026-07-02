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


#include <pharos/hal/v7m/fpu.h>
#include <pharos/hal/v7m/scb.h>
#include <pharos/hal/v7m/v7m.h>

#define PHAROS_CPU_FPCCR_ASP_EN           (1 << 31)
#define PHAROS_CPU_FPCCR_LSP_DIS          (0 << 30)


void pharosCpuFpuInitCpac()
{
    /* init the CPAC (sets to "Privileged Access Only" for CP11 and CP10) */
    PHAROS_CPU_SCB->coprocessorAccessControl = PHAROS_CPU_SCB->coprocessorAccessControl | (0xF << 20);
    
    /* wait for memory to be written to */
    pharosCpuDsb();
    
    /* make sure the co-processor has been updated */
    pharosCpuIsb();
}

void pharosCpuFpuInit(void)
{   
    /* initialize the co-processor */
    pharosCpuFpuInitCpac();
    
    /* enable the floating point (but not the lazy FP context save) */
    PHAROS_CPU_FPU->contextControl = PHAROS_CPU_FPCCR_ASP_EN | PHAROS_CPU_FPCCR_LSP_DIS;
}
