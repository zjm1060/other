cd /DPSLIB
rm *.o
rm *.a
rm *.lib
rm *.dlb

cd /DPSSRC

rm *.o 
rm *.?xe

make -f ipclib.make
make -f dpslib.make
make -f dbwrite.make
make -f myipcrm.make
make -f readshm.make
make -f sysmgr.make
make -f faultTable.make
make -f audAlm.make
make -f syshint.make
make -f sysstop.make

cp -p dbwrite.?xe /h9000bin/
cp -p  sysmgr.?xe /h9000bin/
cp -p  readshm.?xe /h9000bin/
cp -p faultTable.?xe /h9000bin/
cp -p myipcrm.?xe /h9000bin/
cp -p audAlm.?xe /h9000bin/
cp -p syshint.?xe /h9000bin/
cp -p sysstop.?xe /h9000bin/




