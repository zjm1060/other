## -*- Makefile -*-
##
## User: ems
## Time: 2007-5-8 21:23:33
## Makefile created by the Native Languages Module.
##
## This file is generated automatically -- Changes will be lost if regenerated
##


#### Compiler and tool definitions shared by all build targets #####
CC = cc
BASICOPTS = -g
CFLAGS = $(BASICOPTS) -xCC


# Define the target directories.
TARGETDIR_avc.dxe=.


all: avc.dxe

## Target: avc.dxe   ../../../oix/v4.0a/lib/util.lib
CPPFLAGS_avc.dxe = \
	-I../inc
OBJS_avc.dxe =  \
	$(TARGETDIR_avc.dxe)/avc.o
#	$(TARGETDIR_avc.dxe)/Schedule.o
SYSLIBS_avc.dxe = -L/usr/openwin/lib -L/usr/dt/lib -R/usr/openwin/lib -R/usr/dt/lib -lXm -lXt -lXext -lX11 -lm -lsocket -lnsl 
USERLIBS_avc.dxe =  ../../../lan/v4.0a/lib/lan.lib ../../../dps/v4.0a/lib/ipclib.lib ../../../dms/v4.0a/lib/dblib.lib $(SYSLIBS_avc.dxe) 
DEPLIBS_avc.dxe =      
LDLIBS_avc.dxe = $(USERLIBS_avc.dxe)


# Link or archive
$(TARGETDIR_avc.dxe)/avc.dxe: $(TARGETDIR_avc.dxe) $(OBJS_avc.dxe) $(DEPLIBS_avc.dxe)
	$(LINK.c) $(CFLAGS_avc.dxe) $(CPPFLAGS_avc.dxe) -o $@ $(OBJS_avc.dxe) $(LDLIBS_avc.dxe)


# Compile source files into .o files
$(TARGETDIR_avc.dxe)/avc.o: $(TARGETDIR_avc.dxe) avc.c
	$(COMPILE.c) $(CFLAGS_avc.dxe) $(CPPFLAGS_avc.dxe) -o $@ avc.c

#$(TARGETDIR_avc.dxe)/Schedule.o: $(TARGETDIR_avc.dxe) Schedule.c
#	$(COMPILE.c) $(CFLAGS_avc.dxe) $(CPPFLAGS_avc.dxe) -o $@ Schedule.c



#### Clean target deletes all generated files ####
clean:
	rm -f \
		$(TARGETDIR_avc.dxe)/avc.dxe \
		$(TARGETDIR_avc.dxe)/avc.o
#		$(TARGETDIR_avc.dxe)/Schedule.o
# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.Sun-sparc-Solaris

