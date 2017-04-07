## -*- Makefile -*-
##
## User: ems
## Time: 2006-7-3 17:25:36
## Makefile created by Sun Studio.
##
## This file is generated automatically.
##


#### Compiler and tool definitions shared by all build targets #####
include ../../../global.make

CFLAGS = $(BASICOPTS) -c

# Define the target directories.
TARGETDIR=/home/ems/system/dpp/v4.0a/lib


all: $(TARGETDIR)/dpplib.lib

## Target: dpplib.lib
CPPFLAGS = \
	-I../inc
OBJS =  \
	$(TARGETDIR)/CommAnaAlmPro.o \
	$(TARGETDIR)/CommWriteShare.o \
	$(TARGETDIR)/DataAllSend.o \
	$(TARGETDIR)/DataChangeSend.o \
	$(TARGETDIR)/dpplib.o \
	$(TARGETDIR)/ProcessAnalog.o \
	$(TARGETDIR)/ProcessAout.o \
	$(TARGETDIR)/ProcessDii.o \
	$(TARGETDIR)/ProcessDin.o \
	$(TARGETDIR)/ProcessDout.o \
	$(TARGETDIR)/ProcessInd.o \
	$(TARGETDIR)/ProcessMessage.o \
	$(TARGETDIR)/ProcessObj.o \
	$(TARGETDIR)/ProcessParam.o \
	$(TARGETDIR)/ProcessPulse.o \
	$(TARGETDIR)/ProcessSendata.o \
	$(TARGETDIR)/ReadDb.o \
	$(TARGETDIR)/ReadDefineFile.o \
	$(TARGETDIR)/ReadNewData.o


# Link or archive
$(TARGETDIR)/dpplib.lib: $(TARGETDIR) $(OBJS)
	$(ARCHIVE) $@ $(OBJS)


# Compile source files into .o files
$(TARGETDIR)/CommAnaAlmPro.o: $(TARGETDIR) CommAnaAlmPro.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ CommAnaAlmPro.c

$(TARGETDIR)/CommWriteShare.o: $(TARGETDIR) CommWriteShare.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ CommWriteShare.c

$(TARGETDIR)/DataAllSend.o: $(TARGETDIR) DataAllSend.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ DataAllSend.c

$(TARGETDIR)/DataChangeSend.o: $(TARGETDIR) DataChangeSend.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ DataChangeSend.c

$(TARGETDIR)/dpplib.o: $(TARGETDIR) dpplib.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ dpplib.c

$(TARGETDIR)/ProcessAnalog.o: $(TARGETDIR) ProcessAnalog.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ ProcessAnalog.c

$(TARGETDIR)/ProcessAout.o: $(TARGETDIR) ProcessAout.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ ProcessAout.c

$(TARGETDIR)/ProcessDii.o: $(TARGETDIR) ProcessDii.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ ProcessDii.c

$(TARGETDIR)/ProcessDin.o: $(TARGETDIR) ProcessDin.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ ProcessDin.c

$(TARGETDIR)/ProcessDout.o: $(TARGETDIR) ProcessDout.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ ProcessDout.c

$(TARGETDIR)/ProcessInd.o: $(TARGETDIR) ProcessInd.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ ProcessInd.c

$(TARGETDIR)/ProcessMessage.o: $(TARGETDIR) ProcessMessage.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ ProcessMessage.c
	
$(TARGETDIR)/ProcessObj.o: $(TARGETDIR) ProcessObj.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ ProcessObj.c

$(TARGETDIR)/ProcessParam.o: $(TARGETDIR) ProcessParam.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ ProcessParam.c

$(TARGETDIR)/ProcessPulse.o: $(TARGETDIR) ProcessPulse.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ ProcessPulse.c

$(TARGETDIR)/ProcessSendata.o: $(TARGETDIR) ProcessSendata.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ ProcessSendata.c

$(TARGETDIR)/ReadDb.o: $(TARGETDIR) ReadDb.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ ReadDb.c

$(TARGETDIR)/ReadDefineFile.o: $(TARGETDIR) ReadDefineFile.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ ReadDefineFile.c

$(TARGETDIR)/ReadNewData.o: $(TARGETDIR) ReadNewData.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ ReadNewData.c



#### Clean target deletes all generated files ####
clean:
	rm -f \
		$(TARGETDIR)/dpplib.lib \
		$(OBJS)


# Create the target directory (if needed)
$(TARGETDIR):
	mkdir -p $(TARGETDIR)


# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state

