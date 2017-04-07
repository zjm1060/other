/*****
	Modified by zyp from motif to gtk
	changed:
	SGZY_WIDTH SGZY_HEIGHT X_LEFT SGZY_PAGE_SIZE
	SYN : 2008.4.21

*****/


#ifndef _GTK_SGZY_DEF_H_ 
#define _GTK_SGZY_DEF_H_
  
#include "../../../pas/v4.0a/inc/sgzy.h"

#define MAX_PLCNT		50
#define MAX_OBJECTS      	60
#define	MAX_NUM_DB              80


/*#define	TRENDPOINTS		300*/
#define MAX_TREND_FILE_NAME_LEN 100
#define MAX_FAIL_FILE_NAME_LEN	100

#define	SGZY_PAGE_SIZE		3	
#define	SGZY_HIST_PAGE_SIZE	6

int	sgzy_page_numb;
int	sgzy_page_cnt;
int	trend_page_numb;
int	trend_page_cnt;

#define	NPOINTS			80
			
#define	SGZY_WIDTH		480
#define	SGZY_HEIGHT		230
#define	X_SPACE			80
#define	Y_SPACE			40
#define	X_LEFT			80
#define	Y_TOP			60

#define TREND_WIDTH             580//600
#define TREND_HEIGHT            210
#define TREND_X_SPACE           40
#define TREND_Y_SPACE           60
#define TREND_X_LEFT            40
#define TREND_Y_TOP             60

#ifdef WINDOWS_OS
#define SGZY_FILE       "\\home\\ems\\h9000\\dat\\sgzyfile.dat"
#define SGZY_SELECT_FILE "\\home\\ems\\h9000\\dat\\sgzyselect.dat"
#define TREND_FILE	"\\home\\ems\\h9000\\dat\\wdqsfile.dat"
#define TREND_FILE_SELECT	"\\home\\ems\\h9000\\dat\\wdqsfileselect.dat"
#endif 
#ifdef SUN_OS
#define SGZY_FILE       "/home/ems/h9000/dat/sgzyfile.dat"
#define SGZY_SELECT_FILE "/home/ems/h9000/dat/sgzyselect.dat"
#define TREND_FILE	"/home/ems/h9000/dat/wdqsfile.dat"
#define TREND_FILE_SELECT	"/home/ems/h9000/dat/wdqsfileselect.dat"
#endif

#ifdef	SGZY_DEF_MODULE
	short	cur_stn		=1;
	short	cur_dev		=1;
	short	cur_dtype	=1;
	short   sgzy_add_item    =0;
	short   sgzy_del_item    =0;
#else
	extern  short   cur_stn;
	extern	short	cur_dev;
	extern	short	cur_dtype;
	extern	short   sgzy_add_item;
	extern	short   sgzy_del_item;
#endif
			
/*typedef struct
{
        POINTER		ptr[MAX_SGZY_COUNT];
        SHORT           next_pos;
}       SGZYDATA;


typedef struct
{
        CHAR             setbuf[20];
        CHAR             flag_lmt;
        float            f_value;
}       ANA_TRA_DATA;

typedef struct
{
        POINTER         ptr[MAX_WDQS_COUNT];
        SHORT           next_pos;
}       TRENDDATA;

*/
//SGZYDATA	data;
//SGZYDATA        faildata;
//TRENDDATA	trenddata;

SGZY_SHM_DATA	*sgzy_pt;
SGZY_DATA	*sgzy_hist_da;

SGZY_DY data,faildata;

WDQS_DY trenddata, trendSelect;
WDQS_SHM_DATA	*trend_pt;
WDQS_DATA	*trend_hist_da;

GdkGC	*draw_gc;

GtkWidget*	sgzy_main_w[MAX_CRT_NUMBER];
GtkWidget*	sgzy_history_main_w[MAX_CRT_NUMBER];
GtkWidget*  	trend_main_w[MAX_CRT_NUMBER];
GtkWidget*  	trend_history_main_w[MAX_CRT_NUMBER];

short	sgzy_mmi_mode;
extern	char    trend_hist_file_name[MAX_TREND_FILE_NAME_LEN];
extern	short	trend_dev;

#define	SGZY_SET	0
#define	SGZY_HISTORY	1
#define TREND_SET	2
#define TREND_HISTORY	3

#define CURVE_NUM 9  /* added by zyp for gtk */


#endif
