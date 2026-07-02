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


#ifndef PHAROS_VICPL192_H
#define PHAROS_VICPL192_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <pharos/hal/hal.h>
#include <pharos/kernel/interrupt/declarations.h>
#include <pharos/kernel/partition/declarations.h>
#include <pharos/kernel/collection/linkedlist.h>

    typedef uint32_t PharosInterruptNumber;

    /**
     * Handler to clear the interrupt
     */
    typedef void (*PharosInterruptClearHandler)();

    typedef uint32_t PharosInterruptNestNumber VARIABLE_ALIGNMENT_X(4);


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
        volatile uint32_t swPriorityMask;

        /* 0x28 */
        volatile uint32_t vectorPriority4DaisyChain;

        /* 0x2c */
        volatile uint32_t reserved1;

        /* 0x30 */
        volatile uint32_t reserved2;

        /* 0x34 */
        volatile uint32_t reserved3;

        /* 0x38 - 0x9c */
        volatile uint32_t reserved4[50];

        /* 0x100 */
        volatile uint32_t vectorAddressRegisters[32];

        /* 0x140 */
        volatile uint32_t reserved5[32];

        /* 0x200 */
        volatile uint32_t vectorPriorityRegisters[32];

        /* 0x240 */
        volatile uint32_t reserved6[816];

        /* 0xf00 */
        volatile uint32_t vectorAddress;

        /* 0xf04 */
        volatile uint32_t reserved7[55];

        /* 0xfe0 */
        volatile uint32_t peripheralId0;

        /* 0xfe4 */
        volatile uint32_t peripheralId1;

        /* 0xfe8 */
        volatile uint32_t peripheralId2;

        /* 0xfec */
        volatile uint32_t peripheralId3;

        /* 0xff0 */
        volatile uint32_t cellId0;

        /* 0xff4 */
        volatile uint32_t cellId1;

        /* 0xff8 */
        volatile uint32_t cellId2;

        /* 0xffc */
        volatile uint32_t cellId3;

    } ArmVicPl192 , *ptrArmVicPl192;


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
         * Lowest priority interrupt
         */
        PHAROS_INTERRUPT_PRIORITY_15 = 15 ,

    } PharosInterruptPriority , *ptrPharosInterruptPriority;


    struct InterruptConfiguration
    {
        /**
         * number of the interrupt to catch
         * 
         * @note: application must initialize this number
         */
        PharosInterruptNumber number;

        /**
         * Priority of the interrupt
         * 
         * @note: application must initialize this number
         */
        PharosInterruptPriority priority;

        /**
         * Handler to clear the interrupt
         * 
         * @note: application must initialize this number
         */
        PharosInterruptClearHandler clearInterrupt;

        /**
         * Flag indicating if the interrupt is already installed 
         */
        bool isInstalled;
    };


    /**
     * Initialize the Vector Interrupt Controller 
     *  
     * @param address address to the Vector Interrupt Controller
     */
    void armVicPl192Initialize(ptrArmVicPl192 address , ptrInterruptTable table);


    /**
     * Unmask an interrupt 
     * 
     * @param number interrupt number
     */
    void armVicPl192UnmaskInterrupt(PharosInterruptNumber number);


    /**
     * Mask an interrupt (interrupt will not be triggered)
     * 
     * @param number interrupt number 
     */
    void armVicPl192MaskInterrupt(PharosInterruptNumber number);




#ifdef __cplusplus
}
#endif

#endif /* VICPL190_H */
