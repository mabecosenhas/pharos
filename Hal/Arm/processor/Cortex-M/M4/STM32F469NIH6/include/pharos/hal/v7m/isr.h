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


#ifndef PHAROS_ISR_H
#define	PHAROS_ISR_H


/**
 * Compact the size of the I/O registers
 */
#pragma pack(push ,4)


struct PharosCpuIsrTable
{
    PharosRawInterruptHandler watchdog;
    PharosRawInterruptHandler pvd;
    PharosRawInterruptHandler tamperStamp;
    PharosRawInterruptHandler rtcWakeup;
    PharosRawInterruptHandler flash;
    PharosRawInterruptHandler rcc;
    PharosRawInterruptHandler extLine0;
    PharosRawInterruptHandler extLine1;
    PharosRawInterruptHandler extLine2;
    PharosRawInterruptHandler extLine3;
    PharosRawInterruptHandler extLine4;
    PharosRawInterruptHandler dma1Stream0;
    PharosRawInterruptHandler dma1Stream1;
    PharosRawInterruptHandler dma1Stream2;
    PharosRawInterruptHandler dma1Stream3;
    PharosRawInterruptHandler dma1Stream4;
    PharosRawInterruptHandler dma1Stream5;
    PharosRawInterruptHandler dma1Stream6;
    PharosRawInterruptHandler adc;
    PharosRawInterruptHandler can1Tx;
    PharosRawInterruptHandler can1Rx0;
    PharosRawInterruptHandler can1Rx1;
    PharosRawInterruptHandler can1Sce;
    PharosRawInterruptHandler extLine9_5;
    PharosRawInterruptHandler tim1Breakpoint_Timer9;
    PharosRawInterruptHandler timer1Update_Timer10;
    PharosRawInterruptHandler timer1TriggerCommutation_Timer11;
    PharosRawInterruptHandler timer1CaptureCompare;
    PharosRawInterruptHandler timer2;
    PharosRawInterruptHandler timer3;
    PharosRawInterruptHandler timer4;
    PharosRawInterruptHandler i2c1Event;
    PharosRawInterruptHandler i2c1Error;
    PharosRawInterruptHandler i2c2Event;
    PharosRawInterruptHandler i2c2Error;
    PharosRawInterruptHandler spi1;
    PharosRawInterruptHandler spi2;
    PharosRawInterruptHandler usart1;
    PharosRawInterruptHandler usart2;
    PharosRawInterruptHandler usart3;
    PharosRawInterruptHandler extLine15_10;
    PharosRawInterruptHandler rtcAlarm;
    PharosRawInterruptHandler usbOtgFsWakeup;
    PharosRawInterruptHandler timer8Break_Timer12;
    PharosRawInterruptHandler timer8Update_Timer13;
    PharosRawInterruptHandler timer8TriggerCommutation_Timer14;
    PharosRawInterruptHandler timer8CaptureCompare;
    PharosRawInterruptHandler dma1Stream7;
    PharosRawInterruptHandler fmc;
    PharosRawInterruptHandler sdio;
    PharosRawInterruptHandler timer5;
    PharosRawInterruptHandler spi3;
    PharosRawInterruptHandler uart4;
    PharosRawInterruptHandler uart5;
    PharosRawInterruptHandler timer6_dac1Error_dac2Error;
    PharosRawInterruptHandler timer7;
    PharosRawInterruptHandler dma2Stream0;
    PharosRawInterruptHandler dma2Stream1;
    PharosRawInterruptHandler dma2Stream2;
    PharosRawInterruptHandler dma2Stream3;
    PharosRawInterruptHandler dma2Stream4;
    PharosRawInterruptHandler ethernet;
    PharosRawInterruptHandler ethernetWakeup;
    PharosRawInterruptHandler can2Tx;
    PharosRawInterruptHandler can2Rx0;
    PharosRawInterruptHandler can2Rx1;
    PharosRawInterruptHandler can2Sce;
    PharosRawInterruptHandler usbOtgFs;
    PharosRawInterruptHandler dma2Stream5;
    PharosRawInterruptHandler dma2Stream6;
    PharosRawInterruptHandler dma2Stream7;
    PharosRawInterruptHandler usart6;
    PharosRawInterruptHandler i2c3Event;
    PharosRawInterruptHandler i2c3Error;
    PharosRawInterruptHandler usbOtgHsEndPoint1Out;
    PharosRawInterruptHandler usbOtgHsEndPoint1In;
    PharosRawInterruptHandler usbOtgHsWakeup;
    PharosRawInterruptHandler usbOtgHs;
    PharosRawInterruptHandler dcmi;
    PharosRawInterruptHandler cryp;
    PharosRawInterruptHandler hashRng;
    PharosRawInterruptHandler fpu;
    PharosRawInterruptHandler uart7;
    PharosRawInterruptHandler uart8;
    PharosRawInterruptHandler spi4;
    PharosRawInterruptHandler spi5;
    PharosRawInterruptHandler spi6;
    PharosRawInterruptHandler sai1;
    PharosRawInterruptHandler lcdTft;
    PharosRawInterruptHandler lcdTftError;
    PharosRawInterruptHandler dma2d;
    PharosRawInterruptHandler quadspi;
    PharosRawInterruptHandler dsi;
};


/**
 * restore the previous pack definition
 */
#pragma pack(pop)

/* PHAROS_ISR_H */
#endif	
