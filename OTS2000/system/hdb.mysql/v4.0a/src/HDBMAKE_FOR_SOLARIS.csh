cd /HDBSRC

rm *.o

make -f hdblib.make.solaris
#make -f curvescan.make.solaris
make -f curvewrite.make.solaris
#make -f revertscan.make.solaris
#make -f alarmscan.make.solaris
make -f hdbwrite.make.solaris
make -f periodwrite.make.solaris
#make -f hdbtest.make.solaris
cp hdbtest.dxe hdbinitpt.dxe

rm *.o
#cpy_bin.csh

echo "=========== HDBMAKE.csh completed!  ==========="