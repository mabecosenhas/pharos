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


#ifndef PHAROS_PARTITIONINLINE_H
#define PHAROS_PARTITIONINLINE_H


#include <pharos/kernel/core/coreconfinline.h>
#include <pharos/kernel/partition/partition.h>
#include <pharos/hal/cpu/cpu.h>
#include <pharos/hal/interpartition.h>
#include <pharos/kernel/assert/assert.h>


/**
 * Switch the thread running partition.
 * 
 * It is fundamental that this function is executed in the same context as the calling function. That is, the stack must 
 * be the same. If not, then when this function would return to the calling function, it would change the stack and when
 * the thread is restored to the pharosCpuSaveInterPartitionContext call, the stack would have been overwritten and 
 * unpredictable errors would occur. In short, the pharosCpuSaveInterPartitionContext function must be called a macro.
 * 
 * This Macro is intended to have the following equivalent function prototype:
 * 
 * FORCE_INLINE bool pharosIThreadInterPartitionSwitch(ptrThreadInfo thread , ptrPartition newPartition , 
 *                                                     ptrPartition *old , bool *wasInside , 
 *                                                     ptrPharosCpuInterPartitionContext context ,
 *                                                     ptrPharosCpuInterPartitionContext *originalContext , 
 *                                                     bool *interrupted)
 * 
 * However the FORCE_INLINE attribute (to make sure the function is always inline) does not always work and in some 
 * targets (with -O2 specially) the function is placed as a separate function. Hence the need to implement this function
 * as a macro.
 * 
 * @param thread thread to switch
 * @param newPartition new partition that the thread will be on
 * @param old old partition the thread is currently on
 * @param wasInside address of an auxiliary variable 
 * @param context context to switch to
 * @param originalContext running context
 * @param interrupted output variable that will be set to TRUE if the partition switch is successful or 
 * FALSE if the partition was restarted and this thread context restored to this function
 */
#define pharosIThreadInterPartitionSwitch(thread , newPartition , old , wasInside ,                                 \
                                          context , originalContext , interrupted)                                  \
do {                                                                                                                \
    /* interrupt level */                                                                                           \
    PharosInterruptLevel level;                                                                                     \
                                                                                                                    \
    /* get the running core */                                                                                      \
    CoreNumber core = pharosICoreRunning();                                                                         \
                                                                                                                    \
                                                                                                                    \
    /* save the original partition */                                                                               \
    *old = (thread)->runningPartition;                                                                              \
                                                                                                                    \
    /* save the original partition context */                                                                       \
    *(originalContext) = (thread)->activeContext;                                                                   \
                                                                                                                    \
    /* disable interrupts: cannot allow a restart while we are changing */                                          \
    pharosCpuDisableInterrupts(&level);                                                                             \
                                                                                                                    \
    /* check again if the new partition is initialized */                                                           \
    if((newPartition)->partitionFlags.isInitialized == FALSE)                                                       \
    {                                                                                                               \
        *(interrupted) = TRUE;                                                                                      \
    }                                                                                                               \
    else                                                                                                            \
    {                                                                                                               \
        /* replace the context */                                                                                   \
        (thread)->activeContext = (context);                                                                        \
                                                                                                                    \
        /* increment the thread inter partition nesting level */                                                    \
        pharosIThreadStateInterPartitionInc((thread));                                                              \
                                                                                                                    \
        /* if the thread is already inside the running partition list of external threads */                        \
        if(linkedListIsInside(&((thread)->runningPartition->externalThreads[core]) , (thread)) == TRUE)             \
        {                                                                                                           \
            /* then remove it (it can only belong to a single list) */                                              \
            linkedListRemoveObject(&((thread)->runningPartition->externalThreads[core]) , (thread));                \
                                                                                                                    \
            *(wasInside) = TRUE;                                                                                    \
        }                                                                                                           \
        else                                                                                                        \
        {                                                                                                           \
            *(wasInside) = FALSE;                                                                                   \
        }                                                                                                           \
                                                                                                                    \
        /* switch to the new partition */                                                                           \
        (thread)->runningPartition = (newPartition);                                                                \
                                                                                                                    \
        /* add it to the list of the new partition external threads */                                              \
        linkedListAppendLast(&(newPartition)->externalThreads[core] , (thread));                                    \
                                                                                                                    \
        /* save the inter-partition context. This is where the magic happens */                                     \
        *(interrupted) = pharosCpuSaveInterPartitionContext((context) , (thread));                                  \
    }                                                                                                               \
                                                                                                                    \
    /* re-enable interrupts */                                                                                      \
    pharosCpuEnableInterrupts(&level);                                                                              \
}while(0)                                                                                           



INLINE bool pharosIPartitionIsStarted(ptrPartition partition)
{
    /* return the "isInitialized" flag */
    return partition->partitionFlags.isInitialized;
}


/* PHAROS_PARTITIONINLINE_H */
#endif 
