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
RANLIB=armeb-eabi-ranlib
CC=armeb-eabi-gcc
CCC=g++
CXX=g++
FC=gfortran
AS=armeb-eabi-gcc
AR=armeb-eabi-ar

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=arm-v7-r5-tms570lc4357-LAUNCHXL2_570LC43-debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/vicPl192.o


# C Compiler Flags
CFLAGS=-g -O0 -mcpu=cortex-r5 -mfpu=vfpv3-d16 -mbig-endian

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=-mcpu=cortex-r5 -mfpu=vfpv3-d16 -mbig-endian -Iinclude -I../../../../../../Kernel/include -I../../../../isa/Arm/include

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libarmvicpl192.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libarmvicpl192.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libarmvicpl192.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libarmvicpl192.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libarmvicpl192.a

${OBJECTDIR}/src/vicPl192.o: src/vicPl192.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Iinclude -I../../../../../../Kernel/include -I../../../../board/TI/LaunchXL2-570LC43/include -I../../../../support/include -I../../../Cortex-R/R5/tms570lc4357/include -I../../../../../memory/Mpu/include -I../../../../isa/Arm/include -I../../../Cortex-R/R5/shared/include -I../../../../memory/Mpu/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/vicPl192.o src/vicPl192.c

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
