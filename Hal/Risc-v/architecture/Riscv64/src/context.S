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


#include <pharos/hal/cpu/asm.h>
#include <pharos/hal/cpu/conf.h>


.type pharosCpuContextSwitch,                           %function 
.type pharosCpuRestoreHeir,                      %function

.global pharosCpuContextSwitch
.global pharosCpuRestoreHeir
 

/**
 * void pharosCpuContextSwitch(ptrThreadCpuContext current , ptrThreadCpuContext heir);
 * x0 = current
 * x1 = heir
 */
.func pharosCpuContextSwitch
pharosCpuContextSwitch:

    /***************************************************/
    /* Save all "caller saver" registers to TCB        */
    /***************************************************/

    /* read the current SATP */
    csrr    a2 , satp

    sd      ra  , PHAROS_CPU_CONTEXT_RA_OFFSET  (a0)
    sd      sp  , PHAROS_CPU_CONTEXT_SP_OFFSET  (a0)
    sd      t0  , PHAROS_CPU_CONTEXT_T0_OFFSET  (a0)
    sd      t1  , PHAROS_CPU_CONTEXT_T1_OFFSET  (a0)
    sd      t2  , PHAROS_CPU_CONTEXT_T2_OFFSET  (a0)
    sd      t3  , PHAROS_CPU_CONTEXT_T3_OFFSET  (a0)
    sd      t4  , PHAROS_CPU_CONTEXT_T4_OFFSET  (a0)
    sd      t5  , PHAROS_CPU_CONTEXT_T5_OFFSET  (a0)
    sd      t6  , PHAROS_CPU_CONTEXT_T6_OFFSET  (a0)
    sd      s0  , PHAROS_CPU_CONTEXT_S0_OFFSET  (a0)
    sd      s1  , PHAROS_CPU_CONTEXT_S1_OFFSET  (a0)
    sd      s2  , PHAROS_CPU_CONTEXT_S2_OFFSET  (a0)
    sd      s3  , PHAROS_CPU_CONTEXT_S3_OFFSET  (a0)
    sd      s4  , PHAROS_CPU_CONTEXT_S4_OFFSET  (a0)
    sd      s5  , PHAROS_CPU_CONTEXT_S5_OFFSET  (a0)
    sd      s6  , PHAROS_CPU_CONTEXT_S6_OFFSET  (a0)
    sd      s7  , PHAROS_CPU_CONTEXT_S7_OFFSET  (a0)
    sd      s8  , PHAROS_CPU_CONTEXT_S8_OFFSET  (a0)
    sd      s9  , PHAROS_CPU_CONTEXT_S9_OFFSET  (a0)
    sd      s10 , PHAROS_CPU_CONTEXT_S10_OFFSET (a0)
    sd      s11 , PHAROS_CPU_CONTEXT_S11_OFFSET (a0)
    
    /* store the current satp register */
    sd      a2 , PHAROS_CPU_CONTEXT_SATP_OFFSET (a0)

    /* if the CPU has floating point registers (double precision floating point) */
#ifdef PHAROS_CPU_FP

    /* reads the fcsr register to a3 */
    frcsr   a3 

    fsd     f0  , PHAROS_CPU_CONTEXT_F0_OFFSET  (a0)
    fsd     f1  , PHAROS_CPU_CONTEXT_F1_OFFSET  (a0)
    fsd     f2  , PHAROS_CPU_CONTEXT_F2_OFFSET  (a0)
    fsd     f3  , PHAROS_CPU_CONTEXT_F3_OFFSET  (a0)
    fsd     f4  , PHAROS_CPU_CONTEXT_F4_OFFSET  (a0)
    fsd     f5  , PHAROS_CPU_CONTEXT_F5_OFFSET  (a0)
    fsd     f6  , PHAROS_CPU_CONTEXT_F6_OFFSET  (a0)
    fsd     f7  , PHAROS_CPU_CONTEXT_F7_OFFSET  (a0)
    fsd     f8  , PHAROS_CPU_CONTEXT_F8_OFFSET  (a0)
    fsd     f9  , PHAROS_CPU_CONTEXT_F9_OFFSET  (a0)
    fsd     f10 , PHAROS_CPU_CONTEXT_F10_OFFSET (a0)
    fsd     f11 , PHAROS_CPU_CONTEXT_F11_OFFSET (a0)
    fsd     f12 , PHAROS_CPU_CONTEXT_F12_OFFSET (a0)
    fsd     f13 , PHAROS_CPU_CONTEXT_F13_OFFSET (a0)
    fsd     f14 , PHAROS_CPU_CONTEXT_F14_OFFSET (a0)
    fsd     f15 , PHAROS_CPU_CONTEXT_F15_OFFSET (a0)
    fsd     f16 , PHAROS_CPU_CONTEXT_F16_OFFSET (a0)
    fsd     f17 , PHAROS_CPU_CONTEXT_F17_OFFSET (a0)
    fsd     f18 , PHAROS_CPU_CONTEXT_F18_OFFSET (a0)
    fsd     f19 , PHAROS_CPU_CONTEXT_F19_OFFSET (a0)
    fsd     f20 , PHAROS_CPU_CONTEXT_F20_OFFSET (a0)
    fsd     f21 , PHAROS_CPU_CONTEXT_F21_OFFSET (a0)
    fsd     f22 , PHAROS_CPU_CONTEXT_F22_OFFSET (a0)
    fsd     f23 , PHAROS_CPU_CONTEXT_F23_OFFSET (a0)
    fsd     f24 , PHAROS_CPU_CONTEXT_F24_OFFSET (a0)
    fsd     f25 , PHAROS_CPU_CONTEXT_F25_OFFSET (a0)
    fsd     f26 , PHAROS_CPU_CONTEXT_F26_OFFSET (a0)
    fsd     f27 , PHAROS_CPU_CONTEXT_F27_OFFSET (a0)
    fsd     f28 , PHAROS_CPU_CONTEXT_F28_OFFSET (a0)
    fsd     f29 , PHAROS_CPU_CONTEXT_F29_OFFSET (a0)
    fsd     f30 , PHAROS_CPU_CONTEXT_F30_OFFSET (a0)
    fsd     f31 , PHAROS_CPU_CONTEXT_F31_OFFSET (a0)
    sd      a3  , PHAROS_CPU_CONTEXT_FCSR_OFFSET (a0)
 
#endif

    /* read mstatus to t0  */
    csrr    t0 , mstatus

    /* store mstatus */
    sd      t0 , PHAROS_CPU_CONTEXT_MSTATUS_OFFSET (a0)

    /***************************************************/
    /* Restore all "caller saver" registers from TCB   */
    /***************************************************/


    /* if the CPU has floating point registers (double precision floating point) */
#ifdef PHAROS_CPU_FP

    /* load the FPU registers */
    ld      a2  , PHAROS_CPU_CONTEXT_FCSR_OFFSET (a1)
    fld     f0  , PHAROS_CPU_CONTEXT_F0_OFFSET  (a1)
    fld     f1  , PHAROS_CPU_CONTEXT_F1_OFFSET  (a1)
    fld     f2  , PHAROS_CPU_CONTEXT_F2_OFFSET  (a1)
    fld     f3  , PHAROS_CPU_CONTEXT_F3_OFFSET  (a1)
    fld     f4  , PHAROS_CPU_CONTEXT_F4_OFFSET  (a1)
    fld     f5  , PHAROS_CPU_CONTEXT_F5_OFFSET  (a1)
    fld     f6  , PHAROS_CPU_CONTEXT_F6_OFFSET  (a1)
    fld     f7  , PHAROS_CPU_CONTEXT_F7_OFFSET  (a1)
    fld     f8  , PHAROS_CPU_CONTEXT_F8_OFFSET  (a1)
    fld     f9  , PHAROS_CPU_CONTEXT_F9_OFFSET  (a1)
    fld     f10 , PHAROS_CPU_CONTEXT_F10_OFFSET (a1)
    fld     f11 , PHAROS_CPU_CONTEXT_F11_OFFSET (a1)
    fld     f12 , PHAROS_CPU_CONTEXT_F12_OFFSET (a1)
    fld     f13 , PHAROS_CPU_CONTEXT_F13_OFFSET (a1)
    fld     f14 , PHAROS_CPU_CONTEXT_F14_OFFSET (a1)
    fld     f15 , PHAROS_CPU_CONTEXT_F15_OFFSET (a1)
    fld     f16 , PHAROS_CPU_CONTEXT_F16_OFFSET (a1)
    fld     f17 , PHAROS_CPU_CONTEXT_F17_OFFSET (a1)
    fld     f18 , PHAROS_CPU_CONTEXT_F18_OFFSET (a1)
    fld     f19 , PHAROS_CPU_CONTEXT_F19_OFFSET (a1)
    fld     f20 , PHAROS_CPU_CONTEXT_F20_OFFSET (a1)
    fld     f21 , PHAROS_CPU_CONTEXT_F21_OFFSET (a1)
    fld     f22 , PHAROS_CPU_CONTEXT_F22_OFFSET (a1)
    fld     f23 , PHAROS_CPU_CONTEXT_F23_OFFSET (a1)
    fld     f24 , PHAROS_CPU_CONTEXT_F24_OFFSET (a1)
    fld     f25 , PHAROS_CPU_CONTEXT_F25_OFFSET (a1)
    fld     f26 , PHAROS_CPU_CONTEXT_F26_OFFSET (a1)
    fld     f27 , PHAROS_CPU_CONTEXT_F27_OFFSET (a1)
    fld     f28 , PHAROS_CPU_CONTEXT_F28_OFFSET (a1)
    fld     f29 , PHAROS_CPU_CONTEXT_F29_OFFSET (a1)
    fld     f30 , PHAROS_CPU_CONTEXT_F30_OFFSET (a1)
    fld     f31 , PHAROS_CPU_CONTEXT_F31_OFFSET (a1)

    /* write a2 to the fcsr */
    fscsr   a2
#endif

    /* load remaining registers */
    
    ld      ra  , PHAROS_CPU_CONTEXT_RA_OFFSET  (a1)
    ld      sp  , PHAROS_CPU_CONTEXT_SP_OFFSET  (a1)
    ld      t0  , PHAROS_CPU_CONTEXT_T0_OFFSET  (a1)
    ld      t1  , PHAROS_CPU_CONTEXT_T1_OFFSET  (a1)
    ld      t2  , PHAROS_CPU_CONTEXT_T2_OFFSET  (a1)
    ld      t3  , PHAROS_CPU_CONTEXT_T3_OFFSET  (a1)
    ld      t4  , PHAROS_CPU_CONTEXT_T4_OFFSET  (a1)
    ld      t5  , PHAROS_CPU_CONTEXT_T5_OFFSET  (a1)
    ld      t6  , PHAROS_CPU_CONTEXT_T6_OFFSET  (a1)
    ld      s0  , PHAROS_CPU_CONTEXT_S0_OFFSET  (a1)
    ld      s1  , PHAROS_CPU_CONTEXT_S1_OFFSET  (a1)
    ld      s2  , PHAROS_CPU_CONTEXT_S2_OFFSET  (a1)
    ld      s3  , PHAROS_CPU_CONTEXT_S3_OFFSET  (a1)
    ld      s4  , PHAROS_CPU_CONTEXT_S4_OFFSET  (a1)
    ld      s5  , PHAROS_CPU_CONTEXT_S5_OFFSET  (a1)
    ld      s6  , PHAROS_CPU_CONTEXT_S6_OFFSET  (a1)
    ld      s7  , PHAROS_CPU_CONTEXT_S7_OFFSET  (a1)
    ld      s8  , PHAROS_CPU_CONTEXT_S8_OFFSET  (a1)
    ld      s9  , PHAROS_CPU_CONTEXT_S9_OFFSET  (a1)
    ld      s10 , PHAROS_CPU_CONTEXT_S10_OFFSET (a1)
    ld      s11 , PHAROS_CPU_CONTEXT_S11_OFFSET (a1)
    
    /* get the value to place on the satp for memory protection */
    ld      a2 , PHAROS_CPU_CONTEXT_SATP_OFFSET (a1)

    /* write the SATP */
    csrw    satp , a2

    /* load heir thread mstatus */
    ld      t0 , PHAROS_CPU_CONTEXT_MSTATUS_OFFSET (a1)

    /* set the mstatus */
    csrw    mstatus , t0
    
    /* return to the heir thread */
    ret

.endfunc


/**
 * void pharosCpuRestoreHeir(ptrThreadCpuContext heir);
 * x0 = heir
 */
.func pharosCpuRestoreHeir
pharosCpuRestoreHeir:


    /* load registers */
    ld      ra  , PHAROS_CPU_CONTEXT_RA_OFFSET  (a0)
    ld      sp  , PHAROS_CPU_CONTEXT_SP_OFFSET  (a0)
    ld      t0  , PHAROS_CPU_CONTEXT_T0_OFFSET  (a0)
    ld      t1  , PHAROS_CPU_CONTEXT_T1_OFFSET  (a0)
    ld      t2  , PHAROS_CPU_CONTEXT_T2_OFFSET  (a0)
    ld      t3  , PHAROS_CPU_CONTEXT_T3_OFFSET  (a0)
    ld      t4  , PHAROS_CPU_CONTEXT_T4_OFFSET  (a0)
    ld      t5  , PHAROS_CPU_CONTEXT_T5_OFFSET  (a0)
    ld      t6  , PHAROS_CPU_CONTEXT_T6_OFFSET  (a0)
    ld      s0  , PHAROS_CPU_CONTEXT_S0_OFFSET  (a0)
    ld      s1  , PHAROS_CPU_CONTEXT_S1_OFFSET  (a0)
    ld      s2  , PHAROS_CPU_CONTEXT_S2_OFFSET  (a0)
    ld      s3  , PHAROS_CPU_CONTEXT_S3_OFFSET  (a0)
    ld      s4  , PHAROS_CPU_CONTEXT_S4_OFFSET  (a0)
    ld      s5  , PHAROS_CPU_CONTEXT_S5_OFFSET  (a0)
    ld      s6  , PHAROS_CPU_CONTEXT_S6_OFFSET  (a0)
    ld      s7  , PHAROS_CPU_CONTEXT_S7_OFFSET  (a0)
    ld      s8  , PHAROS_CPU_CONTEXT_S8_OFFSET  (a0)
    ld      s9  , PHAROS_CPU_CONTEXT_S9_OFFSET  (a0)
    ld      s10 , PHAROS_CPU_CONTEXT_S10_OFFSET (a0)
    ld      s11 , PHAROS_CPU_CONTEXT_S11_OFFSET (a0)
    
#ifdef PHAROS_CPU_FP
    fld     f0  , PHAROS_CPU_CONTEXT_F0_OFFSET   (a0)
    fld     f1  , PHAROS_CPU_CONTEXT_F1_OFFSET   (a0)
    fld     f2  , PHAROS_CPU_CONTEXT_F2_OFFSET   (a0)
    fld     f3  , PHAROS_CPU_CONTEXT_F3_OFFSET   (a0)
    fld     f4  , PHAROS_CPU_CONTEXT_F4_OFFSET   (a0)
    fld     f5  , PHAROS_CPU_CONTEXT_F5_OFFSET   (a0)
    fld     f6  , PHAROS_CPU_CONTEXT_F6_OFFSET   (a0)
    fld     f7  , PHAROS_CPU_CONTEXT_F7_OFFSET   (a0)
    fld     f8  , PHAROS_CPU_CONTEXT_F8_OFFSET   (a0)
    fld     f9  , PHAROS_CPU_CONTEXT_F9_OFFSET   (a0)
    fld     f10 , PHAROS_CPU_CONTEXT_F10_OFFSET  (a0)
    fld     f11 , PHAROS_CPU_CONTEXT_F11_OFFSET  (a0)
    fld     f12 , PHAROS_CPU_CONTEXT_F12_OFFSET  (a0)
    fld     f13 , PHAROS_CPU_CONTEXT_F13_OFFSET  (a0)
    fld     f14 , PHAROS_CPU_CONTEXT_F14_OFFSET  (a0)
    fld     f15 , PHAROS_CPU_CONTEXT_F15_OFFSET  (a0)
    fld     f16 , PHAROS_CPU_CONTEXT_F16_OFFSET  (a0)
    fld     f17 , PHAROS_CPU_CONTEXT_F17_OFFSET  (a0)
    fld     f18 , PHAROS_CPU_CONTEXT_F18_OFFSET  (a0)
    fld     f19 , PHAROS_CPU_CONTEXT_F19_OFFSET  (a0)
    fld     f20 , PHAROS_CPU_CONTEXT_F20_OFFSET  (a0)
    fld     f21 , PHAROS_CPU_CONTEXT_F21_OFFSET  (a0)
    fld     f22 , PHAROS_CPU_CONTEXT_F22_OFFSET  (a0)
    fld     f23 , PHAROS_CPU_CONTEXT_F23_OFFSET  (a0)
    fld     f24 , PHAROS_CPU_CONTEXT_F24_OFFSET  (a0)
    fld     f25 , PHAROS_CPU_CONTEXT_F25_OFFSET  (a0)
    fld     f26 , PHAROS_CPU_CONTEXT_F26_OFFSET  (a0)
    fld     f27 , PHAROS_CPU_CONTEXT_F27_OFFSET  (a0)
    fld     f28 , PHAROS_CPU_CONTEXT_F28_OFFSET  (a0)
    fld     f29 , PHAROS_CPU_CONTEXT_F29_OFFSET  (a0)
    fld     f30 , PHAROS_CPU_CONTEXT_F30_OFFSET  (a0)
    fld     f31 , PHAROS_CPU_CONTEXT_F31_OFFSET  (a0)
    ld      a2  , PHAROS_CPU_CONTEXT_FCSR_OFFSET (a0)

    /* write a2 to the fcsr */
    fscsr   a2

#endif
    
    /* get the value to place on the satp for memory protection */
    ld      a2 , PHAROS_CPU_CONTEXT_SATP_OFFSET (a0)
 
    /* write the SATP */
    csrw    satp , a2

    /* load heir thread mstatus */
    ld      t0 , PHAROS_CPU_CONTEXT_MSTATUS_OFFSET (a0)

    /* set the mstatus (this will enable interrupts so it must be the last thing we do) */
    csrs   mstatus , t0

    fence

    /* return to the heir thread (maintaining in machine mode, the entry point will switch to user mode) */
    ret 

.endfunc
