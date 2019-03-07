#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=cof
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/szelepvezerlo.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/szelepvezerlo.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../src/Can.c ../src/Console.c ../src/control.c ../src/RF.c ../src/swI2C.c ../src/TimeDelay.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1360937237/Can.o ${OBJECTDIR}/_ext/1360937237/Console.o ${OBJECTDIR}/_ext/1360937237/control.o ${OBJECTDIR}/_ext/1360937237/RF.o ${OBJECTDIR}/_ext/1360937237/swI2C.o ${OBJECTDIR}/_ext/1360937237/TimeDelay.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1360937237/Can.o.d ${OBJECTDIR}/_ext/1360937237/Console.o.d ${OBJECTDIR}/_ext/1360937237/control.o.d ${OBJECTDIR}/_ext/1360937237/RF.o.d ${OBJECTDIR}/_ext/1360937237/swI2C.o.d ${OBJECTDIR}/_ext/1360937237/TimeDelay.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1360937237/Can.o ${OBJECTDIR}/_ext/1360937237/Console.o ${OBJECTDIR}/_ext/1360937237/control.o ${OBJECTDIR}/_ext/1360937237/RF.o ${OBJECTDIR}/_ext/1360937237/swI2C.o ${OBJECTDIR}/_ext/1360937237/TimeDelay.o

# Source Files
SOURCEFILES=../src/Can.c ../src/Console.c ../src/control.c ../src/RF.c ../src/swI2C.c ../src/TimeDelay.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/szelepvezerlo.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18F4680
MP_PROCESSOR_OPTION_LD=18f4680
MP_LINKER_DEBUG_OPTION= -u_DEBUGCODESTART=0xfd30 -u_DEBUGCODELEN=0x2d0 -u_DEBUGDATASTART=0xcf4 -u_DEBUGDATALEN=0xb
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1360937237/Can.o: ../src/Can.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Can.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Can.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1360937237/Can.o   ../src/Can.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1360937237/Can.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/Can.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1360937237/Console.o: ../src/Console.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Console.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Console.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1360937237/Console.o   ../src/Console.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1360937237/Console.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/Console.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1360937237/control.o: ../src/control.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/control.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/control.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1360937237/control.o   ../src/control.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1360937237/control.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/control.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1360937237/RF.o: ../src/RF.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/RF.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/RF.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1360937237/RF.o   ../src/RF.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1360937237/RF.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/RF.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1360937237/swI2C.o: ../src/swI2C.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/swI2C.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/swI2C.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1360937237/swI2C.o   ../src/swI2C.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1360937237/swI2C.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/swI2C.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1360937237/TimeDelay.o: ../src/TimeDelay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/TimeDelay.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/TimeDelay.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1360937237/TimeDelay.o   ../src/TimeDelay.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1360937237/TimeDelay.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/TimeDelay.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
else
${OBJECTDIR}/_ext/1360937237/Can.o: ../src/Can.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Can.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Can.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1360937237/Can.o   ../src/Can.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1360937237/Can.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/Can.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1360937237/Console.o: ../src/Console.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Console.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/Console.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1360937237/Console.o   ../src/Console.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1360937237/Console.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/Console.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1360937237/control.o: ../src/control.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/control.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/control.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1360937237/control.o   ../src/control.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1360937237/control.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/control.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1360937237/RF.o: ../src/RF.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/RF.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/RF.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1360937237/RF.o   ../src/RF.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1360937237/RF.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/RF.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1360937237/swI2C.o: ../src/swI2C.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/swI2C.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/swI2C.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1360937237/swI2C.o   ../src/swI2C.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1360937237/swI2C.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/swI2C.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1360937237/TimeDelay.o: ../src/TimeDelay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/TimeDelay.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/TimeDelay.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1360937237/TimeDelay.o   ../src/TimeDelay.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1360937237/TimeDelay.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/TimeDelay.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/szelepvezerlo.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE)   -p$(MP_PROCESSOR_OPTION_LD)  -w -x -u_DEBUG -m"${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"  -z__MPLAB_BUILD=1  -u_CRUNTIME -z__MPLAB_DEBUG=1 -z__MPLAB_DEBUGGER_PK3=1 $(MP_LINKER_DEBUG_OPTION) -l ${MP_CC_DIR}\\..\\lib  -o dist/${CND_CONF}/${IMAGE_TYPE}/szelepvezerlo.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}   
else
dist/${CND_CONF}/${IMAGE_TYPE}/szelepvezerlo.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE)   -p$(MP_PROCESSOR_OPTION_LD)  -w  -m"${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"  -z__MPLAB_BUILD=1  -u_CRUNTIME -l ${MP_CC_DIR}\\..\\lib  -o dist/${CND_CONF}/${IMAGE_TYPE}/szelepvezerlo.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}   
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
