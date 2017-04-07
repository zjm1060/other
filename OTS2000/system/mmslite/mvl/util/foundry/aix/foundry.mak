#************************************************************************/
#* SISCO SOFTWARE MODULE HEADER *****************************************/
#************************************************************************/
#*			       
#* MODULE NAME : foundry.mak                       
#* PRODUCT(S)  : MMS-EASE-Lite
#*			       
#* MODULE DESCRIPTION :        
#*	This makefile is used by the QNX 'make' utility to build the Named
#* Type generator for MMS-EASE-Lite.  All paths including header 
#* files and object modules are relative to the current project directory 
#* which makes this file as environment independent as possible.  
#* Usage instructions are given below as well as a brief description of 
#* each switch used in the process.  
#*
#* To make the foundry executable, enter the following:
#*
#* 	make -f hpux/foundry.mak
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
#*  -m			Produce a symbolic map file (defults to .map ext)
#*
#* MODIFICATION LOG:
#*  Date     Who   Rev			Comments
#* --------  ---  ------   -------------------------------------------
#* 11/25/02   -     11     proAsset: HP-UX adaption, aCC compiler
#* 06/28/02  GLB    10     Updated for ICCP-Lite
#* 02/05/02  GLB    09     Added mms_rtaa.c
#* 01/09/02  GLB    08     Use slog_l instead of slog_n
#* 12/20/01  GLB    07     Use logging/no debug (_l) version of libraries
#* 10/09/01  JRB    06     Must link to debug libs.
#* 08/07/01  RKR    05     Removed S_THISFILE
#* 08/15/00  GLB    04     Added VPATH macro to work with new "make"
#* 12/03/99  GLB    03     Print build file name on screen during build
#* 11/23/99  GLB    02     Added .c.o:
#* 08/15/97  EJV    01     Created from DOS version
#************************************************************************/

#set the compiler
#CC         = aCC
CC         = gcc

MMSSRCDIR = ../../../src
VPATH  = $(MMSSRCDIR)

LIBDIR = ../../../lib
OBJDIR = .
INCDIR = ../../../inc
EXEDIR = ../../../bin

PROJ  = foundry
OPT   =
DEFS  =
LIST  = >> cc.lst 2>&1

# set compiler flags
CFLAGS = -g -c $(OPT) -I$(INCDIR) $(DEFS) -DMMS_LITE \
	  -DFOUNDRY -DDEBUG_SISCO

LIBS = $(LIBDIR)/mmsl_ld.a $(LIBDIR)/asn1l_ld.a $(LIBDIR)/mem_ld.a \
       $(LIBDIR)/mvl_ld.a $(LIBDIR)/slog_ld.a $(LIBDIR)/util_ld.a \
       $(LIBDIR)/smem_ld.a

EXE = $(EXEDIR)/$(PROJ)
LFLAGS = -v -lm $(OPT) -o $(EXE) 

.PRECIOUS:	$(EXE)

# setup the dependencies for the source files
INCLUDES = $(INCDIR)/glbtypes.h \
	$(INCDIR)/sysincs.h \
	$(INCDIR)/mmsdefs.h \
	$(INCDIR)/glberror.h \
	$(INCDIR)/gen_list.h \
	$(INCDIR)/mem_chk.h \
	$(INCDIR)/mms_pvar.h \
	$(INCDIR)/mms_mp.h \
	$(INCDIR)/asn1defs.h \
	$(INCDIR)/slog.h \
	$(INCDIR)/str_util.h \
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
	$(INCDIR)/mmsefun.h

#  object files
OBJECTS = \
	arg.o \
	fo_tfn.o \
	fo_data.o \
	fo_opt.o \
	fo_main.o \
	fo_write.o \
	fo_parse.o \
	fo_uca.o \
	mms_alta.o \
	mms_rtaa.o \
	mms_tdef.o \
	ms_tdef.o \
	ms_tdef2.o \
	ms_tdef3.o \
	mmslvar.o 

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
	echo foundry.mak: $< $(LIST)
	$(CC) $(CFLAGS) -o $*.o $< $(LIST)

clean:
	-rm $(OBJECTS)
	-rm $(EXE)
	-rm $(EXE).map
	-rm $(OBJDIR)/cc.lst


# Here is where we compile the C files into objects

fo_tfn.o:	fo_tfn.c $(INCLUDES)

fo_data.o:	fo_data.c $(INCLUDES)

fo_opt.o:	fo_opt.c $(INCLUDES)

fo_main.o:	fo_main.c $(INCLUDES)

fo_write.o:	fo_write.c $(INCLUDES)

fo_parse.o:	fo_parse.c $(INCLUDES)

fo_uca.o:	fo_uca.c $(INCLUDES)

arg.o:		arg.c $(INCLUDES)

mms_alta.o:	mms_alta.c $(INCLUDES)

mms_rtaa.o:	mms_rtaa.c $(INCLUDES)

mms_tdef.o:	mms_tdef.c $(INCLUDES)

ms_tdef.o:	ms_tdef.c $(INCLUDES)

ms_tdef2.o:	ms_tdef2.c $(INCLUDES)

ms_tdef3.o:	ms_tdef3.c $(INCLUDES)

mmslvar.o:	mmslvar.c $(INCLUDES)
