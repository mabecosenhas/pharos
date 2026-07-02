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


.type pharosCpuIsb, %function
.type pharosCpuDsb, %function
.type pharosCpuDmb, %function

.global pharosCpuIsb
.global pharosCpuDsb
.global pharosCpuDmb

.syntax unified
.text


.func pharosCpuIsb
pharosCpuIsb:

    /* perform the isb */
    isb

    /* return to caller */
    bx      lr

.endfunc


.func pharosCpuDsb
pharosCpuDsb:

    /* perform the dsb */
    dsb

    /* return to caller */
    bx      lr

.endfunc


.func pharosCpuDmb
pharosCpuDmb:

    /* perform the dmb */
    dmb

    /* return to caller */
    bx      lr

.endfunc
