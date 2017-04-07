#************************************************************************/
#* SISCO SOFTWARE MODULE HEADER *****************************************/
#************************************************************************/
#*			       
#* MODULE NAME : mmslog.mak                       
#* PRODUCT(S)  : MMS-EASE-Lite
#*			       
#* MODULE DESCRIPTION :        
#* This makefile is used by the HP-UX 'make' utility to build the Named
#* Type generator for MMS-EASE-Lite.  All paths including header 
#* files and object modules are relative to the current project directory 
#* which makes this file as environment independent as possible.  
#* Usage instructions are given below as well as a brief description of 
#* each switch used in the process.  
#*
#* To make the mmslog.exe, enter the following:
#*
#* 	nmake -f mmslog.mak 
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
#* 11/25/02   -     10     proAsset: HP-UX adaption, aCC compiler
#* 06/11/02  GLB    09     Updated for ICCP-Lite
#* 02/05/02  GLB    08     Added mms_rtaa.c
#* 01/09/02  GLB    07     Use slog_ld instead of slog_n
#* 12/20/01  GLB    06     Use logging/no debug ( _l) version of libraries
#* 08/07/01  RKR    05     Removed S_THISFILE
#* 08/15/00  GLB    04     Added VPATH macro to work with new "make"
#* 12/03/99  GLB    03     Print build file name on screen during build
#* 11/07/99  GLB    02     Added .c.o:
#* 11/02/98  JRB    01     Created from DOS version                                      
#************************************************************************/

#set the compiler
CC         = gcc

MMSSRCDIR = ../../../src
LIBDIR = ../../../lib
OBJDIR = .
SRCDIR = .
INCDIR = ../../../inc
MMSOPDIR = .
EXEDIR = ../../../bin

VPATH  = $(MMSSRCDIR)

PROJ  = mmslog
OPT   =
DEFS  =
LIST  = >> cc.lst 2>&1

CFLAGS = -g -c $(OPT) -I$(INCDIR) $(DEFS) -DMMS_LITE -DNO_MVL \
	  -DFOUNDRY \
	-I$(MMSOPDIR) -DMLOG_ENABLE -DDEBUG_SISCO 

LIBS = $(LIBDIR)/mmsl_ld.a $(LIBDIR)/mmsle_ld.a $(LIBDIR)/asn1l_ld.a \
       $(LIBDIR)/mem_ld.a $(LIBDIR)/mlogl_ld.a \
       $(LIBDIR)/slog_ld.a $(LIBDIR)/mmsl_ld.a $(LIBDIR)/asn1l_ld.a $(LIBDIR)/util_ld.a

EXE = $(EXEDIR)/$(PROJ)
LFLAGS = -v -lm $(OPT) -o$(EXE) 

.PRECIOUS:	$(EXE)

INCLUDES = $(INCDIR)/glbtypes.h \
        $(INCDIR)/mvl_defs.h \
	$(INCDIR)/sysincs.h \
	$(INCDIR)/mmsdefs.h \
	$(INCDIR)/glberror.h \
	$(INCDIR)/gen_list.h \
	$(INCDIR)/mem_chk.h \
	$(INCDIR)/mms_pvar.h \
	$(INCDIR)/mms_mp.h \
	$(INCDIR)/asn1defs.h \
	$(INCDIR)/slog.h \
	$(INCDIR)/mms_vvar.h \
	$(INCDIR)/mms_err.h \
	$(INCDIR)/mms_def2.h \
	$(INCDIR)/mmsintr2.h \
	$(INCDIR)/mmsldefs.h \
	$(INCDIR)/mms_dfun.h \
	$(INCDIR)/mms_perr.h \
	$(INCDIR)/mms_log.h \
	$(INCDIR)/mmslog.h \
	$(INCDIR)/mms_pcon.h \
	$(INCDIR)/mms_pdom.h \
	$(INCDIR)/mms_pevn.h \
	$(INCDIR)/mms_pfil.h \
	$(INCDIR)/mms_pjou.h \
	$(INCDIR)/mms_pocs.h \
	$(INCDIR)/mms_pprg.h \
	$(INCDIR)/mms_psem.h \
	$(INCDIR)/mms_pvmd.h \
	$(INCDIR)/mmsefun.h \
	$(MMSOPDIR)/mmsop_en.h 

OBJECTS = mmslog.o \
	arg.o \
	mms_alta.o \
	mms_arb.o \
	mms_rtaa.o \
	mms_tdef.o \
	mmsdata.o \
	mmsdataa.o \
	mmsdatat.o \
	mmslvar.o \
	mmsop_en.o \
	ms_tdef.o \
	ms_tdef2.o \
	ms_tdef3.o \
	readhex.o \
	sx_data.o 

# Main target is the executable
$(EXE):	$(OBJECTS) $(LIBS)
	rm -f $(EXE)
	$(CC) $(LFLAGS)\
		$(OBJECTS) \
		$(LIBS) $(LIST)
	@echo "FINISHED GENERATING $(EXE) EXECUTABLE" $(LIST)
	@echo "-----------------------------------------------------" $(LIST)

#************************************************************************/
.c.o:
	echo mmslog.mak: $< $(LIST)
	$(CC) $(CFLAGS) -o $*.o $< $(LIST)

clean:
	-rm $(OBJECTS)
	-rm $(EXE)
	-rm $(EXE).map
	-rm $(OBJDIR)/cc.lst

# Here is where we compile the C files into objects

mmslog.o:	$(SRCDIR)/mmslog.c $(INCLUDES)

arg.o:		arg.c $(INCLUDES)

mms_alta.o:	mms_alta.c $(INCLUDES)

mms_arb.o:	mms_arb.c $(INCLUDES)

mms_rtaa.o:	mms_rtaa.c $(INCLUDES)

mms_tdef.o:	mms_tdef.c $(INCLUDES)

mmsdata.o:	mmsdata.c $(INCLUDES)

mmsdataa.o:	mmsdata.c $(INCLUDES)

mmsdatat.o:	mmsdatat.c $(INCLUDES)

mmslvar.o:	mmslvar.c $(INCLUDES)

mmsop_en.o:	mmsop_en.c $(INCLUDES)

ms_tdef.o:	ms_tdef.c $(INCLUDES)

ms_tdef2.o:	ms_tdef2.c $(INCLUDES)

ms_tdef3.o:	ms_tdef3.c $(INCLUDES)

readhex.o:	readhex.c $(INCLUDES)

sx_data.o:	sx_data.c $(INCLUDES)

