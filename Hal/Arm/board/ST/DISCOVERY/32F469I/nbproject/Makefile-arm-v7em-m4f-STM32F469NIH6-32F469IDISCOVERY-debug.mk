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
RANLIB=ranlib
CC=$$PHAROS_ARM_M4_CC
CCC=g++
CXX=g++
FC=gfortran
AS=$$PHAROS_ARM_M4_CC

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=arm-v7em-m4f-STM32F469NIH6-32F469IDISCOVERY-debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/board.o \
	${OBJECTDIR}/src/sdram.o


# C Compiler Flags
CFLAGS=-O0 -g -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfpu=fpv4-sp-d16 -ffunction-sections -fdata-sections -Wall -Wextra -Wno-unused-parameter -Wno-unused-function

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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/lib32f469i.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/lib32f469i.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/lib32f469i.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/lib32f469i.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/lib32f469i.a

${OBJECTDIR}/src/board.o: src/board.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -Iinclude -I../../../../../../Kernel/common/include -I../../../../../../Kernel/rsp/include -I../../../../isa/Thumb/include -I../../../../processor/Cortex-M/M4/STM32F469NIH6/include -I../../../../architecture/v7/v7-M/ArchV7-M/include -I../../../../architecture/v7/v7-M/PmsrV7-M/include -I../../../../support/include -I../../../../../memory/MemoryAreaMultiple/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/board.o src/board.c

${OBJECTDIR}/src/sdram.o: src/sdram.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -Iinclude -I../../../../../../Kernel/common/include -I../../../../../../Kernel/rsp/include -I../../../../isa/Thumb/include -I../../../../processor/Cortex-M/M4/STM32F469NIH6/include -I../../../../architecture/v7/v7-M/ArchV7-M/include -I../../../../architecture/v7/v7-M/PmsrV7-M/include -I../../../../support/include -I../../../../../memory/MemoryAreaMultiple/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/sdram.o src/sdram.c

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
