/**********************************
  SYN by hcl 2007.12.12
***********************************/
#include <math.h>



#define	DBX0				10		/*	wdk 97.3.13	*/
#define	DBY0				60
#define	DBY1				30
#define	DBH0				30
#define DB_VAL_COL_WIDTH	100
#define DB_VAL_STR_WIDTH	80
#define DBIO_X0				75
#define DBIO_X1				435


#define	DB_DETAIL_ROW_NUM_PER_COL	32

#define	DB_DETAIL_ENTRY_X		20		
#define	DB_DETAIL_VALUE_X		100
#define DB_DETAIL_ENTRY_Y		20
#define	DB_DETAIL_COL_WIDTH		300
#define	DB_DETAIL_ROW_HEIGHT	20


#define	DBTAB_WINDOW_START_Y	150

#define	DBTAB_WINDOW_WIDTH		1240
#define DBTAB_WINDOW_HEIGHT		(1024-DBTAB_WINDOW_START_Y)

#define	DB_CANVAS_WINDOW_WIDTH	DBTAB_WINDOW_WIDTH
#define DB_CANVAS_WINDOW_HEIGHT	(DBTAB_WINDOW_HEIGHT-60)

#define DBINFO_WINDOW_WIDTH		1280
#define DBINFO_WINDOW_HEIGHT	800

#define	DB_TAG_RECT_X1			8
#define	DB_TAG_RECT_X2			(DB_CANVAS_WINDOW_WIDTH-10)


#define DBTAB_PAGE_SIZE	 		((DB_CANVAS_WINDOW_HEIGHT-60)/DBH0)


typedef struct 
{
	int			crt_n;
	POINTER		select_pt;
	int			subwindow_select_flag;
	GtkWidget	*stn_optionmenu, *dev_optionmenu, *datatype_optionmenu;
	/*GtkWidget	*searchdlg,*SearchEntryName_entry,*SearchPtId_entry,*SearchEntryName_radio,*SearchPtId_radio;*/
} DB_TAB_WINDOW_INFO;

#define  FONT_SIZE_DB_TAB   ((char*)(info[0].db_font))  //  "sans 13"


#define	DB_TIME_INTERVAL	1	/* 1 seconds for db table updating	*/



extern int	db_pid_cnt[MAX_CRT_NUMBER];
extern int	db_page_numb[MAX_CRT_NUMBER];
extern int	db_page_cnt[MAX_CRT_NUMBER];

extern GtkWidget *db_pop_shell[MAX_CRT_NUMBER];

extern GtkWidget *db_scrolled_window, *cmbDbDev,*db_detail_info_scrolled_window,
	*db_detail_info[MAX_CRT_NUMBER],
	*db_tab_canvas[MAX_CRT_NUMBER],
	*db_detail_info_canvas[MAX_CRT_NUMBER],
	*db_io_set_box[MAX_CRT_NUMBER];

extern GdkGC *db_draw_gc[MAX_CRT_NUMBER],*db_detail_draw_gc[MAX_CRT_NUMBER];


extern POINTER		db_selected_ptr[MAX_CRT_NUMBER];	/* for db tracing	*/
extern int	db_select_type[MAX_CRT_NUMBER]; 
extern char		name_item[POINT_LONGNAME_SIZE];

extern BACKRCD	db_tag_rcd[MAX_CRT_NUMBER];			/* db io tagged item rcd	*/
extern int	db_tag[MAX_CRT_NUMBER]; 




#define	SEL_DB_PNT		1
#define	SEL_NO_DB_PNT		0

#define	DB_IO_DEAD_BAND_SET	ANA_PARAM_SET_D_BAND 	/*0*/
#define	DB_IO_HH_LMT_SET	ANA_PARAM_SET_HI_ALARM	/*1*/
#define	DB_IO_H_LMT_SET		ANA_PARAM_SET_HI_WARN	/*2*/
#define	DB_IO_L_LMT_SET		ANA_PARAM_SET_LO_WARN	/*3*/
#define DB_IO_LL_LMT_SET	ANA_PARAM_SET_LO_ALARM	/*4*/
#define	DB_IO_HLMT_RTN_SET	ANA_PARAM_SET_HI_RECOVER/*5*/
#define	DB_IO_LLMT_RTN_SET	ANA_PARAM_SET_LO_RECOVER/*6*/
#define	DB_IO_EGNR_VAL_SET	ANA_PARAM_SET_ENG_CVT	/*7*/
#define	DB_IO_HIRANGE_SET	ANA_PARAM_SET_HI_RANGE	/*8*/
#define DB_IO_LORANGE_SET	ANA_PARAM_SET_LO_RANGE	/*9*/
#define	DB_IO_BASE_SET		ANA_PARAM_SET_ZERO	/*10*/
#define DB_IO_COMPENSATE_SET	ANA_PARAM_SET_COMPSATE	/*11*/
#define DB_IO_TR_WARN_SET	ANA_PARAM_SET_TR_WARN	/*12*/
#define DB_IO_TR_RECOVER_SET	ANA_PARAM_SET_TR_RECOVER/*13*/

#define	DB_IO_MAN_SET_ON	DB_IO_TR_RECOVER_SET+1
#define	DB_IO_MAN_SET_OFF	DB_IO_TR_RECOVER_SET+2
#define	DB_IO_ALM_ACK		DB_IO_TR_RECOVER_SET+3
#define	DB_IO_ALM_ENABLE	DB_IO_TR_RECOVER_SET+4
#define	DB_IO_ALM_DISABLE	DB_IO_TR_RECOVER_SET+5
#define	DB_IO_SCAN_ENABLE	DB_IO_TR_RECOVER_SET+6
#define	DB_IO_SCAN_DISABLE	DB_IO_TR_RECOVER_SET+7
#define	DB_IO_VALUE_SET		DB_IO_TR_RECOVER_SET+8
#define DB_IO_OUTS_FORCE_ON     DB_IO_TR_RECOVER_SET+9 /*22*/
#define DB_IO_OUTS_FORCE_OFF    DB_IO_TR_RECOVER_SET+10
#define DB_IO_OUTS_UNFORCE      DB_IO_TR_RECOVER_SET+11
#define DB_IO_OUTA_UNFORCE      DB_IO_TR_RECOVER_SET+12
#define DB_IO_OUTA_FORCE_VAL    DB_IO_TR_RECOVER_SET+13

typedef	struct data_qua_name
{
	char	name_qua[20];
	char	name_scan[20];
	char	name_alm[20];
	char	name_almack[20];
/*#ifdef	V3R0      Ignored by zyp on 2006.2
	short	point_num_dsp;
#endif*/
} DB_DA_STATUS_NAME;



#define		 TEMP_col	 	12	/*sizeof(TEMP_titles)  /sizeof(TEMP_titles[0])  */
#define		 POL_col	 	 9	/* sizeof(POL_titles)   /sizeof(POL_titles[0])  */
#define		 IMP_col		11	/* sizeof(IMP_titles)   /sizeof(IMP_titles[0])   */
#define		 CURVE_col	 	10	 /*sizeof(CURVE_titles) /sizeof(CURVE_titles[0])  */


extern  gchar *ANA_titles[];
extern  gchar *TEMP_titles[];

extern  gchar *POL_titles[];
extern  gchar *IMP_titles[];
extern  gchar *CURVE_titles[];

/*extern  int  TEMP_col,POL_col,IMP_col,CURVE_col;  */


#define MAX_DB_COLUMN MAX(/*ANA_col,*/IMP_col)
