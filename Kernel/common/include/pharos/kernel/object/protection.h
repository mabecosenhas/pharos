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


#ifndef PHAROS_PARTITION_PROTECTION_H
#define PHAROS_PARTITION_PROTECTION_H


#include <pharos/kernel/partition/declarations.h>
#include <pharos/kernel/time/declarations.h>
#include <pharos/kernel/object/declarations.h>
#include <pharos/kernel/buffer/declarations.h>
#include <pharos/kernel/channel/declarations.h>
#include <pharos/kernel/memory/memory.h>


/**
 * Determines if the buffer is completely inside the boundaries of the MMU/MPU context
 * 
 * @param buffer buffer to determine
 * @param context context of the MMU/MPU
 * 
 * @return returns TRUE if the buffer is completely accessible from the context of the MMU/MPU and FALSE otherwise
 */
bool pharosICpuBufferIsInsideProt(const ptrBuffer buffer , ptrEnvMemoryProtection prot);


/**
 * Determines if the buffer is completely inside the boundaries of the MMU/MPU context
 * 
 * @param buffer buffer to determine
 * @param size size of the buffer
 * @param prot context of the MMU/MPU
 * 
 * @return returns TRUE if the buffer is completely accessible from the context of the MMU/MPU and FALSE otherwise
 */
bool pharosCpuBufferIsInsideProtection(const uint8_t *buffer , uint32_t size , const ptrEnvMemoryProtection prot);


/**
 * Determines if the string is completely inside the boundaries of the MMU/MPU context
 * 
 * @param string string to determine
 * @param prot context of the MMU/MPU
 * 
 * @return returns TRUE if the buffer is completely accessible from the context of the MMU/MPU and FALSE otherwise
 */
bool pharosCpuStringIsInsideProtection(const char *string , uint64_t maxSize , const ptrEnvMemoryProtection prot);


/**
 * Determines if the specified address is inside the boundaries of the MMU/MPU context
 * 
 * @param address address to determine
 * @param prot context of the MMU/MPU
 * 
 * @return returns TRUE if the address is accessible from the context of the MMU/MPU and FALSE otherwise
 */
bool pharosCpuAddressInsideProtection(const uint8_t *address , const ptrEnvMemoryProtection prot);


/**
 * Determines if the specified address is inside the kernel data memory space
 * 
 * @param address address to determine 
 * 
 * @return returns TRUE if the address is inside the kernel data memory space and FALSE otherwise
 */
bool pharosCpuAddressInsideKernel(const uint8_t *address);


/**
 * Determines if the specified address is inside the bulks data memory space
 * 
 * @param address address to determine 
 * 
 * @return returns TRUE if the address is inside the bulks data memory space and FALSE otherwise
 */

bool pharosCpuChannelBulkAddressValid(const ptrChannelBulkKernel address);


/**
 * Check if the specified buffer is inside the interrupt stack 
 * 
 * @param start start address of the buffer
 * @param size number of bytes
 * 
 * @return returns TRUE if the buffer is completely inside the interrupt stack and FALSE otherwise
 */
bool pharosIAddressInsideInterruptStack(const uint8_t *start , uint32_t size);

#endif /* PROTECTION_H */
