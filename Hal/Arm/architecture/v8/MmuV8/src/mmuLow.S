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


.type pharosCpuMair_El1Set,                 %function
.type pharosCpuEnableMmu,                   %function
.type pharosCpuMmuInitializeLow,            %function
.type pharosCpuScrtlEl1Set,                 %function
.type pharosCpuScrtlEl1Get,                 %function
.type pharosCpuMmuInvalidateAddress,        %function

.global pharosCpuMair_El1Set
.global pharosCpuEnableMmu
.global pharosCpuMmuInitializeLow
.global pharosCpuScrtlEl1Set
.global pharosCpuScrtlEl1Get
.global pharosCpuMmuInvalidateAddress


/**
 * Initial value of the TCR_EL1 register
 * bit 38 -> TBI1  = 1 (top byte ignored in the address calculation (no TTBR1_EL1)
 * bit 37 -> TBI0  = 0 (top byte ignored in the address calculation (no TTBR1_EL1)
 * bit 36 -> AS    = 0
 * bit 32 -> IPS   = not initialized here (depends on the physical address size that can be calculated from id_aa64mmfr0_el1)
 * bit 30 -> TG1   = 2 (4KiB transalation) -> for TTBR1_EL1 which we don't use
 * bit 28 -> SH1   = 3 (inner shareable) -> used for TTBR1_EL1 which we don't use
 * bit 26 -> ORGN1 = 1 (normal memory, outer write-back write-allocate) -> used for TTBR1_EL1 which we don't use
 * bit 24 -> IRGN1 = 1 (Normal memory, Inner Write-Back Write-Allocate) -> used for TTBR1_EL1 which we don't use
 * bit 23 -> EPD1  = 1 (don't used TTBR1_EL1)
 * bit 22 -> A1    = 0 (use TTBR0_EL1 to define the ASID)
 * bit 16 -> T1SZ  = 0 (The size offset of the memory region addressed by TTBR1_EL1) -> we don't use TTBR1_EL1
 * bit 14 -> TG0   = 0 (Granule size = 4 KiB) for TTBR0_EL1 which we use!
 * bit 12 -> SH0   = 3 (inner shareable) for TTBR0_EL1
 * bit 10 -> ORGN0 = 1 (Normal memory, Outer Write-Back Write-Allocate Cacheable) for translation table walks of TTBR0_EL1
 * bit  8 -> IRGN0 = 1 (Normal memory, Inner Write-Back Write-Allocate Cacheable) for translation table walks of TTBR0_EL1
 * bit  7 -> EPD0  = 0 (Perform translation table walks using TTBR0)
 * bit  6 -> reserved
 * bit  0 -> T0SZ  = 24 (The size offset of the memory region addressed by TTBR0_EL1) -> size = 2^(64-24) = 2^40 = 1 TiB
 * 
 */
#define TCR_EL1_DEFAULT     ((1 << 38) | (1 << 37) | (2 << 30) | (3 << 28) | (1 << 26) | (1 << 24) | (1 << 23) | \
                             (0 << 22) | (25 << 16) | (0 << 14) | (3 << 12) | (1 << 10) | (1 <<  8) | (0 <<  7) | (24 << 0))


/**
 * void pharosCpuMair_El1Set(uint64_t mair);
 */
.func pharosCpuMair_El1Set
pharosCpuMair_El1Set:

    /* set the MAIR EL1 register */
    msr MAIR_EL1, x0

    /* return from function */
    ret

.endfunc


.func pharosCpuScrtlEl1Set
pharosCpuScrtlEl1Set:

    dsb ish

    isb
    
    /* set the SCTLR_EL1 register */
    msr     SCTLR_EL1, x0

    isb

    /* return from function */
    ret

.endfunc


.func pharosCpuScrtlEl1Get
pharosCpuScrtlEl1Get:

    /* read the SCTLR_EL1 register */
    mrs     x0 , SCTLR_EL1

    /* return from function */
    ret

.endfunc


/**
 * void pharosCpuMmuInvalidateAddress(uint8_t *address);
 */
.func pharosCpuMmuInvalidateAddress
pharosCpuMmuInvalidateAddress:

    /* invalidate everything */
    tlbi  vmalle1
 
    /* return from function */
    ret

.endfunc

/**
 * void pharosCpuMmuInitializeLow(ptrPharosCpuMmuLevel0Table tlb);
 */
.func pharosCpuMmuInitializeLow
pharosCpuMmuInitializeLow:

   
    /* load the default value for the TCR_EL1 register */
    ldr     x2 , =TCR_EL1_DEFAULT

    /* read the id_aa64mmfr0_el1 */
    mrs     x1, id_aa64mmfr0_el1

    /* get the first 4 bytes (PARange) */
    and     x1 , x1 , #0xf

    /* shift left the PARange, will be used as the IPS bits of the TCR_EL1 */
    lsl     x1 , x1 , 32

    /* place the IPS bits for the TCR_EL1 */
    orr     x2 , x1 , x2

    /* initialize the TCR_EL1 */
    msr     tcr_el1, x2
    
    /* make sure previous instructions are performed */
    isb

    add     x0 , x0 ,1

    /* initialize the TTBR0_EL1 */
    msr     ttbr0_el1 , x0

    /* return from function */
    ret

.endfunc
