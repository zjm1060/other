##########################################################################
# SISCO SOFTWARE MODULE HEADER ###########################################
##########################################################################
#			       						 #
# MODULE NAME : sreadd.mak                       			 #
# PRODUCT(S)  : MMS-EASE-Lite						 #
#			       						 #
# MODULE DESCRIPTION :        						 #
# This makefile is used by the HP-UX 'make' utility to build the socket	 #
# "Read" Task/Thread for MMS-EASE-Lite. All paths including header	 #
# files and object modules are relative to the current project 	 	 #
# directory which makes this file as environment independent as 	 #
# possible.								 #
#									 #
# To make the SREADD executable, enter the following:			 #
#									 #
# 	make -f sreadd.mak						 #
#			       						 #
#  -c       Suppress the link phase and retain object files		 #
#  -v       Verbose mode                                                 #
#  -g       Produce symbolic info in the object file for debugger	 #
#  +Onoall  Generate non-optimized code sequences			 #
#  +w       Warning output				                 #
#			       						 #
# Description of QNX linker options used:				 #
#  -o outfile   	Output file path for executable file		 #
#  -m			Produce a symbolic map file (defults to .map ext)#
#			       						 #
# MODIFICATION LOG :	       						 #
#  Date     Who   Rev			Comments			 #
# --------  ---  ------   -------------------------------------------    #
# 11/25/02   -     09     proAsset: HP-UX adaption, aCC compiler         #
# 05/16/02  JRB    08     Del meml_n.lib (no longer needed).             #
# 12/18/01  GLB    07     Build with meml_n.lib instead of meml.lib      #
# 08/06/01  RKR    06     Removed S_THISFILE				 #
# 07/26/01  JRB    06     Deleted whitespace chars                       #
# 08/15/00  GLB    04     Added VPATH macro to work with new "make"      #
# 12/03/99  GLB    03     Print build file name on screen during build   #
# 09/24/99  JRB    02     Added:  .c.o:                                  #
# 08/18/97  EJV    01     Created					 #
##########################################################################

#set the compiler
#CC         = aCC
CC = cc

# set the library, object, and include file path names
LIBDIR = ../../lib
OBJDIR = .
INCDIR = ../../inc
SRCDIR = ../../uca/leant
EXEDIR = ../../bin

VPATH  = $(SRCDIR)

PROJ  = sreadd
OPT   =
DEFS  =
LIST  = >> cc.lst 2>&1

# set compiler flags and create debug flags
CFLAGS = -c -v +w +Onoall $(OPT) -I$(INCDIR) $(DEFS) -DMOSI -DLEAN_T -DTP0_ENABLED

LIBS =

EXE = $(EXEDIR)/$(PROJ)
LFLAGS = -lm -lsocket -lnsl -o$(EXE)

.PRECIOUS:	$(EXE)

# setup the dependencies for the source files
INCLUDES = $(INCDIR)/glbtypes.h \
	$(INCDIR)/sysincs.h \
	$(INCDIR)/stime.h \
	$(INCDIR)/slog.h \
	$(INCDIR)/mem_chk.h \
	$(INCDIR)/memlog.h \
	$(INCDIR)/gen_list.h \
	$(INCDIR)/stime.h \
	$(INCDIR)/lean_a.h \
	$(INCDIR)/acse2log.h \
	$(INCDIR)/asn1defs.h \
	$(INCDIR)/acse2usr.h \
	$(INCDIR)/acse2.h \
	$(INCDIR)/tp0_sock.h \
	$(INCDIR)/tp4.h \
	$(INCDIR)/tp4_encd.h \
	$(INCDIR)/tp4_log.h \
	$(INCDIR)/tp4api.h \
	$(INCDIR)/cfglog.h \
	$(INCDIR)/checksum.h \
	$(INCDIR)/cfg_util.h \
	$(INCDIR)/clnp_log.h \
	$(INCDIR)/clnp_usr.h

# object files
OBJECTS = tp0_read.o

# Main target is the executable
$(EXE): $(OBJECTS)
	rm -f $(EXE)
	$(CC) $(LFLAGS)\
		$(OBJECTS) \
		$(LIBS) $(LIST)
	@echo "FINISHED GENERATING $(EXE) EXECUTABLE" $(LIST)
	@echo "-----------------------------------------------------" $(LIST)
	rm $(OBJDIR)/*.o

.c.o:
	echo sreadd.mak: $< $(LIST)
	$(CC) $(CFLAGS) -o $*.o $< $(LIST)

tp0_read.o: 	tp0_read.c $(INCLUDES)

