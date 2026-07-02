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


#ifndef PHAROS_VERSATILE_PB_BOARD_H
#define PHAROS_VERSATILE_PB_BOARD_H

#include <pharos/hal/board/declarations.h>
#include <pharos/hal/board/conf.h>
#include <pharos/hal/board/interruptcontroller.h>
#include <pharos/hal/halapi.h>
#include <pharos/hal/cpu/cpu.h>
#include <pharos/hal/vicPl190.h>
#include <pharos/hal/cpu/linker.h>


/**
 * Address of the Ethernet controller
 */
#define  PHAROS_VERSATILE_PB_LAN91C111  ((ptrPharosLan91c111) (PHAROS_VERSATILE_PB_ETHERNET_ADDRESS))


/**
 * Address of the Vector Primary Interrupt Controller
 */
#define VIC_PIC_BASE_ADDRESS            ((ptrArmVicPl190) PHAROS_VERSATILE_PB_PIC_ADDRESS)


/**
 * Address of the Vector Secondary Interrupt Controller
 */
#define VIC_SIC_BASE_ADDRESS            ((ptrArmInterruptControllerSic) PHAROS_VERSATILE_PB_SIC_ADDRESS)


/**
 * Address of the timer 1
 */
#define PHAROS_TIMER1_BASE_ADDRESS      ((ptrArmSp804Timer) PHAROS_VERSATILE_PB_TIMER0_ADDRESS)


/**
 * Address of the timer 2
 */
#define PHAROS_TIMER2_BASE_ADDRESS      ((ptrArmSp804Timer) PHAROS_VERSATILE_PB_TIMER1_ADDRESS)


/**
 * Address of the timer 3
 */
#define PHAROS_TIMER3_BASE_ADDRESS      ((ptrArmSp804Timer) PHAROS_VERSATILE_PB_TIMER2_ADDRESS)


/**
 * Address of the timer 4
 */
#define PHAROS_TIMER4_BASE_ADDRESS      ((ptrArmSp804Timer) PHAROS_VERSATILE_PB_TIMER3_ADDRESS)


#endif /* PHAROS_VERSATILE_PB_BOARD_H */
