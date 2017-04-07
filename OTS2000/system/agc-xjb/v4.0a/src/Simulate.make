## -*- Makefile -*-
##
## User: ems
## Time: 2006-11-13 15:04:14
## Makefile created by the Native Languages Module.
##
## This file is generated automatically -- Changes will be lost if regenerated
##


#### Compiler and tool definitions shared by all build targets #####
include ../../../global.make

CFLAGS = $(BASICOPTS) -c

# Define the target directories.
TARGETDIR=.
TARGET=Simulate-xjb.$(SUFFIX)

all: $(TARGETDIR)/$(TARGET)

CPPFLAGS = \
	-I../inc
OBJS =  \
	$(TARGETDIR)/Simulate.o

USERLIBS = ../../../dps/v4.0a/lib/dpslib.lib ../../../lan/v4.0a/lib/lan.lib ../../../dms/v4.0a/lib/dblib.lib ../../../dps/v4.0a/lib/ipclib.lib $(SYSLIBS) 
DEPLIBS =      
LDLIBS = $(USERLIBS)


# Link or archive
$(TARGETDIR)/$(TARGET): $(TARGETDIR) $(OBJS) $(DEPLIBS)
	$(LINK) -o $@ $(OBJS) $(LDLIBS)


# Compile source files into .o files
$(TARGETDIR)/Simulate.o: $(TARGETDIR) Simulate.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ Simulate.c



#### Clean target deletes all generated files ####
clean:
	rm -f \
		$(TARGETDIR)/$(TARGET) \
		$(OBJS)
		
# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.Sun-sparc-Solaris

