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
TARGETDIR_curvescan.dxe=.


all: curvescan.dxe

## Target: curvescan.dxe
CPPFLAGS_curvescan.dxe = \
	-I$(ORACLE_HOME)/precomp/public \
	-I$(ORACLE_HOME)/rdbms/demo \
	-I../inc
OBJS_curvescan.dxe =  \
	$(TARGETDIR_curvescan.dxe)/curvescan.o
SYSLIBS_curvescan.dxe =   -L$(ORACLE_HOME)/lib/ -L$(ORACLE_HOME)/rdbms/lib/ -lclntsh -lm -lsocket -lnsl -lthread -lpthread -lintl 
USERLIBS_curvescan.dxe = ../../../lan/v4.0a/lib/lan.lib ../../../dps/v4.0a/lib/ipclib.lib ../../../dms/v4.0a/lib/dblib.lib  ../lib/hdb.lib 
DEPLIBS_curvescan.dxe = ../../../dps/v4.0a/lib/ipclib.lib  ../../../dms/v4.0a/lib/dblib.lib  
LDLIBS_curvescan.dxe = $(USERLIBS_curvescan.dxe)


# Link or archive
$(TARGETDIR_curvescan.dxe)/curvescan.dxe: $(TARGETDIR_curvescan.dxe) $(OBJS_curvescan.dxe) $(DEPLIBS_curvescan.dxe)
	$(LINK.c) $(CFLAGS_curvescan.dxe) $(CPPFLAGS_curvescan.dxe) -o $@ $(OBJS_curvescan.dxe) $(LDLIBS_curvescan.dxe)   $(SYSLIBS_curvescan.dxe)


# Compile source files into .o files
$(TARGETDIR_curvescan.dxe)/curvescan.o: $(TARGETDIR_curvescan.dxe) curvescan.c
	$(COMPILE.c) $(CFLAGS_curvescan.dxe) $(CPPFLAGS_curvescan.dxe) -o $@ curvescan.c



#### Clean target deletes all generated files ####
clean:
	rm -f \
		$(TARGETDIR_curvescan.dxe)/curvescan.dxe \
		$(TARGETDIR_curvescan.dxe)/curvescan.o \
# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.Sun-sparc-Solaris

