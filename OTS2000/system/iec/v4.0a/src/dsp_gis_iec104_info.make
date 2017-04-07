## -*- Makefile -*-
##
## Project: /home/ems/system/iec/v4.0a/src/dsp_gis_iec104_info.prd
## User: ems
## Time: 06/04/21 15:13:51
## Makefile created by Sun WorkShop.
##
## This file is generated automatically -- DO NOT EDIT.
##



project: /home/ems/system/iec/v4.0a/src/dsp_gis_iec104_info.dxe 

##### Compilers and tools definitions shared by all build objects #####
#CC=/opt/SUNWspro/bin/../WS6U1/bin/cc
CC=cc
CFLAGS=-g -xCC 


###### Target: /home/ems/system/iec/v4.0a/src/dsp_gis_iec104_info.dxe ######
TARGETDIR_IEC104_COM.DXE=/home/ems/system/iec/v4.0a/src
CPPFLAGS_IEC104_COM.DXE += \
	-I/usr/openwin/include \
	-I/usr/dt/include 
OBJS_IEC104_COM.DXE = \
	$(TARGETDIR_IEC104_COM.DXE)/dsp_gis_iec104_info.o \
	$(TARGETDIR_IEC104_COM.DXE)/iec_cmm_util.o \
	/home/ems/system/dms/v4.0a/lib/dblib.lib \
	/home/ems/system/dps/v4.0a/lib/ipclib.lib \
	/home/ems/system/lan/v4.0a/lib/lan.lib \
	/home/ems/system/dps/v4.0a/lib/dpslib.lib


# Link or archive
/home/ems/system/iec/v4.0a/src/dsp_gis_iec104_info.dxe: $(OBJS_IEC104_COM.DXE) 
	$(LINK.c)  $(CFLAGS_IEC104_COM.DXE) $(CPPFLAGS_IEC104_COM.DXE) -o /home/ems/system/iec/v4.0a/src/dsp_gis_iec104_info.dxe $(OBJS_IEC104_COM.DXE) \
		/home/ems/system/lan/v4.0a/lib/lan.lib \
		/home/ems/system/dps/v4.0a/lib/ipclib.lib \
		/home/ems/system/dms/v4.0a/lib/dblib.lib \
		-lm -lsocket -lnsl 
## .a's were added twice to link line to make sure they are found in a link.


# Compile source files into .o's
$(TARGETDIR_IEC104_COM.DXE)/dsp_gis_iec104_info.o: /home/ems/system/iec/v4.0a/src/dsp_gis_iec104_info.c
	$(COMPILE.c) $(CFLAGS_IEC104_COM.DXE) $(CPPFLAGS_IEC104_COM.DXE) -o $(TARGETDIR_IEC104_COM.DXE)/dsp_gis_iec104_info.o /home/ems/system/iec/v4.0a/src/dsp_gis_iec104_info.c
$(TARGETDIR_IEC104_COM.DXE)/iec_cmm_util.o: /home/ems/system/iec/v4.0a/src/iec_cmm_util.c
	$(COMPILE.c) $(CFLAGS_IEC104_COM.DXE) $(CPPFLAGS_IEC104_COM.DXE) -o $(TARGETDIR_IEC104_COM.DXE)/iec_cmm_util.o /home/ems/system/iec/v4.0a/src/iec_cmm_util.c


###### clean target: deletes generated files ######
clean:
	$(RM) \
	/home/ems/system/iec/v4.0a/src/dsp_gis_iec104_info.dxe \
	$(TARGETDIR_IEC104_COM.DXE)/dsp_gis_iec104_info.o \
	$(TARGETDIR_IEC104_COM.DXE)/iec_cmm_util.o 

# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE: /home/ems/system/iec/v4.0a/src/.make.state.Makefile.ems.dsp_gis_iec104_info
