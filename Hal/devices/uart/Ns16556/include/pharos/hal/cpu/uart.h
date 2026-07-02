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


#ifndef PHAROS_UART_H
#define PHAROS_UART_H

#include <pharos/hal/hal.h>
#include <pharos/hal/cpu/conf.h>


#define PHAROS_CPU_NS16656_LCR_DLAB_1       (0x80U)
#define PHAROS_CPU_NS16656_LCR_DLAB_0       (0x00U)

#define PHAROS_CPU_NS16656_LCR_PAR_NO       (0x00U)
#define PHAROS_CPU_NS16656_LCR_PAR_ODD      (0x18U)
#define PHAROS_CPU_NS16656_LCR_PAR_EVEN     (0x08U)

#define PHAROS_CPU_NS16656_LCR_DATA_8       (0x03U)
#define PHAROS_CPU_NS16656_LCR_DATA_7       (0x02U)
#define PHAROS_CPU_NS16656_LCR_DATA_6       (0x01U)
#define PHAROS_CPU_NS16656_LCR_DATA_5       (0x00U)

#define PHAROS_CPU_NS16656_LCR_STOP_1       (0x00U)
#define PHAROS_CPU_NS16656_LCR_STOP_2       (0x04U)

#define PHAROS_CPU_NS16656_LSR_IDLE         (0x40U)


/**
 * Compact the size of the I/O registers
 */
#pragma pack(push ,1)

typedef struct
{
    volatile uint8_t receiveTransmit;
    volatile uint8_t interruptEnable;
    volatile uint8_t interruptId_fifoControl;
    volatile uint8_t lineControl;
    volatile uint8_t modemControl;
    volatile uint8_t lineStatus;
    volatile uint8_t modemStatus;
} Ns16656 , *ptrNs16656;

/**
 * restore the pack definition
 */
#pragma pack(pop)


/**
 * Initialize the NS16656
 * 
 * @param device the pointer to the NS16656 device in memory
 * @param freq frequency of the CPU (in MHz)
 * @param baudRate desired baud rate (e.g. 9600)
 */
void pharosCpuNs16656Initialize(ptrNs16656 device , uint32_t freq , uint32_t baudRate);


/**
 * Outputs (polled) a char to the device
 * 
 * @param device device to output the char
 * @param c char to output
 */
INLINE void pharosCpuNs16656OutputChar(ptrNs16656 device , char c)
{
    /* wait while the UART status is not ready */
    while((device->lineStatus & PHAROS_CPU_NS16656_LSR_IDLE) == 0);

    /* transmit the char */
    device->receiveTransmit = c;
}


/* PHAROS_UART_H */
#endif 
