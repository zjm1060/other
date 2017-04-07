##########################################################################
# SISCO SOFTWARE MODULE HEADER ###########################################
##########################################################################
#			       						 #
# MODULE NAME : mkmmsl.mak                       			 #
# PRODUCT(S)  : MMS-EASE-Lite						 #
#			       						 #
# MODULE DESCRIPTION :        						 #
# This makefile is used by the HP-UX 'make' utility to build the core	 #
# Lite library for MMS-EASE-Lite.  All paths including header 		 #
# files and object modules are relative to the current project directory #
# which makes this file as environment independent as possible.  Both 	 #
# the DEBUG and Non DEBUG versions of the Utilities are made with this 	 #
# script.  								 #
#									 #
# To make the MMSLD.LIB, enter the following:				 #
#									 #
# 	make -f mkmmsl.mak OPT=-g DEFS=-DDEBUG_SISCO PROJ=mmsld	 	 #
#									 #
# To make the MMSL.LIB, enter the following:				 #
#									 #
# 	make -f mkmmsl.mak						 #
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
# 11/25/02   -     03     proAsset: HP-UX adaption, aCC compiler         #
# 02/05/02  GLB    08     Added mms_rtaa.c                               #
# 08/06/01  RKR    07     Removed S_THISFILE				 #
# 07/26/01  JRB    06     Deleted whitespace chars.                      #
# 09/07/00  JRB    05     Added ms_size.c, ms_tdef.c			 #
# 08/15/00  GLB    04     Added VPATH macro to work with new make        #
# 01/13/00  JRB    03     Added mms_tdef.c				 #
# 11/07/99  GLB    02     Added .c.o:                                    #
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

PROJ  = mmsl
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
	$(INCDIR)/mmsefun.h

# These are the objects that belong in the library
OBJECTS = mmscancl.o \
	mmsconcl.o \
	mmsdata.o \
	mmsdatat.o \
	mmsdec.o \
	mmsinit.o \
	mmsl_fin.o \
	mmslvar.o \
	mmsstat.o \
	mms_adl.o \
	mms_alta.o \
	mms_aref.o \
	mms_ced.o \
	mms_ced3.o \
	mms_ced4.o \
	mms_ced5.o \
	mms_ced6.o \
	mms_ced7.o \
	mms_err.o \
	mms_err2.o \
	mms_err3.o \
	mms_err4.o \
	mms_rej.o \
	mms_rtaa.o \
	mms_tdef.o \
	msdataas.o \
	ms_ext.o \
	ms_size.o \
	ms_tdef.o \
	ms_tdef2.o \
	rq_ackev.o \
	rq_cancl.o \
	rq_concl.o \
	rq_defee.o \
	rq_evnot.o \
	rq_getcl.o \
	rq_getdo.o \
	rq_getea.o \
	rq_getec.o \
	rq_getee.o \
	rq_getpi.o \
	rq_getty.o \
	rq_getva.o \
	rq_getvl.o \
	rq_ident.o \
	rq_info.o \
	rq_init.o \
	rq_namel.o \
	rq_read.o \
	rq_repea.o \
	rq_repec.o \
	rq_repee.o \
	rq_stat.o \
	rq_trige.o \
	rq_ustat.o \
	rq_write.o \
	rs_ackev.o \
	rs_cancl.o \
	rs_concl.o \
	rs_defee.o \
	rs_evnot.o \
	rs_getcl.o \
	rs_getdo.o \
	rs_getea.o \
	rs_getec.o \
	rs_getee.o \
	rs_getpi.o \
	rs_getty.o \
	rs_getva.o \
	rs_getvl.o \
	rs_ident.o \
	rs_info.o \
	rs_init.o \
	rs_namel.o \
	rs_read.o \
	rs_repea.o \
	rs_repec.o \
	rs_repee.o \
	rs_stat.o \
	rs_trige.o \
	rs_ustat.o \
	rs_write.o

# Main target is the library
# If any objects have changed, delete and recreate it.
# NOTE: The lib commands are broken up due to a problem with very long
# parameter list for lib. There is no other reason for this grouping.
$(LIBRARY): $(OBJECTS)
	rm -f $(LIBRARY)
	ar -q $(LIBRARY) \
		mmscancl.o \
		mmsconcl.o \
		mmsdata.o \
		mmsdatat.o \
		mmsdec.o \
		mmsinit.o \
		mmsl_fin.o \
		mmslvar.o \
		mmsstat.o \
		mms_adl.o \
		mms_alta.o \
		mms_aref.o \
		mms_ced.o \
		mms_ced3.o \
		mms_ced4.o \
		mms_ced5.o \
		mms_ced6.o \
		mms_ced7.o \
		mms_err.o \
		mms_err2.o \
		mms_err3.o  \
		mms_err4.o \
		mms_rej.o \
		mms_rtaa.o \
		mms_tdef.o \
		msdataas.o \
		ms_ext.o \
		ms_size.o \
		ms_tdef.o \
		ms_tdef2.o \
		rq_ackev.o \
		rq_cancl.o \
		rq_concl.o \
		rq_defee.o \
		rq_evnot.o \
		rq_getcl.o \
		rq_getdo.o \
		rq_getea.o \
		rq_getec.o \
		rq_getee.o \
		rq_getpi.o \
		rq_getty.o \
		rq_getva.o  \
		rq_getvl.o \
		rq_ident.o \
		rq_info.o \
		rq_init.o \
		rq_namel.o \
		rq_read.o \
		rq_repea.o \
		rq_repec.o \
		rq_repee.o \
		rq_stat.o   \
		rq_trige.o \
		rq_ustat.o \
		rq_write.o \
		rs_ackev.o \
		rs_cancl.o \
		rs_concl.o \
		rs_defee.o \
		rs_evnot.o \
		rs_getcl.o \
		rs_getdo.o  \
		rs_getea.o \
		rs_getec.o \
		rs_getee.o \
		rs_getpi.o \
		rs_getty.o \
		rs_getva.o \
		rs_getvl.o \
		rs_ident.o \
		rs_info.o \
		rs_init.o  \
		rs_namel.o \
		rs_read.o \
		rs_repea.o \
		rs_repec.o \
		rs_repee.o \
		rs_stat.o \
		rs_trige.o \
		rs_ustat.o \
		rs_write.o  $(LIST)
	rm *.o
	@echo "FINISHED CREATING $(LIBRARY) LIBRARY" $(LIST)
	@echo "-----------------------------------------------------" $(LIST)

.c.o:
	echo mkmmsl.mak: $< $(LIST)
	$(CC) $(CFLAGS) -o $*.o $< $(LIST)

# do the compile
mmscancl.o:	mmscancl.c $(INCLUDES)

mmsconcl.o:	mmsconcl.c $(INCLUDES)

mmsdata.o:	mmsdata.c $(INCLUDES)

mmsdatat.o:	mmsdatat.c $(INCLUDES)

mmsdec.o:	mmsdec.c $(INCLUDES)

mmsinit.o:	mmsinit.c $(INCLUDES)

mmsl_fin.o:	mmsl_fin.c $(INCLUDES)

mmslvar.o:	mmslvar.c $(INCLUDES)

mmsstat.o:	mmsstat.c $(INCLUDES)

mms_adl.o:	mms_adl.c $(INCLUDES)

mms_alta.o:	mms_alta.c $(INCLUDES)

mms_aref.o:	mms_aref.c $(INCLUDES)

mms_ced.o:	mms_ced.c $(INCLUDES)

mms_ced3.o:	mms_ced3.c $(INCLUDES)

mms_ced4.o:	mms_ced4.c $(INCLUDES)

mms_ced5.o:	mms_ced5.c $(INCLUDES)

mms_ced6.o:	mms_ced6.c $(INCLUDES)

mms_ced7.o:	mms_ced7.c $(INCLUDES)

mms_err.o:	mms_err.c $(INCLUDES)

mms_err2.o:	mms_err2.c $(INCLUDES)

mms_err3.o:	mms_err3.c $(INCLUDES)

mms_err4.o:	mms_err4.c $(INCLUDES)

mms_rej.o:	mms_rej.c $(INCLUDES)

mms_rtaa.o:	mms_rtaa.c $(INCLUDES)

mms_tdef.o:	mms_tdef.c $(INCLUDES)

msdataas.o:	msdataas.c $(INCLUDES)

ms_ext.o:	ms_ext.c $(INCLUDES)

ms_size.o:	ms_size.c $(INCLUDES)

ms_tdef.o:	ms_tdef.c $(INCLUDES)

ms_tdef2.o:	ms_tdef2.c $(INCLUDES)

rq_ackev.o:	rq_ackev.c $(INCLUDES)

rq_cancl.o:	rq_cancl.c $(INCLUDES)

rq_concl.o:	rq_concl.c $(INCLUDES)

rq_defee.o:	rq_defee.c $(INCLUDES)

rq_evnot.o:	rq_evnot.c $(INCLUDES)

rq_getcl.o:	rq_getcl.c $(INCLUDES)

rq_getdo.o:	rq_getdo.c $(INCLUDES)

rq_getea.o:	rq_getea.c $(INCLUDES)

rq_getec.o:	rq_getec.c $(INCLUDES)

rq_getee.o:	rq_getee.c $(INCLUDES)

rq_getpi.o:	rq_getpi.c $(INCLUDES)

rq_getty.o:	rq_getty.c $(INCLUDES)

rq_getva.o:	rq_getva.c $(INCLUDES)

rq_getvl.o:	rq_getvl.c $(INCLUDES)

rq_ident.o:	rq_ident.c $(INCLUDES)

rq_info.o:	rq_info.c $(INCLUDES)

rq_init.o:	rq_init.c $(INCLUDES)

rq_namel.o:	rq_namel.c $(INCLUDES)

rq_read.o:	rq_read.c $(INCLUDES)

rq_repea.o:	rq_repea.c $(INCLUDES)

rq_repec.o:	rq_repec.c $(INCLUDES)

rq_repee.o:	rq_repee.c $(INCLUDES)

rq_stat.o:	rq_stat.c $(INCLUDES)

rq_trige.o:	rq_trige.c $(INCLUDES)

rq_ustat.o:	rq_ustat.c $(INCLUDES)

rq_write.o:	rq_write.c $(INCLUDES)

rs_ackev.o:	rs_ackev.c $(INCLUDES)

rs_cancl.o:	rs_cancl.c $(INCLUDES)

rs_concl.o:	rs_concl.c $(INCLUDES)

rs_defee.o:	rs_defee.c $(INCLUDES)

rs_evnot.o:	rs_evnot.c $(INCLUDES)

rs_getcl.o:	rs_getcl.c $(INCLUDES)

rs_getdo.o:	rs_getdo.c $(INCLUDES)

rs_getea.o:	rs_getea.c $(INCLUDES)

rs_getec.o:	rs_getec.c $(INCLUDES)

rs_getee.o:	rs_getee.c $(INCLUDES)

rs_getpi.o:	rs_getpi.c $(INCLUDES)

rs_getty.o:	rs_getty.c $(INCLUDES)

rs_getva.o:	rs_getva.c $(INCLUDES)

rs_getvl.o:	rs_getvl.c $(INCLUDES)

rs_ident.o:	rs_ident.c $(INCLUDES)

rs_info.o:	rs_info.c $(INCLUDES)

rs_init.o:	rs_init.c $(INCLUDES)

rs_namel.o:	rs_namel.c $(INCLUDES)

rs_read.o:	rs_read.c $(INCLUDES)

rs_repea.o:	rs_repea.c $(INCLUDES)

rs_repec.o:	rs_repec.c $(INCLUDES)

rs_repee.o:	rs_repee.c $(INCLUDES)

rs_stat.o:	rs_stat.c $(INCLUDES)

rs_trige.o:	rs_trige.c $(INCLUDES)

rs_ustat.o:	rs_ustat.c $(INCLUDES)

rs_write.o:	rs_write.c $(INCLUDES)

