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
RANLIB=$$PHAROS_ARM_M4_RANLIB
CC=$$PHAROS_ARM_M4_CC
CCC=g++
CXX=g++
FC=gfortran
AS=$$PHAROS_ARM_M4_CC
AR=$$PHAROS_ARM_M4_ARCHIVER

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=arm-v7em-m4f-TM4C129ENCPDT-EK_TM4C129EXL-release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/cache.o \
	${OBJECTDIR}/src/channel.o \
	${OBJECTDIR}/src/channelHigh.o \
	${OBJECTDIR}/src/context.o \
	${OBJECTDIR}/src/contextHigh.o \
	${OBJECTDIR}/src/criticalSection.o \
	${OBJECTDIR}/src/fpu.o \
	${OBJECTDIR}/src/hwqueue.o \
	${OBJECTDIR}/src/hwqueueHigh.o \
	${OBJECTDIR}/src/interpartition.o \
	${OBJECTDIR}/src/interpartitionhigh.o \
	${OBJECTDIR}/src/interrupt.o \
	${OBJECTDIR}/src/interruptstack.o \
	${OBJECTDIR}/src/ipCallHigh.o \
	${OBJECTDIR}/src/ipcall.o \
	${OBJECTDIR}/src/misc.o \
	${OBJECTDIR}/src/nvic.o \
	${OBJECTDIR}/src/resource.o \
	${OBJECTDIR}/src/resourceHigh.o \
	${OBJECTDIR}/src/start.o \
	${OBJECTDIR}/src/svc.o \
	${OBJECTDIR}/src/syscall.o \
	${OBJECTDIR}/src/systick.o \
	${OBJECTDIR}/src/thread.o \
	${OBJECTDIR}/src/userMode.o


# C Compiler Flags
CFLAGS=-O2 -g -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfpu=fpv4-sp-d16 -ffunction-sections -fdata-sections -Wall -Wextra -Wno-unused-parameter -Wno-unused-function

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=-mthumb -c -mcpu=cortex-m4 -march=armv7e-m -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Iinclude -I../../../../processor/Cortex-M/M4/TM4C129ENCPDT/include -I../../../../../../Kernel/common/include

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libarchv7-m.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libarchv7-m.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libarchv7-m.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libarchv7-m.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libarchv7-m.a

${OBJECTDIR}/src/cache.o: src/cache.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../../../../../../Kernel/common/include -Iinclude -I../../../../processor/Cortex-M/M4/TM4C129ENCPDT/include -I../../../../processor/Cortex-M/M4/M4/include -I../../../../support/include -I../PmsrV7-M/include -I../../../../board/TI/EK-TM4C129EXL/include -I../../../../isa/Thumb/include -I../../../../../../Kernel/rsp/include -I../../../../../memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/cache.o src/cache.c

${OBJECTDIR}/src/channel.o: src/channel.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/channel.o src/channel.S

${OBJECTDIR}/src/channelHigh.o: src/channelHigh.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../../../../../../Kernel/common/include -Iinclude -I../../../../processor/Cortex-M/M4/TM4C129ENCPDT/include -I../../../../processor/Cortex-M/M4/M4/include -I../../../../support/include -I../PmsrV7-M/include -I../../../../board/TI/EK-TM4C129EXL/include -I../../../../isa/Thumb/include -I../../../../../../Kernel/rsp/include -I../../../../../memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/channelHigh.o src/channelHigh.c

${OBJECTDIR}/src/context.o: src/context.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/context.o src/context.S

${OBJECTDIR}/src/contextHigh.o: src/contextHigh.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../../../../../../Kernel/common/include -Iinclude -I../../../../processor/Cortex-M/M4/TM4C129ENCPDT/include -I../../../../processor/Cortex-M/M4/M4/include -I../../../../support/include -I../PmsrV7-M/include -I../../../../board/TI/EK-TM4C129EXL/include -I../../../../isa/Thumb/include -I../../../../../../Kernel/rsp/include -I../../../../../memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/contextHigh.o src/contextHigh.c

${OBJECTDIR}/src/criticalSection.o: src/criticalSection.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/criticalSection.o src/criticalSection.S

${OBJECTDIR}/src/fpu.o: src/fpu.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../../../../../../Kernel/common/include -Iinclude -I../../../../processor/Cortex-M/M4/TM4C129ENCPDT/include -I../../../../processor/Cortex-M/M4/M4/include -I../../../../support/include -I../PmsrV7-M/include -I../../../../board/TI/EK-TM4C129EXL/include -I../../../../isa/Thumb/include -I../../../../../../Kernel/rsp/include -I../../../../../memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/fpu.o src/fpu.c

${OBJECTDIR}/src/hwqueue.o: src/hwqueue.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/hwqueue.o src/hwqueue.S

${OBJECTDIR}/src/hwqueueHigh.o: src/hwqueueHigh.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../../../../../../Kernel/common/include -Iinclude -I../../../../processor/Cortex-M/M4/TM4C129ENCPDT/include -I../../../../processor/Cortex-M/M4/M4/include -I../../../../support/include -I../PmsrV7-M/include -I../../../../board/TI/EK-TM4C129EXL/include -I../../../../isa/Thumb/include -I../../../../../../Kernel/rsp/include -I../../../../../memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/hwqueueHigh.o src/hwqueueHigh.c

${OBJECTDIR}/src/interpartition.o: src/interpartition.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/interpartition.o src/interpartition.S

${OBJECTDIR}/src/interpartitionhigh.o: src/interpartitionhigh.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../../../../../../Kernel/common/include -Iinclude -I../../../../processor/Cortex-M/M4/TM4C129ENCPDT/include -I../../../../processor/Cortex-M/M4/M4/include -I../../../../support/include -I../PmsrV7-M/include -I../../../../board/TI/EK-TM4C129EXL/include -I../../../../isa/Thumb/include -I../../../../../../Kernel/rsp/include -I../../../../../memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/interpartitionhigh.o src/interpartitionhigh.c

${OBJECTDIR}/src/interrupt.o: src/interrupt.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/interrupt.o src/interrupt.S

${OBJECTDIR}/src/interruptstack.o: src/interruptstack.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../../../../../../Kernel/common/include -Iinclude -I../../../../processor/Cortex-M/M4/TM4C129ENCPDT/include -I../../../../processor/Cortex-M/M4/M4/include -I../../../../support/include -I../PmsrV7-M/include -I../../../../board/TI/EK-TM4C129EXL/include -I../../../../isa/Thumb/include -I../../../../../../Kernel/rsp/include -I../../../../../memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/interruptstack.o src/interruptstack.c

${OBJECTDIR}/src/ipCallHigh.o: src/ipCallHigh.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../../../../../../Kernel/common/include -Iinclude -I../../../../processor/Cortex-M/M4/TM4C129ENCPDT/include -I../../../../processor/Cortex-M/M4/M4/include -I../../../../support/include -I../PmsrV7-M/include -I../../../../board/TI/EK-TM4C129EXL/include -I../../../../isa/Thumb/include -I../../../../../../Kernel/rsp/include -I../../../../../memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ipCallHigh.o src/ipCallHigh.c

${OBJECTDIR}/src/ipcall.o: src/ipcall.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/ipcall.o src/ipcall.S

${OBJECTDIR}/src/misc.o: src/misc.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/misc.o src/misc.S

${OBJECTDIR}/src/nvic.o: src/nvic.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../../../../../../Kernel/common/include -Iinclude -I../../../../processor/Cortex-M/M4/TM4C129ENCPDT/include -I../../../../processor/Cortex-M/M4/M4/include -I../../../../support/include -I../PmsrV7-M/include -I../../../../board/TI/EK-TM4C129EXL/include -I../../../../isa/Thumb/include -I../../../../../../Kernel/rsp/include -I../../../../../memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/nvic.o src/nvic.c

${OBJECTDIR}/src/resource.o: src/resource.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/resource.o src/resource.S

${OBJECTDIR}/src/resourceHigh.o: src/resourceHigh.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../../../../../../Kernel/common/include -Iinclude -I../../../../processor/Cortex-M/M4/TM4C129ENCPDT/include -I../../../../processor/Cortex-M/M4/M4/include -I../../../../support/include -I../PmsrV7-M/include -I../../../../board/TI/EK-TM4C129EXL/include -I../../../../isa/Thumb/include -I../../../../../../Kernel/rsp/include -I../../../../../memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/resourceHigh.o src/resourceHigh.c

${OBJECTDIR}/src/start.o: src/start.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/start.o src/start.S

${OBJECTDIR}/src/svc.o: src/svc.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../../../../../../Kernel/common/include -Iinclude -I../../../../processor/Cortex-M/M4/TM4C129ENCPDT/include -I../../../../processor/Cortex-M/M4/M4/include -I../../../../support/include -I../PmsrV7-M/include -I../../../../board/TI/EK-TM4C129EXL/include -I../../../../isa/Thumb/include -I../../../../../../Kernel/rsp/include -I../../../../../memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/svc.o src/svc.c

${OBJECTDIR}/src/syscall.o: src/syscall.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/syscall.o src/syscall.S

${OBJECTDIR}/src/systick.o: src/systick.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../../../../../../Kernel/common/include -Iinclude -I../../../../processor/Cortex-M/M4/TM4C129ENCPDT/include -I../../../../processor/Cortex-M/M4/M4/include -I../../../../support/include -I../PmsrV7-M/include -I../../../../board/TI/EK-TM4C129EXL/include -I../../../../isa/Thumb/include -I../../../../../../Kernel/rsp/include -I../../../../../memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/systick.o src/systick.c

${OBJECTDIR}/src/thread.o: src/thread.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../../../../../../Kernel/common/include -Iinclude -I../../../../processor/Cortex-M/M4/TM4C129ENCPDT/include -I../../../../processor/Cortex-M/M4/M4/include -I../../../../support/include -I../PmsrV7-M/include -I../../../../board/TI/EK-TM4C129EXL/include -I../../../../isa/Thumb/include -I../../../../../../Kernel/rsp/include -I../../../../../memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/thread.o src/thread.c

${OBJECTDIR}/src/userMode.o: src/userMode.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/userMode.o src/userMode.S

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
