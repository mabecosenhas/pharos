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
AS=as
AR=$$PHAROS_RISCV64_ARCHIVER

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
	${OBJECTDIR}/src/board.o \
	${OBJECTDIR}/src/cpu.o


# C Compiler Flags
CFLAGS=-g -O0 -mcmodel=medany -ffunction-sections -fdata-sections -march=rv64imafdc -mabi=lp64d -Wall -Wextra -Wno-unused-parameter -Wno-unused-function

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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libvirt.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libvirt.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libvirt.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libvirt.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libvirt.a

${OBJECTDIR}/src/board.o: src/board.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../../../../Kernel/common/include -I../../../../../Kernel/rmp/include -I../../../architecture/Riscv64/include -I../../../architecture/RiscvSv39/include -I../../../../devices/uart/Ns16556/include -I../../../../memory/MemoryAreaSingle/include -I../../../isa/riscvIsa32/include -I../../../devices/sifive/Clint/include -I../../../devices/sifive/Plic/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/board.o src/board.c

${OBJECTDIR}/src/cpu.o: src/cpu.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -Werror -Iinclude -I../../../../../Kernel/common/include -I../../../../../Kernel/rmp/include -I../../../architecture/Riscv64/include -I../../../architecture/RiscvSv39/include -I../../../../devices/uart/Ns16556/include -I../../../../memory/MemoryAreaSingle/include -I../../../isa/riscvIsa32/include -I../../../devices/sifive/Clint/include -I../../../devices/sifive/Plic/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/cpu.o src/cpu.c

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
