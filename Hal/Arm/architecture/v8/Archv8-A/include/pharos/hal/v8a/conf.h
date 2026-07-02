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


#ifndef PHAROS_V8A_CONF_H
#define	PHAROS_V8A_CONF_H


/**
 * Error caused by incorrect configuration of the channel bulk queue number of MMU tables of level 3 (insufficient)
 */
#define PHAROS_CPU_ERROR_CHANNEL_LEVEL3_TABLES_EMPTY        (PHAROS_ERR_CPU_START + 0U)

/**
 * Error caused by incorrect configuration of the channel bulk queue number of MMU tables of level 2 (insufficient)
 */
#define PHAROS_CPU_ERROR_CHANNEL_LEVEL2_TABLES_EMPTY        (PHAROS_ERR_CPU_START + 1U)

/**
 * The ARMv8-A always has an MMU
 */
#define PHAROS_CPU_HAS_MMU


/**
 * Cpu is an ARMv8-A
 */
#define PHAROS_CPU_ARMV8A


/**
 * Alignment required to copy 8 bytes at a time
 */
#define PHAROS_CPU_COPY8_ALIGN                              (8U)


/**
 * Alignment required to copy 4 bytes at a time
 */
#define PHAROS_CPU_COPY4_ALIGN                              (4U)

/* PHAROS_CONF_H */
#endif	
