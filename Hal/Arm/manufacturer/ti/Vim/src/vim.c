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


#include <pharos/hal/interrupt/vim.h>
#include <pharos/hal/cpu/cpu.h>
#include <pharos/hal/cpuapi/arm.h>
#include <pharos/hal/arm/declarations.h>
#include <pharos/kernel/interrupt/interrupt.h>
#include <pharos/error.h>
#include <pharos/hal/cpu/sections.h>
#include <pharos/kernel/interrupt/interruptinline.h>


/**
 * Gives the number of the CHANCTRL register to update. For example:
 *   if channel is   3 will give  0
 *   if channel is   5 will give  1
 *   if channel is 126 will give 31
 */
#define PHAROS_CPU_CHANCTRL_NUMBER(channel)                 (channel >> 2U)

/**
 * Gives the bit offset inside the CHANCTRL register to update. For example:
 *   if channel is   3 will give   0
 *   if channel is   4 will give  24
 *   if channel is   5 will give  16
 *   if channel is 126 will give   8
 */
#define PHAROS_CPU_CHANCTRL_BITPOS(channel)                 (24U-(channel%4U) * 8U)            

/**
 * Gives the mask inside the CHANCTRL register to update. For example:
 *   if channel is   3 will give 0xffffff00
 *   if channel is   4 will give 0x00ffffff
 *   if channel is   5 will give 0xff00ffff
 *   if channel is 126 will give 0xffff00ff
 */
#define PHAROS_CPU_CHANCTRL_MASK(channel)                   (0xffffffffU - (0xffU << PHAROS_CPU_CHANCTRL_BITPOS(channel) ) )


/**
 * Gives the number to add to the CHANCTRL register to update. For example:
 *   if channel is   3 and number is 100 will give 100 <<  0
 *   if channel is   4 and number is  10 will give  10 << 24
 *   if channel is   5 and number is   7 will give   7 << 16
 *   if channel is 126 and number is   6 will give   6 <<  8
 */
#define PHAROS_CPU_CHANCTRL_CHANNEL(channel,number)         (number << PHAROS_CPU_CHANCTRL_BITPOS(channel))



#define DISABLE_INTERRUPT(x)            ((uint32_t) 1U << (x))
#define ENABLE_INTERRUPT(x)             ((uint32_t) 1U << (x))
#define VIM                             ((ptrPharosCpuVim) PHAROS_VIM_ADDRESS)
#define VIM_TABLE                       ((ptrVimInterruptTable) PHAROS_VIM_TABLE_ADDRESS)


#define PHAROS_CPU_VIM_STP_FIQ(x)       (1U << (x))
#define PHAROS_CPU_VIM_STP_IRQ(x)       (0U << (x))


typedef void (*isrEntry)(void);


typedef struct VimInterruptTable VimInterruptTable , *ptrVimInterruptTable;

struct VimInterruptTable
{
    isrEntry isr[PHAROS_CPU_VIM_CHANNEL_SIZE];
};


/**
 * Unmask an interrupt 
 * 
 * @param number interrupt number
 */
static void armVimUnmaskInterrupt(ptrInterruptConfiguration number);


/**
 * Mask an interrupt (interrupt will not be triggered)
 * 
 * @param number interrupt number 
 */
static void armVimMaskInterrupt(ptrInterruptConfiguration number);


/**
 * VIM function to enable interrupts 
 */
void pharosCpuReenableInterrupts(void);


/**
 * VIM function to disable interrupts
 */
void pharosCpuRedisableInterrupts(void);


/**
 * ECC bit error handler for single and double bit errors on the interrupt vector table.
 */
static void eccErrorHandler();


/**
 * Sets the interrupt line to wake up the CPU if is dormant
 * 
 * @param conf interrupt configuration
 */
static void armVimInterruptWakeUpSet(ptrInterruptConfiguration conf);


/**
 * Sets the interrupt line to NOT wake up the CPU if is dormant
 * 
 * @param conf interrupt configuration
 */
static void armVimInterruptWakeUpClear(ptrInterruptConfiguration conf);


void pharosCpuVimInitializeIsrConf(ptrInterruptConfiguration conf)
{
    /* check if the channel number is invalid */
    if(conf->cpu.channel > PHAROS_INTERRUPT_CHANNEL_MAX || conf->cpu.channel < PHAROS_INTERRUPT_CHANNEL_2)
    {
        /* cannot install the interrupt with invalid channel */
        pharosFatalErrorAnnounce(PHAROS_FATAL_CPU_ISR_INV_CHA , __FILE__ , __func__ , __LINE__);
    }
        /* check if the interrupt number is invalid */
    else if(conf->number > PHAROS_CPU_VIM_ISR_NUMBER)
    {
        /* cannot install the interrupt with invalid interrupt number */
        pharosFatalErrorAnnounce(PHAROS_FATAL_CPU_ISR_INV_NUM , __FILE__ , __func__ , __LINE__);
    }
        /* else, the number is correct */
    else
    {

        /* if the interrupt is wake up */
        if(conf->cpu.isWakeUp == TRUE)
        {
            /* set the interrupt to awake the CPU */
            armVimInterruptWakeUpSet(conf);
        }
        else
        {
            /* set the interrupt to not awake the CPU */
            armVimInterruptWakeUpClear(conf);
        }

        /* setup the channel to catch the specified interrupt number */

        /* first clear the bits related to the interrupt channel (this will make channel point to the interrupt number 0).
         * Note: have to change this in a single instruction otherwise an error (ESM) will be raised */
        VIM->channelControlTable[PHAROS_CPU_CHANCTRL_NUMBER(conf->cpu.channel)] =
                (VIM->channelControlTable[PHAROS_CPU_CHANCTRL_NUMBER(conf->cpu.channel)] & PHAROS_CPU_CHANCTRL_MASK(conf->cpu.channel)) |
                PHAROS_CPU_CHANCTRL_CHANNEL(conf->cpu.channel , conf->number);
    }
}


void pharosCpuVimInitialize(ptrInterruptTable table)
{
    uint32_t i;

    /* disable the VIM ECC completely */
    VIM->interruptVectorTableEccControl = PHAROS_CPU_VIM_ECCCTL_ECC_DIS | PHAROS_CPU_VIM_ECCCTL_EDAC_DIS;

    /* for every channel */
    for(i = 0U; i < PHAROS_CPU_VIM_CHANNEL_SIZE; i++)
    {
        /* install the interrupt routine for the channel as the default ISR handler */
        VIM_TABLE->isr[i] = pharosCpuIsrHandler;
    }

    /* set the ECC error handler for the VIM channel table (would triggered on error on the VIM table 
     * IF the VIM ECC was enabled) */
    VIM->fallBackVectorAddress = &eccErrorHandler;

    /* set channel 0 and 1 for FIQ and the 2 - 31 to IRQ (channel 0 and 1 have to be FIQ) */
    VIM->fiqIrqProgramControl0 = PHAROS_CPU_VIM_STP_FIQ(0U) | PHAROS_CPU_VIM_STP_FIQ(1U);

    /* set the channels (32 to 63) to be IRQs */
    VIM->fiqIrqProgramControl1 = 0U;

    /* set the channels (64 to 95) to be IRQs */
    VIM->fiqIrqProgramControl2 = 0U;

    /* set the channels (96 to 127) to be IRQs */
    VIM->fiqIrqProgramControl3 = 0U;

    /* enable channel 0 and 1 (cannot be disabled) - disable 2 to 31 */
    VIM->interruptEnableSet0 = ENABLE_INTERRUPT(0U) | ENABLE_INTERRUPT(1U);

    /* disable channels 32 to 63 */
    VIM->interruptEnableSet1 = 0U;

    /* disable channels 64 to 95 */
    VIM->interruptEnableSet2 = 0U;

    /* disable channels 96 to 127 */
    VIM->interruptEnableSet3 = 0U;

    /* map all channels to interrupt 0 (except channel 1 which must be mapped to interrupt 1) to reset all 
     * channel/interrupt attributions. Note that channel 0 is the highest priority channel and is always mapped to 
     * interrupt 0, so the other channels mapped to interrupt 0 will never be triggered. This so that configuring 
     * a new interrupt to a channel will be mapped to two channels (the default one and the configured one) */

    /* map the channel 0 to interrupt 0 and channel 1 to interrupt 1 (channels 2 and 3 to interrupt 0) */
    VIM->channelControlTable[0] = 0x00010000 | PHAROS_CPU_ISR_PHANTOM << 8U | PHAROS_CPU_ISR_PHANTOM;

    /* for every channel */
    for(i = 1U; i < 32U; i++)
    {
        /* map it to interrupt 0 */
        VIM->channelControlTable[i] = PHAROS_CPU_ISR_PHANTOM << 24U | PHAROS_CPU_ISR_PHANTOM << 16U | PHAROS_CPU_ISR_PHANTOM << 8U | PHAROS_CPU_ISR_PHANTOM;
    }

    /* set capture event sources for the RTI */
    VIM->captureEvent = PHAROS_CPU_VIM_CAPEVT_SRC1(0U) | PHAROS_CPU_VIM_CAPEVT_SRC0(0U);

    /* for every interrupt */
    for(i = 0U; i < table->size; i++)
    {
        /* install the interrupt configuration */
        pharosCpuVimInitializeIsrConf(&table->interrupts[i]);
    }
}


void armVimInterruptWakeUpSet(ptrInterruptConfiguration conf)
{
    /* get the channel associated with the interrupt */
    uint32_t interruptNumber = conf->number;

    if(interruptNumber >= 96U)
    {
        VIM->wakeUpEnableSet3 = ENABLE_INTERRUPT(interruptNumber - 96U);
    }
    else if(interruptNumber >= 64U)
    {
        VIM->wakeUpEnableSet2 = ENABLE_INTERRUPT(interruptNumber - 64U);
    }
    else if(interruptNumber >= 32U)
    {
        VIM->wakeUpEnableSet1 = ENABLE_INTERRUPT(interruptNumber - 32U);
    }
    else if(interruptNumber >= 2U)
    {
        VIM->wakeUpEnableSet0 = ENABLE_INTERRUPT(interruptNumber);
    }
    else
    {
        /* channel is incorrect */
    }
}


void armVimInterruptWakeUpClear(ptrInterruptConfiguration conf)
{
    /* get the channel associated with the interrupt */
    uint32_t interruptNumber = conf->number;

    if(interruptNumber >= 96U)
    {
        VIM->wakeUpEnableClear3 = (uint32_t) 1U << (interruptNumber - 96U);
    }
    else if(interruptNumber >= 64U)
    {
        VIM->wakeUpEnableClear2 = (uint32_t) 1U << (interruptNumber - 64U);
    }
    else if(interruptNumber >= 32U)
    {
        VIM->wakeUpEnableClear1 = (uint32_t) 1U << (interruptNumber - 32U);
    }
    else if(interruptNumber >= 2U)
    {
        VIM->wakeUpEnableClear0 = (uint32_t) 1U << interruptNumber;
    }
    else
    {
        /* channel is incorrect */
    }
}


void armVimUnmaskInterrupt(ptrInterruptConfiguration conf)
{
    /* get the channel associated with the interrupt */
    uint32_t channel = conf->cpu.channel;

    if(channel >= 96U)
    {
        VIM->interruptEnableSet3 = (uint32_t) 1U << (channel - 96U);
    }
    else if(channel >= 64U)
    {
        VIM->interruptEnableSet2 = (uint32_t) 1U << (channel - 64U);
    }
    else if(channel >= 32U)
    {
        VIM->interruptEnableSet1 = (uint32_t) 1U << (channel - 32U);
    }
    else if(channel >= 2U)
    {
        VIM->interruptEnableSet0 = (uint32_t) 1U << channel;
    }
    else
    {
        /* channel 0 and 1 cannot be masked/unmasked (they are always unmasked) */
    }
}


void armVimMaskInterrupt(ptrInterruptConfiguration conf)
{
    /* get the channel associated with the interrupt */
    uint32_t channel = conf->cpu.channel;


    if(channel >= 96U)
    {
        VIM->interruptEnableClear3 = (uint32_t) 1U << (channel - 96U);
    }
    else if(channel >= 64U)
    {
        VIM->interruptEnableClear2 = (uint32_t) 1U << (channel - 64U);
    }
    else if(channel >= 32U)
    {
        VIM->interruptEnableClear1 = (uint32_t) 1U << (channel - 32U);
    }
    else if(channel >= 2U)
    {
        VIM->interruptEnableClear0 = (uint32_t) 1U << channel;
    }
    else
    {
        /* channel 0 and 1 cannot be masked/unmasked (they are always unmasked) */
    }
}


void pharosCpuInterruptInstall(ptrInterruptConfiguration conf)
{
    /* activate the interrupt */
    armVimUnmaskInterrupt(conf);
}


void pharosCpuInterruptRemove(ptrInterruptConfiguration conf)
{
    /* only when there is no handler can de-activate the interrupt */
    armVimMaskInterrupt(conf);
}


void pharosCFiqHandler(void)
{
    /* clear the ESM error */
    pharosEsmClearError(VIM->fiqIndex);
}


void pharosArmCInterruptHandler(const uint8_t *interruptedAddress)
{
    /* interrupt configuration */
    ptrInterruptConfiguration conf;

    /* return of the pre-handler */
    uintptr_t preInterruptHandlerReturn;

    /* read from the IRQ index register which interrupt has the highest priority and should be run */
    uint32_t channel = VIM->irqIndex - 1;

    /* save the interrupts enabled from 0 - 31 */
    uint32_t oldEnabledIsr0 = VIM->interruptEnableSet0;

    /* save the interrupts enabled from 32 - 63 */
    uint32_t oldEnabledIsr1 = VIM->interruptEnableSet1;

    /* save the interrupts enabled from 64 - 95 */
    uint32_t oldEnabledIsr2 = VIM->interruptEnableSet2;

    /* save the interrupts enabled from 96 - 127 */
    uint32_t oldEnabledIsr3 = VIM->interruptEnableSet3;

    /* interrupted partition running */
    ptrPartition oldRunning = pharosIInterruptRunningEnvGet();


    /* using a VIM requires that we disable all lower priority (meaning, lower channels) and also the current channel */

    /* if channel priority is too low */
    if(channel >= 96U)
    {
        /* only clear the lower register (channels > channel) */
        VIM->interruptEnableClear3 = ((0xFFFFFFFFU << (channel - 96U)) & (~VIM->fiqIrqProgramControl3));

        /* make sure the register has been written before enabling interrupts */
        VIM->interruptEnableClear3;
    }
    else if(channel >= 64U)
    {
        /* only clear the lower register (channels > channel) */
        VIM->interruptEnableClear2 = ((0xFFFFFFFFU << (channel - 64U)) & (~VIM->fiqIrqProgramControl2));

        /* disable all interrupts with channel >= 96 */
        VIM->interruptEnableClear3 = (0xFFFFFFFFU & (~VIM->fiqIrqProgramControl3));

        /* make sure the register has been written before enabling interrupts */
        VIM->interruptEnableClear2;

        /* make sure the register has been written before enabling interrupts */
        VIM->interruptEnableClear3;

    }
    else if(channel >= 32U)
    {
        /* only clear the lower register (channels > channel) */
        VIM->interruptEnableClear1 = ((0xFFFFFFFFU << (channel - 32U)) & (~VIM->fiqIrqProgramControl1));

        /* disable all interrupts with 96 > channel >= 64 */
        VIM->interruptEnableClear2 = (0xFFFFFFFFU & (~VIM->fiqIrqProgramControl2));

        /* disable all interrupts with channel >= 96 */
        VIM->interruptEnableClear3 = (0xFFFFFFFFU & (~VIM->fiqIrqProgramControl3));

        /* make sure the register has been written before enabling interrupts */
        VIM->interruptEnableClear1;

        /* make sure the register has been written before enabling interrupts */
        VIM->interruptEnableClear2;

        /* make sure the register has been written before enabling interrupts */
        VIM->interruptEnableClear3;
    }
    else if(channel >= 2U)
    {
        /* only clear the lower register (channels > channel) */
        VIM->interruptEnableClear0 = ((0xFFFFFFFFU << (channel - 0U)) & (~VIM->fiqIrqProgramControl0));

        /* disable all interrupts with 64 > channel >= 32 */
        VIM->interruptEnableClear1 = (0xFFFFFFFFU & (~VIM->fiqIrqProgramControl1));

        /* disable all interrupts with 96 > channel >= 64 */
        VIM->interruptEnableClear2 = (0xFFFFFFFFU & (~VIM->fiqIrqProgramControl2));

        /* disable all interrupts with channel >= 96 */
        VIM->interruptEnableClear3 = (0xFFFFFFFFU & (~VIM->fiqIrqProgramControl3));

        /* make sure the register has been written before enabling interrupts */
        VIM->interruptEnableClear0;

        /* make sure the register has been written before enabling interrupts */
        VIM->interruptEnableClear1;

        /* make sure the register has been written before enabling interrupts */
        VIM->interruptEnableClear2;

        /* make sure the register has been written before enabling interrupts */
        VIM->interruptEnableClear3;
    }

    /* now that all lower priority channels (and the current channel) have been disabled, we can re-enable interrupts.
     * This will allow only higher priority channels to preempt the current ISR */

    /* get the interrupt configuration */
    conf = pharosIInterruptConfigurationGet(channel);

    /* if there is a routine to clear the interrupt */
    if(conf->preInterruptHandler != NULL)
    {
        /* clear the interrupt using the clear routine */
        preInterruptHandlerReturn = conf->preInterruptHandler(conf->argument);
    }
    else
    {
        /* set the default return value */
        preInterruptHandlerReturn = 0U;
    }

    /* now we can enable interrupts since the source that triggered the interrupt has been dealt with */
    pharosCpuInterruptNestedEnable();

    /* execute all interrupt handlers for the channel */
    pharosIInterruptExecute(conf , preInterruptHandlerReturn , interruptedAddress);

    /* disable interrupts */
    pharosCpuInterruptNestedDisable();

    /* restore the interrupts that were enabled (from 0 to 31) */
    VIM->interruptEnableSet0 = oldEnabledIsr0;

    /* restore the interrupts that were enabled (from 32 to 63) */
    VIM->interruptEnableSet1 = oldEnabledIsr1;

    /* restore the interrupts that were enabled (from 64 to 95) */
    VIM->interruptEnableSet2 = oldEnabledIsr2;

    /* restore the interrupts that were enabled (from 96 to 127) */
    VIM->interruptEnableSet3 = oldEnabledIsr3;

    /* restore the previous running interrupt partition */
    pharosIInterruptRunningEnvSet(oldRunning);
}


void eccErrorHandler(void)
{
    /* if the error is a single bit error */
    if((VIM->interruptVectorTableEccStatus & PHAROS_CPU_VIM_ECCSTAT_SBE_CLR) == PHAROS_CPU_VIM_ECCSTAT_SBE_CLR)
    {
        /* then invoke the error handler to let the app know, but the ECC has corrected this */
        pharosErrorHandler(PHAROS_ERROR_CPU_VIM_ECC_SBE , (uint8_t *) VIM->singleBitErrorAddress);

        /* clear single bit error (already handled it) */
        VIM->interruptVectorTableEccStatus = PHAROS_CPU_VIM_ECCSTAT_SBE_CLR;
    }
    else
    {
        /* then invoke the error handler to let the app know, the ECC could not correct the address, but we can */
        pharosErrorHandler(PHAROS_ERROR_CPU_VIM_ECC_DBE , (uint8_t *) VIM->uncorrectableErrorAddress);

        /* all entries in the VIM Table are the same, we can correct it easily */
        *VIM->uncorrectableErrorAddress = (uint32_t) pharosCpuIsrHandler;

        /* clear double bit errors (already corrected the error) */
        VIM->interruptVectorTableEccStatus = PHAROS_CPU_VIM_ECCSTAT_DBE_CLR;
    }
}
