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
TARGETDIR_hdbtest.dxe=.


all: hdbtest.dxe

## Target: hdbtest.dxe
CPPFLAGS_hdbtest.dxe = \
	-I$(ORACLE_HOME)/precomp/public \
	-I$(ORACLE_HOME)/rdbms/demo \
	-I../inc
OBJS_hdbtest.dxe =  \
	$(TARGETDIR_hdbtest.dxe)/hdbtest.o
SYSLIBS_hdbtest.dxe =   -L$(ORACLE_HOME)/lib/ -L$(ORACLE_HOME)/rdbms/lib/ -lclntsh -lm -lsocket -lnsl -lthread -lpthread -lintl 
USERLIBS_hdbtest.dxe = ../../../lan/v4.0a/lib/lan.lib ../../../dps/v4.0a/lib/ipclib.lib ../../../dms/v4.0a/lib/dblib.lib  ../lib/hdb.lib 
DEPLIBS_hdbtest.dxe = ../../../dps/v4.0a/lib/ipclib.lib  ../../../dms/v4.0a/lib/dblib.lib  
LDLIBS_hdbtest.dxe = $(USERLIBS_hdbtest.dxe)


# Link or archive
$(TARGETDIR_hdbtest.dxe)/hdbtest.dxe: $(TARGETDIR_hdbtest.dxe) $(OBJS_hdbtest.dxe) $(DEPLIBS_hdbtest.dxe)
	$(LINK.c) $(CFLAGS_hdbtest.dxe) $(CPPFLAGS_hdbtest.dxe) -o $@ $(OBJS_hdbtest.dxe) $(LDLIBS_hdbtest.dxe)   $(SYSLIBS_hdbtest.dxe)


# Compile source files into .o files
$(TARGETDIR_hdbtest.dxe)/hdbtest.o: $(TARGETDIR_hdbtest.dxe) hdbtest.c
	$(COMPILE.c) $(CFLAGS_hdbtest.dxe) $(CPPFLAGS_hdbtest.dxe) -o $@ hdbtest.c



#### Clean target deletes all generated files ####
clean:
	rm -f \
		$(TARGETDIR_hdbtest.dxe)/hdbtest.dxe \
		$(TARGETDIR_hdbtest.dxe)/hdbtest.o \
# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.Sun-sparc-Solaris

