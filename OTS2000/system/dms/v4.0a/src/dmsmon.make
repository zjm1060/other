## -*- Makefile -*-
##
## User: ems
## Time: 2006-6-29 11:09:50
## Makefile created by Sun Studio.
##
## This file is generated automatically.
##


#### Compiler and tool definitions shared by all build targets #####
include ../../../global.make

CFLAGS = $(BASICOPTS) -c

# Define the target directories.
TARGETDIR=.
TARGET=dmsmon.$(SUFFIX)

all: $(TARGETDIR)/$(TARGET)

OBJS =  \
	$(TARGETDIR)/DefineDatabase.o \
	$(TARGETDIR)/DefineMsgDef.o \
	$(TARGETDIR)/GetEntries.o \
	$(TARGETDIR)/InitializeDatabaseFile.o \
	$(TARGETDIR)/PrintErrorInfo.o \
	$(TARGETDIR)/dmsmon.o \
	$(TARGETDIR)/ResetDatabase.o
	
USERLIBS = ../lib/dblib.lib ../../../dps/v4.0a/lib/ipclib.lib $(SYSLIBS) 
DEPLIBS =
LDLIBS = $(USERLIBS)


# Link or archive
$(TARGETDIR)/$(TARGET): $(TARGETDIR) $(OBJS) $(DEPLIBS)
	$(LINK) -o $@ $(OBJS) $(LDLIBS)


# Compile source files into .o files
$(TARGETDIR)/DefineDatabase.o: $(TARGETDIR) DefineDatabase.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ DefineDatabase.c

$(TARGETDIR)/DefineMsgDef.o: $(TARGETDIR) DefineMsgDef.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ DefineMsgDef.c

$(TARGETDIR)/GetEntries.o: $(TARGETDIR) GetEntries.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ GetEntries.c

$(TARGETDIR)/InitializeDatabaseFile.o: $(TARGETDIR) InitializeDatabaseFile.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ InitializeDatabaseFile.c

$(TARGETDIR)/dmsmon.o: $(TARGETDIR) dmsmon.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ dmsmon.c

$(TARGETDIR)/PrintErrorInfo.o: $(TARGETDIR) PrintErrorInfo.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ PrintErrorInfo.c

$(TARGETDIR)/ResetDatabase.o: $(TARGETDIR) ResetDatabase.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ ResetDatabase.c



#### Clean target deletes all generated files ####
clean:
	rm -f \
		$(TARGETDIR)/$(TARGET) \
		$(OBJS)


# Create the target directory (if needed)
$(TARGETDIR):
	mkdir -p $(TARGETDIR)


# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state

