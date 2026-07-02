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


#include <pharos/hal/hal.h>
#include <pharos/hal/cpuapi/arm.h>
#include <pharos/hal/board.h>
#include <pharos/hal/cpu/cpu.h>
#include <pharos/kernel/partition/partition.h>
#include <pharos/hal/boardApi.h>
#include <pharos/kernel/core/core.h>
#include <pharos/hal/cpu/pll.h>
#include <pharos/hal/performance/performance.h>
#include <pharos/hal/cpu/peripheral.h>
#include <pharos/hal/cpu/mux.h>
#include <pharos/hal/cpu/flash.h>
#include <pharos/hal/cpu/system.h>
#include <pharos/hal/interrupt/vim.h>
#include <pharos/hal/memory/cache.h>
#include <pharos/hal/cpu/sci.h>
#include <pharos/hal/fpu.h>
#include <pharos/hal/cpu/declarations.h>
#include <pharos/kernel/core/rsp.h>


static void externalClockInitialize(void)
{
    /* set ECLK to GIO mode */
    PHAROS_SYSTEM_PRIMARY->pinControl1 = PHAROS_CPU_SYS1_SYSPC1_GIO;

    /* set ECLK default output value to logic low */
    PHAROS_SYSTEM_PRIMARY->pinControl4 = PHAROS_CPU_SYS1_SYSPC4_LOW;

    /* set ECLK to output */
    PHAROS_SYSTEM_PRIMARY->pinControl2 = PHAROS_CPU_SYS1_SYSPC2_OUTPUT;

    /* set ECLK pins open drain enable */
    PHAROS_SYSTEM_PRIMARY->pinControl7 = PHAROS_CPU_SYS1_SYSPC7_LOW;

    /* activate ECLK pull enable */
    PHAROS_SYSTEM_PRIMARY->pinControl8 = PHAROS_CPU_SYS1_SYSPC8_ACTIVE;

    /* ECLK pull up is selected */
    PHAROS_SYSTEM_PRIMARY->pinControl9 = PHAROS_CPU_SYS1_SYSPC9_PULLUP;

    /* setup ECLK with VCLK, disable ECLK output when in suspend mode and set the divider so that the ECLK is VCLK/8 */
    PHAROS_SYSTEM_PRIMARY->ecpControl = PHAROS_CPU_SYS1_ECPCNTL_VCLK |
        PHAROS_CPU_SYS1_ECPCNTL_DIS |
        PHAROS_CPU_SYS1_ECPCNTL_DIV(8);
}


static void resetSystem(void)
{
    /* enable ECC detection and triggering */
    pharosPmsrBusEventExportEnable();

    /* initialize the PLL */
    pharosCpuPllInitialize();

    /* initialize the peripherals */
    pharosCpuPeripheralsInitialize();

    /* initialize the pins multiplexing */
    pharosCpuIoMultiplexingInitialize();

    /* initialize the flash */
    pharosCpuFlashInitialize();

    /* initialize the low power oscillator */
    pharosCpuLpoInitialize();

    /* initialize the CPU clock */
    pharosCpuPllClockInitialize();

    /* set the external clock */
    externalClockInitialize();

    /* initialize the debug output port (as soon as possible) */
    pharosCpuSciInitialize((ptrPharosCpuSci) PHAROS_CPU_SCI_REG1);

    /* initialize the ESM */
    pharosEsmInitialize();

    /* initialize the interrupt controller */
    pharosCpuVicInitialize();

    /* initialize the FPU */
    pharosCpuFpuInitialize();
}


void pharosCpuBoot(void)
{
    /* holds the reset source */
    PharosCpuResetSource reset;


    /* initialize the memory ECLK */
    pharosCpuMemoryInitialize();

    /* get the cause of the reset */
    reset = pharosCpuResetSource();

    /* clear any reset cause */
    pharosCpuResetSourceClear();


    switch(reset)
    {
            /* case there was no reset */
        case PHAROS_CPU_RST_SRC_NO_RESET:

            /* if the reset was caused by a power on */
        case PHAROS_CPU_RST_SRC_POWER_ON:

            /* of by debug */
        case PHAROS_CPU_RST_SRC_DEBUG:

            /* or through an external source */
        case PHAROS_CPU_RST_SRC_EXTERNAL:

            /* then initialize everything */
            resetSystem();

            /* stop handling these cases */
            break;

            /* if the reset was caused by the CPU */
        case PHAROS_CPU_RST_SRC_CPU:

            /* enable ECC detection and triggering */
            pharosPmsrBusEventExportEnable();

            /* no need to initialize the other stuff */
            break;

            /* in case of an oscillator failure */
        case PHAROS_CPU_RST_SRC_OSC:

            /* then initialize everything */
            resetSystem();

            /* announce the error */
            pharosErrorAnnounce(PHAROS_ERROR_CPU_RST_OSC , NULL);

            /* and don't need to initialize everything (already initialized) */
            break;

        case PHAROS_CPU_RST_SRC_WATCHDOG:

            /* announce the error */
            pharosErrorAnnounce(PHAROS_ERROR_CPU_RST_WATCHDOG , NULL);

            /* and don't need to initialize everything (already initialized) */
            break;

        case PHAROS_CPU_RST_SRC_WATCHDOG2:

            /* announce the error */
            pharosErrorAnnounce(PHAROS_ERROR_CPU_RST_WATCHDOG2 , NULL);

            /* and don't need to initialize everything (already initialized) */
            break;

        case PHAROS_CPU_RST_SRC_INTERCONNECT:

            /* then initialize everything */
            resetSystem();

            /* announce the error */
            pharosErrorAnnounce(PHAROS_ERROR_CPU_RST_INTERCON , NULL);

            /* and don't need to initialize everything (already initialized) */
            break;

        case PHAROS_CPU_RST_SRC_SW:

            /* announce the error */
            pharosErrorAnnounce(PHAROS_ERROR_CPU_RST_SW , NULL);

            /* and don't need to initialize everything (already initialized) */
            break;

            /* if no reset */
        default:

            /* then initialize everything */
            resetSystem();

            break;
    }

    /* initialize the cache */
    pharosCpuCacheEnable();

    /* initialize Pharos in single core */
    pharosSingleCoreInitialize();
}


void pharosCpuCoreInitialize(ptrCore core)
{
    /* initialize the interrupt controller with the defined interrupts */
    pharosCpuVimInitialize(&core->interrupts);
}


void outputSChar(char c)
{
    /* write to the SCI port 1 */
    pharosCpuSciWrite((ptrPharosCpuSci) PHAROS_CPU_SCI_REG1 , c);
}


void pharosCpuShutdown(void)
{
    /* interrupt level */
    PharosInterruptLevel level;

    /* disable interrupts so nothing can interrupt the CPU */
    pharosCpuDisableInterrupts(&level);

    /* loop forever */
    for(;;);
}
