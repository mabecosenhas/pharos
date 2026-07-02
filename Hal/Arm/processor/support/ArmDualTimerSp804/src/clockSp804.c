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


/**
 * The code was derived from the technical manual: ARM Dual Timer Module (SP804) r1p0 - Technical Reference Manual
 * 
 * Further information regarding the functionality of this ARM component is documented on this Manual.
 */
#include <pharos/hal/clockSp804.h>

/**************************************************************
 * Definition of the TIMER control bits
 **************************************************************/

/**
 * First bit of the timer control structure
 */
#define CLOCK_SP804_TIMER_CONTROL_ONE_SHOT                  (0x00U)   
#define CLOCK_SP804_TIMER_CONTROL_WRAPPED                   (0x01U)

/**
 * Second bit of the timer control structure
 */
#define CLOCK_SP804_TIMER_CONTROL_16_BITS                   (0x00U)
#define CLOCK_SP804_TIMER_CONTROL_32_BITS                   (0x02U)

#define CLOCK_SP804_TIMER_CONTROL_SCALER_0                  (0x00U)
#define CLOCK_SP804_TIMER_CONTROL_SCALER_1                  (0x04U)
#define CLOCK_SP804_TIMER_CONTROL_SCALER_2                  (0x08U) 

#define CLOCK_SP804_TIMER_CONTROL_INTERRUPT_DISABLE         (0x00U)
#define CLOCK_SP804_TIMER_CONTROL_INTERRUPT_ENABLE          (0x20U)

#define CLOCK_SP804_TIMER_CONTROL_FREE_RUNNING              (0x00U)
#define CLOCK_SP804_TIMER_CONTROL_PERIODIC                  (0x40U) 

#define CLOCK_SP804_TIMER_CONTROL_DISABLE                   (0x00U) 
#define CLOCK_SP804_TIMER_CONTROL_ENABLE                    (0x80U) 


void armSp804TimerInitPeriodic32Bit(uint32_t periodUs , const ptrArmSp804Timer c)
{
    /* set the timer load value */
    c->timerValue = periodUs - 1U;

    /* set the timer load value */
    c->timerLoad = periodUs - 1U;

    /* set the timer control value to:
     * - is not one shot (periodic) 
     * - count in 32 bit mode (as opposed to 16 bit)
     * - don't use scaler stages
     * - generate interrupt
     * - reset the counter once it reaches 0
     * - enable the timer */
    c->timerControl =
            CLOCK_SP804_TIMER_CONTROL_32_BITS |
            CLOCK_SP804_TIMER_CONTROL_SCALER_0 |
            CLOCK_SP804_TIMER_CONTROL_INTERRUPT_ENABLE |
            CLOCK_SP804_TIMER_CONTROL_PERIODIC |
            CLOCK_SP804_TIMER_CONTROL_ENABLE;
}


void armSp804TimerInitOneShot32Bit(uint32_t timeUs , ptrArmSp804Timer c)
{
    /* clear any pending interrupt */
    c->timerInterruptClear = 0U;

    /* set the timer load value with the time in which to fire the interrupt */
    c->timerLoad = timeUs;

    /* set the timer control value to:
     * - place one shot 
     * - count in 32 bit mode (as opposed to 16 bit)
     * - don't use scaler stages
     * - generate interrupt
     * - reset the counter once it reaches 0
     * - enable the timer */
    c->timerControl =
            CLOCK_SP804_TIMER_CONTROL_WRAPPED |
            CLOCK_SP804_TIMER_CONTROL_32_BITS |
            CLOCK_SP804_TIMER_CONTROL_SCALER_0 |
            CLOCK_SP804_TIMER_CONTROL_INTERRUPT_ENABLE |
            CLOCK_SP804_TIMER_CONTROL_FREE_RUNNING |
            CLOCK_SP804_TIMER_CONTROL_ENABLE;
}


void armSp804TimerDisable(const ptrArmSp804Timer c)
{
    /* disable the timer */
    c->timerControl = CLOCK_SP804_TIMER_CONTROL_DISABLE;

    /* clear any pending interrupt */
    c->timerInterruptClear = 0U;
}
