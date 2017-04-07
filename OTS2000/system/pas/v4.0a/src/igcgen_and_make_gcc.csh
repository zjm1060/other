#!/usr/bin/csh
#igcgen_and_make.csh:* make file to make igc.dxe *
set igc_gen_nm="igc_gen.dxe"
set igc_c_nm="igc.c"
cd /PASSRC
if(! -e $igc_gen_nm) then 
goto end0
else
echo start to run igc_gen.dxe
endif
rm igc.c
if( -e $igc_c_nm) then 
goto end1
else
echo begin run igc_gen.dxe
endif
rm igc.dxe
echo start to gen igc.dxe 
igc_gen.dxe
make -f igc_gcc.make
echo igc.dxe gen finished!
#cp -p igc.dxe  /h9000bin/
#echo copy igc.dxe to h9000bin OK!
goto end 
end0:
echo err! can not find /home/ems/system/pas/v4.0a/src/igc_gen.dxe
# $< 
goto end 
end1:
echo err!! can not remove  /home/ems/system/pas/v4.0a/src/igc.c 
# $< 
goto end
end: 
echo end