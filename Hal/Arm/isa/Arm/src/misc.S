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


.type armbit2Integer,   %function

.global armbit2Integer 

.syntax unified
.text

.arm
.func armbit2Integer
armbit2Integer:

    /* calculate number of leading zeros and place them on r0 */
    clz     %r0, %r0

    /* r0 = 31 - r0 -> calculates the leftmost bit number */
    rsb     %r0, %r0, #31

    /* return from function */
    bx      %lr
.endfunc
