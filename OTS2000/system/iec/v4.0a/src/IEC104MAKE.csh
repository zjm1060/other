
cd 	/IEC104SRC

rm *.o
rm *.dxe

make -f iec104_com.make
make -f iec104_com-dx.make
make -f dsp_iec104_info.make
make -f CreateIecDef.make

rm *.o
cpy_bin.csh

echo "=========== IEC104MAKE.csh completed ==============="