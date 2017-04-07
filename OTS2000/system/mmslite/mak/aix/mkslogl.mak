##########################################################################
# SISCO SOFTWARE MODULE HEADER ###########################################
##########################################################################
#			       						 #
# MODULE NAME : mkslogl.mak                       			 #
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
# 	make -f mkslogl.mak						 #
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
# 11/25/02   -     02     proAsset: HP-UX adaption, aCC compiler         #
# 11/25/02   -     01     proAsset: Created from WIN32 version		 #
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

PROJ  = slogl
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
OBJECTS = slogl.o

# Main target is the library
# If any objects have changed, delete and recreate it.
$(LIBRARY): $(OBJECTS)
	rm -f $(LIBRARY)
	ar -q $(LIBRARY)\
		slogl.o    \
		  $(LIST)
	rm *.o
	@echo "FINISHED CREATING $(LIBRARY) LIBRARY" $(LIST)
	@echo "-----------------------------------------------------" $(LIST)

.c.o:
	echo mkslogl.mak: $< $(LIST)
	$(CC) $(CFLAGS) -o $*.o $< $(LIST)

# do the compile

slogl.o:	slogl.c $(INCLUDES)

