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


#ifndef PHAROS_PLL_H
#define	PHAROS_PLL_H

#ifdef	__cplusplus
extern "C"
{
#endif

#include <pharos/hal/cpu/declarations.h>


    /**
     * TMS570LC53x System primary control registers
     * 
     * Information retrieved from "TMS570LC43x 16/32-Bit RISC Flash Microcontroller, Technical Reference Manual, 
     * SPNU563, May 2014"
     */
    typedef enum
    {
        /**
         * oscillator clock source                
         */
        PHAROS_CPU_SYSTEM_CLOCK_OSC = 0x0U ,

        /**
         * PLL 1 clock source                
         */
        PHAROS_CPU_SYSTEM_CLOCK_PLL1 = 0x1U ,

        /**
         * external clock source                
         */
        PHAROS_CPU_SYSTEM_CLOCK_EXTERNAL1 = 0x3U ,

        /**
         * low power oscillator LPO low clock Source                
         */
        PHAROS_CPU_SYSTEM_CLOCK_LPO_LOW = 0x4U ,

        /**
         * low power oscillator LPO high clock Source              
         */
        PHAROS_CPU_SYSTEM_CLOCK_LPO_HIGH = 0x5U ,

        /**
         *  PLL 2 clock Source                
         */
        PHAROS_CPU_SYSTEM_CLOCK_PLL2 = 0x6U ,

        /**
         * external 2 clock Source                
         */
        PHAROS_CPU_SYSTEM_CLOCK_EXTERNAL2 = 0x7U ,

        /**
         * synchronous VCLK1 clock Source               
         */
        PHAROS_CPU_SYSTEM_CLOCK_VCLK = 0x9U ,

        /**
         * PLL2 ODCLK / 8                
         */
        PHAROS_CPU_SYSTEM_CLOCK_PLL2_ODCLK_8 = 0xEU ,

        /**
         * PLL2 ODCLK / 16               
         */
        PHAROS_CPU_SYSTEM_CLOCK_PLL2_ODCLK_16 = 0xFU
    } PharosCpuSystemClockSource;


    /**
     * Initialize the PLL 
     */
    void pharosCpuPllInitialize(void);

    /**
     * Initialize the LPO (low power oscillator)
     */
    void pharosCpuLpoInitialize(void);

    /**
     * Initialize the CPU to use the PLL
     */
    void pharosCpuPllClockInitialize(void);

#ifdef	__cplusplus
}
#endif

#endif	/* PLL_H */
