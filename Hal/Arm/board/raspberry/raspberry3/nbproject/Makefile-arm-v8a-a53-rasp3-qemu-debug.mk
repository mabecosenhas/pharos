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
	${OBJECTDIR}/src/board.o \
	${OBJECTDIR}/src/mbox.o


# C Compiler Flags
CFLAGS=-O0 -g -mcpu=cortex-a53 -ffunction-sections -fdata-sections -Wall -Wextra -Wno-unused-parameter -Wno-unused-function

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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libraspberry3.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libraspberry3.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libraspberry3.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libraspberry3.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libraspberry3.a

${OBJECTDIR}/src/board.o: src/board.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../../../../../Kernel/common/include -Iinclude -I../../../processor/Cortex-A/A53/BCM2837/include -I../../../architecture/v8/Archv8-A/include -I../../../architecture/v8/MmuV8/include -I../../../isa/A64/include -I../../../../../Kernel/rmp/include -I../../../../memory/MemoryAreaSingle/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/board.o src/board.c

${OBJECTDIR}/src/mbox.o: src/mbox.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../../../../../Kernel/common/include -Iinclude -I../../../processor/Cortex-A/A53/BCM2837/include -I../../../architecture/v8/Archv8-A/include -I../../../architecture/v8/MmuV8/include -I../../../isa/A64/include -I../../../../../Kernel/rmp/include -I../../../../memory/MemoryAreaSingle/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/mbox.o src/mbox.c

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
