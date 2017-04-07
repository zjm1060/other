## -*- Makefile -*-
##
## Project: plcscan.prd
## User: ems
## Time: 06/04/20 14:20:39
## Makefile created by Sun WorkShop.
##
## This file is generated automatically -- DO NOT EDIT.
##

#### Compiler and tool definitions shared by all build targets #####
include ../../../global.make

CFLAGS = $(BASICOPTS) -c

# Define the target directories.
TARGETDIR=.
TARGET=plcscan.$(SUFFIX)

all: $(TARGETDIR)/$(TARGET)

CPPFLAGS += \
	-I/home/ems/system/plc/v4.0a/inc \
	-I/home/openwin/include \
	-I/home/dt/include 
	
OBJS = \
	$(TARGETDIR)/geplcnet.o \
	$(TARGETDIR)/mbplcnet.o \
	$(TARGETDIR)/sieplcnet.o \
	$(TARGETDIR)/abplcnet.o \
	$(TARGETDIR)/plclib.o \
	$(TARGETDIR)/plcscan.o \
	$(TARGETDIR)/scanapp.o

USERLIBS = ../../../lan/v4.0a/lib/lan.lib ../../../dms/v4.0a/lib/dblib.lib ../../../dps/v4.0a/lib/ipclib.lib ../../../plc/v4.0a/lib/plc.lib $(SYSLIBS) 
DEPLIBS =    
LDLIBS = $(USERLIBS)

# Link or archive
$(TARGETDIR)/$(TARGET): $(TARGETDIR) $(OBJS) $(DEPLIBS)
	$(LINK) -o $@ $(OBJS) $(LDLIBS)

# Compile source files into .o's
$(TARGETDIR)/geplcnet.o: geplcnet.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ geplcnet.c
$(TARGETDIR)/mbplcnet.o: mbplcnet.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ mbplcnet.c
$(TARGETDIR)/sieplcnet.o: sieplcnet.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ sieplcnet.c
$(TARGETDIR)/abplcnet.o: abplcnet.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ abplcnet.c
$(TARGETDIR)/plclib.o: plclib.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ plclib.c
$(TARGETDIR)/plcscan.o: plcscan.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ plcscan.c
$(TARGETDIR)/scanapp.o: scanapp.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ scanapp.c


###### clean target: deletes generated files ######
clean:
	$(RM) \
	$(TARGETDIR)/$(TARGET) \
	$(OBJS)

# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE: .make.state
