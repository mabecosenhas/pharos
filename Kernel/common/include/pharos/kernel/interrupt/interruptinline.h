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


#ifndef PHAROS_KERNEL_INTERRUPTINLINE_H
#define PHAROS_KERNEL_INTERRUPTINLINE_H

#include <pharos/kernel/interrupt/interrupt.h>
#include <pharos/kernel/core/coreconfinline.h>


INLINE ptrInterruptConfiguration pharosIInterruptConfigurationGet(PharosInterruptNumber vector)
{
    /* get the linked list containing the interrupt configurations to execute */
    return pharosICore()->pharosVIsrTable[vector];
}


INLINE ptrPartition pharosIInterruptRunningEnvGet(void)
{
    /* return the interrupt running partition */
    return pharosICore()->pharosVRunningInterruptEnv;
}


INLINE void pharosIInterruptRunningEnvSet(ptrPartition partition)
{
    /* return the interrupt running partition */
    pharosICore()->pharosVRunningInterruptEnv = partition;
}



#endif /* PHAROS_KERNEL_INTERRUPTINLINE_H */
