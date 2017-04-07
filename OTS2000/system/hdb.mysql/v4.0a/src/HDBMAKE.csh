cd /HDBSRC

rm *.o

make -f hdblib.make
#make -f curvescan.make
make -f curvewrite.make
#make -f revertscan.make
#make -f alarmscan.make
make -f hdbwrite.make
make -f periodwrite.make
#make -f hdbtest.make
cp hdbtest.dxe hdbinitpt.dxe

rm *.o
#cpy_bin.csh

echo "=========== HDBMAKE.csh completed!  ==========="