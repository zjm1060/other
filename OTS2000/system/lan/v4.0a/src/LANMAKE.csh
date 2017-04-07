
cd 	/LANSRC

rm *.o
rm *.?xe

make -f lanlib.make
make -f lan_inb.make
make -f lan_outb.make
make -f looptest.make
make -f stest.make
rm *.o
cpy_bin.csh

echo "=========== LANMAKE.csh completed ==============="