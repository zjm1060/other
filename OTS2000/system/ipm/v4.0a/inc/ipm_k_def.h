#ifndef		IPM_K_DEF_H_ 
#define		IPM_K_DEF_H_  


int	scr;

char	home[30];
#define	HOME				"IPMHOME"

#define LONGNAME_SIZE		52    //NEW
#define	DBNAME_LEN			36   //OLD
#define MENU_NAME_LEN		10   //old -10  chi  06.04.20
#define PROG_NAME_LEN		20   //old 10;
#define	MAX_TEXT_LEN		100   //old -- 40
#define	MAX_GIF_FILE_NAME_LEN	40
#define	MAX_FILE_NAME_LEN	80
#define	DSP_SHORTNAME_LEN	30
#define MAXPLCNT			30
#define MAX_LAYER_NAME_LEN	10
#define MAXOBJECTS      	2500  /*1500*/
#define	MAX_DYN_AREA_NUM	20	
#define	MAX_ACT_AREA_NUM	40	
#define	DSP_FILE_SHORTNAME_LEN	30

#define	MAX_CURVE_ENTRY		96		/* for ipm/oix curve drawing	*/

#define	DYN_TAG_OFF		0
#define	DYN_TAG_ON		1

#define	CTRL_TAG_OFF		0
#define	CTRL_TAG_ON		1

#define	DYN_CGRP_TAG_OFF	0
#define	DYN_CGRP_TAG_ON		1
#define	DYN_CGRP_TAG_ADD_ON	2	

#define  VERTI_OUT   1189   // Tag of vertical print out ;  chi


typedef	struct
{
	int	x1, y1, x2, y2;
}	XRECT;


typedef	struct
{
	char		fill_type;
	char        fillbkmode;
	 
}	FILLTYPE;

typedef struct 
{
    int	    x ,y ,width ,height ;
 
} XRectangle  ;//  string with formating ;



typedef	struct
{
	short	x;
	short	y;
}	XPoint;

typedef	struct
{
	short		x,y;
	unsigned	short	width, height;
	short		angel1,angel2;
}	XArc;



typedef	struct
{
	int	class, name;
}	SYMB_ID;

typedef struct 
{
    char	   text[MAX_TEXT_LEN +5];
	char       DirOut ;
	char       ReservWord ;

} TEXT_S  ;//  string with formating ;




typedef	union
{
	//char		text[MAX_TEXT_LEN +5];
    TEXT_S      text ;
	char		fname[MAX_GIF_FILE_NAME_LEN];
	XPoint		pl[MAXPLCNT];
	XArc		arc;
	SYMB_ID		sym;
}	RCD_DATA;



typedef struct
{
	char	name[DSP_SHORTNAME_LEN];
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

typedef	struct  //add chichi 
{
	int	tag;
	int	point_id;
}	DYN_CURVE_PT;
typedef	struct  //add chichi 
{
	int	tag;
	int	temp_chara_id;
	int length   ;   
}	DYN_TEMP_CHARA;

typedef	struct  //add chichi
{
	int		tag;
	int		point_id;
	float	hi_limit;	
	float	low_limit;
	char	LoadDBName[LONGNAME_SIZE-1];  /*Real curve DBNAME */
}	DYN_LOAD_CURVE;


typedef	struct  //add chichi   090615
{
	int		tag;
	int		point_id;
	float	hi_limit;	
	float	low_limit;
	char    StartTime[25];		/*开始时间例如：“2007-08-05 09:10:23” ---19--bit*/	 
	int		TimeLength ;		/*曲线时长*/
	char	LoadDBName[LONGNAME_SIZE-1];  /*Real curve DBNAME */
}	DYN_REAL_CURVE;


typedef	struct   /*added by chichi */  /*scene redisplay graphy element */
{
 
	char    StartTime[25];		/*2007-08-05 09:10:23 ---19--bit*/	 
	int		TimeLength ;		/*回放时长*/
	char	ReDspFileNm[DSP_FILE_SHORTNAME_LEN];   /*回放画面*/
}	DYN_SCENE_REDISP;


typedef	struct
{
	float	hi_limit;	
	float	low_limit;
	float	hi_limit_2;
	float	low_limit_2;
	int	direction;	/*Increasing value direction
				0=Up,   1=Right, 2=Left, 3=Down,	
				4=Up from Center,  5=Right from Center,
				6=Left from Center,     7=Down from Center */
}	DYN_BAR_GRPH;	


typedef	struct
{
	float	hi_limit;	
	float	low_limit;
	float	hi_limit_2;
	float	low_limit_2;
	int	    vibra_serial;	 
	char	dbname2[LONGNAME_SIZE-1];  /*LOW  LIMIT DBNAME */
}	DYN_VIB_AREA;
typedef	struct
{
	float	hi_limit;
	float	low_limit;
	short	hour_id;
	float	hi_limit_2;
	float	low_limit_2;
}	HIST_BAR_GRPH;

typedef struct
{
	float   hi_limit;
	float   low_limit;
	int     length;
	int     sign_typ;	 /* Sign type
			0=Not_used,  1=No, 2=+or-,      3=Up_arrow,
			4=Dn_arrow,  5=Lf_arrow,        6=Rt_arrow,  7=Blankor- */
	int     dec_pt; 	/* Decimal point position */

}	DYN_NUMERIC;

typedef struct
{
	short   length;
	short   sign_type; /* Sign type
				0=Not_used,  1=No, 2=+or-,      3=Up_arrow,
				4=Dn_arrow,  5=Lf_arrow,        6=Rt_arrow,  7=Blankor- */
	short   dec_pt; /* Decimal point position */
	short   req_type;
	short   hour_id;
	short   date_id;
	short   month_id;
	short	special_type;	/* for impulse vally or peak so so .*/
}       DYN_RPT_NUMERIC;
 
typedef	struct
{
	int	tag;
	int	cgrp_id;
	short	req_type;
}	DYN_CASCAD_COLOR;

typedef	struct
{
	int	tag;
	int	cgrp_id;
	short	update_time;	   /* ms */
}	ANIMATE_COLOR;

typedef struct
{
	int	sym_grp;
	short	limit_type;
}	DYN_ANA_STATUS;

typedef	union
{           /* max number bytes 80 */
	DYN_SYM_GRP			sym;
	DYN_STR_GRP			str;
	DYN_BAR_GRPH		bar_graph;	
	DYN_BAR_GRPH		curve;	
	DYN_NUMERIC			num;
	DYN_RPT_NUMERIC		rpt_num;
	HIST_BAR_GRPH		hist_bar;
	DYN_CASCAD_COLOR	cascad_clr;
	ANIMATE_COLOR		animate_clr;	
	DYN_ANA_STATUS		ana_status;
	DYN_CURVE_PT		curve_point ;		/*added by chichi */
	DYN_LOAD_CURVE		load_curve  ;		/*added by chichi */
	DYN_TEMP_CHARA      temp_chara  ;		/*added by chichi */
	DYN_VIB_AREA        vibra_area  ;		/*added by chichi */
	DYN_SCENE_REDISP	scene_redisp ;		/*fault  redisplay  chichi */
	DYN_REAL_CURVE		real_curve ; 
	int					free_area[MAX_DYN_AREA_NUM]; /* max unoin data of dyn_fmt_def data 80 byte*/ 	
}	DYN_FMT_DEF;

typedef	struct
{
	int		dyn_type;
	int		dyn_tag;
	int		dio_type;	/*    Display Input/Output Type
						0 = Output only
						1 =	Output plus data entry permitted
						2 = No output, data entry only field */
	int		f_by_off;	/*    Numeric field's byte offset from
						start of database entry */
	int		qi_loc;		/*    Quality indicator applicability
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
	XRECT	rect;
    short	draw_type;
	short	tag;
	FILLTYPE	fill;  //short 
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
	char	dbname[DBNAME_LEN];
	ACTION		act;
	DYNAMIC		dyn;
	DYN_COLOR_GRP	cgrp;
}	LINKRCD;


//new type for v4.0a  add by chi 06.03

typedef	struct
{
	char	dbname[LONGNAME_SIZE-1];
	char    state  ;                      // new filed for point state  06.6   ;
	ACTION		act;
	DYNAMIC		dyn;
	DYN_COLOR_GRP	cgrp;
}	NEW_LINKRCD;      //sizeof (NEW_LINKRCD) is 216 !!!!!



typedef	struct
{
	BACKRCD	bck;
	LINKRCD	lnk;
}       GBRCD;


typedef	struct
{
	BACKRCD	bck;
	NEW_LINKRCD	lnk;
}	  NEW_GBRCD;


typedef struct
{
	char	name[MAX_LAYER_NAME_LEN];
	GBRCD buffer[MAXOBJECTS];
	int	nextpos;
}       GDBUFF;

typedef struct
{
	char name[MAX_LAYER_NAME_LEN];
	NEW_GBRCD buffer[MAXOBJECTS];
	int	nextpos;    
}	  NEW_GDBUFF;


typedef struct
{
	int	fb_choice;
	int	clutter;
	SYMB_ID	sym;
	int	sym_grp;
	int	str_grp;
	int	menu;
	int	cgrp;
}	INFO_BUFF;

//added chi   used in pasteing
typedef struct
{
	XRECT	rect;
    BOOL	Pasting;
	unsigned short	tag;
}	PASTEREC;

typedef enum                // Declare enum type direction added by chi
{
   UP,             
   DOWN ,          
   RIGHT,              
   LEFT              
} DIR;

typedef enum                // Declare enum type move mode. added by chi
{
   MOD_CUR,             
   MOD_PASTE ,                        
   MOD_ARROW              
} MOVE_MODE;

typedef enum                // Declare enum type direction. added by chi
{
   IS_UP,             
   IS_BOTTOM ,                        
   IS_LEFT,
   IS_RIGHT,
   IS_LU,
   IS_RU,
   IS_LB,
   IS_RB,
   IS_OUT,
   IS_IN,
} IN_STATE;

typedef struct 
{

  int radius;
  int pointer_width;

  /* Current angle */
  float angle;
  float last_angle;

  /* Old values from adjustment stored so we know when something changes */
  float old_value;
  float old_lower;
  float old_upper;
} DIAL_METER;



INFO_BUFF		info;
NEW_GBRCD		cur;    //change from GBRCD 
NEW_GDBUFF		v4_gdbase; //new
NEW_GDBUFF		GDBASE_FILE; //new
NEW_GDBUFF		gdbuff;  //new
GDBUFF			v3_gdbase ; //old


DWORD UpParam,LoParam;
PASTEREC PasteRec ;  //added chi 
HCURSOR hCurOld,hCurCur;
char cur_dsp_file[MAX_FILE_NAME_LEN];
SCROLLINFO  si ;    //added chi
BOOL   IsLineDash  ;  //added chi  






#endif
