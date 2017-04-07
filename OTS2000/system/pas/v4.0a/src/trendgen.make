## -*- Makefile -*-
##
## Project: trendgen.prd
## User: ems
## Time: 06/03/28 11:23:01
## Makefile created by Sun WorkShop.
##
## This file is generated automatically -- DO NOT EDIT.
##

#### Compiler and tool definitions shared by all build targets #####
include ../../../global.make

CFLAGS = $(BASICOPTS) -c

# Define the target directories.
TARGETDIR=.
TARGET=trendgen.$(SUFFIX)

all: $(TARGETDIR)/$(TARGET)

CPPFLAGS += \
	-I/home/openwin/include \
	-I/home/dt/include 
	
OBJS = \
	$(TARGETDIR)/trendgen.o

USERLIBS = ../../../dms/v4.0a/lib/dblib.lib ../../../dps/v4.0a/lib/ipclib.lib $(SYSLIBS) 
DEPLIBS =    
LDLIBS = $(USERLIBS)

# Link or archive
$(TARGETDIR)/$(TARGET): $(TARGETDIR) $(OBJS) $(DEPLIBS)
	$(LINK) -o $@ $(OBJS) $(LDLIBS)

# Compile source files into .o's
$(TARGETDIR)/trendgen.o: trendgen.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ trendgen.c


###### clean target: deletes generated files ######
clean:
	$(RM) \
	$(TARGETDIR)/$(TARGET) \
	$(OBJS)

# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE: .make.state
