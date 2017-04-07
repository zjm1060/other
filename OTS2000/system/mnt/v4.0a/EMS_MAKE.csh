


# ""
# ""
# "***************************** start EMS_MAKE *******************************"
# "*                                                                          *"
# "* EMS_MAKE will compile files in all EMS directories and sub-directories.  *"
# "*                                                                          *"
# "*              By ciwa 2006.3                                              *"
# "*                                                                          *"
# "****************************************************************************"
# ""

echo ""
echo "-------- now making DMS LIB --------"
echo ""
cd /DBLIB
rm *.a *.o *.lib
cd /DMSSRC
rm *.o
make -f dblib.make


echo ""
echo "-------- now making IPC LIB --------"
echo ""
cd /DPSLIB
rm *.a *.o *.lib
cd /DPSSRC
rm *.o
make -f ipclib.make
make -f dpslib.make

echo ""
echo "-------- now making LAN LIB --------"
echo ""
cd /LANLIB
rm *.a *.o *.lib
cd /LANSRC
rm *.o
make -f lanlib.make



echo ""
echo "-------- now making CMD LIB --------"
echo ""
cd /CMDLIB
rm *.a *.o *.lib
cd /CMDSRC
rm *.o
make -f cmdlib.make


echo ""
echo "-------- now making DPP LIB --------"
echo ""
cd /DPPLIB
rm *.a *.o *.lib
cd /DPPSRC
rm *.o
make -f dpplib.make

echo ""
echo "-------- now making PLC LIB --------"
echo ""
cd /PLCLIB
rm *.a *.o *.lib
cd /PLCSRC
rm *.o
make -f plclib.make

echo ""
echo "-------- now making HDB LIB --------"
echo ""
if ( -d /HDBLIB ) then
cd /HDBLIB
rm *.a *.o *.lib
endif
if ( -d /HDBSRC ) then
cd /HDBSRC
rm *.o
make -f hdblib.make
endif

echo ""
echo "-------- now making DMS subsystem --------"
echo ""
cd /DMSSRC
rm *.o
DMSMAKE.csh

echo ""
echo "-------- now making * AGC * subsystem --------"
echo ""
if ( -d /AGCSRC ) then
cd /AGCSRC
rm *.o
rm *.?xe
AGCMAKE.csh
endif

echo ""
echo "-------- now making HMGR subsystem --------"
echo ""
cd /HMGRSRC
rm *.o
rm *.?xe
HMGRMAKE.csh

echo ""
echo "-------- now making DPS subsystem --------"
echo ""
cd /DPSSRC
DPSMAKE.csh


echo ""
echo "-------- now making LAN subsystem --------"
echo ""
cd /LANSRC
rm *.o
rm *.dxe
LANMAKE.csh



echo ""
echo "-------- now making PAS subsystem --------"
echo ""
cd /PASSRC
rm *.o
rm *.dxe
PASMAKE.csh




echo ""
echo "-------- now making TSsubsystem --------"
echo ""
cd /TSSRC
rm *.o
rm *.dxe
TSMAKE.csh

echo ""
echo "-------- now making PLCsubsystem --------"
echo ""
cd /PLCSRC
rm *.dxe
rm *.o
PLCMAKE.csh



echo ""
echo "-------- now making DPPsubsystem --------"
echo ""
cd /DPPSRC
rm *.o
rm *.dxe
LCUDPP.csh

echo ""
echo "-------- now making  hdb subsystem --------"
echo ""
if ( -d /HDBSRC ) then
cd /HDBSRC
rm *.o
rm *.dxe
HDBMAKE.csh
endif

echo ""
echo "-------- now making  iec subsystem --------"
echo ""
cd /IECSRC
rm *.o
rm *.dxe
IECMAKE.csh

echo ""
echo "-------- now making OIX subsystem --------"
echo ""
cd /OIXSRC
rm *.o
rm *.dxe
OIXMAKE.csh


echo ""
echo "-------- now making SMS subsystem --------"
echo ""
cd /SMSSRC
rm *.o
rm *.dxe
SMSMAKE.csh


echo ""
echo "-------- now copy dxe to /usr/ems/bin --------"
echo ""
/EMSMNT/EMS_EXE_COPY.csh



echo ""
echo "-------- finish EMS_MAKE --------"
echo "-------- ciwa thank you for using EMS_MAKE --------"
echo ""

