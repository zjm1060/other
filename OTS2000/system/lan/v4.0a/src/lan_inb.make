## -*- Makefile -*-
##
## Project: lan_inb.prd
## User: ems
## Time: 06/04/25 17:37:00
## Makefile created by Sun WorkShop.
##
## This file is generated automatically -- DO NOT EDIT.
##


##### Compilers and tools definitions shared by all build objects #####
include ../../../global.make

CFLAGS = $(BASICOPTS) -c

# Define the target directories.
TARGETDIR=.
TARGET=lan_inb.$(SUFFIX)

all: $(TARGETDIR)/$(TARGET)

OBJS = \
	$(TARGETDIR)/lan_inb.o \
	$(TARGETDIR)/netudp.o \
	$(TARGETDIR)/udp.o
	
USERLIBS = ../../../lan/v4.0a/lib/lan.lib ../../../dms/v4.0a/lib/dblib.lib ../../../dps/v4.0a/lib/ipclib.lib $(SYSLIBS) 
DEPLIBS =    
LDLIBS = $(USERLIBS)

# Link or archive
$(TARGETDIR)/$(TARGET): $(TARGETDIR) $(OBJS) $(DEPLIBS)
	$(LINK) -o $@ $(OBJS) $(LDLIBS)

# Compile source files into .o's
$(TARGETDIR)/lan_inb.o: $(TARGETDIR) lan_inb.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ lan_inb.c
$(TARGETDIR)/netudp.o: $(TARGETDIR) netudp.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ netudp.c
$(TARGETDIR)/udp.o: $(TARGETDIR) udp.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ udp.c


###### clean target: deletes generated files ######
clean:
	$(RM) \
	$(TARGETDIR)/$(TARGET)	\
	$(OBJS)

# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE: .make.state
