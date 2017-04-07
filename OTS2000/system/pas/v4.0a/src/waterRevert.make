## -*- Makefile -*-
##
## User: ems
## Time: 2006-11-10 13:18:13
## Makefile created by Sun Studio.
##
## This file is generated automatically.
##


#### Compiler and tool definitions shared by all build targets #####
CC = cc
BASICOPTS = -g
CFLAGS = $(BASICOPTS)


# Define the target directories.
TARGETDIR_waterRevert.dxe=.


all: waterRevert.dxe

## Target: waterRevert.dxe
OBJS_waterRevert.dxe =  \
	$(TARGETDIR_waterRevert.dxe)/waterRevert.o
SYSLIBS_waterRevert.dxe = -lnsl -lpthread
USERLIBS_waterRevert.dxe = ../../../lan/v4.0a/lib/lan.lib ../../../dps/v4.0a/lib/ipclib.lib ../../../dps/v4.0a/lib/dpslib.lib ../../../dms/v4.0a/lib/dblib.lib $(SYSLIBS_waterRevert.dxe) 
DEPLIBS_waterRevert.dxe =     
LDLIBS_waterRevert.dxe = $(USERLIBS_waterRevert.dxe)


# Link or archive
$(TARGETDIR_waterRevert.dxe)/waterRevert.dxe: $(TARGETDIR_waterRevert.dxe) $(OBJS_waterRevert.dxe) $(DEPLIBS_waterRevert.dxe)
	$(LINK.c) $(CFLAGS_waterRevert.dxe) $(CPPFLAGS_waterRevert.dxe) -o $@ $(OBJS_waterRevert.dxe) $(LDLIBS_waterRevert.dxe)


# Compile source files into .o files
$(TARGETDIR_waterRevert.dxe)/waterRevert.o: $(TARGETDIR_waterRevert.dxe) waterRevert.c
	$(COMPILE.c) $(CFLAGS_waterRevert.dxe) $(CPPFLAGS_waterRevert.dxe) -o $@ waterRevert.c



#### Clean target deletes all generated files ####
clean:
	rm -f \
		$(TARGETDIR_waterRevert.dxe)/waterRevert.dxe \
		$(TARGETDIR_waterRevert.dxe)/waterRevert.o
# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.Sun-sparc-Solaris

