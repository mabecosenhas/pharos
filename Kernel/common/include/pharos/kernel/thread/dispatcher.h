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


#ifndef PHAROS_DISPATCHER_H
#define PHAROS_DISPATCHER_H

#include <pharos/kernel/thread/declarations.h>    

/**
 * disables dispatching (one level)
 */
INLINE void pharosIDispatchDisable(void);


/**
 * enables dispatching (one level). If dispatching is enabled, then invoke thread dispatch
 */
INLINE void pharosIDispatchEnable(void);


/**
 * Determines if thread dispatching is enabled 
 * 
 * @return returns TRUE if dispatching is enable and FALSE otherwise
 */
INLINE bool pharosIIsDispatchingEnabled(void);


/**
 * Dispatch to the highest priority ready thread
 */
void pharosIDispatchThread(void);


/**
 * Dispatch to the highest priority ready thread without saving the context of the running thread
 */
void pharosIDispatchHeirThread(void) __attribute__((noreturn));


#endif /* DISPATCHER_H */
