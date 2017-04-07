# Microsoft Developer Studio Project File - Name="teldef" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=teldef - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "teldef.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "teldef.mak" CFG="teldef - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "teldef - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "teldef - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "teldef - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "\home\ems\system\telalm\v4.0a\src\Release"
# PROP Intermediate_Dir "\home\ems\system\telalm\v4.0a\src\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /incremental:yes /debug /machine:I386
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "teldef - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "\home\ems\system\telalm\v4.0a\src\WinDebug"
# PROP Intermediate_Dir "\home\ems\system\telalm\v4.0a\src\WinDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FAcs /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /map /debug /machine:I386 /out:"\home\ems\h9000\bin/teldef.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "teldef - Win32 Release"
# Name "teldef - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AlmAnaDef.cpp
# End Source File
# Begin Source File

SOURCE=.\AlmClsDef.cpp
# End Source File
# Begin Source File

SOURCE=.\CreateText.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ProSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectSms.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectSmsTime.cpp
# End Source File
# Begin Source File

SOURCE=.\SmsDef.cpp
# End Source File
# Begin Source File

SOURCE=.\SmsNum.cpp
# End Source File
# Begin Source File

SOURCE=.\SmsSend.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"../inc/stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TelAudio.cpp
# End Source File
# Begin Source File

SOURCE=.\teldef.cpp
# End Source File
# Begin Source File

SOURCE=.\teldef.rc
# End Source File
# Begin Source File

SOURCE=.\TelDefine.cpp
# End Source File
# Begin Source File

SOURCE=.\teldefView.cpp
# End Source File
# Begin Source File

SOURCE=.\TelSms.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AlmClsDef.h
# End Source File
# Begin Source File

SOURCE=.\CreateText.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\ProSheet.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SelectSms.h
# End Source File
# Begin Source File

SOURCE=.\SelectSmsTime.h
# End Source File
# Begin Source File

SOURCE=.\SmsDef.h
# End Source File
# Begin Source File

SOURCE=.\SmsNum.h
# End Source File
# Begin Source File

SOURCE=.\SmsSend.h
# End Source File
# Begin Source File

SOURCE=.\TelAudio.h
# End Source File
# Begin Source File

SOURCE=.\teldef.h
# End Source File
# Begin Source File

SOURCE=.\teldefDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\teldefDoc.h
# End Source File
# Begin Source File

SOURCE=.\TelDefine.h
# End Source File
# Begin Source File

SOURCE=.\teldefView.h
# End Source File
# Begin Source File

SOURCE=.\TelSms.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\teldef.ico
# End Source File
# Begin Source File

SOURCE=.\res\teldef.rc2
# End Source File
# Begin Source File

SOURCE=.\res\teldefDoc.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
