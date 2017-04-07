## -*- Makefile -*-
##
## Project: SimuEngine.prd
## User: ems
## Time: 06/04/25 17:37:00
## Makefile created by Sun WorkShop.
##
## This file is generated automatically -- DO NOT EDIT.
##



project: /home/ems/system/ots/v4.0a/src/SimuEngine.dxe 

##### Compilers and tools definitions shared by all build objects #####
CC=cc
CFLAGS=-g -mt -xCC 


###### Target: lan_inb.dxe ######
TARGETDIR_SimuEngine.DXE=/home/ems/system/ots/v4.0a/src
OBJS_SimuEngine.DXE = \
	$(TARGETDIR_SimuEngine.DXE)/get_coded_cmd.o \
	$(TARGETDIR_SimuEngine.DXE)/interpolation.o \
	$(TARGETDIR_SimuEngine.DXE)/load_SimuEngine_rtdb.o \
	$(TARGETDIR_SimuEngine.DXE)/model_proc.o \
	$(TARGETDIR_SimuEngine.DXE)/read_recv_buf.o \
	$(TARGETDIR_SimuEngine.DXE)/load_SimuEngine_rtdb.o \	
	/home/ems/system/dms/v4.0a/lib/dblib.lib \
	/home/ems/system/dps/v4.0a/lib/ipclib.lib \
	/home/ems/system/lan/v4.0a/lib/lan.lib \


# Link or archive
/home/ems/system/lan/v4.0a/src/lan_inb.dxe: $(OBJS_LAN_INB.DXE) 
	$(LINK.c)  $(CFLAGS_LAN_INB.DXE) $(CPPFLAGS_LAN_INB.DXE) -o /home/ems/system/lan/v4.0a/src/lan_inb.dxe $(OBJS_LAN_INB.DXE) \
	/home/ems/system/dms/v4.0a/lib/dblib.lib \
	/home/ems/system/dps/v4.0a/lib/ipclib.lib \
	/home/ems/system/dms/v4.0a/lib/dblib.lib \
	-lsocket -lnsl -lthread -lpthread -lposix4 -lm
## .a's were added twice to link line to make sure they are found in a link.


# Compile source files into .o's
$(TARGETDIR_LAN_INB.DXE)/lan_inb.o: lan_inb.c
	$(COMPILE.c) $(CFLAGS_LAN_INB.DXE) $(CPPFLAGS_LAN_INB.DXE) -o $(TARGETDIR_LAN_INB.DXE)/lan_inb.o lan_inb.c
$(TARGETDIR_LAN_INB.DXE)/netudp.o: netudp.c
	$(COMPILE.c) $(CFLAGS_LAN_INB.DXE) $(CPPFLAGS_LAN_INB.DXE) -o $(TARGETDIR_LAN_INB.DXE)/netudp.o netudp.c
$(TARGETDIR_LAN_INB.DXE)/udp.o: udp.c
	$(COMPILE.c) $(CFLAGS_LAN_INB.DXE) $(CPPFLAGS_LAN_INB.DXE) -o $(TARGETDIR_LAN_INB.DXE)/udp.o udp.c


###### clean target: deletes generated files ######
clean:
	$(RM) \
	/home/ems/system/lan/v4.0a/src/lan_inb.dxe \
	$(TARGETDIR_LAN_INB.DXE)/lan_inb.o \
	$(TARGETDIR_LAN_INB.DXE)/netudp.o \
	$(TARGETDIR_LAN_INB.DXE)/udp.o 

# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE: /home/ems/system/lan/v4.0a/src/.make.state.Makefile.ems.lan_inb
