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
CND_CONF=arm-v7em-m4f-TM4C129ENCPDT-EK_TM4C129EXL-debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/mpu.o


# C Compiler Flags
CFLAGS=-O0 -g -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfpu=fpv4-sp-d16 -ffunction-sections -fdata-sections -Wall -Wextra -Wno-unused-parameter -Wno-unused-function

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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libpmsrv7-m.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libpmsrv7-m.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libpmsrv7-m.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libpmsrv7-m.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libpmsrv7-m.a

${OBJECTDIR}/src/mpu.o: src/mpu.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -Iinclude -I../../../../../../Kernel/common/include -I../../../../support/include -I../../../../processor/Cortex-M/M4/M4/include -I../../../../processor/Cortex-M/M4/TM4C129ENCPDT/include -I../ArchV7-M/include -I../../../../isa/Thumb/include -I../../../../board/TI/EK-TM4C129EXL/include -I../../../../../../Kernel/rsp/include -I../../../../../memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/mpu.o src/mpu.c

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
