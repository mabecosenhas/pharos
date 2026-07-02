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
	${OBJECTDIR}/src/cpu.o \
	${OBJECTDIR}/src/gpio.o \
	${OBJECTDIR}/src/sysctl.o \
	${OBJECTDIR}/src/timer.o \
	${OBJECTDIR}/src/uart.o


# C Compiler Flags
CFLAGS=-O0 -g -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfpu=fpv4-sp-d16 -Wall -Wextra -Wno-unused-parameter -Wno-unused-function

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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libtm4c129encpdt.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libtm4c129encpdt.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libtm4c129encpdt.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libtm4c129encpdt.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libtm4c129encpdt.a

${OBJECTDIR}/src/cpu.o: src/cpu.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../../../../architecture/v7/v7-M/ArchV7-M/include -I../../../../architecture/v7/v7-M/PmsrV7-M/include -Iinclude -I../../../../../../Kernel/common/include -I../../../../support/include -I../../../../isa/Thumb/include -I../../../../board/TI/EK-TM4C129EXL/include -I../../../../../../Kernel/rsp/include -I../../../../../memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/cpu.o src/cpu.c

${OBJECTDIR}/src/gpio.o: src/gpio.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../../../../architecture/v7/v7-M/ArchV7-M/include -I../../../../architecture/v7/v7-M/PmsrV7-M/include -Iinclude -I../../../../../../Kernel/common/include -I../../../../support/include -I../../../../isa/Thumb/include -I../../../../board/TI/EK-TM4C129EXL/include -I../../../../../../Kernel/rsp/include -I../../../../../memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/gpio.o src/gpio.c

${OBJECTDIR}/src/sysctl.o: src/sysctl.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../../../../architecture/v7/v7-M/ArchV7-M/include -I../../../../architecture/v7/v7-M/PmsrV7-M/include -Iinclude -I../../../../../../Kernel/common/include -I../../../../support/include -I../../../../isa/Thumb/include -I../../../../board/TI/EK-TM4C129EXL/include -I../../../../../../Kernel/rsp/include -I../../../../../memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/sysctl.o src/sysctl.c

${OBJECTDIR}/src/timer.o: src/timer.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../../../../architecture/v7/v7-M/ArchV7-M/include -I../../../../architecture/v7/v7-M/PmsrV7-M/include -Iinclude -I../../../../../../Kernel/common/include -I../../../../support/include -I../../../../isa/Thumb/include -I../../../../board/TI/EK-TM4C129EXL/include -I../../../../../../Kernel/rsp/include -I../../../../../memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/timer.o src/timer.c

${OBJECTDIR}/src/uart.o: src/uart.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../../../../architecture/v7/v7-M/ArchV7-M/include -I../../../../architecture/v7/v7-M/PmsrV7-M/include -Iinclude -I../../../../../../Kernel/common/include -I../../../../support/include -I../../../../isa/Thumb/include -I../../../../board/TI/EK-TM4C129EXL/include -I../../../../../../Kernel/rsp/include -I../../../../../memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/uart.o src/uart.c

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
