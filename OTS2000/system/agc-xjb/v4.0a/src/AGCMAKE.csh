
cd 	/home/ems/system/agc-xjb/v4.0a/src

rm *.o
rm *.?xe

make -f agc.make
make -f avc.make
make -f Simulate.make


rm *.o
cpy_bin.csh

echo "=========== AGCMAKE.csh completed ==============="