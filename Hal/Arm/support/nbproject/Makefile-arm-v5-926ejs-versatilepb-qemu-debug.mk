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
CND_CONF=arm-v5-926ejs-versatilepb-qemu-debug
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
CFLAGS=-O0 -march=armv5te -mtune=arm926ej-s -g -mfpu=vfp -mfloat-abi=softfp -ffunction-sections -fdata-sections -Wall -Wextra -Wno-unused-parameter -Wno-unused-function

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=-g -c -mfpu=vfp -mtune=arm926ej-s -mcpu=arm926ej-s -Iinclude -I../processor/ARM9/arm926ej-s/include -I../../../Kernel/include

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
	$(COMPILE.c) -g -I../../../Kernel/common/include -Iinclude -I../board/Qemu/Versatilepb/include -I../processor/support/PrimeCell/ArmVicPl190/include -I../processor/ARM9/arm926ej-s/include -I../architecture/v5/Mmuv5/include -I../processor/ARM9/arm926ej-s/include -I../../../Kernel/include -I../isa/Arm/include -I../../memory/MemoryAreaSingle/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/arm.o src/arm.c

${OBJECTDIR}/src/syscallStackSize.o: src/syscallStackSize.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -I../../../Kernel/common/include -Iinclude -I../board/Qemu/Versatilepb/include -I../processor/support/PrimeCell/ArmVicPl190/include -I../processor/ARM9/arm926ej-s/include -I../architecture/v5/Mmuv5/include -I../processor/ARM9/arm926ej-s/include -I../../../Kernel/include -I../isa/Arm/include -I../../memory/MemoryAreaSingle/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/syscallStackSize.o src/syscallStackSize.c

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
