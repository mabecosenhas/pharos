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
RANLIB=$$PHAROS_ARM_AARCH64_RANLIB
CC=$$PHAROS_ARM_AARCH64_CC
CCC=g++
CXX=g++
FC=gfortran
AS=$$PHAROS_ARM_AARCH64_CC
AR=$$PHAROS_ARM_AARCH64_ARCHIVER

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=arm-v8a-a53-rasp3-qemu-debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/cache.o \
	${OBJECTDIR}/src/cacheLow.o \
	${OBJECTDIR}/src/channel.o \
	${OBJECTDIR}/src/context.o \
	${OBJECTDIR}/src/criticalSection.o \
	${OBJECTDIR}/src/hwqueue.o \
	${OBJECTDIR}/src/interrupt.o \
	${OBJECTDIR}/src/interruptHandler.o \
	${OBJECTDIR}/src/interrupts.o \
	${OBJECTDIR}/src/interruptstack.o \
	${OBJECTDIR}/src/ipcall.o \
	${OBJECTDIR}/src/resource.o \
	${OBJECTDIR}/src/restart.o \
	${OBJECTDIR}/src/stack.o \
	${OBJECTDIR}/src/start.o \
	${OBJECTDIR}/src/startup.o \
	${OBJECTDIR}/src/syscallHandler.o \
	${OBJECTDIR}/src/syscalls.o \
	${OBJECTDIR}/src/systemError.o \
	${OBJECTDIR}/src/thread.o \
	${OBJECTDIR}/src/userMode.o


# C Compiler Flags
CFLAGS=-O0 -g -mcpu=cortex-a53 -ffunction-sections -fdata-sections -Wall -Wextra -Wno-unused-parameter -Wno-unused-function

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=-c -g -mcpu=cortex-a53 -Iinclude -I../../../../../Kernel/common/include -I../../../processor/Cortex-A/A53/BCM2837/include

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libarchv8-a.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libarchv8-a.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libarchv8-a.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libarchv8-a.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libarchv8-a.a

${OBJECTDIR}/src/cache.o: src/cache.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -Iinclude -I../../../../../Kernel/common/include -I../../../board/raspberry/raspberry3/include -I../MmuV8/include -I../../../processor/Cortex-A/A53/BCM2837/include -I../../../isa/A64/include -I../MmuV8/include -I../../../../../Kernel/rmp/include -I../../../../memory/MemoryAreaSingle/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/cache.o src/cache.c

${OBJECTDIR}/src/cacheLow.o: src/cacheLow.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/cacheLow.o src/cacheLow.S

${OBJECTDIR}/src/channel.o: src/channel.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/channel.o src/channel.S

${OBJECTDIR}/src/context.o: src/context.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/context.o src/context.S

${OBJECTDIR}/src/criticalSection.o: src/criticalSection.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/criticalSection.o src/criticalSection.S

${OBJECTDIR}/src/hwqueue.o: src/hwqueue.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/hwqueue.o src/hwqueue.S

${OBJECTDIR}/src/interrupt.o: src/interrupt.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -Iinclude -I../../../../../Kernel/common/include -I../../../board/raspberry/raspberry3/include -I../MmuV8/include -I../../../processor/Cortex-A/A53/BCM2837/include -I../../../isa/A64/include -I../MmuV8/include -I../../../../../Kernel/rmp/include -I../../../../memory/MemoryAreaSingle/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/interrupt.o src/interrupt.c

${OBJECTDIR}/src/interruptHandler.o: src/interruptHandler.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/interruptHandler.o src/interruptHandler.S

${OBJECTDIR}/src/interrupts.o: src/interrupts.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/interrupts.o src/interrupts.S

${OBJECTDIR}/src/interruptstack.o: src/interruptstack.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -Iinclude -I../../../../../Kernel/common/include -I../../../board/raspberry/raspberry3/include -I../MmuV8/include -I../../../processor/Cortex-A/A53/BCM2837/include -I../../../isa/A64/include -I../MmuV8/include -I../../../../../Kernel/rmp/include -I../../../../memory/MemoryAreaSingle/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/interruptstack.o src/interruptstack.c

${OBJECTDIR}/src/ipcall.o: src/ipcall.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/ipcall.o src/ipcall.S

${OBJECTDIR}/src/resource.o: src/resource.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/resource.o src/resource.S

${OBJECTDIR}/src/restart.o: src/restart.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -Iinclude -I../../../../../Kernel/common/include -I../../../board/raspberry/raspberry3/include -I../MmuV8/include -I../../../processor/Cortex-A/A53/BCM2837/include -I../../../isa/A64/include -I../MmuV8/include -I../../../../../Kernel/rmp/include -I../../../../memory/MemoryAreaSingle/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/restart.o src/restart.c

${OBJECTDIR}/src/stack.o: src/stack.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/stack.o src/stack.S

${OBJECTDIR}/src/start.o: src/start.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/start.o src/start.S

${OBJECTDIR}/src/startup.o: src/startup.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -Iinclude -I../../../../../Kernel/common/include -I../../../board/raspberry/raspberry3/include -I../MmuV8/include -I../../../processor/Cortex-A/A53/BCM2837/include -I../../../isa/A64/include -I../MmuV8/include -I../../../../../Kernel/rmp/include -I../../../../memory/MemoryAreaSingle/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/startup.o src/startup.c

${OBJECTDIR}/src/syscallHandler.o: src/syscallHandler.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/syscallHandler.o src/syscallHandler.S

${OBJECTDIR}/src/syscalls.o: src/syscalls.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/syscalls.o src/syscalls.S

${OBJECTDIR}/src/systemError.o: src/systemError.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/systemError.o src/systemError.S

${OBJECTDIR}/src/thread.o: src/thread.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -Iinclude -I../../../../../Kernel/common/include -I../../../board/raspberry/raspberry3/include -I../MmuV8/include -I../../../processor/Cortex-A/A53/BCM2837/include -I../../../isa/A64/include -I../MmuV8/include -I../../../../../Kernel/rmp/include -I../../../../memory/MemoryAreaSingle/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/thread.o src/thread.c

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
