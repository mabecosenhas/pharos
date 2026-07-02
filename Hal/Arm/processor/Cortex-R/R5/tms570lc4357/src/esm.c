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


#include <pharos/hal/cpu/esm.h>

#define LOW_COUNTER_LOAD_VALUE      (0x4000U - 1U)


void pharosEsmInitialize(void)
{
    /****************************************************/
    /* Disable error pins                               */
    /****************************************************/

    /* disable all errors on group 1 , channels [0-31] */
    PHAROS_CPU_ESM->disableErrorAction1Ch1 = 0xFFFFFFFFU;

    /* disable all errors on group 1 , channels [32-63] */
    PHAROS_CPU_ESM->errorInfluencePinClear1Ch2 = 0xFFFFFFFFU;

    /* disable all errors on group 1 , channels [64-95] */
    PHAROS_CPU_ESM->errorInfluencePinClear1Ch3 = 0xFFFFFFFFU;

    /****************************************************/
    /* Disable ESM group interrupts                     */
    /****************************************************/

    /* disable all interrupts for group 1 , channels [0-31] */
    PHAROS_CPU_ESM->interruptEnableClear1Ch1 = 0xFFFFFFFFU;

    /* disable all interrupts for group 1 , channels [32-63] */
    PHAROS_CPU_ESM->interruptEnableClear1Ch2 = 0xFFFFFFFFU;

    /* disable all interrupts for group 1 , channels [64-95] */
    PHAROS_CPU_ESM->interruptEnableClear1Ch3 = 0xFFFFFFFFU;

    /****************************************************/
    /* clear error flags                                */
    /****************************************************/

    /* clear all pending errors on group 1 , channels [0-31] */
    PHAROS_CPU_ESM->status1Ch1 = 0xFFFFFFFFU;

    /* clear all pending errors on group 1 , channels [32-63] */
    PHAROS_CPU_ESM->status1Ch2 = 0xFFFFFFFFU;

    /* clear all pending errors on group 1 , channels [64-95] */
    PHAROS_CPU_ESM->status1Ch3 = 0xFFFFFFFFU;

    /* clear all pending errors on group 2 , shadow register */
    PHAROS_CPU_ESM->statusShadow2 = 0xFFFFFFFFU;

    /* clear all pending errors on group 2 */
    PHAROS_CPU_ESM->status2 = 0xFFFFFFFFU;

    /* clear all pending errors on group 2 , shadow register */
    PHAROS_CPU_ESM->statusShadow2 = 0xFFFFFFFFU;

    /* clear all pending errors on group 3 */
    PHAROS_CPU_ESM->status3 = 0xFFFFFFFFU;

    /********************************************************************/
    /* setup the low counter (needed to reset the !ERROR pin if needed) */
    /********************************************************************/

    /* initialize the low counter load value */
    PHAROS_CPU_ESM->errorPinLowTimeCounterLoad = LOW_COUNTER_LOAD_VALUE;

    /****************************************************/
    /* reset error pin status                           */
    /****************************************************/

    /* check if ERROR pin has any error pending */
    if(PHAROS_CPU_ESM->errorPinStatus == PHAROS_CPU_ESM_EPSR_ERR)
    {
        /* !ERROR is set to high when the low time counter (LTC) has completed */
        PHAROS_CPU_ESM->errorKey = PHAROS_CPU_ESM_EKR_RESET;
    }
        /* if no error */
    else
    {
        /* activate normal mode */
        PHAROS_CPU_ESM->errorKey = PHAROS_CPU_ESM_EKR_NORMAL;
    }

    /****************************************************/
    /* force all group 1 interrupts to be low level     */
    /****************************************************/

    /* set interrupts on group 1 , channels [0-31] , to be low priority */
    PHAROS_CPU_ESM->interrruptLevelClear1Ch1 = 0xFFFFFFFFU;

    /* set interrupts on group 1 , channels [32-63] , to be low priority */
    PHAROS_CPU_ESM->interruptLevelClear1Ch2 = 0xFFFFFFFFU;

    /* set interrupts on group 1 , channels [64-95] , to be low priority */
    PHAROS_CPU_ESM->interruptLevelClear1Ch3 = 0xFFFFFFFFU;
}


PHAROS_WEAK void pharosEsmClearError(uint32_t fiqIndex)
{
    /* if we reached here then the error should be ignored */

    if(fiqIndex == 1U)
    {
        /* check if ERROR pin has any error pending */
        if(PHAROS_CPU_ESM->errorPinStatus == PHAROS_CPU_ESM_EPSR_ERR)
        {
            /* !ERROR is set to high when the low time counter (LTC) has completed */
            PHAROS_CPU_ESM->errorKey = PHAROS_CPU_ESM_EKR_RESET;
        }
    }

    /* if the status for group 2 indicates an error */
    if(PHAROS_CPU_ESM->status2 != 0U)
    {
        /* clear it */
        PHAROS_CPU_ESM->status2 = 0xffffffffU;
        PHAROS_CPU_ESM->statusShadow2 = 0xffffffffU;
    }

    if(PHAROS_CPU_ESM->status1Ch1 != 0U)
    {
        /* clear it */
        PHAROS_CPU_ESM->status1Ch1 = 0xffffffffU;
    }

    if(PHAROS_CPU_ESM->status1Ch2 != 0U)
    {
        /* clear it */
        PHAROS_CPU_ESM->status1Ch2 = 0xffffffffU;
    }

    if(PHAROS_CPU_ESM->status1Ch3 != 0U)
    {
        /* clear it */
        PHAROS_CPU_ESM->status1Ch3 = 0xffffffffU;
    }

    if(PHAROS_CPU_ESM->status3 != 0U)
    {
        /* clear it */
        PHAROS_CPU_ESM->status3 = 0xffffffffU;
    }
}
