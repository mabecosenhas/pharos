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


#include <pharos/hal/cpu/system.h>
#include <pharos/hal/cpu/pll.h>

/* read the LPO trim solution value (bits 16 to 31) */
#define LPO_TRIM_VALUE          (((*(volatile uint32_t *)PHAROS_CPU_LPO_TRIM_ADDRESS) & 0xFFFF0000U)>>16U)


#define PLLS_DISABLED           (PHAROS_CPU_SYS1_CSDIS_X_DIS(PHAROS_CPU_SYSTEM_CLOCK_PLL1) & PHAROS_CPU_SYS1_CSDIS_X_DIS(PHAROS_CPU_SYSTEM_CLOCK_PLL2))


void pharosCpuPllInitialize(void)
{
    /* disable PLL1 and PLL2 */
    PHAROS_SYSTEM_PRIMARY->clockSourceDisableSet = PHAROS_CPU_SYS1_CSDISSET_X_DIS(PHAROS_CPU_SYSTEM_CLOCK_PLL1) |
        PHAROS_CPU_SYS1_CSDISSET_X_DIS(PHAROS_CPU_SYSTEM_CLOCK_PLL2);

    /* wait till the PLLs are disabled */
    while((PHAROS_SYSTEM_PRIMARY->clockSourceDisable & PLLS_DISABLED) != PLLS_DISABLED);

    /* clear oscillator fail status, PLL under cycle slip detection and PLL over cycle slip detection */
    PHAROS_SYSTEM_PRIMARY->globalStatus = PHAROS_CPU_SYS1_GLBSTAT_OSCFAIL_CLR |
        PHAROS_CPU_SYS1_GLBSTAT_RFSLIP_CLR |
        PHAROS_CPU_SYS1_GLBSTAT_RBSLIP_CLR;

    /* set the:
     * - PLL multipler to 0x9500 / 256 + 1 = 150
     * - reference clock divider by 8
     * - PLL output divider by 32
     * - PLL slip enable */
    PHAROS_SYSTEM_PRIMARY->pllControl1 = PHAROS_CPU_SYS1_PLLCTL1_BSLIPEN | PHAROS_CPU_SYS1_PLLCTL1_PLLDIV(32U - 1U) |
        PHAROS_CPU_SYS1_PLLCTL1_RCLKDIV(8U - 1U) | PHAROS_CPU_SYS1_PLLCTL1_PLLMUL(0x9500U);

    /* set the:
     * - f_mod to f_int_clk / (2*256) 
     * - multiplier correction 
     * - f_post_odclk = f_vco_clk / 1
     * - spreading amount to 62
     */
    PHAROS_SYSTEM_PRIMARY->pllControl2 = PHAROS_CPU_SYS1_PLLCTL2_SNGRATE(0xffu)
        | PHAROS_CPU_SYS1_PLLCTL2_MULMOD(7U)
        | PHAROS_CPU_SYS1_PLLCTL2_ODPLL(1U - 1U)
        | PHAROS_CPU_SYS1_PLLCTL2_SPR_AMT(62U - 1U);

    /* set the PLL2 to:
     *  - internal output divider to 1
     *  - output divider to 32
     *  - f_intclk2 = foscin / 8
     *  - PLL2 multiplication factor by 0x9500/256 + 1 = 150
     */
    PHAROS_SYSTEM_SECUNDARY->pllControl3 = PHAROS_CPU_SYS2_PLLCTL3_ODPLL2(1U - 1U)
        | PHAROS_CPU_SYS2_PLLCTL3_PLLDIV2(32U - 1U)
        | PHAROS_CPU_SYS2_PLLCTL3_CLKDIV2(8U - 1U)
        | PHAROS_CPU_SYS2_PLLCTL3_PLLMUL2(0x9500U);

    /* enable PLL1 and PLL2 to start up and lock external clock in (EXTCLKIN) and external clock in2 (EXTCLKIN2) */
    PHAROS_SYSTEM_PRIMARY->clockSourceDisable = 0x00000008U | 0x00000080U | 0x00000004U;
}


void pharosCpuLpoInitialize(void)
{
    /* read the LPO trim value */
    uint32_t trim = LPO_TRIM_VALUE;


    /* check if there is an LPO trim to apply */
    if(trim != 0xFFFFU)
    {
        /* if so, then apply the trim */
        PHAROS_SYSTEM_PRIMARY->lpoMonitorControl = PHAROS_CPU_SYS1_LPOMONCTL_BIAS_EN | trim;
    }
    else
    {
        /* set the high trim and low trim to 100% (default) */
        PHAROS_SYSTEM_PRIMARY->lpoMonitorControl = PHAROS_CPU_SYS1_LPOMONCTL_BIAS_EN |
            PHAROS_CPU_SYS1_LPOMONCTL_HFTRIM(16U) |
            PHAROS_CPU_SYS1_LPOMONCTL_LFTRIM(4U);
    }
}


void pharosCpuPllClockInitialize(void)
{
    /* auxiliary variable to hold the value of the clock source disabled */
    uint32_t clockSourceDisable;


    /* setup system clock divider for HCLK (reset value is 0) */
    PHAROS_SYSTEM_SECUNDARY->hclkControl = PHAROS_CPU_SYS2_HCLKCNTL_DIV(1U);

    /* enable VLCKA1 AND VLCK3, disable VLCKA2 */
    PHAROS_SYSTEM_PRIMARY->clockDomainDisable = PHAROS_CPU_SYS1_CDDIS_VCLKA1_EN |
        PHAROS_CPU_SYS1_CDDIS_VCLKA2_DI | PHAROS_CPU_SYS1_CDDIS_VCLKA3_EN;

    /* loop until the clock is ready */
    do
    {
        /* read if the clock is disabled */
        clockSourceDisable = (PHAROS_SYSTEM_PRIMARY->clockSourceDisable ^ 0xffU) & 0xffU;
    }
        /* wait for until clocks are locked */
    while((PHAROS_SYSTEM_PRIMARY->clockSourceValidStatus & clockSourceDisable) != clockSourceDisable);

    /* set the PLL1 to be used as clock source for the HCLK, VCLK and VCLK2 on:
     * - normal operation
     * - after wakeup 
     * - shutdown */
    PHAROS_SYSTEM_PRIMARY->clockSourceConf = PHAROS_CPU_SYS1_GHVSRC_WAKEUP(PHAROS_CPU_SYSTEM_CLOCK_PLL1) |
        PHAROS_CPU_SYS1_GHVSRC_NORMAL(PHAROS_CPU_SYSTEM_CLOCK_PLL1) |
        PHAROS_CPU_SYS1_GHVSRC_PWRDOWN(PHAROS_CPU_SYSTEM_CLOCK_PLL1);

    /* setup RTI clock to 2 divider and the clock source is the VCLK clock */
    /* TODO: check if 1 << 24 make sense */ /* RTI2 divider (Not applicable for lock-step device)  */
    /* TODO: check if PHAROS_CPU_SYSTEM_CLOCK_VCLK << 16U make sense */ /* RTI2 clock source (Not applicable for lock-step device) */
    PHAROS_SYSTEM_PRIMARY->rtiClockSource = (uint32_t) ((uint32_t) 1U << 24U) |
        (uint32_t) ((uint32_t) PHAROS_CPU_SYSTEM_CLOCK_VCLK << 16U) |
        PHAROS_CPU_SYS1_RCLKSRC_DIV(1U) |
        PHAROS_CPU_SYS1_RCLKSRC_SRC(PHAROS_CPU_SYSTEM_CLOCK_VCLK);

    /* set the VCLKA2 and VCLKA1 source to VCLK */
    PHAROS_SYSTEM_PRIMARY->vClockAsynSource = PHAROS_CPU_SYS1_VCLKASRC_SRC2(PHAROS_CPU_SYSTEM_CLOCK_VCLK) |
        PHAROS_CPU_SYS1_VCLKASRC_SRC1(PHAROS_CPU_SYSTEM_CLOCK_VCLK);


    /* change the VCLK1 clock ratio to a divider of 2 */
    PHAROS_SYSTEM_PRIMARY->clockControl = (PHAROS_SYSTEM_PRIMARY->clockControl & PHAROS_CPU_SYS1_CLKCNTL_VCLK1R_MASK) |
        PHAROS_CPU_SYS1_CLKCNTL_VCLK1R(1U);

    /* change the VCLK2 clock ratio to a divider of 2 */
    PHAROS_SYSTEM_PRIMARY->clockControl = (PHAROS_SYSTEM_PRIMARY->clockControl & PHAROS_CPU_SYS1_CLKCNTL_VCLK2R_MASK) |
        PHAROS_CPU_SYS1_CLKCNTL_VCLK2R(1U);

    /* change the VCLK3 clock ratio to a divider of 2 */
    PHAROS_SYSTEM_SECUNDARY->clock2Control = (PHAROS_SYSTEM_SECUNDARY->clock2Control & PHAROS_CPU_SYS2_CLK2CNTRL_VCLK3R_MASK) |
        PHAROS_CPU_SYS2_CLK2CNTRL_VCLK3R(1U);


    /* enable the VCLK4 and VCLK3, set their divider to 1 and set the source to VCLK  */
    PHAROS_SYSTEM_SECUNDARY->peripheralAsynClockConf = PHAROS_CPU_SYS2_VCLKACON1_4DIV_EN |
        PHAROS_CPU_SYS2_VCLKACON1_4DIV(0U) |
        PHAROS_CPU_SYS2_VCLKACON1_4SRC(PHAROS_CPU_SYSTEM_CLOCK_VCLK) |
        PHAROS_CPU_SYS2_VCLKACON1_3DIV(0U) |
        PHAROS_CPU_SYS2_VCLKACON1_3DIV_EN |
        PHAROS_CPU_SYS2_VCLKACON1_3SRC(PHAROS_CPU_SYSTEM_CLOCK_VCLK);

    /* at this point the PLL are locked, so we can:
     *  - increase the speed of its output by setting the divider of the PLL output to 1 
     *  - enable the reset when the PLL slip is detected */
    PHAROS_SYSTEM_PRIMARY->pllControl1 = (PHAROS_SYSTEM_PRIMARY->pllControl1 & PHAROS_CPU_SYS1_PLLCTL1_DIV_MASK) |
        PHAROS_CPU_SYS1_PLLCTL1_DIV(0U) | PHAROS_CPU_SYS1_PLLCTL1_ROS_EN;

    /* increase the speed of the PLL2 output by setting the divider of the output to 1 */
    PHAROS_SYSTEM_SECUNDARY->pllControl3 = (PHAROS_SYSTEM_SECUNDARY->pllControl3 & PHAROS_CPU_SYS1_PLLCTL3_2DIV_MASK) |
        PHAROS_CPU_SYS1_PLLCTL3_DIV(0U);

    /* disable the PLL frequency modulation */
    PHAROS_SYSTEM_PRIMARY->pllControl2 |= PHAROS_CPU_SYS1_PLLCTL2_MOD_DIS;
}
