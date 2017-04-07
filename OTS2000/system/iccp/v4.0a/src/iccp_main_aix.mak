## -*- Makefile -*-
##
## Project: iccp_main
## User: ems
## Time: 06/04/25 17:30:30
## Makefile created by Sun WorkShop.
##
## This file is generated automatically -- DO NOT EDIT.
##



project: /home/ems/system/iccp/v4.0a/src/iccp_main.dxe 

##### Compilers and tools definitions shared by all build objects #####
CC = gcc
CFLAGS = -g 
RM = rm -f

LIBDIR     = ../TASE2lib
COMPILE.c = $(CC)
CFLAGS_ICCP_MAIN.DXE = $(CFLAGS) -c -DMOSI -DLEAN_T -I. -DDEBUG_SISCO -DMMS_LITE -DICCP_LITE -DETHERNET -DMLOG_ENABLE
CPPFLAGS_ICCP_MAIN.DXE = $(CPPFLAGS)

LINK.c = $(COMPILE.c) $(LDFLAGS) $(TARGET ARCH)


###### Target: /home/ems/system/iccp/v4.0a/src/iccp_main.dxe ######
TARGETDIR_ICCP_MAIN.DXE=/home/ems/system/iccp/v4.0a/src
CPPFLAGS_ICCP_MAIN.DXE += \
	-I/home/ems/system/dms/v4.0a/inc \
	-I/home/ems/system/lan/v4.0a/inc \
	-I/home/ems/system/dpp/v4.0a/inc \
	-I/home/ems/system/dps/v4.0a/inc \
	-I/home/openwin/include \
	-I/home/dt/include 	\
	-I../inc		\
	-I../TASE2inc		
	
OBJS_ICCP_MAIN.DXE = \
	  $(TARGETDIR_ICCP_MAIN.DXE)/iccp_main.o \
          $(TARGETDIR_ICCP_MAIN.DXE)/iccp_fun.o \
          $(TARGETDIR_ICCP_MAIN.DXE)/iccp_lib.o \
          $(TARGETDIR_ICCP_MAIN.DXE)/iccp_app_fun.o \
          $(TARGETDIR_ICCP_MAIN.DXE)/event.o \
	  $(TARGETDIR_ICCP_MAIN.DXE)/getval5.o \
	  $(TARGETDIR_ICCP_MAIN.DXE)/logcfgx.o \
	  $(TARGETDIR_ICCP_MAIN.DXE)/mit_devc.o \
	  $(TARGETDIR_ICCP_MAIN.DXE)/mit_imsg.o \
	  $(TARGETDIR_ICCP_MAIN.DXE)/mit_misc.o \
	  $(TARGETDIR_ICCP_MAIN.DXE)/mit_ssoc.o \
	  $(TARGETDIR_ICCP_MAIN.DXE)/mit_tran.o \
	  $(TARGETDIR_ICCP_MAIN.DXE)/mit_usr.o \
	  $(TARGETDIR_ICCP_MAIN.DXE)/mit_util.o \
	  $(TARGETDIR_ICCP_MAIN.DXE)/mit_var.o \
	  $(TARGETDIR_ICCP_MAIN.DXE)/mit_xmlc.o \
	  $(TARGETDIR_ICCP_MAIN.DXE)/miusrobj.o \
	  $(TARGETDIR_ICCP_MAIN.DXE)/mmsop_en.o \
	  $(TARGETDIR_ICCP_MAIN.DXE)/mvlop_en.o \
	  $(TARGETDIR_ICCP_MAIN.DXE)/mvl_acse.o \
	  $(TARGETDIR_ICCP_MAIN.DXE)/ukey.o\
	  /home/ems/system/dms/v4.0a/lib/dblib.lib \
	  /home/ems/system/dps/v4.0a/lib/ipclib.lib \
	  /home/ems/system/dpp/v4.0a/lib/dpplib.lib \
	  /home/ems/system/lan/v4.0a/lib/lan.lib 


# Link or archive
/home/ems/system/iccp/v4.0a/src/iccp_main.dxe: $(OBJS_ICCP_MAIN.DXE) 
	$(LINK.c) -o /home/ems/system/iccp/v4.0a/src/iccp_main.dxe $(OBJS_ICCP_MAIN.DXE) \
	$(LIBDIR)/mi_ld.a \
	$(LIBDIR)/mlogl_ld.a \
	$(LIBDIR)/mvl_ld.a \
        $(LIBDIR)/mmsl_ld.a \
        $(LIBDIR)/mmsle_ld.a \
        $(LIBDIR)/mem_ld.a \
        $(LIBDIR)/slog_ld.a \
        $(LIBDIR)/mlogl_ld.a \
        $(LIBDIR)/util_ld.a \
        $(LIBDIR)/ositcpe_ld.a \
        $(LIBDIR)/asn1l_ld.a \
	/home/ems/system/dms/v4.0a/lib/dblib.lib \
	/home/ems/system/dps/v4.0a/lib/ipclib.lib \
	/home/ems/system/dpp/v4.0a/lib/dpplib.lib \
	/home/ems/system/lan/v4.0a/lib/lan.lib  -lm -lnsl -lpthread #-lsocket  
## .a's were added twice to link line to make sure they are found in a link.


# Compile source files into .o's
$(TARGETDIR_ICCP_MAIN.DXE)/iccp_main.o:	iccp_main.c
	$(COMPILE.c) $(CFLAGS_ICCP_MAIN.DXE) $(CPPFLAGS_ICCP_MAIN.DXE) -o $(TARGETDIR_ICCP_MAIN.DXE)/iccp_main.o iccp_main.c
$(TARGETDIR_ICCP_MAIN.DXE)/iccp_fun.o: iccp_fun.c
	$(COMPILE.c) $(CFLAGS_ICCP_MAIN.DXE) $(CPPFLAGS_ICCP_MAIN.DXE) -o $(TARGETDIR_ICCP_MAIN.DXE)/iccp_fun.o iccp_fun.c
$(TARGETDIR_ICCP_MAIN.DXE)/iccp_lib.o: iccp_lib.c
	$(COMPILE.c) $(CFLAGS_ICCP_MAIN.DXE) $(CPPFLAGS_ICCP_MAIN.DXE) -o $(TARGETDIR_ICCP_MAIN.DXE)/iccp_lib.o iccp_lib.c
$(TARGETDIR_ICCP_MAIN.DXE)/iccp_app_fun.o: iccp_app_fun.c
	$(COMPILE.c) $(CFLAGS_ICCP_MAIN.DXE) $(CPPFLAGS_ICCP_MAIN.DXE) -o $(TARGETDIR_ICCP_MAIN.DXE)/iccp_app_fun.o iccp_app_fun.c
$(TARGETDIR_ICCP_MAIN.DXE)/event.o: event.c
	$(COMPILE.c) $(CFLAGS_ICCP_MAIN.DXE) $(CPPFLAGS_ICCP_MAIN.DXE) -o $(TARGETDIR_ICCP_MAIN.DXE)/event.o event.c
$(TARGETDIR_ICCP_MAIN.DXE)/getval5.o: getval5.c
	$(COMPILE.c) $(CFLAGS_ICCP_MAIN.DXE) $(CPPFLAGS_ICCP_MAIN.DXE) -o $(TARGETDIR_ICCP_MAIN.DXE)/getval5.o getval5.c
$(TARGETDIR_ICCP_MAIN.DXE)/logcfgx.o: logcfgx.c
	$(COMPILE.c) $(CFLAGS_ICCP_MAIN.DXE) $(CPPFLAGS_ICCP_MAIN.DXE) -o $(TARGETDIR_ICCP_MAIN.DXE)/logcfgx.o logcfgx.c
$(TARGETDIR_ICCP_MAIN.DXE)/mit_devc.o: mit_devc.c
	$(COMPILE.c) $(CFLAGS_ICCP_MAIN.DXE) $(CPPFLAGS_ICCP_MAIN.DXE) -o $(TARGETDIR_ICCP_MAIN.DXE)/mit_devc.o mit_devc.c
$(TARGETDIR_ICCP_MAIN.DXE)/mit_imsg.o: mit_imsg.c
	$(COMPILE.c) $(CFLAGS_ICCP_MAIN.DXE) $(CPPFLAGS_ICCP_MAIN.DXE) -o $(TARGETDIR_ICCP_MAIN.DXE)/mit_imsg.o mit_imsg.c
$(TARGETDIR_ICCP_MAIN.DXE)/mit_misc.o: mit_misc.c
	$(COMPILE.c) $(CFLAGS_ICCP_MAIN.DXE) $(CPPFLAGS_ICCP_MAIN.DXE) -o $(TARGETDIR_ICCP_MAIN.DXE)/mit_misc.o mit_misc.c
$(TARGETDIR_ICCP_MAIN.DXE)/mit_ssoc.o: mit_ssoc.c
	$(COMPILE.c) $(CFLAGS_ICCP_MAIN.DXE) $(CPPFLAGS_ICCP_MAIN.DXE) -o $(TARGETDIR_ICCP_MAIN.DXE)/mit_ssoc.o mit_ssoc.c
$(TARGETDIR_ICCP_MAIN.DXE)/mit_tran.o: mit_tran.c
	$(COMPILE.c) $(CFLAGS_ICCP_MAIN.DXE) $(CPPFLAGS_ICCP_MAIN.DXE) -o $(TARGETDIR_ICCP_MAIN.DXE)/mit_tran.o mit_tran.c
$(TARGETDIR_ICCP_MAIN.DXE)/mit_usr.o: mit_usr.c
	$(COMPILE.c) $(CFLAGS_ICCP_MAIN.DXE) $(CPPFLAGS_ICCP_MAIN.DXE) -o $(TARGETDIR_ICCP_MAIN.DXE)/mit_usr.o mit_usr.c
$(TARGETDIR_ICCP_MAIN.DXE)/mit_util.o: mit_util.c
	$(COMPILE.c) $(CFLAGS_ICCP_MAIN.DXE) $(CPPFLAGS_ICCP_MAIN.DXE) -o $(TARGETDIR_ICCP_MAIN.DXE)/mit_util.o mit_util.c
$(TARGETDIR_ICCP_MAIN.DXE)/mit_var.o: mit_var.c
	$(COMPILE.c) $(CFLAGS_ICCP_MAIN.DXE) $(CPPFLAGS_ICCP_MAIN.DXE) -o $(TARGETDIR_ICCP_MAIN.DXE)/mit_var.o mit_var.c
$(TARGETDIR_ICCP_MAIN.DXE)/mit_xmlc.o: mit_xmlc.c
	$(COMPILE.c) $(CFLAGS_ICCP_MAIN.DXE) $(CPPFLAGS_ICCP_MAIN.DXE) -o $(TARGETDIR_ICCP_MAIN.DXE)/mit_xmlc.o mit_xmlc.c
$(TARGETDIR_ICCP_MAIN.DXE)/miusrobj.o: miusrobj.c
	$(COMPILE.c) $(CFLAGS_ICCP_MAIN.DXE) $(CPPFLAGS_ICCP_MAIN.DXE) -o $(TARGETDIR_ICCP_MAIN.DXE)/miusrobj.o miusrobj.c
$(TARGETDIR_ICCP_MAIN.DXE)/mmsop_en.o: mmsop_en.c
	$(COMPILE.c) $(CFLAGS_ICCP_MAIN.DXE) $(CPPFLAGS_ICCP_MAIN.DXE) -o $(TARGETDIR_ICCP_MAIN.DXE)/mmsop_en.o mmsop_en.c
$(TARGETDIR_ICCP_MAIN.DXE)/mvlop_en.o: mvlop_en.c
	$(COMPILE.c) $(CFLAGS_ICCP_MAIN.DXE) $(CPPFLAGS_ICCP_MAIN.DXE) -o $(TARGETDIR_ICCP_MAIN.DXE)/mvlop_en.o mvlop_en.c
$(TARGETDIR_ICCP_MAIN.DXE)/mvl_acse.o: mvl_acse.c
	$(COMPILE.c) $(CFLAGS_ICCP_MAIN.DXE) $(CPPFLAGS_ICCP_MAIN.DXE) -o $(TARGETDIR_ICCP_MAIN.DXE)/mvl_acse.o mvl_acse.c
$(TARGETDIR_ICCP_MAIN.DXE)/ukey.o: ukey.c
	$(COMPILE.c) $(CFLAGS_ICCP_MAIN.DXE) $(CPPFLAGS_ICCP_MAIN.DXE) -o $(TARGETDIR_ICCP_MAIN.DXE)/ukey.o ukey.c


###### clean target: deletes generated files ######
clean:
	$(RM) \
	/home/ems/system/iccp/v4.0a/src/iccp_main.dxe \
	$(TARGETDIR_ICCP_MAIN.DXE)/iccp_main.o \
        $(TARGETDIR_ICCP_MAIN.DXE)/iccp_fun.o \
        $(TARGETDIR_ICCP_MAIN.DXE)/iccp_lib.o \
        $(TARGETDIR_ICCP_MAIN.DXE)/iccp_app_fun.o \
        $(TARGETDIR_ICCP_MAIN.DXE)/event.o \
	$(TARGETDIR_ICCP_MAIN.DXE)/getval5.o \
	$(TARGETDIR_ICCP_MAIN.DXE)/logcfgx.o \
	$(TARGETDIR_ICCP_MAIN.DXE)/mit_devc.o \
	$(TARGETDIR_ICCP_MAIN.DXE)/mit_imsg.o \
	$(TARGETDIR_ICCP_MAIN.DXE)/mit_misc.o \
	$(TARGETDIR_ICCP_MAIN.DXE)/mit_ssoc.o \
	$(TARGETDIR_ICCP_MAIN.DXE)/mit_tran.o \
	$(TARGETDIR_ICCP_MAIN.DXE)/mit_usr.o \
	$(TARGETDIR_ICCP_MAIN.DXE)/mit_util.o \
	$(TARGETDIR_ICCP_MAIN.DXE)/mit_var.o \
	$(TARGETDIR_ICCP_MAIN.DXE)/mit_xmlc.o \
	$(TARGETDIR_ICCP_MAIN.DXE)/miusrobj.o \
	$(TARGETDIR_ICCP_MAIN.DXE)/mmsop_en.o \
	$(TARGETDIR_ICCP_MAIN.DXE)/mvlop_en.o \
	$(TARGETDIR_ICCP_MAIN.DXE)/mvl_acse.o \
	$(TARGETDIR_ICCP_MAIN.DXE)/ukey.o

# Enable dependency checking
#.KEEP_STATE:
#.KEEP_STATE_FILE: /home/ems/system/iccp/v4.0a/src/.make.state.Makefile.ems.iccp_main
