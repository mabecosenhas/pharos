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


#ifndef PHAROS_MBOX_H
#define	PHAROS_MBOX_H

#include <pharos/hal/board.h>
#include <pharos/hal/board/declarations.h>


#define PHAROS_MBOX_TAG_REQUEST        (0U)
#define PHAROS_MBOX_TAG_GETSERIAL      (0x10004U)
#define PHAROS_MBOX_TAG_SETCLKRATE     (0x38002U)
#define PHAROS_MBOX_TAG_LAST           (0U)


typedef enum
{
    PHAROS_CPU_MBOX_CHANNEL_POWER = 0U ,
    PHAROS_CPU_MBOX_CHANNEL_FB = 1U ,
    PHAROS_CPU_MBOX_CHANNEL_VUART = 2U ,
    PHAROS_CPU_MBOX_CHANNEL_VCHIQ = 3U ,
    PHAROS_CPU_MBOX_CHANNEL_LEDS = 4U ,
    PHAROS_CPU_MBOX_CHANNEL_BTNS = 5U ,
    PHAROS_CPU_MBOX_CHANNEL_TOUCH = 6U ,
    PHAROS_CPU_MBOX_CHANNEL_COUNT = 7U ,
    PHAROS_CPU_MBOX_CHANNEL_PROP = 8U ,
} PharosCpuMboxChannel;


#define PHAROS_CPU_MBOX_ADDRESS                 (PHAROS_CPU_IO_BASE + 0x0000B880U)

#define PHAROS_CPU_MBOX                         ((ptrPharosCpuMbox) (PHAROS_CPU_MBOX_ADDRESS))

/**
 * Compact the size of the I/O registers
 */
#pragma pack(push ,4)


struct PharosCpuMbox
{
    /**
     * Read register
     * 
     * offset: 0x0
     */
    volatile uint32_t read;

    /**
     * offset: 0x4
     */
    uint32_t reserved1;

    /**
     * offset: 0x8
     */
    uint32_t reserved2;

    /**
     * offset: 0xc
     */
    uint32_t reserved3;

    /**
     * Poll register
     * 
     * offset: 0x10
     */
    volatile uint32_t poll;

    /**
     * Sender register
     * 
     * offset: 0x14
     */
    volatile uint32_t sender;

    /**
     * Status register
     * 
     * offset: 0x18
     */
    volatile uint32_t status;

    /**
     * Configuration register
     * 
     * offset: 0x1c
     */
    volatile uint32_t config;

    /**
     * Write register
     * 
     * offset: 0x20
     */
    volatile uint32_t write;

};


/**
 * restore the previous pack definition
 */
#pragma pack(pop)


/**
 * Make a mailbox call. Returns 0 on failure, non-zero on success
 */
int32_t pharosCpuMboxCall(PharosCpuMboxChannel ch , uint32_t mbox[]);



/* PHAROS_MBOX_H */
#endif	
