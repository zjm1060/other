# Microsoft Developer Studio Project File - Name="oix" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=oix - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "oix.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "oix.mak" CFG="oix - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "oix - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "NCREMENTAL" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /fo"OIX.RES" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 User32.lib Advapi32.lib \home\ems\system\dms\v4.0a\lib\dblib.lib \home\ems\system\dps\v4.0a\lib\dpslib.lib \home\ems\system\dps\v4.0a\lib\ipclib.lib wsock32.lib \home\ems\system\lan\v4.0a\lib\lan.lib glib-2.0.lib gtk-win32-2.0.lib gdk-win32-2.0.lib gobject-2.0.lib gdk_pixbuf-2.0.lib gthread-2.0.lib gmodule-2.0.lib cairo.lib pangocairo-1.0.lib pango-1.0.lib intl.lib iconv.lib xml2.lib /nologo /subsystem:console /incremental:no /map /debug /debugtype:both /machine:I386 /force /pdbtype:sept
# SUBTRACT LINK32 /pdb:none
# Begin Target

# Name "oix - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\menusrc\callbacks.c
# End Source File
# Begin Source File

SOURCE=.\gtk_activate_p.c
# End Source File
# Begin Source File

SOURCE=.\gtk_alm_info_config.c
# End Source File
# Begin Source File

SOURCE=.\gtk_auto_check.c
# End Source File
# Begin Source File

SOURCE=.\gtk_bit_proc_util.c
# End Source File
# Begin Source File

SOURCE=.\gtk_color_grp_p.c
# End Source File
# Begin Source File

SOURCE=.\gtk_color_p.c
# End Source File
# Begin Source File

SOURCE=.\gtk_ctrl_menu_cb.c
# End Source File
# Begin Source File

SOURCE=.\gtk_curve_widget.c
# End Source File
# Begin Source File

SOURCE=.\gtk_db_tracing_p.c
# End Source File
# Begin Source File

SOURCE=.\gtk_del_hist_file.c
# End Source File
# Begin Source File

SOURCE=.\gtk_demo_dsp_p.c
# End Source File
# Begin Source File

SOURCE=.\gtk_draw_alm_str.c
# End Source File
# Begin Source File

SOURCE=.\gtk_draw_image.c
# End Source File
# Begin Source File

SOURCE=.\gtk_draw_util.c
# End Source File
# Begin Source File

SOURCE=.\gtk_event_p.c
# End Source File
# Begin Source File

SOURCE=.\gtk_fault_redisplay_unix.c
# End Source File
# Begin Source File

SOURCE=.\gtk_FetchSoftCopy.c
# End Source File
# Begin Source File

SOURCE=.\gtk_file_dlg.c
# End Source File
# Begin Source File

SOURCE=.\gtk_file_p.c
# End Source File
# Begin Source File

SOURCE=.\gtk_fill_p.c
# End Source File
# Begin Source File

SOURCE=.\gtk_form_cmd_pkt.c
# End Source File
# Begin Source File

SOURCE=.\gtk_gen_power_plan_curve.c
# End Source File
# Begin Source File

SOURCE=.\gtk_get_io_point_tab.c
# End Source File
# Begin Source File

SOURCE=.\gtk_getime_fromfile.c
# End Source File
# Begin Source File

SOURCE=.\gtk_hist_curve_unix.c
# End Source File
# Begin Source File

SOURCE=.\gtk_io_point_info.c
# End Source File
# Begin Source File

SOURCE=.\gtk_login_info_p.c
# End Source File
# Begin Source File

SOURCE=.\gtk_oix_init.c
# End Source File
# Begin Source File

SOURCE=.\gtk_op_charact_curve.c
# End Source File
# Begin Source File

SOURCE=.\gtk_oper_cond_p.c
# End Source File
# Begin Source File

SOURCE=.\gtk_passwd_p.c
# End Source File
# Begin Source File

SOURCE=.\gtk_pop_menu_real_curve.c
# End Source File
# Begin Source File

SOURCE=.\gtk_prev_next_dsp.c
# End Source File
# Begin Source File

SOURCE=.\gtk_print.c
# End Source File
# Begin Source File

SOURCE=.\gtk_prog_ctrl_p.c
# End Source File
# Begin Source File

SOURCE=.\gtk_prog_p.c
# End Source File
# Begin Source File

SOURCE=.\gtk_ProgAlarmTab.c
# End Source File
# Begin Source File

SOURCE=.\gtk_ProgAlarmTab_new.c
# End Source File
# Begin Source File

SOURCE=.\gtk_ProgDbTab.c
# End Source File
# Begin Source File

SOURCE=.\gtk_ProgKeySignalTab.c
# End Source File
# Begin Source File

SOURCE=.\gtk_ProgTraplog.c
# End Source File
# Begin Source File

SOURCE=.\gtk_pt_check.c
# End Source File
# Begin Source File

SOURCE=.\gtk_read_ftset.c
# End Source File
# Begin Source File

SOURCE=.\gtk_read_gdbase_p.c
# End Source File
# Begin Source File

SOURCE=.\gtk_read_rpt_hist.c
# End Source File
# Begin Source File

SOURCE=.\gtk_real_config.c
# End Source File
# Begin Source File

SOURCE=.\gtk_real_curve.c
# End Source File
# Begin Source File

SOURCE=.\gtk_rpt_p_win.c
# End Source File
# Begin Source File

SOURCE=.\gtk_search_win.c
# End Source File
# Begin Source File

SOURCE=.\gtk_SearchEntry.c
# End Source File
# Begin Source File

SOURCE=.\gtk_sgzy_dsp.c
# End Source File
# Begin Source File

SOURCE=.\gtk_sgzy_hist_dsp.c
# End Source File
# Begin Source File

SOURCE=.\gtk_softcopy.c
# End Source File
# Begin Source File

SOURCE=.\gtk_string_grp_p.c
# End Source File
# Begin Source File

SOURCE=.\gtk_sub_win.c
# End Source File
# Begin Source File

SOURCE=.\gtk_symbol_grp_p.c
# End Source File
# Begin Source File

SOURCE=.\gtk_symbol_p.c
# End Source File
# Begin Source File

SOURCE=.\gtk_tag.c
# End Source File
# Begin Source File

SOURCE=.\gtk_trend_dsp.c
# End Source File
# Begin Source File

SOURCE=.\gtk_trend_hist_dsp.c
# End Source File
# Begin Source File

SOURCE=.\gtk_turn_dsp_config_dlg.c
# End Source File
# Begin Source File

SOURCE=.\gtk_UnresetAlmTab.c
# End Source File
# Begin Source File

SOURCE=.\gtk_update_dsp.c
# End Source File
# Begin Source File

SOURCE=.\gtk_util_p.c
# End Source File
# Begin Source File

SOURCE=.\gtk_value_entry_p.c
# End Source File
# Begin Source File

SOURCE=.\gtk_view_p.c
# End Source File
# Begin Source File

SOURCE=.\gtk_write_db_unix.c
# End Source File
# Begin Source File

SOURCE=.\gtk_zoom_p.c
# End Source File
# Begin Source File

SOURCE=.\menusrc\interface.c
# End Source File
# Begin Source File

SOURCE=.\oix.c
# End Source File
# Begin Source File

SOURCE=.\menusrc\support.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\inc\alm_tab_def.h
# End Source File
# Begin Source File

SOURCE=..\inc\field_def.h
# End Source File
# Begin Source File

SOURCE=..\inc\fill_bitmap.h
# End Source File
# Begin Source File

SOURCE=..\inc\gtk_alarm_tab.h
# End Source File
# Begin Source File

SOURCE=..\inc\gtk_alarm_tabxx.h
# End Source File
# Begin Source File

SOURCE=..\inc\gtk_auto_check.h
# End Source File
# Begin Source File

SOURCE=..\inc\gtk_color_def.h
# End Source File
# Begin Source File

SOURCE=..\inc\gtk_color_grp_def.h
# End Source File
# Begin Source File

SOURCE=..\inc\gtk_curve_widget.h
# End Source File
# Begin Source File

SOURCE=..\inc\gtk_db_tab_def.h
# End Source File
# Begin Source File

SOURCE=..\inc\gtk_draw_def.h
# End Source File
# Begin Source File

SOURCE=..\inc\gtk_fault_alm_tab.h
# End Source File
# Begin Source File

SOURCE=..\inc\gtk_fill_bitmap.h
# End Source File
# Begin Source File

SOURCE=..\inc\gtk_ipm_k_def.h
# End Source File
# Begin Source File

SOURCE=..\inc\gtk_log_def.h
# End Source File
# Begin Source File

SOURCE=..\inc\gtk_msg_text_def.h
# End Source File
# Begin Source File

SOURCE=..\inc\gtk_oix_cstr.h
# End Source File
# Begin Source File

SOURCE=..\inc\gtk_ProgAlarmTab_new.h
# End Source File
# Begin Source File

SOURCE=..\inc\gtk_real_curve.h
# End Source File
# Begin Source File

SOURCE=..\inc\gtk_search_data.h
# End Source File
# Begin Source File

SOURCE=..\inc\gtk_sgzy_def.h
# End Source File
# Begin Source File

SOURCE=..\inc\gtk_sub_dsp_def.h
# End Source File
# Begin Source File

SOURCE=..\inc\gtk_text_edit_def.h
# End Source File
# Begin Source File

SOURCE=..\inc\gtk_view_def.h
# End Source File
# Begin Source File

SOURCE=..\inc\gtk_widgets.h
# End Source File
# Begin Source File

SOURCE=..\inc\menu_def.h
# End Source File
# Begin Source File

SOURCE=..\inc\mode_def.h
# End Source File
# Begin Source File

SOURCE=..\inc\oix.h
# End Source File
# Begin Source File

SOURCE=..\inc\oix_common.h
# End Source File
# Begin Source File

SOURCE=..\inc\popmenu.h
# End Source File
# Begin Source File

SOURCE=.\menusrc\support.h
# End Source File
# Begin Source File

SOURCE=..\inc\symbol_def.h
# End Source File
# Begin Source File

SOURCE=..\inc\xgif.h
# End Source File
# Begin Source File

SOURCE=..\inc\zoom_def.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\oix.ico
# End Source File
# Begin Source File

SOURCE=.\oix.rc
# ADD BASE RSC /l 0x804
# ADD RSC /l 0x804
# End Source File
# End Group
# End Target
# End Project
