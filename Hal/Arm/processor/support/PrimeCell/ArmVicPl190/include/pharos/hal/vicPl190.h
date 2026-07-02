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


#ifndef PHAROS_VICPL190_H
#define PHAROS_VICPL190_H


#include <pharos/hal/hal.h>
#include <pharos/hal/board.h>
#include <pharos/kernel/interrupt/declarations.h>
#include <pharos/kernel/partition/declarations.h>
#include <pharos/kernel/collection/linkedlist.h>



/**
 * Compact the size of the I/O registers
 */
#pragma pack(push ,4)

typedef struct
{
    /* 0x0 */
    volatile uint32_t irqStatus;

    /* 0x4 */
    volatile uint32_t fiqStatus;

    /* 0x8 */
    volatile uint32_t rawInterruptStatus;

    /* 0xc */
    volatile uint32_t interruptSelect;

    /* 0x10 */
    volatile uint32_t interruptEnable;

    /* 0x14 */
    volatile uint32_t interruptEnableClear;

    /* 0x18 */
    volatile uint32_t softwareInterrupt;

    /* 0x1c */
    volatile uint32_t softwareInterruptClear;

    /* 0x20 */
    volatile uint32_t protection;

    /* 0x24 */
    volatile uint32_t reserved1;

    /* 0x28 */
    volatile uint32_t reserved2;

    /* 0x2c */
    volatile uint32_t reserved3;

    /* 0x30 */
    volatile uint32_t vectorAddress;

    /* 0x34 */
    volatile uint32_t defaultVectorAddress;

    /* 0x38 - 0x9c */
    volatile uint32_t reserved4[50];

    /* 0x100 */
    volatile uint32_t vectorAddressRegisters[16];

    /* 0x140 */
    volatile uint32_t reserved5[48];

    /* 0x200 */
    volatile uint32_t vectorControlRegisters[16];

    /* 0x240 */
    volatile uint32_t reserved6[872];

    /* 0xfe0 */
    volatile uint32_t peripheralId0;

    /* 0xfe4 */
    volatile uint32_t peripheralId1;

    /* 0xfe8 */
    volatile uint32_t peripheralId2;

    /* 0xfec */
    volatile uint32_t peripheralId3;

    /* 0xff0 */
    volatile uint32_t id0;

    /* 0xff4 */
    volatile uint32_t id1;

    /* 0xff8 */
    volatile uint32_t id2;

    /* 0xffc */
    volatile uint32_t id3;

} ArmVicPl190 , *ptrArmVicPl190;


/**
 * restore the previous pack definition
 */
#pragma pack(pop)

/**
 * Priority of an interrupt
 */
typedef enum
{
    /**
     * Highest priority interrupt
     */
    PHAROS_INTERRUPT_PRIORITY_0 = 0 ,
    PHAROS_INTERRUPT_PRIORITY_1 ,
    PHAROS_INTERRUPT_PRIORITY_2 ,
    PHAROS_INTERRUPT_PRIORITY_3 ,
    PHAROS_INTERRUPT_PRIORITY_4 ,
    PHAROS_INTERRUPT_PRIORITY_5 ,
    PHAROS_INTERRUPT_PRIORITY_6 ,
    PHAROS_INTERRUPT_PRIORITY_7 ,
    PHAROS_INTERRUPT_PRIORITY_8 ,
    PHAROS_INTERRUPT_PRIORITY_9 ,
    PHAROS_INTERRUPT_PRIORITY_10 ,
    PHAROS_INTERRUPT_PRIORITY_11 ,
    PHAROS_INTERRUPT_PRIORITY_12 ,
    PHAROS_INTERRUPT_PRIORITY_13 ,
    PHAROS_INTERRUPT_PRIORITY_14 ,

    /**
     * Lowest priority interrupt that still has a number. Still has higher priority than the interrupts defined with PHAROS_INTERRUPT_PRIORITY_DEFAULT
     */
    PHAROS_INTERRUPT_PRIORITY_15 = 15 ,

    /**
     * Lowest interrupt priority. All interrupts with this priority are handled the same (have the same priority)
     */
    PHAROS_INTERRUPT_PRIORITY_DEFAULT = 16 ,
} PharosInterruptPriority , *ptrPharosInterruptPriority;

struct PharosCpuInterruptConfiguration
{
    /**
     * Priority of the interrupt
     * 
     * @note application must initialize this number
     */
    PharosInterruptPriority priority;

};


/**
 * Initialize the Vector Interrupt Controller 
 *  
 * @param pl190 address to the Vector Interrupt Controller
 */
void armVicPl190Initialize(ptrArmVicPl190 pl190);


/**
 * Unmask an interrupt 
 * 
 * @param number interrupt number
 * @param pl190 address to the Vector Interrupt Controller
 */
void armVicPl190UnmaskInterrupt(PharosInterruptNumber number , ptrArmVicPl190 pl190);


/**
 * Mask an interrupt (interrupt will not be triggered)
 * 
 * @param number interrupt number 
 * @param pl190 address to the Vector Interrupt Controller
 */
void armVicPl190MaskInterrupt(PharosInterruptNumber number , ptrArmVicPl190 pl190);


/**
 * Install an interrupt on the PL190
 * 
 * @param conf interrupt configuration
 * @param pl190 address to the Vector Interrupt Controller
 */
void pharosCpuPl190InterruptInstall(ptrInterruptConfiguration conf , ptrArmVicPl190 pl190);


/**
 * Uninstall an interrupt on the PL190
 * 
 * @param conf interrupt configuration
 * @param pl190 address to the Vector Interrupt Controller
 */
void pharosCpuPl190InterruptRemove(ptrInterruptConfiguration conf , ptrArmVicPl190 pl190);


/**
 * Gets the active interrupts
 * 
 * @param pl190 address to the Vector Interrupt Controller
 * 
 * @return returns the active interrupts (one bit per active interrupt)
 */
INLINE uint32_t pharosCpuPl190ActiveInterupts(ptrArmVicPl190 pl190)
{
    return pl190->vectorAddress;
}


/**
 * Clear the active interrupts on the PL190
 * 
 * @param pl190 address to the Vector Interrupt Controller
 */
INLINE void pharosCpuPl190ClearActiveInterupts(ptrArmVicPl190 pl190)
{
    /* write something to the vector address to clear the interrupts */
    pl190->vectorAddress = 0U;
}


#endif /* PHAROS_VICPL190_H */
