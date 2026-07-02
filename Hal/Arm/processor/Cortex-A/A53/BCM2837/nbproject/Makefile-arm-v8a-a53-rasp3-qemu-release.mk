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
CC=$$PHAROS_ARM_AARCH64_CC
CCC=g++
CXX=g++
FC=gfortran
AS=$$PHAROS_ARM_AARCH64_CC
AR=$$PHAROS_ARM_AARCH64_ARCHIVER

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=arm-v8a-a53-rasp3-qemu-release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/cpu.o \
	${OBJECTDIR}/src/gpio.o \
	${OBJECTDIR}/src/interruptController.o \
	${OBJECTDIR}/src/timer.o \
	${OBJECTDIR}/src/uart.o


# C Compiler Flags
CFLAGS=-O2 -g -mcpu=cortex-a53 -ffunction-sections -fdata-sections -Wall -Wextra -Wno-unused-parameter -Wno-unused-function

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=-c -O0 -g -mcpu=cortex-a53 -Iinclude -I../../../../architecture/v8/Archv8-A/include -I../../../../../../Kernel/common/include

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libbcm2837.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libbcm2837.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libbcm2837.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libbcm2837.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libbcm2837.a

${OBJECTDIR}/src/cpu.o: src/cpu.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I${PHAROS_FILE_PATH}/Kernel/common/include -I${PHAROS_FILE_PATH}/Hal/Arm/architecture/v8/Archv8-A/include -I${PHAROS_FILE_PATH}/Hal/Arm/architecture/v8/MmuV8/include -I${PHAROS_FILE_PATH}/Hal/Arm/isa/A64/include -I${PHAROS_FILE_PATH}/Hal/Arm/board/raspberry/raspberry3/include -I${PHAROS_FILE_PATH}/Hal/Arm/processor/Cortex-A/A53/BCM2837/include -I${PHAROS_FILE_PATH}/Hal/Arm/architecture/v8/GenericTimer/include -I${PHAROS_FILE_PATH}/Kernel/rmp/include -I${PHAROS_FILE_PATH}/Hal/memory/MemoryAreaSingle/include -I${PHAROS_FILE_PATH}/Kernel/clock/ClockPending/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/cpu.o src/cpu.c

${OBJECTDIR}/src/gpio.o: src/gpio.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I${PHAROS_FILE_PATH}/Kernel/common/include -I${PHAROS_FILE_PATH}/Hal/Arm/architecture/v8/Archv8-A/include -I${PHAROS_FILE_PATH}/Hal/Arm/architecture/v8/MmuV8/include -I${PHAROS_FILE_PATH}/Hal/Arm/isa/A64/include -I${PHAROS_FILE_PATH}/Hal/Arm/board/raspberry/raspberry3/include -I${PHAROS_FILE_PATH}/Hal/Arm/processor/Cortex-A/A53/BCM2837/include -I${PHAROS_FILE_PATH}/Hal/Arm/architecture/v8/GenericTimer/include -I${PHAROS_FILE_PATH}/Kernel/rmp/include -I${PHAROS_FILE_PATH}/Hal/memory/MemoryAreaSingle/include -I${PHAROS_FILE_PATH}/Kernel/clock/ClockPending/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/gpio.o src/gpio.c

${OBJECTDIR}/src/interruptController.o: src/interruptController.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I${PHAROS_FILE_PATH}/Kernel/common/include -I${PHAROS_FILE_PATH}/Hal/Arm/architecture/v8/Archv8-A/include -I${PHAROS_FILE_PATH}/Hal/Arm/architecture/v8/MmuV8/include -I${PHAROS_FILE_PATH}/Hal/Arm/isa/A64/include -I${PHAROS_FILE_PATH}/Hal/Arm/board/raspberry/raspberry3/include -I${PHAROS_FILE_PATH}/Hal/Arm/processor/Cortex-A/A53/BCM2837/include -I${PHAROS_FILE_PATH}/Hal/Arm/architecture/v8/GenericTimer/include -I${PHAROS_FILE_PATH}/Kernel/rmp/include -I${PHAROS_FILE_PATH}/Hal/memory/MemoryAreaSingle/include -I${PHAROS_FILE_PATH}/Kernel/clock/ClockPending/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/interruptController.o src/interruptController.c

${OBJECTDIR}/src/timer.o: src/timer.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I${PHAROS_FILE_PATH}/Kernel/common/include -I${PHAROS_FILE_PATH}/Hal/Arm/architecture/v8/Archv8-A/include -I${PHAROS_FILE_PATH}/Hal/Arm/architecture/v8/MmuV8/include -I${PHAROS_FILE_PATH}/Hal/Arm/isa/A64/include -I${PHAROS_FILE_PATH}/Hal/Arm/board/raspberry/raspberry3/include -I${PHAROS_FILE_PATH}/Hal/Arm/processor/Cortex-A/A53/BCM2837/include -I${PHAROS_FILE_PATH}/Hal/Arm/architecture/v8/GenericTimer/include -I${PHAROS_FILE_PATH}/Kernel/rmp/include -I${PHAROS_FILE_PATH}/Hal/memory/MemoryAreaSingle/include -I${PHAROS_FILE_PATH}/Kernel/clock/ClockPending/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/timer.o src/timer.c

${OBJECTDIR}/src/uart.o: src/uart.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I${PHAROS_FILE_PATH}/Kernel/common/include -I${PHAROS_FILE_PATH}/Hal/Arm/architecture/v8/Archv8-A/include -I${PHAROS_FILE_PATH}/Hal/Arm/architecture/v8/MmuV8/include -I${PHAROS_FILE_PATH}/Hal/Arm/isa/A64/include -I${PHAROS_FILE_PATH}/Hal/Arm/board/raspberry/raspberry3/include -I${PHAROS_FILE_PATH}/Hal/Arm/processor/Cortex-A/A53/BCM2837/include -I${PHAROS_FILE_PATH}/Hal/Arm/architecture/v8/GenericTimer/include -I${PHAROS_FILE_PATH}/Kernel/rmp/include -I${PHAROS_FILE_PATH}/Hal/memory/MemoryAreaSingle/include -I${PHAROS_FILE_PATH}/Kernel/clock/ClockPending/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/uart.o src/uart.c

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
