
cd 	/IECSRC

rm *.o
rm *.dxe

make -f iec104_com_aix.make
make -f iec104s_com_aix.make
make -f infogen_aix.make


rm *.o
cpy_bin.csh

echo "=========== IEC104MAKE.csh completed ==============="