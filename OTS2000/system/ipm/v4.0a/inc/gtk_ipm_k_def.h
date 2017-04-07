/*-----------------------------------------------------------------------
	
	Upadate from X11 to GTK ;
	Created by chi.hailong
	Date : 2005.9.13
     
CMD_BUF : add "stn_id" by zyp on 2006.3
---------------------------------------------------------------------*/


#ifndef _GTK_IPM_K_DEF_H_ 
#define _GTK_IPM_K_DEF_H_  



#include <gtk/gtk.h>
#include <X11/Xlib.h>

#include "/DBINC/dms_structure_def.h"
#include "/LANINC/lan_common.h"

#define	MAX_CRT_NUMBER		3

#define	CRT_1	0
#define	CRT_2	1
#define	CRT_3	2
#define	CRT_4	3



int	scr[MAX_CRT_NUMBER];

char	home[30];
#define	HOME	"OIXHOME"
#define	HOST_NAME	"HOSTNAME"
#define	CRT_NUM		"CRT_NUM"	


#define	DBNAME_LEN		POINT_NAME_SIZE	
#define OLD_DBNAME_LEN		36 

#define MENU_NAME_LEN		10	
#define PROG_NAME_LEN		12	/* Modified by wzg from 10 to 12   2003.8.6 */
#define	MAX_TEXT_LEN		40
#define	MAX_GIF_FILE_NAME_LEN	40
#define	MAX_FILE_NAME_LEN	40
#define	DSP_FILE_SHORTNAME_LEN	30
#define MAXPLCNT		30
#define	MAX_LAYER_NAME_LEN	10
#define MAXOBJECTS	 	1500
#define	MAX_DYN_AREA_NUM	20
#define	MAX_ACT_AREA_NUM	40

#define	MAX_CURVE_ENTRY		96		/* for ipm/oix curve drawing	*/

#define	MAX_FULL_FILE_NAME_LEN	80
#define	MAX_TIME_LEN		100

#define	MAXLAYERS		16

#define	DYN_TAG_OFF		0
#define	DYN_TAG_ON		1

#define	CTRL_TAG_OFF		0
#define	CTRL_TAG_ON		1


#define	DYN_CGRP_TAG_OFF	0
#define	DYN_CGRP_TAG_ON		1
#define	DYN_CGRP_TAG_ADD_ON	2

#define	CUR_DAY_RPT_DATA	0
#define	CUR_MONTH_RPT_DATA	1
#define	HIST_RPT_DATA		2
#define RPT_PRINT		3  /* added by zyp on 2005.11 gtk */

#define	BACKGROUND	0
#define	FOREGROUND	1

#define ALM_YEAR_LEN    4
#define ALM_MONTH_LEN   2

#define	ALM_YES		1
#define	ALM_NO		0
#define VERTI_OUT       1189    /* Tag of vertical print out : chi 05.03.08  */



int alm_sound_flag,alm_bell_number;


typedef	struct
{
	int	x1, y1, x2, y2;
}	RECT;


typedef	struct
{
	int	class, name;
}	SYMB_ID;

/*  added by chi  05.03.08 ;  */

typedef struct 
{
        char  text[MAX_TEXT_LEN +5];
        char  DirOut ;
        char   ReservWord ;             
} TEXT_S     ; /*  string with formating   */


typedef	union
{
	/*   char		text[MAX_TEXT_LEN +5];   rm by chi */
	TEXT_S          text ;
	char		fname[MAX_GIF_FILE_NAME_LEN];
	XPoint		pl[MAXPLCNT];
	XArc		arc;  
	SYMB_ID		sym;
	char            free_area[MAXPLCNT*4];  /*add by zqh, 00.3.24 */
}	RCD_DATA;



typedef struct
{
	char	name[DSP_FILE_SHORTNAME_LEN];
}	ACTION_DSP;


typedef	struct
{
	char	name[MENU_NAME_LEN];
	int	id;
}	ACTION_MENU;


typedef	struct
{
	char	status[PROG_NAME_LEN];
	int	id;
}	ACTION_PROG;


typedef	union
{
	ACTION_DSP	dsp;
	ACTION_MENU	menu;
	ACTION_PROG	prog;
	char		free_area[MAX_ACT_AREA_NUM]; /* max byte for the union of action_data*/
}	ACT_FMT_DEF;

typedef	struct
{
	int		ctrl_type;
	int		ctrl_tag;
	ACT_FMT_DEF	fmt_def;
}	ACTION;

typedef	struct
{
	int	sym_grp;
}	DYN_SYM_GRP;


typedef	struct
{
	int	str_grp;
}	DYN_STR_GRP;

typedef	struct
{
	int	tag;
	int	cgrp_id;
}	DYN_COLOR_GRP;

typedef	struct
{
	float	hi_limit;
	float	low_limit;
	float	hi_limit_2;
	float	low_limit_2;
	int	direction;	/*Increasing value direction
				0=Up,   1=Right, 2=Left, 3=Down,	
				4=Up from Center,  5=Right from Center,
				6=Left from Center,	7=Down from Center */
}	DYN_BAR_GRPH;	

typedef struct
{
	float   hi_limit;
	float   low_limit;
	float   hi_limit_2;
	float   low_limit_2;
}	DYN_VALUE_BAR;
  
typedef struct
{
	float   hi_limit;
	float   low_limit;
	short   hour_id;
	float   hi_limit_2;
	float   low_limit_2;
}	HIST_BAR_GRPH;

typedef struct
{
	float   hi_limit;
	float   low_limit;
	int	length;
	int	sign_typ;	 /* Sign type
						0=Not_used,  1=No, 2=+or-,	 3=Up_arrow,
						4=Dn_arrow,  5=Lf_arrow,	6=Rt_arrow,  7=Blankor- */
	int	dec_pt; 	/* Decimal point position */

}	DYN_NUMERIC;


typedef struct
{
	short   length;
	short   sign_type; /* Sign type
						0=Not_used,  1=No, 2=+or-,	 3=Up_arrow,
						4=Dn_arrow,  5=Lf_arrow,	6=Rt_arrow,  7=Blankor- */
	short   dec_pt; /* Decimal point position */
	short   req_type;
	short   hour_id;
	short   date_id;
	short   month_id;
	short   special_type;   /* for impulse vally or peak so so .*/
}	  DYN_RPT_NUMERIC;
 
    
typedef struct
{
	int	tag;
	int	cgrp_id;
	short   req_type;
}	  DYN_CASCAD_COLOR;
				 
typedef struct
{
	int	tag;
	int	cgrp_id;
	short   update_time;    /* ms */
}	  ANIMATE_COLOR;
				   
typedef struct
{
	int	sym_grp;
	short   limit_type;
}	  DYN_ANA_STATUS;
			

typedef	union
{
	DYN_SYM_GRP	sym;
	DYN_STR_GRP	str;
	DYN_BAR_GRPH	bar_chart;
	DYN_BAR_GRPH	curve;
	DYN_NUMERIC	num;
	DYN_VALUE_BAR	val_bar;
	DYN_RPT_NUMERIC	rpt_num;
	HIST_BAR_GRPH	hist_bar;
	DYN_CASCAD_COLOR cascad_clr;
	ANIMATE_COLOR   animate_clr;
	DYN_ANA_STATUS  ana_status;
	int		free_area[MAX_DYN_AREA_NUM]; /* max unoin data of dyn_fmt_def data */
}	DYN_FMT_DEF;


typedef	struct
{
	int		dyn_type;
	int		dyn_tag;
	int		dio_type;	/* Display Input/Output Type
						0 = Output only
						1 =	Output plus data entry permitted
						2 = No output, data entry only field */
	int		f_by_off;	/* Numeric field's byte offset from
						start of database entry */
	int		qi_loc;		/* Quality indicator applicability
						and relative location.
						0=Not_used, 1=No, 2=Left, 3=Right,
						4=Up, 5=Down */
	int		tagi_loc;	/* Tag indicator applicability
						and relative location.
						0=Not_used, 1=No, 2=Left, 3=Right,
						4=Up, 5=Down */
	int		data_class;
	DYN_FMT_DEF	fmt_def;
}	DYNAMIC;

	

typedef struct
{
	RECT	rect;
	short	draw_type;
	short	tag;
	short	fill_type;
	short	fg, bg;
	short	line_width;
	short	layer;
	short	plcnt;
	short	arc_type;
	short	ftset_id;
	RCD_DATA	data;
}	BACKRCD;

typedef	struct
{
	char	dbname[OLD_DBNAME_LEN];
	ACTION		act;
	DYNAMIC		dyn;
	DYN_COLOR_GRP	cgrp;
}	LINKRCD;

typedef	struct
{
	char	dbname[DBNAME_LEN];
	ACTION		act;
	DYNAMIC		dyn;
	DYN_COLOR_GRP	cgrp;
}	NEW_LINKRCD;

typedef	struct
{
	BACKRCD	bck;
	LINKRCD	lnk;
}	  GBRCD;


typedef	struct
{
	BACKRCD	bck;
	NEW_LINKRCD	lnk;
}	  NEW_GBRCD;
typedef	struct
{
	float	min_visible;
	float	max_visible;
	int	visible_tag;
	int	lock_tag;
	int	hide_tag;
	char	name[MAX_LAYER_NAME_LEN];
}	LAYERDB;


typedef struct
{
	char name[MAX_LAYER_NAME_LEN];
	GBRCD buffer[MAXOBJECTS];
	int	nextpos;
	LAYERDB	layer[MAXLAYERS];
	int	layer_cnt;    
}	  GDBUFF;

typedef struct
{
	char name[MAX_LAYER_NAME_LEN];
	NEW_GBRCD buffer[MAXOBJECTS];
	int	nextpos;
	LAYERDB	layer[MAXLAYERS];
	int	layer_cnt;    
}	  NEW_GDBUFF;

typedef struct
{
/* add crt_menu_gc by wzg 2003.5.23 */
	GdkGC	*gc,*alm_gc,*ctrl_gc;
	SYMB_ID	sym;
	int	sym_grp;
	int	cgrp;
	int	menu;
}	INFO_BUFF;


short	rpt_tab_date[MAX_CRT_NUMBER];
char	str_time_eg[MAX_TIME_LEN];
char	str_time[MAX_TIME_LEN];

char    cur_dsp_name[MAX_CRT_NUMBER][MAX_FULL_FILE_NAME_LEN];
char    select_rpt_hist_name[MAX_CRT_NUMBER][MAX_FULL_FILE_NAME_LEN];
char    select_hist_alm_file[MAX_CRT_NUMBER][MAX_FULL_FILE_NAME_LEN];
char    select_hist_log_file[MAX_CRT_NUMBER][MAX_FULL_FILE_NAME_LEN];
int	tag_mgr_mask[MAX_CRT_NUMBER];

INFO_BUFF	info[MAX_CRT_NUMBER];
NEW_GBRCD		cur[MAX_CRT_NUMBER];    //change from GBRCD 
GDBUFF			v3_gdbase[MAX_CRT_NUMBER]
NEW_GDBUFF		gdbase[MAX_CRT_NUMBER];
short cur_usr_id;
int	fetch_sftcp_nextpos;
int 	BITMAP_TAG_OFF;
int	alm_sound;	/*	Added by zyp on 1999.9.9 for inputing alm_sound time	*/
int	bell_number,alm_bell_number,Init_bell_number ;
int	Tag_bit,no_Tag;

COMMAND_MSG	 cmd_buf[MAX_CRT_NUMBER];	/* MAX_CRT_NUMBER is added by wdk 2000.1.13	*/

float   enter_value_high_lmt[MAX_CRT_NUMBER];   /* MAX_CRT_NUMBER is added by wdk 2000.1.13	*/
float   enter_value_low_lmt[MAX_CRT_NUMBER];


#include	<sys/stat.h>
#include 	<stdio.h>

#endif
