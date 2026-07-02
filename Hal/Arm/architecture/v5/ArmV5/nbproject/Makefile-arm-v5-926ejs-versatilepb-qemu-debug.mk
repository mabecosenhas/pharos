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
	${OBJECTDIR}/src/interpartition.o \
	${OBJECTDIR}/src/interpartitionhigh.o \
	${OBJECTDIR}/src/interruptstack.o \
	${OBJECTDIR}/src/isrhandlerV5.o


# C Compiler Flags
CFLAGS=-O0 -march=armv5te -mtune=arm926ej-s -g -mfpu=vfp -mfloat-abi=softfp -ffunction-sections -fdata-sections -Wall -Wextra -Wno-unused-parameter -Wno-unused-function

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=-g -mfpu=vfp -mtune=arm926ej-s -mcpu=arm926ej-s -c -Iinclude -I../../../board/Qemu/Versatilepb/include -I../../../processor/ARM9/arm926ej-s/include -I../../../../../Kernel/common/include

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libarmv5.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libarmv5.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libarmv5.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libarmv5.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libarmv5.a

${OBJECTDIR}/src/interpartition.o: src/interpartition.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/interpartition.o src/interpartition.S

${OBJECTDIR}/src/interpartitionhigh.o: src/interpartitionhigh.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Iinclude -I../../../board/Qemu/Versatilepb/include -I../../../processor/ARM9/arm926ej-s/include -I../../../../../Kernel/common/include -I../../../architecture/v5/Mmuv5/include -I../../../support/include -I../../../../memory/MemoryAreaSingle/include -I../../../processor/support/PrimeCell/ArmVicPl190/include -I../../../../../Kernel/rsp/include -I../../../isa/Arm/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/interpartitionhigh.o src/interpartitionhigh.c

${OBJECTDIR}/src/interruptstack.o: src/interruptstack.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Iinclude -I../../../board/Qemu/Versatilepb/include -I../../../processor/ARM9/arm926ej-s/include -I../../../../../Kernel/common/include -I../../../architecture/v5/Mmuv5/include -I../../../support/include -I../../../../memory/MemoryAreaSingle/include -I../../../processor/support/PrimeCell/ArmVicPl190/include -I../../../../../Kernel/rsp/include -I../../../isa/Arm/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/interruptstack.o src/interruptstack.c

${OBJECTDIR}/src/isrhandlerV5.o: src/isrhandlerV5.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/isrhandlerV5.o src/isrhandlerV5.S

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
