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


#ifndef RTCENDIAN_H
#define RTCENDIAN_H

#include <pharos/hal/hal.h>


#if __BYTE_ORDER__ == __BIG_ENDIAN__
#include <pharos/rtc/rtcendianbig.h>
#else
#include <pharos/rtc/rtcendianlittle.h>
#endif


/**
 * Convert a number (16-bits) from host native endianness to big-endian.
 * 
 * Typically used for communications since most of the protocols require big-endian. But other applications may also 
 * require this conversion.
 *  
 * @param source number in host endianness
 * 
 * @return returns the number in big-endian
 */
INLINE uint16_t rtcEndianHost2Be16(uint16_t source) __attribute__((const));


/**
 * Convert a number (32-bits) from host native endianness to big-endian.
 * 
 * Typically used for communications since most of the protocols require big-endian. But other applications may also 
 * require this conversion.
 *  
 * @param source number in host endianness
 * 
 * @return returns the number in big-endian
 */
INLINE uint32_t rtcEndianHost2Be32(uint32_t source) __attribute__((const));


/**
 * Convert a number (64-bits) from host native endianness to big-endian.
 * 
 * Typically used for communications since most of the protocols require big-endian. But other applications may also 
 * require this conversion.
 *  
 * @param source number in host endianness
 * 
 * @return returns the number in big-endian
 */
INLINE uint64_t rtcEndianHost2Be64(uint64_t source) __attribute__((const));


/**
 * Convert a number (16-bits) from big-endian to host endianness.
 * 
 * Typically used for communications since most of the protocols require big-endian. But other applications may also 
 * require this conversion.
 *  
 * @param source number in host endianness
 * 
 * @return returns the number in host endianness
 */
INLINE uint16_t rtcEndianBe2Host16(uint16_t source) __attribute__((const));


/**
 * Convert a number (32-bits) from big-endian to host endianness.
 * 
 * Typically used for communications since most of the protocols require big-endian. But other applications may also 
 * require this conversion.
 *  
 * @param source number in host endianness
 * 
 * @return returns the number in host endianness
 */
INLINE uint32_t rtcEndianBe2Host32(uint32_t source) __attribute__((const));


/**
 * Convert a number (64-bits) from big-endian to host endianness.
 * 
 * Typically used for communications since most of the protocols require big-endian. But other applications may also 
 * require this conversion.
 *  
 * @param source number in host endianness
 * 
 * @return returns the number in host endianness
 */
INLINE uint64_t rtcEndianBe2Host64(uint64_t source) __attribute__((const));


/**
 * Fills the buffer with the 16-bit number
 * 
 * @param buffer buffer to place the number
 * @param number number in host endianness
 */
INLINE void rtcMemCopyNumber2Be16(uint8_t *buffer , uint16_t number);

/**
 * Fills the buffer with the 32-bit number
 * 
 * @param buffer buffer to place the number
 * @param number number in host endianness
 */
INLINE void rtcMemCopyNumber2Be32(uint8_t *buffer , uint32_t number);


/**
 * Fills the buffer with the 64-bit number
 * 
 * @param buffer buffer to place the number
 * @param number number in host endianness
 */
INLINE void rtcMemCopyNumber2Be64(uint8_t *buffer , uint64_t number);


#endif /* RTCENDIAN_H */
