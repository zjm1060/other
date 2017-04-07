## -*- Makefile -*-
##
## Project: looptest.prd
## User: ems
## Time: 06/04/25 17:34:00
## Makefile created by Sun WorkShop.
##
## This file is generated automatically -- DO NOT EDIT.
##

include ../../../global.make

CFLAGS = $(BASICOPTS) -c

# Define the target directories.
TARGETDIR=.
TARGET=looptest.$(SUFFIX)

all: $(TARGETDIR)/$(TARGET)

OBJS = \
	$(TARGETDIR)/looptest.o

USERLIBS = ../../../lan/v4.0a/lib/lan.lib ../../../dms/v4.0a/lib/dblib.lib ../../../dps/v4.0a/lib/ipclib.lib $(SYSLIBS) 
DEPLIBS =    
LDLIBS = $(USERLIBS)

# Link or archive
$(TARGETDIR)/$(TARGET): $(TARGETDIR) $(OBJS) $(DEPLIBS)
	$(LINK) -o $@ $(OBJS) $(LDLIBS)


# Compile source files into .o's
$(TARGETDIR)/looptest.o: looptest.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ looptest.c


###### clean target: deletes generated files ######
clean:
	$(RM) \
	$(TARGETDIR)/$(TARGET) \
	$(OBJS)

# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE: .make.state
