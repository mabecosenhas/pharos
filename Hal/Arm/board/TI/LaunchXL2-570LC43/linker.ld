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


OUTPUT_ARCH(arm)

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

ENTRY(pharosStart);


/* Specify the memory areas */
MEMORY
{
  flash   (rx)    : ORIGIN = 0x00000000, LENGTH = 4M
  ram     (rwx)   : ORIGIN = 0x08000000, LENGTH = 512K
}

FLASH_START = 0x00000000;
FLASH_END   = FLASH_START + 4M;

CHANNEL_BULK_SIZE = 64;

/**
 * On this CPU the minimum MPU alignment is 32 bytes, but this requires using sub-regions. 
 * It is much simpler not to use sub-regions and allow this wasted space 
 * (max wasted space is 255-32 = 223 bytes per partition)
 */
REGION_MINIMUM_ALIGNMENT = 256;
REGION_MINIMUM_ALIGNMENT_LOG2 = 8;

/* calculate the log2 of the text section size, used to initialize the MPU size register */
pharosCpuTextSizeLog2   = MAX(REGION_MINIMUM_ALIGNMENT_LOG2 , LOG2CEIL(FLASH_END - FLASH_START));
pharosCpuSharedSizeLog2 = MAX(REGION_MINIMUM_ALIGNMENT_LOG2 , LOG2CEIL(pharosSharedEnd - pharosSharedStart));
pharosCpuIpCallSizeLog2 = MAX(REGION_MINIMUM_ALIGNMENT_LOG2 , LOG2CEIL(pharosIpCallEnd - pharosIpCallStart));
    
PARTITION_0_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition0End - partition0Start));
PARTITION_1_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition1End - partition1Start));
PARTITION_2_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition2End - partition2Start));
PARTITION_3_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition3End - partition3Start));
PARTITION_4_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition4End - partition4Start));
PARTITION_5_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition5End - partition5Start));
PARTITION_6_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition6End - partition6Start));
PARTITION_7_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition7End - partition7Start));
PARTITION_8_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition8End - partition8Start));
PARTITION_9_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition9End - partition9Start));
PARTITION_10_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition10End - partition10Start));
PARTITION_11_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition11End - partition11Start));
PARTITION_12_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition12End - partition12Start));
PARTITION_13_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition13End - partition13Start));
PARTITION_14_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition14End - partition14Start));
PARTITION_15_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition15End - partition15Start));
PARTITION_16_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition16End - partition16Start));
PARTITION_17_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition17End - partition17Start));
PARTITION_18_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition18End - partition18Start));
PARTITION_19_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition19End - partition19Start));
PARTITION_20_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition20End - partition20Start));
PARTITION_21_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition21End - partition21Start));
PARTITION_22_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition22End - partition22Start));
PARTITION_23_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition23End - partition23Start));
PARTITION_24_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition24End - partition24Start));
PARTITION_25_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition25End - partition25Start));
PARTITION_26_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition26End - partition26Start));
PARTITION_27_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition27End - partition27Start));
PARTITION_28_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition28End - partition28Start));
PARTITION_29_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition29End - partition29Start));
PARTITION_30_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition30End - partition30Start));
PARTITION_31_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition31End - partition31Start));
PARTITION_32_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition32End - partition32Start));
PARTITION_33_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition33End - partition33Start));
PARTITION_34_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition34End - partition34Start));
PARTITION_35_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition35End - partition35Start));
PARTITION_36_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition36End - partition36Start));
PARTITION_37_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition37End - partition37Start));
PARTITION_38_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition38End - partition38Start));
PARTITION_39_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition39End - partition39Start));
PARTITION_40_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition40End - partition40Start));
PARTITION_41_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition41End - partition41Start));
PARTITION_42_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition42End - partition42Start));
PARTITION_43_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition43End - partition43Start));
PARTITION_44_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition44End - partition44Start));
PARTITION_45_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition45End - partition45Start));
PARTITION_46_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition46End - partition46Start));
PARTITION_47_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition47End - partition47Start));
PARTITION_48_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition48End - partition48Start));
PARTITION_49_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition49End - partition49Start));
PARTITION_50_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition50End - partition50Start));
PARTITION_51_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition51End - partition51Start));
PARTITION_52_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition52End - partition52Start));
PARTITION_53_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition53End - partition53Start));
PARTITION_54_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition54End - partition54Start));
PARTITION_55_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition55End - partition55Start));
PARTITION_56_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition56End - partition56Start));
PARTITION_57_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition57End - partition57Start));
PARTITION_58_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition58End - partition58Start));
PARTITION_59_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition59End - partition59Start));
PARTITION_60_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition60End - partition60Start));
PARTITION_61_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition61End - partition61Start));
PARTITION_62_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition62End - partition62Start));
PARTITION_63_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(partition63End - partition63Start));


/* alignment for kernel data */
KERNEL_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosKernelEnd - pharosKernelStart));

/* alignment for shared stack data */
SHARED_STACK_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosSharedStackEnd - pharosSharedStackStart));

/* alignment for shared data */
SHARED_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosSharedEnd - pharosSharedStart));

/* alignment for inter-partition calls data */
IP_CALL_ALIGNMENT = MAX(REGION_MINIMUM_ALIGNMENT, 1 << LOG2CEIL(pharosIpCallEnd - pharosIpCallStart));

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
    pharosRodataEnd = .;

    /* variable to check the end of the text section */
    . = ALIGN (0x8);
    pharosEndText = FLASH_END;
  } > flash = 0xff

  /******************************************************************/
  /*                 End of text area and read-only data            */
  /******************************************************************/


  /******************************************************************/
  /*                     Start of kernel area                       */
  /******************************************************************/

  /* address in the flash of the kernel data to copy it to RAM */
  pharosKernelDataLoadAddr = LOADADDR(.kernel);
  pharosKernelDataSize = (pharosKernelDataEnd - pharosKernelDataStart) / 8;
  pharosKernelBssSize = (pharosKernelBssEnd - pharosKernelBssStart) / 8;

  /* Kernel data space to enable memory protection */
  .kernel : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS
    
    /* address in RAM where the kernel data starts */
    pharosKernelDataStart = .;

    . = ALIGN(KERNEL_ALIGNMENT);
    . = ALIGN(8);

    pharosKernelStart = .;
    
    *(.kernel)  
    *(.kernel.*)

    . = ALIGN(8);

    /* address in RAM where the kernel data ends */
    pharosKernelDataEnd = .;
  
  } > ram AT > flash = 0xff
  
  .kernelBss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(8); 

    /* address in RAM where the kernel BSS starts */
    pharosKernelBssStart = .;

    *(.kernelBss)
    *(.kernelBss.*)

    . = ALIGN(8); 
    
    /* address in RAM where the kernel BSS ends */
    pharosKernelBssEnd = .;

    /* place here the data that is NOT to be initialized at all */
    pharosKernelUssStart = .;

    *(.kernelUss)
    *(.kernelUss.*)

    pharosKernelUssEnd = .;

  } > ram

  .kernelBulks (NOLOAD) : 
  {
    CREATE_OBJECT_SYMBOLS
    
    /* need to align the bulks to their size at least */
    . = ALIGN(CHANNEL_BULK_SIZE);

    pharosBulkStart = .;

    *(.kernelBulks)  
    *(.kernelBulks.*)

    pharosBulkEnd = .;

    pharosKernelEnd = .;

    /*
     * note that there may be overloap (we are not aligning here to KERNEL_ALIGNMENT) 
     * from this kernel section to the next section, but that is not an issue 
     * because the kernel already has access to all sections 
     */

  } > ram

  /******************************************************************/
  /*                  Start of shared stack area                    */
  /******************************************************************/

  .pharosSharedStack (NOLOAD) : 
  {
    CREATE_OBJECT_SYMBOLS

    pharosSharedStackBssStart = .;

    /* make sure the next region does not start before the MPU region of this one ends */
    . = ALIGN(SHARED_STACK_ALIGNMENT);

    /* address of the shared stack section start in RAM */
    pharosSharedStackStart = .;

    /* each shared stack must be aligned to its size (e.g. stack of 1KiB must be aligned to 1KiB) */
    *(.pharosSharedStack)
    *(.pharosSharedStack.*)

    . = ALIGN(SHARED_STACK_ALIGNMENT);

    pharosSharedStackEnd = .;
   
  } > ram

  /******************************************************************/
  /*                        End of kernel space                     */
  /******************************************************************/

  /******************************************************************/
  /*                     Start of Shared data area                  */
  /******************************************************************/

  /* address in the flash of the shared data to copy it to RAM */
  pharosSharedDataLoadAddr = LOADADDR(.shared);
  pharosSharedDataSize = (pharosSharedDataEnd - pharosSharedDataStart) / 8;
  pharosSharedBssSize = (pharosSharedBssEnd - pharosSharedBssStart) / 8;

  /* Shared section data space */
  .shared : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the shared data section start in RAM */
    pharosSharedDataStart = ABSOLUTE(.);
    
    . = ALIGN(SHARED_ALIGNMENT);

    /* address of the shared section start in RAM */
    pharosSharedStart = ABSOLUTE(.);
    
    . = ALIGN(8);

    *(.shared)
    *(.shared.*)

    . = ALIGN(8);

    /* address of the shared data section end in RAM */
    pharosSharedDataEnd = ABSOLUTE(.);
  } > ram AT > flash = 0xff

  .sharedBss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    . = ALIGN(8);

    /* address of the shared bss section start in RAM */
    pharosSharedBssStart = ABSOLUTE(.);

    *(.sharedBss)
    *(.sharedBss.*)

    . = ALIGN(8);

    /* address of the shared bss section end in RAM */
    pharosSharedBssEnd = ABSOLUTE(.);

    /* place here the shared data that is NOT to be initialized at all */
    pharosSharedUssStart = .;

    *(.sharedUss)
    *(.sharedUss.*)

    pharosSharedUssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(SHARED_ALIGNMENT);

    /* address of the shared section end in RAM */
    pharosSharedEnd = ABSOLUTE(.);
  } > ram 

  /******************************************************************/
  /*                        End of shared data                      */
  /******************************************************************/

  /******************************************************************/
  /*                     Start of interrupt stack area              */
  /******************************************************************/

  /* the interrupt stack is after the shared section so that overflows will damage this area and not the kernel */

  .interruptStackBss (NOLOAD) : 
  {    
    CREATE_OBJECT_SYMBOLS
    . = ALIGN (REGION_MINIMUM_ALIGNMENT);

    interruptStackBssStart = .;
    *(.interruptStackBss)

    interruptStackBssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(REGION_MINIMUM_ALIGNMENT);

  } > ram


  /******************************************************************/
  /* Separate the partitions data space to enable memory protection */
  /******************************************************************/

  /* determine the size of the data in 64 bit variables */
  partition0DataSize = (partition0DataEnd - partition0DataStart) / 8;
  partition1DataSize = (partition1DataEnd - partition1DataStart) / 8;
  partition2DataSize = (partition2DataEnd - partition2DataStart) / 8;
  partition3DataSize = (partition3DataEnd - partition3DataStart) / 8;
  partition4DataSize = (partition4DataEnd - partition4DataStart) / 8;
  partition5DataSize = (partition5DataEnd - partition5DataStart) / 8;
  partition6DataSize = (partition6DataEnd - partition6DataStart) / 8;
  partition7DataSize = (partition7DataEnd - partition7DataStart) / 8;
  partition8DataSize = (partition8DataEnd - partition8DataStart) / 8;
  partition9DataSize = (partition9DataEnd - partition9DataStart) / 8;
  partition10DataSize = (partition10DataEnd - partition10DataStart) / 8;
  partition11DataSize = (partition11DataEnd - partition11DataStart) / 8;
  partition12DataSize = (partition12DataEnd - partition12DataStart) / 8;
  partition13DataSize = (partition13DataEnd - partition13DataStart) / 8;
  partition14DataSize = (partition14DataEnd - partition14DataStart) / 8;
  partition15DataSize = (partition15DataEnd - partition15DataStart) / 8;
  partition16DataSize = (partition16DataEnd - partition16DataStart) / 8;
  partition17DataSize = (partition17DataEnd - partition17DataStart) / 8;
  partition18DataSize = (partition18DataEnd - partition18DataStart) / 8;
  partition19DataSize = (partition19DataEnd - partition19DataStart) / 8;
  partition20DataSize = (partition20DataEnd - partition20DataStart) / 8;
  partition21DataSize = (partition21DataEnd - partition21DataStart) / 8;
  partition22DataSize = (partition22DataEnd - partition22DataStart) / 8;
  partition23DataSize = (partition23DataEnd - partition23DataStart) / 8;
  partition24DataSize = (partition24DataEnd - partition24DataStart) / 8;
  partition25DataSize = (partition25DataEnd - partition25DataStart) / 8;
  partition26DataSize = (partition26DataEnd - partition26DataStart) / 8;
  partition27DataSize = (partition27DataEnd - partition27DataStart) / 8;
  partition28DataSize = (partition28DataEnd - partition28DataStart) / 8;
  partition29DataSize = (partition29DataEnd - partition29DataStart) / 8;
  partition30DataSize = (partition30DataEnd - partition30DataStart) / 8;
  partition31DataSize = (partition31DataEnd - partition31DataStart) / 8;
  partition32DataSize = (partition32DataEnd - partition32DataStart) / 8;
  partition33DataSize = (partition33DataEnd - partition33DataStart) / 8;
  partition34DataSize = (partition34DataEnd - partition34DataStart) / 8;
  partition35DataSize = (partition35DataEnd - partition35DataStart) / 8;
  partition36DataSize = (partition36DataEnd - partition36DataStart) / 8;
  partition37DataSize = (partition37DataEnd - partition37DataStart) / 8;
  partition38DataSize = (partition38DataEnd - partition38DataStart) / 8;
  partition39DataSize = (partition39DataEnd - partition39DataStart) / 8;
  partition40DataSize = (partition40DataEnd - partition40DataStart) / 8;
  partition41DataSize = (partition41DataEnd - partition41DataStart) / 8;
  partition42DataSize = (partition42DataEnd - partition42DataStart) / 8;
  partition43DataSize = (partition43DataEnd - partition43DataStart) / 8;
  partition44DataSize = (partition44DataEnd - partition44DataStart) / 8;
  partition45DataSize = (partition45DataEnd - partition45DataStart) / 8;
  partition46DataSize = (partition46DataEnd - partition46DataStart) / 8;
  partition47DataSize = (partition47DataEnd - partition47DataStart) / 8;
  partition48DataSize = (partition48DataEnd - partition48DataStart) / 8;
  partition49DataSize = (partition49DataEnd - partition49DataStart) / 8;
  partition50DataSize = (partition50DataEnd - partition50DataStart) / 8;
  partition51DataSize = (partition51DataEnd - partition51DataStart) / 8;
  partition52DataSize = (partition52DataEnd - partition52DataStart) / 8;
  partition53DataSize = (partition53DataEnd - partition53DataStart) / 8;
  partition54DataSize = (partition54DataEnd - partition54DataStart) / 8;
  partition55DataSize = (partition55DataEnd - partition55DataStart) / 8;
  partition56DataSize = (partition56DataEnd - partition56DataStart) / 8;
  partition57DataSize = (partition57DataEnd - partition57DataStart) / 8;
  partition58DataSize = (partition58DataEnd - partition58DataStart) / 8;
  partition59DataSize = (partition59DataEnd - partition59DataStart) / 8;
  partition60DataSize = (partition60DataEnd - partition60DataStart) / 8;
  partition61DataSize = (partition61DataEnd - partition61DataStart) / 8;
  partition62DataSize = (partition62DataEnd - partition62DataStart) / 8;
  partition63DataSize = (partition63DataEnd - partition63DataStart) / 8;

  /* determine the size of the bss in 64 bit variables */
  partition0BssSize = (partition0BssEnd - partition0BssStart) / 8; 
  partition1BssSize = (partition1BssEnd - partition1BssStart) / 8; 
  partition2BssSize = (partition2BssEnd - partition2BssStart) / 8; 
  partition3BssSize = (partition3BssEnd - partition3BssStart) / 8; 
  partition4BssSize = (partition4BssEnd - partition4BssStart) / 8; 
  partition5BssSize = (partition5BssEnd - partition5BssStart) / 8; 
  partition6BssSize = (partition6BssEnd - partition6BssStart) / 8; 
  partition7BssSize = (partition7BssEnd - partition7BssStart) / 8; 
  partition8BssSize = (partition8BssEnd - partition8BssStart) / 8; 
  partition9BssSize = (partition9BssEnd - partition9BssStart) / 8; 
  partition10BssSize = (partition10BssEnd - partition10BssStart) / 8; 
  partition11BssSize = (partition11BssEnd - partition11BssStart) / 8; 
  partition12BssSize = (partition12BssEnd - partition12BssStart) / 8; 
  partition13BssSize = (partition13BssEnd - partition13BssStart) / 8; 
  partition14BssSize = (partition14BssEnd - partition14BssStart) / 8; 
  partition15BssSize = (partition15BssEnd - partition15BssStart) / 8; 
  partition16BssSize = (partition16BssEnd - partition16BssStart) / 8; 
  partition17BssSize = (partition17BssEnd - partition17BssStart) / 8; 
  partition18BssSize = (partition18BssEnd - partition18BssStart) / 8; 
  partition19BssSize = (partition19BssEnd - partition19BssStart) / 8; 
  partition20BssSize = (partition20BssEnd - partition20BssStart) / 8; 
  partition21BssSize = (partition21BssEnd - partition21BssStart) / 8; 
  partition22BssSize = (partition22BssEnd - partition22BssStart) / 8; 
  partition23BssSize = (partition23BssEnd - partition23BssStart) / 8; 
  partition24BssSize = (partition24BssEnd - partition24BssStart) / 8; 
  partition25BssSize = (partition25BssEnd - partition25BssStart) / 8; 
  partition26BssSize = (partition26BssEnd - partition26BssStart) / 8; 
  partition27BssSize = (partition27BssEnd - partition27BssStart) / 8; 
  partition28BssSize = (partition28BssEnd - partition28BssStart) / 8; 
  partition29BssSize = (partition29BssEnd - partition29BssStart) / 8; 
  partition30BssSize = (partition30BssEnd - partition30BssStart) / 8; 
  partition31BssSize = (partition31BssEnd - partition31BssStart) / 8; 
  partition32BssSize = (partition32BssEnd - partition32BssStart) / 8; 
  partition33BssSize = (partition33BssEnd - partition33BssStart) / 8; 
  partition34BssSize = (partition34BssEnd - partition34BssStart) / 8; 
  partition35BssSize = (partition35BssEnd - partition35BssStart) / 8; 
  partition36BssSize = (partition36BssEnd - partition36BssStart) / 8; 
  partition37BssSize = (partition37BssEnd - partition37BssStart) / 8; 
  partition38BssSize = (partition38BssEnd - partition38BssStart) / 8; 
  partition39BssSize = (partition39BssEnd - partition39BssStart) / 8; 
  partition40BssSize = (partition40BssEnd - partition40BssStart) / 8; 
  partition41BssSize = (partition41BssEnd - partition41BssStart) / 8; 
  partition42BssSize = (partition42BssEnd - partition42BssStart) / 8; 
  partition43BssSize = (partition43BssEnd - partition43BssStart) / 8; 
  partition44BssSize = (partition44BssEnd - partition44BssStart) / 8; 
  partition45BssSize = (partition45BssEnd - partition45BssStart) / 8; 
  partition46BssSize = (partition46BssEnd - partition46BssStart) / 8; 
  partition47BssSize = (partition47BssEnd - partition47BssStart) / 8; 
  partition48BssSize = (partition48BssEnd - partition48BssStart) / 8; 
  partition49BssSize = (partition49BssEnd - partition49BssStart) / 8; 
  partition50BssSize = (partition50BssEnd - partition50BssStart) / 8; 
  partition51BssSize = (partition51BssEnd - partition51BssStart) / 8; 
  partition52BssSize = (partition52BssEnd - partition52BssStart) / 8; 
  partition53BssSize = (partition53BssEnd - partition53BssStart) / 8; 
  partition54BssSize = (partition54BssEnd - partition54BssStart) / 8; 
  partition55BssSize = (partition55BssEnd - partition55BssStart) / 8; 
  partition56BssSize = (partition56BssEnd - partition56BssStart) / 8; 
  partition57BssSize = (partition57BssEnd - partition57BssStart) / 8; 
  partition58BssSize = (partition58BssEnd - partition58BssStart) / 8; 
  partition59BssSize = (partition59BssEnd - partition59BssStart) / 8; 
  partition60BssSize = (partition60BssEnd - partition60BssStart) / 8; 
  partition61BssSize = (partition61BssEnd - partition61BssStart) / 8; 
  partition62BssSize = (partition62BssEnd - partition62BssStart) / 8; 
  partition63BssSize = (partition63BssEnd - partition63BssStart) / 8; 


  /******************************************************************/
  /* Partition 0                                                    */
  /******************************************************************/

  /* address in the flash of the partition 0 data to copy it to RAM */
  pharosPartition0DataAddr = LOADADDR(.partition0);

  /* data of partition */
  .partition0 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition0DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_0_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition0Start = ABSOLUTE(.);

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

  } > ram AT > flash = 0xff


  /* bss of partition 0 */
  .partition0Bss (NOLOAD) : ALIGN(8)
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

    /* address of the partition 0 bss section end in RAM */
    . = ALIGN(8);
    partition0BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition0UssStart = .;

    *(.partition0Uss)
    *(.partition0Uss.*)

    partition0UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_0_ALIGNMENT);

    /* address of the partition 0 sections end in RAM */
    partition0End = .;

  } > ram

  /******************************************************************/
  /* Partition 1                                                    */
  /******************************************************************/

  /* address in the flash of the partition 1 data to copy it to RAM */
  pharosPartition1DataAddr = LOADADDR(.partition1);

  /* data of partition */
  .partition1 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition1DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_1_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition1Start = ABSOLUTE(.);

    *(.partition1)
    *(.partition1.*)

    /* address of the partition 1 data section end in RAM */
    . = ALIGN(8);
    partition1DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 1 */
  .partition1Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 1 bss section start in RAM */
    . = ALIGN(8);
    partition1BssStart = .;

    *(.partition1Bss)
    *(.partition1Bss.*)

    /* address of the partition 1 bss section end in RAM */
    . = ALIGN(8);
    partition1BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition1UssStart = .;

    *(.partition1Uss)
    *(.partition1Uss.*)

    partition1UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_1_ALIGNMENT);

    /* address of the partition 1 sections end in RAM */
    partition1End = .;

  } > ram

  /******************************************************************/
  /* Partition 2                                                    */
  /******************************************************************/

  /* address in the flash of the partition 2 data to copy it to RAM */
  pharosPartition2DataAddr = LOADADDR(.partition2);

  /* data of partition */
  .partition2 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition2DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_2_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition2Start = ABSOLUTE(.);

    *(.partition2)
    *(.partition2.*)

    /* address of the partition 2 data section end in RAM */
    . = ALIGN(8);
    partition2DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 2 */
  .partition2Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 2 bss section start in RAM */
    . = ALIGN(8);
    partition2BssStart = .;

    *(.partition2Bss)
    *(.partition2Bss.*)

    /* address of the partition 2 bss section end in RAM */
    . = ALIGN(8);
    partition2BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition2UssStart = .;

    *(.partition2Uss)
    *(.partition2Uss.*)

    partition2UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_2_ALIGNMENT);

    /* address of the partition 2 sections end in RAM */
    partition2End = .;

  } > ram

  /******************************************************************/
  /* Partition 3                                                    */
  /******************************************************************/

  /* address in the flash of the partition 3 data to copy it to RAM */
  pharosPartition3DataAddr = LOADADDR(.partition3);

  /* data of partition */
  .partition3 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition3DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_3_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition3Start = ABSOLUTE(.);

    *(.partition3)
    *(.partition3.*)

    /* address of the partition 3 data section end in RAM */
    . = ALIGN(8);
    partition3DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 3 */
  .partition3Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 3 bss section start in RAM */
    . = ALIGN(8);
    partition3BssStart = .;

    *(.partition3Bss)
    *(.partition3Bss.*)

    /* address of the partition 3 bss section end in RAM */
    . = ALIGN(8);
    partition3BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition3UssStart = .;

    *(.partition3Uss)
    *(.partition3Uss.*)

    partition3UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_3_ALIGNMENT);

    /* address of the partition 3 sections end in RAM */
    partition3End = .;

  } > ram

  /******************************************************************/
  /* Partition 4                                                    */
  /******************************************************************/

  /* address in the flash of the partition 4 data to copy it to RAM */
  pharosPartition4DataAddr = LOADADDR(.partition4);

  /* data of partition */
  .partition4 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition4DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_4_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition4Start = ABSOLUTE(.);

    *(.partition4)
    *(.partition4.*)

    /* address of the partition 4 data section end in RAM */
    . = ALIGN(8);
    partition4DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 4 */
  .partition4Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 4 bss section start in RAM */
    . = ALIGN(8);
    partition4BssStart = .;

    *(.partition4Bss)
    *(.partition4Bss.*)

    /* address of the partition 4 bss section end in RAM */
    . = ALIGN(8);
    partition4BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition4UssStart = .;

    *(.partition4Uss)
    *(.partition4Uss.*)

    partition4UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_4_ALIGNMENT);

    /* address of the partition 4 sections end in RAM */
    partition4End = .;

  } > ram

  /******************************************************************/
  /* Partition 5                                                    */
  /******************************************************************/

  /* address in the flash of the partition 5 data to copy it to RAM */
  pharosPartition5DataAddr = LOADADDR(.partition5);

  /* data of partition */
  .partition5 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition5DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_5_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition5Start = ABSOLUTE(.);

    *(.partition5)
    *(.partition5.*)

    /* address of the partition 5 data section end in RAM */
    . = ALIGN(8);
    partition5DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 5 */
  .partition5Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 5 bss section start in RAM */
    . = ALIGN(8);
    partition5BssStart = .;

    *(.partition5Bss)
    *(.partition5Bss.*)

    /* address of the partition 5 bss section end in RAM */
    . = ALIGN(8);
    partition5BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition5UssStart = .;

    *(.partition5Uss)
    *(.partition5Uss.*)

    partition5UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_5_ALIGNMENT);

    /* address of the partition 5 sections end in RAM */
    partition5End = .;

  } > ram

  /******************************************************************/
  /* Partition 6                                                    */
  /******************************************************************/

  /* address in the flash of the partition 6 data to copy it to RAM */
  pharosPartition6DataAddr = LOADADDR(.partition6);

  /* data of partition */
  .partition6 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition6DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_6_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition6Start = ABSOLUTE(.);

    *(.partition6)
    *(.partition6.*)

    /* address of the partition 6 data section end in RAM */
    . = ALIGN(8);
    partition6DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 6 */
  .partition6Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 6 bss section start in RAM */
    . = ALIGN(8);
    partition6BssStart = .;

    *(.partition6Bss)
    *(.partition6Bss.*)

    /* address of the partition 6 bss section end in RAM */
    . = ALIGN(8);
    partition6BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition6UssStart = .;

    *(.partition6Uss)
    *(.partition6Uss.*)

    partition6UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_6_ALIGNMENT);

    /* address of the partition 6 sections end in RAM */
    partition6End = .;

  } > ram

  /******************************************************************/
  /* Partition 7                                                    */
  /******************************************************************/

  /* address in the flash of the partition 7 data to copy it to RAM */
  pharosPartition7DataAddr = LOADADDR(.partition7);

  /* data of partition */
  .partition7 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition7DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_7_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition7Start = ABSOLUTE(.);

    *(.partition7)
    *(.partition7.*)

    /* address of the partition 7 data section end in RAM */
    . = ALIGN(8);
    partition7DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 7 */
  .partition7Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 7 bss section start in RAM */
    . = ALIGN(8);
    partition7BssStart = .;

    *(.partition7Bss)
    *(.partition7Bss.*)

    /* address of the partition 7 bss section end in RAM */
    . = ALIGN(8);
    partition7BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition7UssStart = .;

    *(.partition7Uss)
    *(.partition7Uss.*)

    partition7UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_7_ALIGNMENT);

    /* address of the partition 7 sections end in RAM */
    partition7End = .;

  } > ram

  /******************************************************************/
  /* Partition 8                                                    */
  /******************************************************************/

  /* address in the flash of the partition 8 data to copy it to RAM */
  pharosPartition8DataAddr = LOADADDR(.partition8);

  /* data of partition */
  .partition8 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition8DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_8_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition8Start = ABSOLUTE(.);

    *(.partition8)
    *(.partition8.*)

    /* address of the partition 8 data section end in RAM */
    . = ALIGN(8);
    partition8DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 8 */
  .partition8Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 8 bss section start in RAM */
    . = ALIGN(8);
    partition8BssStart = .;

    *(.partition8Bss)
    *(.partition8Bss.*)

    /* address of the partition 8 bss section end in RAM */
    . = ALIGN(8);
    partition8BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition8UssStart = .;

    *(.partition8Uss)
    *(.partition8Uss.*)

    partition8UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_8_ALIGNMENT);

    /* address of the partition 8 sections end in RAM */
    partition8End = .;

  } > ram

  /******************************************************************/
  /* Partition 9                                                    */
  /******************************************************************/

  /* address in the flash of the partition 9 data to copy it to RAM */
  pharosPartition9DataAddr = LOADADDR(.partition9);

  /* data of partition */
  .partition9 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition9DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_9_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition9Start = ABSOLUTE(.);

    *(.partition9)
    *(.partition9.*)

    /* address of the partition 9 data section end in RAM */
    . = ALIGN(8);
    partition9DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 9 */
  .partition9Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 9 bss section start in RAM */
    . = ALIGN(8);
    partition9BssStart = .;

    *(.partition9Bss)
    *(.partition9Bss.*)

    /* address of the partition 9 bss section end in RAM */
    . = ALIGN(8);
    partition9BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition9UssStart = .;

    *(.partition9Uss)
    *(.partition9Uss.*)

    partition9UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_9_ALIGNMENT);

    /* address of the partition 9 sections end in RAM */
    partition9End = .;

  } > ram

  /******************************************************************/
  /* Partition 10                                                   */
  /******************************************************************/

  /* address in the flash of the partition 10 data to copy it to RAM */
  pharosPartition10DataAddr = LOADADDR(.partition10);

  /* data of partition */
  .partition10 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition10DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_10_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition10Start = ABSOLUTE(.);

    *(.partition10)
    *(.partition10.*)

    /* address of the partition 10 data section end in RAM */
    . = ALIGN(8);
    partition10DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 10 */
  .partition10Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 10 bss section start in RAM */
    . = ALIGN(8);
    partition10BssStart = .;

    *(.partition10Bss)
    *(.partition10Bss.*)

    /* address of the partition 10 bss section end in RAM */
    . = ALIGN(8);
    partition10BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition10UssStart = .;

    *(.partition10Uss)
    *(.partition10Uss.*)

    partition10UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_10_ALIGNMENT);

    /* address of the partition 10 sections end in RAM */
    partition10End = .;

  } > ram

  /******************************************************************/
  /* Partition 11                                                   */
  /******************************************************************/

  /* address in the flash of the partition 11 data to copy it to RAM */
  pharosPartition11DataAddr = LOADADDR(.partition11);

  /* data of partition */
  .partition11 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition11DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_11_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition11Start = ABSOLUTE(.);

    *(.partition11)
    *(.partition11.*)

    /* address of the partition 11 data section end in RAM */
    . = ALIGN(8);
    partition11DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 11 */
  .partition11Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 11 bss section start in RAM */
    . = ALIGN(8);
    partition11BssStart = .;

    *(.partition11Bss)
    *(.partition11Bss.*)

    /* address of the partition 11 bss section end in RAM */
    . = ALIGN(8);
    partition11BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition11UssStart = .;

    *(.partition11Uss)
    *(.partition11Uss.*)

    partition11UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_11_ALIGNMENT);

    /* address of the partition 11 sections end in RAM */
    partition11End = .;

  } > ram

  /******************************************************************/
  /* Partition 12                                                   */
  /******************************************************************/

  /* address in the flash of the partition 12 data to copy it to RAM */
  pharosPartition12DataAddr = LOADADDR(.partition12);

  /* data of partition */
  .partition12 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition12DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_12_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition12Start = ABSOLUTE(.);

    *(.partition12)
    *(.partition12.*)

    /* address of the partition 12 data section end in RAM */
    . = ALIGN(8);
    partition12DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 12 */
  .partition12Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 12 bss section start in RAM */
    . = ALIGN(8);
    partition12BssStart = .;

    *(.partition12Bss)
    *(.partition12Bss.*)

    /* address of the partition 12 bss section end in RAM */
    . = ALIGN(8);
    partition12BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition12UssStart = .;

    *(.partition12Uss)
    *(.partition12Uss.*)

    partition12UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_12_ALIGNMENT);

    /* address of the partition 12 sections end in RAM */
    partition12End = .;

  } > ram

  /******************************************************************/
  /* Partition 13                                                   */
  /******************************************************************/

  /* address in the flash of the partition 13 data to copy it to RAM */
  pharosPartition13DataAddr = LOADADDR(.partition13);

  /* data of partition */
  .partition13 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition13DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_13_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition13Start = ABSOLUTE(.);

    *(.partition13)
    *(.partition13.*)

    /* address of the partition 13 data section end in RAM */
    . = ALIGN(8);
    partition13DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 13 */
  .partition13Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 13 bss section start in RAM */
    . = ALIGN(8);
    partition13BssStart = .;

    *(.partition13Bss)
    *(.partition13Bss.*)

    /* address of the partition 13 bss section end in RAM */
    . = ALIGN(8);
    partition13BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition13UssStart = .;

    *(.partition13Uss)
    *(.partition13Uss.*)

    partition13UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_13_ALIGNMENT);

    /* address of the partition 13 sections end in RAM */
    partition13End = .;

  } > ram

  /******************************************************************/
  /* Partition 14                                                   */
  /******************************************************************/

  /* address in the flash of the partition 14 data to copy it to RAM */
  pharosPartition14DataAddr = LOADADDR(.partition14);

  /* data of partition */
  .partition14 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition14DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_14_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition14Start = ABSOLUTE(.);

    *(.partition14)
    *(.partition14.*)

    /* address of the partition 14 data section end in RAM */
    . = ALIGN(8);
    partition14DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 14 */
  .partition14Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 14 bss section start in RAM */
    . = ALIGN(8);
    partition14BssStart = .;

    *(.partition14Bss)
    *(.partition14Bss.*)

    /* address of the partition 14 bss section end in RAM */
    . = ALIGN(8);
    partition14BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition14UssStart = .;

    *(.partition14Uss)
    *(.partition14Uss.*)

    partition14UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_14_ALIGNMENT);

    /* address of the partition 14 sections end in RAM */
    partition14End = .;

  } > ram

  /******************************************************************/
  /* Partition 15                                                   */
  /******************************************************************/

  /* address in the flash of the partition 15 data to copy it to RAM */
  pharosPartition15DataAddr = LOADADDR(.partition15);

  /* data of partition */
  .partition15 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition15DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_15_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition15Start = ABSOLUTE(.);

    *(.partition15)
    *(.partition15.*)

    /* address of the partition 15 data section end in RAM */
    . = ALIGN(8);
    partition15DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 15 */
  .partition15Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 15 bss section start in RAM */
    . = ALIGN(8);
    partition15BssStart = .;

    *(.partition15Bss)
    *(.partition15Bss.*)

    /* address of the partition 15 bss section end in RAM */
    . = ALIGN(8);
    partition15BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition15UssStart = .;

    *(.partition15Uss)
    *(.partition15Uss.*)

    partition15UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_15_ALIGNMENT);

    /* address of the partition 15 sections end in RAM */
    partition15End = .;

  } > ram

  /******************************************************************/
  /* Partition 16                                                   */
  /******************************************************************/

  /* address in the flash of the partition 16 data to copy it to RAM */
  pharosPartition16DataAddr = LOADADDR(.partition16);

  /* data of partition */
  .partition16 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition16DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_16_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition16Start = ABSOLUTE(.);

    *(.partition16)
    *(.partition16.*)

    /* address of the partition 16 data section end in RAM */
    . = ALIGN(8);
    partition16DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 16 */
  .partition16Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 16 bss section start in RAM */
    . = ALIGN(8);
    partition16BssStart = .;

    *(.partition16Bss)
    *(.partition16Bss.*)

    /* address of the partition 16 bss section end in RAM */
    . = ALIGN(8);
    partition16BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition16UssStart = .;

    *(.partition16Uss)
    *(.partition16Uss.*)

    partition16UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_16_ALIGNMENT);

    /* address of the partition 16 sections end in RAM */
    partition16End = .;

  } > ram

  /******************************************************************/
  /* Partition 17                                                   */
  /******************************************************************/

  /* address in the flash of the partition 17 data to copy it to RAM */
  pharosPartition17DataAddr = LOADADDR(.partition17);

  /* data of partition */
  .partition17 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition17DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_17_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition17Start = ABSOLUTE(.);

    *(.partition17)
    *(.partition17.*)

    /* address of the partition 17 data section end in RAM */
    . = ALIGN(8);
    partition17DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 17 */
  .partition17Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 17 bss section start in RAM */
    . = ALIGN(8);
    partition17BssStart = .;

    *(.partition17Bss)
    *(.partition17Bss.*)

    /* address of the partition 17 bss section end in RAM */
    . = ALIGN(8);
    partition17BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition17UssStart = .;

    *(.partition17Uss)
    *(.partition17Uss.*)

    partition17UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_17_ALIGNMENT);

    /* address of the partition 17 sections end in RAM */
    partition17End = .;

  } > ram

  /******************************************************************/
  /* Partition 18                                                   */
  /******************************************************************/

  /* address in the flash of the partition 18 data to copy it to RAM */
  pharosPartition18DataAddr = LOADADDR(.partition18);

  /* data of partition */
  .partition18 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition18DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_18_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition18Start = ABSOLUTE(.);

    *(.partition18)
    *(.partition18.*)

    /* address of the partition 18 data section end in RAM */
    . = ALIGN(8);
    partition18DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 18 */
  .partition18Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 18 bss section start in RAM */
    . = ALIGN(8);
    partition18BssStart = .;

    *(.partition18Bss)
    *(.partition18Bss.*)

    /* address of the partition 18 bss section end in RAM */
    . = ALIGN(8);
    partition18BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition18UssStart = .;

    *(.partition18Uss)
    *(.partition18Uss.*)

    partition18UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_18_ALIGNMENT);

    /* address of the partition 18 sections end in RAM */
    partition18End = .;

  } > ram

  /******************************************************************/
  /* Partition 19                                                   */
  /******************************************************************/

  /* address in the flash of the partition 19 data to copy it to RAM */
  pharosPartition19DataAddr = LOADADDR(.partition19);

  /* data of partition */
  .partition19 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition19DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_19_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition19Start = ABSOLUTE(.);

    *(.partition19)
    *(.partition19.*)

    /* address of the partition 19 data section end in RAM */
    . = ALIGN(8);
    partition19DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 19 */
  .partition19Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 19 bss section start in RAM */
    . = ALIGN(8);
    partition19BssStart = .;

    *(.partition19Bss)
    *(.partition19Bss.*)

    /* address of the partition 19 bss section end in RAM */
    . = ALIGN(8);
    partition19BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition19UssStart = .;

    *(.partition19Uss)
    *(.partition19Uss.*)

    partition19UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_19_ALIGNMENT);

    /* address of the partition 19 sections end in RAM */
    partition19End = .;

  } > ram

  /******************************************************************/
  /* Partition 20                                                   */
  /******************************************************************/

  /* address in the flash of the partition 20 data to copy it to RAM */
  pharosPartition20DataAddr = LOADADDR(.partition20);

  /* data of partition */
  .partition20 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition20DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_20_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition20Start = ABSOLUTE(.);

    *(.partition20)
    *(.partition20.*)

    /* address of the partition 20 data section end in RAM */
    . = ALIGN(8);
    partition20DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 20 */
  .partition20Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 20 bss section start in RAM */
    . = ALIGN(8);
    partition20BssStart = .;

    *(.partition20Bss)
    *(.partition20Bss.*)

    /* address of the partition 20 bss section end in RAM */
    . = ALIGN(8);
    partition20BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition20UssStart = .;

    *(.partition20Uss)
    *(.partition20Uss.*)

    partition20UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_20_ALIGNMENT);

    /* address of the partition 20 sections end in RAM */
    partition20End = .;

  } > ram

  /******************************************************************/
  /* Partition 21                                                   */
  /******************************************************************/

  /* address in the flash of the partition 21 data to copy it to RAM */
  pharosPartition21DataAddr = LOADADDR(.partition21);

  /* data of partition */
  .partition21 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition21DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_21_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition21Start = ABSOLUTE(.);

    *(.partition21)
    *(.partition21.*)

    /* address of the partition 21 data section end in RAM */
    . = ALIGN(8);
    partition21DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 21 */
  .partition21Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 21 bss section start in RAM */
    . = ALIGN(8);
    partition21BssStart = .;

    *(.partition21Bss)
    *(.partition21Bss.*)

    /* address of the partition 21 bss section end in RAM */
    . = ALIGN(8);
    partition21BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition21UssStart = .;

    *(.partition21Uss)
    *(.partition21Uss.*)

    partition21UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_21_ALIGNMENT);

    /* address of the partition 21 sections end in RAM */
    partition21End = .;

  } > ram

  /******************************************************************/
  /* Partition 22                                                   */
  /******************************************************************/

  /* address in the flash of the partition 22 data to copy it to RAM */
  pharosPartition22DataAddr = LOADADDR(.partition22);

  /* data of partition */
  .partition22 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition22DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_22_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition22Start = ABSOLUTE(.);

    *(.partition22)
    *(.partition22.*)

    /* address of the partition 22 data section end in RAM */
    . = ALIGN(8);
    partition22DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 22 */
  .partition22Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 22 bss section start in RAM */
    . = ALIGN(8);
    partition22BssStart = .;

    *(.partition22Bss)
    *(.partition22Bss.*)

    /* address of the partition 22 bss section end in RAM */
    . = ALIGN(8);
    partition22BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition22UssStart = .;

    *(.partition22Uss)
    *(.partition22Uss.*)

    partition22UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_22_ALIGNMENT);

    /* address of the partition 22 sections end in RAM */
    partition22End = .;

  } > ram

  /******************************************************************/
  /* Partition 23                                                   */
  /******************************************************************/

  /* address in the flash of the partition 23 data to copy it to RAM */
  pharosPartition23DataAddr = LOADADDR(.partition23);

  /* data of partition */
  .partition23 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition23DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_23_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition23Start = ABSOLUTE(.);

    *(.partition23)
    *(.partition23.*)

    /* address of the partition 23 data section end in RAM */
    . = ALIGN(8);
    partition23DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 23 */
  .partition23Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 23 bss section start in RAM */
    . = ALIGN(8);
    partition23BssStart = .;

    *(.partition23Bss)
    *(.partition23Bss.*)

    /* address of the partition 23 bss section end in RAM */
    . = ALIGN(8);
    partition23BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition23UssStart = .;

    *(.partition23Uss)
    *(.partition23Uss.*)

    partition23UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_23_ALIGNMENT);

    /* address of the partition 23 sections end in RAM */
    partition23End = .;

  } > ram

  /******************************************************************/
  /* Partition 24                                                   */
  /******************************************************************/

  /* address in the flash of the partition 24 data to copy it to RAM */
  pharosPartition24DataAddr = LOADADDR(.partition24);

  /* data of partition */
  .partition24 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition24DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_24_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition24Start = ABSOLUTE(.);

    *(.partition24)
    *(.partition24.*)

    /* address of the partition 24 data section end in RAM */
    . = ALIGN(8);
    partition24DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 24 */
  .partition24Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 24 bss section start in RAM */
    . = ALIGN(8);
    partition24BssStart = .;

    *(.partition24Bss)
    *(.partition24Bss.*)

    /* address of the partition 24 bss section end in RAM */
    . = ALIGN(8);
    partition24BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition24UssStart = .;

    *(.partition24Uss)
    *(.partition24Uss.*)

    partition24UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_24_ALIGNMENT);

    /* address of the partition 24 sections end in RAM */
    partition24End = .;

  } > ram

  /******************************************************************/
  /* Partition 25                                                   */
  /******************************************************************/

  /* address in the flash of the partition 25 data to copy it to RAM */
  pharosPartition25DataAddr = LOADADDR(.partition25);

  /* data of partition */
  .partition25 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition25DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_25_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition25Start = ABSOLUTE(.);

    *(.partition25)
    *(.partition25.*)

    /* address of the partition 25 data section end in RAM */
    . = ALIGN(8);
    partition25DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 25 */
  .partition25Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 25 bss section start in RAM */
    . = ALIGN(8);
    partition25BssStart = .;

    *(.partition25Bss)
    *(.partition25Bss.*)

    /* address of the partition 25 bss section end in RAM */
    . = ALIGN(8);
    partition25BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition25UssStart = .;

    *(.partition25Uss)
    *(.partition25Uss.*)

    partition25UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_25_ALIGNMENT);

    /* address of the partition 25 sections end in RAM */
    partition25End = .;

  } > ram

  /******************************************************************/
  /* Partition 26                                                   */
  /******************************************************************/

  /* address in the flash of the partition 26 data to copy it to RAM */
  pharosPartition26DataAddr = LOADADDR(.partition26);

  /* data of partition */
  .partition26 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition26DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_26_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition26Start = ABSOLUTE(.);

    *(.partition26)
    *(.partition26.*)

    /* address of the partition 26 data section end in RAM */
    . = ALIGN(8);
    partition26DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 26 */
  .partition26Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 26 bss section start in RAM */
    . = ALIGN(8);
    partition26BssStart = .;

    *(.partition26Bss)
    *(.partition26Bss.*)

    /* address of the partition 26 bss section end in RAM */
    . = ALIGN(8);
    partition26BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition26UssStart = .;

    *(.partition26Uss)
    *(.partition26Uss.*)

    partition26UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_26_ALIGNMENT);

    /* address of the partition 26 sections end in RAM */
    partition26End = .;

  } > ram

  /******************************************************************/
  /* Partition 27                                                   */
  /******************************************************************/

  /* address in the flash of the partition 27 data to copy it to RAM */
  pharosPartition27DataAddr = LOADADDR(.partition27);

  /* data of partition */
  .partition27 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition27DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_27_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition27Start = ABSOLUTE(.);

    *(.partition27)
    *(.partition27.*)

    /* address of the partition 27 data section end in RAM */
    . = ALIGN(8);
    partition27DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 27 */
  .partition27Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 27 bss section start in RAM */
    . = ALIGN(8);
    partition27BssStart = .;

    *(.partition27Bss)
    *(.partition27Bss.*)

    /* address of the partition 27 bss section end in RAM */
    . = ALIGN(8);
    partition27BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition27UssStart = .;

    *(.partition27Uss)
    *(.partition27Uss.*)

    partition27UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_27_ALIGNMENT);

    /* address of the partition 27 sections end in RAM */
    partition27End = .;

  } > ram

  /******************************************************************/
  /* Partition 28                                                   */
  /******************************************************************/

  /* address in the flash of the partition 28 data to copy it to RAM */
  pharosPartition28DataAddr = LOADADDR(.partition28);

  /* data of partition */
  .partition28 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition28DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_28_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition28Start = ABSOLUTE(.);

    *(.partition28)
    *(.partition28.*)

    /* address of the partition 28 data section end in RAM */
    . = ALIGN(8);
    partition28DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 28 */
  .partition28Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 28 bss section start in RAM */
    . = ALIGN(8);
    partition28BssStart = .;

    *(.partition28Bss)
    *(.partition28Bss.*)

    /* address of the partition 28 bss section end in RAM */
    . = ALIGN(8);
    partition28BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition28UssStart = .;

    *(.partition28Uss)
    *(.partition28Uss.*)

    partition28UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_28_ALIGNMENT);

    /* address of the partition 28 sections end in RAM */
    partition28End = .;

  } > ram

  /******************************************************************/
  /* Partition 29                                                   */
  /******************************************************************/

  /* address in the flash of the partition 29 data to copy it to RAM */
  pharosPartition29DataAddr = LOADADDR(.partition29);

  /* data of partition */
  .partition29 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition29DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_29_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition29Start = ABSOLUTE(.);

    *(.partition29)
    *(.partition29.*)

    /* address of the partition 29 data section end in RAM */
    . = ALIGN(8);
    partition29DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 29 */
  .partition29Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 29 bss section start in RAM */
    . = ALIGN(8);
    partition29BssStart = .;

    *(.partition29Bss)
    *(.partition29Bss.*)

    /* address of the partition 29 bss section end in RAM */
    . = ALIGN(8);
    partition29BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition29UssStart = .;

    *(.partition29Uss)
    *(.partition29Uss.*)

    partition29UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_29_ALIGNMENT);

    /* address of the partition 29 sections end in RAM */
    partition29End = .;

  } > ram

  /******************************************************************/
  /* Partition 30                                                   */
  /******************************************************************/

  /* address in the flash of the partition 30 data to copy it to RAM */
  pharosPartition30DataAddr = LOADADDR(.partition30);

  /* data of partition */
  .partition30 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition30DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_30_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition30Start = ABSOLUTE(.);

    *(.partition30)
    *(.partition30.*)

    /* address of the partition 30 data section end in RAM */
    . = ALIGN(8);
    partition30DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 30 */
  .partition30Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 30 bss section start in RAM */
    . = ALIGN(8);
    partition30BssStart = .;

    *(.partition30Bss)
    *(.partition30Bss.*)

    /* address of the partition 30 bss section end in RAM */
    . = ALIGN(8);
    partition30BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition30UssStart = .;

    *(.partition30Uss)
    *(.partition30Uss.*)

    partition30UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_30_ALIGNMENT);

    /* address of the partition 30 sections end in RAM */
    partition30End = .;

  } > ram

  /******************************************************************/
  /* Partition 31                                                   */
  /******************************************************************/

  /* address in the flash of the partition 31 data to copy it to RAM */
  pharosPartition31DataAddr = LOADADDR(.partition31);

  /* data of partition */
  .partition31 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition31DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_31_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition31Start = ABSOLUTE(.);

    *(.partition31)
    *(.partition31.*)

    /* address of the partition 31 data section end in RAM */
    . = ALIGN(8);
    partition31DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 31 */
  .partition31Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 31 bss section start in RAM */
    . = ALIGN(8);
    partition31BssStart = .;

    *(.partition31Bss)
    *(.partition31Bss.*)

    /* address of the partition 31 bss section end in RAM */
    . = ALIGN(8);
    partition31BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition31UssStart = .;

    *(.partition31Uss)
    *(.partition31Uss.*)

    partition31UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_31_ALIGNMENT);

    /* address of the partition 31 sections end in RAM */
    partition31End = .;

  } > ram

  /******************************************************************/
  /* Partition 32                                                   */
  /******************************************************************/

  /* address in the flash of the partition 32 data to copy it to RAM */
  pharosPartition32DataAddr = LOADADDR(.partition32);

  /* data of partition */
  .partition32 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition32DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_32_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition32Start = ABSOLUTE(.);

    *(.partition32)
    *(.partition32.*)

    /* address of the partition 32 data section end in RAM */
    . = ALIGN(8);
    partition32DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 32 */
  .partition32Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 32 bss section start in RAM */
    . = ALIGN(8);
    partition32BssStart = .;

    *(.partition32Bss)
    *(.partition32Bss.*)

    /* address of the partition 32 bss section end in RAM */
    . = ALIGN(8);
    partition32BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition32UssStart = .;

    *(.partition32Uss)
    *(.partition32Uss.*)

    partition32UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_32_ALIGNMENT);

    /* address of the partition 32 sections end in RAM */
    partition32End = .;

  } > ram

  /******************************************************************/
  /* Partition 33                                                   */
  /******************************************************************/

  /* address in the flash of the partition 33 data to copy it to RAM */
  pharosPartition33DataAddr = LOADADDR(.partition33);

  /* data of partition */
  .partition33 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition33DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_33_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition33Start = ABSOLUTE(.);

    *(.partition33)
    *(.partition33.*)

    /* address of the partition 33 data section end in RAM */
    . = ALIGN(8);
    partition33DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 33 */
  .partition33Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 33 bss section start in RAM */
    . = ALIGN(8);
    partition33BssStart = .;

    *(.partition33Bss)
    *(.partition33Bss.*)

    /* address of the partition 33 bss section end in RAM */
    . = ALIGN(8);
    partition33BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition33UssStart = .;

    *(.partition33Uss)
    *(.partition33Uss.*)

    partition33UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_33_ALIGNMENT);

    /* address of the partition 33 sections end in RAM */
    partition33End = .;

  } > ram

  /******************************************************************/
  /* Partition 34                                                   */
  /******************************************************************/

  /* address in the flash of the partition 34 data to copy it to RAM */
  pharosPartition34DataAddr = LOADADDR(.partition34);

  /* data of partition */
  .partition34 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition34DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_34_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition34Start = ABSOLUTE(.);

    *(.partition34)
    *(.partition34.*)

    /* address of the partition 34 data section end in RAM */
    . = ALIGN(8);
    partition34DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 34 */
  .partition34Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 34 bss section start in RAM */
    . = ALIGN(8);
    partition34BssStart = .;

    *(.partition34Bss)
    *(.partition34Bss.*)

    /* address of the partition 34 bss section end in RAM */
    . = ALIGN(8);
    partition34BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition34UssStart = .;

    *(.partition34Uss)
    *(.partition34Uss.*)

    partition34UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_34_ALIGNMENT);

    /* address of the partition 34 sections end in RAM */
    partition34End = .;

  } > ram

  /******************************************************************/
  /* Partition 35                                                   */
  /******************************************************************/

  /* address in the flash of the partition 35 data to copy it to RAM */
  pharosPartition35DataAddr = LOADADDR(.partition35);

  /* data of partition */
  .partition35 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition35DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_35_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition35Start = ABSOLUTE(.);

    *(.partition35)
    *(.partition35.*)

    /* address of the partition 35 data section end in RAM */
    . = ALIGN(8);
    partition35DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 35 */
  .partition35Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 35 bss section start in RAM */
    . = ALIGN(8);
    partition35BssStart = .;

    *(.partition35Bss)
    *(.partition35Bss.*)

    /* address of the partition 35 bss section end in RAM */
    . = ALIGN(8);
    partition35BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition35UssStart = .;

    *(.partition35Uss)
    *(.partition35Uss.*)

    partition35UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_35_ALIGNMENT);

    /* address of the partition 35 sections end in RAM */
    partition35End = .;

  } > ram

  /******************************************************************/
  /* Partition 36                                                   */
  /******************************************************************/

  /* address in the flash of the partition 36 data to copy it to RAM */
  pharosPartition36DataAddr = LOADADDR(.partition36);

  /* data of partition */
  .partition36 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition36DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_36_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition36Start = ABSOLUTE(.);

    *(.partition36)
    *(.partition36.*)

    /* address of the partition 36 data section end in RAM */
    . = ALIGN(8);
    partition36DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 36 */
  .partition36Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 36 bss section start in RAM */
    . = ALIGN(8);
    partition36BssStart = .;

    *(.partition36Bss)
    *(.partition36Bss.*)

    /* address of the partition 36 bss section end in RAM */
    . = ALIGN(8);
    partition36BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition36UssStart = .;

    *(.partition36Uss)
    *(.partition36Uss.*)

    partition36UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_36_ALIGNMENT);

    /* address of the partition 36 sections end in RAM */
    partition36End = .;

  } > ram

  /******************************************************************/
  /* Partition 37                                                   */
  /******************************************************************/

  /* address in the flash of the partition 37 data to copy it to RAM */
  pharosPartition37DataAddr = LOADADDR(.partition37);

  /* data of partition */
  .partition37 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition37DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_37_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition37Start = ABSOLUTE(.);

    *(.partition37)
    *(.partition37.*)

    /* address of the partition 37 data section end in RAM */
    . = ALIGN(8);
    partition37DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 37 */
  .partition37Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 37 bss section start in RAM */
    . = ALIGN(8);
    partition37BssStart = .;

    *(.partition37Bss)
    *(.partition37Bss.*)

    /* address of the partition 37 bss section end in RAM */
    . = ALIGN(8);
    partition37BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition37UssStart = .;

    *(.partition37Uss)
    *(.partition37Uss.*)

    partition37UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_37_ALIGNMENT);

    /* address of the partition 37 sections end in RAM */
    partition37End = .;

  } > ram

  /******************************************************************/
  /* Partition 38                                                   */
  /******************************************************************/

  /* address in the flash of the partition 38 data to copy it to RAM */
  pharosPartition38DataAddr = LOADADDR(.partition38);

  /* data of partition */
  .partition38 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition38DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_38_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition38Start = ABSOLUTE(.);

    *(.partition38)
    *(.partition38.*)

    /* address of the partition 38 data section end in RAM */
    . = ALIGN(8);
    partition38DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 38 */
  .partition38Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 38 bss section start in RAM */
    . = ALIGN(8);
    partition38BssStart = .;

    *(.partition38Bss)
    *(.partition38Bss.*)

    /* address of the partition 38 bss section end in RAM */
    . = ALIGN(8);
    partition38BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition38UssStart = .;

    *(.partition38Uss)
    *(.partition38Uss.*)

    partition38UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_38_ALIGNMENT);

    /* address of the partition 38 sections end in RAM */
    partition38End = .;

  } > ram

  /******************************************************************/
  /* Partition 39                                                   */
  /******************************************************************/

  /* address in the flash of the partition 39 data to copy it to RAM */
  pharosPartition39DataAddr = LOADADDR(.partition39);

  /* data of partition */
  .partition39 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition39DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_39_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition39Start = ABSOLUTE(.);

    *(.partition39)
    *(.partition39.*)

    /* address of the partition 39 data section end in RAM */
    . = ALIGN(8);
    partition39DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 39 */
  .partition39Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 39 bss section start in RAM */
    . = ALIGN(8);
    partition39BssStart = .;

    *(.partition39Bss)
    *(.partition39Bss.*)

    /* address of the partition 39 bss section end in RAM */
    . = ALIGN(8);
    partition39BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition39UssStart = .;

    *(.partition39Uss)
    *(.partition39Uss.*)

    partition39UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_39_ALIGNMENT);

    /* address of the partition 39 sections end in RAM */
    partition39End = .;

  } > ram

  /******************************************************************/
  /* Partition 40                                                   */
  /******************************************************************/

  /* address in the flash of the partition 40 data to copy it to RAM */
  pharosPartition40DataAddr = LOADADDR(.partition40);

  /* data of partition */
  .partition40 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition40DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_40_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition40Start = ABSOLUTE(.);

    *(.partition40)
    *(.partition40.*)

    /* address of the partition 40 data section end in RAM */
    . = ALIGN(8);
    partition40DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 40 */
  .partition40Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 40 bss section start in RAM */
    . = ALIGN(8);
    partition40BssStart = .;

    *(.partition40Bss)
    *(.partition40Bss.*)

    /* address of the partition 40 bss section end in RAM */
    . = ALIGN(8);
    partition40BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition40UssStart = .;

    *(.partition40Uss)
    *(.partition40Uss.*)

    partition40UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_40_ALIGNMENT);

    /* address of the partition 40 sections end in RAM */
    partition40End = .;

  } > ram

  /******************************************************************/
  /* Partition 41                                                   */
  /******************************************************************/

  /* address in the flash of the partition 41 data to copy it to RAM */
  pharosPartition41DataAddr = LOADADDR(.partition41);

  /* data of partition */
  .partition41 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition41DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_41_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition41Start = ABSOLUTE(.);

    *(.partition41)
    *(.partition41.*)

    /* address of the partition 41 data section end in RAM */
    . = ALIGN(8);
    partition41DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 41 */
  .partition41Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 41 bss section start in RAM */
    . = ALIGN(8);
    partition41BssStart = .;

    *(.partition41Bss)
    *(.partition41Bss.*)

    /* address of the partition 41 bss section end in RAM */
    . = ALIGN(8);
    partition41BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition41UssStart = .;

    *(.partition41Uss)
    *(.partition41Uss.*)

    partition41UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_41_ALIGNMENT);

    /* address of the partition 41 sections end in RAM */
    partition41End = .;

  } > ram

  /******************************************************************/
  /* Partition 42                                                   */
  /******************************************************************/

  /* address in the flash of the partition 42 data to copy it to RAM */
  pharosPartition42DataAddr = LOADADDR(.partition42);

  /* data of partition */
  .partition42 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition42DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_42_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition42Start = ABSOLUTE(.);

    *(.partition42)
    *(.partition42.*)

    /* address of the partition 42 data section end in RAM */
    . = ALIGN(8);
    partition42DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 42 */
  .partition42Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 42 bss section start in RAM */
    . = ALIGN(8);
    partition42BssStart = .;

    *(.partition42Bss)
    *(.partition42Bss.*)

    /* address of the partition 42 bss section end in RAM */
    . = ALIGN(8);
    partition42BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition42UssStart = .;

    *(.partition42Uss)
    *(.partition42Uss.*)

    partition42UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_42_ALIGNMENT);

    /* address of the partition 42 sections end in RAM */
    partition42End = .;

  } > ram

  /******************************************************************/
  /* Partition 43                                                   */
  /******************************************************************/

  /* address in the flash of the partition 43 data to copy it to RAM */
  pharosPartition43DataAddr = LOADADDR(.partition43);

  /* data of partition */
  .partition43 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition43DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_43_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition43Start = ABSOLUTE(.);

    *(.partition43)
    *(.partition43.*)

    /* address of the partition 43 data section end in RAM */
    . = ALIGN(8);
    partition43DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 43 */
  .partition43Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 43 bss section start in RAM */
    . = ALIGN(8);
    partition43BssStart = .;

    *(.partition43Bss)
    *(.partition43Bss.*)

    /* address of the partition 43 bss section end in RAM */
    . = ALIGN(8);
    partition43BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition43UssStart = .;

    *(.partition43Uss)
    *(.partition43Uss.*)

    partition43UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_43_ALIGNMENT);

    /* address of the partition 43 sections end in RAM */
    partition43End = .;

  } > ram

  /******************************************************************/
  /* Partition 44                                                   */
  /******************************************************************/

  /* address in the flash of the partition 44 data to copy it to RAM */
  pharosPartition44DataAddr = LOADADDR(.partition44);

  /* data of partition */
  .partition44 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition44DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_44_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition44Start = ABSOLUTE(.);

    *(.partition44)
    *(.partition44.*)

    /* address of the partition 44 data section end in RAM */
    . = ALIGN(8);
    partition44DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 44 */
  .partition44Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 44 bss section start in RAM */
    . = ALIGN(8);
    partition44BssStart = .;

    *(.partition44Bss)
    *(.partition44Bss.*)

    /* address of the partition 44 bss section end in RAM */
    . = ALIGN(8);
    partition44BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition44UssStart = .;

    *(.partition44Uss)
    *(.partition44Uss.*)

    partition44UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_44_ALIGNMENT);

    /* address of the partition 44 sections end in RAM */
    partition44End = .;

  } > ram

  /******************************************************************/
  /* Partition 45                                                   */
  /******************************************************************/

  /* address in the flash of the partition 45 data to copy it to RAM */
  pharosPartition45DataAddr = LOADADDR(.partition45);

  /* data of partition */
  .partition45 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition45DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_45_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition45Start = ABSOLUTE(.);

    *(.partition45)
    *(.partition45.*)

    /* address of the partition 45 data section end in RAM */
    . = ALIGN(8);
    partition45DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 45 */
  .partition45Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 45 bss section start in RAM */
    . = ALIGN(8);
    partition45BssStart = .;

    *(.partition45Bss)
    *(.partition45Bss.*)

    /* address of the partition 45 bss section end in RAM */
    . = ALIGN(8);
    partition45BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition45UssStart = .;

    *(.partition45Uss)
    *(.partition45Uss.*)

    partition45UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_45_ALIGNMENT);

    /* address of the partition 45 sections end in RAM */
    partition45End = .;

  } > ram

  /******************************************************************/
  /* Partition 46                                                   */
  /******************************************************************/

  /* address in the flash of the partition 46 data to copy it to RAM */
  pharosPartition46DataAddr = LOADADDR(.partition46);

  /* data of partition */
  .partition46 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition46DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_46_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition46Start = ABSOLUTE(.);

    *(.partition46)
    *(.partition46.*)

    /* address of the partition 46 data section end in RAM */
    . = ALIGN(8);
    partition46DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 46 */
  .partition46Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 46 bss section start in RAM */
    . = ALIGN(8);
    partition46BssStart = .;

    *(.partition46Bss)
    *(.partition46Bss.*)

    /* address of the partition 46 bss section end in RAM */
    . = ALIGN(8);
    partition46BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition46UssStart = .;

    *(.partition46Uss)
    *(.partition46Uss.*)

    partition46UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_46_ALIGNMENT);

    /* address of the partition 46 sections end in RAM */
    partition46End = .;

  } > ram

  /******************************************************************/
  /* Partition 47                                                   */
  /******************************************************************/

  /* address in the flash of the partition 47 data to copy it to RAM */
  pharosPartition47DataAddr = LOADADDR(.partition47);

  /* data of partition */
  .partition47 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition47DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_47_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition47Start = ABSOLUTE(.);

    *(.partition47)
    *(.partition47.*)

    /* address of the partition 47 data section end in RAM */
    . = ALIGN(8);
    partition47DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 47 */
  .partition47Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 47 bss section start in RAM */
    . = ALIGN(8);
    partition47BssStart = .;

    *(.partition47Bss)
    *(.partition47Bss.*)

    /* address of the partition 47 bss section end in RAM */
    . = ALIGN(8);
    partition47BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition47UssStart = .;

    *(.partition47Uss)
    *(.partition47Uss.*)

    partition47UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_47_ALIGNMENT);

    /* address of the partition 47 sections end in RAM */
    partition47End = .;

  } > ram

  /******************************************************************/
  /* Partition 48                                                   */
  /******************************************************************/

  /* address in the flash of the partition 48 data to copy it to RAM */
  pharosPartition48DataAddr = LOADADDR(.partition48);

  /* data of partition */
  .partition48 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition48DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_48_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition48Start = ABSOLUTE(.);

    *(.partition48)
    *(.partition48.*)

    /* address of the partition 48 data section end in RAM */
    . = ALIGN(8);
    partition48DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 48 */
  .partition48Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 48 bss section start in RAM */
    . = ALIGN(8);
    partition48BssStart = .;

    *(.partition48Bss)
    *(.partition48Bss.*)

    /* address of the partition 48 bss section end in RAM */
    . = ALIGN(8);
    partition48BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition48UssStart = .;

    *(.partition48Uss)
    *(.partition48Uss.*)

    partition48UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_48_ALIGNMENT);

    /* address of the partition 48 sections end in RAM */
    partition48End = .;

  } > ram

  /******************************************************************/
  /* Partition 49                                                   */
  /******************************************************************/

  /* address in the flash of the partition 49 data to copy it to RAM */
  pharosPartition49DataAddr = LOADADDR(.partition49);

  /* data of partition */
  .partition49 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition49DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_49_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition49Start = ABSOLUTE(.);

    *(.partition49)
    *(.partition49.*)

    /* address of the partition 49 data section end in RAM */
    . = ALIGN(8);
    partition49DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 49 */
  .partition49Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 49 bss section start in RAM */
    . = ALIGN(8);
    partition49BssStart = .;

    *(.partition49Bss)
    *(.partition49Bss.*)

    /* address of the partition 49 bss section end in RAM */
    . = ALIGN(8);
    partition49BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition49UssStart = .;

    *(.partition49Uss)
    *(.partition49Uss.*)

    partition49UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_49_ALIGNMENT);

    /* address of the partition 49 sections end in RAM */
    partition49End = .;

  } > ram

  /******************************************************************/
  /* Partition 50                                                   */
  /******************************************************************/

  /* address in the flash of the partition 50 data to copy it to RAM */
  pharosPartition50DataAddr = LOADADDR(.partition50);

  /* data of partition */
  .partition50 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition50DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_50_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition50Start = ABSOLUTE(.);

    *(.partition50)
    *(.partition50.*)

    /* address of the partition 50 data section end in RAM */
    . = ALIGN(8);
    partition50DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 50 */
  .partition50Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 50 bss section start in RAM */
    . = ALIGN(8);
    partition50BssStart = .;

    *(.partition50Bss)
    *(.partition50Bss.*)

    /* address of the partition 50 bss section end in RAM */
    . = ALIGN(8);
    partition50BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition50UssStart = .;

    *(.partition50Uss)
    *(.partition50Uss.*)

    partition50UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_50_ALIGNMENT);

    /* address of the partition 50 sections end in RAM */
    partition50End = .;

  } > ram

  /******************************************************************/
  /* Partition 51                                                   */
  /******************************************************************/

  /* address in the flash of the partition 51 data to copy it to RAM */
  pharosPartition51DataAddr = LOADADDR(.partition51);

  /* data of partition */
  .partition51 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition51DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_51_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition51Start = ABSOLUTE(.);

    *(.partition51)
    *(.partition51.*)

    /* address of the partition 51 data section end in RAM */
    . = ALIGN(8);
    partition51DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 51 */
  .partition51Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 51 bss section start in RAM */
    . = ALIGN(8);
    partition51BssStart = .;

    *(.partition51Bss)
    *(.partition51Bss.*)

    /* address of the partition 51 bss section end in RAM */
    . = ALIGN(8);
    partition51BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition51UssStart = .;

    *(.partition51Uss)
    *(.partition51Uss.*)

    partition51UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_51_ALIGNMENT);

    /* address of the partition 51 sections end in RAM */
    partition51End = .;

  } > ram

  /******************************************************************/
  /* Partition 52                                                   */
  /******************************************************************/

  /* address in the flash of the partition 52 data to copy it to RAM */
  pharosPartition52DataAddr = LOADADDR(.partition52);

  /* data of partition */
  .partition52 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition52DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_52_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition52Start = ABSOLUTE(.);

    *(.partition52)
    *(.partition52.*)

    /* address of the partition 52 data section end in RAM */
    . = ALIGN(8);
    partition52DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 52 */
  .partition52Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 52 bss section start in RAM */
    . = ALIGN(8);
    partition52BssStart = .;

    *(.partition52Bss)
    *(.partition52Bss.*)

    /* address of the partition 52 bss section end in RAM */
    . = ALIGN(8);
    partition52BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition52UssStart = .;

    *(.partition52Uss)
    *(.partition52Uss.*)

    partition52UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_52_ALIGNMENT);

    /* address of the partition 52 sections end in RAM */
    partition52End = .;

  } > ram

  /******************************************************************/
  /* Partition 53                                                   */
  /******************************************************************/

  /* address in the flash of the partition 53 data to copy it to RAM */
  pharosPartition53DataAddr = LOADADDR(.partition53);

  /* data of partition */
  .partition53 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition53DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_53_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition53Start = ABSOLUTE(.);

    *(.partition53)
    *(.partition53.*)

    /* address of the partition 53 data section end in RAM */
    . = ALIGN(8);
    partition53DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 53 */
  .partition53Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 53 bss section start in RAM */
    . = ALIGN(8);
    partition53BssStart = .;

    *(.partition53Bss)
    *(.partition53Bss.*)

    /* address of the partition 53 bss section end in RAM */
    . = ALIGN(8);
    partition53BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition53UssStart = .;

    *(.partition53Uss)
    *(.partition53Uss.*)

    partition53UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_53_ALIGNMENT);

    /* address of the partition 53 sections end in RAM */
    partition53End = .;

  } > ram

  /******************************************************************/
  /* Partition 54                                                   */
  /******************************************************************/

  /* address in the flash of the partition 54 data to copy it to RAM */
  pharosPartition54DataAddr = LOADADDR(.partition54);

  /* data of partition */
  .partition54 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition54DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_54_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition54Start = ABSOLUTE(.);

    *(.partition54)
    *(.partition54.*)

    /* address of the partition 54 data section end in RAM */
    . = ALIGN(8);
    partition54DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 54 */
  .partition54Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 54 bss section start in RAM */
    . = ALIGN(8);
    partition54BssStart = .;

    *(.partition54Bss)
    *(.partition54Bss.*)

    /* address of the partition 54 bss section end in RAM */
    . = ALIGN(8);
    partition54BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition54UssStart = .;

    *(.partition54Uss)
    *(.partition54Uss.*)

    partition54UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_54_ALIGNMENT);

    /* address of the partition 54 sections end in RAM */
    partition54End = .;

  } > ram

  /******************************************************************/
  /* Partition 55                                                   */
  /******************************************************************/

  /* address in the flash of the partition 55 data to copy it to RAM */
  pharosPartition55DataAddr = LOADADDR(.partition55);

  /* data of partition */
  .partition55 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition55DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_55_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition55Start = ABSOLUTE(.);

    *(.partition55)
    *(.partition55.*)

    /* address of the partition 55 data section end in RAM */
    . = ALIGN(8);
    partition55DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 55 */
  .partition55Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 55 bss section start in RAM */
    . = ALIGN(8);
    partition55BssStart = .;

    *(.partition55Bss)
    *(.partition55Bss.*)

    /* address of the partition 55 bss section end in RAM */
    . = ALIGN(8);
    partition55BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition55UssStart = .;

    *(.partition55Uss)
    *(.partition55Uss.*)

    partition55UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_55_ALIGNMENT);

    /* address of the partition 55 sections end in RAM */
    partition55End = .;

  } > ram

  /******************************************************************/
  /* Partition 56                                                   */
  /******************************************************************/

  /* address in the flash of the partition 56 data to copy it to RAM */
  pharosPartition56DataAddr = LOADADDR(.partition56);

  /* data of partition */
  .partition56 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition56DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_56_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition56Start = ABSOLUTE(.);

    *(.partition56)
    *(.partition56.*)

    /* address of the partition 56 data section end in RAM */
    . = ALIGN(8);
    partition56DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 56 */
  .partition56Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 56 bss section start in RAM */
    . = ALIGN(8);
    partition56BssStart = .;

    *(.partition56Bss)
    *(.partition56Bss.*)

    /* address of the partition 56 bss section end in RAM */
    . = ALIGN(8);
    partition56BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition56UssStart = .;

    *(.partition56Uss)
    *(.partition56Uss.*)

    partition56UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_56_ALIGNMENT);

    /* address of the partition 56 sections end in RAM */
    partition56End = .;

  } > ram

  /******************************************************************/
  /* Partition 57                                                   */
  /******************************************************************/

  /* address in the flash of the partition 57 data to copy it to RAM */
  pharosPartition57DataAddr = LOADADDR(.partition57);

  /* data of partition */
  .partition57 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition57DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_57_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition57Start = ABSOLUTE(.);

    *(.partition57)
    *(.partition57.*)

    /* address of the partition 57 data section end in RAM */
    . = ALIGN(8);
    partition57DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 57 */
  .partition57Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 57 bss section start in RAM */
    . = ALIGN(8);
    partition57BssStart = .;

    *(.partition57Bss)
    *(.partition57Bss.*)

    /* address of the partition 57 bss section end in RAM */
    . = ALIGN(8);
    partition57BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition57UssStart = .;

    *(.partition57Uss)
    *(.partition57Uss.*)

    partition57UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_57_ALIGNMENT);

    /* address of the partition 57 sections end in RAM */
    partition57End = .;

  } > ram

  /******************************************************************/
  /* Partition 58                                                   */
  /******************************************************************/

  /* address in the flash of the partition 58 data to copy it to RAM */
  pharosPartition58DataAddr = LOADADDR(.partition58);

  /* data of partition */
  .partition58 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition58DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_58_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition58Start = ABSOLUTE(.);

    *(.partition58)
    *(.partition58.*)

    /* address of the partition 58 data section end in RAM */
    . = ALIGN(8);
    partition58DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 58 */
  .partition58Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 58 bss section start in RAM */
    . = ALIGN(8);
    partition58BssStart = .;

    *(.partition58Bss)
    *(.partition58Bss.*)

    /* address of the partition 58 bss section end in RAM */
    . = ALIGN(8);
    partition58BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition58UssStart = .;

    *(.partition58Uss)
    *(.partition58Uss.*)

    partition58UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_58_ALIGNMENT);

    /* address of the partition 58 sections end in RAM */
    partition58End = .;

  } > ram

  /******************************************************************/
  /* Partition 59                                                   */
  /******************************************************************/

  /* address in the flash of the partition 59 data to copy it to RAM */
  pharosPartition59DataAddr = LOADADDR(.partition59);

  /* data of partition */
  .partition59 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition59DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_59_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition59Start = ABSOLUTE(.);

    *(.partition59)
    *(.partition59.*)

    /* address of the partition 59 data section end in RAM */
    . = ALIGN(8);
    partition59DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 59 */
  .partition59Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 59 bss section start in RAM */
    . = ALIGN(8);
    partition59BssStart = .;

    *(.partition59Bss)
    *(.partition59Bss.*)

    /* address of the partition 59 bss section end in RAM */
    . = ALIGN(8);
    partition59BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition59UssStart = .;

    *(.partition59Uss)
    *(.partition59Uss.*)

    partition59UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_59_ALIGNMENT);

    /* address of the partition 59 sections end in RAM */
    partition59End = .;

  } > ram

  /******************************************************************/
  /* Partition 60                                                   */
  /******************************************************************/

  /* address in the flash of the partition 60 data to copy it to RAM */
  pharosPartition60DataAddr = LOADADDR(.partition60);

  /* data of partition */
  .partition60 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition60DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_60_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition60Start = ABSOLUTE(.);

    *(.partition60)
    *(.partition60.*)

    /* address of the partition 60 data section end in RAM */
    . = ALIGN(8);
    partition60DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 60 */
  .partition60Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 60 bss section start in RAM */
    . = ALIGN(8);
    partition60BssStart = .;

    *(.partition60Bss)
    *(.partition60Bss.*)

    /* address of the partition 60 bss section end in RAM */
    . = ALIGN(8);
    partition60BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition60UssStart = .;

    *(.partition60Uss)
    *(.partition60Uss.*)

    partition60UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_60_ALIGNMENT);

    /* address of the partition 60 sections end in RAM */
    partition60End = .;

  } > ram

  /******************************************************************/
  /* Partition 61                                                   */
  /******************************************************************/

  /* address in the flash of the partition 61 data to copy it to RAM */
  pharosPartition61DataAddr = LOADADDR(.partition61);

  /* data of partition */
  .partition61 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition61DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_61_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition61Start = ABSOLUTE(.);

    *(.partition61)
    *(.partition61.*)

    /* address of the partition 61 data section end in RAM */
    . = ALIGN(8);
    partition61DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 61 */
  .partition61Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 61 bss section start in RAM */
    . = ALIGN(8);
    partition61BssStart = .;

    *(.partition61Bss)
    *(.partition61Bss.*)

    /* address of the partition 61 bss section end in RAM */
    . = ALIGN(8);
    partition61BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition61UssStart = .;

    *(.partition61Uss)
    *(.partition61Uss.*)

    partition61UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_61_ALIGNMENT);

    /* address of the partition 61 sections end in RAM */
    partition61End = .;

  } > ram

  /******************************************************************/
  /* Partition 62                                                   */
  /******************************************************************/

  /* address in the flash of the partition 62 data to copy it to RAM */
  pharosPartition62DataAddr = LOADADDR(.partition62);

  /* data of partition */
  .partition62 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition62DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_62_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition62Start = ABSOLUTE(.);

    *(.partition62)
    *(.partition62.*)

    /* address of the partition 62 data section end in RAM */
    . = ALIGN(8);
    partition62DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 62 */
  .partition62Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 62 bss section start in RAM */
    . = ALIGN(8);
    partition62BssStart = .;

    *(.partition62Bss)
    *(.partition62Bss.*)

    /* address of the partition 62 bss section end in RAM */
    . = ALIGN(8);
    partition62BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition62UssStart = .;

    *(.partition62Uss)
    *(.partition62Uss.*)

    partition62UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_62_ALIGNMENT);

    /* address of the partition 62 sections end in RAM */
    partition62End = .;

  } > ram

  /******************************************************************/
  /* Partition 63                                                   */
  /******************************************************************/

  /* address in the flash of the partition 63 data to copy it to RAM */
  pharosPartition63DataAddr = LOADADDR(.partition63);

  /* data of partition */
  .partition63 : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition data section start in RAM, so that it can be initialized from this address */
    partition63DataStart = ABSOLUTE(.); 

    . = ALIGN(PARTITION_63_ALIGNMENT);
    . = ALIGN(8);

    /* address where the MPU shall be initialized to (correctly aligned) */
    partition63Start = ABSOLUTE(.);

    *(.partition63)
    *(.partition63.*)

    /* address of the partition 63 data section end in RAM */
    . = ALIGN(8);
    partition63DataEnd = .;

  } > ram AT > flash = 0xff


  /* bss of partition 63 */
  .partition63Bss (NOLOAD) : ALIGN(8)
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 63 bss section start in RAM */
    . = ALIGN(8);
    partition63BssStart = .;

    *(.partition63Bss)
    *(.partition63Bss.*)

    /* address of the partition 63 bss section end in RAM */
    . = ALIGN(8);
    partition63BssEnd = .;

    /* place here the partition data that is NOT to be initialized at all */
    partition63UssStart = .;

    *(.partition63Uss)
    *(.partition63Uss.*)

    partition63UssEnd = .;

    /* make sure the next partition does not start before the MPU region of this one ends */
    . = ALIGN(PARTITION_63_ALIGNMENT);

    /* address of the partition 63 sections end in RAM */
    partition63End = .;

  } > ram


  /******************************************************************/
  /*                     End of partitions                          */
  /******************************************************************/

  /******************************************************************/
  /*             Start of Inter-partition calls area                */
  /******************************************************************/

  /* place this section last because it usually takes very little space and therefore has low alignment constraints */
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

  } > ram AT > flash = 0xff

}