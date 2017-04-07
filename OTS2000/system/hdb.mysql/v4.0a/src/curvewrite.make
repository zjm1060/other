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
TARGETDIR_curvewrite.dxe=.


all: curvewrite.dxe

## Target: curvewrite.dxe
CPPFLAGS_curvewrite.dxe = \
	-I$(ORACLE_HOME)/precomp/public \
	-I$(ORACLE_HOME)/rdbms/demo \
	-I../inc
OBJS_curvewrite.dxe =  \
	$(TARGETDIR_curvewrite.dxe)/curvewrite.o
SYSLIBS_curvewrite.dxe =   -L$(ORACLE_HOME)/lib/ -L$(ORACLE_HOME)/rdbms/lib/ -lclntsh -lm -lsocket -lnsl -lthread -lpthread -lintl 
USERLIBS_curvewrite.dxe = ../../../lan/v4.0a/lib/lan.lib ../../../dps/v4.0a/lib/ipclib.lib ../../../dms/v4.0a/lib/dblib.lib  ../lib/hdb.lib 
DEPLIBS_curvewrite.dxe = ../../../dps/v4.0a/lib/ipclib.lib  ../../../dms/v4.0a/lib/dblib.lib  
LDLIBS_curvewrite.dxe = $(USERLIBS_curvewrite.dxe)


# Link or archive
$(TARGETDIR_curvewrite.dxe)/curvewrite.dxe: $(TARGETDIR_curvewrite.dxe) $(OBJS_curvewrite.dxe) $(DEPLIBS_curvewrite.dxe)
	$(LINK.c) $(CFLAGS_curvewrite.dxe) $(CPPFLAGS_curvewrite.dxe) -o $@ $(OBJS_curvewrite.dxe) $(LDLIBS_curvewrite.dxe)   $(SYSLIBS_curvewrite.dxe)


# Compile source files into .o files
$(TARGETDIR_curvewrite.dxe)/curvewrite.o: $(TARGETDIR_curvewrite.dxe) curvewrite.c
	$(COMPILE.c) $(CFLAGS_curvewrite.dxe) $(CPPFLAGS_curvewrite.dxe) -o $@ curvewrite.c



#### Clean target deletes all generated files ####
clean:
	rm -f \
		$(TARGETDIR_curvewrite.dxe)/curvewrite.dxe \
		$(TARGETDIR_curvewrite.dxe)/curvewrite.o \
# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.Sun-sparc-Solaris

