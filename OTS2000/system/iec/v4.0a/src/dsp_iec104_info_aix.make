## -*- Makefile -*-
##
## Project: dsp_iec104_info.prd
## User: ems
## Time: 06/04/20 15:37:00
## Makefile created by Sun WorkShop.
##
## This file is generated automatically -- DO NOT EDIT.
##



project: /home/ems/system/iec/v4.0a/src/dsp_iec104_info.dxe 

##### Compilers and tools definitions shared by all build objects #####
#CC=/opt/SUNWspro/bin/../WS6U1/bin/cc
CC = gcc
BASICOPTS = -g
CFLAGS = $(BASICOPTS)

COMPILE.c = $(CC)
CFLAGS_INFOGEN.DXE = $(CFLAGS) -c
CPPFLAGS_INFOGEN.DXE = $(CPPFLAGS)

LINK.c = $(COMPILE.c) $(LDFLAGS) $(TARGET ARCH)


###### Target: dsp_iec104_info.dxe ######
TARGETDIR_INFOGEN.DXE=/home/ems/system/iec/v4.0a/src
CPPFLAGS_INFOGEN.DXE += \
	-I/usr/openwin/include \
	-I/usr/dt/include 
OBJS_INFOGEN.DXE = \
	$(TARGETDIR_INFOGEN.DXE)/dsp_iec104_info.o \
	/home/ems/system/dms/v4.0a/lib/dblib.lib


# Link or archive
/home/ems/system/iec/v4.0a/src/dsp_iec104_info.dxe: $(OBJS_INFOGEN.DXE) 
	$(LINK.c) -o $@ $(OBJS_INFOGEN.DXE) 
## .a's were added twice to link line to make sure they are found in a link.


# Compile source files into .o's
$(TARGETDIR_INFOGEN.DXE)/dsp_iec104_info.o: dsp_iec104_info.c
	$(COMPILE.c) $(CFLAGS_INFOGEN.DXE) $(CPPFLAGS_INFOGEN.DXE) -o $(TARGETDIR_INFOGEN.DXE)/dsp_iec104_info.o dsp_iec104_info.c


###### clean target: deletes generated files ######
clean:
	$(RM) \
	/home/ems/system/iec/v4.0a/src/dsp_iec104_info.dxe \
	$(TARGETDIR_INFOGEN.DXE)/dsp_iec104_info.o 

# Enable dependency checking
#.KEEP_STATE:
#.KEEP_STATE_FILE: /home/ems/system/iec/v4.0a/src/.make.state.Makefile.ems.dsp_iec104_info
