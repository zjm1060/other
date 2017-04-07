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

make -f mkmeml.mak	 OPT=-g  DEFS=-DDEBUG_SISCO  PROJ=meml_ld     # logging    / debug
make -f mkmem.mak        OPT=-g  DEFS=-DDEBUG_SISCO  PROJ=mem_ld      # debug      / logging 
make -f mksmem.mak	 OPT=-g  DEFS=-DDEBUG_SISCO  PROJ=smem_ld     # logging    / debug
make -f mkslog.mak	OPT=-g   DEFS=-DDEBUG_SISCO  PROJ=slog_ld     # logging    / debug
make -f mkslogl.mak	OPT=-g   DEFS=-DDEBUG_SISCO  PROJ=slogl_ld     # logging    / debug
make -f mkasn1l.mak	 OPT=-g  DEFS=-DDEBUG_SISCO  PROJ=asn1l_ld    # logging    / debug
make -f mkmmsl.mak	 OPT=-g  DEFS=-DDEBUG_SISCO  PROJ=mmsl_ld     # logging    / debug
make -f mkmmsle.mak	 OPT=-g  DEFS=-DDEBUG_SISCO  PROJ=mmsle_ld    # logging    / debug
make -f ositcpe.mak	 OPT=-g  DEFS=-DDEBUG_SISCO  PROJ=ositcpe_ld  # logging    / debug
make -f mkutil.mak	 OPT=-g  DEFS=-DDEBUG_SISCO  PROJ=util_ld     # logging    / debug
make -f mkmlogl.mak	 OPT=-g  DEFS=-DDEBUG_SISCO  PROJ=mlogl_ld    # logging    / debug
make -f mkmi.mak	 OPT=-g  DEFS=-DDEBUG_SISCO  PROJ=mi_ld       # logging    / debug
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

make -f aix/mkmvl.mak    OPT=-g  DEFS=-DDEBUG_SISCO  PROJ=mvl_ld       # logging    / debug
cat cc.lst >> ../../../mak/aix/cc.lst

cd ../..


# ************************************************************************
#  MAKE THE UTILITIES 
# ************************************************************************


# ************************************************************************
#  FOUNDRY UTILITY  

cd mvl/util/foundry

rm cc.lst
make -f aix/foundry.mak clean  		     
make -f aix/foundry.mak
cat cc.lst >> ../../../mak/aix/cc.lst
cd ../../..


# ************************************************************************
#  MMSLOG UTILITY  

cd mvl/util/mmslog

rm cc.lst
make -f aix/mmslog.mak clean
make -f aix/mmslog.mak
cat cc.lst >> ../../../mak/aix/cc.lst
cd ../../..


# ************************************************************************
#  MICSTCPE UTILITY  

cd iccp/usr

rm cc.lst
make -f aix/micstcpe.mak OPT=-g  DEFS=-DDEBUG_SISCO  PROJ=micstcpe_ld   # logging    / debug
cat cc.lst >> ../../mak/aix/cc.lst
cd ../..


# ************************************************************************
#  MAKE MITEST UTILITY 

cd iccp/mitest

rm cc.lst
make -f aix/mitest.mak                              PROJ=mitest
cat cc.lst >> ../../mak/aix/cc.lst
cd ../..

cd mak/aix

