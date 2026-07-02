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


#ifndef PHAROS_API_INTERRUPT_H
#define PHAROS_API_INTERRUPT_H

#include <pharos/declarations.h>


/**
 * Catch an interrupt given its index on the running partition
 * 
 * @param index index of the interrupt to install on the running partition InterruptTable
 * @param argument argument to pass to the interrupt routine
 * 
 * @return returns success or the error when attempting to install the interrupt
 */
PharosInterruptInstallR pharosInterruptInstall(uint32_t index , void *argument);


/**
 * Removes the interrupt given index on the running partition 
 * 
 * @param index index of the interrupt to remove from the running partition
 * 
 * @return returns success or the error when attempting to remove the interrupt 
 */
PharosInterruptRemoveR pharosInterruptRemove(uint32_t index);


/**
 * Executes the specified critical section given its index on the running partition
 * 
 * @param index index of the critical section to execute on the running partition CriticalSectionTable
 * @param arg1 argument 1 of the critical section function
 * @param arg2 argument 2 of the critical section function
 * @param arg3 argument 3 of the critical section function
 * @param arg4 argument 4 of the critical section function
 * @return returns success or error when attempting to execute the critical section
 */
PharosCsExecuteR pharosCriticalSectionExecute(uint32_t index , uint32_t arg1 , uint32_t arg2 , uint32_t arg3 , uint32_t arg4 , uint32_t *output);


#endif /* PHAROS_API_INTERRUPT_H */
