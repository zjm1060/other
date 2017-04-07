## -*- Makefile -*-
##
## User: ems
## Time: 2006-6-29 10:41:04
## Makefile created by Sun Studio.
##
## This file is generated automatically.
##


#### Compiler and tool definitions shared by all build targets #####
include ../../../global.make

CFLAGS = $(BASICOPTS) -c


# Define the target directories.
TARGETDIR=/home/ems/system/dms/v4.0a/lib


all: $(TARGETDIR)/dblib.lib

## Target: dblib.lib
OBJS =  \
	$(TARGETDIR)/dms_util.o \
	$(TARGETDIR)/FileMapService.o \
	$(TARGETDIR)/interlockfn.o \
	$(TARGETDIR)/LoadEmsDefine.o \
	$(TARGETDIR)/MountDB.o \
	$(TARGETDIR)/ReadDatabase.o \
	$(TARGETDIR)/ReadPoint.o \
	$(TARGETDIR)/relateptfn.o \
	$(TARGETDIR)/WritePoint.o \
	$(TARGETDIR)/stringTextFunc.o \
	$(TARGETDIR)/mac.o \
	$(TARGETDIR)/syslog.o


# Link or archive
$(TARGETDIR)/dblib.lib: $(TARGETDIR) $(OBJS)
	$(ARCHIVE) $@ $(OBJS)


# Compile source files into .o files
$(TARGETDIR)/dms_util.o: $(TARGETDIR) dms_util.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ dms_util.c

$(TARGETDIR)/FileMapService.o: $(TARGETDIR) FileMapService.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ FileMapService.c

$(TARGETDIR)/interlockfn.o: $(TARGETDIR) interlockfn.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ interlockfn.c

$(TARGETDIR)/LoadEmsDefine.o: $(TARGETDIR) LoadEmsDefine.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ LoadEmsDefine.c

$(TARGETDIR)/MountDB.o: $(TARGETDIR) MountDB.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ MountDB.c

$(TARGETDIR)/ReadDatabase.o: $(TARGETDIR) ReadDatabase.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ ReadDatabase.c

$(TARGETDIR)/ReadPoint.o: $(TARGETDIR) ReadPoint.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ ReadPoint.c

$(TARGETDIR)/relateptfn.o: $(TARGETDIR) relateptfn.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ relateptfn.c

$(TARGETDIR)/WritePoint.o: $(TARGETDIR) WritePoint.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ WritePoint.c

$(TARGETDIR)/stringTextFunc.o: $(TARGETDIR) stringTextFunc.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ stringTextFunc.c

$(TARGETDIR)/mac.o: $(TARGETDIR) mac.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ mac.c

$(TARGETDIR)/syslog.o: $(TARGETDIR) syslog.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ syslog.c



#### Clean target deletes all generated files ####
clean:
	rm -f \
		$(TARGETDIR)/dblib.lib \
		$(OBJS)


# Create the target directory (if needed)
$(TARGETDIR):
	mkdir -p $(TARGETDIR)


# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state

