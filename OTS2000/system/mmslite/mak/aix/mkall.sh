# ************************************************************************
# MODIFICATION LOG :	       						                     #
#  Date     Who   Rev			Comments			                     #
# --------  ---  ------   -------------------------------------------	 #
# 10/26/00  JRB    01     Delete conversion instructions
#			  Change qmake back to make
#			  Add commands to build mvlu.lib and mvlud.lib
# ************************************************************************

# ****************** MAKE ALL LIBRARIES ******************
rm cc.lst
mkdir ../../lib
mkdir ../../bin

make -f mkmeml.mak
make -f mkmem.mak
make -f mkmem.mak    OPT=-g DEFS=-DDEBUG_SISCO PROJ=memd
make -f mkslog.mak
make -f mkasn1l.mak
make -f mkasn1l.mak  OPT=-g DEFS=-DDEBUG_SISCO PROJ=asn1ld
make -f mkmmsl.mak
make -f mkmmsl.mak   OPT=-g DEFS=-DDEBUG_SISCO PROJ=mmsld
make -f mkmmsle.mak
make -f mkmmsle.mak  OPT=-g DEFS=-DDEBUG_SISCO PROJ=mmsled
make -f ositcpe.mak
make -f ositcpe.mak  OPT=-g DEFS=-DDEBUG_SISCO PROJ=ositcped
make -f mkutil.mak
make -f mkutil.mak   OPT=-g DEFS=-DDEBUG_SISCO PROJ=utild
make -f mkmlogl.mak
make -f mkmlogl.mak   OPT=-g DEFS=-DDEBUG_SISCO PROJ=mlogld
make -f mkmi.mak
make -f mkmi.mak  OPT=-g DEFS=-DDEBUG_SISCO PROJ=mid
make -f mksmem.mak

rm ../../lib/*.bak


# Make socket LISTEN task
make -f slistend.mak
# Make socket READ task
make -f sreadd.mak

# Go back to main dir
cd ../..

# Make MVL Libraries, both debug and non-debug
cd mvl/src
rm cc.lst
make -f aix/mkmvl.mak
make -f aix/mkmvl.mak OPT=-g DEFS=-DDEBUG_SISCO              PROJ=mvld
make -f aix/mkmvl.mak        DEFS=-DMVL_UCA                  PROJ=mvlu
make -f aix/mkmvl.mak OPT=-g "DEFS=-DMVL_UCA -DDEBUG_SISCO"  PROJ=mvlud
cat cc.lst >> ../../mak/aix/cc.lst
cd ../..

# Make utilities

cd mvl/util/mbufcalc
rm cc.lst
make -f aix/mbufcalc.mak
cat cc.lst >> ../../../mak/aix/cc.lst
cd ../../..

cd mvl/util/foundry
rm cc.lst
make -f aix/foundry.mak
cat cc.lst >> ../../../mak/aix/cc.lst
cd ../../..

cd mvl/util/mmslog
rm cc.lst
make -f aix/mmslog.mak
cat cc.lst >> ../../../mak/aix/cc.lst
cd ../../..

# Make MVL sample Server
cd mvl/usr/server
rm cc.lst
make -f hpux/server.mak
cat cc.lst >> ../../../cmd/hpux/cc.lst
cd ../../..

# Make MVL sample client
cd mvl/usr/client
rm cc.lst
make -f hpux/client.mak
cat cc.lst >> ../../../cmd/hpux/cc.lst
cd ../../..

# Make UCA Server
cd mvl/usr/uca_srvr
rm cc.lst
make -f hpux/uca_srvr.mak
cat cc.lst >> ../../../cmd/hpux/cc.lst
cd ../../..

# Make MITEST
cd iccp/mitest/aix
rm cc.lst
make -f mitest.mak
cat cc.lst >> ../../../mak/aix/cc.lst
cd ../../..

cd mak/aix
