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


#include <pharos/hal/cpu/aux.h>
#include <pharos/hal/cpu/gpio.h>
#include <pharos/hal/cpu/uart.h>
#include <pharos/hal/mbox.h>


__attribute__((optimize("O0"))) void doNothing(void)
{

}


void pharosCpuUartInit(void)
{
    uint32_t aux;
   
    /* mailbox buffer */
    uint32_t mbox[36] __attribute__((aligned(16)));

    /* disable the UART so we can change its configuration (initialize it) */
    PHAROS_CPU_UART0->control = PHAROS_CPU_UART0_CR_DISABLE;

    /* set up clock */
    mbox[0] = 8 * 4;
    
    /* request */
    mbox[1] = PHAROS_MBOX_TAG_REQUEST;
    
    /* set the clock rate */
    mbox[2] = PHAROS_MBOX_TAG_SETCLKRATE; 
    mbox[3] = 12;
    mbox[4] = 8;
    
    /* set the UART clock */
    mbox[5] = 2; 
    
    /* set to 4 MHz */
    mbox[6] = 4000000; 
    
    /* last value of the message */
    mbox[7] = PHAROS_MBOX_TAG_LAST;
    
    /* send the message to the mbox */
    pharosCpuMboxCall(PHAROS_CPU_MBOX_CHANNEL_PROP , mbox);

    /* map UART0 to GPIO pins */

    /* read the function select (we want GPIO 14 and 15) */
    aux = PHAROS_CPU_GPIO->functionSelect1;

    /* clear GPIO 14 */
    aux &= ~PHAROS_CPU_GPFSEL1_MASK(14);

    /* select the PIN 14 as alternate 5 */
    aux |= PHAROS_CPU_GPFSEL1_ALTER5(14);

    /* clear GPIO 15 */
    aux &= ~PHAROS_CPU_GPFSEL1_MASK(15);

    /* select the PIN 15 as alternate 5 */
    aux |= PHAROS_CPU_GPFSEL1_ALTER5(15);

    /* set the function select for GPIO 14 and 15 */
    PHAROS_CPU_GPIO->functionSelect1 = aux;

    /* disable pull up/down */
    PHAROS_CPU_GPIO->pinPullUpDownEnable = 0U;

    /* wait cycle */
    for(aux = 0; aux < 150; aux++)
    {
        doNothing();
    }

    /* enable clock 14 and 15 */
    PHAROS_CPU_GPIO->pinPullUpDownEnableClock0 = PHAROS_CPU_GPPUDCLK0_EN(14) | PHAROS_CPU_GPPUDCLK0_EN(15);

    /* wait cycle */
    for(aux = 0; aux < 150; aux++)
    {
        doNothing();
    }

    /* do something to this register so the clock is activated */
    PHAROS_CPU_GPIO->pinPullUpDownEnableClock0 = 0U;

    /* clear all the UART interrupts (Overrun, break and transmit) */
    /* TODO: check if instead of 0x7ff could be 0xf */
    PHAROS_CPU_UART0->interruptClear = 0x7FF;

    /* set the baud rate to 115200 */
    PHAROS_CPU_UART0->integerBaudRateDivisor = 2;
    PHAROS_CPU_UART0->fractionalBaudRateDivisor = 0xb;

    /* set 8 data bits, one stop bit and parity disabled */
    PHAROS_CPU_UART0->lineControl = PHAROS_CPU_UART0_LCRH_DATA8 | PHAROS_CPU_UART0_LCRH_PDIS | PHAROS_CPU_UART0_LCRH_STOP1;

    /* enable the UART, together with the Tx and Rx */
    PHAROS_CPU_UART0->control = PHAROS_CPU_UART0_CR_TX_EN | PHAROS_CPU_UART0_CR_RX_EN | PHAROS_CPU_UART0_CR_ENABLE;
}


void outputSChar(char c)
{
    /* wait while the UART status is not ready */
    while((PHAROS_CPU_UART0->flag & PHAROS_CPU_UART0_TXFF_FULL) != 0);

    /* use UART serial port 0 to transfer the char */
    PHAROS_CPU_UART0->data = c;
}
