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


/*! \addtogroup PHAROS-RTM
 * 
 * @{
 */

#ifndef RTM_KERNEL_H
#define RTM_KERNEL_H

#include <pharos/rtm/rtmdefs.h>


/**
 * kernel sin function on [-pi/4, pi/4], pi/4 ~ 0.785398164 of x
 * 
 * 
 * @param x value of x assumed to be bounded by ~pi/4 in magnitude.
 * @param y y is the tail of x
 * @param iy iy indicates whether y is 0. (if iy=0, y assume to be 0). 
 * 
 * @return returns the sine of x
 */
float64_t rtmKernelSinf64(float64_t x , float64_t y , int32_t iy) __attribute__((const));


/**
 * kernel cos function on [-pi/4, pi/4], pi/4 ~ 0.785398164 of x
 * 
 * @param x value of x (assumed to be bounded by ~pi/4 in magnitude)
 * @param y y is the tail of x
 * 
 * @return returns the cosine of x
 */
float64_t rtmKernelCosf64(float64_t x , float64_t y) __attribute__((const));


/**
 * kernel tan function on [-pi/4, pi/4], pi/4 ~ 0.7854 of x
 * 
 * @param x value of x (assumed to be bounded by ~pi/4 in magnitude)
 * @param y y is the tail of x.
 * @param k k indicates whether tan (if k = 1) or -1/tan (if k = -1) is returned.
 * 
 * @return returns the tangent of x
 */
float64_t rtmKernelTanf64(float64_t x , float64_t y , int32_t k) __attribute__((const));


/**
 * calculate the last three digits of N with y = x - N*pi/2 so that |y| < pi/2
 * 
 * @param x value of x
 * @param y y address where to place the result (must be an array of 2 positions)
 * @param e0
 * @param nx
 * 
 * @return returns the last three digits of N with y = x - N*pi/2 so that |y| < pi/2
 */
int32_t rtmKernelRemPio2f64(float64_t *x , float64_t *y , int32_t e0 , int32_t nx);


/**
 * Calculate the remainder of x rem pi/2 in y[0]+y[1]. 
 * 
 * @param x value to calculate the rem pi/2 of
 * @param y address where to place the result (must be an array of 2 positions)
 * 
 * @return returns the last three digits of N with y = x - N*pi/2 so that |y| < pi/2.
 */
int32_t rtmRemPio2f64(float64_t x , float64_t* y);


/* RTM_KERNEL_H */
#endif 


/*! @} */
