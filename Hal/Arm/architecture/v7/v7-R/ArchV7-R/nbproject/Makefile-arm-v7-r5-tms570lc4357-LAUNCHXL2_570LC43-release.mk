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
CND_CONF=arm-v7-r5-tms570lc4357-LAUNCHXL2_570LC43-release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/handlerDataAbort.o \
	${OBJECTDIR}/src/idle.o \
	${OBJECTDIR}/src/interpartition.o \
	${OBJECTDIR}/src/interpartitionhigh.o \
	${OBJECTDIR}/src/interruptstack.o \
	${OBJECTDIR}/src/performance.o \
	${OBJECTDIR}/src/start.o \
	${OBJECTDIR}/src/thread.o \
	${OBJECTDIR}/src/userMode.o


# C Compiler Flags
CFLAGS=-O2 -mcpu=cortex-r5 -mfpu=vfpv3-d16 -mbig-endian -ffunction-sections -fdata-sections -Wall -Wextra -Wno-unused-parameter -Wno-unused-function

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=-mcpu=cortex-r5 -mfpu=vfpv3-d16 -mbig-endian -g -c -I../../../../processor/Cortex-R/R5/tms570lc4357/include -I../../../../isa/Arm/include -I../../../../../../Kernel/common/include -Iinclude -I../../../../processor/Cortex-R/R5/R5/include

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libarchv7-r.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libarchv7-r.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libarchv7-r.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libarchv7-r.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libarchv7-r.a

${OBJECTDIR}/src/handlerDataAbort.o: src/handlerDataAbort.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/handlerDataAbort.o src/handlerDataAbort.S

${OBJECTDIR}/src/idle.o: src/idle.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/idle.o src/idle.S

${OBJECTDIR}/src/interpartition.o: src/interpartition.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/interpartition.o src/interpartition.S

${OBJECTDIR}/src/interpartitionhigh.o: src/interpartitionhigh.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../../../../processor/Cortex-R/R5/tms570lc4357/include -I../../../../isa/Arm/include -I../../../../../../Kernel/common/include -Iinclude -I../../../../processor/Cortex-R/R5/R5/include -I../../../../processor/Cortex-R/R5/tms570lc4357/include -I../../../../board/TI/LaunchXL2-570LC43/include -I../../../../support/include -I../PmsrV7/include -I../../../../manufacturer/ti/Vim/include -I../../../../../memory/MemoryAreaSingle/include -I../../../../../../Kernel/rsp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/interpartitionhigh.o src/interpartitionhigh.c

${OBJECTDIR}/src/interruptstack.o: src/interruptstack.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../../../../processor/Cortex-R/R5/tms570lc4357/include -I../../../../isa/Arm/include -I../../../../../../Kernel/common/include -Iinclude -I../../../../processor/Cortex-R/R5/R5/include -I../../../../processor/Cortex-R/R5/tms570lc4357/include -I../../../../board/TI/LaunchXL2-570LC43/include -I../../../../support/include -I../PmsrV7/include -I../../../../manufacturer/ti/Vim/include -I../../../../../memory/MemoryAreaSingle/include -I../../../../../../Kernel/rsp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/interruptstack.o src/interruptstack.c

${OBJECTDIR}/src/performance.o: src/performance.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/performance.o src/performance.S

${OBJECTDIR}/src/start.o: src/start.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/start.o src/start.S

${OBJECTDIR}/src/thread.o: src/thread.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../../../../processor/Cortex-R/R5/tms570lc4357/include -I../../../../isa/Arm/include -I../../../../../../Kernel/common/include -Iinclude -I../../../../processor/Cortex-R/R5/R5/include -I../../../../processor/Cortex-R/R5/tms570lc4357/include -I../../../../board/TI/LaunchXL2-570LC43/include -I../../../../support/include -I../PmsrV7/include -I../../../../manufacturer/ti/Vim/include -I../../../../../memory/MemoryAreaSingle/include -I../../../../../../Kernel/rsp/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/thread.o src/thread.c

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
