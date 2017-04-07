# Microsoft Developer Studio Generated NMAKE File, Based on igc_gen.dsp
!IF "$(CFG)" == ""
CFG=igc_gen - Win32 Debug
!MESSAGE No configuration specified. Defaulting to igc_gen - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "igc_gen - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "igc_gen.mak" CFG="igc_gen - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "igc_gen - Win32 Debug" (based on "Win32 (x86) Console Application")
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

ALL : "$(OUTDIR)\igc_gen.exe" "$(OUTDIR)\igc_gen.bsc"


CLEAN :
	-@erase "$(INTDIR)\igc_gen.obj"
	-@erase "$(INTDIR)\igc_gen.res"
	-@erase "$(INTDIR)\igc_gen.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\igc_gen.bsc"
	-@erase "$(OUTDIR)\igc_gen.exe"
	-@erase "$(OUTDIR)\igc_gen.ilk"
	-@erase "$(OUTDIR)\igc_gen.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\igc_gen.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\igc_gen.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\igc_gen.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\igc_gen.sbr"

"$(OUTDIR)\igc_gen.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=..\..\..\pas\v4.0a\lib\igclib.lib ..\..\..\dms\v4.0a\lib\dblib.lib ..\..\..\dps\v4.0a\lib\dpslib.lib ..\..\..\lan\v4.0a\lib\lan.lib ..\..\..\dps\v4.0a\lib\ipclib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\igc_gen.pdb" /debug /machine:I386 /out:"$(OUTDIR)\igc_gen.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\igc_gen.obj" \
	"$(INTDIR)\igc_gen.res"

"$(OUTDIR)\igc_gen.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
   Put_SVNNum_to_exe.bat igc_gen
	 $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"

!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("igc_gen.dep")
!INCLUDE "igc_gen.dep"
!ELSE 
!MESSAGE Warning: cannot find "igc_gen.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "igc_gen - Win32 Debug"
SOURCE=.\igc_gen.rc

"$(INTDIR)\igc_gen.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\igc_gen.c

"$(INTDIR)\igc_gen.obj"	"$(INTDIR)\igc_gen.sbr" : $(SOURCE) "$(INTDIR)"



!ENDIF 

