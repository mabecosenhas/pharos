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


#ifndef PHAROS_CPU_FLASH_H
#define	PHAROS_CPU_FLASH_H

#ifdef	__cplusplus
extern "C"
{
#endif

#include <pharos/hal/cpu/declarations.h>
#include <pharos/hal/cpu/conf.h>

#define PHAROS_CPU_L2FMC     ((ptrPharosCpuL2fmc) PHAROS_CPU_L2FMC_ADDRESS)

    /************************************************************
     * FRDCNTL register definitions                             *
     ************************************************************/
#define PHAROS_CPU_L2FMC_FRDCNTL_PFUENA                 (1U << 0U)
#define PHAROS_CPU_L2FMC_FRDCNTL_PFUENB                 (1U << 1U)
#define PHAROS_CPU_L2FMC_FRDCNTL_RWAIT(x)               (x << 8U)

    /************************************************************
     * FSM_WR_ENA register definitions                          *
     ************************************************************/
#define PHAROS_CPU_L2FMC_FSM_WR_ENA_EN                  (0x5U << 0U)
#define PHAROS_CPU_L2FMC_FSM_WR_ENA_DIS                 (0x1U << 0U)

    /************************************************************
     * EEPROM_CONFIG register definitions                          *
     ************************************************************/
#define PHAROS_CPU_L2FMC_EE_CONF_EWAIT(x)               (x << 16U)

    /************************************************************
     * FBPWRMODE register definitions                          *
     ************************************************************/
#define PHAROS_CPU_L2FMC_FBPWRMODE_B7_A                 (3U << 14U)
#define PHAROS_CPU_L2FMC_FBPWRMODE_B1_A                 (3U << 2U)
#define PHAROS_CPU_L2FMC_FBPWRMODE_B0_A                 (3U << 0U)

    /**
     * Compact the size of the I/O registers
     */
#pragma pack(push ,4)


    struct PharosCpuL2Fmc
    {
        /**
         * Flash Read Control Register (FRDCNTL)
         * 
         * This register controls flash timings for the main flash banks
         * 
         * Offset: 0x0
         */
        volatile uint32_t flashReadControl;

        /**
         * Reserved
         * 
         * Offset: 0x4
         */
        volatile uint32_t reserved1;


        /**
         * EEPROM Error Correction Control Register (EE_FEDACCTRL1)
         * 
         * When a EEPROM bank is erased or zeroed out, the contents will be all 1's or all 0's respectively
         * 
         * Offset: 0x8
         */
        volatile uint32_t eepromErrorCorrectionControl;


        /**
         * Reserved
         * 
         * Offset: 0xc
         */
        volatile uint32_t reserved2;

        /**
         * Reserved
         * 
         * Offset: 0x10
         */
        volatile uint32_t reserved3;

        /**
         * Flash PortA Error and Status Register (FEDAC_PASTATUS)
         * 
         * This register applies to accesses made to the main or EEPROM flash banks through PortA.
         * 
         * Offset: 0x14
         */
        volatile uint32_t flashPortAErrorStatus;

        /**
         * Flash PortB Error and Status Register (FEDAC_PBSTATUS)
         * 
         * This register applies to accesses made to the main or EEPROM flash banks through PortB.
         * 
         * Offset: 0x18
         */
        volatile uint32_t flashPortBErrorStatus;

        /**
         * Flash Global Error and Status Register (FEDAC_GBLSTATUS)
         * 
         * Global error and status flags in L2FMC.
         * 
         * Offset: 0x1c
         */
        volatile uint32_t flashGlobalErrorStatus;

        /**
         * Reserved
         * 
         * Offset: 0x20
         */
        volatile uint32_t reserved4;

        /**
         * Flash Error Detection and Correction Sector Disable Register (FEDACSDIS)
         * 
         * This register is used to disable the SECDED function for one or two sectors from the EEPROM Emulation 
         * flash (bank 7)
         * 
         * Offset: 0x24
         */
        volatile uint32_t flashErrorDetCorrSectDis;

        /**
         * Primary Address Tag Register (FPRIM_ADD_TAG)
         * 
         * This register is used to test the prefetch address tag registers
         * 
         * Offset: 0x28
         */
        volatile uint32_t primaryAddressTag;

        /**
         * Duplicate Address Tag Register (FDUP_ADD_TAG)
         * 
         * This register is used to test the prefetch address tag registers
         * 
         * Offset: 0x2c
         */
        volatile uint32_t duplicateAddressTag;

        /**
         * Flash Bank Protection Register (FBPROT)
         * 
         * Setting this bit disables protection from writing to the OTPPROTDIS bits in the FBAC 
         * register as well as the BSE bits for all banks in the FBSE register. Clearing this bit enables
         * protection and disables write access to the OTPPROTDIS bits and FBSE register.
         * 
         * Offset: 0x30
         */
        volatile uint32_t flashBankProtection;

        /**
         * Flash Bank Sector Enable Register (FBSE)
         * 
         * FBSE provides one enable bit per sector for up to 16 sectors per bank. Each bank in the Flash module
         * has one FBSE register. The bank is selected via the BANK bits in the FMAC register. As only one bank at
         * a time can be selected by FMAC, only the register for the bank selected appears at this address         
         * 
         * Offset: 0x34
         */
        volatile uint32_t flashBankSectorEanble;

        /**
         * Flash Bank Busy Register (FBBUSY)
         * 
         * Each bit corresponds to a Flash bank.
         * 
         * Offset: 0x38
         */
        volatile uint32_t flashBankBusy;

        /**
         * Flash Bank Access Control Register (FBAC)
         * 
         * Controls the access to the flash bank
         * 
         * Offset: 0x3c
         */
        volatile uint32_t flashBankAccessControl;

        /**
         * Flash Bank Power Mode Register (FBPWRMODE)
         * 
         * Sets the power mode for each flash bank
         * 
         * Offset: 0x40
         */
        volatile uint32_t flashBankPowerMode;

        /**
         * Flash Bank/Pump Ready Register (FBPRDY)
         * 
         * FBPRDY register allows the user to determine if the pump and banks are ready for performing read access.
         * 
         * Offset: 0x44
         */
        volatile uint32_t flashBankPumpReady;

        /**
         * Flash Pump Access Control Register 1 (FPAC1)
         * 
         * Flash Charge Pump Fallback Power Mode
         * 
         * Offset: 0x48
         */
        volatile uint32_t flashPumpAccessControl;

        /**
         * Reserved
         * 
         * Offset: 0x4c
         */
        volatile uint32_t reserved5;

        /**
         * Flash Module Access Control Register (FMAC)
         * 
         * Bank Enable
         * 
         * Offset: 0x50
         */
        volatile uint32_t flashModuleAccessControl;

        /**
         * Flash Module Status Register (FMSTAT)
         * 
         * Status of the flash errors
         * 
         * Offset: 0x54
         */
        volatile uint32_t flashModuleStatus;

        /**
         * EEPROM Emulation Data MSW Register (FEMU_DMSW)
         * 
         * This register is used in diagnostic mode 7 to XOR the upper 32b of the data being delivered to the bus 
         * master.
         * 
         * Offset: 0x58
         */
        volatile uint32_t eepromEmulationDataMsw;

        /**
         * EEPROM Emulation Data LSW Register (FEMU_DLSW)
         * 
         * This register is used in diagnostic mode 7 to XOR the lower 32b of the data being delivered to the bus
         * master.
         * 
         * Offset: 0x5c
         */
        volatile uint32_t eepromEmulationDataLsw;

        /**
         * EEPROM Emulation ECC Register (FEMU_ECC)
         * 
         * This register is used in diagnostic mode 7 to XOR the ECC being delivered to the bus master.
         * 
         * Offset: 0x60
         */
        volatile uint32_t eepromEmulationEcc;

        /**
         * Flash Lock Register (FLOCK)
         * 
         * Enable writes to EE_FEDACCTRL1 register
         * 
         * Offset: 0x64
         */
        volatile uint32_t flashLock;

        /**
         * Reserved
         * 
         * Offset: 0x68
         */
        volatile uint32_t reserved6;

        /**
         * Diagnostic Control Register (FDIAGCTRL)
         * 
         * First set the DIAGMODE and the DIAG_EN_KEY bits before setting up the other registers to block the
         * other registers from causing a false error. The final write should set the DIAG_TRIG bit to activate 
         * the test. Running out of RAM will prevent problems with the diagnostic test corrupting the flash access 
         * in some of the modes
         * 
         * Offset: 0x6c
         */
        volatile uint32_t diagnosticControl;

        /**
         * Reserved
         * 
         * Offset: 0x70
         */
        volatile uint32_t reserved7;

        /**
         * Raw Address Register (FRAW_ADDR)
         * 
         * This register is used during the address tag diagnostic test, DIAG_MODE = 5, to replace the address bus
         * bits 31:3. Lower 5 bits are not compared during the diagnostic.
         * 
         * Offset: 0x74
         */
        volatile uint32_t rawAddress;

        /**
         * Reserved
         * 
         * Offset: 0x78
         */
        volatile uint32_t reserved8;

        /**
         * Parity Override Register (FPAR_OVR)
         * 
         * This register allows overriding the parity that is internally computed by the L2FMC for checking the parity
         * circuit.
         * 
         * Offset: 0x7c
         */
        volatile uint32_t parityOverride;

        /**
         * Reserved
         * 
         * Offset: 0x80
         */
        volatile uint32_t reserved9[13U];

        /**
         * Reset Configuration Valid Register (RCR_VALID)
         * 
         * This register reflects the validity of the implicit read.
         * 
         * Offset: 0xb4
         */
        volatile uint32_t resetConfValid;

        /**
         * Crossbar Access Time Threshold Register (ACC_THRESHOLD)
         * 
         * Configures maximum number of clocks beyond which the L2FMC internal switch will timeout the 
         * access. This can occur due to soft error in internal logic. It is NOT recommended to modify this
         * register unless a crossbar diagnostic is being performed.
         * 
         * Offset: 0xb8
         */
        volatile uint32_t crossbarAccessTimeThreshold;

        /**
         * Reserved
         * 
         * Offset: 0xbc
         */
        volatile uint32_t reserved10;

        /**
         * Flash Error Detection and Correction Sector Disable Register 2 (FEDACSDIS2)
         * 
         * This register is used to disable the SECDED function on additional two sectors on the EEPROM 
         * Emulation flash (bank 7)
         * 
         * Offset: 0xc0
         */
        volatile uint32_t flashErrorDectCorrecSectorDis2;

        /**
         * Reserved
         * 
         * Offset: 0xc4
         */
        volatile uint32_t reserved11[3];

        /**
         * Lower Word of Reset Configuration Read Register (RCR_VALUE0)
         * 
         * When L2FMC completes the implicit read, it populates this register with the lower 32 bits of the data. This
         * is useful to perform a software diagnostic of the SECDED module
         * 
         * Offset: 0xd0
         */
        volatile uint32_t resetConfRead0;

        /**
         * Upper Word of Reset Configuration Read Register (RCR_VALUE1)
         * 
         * When L2FMC completes the implicit read, it populates this register with the upper 32 bits of the data. This
         * is useful to perform a software diagnostic of the SECDED module
         * 
         * Offset: 0xd4
         */
        volatile uint32_t resetConfRead1;

        /**
         * Reserved
         * 
         * Offset: 0xd8
         */
        volatile uint32_t reserved12[108U];

        /**
         * FSM Register Write Enable Register (FSM_WR_ENA)
         * 
         * FSM Write Enable
         * 
         * Offset: 0x288
         */
        volatile uint32_t fsmWriteEnable;

        /**
         * Reserved
         * 
         * Offset: 0x2b4
         */
        volatile uint32_t reserved13[11U];

        /**
         * EEPROM Emulation Configuration Register (EEPROM_CONFIG)
         * 
         * EEPROM Wait state Counter
         * 
         * Offset: 0x2b8
         */
        volatile uint32_t eepromEmulationConf;


        /**
         * Reserved
         * 
         * Offset: 0x2bc
         */
        volatile uint32_t reserved14;

        /**
         * FSM Sector Register 1 (FSM_SECTOR1)
         * 
         * This is a banked register. A separate register is implemented for each bank, but they all occupy the same
         * address. The correct bank must be selected in the FMAC register before reading or writing this register
         * 
         * Offset: 0x2c0
         */
        volatile uint32_t fsmSector1;


        /**
         * FSM Sector Register 2 (FSM_SECTOR2)
         * 
         * This register is applicable to EEPROM bank having more than 32 sectors only. Refer device datasheet to
         * find the number of EEPROM sectors in a particular device
         * 
         * Offset: 0x2c4
         */
        volatile uint32_t fsmSector2;


        /**
         * Reserved
         * 
         * Offset: 0x2a8
         */
        volatile uint32_t reserved15[78U];


        /**
         * Flash Bank Configuration Register (FCFG_BANK)
         * 
         * Width of flash banks
         * 
         * Offset: 0x2b8
         */
        volatile uint32_t flashBankConf;
    };


    /**
     * restore the previous pack definition
     */
#pragma pack(pop)

    /**
     * Initialize the flash
     */
    void pharosCpuFlashInitialize(void);

#ifdef	__cplusplus
}
#endif

#endif	/* FLASH_H */
