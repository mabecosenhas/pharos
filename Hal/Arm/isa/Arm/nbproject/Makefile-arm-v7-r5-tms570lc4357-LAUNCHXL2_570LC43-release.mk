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
RANLIB=$$PHAROS_ARM_R5_BIG_RANLIB
CC=$$PHAROS_ARM_R5_BIG_CC
CCC=g++
CXX=g++
FC=gfortran
AS=$$PHAROS_ARM_R5_BIG_CC
AR=$$PHAROS_ARM_R5_BIG_ARCHIVER

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
	${OBJECTDIR}/src/cache.o \
	${OBJECTDIR}/src/fpu.o \
	${OBJECTDIR}/src/interrupts.o \
	${OBJECTDIR}/src/io.o \
	${OBJECTDIR}/src/misc.o \
	${OBJECTDIR}/src/stack.o \
	${OBJECTDIR}/src/syscall.o


# C Compiler Flags
CFLAGS=-O2 -mcpu=cortex-r5 -mfpu=vfpv3-d16 -mbig-endian -ffunction-sections -fdata-sections -Wall -Wextra -Wno-unused-parameter -Wno-unused-function

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=-c -mcpu=cortex-r5 -mfpu=vfpv3-d16 -mbig-endian -Iinclude -I../../processor/Cortex-R/R5/tms570lc4357/include -I../../processor/Cortex-R/R5/R5/include -I../../architecture/v7/v7-R/ArchV7-R/include -I../../architecture/v7/v7-R/PmsrV7/include -I../../../../Kernel/common/include

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libarm.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libarm.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libarm.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libarm.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libarm.a

${OBJECTDIR}/src/cache.o: src/cache.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/cache.o src/cache.S

${OBJECTDIR}/src/fpu.o: src/fpu.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/fpu.o src/fpu.S

${OBJECTDIR}/src/interrupts.o: src/interrupts.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/interrupts.o src/interrupts.S

${OBJECTDIR}/src/io.o: src/io.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -Iinclude -I../../processor/Cortex-R/R5/tms570lc4357/include -I../../processor/Cortex-R/R5/R5/include -I../../architecture/v7/v7-R/ArchV7-R/include -I../../architecture/v7/v7-R/PmsrV7/include -I../../../../Kernel/common/include -I../../support/include -I../../manufacturer/ti/Vim/include -I../../../memory/MemoryAreaSingle/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/io.o src/io.c

${OBJECTDIR}/src/misc.o: src/misc.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/misc.o src/misc.S

${OBJECTDIR}/src/stack.o: src/stack.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/stack.o src/stack.S

${OBJECTDIR}/src/syscall.o: src/syscall.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/syscall.o src/syscall.S

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
