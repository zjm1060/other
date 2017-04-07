
#!/usr/bin/csh
#PASMAKE.csh:* make file to make all PAS c programs *

cd /IECSRC

rm *.o
rm *.dxe

make -f Makefile.ems.iec104_com
make -f Makefile.ems.iec104s_com
make -f Makefile.ems.iec101s_com
make -f Makefile.ems.cc_iec101s_com
make -f Makefile.ems.infogen
make -f Makefile.ems.dsp_iec104_info
make -f Makefile.ems.dsp_comm_info

cp -p iec104s_com.dxe /h9000bin/
cp -p iec101s_com.dxe /h9000bin/
cp -p cc_iec101s_com.dxe /h9000bin/
cp -p iec104_com.dxe /h9000bin/
cp -p infogen.dxe /h9000bin/
cp -p dsp_iec104_info.dxe /h9000bin/
cp -p dsp_comm_info.dxe  /h9000bin/
