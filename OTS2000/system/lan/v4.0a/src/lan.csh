cd /LANSRC
rm *.o

make -f lanlib.make
make -f lan_outb.make
make -f lan_inb.make
make -f test.make
