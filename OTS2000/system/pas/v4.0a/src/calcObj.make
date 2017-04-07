## -*- Makefile -*-
##
## User: ems
## Time: 2006-6-29 14:03:22
## Makefile created by Sun Studio.
##
## This file is generated automatically.
##


#### Compiler and tool definitions shared by all build targets #####
CC = CC
BASICOPTS = -g   
CFLAGS = $(BASICOPTS)


# Define the target directories.
TARGETDIR_calcObj.dxe=.


all: calcObj.dxe
 
## Target: calcObj.dxe
OBJS_calcObj.dxe =  \
	$(TARGETDIR_calcObj.dxe)/calcObj.o
SYSLIBS_calcObj.dxe = -lnsl -lsocket -lxml2
USERLIBS_calcObj.dxe = ../../../dms/v4.0a/lib/dblib.lib ../../../dps/v4.0a/lib/ipclib.lib  ../../../dps/v4.0a/lib/dpslib.lib ../../../lan/v4.0a/lib/lan.lib $(SYSLIBS_calcObj.dxe) 
DEPLIBS_calcObj.dxe = ../../../dms/v4.0a/lib/dblib.lib  ../../../dps/v4.0a/lib/ipclib.lib ../../../dps/v4.0a/lib/dpslib.lib ../../../lan/v4.0a/lib/lan.lib   
LDLIBS_calcObj.dxe = $(USERLIBS_calcObj.dxe)


# Link or archive
$(TARGETDIR_calcObj.dxe)/calcObj.dxe: $(TARGETDIR_calcObj.dxe) $(OBJS_calcObj.dxe) $(DEPLIBS_calcObj.dxe)
	$(LINK.c) $(CFLAGS_calcObj.dxe) $(CPPFLAGS_calcObj.dxe) -o $@ $(OBJS_calcObj.dxe) $(LDLIBS_calcObj.dxe)


# Compile source files into .o files
$(TARGETDIR_calcObj.dxe)/calcObj.o: $(TARGETDIR_calcObj.dxe) calcObj.cpp  
	$(COMPILE.c) -I/usr/include/libxml2 $(CFLAGS_calcObj.dxe) $(CPPFLAGS_calcObj.dxe) -o $@ calcObj.cpp



#### Clean target deletes all generated files ####
clean:
	rm -f \
		$(TARGETDIR_calcObj.dxe)/calcObj.dxe \
		$(TARGETDIR_calcObj.dxe)/calcObj.o
# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.Sun-sparc-Solaris

