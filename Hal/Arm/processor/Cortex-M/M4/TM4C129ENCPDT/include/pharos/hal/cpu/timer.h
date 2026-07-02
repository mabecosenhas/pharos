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


#ifndef PHAROS_CPU_TIMER_H
#define	PHAROS_CPU_TIMER_H

#include <pharos/hal/cpu/declarations.h>


typedef enum
{
    PHAROS_CPU_TIMER0_BASE = 0x40030000 ,
    PHAROS_CPU_TIMER1_BASE = 0x40031000 ,
    PHAROS_CPU_TIMER2_BASE = 0x40032000 ,
    PHAROS_CPU_TIMER3_BASE = 0x40033000 ,
    PHAROS_CPU_TIMER4_BASE = 0x40034000 ,
    PHAROS_CPU_TIMER5_BASE = 0x40035000 ,
    PHAROS_CPU_TIMER6_BASE = 0x400e0000 ,
    PHAROS_CPU_TIMER7_BASE = 0x400e1000 ,

} PharosCpuTimerAddress , *ptrPharosCpuTimerAddress;


typedef enum
{
    /**
     * Configuration for one shot timer, with full width (32-bit)
     */
    PHAROS_CPU_TIMER_CFG_ONE_SHOT = 0x00000021U ,

    /**
     * Full-width one-shot up-count
     */
    PHAROS_CPU_TIMER_CFG_ONE_SHOT_UP = 0x31U ,

    /**
     *Full-width periodic 
     */
    PHAROS_CPU_TIMER_CFG_PERIODIC = 0x00000022U ,

    /**
     * Full-width periodic up-count 
     */
    PHAROS_CPU_TIMER_CFG_PERIODIC_UP = 0x00000032U ,

    /**
     * Full-width RTC 
     */
    PHAROS_CPU_TIMER_CFG_RTC = 0x01000000U ,

    /**
     * Two half-width timers
     */
    PHAROS_CPU_TIMER_CFG_SPLIT_PAIR = 0x04000000U ,

    /**
     * Timer A one-shot 
     */
    PHAROS_CPU_TIMER_CFG_A_ONE_SHOT = 0x00000021U ,

    /**
     * Timer A one-shot up-count 
     */
    PHAROS_CPU_TIMER_CFG_A_ONE_SHOT_UP = 0x00000031U ,

    /**
     * Timer A periodic 
     */
    PHAROS_CPU_TIMER_CFG_A_PERIODIC = 0x00000022U ,

    /**
     * Timer A periodic up-count 
     */
    PHAROS_CPU_TIMER_CFG_A_PERIODIC_UP = 0x00000032U ,

    /**
     * Timer A event counter
     */
    PHAROS_CPU_TIMER_CFG_A_CAP_COUNT = 0x00000003U ,

    /**
     * Timer A event up-counter
     */
    PHAROS_CPU_TIMER_CFG_A_CAP_COUNT_UP = 0x00000013U ,

    /**
     * Timer A event 
     */
    PHAROS_CPU_TIMER_CFG_A_CAP_TIME = 0x00000007U ,

    /**
     * Timer A event up-count 
     */
    PHAROS_CPU_TIMER_CFG_A_CAP_TIME_UP = 0x00000017U ,

    /**
     * Timer A PWM output
     */
    PHAROS_CPU_TIMER_CFG_A_PWM = 0x0000000AU ,

    /**
     * Timer B one-shot 
     */
    PHAROS_CPU_TIMER_CFG_B_ONE_SHOT = 0x00002100U ,

    /**
     * Timer B one-shot up-count 
     */
    PHAROS_CPU_TIMER_CFG_B_ONE_SHOT_UP = 0x00003100U ,

    /**
     * Timer B periodic 
     */
    PHAROS_CPU_TIMER_CFG_B_PERIODIC = 0x00002200U ,

    /**
     * Timer B periodic up-count 
     */
    PHAROS_CPU_TIMER_CFG_B_PERIODIC_UP = 0x00003200U ,

    /**
     * Timer B event counter
     */
    PHAROS_CPU_TIMER_CFG_B_CAP_COUNT = 0x00000300U ,

    /**
     * Timer B event up-counter
     */
    PHAROS_CPU_TIMER_CFG_B_CAP_COUNT_UP = 0x00001300U ,

    /**
     * Timer B event 
     */
    PHAROS_CPU_TIMER_CFG_B_CAP_TIME = 0x00000700U ,

    /**
     * Timer B event up-count 
     */
    PHAROS_CPU_TIMER_CFG_B_CAP_TIME_UP = 0x00001700U ,

    /**
     * Timer B PWM output
     */
    PHAROS_CPU_TIMER_CFG_B_PWM = 0x00000A00U ,

    /**
     * Timer A compare action disable time-out interrupt
     */
    PHAROS_CPU_TIMER_CFG_A_ACT_TOINTD = 0x00010000U ,

    /**
     * Timer A compare action none
     */
    PHAROS_CPU_TIMER_CFG_A_ACT_NONE = 0x00000000U ,

    /**
     * Timer A compare action toggle
     */
    PHAROS_CPU_TIMER_CFG_A_ACT_TOGGLE = 0x00020000U ,

    /**
     * Timer A compare action CCP clear on time-out
     */
    PHAROS_CPU_TIMER_CFG_A_ACT_CLRTO = 0x00040000U ,

    /**
     * Timer A compare action CCP set on time-out
     */
    PHAROS_CPU_TIMER_CFG_A_ACT_SETTO = 0x00060000U ,

    /**
     * Timer A compare action set CCP toggle on time-out
     */
    PHAROS_CPU_TIMER_CFG_A_ACT_SETTOGTO = 0x00080000U ,

    /**
     * Timer A compare action clear CCP toggle on time-out
     */
    PHAROS_CPU_TIMER_CFG_A_ACT_CLRTOGTO = 0x000A0000U ,

    /**
     * Timer A compare action set CCP clear on time-out
     */
    PHAROS_CPU_TIMER_CFG_A_ACT_SETCLRTO = 0x000C0000U ,

    /**
     * Timer A compare action clear CCP set on time-out
     */
    PHAROS_CPU_TIMER_CFG_A_ACT_CLRSETTO = 0x000E0000U ,

    /**
     * Timer B compare action disable time-out interrupt
     */
    PHAROS_CPU_TIMER_CFG_B_ACT_TOINTD = 0x00100000U ,

    /**
     * Timer A compare action none
     */
    PHAROS_CPU_TIMER_CFG_B_ACT_NONE = 0x00000000U ,

    /**
     * Timer A compare action toggle
     */
    PHAROS_CPU_TIMER_CFG_B_ACT_TOGGLE = 0x00200000U ,

    /**
     *  Timer A compare action CCP clear on time-out
     */
    PHAROS_CPU_TIMER_CFG_B_ACT_CLRTO = 0x00400000U ,

    /**
     * Timer A compare action CCP set on time-out
     */
    PHAROS_CPU_TIMER_CFG_B_ACT_SETTO = 0x00600000U ,

    /**
     * Timer A compare action set CCP toggle on time-out
     */
    PHAROS_CPU_TIMER_CFG_B_ACT_SETTOGTO = 0x00800000U ,

    /**
     * Timer A compare action clear CCP toggle on time-out
     */
    PHAROS_CPU_TIMER_CFG_B_ACT_CLRTOGTO = 0x00A00000U ,

    /**
     * Timer A compare action set CCP clear on time-out
     */
    PHAROS_CPU_TIMER_CFG_B_ACT_SETCLRTO = 0x00C00000U ,

    /**
     * Timer A compare action clear CCP set on time-out.
     */
    PHAROS_CPU_TIMER_CFG_B_ACT_CLRSETTO = 0x0000E000U ,

} PharosCpuTimerConfiguration , *ptrPharosCpuTimerConfiguration;


typedef enum
{
    /**
     * Select only timer A
     */
    PHAROS_CPU_TIMER_SELECT_A = 0x000000ffU ,

    /**
     * Select only timer B
     */
    PHAROS_CPU_TIMER_SELECT_B = 0x0000ff00U ,

    /**
     * Select both timers
     */
    PHAROS_CPU_TIMER_SELECT_BOTH = 0x0000ffffU ,

} PharosCpuTimerSelect , *ptrPharosCpuTimerSelect;


typedef enum
{
    /**
     * TimerB DMA Complete Interrupt
     */
    PHAROS_CPU_TIMER_TIMB_DMA = 0x00002000 ,

    /**
     * TimerB match interrupt
     */
    PHAROS_CPU_TIMER_TIMB_MATCH = 0x00000800 ,

    /**
     * CaptureB event interrupt
     */
    PHAROS_CPU_TIMER_CAPB_EVENT = 0x00000400 ,

    /**
     * CaptureB match interrupt
     */
    PHAROS_CPU_TIMER_CAPB_MATCH = 0x00000200 ,

    /**
     * TimerB time out interrupt
     */
    PHAROS_CPU_TIMER_TIMB_TIMEOUT = 0x00000100 ,

    /**
     * TimerA DMA Complete Interrupt
     */
    PHAROS_CPU_TIMER_TIMA_DMA = 0x00000020 ,

    /**
     * TimerA match interrupt
     */
    PHAROS_CPU_TIMER_TIMA_MATCH = 0x00000010 ,

    /**
     * RTC interrupt mask
     */
    PHAROS_CPU_TIMER_RTC_MATCH = 0x00000008 ,

    /**
     * CaptureA event interrupt
     */
    PHAROS_CPU_TIMER_CAPA_EVENT = 0x00000004 ,

    /**
     * CaptureA match interrupt
     */
    PHAROS_CPU_TIMER_CAPA_MATCH = 0x00000002 ,

    /**
     * TimerA time out interrupt
     */
    PHAROS_CPU_TIMER_TIMA_TIMEOUT = 0x00000001 ,
} PharosCpuTimerIntConf , *ptrPharosCpuTimerIntConf;


/**
 * Initialize the timer 0-A for WCET
 */
void pharosCpuTimerWcetInit(void);


/**
 * Enable interrupts for a specific timer
 * 
 * @param timerAddress timer to enable
 * @param intConf interrupt configuration 
 */
void pharosCpuTimerIntEnable(PharosCpuTimerAddress timerAddress , PharosCpuTimerIntConf intConf);


/**
 * Enables the interrupt of the specified timer
 * 
 * @param interruptNumber timer interrupt to enable
 */
void pharosCpuInterruptEnable(PharosInterruptNumber interruptNumber);


/**
 * Disable interrupts for a specific timer
 * 
 * @param timerAddress timer to disable
 * @param timer timer selected (A, B or both)
 */
void pharosCpuTimerIntDisable(PharosCpuTimerAddress timerAddress , PharosCpuTimerSelect timer);


/**
 * Disables the specified timer
 * 
 * @param timerAddress timer address
 * @param timer timer selected (A, B or both)
 */
void pharosCpuTimerDisable(PharosCpuTimerAddress timerAddress , PharosCpuTimerSelect timer);


/**
 * Configure the specified timer
 * 
 * @param timerAddress timer to configure
 * @param conf timer configuration
 */
void pharosCpuTimerConfigure(PharosCpuTimerAddress timerAddress , PharosCpuTimerConfiguration conf);


/**
 * Clear the interrupt for a specified timer 
 * 
 * @param timerAddress timer to clear the interrupt
 * @param intConf configuration of the interrupt
 */
void pharosCpuTimerIntClear(PharosCpuTimerAddress timerAddress , PharosCpuTimerIntConf intConf);


/**
 * Loads a value for a specified timer
 * 
 * @param timerAddress timer address
 * @param timer timer selected (A, B or both)
 * @param value value to load on the timer
 */
void pharosCpuTimerLoadSet(PharosCpuTimerAddress timerAddress , PharosCpuTimerSelect timer , uint32_t value);


/**
 * Get the current value of the specified timer
 * 
 * @param timerAddress timer address
 * @param timer timer selected (A, B or both)
 * 
 * @return returns the current value of the timer
 */
uint32_t pharosCpuTimerLoadGet(PharosCpuTimerAddress timerAddress , PharosCpuTimerSelect timer);


/**
 * Timer enable 
 * 
 * @param timerAddress timer to enable
 * @param timer timer selected (A, B or both)
 */
void pharosCpuTimerEnable(PharosCpuTimerAddress timerAddress , PharosCpuTimerSelect timer);



/* PHAROS_TIMER_H */
#endif	
