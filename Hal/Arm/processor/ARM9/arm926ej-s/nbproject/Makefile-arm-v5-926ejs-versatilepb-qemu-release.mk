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
CCC=arm-none-eabi-gcc
CXX=arm-none-eabi-gcc
FC=gfortran
AS=$$PHAROS_ARM_V5_CC
AR=$$PHAROS_ARM_V5_ARCHIVER

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=arm-v5-926ejs-versatilepb-qemu-release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/e71745e7/context.o \
	${OBJECTDIR}/src/clock.o \
	${OBJECTDIR}/src/thread.o


# C Compiler Flags
CFLAGS=-O2 -march=armv5te -mtune=arm926ej-s -g -mfpu=vfp -mfloat-abi=softfp -ffunction-sections -fdata-sections -Wall -Wextra -Wno-unused-parameter -Wno-unused-function

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=-g -mfpu=vfp -mtune=arm926ej-s -mcpu=arm926ej-s -c -Iinclude -I../../../../../Kernel/common/include

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libarm926ej-s.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libarm926ej-s.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libarm926ej-s.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libarm926ej-s.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libarm926ej-s.a

${OBJECTDIR}/_ext/e71745e7/context.o: ../../../processor/ARM9/arm926ej-s/src/context.S
	${MKDIR} -p ${OBJECTDIR}/_ext/e71745e7
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/_ext/e71745e7/context.o ../../../processor/ARM9/arm926ej-s/src/context.S

${OBJECTDIR}/src/clock.o: src/clock.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -Werror -DQEMU -I../../../support/include -I../../../../../Kernel/common/include -Iinclude -I../../../board/Qemu/Versatilepb/include -I../../support/PrimeCell/ArmVicPl190/include -I../../../../memory/Mmu/include -I../../../architecture/v5/Mmuv5/include -I../../support/ArmDualTimerSp804/include -I../../../isa/Arm/include -I../../../../../Kernel/rsp/include -I../../../../memory/MemoryAreaSingle/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/clock.o src/clock.c

${OBJECTDIR}/src/thread.o: src/thread.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -Werror -DQEMU -I../../../support/include -I../../../../../Kernel/common/include -Iinclude -I../../../board/Qemu/Versatilepb/include -I../../support/PrimeCell/ArmVicPl190/include -I../../../../memory/Mmu/include -I../../../architecture/v5/Mmuv5/include -I../../support/ArmDualTimerSp804/include -I../../../isa/Arm/include -I../../../../../Kernel/rsp/include -I../../../../memory/MemoryAreaSingle/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/thread.o src/thread.c

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
