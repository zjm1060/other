/* 
* File:   oix_all_header.h
* Author: chi.hailong;
*
* Created on 2006年8月30日, 上午10:48
* SYN  by chi  2007年5月20日
* LAST UPDATE  2007年12月05日
*/

#ifndef _oix_all_header_H
#define	_oix_all_header_H

#define        Bool    BOOL
#define        True    1
#define        False	0

#ifdef	__cplusplus
extern "C" {
#endif
	
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <iconv.h>
	/*********OIX external dependency  header file list ********************/
#include	"../../../dms/v4.0a/inc/dms_com.h"
#include	"../../../dps/v4.0a/inc/dps_all.h"
#include        "../../../dps/v4.0a/inc/fault_table_def.h"
#include	"../../../lan/v4.0a/inc/lan.h" 
#include        "../../../cmd/v4.0a/inc/cmd_def.h"
#include 	"../../../pas/v4.0a/inc/interlockgen.h"
#include 	"../../../pas/v4.0a/inc/relatepoint.h"
#include 	"../../../dps/v4.0a/inc/dps_rec_def.h"
	
	
	/***********    The following define oix self need header file *************/  
 #include	"../inc/gtk_msg_text_def.h" 
   
#include	"../inc/gtk_color_def.h"
#include	"../inc/gtk_widgets.h"
#include	"../inc/gtk_ipm_k_def.h"
#include	"../inc/gtk_draw_def.h"
#include	"../inc/dyn_form_def.h"   
#include	"../inc/dyn_prog_def.h" 
	
#include	"../inc/gtk_sgzy_def.h"
#include	"../inc/gtk_db_tab_def.h"   
#include	"../inc/gtk_oix_cstr.h" 
#include	"../inc/menu_def.h" 
#include	"../src/menusrc/gtk_menu.h" 
#include	"../inc/station_def.h"
#include        "../../../sms/v4.0a/inc/user_privilege_def.h"
#include	"../inc/dsp_recall.h" 
#include        "../inc/bit_proc_def.h"
	
#include	"../inc/oix_common.h"
#include	"../inc/gtk_text_edit_def.h"
#include	"../inc/gtk_alarm_tab.h"
#include	"../inc/gtk_color_grp_def.h"
#include 	"../inc/mode_def.h"  
	
#include	"../inc/demo_dsp_def.h"
#include	"../inc/val_entry_def.h"
#include    "../inc/fault_redisplay.h"
#include    "../inc/gtk_sub_dsp_def.h"
#include	"../inc/string_grp_def.h"
#include	"../inc/symbol_grp_def.h"
#include	"../inc/symbol_def.h"
#include	"../inc/field_def.h"
#include 	"../inc/scaler.h" 
#include	"../inc/zoom_def.h"
#include 	"../inc/gtk_log_def.h"
#include 	"../inc/gtk_ProgAlarmTab_new.h"
//#include	"../inc/gtk_auto_check.h"

#include "../inc/gtk_curve_widget.h"
    
#define     GTK_RC_FILE   "" /*E:\\home\\ems\\h9000\\def\\gtkrc.zh*hcl 071214*/
#ifdef	__cplusplus
}
#endif



#if defined(WINDOWS_OS)
#ifndef  _FILES_PATH_
#define  _FILES_PATH_
#define  FILE_DSP_PATH			"\\home\\ems\\h9000\\dsp\\"   
#define  FILE_COLOR_GROUP		"\\home\\ems\\h9000\\dsp\\COLOR_GRP_LIBRARY.DBIN"
#define  FILE_CTRL_MENU			"\\home\\ems\\h9000\\dsp\\CTRL_MENU_LIBRARY.DBIN"
#define  FILE_CTRL_MENU_EXLANG	"\\home\\ems\\h9000\\dsp\\CTRL_MENU_LIBRARY_EX.DBIN"   //扩展语言
#define  FILE_SYMBOL_GROUP		"\\home\\ems\\h9000\\dsp\\SYMBOL_GRP_LIBRARY.DBIN"
#define  FILE_SYMBOL_LIBRARY    "\\home\\ems\\h9000\\dsp\\SYMBOL_LIBRARY.DBIN"
#define  FILE_STRING_GRP		"\\home\\ems\\h9000\\dsp\\STRING_GRP_LIBRARY.DBIN"
#define  FILE_STRING_GRP_EXLANG	"\\home\\ems\\h9000\\dsp\\STRING_GRP_LIBRARY_EX.DBIN"   //扩展语言



#define H9000_HIS_TRAP "\\home\\ems\\h9000\\his\\trap\\*.LOG"

#define	 FILE_PRIORITY_DATA     "\\home\\ems\\h9000\\dat\\priority_database.dat" 
#define  FILE_INTER_LOCK	    "\\home\\ems\\h9000\\dat\\InterLockGen.dat" 
#define  PATH_PIPE		'\\'
#define WITHREPORT    /**show report menu**/
#define SGZY_FILE_PATH "\\home\\ems\\h9000\\his\\sgzy\\"
#define SG_FILE_PATH		"\\home\\ems\\h9000\\dat\\"
#define DEMO_DSP_NAME "\\home\\ems\\h9000\\def\\demo_dsp_name.def"
#define OIX_TAG_DEF_FILE   "\\home\\ems\\h9000\\def\\oix_tag_disp.def"
#define DEMO_DSP_NAME_EX "\\home\\ems\\h9000\\def\\demo_dsp_name_ex.def"
#define FILE_SOFTCP_PIC         "\\home\\ems\\h9000\\his\\sftcp\\*.sfbin"
#define PATH_SOFTCP_PIC			"\\home\\ems\\h9000\\his\\sftcp\\"
#define RPT_LOG_PATH	"\\home\\ems\\h9000\\his\\rpt\\"
#define PATH_PRINT_TEMP_PIC	"\\home\\ems\\h9000\\tmp\\"
#define FILE_STRLST_DEF		"\\home\\ems\\h9000\\def\\oix_str_list.def"
#define HIS_DATABASE_NAME "\\home\\ems\\h9000\\dat\\his_database.out"
#define FILE_OIX_DEF_DAT  "\\home\\ems\\h9000\\dat\\oix_def.dat"    

#define FILE_OIX_CONFIG_XML "\\home\\ems\\h9000\\def\\oix_config.xml"
#define AUTOCHECK_DIR		"\\home\\ems\\h9000\\his\\oix\\"
#endif




/******************MACOR DEFINE********************************/
#ifndef IPCMODE
#define IPCMODE  
#endif


#ifdef AUD_ALM_BEFORE_CMD
#undef AUD_ALM_BEFORE_CMD
#endif    



/***************** ****************************************/



#else /*UNIX*/
#endif

#ifdef SUN_OS
#define  FILE_DSP_PATH			"/home/ems/h9000/dsp/" 
#define  FILE_COLOR_GROUP		"/home/ems/h9000/dsp/COLOR_GRP_LIBRARY.DBIN"
#define  FILE_CTRL_MENU			"/home/ems/h9000/dsp/CTRL_MENU_LIBRARY.DBIN"
#define  FILE_CTRL_MENU_EXLANG	"/home/ems/h9000/dsp/CTRL_MENU_LIBRARY_EX.DBIN"    //扩展语言版本
#define  FILE_SYMBOL_GROUP		"/home/ems/h9000/dsp/SYMBOL_GRP_LIBRARY.DBIN"
#define  FILE_SYMBOL_LIBRARY    "/home/ems/h9000/dsp/SYMBOL_LIBRARY.DBIN"
#define  FILE_STRING_GRP		"/home/ems/h9000/dsp/STRING_GRP_LIBRARY.DBIN"
#define  FILE_STRING_GRP_EXLANG	"/home/ems/h9000/dsp/STRING_GRP_LIBRARY_EX.DBIN"	  //扩展语言版本
#define	 FILE_PRIORITY_DATA     "/home/ems/h9000/dat/priority_database.dat" 
#define  FILE_INTER_LOCK	    "/home/ems/h9000/dat/InterLockGen.dat" 
#define  H9000_HIS_TRAP			"/home/ems/h9000/his/trap/*.LOG"
#define  PATH_PIPE		'//'
//#define WITHREPORT    /**show report menu**/
#define SGZY_FILE_PATH "/home/ems/h9000/his/sgzy/"
#define DEMO_DSP_NAME "/home/ems/h9000/def/demo_dsp_name.def"
#define DEMO_DSP_NAME_EX "/home/ems/h9000/def/demo_dsp_name_ex.def"
#define OIX_TAG_DEF_FILE   "/home/ems/h9000/def/oix_tag_disp.def"
#define FILE_SOFTCP_PIC         "/home/ems/h9000/his/sftcp/*.sfbin"
#define PATH_SOFTCP_PIC			"/home/ems/h9000/his/sftcp/"
#define SG_FILE_PATH		"/home/ems/h9000/dat/"
#define RPT_LOG_PATH		"/home/ems/h9000/his/rpt/"
#define FILE_STRLST_DEF		"/home/ems/h9000/def/oix_str_list.def"
#define HIS_DATABASE_NAME "/home/ems/h9000/dat/his_database.out"
#define FILE_OIX_DEF_DAT  "/home/ems/h9000/dat/oix_def.dat"    

#define FILE_OIX_CONFIG_XML "/home/ems/h9000/def/oix_config.xml"
#define AUTOCHECK_DIR		"/home/ems/h9000/his/oix/"
#endif

#define REAL_CURVE_FILE   "/home/ems/h9000/dat/curvefile.dat"  //实时曲线配置结果
#define REAL_DIS_CURVE_FILE "/home/ems/h9000/dat/curve_dis_file.dat" //显示六条曲线

#define GROUP_DEFINE_FILE	"/home/ems/h9000/gen/DBIOGROUP.DEF"

#ifndef _ALM_CANVAS_SIZE_
#define _ALM_CANVAS_SIZE_ 
#define  ALM_CANVAS_WIDTH    1250
#define  ALM_CANVAS_HEIGHT   380
#define  ALM_CANVAS_X_OFFSET 0 

#endif

extern char	local_hostname[];
extern char	cur_usr_name[];

///////////oix_config.xml/////////////////
char commonDirName[MAX_FILE_NAME_LEN];
char zjxName[MAX_FILE_NAME_LEN];
int xml_comdir_btn; //menubar comdir- 0=button;1=combox
int xml_auto_filter; //ProgAlarmTab - 0=no autoalarm;1=show autoalarm

#endif	/* _oix_all_header_H */

