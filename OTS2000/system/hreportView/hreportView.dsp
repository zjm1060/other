# Microsoft Developer Studio Project File - Name="hreportView" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=hreportView - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "hreportView.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "hreportView.mak" CFG="hreportView - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "hreportView - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "hreportView - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "hreportView - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "release"
# PROP BASE Intermediate_Dir "release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "release"
# PROP Intermediate_Dir "release"
# PROP Target_Dir ""
# ADD CPP /nologo /MD /W3 /GR /GX /O1 /I "c:\Qt\4.4.2\include\QtCore" /I "c:\Qt\4.4.2\include\QtGui" /I "c:\Qt\4.4.2\include\QtXml" /I "c:\Qt\4.4.2\include\QtSvg" /I "c:\Qt\4.4.2\include" /I "." /I "c:\Qt\4.4.2\include\ActiveQt" /I "release" /D "_WINDOWS" /D "UNICODE" /D "QT_LARGEFILE_SUPPORT" /D "QT_DLL" /D "QT_NO_DEBUG" /D "QT_SVG_LIB" /D "QT_XML_LIB" /D "QT_GUI_LIB" /D "QT_CORE_LIB" /D "QT_THREAD_SUPPORT" /D "WIN32" /FD /Ic:\Qt\4.4.2\mkspecs\win32-msvc -Zm200 /c
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 c:\Qt\4.4.2\lib\qtmain.lib c:\Qt\4.4.2\lib\QtSvg4.lib c:\Qt\4.4.2\lib\QtXml4.lib c:\Qt\4.4.2\lib\QtGui4.lib c:\Qt\4.4.2\lib\QtCore4.lib /nologo /subsystem:windows /machine:IX86 /libpath:"c:\Qt\4.4.2\lib"

!ELSEIF  "$(CFG)" == "hreportView - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "debug"
# PROP BASE Intermediate_Dir "debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "debug"
# PROP Intermediate_Dir "debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /MDd /W3 /GR /GX /Zi /I "c:\Qt\4.4.2\include\QtCore" /I "c:\Qt\4.4.2\include\QtGui" /I "c:\Qt\4.4.2\include\QtXml" /I "c:\Qt\4.4.2\include\QtSvg" /I "c:\Qt\4.4.2\include" /I "." /I "c:\Qt\4.4.2\include\ActiveQt" /I "debug" /D "_WINDOWS" /D "UNICODE" /D "QT_LARGEFILE_SUPPORT" /D "QT_DLL" /D "QT_SVG_LIB" /D "QT_XML_LIB" /D "QT_GUI_LIB" /D "QT_CORE_LIB" /D "QT_THREAD_SUPPORT" /D "WIN32" /FR /FD /Ic:\Qt\4.4.2\mkspecs\win32-msvc -Zm200 /c
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 c:\Qt\4.4.2\lib\qtmaind.lib c:\Qt\4.4.2\lib\QtSvgd4.lib c:\Qt\4.4.2\lib\QtXmld4.lib c:\Qt\4.4.2\lib\QtGuid4.lib c:\Qt\4.4.2\lib\QtCored4.lib /nologo /subsystem:windows /debug /machine:IX86 /libpath:"c:\Qt\4.4.2\lib" /FORCE:MULTIPLE
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "hreportView - Win32 Release"
# Name "hreportView - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=main.cpp
# End Source File
# Begin Source File

SOURCE=mainwindow.cpp
# End Source File
# Begin Source File

SOURCE=reportConfigReader.cpp
# End Source File
# Begin Source File

SOURCE=.\reportGen.cpp
# End Source File
# Begin Source File

SOURCE=reportInfoReader.cpp
# End Source File
# Begin Source File

SOURCE=reportService.cpp
# End Source File
# Begin Source File

SOURCE=reportTreeReader.cpp
# End Source File
# Begin Source File

SOURCE=soapC.cpp
# End Source File
# Begin Source File

SOURCE=soapClient.cpp
# End Source File
# Begin Source File

SOURCE=stdsoap2.cpp
# End Source File
# Begin Source File

SOURCE=svgview.cpp
# End Source File
# Begin Source File

SOURCE=svgwindow.cpp
# End Source File
# Begin Source File

SOURCE=timeInput.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=mainwindow.h

!IF  "$(CFG)" == "hreportView - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on mainwindow.h
InputPath=mainwindow.h

"release\moc_mainwindow.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	C:\Qt\4.4.2\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DQT_DLL -DQT_NO_DEBUG -DQT_SVG_LIB -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_THREAD_SUPPORT -Ic:\Qt\4.4.2\include\QtCore -Ic:\Qt\4.4.2\include\QtCore -Ic:\Qt\4.4.2\include\QtGui -Ic:\Qt\4.4.2\include\QtGui -Ic:\Qt\4.4.2\include\QtXml -Ic:\Qt\4.4.2\include\QtXml -Ic:\Qt\4.4.2\include\QtSvg -Ic:\Qt\4.4.2\include\QtSvg -Ic:\Qt\4.4.2\include -I. -Ic:\Qt\4.4.2\include\ActiveQt -Irelease -I. -Ic:\Qt\4.4.2\mkspecs\win32-msvc -D_MSC_VER=1200 -DWIN32 $(InputPath) -o release\moc_mainwindow.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "hreportView - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on mainwindow.h
InputPath=mainwindow.h

"debug\moc_mainwindow.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	C:\Qt\4.4.2\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DQT_DLL -DQT_SVG_LIB -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_THREAD_SUPPORT -Ic:\Qt\4.4.2\include\QtCore -Ic:\Qt\4.4.2\include\QtCore -Ic:\Qt\4.4.2\include\QtGui -Ic:\Qt\4.4.2\include\QtGui -Ic:\Qt\4.4.2\include\QtXml -Ic:\Qt\4.4.2\include\QtXml -Ic:\Qt\4.4.2\include\QtSvg -Ic:\Qt\4.4.2\include\QtSvg -Ic:\Qt\4.4.2\include -I. -Ic:\Qt\4.4.2\include\ActiveQt -Idebug -I. -Ic:\Qt\4.4.2\mkspecs\win32-msvc -D_MSC_VER=1200 -DWIN32 $(InputPath) -o debug\moc_mainwindow.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=reportConfigReader.h
# End Source File
# Begin Source File

SOURCE=.\reportGen.h
# End Source File
# Begin Source File

SOURCE=reportInfoReader.h
# End Source File
# Begin Source File

SOURCE=reportService.h
# End Source File
# Begin Source File

SOURCE=reportTreeReader.h
# End Source File
# Begin Source File

SOURCE=ServiceSoap.nsmap
# End Source File
# Begin Source File

SOURCE=soapH.h
# End Source File
# Begin Source File

SOURCE=soapServiceSoapProxy.h
# End Source File
# Begin Source File

SOURCE=soapStub.h
# End Source File
# Begin Source File

SOURCE=stdsoap2.h
# End Source File
# Begin Source File

SOURCE=svgview.h

!IF  "$(CFG)" == "hreportView - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on svgview.h
InputPath=svgview.h

"release\moc_svgview.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	C:\Qt\4.4.2\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DQT_DLL -DQT_NO_DEBUG -DQT_SVG_LIB -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_THREAD_SUPPORT -Ic:\Qt\4.4.2\include\QtCore -Ic:\Qt\4.4.2\include\QtCore -Ic:\Qt\4.4.2\include\QtGui -Ic:\Qt\4.4.2\include\QtGui -Ic:\Qt\4.4.2\include\QtXml -Ic:\Qt\4.4.2\include\QtXml -Ic:\Qt\4.4.2\include\QtSvg -Ic:\Qt\4.4.2\include\QtSvg -Ic:\Qt\4.4.2\include -I. -Ic:\Qt\4.4.2\include\ActiveQt -Irelease -I. -Ic:\Qt\4.4.2\mkspecs\win32-msvc -D_MSC_VER=1200 -DWIN32 $(InputPath) -o release\moc_svgview.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "hreportView - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on svgview.h
InputPath=svgview.h

"debug\moc_svgview.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	C:\Qt\4.4.2\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DQT_DLL -DQT_SVG_LIB -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_THREAD_SUPPORT -Ic:\Qt\4.4.2\include\QtCore -Ic:\Qt\4.4.2\include\QtCore -Ic:\Qt\4.4.2\include\QtGui -Ic:\Qt\4.4.2\include\QtGui -Ic:\Qt\4.4.2\include\QtXml -Ic:\Qt\4.4.2\include\QtXml -Ic:\Qt\4.4.2\include\QtSvg -Ic:\Qt\4.4.2\include\QtSvg -Ic:\Qt\4.4.2\include -I. -Ic:\Qt\4.4.2\include\ActiveQt -Idebug -I. -Ic:\Qt\4.4.2\mkspecs\win32-msvc -D_MSC_VER=1200 -DWIN32 $(InputPath) -o debug\moc_svgview.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=svgwindow.h
# End Source File
# Begin Source File

SOURCE=timeInput.h

!IF  "$(CFG)" == "hreportView - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on timeInput.h
InputPath=timeInput.h

"release\moc_timeInput.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	C:\Qt\4.4.2\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DQT_DLL -DQT_NO_DEBUG -DQT_SVG_LIB -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_THREAD_SUPPORT -Ic:\Qt\4.4.2\include\QtCore -Ic:\Qt\4.4.2\include\QtCore -Ic:\Qt\4.4.2\include\QtGui -Ic:\Qt\4.4.2\include\QtGui -Ic:\Qt\4.4.2\include\QtXml -Ic:\Qt\4.4.2\include\QtXml -Ic:\Qt\4.4.2\include\QtSvg -Ic:\Qt\4.4.2\include\QtSvg -Ic:\Qt\4.4.2\include -I. -Ic:\Qt\4.4.2\include\ActiveQt -Irelease -I. -Ic:\Qt\4.4.2\mkspecs\win32-msvc -D_MSC_VER=1200 -DWIN32 $(InputPath) -o release\moc_timeInput.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "hreportView - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running MOC on timeInput.h
InputPath=timeInput.h

"debug\moc_timeInput.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	C:\Qt\4.4.2\bin\moc.exe  -DUNICODE -DQT_LARGEFILE_SUPPORT -DQT_DLL -DQT_SVG_LIB -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_THREAD_SUPPORT -Ic:\Qt\4.4.2\include\QtCore -Ic:\Qt\4.4.2\include\QtCore -Ic:\Qt\4.4.2\include\QtGui -Ic:\Qt\4.4.2\include\QtGui -Ic:\Qt\4.4.2\include\QtXml -Ic:\Qt\4.4.2\include\QtXml -Ic:\Qt\4.4.2\include\QtSvg -Ic:\Qt\4.4.2\include\QtSvg -Ic:\Qt\4.4.2\include -I. -Ic:\Qt\4.4.2\include\ActiveQt -Idebug -I. -Ic:\Qt\4.4.2\mkspecs\win32-msvc -D_MSC_VER=1200 -DWIN32 $(InputPath) -o debug\moc_timeInput.cpp

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Resources"

# PROP Default_Filter "rc;qrc"
# Begin Source File

SOURCE=hreportView.qrc

!IF  "$(CFG)" == "hreportView - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running RCC on hreportView.qrc
InputPath=hreportView.qrc

"release\qrc_hreportView.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	C:\Qt\4.4.2\bin\rcc.exe -name hreportView $(InputPath) -o release\qrc_hreportView.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "hreportView - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Running RCC on hreportView.qrc
InputPath=hreportView.qrc

"debug\qrc_hreportView.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	C:\Qt\4.4.2\bin\rcc.exe -name hreportView $(InputPath) -o debug\qrc_hreportView.cpp

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Generated"

# PROP Default_Filter ""
# Begin Source File

SOURCE=debug\moc_mainwindow.cpp

!IF  "$(CFG)" == "hreportView - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "hreportView - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=release\moc_mainwindow.cpp

!IF  "$(CFG)" == "hreportView - Win32 Release"

!ELSEIF  "$(CFG)" == "hreportView - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=debug\moc_svgview.cpp

!IF  "$(CFG)" == "hreportView - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "hreportView - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=release\moc_svgview.cpp

!IF  "$(CFG)" == "hreportView - Win32 Release"

!ELSEIF  "$(CFG)" == "hreportView - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=debug\moc_timeInput.cpp

!IF  "$(CFG)" == "hreportView - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "hreportView - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=release\moc_timeInput.cpp

!IF  "$(CFG)" == "hreportView - Win32 Release"

!ELSEIF  "$(CFG)" == "hreportView - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=debug\qrc_hreportView.cpp

!IF  "$(CFG)" == "hreportView - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "hreportView - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=release\qrc_hreportView.cpp

!IF  "$(CFG)" == "hreportView - Win32 Release"

!ELSEIF  "$(CFG)" == "hreportView - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
