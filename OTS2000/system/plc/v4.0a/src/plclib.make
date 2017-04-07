## -*- Makefile -*-
##
## User: ems
## Time: 2006-9-14 15:14:10
## Makefile created by Sun Studio.
##
## This file is generated automatically.
##

#### Compiler and tool definitions shared by all build targets #####
include ../../../global.make

CFLAGS = $(BASICOPTS) -c


# Define the target directories.
TARGETDIR=/home/ems/system/plc/v4.0a/lib


all: $(TARGETDIR)/plc.lib

CPPFLAGS = \
	-I../inc
	
OBJS =  \
	$(TARGETDIR)/comlib.o


# Link or archive
$(TARGETDIR)/plc.lib: $(TARGETDIR) $(OBJS)
	$(ARCHIVE) $@ $(OBJS)


# Compile source files into .o files
$(TARGETDIR)/comlib.o: $(TARGETDIR) comlib.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ comlib.c

#### Clean target deletes all generated files ####
clean:
	rm -f \
	$(TARGETDIR)/plc.lib \
	$(OBJS)
	

# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.Sun-sparc-Solaris

