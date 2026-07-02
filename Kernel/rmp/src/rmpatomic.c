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


#include <pharos/hal/atomic.h>


void pharosIUint32AtomicInc(uint32_t *number , uint32_t value)
{
    /* use the atomic instruction to increment */
    __atomic_add_fetch(number , value , __ATOMIC_ACQUIRE);
}


uint32_t pharosIUint32AtomicDec(uint32_t *number , uint32_t value)
{
    /* use the atomic instruction to decrement */
    return __atomic_sub_fetch(number , value , __ATOMIC_ACQUIRE);
}
