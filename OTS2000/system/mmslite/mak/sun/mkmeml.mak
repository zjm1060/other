##########################################################################
# SISCO SOFTWARE MODULE HEADER ###########################################
##########################################################################
#			       						 #
# MODULE NAME : mkmeml.mak                       			 #
# PRODUCT(S)  : MMS-EASE-Lite						 #
#			       						 #
# MODULE DESCRIPTION :        						 #
# This makefile is used by the HP-UX 'make' utility to build the         #
# Memory allocation library for MMS-EASE-Lite.  All paths including      #
# header files and object modules are relative to the current project    #
# directory which makes this file as environment independent as  	 #
# possible.			                                         #
#								         #
# To make the MEML.LIB, enter the following:			         #
#								         #
# 	make -f mkmeml.mak					         #
#								         #
# Description of HP aCC compiler options used:			         #
#	  		       					         #
#  -c       Suppress the link phase and retain object files	         #
#  -v       Verbose mode                                                 #
#  -g       Produce symbolic info in the object file for debugger	 #
#  +Onoall  Generate non-optimized code sequences		         #
#  +w       Warning output                                               #
#				                                         #
# MODIFICATION LOG :	       						 #
#  Date     Who   Rev			Comments			 #
# --------  ---  ------   -------------------------------------------	 #
# 11/22/02   -     08     proAsset: HP-UX adaption, aCC compiler         #
# 08/06/01  RKR    07     Removed S_THISFILE				 #
# 07/26/01  JRB    06     Deleted whitespace chars.                      #
# 08/15/00  GLB    05     Added VPATH macro to work with new "make"      #
# 12/03/99  GLB    04     Print build file name on screen during build   #
# 09/24/99  JRB    03     Added:  .c.o:                                  #
# 10/26/98  JRB    02     Added -DMMS_LITE			         #
# 08/13/97  EJV    01     Created from DOS version			 #
##########################################################################

#set the compiler
#CC         = aCC
CC = cc

# set the library, object, and include file path names

LIBDIR = ../../lib
OBJDIR = ../../src
INCDIR = ../../inc
SRCDIR = ../../src

VPATH  = $(SRCDIR)

PROJ  = meml
OPT   =
DEFS  =
LIST  = >> cc.lst 2>&1

# set compiler flags
CFLAGS = -c -v +w +Onoall $(OPT) -I$(INCDIR) $(DEFS) -DMMS_LITE

LIBRARY = $(LIBDIR)/$(PROJ).a

.PRECIOUS: $(LIBRARY)

# setup the dependencies for the source files
INCLUDES = $(INCDIR)/glbtypes.h \
	$(INCDIR)/sysincs.h \
	$(INCDIR)/mem_chk.h \
	$(INCDIR)/memlog.h \
	$(INCDIR)/slog.h \
	$(INCDIR)/gen_list.h

# These are the objects that belong in the library
OBJECTS = mem_chkl.o

# Main target is the library
# If any objects have changed, delete and recreate it.
$(LIBRARY): $(OBJECTS)
	rm -f $(LIBRARY)
	ar -q $(LIBRARY)\
		mem_chkl.o  $(LIST)
	rm *.o
	@echo "FINISHED CREATING $(LIBRARY) LIBRARY" $(LIST)
	@echo "-----------------------------------------------------" $(LIST)

.c.o:
	echo $< $(LIST)
	$(CC) $(CFLAGS) -o $*.o $< $(LIST)

# do the compile
mem_chkl.o:	mem_chkl.c $(INCLUDES)

