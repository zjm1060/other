
cd /LANSRC
rm *.o
make -f lanlib.make
cd /PLCSRC
rm *.o
rm *.?xe

make -f plclib.make
make -f plcscan.make
make -f plccmd.make
make -f plcmon.make

cpy_bin.csh
