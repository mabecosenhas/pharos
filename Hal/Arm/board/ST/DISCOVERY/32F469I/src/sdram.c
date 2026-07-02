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


#include <pharos/hal/cpu/sdram.h>
#include <pharos/hal/cpu/fmc.h>
#include <pharos/hal/cpu/rcc.h>
#include <pharos/hal/cpu/gpio.h>
#include <pharos/hal/boardApi.h>


/* SDRAM specific mode configuration */
#define PHAROS_CPU_SDRAM_MODE_BURST_LENGTH_1                ((uint16_t)0x0000)
#define PHAROS_CPU_SDRAM_MODE_BURST_LENGTH_2                ((uint16_t)0x0001)
#define PHAROS_CPU_SDRAM_MODE_BURST_LENGTH_4                ((uint16_t)0x0002)
#define PHAROS_CPU_SDRAM_MODE_BURST_LENGTH_8                ((uint16_t)0x0004)
#define PHAROS_CPU_SDRAM_MODE_BURST_TYPE_SEQUENTIAL         ((uint16_t)0x0000)
#define PHAROS_CPU_SDRAM_MODE_BURST_TYPE_INTERLEAVED        ((uint16_t)0x0008)
#define PHAROS_CPU_SDRAM_MODE_CAS_LATENCY_2                 ((uint16_t)0x0020)
#define PHAROS_CPU_SDRAM_MODE_CAS_LATENCY_3                 ((uint16_t)0x0030)
#define PHAROS_CPU_SDRAM_MODE_OPERATING_MODE_STANDARD       ((uint16_t)0x0000)
#define PHAROS_CPU_SDRAM_MODE_WRITEBURST_MODE_PROGRAMMED    ((uint16_t)0x0000)
#define PHAROS_CPU_SDRAM_MODE_WRITEBURST_MODE_SINGLE        ((uint16_t)0x0200)


PHAROS_OPTIMIZE_O0 static void delay(uint32_t d)
{
    /* decrease the delay until it reaches 0 */
    for(; d > 0; d--);
}


void pharosCpuInitSdram()
{
    uint32_t aux;

    /* configure PC0 to SDNWE */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOC , PHAROS_CPU_GPIO_PIN_0 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);


    /* configure PD0 to D2 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOD , PHAROS_CPU_GPIO_PIN_0 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PD1 to D3 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOD , PHAROS_CPU_GPIO_PIN_1 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PD2 to D13 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOD , PHAROS_CPU_GPIO_PIN_8 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PD9 to D14 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOD , PHAROS_CPU_GPIO_PIN_9 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PD10 to D15 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOD , PHAROS_CPU_GPIO_PIN_10 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PD14 to D0 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOD , PHAROS_CPU_GPIO_PIN_14 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PD15 to D1 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOD , PHAROS_CPU_GPIO_PIN_15 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);


    /* configure PE0 to NBL0 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOE , PHAROS_CPU_GPIO_PIN_0 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PE1 to NBL1 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOE , PHAROS_CPU_GPIO_PIN_1 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PE7 to D4 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOE , PHAROS_CPU_GPIO_PIN_7 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PE8 to D4 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOE , PHAROS_CPU_GPIO_PIN_8 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PE9 to D4 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOE , PHAROS_CPU_GPIO_PIN_9 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PE10 to D4 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOE , PHAROS_CPU_GPIO_PIN_10 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PE11 to D4 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOE , PHAROS_CPU_GPIO_PIN_11 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PE12 to D4 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOE , PHAROS_CPU_GPIO_PIN_12 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PE13 to D4 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOE , PHAROS_CPU_GPIO_PIN_13 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PE14 to D4 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOE , PHAROS_CPU_GPIO_PIN_14 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PE15 to D4 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOE , PHAROS_CPU_GPIO_PIN_15 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);


    /* configure PF0 to A0 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOF , PHAROS_CPU_GPIO_PIN_0 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PF1 to A1 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOF , PHAROS_CPU_GPIO_PIN_1 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PF2 to A2 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOF , PHAROS_CPU_GPIO_PIN_2 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PF3 to A3 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOF , PHAROS_CPU_GPIO_PIN_3 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PF4 to A4 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOF , PHAROS_CPU_GPIO_PIN_4 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PF5 to A5 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOF , PHAROS_CPU_GPIO_PIN_5 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PF11 to SDNRAS */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOF , PHAROS_CPU_GPIO_PIN_11 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PF12 to A6 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOF , PHAROS_CPU_GPIO_PIN_12 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PF13 to A7 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOF , PHAROS_CPU_GPIO_PIN_13 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PF14 to A8 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOF , PHAROS_CPU_GPIO_PIN_14 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PF15 to A9 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOF , PHAROS_CPU_GPIO_PIN_15 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);


    /* configure PG0 to A10 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOG , PHAROS_CPU_GPIO_PIN_0 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PG1 to A11 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOG , PHAROS_CPU_GPIO_PIN_1 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PG4 to A14 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOG , PHAROS_CPU_GPIO_PIN_4 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PG5 to A15 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOG , PHAROS_CPU_GPIO_PIN_5 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PG8 to SDCLK */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOG , PHAROS_CPU_GPIO_PIN_8 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PG15 to SDNCAS */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOG , PHAROS_CPU_GPIO_PIN_15 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);


    /* configure PH2 to SDCKE0 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOH , PHAROS_CPU_GPIO_PIN_2 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PH3 to SDNE0 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOH , PHAROS_CPU_GPIO_PIN_3 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PH8 to D16 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOH , PHAROS_CPU_GPIO_PIN_8 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PH9 to D17 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOH , PHAROS_CPU_GPIO_PIN_9 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PH10 to D18 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOH , PHAROS_CPU_GPIO_PIN_10 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PH11 to D19 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOH , PHAROS_CPU_GPIO_PIN_11 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PH12 to D20 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOH , PHAROS_CPU_GPIO_PIN_12 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PH13 to D21 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOH , PHAROS_CPU_GPIO_PIN_13 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PH14 to D22 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOH , PHAROS_CPU_GPIO_PIN_14 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PH15 to D23 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOH , PHAROS_CPU_GPIO_PIN_15 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);


    /* configure PI0 to D24 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOI , PHAROS_CPU_GPIO_PIN_0 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PI1 to D25 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOI , PHAROS_CPU_GPIO_PIN_1 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PI2 to D26 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOI , PHAROS_CPU_GPIO_PIN_2 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PI3 to D27 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOI , PHAROS_CPU_GPIO_PIN_3 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PI4 to NBL2 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOI , PHAROS_CPU_GPIO_PIN_4 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PI5 to NBL3 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOI , PHAROS_CPU_GPIO_PIN_5 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PI6 to D28 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOI , PHAROS_CPU_GPIO_PIN_6 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PI7 to D29 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOI , PHAROS_CPU_GPIO_PIN_7 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PI9 to D30 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOI , PHAROS_CPU_GPIO_PIN_9 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* configure PI10 to D31 */
    pharosCpuGpioConfAlt(PHAROS_CPU_GPIOI , PHAROS_CPU_GPIO_PIN_10 , PHAROS_CPU_GPIO_AF12 , PHAROS_CPU_GPIO_SPEED_VERY_HIGH , PHAROS_CPU_GPIO_OUTPUT_TYPE_PP);

    /* enable all the peripheral clocks required for the SDRAM */
    PHAROS_CPU_RCC->ahb1PeripheralClockEnable |= PHAROS_CPU_RCC_AHB1ENR_GPIOC_EN |
            PHAROS_CPU_RCC_AHB1ENR_GPIOD_EN | PHAROS_CPU_RCC_AHB1ENR_GPIOE_EN | PHAROS_CPU_RCC_AHB1ENR_GPIOF_EN |
            PHAROS_CPU_RCC_AHB1ENR_GPIOG_EN | PHAROS_CPU_RCC_AHB1ENR_GPIOH_EN | PHAROS_CPU_RCC_AHB1ENR_GPIOI_EN;

    /* enable the SDRAM clock on AHB3 */
    PHAROS_CPU_RCC->ahb3PeripheralClockEnable |= PHAROS_CPU_RCC_AHB3ENR_FMCEN_EN;

    /* reset the SDRAM bank 1 */
    aux = PHAROS_CPU_FMC->sdramControl[0];

    /* clear any configurable bits (leave only the reserved bits) */
    aux &= PHAROS_CPU_FMC_SDCR_CLEAR;

    /* set 8 columns, 
     * 12 rows, 
     * 32 bit data, 
     * 4 internal banks, 
     * CAS = 3, 
     * enable writes, 
     * divide the clock by 2 (180/2 = 90) , 
     * enable bursts and no pipe delay */
    aux |= PHAROS_CPU_FMC_SDCR_NC_8 | PHAROS_CPU_FMC_SDCR_NR_12 | PHAROS_CPU_FMC_SDCR_MWID_32 | PHAROS_CPU_FMC_SDCR_NB_4 | PHAROS_CPU_FMC_SDCR_CAS_3 |
            PHAROS_CPU_FMC_SDCR_WP_DIS | PHAROS_CPU_FMC_SDCR_SDCLK_2 | PHAROS_CPU_FMC_SDCR_RBURST_EN | PHAROS_CPU_FMC_SDCR_RPIPE_0;

    /* set the SDRAM SDCR register */
    PHAROS_CPU_FMC->sdramControl[0] = aux;

    /* timing init */
    aux = PHAROS_CPU_FMC->sdramTiming[0];

    /* clear all the bits (except for the reserved bits) */
    aux &= ~(0x0FFFFFFFU);

    /* set the SDRAM timing set */
    aux |= PHAROS_CPU_FMC_SDTR_TMRD_2 | PHAROS_CPU_FMC_SDTR_TXSR_7 | PHAROS_CPU_FMC_SDTR_TRAS_4 |
            PHAROS_CPU_FMC_SDTR_TRC_7 | PHAROS_CPU_FMC_SDTR_TWR_3 | PHAROS_CPU_FMC_SDTR_TRP_2 | PHAROS_CPU_FMC_SDTR_TRCD_2;

    /* set the SDRAM SDTR register */
    PHAROS_CPU_FMC->sdramTiming[0] = aux;

    /* send the clock enable command */
    pharosCpuSdramClockEnable();

    /* must have a delay of at least 100 us */
    delay(1000);

    /* send the PALL command */
    pharosCpuSdramPall();

    /* send the auto-refresh */
    pharosCpuSdramAutorefresh(8);

    /* send the load command */
    pharosCpuSdramLoadCommand(PHAROS_CPU_SDRAM_MODE_BURST_LENGTH_1 | PHAROS_CPU_SDRAM_MODE_BURST_TYPE_SEQUENTIAL |
                              PHAROS_CPU_SDRAM_MODE_CAS_LATENCY_3 | PHAROS_CPU_SDRAM_MODE_OPERATING_MODE_STANDARD |
                              PHAROS_CPU_SDRAM_MODE_WRITEBURST_MODE_SINGLE);

    /* set the refresh time to 90 MHz (the CPU FREQ divided by 2 is the SDRAM clock, and the SDRAM requires 64 ms refresh per each of the 2^12 rows */
    PHAROS_CPU_FMC->sdramRefreshTimer = PHAROS_CPU_FMC_SDRTR_COUNT(((PHAROS_CPU_FREQ_MHZ / 2U) * 64000U) / 4096U - 20U);
}
