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


#include <pharos/hal/cpu/uart.h>

#define PHAROS_CPU_USART_CR1_RESET              (0U)
#define PHAROS_CPU_USART_CR1_OVER8_16           (0U << 15U)
#define PHAROS_CPU_USART_CR1_OVER8_8            (1U << 15U)

#define PHAROS_CPU_USART_CR1_UE_EN              (1U << 13U)
#define PHAROS_CPU_USART_CR1_M_8                (0U << 12U)
#define PHAROS_CPU_USART_CR1_PCE_DIS            (0U << 10U)
#define PHAROS_CPU_USART_CR1_TE_EN              (1U << 3U)
#define PHAROS_CPU_USART_CR1_RE_EN              (1U << 2U)



#define PHAROS_CPU_USART_CR2_RESET              (0U)
#define PHAROS_CPU_USART_CR2_STOP1              (0b00 << 12)
#define PHAROS_CPU_USART_CR2_STOP_MASK          (~((uint32_t) (0b00 << 12)))


#define PHAROS_CPU_USART_CR3_RESET              (0U)

/**
 * for a baud of 9600 we have:
 * Fcl = 45 MHz 
 * baud = Fcl / (8 * 2 * DIV) => DIV = 292,96875
 * => Mant = 292
 * => Frac = 16 * 0.96875 = 15.5 +-= 16 = 0xf
 */
#define PHAROS_CPU_USART_BRR_9600               ((292 << 4)+ 0xf)


void pharosCpuUartInit(ptrPharosCpuUsart usart)
{
    /* reset the control 1 register */
    usart->control1 = PHAROS_CPU_USART_CR1_RESET;

    /* reset the control 2 register */
    usart->control2 = PHAROS_CPU_USART_CR2_RESET;

    /* reset the control 3 register */
    usart->control3 = PHAROS_CPU_USART_CR3_RESET;

    /* set oversampling to 16, 8 data bits, no parity, transmit and receive enabled */
    usart->control1 |= PHAROS_CPU_USART_CR1_OVER8_16 | PHAROS_CPU_USART_CR1_M_8 | PHAROS_CPU_USART_CR1_PCE_DIS |
        PHAROS_CPU_USART_CR1_TE_EN | PHAROS_CPU_USART_CR1_RE_EN;

    /* set the number of stop bits in the USART to 1 */
    usart->control2 |= PHAROS_CPU_USART_CR2_STOP1;

    /* set the baud rate to 9600 */
    usart->baudRate = PHAROS_CPU_USART_BRR_9600;

    /* enable the USART */
    usart->control1 |= PHAROS_CPU_USART_CR1_UE_EN;
}
