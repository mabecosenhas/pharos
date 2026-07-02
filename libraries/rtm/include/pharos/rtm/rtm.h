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


/*! \defgroup PHAROS-RTM PHAROS-RTM
 *
 * Math library that provides the most common floating point (float64_t) operations.
 * 
 * Based on the FDLIBM library from Sun Microsystems, changes have been made to this library in order
 * to remove non-reentrant functions (all of the functions here are reentrant) and remove any reference to TLS 
 * (Thread Local Storage) variables, such as errno.
 *  
 * This math library is compliant with the IEEE 754 standard with the exception of inexact exceptions being raised.
 * That is, in some functions (like sin(x) with x = 0) the return value is fixed and the result is exact. However,
 * for x << 1 and x != 0 then sin(x) = x (but not exact) and an inexact exception should be raised.
 *  
 * In RTM there are no exceptions raised. This is because in many CPU arhictectures this requires the use of TLS 
 * variables. Given the little added value of having these exceptions and the TLS overhead, it was decided not to be
 * implemented.
 * 
 * Due to the changes made, the functions no longer follow the C Standard Library (e.g. C90). Therefore the functions
 * were also renamed (e.g sin(x) changed to rtmSinf64(x)) so as not to confuse this library user. This has the added 
 * advantage that this library can be used together with the C Standard Library (if such a need arises).
 * 
 * When possible, this library uses hardware instructions to perform the operations. Most notably with fabs and sqrt.
 * 
 * This library has also been "improved" to:
 * - correct many MISRA rules violations
 * - increase the number of comments
 * - standardize the code style
 * - hardware optimizations
 * - remove floating point exceptions
 *  
 * @{
 */

#ifndef RTM_M_H
#define RTM_M_H


#include <pharos/rtm/rtmdefs.h>
#include <pharos/rtm/rtmkernel.h>
#include <pharos/rtm/rtminline.h>


/**
 * Calculate the arc cosine of x [radian] in case x belongs to [-1 , 1].
 * 
 * Special cases:
 *   x is NaN   -> NaN is returned
 *   x is +1    -> +0 is returned
 *   x is +Inf  -> NaN is returned
 *   x is -Inf  -> NaN is returned
 *   |x| > 1    -> NaN is returned
 * 
 * @param x value to calculate the arc cosine of
 * 
 * @return returns the arc cosine of x [radian]
 */
float64_t rtmAcosf64(float64_t x) __attribute__((const));


/**
 * Calculate the arc sine of x in case x belongs to [-1 , 1].
 * 
 * Special cases:
 *   x is NaN   -> NaN is returned
 *   x is +0    -> +0 is returned
 *   x is -0    -> -0 is returned
 *   x is +Inf  -> NaN is returned
 *   x is -Inf  -> NaN is returned
 *   |x| > 1    -> NaN is returned
 * 
 * @param x value to calculate the arc cosine of
 * 
 * @return returns the arc sine of x [radian]
 */
float64_t rtmAsinf64(float64_t x) __attribute__((const));


/**
 * Calculate the arc tangent of x [radian]
 * 
 * Special cases:
 *   x NaN      -> NaN is returned
 * 
 * @param x value to calculate the arc tangent of 
 * 
 * @return returns the arc tangent of x [radian].
 */
float64_t rtmAtanf64(float64_t x) __attribute__((const));


/**
 * Calculate the arc tangent of y/x [radian]. Uses the signs of both arguments to calculate the quadrant.
 * 
 * Special cases:
 *   x or y NaN -> NaN is returned
 * 
 * @param y the dividend
 * @param x the divisor
 * 
 * @return returns the arc tangent of y/x [radian].
 */
float64_t rtmAtan2f64(float64_t y , float64_t x) __attribute__((const));


/**
 * Calculate the cosine of x
 * 
 * Special cases:
 *   x is NaN   -> NaN is returned
 *   x is +Inf  -> NaN is returned
 *   x is -Inf  -> NaN is returned
 * 
 * @param x value to calculate the cosine of [radian]
 * 
 * @return returns the cosine of x 
 */
float64_t rtmCosf64(float64_t x) __attribute__((const));


/**
 * Calculate the sine of x
 * 
 * Special cases:
 *   x is NaN   -> NaN is returned
 *   x is +Inf  -> NaN is returned
 *   x is -Inf  -> NaN is returned
 * 
 * @param x value to calculate the sine of [radian]
 * 
 * @return returns the sine of x 
 */
float64_t rtmSinf64(float64_t x) __attribute__((const));


/**
 * Calculate the tangent of x
 * 
 * Special cases:
 *   x is NaN   -> NaN is returned
 *   x is +Inf  -> NaN is returned
 *   x is -Inf  -> NaN is returned
 * 
 * @param x value to calculate the tangent of [radian]
 * 
 * @return returns the tangent of x 
 */
float64_t rtmTanf64(float64_t x) __attribute__((const));


/**
 * Calculate the hyperbolic cosine of x
 * 
 * cosh(x) = (exp(x) + exp(-x)) / 2
 * 
 * Special cases:
 *   x is NaN   -> NaN is returned
 *   x is +Inf  -> +Inf is returned
 *   x is -Inf  -> +Inf is returned
 * 
 * Exceptions:
 *   Inexact flag raised if x is not 0.
 * 
 * @param x value to calculate the hyperbolic cosine of
 * 
 * @return returns the hyperbolic cosine of x 
 */
float64_t rtmCoshf64(float64_t x) __attribute__((const));


/**
 * Calculate the hyperbolic sine of x
 * 
 * sinh(x) = (exp(x) - exp(-x)) / 2
 * 
 * Special cases:
 *   x is NaN   -> NaN is returned
 *   x is +Inf  -> +Inf is returned
 *   x is -Inf  -> -Inf is returned
 * 
 * @param x value to calculate the hyperbolic sine of
 * 
 * @return returns the hyperbolic sine of x 
 */
float64_t rtmSinhf64(float64_t x) __attribute__((const));


/**
 * Calculate the hyperbolic tangent of x
 * 
 * tanh(x) = sinh(x) / cosh(x)
 * 
 * Special cases:
 *   x is NaN   -> NaN is returned
 *   x is +Inf  -> +1 is returned
 *   x is -Inf  -> -1 is returned
 * 
 * @param x value to calculate the hyperbolic tangent of
 * 
 * @return returns the hyperbolic tangent of x 
 */
float64_t rtmTanhf64(float64_t x) __attribute__((const));


/**
 * Calculate the exponential of x (e raised to the power of x -> e^x)
 * 
 * Special cases:
 *   x is NaN   -> NaN is returned
 *   x is +Inf  -> +Inf is returned
 *   x is -Inf  -> +0 is returned
 * 
 * @param x value to calculate the exponential of
 * 
 * @return returns the exponential of x (e^x)
 */
float64_t rtmExpf64(float64_t x) __attribute__((const));


/**
 * Split the number x into a normalized fraction and an exponent (stored in y)
 * for non-zero x 
 *	x = frexp(arg,&exp);
 * return a float64_t fp quantity x such that 0.5 <= |x| <1.0
 * 
 * Special cases:
 *   x is 0     -> 0 is returned
 *   x is NaN   -> NaN is returned
 *   x is +Inf  -> +Inf is returned
 *   x is -Inf  -> -Inf is returned
 * 
 * @param x value to calculate the exponential of
 * @param exp address where to place the exponent
 * 
 * @return returns the fraction
 */
float64_t rtmFrexpf64(float64_t x , int32_t *exp) __attribute__((const));


/**
 * Calculate x * 2^y 
 * 
 * Special cases:
 *   x is NaN   -> NaN is returned
 *   y is 0     -> x is returned
 *   x is +Inf  -> +Inf is returned
 *   x is -Inf  -> +0 is returned
 * 
 * @param x value to calculate the exponential of
 * @param y value of y
 * 
 * @return returns x * 2^y 
 */
float64_t rtmLdexpf64(float64_t x , int32_t y) __attribute__((const));


/**
 * Calculate ln(x) (natural logarithm of x)
 * 
 * Special cases:
 *   x is NaN   -> NaN is returned
 *   x is +Inf  -> +Inf is returned
 *   x is +-0   -> -Inf is returned
 * 
 * @param x value to calculate the ln of
 * 
 * @return returns ln(x) 
 */
float64_t rtmLogf64(float64_t x) __attribute__((const));


/**
 * Calculate log10(x) (logarithm of base 10 of x)
 * 
 * Special cases:
 *   x is NaN   -> NaN is returned
 *   x is +Inf  -> +Inf is returned
 *   x is +-0   -> -Inf is returned
 * 
 * @param x value to calculate the logarithm base 10 of
 * 
 * @return returns log10(x) 
 */
float64_t rtmLog10f64(float64_t x) __attribute__((const));


/**
 * Extract the signed integral part of x and the fractional of x
 * 
 * Special cases:
 *   x is NaN   -> NaN is returned
 *   x is +Inf  -> +0 is returned, *y is set to +Inf
 *   x is -Inf  -> -0 is returned, *y is set to -Inf
 * 
 * @param x value to calculate
 * @param y address where to place the integral part of x
 * 
 * @return returns the fractional integral part of x
 */
float64_t rtmModf64(float64_t x , float64_t *y) __attribute__((const));


/**
 * Calculate x^y
 * 
 * Special cases:
 *   x is +1                -> 1 is returned (always)
 *   x is NaN or y is NaN   -> NaN is returned
 * 
 * @param x base
 * @param y exponent
 * 
 * @return returns x^y
 */
float64_t rtmPowf64(float64_t x , float64_t y) __attribute__((const));


/**
 * Calculate the square root of x
 * 
 * Special cases:
 *   x is NaN   -> NaN is returned
 *   x is +-0   -> +-0 is returned
 *   x is +Inf  -> +Inf is returned
 *   x < -0     -> NaN is returned
 * 
 * @param x value to calculate
 * 
 * @return returns the square root of x
 */
float64_t rtmSqrtf64(float64_t x) __attribute__((const));


/**
 * Calculate the ceil of x (smallest integral value not less than x)
 * 
 * 
 * Special cases:
 *   x is NaN   -> NaN is returned
 *   x is +Inf  -> +Inf is returned
 *   x is -Inf  -> -Inf is returned
 * 
 * Exceptions:
 *   Inexact flag raised if x not equal to ceil(x).
 * 
 * @param x value to calculate
 * 
 * @return returns x rounded toward -Inf to integral value.
 */
float64_t rtmCeilf64(float64_t x) __attribute__((const));


/**
 * Calculate the floor of x (argest integral value not greater than x)
 * 
 * Special cases:
 *   x is NaN   -> NaN is returned
 *   x is +Inf  -> +Inf is returned
 *   x is -Inf  -> -Inf is returned
 * 
 * Exceptions:
 *   Inexact flag raised if x not equal to floor(x).
 * 
 * @param x value to calculate
 * 
 * @return returns the floor of x
 */
float64_t rtmFloorf64(float64_t x) __attribute__((const));


/**
 * Calculate the floating-point remainder of x / y.
 * 
 * Special cases:
 *   x or y is NaN  -> NaN is returned
 *   x is +-Inf     -> NaN is returned
 *   y is 0         -> NaN is returned
 * 
 * @param x dividend
 * @param y divisor
 * 
 * @return returns the remainder of x / y
 */
float64_t rtmFmodf64(float64_t x , float64_t y) __attribute__((const));


/**
 * Calculate the error function of x
 * 
 * erf(x) = 2 / sqrt(pi) * (integral from 0 to x of exp(-y * y) dy)
 * 
 * Special cases:
 *   x is NaN   -> NaN is returned
 *   x is +Inf  -> +1 is returned
 *   x is -Inf  -> -1 is returned
 * 
 * @param x value to calculate
 * 
 * @return returns the error function of x
 */
float64_t rtmErff64(float64_t x) __attribute__((const));


/**
 * Calculate the complementary error function of x
 * 
 * erfc(x) = 1 - erf(x)
 * 
 * Special cases:
 *   x is NaN   -> NaN is returned
 *   x is +Inf  -> +0 is returned
 *   x is -Inf  ->  2 is returned
 * 
 * @param x value to calculate
 * 
 * @return returns the complementary error function of x
 */
float64_t rtmErfcf64(float64_t x) __attribute__((const));


/**
 * Calculate the Euclidean distance between the vector (x,y) to the origin (0,0). 
 * In short, returns sqrt( x^2 + y^2 )
 * 
 * Special cases:
 *   x or y is +-Inf-> +Inf is returned
 *   x or y is NaN  -> NaN is returned
 * 
 * @param x first variable
 * @param y second variable
 * 
 * @return returns sqrt( x^2 + y^2 )
 */
float64_t rtmHypotf64(float64_t x , float64_t y) __attribute__((const));


/**
 * Calculate the Bessel function of the first kind of order 0 for a given x
 * 
 * Special cases:
 *   x is NaN  -> NaN is returned
 *   x is +Inf -> 0 is returned
 * 
 * @param x value to calculate
 * 
 * @return returns the Bessel function of the first kind of order 0 for a given x
 */
float64_t rtmJ0f64(float64_t x) __attribute__((const));


/**
 * Calculate the Bessel function of the first kind of order 1 for a given x
 * 
 * Special cases:
 *   x is NaN  -> NaN is returned
 *   x is +Inf -> 0 is returned
 * 
 * @param x value to calculate
 * 
 * @return returns the Bessel function of the first kind of order 1 for a given x
 */
float64_t rtmJ1f64(float64_t x) __attribute__((const));


/**
 * Calculate the Bessel function of the first kind of order n for a given x
 * 
 * Special cases:
 *   x is NaN  -> NaN is returned
 *   x is +Inf -> 0 is returned
 * 
 * @param x value to calculate
 * @param n order of the Bessel function
 * 
 * @return returns the Bessel function of the first kind of order n for a given x
 */
float64_t rtmJnf64(int32_t n , float64_t x) __attribute__((const));


/**
 * Calculate the Bessel function of the second kind of order 0 for a given x
 * 
 * Special cases:
 *   x is NaN  -> NaN is returned
 *   x is +Inf -> 0 is returned
 *   x < 0     -> NaN
 *   x is +-0  -> -Inf
 * 
 * @param x value to calculate
 * 
 * @return returns the Bessel function of the second kind of order 0 for a given x
 */
float64_t rtmY0f64(float64_t x) __attribute__((const));


/**
 * Calculate the Bessel function of the second kind of order 1 for a given x
 * 
 * Special cases:
 *   x is NaN  -> NaN is returned
 *   x is +Inf -> 0 is returned
 *   x < 0     -> NaN
 *   x is +-0  -> -Inf
 * 
 * @param x value to calculate
 * 
 * @return returns the Bessel function of the second kind of order 1 for a given x
 */
float64_t rtmY1f64(float64_t x) __attribute__((const));


/**
 * Calculate the Bessel function of the second kind of order n for a given x
 * 
 * Special cases:
 *   x is NaN  -> NaN is returned
 *   x is +Inf -> 0 is returned
 *   x < 0     -> NaN
 *   x is +-0  -> -Inf
 * 
 * @param x value to calculate
 * @param n order of the Bessel function
 * 
 * @return returns the Bessel function of the second kind of order n for a given x
 */
float64_t rtmYnf64(int32_t n , float64_t x) __attribute__((const));


/**
 * Calculate the inverse hyperbolic cosine of x
 * 
 * Special cases:
 *   x is NaN  -> NaN is returned
 *   x is +Inf -> +Inf is returned
 *   x < 1     -> NaN is returned
 *   x is +1   -> +0 is returned 
 * 
 * @param x value to calculate
 * 
 * @return returns the inverse hyperbolic cosine of x
 */
float64_t rtmAcoshf64(float64_t x) __attribute__((const));


/**
 * Calculate the inverse hyperbolic sine of x
 * 
 * Special cases:
 *   x is NaN   -> NaN is returned
 *   x is +-Inf -> +-Inf is returned
 *   x is +-0   -> +-0 is returned 
 * 
 * @param x value to calculate
 * 
 * @return returns the inverse hyperbolic sine of x
 */
float64_t rtmAsinhf64(float64_t x) __attribute__((const));


/**
 * Calculate the inverse hyperbolic tangent of x
 * 
 * Special cases:
 *   x is NaN   -> NaN is returned
 *   |x| is > 1 -> NaN is returned
 *   x is +-0   -> +-0 is returned
 *   x is +1    -> +Inf is returned
 *   x is -1    -> -Inf is returned
 * 
 * @param x value to calculate
 * 
 * @return returns the inverse hyperbolic tangent of x
 */
float64_t rtmAtanhf64(float64_t x) __attribute__((const));


/**
 * Calculate the cube root of x 
 * 
 * @param x value to calculate the cube root of
 * 
 * Special cases:
 *   x is NaN   -> NaN is returned
 *   x is +Inf  -> +Inf is returned
 *   x is -Inf  -> -Inf is returned
 *   x is +-0   -> +-0 is returned
 *   x is -0    -> NaN is returned
 * 
 * @return return cube root of x
 */
float64_t rtmCbrtf64(float64_t x) __attribute__((const));


/**
 * Get the exponent part of x (base 2). Consider using ilogb instead of this function.
 * 
 * Special cases:
 *   x is NaN   -> NaN is returned
 *   x is +Inf  -> +Inf is returned
 *   x is -Inf  -> +Inf is returned
 *   x is +-0   -> -Inf is returned
 * 
 * @param x value to calculate
 * 
 * @return returns the binary exponent of non-zero x
 */
float64_t rtmLogbf64(float64_t x) __attribute__((const));


/**
 * Calculates the next machine floating-point number of x in the direction toward y.
 * 
 * Special cases:
 *  x or y is NaN   -> NaN is returned
 *  
 * @param x value of x
 * @param y value of y
 * 
 * @return returns the next machine floating-point number of x in the direction toward y.
 */
float64_t rtmNextafterf64(float64_t x , float64_t y) __attribute__((const));


/**
 * Calculate x REM y = n, where n is the is the nearest integer to x / y.
 * 
 * Special cases:
 *   x or y is NaN   -> NaN is returned
 * 
 * @param x dividend
 * @param y divisor
 * 
 * @return returns x REM y
 */
float64_t rtmRemainderf64(float64_t x , float64_t y) __attribute__((const));


/**
 * Calculate x * RADIX ^ y where RADIX corresponds to the float point exponent base (most CPUs have RADIX = 2).
 * 
 * Special cases:
 *   x or y is NaN              -> NaN is returned
 *   x = +Inf and y != -Inf     -> +Inf is returned
 *   x = +Inf and y  = -Inf     -> NaN is returned (+Inf * 2 ^ -Inf)
 *   x = +-0  and y != +Inf     -> +-0 is returned
 *   x = +-0  and y  = +Inf     -> NaN is returned (+-0 * 2 ^ -Inf)
 * 
 * @param x value of x
 * @param y value of the exponent
 * 
 * @return returns x * RADIX ^ y
 */
float64_t rtmScalbf64(float64_t x , float64_t y) __attribute__((const));


/**
 * calculate the mantissa of x
 * 
 * @param x value to calculate
 * 
 * @return returns the mantissa of x
 */
float64_t rtmSignificandf64(float64_t x) __attribute__((const));


/**
 * Get the exponent part of x. This function is equivalent to logb except it returns an integer 32 bits.
 * 
 * Special cases:
 *   x is NaN   -> __INT32_MAX__ is returned
 *   x is +Inf  -> __INT32_MAX__ is returned
 *   x is -Inf  -> __INT32_MAX__ is returned
 *   x is +-0   -> -__INT32_MAX__ (the smallest int32_t number) is returned
 * 
 * @param x value to calculate
 * 
 * @return returns the binary exponent of non-zero x
 */
int32_t rtmIlogbf64(float64_t x) __attribute__((const));


/**
 * Calculates the nearest integer of x
 * 
 *  Special cases:
 *   x is NaN   -> NaN is returned
 *   x is +Inf  -> +Inf is returned
 *   x is -Inf  -> -Inf is returned
 * 
 * @param x value of x
 * 
 * @return returns x rounded to integral value according to the prevailing rounding mode.
 */
float64_t rtmRintf64(float64_t x) __attribute__((const));


/**
 * Calculates x * 2^n very fast (uses the internal representation of the floating to speed up the calculation)
 * 
 * @param x value of x
 * @param n value of n
 * 
 * @return returns x * 2^n computed by exponent manipulation rather than by actually performing an 
 * exponentiation or a multiplication.
 */
float64_t rtmScalbnf64(float64_t x , int32_t n) __attribute__((const));


/**
 * Returns exp(x)-1, the exponential of x minus 1.
 * 
 * @param x the value to calculate 
 * 
 * @return Returns exp(x)-1, the exponential of x minus 1.
 */
float64_t rtmExpm1f64(float64_t x) __attribute__((const));


/**
 * Calculate ln(1+x) with 64-bit float
 * 
 * Special cases:
 *   x < -1      -> NaN is returned with signal
 *   x is -Inf   -> NaN is returned with signal
 *   x is +Inf   -> +Inf
 *   x is -1     -> -Inf
 *   x is NaN    -> NaN is returned with no signal
 * 
 * @param x value of x
 * 
 * @return returns the value of ln(1+x)
 */
float64_t rtmLog1f64(float64_t x) __attribute__((const));


/**
 * Calculate the gamma function of x
 * 
 * Gamma(x) = integral (from 0 to infinity) t^(x-1) e^-t dt
 * 
 * Special cases:
 *   x is NaN   -> NaN is returned
 *   x is +Inf  -> +Inf is returned
 *   x is -Inf  -> -Inf is returned
 *   x is < -0  -> NaN is returned
 *   x is -0    -> NaN is returned
 * 
 * @param x value to calculate
 * @param signgamp address where to place the signgamp
 * 
 * @return returns the complementary error function of x
 */
float64_t rtmLgammaf64(float64_t x , int32_t *signgamp) __attribute__((const));


/**
 * Determines if x is a finite number (as opposed to NaN or Inf)
 * 
 * @param x the float64_t to analyse
 * 
 * @return returns 0 if not finite and different from 0 if it is finite
 */
INLINE int32_t rtmIsFinitef64(float64_t x) __attribute__((const));


/**
 * Determines if x is a finite number (as opposed to NaN or Inf)
 * 
 * @param x the float64_t to analyse
 * 
 * @return returns 0 is not finite and different from 0 if it is finite
 */
INLINE int32_t rtmIsInfinitef64(float64_t x) __attribute__((const));

/**
 * Calculates the absolute value of x
 * 
 * @param x the value to calculate the absolute value of
 * 
 * Special cases:
 *   x is NaN   -> NaN is returned
 *   x is +Inf  -> +Inf is returned
 *   x is -Inf  -> +Inf is returned
 *   x is -0    -> +0 is returned
 * 
 * @return returns |x|
 */
float64_t rtmAbsf64(float64_t x) __attribute__((const));


/**
 * Determines if the specified float64_t number is a NaN
 * 
 * @param x the float64_t to analyse
 * 
 * @return returns a nonzero value if x is a NaN
 */
INLINE int32_t rtmIsNanf64(float64_t x) __attribute__((const));


/**
 * Determines if the specified float64_t number is a zero (+-0)
 * 
 * @param x value to check
 * 
 * @return returns 1 if the value is 0 and 0 otherwise
 */
INLINE int32_t rtmIsZerof64(float64_t x) __attribute__((const));


/**
 * Calculates a value with the magnitude of x and with the sign bit of y.
 * 
 * In short, removes the sign of x and place the sign of y on the high word of x.
 * 
 * @param x value of x
 * @param y value of y
 * 
 * @return returns a value with the magnitude of x and with the sign bit of y. 
 */
INLINE float64_t rtmCopySignf64(float64_t x , float64_t y) __attribute__((const));


/**
 * Get the high bits of x
 * 
 * @param x value of x
 * 
 * @return returns the 32 highest bits of x
 */
INLINE int32_t rtmGetHighf64(float64_t x) __attribute__((const));


/**
 * Get the low bits of x
 * 
 * @param x value of x
 * 
 * @return returns the 32 lowest bits of x
 */
INLINE int32_t rtmGetLowf64(float64_t x) __attribute__((const));


/* RTM_M_H */
#endif 


/*! @} */
