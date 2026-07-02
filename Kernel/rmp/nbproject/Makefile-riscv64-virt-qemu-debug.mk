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
AS=$$PHAROS_ARM_AARCH64_CC
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
	${OBJECTDIR}/src/channel/rmpbulk.o \
	${OBJECTDIR}/src/channel/rmpchannelreceive.o \
	${OBJECTDIR}/src/mpsc.o \
	${OBJECTDIR}/src/rmp.o \
	${OBJECTDIR}/src/rmpatomic.o \
	${OBJECTDIR}/src/rmpchannelsend.o \
	${OBJECTDIR}/src/rmpfuture.o \
	${OBJECTDIR}/src/rmphwqueue.o \
	${OBJECTDIR}/src/rmppartition.o \
	${OBJECTDIR}/src/rmpscheduler.o \
	${OBJECTDIR}/src/rmpthread.o \
	${OBJECTDIR}/src/rmpthreadqueue.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/librmp.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/librmp.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/librmp.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/librmp.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/librmp.a

${OBJECTDIR}/src/channel/rmpbulk.o: src/channel/rmpbulk.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/channel
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../common/include -Iinclude -I../../Hal/memory/MemoryAreaSingle/include -I../../Hal/Risc-v/architecture/Riscv64/include -I../../Hal/Risc-v/architecture/RiscvSv39/include -I../../Hal/Risc-v/isa/riscvIsa32/include -I../../Hal/Risc-v/cpu/qemu/Virt/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/channel/rmpbulk.o src/channel/rmpbulk.c

${OBJECTDIR}/src/channel/rmpchannelreceive.o: src/channel/rmpchannelreceive.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src/channel
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../common/include -Iinclude -I../../Hal/memory/MemoryAreaSingle/include -I../../Hal/Risc-v/architecture/Riscv64/include -I../../Hal/Risc-v/architecture/RiscvSv39/include -I../../Hal/Risc-v/isa/riscvIsa32/include -I../../Hal/Risc-v/cpu/qemu/Virt/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/channel/rmpchannelreceive.o src/channel/rmpchannelreceive.c

${OBJECTDIR}/src/mpsc.o: src/mpsc.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../common/include -Iinclude -I../../Hal/memory/MemoryAreaSingle/include -I../../Hal/Risc-v/architecture/Riscv64/include -I../../Hal/Risc-v/architecture/RiscvSv39/include -I../../Hal/Risc-v/isa/riscvIsa32/include -I../../Hal/Risc-v/cpu/qemu/Virt/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/mpsc.o src/mpsc.c

${OBJECTDIR}/src/rmp.o: src/rmp.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../common/include -Iinclude -I../../Hal/memory/MemoryAreaSingle/include -I../../Hal/Risc-v/architecture/Riscv64/include -I../../Hal/Risc-v/architecture/RiscvSv39/include -I../../Hal/Risc-v/isa/riscvIsa32/include -I../../Hal/Risc-v/cpu/qemu/Virt/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/rmp.o src/rmp.c

${OBJECTDIR}/src/rmpatomic.o: src/rmpatomic.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../common/include -Iinclude -I../../Hal/memory/MemoryAreaSingle/include -I../../Hal/Risc-v/architecture/Riscv64/include -I../../Hal/Risc-v/architecture/RiscvSv39/include -I../../Hal/Risc-v/isa/riscvIsa32/include -I../../Hal/Risc-v/cpu/qemu/Virt/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/rmpatomic.o src/rmpatomic.c

${OBJECTDIR}/src/rmpchannelsend.o: src/rmpchannelsend.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../common/include -Iinclude -I../../Hal/memory/MemoryAreaSingle/include -I../../Hal/Risc-v/architecture/Riscv64/include -I../../Hal/Risc-v/architecture/RiscvSv39/include -I../../Hal/Risc-v/isa/riscvIsa32/include -I../../Hal/Risc-v/cpu/qemu/Virt/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/rmpchannelsend.o src/rmpchannelsend.c

${OBJECTDIR}/src/rmpfuture.o: src/rmpfuture.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../common/include -Iinclude -I../../Hal/memory/MemoryAreaSingle/include -I../../Hal/Risc-v/architecture/Riscv64/include -I../../Hal/Risc-v/architecture/RiscvSv39/include -I../../Hal/Risc-v/isa/riscvIsa32/include -I../../Hal/Risc-v/cpu/qemu/Virt/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/rmpfuture.o src/rmpfuture.c

${OBJECTDIR}/src/rmphwqueue.o: src/rmphwqueue.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../common/include -Iinclude -I../../Hal/memory/MemoryAreaSingle/include -I../../Hal/Risc-v/architecture/Riscv64/include -I../../Hal/Risc-v/architecture/RiscvSv39/include -I../../Hal/Risc-v/isa/riscvIsa32/include -I../../Hal/Risc-v/cpu/qemu/Virt/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/rmphwqueue.o src/rmphwqueue.c

${OBJECTDIR}/src/rmppartition.o: src/rmppartition.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../common/include -Iinclude -I../../Hal/memory/MemoryAreaSingle/include -I../../Hal/Risc-v/architecture/Riscv64/include -I../../Hal/Risc-v/architecture/RiscvSv39/include -I../../Hal/Risc-v/isa/riscvIsa32/include -I../../Hal/Risc-v/cpu/qemu/Virt/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/rmppartition.o src/rmppartition.c

${OBJECTDIR}/src/rmpscheduler.o: src/rmpscheduler.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../common/include -Iinclude -I../../Hal/memory/MemoryAreaSingle/include -I../../Hal/Risc-v/architecture/Riscv64/include -I../../Hal/Risc-v/architecture/RiscvSv39/include -I../../Hal/Risc-v/isa/riscvIsa32/include -I../../Hal/Risc-v/cpu/qemu/Virt/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/rmpscheduler.o src/rmpscheduler.c

${OBJECTDIR}/src/rmpthread.o: src/rmpthread.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../common/include -Iinclude -I../../Hal/memory/MemoryAreaSingle/include -I../../Hal/Risc-v/architecture/Riscv64/include -I../../Hal/Risc-v/architecture/RiscvSv39/include -I../../Hal/Risc-v/isa/riscvIsa32/include -I../../Hal/Risc-v/cpu/qemu/Virt/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/rmpthread.o src/rmpthread.c

${OBJECTDIR}/src/rmpthreadqueue.o: src/rmpthreadqueue.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../common/include -Iinclude -I../../Hal/memory/MemoryAreaSingle/include -I../../Hal/Risc-v/architecture/Riscv64/include -I../../Hal/Risc-v/architecture/RiscvSv39/include -I../../Hal/Risc-v/isa/riscvIsa32/include -I../../Hal/Risc-v/cpu/qemu/Virt/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/rmpthreadqueue.o src/rmpthreadqueue.c

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
