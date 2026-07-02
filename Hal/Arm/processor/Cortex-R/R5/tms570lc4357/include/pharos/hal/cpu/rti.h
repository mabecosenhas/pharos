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


#ifndef PHAROS_RTI_H
#define	PHAROS_RTI_H

#ifdef	__cplusplus
extern "C"
{
#endif

#include <pharos/hal/cpu/declarations.h>

    /**
     * Pointer to the RTI 
     */
#define PHAROS_CPU_RTI                                  ((ptrPharosCpuRti) PHAROS_CPU_RTI_ADDRESS)

    /************************************************************
     * RTIGCTRL register definitions                            *
     ************************************************************/
#define PHAROS_CPU_RTI_GCTRL_EN(x)                      (1U << x)
#define PHAROS_CPU_RTI_GCTRL_DIS(x)                     (0U << x)

    /**
     * Continue both RTI0 and RTI1 on halting debug mode
     */
#define PHAROS_CPU_RTI_GCTRL_COS_EN                     (1U << 15U)

    /**
     * Halt both RTI0 and RTI1 on halting debug mode
     */
#define PHAROS_CPU_RTI_GCTRL_COS_DIS                    (0U << 15U)

#define PHAROS_CPU_RTI_GCTRL_PLL2                       (0xa << 16U)

    /************************************************************
     * RTITBCTRL register definitions                           *
     ************************************************************/
#define PHAROS_CPU_RTI_TBCTRL_RTIUC0                    (0U << 0U)

#define PHAROS_CPU_RTI_TBCTRL_NOINC                     (0U << 1U)
#define PHAROS_CPU_RTI_TBCTRL_INC                       (1U << 1U)

    /************************************************************
     * RTICOMPCTRL register definitions                         *
     ************************************************************/
#define PHAROS_CPU_RTI_COMPCTRL_CMP0C0                  (0U << 0U)
#define PHAROS_CPU_RTI_COMPCTRL_CMP0C1                  (1U << 0U)

#define PHAROS_CPU_RTI_COMPCTRL_CMP1C0                  (0U << 4U)
#define PHAROS_CPU_RTI_COMPCTRL_CMP1C1                  (1U << 4U)

#define PHAROS_CPU_RTI_COMPCTRL_CMP2C0                  (0U << 8U)
#define PHAROS_CPU_RTI_COMPCTRL_CMP2C1                  (1U << 8U)

#define PHAROS_CPU_RTI_COMPCTRL_CMP3C0                  (0U << 12U)
#define PHAROS_CPU_RTI_COMPCTRL_CMP3C1                  (1U << 12U)

    /************************************************************
     * RTIINTFLAG register definitions                          *
     ************************************************************/
#define PHAROS_CPU_RTI_INTFLAG_CLEAR(x)                 (1U << x)
#define PHAROS_CPU_RTI_INTFLAG_IS_PEND(x)               (1U << x)

    /************************************************************
     * RTISETINTENA register definitions                      *
     ************************************************************/
#define PHAROS_CPU_RTI_SETINTENA_INT(x)                 (1U << x)

    /************************************************************
     * RTICLEARINTENA register definitions                      *
     ************************************************************/
#define PHAROS_CPU_RTI_CLEARINTENA_INT(x)               (1U << x)
#define PHAROS_CPU_RTI_CLEARINTENA_TB                   (1U << 16U)
#define PHAROS_CPU_RTI_CLEARINTENA_OVL(x)               (1U << (17U + x))

    /************************************************************
     * RTIINTFLAG register definitions                          *
     ************************************************************/
#define PHAROS_CPU_RTI_INTFLAG_INT_CLR(x)               (1U << x)
#define PHAROS_CPU_RTI_INTFLAG_TB                       (1U << 16U)
#define PHAROS_CPU_RTI_INTFLAG_OVL(x)                   (1U << (17U + x))



    /**
     * RTI Clear Interrupt Enable Register ()
     * 
     * Disables a specific interrupt
     * 
     * Offset: 0x84
     */
    volatile uint32_t interruptClear;

    /**
     * Compact the size of the I/O registers
     */
#pragma pack(push ,4)


    struct PharosCpuRti
    {
        /**
         * RTI Global Control Register (RTIGCTRL)
         * 
         * The global control register starts/stops the counters and selects the signal compared with the timebase
         * control circuit
         * 
         * Offset: 0x0
         */
        volatile uint32_t globalControl;

        /**
         * RTI Timebase Control Register (RTITBCTRL)
         * 
         * The timebase control register selects if the free running counter 0 is incremented by RTICLK or NTU
         * 
         * Offset: 0x4
         */
        volatile uint32_t timebaseControl;

        /**
         * RTI Capture Control Register (RTICAPCTRL)
         * 
         * The capture control register controls the capture source for the counters
         * 
         * Offset: 0x8
         */
        volatile uint32_t captureControl;

        /**
         * RTI Compare Control Register (RTICOMPCTRL)
         * 
         * The compare control register controls the source for the compare registers
         * 
         * Offset: 0xc
         */
        volatile uint32_t compareControl;

        /**
         * RTI Free Running Counter 0 Register (RTIFRC0)
         * 
         * The free running counter 0 register holds the current value of free running counter 0
         * 
         * Offset: 0x10
         */
        volatile uint32_t freeRunningCounter0;

        /**
         * RTI Up Counter 0 Register (RTIUC0)
         * 
         * The up counter 0 register holds the current value of prescale counter
         * 
         * Offset: 0x14
         */
        volatile uint32_t upCounter0;

        /**
         * RTI Compare Up Counter 0 Register (RTICPUC0)
         * 
         * The compare up counter 0 register holds the value to be compared with prescale counter 0 (RTIUC0)
         * 
         * Offset: 0x18
         */
        volatile uint32_t compareUpCounter0;

        /**
         * Reserved
         * 
         * Offset: 0x1c
         */
        volatile uint32_t reserved1;


        /**
         * RTI Capture Free Running Counter 0 Register (RTICAFRC0)
         * 
         * The capture free running counter 0 register holds the free running counter 0 on external events
         * 
         * Offset: 0x20
         */
        volatile uint32_t captureFreeRunningCounter0;

        /**
         * RTI Capture Up Counter 0 Register (RTICAUC0)
         * 
         * The capture up counter 0 register holds the current value of prescale counter 0 on external events
         * 
         * Offset: 0x24
         */
        volatile uint32_t captureUpCounter0;

        /**
         * Reserved
         * 
         * Offset: 0x28
         */
        volatile uint32_t reserved2;

        /**
         * Reserved
         * 
         * Offset: 0x2c
         */
        volatile uint32_t reserved3;

        /**
         * RTI Free Running Counter 1 Register (RTIFRC1)
         * 
         * Holds the current value of the free running counter 1
         * 
         * Offset: 0x30
         */
        volatile uint32_t freeRunningCounter1;

        /**
         * RTI Up Counter 1 Register (RTIUC1)
         * 
         * The up counter 1 register holds the current value of the prescale counter 1
         * 
         * Offset: 0x34
         */
        volatile uint32_t upCounter1;

        /**
         * RTI Compare Up Counter 1 Register (RTICPUC1)
         * 
         * The compare up counter 1 register holds the value compared with prescale counter 1.
         * 
         * Offset: 0x38
         */
        volatile uint32_t compareUpCounter1;

        /**
         * Reserved
         * 
         * Offset: 0x3c
         */
        volatile uint32_t reserved4;

        /**
         * RTI Capture Free Running Counter 1 Register (RTICAFRC1)
         * 
         * The capture free running counter 1 register holds the current value of free running counter 1 on external 
         * events
         * 
         * Offset: 0x40
         */
        volatile uint32_t captureFreeRunningCounter1;

        /**
         * RTI Capture Up Counter 1 Register (RTICAUC1)
         * 
         * The capture up counter 1 register holds the current value of prescale counter 1 on external events
         * 
         * Offset: 0x44
         */
        volatile uint32_t captureUpCounter1;

        /**
         * Reserved
         * 
         * Offset: 0x48
         */
        volatile uint32_t reserved5;

        /**
         * Reserved
         * 
         * Offset: 0x4c
         */
        volatile uint32_t reserved6;

        /**
         * RTI Compare 0 Register (RTICOMP0)
         * 
         * The compare 0 register holds the value to be compared with the counters
         * 
         * Offset: 0x50
         */
        volatile uint32_t compare0;

        /**
         * RTI Update Compare 0 Register (RTIUDCP0)
         * 
         * The update compare 0 register holds the value to be added to the compare register 0 value on a compare 
         * match
         * 
         * Offset: 0x54
         */
        volatile uint32_t updateCompare0;

        /**
         * RTI Compare 1 Register (RTICOMP1)
         * 
         * Holds the value to be compared to the counters
         * 
         * Offset: 0x58
         */
        volatile uint32_t compare1;

        /**
         * RTI Update Compare 1 Register (RTIUDCP1)
         * 
         * Holds the value to be added to the compare register 1 value on a compare match
         * 
         * Offset: 0x5c
         */
        volatile uint32_t updateCompare1;

        /**
         * RTI Compare 2 Register (RTICOMP2)
         * 
         * Holds the value to be compared to the counters
         * 
         * Offset: 0x60
         */
        volatile uint32_t compare2;

        /**
         * RTI Update Compare 2 Register (RTIUDCP2)
         * 
         * Holds the value to be added to the compare register 2 value on a compare match
         * 
         * Offset: 0x64
         */
        volatile uint32_t updateCompare2;

        /**
         * RTI Compare 3 Register (RTICOMP3)
         * 
         * Holds the value to be compared to the counters.
         * 
         * Offset: 0x68
         */
        volatile uint32_t compare3;

        /**
         * RTI Update Compare 3 Register (RTIUDCP3)
         * 
         * Holds the value to be added to the compare register 3 value on a compare match
         * 
         * Offset: 0x6c
         */
        volatile uint32_t updateCompare3;

        /**
         * RTI Timebase Low Compare Register (RTITBLCOMP)
         * 
         * Holds the value to activate the edge detection circuit
         * 
         * Offset: 0x70
         */
        volatile uint32_t timebaseLowCompare;

        /**
         * RTI Timebase High Compare Register (RTITBHCOMP)
         * 
         * Holds the value to deactivate the edge detection circuit
         * 
         * Offset: 0x74
         */
        volatile uint32_t timebaseHighCompare;

        /**
         * Reserved
         * 
         * Offset: 0x78
         */
        volatile uint32_t reserved7;

        /**
         * Reserved
         * 
         * Offset: 0x7c
         */
        volatile uint32_t reserved8;

        /**
         * RTI Set Interrupt Enable Register (RTISETINTENA)
         * 
         * Enable a specific interrupt
         * 
         * Offset: 0x80
         */
        volatile uint32_t interruptSet;

        /**
         * RTI Clear Interrupt Enable Register (RTICLEARINTENA)
         * 
         * Disables a specific interrupt
         * 
         * Offset: 0x84
         */
        volatile uint32_t interruptClear;

        /**
         * RTI Interrupt Flag Register (RTIINTFLAG)
         * 
         * The corresponding flags are set at every compare match of the RTIFRCx and RTICOMPx values, whether
         * the interrupt is enabled or not
         * 
         * Offset: 0x88
         */
        volatile uint32_t interruptFlag;

        /**
         * Reserved
         * 
         * Offset: 0x8c
         */
        volatile uint32_t reserved9;

        /**
         * Digital Watchdog Control Register (RTIDWDCTRL)
         * 
         * The software has to write to the DWDCTRL field in order to enable the DWD
         * 
         * Offset: 0x90
         */
        volatile uint32_t digitalWdControl;

        /**
         * Digital Watchdog Preload Register (RTIDWDPRLD)
         * 
         * sets the expiration time of the DWD
         * 
         * Offset: 0x94
         */
        volatile uint32_t digitalWdPreload;

        /**
         * Watchdog Status Register (RTIWDSTATUS)
         * 
         * records the status of the DWD
         * 
         * Offset: 0x98
         */
        volatile uint32_t wdStatus;

        /**
         * RTI Watchdog Key Register (RTIWDKEY)
         * 
         * This register must be written with the correct written key values to serve the watchdog
         * 
         * Offset: 0x9c
         */
        volatile uint32_t wdKey;

        /**
         * RTI Digital Watchdog Down Counter (RTIDWDCNTR)
         * 
         * Provides the current value of the DWD down counter.
         * 
         * Offset: 0xa0
         */
        volatile uint32_t digitalWdDownCounter;

        /**
         * Digital Windowed Watchdog Reaction Control (RTIWWDRXNCTRL)
         * 
         * Selects the DWWD reaction if the watchdog is serviced outside the time window
         * 
         * Offset: 0xa4
         */
        volatile uint32_t digitalWindowedWdReactionCtrl;

        /**
         * Digital Windowed Watchdog Window Size Control (RTIWWDSIZECTRL)
         * 
         * Selects the DWWD window size
         * 
         * Offset: 0xa8
         */
        volatile uint32_t digitalWindowedWdWindowSize;

        /**
         * RTI Compare Interrupt Clear Enable Register (RTIINTCLRENABLE)
         * 
         * When the RTI compare event is configured to generate a DMA request or triggers (all triggered by RTI
         * compare interrupt request flag) to other peripherals, it is often desirable to clear the RTI compare flag
         * automatically so that the requests can be generated repeatedly without any CPU intervention
         * 
         * Offset: 0xac
         */
        volatile uint32_t compareInterruptClear;

        /**
         * RTI Compare 0 Clear Register (RTICMP0CLR)
         * 
         * This registers holds an initial value which is larger than the value in the RTI Compare 0 register
         * 
         * Offset: 0xb0
         */
        volatile uint32_t compare0Clear;

        /**
         * RTI Compare 1 Clear Register (RTICMP1CLR)
         * 
         * Holds an initial value which is larger than the value in the RTI Compare 1 register
         * 
         * Offset: 0xb4
         */
        volatile uint32_t compare1Clear;

        /**
         * RTI Compare 2 Clear Register (RTICMP2CLR)
         * 
         * Holds an initial value which is larger than the value in the RTI Compare 2 register
         * 
         * Offset: 0xb8
         */
        volatile uint32_t compare2Clear;

        /**
         * RTI Compare 3 Clear Register (RTICMP3CLR)
         * 
         * Holds an initial value which is larger than the value in the RTI Compare 3 register
         * 
         * Offset: 0xbc
         */
        volatile uint32_t compare3Clear;

    };


    /**
     * restore the previous pack definition
     */
#pragma pack(pop)

    
#ifdef	__cplusplus
}
#endif

#endif	/* RTI_H */
