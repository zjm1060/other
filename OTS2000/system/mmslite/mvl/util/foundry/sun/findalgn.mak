#************************************************************************/
#* SISCO SOFTWARE MODULE HEADER *****************************************/
#************************************************************************/
#*			       
#* MODULE NAME : findalgn.mak                       
#* PRODUCT(S)  : MMS-EASE-Lite
#*			       
#* MODULE DESCRIPTION :        
#* This makefile is used by the HP-UX 'make' utility to build the Find
#* Alignment Utility for MMS-EASE-Lite.  All paths including header 
#* files and object modules are relative to the current project directory 
#* which makes this file as environment independent as possible.  
#* Usage instructions are given below as well as a brief description of 
#* each switch used in the process.  
#*
#* To make the findalgn.exe, enter the following:
#*
#* 	make -f hpux/findalgn.mak
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
#*  -m			Produce a symbolic map file (defaults to .map ext)
#*
#* MODIFICATION LOG:
#*  Date     Who   Rev			Comments
#* --------  ---  ------   -------------------------------------------
#* 11/25/02   -     03     proAsset: HP-UX adaption, aCC compiler
#* 08/07/01  RKR    02     Removed S_THISFILE
#* 08/19/97  EJV    01     Created from DOS version
#************************************************************************/

#set the compiler
CC         = aCC

SRCDIR = .
LIBDIR = ../../../lib
OBJDIR = .
INCDIR = ../../../inc
EXEDIR = ../../../bin

VPATH  = ..

PROJ  = findalgn
OPT   =
DEFS  =
LIST  = >> cc.lst 2>&1

# set compiler flags
CFLAGS = -c -v +w +Onoall +p $(OPT) -I$(INCDIR) $(DEFS)

LIBS = $(LIBDIR)/slogl_l.a $(LIBDIR)/mem_l.a $(LIBDIR)/meml_l.a $(LIBDIR)/util_l.a

EXE = $(EXEDIR)/$(PROJ)
LFLAGS = -v +m -o $(EXE) 

.PRECIOUS:	$(EXE)

# setup the dependencies for the source files
INCLUDES = $(INCDIR)/glbtypes.h \
	$(INCDIR)/sysincs.h

#  object files
OBJECTS =  findalgn.o

# Main target is the executable
$(EXE):	$(OBJECTS) $(LIBS)
	rm -f $(EXE)
	$(CC) $(LFLAGS)\
		$(OBJECTS) \
		$(LIBS) $(LIST)
	@echo "FINISHED GENERATING $(EXE) EXECUTABLE" $(LIST)
	@echo "-----------------------------------------------------" $(LIST)

findalgn.o:	findalgn.c 
	$(CC) $(CFLAGS) -o findalgn.o findalgn.c $(LIST)

