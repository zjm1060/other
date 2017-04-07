## -*- Makefile -*-
##
## User: ems
## Time: 2006-6-29 16:39:10
## Makefile created by Sun Studio.
##
## This file is generated automatically.
##


#### Compiler and tool definitions shared by all build targets #####
include ../../../global.make

CFLAGS = $(BASICOPTS) -c

# Define the target directories.
TARGETDIR=.
TARGET=readshm.$(SUFFIX)

all: $(TARGETDIR)/$(TARGET)

OBJS =  \
	$(TARGETDIR)/read_shm.o

USERLIBS = ../../../dms/v4.0a/lib/dblib.lib ../lib/ipclib.lib $(SYSLIBS) 
DEPLIBS = 
LDLIBS = $(USERLIBS)


# Link or archive
$(TARGETDIR)/$(TARGET): $(TARGETDIR) $(OBJS) $(DEPLIBS)
	$(LINK) -o $@ $(OBJS) $(LDLIBS)


# Compile source files into .o files
$(TARGETDIR)/read_shm.o: $(TARGETDIR) read_shm.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ read_shm.c



#### Clean target deletes all generated files ####
clean:
	rm -f \
		$(TARGETDIR)/$(TARGET) \
		$(OBJS)
		
# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state

