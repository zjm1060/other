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
TARGETDIR_agc.dxe=.


all: agc.dxe

## Target: agc.dxe   ../../../oix/v4.0a/lib/util.lib
CPPFLAGS_agc.dxe = \
	-I../inc
OBJS_agc.dxe =  \
	$(TARGETDIR_agc.dxe)/agc.o
#	$(TARGETDIR_agc.dxe)/Schedule.o
SYSLIBS_agc.dxe = -L/usr/openwin/lib -L/usr/dt/lib -R/usr/openwin/lib -R/usr/dt/lib -lXm -lXt -lXext -lX11 -lm -lsocket -lnsl 
USERLIBS_agc.dxe =  ../../../lan/v4.0a/lib/lan.lib ../../../dps/v4.0a/lib/ipclib.lib ../../../dps/v4.0a/lib/dpslib.lib ../../../dms/v4.0a/lib/dblib.lib $(SYSLIBS_agc.dxe) 
DEPLIBS_agc.dxe =      
LDLIBS_agc.dxe = $(USERLIBS_agc.dxe)


# Link or archive
$(TARGETDIR_agc.dxe)/agc.dxe: $(TARGETDIR_agc.dxe) $(OBJS_agc.dxe) $(DEPLIBS_agc.dxe)
	$(LINK.c) $(CFLAGS_agc.dxe) $(CPPFLAGS_agc.dxe) -o $@ $(OBJS_agc.dxe) $(LDLIBS_agc.dxe)


# Compile source files into .o files
$(TARGETDIR_agc.dxe)/agc.o: $(TARGETDIR_agc.dxe) agc.c
	$(COMPILE.c) $(CFLAGS_agc.dxe) $(CPPFLAGS_agc.dxe) -o $@ agc.c

$(TARGETDIR_agc.dxe)/Schedule.o: $(TARGETDIR_agc.dxe) Schedule.c
	$(COMPILE.c) $(CFLAGS_agc.dxe) $(CPPFLAGS_agc.dxe) -o $@ Schedule.c



#### Clean target deletes all generated files ####
clean:
	rm -f \
		$(TARGETDIR_agc.dxe)/agc.dxe \
		$(TARGETDIR_agc.dxe)/agc.o \
		$(TARGETDIR_agc.dxe)/Schedule.o
# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.Sun-sparc-Solaris

