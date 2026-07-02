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


#ifndef PHAROS_V7M_ISR_H
#define	PHAROS_V7M_ISR_H


#include <pharos/hal/v7m/declarations.h>
#include <pharos/hal/cpu/declarations.h>

/**
 * Compact the size of the I/O registers
 */
#pragma pack(push ,4)


struct PharosCpuIsrTable
{
    PharosRawInterruptHandler gpioPortA;
    PharosRawInterruptHandler gpioPortB;
    PharosRawInterruptHandler gpioPortC;
    PharosRawInterruptHandler gpioPortD;
    PharosRawInterruptHandler gpioPortE;
    PharosRawInterruptHandler uart0RxTx;
    PharosRawInterruptHandler uart1RxTx;
    PharosRawInterruptHandler ssi0RxTx;
    PharosRawInterruptHandler i2c0;
    PharosRawInterruptHandler pwmFault;
    PharosRawInterruptHandler pwmGenerator0;
    PharosRawInterruptHandler pwmGenerator1;
    PharosRawInterruptHandler pwmGenerator2;
    PharosRawInterruptHandler quadratureEncoder0;
    PharosRawInterruptHandler adcSequence0;
    PharosRawInterruptHandler adcSequence1;
    PharosRawInterruptHandler adcSequence2;
    PharosRawInterruptHandler adcSequence3;
    PharosRawInterruptHandler watchdogTimer;
    PharosRawInterruptHandler timer0A;
    PharosRawInterruptHandler timer0B;
    PharosRawInterruptHandler timer1A;
    PharosRawInterruptHandler timer1B;
    PharosRawInterruptHandler timer2A;
    PharosRawInterruptHandler timer2B;
    PharosRawInterruptHandler analogComparator0;
    PharosRawInterruptHandler analogComparator1;
    PharosRawInterruptHandler analogComparator2;
    PharosRawInterruptHandler systemControl;
    PharosRawInterruptHandler flashControl;
    PharosRawInterruptHandler gioPortF;
    PharosRawInterruptHandler gioPortG;
    PharosRawInterruptHandler gioPortH;
    PharosRawInterruptHandler uart2RxTx;
    PharosRawInterruptHandler ssi1RxTx;
    PharosRawInterruptHandler timer3A;
    PharosRawInterruptHandler timer3B;
    PharosRawInterruptHandler i2c1;
    PharosRawInterruptHandler can0;
    PharosRawInterruptHandler can1;
    PharosRawInterruptHandler ethernet;
    PharosRawInterruptHandler hibernate;
    PharosRawInterruptHandler usb0;
    PharosRawInterruptHandler pwmGenerator3;
    PharosRawInterruptHandler uDmaSoftTransfer;
    PharosRawInterruptHandler uDmaError;
    PharosRawInterruptHandler adc1Seq0;
    PharosRawInterruptHandler adc1Seq1;
    PharosRawInterruptHandler adc1Seq2;
    PharosRawInterruptHandler adc1Seq3;
    PharosRawInterruptHandler externalBus0;
    PharosRawInterruptHandler gpioPortJ;
    PharosRawInterruptHandler gpioPortK;
    PharosRawInterruptHandler gpioPortL;
    PharosRawInterruptHandler ssi2RxTx;
    PharosRawInterruptHandler ssi3RxTx;
    PharosRawInterruptHandler uart3RxTx;
    PharosRawInterruptHandler uart4RxTx;
    PharosRawInterruptHandler uart5RxTx;
    PharosRawInterruptHandler uart6RxTx;
    PharosRawInterruptHandler uart7RxTx;
    PharosRawInterruptHandler i2c2;
    PharosRawInterruptHandler i2c3;
    PharosRawInterruptHandler timer4A;
    PharosRawInterruptHandler timer4B;
    PharosRawInterruptHandler timer5A;
    PharosRawInterruptHandler timer5B;
    PharosRawInterruptHandler fpu;
    PharosRawInterruptHandler reserved1;
    PharosRawInterruptHandler reserved2;
    PharosRawInterruptHandler i2c4;
    PharosRawInterruptHandler i2c5;
    PharosRawInterruptHandler gioPortM;
    PharosRawInterruptHandler gioPortN;
    PharosRawInterruptHandler reserved3;
    PharosRawInterruptHandler tamper;
    PharosRawInterruptHandler gioPortP;
    PharosRawInterruptHandler gioPortP1;
    PharosRawInterruptHandler gioPortP2;
    PharosRawInterruptHandler gioPortP3;
    PharosRawInterruptHandler gioPortP4;
    PharosRawInterruptHandler gioPortP5;
    PharosRawInterruptHandler gioPortP6;
    PharosRawInterruptHandler gioPortP7;
    PharosRawInterruptHandler gioPortQ;
    PharosRawInterruptHandler gioPortQ1;
    PharosRawInterruptHandler gioPortQ2;
    PharosRawInterruptHandler gioPortQ3;
    PharosRawInterruptHandler gioPortQ4;
    PharosRawInterruptHandler gioPortQ5;
    PharosRawInterruptHandler gioPortQ6;
    PharosRawInterruptHandler gioPortQ7;
    PharosRawInterruptHandler gioPortR;
    PharosRawInterruptHandler gioPortS;
    PharosRawInterruptHandler shaMd5;
    PharosRawInterruptHandler aes;
    PharosRawInterruptHandler des3des;
    PharosRawInterruptHandler lcdController;
    PharosRawInterruptHandler timer6A;
    PharosRawInterruptHandler timer6B;
    PharosRawInterruptHandler timer7A;
    PharosRawInterruptHandler timer7B;
    PharosRawInterruptHandler i2C6;
    PharosRawInterruptHandler i2C7;
    PharosRawInterruptHandler himScanMatrixKeyboard;
    PharosRawInterruptHandler oneWire;
    PharosRawInterruptHandler himPS2;
    PharosRawInterruptHandler himLedSequencer;
    PharosRawInterruptHandler himConsumerIr;
    PharosRawInterruptHandler i2c8;
    PharosRawInterruptHandler i2c9;
    PharosRawInterruptHandler gpioPortT;
    PharosRawInterruptHandler fan1;
    PharosRawInterruptHandler reserved4;
};


/**
 * restore the previous pack definition
 */
#pragma pack(pop)


#endif	/* PHAROS_V7M_ISR_H */
