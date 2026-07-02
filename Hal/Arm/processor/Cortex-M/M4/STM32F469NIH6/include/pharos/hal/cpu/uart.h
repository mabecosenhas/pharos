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
#define	PHAROS_UART_H


#include <pharos/hal/cpu/declarations.h>
#include <pharos/hal/cpu/conf.h>


#define PHAROS_CPU_USART1                   ((ptrPharosCpuUsart) PHAROS_CPU_USART1_ADDRESS)
#define PHAROS_CPU_USART2                   ((ptrPharosCpuUsart) PHAROS_CPU_USART2_ADDRESS)
#define PHAROS_CPU_USART3                   ((ptrPharosCpuUsart) PHAROS_CPU_USART3_ADDRESS)
#define PHAROS_CPU_USART4                   ((ptrPharosCpuUsart) PHAROS_CPU_USART4_ADDRESS)
#define PHAROS_CPU_USART5                   ((ptrPharosCpuUsart) PHAROS_CPU_USART5_ADDRESS)
#define PHAROS_CPU_USART6                   ((ptrPharosCpuUsart) PHAROS_CPU_USART6_ADDRESS)
#define PHAROS_CPU_UART7                    ((ptrPharosCpuUsart) PHAROS_CPU_UART7_ADDRESS)
#define PHAROS_CPU_UART8                    ((ptrPharosCpuUsart) PHAROS_CPU_UART8_ADDRESS)

#define PHAROS_CPU_USART_SR_TXE_RDY         (1 << 7)

struct PharosCpuUsart
{
    /**
     * Status register (USART_SR)
     * 
     * offset:      0x00
     * Reset value: 0x00C0 0000
     */
    volatile uint32_t status;

    /**
     *  Data register (USART_DR)
     * 
     * offset:      0x04
     * Reset value: 0x0000 0000
     */
    volatile uint32_t data;

    /**
     *  Baud rate register (USART_BRR)
     * 
     * offset:      0x08
     * Reset value: 0x0000 0000
     */
    volatile uint32_t baudRate;

    /**
     *  Control register 1 (USART_CR1)
     * 
     * offset:      0x0c
     * Reset value: 0x0000 0000
     */
    volatile uint32_t control1;

    /**
     *  Control register 2 (USART_CR2)
     * 
     * offset:      0x10
     * Reset value: 0x0000 0000
     */
    volatile uint32_t control2;

    /**
     *  Control register 3 (USART_CR3)
     * 
     * offset:      0x14
     * Reset value: 0x0000 0000
     */
    volatile uint32_t control3;

    /**
     *  Guard time and prescaler register (USART_GTPR)
     * 
     * offset:      0x18
     * Reset value: 0x0000 0000
     */
    volatile uint32_t guardTimePrescalar;
};




/**
 * Configures the UART of the CPU to 9600 baud rate, 8 data bits, no-parity and 1 stop bit
 */
void pharosCpuUartInit(ptrPharosCpuUsart usart);


/* PHAROS_UART_H */
#endif	
