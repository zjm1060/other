## -*- Makefile -*-
##
## Project: /home/ems/system/sc1801/v4.0a/src/sc1801host.prd
## User: ems
## Time: 06/04/21 11:50:11
## Makefile created by Sun WorkShop.
##
## This file is generated automatically -- DO NOT EDIT.
##



project: /home/ems/system/sc1801/v4.0a/src/sc1801host.dxe 

##### Compilers and tools definitions shared by all build objects #####
#CC=/opt/SUNWspro/bin/../WS6U1/bin/cc
CC=cc
CFLAGS=-g -mt -xCC 


###### Target: /home/ems/system/iec/v4.0a/src/iec101s_com.dxe ######
TARGETDIR_sc1801host.DXE=/home/ems/system/sc1801/v4.0a/src
CPPFLAGS_sc1801host.DXE += \
	-I/usr/openwin/include \
	-I/usr/dt/include 
OBJS_sc1801host.DXE = \
	$(TARGETDIR_sc1801host.DXE)/Host_init.o \
	$(TARGETDIR_sc1801host.DXE)/Host_app_fun.o \
	$(TARGETDIR_sc1801host.DXE)/sc1801Host.o \
	$(TARGETDIR_sc1801host.DXE)/Host_fun.o \
	/home/ems/system/dms/v4.0a/lib/dblib.lib \
	/home/ems/system/dps/v4.0a/lib/ipclib.lib \
	/home/ems/system/lan/v4.0a/lib/lan.lib 


# Link or archive
/home/ems/system/sc1801/v4.0a/src/sc1801host.dxe: $(OBJS_sc1801host.DXE) 
	$(LINK.c)  $(CFLAGS_sc1801host.DXE) $(CPPFLAGS_sc1801host.DXE) -o /home/ems/system/sc1801/v4.0a/src/sc1801host.dxe $(OBJS_sc1801host.DXE) \
	/home/ems/system/dms/v4.0a/lib/dblib.lib \
	/home/ems/system/dps/v4.0a/lib/ipclib.lib -lm -lsocket -lnsl -lthread 
## .a's were added twice to link line to make sure they are found in a link.


# Compile source files into .o's
$(TARGETDIR_sc1801host.DXE)/Host_init.o: Host_init.c
	$(COMPILE.c) $(CFLAGS_sc1801host.DXE) $(CPPFLAGS_sc1801host.DXE) -o $(TARGETDIR_sc1801host.DXE)/Host_init.o Host_init.c
$(TARGETDIR_sc1801host.DXE)/Host_app_fun.o: Host_app_fun.c
	$(COMPILE.c) $(CFLAGS_sc1801host.DXE) $(CPPFLAGS_sc1801host.DXE) -o $(TARGETDIR_sc1801host.DXE)/Host_app_fun.o Host_app_fun.c
$(TARGETDIR_sc1801host.DXE)/sc1801Host.o: sc1801Host.c
	$(COMPILE.c) $(CFLAGS_sc1801host.DXE) $(CPPFLAGS_sc1801host.DXE) -o $(TARGETDIR_sc1801host.DXE)/sc1801Host.o sc1801Host.c
$(TARGETDIR_sc1801host.DXE)/Host_fun.o: Host_fun.c
	$(COMPILE.c) $(CFLAGS_sc1801host.DXE) $(CPPFLAGS_sc1801host.DXE) -o $(TARGETDIR_sc1801host.DXE)/Host_fun.o Host_fun.c


###### clean target: deletes generated files ######
clean:
	$(RM) \
	/home/ems/system/sc1801/v4.0a/src/sc1801host.DXE \
	$(TARGETDIR_sc1801host.DXE)/Host_init.o \
	$(TARGETDIR_sc1801host.DXE)/Host_app_fun.o \
	$(TARGETDIR_sc1801host.DXE)/sc1801Host.o \
	$(TARGETDIR_sc1801host.DXE)/Host_fun.o 

# Enable dependency checking
#.KEEP_STATE:
#.KEEP_STATE_FILE: /home/ems/system/sc1801/v4.0a/src/.make.state.Makefile.ems.sc1801Host
