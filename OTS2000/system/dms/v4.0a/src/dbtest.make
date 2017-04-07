## -*- Makefile -*-
##
## User: ems
## Time: 2006-9-20 16:17:33
## Makefile created by Sun Studio.
##
## This file is generated automatically.
##


#### Compiler and tool definitions shared by all build targets #####
CC = cc
BASICOPTS = -g
CFLAGS = $(BASICOPTS)


# Define the target directories.
TARGETDIR_dbtest.dxe=.


all: dbtest.dxe

## Target: dbtest.dxe
OBJS_dbtest.dxe =  \
	$(TARGETDIR_dbtest.dxe)/dbtest.o
USERLIBS_dbtest.dxe = ../lib/dblib.lib $(SYSLIBS_dbtest.dxe) -lnsl  -lsocket
DEPLIBS_dbtest.dxe =  ../lib/dblib.lib
LDLIBS_dbtest.dxe = $(USERLIBS_dbtest.dxe)


# Link or archive
$(TARGETDIR_dbtest.dxe)/dbtest.dxe: $(TARGETDIR_dbtest.dxe) $(OBJS_dbtest.dxe) $(DEPLIBS_dbtest.dxe)
	$(LINK.c) $(CFLAGS_dbtest.dxe) $(CPPFLAGS_dbtest.dxe) -o $@ $(OBJS_dbtest.dxe) $(LDLIBS_dbtest.dxe)


# Compile source files into .o files
$(TARGETDIR_dbtest.dxe)/dbtest.o: $(TARGETDIR_dbtest.dxe) db_test.c
	$(COMPILE.c) $(CFLAGS_dbtest.dxe) $(CPPFLAGS_dbtest.dxe) -o $@ db_test.c



#### Clean target deletes all generated files ####
clean:
	rm -f \
		$(TARGETDIR_dbtest.dxe)/dbtest.dxe \
		$(TARGETDIR_dbtest.dxe)/dbtest.o
# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.Sun-sparc-Solaris

