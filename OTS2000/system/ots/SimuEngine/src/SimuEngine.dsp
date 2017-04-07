# Microsoft Developer Studio Project File - Name="SimuEngine" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=SimuEngine - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SimuEngine.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SimuEngine.mak" CFG="SimuEngine - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SimuEngine - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "SimuEngine - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SimuEngine - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib \home\ems\system\lan\v4.0a\lib\lan.lib \home\ems\system\dps\v4.0a\lib\ipclib.lib \home\ems\system\dms\v4.0a\lib\dblib.lib \home\ems\system\cmd\v4.0a\lib\cmdlib.lib \home\ems\system\ots\ots_hdb\lib\ots.lib /nologo /subsystem:console /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"libcd.lib" /nodefaultlib:"libcmtd.lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "SimuEngine - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SimuEngine_"
# PROP BASE Intermediate_Dir "SimuEngine_"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /Gm /GX /ZI /Od /D "_WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /c
# SUBTRACT CPP /WX
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 wsock32.lib \home\ems\system\lan\v4.0a\lib\lan.lib \home\ems\system\dps\v4.0a\lib\ipclib.lib \home\ems\system\dms\v4.0a\lib\dblib.lib \home\ems\system\cmd\v4.0a\lib\cmdlib.lib \home\ems\system\ots\ots_hdb\lib\otsd.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /profile /debug /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"LIBCd.lib" /nodefaultlib:"libcmtd.lib"

!ENDIF 

# Begin Target

# Name "SimuEngine - Win32 Release"
# Name "SimuEngine - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\get_coded_cmd.c
# End Source File
# Begin Source File

SOURCE=.\interpolation.c
# End Source File
# Begin Source File

SOURCE=.\model_proc.c
# End Source File
# Begin Source File

SOURCE=.\read_recv_buf.c
# End Source File
# Begin Source File

SOURCE=.\SimuEngine.c
# End Source File
# Begin Source File

SOURCE=.\timer_task.c
# End Source File
# End Group
# Begin Group "Data Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\project\ots\v1.0a\Data\SimuEngineList.txt
# End Source File
# Begin Source File

SOURCE=..\..\..\..\project\ots\v1.0a\DATA\SimuEngineMain.txt
# End Source File
# Begin Source File

SOURCE=..\..\..\..\h9000\ots\sx_unit1.txt
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=..\..\..\oix\v4.0a\inc\bit_proc_def.h
# End Source File
# Begin Source File

SOURCE=..\..\..\cmd\v4.0a\inc\cmd_def.h
# End Source File
# Begin Source File

SOURCE=..\..\..\dms\v4.0a\inc\dms_structure_def.h
# End Source File
# Begin Source File

SOURCE=..\..\..\dps\v4.0a\inc\dps_rec_def.h
# End Source File
# Begin Source File

SOURCE=..\..\..\dps\v4.0a\inc\dps_type_def.h
# End Source File
# Begin Source File

SOURCE=..\inc\ots.h
# End Source File
# End Group
# End Target
# End Project
