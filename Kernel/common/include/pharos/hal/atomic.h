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


#ifndef PHAROS_ATOMIC_H
#define PHAROS_ATOMIC_H

#include <pharos/hal/hal.h>


/**
 * Increment the number by the specified value atomically 
 * 
 * @param number number to increment
 * @param value value to increment by
 */
void pharosIUint32AtomicInc(uint32_t *number , uint32_t value);


/**
 * Decrement the number by the specified value atomically 
 * 
 * @param number number to decrement
 * @param value value to increment by
 * 
 * @return returns the number decremented by the value
 */
uint32_t pharosIUint32AtomicDec(uint32_t *number , uint32_t value);


/* PHAROS_ATOMIC_H */
#endif 
