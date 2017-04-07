
cd 	/AGCSRC

rm *.o
rm *.dxe

make -f agc.make
make -f avc.make
make -f Simulate.make
make -f TransAck.make
make -f TransSch.make

rm *.o
cpy_bin.csh

echo "=========== AGCMAKE.csh completed ==============="