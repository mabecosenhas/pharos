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
	${OBJECTDIR}/src/protection.o \
	${OBJECTDIR}/src/protection2.o \
	${OBJECTDIR}/src/protection3.o \
	${OBJECTDIR}/src/protection4.o


# C Compiler Flags
CFLAGS=-O2 -g -mcpu=cortex-a53 -Wall -Wextra -Wno-unused-parameter -Wno-unused-function

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=-c

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmemoryareasingle.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmemoryareasingle.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmemoryareasingle.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmemoryareasingle.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmemoryareasingle.a

${OBJECTDIR}/src/protection.o: src/protection.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../../../Kernel/common/include -I../../Arm/support/include -Iinclude -I../../Arm/architecture/v8/Archv8-A/include -I../../Arm/architecture/v8/MmuV8/include -I../../Arm/board/raspberry/raspberry3/include -I../../Arm/isa/A64/include -I../../Arm/processor/Cortex-A/A53/BCM2837/include -I../../../Kernel/clock/ClockPending/include -I../../../Kernel/rmp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/protection.o src/protection.c

${OBJECTDIR}/src/protection2.o: src/protection2.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../../../Kernel/common/include -I../../Arm/support/include -Iinclude -I../../Arm/architecture/v8/Archv8-A/include -I../../Arm/architecture/v8/MmuV8/include -I../../Arm/board/raspberry/raspberry3/include -I../../Arm/isa/A64/include -I../../Arm/processor/Cortex-A/A53/BCM2837/include -I../../../Kernel/clock/ClockPending/include -I../../../Kernel/rmp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/protection2.o src/protection2.c

${OBJECTDIR}/src/protection3.o: src/protection3.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../../../Kernel/common/include -I../../Arm/support/include -Iinclude -I../../Arm/architecture/v8/Archv8-A/include -I../../Arm/architecture/v8/MmuV8/include -I../../Arm/board/raspberry/raspberry3/include -I../../Arm/isa/A64/include -I../../Arm/processor/Cortex-A/A53/BCM2837/include -I../../../Kernel/clock/ClockPending/include -I../../../Kernel/rmp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/protection3.o src/protection3.c

${OBJECTDIR}/src/protection4.o: src/protection4.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../../../Kernel/common/include -I../../Arm/support/include -Iinclude -I../../Arm/architecture/v8/Archv8-A/include -I../../Arm/architecture/v8/MmuV8/include -I../../Arm/board/raspberry/raspberry3/include -I../../Arm/isa/A64/include -I../../Arm/processor/Cortex-A/A53/BCM2837/include -I../../../Kernel/clock/ClockPending/include -I../../../Kernel/rmp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/protection4.o src/protection4.c

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
