##########################################################################
# SISCO SOFTWARE MODULE HEADER ###########################################
##########################################################################
#			       						 #
# MODULE NAME : mkasn1l.mak                       			 #
# PRODUCT(S)  : MMS-EASE-Lite						 #
#			       						 #
# MODULE DESCRIPTION :        						 #
# This makefile is used by the HP-UX 'make' utility to build the ASN1    #
# Lite library for MMS-EASE-Lite.  All paths including header 		 #
# files and object modules are relative to the current project directory #
# which makes this file as environment independent as possible.  Both 	 #
# the DEBUG and Non DEBUG versions of the Utilities are made with this 	 #
# script				                                 #
#       		        	                                 #
# To make the ASN1LD.LIB, enter the following:   	                 #
#									 #
# 	make -f mkasn1l.mak OPT=-g DEFS=-DDEBUG_SISCO PROJ=asn1ld	 #
#									 #
# To make the ASN1L.LIB, enter the following:				 #
#									 #
# 	make -f mkasn1l.mak					 	 #
#			       						 #
# Description of HP aCC compiler options used:				 #
#			       						 #
#  -c       Suppress the link phase and retain object files		 #
#  -v       Verbose mode                                                 #
#  -g       Produce symbolic info in the object file for debugger        #
#  +Onoall  Generate non-optimized code sequences			 #
#  +w       Warning output						 #
#			       						 #
# MODIFICATION LOG :	       						 #
#  Date     Who   Rev			Comments			 #
# --------  ---  ------   -------------------------------------------	 #
# 11/21/02   -     10     proAsset: HP-UX adaption, aCC compiler         #
# 01/18/02  GLB    09     Changed "ad_" to "ard_" & "ae_" to "are_"      #  
# 12/03/01  GLB    08     Added ad_utc.c, ae_utc.c, ard_utc.c & are_utc.c#
# 08/06/01  RKR    07     Removed S_THISFILE				 #
# 07/26/01  JRB    06     Deleted whitespace chars.                      #
# 08/15/00  GLB    05     Added VPATH macro to work with new "make"      #
# 12/03/99  GLB    04     Print make file name on screen during build    #
# 09/24/99  JRB    03     Added:  .c.o:                                  #
# 08/27/97  EJV    02     Added ae_btod.c and ad_btod.c			 #
# 08/12/97  EJV    01     Created from DOS version			 #
##########################################################################

#set the compiler
#CC         = aCC
CC = gcc

# set the library, object, and include file path names

LIBDIR = ../../lib
OBJDIR = ../../src
INCDIR = ../../inc
SRCDIR = ../../src

VPATH  = $(SRCDIR)

PROJ  = asn1l
OPT   =
DEFS  =
LIST  = >> cc.lst 2>&1

# set compiler flags
CFLAGS = -g -c $(OPT) -I$(INCDIR) $(DEFS) -DMMS_LITE 

LIBRARY = $(LIBDIR)/$(PROJ).a

.PRECIOUS: $(LIBRARY)

# setup the dependencies for the source files
INCLUDES = $(INCDIR)/glbtypes.h \
	$(INCDIR)/sysincs.h \
	$(INCDIR)/asn1defs.h \
	$(INCDIR)/slog.h \
	$(INCDIR)/asn1log.h

# These are the objects that belong in the library
OBJECTS = asn1r.o \
	ard_btod.o  \
	ard_bool.o  \
	ard_bstr.o  \
	ard_delm.o  \
	ard_flt.o   \
	ard_idnt.o  \
	ard_int.o   \
	ard_objd.o  \
	ard_ostr.o  \
	ard_strn.o  \
	ard_time.o  \
	ard_utc.o   \
	ard_vstr.o  \
	are_btod.o  \
	are_bool.o  \
	are_bstr.o  \
	are_delm.o  \
	are_flt.o   \
	are_int.o   \
	are_objd.o  \
	are_ostr.o  \
	are_time.o  \
	are_utc.o   \
	are_vstr.o

# Main target is the library
# If any objects have changed, delete and recreate it.
$(LIBRARY): $(OBJECTS)
	rm -f $(LIBRARY) 
	ar -q $(LIBRARY)\
		asn1r.o     \
		ard_btod.o  \
		ard_bool.o  \
		ard_bstr.o  \
		ard_delm.o  \
		ard_flt.o   \
		ard_idnt.o  \
		ard_int.o   \
		ard_objd.o  \
		ard_ostr.o  \
		ard_strn.o  \
		ard_time.o  \
		ard_vstr.o  \
		ard_utc.o   \
		are_btod.o  \
		are_bool.o  \
		are_bstr.o  \
		are_delm.o  \
		are_flt.o   \
		are_int.o   \
		are_objd.o  \
		are_ostr.o  \
		are_time.o  \
		are_utc.o   \
		are_vstr.o  $(LIST)
	rm *.o
	@echo "FINISHED CREATING $(LIBRARY) LIBRARY" $(LIST)
	@echo "-----------------------------------------------------" $(LIST)
.c.o:
	echo mkasn1l.mak: $< $(LIST)
	$(CC) $(CFLAGS) -o $*.o $< $(LIST)


# do the compile
asn1r.o:	asn1r.c $(INCLUDES)

ard_btod.o:	ard_btod.c $(INCLUDES)

ard_bool.o:	ard_bool.c $(INCLUDES)

ard_bstr.o:	ard_bstr.c $(INCLUDES)

ard_delm.o:	ard_delm.c $(INCLUDES)

ard_flt.o:	ard_flt.c $(INCLUDES)

ard_idnt.o:	ard_idnt.c $(INCLUDES)

ard_int.o:	ard_int.c $(INCLUDES)

ard_objd.o:	ard_objd.c $(INCLUDES)

ard_ostr.o:	ard_ostr.c $(INCLUDES)

ard_strn.o:	ard_strn.c $(INCLUDES)

ard_time.o:	ard_time.c $(INCLUDES)

ard_utc.o:	ard_utc.c $(INCLUDES)

ard_vstr.o:   ard_vstr.c $(INCLUDES)

are_btod.o:	are_btod.c $(INCLUDES)

are_bool.o:	are_bool.c $(INCLUDES)

are_bstr.o:	are_bstr.c $(INCLUDES)

are_delm.o:	are_delm.c $(INCLUDES)

are_flt.o:	are_flt.c $(INCLUDES)

are_int.o:	are_int.c $(INCLUDES)

are_objd.o:	are_objd.c $(INCLUDES)

are_ostr.o:	are_ostr.c $(INCLUDES)

are_time.o:	are_time.c $(INCLUDES)

are_utc.o:	are_utc.c $(INCLUDES)

are_vstr.o:   are_vstr.c $(INCLUDES)

