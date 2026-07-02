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
	${OBJECTDIR}/_ext/7cc14b06/mmu.o \
	${OBJECTDIR}/_ext/e71745e7/handlerDataAbort.o \
	${OBJECTDIR}/src/channel.o \
	${OBJECTDIR}/src/criticalSection.o \
	${OBJECTDIR}/src/div.o \
	${OBJECTDIR}/src/handlerSwi.o \
	${OBJECTDIR}/src/hwqueue.o \
	${OBJECTDIR}/src/ipcall.o \
	${OBJECTDIR}/src/mmuV5.o \
	${OBJECTDIR}/src/resource.o \
	${OBJECTDIR}/src/start.o \
	${OBJECTDIR}/src/userMode.o


# C Compiler Flags
CFLAGS=-O0 -march=armv5te -mtune=arm926ej-s -g -mfpu=vfp -mfloat-abi=softfp -ffunction-sections -fdata-sections -Wall -Wextra -Wno-unused-parameter -Wno-unused-function

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=-c -g -mfpu=vfp -mtune=arm926ej-s -mcpu=arm926ej-s -I../../../../../Kernel/common/include/ -I../../../support/include -I../../../../memory/Mmu/include -Iinclude -I../../../board/Qemu/Versatilepb/include -I../../../processor/ARM9/arm926ej-s/include -I../../../processor/support/PrimeCell/ArmVicPl190/include -I../../../isa/Arm/include

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmmuv5.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmmuv5.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmmuv5.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmmuv5.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmmuv5.a

${OBJECTDIR}/_ext/7cc14b06/mmu.o: ../../../architecture/v5/Mmuv5/src/mmu.S
	${MKDIR} -p ${OBJECTDIR}/_ext/7cc14b06
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/_ext/7cc14b06/mmu.o ../../../architecture/v5/Mmuv5/src/mmu.S

${OBJECTDIR}/_ext/e71745e7/handlerDataAbort.o: ../../../processor/ARM9/arm926ej-s/src/handlerDataAbort.S
	${MKDIR} -p ${OBJECTDIR}/_ext/e71745e7
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/_ext/e71745e7/handlerDataAbort.o ../../../processor/ARM9/arm926ej-s/src/handlerDataAbort.S

${OBJECTDIR}/src/channel.o: src/channel.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/channel.o src/channel.S

${OBJECTDIR}/src/criticalSection.o: src/criticalSection.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/criticalSection.o src/criticalSection.S

${OBJECTDIR}/src/div.o: src/div.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/div.o src/div.S

${OBJECTDIR}/src/handlerSwi.o: src/handlerSwi.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/handlerSwi.o src/handlerSwi.S

${OBJECTDIR}/src/hwqueue.o: src/hwqueue.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/hwqueue.o src/hwqueue.S

${OBJECTDIR}/src/ipcall.o: src/ipcall.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/ipcall.o src/ipcall.S

${OBJECTDIR}/src/mmuV5.o: src/mmuV5.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -Werror -I../../../../../Kernel/common/include/ -I../../../support/include -I../../../../memory/Mmu/include -Iinclude -I../../../board/Qemu/Versatilepb/include -I../../../processor/ARM9/arm926ej-s/include -I../../../processor/support/PrimeCell/ArmVicPl190/include -I../../../isa/Arm/include -I../../../../memory/MemoryAreaSingle/include -I../../../../../Kernel/rsp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/mmuV5.o src/mmuV5.c

${OBJECTDIR}/src/resource.o: src/resource.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/resource.o src/resource.S

${OBJECTDIR}/src/start.o: src/start.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/start.o src/start.S

${OBJECTDIR}/src/userMode.o: src/userMode.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/userMode.o src/userMode.S

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
