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
	${OBJECTDIR}/src/context.o \
	${OBJECTDIR}/src/cpu.o \
	${OBJECTDIR}/src/esm.o \
	${OBJECTDIR}/src/flash.o \
	${OBJECTDIR}/src/mem.o \
	${OBJECTDIR}/src/mux.o \
	${OBJECTDIR}/src/peripheral.o \
	${OBJECTDIR}/src/pll.o \
	${OBJECTDIR}/src/rti.o \
	${OBJECTDIR}/src/sci.o


# C Compiler Flags
CFLAGS=-O0 -g -mcpu=cortex-r5 -mfpu=vfpv3-d16 -mbig-endian -Wall -Wextra -Wno-unused-parameter -Wno-unused-function

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=-c -mcpu=cortex-r5 -mfpu=vfpv3-d16 -mbig-endian -I../../../../../Kernel/include -I../../../../memory/Mpu/include -Iinclude -I../../../../manufacturer/ti/Vim/include -I../tms570lc4357/include -I../../../../../../Kernel/include -I../R5/include -I../../../../architecture/v7/v7-R/ArchV7-R/include -I../../../../../../Kernel/common/include

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libtms570lc4357.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libtms570lc4357.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libtms570lc4357.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libtms570lc4357.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libtms570lc4357.a

${OBJECTDIR}/src/context.o: src/context.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/context.o src/context.S

${OBJECTDIR}/src/cpu.o: src/cpu.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -Werror -I../../../../isa/Arm/include -I../../../../memory/Mpu/include -I../R5/include -I../../../../../../Kernel/common/include -I../../../../architecture/v7/v7-R/ArchV7-R/include -I../../../../architecture/v7/v7-R/PmsrV7/include -Iinclude -I../../../../manufacturer/ti/Vim/include -I../../../../support/include -I../../../../board/TI/LaunchXL2-570LC43/include -I../../../../manufacturer/ti/Vim/include -I../../../../../../Kernel/rsp/include -I../../../../../memory/MemoryAreaSingle/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/cpu.o src/cpu.c

${OBJECTDIR}/src/esm.o: src/esm.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -Werror -I../../../../isa/Arm/include -I../../../../memory/Mpu/include -I../R5/include -I../../../../../../Kernel/common/include -I../../../../architecture/v7/v7-R/ArchV7-R/include -I../../../../architecture/v7/v7-R/PmsrV7/include -Iinclude -I../../../../manufacturer/ti/Vim/include -I../../../../support/include -I../../../../board/TI/LaunchXL2-570LC43/include -I../../../../manufacturer/ti/Vim/include -I../../../../../../Kernel/rsp/include -I../../../../../memory/MemoryAreaSingle/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/esm.o src/esm.c

${OBJECTDIR}/src/flash.o: src/flash.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -Werror -I../../../../isa/Arm/include -I../../../../memory/Mpu/include -I../R5/include -I../../../../../../Kernel/common/include -I../../../../architecture/v7/v7-R/ArchV7-R/include -I../../../../architecture/v7/v7-R/PmsrV7/include -Iinclude -I../../../../manufacturer/ti/Vim/include -I../../../../support/include -I../../../../board/TI/LaunchXL2-570LC43/include -I../../../../manufacturer/ti/Vim/include -I../../../../../../Kernel/rsp/include -I../../../../../memory/MemoryAreaSingle/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/flash.o src/flash.c

${OBJECTDIR}/src/mem.o: src/mem.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -Werror -I../../../../isa/Arm/include -I../../../../memory/Mpu/include -I../R5/include -I../../../../../../Kernel/common/include -I../../../../architecture/v7/v7-R/ArchV7-R/include -I../../../../architecture/v7/v7-R/PmsrV7/include -Iinclude -I../../../../manufacturer/ti/Vim/include -I../../../../support/include -I../../../../board/TI/LaunchXL2-570LC43/include -I../../../../manufacturer/ti/Vim/include -I../../../../../../Kernel/rsp/include -I../../../../../memory/MemoryAreaSingle/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/mem.o src/mem.c

${OBJECTDIR}/src/mux.o: src/mux.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -Werror -I../../../../isa/Arm/include -I../../../../memory/Mpu/include -I../R5/include -I../../../../../../Kernel/common/include -I../../../../architecture/v7/v7-R/ArchV7-R/include -I../../../../architecture/v7/v7-R/PmsrV7/include -Iinclude -I../../../../manufacturer/ti/Vim/include -I../../../../support/include -I../../../../board/TI/LaunchXL2-570LC43/include -I../../../../manufacturer/ti/Vim/include -I../../../../../../Kernel/rsp/include -I../../../../../memory/MemoryAreaSingle/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/mux.o src/mux.c

${OBJECTDIR}/src/peripheral.o: src/peripheral.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -Werror -I../../../../isa/Arm/include -I../../../../memory/Mpu/include -I../R5/include -I../../../../../../Kernel/common/include -I../../../../architecture/v7/v7-R/ArchV7-R/include -I../../../../architecture/v7/v7-R/PmsrV7/include -Iinclude -I../../../../manufacturer/ti/Vim/include -I../../../../support/include -I../../../../board/TI/LaunchXL2-570LC43/include -I../../../../manufacturer/ti/Vim/include -I../../../../../../Kernel/rsp/include -I../../../../../memory/MemoryAreaSingle/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/peripheral.o src/peripheral.c

${OBJECTDIR}/src/pll.o: src/pll.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -Werror -I../../../../isa/Arm/include -I../../../../memory/Mpu/include -I../R5/include -I../../../../../../Kernel/common/include -I../../../../architecture/v7/v7-R/ArchV7-R/include -I../../../../architecture/v7/v7-R/PmsrV7/include -Iinclude -I../../../../manufacturer/ti/Vim/include -I../../../../support/include -I../../../../board/TI/LaunchXL2-570LC43/include -I../../../../manufacturer/ti/Vim/include -I../../../../../../Kernel/rsp/include -I../../../../../memory/MemoryAreaSingle/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/pll.o src/pll.c

${OBJECTDIR}/src/rti.o: src/rti.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -Werror -I../../../../isa/Arm/include -I../../../../memory/Mpu/include -I../R5/include -I../../../../../../Kernel/common/include -I../../../../architecture/v7/v7-R/ArchV7-R/include -I../../../../architecture/v7/v7-R/PmsrV7/include -Iinclude -I../../../../manufacturer/ti/Vim/include -I../../../../support/include -I../../../../board/TI/LaunchXL2-570LC43/include -I../../../../manufacturer/ti/Vim/include -I../../../../../../Kernel/rsp/include -I../../../../../memory/MemoryAreaSingle/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/rti.o src/rti.c

${OBJECTDIR}/src/sci.o: src/sci.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -Werror -I../../../../isa/Arm/include -I../../../../memory/Mpu/include -I../R5/include -I../../../../../../Kernel/common/include -I../../../../architecture/v7/v7-R/ArchV7-R/include -I../../../../architecture/v7/v7-R/PmsrV7/include -Iinclude -I../../../../manufacturer/ti/Vim/include -I../../../../support/include -I../../../../board/TI/LaunchXL2-570LC43/include -I../../../../manufacturer/ti/Vim/include -I../../../../../../Kernel/rsp/include -I../../../../../memory/MemoryAreaSingle/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/sci.o src/sci.c

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
