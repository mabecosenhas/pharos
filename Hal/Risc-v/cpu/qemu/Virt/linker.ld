OUTPUT_ARCH("riscv")

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

PAGE_START_ALIGN_4K = 4K;
PAGE_START_ALIGN_2M = 2M;
PAGE_START_ALIGN_1G = 1M * 1K;

/* partitions start at 2M */
PARTITION_ALIGNMENT = PAGE_START_ALIGN_2M;

CHANNEL_BULK_SIZE = 512;

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

/* number of tables level 2 needed for all the the shared stack for one partition =ceil(sharedStackSize/2M) */
pharosNumberSharedStackMmuTables2PerPartition = pharosSharedStackEnd == pharosSharedStackStart ? 0 : 1 << LOG2CEIL(pharosSharedStackSize / 2M);

 
IP_CALL_ALIGNMENT = PARTITION_ALIGNMENT;


MEMORY
{
  /* sdram location */
  sdram         : ORIGIN = 0x80000000, LENGTH = 2 * 1024M 
}

pharosTextSize = pharosEndText - pharosStartText;

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

    KEEP(*(.textStart))
    KEEP(*(.textStart.*))

    . = ALIGN (0x4);
    *(.text)
    *(.text.*)

    /* read-only data is placed in text */
    . = ALIGN (0x10);
    pharosRodataStart = . ;
    *(.srodata)
    *(.srodata.*)
    *(.rodata)
    *(.rodata.*)
    *(.note.gnu.build-id)
    *(.note.gnu.build-id.*)
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
  
  pharosKernelDataLoadAddr = LOADADDR(.kernel);
  pharosKernelDataSize = pharosKernelDataLoadAddr == pharosKernelDataStart ? 0  : (pharosKernelDataEnd - pharosKernelDataStart) / 8;
  pharosKernelBssSize = (pharosKernelBssEnd - pharosKernelBssStart) / 8;


  /* Kernel data space to enable memory protection */
  .kernel : 
  {
    CREATE_OBJECT_SYMBOLS
    pharosKernelDataStart = .;

    . = ALIGN (PARTITION_ALIGNMENT);
    . = ALIGN(8);

    pharosKernelStart = .;
    *(.kernel)  
    *(.kernel.*)
  
    . = ALIGN(8);

    /* address in RAM where the kernel data ends */
    pharosKernelDataEnd = .;

  } > sdram AT > sdram = 0xff

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
 
    /* reserve space for the partition 0 MMU tables level 1 according to its memory area size */
    pharosPartition0MmuLevel1Start = .;
    . = . + ((pharosCpuPartition0Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition0MmuLevel1End = .;

    /* reserve space for the partition 1 MMU tables level 1 according to its memory area size */
    pharosPartition1MmuLevel1Start = .;
    . = . + ((pharosCpuPartition1Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition1MmuLevel1End = .;

    /* reserve space for the partition 2 MMU tables level 1 according to its memory area size */
    pharosPartition2MmuLevel1Start = .;
    . = . + ((pharosCpuPartition2Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition2MmuLevel1End = .;

    /* reserve space for the partition 3 MMU tables level 1 according to its memory area size */
    pharosPartition3MmuLevel1Start = .;
    . = . + ((pharosCpuPartition3Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition3MmuLevel1End = .;

    /* reserve space for the partition 4 MMU tables level 1 according to its memory area size */
    pharosPartition4MmuLevel1Start = .;
    . = . + ((pharosCpuPartition4Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition4MmuLevel1End = .;

    /* reserve space for the partition 5 MMU tables level 1 according to its memory area size */
    pharosPartition5MmuLevel1Start = .;
    . = . + ((pharosCpuPartition5Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition5MmuLevel1End = .;

    /* reserve space for the partition 6 MMU tables level 1 according to its memory area size */
    pharosPartition6MmuLevel1Start = .;
    . = . + ((pharosCpuPartition6Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition6MmuLevel1End = .;

    /* reserve space for the partition 7 MMU tables level 1 according to its memory area size */
    pharosPartition7MmuLevel1Start = .;
    . = . + ((pharosCpuPartition7Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition7MmuLevel1End = .;

    /* reserve space for the partition 8 MMU tables level 1 according to its memory area size */
    pharosPartition8MmuLevel1Start = .;
    . = . + ((pharosCpuPartition8Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition8MmuLevel1End = .;

    /* reserve space for the partition 9 MMU tables level 1 according to its memory area size */
    pharosPartition9MmuLevel1Start = .;
    . = . + ((pharosCpuPartition9Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition9MmuLevel1End = .;

    /* reserve space for the partition 10 MMU tables level 1 according to its memory area size */
    pharosPartition10MmuLevel1Start = .;
    . = . + ((pharosCpuPartition10Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition10MmuLevel1End = .;

    /* reserve space for the partition 11 MMU tables level 1 according to its memory area size */
    pharosPartition11MmuLevel1Start = .;
    . = . + ((pharosCpuPartition11Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition11MmuLevel1End = .;

    /* reserve space for the partition 12 MMU tables level 1 according to its memory area size */
    pharosPartition12MmuLevel1Start = .;
    . = . + ((pharosCpuPartition12Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition12MmuLevel1End = .;

    /* reserve space for the partition 13 MMU tables level 1 according to its memory area size */
    pharosPartition13MmuLevel1Start = .;
    . = . + ((pharosCpuPartition13Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition13MmuLevel1End = .;

    /* reserve space for the partition 14 MMU tables level 1 according to its memory area size */
    pharosPartition14MmuLevel1Start = .;
    . = . + ((pharosCpuPartition14Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition14MmuLevel1End = .;

    /* reserve space for the partition 15 MMU tables level 1 according to its memory area size */
    pharosPartition15MmuLevel1Start = .;
    . = . + ((pharosCpuPartition15Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition15MmuLevel1End = .;

    /* reserve space for the partition 16 MMU tables level 1 according to its memory area size */
    pharosPartition16MmuLevel1Start = .;
    . = . + ((pharosCpuPartition16Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition16MmuLevel1End = .;

    /* reserve space for the partition 17 MMU tables level 1 according to its memory area size */
    pharosPartition17MmuLevel1Start = .;
    . = . + ((pharosCpuPartition17Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition17MmuLevel1End = .;

    /* reserve space for the partition 18 MMU tables level 1 according to its memory area size */
    pharosPartition18MmuLevel1Start = .;
    . = . + ((pharosCpuPartition18Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition18MmuLevel1End = .;

    /* reserve space for the partition 19 MMU tables level 1 according to its memory area size */
    pharosPartition19MmuLevel1Start = .;
    . = . + ((pharosCpuPartition19Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition19MmuLevel1End = .;

    /* reserve space for the partition 20 MMU tables level 1 according to its memory area size */
    pharosPartition20MmuLevel1Start = .;
    . = . + ((pharosCpuPartition20Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition20MmuLevel1End = .;

    /* reserve space for the partition 21 MMU tables level 1 according to its memory area size */
    pharosPartition21MmuLevel1Start = .;
    . = . + ((pharosCpuPartition21Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition21MmuLevel1End = .;

    /* reserve space for the partition 22 MMU tables level 1 according to its memory area size */
    pharosPartition22MmuLevel1Start = .;
    . = . + ((pharosCpuPartition22Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition22MmuLevel1End = .;

    /* reserve space for the partition 23 MMU tables level 1 according to its memory area size */
    pharosPartition23MmuLevel1Start = .;
    . = . + ((pharosCpuPartition23Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition23MmuLevel1End = .;

    /* reserve space for the partition 24 MMU tables level 1 according to its memory area size */
    pharosPartition24MmuLevel1Start = .;
    . = . + ((pharosCpuPartition24Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition24MmuLevel1End = .;

    /* reserve space for the partition 25 MMU tables level 1 according to its memory area size */
    pharosPartition25MmuLevel1Start = .;
    . = . + ((pharosCpuPartition25Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition25MmuLevel1End = .;

    /* reserve space for the partition 26 MMU tables level 1 according to its memory area size */
    pharosPartition26MmuLevel1Start = .;
    . = . + ((pharosCpuPartition26Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition26MmuLevel1End = .;

    /* reserve space for the partition 27 MMU tables level 1 according to its memory area size */
    pharosPartition27MmuLevel1Start = .;
    . = . + ((pharosCpuPartition27Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition27MmuLevel1End = .;

    /* reserve space for the partition 28 MMU tables level 1 according to its memory area size */
    pharosPartition28MmuLevel1Start = .;
    . = . + ((pharosCpuPartition28Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition28MmuLevel1End = .;

    /* reserve space for the partition 29 MMU tables level 1 according to its memory area size */
    pharosPartition29MmuLevel1Start = .;
    . = . + ((pharosCpuPartition29Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition29MmuLevel1End = .;

    /* reserve space for the partition 30 MMU tables level 1 according to its memory area size */
    pharosPartition30MmuLevel1Start = .;
    . = . + ((pharosCpuPartition30Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition30MmuLevel1End = .;

    /* reserve space for the partition 31 MMU tables level 1 according to its memory area size */
    pharosPartition31MmuLevel1Start = .;
    . = . + ((pharosCpuPartition31Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition31MmuLevel1End = .;

    /* reserve space for the partition 32 MMU tables level 1 according to its memory area size */
    pharosPartition32MmuLevel1Start = .;
    . = . + ((pharosCpuPartition32Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition32MmuLevel1End = .;

    /* reserve space for the partition 33 MMU tables level 1 according to its memory area size */
    pharosPartition33MmuLevel1Start = .;
    . = . + ((pharosCpuPartition33Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition33MmuLevel1End = .;

    /* reserve space for the partition 34 MMU tables level 1 according to its memory area size */
    pharosPartition34MmuLevel1Start = .;
    . = . + ((pharosCpuPartition34Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition34MmuLevel1End = .;

    /* reserve space for the partition 35 MMU tables level 1 according to its memory area size */
    pharosPartition35MmuLevel1Start = .;
    . = . + ((pharosCpuPartition35Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition35MmuLevel1End = .;

    /* reserve space for the partition 36 MMU tables level 1 according to its memory area size */
    pharosPartition36MmuLevel1Start = .;
    . = . + ((pharosCpuPartition36Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition36MmuLevel1End = .;

    /* reserve space for the partition 37 MMU tables level 1 according to its memory area size */
    pharosPartition37MmuLevel1Start = .;
    . = . + ((pharosCpuPartition37Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition37MmuLevel1End = .;

    /* reserve space for the partition 38 MMU tables level 1 according to its memory area size */
    pharosPartition38MmuLevel1Start = .;
    . = . + ((pharosCpuPartition38Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition38MmuLevel1End = .;

    /* reserve space for the partition 39 MMU tables level 1 according to its memory area size */
    pharosPartition39MmuLevel1Start = .;
    . = . + ((pharosCpuPartition39Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition39MmuLevel1End = .;

    /* reserve space for the partition 40 MMU tables level 1 according to its memory area size */
    pharosPartition40MmuLevel1Start = .;
    . = . + ((pharosCpuPartition40Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition40MmuLevel1End = .;

    /* reserve space for the partition 41 MMU tables level 1 according to its memory area size */
    pharosPartition41MmuLevel1Start = .;
    . = . + ((pharosCpuPartition41Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition41MmuLevel1End = .;

    /* reserve space for the partition 42 MMU tables level 1 according to its memory area size */
    pharosPartition42MmuLevel1Start = .;
    . = . + ((pharosCpuPartition42Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition42MmuLevel1End = .;

    /* reserve space for the partition 43 MMU tables level 1 according to its memory area size */
    pharosPartition43MmuLevel1Start = .;
    . = . + ((pharosCpuPartition43Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition43MmuLevel1End = .;

    /* reserve space for the partition 44 MMU tables level 1 according to its memory area size */
    pharosPartition44MmuLevel1Start = .;
    . = . + ((pharosCpuPartition44Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition44MmuLevel1End = .;

    /* reserve space for the partition 45 MMU tables level 1 according to its memory area size */
    pharosPartition45MmuLevel1Start = .;
    . = . + ((pharosCpuPartition45Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition45MmuLevel1End = .;

    /* reserve space for the partition 46 MMU tables level 1 according to its memory area size */
    pharosPartition46MmuLevel1Start = .;
    . = . + ((pharosCpuPartition46Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition46MmuLevel1End = .;

    /* reserve space for the partition 47 MMU tables level 1 according to its memory area size */
    pharosPartition47MmuLevel1Start = .;
    . = . + ((pharosCpuPartition47Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition47MmuLevel1End = .;

    /* reserve space for the partition 48 MMU tables level 1 according to its memory area size */
    pharosPartition48MmuLevel1Start = .;
    . = . + ((pharosCpuPartition48Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition48MmuLevel1End = .;

    /* reserve space for the partition 49 MMU tables level 1 according to its memory area size */
    pharosPartition49MmuLevel1Start = .;
    . = . + ((pharosCpuPartition49Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition49MmuLevel1End = .;

    /* reserve space for the partition 50 MMU tables level 1 according to its memory area size */
    pharosPartition50MmuLevel1Start = .;
    . = . + ((pharosCpuPartition50Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition50MmuLevel1End = .;

    /* reserve space for the partition 51 MMU tables level 1 according to its memory area size */
    pharosPartition51MmuLevel1Start = .;
    . = . + ((pharosCpuPartition51Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition51MmuLevel1End = .;

    /* reserve space for the partition 52 MMU tables level 1 according to its memory area size */
    pharosPartition52MmuLevel1Start = .;
    . = . + ((pharosCpuPartition52Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition52MmuLevel1End = .;

    /* reserve space for the partition 53 MMU tables level 1 according to its memory area size */
    pharosPartition53MmuLevel1Start = .;
    . = . + ((pharosCpuPartition53Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition53MmuLevel1End = .;

    /* reserve space for the partition 54 MMU tables level 1 according to its memory area size */
    pharosPartition54MmuLevel1Start = .;
    . = . + ((pharosCpuPartition54Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition54MmuLevel1End = .;

    /* reserve space for the partition 55 MMU tables level 1 according to its memory area size */
    pharosPartition55MmuLevel1Start = .;
    . = . + ((pharosCpuPartition55Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition55MmuLevel1End = .;

    /* reserve space for the partition 56 MMU tables level 1 according to its memory area size */
    pharosPartition56MmuLevel1Start = .;
    . = . + ((pharosCpuPartition56Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition56MmuLevel1End = .;

    /* reserve space for the partition 57 MMU tables level 1 according to its memory area size */
    pharosPartition57MmuLevel1Start = .;
    . = . + ((pharosCpuPartition57Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition57MmuLevel1End = .;

    /* reserve space for the partition 58 MMU tables level 1 according to its memory area size */
    pharosPartition58MmuLevel1Start = .;
    . = . + ((pharosCpuPartition58Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition58MmuLevel1End = .;

    /* reserve space for the partition 59 MMU tables level 1 according to its memory area size */
    pharosPartition59MmuLevel1Start = .;
    . = . + ((pharosCpuPartition59Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition59MmuLevel1End = .;

    /* reserve space for the partition 60 MMU tables level 1 according to its memory area size */
    pharosPartition60MmuLevel1Start = .;
    . = . + ((pharosCpuPartition60Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition60MmuLevel1End = .;

    /* reserve space for the partition 61 MMU tables level 1 according to its memory area size */
    pharosPartition61MmuLevel1Start = .;
    . = . + ((pharosCpuPartition61Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition61MmuLevel1End = .;

    /* reserve space for the partition 62 MMU tables level 1 according to its memory area size */
    pharosPartition62MmuLevel1Start = .;
    . = . + ((pharosCpuPartition62Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition62MmuLevel1End = .;

    /* reserve space for the partition 63 MMU tables level 1 according to its memory area size */
    pharosPartition63MmuLevel1Start = .;
    . = . + ((pharosCpuPartition63Size + pharosTextSize) / 2M + 1) * 4K;
    pharosPartition63MmuLevel1End = .;

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

    /* make sure we are aligned here to 4K (even though PARTITION_ALIGNMENT should be higher than 4K so this is redundant) */
    . = ALIGN(4K);

    /* start of the MMU tables level 2 for the shared stack */
    pharosSharedStackTables2Start = .;

    /* here is the space for the level 2 tables to be used for all partitions to map the shared stack area (each table takes 4K) */
    . = . + (pharosNumberSharedStackMmuTables2PerPartition * 4K) * PARTITION_MAXIMUM_NUMBER;

    pharosSharedStackTables2End = .;   

    . = ALIGN (PARTITION_ALIGNMENT);

  } > sdram
 
  /******************************************************************/
  /*                        End of kernel space                     */
  /******************************************************************/

  
  /******************************************************************/
  /*                     Start of Shared data area                  */
  /******************************************************************/

  pharosSharedDataLoadAddr = LOADADDR(.shared);
  pharosSharedDataSize = (pharosSharedDataEnd - pharosSharedDataStart) / 8;
  pharosSharedBssSize = (pharosSharedBssEnd - pharosSharedBssStart) / 8;


  /* Shared section data space */
  .shared : 
  {
    CREATE_OBJECT_SYMBOLS

    pharosSharedDataStart = .;

    . = ALIGN (PARTITION_ALIGNMENT);

    pharosSharedStart = .;

    *(.shared)
    *(.shared.*)

    . = ALIGN(8);
    pharosSharedDataEnd = .;

  } > sdram AT > sdram = 0xff

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

    . = ALIGN (PARTITION_ALIGNMENT);
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
  pharosPartition0DataAddr = partition0DataStart;

  /* data of partition */
  .partition0 :
  {
    CREATE_OBJECT_SYMBOLS

    /* address of the partition 0 data section start in RAM */
    partition0DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition0Start = .;

    *(.partition0)
    *(.partition0.*)

    /* by default, the data section will go to partition 0 */
    *(.data.*)
    *(.sdata.*)

    /* address of the partition 0 data section end in RAM */
    . = ALIGN(8);
    partition0DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 1 data section start in RAM */
    partition1DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition1Start = .;

    *(.partition1)
    *(.partition1.*)

    /* address of the partition 1 data section end in RAM */
    . = ALIGN(8);
    partition1DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 2 data section start in RAM */
    partition2DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition2Start = .;

    *(.partition2)
    *(.partition2.*)

    /* address of the partition 2 data section end in RAM */
    . = ALIGN(8);
    partition2DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 3 data section start in RAM */
    partition3DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition3Start = .;

    *(.partition3)
    *(.partition3.*)

    /* address of the partition 3 data section end in RAM */
    . = ALIGN(8);
    partition3DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 4 data section start in RAM */
    partition4DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition4Start = .;

    *(.partition4)
    *(.partition4.*)

    /* address of the partition 4 data section end in RAM */
    . = ALIGN(8);
    partition4DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 5 data section start in RAM */
    partition5DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition5Start = .;


    *(.partition5)
    *(.partition5.*)

    /* address of the partition 5 data section end in RAM */
    . = ALIGN(8);
    partition5DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 6 data section start in RAM */
    partition6DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition6Start = .;

    *(.partition6)
    *(.partition6.*)

    /* address of the partition 6 data section end in RAM */
    . = ALIGN(8);
    partition6DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 7 data section start in RAM */
    partition7DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition7Start = .;

    *(.partition7)
    *(.partition7.*)

    /* address of the partition 7 data section end in RAM */
    . = ALIGN(8);
    partition7DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 8 data section start in RAM */
    partition8DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition8Start = .;

    *(.partition8)
    *(.partition8.*)

    /* address of the partition 8 data section end in RAM */
    . = ALIGN(8);
    partition8DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 9 data section start in RAM */
    partition9DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition9Start = .;

    *(.partition9)
    *(.partition9.*)

    /* address of the partition 9 data section end in RAM */
    . = ALIGN(8);
    partition9DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 10 data section start in RAM */
    partition10DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition10Start = .;

    *(.partition10)
    *(.partition10.*)

    /* address of the partition 10 data section end in RAM */
    . = ALIGN(8);
    partition10DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 11 data section start in RAM */
    partition11DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition11Start = .;

    *(.partition11)
    *(.partition11.*)

    /* address of the partition 11 data section end in RAM */
    . = ALIGN(8);
    partition11DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 12 data section start in RAM */
    partition12DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition12Start = .;

    *(.partition12)
    *(.partition12.*)

    /* address of the partition 12 data section end in RAM */
    . = ALIGN(8);
    partition12DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 13 data section start in RAM */
    partition13DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition13Start = .;

    *(.partition13)
    *(.partition13.*)

    /* address of the partition 13 data section end in RAM */
    . = ALIGN(8);
    partition13DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 14 data section start in RAM */
    partition14DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition14Start = .;

    *(.partition14)
    *(.partition14.*)

    /* address of the partition 14 data section end in RAM */
    . = ALIGN(8);
    partition14DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 15 data section start in RAM */
    partition15DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition15Start = .;

    *(.partition15)
    *(.partition15.*)

    /* address of the partition 15 data section end in RAM */
    . = ALIGN(8);
    partition15DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 16 data section start in RAM */
    partition16DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition16Start = .;

    *(.partition16)
    *(.partition16.*)

    /* address of the partition 16 data section end in RAM */
    . = ALIGN(8);
    partition16DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 17 data section start in RAM */
    partition17DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition17Start = .;

    *(.partition17)
    *(.partition17.*)

    /* address of the partition 17 data section end in RAM */
    . = ALIGN(8);
    partition17DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 18 data section start in RAM */
    partition18DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition18Start = .;

    *(.partition18)
    *(.partition18.*)

    /* address of the partition 18 data section end in RAM */
    . = ALIGN(8);
    partition18DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 19 data section start in RAM */
    partition19DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition19Start = .;

    *(.partition19)
    *(.partition19.*)

    /* address of the partition 19 data section end in RAM */
    . = ALIGN(8);
    partition19DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 20 data section start in RAM */
    partition20DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition20Start = .;

    *(.partition20)
    *(.partition20.*)

    /* address of the partition 20 data section end in RAM */
    . = ALIGN(8);
    partition20DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 21 data section start in RAM */
    partition21DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition21Start = .;

    *(.partition21)
    *(.partition21.*)

    /* address of the partition 21 data section end in RAM */
    . = ALIGN(8);
    partition21DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 22 data section start in RAM */
    partition22DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition22Start = .;

    *(.partition22)
    *(.partition22.*)

    /* address of the partition 22 data section end in RAM */
    . = ALIGN(8);
    partition22DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 23 data section start in RAM */
    partition23DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition23Start = .;

    *(.partition23)
    *(.partition23.*)

    /* address of the partition 23 data section end in RAM */
    . = ALIGN(8);
    partition23DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 24 data section start in RAM */
    partition24DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition24Start = .;

    *(.partition24)
    *(.partition24.*)

    /* address of the partition 24 data section end in RAM */
    . = ALIGN(8);
    partition24DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 25 data section start in RAM */
    partition25DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition25Start = .;

    *(.partition25)
    *(.partition25.*)

    /* address of the partition 25 data section end in RAM */
    . = ALIGN(8);
    partition25DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 26 data section start in RAM */
    partition26DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition26Start = .;

    *(.partition26)
    *(.partition26.*)

    /* address of the partition 26 data section end in RAM */
    . = ALIGN(8);
    partition26DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 27 data section start in RAM */
    partition27DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition27Start = .;

    *(.partition27)
    *(.partition27.*)

    /* address of the partition 27 data section end in RAM */
    . = ALIGN(8);
    partition27DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 28 data section start in RAM */
    partition28DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition28Start = .;

    *(.partition28)
    *(.partition28.*)

    /* address of the partition 28 data section end in RAM */
    . = ALIGN(8);
    partition28DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 29 data section start in RAM */
    partition29DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition29Start = .;

    *(.partition29)
    *(.partition29.*)

    /* address of the partition 29 data section end in RAM */
    . = ALIGN(8);
    partition29DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 30 data section start in RAM */
    partition30DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition30Start = .;

    *(.partition30)
    *(.partition30.*)

    /* address of the partition 30 data section end in RAM */
    . = ALIGN(8);
    partition30DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 31 data section start in RAM */
    partition31DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition31Start = .;

    *(.partition31)
    *(.partition31.*)

    /* address of the partition 31 data section end in RAM */
    . = ALIGN(8);
    partition31DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 32 data section start in RAM */
    partition32DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition32Start = .;

    *(.partition32)
    *(.partition32.*)

    /* address of the partition 32 data section end in RAM */
    . = ALIGN(8);
    partition32DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 33 data section start in RAM */
    partition33DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition33Start = .;

    *(.partition33)
    *(.partition33.*)

    /* address of the partition 33 data section end in RAM */
    . = ALIGN(8);
    partition33DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 34 data section start in RAM */
    partition34DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition34Start = .;

    *(.partition34)
    *(.partition34.*)

    /* address of the partition 34 data section end in RAM */
    . = ALIGN(8);
    partition34DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 35 data section start in RAM */
    partition35DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition35Start = .;

    *(.partition35)
    *(.partition35.*)

    /* address of the partition 35 data section end in RAM */
    . = ALIGN(8);
    partition35DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 36 data section start in RAM */
    partition36DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition36Start = .;

    *(.partition36)
    *(.partition36.*)

    /* address of the partition 36 data section end in RAM */
    . = ALIGN(8);
    partition36DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 37 data section start in RAM */
    partition37DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition37Start = .;

    *(.partition37)
    *(.partition37.*)

    /* address of the partition 37 data section end in RAM */
    . = ALIGN(8);
    partition37DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 38 data section start in RAM */
    partition38DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition38Start = .;

    *(.partition38)
    *(.partition38.*)

    /* address of the partition 38 data section end in RAM */
    . = ALIGN(8);
    partition38DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 39 data section start in RAM */
    partition39DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition39Start = .;

    *(.partition39)
    *(.partition39.*)

    /* address of the partition 39 data section end in RAM */
    . = ALIGN(8);
    partition39DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 40 data section start in RAM */
    partition40DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition40Start = .;

    *(.partition40)
    *(.partition40.*)

    /* address of the partition 40 data section end in RAM */
    . = ALIGN(8);
    partition40DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 41 data section start in RAM */
    partition41DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition41Start = .;

    *(.partition41)
    *(.partition41.*)

    /* address of the partition 41 data section end in RAM */
    . = ALIGN(8);
    partition41DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 42 data section start in RAM */
    partition42DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition42Start = .;

    *(.partition42)
    *(.partition42.*)

    /* address of the partition 42 data section end in RAM */
    . = ALIGN(8);
    partition42DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 43 data section start in RAM */
    partition43DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition43Start = .;

    *(.partition43)
    *(.partition43.*)

    /* address of the partition 43 data section end in RAM */
    . = ALIGN(8);
    partition43DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 44 data section start in RAM */
    partition44DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition44Start = .;

    *(.partition44)
    *(.partition44.*)

    /* address of the partition 44 data section end in RAM */
    . = ALIGN(8);
    partition44DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 45 data section start in RAM */
    partition45DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition45Start = .;

    *(.partition45)
    *(.partition45.*)

    /* address of the partition 45 data section end in RAM */
    . = ALIGN(8);
    partition45DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 46 data section start in RAM */
    partition46DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition46Start = .;

    *(.partition46)
    *(.partition46.*)

    /* address of the partition 46 data section end in RAM */
    . = ALIGN(8);
    partition46DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 47 data section start in RAM */
    partition47DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition47Start = .;

    *(.partition47)
    *(.partition47.*)

    /* address of the partition 47 data section end in RAM */
    . = ALIGN(8);
    partition47DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 48 data section start in RAM */
    partition48DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition48Start = .;

    *(.partition48)
    *(.partition48.*)

    /* address of the partition 48 data section end in RAM */
    . = ALIGN(8);
    partition48DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 49 data section start in RAM */
    partition49DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition49Start = .;

    *(.partition49)
    *(.partition49.*)

    /* address of the partition 49 data section end in RAM */
    . = ALIGN(8);
    partition49DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 50 data section start in RAM */
    partition50DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition50Start = .;

    *(.partition50)
    *(.partition50.*)

    /* address of the partition 50 data section end in RAM */
    . = ALIGN(8);
    partition50DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 51 data section start in RAM */
    partition51DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition51Start = .;

    *(.partition51)
    *(.partition51.*)

    /* address of the partition 51 data section end in RAM */
    . = ALIGN(8);
    partition51DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 52 data section start in RAM */
    partition52DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition52Start = .;

    *(.partition52)
    *(.partition52.*)

    /* address of the partition 52 data section end in RAM */
    . = ALIGN(8);
    partition52DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 53 data section start in RAM */
    partition53DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition53Start = .;

    *(.partition53)
    *(.partition53.*)

    /* address of the partition 53 data section end in RAM */
    . = ALIGN(8);
    partition53DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 54 data section start in RAM */
    partition54DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition54Start = .;

    *(.partition54)
    *(.partition54.*)

    /* address of the partition 54 data section end in RAM */
    . = ALIGN(8);
    partition54DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 55 data section start in RAM */
    partition55DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition55Start = .;

    *(.partition55)
    *(.partition55.*)

    /* address of the partition 55 data section end in RAM */
    . = ALIGN(8);
    partition55DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 56 data section start in RAM */
    partition56DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition56Start = .;

    *(.partition56)
    *(.partition56.*)

    /* address of the partition 56 data section end in RAM */
    . = ALIGN(8);
    partition56DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 57 data section start in RAM */
    partition57DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition57Start = .;

    *(.partition57)
    *(.partition57.*)

    /* address of the partition 57 data section end in RAM */
    . = ALIGN(8);
    partition57DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 58 data section start in RAM */
    partition58DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition58Start = .;

    *(.partition58)
    *(.partition58.*)

    /* address of the partition 58 data section end in RAM */
    . = ALIGN(8);
    partition58DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 59 data section start in RAM */
    partition59DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition59Start = .;

    *(.partition59)
    *(.partition59.*)

    /* address of the partition 59 data section end in RAM */
    . = ALIGN(8);
    partition59DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 60 data section start in RAM */
    partition60DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition60Start = .;

    *(.partition60)
    *(.partition60.*)

    /* address of the partition 60 data section end in RAM */
    . = ALIGN(8);
    partition60DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 61 data section start in RAM */
    partition61DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition61Start = .;

    *(.partition61)
    *(.partition61.*)

    /* address of the partition 61 data section end in RAM */
    . = ALIGN(8);
    partition61DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 62 data section start in RAM */
    partition62DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition62Start = .;

    *(.partition62)
    *(.partition62.*)

    /* address of the partition 62 data section end in RAM */
    . = ALIGN(8);
    partition62DataEnd = .;

  } > sdram AT > sdram = 0xff

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

    /* address of the partition 63 data section start in RAM */
    partition63DataStart = .;

    . = ALIGN(PARTITION_ALIGNMENT);
    . = ALIGN(8);

    partition63Start = .;

    *(.partition63)
    *(.partition63.*)

    /* address of the partition 63 data section end in RAM */
    . = ALIGN(8);
    partition63DataEnd = .;

  } > sdram AT > sdram = 0xff

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


  /******************************************************************/
  /*                  Start of channel buffers area                 */
  /******************************************************************/

  /* pages of 1G */
  .pharosPage1G (NOLOAD) :
  {
    CREATE_OBJECT_SYMBOLS
    
    . = ALIGN (PAGE_START_ALIGN_1G);
    pharosPageStart1G = .;

    *(.pharosPage1G)
    
    . = ALIGN (PAGE_START_ALIGN_1G);
    pharosPageEnd1G = .;
  } > sdram

  /******************************************************************/
  /*                   End of channel buffers area                  */
  /******************************************************************/

}
