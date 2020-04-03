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
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/Saat_Termometre_DHT22.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/Saat_Termometre_DHT22.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=
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
SOURCEFILES_QUOTED_IF_SPACED=Src/main.c Src/hc595.c Src/dht22.c Src/ds3231.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/Src/main.o ${OBJECTDIR}/Src/hc595.o ${OBJECTDIR}/Src/dht22.o ${OBJECTDIR}/Src/ds3231.o
POSSIBLE_DEPFILES=${OBJECTDIR}/Src/main.o.d ${OBJECTDIR}/Src/hc595.o.d ${OBJECTDIR}/Src/dht22.o.d ${OBJECTDIR}/Src/ds3231.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/Src/main.o ${OBJECTDIR}/Src/hc595.o ${OBJECTDIR}/Src/dht22.o ${OBJECTDIR}/Src/ds3231.o

# Source Files
SOURCEFILES=Src/main.c Src/hc595.c Src/dht22.c Src/ds3231.c



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
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/Saat_Termometre_DHT22.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_CC="D:\Applications\PICC\CCSCON.exe"
MP_LD="D:\Applications\PICC\CCSCON.exe"
sourceline.device=sourceline="\#device PIC16F88"
sourceline.xccompat=sourceline="\#device PASS_STRINGS=IN_RAM" sourceline="\#device CONST=READ_ONLY" sourceline="\#case" sourceline="\#TYPE SIGNED" sourceline="\#type INT=16, LONG=32"
sourceline.gcccompat=sourceline="\#device PASS_STRINGS=IN_RAM" sourceline="\#device CONST=READ_ONLY" sourceline="\#case" sourceline="\#TYPE SIGNED"
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/Src/main.o: Src/main.c  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} ${OBJECTDIR}/Src 
ifeq (4,1) 
	${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC}  out="${OBJECTDIR}\Src"  Src/main.c +FM +DF +CC +Y=0 +EA I+="Inc" I+="D:\Applications\PICC\Devices" +DF +LN +T +A +M +J +Z -P #__16F88=1 
	@mv ${OBJECTDIR}/Src/main.cof "dist/${CND_CONF}/${IMAGE_TYPE}/Saat_Termometre_DHT22.X.${IMAGE_TYPE}.cof" 
	@mv ${OBJECTDIR}/Src/main.hex "dist/${CND_CONF}/${IMAGE_TYPE}/Saat_Termometre_DHT22.X.${IMAGE_TYPE}.hex"
else 
	${MP_CC}  out=""${OBJECTDIR}\Src"" Src/main.c +EXPORT +FM +DF +CC +Y=0 +EA I+="Inc" I+="D:\Applications\PICC\Devices" +DF +LN +T +A +M +J +Z -P #__16F88=1 +EXPORTD="${OBJECTDIR}/Src"  
	
endif 
	
${OBJECTDIR}/Src/hc595.o: Src/hc595.c  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} ${OBJECTDIR}/Src 
ifeq (4,1) 
	${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC}  out="${OBJECTDIR}\Src"  Src/hc595.c +FM +DF +CC +Y=0 +EA I+="Inc" I+="D:\Applications\PICC\Devices" +DF +LN +T +A +M +J +Z -P #__16F88=1 
	@mv ${OBJECTDIR}/Src/hc595.cof "dist/${CND_CONF}/${IMAGE_TYPE}/Saat_Termometre_DHT22.X.${IMAGE_TYPE}.cof" 
	@mv ${OBJECTDIR}/Src/hc595.hex "dist/${CND_CONF}/${IMAGE_TYPE}/Saat_Termometre_DHT22.X.${IMAGE_TYPE}.hex"
else 
	${MP_CC}  out=""${OBJECTDIR}\Src"" Src/hc595.c +EXPORT +FM +DF +CC +Y=0 +EA I+="Inc" I+="D:\Applications\PICC\Devices" +DF +LN +T +A +M +J +Z -P #__16F88=1 +EXPORTD="${OBJECTDIR}/Src"  
	
endif 
	
${OBJECTDIR}/Src/dht22.o: Src/dht22.c  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} ${OBJECTDIR}/Src 
ifeq (4,1) 
	${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC}  out="${OBJECTDIR}\Src"  Src/dht22.c +FM +DF +CC +Y=0 +EA I+="Inc" I+="D:\Applications\PICC\Devices" +DF +LN +T +A +M +J +Z -P #__16F88=1 
	@mv ${OBJECTDIR}/Src/dht22.cof "dist/${CND_CONF}/${IMAGE_TYPE}/Saat_Termometre_DHT22.X.${IMAGE_TYPE}.cof" 
	@mv ${OBJECTDIR}/Src/dht22.hex "dist/${CND_CONF}/${IMAGE_TYPE}/Saat_Termometre_DHT22.X.${IMAGE_TYPE}.hex"
else 
	${MP_CC}  out=""${OBJECTDIR}\Src"" Src/dht22.c +EXPORT +FM +DF +CC +Y=0 +EA I+="Inc" I+="D:\Applications\PICC\Devices" +DF +LN +T +A +M +J +Z -P #__16F88=1 +EXPORTD="${OBJECTDIR}/Src"  
	
endif 
	
${OBJECTDIR}/Src/ds3231.o: Src/ds3231.c  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} ${OBJECTDIR}/Src 
ifeq (4,1) 
	${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC}  out="${OBJECTDIR}\Src"  Src/ds3231.c +FM +DF +CC +Y=0 +EA I+="Inc" I+="D:\Applications\PICC\Devices" +DF +LN +T +A +M +J +Z -P #__16F88=1 
	@mv ${OBJECTDIR}/Src/ds3231.cof "dist/${CND_CONF}/${IMAGE_TYPE}/Saat_Termometre_DHT22.X.${IMAGE_TYPE}.cof" 
	@mv ${OBJECTDIR}/Src/ds3231.hex "dist/${CND_CONF}/${IMAGE_TYPE}/Saat_Termometre_DHT22.X.${IMAGE_TYPE}.hex"
else 
	${MP_CC}  out=""${OBJECTDIR}\Src"" Src/ds3231.c +EXPORT +FM +DF +CC +Y=0 +EA I+="Inc" I+="D:\Applications\PICC\Devices" +DF +LN +T +A +M +J +Z -P #__16F88=1 +EXPORTD="${OBJECTDIR}/Src"  
	
endif 
	
else
${OBJECTDIR}/Src/main.o: Src/main.c  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} ${OBJECTDIR}/Src 
ifeq (4,1) 
	${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC}  out="${OBJECTDIR}\Src"  Src/main.c +FM +DF +CC +Y=0 +EA I+="Inc" I+="D:\Applications\PICC\Devices" +DF +LN +T +A +M +J +Z -P #__16F88=1 
	@mv ${OBJECTDIR}/Src/main.cof "dist/${CND_CONF}/${IMAGE_TYPE}/Saat_Termometre_DHT22.X.${IMAGE_TYPE}.cof" 
	@mv ${OBJECTDIR}/Src/main.hex "dist/${CND_CONF}/${IMAGE_TYPE}/Saat_Termometre_DHT22.X.${IMAGE_TYPE}.hex"
else 
	${MP_CC}  out=""${OBJECTDIR}\Src"" Src/main.c +EXPORT +FM +DF +CC +Y=0 +EA I+="Inc" I+="D:\Applications\PICC\Devices" +DF +LN +T +A +M +J +Z -P #__16F88=1 +EXPORTD="${OBJECTDIR}/Src"  
	
endif 
	
${OBJECTDIR}/Src/hc595.o: Src/hc595.c  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} ${OBJECTDIR}/Src 
ifeq (4,1) 
	${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC}  out="${OBJECTDIR}\Src"  Src/hc595.c +FM +DF +CC +Y=0 +EA I+="Inc" I+="D:\Applications\PICC\Devices" +DF +LN +T +A +M +J +Z -P #__16F88=1 
	@mv ${OBJECTDIR}/Src/hc595.cof "dist/${CND_CONF}/${IMAGE_TYPE}/Saat_Termometre_DHT22.X.${IMAGE_TYPE}.cof" 
	@mv ${OBJECTDIR}/Src/hc595.hex "dist/${CND_CONF}/${IMAGE_TYPE}/Saat_Termometre_DHT22.X.${IMAGE_TYPE}.hex"
else 
	${MP_CC}  out=""${OBJECTDIR}\Src"" Src/hc595.c +EXPORT +FM +DF +CC +Y=0 +EA I+="Inc" I+="D:\Applications\PICC\Devices" +DF +LN +T +A +M +J +Z -P #__16F88=1 +EXPORTD="${OBJECTDIR}/Src"  
	
endif 
	
${OBJECTDIR}/Src/dht22.o: Src/dht22.c  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} ${OBJECTDIR}/Src 
ifeq (4,1) 
	${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC}  out="${OBJECTDIR}\Src"  Src/dht22.c +FM +DF +CC +Y=0 +EA I+="Inc" I+="D:\Applications\PICC\Devices" +DF +LN +T +A +M +J +Z -P #__16F88=1 
	@mv ${OBJECTDIR}/Src/dht22.cof "dist/${CND_CONF}/${IMAGE_TYPE}/Saat_Termometre_DHT22.X.${IMAGE_TYPE}.cof" 
	@mv ${OBJECTDIR}/Src/dht22.hex "dist/${CND_CONF}/${IMAGE_TYPE}/Saat_Termometre_DHT22.X.${IMAGE_TYPE}.hex"
else 
	${MP_CC}  out=""${OBJECTDIR}\Src"" Src/dht22.c +EXPORT +FM +DF +CC +Y=0 +EA I+="Inc" I+="D:\Applications\PICC\Devices" +DF +LN +T +A +M +J +Z -P #__16F88=1 +EXPORTD="${OBJECTDIR}/Src"  
	
endif 
	
${OBJECTDIR}/Src/ds3231.o: Src/ds3231.c  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} ${OBJECTDIR}/Src 
ifeq (4,1) 
	${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC}  out="${OBJECTDIR}\Src"  Src/ds3231.c +FM +DF +CC +Y=0 +EA I+="Inc" I+="D:\Applications\PICC\Devices" +DF +LN +T +A +M +J +Z -P #__16F88=1 
	@mv ${OBJECTDIR}/Src/ds3231.cof "dist/${CND_CONF}/${IMAGE_TYPE}/Saat_Termometre_DHT22.X.${IMAGE_TYPE}.cof" 
	@mv ${OBJECTDIR}/Src/ds3231.hex "dist/${CND_CONF}/${IMAGE_TYPE}/Saat_Termometre_DHT22.X.${IMAGE_TYPE}.hex"
else 
	${MP_CC}  out=""${OBJECTDIR}\Src"" Src/ds3231.c +EXPORT +FM +DF +CC +Y=0 +EA I+="Inc" I+="D:\Applications\PICC\Devices" +DF +LN +T +A +M +J +Z -P #__16F88=1 +EXPORTD="${OBJECTDIR}/Src"  
	
endif 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/Saat_Termometre_DHT22.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
ifeq (4,1) 
	
else 
	${MP_LD}   out="dist/${CND_CONF}/${IMAGE_TYPE}"  +FM +DF +CC LINK=Saat_Termometre_DHT22.X.${IMAGE_TYPE}.hex=${OBJECTDIR}/Src/main.o,${OBJECTDIR}/Src/hc595.o,${OBJECTDIR}/Src/dht22.o,${OBJECTDIR}/Src/ds3231.o +Y=0 +EA I+="Inc" I+="D:\Applications\PICC\Devices" +DF +LN +T +A +M +J +Z -P
	
endif 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/Saat_Termometre_DHT22.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
ifeq (4,1) 
	
else 
	${MP_LD}   out="dist/${CND_CONF}/${IMAGE_TYPE}"  +FM +DF +CC LINK=Saat_Termometre_DHT22.X.${IMAGE_TYPE}.hex=${OBJECTDIR}/Src/main.o,${OBJECTDIR}/Src/hc595.o,${OBJECTDIR}/Src/dht22.o,${OBJECTDIR}/Src/ds3231.o +Y=0 +EA I+="Inc" I+="D:\Applications\PICC\Devices" +DF +LN +T +A +M +J +Z -P
	
endif 
	
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
