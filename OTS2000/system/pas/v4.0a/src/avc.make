## -*- Makefile -*-
##
## Project: /home/ems/system/pas/v4.0a/src/avc.prd
## User: ems
## Time: 06/03/27 21:31:11
## Makefile created by Sun WorkShop.
##
## This file is generated automatically -- DO NOT EDIT.
##



project: /usr/ems/system/pas/v4.0a/src/avc.dxe 

##### Compilers and tools definitions shared by all build objects #####
CC=/opt/SUNWspro/bin/../WS6U1/bin/cc
CFLAGS=-g


###### Target: /usr/ems/system/pas/v4.0a/src/avc.dxe ######
TARGETDIR_AVC.DXE=/usr/ems/system/pas/v4.0a/src
CPPFLAGS_AVC.DXE += \
	-I/usr/openwin/include \
	-I/usr/dt/include 
OBJS_AVC.DXE = \
	$(TARGETDIR_AVC.DXE)/avc.o \
	/usr/ems/system/dms/v4.0a/lib/dblib.a \
	/usr/ems/system/lan/v4.0a/lib/lan.lib \
	/usr/ems/system/oix/v4.0a/lib/util.lib 


# Link or archive
/usr/ems/system/pas/v4.0a/src/avc.dxe: $(OBJS_AVC.DXE) 
	$(LINK.c)  $(CFLAGS_AVC.DXE) $(CPPFLAGS_AVC.DXE) -o /usr/ems/system/pas/v4.0a/src/avc.dxe $(OBJS_AVC.DXE) \
	/usr/ems/system/dms/v4.0a/lib/dblib.a -lm 
## .a's were added twice to link line to make sure they are found in a link.


# Compile source files into .o's
$(TARGETDIR_AVC.DXE)/avc.o: avc.c
	$(COMPILE.c) $(CFLAGS_AVC.DXE) $(CPPFLAGS_AVC.DXE) -o $(TARGETDIR_AVC.DXE)/avc.o avc.c


###### clean target: deletes generated files ######
clean:
	$(RM) \
	/usr/ems/system/pas/v4.0a/src/avc.dxe \
	$(TARGETDIR_AVC.DXE)/avc.o 

# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE: /usr/ems/system/pas/v4.0a/src/.make.state.Makefile.ems.avc
