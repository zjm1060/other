## -*- Makefile -*-
##
## User: ems
## Time: 2006-7-3 17:33:01
## Makefile created by Sun Studio.
##
## This file is generated automatically.
##


#### Compiler and tool definitions shared by all build targets #####
include ../../../global.make

CFLAGS = $(BASICOPTS) -c

# Define the target directories.
TARGETDIR=.
TARGET=dpp.$(SUFFIX)

all: $(TARGETDIR)/$(TARGET)

CPPFLAGS = \
	-I../inc
OBJS =  \
	$(TARGETDIR)/Dpp.o \
	$(TARGETDIR)/ProcessCommData.o \
	$(TARGETDIR)/ProcessPlcData.o \
	$(TARGETDIR)/Scada.o

USERLIBS = ../../../lan/v4.0a/lib/lan.lib ../../../dms/v4.0a/lib/dblib.lib ../../../dps/v4.0a/lib/ipclib.lib ../lib/dpplib.lib $(SYSLIBS) 
DEPLIBS =  
LDLIBS = $(USERLIBS)


# Link or archive
$(TARGETDIR)/$(TARGET): $(TARGETDIR) $(OBJS) $(DEPLIBS)
	$(LINK) -o $@ $(OBJS) $(LDLIBS)


# Compile source files into .o files
$(TARGETDIR)/Dpp.o: $(TARGETDIR) Dpp.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ Dpp.c

$(TARGETDIR)/ProcessCommData.o: $(TARGETDIR) ProcessCommData.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ ProcessCommData.c

$(TARGETDIR)/ProcessPlcData.o: $(TARGETDIR) ProcessPlcData.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ ProcessPlcData.c

$(TARGETDIR)/Scada.o: $(TARGETDIR) Scada.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ Scada.c



#### Clean target deletes all generated files ####
clean:
	rm -f \
		$(TARGETDIR)/$(TARGET) \
		$(OBJS)
		
# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state

