# Microsoft Developer Studio Generated NMAKE File, Based on igc.dsp
!IF "$(CFG)" == ""
CFG=igc - Win32 Debug
!MESSAGE No configuration specified. Defaulting to igc - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "igc - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "igc.mak" CFG="igc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "igc - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\igc.exe" "$(OUTDIR)\igc.bsc"


CLEAN :
	-@erase "$(INTDIR)\igc.obj"
	-@erase "$(INTDIR)\igc.res"
	-@erase "$(INTDIR)\igc.sbr"
	-@erase "$(INTDIR)\stringTextFunc.obj"
	-@erase "$(INTDIR)\stringTextFunc.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\igc.bsc"
	-@erase "$(OUTDIR)\igc.exe"
	-@erase "$(OUTDIR)\igc.ilk"
	-@erase "$(OUTDIR)\igc.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\igc.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\igc.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\igc.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\igc.sbr" \
	"$(INTDIR)\stringTextFunc.sbr"

"$(OUTDIR)\igc.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=..\..\..\pas\v4.0a\lib\igclib.lib ..\..\..\dms\v4.0a\lib\dblib.lib ..\..\..\dps\v4.0a\lib\dpslib.lib ..\..\..\lan\v4.0a\lib\lan.lib ..\..\..\dps\v4.0a\lib\ipclib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\igc.pdb" /debug /machine:I386 /out:"$(OUTDIR)\igc.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\igc.obj" \
	"$(INTDIR)\stringTextFunc.obj" \
	"$(INTDIR)\igc.res"

"$(OUTDIR)\igc.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
   Put_SVNNum_to_exe.bat igc
	 $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"

!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("igc.dep")
!INCLUDE "igc.dep"
!ELSE 
!MESSAGE Warning: cannot find "igc.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "igc - Win32 Debug"
SOURCE=.\igc.rc

"$(INTDIR)\igc.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\igc.c

"$(INTDIR)\igc.obj"	"$(INTDIR)\igc.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=..\..\..\dms\v4.0a\src\stringTextFunc.c

"$(INTDIR)\stringTextFunc.obj"	"$(INTDIR)\stringTextFunc.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

