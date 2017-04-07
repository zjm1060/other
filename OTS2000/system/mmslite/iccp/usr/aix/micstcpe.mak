#************************************************************************/
#* SISCO SOFTWARE MODULE HEADER *****************************************/
#************************************************************************/
#*			       
#* MODULE NAME : micstcpe.mak                       
#* PRODUCT(S)  : MMS-EASE-Lite
#*			       
#* MODULE DESCRIPTION :        
#* This makefile is used by the HP-UX 'make' utility to build the Client TCP
#* example for ICCP-Lite. All paths including header 
#* files and object modules are relative to the current project directory 
#* which makes this file as environment independent as possible.  
#* Usage instructions are given below as well as a brief description of 
#* each switch used in the process.  
#*
#* To make the MICSTCPE executable, enter the following:
#*
#* 	make -f hpux/micstcpe.mak
#*
#* Description of HP aCC compiler options used:
#*  -c       Suppress the link phase and retain object files
#*  -v       Verbose mode
#*  -g       Produce symbolic info in the object file for debugger
#*  +Onoall  Generate non-optimized code sequences
#*  +w       Warning output
#*
#* Description of HP-UX linker options used:
#*  -o outfile   	Output file path for executable file
#*
#* MODIFICATION LOG:
#*  Date     Who   Rev			Comments
#* --------  ---  ------   -------------------------------------------
#* 11/25/02   -     04     proAsset: HP-UX adaption, aCC compiler
#* 07/22/02  GLB    03     Remove any reference to ml_init.c                  
#* 06/11/02  GLB    02     Updated
#* 06/22/01  ASK    01     Created
#************************************************************************/

#set the compiler
CC         = gcc

#************************************************************************/
# Define directories here

ACSESRCDIR   = ../../mvl/src/acse
ALIGNDIR     = ../../mvl/util/foundry/hpux
EXEDIR       = .
FOUNDRYDIR   = ../../bin
ICCPSRCDIR   = ../src
ICCPUSRDIR   = .
INCDIR       = ../../inc
INCDIR2      = .
LIBDIR       = ../../lib
MMSOPDIR     = .
MMSSRCDIR    = ../../src
MVLSRCDIR    = ../../mvl/src
MVLUSRDIR    = ../../mvl/usr
OBJDIR       = .
SRCDIR       = ../../src
USRSRCDIR    = ../../mvl/usr

VPATH = $(MVLSRCDIR):$(MVLSRCDIR)/acse:$(USRSRCDIR):$(SRCDIR):$(ALIGNDIR)

PROJ  = micstcpe
OPT   =
DEFS  =
LIST  = >> cc.lst 2>&1

#************************************************************************/
# Define compiler/linker switches to use. These depend on the target ...

CFLAGS = -g -c +inst_auto +inst_close $(OPT) -I$(INCDIR) -I$(INCDIR2) $(DEFS) -DMMS_LITE \
	 -DMOSI -DLEAN_T -I$(MMSOPDIR) -DDEBUG_SISCO \
         -DETHERNET -DICCP_LITE -DMLOG_ENABLE -DTP0_ENABLED

LIBS = $(LIBDIR)/mi_ld.a $(LIBDIR)/mlogl_ld.a \
       $(LIBDIR)/mvl_ld.a $(LIBDIR)/mmsl_ld.a $(LIBDIR)/mmsle_ld.a \
       $(LIBDIR)/mem_ld.a $(LIBDIR)/slog_ld.a \
       $(LIBDIR)/mlogl_ld.a \
       $(LIBDIR)/util_ld.a $(LIBDIR)/ositcpe_ld.a $(LIBDIR)/asn1l_ld.a 

EXE = $(EXEDIR)/$(PROJ)
LFLAGS = -v -g -o $(EXE)

#************************************************************************/
# Define includes dependancies

INCLUDES = $(INCDIR)/icfg.h \
           $(INCDIR)/mloguser.h \
           $(INCDIR)/mi.h \
           $(INCDIR)/mi_icfg.h \
           $(INCDIR)/mi_log.h \
           $(ICCPUSRDIR)/mics_cfg.h \
	   $(INCDIR)/acse2usr.h \
	   $(INCDIR)/asn1defs.h \
	   $(INCDIR)/asn1r.h \
	   $(INCDIR)/clnp_usr.h \
	   $(INCDIR)/gen_list.h \
	   $(INCDIR)/glberror.h \
	   $(INCDIR)/glbopt.h \
	   $(INCDIR)/glbsem.h \
	   $(INCDIR)/glbtypes.h \
	   $(INCDIR)/lean_a.h \
	   $(INCDIR)/mem_chk.h \
	   $(INCDIR)/mms_def2.h \
	   $(INCDIR)/mms_dfun.h \
	   $(INCDIR)/mms_err.h \
	   $(INCDIR)/mms_log.h \
	   $(INCDIR)/mms_mp.h \
	   $(INCDIR)/mms_pcon.h \
	   $(INCDIR)/mms_pdom.h \
	   $(INCDIR)/mms_perr.h \
	   $(INCDIR)/mms_pevn.h \
	   $(INCDIR)/mms_pfil.h \
	   $(INCDIR)/mms_pjou.h \
	   $(INCDIR)/mms_pocs.h \
	   $(INCDIR)/mms_pprg.h \
	   $(INCDIR)/mms_psem.h \
	   $(INCDIR)/mms_pvar.h \
	   $(INCDIR)/mms_pvmd.h \
	   $(INCDIR)/mms_vvar.h \
	   $(INCDIR)/mmsdefs.h \
	   $(INCDIR)/mmsefun.h \
	   $(INCDIR)/mmsintr2.h \
	   $(INCDIR)/mmsldefs.h \
           $(INCDIR)/mmslog.h \
	   $(INCDIR)/slog.h \
	   $(INCDIR)/stime.h \
	   $(INCDIR)/suicacse.h \
	   $(INCDIR)/sysincs.h \
	   $(INCDIR)/tp4api.h \
	   mi_usr.h \
	   mmsop_en.h \
           miusrobj.h

#************************************************************************/
# Define objects used in the executable

OBJECTS = mic_usr.o \
	  mics_cfg.o \
	  mis_usr.o \
	  miusrobj.o \
	  mmslvar.o \
	  mmsop_en.o \
	  mvlop_en.o \
	  mvl_acse.o \
          mi_main.o \
	  logcfgx.o \
	  ukey.o
#************************************************************************/
# The main target is the execcutable

$(EXE):	$(OBJECTS) $(LIBS)
	rm -f $(EXE)
	$(CC) $(LFLAGS)\
		$(OBJECTS) \
		$(LIBS) -lm -lsocket -lnsl $(LIST)
	@echo "FINISHED GENERATING $(EXE) EXECUTABLE" $(LIST)
	@echo "-----------------------------------------------------" $(LIST)
	rm $(OBJDIR)/*.o


#************************************************************************/
# Here we create the 'C' files via the 'mbufcalc' and 'foundry' utilities

# miusrobj.h is created using the foundry utility.
$(ICCPUSRDIR)/miusrobj.c: $(ICCPUSRDIR)/miusrobj.odf 
	$(FOUNDRYDIR)/foundry -o -c$(FOUNDRYDIR)/align.cfg $(ICCPUSRDIR)/miusrobj.odf $(ICCPUSRDIR)/miusrobj.h $(LIST)

#************************************************************************/
# Here is where we compile the C files into objects

.c.o:
	echo micstcpe.mak: $< $(LIST)
	$(CC) $(CFLAGS) -o $*.o $< $(LIST)

$(OBJDIR)/event.o:      $(USRSRCDIR)/event.c $(INCLUDES)

$(OBJDIR)/mi_main.o:	$(ICCPUSRDIR)/mi_main.c $(INCLUDES)

$(OBJDIR)/mic_usr.o:	$(ICCPUSRDIR)/mic_usr.c $(INCLUDES)

$(OBJDIR)/mics_cfg.o:	$(ICCPUSRDIR)/mics_cfg.c $(INCLUDES)

$(OBJDIR)/mis_usr.o:	$(ICCPUSRDIR)/mis_usr.c $(INCLUDES)

$(OBJDIR)/miusrobj.o:	$(ICCPUSRDIR)/miusrobj.c $(INCLUDES)

$(OBJDIR)/mmslvar.o:	$(SRCDIR)/mmslvar.c $(INCLUDES)

$(OBJDIR)/mmsop_en.o:	$(SRCDIR)/mmsop_en.c $(INCLUDES)

$(OBJDIR)/mvlop_en.o:	$(MVLSRCDIR)/mvlop_en.c $(INCLUDES)

$(OBJDIR)/mvl_acse.o:	$(MVLSRCDIR)/acse/mvl_acse.c $(INCLUDES)

$(OBJDIR)/logcfgx.o:	$(USRSRCDIR)/logcfgx.c $(INCLUDES)

