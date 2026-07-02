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
RANLIB=$$PHAROS_ARM_AARCH64_RANLIB
CC=$$PHAROS_RISCV64_CC
CCC=g++
CXX=g++
FC=gfortran
AS=$$PHAROS_ARM_AARCH64_CC
AR=$$PHAROS_ARM_AARCH64_ARCHIVER

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=riscv64-virt-qemu-debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/stack.o


# C Compiler Flags
CFLAGS=-O0 -mcmodel=medany -ffunction-sections -fdata-sections -march=rv64imafdc -mabi=lp64d -g -Wall -Wextra -Wno-unused-parameter -Wno-unused-function

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=-c -mcpu=v7 -g -mcpu=arm926ej-s

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdescending.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdescending.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdescending.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdescending.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdescending.a

${OBJECTDIR}/src/stack.o: src/stack.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -Werror -I${PHAROS_FILE_PATH}/Kernel/common/include -I${PHAROS_FILE_PATH}/Hal/Risc-v/devices/sifive/Plic/include -I${PHAROS_FILE_PATH}/Hal/Risc-v/devices/sifive/Clint/include -I${PHAROS_FILE_PATH}/Hal/memory/MemoryAreaSingle/include -I${PHAROS_FILE_PATH}/Hal/devices/uart/Ns16556/include -I${PHAROS_FILE_PATH}/Hal/Risc-v/architecture/Riscv64/include -I${PHAROS_FILE_PATH}/Hal/Risc-v/architecture/RiscvSv39/include -I${PHAROS_FILE_PATH}/Hal/Risc-v/isa/riscvIsa32/include -I${PHAROS_FILE_PATH}/Hal/Risc-v/cpu/qemu/Virt/include -I${PHAROS_FILE_PATH}/Kernel/rmp/include -I${PHAROS_FILE_PATH}/Kernel/clock/ClockContinuous/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/stack.o src/stack.c

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
