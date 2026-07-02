#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=$$PHAROS_ARM_R5_BIG_RANLIB
CC=$$PHAROS_ARM_R5_BIG_CC
CCC=g++
CXX=g++
FC=gfortran
AS=$$PHAROS_ARM_R5_BIG_CC
AR=$$PHAROS_ARM_R5_BIG_ARCHIVER

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=arm-v7-r5-tms570lc4357-LAUNCHXL2_570LC43-release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/assert/assert.o \
	${OBJECTDIR}/src/buffer/buffer.o \
	${OBJECTDIR}/src/bulk/bulk.o \
	${OBJECTDIR}/src/channel/channel.o \
	${OBJECTDIR}/src/channel/channelbulk.o \
	${OBJECTDIR}/src/channel/channelfilter.o \
	${OBJECTDIR}/src/channel/channelfilterexecute.o \
	${OBJECTDIR}/src/channel/channelreceive.o \
	${OBJECTDIR}/src/channel/channelsend.o \
	${OBJECTDIR}/src/clock/clock.o \
	${OBJECTDIR}/src/collection/doublelinkedsimplelist.o \
	${OBJECTDIR}/src/collection/linkedlist.o \
	${OBJECTDIR}/src/collection/simplelist.o \
	${OBJECTDIR}/src/collection/treemap.o \
	${OBJECTDIR}/src/core/core.o \
	${OBJECTDIR}/src/core/math.o \
	${OBJECTDIR}/src/core/multithreading.o \
	${OBJECTDIR}/src/core/priority.o \
	${OBJECTDIR}/src/core/syscall.o \
	${OBJECTDIR}/src/error/error.o \
	${OBJECTDIR}/src/event/event.o \
	${OBJECTDIR}/src/future/future.o \
	${OBJECTDIR}/src/future/futureQueue.o \
	${OBJECTDIR}/src/future/futureTable.o \
	${OBJECTDIR}/src/future/futurefree.o \
	${OBJECTDIR}/src/future/futureready.o \
	${OBJECTDIR}/src/interrupt/critical.o \
	${OBJECTDIR}/src/interrupt/interrupt.o \
	${OBJECTDIR}/src/ipcall/ipcall.o \
	${OBJECTDIR}/src/ipcall/ipcallexecute.o \
	${OBJECTDIR}/src/ipcall/ipcallinternal.o \
	${OBJECTDIR}/src/memory/malloc.o \
	${OBJECTDIR}/src/memory/protection.o \
	${OBJECTDIR}/src/object/object.o \
	${OBJECTDIR}/src/object/protection.o \
	${OBJECTDIR}/src/partition/partition.o \
	${OBJECTDIR}/src/partition/partitionrestart.o \
	${OBJECTDIR}/src/queue/hw/hwqueue.o \
	${OBJECTDIR}/src/queue/hw/hwqueuefilter.o \
	${OBJECTDIR}/src/queue/hw/hwqueuefilterexecute.o \
	${OBJECTDIR}/src/queue/lw/lwqueue.o \
	${OBJECTDIR}/src/resource/resource.o \
	${OBJECTDIR}/src/resource/resourceacess.o \
	${OBJECTDIR}/src/resource/resourceexecute.o \
	${OBJECTDIR}/src/resource/resourcefilter.o \
	${OBJECTDIR}/src/rtc/memcmp.o \
	${OBJECTDIR}/src/rtc/memcpy.o \
	${OBJECTDIR}/src/rtc/memmove.o \
	${OBJECTDIR}/src/rtc/memrmov.o \
	${OBJECTDIR}/src/rtc/memset.o \
	${OBJECTDIR}/src/rtc/strnlen.o \
	${OBJECTDIR}/src/scheduler/flat.o \
	${OBJECTDIR}/src/scheduler/hierarchical.o \
	${OBJECTDIR}/src/semaphore/ceiling.o \
	${OBJECTDIR}/src/semaphore/counting.o \
	${OBJECTDIR}/src/semaphore/mutex.o \
	${OBJECTDIR}/src/thread/queue/queue.o \
	${OBJECTDIR}/src/thread/queue/queuefifo.o \
	${OBJECTDIR}/src/thread/queue/queuepriority.o \
	${OBJECTDIR}/src/thread/thread.o \
	${OBJECTDIR}/src/thread/threadaperiodic.o \
	${OBJECTDIR}/src/thread/threadcreate.o \
	${OBJECTDIR}/src/thread/threaddispatcher.o \
	${OBJECTDIR}/src/thread/threadid.o \
	${OBJECTDIR}/src/thread/threadidle.o \
	${OBJECTDIR}/src/thread/threadperiodic.o \
	${OBJECTDIR}/src/thread/threadpriority.o \
	${OBJECTDIR}/src/thread/threadrestart.o \
	${OBJECTDIR}/src/thread/threadresume.o \
	${OBJECTDIR}/src/thread/threadsporadic.o \
	${OBJECTDIR}/src/thread/threadstack.o \
	${OBJECTDIR}/src/thread/threadstart.o \
	${OBJECTDIR}/src/thread/threadstate.o \
	${OBJECTDIR}/src/thread/threadstatus.o \
	${OBJECTDIR}/src/thread/threadstop.o \
	${OBJECTDIR}/src/thread/threadsuspend.o \
	${OBJECTDIR}/src/time/time.o \
	${OBJECTDIR}/src/timer/timer.o


# C Compiler Flags
CFLAGS=-fno-common -fno-exceptions -fno-unwind-tables -O2 -mcpu=cortex-r5 -mfpu=vfpv3-d16 -mbig-endian -ffunction-sections -fdata-sections -Wall -Wextra -Wno-unused-parameter -Wno-unused-function

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=-c -mcpu=v7 -g -mcpu=arm926ej-s

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libcommon.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libcommon.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libcommon.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libcommon.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libcommon.a

${OBJECTDIR}/src/assert/assert.o: src/assert/assert.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/assert
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/assert/assert.o src/assert/assert.c

${OBJECTDIR}/src/buffer/buffer.o: src/buffer/buffer.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/buffer
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/buffer/buffer.o src/buffer/buffer.c

${OBJECTDIR}/src/bulk/bulk.o: src/bulk/bulk.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/bulk
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/bulk/bulk.o src/bulk/bulk.c

${OBJECTDIR}/src/channel/channel.o: src/channel/channel.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/channel
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/channel/channel.o src/channel/channel.c

${OBJECTDIR}/src/channel/channelbulk.o: src/channel/channelbulk.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/channel
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/channel/channelbulk.o src/channel/channelbulk.c

${OBJECTDIR}/src/channel/channelfilter.o: src/channel/channelfilter.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/channel
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/channel/channelfilter.o src/channel/channelfilter.c

${OBJECTDIR}/src/channel/channelfilterexecute.o: src/channel/channelfilterexecute.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/channel
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/channel/channelfilterexecute.o src/channel/channelfilterexecute.c

${OBJECTDIR}/src/channel/channelreceive.o: src/channel/channelreceive.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/channel
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/channel/channelreceive.o src/channel/channelreceive.c

${OBJECTDIR}/src/channel/channelsend.o: src/channel/channelsend.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/channel
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/channel/channelsend.o src/channel/channelsend.c

${OBJECTDIR}/src/clock/clock.o: src/clock/clock.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/clock
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/clock/clock.o src/clock/clock.c

${OBJECTDIR}/src/collection/doublelinkedsimplelist.o: src/collection/doublelinkedsimplelist.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/collection
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/collection/doublelinkedsimplelist.o src/collection/doublelinkedsimplelist.c

${OBJECTDIR}/src/collection/linkedlist.o: src/collection/linkedlist.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/collection
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/collection/linkedlist.o src/collection/linkedlist.c

${OBJECTDIR}/src/collection/simplelist.o: src/collection/simplelist.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/collection
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/collection/simplelist.o src/collection/simplelist.c

${OBJECTDIR}/src/collection/treemap.o: src/collection/treemap.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/collection
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/collection/treemap.o src/collection/treemap.c

${OBJECTDIR}/src/core/core.o: src/core/core.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/core
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/core/core.o src/core/core.c

${OBJECTDIR}/src/core/math.o: src/core/math.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/core
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/core/math.o src/core/math.c

${OBJECTDIR}/src/core/multithreading.o: src/core/multithreading.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/core
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/core/multithreading.o src/core/multithreading.c

${OBJECTDIR}/src/core/priority.o: src/core/priority.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/core
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/core/priority.o src/core/priority.c

${OBJECTDIR}/src/core/syscall.o: src/core/syscall.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/core
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/core/syscall.o src/core/syscall.c

${OBJECTDIR}/src/error/error.o: src/error/error.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/error
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/error/error.o src/error/error.c

${OBJECTDIR}/src/event/event.o: src/event/event.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/event
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/event/event.o src/event/event.c

${OBJECTDIR}/src/future/future.o: src/future/future.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/future
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/future/future.o src/future/future.c

${OBJECTDIR}/src/future/futureQueue.o: src/future/futureQueue.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/future
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/future/futureQueue.o src/future/futureQueue.c

${OBJECTDIR}/src/future/futureTable.o: src/future/futureTable.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/future
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/future/futureTable.o src/future/futureTable.c

${OBJECTDIR}/src/future/futurefree.o: src/future/futurefree.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/future
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/future/futurefree.o src/future/futurefree.c

${OBJECTDIR}/src/future/futureready.o: src/future/futureready.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/future
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/future/futureready.o src/future/futureready.c

${OBJECTDIR}/src/interrupt/critical.o: src/interrupt/critical.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/interrupt
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/interrupt/critical.o src/interrupt/critical.c

${OBJECTDIR}/src/interrupt/interrupt.o: src/interrupt/interrupt.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/interrupt
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/interrupt/interrupt.o src/interrupt/interrupt.c

${OBJECTDIR}/src/ipcall/ipcall.o: src/ipcall/ipcall.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/ipcall
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ipcall/ipcall.o src/ipcall/ipcall.c

${OBJECTDIR}/src/ipcall/ipcallexecute.o: src/ipcall/ipcallexecute.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/ipcall
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ipcall/ipcallexecute.o src/ipcall/ipcallexecute.c

${OBJECTDIR}/src/ipcall/ipcallinternal.o: src/ipcall/ipcallinternal.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/ipcall
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ipcall/ipcallinternal.o src/ipcall/ipcallinternal.c

${OBJECTDIR}/src/memory/malloc.o: src/memory/malloc.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/memory
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/memory/malloc.o src/memory/malloc.c

${OBJECTDIR}/src/memory/protection.o: src/memory/protection.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/memory
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/memory/protection.o src/memory/protection.c

${OBJECTDIR}/src/object/object.o: src/object/object.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/object
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/object/object.o src/object/object.c

${OBJECTDIR}/src/object/protection.o: src/object/protection.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/object
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/object/protection.o src/object/protection.c

${OBJECTDIR}/src/partition/partition.o: src/partition/partition.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/partition
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/partition/partition.o src/partition/partition.c

${OBJECTDIR}/src/partition/partitionrestart.o: src/partition/partitionrestart.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/partition
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/partition/partitionrestart.o src/partition/partitionrestart.c

${OBJECTDIR}/src/queue/hw/hwqueue.o: src/queue/hw/hwqueue.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/queue/hw
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/queue/hw/hwqueue.o src/queue/hw/hwqueue.c

${OBJECTDIR}/src/queue/hw/hwqueuefilter.o: src/queue/hw/hwqueuefilter.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/queue/hw
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/queue/hw/hwqueuefilter.o src/queue/hw/hwqueuefilter.c

${OBJECTDIR}/src/queue/hw/hwqueuefilterexecute.o: src/queue/hw/hwqueuefilterexecute.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/queue/hw
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/queue/hw/hwqueuefilterexecute.o src/queue/hw/hwqueuefilterexecute.c

${OBJECTDIR}/src/queue/lw/lwqueue.o: src/queue/lw/lwqueue.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/queue/lw
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/queue/lw/lwqueue.o src/queue/lw/lwqueue.c

${OBJECTDIR}/src/resource/resource.o: src/resource/resource.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/resource
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/resource/resource.o src/resource/resource.c

${OBJECTDIR}/src/resource/resourceacess.o: src/resource/resourceacess.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/resource
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/resource/resourceacess.o src/resource/resourceacess.c

${OBJECTDIR}/src/resource/resourceexecute.o: src/resource/resourceexecute.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/resource
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/resource/resourceexecute.o src/resource/resourceexecute.c

${OBJECTDIR}/src/resource/resourcefilter.o: src/resource/resourcefilter.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/resource
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/resource/resourcefilter.o src/resource/resourcefilter.c

${OBJECTDIR}/src/rtc/memcmp.o: src/rtc/memcmp.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/rtc
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/rtc/memcmp.o src/rtc/memcmp.c

${OBJECTDIR}/src/rtc/memcpy.o: src/rtc/memcpy.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/rtc
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/rtc/memcpy.o src/rtc/memcpy.c

${OBJECTDIR}/src/rtc/memmove.o: src/rtc/memmove.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/rtc
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/rtc/memmove.o src/rtc/memmove.c

${OBJECTDIR}/src/rtc/memrmov.o: src/rtc/memrmov.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/rtc
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/rtc/memrmov.o src/rtc/memrmov.c

${OBJECTDIR}/src/rtc/memset.o: src/rtc/memset.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/rtc
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/rtc/memset.o src/rtc/memset.c

${OBJECTDIR}/src/rtc/strnlen.o: src/rtc/strnlen.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/rtc
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/rtc/strnlen.o src/rtc/strnlen.c

${OBJECTDIR}/src/scheduler/flat.o: src/scheduler/flat.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/scheduler
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/scheduler/flat.o src/scheduler/flat.c

${OBJECTDIR}/src/scheduler/hierarchical.o: src/scheduler/hierarchical.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/scheduler
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/scheduler/hierarchical.o src/scheduler/hierarchical.c

${OBJECTDIR}/src/semaphore/ceiling.o: src/semaphore/ceiling.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/semaphore
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/semaphore/ceiling.o src/semaphore/ceiling.c

${OBJECTDIR}/src/semaphore/counting.o: src/semaphore/counting.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/semaphore
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/semaphore/counting.o src/semaphore/counting.c

${OBJECTDIR}/src/semaphore/mutex.o: src/semaphore/mutex.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/semaphore
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/semaphore/mutex.o src/semaphore/mutex.c

${OBJECTDIR}/src/thread/queue/queue.o: src/thread/queue/queue.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/thread/queue
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/thread/queue/queue.o src/thread/queue/queue.c

${OBJECTDIR}/src/thread/queue/queuefifo.o: src/thread/queue/queuefifo.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/thread/queue
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/thread/queue/queuefifo.o src/thread/queue/queuefifo.c

${OBJECTDIR}/src/thread/queue/queuepriority.o: src/thread/queue/queuepriority.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/thread/queue
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/thread/queue/queuepriority.o src/thread/queue/queuepriority.c

${OBJECTDIR}/src/thread/thread.o: src/thread/thread.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/thread
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/thread/thread.o src/thread/thread.c

${OBJECTDIR}/src/thread/threadaperiodic.o: src/thread/threadaperiodic.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/thread
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/thread/threadaperiodic.o src/thread/threadaperiodic.c

${OBJECTDIR}/src/thread/threadcreate.o: src/thread/threadcreate.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/thread
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/thread/threadcreate.o src/thread/threadcreate.c

${OBJECTDIR}/src/thread/threaddispatcher.o: src/thread/threaddispatcher.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/thread
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/thread/threaddispatcher.o src/thread/threaddispatcher.c

${OBJECTDIR}/src/thread/threadid.o: src/thread/threadid.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/thread
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/thread/threadid.o src/thread/threadid.c

${OBJECTDIR}/src/thread/threadidle.o: src/thread/threadidle.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/thread
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/thread/threadidle.o src/thread/threadidle.c

${OBJECTDIR}/src/thread/threadperiodic.o: src/thread/threadperiodic.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/thread
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/thread/threadperiodic.o src/thread/threadperiodic.c

${OBJECTDIR}/src/thread/threadpriority.o: src/thread/threadpriority.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/thread
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/thread/threadpriority.o src/thread/threadpriority.c

${OBJECTDIR}/src/thread/threadrestart.o: src/thread/threadrestart.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/thread
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/thread/threadrestart.o src/thread/threadrestart.c

${OBJECTDIR}/src/thread/threadresume.o: src/thread/threadresume.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/thread
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/thread/threadresume.o src/thread/threadresume.c

${OBJECTDIR}/src/thread/threadsporadic.o: src/thread/threadsporadic.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/thread
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/thread/threadsporadic.o src/thread/threadsporadic.c

${OBJECTDIR}/src/thread/threadstack.o: src/thread/threadstack.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/thread
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/thread/threadstack.o src/thread/threadstack.c

${OBJECTDIR}/src/thread/threadstart.o: src/thread/threadstart.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/thread
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/thread/threadstart.o src/thread/threadstart.c

${OBJECTDIR}/src/thread/threadstate.o: src/thread/threadstate.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/thread
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/thread/threadstate.o src/thread/threadstate.c

${OBJECTDIR}/src/thread/threadstatus.o: src/thread/threadstatus.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/thread
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/thread/threadstatus.o src/thread/threadstatus.c

${OBJECTDIR}/src/thread/threadstop.o: src/thread/threadstop.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/thread
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/thread/threadstop.o src/thread/threadstop.c

${OBJECTDIR}/src/thread/threadsuspend.o: src/thread/threadsuspend.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/thread
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/thread/threadsuspend.o src/thread/threadsuspend.c

${OBJECTDIR}/src/time/time.o: src/time/time.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/time
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/time/time.o src/time/time.c

${OBJECTDIR}/src/timer/timer.o: src/timer/timer.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/timer
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../Hal/Arm/support/include -I../../Hal/Arm/isa/Arm/include -I../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../Hal/Arm/memory/Mpu/include -I../../Hal/Arm/manufacturer/ti/Vim/include -I../rsp/include -I../../Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/timer/timer.o src/timer/timer.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
