## -*- Makefile -*-
##
## User: ems
## Time: 2006-10-27 10:38:48
## Makefile created by Sun Studio.
##
## This file is generated automatically.
##

#### Compiler and tool definitions shared by all build targets  gthread-2.0  #####
include ../../../global.make

LINKOPTS = `pkg-config gtk+-2.0 --libs`
CFLAGS = $(BASICOPTS) -c `pkg-config gtk+-2.0 --cflags`

# Define the target directories.
TARGETDIR=.
TARGET=privilege_mgr.$(SUFFIX)

all: $(TARGETDIR)/$(TARGET)


CPPFLAGS= \
	-I../inc
	
OBJS =  \
	$(TARGETDIR)/gtk_usr_mgr_win.o \
	$(TARGETDIR)/gtk_new_user.o
	
#SYSLIBS += -ltiff -lsocket -lnsl -lthread -lpthread -lposix4 -lintl -lm -lcurses 
USERLIBS = ../../../dms/v4.0a/lib/dblib.lib ../../../lan/v4.0a/lib/lan.lib ../../../dps/v4.0a/lib/ipclib.lib ../../../dms/v4.0a/lib/dblib.lib ../../../dps/v4.0a/lib/dpslib.lib $(SYSLIBS) 
DEPLIBS = 
LDLIBS = $(USERLIBS)

$(TARGETDIR)/$(TARGET): $(TARGETDIR) $(OBJS) $(DEPLIBS)
	$(LINK) $(LINKOPTS) $(CPPFLAGS) -o $@ $(OBJS) $(LDLIBS)

# Compile source files into .o files
$(TARGETDIR)/gtk_usr_mgr_win.o: $(TARGETDIR) gtk_usr_mgr_win.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_usr_mgr_win.c
$(TARGETDIR)/gtk_new_user.o: $(TARGETDIR) gtk_new_user.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_new_user.c


#### Clean target deletes all generated files ####
clean:
	rm -f \
	$(TARGETDIR)/$(TARGET) \
	$(OBJS)
	
# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state

