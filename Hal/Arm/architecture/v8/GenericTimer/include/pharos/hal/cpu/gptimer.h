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


#ifndef PHAROS_GENERIC_TIMER_H
#define	PHAROS_GENERIC_TIMER_H

#include <pharos/hal/hal.h>


#define PHAROS_CPU_CNTK_CTL_EL0PCT_EN           (1 << 0)
#define PHAROS_CPU_CNTK_CTL_EL0PCT_DIS          (0 << 0)
#define PHAROS_CPU_CNTK_CTL_EL0VCT_EN           (1<<1)
#define PHAROS_CPU_CNTK_CTL_EL0VCT_DIS          (0<<1)

void pharosCpuGenericTimerInitialize(uint32_t frequency);

void pharosCpuCntFrqEl0Set(uint32_t value);

void pharosCpuCntvOffsetSet(uint32_t value);

void pharosCpuCntkCtlEl1Set(uint32_t value);

void pharosCpuCntpCtlEl0Set(uint32_t value);
void pharosCpuCntvCtlEl0Set(uint32_t value);
void pharosCpuCnthpCtlEl2Set(uint32_t value);


uint32_t pharosCpuCntpCtlEl0Get(void);
uint32_t pharosCpuCnthpCtlEl2Get(void);


void pharosCpuCntpCvalEl0Set(uint64_t value);
void pharosCpuCntvCvalEl0Set(uint64_t value);

int64_t pharosCpuCntpCvalEl0Get();

void pharosCpuCntpTvalEl0Set(uint32_t value);
void pharosCpuCntvTvalEl0Set(uint32_t value);
void pharosCpuCnthpTvalEl2Set(uint32_t value);


int32_t pharosCpuCntpTvalEl0Get(void);
int32_t pharosCpuCnthpTvalEl2Get(void);

uint64_t pharosCpuCntpCtEl0Get(void);

uint32_t pharosCpuCntFreqGet(void);


/* PHAROS_GENERIC_TIMER_H */
#endif	
