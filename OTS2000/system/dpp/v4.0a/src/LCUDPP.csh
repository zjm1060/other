cd /DPPLIB
rm *.o
rm *.lib


cd /DPPSRC
rm *.o
rm *.?xe

make -f dpplib.make

make -f dpp.make

cp dpp.?xe /h9000bin/

