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
#include <pharos/kernel/core/core.h>
#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/hal/cpu/uart.h>
#include <pharos/hal/cpu/conf.h>

#define OUTPUT_CHAR_DEVICE                      ((ptrNs16656) PHAROS_CPU_NS16656_ADDRESS)


void pharosCpuBoot()
{
    /* get the core we are running on */
    CoreNumber core = pharosICoreRunning();


    
    /* for the first core */
    if(core == 0U)
    {
        /* initialize the UART */
        pharosCpuNs16656Initialize(OUTPUT_CHAR_DEVICE , PHAROS_CPU_FREQ , 9600U);
    }

    /* initialize the multicore core */
    pharosMulticoreInitialize(core);
}


void outputSChar(char c)
{
    /* use the NS16656 device to output the char */
    pharosCpuNs16656OutputChar(OUTPUT_CHAR_DEVICE , c);
}


void pharosCpuShutdown(void)
{
    while(TRUE);
}


void pharosCpuRestart()
{
    while(1);
}
