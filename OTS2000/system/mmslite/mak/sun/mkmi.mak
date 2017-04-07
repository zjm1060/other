##########################################################################
# SISCO SOFTWARE MODULE HEADER ###########################################
##########################################################################
#			       						 #
# MODULE NAME : mkmi.mak                       				 #
# PRODUCT(S)  : MMS-EASE-Lite						 #
#			       						 #
# MODULE DESCRIPTION :        						 #
# This makefile is used by the HP-UX 'make' utility to build the MI	 #
# library for MMS-EASE-Lite.  All paths including header 		 #
# files and object modules are relative to the current project directory #
# which makes this file as environment independent as possible.  Both    #
# the DEBUG and Non DEBUG versions of the Utilities are made with this   #
# script								 #
#									 #
# To make the MEMD.LIB which shows calling information, enter the 	 #
# following:								 #
#									 #
# 	make -f mkmi.mak OPT=-g DEFS=-DDEBUG_SISCO PROJ=mid		 #
#									 #
# To make the MEM.LIB, enter the following:				 #
#									 #
# 	make -f mkmi.mak						 #
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
# 11/25/02   -     03     proAsset: HP-UX adaption, aCC compiler         #
# 06/11/02  GLB    02     Updated for ICCP-Lite			         #
# 02/15/02  RKR    01     Created from DOS version			 #
##########################################################################

#set the compiler
#CC         = aCC
CC = cc

# set the library, object, and include file path names

LIBDIR     = ../../lib
OBJDIR     = ../../src
INCDIR     = ../../inc
ICCPSRCDIR = ../../iccp/src
ICCPUSRDIR = ../../iccp/usr

VPATH  = $(ICCPSRCDIR):$(ICCPUSRDIR)

PROJ  = mi
OPT   =
DEFS  =
LIST  = >> cc.lst 2>&1

# set compiler flags
CFLAGS = -c -v +w +Onoall $(OPT) -I$(INCDIR) $(DEFS) -DMMS_LITE -DLEAN_T -DMOSI -DCOACSE -DICCP_LITE

LIBRARY = $(LIBDIR)/$(PROJ).a

.PRECIOUS: $(LIBRARY)

# setup the dependancies for the source files
INCLUDES = $(INCDIR)/glbtypes.h \
	$(INCDIR)/sysincs.h \
	$(INCDIR)/gen_list.h \
	$(INCDIR)/mem_chk.h \
	$(INCDIR)/mi.h \
	$(INCDIR)/mi_icfg.h \
	$(INCDIR)/mi_log.h \
	$(INCDIR)/icfg.h \
	$(INCDIR)/slog.h 

# These are the objects that belong in the library
OBJECTS = mi.o \
	icfg.o \
	icfgxml.o \
	mi_size.o \
	misem.o \
	mi_wrcfg.o \
	mi_icfg.o \
	mi_grobj.o \
	mic.o \
	micb4.o \
	mis.o \
	misb4.o \
	micb5.o \
	misb5.o 

# Main target is the library
# If any objects have changed, delete and recreate it.
$(LIBRARY): $(OBJECTS)
	rm -f $(LIBRARY)
	ar -q $(LIBRARY)\
		mi.o \
		icfg.o \
		icfgxml.o \
		mi_size.o \
		misem.o \
		mi_wrcfg.o \
		mi_icfg.o \
		mi_grobj.o \
		mic.o \
		micb4.o \
		mis.o \
		misb4.o \
		micb5.o \
		misb5.o $(LIST)
	rm *.o
	@echo "FINISHED CREATING $(LIBRARY) LIBRARY" $(LIST)
	@echo "-----------------------------------------------------" $(LIST)

.c.o:
	echo mkmi.mak: $< $(LIST)
	$(CC) $(CFLAGS) -o $*.o $< $(LIST)

# Make sure all objects are up to date
mi.o:	        mi.c $(INCLUDES)

cfg.o:	icfg.c $(INCLUDES)

icfgxml.o:	icfgxml.c $(INCLUDES)

mi_size.o:	mi_size.c $(INCLUDES)

misem.o:	misem.c $(INCLUDES)

mi_wrcfg.o:	mi_wrcfg.c $(INCLUDES)

mi_icfg.o:	mi_icfg.c $(INCLUDES)

mi_grobj.o:	mi_grobj.c $(INCLUDES)

mic.o:	mic.c $(INCLUDES)

micb4.o:	micb4.c $(INCLUDES)

mis.o:	mis.c $(INCLUDES)

misb4.o:	misb4.c $(INCLUDES)

micb5.o:	micb5.c $(INCLUDES)

misb5.o:	misb5.c $(INCLUDES)






