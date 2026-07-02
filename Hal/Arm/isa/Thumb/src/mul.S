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


.type __aeabi_lmul, %function

.global __aeabi_lmul

.syntax unified
.text


/**
 * uint64_t __aeabi_lmul(uint64_t a , uint64_t b)
 * 
 * r0 - a
 * r1 - a
 * r2 - b
 * r3 - b
 */
.thumb
.func __aeabi_lmul
__aeabi_lmul:

    mul     %r3 , %r0 , %r3

    /* save aux registers */
    push    {%r5 , %r6}

    /* multiply the upper parts of r0 and r2 and store them in r5 and r6 */
    umull   %r5 , %r6 , %r0 , %r2

    /* multiply the lower parts of r1 and r3 and store them in r1 and r2 */
    mla     %r2 , %r1 , %r2 , %r3

    /* store the upper part in r0 */
    mov     %r0 , %r5

    /* add the lower half to r6 */
    add     %r6 , %r2 , %r6

    /* store the lower part in r1 */
    mov     %r1 , %r6

    /* restore aux registers */
    pop     {%r5 , %r6}

    /* return to caller */
    bx      %lr

.endfunc
