##########################################################################
# SISCO SOFTWARE MODULE HEADER ###########################################
##########################################################################
#			       						 #
# MODULE NAME : mkmmsle.mak                       			 #
# PRODUCT(S)  : MMS-EASE-Lite						 #
#			       						 #
# MODULE DESCRIPTION :        						 #
# This makefile is used by the HP-UX 'make' utility to build the 	 #
# MMS-EASE-Lite exensions library.   All paths including header 	 #
# files and object modules are relative to the current project directory #
# which makes this file as environment independent as possible.  Both    #
# the DEBUG and Non DEBUG versions of the Utilities are made with this   #
# script.  								 #
#									 #
# To make the MMSLED.LIB, enter the following:				 #
#									 #
# 	make -f mkmmsle.mak OPT=-g DEFS=-DDEBUG_SISCO PROJ=mmsled 	 #
#									 #
# To make the MMSLE.LIB, enter the following:				 #
#									 #
# 	make -f mkmmsle.mak						 #
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
# 11/25/02   -     08     proAsset: HP-UX adaption, aCC compiler         #
# 08/06/01  RKR    07     Removed S_THISFILE				 #
# 07/26/01  JRB    06     Deleted whitespace chars                       #
# 08/15/00  GLB    05     Added VPATH macro to work with ne "make"       #
# 12/03/99  GLB    04     Print build file name on screen during build   #
# 10/15/99  GLB    03     Added file Rename and Obtain File              #
# 09/24/99  JRB    02     added:  .c.o:                                  #
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

PROJ  = mmsle
OPT   =
DEFS  =
LIST  = >> cc.lst 2>&1

# set compiler flags and create debug flags
CFLAGS = -c -v +w +Onoall $(OPT) -I$(INCDIR) $(DEFS) -DMMS_LITE

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
OBJECTS = mms_ced1.o \
	mms_ced2.o \
	rq_altec.o \
	rq_altee.o \
	rq_crepi.o \
	rq_defea.o \
	rq_defec.o \
	rq_defsc.o \
	rq_defse.o \
	rq_defty.o \
	rq_defva.o \
	rq_defvl.o \
	rq_deldo.o \
	rq_delea.o \
	rq_delec.o \
	rq_delee.o \
	rq_delpi.o \
	rq_delse.o \
	rq_delty.o \
	rq_delva.o \
	rq_delvl.o \
	rq_downl.o \
	rq_fclos.o \
	rq_fdele.o \
	rq_frena.o \
	rq_fdir.o  \
	rq_fopen.o \
	rq_fread.o \
	rq_getae.o \
	rq_getas.o \
	rq_getsc.o \
	rq_initd.o \
	rq_initu.o \
	rq_input.o \
	rq_jcrea.o \
	rq_jdele.o \
	rq_jinit.o \
	rq_jread.o \
	rq_jstat.o \
	rq_jwrit.o \
	rq_kill.o  \
	rq_loadd.o \
	rq_obtfi.o \
	rq_outpu.o \
	rq_rddwn.o \
	rq_rdupl.o \
	rq_relct.o \
	rq_renam.o \
	rq_reset.o \
	rq_resum.o \
	rq_rsent.o \
	rq_rspoo.o \
	rq_rssta.o \
	rq_start.o \
	rq_stop.o  \
	rq_store.o \
	rq_takec.o \
	rq_termd.o \
	rq_termu.o \
	rq_uploa.o \
	rs_altec.o \
	rs_altee.o \
	rs_crepi.o \
	rs_defea.o \
	rs_defec.o \
	rs_defsc.o \
	rs_defse.o \
	rs_defty.o \
	rs_defva.o \
	rs_defvl.o \
	rs_deldo.o \
	rs_delea.o \
	rs_delec.o \
	rs_delee.o \
	rs_delpi.o \
	rs_delse.o \
	rs_delty.o \
	rs_delva.o \
	rs_delvl.o \
	rs_downl.o \
	rs_fclos.o \
	rs_fdele.o \
	rs_frena.o \
	rs_fdir.o  \
	rs_fopen.o \
	rs_fread.o \
	rs_getae.o \
	rs_getas.o \
	rs_getsc.o \
	rs_initd.o \
	rs_initu.o \
	rs_input.o \
	rs_jcrea.o \
	rs_jdele.o \
	rs_jinit.o \
	rs_jread.o \
	rs_jstat.o \
	rs_jwrit.o \
	rs_kill.o  \
	rs_loadd.o \
	rs_obtfi.o \
	rs_outpu.o \
	rs_rddwn.o \
	rs_rdupl.o \
	rs_relct.o \
	rs_renam.o \
	rs_reset.o \
	rs_resum.o \
	rs_rsent.o \
	rs_rspoo.o \
	rs_rssta.o \
	rs_start.o \
	rs_stop.o  \
	rs_store.o \
	rs_takec.o \
	rs_termd.o \
	rs_termu.o \
	rs_uploa.o

# Main target is the library
# If any objects have changed, delete and recreate it.
# NOTE: The lib commands are broken up due to a problem with very long
# parameter list for lib. There is no other reason for this grouping.
$(LIBRARY): $(OBJECTS)
	rm -f $(LIBRARY)
	ar -q $(LIBRARY)\
		mms_ced1.o \
		mms_ced2.o \
		rq_altec.o \
		rq_altee.o \
		rq_crepi.o \
		rq_defea.o \
		rq_defec.o \
		rq_defsc.o \
		rq_defse.o \
		rq_defty.o \
		rq_defva.o \
		rq_defvl.o \
		rq_deldo.o \
		rq_delea.o \
		rq_delec.o \
		rq_delee.o \
		rq_delpi.o \
		rq_delse.o \
		rq_delty.o \
		rq_delva.o \
		rq_delvl.o \
		rq_downl.o \
		rq_fclos.o \
		rq_fdele.o \
		rq_frena.o \
		rq_fdir.o \
		rq_fopen.o \
		rq_fread.o \
		rq_getae.o \
		rq_getas.o \
		rq_getsc.o \
		rq_initd.o \
		rq_initu.o \
		rq_input.o \
		rq_jcrea.o \
		rq_jdele.o \
		rq_jinit.o \
		rq_jread.o \
		rq_jstat.o \
		rq_jwrit.o \
		rq_kill.o \
		rq_loadd.o \
		rq_obtfi.o \
		rq_outpu.o \
		rq_rddwn.o \
		rq_rdupl.o \
		rq_relct.o \
		rq_renam.o \
		rq_reset.o \
		rq_resum.o \
		rq_rsent.o \
		rq_rspoo.o \
		rq_rssta.o \
		rq_start.o \
		rq_stop.o \
		rq_store.o \
		rq_takec.o \
		rq_termd.o \
		rq_termu.o \
		rq_uploa.o \
		rs_altec.o \
		rs_altee.o \
		rs_crepi.o \
		rs_defea.o \
		rs_defec.o \
		rs_defsc.o \
		rs_defse.o \
		rs_defty.o \
		rs_defva.o \
		rs_defvl.o \
		rs_deldo.o \
		rs_delea.o \
		rs_delec.o \
		rs_delee.o \
		rs_delpi.o \
		rs_delse.o \
		rs_delty.o \
		rs_delva.o \
		rs_delvl.o \
		rs_downl.o \
		rs_fclos.o \
		rs_fdele.o \
		rs_fdir.o \
		rs_fopen.o \
		rs_fread.o \
		rs_frena.o \
		rs_getae.o \
		rs_getas.o \
		rs_getsc.o \
		rs_initd.o \
		rs_initu.o \
		rs_input.o \
		rs_jcrea.o \
		rs_jdele.o \
		rs_jinit.o \
		rs_jread.o \
		rs_jstat.o \
		rs_jwrit.o \
		rs_kill.o \
		rs_loadd.o \
		rs_obtfi.o \
		rs_outpu.o \
		rs_rddwn.o \
		rs_rdupl.o \
		rs_relct.o \
		rs_renam.o \
		rs_reset.o \
		rs_resum.o \
		rs_rsent.o \
		rs_rspoo.o \
		rs_rssta.o \
		rs_start.o \
		rs_stop.o \
		rs_store.o \
		rs_takec.o \
		rs_termd.o \
		rs_termu.o \
		rs_uploa.o $(LIST)
	rm *.o
	@echo "FINISHED CREATING $(LIBRARY) LIBRARY" $(LIST)
	@echo "-----------------------------------------------------" $(LIST)

.c.o:
	echo mkmmsle.mak: $< $(LIST)
	$(CC) $(CFLAGS) -o $*.o $< $(LIST)

# do the compile
mms_ced1.o:	mms_ced1.c $(INCLUDES)

mms_ced2.o:	mms_ced2.c $(INCLUDES)

rq_altec.o:	rq_altec.c $(INCLUDES)

rq_altee.o:	rq_altee.c $(INCLUDES)

rq_crepi.o:	rq_crepi.c $(INCLUDES)

rq_defea.o:	rq_defea.c $(INCLUDES)

rq_defec.o:	rq_defec.c $(INCLUDES)

rq_defsc.o:	rq_defsc.c $(INCLUDES)

rq_defse.o:	rq_defse.c $(INCLUDES)

rq_defty.o:	rq_defty.c $(INCLUDES)

rq_defva.o:	rq_defva.c $(INCLUDES)

rq_defvl.o:	rq_defvl.c $(INCLUDES)

rq_deldo.o:	rq_deldo.c $(INCLUDES)

rq_delea.o:	rq_delea.c $(INCLUDES)

rq_delec.o:	rq_delec.c $(INCLUDES)

rq_delee.o:	rq_delee.c $(INCLUDES)

rq_delpi.o:	rq_delpi.c $(INCLUDES)

rq_delse.o:	rq_delse.c $(INCLUDES)

rq_delty.o:	rq_delty.c $(INCLUDES)

rq_delva.o:	rq_delva.c $(INCLUDES)

rq_delvl.o:	rq_delvl.c $(INCLUDES)

rq_downl.o:	rq_downl.c $(INCLUDES)

rq_fclos.o:	rq_fclos.c $(INCLUDES)

rq_fdele.o:	rq_fdele.c $(INCLUDES)

rq_fdir.o:	rq_fdir.c $(INCLUDES)

rq_fopen.o:	rq_fopen.c $(INCLUDES)

rq_fread.o:	rq_fread.c $(INCLUDES)

rq_frena.o:	rq_frena.c $(INCLUDES)

rq_getae.o:	rq_getae.c $(INCLUDES)

rq_getas.o:	rq_getas.c $(INCLUDES)

rq_getsc.o:	rq_getsc.c $(INCLUDES)

rq_initd.o:	rq_initd.c $(INCLUDES)

rq_initu.o:	rq_initu.c $(INCLUDES)

rq_input.o:	rq_input.c $(INCLUDES)

rq_jcrea.o:	rq_jcrea.c $(INCLUDES)

rq_jdele.o:	rq_jdele.c $(INCLUDES)

rq_jinit.o:	rq_jinit.c $(INCLUDES)

rq_jread.o:	rq_jread.c $(INCLUDES)

rq_jstat.o:	rq_jstat.c $(INCLUDES)

rq_jwrit.o:	rq_jwrit.c $(INCLUDES)

rq_kill.o:	rq_kill.c $(INCLUDES)

rq_loadd.o:	rq_loadd.c $(INCLUDES)

rq_obtfi.o:	rq_obtfi.c $(INCLUDES)

rq_outpu.o:	rq_outpu.c $(INCLUDES)

rq_rddwn.o:	rq_rddwn.c $(INCLUDES)

rq_rdupl.o:	rq_rdupl.c $(INCLUDES)

rq_relct.o:	rq_relct.c $(INCLUDES)

rq_renam.o:	rq_renam.c $(INCLUDES)

rq_reset.o:	rq_reset.c $(INCLUDES)

rq_resum.o:	rq_resum.c $(INCLUDES)

rq_rsent.o:	rq_rsent.c $(INCLUDES)

rq_rspoo.o:	rq_rspoo.c $(INCLUDES)

rq_rssta.o:	rq_rssta.c $(INCLUDES)

rq_start.o:	rq_start.c $(INCLUDES)

rq_stop.o:	rq_stop.c $(INCLUDES)

rq_store.o:	rq_store.c $(INCLUDES)

rq_takec.o:	rq_takec.c $(INCLUDES)

rq_termd.o:	rq_termd.c $(INCLUDES)

rq_termu.o:	rq_termu.c $(INCLUDES)

rq_uploa.o:	rq_uploa.c $(INCLUDES)

rs_altec.o:	rs_altec.c $(INCLUDES)

rs_altee.o:	rs_altee.c $(INCLUDES)

rs_crepi.o:	rs_crepi.c $(INCLUDES)

rs_defea.o:	rs_defea.c $(INCLUDES)

rs_defec.o:	rs_defec.c $(INCLUDES)

rs_defsc.o:	rs_defsc.c $(INCLUDES)

rs_defse.o:	rs_defse.c $(INCLUDES)

rs_defty.o:	rs_defty.c $(INCLUDES)

rs_defva.o:	rs_defva.c $(INCLUDES)

rs_defvl.o:	rs_defvl.c $(INCLUDES)

rs_deldo.o:	rs_deldo.c $(INCLUDES)

rs_delea.o:	rs_delea.c $(INCLUDES)

rs_delec.o:	rs_delec.c $(INCLUDES)

rs_delee.o:	rs_delee.c $(INCLUDES)

rs_delpi.o:	rs_delpi.c $(INCLUDES)

rs_delse.o:	rs_delse.c $(INCLUDES)

rs_delty.o:	rs_delty.c $(INCLUDES)

rs_delva.o:	rs_delva.c $(INCLUDES)

rs_delvl.o:	rs_delvl.c $(INCLUDES)

rs_downl.o:	rs_downl.c $(INCLUDES)

rs_fclos.o:	rs_fclos.c $(INCLUDES)

rs_fdele.o:	rs_fdele.c $(INCLUDES)

rs_fdir.o:	rs_fdir.c $(INCLUDES)

rs_fopen.o:	rs_fopen.c $(INCLUDES)

rs_fread.o:	rs_fread.c $(INCLUDES)

rs_frena.o:	rs_frena.c $(INCLUDES)

rs_getae.o:	rs_getae.c $(INCLUDES)

rs_getas.o:	rs_getas.c $(INCLUDES)

rs_getsc.o:	rs_getsc.c $(INCLUDES)

rs_initd.o:	rs_initd.c $(INCLUDES)

rs_initu.o:	rs_initu.c $(INCLUDES)

rs_input.o:	rs_input.c $(INCLUDES)

rs_jcrea.o:	rs_jcrea.c $(INCLUDES)

rs_jdele.o:	rs_jdele.c $(INCLUDES)

rs_jinit.o:	rs_jinit.c $(INCLUDES)

rs_jread.o:	rs_jread.c $(INCLUDES)

rs_jstat.o:	rs_jstat.c $(INCLUDES)

rs_jwrit.o:	rs_jwrit.c $(INCLUDES)

rs_kill.o:	rs_kill.c $(INCLUDES)

rs_loadd.o:	rs_loadd.c $(INCLUDES)

rs_obtfi.o:	rs_obtfi.c $(INCLUDES)

rs_outpu.o:	rs_outpu.c $(INCLUDES)

rs_rddwn.o:	rs_rddwn.c $(INCLUDES)

rs_rdupl.o:	rs_rdupl.c $(INCLUDES)

rs_relct.o:	rs_relct.c $(INCLUDES)

rs_renam.o:	rs_renam.c $(INCLUDES)

rs_reset.o:	rs_reset.c $(INCLUDES)

rs_resum.o:	rs_resum.c $(INCLUDES)

rs_rsent.o:	rs_rsent.c $(INCLUDES)

rs_rspoo.o:	rs_rspoo.c $(INCLUDES)

rs_rssta.o:	rs_rssta.c $(INCLUDES)

rs_start.o:	rs_start.c $(INCLUDES)

rs_stop.o:	rs_stop.c $(INCLUDES)

rs_store.o:	rs_store.c $(INCLUDES)

rs_takec.o:	rs_takec.c $(INCLUDES)

rs_termd.o:	rs_termd.c $(INCLUDES)

rs_termu.o:	rs_termu.c $(INCLUDES)

rs_uploa.o:	rs_uploa.c $(INCLUDES)


