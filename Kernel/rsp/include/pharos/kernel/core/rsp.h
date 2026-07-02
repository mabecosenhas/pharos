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


#ifndef PHAROS_RSP_H
#define	PHAROS_RSP_H

#include <pharos/hal/hal.h>


/**
 * Initialize the data sections for all partitions and kernel, shared and IPCall
 */
void pharosIDataInitialize(void);


/**
 * Initialize Pharos in single core configuration
 */
void pharosSingleCoreInitialize() __attribute__((noreturn));


/* PHAROS_RSP_H */
#endif	
