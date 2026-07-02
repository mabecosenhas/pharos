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


#ifndef PHAROS_SDRAM_H
#define	PHAROS_SDRAM_H

#include <pharos/hal/hal.h>


/**
 * Enable the SDRAM clock 
 */
void pharosCpuSdramClockEnable();


/**
 * Send the PALL command to the SDRAM controller
 */
void pharosCpuSdramPall();


/**
 * Send the auto-refresh command to the SDRAM controller
 * 
 * @param numberAutoRefresh number of auto-refresh  cycles (depends on the SDRAM chip used)
 */
void pharosCpuSdramAutorefresh(uint32_t numberAutoRefresh);


/**
 * Send the Load  command to the SDRAM controller
 * 
 * @param mrd MRD field to send (depends on the SDRAM chip used)
 */
void pharosCpuSdramLoadCommand(uint32_t mrd);


/* PHAROS_SDRAM_H */
#endif	
