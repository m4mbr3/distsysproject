#
# OMNeT++/OMNEST Makefile for shareRDS
#
# This file was generated with the command:
#  opp_makemake -f --deep
#

# Name of target to be created (-o option)
TARGET = shareRDS$(EXE_SUFFIX)

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I. \
    -Iapplication \
    -Icompmodules \
    -Iconcurrency \
    -Ifailure \
    -Iinvocation \
    -Imessages \
    -Imulticast \
    -Irecovery \
    -Ireplication \
    -Isynchronization \
    -Itest

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS =

# Output directory
PROJECT_OUTPUT_DIR = ../out
PROJECTRELATIVE_PATH = src
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = \
    $O/application/Application.o \
    $O/concurrency/DistMutualExclusion.o \
    $O/failure/ClientReincarnation.o \
    $O/failure/FailureManager.o \
    $O/invocation/InvocationManager.o \
    $O/multicast/AtomicMulticast.o \
    $O/multicast/BasicMulticast.o \
    $O/recovery/LogManager.o \
    $O/recovery/WriteAheadLog.o \
    $O/replication/DataItemsManager.o \
    $O/replication/RemoteWriteProtocol.o \
    $O/replication/ReplicaGroupManager.o \
    $O/synchronization/BarkleySynch.o \
    $O/synchronization/LamportClock.o \
    $O/test/Txc.o \
    $O/messages/CSynchMsg_m.o \
    $O/messages/SystemMsg_m.o

# Message files
MSGFILES = \
    messages/CSynchMsg.msg \
    messages/SystemMsg.msg

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc or configuser.vc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIB_SUBDIR = $(OMNETPP_LIB_DIR)/$(TOOLCHAIN_NAME)
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" -loppmain$D $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)

COPTS = $(CFLAGS)  $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)

# we want to recompile everything if COPTS changes,
# so we store COPTS into $COPTS_FILE and have object
# files depend on it (except when "make depend" was called)
COPTS_FILE = $O/.last-copts
ifneq ($(MAKECMDGOALS),depend)
ifneq ("$(COPTS)","$(shell cat $(COPTS_FILE) 2>/dev/null || echo '')")
$(shell $(MKPATH) "$O" && echo "$(COPTS)" >$(COPTS_FILE))
endif
endif

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# <<<
#------------------------------------------------------------------------------

# Main target
all: $O/$(TARGET)
	$(LN) $O/$(TARGET) .

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile
	@$(MKPATH) $O
	$(CXX) $(LDFLAGS) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

.PHONY: all clean cleanall depend msgheaders

.SUFFIXES: .cc

$O/%.o: %.cc $(COPTS_FILE)
	@$(MKPATH) $(dir $@)
	$(CXX) -c $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(MSGC) -s _m.cc $(MSGCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

clean:
	-rm -rf $O
	-rm -f shareRDS shareRDS.exe libshareRDS.so libshareRDS.a libshareRDS.dll libshareRDS.dylib
	-rm -f ./*_m.cc ./*_m.h
	-rm -f application/*_m.cc application/*_m.h
	-rm -f compmodules/*_m.cc compmodules/*_m.h
	-rm -f concurrency/*_m.cc concurrency/*_m.h
	-rm -f failure/*_m.cc failure/*_m.h
	-rm -f invocation/*_m.cc invocation/*_m.h
	-rm -f messages/*_m.cc messages/*_m.h
	-rm -f multicast/*_m.cc multicast/*_m.h
	-rm -f recovery/*_m.cc recovery/*_m.h
	-rm -f replication/*_m.cc replication/*_m.h
	-rm -f synchronization/*_m.cc synchronization/*_m.h
	-rm -f test/*_m.cc test/*_m.h

cleanall: clean
	-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc application/*.cc compmodules/*.cc concurrency/*.cc failure/*.cc invocation/*.cc messages/*.cc multicast/*.cc recovery/*.cc replication/*.cc synchronization/*.cc test/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/application/Application.o: application/Application.cc \
	application/Application.h
$O/concurrency/DistMutualExclusion.o: concurrency/DistMutualExclusion.cc \
	concurrency/DistMutualExclusion.h
$O/failure/ClientReincarnation.o: failure/ClientReincarnation.cc \
	failure/ClientReincarnation.h \
	failure/FailureManager.h \
	messages/SystemMsg_m.h \
	replication/DataItemsManager.h
$O/failure/FailureManager.o: failure/FailureManager.cc \
	failure/FailureManager.h
$O/invocation/InvocationManager.o: invocation/InvocationManager.cc \
	invocation/InvocationManager.h \
	messages/SystemMsg_m.h
$O/messages/CSynchMsg_m.o: messages/CSynchMsg_m.cc \
	messages/CSynchMsg_m.h
$O/messages/SystemMsg_m.o: messages/SystemMsg_m.cc \
	messages/SystemMsg_m.h
$O/multicast/AtomicMulticast.o: multicast/AtomicMulticast.cc \
	multicast/AtomicMulticast.h
$O/multicast/BasicMulticast.o: multicast/BasicMulticast.cc \
	multicast/BasicMulticast.h
$O/recovery/LogManager.o: recovery/LogManager.cc \
	recovery/LogManager.h
$O/recovery/WriteAheadLog.o: recovery/WriteAheadLog.cc \
	messages/SystemMsg_m.h \
	recovery/WriteAheadLog.h
$O/replication/DataItemsManager.o: replication/DataItemsManager.cc \
	messages/SystemMsg_m.h \
	replication/DataItemsManager.h
$O/replication/RemoteWriteProtocol.o: replication/RemoteWriteProtocol.cc \
	messages/SystemMsg_m.h \
	replication/RemoteWriteProtocol.h
$O/replication/ReplicaGroupManager.o: replication/ReplicaGroupManager.cc \
	messages/SystemMsg_m.h \
	replication/ReplicaGroupManager.h
$O/synchronization/BarkleySynch.o: synchronization/BarkleySynch.cc \
	synchronization/BarkleySynch.h
$O/synchronization/LamportClock.o: synchronization/LamportClock.cc \
	messages/SystemMsg_m.h \
	synchronization/LamportClock.h
$O/test/Txc.o: test/Txc.cc \
	test/Txc.h

