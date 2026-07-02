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
#include <pharos/hal/cpu/rom.h>
#include <pharos/hal/cpu/uart.h>
#include <pharos/declarations.h>
#include <pharos/hal/cpu/peripheral.h>


/**
 * Configuration for the ROM function to place 8 data bits
 */
#define DATA_BITS_8             0x00000060 

/**
 * Configuration for the ROM function to place 1 stop bit
 */
#define STOP_BIT_ONE            0x00000000 

/**
 * Configuration for the ROM function to place no parity bit
 */
#define PARITY_NO               0x00000000 


/**
 * Prototype of the ROM function to set the baud rate, number of data bits, stop bits and parity, given the clock for the
 * UART peripheral
 */
typedef void (* ROM_UARTConfigSetExpClk)(uint32_t baseAddress , uint32_t uartClock , uint32_t desiredBaud , uint32_t uartConfiguration);
typedef bool(* ROM_UARTCharPutNonBlocking)(uint32_t baseAddress , uint8_t c);


void pharosCpuUartInit(void)
{
    /* use the ROM function to setup the baud rate (9600), 8 data bits, one stop bit and no parity */
    ((ROM_UARTConfigSetExpClk) ROM_UARTTABLE[5])(PHAROS_CPU_UART0_BASE , 120e6
                                                 , 9600 , DATA_BITS_8 | PARITY_NO | STOP_BIT_ONE);
}


void outputSChar(char_t c)
{
    /* use the ROM function to send the char to the UART (loop until it is sent) */
    while(((ROM_UARTCharPutNonBlocking) ROM_UARTTABLE[15])(PHAROS_CPU_UART0_BASE , c) == FALSE);
}
