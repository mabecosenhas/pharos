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


#ifndef PHAROS_PMSA_H
#define	PHAROS_PMSA_H

#ifdef	__cplusplus
extern "C"
{
#endif

#include <pharos/hal/memory/declarations.h>


#define PHAROS_CPU_PMSA_SCTRL_VE_EN                 (1U << 24U)
#define PHAROS_CPU_PMSA_SCTRL_DZ_DIS                (0U << 19U)
#define PHAROS_CPU_PMSA_SCTRL_DZ_MASK               (0xFFF7FFFFU)
#define PHAROS_CPU_PMSA_SCTRL_BR_EN                 (1U << 17U)
#define PHAROS_CPU_PMSA_SCTRL_MPU_EN                (1U <<  0U)


#define PHAROS_CPU_PMSA_RGNR_REGION(x)              ((x) << 0U)



#define PHAROS_CPU_PMSA_DRSR_EN                     (1U << 0U)
#define PHAROS_CPU_PMSA_DRSR_DIS                    (0U << 0U)

#define PHAROS_CPU_PMSA_DRSR_SIZE(x)                (((x)-1U) << 1U)

#define PHAROS_CPU_PMSA_DRSR_SR_EN_ALL              (0x00U << 8U)




#define PHAROS_CPU_PMSA_DRBAR_ADDR(x)               ((x) << 0U)

#define PHAROS_CPU_PMSA_DRACR_EX_EN                 (0U << 12U)
#define PHAROS_CPU_PMSA_DRACR_EX_DIS                (1U << 12U)

#define PHAROS_CPU_PMSA_DRACR_AP_FULL               (3U << 8U)
#define PHAROS_CPU_PMSA_DRACR_AP_RONLY              (6U << 8U)

#define PHAROS_CPU_PMSA_DRACR_TEX_NOR               (1U << 3U)
#define PHAROS_CPU_PMSA_DRACR_S_NOR                 (0U << 2U)
#define PHAROS_CPU_PMSA_DRACR_C_NOR                 (1U << 1U)
#define PHAROS_CPU_PMSA_DRACR_B_NOR                 (1U << 0U)

#define PHAROS_CPU_PMSA_DRACR_TEX_DEV               (2U << 3U)
#define PHAROS_CPU_PMSA_DRACR_S_DEV                 (0U << 2U)
#define PHAROS_CPU_PMSA_DRACR_C_DEV                 (0U << 1U)
#define PHAROS_CPU_PMSA_DRACR_B_DEV                 (0U << 0U)

#define PHAROS_CPU_PMSA_DRACR_TEX(x)                ((x) << 3U)
#define PHAROS_CPU_PMSA_DRACR_S(x)                  ((x) << 2U)
#define PHAROS_CPU_PMSA_DRACR_C(x)                  ((x) << 1U)
#define PHAROS_CPU_PMSA_DRACR_B(x)                  ((x) << 0U)

#define PHAROS_IO_MEM_DIRECT_TEX_NORMAL             (0x1U)
#define PHAROS_IO_MEM_DIRECT_S_NORMAL               (0x0U)
#define PHAROS_IO_MEM_DIRECT_B_NORMAL               (0x1U)
#define PHAROS_IO_MEM_DIRECT_C_NORMAL               (0x1U)    

#define PHAROS_IO_MEM_DIRECT_TEX_DEVICE             (0x2U)
#define PHAROS_IO_MEM_DIRECT_S_DEVICE               (0x0U)
#define PHAROS_IO_MEM_DIRECT_B_DEVICE               (0x0U)
#define PHAROS_IO_MEM_DIRECT_C_DEVICE               (0x0U)    


    /**
     * Gets the PMSA SCTLR register
     * 
     * @return returns the content of the PMSA SCTLR register
     */
    uint32_t pharosCpuPmsaSctlrGet(void);


    /**
     * Sets the PMSA SCTLR register
     * 
     * @param sctlr new SCTLR value to place
     */
    void pharosCpuPmsaSctlrSet(uint32_t sctlr);


    /**
     * Sets the PMSA RGNR register
     * 
     * @param rgnr new RGNR value to place
     */
    void pharosCpuPmsaRgnrSet(uint32_t rgnr);

    uint32_t pharosCpuPmsaDrbarGet(void);

    /**
     * Sets the PMSA DRBAR register
     * 
     * @param drbar new DRBAR value to place
     */
    void pharosCpuPmsaDrbarSet(uint32_t drbar);


    /**
     * Gets the PMSA DFAR register
     * 
     * @return returns the content of the PMSA DFAR register
     */
    uint32_t pharosCpuPmsaDfarGet(void);


    /**
     * Sets the PMSA DRSR register
     * 
     * @param drsr new DRSR value to place
     */
    void pharosCpuPmsaDrsrSet(uint32_t drsr);

    uint32_t pharosCpuPmsaDrsrGet(void);


    /**
     * Sets the PMSA DRACR register
     * 
     * @param dracr new DRACR value to place
     */
    void pharosCpuPmsaDracrSet(uint32_t dracr);


    /**
     * Gets the PMSA DRACR register
     * 
     * @return returns the content of the PMSA DRACR register
     */
    uint32_t pharosCpuPmsaDracrGet(void);


    /**
     * Performs a low level DMB instruction 
     * 
     * @return 
     */
    void pharosCpuDmb(void);

    void pharosCpuIsb(void);

    void pharosCpuDsb(void);

#ifdef	__cplusplus
}
#endif

#endif	/* PMSA_H */
