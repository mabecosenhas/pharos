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


#ifndef PHAROS_ROM_H
#define	PHAROS_ROM_H

/*****************************************************************************
 * Pointers to the main API tables.
 *****************************************************************************/
#define ROM_APITABLE            ((uint32_t *)0x01000010)
#define ROM_VERSION             (ROM_APITABLE[0])
#define ROM_UARTTABLE           ((uint32_t *)(ROM_APITABLE[1]))
#define ROM_SSITABLE            ((uint32_t *)(ROM_APITABLE[2]))
#define ROM_I2CTABLE            ((uint32_t *)(ROM_APITABLE[3]))
#define ROM_GPIOTABLE           ((uint32_t *)(ROM_APITABLE[4]))
#define ROM_ADCTABLE            ((uint32_t *)(ROM_APITABLE[5]))
#define ROM_COMPARATORTABLE     ((uint32_t *)(ROM_APITABLE[6]))
#define ROM_FLASHTABLE          ((uint32_t *)(ROM_APITABLE[7]))
#define ROM_PWMTABLE            ((uint32_t *)(ROM_APITABLE[8]))
#define ROM_QEITABLE            ((uint32_t *)(ROM_APITABLE[9]))
#define ROM_SYSTICKTABLE        ((uint32_t *)(ROM_APITABLE[10]))
#define ROM_TIMERTABLE          ((uint32_t *)(ROM_APITABLE[11]))
#define ROM_WATCHDOGTABLE       ((uint32_t *)(ROM_APITABLE[12]))
#define ROM_SYSCTLTABLE         ((uint32_t *)(ROM_APITABLE[13]))
#define ROM_INTERRUPTTABLE      ((uint32_t *)(ROM_APITABLE[14]))
#define ROM_USBTABLE            ((uint32_t *)(ROM_APITABLE[16]))
#define ROM_UDMATABLE           ((uint32_t *)(ROM_APITABLE[17]))
#define ROM_CANTABLE            ((uint32_t *)(ROM_APITABLE[18]))
#define ROM_HIBERNATETABLE      ((uint32_t *)(ROM_APITABLE[19]))
#define ROM_MPUTABLE            ((uint32_t *)(ROM_APITABLE[20]))
#define ROM_SOFTWARETABLE       ((uint32_t *)(ROM_APITABLE[21]))
#define ROM_EPITABLE            ((uint32_t *)(ROM_APITABLE[23]))
#define ROM_EEPROMTABLE         ((uint32_t *)(ROM_APITABLE[24]))
#define ROM_FPUTABLE            ((uint32_t *)(ROM_APITABLE[26]))
#define ROM_SMBUSTABLE          ((uint32_t *)(ROM_APITABLE[29]))
#define ROM_SYSEXCTABLE         ((uint32_t *)(ROM_APITABLE[30]))
#define ROM_ONEWIRETABLE        ((uint32_t *)(ROM_APITABLE[34]))
#define ROM_SPIFLASHTABLE       ((uint32_t *)(ROM_APITABLE[38]))
#define ROM_LCDTABLE            ((uint32_t *)(ROM_APITABLE[41]))
#define ROM_EMACTABLE           ((uint32_t *)(ROM_APITABLE[42]))
#define ROM_AESTABLE            ((uint32_t *)(ROM_APITABLE[43]))
#define ROM_CRCTABLE            ((uint32_t *)(ROM_APITABLE[44]))
#define ROM_DESTABLE            ((uint32_t *)(ROM_APITABLE[45]))
#define ROM_SHAMD5TABLE         ((uint32_t *)(ROM_APITABLE[46]))

/* PHAROS_ROM_H */
#endif	
