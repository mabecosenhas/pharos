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


#ifndef PHAROS_ASSERT_DECLARATIONS_H
#define PHAROS_ASSERT_DECLARATIONS_H


/**
 * Declare the printk function (application must be compiled with RTC library linked)
 * 
 * @param fmt argument
 * @param ... several arguments
 */
void printk(const char *fmt , ...) __attribute__((format(printf , 1 , 2)));


/**
 * empty function to allow the developer to place a break point and debug when an error occurs
 */

void pharosDebugCallback();


#endif /* PHAROS_ASSERT_DECLARATIONS_H */
