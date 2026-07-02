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
CCC=arm-none-eabi-gcc
CXX=arm-none-eabi-gcc
FC=gfortran
AS=$$PHAROS_ARM_M4_CC
AR=$$PHAROS_ARM_M4_ARCHIVER

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=arm-v7em-m4f-STM32F469NIH6-32F469IDISCOVERY-release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/arm.o \
	${OBJECTDIR}/src/syscallStackSize.o


# C Compiler Flags
CFLAGS=-O2 -g -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfpu=fpv4-sp-d16 -ffunction-sections -fdata-sections -Wall -Wextra -Wno-unused-parameter -Wno-unused-function

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=-g -c -mfpu=vfp -mtune=arm926ej-s -mcpu=arm926ej-s -Iinclude -I../processor/ARM9/arm926ej-s/include -I../../../Kernel/common/include

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsupport.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsupport.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsupport.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsupport.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libsupport.a

${OBJECTDIR}/src/arm.o: src/arm.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -I../../../Kernel/common/include -Iinclude -I../architecture/v7/v7-M/ArchV7-M/include -I../architecture/v7/v7-M/PmsrV7-M/include -I../isa/Thumb/include -I../board/ST/DISCOVERY/32F469I/include -I../processor/Cortex-M/M4/STM32F469NIH6/include -I../../memory/MemoryAreaMultiple/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/arm.o src/arm.c

${OBJECTDIR}/src/syscallStackSize.o: src/syscallStackSize.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -I../../../Kernel/common/include -Iinclude -I../architecture/v7/v7-M/ArchV7-M/include -I../architecture/v7/v7-M/PmsrV7-M/include -I../isa/Thumb/include -I../board/ST/DISCOVERY/32F469I/include -I../processor/Cortex-M/M4/STM32F469NIH6/include -I../../memory/MemoryAreaMultiple/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/syscallStackSize.o src/syscallStackSize.c

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
