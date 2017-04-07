##########################################################################
# SISCO SOFTWARE MODULE HEADER ###########################################
##########################################################################
#			       						 #
# MODULE NAME : mkutil.mak                       			 #
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
# To make the UTILD.LIB enter the following: 				 #
#									 #
# 	make -f mkutil.mak OPT=-g DEFS=-DDEBUG_SISCO PROJ=utild		 #
#									 #
# To make the UTIL.LIB, enter the following:				 #
#									 #
# 	make -f mkutil.mak						 #
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
# 11/25/02   -     10     proAsset: HP-UX adaption, aCC compiler         #
# 08/06/01  RKR    09     Removed S_THISFILE				 #
# 07/26/01  JRB    08     Deleted whitespace chars                       #
#			  Put back in DOS format                         #
# 03/28/01  MDE    07     Added sx_axs4d.c                               #
# 11/26/00  GLB    06     Added str_util.c                               #
# 08/15/00  GLB    05     Added VPATH macro to work with new "make"      #
# 12/03/99  GLB    04     Print build file name on screen during build   #
# 09/24/99  JRB    03     Added:  .c.o:                                  #
# 10/26/98  JRB    02     Added -DMMS_LITE				 #
# 08/14/97  EJV    01     Created from DOS version			 #
##########################################################################

#set the compiler
#CC         = aCC
CC = cc

# set the library, object, and include file path names
LIBDIR = ../../lib
OBJDIR = .
INCDIR = ../../inc
SRCDIR = ../../src

VPATH  = $(SRCDIR)

PROJ  = util
OPT   =
DEFS  =
LIST  = >> cc.lst 2>&1

# set compiler flags
CFLAGS = -c -v +w +Onoall $(OPT) -I$(INCDIR) $(DEFS) -DMMS_LITE

LIBRARY = $(LIBDIR)/$(PROJ).a

.PRECIOUS: $(LIBRARY)

# setup the dependancies for the source files
INCLUDES = $(INCDIR)/glbtypes.h \
	$(INCDIR)/gen_list.h \
	$(INCDIR)/sysincs.h \
	$(INCDIR)/mem_chk.h \
	$(INCDIR)/cfg_util.h \
	$(INCDIR)/stime.h \
	$(INCDIR)/slog.h \
	$(INCDIR)/cfglog.h \
	$(INCDIR)/sx_log.h \
	$(INCDIR)/sx_defs.h \
	$(INCDIR)/str_util.h \
	$(INCDIR)/time_str.h

# These are the objects that belong in the library
OBJECTS = genlists.o \
	cfg_util.o \
	glbsem.o \
	stime.o \
	str_util.o \
	sx_dec.o \
	sx_enc.o \
	sx_data.o \
	sx_axs4d.o \
	time_str.o

# Main target is the library
# If any objects have changed, delete and recreate it.
$(LIBRARY): $(OBJECTS)
	rm -f $(LIBRARY)
	ar -q $(LIBRARY)\
		genlists.o \
		cfg_util.o \
		glbsem.o \
		stime.o \
		str_util.o \
		sx_dec.o \
		sx_enc.o \
		sx_data.o \
		sx_axs4d.o \
		time_str.o $(LIST)
	rm *.o
	@echo "FINISHED CREATING $(LIBRARY) LIBRARY" $(LIST)
	@echo "-----------------------------------------------------" $(LIST)

.c.o:
	echo mkutil.mak: $< $(LIST)
	$(CC) $(CFLAGS) -o $*.o $< $(LIST)

# do the compile
genlists.o:	genlists.c $(INCLUDES)

cfg_util.o:	cfg_util.c $(INCLUDES)

stime.o:	stime.c $(INCLUDES)

str_util.o:   str_util.c $(INCLUDES)

glbsem.o:	glbsem.c $(INCLUDES)

sx_dec.o:	sx_dec.c $(INCLUDES)

sx_enc.o:	sx_enc.c $(INCLUDES)

sx_data.o:	sx_data.c $(INCLUDES)

sx_axs4d.o:	sx_axs4d.c $(INCLUDES)

time_str.o:	time_str.c $(INCLUDES)




