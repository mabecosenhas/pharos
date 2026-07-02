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
AS=$$PHAROS_ARM_V5_CC
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
	${OBJECTDIR}/src/board.o \
	${OBJECTDIR}/src/interruptcontroller.o


# C Compiler Flags
CFLAGS=-O0 -g -mcpu=arm926ej-s -mfloat-abi=softfp -Wall -Wextra -Wno-unused-parameter -Wno-unused-function

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=-g -c -mcpu=arm926ej-s -Iinclude -I../../../../../Kernel/include -I../../../processor/ARM9/arm926ej-s/include

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libversatilepb.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libversatilepb.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libversatilepb.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libversatilepb.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libversatilepb.a

${OBJECTDIR}/src/board.o: src/board.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -Werror -I../../../../../Kernel/common/include -I../../../support/include -Iinclude -I../../../support/include -I../../../processor/support/ArmDualTimerSp804/include -I../../../processor/support/PrimeCell/ArmVicPl190/include -I../../../processor/ARM9/arm926ej-s/include -I../../../../memory/Mmu/include -I../../../architecture/v5/Mmuv5/include -I../../../isa/Arm/include -I../../../../../Kernel/rsp/include -I../../../../memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/board.o src/board.c

${OBJECTDIR}/src/interruptcontroller.o: src/interruptcontroller.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -Werror -I../../../../../Kernel/common/include -I../../../support/include -Iinclude -I../../../support/include -I../../../processor/support/ArmDualTimerSp804/include -I../../../processor/support/PrimeCell/ArmVicPl190/include -I../../../processor/ARM9/arm926ej-s/include -I../../../../memory/Mmu/include -I../../../architecture/v5/Mmuv5/include -I../../../isa/Arm/include -I../../../../../Kernel/rsp/include -I../../../../memory/MemoryAreaSingle/include -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/interruptcontroller.o src/interruptcontroller.c

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
