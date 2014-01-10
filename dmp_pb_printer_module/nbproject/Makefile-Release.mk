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
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/dmp_printer_module.o \
	${OBJECTDIR}/printer_config.o \
	${OBJECTDIR}/strip_processing.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f1

# C Compiler Flags
CFLAGS=`pkg-config --cflags glib-2.0 MagickWand gthread-2.0` `cups-config --cflags` 

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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdmp_pb_printer_module.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdmp_pb_printer_module.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdmp_pb_printer_module.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} `pkg-config --libs glib-2.0 MagickWand gthread-2.0` `cups-config --cflags` -shared -fPIC

${OBJECTDIR}/dmp_printer_module.o: dmp_printer_module.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 `pkg-config --cflags glib-2.0 MagickWand gthread-2.0` `cups-config --cflags` -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/dmp_printer_module.o dmp_printer_module.c

${OBJECTDIR}/printer_config.o: printer_config.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 `pkg-config --cflags glib-2.0 MagickWand gthread-2.0` `cups-config --cflags` -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/printer_config.o printer_config.c

${OBJECTDIR}/strip_processing.o: strip_processing.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 `pkg-config --cflags glib-2.0 MagickWand gthread-2.0` `cups-config --cflags` -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/strip_processing.o strip_processing.c

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-conf ${TESTFILES}
${TESTDIR}/TestFiles/f1: ${TESTDIR}/tests/printer_module_tests.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c}   -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS} 


${TESTDIR}/tests/printer_module_tests.o: tests/printer_module_tests.c 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.c) -O2 -I. -I. `pkg-config --cflags glib-2.0 MagickWand gthread-2.0` `cups-config --cflags` -MMD -MP -MF $@.d -o ${TESTDIR}/tests/printer_module_tests.o tests/printer_module_tests.c


${OBJECTDIR}/dmp_printer_module_nomain.o: ${OBJECTDIR}/dmp_printer_module.o dmp_printer_module.c 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/dmp_printer_module.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -O2 `pkg-config --cflags glib-2.0 MagickWand gthread-2.0` `cups-config --cflags` -fPIC  -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/dmp_printer_module_nomain.o dmp_printer_module.c;\
	else  \
	    ${CP} ${OBJECTDIR}/dmp_printer_module.o ${OBJECTDIR}/dmp_printer_module_nomain.o;\
	fi

${OBJECTDIR}/printer_config_nomain.o: ${OBJECTDIR}/printer_config.o printer_config.c 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/printer_config.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -O2 `pkg-config --cflags glib-2.0 MagickWand gthread-2.0` `cups-config --cflags` -fPIC  -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/printer_config_nomain.o printer_config.c;\
	else  \
	    ${CP} ${OBJECTDIR}/printer_config.o ${OBJECTDIR}/printer_config_nomain.o;\
	fi

${OBJECTDIR}/strip_processing_nomain.o: ${OBJECTDIR}/strip_processing.o strip_processing.c 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/strip_processing.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -O2 `pkg-config --cflags glib-2.0 MagickWand gthread-2.0` `cups-config --cflags` -fPIC  -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/strip_processing_nomain.o strip_processing.c;\
	else  \
	    ${CP} ${OBJECTDIR}/strip_processing.o ${OBJECTDIR}/strip_processing_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f1 || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdmp_pb_printer_module.${CND_DLIB_EXT}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
