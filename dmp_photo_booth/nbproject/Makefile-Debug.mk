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
	${OBJECTDIR}/configuration.o \
	${OBJECTDIR}/console_queue.o \
	${OBJECTDIR}/error_handling.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/module.o \
	${OBJECTDIR}/module_callbacks.o \
	${OBJECTDIR}/module_status_watchdog.o \
	${OBJECTDIR}/user_interface.o


# C Compiler Flags
CFLAGS=`pkg-config --cflags gmodule-2.0 gthread-2.0 gtk+-3.0 MagickWand` 

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=`pkg-config --libs gmodule-2.0` `pkg-config --libs gtk+-3.0` `pkg-config --libs glib-2.0` `pkg-config --libs MagickWand`  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/dmp_photo_booth

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/dmp_photo_booth: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/dmp_photo_booth ${OBJECTFILES} ${LDLIBSOPTIONS} `pkg-config --libs gmodule-2.0 gthread-2.0 gtk+-3.0 MagickWand`

${OBJECTDIR}/configuration.o: configuration.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -I. -I. -I. -I. `pkg-config --cflags gmodule-2.0` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags glib-2.0` `pkg-config --cflags MagickWand`  `pkg-config --cflags gmodule-2.0 gthread-2.0 gtk+-3.0 MagickWand` -MMD -MP -MF $@.d -o ${OBJECTDIR}/configuration.o configuration.c

${OBJECTDIR}/console_queue.o: console_queue.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -I. -I. -I. -I. `pkg-config --cflags gmodule-2.0` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags glib-2.0` `pkg-config --cflags MagickWand`  `pkg-config --cflags gmodule-2.0 gthread-2.0 gtk+-3.0 MagickWand` -MMD -MP -MF $@.d -o ${OBJECTDIR}/console_queue.o console_queue.c

${OBJECTDIR}/error_handling.o: error_handling.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -I. -I. -I. -I. `pkg-config --cflags gmodule-2.0` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags glib-2.0` `pkg-config --cflags MagickWand`  `pkg-config --cflags gmodule-2.0 gthread-2.0 gtk+-3.0 MagickWand` -MMD -MP -MF $@.d -o ${OBJECTDIR}/error_handling.o error_handling.c

${OBJECTDIR}/main.o: main.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -I. -I. -I. -I. `pkg-config --cflags gmodule-2.0` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags glib-2.0` `pkg-config --cflags MagickWand`  `pkg-config --cflags gmodule-2.0 gthread-2.0 gtk+-3.0 MagickWand` -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.c

${OBJECTDIR}/module.o: module.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -I. -I. -I. -I. `pkg-config --cflags gmodule-2.0` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags glib-2.0` `pkg-config --cflags MagickWand`  `pkg-config --cflags gmodule-2.0 gthread-2.0 gtk+-3.0 MagickWand` -MMD -MP -MF $@.d -o ${OBJECTDIR}/module.o module.c

${OBJECTDIR}/module_callbacks.o: module_callbacks.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -I. -I. -I. -I. `pkg-config --cflags gmodule-2.0` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags glib-2.0` `pkg-config --cflags MagickWand`  `pkg-config --cflags gmodule-2.0 gthread-2.0 gtk+-3.0 MagickWand` -MMD -MP -MF $@.d -o ${OBJECTDIR}/module_callbacks.o module_callbacks.c

${OBJECTDIR}/module_status_watchdog.o: module_status_watchdog.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -I. -I. -I. -I. `pkg-config --cflags gmodule-2.0` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags glib-2.0` `pkg-config --cflags MagickWand`  `pkg-config --cflags gmodule-2.0 gthread-2.0 gtk+-3.0 MagickWand` -MMD -MP -MF $@.d -o ${OBJECTDIR}/module_status_watchdog.o module_status_watchdog.c

${OBJECTDIR}/user_interface.o: user_interface.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -I. -I. -I. -I. `pkg-config --cflags gmodule-2.0` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags glib-2.0` `pkg-config --cflags MagickWand`  `pkg-config --cflags gmodule-2.0 gthread-2.0 gtk+-3.0 MagickWand` -MMD -MP -MF $@.d -o ${OBJECTDIR}/user_interface.o user_interface.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/dmp_photo_booth

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
