@echo on
@set igc_gen_path="\home\ems\system\pas\v4.0a\src\Debug\igc_gen.exe"
@set igc__path="\home\ems\system\pas\v4.0a\src\Debug\igc.exe"
@set igc_c_path="\home\ems\system\pas\v4.0a\src\igc.c"
@if not exist %igc_gen_path% goto end0
@del %igc_c_path%
@if exist %igc_c_path% goto end1
@\home\ems\system\pas\v4.0a\src\Debug\igc_gen.exe
@del %igc__path%
echo igcgen finished!
echo start to gen igc.exe 
@NMAKE /f "igc.mak" CFG="igc - Win32 Debug"
echo igc.exe gen finished,press any key to exit 
@echo off
@pause 
goto end
:end0
echo err! can find %igc_gen_path%,press any key to exit 
@pause 
goto end
:end1
echo err!! can not remove  %igc_c_path%,press any key to exit 
@pause 
goto end
:end