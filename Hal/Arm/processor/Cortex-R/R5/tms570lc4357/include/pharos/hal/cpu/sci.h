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


#ifndef PHAROS_SCI_H
#define	PHAROS_SCI_H

#ifdef	__cplusplus
extern "C"
{
#endif

#include <pharos/hal/cpu/declarations.h>

    /**
     * Compact the size of the I/O registers
     */
#pragma pack(push ,4)


    /**
     * TMS570LC53x System primary control registers
     * 
     * Information retrieved from "TMS570LC43x 16/32-Bit RISC Flash Microcontroller, Technical Reference Manual, 
     * SPNU563, May 2014"
     */
    struct PharosCpuSci
    {
        /**
         * SCI Global Control Register 0 (SCIGCR0)
         * 
         * defines the module reset
         * 
         * Offset: 0x00
         */
        volatile uint32_t globalControl0;


        /**
         * SCI Global Control Register 1 (SCIGCR1)
         * 
         * defines the frame format, protocol, and communication mode used by the SCI
         * 
         * Offset: 0x04
         */
        volatile uint32_t globalControl1;

        /**
         * SCI Global Control Register 2 (SCIGCR2)
         * 
         * used to send or compare a checksum byte during extended frames, to generate a wakeup and for low-power 
         * mode control of the LIN module
         * Note: Applicable only to LIN SCI Compatibility Mode,Reserved for standalone SCI
         * 
         * Offset: 0x08
         */
        volatile uint32_t globalControl2;

        /**
         * SCI Set Interrupt Register (SCISETINT)
         * 
         * sets interrupt enable for various types of interrupt sources
         * 
         * Offset: 0x0c
         */
        volatile uint32_t interruptEnableSet;

        /**
         * SCI Clear Interrupt Register (SCICLEARINT)
         * 
         * clears interrupt enable for various types of interrupt sources
         * 
         * Offset: 0x10
         */
        volatile uint32_t interruptClear;

        /**
         * SCI Set Interrupt Level Register (SCISETINTLVL)
         * 
         * set interrupt level
         * 
         * Offset: 0x14
         */
        volatile uint32_t interruptSetLevel;

        /**
         * SCI Clear Interrupt Level Register (SCICLEARINTLVL)
         * 
         * clears the interrupt level
         * 
         * Offset: 0x18
         */
        volatile uint32_t interruptClearLevel;

        /**
         * SCI Flags Register (SCIFLR)
         * 
         * status of the SCI errors
         * 
         * Offset: 0x1c
         */
        volatile uint32_t flags;

        /**
         * SCI Interrupt Vector Offset 0 (SCIINTVECT0)
         * 
         * interrupt vector offset for interrupt line INT0
         * 
         * Offset: 0x20
         */
        volatile uint32_t interruptVector0;

        /**
         * SCI Interrupt Vector Offset 1 (SCIINTVECT1)
         * 
         * interrupt vector offset for interrupt line INT1
         * 
         * Offset: 0x24
         */
        volatile uint32_t interruptVector1;

        /**
         * SCI Format Control Register (SCIFORMAT)
         * 
         * control the format of the frame
         * 
         * Offset: 0x28
         */
        volatile uint32_t frameFormatControl;

        /**
         * Baud Rate Selection Register (SCIBRS)
         * 
         * controls the baud rate 
         * 
         * Offset: 0x2c
         */
        volatile uint32_t BRS;

        /**
         * SCI Receiver Emulation Data Buffer (SCIED)
         * 
         * Emulator data
         * physically the same register as SCIRD
         * Available in SCI mode only
         * 
         * Offset: 0x30
         */
        volatile uint32_t receiverEmulationDataBuffer;

        /**
         * SCI Receiver Data Buffer (SCIRD)
         * 
         * Receiver data
         * physically the same register as SCIED
         * Available in SCI mode only
         * 
         * Offset: 0x34
         */
        volatile uint32_t receiveBuffer;

        /**
         * SCI Transmit Data Buffer Register (SCI)
         * 
         * Data to be transmitted is written to the SCITD register
         * Available in SCI mode only
         * 
         * Offset: 0x38
         */
        volatile uint32_t transmitBuffer;

        /**
         * SCI Pin I/O Control Register 0 (SCIPIO0)
         * 
         * controls the pins I/O function
         * 
         * Offset: 0x3c
         */
        volatile uint32_t pioControl0;

        /**
         * SCI Pin I/O Control Register 1 (SCIPIO1)
         * 
         * controls the pins I/O function
         * 
         * Offset: 0x40
         */
        volatile uint32_t pioControl1;

        /**
         * SCI Pin I/O Control Register 2 (SCIPIO2)
         * 
         * controls the pins I/O function
         * 
         * Offset: 0x44
         */
        volatile uint32_t pioControl2;

        /**
         * SCI Pin I/O Control Register 3 (SCIPIO3)
         * 
         * controls the pins I/O function
         * 
         * Offset: 0x48
         */
        volatile uint32_t pioControl3;

        /**
         * SCI Pin I/O Control Register 4 (SCIPIO4)
         * 
         * controls the pins I/O function
         * 
         * Offset: 0x4c
         */
        volatile uint32_t pioControl4;

        /**
         * SCI Pin I/O Control Register 5 (SCIPIO5)
         * 
         * controls the pins I/O function
         * 
         * Offset: 0x50
         */
        volatile uint32_t pioControl5;

        /**
         * SCI Pin I/O Control Register 6 (SCIPIO6)
         * 
         * controls the pins I/O function
         * 
         * Offset: 0x54
         */
        volatile uint32_t pioControl6;

        /**
         * SCI Pin I/O Control Register 7 (SCIPIO7)
         * 
         * controls the pins I/O function
         * 
         * Offset: 0x58
         */
        volatile uint32_t pioControl7;

        /**
         * SCI Pin I/O Control Register 8 (SCIPIO8)
         * 
         * controls the pins I/O function
         * 
         * Offset: 0x5c
         */
        volatile uint32_t pioControl8;

        /**
         * Reserved
         * 
         * Offset: 0x60
         */
        volatile uint32_t reserved[12U];

        /**
         * Input/Output Error Enable (IODFTCTRL)
         * 
         * I/O Error Enable Register
         * 
         * Offset: 0x90
         */
        volatile uint32_t ioErrorEnable;
    };


    /**
     * restore the previous pack definition
     */
#pragma pack(pop)

    /**
     * Initialize the SCI registers
     * 
     * @param SCI interface to initialize
     */
    void pharosCpuSciInitialize(ptrPharosCpuSci sci);


    /**
     * Writes an output char to the specified SCI interface
     * 
     * @param sci SCI interface to write to
     * @param c char to write
     */
    void pharosCpuSciWrite(ptrPharosCpuSci sci , char c);


#ifdef	__cplusplus
}
#endif

#endif	/* SCI_H */
