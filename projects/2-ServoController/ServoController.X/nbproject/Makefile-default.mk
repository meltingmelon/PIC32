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
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/5-lab2Application.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/5-lab2Application.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../../../src/BOARD.c ../../../src/FreeRunningTimer.c ../../../src/PingSensor.c ../../../src/RCServo.c ../../../src/RotaryEncoder.c lab2Application.c ../../../src/delays.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1386528437/BOARD.o ${OBJECTDIR}/_ext/1386528437/FreeRunningTimer.o ${OBJECTDIR}/_ext/1386528437/PingSensor.o ${OBJECTDIR}/_ext/1386528437/RCServo.o ${OBJECTDIR}/_ext/1386528437/RotaryEncoder.o ${OBJECTDIR}/lab2Application.o ${OBJECTDIR}/_ext/1386528437/delays.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1386528437/BOARD.o.d ${OBJECTDIR}/_ext/1386528437/FreeRunningTimer.o.d ${OBJECTDIR}/_ext/1386528437/PingSensor.o.d ${OBJECTDIR}/_ext/1386528437/RCServo.o.d ${OBJECTDIR}/_ext/1386528437/RotaryEncoder.o.d ${OBJECTDIR}/lab2Application.o.d ${OBJECTDIR}/_ext/1386528437/delays.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1386528437/BOARD.o ${OBJECTDIR}/_ext/1386528437/FreeRunningTimer.o ${OBJECTDIR}/_ext/1386528437/PingSensor.o ${OBJECTDIR}/_ext/1386528437/RCServo.o ${OBJECTDIR}/_ext/1386528437/RotaryEncoder.o ${OBJECTDIR}/lab2Application.o ${OBJECTDIR}/_ext/1386528437/delays.o

# Source Files
SOURCEFILES=../../../src/BOARD.c ../../../src/FreeRunningTimer.c ../../../src/PingSensor.c ../../../src/RCServo.c ../../../src/RotaryEncoder.c lab2Application.c ../../../src/delays.c



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
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/5-lab2Application.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX340F512H
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1386528437/BOARD.o: ../../../src/BOARD.c  .generated_files/9c2f9ea796353b15137d57751c86482246fa666f.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1386528437" 
	@${RM} ${OBJECTDIR}/_ext/1386528437/BOARD.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386528437/BOARD.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1386528437/BOARD.o.d" -o ${OBJECTDIR}/_ext/1386528437/BOARD.o ../../../src/BOARD.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1386528437/FreeRunningTimer.o: ../../../src/FreeRunningTimer.c  .generated_files/a988f2e5fd72c469e88cccd6c90cd8a47d73c6f5.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1386528437" 
	@${RM} ${OBJECTDIR}/_ext/1386528437/FreeRunningTimer.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386528437/FreeRunningTimer.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1386528437/FreeRunningTimer.o.d" -o ${OBJECTDIR}/_ext/1386528437/FreeRunningTimer.o ../../../src/FreeRunningTimer.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1386528437/PingSensor.o: ../../../src/PingSensor.c  .generated_files/b027583653da73f9d474fdf097fc9cc8764f7c80.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1386528437" 
	@${RM} ${OBJECTDIR}/_ext/1386528437/PingSensor.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386528437/PingSensor.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1386528437/PingSensor.o.d" -o ${OBJECTDIR}/_ext/1386528437/PingSensor.o ../../../src/PingSensor.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1386528437/RCServo.o: ../../../src/RCServo.c  .generated_files/83cea1676d9fa3dfb1f141c7b1f886b5db613af0.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1386528437" 
	@${RM} ${OBJECTDIR}/_ext/1386528437/RCServo.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386528437/RCServo.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1386528437/RCServo.o.d" -o ${OBJECTDIR}/_ext/1386528437/RCServo.o ../../../src/RCServo.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1386528437/RotaryEncoder.o: ../../../src/RotaryEncoder.c  .generated_files/5ad62eda96779dc440fe2da28da8296b82b316e.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1386528437" 
	@${RM} ${OBJECTDIR}/_ext/1386528437/RotaryEncoder.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386528437/RotaryEncoder.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1386528437/RotaryEncoder.o.d" -o ${OBJECTDIR}/_ext/1386528437/RotaryEncoder.o ../../../src/RotaryEncoder.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/lab2Application.o: lab2Application.c  .generated_files/48ffb9cb1d47bbd78a4579f2ce26e691a97135d4.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/lab2Application.o.d 
	@${RM} ${OBJECTDIR}/lab2Application.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -Wall -MP -MMD -MF "${OBJECTDIR}/lab2Application.o.d" -o ${OBJECTDIR}/lab2Application.o lab2Application.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1386528437/delays.o: ../../../src/delays.c  .generated_files/f0155557b787d588cad66beac88b4feb147e5a69.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1386528437" 
	@${RM} ${OBJECTDIR}/_ext/1386528437/delays.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386528437/delays.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1386528437/delays.o.d" -o ${OBJECTDIR}/_ext/1386528437/delays.o ../../../src/delays.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
else
${OBJECTDIR}/_ext/1386528437/BOARD.o: ../../../src/BOARD.c  .generated_files/1fd178ecf538dbedcd656e3c712a98161f02e026.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1386528437" 
	@${RM} ${OBJECTDIR}/_ext/1386528437/BOARD.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386528437/BOARD.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1386528437/BOARD.o.d" -o ${OBJECTDIR}/_ext/1386528437/BOARD.o ../../../src/BOARD.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1386528437/FreeRunningTimer.o: ../../../src/FreeRunningTimer.c  .generated_files/f299bfbc79a9cf2ccce88734b61c389c1db91017.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1386528437" 
	@${RM} ${OBJECTDIR}/_ext/1386528437/FreeRunningTimer.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386528437/FreeRunningTimer.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1386528437/FreeRunningTimer.o.d" -o ${OBJECTDIR}/_ext/1386528437/FreeRunningTimer.o ../../../src/FreeRunningTimer.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1386528437/PingSensor.o: ../../../src/PingSensor.c  .generated_files/58f27e985472f1c9039e8112180f98fac686bb9a.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1386528437" 
	@${RM} ${OBJECTDIR}/_ext/1386528437/PingSensor.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386528437/PingSensor.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1386528437/PingSensor.o.d" -o ${OBJECTDIR}/_ext/1386528437/PingSensor.o ../../../src/PingSensor.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1386528437/RCServo.o: ../../../src/RCServo.c  .generated_files/b12da4864f2327b8d612e8c50fbdc6cb5392bf1b.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1386528437" 
	@${RM} ${OBJECTDIR}/_ext/1386528437/RCServo.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386528437/RCServo.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1386528437/RCServo.o.d" -o ${OBJECTDIR}/_ext/1386528437/RCServo.o ../../../src/RCServo.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1386528437/RotaryEncoder.o: ../../../src/RotaryEncoder.c  .generated_files/9b86f78867405209671643671e6142858509298e.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1386528437" 
	@${RM} ${OBJECTDIR}/_ext/1386528437/RotaryEncoder.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386528437/RotaryEncoder.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1386528437/RotaryEncoder.o.d" -o ${OBJECTDIR}/_ext/1386528437/RotaryEncoder.o ../../../src/RotaryEncoder.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/lab2Application.o: lab2Application.c  .generated_files/63b272bddd679dda15da6ee4a0f021bc85e0e2df.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/lab2Application.o.d 
	@${RM} ${OBJECTDIR}/lab2Application.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -Wall -MP -MMD -MF "${OBJECTDIR}/lab2Application.o.d" -o ${OBJECTDIR}/lab2Application.o lab2Application.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1386528437/delays.o: ../../../src/delays.c  .generated_files/6c0275deb5614fb4a4b36a39e4f61083375e837e.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1386528437" 
	@${RM} ${OBJECTDIR}/_ext/1386528437/delays.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386528437/delays.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1386528437/delays.o.d" -o ${OBJECTDIR}/_ext/1386528437/delays.o ../../../src/delays.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/5-lab2Application.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ../Protocol.X.a  
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g -mdebugger -D__MPLAB_DEBUGGER_PK3=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/5-lab2Application.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}    ../Protocol.X.a      -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)   -mreserve=data@0x0:0x1FC -mreserve=boot@0x1FC02000:0x1FC02FEF -mreserve=boot@0x1FC02000:0x1FC024FF  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=__MPLAB_DEBUGGER_PK3=1,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -mdfp="${DFP_DIR}"
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/5-lab2Application.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ../Protocol.X.a 
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/5-lab2Application.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}    ../Protocol.X.a      -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -mdfp="${DFP_DIR}"
	${MP_CC_DIR}/xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/5-lab2Application.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
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

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
