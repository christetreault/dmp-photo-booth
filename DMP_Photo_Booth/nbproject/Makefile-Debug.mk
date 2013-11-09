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
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/console_queue.o \
	${OBJECTDIR}/error_handling.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/module.o \
	${OBJECTDIR}/module_callbacks.o \
	${OBJECTDIR}/module_status_watchdog.o \
	${OBJECTDIR}/user_interface.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f3 \
	${TESTDIR}/TestFiles/f1

# C Compiler Flags
CFLAGS=`pkg-config --cflags gmodule-2.0 gthread-2.0 gtk+-3.0` 

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=`pkg-config --libs gmodule-2.0` `pkg-config --libs gthread-2.0` `pkg-config --libs gtk+-3.0` `pkg-config --libs gthread-2.0` `pkg-config --libs gmodule-2.0` `pkg-config --libs gtk+-3.0` `pkg-config --libs glib-2.0`  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/dmp_photo_booth

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/dmp_photo_booth: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/dmp_photo_booth ${OBJECTFILES} ${LDLIBSOPTIONS} `pkg-config --libs gmodule-2.0 gthread-2.0 gtk+-3.0`

${OBJECTDIR}/console_queue.o: console_queue.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -I. -I. -I. -I. -I. `pkg-config --cflags gmodule-2.0` `pkg-config --cflags gthread-2.0` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags gthread-2.0` `pkg-config --cflags gmodule-2.0` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags glib-2.0`  `pkg-config --cflags gmodule-2.0 gthread-2.0 gtk+-3.0` -MMD -MP -MF $@.d -o ${OBJECTDIR}/console_queue.o console_queue.c

${OBJECTDIR}/error_handling.o: error_handling.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -I. -I. -I. -I. -I. `pkg-config --cflags gmodule-2.0` `pkg-config --cflags gthread-2.0` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags gthread-2.0` `pkg-config --cflags gmodule-2.0` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags glib-2.0`  `pkg-config --cflags gmodule-2.0 gthread-2.0 gtk+-3.0` -MMD -MP -MF $@.d -o ${OBJECTDIR}/error_handling.o error_handling.c

${OBJECTDIR}/main.o: main.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -I. -I. -I. -I. -I. `pkg-config --cflags gmodule-2.0` `pkg-config --cflags gthread-2.0` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags gthread-2.0` `pkg-config --cflags gmodule-2.0` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags glib-2.0`  `pkg-config --cflags gmodule-2.0 gthread-2.0 gtk+-3.0` -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.c

${OBJECTDIR}/module.o: module.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -I. -I. -I. -I. -I. `pkg-config --cflags gmodule-2.0` `pkg-config --cflags gthread-2.0` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags gthread-2.0` `pkg-config --cflags gmodule-2.0` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags glib-2.0`  `pkg-config --cflags gmodule-2.0 gthread-2.0 gtk+-3.0` -MMD -MP -MF $@.d -o ${OBJECTDIR}/module.o module.c

${OBJECTDIR}/module_callbacks.o: module_callbacks.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -I. -I. -I. -I. -I. `pkg-config --cflags gmodule-2.0` `pkg-config --cflags gthread-2.0` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags gthread-2.0` `pkg-config --cflags gmodule-2.0` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags glib-2.0`  `pkg-config --cflags gmodule-2.0 gthread-2.0 gtk+-3.0` -MMD -MP -MF $@.d -o ${OBJECTDIR}/module_callbacks.o module_callbacks.c

${OBJECTDIR}/module_status_watchdog.o: module_status_watchdog.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -I. -I. -I. -I. -I. `pkg-config --cflags gmodule-2.0` `pkg-config --cflags gthread-2.0` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags gthread-2.0` `pkg-config --cflags gmodule-2.0` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags glib-2.0`  `pkg-config --cflags gmodule-2.0 gthread-2.0 gtk+-3.0` -MMD -MP -MF $@.d -o ${OBJECTDIR}/module_status_watchdog.o module_status_watchdog.c

${OBJECTDIR}/user_interface.o: user_interface.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -I. -I. -I. -I. -I. `pkg-config --cflags gmodule-2.0` `pkg-config --cflags gthread-2.0` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags gthread-2.0` `pkg-config --cflags gmodule-2.0` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags glib-2.0`  `pkg-config --cflags gmodule-2.0 gthread-2.0 gtk+-3.0` -MMD -MP -MF $@.d -o ${OBJECTDIR}/user_interface.o user_interface.c

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-conf ${TESTFILES}
${TESTDIR}/TestFiles/f3: ${TESTDIR}/tests/console_queue_tests.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c}   -o ${TESTDIR}/TestFiles/f3 $^ ${LDLIBSOPTIONS} 

${TESTDIR}/TestFiles/f1: ${TESTDIR}/tests/module_tests.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c}   -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS} `pkg-config --libs gmodule-2.0` `pkg-config --libs gthread-2.0` `pkg-config --libs gtk+-3.0`   


${TESTDIR}/tests/console_queue_tests.o: tests/console_queue_tests.c 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.c) -g -I. -I. -I. -I. -I. -I. `pkg-config --cflags gmodule-2.0` `pkg-config --cflags gthread-2.0` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags gthread-2.0` `pkg-config --cflags gmodule-2.0` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags glib-2.0`  `pkg-config --cflags gmodule-2.0 gthread-2.0 gtk+-3.0` -MMD -MP -MF $@.d -o ${TESTDIR}/tests/console_queue_tests.o tests/console_queue_tests.c


${TESTDIR}/tests/module_tests.o: tests/module_tests.c 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.c) -g -I. -I. -I. -I. -I. -I. -I. `pkg-config --cflags gmodule-2.0` `pkg-config --cflags gthread-2.0` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags gthread-2.0` `pkg-config --cflags gmodule-2.0` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags glib-2.0`  `pkg-config --cflags gmodule-2.0 gthread-2.0 gtk+-3.0` -MMD -MP -MF $@.d -o ${TESTDIR}/tests/module_tests.o tests/module_tests.c


${OBJECTDIR}/console_queue_nomain.o: ${OBJECTDIR}/console_queue.o console_queue.c 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/console_queue.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -g -I. -I. -I. -I. -I. `pkg-config --cflags gmodule-2.0` `pkg-config --cflags gthread-2.0` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags gthread-2.0` `pkg-config --cflags gmodule-2.0` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags glib-2.0`  `pkg-config --cflags gmodule-2.0 gthread-2.0 gtk+-3.0` -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/console_queue_nomain.o console_queue.c;\
	else  \
	    ${CP} ${OBJECTDIR}/console_queue.o ${OBJECTDIR}/console_queue_nomain.o;\
	fi

${OBJECTDIR}/error_handling_nomain.o: ${OBJECTDIR}/error_handling.o error_handling.c 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/error_handling.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -g -I. -I. -I. -I. -I. `pkg-config --cflags gmodule-2.0` `pkg-config --cflags gthread-2.0` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags gthread-2.0` `pkg-config --cflags gmodule-2.0` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags glib-2.0`  `pkg-config --cflags gmodule-2.0 gthread-2.0 gtk+-3.0` -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/error_handling_nomain.o error_handling.c;\
	else  \
	    ${CP} ${OBJECTDIR}/error_handling.o ${OBJECTDIR}/error_handling_nomain.o;\
	fi

${OBJECTDIR}/main_nomain.o: ${OBJECTDIR}/main.o main.c 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/main.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -g -I. -I. -I. -I. -I. `pkg-config --cflags gmodule-2.0` `pkg-config --cflags gthread-2.0` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags gthread-2.0` `pkg-config --cflags gmodule-2.0` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags glib-2.0`  `pkg-config --cflags gmodule-2.0 gthread-2.0 gtk+-3.0` -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/main_nomain.o main.c;\
	else  \
	    ${CP} ${OBJECTDIR}/main.o ${OBJECTDIR}/main_nomain.o;\
	fi

${OBJECTDIR}/module_nomain.o: ${OBJECTDIR}/module.o module.c 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/module.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -g -I. -I. -I. -I. -I. `pkg-config --cflags gmodule-2.0` `pkg-config --cflags gthread-2.0` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags gthread-2.0` `pkg-config --cflags gmodule-2.0` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags glib-2.0`  `pkg-config --cflags gmodule-2.0 gthread-2.0 gtk+-3.0` -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/module_nomain.o module.c;\
	else  \
	    ${CP} ${OBJECTDIR}/module.o ${OBJECTDIR}/module_nomain.o;\
	fi

${OBJECTDIR}/module_callbacks_nomain.o: ${OBJECTDIR}/module_callbacks.o module_callbacks.c 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/module_callbacks.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -g -I. -I. -I. -I. -I. `pkg-config --cflags gmodule-2.0` `pkg-config --cflags gthread-2.0` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags gthread-2.0` `pkg-config --cflags gmodule-2.0` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags glib-2.0`  `pkg-config --cflags gmodule-2.0 gthread-2.0 gtk+-3.0` -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/module_callbacks_nomain.o module_callbacks.c;\
	else  \
	    ${CP} ${OBJECTDIR}/module_callbacks.o ${OBJECTDIR}/module_callbacks_nomain.o;\
	fi

${OBJECTDIR}/module_status_watchdog_nomain.o: ${OBJECTDIR}/module_status_watchdog.o module_status_watchdog.c 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/module_status_watchdog.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -g -I. -I. -I. -I. -I. `pkg-config --cflags gmodule-2.0` `pkg-config --cflags gthread-2.0` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags gthread-2.0` `pkg-config --cflags gmodule-2.0` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags glib-2.0`  `pkg-config --cflags gmodule-2.0 gthread-2.0 gtk+-3.0` -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/module_status_watchdog_nomain.o module_status_watchdog.c;\
	else  \
	    ${CP} ${OBJECTDIR}/module_status_watchdog.o ${OBJECTDIR}/module_status_watchdog_nomain.o;\
	fi

${OBJECTDIR}/user_interface_nomain.o: ${OBJECTDIR}/user_interface.o user_interface.c 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/user_interface.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -g -I. -I. -I. -I. -I. `pkg-config --cflags gmodule-2.0` `pkg-config --cflags gthread-2.0` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags gthread-2.0` `pkg-config --cflags gmodule-2.0` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags glib-2.0`  `pkg-config --cflags gmodule-2.0 gthread-2.0 gtk+-3.0` -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/user_interface_nomain.o user_interface.c;\
	else  \
	    ${CP} ${OBJECTDIR}/user_interface.o ${OBJECTDIR}/user_interface_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f3 || true; \
	    ${TESTDIR}/TestFiles/f1 || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/dmp_photo_booth

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
