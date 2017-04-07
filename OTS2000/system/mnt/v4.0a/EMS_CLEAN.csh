
echo ""
echo ""
echo "******************** start Cleaning EMS system *****************************"
echo "*                                                                          *"
echo "* CLEAN_EMS will remove core file and mal-linking in all EMS directories.  *"
echo "*                                                                          *"
echo "*              By ciwa 2006.                                     *"
echo "*                                                                          *"
echo "****************************************************************************"
echo ""
echo "-------- *** Cleaning EMS root *** system --------"

echo ""
echo "-------- *** Cleaning HDB *** sub-system --------"


cd /HDBSRC
rm -r src
rm *.o 
rm core
cd /HDBINC
rm -r inc
rm core


echo ""
echo "-------- *** Cleaning AGC *** sub-system --------"
cd /AGCSRC
rm -r src
rm *.o 
rm core
cd /AGCINC
rm -r inc
rm core

echo ""
echo "-------- *** Cleaning HMGR *** sub-system --------"
cd /HMGRSRC
rm -r src
rm *.o 
rm core

echo ""
echo "-------- *** Cleaning DMS *** sub-system --------"

cd /DMSSRC
rm -r src
rm *.o  
rm core
cd /DBINC
rm -r inc
rm core
cd /DBLIB
rm -r lib
rm core
rm *.o 

echo ""
echo "-------- *** Cleaning DPS *** sub-system --------"
cd /DPSINC
rm -r inc
rm core
cd /DPSSRC
rm -r src
rm *.o  
rm core
cd /DPSLIB
rm -r lib
rm core
rm *.o


echo ""
echo "-------- *** Cleaning LAN *** sub-system --------"
cd /LANINC
rm -r inc
rm core
cd /LANLIB
rm -r lib
rm core
cd /LANSRC
rm -r src
rm *.o 
rm core

echo ""
echo "-------- *** Cleaning MNT *** sub-system --------"
cd /EMSMNT
rm -r mnt
rm core

echo ""
echo "-------- *** Cleaning OIX *** sub-system --------"
cd /OIXINC
rm -r inc
rm core
cd /OIXLIB
rm -r lib
rm core
rm *.o
cd /OIXSRC
rm -r src
rm *.o  
rm core


echo ""
echo "-------- *** Cleaning PAS *** sub-system --------"

cd /PASINC
rm -r inc
rm core
cd /PASSRC
rm -r src
rm *.o 
rm core

echo ""
echo "-------- *** Cleaning RPT *** sub-system --------"
cd /RPTLIB
rm -r lib
rm core
cd /RPTSRC
rm -r src
rm *.o 
rm core

echo ""
echo "-------- *** Cleaning SMS *** sub-system --------"
cd /SMSINC
rm -r inc
rm core
cd /SMSSRC
rm -r src
rm *.o  
rm core

echo ""
echo "-------- *** Cleaning TS *** sub-system --------"
cd /TSSRC
rm -r src
rm core
rm *.o 


echo ""
echo "-------- *** Cleaning CMD  *** sub-system --------"
cd /CMDSRC
rm -r src
rm core
rm *.o 

echo ""
echo "-------- *** Cleaning CMD  *** sub-system --------"
cd /CMDINC
rm -r inc 

echo ""
echo "-------- *** Cleaning  DPP *** sub-system --------"
cd /DPPSRC
rm -r src
rm core
rm *.o 

echo ""
echo "-------- *** Cleaning DPP  *** sub-system --------"
cd /DPPINC
rm -r inc

echo ""
echo "-------- *** Cleaning PLC *** sub-system --------"
cd /PLCSRC
rm -r src
rm core
rm *.o 
cd /PLCINC
rm -r inc

echo ""
echo "-------- *** Cleaning ICCP *** sub-system --------"
cd /ICCPSRC
rm -r src
rm core
rm *.o 
cd /ICCPINC
rm -r inc

echo ""
echo "-------- *** Cleaning IEC *** sub-system --------"
cd /IECSRC
rm -r src
rm core
rm *.o 
cd /IECINC
rm -r inc


echo ""
echo "********************** finish Clean EMS system *****************************"
echo "*                                                                          *"
echo "*   AEC/IWHR thank you for using CLEAN_EMS. Please clean regularly EMS.    *"
echo "*                                                                          *"
echo "****************************************************************************"


