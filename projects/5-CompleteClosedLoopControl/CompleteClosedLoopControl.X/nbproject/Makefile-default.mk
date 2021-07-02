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
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/lab5Application.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/lab5Application.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
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
SOURCEFILES_QUOTED_IF_SPACED=../../../src/ADCFilter.c ../../../src/BOARD.c ../../../src/DCMotorDrive.c ../../../src/FeedbackControl.c ../../../src/FreeRunningTimer.c ../../../src/NonVolatileMemory.c ../../../src/RotaryEncoder.c lab5Application.c ../../../src/delays.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1386528437/ADCFilter.o ${OBJECTDIR}/_ext/1386528437/BOARD.o ${OBJECTDIR}/_ext/1386528437/DCMotorDrive.o ${OBJECTDIR}/_ext/1386528437/FeedbackControl.o ${OBJECTDIR}/_ext/1386528437/FreeRunningTimer.o ${OBJECTDIR}/_ext/1386528437/NonVolatileMemory.o ${OBJECTDIR}/_ext/1386528437/RotaryEncoder.o ${OBJECTDIR}/lab5Application.o ${OBJECTDIR}/_ext/1386528437/delays.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1386528437/ADCFilter.o.d ${OBJECTDIR}/_ext/1386528437/BOARD.o.d ${OBJECTDIR}/_ext/1386528437/DCMotorDrive.o.d ${OBJECTDIR}/_ext/1386528437/FeedbackControl.o.d ${OBJECTDIR}/_ext/1386528437/FreeRunningTimer.o.d ${OBJECTDIR}/_ext/1386528437/NonVolatileMemory.o.d ${OBJECTDIR}/_ext/1386528437/RotaryEncoder.o.d ${OBJECTDIR}/lab5Application.o.d ${OBJECTDIR}/_ext/1386528437/delays.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1386528437/ADCFilter.o ${OBJECTDIR}/_ext/1386528437/BOARD.o ${OBJECTDIR}/_ext/1386528437/DCMotorDrive.o ${OBJECTDIR}/_ext/1386528437/FeedbackControl.o ${OBJECTDIR}/_ext/1386528437/FreeRunningTimer.o ${OBJECTDIR}/_ext/1386528437/NonVolatileMemory.o ${OBJECTDIR}/_ext/1386528437/RotaryEncoder.o ${OBJECTDIR}/lab5Application.o ${OBJECTDIR}/_ext/1386528437/delays.o

# Source Files
SOURCEFILES=../../../src/ADCFilter.c ../../../src/BOARD.c ../../../src/DCMotorDrive.c ../../../src/FeedbackControl.c ../../../src/FreeRunningTimer.c ../../../src/NonVolatileMemory.c ../../../src/RotaryEncoder.c lab5Application.c ../../../src/delays.c



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
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/lab5Application.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

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
${OBJECTDIR}/_ext/1386528437/ADCFilter.o: ../../../src/ADCFilter.c  .generated_files/e7954a38e2ee5d3e514d1106e380ac3de23b305e.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1386528437" 
	@${RM} ${OBJECTDIR}/_ext/1386528437/ADCFilter.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386528437/ADCFilter.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -MP -MMD -MF "${OBJECTDIR}/_ext/1386528437/ADCFilter.o.d" -o ${OBJECTDIR}/_ext/1386528437/ADCFilter.o ../../../src/ADCFilter.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1386528437/BOARD.o: ../../../src/BOARD.c  .generated_files/1e63997d2df5616ba7d62af9aff297f910a748b5.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1386528437" 
	@${RM} ${OBJECTDIR}/_ext/1386528437/BOARD.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386528437/BOARD.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -MP -MMD -MF "${OBJECTDIR}/_ext/1386528437/BOARD.o.d" -o ${OBJECTDIR}/_ext/1386528437/BOARD.o ../../../src/BOARD.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1386528437/DCMotorDrive.o: ../../../src/DCMotorDrive.c  .generated_files/cab492cc0aad8c13f359ee54a6ff9d00be876a5a.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1386528437" 
	@${RM} ${OBJECTDIR}/_ext/1386528437/DCMotorDrive.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386528437/DCMotorDrive.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -MP -MMD -MF "${OBJECTDIR}/_ext/1386528437/DCMotorDrive.o.d" -o ${OBJECTDIR}/_ext/1386528437/DCMotorDrive.o ../../../src/DCMotorDrive.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1386528437/FeedbackControl.o: ../../../src/FeedbackControl.c  .generated_files/a69e6b770f1590d5a73b7bb96993cec19fafece.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1386528437" 
	@${RM} ${OBJECTDIR}/_ext/1386528437/FeedbackControl.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386528437/FeedbackControl.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -MP -MMD -MF "${OBJECTDIR}/_ext/1386528437/FeedbackControl.o.d" -o ${OBJECTDIR}/_ext/1386528437/FeedbackControl.o ../../../src/FeedbackControl.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1386528437/FreeRunningTimer.o: ../../../src/FreeRunningTimer.c  .generated_files/2b8b70bde2060c83a9d94a86ac4e4f5c0ffdf51d.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1386528437" 
	@${RM} ${OBJECTDIR}/_ext/1386528437/FreeRunningTimer.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386528437/FreeRunningTimer.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -MP -MMD -MF "${OBJECTDIR}/_ext/1386528437/FreeRunningTimer.o.d" -o ${OBJECTDIR}/_ext/1386528437/FreeRunningTimer.o ../../../src/FreeRunningTimer.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1386528437/NonVolatileMemory.o: ../../../src/NonVolatileMemory.c  .generated_files/df79e857ab120078cbb90aeaaaa5e3bc097be6b1.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1386528437" 
	@${RM} ${OBJECTDIR}/_ext/1386528437/NonVolatileMemory.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386528437/NonVolatileMemory.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -MP -MMD -MF "${OBJECTDIR}/_ext/1386528437/NonVolatileMemory.o.d" -o ${OBJECTDIR}/_ext/1386528437/NonVolatileMemory.o ../../../src/NonVolatileMemory.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1386528437/RotaryEncoder.o: ../../../src/RotaryEncoder.c  .generated_files/1fa98b050d0038c73ee47801d335448563f26172.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1386528437" 
	@${RM} ${OBJECTDIR}/_ext/1386528437/RotaryEncoder.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386528437/RotaryEncoder.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -MP -MMD -MF "${OBJECTDIR}/_ext/1386528437/RotaryEncoder.o.d" -o ${OBJECTDIR}/_ext/1386528437/RotaryEncoder.o ../../../src/RotaryEncoder.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/lab5Application.o: lab5Application.c  .generated_files/51dd53147d0677cb677a4cf47dc45574d9219afb.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/lab5Application.o.d 
	@${RM} ${OBJECTDIR}/lab5Application.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -MP -MMD -MF "${OBJECTDIR}/lab5Application.o.d" -o ${OBJECTDIR}/lab5Application.o lab5Application.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1386528437/delays.o: ../../../src/delays.c  .generated_files/9bd3cd4e94cfc109f2fb5f100a50d84a95c5519d.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1386528437" 
	@${RM} ${OBJECTDIR}/_ext/1386528437/delays.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386528437/delays.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -MP -MMD -MF "${OBJECTDIR}/_ext/1386528437/delays.o.d" -o ${OBJECTDIR}/_ext/1386528437/delays.o ../../../src/delays.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
else
${OBJECTDIR}/_ext/1386528437/ADCFilter.o: ../../../src/ADCFilter.c  .generated_files/4578eb128c7f227e3c460a2d3b2195a51c853445.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1386528437" 
	@${RM} ${OBJECTDIR}/_ext/1386528437/ADCFilter.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386528437/ADCFilter.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -MP -MMD -MF "${OBJECTDIR}/_ext/1386528437/ADCFilter.o.d" -o ${OBJECTDIR}/_ext/1386528437/ADCFilter.o ../../../src/ADCFilter.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1386528437/BOARD.o: ../../../src/BOARD.c  .generated_files/dc81d94aa407f1c75de4b6df0014f57b70c49ce6.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1386528437" 
	@${RM} ${OBJECTDIR}/_ext/1386528437/BOARD.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386528437/BOARD.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -MP -MMD -MF "${OBJECTDIR}/_ext/1386528437/BOARD.o.d" -o ${OBJECTDIR}/_ext/1386528437/BOARD.o ../../../src/BOARD.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1386528437/DCMotorDrive.o: ../../../src/DCMotorDrive.c  .generated_files/3ad6e3d4322773a3b15f047ece06f888c88fd98f.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1386528437" 
	@${RM} ${OBJECTDIR}/_ext/1386528437/DCMotorDrive.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386528437/DCMotorDrive.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -MP -MMD -MF "${OBJECTDIR}/_ext/1386528437/DCMotorDrive.o.d" -o ${OBJECTDIR}/_ext/1386528437/DCMotorDrive.o ../../../src/DCMotorDrive.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1386528437/FeedbackControl.o: ../../../src/FeedbackControl.c  .generated_files/1eb48debe804712ddab1f45a7142c81c7644b0c7.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1386528437" 
	@${RM} ${OBJECTDIR}/_ext/1386528437/FeedbackControl.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386528437/FeedbackControl.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -MP -MMD -MF "${OBJECTDIR}/_ext/1386528437/FeedbackControl.o.d" -o ${OBJECTDIR}/_ext/1386528437/FeedbackControl.o ../../../src/FeedbackControl.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1386528437/FreeRunningTimer.o: ../../../src/FreeRunningTimer.c  .generated_files/1b9ba339f5540a75abadf610f3f1e51f7048e221.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1386528437" 
	@${RM} ${OBJECTDIR}/_ext/1386528437/FreeRunningTimer.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386528437/FreeRunningTimer.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -MP -MMD -MF "${OBJECTDIR}/_ext/1386528437/FreeRunningTimer.o.d" -o ${OBJECTDIR}/_ext/1386528437/FreeRunningTimer.o ../../../src/FreeRunningTimer.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1386528437/NonVolatileMemory.o: ../../../src/NonVolatileMemory.c  .generated_files/8088de3c81b7b34cf7ac7d8af24c422265dace77.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1386528437" 
	@${RM} ${OBJECTDIR}/_ext/1386528437/NonVolatileMemory.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386528437/NonVolatileMemory.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -MP -MMD -MF "${OBJECTDIR}/_ext/1386528437/NonVolatileMemory.o.d" -o ${OBJECTDIR}/_ext/1386528437/NonVolatileMemory.o ../../../src/NonVolatileMemory.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1386528437/RotaryEncoder.o: ../../../src/RotaryEncoder.c  .generated_files/e6c51df609145d5e629177c56461798a51408837.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1386528437" 
	@${RM} ${OBJECTDIR}/_ext/1386528437/RotaryEncoder.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386528437/RotaryEncoder.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -MP -MMD -MF "${OBJECTDIR}/_ext/1386528437/RotaryEncoder.o.d" -o ${OBJECTDIR}/_ext/1386528437/RotaryEncoder.o ../../../src/RotaryEncoder.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/lab5Application.o: lab5Application.c  .generated_files/52890af84c3cb68f1bf5906fbfc789cd15d0c05c.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/lab5Application.o.d 
	@${RM} ${OBJECTDIR}/lab5Application.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -MP -MMD -MF "${OBJECTDIR}/lab5Application.o.d" -o ${OBJECTDIR}/lab5Application.o lab5Application.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1386528437/delays.o: ../../../src/delays.c  .generated_files/35f8fc5710f5ffec8ad2cb1c1250bbd44f557823.flag .generated_files/71b2fa7f85a22426d8b136d9a94b6b5776fdc3b0.flag
	@${MKDIR} "${OBJECTDIR}/_ext/1386528437" 
	@${RM} ${OBJECTDIR}/_ext/1386528437/delays.o.d 
	@${RM} ${OBJECTDIR}/_ext/1386528437/delays.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../../include" -I"../../../src" -MP -MMD -MF "${OBJECTDIR}/_ext/1386528437/delays.o.d" -o ${OBJECTDIR}/_ext/1386528437/delays.o ../../../src/delays.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/lab5Application.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ../../../src/Protocol.X.a  
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g -mdebugger -D__MPLAB_DEBUGGER_PK3=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/lab5Application.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}    ../../../src/Protocol.X.a      -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)   -mreserve=data@0x0:0x1FC -mreserve=boot@0x1FC02000:0x1FC02FEF -mreserve=boot@0x1FC02000:0x1FC024FF  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=__MPLAB_DEBUGGER_PK3=1,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -mdfp="${DFP_DIR}"
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/lab5Application.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ../../../src/Protocol.X.a 
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/lab5Application.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}    ../../../src/Protocol.X.a      -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -mdfp="${DFP_DIR}"
	${MP_CC_DIR}/xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/lab5Application.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
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
