rem
rem This file put SVN revision num into H9000 *.exe property
rem 此文件使用绝对路径，将本地工作拷贝的最新版本号写入生成的exe文件
rem 

set PATH=%PATH%;C:\Program Files\TortoiseSVN\bin

cd d:\home\ems\system\plc\v4.0a\src\

SubWCRev.exe . netmon.rc  netmon1.rc -f
rc.exe /l 0x804 /fo "Debug\netmon.res" /d "_AUTOVERSION"  "netmon1.rc" 
del netmon1.rc
