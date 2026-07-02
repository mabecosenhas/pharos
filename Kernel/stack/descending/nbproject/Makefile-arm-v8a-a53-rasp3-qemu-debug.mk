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
	${OBJECTDIR}/src/stack.o


# C Compiler Flags
CFLAGS=-fno-common -O0 -g -mcpu=cortex-a53 -ffunction-sections -fdata-sections -Wall -Wextra -Wno-unused-parameter -Wno-unused-function

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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdescending.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdescending.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdescending.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdescending.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdescending.a

${OBJECTDIR}/src/stack.o: src/stack.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -Werror -I${PHAROS_FILE_PATH}/Kernel/common/include -I${PHAROS_FILE_PATH}/Hal/Arm/architecture/v8/Archv8-A/include -I${PHAROS_FILE_PATH}/Hal/Arm/architecture/v8/MmuV8/include -I${PHAROS_FILE_PATH}/Hal/Arm/isa/A64/include -I${PHAROS_FILE_PATH}/Hal/Arm/board/raspberry/raspberry3/include -I${PHAROS_FILE_PATH}/Hal/Arm/processor/Cortex-A/A53/BCM2837/include -I${PHAROS_FILE_PATH}/Hal/Arm/architecture/v8/GenericTimer/include -I${PHAROS_FILE_PATH}/Kernel/rmp/include -I${PHAROS_FILE_PATH}/Hal/memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/stack.o src/stack.c

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
