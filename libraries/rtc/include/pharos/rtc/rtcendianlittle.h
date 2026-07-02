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


#ifndef PHAROS_RTCENDIANLITTLE_H
#define PHAROS_RTCENDIANLITTLE_H

#include <pharos/rtc/rtcendian.h>


INLINE void rtcMemCopyNumber2Be16(uint8_t *buffer , uint16_t number)
{
    buffer[0] = (uint8_t) (number >> 8U);
    buffer[1] = (uint8_t) (number & 0xffU);
}


INLINE void rtcMemCopyNumber2Be32(uint8_t *buffer , uint32_t number)
{
    buffer[3] = (uint8_t) ((number >> 24U) & 0xffU);
    buffer[2] = (uint8_t) ((number >> 16U) & 0xffU);
    buffer[1] = (uint8_t) ((number >> 8U) & 0xffU);
    buffer[0] = (uint8_t) (number & 0xffU);
}


INLINE void rtcMemCopyNumber2Be64(uint8_t *buffer , uint64_t number)
{
    buffer[7] = (uint8_t) ((number >> 56U) & 0xffU);
    buffer[6] = (uint8_t) ((number >> 48U) & 0xffU);
    buffer[5] = (uint8_t) ((number >> 40U) & 0xffU);
    buffer[4] = (uint8_t) ((number >> 32U) & 0xffU);
    buffer[3] = (uint8_t) ((number >> 24U) & 0xffU);
    buffer[2] = (uint8_t) ((number >> 16U) & 0xffU);
    buffer[1] = (uint8_t) ((number >> 8U) & 0xffU);
    buffer[0] = (uint8_t) (number & 0xffU);
}


INLINE uint16_t rtcEndianHost2Be16(uint16_t source)
{
    /* use the builtin function to switch the endianness */
    return __builtin_bswap16(source);
}


INLINE uint32_t rtcEndianHost2Be32(uint32_t source)
{
    /* use the builtin function to switch the endianness */
    return __builtin_bswap32(source);
}


INLINE uint64_t rtcEndianHost2Be64(uint64_t source)
{
    /* use the builtin function to switch the endianness */
    return __builtin_bswap64(source);
}


INLINE uint16_t rtcEndianBe2Host16(uint16_t source)
{
    /* use the builtin function to switch the endianness */
    return __builtin_bswap16(source);
}


INLINE uint32_t rtcEndianBe2Host32(uint32_t source)
{
    /* use the builtin function to switch the endianness */
    return __builtin_bswap32(source);
}


INLINE uint64_t rtcEndianBe2Host64(uint64_t source)
{
    /* use the builtin function to switch the endianness */
    return __builtin_bswap64(source);
}

#endif /* PHAROS_RTCENDIANLITTLE_H */
