#************************************************************************/
#* 	CIWA SOFTWARE MODULE HEADER		*/
#************************************************************************/
#*			       
#* MODULE NAME : iccp_s_main.mak                       
#*			       
#* MODULE DESCRIPTION :        
#* This makefile is used by the Solaris  'make' utility to build the ICCP
#* application. All paths including header and source
#* files and object modules are relative to the current iccp directory 
#* which makes this file as environment independent as possible.  
#*
#* To make the APPLICATION executable, enter the following:
#*
#* 	make -f iccp_s_main.mak
#*
#*
#* MODIFICATION LOG:
#*  Date     Who   Rev			Comments
#* --------  ---  ------   -------------------------------------------
#* 03/28/07  ZY    02     Updated
#* 12/22/06  ZY    01     Created iccp_s_main.mak
#************************************************************************/
#
#************************************************************************/

# set the compiler
CC         = cc

# define directories
# assume: initial directory location is /home/ems/system/iccp/v4.0a/src
SRCDIR     = .
INCDIR0    = ../inc
INCDIR1    = ../TASE2inc
LIBDIR     = ../TASE2lib
EXEDIR     = .

PROJ  = iccp_s_main.dxe
OPT   =
DEFS  =
#LIST  = >> cc.lst 2>&1
LIST  = 

# Define compiler/linker switches to use. These depend on the target ...
# CFLAGS = -c -v -g +w +Onoall $(OPT) -I$(INCDIR0) -I$(INCDIR1) -I$(INCDIR2) $(DEFS)

CFLAGS = -c -v -g +w +Onoall $(OPT) -I$(INCDIR0) -I$(INCDIR1) $(DEFS) \
	 -DMOSI -DLEAN_T -I. -DDEBUG_SISCO \
         -DMMS_LITE -DICCP_LITE -DETHERNET -DMLOG_ENABLE
LFLAGS = -v -g -o $(EXE) 


LIBS = $(LIBDIR)/mi_ld.a $(LIBDIR)/mlogl_ld.a $(LIBDIR)/mvl_ld.a \
       $(LIBDIR)/mmsl_ld.a $(LIBDIR)/mmsle_ld.a $(LIBDIR)/mem_ld.a \
       $(LIBDIR)/slog_ld.a $(LIBDIR)/mlogl_ld.a $(LIBDIR)/util_ld.a \
       $(LIBDIR)/ositcpe_ld.a $(LIBDIR)/asn1l_ld.a \
	/home/ems/system/dms/v4.0a/lib/dblib.lib \
	/home/ems/system/dps/v4.0a/lib/ipclib.lib \
	/home/ems/system/dpp/v4.0a/lib/dpplib.lib \
	/home/ems/system/lan/v4.0a/lib/lan.lib 

EXE = $(EXEDIR)/$(PROJ)

# define objects that belong to the executable program
OBJECTS = \
          iccp_s_main.o \
          iccp_s_app_fun.o \
          iccp_fun.o \
          iccp_lib.o \
          event.o \
	  getval5.o \
	  logcfgx.o \
	  mit_devc.o \
	  mit_imsg.o \
	  mit_misc.o \
	  mit_ssoc.o \
	  mit_tran.o \
	  mit_usr.o \
	  mit_util.o \
	  mit_var.o \
	  mit_xmlc.o \
	  miusrobj.o \
	  mmsop_en.o \
	  mvlop_en.o \
	  mvl_acse.o \
	  ukey.o

#************************************************************************/
# main target
$(EXE): $(OBJECTS) $(LIBS)
	-rm -f $(EXE) $(LIST)
	$(CC) $(LFLAGS) \
		$(OBJECTS) \
		$(LIBS) \
		/home/ems/system/dms/v4.0a/lib/dblib.lib -lm -lsocket -lnsl $(LIST)
	-rm -f $(OBJECTS)
	@echo "FINISHED GENERATING $(EXE) PROGRAM" $(LIST)

# compile C files into objects
.c.o:
	echo iccp_s_main.mak: $< $(LIST)
	$(CC) $(CFLAGS) -o $*.o $< $(LIST)

iccp_s_main.o:	iccp_s_main.c

iccp_s_app_fun.o:	iccp_s_app_fun.c

iccp_fun.o:	iccp_fun.c

iccp_lib.o:	iccp_lib.c

event.o:	event.c

getval5.o:	getval5.c

logcfgx.o:	logcfgx.c

mit_devc.o:	mit_devc.c

mit_imsg.o:	mit_imsg.c

mit_misc.o:	mit_misc.c

mit_ssoc.o:	mit_ssoc.c

mit_tran.o:	mit_tran.c

mit_usr.o:	mit_usr.c

mit_util.o:	mit_util.c

mit_var.o:	mit_var.c

mit_xmlc.o:	mit_xmlc.c

miusrobj.o:	miusrobj.c

mmsop_en.o:	mmsop_en.c

mvlop_en.o:	mvlop_en.c

mvl_acse.o:	mvl_acse.c

ukey.o:	ukey.c

