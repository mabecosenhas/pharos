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


#ifndef PHAROS_VIRT_DECLARATIONS_H
#define PHAROS_VIRT_DECLARATIONS_H


#include <pharos/hal/hal.h>

typedef enum
{
    /**
     * Will not interrupt ("never interrupt")
     */
    PHAROS_INTERRUPT_PRIORITY_0 = 0U ,

    /**
     * Lowest priority 
     */
    PHAROS_INTERRUPT_PRIORITY_1 = 1U ,
    PHAROS_INTERRUPT_PRIORITY_2 = 2U ,
    PHAROS_INTERRUPT_PRIORITY_3 = 3U ,
    PHAROS_INTERRUPT_PRIORITY_4 = 4U ,
    PHAROS_INTERRUPT_PRIORITY_5 = 5U ,
    PHAROS_INTERRUPT_PRIORITY_6 = 6U ,

    /**
     * Highest priority
     */
    PHAROS_INTERRUPT_PRIORITY_7 = 7U ,
} PharosInterruptPriority;

typedef enum
{
    /************************************************************
     *   CPU specific interrupts                                *
     ************************************************************/

    PHAROS_CPU_ISR_VIRTIO_1 = 1U ,
    PHAROS_CPU_ISR_VIRTIO_2 = 2U ,
    PHAROS_CPU_ISR_VIRTIO_3 = 3U ,
    PHAROS_CPU_ISR_VIRTIO_4 = 4U ,
    PHAROS_CPU_ISR_VIRTIO_5 = 5U ,
    PHAROS_CPU_ISR_VIRTIO_6 = 6U ,
    PHAROS_CPU_ISR_VIRTIO_7 = 7U ,
    PHAROS_CPU_ISR_VIRTIO_8 = 8U ,

    /**
     * UART interrupt
     */
    PHAROS_CPU_ISR_UART0 = 10U ,

    PHAROS_CPU_ISR_PCIE_IRQ_1 = 0x20U ,
    PHAROS_CPU_ISR_PCIE_IRQ_2 = 0x21U ,
    PHAROS_CPU_ISR_PCIE_IRQ_3 = 0x22U ,
    PHAROS_CPU_ISR_PCIE_IRQ_4 = 0x23U ,

    PHAROS_CPU_ISR_VIRTIO_NDEV = 0x35U ,

    /**
     * Last interrupt
     */
    PHAROS_CPU_ISR_LAST = PHAROS_CPU_ISR_VIRTIO_NDEV ,

    /**
     * Contains the number of interrupts
     */
    PHAROS_CPU_INTERRUPT_NUMBER = PHAROS_CPU_ISR_LAST + 1U ,
} PharosInterruptNumber;



/* PHAROS_VIRT_DECLARATIONS_H */
#endif 
