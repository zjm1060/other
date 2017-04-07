## -*- Makefile -*-
##
## User: ems
## Time: 2006-6-29 11:29:10
## Makefile created by Sun Studio.
##
## This file is generated automatically.
##


#### Compiler and tool definitions shared by all build targets #####
include ../../../global.make

CFLAGS = $(BASICOPTS) -c

# Define the target directories.
TARGETDIR=/home/ems/system/dps/v4.0a/lib


all: $(TARGETDIR)/ipclib.lib

## Target: ipclib.lib
OBJS =  \
	$(TARGETDIR)/ipc_lib.o


# Link or archive
$(TARGETDIR)/ipclib.lib: $(TARGETDIR) $(OBJS)
	$(ARCHIVE) $@ $(OBJS)


# Compile source files into .o files
$(TARGETDIR)/ipc_lib.o: $(TARGETDIR) ipc_lib.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ ipc_lib.c



#### Clean target deletes all generated files ####
clean:
	rm -f \
		$(TARGETDIR)/ipclib.lib \
		$(OBJS)

# Create the target directory (if needed)
$(TARGETDIR):
	mkdir -p $(TARGETDIR)


# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state

