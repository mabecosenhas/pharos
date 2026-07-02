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


#include <pharos/hal/cpu/sections.h>
#include <pharos/hal/cpu/uart.h>
#include <pharos/hal/cpu/conf.h>


void pharosCpuNs16656Initialize(ptrNs16656 device , uint32_t freq , uint32_t baudRate)
{
    /* set the divisor to get the 9600 baud rate */
    uint32_t divisor = (freq * VALUE_1M) / (16U * baudRate);


    /* switch to DLAB mode */
    device->lineControl = PHAROS_CPU_NS16656_LCR_DLAB_1;

    /* set the first part of the divisor (DLL register) */
    device->receiveTransmit = divisor & 0xff;

    /* set the second part of the divisor (DLM register) */
    device->interruptEnable = (divisor >> 8) & 0xff;

    /* switch back to normal mode and set the parity to odd and 8 data bits and one stop bit */
    device->lineControl = PHAROS_CPU_NS16656_LCR_DLAB_0 | PHAROS_CPU_NS16656_LCR_PAR_ODD |
            PHAROS_CPU_NS16656_LCR_DATA_8 | PHAROS_CPU_NS16656_LCR_STOP_1;
}
