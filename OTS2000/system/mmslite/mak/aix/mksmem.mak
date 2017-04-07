##########################################################################
# SISCO SOFTWARE MODULE HEADER ###########################################
##########################################################################
#			       						 #
# MODULE NAME : mksmem.mak                       			 #
# PRODUCT(S)  : MMS-EASE-Lite						 #
#			       						 #
# MODULE DESCRIPTION :        						 #
# This makefile is used by the HP-UX 'make' utility to build the Memory	 #
# allocation library for MMS-EASE-Lite.  All paths including header 	 #
# files and object modules are relative to the current project directory #
# which makes this file as environment independent as possible.  Both    #
# the DEBUG and Non DEBUG versions of the Utilities are made with this   #
# script								 #
#									 #
# To make the 'smemd.lib' which shows calling information, enter the 	 #
# following:								 #
#									 #
# 	make -f mksmem.mak OPT=-g DEFS=-DDEBUG_SISCO PROJ=smemd		 #
#									 #
# To make the 'smem.lib', enter the following:				 #
#									 #
# 	make -f mksmem.mak						 #
#			       						 #
# Description of HP aCC compiler options used:				 #
#			       						 #
#  -c       Suppress the link phase and retain object files		 #
#  -v       Verbose mode                                                 #
#  -g       Produce symbolic info in the object file for debugger	 #
#  +Onoall  Generate non-optimized code sequences			 #
#  +w       Warning output				                 #
#			       						 #
# MODIFICATION LOG :	       						 #
#  Date     Who   Rev			Comments			 #
# --------  ---  ------   -------------------------------------------    #
# 11/25/02   -     02     proAsset: HP-UX adaption, aCC compiler         #
# 10/01/01  JRB    01     Created from mkmem.mak			 #
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

PROJ  = smem
OPT   =
DEFS  =
LIST  = >> cc.lst 2>&1

# set compiler flags
CFLAGS = -g -c $(OPT) -I$(INCDIR) $(DEFS) -DMMS_LITE -DSMEM_ENABLE

LIBRARY = $(LIBDIR)/$(PROJ).a

.PRECIOUS: $(LIBRARY)

# setup the dependancies for the source files
INCLUDES = $(INCDIR)/glbtypes.h \
	$(INCDIR)/sysincs.h \
	$(INCDIR)/mem_chk.h \
	$(INCDIR)/smem.h \
	$(INCDIR)/memlog.h \
	$(INCDIR)/slog.h \
	$(INCDIR)/gen_list.h

# These are the objects that belong in the library
OBJECTS = \
	mem_smem.o	\
	smem.o	\
	smemcfgx.o

# Main target is the library
# If any objects have changed, delete and recreate it.
$(LIBRARY): $(OBJECTS)
	rm -f $(LIBRARY)
	ar -q $(LIBRARY)\
		mem_smem.o	\
		smem.o	\
		smemcfgx.o	\
		$(LIST)
	rm *.o
	@echo "FINISHED CREATING $(LIBRARY) LIBRARY" $(LIST)
	@echo "-----------------------------------------------------" $(LIST)

.c.o:
	echo mksmem.mak: $< $(LIST)
	$(CC) $(CFLAGS) -o $*.o $< $(LIST)

# Make sure all objects are up to date
mem_smem.o:	mem_smem.c $(INCLUDES)

smem.o:	smem.c $(INCLUDES)

smemcfgx.o:	smemcfgx.c $(INCLUDES)
