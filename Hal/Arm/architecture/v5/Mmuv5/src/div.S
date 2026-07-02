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


.type __aeabi_uidiv, %function
.type __aeabi_uidivmod, %function

.global __aeabi_uidiv
.global __aeabi_uidivmod


.syntax unified
.text

@ unsigned __aeabi_uidiv(unsigned num, unsigned denom)
@
@ Just an alias for __aeabi_uidivmod(), the remainder is ignored
@


@ {unsigned quotient:r0, unsigned remainder:r1}
@  __aeabi_uidivmod(unsigned numerator:r0, unsigned denominator:r1)
@
@ Divide r0 by r1 and return the quotient in r0 and the remainder in r1
@

.arm
.func __aeabi_uidiv
__aeabi_uidiv:

	#cmp	r1, #0
	#bne	L_no_div0
	#b	__aeabi_idiv0
	@ Shift left the denominator until it is greater than the numerator
	movs	r2, #1		@ counter
	
    movs	r3, #0		@ result
	
    cmp     r0, r1
	bls     L_sub_loop0_1
	adds	r1, #0		@ dont shift if denominator would overflow
	bmi     L_sub_loop0_1
	
L_denom_shift_loop_1:
	lsls	r2, #1
	lsls	r1, #1
	bmi     L_sub_loop0_1
	cmp     r0, r1
	bhi     L_denom_shift_loop_1
	
L_sub_loop0_1:	
	cmp     r0, r1
	bcc     L_dont_sub0_1	@ if (num>denom)

	subs	r0, r1		@ numerator -= denom
	orrs	r3, r2		@ result(r3) |= bitmask(r2)
L_dont_sub0_1:

	lsrs	r1, #1		@ denom(r1) >>= 1
	lsrs	r2, #1		@ bitmask(r2) >>= 1
	bne     L_sub_loop0_1

	mov     r1, r0		@ remainder(r1) = numerator(r0)
	mov     r0, r3		@ quotient(r0) = result(r3)

    /* return to caller */
	bx      lr
.endfunc



.arm
.func __aeabi_uidivmod
__aeabi_uidivmod:


	#cmp	r1, #0
	#bne	L_no_div0
	#b	__aeabi_idiv0
L_no_div0:

	@ Shift left the denominator until it is greater than the numerator
	movs	r2, #1		@ counter
	
    movs	r3, #0		@ result
	
    cmp     r0, r1
	bls     L_sub_loop0
	adds	r1, #0		@ dont shift if denominator would overflow
	bmi     L_sub_loop0
	
L_denom_shift_loop:
	lsls	r2, #1
	lsls	r1, #1
	bmi     L_sub_loop0
	cmp     r0, r1
	bhi     L_denom_shift_loop	
	
L_sub_loop0:	
	cmp     r0, r1
	bcc     L_dont_sub0	@ if (num>denom)

	subs	r0, r1		@ numerator -= denom
	orrs	r3, r2		@ result(r3) |= bitmask(r2)
L_dont_sub0:

	lsrs	r1, #1		@ denom(r1) >>= 1
	lsrs	r2, #1		@ bitmask(r2) >>= 1
	bne     L_sub_loop0

	mov     r1, r0		@ remainder(r1) = numerator(r0)
	mov     r0, r3		@ quotient(r0) = result(r3)

    /* return to caller */
	bx      lr

.endfunc
