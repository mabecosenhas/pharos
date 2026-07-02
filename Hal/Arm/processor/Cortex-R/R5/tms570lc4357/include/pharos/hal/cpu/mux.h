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


#ifndef PHAROS_MUX_H
#define	PHAROS_MUX_H

#ifdef	__cplusplus
extern "C"
{
#endif

#include <pharos/hal/cpu/declarations.h>
#include <pharos/hal/cpu/conf.h>

#define PHAROS_CPU_Iomm      ((ptrPharosCpuIomm) PHAROS_CPU_IOMM_ADDRESS) 

    /************************************************************
     * KICK_REG0 register definitions                           *
     ************************************************************/
#define PHAROS_CPU_IOMM_KICK0_EN                       (0x83E70B13U)
#define PHAROS_CPU_IOMM_KICK0_DIS                      (0x00000001U)

    /************************************************************
     * KICK_REG1 register definitions                           *
     ************************************************************/
#define PHAROS_CPU_IOMM_KICK1_EN                       (0x95A4F1E0U)
#define PHAROS_CPU_IOMM_KICK1_DIS                      (0x00000001U)


    /**
     * Compact the size of the I/O registers
     */
#pragma pack(push ,4)


    struct PharosCpuIomm
    {
        /**
         * Revision Register (REVISION_REG)
         * 
         * Read-only register Containing the revision number of the IOMM
         * 
         * Offset: 0x0
         */
        volatile uint32_t revision;

        /**
         * Reserved
         * 
         * Offset: 0x4
         */
        volatile uint32_t reserved1[7];

        /**
         * Boot mode Register (BOOT_REG)
         * 
         * Read-only register containing the little or big-endian configuration
         * 
         * Offset: 0x20
         */
        volatile uint32_t bootMode;

        /**
         * Reserved
         * 
         * Offset: 0x24
         */
        volatile uint32_t reserved2[5];

        /**
         * Kicker Register 0 (KICK_REG0)
         * 
         * Kicker 0 Register. The value 83E7 0B13h must be written to KICK0 as part of the process to unlock the CPU 
         * write access to the PINMMRnn registers
         * 
         * Offset: 0x38
         */
        volatile uint32_t kicker0;

        /**
         *  Kicker Register 1 (KICK_REG1)
         * 
         * The value 95A4 F1E0h must be written to the KICK1 as part of the process to unlock the CPU 
         * write access to the PINMMRnn registers.
         * 
         * Offset: 0x3c
         */
        volatile uint32_t kicker1;

        /**
         * Reserved
         * 
         * Offset: 0x40
         */
        volatile uint32_t reserved3[40];

        /**
         * Error Raw Status / Set Register (ERR_RAW_STATUS_REG)
         * 
         * This register shows the status of the error conditions (before enabling) and allows setting the error status
         * 
         * Offset: 0xe0
         */
        volatile uint32_t errorRawStatus;

        /**
         * Error Enabled Status / Clear Register (ERR_ENABLED_STATUS_REG)
         * 
         * This register shows the status of the error conditions and allows clearing of the error status.
         * 
         * Offset: 0xe4
         */
        volatile uint32_t errorEnabledStatus;

        /**
         * Error Signaling Enable Register (ERR_ENABLE_REG)
         * 
         * This register shows the interrupt enable status and allows enabling of the interrupts
         * 
         * Offset: 0xe8
         */
        volatile uint32_t errorSignalEnable;

        /**
         * Error Signaling Enable Clear Register (ERR_ENABLE_CLR_REG)
         * 
         * This register shows the error signaling enable status and allows disabling of the error signaling
         * 
         * Offset: 0xec
         */
        volatile uint32_t errorSignalEnableClear;

        /**
         * Reserved
         * 
         * Offset: 0xf0
         */
        volatile uint32_t reserved4;

        /**
         * Fault Address Register (FAULT_ADDRESS_REG)
         * 
         * This register holds the address offset of the first fault transfer.
         * 
         * Offset: 0xf4
         */
        volatile uint32_t faultAddress;

        /**
         * Fault Status Register (FAULT_STATUS_REG)
         * 
         * This register holds the status and attributes of the first fault transfer
         * 
         * Offset: 0xf8
         */
        volatile uint32_t faultStatus;

        /**
         * Fault Clear Register (FAULT_CLEAR_REG)
         * 
         * This register allows the application to clear the current fault so that another can be captured when 1 is
         * written to this register.
         * 
         * Offset: 0xfc
         */
        volatile uint32_t faultClear;

        /**
         * Reserved
         * 
         * Offset: 0x100
         */
        volatile uint32_t reserved5[4];

        /**
         * Output Pin Multiplexing Control Registers (PINMMRnn)
         * 
         * These registers control the output multiplexing of the functionality available on each pad on the
         * microcontroller. There are 38 such registers – PINMMR0 through PINMMR37. Each 8-bit field of a
         * PINMMR register controls the functionality of a single ball/pin.
         * 
         * Offset: 0x110
         */
        volatile uint32_t outputPinMuxControl[38];

        /**
         * Reserved
         * 
         * Offset: 0x1A8
         */
        volatile uint32_t reserved6[42];

        /**
         * Input Pin Multiplexing Control Registers (PINMMRnn)
         * 
         * These registers control the input multiplexing of the functionality available on each pad on the
         * microcontroller. There are 20 such registers – PINMMR80 through PINMMR99. Each 8-bit field of a 
         * PINMMR register controls the functionality of a single ball/pin.
         * 
         * Offset: 0x250
         */
        volatile uint32_t inputPinMuxControl[20];

        /**
         * Reserved
         * 
         * Offset: 0x2A0
         */
        volatile uint32_t reserved7[60];

        /**
         * Special Functionality Multiplexing Control Registers (PINMMRnn)
         * 
         * These registers control the special functionalities on the microcontroller. There are 20 such registers –
         * PINMMR160 through PINMMR179. Each 8-bit field of a PINMMR register controls one special functionality.
         * 
         * Offset: 0x390
         */
        volatile uint32_t specialPinMuxControl[20];
    };


    /**
     * restore the previous pack definition
     */
#pragma pack(pop)


    /**
     * Initialize the I/O Multiplexing
     */
    void pharosCpuIoMultiplexingInitialize(void);


#ifdef	__cplusplus
}
#endif

#endif	/* MUX_H */
