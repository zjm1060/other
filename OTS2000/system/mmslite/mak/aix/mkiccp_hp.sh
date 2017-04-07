#************************************************************************/
#* SISCO SOFTWARE MODULE HEADER *****************************************/
#************************************************************************/
#*			       
#* MODULE NAME : mkiccp.sh                       
#* PRODUCT(S)  : MMS-EASE-Lite
#*			       
#* MODULE DESCRIPTION :        
#* This command file is used to the ICCP-Lite TCP example for ICCP-Lite. 
#* All paths including header files and object modules are relative 
#* to the current project directory which makes this file as environment 
#* independent as possible. 
#*
#* To build enter: 
#*
#*      chmod +x mkiccp.sh
#*      mkiccp.sh
#*
# ************************************************************************
# MODIFICATION LOG :	       						                     #
#  Date     Who   Rev			Comments			                     #
# --------  ---  ------   -------------------------------------------	 #
# 06/11/02  GLB    02     Updated
# 05/05/02         01     Created from mkall.sh for MMS-EASE Lite
# ************************************************************************

# ************************************************************************
#  MAKE ALL LIBRARIES 
# ************************************************************************

#LIST  = >> cc.lst 2>&1

rm cc.lst

#mkdir ../../lib
#mkdir ../../bin

echo STARTING MEML_L LIBRARY  >> cc.lst 2>&1
make -f mkmeml.mak	 DEFS=-DDEBUG_SISCO          PROJ=meml_l      # logging    / no debug
echo STARTING MEML_N LIBRARY  >> cc.lst 2>&1
make -f mkmeml.mak				     PROJ=meml_n      # no logging / no debug
echo STARTING MEML_LD LIBRARY  >> cc.lst 2>&1                 
make -f mkmeml.mak	 OPT=-g  DEFS=-DDEBUG_SISCO  PROJ=meml_ld     # logging    / debug
echo STARTING MEML_ND LIBRARY  >> cc.lst 2>&1
make -f mkmeml.mak	 OPT=-g                      PROJ=meml_nd     # no logging / debug

echo STARTING MEM_L LIBRARY  >> cc.lst 2>&1
make -f mkmem.mak	         DEFS=-DDEBUG_SISCO  PROJ=mem_l	      # logging    / no debug
echo STARTING MEM_N LIBRARY  >> cc.lst 2>&1
make -f mkmem.mak				     PROJ=mem_n       # no logging / no debug
echo STARTING MEM_LD LIBRARY  >> cc.lst 2>&1
make -f mkmem.mak        OPT=-g  DEFS=-DDEBUG_SISCO  PROJ=mem_ld      # debug      / logging 
echo STARTING MEM_ND LIBRARY  >> cc.lst 2>&1
make -f mkmem.mak        OPT=-g                      PROJ=mem_nd      # no logging / debug

echo STARTING SMEM_L LIBRARY  >> cc.lst 2>&1
make -f mksmem.mak	         DEFS=-DDEBUG_SISCO  PROJ=smem_l      # logging    / no debug
echo STARTING SMEM_N LIBRARY  >> cc.lst 2>&1
make -f mksmem.mak				     PROJ=smem_n      # no logging / no debug
echo STARTING SMEM_LD LIBRARY  >> cc.lst 2>&1
make -f mksmem.mak	 OPT=-g  DEFS=-DDEBUG_SISCO  PROJ=smem_ld     # logging    / debug
echo STARTING SMEM_ND LIBRARY  >> cc.lst 2>&1
make -f mksmem.mak       OPT=-g                      PROJ=smem_nd     # no logging / debug

echo STARTING SLOG_L LIBRARY  >> cc.lst 2>&1
make -f mkslog.mak		 DEFS=-DDEBUG_SISCO  PROJ=slog_l      # logging    / no debug
echo STARTING SLOG_LD LIBRARY  >> cc.lst 2>&1
make -f mkslog.mak	OPT=-g   DEFS=-DDEBUG_SISCO  PROJ=slog_ld     # logging    / debug

echo STARTING SLOGL_L LIBRARY  >> cc.lst 2>&1
make -f mkslogl.mak		 DEFS=-DDEBUG_SISCO  PROJ=slogl_l      # logging    / no debug
echo STARTING SLOGL_LD LIBRARY  >> cc.lst 2>&1
make -f mkslogl.mak	OPT=-g   DEFS=-DDEBUG_SISCO  PROJ=slogl_ld     # logging    / debug

echo STARTING ASN1_L LIBRARY  >> cc.lst 2>&1
make -f mkasn1l.mak		 DEFS=-DDEBUG_SISCO  PROJ=asn1l_l     # logging    / no debug
echo STARTING ASN1_N LIBRARY  >> cc.lst 2>&1
make -f mkasn1l.mak				     PROJ=asn1l_n     # no logging / no debug
echo STARTING ASN1_LD LIBRARY  >> cc.lst 2>&1
make -f mkasn1l.mak	 OPT=-g  DEFS=-DDEBUG_SISCO  PROJ=asn1l_ld    # logging    / debug
echo STARTING ASN1_ND LIBRARY  >> cc.lst 2>&1
make -f mkasn1l.mak	 OPT=-g                      PROJ=asn1l_nd    # no logging / debug

echo STARTING MMSL_L LIBRARY  >> cc.lst 2>&1
make -f mkmmsl.mak		 DEFS=-DDEBUG_SISCO  PROJ=mmsl_l      # logging    / no debug
echo STARTING MMSL_N LIBRARY  >> cc.lst 2>&1
make -f mkmmsl.mak                                   PROJ=mmsl_n      # no logging / no debug 
echo STARTING MMSL_LD LIBRARY  >> cc.lst 2>&1
make -f mkmmsl.mak	 OPT=-g  DEFS=-DDEBUG_SISCO  PROJ=mmsl_ld     # logging    / debug
echo STARTING MMSL_ND LIBRARY  >> cc.lst 2>&1
make -f mkmmsl.mak       OPT=-g                      PROJ=mmsl_nd     # no logging / debug

echo STARTING MMSLE_L LIBRARY  >> cc.lst 2>&1
make -f mkmmsle.mak	         DEFS=-DDEBUG_SISCO  PROJ=mmsle_l     # logging    / no debug
echo STARTING MMSLE_N LIBRARY  >> cc.lst 2>&1
make -f mkmmsle.mak				     PROJ=mmsle_n     # no logging / no debug
echo STARTING MMSLE_LD LIBRARY  >> cc.lst 2>&1
make -f mkmmsle.mak	 OPT=-g  DEFS=-DDEBUG_SISCO  PROJ=mmsle_ld    # logging    / debug
echo STARTING MMSLE_ND LIBRARY  >> cc.lst 2>&1
make -f mkmmsle.mak      OPT=-g                      PROJ=mmsle_nd    # no logging / debug

echo STARTING OSITCPE_L LIBRARY  >> cc.lst 2>&1
make -f ositcpe.mak	         DEFS=-DDEBUG_SISCO  PROJ=ositcpe_l   # logging    / no debug
echo STARTING OSITCPE_N LIBRARY  >> cc.lst 2>&1
make -f ositcpe.mak				     PROJ=ositcpe_n   # no logging / no debug
echo STARTING OSITCPE_LD LIBRARY  >> cc.lst 2>&1
make -f ositcpe.mak	 OPT=-g  DEFS=-DDEBUG_SISCO  PROJ=ositcpe_ld  # logging    / debug
echo STARTING OSITCPE_ND LIBRARY  >> cc.lst 2>&1
make -f ositcpe.mak      OPT=-g                      PROJ=ositcpe_nd  # no logging / debug

echo STARTING UTIL_L LIBRARY  >> cc.lst 2>&1
make -f mkutil.mak	         DEFS=-DDEBUG_SISCO  PROJ=util_l      # logging    / no debug
echo STARTING UTIL_N LIBRARY  >> cc.lst 2>&1
make -f mkutil.mak				     PROJ=util_n      # no logging / no debug
echo STARTING UTIL_LD LIBRARY  >> cc.lst 2>&1
make -f mkutil.mak	 OPT=-g  DEFS=-DDEBUG_SISCO  PROJ=util_ld     # logging    / debug
echo STARTING UTIL_ND LIBRARY  >> cc.lst 2>&1
make -f mkutil.mak       OPT=-g                      PROJ=util_nd     # no logging / debug

echo STARTING MLOGL_L LIBRARY  >> cc.lst 2>&1
make -f mkmlogl.mak	         DEFS=-DDEBUG_SISCO  PROJ=mlogl_l     # logging    / no debug
echo STARTING MLOGL_N LIBRARY  >> cc.lst 2>&1
make -f mkmlogl.mak				     PROJ=mlogl_n     # no logging / no debug
echo STARTING MLOGL_LD LIBRARY  >> cc.lst 2>&1
make -f mkmlogl.mak	 OPT=-g  DEFS=-DDEBUG_SISCO  PROJ=mlogl_ld    # logging    / debug
echo STARTING MLOGL_ND LIBRARY  >> cc.lst 2>&1
make -f mkmlogl.mak      OPT=-g                      PROJ=mlogl_nd    # no logging / debug

echo STARTING MI_L LIBRARY  >> cc.lst 2>&1
make -f mkmi.mak	         DEFS=-DDEBUG_SISCO  PROJ=mi_l        # logging    / no debug
echo STARTING MI_N LIBRARY  >> cc.lst 2>&1
make -f mkmi.mak				     PROJ=mi_n        # no logging / no debug
echo STARTING MI_LD LIBRARY  >> cc.lst 2>&1
make -f mkmi.mak	 OPT=-g  DEFS=-DDEBUG_SISCO  PROJ=mi_ld       # logging    / debug
echo STARTING MI_ND LIBRARY  >> cc.lst 2>&1
make -f mkmi.mak         OPT=-g                      PROJ=mi_nd       # no logging / debug


rm ../../lib/*.bak

# ************************************************************************
#  Make socket tasks  
# ************************************************************************

# Make socket LISTEN task
make -f slistend.mak
# Make socket READ task
make -f sreadd.mak


# Go back to main dir
cd ../..


# ************************************************************************
#  Make MVL Libraries  
# ************************************************************************

cd mvl/src
rm cc.lst

echo STARTING MVL_L LIBRARY  >> cc.lst 2>&1
make -f hpux/mkmvl.mak		 DEFS=-DDEBUG_SISCO  PROJ=mvl_l        # logging    / no debug 
echo STARTING MVL_N LIBRARY  >> cc.lst 2>&1
make -f hpux/mkmvl.mak				     PROJ=mvl_n        # no logging / no debug
echo STARTING MVL_LD LIBRARY  >> cc.lst 2>&1
make -f hpux/mkmvl.mak    OPT=-g  DEFS=-DDEBUG_SISCO  PROJ=mvl_ld       # logging    / debug
echo STARTING MVL_ND LIBRARY  >> cc.lst 2>&1
make -f hpux/mkmvl.mak    OPT=-g                      PROJ=mvl_nd       # no logging / debug

cat cc.lst >> ../../cmd/hpux/cc.lst

cd ../..


# ************************************************************************
#  MAKE THE UTILITIES 
# ************************************************************************


# ************************************************************************
#  FOUNDRY UTILITY  

cd mvl/util/foundry

rm cc.lst
echo STARTING FOUNDRY  >> cc.lst 2>&1
make -f hpux/foundry.mak clean  		     
make -f hpux/foundry.mak
cat cc.lst >> ../../../cmd/hpux/cc.lst
cd ../../..


# ************************************************************************
#  MMSLOG UTILITY  

cd mvl/util/mmslog

rm cc.lst
echo STARTING MMSLOG  >> cc.lst 2>&1
make -f hpux/mmslog.mak clean
make -f hpux/mmslog.mak
cat cc.lst >> ../../../cmd/hpux/cc.lst
cd ../../..


# ************************************************************************
#  MICSTCPE UTILITY  

cd iccp/usr

rm cc.lst
echo STARTING MICSTCPE_L   >> cc.lst 2>&1
make -f hpux/micstcpe.mak	 DEFS=-DDEBUG_SISCO  PROJ=micstcpe_l    # logging    / no debug 
echo STARTING MICSTCPE_N   >> cc.lst 2>&1
make -f hpux/micstcpe.mak		             PROJ=micstcpe_n    # no logging / no debug
echo STARTING MICSTCPE_LD  >> cc.lst 2>&1
make -f hpux/micstcpe.mak OPT=-g  DEFS=-DDEBUG_SISCO  PROJ=micstcpe_ld   # logging    / debug
echo STARTING MICSTCPE_ND  >> cc.lst 2>&1
make -f hpux/micstcpe.mak OPT=-g                      PROJ=micstcpe_nd   # no logging / debug
cat cc.lst >> ../../cmd/hpux/cc.lst
cd ../..


# ************************************************************************
#  MAKE MITEST UTILITY 

cd iccp/mitest

rm cc.lst
echo STARTING MITEST  >> cc.lst 2>&1
make -f hpux/mitest.mak  OPT=-g                      PROJ=mitestd
echo STARTING MITEST  >> cc.lst 2>&1
make -f hpux/mitest.mak                              PROJ=mitest
cat cc.lst >> ../../cmd/hpux/cc.lst
cd ../..

cd cmd/hpux

