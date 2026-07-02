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
CND_CONF=arm-v7-r5-tms570lc4357-LAUNCHXL2_570LC43-release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/resourceHigh.o \
	${OBJECTDIR}/src/cache.o \
	${OBJECTDIR}/src/channel.o \
	${OBJECTDIR}/src/channelHigh.o \
	${OBJECTDIR}/src/criticalSection.o \
	${OBJECTDIR}/src/handlerSwi.o \
	${OBJECTDIR}/src/hwqueue.o \
	${OBJECTDIR}/src/hwqueueHigh.o \
	${OBJECTDIR}/src/ipCallHigh.o \
	${OBJECTDIR}/src/ipcall.o \
	${OBJECTDIR}/src/mpu.o \
	${OBJECTDIR}/src/pmsa.o \
	${OBJECTDIR}/src/resource.o


# C Compiler Flags
CFLAGS=-O2 -mcpu=cortex-r5 -mfpu=vfpv3-d16 -mbig-endian -ffunction-sections -fdata-sections -Wall -Wextra -Wno-unused-parameter -Wno-unused-function

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=-mcpu=cortex-r5 -mfpu=vfpv3-d16 -mbig-endian -g -c -I../ArchV7-R/include -Iinclude -I../../../../../../Kernel/common/include -I../../../../processor/Cortex-R/R5/tms570lc4357/include -I../../../../processor/Cortex-R/R5/R5/include -I../../../../isa/Arm/include -I../../../../support/include -I../../../../manufacturer/ti/Vim/include

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libpmsrv7.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libpmsrv7.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libpmsrv7.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libpmsrv7.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libpmsrv7.a

${OBJECTDIR}/resourceHigh.o: resourceHigh.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../ArchV7-R/include -Iinclude -I../../../../../../Kernel/common/include -I../../../../processor/Cortex-R/R5/tms570lc4357/include -I../../../../processor/Cortex-R/R5/R5/include -I../../../../isa/Arm/include -I../../../../support/include -I../../../../manufacturer/ti/Vim/include -I../../../../board/TI/LaunchXL2-570LC43/include -I../../../../../../Kernel/rsp/include -I../../../../../memory/MemoryAreaSingle/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/resourceHigh.o resourceHigh.c

${OBJECTDIR}/src/cache.o: src/cache.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/cache.o src/cache.S

${OBJECTDIR}/src/channel.o: src/channel.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/channel.o src/channel.S

${OBJECTDIR}/src/channelHigh.o: src/channelHigh.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../ArchV7-R/include -Iinclude -I../../../../../../Kernel/common/include -I../../../../processor/Cortex-R/R5/tms570lc4357/include -I../../../../processor/Cortex-R/R5/R5/include -I../../../../isa/Arm/include -I../../../../support/include -I../../../../manufacturer/ti/Vim/include -I../../../../board/TI/LaunchXL2-570LC43/include -I../../../../../../Kernel/rsp/include -I../../../../../memory/MemoryAreaSingle/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/channelHigh.o src/channelHigh.c

${OBJECTDIR}/src/criticalSection.o: src/criticalSection.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/criticalSection.o src/criticalSection.S

${OBJECTDIR}/src/handlerSwi.o: src/handlerSwi.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/handlerSwi.o src/handlerSwi.S

${OBJECTDIR}/src/hwqueue.o: src/hwqueue.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/hwqueue.o src/hwqueue.S

${OBJECTDIR}/src/hwqueueHigh.o: src/hwqueueHigh.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../ArchV7-R/include -Iinclude -I../../../../../../Kernel/common/include -I../../../../processor/Cortex-R/R5/tms570lc4357/include -I../../../../processor/Cortex-R/R5/R5/include -I../../../../isa/Arm/include -I../../../../support/include -I../../../../manufacturer/ti/Vim/include -I../../../../board/TI/LaunchXL2-570LC43/include -I../../../../../../Kernel/rsp/include -I../../../../../memory/MemoryAreaSingle/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/hwqueueHigh.o src/hwqueueHigh.c

${OBJECTDIR}/src/ipCallHigh.o: src/ipCallHigh.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../ArchV7-R/include -Iinclude -I../../../../../../Kernel/common/include -I../../../../processor/Cortex-R/R5/tms570lc4357/include -I../../../../processor/Cortex-R/R5/R5/include -I../../../../isa/Arm/include -I../../../../support/include -I../../../../manufacturer/ti/Vim/include -I../../../../board/TI/LaunchXL2-570LC43/include -I../../../../../../Kernel/rsp/include -I../../../../../memory/MemoryAreaSingle/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ipCallHigh.o src/ipCallHigh.c

${OBJECTDIR}/src/ipcall.o: src/ipcall.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/ipcall.o src/ipcall.S

${OBJECTDIR}/src/mpu.o: src/mpu.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I../ArchV7-R/include -Iinclude -I../../../../../../Kernel/common/include -I../../../../processor/Cortex-R/R5/tms570lc4357/include -I../../../../processor/Cortex-R/R5/R5/include -I../../../../isa/Arm/include -I../../../../support/include -I../../../../manufacturer/ti/Vim/include -I../../../../board/TI/LaunchXL2-570LC43/include -I../../../../../../Kernel/rsp/include -I../../../../../memory/MemoryAreaSingle/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/mpu.o src/mpu.c

${OBJECTDIR}/src/pmsa.o: src/pmsa.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/pmsa.o src/pmsa.S

${OBJECTDIR}/src/resource.o: src/resource.S
	${MKDIR} -p ${OBJECTDIR}/src
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/src/resource.o src/resource.S

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
