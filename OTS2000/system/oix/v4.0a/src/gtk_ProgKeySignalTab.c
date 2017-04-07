/*******************************************
2005.11 created by zyp 
2006.03 updated by chi hailong  by gtk
2007.06.22 : have been SYNC  by chichi;
LAST UPATE : 2007.12.03 by chichi ; 
******************************************/


#include <stdio.h>


#include <string.h>
#include <gtk/gtk.h>
#include <iconv.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include	"../inc/oix.h"
#include	"../inc/gtk_widgets.h"

#include        "../../../dps/v4.0a/inc/fault_table_def.h"
#include        "../../../dps/v4.0a/inc/dps_functions_def.h"


#define	SRC_NAME_DEF
#define HISTORY_DB_ALM 
#ifndef min 
#define min(a,b)		(((a)  <  (b))  ? (a)  : (b))
#endif 


#ifndef max 
#define max(a,b)		(((a)  >  (b))  ? (a)  : (b))
#endif 


#define	DEBUG	 
#define  INTERVAL     100
#define  SECNUM		   10     /*10*100 =  1000ms */	


extern int PrintTitleLocation[8];
extern int PrintElementLocation[8] ;
extern char PrintLocationRuler[PRINT_RULER_LEN];





PAGE_INFO		page_k[MAX_CRT_NUMBER];

int last_crt_k=0;

extern gint num_screen;
extern char OIX_EVENT_DSP_LOGICNAME ; 
extern char OIX_EVENT_ALIAN_MOD ; 
HISTORY_RECORD          hist_rec_addr_k[MAX_CRT_NUMBER] ;
static GMutex *alm_mutex_k[MAX_CRT_NUMBER] = {NULL,NULL,NULL};

SHM_HDB                 *shm_hdb_alm_k;
#define ALM_REC_REAL1			0
#define ALM_REC_HISTRY1			1
#define ALM_REC_SYS_INFO		2
#define  ALM_REC_HISTRYDB		3
#define ALM_REC_FILE_SEARCH	        4
#define ALL_STATION     MAX_STN_NUM
#define SHOW_ALL     99
#define SHOW_NONE    100
#define MAX_FILTER_NUMBER		32
#define MAX_FILTER_NAME_LEN		64
#define MAX_UNIT_STATUS_NUM		32

#define DEBUG
int HistDBAlmDlgDelete_k (GtkWidget *w , gpointer data);
int HistDBFileAlmDlgDelete_k (GtkWidget *w , gpointer data);
extern int	blink_flag[MAX_CRT_NUMBER];
extern int	GetAlmStrings(REC_ENTRY_UNION *rec, ALM_STR *a, BOOL *blink, COLOR_PAIR *color);
extern int	GetAnaAlmStrings(ANLG_REC *ana_rec, ALM_STR *a, BOOL *blink, COLOR_PAIR *color);
extern int	GetImpAlmStrings(ANLG_REC *ana_rec, ALM_STR *a, BOOL *blink, COLOR_PAIR *color);
extern int	GetIndAlmStrings(IND_REC *ind_rec, ALM_STR *a, BOOL *blink, COLOR_PAIR *color);
extern int	GetParamSetAlmStrings(PARAM_SET_REC *param_set_rec, ALM_STR *a, BOOL *blink, COLOR_PAIR *color);
extern int	GetLmsAlmStrings(LMS_REC *lms_rec, ALM_STR *a, BOOL *blink, COLOR_PAIR *color);
extern int	GetSmsAlmStrings(SMS_REC *sms_rec, ALM_STR *a, BOOL *blink, COLOR_PAIR *color);
extern int	GetCmdRecStrings(CMD_REC *cmd_rec, ALM_STR *a, BOOL *blink, COLOR_PAIR *color);
extern int	GetCmdSrcStr(USHORT	src_id, char *cmd_src);
extern int      GetAlmPointer(REC_ENTRY_UNION *rec , POINTER *pt) ;
GtkWidget *CreateHistDBFileAlmDlg_k(GtkWidget *parent ,int crt_n);
extern int	alm_rec_fault(FILTER_T filter, INDEX *index);
extern int    gdk_rgb(int red  ,int green, int  blue , GdkColor *clr  );
extern void  fault_alarm_expose_k(GtkWidget *w ,  GdkEventExpose *event, gpointer data)  ;
extern int DrawFaultAlarm_k(int crt_n ,GdkDrawable *drawable,GdkGC  *gc  );
extern void GetCrtNumberOfWgt ( GtkWidget  *wgt, int     *crt_n );
extern gint  AlmDspProc_k( crt_n );
extern gint SetDataTypeMask_k(GtkWidget  *widget, gpointer data);
extern int OpenDspFile (int crt_n, char	*filename);
extern int	alm_rec_filter_k(FILTER_T filter, INDEX *index);
extern FILE    *OpenFileX (char    *full_name);
extern int    GetHost_Cmd();
extern windk_text_height    (GdkFont  *font,  const gchar  *text,  gint txt_length ,char *fontdesc);
extern GtkWidget* PopupDetailInfoBox(GtkWidget *parent,int crt_n,POINTER pt);
extern gint doAlarmAckEx_k(GtkWidget *widget, int crt_n ,  int startI ,int  endI  ,BOOL isRangeAck );
extern void PrintList(GtkWidget*parent,int crt_n,int prt_obj_type ,char *output_file);
extern void PopFileSelectionDlg(GtkWidget *parent ,int crt_n,char *title,char *dir);
extern void	PopupMessageBox (GtkWidget *parent,char msg[] );
extern void popupErrorMsgBox(GtkWidget* pParent, gchar* strMsg);
extern char *_fromUtf8(char *c);
extern char *_toUtf8EX(char *c);
extern char *_toUtf8(char *c);
extern BOOL  isStnInMask (UCHAR stn_id) ;  
extern BOOL IsFileEndStr(char *str);
extern void    windk_draw_text(GdkDrawable * drawable ,GdkFont    *font,GdkGC  *gc, int x,int y,const char      *text,int  strlength,int crt_n) ;
void    windk_draw_text_EX_k(GdkDrawable * drawable ,char  *fontdescr ,GdkGC  *gc, int x,int y,const char      *text,int  strlength,int crt_n ) ;
extern          SHM_RECORD	*shm_rec_addr;
extern          FAULT_RECORD_TABLE *shm_fault_addr;
extern			INFO_RECORD		*shm_info_rec_addr;   //added by chi 070911
extern gint num_screen;
extern			SYS_COMM_TABLE  *shm_syscomm_addr;
void            SwapRecEntryByte(REC_ENTRY_UNION *rec_entry);

void 	ProgKeySignalTab (GtkWidget *parent,	int crt_n  ,int TabMode  );
extern void            InitAlarmFilter(FILTER_T *pFilter,int crt_n);
static void *FetchDataFromSHM_k(void *arg);
static          GtkWidget* CreateHistDBAlmDlg_k(GtkWidget *parent ,int crt_n);
void  cb_scroll_page_k(GtkAdjustment *adj , int crt_n);
extern int ReadFilterXml(GtkListStore *listStore,const char *nodeName);
extern int SaveFilterXml(gchar *filterName,FILTER_T *pFilter,const char *nodeName);
extern int DelFilterXml(gchar *filterName,const char *nodeName);
GtkAdjustment  *adj1_k[MAX_CRT_NUMBER] ;
char   dest_filename_k[256] ; //历史搜索用临时文件 search_rec_file.rec
EVENT_SEARCH  eventFilter_k ;
/*thread_t 	almtid[NUM_THREADS]; */
extern  unsigned int  stnMask4alarm ; 
int curDev_a_k;


static GtkWidget *CreateAlmToolbar_k (GtkWidget * ,int);
void	ProgAlarmAckBrdcst_k ( COMMAND_MSG *alm_msg,  int send_num);
void	DrawAlmTabHead_k(GdkDrawable *drawable ,GdkGC *gc ,int crt_n );
static  GtkWidget* CreateProgAlarmTab_k (GtkWidget *parent,	int crt_n   );
static  GtkWidget* CreateDevFilterDlg_k(GtkWidget *parent ,int crt_n);
static  GtkWidget* CreateCustomFilterDlg_k(GtkWidget *parent ,int crt_n);
static  GtkWidget* CreateAttribFilterDlg_k(GtkWidget *parent ,int crt_n);
int     ReadHistRecFile_k (int crt_n, char *filename);
extern int getMenuDataPointer(int crtn,int data);
extern int getCrtNumFromPointer(int data);
extern int getDataFromPointer(int data);
extern char *TimeToString(time_t tmTime);
extern time_t StringToTime(char * szTime); 
gint FiterInit_k(GtkWidget*w , gpointer data);

static  BOOL	alm_exit_flag_k[MAX_CRT_NUMBER] = {FALSE,FALSE,FALSE};
INDEX   	index_tmp_k[MAX_CRT_NUMBER],index_tmp1[MAX_CRT_NUMBER];
INDEX   	index_fault_k[MAX_CRT_NUMBER];
SEL_PT   	sel_pt_k[MAX_CRT_NUMBER];
int			iHisNameMode_k[MAX_CRT_NUMBER] ;
int			alm_rec_mode_k[MAX_CRT_NUMBER];

char *AlarmHeadTitle_k[] = {OIX_STR_REC_NUMBER   , OIX_STR_TIME  ,OIX_STR_REC_LOGNAME, OIX_STR_MSG_NAME  ,OIX_STR_RT_STATUS,"",OIX_STR_USER};
int dsp_ID_k=0;

UINT    	devMask_k[MAX_CRT_NUMBER] = {0xffffffff,0xffffffff,0xffffffff};
UINT		groupMask_k[MAX_CRT_NUMBER][MAX_GROUP_MASK_NUM] = {0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,
															 0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,
															 0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,
															 0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,
															 0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,
															 0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,
															 0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,
															 0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,
															 0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,
															 0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,
															 0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,
															 0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,
															 0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,
															 0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,
															 0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff};
UINT		attribMask_k[MAX_CRT_NUMBER][2] = {0,0,0,0,0,0};
int     	alm_rec_cnt_k[MAX_CRT_NUMBER];
gint		isLastPage_k[MAX_CRT_NUMBER]={0,0,0};
FILTER_T 	FilterAlarm_k[MAX_CRT_NUMBER],FilterAlarm1[MAX_CRT_NUMBER];
FILTER_T 	FilterAlarmFault_k[MAX_CRT_NUMBER];
GtkWidget       *filterDlg_k[MAX_CRT_NUMBER] ={ NULL,NULL,NULL};
GtkWidget		*AttribFilterDlg_k[MAX_CRT_NUMBER] ={ NULL,NULL,NULL};
GtkWidget       *HistDBDlg_k[MAX_CRT_NUMBER] ={ NULL,NULL,NULL};
GtkWidget       *HistFileDBDlg_k[MAX_CRT_NUMBER] ={ NULL,NULL,NULL};
GtkWidget       *SetFilterDlg_k[MAX_CRT_NUMBER] ={ NULL,NULL,NULL};

extern 		int lcu_num;
extern 		int lcu_status[];
GtkWidget 	*alm_pop_shell_k[MAX_CRT_NUMBER]={NULL,NULL,NULL};
GtkWidget 	*alm_canvas_k[MAX_CRT_NUMBER]={NULL,NULL,NULL};
GdkGC  		*alm_draw_gc_k[MAX_CRT_NUMBER]={NULL,NULL,NULL};
GtkWidget 	*item_k[MAX_CRT_NUMBER][GROUP_ENTRY_NUMBER];
GtkWidget	*DataTypeItem_k[MAX_CRT_NUMBER][MAX_DATATYPE_NUM + 2];
GtkWidget	*attribCheck_k[MAX_CRT_NUMBER][MAX_ATTRIB_NUM],*attribToggle_k[MAX_CRT_NUMBER][MAX_ATTRIB_NUM];
GtkWidget	*DevItem_k[MAX_CRT_NUMBER][SHOW_NONE];
GtkListStore *storeFilter_a_k[MAX_CRT_NUMBER],*storeDev_a_k[MAX_CRT_NUMBER],*storeGroup_a_k[MAX_CRT_NUMBER];
int	filterNum_k,curFilter_k;
gboolean	auto_filter_flag_k[MAX_CRT_NUMBER];
GdkPixmap 	*AlmOffDrawable_k[MAX_CRT_NUMBER]= {NULL,NULL,NULL};
gint 	alm_timeout_id_k[MAX_CRT_NUMBER] ={0,0,0} ;
gint 	fault_alm_timeout_id_k[MAX_CRT_NUMBER] ={0,0,0} ;
STNDEF	keySignalStnDef[STATION_ENTRY_NUMBER];
gint 	autofilter_timeout_id_k[MAX_CRT_NUMBER] ={0,0,0} ;

typedef struct
{
	int  crt_n ; 
	GtkWidget *canvas ;
	GdkPixmap *pixmap , *back_pixmap;
	GdkGC  *gc ; 
} XHDC ;    /* draw devcie context !!! */

void  init_alm_mutex_k(int crt_n )
{
	g_assert (alm_mutex_k[crt_n] == NULL);
	
	alm_mutex_k[crt_n]  = g_mutex_new ();
	
}

void  free_alm_mutex_k(int crt_n )
{
	g_assert (alm_mutex_k[crt_n] != NULL);
	
	g_mutex_free (	alm_mutex_k[crt_n] );
	
	alm_mutex_k[crt_n] = NULL ; 
}

int   alarm_scroll_event_k (GtkWidget*widget,   GdkEventScroll *event, gpointer  data)  
{   int crt_n  = (int) data; 

if (event->direction == GDK_SCROLL_DOWN)
{
	// down code
	adj1_k[crt_n]->value ++ ; 
	//printf ("---------scroll++++++++++++++++++++1\n");
}
else if (event->direction == GDK_SCROLL_UP)
{
	// up code
	adj1_k[crt_n]->value -- ; 
	//printf ("---------scroll---------------------1\n");
}

cb_scroll_page_k(adj1_k[crt_n] ,   crt_n);

return FALSE;


}

/****************  Timer  call  !!!      ************/
gint  FaultAlmDspProc_k(gpointer  data)
{
	XHDC *dc_k =  (XHDC *)data ;
	GdkColor clr ;
	
	
	gdk_rgb(51,51,51,&clr);
	gdk_gc_set_foreground(dc_k->gc, &clr);
	
	gdk_draw_rectangle (dc_k->pixmap,dc_k->gc,TRUE, 0,0,LOCAL_WIDTH , 1600);	   
	/*printf("index_tmp1[%d].header=%d,shm_rec_addr->head.header_ptr=%d\n",dc_k->crt_n,index_tmp1[dc_k->crt_n].header,shm_rec_addr->head.header_ptr);
	*/	   
	DrawFaultAlarm_k(dc_k->crt_n ,dc_k->pixmap,dc_k->gc  );
	gdk_window_copy_area (dc_k->canvas->window,dc_k->gc,0,0,dc_k->pixmap ,0,0,LOCAL_WIDTH, 1600);
	return TRUE;
	
}


gint fault_alarm_click_k( GtkWidget *w ,GdkEvent *event  , gpointer data)
{
	gint  crt_n = (gint)  data ;
	
	if (event->type == GDK_2BUTTON_PRESS) {  /*  GDK_2BUTTON_PRESS ---double clicked !!!! */
		GdkEventButton *bevent = (GdkEventButton *) event; 
		if (bevent->button == 1)  
		{	
			ProgKeySignalTab (main_w[crt_n],crt_n ,FALSE);
			return TRUE ; 
		}
	}
	return TRUE;
}

gint onlyDisplayNoAck_k( GtkWidget *w , gpointer data)
{
	
	
	
	
	
	
	
	gint crt_n = (gint )data;
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER ) return FALSE;
	if (gtk_toggle_button_get_active  (GTK_TOGGLE_BUTTON(w)))
		FilterAlarm_k[crt_n].isNoAck = 1   ;
	else
		FilterAlarm_k[crt_n].isNoAck = 0   ; 
	
	FilterAlarm_k[crt_n].is_change  = 1 ; 
	
	return AlmDspProc_k( crt_n );
}

void SetTypeIdMask_k (GtkWidget * widget, gpointer data)
{    
	int i  ;
	int crt_n ;
	//GetCrtNumberOfWgt (widget, &crt_n );
	//data=(int)data;
	crt_n=getCrtNumFromPointer((int)data);
	i=getDataFromPointer((int)data);
	
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return ;
	
	/*   g_message("toggle[%d]  \n" ,i  );   */
	FilterAlarm_k[crt_n].cmd_type  = i ; 
	
	FilterAlarm_k[crt_n].is_change = TRUE; 
	FilterAlarm_k[crt_n].change_flag =TRUE;
	AlmDspProc_k( crt_n );
	
}
void  InitToolBarButton_k(GtkWidget*parent,int crt_n)
{
	GtkWidget  *opmenu; 
	int MenuPointer;
	
	opmenu = g_object_get_data(G_OBJECT(parent),"StationOpMenu" );
	gtk_option_menu_set_history( GTK_OPTION_MENU(opmenu) ,0);

	opmenu = g_object_get_data(G_OBJECT(parent),"TypeIdOpMenu" );
	gtk_option_menu_set_history     ( GTK_OPTION_MENU(opmenu) ,0);
	
	opmenu = g_object_get_data(G_OBJECT(parent),"DataTypeOpMenu" );
	gtk_option_menu_set_history     ( GTK_OPTION_MENU(opmenu) ,0);
	
	//hcl
	MenuPointer=getMenuDataPointer(crt_n,MAX_DATATYPE_NUM);
	SetDataTypeMask_k(DataTypeItem_k[crt_n][MAX_DATATYPE_NUM], GINT_TO_POINTER ( MenuPointer)) ;  /* set all data type ; */
	
}


void  fault_alarm_expose_k(GtkWidget *w ,  GdkEventExpose *event, gpointer data)  
{ 
	XHDC  dc_k ;
	int crt_n = (gint) data ;
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER ) return ;
	dc_k.crt_n = (gint ) data;
	dc_k.canvas = w ;
	dc_k.pixmap = pixmap2[dc_k.crt_n];
	dc_k.gc = info[dc_k.crt_n].gc2 ; 
	FaultAlmDspProc_k( &dc_k);
}



int DrawFaultAlarm_k(int crt_n ,GdkDrawable *drawable,GdkGC  *gc  )
{
	extern int dsp_rec_ptr;
	static int dsp_time_out=0;
	
	POINTER cur_pt;
	DMS_COMMON dms_cmm;
	int i , j ;
	ALM_STR			alarm_str ;
	BOOL			blink ;
	COLOR_PAIR		color ;
	static short blankstate[MAX_CRT_NUMBER];
	/*char Buff_k[20] ;*/
	int fault_count;
	char record[20];
	/*char hostname[25];*/   
	/*POINT_NAME pt_name;*/  
	//hcl updata for double screen	
	/*
	
	  /*#ifdef SUN_OS    
	  g_display = gdk_display_get_default ();
	  #endif
	  num_screen = gdk_display_get_n_screens (g_display); 
	  #endif 
	  #ifdef WINDOWS_OS 
	  num_screen=SCREENNUM;
#endif*/
	InitAlarmFilter( &FilterAlarmFault_k[crt_n],crt_n);   
	FilterAlarmFault_k[crt_n].pRec   = shm_rec_addr->rec;   
	FilterAlarmFault_k[crt_n].phead  = &shm_rec_addr->head ; 
	index_fault_k[crt_n].buff_size = 10 ;
	
	
	
	if (index_fault_k[crt_n].header != shm_rec_addr->head.header_ptr )  
	{
		fault_count =  index_fault_k[crt_n].count ;
		index_fault_k[crt_n].count = 0 ;
		
		index_fault_k[crt_n].header = shm_rec_addr->head.header_ptr;          
		
		alm_rec_fault(FilterAlarmFault_k[crt_n], &index_fault_k[crt_n]);              
		
		if ( index_fault_k[crt_n].count>fault_count && crt_n ==1)
		{
			gdk_beep();
			
		}
	}
	
	for (i = 0 , j= 0 ; i < index_fault_k[crt_n].count;j++, i++)
	{		 
		GetAlmStrings(&shm_rec_addr->rec[index_fault_k[crt_n].index_pt[i].pt], &alarm_str , &blink , &color );
		/* if((index_fault_k[crt_n].index_pt[i]==last_fault_no)&&(crt_n==0))
		new_fault_no =i;*/
		
		if (blink)
		{                                   
			gdk_gc_set_foreground ( gc, &pixels[crt_n][ color.clr[blankstate[crt_n]]]);	
		}
		else
			gdk_gc_set_foreground ( gc, &pixels[crt_n][color.clr[0]]);           
		
		
		sprintf(record , "%s: ",  alarm_str.sn_str);
		strcat(record , alarm_str.repeatAlm);  
		/*windk_draw_text( drawable, font_set[2], gc,  ALM_X0, FAULT_ALM_Y1 + j * ALM_ROW , record);*/
		windk_draw_text( drawable,font_set[2], gc,  FAULT_ALM_X0, FAULT_ALM_Y1 + j * ALM_ROW , alarm_str.time_str ,strlen(alarm_str.time_str),crt_n);
        windk_draw_text( drawable,font_set[2], gc,  FAULT_ALM_X1, FAULT_ALM_Y1 + j * ALM_ROW , alarm_str.logicname ,strlen(alarm_str.logicname),crt_n);
		windk_draw_text( drawable,font_set[2], gc,  FAULT_ALM_X2, FAULT_ALM_Y1 + j * ALM_ROW , alarm_str.longname,strlen(alarm_str.longname),crt_n);
        windk_draw_text( drawable,font_set[2], gc,  FAULT_ALM_X3+300 , FAULT_ALM_Y1 + j * ALM_ROW , alarm_str.cmd_result,strlen(alarm_str.cmd_result),crt_n);
        windk_draw_text( drawable,font_set[2], gc,  FAULT_ALM_X3, FAULT_ALM_Y1 + j * ALM_ROW , alarm_str.message ,strlen(alarm_str.message ),crt_n);
		windk_draw_text( drawable,font_set[2], gc,  FAULT_ALM_X3 +  200, FAULT_ALM_Y1 + j * ALM_ROW , alarm_str.state_str,strlen(alarm_str.state_str),crt_n);
		/*   RM BY CHICHI
		if (strcmp(alarm_str.usrname,"")!=0)
		sprintf(hostname , "%s@%s", alarm_str.usrname, alarm_str.hostname);
		else
		sprintf(hostname , "%s",   alarm_str.hostname);
		windk_draw_text( drawable, font_set[0], gc,  ALM_X4, FAULT_ALM_Y1 + j * ALM_ROW , hostname);	
		
		*/
		
	}			
	if   (blankstate[crt_n] >0)
		blankstate[crt_n] = 0 ;
	else
		blankstate[crt_n] = 1 ;
	
	/* added by zyp on 2006.9.26 for display the dspfile auto */
	
	if(crt_n==0)
	{  
		dsp_time_out++;
		if(dsp_time_out>100)
			dsp_time_out =100; /* dsp_time_out<32767*/
		for(;dsp_rec_ptr != shm_rec_addr->head.header_ptr;
		dsp_rec_ptr++,dsp_rec_ptr=(dsp_rec_ptr>=shm_rec_addr->head.buf_size)? 0:dsp_rec_ptr)
		{
			if(!IsAlmRecord(&shm_rec_addr->rec[dsp_rec_ptr]))
				continue;
			if(-1==GetPointFromRec(&shm_rec_addr->rec[dsp_rec_ptr],&cur_pt))              
				continue;
			//printf("###i=%d,cur_pt=%d.%d.%d.%d\n",dsp_rec_ptr,cur_pt.stn_id,cur_pt.dev_id,cur_pt.data_type,cur_pt.pt_id); 
			
			if(ReadEntryById(&cur_pt,&dms_cmm)!=-1)
				if((strcmp(dms_cmm.fixed.dsp_filename,"")!=0)&&(dms_cmm.fixed.ioattr.fault==1||dms_cmm.fixed.ioattr.fail==1))//事故或者故障点才推光字
				{      
					printf("####dsp_filename=%s\n",dms_cmm.fixed.dsp_filename);
					if(dsp_time_out>30)
					{
						last_crt_k =CRT_2;       
						printf("dsp_time_out=%d,last_crt_k=%d\n",dsp_time_out,last_crt_k);
					}
					else
					{
						last_crt_k ++;
						if(last_crt_k>num_screen-1)
							last_crt_k = CRT_2;
					}                        
					
					if(last_crt_k>=num_screen)
						last_crt_k= num_screen-1;
					
					{                                   
						OpenDspFile(last_crt_k,dms_cmm.fixed.dsp_filename);  
						dsp_time_out =0;                      
						
						printf("dsp opened on crt %d,file=%s\n",last_crt_k,dms_cmm.fixed.dsp_filename);
						
					}
				}
				
		}
	}
	/* end by zyp */
	return TRUE;
	
}

GdkRectangle   AlarmCanasRECT_k[MAX_CRT_NUMBER];
void  alarm_canvas_allocate_k(GtkWidget *widget,GtkAllocation *a ,gpointer data) 
{
	int crt_n = (gint) data ; 
	AlarmCanasRECT_k[crt_n].width =  a->width ;
	AlarmCanasRECT_k[crt_n].height =  a->height ;
	page_k[crt_n].PageSize =   AlarmCanasRECT_k[crt_n].height/ (int)(ALM_ROW*yscale[crt_n]) ;
	page_k[crt_n].PageSize -- ;   
	/* g_message("alloresize ****** alarm page_k is %d \n", page_k[crt_n].PageSize);  */
	
}



XHDC  dc_k[MAX_CRT_NUMBER] ;
int CreateFaultAlarm_k(int crt_n ,GtkWidget *DrawArea,GdkGC  *gc  )
{
	
	dc_k[crt_n].crt_n = crt_n;
	dc_k[crt_n].canvas = DrawArea ;
	dc_k[crt_n].pixmap = pixmap2[ crt_n];
	dc_k[crt_n].gc = info[crt_n].gc2 ;
	
	
	gtk_widget_set_events(DrawArea, GDK_EXPOSURE_MASK  
		| GDK_LEAVE_NOTIFY_MASK
		| GDK_BUTTON_PRESS_MASK
		| GDK_POINTER_MOTION_MASK );
	
	
	gtk_signal_connect (GTK_OBJECT (DrawArea),"expose_event",GTK_SIGNAL_FUNC(fault_alarm_expose_k),GINT_TO_POINTER (crt_n)) ;
	gtk_signal_connect (GTK_OBJECT (DrawArea),"button_press_event",GTK_SIGNAL_FUNC(fault_alarm_click_k),GINT_TO_POINTER (crt_n)) ;
	
	if (fault_alm_timeout_id_k[crt_n] != 0 )
	{
		gtk_timeout_remove(fault_alm_timeout_id_k[crt_n]);
		fault_alm_timeout_id_k[crt_n]= 0;
	}
	fault_alm_timeout_id_k[crt_n]=gtk_timeout_add ( 1000,(GtkFunction)FaultAlmDspProc_k, (gpointer) &dc_k[crt_n] );  
	printf("^^^index_tmp1[crt_n].header=%d,shm_rec_addr->head.header_ptr=%d\n",index_tmp1[crt_n].header,shm_rec_addr->head.header_ptr);
	
	DrawFaultAlarm_k(  crt_n ,DrawArea->window,info[crt_n].gc2  ) ;
	
	return TRUE;
}

gint SaveCustomFilter_k(GtkWidget	*widget,  gpointer	data)
{   
	GtkWidget *filterNameEntry,*filterList;
	int crt_n,i;
//	GtkTreeModel *model;
//	GtkListStore *store;
	GtkTreeIter  iter; 	
	GtkWidget *window = data;
//	gint num;
	gchar *filterName;
	gchar stn_id[2],cmd_type[2],fault_mask[9],type_id_mask[9],dev_mask[9],group_mask[450],datatype_mask[9],attrib_mask[18],start_time[30],end_time[30],logic_name[50],long_name[MAX_SEARCH_NAME_SIZE];
	int offset =0;
	GetCrtNumberOfWgt (widget, &crt_n );
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE ;
	
	if (!FilterAlarm_k[crt_n].change_flag)
	{
		PopupMessageBox(alm_pop_shell_k[crt_n],MSG_FILTER_NOT_CHANGE);
		return FALSE;
	}
	filterList = g_object_get_data(G_OBJECT(window),"customFilterList");
	filterNameEntry  = g_object_get_data(G_OBJECT(window),"customFilterNameEntry");
	if (filterNum_k > MAX_FILTER_NUMBER)
	{
		PopupMessageBox(alm_pop_shell_k[crt_n],MSG_FILTER_OUTNUMBER);
		return FALSE;
	}
	else
	{
		if (strlen(gtk_entry_get_text(GTK_ENTRY(filterNameEntry))) > MAX_FILTER_NAME_LEN)
		{
			PopupMessageBox(alm_pop_shell_k[crt_n],MSG_FILTER_NAME_MAX_LEN); 
			return FALSE;
		}
		else if (strcmp(gtk_entry_get_text(GTK_ENTRY(filterNameEntry)),"")==0)
		{
			PopupMessageBox(alm_pop_shell_k[crt_n],MSG_INPUT_FILTER_NAME);
			return FALSE;
		}
		else
		{
			if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(storeFilter_a_k[crt_n]),&iter))
			{
				do 
				{
					gtk_tree_model_get(GTK_TREE_MODEL(storeFilter_a_k[crt_n]),&iter,eName_Filter,&filterName,-1);
					if (filterName!=NULL&&strcmp(filterName,gtk_entry_get_text(GTK_ENTRY(filterNameEntry)))==0)
					{
						PopupMessageBox(alm_pop_shell_k[crt_n],MSG_FILTER_NAME_REPEAT);
						return FALSE;
					}
						
				} while (gtk_tree_model_iter_next(GTK_TREE_MODEL(storeFilter_a_k[crt_n]),&iter));
			}
			sprintf(stn_id,"%u",FilterAlarm_k[crt_n].stn_id);
			sprintf(cmd_type,"%u",FilterAlarm_k[crt_n].cmd_type);
			sprintf(fault_mask,"%u",FilterAlarm_k[crt_n].fault_mask);
			sprintf(type_id_mask,"%u",FilterAlarm_k[crt_n].type_id_mask);
			sprintf(dev_mask,"%08X",FilterAlarm_k[crt_n].dev_mask[0]);
			for (i=0;i<MAX_GROUP_MASK_NUM;i++)
			{
				offset += sprintf(group_mask + offset, "%08X", FilterAlarm_k[crt_n].group_mask[i]); 
 			}
			sprintf(datatype_mask,"%08X",FilterAlarm_k[crt_n].datatype_mask);
			offset = sprintf(attrib_mask,"%08X",FilterAlarm_k[crt_n].attrib_mask[0]);
			sprintf(attrib_mask + offset,"%08X",FilterAlarm_k[crt_n].attrib_mask[1]);
			strcpy(start_time,TimeToString(FilterAlarm_k[crt_n].start_time));
			strcpy(end_time,TimeToString(FilterAlarm_k[crt_n].end_time));
			strcpy(logic_name,FilterAlarm_k[crt_n].LogicName);
			strcpy(long_name,FilterAlarm_k[crt_n].search_str);
			gtk_list_store_insert(GTK_LIST_STORE(storeFilter_a_k[crt_n]), &iter,1);	
			gtk_list_store_set(GTK_LIST_STORE(storeFilter_a_k[crt_n]), &iter, eName_Filter,gtk_entry_get_text(GTK_ENTRY(filterNameEntry)),
																			  eStnId_Filter,stn_id,
																			  eCmdType_Filter,cmd_type,
																		  	  eFaultMask_Filter,fault_mask,
																			  eTypeIdMask_Filter,type_id_mask,
																			  eDevMask_Filter,dev_mask,
																		  	  eGroupMask_Filter,group_mask,
																			  eDataTypeMask_Filter,datatype_mask,
																			  eAttribMask_Filter,attrib_mask,
																			  eStartTime_Filter,start_time,
																			  eEndTime_Filter,end_time,
																			  eLogicName_Filter,logic_name,
																			  eLongName_Filter,long_name,-1);
			SaveFilterXml(gtk_entry_get_text(GTK_ENTRY(filterNameEntry)),&FilterAlarm_k[crt_n],"Predefine");
			gtk_entry_set_text(GTK_ENTRY(filterNameEntry),"");	 
			filterNum_k++;
		}
	}
	FilterAlarm_k[crt_n].change_flag = FALSE;
	return TRUE;
}

gint DelCustomFilter_k(GtkWidget	*widget,  gpointer	data)
{
	int crt_n;
	GtkWidget *filterList;
	gchar *filterName;
//	GtkListStore *store;
	GtkTreeModel *model;
	GtkTreeIter  iter, filter_iter;
	GtkTreeSelection *selection; 
	GtkWidget *window = data;
	GetCrtNumberOfWgt (widget, &crt_n );
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE ;
	
	filterList = g_object_get_data(G_OBJECT(window),"customFilterList");
	model = gtk_tree_view_get_model (GTK_TREE_VIEW (filterList));
	selection  = gtk_tree_view_get_selection(GTK_TREE_VIEW(filterList));
	if (gtk_tree_model_get_iter_first(model, &iter) == FALSE) 
		return FALSE;
	if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection), &model, &iter)) 
	{
		gtk_tree_model_get(model,&iter,eName_Filter,&filterName,-1);
		gtk_tree_model_filter_convert_iter_to_child_iter(GTK_TREE_MODEL_FILTER(model), &filter_iter, &iter);
		gtk_list_store_remove(GTK_LIST_STORE(storeFilter_a_k[crt_n]), &filter_iter);	
		DelFilterXml(filterName,"Predefine");
		g_free(filterName);
		filterNum_k--;
	}
	return TRUE;
}

gboolean  RowVisible_k(GtkTreeModel *model,GtkTreeIter *iter,gpointer data)
{
	gchar *filterName;
	gchar *pConvert=0, *pConvert1=0;
	gtk_tree_model_get(model, iter, eName_Filter, &filterName, -1);
	if (filterName == NULL)
		return FALSE;
	if (strcmp(filterName, pConvert=_toUtf8EX("OIX_STR_FILTER_SET"))==0 || 
		strcmp(filterName, pConvert1=_toUtf8EX("OIX_STR_FILTER_INIT"))==0)
	{
		G_SAFE_FREE(pConvert);
		G_SAFE_FREE(pConvert1);
		return FALSE;
	}
	else
	{
		G_SAFE_FREE(pConvert);
		G_SAFE_FREE(pConvert1);
		return TRUE;
	}
}

gint CustomFilterDlgDelete_k(GtkWidget *w , gpointer data);
static GtkWidget*
CreateCustomFilterDlg_k(GtkWidget *parent ,int crt_n)
{
	GtkWidget *window;
	GtkWidget *vbox,*hbox1,*hbox2;
	GtkWidget *list;
	GtkWidget *scrolled_window; 
	GtkWidget *separator; 
	GtkWidget *button1,*button2,*button3; 
//	GtkWidget *list_item; 
	GtkWidget *entry;
	GtkWidget *label;
	GtkTreeModel *modelfilter;
	GtkCellRenderer    *renderer;
	GtkTreeViewColumn  *column;
//	GtkListStore       *store;
//	int i;

	gchar *pConvert=0;
	
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), pConvert=_toUtf8EX("OIX_STR_FILTER_SET"));
	G_SAFE_FREE(pConvert);
	gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(parent)); 
	gtk_window_set_destroy_with_parent(GTK_WINDOW(window),TRUE);
	
	gtk_signal_connect (GTK_OBJECT (window), "delete_event", 
		GTK_SIGNAL_FUNC (CustomFilterDlgDelete_k), GINT_TO_POINTER (crt_n)); 
	
	gtk_widget_show (window);
	vbox = gtk_vbox_new (FALSE, 5);
	gtk_widget_show (vbox);
	gtk_container_border_width(GTK_CONTAINER(vbox), 5); 
	gtk_container_add (GTK_CONTAINER (window), vbox);
	
	scrolled_window=gtk_scrolled_window_new(NULL, NULL);
	gtk_widget_set_usize(scrolled_window, 250, 150); 
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
	gtk_container_add(GTK_CONTAINER(vbox), scrolled_window); 
	gtk_widget_show(scrolled_window); 
	
	list = gtk_tree_view_new();
	//gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (scrolled_window), list);
	g_object_set_data(G_OBJECT(window),"customFilterList",list);
	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes(pConvert=_toUtf8EX("OIX_STR_FILTER_NAME"),renderer, "text", eName_Filter, NULL);
	G_SAFE_FREE(pConvert);
	gtk_tree_view_append_column(GTK_TREE_VIEW (list), column);
	modelfilter = gtk_tree_model_filter_new(GTK_TREE_MODEL(storeFilter_a_k[crt_n]),NULL);
	gtk_tree_model_filter_set_visible_func(GTK_TREE_MODEL_FILTER(modelfilter),RowVisible_k,NULL,NULL);
	gtk_tree_view_set_model(GTK_TREE_VIEW (list), GTK_TREE_MODEL(modelfilter));
	g_object_unref(storeFilter_a_k[crt_n]);
	gtk_container_add (GTK_CONTAINER (scrolled_window), list);
	gtk_widget_show(list);
	
	hbox2 = gtk_hbox_new (FALSE, 2);
	gtk_widget_show (hbox2);
	gtk_container_border_width(GTK_CONTAINER(hbox2), 2); 
	gtk_container_add (GTK_CONTAINER (vbox), hbox2);
	
	label = gtk_label_new(pConvert=_toUtf8EX("OIX_STR_FILTER_NAME"));
	G_SAFE_FREE(pConvert);
	gtk_container_add(GTK_CONTAINER(hbox2), label);
	gtk_widget_show (label);
	
	entry = gtk_entry_new ();
	gtk_container_add(GTK_CONTAINER(hbox2), entry);
	gtk_widget_show (entry);
	g_object_set_data(G_OBJECT(window),"customFilterNameEntry",entry);
	gtk_entry_set_max_length(GTK_ENTRY(entry),MAX_FILTER_NAME_LEN);
	
	separator=gtk_hseparator_new(); 
	gtk_container_add(GTK_CONTAINER(vbox), separator); 
	gtk_widget_show(separator); 
	
	hbox1 = gtk_hbox_new (FALSE, 2);
	gtk_widget_show (hbox1);
	gtk_container_border_width(GTK_CONTAINER(hbox1), 2); 
	gtk_container_add (GTK_CONTAINER (vbox), hbox1);
	
	button1=gtk_button_new_with_label(pConvert=_toUtf8EX("OIX_CSTR_ADD")); 
	G_SAFE_FREE(pConvert);
	g_signal_connect (G_OBJECT (button1), "clicked",GTK_SIGNAL_FUNC (SaveCustomFilter_k),  window);
	gtk_container_add(GTK_CONTAINER(hbox1), button1); 
	gtk_widget_show(button1); 
	
	button2=gtk_button_new_with_label(pConvert=_toUtf8EX("OIX_CSTR_DEL")); 
	G_SAFE_FREE(pConvert);
	g_signal_connect (G_OBJECT (button2), "clicked",GTK_SIGNAL_FUNC (DelCustomFilter_k),  window);
	gtk_container_add(GTK_CONTAINER(hbox1), button2); 
	gtk_widget_show(button2); 
	
	button3=gtk_button_new_with_label(pConvert=_toUtf8EX("OIX_CSTR_EXIT")); 
	G_SAFE_FREE(pConvert);
	g_signal_connect (G_OBJECT (button3), "clicked",GTK_SIGNAL_FUNC (CustomFilterDlgDelete_k),  GINT_TO_POINTER(crt_n));
	gtk_container_add(GTK_CONTAINER(hbox1), button3); 
	gtk_widget_show(button3);

	return  window ;	
}


gint ChgCustomFilter_k(GtkComboBox* pComboBox, gpointer data)
{
	GtkTreeModel *pModel;
	GtkTreeIter iter;
	GtkWidget  *opmenu;
	int i,j;
	char *filterName;
	gchar *stn_id,*cmd_type,*fault_mask,*type_id_mask,*dev_mask,*group_mask,*datatype_mask,*attrib_mask,*start_time,*end_time,*logic_name,*long_name;
	int offset = 0;
	char  recordTypeName[RECORD_LONGNAME_SIZE];
	gchar *pConvert=0, *pConvert1=0;
	gint crt_n = (gint) data ;
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE;
	
	pModel=gtk_combo_box_get_model(GTK_COMBO_BOX(pComboBox));
	if (!gtk_combo_box_get_active_iter(GTK_COMBO_BOX(pComboBox), &iter))
	{
		return FALSE;
	}
	gtk_tree_model_get(pModel, &iter, eName_Filter, &filterName, -1);
	if (strcmp(filterName, pConvert=_toUtf8EX("OIX_STR_FILTER_SET"))==0)
	{
		if (SetFilterDlg_k[crt_n] == (GtkWidget *)NULL)
		{
			SetFilterDlg_k[crt_n] = CreateCustomFilterDlg_k( alm_pop_shell_k[crt_n] ,crt_n);
			gtk_widget_show(SetFilterDlg_k[crt_n]);
		}
		else
			gtk_window_present (GTK_WINDOW(SetFilterDlg_k[crt_n]));
		if (FilterAlarm_k[crt_n].change_flag)
			gtk_combo_box_set_active(pComboBox,-1);
		else
			gtk_combo_box_set_active(pComboBox,curFilter_k);
	}
	else if (strcmp(filterName, pConvert1=_toUtf8EX("OIX_STR_FILTER_INIT"))==0)
	{
		FiterInit_k(alm_pop_shell_k[crt_n],GINT_TO_POINTER(crt_n));
		AlmDspProc_k( crt_n);
	}
	else
	{
		gtk_tree_model_get(pModel, &iter, eStnId_Filter, &stn_id,
										  eCmdType_Filter,&cmd_type,
										  eFaultMask_Filter,&fault_mask, 
										  eTypeIdMask_Filter,&type_id_mask,
										  eDevMask_Filter,&dev_mask,
										  eGroupMask_Filter,&group_mask,
										  eDataTypeMask_Filter,&datatype_mask,
										  eAttribMask_Filter,&attrib_mask,
										  eStartTime_Filter,&start_time,
										  eEndTime_Filter,&end_time,
										  eLogicName_Filter,&logic_name,
										  eLongName_Filter,&long_name,-1);
		FilterAlarm_k[crt_n].stn_id = (UCHAR)atoi(stn_id);
		FilterAlarm_k[crt_n].cmd_type = (UCHAR)atoi(cmd_type);
		FilterAlarm_k[crt_n].fault_mask = (UINT)atoi(fault_mask);
		FilterAlarm_k[crt_n].type_id_mask = (UINT)atoi(type_id_mask);
		sscanf(dev_mask,"%08X",&FilterAlarm_k[crt_n].dev_mask[0]);
		for (i=0;i<MAX_GROUP_MASK_NUM;i++)
		{
			sscanf(group_mask+offset,"%08X",&FilterAlarm_k[crt_n].group_mask[i]);
			offset += 8;
		}
		sscanf(datatype_mask,"%08X",&FilterAlarm_k[crt_n].datatype_mask);
		sscanf(attrib_mask,"%08X",&FilterAlarm_k[crt_n].attrib_mask[0]);
		sscanf(attrib_mask+8,"%08X",&FilterAlarm_k[crt_n].attrib_mask[1]);
		FilterAlarm_k[crt_n].start_time = StringToTime(start_time);
		FilterAlarm_k[crt_n].end_time = StringToTime(end_time);
		if(logic_name!=NULL)
			strcpy(FilterAlarm_k[crt_n].LogicName,logic_name);
		if(long_name!=NULL)
			strcpy(FilterAlarm_k[crt_n].search_str,long_name);

		opmenu = g_object_get_data(G_OBJECT(alm_pop_shell_k[crt_n]),"StationOpMenu" );
		gtk_option_menu_set_history( GTK_OPTION_MENU(opmenu) ,(guint)FilterAlarm_k[crt_n].stn_id);

		opmenu = g_object_get_data(G_OBJECT(alm_pop_shell_k[crt_n]),"TypeIdOpMenu" );
		gtk_option_menu_set_history( GTK_OPTION_MENU(opmenu) ,(guint)FilterAlarm_k[crt_n].cmd_type);
		
		for (i=0,j=1;i<MAX_DATATYPE_NUM;i++)
		{
			if(GetRecLongnameByType((UCHAR)i, recordTypeName )!=-1)
			{
				if (FilterAlarm_k[crt_n].datatype_mask & 1<<(i-1) )    		
					gtk_check_menu_item_set_active  ( GTK_CHECK_MENU_ITEM(DataTypeItem_k[crt_n][j]),TRUE);
				else
					gtk_check_menu_item_set_active  ( GTK_CHECK_MENU_ITEM(DataTypeItem_k[crt_n][j]),FALSE);
				j++;
			}
		}
		FilterAlarm_k[crt_n].change_flag = FALSE;
		FilterAlarm_k[crt_n].is_change = TRUE;
		AlmDspProc_k( crt_n);
	}
	curFilter_k = gtk_combo_box_get_active(pComboBox);
	G_SAFE_FREE(pConvert);
	G_SAFE_FREE(pConvert1);
	return TRUE;
}

gint FiterInit_k(GtkWidget*w , gpointer data)
{
	int crt_n;
	crt_n = (gint)data ;
//	crt_n=getCrtNumFromPointer((int)data);
	
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER ) return 0 ;
	InitAlarmFilter(&FilterAlarm_k[crt_n],crt_n) ;  
	InitToolBarButton_k(alm_pop_shell_k[crt_n],crt_n)	 ;
	
	switch(alm_rec_mode_k[crt_n] )
	{
	case ALM_REC_REAL1:    /*ALM_REC_REAL1*/
		
		
		FilterAlarm_k[crt_n].phead =  &shm_rec_addr->head;
		FilterAlarm_k[crt_n].pRec   = (REC_ENTRY_UNION* )&shm_rec_addr->rec;	        
		
		break;   
		
		
	case ALM_REC_SYS_INFO :    /*综合信息*/
		
		FilterAlarm_k[crt_n].phead =  &shm_info_rec_addr->head;
		FilterAlarm_k[crt_n].pRec   =(REC_ENTRY_UNION*) &shm_info_rec_addr->rec;	        
		
		break; 
	}
	
	
	
	
	FilterAlarm_k[crt_n].is_change = TRUE;
	
	return 0 ;  
	
}


void SetAdjInfo_k(PAGE_INFO *PageInfo,GtkAdjustment *AdjSet)
{
	/*  AdjSet->lower  = 0 ; */
	/*	GtkWidget *scroll ; */
	AdjSet->upper  = (PageInfo->RecCount+ PageInfo->PageSize)/ PageInfo->PageSize   * PageInfo->PageSize  ;  
	AdjSet->page_size =  PageInfo->PageSize ;
	AdjSet->page_increment =  PageInfo->PageSize ;
	AdjSet->step_increment  = 1 ; 
	AdjSet->value =  PageInfo->PageStart ;  /* chichi */  
	gtk_adjustment_set_value(AdjSet,CLAMP( PageInfo->PageStart,0,(AdjSet->upper -AdjSet->page_size  )));
	//    scroll = g_object_get_data(GTK_OBJECT(window),"scrollbar",scrollbar);  
	
}



/*****************************
input CurPage &&  RecCount %;
******************************/
void InitPageInfo_k(PAGE_INFO *PageInfo , int crt_n,int recCount)
{
	PageInfo->RecCount   = recCount /*shm_rec_addr->head.count  */ ;
	PageInfo->PageCount  = (PageInfo->RecCount+PageInfo->PageSize-1)/PageInfo->PageSize ;
	PageInfo->CurPage    = max(PageInfo->PageCount-1 , 0) ;
	PageInfo->isSelected =0;
	PageInfo->SelectStart = PageInfo->SelectEnd =0;
	PageInfo->PageStart  =  (PageInfo->CurPage) * (PageInfo->PageSize) ;
	PageInfo->PageEnd    =   min ((PageInfo->PageStart + PageInfo->PageSize -1) , (PageInfo->RecCount-1) )   ;        
	
}
void SetPageInfo_k(PAGE_INFO *PageInfo )
{
	
	if(PageInfo->PageSize<1) /* added by zyp on 2008.2*/
		PageInfo->PageSize=1;
	PageInfo->PageCount = (PageInfo->RecCount+PageInfo->PageSize-1)/PageInfo->PageSize ;
	if (PageInfo->PageCount >1 ) 
		if (PageInfo->RecCount  %  PageInfo->PageSize)
			PageInfo->PageMod   = PageInfo->PageSize - (PageInfo->RecCount  %  PageInfo->PageSize ) ;
		else
			PageInfo->PageMod = 0 ;
		else
			PageInfo->PageMod = 0 ;
		
		if (PageInfo->CurPage < 0  )  
		{
			
			PageInfo->CurPage  = 0 ;
		}
		if ( PageInfo->CurPage > max( PageInfo->PageCount-1  ,0)) 
		{
			
			PageInfo->CurPage  = PageInfo->PageCount -1  ;
		} 
		
		
#ifdef DEBUG
		/*  printf ("PageCount:%d,PageCurPage:%d,\t PageStar:%d, PageEnd:%d , RecCount :%d \n",PageInfo->PageCount,
		PageInfo->CurPage,PageInfo->PageStart,PageInfo->PageEnd,PageInfo->RecCount );     */
#endif 
}

int  isInSel_k(crt_n ,iRec)
{   UINT j  ;
for  (j= 0 ; j<  sel_pt_k[crt_n].nextpos ; j++ )
{
	if (iRec == sel_pt_k[crt_n].sel[j]  ) return TRUE;
	
}
return   FALSE ; 
}
/*****************************  begin  code *****************************************/
void AlmDspProcE_k ( GdkDrawable *drawable , int crt_n, int isPrint)
{
	int i,j ,X_OFFSET = 0 ;
	ALM_STR			alarm_str ;
	BOOL			blink ;
	COLOR_PAIR		color ;
	static short blankstate[MAX_CRT_NUMBER];
	char Buff_k[20] ;	
	char msgRes[512] ;
	char record[64];
	char hostname[64],cmdresult[128];
	GtkWidget *labelPage ; 
	GdkColor  bkColor ; 
	FILE *fp;	
	int ruler=0;
	gchar *pConvert=0;
	DrawAlmTabHead_k( drawable ,alm_draw_gc_k[crt_n],crt_n  );
	index_tmp_k[crt_n].buff_size = MAX_SHM_RECORD_NUM ;
	if (alm_rec_mode_k[crt_n]	== ALM_REC_REAL1)  
	{
		if (FilterAlarm_k[crt_n].is_change)
			page_k[crt_n].isSelected = 0 ; 
		if ((index_tmp_k[crt_n].header != shm_rec_addr->head.header_ptr && isLastPage_k[crt_n]) 
			|| FilterAlarm_k[crt_n].is_change || (page_k[crt_n].CurPage+1)==page_k[crt_n].PageCount )  
		{
			index_tmp_k[crt_n].lastcount = index_tmp_k[crt_n].count ;
			index_tmp_k[crt_n].count = 0 ;
			index_tmp_k[crt_n].header = shm_rec_addr->head.header_ptr;
			alm_rec_filter_k(FilterAlarm_k[crt_n], &index_tmp_k[crt_n]);  
			FilterAlarm_k[crt_n].is_change = FALSE ;
			page_k[crt_n].RecCount = index_tmp_k[crt_n].count ;     
			/*printf("$$$$$$$$$$$$$$$$indxe_lastcount =%d , curcount = d%\n" ,index_tmp_k[crt_n].lastcount ,index_tmp_k[crt_n].count )	;*/					
		}
	} else if (alm_rec_mode_k[crt_n]	== ALM_REC_SYS_INFO)
	{
		if (FilterAlarm_k[crt_n].is_change)
			page_k[crt_n].isSelected = 0 ; 
		if (index_tmp_k[crt_n].header != shm_info_rec_addr->head.header_ptr || FilterAlarm_k[crt_n].is_change    )  
		{
			index_tmp_k[crt_n].lastcount = index_tmp_k[crt_n].count ;
			index_tmp_k[crt_n].count = 0 ;
			index_tmp_k[crt_n].header = shm_info_rec_addr->head.header_ptr;
			alm_rec_filter_k(FilterAlarm_k[crt_n], &index_tmp_k[crt_n]);  
			FilterAlarm_k[crt_n].is_change = FALSE ;
			page_k[crt_n].RecCount = index_tmp_k[crt_n].count ;     
			/*printf("$$$$$$$$$$$$$$$$indxe_lastcount =%d , curcount = d%\n" ,index_tmp_k[crt_n].lastcount ,index_tmp_k[crt_n].count )	;*/					
		}
	}
	else if (alm_rec_mode_k[crt_n]	== ALM_REC_HISTRYDB) 
	{
		
#ifdef   HISTORY_DB_ALM     
		page_k[crt_n].RecCount = 0 ; 
		if  (shm_hdb_alm_k->data_al.queryflag   )
		{
			gdk_rgb( 150,0,0,&bkColor )  ;
			gdk_gc_set_foreground ( alm_draw_gc_k[crt_n], &bkColor);
			windk_draw_text( drawable, font_set[3], alm_draw_gc_k[crt_n],  ALM_X0, ALM_Y1 + 4 * ALM_ROW , "Please wait for connecting History database !!!",strlen("Please wait for connecting History database !!!"),crt_n); ; 
			printf ("while  query !!!!!\n");
			return ;
		}
		if (  /* FilterAlarm_k[crt_n].is_change  && */  shm_hdb_alm_k ) 
		{
			
			index_tmp_k[crt_n].count = 0 ;
			index_tmp_k[crt_n].header = shm_hdb_alm_k->data_al.hdb_rec.head.header_ptr;
			FilterAlarm_k[crt_n].phead =  &shm_hdb_alm_k->data_al.hdb_rec.head;
			FilterAlarm_k[crt_n].pRec  = (REC_ENTRY_UNION*)(&shm_hdb_alm_k->data_al.hdb_rec.rec );
			
			alm_rec_filter_k(FilterAlarm_k[crt_n], &index_tmp_k[crt_n]);  
			FilterAlarm_k[crt_n].is_change = FALSE ;
			page_k[crt_n].RecCount = index_tmp_k[crt_n].count ;     
			
		}
#endif             
	}else   /*history file rec */
	{
		if ( FilterAlarm_k[crt_n].is_change)
		{
			index_tmp_k[crt_n].count = 0 ;
			if (hist_rec_addr_k[crt_n].head.header_ptr == -1) return ;  //文件有问题 !added by chichi  080829
			index_tmp_k[crt_n].header = hist_rec_addr_k[crt_n].head.header_ptr;
			FilterAlarm_k[crt_n].phead =  &hist_rec_addr_k[crt_n].head;
			FilterAlarm_k[crt_n].pRec = (REC_ENTRY_UNION*)(&hist_rec_addr_k[crt_n].rec );
			alm_rec_filter_k(FilterAlarm_k[crt_n], &index_tmp_k[crt_n]);  
			FilterAlarm_k[crt_n].is_change = FALSE ;
			page_k[crt_n].RecCount = index_tmp_k[crt_n].count ;
			
		}
		
	}
	
	SetPageInfo_k( &page_k[crt_n] );  
	if (isLastPage_k[crt_n])
	{
		page_k[crt_n].CurPage = max(page_k[crt_n].PageCount -1 ,0 )   ;
		
		page_k[crt_n].PageStart =  (page_k[crt_n].CurPage) * (page_k[crt_n].PageSize)  -   page_k[crt_n].PageMod   ;  /* slide back  window PageMode step*/
		
		page_k[crt_n].PageEnd   =   min ((page_k[crt_n].PageStart + page_k[crt_n].PageSize -1) , (page_k[crt_n].RecCount-1) )   -   page_k[crt_n].PageMod   ;
	}
	else
		page_k[crt_n].CurPage = (page_k[crt_n].PageStart+ 2 )/ page_k[crt_n].PageSize    ;  
	
	
	SetPageInfo_k( &page_k[crt_n] );
	page_k[crt_n].PageEnd   =   min ((page_k[crt_n].PageStart + page_k[crt_n].PageSize -1) , (page_k[crt_n].RecCount-1) )   ;
	//add by hcl resolve page[crt_n].RecCount=12000 ,can't stop flash
	if ((page_k[crt_n].PageEnd==(page_k[crt_n].RecCount-1))&&(page_k[crt_n].RecCount==MAX_SHM_RECORD_NUM))
	{
		page_k[crt_n].PageStart=page_k[crt_n].PageEnd-page_k[crt_n].PageSize+1;
		
	}
	SetAdjInfo_k( &page_k[crt_n],adj1_k[crt_n] );   
	
	labelPage = g_object_get_data(G_OBJECT(alm_pop_shell_k[crt_n]),"labelPage");
	
	sprintf(Buff_k , "%s:%d/%d ", pConvert=_toUtf8EX(OIX_CSTR_PAGE_NUM), page_k[crt_n].CurPage+1,page_k[crt_n].PageCount);
	G_SAFE_FREE(pConvert);

	if (labelPage != NULL)
		gtk_label_set_text(GTK_LABEL(labelPage), Buff_k);
#ifdef 	DEBUG
	/*   printf ("\n pagestart[%d]:%d \t  pageEnd: %d  \n",crt_n, page_k[crt_n].PageStart ,page_k[crt_n].PageEnd );    */
#endif 
	if (isPrint)
	{
		fp = fopen(AlarmPrintFile,"w");
		if (fp ) 
		{
// #ifdef SUN_OS
// 			fprintf(fp,"%s          %s        %s        %s          \n", 
// 				_CS_(AlarmHeadTitle_k[0]),_CS_(AlarmHeadTitle_k[1]),_CS_(AlarmHeadTitle_k[2]),_CS_(AlarmHeadTitle_k[3]));
// #endif

			
			//初始化标尺 
			ruler=0;
			while(ruler<PRINT_RULER_LEN)
			{
				PrintLocationRuler[ruler]=' ';
				ruler++;
			}
			PrintLocationRuler[ruler]='\0';					
			
			for (i=0,j=0;i<=PRINTELEMENT;i++)
			{	
				strncpy(PrintLocationRuler+PrintTitleLocation[j], _CS_(AlarmHeadTitle_k[i]), strlen(_CS_(AlarmHeadTitle_k[i])));	    	
				j++;
			}
			/*printf("PrintLocationRuler:%s** \n",PrintLocationRuler);*/
			fprintf(fp,"%s \n",PrintLocationRuler);

			
			
		}   /* if(fp)end  */
	}
	if   (!page_k[crt_n].RecCount) return    ;
	
	for (i = page_k[crt_n].PageStart , j= 0 ; i <=  page_k[crt_n].PageEnd ;j++, i++)
	{
		
		
		if (alm_rec_mode_k[crt_n]	== ALM_REC_REAL1)
		{
			
			GetAlmStrings(&shm_rec_addr->rec[index_tmp_k[crt_n].index_pt[i].pt], &alarm_str , &blink , &color );
		}
		else if (alm_rec_mode_k[crt_n]	== ALM_REC_HISTRYDB)
		{   
			
			GetAlmStrings(&shm_hdb_alm_k->data_al.hdb_rec.rec[index_tmp_k[crt_n].index_pt[i].pt] ,&alarm_str , &blink , &color );
		}
		else if (alm_rec_mode_k[crt_n]	== ALM_REC_SYS_INFO)
		{
			GetAlmStrings(&shm_info_rec_addr->rec[index_tmp_k[crt_n].index_pt[i].pt], &alarm_str , &blink , &color );
		}
		else if (alm_rec_mode_k[crt_n]	== ALM_REC_HISTRY1)
		{
			
			GetAlmStrings(&hist_rec_addr_k[crt_n].rec[index_tmp_k[crt_n].index_pt[i].pt], &alarm_str , &blink , &color );
		}
		
		if (i%2) 
		{
			gdk_rgb(20,20,20,&bkColor )  ;        
		}else
			gdk_rgb( 25,30,30,&bkColor )  ; 
		
		/*  printf("11isSelect %d ,pageStart %d ,pageEnd %d ,i \n", page_k[crt_n].isSelected,page_k[crt_n].SelectStart  ,page_k[crt_n].SelectEnd,i);     */       
		
		/***************************************************************************************************/
		
		sprintf(record , "%s: ",  alarm_str.sn_str);
		strcat(record , alarm_str.repeatAlm);	
		
		if (strcmp(alarm_str.usrname,"")!=0)
			sprintf(hostname , "%s@%s", alarm_str.usrname, alarm_str.hostname);
		else
			sprintf(hostname , "%s",   alarm_str.hostname);
		
		
		if ((page_k[crt_n].isSelected)&& isInSel_k(crt_n , index_tmp_k[crt_n].index_pt[i].pt) )
		{
			gdk_rgb( 150,150,200,&bkColor )  ;
			gdk_gc_set_foreground ( alm_draw_gc_k[crt_n], &bkColor);
			gdk_draw_rectangle(drawable,alm_draw_gc_k[crt_n],1,(int)(ALM_X0*xscale[crt_n]),(int)((FAULT_ALM_Y1  + (j+1) * ALM_ROW)*yscale[crt_n]-10),(int)(ALM_CANVAS_WIDTH*xscale[crt_n]),(int)((ALM_ROW-4)*yscale[crt_n])-5); 
			
			// 			if (isPrint && fp)  
			// 			{
			// #ifdef SUN_OS
			// 				fprintf(fp,"%s %s \t  %s \t %s \t %s  \t%s \t %s  \n",record ,alarm_str.time_str,alarm_str.longname,alarm_str.cmd_result ,alarm_str.message,alarm_str.state_str , hostname);
			// #endif 
			// 				
			// #ifdef WINDOWS_OS
			// 				
			// 				//初始化标尺 
			// 				ruler=0;
			// 				while(ruler<PRINT_RULER_LEN)
			// 				{
			// 					PrintLocationRuler[ruler]=' ';
			// 					ruler++;
			// 				}
			// 				PrintLocationRuler[ruler]='\0';
			// 				strncpy(PrintLocationRuler+PrintElementLocation[0], record, strlen(record));			
			// 				strncpy(PrintLocationRuler+PrintElementLocation[1], alarm_str.time_str+2, strlen(alarm_str.time_str)-2);				
			// 				strncpy(PrintLocationRuler+PrintElementLocation[2], alarm_str.logicname, strlen(alarm_str.logicname));
			// 				if(alarm_str.message[0]==0)
			// 				{
			// 					strncpy(PrintLocationRuler+PrintElementLocation[3], alarm_str.longname, strlen(alarm_str.longname));					
			// 				}
			// 				else
			// 				{
			// 					strncpy(PrintLocationRuler+PrintElementLocation[3], alarm_str.message , strlen(alarm_str.message ));										
			// 				}
			// 				strncpy(PrintLocationRuler+PrintElementLocation[3], alarm_str.longname, strlen(alarm_str.longname));		
			// 				strncpy(PrintLocationRuler+PrintElementLocation[4], alarm_str.state_str, strlen(alarm_str.state_str));								
			// 				strncpy(PrintLocationRuler+PrintElementLocation[5], alarm_str.cmd_result, strlen(alarm_str.cmd_result));				
			// 				strncpy(PrintLocationRuler+PrintElementLocation[6], hostname, strlen(hostname));
			// 				
			// 				fprintf(fp,"%s '\n'",PrintLocationRuler);
			// 				
			// #endif
			// 			}
		}
		else
		{
			gdk_gc_set_foreground ( alm_draw_gc_k[crt_n], &bkColor);           
			gdk_draw_rectangle(drawable,alm_draw_gc_k[crt_n],1,(int)(ALM_X0*xscale[crt_n]),(int)((FAULT_ALM_Y1 + (j+1) * ALM_ROW)*yscale[crt_n]-15),(int)(ALM_CANVAS_WIDTH*xscale[crt_n]),(int)(ALM_ROW*yscale[crt_n])); 
		}
		
		if (isPrint && fp)  
		{
// #ifdef SUN_OS
// 			fprintf(fp,"%s %s \t  %s \t %s \t %s  \t%s \t %s  \n",record ,alarm_str.time_str,alarm_str.longname,alarm_str.cmd_result ,alarm_str.message,alarm_str.state_str , hostname);
// #endif 
			
			
			
			//初始化标尺 
			ruler=0;
			while(ruler<PRINT_RULER_LEN)
			{
				PrintLocationRuler[ruler]=' ';
				ruler++;
			}
			PrintLocationRuler[ruler]='\0';
			strncpy(PrintLocationRuler+PrintElementLocation[0], record, strlen(record));			
			//strncpy(PrintLocationRuler+PrintElementLocation[1], alarm_str.time_str+2, strlen(alarm_str.time_str)-2);				
			strncpy(PrintLocationRuler+PrintElementLocation[1], alarm_str.time_str, strlen(alarm_str.time_str));				
			strncpy(PrintLocationRuler+PrintElementLocation[2], alarm_str.logicname, strlen(alarm_str.logicname));
			if(alarm_str.message[0]==0)
			{
				strncpy(PrintLocationRuler+PrintElementLocation[3], alarm_str.longname, strlen(alarm_str.longname));					
			}
			else
			{
				strncpy(PrintLocationRuler+PrintElementLocation[3], alarm_str.message , strlen(alarm_str.message ));										
			}
			strncpy(PrintLocationRuler+PrintElementLocation[3], alarm_str.longname, strlen(alarm_str.longname));		
			strncpy(PrintLocationRuler+PrintElementLocation[4], alarm_str.state_str, strlen(alarm_str.state_str));								
			strncpy(PrintLocationRuler+PrintElementLocation[5], alarm_str.cmd_result, strlen(alarm_str.cmd_result));				
			strncpy(PrintLocationRuler+PrintElementLocation[6], hostname, strlen(hostname));
			
			fprintf(fp,"%s '\n'",PrintLocationRuler);
			

		}
		/***************************************************************************************************/                
		if (blink)
		{
			gdk_gc_set_foreground ( alm_draw_gc_k[crt_n], &pixels[crt_n][ color.clr[blankstate[crt_n]]]);	
		}
		else
			gdk_gc_set_foreground ( alm_draw_gc_k[crt_n], &pixels[crt_n][color.clr[0]]);
		if (blink)
			windk_draw_text( drawable,font_set[0], alm_draw_gc_k[crt_n],  ALM_X0-20 , ALM_Y1 + j * ALM_ROW , "★" ,strlen("★" ) ,crt_n);
		
		gdk_gc_set_foreground ( alm_draw_gc_k[crt_n], &pixels[crt_n][color.clr[0]]);
		
		windk_draw_text( drawable, font_set[0], alm_draw_gc_k[crt_n],  ALM_X0, ALM_Y1 + j * ALM_ROW , record ,strlen(record),crt_n);
		windk_draw_text( drawable, font_set[0], alm_draw_gc_k[crt_n],  ALM_X1, ALM_Y1 + j * ALM_ROW , alarm_str.time_str,strlen(alarm_str.time_str),crt_n);
        if (OIX_EVENT_DSP_LOGICNAME )
		{
			X_OFFSET  = 120 ; 
			windk_draw_text( drawable, font_set[0], alm_draw_gc_k[crt_n],  ALM_X2, ALM_Y1 + j * ALM_ROW , alarm_str.logicname ,strlen(alarm_str.logicname),crt_n);  
		}
		sprintf(msgRes,"%s   %s" ,alarm_str.longname   , alarm_str.state_str  ) ; 
		if (OIX_EVENT_ALIAN_MOD)  //需要命令状态对齐；
		{
			windk_draw_text( drawable, font_set[2], alm_draw_gc_k[crt_n],  ALM_X3+X_OFFSET, ALM_Y1 + j * ALM_ROW ,   alarm_str.longname  ,strlen(alarm_str.longname),crt_n);
			windk_draw_text( drawable, font_set[0], alm_draw_gc_k[crt_n],  ALM_X4+X_OFFSET, ALM_Y1 + j * ALM_ROW , alarm_str.state_str ,strlen(alarm_str.state_str),crt_n);  
			
		}
		else
			windk_draw_text( drawable, font_set[2], alm_draw_gc_k[crt_n],  ALM_X3+X_OFFSET, ALM_Y1 + j * ALM_ROW , msgRes/*alarm_str.longname*/ ,strlen(alarm_str.longname),crt_n);
		
		if (strcmp(alarm_str.cmd_src,""))
			sprintf(cmdresult,"%s:%s",alarm_str.cmd_src ,alarm_str.cmd_result) ;
		else
			sprintf(cmdresult,"%s",alarm_str.cmd_result) ;
		
		windk_draw_text( drawable, font_set[0], alm_draw_gc_k[crt_n],   ALM_X3 + 550+X_OFFSET, ALM_Y1 + j * ALM_ROW ,  cmdresult ,strlen(cmdresult),crt_n);
		
		windk_draw_text( drawable, font_set[2], alm_draw_gc_k[crt_n],  ALM_X3+X_OFFSET, ALM_Y1 + j * ALM_ROW ,  alarm_str.message ,strlen(alarm_str.message),crt_n);
		
		if (alm_rec_mode_k[crt_n]	!= ALM_REC_HISTRYDB)
			windk_draw_text( drawable, font_set[0], alm_draw_gc_k[crt_n],  ALM_X5, ALM_Y1 + j * ALM_ROW , hostname ,strlen(hostname),crt_n);
		
		
	}
	
	
	if (isPrint && fp) fclose(fp);
	
	if   (blankstate[crt_n] >0)
		blankstate[crt_n] = 0 ;
	else
		blankstate[crt_n] = 1 ;
#ifdef 	DEBUG
	/*  g_message("FilterAlarm_k[%d].is_change:%d ,index_tmp_k[%d].count :%d \n",crt_n ,  FilterAlarm_k[crt_n].is_change , crt_n  ,index_tmp_k[crt_n].count); */ 
#endif	
	
}

/****************  Timer  call  !!!      ************/
gint  AlmDspProc_k( crt_n )
int crt_n ;
{
	GtkWidget *scroll ; 
	GdkColor clr ;
	/*	g_mutex_lock (alm_mutex_k[crt_n]);    */
	
	gdk_rgb(20,20,20,&clr);
	
	
	/* printf("OIX::+++++++++++++++++++++++AlmDspProc_k( crt_n )++++++++++++!!!!\n"); */ 
	/*  printf("pagcurge = %d, pagecount =%d \n",page_k[crt_n].CurPage , page_k[crt_n].PageCount);  */
	gdk_gc_set_foreground(alm_draw_gc_k[crt_n], &clr);
	gdk_draw_rectangle (AlmOffDrawable_k[crt_n],alm_draw_gc_k[crt_n],TRUE, 0,0,LOCAL_WIDTH, 1600); 
	AlmDspProcE_k( AlmOffDrawable_k[crt_n] ,crt_n,FALSE);
	gdk_window_copy_area (alm_canvas_k[crt_n] ->window,alm_draw_gc_k[crt_n],0,0,AlmOffDrawable_k[crt_n],0,0,LOCAL_WIDTH, 1600);
	scroll = g_object_get_data(G_OBJECT(alm_pop_shell_k[crt_n]),"scrollbar" );
	gtk_widget_queue_draw(scroll);
	return TRUE;
	/*	 g_mutex_unlock (alm_mutex_k[crt_n]);  */
	
	
}

gint AutoFilterProc_k(int crt_n)
{
	int i,j,k;
	char    unit_state_str[POINT_NAME_SIZE+12];
	for(i=1;i<=keySignalStnDef[FilterAlarm_k[crt_n].stn_id].dev_num;i++)
	{
		for (j=1;j<=(int)keySignalStnDef[FilterAlarm_k[crt_n].stn_id].dev[i].group_num;j++)
		{	
			for (k=0;k<MAX_UNIT_STATUS_NUM;k++)
			{
				sprintf(unit_state_str,"%s.%s.%s%d.",keySignalStnDef[FilterAlarm_k[crt_n].stn_id].Name,keySignalStnDef[FilterAlarm_k[crt_n].stn_id].dev[i].Name,"AUTO.FILTER.STATE",k);
				if (InterLock_Judge(unit_state_str) == 1)
				{
					if (keySignalStnDef[FilterAlarm_k[crt_n].stn_id].dev[i].group[j].Status & (0X1<<k))
					{
						keySignalStnDef[FilterAlarm_k[crt_n].stn_id].dev[i].group[j].isFilter[crt_n] = FALSE;
						FilterAlarm_k[crt_n].group_mask[i-1] = FilterAlarm_k[crt_n].group_mask[i-1] | (1<<(j-1));
					}
					else
					{
						keySignalStnDef[FilterAlarm_k[crt_n].stn_id].dev[i].group[j].isFilter[crt_n] = TRUE;
						FilterAlarm_k[crt_n].group_mask[i-1] = FilterAlarm_k[crt_n].group_mask[i-1] & (~(1<<(j-1)));
					}
					FilterAlarm_k[crt_n].is_change = TRUE;
					AlmDspProc_k(crt_n);
				}
			}
		}
	}
	return TRUE;
}

USHORT SecCall_k[MAX_CRT_NUM] ={0,0,0,} ;
gint  TimeCall_k( crt_n )
{
	/* printf ("OIX:  dsp @@@@ !!! ctr_n= %d  \n " ,crt_n);	*/
	SecCall_k[crt_n] ++  ;
	if  (alm_rec_mode_k[crt_n]	== ALM_REC_REAL1   )  
		if (index_tmp_k[crt_n].header != shm_rec_addr->head.header_ptr  || !(SecCall_k[crt_n]%SECNUM)  )
		{
			AlmDspProc_k( crt_n ) ;
			/* printf ("OIX:--time  ------thread calll FetchDataFromSHM_k  !!! ctr_n= %d  \n " ,crt_n);	*/
		}
		
		if  (alm_rec_mode_k[crt_n]	== ALM_REC_SYS_INFO   )  
			if (index_tmp_k[crt_n].header != shm_info_rec_addr->head.header_ptr  || !(SecCall_k[crt_n]%SECNUM)  )
			{
				AlmDspProc_k( crt_n ) ;
				/* printf ("OIX:--time  ------thread calll FetchDataFromSHM_k  !!! ctr_n= %d  \n " ,crt_n);	*/
			}
			
			if  ((alm_rec_mode_k[crt_n]	== ALM_REC_HISTRYDB ) && (!(SecCall_k[crt_n]%(SECNUM *2))) )
			{
				AlmDspProc_k( crt_n ) ;
				printf ("OIX:--time  ------thread calll FetchDataFromHDB !!! ctr_n= %d  \n " ,crt_n);	 
			}
			
			return TRUE ; 
}
void	DrawAlmTabHead_k(GdkDrawable *drawable ,GdkGC *gc,int crt_n  )
{
	int  X_OFFSET  = 0 ; 
	
	gdk_gc_set_foreground ( gc, &pixels[0][L_BLUE1]);
	windk_draw_text( drawable,  font_set[DBTAB_FONT], gc,ALM_X0, ALM_Y0, _CS_(OIX_STR_REC_NUMBER), -1 ,crt_n );
	windk_draw_text( drawable,  font_set[DBTAB_FONT], gc, ALM_X1, ALM_Y0, _CS_(OIX_STR_TIME), -1 ,crt_n);
	if (OIX_EVENT_DSP_LOGICNAME )
    {
		X_OFFSET  = 180 ; 
		windk_draw_text( drawable,  font_set[DBTAB_FONT], gc, ALM_X2, ALM_Y0, _CS_(OIX_STR_REC_LOGNAME), -1,crt_n); 
	}
	windk_draw_text( drawable, font_set[DBTAB_FONT], gc, ALM_X3+30+X_OFFSET, ALM_Y0, _CS_(OIX_STR_MSG_NAME), -1 ,crt_n); 
	windk_draw_text( drawable, font_set[DBTAB_FONT], gc, ALM_X4 +X_OFFSET, ALM_Y0, _CS_(OIX_STR_RT_STATUS), -1 ,crt_n );
	windk_draw_text( drawable, font_set[DBTAB_FONT], gc, ALM_X5, ALM_Y0, _CS_(OIX_CSTR_USER_AT_HOST), -1 ,crt_n );
	
	
}

void  alarm_expose_k(GtkWidget *w ,  GdkEventExpose *event, gpointer data)  
{ 
	
	int crt_n = (gint ) data;
	
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER ) return ;
	
	AlmDspProc_k( crt_n);
}



void  alarm_motion_k(GtkWidget *w ,GdkEventMotion *event,   gpointer data)
{
	int crt_n =(gint) data ; 
	
	
	if(   !(event->state & GDK_BUTTON1_MASK))
		return  ;
	page_k[crt_n].mycur.x2 = (int)(event->x) ; page_k[crt_n].mycur.y2 = (int)(event->y) ;
	/* printf ("motion :::x = %d, y=%d \n "  ,(int)event->x ,(int)event->y)  ;    */   
	
}

void  alarm_button_release_k( GtkWidget *w ,GdkEvent *event  , gpointer data)
{   int  i,j,S,E ;
int  crt_n = (gint)  data ;

gint rowNum ,height ; 
if (event->type == GDK_BUTTON_RELEASE) {   
	GdkEventButton *bevent = (GdkEventButton *) event; 
	if (bevent->button == 1)   
	{	        
		height = windk_text_height(font_set[DBTAB_FONT] ,"娑", strlen("娑"), FONT_SIZE_ALARM);
		
		printf("height=========%d\n",height);
		
		rowNum = ( min(page_k[crt_n].mycur.y1,page_k[crt_n].mycur.y2) - (int)((ALM_Y1 -  height)*yscale[crt_n]+10)) / (int)(ALM_ROW*yscale[crt_n] )  ;
		rowNum = max(rowNum ,0);
		page_k[crt_n].isSelected  =  1 ; 
		S = rowNum + page_k[crt_n].PageStart ;
		page_k[crt_n].SelectStart  = index_tmp_k[crt_n].index_pt[S].pt ; 
		
		rowNum = (  max(page_k[crt_n].mycur.y1,page_k[crt_n].mycur.y2) - (int)((ALM_Y1 -  height)*yscale[crt_n]+10)) / (int)(ALM_ROW*yscale[crt_n] )  ; /* ALM_Y1 +  height) / ALM_ROW   ;*/
		E = min((rowNum + page_k[crt_n].PageStart),page_k[crt_n].PageEnd) ;
		page_k[crt_n].SelectEnd  = index_tmp_k[crt_n].index_pt[E].pt ; 
		
		sel_pt_k[crt_n].nextpos = 0  ;
		for (i = S ,j =0 ; i<=E ; j ++ , i++ )
		{
			sel_pt_k[crt_n].sel[j] = index_tmp_k[crt_n].index_pt[i].pt ;
			sel_pt_k[crt_n].nextpos++ ; 
			printf ("_____*******sel = %d ,nextpos = %d \n" ,sel_pt_k[crt_n].sel[j] , sel_pt_k[crt_n].nextpos) ;  
		}
		printf ("+++++++++++rownum = %d , selectstart = %d , selectend = %d ,S = %d ,E = %d \n" ,
			rowNum,page_k[crt_n].SelectStart ,page_k[crt_n].SelectEnd,S,E ) ;
		
		
		AlmDspProc_k( crt_n);        
		
	} 
	
}
}

extern DB_TAB_WINDOW_INFO db_tab_window_info[];


gint alarm_button_press_ex_k( GtkWidget *w ,GdkPoint *pt  , gpointer data,GdkEventButton *bevent)
{
	gint  crt_n = (gint)  data ;
	POINTER  dbpt ;
	GtkWidget  *popupmenu;
	extern POINTER    db_selected_ptr[];
	gint rowNum ,height ,count; 
	
	height = windk_text_height(font_set[DBTAB_FONT] ,"娑", strlen("娑"),FONT_SIZE_ALARM/*fontdesc[0]*/);		 
	rowNum = ( pt->y - (int)((ALM_Y1 -  height)*yscale[crt_n])) / (int)(ALM_ROW*yscale[crt_n])  + 1 ;
	count  = rowNum + page_k[crt_n].PageStart ; /* page_k[crt_n].CurPage   * page_k[crt_n].PageSize ;   */
	
	if ( count   > index_tmp_k[crt_n].count ||count <=  page_k[crt_n].PageStart)
		return 0 ;
	if (alm_rec_mode_k[crt_n]	== ALM_REC_REAL1)
	{
		if(-1==GetPointFromRec(&shm_rec_addr->rec[index_tmp_k[crt_n].index_pt[count -1 ].pt],&dbpt))
			return 0 ; 
	}
	else if (alm_rec_mode_k[crt_n]  == ALM_REC_HISTRYDB)
	{   
		
		if(-1==GetPointFromRec(&shm_hdb_alm_k->data_al.hdb_rec.rec[index_tmp_k[crt_n].index_pt[count -1 ].pt],&dbpt))
			return 0 ; 
		
	}
	else if (alm_rec_mode_k[crt_n]  == ALM_REC_HISTRY1) /* HISTORY FILE!!! */
	{
		if(-1==GetPointFromRec(&hist_rec_addr_k[crt_n].rec[index_tmp_k[crt_n].index_pt[count -1 ].pt],&dbpt))
			return 0 ;   
	}
	else if (alm_rec_mode_k[crt_n]  == ALM_REC_SYS_INFO)
	{
		if(-1==GetPointFromRec(&shm_info_rec_addr->rec[index_tmp_k[crt_n].index_pt[count -1 ].pt],&dbpt))
			return 0 ;
	}
	else 
		return  0 ;
	/* page_k[crt_n].SelectStart = page_k[crt_n].SelectEnd = index_tmp_k[crt_n].index_pt[count-1].pt ;  */
	sel_pt_k[crt_n].sel[0] = index_tmp_k[crt_n].index_pt[count-1].pt ;
	sel_pt_k[crt_n].nextpos =1  ; 
	page_k[crt_n].isSelected = 1 ; 
	db_tab_window_info[crt_n].select_pt  =  dbpt ;
	//	PopupDetailInfoBox(alm_pop_shell_k[crt_n ], crt_n, db_tab_window_info[crt_n].select_pt);
	popupmenu  = g_object_get_data(G_OBJECT(main_w[crt_n]),"PopMenuEvt"); //PopMenuEvt
	printf("popupmenu=%x\n", popupmenu );
	if (popupmenu != NULL )
		gtk_menu_popup (GTK_MENU (popupmenu), NULL, NULL, NULL, NULL,
		bevent->button, bevent->time);
	
	AlmDspProc_k( crt_n);     
#ifdef  DEBUG                
	printf ("You select row :%d && count : %d \n" , rowNum, shm_rec_addr->rec[index_tmp_k[crt_n].index_pt[count -1 ].pt].ana_rec.rec_sn) ;
	printf ("\nPt.stn_id:%d,Pt.dev_id:%d,Pt.data_type:%d,Pt.pt_id:%d\n",
		dbpt.stn_id,dbpt.dev_id,dbpt.data_type,dbpt.pt_id);
	
#endif 
	
	return TRUE;
}

gint alarm_button_press_k( GtkWidget *w ,GdkEvent *event  , gpointer data)
{
	GdkPoint   pt;
	
	gint  crt_n = (gint)  data ;
	if (event->type == GDK_BUTTON_PRESS) {   
		GdkEventButton *bevent = (GdkEventButton *) event; 
		if (bevent->button == 3)  
		{ 
			pt.x =(int) (bevent->x) ;
			pt.y =(int) (bevent->y) ;
			
			alarm_button_press_ex_k(  w , &pt  ,  GINT_TO_POINTER (crt_n),bevent)	 ;    	
			
		}else if (bevent->button == 1) 
		{
			page_k[crt_n].mycur.x1 = page_k[crt_n].mycur.x2 = (int)(bevent->x);
			page_k[crt_n].mycur.y1 = page_k[crt_n].mycur.y2 =  (int)(bevent->y);            
		}
	}
	return TRUE;
}

gint PrevPage_k( GtkWidget *w , gpointer data)
{
	
	
	gint crt_n = (gint )data;
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER ) return FALSE;
	
#ifdef DEBUG
	g_message("^^^^^^PrevPage_k  crt_n is %d", crt_n );
#endif
	
	page_k[crt_n].CurPage -- ;
	if (page_k[crt_n].CurPage < 0  )  
	{
		gdk_beep(    ); 
		page_k[crt_n].CurPage  = 0 ;
		return FALSE; 
	}
	SetPageInfo_k (&page_k[crt_n]);
	
	page_k[crt_n].PageStart =  (page_k[crt_n].CurPage) * (page_k[crt_n].PageSize) ;
	page_k[crt_n].PageEnd   =   min ((page_k[crt_n].PageStart + page_k[crt_n].PageSize -1) , (page_k[crt_n].RecCount-1) )   ;
	SetAdjInfo_k(&page_k[crt_n],adj1_k[crt_n]);
	isLastPage_k[crt_n] = FALSE;
	AlmDspProc_k( crt_n );
	
// 	if (page_k[crt_n].CurPage != page_k[crt_n].PageCount-1 &&  !isLastPage_k[crt_n])
// 	{
// 		if (alm_timeout_id_k[crt_n])
// 			gtk_timeout_remove ( alm_timeout_id_k[crt_n]  ); 
// 		alm_timeout_id_k[crt_n] =0 ;
// 	}else
// 	{
// 		if (!alm_timeout_id_k[crt_n])
// 			alm_timeout_id_k[crt_n]=  gtk_timeout_add ( INTERVAL,(GtkFunction)TimeCall_k, GINT_TO_POINTER (crt_n) ); 
// 	}
	return TRUE;
}

gint NextPage_k( GtkWidget *w ,    gpointer data)
{
	gint crt_n = (gint )data ;
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER ) return FALSE ;
	
#ifdef DEBUG
	g_message("&&&&&NextPage_k  crt_n is %d", crt_n );
#endif
	page_k[crt_n].CurPage ++ ;
	if ( page_k[crt_n].CurPage > max( page_k[crt_n].PageCount -1   ,0)) 
	{
		gdk_beep(    ); 
		page_k[crt_n].CurPage  = page_k[crt_n].PageCount  -1 ;
		return FALSE;
	} 
	SetPageInfo_k (&page_k[crt_n] );
	page_k[crt_n].PageStart =  (page_k[crt_n].CurPage) * (page_k[crt_n].PageSize) ;
	page_k[crt_n].PageEnd   =   min ((page_k[crt_n].PageStart + page_k[crt_n].PageSize -1) , (page_k[crt_n].RecCount-1) )   ;
	SetAdjInfo_k(&page_k[crt_n],adj1_k[crt_n]);
	isLastPage_k[crt_n] = FALSE;
	
	AlmDspProc_k( crt_n );
	
// 	if (page_k[crt_n].CurPage != page_k[crt_n].PageCount-1 && !isLastPage_k[crt_n] )
// 	{    
// 		if (alm_timeout_id_k[crt_n])
// 			gtk_timeout_remove ( alm_timeout_id_k[crt_n]  ); 
// 		alm_timeout_id_k[crt_n] =0 ;
// 	}else  if (page_k[crt_n].CurPage == page_k[crt_n].PageCount-1  || page_k[crt_n].PageCount ==0 )
// 	{
// 		if (!alm_timeout_id_k[crt_n])
// 			alm_timeout_id_k[crt_n]=  gtk_timeout_add ( INTERVAL,(GtkFunction)TimeCall_k, GINT_TO_POINTER (crt_n) ); 
// 	}
	return TRUE;
}



void alarm_draw_area_realize_k(GtkWidget *widget,gpointer data)
{
	int crt_n = (gint)data ;
	
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return ; 
	alm_canvas_k[crt_n] = widget ;
	alm_draw_gc_k[crt_n] = gdk_gc_new(widget->window);
	AlmOffDrawable_k[crt_n] = gdk_pixmap_new(widget->window,LOCAL_WIDTH, 1600,-1);
	gdk_window_set_background(widget->window,&pixels[crt_n][BLACK]);  
	
}






gint AlarmFormQuit_k(GtkWidget *widget,   gpointer data)
{
	int crt_n = (gint) data ;
	/*  GetCrtNumberOfWgt(widget,&crt_n);  */
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE; 
	if(alm_timeout_id_k[crt_n]!=0)
	{
		gtk_timeout_remove(alm_timeout_id_k[crt_n]);
		alm_timeout_id_k[crt_n]= 0;
	}
	if (autofilter_timeout_id_k[crt_n]!=0)
	{
		gtk_timeout_remove(autofilter_timeout_id_k[crt_n]);
		autofilter_timeout_id_k[crt_n] = 0;
	}
	g_object_unref (AlmOffDrawable_k[crt_n]);     
	isLastPage_k[crt_n]=0;
	alm_exit_flag_k[crt_n] = TRUE;   
	
	gtk_widget_destroy(GTK_WIDGET(alm_pop_shell_k[crt_n])); 
	alm_pop_shell_k[crt_n] = (GtkWidget *)NULL ;
	filterDlg_k[crt_n] = (GtkWidget *)NULL ;
	HistDBDlg_k[crt_n] = NULL;
	/*	 free_alm_mutex_k(  crt_n ) ;  */
	return TRUE;
	
}



gint CreateDbDevB_k(GtkWidget *widget,   gpointer data)
{
	
	int crt_n   =   0;
	int i;
	
	//GetCrtNumberOfWgt (widget, &crt_n );
	crt_n=getCrtNumFromPointer((int)data);
	data=(gpointer)getDataFromPointer((int)data);
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE;
	
	FilterAlarm_k[crt_n].stn_id = (gint)data ;
	FilterAlarm_k[crt_n].is_change = TRUE;
	FilterAlarm_k[crt_n].change_flag = TRUE;
	for (i=0;i<MAX_DEV_MASK_NUM;i++)
	{
		FilterAlarm_k[crt_n].dev_mask[i] = 0xffffffff;
	}
	for (i=0;i<MAX_GROUP_MASK_NUM;i++)
	{
		FilterAlarm_k[crt_n].group_mask[i] = 0xffffffff;
	}
	AlmDspProc_k( crt_n );
	
#ifdef DEBUG	 
	printf ("cFilterAlarm[%d].stn_id = %d \n",   crt_n ,FilterAlarm_k[crt_n].stn_id );
#endif  
	return TRUE;
}


int  canUpdate_k[MAX_CRT_NUMBER] = {1,1,1};  
gint SetDataTypeMask_k(GtkWidget       *widget,   gpointer     data)
{   
	int i  ;
	int crt_n , j;
	int  bull ;

	crt_n=getCrtNumFromPointer((int)data);
	i=getDataFromPointer((int)data);

	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
	return FALSE;

	if( i == MAX_DATATYPE_NUM   +	1     )   /*  all not  */
	{
		for (j = 0 ; j< MAX_DATATYPE_NUM ; j++ )
		{
			if ( DataTypeItem_k[crt_n][j]!= (GtkWidget*)NULL )gtk_check_menu_item_set_active  ( GTK_CHECK_MENU_ITEM(DataTypeItem_k[crt_n][j]),FALSE);
			canUpdate_k[crt_n] = (j<MAX_DATATYPE_NUM-1?0:1) ;
		}
	}
	else if( i == MAX_DATATYPE_NUM    )   /*   all  */
	{
		for (j = 0 ; j< MAX_DATATYPE_NUM ; j++ )
		{
			if ( DataTypeItem_k[crt_n][j]!= (GtkWidget*)NULL )gtk_check_menu_item_set_active  (  GTK_CHECK_MENU_ITEM(DataTypeItem_k[crt_n][j]),TRUE);
			canUpdate_k[crt_n] = (j<MAX_DATATYPE_NUM-1?0:1) ;
		}
	}
	else
	{
		bull = gtk_check_menu_item_get_active  ( GTK_CHECK_MENU_ITEM(widget));  
		if (gtk_check_menu_item_get_active  ( GTK_CHECK_MENU_ITEM(widget)))
		{
			FilterAlarm_k[crt_n].datatype_mask = (FilterAlarm_k[crt_n].datatype_mask)  | (1<<(i-1)) ;  /*  Set bit N */
			FilterAlarm_k[crt_n].is_change = TRUE;	
			FilterAlarm_k[crt_n].change_flag = TRUE;
		}
		else
		{
			FilterAlarm_k[crt_n].datatype_mask = (FilterAlarm_k[crt_n].datatype_mask)  & (~(1<<(i-1))) ;   /*  Clear bit N */
			FilterAlarm_k[crt_n].is_change = TRUE	;
			FilterAlarm_k[crt_n].change_flag = TRUE;
		}
		if (canUpdate_k[crt_n])
			AlmDspProc_k( crt_n );   
		canUpdate_k[crt_n] = 1  ;

		#ifdef  DEBUG 
		g_message("FilterAlarm_k[crt_n].datatype_mask is %x  " ,FilterAlarm_k[crt_n].datatype_mask );
		g_message("toggle[%d] is %d  " ,i, bull );
		#endif 

		
	}
	return  TRUE;	
}

int dev_button_press_k(GtkWidget       *widget,  gpointer     data)
{   
	
	
	gint crt_n = (gint) data ;
	
	if (filterDlg_k[crt_n] == (GtkWidget *)NULL)
	{
		filterDlg_k[crt_n] = CreateDevFilterDlg_k( alm_pop_shell_k[crt_n] ,crt_n);
		gtk_widget_show(filterDlg_k[crt_n]);
	}
	else
		gtk_window_present (GTK_WINDOW(filterDlg_k[crt_n]));
	
	
	if (FilterAlarm_k[crt_n].stn_id == 0) return FALSE;
	
	
	return TRUE;
	
	
}

int attrib_button_press_k(GtkWidget       *widget,  gpointer     data)
{   
	
	
	gint crt_n = (gint) data ;
	
	if (AttribFilterDlg_k[crt_n] == (GtkWidget *)NULL)
	{
		AttribFilterDlg_k[crt_n] = CreateAttribFilterDlg_k( alm_pop_shell_k[crt_n] ,crt_n);
		gtk_widget_show(AttribFilterDlg_k[crt_n]);
	}
	else
		gtk_window_present (GTK_WINDOW(AttribFilterDlg_k[crt_n]));
	
	return TRUE;
	
	
}

void  cb_scroll_page_k(GtkAdjustment *adj , int crt_n)
{
	
	
	int maxNum ; 
	
	maxNum  =  index_tmp_k[crt_n].count;
	/*   printf ("adj.value = %d,adj.upper = %8.3f,adj.page_increment = %8.3f,crt = %d\n",(int)adj->value,adj->upper,adj->page_increment,crt_n); */  
	page_k[crt_n].PageStart = min((int)adj->value,maxNum-page_k[crt_n].PageSize)  ; 
	if (page_k[crt_n].PageStart <0 ) page_k[crt_n].PageStart  = 0 ;
	page_k[crt_n].PageEnd   =  min ((page_k[crt_n].PageStart + page_k[crt_n].PageSize -1) , (page_k[crt_n].RecCount-1) )   ;
	
// 	if (page_k[crt_n].CurPage != page_k[crt_n].PageCount-1 &&  !isLastPage_k[crt_n])
// 	{
// 		if (alm_timeout_id_k[crt_n])
// 			gtk_timeout_remove ( alm_timeout_id_k[crt_n]  ); 
// 		alm_timeout_id_k[crt_n] =0 ;
// 	}else if (page_k[crt_n].CurPage == page_k[crt_n].PageCount-1  || page_k[crt_n].PageCount ==0 )
// 	{
// 		if (!alm_timeout_id_k[crt_n])
// 			alm_timeout_id_k[crt_n]=  gtk_timeout_add ( INTERVAL,(GtkFunction)TimeCall_k, GINT_TO_POINTER (crt_n) );   
// 	}
	AlmDspProc_k( crt_n);
}


void 	ProgKeySignalTab (GtkWidget *parent,	int crt_n  ,int TabMode  )
{
	if (alm_pop_shell_k[crt_n]==(GtkWidget*)NULL)
	{
		alm_pop_shell_k[crt_n] = CreateProgAlarmTab_k ( parent, crt_n   ) ;
		//gtk_window_set_position(GTK_WINDOW(alm_pop_shell_k[crt_n]),GTK_WIN_POS_CENTER_ON_PARENT);
		//gtk_widget_show(alm_pop_shell_k[crt_n]);
		/*alm_rec_mode_k[crt_n]	= ALM_REC_REAL1 ; rm by chi */
#ifdef   HISTORY_DB_ALM     
		printf(" start to  get shared memory..... \n");
		
		if (get_hdb_shm()==FALSE)
		{
			printf("OIX::can't get shared memory\n");
			shm_hdb_alm_k = NULL; 
			return ;
		} 
		
		shm_hdb_alm_k = shm_hdb_addr;   
#endif
		
		
		isLastPage_k[crt_n] = FALSE;
		if (TabMode == 1 )  /* TabMode ==1 系统综合信息*/	
			InitPageInfo_k(&page_k[crt_n] , crt_n,shm_rec_addr->head.count) ;  /* initial  set  realtime record   */
		else 
			InitPageInfo_k(&page_k[crt_n] , crt_n,shm_rec_addr->head.count) ;  /* initial  set  realtime record   */ 
		
		SetAdjInfo_k( &page_k[crt_n],adj1_k[crt_n]) ;  
		/*  InitAlarmFilter( &FilterAlarm_k[crt_n],crt_n);  */
		FilterAlarm_k[crt_n].is_change = TRUE; 
		/* CreateDspThread(crt_n);  */
		
		
		if (alm_timeout_id_k[crt_n] != 0 )
		{
			gtk_timeout_remove(alm_timeout_id_k[crt_n]);
			alm_timeout_id_k[crt_n]= 0;
		}
		alm_timeout_id_k[crt_n]=gtk_timeout_add ( INTERVAL,(GtkFunction)TimeCall_k, GINT_TO_POINTER (crt_n) ); 
		
	}else
	{
		gtk_window_present(GTK_WINDOW(alm_pop_shell_k[crt_n]));
	}
}


/***************************************************************************

  create a main alarm windows ; 
  entrance of process of alarm ;
  
****************************************************************************/
static  GtkWidget*
CreateProgAlarmTab_k (GtkWidget *parent,	int crt_n   )
{
	GtkWidget *scrolled_window,*draw_area ,*window;
	GtkWidget *vbox, *hbox;
	GtkWidget  *toolbar,*scrollbar ;
//	GtkWidget  *toolbar1 ;
	GtkWidget *hbox4;
	GdkScreen   *scr ; 	
	gchar *pConvert=0;
	g_message("crt_n is %d \n "  , crt_n );
	
	/*  gtk_set_locale();
	gtk_rc_add_default_file(GTK_RC_FILE);*/
	
	window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_transient_for(GTK_WINDOW(window),GTK_WINDOW(main_w[crt_n]));
	
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	
	scr  = gtk_widget_get_screen (GTK_WIDGET (parent))  ; 
	//	gtk_window_set_screen (GTK_WINDOW (window),
	//			   second_screen[crt_n]);
	//        gtk_widget_show  (window)  ;
	
	
	gtk_window_set_default_size(GTK_WINDOW(window),(int)(ALM_CANVAS_WIDTH*xscale[crt_n]), (int)(ALM_CANVAS_HEIGHT*yscale[crt_n]));
	
	
	
#ifdef  SUN_OS
	gtk_window_move(GTK_WINDOW(window) , 0, 80);
#endif
#ifdef WINDOWS_OS
	gtk_window_move(GTK_WINDOW(window) , LOCAL_WIDTH*crt_n,80);
#endif
	
	
	gtk_window_set_title(GTK_WINDOW(window), pConvert=_toUtf8EX(OIX_CSTR_KEYSIGNAL_TAB));
	G_SAFE_FREE(pConvert);
	
	vbox=gtk_vbox_new(FALSE, 5);
	gtk_container_set_border_width(GTK_CONTAINER(vbox), 0);
	gtk_container_add(GTK_CONTAINER(window), vbox);
	gtk_widget_show(vbox);
	
	toolbar =  CreateAlmToolbar_k(window,crt_n);/*updata by hcl*/
	gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, TRUE, 0);
	gtk_widget_show(toolbar);
	gtk_signal_connect(GTK_OBJECT(window),
		"destroy",
		GTK_SIGNAL_FUNC(AlarmFormQuit_k),
		GINT_TO_POINTER (crt_n));
	
	
	scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
		GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
	
	draw_area = gtk_drawing_area_new();
	hbox4 =  gtk_hbox_new(FALSE, 0);
	gtk_widget_show(hbox4); 
	gtk_box_pack_start(GTK_BOX(vbox), hbox4, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(hbox4), draw_area, TRUE, TRUE, 0);
	adj1_k[crt_n] =(GtkAdjustment*)(gtk_adjustment_new(0.0,0.0,101.0,0.1,1.0,1.0));
	scrollbar = gtk_vscrollbar_new(GTK_ADJUSTMENT(adj1_k[crt_n]));
	gtk_widget_show(scrollbar)   ;
	g_object_set_data(G_OBJECT(window),"scrollbar",scrollbar);
	gtk_box_pack_start(GTK_BOX(hbox4), scrollbar, FALSE, FALSE, 0);
	/* gtk_widget_set_usize(draw_area, 1024, 400); */
	//gtk_widget_set_usize(draw_area, (int)(ALM_CANVAS_WIDTH*xscale[crt_n]), (int)(ALM_CANVAS_HEIGHT*yscale[crt_n]));
	g_signal_connect(G_OBJECT(adj1_k[crt_n]),"value_changed",G_CALLBACK(cb_scroll_page_k),GINT_TO_POINTER (crt_n));
	
	
	
	
	gtk_widget_set_events(draw_area, GDK_EXPOSURE_MASK 
		| GDK_SCROLL_MASK            
		| GDK_LEAVE_NOTIFY_MASK
		| GDK_BUTTON_PRESS_MASK
		| GDK_BUTTON_RELEASE_MASK
		| GDK_POINTER_MOTION_MASK );
	
	
	gtk_signal_connect (GTK_OBJECT (draw_area),"expose_event",GTK_SIGNAL_FUNC(alarm_expose_k),GINT_TO_POINTER (crt_n)) ;
	gtk_signal_connect (GTK_OBJECT (draw_area),"scroll-event",GTK_SIGNAL_FUNC(alarm_scroll_event_k),GINT_TO_POINTER(crt_n)) ;    
	gtk_signal_connect (GTK_OBJECT (draw_area),"motion_notify_event",GTK_SIGNAL_FUNC(alarm_motion_k),GINT_TO_POINTER(crt_n)) ;
	gtk_signal_connect (GTK_OBJECT (draw_area), "realize", GTK_SIGNAL_FUNC (alarm_draw_area_realize_k), GINT_TO_POINTER(crt_n));
	gtk_signal_connect (GTK_OBJECT (draw_area), "button_press_event", GTK_SIGNAL_FUNC (alarm_button_press_k),GINT_TO_POINTER(crt_n) );    
	gtk_signal_connect (GTK_OBJECT (draw_area), "button_release_event", GTK_SIGNAL_FUNC (alarm_button_release_k),GINT_TO_POINTER(crt_n) );    
	gtk_signal_connect (GTK_OBJECT (draw_area),"size_allocate",GTK_SIGNAL_FUNC(alarm_canvas_allocate_k),GINT_TO_POINTER(crt_n)) ;
	/*   gtk_scrolled_window_add_with_viewport(GTK_CONTAINER(scrolled_window), draw_area);*/
	gtk_widget_realize(GTK_WIDGET( draw_area)) ;
	gtk_widget_show(draw_area);
	
	
	
	
	
	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, TRUE, 0);
	gtk_widget_show(hbox);
	
	
	gtk_widget_show(window);
	
	
	/*	init_alm_mutex_k(  crt_n ) ;  */
	return  window ;
	
	
}
void  SetHistAlmCName_k(GtkWidget * widget, gpointer data)
{
	int crt_n ;
	int i = (gint) data ;
	GetCrtNumberOfWgt (widget, &crt_n );
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return ;
	g_object_set_data(G_OBJECT(widget),"iNameMode", GINT_TO_POINTER (i));  
	iHisNameMode_k[crt_n]  = i+1 ; 
	g_message ("iNameMode = %d \n " , i )  ;
	
}

void SetHistAlmBtn_k (GtkWidget * widget, gpointer data)
{    
	int i  ;
	int crt_n ;
	
	char dirName[100];
	gchar *pConvert=0;
	
	//GetCrtNumberOfWgt (widget, &crt_n );
	//hcl
	crt_n=getCrtNumFromPointer((int)data);
	i=getDataFromPointer((int)data);
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return ;
	
	
	switch(i )
	{
	case 0 :    /*ALM_REC_REAL1*/
		alm_rec_mode_k[crt_n]	= ALM_REC_REAL1;
		/* InitAlarmFilter(&FilterAlarm_k[crt_n],crt_n);  */
		FilterAlarm_k[crt_n].phead =  &shm_rec_addr->head;
		FilterAlarm_k[crt_n].pRec   = (REC_ENTRY_UNION* )&shm_rec_addr->rec;	        
		InitPageInfo_k(&page_k[crt_n] , crt_n,shm_rec_addr->head.count) ;  /* initial  set  realtime record   */
		SetAdjInfo_k( &page_k[crt_n],adj1_k[crt_n]) ;  
		gtk_window_set_title(GTK_WINDOW(alm_pop_shell_k[crt_n]), pConvert=_toUtf8EX(OIX_CSTR_ALARM_TAB));
		G_SAFE_FREE(pConvert);
		if (!alm_timeout_id_k[crt_n])
			alm_timeout_id_k[crt_n]=  gtk_timeout_add ( INTERVAL,(GtkFunction)TimeCall_k, GINT_TO_POINTER (crt_n) ); 
		break;   
	case 1:    /*ALM_HIST_FILE*/
		alm_rec_mode_k[crt_n]	= ALM_REC_HISTRY1;
		file_mode[crt_n]       = ALM_HIST_FILE; 
		page_k[crt_n].RecCount = 0 ;
		sprintf(dirName , "%s*.REC" , EVENT_LOG_PATH );
		PopFileSelectionDlg(main_w[crt_n],crt_n, pConvert=_toUtf8EX(OIX_STR_ALARM_FILE_SELECT_BOX),dirName);  
		G_SAFE_FREE(pConvert);
		gdk_beep();
		break;
		
	case 2 :    /*综合信息*/
		alm_rec_mode_k[crt_n]	= ALM_REC_SYS_INFO;
		
		FilterAlarm_k[crt_n].phead =  &shm_info_rec_addr->head;
		FilterAlarm_k[crt_n].pRec   =(REC_ENTRY_UNION*) &shm_info_rec_addr->rec;	        
		InitPageInfo_k(&page_k[crt_n] , crt_n,shm_info_rec_addr->head.count) ;  /* initial  set  realtime record   */
		SetAdjInfo_k( &page_k[crt_n],adj1_k[crt_n]) ;  
		gtk_window_set_title(GTK_WINDOW(alm_pop_shell_k[crt_n]), pConvert=_toUtf8EX(OIX_CSTR_SYSMSG));
		G_SAFE_FREE(pConvert);
		if (!alm_timeout_id_k[crt_n])
			alm_timeout_id_k[crt_n]=  gtk_timeout_add ( INTERVAL,(GtkFunction)TimeCall_k, GINT_TO_POINTER(crt_n) ); 
		break; 
		
	case  3:  /*HDB_ALARM*/
		if  (shm_hdb_alm_k->connflag_al  == 1   )  /*chichi*/
		{
			if (HistDBDlg_k[crt_n] == (GtkWidget *)NULL)
			{
				HistDBDlg_k[crt_n] = CreateHistDBAlmDlg_k ( alm_pop_shell_k[crt_n] ,crt_n);
				gtk_widget_show(HistDBDlg_k[crt_n]);
			}
			else
				gtk_window_present (GTK_WINDOW(HistDBDlg_k[crt_n]));
		}else
		{
			PopupMessageBox(alm_pop_shell_k[crt_n] ,MSG_CONNECT_DATABASE_ERROR);	 
		}
		break;
	case 4: /*历史文件搜索*/
		if (HistFileDBDlg_k[crt_n] == (GtkWidget *)NULL)
		{
			HistFileDBDlg_k[crt_n] = CreateHistDBFileAlmDlg_k ( alm_pop_shell_k[crt_n] ,crt_n);
			gtk_widget_show(HistFileDBDlg_k[crt_n]);
		}
		else
			gtk_window_present (GTK_WINDOW(HistFileDBDlg_k[crt_n]));		
		break; 
	}  
	
	FilterAlarm_k[crt_n].is_change = TRUE; 
	AlmDspProc_k(crt_n) ;
	
}


void CreateAllNonDevBtn_k(GtkWidget *box2)
{
	int crt_n=0;
	
	GtkWidget *hbox;
	
	hbox =gtk_hbox_new (FALSE, 10); 	
	
	
}
int  SetFileterToEntry_k(GtkWidget *entry, time_t *TimeRes)
{
	time_t result  ; 
	struct tm *when ;
	/*int  year,mon,day,hh,mm,ss;*/
	char ss1[23]  ;
	
	
	when = localtime(  TimeRes );
	if (when ==  NULL || *TimeRes ==-1 || *TimeRes ==0 )
	{
		result = time(NULL);
		when = localtime( &result);
	}    
	
	sprintf(ss1,"%d-%d-%d %d:%d:%d",when->tm_year+1900,when->tm_mon+1,when->tm_mday,
		when->tm_hour,when->tm_min,when->tm_sec)  ; 
	gtk_entry_set_text(GTK_ENTRY(entry),ss1) ;
	
	/*
	year = mon = day = hh = mm = ss = -1 ;
	if (strcmp(ss1,"")!=0)
	{
	sscanf(ss1 , "%d-%d-%d %d:%d:%d" , &year,&mon,&day,&hh,&mm,&ss);
	when.tm_year = year -1900 ; 
	when.tm_mon = mon -1 ;
	when.tm_mday = day ;
	when.tm_hour = hh  ; 
	when.tm_min = mm ;
	when.tm_sec = ss ;
	when.tm_isdst = -1 ;
	*TimeRes  = mktime(&when);
	if ( *TimeRes !=   -1 )
	printf( "success is : %4d-%2d-%2d %2d:%2d:%2d\n",when.tm_year+1900,when.tm_mon+1,when.tm_mday,when.tm_hour,when.tm_min,when.tm_sec ) ; 
	sprintf(ss1,"%d-%d-%d %d:%d:%d",when.tm_year+1900,when.tm_mon+1,when.tm_mday,when.tm_hour,when.tm_min,when.tm_sec)  ;   
	gtk_entry_set_text(entry,ss1) ;
	return  TRUE;
	}
	else 
	{
	
	  return  FALSE;
	  }
	*/ 
	
	
	return FALSE;
}
int  SetTimeFilter_k  (GtkWidget *entry, time_t *TimeRes)
{
	
	struct tm when ;
	int  year,mon,day,hh,mm,ss;
	char ss1[23]  ;
	strcpy (ss1 ,gtk_entry_get_text(GTK_ENTRY(entry)));
	/*  strcpy(ss1,"2003-02-02 02:02:02");  */
	year = mon = day = hh = mm = ss = -1 ;
	if (strcmp(ss1,"")!=0)
	{
		sscanf(ss1 , "%d-%d-%d %d:%d:%d" , &year,&mon,&day,&hh,&mm,&ss);
		when.tm_year = year -1900 ; 
		when.tm_mon = mon -1 ;
		when.tm_mday = day ;
		when.tm_hour = hh  ; 
		when.tm_min = mm ;
		when.tm_sec = ss ;
		when.tm_isdst = -1 ;
		*TimeRes  = mktime(&when);
		if ( *TimeRes !=   -1 )
			printf( "success is : %04d-%02d-%02d %02d:%02d:%02d\n",when.tm_year+1900,when.tm_mon+1,when.tm_mday,when.tm_hour,when.tm_min,when.tm_sec ) ; 
		sprintf(ss1,"%d-%d-%d %d:%d:%d",when.tm_year+1900,when.tm_mon+1,when.tm_mday,when.tm_hour,when.tm_min,when.tm_sec)  ;   
		gtk_entry_set_text(GTK_ENTRY(entry),ss1) ;
		return  TRUE;
	}
	else 
	{
		
		return  FALSE;
	}
	
	
}    
int HistDBAlarmOK_k(GtkWidget *w , gpointer data)
{
	//#ifdef  SUN_OS   // added by chichi  
	int crt_n;
	gchar* ss1 ;
	int iMode ; 
	GtkWidget *startEntry , *endEntry,*cNameEntry ;
	gchar *pConvert=0;
	//    time_t startTm  ,endTm ;
	GtkWidget *window =data ;	
	GetCrtNumberOfWgt (w, &crt_n );
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return TRUE ;
	
	startEntry  = g_object_get_data(G_OBJECT(window),"startWgt");
	endEntry   = g_object_get_data(G_OBJECT(window), "endWgt");
	cNameEntry  = g_object_get_data(G_OBJECT(window), "cNameWgt");
	iMode  =  iHisNameMode_k[crt_n] ;
	ss1  = (gchar*)gtk_entry_get_text(GTK_ENTRY(startEntry));
	if (strcmp(ss1,"")!=0)
	{
		strcpy(shm_hdb_alm_k->data_al.starttime, pConvert=_fromUtf8( ss1));
		G_SAFE_FREE(pConvert);
	}
	ss1  = (gchar*)gtk_entry_get_text(GTK_ENTRY(endEntry));
	if (strcmp(ss1,"")!=0)
	{
		strcpy(shm_hdb_alm_k->data_al.endtime, pConvert=_fromUtf8( ss1));
		G_SAFE_FREE(pConvert);
	}
	
	ss1  = (gchar*)gtk_entry_get_text(GTK_ENTRY(cNameEntry));
	strcpy(shm_hdb_alm_k->data_al.cname, pConvert=_fromUtf8( ss1));
	G_SAFE_FREE(pConvert);
	
	shm_hdb_alm_k->data_al.cname_type  = 	iHisNameMode_k[crt_n] ; 
	shm_hdb_alm_k->data_al.queryflag  = 1 ;  /* start query !!! */
	shm_hdb_alm_k->data_al.querytype  = 0 ;
	
	HistDBAlmDlgDelete_k( w ,   data);  
	alm_rec_mode_k[crt_n]   =  ALM_REC_HISTRYDB;
	
	page_k[crt_n].PageStart =  0 ;             
	page_k[crt_n].RecCount = 0;  
	FilterAlarm_k[crt_n].is_change = TRUE;
	return FALSE ;
	
	
	//Fail:
	PopupMessageBox(alm_pop_shell_k[crt_n] , MSG_INVALID_DATA_FORMATE); 
	return TRUE ;
	//#endif
	alm_rec_mode_k[0]   =  ALM_REC_HISTRYDB;
	return TRUE ;
}

int DevFilterDlgDelete_k(GtkWidget *w , gpointer data) ;
int DevFilterDlgOK_k(GtkWidget *w , gpointer data)
{
	
	int crt_n,i;
	char *ss1 ;
	GtkWidget *startEntry ,	*endEntry , *searchEntry ,  *logNameEntry ;
	gchar *pConvert=0;
	GetCrtNumberOfWgt (w, &crt_n );
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return TRUE ;
	FilterAlarm_k[crt_n].dev_mask[0] = devMask_k[crt_n];
	for (i=0;i<MAX_GROUP_MASK_NUM;i++)
	{
		FilterAlarm_k[crt_n].group_mask[i] = groupMask_k[crt_n][i];
	}
	startEntry  = g_object_get_data(G_OBJECT(filterDlg_k[crt_n]),"startWgt");
	endEntry   = g_object_get_data(G_OBJECT(filterDlg_k[crt_n]),"endWgt");
	searchEntry = g_object_get_data(G_OBJECT(filterDlg_k[crt_n]),"searchWgt");
	logNameEntry = g_object_get_data(G_OBJECT(filterDlg_k[crt_n]),"logNameWgt");
	if (SetTimeFilter_k  ( startEntry, &FilterAlarm_k[crt_n].start_time) ==FALSE)  goto Fail  ;
	if (SetTimeFilter_k  ( endEntry,   &FilterAlarm_k[crt_n].end_time)	  ==FALSE )   goto Fail ; 
	if (FilterAlarm_k[crt_n].start_time  ==  FilterAlarm_k[crt_n].end_time )
		FilterAlarm_k[crt_n].start_time =  FilterAlarm_k[crt_n].end_time = -1 ;
	ss1  =(char*) gtk_entry_get_text(GTK_ENTRY(searchEntry));
	/* if (strcmp(ss1,"")!=0)*/
	strcpy((char *)(&FilterAlarm_k[crt_n].search_str), pConvert=_fromUtf8( ss1));
	G_SAFE_FREE(pConvert);
	ss1  = (char *)gtk_entry_get_text(GTK_ENTRY(logNameEntry));
	/*if (strcmp(ss1,"")!=0) */
	strcpy((char*)(&FilterAlarm_k[crt_n].LogicName), pConvert=_fromUtf8( ss1));
	G_SAFE_FREE(pConvert);
	
	FilterAlarm_k[crt_n].is_change = TRUE;
	FilterAlarm_k[crt_n].change_flag = TRUE;
	//	SaveCustomFilterDef(&FilterAlarm_k[crt_n],crt_n); //add by dm
	DevFilterDlgDelete_k( w ,   data);  
	AlmDspProc_k( crt_n);
	return FALSE ;
	
Fail:
	PopupMessageBox(alm_pop_shell_k[crt_n] , MSG_INVALID_DATA_FORMATE); 
	return TRUE ;
	
}

int HistDBAlmDlgDelete_k (GtkWidget *w , gpointer data)
{
	int  crt_n = (gint)data;
	GetCrtNumberOfWgt (w, &crt_n );
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE ;
	gtk_widget_destroy(HistDBDlg_k[crt_n] );
	HistDBDlg_k[crt_n]  = (GtkWidget*)NULL ;
	
	return TRUE;
}

int DevFilterDlgDelete_k(GtkWidget *w , gpointer data)
{
	int  crt_n = (gint)data;
	GetCrtNumberOfWgt (w, &crt_n );
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE ;
	gtk_widget_destroy(filterDlg_k[crt_n]);
	filterDlg_k[crt_n] = (GtkWidget*)NULL ;
	return TRUE;
}

int AttribFilterDlgDelete_k(GtkWidget *w , gpointer data)
{
	
	int  crt_n = (gint)data;
	GetCrtNumberOfWgt (w, &crt_n );
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE ;
	gtk_widget_destroy(AttribFilterDlg_k[crt_n]);
	AttribFilterDlg_k[crt_n] = (GtkWidget*)NULL ;
	return TRUE;
}

int CustomFilterDlgDelete_k(GtkWidget *w , gpointer data)
{
	int  crt_n = (gint)data;
	GetCrtNumberOfWgt (w, &crt_n );
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE ;
	gtk_widget_destroy(SetFilterDlg_k[crt_n]);
	SetFilterDlg_k[crt_n] = (GtkWidget*)NULL ;
	return TRUE;
}

gint doPrint_k (GtkWidget       *widget,   gpointer     data)
{
	int crt_n=(gint)data;
	
	if(!CheckUserPrivilege(cur_usr_name, PRIV_PRINT))
	{
		PopupMessageBox ( alm_pop_shell_k[crt_n], MSG_RPT_PRINTING  ); 
		return  TRUE;
	}
	
	
	//	if(page_k[crt_n].isSelected==0 )
	//	PopupMessageBox ( alm_pop_shell_k[crt_n], MSG_SELECT_PRINT_RANGE); 
	//	else  
	{
		AlmDspProcE_k( AlmOffDrawable_k[crt_n] ,crt_n,TRUE);
		
		PrintList(alm_pop_shell_k[crt_n],crt_n,ALM_LIST,AlarmPrintFile); 
	}
	return  TRUE;
}



int doDSPAlarmAck_k(GtkWidget       *widget,   gpointer     data)
{
	int i,  j ,crt_n ,send_num = 0 ;
	
	POINTER dbpt ; 
	USHORT  noAck;
	COMMAND_MSG	alm_msg[ALM_PAGE_SIZE];
	time_t     newtime;
	struct  tm      devtime;
	crt_n = (gint ) data ;
	newtime = time(NULL);
	devtime = *localtime( &newtime);
	if(!CheckUserPrivilege(cur_usr_name, PRIV_ALM_ACK))
	{
		PopupMessageBox ( widget, MSG_ALARM_ACK);
		return 0;
	}
	
	/*for (i = 0 , j= 0 ; i < index_fault_k[crt_n].count;j++, i++) */
	
	for (i =0 ,j = 0 ; j<= MIN(index_fault_k[crt_n].count,15) ; j++  )
	{
		
		if(-1==GetPointFromRec(&shm_rec_addr->rec[index_fault_k[crt_n].index_pt[i].pt],&dbpt))
			continue ;
		IsPtAlmNotAckedById(&dbpt, &noAck);
		if(!noAck)
			continue;
		alm_msg[i].type_id     = DPS_IOST_SET ;
		alm_msg[i].stn_id      = dbpt.stn_id ;
		alm_msg[i].dev_id      = dbpt.dev_id ;
		alm_msg[i].data_type   = dbpt.data_type ;
		alm_msg[i].point_id    = dbpt.pt_id ;
		alm_msg[i].status.opr  =  3  ;       /* ALM_ACK_STATE */
		alm_msg[i].status.state=  0  ;    /* ack */
		alm_msg[i].status.src  =  0  ;      /* ack */
		memset(&alm_msg[i].data ,0, sizeof (int));
		alm_msg[i].ctrl_addr   = 0 ;
		GetSrcId(&alm_msg[i].host_id);
		alm_msg[i].usr_id      = ( char ) cur_usr_id ;
		alm_msg[i].second	= devtime.tm_sec;
		alm_msg[i].minute	= devtime.tm_min;
		alm_msg[i].hour         = devtime.tm_hour;
		alm_msg[i].day          = devtime.tm_mday;
		alm_msg[i].month	= devtime.tm_mon+1;
		alm_msg[i].year	= devtime.tm_year+1900;	
		strcpy(alm_msg[i].usr_login_name, cur_usr_name);
		
#ifdef SUN_OS	
		Swap2Byte(&alm_msg[i].point_id );
		Swap2Byte(&alm_msg[i].status);
		Swap4Byte(&alm_msg[i].data);
		Swap2Byte(&alm_msg[i].year);
		
#endif  
		
		send_num++ ; 
		i++;
		
	}
	
	ProgAlarmAckBrdcst_k ( alm_msg ,   send_num) ;  /* broad cast !!! */
	return TRUE;
}

gint doAlarmAck_k(GtkWidget       *widget,   gpointer     data)
{  int crt_n = (int)data ;
doAlarmAckEx_k( widget,crt_n, page_k[crt_n].PageStart ,page_k[crt_n].PageEnd  ,FALSE ) ;
AlmDspProc_k(crt_n) ;
return TRUE;
}

int ComposeAlmMsg_k (int crt_n ,COMMAND_MSG	alm_msg[] ,int  i  ,  int iRec , struct  tm      *devtime   )
{
	POINTER dbpt ; 
	int   send_num = 0 ;
	USHORT  noAck;
	if(-1==GetPointFromRec(&shm_rec_addr->rec[iRec],&dbpt))
		return FALSE;
	
	switch(alm_rec_mode_k[crt_n] )
	{
	case ALM_REC_REAL1:    /*ALM_REC_REAL1*/
        		
		if(-1==GetPointFromRec(&shm_rec_addr->rec[iRec],&dbpt))
		return FALSE;

		break;   
		
		
	case ALM_REC_SYS_INFO :    /*综合信息*/
			        
		if(-1==GetPointFromRec(&shm_info_rec_addr->rec[iRec],&dbpt))
		return FALSE;

		break; 

	case 	ALM_REC_HISTRY1:	//*.rec file
		if(-1==GetPointFromRec(&hist_rec_addr_k[crt_n].rec[iRec],&dbpt))
		return FALSE;

	}
	IsPtAlmNotAckedById(&dbpt, &noAck);
	if(!noAck)
		return FALSE;
	alm_msg[i].type_id     = DPS_IOST_SET ;
	alm_msg[i].stn_id      = dbpt.stn_id ;
	alm_msg[i].dev_id      = dbpt.dev_id ;
	alm_msg[i].data_type   = dbpt.data_type ;
	alm_msg[i].point_id    = dbpt.pt_id ;
	alm_msg[i].status.opr  =  3  ;       /* ALM_ACK_STATE */
	alm_msg[i].status.state=  0  ;    /* ack */
	alm_msg[i].status.src  =  0  ;      /* ack */
	memset(&alm_msg[i].data ,0, sizeof (int));
	alm_msg[i].ctrl_addr   = 0 ;
	GetSrcId(&alm_msg[i].host_id);
	alm_msg[i].usr_id      = ( char ) cur_usr_id ;
	alm_msg[i].second	= devtime->tm_sec;
	alm_msg[i].minute	= devtime->tm_min;
	alm_msg[i].hour         = devtime->tm_hour;
	alm_msg[i].day          = devtime->tm_mday;
	alm_msg[i].month	= devtime->tm_mon+1;
	alm_msg[i].year	= devtime->tm_year+1900;	
	strcpy(alm_msg[i].usr_login_name, cur_usr_name);
	
#ifdef SUN_OS	
	Swap2Byte(&alm_msg[i].point_id );
	Swap2Byte(&alm_msg[i].status);
	Swap4Byte(&alm_msg[i].data);
	Swap2Byte(&alm_msg[i].year);
	
#endif  
	
	return TRUE; 
	
}		


gint doAlarmAckRange_k(GtkWidget       *widget,   gpointer     data)
{  int crt_n = (int)data ;

if (!page_k[crt_n].isSelected)
{
	
	PopupMessageBox(alm_pop_shell_k[crt_n],MSG_SELECT_ACK_RANGE);
	return FALSE ;
}
/*printf ("$$$$$$select start =%d , end = %d \n" ,page_k[crt_n].SelectStart ,page_k[crt_n].SelectEnd ) ;*/
doAlarmAckEx_k( widget,  crt_n ,  0 ,0  ,TRUE)  ;
MySleep(1000) ;
AlmDspProc_k(crt_n) ;
return TRUE;
}

gint doAlarmAckEx_k(GtkWidget       *widget, int crt_n ,  int startI ,int  endI  ,BOOL isRangeAck )
{   
	UINT i;
	int  j  ;
	int send_num = 0 ;
	COMMAND_MSG	alm_msg[ALM_PAGE_SIZE];
	time_t    newtime;
	struct  tm      devtime;
	
	newtime = time(NULL);
	devtime = *localtime( &newtime);
	if(!CheckUserPrivilege(cur_usr_name, PRIV_ALM_ACK))
	{
		PopupMessageBox ( alm_pop_shell_k[crt_n], MSG_ALARM_ACK);
		return 0;
	}
	if (isRangeAck)
	{
		for (i =j = 0   ; i< sel_pt_k[crt_n].nextpos ; i++  )
		{
			if (!ComposeAlmMsg_k (  crt_n , 	alm_msg , j  ,sel_pt_k[crt_n].sel[i] ,&devtime     ))
				continue ; 
			send_num++ ;
			j++   ;      
		}
	}else
	{	
		
		for (i =0 ,j = startI ; j<= endI ; j++  )
		{
			if (!ComposeAlmMsg_k (  crt_n , 	alm_msg , i  ,index_tmp_k[crt_n].index_pt[j].pt,&devtime   ) )
				continue;
			send_num++ ; 
			i++    ;
		}
	}
	
	if ( send_num>0)  
		ProgAlarmAckBrdcst_k ( alm_msg ,   send_num) ;  /* broad cast !!! */
	return TRUE;
}


gint isDisplayMore_k(GtkWidget       *widget,   gpointer     data)
{  
	GtkWidget *window =  (GtkWidget*)data;
	GtkWidget *frame = g_object_get_data(G_OBJECT(window),"frameWgt");
	if (gtk_toggle_button_get_active  (GTK_TOGGLE_BUTTON(widget)))
		gtk_widget_show(frame);
	else
		gtk_widget_hide(frame);
	return TRUE;
}


const char *sName_k[] = {OIX_STR_CHINESE,OIX_STR_FORGEIN};
static GtkWidget* CreateHistDBAlmDlg_k(GtkWidget *parent ,int crt_n)
{
	gint i ; 
	GtkWidget *optionmenu   , *menu , *items ;
	GtkWidget  *windows1 ,*vbox1  , *frame1 ;
	GtkWidget *table1 ,*label4 ,*label5 ,*entry1, *entry2;
	GtkWidget *entry3, *hbox1 ,*button1  ,*button2 ;
	
	time_t     newtime;
	struct  tm      devtime; 
	gchar *pConvert=0;
	
	char timer[30]; 
	newtime = time(NULL);
	devtime = *localtime( &newtime);
	
	sprintf (timer , "%4d-%02d-%02d %02d:%02d:%02d" ,devtime.tm_year+1900,
		devtime.tm_mon+1 ,devtime.tm_mday , devtime.tm_hour, devtime.tm_min ,devtime.tm_sec) ;
	
	
	windows1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(windows1), pConvert=_toUtf8("HDB ALARM DLIALOG"));
	G_SAFE_FREE(pConvert);
	gtk_window_set_transient_for(GTK_WINDOW(windows1), GTK_WINDOW(parent)); 
	gtk_window_set_destroy_with_parent(GTK_WINDOW(windows1),TRUE);
	
	gtk_signal_connect (GTK_OBJECT (windows1), "delete_event", 
		GTK_SIGNAL_FUNC (HistDBAlmDlgDelete_k), GINT_TO_POINTER (crt_n)); 
	
	gtk_widget_show (windows1);
	vbox1 = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox1);
	gtk_container_add (GTK_CONTAINER (windows1), vbox1);
	
	
	
	frame1 = gtk_frame_new (NULL);
	gtk_widget_show (frame1); 
	g_object_set_data(G_OBJECT(windows1),"frameWgt",frame1);
	gtk_box_pack_start (GTK_BOX (vbox1), frame1, FALSE, FALSE, 0);
	
	table1 = gtk_table_new (2, 2, FALSE);
	gtk_widget_show (table1);
	gtk_container_add (GTK_CONTAINER (frame1), table1);
	
	
	
	label4 = gtk_label_new (pConvert=_toUtf8EX(OIX_STR_STATR_DATE));
	G_SAFE_FREE(pConvert);
	gtk_widget_show (label4);
	gtk_table_attach (GTK_TABLE (table1), label4, 0, 1, 0, 1,
		(GtkAttachOptions) (GTK_FILL),
		(GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label4), GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label4), 0, 0.5);
	
	
	
	label5 = gtk_label_new (pConvert=_toUtf8EX(OIX_STR_END_DATE));
	G_SAFE_FREE(pConvert);
	gtk_widget_show (label5);
	gtk_table_attach (GTK_TABLE (table1), label5, 0, 1, 1, 2,
		(GtkAttachOptions) (GTK_FILL),
		(GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label5), GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label5), 0, 0.5);
	
	
	
	entry1 = gtk_entry_new ();
	gtk_widget_show (entry1);
	gtk_table_attach (GTK_TABLE (table1), entry1, 1, 2, 0, 1,
		(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
		(GtkAttachOptions) (0), 0, 0);
	g_object_set_data(G_OBJECT(windows1),"startWgt",entry1);
	
	entry2 = gtk_entry_new ();
	gtk_widget_show (entry2);
	gtk_table_attach (GTK_TABLE (table1), entry2, 1, 2, 1, 2,
		(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
		(GtkAttachOptions) (0), 0, 0);
	
	g_object_set_data(G_OBJECT(windows1),"endWgt",entry2);
	
	/***********************************************************************************/
	optionmenu = gtk_option_menu_new();
	gtk_widget_show(optionmenu);
	gtk_table_attach (GTK_TABLE (table1), optionmenu, 0, 1, 2, 3,
		(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
		(GtkAttachOptions) (0), 0, 0);
	menu =gtk_menu_new();
	gtk_widget_set_usize (menu, 130, -1);
	/*  OIX_CSTR_REALTIME_REC ,OIX_CSTR_HIST_REC*/
	for (i = 0 ; i < 2  ; i++)
	{
		items = gtk_menu_item_new_with_label(pConvert=_toUtf8EX((char*)sName_k[i]));
		G_SAFE_FREE(pConvert);
		gtk_widget_show( items);
		gtk_container_add(GTK_CONTAINER(menu),items);
		gtk_signal_connect (GTK_OBJECT (items), "activate",GTK_SIGNAL_FUNC (SetHistAlmCName_k), GINT_TO_POINTER(i));
	}   
	
	
	gtk_option_menu_set_menu(GTK_OPTION_MENU(optionmenu),menu);
	
	
	/***********************************************************************************/
	entry3 = gtk_entry_new ();
	gtk_widget_show (entry3);
	gtk_table_attach (GTK_TABLE (table1), entry3, 1, 2, 2, 3,
		(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
		(GtkAttachOptions) (0), 0, 0);
	
	/*add by hcl*/
	if(strlen(shm_hdb_alm_k->data_al.cname)>0)
	{
		gtk_entry_set_text(GTK_ENTRY(entry3),pConvert=_toUtf8(shm_hdb_alm_k->data_al.cname));
		G_SAFE_FREE(pConvert);
	}
	
	g_object_set_data(G_OBJECT(windows1),"cNameWgt",entry3);
	
	if(strlen(shm_hdb_alm_k->data_al.starttime)>0)
	{
		gtk_entry_set_text(GTK_ENTRY(entry1), pConvert=_toUtf8(shm_hdb_alm_k->data_al.starttime));
		G_SAFE_FREE(pConvert);
	}
	else
		gtk_entry_set_text(GTK_ENTRY(entry1),timer);
	
	if(strlen(shm_hdb_alm_k->data_al.endtime)>0)
	{
		gtk_entry_set_text(GTK_ENTRY(entry2), pConvert=_toUtf8(shm_hdb_alm_k->data_al.endtime));
		G_SAFE_FREE(pConvert);
	}
	else
		gtk_entry_set_text(GTK_ENTRY(entry2),timer);
	
	
	
	hbox1 = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox1);
	gtk_box_pack_start (GTK_BOX (vbox1), hbox1, FALSE, FALSE, 0);
	
	button1 = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_CANCEL));//gtk_button_new_from_stock ("gtk-cancel");
	G_SAFE_FREE(pConvert);
	gtk_widget_show (button1);
	gtk_box_pack_end (GTK_BOX (hbox1), button1, FALSE, FALSE, 0);
	
	button2 = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_APPLY));
	G_SAFE_FREE(pConvert);
	//gtk_button_new_from_stock ("gtk-apply");
	gtk_widget_show (button2);
	gtk_box_pack_end (GTK_BOX (hbox1), button2, FALSE, FALSE, 0);
	
	gtk_signal_connect (GTK_OBJECT (button1), "clicked",GTK_SIGNAL_FUNC (HistDBAlmDlgDelete_k), windows1);  
	gtk_signal_connect (GTK_OBJECT (button2), "clicked",GTK_SIGNAL_FUNC (HistDBAlarmOK_k), windows1);
	
	gtk_window_set_position(GTK_WINDOW(windows1),GTK_WIN_POS_CENTER);  
	iHisNameMode_k[crt_n] =  1 ;  
	return  windows1 ;
	
	
}

static void
SetDevMask_a_k (GtkCellRendererToggle *cell,
			gchar                 *path_str,
			gpointer               data)
{
	GtkTreeIter  iterDev,iterGroup;
	GtkTreePath *path = gtk_tree_path_new_from_string (path_str);
	gboolean fixed;
//	int j; 
	gint crt_n = (gint) data ;
	gint i = (gint)g_ascii_strtoll(path_str,NULL,0);
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return  ;
	gtk_tree_model_get_iter (GTK_TREE_MODEL(storeDev_a_k[crt_n]), &iterDev, path);
	gtk_tree_model_get (GTK_TREE_MODEL(storeDev_a_k[crt_n]), &iterDev, COLUMN_CHECKED, &fixed, -1);
	if (!fixed)
	{
		devMask_k[crt_n] = devMask_k[crt_n]  | (1<<i);
		groupMask_k[crt_n][i] = 0xFFFFFFFF;
		if (i==curDev_a_k&&gtk_tree_model_get_iter_first(GTK_TREE_MODEL(storeGroup_a_k[crt_n]),&iterGroup))
		{
			do 
			{
				gtk_list_store_set (storeGroup_a_k[crt_n], &iterGroup, COLUMN_CHECKED, TRUE, -1);
			} while (gtk_tree_model_iter_next(GTK_TREE_MODEL(storeGroup_a_k[crt_n]),&iterGroup));
		}
	}
	else
	{
		devMask_k[crt_n] = devMask_k[crt_n]  & (~(1<<i)); /*  Clear bit N */  
		groupMask_k[crt_n][i] = 0;
		if (i==curDev_a_k&&gtk_tree_model_get_iter_first(GTK_TREE_MODEL(storeGroup_a_k[crt_n]),&iterGroup))
		{
			do 
			{
				gtk_list_store_set (storeGroup_a_k[crt_n], &iterGroup, COLUMN_CHECKED, FALSE, -1);
			} while (gtk_tree_model_iter_next(GTK_TREE_MODEL(storeGroup_a_k[crt_n]),&iterGroup));
		}
	}

	fixed ^= 1;
	gtk_list_store_set (storeDev_a_k[crt_n], &iterDev, COLUMN_CHECKED, fixed, -1);
	//DevClicked_a_k(GTK_TREE_VIEW(gtk_tree_view_new_with_model(GTK_TREE_MODEL(storeDev_a_k[crt_n]))),data);
	gtk_tree_path_free (path);  
}

static void
SetGroupMask_a_k (GtkCellRendererToggle *cell,
			  gchar                 *path_str,
			  gpointer               data)
{
	GtkTreeIter  iter;
	GtkTreePath *path = gtk_tree_path_new_from_string (path_str);
	gboolean fixed;
//	int j; 
	gint crt_n = (gint) data ;
	gint i =(gint) g_ascii_strtoll(path_str,NULL,0);
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return ;
	
	gtk_tree_model_get_iter (GTK_TREE_MODEL(storeGroup_a_k[crt_n]), &iter, path);
	gtk_tree_model_get (GTK_TREE_MODEL(storeGroup_a_k[crt_n]), &iter, COLUMN_CHECKED, &fixed, -1);
	if (!fixed)
		groupMask_k[crt_n][curDev_a_k] = groupMask_k[crt_n][curDev_a_k]  | (1<<i);
	else
		groupMask_k[crt_n][curDev_a_k] = groupMask_k[crt_n][curDev_a_k]  & (~(1<<i)); /*  Clear bit N */  
	
	fixed ^= 1;
	gtk_list_store_set (storeGroup_a_k[crt_n], &iter, COLUMN_CHECKED, fixed, -1);
	gtk_tree_path_free (path);  
}

gint SetDevMaskAllOrNone_k(GtkWidget *widget , gpointer data)
{
	int i  ;
	int crt_n ,j;
	GtkTreeIter  iter;
	int stn_idx,station_num;
	GetCrtNumberOfWgt (widget, &crt_n );
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE ;

	GetStationNum(&station_num);
	stn_idx=0;
	if (station_num>=1)
	{
		for(i=1;i<=station_num;i++) 
		{
			if (keySignalStnDef[i].Id==FilterAlarm_k[crt_n].stn_id)
			{
				stn_idx=i;
				break;
			}
			
		}
	}

	if ((gint) data == SHOW_NONE)  /* show all !!!*/
	{
		if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(storeDev_a_k[crt_n]),&iter))
		{
			gtk_list_store_set (storeDev_a_k[crt_n], &iter, COLUMN_CHECKED, FALSE, -1);
			devMask_k[crt_n] = devMask_k[crt_n]  & (~(1<<0));
		}
		for (j = 1 ; j <= keySignalStnDef[stn_idx].dev_num ; j++)
		{			
			if (gtk_tree_model_iter_next(GTK_TREE_MODEL(storeDev_a_k[crt_n]),&iter))
			{
				gtk_list_store_set (storeDev_a_k[crt_n], &iter, COLUMN_CHECKED, FALSE, -1);
				devMask_k[crt_n] = devMask_k[crt_n]  & (~(1<<j));
			}
		}
		if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(storeGroup_a_k[crt_n]),&iter))
		{
			gtk_list_store_set (storeGroup_a_k[crt_n], &iter, COLUMN_CHECKED, FALSE, -1);
		}
		for (j=1;j<=(int)keySignalStnDef[stn_idx].dev[curDev_a_k+1].group_num;j++)
		{
			if (gtk_tree_model_iter_next(GTK_TREE_MODEL(storeGroup_a_k[crt_n]),&iter))
			{
				gtk_list_store_set (storeGroup_a_k[crt_n], &iter, COLUMN_CHECKED, FALSE, -1);
			}
		}
		for (j=0;j<MAX_GROUP_MASK_NUM;j++)
		{
			groupMask_k[crt_n][j] = 0;
		}
	}else if ((gint) data ==SHOW_ALL) /*  show none !!!*/
	{
		if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(storeDev_a_k[crt_n]),&iter))
		{
			gtk_list_store_set (storeDev_a_k[crt_n], &iter, COLUMN_CHECKED, TRUE, -1);
			devMask_k[crt_n] = devMask_k[crt_n]  | (1<<0);
		}
		for (j = 1 ; j <= keySignalStnDef[stn_idx].dev_num ; j++)
		{
			if (gtk_tree_model_iter_next(GTK_TREE_MODEL(storeDev_a_k[crt_n]),&iter))
			{
				gtk_list_store_set (storeDev_a_k[crt_n], &iter, COLUMN_CHECKED, TRUE, -1);  
				devMask_k[crt_n] = devMask_k[crt_n]  | (1<<j);
			}
		}
		if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(storeGroup_a_k[crt_n]),&iter))
		{
			gtk_list_store_set (storeGroup_a_k[crt_n], &iter, COLUMN_CHECKED, TRUE, -1);
		}
		for (j=1;j<=(int)keySignalStnDef[stn_idx].dev[curDev_a_k+1].group_num;j++)
		{
			if (gtk_tree_model_iter_next(GTK_TREE_MODEL(storeGroup_a_k[crt_n]),&iter))
			{
				gtk_list_store_set (storeGroup_a_k[crt_n], &iter, COLUMN_CHECKED, TRUE, -1);
			}
		}
		for (j=0;j<MAX_GROUP_MASK_NUM;j++)
		{
			groupMask_k[crt_n][j] = 0xffffffff;
		}
	}
	return TRUE;
}
gboolean DevClicked_a_k(GtkTreeView *treeview,
					   gpointer     data)
{
	GtkTreePath  *path;
	GtkTreeModel *model;
	GtkTreeIter  iter;
	int i,j;
	gchar *devName;
	BOOL groupIsChecked,fixed;
	gint *path_num;
	gchar *pConvert=0, *pConvert1=0;
	gint crt_n = (gint) data ;
	int stn_idx,station_num;
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE;
	gtk_tree_view_get_cursor(treeview,&path,NULL);
	path_num = gtk_tree_path_get_indices(path);
	curDev_a_k = *path_num;
	model = gtk_tree_view_get_model(treeview);
	gtk_tree_model_get_iter(model,&iter,path);
	gtk_tree_model_get(model,&iter,COLUMN_CHECKED,&fixed,-1);
	gtk_tree_model_get (model, &iter, COLUMN_NAME, &devName, -1);
	gtk_list_store_clear(storeGroup_a_k[crt_n]);

	GetStationNum(&station_num);
	stn_idx=0;
	if (station_num>=1)
	{
		for(i=1;i<=station_num;i++) 
		{
			if (keySignalStnDef[i].Id==FilterAlarm_k[crt_n].stn_id)
			{
				stn_idx=i;
				break;
			}
			
		}
	}

	if (fixed)
	{
		for(i=1;i<=keySignalStnDef[stn_idx].dev_num;i++)
		{
			if (!strcmp(keySignalStnDef[stn_idx].dev[i].CName,pConvert1=_fromUtf8(devName)))
			{
				for (j=1;j<=(int)keySignalStnDef[stn_idx].dev[i].group_num;j++)
				{
					if(strcmp(keySignalStnDef[stn_idx].dev[i].group[j].CName,"")!=0
						&&keySignalStnDef[stn_idx].dev[i].group[j].isFilter[crt_n] == FALSE)
					{ 
						gtk_list_store_append (storeGroup_a_k[crt_n], &iter);
						if (groupMask_k[crt_n][curDev_a_k] & (0X1<<(j-1)) ) 
							groupIsChecked = TRUE;  
						else
							groupIsChecked = FALSE;
						gtk_list_store_set (storeGroup_a_k[crt_n], &iter,COLUMN_CHECKED, groupIsChecked,COLUMN_NAME, 
							pConvert=_toUtf8(keySignalStnDef[stn_idx].dev[i].group[j].CName),-1);
						G_SAFE_FREE(pConvert);
					}
				}
			}

			G_SAFE_FREE(pConvert1);
		}
	}
	g_free(devName);
	return TRUE;
}
static GtkWidget*
CreateDevFilterDlg_k(GtkWidget *parent ,int crt_n)
{
	int i   ;
	struct  tm      devtime; 
	char timer[30]; 
	time_t     newtime;
	GtkWidget  *windows1 ,*vbox1  ,*hbox, *frame1 ;
	GtkWidget *table1 ,*label4 ,*label5,  *label6 ,*label7,*entry1, *entry2,*entry4;
	GtkWidget *entry3 ,*hbox1 ,*button1  ,*button2 ,*checkbutton3;
	GtkWidget *devAll,*devNone;
	GtkWidget *listDev,*listGroup;
	GtkTreeIter iter;
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	gchar *pConvert=0;
	int station_num,stn_idx;
//	GtkTreeSelection *treeSelection;
	BOOL	devIsChecked;
	char *sName_k[] ={OIX_CSTR_ALM_REC_NON ,OIX_CSTR_ALM_REC_ALL      };
	devMask_k[crt_n] =  FilterAlarm_k[crt_n].dev_mask[0] ;
	for (i=0;i<MAX_GROUP_MASK_NUM;i++)
	{
		groupMask_k[crt_n][i] = FilterAlarm_k[crt_n].group_mask[i];
	}
	
	newtime = time(NULL);
	devtime = *localtime( &newtime);
	
	sprintf (timer , "%4d-%02d-%02d %02d:%02d:%02d" ,devtime.tm_year+1900,
	devtime.tm_mon+1 ,devtime.tm_mday , devtime.tm_hour, devtime.tm_min ,devtime.tm_sec) ;
	
	
	windows1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(windows1), pConvert=_toUtf8EX(OIX_STR_DEV_FILTER));
	G_SAFE_FREE(pConvert);
	gtk_window_set_transient_for(GTK_WINDOW(windows1), GTK_WINDOW(parent)); 
	
	gtk_signal_connect (GTK_OBJECT (windows1), "delete_event", 
	GTK_SIGNAL_FUNC (DevFilterDlgDelete_k),  GINT_TO_POINTER(crt_n)); 
	
	gtk_window_set_destroy_with_parent(GTK_WINDOW(windows1),TRUE);
	gtk_window_set_position(GTK_WINDOW(windows1),GTK_WIN_POS_CENTER);  
	gtk_widget_show (windows1);
	vbox1 = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox1);
	gtk_container_add (GTK_CONTAINER (windows1), vbox1);
	printf ("\n ######stn.id= %d",FilterAlarm_k[crt_n].stn_id);

	hbox = gtk_hbox_new (FALSE, 0);
 	gtk_widget_show (hbox);
	storeDev_a_k[crt_n] = gtk_list_store_new (NUM_COLUMNS,G_TYPE_BOOLEAN,G_TYPE_STRING);
	storeGroup_a_k[crt_n] = gtk_list_store_new (NUM_COLUMNS,G_TYPE_BOOLEAN,G_TYPE_STRING);

	GetStationNum(&station_num);
	stn_idx=0;
	if (station_num>=1)
	{
		for(i=1;i<=station_num;i++) 
		{
			if (keySignalStnDef[i].Id==FilterAlarm_k[crt_n].stn_id)
			{
				stn_idx=i;
				break;
			}
			
		}
	}

	for(i=1;i<=keySignalStnDef[stn_idx].dev_num;i++)
	{
		if(strcmp(keySignalStnDef[stn_idx].dev[i].CName,"")!=0)
		{ 
			if (FilterAlarm_k[crt_n].dev_mask[0] & (0X1<<(i-1)) ) 
				devIsChecked = TRUE;  
			else
				devIsChecked = FALSE;
			gtk_list_store_append (storeDev_a_k[crt_n], &iter);
			gtk_list_store_set (storeDev_a_k[crt_n], &iter,COLUMN_CHECKED, devIsChecked,COLUMN_NAME, 
				pConvert=_toUtf8(keySignalStnDef[stn_idx].dev[i].CName),-1);
			G_SAFE_FREE(pConvert);
		}
	}

	listDev = gtk_tree_view_new_with_model (GTK_TREE_MODEL(storeDev_a_k[crt_n]));
	//treeSelection = gtk_tree_view_get_selection(GTK_TREE_VIEW(listDev));
	//gtk_tree_selection_set_mode(treeSelection,GTK_SELECTION_SINGLE);
	gtk_signal_connect(GTK_OBJECT(listDev),"cursor-changed",G_CALLBACK(DevClicked_a_k),GINT_TO_POINTER(crt_n));
	g_object_unref (storeDev_a_k[crt_n]);
	renderer = gtk_cell_renderer_toggle_new ();
	g_signal_connect (renderer, "toggled",G_CALLBACK (SetDevMask_a_k), GINT_TO_POINTER(crt_n));
	column = gtk_tree_view_column_new_with_attributes (NULL,renderer,"active", COLUMN_CHECKED,NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW(listDev), column);
	renderer = gtk_cell_renderer_text_new ();
	column = gtk_tree_view_column_new_with_attributes (NULL,renderer,"text",COLUMN_NAME,NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW(listDev), column);
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(listDev),FALSE);
	gtk_box_pack_start (GTK_BOX (hbox), listDev, FALSE, FALSE, 0);

	listGroup = gtk_tree_view_new_with_model(GTK_TREE_MODEL(storeGroup_a_k[crt_n]));
	g_object_unref (storeGroup_a_k[crt_n]);
	renderer = gtk_cell_renderer_toggle_new ();
	g_signal_connect (renderer, "toggled",G_CALLBACK (SetGroupMask_a_k), GINT_TO_POINTER(crt_n));
	column = gtk_tree_view_column_new_with_attributes (NULL,renderer,"active", COLUMN_CHECKED,NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW(listGroup), column);
	renderer = gtk_cell_renderer_text_new ();
	column = gtk_tree_view_column_new_with_attributes (NULL,renderer,"text",COLUMN_NAME,NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW(listGroup), column);
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(listGroup),FALSE);
	gtk_box_pack_start (GTK_BOX (hbox), listGroup, TRUE, TRUE, 0);

	gtk_box_pack_start (GTK_BOX (vbox1), hbox, FALSE, FALSE, 0);
	gtk_widget_show(listDev);
	gtk_widget_show(listGroup);
	
	devNone = gtk_button_new_with_label(pConvert=_toUtf8(_CS_(sName_k[0])));
	G_SAFE_FREE(pConvert);
	gtk_widget_show(devNone);
	gtk_box_pack_start (GTK_BOX (vbox1),  devNone, FALSE, FALSE, 0);
	gtk_signal_connect (GTK_OBJECT (devNone), "clicked",GTK_SIGNAL_FUNC (SetDevMaskAllOrNone_k),GINT_TO_POINTER( SHOW_NONE));
	
	devAll = gtk_button_new_with_label(pConvert=_toUtf8(_CS_(sName_k[1])));
	G_SAFE_FREE(pConvert);
	gtk_widget_show(devAll);
	gtk_box_pack_start (GTK_BOX (vbox1),  devAll, FALSE, FALSE, 0);
	gtk_signal_connect (GTK_OBJECT (devAll), "clicked",GTK_SIGNAL_FUNC (SetDevMaskAllOrNone_k),GINT_TO_POINTER( SHOW_ALL));
	
	frame1 = gtk_frame_new (NULL);
	/*  gtk_widget_show (frame1);*/
	g_object_set_data(G_OBJECT(windows1),"frameWgt",frame1);
	gtk_box_pack_start (GTK_BOX (vbox1), frame1, FALSE, FALSE, 0);
	
	table1 = gtk_table_new (4, 2, FALSE);  /*3*/
	gtk_widget_show (table1);
	gtk_container_add (GTK_CONTAINER (frame1), table1);
	
	
	
	label4 = gtk_label_new (pConvert=_toUtf8EX(OIX_STR_STATR_DATE));
	G_SAFE_FREE(pConvert);
	gtk_widget_show (label4);
	gtk_table_attach (GTK_TABLE (table1), label4, 0, 1, 0, 1,
	(GtkAttachOptions) (GTK_FILL),
	(GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label4), GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label4), 0, 0.5);
	
	
	
	label5 = gtk_label_new (pConvert=_toUtf8EX(OIX_STR_END_DATE));
	G_SAFE_FREE(pConvert);
	gtk_widget_show (label5);
	gtk_table_attach (GTK_TABLE (table1), label5, 0, 1, 1, 2,
	(GtkAttachOptions) (GTK_FILL),
	(GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label5), GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label5), 0, 0.5);
	
	
	
	label6 = gtk_label_new (pConvert=_toUtf8EX(OIX_STR_SEARCH_STR));
	G_SAFE_FREE(pConvert);
	gtk_widget_show (label6);
	gtk_table_attach (GTK_TABLE (table1), label6, 0, 1, 2, 3,
	(GtkAttachOptions) (GTK_FILL),
	(GtkAttachOptions) (0), 0, 0);
	
	label7 = gtk_label_new (pConvert=_toUtf8EX( OIX_STR_LOGNAME_STR));
	G_SAFE_FREE(pConvert);
	gtk_widget_show (label7);
	gtk_table_attach (GTK_TABLE (table1), label7, 0, 1, 3, 4,
	(GtkAttachOptions) (GTK_FILL),
	(GtkAttachOptions) (0), 0, 0);
	
	
	
	gtk_label_set_justify (GTK_LABEL (label6), GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label6), 0, 0.5);
	
	entry1 = gtk_entry_new ();
	gtk_widget_show (entry1);
	gtk_table_attach (GTK_TABLE (table1), entry1, 1, 2, 0, 1,
	(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
	(GtkAttachOptions) (0), 0, 0);
	g_object_set_data(G_OBJECT(windows1),"startWgt",entry1);
	gtk_entry_set_max_length(GTK_ENTRY(entry1),20);
	SetFileterToEntry_k  ( entry1, &FilterAlarm_k[crt_n].start_time)    ;
	
	
	entry2 = gtk_entry_new ();
	gtk_widget_show (entry2);
	gtk_table_attach (GTK_TABLE (table1), entry2, 1, 2, 1, 2,
	(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
	(GtkAttachOptions) (0), 0, 0);
	
	g_object_set_data(G_OBJECT (windows1),"endWgt",entry2);
	gtk_entry_set_max_length(GTK_ENTRY(entry2),20);
	SetFileterToEntry_k  ( entry2, &FilterAlarm_k[crt_n].end_time)    ;
	
	entry3 = gtk_entry_new ();
	gtk_widget_show (entry3);
	gtk_table_attach (GTK_TABLE (table1), entry3, 1, 2, 2, 3,
	(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
	(GtkAttachOptions) (0), 0, 0);
	
	
	g_object_set_data(G_OBJECT(windows1),"searchWgt",entry3);
	gtk_entry_set_max_length(GTK_ENTRY(entry3),MAX_SEARCH_NAME_SIZE);
	
	if (FilterAlarm_k[crt_n].search_str[0] !=0)  
	{
		gtk_entry_set_text(GTK_ENTRY(entry3), pConvert=_toUtf8(FilterAlarm_k[crt_n].search_str));
		G_SAFE_FREE(pConvert);
	}
	
	entry4 = gtk_entry_new ();
	gtk_widget_show (entry4);
	gtk_table_attach (GTK_TABLE (table1), entry4, 1, 2, 3, 4,
	(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
	(GtkAttachOptions) (0), 0, 0);
	
	g_object_set_data(G_OBJECT(windows1),"logNameWgt",entry4);
	gtk_entry_set_max_length(GTK_ENTRY(entry4),50);
	
	if (FilterAlarm_k[crt_n].LogicName[0] !=0)  
	{
		gtk_entry_set_text(GTK_ENTRY(entry4), pConvert=_toUtf8(FilterAlarm_k[crt_n].LogicName));
		G_SAFE_FREE(pConvert);
	}
	
	hbox1 = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox1);
	gtk_box_pack_start (GTK_BOX (vbox1), hbox1, FALSE, FALSE, 0);
	
	button1 = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_CANCEL));
	G_SAFE_FREE(pConvert);
	//gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (button1);
	gtk_box_pack_end (GTK_BOX (hbox1), button1, FALSE, FALSE, 0);
	
	button2 = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_APPLY));
	G_SAFE_FREE(pConvert);
	//gtk_button_new_from_stock ("gtk-apply");
	gtk_widget_show (button2);
	gtk_box_pack_end (GTK_BOX (hbox1), button2, FALSE, FALSE, 0);
	
	checkbutton3 = gtk_check_button_new_with_label (pConvert=_toUtf8(_CS_(OIX_STR_MORE)));
	G_SAFE_FREE(pConvert);
	gtk_widget_show (checkbutton3);
	gtk_box_pack_end (GTK_BOX (hbox1), checkbutton3, FALSE, FALSE, 0);
	
	gtk_signal_connect (GTK_OBJECT (checkbutton3), "toggled",GTK_SIGNAL_FUNC (isDisplayMore_k), windows1);    
	gtk_signal_connect (GTK_OBJECT (button1), "clicked",GTK_SIGNAL_FUNC (DevFilterDlgDelete_k), windows1);  
	gtk_signal_connect (GTK_OBJECT (button2), "clicked",GTK_SIGNAL_FUNC (DevFilterDlgOK_k), windows1);
	
	gtk_toggle_button_set_active(checkbutton3,1) ; //added by chi 
	gtk_window_move(GTK_WINDOW(windows1) , 100, 100);       
	
	return  windows1 ;
	
	
}

int AttribCheckToggled_k(GtkWidget *widget,gpointer data)
{
	int i  ;
	int crt_n ;
	crt_n=getCrtNumFromPointer((int)data);
	i=getDataFromPointer((int)data);
	
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE;
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)))
	{
		gtk_widget_set_sensitive(GTK_WIDGET(attribToggle_k[crt_n][i]),TRUE);
		attribMask_k[crt_n][0] = attribMask_k[crt_n][0]  | (1<<i) ;	/*  Set bit N */
	} 
	else
	{
		gtk_widget_set_sensitive(GTK_WIDGET(attribToggle_k[crt_n][i]),FALSE);
		attribMask_k[crt_n][0] = attribMask_k[crt_n][0]  & (~(1<<i)) ;	/*  Clear bit N */
	}	
	return TRUE;
}

int AttribToggleToggled_k(GtkWidget *widget,gpointer data)
{
	int i  ;
	int crt_n ;
	gchar *pConvert=0;
	crt_n=getCrtNumFromPointer((int)data);
	i=getDataFromPointer((int)data);
	
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE;

	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)))
	{
		gtk_button_set_label(widget,pConvert = _toUtf8("是"));
		attribMask_k[crt_n][1] = attribMask_k[crt_n][1]  | (1<<i) ;	/*  Set bit N */
		G_SAFE_FREE(pConvert);
	} 
	else
	{
		gtk_button_set_label(widget,pConvert = _toUtf8("否"));
		attribMask_k[crt_n][1] = attribMask_k[crt_n][1]  & (~(1<<i)) ;	/*  Clear bit N */
		G_SAFE_FREE(pConvert);
	}
	return TRUE;
}

int AttribButtonApply_k(GtkWidget *widget,gpointer data)
{
	int crt_n ;
	gchar *pConvert=0;

	crt_n=(int)data;
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE ;

	FilterAlarm_k[crt_n].attrib_mask[0] = attribMask_k[crt_n][0];
	FilterAlarm_k[crt_n].attrib_mask[1] = attribMask_k[crt_n][1];
	FilterAlarm_k[crt_n].is_change = TRUE;
	FilterAlarm_k[crt_n].change_flag = TRUE;
	AttribFilterDlgDelete_k( widget ,   data);  
	AlmDspProc_k( crt_n);
	return TRUE ;
}

char *AttribName_k[]={OIX_STR_FAULT ,OIX_STR_FAIL,OIX_CSTR_ScanEnable,OIX_CSTR_AlarmEnable,OIX_CSTR_AlarmNoAck,
OIX_STR_CTRL_LOCK,OIX_CSTR_NO_UPDATE,OIX_CSTR_ALM_REPEAT,OIX_STR_TAG_STATE};
static GtkWidget*
CreateAttribFilterDlg_k(GtkWidget *parent ,int crt_n)
{
	GtkWidget *windows,*button1,*button2;
	GtkWidget *vbox,*hbox1,*hbox[MAX_ATTRIB_NUM];
	gchar *pConvert=0;
	int i,MenuPointer;
	gboolean isChecked,isToggled;
	gchar *strToggle[2]={"否","是"};
	windows = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_usize (windows, 200, -1);
	gtk_window_set_title(GTK_WINDOW(windows), pConvert=_toUtf8EX(OIX_CSTR_ALM_ATTRIB_FILTER));
	G_SAFE_FREE(pConvert);
	gtk_window_set_transient_for(GTK_WINDOW(windows), GTK_WINDOW(parent)); 
	
	gtk_signal_connect (GTK_OBJECT (windows), "delete-event", 
		GTK_SIGNAL_FUNC (AttribFilterDlgDelete_k),  GINT_TO_POINTER(crt_n)); 
	
	gtk_window_set_destroy_with_parent(GTK_WINDOW(windows),TRUE);
	gtk_window_set_position(GTK_WINDOW(windows),GTK_WIN_POS_CENTER);  
	vbox = gtk_vbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (windows), vbox);
	attribMask_k[crt_n][0] = FilterAlarm_k[crt_n].attrib_mask[0];
	attribMask_k[crt_n][1] = FilterAlarm_k[crt_n].attrib_mask[1];
	for (i=0;i<attrib_num;i++)
	{
		if (FilterAlarm_k[crt_n].attrib_mask[0] & (0X1<<i) ) 
			isChecked = TRUE;  
		else
			isChecked = FALSE;
		if (FilterAlarm_k[crt_n].attrib_mask[1] & (0X1<<i) ) 
			isToggled = TRUE;  
		else
			isToggled = FALSE;
		hbox[i] = gtk_hbox_new(FALSE,0);
		attribCheck_k[crt_n][i] = gtk_check_button_new_with_label(pConvert = _toUtf8EX(AttribName_k[i]));
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(attribCheck_k[crt_n][i]),isChecked);
		MenuPointer=getMenuDataPointer(crt_n,i);
		gtk_signal_connect (GTK_OBJECT (attribCheck_k[crt_n][i]), "toggled",GTK_SIGNAL_FUNC (AttribCheckToggled_k), GINT_TO_POINTER(MenuPointer));
		G_SAFE_FREE(pConvert);
		attribToggle_k[crt_n][i] = gtk_toggle_button_new_with_label(pConvert = _toUtf8EX(strToggle[isToggled]));
		gtk_widget_set_sensitive(GTK_WIDGET(attribToggle_k[crt_n][i]),isChecked);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(attribToggle_k[crt_n][i]),isToggled);
		gtk_signal_connect (GTK_OBJECT (attribToggle_k[crt_n][i]), "toggled",GTK_SIGNAL_FUNC (AttribToggleToggled_k), GINT_TO_POINTER(MenuPointer));
		gtk_widget_set_usize (attribToggle_k[crt_n][i], 50, -1);
		G_SAFE_FREE(pConvert);
		gtk_box_pack_start (GTK_BOX (hbox[i]), attribCheck_k[crt_n][i], FALSE, FALSE, 0);  
		gtk_box_pack_end (GTK_BOX (hbox[i]), attribToggle_k[crt_n][i], FALSE, FALSE, 0); 
		gtk_box_pack_start (GTK_BOX (vbox), hbox[i], FALSE, FALSE, 0);  
	}
	
	hbox1 = gtk_hbox_new (FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox), hbox1, FALSE, FALSE, 0);
	
	button1 = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_APPLY));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start (GTK_BOX (hbox1), button1, TRUE, TRUE, 0);
	gtk_signal_connect (GTK_OBJECT (button1), "clicked",GTK_SIGNAL_FUNC (AttribButtonApply_k), GINT_TO_POINTER(crt_n));
	
	button2 = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_CANCEL));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start (GTK_BOX (hbox1), button2, TRUE, TRUE, 0);
	gtk_signal_connect (GTK_OBJECT (button2), "clicked",GTK_SIGNAL_FUNC (AttribFilterDlgDelete_k), GINT_TO_POINTER(crt_n));

	gtk_widget_show_all(windows);
	return windows;
}

void
on_last_button_press_k  (GtkToggleButton *w,   gpointer         user_data)
{
	GtkWidget *prev , *next ,  *scrollbar  ;
	GtkWidget *toolbar =  ( GtkWidget* )user_data ;
	int crt_n =   0;
	GetCrtNumberOfWgt (GTK_WIDGET(w), &crt_n );
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return  ;
	
	prev = g_object_get_data(G_OBJECT(toolbar),"prevWgt");
	next = g_object_get_data(G_OBJECT(toolbar),"nextWgt");
	scrollbar = g_object_get_data(G_OBJECT(alm_pop_shell_k[crt_n]),"scrollbar");
	isLastPage_k[crt_n]=	gtk_toggle_button_get_active(w);
	if (isLastPage_k[crt_n])
	{
		gtk_widget_set_sensitive(prev, FALSE);
		gtk_widget_set_sensitive(next, FALSE);
		gtk_widget_set_sensitive(scrollbar, FALSE);
		
		if (!alm_timeout_id_k[crt_n])
			alm_timeout_id_k[crt_n]=  gtk_timeout_add ( INTERVAL,(GtkFunction)TimeCall_k, GINT_TO_POINTER(crt_n) ); 
		
	}
	else
	{
		gtk_widget_set_sensitive(prev, TRUE);
		gtk_widget_set_sensitive(next, TRUE);
		gtk_widget_set_sensitive(scrollbar, TRUE);
	}
	
	
	AlmDspProc_k( crt_n );         
	
}

gint AutoFilterToggled_k(GtkWidget *widget,gpointer data)
{
	int i,j;
	gint crt_n = (gint) data ;
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE;
	auto_filter_flag_k[crt_n] = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
	if (FilterAlarm_k[crt_n].stn_id != 0)
	{
		if (auto_filter_flag_k[crt_n])
		{
			if (!autofilter_timeout_id_k[crt_n])
				autofilter_timeout_id_k[crt_n] = gtk_timeout_add ( 1000,(GtkFunction)AutoFilterProc_k, GINT_TO_POINTER (crt_n) );
		}
		else
		{
			if (autofilter_timeout_id_k[crt_n])
			{
				gtk_timeout_remove (autofilter_timeout_id_k[crt_n]);
				autofilter_timeout_id_k[crt_n] = 0;
			}
			for(i=1;i<=keySignalStnDef[FilterAlarm_k[crt_n].stn_id].dev_num;i++)
				for (j=1;j<=(int)keySignalStnDef[FilterAlarm_k[crt_n].stn_id].dev[i].group_num;j++)	
				{
					keySignalStnDef[FilterAlarm_k[crt_n].stn_id].dev[i].group[j].isFilter[crt_n] = FALSE;
					FilterAlarm_k[crt_n].group_mask[i-1] = (FilterAlarm_k[crt_n].group_mask[i-1]) | (1<<(j-1));
				}
			FilterAlarm_k[crt_n].is_change = TRUE;
			AlmDspProc_k(crt_n);		
		}
	}	
	return TRUE;
}
/*
static gboolean
is_separator (GtkTreeModel *model,
			  GtkTreeIter  *iter,
			  gpointer      data)
{
	GtkTreePath *path;
	gboolean result;
	gint iter_num; 
	gint crt_n = (gint) data ;
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE;
	path = gtk_tree_model_get_path (model, iter);
	iter_num = gtk_tree_model_iter_n_children(model,NULL);
	result = gtk_tree_path_get_indices (path)[0] == (iter_num-2);
	gtk_tree_path_free (path);
	
	return result;
}
*/
gint PopupCustomFilter_k(GtkComboBox *widget,gpointer data) 
{
	gint crt_n = (gint) data ;
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE;
	gtk_combo_box_set_active(widget,-1);
	return TRUE;
}
char *Buff_k[]  = {OIX_CSTR_REALTIME_REC ,OIX_CSTR_HIST_REC ,OIX_CSTR_SYSMSG,OIX_CSTR_HISTDB_REC,  "历史报警搜索"};
char *BuffCmd_k[]  = {OIX_CSTR_ALM_ALL_TYPEID, OIX_CSTR_ALM_CMD_TYPEID,OIX_CSTR_ALM_INFO_TYPEID};

static GtkWidget *
CreateAlmToolbar_k (GtkWidget * parent,	int	crt_n)
{
	
	GtkWidget *box,*box1, *last_button ,*print_button ;
	GtkWidget *vbox,*ckbtDisNoAck;	
	GtkWidget *toolbar; 
	GtkWidget *Prev_button,*next_button,*button_exit,*dev_button1,*attrib_button,*ack_button,*range_ack_btn;
	GtkWidget *optionmenu, *menu,*items  , *lbPage,*filterCombo;
	GtkWidget *auto_filter;
	GtkTreeIter  iter;
	GtkCellRenderer *pRenderFilter;
	int	i,j;
	int	station_num; 
	GList *glist = NULL ;	
	char  recordTypeName[RECORD_LONGNAME_SIZE]; 		
	int MenuPointer;//hcl
	gchar *pConvert=0;
	UCHAR	stn_id;
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER ) return ( GtkWidget *)NULL;
	
	memcpy(keySignalStnDef,StnDef,sizeof(StnDef));
	toolbar = gtk_handle_box_new ();
	vbox=gtk_vbox_new(FALSE, 0);
	gtk_container_add (GTK_CONTAINER (toolbar), vbox);
	gtk_widget_show (vbox);
	
	box = gtk_hbox_new (FALSE, 4);
	gtk_box_pack_start (GTK_BOX (vbox), box, FALSE, FALSE, 0);       
	gtk_widget_show (box);
	
	box1 = gtk_hbox_new (FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox), box1, FALSE, FALSE, 0);
	gtk_container_border_width (GTK_CONTAINER (box1), 0);
	gtk_widget_show (box1);
	if (xscale[crt_n] >=1 ) box1 = box ;
	/*  *************  real or his type combobox  ****************** */
	
	
	optionmenu = gtk_option_menu_new();
	gtk_widget_show(optionmenu);
	gtk_box_pack_start (GTK_BOX (box), optionmenu, FALSE, FALSE, 0);
	menu =gtk_menu_new();
	gtk_widget_set_usize (menu, 130, -1);
	/*  OIX_CSTR_REALTIME_REC ,OIX_CSTR_HIST_REC*/
	//for (i = 0 ; i < (crt_n==0?5:3)  ; i++) //hcl 
	i=0; //hcl 重要点一览表只能显示实时记录
	{
		items = gtk_menu_item_new_with_label(pConvert=_toUtf8(_CS_(Buff_k[i])));
		G_SAFE_FREE(pConvert);
		gtk_widget_show( items);
		gtk_container_add(GTK_CONTAINER(menu),items);
		MenuPointer=getMenuDataPointer(crt_n,i);		 		 	 
	//	gtk_signal_connect (GTK_OBJECT (items), "activate",GTK_SIGNAL_FUNC (SetHistAlmBtn_k), GINT_TO_POINTER(MenuPointer));
		
	}   
	gtk_menu_set_active ( GTK_MENU(menu),alm_rec_mode_k[crt_n]  );  
	gtk_box_pack_start (GTK_BOX (box), menu, FALSE, FALSE, 0);
	gtk_option_menu_set_menu(GTK_OPTION_MENU(optionmenu),menu);
	
	
	/*  *************  Power station combobox  ****************** */
	
	
	
	GetStationNum(&station_num);
	
	optionmenu = gtk_option_menu_new();
	gtk_widget_show(optionmenu);
	gtk_box_pack_start (GTK_BOX (box), optionmenu, FALSE, FALSE, 0);
	menu =gtk_menu_new();
	gtk_widget_set_usize (menu, 130, -1);
	
	if(station_num>1)/* added by zyp on 2006.11.1 */
	{
		items = gtk_menu_item_new_with_label(pConvert=_toUtf8EX(OIX_STR_ALL_STATION));	
		G_SAFE_FREE(pConvert);
		gtk_widget_show( items);
		gtk_container_add(GTK_CONTAINER(menu),items);
		MenuPointer=getMenuDataPointer(crt_n,0);
		gtk_signal_connect (GTK_OBJECT (items), "activate",GTK_SIGNAL_FUNC (CreateDbDevB_k), GINT_TO_POINTER(MenuPointer));
		FilterAlarm_k[crt_n].stn_id=0;
	}
	else
		FilterAlarm_k[crt_n].stn_id=StnDef[1].Id; /* added by zyp on 2006.11.1 */
	for(i=1;i<=station_num;i++) 
	{
		if(strcmp(keySignalStnDef[i].CName,"")!=0)
		{			
			if(-1==GetStnIdByIndex(i, &stn_id))
				continue;

			if (isStnInMask((UCHAR)i)==0) continue ; 
			items = gtk_menu_item_new_with_label(pConvert=_toUtf8(keySignalStnDef[i].CName));	 
			G_SAFE_FREE(pConvert);
			gtk_widget_show( items);
			gtk_container_add(GTK_CONTAINER(menu),items);
			
			MenuPointer=getMenuDataPointer(crt_n,stn_id);
			
			gtk_signal_connect (GTK_OBJECT (items), "activate",GTK_SIGNAL_FUNC (CreateDbDevB_k), GINT_TO_POINTER(MenuPointer));
			
			
		}
	}
	g_object_set_data   ( G_OBJECT(parent),"StationOpMenu",optionmenu);
	gtk_option_menu_set_menu(GTK_OPTION_MENU(optionmenu),menu);
	
	/*  *************  dev   combobox  ****************** */ 
	
	dev_button1  = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_ALM_DEV_FILTER));
	G_SAFE_FREE(pConvert);
	g_signal_connect (G_OBJECT (dev_button1), "clicked",G_CALLBACK (dev_button_press_k),  GINT_TO_POINTER(crt_n));
	gtk_widget_show(dev_button1);
	gtk_box_pack_start (GTK_BOX (box), dev_button1, FALSE, FALSE, 0);
	
	/*********command type ***********/       
	
	optionmenu = gtk_option_menu_new();
	gtk_widget_show(optionmenu);
	gtk_box_pack_start (GTK_BOX (box), optionmenu, FALSE, FALSE, 0);
	menu =gtk_menu_new();
	gtk_widget_set_usize (menu, 130, -1);
	/*  OIX_CSTR_all ,OIX_CSTR_CMD  ,OIX_CSTR_INFO */
	for (i = 0 ; i < 3 ; i++)
	{
		items = gtk_menu_item_new_with_label(pConvert=_toUtf8(_CS_(BuffCmd_k[i])));	 
		G_SAFE_FREE(pConvert);
		gtk_widget_show( items);
		gtk_container_add(GTK_CONTAINER(menu),items);
		
		MenuPointer=getMenuDataPointer(crt_n,i);
		
		gtk_signal_connect (GTK_OBJECT(items), "activate",GTK_SIGNAL_FUNC (SetTypeIdMask_k), GINT_TO_POINTER (MenuPointer));
		
	}
	
	gtk_menu_set_active ( GTK_MENU(menu),FilterAlarm_k[crt_n].cmd_type  ); 
	g_object_set_data   ( G_OBJECT(parent),"TypeIdOpMenu",optionmenu);
	gtk_box_pack_start (GTK_BOX (box), menu, FALSE, FALSE, 0);
	gtk_option_menu_set_menu(GTK_OPTION_MENU(optionmenu),menu);
	
	
	/*  *************  datatype   combobox  *******************/ 
	optionmenu = gtk_option_menu_new();
	gtk_widget_show(optionmenu);
	gtk_box_pack_start (GTK_BOX (box), optionmenu, FALSE, FALSE, 0);
	menu =gtk_menu_new();
	gtk_widget_set_usize (menu, 130, -1);
	
	
	DataTypeItem_k[crt_n][ MAX_DATATYPE_NUM] = gtk_menu_item_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_ALM_REC_ALL));
	G_SAFE_FREE(pConvert);
	gtk_widget_show(DataTypeItem_k[crt_n][MAX_DATATYPE_NUM]);
	gtk_container_add(GTK_CONTAINER(menu),DataTypeItem_k[crt_n][MAX_DATATYPE_NUM]);
	MenuPointer=getMenuDataPointer(crt_n,MAX_DATATYPE_NUM);
	gtk_signal_connect (GTK_OBJECT (DataTypeItem_k[crt_n][MAX_DATATYPE_NUM]), "activate",GTK_SIGNAL_FUNC (SetDataTypeMask_k),GINT_TO_POINTER (MenuPointer) );
	
	
	DataTypeItem_k[crt_n][ MAX_DATATYPE_NUM+1] = gtk_menu_item_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_ALM_REC_NON));
	G_SAFE_FREE(pConvert);
	gtk_widget_show(DataTypeItem_k[crt_n][ MAX_DATATYPE_NUM+1]);
	gtk_container_add(GTK_CONTAINER(menu),DataTypeItem_k[crt_n][ MAX_DATATYPE_NUM+1]);
	MenuPointer=getMenuDataPointer(crt_n,MAX_DATATYPE_NUM+1);
	gtk_signal_connect (GTK_OBJECT (DataTypeItem_k[crt_n][ MAX_DATATYPE_NUM+1]), "activate",GTK_SIGNAL_FUNC (SetDataTypeMask_k),GINT_TO_POINTER(MenuPointer) );
	
	
	for(i=0,j=1;i<MAX_DATATYPE_NUM;i++)
	{
		if(GetRecLongnameByType((UCHAR)i, recordTypeName )!=-1)
		{					
			DataTypeItem_k[crt_n][j] = gtk_check_menu_item_new_with_label(pConvert=_toUtf8(recordTypeName));
			G_SAFE_FREE(pConvert);
			if (FilterAlarm_k[crt_n].datatype_mask & 1<<(i-1) )    		
				gtk_check_menu_item_set_active  ( GTK_CHECK_MENU_ITEM( DataTypeItem_k[crt_n][j]),TRUE);
			else
				gtk_check_menu_item_set_active  ( GTK_CHECK_MENU_ITEM(DataTypeItem_k[crt_n][j]),FALSE);
			
			gtk_widget_show(DataTypeItem_k[crt_n][j]);
			gtk_container_add(GTK_CONTAINER(menu),DataTypeItem_k[crt_n][j]);
//MenuPointer=getMenuDataPointer(crt_n,j); //传入j后，筛选错位
			MenuPointer=getMenuDataPointer(crt_n,i);

			gtk_signal_connect (GTK_OBJECT (DataTypeItem_k[crt_n][j]), "toggled",GTK_SIGNAL_FUNC (SetDataTypeMask_k), GINT_TO_POINTER(MenuPointer));	
			j++;
		}
	}
	
	g_object_set_data   ( G_OBJECT(parent),"DataTypeOpMenu",optionmenu);
	
	gtk_option_menu_set_menu(GTK_OPTION_MENU(optionmenu),menu);

	/***********************************attrib combobox****************************************/
	
	attrib_button  = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_ALM_ATTRIB_FILTER));
	G_SAFE_FREE(pConvert);
	g_signal_connect (G_OBJECT (attrib_button), "clicked",G_CALLBACK (attrib_button_press_k),  GINT_TO_POINTER(crt_n));
	gtk_widget_show(attrib_button);
	gtk_box_pack_start (GTK_BOX (box), attrib_button, FALSE, FALSE, 0);
	AttribFilterDlg_k[crt_n]=NULL;

	///////////////////////////报警确认筛选/////////////////////////////////////onlyDisplayNoAck_k
	
	ckbtDisNoAck = gtk_check_button_new_with_label (pConvert=_toUtf8EX(OIX_CSTR_SHOW_ALM_NOLY_AFFIRM));   //OIX_CSTR_SHOW_ALM_NOLY_AFFIRM  "只显示未确认报警"  
	G_SAFE_FREE(pConvert);
	gtk_widget_show (ckbtDisNoAck);
	gtk_box_pack_start (GTK_BOX (box1), ckbtDisNoAck, FALSE, FALSE, 0);
	gtk_signal_connect (GTK_OBJECT (ckbtDisNoAck), "toggled",GTK_SIGNAL_FUNC (onlyDisplayNoAck_k), GINT_TO_POINTER(crt_n));    
	
	
	Prev_button = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_PAGE_UP));
	G_SAFE_FREE(pConvert);
	
	gtk_box_pack_start (GTK_BOX (box1), Prev_button, FALSE, FALSE, 0);     	     
	gtk_signal_connect(GTK_OBJECT(Prev_button), "clicked", GTK_SIGNAL_FUNC(PrevPage_k), GINT_TO_POINTER(crt_n));
	gtk_widget_show (Prev_button);
	
	g_object_set_data(G_OBJECT(toolbar),"prevWgt",Prev_button );
	
	next_button = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_PAGE_DOWN));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start (GTK_BOX (box1), next_button, FALSE, FALSE, 0);     	     
	gtk_signal_connect(GTK_OBJECT(next_button), "clicked", GTK_SIGNAL_FUNC(NextPage_k),   GINT_TO_POINTER(crt_n));
	gtk_widget_show (next_button);
	
	g_object_set_data(G_OBJECT(toolbar),"nextWgt",next_button );
	
	// 	init_button = gtk_button_new_with_label(_toUtf8EX(OIX_STR_FILTER_INIT));
	// 	gtk_box_pack_start (GTK_BOX (box1), init_button, FALSE, FALSE, 0);     	     
	// 	gtk_signal_connect(GTK_OBJECT(init_button), "clicked", GTK_SIGNAL_FUNC(FiterInit_k),   GINT_TO_POINTER(crt_n));
	// 	gtk_widget_show (init_button); 
	
	
	/**************************************************Filter combobox*****************************************************/
	curFilter_k = 0;
	storeFilter_a_k[crt_n]=gtk_list_store_new(num_columns_Filter,G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, 
										   	  G_TYPE_STRING, G_TYPE_STRING,G_TYPE_STRING, G_TYPE_STRING,G_TYPE_STRING,
											  G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING);
	filterCombo = gtk_combo_box_new_with_model(GTK_TREE_MODEL(storeFilter_a_k[crt_n]));
	
	pRenderFilter = gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(filterCombo), pRenderFilter, FALSE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(filterCombo), pRenderFilter, "text", eName_Filter, NULL);
	g_object_unref(storeFilter_a_k[crt_n]);
	
	gtk_box_pack_start (GTK_BOX (box), filterCombo, FALSE, FALSE, 0);
	gtk_widget_set_usize (filterCombo, 130, -1);
	gtk_widget_show(filterCombo);
	gtk_list_store_insert(storeFilter_a_k[crt_n], &iter,0);
	gtk_list_store_set (storeFilter_a_k[crt_n], &iter, eName_Filter, pConvert=_toUtf8EX("OIX_STR_FILTER_INIT"), -1);
	G_SAFE_FREE(pConvert);
//	gtk_combo_box_set_row_separator_func (GTK_COMBO_BOX (filterCombo), is_separator, GINT_TO_POINTER(crt_n), NULL);
//	gtk_list_store_insert(storeFilter_a_k[crt_n], &iter,1);
//	gtk_list_store_set (storeFilter_a_k[crt_n], &iter, eFilterName,NULL, -1);
	gtk_list_store_insert(storeFilter_a_k[crt_n], &iter,1);
	gtk_list_store_set (storeFilter_a_k[crt_n], &iter, eName_Filter, pConvert=_toUtf8EX("OIX_STR_FILTER_SET"), -1);
	G_SAFE_FREE(pConvert);
	filterNum_k = ReadFilterXml(storeFilter_a_k[crt_n],"Predefine");
	gtk_combo_box_set_active(GTK_COMBO_BOX(filterCombo),curFilter_k);
	g_signal_connect (G_OBJECT(filterCombo), "changed", G_CALLBACK(ChgCustomFilter_k), GINT_TO_POINTER(crt_n));
	g_signal_connect(G_OBJECT(filterCombo), "popup", G_CALLBACK(PopupCustomFilter_k), GINT_TO_POINTER(crt_n));
	/********************************************************************************************************************/
	
	ack_button = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_ALM_TAB_ACKN));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start (GTK_BOX (box1), ack_button, FALSE, FALSE, 0);     	     
	gtk_signal_connect(GTK_OBJECT(ack_button), "clicked", GTK_SIGNAL_FUNC(doAlarmAck_k),   GINT_TO_POINTER(crt_n));
	gtk_widget_show (ack_button); 
	
	range_ack_btn = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_ALM_RANGE_ACKN));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start (GTK_BOX (box1),  range_ack_btn, FALSE, FALSE, 0);     	     
	gtk_signal_connect(GTK_OBJECT( range_ack_btn), "clicked", GTK_SIGNAL_FUNC(doAlarmAckRange_k),   GINT_TO_POINTER(crt_n));
	gtk_widget_show ( range_ack_btn); 
	
	
	print_button = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_PRINT));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start (GTK_BOX (box1), print_button, FALSE, FALSE, 0);     	     
	gtk_signal_connect(GTK_OBJECT(print_button), "clicked", GTK_SIGNAL_FUNC(doPrint_k),   GINT_TO_POINTER(crt_n));
	gtk_widget_show (print_button); 
	
	
	
	last_button  = gtk_toggle_button_new_with_label (pConvert=_toUtf8EX(OIX_CSTR_NEW_PAGE));
	G_SAFE_FREE(pConvert);
	gtk_widget_show (last_button);
	gtk_box_pack_start (GTK_BOX (box1), last_button, FALSE, FALSE, 0);     	     
	gtk_signal_connect(GTK_OBJECT(last_button), "toggled", GTK_SIGNAL_FUNC(on_last_button_press_k),  toolbar);
	/*  gtk_toggle_button_toggled(last_button) ;  */
	lbPage = gtk_label_new ("page_k:1/3");
	gtk_box_pack_end (GTK_BOX (box1), lbPage, FALSE, FALSE, 0);
	gtk_widget_show (lbPage);
	g_object_set_data(G_OBJECT(parent),"labelPage",lbPage );
	if (xml_auto_filter)
	{
	auto_filter = gtk_check_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_AUTO_FILTER));
	G_SAFE_FREE(pConvert);
	gtk_widget_show(auto_filter);
	gtk_box_pack_start(GTK_BOX(box1),auto_filter,FALSE,FALSE,0);
	gtk_signal_connect(GTK_OBJECT(auto_filter), "toggled",GTK_SIGNAL_FUNC(AutoFilterToggled_k), GINT_TO_POINTER(crt_n));   
	auto_filter_flag_k[crt_n] = FALSE;
	}
	button_exit = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_EXIT)); 
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start(GTK_BOX(box1), button_exit, FALSE, FALSE, 0);   
	gtk_signal_connect(GTK_OBJECT(button_exit), "clicked",GTK_SIGNAL_FUNC(AlarmFormQuit_k), GINT_TO_POINTER(crt_n));   
	gtk_widget_show(button_exit);
	
	return (toolbar);
}

void	ProgAlarmAckBrdcst_k ( COMMAND_MSG *alm_msg,  int send_num)
{
	
	LAN_HEAD	cmd_head;	
	
	printf ( "OIX: in ProgDoAckCel BROADCAST :::: command_buf will be send out !!!\n" );
	
	cmd_head.length 	= sizeof ( COMMAND_MSG )* send_num;
	printf ( " OIX:  cmd_len =%d\n", cmd_head.length );
	cmd_head.src_id 	= GetHost_Cmd(); 
	cmd_head.dest_id 	= 0;
	cmd_head.dest_stn 	= 0;
	cmd_head.dp_type 	= COMM_RECORD_TIME;
	cmd_head.packet_num 	= 0;
#ifdef DEBUG
	printf("cmd_head::length=%d,dest_id=%d,dest_stn=%d,dp_type=%d,src_id=%d\n",cmd_head.length,cmd_head.dest_id,cmd_head.dest_stn,cmd_head.dp_type,cmd_head.src_id);
	
#endif
	
#ifdef SUN_OS
	Swap2Byte(&cmd_head.length);
#endif
	
	printf("cmd_head::length=%d,dest_id=%d,dest_stn=%d,dp_type=%d,src_id=%d\n",cmd_head.length,cmd_head.dest_id,cmd_head.dest_stn,cmd_head.dp_type,cmd_head.src_id);
	
	lan_out ( (char*)alm_msg, cmd_head,0 );
	printf ( "OIX: after ProgDoAckCelBrdcst: Lan_out \n" );
}

int ReadHistRecFile_k (int crt_n, char *filename)
{
	FILE	*fp;
	int	count1, len ,i, numread ;
	struct stat Buff_k;
	/*if (alm_rec_mode_k[crt_n] == ALM_REC_HISTRY1) */
	{
		fp	 = OpenFileX ( filename );	
		if ( fp == NULL)
		{
			PopupMessageBox(alm_pop_shell_k[crt_n],MSG_OPER_HISTORYFILE_ERROR);
			return  FALSE;
		}
		
		stat(filename,&Buff_k);
		len	= Buff_k.st_size ;
		if (len > REC_ENTRY_SIZE * MAX_FILE_RECORD_NUM )
			len  =  REC_ENTRY_SIZE * MAX_FILE_RECORD_NUM  ;
		if ((len% REC_ENTRY_SIZE) == 0)
			count1 = (len )/ REC_ENTRY_SIZE;
		else
			count1 = 0;
		
		fseek ( fp, 0L, SEEK_SET );
		
		numread =fread ( &hist_rec_addr_k[crt_n].rec, len , 1, fp );
		
		fclose ( fp );
		for (i = 0 ; i <count1 ; i++ )
			SwapRecEntryByte(&hist_rec_addr_k[crt_n].rec[i]) ; 
		
		
		hist_rec_addr_k[crt_n].head.count	= count1;
		
		hist_rec_addr_k[crt_n].head.buf_size = MAX_FILE_RECORD_NUM;
		if (count1 ==  MAX_FILE_RECORD_NUM )
			hist_rec_addr_k[crt_n].head.header_ptr = 0 ; 
		else if (count1<  MAX_FILE_RECORD_NUM )
			hist_rec_addr_k[crt_n].head.header_ptr = count1; 
		else    /*  count1 > MAX_FILE_RECORD_NUM  只取锟斤拷前锟斤拷锟?MAX_FILE_RECORD_NUM -6000锟斤拷锟斤拷录 */
			hist_rec_addr_k[crt_n].head.header_ptr = -1 ; ;
		
		/** InitAlarmFilter(&FilterAlarm_k[crt_n],crt_n);*/
		alm_rec_mode_k[crt_n]	       = ALM_REC_HISTRY1;
		FilterAlarm_k[crt_n].phead   =  &hist_rec_addr_k[crt_n].head ;
		FilterAlarm_k[crt_n].pRec    =  (REC_ENTRY_UNION* )(&hist_rec_addr_k[crt_n].rec);
		FilterAlarm_k[crt_n].is_change = TRUE;
		
	}
	
	AlmDspProc_k( crt_n );
	return TRUE; 
	
}


int ReadSearchHitsRecFile_k (int crt_n,int recCount )
{
	
	FILE	*fp;
	int	count1, len ,i;
	struct stat Buff_k;
	char  infoMessage[256] ; 
	
	sprintf(dest_filename_k, "%s%s", SYS_TMP_PATH, EVENT_SEARCH_TMP_FILE); 
	
	
	fp	 = OpenFileX ( dest_filename_k );	
	if ( fp == NULL)
	{
		PopupMessageBox(alm_pop_shell_k[crt_n],"MSG_OPER_FILE_ERROR!");   //"MSG_OPER_FILE_ERROR"  操作文件错误
		return  FALSE;
	}
	
	stat(dest_filename_k,&Buff_k);
	
	len	= Buff_k.st_size ;
	/*
	if ((len% REC_ENTRY_SIZE) == 0)
	count1 = (len )/ REC_ENTRY_SIZE;
	else
	count1 = 0;*/
	
	count1 =  recCount ; 
	fseek ( fp, 0L, 0 );
	if (count1 > MAX_FILE_RECORD_NUM )
	{  
		sprintf(infoMessage, "搜索结果超过%d条，只能显示最新%d条\n建议细化搜索条件!", 	MAX_FILE_RECORD_NUM,MAX_FILE_RECORD_NUM); 
		PopupMessageBox(alm_pop_shell_k[crt_n],infoMessage);
		fseek(fp, MAX_FILE_RECORD_NUM*REC_ENTRY_SIZE, SEEK_END);
		fread ( &hist_rec_addr_k[crt_n].rec, MAX_FILE_RECORD_NUM*REC_ENTRY_SIZE , 1, fp );
		count1 =  MAX_FILE_RECORD_NUM  ; 						
	}
	else
		fread ( &hist_rec_addr_k[crt_n].rec, len , 1, fp );
	
	fclose ( fp );
	for (i = 0 ; i <count1 ; i++ )
		SwapRecEntryByte(&hist_rec_addr_k[crt_n].rec[i]) ; 
	
	hist_rec_addr_k[crt_n].head.count	= count1;						
	hist_rec_addr_k[crt_n].head.buf_size = MAX_FILE_RECORD_NUM;
	if (count1 ==  MAX_FILE_RECORD_NUM )
		hist_rec_addr_k[crt_n].head.header_ptr = 0 ; 
	else if (count1<  MAX_FILE_RECORD_NUM )
		hist_rec_addr_k[crt_n].head.header_ptr = count1; 
	else
		;   //error ;
	
	/** InitAlarmFilter(&FilterAlarm_k[crt_n],crt_n);*/
	alm_rec_mode_k[crt_n]	= ALM_REC_HISTRY1;
	FilterAlarm_k[crt_n].phead   =  &hist_rec_addr_k[crt_n].head ;
	FilterAlarm_k[crt_n].pRec    =  (REC_ENTRY_UNION* )(&hist_rec_addr_k[crt_n].rec);
	FilterAlarm_k[crt_n].is_change = TRUE;					
	return TRUE; 
	
}

void  SetHistFileAlmCName_k(GtkWidget * widget, gpointer data)



{
	int crt_n ;
	int i = (gint) data ;
	//	GetCrtNumberOfWgt (widget, &crt_n );
	crt_n  = 0 ; 
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return ;
	g_object_set_data(G_OBJECT(widget),"iNameMode", GINT_TO_POINTER (i));  
	eventFilter_k.search_type =  i  ; 
	g_message ("iNameMode = %d \n " , i )  ;
	
}

int HistFileDBAlarmOK_k(GtkWidget *w , gpointer data)
{ 
	int crt_n;
	char *ss1 ;
	//	int iMode ; 
	int ret ; 
	int year,month,day ,minute,second,hour; 
	GtkWidget *startEntry , *endEntry,*cNameEntry ;
	gchar *pConvert=0;

	//	time_t startTm  ,endTm ;
	GtkWidget *window =data ;	
	GetCrtNumberOfWgt (w, &crt_n );
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return TRUE ;
	
	startEntry  = g_object_get_data(G_OBJECT(window),"startWgt");
	endEntry   = g_object_get_data(G_OBJECT(window), "endWgt");
	cNameEntry  = g_object_get_data(G_OBJECT(window), "cNameWgt");
	//	iMode  =  iHisNameMode_k[crt_n] ;
	ss1  = (gchar*)gtk_entry_get_text(GTK_ENTRY(startEntry));
	if (strcmp(ss1,"")!=0)
	{   
		sscanf(ss1,"%4d-%02d-%02d %02d:%02d",&year,&month,&day,&hour,&minute,&second) ;
		eventFilter_k.start_t.year	= 	 year ;
		eventFilter_k.start_t.month	=	 month;
		eventFilter_k.start_t.day		= 	 day;
		eventFilter_k.start_t.hour	= 	 hour ;
		eventFilter_k.start_t.minute	= 	 minute;
		eventFilter_k.start_t.second	= 	 second ;
	}
	ss1  = (gchar*)gtk_entry_get_text(GTK_ENTRY(endEntry));
	if (strcmp(ss1,"")!=0)
	{   
		sscanf(ss1,"%4d-%02d-%02d %02d:%02d",&year,&month,&day,&hour,&minute,&second) ;
		eventFilter_k.end_t.year		= 	 year ;
		eventFilter_k.end_t.month		=	 month;
		eventFilter_k.end_t.day		= 	 day;
		eventFilter_k.end_t.hour		= 	 hour ;
		eventFilter_k.end_t.minute	= 	 minute;
		eventFilter_k.end_t.second	= 	 second ;
	}
	
	ss1  = (gchar*)gtk_entry_get_text(GTK_ENTRY(cNameEntry));
	if (strcmp(ss1,"")!=0)
	{
		strcpy(eventFilter_k.search_str, pConvert=_fromUtf8(ss1));
		G_SAFE_FREE(pConvert);
	}
	eventFilter_k.search_flag =1;   //start search ......
	memcpy(&shm_syscomm_addr->event_search,&eventFilter_k,sizeof(EVENT_SEARCH)) ;
#ifdef SUN_OS 
	ret = system("//home//ems//h9000//bin//event_search.dxe");
#endif
#ifdef WINDOWS_OS 
	ret = system("\\home\\ems\\h9000\\bin\\event_search.exe");
#endif
	while (shm_syscomm_addr->event_search.search_flag) 
	{
		MySleep(100) ; 
	}
	
	HistDBFileAlmDlgDelete_k( w ,   data);  
	alm_rec_mode_k[crt_n]  =  ALM_REC_FILE_SEARCH;
	ReadSearchHitsRecFile_k (  crt_n,shm_syscomm_addr->event_search.event_num) ;
	
	if (alm_pop_shell_k[crt_n])
	{
		gtk_window_set_title(GTK_WINDOW(alm_pop_shell_k[crt_n]), pConvert=_toUtf8("MSG_HIS_FILE_SERP...")); //"MSG_HIS_FILE_SERP" 历史文件搜索结果...
		G_SAFE_FREE(pConvert);
	}
	AlmDspProc_k( crt_n ); 
	return FALSE ;
	
	
	
	
	//Fail:
	PopupMessageBox(alm_pop_shell_k[crt_n] , "MSG_DATA_FORMATE_ERROR!");  //"MSG_DATA_FORMATE_ERROR" 数据格式出错 
	return TRUE ;
	
	
	alm_rec_mode_k[0]   =  ALM_REC_FILE_SEARCH;
	return TRUE ;
}


const char *sCName_k[] = {"通过点号搜索","通过逻辑名搜索","通过长名搜索"};
GtkWidget *CreateHistDBFileAlmDlg_k(GtkWidget *parent ,int crt_n)
{
	gint i ; 
	GtkWidget *optionmenu   , *menu , *items ;
	GtkWidget  *windows1 ,*vbox1  , *frame1 ;
	GtkWidget *table1 ,*label4 ,*label5 ,*entry1, *entry2;
	GtkWidget *entry3, *hbox1 ,*button1  ,*button2 ;
	
	time_t     newtime;
	struct  tm      devtime; 

	gchar *pConvert=0;
	
	char Stimer[30] ,Etimer[30]; 
	newtime = time(NULL);
	devtime = *localtime( &newtime);
	if (eventFilter_k.start_t.year  ==0 && eventFilter_k.start_t.month ==0  && eventFilter_k.start_t.day ==0 
		&&  eventFilter_k.start_t.hour ==0 &&  eventFilter_k.start_t.minute ==0 && eventFilter_k.start_t.second ==0 )
	{
		eventFilter_k.start_t.year		= 	 devtime.tm_year+1900 ;
		eventFilter_k.start_t.month		=	 devtime.tm_mon+1;
		eventFilter_k.start_t.day			= 	 devtime.tm_mday ;
		eventFilter_k.start_t.hour		= 	 devtime.tm_hour ;
		eventFilter_k.start_t.minute		= 	 devtime.tm_min ;
		eventFilter_k.start_t.second		= 	 0 ;
		
	}
	sprintf (Stimer , "%4d-%02d-%02d %02d:%02d" ,eventFilter_k.start_t.year,
		eventFilter_k.start_t.month ,eventFilter_k.start_t.day, eventFilter_k.start_t.hour, eventFilter_k.start_t.minute  ) ;
	
	if (eventFilter_k.end_t.year  ==0 && eventFilter_k.end_t.month ==0  && eventFilter_k.end_t.day ==0 
		&&  eventFilter_k.end_t.hour ==0 &&  eventFilter_k.end_t.minute ==0 && eventFilter_k.end_t.second ==0 )
	{
		eventFilter_k.end_t.year		= 	 devtime.tm_year+1900 ;
		eventFilter_k.end_t.month		=    devtime.tm_mon+1;
		eventFilter_k.end_t.day		= 	 devtime.tm_mday ;
		eventFilter_k.end_t.hour		= 	 devtime.tm_hour ;
		eventFilter_k.end_t.minute	=    devtime.tm_min ;
		eventFilter_k.end_t.second	= 	 0 ;
		
	}
	sprintf (Etimer , "%4d-%02d-%02d %02d:%02d" ,eventFilter_k.end_t.year,
		eventFilter_k.end_t.month ,eventFilter_k.end_t.day, eventFilter_k.end_t.hour, eventFilter_k.end_t.minute  ) ;
	
	
	windows1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(windows1), pConvert=_toUtf8EX(OIX_STR_HIS_FILE_SEARCH_WIN));  //"MSG_HIS_FILE_SEARCH_WIN" 历史文件搜索窗口
	G_SAFE_FREE(pConvert);
	gtk_window_set_transient_for(GTK_WINDOW(windows1), GTK_WINDOW(parent)); 
	gtk_window_set_destroy_with_parent(GTK_WINDOW(windows1),TRUE);
	
	gtk_signal_connect (GTK_OBJECT (windows1), "delete_event", 
		GTK_SIGNAL_FUNC (HistDBFileAlmDlgDelete_k), GINT_TO_POINTER (crt_n)); 
	
	gtk_widget_show (windows1);
	vbox1 = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox1);
	gtk_container_add (GTK_CONTAINER (windows1), vbox1);
	
	
	
	frame1 = gtk_frame_new (NULL);
	gtk_widget_show (frame1); 
	g_object_set_data(G_OBJECT(windows1),"frameWgt",frame1);
	gtk_box_pack_start (GTK_BOX (vbox1), frame1, FALSE, FALSE, 0);
	
	table1 = gtk_table_new (2, 2, FALSE);
	gtk_widget_show (table1);
	gtk_container_add (GTK_CONTAINER (frame1), table1);
	
	
	
	label4 = gtk_label_new (pConvert=_toUtf8EX(OIX_STR_STATR_DATE));
	G_SAFE_FREE(pConvert);
	gtk_widget_show (label4);
	gtk_table_attach (GTK_TABLE (table1), label4, 0, 1, 0, 1,
		(GtkAttachOptions) (GTK_FILL),
		(GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label4), GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label4), 0, 0.5);
	
	
	
	label5 = gtk_label_new (pConvert=_toUtf8EX(OIX_STR_END_DATE));
	G_SAFE_FREE(pConvert);
	gtk_widget_show (label5);
	gtk_table_attach (GTK_TABLE (table1), label5, 0, 1, 1, 2,
		(GtkAttachOptions) (GTK_FILL),
		(GtkAttachOptions) (0), 0, 0);
	gtk_label_set_justify (GTK_LABEL (label5), GTK_JUSTIFY_LEFT);
	gtk_misc_set_alignment (GTK_MISC (label5), 0, 0.5);
	
	
	
	entry1 = gtk_entry_new ();
	gtk_widget_show (entry1);
	gtk_table_attach (GTK_TABLE (table1), entry1, 1, 2, 0, 1,
		(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
		(GtkAttachOptions) (0), 0, 0);
	g_object_set_data(G_OBJECT(windows1),"startWgt",entry1);
	
	entry2 = gtk_entry_new ();
	gtk_widget_show (entry2);
	gtk_table_attach (GTK_TABLE (table1), entry2, 1, 2, 1, 2,
		(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
		(GtkAttachOptions) (0), 0, 0);
	
	g_object_set_data(G_OBJECT(windows1),"endWgt",entry2);
	
	/***********************************************************************************/
	optionmenu = gtk_option_menu_new();
	gtk_widget_show(optionmenu);
	gtk_table_attach (GTK_TABLE (table1), optionmenu, 0, 1, 2, 3,
		(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
		(GtkAttachOptions) (0), 0, 0);
	menu =gtk_menu_new();
	gtk_widget_set_usize (menu, 130, -1);
	/*  OIX_CSTR_REALTIME_REC ,OIX_CSTR_HIST_REC*/
	if (eventFilter_k.search_type==0)
		eventFilter_k.search_type =1  ; 
	for (i = 1 ; i < 3  ; i++)
	{
		items = gtk_menu_item_new_with_label(pConvert=_toUtf8((char*)sCName_k[i]));	 
		G_SAFE_FREE(pConvert);
		gtk_widget_show( items);
		gtk_container_add(GTK_CONTAINER(menu),items);
		gtk_signal_connect (GTK_OBJECT (items), "activate",GTK_SIGNAL_FUNC (SetHistFileAlmCName_k), GINT_TO_POINTER(i));
	}   
	
	gtk_menu_set_active ( GTK_MENU(menu), eventFilter_k.search_type-1  ); 
	gtk_option_menu_set_menu(GTK_OPTION_MENU(optionmenu),menu);
	
	
	/***********************************************************************************/
	entry3 = gtk_entry_new ();
	gtk_widget_show (entry3);
	gtk_table_attach (GTK_TABLE (table1), entry3, 1, 2, 2, 3,
		(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
		(GtkAttachOptions) (0), 0, 0);
	
	
	if(strlen(eventFilter_k.search_str )>0)
	{
		gtk_entry_set_text(GTK_ENTRY(entry3), pConvert=_toUtf8(eventFilter_k.search_str));
		G_SAFE_FREE(pConvert);
	}
	
	g_object_set_data(G_OBJECT(windows1),"cNameWgt",entry3);
	
	
	gtk_entry_set_text(GTK_ENTRY(entry1), pConvert=_toUtf8(Stimer));
	G_SAFE_FREE(pConvert);
	gtk_entry_set_text(GTK_ENTRY(entry2), pConvert=_toUtf8(Etimer));
	G_SAFE_FREE(pConvert);
	
	
	
	
	hbox1 = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox1);
	gtk_box_pack_start (GTK_BOX (vbox1), hbox1, FALSE, FALSE, 0);
	
	button1 = gtk_button_new_with_label(pConvert=_toUtf8(_CS_(OIX_CSTR_CANCEL)));   //gtk_button_new_from_stock ("gtk-cancel");
	G_SAFE_FREE(pConvert);
	gtk_widget_show (button1);
	gtk_box_pack_end (GTK_BOX (hbox1), button1, FALSE, FALSE, 0);
	
	button2 = gtk_button_new_with_label(pConvert=_toUtf8(_CS_(OIX_CSTR_OK)));
	G_SAFE_FREE(pConvert);
	
	gtk_widget_show (button2);
	gtk_box_pack_end (GTK_BOX (hbox1), button2, FALSE, FALSE, 0);
	
	gtk_signal_connect (GTK_OBJECT (button1), "clicked",GTK_SIGNAL_FUNC (HistDBFileAlmDlgDelete_k), windows1);  
	gtk_signal_connect (GTK_OBJECT (button2), "clicked",GTK_SIGNAL_FUNC (HistFileDBAlarmOK_k), windows1);
	
	gtk_window_set_position(GTK_WINDOW(windows1),GTK_WIN_POS_CENTER);  
	//	iHisNameMode_k[crt_n] =  1 ;  
	return  windows1 ;
	
}

int HistDBFileAlmDlgDelete_k (GtkWidget *w , gpointer data)
{
	int  crt_n = (gint)data;
	GetCrtNumberOfWgt (w, &crt_n );
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE ;
	gtk_widget_destroy(HistFileDBDlg_k[crt_n] );
	HistFileDBDlg_k[crt_n]  = (GtkWidget*)NULL ; 
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

GtkWidget *PtEventDlg_k[MAX_CRT_NUMBER];
enum
{
	
	
	COL_REC_NUMBER=0,
		COL_TIME,
		COL_LOGICNAME,
		COL_MSG_NAME,
		COL_RT_STATUS,
		COL_USER_AT_HOST,
		COL_COUNT
};

GtkTreeModel* mx_treeview_create_model_k()
{
    GtkListStore* store = gtk_list_store_new(COL_COUNT,
		G_TYPE_STRING,
		G_TYPE_STRING,
		G_TYPE_STRING,
		G_TYPE_STRING,
		G_TYPE_STRING,
		G_TYPE_STRING);
    return GTK_TREE_MODEL(store);
	
}

void mx_treeview_set_header_k(GtkWidget* treeview)
{
//	char str[500];
	gchar *pConvert=0;
    GtkCellRenderer* renderer = gtk_cell_renderer_text_new();
    
// 	sprintf(str,"%s      %s                 %s                 %s                   %s       %s",_toUtf8EX(OIX_STR_REC_NUMBER),
// 									_toUtf8EX(OIX_STR_TIME),
// 									_toUtf8EX(OIX_STR_REC_LOGNAME),
// 									_toUtf8EX(OIX_STR_MSG_NAME),
// 									_toUtf8EX(OIX_STR_RT_STATUS),
// 									_toUtf8EX(OIX_CSTR_USER_AT_HOST)
// 									);
// 
// 	gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview), -1,
// 		str, renderer, "text", COL_REC_NUMBER, NULL);

	
	gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview), -1,
		pConvert=_toUtf8EX(OIX_STR_REC_NUMBER), renderer, "text", COL_REC_NUMBER, NULL);
	G_SAFE_FREE(pConvert);

	gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview), -1,
		pConvert=_toUtf8EX(OIX_STR_TIME), renderer, "text", COL_TIME, NULL);
	G_SAFE_FREE(pConvert);

    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview), -1,
		pConvert=_toUtf8EX(OIX_STR_REC_LOGNAME), renderer, "text", COL_LOGICNAME, NULL);
	G_SAFE_FREE(pConvert);

	gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview), -1,
		pConvert=_toUtf8EX(OIX_STR_MSG_NAME), renderer, "text", COL_MSG_NAME, NULL);
	G_SAFE_FREE(pConvert);

    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview), -1,
		pConvert=_toUtf8EX(OIX_STR_RT_STATUS), renderer, "text", COL_RT_STATUS, NULL);
	G_SAFE_FREE(pConvert);

    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(treeview), -1,
		pConvert=_toUtf8EX(OIX_CSTR_USER_AT_HOST), renderer, "text", COL_USER_AT_HOST, NULL);
	G_SAFE_FREE(pConvert);

	g_object_set (G_OBJECT(renderer),
	//	"foreground", "green", //设置前景色
	//	"background", "black", //设置背景色
		"font", "sans 11", //设置字体
                  NULL);
}

void mx_treeview_insert_data_k(GtkTreeModel* store, gchar* num, gchar* time, gchar* logicname,gchar* msg,gchar* status,gchar* user)
{
    GtkTreeIter iter;
    gtk_list_store_append(GTK_LIST_STORE(store), &iter);
    gtk_list_store_set(GTK_LIST_STORE(store), &iter,
		COL_REC_NUMBER, num,
		COL_TIME, time,
		COL_LOGICNAME,logicname,
        COL_MSG_NAME, msg,
		COL_RT_STATUS,status,
		COL_USER_AT_HOST,user,
                       -1);
}
gint ptEventDlgQuit_k(GtkWidget *widget,   gpointer data)
{
	int crt_n = (gint) data ;
	/*  GetCrtNumberOfWgt(widget,&crt_n);  */
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE; 
	
	gtk_widget_destroy(GTK_WIDGET(PtEventDlg_k[crt_n])); 
	PtEventDlg_k[crt_n] = (GtkWidget *)NULL ;
	
	return TRUE;
	
}

GtkWidget * createptEventDlg_k(int crt_n,POINTER pt)
{
    GtkWidget *window, *vbox, *label;
    GtkWidget *treeview;
	GtkWidget *scrolledwindow;
    GtkTreeModel *store;
	gchar *pConvert=0;
	
	//    gtk_init(&argc, &argv);
    /* 窗体 */
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), pConvert=_toUtf8EX(OIX_CSTR_PTEVENT_TAB));
	G_SAFE_FREE(pConvert);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(main_w[crt_n]));
	
	
    /* vbox */
    vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);
	
    /* img */
//    img = gtk_image_new_from_file("lanage-top.png");
//    gtk_widget_set_size_request(img, WIN_WIDTH, 40);
	// gtk_box_pack_start(GTK_BOX(vbox), img,TRUE, TRUE, 0);
	
    /* label */
    label = gtk_label_new("");
    gtk_widget_set_size_request(label, LOCAL_WIDTH/2, 20);
    gtk_box_pack_end(GTK_BOX(vbox), label, FALSE, FALSE, 0);
	
	scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_show (scrolledwindow);
	gtk_container_add (GTK_CONTAINER (window), scrolledwindow);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow), GTK_SHADOW_IN);

    /* treeview */
    treeview = gtk_tree_view_new();
    gtk_widget_set_size_request(treeview, LOCAL_WIDTH/2, ALM_CANVAS_HEIGHT);

	gtk_container_add (GTK_CONTAINER (scrolledwindow), treeview);

    gtk_box_pack_start(GTK_BOX(vbox), scrolledwindow, TRUE, TRUE, 0);
	gtk_tree_view_set_grid_lines (GTK_TREE_VIEW(treeview), GTK_TREE_VIEW_GRID_LINES_NONE);

		/* 关联 model */
		store = mx_treeview_create_model_k();
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), store);
	
    /* 设置表头 */
    mx_treeview_set_header_k(treeview);
	
    /* 插入数据 */
	gtk_object_set_data(GTK_OBJECT(window),"store",store);
	//  for(i = 0; i < TOP; i++)
	//      mx_treeview_insert_data_k(store, datas[i].lanage, datas[i].level, datas[i].dir);
// 	loadptEvent_k( store,crt_n,pt);	

		//loadptEvent_k( store,crt_n,pt);
	
    g_object_unref(store);
	
    /* 事件 */
    //g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
	gtk_signal_connect(GTK_OBJECT(window),
		"destroy",
		GTK_SIGNAL_FUNC(ptEventDlgQuit_k),
		GINT_TO_POINTER (crt_n));
    /* 显示 */
   gtk_widget_show_all(window);

    return window;
}

int loadptEvent_k( GtkTreeModel *store,int crt_n,POINTER pt)
{
//	int header_ptr;
	int i;
//	REC_HEAD *phead;
	ALM_STR			alarm_str ;
	char record[64];
//	char state_str[200];
	char hostname[64];
	char str[500];

	char *pConvert=0, *pConvert1=0, *pConvert2=0, *pConvert3=0;

	int index=0;

	int start,count;
	count=shm_rec_addr->head.count;
	
		if(shm_rec_addr->head.header_ptr-count>0)
			start=shm_rec_addr->head.header_ptr-count;
	else
		start=shm_rec_addr->head.header_ptr-count+shm_rec_addr->head.buf_size;

	gtk_list_store_clear(GTK_LIST_STORE(store)); 
	for (i=0;i<count;i++)
	{
		if ((shm_rec_addr->rec[start].ind_rec.stn_id==pt.stn_id)
			&&(shm_rec_addr->rec[start].ind_rec.dev_id==pt.dev_id)  
			&&(shm_rec_addr->rec[start].ind_rec.data_type==pt.data_type)
			&&(shm_rec_addr->rec[start].ind_rec.pt_id==pt.pt_id))
		{
			GetAlmStrings(&shm_rec_addr->rec[start], &alarm_str , NULL , NULL );

			sprintf(record , "%s: ",  alarm_str.sn_str);
			strcat(record , alarm_str.repeatAlm);

			if (strcmp(alarm_str.usrname,"")!=0)
				sprintf(hostname , "%s@%s", alarm_str.usrname, alarm_str.hostname);
			else
			sprintf(hostname , "%s",   alarm_str.hostname);

			sprintf(str,"%s %s %s %s %s %s",record,alarm_str.time_str, pConvert=_toUtf8EX(alarm_str.longname),
				pConvert1=_toUtf8EX(alarm_str.state_str), pConvert2=_toUtf8EX(alarm_str.cmd_result),
				pConvert3=_toUtf8EX(hostname ));
			G_SAFE_FREE(pConvert);
			G_SAFE_FREE(pConvert1);
			G_SAFE_FREE(pConvert2);
			G_SAFE_FREE(pConvert3);
			mx_treeview_insert_data_k(store,record,alarm_str.time_str, pConvert=_toUtf8EX(alarm_str.longname),
				pConvert1=_toUtf8EX(alarm_str.state_str), pConvert2=_toUtf8EX(alarm_str.cmd_result), 
				pConvert3=_toUtf8EX(hostname ));
			G_SAFE_FREE(pConvert);
			G_SAFE_FREE(pConvert1);
			G_SAFE_FREE(pConvert2);
			G_SAFE_FREE(pConvert3);
		//	mx_treeview_insert_data_k(store,str,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
			
			index++;
			
		}
		start=(start+1)%shm_rec_addr->head.buf_size;
		
	}

	for (i=0;i<20;i++)
	{
		
		mx_treeview_insert_data_k(store,NULL,NULL,NULL,NULL,
				NULL,NULL);
	}
	return TRUE;
}
int ptEventProg_k(GtkWidget *parent,int crt_n,POINTER pt)
{
	
	GtkTreeModel *store;
	if (PtEventDlg_k[crt_n]==(GtkWidget*)NULL)
	{
		PtEventDlg_k[crt_n]=createptEventDlg_k(crt_n,pt);
	}else
	{
		gtk_window_present(GTK_WINDOW(PtEventDlg_k[crt_n]));
	}

	store=gtk_object_get_data(GTK_OBJECT(PtEventDlg_k[crt_n]), "store");
	
	
	loadptEvent_k( store,crt_n,pt);
	return TRUE;
}
