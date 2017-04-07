## -*- Makefile -*-
##
## User: ems
## Time: 2006-7-4 9:37:27
## Makefile created by Sun Studio.
##
## This file is generated automatically.
##


#### Compiler and tool definitions shared by all build targets #####
include ../../../global.make

CFLAGS = $(BASICOPTS) -c

# Define the target directories.
TARGETDIR=/home/ems/system/cmd/v4.0a/lib


all: $(TARGETDIR)/cmdlib.lib

## Target: cmdlib.lib
CPPFLAGS = \
	-I../inc
OBJS =  \
	$(TARGETDIR)/CmdProcess.o


# Link or archive
$(TARGETDIR)/cmdlib.lib: $(TARGETDIR) $(OBJS)
	$(ARCHIVE) $@ $(OBJS)


# Compile source files into .o files
$(TARGETDIR)/CmdProcess.o: $(TARGETDIR) CmdProcess.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ CmdProcess.c



#### Clean target deletes all generated files ####
clean:
	rm -f \
		$(TARGETDIR)/cmdlib.lib \
		$(OBJS)

# Create the target directory (if needed)
$(TARGETDIR):
	mkdir -p $(TARGETDIR)


# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state

