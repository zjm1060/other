## -*- Makefile -*-
##
## User: ems
## Time: 2006-10-14 15:14:49
## Makefile created by Sun Studio.
##
## This file is generated automatically.
##


#### Compiler and tool definitions shared by all build targets #####
include ../../../global.make

CFLAGS = $(BASICOPTS) -c

# Define the target directories.
TARGETDIR=.
TARGET=plcmon.$(SUFFIX)

all: $(TARGETDIR)/$(TARGET)

CPPFLAGS = \
	-I../inc
	
OBJS =  \
	$(TARGETDIR)/plcmon.o \
	$(TARGETDIR)/plcping.o 
	
USERLIBS = ../lib/plc.lib ../../../lan/v4.0a/lib/lan.lib ../../../dps/v4.0a/lib/ipclib.lib ../../../dms/v4.0a/lib/dblib.lib $(SYSLIBS) 
DEPLIBS =      
LDLIBS = $(USERLIBS)


# Link or archive
$(TARGETDIR)/$(TARGET): $(TARGETDIR) $(OBJS) $(DEPLIBS)
	$(LINK) -o $@ $(OBJS) $(LDLIBS)


# Compile source files into .o files
$(TARGETDIR)/plcmon.o: $(TARGETDIR) plcmon.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ plcmon.c

$(TARGETDIR)/plcping.o: $(TARGETDIR) plcping.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ plcping.c


#### Clean target deletes all generated files ####
clean:
	rm -f \
	$(TARGETDIR)/$(TARGET) \
	$(OBJS)
	
# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state

