## -*- Makefile -*-
##
## Project: lan_outb.prd
## User: ems
## Time: 06/04/25 17:30:30
## Makefile created by Sun WorkShop.
##
## This file is generated automatically -- DO NOT EDIT.
##


include ../../../global.make

CFLAGS = $(BASICOPTS) -c

# Define the target directories.
TARGETDIR=.
TARGET=lan_outb.$(SUFFIX)

all: $(TARGETDIR)/$(TARGET)

CPPFLAGS += \
	-I/home/ems/system/dms/v4.0a/inc \
	-I/home/ems/system/lan/v4.0a/inc \
	-I/home/openwin/include \
	-I/home/dt/include 
	
OBJS = \
	$(TARGETDIR)/commonlib.o \
	$(TARGETDIR)/lan_outb.o \
	$(TARGETDIR)/lanlib.o \
	$(TARGETDIR)/netudp.o \
	$(TARGETDIR)/udp.o
	
USERLIBS = ../../../lan/v4.0a/lib/lan.lib ../../../dms/v4.0a/lib/dblib.lib ../../../dps/v4.0a/lib/ipclib.lib $(SYSLIBS) 
DEPLIBS =    
LDLIBS = $(USERLIBS)

# Link or archive
$(TARGETDIR)/$(TARGET): $(TARGETDIR) $(OBJS) $(DEPLIBS)
	$(LINK) -o $@ $(OBJS) $(LDLIBS)

# Compile source files into .o's
$(TARGETDIR)/commonlib.o: commonlib.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ commonlib.c
$(TARGETDIR)/lan_outb.o: lan_outb.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ lan_outb.c
$(TARGETDIR)/lanlib.o: lanlib.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ lanlib.c
$(TARGETDIR)/netudp.o: netudp.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ netudp.c
$(TARGETDIR)/udp.o: udp.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ udp.c


###### clean target: deletes generated files ######
clean:
	$(RM) \
	$(TARGETDIR)/$(TARGET) \
	$(OBJS)

# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE: .make.state
