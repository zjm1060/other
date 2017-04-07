## -*- Makefile -*-
##
## User: ems
## Time: 2006-6-29 15:41:24
## Makefile created by Sun Studio.
##
## This file is generated automatically.
##


#### Compiler and tool definitions shared by all build targets #####
include ../../../global.make

LINKOPTS = `pkg-config gtk+-2.0 --libs`
CFLAGS = $(BASICOPTS) -c `pkg-config gtk+-2.0 --cflags`

# Define the target directories.
TARGETDIR=.
TARGET=sysmgr.$(SUFFIX)

all: $(TARGETDIR)/$(TARGET)

OBJS =  \
	$(TARGETDIR)/sysmgr.o

USERLIBS = ../../../dms/v4.0a/lib/dblib.lib ../lib/ipclib.lib ../../../lan/v4.0a/lib/lan.lib ../lib/dpslib.lib ../../../dms/v4.0a/lib/dblib.lib $(SYSLIBS) 
DEPLIBS = 
LDLIBS = $(USERLIBS)


# Link or archive
$(TARGETDIR)/$(TARGET): $(TARGETDIR) $(OBJS) $(DEPLIBS)
	$(LINK) $(LINKOPTS) $(CPPFLAGS) -o $@ $(OBJS) $(LDLIBS)


# Compile source files into .o files
$(TARGETDIR)/sysmgr.o: $(TARGETDIR) sysmgr.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ sysmgr.c



#### Clean target deletes all generated files ####
clean:
	rm -f \
		$(TARGETDIR)/$(TARGET) \
		$(OBJS)
		
# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state
