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


#ifndef PHAROS_PERIPHERAL_H
#define	PHAROS_PERIPHERAL_H

#ifdef	__cplusplus
extern "C"
{
#endif

#include <pharos/hal/cpu/declarations.h>

#define PHAROS_CPU_PCR_1 ((ptrPharosCpuPcr) PHAROS_CPU_PCR1_ADDRESS) 
#define PHAROS_CPU_PCR_2 ((ptrPharosCpuPcr) PHAROS_CPU_PCR2_ADDRESS) 
#define PHAROS_CPU_PCR_3 ((ptrPharosCpuPcr) PHAROS_CPU_PCR3_ADDRESS) 

    /**
     * Compact the size of the I/O registers
     */
#pragma pack(push ,4)


    /**
     * TMS570LC53x peripheral frame control registers
     * 
     * Information retrieved from "TMS570LC43x 16/32-Bit RISC Flash Microcontroller, Technical Reference Manual, 
     * SPNU563, May 2014"
     */
    struct PharosCpuPcr
    {
        /**
         * Peripheral Memory Protection Set Register 0 (PMPROTSET0)
         * 
         * Peripheral memory frame protection set
         * 
         * Offset: 0x0
         */
        volatile uint32_t peripheralMemProtSet0;


        /**
         * Peripheral Memory Protection Set Register 1 (PMPROTSET1)
         * 
         * Peripheral memory frame protection set
         * 
         * Offset: 0x4
         */
        volatile uint32_t PMPROTSET1;

        /**
         * Reserved
         * 
         * Offset: 0x8
         */
        volatile uint32_t reserved0[2U];

        /**
         * Peripheral Memory Protection Clear Register 0 (PMPROTCLR0)
         * 
         * Peripheral memory frame protection clear
         * 
         * Offset: 0x10
         */
        volatile uint32_t PMPROTCLR0;

        /**
         * Peripheral Memory Protection Clear Register 1 (PMPROTCLR1)
         * 
         * Peripheral memory frame protection clear
         * 
         * Offset: 0x14
         */
        volatile uint32_t PMPROTCLR1;

        /**
         * Reserved
         * 
         * Offset: 0x18
         */
        volatile uint32_t reserved1[2U];

        /**
         * Peripheral Protection Set Register 0 (PPROTSET0)
         * 
         * Peripheral select quadrant protection set
         * 
         * Offset: 0x20
         */
        volatile uint32_t PPROTSET0;

        /**
         * Peripheral Protection Set Register 1 (PPROTSET1)
         * 
         * Peripheral select quadrant protection set
         * 
         * Offset: 0x24
         */
        volatile uint32_t PPROTSET1;

        /**
         * Peripheral Protection Set Register 2 (PPROTSET2)
         * 
         * Peripheral select quadrant protection set
         * 
         * Offset: 0x28
         */
        volatile uint32_t PPROTSET2;

        /**
         * Peripheral Protection Set Register 3 (PPROTSET3)
         * 
         * Peripheral select quadrant protection set
         * 
         * Offset: 0x2c
         */
        volatile uint32_t PPROTSET3;

        /**
         * Reserved
         * 
         * Offset: 0x30
         */
        volatile uint32_t reserved2[4U];

        /**
         * Peripheral Protection Clear Register 0 (PPROTCLR0)
         * 
         * Peripheral select quadrant protection clear
         * 
         * Offset: 0x40
         */
        volatile uint32_t PPROTCLR0;

        /**
         * Peripheral Protection Clear Register 1 (PPROTCLR1)
         * 
         * Peripheral select quadrant protection clear
         * 
         * Offset: 0x44
         */
        volatile uint32_t PPROTCLR1;

        /**
         * Peripheral Protection Clear Register 2 (PPROTCLR2)
         * 
         * Peripheral select quadrant protection clear
         * 
         * 
         * Offset: 0x48
         */
        volatile uint32_t PPROTCLR2;

        /**
         * Peripheral Protection Clear Register 3 (PPROTCLR3)
         * 
         * Peripheral select quadrant protection clear
         * 
         * 
         * Offset: 0x4c
         */
        volatile uint32_t PPROTCLR3;

        /**
         * Reserved
         * 
         * Offset: 0x50
         */
        volatile uint32_t reserved3[4U];

        /**
         * Peripheral Memory Power-Down Set Register 0 (PCSPWRDWNSET0)
         * 
         * Peripheral memory clock power-down set
         * 
         * Offset: 0x60
         */
        volatile uint32_t PCSPWRDWNSET0;

        /**
         * Peripheral Memory Power-Down Set Register 1 (PCSPWRDWNSET1)
         * 
         * Peripheral memory clock power-down set
         * 
         * Offset: 0x64
         */
        volatile uint32_t PCSPWRDWNSET1;

        /**
         * Reserved
         * 
         * Offset: 0x68
         */
        volatile uint32_t reserved4[2U];

        /**
         * Peripheral Memory Power-Down Clear Register 0 (PCSPWRDWNCLR0)
         * 
         * Peripheral memory clock power-down clear
         * 
         * Offset: 0x70
         */
        volatile uint32_t PCSPWRDWNCLR0;

        /**
         * Peripheral Memory Power-Down Clear Register 1 (PCSPWRDWNCLR1)
         * 
         * Peripheral memory clock power-down clear
         * 
         * Offset: 0x74
         */
        volatile uint32_t PCSPWRDWNCLR1;

        /**
         * Reserved
         * 
         * Offset: 0x78
         */
        volatile uint32_t reserved5[2U];

        /**
         * Peripheral Power-Down Set Register 0 (PSPWRDWNSET0)
         * 
         * Peripheral select quadrant clock power-down set
         * 
         * Offset: 0x80
         */
        volatile uint32_t PSPWRDWNSET0;

        /**
         * Peripheral Power-Down Set Register 1 (PSPWRDWNSET1)
         * 
         * Peripheral select quadrant clock power-down set
         * 
         * Offset: 0x84
         */
        volatile uint32_t PSPWRDWNSET1;

        /**
         * Peripheral Power-Down Set Register 2 (PSPWRDWNSET2)
         * 
         * Peripheral select quadrant clock power-down set
         * 
         * Offset: 0x88
         */
        volatile uint32_t PSPWRDWNSET2;

        /**
         * Peripheral Power-Down Set Register 3 (PSPWRDWNSET3)
         * 
         * Peripheral select quadrant clock power-down set
         * 
         * Offset: 0x8c
         */
        volatile uint32_t PSPWRDWNSET3;

        /**
         * Reserved
         * 
         * Offset: 0x90
         */
        volatile uint32_t reserved6[4U];

        /**
         * Peripheral Power-Down Clear Register 0 (PSPWRDWNCLR0)
         * 
         * Peripheral select quadrant clock power-down clear
         * 
         * Offset: 0xa0
         */
        volatile uint32_t PSPWRDWNCLR0;

        /**
         * Peripheral Power-Down Clear Register 1 (PSPWRDWNCLR1)
         * 
         * Peripheral select quadrant clock power-down clear
         * 
         * Offset: 0xa4
         */
        volatile uint32_t PSPWRDWNCLR1;

        /**
         * Peripheral Power-Down Clear Register 2 (PSPWRDWNCLR2)
         * 
         * Peripheral select quadrant clock power-down clear
         * 
         * Offset: 0xa8
         */
        volatile uint32_t PSPWRDWNCLR2;

        /**
         * Peripheral Power-Down Clear Register 3 (PSPWRDWNCLR3)
         * 
         * Peripheral select quadrant clock power-down clear
         * 
         * Offset: 0xac
         */
        volatile uint32_t PSPWRDWNCLR3;

        /**
         * Reserved
         * 
         * Offset: 0xb0
         */
        volatile uint32_t reserved7[4U];

        /**
         * Debug Frame Powerdown Set Register (PDPWRDWNSET)
         * 
         * sets the debug frame clock
         * 
         * Offset: 0xc0
         */
        volatile uint32_t PDPWRDWNSET;

        /**
         * Debug Frame Powerdown Clear Register (PDPWRDWNCLR)
         * 
         * clears the debug frame clock
         * 
         * Offset: 0xc4
         */
        volatile uint32_t PDPWRDWNCLR;

        /**
         * Reserved
         * 
         * Offset: 0xc8
         */
        volatile uint32_t reserved8[78U];

        /**
         * MasterID Protection Write Enable Register (MSTIDWRENA)
         * 
         * This key must be programmed with "1010" to unlock writes to all Master-ID registers
         * This is a 4-bit key for enabling writes to all Master-ID registers from address offset 0x300-0x5DC
         * 
         * Offset: 0x200
         */
        volatile uint32_t masterIdProtWriteEnable;

        /**
         * MasterID Enable Register (MSTIDENA)
         * 
         * MasterID Check Enable. This is a 4-bit key for enabling Master-ID check. This key must be 
         * programmed with "1010" to enable Master-ID Check functionality.
         * 
         * Offset: 0x204
         */
        volatile uint32_t masterIdEnable;

        /**
         * MasterID Diagnostic Control Register (MSTIDDIAGCTRL)
         * 
         * Diagnostic Compare Value. The value stored in this register is compared against the programmed master-id 
         * register bits for all accesses
         * 
         * Offset: 0x208
         */
        volatile uint32_t masterIdDiagControl;

        /**
         * Reserved
         * 
         * Offset: 0x20c
         */
        volatile uint32_t reserved9[61U];


        /**
         * structure containing 64 registers (32 high + 32 low) for the MasterID protection (PSnMSTID_L/H)
         * 
         * Can only access these registers once the MSTIDWRENA register has been set with the correct key
         * 
         * 
         * Offset: 0x300
         */
        struct
        {
            /**
             *  Peripheral Frame n MasterID Protection Register_L (PSnMSTID_L)
             * 
             *  Controls access for the MasterID to access the peripheral mapped in these quadrants (0 and 1)
             */
            volatile uint32_t psLow;

            /**
             *  Peripheral Frame n MasterID Protection Register_L (PSnMSTID_H)
             * 
             *  Controls access for the MasterID to access the peripheral mapped in these quadrants (2 and 3)
             */
            volatile uint32_t psHigh;
        } periFrameMasterIdProt[32];


        /**
         * structure contains 8 registers (4 high + 4 low) for the MasterID Protection for privileged peripheral 
         * frames (PPSnMSTID_L/H)
         * 
         * Can only access these registers once the MSTIDWRENA register has been set with the correct key
         * 
         * Offset: 0x400
         */
        struct
        {
            /**
             *  Privileged Peripheral Frame n MasterID Protection Register_L (PPS0MSTID_L)
             * 
             *  MasterID filtering for quadrants 0 and 1
             */
            volatile uint32_t ppsLow;

            /**
             *  Privileged Peripheral Frame n MasterID Protection Register_L (PPS0MSTID_H)
             * 
             *  MasterID filtering for quadrants 2 and 3
             */
            volatile uint32_t ppsHigh;
        } privPeriMasterIdProt[8];


        /**
         * structure contains 64 registers (32 high + 32 low) for the MasterID filtering (PPSE[1-31]MSTID_L/H)
         * 
         * Can only access these registers once the MSTIDWRENA register has been set with the correct key
         * 
         * Offset: 0x440
         */
        struct
        {
            /**
             * Privileged Peripheral Extended Frame n MasterID Protection Register_L (PPSE[1-31]MSTID_L)
             * 
             * MasterID filtering for Quadrant 0 and 1
             */
            volatile uint32_t ppseLow;

            /**
             * Privileged Peripheral Extended Frame n MasterID Protection Register_H (PPSE[1-31]MSTID_H)
             * 
             * MasterID filtering for Quadrant 2 and 3
             */
            volatile uint32_t ppseHigh;
        } privPeriExtMasterIdProt[32];


        /**
         * Peripheral Memory Frame MasterID Protection Register (PCS[0-31]MSTID)
         * 
         * MasterID filtering for PCS
         * Can only access these registers once the MSTIDWRENA register has been set with the correct key
         * 
         * Offset: 0x540
         */
        volatile uint32_t periMemFrameMasterIdProt[32];


        /**
         * Privileged Peripheral Memory Frame MasterID Protection Register (PPCS[0-7]MSTID)
         * 
         * Can only access these registers once the MSTIDWRENA register has been set with the correct key
         * 
         * Offset: 0x5c0
         */
        volatile uint32_t privPeriMemFramMasterIdProt[8];
    };


    /**
     * restore the previous pack definition
     */
#pragma pack(pop)

    /**
     * Initialize the peripherals
     */
    void pharosCpuPeripheralsInitialize(void);


#ifdef	__cplusplus
}
#endif

#endif	/* PERIPHERAL_H */
