cd /DBLIB
rm *.o
rm *.a
rm *.lib
rm *.dlb

cd /DMSSRC

rm *.o
rm *.?xe

make -f dblib.make

make -f dmsmon.make

make -f dbread.make

make -f strTextGen.make

cp -p dmsmon.?xe /home/ems/h9000/bin/
cp -p dbread.?xe /home/ems/h9000/bin/
cp -p strTextGen.?xe /home/ems/h9000/bin/

