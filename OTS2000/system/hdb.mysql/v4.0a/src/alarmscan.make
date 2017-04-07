## -*- Makefile -*-
##
## User: ems
## Time: 2006-7-5 15:07:30
## Makefile created by Sun Studio.
##
## This file is generated automatically.
##


#### Compiler and tool definitions shared by all build targets #####
CC = cc
BASICOPTS = -g
CFLAGS = $(BASICOPTS) -xCC


# Define the target directories.
TARGETDIR_alarmscan.dxe=.


all: alarmscan.dxe

## Target: alarmscan.dxe
CPPFLAGS_alarmscan.dxe = \
	-I$(ORACLE_HOME)/precomp/public \
	-I$(ORACLE_HOME)/rdbms/demo \
	-I../inc
OBJS_alarmscan.dxe =  \
	$(TARGETDIR_alarmscan.dxe)/alarmscan.o
SYSLIBS_alarmscan.dxe =   -L$(ORACLE_HOME)/lib/ -L$(ORACLE_HOME)/rdbms/lib/ -lclntsh -lm -lsocket -lnsl -lthread -lpthread -lintl 
USERLIBS_alarmscan.dxe = ../../../lan/v4.0a/lib/lan.lib ../../../dps/v4.0a/lib/ipclib.lib ../../../dms/v4.0a/lib/dblib.lib  ../lib/hdb.lib 
DEPLIBS_alarmscan.dxe = ../../../dps/v4.0a/lib/ipclib.lib  ../../../dms/v4.0a/lib/dblib.lib  
LDLIBS_alarmscan.dxe = $(USERLIBS_alarmscan.dxe)


# Link or archive
$(TARGETDIR_alarmscan.dxe)/alarmscan.dxe: $(TARGETDIR_alarmscan.dxe) $(OBJS_alarmscan.dxe) $(DEPLIBS_alarmscan.dxe)
	$(LINK.c) $(CFLAGS_alarmscan.dxe) $(CPPFLAGS_alarmscan.dxe) -o $@ $(OBJS_alarmscan.dxe) $(LDLIBS_alarmscan.dxe)   $(SYSLIBS_alarmscan.dxe)


# Compile source files into .o files
$(TARGETDIR_alarmscan.dxe)/alarmscan.o: $(TARGETDIR_alarmscan.dxe) alarmscan.c
	$(COMPILE.c) $(CFLAGS_alarmscan.dxe) $(CPPFLAGS_alarmscan.dxe) -o $@ alarmscan.c



#### Clean target deletes all generated files ####
clean:
	rm -f \
		$(TARGETDIR_alarmscan.dxe)/alarmscan.dxe \
		$(TARGETDIR_alarmscan.dxe)/alarmscan.o \
# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.Sun-sparc-Solaris

