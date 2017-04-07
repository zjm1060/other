
/*-----------------------------------------------------------------------
	
	Upadate from MOTIF to GTK ;
	Created by chi.hailong
	Create Date : 2005.9.12
	Last Update : 2005.9.22
	SYN by hcl 2007.12.12
     

---------------------------------------------------------------------*/

#ifndef _GTK_WIDGETS_H_
#define	_GTK_WIDGETS_H_


#include <gtk/gtk.h>




/*
 * These numbers are matched with corresponding numbers in the IPM
 * UIL module.
 */
#define	WgtN_nyi			1
#define	WgtN_help			2
#define WgtN_ctrl_menu_box 		3
#define WgtN_ctrl_menu_canvas 		4
#define WgtN_canvas 			5
#define WgtN_xclock 			6
#define WgtN_open_file_btn		7
#define WgtN_ok				8
#define WgtN_cancel			9

#define	WgtN_print_file			10
#define WgtN_alm_canvas			11
#define WgtN_alm_dsp_box		12
#define WgtN_alm_list_canvas		13
#define WgtN_alm_flag			14
#define	WgtN_view_create 		15
#define	WgtN_view_close			16
#define	WgtN_view_popup			17
#define	WgtN_login			18
#define	WgtN_logout			19

#define	WgtN_db_canvas			20
#define	WgtN_db_page_up			21
#define	WgtN_db_page_down		22
#define	WgtN_db_quit_tab_btn		23
#define	WgtN_alm_page_up		24
#define	WgtN_alm_page_down		25
#define	WgtN_sgzy_page_up		26
#define	WgtN_sgzy_page_down		27
#define	WgtN_sgzy_hist_page_up		28
#define	WgtN_sgzy_hist_page_down	29

#define	WgtN_sys_flag			30
#define	WgtN_realtime			31
#define	WgtN_demo_mode			32
#define WgtN_quit_tab_btn               33
#define	WgtN_alm_tab_area		34
#define WgtN_alm_page_new		35
#define WgtN_hist_rec_file_btn		36
#define WgtN_alm_rec_mode_btn		37
#define WgtN_real_alm_rec_btn		38
#define WgtN_histry_alm_rec_btn 	39

#define WgtN_val_ent_scale		40
#define WgtN_val_ent_chg_to		41
#define WgtN_val_ent_cur_val		42
#define WgtN_alarm_table_label		43
#define WgtN_db_table_label		44
#define	WgtN_set_bg_color		45
#define	WgtN_open_gif_file_btn		46
#define	WgtN_alm_tab_ackn		47
#define	WgtN_db_alarm_ackn		48
#define WgtN_db_dev_btn			49

#define	WgtN_prev_dsp			50
#define	WgtN_next_dsp			51
#define	WgtN_trend_list			52
#define	WgtN_trend_list_b		53
#define	WgtN_trend_area			54
#define	WgtN_trend_add			55
#define	WgtN_trend_remove		56
#define	WgtN_trend_print		57
#define	WgtN_trend_page_up		58
#define	WgtN_trend_page_down		59

#define WgtN_sgzy_file_btn              61
#define WgtN_print_tab_btn              62
#define	WgtN_rpt_print_btn		63
#define	WgtN_sftcp_btn			64
#define WgtN_sgzy_hist_area             65
#define WgtN_sgzy_hist_print            66
#define	WgtN_dsp_alm_ackn		67
#define	WgtN_display_login_info		68
#define	WgtN_sgzy_dev_pmenu		69

#define	WgtN_sgzy_quit			70
#define	WgtN_sgzy_list			71
#define	WgtN_sgzy_list_b		72
#define	WgtN_sgzy_area			73
#define	WgtN_sgzy_ok			74
#define	WgtN_sgzy_cancel		75
#define	WgtN_sgzy_add			76
#define	WgtN_sgzy_remove		77
#define	WgtN_fail_file_btn		78
#define	WgtN_sgzy_all_btn		79
	
#define	WgtN_passwd_box			80
#define	WgtN_username			81
#define	WgtN_passwd_text		82
#define	WgtN_new_passwd_text		83
#define	WgtN_passwd_ok			84
#define	WgtN_passwd_cancel		85
#define	WgtN_passwd_mgr_box		86
#define	WgtN_change_passwd		87
#define	WgtN_new_passwd_rc		88
#define	WgtN_new_passwd_again		89

#define	WgtN_get_str_box		90
#define	WgtN_str_box_ok			91
#define	WgtN_str_box_cancel		92
#define	WgtN_str_text_wgt		93
#define	WgtN_str_box_label		94
#define	WgtN_cur_dspname		95
#define WgtN_oper_cond_box 		96
#define WgtN_oper_cond_canvas 		97
#define	WgtN_oper_cond_quit_btn		98

#define WgtN_trend_hist_area            100
#define WgtN_trend_hist_page_up         101
#define WgtN_trend_hist_page_down       102
#define WgtN_trend_file_btn             103
#define WgtN_trend_save			104	
#define	WgtN_db_form_ana		105
#define	WgtN_db_form_int		106
#define	WgtN_db_dat_type_btn		107
#define	WgtN_SMS_username		108


#define	WgtN_SMS_dsp_monit		110
#define	WgtN_SMS_rpt_print		111
#define	WgtN_SMS_alarm_ack		112
#define	WgtN_SMS_io_lmt_set		113
#define	WgtN_SMS_io_point_mgr		114
#define	WgtN_SMS_param_setting		115
#define	WgtN_SMS_manual_setting		116
#define	WgtN_SMS_config_setting		117
#define	WgtN_SMS_sys_mgr		118
#define	WgtN_SMS_power_adjust		119

#define	WgtN_SMS_plant_dev_ctrl		120
#define	WgtN_SMS_temp_trend_set		121
#define	WgtN_SMS_energy_setting		122
#define WgtN_SMS_pseu_param_setting	123
#define WgtN_trend_dev_pmenu		124
#define WgtN_db_dev_pmenu		125

#define	WgtN_zoom_out_to_1_btn		129

#define WgtN_alm_ackn			130
#define	WgtN_rpt_date_btn		131
#define	WgtN_rpt_cur_day_btn		132
#define	WgtN_rpt_cur_month_btn		133
#define	WgtN_rpt_hist_btn		134
#define	WgtN_hdcp_btn			135

#define WgtN_replay_btn			136  /* Added by zyp on 2004.7.14 */

#define WgtN_alm_sound_prohibit		137 
#define WgtN_alm_val_ent_chg_to		138

#define WgtN_all_alarm_btn		140
#define WgtN_fault_alarm_btn		141
#define WgtN_event_msg_btn		142
#define	WgtN_int_tab_btn		143
#define	WgtN_pol_tab_btn		144
#define	WgtN_sysmsg_tab_btn		145
#define WgtN_command_tab_btn            146
#define	WgtN_ana_tab_btn		147
#define WgtN_param_set_btn              148

#define	WgtN_start_stop_btn		149
#define WgtN_non_alarm_btn              150
#define	WgtN_bhmsg_tab_btn		151
#define	WgtN_tag_mgr			152		/* wdk 99.6.1.	*/
#define	WgtN_pulse_tab_btn		153
#define WgtN_int_pol_alarm_btn		154
#define WgtN_alm_dev_pmenu		155
#define	WgtN_alm_scroll_window		156
#define	WgtN_alm_scroll_bar		157
#define	WgtN_db_scroll_window		158

#define WgtN_dismiss                    182
#define WgtN_station_btn		183 /* Added by zyp on 2004.6.21 */
#define WgtN_station1_btn		184
#define WgtN_station2_btn               185
#define WgtN_station3_btn               186
#define WgtN_station4_btn               187
#define WgtN_station5_btn               188
#define WgtN_station6_btn               189
#define WgtN_station7_btn               190
#define WgtN_station8_btn               191
#define WgtN_station9_btn               192
#define WgtN_station10_btn              193
#define WgtN_station11_btn              194
#define WgtN_station12_btn              195
#define WgtN_station13_btn		196
#define WgtN_station14_btn              197
#define WgtN_station15_btn              198
#define WgtN_station16_btn              199
#define WgtN_station17_btn              200
#define WgtN_station18_btn              201
#define WgtN_station19_btn              202
#define WgtN_station20_btn              203
#define WgtN_station21_btn              204
#define WgtN_station22_btn              205
#define WgtN_station23_btn              206
#define WgtN_station24_btn              207
#define WgtN_station25_btn              208

#define WgtN_alm_station_pmenu		209 
#define	WgtN_all_station_btn		210	
#define WgtN_dev_form			211

#define	WgtN_db_station_pmenu           212
#define	WgtN_sgzy_station_pmenu         213
#define	WgtN_trend_station_pmenu        214
		
#define WgtN_alm_station_btn		215
#define WgtN_db_station_btn		216
#define WgtN_sgzy_station_btn		217
#define WgtN_trend_station_btn		218

#define WgtN_alm_dev_ok_btn		220

#define	WgtN_curve_canvas		231
#define	WgtN_curve_ana_list		232
#define	WgtN_curve_date_input		233
#define WgtN_curve_add			234
#define WgtN_curve_del			235
	
#define WgtN_sg_dev_btn			241
#define WgtN_trend_dev_btn		242

#define WgtN_all_alm_dev_btn            243 
#define WgtN_non_alm_dev_btn            244 

#define	WgtN_cur_user			247
#define	WgtN_curve_btn			248
#define WgtN_db_iost			249

#define       k_max_widget              250  /*194*/ 


#define	MAX_WIDGETS	(k_max_widget + 1)

#ifndef		MAX_CRT_NUMBER
#define		MAX_CRT_NUMBER		3
#endif

GtkWidget* file_select_dialog[MAX_CRT_NUMBER];   
typedef struct 
{

  gint radius;
  gint pointer_width;

  /* Current angle */
  gfloat angle;
  gfloat last_angle;

  /* Old values from adjustment stored so we know when something changes */
  gfloat old_value;
  gfloat old_lower;
  gfloat old_upper;
} DIAL_METER;



GdkDisplay      *g_display ;
GdkDisplay	*second_display[MAX_CRT_NUMBER];
GdkScreen 	*second_screen[MAX_CRT_NUMBER] ; 
GtkWidget 	*wgts[MAX_CRT_NUMBER][MAX_WIDGETS];
GtkWidget 	*toplevel[MAX_CRT_NUMBER],
		*main_w[MAX_CRT_NUMBER],   /* main window  */
		*alarm_w[MAX_CRT_NUMBER],  /* alarm window */
 		*canvas[MAX_CRT_NUMBER],   /* Draw Area Widget   */
                *canvas2[MAX_CRT_NUMBER];
//GtkWidget	*ctrl_menu_shell[MAX_CRT_NUMBER]  ;
//GtkWidget	*ctrl_menu_canvas[MAX_CRT_NUMBER]  ;  /* Draw Area Widget   */

/*GtkWidget	*io_point_info[MAX_CRT_NUMBER];  v3.0*/



#endif

 

