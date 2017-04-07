#************************************************************************/
#* SISCO SOFTWARE MODULE HEADER *****************************************/
#************************************************************************/
#*			       
#* MODULE NAME : mbufcalc.mak                       
#* PRODUCT(S)  : MMS-EASE-Lite
#*			       
#* MODULE DESCRIPTION :        
#*	This makefile is used by the HP-UX 'make' utility to build the
#* utility to calculate the mms decode buffer size for MMS-EASE-Lite.
#* All paths including header 
#* files and object modules are relative to the current project directory 
#* which makes this file as environment independent as possible.  
#* Usage instructions are given below as well as a brief description of 
#* each switch used in the process.  
#*
#* To make the MBUFCALC executable, enter the following:
#*
#* 	make -f mbufcalc.mak
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
#* 11/25/02   -     06     proAsset: HP-UX adaption, aCC compiler
#* 06/25/02  GLB    05     Change "exe" directory to "bin" directory
#* 12/20/01  GLB    04     Use logging/no debug (_l) version of libraries
#* 08/07/01  RKR    03     Removed S_THISFILE
#* 11/23/99  GLB    02     Added .c.o:
#* 08/18/97  EJV    01     Created from DOS version
#************************************************************************/

#set the compiler
#CC         = aCC
CC         = cc

LIBDIR = ../../../lib
OBJDIR = .
INCDIR = ../../../inc
MMSOPDIR = ../../../inc/mmsop_en
EXEDIR = ../../../bin

PROJ  = mbufcalc
OPT   =
DEFS  =
LIST  = >> cc.lst 2>&1

# set compiler flags
CFLAGS = -c -v +w +Onoall $(OPT) -I$(INCDIR) $(DEFS) -DMMS_LITE \
	 -I$(MMSOPDIR) -DDEBUG_SISCO

#LIBS = $(LIBDIR)/mmsl_ld.a $(LIBDIR)/asn1l_ld.a $(LIBDIR)/mem_ld.a \
#       $(LIBDIR)/mvl_ld.a $(LIBDIR)/slog_ld.a  $(LIBDIR)/mi_ld.a \
#       $(LIBDIR)/util_ld.a

LIBS = $(LIBDIR)/mmsl_ld.a $(LIBDIR)/mmsle_ld.a $(LIBDIR)/asn1l_ld.a \
       $(LIBDIR)/mem_ld.a $(LIBDIR)/mlogl_ld.a \
       $(LIBDIR)/slog_ld.a $(LIBDIR)/mmsl_ld.a $(LIBDIR)/asn1l_ld.a $(LIBDIR)/util_ld.a
     
EXE = $(EXEDIR)/$(PROJ)
LFLAGS = -v -lm -o $(EXE) 

.PRECIOUS:	$(EXE)

# setup the dependencies for the source files
INCLUDES = $(INCDIR)/glbtypes.h \
	$(INCDIR)/sysincs.h \
	$(INCDIR)/mmsdefs.h \
	$(INCDIR)/glberror.h \
#	$(INCDIR)/gen_list.h \
#	$(INCDIR)/mem_chk.h \
	$(INCDIR)/mms_pvar.h \
#	$(INCDIR)/mms_mp.h \
	$(INCDIR)/asn1defs.h \
#	$(INCDIR)/slog.h \
#	$(INCDIR)/mms_vvar.h \
#	$(INCDIR)/mms_err.h \
#	$(INCDIR)/mms_def2.h \
#	$(INCDIR)/mmsintr2.h \
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

#  object files
OBJECTS =  $(OBJDIR)/mbufcalc.o

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
	echo mbufcalc.mak: $< $(LIST)
	$(CC) $(CFLAGS) -o $*.o $< $(LIST)

clean:
	-rm $(OBJECTS)
	-rm $(EXE)
	-rm $(EXE).map
	-rm $(OBJDIR)/cc.lst


# Here is where we compile the C files into objects

$(OBJDIR)/mbufcalc.o:	mbufcalc.c $(INCLUDES)


