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
RANLIB=$$PHAROS_ARM_V5_RANLIB
CC=$$PHAROS_ARM_V5_CC
CCC=g++
CXX=g++
FC=gfortran
AS=as
AR=$$PHAROS_ARM_V5_ARCHIVER

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=arm-v5-926ejs-versatilepb-qemu-debug
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
CFLAGS=-O0 -g -mfpu=vfp -mcpu=arm926ej-s -mfloat-abi=softfp -Wall -Wextra -Wno-unused-parameter -Wno-unused-function

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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmemoryareasingle.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmemoryareasingle.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmemoryareasingle.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmemoryareasingle.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmemoryareasingle.a

${OBJECTDIR}/src/protection.o: src/protection.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../../../Kernel/common/include -I../../../Kernel/rsp/include -I../../Arm/architecture/v5/ArmV5/include -I../../Arm/architecture/v5/Mmuv5/include -I../../Arm/board/Qemu/Versatilepb/include -I../../Arm/isa/Arm/include -I../../Arm/support/include -I../../Arm/processor/ARM9/arm926ej-s/include -Iinclude -I../../Arm/processor/support/PrimeCell/ArmVicPl190/include -I../../../Kernel/clock/ClockPending/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/protection.o src/protection.c

${OBJECTDIR}/src/protection2.o: src/protection2.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../../../Kernel/common/include -I../../../Kernel/rsp/include -I../../Arm/architecture/v5/ArmV5/include -I../../Arm/architecture/v5/Mmuv5/include -I../../Arm/board/Qemu/Versatilepb/include -I../../Arm/isa/Arm/include -I../../Arm/support/include -I../../Arm/processor/ARM9/arm926ej-s/include -Iinclude -I../../Arm/processor/support/PrimeCell/ArmVicPl190/include -I../../../Kernel/clock/ClockPending/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/protection2.o src/protection2.c

${OBJECTDIR}/src/protection3.o: src/protection3.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../../../Kernel/common/include -I../../../Kernel/rsp/include -I../../Arm/architecture/v5/ArmV5/include -I../../Arm/architecture/v5/Mmuv5/include -I../../Arm/board/Qemu/Versatilepb/include -I../../Arm/isa/Arm/include -I../../Arm/support/include -I../../Arm/processor/ARM9/arm926ej-s/include -Iinclude -I../../Arm/processor/support/PrimeCell/ArmVicPl190/include -I../../../Kernel/clock/ClockPending/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/protection3.o src/protection3.c

${OBJECTDIR}/src/protection4.o: src/protection4.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../../../Kernel/common/include -I../../../Kernel/rsp/include -I../../Arm/architecture/v5/ArmV5/include -I../../Arm/architecture/v5/Mmuv5/include -I../../Arm/board/Qemu/Versatilepb/include -I../../Arm/isa/Arm/include -I../../Arm/support/include -I../../Arm/processor/ARM9/arm926ej-s/include -Iinclude -I../../Arm/processor/support/PrimeCell/ArmVicPl190/include -I../../../Kernel/clock/ClockPending/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/protection4.o src/protection4.c

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
