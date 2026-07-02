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
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=host-release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/acos.o \
	${OBJECTDIR}/src/acosh.o \
	${OBJECTDIR}/src/asin.o \
	${OBJECTDIR}/src/asinh.o \
	${OBJECTDIR}/src/atan.o \
	${OBJECTDIR}/src/atan2.o \
	${OBJECTDIR}/src/atanh.o \
	${OBJECTDIR}/src/cbrt.o \
	${OBJECTDIR}/src/ceil.o \
	${OBJECTDIR}/src/cos.o \
	${OBJECTDIR}/src/cosh.o \
	${OBJECTDIR}/src/erf.o \
	${OBJECTDIR}/src/exp.o \
	${OBJECTDIR}/src/expm1.o \
	${OBJECTDIR}/src/fabs.o \
	${OBJECTDIR}/src/floor.o \
	${OBJECTDIR}/src/fmod.o \
	${OBJECTDIR}/src/frexp.o \
	${OBJECTDIR}/src/hypot.o \
	${OBJECTDIR}/src/ilogb.o \
	${OBJECTDIR}/src/j0.o \
	${OBJECTDIR}/src/j1.o \
	${OBJECTDIR}/src/jn.o \
	${OBJECTDIR}/src/ldexp.o \
	${OBJECTDIR}/src/lgamma.o \
	${OBJECTDIR}/src/log.o \
	${OBJECTDIR}/src/log1.o \
	${OBJECTDIR}/src/log10.o \
	${OBJECTDIR}/src/logb.o \
	${OBJECTDIR}/src/modf.o \
	${OBJECTDIR}/src/nextafter.o \
	${OBJECTDIR}/src/pow.o \
	${OBJECTDIR}/src/rem_pio2.o \
	${OBJECTDIR}/src/remainder.o \
	${OBJECTDIR}/src/rint.o \
	${OBJECTDIR}/src/scalb.o \
	${OBJECTDIR}/src/scalbn.o \
	${OBJECTDIR}/src/significand.o \
	${OBJECTDIR}/src/sin.o \
	${OBJECTDIR}/src/sinh.o \
	${OBJECTDIR}/src/sqrt.o \
	${OBJECTDIR}/src/tan.o \
	${OBJECTDIR}/src/tanh.o


# C Compiler Flags
CFLAGS=-O2 -g -ffunction-sections -fdata-sections -Wall -Wextra -Wno-unused-parameter -Wno-unused-function

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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/librtm.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/librtm.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/librtm.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/librtm.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/librtm.a

${OBJECTDIR}/src/acos.o: src/acos.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Iinclude -I../../Kernel/common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/acos.o src/acos.c

${OBJECTDIR}/src/acosh.o: src/acosh.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Iinclude -I../../Kernel/common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/acosh.o src/acosh.c

${OBJECTDIR}/src/asin.o: src/asin.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Iinclude -I../../Kernel/common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/asin.o src/asin.c

${OBJECTDIR}/src/asinh.o: src/asinh.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Iinclude -I../../Kernel/common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/asinh.o src/asinh.c

${OBJECTDIR}/src/atan.o: src/atan.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Iinclude -I../../Kernel/common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/atan.o src/atan.c

${OBJECTDIR}/src/atan2.o: src/atan2.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Iinclude -I../../Kernel/common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/atan2.o src/atan2.c

${OBJECTDIR}/src/atanh.o: src/atanh.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Iinclude -I../../Kernel/common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/atanh.o src/atanh.c

${OBJECTDIR}/src/cbrt.o: src/cbrt.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Iinclude -I../../Kernel/common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/cbrt.o src/cbrt.c

${OBJECTDIR}/src/ceil.o: src/ceil.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Iinclude -I../../Kernel/common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ceil.o src/ceil.c

${OBJECTDIR}/src/cos.o: src/cos.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Iinclude -I../../Kernel/common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/cos.o src/cos.c

${OBJECTDIR}/src/cosh.o: src/cosh.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Iinclude -I../../Kernel/common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/cosh.o src/cosh.c

${OBJECTDIR}/src/erf.o: src/erf.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Iinclude -I../../Kernel/common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/erf.o src/erf.c

${OBJECTDIR}/src/exp.o: src/exp.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Iinclude -I../../Kernel/common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/exp.o src/exp.c

${OBJECTDIR}/src/expm1.o: src/expm1.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Iinclude -I../../Kernel/common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/expm1.o src/expm1.c

${OBJECTDIR}/src/fabs.o: src/fabs.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Iinclude -I../../Kernel/common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/fabs.o src/fabs.c

${OBJECTDIR}/src/floor.o: src/floor.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Iinclude -I../../Kernel/common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/floor.o src/floor.c

${OBJECTDIR}/src/fmod.o: src/fmod.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Iinclude -I../../Kernel/common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/fmod.o src/fmod.c

${OBJECTDIR}/src/frexp.o: src/frexp.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Iinclude -I../../Kernel/common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/frexp.o src/frexp.c

${OBJECTDIR}/src/hypot.o: src/hypot.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Iinclude -I../../Kernel/common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/hypot.o src/hypot.c

${OBJECTDIR}/src/ilogb.o: src/ilogb.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Iinclude -I../../Kernel/common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ilogb.o src/ilogb.c

${OBJECTDIR}/src/j0.o: src/j0.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Iinclude -I../../Kernel/common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/j0.o src/j0.c

${OBJECTDIR}/src/j1.o: src/j1.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Iinclude -I../../Kernel/common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/j1.o src/j1.c

${OBJECTDIR}/src/jn.o: src/jn.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Iinclude -I../../Kernel/common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/jn.o src/jn.c

${OBJECTDIR}/src/ldexp.o: src/ldexp.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Iinclude -I../../Kernel/common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ldexp.o src/ldexp.c

${OBJECTDIR}/src/lgamma.o: src/lgamma.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Iinclude -I../../Kernel/common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/lgamma.o src/lgamma.c

${OBJECTDIR}/src/log.o: src/log.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Iinclude -I../../Kernel/common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/log.o src/log.c

${OBJECTDIR}/src/log1.o: src/log1.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Iinclude -I../../Kernel/common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/log1.o src/log1.c

${OBJECTDIR}/src/log10.o: src/log10.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Iinclude -I../../Kernel/common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/log10.o src/log10.c

${OBJECTDIR}/src/logb.o: src/logb.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Iinclude -I../../Kernel/common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/logb.o src/logb.c

${OBJECTDIR}/src/modf.o: src/modf.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Iinclude -I../../Kernel/common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/modf.o src/modf.c

${OBJECTDIR}/src/nextafter.o: src/nextafter.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Iinclude -I../../Kernel/common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/nextafter.o src/nextafter.c

${OBJECTDIR}/src/pow.o: src/pow.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Iinclude -I../../Kernel/common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/pow.o src/pow.c

${OBJECTDIR}/src/rem_pio2.o: src/rem_pio2.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Iinclude -I../../Kernel/common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/rem_pio2.o src/rem_pio2.c

${OBJECTDIR}/src/remainder.o: src/remainder.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Iinclude -I../../Kernel/common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/remainder.o src/remainder.c

${OBJECTDIR}/src/rint.o: src/rint.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Iinclude -I../../Kernel/common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/rint.o src/rint.c

${OBJECTDIR}/src/scalb.o: src/scalb.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Iinclude -I../../Kernel/common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/scalb.o src/scalb.c

${OBJECTDIR}/src/scalbn.o: src/scalbn.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Iinclude -I../../Kernel/common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/scalbn.o src/scalbn.c

${OBJECTDIR}/src/significand.o: src/significand.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Iinclude -I../../Kernel/common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/significand.o src/significand.c

${OBJECTDIR}/src/sin.o: src/sin.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Iinclude -I../../Kernel/common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/sin.o src/sin.c

${OBJECTDIR}/src/sinh.o: src/sinh.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Iinclude -I../../Kernel/common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/sinh.o src/sinh.c

${OBJECTDIR}/src/sqrt.o: src/sqrt.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Iinclude -I../../Kernel/common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/sqrt.o src/sqrt.c

${OBJECTDIR}/src/tan.o: src/tan.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Iinclude -I../../Kernel/common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/tan.o src/tan.c

${OBJECTDIR}/src/tanh.o: src/tanh.c nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -Iinclude -I../../Kernel/common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/tanh.o src/tanh.c

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
