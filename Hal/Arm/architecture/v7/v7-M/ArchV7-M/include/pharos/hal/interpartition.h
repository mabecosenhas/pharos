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


#ifndef PHAROS_INTERPARTITION_H
#define PHAROS_INTERPARTITION_H


#include <pharos/hal/cpu/context.h>


/**
 * Make sure this call is packed to 4 bytes 
 */
#pragma pack(push , 4)

struct PharosCpuInterPartitionContext
{
    /****************************************************************************/
    /* thread state when switching to another partition context                 */
    /****************************************************************************/
    ThreadCpuBaseContext ipBaseContext;

    /* previous stack before the syscall was performed (used on the swi exception) */
    uint32_t previousSvcStack;

    /* context of the thread MPU */
    PharosCpuMpuPartitionContext mpu;
};

/**
 * restore the previous pack definition
 */
#pragma pack(pop)


/**
 * Saves the CPU low level (assembly registers) context of the running thread so that it can be restored later on.
 * 
 * @param context context where to save the running thread context
 * 
 * @return returns FALSE if the context was saved and TRUE if the thread was restored back to this point
 */
bool pharosCpuSaveInterPartitionContextLow(ptrPharosCpuInterPartitionContext context);


/**
 * Saves the CPU context of the running thread so that it can be restored later on.
 * This function must be inline (like pharosIThreadInterPartitionSwitch) since it calls 
 * pharosCpuSaveInterPartitionContextLow which must be called in the same function.
 * 
 * This function must be compiled with "-O0" flag since in Release mode it does not work! See Test1920.
 * alternatively, implement this function as a macro.
 * 
 * @param context context where to save the running thread context
 * @param thread thread to save
 * 
 * @return returns FALSE if the context was saved and TRUE if the thread was restored back to this point
 */
FORCE_INLINE bool pharosCpuSaveInterPartitionContext(ptrPharosCpuInterPartitionContext context , ptrThreadInfo thread);
 

PHAROS_OPTIMIZE_O0 FORCE_INLINE bool pharosCpuSaveInterPartitionContext(ptrPharosCpuInterPartitionContext context , ptrThreadInfo thread)
{
    /* save the MPU registers of the current thread */
    memcpy(&context->mpu , &thread->runningPartition->cpuInfo.partitionMpuContext , sizeof(PharosCpuMpuPartitionContext));

    /* save the previous stack */
    context->previousSvcStack = thread->cpuContext.previousSvcStack;

    /* save the context. If the running thread is stopped (using pharosCpuThreadFinishCall) then the thread will be 
     * restored to this point and this function will return TRUE */
    return pharosCpuSaveInterPartitionContextLow(context);
}


/* PHAROS_INTERPARTITION_H */
#endif 
