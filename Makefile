#-----------------------------------------------------------------------------
#
#                     *** Acqu++ <-> Root ***
#
#    Online/Offline Analysis of Sub-Atomic Physics Experimental Data.
#
#		************* Makefile ****************
#
#			     GNU Version
#
#	Unix/GNU  makefile to compile and link software modules
#	of the online/offline  data acquisition system
#
#	JRM Annand	 1st Feb 2003	3v5 update
#	JRM Annand	 8th Apr 2003	Add Acqu-Root PRELIMINARY
#	JRM Annand	 8th Jan 2004	debug switch from environment
#	JRM Annand	20th Nov 2004	LongBar_t
#	JRM Annand	 1st Dec 2004   Update class list
#	JRM Annand	19th Oct 2005   Add MCGenerator
#	JRM Annand	18th Jan 2007   LDFLAGS change
#       JRM Annand      22nd Jan 2007   4v0 update
#	JRM Annand	30th Jan 2007   Add AcquDAQ
#       Ken Livingston  14th May 2007   Changed binary directories to 
#                                       $(acqu_sys)/obj/$(ACQU_OS_NAME) etc.
#                                       Dependencies in deps/$(ACQU_OS_NAME)
#                                       $(ACQU_OS_NAME) may be "" (acqu_setup)
#	JRM Annand	 6th Jul 2007   Include stuff after 14th May
#       JRM Annand	23rd Aug 2007	deps directory bug fix (K.Livingston)
#	JRM Annand	18th Nov 2007	ROOT includes after Acqu
#	JRM Annand	14th May 2008	Install dummy CAEN libs if not there
#	JRM Annand	 3rd Jun 2008	Conditional comp for CAEN hardware
#	JRM Annand	 2nd Oct 2008	Add TA2TOFApparatus
#	Ken Livingston	11th Jan 2013	Add support for including EPICS in the data stream
#	JRM Annand	 6th Mar 2013	Add TA2TAPSMk2Format
#
#----------------------------------------------------------------------------
#
# Directories
IR = $(ROOTSYS)/include
O = $(acqu_sys)/obj/$(ACQU_OS_NAME)
B = $(acqu_sys)/bin/$(ACQU_OS_NAME)
L = $(acqu_sys)/lib/$(ACQU_OS_NAME)
AR = $(acqu_sys)/AcquRoot/src
MC = $(acqu_sys)/AcquMC/src
D = $(acqu_sys)/deps/$(ACQU_OS_NAME)
EZCA = $(acqu_sys)/ezcaRoot/src
#
DAQ = $(acqu_sys)/AcquDAQ/src
DAQV = $(acqu_sys)/AcquDAQ/vme
#
#acqu_debug = -g
#
# ROOT libraries etc.
ROOTLIBS     := $(shell root-config --glibs) -lEG
#-lGuiBld -lGuiHtml -lGX11 -lGX11TTF
ROOTCFLAGS   := $(shell root-config --auxcflags)
#
# Compile/Link Flags
# Remember there is an alias makedeb which turns on debug mode
#
CXXFLAGS  = $(acqu_debug) -c -Wall -fPIC $(ROOTCFLAGS)
LDXXFLAGS = $(acqu_debug) -shared -o $L/$@
#INCFLAGS  = -I$(IR)
#
CLEANMOD = $B/AcquRoot $B/AcquMC $B/AcquDAQ
#
# Assume that if EPICS_EXTENSIONS is defined from environment epics is installed and can be compiled
ifdef EPICS_EXTENSIONS
	EPICSSO = libRootEzca.so
	EZCALIBS = -L$(EPICS_BASE)/lib/$(EPICS_HOST_ARCH) -L$(EPICS_EXTENSIONS)/lib/$(EPICS_HOST_ARCH) -lca -lCom -lezca
	EZCAINC = -I$(EZCA) -I$(EPICS_BASE)/include -I$(EPICS_BASE)/include/os/Linux -I$(EPICS_EXTENSIONS)/src/ezca
	EPICSFLAGS = -DACQUROOT_EPICS $(EZCAINC)
	EPICSLIB = -L$(L) -lRootEzca
	EPICSLINKS = epicslinks
endif
#-----------------------------------------------------------------------------
#
#	Do the whole lot
#
all: AcquRoot AcquMC AcquDAQ RSupervise docs
#
#	Get rid of all binaries and obj 
veryclean: clean
	@rm -f $(L)/libAcqu*.so
	@rm -f $(CLEANMOD)
	@rm -f *.log
	@rm -f $(D)/*.d
#	@rm -f $(AR)/*.d
#	@rm -f $(MC)/*.d
#	@rm -f $(DAQ)/*.d
	@rm -rf htmldoc
	@echo Acqu++ Flush Complete
ultraclean: veryclean
	@rm -f ./*~
	@rm -f ./#*#
	@rm -f $(AR)/*.cc~
	@rm -f $(AR)/AcquRootDict.*
	@rm -f $(AR)/*.h~
	@rm -f $(AR)/#*#
	@rm -f $(AR)/../macros/*.C~
	@rm -f $(MC)/*.cc~
	@rm -f $(MC)/MCDict.*
	@rm -f $(MC)/*.h~
	@rm -f $(MC)/#*#
	@rm -f $(MC)/../macros/*.C~
	@rm -f $(DAQ)/*.cc~
	@rm -f $(DAQ)/DAQDict.*
	@rm -f $(DAQ)/*.h~
	@rm -f $(DAQ)/#*#
	@rm -f $(DAQ)/../macros/*.C~
	@rm -f $(EZCA)/ezcaRootDict.*
	@rm -f $(EZCA)/.cc~
	@echo  Acqu++ Purge Complete
#
clean:
	@rm -f $(O)/*
	@rm -f core*
	@echo Binaries and Cores deleted

# Make gzipped tarball of source and setup files
Ad = $(acquversionID)/acqu
archive:
	cd; tar -zcvf $(HOME)/$(acquversionID)_source.tar.gz \
	$(Ad)/Makefile \
	$(Ad)/acqu_setup $(Ad)/README $(Ad)/Release.Notes \
	$(Ad)/AcquRoot/src/*.cc $(Ad)/AcquRoot/src/*.h \
	$(Ad)/AcquRoot/macros/*.C \
	$(Ad)/AcquMC/src/*.cc $(Ad)/AcquMC/src/*.h $(Ad)/AcquMC/macros/*.C \
	$(Ad)/AcquDAQ/src/*.cc $(Ad)/AcquDAQ/src/*.h $(Ad)/AcquDAQ/macros/*.C \
	$(Ad)/AcquDAQ/data/* \
	$(Ad)/AcquDAQ/vme/*.h $(Ad)/AcquDAQ/vme/LinkCAENlibs \
	$(Ad)/ezcaRoot/README \
	$(Ad)/ezcaRoot/Uname \
	$(Ad)/ezcaRoot/macros \
	$(Ad)/ezcaRoot/src \
	$(Ad)/bin/Acqu* $(Ad)/bin/DownloadRAM \
	$(Ad)/User/Makefile $(Ad)/User/Makefile.user $(Ad)/User/.acqurc \
	$(Ad)/User/.rootrc $(Ad)/User/.tcshrc \
	$(Ad)/User/README
#	$(Ad)/User/$(acquversionID)_UserSetup.tar.gz \
	chmod a-x $(HOME)/$(acquversionID)_source.tar.gz
#
userarchive:
	cd User; tar zcvf UserAcqu.tar.gz .acqurc .rootrc acqu
#
vpath %.cc $(AR):$(MC):$(DAQ)
vpath %.c $(AR):$(MC):$(DAQ):$(DAQV)
vpath %.f $(F)
vpath %.h $(AR):$(MC):$(DAQ)
vpath %.o $(O)
vpath %.d $(D)
#
# AcquRoot Classes
#
ARClasses = \
	TA2System TA2HistManager TA2DataManager TA2Control TAcquRoot   \
	TA2Analysis \
	TA2Physics TA2GenericPhysics \
	TA2Apparatus TA2GenericApparatus TA2TOFApparatus TA2Tagger\
	TA2Detector TA2GenericDetector TA2ClusterDetector TA2LongScint \
	TA2Ladder TA2GenericCluster \
	TA2WireChamber \
	TA2H TA2Cut TA2MultiCut TA2Track \
	TA2WCLayer TA2CylWire TA2CylStrip \
	TA2BitPattern TA2RateMonitor TA2ParticleID TA2Particle \
	TA2RingBuffer \
	TA2DataServer TA2DataSource TA2NetSource TA2FileSource TA2LocalSource \
	TA2DataFormat TA2Mk1Format TA2Mk2Format TA2TAPSFormat TA2TAPSMk2Format \
	HitD2A_t LongBar_t HitCluster_t GenHitCluster_t
#
ARClassesA = $(ARClasses) TA2H1 TA2H2 ARSocket_t ARFile_t
ARModules = $(ARClassesA) AcquRootDict
ARSrc = $(addsuffix .cc, $(ARModules))
ARSrcD = $(addprefix $(AR)/, $(ARObj))
ARObj = $(addsuffix .o, $(ARModules))
ARObjD = $(addprefix $O/, $(ARObj))
ARInc = $(addsuffix .h, $(ARClasses))
#ARIncD = $(addprefix src/, $(ARObj))
# Dependencies
ARModulesA = $(ARClassesA) MainAcquRoot
ARDep = $(addsuffix .d, $(ARModulesA))
#ARDepD =  $(addprefix $(AR)/, $(ARDep))
ARDepD =  $(addprefix $(D)/, $(ARDep))
INCFLAGS += -I$(AR)


##This is to build a ROOT shared library for EPICS (not acqu specific)

libRootEzca.so: $(EZCA)/ezcaRootDict.cc $(O)/ezcaRoot.o $(O)/ezcaRootDict.o $(EPICSLINKS)
	$(CCCOMP) $(LDXXFLAGS) $(EZCALIBS) $(O)/ezcaRoot.o $(O)/ezcaRootDict.o
	@echo "$@ done"
	@echo "##-----------------------------------------------------------"

$(EZCA)/ezcaRootDict.cc: $(EZCA)/ezcaRoot.h $(EZCA)/ezcaRootLinkDef.h
	@echo "Generating dictionary $@..."
	@rootcint  -f $@ -c $^
	@echo "##-----------------------------------------------------------"

$(O)/ezcaRoot.o: $(EZCA)/ezcaRoot.cc  
	$(CCCOMP) $(CXXFLAGS) $(EZCAINC) $(INCFLAGS) -o $@ -c $<

$(O)/ezcaRootDict.o: $(EZCA)/ezcaRootDict.cc  
	$(CCCOMP) $(CXXFLAGS) $(EZCAINC) $(INCFLAGS) -o $@ -c $<

$(EPICSLINKS):
#link the epics libraries into the acqusys/lib directory so users will pick them up with usermake, and running
#remove old links first in case out of date
	rm -f $(L)/libca.so 
	rm -f $(L)/libCom.so
	rm -f $(L)/libezca.so
	ln -s $(EPICS_BASE)/lib/$(EPICS_HOST_ARCH)/libca.so $(L)/libca.so
	ln -s $(EPICS_BASE)/lib/$(EPICS_HOST_ARCH)/libCom.so $(L)/libCom.so
	ln -s $(EPICS_EXTENSIONS)/lib/$(EPICS_HOST_ARCH)/libezca.so $(L)/libezca.so

libAcquRoot.so: $(ARObj)
	$(CCCOMP) $(LDXXFLAGS) $(ARObjD)
	@echo "$@ done"
	@echo "##-----------------------------------------------------------"
#
# AcquMC Classes
#
MCCl = $(wildcard $(MC)/TMC*.cc)
#MCCl_t = $(wildcard $(MC)/MC*.cc)
MCSrc = $(MCCl) $(MC)/MCDict.cc
MCInc = $(patsubst $(MC)/%.cc, $(MC)/%.h, $(MCCl))
MCObjD = $(patsubst $(MC)/%.cc, $(O)/%.o, $(MCSrc))
MCObj = $(notdir $(MCObjD))
#MCDep = $(patsubst $(MC)/%.cc, $(MC)/%.d, $(MCCl)) $(MC)/MainAcquMC.d
#MCDepInc = -I$(MC)
MCDep = $(patsubst $(MC)/%.cc, $(D)/%.d, $(MCCl)) $(D)/MainAcquMC.d
#MCDepInc = -I$(MC)
INCFLAGS += -I$(MC)
#
libAcquMC.so: $(MCObj)
	$(CCCOMP) $(LDXXFLAGS) $(MCObjD)
	@echo "$@ done"
	@echo "##-----------------------------------------------------------"
#
# AcquDAQ Classes
#
DAQCl = $(wildcard $(DAQ)/T*.cc) $(DAQ)/DAQMemMap_t.cc
DAQInc = $(patsubst $(DAQ)/%.cc, $(DAQ)/%.h, $(DAQCl))
##DAQCl += $(DAQ)/DAQMemMap_t.cc
DAQSrc = $(DAQCl) $(DAQ)/DAQDict.cc
DAQObjD = $(patsubst $(DAQ)/%.cc, $(O)/%.o, $(DAQSrc))
DAQObj = $(notdir $(DAQObjD))
#DAQDep = $(patsubst $(DAQ)/%.cc, $(DAQ)/%.d, $(DAQCl)) $(DAQ)/MainAcquDAQ.d
#DAQDepInc = -I$(DAQ) -I$(DAQV)
DAQDep = $(patsubst $(DAQ)/%.cc, $(D)/%.d, $(DAQCl)) $(D)/MainAcquDAQ.d
#DAQDepInc = -I$(DAQ) -I$(DAQV)
INCFLAGS += -I$(DAQ) -I$(DAQV) -I$(IR)
#
libAcquDAQ.so: $(DAQObj) $(EPICSSO)
	$(CCCOMP) $(LDXXFLAGS) $(DAQObjD)
	@echo "$@ done"
	@echo "##-----------------------------------------------------------"
#
#---------------------------------------------------------------------------
# Programs

AcquRoot:MainAcquRoot.o libAcquRoot.so libAcquDAQ.so $(EPICSSO)
	$(CCCOMP) $(acqu_debug) $O/MainAcquRoot.o $L/libAcquRoot.so \
	$(ROOTLIBS) $(EPICSLIB) -lThread \
	$(L)/libAcquDAQ.so \
	$(CAENVME_LIB) \
	-o $(B)/AcquRoot
	@echo "$@ done"
	@echo "##-----------------------------------------------------------"

AcquMC: MainAcquMC.o libAcquMC.so libAcquRoot.so libAcquDAQ.so $(EPICSSO)
#	@rm -f $(MC)/MCDict.*
	$(CCCOMP) $(acqu_debug) $(O)/MainAcquMC.o \
	$L/libAcquRoot.so $(L)/libAcquMC.so \
	$L/libAcquDAQ.so \
	$(CAENVME_LIB) \
	$(ROOTLIBS) $(EPICSLIB) -lThread -lFoam \
	-o $(B)/AcquMC
	@echo "$@ done"
	@echo "##-----------------------------------------------------------"

AcquDAQ: MainAcquDAQ.o libAcquDAQ.so libAcquRoot.so $(EPICSSO)
	$(CCCOMP) $(acqu_debug)  $(O)/MainAcquDAQ.o \
	$(L)/libAcquDAQ.so $(L)/libAcquRoot.so \
	$(CAENVME_LIB) \
	$(ROOTLIBS) $(EPICSLIB) -lThread \
	-o $(B)/AcquDAQ
	@echo "$@ done"
	@echo "##-----------------------------------------------------------"

AcquHV: MainHV.o libAcquDAQ.so libAcquRoot.so  $(EPICSSO)
	$(CCCOMP) $(acqu_debug)  $(O)/MainHV.o \
	$(L)/libAcquDAQ.so $(L)/libAcquRoot.so \
	$(CAENVME_LIB) \
	$(ROOTLIBS) -lThread \
	-o $(B)/AcquHV
	@echo "$@ done"
	@echo "##-----------------------------------------------------------"

RSupervise: RSupervise.o libAcquRoot.so $(EPICSSO) $(EPICSLINKS)
	$(CCCOMP) $(acqu_debug)  $(O)/RSupervise.o \
	$(L)/libAcquRoot.so $(L)/libAcquDAQ.so $(CAENVME_LIB) \
	$(ROOTLIBS) $(EPICSLIB) -lThread \
	-o $(B)/RSupervise
	@echo "$@ done"
	@echo "##-----------------------------------------------------------"

TapeDir: TapeDir.o
	$(CCCOMP) $(acqu_debug)  $(O)/TapeDir.o \
	$(ROOTLIBS) -lThread \
	-o $(B)/TapeDir
	@echo "$@ done"
	@echo "##-----------------------------------------------------------"

docs:
	@echo " Invoking ROOT html automatic documentation"
	@rm -rf $(acqu_sys)/htmldoc
	@cd $(acqu_sys)
	root -b -n -q htmldoc.C


$(AR)/AcquRootDict.cc: $(ARInc) AcquRootLinkDef.h
	@echo "Generating dictionary $@..."
	@rootcint  -f $@ -c $(INCFLAGS) $^
	@echo "##-----------------------------------------------------------"

$(MC)/MCDict.cc: $(MCInc) MCLinkDef.h
	@echo "Generating dictionary $@..."
	@rootcint  -f $@ -c $(INCFLAGS) $^
	@echo "##-----------------------------------------------------------"

$(DAQ)/DAQDict.cc: $(DAQInc) AcquDAQLinkDef.h
	@echo "Generating dictionary $@..."
	@rootcint  -f $@ -c $(INCFLAGS) $^
	@echo "##-----------------------------------------------------------"

%.o: %.cc
	$(CCCOMP) $(CXXFLAGS) $(EPICSFLAGS) $(INCFLAGS) $(CAENVME_COMP) -o $(O)/$@ -c $< 
#
# Need the explicit target directory for deps. vpath doesn't seem to work
# for this
#
$(D)/%.d: %.cc
	@echo Making dependencies for $<
	@$(SHELL) -ec 'gcc -MM $(INCFLAGS) -c $< \
		| sed '\''s%^.*\.ox%$*\.o%g'\'' \
		| sed '\''s%\($*\)\.o[ :]*%\1.o $@ : %g'\'' > $@; \
		[ -s $@ ] || rm -f $@'

-include $(ARDepD)
-include $(MCDep)
-include $(DAQDep)
