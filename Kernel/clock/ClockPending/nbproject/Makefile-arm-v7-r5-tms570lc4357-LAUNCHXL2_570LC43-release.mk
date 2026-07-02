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
CC=$$PHAROS_ARM_R5_BIG_CC
CCC=g++
CXX=g++
FC=gfortran
AS=as

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
	${OBJECTDIR}/src/clock.o


# C Compiler Flags
CFLAGS=-fno-exceptions -fno-unwind-tables -O2 -g -mcpu=cortex-r5 -mfpu=vfpv3-d16 -mbig-endian -Wall -Wextra -Wno-unused-parameter -Wno-unused-function

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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libclockpending.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libclockpending.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libclockpending.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libclockpending.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libclockpending.a

${OBJECTDIR}/src/clock.o: src/clock.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -Iinclude -I../../common/include -I../../rsp/include -I../../../Hal/memory/MemoryAreaSingle/include -I../../../Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I../../../Hal/Arm/processor/Cortex-R/R5/R5/include -I../../../Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I../../../Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I../../../Hal/Arm/isa/Arm/include -I../../../Hal/Arm/board/TI/LaunchXL2-570LC43/include -I../../../Hal/Arm/support/include -I../../../Hal/Arm/manufacturer/ti/Vim/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/clock.o src/clock.c

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
