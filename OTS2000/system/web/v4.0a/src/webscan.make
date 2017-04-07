## -*- Makefile -*-
##
## User: ems
## Time: 2006-6-29 11:26:24
## Makefile created by Sun Studio.
##
## This file is generated automatically.
##


#### Compiler and tool definitions shared by all build targets #####
CC = cc
BASICOPTS = -g
CFLAGS = $(BASICOPTS) -xCC


# Define the target directories.
TARGETDIR_webscan.dxe=.


all: $(TARGETDIR_webscan.dxe)/webscan.dxe

## Target: webscan.dxe
OBJS_webscan.dxe =  \
	$(TARGETDIR_webscan.dxe)/webscan.o
SYSLIBS_webscan.dxe = -lsocket -lnsl
USERLIBS_webscan.dxe = ../../../dms/v4.0a/lib/dblib.lib ../../../dps/v4.0a/lib/ipclib.lib $(SYSLIBS_webscan.dxe) 
DEPLIBS_webscan.dxe = ../../../dms/v4.0a/lib/dblib.lib    ../../../dps/v4.0a/lib/ipclib.lib
LDLIBS_webscan.dxe = $(USERLIBS_webscan.dxe)


# Link or archive
$(TARGETDIR_webscan.dxe)/webscan.dxe: $(TARGETDIR_webscan.dxe) $(OBJS_webscan.dxe) $(DEPLIBS_webscan.dxe)
	$(LINK.c) $(CFLAGS_webscan.dxe) $(CPPFLAGS_webscan.dxe) -o $@ $(OBJS_webscan.dxe) $(LDLIBS_webscan.dxe)


# Compile source files into .o files
$(TARGETDIR_webscan.dxe)/webscan.o: $(TARGETDIR_webscan.dxe) webscan.c
	$(COMPILE.c) $(CFLAGS_webscan.dxe) $(CPPFLAGS_webscan.dxe) -o $@ webscan.c



#### Clean target deletes all generated files ####
clean:
	rm -f \
		$(TARGETDIR_webscan.dxe)/webscan.dxe \
		$(TARGETDIR_webscan.dxe)/webscan.o
# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.Sun-sparc-Solaris

