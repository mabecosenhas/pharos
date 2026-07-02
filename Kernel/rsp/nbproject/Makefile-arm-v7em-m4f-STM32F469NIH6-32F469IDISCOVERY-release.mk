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
CND_CONF=arm-v7em-m4f-STM32F469NIH6-32F469IDISCOVERY-release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/rsp.o \
	${OBJECTDIR}/src/rspatomic.o \
	${OBJECTDIR}/src/rspchannel.o \
	${OBJECTDIR}/src/rspfuture.o \
	${OBJECTDIR}/src/rsphwqueue.o \
	${OBJECTDIR}/src/rsppartition.o \
	${OBJECTDIR}/src/rspscheduler.o \
	${OBJECTDIR}/src/rspthread.o


# C Compiler Flags
CFLAGS=-O2 -g -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfpu=fpv4-sp-d16 -ffunction-sections -fdata-sections -Wall -Wextra -Wno-unused-parameter -Wno-unused-function

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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/librsp.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/librsp.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/librsp.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/librsp.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/librsp.a

${OBJECTDIR}/src/rsp.o: src/rsp.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../common/include -Iinclude -I../../Hal/Arm/architecture/v7/v7-M/ArchV7-M/include -I../../Hal/Arm/architecture/v7/v7-M/PmsrV7-M/include -I../../Hal/Arm/isa/Thumb/include -I../../Hal/Arm/processor/Cortex-M/M4/M4/include -I../../Hal/Arm/support/include -I../../Hal/Arm/board/ST/DISCOVERY/32F469I/include -I../../Hal/Arm/processor/Cortex-M/M4/STM32F469NIH6/include -I../../Hal/memory/MemoryAreaMultiple/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/rsp.o src/rsp.c

${OBJECTDIR}/src/rspatomic.o: src/rspatomic.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../common/include -Iinclude -I../../Hal/Arm/architecture/v7/v7-M/ArchV7-M/include -I../../Hal/Arm/architecture/v7/v7-M/PmsrV7-M/include -I../../Hal/Arm/isa/Thumb/include -I../../Hal/Arm/processor/Cortex-M/M4/M4/include -I../../Hal/Arm/support/include -I../../Hal/Arm/board/ST/DISCOVERY/32F469I/include -I../../Hal/Arm/processor/Cortex-M/M4/STM32F469NIH6/include -I../../Hal/memory/MemoryAreaMultiple/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/rspatomic.o src/rspatomic.c

${OBJECTDIR}/src/rspchannel.o: src/rspchannel.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../common/include -Iinclude -I../../Hal/Arm/architecture/v7/v7-M/ArchV7-M/include -I../../Hal/Arm/architecture/v7/v7-M/PmsrV7-M/include -I../../Hal/Arm/isa/Thumb/include -I../../Hal/Arm/processor/Cortex-M/M4/M4/include -I../../Hal/Arm/support/include -I../../Hal/Arm/board/ST/DISCOVERY/32F469I/include -I../../Hal/Arm/processor/Cortex-M/M4/STM32F469NIH6/include -I../../Hal/memory/MemoryAreaMultiple/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/rspchannel.o src/rspchannel.c

${OBJECTDIR}/src/rspfuture.o: src/rspfuture.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../common/include -Iinclude -I../../Hal/Arm/architecture/v7/v7-M/ArchV7-M/include -I../../Hal/Arm/architecture/v7/v7-M/PmsrV7-M/include -I../../Hal/Arm/isa/Thumb/include -I../../Hal/Arm/processor/Cortex-M/M4/M4/include -I../../Hal/Arm/support/include -I../../Hal/Arm/board/ST/DISCOVERY/32F469I/include -I../../Hal/Arm/processor/Cortex-M/M4/STM32F469NIH6/include -I../../Hal/memory/MemoryAreaMultiple/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/rspfuture.o src/rspfuture.c

${OBJECTDIR}/src/rsphwqueue.o: src/rsphwqueue.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../common/include -Iinclude -I../../Hal/Arm/architecture/v7/v7-M/ArchV7-M/include -I../../Hal/Arm/architecture/v7/v7-M/PmsrV7-M/include -I../../Hal/Arm/isa/Thumb/include -I../../Hal/Arm/processor/Cortex-M/M4/M4/include -I../../Hal/Arm/support/include -I../../Hal/Arm/board/ST/DISCOVERY/32F469I/include -I../../Hal/Arm/processor/Cortex-M/M4/STM32F469NIH6/include -I../../Hal/memory/MemoryAreaMultiple/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/rsphwqueue.o src/rsphwqueue.c

${OBJECTDIR}/src/rsppartition.o: src/rsppartition.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../common/include -Iinclude -I../../Hal/Arm/architecture/v7/v7-M/ArchV7-M/include -I../../Hal/Arm/architecture/v7/v7-M/PmsrV7-M/include -I../../Hal/Arm/isa/Thumb/include -I../../Hal/Arm/processor/Cortex-M/M4/M4/include -I../../Hal/Arm/support/include -I../../Hal/Arm/board/ST/DISCOVERY/32F469I/include -I../../Hal/Arm/processor/Cortex-M/M4/STM32F469NIH6/include -I../../Hal/memory/MemoryAreaMultiple/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/rsppartition.o src/rsppartition.c

${OBJECTDIR}/src/rspscheduler.o: src/rspscheduler.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../common/include -Iinclude -I../../Hal/Arm/architecture/v7/v7-M/ArchV7-M/include -I../../Hal/Arm/architecture/v7/v7-M/PmsrV7-M/include -I../../Hal/Arm/isa/Thumb/include -I../../Hal/Arm/processor/Cortex-M/M4/M4/include -I../../Hal/Arm/support/include -I../../Hal/Arm/board/ST/DISCOVERY/32F469I/include -I../../Hal/Arm/processor/Cortex-M/M4/STM32F469NIH6/include -I../../Hal/memory/MemoryAreaMultiple/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/rspscheduler.o src/rspscheduler.c

${OBJECTDIR}/src/rspthread.o: src/rspthread.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../common/include -Iinclude -I../../Hal/Arm/architecture/v7/v7-M/ArchV7-M/include -I../../Hal/Arm/architecture/v7/v7-M/PmsrV7-M/include -I../../Hal/Arm/isa/Thumb/include -I../../Hal/Arm/processor/Cortex-M/M4/M4/include -I../../Hal/Arm/support/include -I../../Hal/Arm/board/ST/DISCOVERY/32F469I/include -I../../Hal/Arm/processor/Cortex-M/M4/STM32F469NIH6/include -I../../Hal/memory/MemoryAreaMultiple/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/rspthread.o src/rspthread.c

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
