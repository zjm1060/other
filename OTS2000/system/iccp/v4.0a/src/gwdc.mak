#
#************************************************************************/

# set the compiler
CC         = CC

# define directories
# assume: initial directory location is gway/gwdc
SRCDIR     = .
INCDIR0    = ../gwinc
INCDIR1    = ../TASE2inc
INCDIR2    = .
LIBDIR0    = ../gwlib.new
LIBDIR1    = ../TASE2lib
EXEDIR     = ../gwbin.new

VPATH = $(SRCDIR):../gwmain

PROJ  = gwdc
OPT   =
DEFS  =
LIST  = >> cc.lst 2>&1

# Define compiler/linker switches to use. These depend on the target ...
CFLAGS = -c -v -g +w +Onoall $(OPT) -I$(INCDIR0) -I$(INCDIR1) $(DEFS) \
	 -DMOSI -DLEAN_T -I. -DDEBUG_SISCO \
         -DMMS_LITE -DICCP_LITE -DETHERNET -DMLOG_ENABLE
LFLAGS = -v -g -o $(EXE) 

LIBS = $(LIBDIR0)/gwutl.a \
       $(LIBDIR1)/mi_ld.a $(LIBDIR1)/mlogl_ld.a $(LIBDIR1)/mvl_ld.a \
       $(LIBDIR1)/mmsl_ld.a $(LIBDIR1)/mmsle_ld.a $(LIBDIR1)/mem_ld.a \
       $(LIBDIR1)/slog_ld.a $(LIBDIR1)/mlogl_ld.a $(LIBDIR1)/util_ld.a \
       $(LIBDIR1)/ositcpe_ld.a $(LIBDIR1)/asn1l_ld.a

EXE = $(EXEDIR)/$(PROJ)

# define objects that belong to the executable program
OBJECTS = \
          gwdc.o \
          gwdcloop.o \
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
	  ukey.o \
          gwprtfct.o

#************************************************************************/
# main target
$(EXE): $(OBJECTS) $(LIBS)
	-rm -f $(EXE) $(LIST)
	$(CC) $(LFLAGS) $(OBJECTS) $(LIBS) $(LIST)
	-rm -f $(OBJECTS)
	@echo "FINISHED GENERATING $(EXE) PROGRAM" $(LIST)

# compile C files into objects
.c.o:
	echo gwdc.mak: $< $(LIST)
	$(CC) $(CFLAGS) -o $*.o $< $(LIST)

event.o:	event.c

getval5.o:	getval5.c

logcfgx.o:	logcfgx.c

mit_devc.o:	mit_devc.c

mit_imsg.o:	mit_imsg.c

mit_main.o:	mit_main.c

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

