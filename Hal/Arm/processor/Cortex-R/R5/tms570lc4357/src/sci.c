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


#include <pharos/hal/cpu/sci.h>

#define SCI_TRANSMIT_READY      ((uint32_t) (0x00000100U))



void pharosCpuSciInitialize(ptrPharosCpuSci sci)
{
    /** @b initialize @b SCI1 */

    /** - bring SCI1 out of reset */
    sci->globalControl0 = 0U;
    sci->globalControl0 = 1U;

    /** - Disable all interrupts */
    sci->interruptClear = 0xFFFFFFFFU;
    sci->interruptClearLevel = 0xFFFFFFFFU;

    /** - global control 1 */
    sci->globalControl1 = (uint32_t) ((uint32_t) 1U << 25U) /* enable transmit */
        | (uint32_t) ((uint32_t) 1U << 24U) /* enable receive */
        | (uint32_t) ((uint32_t) 1U << 5U) /* internal clock (device has no clock pin) */
        | (uint32_t) ((uint32_t) (2U - 1U) << 4U) /* number of stop bits */
        | (uint32_t) ((uint32_t) 0U << 3U) /* even parity, otherwise odd */
        | (uint32_t) ((uint32_t) 0U << 2U) /* enable parity */
        | (uint32_t) ((uint32_t) 1U << 1U); /* asynchronous timing mode */

    /* set baudrate */
    sci->BRS = 487U;

    /* transmission length */
    sci->frameFormatControl = 8U - 1U; /* length */

    /* set SCI pins functional mode */
    sci->pioControl0 = (uint32_t) ((uint32_t) 1U << 2U) /* tx pin */
        | (uint32_t) ((uint32_t) 1U << 1U); /* rx pin */

    /* set SCI pins default output value */
    sci->pioControl3 = (uint32_t) ((uint32_t) 0U << 2U) /* tx pin */
        | (uint32_t) ((uint32_t) 0U << 1U); /* rx pin */

    /* set SCI pins output direction */
    sci->pioControl1 = (uint32_t) ((uint32_t) 0U << 2U) /* tx pin */
        | (uint32_t) ((uint32_t) 0U << 1U); /* rx pin */

    /* set SCI pins open drain enable */
    sci->pioControl6 = (uint32_t) ((uint32_t) 0U << 2U) /* tx pin */
        | (uint32_t) ((uint32_t) 0U << 1U); /* rx pin */

    /* set SCI pins pullup/pulldown enable */
    sci->pioControl7 = (uint32_t) ((uint32_t) 0U << 2U) /* tx pin */
        | (uint32_t) ((uint32_t) 0U << 1U); /* rx pin */

    /* set SCI pins pullup/pulldown select */
    sci->pioControl8 = (uint32_t) ((uint32_t) 1U << 2U) /* tx pin */
        | (uint32_t) ((uint32_t) 1U << 1U); /* rx pin */

    /* set interrupt level */
    sci->interruptSetLevel = (uint32_t) ((uint32_t) 0U << 26U) /* Framing error */
        | (uint32_t) ((uint32_t) 0U << 25U) /* Overrun error */
        | (uint32_t) ((uint32_t) 0U << 24U) /* Parity error */
        | (uint32_t) ((uint32_t) 0U << 9U) /* Receive */
        | (uint32_t) ((uint32_t) 0U << 8U) /* Transmit */
        | (uint32_t) ((uint32_t) 0U << 1U) /* Wakeup */
        | (uint32_t) ((uint32_t) 0U << 0U); /* Break detect */

    /* set interrupt enable */
    sci->interruptEnableSet = (uint32_t) ((uint32_t) 0U << 26U) /* Framing error */
        | (uint32_t) ((uint32_t) 0U << 25U) /* Overrun error */
        | (uint32_t) ((uint32_t) 0U << 24U) /* Parity error */
        | (uint32_t) ((uint32_t) 0U << 9U) /* Receive */
        | (uint32_t) ((uint32_t) 0U << 1U) /* Wakeup */
        | (uint32_t) ((uint32_t) 0U << 0U); /* Break detect */

    /* start SCI */
    sci->globalControl1 |= 0x80U;
}


void pharosCpuSciWrite(ptrPharosCpuSci sci , char c)
{
    /* wait until the SCI is ready for transmission */
    while((sci->flags & SCI_TRANSMIT_READY) == 0U);

    /* transmit the data */
    sci->transmitBuffer = (uint32_t) (c);
}
