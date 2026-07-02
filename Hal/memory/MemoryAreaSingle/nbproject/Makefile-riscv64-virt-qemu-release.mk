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
RANLIB=$$PHAROS_RISCV64_RANLIB
CC=$$PHAROS_RISCV64_CC
CCC=g++
CXX=g++
FC=gfortran
AS=$$PHAROS_ARM_R5_BIG_CC
AR=$$PHAROS_RISCV64_ARCHIVER

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=riscv64-virt-qemu-release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/protection.o \
	${OBJECTDIR}/src/protection2.o \
	${OBJECTDIR}/src/protection3.o \
	${OBJECTDIR}/src/protection4.o


# C Compiler Flags
CFLAGS=-g -O2 -mcmodel=medany -ffunction-sections -fdata-sections -march=rv64imafdc -mabi=lp64d -Wall -Wextra -Wno-unused-parameter -Wno-unused-function

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=-c

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmemoryareasingle.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmemoryareasingle.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmemoryareasingle.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmemoryareasingle.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libmemoryareasingle.a

${OBJECTDIR}/src/protection.o: src/protection.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../../../Kernel/common/include -Iinclude -I../../Risc-v/isa/riscvIsa32/include -I../../Risc-v/architecture/Riscv64/include -I../../Risc-v/architecture/RiscvSv39/include -I../../../Kernel/rmp/include -I../../../Kernel/clock/ClockContinuous/include -I../../Risc-v/cpu/qemu/Virt/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/protection.o src/protection.c

${OBJECTDIR}/src/protection2.o: src/protection2.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../../../Kernel/common/include -Iinclude -I../../Risc-v/isa/riscvIsa32/include -I../../Risc-v/architecture/Riscv64/include -I../../Risc-v/architecture/RiscvSv39/include -I../../../Kernel/rmp/include -I../../../Kernel/clock/ClockContinuous/include -I../../Risc-v/cpu/qemu/Virt/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/protection2.o src/protection2.c

${OBJECTDIR}/src/protection3.o: src/protection3.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../../../Kernel/common/include -Iinclude -I../../Risc-v/isa/riscvIsa32/include -I../../Risc-v/architecture/Riscv64/include -I../../Risc-v/architecture/RiscvSv39/include -I../../../Kernel/rmp/include -I../../../Kernel/clock/ClockContinuous/include -I../../Risc-v/cpu/qemu/Virt/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/protection3.o src/protection3.c

${OBJECTDIR}/src/protection4.o: src/protection4.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../../../Kernel/common/include -Iinclude -I../../Risc-v/isa/riscvIsa32/include -I../../Risc-v/architecture/Riscv64/include -I../../Risc-v/architecture/RiscvSv39/include -I../../../Kernel/rmp/include -I../../../Kernel/clock/ClockContinuous/include -I../../Risc-v/cpu/qemu/Virt/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/protection4.o src/protection4.c

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
