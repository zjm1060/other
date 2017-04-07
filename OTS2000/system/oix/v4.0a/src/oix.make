## -*- Makefile -*-
##
## User: ems
## Time: 2006-6-29 11:17:38
## Makefile created by the Native Languages Module.
##
## This file is generated automatically -- Changes will be lost if regenerated
##


#### Compiler and tool definitions shared by all build targets  gthread-2.0  #####
include ../../../global.make

ifeq ($(UNAME), SunOS)
CC=gcc

COMPILE = $(CC)
LINK = $(COMPILE) $(LDFLAGS) $(TARGET ARCH)
endif

LINKOPTS = `pkg-config gtk+-2.0 --libs` `pkg-config libxml-2.0 --libs`
CFLAGS = $(BASICOPTS) -c `pkg-config gtk+-2.0 --cflags` `pkg-config libxml-2.0 --cflags`

# Define the target directories.
TARGETDIR=.
TARGET=oix.$(SUFFIX)

all: $(TARGETDIR)/$(TARGET)

CPPFLAGS = \
	-I/OIXINC \
	-I/DPSINC \
	-I/HDBINC \
	-I/LANINC \
	-I/PASINC \
	-I/DBINC
	
OBJS =  \
	$(TARGETDIR)/gtk_del_hist_file.o \
	$(TARGETDIR)/gtk_activate_p.o \
	$(TARGETDIR)/gtk_alm_info_config.o \
	$(TARGETDIR)/gtk_auto_check.o \
	$(TARGETDIR)/gtk_color_grp_p.o \
	$(TARGETDIR)/gtk_color_p.o \
	$(TARGETDIR)/gtk_ctrl_menu_cb.o \
	$(TARGETDIR)/gtk_db_tracing_p.o \
	$(TARGETDIR)/gtk_demo_dsp_p.o \
	$(TARGETDIR)/gtk_draw_alm_str.o \
	$(TARGETDIR)/gtk_draw_util.o \
	$(TARGETDIR)/gtk_event_p.o \
	$(TARGETDIR)/gtk_fault_redisplay_unix.o \
	$(TARGETDIR)/gtk_FetchSoftCopy.o \
	$(TARGETDIR)/gtk_file_dlg.o \
	$(TARGETDIR)/gtk_file_p.o \
	$(TARGETDIR)/gtk_fill_p.o \
	$(TARGETDIR)/gtk_form_cmd_pkt.o \
	$(TARGETDIR)/gtk_get_io_point_tab.o \
	$(TARGETDIR)/gtk_hist_curve_unix.o \
	$(TARGETDIR)/gtk_real_curve.o \
	$(TARGETDIR)/gtk_io_point_info.o \
	$(TARGETDIR)/gtk_login_info_p.o \
	$(TARGETDIR)/gtk_oix_init.o \
	$(TARGETDIR)/gtk_oper_cond_p.o \
	$(TARGETDIR)/gtk_passwd_p.o \
	$(TARGETDIR)/gtk_prev_next_dsp.o \
	$(TARGETDIR)/gtk_prog_ctrl_p.o \
	$(TARGETDIR)/gtk_prog_p.o \
	$(TARGETDIR)/gtk_ProgAlarmTab.o \
	$(TARGETDIR)/gtk_ProgAlarmTab_new.o \
	$(TARGETDIR)/gtk_ProgKeySignalTab.o \
	$(TARGETDIR)/gtk_ProgDbTab.o \
	$(TARGETDIR)/gtk_ProgTraplog.o \
	$(TARGETDIR)/gtk_read_ftset.o \
	$(TARGETDIR)/gtk_read_gdbase_p.o \
	$(TARGETDIR)/gtk_rpt_p_unix.o \
	$(TARGETDIR)/gtk_real_config.o \
	$(TARGETDIR)/gtk_SearchEntry.o \
	$(TARGETDIR)/gtk_search_win.o \
	$(TARGETDIR)/gtk_sgzy_dsp.o \
	$(TARGETDIR)/gtk_sgzy_hist_dsp.o \
	$(TARGETDIR)/gtk_softcopy.o \
	$(TARGETDIR)/gtk_string_grp_p.o \
	$(TARGETDIR)/gtk_symbol_grp_p.o \
	$(TARGETDIR)/gtk_symbol_p.o \
	$(TARGETDIR)/gtk_tag.o \
	$(TARGETDIR)/gtk_trend_dsp.o \
	$(TARGETDIR)/gtk_trend_hist_dsp.o \
	$(TARGETDIR)/gtk_UnresetAlmTab.o \
	$(TARGETDIR)/gtk_update_dsp.o \
	$(TARGETDIR)/gtk_value_entry_p.o \
	$(TARGETDIR)/gtk_zoom_p.o \
	$(TARGETDIR)/gtk_draw_image.o\
	$(TARGETDIR)/gtk_sub_win.o\
	$(TARGETDIR)/oix.o \
	$(TARGETDIR)/gtk_write_db_unix.o \
	$(TARGETDIR)/gtk_turn_dsp_config_dlg.o \
	$(TARGETDIR)/gtk_read_rpt_hist.o \
	$(TARGETDIR)/support.o\
	$(TARGETDIR)/interface.o\
	$(TARGETDIR)/callbacks.o\
	$(TARGETDIR)/gtk_bit_proc_util.o\
	$(TARGETDIR)/gtk_getime_fromfile.o\
	$(TARGETDIR)/gtk_curve_widget.o\
	$(TARGETDIR)/gtk_util_p.o \
	$(TARGETDIR)/gtk_view_p.o	\
	$(TARGETDIR)/gtk_gen_power_plan_curve.o	\
	$(TARGETDIR)/gtk_op_charact_curve.o
	
USERLIBS =  ../../../dps/v4.0a/lib/dpslib.lib ../../../lan/v4.0a/lib/lan.lib ../../../dms/v4.0a/lib/dblib.lib ../../../dps/v4.0a/lib/ipclib.lib $(SYSLIBS) 
DEPLIBS =    
LDLIBS = $(USERLIBS)

# Link or archive  ../lib/util.lib
$(TARGETDIR)/$(TARGET): $(TARGETDIR) $(OBJS) $(DEPLIBS)
	$(LINK) $(LINKOPTS) $(CPPFLAGS) -o $@ $(OBJS) $(LDLIBS)


# Compile source files into .o files
$(TARGETDIR)/gtk_del_hist_file.o: $(TARGETDIR) gtk_del_hist_file.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_del_hist_file.c

$(TARGETDIR)/gtk_activate_p.o: $(TARGETDIR) gtk_activate_p.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_activate_p.c

$(TARGETDIR)/gtk_alm_info_config.o: $(TARGETDIR) gtk_alm_info_config.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_alm_info_config.c

$(TARGETDIR)/gtk_auto_check.o: $(TARGETDIR) gtk_auto_check.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_auto_check.c

$(TARGETDIR)/gtk_color_grp_p.o: $(TARGETDIR) gtk_color_grp_p.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_color_grp_p.c

$(TARGETDIR)/gtk_color_p.o: $(TARGETDIR) gtk_color_p.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_color_p.c

$(TARGETDIR)/gtk_ctrl_menu_cb.o: $(TARGETDIR) gtk_ctrl_menu_cb.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_ctrl_menu_cb.c

$(TARGETDIR)/gtk_db_tracing_p.o: $(TARGETDIR) gtk_db_tracing_p.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_db_tracing_p.c

$(TARGETDIR)/gtk_demo_dsp_p.o: $(TARGETDIR) gtk_demo_dsp_p.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_demo_dsp_p.c

$(TARGETDIR)/gtk_draw_alm_str.o: $(TARGETDIR) gtk_draw_alm_str.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_draw_alm_str.c

$(TARGETDIR)/gtk_draw_util.o: $(TARGETDIR) gtk_draw_util.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_draw_util.c

$(TARGETDIR)/gtk_event_p.o: $(TARGETDIR) gtk_event_p.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_event_p.c

$(TARGETDIR)/gtk_fault_redisplay_unix.o: $(TARGETDIR) gtk_fault_redisplay_unix.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_fault_redisplay_unix.c

$(TARGETDIR)/gtk_FetchSoftCopy.o: $(TARGETDIR) gtk_FetchSoftCopy.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_FetchSoftCopy.c

$(TARGETDIR)/gtk_file_dlg.o: $(TARGETDIR) gtk_file_dlg.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_file_dlg.c

$(TARGETDIR)/gtk_file_p.o: $(TARGETDIR) gtk_file_p.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_file_p.c

$(TARGETDIR)/gtk_fill_p.o: $(TARGETDIR) gtk_fill_p.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_fill_p.c

$(TARGETDIR)/gtk_form_cmd_pkt.o: $(TARGETDIR) gtk_form_cmd_pkt.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_form_cmd_pkt.c

$(TARGETDIR)/gtk_get_io_point_tab.o: $(TARGETDIR) gtk_get_io_point_tab.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_get_io_point_tab.c

$(TARGETDIR)/gtk_hist_curve_unix.o: $(TARGETDIR) gtk_hist_curve_unix.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_hist_curve_unix.c

$(TARGETDIR)/gtk_real_curve.o: $(TARGETDIR) gtk_real_curve.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_real_curve.c

$(TARGETDIR)/gtk_HostInit.o: $(TARGETDIR) gtk_HostInit.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_HostInit.c

$(TARGETDIR)/gtk_io_point_info.o: $(TARGETDIR) gtk_io_point_info.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_io_point_info.c

$(TARGETDIR)/gtk_login_info_p.o: $(TARGETDIR) gtk_login_info_p.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_login_info_p.c

$(TARGETDIR)/gtk_oix_init.o: $(TARGETDIR) gtk_oix_init.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_oix_init.c

$(TARGETDIR)/gtk_oper_cond_p.o: $(TARGETDIR) gtk_oper_cond_p.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_oper_cond_p.c

$(TARGETDIR)/gtk_passwd_p.o: $(TARGETDIR) gtk_passwd_p.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_passwd_p.c

$(TARGETDIR)/gtk_prev_next_dsp.o: $(TARGETDIR) gtk_prev_next_dsp.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_prev_next_dsp.c

$(TARGETDIR)/gtk_prog_ctrl_p.o: $(TARGETDIR) gtk_prog_ctrl_p.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_prog_ctrl_p.c

$(TARGETDIR)/gtk_prog_p.o: $(TARGETDIR) gtk_prog_p.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_prog_p.c

$(TARGETDIR)/gtk_ProgAlarmTab.o: $(TARGETDIR) gtk_ProgAlarmTab.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_ProgAlarmTab.c

$(TARGETDIR)/gtk_ProgAlarmTab_new.o: $(TARGETDIR) gtk_ProgAlarmTab_new.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_ProgAlarmTab_new.c

$(TARGETDIR)/gtk_ProgKeySignalTab.o: $(TARGETDIR) gtk_ProgKeySignalTab.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_ProgKeySignalTab.c

$(TARGETDIR)/gtk_ProgDbTab.o: $(TARGETDIR) gtk_ProgDbTab.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_ProgDbTab.c

$(TARGETDIR)/gtk_ProgTraplog.o: $(TARGETDIR) gtk_ProgTraplog.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_ProgTraplog.c

$(TARGETDIR)/gtk_read_ftset.o: $(TARGETDIR) gtk_read_ftset.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_read_ftset.c

$(TARGETDIR)/gtk_read_gdbase_p.o: $(TARGETDIR) gtk_read_gdbase_p.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_read_gdbase_p.c

$(TARGETDIR)/gtk_rpt_p_unix.o: $(TARGETDIR) gtk_rpt_p_unix.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_rpt_p_unix.c
	
$(TARGETDIR)/gtk_real_config.o: $(TARGETDIR) gtk_real_config.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_real_config.c


$(TARGETDIR)/gtk_SearchEntry.o: $(TARGETDIR) gtk_SearchEntry.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_SearchEntry.c

$(TARGETDIR)/gtk_search_win.o: $(TARGETDIR) gtk_search_win.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_search_win.c

$(TARGETDIR)/gtk_sgzy_dsp.o: $(TARGETDIR) gtk_sgzy_dsp.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_sgzy_dsp.c

$(TARGETDIR)/gtk_sgzy_hist_dsp.o: $(TARGETDIR) gtk_sgzy_hist_dsp.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_sgzy_hist_dsp.c

$(TARGETDIR)/gtk_softcopy.o: $(TARGETDIR) gtk_softcopy.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_softcopy.c

$(TARGETDIR)/gtk_string_grp_p.o: $(TARGETDIR) gtk_string_grp_p.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_string_grp_p.c

$(TARGETDIR)/gtk_symbol_grp_p.o: $(TARGETDIR) gtk_symbol_grp_p.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_symbol_grp_p.c

$(TARGETDIR)/gtk_symbol_p.o: $(TARGETDIR) gtk_symbol_p.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_symbol_p.c

$(TARGETDIR)/gtk_tag.o: $(TARGETDIR) gtk_tag.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_tag.c

$(TARGETDIR)/gtk_trend_dsp.o: $(TARGETDIR) gtk_trend_dsp.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_trend_dsp.c

$(TARGETDIR)/gtk_trend_hist_dsp.o: $(TARGETDIR) gtk_trend_hist_dsp.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_trend_hist_dsp.c

$(TARGETDIR)/gtk_UnresetAlmTab.o: $(TARGETDIR) gtk_UnresetAlmTab.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_UnresetAlmTab.c

$(TARGETDIR)/gtk_update_dsp.o: $(TARGETDIR) gtk_update_dsp.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_update_dsp.c

$(TARGETDIR)/gtk_util_p.o: $(TARGETDIR) gtk_util_p.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_util_p.c

$(TARGETDIR)/gtk_value_entry_p.o: $(TARGETDIR) gtk_value_entry_p.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_value_entry_p.c

$(TARGETDIR)/gtk_draw_image.o: $(TARGETDIR) gtk_draw_image.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_draw_image.c

$(TARGETDIR)/gtk_zoom_p.o: $(TARGETDIR) gtk_zoom_p.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_zoom_p.c

$(TARGETDIR)/gtk_curve_widget.o: $(TARGETDIR) gtk_curve_widget.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_curve_widget.c
	
$(TARGETDIR)/gtk_sub_win.o: $(TARGETDIR) gtk_sub_win.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_sub_win.c

$(TARGETDIR)/oix.o: $(TARGETDIR) oix.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ oix.c


$(TARGETDIR)/gtk_write_db_unix.o: $(TARGETDIR) gtk_write_db_unix.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_write_db_unix.c

$(TARGETDIR)/gtk_turn_dsp_config_dlg.o: $(TARGETDIR) gtk_turn_dsp_config_dlg.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_turn_dsp_config_dlg.c

$(TARGETDIR)/gtk_read_rpt_hist.o: $(TARGETDIR) gtk_read_rpt_hist.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_read_rpt_hist.c

$(TARGETDIR)/support.o: $(TARGETDIR) menusrc/support.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ menusrc/support.c

$(TARGETDIR)/interface.o: $(TARGETDIR) menusrc/interface.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ menusrc/interface.c

$(TARGETDIR)/callbacks.o: $(TARGETDIR) menusrc/callbacks.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ menusrc/callbacks.c

$(TARGETDIR)/gtk_getime_fromfile.o: $(TARGETDIR) gtk_getime_fromfile.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_getime_fromfile.c

$(TARGETDIR)/gtk_bit_proc_util.o: $(TARGETDIR) gtk_bit_proc_util.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_bit_proc_util.c
	
$(TARGETDIR)/gtk_view_p.o: $(TARGETDIR) gtk_view_p.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_view_p.c
	
$(TARGETDIR)/gtk_gen_power_plan_curve.o: $(TARGETDIR) gtk_gen_power_plan_curve.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_gen_power_plan_curve.c

$(TARGETDIR)/gtk_op_charact_curve.o: $(TARGETDIR) gtk_op_charact_curve.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ gtk_op_charact_curve.c
	
	
#### Clean target deletes all generated files ####

clean:
	rm -f \
		$(TARGETDIR)/$(TARGET) \
		$(OBJS)

# Create the target directory (if needed)
$(TARGETDIR):
	mkdir -p $(TARGETDIR)


# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state

