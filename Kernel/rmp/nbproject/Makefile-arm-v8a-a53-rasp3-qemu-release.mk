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
CND_CONF=arm-v8a-a53-rasp3-qemu-release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/channel/rmpbulk.o \
	${OBJECTDIR}/src/channel/rmpchannelreceive.o \
	${OBJECTDIR}/src/mpsc.o \
	${OBJECTDIR}/src/rmp.o \
	${OBJECTDIR}/src/rmpatomic.o \
	${OBJECTDIR}/src/rmpchannelsend.o \
	${OBJECTDIR}/src/rmpfuture.o \
	${OBJECTDIR}/src/rmphwqueue.o \
	${OBJECTDIR}/src/rmppartition.o \
	${OBJECTDIR}/src/rmpscheduler.o \
	${OBJECTDIR}/src/rmpthread.o \
	${OBJECTDIR}/src/rmpthreadqueue.o


# C Compiler Flags
CFLAGS=-O2 -g -mcpu=cortex-a53 -ffunction-sections -fdata-sections -Wall -Wextra -Wno-unused-parameter -Wno-unused-function

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/librmp.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/librmp.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/librmp.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/librmp.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/librmp.a

${OBJECTDIR}/src/channel/rmpbulk.o: src/channel/rmpbulk.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/channel
	${RM} "$@.d"
	$(COMPILE.c) -g -I../common/include -Iinclude -I../../Hal/Arm/board/raspberry/raspberry3/include -I../../Hal/Arm/processor/Cortex-A/A53/BCM2837/include -I../../Hal/Arm/architecture/v8/Archv8-A/include -I../../Hal/Arm/architecture/v8/MmuV8/include -I../../Hal/Arm/architecture/v8/GenericTimer/include -I../../Hal/Arm/isa/A64/include -I../../Hal/memory/MemoryAreaSingle/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/channel/rmpbulk.o src/channel/rmpbulk.c

${OBJECTDIR}/src/channel/rmpchannelreceive.o: src/channel/rmpchannelreceive.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/channel
	${RM} "$@.d"
	$(COMPILE.c) -g -I../common/include -Iinclude -I../../Hal/Arm/board/raspberry/raspberry3/include -I../../Hal/Arm/processor/Cortex-A/A53/BCM2837/include -I../../Hal/Arm/architecture/v8/Archv8-A/include -I../../Hal/Arm/architecture/v8/MmuV8/include -I../../Hal/Arm/architecture/v8/GenericTimer/include -I../../Hal/Arm/isa/A64/include -I../../Hal/memory/MemoryAreaSingle/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/channel/rmpchannelreceive.o src/channel/rmpchannelreceive.c

${OBJECTDIR}/src/mpsc.o: src/mpsc.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -I../common/include -Iinclude -I../../Hal/Arm/board/raspberry/raspberry3/include -I../../Hal/Arm/processor/Cortex-A/A53/BCM2837/include -I../../Hal/Arm/architecture/v8/Archv8-A/include -I../../Hal/Arm/architecture/v8/MmuV8/include -I../../Hal/Arm/architecture/v8/GenericTimer/include -I../../Hal/Arm/isa/A64/include -I../../Hal/memory/MemoryAreaSingle/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/mpsc.o src/mpsc.c

${OBJECTDIR}/src/rmp.o: src/rmp.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -I../common/include -Iinclude -I../../Hal/Arm/board/raspberry/raspberry3/include -I../../Hal/Arm/processor/Cortex-A/A53/BCM2837/include -I../../Hal/Arm/architecture/v8/Archv8-A/include -I../../Hal/Arm/architecture/v8/MmuV8/include -I../../Hal/Arm/architecture/v8/GenericTimer/include -I../../Hal/Arm/isa/A64/include -I../../Hal/memory/MemoryAreaSingle/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/rmp.o src/rmp.c

${OBJECTDIR}/src/rmpatomic.o: src/rmpatomic.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -I../common/include -Iinclude -I../../Hal/Arm/board/raspberry/raspberry3/include -I../../Hal/Arm/processor/Cortex-A/A53/BCM2837/include -I../../Hal/Arm/architecture/v8/Archv8-A/include -I../../Hal/Arm/architecture/v8/MmuV8/include -I../../Hal/Arm/architecture/v8/GenericTimer/include -I../../Hal/Arm/isa/A64/include -I../../Hal/memory/MemoryAreaSingle/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/rmpatomic.o src/rmpatomic.c

${OBJECTDIR}/src/rmpchannelsend.o: src/rmpchannelsend.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -I../common/include -Iinclude -I../../Hal/Arm/board/raspberry/raspberry3/include -I../../Hal/Arm/processor/Cortex-A/A53/BCM2837/include -I../../Hal/Arm/architecture/v8/Archv8-A/include -I../../Hal/Arm/architecture/v8/MmuV8/include -I../../Hal/Arm/architecture/v8/GenericTimer/include -I../../Hal/Arm/isa/A64/include -I../../Hal/memory/MemoryAreaSingle/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/rmpchannelsend.o src/rmpchannelsend.c

${OBJECTDIR}/src/rmpfuture.o: src/rmpfuture.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -I../common/include -Iinclude -I../../Hal/Arm/board/raspberry/raspberry3/include -I../../Hal/Arm/processor/Cortex-A/A53/BCM2837/include -I../../Hal/Arm/architecture/v8/Archv8-A/include -I../../Hal/Arm/architecture/v8/MmuV8/include -I../../Hal/Arm/architecture/v8/GenericTimer/include -I../../Hal/Arm/isa/A64/include -I../../Hal/memory/MemoryAreaSingle/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/rmpfuture.o src/rmpfuture.c

${OBJECTDIR}/src/rmphwqueue.o: src/rmphwqueue.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -I../common/include -Iinclude -I../../Hal/Arm/board/raspberry/raspberry3/include -I../../Hal/Arm/processor/Cortex-A/A53/BCM2837/include -I../../Hal/Arm/architecture/v8/Archv8-A/include -I../../Hal/Arm/architecture/v8/MmuV8/include -I../../Hal/Arm/architecture/v8/GenericTimer/include -I../../Hal/Arm/isa/A64/include -I../../Hal/memory/MemoryAreaSingle/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/rmphwqueue.o src/rmphwqueue.c

${OBJECTDIR}/src/rmppartition.o: src/rmppartition.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -I../common/include -Iinclude -I../../Hal/Arm/board/raspberry/raspberry3/include -I../../Hal/Arm/processor/Cortex-A/A53/BCM2837/include -I../../Hal/Arm/architecture/v8/Archv8-A/include -I../../Hal/Arm/architecture/v8/MmuV8/include -I../../Hal/Arm/architecture/v8/GenericTimer/include -I../../Hal/Arm/isa/A64/include -I../../Hal/memory/MemoryAreaSingle/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/rmppartition.o src/rmppartition.c

${OBJECTDIR}/src/rmpscheduler.o: src/rmpscheduler.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -I../common/include -Iinclude -I../../Hal/Arm/board/raspberry/raspberry3/include -I../../Hal/Arm/processor/Cortex-A/A53/BCM2837/include -I../../Hal/Arm/architecture/v8/Archv8-A/include -I../../Hal/Arm/architecture/v8/MmuV8/include -I../../Hal/Arm/architecture/v8/GenericTimer/include -I../../Hal/Arm/isa/A64/include -I../../Hal/memory/MemoryAreaSingle/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/rmpscheduler.o src/rmpscheduler.c

${OBJECTDIR}/src/rmpthread.o: src/rmpthread.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -I../common/include -Iinclude -I../../Hal/Arm/board/raspberry/raspberry3/include -I../../Hal/Arm/processor/Cortex-A/A53/BCM2837/include -I../../Hal/Arm/architecture/v8/Archv8-A/include -I../../Hal/Arm/architecture/v8/MmuV8/include -I../../Hal/Arm/architecture/v8/GenericTimer/include -I../../Hal/Arm/isa/A64/include -I../../Hal/memory/MemoryAreaSingle/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/rmpthread.o src/rmpthread.c

${OBJECTDIR}/src/rmpthreadqueue.o: src/rmpthreadqueue.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -I../common/include -Iinclude -I../../Hal/Arm/board/raspberry/raspberry3/include -I../../Hal/Arm/processor/Cortex-A/A53/BCM2837/include -I../../Hal/Arm/architecture/v8/Archv8-A/include -I../../Hal/Arm/architecture/v8/MmuV8/include -I../../Hal/Arm/architecture/v8/GenericTimer/include -I../../Hal/Arm/isa/A64/include -I../../Hal/memory/MemoryAreaSingle/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/rmpthreadqueue.o src/rmpthreadqueue.c

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
