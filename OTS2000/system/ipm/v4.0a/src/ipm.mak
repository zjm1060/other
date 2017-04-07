# Microsoft Developer Studio Generated NMAKE File, Based on ipm.dsp
!IF "$(CFG)" == ""
CFG=ipm - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ipm - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ipm - Win32 Release" && "$(CFG)" != "ipm - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ipm.mak" CFG="ipm - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ipm - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ipm - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ipm - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\ipm.exe"


CLEAN :
	-@erase "$(INTDIR)\ipm.res"
	-@erase "$(INTDIR)\mc_attribute.obj"
	-@erase "$(INTDIR)\mc_color_grp_p.obj"
	-@erase "$(INTDIR)\mc_color_p.obj"
	-@erase "$(INTDIR)\mc_draw_p.obj"
	-@erase "$(INTDIR)\mc_draw_util.obj"
	-@erase "$(INTDIR)\mc_dyn_form_p.obj"
	-@erase "$(INTDIR)\mc_dyn_handler.obj"
	-@erase "$(INTDIR)\mc_dyn_test.obj"
	-@erase "$(INTDIR)\mc_dyn_util.obj"
	-@erase "$(INTDIR)\mc_edit_p.obj"
	-@erase "$(INTDIR)\mc_file_p.obj"
	-@erase "$(INTDIR)\mc_file_search.obj"
	-@erase "$(INTDIR)\mc_fill_p.obj"
	-@erase "$(INTDIR)\mc_find_paths.obj"
	-@erase "$(INTDIR)\mc_font_p.obj"
	-@erase "$(INTDIR)\mc_format_p.obj"
	-@erase "$(INTDIR)\mc_get_text_p.obj"
	-@erase "$(INTDIR)\mc_grid_p.obj"
	-@erase "$(INTDIR)\mc_ipm.obj"
	-@erase "$(INTDIR)\mc_ipm_init.obj"
	-@erase "$(INTDIR)\mc_ipmmenu.obj"
	-@erase "$(INTDIR)\mc_layer_p.obj"
	-@erase "$(INTDIR)\mc_line_width_p.obj"
	-@erase "$(INTDIR)\mc_menu_edit_p.obj"
	-@erase "$(INTDIR)\mc_parse_pattern.obj"
	-@erase "$(INTDIR)\mc_pr.obj"
	-@erase "$(INTDIR)\mc_read_tmplt_file.obj"
	-@erase "$(INTDIR)\mc_replace_p.obj"
	-@erase "$(INTDIR)\mc_string_grp_p.obj"
	-@erase "$(INTDIR)\mc_sub_dsp_proc.obj"
	-@erase "$(INTDIR)\mc_symbol_grp_p.obj"
	-@erase "$(INTDIR)\mc_symbol_p.obj"
	-@erase "$(INTDIR)\mc_text_edit.obj"
	-@erase "$(INTDIR)\mc_toolkit.obj"
	-@erase "$(INTDIR)\mc_util_p.obj"
	-@erase "$(INTDIR)\mc_view_p.obj"
	-@erase "$(INTDIR)\mc_zoom_p.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\ipm.exe"
	-@erase "$(OUTDIR)\ipm.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\ipm.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\ipm.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ipm.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=\users\ems\system\dms\v4.0a\lib\dblib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\ipm.pdb" /machine:I386 /out:"$(OUTDIR)\ipm.exe" 
LINK32_OBJS= \
	"$(INTDIR)\mc_attribute.obj" \
	"$(INTDIR)\mc_color_grp_p.obj" \
	"$(INTDIR)\mc_color_p.obj" \
	"$(INTDIR)\mc_draw_p.obj" \
	"$(INTDIR)\mc_draw_util.obj" \
	"$(INTDIR)\mc_dyn_form_p.obj" \
	"$(INTDIR)\mc_dyn_handler.obj" \
	"$(INTDIR)\mc_dyn_test.obj" \
	"$(INTDIR)\mc_dyn_util.obj" \
	"$(INTDIR)\mc_edit_p.obj" \
	"$(INTDIR)\mc_file_p.obj" \
	"$(INTDIR)\mc_file_search.obj" \
	"$(INTDIR)\mc_fill_p.obj" \
	"$(INTDIR)\mc_find_paths.obj" \
	"$(INTDIR)\mc_font_p.obj" \
	"$(INTDIR)\mc_format_p.obj" \
	"$(INTDIR)\mc_get_text_p.obj" \
	"$(INTDIR)\mc_grid_p.obj" \
	"$(INTDIR)\mc_ipm.obj" \
	"$(INTDIR)\mc_ipm_init.obj" \
	"$(INTDIR)\mc_ipmmenu.obj" \
	"$(INTDIR)\mc_layer_p.obj" \
	"$(INTDIR)\mc_line_width_p.obj" \
	"$(INTDIR)\mc_menu_edit_p.obj" \
	"$(INTDIR)\mc_parse_pattern.obj" \
	"$(INTDIR)\mc_pr.obj" \
	"$(INTDIR)\mc_read_tmplt_file.obj" \
	"$(INTDIR)\mc_replace_p.obj" \
	"$(INTDIR)\mc_string_grp_p.obj" \
	"$(INTDIR)\mc_sub_dsp_proc.obj" \
	"$(INTDIR)\mc_symbol_grp_p.obj" \
	"$(INTDIR)\mc_symbol_p.obj" \
	"$(INTDIR)\mc_text_edit.obj" \
	"$(INTDIR)\mc_toolkit.obj" \
	"$(INTDIR)\mc_util_p.obj" \
	"$(INTDIR)\mc_view_p.obj" \
	"$(INTDIR)\mc_zoom_p.obj" \
	"$(INTDIR)\ipm.res"

"$(OUTDIR)\ipm.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ipm - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\ipm.exe" "$(OUTDIR)\ipm.bsc"


CLEAN :
	-@erase "$(INTDIR)\ipm.res"
	-@erase "$(INTDIR)\mc_attribute.obj"
	-@erase "$(INTDIR)\mc_attribute.sbr"
	-@erase "$(INTDIR)\mc_color_grp_p.obj"
	-@erase "$(INTDIR)\mc_color_grp_p.sbr"
	-@erase "$(INTDIR)\mc_color_p.obj"
	-@erase "$(INTDIR)\mc_color_p.sbr"
	-@erase "$(INTDIR)\mc_draw_p.obj"
	-@erase "$(INTDIR)\mc_draw_p.sbr"
	-@erase "$(INTDIR)\mc_draw_util.obj"
	-@erase "$(INTDIR)\mc_draw_util.sbr"
	-@erase "$(INTDIR)\mc_dyn_form_p.obj"
	-@erase "$(INTDIR)\mc_dyn_form_p.sbr"
	-@erase "$(INTDIR)\mc_dyn_handler.obj"
	-@erase "$(INTDIR)\mc_dyn_handler.sbr"
	-@erase "$(INTDIR)\mc_dyn_test.obj"
	-@erase "$(INTDIR)\mc_dyn_test.sbr"
	-@erase "$(INTDIR)\mc_dyn_util.obj"
	-@erase "$(INTDIR)\mc_dyn_util.sbr"
	-@erase "$(INTDIR)\mc_edit_p.obj"
	-@erase "$(INTDIR)\mc_edit_p.sbr"
	-@erase "$(INTDIR)\mc_file_p.obj"
	-@erase "$(INTDIR)\mc_file_p.sbr"
	-@erase "$(INTDIR)\mc_file_search.obj"
	-@erase "$(INTDIR)\mc_file_search.sbr"
	-@erase "$(INTDIR)\mc_fill_p.obj"
	-@erase "$(INTDIR)\mc_fill_p.sbr"
	-@erase "$(INTDIR)\mc_find_paths.obj"
	-@erase "$(INTDIR)\mc_find_paths.sbr"
	-@erase "$(INTDIR)\mc_font_p.obj"
	-@erase "$(INTDIR)\mc_font_p.sbr"
	-@erase "$(INTDIR)\mc_format_p.obj"
	-@erase "$(INTDIR)\mc_format_p.sbr"
	-@erase "$(INTDIR)\mc_get_text_p.obj"
	-@erase "$(INTDIR)\mc_get_text_p.sbr"
	-@erase "$(INTDIR)\mc_grid_p.obj"
	-@erase "$(INTDIR)\mc_grid_p.sbr"
	-@erase "$(INTDIR)\mc_ipm.obj"
	-@erase "$(INTDIR)\mc_ipm.sbr"
	-@erase "$(INTDIR)\mc_ipm_init.obj"
	-@erase "$(INTDIR)\mc_ipm_init.sbr"
	-@erase "$(INTDIR)\mc_ipmmenu.obj"
	-@erase "$(INTDIR)\mc_ipmmenu.sbr"
	-@erase "$(INTDIR)\mc_layer_p.obj"
	-@erase "$(INTDIR)\mc_layer_p.sbr"
	-@erase "$(INTDIR)\mc_line_width_p.obj"
	-@erase "$(INTDIR)\mc_line_width_p.sbr"
	-@erase "$(INTDIR)\mc_menu_edit_p.obj"
	-@erase "$(INTDIR)\mc_menu_edit_p.sbr"
	-@erase "$(INTDIR)\mc_parse_pattern.obj"
	-@erase "$(INTDIR)\mc_parse_pattern.sbr"
	-@erase "$(INTDIR)\mc_pr.obj"
	-@erase "$(INTDIR)\mc_pr.sbr"
	-@erase "$(INTDIR)\mc_read_tmplt_file.obj"
	-@erase "$(INTDIR)\mc_read_tmplt_file.sbr"
	-@erase "$(INTDIR)\mc_replace_p.obj"
	-@erase "$(INTDIR)\mc_replace_p.sbr"
	-@erase "$(INTDIR)\mc_string_grp_p.obj"
	-@erase "$(INTDIR)\mc_string_grp_p.sbr"
	-@erase "$(INTDIR)\mc_sub_dsp_proc.obj"
	-@erase "$(INTDIR)\mc_sub_dsp_proc.sbr"
	-@erase "$(INTDIR)\mc_symbol_grp_p.obj"
	-@erase "$(INTDIR)\mc_symbol_grp_p.sbr"
	-@erase "$(INTDIR)\mc_symbol_p.obj"
	-@erase "$(INTDIR)\mc_symbol_p.sbr"
	-@erase "$(INTDIR)\mc_text_edit.obj"
	-@erase "$(INTDIR)\mc_text_edit.sbr"
	-@erase "$(INTDIR)\mc_toolkit.obj"
	-@erase "$(INTDIR)\mc_toolkit.sbr"
	-@erase "$(INTDIR)\mc_util_p.obj"
	-@erase "$(INTDIR)\mc_util_p.sbr"
	-@erase "$(INTDIR)\mc_view_p.obj"
	-@erase "$(INTDIR)\mc_view_p.sbr"
	-@erase "$(INTDIR)\mc_zoom_p.obj"
	-@erase "$(INTDIR)\mc_zoom_p.sbr"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\ipm.bsc"
	-@erase "$(OUTDIR)\ipm.exe"
	-@erase "$(OUTDIR)\ipm.ilk"
	-@erase "$(OUTDIR)\ipm.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\ipm.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\ipm.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ipm.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\mc_attribute.sbr" \
	"$(INTDIR)\mc_color_grp_p.sbr" \
	"$(INTDIR)\mc_color_p.sbr" \
	"$(INTDIR)\mc_draw_p.sbr" \
	"$(INTDIR)\mc_draw_util.sbr" \
	"$(INTDIR)\mc_dyn_form_p.sbr" \
	"$(INTDIR)\mc_dyn_handler.sbr" \
	"$(INTDIR)\mc_dyn_test.sbr" \
	"$(INTDIR)\mc_dyn_util.sbr" \
	"$(INTDIR)\mc_edit_p.sbr" \
	"$(INTDIR)\mc_file_p.sbr" \
	"$(INTDIR)\mc_file_search.sbr" \
	"$(INTDIR)\mc_fill_p.sbr" \
	"$(INTDIR)\mc_find_paths.sbr" \
	"$(INTDIR)\mc_font_p.sbr" \
	"$(INTDIR)\mc_format_p.sbr" \
	"$(INTDIR)\mc_get_text_p.sbr" \
	"$(INTDIR)\mc_grid_p.sbr" \
	"$(INTDIR)\mc_ipm.sbr" \
	"$(INTDIR)\mc_ipm_init.sbr" \
	"$(INTDIR)\mc_ipmmenu.sbr" \
	"$(INTDIR)\mc_layer_p.sbr" \
	"$(INTDIR)\mc_line_width_p.sbr" \
	"$(INTDIR)\mc_menu_edit_p.sbr" \
	"$(INTDIR)\mc_parse_pattern.sbr" \
	"$(INTDIR)\mc_pr.sbr" \
	"$(INTDIR)\mc_read_tmplt_file.sbr" \
	"$(INTDIR)\mc_replace_p.sbr" \
	"$(INTDIR)\mc_string_grp_p.sbr" \
	"$(INTDIR)\mc_sub_dsp_proc.sbr" \
	"$(INTDIR)\mc_symbol_grp_p.sbr" \
	"$(INTDIR)\mc_symbol_p.sbr" \
	"$(INTDIR)\mc_text_edit.sbr" \
	"$(INTDIR)\mc_toolkit.sbr" \
	"$(INTDIR)\mc_util_p.sbr" \
	"$(INTDIR)\mc_view_p.sbr" \
	"$(INTDIR)\mc_zoom_p.sbr"

"$(OUTDIR)\ipm.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=..\..\..\dms\v4.0a\lib\dblib.lib ..\..\..\lan\v4.0a\lib\lan.lib ..\..\..\dps\v4.0a\lib\ipclib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\ipm.pdb" /debug /machine:I386 /nodefaultlib:"libc.lib" /out:"$(OUTDIR)\ipm.exe" 
LINK32_OBJS= \
	"$(INTDIR)\mc_attribute.obj" \
	"$(INTDIR)\mc_color_grp_p.obj" \
	"$(INTDIR)\mc_color_p.obj" \
	"$(INTDIR)\mc_draw_p.obj" \
	"$(INTDIR)\mc_draw_util.obj" \
	"$(INTDIR)\mc_dyn_form_p.obj" \
	"$(INTDIR)\mc_dyn_handler.obj" \
	"$(INTDIR)\mc_dyn_test.obj" \
	"$(INTDIR)\mc_dyn_util.obj" \
	"$(INTDIR)\mc_edit_p.obj" \
	"$(INTDIR)\mc_file_p.obj" \
	"$(INTDIR)\mc_file_search.obj" \
	"$(INTDIR)\mc_fill_p.obj" \
	"$(INTDIR)\mc_find_paths.obj" \
	"$(INTDIR)\mc_font_p.obj" \
	"$(INTDIR)\mc_format_p.obj" \
	"$(INTDIR)\mc_get_text_p.obj" \
	"$(INTDIR)\mc_grid_p.obj" \
	"$(INTDIR)\mc_ipm.obj" \
	"$(INTDIR)\mc_ipm_init.obj" \
	"$(INTDIR)\mc_ipmmenu.obj" \
	"$(INTDIR)\mc_layer_p.obj" \
	"$(INTDIR)\mc_line_width_p.obj" \
	"$(INTDIR)\mc_menu_edit_p.obj" \
	"$(INTDIR)\mc_parse_pattern.obj" \
	"$(INTDIR)\mc_pr.obj" \
	"$(INTDIR)\mc_read_tmplt_file.obj" \
	"$(INTDIR)\mc_replace_p.obj" \
	"$(INTDIR)\mc_string_grp_p.obj" \
	"$(INTDIR)\mc_sub_dsp_proc.obj" \
	"$(INTDIR)\mc_symbol_grp_p.obj" \
	"$(INTDIR)\mc_symbol_p.obj" \
	"$(INTDIR)\mc_text_edit.obj" \
	"$(INTDIR)\mc_toolkit.obj" \
	"$(INTDIR)\mc_util_p.obj" \
	"$(INTDIR)\mc_view_p.obj" \
	"$(INTDIR)\mc_zoom_p.obj" \
	"$(INTDIR)\ipm.res"

"$(OUTDIR)\ipm.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

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


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("ipm.dep")
!INCLUDE "ipm.dep"
!ELSE 
!MESSAGE Warning: cannot find "ipm.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ipm - Win32 Release" || "$(CFG)" == "ipm - Win32 Debug"
SOURCE=.\ipm.rc

"$(INTDIR)\ipm.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\mc_attribute.c

!IF  "$(CFG)" == "ipm - Win32 Release"


"$(INTDIR)\mc_attribute.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ipm - Win32 Debug"


"$(INTDIR)\mc_attribute.obj"	"$(INTDIR)\mc_attribute.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mc_color_grp_p.c

!IF  "$(CFG)" == "ipm - Win32 Release"


"$(INTDIR)\mc_color_grp_p.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ipm - Win32 Debug"


"$(INTDIR)\mc_color_grp_p.obj"	"$(INTDIR)\mc_color_grp_p.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mc_color_p.c

!IF  "$(CFG)" == "ipm - Win32 Release"


"$(INTDIR)\mc_color_p.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ipm - Win32 Debug"


"$(INTDIR)\mc_color_p.obj"	"$(INTDIR)\mc_color_p.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mc_draw_p.c

!IF  "$(CFG)" == "ipm - Win32 Release"


"$(INTDIR)\mc_draw_p.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ipm - Win32 Debug"


"$(INTDIR)\mc_draw_p.obj"	"$(INTDIR)\mc_draw_p.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mc_draw_util.c

!IF  "$(CFG)" == "ipm - Win32 Release"


"$(INTDIR)\mc_draw_util.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ipm - Win32 Debug"


"$(INTDIR)\mc_draw_util.obj"	"$(INTDIR)\mc_draw_util.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mc_dyn_form_p.c

!IF  "$(CFG)" == "ipm - Win32 Release"


"$(INTDIR)\mc_dyn_form_p.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ipm - Win32 Debug"


"$(INTDIR)\mc_dyn_form_p.obj"	"$(INTDIR)\mc_dyn_form_p.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mc_dyn_handler.c

!IF  "$(CFG)" == "ipm - Win32 Release"


"$(INTDIR)\mc_dyn_handler.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ipm - Win32 Debug"


"$(INTDIR)\mc_dyn_handler.obj"	"$(INTDIR)\mc_dyn_handler.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mc_dyn_test.c

!IF  "$(CFG)" == "ipm - Win32 Release"


"$(INTDIR)\mc_dyn_test.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ipm - Win32 Debug"


"$(INTDIR)\mc_dyn_test.obj"	"$(INTDIR)\mc_dyn_test.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mc_dyn_util.c

!IF  "$(CFG)" == "ipm - Win32 Release"


"$(INTDIR)\mc_dyn_util.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ipm - Win32 Debug"


"$(INTDIR)\mc_dyn_util.obj"	"$(INTDIR)\mc_dyn_util.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mc_edit_p.c

!IF  "$(CFG)" == "ipm - Win32 Release"


"$(INTDIR)\mc_edit_p.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ipm - Win32 Debug"


"$(INTDIR)\mc_edit_p.obj"	"$(INTDIR)\mc_edit_p.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mc_file_p.c

!IF  "$(CFG)" == "ipm - Win32 Release"


"$(INTDIR)\mc_file_p.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ipm - Win32 Debug"


"$(INTDIR)\mc_file_p.obj"	"$(INTDIR)\mc_file_p.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mc_file_search.c

!IF  "$(CFG)" == "ipm - Win32 Release"


"$(INTDIR)\mc_file_search.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ipm - Win32 Debug"


"$(INTDIR)\mc_file_search.obj"	"$(INTDIR)\mc_file_search.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mc_fill_p.c

!IF  "$(CFG)" == "ipm - Win32 Release"


"$(INTDIR)\mc_fill_p.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ipm - Win32 Debug"


"$(INTDIR)\mc_fill_p.obj"	"$(INTDIR)\mc_fill_p.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mc_find_paths.c

!IF  "$(CFG)" == "ipm - Win32 Release"


"$(INTDIR)\mc_find_paths.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ipm - Win32 Debug"


"$(INTDIR)\mc_find_paths.obj"	"$(INTDIR)\mc_find_paths.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mc_font_p.c

!IF  "$(CFG)" == "ipm - Win32 Release"


"$(INTDIR)\mc_font_p.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ipm - Win32 Debug"


"$(INTDIR)\mc_font_p.obj"	"$(INTDIR)\mc_font_p.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mc_format_p.c

!IF  "$(CFG)" == "ipm - Win32 Release"


"$(INTDIR)\mc_format_p.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ipm - Win32 Debug"


"$(INTDIR)\mc_format_p.obj"	"$(INTDIR)\mc_format_p.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mc_get_text_p.c

!IF  "$(CFG)" == "ipm - Win32 Release"


"$(INTDIR)\mc_get_text_p.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ipm - Win32 Debug"


"$(INTDIR)\mc_get_text_p.obj"	"$(INTDIR)\mc_get_text_p.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mc_grid_p.c

!IF  "$(CFG)" == "ipm - Win32 Release"


"$(INTDIR)\mc_grid_p.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ipm - Win32 Debug"


"$(INTDIR)\mc_grid_p.obj"	"$(INTDIR)\mc_grid_p.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mc_ipm.c

!IF  "$(CFG)" == "ipm - Win32 Release"


"$(INTDIR)\mc_ipm.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ipm - Win32 Debug"


"$(INTDIR)\mc_ipm.obj"	"$(INTDIR)\mc_ipm.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mc_ipm_init.c

!IF  "$(CFG)" == "ipm - Win32 Release"


"$(INTDIR)\mc_ipm_init.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ipm - Win32 Debug"


"$(INTDIR)\mc_ipm_init.obj"	"$(INTDIR)\mc_ipm_init.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mc_ipmmenu.c

!IF  "$(CFG)" == "ipm - Win32 Release"


"$(INTDIR)\mc_ipmmenu.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ipm - Win32 Debug"


"$(INTDIR)\mc_ipmmenu.obj"	"$(INTDIR)\mc_ipmmenu.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mc_layer_p.c

!IF  "$(CFG)" == "ipm - Win32 Release"


"$(INTDIR)\mc_layer_p.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ipm - Win32 Debug"


"$(INTDIR)\mc_layer_p.obj"	"$(INTDIR)\mc_layer_p.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mc_line_width_p.c

!IF  "$(CFG)" == "ipm - Win32 Release"


"$(INTDIR)\mc_line_width_p.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ipm - Win32 Debug"


"$(INTDIR)\mc_line_width_p.obj"	"$(INTDIR)\mc_line_width_p.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mc_menu_edit_p.c

!IF  "$(CFG)" == "ipm - Win32 Release"


"$(INTDIR)\mc_menu_edit_p.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ipm - Win32 Debug"


"$(INTDIR)\mc_menu_edit_p.obj"	"$(INTDIR)\mc_menu_edit_p.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mc_parse_pattern.c

!IF  "$(CFG)" == "ipm - Win32 Release"


"$(INTDIR)\mc_parse_pattern.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ipm - Win32 Debug"


"$(INTDIR)\mc_parse_pattern.obj"	"$(INTDIR)\mc_parse_pattern.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mc_pr.c

!IF  "$(CFG)" == "ipm - Win32 Release"


"$(INTDIR)\mc_pr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ipm - Win32 Debug"


"$(INTDIR)\mc_pr.obj"	"$(INTDIR)\mc_pr.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mc_read_tmplt_file.c

!IF  "$(CFG)" == "ipm - Win32 Release"


"$(INTDIR)\mc_read_tmplt_file.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ipm - Win32 Debug"


"$(INTDIR)\mc_read_tmplt_file.obj"	"$(INTDIR)\mc_read_tmplt_file.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mc_replace_p.c

!IF  "$(CFG)" == "ipm - Win32 Release"


"$(INTDIR)\mc_replace_p.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ipm - Win32 Debug"


"$(INTDIR)\mc_replace_p.obj"	"$(INTDIR)\mc_replace_p.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mc_string_grp_p.c

!IF  "$(CFG)" == "ipm - Win32 Release"


"$(INTDIR)\mc_string_grp_p.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ipm - Win32 Debug"


"$(INTDIR)\mc_string_grp_p.obj"	"$(INTDIR)\mc_string_grp_p.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mc_sub_dsp_proc.c

!IF  "$(CFG)" == "ipm - Win32 Release"


"$(INTDIR)\mc_sub_dsp_proc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ipm - Win32 Debug"


"$(INTDIR)\mc_sub_dsp_proc.obj"	"$(INTDIR)\mc_sub_dsp_proc.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mc_symbol_grp_p.c

!IF  "$(CFG)" == "ipm - Win32 Release"


"$(INTDIR)\mc_symbol_grp_p.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ipm - Win32 Debug"


"$(INTDIR)\mc_symbol_grp_p.obj"	"$(INTDIR)\mc_symbol_grp_p.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mc_symbol_p.c

!IF  "$(CFG)" == "ipm - Win32 Release"


"$(INTDIR)\mc_symbol_p.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ipm - Win32 Debug"


"$(INTDIR)\mc_symbol_p.obj"	"$(INTDIR)\mc_symbol_p.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mc_text_edit.c

!IF  "$(CFG)" == "ipm - Win32 Release"


"$(INTDIR)\mc_text_edit.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ipm - Win32 Debug"


"$(INTDIR)\mc_text_edit.obj"	"$(INTDIR)\mc_text_edit.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mc_toolkit.c

!IF  "$(CFG)" == "ipm - Win32 Release"


"$(INTDIR)\mc_toolkit.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ipm - Win32 Debug"


"$(INTDIR)\mc_toolkit.obj"	"$(INTDIR)\mc_toolkit.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mc_util_p.c

!IF  "$(CFG)" == "ipm - Win32 Release"


"$(INTDIR)\mc_util_p.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ipm - Win32 Debug"


"$(INTDIR)\mc_util_p.obj"	"$(INTDIR)\mc_util_p.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mc_view_p.c

!IF  "$(CFG)" == "ipm - Win32 Release"


"$(INTDIR)\mc_view_p.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ipm - Win32 Debug"


"$(INTDIR)\mc_view_p.obj"	"$(INTDIR)\mc_view_p.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\mc_zoom_p.c

!IF  "$(CFG)" == "ipm - Win32 Release"


"$(INTDIR)\mc_zoom_p.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "ipm - Win32 Debug"


"$(INTDIR)\mc_zoom_p.obj"	"$(INTDIR)\mc_zoom_p.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 


!ENDIF 

