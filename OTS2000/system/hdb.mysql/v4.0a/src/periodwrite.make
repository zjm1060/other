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
TARGETDIR_periodwrite.dxe=.


all: periodwrite.dxe

## Target: periodwrite.dxe
CPPFLAGS_periodwrite.dxe = \
	-I$(ORACLE_HOME)/precomp/public \
	-I$(ORACLE_HOME)/rdbms/demo \
	-I../inc
OBJS_periodwrite.dxe =  \
	$(TARGETDIR_periodwrite.dxe)/periodwrite.o
SYSLIBS_periodwrite.dxe =   -L$(ORACLE_HOME)/lib/ -L$(ORACLE_HOME)/rdbms/lib/ -lclntsh -lm -lsocket -lnsl -lthread -lpthread -lintl 
USERLIBS_periodwrite.dxe = ../../../lan/v4.0a/lib/lan.lib ../../../dps/v4.0a/lib/ipclib.lib ../../../dms/v4.0a/lib/dblib.lib  ../lib/hdb.lib 
DEPLIBS_periodwrite.dxe = ../../../dps/v4.0a/lib/ipclib.lib  ../../../dms/v4.0a/lib/dblib.lib  
LDLIBS_periodwrite.dxe = $(USERLIBS_periodwrite.dxe)


# Link or archive
$(TARGETDIR_periodwrite.dxe)/periodwrite.dxe: $(TARGETDIR_periodwrite.dxe) $(OBJS_periodwrite.dxe) $(DEPLIBS_periodwrite.dxe)
	$(LINK.c) $(CFLAGS_periodwrite.dxe) $(CPPFLAGS_periodwrite.dxe) -o $@ $(OBJS_periodwrite.dxe) $(LDLIBS_periodwrite.dxe)   $(SYSLIBS_periodwrite.dxe)


# Compile source files into .o files
$(TARGETDIR_periodwrite.dxe)/periodwrite.o: $(TARGETDIR_periodwrite.dxe) periodwrite.c
	$(COMPILE.c) $(CFLAGS_periodwrite.dxe) $(CPPFLAGS_periodwrite.dxe) -o $@ periodwrite.c



#### Clean target deletes all generated files ####
clean:
	rm -f \
		$(TARGETDIR_periodwrite.dxe)/periodwrite.dxe \
		$(TARGETDIR_periodwrite.dxe)/periodwrite.o \
# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.Sun-sparc-Solaris

