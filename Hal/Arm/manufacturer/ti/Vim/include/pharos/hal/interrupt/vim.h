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


#ifndef PHAROS_TI_VIM_H
#define	PHAROS_TI_VIM_H

#ifdef	__cplusplus
extern "C"
{
#endif

#include <pharos/hal/cpu/cpu.h>
#include <pharos/hal/cpu/conf.h>
#include <pharos/hal/hal.h>
#include <pharos/kernel/interrupt/declarations.h>
#include <pharos/kernel/partition/declarations.h>
#include <pharos/hal/cpu/declarations.h>

    typedef struct PharosCpuVim PharosCpuVim , *ptrPharosCpuVim;


    typedef void (*pharosCpuVimEccErrorHandler)(void);

    /************************************************************
     * ECCSTAT register definitions                              *
     ************************************************************/
#define PHAROS_CPU_VIM_ECCSTAT_SBE_CLR              (1U << 8U)
#define PHAROS_CPU_VIM_ECCSTAT_DBE_CLR              (1U << 0U)



    /************************************************************
     * ECCCTL register definitions                              *
     ************************************************************/
#define PHAROS_CPU_VIM_ECCCTL_SBE_EN                (0xAU << 24U)
#define PHAROS_CPU_VIM_ECCCTL_SBE_DIS               (0x5U << 24U)

#define PHAROS_CPU_VIM_ECCCTL_EDAC_EN               (0xAU << 16U)
#define PHAROS_CPU_VIM_ECCCTL_EDAC_DIS              (0x5U << 16U)

#define PHAROS_CPU_VIM_ECCCTL_ECC_EN                (0xAU << 0U)
#define PHAROS_CPU_VIM_ECCCTL_ECC_DIS               (0x5U << 0U)

    /************************************************************
     * CAPEVT register definitions                              *
     ************************************************************/
#define PHAROS_CPU_VIM_CAPEVT_SRC1(x)               (x << 16U)
#define PHAROS_CPU_VIM_CAPEVT_SRC0(x)               (x << 0U)

    /**
     * Compact the size of the I/O registers
     */
#pragma pack(push ,4)


    struct PharosCpuVim
    {
        /**
         * Reserved
         * 
         * Offset: 0x0 - 0xe8
         */
        volatile uint32_t reserved1[59U];

        /**
         * Interrupt Vector Table ECC Status Register(ECCSTAT)
         * 
         * Contains the status of the ECC error on the interrupt vector table
         * 
         * Offset: 0xec
         */
        volatile uint32_t interruptVectorTableEccStatus;

        /**
         * Interrupt Vector Table ECC Control Register (ECCCTL)
         * 
         * Controls the ECC error detection and handling inside the Interrupt Vector Table.
         * 
         * Offset: 0xf0
         */
        volatile uint32_t interruptVectorTableEccControl;

        /**
         * Uncorrectable Error Address Register (UERRADDR)
         * 
         * The uncorrectable error address register gives the address of the first uncorrectable error location.
         * 
         * Offset: 0xf4
         */
        volatile uint32_t *uncorrectableErrorAddress;

        /**
         * Fallback Vector Address Register (FBVECADDR)
         * 
         * This register provides a fall-back address to the VIM if a uncorrectable error has occurred in the Interrupt
         * Vector Table
         * 
         * Offset: 0xf8
         */
        volatile pharosCpuVimEccErrorHandler fallBackVectorAddress;

        /**
         * Single Bit Error Address Register (SBERRADDR)
         * 
         * Address of the first single bit ECC error detected by the ECC logic
         * 
         * Offset: 0xfc
         */
        volatile uint8_t *singleBitErrorAddress;

        /**
         * IRQ Index Offset Vector Register (IRQINDEX)
         * 
         * The register provides the user with the numerical index value that represents the pending IRQ
         * interrupt with the highest priority
         * 
         * Offset: 0x100
         */
        volatile uint32_t irqIndex;

        /**
         * FIQ Index Offset Vector Registers (FIQINDEX)
         * 
         * The register provides the user with a numerical index value that represents the pending FIQ 
         * interrupt with the highest priority
         * 
         * Offset: 0x104
         */
        volatile uint32_t fiqIndex;

        /**
         * Reserved
         * 
         * Offset: 0x108
         */
        volatile uint32_t reserved2;

        /**
         * Reserved
         * 
         * Offset: 0x10c
         */
        volatile uint32_t reserved3;

        /**
         * FIQ/IRQ Program Control Register 0 (FIRQPR0)
         * 
         * Determines whether a given interrupt request will be either a FIQ or an IRQ (for interrupts 2 to 31)
         * Interrupt 0 and 1 are always FIQ
         * 
         * Offset: 0x110
         */
        volatile uint32_t fiqIrqProgramControl0;

        /**
         * FIQ/IRQ Program Control Register 1 (FIRQPR1)
         * 
         * Determines whether a given interrupt request will be either a FIQ or an IRQ (for interrupts 32 to 63)
         * 
         * Offset: 0x114
         */
        volatile uint32_t fiqIrqProgramControl1;

        /**
         * FIQ/IRQ Program Control Register 2 (FIRQPR2)
         * 
         * Determines whether a given interrupt request will be either a FIQ or an IRQ (for interrupts 64 to 95)
         * 
         * Offset: 0x118
         */
        volatile uint32_t fiqIrqProgramControl2;

        /**
         * FIQ/IRQ Program Control Register 3 (FIRQPR3)
         * 
         * Determines whether a given interrupt request will be either a FIQ or an IRQ (for interrupts 96 to 127)
         * 
         * Offset: 0x114
         */
        volatile uint32_t fiqIrqProgramControl3;

        /**
         * Pending Interrupt Read Location Registers 0 (INTREQ0)
         * 
         * gives the pending interrupt requests for interrupts 0-31
         * 
         * Offset: 0x120
         */
        volatile uint32_t interruptPending0;

        /**
         * Pending Interrupt Read Location Registers 1 (INTREQ1)
         * 
         * gives the pending interrupt requests for interrupts 32-63
         * 
         * Offset: 0x124
         */
        volatile uint32_t interruptPending1;

        /**
         * Pending Interrupt Read Location Registers 2 (INTREQ2)
         * 
         * gives the pending interrupt requests for interrupts 64-95
         * 
         * Offset: 0x128
         */
        volatile uint32_t interruptPending2;

        /**
         * Pending Interrupt Read Location Registers 3 (INTREQ3)
         * 
         * gives the pending interrupt requests for interrupts 96-127
         * 
         * Offset: 0x12c
         */
        volatile uint32_t interruptPending3;

        /**
         * Interrupt Enable Set Register 0 (REQENASET0)
         * 
         * selectively enables individual request channels (for channels 2 to 31)
         * Channel 0 and channel 1 are always enabled.
         * 
         * Offset: 0x130
         */
        volatile uint32_t interruptEnableSet0;

        /**
         * Interrupt Enable Set Register 1 (REQENASET1)
         * 
         * selectively enables individual request channels (for channels 32 to 63)
         * 
         * Offset: 0x134
         */
        volatile uint32_t interruptEnableSet1;

        /**
         * Interrupt Enable Set Register 2 (REQENASET2)
         * 
         * selectively enables individual request channels (for channels 64 to 95)
         * 
         * Offset: 0x138
         */
        volatile uint32_t interruptEnableSet2;

        /**
         * Interrupt Enable Set Register 3 (REQENASET3)
         * 
         * selectively enables individual request channels (for channels 96 to 127)
         * 
         * Offset: 0x13c
         */
        volatile uint32_t interruptEnableSet3;

        /**
         * Interrupt Enable Clear Register 0 (REQENACLR0)
         * 
         * selectively disable individual request channels (for channels 2 to 31)
         * Channel 0 and channel 1 are always enabled.
         * 
         * Offset: 0x140
         */
        volatile uint32_t interruptEnableClear0;

        /**
         * Interrupt Enable Clear Register 1 (REQENACLR1)
         * 
         * selectively disable individual request channels (for channels 32 to 63)
         * 
         * Offset: 0x144
         */
        volatile uint32_t interruptEnableClear1;

        /**
         * Interrupt Enable Clear Register 2 (REQENACLR2)
         * 
         * selectively disable individual request channels (for channels 64 to 95)
         * 
         * Offset: 0x148
         */
        volatile uint32_t interruptEnableClear2;

        /**
         * Interrupt Enable Clear Register 3 (REQENACLR3)
         * 
         * selectively disable individual request channels (for channels 96 to 127)
         * 
         * Offset: 0x14c
         */
        volatile uint32_t interruptEnableClear3;

        /**
         * Wake-Up Enable Set Register 0 (WAKEENASET0)
         * 
         * enables individual wake-up interrupt request lines (interrupt 0 to 31)
         * 
         * Offset: 0x150
         */
        volatile uint32_t wakeUpEnableSet0;

        /**
         * Wake-Up Enable Set Register 1 (WAKEENASET1)
         * 
         * enables individual wake-up interrupt request lines (interrupt 32 to 63)
         * 
         * Offset: 0x154
         */
        volatile uint32_t wakeUpEnableSet1;

        /**
         * Wake-Up Enable Set Register 2 (WAKEENASET2)
         * 
         * enables individual wake-up interrupt request lines (interrupt 64 to 95)
         * 
         * Offset: 0x158
         */
        volatile uint32_t wakeUpEnableSet2;

        /**
         * Wake-Up Enable Set Register 3 (WAKEENASET3)
         * 
         * enables individual wake-up interrupt request lines (interrupt 96 to 127)
         * 
         * Offset: 0x15c
         */
        volatile uint32_t wakeUpEnableSet3;

        /**
         * Wake-Up Enable Clear Register 0 (WAKEENACLR0)
         * 
         * disables individual wake-up interrupt request lines (interrupt 0 to 31)
         * 
         * Offset: 0x160
         */
        volatile uint32_t wakeUpEnableClear0;

        /**
         * Wake-Up Enable Clear Register 1 (WAKEENACLR1)
         * 
         * disables individual wake-up interrupt request lines (interrupt 32 to 63)
         * 
         * Offset: 0x164
         */
        volatile uint32_t wakeUpEnableClear1;

        /**
         * Wake-Up Enable Clear Register 2 (WAKEENACLR2)
         * 
         * disables individual wake-up interrupt request lines (interrupt 64 to 95)
         * 
         * Offset: 0x168
         */
        volatile uint32_t wakeUpEnableClear2;

        /**
         * Wake-Up Enable Clear Register 3 (WAKEENACLR3)
         * 
         * disables individual wake-up interrupt request lines (interrupt 96 to 127)
         * 
         * Offset: 0x16c
         */
        volatile uint32_t wakeUpEnableClear3;

        /**
         * IRQ Interrupt Vector Register (IRQVECREG)
         * 
         * address of the enabled and active IRQ (address of the highest pending IRQ request)
         * 
         * Offset: 0x170
         */
        volatile uint32_t irqHighestVector;

        /**
         * FIQ Interrupt Vector Register (FIQVECREG)
         * 
         * address of the enabled and active FIQ (address of the highest pending FIQ request)
         * 
         * Offset: 0x174
         */
        volatile uint32_t fiqHighestVector;

        /**
         * Capture Event Register (CAPEVT)
         * 
         * Controls which interrupt request maps to the capture event source 0 and 1 of the RTI (Real-Time Interrupt)
         * 
         * Offset: 0x178
         */
        volatile uint32_t captureEvent;

        /**
         * Reserved
         * 
         * Offset: 0x17c
         */
        volatile uint32_t reserved4;

        /**
         * VIM Interrupt Control Registers (CHANCTRL[0:31])
         * 
         * Thirty-two interrupt control registers control the 128 interrupt channels of the VIM. Each register controls
         * four interrupt channels: each of them is indexed from 0 to 127
         * 
         * For example, CHANCTRL[0] controls MAP0, MAP1, MAP2 and MAP3. Each MAPx contain the interrupt request for the 
         * associated channel. MAP0 contains the interrupt request for channel 0 (7 bits - 0 to 127).
         * 
         * Offset: 0x180-0x2fc
         */
        volatile uint32_t channelControlTable[32U];
    };


    /**
     * restore the previous pack definition
     */
#pragma pack(pop)


    /**
     * Configuration of an interrupt under the VIM interrupt controller
     * 
     * Please refer to the proper documentation since the VIM is a quite complex interrupt controller.
     * 
     * TODO: could add definition if interrupt is FIQ or IRQ. For now all are IRQ 
     */
    struct PharosCpuInterruptConfiguration
    {
        /**
         * Channel of the interrupt
         * Channel 0 and 1 are reserved.
         * 
         * @note: application must initialize this number
         */
        PharosInterruptChannel channel;

        /**
         * Flag indicating if the interrupt should wake-up the CPU if it is dormant
         * TRUE will wake-up 
         * FALSE will not wake-up
         * 
         * @note: application must initialize this number
         */
        bool isWakeUp;
    };


    /**
     * Initialize the Vector Interrupt Controller 
     *  
     * @param address address to the Vector Interrupt Controller
     */
    void pharosCpuVimInitialize(ptrInterruptTable table);


    /**
     * Interrupt handler for all interrupts
     */
    void pharosCpuIsrHandler(void);


    /**
     * Install a particular interrupt
     * 
     * @param conf interrupt configuration to install
     */
    void pharosCpuVimInitializeIsrConf(ptrInterruptConfiguration conf);



#ifdef	__cplusplus
}
#endif

#endif	/* VIM_H */
