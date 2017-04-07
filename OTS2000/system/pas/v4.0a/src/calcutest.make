## -*- Makefile -*-
##
## Project: calcutest.prd
## User: ems
## Time: 06/03/27 21:29:18
## Makefile created by Sun WorkShop.
##
## This file is generated automatically -- DO NOT EDIT.
##

#### Compiler and tool definitions shared by all build targets #####
include ../../../global.make

CFLAGS = $(BASICOPTS) -c

# Define the target directories.
TARGETDIR=.
TARGET=calcutest.$(SUFFIX)

all: $(TARGETDIR)/$(TARGET)

CPPFLAGS += \
	-I/usr/openwin/include \
	-I/usr/dt/include 
	
OBJS = \
	$(TARGETDIR)/calcufn.o \
	$(TARGETDIR)/calcutest.o
 
USERLIBS = ../../../lan/v4.0a/lib/lan.lib ../../../dms/v4.0a/lib/dblib.lib ../../../dps/v4.0a/lib/ipclib.lib $(SYSLIBS) 
DEPLIBS =    
LDLIBS = $(USERLIBS)

# Link or archive
$(TARGETDIR)/$(TARGET): $(TARGETDIR) $(OBJS) $(DEPLIBS)
	$(LINK) -o $@ $(OBJS) $(LDLIBS)

# Compile source files into .o's
$(TARGETDIR)/calcufn.o: calcufn.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ calcufn.c
$(TARGETDIR)/calcutest.o: calcutest.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ calcutest.c


###### clean target: deletes generated files ######
clean:
	$(RM) \
	$(TARGETDIR)/$(TARGET) \
	$(OBJS) 

# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE: .make.state
