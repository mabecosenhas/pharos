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


#ifndef PHAROS_CLOCKSP804_H
#define PHAROS_CLOCKSP804_H

#include <pharos/hal/hal.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * Compact the size of the I/O registers
     */
#pragma pack(push ,4)


    typedef struct
    {
        /**
         * Timer load register
         * 
         * Offset 0x0
         */
        volatile uint32_t timerLoad;

        /**
         * Timer value register
         * 
         * Offset 0x4
         */
        volatile uint32_t timerValue;

        /**
         * Timer control register
         * 
         * Offset 0x8
         */
        volatile uint32_t timerControl;

        /**
         * Timer interrupt clear register
         * 
         * Offset 0xc
         */
        volatile uint32_t timerInterruptClear;

        /**
         * Timer raw interrupt status register (TimerXRIS)
         * 
         * Offset 0x10
         */
        volatile uint32_t timerInterruptRawStatus;

        /**
         * Timer interrupt mask register (TimerXMIS)
         * 
         * Offset 0x14
         */
        volatile uint32_t timerInterruptMaskStatus;

        /**
         * Timer load (background) register
         * 
         * Offset 0x18
         */
        volatile uint32_t timerBackgroundLoadRegister;

    } ArmSp804Timer , *ptrArmSp804Timer;


    /**
     * restore the previous pack definition
     */
#pragma pack(pop)


    /**
     * Initialize the timer1 as a periodic 32 bit counter 
     * 
     * @param handler interrupt handler that will be called periodically
     * @param periodUs period in microseconds
     * @param c pointer to the SP804 address in IO
     */
    void armSp804TimerInitPeriodic32Bit(uint32_t periodUs , const ptrArmSp804Timer c);


    /**
     * Read the timer counter value
     * 
     * @param clock pointer to the SP804 address in IO
     * 
     * @return returns the current counter value
     */
    INLINE uint32_t armSp804TimerRead(const ptrArmSp804Timer c)
    {
        /* return the value of the timer 1 counter */
        return c->timerValue;
    }


    /**
     * Determine if the timer interrupt is pending
     * 
     * @param c pointer to the SP804 address in IO
     * 
     * @return returns TRUE if the interrupt is pending and FALSE otherwise 
     */
    INLINE bool armSp804TimerInterruptIsPending(const ptrArmSp804Timer c)
    {
        /* get the first bit of the interrupt status (ignore other bits) to check if the interrupt is pending */
        return (c->timerInterruptMaskStatus & 0x1U) == 1U ? TRUE : FALSE;
    }


    /**
     * Initialize the timer 2 as a one-shot 32 bit counter 
     * 
     * @param handler interrupt handler that will be called
     * @param timeUs time in which to generate the interrupt (in microseconds)
     * @param c pointer to the SP804 address in IO
     */
    void armSp804TimerInitOneShot32Bit(uint32_t timeUs , ptrArmSp804Timer c);


    /**
     * disable the timer
     * 
     * @param c pointer to the SP804 address in IO
     */
    void armSp804TimerDisable(const ptrArmSp804Timer c);


    /**
     * Clear the timer interrupt 
     * 
     * @param clock pointer to the SP804 address in IO
     */
    INLINE void armSp804TimerClearInterrupt(const ptrArmSp804Timer c)
    {
        /* write anything to this register to clear it */
        c->timerInterruptClear = 0U;
    }


#ifdef __cplusplus
}
#endif

#endif /* CLOCKSP804_H */
