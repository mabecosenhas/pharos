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
CND_CONF=arm-v7-r5-tms570lc4357-LAUNCHXL2_570LC43-debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/handlerFiqVim.o \
	${OBJECTDIR}/src/handlerIsrVim.o \
	${OBJECTDIR}/src/vim.o


# C Compiler Flags
CFLAGS=-O0 -g -mcpu=cortex-r5 -mfpu=vfpv3-d16 -mbig-endian -Wall -Wextra -Wno-unused-parameter -Wno-unused-function

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=-c -mcpu=cortex-r5 -mfpu=vfpv3-d16 -mbig-endian -I../../../../../Kernel/include -I../../../processor/Cortex-R/R5/tms570lc4357/include -Iinclude -I../../../processor/Cortex-R/R5/R5/include -I../../../support/include -I../../../isa/Arm/include -I../../../memory/Mpu/include -I../../../board/TI/LaunchXL2-570LC43/include -I../../../architecture/v7/v7-R/PmsrV7/include -I../../../architecture/v7/v7-R/ArchV7-R/include -I../../../../../Kernel/common/include

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libvim.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libvim.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libvim.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libvim.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libvim.a

${OBJECTDIR}/src/handlerFiqVim.o: src/handlerFiqVim.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/handlerFiqVim.o src/handlerFiqVim.S

${OBJECTDIR}/src/handlerIsrVim.o: src/handlerIsrVim.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/handlerIsrVim.o src/handlerIsrVim.S

${OBJECTDIR}/src/vim.o: src/vim.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I${PHAROS_FILE_PATH}/Kernel/common/include -I${PHAROS_FILE_PATH}/Hal/Arm/support/include -I${PHAROS_FILE_PATH}/Hal/Arm/processor/Cortex-R/R5/R5/include -I${PHAROS_FILE_PATH}/Hal/Arm/processor/Cortex-R/R5/tms570lc4357/include -I${PHAROS_FILE_PATH}/Hal/Arm/architecture/v7/v7-R/ArchV7-R/include -I${PHAROS_FILE_PATH}/Hal/Arm/architecture/v7/v7-R/PmsrV7/include -I${PHAROS_FILE_PATH}/Hal/Arm/board/TI/LaunchXL2-570LC43/include -I${PHAROS_FILE_PATH}/Hal/Arm/manufacturer/ti/Vim/include -I${PHAROS_FILE_PATH}/Hal/Arm/support/include -I${PHAROS_FILE_PATH}/Hal/Arm/isa/Arm/include -I${PHAROS_FILE_PATH}/Kernel/rsp/include -I${PHAROS_FILE_PATH}/Hal/memory/MemoryAreaSingle/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/vim.o src/vim.c

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
