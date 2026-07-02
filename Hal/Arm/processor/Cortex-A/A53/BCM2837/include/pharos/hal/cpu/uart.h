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

#define PHAROS_CPU_UART0                ((ptrPharosCpuPl011) PHAROS_CPU_PL011_ADDRESS)

#define PHAROS_CPU_UART0_LCRH_DATA8     (0b11 << 5)
#define PHAROS_CPU_UART0_LCRH_DATA7     (0b10 << 5)
#define PHAROS_CPU_UART0_LCRH_DATA6     (0b01 << 5)
#define PHAROS_CPU_UART0_LCRH_DATA5     (0b00 << 5)

#define PHAROS_CPU_UART0_LCRH_PDIS      (0b0 << 1)
#define PHAROS_CPU_UART0_LCRH_PENA      (0b1 << 1)

#define PHAROS_CPU_UART0_LCRH_STOP1     (0b0 << 3)
#define PHAROS_CPU_UART0_LCRH_STOP2     (0b1 << 3)

#define PHAROS_CPU_UART0_CR_TX_EN       (1 << 8)
#define PHAROS_CPU_UART0_CR_RX_EN       (1 << 9)
#define PHAROS_CPU_UART0_CR_ENABLE      (1 << 0)
#define PHAROS_CPU_UART0_CR_DISABLE     (0 << 0)


#define PHAROS_CPU_UART0_TXFF_FULL      (1U << 5U)

/**
 * Compact the size of the I/O registers
 */
#pragma pack(push ,4)


struct PharosCpuPl011
{
    /**
     * Data register (DR)
     * 
     * offset: 0x00
     */
    volatile uint32_t data;

    /**
     * Receive status/error clear register (RSRECR)
     * 
     * offset: 0x04
     */
    volatile uint32_t receiveStatus;

    /**
     * Reserved
     * 
     * offset: 0x08
     */
    volatile uint32_t reserved1[4];

    /**
     * Flag register (FR)
     * 
     * offset: 0x18
     */
    volatile uint32_t flag;

    /**
     * Reserved
     * 
     * offset: 0x1c
     */
    volatile uint32_t reserved2;

    /**
     * not being used
     * 
     * offset: 0x20
     */
    volatile uint32_t ilpr;

    /**
     * Integer Baud rate divisor (IBRD)
     * 
     * offset: 0x24
     */
    volatile uint32_t integerBaudRateDivisor;

    /**
     * Fractional Baud rate divisor (FBRD)
     * 
     * offset: 0x28
     */
    volatile uint32_t fractionalBaudRateDivisor;

    /**
     * Line Control register (LCRH)
     * 
     * offset: 0x2c
     */
    volatile uint32_t lineControl;

    /**
     * Control register (CR)
     * 
     * offset: 0x30
     */
    volatile uint32_t control;

    /**
     * Interrupt FIFO Level select register (IFLS)
     * 
     * offset: 0x34
     */
    volatile uint32_t interruptFifoLevelSelect;

    /**
     * Interrupt mask set clear register (IMSC)
     * 
     * offset: 0x38
     */
    volatile uint32_t interruptMaskSetClear;

    /**
     * Raw interrupt status register (RIS)
     * 
     * offset: 0x3c
     */
    volatile uint32_t rawInterruptSetClear;

    /**
     * Masked interrupt status register (MIS)
     * 
     * offset: 0x40
     */
    volatile uint32_t maskedInterruptStatus;

    /**
     * Interrupt Clear register (ICR)
     * 
     * offset: 0x44
     */
    volatile uint32_t interruptClear;

    /**
     * DMA Control register (DMACR)
     * 
     * offset: 0x48
     */
    volatile uint32_t dmaControl;

    /**
     * Reserved
     * 
     * offset: 0x4c
     */
    volatile uint32_t reserved[12];


    /**
     * Test Control register (ITCR)
     * 
     * offset: 0x80
     */
    volatile uint32_t testControl;

    /**
     * Integration Test input register (ITIP)
     * 
     * offset: 0x84
     */
    volatile uint32_t interationTestInput;

    /**
     * Integration Test output register (ITOP)
     * 
     * offset: 0x88
     */
    volatile uint32_t integrationTestOutput;

    /**
     * Test data register (TDR)
     * 
     * offset: 0x8c
     */
    volatile uint32_t testData;

};


/**
 * restore the pack definition
 */
#pragma pack(pop)


/* PHAROS_UART_H */
#endif	
