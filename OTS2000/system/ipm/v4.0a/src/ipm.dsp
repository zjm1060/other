# Microsoft Developer Studio Project File - Name="ipm" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ipm - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ipm.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ipm.mak" CFG="ipm - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ipm - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WinDebug"
# PROP BASE Intermediate_Dir "WinDebug"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# ADD BASE CPP /nologo /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /MTd /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 ..\..\..\dms\v4.0a\lib\dblib.lib ..\..\..\lan\v4.0a\lib\lan.lib ..\..\..\dps\v4.0a\lib\ipclib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libc.lib"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PreLink_Cmds=Put_SVNNum_to_exe.bat ipm
# End Special Build Tool
# Begin Target

# Name "ipm - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\ipm.rc
# End Source File
# Begin Source File

SOURCE=.\mc_attribute.c
DEP_CPP_MC_AT=\
	"..\..\..\dms\v4.0a\inc\dms_com.h"\
	"..\..\..\dms\v4.0a\inc\dms_data_def.h"\
	"..\..\..\dms\v4.0a\inc\dms_functions.h"\
	"..\..\..\dms\v4.0a\inc\dms_std.h"\
	"..\..\..\dms\v4.0a\inc\dms_structure_def.h"\
	"..\..\..\dms\v4.0a\inc\dms_util_def.h"\
	"..\..\..\dms\v4.0a\inc\ems_init_def.h"\
	"..\..\..\dms\v4.0a\inc\os_type_def.h"\
	"..\inc\color_grp_def.h"\
	"..\inc\draw_def.h"\
	"..\inc\dyn_form_def.h"\
	"..\inc\dyn_prog_def.h"\
	"..\inc\ipm.h"\
	"..\INC\ipm_k_def.h"\
	"..\inc\menu_def.h"\
	"..\inc\mode_def.h"\
	"..\inc\pr.h"\
	"..\inc\string_grp_def.h"\
	"..\inc\symbol_def.h"\
	"..\inc\symbol_grp_def.h"\
	"..\inc\tmplt.h"\
	"..\inc\toolkit.h"\
	"..\inc\view_def.h"\
	"..\inc\zoom_def.h"\
	
# End Source File
# Begin Source File

SOURCE=.\mc_color_grp_p.c
DEP_CPP_MC_CO=\
	"..\inc\color_def.h"\
	"..\inc\color_grp_def.h"\
	"..\inc\draw_def.h"\
	"..\inc\ipm.h"\
	"..\INC\ipm_k_def.h"\
	"..\inc\mode_def.h"\
	"..\inc\symbol_def.h"\
	
# End Source File
# Begin Source File

SOURCE=.\mc_color_p.c
DEP_CPP_MC_COL=\
	"..\inc\color_def.h"\
	"..\inc\draw_def.h"\
	"..\inc\ipm.h"\
	"..\INC\ipm_k_def.h"\
	"..\inc\symbol_def.h"\
	"..\inc\toolkit.h"\
	
# End Source File
# Begin Source File

SOURCE=.\mc_draw_p.c
DEP_CPP_MC_DR=\
	"..\inc\draw_def.h"\
	"..\inc\grid_def.h"\
	"..\inc\ipm.h"\
	"..\INC\ipm_k_def.h"\
	"..\inc\layer_def.h"\
	"..\inc\mode_def.h"\
	"..\inc\SCALER.H"\
	"..\inc\symbol_def.h"\
	"..\inc\text_edit_def.h"\
	"..\inc\zoom_def.h"\
	
# End Source File
# Begin Source File

SOURCE=.\mc_draw_util.c
DEP_CPP_MC_DRA=\
	"..\inc\color_def.h"\
	"..\inc\draw_def.h"\
	"..\inc\dyn_form_def.h"\
	"..\inc\fill_bitmap.h"\
	"..\inc\ipm.h"\
	"..\INC\ipm_k_def.h"\
	"..\inc\layer_def.h"\
	"..\inc\pr.h"\
	"..\inc\SCALER.H"\
	"..\inc\symbol_def.h"\
	"..\inc\text_edit_def.h"\
	"..\inc\zoom_def.h"\
	
# End Source File
# Begin Source File

SOURCE=.\mc_dyn_form_p.c
DEP_CPP_MC_DY=\
	"..\inc\color_grp_def.h"\
	"..\inc\draw_def.h"\
	"..\inc\dyn_form_def.h"\
	"..\inc\dyn_prog_def.h"\
	"..\inc\field_def.h"\
	"..\inc\ipm.h"\
	"..\INC\ipm_k_def.h"\
	"..\inc\menu_def.h"\
	"..\inc\string_grp_def.h"\
	"..\inc\symbol_def.h"\
	"..\inc\symbol_grp_def.h"\
	"..\inc\tmplt.h"\
	
# End Source File
# Begin Source File

SOURCE=.\mc_dyn_handler.c
DEP_CPP_MC_DYN=\
	"..\inc\draw_def.h"\
	"..\inc\dyn_form_def.h"\
	"..\inc\dyn_prog_def.h"\
	"..\INC\ipm_k_def.h"\
	"..\inc\menu_def.h"\
	"..\inc\tmplt.h"\
	
# End Source File
# Begin Source File

SOURCE=.\mc_dyn_test.c
DEP_CPP_MC_DYN_=\
	"..\..\..\dms\v4.0a\inc\dms_com.h"\
	"..\..\..\dms\v4.0a\inc\dms_data_def.h"\
	"..\..\..\dms\v4.0a\inc\dms_functions.h"\
	"..\..\..\dms\v4.0a\inc\dms_std.h"\
	"..\..\..\dms\v4.0a\inc\dms_structure_def.h"\
	"..\..\..\dms\v4.0a\inc\dms_util_def.h"\
	"..\..\..\dms\v4.0a\inc\ems_init_def.h"\
	"..\..\..\dms\v4.0a\inc\os_type_def.h"\
	"..\inc\draw_def.h"\
	"..\INC\ipm_k_def.h"\
	
# End Source File
# Begin Source File

SOURCE=.\mc_dyn_util.c
DEP_CPP_MC_DYN_U=\
	"..\inc\draw_def.h"\
	"..\inc\dyn_form_def.h"\
	"..\inc\field_def.h"\
	"..\inc\ipm.h"\
	"..\INC\ipm_k_def.h"\
	"..\inc\SCALER.H"\
	"..\inc\symbol_def.h"\
	"..\inc\tmplt.h"\
	
# End Source File
# Begin Source File

SOURCE=.\mc_edit_p.c
DEP_CPP_MC_ED=\
	"..\inc\draw_def.h"\
	"..\inc\ipm.h"\
	"..\INC\ipm_k_def.h"\
	"..\inc\symbol_def.h"\
	
# End Source File
# Begin Source File

SOURCE=.\mc_file_p.c
DEP_CPP_MC_FI=\
	"..\inc\cderr.h"\
	"..\inc\draw_def.h"\
	"..\inc\dyn_form_def.h"\
	"..\inc\ipm.h"\
	"..\INC\ipm_k_def.h"\
	"..\inc\layer_def.h"\
	"..\inc\menu_def.h"\
	"..\inc\mode_def.h"\
	"..\inc\sub_dsp_def.h"\
	"..\inc\symbol_def.h"\
	
# End Source File
# Begin Source File

SOURCE=.\mc_file_search.c
DEP_CPP_MC_FIL=\
	"..\..\..\dms\v4.0a\inc\dms_com.h"\
	"..\..\..\dms\v4.0a\inc\dms_data_def.h"\
	"..\..\..\dms\v4.0a\inc\dms_functions.h"\
	"..\..\..\dms\v4.0a\inc\dms_std.h"\
	"..\..\..\dms\v4.0a\inc\dms_structure_def.h"\
	"..\..\..\dms\v4.0a\inc\dms_util_def.h"\
	"..\..\..\dms\v4.0a\inc\ems_init_def.h"\
	"..\..\..\dms\v4.0a\inc\os_type_def.h"\
	"..\inc\ipm.h"\
	"..\INC\ipm_k_def.h"\
	"..\inc\symbol_def.h"\
	
# End Source File
# Begin Source File

SOURCE=.\mc_fill_p.c
DEP_CPP_MC_FILL=\
	"..\inc\draw_def.h"\
	"..\inc\fill_bitmap.h"\
	"..\inc\ipm.h"\
	"..\INC\ipm_k_def.h"\
	"..\inc\symbol_def.h"\
	
# End Source File
# Begin Source File

SOURCE=.\mc_find_paths.c
# End Source File
# Begin Source File

SOURCE=.\mc_font_p.c
DEP_CPP_MC_FO=\
	"..\inc\draw_def.h"\
	"..\inc\ipm.h"\
	"..\INC\ipm_k_def.h"\
	"..\inc\symbol_def.h"\
	"..\inc\text_edit_def.h"\
	
# End Source File
# Begin Source File

SOURCE=.\mc_format_p.c
DEP_CPP_MC_FOR=\
	"..\inc\draw_def.h"\
	"..\inc\dyn_form_def.h"\
	"..\inc\field_def.h"\
	"..\inc\ipm.h"\
	"..\INC\ipm_k_def.h"\
	"..\inc\symbol_def.h"\
	"..\inc\tmplt.h"\
	
# End Source File
# Begin Source File

SOURCE=.\mc_get_text_p.c
DEP_CPP_MC_GE=\
	"..\inc\color_grp_def.h"\
	"..\inc\ipm.h"\
	"..\INC\ipm_k_def.h"\
	"..\inc\menu_def.h"\
	"..\inc\mode_def.h"\
	"..\inc\string_grp_def.h"\
	"..\inc\symbol_def.h"\
	"..\inc\symbol_grp_def.h"\
	
# End Source File
# Begin Source File

SOURCE=.\mc_grid_p.c
DEP_CPP_MC_GR=\
	"..\inc\color_def.h"\
	"..\inc\draw_def.h"\
	"..\inc\grid_def.h"\
	"..\inc\ipm.h"\
	"..\INC\ipm_k_def.h"\
	"..\inc\mode_def.h"\
	"..\inc\sub_dsp_def.h"\
	"..\inc\symbol_def.h"\
	
# End Source File
# Begin Source File

SOURCE=.\mc_ipm.c
DEP_CPP_MC_IP=\
	"..\..\..\dms\v4.0a\inc\dms_com.h"\
	"..\..\..\dms\v4.0a\inc\dms_data_def.h"\
	"..\..\..\dms\v4.0a\inc\dms_functions.h"\
	"..\..\..\dms\v4.0a\inc\dms_std.h"\
	"..\..\..\dms\v4.0a\inc\dms_structure_def.h"\
	"..\..\..\dms\v4.0a\inc\dms_util_def.h"\
	"..\..\..\dms\v4.0a\inc\ems_init_def.h"\
	"..\..\..\dms\v4.0a\inc\os_type_def.h"\
	"..\inc\color_def.h"\
	"..\inc\color_grp_def.h"\
	"..\inc\draw_def.h"\
	"..\inc\fill_bitmap.h"\
	"..\inc\ipm.h"\
	"..\INC\ipm_k_def.h"\
	"..\inc\mode_def.h"\
	"..\inc\SCALER.H"\
	"..\inc\string_grp_def.h"\
	"..\inc\symbol_def.h"\
	"..\inc\symbol_grp_def.h"\
	"..\inc\text_edit_def.h"\
	"..\inc\toolkit.h"\
	"..\inc\view_def.h"\
	"..\inc\zoom_def.h"\
	
# End Source File
# Begin Source File

SOURCE=.\mc_ipm_init.c
DEP_CPP_MC_IPM=\
	"..\inc\color_def.h"\
	"..\inc\draw_def.h"\
	"..\inc\ipm.h"\
	"..\INC\ipm_k_def.h"\
	"..\inc\layer_def.h"\
	"..\inc\mode_def.h"\
	"..\inc\symbol_def.h"\
	"..\inc\text_edit_def.h"\
	"..\inc\zoom_def.h"\
	
# End Source File
# Begin Source File

SOURCE=.\mc_ipmmenu.c
DEP_CPP_MC_IPMM=\
	"..\inc\color_grp_def.h"\
	"..\inc\draw_def.h"\
	"..\inc\ipm.h"\
	"..\INC\ipm_k_def.h"\
	"..\inc\menu_def.h"\
	"..\inc\mode_def.h"\
	"..\inc\pr.h"\
	"..\inc\string_grp_def.h"\
	"..\inc\symbol_def.h"\
	"..\inc\symbol_grp_def.h"\
	"..\inc\toolkit.h"\
	"..\inc\view_def.h"\
	"..\inc\zoom_def.h"\
	
# End Source File
# Begin Source File

SOURCE=.\mc_layer_p.c
DEP_CPP_MC_LA=\
	"..\inc\ipm.h"\
	"..\INC\ipm_k_def.h"\
	"..\inc\layer_def.h"\
	"..\inc\symbol_def.h"\
	"..\inc\zoom_def.h"\
	
# End Source File
# Begin Source File

SOURCE=.\mc_line_width_p.c
DEP_CPP_MC_LI=\
	"..\inc\draw_def.h"\
	"..\inc\ipm.h"\
	"..\INC\ipm_k_def.h"\
	"..\inc\symbol_def.h"\
	"..\inc\toolkit.h"\
	
# End Source File
# Begin Source File

SOURCE=.\mc_menu_edit_p.c
DEP_CPP_MC_ME=\
	"..\inc\draw_def.h"\
	"..\inc\ipm.h"\
	"..\INC\ipm_k_def.h"\
	"..\inc\menu_def.h"\
	"..\inc\mode_def.h"\
	"..\inc\SCALER.H"\
	"..\inc\symbol_def.h"\
	
# End Source File
# Begin Source File

SOURCE=.\mc_parse_pattern.c
DEP_CPP_MC_PA=\
	"..\inc\string_pattern.h"\
	
# End Source File
# Begin Source File

SOURCE=.\mc_pr.c
DEP_CPP_MC_PR=\
	"..\inc\color_def.h"\
	"..\inc\ipm.h"\
	"..\INC\ipm_k_def.h"\
	"..\inc\pr.h"\
	"..\inc\SCALER.H"\
	"..\inc\symbol_def.h"\
	"..\inc\text_edit_def.h"\
	
# End Source File
# Begin Source File

SOURCE=.\mc_read_tmplt_file.c
DEP_CPP_MC_RE=\
	"..\inc\ipm.h"\
	"..\INC\ipm_k_def.h"\
	"..\inc\symbol_def.h"\
	"..\inc\tmplt.h"\
	
# End Source File
# Begin Source File

SOURCE=.\mc_replace_p.c
DEP_CPP_MC_REP=\
	"..\inc\draw_def.h"\
	"..\inc\field_def.h"\
	"..\inc\ipm.h"\
	"..\INC\ipm_k_def.h"\
	"..\inc\string_pattern.h"\
	"..\inc\symbol_def.h"\
	
# End Source File
# Begin Source File

SOURCE=.\mc_string_grp_p.c
DEP_CPP_MC_ST=\
	"..\inc\ipm.h"\
	"..\INC\ipm_k_def.h"\
	"..\inc\mode_def.h"\
	"..\inc\string_grp_def.h"\
	"..\inc\symbol_def.h"\
	
# End Source File
# Begin Source File

SOURCE=.\mc_sub_dsp_proc.c
DEP_CPP_MC_SU=\
	"..\inc\ipm.h"\
	"..\INC\ipm_k_def.h"\
	"..\inc\mode_def.h"\
	"..\inc\sub_dsp_def.h"\
	"..\inc\symbol_def.h"\
	
# End Source File
# Begin Source File

SOURCE=.\mc_symbol_grp_p.c
DEP_CPP_MC_SY=\
	"..\inc\ipm.h"\
	"..\INC\ipm_k_def.h"\
	"..\inc\mode_def.h"\
	"..\inc\SCALER.H"\
	"..\inc\symbol_def.h"\
	"..\inc\symbol_grp_def.h"\
	
# End Source File
# Begin Source File

SOURCE=.\mc_symbol_p.c
DEP_CPP_MC_SYM=\
	"..\inc\color_def.h"\
	"..\inc\draw_def.h"\
	"..\inc\grid_def.h"\
	"..\inc\ipm.h"\
	"..\INC\ipm_k_def.h"\
	"..\inc\mode_def.h"\
	"..\inc\SCALER.H"\
	"..\inc\symbol_def.h"\
	"..\inc\toolkit.h"\
	"..\inc\zoom_def.h"\
	
# End Source File
# Begin Source File

SOURCE=.\mc_text_edit.c
DEP_CPP_MC_TE=\
	"..\inc\color_def.h"\
	"..\inc\draw_def.h"\
	"..\inc\ipm.h"\
	"..\INC\ipm_k_def.h"\
	"..\inc\symbol_def.h"\
	"..\inc\text_edit_def.h"\
	
# End Source File
# Begin Source File

SOURCE=.\mc_toolkit.c
DEP_CPP_MC_TO=\
	"..\inc\draw_def.h"\
	"..\inc\fill_bitmap.h"\
	"..\inc\ipm.h"\
	"..\INC\ipm_k_def.h"\
	"..\inc\mode_def.h"\
	"..\inc\symbol_def.h"\
	"..\inc\text_edit_def.h"\
	"..\inc\toolkit.h"\
	
# End Source File
# Begin Source File

SOURCE=.\mc_util_p.c
# End Source File
# Begin Source File

SOURCE=.\mc_view_p.c
DEP_CPP_MC_VI=\
	"..\inc\color_def.h"\
	"..\inc\draw_def.h"\
	"..\inc\ipm.h"\
	"..\INC\ipm_k_def.h"\
	"..\inc\SCALER.H"\
	"..\inc\symbol_def.h"\
	"..\inc\view_def.h"\
	"..\inc\zoom_def.h"\
	
# End Source File
# Begin Source File

SOURCE=.\mc_zoom_p.c
DEP_CPP_MC_ZO=\
	"..\inc\draw_def.h"\
	"..\INC\ipm_k_def.h"\
	"..\inc\mode_def.h"\
	"..\inc\SCALER.H"\
	"..\inc\view_def.h"\
	"..\inc\zoom_def.h"\
	
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\DD.H
# End Source File
# Begin Source File

SOURCE=..\inc\dyn_prog_def.h
# End Source File
# Begin Source File

SOURCE=..\INC\ipm_k_def.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=..\inc\SCALER.H
# End Source File
# Begin Source File

SOURCE="C:\Program Files\Microsoft Visual Studio\VC98\Include\WINUSER.H"
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\arrow_i.cur
# End Source File
# Begin Source File

SOURCE=.\arrow_l.cur
# End Source File
# Begin Source File

SOURCE=.\arrow_m.cur
# End Source File
# Begin Source File

SOURCE=.\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp_line.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp_text.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp_zoom.bmp
# End Source File
# Begin Source File

SOURCE=.\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\ipm.ico
# End Source File
# Begin Source File

SOURCE=.\LIST.BMP
# End Source File
# Begin Source File

SOURCE=.\lmove.cur
# End Source File
# Begin Source File

SOURCE=.\lnwdth1.bmp
# End Source File
# Begin Source File

SOURCE=.\lnwdth2.bmp
# End Source File
# Begin Source File

SOURCE=.\lnwdth3.bmp
# End Source File
# Begin Source File

SOURCE=.\lnwdth4.bmp
# End Source File
# Begin Source File

SOURCE=.\lnwdth5.bmp
# End Source File
# Begin Source File

SOURCE=.\lnwdth6.bmp
# End Source File
# Begin Source File

SOURCE=.\Pan_Grab.cur
# End Source File
# Begin Source File

SOURCE=.\Pan_Open.cur
# End Source File
# Begin Source File

SOURCE=.\pen_im.cur
# End Source File
# Begin Source File

SOURCE=.\SARROWS.CUR
# End Source File
# Begin Source File

SOURCE=.\SPLITH.CUR
# End Source File
# Begin Source File

SOURCE=.\SPLITV.CUR
# End Source File
# Begin Source File

SOURCE=.\tkarcd.bmp
# End Source File
# Begin Source File

SOURCE=.\tkarcu.bmp
# End Source File
# Begin Source File

SOURCE=.\tkcircled.bmp
# End Source File
# Begin Source File

SOURCE=.\tkcircleu.bmp
# End Source File
# Begin Source File

SOURCE=.\tkgifd.bmp
# End Source File
# Begin Source File

SOURCE=.\tkgifu.bmp
# End Source File
# Begin Source File

SOURCE=.\tklined.bmp
# End Source File
# Begin Source File

SOURCE=.\tklinedDash.bmp
# End Source File
# Begin Source File

SOURCE=.\tklineu.bmp
# End Source File
# Begin Source File

SOURCE=.\tklineuDash.bmp
# End Source File
# Begin Source File

SOURCE=.\tkplylnd.bmp
# End Source File
# Begin Source File

SOURCE=.\tkplylnu.bmp
# End Source File
# Begin Source File

SOURCE=.\tkrectd.bmp
# End Source File
# Begin Source File

SOURCE=.\tkrectu.bmp
# End Source File
# Begin Source File

SOURCE=.\tkselectd.bmp
# End Source File
# Begin Source File

SOURCE=.\tkselectu.bmp
# End Source File
# Begin Source File

SOURCE=.\tksymd.bmp
# End Source File
# Begin Source File

SOURCE=.\tksymu.bmp
# End Source File
# Begin Source File

SOURCE=.\tktextd.bmp
# End Source File
# Begin Source File

SOURCE=.\tktextu.bmp
# End Source File
# Begin Source File

SOURCE=.\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\TRCKNESW.CUR
# End Source File
# Begin Source File

SOURCE=.\TRCKNS.CUR
# End Source File
# Begin Source File

SOURCE=.\TRCKNWSE.CUR
# End Source File
# Begin Source File

SOURCE=.\TRCKWE.CUR
# End Source File
# Begin Source File

SOURCE=.\TREEVIEW.BMP
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\dms\v4.0a\inc\dms_functions.h
# End Source File
# End Target
# End Project
