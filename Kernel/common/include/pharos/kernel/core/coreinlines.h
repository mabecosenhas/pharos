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


#ifndef PHAROS_PHAROS_COREINLINES_H
#define PHAROS_PHAROS_COREINLINES_H


#include <pharos/kernel/core/core.h>


INLINE bool pharosIPartitionHasSupervisor(void)
{
    /* return if the running partition has supervisor permissions */
    return pharosIPartitionGetRunning()->partitionFlags.isSupervisor;
}


INLINE bool pharosIPartitionIsSupervisor(ptrPartition partition)
{
    /* return if the partition has supervisor permissions */
    return partition->partitionFlags.isSupervisor;
}


INLINE bool pharosICoreIsInNominalState(ptrCore core)
{
    /* return TRUE if the state is nominal */
    return core->state == PHAROS_CORE_STATE_NOMINAL ? TRUE : FALSE;
}


INLINE void pharosICoreSetState(ptrCore core , PharosCoreState state)
{
    /* set the state */
    core->state = state;
}

#endif /* COREINLINES_H */
