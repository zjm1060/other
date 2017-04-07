##########################################################################
# SISCO SOFTWARE MODULE HEADER ###########################################
##########################################################################
#			       						 #
# MODULE NAME : mkslog.mak                       			 #
# PRODUCT(S)  : MMS-EASE-Lite						 #
#			       						 #
# MODULE DESCRIPTION :        						 #
# This makefile is used by the HP-UX 'make' utility to build the SLOG	 #
# library for MMS-EASE-Lite.  All paths including header 		 #
# files and object modules are relative to the current project directory #
# which makes this file as environment independent as possible.  	 #
#									 #
# To make the SLOG.LIB, enter the following:				 #
#									 #
# 	make -f mkslog.mak						 #
#			       						 #
# Description of HP aCC compiler options used:				 #
#			       						 #
#  -c       Suppress the link phase and retain object files		 #
#  -v       Verbose mode                                                 #
#  -g       Produce symbolic info in the object file for debugger	 #
#  +Onoall  Generate non-optimized code sequences			 #
#  +w       Warning output				                 #
#									 #
# MODIFICATION LOG :	       						 #
#  Date     Who   Rev			Comments			 #
# --------  ---  ------   -------------------------------------------	 #
# 11/25/02   -     06     proAsset: HP-UX adaption, aCC compiler         #
# 08/06/01  RKR    05     Removed S_THISFILE				 #
# 08/15/00  GLB    04     Added VPATH macro to work with new "make"      #
# 12/03/99  GLB    03     Print build file name on screen during build   #
# 09/24/99  JRB    02     Added:  .c.o:                                  #
# 08/13/97  EJV    01     Created from DOS version			 #
##########################################################################

#set the compiler
#CC         = aCC
CC = gcc

# set the library, object, and include file path names
LIBDIR = ../../lib
OBJDIR = .
INCDIR = ../../inc
SRCDIR = ../../src

VPATH  = $(SRCDIR)

PROJ  = slog
OPT   =
DEFS  =
LIST  = >> cc.lst 2>&1

# set compiler flags
CFLAGS = -g -c $(OPT) -I$(INCDIR) $(DEFS) -DMMS_LITE

LIBRARY = $(LIBDIR)/$(PROJ).a

.PRECIOUS: $(LIBRARY)

# setup the dependancies for the source files
INCLUDES = $(INCDIR)/glbtypes.h \
	$(INCDIR)/sysincs.h \
	$(INCDIR)/mem_chk.h \
	$(INCDIR)/slog.h \
	$(INCDIR)/stime.h \
	$(INCDIR)/qmem.h

# These are the objects that belong in the library
OBJECTS = sloghex.o \
	slog.o    \
	slogfil.o \
	slogmem.o \
	qmem.o

# Main target is the library
# If any objects have changed, delete and recreate it.
$(LIBRARY): $(OBJECTS)
	rm -f $(LIBRARY)
	ar -q $(LIBRARY)\
		sloghex.o \
		slog.o    \
		slogfil.o \
		slogmem.o \
		qmem.o $(LIST)
	rm *.o
	@echo "FINISHED CREATING $(LIBRARY) LIBRARY" $(LIST)
	@echo "-----------------------------------------------------" $(LIST)

.c.o:
	echo mkslog.mak: $< $(LIST)
	$(CC) $(CFLAGS) -o $*.o $< $(LIST)

# do the compile
sloghex.o:	sloghex.c $(INCLUDES)

slog.o:		slog.c $(INCLUDES)

slogfil.o:	slogfil.c $(INCLUDES)

slogmem.o:	slogmem.c $(INCLUDES)

qmem.o:		qmem.c $(INCLUDES)

