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


#ifndef PHAROS_ASSERT_H
#define PHAROS_ASSERT_H

#include <pharos/hal/hal.h>
#include <pharos/error.h>
#include <pharos/kernel/assert/declarations.h>
#include <pharos/kernel/core/coreassert.h>
#include <pharos/hal/boardApi.h>


/**
 * If compiler optimization is selected (e.g -O1, -Os, etc)
 */
#if __OPTIMIZE__

/**
 * Then don't make any assertions
 */
#define PHAROS_ASSERT(condition , message , ...)

#define PHAROS_ASSERT_EQUAL(obtained , expected , message , ...)

#define PHAROS_ASSERT_EQUAL_OR(obtained , expected ,  expected2, message , ...)


/**
 * If no optimizations are selected
 */
#else

/**
 * Define the Assert function to check for errors (typically inside Pharos, but could also be used by the application if partition has supervisor permissions)
 * If the specified condition is not true, then the system will display an error message and halt
 * 
 * @param condition condition that if FALSE, will trigger the error
 * @param message message to present to the printk function when the error occurs
 */
#define PHAROS_ASSERT(condition , message , ...)                                                                    \
do                                                                                                                  \
{                                                                                                                   \
    if(!(condition))                                                                                                \
    {                                                                                                               \
        PharosInterruptLevel interrupt;                                                                             \
        pharosCpuDisableInterrupts(&interrupt);                                                                     \
        /* optional debug callback function. It could be useful to place a breakpoint and see the back trace */     \
        pharosDebugCallback();                                                                                      \
        printk("\r\n\nPharos Panic Error in file %s, line %d, function %s\r\n\n" ,                                  \
               __FILE__ , __LINE__ , __func__);                                                                     \
        printk(message,  ##__VA_ARGS__);                                                                            \
        printk("\r\n");                                                                                             \
        pharosFatalErrorHandler(PHAROS_FATAL_INTERNAL_ERROR_DETECTED , __FILE__ , __func__ , __LINE__);             \
    }                                                                                                               \
}                                                                                                                   \
while(0)


/**
 * Define the Assert function to check for errors (typically inside Pharos, but could also be used by the application)
 * If the specified obtained value is not equal to the expected value, then an error is raised
 * 
 * @param obtained obtained value
 * @param expected expected value
 * @param message message to present to the printk function when the expected value is different than the obtained value
 */
#define PHAROS_ASSERT_EQUAL(obtained , expected , message , ...)                                                    \
do                                                                                                                  \
{                                                                                                                   \
    if((obtained) != (expected))                                                                                    \
    {                                                                                                               \
        /* optional debug callback function. It could be useful to place a breakpoint and see the back trace */     \
        pharosDebugCallback();                                                                                      \
        printk("\r\n\nPharos Panic Error in file %s, line %d, function %s\r\n\n" ,                                  \
               __FILE__ , __LINE__ , __func__);                                                                     \
        printk(message,  ##__VA_ARGS__);                                                                            \
        printk("\r\nobtained = 0x%x and expected = 0x%x\r\n",  obtained , expected);                                \
        while(1);                                                                                                   \
    }                                                                                                               \
}                                                                                                                   \
while(0)


/**
 * Define the Assert function to check for errors (typically inside Pharos, but could also be used by the application)
 * If the specified obtained value is not equal to the expected value, then an error is raised
 * 
 * @param obtained obtained value
 * @param expected expected value
 * @param message message to present to the printk function when the expected value is different than the obtained value
 */
#define PHAROS_ASSERT_EQUAL_OR(obtained , expected ,  expected2, message , ...)                                     \
do                                                                                                                  \
{                                                                                                                   \
    if((obtained != expected) && (obtained != expected2))                                                           \
    {                                                                                                               \
        /* optional debug callback function. It could be useful to place a breakpoint and see the back trace */     \
        pharosDebugCallback();                                                                                      \
        printk("\r\n\nPharos Panic Error in file %s, line %d, function %s\r\n\n" ,                                  \
               __FILE__ , __LINE__ , __func__);                                                                     \
        printk(message,  ##__VA_ARGS__);                                                                            \
        printk("\r\nobtained = 0x%x and expected = 0x%x\r\n",  obtained , expected);                                \
        while(1);                                                                                                   \
    }                                                                                                               \
}                                                                                                                   \
while(0);

#endif


/**
 * Static assertion
 */
#define PHAROS_ASSERT_STATIC(condition , message)   _Static_assert((condition), message)


#endif /* PHAROS_ASSERT_H */
