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
AS=$$PHAROS_RISCV64_CC
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
	${OBJECTDIR}/src/mmuLow.o \
	${OBJECTDIR}/src/riscvmmu.o


# C Compiler Flags
CFLAGS=-g -O0 -mcmodel=medany -ffunction-sections -fdata-sections -march=rv64imafdc -mabi=lp64d -Wall -Wextra -Wno-unused-parameter -Wno-unused-function

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=-c -mcmodel=medany -ffunction-sections -fdata-sections -march=rv64imafdc -mabi=lp64d -g

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libriscvsv39.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libriscvsv39.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libriscvsv39.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libriscvsv39.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libriscvsv39.a

${OBJECTDIR}/src/mmuLow.o: src/mmuLow.S nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/mmuLow.o src/mmuLow.S

${OBJECTDIR}/src/riscvmmu.o: src/riscvmmu.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../../isa/riscvIsa32/include -I../Riscv64/include -Iinclude -I../../../../Kernel/common/include -I../../../../Kernel/rmp/include -I../../../memory/MemoryAreaSingle/include -I../../cpu/qemu/Virt/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/riscvmmu.o src/riscvmmu.c

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
