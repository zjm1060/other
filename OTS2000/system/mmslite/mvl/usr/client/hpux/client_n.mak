#************************************************************************/
#* SISCO SOFTWARE MODULE HEADER *****************************************/
#************************************************************************/
#*			       
#* MODULE NAME : client_n.mak                       
#* PRODUCT(S)  : MMS-EASE-Lite
#*			       
#* MODULE DESCRIPTION :        
#* This makefile is used by the HP-UX 'make' utility to build the Client
#* example for MMS-EASE-Lite. All paths including header 
#* files and object modules are relative to the current project directory 
#* which makes this file as environment independent as possible.  
#* Usage instructions are given below as well as a brief description of 
#* each switch used in the process.  
#*			       
#* To make the COSITCPE_N executable (no logging/no debug) enter the following:
#*
#* 	make -f client_n.mak
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
#* 11/25/02   -            proAsset: HP-UX adaption, aCC compiler
#* 03/13/02  JRB           Chg FOUNDYDIR. Add ALIGNDIR.
#* 02/14/02  GLB           Removed ml_inits.c
#* 02/05/02  GLB           Remove reference to slog library and
#*                           logcfgx.c and log_cfg.c
#* 02/05/02  GLB           Delete MVL_NUM_CALLING
#* 12/21/01  GLB           Split from server.mak to build 
#*                           no logging/no debug version only
#* 12/06/01  GLB           Removed mbufcalc.cfg & mbufcalc.c (ml_init.c)
#*                           added ml_inits.c
#* 08/07/01  RKR           Removed S_THISFILE				 
#* 11/08/00  GLB           Added logcfgx.c to parse logcfg.xml
#* 12/03/99  GLB           Print build file name during build
#* 11/15/99  GLB           Added .c.o:                           
#* 08/19/97  EJV           Created from DOS version
#************************************************************************/

#set the compiler
CC         = aCC

#************************************************************************/
# Define directories here

LIBDIR = ../../../lib
OBJDIR = .
INCDIR = ../../../inc
SRCDIR = ../../../src
MMSOPDIR = .
MMSSRCDIR = ../../../src
MVLSRCDIR = ../../src
FOUNDRYDIR = ../../../exe
ALIGNDIR = ../../util/foundry/qnx
USRSRCDIR = ../../usr
ACSESRCDIR = ../../src/acse
ECLDIR = /ecl
EXEDIR = .

VPATH  = $(MVLSRCDIR):$(USRSRCDIR):$(ACSESRCDIR):$(SRCDIR)

PROJ  = cositcpe_n    # renamed so not confused with "client" directory 
                    # searched because of the VPATH macro
OPT   =
DEFS  =
LIST  = >> cc_n.lst 2>&1

#************************************************************************/
# Define compiler/linker switches to use. These depend on the target ...

CFLAGS = -c -v +w +Onoall $(OPT) -I$(INCDIR) $(DEFS) -DMMS_LITE \
	     -DMOSI -DLEAN_T -I$(MMSOPDIR) \
         -DETHERNET

LIBS = $(LIBDIR)/mvl_n.lib $(LIBDIR)/mmsl_n.lib $(LIBDIR)/mmsle_n.lib \
       $(LIBDIR)/asn1l_n.lib $(LIBDIR)/mem_n.lib \
       $(LIBDIR)/util_n.lib $(LIBDIR)/ositcpe_n.lib 

EXE = $(EXEDIR)/$(PROJ)
LFLAGS = -v -m -o $(EXE) 

#************************************************************************/
# Define includes dependancies

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
	$(MMSOPDIR)/mmsop_en.h \
	clntobj.h 

#************************************************************************/
# Define objects used in the exectable

OBJECTS = $(OBJDIR)/client.o \
	$(OBJDIR)/mmsop_en.o \
	$(OBJDIR)/mvlop_en.o \
	$(OBJDIR)/mvl_acse.o \
	$(OBJDIR)/event.o \
	$(OBJDIR)/clntobj.o 

#************************************************************************/
# The main target is the execcutable

$(EXE):	$(OBJECTS) $(LIBS)
	rm -f $(EXE)
	$(CC) $(LFLAGS)\
		$(OBJECTS) \
		$(LIBS) $(LIST)
	@echo "FINISHED GENERATING $(EXE) EXECUTABLE" $(LIST)
	@echo "-----------------------------------------------------" $(LIST)
	rm $(OBJDIR)/*.o

# clntobj.c (and clntobj.h) are created using the foundry utility.
clntobj.c: clntobj.odf 
	$(FOUNDRYDIR)/foundry -o -c$(ALIGNDIR)/align.cfg clntobj.odf clntobj.c $(LIST)

clntobj.h: clntobj.odf 
	$(FOUNDRYDIR)/foundry -o -c$(ALIGNDIR)/align.cfg clntobj.odf clntobj.h $(LIST)

#************************************************************************/
.c.o:
	echo client_n.mak: $< $(LIST)
	$(CC) $(CFLAGS) -o $*.o $< $(LIST)

# Here is where we compile the C files into objects

$(OBJDIR)/client.o:	client.c $(INCLUDES)

$(OBJDIR)/mmsop_en.o:	$(MMSSRCDIR)/mmsop_en.c $(INCLUDES)
	echo client_n.mak: $(MMSSRCDIR)/mmsop_en.c $(LIST)
	$(CC) $(CFLAGS) $(MMSSRCDIR)/mmsop_en.c $(LIST)

$(OBJDIR)/mvlop_en.o:	$(MVLSRCDIR)/mvlop_en.c $(INCLUDES)

$(OBJDIR)/mvl_acse.o:	$(ACSESRCDIR)/mvl_acse.c $(INCLUDES)

$(OBJDIR)/event.o:	    $(USRSRCDIR)/event.c $(INCLUDES)

$(OBJDIR)/clntobj.o:	clntobj.c $(INCLUDES)

