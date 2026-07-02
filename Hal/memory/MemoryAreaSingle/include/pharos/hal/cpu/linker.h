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


#ifndef PHAROS_LINKER_H
#define	PHAROS_LINKER_H


#include <pharos/hal/hal.h>
#include <pharos/kernel/object/declarations.h>

/**
 * Linker script variable whose address indicates the start of the kernel section
 */
extern uint8_t pharosKernelStart;

/**
 * Linker script variable whose address indicates the end of the kernel section
 */
extern uint8_t pharosKernelEnd;

/**
 * Linker script variable whose address indicates the start of the shared section
 */
extern uint8_t pharosSharedStart;

/**
 * Linker script variable whose address indicates the end of the shared section
 */
extern uint8_t pharosSharedEnd;

extern uint8_t pharosStartText;

extern uint8_t pharosEndText;

extern uint64_t pharosKernelDataStart;
extern uint64_t pharosKernelBssStart;
extern uint64_t pharosKernelDataEnd;
extern uint64_t pharosKernelBssEnd;

extern uint64_t pharosSharedStackStart;
extern uint64_t pharosSharedStackEnd;

extern uint64_t pharosSharedDataStart;
extern uint64_t pharosSharedBssStart;
extern uint64_t pharosSharedDataEnd;
extern uint64_t pharosSharedBssEnd;

extern uint64_t pharosKernelDataLoadAddr;
extern uint64_t pharosSharedDataLoadAddr;


extern uint8_t pharosKernelBssSize;
extern uint8_t pharosSharedBssSize;

extern uint8_t pharosKernelDataSize;
extern uint8_t pharosSharedDataSize;

extern uint64_t pharosIpCallDataLoadAddr;
extern uint8_t pharosIpCallDataSize;
extern uint8_t pharosIpCallStart;
extern uint8_t pharosIpCallEnd;
extern uint64_t pharosIpCallSectionStart;

/**
 * Linker script variable whose address indicates the start of the bulks data
 */
extern uint64_t pharosBulkStart;

/**
 * Linker script variable whose address indicates the end of the bulks data
 */
extern uint64_t pharosBulkEnd;

/**
 * Linker script variable whose address indicates the start of the read-only data
 */
extern uint8_t pharosRodataStart;

/**
 * Linker script variable whose address indicates the end of the read-only data
 */
extern uint8_t pharosRodataEnd;


/**
 * Linker script variable whose address indicates the start of the partitions
 */

/* declarations of the linker script variable with the start address of partition 0 */
extern uint8_t partition0Start;

/* declarations of the linker script variable with the start address of partition 1 */
extern uint8_t partition1Start;

/* declarations of the linker script variable with the start address of partition 2 */
extern uint8_t partition2Start;

/* declarations of the linker script variable with the start address of partition 3 */
extern uint8_t partition3Start;

/* declarations of the linker script variable with the start address of partition 4 */
extern uint8_t partition4Start;

/* declarations of the linker script variable with the start address of partition 5 */
extern uint8_t partition5Start;

/* declarations of the linker script variable with the start address of partition 6 */
extern uint8_t partition6Start;

/* declarations of the linker script variable with the start address of partition 7 */
extern uint8_t partition7Start;

/* declarations of the linker script variable with the start address of partition 8 */
extern uint8_t partition8Start;

/* declarations of the linker script variable with the start address of partition 9 */
extern uint8_t partition9Start;

/* declarations of the linker script variable with the start address of partition 10 */
extern uint8_t partition10Start;

/* declarations of the linker script variable with the start address of partition 11 */
extern uint8_t partition11Start;

/* declarations of the linker script variable with the start address of partition 12 */
extern uint8_t partition12Start;

/* declarations of the linker script variable with the start address of partition 13 */
extern uint8_t partition13Start;

/* declarations of the linker script variable with the start address of partition 14 */
extern uint8_t partition14Start;

/* declarations of the linker script variable with the start address of partition 15 */
extern uint8_t partition15Start;

/* declarations of the linker script variable with the start address of partition 16 */
extern uint8_t partition16Start;

/* declarations of the linker script variable with the start address of partition 17 */
extern uint8_t partition17Start;

/* declarations of the linker script variable with the start address of partition 18 */
extern uint8_t partition18Start;

/* declarations of the linker script variable with the start address of partition 19 */
extern uint8_t partition19Start;

/* declarations of the linker script variable with the start address of partition 20 */
extern uint8_t partition20Start;

/* declarations of the linker script variable with the start address of partition 21 */
extern uint8_t partition21Start;

/* declarations of the linker script variable with the start address of partition 22 */
extern uint8_t partition22Start;

/* declarations of the linker script variable with the start address of partition 23 */
extern uint8_t partition23Start;

/* declarations of the linker script variable with the start address of partition 24 */
extern uint8_t partition24Start;

/* declarations of the linker script variable with the start address of partition 25 */
extern uint8_t partition25Start;

/* declarations of the linker script variable with the start address of partition 26 */
extern uint8_t partition26Start;

/* declarations of the linker script variable with the start address of partition 27 */
extern uint8_t partition27Start;

/* declarations of the linker script variable with the start address of partition 28 */
extern uint8_t partition28Start;

/* declarations of the linker script variable with the start address of partition 29 */
extern uint8_t partition29Start;

/* declarations of the linker script variable with the start address of partition 30 */
extern uint8_t partition30Start;

/* declarations of the linker script variable with the start address of partition 31 */
extern uint8_t partition31Start;

/* declarations of the linker script variable with the start address of partition 32 */
extern uint8_t partition32Start;

/* declarations of the linker script variable with the start address of partition 33 */
extern uint8_t partition33Start;

/* declarations of the linker script variable with the start address of partition 34 */
extern uint8_t partition34Start;

/* declarations of the linker script variable with the start address of partition 35 */
extern uint8_t partition35Start;

/* declarations of the linker script variable with the start address of partition 36 */
extern uint8_t partition36Start;

/* declarations of the linker script variable with the start address of partition 37 */
extern uint8_t partition37Start;

/* declarations of the linker script variable with the start address of partition 38 */
extern uint8_t partition38Start;

/* declarations of the linker script variable with the start address of partition 39 */
extern uint8_t partition39Start;

/* declarations of the linker script variable with the start address of partition 40 */
extern uint8_t partition40Start;

/* declarations of the linker script variable with the start address of partition 41 */
extern uint8_t partition41Start;

/* declarations of the linker script variable with the start address of partition 42 */
extern uint8_t partition42Start;

/* declarations of the linker script variable with the start address of partition 43 */
extern uint8_t partition43Start;

/* declarations of the linker script variable with the start address of partition 44 */
extern uint8_t partition44Start;

/* declarations of the linker script variable with the start address of partition 45 */
extern uint8_t partition45Start;

/* declarations of the linker script variable with the start address of partition 46 */
extern uint8_t partition46Start;

/* declarations of the linker script variable with the start address of partition 47 */
extern uint8_t partition47Start;

/* declarations of the linker script variable with the start address of partition 48 */
extern uint8_t partition48Start;

/* declarations of the linker script variable with the start address of partition 49 */
extern uint8_t partition49Start;

/* declarations of the linker script variable with the start address of partition 50 */
extern uint8_t partition50Start;

/* declarations of the linker script variable with the start address of partition 51 */
extern uint8_t partition51Start;

/* declarations of the linker script variable with the start address of partition 52 */
extern uint8_t partition52Start;

/* declarations of the linker script variable with the start address of partition 53 */
extern uint8_t partition53Start;

/* declarations of the linker script variable with the start address of partition 54 */
extern uint8_t partition54Start;

/* declarations of the linker script variable with the start address of partition 55 */
extern uint8_t partition55Start;

/* declarations of the linker script variable with the start address of partition 56 */
extern uint8_t partition56Start;

/* declarations of the linker script variable with the start address of partition 57 */
extern uint8_t partition57Start;

/* declarations of the linker script variable with the start address of partition 58 */
extern uint8_t partition58Start;

/* declarations of the linker script variable with the start address of partition 59 */
extern uint8_t partition59Start;

/* declarations of the linker script variable with the start address of partition 60 */
extern uint8_t partition60Start;

/* declarations of the linker script variable with the start address of partition 61 */
extern uint8_t partition61Start;

/* declarations of the linker script variable with the start address of partition 62 */
extern uint8_t partition62Start;

/* declarations of the linker script variable with the start address of partition 63 */
extern uint8_t partition63Start;


/**
 * Linker script variable whose address indicates the end of the partitions
 */


/* declarations of the linker script variable with the end address of partition 0 */
extern uint8_t partition0End;

/* declarations of the linker script variable with the end address of partition 1 */
extern uint8_t partition1End;

/* declarations of the linker script variable with the end address of partition 2 */
extern uint8_t partition2End;

/* declarations of the linker script variable with the end address of partition 3 */
extern uint8_t partition3End;

/* declarations of the linker script variable with the end address of partition 4 */
extern uint8_t partition4End;

/* declarations of the linker script variable with the end address of partition 5 */
extern uint8_t partition5End;

/* declarations of the linker script variable with the end address of partition 6 */
extern uint8_t partition6End;

/* declarations of the linker script variable with the end address of partition 7 */
extern uint8_t partition7End;

/* declarations of the linker script variable with the end address of partition 8 */
extern uint8_t partition8End;

/* declarations of the linker script variable with the end address of partition 9 */
extern uint8_t partition9End;

/* declarations of the linker script variable with the end address of partition 10 */
extern uint8_t partition10End;

/* declarations of the linker script variable with the end address of partition 11 */
extern uint8_t partition11End;

/* declarations of the linker script variable with the end address of partition 12 */
extern uint8_t partition12End;

/* declarations of the linker script variable with the end address of partition 13 */
extern uint8_t partition13End;

/* declarations of the linker script variable with the end address of partition 14 */
extern uint8_t partition14End;

/* declarations of the linker script variable with the end address of partition 15 */
extern uint8_t partition15End;

/* declarations of the linker script variable with the end address of partition 16 */
extern uint8_t partition16End;

/* declarations of the linker script variable with the end address of partition 17 */
extern uint8_t partition17End;

/* declarations of the linker script variable with the end address of partition 18 */
extern uint8_t partition18End;

/* declarations of the linker script variable with the end address of partition 19 */
extern uint8_t partition19End;

/* declarations of the linker script variable with the end address of partition 20 */
extern uint8_t partition20End;

/* declarations of the linker script variable with the end address of partition 21 */
extern uint8_t partition21End;

/* declarations of the linker script variable with the end address of partition 22 */
extern uint8_t partition22End;

/* declarations of the linker script variable with the end address of partition 23 */
extern uint8_t partition23End;

/* declarations of the linker script variable with the end address of partition 24 */
extern uint8_t partition24End;

/* declarations of the linker script variable with the end address of partition 25 */
extern uint8_t partition25End;

/* declarations of the linker script variable with the end address of partition 26 */
extern uint8_t partition26End;

/* declarations of the linker script variable with the end address of partition 27 */
extern uint8_t partition27End;

/* declarations of the linker script variable with the end address of partition 28 */
extern uint8_t partition28End;

/* declarations of the linker script variable with the end address of partition 29 */
extern uint8_t partition29End;

/* declarations of the linker script variable with the end address of partition 30 */
extern uint8_t partition30End;

/* declarations of the linker script variable with the end address of partition 31 */
extern uint8_t partition31End;

/* declarations of the linker script variable with the end address of partition 32 */
extern uint8_t partition32End;

/* declarations of the linker script variable with the end address of partition 33 */
extern uint8_t partition33End;

/* declarations of the linker script variable with the end address of partition 34 */
extern uint8_t partition34End;

/* declarations of the linker script variable with the end address of partition 35 */
extern uint8_t partition35End;

/* declarations of the linker script variable with the end address of partition 36 */
extern uint8_t partition36End;

/* declarations of the linker script variable with the end address of partition 37 */
extern uint8_t partition37End;

/* declarations of the linker script variable with the end address of partition 38 */
extern uint8_t partition38End;

/* declarations of the linker script variable with the end address of partition 39 */
extern uint8_t partition39End;

/* declarations of the linker script variable with the end address of partition 40 */
extern uint8_t partition40End;

/* declarations of the linker script variable with the end address of partition 41 */
extern uint8_t partition41End;

/* declarations of the linker script variable with the end address of partition 42 */
extern uint8_t partition42End;

/* declarations of the linker script variable with the end address of partition 43 */
extern uint8_t partition43End;

/* declarations of the linker script variable with the end address of partition 44 */
extern uint8_t partition44End;

/* declarations of the linker script variable with the end address of partition 45 */
extern uint8_t partition45End;

/* declarations of the linker script variable with the end address of partition 46 */
extern uint8_t partition46End;

/* declarations of the linker script variable with the end address of partition 47 */
extern uint8_t partition47End;

/* declarations of the linker script variable with the end address of partition 48 */
extern uint8_t partition48End;

/* declarations of the linker script variable with the end address of partition 49 */
extern uint8_t partition49End;

/* declarations of the linker script variable with the end address of partition 50 */
extern uint8_t partition50End;

/* declarations of the linker script variable with the end address of partition 51 */
extern uint8_t partition51End;

/* declarations of the linker script variable with the end address of partition 52 */
extern uint8_t partition52End;

/* declarations of the linker script variable with the end address of partition 53 */
extern uint8_t partition53End;

/* declarations of the linker script variable with the end address of partition 54 */
extern uint8_t partition54End;

/* declarations of the linker script variable with the end address of partition 55 */
extern uint8_t partition55End;

/* declarations of the linker script variable with the end address of partition 56 */
extern uint8_t partition56End;

/* declarations of the linker script variable with the end address of partition 57 */
extern uint8_t partition57End;

/* declarations of the linker script variable with the end address of partition 58 */
extern uint8_t partition58End;

/* declarations of the linker script variable with the end address of partition 59 */
extern uint8_t partition59End;

/* declarations of the linker script variable with the end address of partition 60 */
extern uint8_t partition60End;

/* declarations of the linker script variable with the end address of partition 61 */
extern uint8_t partition61End;

/* declarations of the linker script variable with the end address of partition 62 */
extern uint8_t partition62End;

/* declarations of the linker script variable with the end address of partition 63 */
extern uint8_t partition63End;

/**
 * Linker script variable whose address indicates the load address of the data sections
 */


/* declarations of the linker script variable with the load start address of partition 0 data */
extern uint64_t pharosPartition0DataAddr;

/* declarations of the linker script variable with the load start address of partition 1 data */
extern uint64_t pharosPartition1DataAddr;

/* declarations of the linker script variable with the load start address of partition 2 data */
extern uint64_t pharosPartition2DataAddr;

/* declarations of the linker script variable with the load start address of partition 3 data */
extern uint64_t pharosPartition3DataAddr;

/* declarations of the linker script variable with the load start address of partition 4 data */
extern uint64_t pharosPartition4DataAddr;

/* declarations of the linker script variable with the load start address of partition 5 data */
extern uint64_t pharosPartition5DataAddr;

/* declarations of the linker script variable with the load start address of partition 6 data */
extern uint64_t pharosPartition6DataAddr;

/* declarations of the linker script variable with the load start address of partition 7 data */
extern uint64_t pharosPartition7DataAddr;

/* declarations of the linker script variable with the load start address of partition 8 data */
extern uint64_t pharosPartition8DataAddr;

/* declarations of the linker script variable with the load start address of partition 9 data */
extern uint64_t pharosPartition9DataAddr;

/* declarations of the linker script variable with the load start address of partition 10 data */
extern uint64_t pharosPartition10DataAddr;

/* declarations of the linker script variable with the load start address of partition 11 data */
extern uint64_t pharosPartition11DataAddr;

/* declarations of the linker script variable with the load start address of partition 12 data */
extern uint64_t pharosPartition12DataAddr;

/* declarations of the linker script variable with the load start address of partition 13 data */
extern uint64_t pharosPartition13DataAddr;

/* declarations of the linker script variable with the load start address of partition 14 data */
extern uint64_t pharosPartition14DataAddr;

/* declarations of the linker script variable with the load start address of partition 15 data */
extern uint64_t pharosPartition15DataAddr;

/* declarations of the linker script variable with the load start address of partition 16 data */
extern uint64_t pharosPartition16DataAddr;

/* declarations of the linker script variable with the load start address of partition 17 data */
extern uint64_t pharosPartition17DataAddr;

/* declarations of the linker script variable with the load start address of partition 18 data */
extern uint64_t pharosPartition18DataAddr;

/* declarations of the linker script variable with the load start address of partition 19 data */
extern uint64_t pharosPartition19DataAddr;

/* declarations of the linker script variable with the load start address of partition 20 data */
extern uint64_t pharosPartition20DataAddr;

/* declarations of the linker script variable with the load start address of partition 21 data */
extern uint64_t pharosPartition21DataAddr;

/* declarations of the linker script variable with the load start address of partition 22 data */
extern uint64_t pharosPartition22DataAddr;

/* declarations of the linker script variable with the load start address of partition 23 data */
extern uint64_t pharosPartition23DataAddr;

/* declarations of the linker script variable with the load start address of partition 24 data */
extern uint64_t pharosPartition24DataAddr;

/* declarations of the linker script variable with the load start address of partition 25 data */
extern uint64_t pharosPartition25DataAddr;

/* declarations of the linker script variable with the load start address of partition 26 data */
extern uint64_t pharosPartition26DataAddr;

/* declarations of the linker script variable with the load start address of partition 27 data */
extern uint64_t pharosPartition27DataAddr;

/* declarations of the linker script variable with the load start address of partition 28 data */
extern uint64_t pharosPartition28DataAddr;

/* declarations of the linker script variable with the load start address of partition 29 data */
extern uint64_t pharosPartition29DataAddr;

/* declarations of the linker script variable with the load start address of partition 30 data */
extern uint64_t pharosPartition30DataAddr;

/* declarations of the linker script variable with the load start address of partition 31 data */
extern uint64_t pharosPartition31DataAddr;

/* declarations of the linker script variable with the load start address of partition 32 data */
extern uint64_t pharosPartition32DataAddr;

/* declarations of the linker script variable with the load start address of partition 33 data */
extern uint64_t pharosPartition33DataAddr;

/* declarations of the linker script variable with the load start address of partition 34 data */
extern uint64_t pharosPartition34DataAddr;

/* declarations of the linker script variable with the load start address of partition 35 data */
extern uint64_t pharosPartition35DataAddr;

/* declarations of the linker script variable with the load start address of partition 36 data */
extern uint64_t pharosPartition36DataAddr;

/* declarations of the linker script variable with the load start address of partition 37 data */
extern uint64_t pharosPartition37DataAddr;

/* declarations of the linker script variable with the load start address of partition 38 data */
extern uint64_t pharosPartition38DataAddr;

/* declarations of the linker script variable with the load start address of partition 39 data */
extern uint64_t pharosPartition39DataAddr;

/* declarations of the linker script variable with the load start address of partition 40 data */
extern uint64_t pharosPartition40DataAddr;

/* declarations of the linker script variable with the load start address of partition 41 data */
extern uint64_t pharosPartition41DataAddr;

/* declarations of the linker script variable with the load start address of partition 42 data */
extern uint64_t pharosPartition42DataAddr;

/* declarations of the linker script variable with the load start address of partition 43 data */
extern uint64_t pharosPartition43DataAddr;

/* declarations of the linker script variable with the load start address of partition 44 data */
extern uint64_t pharosPartition44DataAddr;

/* declarations of the linker script variable with the load start address of partition 45 data */
extern uint64_t pharosPartition45DataAddr;

/* declarations of the linker script variable with the load start address of partition 46 data */
extern uint64_t pharosPartition46DataAddr;

/* declarations of the linker script variable with the load start address of partition 47 data */
extern uint64_t pharosPartition47DataAddr;

/* declarations of the linker script variable with the load start address of partition 48 data */
extern uint64_t pharosPartition48DataAddr;

/* declarations of the linker script variable with the load start address of partition 49 data */
extern uint64_t pharosPartition49DataAddr;

/* declarations of the linker script variable with the load start address of partition 50 data */
extern uint64_t pharosPartition50DataAddr;

/* declarations of the linker script variable with the load start address of partition 51 data */
extern uint64_t pharosPartition51DataAddr;

/* declarations of the linker script variable with the load start address of partition 52 data */
extern uint64_t pharosPartition52DataAddr;

/* declarations of the linker script variable with the load start address of partition 53 data */
extern uint64_t pharosPartition53DataAddr;

/* declarations of the linker script variable with the load start address of partition 54 data */
extern uint64_t pharosPartition54DataAddr;

/* declarations of the linker script variable with the load start address of partition 55 data */
extern uint64_t pharosPartition55DataAddr;

/* declarations of the linker script variable with the load start address of partition 56 data */
extern uint64_t pharosPartition56DataAddr;

/* declarations of the linker script variable with the load start address of partition 57 data */
extern uint64_t pharosPartition57DataAddr;

/* declarations of the linker script variable with the load start address of partition 58 data */
extern uint64_t pharosPartition58DataAddr;

/* declarations of the linker script variable with the load start address of partition 59 data */
extern uint64_t pharosPartition59DataAddr;

/* declarations of the linker script variable with the load start address of partition 60 data */
extern uint64_t pharosPartition60DataAddr;

/* declarations of the linker script variable with the load start address of partition 61 data */
extern uint64_t pharosPartition61DataAddr;

/* declarations of the linker script variable with the load start address of partition 62 data */
extern uint64_t pharosPartition62DataAddr;

/* declarations of the linker script variable with the load start address of partition 63 data */
extern uint64_t pharosPartition63DataAddr;


/**
 * Linker script variable whose address indicates the data start of the partitions
 */



/* declarations of the linker script variable with the start address of partition 0 data */
extern uint64_t partition0DataStart;

/* declarations of the linker script variable with the start address of partition 1 data */
extern uint64_t partition1DataStart;

/* declarations of the linker script variable with the start address of partition 2 data */
extern uint64_t partition2DataStart;

/* declarations of the linker script variable with the start address of partition 3 data */
extern uint64_t partition3DataStart;

/* declarations of the linker script variable with the start address of partition 4 data */
extern uint64_t partition4DataStart;

/* declarations of the linker script variable with the start address of partition 5 data */
extern uint64_t partition5DataStart;

/* declarations of the linker script variable with the start address of partition 6 data */
extern uint64_t partition6DataStart;

/* declarations of the linker script variable with the start address of partition 7 data */
extern uint64_t partition7DataStart;

/* declarations of the linker script variable with the start address of partition 8 data */
extern uint64_t partition8DataStart;

/* declarations of the linker script variable with the start address of partition 9 data */
extern uint64_t partition9DataStart;

/* declarations of the linker script variable with the start address of partition 10 data */
extern uint64_t partition10DataStart;

/* declarations of the linker script variable with the start address of partition 11 data */
extern uint64_t partition11DataStart;

/* declarations of the linker script variable with the start address of partition 12 data */
extern uint64_t partition12DataStart;

/* declarations of the linker script variable with the start address of partition 13 data */
extern uint64_t partition13DataStart;

/* declarations of the linker script variable with the start address of partition 14 data */
extern uint64_t partition14DataStart;

/* declarations of the linker script variable with the start address of partition 15 data */
extern uint64_t partition15DataStart;

/* declarations of the linker script variable with the start address of partition 16 data */
extern uint64_t partition16DataStart;

/* declarations of the linker script variable with the start address of partition 17 data */
extern uint64_t partition17DataStart;

/* declarations of the linker script variable with the start address of partition 18 data */
extern uint64_t partition18DataStart;

/* declarations of the linker script variable with the start address of partition 19 data */
extern uint64_t partition19DataStart;

/* declarations of the linker script variable with the start address of partition 20 data */
extern uint64_t partition20DataStart;

/* declarations of the linker script variable with the start address of partition 21 data */
extern uint64_t partition21DataStart;

/* declarations of the linker script variable with the start address of partition 22 data */
extern uint64_t partition22DataStart;

/* declarations of the linker script variable with the start address of partition 23 data */
extern uint64_t partition23DataStart;

/* declarations of the linker script variable with the start address of partition 24 data */
extern uint64_t partition24DataStart;

/* declarations of the linker script variable with the start address of partition 25 data */
extern uint64_t partition25DataStart;

/* declarations of the linker script variable with the start address of partition 26 data */
extern uint64_t partition26DataStart;

/* declarations of the linker script variable with the start address of partition 27 data */
extern uint64_t partition27DataStart;

/* declarations of the linker script variable with the start address of partition 28 data */
extern uint64_t partition28DataStart;

/* declarations of the linker script variable with the start address of partition 29 data */
extern uint64_t partition29DataStart;

/* declarations of the linker script variable with the start address of partition 30 data */
extern uint64_t partition30DataStart;

/* declarations of the linker script variable with the start address of partition 31 data */
extern uint64_t partition31DataStart;

/* declarations of the linker script variable with the start address of partition 32 data */
extern uint64_t partition32DataStart;

/* declarations of the linker script variable with the start address of partition 33 data */
extern uint64_t partition33DataStart;

/* declarations of the linker script variable with the start address of partition 34 data */
extern uint64_t partition34DataStart;

/* declarations of the linker script variable with the start address of partition 35 data */
extern uint64_t partition35DataStart;

/* declarations of the linker script variable with the start address of partition 36 data */
extern uint64_t partition36DataStart;

/* declarations of the linker script variable with the start address of partition 37 data */
extern uint64_t partition37DataStart;

/* declarations of the linker script variable with the start address of partition 38 data */
extern uint64_t partition38DataStart;

/* declarations of the linker script variable with the start address of partition 39 data */
extern uint64_t partition39DataStart;

/* declarations of the linker script variable with the start address of partition 40 data */
extern uint64_t partition40DataStart;

/* declarations of the linker script variable with the start address of partition 41 data */
extern uint64_t partition41DataStart;

/* declarations of the linker script variable with the start address of partition 42 data */
extern uint64_t partition42DataStart;

/* declarations of the linker script variable with the start address of partition 43 data */
extern uint64_t partition43DataStart;

/* declarations of the linker script variable with the start address of partition 44 data */
extern uint64_t partition44DataStart;

/* declarations of the linker script variable with the start address of partition 45 data */
extern uint64_t partition45DataStart;

/* declarations of the linker script variable with the start address of partition 46 data */
extern uint64_t partition46DataStart;

/* declarations of the linker script variable with the start address of partition 47 data */
extern uint64_t partition47DataStart;

/* declarations of the linker script variable with the start address of partition 48 data */
extern uint64_t partition48DataStart;

/* declarations of the linker script variable with the start address of partition 49 data */
extern uint64_t partition49DataStart;

/* declarations of the linker script variable with the start address of partition 50 data */
extern uint64_t partition50DataStart;

/* declarations of the linker script variable with the start address of partition 51 data */
extern uint64_t partition51DataStart;

/* declarations of the linker script variable with the start address of partition 52 data */
extern uint64_t partition52DataStart;

/* declarations of the linker script variable with the start address of partition 53 data */
extern uint64_t partition53DataStart;

/* declarations of the linker script variable with the start address of partition 54 data */
extern uint64_t partition54DataStart;

/* declarations of the linker script variable with the start address of partition 55 data */
extern uint64_t partition55DataStart;

/* declarations of the linker script variable with the start address of partition 56 data */
extern uint64_t partition56DataStart;

/* declarations of the linker script variable with the start address of partition 57 data */
extern uint64_t partition57DataStart;

/* declarations of the linker script variable with the start address of partition 58 data */
extern uint64_t partition58DataStart;

/* declarations of the linker script variable with the start address of partition 59 data */
extern uint64_t partition59DataStart;

/* declarations of the linker script variable with the start address of partition 60 data */
extern uint64_t partition60DataStart;

/* declarations of the linker script variable with the start address of partition 61 data */
extern uint64_t partition61DataStart;

/* declarations of the linker script variable with the start address of partition 62 data */
extern uint64_t partition62DataStart;

/* declarations of the linker script variable with the start address of partition 63 data */
extern uint64_t partition63DataStart;


/**
 * Linker script variable whose address indicates the data end of the partitions
 */


/* declarations of the linker script variable with the end address of partition 0 data */
extern uint64_t partition0DataEnd;

/* declarations of the linker script variable with the end address of partition 1 data */
extern uint64_t partition1DataEnd;

/* declarations of the linker script variable with the end address of partition 2 data */
extern uint64_t partition2DataEnd;

/* declarations of the linker script variable with the end address of partition 3 data */
extern uint64_t partition3DataEnd;

/* declarations of the linker script variable with the end address of partition 4 data */
extern uint64_t partition4DataEnd;

/* declarations of the linker script variable with the end address of partition 5 data */
extern uint64_t partition5DataEnd;

/* declarations of the linker script variable with the end address of partition 6 data */
extern uint64_t partition6DataEnd;

/* declarations of the linker script variable with the end address of partition 7 data */
extern uint64_t partition7DataEnd;

/* declarations of the linker script variable with the end address of partition 8 data */
extern uint64_t partition8DataEnd;

/* declarations of the linker script variable with the end address of partition 9 data */
extern uint64_t partition9DataEnd;

/* declarations of the linker script variable with the end address of partition 10 data */
extern uint64_t partition10DataEnd;

/* declarations of the linker script variable with the end address of partition 11 data */
extern uint64_t partition11DataEnd;

/* declarations of the linker script variable with the end address of partition 12 data */
extern uint64_t partition12DataEnd;

/* declarations of the linker script variable with the end address of partition 13 data */
extern uint64_t partition13DataEnd;

/* declarations of the linker script variable with the end address of partition 14 data */
extern uint64_t partition14DataEnd;

/* declarations of the linker script variable with the end address of partition 15 data */
extern uint64_t partition15DataEnd;

/* declarations of the linker script variable with the end address of partition 16 data */
extern uint64_t partition16DataEnd;

/* declarations of the linker script variable with the end address of partition 17 data */
extern uint64_t partition17DataEnd;

/* declarations of the linker script variable with the end address of partition 18 data */
extern uint64_t partition18DataEnd;

/* declarations of the linker script variable with the end address of partition 19 data */
extern uint64_t partition19DataEnd;

/* declarations of the linker script variable with the end address of partition 20 data */
extern uint64_t partition20DataEnd;

/* declarations of the linker script variable with the end address of partition 21 data */
extern uint64_t partition21DataEnd;

/* declarations of the linker script variable with the end address of partition 22 data */
extern uint64_t partition22DataEnd;

/* declarations of the linker script variable with the end address of partition 23 data */
extern uint64_t partition23DataEnd;

/* declarations of the linker script variable with the end address of partition 24 data */
extern uint64_t partition24DataEnd;

/* declarations of the linker script variable with the end address of partition 25 data */
extern uint64_t partition25DataEnd;

/* declarations of the linker script variable with the end address of partition 26 data */
extern uint64_t partition26DataEnd;

/* declarations of the linker script variable with the end address of partition 27 data */
extern uint64_t partition27DataEnd;

/* declarations of the linker script variable with the end address of partition 28 data */
extern uint64_t partition28DataEnd;

/* declarations of the linker script variable with the end address of partition 29 data */
extern uint64_t partition29DataEnd;

/* declarations of the linker script variable with the end address of partition 30 data */
extern uint64_t partition30DataEnd;

/* declarations of the linker script variable with the end address of partition 31 data */
extern uint64_t partition31DataEnd;

/* declarations of the linker script variable with the end address of partition 32 data */
extern uint64_t partition32DataEnd;

/* declarations of the linker script variable with the end address of partition 33 data */
extern uint64_t partition33DataEnd;

/* declarations of the linker script variable with the end address of partition 34 data */
extern uint64_t partition34DataEnd;

/* declarations of the linker script variable with the end address of partition 35 data */
extern uint64_t partition35DataEnd;

/* declarations of the linker script variable with the end address of partition 36 data */
extern uint64_t partition36DataEnd;

/* declarations of the linker script variable with the end address of partition 37 data */
extern uint64_t partition37DataEnd;

/* declarations of the linker script variable with the end address of partition 38 data */
extern uint64_t partition38DataEnd;

/* declarations of the linker script variable with the end address of partition 39 data */
extern uint64_t partition39DataEnd;

/* declarations of the linker script variable with the end address of partition 40 data */
extern uint64_t partition40DataEnd;

/* declarations of the linker script variable with the end address of partition 41 data */
extern uint64_t partition41DataEnd;

/* declarations of the linker script variable with the end address of partition 42 data */
extern uint64_t partition42DataEnd;

/* declarations of the linker script variable with the end address of partition 43 data */
extern uint64_t partition43DataEnd;

/* declarations of the linker script variable with the end address of partition 44 data */
extern uint64_t partition44DataEnd;

/* declarations of the linker script variable with the end address of partition 45 data */
extern uint64_t partition45DataEnd;

/* declarations of the linker script variable with the end address of partition 46 data */
extern uint64_t partition46DataEnd;

/* declarations of the linker script variable with the end address of partition 47 data */
extern uint64_t partition47DataEnd;

/* declarations of the linker script variable with the end address of partition 48 data */
extern uint64_t partition48DataEnd;

/* declarations of the linker script variable with the end address of partition 49 data */
extern uint64_t partition49DataEnd;

/* declarations of the linker script variable with the end address of partition 50 data */
extern uint64_t partition50DataEnd;

/* declarations of the linker script variable with the end address of partition 51 data */
extern uint64_t partition51DataEnd;

/* declarations of the linker script variable with the end address of partition 52 data */
extern uint64_t partition52DataEnd;

/* declarations of the linker script variable with the end address of partition 53 data */
extern uint64_t partition53DataEnd;

/* declarations of the linker script variable with the end address of partition 54 data */
extern uint64_t partition54DataEnd;

/* declarations of the linker script variable with the end address of partition 55 data */
extern uint64_t partition55DataEnd;

/* declarations of the linker script variable with the end address of partition 56 data */
extern uint64_t partition56DataEnd;

/* declarations of the linker script variable with the end address of partition 57 data */
extern uint64_t partition57DataEnd;

/* declarations of the linker script variable with the end address of partition 58 data */
extern uint64_t partition58DataEnd;

/* declarations of the linker script variable with the end address of partition 59 data */
extern uint64_t partition59DataEnd;

/* declarations of the linker script variable with the end address of partition 60 data */
extern uint64_t partition60DataEnd;

/* declarations of the linker script variable with the end address of partition 61 data */
extern uint64_t partition61DataEnd;

/* declarations of the linker script variable with the end address of partition 62 data */
extern uint64_t partition62DataEnd;

/* declarations of the linker script variable with the end address of partition 63 data */
extern uint64_t partition63DataEnd;


/**
 * Linker script variable whose address indicates the bss start of the partitions
 */


/* declarations of the linker script variable with the start address of partition 0 bss */
extern uint64_t partition0BssStart;

/* declarations of the linker script variable with the start address of partition 1 bss */
extern uint64_t partition1BssStart;

/* declarations of the linker script variable with the start address of partition 2 bss */
extern uint64_t partition2BssStart;

/* declarations of the linker script variable with the start address of partition 3 bss */
extern uint64_t partition3BssStart;

/* declarations of the linker script variable with the start address of partition 4 bss */
extern uint64_t partition4BssStart;

/* declarations of the linker script variable with the start address of partition 5 bss */
extern uint64_t partition5BssStart;

/* declarations of the linker script variable with the start address of partition 6 bss */
extern uint64_t partition6BssStart;

/* declarations of the linker script variable with the start address of partition 7 bss */
extern uint64_t partition7BssStart;

/* declarations of the linker script variable with the start address of partition 8 bss */
extern uint64_t partition8BssStart;

/* declarations of the linker script variable with the start address of partition 9 bss */
extern uint64_t partition9BssStart;

/* declarations of the linker script variable with the start address of partition 10 bss */
extern uint64_t partition10BssStart;

/* declarations of the linker script variable with the start address of partition 11 bss */
extern uint64_t partition11BssStart;

/* declarations of the linker script variable with the start address of partition 12 bss */
extern uint64_t partition12BssStart;

/* declarations of the linker script variable with the start address of partition 13 bss */
extern uint64_t partition13BssStart;

/* declarations of the linker script variable with the start address of partition 14 bss */
extern uint64_t partition14BssStart;

/* declarations of the linker script variable with the start address of partition 15 bss */
extern uint64_t partition15BssStart;

/* declarations of the linker script variable with the start address of partition 16 bss */
extern uint64_t partition16BssStart;

/* declarations of the linker script variable with the start address of partition 17 bss */
extern uint64_t partition17BssStart;

/* declarations of the linker script variable with the start address of partition 18 bss */
extern uint64_t partition18BssStart;

/* declarations of the linker script variable with the start address of partition 19 bss */
extern uint64_t partition19BssStart;

/* declarations of the linker script variable with the start address of partition 20 bss */
extern uint64_t partition20BssStart;

/* declarations of the linker script variable with the start address of partition 21 bss */
extern uint64_t partition21BssStart;

/* declarations of the linker script variable with the start address of partition 22 bss */
extern uint64_t partition22BssStart;

/* declarations of the linker script variable with the start address of partition 23 bss */
extern uint64_t partition23BssStart;

/* declarations of the linker script variable with the start address of partition 24 bss */
extern uint64_t partition24BssStart;

/* declarations of the linker script variable with the start address of partition 25 bss */
extern uint64_t partition25BssStart;

/* declarations of the linker script variable with the start address of partition 26 bss */
extern uint64_t partition26BssStart;

/* declarations of the linker script variable with the start address of partition 27 bss */
extern uint64_t partition27BssStart;

/* declarations of the linker script variable with the start address of partition 28 bss */
extern uint64_t partition28BssStart;

/* declarations of the linker script variable with the start address of partition 29 bss */
extern uint64_t partition29BssStart;

/* declarations of the linker script variable with the start address of partition 30 bss */
extern uint64_t partition30BssStart;

/* declarations of the linker script variable with the start address of partition 31 bss */
extern uint64_t partition31BssStart;

/* declarations of the linker script variable with the start address of partition 32 bss */
extern uint64_t partition32BssStart;

/* declarations of the linker script variable with the start address of partition 33 bss */
extern uint64_t partition33BssStart;

/* declarations of the linker script variable with the start address of partition 34 bss */
extern uint64_t partition34BssStart;

/* declarations of the linker script variable with the start address of partition 35 bss */
extern uint64_t partition35BssStart;

/* declarations of the linker script variable with the start address of partition 36 bss */
extern uint64_t partition36BssStart;

/* declarations of the linker script variable with the start address of partition 37 bss */
extern uint64_t partition37BssStart;

/* declarations of the linker script variable with the start address of partition 38 bss */
extern uint64_t partition38BssStart;

/* declarations of the linker script variable with the start address of partition 39 bss */
extern uint64_t partition39BssStart;

/* declarations of the linker script variable with the start address of partition 40 bss */
extern uint64_t partition40BssStart;

/* declarations of the linker script variable with the start address of partition 41 bss */
extern uint64_t partition41BssStart;

/* declarations of the linker script variable with the start address of partition 42 bss */
extern uint64_t partition42BssStart;

/* declarations of the linker script variable with the start address of partition 43 bss */
extern uint64_t partition43BssStart;

/* declarations of the linker script variable with the start address of partition 44 bss */
extern uint64_t partition44BssStart;

/* declarations of the linker script variable with the start address of partition 45 bss */
extern uint64_t partition45BssStart;

/* declarations of the linker script variable with the start address of partition 46 bss */
extern uint64_t partition46BssStart;

/* declarations of the linker script variable with the start address of partition 47 bss */
extern uint64_t partition47BssStart;

/* declarations of the linker script variable with the start address of partition 48 bss */
extern uint64_t partition48BssStart;

/* declarations of the linker script variable with the start address of partition 49 bss */
extern uint64_t partition49BssStart;

/* declarations of the linker script variable with the start address of partition 50 bss */
extern uint64_t partition50BssStart;

/* declarations of the linker script variable with the start address of partition 51 bss */
extern uint64_t partition51BssStart;

/* declarations of the linker script variable with the start address of partition 52 bss */
extern uint64_t partition52BssStart;

/* declarations of the linker script variable with the start address of partition 53 bss */
extern uint64_t partition53BssStart;

/* declarations of the linker script variable with the start address of partition 54 bss */
extern uint64_t partition54BssStart;

/* declarations of the linker script variable with the start address of partition 55 bss */
extern uint64_t partition55BssStart;

/* declarations of the linker script variable with the start address of partition 56 bss */
extern uint64_t partition56BssStart;

/* declarations of the linker script variable with the start address of partition 57 bss */
extern uint64_t partition57BssStart;

/* declarations of the linker script variable with the start address of partition 58 bss */
extern uint64_t partition58BssStart;

/* declarations of the linker script variable with the start address of partition 59 bss */
extern uint64_t partition59BssStart;

/* declarations of the linker script variable with the start address of partition 60 bss */
extern uint64_t partition60BssStart;

/* declarations of the linker script variable with the start address of partition 61 bss */
extern uint64_t partition61BssStart;

/* declarations of the linker script variable with the start address of partition 62 bss */
extern uint64_t partition62BssStart;

/* declarations of the linker script variable with the start address of partition 63 bss */
extern uint64_t partition63BssStart;

/**
 * Linker script variable whose address indicates the bss end of the partitions
 */


/* declarations of the linker script variable with the end address of partition 0 bss */
extern uint64_t partition0BssEnd;

/* declarations of the linker script variable with the end address of partition 1 bss */
extern uint64_t partition1BssEnd;

/* declarations of the linker script variable with the end address of partition 2 bss */
extern uint64_t partition2BssEnd;

/* declarations of the linker script variable with the end address of partition 3 bss */
extern uint64_t partition3BssEnd;

/* declarations of the linker script variable with the end address of partition 4 bss */
extern uint64_t partition4BssEnd;

/* declarations of the linker script variable with the end address of partition 5 bss */
extern uint64_t partition5BssEnd;

/* declarations of the linker script variable with the end address of partition 6 bss */
extern uint64_t partition6BssEnd;

/* declarations of the linker script variable with the end address of partition 7 bss */
extern uint64_t partition7BssEnd;

/* declarations of the linker script variable with the end address of partition 8 bss */
extern uint64_t partition8BssEnd;

/* declarations of the linker script variable with the end address of partition 9 bss */
extern uint64_t partition9BssEnd;

/* declarations of the linker script variable with the end address of partition 10 bss */
extern uint64_t partition10BssEnd;

/* declarations of the linker script variable with the end address of partition 11 bss */
extern uint64_t partition11BssEnd;

/* declarations of the linker script variable with the end address of partition 12 bss */
extern uint64_t partition12BssEnd;

/* declarations of the linker script variable with the end address of partition 13 bss */
extern uint64_t partition13BssEnd;

/* declarations of the linker script variable with the end address of partition 14 bss */
extern uint64_t partition14BssEnd;

/* declarations of the linker script variable with the end address of partition 15 bss */
extern uint64_t partition15BssEnd;

/* declarations of the linker script variable with the end address of partition 16 bss */
extern uint64_t partition16BssEnd;

/* declarations of the linker script variable with the end address of partition 17 bss */
extern uint64_t partition17BssEnd;

/* declarations of the linker script variable with the end address of partition 18 bss */
extern uint64_t partition18BssEnd;

/* declarations of the linker script variable with the end address of partition 19 bss */
extern uint64_t partition19BssEnd;

/* declarations of the linker script variable with the end address of partition 20 bss */
extern uint64_t partition20BssEnd;

/* declarations of the linker script variable with the end address of partition 21 bss */
extern uint64_t partition21BssEnd;

/* declarations of the linker script variable with the end address of partition 22 bss */
extern uint64_t partition22BssEnd;

/* declarations of the linker script variable with the end address of partition 23 bss */
extern uint64_t partition23BssEnd;

/* declarations of the linker script variable with the end address of partition 24 bss */
extern uint64_t partition24BssEnd;

/* declarations of the linker script variable with the end address of partition 25 bss */
extern uint64_t partition25BssEnd;

/* declarations of the linker script variable with the end address of partition 26 bss */
extern uint64_t partition26BssEnd;

/* declarations of the linker script variable with the end address of partition 27 bss */
extern uint64_t partition27BssEnd;

/* declarations of the linker script variable with the end address of partition 28 bss */
extern uint64_t partition28BssEnd;

/* declarations of the linker script variable with the end address of partition 29 bss */
extern uint64_t partition29BssEnd;

/* declarations of the linker script variable with the end address of partition 30 bss */
extern uint64_t partition30BssEnd;

/* declarations of the linker script variable with the end address of partition 31 bss */
extern uint64_t partition31BssEnd;

/* declarations of the linker script variable with the end address of partition 32 bss */
extern uint64_t partition32BssEnd;

/* declarations of the linker script variable with the end address of partition 33 bss */
extern uint64_t partition33BssEnd;

/* declarations of the linker script variable with the end address of partition 34 bss */
extern uint64_t partition34BssEnd;

/* declarations of the linker script variable with the end address of partition 35 bss */
extern uint64_t partition35BssEnd;

/* declarations of the linker script variable with the end address of partition 36 bss */
extern uint64_t partition36BssEnd;

/* declarations of the linker script variable with the end address of partition 37 bss */
extern uint64_t partition37BssEnd;

/* declarations of the linker script variable with the end address of partition 38 bss */
extern uint64_t partition38BssEnd;

/* declarations of the linker script variable with the end address of partition 39 bss */
extern uint64_t partition39BssEnd;

/* declarations of the linker script variable with the end address of partition 40 bss */
extern uint64_t partition40BssEnd;

/* declarations of the linker script variable with the end address of partition 41 bss */
extern uint64_t partition41BssEnd;

/* declarations of the linker script variable with the end address of partition 42 bss */
extern uint64_t partition42BssEnd;

/* declarations of the linker script variable with the end address of partition 43 bss */
extern uint64_t partition43BssEnd;

/* declarations of the linker script variable with the end address of partition 44 bss */
extern uint64_t partition44BssEnd;

/* declarations of the linker script variable with the end address of partition 45 bss */
extern uint64_t partition45BssEnd;

/* declarations of the linker script variable with the end address of partition 46 bss */
extern uint64_t partition46BssEnd;

/* declarations of the linker script variable with the end address of partition 47 bss */
extern uint64_t partition47BssEnd;

/* declarations of the linker script variable with the end address of partition 48 bss */
extern uint64_t partition48BssEnd;

/* declarations of the linker script variable with the end address of partition 49 bss */
extern uint64_t partition49BssEnd;

/* declarations of the linker script variable with the end address of partition 50 bss */
extern uint64_t partition50BssEnd;

/* declarations of the linker script variable with the end address of partition 51 bss */
extern uint64_t partition51BssEnd;

/* declarations of the linker script variable with the end address of partition 52 bss */
extern uint64_t partition52BssEnd;

/* declarations of the linker script variable with the end address of partition 53 bss */
extern uint64_t partition53BssEnd;

/* declarations of the linker script variable with the end address of partition 54 bss */
extern uint64_t partition54BssEnd;

/* declarations of the linker script variable with the end address of partition 55 bss */
extern uint64_t partition55BssEnd;

/* declarations of the linker script variable with the end address of partition 56 bss */
extern uint64_t partition56BssEnd;

/* declarations of the linker script variable with the end address of partition 57 bss */
extern uint64_t partition57BssEnd;

/* declarations of the linker script variable with the end address of partition 58 bss */
extern uint64_t partition58BssEnd;

/* declarations of the linker script variable with the end address of partition 59 bss */
extern uint64_t partition59BssEnd;

/* declarations of the linker script variable with the end address of partition 60 bss */
extern uint64_t partition60BssEnd;

/* declarations of the linker script variable with the end address of partition 61 bss */
extern uint64_t partition61BssEnd;

/* declarations of the linker script variable with the end address of partition 62 bss */
extern uint64_t partition62BssEnd;

/* declarations of the linker script variable with the end address of partition 63 bss */
extern uint64_t partition63BssEnd;


/**
 * Linker script variable whose address indicates the data size of each partition
 */

/* declarations of the linker script variable with the size of the data of partition 0 */
extern uint8_t partition0DataSize;

/* declarations of the linker script variable with the size of the data of partition 1 */
extern uint8_t partition1DataSize;

/* declarations of the linker script variable with the size of the data of partition 2 */
extern uint8_t partition2DataSize;

/* declarations of the linker script variable with the size of the data of partition 3 */
extern uint8_t partition3DataSize;

/* declarations of the linker script variable with the size of the data of partition 4 */
extern uint8_t partition4DataSize;

/* declarations of the linker script variable with the size of the data of partition 5 */
extern uint8_t partition5DataSize;

/* declarations of the linker script variable with the size of the data of partition 6 */
extern uint8_t partition6DataSize;

/* declarations of the linker script variable with the size of the data of partition 7 */
extern uint8_t partition7DataSize;

/* declarations of the linker script variable with the size of the data of partition 8 */
extern uint8_t partition8DataSize;

/* declarations of the linker script variable with the size of the data of partition 9 */
extern uint8_t partition9DataSize;

/* declarations of the linker script variable with the size of the data of partition 10 */
extern uint8_t partition10DataSize;

/* declarations of the linker script variable with the size of the data of partition 11 */
extern uint8_t partition11DataSize;

/* declarations of the linker script variable with the size of the data of partition 12 */
extern uint8_t partition12DataSize;

/* declarations of the linker script variable with the size of the data of partition 13 */
extern uint8_t partition13DataSize;

/* declarations of the linker script variable with the size of the data of partition 14 */
extern uint8_t partition14DataSize;

/* declarations of the linker script variable with the size of the data of partition 15 */
extern uint8_t partition15DataSize;

/* declarations of the linker script variable with the size of the data of partition 16 */
extern uint8_t partition16DataSize;

/* declarations of the linker script variable with the size of the data of partition 17 */
extern uint8_t partition17DataSize;

/* declarations of the linker script variable with the size of the data of partition 18 */
extern uint8_t partition18DataSize;

/* declarations of the linker script variable with the size of the data of partition 19 */
extern uint8_t partition19DataSize;

/* declarations of the linker script variable with the size of the data of partition 20 */
extern uint8_t partition20DataSize;

/* declarations of the linker script variable with the size of the data of partition 21 */
extern uint8_t partition21DataSize;

/* declarations of the linker script variable with the size of the data of partition 22 */
extern uint8_t partition22DataSize;

/* declarations of the linker script variable with the size of the data of partition 23 */
extern uint8_t partition23DataSize;

/* declarations of the linker script variable with the size of the data of partition 24 */
extern uint8_t partition24DataSize;

/* declarations of the linker script variable with the size of the data of partition 25 */
extern uint8_t partition25DataSize;

/* declarations of the linker script variable with the size of the data of partition 26 */
extern uint8_t partition26DataSize;

/* declarations of the linker script variable with the size of the data of partition 27 */
extern uint8_t partition27DataSize;

/* declarations of the linker script variable with the size of the data of partition 28 */
extern uint8_t partition28DataSize;

/* declarations of the linker script variable with the size of the data of partition 29 */
extern uint8_t partition29DataSize;

/* declarations of the linker script variable with the size of the data of partition 30 */
extern uint8_t partition30DataSize;

/* declarations of the linker script variable with the size of the data of partition 31 */
extern uint8_t partition31DataSize;

/* declarations of the linker script variable with the size of the data of partition 32 */
extern uint8_t partition32DataSize;

/* declarations of the linker script variable with the size of the data of partition 33 */
extern uint8_t partition33DataSize;

/* declarations of the linker script variable with the size of the data of partition 34 */
extern uint8_t partition34DataSize;

/* declarations of the linker script variable with the size of the data of partition 35 */
extern uint8_t partition35DataSize;

/* declarations of the linker script variable with the size of the data of partition 36 */
extern uint8_t partition36DataSize;

/* declarations of the linker script variable with the size of the data of partition 37 */
extern uint8_t partition37DataSize;

/* declarations of the linker script variable with the size of the data of partition 38 */
extern uint8_t partition38DataSize;

/* declarations of the linker script variable with the size of the data of partition 39 */
extern uint8_t partition39DataSize;

/* declarations of the linker script variable with the size of the data of partition 40 */
extern uint8_t partition40DataSize;

/* declarations of the linker script variable with the size of the data of partition 41 */
extern uint8_t partition41DataSize;

/* declarations of the linker script variable with the size of the data of partition 42 */
extern uint8_t partition42DataSize;

/* declarations of the linker script variable with the size of the data of partition 43 */
extern uint8_t partition43DataSize;

/* declarations of the linker script variable with the size of the data of partition 44 */
extern uint8_t partition44DataSize;

/* declarations of the linker script variable with the size of the data of partition 45 */
extern uint8_t partition45DataSize;

/* declarations of the linker script variable with the size of the data of partition 46 */
extern uint8_t partition46DataSize;

/* declarations of the linker script variable with the size of the data of partition 47 */
extern uint8_t partition47DataSize;

/* declarations of the linker script variable with the size of the data of partition 48 */
extern uint8_t partition48DataSize;

/* declarations of the linker script variable with the size of the data of partition 49 */
extern uint8_t partition49DataSize;

/* declarations of the linker script variable with the size of the data of partition 50 */
extern uint8_t partition50DataSize;

/* declarations of the linker script variable with the size of the data of partition 51 */
extern uint8_t partition51DataSize;

/* declarations of the linker script variable with the size of the data of partition 52 */
extern uint8_t partition52DataSize;

/* declarations of the linker script variable with the size of the data of partition 53 */
extern uint8_t partition53DataSize;

/* declarations of the linker script variable with the size of the data of partition 54 */
extern uint8_t partition54DataSize;

/* declarations of the linker script variable with the size of the data of partition 55 */
extern uint8_t partition55DataSize;

/* declarations of the linker script variable with the size of the data of partition 56 */
extern uint8_t partition56DataSize;

/* declarations of the linker script variable with the size of the data of partition 57 */
extern uint8_t partition57DataSize;

/* declarations of the linker script variable with the size of the data of partition 58 */
extern uint8_t partition58DataSize;

/* declarations of the linker script variable with the size of the data of partition 59 */
extern uint8_t partition59DataSize;

/* declarations of the linker script variable with the size of the data of partition 60 */
extern uint8_t partition60DataSize;

/* declarations of the linker script variable with the size of the data of partition 61 */
extern uint8_t partition61DataSize;

/* declarations of the linker script variable with the size of the data of partition 62 */
extern uint8_t partition62DataSize;

/* declarations of the linker script variable with the size of the data of partition 63 */
extern uint8_t partition63DataSize;

/**
 * Linker script variable whose address indicates the bss size of the partitions
 */


/* declarations of the linker script variable with the size of the bss of partition 0 */
extern uint8_t partition0BssSize;

/* declarations of the linker script variable with the size of the bss of partition 1 */
extern uint8_t partition1BssSize;

/* declarations of the linker script variable with the size of the bss of partition 2 */
extern uint8_t partition2BssSize;

/* declarations of the linker script variable with the size of the bss of partition 3 */
extern uint8_t partition3BssSize;

/* declarations of the linker script variable with the size of the bss of partition 4 */
extern uint8_t partition4BssSize;

/* declarations of the linker script variable with the size of the bss of partition 5 */
extern uint8_t partition5BssSize;

/* declarations of the linker script variable with the size of the bss of partition 6 */
extern uint8_t partition6BssSize;

/* declarations of the linker script variable with the size of the bss of partition 7 */
extern uint8_t partition7BssSize;

/* declarations of the linker script variable with the size of the bss of partition 8 */
extern uint8_t partition8BssSize;

/* declarations of the linker script variable with the size of the bss of partition 9 */
extern uint8_t partition9BssSize;

/* declarations of the linker script variable with the size of the bss of partition 10 */
extern uint8_t partition10BssSize;

/* declarations of the linker script variable with the size of the bss of partition 11 */
extern uint8_t partition11BssSize;

/* declarations of the linker script variable with the size of the bss of partition 12 */
extern uint8_t partition12BssSize;

/* declarations of the linker script variable with the size of the bss of partition 13 */
extern uint8_t partition13BssSize;

/* declarations of the linker script variable with the size of the bss of partition 14 */
extern uint8_t partition14BssSize;

/* declarations of the linker script variable with the size of the bss of partition 15 */
extern uint8_t partition15BssSize;

/* declarations of the linker script variable with the size of the bss of partition 16 */
extern uint8_t partition16BssSize;

/* declarations of the linker script variable with the size of the bss of partition 17 */
extern uint8_t partition17BssSize;

/* declarations of the linker script variable with the size of the bss of partition 18 */
extern uint8_t partition18BssSize;

/* declarations of the linker script variable with the size of the bss of partition 19 */
extern uint8_t partition19BssSize;

/* declarations of the linker script variable with the size of the bss of partition 20 */
extern uint8_t partition20BssSize;

/* declarations of the linker script variable with the size of the bss of partition 21 */
extern uint8_t partition21BssSize;

/* declarations of the linker script variable with the size of the bss of partition 22 */
extern uint8_t partition22BssSize;

/* declarations of the linker script variable with the size of the bss of partition 23 */
extern uint8_t partition23BssSize;

/* declarations of the linker script variable with the size of the bss of partition 24 */
extern uint8_t partition24BssSize;

/* declarations of the linker script variable with the size of the bss of partition 25 */
extern uint8_t partition25BssSize;

/* declarations of the linker script variable with the size of the bss of partition 26 */
extern uint8_t partition26BssSize;

/* declarations of the linker script variable with the size of the bss of partition 27 */
extern uint8_t partition27BssSize;

/* declarations of the linker script variable with the size of the bss of partition 28 */
extern uint8_t partition28BssSize;

/* declarations of the linker script variable with the size of the bss of partition 29 */
extern uint8_t partition29BssSize;

/* declarations of the linker script variable with the size of the bss of partition 30 */
extern uint8_t partition30BssSize;

/* declarations of the linker script variable with the size of the bss of partition 31 */
extern uint8_t partition31BssSize;

/* declarations of the linker script variable with the size of the bss of partition 32 */
extern uint8_t partition32BssSize;

/* declarations of the linker script variable with the size of the bss of partition 33 */
extern uint8_t partition33BssSize;

/* declarations of the linker script variable with the size of the bss of partition 34 */
extern uint8_t partition34BssSize;

/* declarations of the linker script variable with the size of the bss of partition 35 */
extern uint8_t partition35BssSize;

/* declarations of the linker script variable with the size of the bss of partition 36 */
extern uint8_t partition36BssSize;

/* declarations of the linker script variable with the size of the bss of partition 37 */
extern uint8_t partition37BssSize;

/* declarations of the linker script variable with the size of the bss of partition 38 */
extern uint8_t partition38BssSize;

/* declarations of the linker script variable with the size of the bss of partition 39 */
extern uint8_t partition39BssSize;

/* declarations of the linker script variable with the size of the bss of partition 40 */
extern uint8_t partition40BssSize;

/* declarations of the linker script variable with the size of the bss of partition 41 */
extern uint8_t partition41BssSize;

/* declarations of the linker script variable with the size of the bss of partition 42 */
extern uint8_t partition42BssSize;

/* declarations of the linker script variable with the size of the bss of partition 43 */
extern uint8_t partition43BssSize;

/* declarations of the linker script variable with the size of the bss of partition 44 */
extern uint8_t partition44BssSize;

/* declarations of the linker script variable with the size of the bss of partition 45 */
extern uint8_t partition45BssSize;

/* declarations of the linker script variable with the size of the bss of partition 46 */
extern uint8_t partition46BssSize;

/* declarations of the linker script variable with the size of the bss of partition 47 */
extern uint8_t partition47BssSize;

/* declarations of the linker script variable with the size of the bss of partition 48 */
extern uint8_t partition48BssSize;

/* declarations of the linker script variable with the size of the bss of partition 49 */
extern uint8_t partition49BssSize;

/* declarations of the linker script variable with the size of the bss of partition 50 */
extern uint8_t partition50BssSize;

/* declarations of the linker script variable with the size of the bss of partition 51 */
extern uint8_t partition51BssSize;

/* declarations of the linker script variable with the size of the bss of partition 52 */
extern uint8_t partition52BssSize;

/* declarations of the linker script variable with the size of the bss of partition 53 */
extern uint8_t partition53BssSize;

/* declarations of the linker script variable with the size of the bss of partition 54 */
extern uint8_t partition54BssSize;

/* declarations of the linker script variable with the size of the bss of partition 55 */
extern uint8_t partition55BssSize;

/* declarations of the linker script variable with the size of the bss of partition 56 */
extern uint8_t partition56BssSize;

/* declarations of the linker script variable with the size of the bss of partition 57 */
extern uint8_t partition57BssSize;

/* declarations of the linker script variable with the size of the bss of partition 58 */
extern uint8_t partition58BssSize;

/* declarations of the linker script variable with the size of the bss of partition 59 */
extern uint8_t partition59BssSize;

/* declarations of the linker script variable with the size of the bss of partition 60 */
extern uint8_t partition60BssSize;

/* declarations of the linker script variable with the size of the bss of partition 61 */
extern uint8_t partition61BssSize;

/* declarations of the linker script variable with the size of the bss of partition 62 */
extern uint8_t partition62BssSize;

/* declarations of the linker script variable with the size of the bss of partition 63 */
extern uint8_t partition63BssSize;

extern uint8_t pharosCpuTextSizeLog2;
extern uint8_t pharosCpuSharedSizeLog2;
extern uint8_t pharosCpuIpCallSizeLog2;


/**
 * The following variables should be able to be declared static in the protection.c file. 
 * However, due to a GCC bug (at least 7.1.0) it gives an error stating that cannot place variables with different types
 * (e.g. uint64_t * and uint8_t *) on the same section. Furthermore, the "const" attribute does not seem to place the
 * variables on the correct section (they ended up in RAM on the EK-TM4C129EXL board).
 */
extern const uint64_t *partitionsBssStart[];
extern const uint8_t *pharosVPartitionsStart[];
extern const uint8_t *pharosVPartitionsEnd[];
extern const uint64_t *partitionsDataStart[];
extern const uint64_t *partitionsDataAddr[];


/* PHAROS_LINKER_H */
#endif	
