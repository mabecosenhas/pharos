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


OUTPUT_ARCH(aarch64)

/*
 * The memory map looks like this:
 * +-----------------------+ <- low memory
 * | .text                 |
 * |        etext          |
 * |        _endtext       |
 * +-----------------------+
 * | .kernel               | initialized data goes here
 * |        kernel data    |
 * |        kernel bss     |
 * |        kernel uss     |
 * +-----------------------+
 * | .partition0           | initialized data goes here
 * |        partition data |
 * |        partition bss  |
 * |        partition uss  |
 * +-----------------------+
 * | .partition1           | initialized data goes here
 * |        partition data |
 * |        partition bss  |
 * |        partition uss  |
 * +-----------------------+
 *            .
 *            .
 *            .
 * +-----------------------+
 * | .partition63          | initialized data goes here
 * |        partition data |
 * |        partition bss  |
 * |        partition uss  |
 * +-----------------------+ <- high memory
 */

/* partitions start at 2M */
PARTITION_ALIGNMENT = 0x200000;

CHANNEL_BULK_SIZE = 512;

PAGE_START_ALIGN_4K = 4K;
PAGE_START_ALIGN_2M = 2M;

PARTITION_MAXIMUM_NUMBER = 64;

/* calculate the log2 of the text section size, used to initialize the MPU size register */
pharosCpuTextSize   = SIZEOF(.text);
pharosCpuSharedSize = pharosSharedEnd - pharosSharedStart;
pharosCpuIpCallSize = pharosIpCallEnd - pharosIpCallStart;
pharosCpuKernelSize = pharosKernelEnd - pharosKernelStart;

pharosTextSize      = pharosEndText - pharosStartText;

pharosCpuSharedSizeLog2 = LOG2CEIL(pharosSharedEnd - pharosSharedStart);

pharosCpuPartition0Size = partition0End - partition0Start;
pharosCpuPartition1Size = partition1End - partition1Start;
pharosCpuPartition2Size = partition2End - partition2Start;
pharosCpuPartition3Size = partition3End - partition3Start;
pharosCpuPartition4Size = partition4End - partition4Start;
pharosCpuPartition5Size = partition5End - partition5Start;
pharosCpuPartition6Size = partition6End - partition6Start;
pharosCpuPartition7Size = partition7End - partition7Start;
pharosCpuPartition8Size = partition8End - partition8Start;
pharosCpuPartition9Size = partition9End - partition9Start;
pharosCpuPartition10Size = partition10End - partition10Start;
pharosCpuPartition11Size = partition11End - partition11Start;
pharosCpuPartition12Size = partition12End - partition12Start;
pharosCpuPartition13Size = partition13End - partition13Start;
pharosCpuPartition14Size = partition14End - partition14Start;
pharosCpuPartition15Size = partition15End - partition15Start;
pharosCpuPartition16Size = partition16End - partition16Start;
pharosCpuPartition17Size = partition17End - partition17Start;
pharosCpuPartition18Size = partition18End - partition18Start;
pharosCpuPartition19Size = partition19End - partition19Start;
pharosCpuPartition20Size = partition20End - partition20Start;
pharosCpuPartition21Size = partition21End - partition21Start;
pharosCpuPartition22Size = partition22End - partition22Start;
pharosCpuPartition23Size = partition23End - partition23Start;
pharosCpuPartition24Size = partition24End - partition24Start;
pharosCpuPartition25Size = partition25End - partition25Start;
pharosCpuPartition26Size = partition26End - partition26Start;
pharosCpuPartition27Size = partition27End - partition27Start;
pharosCpuPartition28Size = partition28End - partition28Start;
pharosCpuPartition29Size = partition29End - partition29Start;
pharosCpuPartition30Size = partition30End - partition30Start;
pharosCpuPartition31Size = partition31End - partition31Start;
pharosCpuPartition32Size = partition32End - partition32Start;
pharosCpuPartition33Size = partition33End - partition33Start;
pharosCpuPartition34Size = partition34End - partition34Start;
pharosCpuPartition35Size = partition35End - partition35Start;
pharosCpuPartition36Size = partition36End - partition36Start;
pharosCpuPartition37Size = partition37End - partition37Start;
pharosCpuPartition38Size = partition38End - partition38Start;
pharosCpuPartition39Size = partition39End - partition39Start;
pharosCpuPartition40Size = partition40End - partition40Start;
pharosCpuPartition41Size = partition41End - partition41Start;
pharosCpuPartition42Size = partition42End - partition42Start;
pharosCpuPartition43Size = partition43End - partition43Start;
pharosCpuPartition44Size = partition44End - partition44Start;
pharosCpuPartition45Size = partition45End - partition45Start;
pharosCpuPartition46Size = partition46End - partition46Start;
pharosCpuPartition47Size = partition47End - partition47Start;
pharosCpuPartition48Size = partition48End - partition48Start;
pharosCpuPartition49Size = partition49End - partition49Start;
pharosCpuPartition50Size = partition50End - partition50Start;
pharosCpuPartition51Size = partition51End - partition51Start;
pharosCpuPartition52Size = partition52End - partition52Start;
pharosCpuPartition53Size = partition53End - partition53Start;
pharosCpuPartition54Size = partition54End - partition54Start;
pharosCpuPartition55Size = partition55End - partition55Start;
pharosCpuPartition56Size = partition56End - partition56Start;
pharosCpuPartition57Size = partition57End - partition57Start;
pharosCpuPartition58Size = partition58End - partition58Start;
pharosCpuPartition59Size = partition59End - partition59Start;
pharosCpuPartition60Size = partition60End - partition60Start;
pharosCpuPartition61Size = partition61End - partition61Start;
pharosCpuPartition62Size = partition62End - partition62Start;
pharosCpuPartition63Size = partition63End - partition63Start;

pharosSharedStackSize = pharosSharedStackEnd - pharosSharedStackStart;
/* number of tables level 2 needed for all the the shared stack for one partition */
pharosNumberSharedStackMmuTables2PerPartition = 1 << LOG2CEIL(pharosSharedStackSize / 1024M);

/* number of tables level 3 needed for all the the shared stack for one partition */
pharosNumberSharedStackMmuTables3PerPartition = 1 << LOG2CEIL(pharosSharedStackSize / 2M);

 
IP_CALL_ALIGNMENT = PARTITION_ALIGNMENT;

PHAROS_CPU_IO_START = 0x3F000000;
PHAROS_CPU_IO_END   = 0x40000000;


MEMORY
{
  /* start text at 0x8000 because of QEMU internal way of doing things */
  sdram     : ORIGIN = 0x0, LENGTH = 1024M   
}

ENTRY(pharosCpuStart);

/* Define output sections */
SECTIONS
{

  /******************************************************************/
  /*              Start of text and read-only data area             */
  /******************************************************************/
  .text :
  {
    CREATE_OBJECT_SYMBOLS

    /* source code is in text section */
    pharosStartText = .;
    
    *(.textStart)
    *(.textStart.*)
    *(.text)
    *(.text.*)

    /* read-only data is placed in text */
    . = ALIGN (0x10);
    pharosRodataStart = . ;
    *(.rodata)
    *(.rodata.*)
    *(.srodata)
    *(.srodata.*)
    *(.note.gnu.build-id)
    pharosRodataEnd = .;

    /* variable to check the end of the text section */
    . = ALIGN (0x10);
    _endtext = .;
    pharosEndText = .;
  } > sdram

  /******************************************************************/
  /*                 End of text area and read-only data            */
  /******************************************************************/


  /******************************************************************/
  /*                     Start of kernel area                       */
  /******************************************************************/
 
  pharosKernelDataLoadAddr = pharosKernelStart;

  pharosKernelDataSize = 0;
  pharosKernelBssSize = 0;


  /* Kernel data space to enable memory protection */
  .kernel : 
  {
    CREATE_OBJECT_SYMBOLS
    
    . = ALIGN (PARTITION_ALIGNMENT);
    . = ALIGN(8);

    pharosKernelStart = .;
    pharosKernelDataStart = .;
    *(.kernel)  
    *(.kernel.*)
  
    . = ALIGN(8);
    pharosKernelDataEnd = .;

  } > sdram

  .kernelBss (NOLOAD):
  {
    CREATE_OBJECT_SYMBOLS
    
    . = ALIGN(8);
    pharosKernelBssStart = .;

    *(.kernelBss)   
    
    . = ALIGN(8);
    pharosKernelBssEnd = .;

    /* place here the data that is NOT to be initialized at all */
    pharosKernelUssStart = .;

    *(.kernelUss)
    *(.kernelUss.*)

    pharosKernelUssEnd = .;

    . = ALIGN(4K);

    /* reserve space for the partition 0 MMU tables level 2 according to its memory area size */
    pharosPartition0MmuLevel2Start = .;
    . = . + ((pharosCpuPartition0Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition0MmuLevel2End = .;

    /* reserve space for the partition 0 MMU tables level 3 according to its memory area size */
    pharosPartition0MmuLevel3Start = .;
    . = . + ((pharosCpuPartition0Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition0MmuLevel3End = .;


    /* reserve space for the partition 1 MMU tables level 2 according to its memory area size */
    pharosPartition1MmuLevel2Start = .;
    . = . + ((pharosCpuPartition1Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition1MmuLevel2End = .;

    /* reserve space for the partition 1 MMU tables level 3 according to its memory area size */
    pharosPartition1MmuLevel3Start = .;
    . = . + ((pharosCpuPartition1Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition1MmuLevel3End = .;


    /* reserve space for the partition 2 MMU tables level 2 according to its memory area size */
    pharosPartition2MmuLevel2Start = .;
    . = . + ((pharosCpuPartition2Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition2MmuLevel2End = .;

    /* reserve space for the partition 2 MMU tables level 3 according to its memory area size */
    pharosPartition2MmuLevel3Start = .;
    . = . + ((pharosCpuPartition2Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition2MmuLevel3End = .;


    /* reserve space for the partition 3 MMU tables level 2 according to its memory area size */
    pharosPartition3MmuLevel2Start = .;
    . = . + ((pharosCpuPartition3Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition3MmuLevel2End = .;

    /* reserve space for the partition 3 MMU tables level 3 according to its memory area size */
    pharosPartition3MmuLevel3Start = .;
    . = . + ((pharosCpuPartition3Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition3MmuLevel3End = .;


    /* reserve space for the partition 4 MMU tables level 2 according to its memory area size */
    pharosPartition4MmuLevel2Start = .;
    . = . + ((pharosCpuPartition4Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition4MmuLevel2End = .;

    /* reserve space for the partition 4 MMU tables level 3 according to its memory area size */
    pharosPartition4MmuLevel3Start = .;
    . = . + ((pharosCpuPartition4Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition4MmuLevel3End = .;


    /* reserve space for the partition 5 MMU tables level 2 according to its memory area size */
    pharosPartition5MmuLevel2Start = .;
    . = . + ((pharosCpuPartition5Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition5MmuLevel2End = .;

    /* reserve space for the partition 5 MMU tables level 3 according to its memory area size */
    pharosPartition5MmuLevel3Start = .;
    . = . + ((pharosCpuPartition5Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition5MmuLevel3End = .;


    /* reserve space for the partition 6 MMU tables level 2 according to its memory area size */
    pharosPartition6MmuLevel2Start = .;
    . = . + ((pharosCpuPartition6Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition6MmuLevel2End = .;

    /* reserve space for the partition 6 MMU tables level 3 according to its memory area size */
    pharosPartition6MmuLevel3Start = .;
    . = . + ((pharosCpuPartition6Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition6MmuLevel3End = .;


    /* reserve space for the partition 7 MMU tables level 2 according to its memory area size */
    pharosPartition7MmuLevel2Start = .;
    . = . + ((pharosCpuPartition7Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition7MmuLevel2End = .;

    /* reserve space for the partition 7 MMU tables level 3 according to its memory area size */
    pharosPartition7MmuLevel3Start = .;
    . = . + ((pharosCpuPartition7Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition7MmuLevel3End = .;


    /* reserve space for the partition 8 MMU tables level 2 according to its memory area size */
    pharosPartition8MmuLevel2Start = .;
    . = . + ((pharosCpuPartition8Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition8MmuLevel2End = .;

    /* reserve space for the partition 8 MMU tables level 3 according to its memory area size */
    pharosPartition8MmuLevel3Start = .;
    . = . + ((pharosCpuPartition8Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition8MmuLevel3End = .;


    /* reserve space for the partition 9 MMU tables level 2 according to its memory area size */
    pharosPartition9MmuLevel2Start = .;
    . = . + ((pharosCpuPartition9Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition9MmuLevel2End = .;

    /* reserve space for the partition 9 MMU tables level 3 according to its memory area size */
    pharosPartition9MmuLevel3Start = .;
    . = . + ((pharosCpuPartition9Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition9MmuLevel3End = .;


    /* reserve space for the partition 10 MMU tables level 2 according to its memory area size */
    pharosPartition10MmuLevel2Start = .;
    . = . + ((pharosCpuPartition10Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition10MmuLevel2End = .;

    /* reserve space for the partition 10 MMU tables level 3 according to its memory area size */
    pharosPartition10MmuLevel3Start = .;
    . = . + ((pharosCpuPartition10Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition10MmuLevel3End = .;


    /* reserve space for the partition 11 MMU tables level 2 according to its memory area size */
    pharosPartition11MmuLevel2Start = .;
    . = . + ((pharosCpuPartition11Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition11MmuLevel2End = .;

    /* reserve space for the partition 11 MMU tables level 3 according to its memory area size */
    pharosPartition11MmuLevel3Start = .;
    . = . + ((pharosCpuPartition11Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition11MmuLevel3End = .;


    /* reserve space for the partition 12 MMU tables level 2 according to its memory area size */
    pharosPartition12MmuLevel2Start = .;
    . = . + ((pharosCpuPartition12Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition12MmuLevel2End = .;

    /* reserve space for the partition 12 MMU tables level 3 according to its memory area size */
    pharosPartition12MmuLevel3Start = .;
    . = . + ((pharosCpuPartition12Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition12MmuLevel3End = .;


    /* reserve space for the partition 13 MMU tables level 2 according to its memory area size */
    pharosPartition13MmuLevel2Start = .;
    . = . + ((pharosCpuPartition13Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition13MmuLevel2End = .;

    /* reserve space for the partition 13 MMU tables level 3 according to its memory area size */
    pharosPartition13MmuLevel3Start = .;
    . = . + ((pharosCpuPartition13Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition13MmuLevel3End = .;


    /* reserve space for the partition 14 MMU tables level 2 according to its memory area size */
    pharosPartition14MmuLevel2Start = .;
    . = . + ((pharosCpuPartition14Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition14MmuLevel2End = .;

    /* reserve space for the partition 14 MMU tables level 3 according to its memory area size */
    pharosPartition14MmuLevel3Start = .;
    . = . + ((pharosCpuPartition14Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition14MmuLevel3End = .;


    /* reserve space for the partition 15 MMU tables level 2 according to its memory area size */
    pharosPartition15MmuLevel2Start = .;
    . = . + ((pharosCpuPartition15Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition15MmuLevel2End = .;

    /* reserve space for the partition 15 MMU tables level 3 according to its memory area size */
    pharosPartition15MmuLevel3Start = .;
    . = . + ((pharosCpuPartition15Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition15MmuLevel3End = .;


    /* reserve space for the partition 16 MMU tables level 2 according to its memory area size */
    pharosPartition16MmuLevel2Start = .;
    . = . + ((pharosCpuPartition16Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition16MmuLevel2End = .;

    /* reserve space for the partition 16 MMU tables level 3 according to its memory area size */
    pharosPartition16MmuLevel3Start = .;
    . = . + ((pharosCpuPartition16Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition16MmuLevel3End = .;


    /* reserve space for the partition 17 MMU tables level 2 according to its memory area size */
    pharosPartition17MmuLevel2Start = .;
    . = . + ((pharosCpuPartition17Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition17MmuLevel2End = .;

    /* reserve space for the partition 17 MMU tables level 3 according to its memory area size */
    pharosPartition17MmuLevel3Start = .;
    . = . + ((pharosCpuPartition17Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition17MmuLevel3End = .;


    /* reserve space for the partition 18 MMU tables level 2 according to its memory area size */
    pharosPartition18MmuLevel2Start = .;
    . = . + ((pharosCpuPartition18Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition18MmuLevel2End = .;

    /* reserve space for the partition 18 MMU tables level 3 according to its memory area size */
    pharosPartition18MmuLevel3Start = .;
    . = . + ((pharosCpuPartition18Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition18MmuLevel3End = .;


    /* reserve space for the partition 19 MMU tables level 2 according to its memory area size */
    pharosPartition19MmuLevel2Start = .;
    . = . + ((pharosCpuPartition19Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition19MmuLevel2End = .;

    /* reserve space for the partition 19 MMU tables level 3 according to its memory area size */
    pharosPartition19MmuLevel3Start = .;
    . = . + ((pharosCpuPartition19Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition19MmuLevel3End = .;


    /* reserve space for the partition 20 MMU tables level 2 according to its memory area size */
    pharosPartition20MmuLevel2Start = .;
    . = . + ((pharosCpuPartition20Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition20MmuLevel2End = .;

    /* reserve space for the partition 20 MMU tables level 3 according to its memory area size */
    pharosPartition20MmuLevel3Start = .;
    . = . + ((pharosCpuPartition20Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition20MmuLevel3End = .;


    /* reserve space for the partition 21 MMU tables level 2 according to its memory area size */
    pharosPartition21MmuLevel2Start = .;
    . = . + ((pharosCpuPartition21Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition21MmuLevel2End = .;

    /* reserve space for the partition 21 MMU tables level 3 according to its memory area size */
    pharosPartition21MmuLevel3Start = .;
    . = . + ((pharosCpuPartition21Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition21MmuLevel3End = .;


    /* reserve space for the partition 22 MMU tables level 2 according to its memory area size */
    pharosPartition22MmuLevel2Start = .;
    . = . + ((pharosCpuPartition22Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition22MmuLevel2End = .;

    /* reserve space for the partition 22 MMU tables level 3 according to its memory area size */
    pharosPartition22MmuLevel3Start = .;
    . = . + ((pharosCpuPartition22Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition22MmuLevel3End = .;


    /* reserve space for the partition 23 MMU tables level 2 according to its memory area size */
    pharosPartition23MmuLevel2Start = .;
    . = . + ((pharosCpuPartition23Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition23MmuLevel2End = .;

    /* reserve space for the partition 23 MMU tables level 3 according to its memory area size */
    pharosPartition23MmuLevel3Start = .;
    . = . + ((pharosCpuPartition23Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition23MmuLevel3End = .;


    /* reserve space for the partition 24 MMU tables level 2 according to its memory area size */
    pharosPartition24MmuLevel2Start = .;
    . = . + ((pharosCpuPartition24Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition24MmuLevel2End = .;

    /* reserve space for the partition 24 MMU tables level 3 according to its memory area size */
    pharosPartition24MmuLevel3Start = .;
    . = . + ((pharosCpuPartition24Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition24MmuLevel3End = .;


    /* reserve space for the partition 25 MMU tables level 2 according to its memory area size */
    pharosPartition25MmuLevel2Start = .;
    . = . + ((pharosCpuPartition25Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition25MmuLevel2End = .;

    /* reserve space for the partition 25 MMU tables level 3 according to its memory area size */
    pharosPartition25MmuLevel3Start = .;
    . = . + ((pharosCpuPartition25Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition25MmuLevel3End = .;


    /* reserve space for the partition 26 MMU tables level 2 according to its memory area size */
    pharosPartition26MmuLevel2Start = .;
    . = . + ((pharosCpuPartition26Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition26MmuLevel2End = .;

    /* reserve space for the partition 26 MMU tables level 3 according to its memory area size */
    pharosPartition26MmuLevel3Start = .;
    . = . + ((pharosCpuPartition26Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition26MmuLevel3End = .;


    /* reserve space for the partition 27 MMU tables level 2 according to its memory area size */
    pharosPartition27MmuLevel2Start = .;
    . = . + ((pharosCpuPartition27Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition27MmuLevel2End = .;

    /* reserve space for the partition 27 MMU tables level 3 according to its memory area size */
    pharosPartition27MmuLevel3Start = .;
    . = . + ((pharosCpuPartition27Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition27MmuLevel3End = .;


    /* reserve space for the partition 28 MMU tables level 2 according to its memory area size */
    pharosPartition28MmuLevel2Start = .;
    . = . + ((pharosCpuPartition28Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition28MmuLevel2End = .;

    /* reserve space for the partition 28 MMU tables level 3 according to its memory area size */
    pharosPartition28MmuLevel3Start = .;
    . = . + ((pharosCpuPartition28Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition28MmuLevel3End = .;


    /* reserve space for the partition 29 MMU tables level 2 according to its memory area size */
    pharosPartition29MmuLevel2Start = .;
    . = . + ((pharosCpuPartition29Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition29MmuLevel2End = .;

    /* reserve space for the partition 29 MMU tables level 3 according to its memory area size */
    pharosPartition29MmuLevel3Start = .;
    . = . + ((pharosCpuPartition29Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition29MmuLevel3End = .;


    /* reserve space for the partition 30 MMU tables level 2 according to its memory area size */
    pharosPartition30MmuLevel2Start = .;
    . = . + ((pharosCpuPartition30Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition30MmuLevel2End = .;

    /* reserve space for the partition 30 MMU tables level 3 according to its memory area size */
    pharosPartition30MmuLevel3Start = .;
    . = . + ((pharosCpuPartition30Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition30MmuLevel3End = .;


    /* reserve space for the partition 31 MMU tables level 2 according to its memory area size */
    pharosPartition31MmuLevel2Start = .;
    . = . + ((pharosCpuPartition31Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition31MmuLevel2End = .;

    /* reserve space for the partition 31 MMU tables level 3 according to its memory area size */
    pharosPartition31MmuLevel3Start = .;
    . = . + ((pharosCpuPartition31Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition31MmuLevel3End = .;


    /* reserve space for the partition 32 MMU tables level 2 according to its memory area size */
    pharosPartition32MmuLevel2Start = .;
    . = . + ((pharosCpuPartition32Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition32MmuLevel2End = .;

    /* reserve space for the partition 32 MMU tables level 3 according to its memory area size */
    pharosPartition32MmuLevel3Start = .;
    . = . + ((pharosCpuPartition32Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition32MmuLevel3End = .;


    /* reserve space for the partition 33 MMU tables level 2 according to its memory area size */
    pharosPartition33MmuLevel2Start = .;
    . = . + ((pharosCpuPartition33Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition33MmuLevel2End = .;

    /* reserve space for the partition 33 MMU tables level 3 according to its memory area size */
    pharosPartition33MmuLevel3Start = .;
    . = . + ((pharosCpuPartition33Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition33MmuLevel3End = .;


    /* reserve space for the partition 34 MMU tables level 2 according to its memory area size */
    pharosPartition34MmuLevel2Start = .;
    . = . + ((pharosCpuPartition34Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition34MmuLevel2End = .;

    /* reserve space for the partition 34 MMU tables level 3 according to its memory area size */
    pharosPartition34MmuLevel3Start = .;
    . = . + ((pharosCpuPartition34Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition34MmuLevel3End = .;


    /* reserve space for the partition 35 MMU tables level 2 according to its memory area size */
    pharosPartition35MmuLevel2Start = .;
    . = . + ((pharosCpuPartition35Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition35MmuLevel2End = .;

    /* reserve space for the partition 35 MMU tables level 3 according to its memory area size */
    pharosPartition35MmuLevel3Start = .;
    . = . + ((pharosCpuPartition35Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition35MmuLevel3End = .;


    /* reserve space for the partition 36 MMU tables level 2 according to its memory area size */
    pharosPartition36MmuLevel2Start = .;
    . = . + ((pharosCpuPartition36Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition36MmuLevel2End = .;

    /* reserve space for the partition 36 MMU tables level 3 according to its memory area size */
    pharosPartition36MmuLevel3Start = .;
    . = . + ((pharosCpuPartition36Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition36MmuLevel3End = .;


    /* reserve space for the partition 37 MMU tables level 2 according to its memory area size */
    pharosPartition37MmuLevel2Start = .;
    . = . + ((pharosCpuPartition37Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition37MmuLevel2End = .;

    /* reserve space for the partition 37 MMU tables level 3 according to its memory area size */
    pharosPartition37MmuLevel3Start = .;
    . = . + ((pharosCpuPartition37Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition37MmuLevel3End = .;


    /* reserve space for the partition 38 MMU tables level 2 according to its memory area size */
    pharosPartition38MmuLevel2Start = .;
    . = . + ((pharosCpuPartition38Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition38MmuLevel2End = .;

    /* reserve space for the partition 38 MMU tables level 3 according to its memory area size */
    pharosPartition38MmuLevel3Start = .;
    . = . + ((pharosCpuPartition38Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition38MmuLevel3End = .;


    /* reserve space for the partition 39 MMU tables level 2 according to its memory area size */
    pharosPartition39MmuLevel2Start = .;
    . = . + ((pharosCpuPartition39Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition39MmuLevel2End = .;

    /* reserve space for the partition 39 MMU tables level 3 according to its memory area size */
    pharosPartition39MmuLevel3Start = .;
    . = . + ((pharosCpuPartition39Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition39MmuLevel3End = .;


    /* reserve space for the partition 40 MMU tables level 2 according to its memory area size */
    pharosPartition40MmuLevel2Start = .;
    . = . + ((pharosCpuPartition40Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition40MmuLevel2End = .;

    /* reserve space for the partition 40 MMU tables level 3 according to its memory area size */
    pharosPartition40MmuLevel3Start = .;
    . = . + ((pharosCpuPartition40Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition40MmuLevel3End = .;


    /* reserve space for the partition 41 MMU tables level 2 according to its memory area size */
    pharosPartition41MmuLevel2Start = .;
    . = . + ((pharosCpuPartition41Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition41MmuLevel2End = .;

    /* reserve space for the partition 41 MMU tables level 3 according to its memory area size */
    pharosPartition41MmuLevel3Start = .;
    . = . + ((pharosCpuPartition41Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition41MmuLevel3End = .;


    /* reserve space for the partition 42 MMU tables level 2 according to its memory area size */
    pharosPartition42MmuLevel2Start = .;
    . = . + ((pharosCpuPartition42Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition42MmuLevel2End = .;

    /* reserve space for the partition 42 MMU tables level 3 according to its memory area size */
    pharosPartition42MmuLevel3Start = .;
    . = . + ((pharosCpuPartition42Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition42MmuLevel3End = .;


    /* reserve space for the partition 43 MMU tables level 2 according to its memory area size */
    pharosPartition43MmuLevel2Start = .;
    . = . + ((pharosCpuPartition43Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition43MmuLevel2End = .;

    /* reserve space for the partition 43 MMU tables level 3 according to its memory area size */
    pharosPartition43MmuLevel3Start = .;
    . = . + ((pharosCpuPartition43Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition43MmuLevel3End = .;


    /* reserve space for the partition 44 MMU tables level 2 according to its memory area size */
    pharosPartition44MmuLevel2Start = .;
    . = . + ((pharosCpuPartition44Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition44MmuLevel2End = .;

    /* reserve space for the partition 44 MMU tables level 3 according to its memory area size */
    pharosPartition44MmuLevel3Start = .;
    . = . + ((pharosCpuPartition44Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition44MmuLevel3End = .;


    /* reserve space for the partition 45 MMU tables level 2 according to its memory area size */
    pharosPartition45MmuLevel2Start = .;
    . = . + ((pharosCpuPartition45Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition45MmuLevel2End = .;

    /* reserve space for the partition 45 MMU tables level 3 according to its memory area size */
    pharosPartition45MmuLevel3Start = .;
    . = . + ((pharosCpuPartition45Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition45MmuLevel3End = .;


    /* reserve space for the partition 46 MMU tables level 2 according to its memory area size */
    pharosPartition46MmuLevel2Start = .;
    . = . + ((pharosCpuPartition46Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition46MmuLevel2End = .;

    /* reserve space for the partition 46 MMU tables level 3 according to its memory area size */
    pharosPartition46MmuLevel3Start = .;
    . = . + ((pharosCpuPartition46Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition46MmuLevel3End = .;


    /* reserve space for the partition 47 MMU tables level 2 according to its memory area size */
    pharosPartition47MmuLevel2Start = .;
    . = . + ((pharosCpuPartition47Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition47MmuLevel2End = .;

    /* reserve space for the partition 47 MMU tables level 3 according to its memory area size */
    pharosPartition47MmuLevel3Start = .;
    . = . + ((pharosCpuPartition47Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition47MmuLevel3End = .;


    /* reserve space for the partition 48 MMU tables level 2 according to its memory area size */
    pharosPartition48MmuLevel2Start = .;
    . = . + ((pharosCpuPartition48Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition48MmuLevel2End = .;

    /* reserve space for the partition 48 MMU tables level 3 according to its memory area size */
    pharosPartition48MmuLevel3Start = .;
    . = . + ((pharosCpuPartition48Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition48MmuLevel3End = .;


    /* reserve space for the partition 49 MMU tables level 2 according to its memory area size */
    pharosPartition49MmuLevel2Start = .;
    . = . + ((pharosCpuPartition49Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition49MmuLevel2End = .;

    /* reserve space for the partition 49 MMU tables level 3 according to its memory area size */
    pharosPartition49MmuLevel3Start = .;
    . = . + ((pharosCpuPartition49Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition49MmuLevel3End = .;


    /* reserve space for the partition 50 MMU tables level 2 according to its memory area size */
    pharosPartition50MmuLevel2Start = .;
    . = . + ((pharosCpuPartition50Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition50MmuLevel2End = .;

    /* reserve space for the partition 50 MMU tables level 3 according to its memory area size */
    pharosPartition50MmuLevel3Start = .;
    . = . + ((pharosCpuPartition50Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition50MmuLevel3End = .;


    /* reserve space for the partition 51 MMU tables level 2 according to its memory area size */
    pharosPartition51MmuLevel2Start = .;
    . = . + ((pharosCpuPartition51Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition51MmuLevel2End = .;

    /* reserve space for the partition 51 MMU tables level 3 according to its memory area size */
    pharosPartition51MmuLevel3Start = .;
    . = . + ((pharosCpuPartition51Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition51MmuLevel3End = .;


    /* reserve space for the partition 52 MMU tables level 2 according to its memory area size */
    pharosPartition52MmuLevel2Start = .;
    . = . + ((pharosCpuPartition52Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition52MmuLevel2End = .;

    /* reserve space for the partition 52 MMU tables level 3 according to its memory area size */
    pharosPartition52MmuLevel3Start = .;
    . = . + ((pharosCpuPartition52Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition52MmuLevel3End = .;


    /* reserve space for the partition 53 MMU tables level 2 according to its memory area size */
    pharosPartition53MmuLevel2Start = .;
    . = . + ((pharosCpuPartition53Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition53MmuLevel2End = .;

    /* reserve space for the partition 53 MMU tables level 3 according to its memory area size */
    pharosPartition53MmuLevel3Start = .;
    . = . + ((pharosCpuPartition53Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition53MmuLevel3End = .;


    /* reserve space for the partition 54 MMU tables level 2 according to its memory area size */
    pharosPartition54MmuLevel2Start = .;
    . = . + ((pharosCpuPartition54Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition54MmuLevel2End = .;

    /* reserve space for the partition 54 MMU tables level 3 according to its memory area size */
    pharosPartition54MmuLevel3Start = .;
    . = . + ((pharosCpuPartition54Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition54MmuLevel3End = .;


    /* reserve space for the partition 55 MMU tables level 2 according to its memory area size */
    pharosPartition55MmuLevel2Start = .;
    . = . + ((pharosCpuPartition55Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition55MmuLevel2End = .;

    /* reserve space for the partition 55 MMU tables level 3 according to its memory area size */
    pharosPartition55MmuLevel3Start = .;
    . = . + ((pharosCpuPartition55Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition55MmuLevel3End = .;


    /* reserve space for the partition 56 MMU tables level 2 according to its memory area size */
    pharosPartition56MmuLevel2Start = .;
    . = . + ((pharosCpuPartition56Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition56MmuLevel2End = .;

    /* reserve space for the partition 56 MMU tables level 3 according to its memory area size */
    pharosPartition56MmuLevel3Start = .;
    . = . + ((pharosCpuPartition56Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition56MmuLevel3End = .;


    /* reserve space for the partition 57 MMU tables level 2 according to its memory area size */
    pharosPartition57MmuLevel2Start = .;
    . = . + ((pharosCpuPartition57Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition57MmuLevel2End = .;

    /* reserve space for the partition 57 MMU tables level 3 according to its memory area size */
    pharosPartition57MmuLevel3Start = .;
    . = . + ((pharosCpuPartition57Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition57MmuLevel3End = .;


    /* reserve space for the partition 58 MMU tables level 2 according to its memory area size */
    pharosPartition58MmuLevel2Start = .;
    . = . + ((pharosCpuPartition58Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition58MmuLevel2End = .;

    /* reserve space for the partition 58 MMU tables level 3 according to its memory area size */
    pharosPartition58MmuLevel3Start = .;
    . = . + ((pharosCpuPartition58Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition58MmuLevel3End = .;


    /* reserve space for the partition 59 MMU tables level 2 according to its memory area size */
    pharosPartition59MmuLevel2Start = .;
    . = . + ((pharosCpuPartition59Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition59MmuLevel2End = .;

    /* reserve space for the partition 59 MMU tables level 3 according to its memory area size */
    pharosPartition59MmuLevel3Start = .;
    . = . + ((pharosCpuPartition59Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition59MmuLevel3End = .;


    /* reserve space for the partition 60 MMU tables level 2 according to its memory area size */
    pharosPartition60MmuLevel2Start = .;
    . = . + ((pharosCpuPartition60Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition60MmuLevel2End = .;

    /* reserve space for the partition 60 MMU tables level 3 according to its memory area size */
    pharosPartition60MmuLevel3Start = .;
    . = . + ((pharosCpuPartition60Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition60MmuLevel3End = .;


    /* reserve space for the partition 61 MMU tables level 2 according to its memory area size */
    pharosPartition61MmuLevel2Start = .;
    . = . + ((pharosCpuPartition61Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition61MmuLevel2End = .;

    /* reserve space for the partition 61 MMU tables level 3 according to its memory area size */
    pharosPartition61MmuLevel3Start = .;
    . = . + ((pharosCpuPartition61Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition61MmuLevel3End = .;


    /* reserve space for the partition 62 MMU tables level 2 according to its memory area size */
    pharosPartition62MmuLevel2Start = .;
    . = . + ((pharosCpuPartition62Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition62MmuLevel2End = .;

    /* reserve space for the partition 62 MMU tables level 3 according to its memory area size */
    pharosPartition62MmuLevel3Start = .;
    . = . + ((pharosCpuPartition62Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition62MmuLevel3End = .;


    /* reserve space for the partition 63 MMU tables level 2 according to its memory area size */
    pharosPartition63MmuLevel2Start = .;
    . = . + ((pharosCpuPartition63Size + pharosEndText) / 1024M + 1) * 4K;
    pharosPartition63MmuLevel2End = .;

    /* reserve space for the partition 63 MMU tables level 3 according to its memory area size */
    pharosPartition63MmuLevel3Start = .;
    . = . + ((pharosCpuPartition63Size + pharosEndText) / 2M + 1) * 4K;
    pharosPartition63MmuLevel3End = .;

  } > sdram
 
  .kernelBulks (NOLOAD) : 
  {
    CREATE_OBJECT_SYMBOLS

    /* need to align the bulks to their size at least */
    . = ALIGN(CHANNEL_BULK_SIZE);

    pharosBulkStart = .;

    *(.kernelBulks)  
    *(.kernelBulks.*)

    . = ALIGN(CHANNEL_BULK_SIZE);

    pharosBulkEnd = .;

    pharosKernelEnd = .;
  } > sdram
  

  /******************************************************************/
  /*                  Start of channel buffers area                 */
  /******************************************************************/

  /* pages of 4K */
  .pharosPage4K (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* align to section so as to not split the MMU page table into 2 sections */
    . = ALIGN (PAGE_START_ALIGN_4K);
    pharosPageStart4K = .;

    *(.pharosPage4K)
    
    . = ALIGN (PAGE_START_ALIGN_4K);
    pharosPageEnd4K = .;
  } > sdram 

  /* pages of 2M */
  .pharosPage2M (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS
    
    . = ALIGN (PAGE_START_ALIGN_2M);
    pharosPageStart2M = .;

    *(.pharosPage2M)
    
    . = ALIGN (PAGE_START_ALIGN_2M);
    pharosPageEnd2M = .;
  } > sdram


  /******************************************************************/
  /*                   End of channel buffers area                  */
  /******************************************************************/


  /******************************************************************/
  /*                  Start of shared stack area                    */
  /******************************************************************/

  .pharosSharedStack (NOLOAD): 
  {
    CREATE_OBJECT_SYMBOLS

    /* align to 2 MiB so that the page tables that reference this memory area are only for shared stacks and not for anything else */
    . = ALIGN (PARTITION_ALIGNMENT);

    pharosSharedStackStart = .;

    *(.pharosSharedStack)
    *(.pharosSharedStack.*)

    /* align to 2 MiB so that the page tables that reference this memory area are only for shared stacks and not for anything else */
    . = ALIGN (PARTITION_ALIGNMENT);
    pharosSharedStackEnd = .;

   
    pharosSharedStackTables2Start = .;

    /* here is the space for the level 2 tables to be used for all partitions to map the shared stack area (each table takes 4K) */
    . = . + (pharosNumberSharedStackMmuTables2PerPartition * 4K) * PARTITION_MAXIMUM_NUMBER;

    pharosSharedStackTables2End = .;   

    pharosSharedStackTables3Start = .;

    /* here is the space for the level 3 tables to be used for all partitions to map the shared stack area (each table takes 4K) */
    . = . + (pharosNumberSharedStackMmuTables3PerPartition * 4K) * PARTITION_MAXIMUM_NUMBER;

    pharosSharedStackTables3End = .;

  } > sdram
 
  /******************************************************************/
  /*                        End of kernel space                     */
  /******************************************************************/

  
  /******************************************************************/
  /*                     Start of Shared data area                  */
  /******************************************************************/

  pharosSharedDataLoadAddr = pharosSharedDataStart;
  pharosSharedDataSize = 0;
  pharosSharedBssSize = 0;


  /* Shared section data space */
  .shared : 
  {
    CREATE_OBJECT_SYMBOLS
    . = ALIGN (PARTITION_ALIGNMENT);
    . = ALIGN (8);

    pharosSharedStart = .;
    pharosSharedDataStart = .;

    *(.shared)
    . = ALIGN(8);

    pharosSharedDataEnd = .;

  } > sdram

  .sharedBss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS
    . = ALIGN (8);

    pharosSharedBssStart = .;

    *(.sharedBss)
    *(.sharedBss.*)

    . = ALIGN(8);

    pharosSharedBssEnd = .;

    /* place here the shared data that is NOT to be initialized at all */
    pharosSharedUssStart = .;

    *(.sharedUss)
    *(.sharedUss.*)

    pharosSharedUssEnd = .;

    pharosSharedEnd = .;
  } > sdram

  /******************************************************************/
  /*                        End of shared data                      */
  /******************************************************************/

  .interruptStackBss (NOLOAD): 
  {    
    CREATE_OBJECT_SYMBOLS
    . = ALIGN (PARTITION_ALIGNMENT);

    interruptStackBssStart = .;
    *(.interruptStackBss)
    interruptStackBssEnd = .;

  } > sdram




  /******************************************************************/
  /* Separate the partitions data space to enable memory protection */
  /******************************************************************/

  /* determine the size of the data in 64 bit variables */

  partition0DataSize = 0;
  partition1DataSize = 0;
  partition2DataSize = 0;
  partition3DataSize = 0;
  partition4DataSize = 0;
  partition5DataSize = 0;
  partition6DataSize = 0;
  partition7DataSize = 0;
  partition8DataSize = 0;
  partition9DataSize = 0;
  partition10DataSize = 0;
  partition11DataSize = 0;
  partition12DataSize = 0;
  partition13DataSize = 0;
  partition14DataSize = 0;
  partition15DataSize = 0;
  partition16DataSize = 0;
  partition17DataSize = 0;
  partition18DataSize = 0;
  partition19DataSize = 0;
  partition20DataSize = 0;
  partition21DataSize = 0;
  partition22DataSize = 0;
  partition23DataSize = 0;
  partition24DataSize = 0;
  partition25DataSize = 0;
  partition26DataSize = 0;
  partition27DataSize = 0;
  partition28DataSize = 0;
  partition29DataSize = 0;
  partition30DataSize = 0;
  partition31DataSize = 0;
  partition32DataSize = 0;
  partition33DataSize = 0;
  partition34DataSize = 0;
  partition35DataSize = 0;
  partition36DataSize = 0;
  partition37DataSize = 0;
  partition38DataSize = 0;
  partition39DataSize = 0;
  partition40DataSize = 0;
  partition41DataSize = 0;
  partition42DataSize = 0;
  partition43DataSize = 0;
  partition44DataSize = 0;
  partition45DataSize = 0;
  partition46DataSize = 0;
  partition47DataSize = 0;
  partition48DataSize = 0;
  partition49DataSize = 0;
  partition50DataSize = 0;
  partition51DataSize = 0;
  partition52DataSize = 0;
  partition53DataSize = 0;
  partition54DataSize = 0;
  partition55DataSize = 0;
  partition56DataSize = 0;
  partition57DataSize = 0;
  partition58DataSize = 0;
  partition59DataSize = 0;
  partition60DataSize = 0;
  partition61DataSize = 0;
  partition62DataSize = 0;
  partition63DataSize = 0;

  /* determine the size of the bss in 64 bit variables */

  partition0BssSize = 0;
  partition1BssSize = 0;
  partition2BssSize = 0;
  partition3BssSize = 0;
  partition4BssSize = 0;
  partition5BssSize = 0;
  partition6BssSize = 0;
  partition7BssSize = 0;
  partition8BssSize = 0;
  partition9BssSize = 0;
  partition10BssSize = 0;
  partition11BssSize = 0;
  partition12BssSize = 0;
  partition13BssSize = 0;
  partition14BssSize = 0;
  partition15BssSize = 0;
  partition16BssSize = 0;
  partition17BssSize = 0;
  partition18BssSize = 0;
  partition19BssSize = 0;
  partition20BssSize = 0;
  partition21BssSize = 0;
  partition22BssSize = 0;
  partition23BssSize = 0;
  partition24BssSize = 0;
  partition25BssSize = 0;
  partition26BssSize = 0;
  partition27BssSize = 0;
  partition28BssSize = 0;
  partition29BssSize = 0;
  partition30BssSize = 0;
  partition31BssSize = 0;
  partition32BssSize = 0;
  partition33BssSize = 0;
  partition34BssSize = 0;
  partition35BssSize = 0;
  partition36BssSize = 0;
  partition37BssSize = 0;
  partition38BssSize = 0;
  partition39BssSize = 0;
  partition40BssSize = 0;
  partition41BssSize = 0;
  partition42BssSize = 0;
  partition43BssSize = 0;
  partition44BssSize = 0;
  partition45BssSize = 0;
  partition46BssSize = 0;
  partition47BssSize = 0;
  partition48BssSize = 0;
  partition49BssSize = 0;
  partition50BssSize = 0;
  partition51BssSize = 0;
  partition52BssSize = 0;
  partition53BssSize = 0;
  partition54BssSize = 0;
  partition55BssSize = 0;
  partition56BssSize = 0;
  partition57BssSize = 0;
  partition58BssSize = 0;
  partition59BssSize = 0;
  partition60BssSize = 0;
  partition61BssSize = 0;
  partition62BssSize = 0;
  partition63BssSize = 0;


  /******************************************************************/
  /* Partition 0                                                    */
  /******************************************************************/

  /* address in the flash of the partition 0 data to copy it to RAM */
  pharosPartition0DataAddr = partition0DataStart;

  /* data of partition */
  .partition0 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition0Start = .;

    /* address of the partition 0 data section start in RAM */
    partition0DataStart = .;

    *(.partition0)
    *(.partition0.*)

    /* by default, the data section will go to partition 0 */
    *(.data)
    *(.data.*)
    *(.sdata)
    *(.sdata.*)

    /* address of the partition 0 data section end in RAM */
    . = ALIGN(8);
    partition0DataEnd = .;

  } > sdram

  /* bss of partition 0*/
  .partition0Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 0 bss section start in RAM */
    . = ALIGN(8);
    partition0BssStart = .;

    *(.partition0Bss)
    *(.partition0Bss.*)

     /* by default, the bss section will go to partition 0 */
     *(.bss)
     *(.bss.*)
     *(.sbss)
     *(.sbss.*)
     *(COMMON)
     *(COMMON.*)
     *(SCOMMON)
     *(SCOMMON.*)

    /* address of the shared partition 0 section end in RAM */
    . = ALIGN(8);
    partition0BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition0UssStart = .;

    *(.partition0Uss)
    *(.partition0Uss.*)

    partition0UssEnd = .;

    partition0End = .;

  } > sdram


  /******************************************************************/
  /* Partition 1                                                    */
  /******************************************************************/

  /* address in the flash of the partition 1 data to copy it to RAM */
  pharosPartition1DataAddr = partition1DataStart;

  /* data of partition */
  .partition1 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition1Start = .;

    /* address of the partition 1 data section start in RAM */
    partition1DataStart = .;

    *(.partition1)
    *(.partition1.*)

    /* address of the partition 1 data section end in RAM */
    . = ALIGN(8);
    partition1DataEnd = .;

  } > sdram

  /* bss of partition 1*/
  .partition1Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 1 bss section start in RAM */
    . = ALIGN(8);
    partition1BssStart = .;

    *(.partition1Bss)
    *(.partition1Bss.*)

    /* address of the shared partition 1 section end in RAM */
    . = ALIGN(8);
    partition1BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition1UssStart = .;

    *(.partition1Uss)
    *(.partition1Uss.*)

    partition1UssEnd = .;

    partition1End = .;

  } > sdram


  /******************************************************************/
  /* Partition 2                                                    */
  /******************************************************************/

  /* address in the flash of the partition 2 data to copy it to RAM */
  pharosPartition2DataAddr = partition2DataStart;

  /* data of partition */
  .partition2 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition2Start = .;

    /* address of the partition 2 data section start in RAM */
    partition2DataStart = .;

    *(.partition2)
    *(.partition2.*)

    /* address of the partition 2 data section end in RAM */
    . = ALIGN(8);
    partition2DataEnd = .;

  } > sdram

  /* bss of partition 2*/
  .partition2Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 2 bss section start in RAM */
    . = ALIGN(8);
    partition2BssStart = .;

    *(.partition2Bss)
    *(.partition2Bss.*)

    /* address of the shared partition 2 section end in RAM */
    . = ALIGN(8);
    partition2BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition2UssStart = .;

    *(.partition2Uss)
    *(.partition2Uss.*)

    partition2UssEnd = .;

    partition2End = .;

  } > sdram


  /******************************************************************/
  /* Partition 3                                                    */
  /******************************************************************/

  /* address in the flash of the partition 3 data to copy it to RAM */
  pharosPartition3DataAddr = partition3DataStart;

  /* data of partition */
  .partition3 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition3Start = .;

    /* address of the partition 3 data section start in RAM */
    partition3DataStart = .;

    *(.partition3)
    *(.partition3.*)

    /* address of the partition 3 data section end in RAM */
    . = ALIGN(8);
    partition3DataEnd = .;

  } > sdram

  /* bss of partition 3*/
  .partition3Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 3 bss section start in RAM */
    . = ALIGN(8);
    partition3BssStart = .;

    *(.partition3Bss)
    *(.partition3Bss.*)

    /* address of the shared partition 3 section end in RAM */
    . = ALIGN(8);
    partition3BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition3UssStart = .;

    *(.partition3Uss)
    *(.partition3Uss.*)

    partition3UssEnd = .;

    partition3End = .;

  } > sdram


  /******************************************************************/
  /* Partition 4                                                    */
  /******************************************************************/

  /* address in the flash of the partition 4 data to copy it to RAM */
  pharosPartition4DataAddr = partition4DataStart;

  /* data of partition */
  .partition4 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition4Start = .;

    /* address of the partition 4 data section start in RAM */
    partition4DataStart = .;

    *(.partition4)
    *(.partition4.*)

    /* address of the partition 4 data section end in RAM */
    . = ALIGN(8);
    partition4DataEnd = .;

  } > sdram

  /* bss of partition 4*/
  .partition4Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 4 bss section start in RAM */
    . = ALIGN(8);
    partition4BssStart = .;

    *(.partition4Bss)
    *(.partition4Bss.*)

    /* address of the shared partition 4 section end in RAM */
    . = ALIGN(8);
    partition4BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition4UssStart = .;

    *(.partition4Uss)
    *(.partition4Uss.*)

    partition4UssEnd = .;

    partition4End = .;

  } > sdram


  /******************************************************************/
  /* Partition 5                                                    */
  /******************************************************************/

  /* address in the flash of the partition 5 data to copy it to RAM */
  pharosPartition5DataAddr = partition5DataStart;

  /* data of partition */
  .partition5 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition5Start = .;

    /* address of the partition 5 data section start in RAM */
    partition5DataStart = .;

    *(.partition5)
    *(.partition5.*)

    /* address of the partition 5 data section end in RAM */
    . = ALIGN(8);
    partition5DataEnd = .;

  } > sdram

  /* bss of partition 5*/
  .partition5Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 5 bss section start in RAM */
    . = ALIGN(8);
    partition5BssStart = .;

    *(.partition5Bss)
    *(.partition5Bss.*)

    /* address of the shared partition 5 section end in RAM */
    . = ALIGN(8);
    partition5BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition5UssStart = .;

    *(.partition5Uss)
    *(.partition5Uss.*)

    partition5UssEnd = .;

    partition5End = .;

  } > sdram


  /******************************************************************/
  /* Partition 6                                                    */
  /******************************************************************/

  /* address in the flash of the partition 6 data to copy it to RAM */
  pharosPartition6DataAddr = partition6DataStart;

  /* data of partition */
  .partition6 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition6Start = .;

    /* address of the partition 6 data section start in RAM */
    partition6DataStart = .;

    *(.partition6)
    *(.partition6.*)

    /* address of the partition 6 data section end in RAM */
    . = ALIGN(8);
    partition6DataEnd = .;

  } > sdram

  /* bss of partition 6*/
  .partition6Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 6 bss section start in RAM */
    . = ALIGN(8);
    partition6BssStart = .;

    *(.partition6Bss)
    *(.partition6Bss.*)

    /* address of the shared partition 6 section end in RAM */
    . = ALIGN(8);
    partition6BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition6UssStart = .;

    *(.partition6Uss)
    *(.partition6Uss.*)

    partition6UssEnd = .;

    partition6End = .;

  } > sdram


  /******************************************************************/
  /* Partition 7                                                    */
  /******************************************************************/

  /* address in the flash of the partition 7 data to copy it to RAM */
  pharosPartition7DataAddr = partition7DataStart;

  /* data of partition */
  .partition7 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition7Start = .;

    /* address of the partition 7 data section start in RAM */
    partition7DataStart = .;

    *(.partition7)
    *(.partition7.*)

    /* address of the partition 7 data section end in RAM */
    . = ALIGN(8);
    partition7DataEnd = .;

  } > sdram

  /* bss of partition 7*/
  .partition7Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 7 bss section start in RAM */
    . = ALIGN(8);
    partition7BssStart = .;

    *(.partition7Bss)
    *(.partition7Bss.*)

    /* address of the shared partition 7 section end in RAM */
    . = ALIGN(8);
    partition7BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition7UssStart = .;

    *(.partition7Uss)
    *(.partition7Uss.*)

    partition7UssEnd = .;

    partition7End = .;

  } > sdram


  /******************************************************************/
  /* Partition 8                                                    */
  /******************************************************************/

  /* address in the flash of the partition 8 data to copy it to RAM */
  pharosPartition8DataAddr = partition8DataStart;

  /* data of partition */
  .partition8 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition8Start = .;

    /* address of the partition 8 data section start in RAM */
    partition8DataStart = .;

    *(.partition8)
    *(.partition8.*)

    /* address of the partition 8 data section end in RAM */
    . = ALIGN(8);
    partition8DataEnd = .;

  } > sdram

  /* bss of partition 8*/
  .partition8Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 8 bss section start in RAM */
    . = ALIGN(8);
    partition8BssStart = .;

    *(.partition8Bss)
    *(.partition8Bss.*)

    /* address of the shared partition 8 section end in RAM */
    . = ALIGN(8);
    partition8BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition8UssStart = .;

    *(.partition8Uss)
    *(.partition8Uss.*)

    partition8UssEnd = .;

    partition8End = .;

  } > sdram


  /******************************************************************/
  /* Partition 9                                                    */
  /******************************************************************/

  /* address in the flash of the partition 9 data to copy it to RAM */
  pharosPartition9DataAddr = partition9DataStart;

  /* data of partition */
  .partition9 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition9Start = .;

    /* address of the partition 9 data section start in RAM */
    partition9DataStart = .;

    *(.partition9)
    *(.partition9.*)

    /* address of the partition 9 data section end in RAM */
    . = ALIGN(8);
    partition9DataEnd = .;

  } > sdram

  /* bss of partition 9*/
  .partition9Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 9 bss section start in RAM */
    . = ALIGN(8);
    partition9BssStart = .;

    *(.partition9Bss)
    *(.partition9Bss.*)

    /* address of the shared partition 9 section end in RAM */
    . = ALIGN(8);
    partition9BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition9UssStart = .;

    *(.partition9Uss)
    *(.partition9Uss.*)

    partition9UssEnd = .;

    partition9End = .;

  } > sdram


  /******************************************************************/
  /* Partition 10                                                   */
  /******************************************************************/

  /* address in the flash of the partition 10 data to copy it to RAM */
  pharosPartition10DataAddr = partition10DataStart;

  /* data of partition */
  .partition10 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition10Start = .;

    /* address of the partition 10 data section start in RAM */
    partition10DataStart = .;

    *(.partition10)
    *(.partition10.*)

    /* address of the partition 10 data section end in RAM */
    . = ALIGN(8);
    partition10DataEnd = .;

  } > sdram

  /* bss of partition 10*/
  .partition10Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 10 bss section start in RAM */
    . = ALIGN(8);
    partition10BssStart = .;

    *(.partition10Bss)
    *(.partition10Bss.*)

    /* address of the shared partition 10 section end in RAM */
    . = ALIGN(8);
    partition10BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition10UssStart = .;

    *(.partition10Uss)
    *(.partition10Uss.*)

    partition10UssEnd = .;

    partition10End = .;

  } > sdram


  /******************************************************************/
  /* Partition 11                                                   */
  /******************************************************************/

  /* address in the flash of the partition 11 data to copy it to RAM */
  pharosPartition11DataAddr = partition11DataStart;

  /* data of partition */
  .partition11 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition11Start = .;

    /* address of the partition 11 data section start in RAM */
    partition11DataStart = .;

    *(.partition11)
    *(.partition11.*)

    /* address of the partition 11 data section end in RAM */
    . = ALIGN(8);
    partition11DataEnd = .;

  } > sdram

  /* bss of partition 11*/
  .partition11Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 11 bss section start in RAM */
    . = ALIGN(8);
    partition11BssStart = .;

    *(.partition11Bss)
    *(.partition11Bss.*)

    /* address of the shared partition 11 section end in RAM */
    . = ALIGN(8);
    partition11BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition11UssStart = .;

    *(.partition11Uss)
    *(.partition11Uss.*)

    partition11UssEnd = .;

    partition11End = .;

  } > sdram


  /******************************************************************/
  /* Partition 12                                                   */
  /******************************************************************/

  /* address in the flash of the partition 12 data to copy it to RAM */
  pharosPartition12DataAddr = partition12DataStart;

  /* data of partition */
  .partition12 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition12Start = .;

    /* address of the partition 12 data section start in RAM */
    partition12DataStart = .;

    *(.partition12)
    *(.partition12.*)

    /* address of the partition 12 data section end in RAM */
    . = ALIGN(8);
    partition12DataEnd = .;

  } > sdram

  /* bss of partition 12*/
  .partition12Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 12 bss section start in RAM */
    . = ALIGN(8);
    partition12BssStart = .;

    *(.partition12Bss)
    *(.partition12Bss.*)

    /* address of the shared partition 12 section end in RAM */
    . = ALIGN(8);
    partition12BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition12UssStart = .;

    *(.partition12Uss)
    *(.partition12Uss.*)

    partition12UssEnd = .;

    partition12End = .;

  } > sdram


  /******************************************************************/
  /* Partition 13                                                   */
  /******************************************************************/

  /* address in the flash of the partition 13 data to copy it to RAM */
  pharosPartition13DataAddr = partition13DataStart;

  /* data of partition */
  .partition13 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition13Start = .;

    /* address of the partition 13 data section start in RAM */
    partition13DataStart = .;

    *(.partition13)
    *(.partition13.*)

    /* address of the partition 13 data section end in RAM */
    . = ALIGN(8);
    partition13DataEnd = .;

  } > sdram

  /* bss of partition 13*/
  .partition13Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 13 bss section start in RAM */
    . = ALIGN(8);
    partition13BssStart = .;

    *(.partition13Bss)
    *(.partition13Bss.*)

    /* address of the shared partition 13 section end in RAM */
    . = ALIGN(8);
    partition13BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition13UssStart = .;

    *(.partition13Uss)
    *(.partition13Uss.*)

    partition13UssEnd = .;

    partition13End = .;

  } > sdram


  /******************************************************************/
  /* Partition 14                                                   */
  /******************************************************************/

  /* address in the flash of the partition 14 data to copy it to RAM */
  pharosPartition14DataAddr = partition14DataStart;

  /* data of partition */
  .partition14 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition14Start = .;

    /* address of the partition 14 data section start in RAM */
    partition14DataStart = .;

    *(.partition14)
    *(.partition14.*)

    /* address of the partition 14 data section end in RAM */
    . = ALIGN(8);
    partition14DataEnd = .;

  } > sdram

  /* bss of partition 14*/
  .partition14Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 14 bss section start in RAM */
    . = ALIGN(8);
    partition14BssStart = .;

    *(.partition14Bss)
    *(.partition14Bss.*)

    /* address of the shared partition 14 section end in RAM */
    . = ALIGN(8);
    partition14BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition14UssStart = .;

    *(.partition14Uss)
    *(.partition14Uss.*)

    partition14UssEnd = .;

    partition14End = .;

  } > sdram


  /******************************************************************/
  /* Partition 15                                                   */
  /******************************************************************/

  /* address in the flash of the partition 15 data to copy it to RAM */
  pharosPartition15DataAddr = partition15DataStart;

  /* data of partition */
  .partition15 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition15Start = .;

    /* address of the partition 15 data section start in RAM */
    partition15DataStart = .;

    *(.partition15)
    *(.partition15.*)

    /* address of the partition 15 data section end in RAM */
    . = ALIGN(8);
    partition15DataEnd = .;

  } > sdram

  /* bss of partition 15*/
  .partition15Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 15 bss section start in RAM */
    . = ALIGN(8);
    partition15BssStart = .;

    *(.partition15Bss)
    *(.partition15Bss.*)

    /* address of the shared partition 15 section end in RAM */
    . = ALIGN(8);
    partition15BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition15UssStart = .;

    *(.partition15Uss)
    *(.partition15Uss.*)

    partition15UssEnd = .;

    partition15End = .;

  } > sdram


  /******************************************************************/
  /* Partition 16                                                   */
  /******************************************************************/

  /* address in the flash of the partition 16 data to copy it to RAM */
  pharosPartition16DataAddr = partition16DataStart;

  /* data of partition */
  .partition16 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition16Start = .;

    /* address of the partition 16 data section start in RAM */
    partition16DataStart = .;

    *(.partition16)
    *(.partition16.*)

    /* address of the partition 16 data section end in RAM */
    . = ALIGN(8);
    partition16DataEnd = .;

  } > sdram

  /* bss of partition 16*/
  .partition16Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 16 bss section start in RAM */
    . = ALIGN(8);
    partition16BssStart = .;

    *(.partition16Bss)
    *(.partition16Bss.*)

    /* address of the shared partition 16 section end in RAM */
    . = ALIGN(8);
    partition16BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition16UssStart = .;

    *(.partition16Uss)
    *(.partition16Uss.*)

    partition16UssEnd = .;

    partition16End = .;

  } > sdram


  /******************************************************************/
  /* Partition 17                                                   */
  /******************************************************************/

  /* address in the flash of the partition 17 data to copy it to RAM */
  pharosPartition17DataAddr = partition17DataStart;

  /* data of partition */
  .partition17 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition17Start = .;

    /* address of the partition 17 data section start in RAM */
    partition17DataStart = .;

    *(.partition17)
    *(.partition17.*)

    /* address of the partition 17 data section end in RAM */
    . = ALIGN(8);
    partition17DataEnd = .;

  } > sdram

  /* bss of partition 17*/
  .partition17Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 17 bss section start in RAM */
    . = ALIGN(8);
    partition17BssStart = .;

    *(.partition17Bss)
    *(.partition17Bss.*)

    /* address of the shared partition 17 section end in RAM */
    . = ALIGN(8);
    partition17BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition17UssStart = .;

    *(.partition17Uss)
    *(.partition17Uss.*)

    partition17UssEnd = .;

    partition17End = .;

  } > sdram


  /******************************************************************/
  /* Partition 18                                                   */
  /******************************************************************/

  /* address in the flash of the partition 18 data to copy it to RAM */
  pharosPartition18DataAddr = partition18DataStart;

  /* data of partition */
  .partition18 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition18Start = .;

    /* address of the partition 18 data section start in RAM */
    partition18DataStart = .;

    *(.partition18)
    *(.partition18.*)

    /* address of the partition 18 data section end in RAM */
    . = ALIGN(8);
    partition18DataEnd = .;

  } > sdram

  /* bss of partition 18*/
  .partition18Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 18 bss section start in RAM */
    . = ALIGN(8);
    partition18BssStart = .;

    *(.partition18Bss)
    *(.partition18Bss.*)

    /* address of the shared partition 18 section end in RAM */
    . = ALIGN(8);
    partition18BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition18UssStart = .;

    *(.partition18Uss)
    *(.partition18Uss.*)

    partition18UssEnd = .;

    partition18End = .;

  } > sdram


  /******************************************************************/
  /* Partition 19                                                   */
  /******************************************************************/

  /* address in the flash of the partition 19 data to copy it to RAM */
  pharosPartition19DataAddr = partition19DataStart;

  /* data of partition */
  .partition19 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition19Start = .;

    /* address of the partition 19 data section start in RAM */
    partition19DataStart = .;

    *(.partition19)
    *(.partition19.*)

    /* address of the partition 19 data section end in RAM */
    . = ALIGN(8);
    partition19DataEnd = .;

  } > sdram

  /* bss of partition 19*/
  .partition19Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 19 bss section start in RAM */
    . = ALIGN(8);
    partition19BssStart = .;

    *(.partition19Bss)
    *(.partition19Bss.*)

    /* address of the shared partition 19 section end in RAM */
    . = ALIGN(8);
    partition19BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition19UssStart = .;

    *(.partition19Uss)
    *(.partition19Uss.*)

    partition19UssEnd = .;

    partition19End = .;

  } > sdram


  /******************************************************************/
  /* Partition 20                                                   */
  /******************************************************************/

  /* address in the flash of the partition 20 data to copy it to RAM */
  pharosPartition20DataAddr = partition20DataStart;

  /* data of partition */
  .partition20 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition20Start = .;

    /* address of the partition 20 data section start in RAM */
    partition20DataStart = .;

    *(.partition20)
    *(.partition20.*)

    /* address of the partition 20 data section end in RAM */
    . = ALIGN(8);
    partition20DataEnd = .;

  } > sdram

  /* bss of partition 20*/
  .partition20Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 20 bss section start in RAM */
    . = ALIGN(8);
    partition20BssStart = .;

    *(.partition20Bss)
    *(.partition20Bss.*)

    /* address of the shared partition 20 section end in RAM */
    . = ALIGN(8);
    partition20BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition20UssStart = .;

    *(.partition20Uss)
    *(.partition20Uss.*)

    partition20UssEnd = .;

    partition20End = .;

  } > sdram


  /******************************************************************/
  /* Partition 21                                                   */
  /******************************************************************/

  /* address in the flash of the partition 21 data to copy it to RAM */
  pharosPartition21DataAddr = partition21DataStart;

  /* data of partition */
  .partition21 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition21Start = .;

    /* address of the partition 21 data section start in RAM */
    partition21DataStart = .;

    *(.partition21)
    *(.partition21.*)

    /* address of the partition 21 data section end in RAM */
    . = ALIGN(8);
    partition21DataEnd = .;

  } > sdram

  /* bss of partition 21*/
  .partition21Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 21 bss section start in RAM */
    . = ALIGN(8);
    partition21BssStart = .;

    *(.partition21Bss)
    *(.partition21Bss.*)

    /* address of the shared partition 21 section end in RAM */
    . = ALIGN(8);
    partition21BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition21UssStart = .;

    *(.partition21Uss)
    *(.partition21Uss.*)

    partition21UssEnd = .;

    partition21End = .;

  } > sdram


  /******************************************************************/
  /* Partition 22                                                   */
  /******************************************************************/

  /* address in the flash of the partition 22 data to copy it to RAM */
  pharosPartition22DataAddr = partition22DataStart;

  /* data of partition */
  .partition22 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition22Start = .;

    /* address of the partition 22 data section start in RAM */
    partition22DataStart = .;

    *(.partition22)
    *(.partition22.*)

    /* address of the partition 22 data section end in RAM */
    . = ALIGN(8);
    partition22DataEnd = .;

  } > sdram

  /* bss of partition 22*/
  .partition22Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 22 bss section start in RAM */
    . = ALIGN(8);
    partition22BssStart = .;

    *(.partition22Bss)
    *(.partition22Bss.*)

    /* address of the shared partition 22 section end in RAM */
    . = ALIGN(8);
    partition22BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition22UssStart = .;

    *(.partition22Uss)
    *(.partition22Uss.*)

    partition22UssEnd = .;

    partition22End = .;

  } > sdram


  /******************************************************************/
  /* Partition 23                                                   */
  /******************************************************************/

  /* address in the flash of the partition 23 data to copy it to RAM */
  pharosPartition23DataAddr = partition23DataStart;

  /* data of partition */
  .partition23 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition23Start = .;

    /* address of the partition 23 data section start in RAM */
    partition23DataStart = .;

    *(.partition23)
    *(.partition23.*)

    /* address of the partition 23 data section end in RAM */
    . = ALIGN(8);
    partition23DataEnd = .;

  } > sdram

  /* bss of partition 23*/
  .partition23Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 23 bss section start in RAM */
    . = ALIGN(8);
    partition23BssStart = .;

    *(.partition23Bss)
    *(.partition23Bss.*)

    /* address of the shared partition 23 section end in RAM */
    . = ALIGN(8);
    partition23BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition23UssStart = .;

    *(.partition23Uss)
    *(.partition23Uss.*)

    partition23UssEnd = .;

    partition23End = .;

  } > sdram


  /******************************************************************/
  /* Partition 24                                                   */
  /******************************************************************/

  /* address in the flash of the partition 24 data to copy it to RAM */
  pharosPartition24DataAddr = partition24DataStart;

  /* data of partition */
  .partition24 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition24Start = .;

    /* address of the partition 24 data section start in RAM */
    partition24DataStart = .;

    *(.partition24)
    *(.partition24.*)

    /* address of the partition 24 data section end in RAM */
    . = ALIGN(8);
    partition24DataEnd = .;

  } > sdram

  /* bss of partition 24*/
  .partition24Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 24 bss section start in RAM */
    . = ALIGN(8);
    partition24BssStart = .;

    *(.partition24Bss)
    *(.partition24Bss.*)

    /* address of the shared partition 24 section end in RAM */
    . = ALIGN(8);
    partition24BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition24UssStart = .;

    *(.partition24Uss)
    *(.partition24Uss.*)

    partition24UssEnd = .;

    partition24End = .;

  } > sdram


  /******************************************************************/
  /* Partition 25                                                   */
  /******************************************************************/

  /* address in the flash of the partition 25 data to copy it to RAM */
  pharosPartition25DataAddr = partition25DataStart;

  /* data of partition */
  .partition25 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition25Start = .;

    /* address of the partition 25 data section start in RAM */
    partition25DataStart = .;

    *(.partition25)
    *(.partition25.*)

    /* address of the partition 25 data section end in RAM */
    . = ALIGN(8);
    partition25DataEnd = .;

  } > sdram

  /* bss of partition 25*/
  .partition25Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 25 bss section start in RAM */
    . = ALIGN(8);
    partition25BssStart = .;

    *(.partition25Bss)
    *(.partition25Bss.*)

    /* address of the shared partition 25 section end in RAM */
    . = ALIGN(8);
    partition25BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition25UssStart = .;

    *(.partition25Uss)
    *(.partition25Uss.*)

    partition25UssEnd = .;

    partition25End = .;

  } > sdram


  /******************************************************************/
  /* Partition 26                                                   */
  /******************************************************************/

  /* address in the flash of the partition 26 data to copy it to RAM */
  pharosPartition26DataAddr = partition26DataStart;

  /* data of partition */
  .partition26 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition26Start = .;

    /* address of the partition 26 data section start in RAM */
    partition26DataStart = .;

    *(.partition26)
    *(.partition26.*)

    /* address of the partition 26 data section end in RAM */
    . = ALIGN(8);
    partition26DataEnd = .;

  } > sdram

  /* bss of partition 26*/
  .partition26Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 26 bss section start in RAM */
    . = ALIGN(8);
    partition26BssStart = .;

    *(.partition26Bss)
    *(.partition26Bss.*)

    /* address of the shared partition 26 section end in RAM */
    . = ALIGN(8);
    partition26BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition26UssStart = .;

    *(.partition26Uss)
    *(.partition26Uss.*)

    partition26UssEnd = .;

    partition26End = .;

  } > sdram


  /******************************************************************/
  /* Partition 27                                                   */
  /******************************************************************/

  /* address in the flash of the partition 27 data to copy it to RAM */
  pharosPartition27DataAddr = partition27DataStart;

  /* data of partition */
  .partition27 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition27Start = .;

    /* address of the partition 27 data section start in RAM */
    partition27DataStart = .;

    *(.partition27)
    *(.partition27.*)

    /* address of the partition 27 data section end in RAM */
    . = ALIGN(8);
    partition27DataEnd = .;

  } > sdram

  /* bss of partition 27*/
  .partition27Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 27 bss section start in RAM */
    . = ALIGN(8);
    partition27BssStart = .;

    *(.partition27Bss)
    *(.partition27Bss.*)

    /* address of the shared partition 27 section end in RAM */
    . = ALIGN(8);
    partition27BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition27UssStart = .;

    *(.partition27Uss)
    *(.partition27Uss.*)

    partition27UssEnd = .;

    partition27End = .;

  } > sdram


  /******************************************************************/
  /* Partition 28                                                   */
  /******************************************************************/

  /* address in the flash of the partition 28 data to copy it to RAM */
  pharosPartition28DataAddr = partition28DataStart;

  /* data of partition */
  .partition28 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition28Start = .;

    /* address of the partition 28 data section start in RAM */
    partition28DataStart = .;

    *(.partition28)
    *(.partition28.*)

    /* address of the partition 28 data section end in RAM */
    . = ALIGN(8);
    partition28DataEnd = .;

  } > sdram

  /* bss of partition 28*/
  .partition28Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 28 bss section start in RAM */
    . = ALIGN(8);
    partition28BssStart = .;

    *(.partition28Bss)
    *(.partition28Bss.*)

    /* address of the shared partition 28 section end in RAM */
    . = ALIGN(8);
    partition28BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition28UssStart = .;

    *(.partition28Uss)
    *(.partition28Uss.*)

    partition28UssEnd = .;

    partition28End = .;

  } > sdram


  /******************************************************************/
  /* Partition 29                                                   */
  /******************************************************************/

  /* address in the flash of the partition 29 data to copy it to RAM */
  pharosPartition29DataAddr = partition29DataStart;

  /* data of partition */
  .partition29 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition29Start = .;

    /* address of the partition 29 data section start in RAM */
    partition29DataStart = .;

    *(.partition29)
    *(.partition29.*)

    /* address of the partition 29 data section end in RAM */
    . = ALIGN(8);
    partition29DataEnd = .;

  } > sdram

  /* bss of partition 29*/
  .partition29Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 29 bss section start in RAM */
    . = ALIGN(8);
    partition29BssStart = .;

    *(.partition29Bss)
    *(.partition29Bss.*)

    /* address of the shared partition 29 section end in RAM */
    . = ALIGN(8);
    partition29BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition29UssStart = .;

    *(.partition29Uss)
    *(.partition29Uss.*)

    partition29UssEnd = .;

    partition29End = .;

  } > sdram


  /******************************************************************/
  /* Partition 30                                                   */
  /******************************************************************/

  /* address in the flash of the partition 30 data to copy it to RAM */
  pharosPartition30DataAddr = partition30DataStart;

  /* data of partition */
  .partition30 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition30Start = .;

    /* address of the partition 30 data section start in RAM */
    partition30DataStart = .;

    *(.partition30)
    *(.partition30.*)

    /* address of the partition 30 data section end in RAM */
    . = ALIGN(8);
    partition30DataEnd = .;

  } > sdram

  /* bss of partition 30*/
  .partition30Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 30 bss section start in RAM */
    . = ALIGN(8);
    partition30BssStart = .;

    *(.partition30Bss)
    *(.partition30Bss.*)

    /* address of the shared partition 30 section end in RAM */
    . = ALIGN(8);
    partition30BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition30UssStart = .;

    *(.partition30Uss)
    *(.partition30Uss.*)

    partition30UssEnd = .;

    partition30End = .;

  } > sdram


  /******************************************************************/
  /* Partition 31                                                   */
  /******************************************************************/

  /* address in the flash of the partition 31 data to copy it to RAM */
  pharosPartition31DataAddr = partition31DataStart;

  /* data of partition */
  .partition31 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition31Start = .;

    /* address of the partition 31 data section start in RAM */
    partition31DataStart = .;

    *(.partition31)
    *(.partition31.*)

    /* address of the partition 31 data section end in RAM */
    . = ALIGN(8);
    partition31DataEnd = .;

  } > sdram

  /* bss of partition 31*/
  .partition31Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 31 bss section start in RAM */
    . = ALIGN(8);
    partition31BssStart = .;

    *(.partition31Bss)
    *(.partition31Bss.*)

    /* address of the shared partition 31 section end in RAM */
    . = ALIGN(8);
    partition31BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition31UssStart = .;

    *(.partition31Uss)
    *(.partition31Uss.*)

    partition31UssEnd = .;

    partition31End = .;

  } > sdram


  /******************************************************************/
  /* Partition 32                                                   */
  /******************************************************************/

  /* address in the flash of the partition 32 data to copy it to RAM */
  pharosPartition32DataAddr = partition32DataStart;

  /* data of partition */
  .partition32 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition32Start = .;

    /* address of the partition 32 data section start in RAM */
    partition32DataStart = .;

    *(.partition32)
    *(.partition32.*)

    /* address of the partition 32 data section end in RAM */
    . = ALIGN(8);
    partition32DataEnd = .;

  } > sdram

  /* bss of partition 32*/
  .partition32Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 32 bss section start in RAM */
    . = ALIGN(8);
    partition32BssStart = .;

    *(.partition32Bss)
    *(.partition32Bss.*)

    /* address of the shared partition 32 section end in RAM */
    . = ALIGN(8);
    partition32BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition32UssStart = .;

    *(.partition32Uss)
    *(.partition32Uss.*)

    partition32UssEnd = .;

    partition32End = .;

  } > sdram


  /******************************************************************/
  /* Partition 33                                                   */
  /******************************************************************/

  /* address in the flash of the partition 33 data to copy it to RAM */
  pharosPartition33DataAddr = partition33DataStart;

  /* data of partition */
  .partition33 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition33Start = .;

    /* address of the partition 33 data section start in RAM */
    partition33DataStart = .;

    *(.partition33)
    *(.partition33.*)

    /* address of the partition 33 data section end in RAM */
    . = ALIGN(8);
    partition33DataEnd = .;

  } > sdram

  /* bss of partition 33*/
  .partition33Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 33 bss section start in RAM */
    . = ALIGN(8);
    partition33BssStart = .;

    *(.partition33Bss)
    *(.partition33Bss.*)

    /* address of the shared partition 33 section end in RAM */
    . = ALIGN(8);
    partition33BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition33UssStart = .;

    *(.partition33Uss)
    *(.partition33Uss.*)

    partition33UssEnd = .;

    partition33End = .;

  } > sdram


  /******************************************************************/
  /* Partition 34                                                   */
  /******************************************************************/

  /* address in the flash of the partition 34 data to copy it to RAM */
  pharosPartition34DataAddr = partition34DataStart;

  /* data of partition */
  .partition34 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition34Start = .;

    /* address of the partition 34 data section start in RAM */
    partition34DataStart = .;

    *(.partition34)
    *(.partition34.*)

    /* address of the partition 34 data section end in RAM */
    . = ALIGN(8);
    partition34DataEnd = .;

  } > sdram

  /* bss of partition 34*/
  .partition34Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 34 bss section start in RAM */
    . = ALIGN(8);
    partition34BssStart = .;

    *(.partition34Bss)
    *(.partition34Bss.*)

    /* address of the shared partition 34 section end in RAM */
    . = ALIGN(8);
    partition34BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition34UssStart = .;

    *(.partition34Uss)
    *(.partition34Uss.*)

    partition34UssEnd = .;

    partition34End = .;

  } > sdram


  /******************************************************************/
  /* Partition 35                                                   */
  /******************************************************************/

  /* address in the flash of the partition 35 data to copy it to RAM */
  pharosPartition35DataAddr = partition35DataStart;

  /* data of partition */
  .partition35 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition35Start = .;

    /* address of the partition 35 data section start in RAM */
    partition35DataStart = .;

    *(.partition35)
    *(.partition35.*)

    /* address of the partition 35 data section end in RAM */
    . = ALIGN(8);
    partition35DataEnd = .;

  } > sdram

  /* bss of partition 35*/
  .partition35Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 35 bss section start in RAM */
    . = ALIGN(8);
    partition35BssStart = .;

    *(.partition35Bss)
    *(.partition35Bss.*)

    /* address of the shared partition 35 section end in RAM */
    . = ALIGN(8);
    partition35BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition35UssStart = .;

    *(.partition35Uss)
    *(.partition35Uss.*)

    partition35UssEnd = .;

    partition35End = .;

  } > sdram


  /******************************************************************/
  /* Partition 36                                                   */
  /******************************************************************/

  /* address in the flash of the partition 36 data to copy it to RAM */
  pharosPartition36DataAddr = partition36DataStart;

  /* data of partition */
  .partition36 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition36Start = .;

    /* address of the partition 36 data section start in RAM */
    partition36DataStart = .;

    *(.partition36)
    *(.partition36.*)

    /* address of the partition 36 data section end in RAM */
    . = ALIGN(8);
    partition36DataEnd = .;

  } > sdram

  /* bss of partition 36*/
  .partition36Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 36 bss section start in RAM */
    . = ALIGN(8);
    partition36BssStart = .;

    *(.partition36Bss)
    *(.partition36Bss.*)

    /* address of the shared partition 36 section end in RAM */
    . = ALIGN(8);
    partition36BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition36UssStart = .;

    *(.partition36Uss)
    *(.partition36Uss.*)

    partition36UssEnd = .;

    partition36End = .;

  } > sdram


  /******************************************************************/
  /* Partition 37                                                   */
  /******************************************************************/

  /* address in the flash of the partition 37 data to copy it to RAM */
  pharosPartition37DataAddr = partition37DataStart;

  /* data of partition */
  .partition37 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition37Start = .;

    /* address of the partition 37 data section start in RAM */
    partition37DataStart = .;

    *(.partition37)
    *(.partition37.*)

    /* address of the partition 37 data section end in RAM */
    . = ALIGN(8);
    partition37DataEnd = .;

  } > sdram

  /* bss of partition 37*/
  .partition37Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 37 bss section start in RAM */
    . = ALIGN(8);
    partition37BssStart = .;

    *(.partition37Bss)
    *(.partition37Bss.*)

    /* address of the shared partition 37 section end in RAM */
    . = ALIGN(8);
    partition37BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition37UssStart = .;

    *(.partition37Uss)
    *(.partition37Uss.*)

    partition37UssEnd = .;

    partition37End = .;

  } > sdram


  /******************************************************************/
  /* Partition 38                                                   */
  /******************************************************************/

  /* address in the flash of the partition 38 data to copy it to RAM */
  pharosPartition38DataAddr = partition38DataStart;

  /* data of partition */
  .partition38 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition38Start = .;

    /* address of the partition 38 data section start in RAM */
    partition38DataStart = .;

    *(.partition38)
    *(.partition38.*)

    /* address of the partition 38 data section end in RAM */
    . = ALIGN(8);
    partition38DataEnd = .;

  } > sdram

  /* bss of partition 38*/
  .partition38Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 38 bss section start in RAM */
    . = ALIGN(8);
    partition38BssStart = .;

    *(.partition38Bss)
    *(.partition38Bss.*)

    /* address of the shared partition 38 section end in RAM */
    . = ALIGN(8);
    partition38BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition38UssStart = .;

    *(.partition38Uss)
    *(.partition38Uss.*)

    partition38UssEnd = .;

    partition38End = .;

  } > sdram


  /******************************************************************/
  /* Partition 39                                                   */
  /******************************************************************/

  /* address in the flash of the partition 39 data to copy it to RAM */
  pharosPartition39DataAddr = partition39DataStart;

  /* data of partition */
  .partition39 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition39Start = .;

    /* address of the partition 39 data section start in RAM */
    partition39DataStart = .;

    *(.partition39)
    *(.partition39.*)

    /* address of the partition 39 data section end in RAM */
    . = ALIGN(8);
    partition39DataEnd = .;

  } > sdram

  /* bss of partition 39*/
  .partition39Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 39 bss section start in RAM */
    . = ALIGN(8);
    partition39BssStart = .;

    *(.partition39Bss)
    *(.partition39Bss.*)

    /* address of the shared partition 39 section end in RAM */
    . = ALIGN(8);
    partition39BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition39UssStart = .;

    *(.partition39Uss)
    *(.partition39Uss.*)

    partition39UssEnd = .;

    partition39End = .;

  } > sdram


  /******************************************************************/
  /* Partition 40                                                   */
  /******************************************************************/

  /* address in the flash of the partition 40 data to copy it to RAM */
  pharosPartition40DataAddr = partition40DataStart;

  /* data of partition */
  .partition40 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition40Start = .;

    /* address of the partition 40 data section start in RAM */
    partition40DataStart = .;

    *(.partition40)
    *(.partition40.*)

    /* address of the partition 40 data section end in RAM */
    . = ALIGN(8);
    partition40DataEnd = .;

  } > sdram

  /* bss of partition 40*/
  .partition40Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 40 bss section start in RAM */
    . = ALIGN(8);
    partition40BssStart = .;

    *(.partition40Bss)
    *(.partition40Bss.*)

    /* address of the shared partition 40 section end in RAM */
    . = ALIGN(8);
    partition40BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition40UssStart = .;

    *(.partition40Uss)
    *(.partition40Uss.*)

    partition40UssEnd = .;

    partition40End = .;

  } > sdram


  /******************************************************************/
  /* Partition 41                                                   */
  /******************************************************************/

  /* address in the flash of the partition 41 data to copy it to RAM */
  pharosPartition41DataAddr = partition41DataStart;

  /* data of partition */
  .partition41 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition41Start = .;

    /* address of the partition 41 data section start in RAM */
    partition41DataStart = .;

    *(.partition41)
    *(.partition41.*)

    /* address of the partition 41 data section end in RAM */
    . = ALIGN(8);
    partition41DataEnd = .;

  } > sdram

  /* bss of partition 41*/
  .partition41Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 41 bss section start in RAM */
    . = ALIGN(8);
    partition41BssStart = .;

    *(.partition41Bss)
    *(.partition41Bss.*)

    /* address of the shared partition 41 section end in RAM */
    . = ALIGN(8);
    partition41BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition41UssStart = .;

    *(.partition41Uss)
    *(.partition41Uss.*)

    partition41UssEnd = .;

    partition41End = .;

  } > sdram


  /******************************************************************/
  /* Partition 42                                                   */
  /******************************************************************/

  /* address in the flash of the partition 42 data to copy it to RAM */
  pharosPartition42DataAddr = partition42DataStart;

  /* data of partition */
  .partition42 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition42Start = .;

    /* address of the partition 42 data section start in RAM */
    partition42DataStart = .;

    *(.partition42)
    *(.partition42.*)

    /* address of the partition 42 data section end in RAM */
    . = ALIGN(8);
    partition42DataEnd = .;

  } > sdram

  /* bss of partition 42*/
  .partition42Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 42 bss section start in RAM */
    . = ALIGN(8);
    partition42BssStart = .;

    *(.partition42Bss)
    *(.partition42Bss.*)

    /* address of the shared partition 42 section end in RAM */
    . = ALIGN(8);
    partition42BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition42UssStart = .;

    *(.partition42Uss)
    *(.partition42Uss.*)

    partition42UssEnd = .;

    partition42End = .;

  } > sdram


  /******************************************************************/
  /* Partition 43                                                   */
  /******************************************************************/

  /* address in the flash of the partition 43 data to copy it to RAM */
  pharosPartition43DataAddr = partition43DataStart;

  /* data of partition */
  .partition43 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition43Start = .;

    /* address of the partition 43 data section start in RAM */
    partition43DataStart = .;

    *(.partition43)
    *(.partition43.*)

    /* address of the partition 43 data section end in RAM */
    . = ALIGN(8);
    partition43DataEnd = .;

  } > sdram

  /* bss of partition 43*/
  .partition43Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 43 bss section start in RAM */
    . = ALIGN(8);
    partition43BssStart = .;

    *(.partition43Bss)
    *(.partition43Bss.*)

    /* address of the shared partition 43 section end in RAM */
    . = ALIGN(8);
    partition43BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition43UssStart = .;

    *(.partition43Uss)
    *(.partition43Uss.*)

    partition43UssEnd = .;

    partition43End = .;

  } > sdram


  /******************************************************************/
  /* Partition 44                                                   */
  /******************************************************************/

  /* address in the flash of the partition 44 data to copy it to RAM */
  pharosPartition44DataAddr = partition44DataStart;

  /* data of partition */
  .partition44 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition44Start = .;

    /* address of the partition 44 data section start in RAM */
    partition44DataStart = .;

    *(.partition44)
    *(.partition44.*)

    /* address of the partition 44 data section end in RAM */
    . = ALIGN(8);
    partition44DataEnd = .;

  } > sdram

  /* bss of partition 44*/
  .partition44Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 44 bss section start in RAM */
    . = ALIGN(8);
    partition44BssStart = .;

    *(.partition44Bss)
    *(.partition44Bss.*)

    /* address of the shared partition 44 section end in RAM */
    . = ALIGN(8);
    partition44BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition44UssStart = .;

    *(.partition44Uss)
    *(.partition44Uss.*)

    partition44UssEnd = .;

    partition44End = .;

  } > sdram


  /******************************************************************/
  /* Partition 45                                                   */
  /******************************************************************/

  /* address in the flash of the partition 45 data to copy it to RAM */
  pharosPartition45DataAddr = partition45DataStart;

  /* data of partition */
  .partition45 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition45Start = .;

    /* address of the partition 45 data section start in RAM */
    partition45DataStart = .;

    *(.partition45)
    *(.partition45.*)

    /* address of the partition 45 data section end in RAM */
    . = ALIGN(8);
    partition45DataEnd = .;

  } > sdram

  /* bss of partition 45*/
  .partition45Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 45 bss section start in RAM */
    . = ALIGN(8);
    partition45BssStart = .;

    *(.partition45Bss)
    *(.partition45Bss.*)

    /* address of the shared partition 45 section end in RAM */
    . = ALIGN(8);
    partition45BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition45UssStart = .;

    *(.partition45Uss)
    *(.partition45Uss.*)

    partition45UssEnd = .;

    partition45End = .;

  } > sdram


  /******************************************************************/
  /* Partition 46                                                   */
  /******************************************************************/

  /* address in the flash of the partition 46 data to copy it to RAM */
  pharosPartition46DataAddr = partition46DataStart;

  /* data of partition */
  .partition46 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition46Start = .;

    /* address of the partition 46 data section start in RAM */
    partition46DataStart = .;

    *(.partition46)
    *(.partition46.*)

    /* address of the partition 46 data section end in RAM */
    . = ALIGN(8);
    partition46DataEnd = .;

  } > sdram

  /* bss of partition 46*/
  .partition46Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 46 bss section start in RAM */
    . = ALIGN(8);
    partition46BssStart = .;

    *(.partition46Bss)
    *(.partition46Bss.*)

    /* address of the shared partition 46 section end in RAM */
    . = ALIGN(8);
    partition46BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition46UssStart = .;

    *(.partition46Uss)
    *(.partition46Uss.*)

    partition46UssEnd = .;

    partition46End = .;

  } > sdram


  /******************************************************************/
  /* Partition 47                                                   */
  /******************************************************************/

  /* address in the flash of the partition 47 data to copy it to RAM */
  pharosPartition47DataAddr = partition47DataStart;

  /* data of partition */
  .partition47 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition47Start = .;

    /* address of the partition 47 data section start in RAM */
    partition47DataStart = .;

    *(.partition47)
    *(.partition47.*)

    /* address of the partition 47 data section end in RAM */
    . = ALIGN(8);
    partition47DataEnd = .;

  } > sdram

  /* bss of partition 47*/
  .partition47Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 47 bss section start in RAM */
    . = ALIGN(8);
    partition47BssStart = .;

    *(.partition47Bss)
    *(.partition47Bss.*)

    /* address of the shared partition 47 section end in RAM */
    . = ALIGN(8);
    partition47BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition47UssStart = .;

    *(.partition47Uss)
    *(.partition47Uss.*)

    partition47UssEnd = .;

    partition47End = .;

  } > sdram


  /******************************************************************/
  /* Partition 48                                                   */
  /******************************************************************/

  /* address in the flash of the partition 48 data to copy it to RAM */
  pharosPartition48DataAddr = partition48DataStart;

  /* data of partition */
  .partition48 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition48Start = .;

    /* address of the partition 48 data section start in RAM */
    partition48DataStart = .;

    *(.partition48)
    *(.partition48.*)

    /* address of the partition 48 data section end in RAM */
    . = ALIGN(8);
    partition48DataEnd = .;

  } > sdram

  /* bss of partition 48*/
  .partition48Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 48 bss section start in RAM */
    . = ALIGN(8);
    partition48BssStart = .;

    *(.partition48Bss)
    *(.partition48Bss.*)

    /* address of the shared partition 48 section end in RAM */
    . = ALIGN(8);
    partition48BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition48UssStart = .;

    *(.partition48Uss)
    *(.partition48Uss.*)

    partition48UssEnd = .;

    partition48End = .;

  } > sdram


  /******************************************************************/
  /* Partition 49                                                   */
  /******************************************************************/

  /* address in the flash of the partition 49 data to copy it to RAM */
  pharosPartition49DataAddr = partition49DataStart;

  /* data of partition */
  .partition49 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition49Start = .;

    /* address of the partition 49 data section start in RAM */
    partition49DataStart = .;

    *(.partition49)
    *(.partition49.*)

    /* address of the partition 49 data section end in RAM */
    . = ALIGN(8);
    partition49DataEnd = .;

  } > sdram

  /* bss of partition 49*/
  .partition49Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 49 bss section start in RAM */
    . = ALIGN(8);
    partition49BssStart = .;

    *(.partition49Bss)
    *(.partition49Bss.*)

    /* address of the shared partition 49 section end in RAM */
    . = ALIGN(8);
    partition49BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition49UssStart = .;

    *(.partition49Uss)
    *(.partition49Uss.*)

    partition49UssEnd = .;

    partition49End = .;

  } > sdram


  /******************************************************************/
  /* Partition 50                                                   */
  /******************************************************************/

  /* address in the flash of the partition 50 data to copy it to RAM */
  pharosPartition50DataAddr = partition50DataStart;

  /* data of partition */
  .partition50 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition50Start = .;

    /* address of the partition 50 data section start in RAM */
    partition50DataStart = .;

    *(.partition50)
    *(.partition50.*)

    /* address of the partition 50 data section end in RAM */
    . = ALIGN(8);
    partition50DataEnd = .;

  } > sdram

  /* bss of partition 50*/
  .partition50Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 50 bss section start in RAM */
    . = ALIGN(8);
    partition50BssStart = .;

    *(.partition50Bss)
    *(.partition50Bss.*)

    /* address of the shared partition 50 section end in RAM */
    . = ALIGN(8);
    partition50BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition50UssStart = .;

    *(.partition50Uss)
    *(.partition50Uss.*)

    partition50UssEnd = .;

    partition50End = .;

  } > sdram


  /******************************************************************/
  /* Partition 51                                                   */
  /******************************************************************/

  /* address in the flash of the partition 51 data to copy it to RAM */
  pharosPartition51DataAddr = partition51DataStart;

  /* data of partition */
  .partition51 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition51Start = .;

    /* address of the partition 51 data section start in RAM */
    partition51DataStart = .;

    *(.partition51)
    *(.partition51.*)

    /* address of the partition 51 data section end in RAM */
    . = ALIGN(8);
    partition51DataEnd = .;

  } > sdram

  /* bss of partition 51*/
  .partition51Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 51 bss section start in RAM */
    . = ALIGN(8);
    partition51BssStart = .;

    *(.partition51Bss)
    *(.partition51Bss.*)

    /* address of the shared partition 51 section end in RAM */
    . = ALIGN(8);
    partition51BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition51UssStart = .;

    *(.partition51Uss)
    *(.partition51Uss.*)

    partition51UssEnd = .;

    partition51End = .;

  } > sdram


  /******************************************************************/
  /* Partition 52                                                   */
  /******************************************************************/

  /* address in the flash of the partition 52 data to copy it to RAM */
  pharosPartition52DataAddr = partition52DataStart;

  /* data of partition */
  .partition52 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition52Start = .;

    /* address of the partition 52 data section start in RAM */
    partition52DataStart = .;

    *(.partition52)
    *(.partition52.*)

    /* address of the partition 52 data section end in RAM */
    . = ALIGN(8);
    partition52DataEnd = .;

  } > sdram

  /* bss of partition 52*/
  .partition52Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 52 bss section start in RAM */
    . = ALIGN(8);
    partition52BssStart = .;

    *(.partition52Bss)
    *(.partition52Bss.*)

    /* address of the shared partition 52 section end in RAM */
    . = ALIGN(8);
    partition52BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition52UssStart = .;

    *(.partition52Uss)
    *(.partition52Uss.*)

    partition52UssEnd = .;

    partition52End = .;

  } > sdram


  /******************************************************************/
  /* Partition 53                                                   */
  /******************************************************************/

  /* address in the flash of the partition 53 data to copy it to RAM */
  pharosPartition53DataAddr = partition53DataStart;

  /* data of partition */
  .partition53 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition53Start = .;

    /* address of the partition 53 data section start in RAM */
    partition53DataStart = .;

    *(.partition53)
    *(.partition53.*)

    /* address of the partition 53 data section end in RAM */
    . = ALIGN(8);
    partition53DataEnd = .;

  } > sdram

  /* bss of partition 53*/
  .partition53Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 53 bss section start in RAM */
    . = ALIGN(8);
    partition53BssStart = .;

    *(.partition53Bss)
    *(.partition53Bss.*)

    /* address of the shared partition 53 section end in RAM */
    . = ALIGN(8);
    partition53BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition53UssStart = .;

    *(.partition53Uss)
    *(.partition53Uss.*)

    partition53UssEnd = .;

    partition53End = .;

  } > sdram


  /******************************************************************/
  /* Partition 54                                                   */
  /******************************************************************/

  /* address in the flash of the partition 54 data to copy it to RAM */
  pharosPartition54DataAddr = partition54DataStart;

  /* data of partition */
  .partition54 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition54Start = .;

    /* address of the partition 54 data section start in RAM */
    partition54DataStart = .;

    *(.partition54)
    *(.partition54.*)

    /* address of the partition 54 data section end in RAM */
    . = ALIGN(8);
    partition54DataEnd = .;

  } > sdram

  /* bss of partition 54*/
  .partition54Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 54 bss section start in RAM */
    . = ALIGN(8);
    partition54BssStart = .;

    *(.partition54Bss)
    *(.partition54Bss.*)

    /* address of the shared partition 54 section end in RAM */
    . = ALIGN(8);
    partition54BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition54UssStart = .;

    *(.partition54Uss)
    *(.partition54Uss.*)

    partition54UssEnd = .;

    partition54End = .;

  } > sdram


  /******************************************************************/
  /* Partition 55                                                   */
  /******************************************************************/

  /* address in the flash of the partition 55 data to copy it to RAM */
  pharosPartition55DataAddr = partition55DataStart;

  /* data of partition */
  .partition55 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition55Start = .;

    /* address of the partition 55 data section start in RAM */
    partition55DataStart = .;

    *(.partition55)
    *(.partition55.*)

    /* address of the partition 55 data section end in RAM */
    . = ALIGN(8);
    partition55DataEnd = .;

  } > sdram

  /* bss of partition 55*/
  .partition55Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 55 bss section start in RAM */
    . = ALIGN(8);
    partition55BssStart = .;

    *(.partition55Bss)
    *(.partition55Bss.*)

    /* address of the shared partition 55 section end in RAM */
    . = ALIGN(8);
    partition55BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition55UssStart = .;

    *(.partition55Uss)
    *(.partition55Uss.*)

    partition55UssEnd = .;

    partition55End = .;

  } > sdram


  /******************************************************************/
  /* Partition 56                                                   */
  /******************************************************************/

  /* address in the flash of the partition 56 data to copy it to RAM */
  pharosPartition56DataAddr = partition56DataStart;

  /* data of partition */
  .partition56 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition56Start = .;

    /* address of the partition 56 data section start in RAM */
    partition56DataStart = .;

    *(.partition56)
    *(.partition56.*)

    /* address of the partition 56 data section end in RAM */
    . = ALIGN(8);
    partition56DataEnd = .;

  } > sdram

  /* bss of partition 56*/
  .partition56Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 56 bss section start in RAM */
    . = ALIGN(8);
    partition56BssStart = .;

    *(.partition56Bss)
    *(.partition56Bss.*)

    /* address of the shared partition 56 section end in RAM */
    . = ALIGN(8);
    partition56BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition56UssStart = .;

    *(.partition56Uss)
    *(.partition56Uss.*)

    partition56UssEnd = .;

    partition56End = .;

  } > sdram


  /******************************************************************/
  /* Partition 57                                                   */
  /******************************************************************/

  /* address in the flash of the partition 57 data to copy it to RAM */
  pharosPartition57DataAddr = partition57DataStart;

  /* data of partition */
  .partition57 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition57Start = .;

    /* address of the partition 57 data section start in RAM */
    partition57DataStart = .;

    *(.partition57)
    *(.partition57.*)

    /* address of the partition 57 data section end in RAM */
    . = ALIGN(8);
    partition57DataEnd = .;

  } > sdram

  /* bss of partition 57*/
  .partition57Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 57 bss section start in RAM */
    . = ALIGN(8);
    partition57BssStart = .;

    *(.partition57Bss)
    *(.partition57Bss.*)

    /* address of the shared partition 57 section end in RAM */
    . = ALIGN(8);
    partition57BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition57UssStart = .;

    *(.partition57Uss)
    *(.partition57Uss.*)

    partition57UssEnd = .;

    partition57End = .;

  } > sdram


  /******************************************************************/
  /* Partition 58                                                   */
  /******************************************************************/

  /* address in the flash of the partition 58 data to copy it to RAM */
  pharosPartition58DataAddr = partition58DataStart;

  /* data of partition */
  .partition58 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition58Start = .;

    /* address of the partition 58 data section start in RAM */
    partition58DataStart = .;

    *(.partition58)
    *(.partition58.*)

    /* address of the partition 58 data section end in RAM */
    . = ALIGN(8);
    partition58DataEnd = .;

  } > sdram

  /* bss of partition 58*/
  .partition58Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 58 bss section start in RAM */
    . = ALIGN(8);
    partition58BssStart = .;

    *(.partition58Bss)
    *(.partition58Bss.*)

    /* address of the shared partition 58 section end in RAM */
    . = ALIGN(8);
    partition58BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition58UssStart = .;

    *(.partition58Uss)
    *(.partition58Uss.*)

    partition58UssEnd = .;

    partition58End = .;

  } > sdram


  /******************************************************************/
  /* Partition 59                                                   */
  /******************************************************************/

  /* address in the flash of the partition 59 data to copy it to RAM */
  pharosPartition59DataAddr = partition59DataStart;

  /* data of partition */
  .partition59 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition59Start = .;

    /* address of the partition 59 data section start in RAM */
    partition59DataStart = .;

    *(.partition59)
    *(.partition59.*)

    /* address of the partition 59 data section end in RAM */
    . = ALIGN(8);
    partition59DataEnd = .;

  } > sdram

  /* bss of partition 59*/
  .partition59Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 59 bss section start in RAM */
    . = ALIGN(8);
    partition59BssStart = .;

    *(.partition59Bss)
    *(.partition59Bss.*)

    /* address of the shared partition 59 section end in RAM */
    . = ALIGN(8);
    partition59BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition59UssStart = .;

    *(.partition59Uss)
    *(.partition59Uss.*)

    partition59UssEnd = .;

    partition59End = .;

  } > sdram


  /******************************************************************/
  /* Partition 60                                                   */
  /******************************************************************/

  /* address in the flash of the partition 60 data to copy it to RAM */
  pharosPartition60DataAddr = partition60DataStart;

  /* data of partition */
  .partition60 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition60Start = .;

    /* address of the partition 60 data section start in RAM */
    partition60DataStart = .;

    *(.partition60)
    *(.partition60.*)

    /* address of the partition 60 data section end in RAM */
    . = ALIGN(8);
    partition60DataEnd = .;

  } > sdram

  /* bss of partition 60*/
  .partition60Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 60 bss section start in RAM */
    . = ALIGN(8);
    partition60BssStart = .;

    *(.partition60Bss)
    *(.partition60Bss.*)

    /* address of the shared partition 60 section end in RAM */
    . = ALIGN(8);
    partition60BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition60UssStart = .;

    *(.partition60Uss)
    *(.partition60Uss.*)

    partition60UssEnd = .;

    partition60End = .;

  } > sdram


  /******************************************************************/
  /* Partition 61                                                   */
  /******************************************************************/

  /* address in the flash of the partition 61 data to copy it to RAM */
  pharosPartition61DataAddr = partition61DataStart;

  /* data of partition */
  .partition61 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition61Start = .;

    /* address of the partition 61 data section start in RAM */
    partition61DataStart = .;

    *(.partition61)
    *(.partition61.*)

    /* address of the partition 61 data section end in RAM */
    . = ALIGN(8);
    partition61DataEnd = .;

  } > sdram

  /* bss of partition 61*/
  .partition61Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 61 bss section start in RAM */
    . = ALIGN(8);
    partition61BssStart = .;

    *(.partition61Bss)
    *(.partition61Bss.*)

    /* address of the shared partition 61 section end in RAM */
    . = ALIGN(8);
    partition61BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition61UssStart = .;

    *(.partition61Uss)
    *(.partition61Uss.*)

    partition61UssEnd = .;

    partition61End = .;

  } > sdram


  /******************************************************************/
  /* Partition 62                                                   */
  /******************************************************************/

  /* address in the flash of the partition 62 data to copy it to RAM */
  pharosPartition62DataAddr = partition62DataStart;

  /* data of partition */
  .partition62 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition62Start = .;

    /* address of the partition 62 data section start in RAM */
    partition62DataStart = .;

    *(.partition62)
    *(.partition62.*)

    /* address of the partition 62 data section end in RAM */
    . = ALIGN(8);
    partition62DataEnd = .;

  } > sdram

  /* bss of partition 62*/
  .partition62Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 62 bss section start in RAM */
    . = ALIGN(8);
    partition62BssStart = .;

    *(.partition62Bss)
    *(.partition62Bss.*)

    /* address of the shared partition 62 section end in RAM */
    . = ALIGN(8);
    partition62BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition62UssStart = .;

    *(.partition62Uss)
    *(.partition62Uss.*)

    partition62UssEnd = .;

    partition62End = .;

  } > sdram


  /******************************************************************/
  /* Partition 63                                                   */
  /******************************************************************/

  /* address in the flash of the partition 63 data to copy it to RAM */
  pharosPartition63DataAddr = partition63DataStart;

  /* data of partition */
  .partition63 :
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition63Start = .;

    /* address of the partition 63 data section start in RAM */
    partition63DataStart = .;

    *(.partition63)
    *(.partition63.*)

    /* address of the partition 63 data section end in RAM */
    . = ALIGN(8);
    partition63DataEnd = .;

  } > sdram

  /* bss of partition 63*/
  .partition63Bss (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 63 bss section start in RAM */
    . = ALIGN(8);
    partition63BssStart = .;

    *(.partition63Bss)
    *(.partition63Bss.*)

    /* address of the shared partition 63 section end in RAM */
    . = ALIGN(8);
    partition63BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition63UssStart = .;

    *(.partition63Uss)
    *(.partition63Uss.*)

    partition63UssEnd = .;

    partition63End = .;

  } > sdram

  /******************************************************************/
  /*                     End of partitions                          */
  /******************************************************************/

  /******************************************************************/
  /*             Start of Inter-partition calls area                */
  /******************************************************************/

  pharosIpCallDataLoadAddr = LOADADDR(.ipCall);
  pharosIpCallDataSize = (pharosIpCallEnd - pharosIpCallSectionStart) / 8;

  /* this area will be read-only for the user application but must be written in supervisor mode at iniqtialization */
  .ipCall : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    pharosIpCallSectionStart = ABSOLUTE(.);
    
    . = ALIGN(IP_CALL_ALIGNMENT);

    /* address of the shared section start in RAM */
    pharosIpCallStart = ABSOLUTE(.);
    *(.ipCall)  
    *(.ipCall.*)  

    . = ALIGN(8);

    /* address of the shared section end in RAM */
    pharosIpCallEnd = ABSOLUTE(.);

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(IP_CALL_ALIGNMENT);

  } > sdram


}
