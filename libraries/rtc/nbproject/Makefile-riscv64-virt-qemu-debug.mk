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
	${OBJECTDIR}/src/io/printk.o \
	${OBJECTDIR}/src/io/rprintk.o \
	${OBJECTDIR}/src/memory/memchr.o \
	${OBJECTDIR}/src/memory/memrchr.o \
	${OBJECTDIR}/src/memory/memread.o \
	${OBJECTDIR}/src/memory/memset.o \
	${OBJECTDIR}/src/string/compare.o \
	${OBJECTDIR}/src/string/comparen.o \
	${OBJECTDIR}/src/string/concatenate.o \
	${OBJECTDIR}/src/string/copy.o \
	${OBJECTDIR}/src/string/length.o \
	${OBJECTDIR}/src/string/print.o \
	${OBJECTDIR}/src/string/searchaccept.o \
	${OBJECTDIR}/src/string/searchreject.o \
	${OBJECTDIR}/src/string/tokenizer.o \
	${OBJECTDIR}/src/string/vprintk.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/librtc.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/librtc.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/librtc.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/librtc.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/librtc.a

${OBJECTDIR}/src/io/printk.o: src/io/printk.c
	${MKDIR} -p ${OBJECTDIR}/src/io
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -Iinclude -I../../Kernel/common/include -I../rtm/include -I../../Hal/Risc-v/architecture/Riscv64/include -I../../Hal/Risc-v/cpu/qemu/Virt/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/io/printk.o src/io/printk.c

${OBJECTDIR}/src/io/rprintk.o: src/io/rprintk.c
	${MKDIR} -p ${OBJECTDIR}/src/io
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -Iinclude -I../../Kernel/common/include -I../rtm/include -I../../Hal/Risc-v/architecture/Riscv64/include -I../../Hal/Risc-v/cpu/qemu/Virt/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/io/rprintk.o src/io/rprintk.c

${OBJECTDIR}/src/memory/memchr.o: src/memory/memchr.c
	${MKDIR} -p ${OBJECTDIR}/src/memory
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -Iinclude -I../../Kernel/common/include -I../rtm/include -I../../Hal/Risc-v/architecture/Riscv64/include -I../../Hal/Risc-v/cpu/qemu/Virt/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/memory/memchr.o src/memory/memchr.c

${OBJECTDIR}/src/memory/memrchr.o: src/memory/memrchr.c
	${MKDIR} -p ${OBJECTDIR}/src/memory
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -Iinclude -I../../Kernel/common/include -I../rtm/include -I../../Hal/Risc-v/architecture/Riscv64/include -I../../Hal/Risc-v/cpu/qemu/Virt/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/memory/memrchr.o src/memory/memrchr.c

${OBJECTDIR}/src/memory/memread.o: src/memory/memread.c
	${MKDIR} -p ${OBJECTDIR}/src/memory
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -Iinclude -I../../Kernel/common/include -I../rtm/include -I../../Hal/Risc-v/architecture/Riscv64/include -I../../Hal/Risc-v/cpu/qemu/Virt/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/memory/memread.o src/memory/memread.c

${OBJECTDIR}/src/memory/memset.o: src/memory/memset.c
	${MKDIR} -p ${OBJECTDIR}/src/memory
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -Iinclude -I../../Kernel/common/include -I../rtm/include -I../../Hal/Risc-v/architecture/Riscv64/include -I../../Hal/Risc-v/cpu/qemu/Virt/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/memory/memset.o src/memory/memset.c

${OBJECTDIR}/src/string/compare.o: src/string/compare.c
	${MKDIR} -p ${OBJECTDIR}/src/string
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -Iinclude -I../../Kernel/common/include -I../rtm/include -I../../Hal/Risc-v/architecture/Riscv64/include -I../../Hal/Risc-v/cpu/qemu/Virt/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/string/compare.o src/string/compare.c

${OBJECTDIR}/src/string/comparen.o: src/string/comparen.c
	${MKDIR} -p ${OBJECTDIR}/src/string
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -Iinclude -I../../Kernel/common/include -I../rtm/include -I../../Hal/Risc-v/architecture/Riscv64/include -I../../Hal/Risc-v/cpu/qemu/Virt/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/string/comparen.o src/string/comparen.c

${OBJECTDIR}/src/string/concatenate.o: src/string/concatenate.c
	${MKDIR} -p ${OBJECTDIR}/src/string
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -Iinclude -I../../Kernel/common/include -I../rtm/include -I../../Hal/Risc-v/architecture/Riscv64/include -I../../Hal/Risc-v/cpu/qemu/Virt/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/string/concatenate.o src/string/concatenate.c

${OBJECTDIR}/src/string/copy.o: src/string/copy.c
	${MKDIR} -p ${OBJECTDIR}/src/string
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -Iinclude -I../../Kernel/common/include -I../rtm/include -I../../Hal/Risc-v/architecture/Riscv64/include -I../../Hal/Risc-v/cpu/qemu/Virt/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/string/copy.o src/string/copy.c

${OBJECTDIR}/src/string/length.o: src/string/length.c
	${MKDIR} -p ${OBJECTDIR}/src/string
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -Iinclude -I../../Kernel/common/include -I../rtm/include -I../../Hal/Risc-v/architecture/Riscv64/include -I../../Hal/Risc-v/cpu/qemu/Virt/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/string/length.o src/string/length.c

${OBJECTDIR}/src/string/print.o: src/string/print.c
	${MKDIR} -p ${OBJECTDIR}/src/string
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -Iinclude -I../../Kernel/common/include -I../rtm/include -I../../Hal/Risc-v/architecture/Riscv64/include -I../../Hal/Risc-v/cpu/qemu/Virt/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/string/print.o src/string/print.c

${OBJECTDIR}/src/string/searchaccept.o: src/string/searchaccept.c
	${MKDIR} -p ${OBJECTDIR}/src/string
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -Iinclude -I../../Kernel/common/include -I../rtm/include -I../../Hal/Risc-v/architecture/Riscv64/include -I../../Hal/Risc-v/cpu/qemu/Virt/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/string/searchaccept.o src/string/searchaccept.c

${OBJECTDIR}/src/string/searchreject.o: src/string/searchreject.c
	${MKDIR} -p ${OBJECTDIR}/src/string
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -Iinclude -I../../Kernel/common/include -I../rtm/include -I../../Hal/Risc-v/architecture/Riscv64/include -I../../Hal/Risc-v/cpu/qemu/Virt/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/string/searchreject.o src/string/searchreject.c

${OBJECTDIR}/src/string/tokenizer.o: src/string/tokenizer.c
	${MKDIR} -p ${OBJECTDIR}/src/string
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -Iinclude -I../../Kernel/common/include -I../rtm/include -I../../Hal/Risc-v/architecture/Riscv64/include -I../../Hal/Risc-v/cpu/qemu/Virt/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/string/tokenizer.o src/string/tokenizer.c

${OBJECTDIR}/src/string/vprintk.o: src/string/vprintk.c
	${MKDIR} -p ${OBJECTDIR}/src/string
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -Iinclude -I../../Kernel/common/include -I../rtm/include -I../../Hal/Risc-v/architecture/Riscv64/include -I../../Hal/Risc-v/cpu/qemu/Virt/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/string/vprintk.o src/string/vprintk.c

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
