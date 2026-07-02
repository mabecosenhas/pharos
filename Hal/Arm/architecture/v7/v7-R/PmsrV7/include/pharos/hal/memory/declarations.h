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


#ifndef PHAROS_PMSRV7_DECLARATIONS_H
#define	PHAROS_PMSRV7_DECLARATIONS_H


/**
 * Calculate the correct size of the shared stack. Must be the next power of 2 of the original stack size and not 
 * below 256 bytes
 */
#define PHAROS_SHARED_STACK_CORRECT_SIZE(size)      MAX_MACRO(256U , PHAROS_NEXT_POWER_2(size))


/**
 * Alignment of the shared stack. Must be aligned be the size of the stack
 */
#define PHAROS_DYNAMIC_STACK_ALIGN(size)   PHAROS_SHARED_STACK_CORRECT_SIZE(size)


#endif	/* DECLARATIONS_H */
