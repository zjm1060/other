
cp -p /home/ems/system/agc/v4.0a/src/agc.?xe /home/ems/h9000/bin/
cp -p /home/ems/system/agc/v4.0a/src/avc.?xe /home/ems/h9000/bin/
cp -p /home/ems/system/agc/v4.0a/src/TransAck.?xe /home/ems/h9000/bin/
cp -p /home/ems/system/agc/v4.0a/src/TransSch.?xe /home/ems/h9000/bin/
cp -p /home/ems/system/agc/v4.0a/src/Simulate.?xe /home/ems/h9000/bin/

cp -p /home/ems/system/hmgr/v4.0a/src/hmgr.?xe /home/ems/h9000/bin/

cp -p /home/ems/system/dms/v4.0a/src/dmsmon.?xe /home/ems/h9000/bin/
cp -p /home/ems/system/dms/v4.0a/src/dbread.?xe /home/ems/h9000/bin/
cp -p /home/ems/system/dms/v4.0a/src/strTextGen.?xe /home/ems/h9000/bin/

cp -p /home/ems/system/dps/v4.0a/src/sysmgr.?xe /home/ems/h9000/bin/
cp -p /home/ems/system/dps/v4.0a/src/syshint.?xe /home/ems/h9000/bin/
cp -p /home/ems/system/dps/v4.0a/src/sysstop.?xe /home/ems/h9000/bin/
cp -p /home/ems/system/dps/v4.0a/src/dbwrite.?xe /home/ems/h9000/bin/
cp -p /home/ems/system/dps/v4.0a/src/myipcrm.?xe /home/ems/h9000/bin/
cp -p /home/ems/system/dps/v4.0a/src/faultTable.?xe /home/ems/h9000/bin/
cp -p /home/ems/system/dps/v4.0a/src/readshm.?xe /home/ems/h9000/bin/
cp -p /home/ems/system/dps/v4.0a/src/audAlm.?xe /home/ems/h9000/bin/



cp -p /home/ems/system/lan/v4.0a/src/lan_inb.?xe   /home/ems/h9000/bin/
cp -p /home/ems/system/lan/v4.0a/src/lan_outb.?xe  /home/ems/h9000/bin/
cp -p /home/ems/system/lan/v4.0a/src/stest.?xe  /home/ems/h9000/bin/
cp -p /home/ems/system/lan/v4.0a/src/looptest.?xe  /home/ems/h9000/bin/
cp -p /home/ems/system/lan/v4.0a/src/logwrite.?xe  /home/ems/h9000/bin/

cp -p /home/ems/system/oix/v4.0a/src/oix.?xe  /home/ems/h9000/bin/
cp -p /home/ems/system/oix/v4.0a/src/findstr.?xe  /home/ems/h9000/bin/

rm -f /home/ems/h9000/bin/plcmon.?xe
cp -p /home/ems/system/plc/v4.0a/src/plcscan.?xe   /home/ems/h9000/bin/
cp -p /home/ems/system/plc/v4.0a/src/plccmd.?xe   /home/ems/h9000/bin/

chmod u+s /home/ems/system/plc/v4.0a/src/plcmon.?xe
cp -p /home/ems/system/plc/v4.0a/src/plcmon.?xe   /home/ems/h9000/bin/

cp -p /home/ems/system/dpp/v4.0a/src/dpp.?xe	/home/ems/h9000/bin/

cp -p /home/ems/system/pas/v4.0a/src/calcugenname.?xe /home/ems/h9000/bin/
cp -p /home/ems/system/pas/v4.0a/src/interlockname.?xe /home/ems/h9000/bin/
cp -p /home/ems/system/pas/v4.0a/src/pdc.?xe /home/ems/h9000/bin/
cp -p /home/ems/system/pas/v4.0a/src/sgzy.?xe /home/ems/h9000/bin/
cp -p /home/ems/system/pas/v4.0a/src/sgzygen.?xe /home/ems/h9000/bin/
cp -p /home/ems/system/pas/v4.0a/src/trend_data_rec.?xe /home/ems/h9000/bin/
cp -p /home/ems/system/pas/v4.0a/src/RptGen.?xe /home/ems/h9000/bin/
cp -p /home/ems/system/pas/v4.0a/src/relatepoint.?xe /home/ems/h9000/bin/
cp -p /home/ems/system/pas/v4.0a/src/trendgen.?xe /home/ems/h9000/bin/


cp -p /home/ems/system/sms/v4.0a/src/privilege_mgr.?xe /home/ems/h9000/bin/


chmod u+s /home/ems/system/ts/v4.0a/src/readgps.?xe
cp -p /home/ems/system/ts/v4.0a/src/readgps.?xe /home/ems/h9000/bin/
 
cp -p /home/ems/system/hdb/v4.0a/src/alarmscan.?xe  /home/ems/h9000/bin/
cp -p /home/ems/system/hdb/v4.0a/src/revertscan.?xe /home/ems/h9000/bin/
cp -p /home/ems/system/hdb/v4.0a/src/curvescan.?xe /home/ems/h9000/bin/
cp -p /home/ems/system/hdb/v4.0a/src/hdbwrite.?xe /home/ems/h9000/bin/
 
chmod u+s /home/ems/system/iccp/v4.0a/src/iccp_main.?xe
cp -p /home/ems/system/iccp/v4.0a/src/iccp_main.?xe  /home/ems/h9000/bin/

cp -p /home/ems/system/iec/v4.0a/src/*.?xe  /home/ems/h9000/bin/

exit 0
