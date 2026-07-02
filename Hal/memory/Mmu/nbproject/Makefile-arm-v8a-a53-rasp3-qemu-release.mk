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
	${OBJECTDIR}/src/mmu.o


# C Compiler Flags
CFLAGS=-O2 -g -mcpu=cortex-a53

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=-c -g -mcpu=cortex-a53

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmmu.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmmu.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmmu.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmmu.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmmu.a

${OBJECTDIR}/src/mmu.o: src/mmu.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../../../Kernel/common/include -I../../../../memory/Mmu/include -I../../Arm/support/include -I../../Arm/processor/Cortex-A/A53/BCM2837/include -Iinclude -I../../Arm/architecture/v8/Archv8-A/include -I../../Arm/architecture/v8/MmuV8/include -I../../Arm/isa/A64/include -I../../Arm/board/raspberry/raspberry3/include -I../MemoryAreaSingle/include -I../../../Kernel/rmp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/mmu.o src/mmu.c

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
