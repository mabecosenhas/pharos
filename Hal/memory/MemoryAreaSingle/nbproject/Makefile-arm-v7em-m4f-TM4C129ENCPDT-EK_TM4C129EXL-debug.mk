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
	${OBJECTDIR}/src/protection.o \
	${OBJECTDIR}/src/protection2.o \
	${OBJECTDIR}/src/protection3.o \
	${OBJECTDIR}/src/protection4.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmemoryareasingle.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmemoryareasingle.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmemoryareasingle.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmemoryareasingle.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmemoryareasingle.a

${OBJECTDIR}/src/protection.o: src/protection.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../../../Kernel/common/include -I../../../Kernel/rsp/include -I../../Arm/support/include -Iinclude -I../../Arm/architecture/v7/v7-M/ArchV7-M/include -I../../Arm/architecture/v7/v7-M/PmsrV7-M/include -I../../Arm/board/TI/EK-TM4C129EXL/include -I../../Arm/processor/Cortex-M/M4/M4/include -I../../Arm/processor/Cortex-M/M4/TM4C129ENCPDT/include -I../../Arm/isa/Thumb/include -I../../../Kernel/clock/ClockPending/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/protection.o src/protection.c

${OBJECTDIR}/src/protection2.o: src/protection2.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../../../Kernel/common/include -I../../../Kernel/rsp/include -I../../Arm/support/include -Iinclude -I../../Arm/architecture/v7/v7-M/ArchV7-M/include -I../../Arm/architecture/v7/v7-M/PmsrV7-M/include -I../../Arm/board/TI/EK-TM4C129EXL/include -I../../Arm/processor/Cortex-M/M4/M4/include -I../../Arm/processor/Cortex-M/M4/TM4C129ENCPDT/include -I../../Arm/isa/Thumb/include -I../../../Kernel/clock/ClockPending/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/protection2.o src/protection2.c

${OBJECTDIR}/src/protection3.o: src/protection3.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../../../Kernel/common/include -I../../../Kernel/rsp/include -I../../Arm/support/include -Iinclude -I../../Arm/architecture/v7/v7-M/ArchV7-M/include -I../../Arm/architecture/v7/v7-M/PmsrV7-M/include -I../../Arm/board/TI/EK-TM4C129EXL/include -I../../Arm/processor/Cortex-M/M4/M4/include -I../../Arm/processor/Cortex-M/M4/TM4C129ENCPDT/include -I../../Arm/isa/Thumb/include -I../../../Kernel/clock/ClockPending/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/protection3.o src/protection3.c

${OBJECTDIR}/src/protection4.o: src/protection4.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../../../Kernel/common/include -I../../../Kernel/rsp/include -I../../Arm/support/include -Iinclude -I../../Arm/architecture/v7/v7-M/ArchV7-M/include -I../../Arm/architecture/v7/v7-M/PmsrV7-M/include -I../../Arm/board/TI/EK-TM4C129EXL/include -I../../Arm/processor/Cortex-M/M4/M4/include -I../../Arm/processor/Cortex-M/M4/TM4C129ENCPDT/include -I../../Arm/isa/Thumb/include -I../../../Kernel/clock/ClockPending/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/protection4.o src/protection4.c

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
