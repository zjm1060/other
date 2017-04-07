
#case "$1" in
#'MAKE_EMS_LINK_DIR')

echo ""
echo ""
echo "****************** start LINK_EMS_DIR system *****************************"
echo "*                                                                        *"
echo "* LINK_EMS_DIR will rebuild all EMS directories and sub-directories      *"
echo "* links.                                                                 *"
echo "*                                                                        *"
echo "*				By AEC/IWHR 1999.2.28.                         *"
echo "*                                                                        *"
echo "**************************************************************************"
echo ""
echo ""


#!/usr/bin/csh

#LINK_EMS_DIR.csh: Link EMS dir in root
set `/usr/bin/id`
if [ $1 != "uid=0(root)" ]; then
	echo "$0: must be run as root"
	exit 1
fi
  
cd /

#	build up "ems" system enviroment
rm -r /h9000bin
ln -s -f /home/ems/h9000/bin			/h9000bin

rm -r /h9000dat
ln -s -f /home/ems/h9000/dat			/h9000dat

rm -r /h9000def
ln -s -f /home/ems/h9000/def			/h9000def

rm -r /h9000dsp
ln -s -f /home/ems/h9000/dsp			/h9000dsp

rm -r /h9000gen
ln -s -f /home/ems/h9000/gen			/h9000gen

rm -r /h9000his
ln -s -f /home/ems/h9000/his			/h9000his

rm -r /h9000tmp
ln -s -f /home/ems/h9000/tmp			/h9000tmp





#	build up "agc" sub-system enviroment
rm -r /AGCINC /AGCSRC
ln -s -f /home/ems/system/agc/v4.0a/inc	/AGCINC
ln -s -f /home/ems/system/agc/v4.0a/src	/AGCSRC


#	build up "hmgr" sub-system enviroment
rm -r /DHSWSRC /DHSWINC /HMGRINC /HMGRSRC
ln -s -f /home/ems/system/hmgr/v4.0a/inc	/HMGRINC
ln -s -f /home/ems/system/hmgr/v4.0a/src	/HMGRSRC

#	build up "dms" sub-system enviroment
rm -r /DBINC /DBLIB /DMSSRC
ln -s -f /home/ems/system/dms/v4.0a/inc	/DBINC
ln -s -f /home/ems/system/dms/v4.0a/lib	/DBLIB
ln -s -f /home/ems/system/dms/v4.0a/src	/DMSSRC

#	build up "dps" sub-system enviroment
rm -r /DPSINC /DPSSRC /DPSLIB
ln -s -f /home/ems/system/dps/v4.0a/inc	/DPSINC
ln -s -f /home/ems/system/dps/v4.0a/src	/DPSSRC
ln -s -f /home/ems/system/dps/v4.0a/lib	/DPSLIB

#	build up "lan" sub-system enviroment
rm -r /LANINC /LANSRC /LANLIB
ln -s -f /home/ems/system/lan/v4.0a/inc	/LANINC
ln -s -f /home/ems/system/lan/v4.0a/src	/LANSRC
ln -s -f /home/ems/system/lan/v4.0a/lib	/LANLIB



#	build up "mnt" sub-system enviroment
rm -r /EMSMNT
ln -s -f /home/ems/system/mnt/v4.0a	/EMSMNT


#	build up "oix" sub-system enviroment
rm -r /OIXINC /OIXSRC /OIXLIB
ln -s -f /home/ems/system/oix/v4.0a/inc		/OIXINC
ln -s -f /home/ems/system/oix/v4.0a/src		/OIXSRC
ln -s -f /home/ems/system/oix/v4.0a/lib		/OIXLIB

#	build up "pas" sub-system enviroment
rm -r /PASINC /PASSRC
ln -s -f /home/ems/system/pas/v4.0a/inc		/PASINC
ln -s -f /home/ems/system/pas/v4.0a/src		/PASSRC

#	build up "rpt" sub-system enviroment
rm -r  /RPTSRC /RPTINC /RPTLIB
ln -s -f /home/ems/system/rpt/v4.0a/src		/RPTSRC
ln -s -f /home/ems/system/rpt/v4.0a/inc		/RPTINC
ln -s -f /home/ems/system/rpt/v4.0a/lib		/RPTLIB

#	build up "sms" sub-system enviroment
rm -r /SMSINC /SMSSRC
ln -s -f /home/ems/system/sms/v4.0a/inc		/SMSINC
ln -s -f /home/ems/system/sms/v4.0a/src		/SMSSRC

#	build up "ts" sub-system enviroment
rm -r /TSSRC
ln -s -f /home/ems/system/ts/v4.0a/src	/TSSRC

#  build up  lcu  sub-system enviroment
rm /DPPSRC  /DPPINC  /DPPLIB
ln -s -f /home/ems/system/dpp/v4.0a/src   /DPPSRC
ln -s -f /home/ems/system/dpp/v4.0a/inc   /DPPINC
ln -s -f /home/ems/system/dpp/v4.0a/lib   /DPPLIB

rm /CMDSRC /CMDINC /CMDLIB  
ln -s -f /home/ems/system/cmd/v4.0a/src   /CMDSRC
ln -s -f /home/ems/system/cmd/v4.0a/inc   /CMDINC
ln -s -f /home/ems/system/cmd/v4.0a/lib   /CMDLIB

rm /PLCSRC /PLCINC
ln -s -f /home/ems/system/plc/v4.0a/src   /PLCSRC
ln -s -f /home/ems/system/plc/v4.0a/inc   /PLCINC
ln -s -f /home/ems/system/plc/v4.0a/lib   /PLCLIB

rm /HDBSRC /HDBINC 
ln -s -f /home/ems/system/hdb/v4.0a/src   /HDBSRC
ln -s -f /home/ems/system/hdb/v4.0a/inc   /HDBINC
ln -s -f /home/ems/system/hdb/v4.0a/lib   /HDBLIB

rm /IECSRC /IECINC
ln -s -f /home/ems/system/iec/v4.0a/src   /IECSRC
ln -s -f /home/ems/system/iec/v4.0a/inc   /IECINC

rm /ICCPSRC /ICCPINC
ln -s -f /home/ems/system/iccp/v4.0a/src   /ICCPSRC
ln -s -f /home/ems/system/iccp/v4.0a/inc   /ICCPINC


echo ""
echo "***************** finish LINK_EMS_DIR system *****************************"
echo "*                                                                        *"
echo "*          AEC/IWHR thank you for using LINK_EMS_DIR.                    *"
echo "*                                                                        *"
echo "**************************************************************************"





exit 0
