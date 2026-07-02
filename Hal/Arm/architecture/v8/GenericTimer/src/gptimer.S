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


.type pharosCpuCntFrqEl0Set,                        %function 
.type pharosCpuCntvOffsetSet,                       %function 
.type pharosCpuCntpCtlEl0Set,                       %function 
.type pharosCpuCntpCvalEl0Set,                      %function
.type pharosCpuCntpTvalEl0Set,                      %function
.type pharosCpuCntpCtEl0Get,                        %function
.type pharosCpuCntpCtlEl0Get,                       %function
.type pharosCpuCntpTvalEl0Get,                      %function
.type pharosCpuCntpCvalEl0Get,                      %function
.type pharosCpuCntvCvalEl0Set,                      %function
.type pharosCpuCntvCtlEl0Set,                       %function
.type pharosCpuCntFreqGet,                          %function
.type pharosCpuCntvTvalEl0Set,                      %function
.type pharosCpuCnthpCtlEl2Set,                      %function
.type pharosCpuCntpsCvalEl1Set,                     %function
.type pharosCpuCnthpTvalEl2Set,                     %function
.type pharosCpuCnthpTvalEl2Get,                     %function
.type pharosCpuCnthpCtlEl2Get,                      %function

.global pharosCpuCntFrqEl0Set
.global pharosCpuCntvOffsetSet
.global pharosCpuCntpCtlEl0Set
.global pharosCpuCntpCvalEl0Set
.global pharosCpuCntpTvalEl0Set
.global pharosCpuCntpCtEl0Get
.global pharosCpuCntpCtlEl0Get
.global pharosCpuCntpTvalEl0Get
.global pharosCpuCntpCvalEl0Get
.global pharosCpuCntvCvalEl0Set
.global pharosCpuCntvCtlEl0Set
.global pharosCpuCntFreqGet
.global pharosCpuCntvTvalEl0Set
.global pharosCpuCnthpCtlEl2Set
.global pharosCpuCntpsCvalEl1Set
.global pharosCpuCnthpTvalEl2Set
.global pharosCpuCnthpTvalEl2Get
.global pharosCpuCnthpCtlEl2Get


.section .text

/**
 * void pharosCpuCntFrqEl0Set(uint32_t value);
 */
.func pharosCpuCntFrqEl0Set
pharosCpuCntFrqEl0Set:

    /* set the CNTFRQ_EL0 register */
    //msr     CNTFRQ_EL0 , x0

    /* return from function */
    ret 


.endfunc


.func pharosCpuCntkCtlEl1Set
pharosCpuCntkCtlEl1Set:

    msr     CNTKCTL_EL1 , x0

    /* return from function */
    ret 
.endfunc 


.func pharosCpuCntFreqGet
pharosCpuCntFreqGet:

    mrs     x0 , CNTFRQ_EL0

    /* return from function */
    ret 
.endfunc 


/**
 * void pharosCpuCntvOffsetSet(uint32_t value);
 */
.func pharosCpuCntvOffsetSet
pharosCpuCntvOffsetSet:

    /* set the CNTVOFF_EL2 register */
    msr     CNTVOFF_EL2 , x0

    /* return from function */
    ret 

.endfunc

.func pharosCpuCntpCtlEl0Set
pharosCpuCntpCtlEl0Set:

    /* set the CNTP_CTL_EL0 register */
    msr     CNTP_CTL_EL0, x0

    /* return from function */
    ret 

.endfunc


.func pharosCpuCnthpCtlEl2Set
pharosCpuCnthpCtlEl2Set:

    /* set the CNTHP_CTL_EL2 register */
    msr     CNTHP_CTL_EL2, x0

    /* return from function */
    ret 

.endfunc


.func pharosCpuCnthpTvalEl2Set
pharosCpuCnthpTvalEl2Set:

    /* set the CNTP_TVAL_EL1 register */
    msr     CNTHP_TVAL_EL2, x0

    /* return from function */
    ret 

.endfunc


.func pharosCpuCntvCtlEl0Set
pharosCpuCntvCtlEl0Set:

    /* set the CNTV_CTL_EL0 register */
    msr     CNTV_CTL_EL0, x0

    /* return from function */
    ret 

.endfunc

/**
 * void pharosCpuCntpCvalEl0Set(uint64_t value);
 */
.func pharosCpuCntpCvalEl0Set
pharosCpuCntpCvalEl0Set:

    /* set the CNTP_CVAL_EL0 register */
    msr     CNTP_CVAL_EL0, x0

    /* return from function */
    ret 

.endfunc


/**
 * void pharosCpuCntvCvalEl0Set(uint64_t value);
 */
.func pharosCpuCntvCvalEl0Set
pharosCpuCntvCvalEl0Set:

    /* set the CNTV_CVAL_EL0 register */
    msr     CNTV_CVAL_EL0, x0

    /* return from function */
    ret 

.endfunc


/**
 * void pharosCpuCntpTvalEl0Set(uint32_t value);
 */
.func pharosCpuCntpTvalEl0Set
pharosCpuCntpTvalEl0Set:

    /* set the CNTP_TVAL_EL0 register */
    msr     CNTP_TVAL_EL0, x0

    /* return from function */
    ret 

.endfunc


/**
 * void pharosCpuCntpTvalEl0Set(uint32_t value);
 */
.func pharosCpuCntvTvalEl0Set
pharosCpuCntvTvalEl0Set:

    /* set the CNTV_TVAL_EL0 register */
    msr     CNTV_TVAL_EL0, x0

    /* return from function */
    ret 

.endfunc

/**
 * uint64_t pharosCpuCntpCtEl0Get(void);
 */
.func pharosCpuCntpCtEl0Get
pharosCpuCntpCtEl0Get:

    mrs     x0 , CNTPCT_EL0

    /* return from function */
    ret

.endfunc


.func pharosCpuCntpCtlEl0Get
pharosCpuCntpCtlEl0Get:

    /* get the CNTP_CTL_EL0 register */
    mrs     x0 , CNTP_CTL_EL0

    /* return from function */
    ret

.endfunc


.func pharosCpuCntpTvalEl0Get
pharosCpuCntpTvalEl0Get:

    /* get the CNTP_TVAL_EL0 register */
    mrs     x0, CNTP_TVAL_EL0

    /* return from function */
    ret

.endfunc


.func pharosCpuCnthpCtlEl2Get
pharosCpuCnthpCtlEl2Get:

    /* get the CNTHP_CTL_EL2 register */
    mrs     x0, CNTHP_CTL_EL2

    /* return from function */
    ret

.endfunc


.func pharosCpuCnthpTvalEl2Get
pharosCpuCnthpTvalEl2Get:

    /* get the CNTHP_TVAL_EL2 register */
    mrs     x0, CNTHP_TVAL_EL2

    /* return from function */
    ret

.endfunc


.func pharosCpuCntpCvalEl0Get
pharosCpuCntpCvalEl0Get:

    /* get the CNTP_TVAL_EL0 register */
    mrs     x0, CNTP_CVAL_EL0

    /* return from function */
    ret

.endfunc
 
