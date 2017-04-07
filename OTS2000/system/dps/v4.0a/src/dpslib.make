## -*- Makefile -*-
##
## User: ems
## Time: 2006-10-16 16:34:52
## Makefile created by the Native Languages Module.
##
## This file is generated automatically -- Changes will be lost if regenerated
##


#### Compiler and tool definitions shared by all build targets #####
include ../../../global.make

CFLAGS = $(BASICOPTS) -c

# Define the target directories.
TARGETDIR=../lib

all: $(TARGETDIR)/dpslib.lib

## Target: dpslib.lib
OBJS =  \
	$(TARGETDIR)/dps_lib.o \
	$(TARGETDIR)/load_passwd_p.o
	
# Link or archive
$(TARGETDIR)/dpslib.lib: $(TARGETDIR) $(OBJS)
	$(ARCHIVE) $@ $(OBJS)


# Compile source files into .o files
$(TARGETDIR)/dps_lib.o: $(TARGETDIR) dps_lib.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ dps_lib.c

$(TARGETDIR)/load_passwd_p.o: $(TARGETDIR) load_passwd_p.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ load_passwd_p.c



#### Clean target deletes all generated files ####
clean:
	rm -f \
		$(TARGETDIR)/dpslib.lib \
		$(OBJS)


# Create the target directory (if needed)
$(TARGETDIR):
	mkdir -p $(TARGETDIR)


# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state

