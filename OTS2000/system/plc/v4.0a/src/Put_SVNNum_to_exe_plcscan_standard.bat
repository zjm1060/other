rem
rem This file put SVN revision num into H9000 *.exe property
rem 此文件使用绝对路径，将本地工作拷贝的最新版本号写入生成的exe文件
rem 

set PATH=%PATH%;C:\Program Files\TortoiseSVN\bin

cd d:\home\ems\system\plc\v4.0a\src\

SubWCRev.exe . plcscan.rc  plcscan1.rc -f
rc.exe /l 0x804 /fo "Debug\plcscan.res" /d "_AUTOVERSION"  "plcscan1.rc" 
del plcscan1.rc
