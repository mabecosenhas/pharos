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
	${OBJECTDIR}/src/cache.o \
	${OBJECTDIR}/src/channel.o \
	${OBJECTDIR}/src/clock.o \
	${OBJECTDIR}/src/context.o \
	${OBJECTDIR}/src/criticalSection.o \
	${OBJECTDIR}/src/exception.o \
	${OBJECTDIR}/src/hwqueue.o \
	${OBJECTDIR}/src/interrupt.o \
	${OBJECTDIR}/src/interruptstack.o \
	${OBJECTDIR}/src/ipcall.o \
	${OBJECTDIR}/src/resource.o \
	${OBJECTDIR}/src/start.o \
	${OBJECTDIR}/src/startup.o \
	${OBJECTDIR}/src/syscalls.o \
	${OBJECTDIR}/src/thread.o \
	${OBJECTDIR}/src/trap.o \
	${OBJECTDIR}/src/userMode.o


# C Compiler Flags
CFLAGS=-g -O0 -mcmodel=medany -ffunction-sections -fdata-sections -march=rv64imafdc -mabi=lp64d -Wall -Wextra -Wno-unused-parameter -Wno-unused-function

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=-c -g -Iinclude -I../../../../Kernel/common/include -I../../cpu/qemu/Virt/include -O0 -mcmodel=medany -ffunction-sections -fdata-sections -march=rv64imafdc -mabi=lp64d

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libriscv64.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libriscv64.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libriscv64.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libriscv64.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libriscv64.a

${OBJECTDIR}/src/cache.o: src/cache.S nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/cache.o src/cache.S

${OBJECTDIR}/src/channel.o: src/channel.S nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/channel.o src/channel.S

${OBJECTDIR}/src/clock.o: src/clock.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -Werror -I../../../../Kernel/common/include -I../../../../Kernel/rmp/include -I../../../memory/MemoryAreaSingle/include -Iinclude -I../RiscvSv39/include -I../../isa/riscvIsa32/include -I../../../../Kernel/time/ClockContinuous/include -I../../devices/sifive/Clint/include -I../../devices/sifive/Plic/include -I../../cpu/qemu/Virt/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/clock.o src/clock.c

${OBJECTDIR}/src/context.o: src/context.S nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/context.o src/context.S

${OBJECTDIR}/src/criticalSection.o: src/criticalSection.S nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/criticalSection.o src/criticalSection.S

${OBJECTDIR}/src/exception.o: src/exception.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -Werror -I../../../../Kernel/common/include -I../../../../Kernel/rmp/include -I../../../memory/MemoryAreaSingle/include -Iinclude -I../RiscvSv39/include -I../../isa/riscvIsa32/include -I../../../../Kernel/time/ClockContinuous/include -I../../devices/sifive/Clint/include -I../../devices/sifive/Plic/include -I../../cpu/qemu/Virt/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/exception.o src/exception.c

${OBJECTDIR}/src/hwqueue.o: src/hwqueue.S nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/hwqueue.o src/hwqueue.S

${OBJECTDIR}/src/interrupt.o: src/interrupt.S nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/interrupt.o src/interrupt.S

${OBJECTDIR}/src/interruptstack.o: src/interruptstack.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -Werror -I../../../../Kernel/common/include -I../../../../Kernel/rmp/include -I../../../memory/MemoryAreaSingle/include -Iinclude -I../RiscvSv39/include -I../../isa/riscvIsa32/include -I../../../../Kernel/time/ClockContinuous/include -I../../devices/sifive/Clint/include -I../../devices/sifive/Plic/include -I../../cpu/qemu/Virt/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/interruptstack.o src/interruptstack.c

${OBJECTDIR}/src/ipcall.o: src/ipcall.S nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/ipcall.o src/ipcall.S

${OBJECTDIR}/src/resource.o: src/resource.S nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/resource.o src/resource.S

${OBJECTDIR}/src/start.o: src/start.S nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/start.o src/start.S

${OBJECTDIR}/src/startup.o: src/startup.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -Werror -I../../../../Kernel/common/include -I../../../../Kernel/rmp/include -I../../../memory/MemoryAreaSingle/include -Iinclude -I../RiscvSv39/include -I../../isa/riscvIsa32/include -I../../../../Kernel/time/ClockContinuous/include -I../../devices/sifive/Clint/include -I../../devices/sifive/Plic/include -I../../cpu/qemu/Virt/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/startup.o src/startup.c

${OBJECTDIR}/src/syscalls.o: src/syscalls.S nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/syscalls.o src/syscalls.S

${OBJECTDIR}/src/thread.o: src/thread.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -Werror -I../../../../Kernel/common/include -I../../../../Kernel/rmp/include -I../../../memory/MemoryAreaSingle/include -Iinclude -I../RiscvSv39/include -I../../isa/riscvIsa32/include -I../../../../Kernel/time/ClockContinuous/include -I../../devices/sifive/Clint/include -I../../devices/sifive/Plic/include -I../../cpu/qemu/Virt/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/thread.o src/thread.c

${OBJECTDIR}/src/trap.o: src/trap.S nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/trap.o src/trap.S

${OBJECTDIR}/src/userMode.o: src/userMode.S nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/userMode.o src/userMode.S

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
