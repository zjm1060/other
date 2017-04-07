## -*- Makefile -*-
##
## User: ems
## Time: 06/04/25 17:30:59
## Makefile created by Sun WorkShop.
##
## This file is generated automatically -- DO NOT EDIT.
##



project: /home/ems/system/plc/v4.0a/src/mytest.dxe 

##### Compilers and tools definitions shared by all build objects #####
CC=cc
CFLAGS=-g


###### Target: mytest.dxe ######
TARGETDIR_TEST.DXE=/home/ems/system/plc/v4.0a/src
OBJS_TEST.DXE = \
	$(TARGETDIR_TEST.DXE)/mytest.o \
	/home/ems/system/dms/v4.0a/lib/dblib.lib \
	/home/ems/system/dps/v4.0a/lib/ipclib.lib \
	/home/ems/system/lan/v4.0a/lib/lan.lib 


# Link or archive
/home/ems/system/plc/v4.0a/src/mytest.dxe: $(OBJS_TEST.DXE) 
	$(LINK.c)  $(CFLAGS_TEST.DXE) $(CPPFLAGS_TEST.DXE) -o /home/ems/system/plc/v4.0a/src/mytest.dxe $(OBJS_TEST.DXE) \
	/home/ems/system/dms/v4.0a/lib/dblib.lib \
	/home/ems/system/dps/v4.0a/lib/ipclib.lib -lnsl -lpthread
## .a's were added twice to link line to make sure they are found in a link.


# Compile source files into .o's
$(TARGETDIR_TEST.DXE)/mytest.o: mytest.c
	$(COMPILE.c) $(CFLAGS_TEST.DXE) $(CPPFLAGS_TEST.DXE) -o $(TARGETDIR_TEST.DXE)/mytest.o mytest.c


###### clean target: deletes generated files ######
clean:
	$(RM) \
	/home/ems/system/plc/v4.0a/src/mytest.dxe \
	$(TARGETDIR_TEST.DXE)/mytest.o 


