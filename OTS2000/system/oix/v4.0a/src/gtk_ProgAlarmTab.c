/*******************************************
2005.11 created by zyp 
2006.03 updated by chi hailong  by gtk
2007.06.22 : have been SYNC  by chichi;
LAST UPATE : 2007.12.03 by chichi ; 
******************************************/


#include <stdio.h>


#include <string.h>
#include <gtk/gtk.h>
#include <string.h>

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





PAGE_INFO		page[MAX_CRT_NUMBER];

int last_crt=0;

extern gint num_screen;
extern char OIX_EVENT_DSP_LOGICNAME ; 
extern char OIX_EVENT_ALIAN_MOD ; 
HISTORY_RECORD          hist_rec_addr[MAX_CRT_NUMBER] ;
static GMutex *alm_mutex[MAX_CRT_NUMBER] = {NULL,NULL,NULL};

SHM_HDB                 *shm_hdb_alm;
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
int HistDBAlmDlgDelete (GtkWidget *w , gpointer data);
int HistDBFileAlmDlgDelete (GtkWidget *w , gpointer data);
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
GtkWidget *CreateHistDBFileAlmDlg(GtkWidget *parent ,int crt_n);
extern int	alm_rec_fault(FILTER_T filter, INDEX *index);
extern int    gdk_rgb(int red  ,int green, int  blue , GdkColor *clr  );
extern void  fault_alarm_expose(GtkWidget *w ,  GdkEventExpose *event, gpointer data)  ;
extern int DrawFaultAlarm(int crt_n ,GdkDrawable *drawable,GdkGC  *gc  );
extern void GetCrtNumberOfWgt ( GtkWidget  *wgt, int     *crt_n );
extern gint  AlmDspProc( crt_n );
extern gint SetDataTypeMask(GtkWidget  *widget, gpointer data);
extern int OpenDspFile (int crt_n, char	*filename);
extern int	alm_rec_filter(FILTER_T filter, INDEX *index);
extern FILE    *OpenFileX (char    *full_name);
extern int    GetHost_Cmd();
extern windk_text_height    (GdkFont  *font,  const gchar  *text,  gint txt_length ,char *fontdesc);
extern GtkWidget* PopupDetailInfoBox(GtkWidget *parent,int crt_n,POINTER pt);
extern gint doAlarmAckEx(GtkWidget *widget, int crt_n ,  int startI ,int  endI  ,BOOL isRangeAck );
extern void PrintList(GtkWidget*parent,int crt_n,int prt_obj_type ,char *output_file);
extern void PopFileSelectionDlg(GtkWidget *parent ,int crt_n,char *title,char *dir);
extern void	PopupMessageBox (GtkWidget *parent,char msg[] );
extern void popupErrorMsgBox(GtkWidget* pParent, gchar* strMsg);
extern char *_fromUtf8(char *c);
extern char *_toUtf8EX(char *c);
extern char *_toUtf8(char *c);
extern char *TimeToString(time_t tmTime);
extern time_t StringToTime(char * szTime);
extern BOOL  isStnInMask (UCHAR stn_id) ;  
extern BOOL IsFileEndStr(char *str);
void    windk_draw_text(GdkDrawable * drawable ,GdkFont    *font,GdkGC  *gc, int x,int y,const char      *text,int  strlength,int crt_n) ;
void    windk_draw_text_EX(GdkDrawable * drawable ,char  *fontdescr ,GdkGC  *gc, int x,int y,const char      *text,int  strlength,int crt_n ) ;
extern          SHM_RECORD	*shm_rec_addr;
extern          FAULT_RECORD_TABLE *shm_fault_addr;
extern			INFO_RECORD		*shm_info_rec_addr;   //added by chi 070911
extern gint num_screen;
extern			SYS_COMM_TABLE  *shm_syscomm_addr;
void            SwapRecEntryByte(REC_ENTRY_UNION *rec_entry);

void 	ProgAlarmTab (GtkWidget *parent,	int crt_n  ,int TabMode  );
void            InitAlarmFilter(FILTER_T *pFilter,int crt_n);
static void *FetchDataFromSHM(void *arg);
static          GtkWidget* CreateHistDBAlmDlg(GtkWidget *parent ,int crt_n);
void  cb_scroll_page(GtkAdjustment *adj , int crt_n);
int DelFilterXml(gchar *filterName,const char *nodeName);

GtkAdjustment  *adj1[MAX_CRT_NUMBER] ;
char   dest_filename[256] ; //历史搜索用临时文件 search_rec_file.rec
EVENT_SEARCH  eventFilter ;
/*thread_t 	almtid[NUM_THREADS]; */
extern  unsigned int  stnMask4alarm ; 
int curDev_a;

gint event_search_timer_id[MAX_CRT_NUMBER]={0, 0, 0};


static GtkWidget *CreateAlmToolbar (GtkWidget * ,int);
static GtkWidget *CreateAlmToolbar1 (GtkWidget * parent,GtkWidget *vbox1,	int	crt_n);
void	ProgAlarmAckBrdcst ( COMMAND_MSG *alm_msg,  int send_num);
void	DrawAlmTabHead(GdkDrawable *drawable ,GdkGC *gc ,int crt_n );
static  GtkWidget* CreateProgAlarmTab (GtkWidget *parent,	int crt_n   );
static  GtkWidget* CreateDevFilterDlg(GtkWidget *parent ,int crt_n);
static  GtkWidget* CreateAttribFilterDlg(GtkWidget *parent ,int crt_n);
static  GtkWidget* CreateCustomFilterDlg(GtkWidget *parent ,int crt_n);
int     ReadHistRecFile (int crt_n, char *filename);
int getMenuDataPointer(int crtn,int data);
int getCrtNumFromPointer(int data);
int getDataFromPointer(int data);
int SaveFilterXml(gchar *filterName,FILTER_T *pFilter,const char *nodeName);
gint FiterInit(GtkWidget*w , gpointer data);
int ReadFilterXml(GtkListStore *listStore,const char *nodeName);

static  BOOL	alm_exit_flag[MAX_CRT_NUMBER] = {FALSE,FALSE,FALSE};
INDEX   	index_tmp[MAX_CRT_NUMBER],index_tmp1[MAX_CRT_NUMBER];
INDEX   	index_fault[MAX_CRT_NUMBER];
SEL_PT   	sel_pt[MAX_CRT_NUMBER];
int			iHisNameMode[MAX_CRT_NUMBER] ;

char *AlarmHeadTitle[] = {OIX_STR_REC_NUMBER   , OIX_STR_TIME  ,OIX_STR_REC_LOGNAME, OIX_STR_MSG_NAME  ,OIX_STR_RT_STATUS,"",OIX_STR_USER};
int dsp_ID=0;

UINT    	devMask[MAX_CRT_NUMBER] = {0xffffffff,0xffffffff,0xffffffff};
UINT		groupMask[MAX_CRT_NUMBER][MAX_GROUP_MASK_NUM] = {0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,0xffffffff,
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
UINT		attribMask[MAX_CRT_NUMBER][2] = {0,0,0,0,0,0};
int     	alm_rec_cnt[MAX_CRT_NUMBER];
gint		isLastPage[MAX_CRT_NUMBER]={0,0,0};
FILTER_T 	FilterAlarm[MAX_CRT_NUMBER],FilterAlarm1[MAX_CRT_NUMBER];
FILTER_T 	FilterAlarmFault[MAX_CRT_NUMBER];
GtkWidget       *filterDlg[MAX_CRT_NUMBER] ={ NULL,NULL,NULL};
GtkWidget		*AttribFilterDlg[MAX_CRT_NUMBER] = {NULL,NULL,NULL};
GtkWidget       *HistDBDlg[MAX_CRT_NUMBER] ={ NULL,NULL,NULL};
GtkWidget       *HistFileDBDlg[MAX_CRT_NUMBER] ={ NULL,NULL,NULL};
GtkWidget       *SetFilterDlg[MAX_CRT_NUMBER] ={ NULL,NULL,NULL};

extern 		int lcu_num;
extern 		int lcu_status[];
GtkWidget 	*alm_pop_shell[MAX_CRT_NUMBER]={NULL,NULL,NULL};
GtkWidget 	*alm_canvas[MAX_CRT_NUMBER]={NULL,NULL,NULL};
GdkGC  		*alm_draw_gc[MAX_CRT_NUMBER]={NULL,NULL,NULL};
GtkWidget 	*item[MAX_CRT_NUMBER][GROUP_ENTRY_NUMBER];
GtkWidget	*DataTypeItem[MAX_CRT_NUMBER][MAX_DATATYPE_NUM + 2];
GtkWidget	*attribCheck[MAX_CRT_NUMBER][MAX_ATTRIB_NUM],*attribToggle[MAX_CRT_NUMBER][MAX_ATTRIB_NUM];
GtkWidget	*DevItem[MAX_CRT_NUMBER][SHOW_NONE];
GtkListStore *storeFilter_a[MAX_CRT_NUMBER],*storeDev_a[MAX_CRT_NUMBER],*storeGroup_a[MAX_CRT_NUMBER];
int	filterNum,curFilter;
gboolean	auto_filter_flag[MAX_CRT_NUMBER];
GdkPixmap 	*AlmOffDrawable[MAX_CRT_NUMBER]= {NULL,NULL,NULL};
gint 	alm_timeout_id[MAX_CRT_NUMBER] ={0,0,0} ;
gint 	fault_alm_timeout_id[MAX_CRT_NUMBER] ={0,0,0} ;
STNDEF	alarmStnDef[STATION_ENTRY_NUMBER];
gint 	autofilter_timeout_id[MAX_CRT_NUMBER] ={0,0,0} ;

typedef struct
{
	int  crt_n ; 
	GtkWidget *canvas ;
	GdkPixmap *pixmap , *back_pixmap;
	GdkGC  *gc ; 
} XHDC ;    /* draw devcie context !!! */

void  init_alm_mutex(int crt_n )
{
	g_assert (alm_mutex[crt_n] == NULL);
	
	alm_mutex[crt_n]  = g_mutex_new ();
	
}

void  free_alm_mutex(int crt_n )
{
	g_assert (alm_mutex[crt_n] != NULL);
	
	g_mutex_free (	alm_mutex[crt_n] );
	
	alm_mutex[crt_n] = NULL ; 
}

int   alarm_scroll_event (GtkWidget*widget,   GdkEventScroll *event, gpointer  data)  
{   int crt_n  = (int) data; 

if (event->direction == GDK_SCROLL_DOWN)
{
	// down code
	adj1[crt_n]->value ++ ; 
	//printf ("---------scroll++++++++++++++++++++1\n");
}
else if (event->direction == GDK_SCROLL_UP)
{
	// up code
	adj1[crt_n]->value -- ; 
	//printf ("---------scroll---------------------1\n");
}

cb_scroll_page(adj1[crt_n] ,   crt_n);

return FALSE;


}

/****************  Timer  call  !!!      ************/
gint  FaultAlmDspProc(gpointer  data)
{
	XHDC *dc =  (XHDC *)data ;
	GdkColor clr ;
	
	
	gdk_rgb(51,51,51,&clr);
	gdk_gc_set_foreground(dc->gc, &clr);
	
	gdk_draw_rectangle (dc->pixmap,dc->gc,TRUE, 0,0,LOCAL_WIDTH , 1600);	   
	/*printf("index_tmp1[%d].header=%d,shm_rec_addr->head.header_ptr=%d\n",dc->crt_n,index_tmp1[dc->crt_n].header,shm_rec_addr->head.header_ptr);
	*/	   
	DrawFaultAlarm(dc->crt_n ,dc->pixmap,dc->gc  );
	gdk_window_copy_area (dc->canvas->window,dc->gc,0,0,dc->pixmap ,0,0,LOCAL_WIDTH, 1600);
	return TRUE;
	
}


gint fault_alarm_click( GtkWidget *w ,GdkEvent *event  , gpointer data)
{
	gint  crt_n = (gint)  data ;
	
	if (event->type == GDK_2BUTTON_PRESS) {  /*  GDK_2BUTTON_PRESS ---double clicked !!!! */
		GdkEventButton *bevent = (GdkEventButton *) event; 
		if (bevent->button == 1)  
		{	
			ProgAlarmTab (main_w[crt_n],crt_n ,FALSE);
			return TRUE ; 
		}
	}
	return TRUE;
}

gint onlyDisplayNoAck( GtkWidget *w , gpointer data)
{
	
	
	
	
	
	
	
	gint crt_n = (gint )data;
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER ) return FALSE;
	if (gtk_toggle_button_get_active  (GTK_TOGGLE_BUTTON(w)))
		FilterAlarm[crt_n].isNoAck = 1   ;
	else
		FilterAlarm[crt_n].isNoAck = 0   ; 
	
	FilterAlarm[crt_n].is_change  = 1 ; 
	
	return AlmDspProc( crt_n );
}

void SetTypeIdMask (GtkWidget * widget, gpointer data)
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
	FilterAlarm[crt_n].cmd_type  = i ; 
	
	FilterAlarm[crt_n].is_change = TRUE; 
	FilterAlarm[crt_n].change_flag =TRUE;
	AlmDspProc( crt_n );
	
}
void  InitToolBarButton(GtkWidget*parent,int crt_n)
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
	SetDataTypeMask(DataTypeItem[crt_n][MAX_DATATYPE_NUM], GINT_TO_POINTER ( MenuPointer)) ;  /* set all data type ; */
	
}


void  fault_alarm_expose(GtkWidget *w ,  GdkEventExpose *event, gpointer data)  
{ 
	XHDC  dc ;
	int crt_n = (gint) data ;
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER ) return ;
	dc.crt_n = (gint ) data;
	dc.canvas = w ;
	dc.pixmap = pixmap2[dc.crt_n];
	dc.gc = info[dc.crt_n].gc2 ; 
	FaultAlmDspProc( &dc);
}



int DrawFaultAlarm(int crt_n ,GdkDrawable *drawable,GdkGC  *gc  )
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
	/*char buff[20] ;*/
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
	InitAlarmFilter( &FilterAlarmFault[crt_n],crt_n);   
	FilterAlarmFault[crt_n].pRec   = shm_rec_addr->rec;   
	FilterAlarmFault[crt_n].phead  = &shm_rec_addr->head ; 
	index_fault[crt_n].buff_size = 10 ;
	
	
	
	if (index_fault[crt_n].header != shm_rec_addr->head.header_ptr )  
	{
		fault_count =  index_fault[crt_n].count ;
		index_fault[crt_n].count = 0 ;
		
		index_fault[crt_n].header = shm_rec_addr->head.header_ptr;          
		
		alm_rec_fault(FilterAlarmFault[crt_n], &index_fault[crt_n]);              
		
		if ( index_fault[crt_n].count>fault_count && crt_n ==1)
		{
			gdk_beep();
			
		}
	}
	
	for (i = 0 , j= 0 ; i < index_fault[crt_n].count;j++, i++)
	{		 
		GetAlmStrings(&shm_rec_addr->rec[index_fault[crt_n].index_pt[i].pt], &alarm_str , &blink , &color );
		/* if((index_fault[crt_n].index_pt[i]==last_fault_no)&&(crt_n==0))
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
						last_crt =CRT_2;       
						printf("dsp_time_out=%d,last_crt=%d\n",dsp_time_out,last_crt);
					}
					else
					{
						last_crt ++;
						if(last_crt>num_screen-1)
							last_crt = CRT_2;
					}                        
					
					if(last_crt>=num_screen)
						last_crt= num_screen-1;
					
					{        
						OpenDspFile(last_crt,dms_cmm.fixed.dsp_filename);  
						dsp_time_out =0;                      
						printf("dsp opened on crt %d,file=%s\n",last_crt,dms_cmm.fixed.dsp_filename);
					}
				}
				
		}
	}
	/* end by zyp */
	return TRUE;
	
}

GdkRectangle   AlarmCanasRECT[MAX_CRT_NUMBER];
void  alarm_canvas_allocate(GtkWidget *widget,GtkAllocation *a ,gpointer data) 
{
	int crt_n = (gint) data ; 
	AlarmCanasRECT[crt_n].width =  a->width ;
	AlarmCanasRECT[crt_n].height =  a->height ;
	page[crt_n].PageSize =   AlarmCanasRECT[crt_n].height/ (int)(ALM_ROW*yscale[crt_n]) ;
	page[crt_n].PageSize -- ;   
	/* g_message("alloresize ****** alarm page is %d \n", page[crt_n].PageSize);  */
	
}



XHDC  dc[MAX_CRT_NUMBER] ;
int CreateFaultAlarm(int crt_n ,GtkWidget *DrawArea,GdkGC  *gc  )
{
	
	dc[crt_n].crt_n = crt_n;
	dc[crt_n].canvas = DrawArea ;
	dc[crt_n].pixmap = pixmap2[ crt_n];
	dc[crt_n].gc = info[crt_n].gc2 ;
	
	
	gtk_widget_set_events(DrawArea, GDK_EXPOSURE_MASK  
		| GDK_LEAVE_NOTIFY_MASK
		| GDK_BUTTON_PRESS_MASK
		| GDK_POINTER_MOTION_MASK );
	
	
	gtk_signal_connect (GTK_OBJECT (DrawArea),"expose_event",GTK_SIGNAL_FUNC(fault_alarm_expose),GINT_TO_POINTER (crt_n)) ;
	gtk_signal_connect (GTK_OBJECT (DrawArea),"button_press_event",GTK_SIGNAL_FUNC(fault_alarm_click),GINT_TO_POINTER (crt_n)) ;
	
	if (fault_alm_timeout_id[crt_n] != 0 )
	{
		gtk_timeout_remove(fault_alm_timeout_id[crt_n]);
		fault_alm_timeout_id[crt_n]= 0;
	}
	fault_alm_timeout_id[crt_n]=gtk_timeout_add ( 1000,(GtkFunction)FaultAlmDspProc, (gpointer) &dc[crt_n] );  
	printf("^^^index_tmp1[crt_n].header=%d,shm_rec_addr->head.header_ptr=%d\n",index_tmp1[crt_n].header,shm_rec_addr->head.header_ptr);
	
	DrawFaultAlarm(  crt_n ,DrawArea->window,info[crt_n].gc2  ) ;
	
	return TRUE;
}

gint SaveCustomFilter(GtkWidget	*widget,  gpointer	data)
{   
	GtkWidget *filterNameEntry,*filterList;
	int crt_n,i;
//	GtkTreeModel *model;
//	GtkListStore *store;
	GtkTreeIter  iter; 	
	GtkWidget *window = data;
	gchar *filterName;
	gchar stn_id[2],fault_mask[9],cmd_type[2],type_id_mask[9],dev_mask[9],group_mask[450],datatype_mask[9],attrib_mask[18],start_time[30],end_time[30],logic_name[50],long_name[MAX_SEARCH_NAME_SIZE];
	int offset =0;
	GetCrtNumberOfWgt (widget, &crt_n );
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE ;
	if (!FilterAlarm[crt_n].change_flag)
	{
		PopupMessageBox(alm_pop_shell[crt_n],MSG_FILTER_NOT_CHANGE);
		return FALSE;
	}
	filterList = g_object_get_data(G_OBJECT(window),"customFilterList");
	filterNameEntry  = g_object_get_data(G_OBJECT(window),"customFilterNameEntry");
	if (filterNum > MAX_FILTER_NUMBER)
	{
		PopupMessageBox(alm_pop_shell[crt_n],MSG_FILTER_OUTNUMBER);
		return FALSE;
	}
	else
	{
		if (strlen(gtk_entry_get_text(GTK_ENTRY(filterNameEntry))) > MAX_FILTER_NAME_LEN)
		{
			PopupMessageBox(alm_pop_shell[crt_n],MSG_FILTER_NAME_MAX_LEN); 
			return FALSE;
		}
		else if (strcmp(gtk_entry_get_text(GTK_ENTRY(filterNameEntry)),"")==0)
		{
			PopupMessageBox(alm_pop_shell[crt_n],MSG_INPUT_FILTER_NAME);
			return FALSE;
		}
		else
		{
			if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(storeFilter_a[crt_n]),&iter))
			{
				do 
				{
					gtk_tree_model_get(GTK_TREE_MODEL(storeFilter_a[crt_n]),&iter,eName_Filter,&filterName,-1);
					if (filterName!=NULL&&strcmp(filterName,gtk_entry_get_text(GTK_ENTRY(filterNameEntry)))==0)
					{
						PopupMessageBox(alm_pop_shell[crt_n],MSG_FILTER_NAME_REPEAT);
						return FALSE;
					}
						
				} while (gtk_tree_model_iter_next(GTK_TREE_MODEL(storeFilter_a[crt_n]),&iter));
			}
			sprintf(stn_id,"%u",FilterAlarm[crt_n].stn_id);
			sprintf(cmd_type,"%u",FilterAlarm[crt_n].cmd_type);
			sprintf(fault_mask,"%u",FilterAlarm[crt_n].fault_mask);
			sprintf(type_id_mask,"%u",FilterAlarm[crt_n].type_id_mask);
			sprintf(dev_mask,"%08X",FilterAlarm[crt_n].dev_mask[0]);
			for (i=0;i<MAX_GROUP_MASK_NUM;i++)
			{
				offset += sprintf(group_mask + offset, "%08X", FilterAlarm[crt_n].group_mask[i]); 
 			}
			sprintf(datatype_mask,"%08X",FilterAlarm[crt_n].datatype_mask);
			offset = sprintf(attrib_mask,"%08X",FilterAlarm[crt_n].attrib_mask[0]);
			sprintf(attrib_mask + offset,"%08X",FilterAlarm[crt_n].attrib_mask[1]);
			strcpy(start_time,TimeToString(FilterAlarm[crt_n].start_time));
			strcpy(end_time,TimeToString(FilterAlarm[crt_n].end_time));
			strcpy(logic_name,FilterAlarm[crt_n].LogicName);
			strcpy(long_name,FilterAlarm[crt_n].search_str);
			gtk_list_store_insert(GTK_LIST_STORE(storeFilter_a[crt_n]), &iter,1);	
			gtk_list_store_set(GTK_LIST_STORE(storeFilter_a[crt_n]), &iter, eName_Filter,gtk_entry_get_text(GTK_ENTRY(filterNameEntry)),
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
			SaveFilterXml(gtk_entry_get_text(GTK_ENTRY(filterNameEntry)),&FilterAlarm[crt_n],"Predefine");
			gtk_entry_set_text(GTK_ENTRY(filterNameEntry),"");	 
			filterNum++;
		}
	}
	FilterAlarm[crt_n].change_flag = FALSE;
	return TRUE;
}

gint DelCustomFilter(GtkWidget	*widget,  gpointer	data)
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
		gtk_list_store_remove(GTK_LIST_STORE(storeFilter_a[crt_n]), &filter_iter);	
		DelFilterXml(filterName,"Predefine");
		g_free(filterName);
		filterNum--;
	}
	return TRUE;
}

gboolean  RowVisible(GtkTreeModel *model,GtkTreeIter *iter,gpointer data)
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

gint CustomFilterDlgDelete(GtkWidget *w , gpointer data);
static GtkWidget*
CreateCustomFilterDlg(GtkWidget *parent ,int crt_n)
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

	gchar *pConvert;
	
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), pConvert=_toUtf8EX("OIX_STR_FILTER_SET"));
	G_SAFE_FREE(pConvert);
	gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(parent)); 
	gtk_window_set_destroy_with_parent(GTK_WINDOW(window),TRUE);
	
	gtk_signal_connect (GTK_OBJECT (window), "delete_event", 
		GTK_SIGNAL_FUNC (CustomFilterDlgDelete), GINT_TO_POINTER (crt_n)); 
	
//	gtk_widget_show (window);
	vbox = gtk_vbox_new (FALSE, 5);
//	gtk_widget_show (vbox);
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
	column = gtk_tree_view_column_new_with_attributes(pConvert=_toUtf8EX("OIX_STR_FILTER_NAME"),renderer, 
		"text", eName_Filter, NULL);
	G_SAFE_FREE(pConvert);
	gtk_tree_view_append_column(GTK_TREE_VIEW (list), column);
	modelfilter = gtk_tree_model_filter_new(GTK_TREE_MODEL(storeFilter_a[crt_n]),NULL);
	gtk_tree_model_filter_set_visible_func(GTK_TREE_MODEL_FILTER(modelfilter),RowVisible,NULL,NULL);
	gtk_tree_view_set_model(GTK_TREE_VIEW (list), GTK_TREE_MODEL(modelfilter));
	g_object_unref(storeFilter_a[crt_n]);
	gtk_container_add (GTK_CONTAINER (scrolled_window), list);
//	gtk_widget_show(list);
	
	hbox2 = gtk_hbox_new (FALSE, 2);
//	gtk_widget_show (hbox2);
	gtk_container_border_width(GTK_CONTAINER(hbox2), 2); 
	gtk_container_add (GTK_CONTAINER (vbox), hbox2);
	
	label = gtk_label_new(pConvert=_toUtf8EX("OIX_STR_FILTER_NAME"));
	G_SAFE_FREE(pConvert);
	gtk_container_add(GTK_CONTAINER(hbox2), label);
//	gtk_widget_show (label);
	
	entry = gtk_entry_new ();
	gtk_container_add(GTK_CONTAINER(hbox2), entry);
//	gtk_widget_show (entry);
	g_object_set_data(G_OBJECT(window),"customFilterNameEntry",entry);
	gtk_entry_set_max_length(GTK_ENTRY(entry),MAX_FILTER_NAME_LEN);
	
	separator=gtk_hseparator_new(); 
	gtk_container_add(GTK_CONTAINER(vbox), separator); 
//	gtk_widget_show(separator); 
	
	hbox1 = gtk_hbox_new (FALSE, 2);
//	gtk_widget_show (hbox1);
	gtk_container_border_width(GTK_CONTAINER(hbox1), 2); 
	gtk_container_add (GTK_CONTAINER (vbox), hbox1);
	
	button1=gtk_button_new_with_label(pConvert=_toUtf8EX("OIX_CSTR_ADD")); 
	G_SAFE_FREE(pConvert);
	g_signal_connect (G_OBJECT (button1), "clicked",GTK_SIGNAL_FUNC (SaveCustomFilter),  window);
	gtk_container_add(GTK_CONTAINER(hbox1), button1); 
//	gtk_widget_show(button1); 
	
	button2=gtk_button_new_with_label(pConvert=_toUtf8EX("OIX_CSTR_DEL")); 
	G_SAFE_FREE(pConvert);
	g_signal_connect (G_OBJECT (button2), "clicked",GTK_SIGNAL_FUNC (DelCustomFilter),  window);
	gtk_container_add(GTK_CONTAINER(hbox1), button2); 
//	gtk_widget_show(button2); 
	
	button3=gtk_button_new_with_label(pConvert=_toUtf8EX("OIX_CSTR_EXIT")); 
	G_SAFE_FREE(pConvert);
	g_signal_connect (G_OBJECT (button3), "clicked",GTK_SIGNAL_FUNC (CustomFilterDlgDelete),  GINT_TO_POINTER(crt_n));
	gtk_container_add(GTK_CONTAINER(hbox1), button3); 

	gtk_widget_show_all(window);
	return  window ;	
}

char *TimeToString(time_t tmTime)
{
	struct tm *tm1 ;
	char szTime[23]={"-1"};
	if (tmTime != -1)
	{
		tm1 = localtime(&tmTime);
		sprintf(szTime,"%d-%d-%d %d:%d:%d",tm1->tm_year+1900,tm1->tm_mon+1,tm1->tm_mday,
			tm1->tm_hour,tm1->tm_min,tm1->tm_sec)  ;
	}
	return szTime;
}

time_t StringToTime(char * szTime)  
{  
	struct tm tm1;  
	time_t time1 = -1;  
	if (strcmp(szTime,"-1")!=0)
	{
		sscanf(szTime, "%d-%d-%d %d:%d:%d",     
			&tm1.tm_year,  
			&tm1.tm_mon,  
			&tm1.tm_mday,  
			&tm1.tm_hour,  
			&tm1.tm_min,  
			&tm1.tm_sec);               
		tm1.tm_year -= 1900;  
		tm1.tm_mon --;  
		tm1.tm_isdst=-1;    
		time1 = mktime(&tm1);  
	}
	return time1;
} 

gint ChgCustomFilter(GtkComboBox* pComboBox, gpointer data)
{
	GtkTreeModel *pModel;
	GtkTreeIter iter;
	GtkWidget  *opmenu;
	int i,j;
	char *filterName;
	gchar *stn_id,*fault_mask,*cmd_type,*type_id_mask,*dev_mask,*group_mask,*datatype_mask,*attrib_mask,*start_time,*end_time,*logic_name,*long_name;
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
		if (SetFilterDlg[crt_n] == (GtkWidget *)NULL)
		{
			SetFilterDlg[crt_n] = CreateCustomFilterDlg( alm_pop_shell[crt_n] ,crt_n);
			gtk_widget_show(SetFilterDlg[crt_n]);
		}
		else
			gtk_window_present (GTK_WINDOW(SetFilterDlg[crt_n]));
		if (FilterAlarm[crt_n].change_flag)
			gtk_combo_box_set_active(pComboBox,-1);
		else
			gtk_combo_box_set_active(pComboBox,curFilter);
	}
	else if (strcmp(filterName, pConvert1=_toUtf8EX("OIX_STR_FILTER_INIT"))==0)
	{
		FiterInit(alm_pop_shell[crt_n],GINT_TO_POINTER(crt_n));
		AlmDspProc( crt_n);
	}
	else
	{
		gtk_tree_model_get(pModel, &iter, eStnId_Filter,&stn_id,
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
		FilterAlarm[crt_n].stn_id = (UCHAR)atoi(stn_id);
		if (cmd_type != NULL)	
		{
			FilterAlarm[crt_n].cmd_type = (UCHAR)atoi(cmd_type);
		}
		else
		{
			FilterAlarm[crt_n].cmd_type = 0;
		}
		FilterAlarm[crt_n].fault_mask = (UINT)atoi(fault_mask);
		FilterAlarm[crt_n].type_id_mask = (UINT)atoi(type_id_mask);
		sscanf(dev_mask,"%08X",&FilterAlarm[crt_n].dev_mask[0]);
		for (i=0;i<MAX_GROUP_MASK_NUM;i++)
		{
			sscanf(group_mask+offset,"%08X",&FilterAlarm[crt_n].group_mask[i]);
			offset += 8;
		}
		sscanf(datatype_mask,"%08X",&FilterAlarm[crt_n].datatype_mask);
		sscanf(attrib_mask,"%08X",&FilterAlarm[crt_n].attrib_mask[0]);
		sscanf(attrib_mask+8,"%08X",&FilterAlarm[crt_n].attrib_mask[1]);
		FilterAlarm[crt_n].start_time = StringToTime(start_time);
		FilterAlarm[crt_n].end_time = StringToTime(end_time);
		if(logic_name!=NULL)
			strcpy(FilterAlarm[crt_n].LogicName,logic_name);
		if(long_name!=NULL)
			strcpy(FilterAlarm[crt_n].search_str,long_name);

		opmenu = g_object_get_data(G_OBJECT(alm_pop_shell[crt_n]),"StationOpMenu" );
		gtk_option_menu_set_history( GTK_OPTION_MENU(opmenu) ,(guint)FilterAlarm[crt_n].stn_id);

		opmenu = g_object_get_data(G_OBJECT(alm_pop_shell[crt_n]),"TypeIdOpMenu" );
		gtk_option_menu_set_history( GTK_OPTION_MENU(opmenu) ,(guint)FilterAlarm[crt_n].cmd_type);
		
		for (i=0,j=1;i<MAX_DATATYPE_NUM;i++)
		{
			if(GetRecLongnameByType((UCHAR)i, recordTypeName )!=-1)
			{
				if (FilterAlarm[crt_n].datatype_mask & 1<<(i-1) )    		
					gtk_check_menu_item_set_active  ( GTK_CHECK_MENU_ITEM(DataTypeItem[crt_n][j]),TRUE);
				else
					gtk_check_menu_item_set_active  ( GTK_CHECK_MENU_ITEM(DataTypeItem[crt_n][j]),FALSE);
				j++;
			}
		}
		FilterAlarm[crt_n].change_flag = FALSE;
		FilterAlarm[crt_n].is_change = TRUE;
		AlmDspProc( crt_n);
	}
	curFilter = gtk_combo_box_get_active(pComboBox);
	G_SAFE_FREE(pConvert);
	G_SAFE_FREE(pConvert1);

	return TRUE;
}

void InitAlarmFilter(FILTER_T *pFilter ,int crt_n)
{
	int i,station_num;
	GetStationNum(&station_num);
	if (station_num>1)
		pFilter->stn_id  = 0 ;
	else
		pFilter->stn_id  = StnDef[1].Id ;
	pFilter->stn_mask = 0xffffffff;
	pFilter->rec_type   = 0 ;
	pFilter->fault_mask   = 0xffffffff ;
	pFilter->cmd_type	=	0;
	pFilter->type_id_mask   = 0xffffffff ;
	for (i=0;i<MAX_DEV_MASK_NUM;i++)
	{
		pFilter->dev_mask[i] = 0xffffffff;
	}
	pFilter->datatype_mask = 0xffffffff ;
	for (i=0;i<MAX_GROUP_MASK_NUM;i++)
	{
		pFilter->group_mask[i] = 0xffffffff;
	}
	pFilter->attrib_mask[0] = 0;
	pFilter->attrib_mask[1] = 0;
	pFilter->start_time  = -1 ;
	pFilter->end_time   = -1 ;
	strcpy(pFilter->search_str, "");
	strcpy(pFilter->LogicName, "");
	
	pFilter->phead =  &shm_rec_addr->head;
	pFilter->pRec   =(REC_ENTRY_UNION*) &shm_rec_addr->rec;
	
	pFilter->is_change =  1;
	pFilter->change_flag = FALSE;
}

gint FiterInit(GtkWidget*w , gpointer data)
{
	int crt_n;
	crt_n = (gint)data ;
	//crt_n=getCrtNumFromPointer((int)data);
	
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER ) return 0 ;
	InitAlarmFilter(&FilterAlarm[crt_n],crt_n) ;  
	InitToolBarButton(alm_pop_shell[crt_n],crt_n)	 ;
	
	switch(alm_rec_mode[crt_n] )
	{
	case ALM_REC_REAL1:    /*ALM_REC_REAL1*/
		
		
		FilterAlarm[crt_n].phead =  &shm_rec_addr->head;
		FilterAlarm[crt_n].pRec   = (REC_ENTRY_UNION* )&shm_rec_addr->rec;	        
		
		break;   
		
		
	case ALM_REC_SYS_INFO :    /*综合信息*/
		
		FilterAlarm[crt_n].phead =  &shm_info_rec_addr->head;
		FilterAlarm[crt_n].pRec   =(REC_ENTRY_UNION*) &shm_info_rec_addr->rec;	        
		
		break; 
	}
	
	
	
	
	FilterAlarm[crt_n].is_change = TRUE;
	
	return 0 ;  
	
}


void SetAdjInfo(PAGE_INFO *PageInfo,GtkAdjustment *AdjSet)
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
void InitPageInfo(PAGE_INFO *PageInfo , int crt_n,int recCount)
{
	PageInfo->RecCount   = recCount /*shm_rec_addr->head.count  */ ;
	PageInfo->PageCount  = (PageInfo->RecCount+PageInfo->PageSize-1)/PageInfo->PageSize ;
	PageInfo->CurPage    = max(PageInfo->PageCount-1 , 0) ;
	PageInfo->isSelected =0;
	PageInfo->SelectStart = PageInfo->SelectEnd =0;
	PageInfo->PageStart  =  (PageInfo->CurPage) * (PageInfo->PageSize) ;
	PageInfo->PageEnd    =   min ((PageInfo->PageStart + PageInfo->PageSize -1) , (PageInfo->RecCount-1) )   ;        
	
}
void SetPageInfo(PAGE_INFO *PageInfo )
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

int  isInSel(crt_n ,iRec)
{   UINT j  ;
for  (j= 0 ; j<  sel_pt[crt_n].nextpos ; j++ )
{
	if (iRec == sel_pt[crt_n].sel[j]  ) return TRUE;
	
}
return   FALSE ; 
}
/*****************************  begin  code *****************************************/
void AlmDspProcE ( GdkDrawable *drawable , int crt_n, int isPrint)
{
	int i,j ,X_OFFSET = 0 ;
	ALM_STR			alarm_str ;
	BOOL			blink ;
	COLOR_PAIR		color ;
	static short blankstate[MAX_CRT_NUMBER];
	char buff[20] ;	
	char msgRes[512] ;
	char record[64];
	char hostname[64],cmdresult[128];
	GtkWidget *labelPage ; 
	GdkColor  bkColor ; 
	FILE *fp;	
	int ruler=0;

	gchar *pConvert=0;

	DrawAlmTabHead( drawable ,alm_draw_gc[crt_n],crt_n  );
	index_tmp[crt_n].buff_size = MAX_SHM_RECORD_NUM ;
	if (alm_rec_mode[crt_n]	== ALM_REC_REAL1)  
	{
		if (FilterAlarm[crt_n].is_change)
			page[crt_n].isSelected = 0 ; 
		if ((index_tmp[crt_n].header != shm_rec_addr->head.header_ptr && isLastPage[crt_n]) 
			|| FilterAlarm[crt_n].is_change || (page[crt_n].CurPage+1)==page[crt_n].PageCount )  
		{
			index_tmp[crt_n].lastcount = index_tmp[crt_n].count ;
			index_tmp[crt_n].count = 0 ;
			index_tmp[crt_n].header = shm_rec_addr->head.header_ptr;
			alm_rec_filter(FilterAlarm[crt_n], &index_tmp[crt_n]);  
			FilterAlarm[crt_n].is_change = FALSE ;
			page[crt_n].RecCount = index_tmp[crt_n].count ;     
			/*printf("$$$$$$$$$$$$$$$$indxe_lastcount =%d , curcount = d%\n" ,index_tmp[crt_n].lastcount ,index_tmp[crt_n].count )	;*/					
		}
	} else if (alm_rec_mode[crt_n]	== ALM_REC_SYS_INFO)
	{
		if (FilterAlarm[crt_n].is_change)
			page[crt_n].isSelected = 0 ; 
		if (index_tmp[crt_n].header != shm_info_rec_addr->head.header_ptr || FilterAlarm[crt_n].is_change    )  
		{
			index_tmp[crt_n].lastcount = index_tmp[crt_n].count ;
			index_tmp[crt_n].count = 0 ;
			index_tmp[crt_n].header = shm_info_rec_addr->head.header_ptr;
			alm_rec_filter(FilterAlarm[crt_n], &index_tmp[crt_n]);  
			FilterAlarm[crt_n].is_change = FALSE ;
			page[crt_n].RecCount = index_tmp[crt_n].count ;     
			/*printf("$$$$$$$$$$$$$$$$indxe_lastcount =%d , curcount = d%\n" ,index_tmp[crt_n].lastcount ,index_tmp[crt_n].count )	;*/					
		}
	}
	else if (alm_rec_mode[crt_n]	== ALM_REC_HISTRYDB) 
	{
		
#ifdef   HISTORY_DB_ALM     
		page[crt_n].RecCount = 0 ; 
		if  (shm_hdb_alm->data_al.queryflag   )
		{
			gdk_rgb( 150,0,0,&bkColor )  ;
			gdk_gc_set_foreground ( alm_draw_gc[crt_n], &bkColor);
			windk_draw_text( drawable, font_set[3], alm_draw_gc[crt_n],  ALM_X0, ALM_Y1 + 4 * ALM_ROW , "Please wait for connecting History database !!!",strlen("Please wait for connecting History database !!!"),crt_n); ; 
			printf ("while  query !!!!!\n");
			return ;
		}
		if (  /* FilterAlarm[crt_n].is_change  && */  shm_hdb_alm ) 
		{
			
			index_tmp[crt_n].count = 0 ;
			index_tmp[crt_n].header = shm_hdb_alm->data_al.hdb_rec.head.header_ptr;
			FilterAlarm[crt_n].phead =  &shm_hdb_alm->data_al.hdb_rec.head;
			FilterAlarm[crt_n].pRec  = (REC_ENTRY_UNION*)(&shm_hdb_alm->data_al.hdb_rec.rec );
			
			alm_rec_filter(FilterAlarm[crt_n], &index_tmp[crt_n]);  
			FilterAlarm[crt_n].is_change = FALSE ;
			page[crt_n].RecCount = index_tmp[crt_n].count ;     
			
		}
#endif             
	}else   /*history file rec */
	{
		if ( FilterAlarm[crt_n].is_change)
		{
			index_tmp[crt_n].count = 0 ;
			if (hist_rec_addr[crt_n].head.header_ptr == -1) return ;  //文件有问题 !added by chichi  080829
			index_tmp[crt_n].header = hist_rec_addr[crt_n].head.header_ptr;
			FilterAlarm[crt_n].phead =  &hist_rec_addr[crt_n].head;
			FilterAlarm[crt_n].pRec = (REC_ENTRY_UNION*)(&hist_rec_addr[crt_n].rec );
			alm_rec_filter(FilterAlarm[crt_n], &index_tmp[crt_n]);  
			FilterAlarm[crt_n].is_change = FALSE ;
			page[crt_n].RecCount = index_tmp[crt_n].count ;
			
		}
		
	}
	
	SetPageInfo( &page[crt_n] );  
	if (isLastPage[crt_n])
	{
		page[crt_n].CurPage = max(page[crt_n].PageCount -1 ,0 )   ;
		
		page[crt_n].PageStart =  (page[crt_n].CurPage) * (page[crt_n].PageSize)  -   page[crt_n].PageMod   ;  /* slide back  window PageMode step*/
		
		page[crt_n].PageEnd   =   min ((page[crt_n].PageStart + page[crt_n].PageSize -1) , (page[crt_n].RecCount-1) )   -   page[crt_n].PageMod   ;
	}
	else
		page[crt_n].CurPage = (page[crt_n].PageStart+ 2 )/ page[crt_n].PageSize    ;  
	
	
	SetPageInfo( &page[crt_n] );
	page[crt_n].PageEnd   = min ((page[crt_n].PageStart + page[crt_n].PageSize -1) , (page[crt_n].RecCount-1) )   ;

	//add by hcl resolve page[crt_n].RecCount=12000 ,can't stop flash
	if ((page[crt_n].PageEnd==(page[crt_n].RecCount-1))&&(page[crt_n].RecCount==MAX_SHM_RECORD_NUM))
	{
		page[crt_n].PageStart=page[crt_n].PageEnd-page[crt_n].PageSize+1;

	}

	SetAdjInfo( &page[crt_n],adj1[crt_n] );   
	
	labelPage = g_object_get_data(G_OBJECT(alm_pop_shell[crt_n]),"labelPage");
	
	sprintf(buff , "%s:%d/%d ", pConvert=_toUtf8EX(OIX_CSTR_PAGE_NUM), page[crt_n].CurPage+1,page[crt_n].PageCount);
	G_SAFE_FREE(pConvert);
	
	if (labelPage != NULL)
		gtk_label_set_text(GTK_LABEL(labelPage), buff);
#ifdef 	DEBUG
	/*   printf ("\n pagestart[%d]:%d \t  pageEnd: %d  \n",crt_n, page[crt_n].PageStart ,page[crt_n].PageEnd );    */
#endif 
	if (isPrint)
	{
		fp = fopen(AlarmPrintFile,"w");
		if (fp ) 
		{
// #ifdef SUN_OS
// 			fprintf(fp,"%s          %s        %s        %s          \n", 
// 				_CS_(AlarmHeadTitle[0]),_CS_(AlarmHeadTitle[1]),_CS_(AlarmHeadTitle[2]),_CS_(AlarmHeadTitle[3]));
// #endif
//#ifdef WINDOWS_OS
			
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
				strncpy(PrintLocationRuler+PrintTitleLocation[j], _CS_(AlarmHeadTitle[i]), strlen(_CS_(AlarmHeadTitle[i])));	    	
				j++;
			}
			/*printf("PrintLocationRuler:%s** \n",PrintLocationRuler);*/
			fprintf(fp,"%s \n",PrintLocationRuler);
//#endif
			
			
		}   /* if(fp)end  */
	}
	if   (!page[crt_n].RecCount) return    ;
	
	for (i = page[crt_n].PageStart , j= 0 ; i <=  page[crt_n].PageEnd ;j++, i++)
	{
		
		
		if (alm_rec_mode[crt_n]	== ALM_REC_REAL1)
		{
			
			GetAlmStrings(&shm_rec_addr->rec[index_tmp[crt_n].index_pt[i].pt], &alarm_str , &blink , &color );
		}
		else if (alm_rec_mode[crt_n]	== ALM_REC_HISTRYDB)
		{   
			
			GetAlmStrings(&shm_hdb_alm->data_al.hdb_rec.rec[index_tmp[crt_n].index_pt[i].pt] ,&alarm_str , &blink , &color );
		}
		else if (alm_rec_mode[crt_n]	== ALM_REC_SYS_INFO)
		{
			GetAlmStrings(&shm_info_rec_addr->rec[index_tmp[crt_n].index_pt[i].pt], &alarm_str , &blink , &color );
		}
		else if (alm_rec_mode[crt_n]	== ALM_REC_HISTRY1)
		{
			
			GetAlmStrings(&hist_rec_addr[crt_n].rec[index_tmp[crt_n].index_pt[i].pt], &alarm_str , &blink , &color );
		}
		
		if (i%2) 
		{
			gdk_rgb(20,20,20,&bkColor )  ;        
		}else
			gdk_rgb( 25,30,30,&bkColor )  ; 
		
		/*  printf("11isSelect %d ,pageStart %d ,pageEnd %d ,i \n", page[crt_n].isSelected,page[crt_n].SelectStart  ,page[crt_n].SelectEnd,i);     */       
		
		/***************************************************************************************************/
		
		sprintf(record , "%s: ",  alarm_str.sn_str);
		strcat(record , alarm_str.repeatAlm);	
		
		if (strcmp(alarm_str.usrname,"")!=0)
			sprintf(hostname , "%s@%s", alarm_str.usrname, alarm_str.hostname);
		else
			sprintf(hostname , "%s",   alarm_str.hostname);
		
		
		if ((page[crt_n].isSelected)&& isInSel(crt_n , index_tmp[crt_n].index_pt[i].pt) )
		{
			gdk_rgb( 150,150,200,&bkColor )  ;
			gdk_gc_set_foreground ( alm_draw_gc[crt_n], &bkColor);
			gdk_draw_rectangle(drawable,alm_draw_gc[crt_n],1,(int)(ALM_X0*xscale[crt_n]),(int)((FAULT_ALM_Y1  + (j+1) * ALM_ROW)*yscale[crt_n]-10),(int)(ALM_CANVAS_WIDTH*xscale[crt_n]),(int)((ALM_ROW-4)*yscale[crt_n])-5); 
			
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
			gdk_gc_set_foreground ( alm_draw_gc[crt_n], &bkColor);           
			gdk_draw_rectangle(drawable,alm_draw_gc[crt_n],1,(int)(ALM_X0*xscale[crt_n]),(int)((FAULT_ALM_Y1 + (j+1) * ALM_ROW)*yscale[crt_n]-15),(int)(ALM_CANVAS_WIDTH*xscale[crt_n]),(int)(ALM_ROW*yscale[crt_n])); 
			
		}
		
		if (isPrint && fp)  
		{
// #ifdef SUN_OS
// 			fprintf(fp,"%s %s \t  %s \t %s \t %s  \t%s \t %s  \n",record ,alarm_str.time_str,alarm_str.longname,alarm_str.cmd_result ,alarm_str.message,alarm_str.state_str , hostname);
// #endif 
// 			
// #ifdef WINDOWS_OS
			
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
			
//#endif
		}
		/***************************************************************************************************/                
		if (blink)
		{
			gdk_gc_set_foreground ( alm_draw_gc[crt_n], &pixels[crt_n][ color.clr[blankstate[crt_n]]]);	
		}
		else
			gdk_gc_set_foreground ( alm_draw_gc[crt_n], &pixels[crt_n][color.clr[0]]);
		if (blink)
			windk_draw_text( drawable,font_set[0], alm_draw_gc[crt_n],  ALM_X0-20 , ALM_Y1 + j * ALM_ROW , "★" ,strlen("★" ) ,crt_n);
		
		gdk_gc_set_foreground ( alm_draw_gc[crt_n], &pixels[crt_n][color.clr[0]]);
		
		windk_draw_text( drawable, font_set[0], alm_draw_gc[crt_n],  ALM_X0, ALM_Y1 + j * ALM_ROW , record ,strlen(record),crt_n);
		windk_draw_text( drawable, font_set[0], alm_draw_gc[crt_n],  ALM_X1, ALM_Y1 + j * ALM_ROW , alarm_str.time_str,strlen(alarm_str.time_str),crt_n);
        if (OIX_EVENT_DSP_LOGICNAME )
		{
			X_OFFSET  = 120 ; 
			windk_draw_text( drawable, font_set[0], alm_draw_gc[crt_n],  ALM_X2, ALM_Y1 + j * ALM_ROW , alarm_str.logicname ,strlen(alarm_str.logicname),crt_n);  
		}
		sprintf(msgRes,"%s   %s" ,alarm_str.longname   , alarm_str.state_str  ) ; 
		if (OIX_EVENT_ALIAN_MOD)  //需要命令状态对齐；
		{
			windk_draw_text( drawable, font_set[2], alm_draw_gc[crt_n],  ALM_X3+X_OFFSET, ALM_Y1 + j * ALM_ROW ,   alarm_str.longname  ,strlen(alarm_str.longname),crt_n);
			windk_draw_text( drawable, font_set[0], alm_draw_gc[crt_n],  ALM_X4+X_OFFSET, ALM_Y1 + j * ALM_ROW , alarm_str.state_str ,strlen(alarm_str.state_str),crt_n);  
			
		}
		else
			windk_draw_text( drawable, font_set[2], alm_draw_gc[crt_n],  ALM_X3+X_OFFSET, ALM_Y1 + j * ALM_ROW , msgRes/*alarm_str.longname*/ ,strlen(alarm_str.longname),crt_n);
		
		if (strcmp(alarm_str.cmd_src,""))
			sprintf(cmdresult,"%s:%s",alarm_str.cmd_src ,alarm_str.cmd_result) ;
		else
			sprintf(cmdresult,"%s",alarm_str.cmd_result) ;
		
		windk_draw_text( drawable, font_set[0], alm_draw_gc[crt_n],   ALM_X3 + 550+X_OFFSET, ALM_Y1 + j * ALM_ROW ,  cmdresult ,strlen(cmdresult),crt_n);
		
		windk_draw_text( drawable, font_set[2], alm_draw_gc[crt_n],  ALM_X3+X_OFFSET, ALM_Y1 + j * ALM_ROW ,  alarm_str.message ,strlen(alarm_str.message),crt_n);
		
		if (alm_rec_mode[crt_n]	!= ALM_REC_HISTRYDB)
			windk_draw_text( drawable, font_set[0], alm_draw_gc[crt_n],  ALM_X5, ALM_Y1 + j * ALM_ROW , hostname ,strlen(hostname),crt_n);
		
	}
	
	
	if (isPrint && fp) fclose(fp);
	
	if   (blankstate[crt_n] >0)
		blankstate[crt_n] = 0 ;
	else
		blankstate[crt_n] = 1 ;
#ifdef 	DEBUG
	/*  g_message("FilterAlarm[%d].is_change:%d ,index_tmp[%d].count :%d \n",crt_n ,  FilterAlarm[crt_n].is_change , crt_n  ,index_tmp[crt_n].count); */ 
#endif	
	
}

/****************  Timer  call  !!!      ************/
gint  AlmDspProc( crt_n )
int crt_n ;
{
	GtkWidget *scroll ; 
	GdkColor clr ;
	/*	g_mutex_lock (alm_mutex[crt_n]);    */
	
	gdk_rgb(20,20,20,&clr);
	
	
	/* printf("OIX::+++++++++++++++++++++++AlmDspProc( crt_n )++++++++++++!!!!\n"); */ 
	/*  printf("pagcurge = %d, pagecount =%d \n",page[crt_n].CurPage , page[crt_n].PageCount);  */
	gdk_gc_set_foreground(alm_draw_gc[crt_n], &clr);
	gdk_draw_rectangle (AlmOffDrawable[crt_n],alm_draw_gc[crt_n],TRUE, 0,0,LOCAL_WIDTH, 1600); 
	AlmDspProcE( AlmOffDrawable[crt_n] ,crt_n,FALSE);
	gdk_window_copy_area (alm_canvas[crt_n] ->window,alm_draw_gc[crt_n],0,0,AlmOffDrawable[crt_n],0,0,LOCAL_WIDTH, 1600);
	scroll = g_object_get_data(G_OBJECT(alm_pop_shell[crt_n]),"scrollbar" );
	gtk_widget_queue_draw(scroll);
	return TRUE;
	/*	 g_mutex_unlock (alm_mutex[crt_n]);  */
	
	
}
gint AutoFilterProc(int crt_n)
{
	int i,j,k;
	char    unit_state_str[POINT_NAME_SIZE+12];
	for(i=1;i<=alarmStnDef[FilterAlarm[crt_n].stn_id].dev_num;i++)
	{
		for (j=1;j<=(int)alarmStnDef[FilterAlarm[crt_n].stn_id].dev[i].group_num;j++)
		{	
			for (k=0;k<MAX_UNIT_STATUS_NUM;k++)
			{
				sprintf(unit_state_str,"%s.%s.%s%d.",alarmStnDef[FilterAlarm[crt_n].stn_id].Name,alarmStnDef[FilterAlarm[crt_n].stn_id].dev[i].Name,"AUTO.FILTER.STATE",k);
				if (InterLock_Judge(unit_state_str)==1)
				{
					if (alarmStnDef[FilterAlarm[crt_n].stn_id].dev[i].group[j].Status & (0X1<<k))
					{
						alarmStnDef[FilterAlarm[crt_n].stn_id].dev[i].group[j].isFilter[crt_n] = FALSE;
						FilterAlarm[crt_n].group_mask[i-1] = FilterAlarm[crt_n].group_mask[i-1] | (1<<(j-1));
					}
					else
					{
						alarmStnDef[FilterAlarm[crt_n].stn_id].dev[i].group[j].isFilter[crt_n] = TRUE;
						FilterAlarm[crt_n].group_mask[i-1] = FilterAlarm[crt_n].group_mask[i-1] & (~(1<<(j-1)));
					}
					FilterAlarm[crt_n].is_change = TRUE;
					AlmDspProc(crt_n);
				}
			}
		}
	}
	return TRUE;
}

USHORT SecCall[MAX_CRT_NUM] ={0,0,0,} ;
gint  TimeCall( crt_n )
{
	/* printf ("OIX:  dsp @@@@ !!! ctr_n= %d  \n " ,crt_n);	*/
	SecCall[crt_n] ++  ;
	if  (alm_rec_mode[crt_n]	== ALM_REC_REAL1   )  
		if (index_tmp[crt_n].header != shm_rec_addr->head.header_ptr  || !(SecCall[crt_n]%SECNUM)  )
		{
			AlmDspProc( crt_n ) ;
			/* printf ("OIX:--time  ------thread calll FetchDataFromSHM  !!! ctr_n= %d  \n " ,crt_n);	*/
		}
		
		if  (alm_rec_mode[crt_n]	== ALM_REC_SYS_INFO   )  
			if (index_tmp[crt_n].header != shm_info_rec_addr->head.header_ptr  || !(SecCall[crt_n]%SECNUM)  )
			{
				AlmDspProc( crt_n ) ;
				/* printf ("OIX:--time  ------thread calll FetchDataFromSHM  !!! ctr_n= %d  \n " ,crt_n);	*/
			}
			
			if  ((alm_rec_mode[crt_n]	== ALM_REC_HISTRYDB ) && (!(SecCall[crt_n]%(SECNUM *2))) )
			{
				AlmDspProc( crt_n ) ;
				printf ("OIX:--time  ------thread calll FetchDataFromHDB !!! ctr_n= %d  \n " ,crt_n);	 
			}
			
			return TRUE ; 
}
void	DrawAlmTabHead(GdkDrawable *drawable ,GdkGC *gc,int crt_n  )
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

void  alarm_expose(GtkWidget *w ,  GdkEventExpose *event, gpointer data)  
{ 
	
	int crt_n = (gint ) data;
	
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER ) return ;
	
	AlmDspProc( crt_n);
}



void  alarm_motion(GtkWidget *w ,GdkEventMotion *event,   gpointer data)
{
	int crt_n =(gint) data ; 
	
	
	if(   !(event->state & GDK_BUTTON1_MASK))
		return  ;
	page[crt_n].mycur.x2 = (int)(event->x) ; page[crt_n].mycur.y2 = (int)(event->y) ;
	/* printf ("motion :::x = %d, y=%d \n "  ,(int)event->x ,(int)event->y)  ;    */   
	
}

void  alarm_button_release( GtkWidget *w ,GdkEvent *event  , gpointer data)
{   int  i,j,S,E ;
int  crt_n = (gint)  data ;

gint rowNum ,height ; 
if (event->type == GDK_BUTTON_RELEASE) {   
	GdkEventButton *bevent = (GdkEventButton *) event; 
	if (bevent->button == 1)   
	{	        
		height = windk_text_height(font_set[DBTAB_FONT] ,"娑", strlen("娑"), FONT_SIZE_ALARM);
		
		printf("height=========%d\n",height);
		
		rowNum = ( min(page[crt_n].mycur.y1,page[crt_n].mycur.y2) - (int)((ALM_Y1 -  height)*yscale[crt_n]+10)) / (int)(ALM_ROW*yscale[crt_n] )  ;
		rowNum = max(rowNum ,0);
		page[crt_n].isSelected  =  1 ; 
		S = rowNum + page[crt_n].PageStart ;
		page[crt_n].SelectStart  = index_tmp[crt_n].index_pt[S].pt ; 
		
		rowNum = (  max(page[crt_n].mycur.y1,page[crt_n].mycur.y2) - (int)((ALM_Y1 -  height)*yscale[crt_n]+10)) / (int)(ALM_ROW*yscale[crt_n] )  ; /* ALM_Y1 +  height) / ALM_ROW   ;*/
		E = min((rowNum + page[crt_n].PageStart),page[crt_n].PageEnd) ;
		page[crt_n].SelectEnd  = index_tmp[crt_n].index_pt[E].pt ; 
		
		sel_pt[crt_n].nextpos = 0  ;
		for (i = S ,j =0 ; i<=E ; j ++ , i++ )
		{
			sel_pt[crt_n].sel[j] = index_tmp[crt_n].index_pt[i].pt ;
			sel_pt[crt_n].nextpos++ ; 
			printf ("_____*******sel = %d ,nextpos = %d \n" ,sel_pt[crt_n].sel[j] , sel_pt[crt_n].nextpos) ;  
		}
		printf ("+++++++++++rownum = %d , selectstart = %d , selectend = %d ,S = %d ,E = %d \n" ,
			rowNum,page[crt_n].SelectStart ,page[crt_n].SelectEnd,S,E ) ;
		
		
		AlmDspProc( crt_n);        
		
	} 
	
}
}

extern DB_TAB_WINDOW_INFO db_tab_info_tmp[];


gint alarm_button_press_ex( GtkWidget *w ,GdkPoint *pt  , gpointer data,GdkEventButton *bevent)
{
	gint  crt_n = (gint)  data ;
	POINTER  dbpt ;
	GtkWidget  *popupmenu;
	extern POINTER    db_selected_ptr[];
	gint rowNum ,height ,count; 
	
	height = windk_text_height(font_set[DBTAB_FONT] ,"娑", strlen("娑"),FONT_SIZE_ALARM/*fontdesc[0]*/);		 
	rowNum = ( pt->y - (int)((ALM_Y1 -  height)*yscale[crt_n])) / (int)(ALM_ROW*yscale[crt_n])  + 1 ;
	count  = rowNum + page[crt_n].PageStart ; /* page[crt_n].CurPage   * page[crt_n].PageSize ;   */
	
	if ( count   > index_tmp[crt_n].count ||count <=  page[crt_n].PageStart)
		return 0 ;
	if (alm_rec_mode[crt_n]	== ALM_REC_REAL1)
	{
		if(-1==GetPointFromRec(&shm_rec_addr->rec[index_tmp[crt_n].index_pt[count -1 ].pt],&dbpt))
			return 0 ; 
	}
	else if (alm_rec_mode[crt_n]  == ALM_REC_HISTRYDB)
	{   
		
		if(-1==GetPointFromRec(&shm_hdb_alm->data_al.hdb_rec.rec[index_tmp[crt_n].index_pt[count -1 ].pt],&dbpt))
			return 0 ; 
		
	}
	else if (alm_rec_mode[crt_n]  == ALM_REC_HISTRY1) /* HISTORY FILE!!! */
	{
		if(-1==GetPointFromRec(&hist_rec_addr[crt_n].rec[index_tmp[crt_n].index_pt[count -1 ].pt],&dbpt))
			return 0 ;   
	}
	else if (alm_rec_mode[crt_n]  == ALM_REC_SYS_INFO)
	{
		if(-1==GetPointFromRec(&shm_info_rec_addr->rec[index_tmp[crt_n].index_pt[count -1 ].pt],&dbpt))
			return 0 ;
	}
	else 
		return  0 ;
	/* page[crt_n].SelectStart = page[crt_n].SelectEnd = index_tmp[crt_n].index_pt[count-1].pt ;  */
	sel_pt[crt_n].sel[0] = index_tmp[crt_n].index_pt[count-1].pt ;
	sel_pt[crt_n].nextpos =1  ; 
	page[crt_n].isSelected = 1 ; 
	db_tab_info_tmp[crt_n].select_pt  =  dbpt ;
	//	PopupDetailInfoBox(alm_pop_shell[crt_n ], crt_n, db_tab_window_info[crt_n].select_pt);
	popupmenu  = g_object_get_data(G_OBJECT(main_w[crt_n]),"PopMenuEvt"); //PopMenuEvt
	printf("popupmenu=%x\n", popupmenu );
	if (popupmenu != NULL )
		gtk_menu_popup (GTK_MENU (popupmenu), NULL, NULL, NULL, NULL,
		bevent->button, bevent->time);
	
	AlmDspProc( crt_n);     
#ifdef  DEBUG                
	printf ("You select row :%d && count : %d \n" , rowNum, shm_rec_addr->rec[index_tmp[crt_n].index_pt[count -1 ].pt].ana_rec.rec_sn) ;
	printf ("\nPt.stn_id:%d,Pt.dev_id:%d,Pt.data_type:%d,Pt.pt_id:%d\n",
		dbpt.stn_id,dbpt.dev_id,dbpt.data_type,dbpt.pt_id);
	
#endif 
	
	return TRUE;
}

gint alarm_button_press( GtkWidget *w ,GdkEvent *event  , gpointer data)
{
	GdkPoint   pt;
	
	gint  crt_n = (gint)  data ;
	if (event->type == GDK_BUTTON_PRESS) {   
		GdkEventButton *bevent = (GdkEventButton *) event; 
		if (bevent->button == 3)  
		{ 
			pt.x =(int) (bevent->x) ;
			pt.y =(int) (bevent->y) ;
			
			alarm_button_press_ex(  w , &pt  ,  GINT_TO_POINTER (crt_n),bevent)	 ;    	
			
		}else if (bevent->button == 1) 
		{
			page[crt_n].mycur.x1 = page[crt_n].mycur.x2 = (int)(bevent->x);
			page[crt_n].mycur.y1 = page[crt_n].mycur.y2 =  (int)(bevent->y);            
		}
	}
	return TRUE;
}

gint PrevPage( GtkWidget *w , gpointer data)
{
	
	
	gint crt_n = (gint )data;
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER ) return FALSE;
	
#ifdef DEBUG
	g_message("^^^^^^PrevPage  crt_n is %d", crt_n );
#endif
	
	page[crt_n].CurPage -- ;
	if (page[crt_n].CurPage < 0  )  
	{
		gdk_beep(    ); 
		page[crt_n].CurPage  = 0 ;
		return FALSE; 
	}
	SetPageInfo (&page[crt_n]);
	
	page[crt_n].PageStart =  (page[crt_n].CurPage) * (page[crt_n].PageSize) ;
	page[crt_n].PageEnd   =   min ((page[crt_n].PageStart + page[crt_n].PageSize -1) , (page[crt_n].RecCount-1) )   ;
	SetAdjInfo(&page[crt_n],adj1[crt_n]);
	isLastPage[crt_n] = FALSE;
	AlmDspProc( crt_n );
	
	//zlb 以下关闭刷新定时器，会影响‘页报警确认’功能
// 	if (page[crt_n].CurPage != page[crt_n].PageCount-1 &&  !isLastPage[crt_n])
// 	{
// 		if (alm_timeout_id[crt_n])
// 			gtk_timeout_remove ( alm_timeout_id[crt_n]  ); 
// 		alm_timeout_id[crt_n] =0 ;
// 	}else
// 	{
// 		if (!alm_timeout_id[crt_n])
// 			alm_timeout_id[crt_n]=  gtk_timeout_add ( INTERVAL,(GtkFunction)TimeCall, GINT_TO_POINTER (crt_n) ); 
// 	}
	return TRUE;
}

gint NextPage( GtkWidget *w ,    gpointer data)
{
	gint crt_n = (gint )data ;
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER ) return FALSE ;
	
#ifdef DEBUG
	g_message("&&&&&NextPage  crt_n is %d", crt_n );
#endif
	page[crt_n].CurPage ++ ;
	if ( page[crt_n].CurPage > max( page[crt_n].PageCount -1   ,0)) 
	{
		gdk_beep(    ); 
		page[crt_n].CurPage  = page[crt_n].PageCount  -1 ;
		return FALSE;
	} 
	SetPageInfo (&page[crt_n] );
	page[crt_n].PageStart =  (page[crt_n].CurPage) * (page[crt_n].PageSize) ;
	page[crt_n].PageEnd   =   min ((page[crt_n].PageStart + page[crt_n].PageSize -1) , (page[crt_n].RecCount-1) )   ;
	SetAdjInfo(&page[crt_n],adj1[crt_n]);
	isLastPage[crt_n] = FALSE;
	
	AlmDspProc( crt_n );
	
	//zlb 以下关闭刷新定时器，会影响‘页报警确认’功能
// 	if (page[crt_n].CurPage != page[crt_n].PageCount-1 && !isLastPage[crt_n] )
// 	{    
// 		if (alm_timeout_id[crt_n])
// 			gtk_timeout_remove ( alm_timeout_id[crt_n]  ); 
// 		alm_timeout_id[crt_n] =0 ;
// 	}else  if (page[crt_n].CurPage == page[crt_n].PageCount-1  || page[crt_n].PageCount ==0 )
// 	{
// 		if (!alm_timeout_id[crt_n])
// 			alm_timeout_id[crt_n]=  gtk_timeout_add ( INTERVAL,(GtkFunction)TimeCall, GINT_TO_POINTER (crt_n) ); 
// 	}
	return TRUE;
}



void alarm_draw_area_realize(GtkWidget *widget,gpointer data)
{
	int crt_n = (gint)data ;
	
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return ; 
	alm_canvas[crt_n] = widget ;
	alm_draw_gc[crt_n] = gdk_gc_new(widget->window);
	AlmOffDrawable[crt_n] = gdk_pixmap_new(widget->window,LOCAL_WIDTH, 1600,-1);
	gdk_window_set_background(widget->window,&pixels[crt_n][BLACK]);  
	
}






gint AlarmFormQuit(GtkWidget *widget,   gpointer data)
{
	int crt_n = (gint) data ;
	/*  GetCrtNumberOfWgt(widget,&crt_n);  */
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE; 
	if(alm_timeout_id[crt_n]!=0)
	{
		gtk_timeout_remove(alm_timeout_id[crt_n]);
		alm_timeout_id[crt_n]= 0;
	}
	if (autofilter_timeout_id[crt_n]!=0)
	{
		gtk_timeout_remove(autofilter_timeout_id[crt_n]);
		autofilter_timeout_id[crt_n] = 0;
	}
	g_object_unref (AlmOffDrawable[crt_n]);     
	isLastPage[crt_n]=0;
	alm_exit_flag[crt_n] = TRUE;   
	
	gtk_widget_destroy(GTK_WIDGET(alm_pop_shell[crt_n])); 
	alm_pop_shell[crt_n] = (GtkWidget *)NULL ;
	filterDlg[crt_n] = (GtkWidget *)NULL ;
	HistDBDlg[crt_n] = NULL;
	/*	 free_alm_mutex(  crt_n ) ;  */
	return TRUE;
	
}



gint CreateDbDevB(GtkWidget *widget,   gpointer data)
{
	
	int crt_n   =   0;
	int i;
	
	//GetCrtNumberOfWgt (widget, &crt_n );
	crt_n=getCrtNumFromPointer((int)data);
	data=(gpointer)getDataFromPointer((int)data);
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE;
	
	FilterAlarm[crt_n].stn_id = (gint)data ;
	FilterAlarm[crt_n].is_change = TRUE;
	FilterAlarm[crt_n].change_flag = TRUE;
	for (i=0;i<MAX_DEV_MASK_NUM;i++)
	{
		FilterAlarm[crt_n].dev_mask[i] = 0xffffffff;
	}
	for (i=0;i<MAX_GROUP_MASK_NUM;i++)
	{
		FilterAlarm[crt_n].group_mask[i] = 0xffffffff;
	}
	//statio change init devfilterdlg////////////////////////////////////////
	if (filterDlg[crt_n])
	{
		gtk_widget_destroy(filterDlg[crt_n]);
		filterDlg[crt_n] = NULL ;
	}
	//////////////////////////////////////////////////////////////////////////
	AlmDspProc( crt_n );
	
#ifdef DEBUG	 
	printf ("cFilterAlarm[%d].stn_id = %d \n",   crt_n ,FilterAlarm[crt_n].stn_id );
#endif  
	return TRUE;
}


int  canUpdate[MAX_CRT_NUMBER] = {1,1,1};  
gint SetDataTypeMask(GtkWidget       *widget,   gpointer     data)
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
			if ( DataTypeItem[crt_n][j]!= (GtkWidget*)NULL )gtk_check_menu_item_set_active  ( GTK_CHECK_MENU_ITEM(DataTypeItem[crt_n][j]),FALSE);
			canUpdate[crt_n] = (j<MAX_DATATYPE_NUM-1?0:1) ;
		}
	}
	else if( i == MAX_DATATYPE_NUM    )   /*   all  */
	{
		for (j = 0 ; j< MAX_DATATYPE_NUM ; j++ )
		{
			if ( DataTypeItem[crt_n][j]!= (GtkWidget*)NULL )gtk_check_menu_item_set_active  (  GTK_CHECK_MENU_ITEM(DataTypeItem[crt_n][j]),TRUE);
			canUpdate[crt_n] = (j<MAX_DATATYPE_NUM-1?0:1) ;
		}
	}
	else
	{
		bull = gtk_check_menu_item_get_active  ( GTK_CHECK_MENU_ITEM(widget));  
		if (gtk_check_menu_item_get_active  ( GTK_CHECK_MENU_ITEM(widget)))
		{
			FilterAlarm[crt_n].datatype_mask = (FilterAlarm[crt_n].datatype_mask)  | (1<<(i-1)) ;  /*  Set bit N */
			FilterAlarm[crt_n].is_change = TRUE;	
			FilterAlarm[crt_n].change_flag = TRUE;
		}
		else
		{
			FilterAlarm[crt_n].datatype_mask = (FilterAlarm[crt_n].datatype_mask)  & (~(1<<(i-1))) ;   /*  Clear bit N */
			FilterAlarm[crt_n].is_change = TRUE	;
			FilterAlarm[crt_n].change_flag = TRUE;
		}
		if (canUpdate[crt_n])
			AlmDspProc( crt_n );   
		canUpdate[crt_n] = 1  ;

		#ifdef  DEBUG 
		g_message("FilterAlarm[crt_n].datatype_mask is %x  " ,FilterAlarm[crt_n].datatype_mask );
		g_message("toggle[%d] is %d  " ,i, bull );
		#endif 

		
	}
	return  TRUE;	
}

int dev_button_press(GtkWidget       *widget,  gpointer     data)
{   
	
	
	gint crt_n = (gint) data ;
	
	if (filterDlg[crt_n] == (GtkWidget *)NULL)
	{
		filterDlg[crt_n] = CreateDevFilterDlg( alm_pop_shell[crt_n] ,crt_n);
		gtk_widget_show(filterDlg[crt_n]);
	}
	else
		gtk_window_present (GTK_WINDOW(filterDlg[crt_n]));
	
	
	if (FilterAlarm[crt_n].stn_id == 0) return FALSE;
	
	
	return TRUE;
	
	
}

int attrib_button_press(GtkWidget       *widget,  gpointer     data)
{   
	
	
	gint crt_n = (gint) data ;
	
	if (AttribFilterDlg[crt_n] == (GtkWidget *)NULL)
	{
		AttribFilterDlg[crt_n] = CreateAttribFilterDlg( alm_pop_shell[crt_n] ,crt_n);
		gtk_widget_show(AttribFilterDlg[crt_n]);
	}
	else
		gtk_window_present (GTK_WINDOW(AttribFilterDlg[crt_n]));
	
	return TRUE;
	
	
}

void  cb_scroll_page(GtkAdjustment *adj , int crt_n)
{
	
	
	int maxNum ; 
	
	maxNum  =  index_tmp[crt_n].count;
	/*   printf ("adj.value = %d,adj.upper = %8.3f,adj.page_increment = %8.3f,crt = %d\n",(int)adj->value,adj->upper,adj->page_increment,crt_n); */  
	page[crt_n].PageStart = min((int)adj->value,maxNum-page[crt_n].PageSize)  ; 
	if (page[crt_n].PageStart <0 ) page[crt_n].PageStart  = 0 ;
	page[crt_n].PageEnd   =  min ((page[crt_n].PageStart + page[crt_n].PageSize -1) , (page[crt_n].RecCount-1) )   ;
	
// 	if (page[crt_n].CurPage != page[crt_n].PageCount-1 &&  !isLastPage[crt_n])
// 	{
// 		if (alm_timeout_id[crt_n])
// 			gtk_timeout_remove ( alm_timeout_id[crt_n]  ); 
// 		alm_timeout_id[crt_n] =0 ;
// 	}else if (page[crt_n].CurPage == page[crt_n].PageCount-1  || page[crt_n].PageCount ==0 )
// 	{
// 		if (!alm_timeout_id[crt_n])
// 			alm_timeout_id[crt_n]=  gtk_timeout_add ( INTERVAL,(GtkFunction)TimeCall, GINT_TO_POINTER (crt_n) );   
// 	}
	AlmDspProc( crt_n);
}


void 	ProgAlarmTab (GtkWidget *parent,	int crt_n  ,int TabMode  )
{
	if (alm_pop_shell[crt_n]==(GtkWidget*)NULL)
	{
		alm_pop_shell[crt_n] = CreateProgAlarmTab ( parent, crt_n   ) ;
		//gtk_window_set_position(GTK_WINDOW(alm_pop_shell[crt_n]),GTK_WIN_POS_CENTER_ON_PARENT);
		//gtk_widget_show(alm_pop_shell[crt_n]);
		/*alm_rec_mode[crt_n]	= ALM_REC_REAL1 ; rm by chi */
#ifdef   HISTORY_DB_ALM     
		printf(" start to  get shared memory..... \n");
		
		if (get_hdb_shm()==FALSE)
		{
			printf("OIX::can't get shared memory\n");
			shm_hdb_alm = NULL; 
			return ;
		} 
		
		shm_hdb_alm = shm_hdb_addr;   
#endif
				
		isLastPage[crt_n] = FALSE;
		if (TabMode == 1 )  /* TabMode ==1 系统综合信息*/	
			InitPageInfo(&page[crt_n] , crt_n,shm_rec_addr->head.count) ;  /* initial  set  realtime record   */
		else 
			InitPageInfo(&page[crt_n] , crt_n,shm_rec_addr->head.count) ;  /* initial  set  realtime record   */ 
		
		SetAdjInfo( &page[crt_n],adj1[crt_n]) ;  
		/*  InitAlarmFilter( &FilterAlarm[crt_n],crt_n);  */
		FilterAlarm[crt_n].is_change = TRUE; 
		/* CreateDspThread(crt_n);  */
		
		
		if (alm_timeout_id[crt_n] != 0 )
		{
			gtk_timeout_remove(alm_timeout_id[crt_n]);
			alm_timeout_id[crt_n]= 0;
		}
		alm_timeout_id[crt_n]=gtk_timeout_add ( INTERVAL,(GtkFunction)TimeCall, GINT_TO_POINTER (crt_n) ); 
		
	}else
	{
		gtk_window_present(GTK_WINDOW(alm_pop_shell[crt_n]));
	}
}


/***************************************************************************

  create a main alarm windows ; 
  entrance of process of alarm ;
  
****************************************************************************/
static  GtkWidget*
CreateProgAlarmTab (GtkWidget *parent,	int crt_n   )
{
	GtkWidget *scrolled_window,*draw_area ,*window;
	GtkWidget *vbox, *hbox;
	GtkWidget  *toolbar,*scrollbar ;
//	GtkWidget  *toolbar1 ;
	GtkWidget *hbox4;
	GdkScreen   *scr ; 	int x , y ; 

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
	
	
	gtk_window_set_default_size(GTK_WINDOW(window),/*LOCAL_WIDTH*/(int)(ALM_CANVAS_WIDTH*xscale[crt_n]), (int)(ALM_CANVAS_HEIGHT*yscale[crt_n]));
	
	
	gtk_window_get_position(GTK_WINDOW(window),&x,&y ) ; 	
#ifdef  SUN_OS
	gtk_window_move(GTK_WINDOW(window) , x, 80);
#endif
#ifdef WINDOWS_OS
//	gtk_window_move(GTK_WINDOW(window) , x/*LOCAL_WIDTH*crt_n*/,80);
#endif
	
	
	gtk_window_set_title(GTK_WINDOW(window), pConvert=_toUtf8EX(OIX_CSTR_ALARM_TAB));
	G_SAFE_FREE(pConvert);
	
	
	vbox=gtk_vbox_new(FALSE, 5);
	gtk_container_set_border_width(GTK_CONTAINER(vbox), 0);
	gtk_container_add(GTK_CONTAINER(window), vbox);
	gtk_widget_show(vbox);
	
	toolbar =  CreateAlmToolbar(window,crt_n);/*updata by hcl*/
	gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, TRUE, 0);
	gtk_widget_show(toolbar);
	gtk_signal_connect(GTK_OBJECT(window),
		"destroy",
		GTK_SIGNAL_FUNC(AlarmFormQuit),
		GINT_TO_POINTER (crt_n));
	
	
	scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
		GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
	
	draw_area = gtk_drawing_area_new();
	hbox4 =  gtk_hbox_new(FALSE, 0);
	gtk_widget_show(hbox4); 
	gtk_box_pack_start(GTK_BOX(vbox), hbox4, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(hbox4), draw_area, TRUE, TRUE, 0);
	adj1[crt_n] =(GtkAdjustment*)(gtk_adjustment_new(0.0,0.0,101.0,0.1,1.0,1.0));
	scrollbar = gtk_vscrollbar_new(GTK_ADJUSTMENT(adj1[crt_n]));
	gtk_widget_show(scrollbar)   ;
	g_object_set_data(G_OBJECT(window),"scrollbar",scrollbar);
	gtk_box_pack_start(GTK_BOX(hbox4), scrollbar, FALSE, FALSE, 0);
	/* gtk_widget_set_usize(draw_area, 1024, 400); */
	//gtk_widget_set_usize(draw_area, (int)(ALM_CANVAS_WIDTH*xscale[crt_n]), (int)(ALM_CANVAS_HEIGHT*yscale[crt_n]));
	g_signal_connect(G_OBJECT(adj1[crt_n]),"value_changed",G_CALLBACK(cb_scroll_page),GINT_TO_POINTER (crt_n));
	
	
	gtk_widget_set_events(draw_area, GDK_EXPOSURE_MASK 
		| GDK_SCROLL_MASK            
		| GDK_LEAVE_NOTIFY_MASK
		| GDK_BUTTON_PRESS_MASK
		| GDK_BUTTON_RELEASE_MASK
		| GDK_POINTER_MOTION_MASK );
	
	
	gtk_signal_connect (GTK_OBJECT (draw_area),"expose_event",GTK_SIGNAL_FUNC(alarm_expose),GINT_TO_POINTER (crt_n)) ;
	gtk_signal_connect (GTK_OBJECT (draw_area),"scroll-event",GTK_SIGNAL_FUNC(alarm_scroll_event),GINT_TO_POINTER(crt_n)) ;    
	gtk_signal_connect (GTK_OBJECT (draw_area),"motion_notify_event",GTK_SIGNAL_FUNC(alarm_motion),GINT_TO_POINTER(crt_n)) ;
	gtk_signal_connect (GTK_OBJECT (draw_area), "realize", GTK_SIGNAL_FUNC (alarm_draw_area_realize), GINT_TO_POINTER(crt_n));
	gtk_signal_connect (GTK_OBJECT (draw_area), "button_press_event", GTK_SIGNAL_FUNC (alarm_button_press),GINT_TO_POINTER(crt_n) );    
	gtk_signal_connect (GTK_OBJECT (draw_area), "button_release_event", GTK_SIGNAL_FUNC (alarm_button_release),GINT_TO_POINTER(crt_n) );    
	gtk_signal_connect (GTK_OBJECT (draw_area),"size_allocate",GTK_SIGNAL_FUNC(alarm_canvas_allocate),GINT_TO_POINTER(crt_n)) ;
	/*   gtk_scrolled_window_add_with_viewport(GTK_CONTAINER(scrolled_window), draw_area);*/
	gtk_widget_realize(GTK_WIDGET( draw_area)) ;
	gtk_widget_show(draw_area);
	
	
	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, TRUE, 0);
	gtk_widget_show(hbox);
	
	
	gtk_widget_show(window);
	
	
	/*	init_alm_mutex(  crt_n ) ;  */
	return  window ;
	
	
}
void  SetHistAlmCName(GtkWidget * widget, gpointer data)
{
	int crt_n ;
	int i = (gint) data ;
	GetCrtNumberOfWgt (widget, &crt_n );
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return ;
	g_object_set_data(G_OBJECT(widget),"iNameMode", GINT_TO_POINTER (i));  
	iHisNameMode[crt_n]  = i+1 ; 
	g_message ("iNameMode = %d \n " , i )  ;
	
}

void SetHistAlmBtn (GtkWidget * widget, gpointer data)
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
		alm_rec_mode[crt_n]	= ALM_REC_REAL1;
		/* InitAlarmFilter(&FilterAlarm[crt_n],crt_n);  */
		FilterAlarm[crt_n].phead =  &shm_rec_addr->head;
		FilterAlarm[crt_n].pRec   = (REC_ENTRY_UNION* )&shm_rec_addr->rec;	        
		InitPageInfo(&page[crt_n] , crt_n,shm_rec_addr->head.count) ;  /* initial  set  realtime record   */
		SetAdjInfo( &page[crt_n],adj1[crt_n]) ;  
		gtk_window_set_title(GTK_WINDOW(alm_pop_shell[crt_n]), pConvert=_toUtf8EX(OIX_CSTR_ALARM_TAB));
		G_SAFE_FREE(pConvert);

		if (!alm_timeout_id[crt_n])
			alm_timeout_id[crt_n]=  gtk_timeout_add ( INTERVAL,(GtkFunction)TimeCall, GINT_TO_POINTER (crt_n) ); 
		break;   
	case 1:    /*ALM_HIST_FILE*/
		alm_rec_mode[crt_n]	= ALM_REC_HISTRY1;
		file_mode[crt_n]       = ALM_HIST_FILE; 
		page[crt_n].RecCount = 0 ;
		sprintf(dirName , "%s*.REC" , EVENT_LOG_PATH );
		PopFileSelectionDlg(main_w[crt_n],crt_n, pConvert=_toUtf8EX(OIX_STR_ALARM_FILE_SELECT_BOX),dirName); 
		G_SAFE_FREE(pConvert);

		gdk_beep();
		break;
		
	case 2 :    /*综合信息*/
		alm_rec_mode[crt_n]	= ALM_REC_SYS_INFO;
		
		FilterAlarm[crt_n].phead =  &shm_info_rec_addr->head;
		FilterAlarm[crt_n].pRec   =(REC_ENTRY_UNION*) &shm_info_rec_addr->rec;	        
		InitPageInfo(&page[crt_n] , crt_n,shm_info_rec_addr->head.count) ;  /* initial  set  realtime record   */
		SetAdjInfo( &page[crt_n],adj1[crt_n]) ;  
		gtk_window_set_title(GTK_WINDOW(alm_pop_shell[crt_n]), pConvert=_toUtf8EX(OIX_CSTR_SYSMSG));
		G_SAFE_FREE(pConvert);
		if (!alm_timeout_id[crt_n])
			alm_timeout_id[crt_n]=  gtk_timeout_add ( INTERVAL,(GtkFunction)TimeCall, GINT_TO_POINTER(crt_n) ); 
		break; 
		
	case  3:  /*HDB_ALARM*/
		alm_rec_mode[crt_n]	= ALM_REC_HISTRYDB;
		if  (shm_hdb_alm->connflag_al  == 1   )  /*chichi*/
		{
			if (HistDBDlg[crt_n] == (GtkWidget *)NULL)
			{
				HistDBDlg[crt_n] = CreateHistDBAlmDlg ( alm_pop_shell[crt_n] ,crt_n);
				gtk_widget_show(HistDBDlg[crt_n]);
			}
			else
				gtk_window_present (GTK_WINDOW(HistDBDlg[crt_n]));
		}else
		{
			PopupMessageBox(alm_pop_shell[crt_n] ,MSG_CONNECT_DATABASE_ERROR);	 
		}
		break;
	case 4: /*历史文件搜索*/
		alm_rec_mode[crt_n]	= ALM_REC_FILE_SEARCH;
		if (HistFileDBDlg[crt_n] == (GtkWidget *)NULL)
		{
			HistFileDBDlg[crt_n] = CreateHistDBFileAlmDlg ( alm_pop_shell[crt_n] ,crt_n);
			gtk_widget_show(HistFileDBDlg[crt_n]);
		}
		else
			gtk_window_present (GTK_WINDOW(HistFileDBDlg[crt_n]));		
		break; 
	}  
	
	FilterAlarm[crt_n].is_change = TRUE; 
	AlmDspProc(crt_n) ;
	
}


void CreateAllNonDevBtn(GtkWidget *box2)
{
	int crt_n=0;
	
	GtkWidget *hbox;
	
	hbox =gtk_hbox_new (FALSE, 10); 	
	
	
}
int  SetFileterToEntry(GtkWidget *entry, time_t *TimeRes)
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
int  SetTimeFilter  (GtkWidget *entry, time_t *TimeRes)
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
int HistDBAlarmOK(GtkWidget *w , gpointer data)
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
	iMode  =  iHisNameMode[crt_n] ;
	ss1  = (gchar*)gtk_entry_get_text(GTK_ENTRY(startEntry));
	if (strcmp(ss1,"")!=0)
	{
		strcpy(shm_hdb_alm->data_al.starttime, pConvert=_fromUtf8( ss1));
		G_SAFE_FREE(pConvert);
	}
	ss1  = (gchar*)gtk_entry_get_text(GTK_ENTRY(endEntry));
	if (strcmp(ss1,"")!=0)
	{
		strcpy(shm_hdb_alm->data_al.endtime, pConvert=_fromUtf8( ss1));
		G_SAFE_FREE(pConvert);
	}
	
	ss1  = (gchar*)gtk_entry_get_text(GTK_ENTRY(cNameEntry));
	strcpy(shm_hdb_alm->data_al.cname, pConvert=_fromUtf8( ss1));
	G_SAFE_FREE(pConvert);
	
	shm_hdb_alm->data_al.cname_type  = 	iHisNameMode[crt_n] ; 
	shm_hdb_alm->data_al.queryflag  = 1 ;  /* start query !!! */
	shm_hdb_alm->data_al.querytype  = 0 ;
	
	HistDBAlmDlgDelete( w ,   data);  
	alm_rec_mode[crt_n]   =  ALM_REC_HISTRYDB;
	
	page[crt_n].PageStart =  0 ;             
	page[crt_n].RecCount = 0;  
	FilterAlarm[crt_n].is_change = TRUE;
	return FALSE ;
	
	
	//Fail:
	PopupMessageBox(alm_pop_shell[crt_n] , MSG_INVALID_DATA_FORMATE); 
	return TRUE ;
	//#endif
	alm_rec_mode[0]   =  ALM_REC_HISTRYDB;
	return TRUE ;
}

int DevFilterDlgDelete(GtkWidget *w , gpointer data) ;
int DevFilterDlgOK(GtkWidget *w , gpointer data)
{
	
	int crt_n,i;
	char *ss1 ;
	GtkWidget *startEntry ,	*endEntry , *searchEntry ,  *logNameEntry ;
	gchar *pConvert=0;
	//GetCrtNumberOfWgt (w, &crt_n );
	crt_n=(int)data;
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return TRUE ;
	FilterAlarm[crt_n].dev_mask[0] = devMask[crt_n];
	for (i=0;i<MAX_GROUP_MASK_NUM;i++)
	{
		FilterAlarm[crt_n].group_mask[i] = groupMask[crt_n][i];
	}
	startEntry  = g_object_get_data(G_OBJECT(filterDlg[crt_n]),"startWgt");
	endEntry   = g_object_get_data(G_OBJECT(filterDlg[crt_n]),"endWgt");
	searchEntry = g_object_get_data(G_OBJECT(filterDlg[crt_n]),"searchWgt");
	logNameEntry = g_object_get_data(G_OBJECT(filterDlg[crt_n]),"logNameWgt");
	if (SetTimeFilter  ( startEntry, &FilterAlarm[crt_n].start_time) ==FALSE)  goto Fail  ;
	if (SetTimeFilter  ( endEntry,   &FilterAlarm[crt_n].end_time)	  ==FALSE )   goto Fail ; 
	if (FilterAlarm[crt_n].start_time  ==  FilterAlarm[crt_n].end_time )
		FilterAlarm[crt_n].start_time =  FilterAlarm[crt_n].end_time = -1 ;
	ss1  =(char*) gtk_entry_get_text(GTK_ENTRY(searchEntry));
	/* if (strcmp(ss1,"")!=0)*/
	strcpy((char *)(&FilterAlarm[crt_n].search_str), pConvert=_fromUtf8( ss1));
	G_SAFE_FREE(pConvert);
	ss1  = (char *)gtk_entry_get_text(GTK_ENTRY(logNameEntry));
	/*if (strcmp(ss1,"")!=0) */
	strcpy((char*)(&FilterAlarm[crt_n].LogicName), pConvert=_fromUtf8( ss1));
	G_SAFE_FREE(pConvert);
	
	FilterAlarm[crt_n].is_change = TRUE;
	FilterAlarm[crt_n].change_flag = TRUE;
	//	SaveCustomFilterDef(&FilterAlarm[crt_n],crt_n); //add by dm
	DevFilterDlgDelete( w ,   data);  
	AlmDspProc( crt_n);
	return FALSE ;
	
Fail:
	PopupMessageBox(alm_pop_shell[crt_n] , MSG_INVALID_DATA_FORMATE); 
	return TRUE ;
	
}

int HistDBAlmDlgDelete (GtkWidget *w , gpointer data)
{
	int  crt_n = (gint)data;
	GetCrtNumberOfWgt (w, &crt_n );
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE ;
	gtk_widget_destroy(HistDBDlg[crt_n] );
	HistDBDlg[crt_n]  = (GtkWidget*)NULL ;
	
	return TRUE;
}

int DevFilterDlgDelete(GtkWidget *w , gpointer data)
{

	int  crt_n = (gint)data;
	GetCrtNumberOfWgt (w, &crt_n );
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE ;
	gtk_widget_destroy(filterDlg[crt_n]);
	filterDlg[crt_n] = (GtkWidget*)NULL ;
	return TRUE;
}

int AttribFilterDlgDelete(GtkWidget *w , gpointer data)
{
	
	int  crt_n = (gint)data;
	GetCrtNumberOfWgt (w, &crt_n );
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE ;
	gtk_widget_destroy(AttribFilterDlg[crt_n]);
	AttribFilterDlg[crt_n] = (GtkWidget*)NULL ;
	return TRUE;
}

int CustomFilterDlgDelete(GtkWidget *w , gpointer data)
{
	int  crt_n = (gint)data;
	GetCrtNumberOfWgt (w, &crt_n );
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE ;
	gtk_widget_destroy(SetFilterDlg[crt_n]);
	SetFilterDlg[crt_n] = (GtkWidget*)NULL ;
	return TRUE;
}

gint doPrint (GtkWidget       *widget,   gpointer     data)
{
	int crt_n=(gint)data;
	
	if(!CheckUserPrivilege(cur_usr_name, PRIV_PRINT))
	{
		PopupMessageBox ( alm_pop_shell[crt_n], MSG_RPT_PRINTING  ); 
		return  TRUE;
	}
	
	
	//	if(page[crt_n].isSelected==0 )
	//	PopupMessageBox ( alm_pop_shell[crt_n], MSG_SELECT_PRINT_RANGE); 
	//	else  
	{
		AlmDspProcE( AlmOffDrawable[crt_n] ,crt_n,TRUE);
		
		PrintList(alm_pop_shell[crt_n],crt_n,ALM_LIST,AlarmPrintFile); 
	}
	return  TRUE;
}



int doDSPAlarmAck(GtkWidget       *widget,   gpointer     data)
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
	
	/*for (i = 0 , j= 0 ; i < index_fault[crt_n].count;j++, i++) */
	
	for (i =0 ,j = 0 ; j<= MIN(index_fault[crt_n].count,15) ; j++  )
	{
		
		if(-1==GetPointFromRec(&shm_rec_addr->rec[index_fault[crt_n].index_pt[i].pt],&dbpt))
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
	
	ProgAlarmAckBrdcst ( alm_msg ,   send_num) ;  /* broad cast !!! */
	return TRUE;
}

gint doAlarmAck(GtkWidget       *widget,   gpointer     data)
{  int crt_n = (int)data ;
doAlarmAckEx( widget,crt_n, page[crt_n].PageStart ,page[crt_n].PageEnd  ,FALSE ) ;
AlmDspProc(crt_n) ;
return TRUE;
}

int ComposeAlmMsg (int crt_n ,COMMAND_MSG	alm_msg[] ,int  i  ,  int iRec , struct  tm      *devtime   )
{
	POINTER dbpt ; 
	int   send_num = 0 ;
	USHORT  noAck;

	switch(alm_rec_mode[crt_n] )
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
		if(-1==GetPointFromRec(&hist_rec_addr[crt_n].rec[iRec],&dbpt))
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


gint doAlarmAckRange(GtkWidget       *widget,   gpointer     data)
{  int crt_n = (int)data ;

if (!page[crt_n].isSelected)
{
	
	PopupMessageBox(alm_pop_shell[crt_n],MSG_SELECT_ACK_RANGE);
	return FALSE ;
}
/*printf ("$$$$$$select start =%d , end = %d \n" ,page[crt_n].SelectStart ,page[crt_n].SelectEnd ) ;*/
doAlarmAckEx( widget,  crt_n ,  0 ,0  ,TRUE)  ;
MySleep(1000) ;
AlmDspProc(crt_n) ;
return TRUE;
}

gint doAlarmAckEx(GtkWidget       *widget, int crt_n ,  int startI ,int  endI  ,BOOL isRangeAck )
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
 		PopupMessageBox ( alm_pop_shell[crt_n], MSG_ALARM_ACK);
 		return 0;
 	}
	if (isRangeAck)
	{
		for (i =j = 0   ; i< sel_pt[crt_n].nextpos ; i++  )
		{
			if (!ComposeAlmMsg (  crt_n , 	alm_msg , j  ,sel_pt[crt_n].sel[i] ,&devtime     ))
				continue ; 
			send_num++ ;
			j++   ;      
		}
	}else
	{	
		
		for (i =0 ,j = startI ; j<= endI ; j++  )
		{
			if (!ComposeAlmMsg (  crt_n , 	alm_msg , i  ,index_tmp[crt_n].index_pt[j].pt,&devtime   ) )
				continue;
			send_num++ ; 
			i++    ;
		}
	}
	
	if ( send_num>0)  
		ProgAlarmAckBrdcst ( alm_msg ,   send_num) ;  /* broad cast !!! */
	return TRUE;
}


gint isDisplayMore(GtkWidget       *widget,   gpointer     data)
{  
	GtkWidget *window =  (GtkWidget*)data;
	GtkWidget *frame = g_object_get_data(G_OBJECT(window),"frameWgt");
	if (gtk_toggle_button_get_active  (GTK_TOGGLE_BUTTON(widget)))
		gtk_widget_show(frame);
	else
		gtk_widget_hide(frame);
	return TRUE;
}


const char *sName[] = {OIX_STR_CHINESE,OIX_STR_FORGEIN};
static GtkWidget* CreateHistDBAlmDlg(GtkWidget *parent ,int crt_n)
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
		GTK_SIGNAL_FUNC (HistDBAlmDlgDelete), GINT_TO_POINTER (crt_n)); 
	
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
		items = gtk_menu_item_new_with_label(pConvert=_toUtf8EX((char*)sName[i]));
		G_SAFE_FREE(pConvert);
		gtk_widget_show( items);
		gtk_container_add(GTK_CONTAINER(menu),items);
		gtk_signal_connect (GTK_OBJECT (items), "activate",GTK_SIGNAL_FUNC (SetHistAlmCName), GINT_TO_POINTER(i));
	}   
	
	
	gtk_option_menu_set_menu(GTK_OPTION_MENU(optionmenu),menu);
	
	
	/***********************************************************************************/
	entry3 = gtk_entry_new ();
	gtk_widget_show (entry3);
	gtk_table_attach (GTK_TABLE (table1), entry3, 1, 2, 2, 3,
		(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
		(GtkAttachOptions) (0), 0, 0);
	
	/*add by hcl*/
	if(strlen(shm_hdb_alm->data_al.cname)>0)
	{
		gtk_entry_set_text(GTK_ENTRY(entry3), pConvert=_toUtf8(shm_hdb_alm->data_al.cname));
		G_SAFE_FREE(pConvert);
	}
	
	g_object_set_data(G_OBJECT(windows1),"cNameWgt",entry3);
	
	if(strlen(shm_hdb_alm->data_al.starttime)>0)
	{
		gtk_entry_set_text(GTK_ENTRY(entry1), pConvert=_toUtf8(shm_hdb_alm->data_al.starttime));
		G_SAFE_FREE(pConvert);
	}
	else
		gtk_entry_set_text(GTK_ENTRY(entry1),timer);
	
	if(strlen(shm_hdb_alm->data_al.endtime)>0)
	{
		gtk_entry_set_text(GTK_ENTRY(entry2), pConvert=_toUtf8(shm_hdb_alm->data_al.endtime));
		G_SAFE_FREE(pConvert);
	}
	else
		gtk_entry_set_text(GTK_ENTRY(entry2),timer);
	
	
	
	hbox1 = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox1);
	gtk_box_pack_start (GTK_BOX (vbox1), hbox1, FALSE, FALSE, 0);
	
	button1 = gtk_button_new_with_label(pConvert= _toUtf8EX(OIX_CSTR_CANCEL));//gtk_button_new_from_stock ("gtk-cancel");
	G_SAFE_FREE(pConvert);

	gtk_widget_show (button1);
	gtk_box_pack_end (GTK_BOX (hbox1), button1, FALSE, FALSE, 0);
	
	button2 = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_APPLY));
	G_SAFE_FREE(pConvert);
	//gtk_button_new_from_stock ("gtk-apply");
	gtk_widget_show (button2);
	gtk_box_pack_end (GTK_BOX (hbox1), button2, FALSE, FALSE, 0);
	
	gtk_signal_connect (GTK_OBJECT (button1), "clicked",GTK_SIGNAL_FUNC (HistDBAlmDlgDelete), windows1);  
	gtk_signal_connect (GTK_OBJECT (button2), "clicked",GTK_SIGNAL_FUNC (HistDBAlarmOK), windows1);
	
	gtk_window_set_position(GTK_WINDOW(windows1),GTK_WIN_POS_CENTER);  
	iHisNameMode[crt_n] =  1 ;  
	return  windows1 ;
	
	
}

static void
SetDevMask_a (GtkCellRendererToggle *cell,
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
	gtk_tree_model_get_iter (GTK_TREE_MODEL(storeDev_a[crt_n]), &iterDev, path);
	gtk_tree_model_get (GTK_TREE_MODEL(storeDev_a[crt_n]), &iterDev, COLUMN_CHECKED, &fixed, -1);
	if (!fixed)
	{
		devMask[crt_n] = devMask[crt_n]  | (1<<i);
		groupMask[crt_n][i] = 0xFFFFFFFF;
		if (i==curDev_a&&gtk_tree_model_get_iter_first(GTK_TREE_MODEL(storeGroup_a[crt_n]),&iterGroup))
		{
			do 
			{
				gtk_list_store_set (storeGroup_a[crt_n], &iterGroup, COLUMN_CHECKED, TRUE, -1);
			} while (gtk_tree_model_iter_next(GTK_TREE_MODEL(storeGroup_a[crt_n]),&iterGroup));
		}
	}
	else
	{
		devMask[crt_n] = devMask[crt_n]  & (~(1<<i)); /*  Clear bit N */  
		groupMask[crt_n][i] = 0;
		if (i==curDev_a&&gtk_tree_model_get_iter_first(GTK_TREE_MODEL(storeGroup_a[crt_n]),&iterGroup))
		{
			do 
			{
				gtk_list_store_set (storeGroup_a[crt_n], &iterGroup, COLUMN_CHECKED, FALSE, -1);
			} while (gtk_tree_model_iter_next(GTK_TREE_MODEL(storeGroup_a[crt_n]),&iterGroup));
		}
	}

	fixed ^= 1;
	gtk_list_store_set (storeDev_a[crt_n], &iterDev, COLUMN_CHECKED, fixed, -1);
	//DevClicked_a(GTK_TREE_VIEW(gtk_tree_view_new_with_model(GTK_TREE_MODEL(storeDev_a[crt_n]))),data);
	gtk_tree_path_free (path);  
}

static void
SetGroupMask_a (GtkCellRendererToggle *cell,
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
	
	gtk_tree_model_get_iter (GTK_TREE_MODEL(storeGroup_a[crt_n]), &iter, path);
	gtk_tree_model_get (GTK_TREE_MODEL(storeGroup_a[crt_n]), &iter, COLUMN_CHECKED, &fixed, -1);
	if (!fixed)
		groupMask[crt_n][curDev_a] = groupMask[crt_n][curDev_a]  | (1<<i);
	else
		groupMask[crt_n][curDev_a] = groupMask[crt_n][curDev_a]  & (~(1<<i)); /*  Clear bit N */  
	
	fixed ^= 1;
	gtk_list_store_set (storeGroup_a[crt_n], &iter, COLUMN_CHECKED, fixed, -1);
	gtk_tree_path_free (path);  
}

gint SetDevMaskAllOrNone(GtkWidget *widget , gpointer data)
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
			if (alarmStnDef[i].Id==FilterAlarm[crt_n].stn_id)
			{
				stn_idx=i;
				break;
			}
			
		}
	}

	if ((int)data == SHOW_NONE)  /* show all !!!*/
	{
		if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(storeDev_a[crt_n]),&iter))
		{
			gtk_list_store_set (storeDev_a[crt_n], &iter, COLUMN_CHECKED, FALSE, -1);
			devMask[crt_n] = devMask[crt_n]  & (~(1<<0));
		}
		for (j = 1 ; j <= alarmStnDef[stn_idx].dev_num ; j++)
		{			
			if (gtk_tree_model_iter_next(GTK_TREE_MODEL(storeDev_a[crt_n]),&iter))
			{
				gtk_list_store_set (storeDev_a[crt_n], &iter, COLUMN_CHECKED, FALSE, -1);
				devMask[crt_n] = devMask[crt_n]  & (~(1<<j));
			}
		}
		if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(storeGroup_a[crt_n]),&iter))
		{
			gtk_list_store_set (storeGroup_a[crt_n], &iter, COLUMN_CHECKED, FALSE, -1);
		}
		for (j=1;j<=(int)alarmStnDef[stn_idx].dev[curDev_a+1].group_num;j++)
		{
			if (gtk_tree_model_iter_next(GTK_TREE_MODEL(storeGroup_a[crt_n]),&iter))
			{
				gtk_list_store_set (storeGroup_a[crt_n], &iter, COLUMN_CHECKED, FALSE, -1);
			}
		}
		for (j=0;j<MAX_GROUP_MASK_NUM;j++)
		{
			groupMask[crt_n][j] = 0;
		}
	}else if ((int)data ==SHOW_ALL) /*  show none !!!*/
	{
		if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(storeDev_a[crt_n]),&iter))
		{
			gtk_list_store_set (storeDev_a[crt_n], &iter, COLUMN_CHECKED, TRUE, -1);
			devMask[crt_n] = devMask[crt_n]  | (1<<0);
		}
		for (j = 1 ; j <= alarmStnDef[stn_idx].dev_num ; j++)
		{
			if (gtk_tree_model_iter_next(GTK_TREE_MODEL(storeDev_a[crt_n]),&iter))
			{
				gtk_list_store_set (storeDev_a[crt_n], &iter, COLUMN_CHECKED, TRUE, -1);  
				devMask[crt_n] = devMask[crt_n]  | (1<<j);
			}
		}
		if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(storeGroup_a[crt_n]),&iter))
		{
			gtk_list_store_set (storeGroup_a[crt_n], &iter, COLUMN_CHECKED, TRUE, -1);
		}
		for (j=1;j<=(int)alarmStnDef[stn_idx].dev[curDev_a+1].group_num;j++)
		{
			if (gtk_tree_model_iter_next(GTK_TREE_MODEL(storeGroup_a[crt_n]),&iter))
			{
				gtk_list_store_set (storeGroup_a[crt_n], &iter, COLUMN_CHECKED, TRUE, -1);
			}
		}
		for (j=0;j<MAX_GROUP_MASK_NUM;j++)
		{
			groupMask[crt_n][j] = 0xffffffff;
		}
	}
	return TRUE;
}
gboolean DevClicked_a(GtkTreeView *treeview,
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
	int stn_idx,station_num;

	gint crt_n = (gint) data ;
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE;
	gtk_tree_view_get_cursor(treeview,&path,NULL);
	path_num = gtk_tree_path_get_indices(path);
	curDev_a = *path_num;
	model = gtk_tree_view_get_model(treeview);
	gtk_tree_model_get_iter(model,&iter,path);
	gtk_tree_model_get(model,&iter,COLUMN_CHECKED,&fixed,-1);
	gtk_tree_model_get (model, &iter, COLUMN_NAME, &devName, -1);
	gtk_list_store_clear(storeGroup_a[crt_n]);

	GetStationNum(&station_num);
	stn_idx=0;
	if (station_num>=1)
	{
		for(i=1;i<=station_num;i++) 
		{
			if (alarmStnDef[i].Id==FilterAlarm[crt_n].stn_id)
			{
				stn_idx=i;
				break;
			}
			
		}
	}

	if (fixed)
	{
		for(i=1;i<=alarmStnDef[stn_idx].dev_num;i++)
		{
			if (!strcmp(alarmStnDef[stn_idx].dev[i].CName, pConvert1=_fromUtf8(devName)))
			{
				for (j=1;j<=(int)alarmStnDef[stn_idx].dev[i].group_num;j++)
				{
					if(strcmp(alarmStnDef[stn_idx].dev[i].group[j].CName,"")!=0
						&&alarmStnDef[stn_idx].dev[i].group[j].isFilter[crt_n] == FALSE)
					{ 
						gtk_list_store_append (storeGroup_a[crt_n], &iter);
						if (groupMask[crt_n][curDev_a] & (0X1<<(j-1)) ) 
							groupIsChecked = TRUE;  
						else
							groupIsChecked = FALSE;
						gtk_list_store_set (storeGroup_a[crt_n], &iter,COLUMN_CHECKED, groupIsChecked,COLUMN_NAME,
							pConvert=_toUtf8(alarmStnDef[stn_idx].dev[i].group[j].CName),-1);
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
CreateDevFilterDlg(GtkWidget *parent ,int crt_n)
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
	int station_num,stn_idx;
	gchar *pConvert=0;
//	GtkTreeSelection *treeSelection;
	BOOL	devIsChecked;
	char *sName[] ={OIX_CSTR_ALM_REC_NON ,OIX_CSTR_ALM_REC_ALL      };
	devMask[crt_n] =  FilterAlarm[crt_n].dev_mask[0] ;
	for (i=0;i<MAX_GROUP_MASK_NUM;i++)
	{
		groupMask[crt_n][i] = FilterAlarm[crt_n].group_mask[i];
	}
	
	newtime = time(NULL);
	devtime = *localtime( &newtime);
	
	sprintf (timer , "%4d-%02d-%02d %02d:%02d:%02d" ,devtime.tm_year+1900,
	devtime.tm_mon+1 ,devtime.tm_mday , devtime.tm_hour, devtime.tm_min ,devtime.tm_sec) ;
	
	
	windows1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(windows1), pConvert=_toUtf8EX(OIX_STR_DEV_FILTER));
	G_SAFE_FREE(pConvert);
	gtk_window_set_transient_for(GTK_WINDOW(windows1), GTK_WINDOW(parent)); 
	
	gtk_signal_connect (GTK_OBJECT (windows1), "delete-event", 
	GTK_SIGNAL_FUNC (DevFilterDlgDelete),  GINT_TO_POINTER(crt_n)); 
	
	gtk_window_set_destroy_with_parent(GTK_WINDOW(windows1),TRUE);
	gtk_window_set_position(GTK_WINDOW(windows1),GTK_WIN_POS_CENTER);  
	gtk_widget_show (windows1);
	vbox1 = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox1);
	gtk_container_add (GTK_CONTAINER (windows1), vbox1);
	printf ("\n ######stn.id= %d",FilterAlarm[crt_n].stn_id);

	hbox = gtk_hbox_new (FALSE, 0);
 	gtk_widget_show (hbox);
	storeDev_a[crt_n] = gtk_list_store_new (NUM_COLUMNS,G_TYPE_BOOLEAN,G_TYPE_STRING);
	storeGroup_a[crt_n] = gtk_list_store_new (NUM_COLUMNS,G_TYPE_BOOLEAN,G_TYPE_STRING);

	GetStationNum(&station_num);
	stn_idx=0;
	if (station_num>=1)
	{
		for(i=1;i<=station_num;i++) 
		{
			if (alarmStnDef[i].Id==FilterAlarm[crt_n].stn_id)
			{
				stn_idx=i;
				break;
			}
		
		}
	}
	for(i=1;i<=alarmStnDef[stn_idx].dev_num;i++)
	{
		if(strcmp(alarmStnDef[stn_idx].dev[i].CName,"")!=0)
		{ 
			if (FilterAlarm[crt_n].dev_mask[0] & (0X1<<(i-1)) ) 
				devIsChecked = TRUE;  
			else
				devIsChecked = FALSE;
			gtk_list_store_append (storeDev_a[crt_n], &iter);
			gtk_list_store_set (storeDev_a[crt_n], &iter,COLUMN_CHECKED, devIsChecked,COLUMN_NAME, 
				pConvert=_toUtf8(alarmStnDef[stn_idx].dev[i].CName),-1);
			G_SAFE_FREE(pConvert);
		}
	}

	listDev = gtk_tree_view_new_with_model (GTK_TREE_MODEL(storeDev_a[crt_n]));
	//treeSelection = gtk_tree_view_get_selection(GTK_TREE_VIEW(listDev));
	//gtk_tree_selection_set_mode(treeSelection,GTK_SELECTION_SINGLE);
	gtk_signal_connect(GTK_OBJECT(listDev),"cursor-changed",G_CALLBACK(DevClicked_a),GINT_TO_POINTER(crt_n));
	g_object_unref (storeDev_a[crt_n]);
	renderer = gtk_cell_renderer_toggle_new ();
	g_signal_connect (renderer, "toggled",G_CALLBACK (SetDevMask_a), GINT_TO_POINTER(crt_n));
	column = gtk_tree_view_column_new_with_attributes (NULL,renderer,"active", COLUMN_CHECKED,NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW(listDev), column);
	renderer = gtk_cell_renderer_text_new ();
	column = gtk_tree_view_column_new_with_attributes (NULL,renderer,"text",COLUMN_NAME,NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW(listDev), column);
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(listDev),FALSE);
	gtk_box_pack_start (GTK_BOX (hbox), listDev, FALSE, FALSE, 0);

	listGroup = gtk_tree_view_new_with_model(GTK_TREE_MODEL(storeGroup_a[crt_n]));
	g_object_unref (storeGroup_a[crt_n]);
	renderer = gtk_cell_renderer_toggle_new ();
	g_signal_connect (renderer, "toggled",G_CALLBACK (SetGroupMask_a), GINT_TO_POINTER(crt_n));
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
	
	devNone = gtk_button_new_with_label(pConvert=_toUtf8(_CS_(sName[0])));
	G_SAFE_FREE(pConvert);
	gtk_widget_show(devNone);
	gtk_box_pack_start (GTK_BOX (vbox1),  devNone, FALSE, FALSE, 0);
	gtk_signal_connect (GTK_OBJECT (devNone), "clicked",GTK_SIGNAL_FUNC (SetDevMaskAllOrNone),GINT_TO_POINTER( SHOW_NONE));
	
	devAll = gtk_button_new_with_label(pConvert=_toUtf8(_CS_(sName[1])));
	G_SAFE_FREE(pConvert);
	gtk_widget_show(devAll);
	gtk_box_pack_start (GTK_BOX (vbox1),  devAll, FALSE, FALSE, 0);
	gtk_signal_connect (GTK_OBJECT (devAll), "clicked",GTK_SIGNAL_FUNC (SetDevMaskAllOrNone),GINT_TO_POINTER( SHOW_ALL));
	
	frame1 = gtk_frame_new (NULL);
	/*  gtk_widget_show (frame1);*/
	g_object_set_data(G_OBJECT(windows1),"frameWgt",frame1);
	gtk_box_pack_start (GTK_BOX (vbox1), frame1, FALSE, FALSE, 0);
	gtk_widget_show(frame1);
	
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
	SetFileterToEntry  ( entry1, &FilterAlarm[crt_n].start_time)    ;
	
	
	entry2 = gtk_entry_new ();
	gtk_widget_show (entry2);
	gtk_table_attach (GTK_TABLE (table1), entry2, 1, 2, 1, 2,
	(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
	(GtkAttachOptions) (0), 0, 0);
	
	g_object_set_data(G_OBJECT (windows1),"endWgt",entry2);
	gtk_entry_set_max_length(GTK_ENTRY(entry2),20);
	SetFileterToEntry  ( entry2, &FilterAlarm[crt_n].end_time)    ;
	
	entry3 = gtk_entry_new ();
	gtk_widget_show (entry3);
	gtk_table_attach (GTK_TABLE (table1), entry3, 1, 2, 2, 3,
	(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
	(GtkAttachOptions) (0), 0, 0);
	
	
	g_object_set_data(G_OBJECT(windows1),"searchWgt",entry3);
	gtk_entry_set_max_length(GTK_ENTRY(entry3),MAX_SEARCH_NAME_SIZE);
	
	if (FilterAlarm[crt_n].search_str[0] !=0)  
	{
		gtk_entry_set_text(GTK_ENTRY(entry3),pConvert=_toUtf8(FilterAlarm[crt_n].search_str));
		G_SAFE_FREE(pConvert);
	}
	
	entry4 = gtk_entry_new ();
	gtk_widget_show (entry4);
	gtk_table_attach (GTK_TABLE (table1), entry4, 1, 2, 3, 4,
	(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
	(GtkAttachOptions) (0), 0, 0);
	
	g_object_set_data(G_OBJECT(windows1),"logNameWgt",entry4);
	gtk_entry_set_max_length(GTK_ENTRY(entry4),50);
	
	if (FilterAlarm[crt_n].LogicName[0] !=0)  
	{
		gtk_entry_set_text(GTK_ENTRY(entry4), pConvert=_toUtf8(FilterAlarm[crt_n].LogicName));
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
	
	//zlb 由于其往往在不知道情况下隐藏过滤条件，对运行造成影响，去掉隐藏性，一直保持可见
// 	checkbutton3 = gtk_check_button_new_with_label (pConvert=_toUtf8(_CS_(OIX_STR_MORE)));
// 	G_SAFE_FREE(pConvert);
// 	gtk_widget_show (checkbutton3);
// 	gtk_box_pack_end (GTK_BOX (hbox1), checkbutton3, FALSE, FALSE, 0);
	
//	gtk_signal_connect (GTK_OBJECT (checkbutton3), "toggled",GTK_SIGNAL_FUNC (isDisplayMore), windows1);    
	gtk_signal_connect (GTK_OBJECT (button1), "clicked",GTK_SIGNAL_FUNC (DevFilterDlgDelete), windows1);  
	gtk_signal_connect (GTK_OBJECT (button2), "clicked",GTK_SIGNAL_FUNC (DevFilterDlgOK), GINT_TO_POINTER(crt_n));
	
	gtk_window_move(GTK_WINDOW(windows1) , 100, 100);       
	
	return  windows1 ;
	
	
}

int AttribCheckToggled(GtkWidget *widget,gpointer data)
{
	int i  ;
	int crt_n ;
	crt_n=getCrtNumFromPointer((int)data);
	i=getDataFromPointer((int)data);
	
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE;
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)))
	{
		gtk_widget_set_sensitive(GTK_WIDGET(attribToggle[crt_n][i]),TRUE);
		attribMask[crt_n][0] = attribMask[crt_n][0]  | (1<<i) ;	/*  Set bit N */
	} 
	else
	{
		gtk_widget_set_sensitive(GTK_WIDGET(attribToggle[crt_n][i]),FALSE);
		attribMask[crt_n][0] = attribMask[crt_n][0]  & (~(1<<i)) ;	/*  Clear bit N */
	}	
	return TRUE;
}

int AttribToggleToggled(GtkWidget *widget,gpointer data)
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
		gtk_button_set_label(GTK_BUTTON(widget),pConvert = _toUtf8("是"));
		attribMask[crt_n][1] = attribMask[crt_n][1]  | (1<<i) ;	/*  Set bit N */
		G_SAFE_FREE(pConvert);
	} 
	else
	{
		gtk_button_set_label(GTK_BUTTON(widget),pConvert = _toUtf8("否"));
		attribMask[crt_n][1] = attribMask[crt_n][1]  & (~(1<<i)) ;	/*  Clear bit N */
		G_SAFE_FREE(pConvert);
	}
	return TRUE;
}

int AttribButtonApply(GtkWidget *widget,gpointer data)
{
	int crt_n ;
	gchar *pConvert=0;

	crt_n=(int)data;
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE ;

	FilterAlarm[crt_n].attrib_mask[0] = attribMask[crt_n][0];
	FilterAlarm[crt_n].attrib_mask[1] = attribMask[crt_n][1];
	FilterAlarm[crt_n].is_change = TRUE;
	FilterAlarm[crt_n].change_flag = TRUE;
	AttribFilterDlgDelete( widget ,   data);  
	AlmDspProc( crt_n);
	return TRUE ;
}

char *AttribName[]={OIX_STR_FAULT ,OIX_STR_FAIL,OIX_CSTR_ScanEnable,OIX_CSTR_AlarmEnable,OIX_CSTR_AlarmNoAck,
OIX_STR_CTRL_LOCK,OIX_CSTR_NO_UPDATE,OIX_CSTR_ALM_REPEAT,OIX_STR_TAG_STATE};
static GtkWidget*
CreateAttribFilterDlg(GtkWidget *parent ,int crt_n)
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
		GTK_SIGNAL_FUNC (AttribFilterDlgDelete),  GINT_TO_POINTER(crt_n)); 
	
	gtk_window_set_destroy_with_parent(GTK_WINDOW(windows),TRUE);
	gtk_window_set_position(GTK_WINDOW(windows),GTK_WIN_POS_CENTER);  
	vbox = gtk_vbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (windows), vbox);
	attribMask[crt_n][0] = FilterAlarm[crt_n].attrib_mask[0];
	attribMask[crt_n][1] = FilterAlarm[crt_n].attrib_mask[1];
	for (i=0;i<attrib_num;i++)
	{
		if (FilterAlarm[crt_n].attrib_mask[0] & (0X1<<i) ) 
			isChecked = TRUE;  
		else
			isChecked = FALSE;
		if (FilterAlarm[crt_n].attrib_mask[1] & (0X1<<i) ) 
			isToggled = TRUE;  
		else
			isToggled = FALSE;
		hbox[i] = gtk_hbox_new(FALSE,0);
		attribCheck[crt_n][i] = gtk_check_button_new_with_label(pConvert = _toUtf8EX(AttribName[i]));
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(attribCheck[crt_n][i]),isChecked);
		MenuPointer=getMenuDataPointer(crt_n,i);
		gtk_signal_connect (GTK_OBJECT (attribCheck[crt_n][i]), "toggled",GTK_SIGNAL_FUNC (AttribCheckToggled), GINT_TO_POINTER(MenuPointer));
		G_SAFE_FREE(pConvert);
		attribToggle[crt_n][i] = gtk_toggle_button_new_with_label(pConvert = _toUtf8EX(strToggle[isToggled]));
		gtk_widget_set_sensitive(GTK_WIDGET(attribToggle[crt_n][i]),isChecked);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(attribToggle[crt_n][i]),isToggled);
		gtk_signal_connect (GTK_OBJECT (attribToggle[crt_n][i]), "toggled",GTK_SIGNAL_FUNC (AttribToggleToggled), GINT_TO_POINTER(MenuPointer));
		gtk_widget_set_usize (attribToggle[crt_n][i], 50, -1);
		G_SAFE_FREE(pConvert);
		gtk_box_pack_start (GTK_BOX (hbox[i]), attribCheck[crt_n][i], FALSE, FALSE, 0);  
		gtk_box_pack_end (GTK_BOX (hbox[i]), attribToggle[crt_n][i], FALSE, FALSE, 0); 
		gtk_box_pack_start (GTK_BOX (vbox), hbox[i], FALSE, FALSE, 0);  
	}
	
	hbox1 = gtk_hbox_new (FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox), hbox1, FALSE, FALSE, 0);
	
	button1 = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_APPLY));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start (GTK_BOX (hbox1), button1, TRUE, TRUE, 0);
	gtk_signal_connect (GTK_OBJECT (button1), "clicked",GTK_SIGNAL_FUNC (AttribButtonApply), GINT_TO_POINTER(crt_n));
	
	button2 = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_CANCEL));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start (GTK_BOX (hbox1), button2, TRUE, TRUE, 0);
	gtk_signal_connect (GTK_OBJECT (button2), "clicked",GTK_SIGNAL_FUNC (AttribFilterDlgDelete), GINT_TO_POINTER(crt_n));

	gtk_widget_show_all(windows);
	return windows;
}


void
on_last_button_press  (GtkToggleButton *w,   gpointer         user_data)
{
	GtkWidget *prev , *next ,  *scrollbar  ;
	GtkWidget *toolbar =  ( GtkWidget* )user_data ;
	int crt_n =   0;
	GetCrtNumberOfWgt (GTK_WIDGET(w), &crt_n );
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return  ;
	
	prev = g_object_get_data(G_OBJECT(toolbar),"prevWgt");
	next = g_object_get_data(G_OBJECT(toolbar),"nextWgt");
	scrollbar = g_object_get_data(G_OBJECT(alm_pop_shell[crt_n]),"scrollbar");
	isLastPage[crt_n]=	gtk_toggle_button_get_active(w);
	if (isLastPage[crt_n])
	{
		gtk_widget_set_sensitive(prev, FALSE);
		gtk_widget_set_sensitive(next, FALSE);
		gtk_widget_set_sensitive(scrollbar, FALSE);
		
		if (!alm_timeout_id[crt_n])
			alm_timeout_id[crt_n]=  gtk_timeout_add ( INTERVAL,(GtkFunction)TimeCall, GINT_TO_POINTER(crt_n) ); 
		
	}
	else
	{
		gtk_widget_set_sensitive(prev, TRUE);
		gtk_widget_set_sensitive(next, TRUE);
		gtk_widget_set_sensitive(scrollbar, TRUE);
	}
	
	
	AlmDspProc( crt_n );         
	
}
gint AutoFilterToggled(GtkWidget *widget,gpointer data)
{
	int i,j;
	gint crt_n = (gint) data ;
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE;
	auto_filter_flag[crt_n] = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
	if (FilterAlarm[crt_n].stn_id != 0)
	{
		if (auto_filter_flag[crt_n])
		{
			if (!autofilter_timeout_id[crt_n])
				autofilter_timeout_id[crt_n] = gtk_timeout_add ( 1000,(GtkFunction)AutoFilterProc, GINT_TO_POINTER (crt_n) );
		}
		else
		{
			if (autofilter_timeout_id[crt_n])
			{
				gtk_timeout_remove (autofilter_timeout_id[crt_n]);
				autofilter_timeout_id[crt_n] = 0;
			}
			for(i=1;i<=alarmStnDef[FilterAlarm[crt_n].stn_id].dev_num;i++)
				for (j=1;j<=(int)alarmStnDef[FilterAlarm[crt_n].stn_id].dev[i].group_num;j++)	
				{
					alarmStnDef[FilterAlarm[crt_n].stn_id].dev[i].group[j].isFilter[crt_n] = FALSE;
					FilterAlarm[crt_n].group_mask[i-1] = FilterAlarm[crt_n].group_mask[i-1] | (1<<(j-1));
				}
			FilterAlarm[crt_n].is_change = TRUE;
			AlmDspProc(crt_n);		
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
*/ /*gtk低版本无效*/

gint PopupCustomFilter(GtkComboBox *widget,gpointer data) 
{
	gint crt_n = (gint) data ;
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE;
	gtk_combo_box_set_active(widget,-1);
	return TRUE;
}

char *Buff[]  = {OIX_CSTR_REALTIME_REC ,OIX_CSTR_HIST_REC ,OIX_CSTR_SYSMSG,OIX_CSTR_HISTDB_REC,  "历史报警搜索"};
char *BuffCmd[]  = {OIX_CSTR_ALM_ALL_TYPEID, OIX_CSTR_ALM_CMD_TYPEID,OIX_CSTR_ALM_INFO_TYPEID};

static GtkWidget *
CreateAlmToolbar (GtkWidget * parent,	int	crt_n)
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
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER ) return ( GtkWidget *)NULL;
	
	memcpy(alarmStnDef,StnDef,sizeof(StnDef));
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
	for (i = 0 ; i < (crt_n==0?5:3)  ; i++)
	{
		items = gtk_menu_item_new_with_label(pConvert=_toUtf8(_CS_(Buff[i])));
		G_SAFE_FREE(pConvert);
		gtk_widget_show( items);
		gtk_container_add(GTK_CONTAINER(menu),items);
		MenuPointer=getMenuDataPointer(crt_n,i);		 		 	 
		gtk_signal_connect (GTK_OBJECT (items), "activate",GTK_SIGNAL_FUNC (SetHistAlmBtn), GINT_TO_POINTER(MenuPointer));
		
	}   
	gtk_menu_set_active ( GTK_MENU(menu),alm_rec_mode[crt_n]  );  
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
		gtk_signal_connect (GTK_OBJECT (items), "activate",GTK_SIGNAL_FUNC (CreateDbDevB), GINT_TO_POINTER(MenuPointer));
		FilterAlarm[crt_n].stn_id=0;
	}
	else
		FilterAlarm[crt_n].stn_id=StnDef[1].Id; /* added by zyp on 2006.11.1 *//*modify by dm*/
	for(i=1;i<=station_num;i++) 
	{
		if(strcmp(alarmStnDef[i].CName,"")!=0)
		{	
			UCHAR	stn_id;
			if(-1==GetStnIdByIndex(i, &stn_id))
			continue;

			if (isStnInMask((UCHAR)stn_id)==0) continue ; 
			items = gtk_menu_item_new_with_label(pConvert=_toUtf8(alarmStnDef[i].CName));
			G_SAFE_FREE(pConvert);
			gtk_widget_show( items);
			gtk_container_add(GTK_CONTAINER(menu),items);
			
			MenuPointer=getMenuDataPointer(crt_n,stn_id);
			
			gtk_signal_connect (GTK_OBJECT (items), "activate",GTK_SIGNAL_FUNC (CreateDbDevB), GINT_TO_POINTER(MenuPointer));
			
			
		}
	}
	g_object_set_data   ( G_OBJECT(parent),"StationOpMenu",optionmenu);
	gtk_option_menu_set_menu(GTK_OPTION_MENU(optionmenu),menu);
	
	/*  *************  dev   combobox  ****************** */ 
	
	dev_button1  = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_ALM_DEV_FILTER));
	G_SAFE_FREE(pConvert);
	g_signal_connect (G_OBJECT (dev_button1), "clicked",G_CALLBACK (dev_button_press),  GINT_TO_POINTER(crt_n));
	gtk_widget_show(dev_button1);
	gtk_box_pack_start (GTK_BOX (box), dev_button1, FALSE, FALSE, 0);
	filterDlg[crt_n]=NULL;

	
	/*********command type ***********/       
	
	optionmenu = gtk_option_menu_new();
	gtk_widget_show(optionmenu);
	gtk_box_pack_start (GTK_BOX (box), optionmenu, FALSE, FALSE, 0);
	menu =gtk_menu_new();
	gtk_widget_set_usize (menu, 130, -1);
	/*  OIX_CSTR_all ,OIX_CSTR_CMD  ,OIX_CSTR_INFO */
	for (i = 0 ; i < 3 ; i++)
	{
		items = gtk_menu_item_new_with_label(pConvert=_toUtf8(_CS_(BuffCmd[i])));
		G_SAFE_FREE(pConvert);
		gtk_widget_show( items);
		gtk_container_add(GTK_CONTAINER(menu),items);
		
		MenuPointer=getMenuDataPointer(crt_n,i);
		
		gtk_signal_connect (GTK_OBJECT(items), "activate",GTK_SIGNAL_FUNC (SetTypeIdMask), GINT_TO_POINTER (MenuPointer));
		
	}
	
	gtk_menu_set_active ( GTK_MENU(menu),FilterAlarm[crt_n].cmd_type  ); 
	g_object_set_data   ( G_OBJECT(parent),"TypeIdOpMenu",optionmenu);
	gtk_box_pack_start (GTK_BOX (box), menu, FALSE, FALSE, 0);
	gtk_option_menu_set_menu(GTK_OPTION_MENU(optionmenu),menu);
	
	
	/*  *************  datatype   combobox  *******************/ 
	optionmenu = gtk_option_menu_new();
	gtk_widget_show(optionmenu);
	gtk_box_pack_start (GTK_BOX (box), optionmenu, FALSE, FALSE, 0);
	menu =gtk_menu_new();
	gtk_widget_set_usize (menu, 130, -1);
	
	
	DataTypeItem[crt_n][ MAX_DATATYPE_NUM] = gtk_menu_item_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_ALM_REC_ALL));
	G_SAFE_FREE(pConvert);
	gtk_widget_show(DataTypeItem[crt_n][MAX_DATATYPE_NUM]);
	gtk_container_add(GTK_CONTAINER(menu),DataTypeItem[crt_n][MAX_DATATYPE_NUM]);
	MenuPointer=getMenuDataPointer(crt_n,MAX_DATATYPE_NUM);
	gtk_signal_connect (GTK_OBJECT (DataTypeItem[crt_n][MAX_DATATYPE_NUM]), "activate",GTK_SIGNAL_FUNC (SetDataTypeMask),GINT_TO_POINTER (MenuPointer) );
	
	
	DataTypeItem[crt_n][ MAX_DATATYPE_NUM+1] = gtk_menu_item_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_ALM_REC_NON));
	G_SAFE_FREE(pConvert);
	gtk_widget_show(DataTypeItem[crt_n][ MAX_DATATYPE_NUM+1]);
	gtk_container_add(GTK_CONTAINER(menu),DataTypeItem[crt_n][ MAX_DATATYPE_NUM+1]);
	MenuPointer=getMenuDataPointer(crt_n,MAX_DATATYPE_NUM+1);
	gtk_signal_connect (GTK_OBJECT (DataTypeItem[crt_n][ MAX_DATATYPE_NUM+1]), "activate",GTK_SIGNAL_FUNC (SetDataTypeMask),GINT_TO_POINTER(MenuPointer) );
	
	
	for(i=0,j=1;i<MAX_DATATYPE_NUM;i++)
	{
		if(GetRecLongnameByType((UCHAR)i, recordTypeName )!=-1)
		{					
			DataTypeItem[crt_n][j] = gtk_check_menu_item_new_with_label(pConvert=_toUtf8(recordTypeName));
			G_SAFE_FREE(pConvert);
			if (FilterAlarm[crt_n].datatype_mask & 1<<(i-1) )    //再次打开事件一览表显示错误，修改j为i		
				gtk_check_menu_item_set_active  ( GTK_CHECK_MENU_ITEM( DataTypeItem[crt_n][j]),TRUE);
			else
				gtk_check_menu_item_set_active  ( GTK_CHECK_MENU_ITEM(DataTypeItem[crt_n][j]),FALSE);
			
			gtk_widget_show(DataTypeItem[crt_n][j]);
			gtk_container_add(GTK_CONTAINER(menu),DataTypeItem[crt_n][j]);
			//MenuPointer=getMenuDataPointer(crt_n,j); //传入j后，筛选错位
			MenuPointer=getMenuDataPointer(crt_n,i);
			gtk_signal_connect (GTK_OBJECT (DataTypeItem[crt_n][j]), "toggled",GTK_SIGNAL_FUNC (SetDataTypeMask), GINT_TO_POINTER(MenuPointer));	
			j++;
		}
	}
	
	g_object_set_data   ( G_OBJECT(parent),"DataTypeOpMenu",optionmenu);
	
	gtk_option_menu_set_menu(GTK_OPTION_MENU(optionmenu),menu);
	/***********************************attrib combobox****************************************/

	attrib_button  = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_ALM_ATTRIB_FILTER));
	G_SAFE_FREE(pConvert);
	g_signal_connect (G_OBJECT (attrib_button), "clicked",G_CALLBACK (attrib_button_press),  GINT_TO_POINTER(crt_n));
	gtk_widget_show(attrib_button);
	gtk_box_pack_start (GTK_BOX (box), attrib_button, FALSE, FALSE, 0);
	AttribFilterDlg[crt_n]=NULL;


	///////////////////////////报警确认筛选/////////////////////////////////////onlyDisplayNoAck
	
	ckbtDisNoAck = gtk_check_button_new_with_label (pConvert=_toUtf8EX(OIX_CSTR_SHOW_ALM_NOLY_AFFIRM));   //OIX_CSTR_SHOW_ALM_NOLY_AFFIRM  "只显示未确认报警"  
	G_SAFE_FREE(pConvert);
	gtk_widget_show (ckbtDisNoAck);
	gtk_box_pack_start (GTK_BOX (box1), ckbtDisNoAck, FALSE, FALSE, 0);
	gtk_signal_connect (GTK_OBJECT (ckbtDisNoAck), "toggled",GTK_SIGNAL_FUNC (onlyDisplayNoAck), GINT_TO_POINTER(crt_n));    
	
	
	Prev_button = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_PAGE_UP));
	G_SAFE_FREE(pConvert);
	
	gtk_box_pack_start (GTK_BOX (box1), Prev_button, FALSE, FALSE, 0);     	     
	gtk_signal_connect(GTK_OBJECT(Prev_button), "clicked", GTK_SIGNAL_FUNC(PrevPage), GINT_TO_POINTER(crt_n));
	gtk_widget_show (Prev_button);
	
	g_object_set_data(G_OBJECT(toolbar),"prevWgt",Prev_button );
	
	next_button = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_PAGE_DOWN));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start (GTK_BOX (box1), next_button, FALSE, FALSE, 0);     	     
	gtk_signal_connect(GTK_OBJECT(next_button), "clicked", GTK_SIGNAL_FUNC(NextPage),   GINT_TO_POINTER(crt_n));
	gtk_widget_show (next_button);
	
	g_object_set_data(G_OBJECT(toolbar),"nextWgt",next_button );
	
	// 	init_button = gtk_button_new_with_label(_toUtf8EX(OIX_STR_FILTER_INIT));
	// 	gtk_box_pack_start (GTK_BOX (box1), init_button, FALSE, FALSE, 0);     	     
	// 	gtk_signal_connect(GTK_OBJECT(init_button), "clicked", GTK_SIGNAL_FUNC(FiterInit),   GINT_TO_POINTER(crt_n));
	// 	gtk_widget_show (init_button); 
	
	
	/**************************************************Filter combobox*****************************************************/
	curFilter = 0;
	storeFilter_a[crt_n]=gtk_list_store_new(num_columns_Filter,G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, 
											G_TYPE_STRING, G_TYPE_STRING,G_TYPE_STRING, G_TYPE_STRING,G_TYPE_STRING,
											G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING);
	filterCombo = gtk_combo_box_new_with_model(GTK_TREE_MODEL(storeFilter_a[crt_n]));
	
	pRenderFilter = gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(filterCombo), pRenderFilter, FALSE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(filterCombo), pRenderFilter, "text", eName_Filter, NULL);
	g_object_unref(storeFilter_a[crt_n]);
	
	gtk_box_pack_start (GTK_BOX (box), filterCombo, FALSE, FALSE, 0);
	gtk_widget_set_usize (filterCombo, 130, -1);
	gtk_widget_show(filterCombo);
	gtk_list_store_insert(storeFilter_a[crt_n], &iter,0);
	gtk_list_store_set (storeFilter_a[crt_n], &iter, eName_Filter, pConvert=_toUtf8EX("OIX_STR_FILTER_INIT"), -1);
	G_SAFE_FREE(pConvert);
//	gtk_combo_box_set_row_separator_func (GTK_COMBO_BOX (filterCombo), is_separator, GINT_TO_POINTER(crt_n), NULL);
//	gtk_list_store_insert(storeFilter_a[crt_n], &iter,1);
//	gtk_list_store_set (storeFilter_a[crt_n], &iter, eFilterName,NULL, -1);
	gtk_list_store_insert(storeFilter_a[crt_n], &iter,1);
	gtk_list_store_set (storeFilter_a[crt_n], &iter, eName_Filter, pConvert=_toUtf8EX("OIX_STR_FILTER_SET"), -1);
	G_SAFE_FREE(pConvert);
	filterNum = ReadFilterXml(storeFilter_a[crt_n],"Predefine");
	gtk_combo_box_set_active(GTK_COMBO_BOX(filterCombo),curFilter);
	g_signal_connect (G_OBJECT(filterCombo), "changed", G_CALLBACK(ChgCustomFilter), GINT_TO_POINTER(crt_n));
	g_signal_connect(G_OBJECT(filterCombo), "popup", G_CALLBACK(PopupCustomFilter), GINT_TO_POINTER(crt_n));
	/********************************************************************************************************************/
	
	ack_button = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_ALM_TAB_ACKN));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start (GTK_BOX (box1), ack_button, FALSE, FALSE, 0);     	     
	gtk_signal_connect(GTK_OBJECT(ack_button), "clicked", GTK_SIGNAL_FUNC(doAlarmAck),   GINT_TO_POINTER(crt_n));
	gtk_widget_show (ack_button); 
	
	range_ack_btn = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_ALM_RANGE_ACKN));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start (GTK_BOX (box1),  range_ack_btn, FALSE, FALSE, 0);     	     
	gtk_signal_connect(GTK_OBJECT( range_ack_btn), "clicked", GTK_SIGNAL_FUNC(doAlarmAckRange),   GINT_TO_POINTER(crt_n));
	gtk_widget_show ( range_ack_btn); 
	
	
	print_button = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_PRINT));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start (GTK_BOX (box1), print_button, FALSE, FALSE, 0);     	     
	gtk_signal_connect(GTK_OBJECT(print_button), "clicked", GTK_SIGNAL_FUNC(doPrint),   GINT_TO_POINTER(crt_n));
	gtk_widget_show (print_button); 
	
	
	
	last_button  = gtk_toggle_button_new_with_label (pConvert=_toUtf8EX(OIX_CSTR_NEW_PAGE));
	G_SAFE_FREE(pConvert);
	gtk_widget_show (last_button);
	gtk_box_pack_start (GTK_BOX (box1), last_button, FALSE, FALSE, 0);     	     
	gtk_signal_connect(GTK_OBJECT(last_button), "toggled", GTK_SIGNAL_FUNC(on_last_button_press),  toolbar);
	/*  gtk_toggle_button_toggled(last_button) ;  */
	lbPage = gtk_label_new ("Page:1/3");
	gtk_box_pack_end (GTK_BOX (box1), lbPage, FALSE, FALSE, 0);
	gtk_widget_show (lbPage);
	g_object_set_data(G_OBJECT(parent),"labelPage",lbPage );
	
	if (xml_auto_filter)
	{
	auto_filter = gtk_check_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_AUTO_FILTER));
	G_SAFE_FREE(pConvert);
	gtk_widget_show(auto_filter);
	gtk_box_pack_start(GTK_BOX(box1),auto_filter,FALSE,FALSE,0);
	gtk_signal_connect(GTK_OBJECT(auto_filter), "toggled",GTK_SIGNAL_FUNC(AutoFilterToggled), GINT_TO_POINTER(crt_n));   
	auto_filter_flag[crt_n] = FALSE;
	}

	button_exit = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_EXIT)); 
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start(GTK_BOX(box1), button_exit, FALSE, FALSE, 0);   
	gtk_signal_connect(GTK_OBJECT(button_exit), "clicked",GTK_SIGNAL_FUNC(AlarmFormQuit), GINT_TO_POINTER(crt_n));   
	gtk_widget_show(button_exit);
	
	return (toolbar);
}

void	ProgAlarmAckBrdcst ( COMMAND_MSG *alm_msg,  int send_num)
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

int ReadHistRecFile (int crt_n, char *filename)
{
	FILE	*fp;
	int	count1, len ,i, numread ;
	struct stat buff;
	/*if (alm_rec_mode[crt_n] == ALM_REC_HISTRY1) */
	{
		fp	 = OpenFileX ( filename );	
		if ( fp == NULL)
		{
			PopupMessageBox(alm_pop_shell[crt_n],MSG_OPER_HISTORYFILE_ERROR);
			return  FALSE;
		}
		
		stat(filename,&buff);
		len	= buff.st_size ;
		if (len > REC_ENTRY_SIZE * MAX_FILE_RECORD_NUM )
			len  =  REC_ENTRY_SIZE * MAX_FILE_RECORD_NUM  ;
		if ((len% REC_ENTRY_SIZE) == 0)
			count1 = (len )/ REC_ENTRY_SIZE;
		else
			count1 = 0;
		
		fseek ( fp, 0L, SEEK_SET );
		
		numread =fread ( &hist_rec_addr[crt_n].rec, len , 1, fp );
		
		fclose ( fp );
		for (i = 0 ; i <count1 ; i++ )
			SwapRecEntryByte(&hist_rec_addr[crt_n].rec[i]) ; 
		
		
		hist_rec_addr[crt_n].head.count	= count1;
		
		hist_rec_addr[crt_n].head.buf_size = MAX_FILE_RECORD_NUM;
		if (count1 ==  MAX_FILE_RECORD_NUM )
			hist_rec_addr[crt_n].head.header_ptr = 0 ; 
		else if (count1<  MAX_FILE_RECORD_NUM )
			hist_rec_addr[crt_n].head.header_ptr = count1; 
		else    /*  count1 > MAX_FILE_RECORD_NUM  只取锟斤拷前锟斤拷锟?MAX_FILE_RECORD_NUM -6000锟斤拷锟斤拷录 */
			hist_rec_addr[crt_n].head.header_ptr = -1 ; ;
		
		/** InitAlarmFilter(&FilterAlarm[crt_n],crt_n);*/
		alm_rec_mode[crt_n]	       = ALM_REC_HISTRY1;
		FilterAlarm[crt_n].phead   =  &hist_rec_addr[crt_n].head ;
		FilterAlarm[crt_n].pRec    =  (REC_ENTRY_UNION* )(&hist_rec_addr[crt_n].rec);
		FilterAlarm[crt_n].is_change = TRUE;
		
	}
	
	AlmDspProc( crt_n );
	return TRUE; 
	
}


/*以下三个函数 用于处理menu crt_n的获取*/
int getMenuDataPointer(int crtn,int data)
{
	data=crtn*LENFORPOINTER+data;
	return(data);
}

int getCrtNumFromPointer(int data)
{
	return data/LENFORPOINTER;
}

int getDataFromPointer(int data)
{
	return data%LENFORPOINTER;
}


int ReadSearchHitsRecFile (int crt_n,int recCount )
{
	
	FILE	*fp;
	int	count1, len ,i;
	struct stat buff;
	char  infoMessage[256] ; 
	
	sprintf(dest_filename, "%s%s", SYS_TMP_PATH, EVENT_SEARCH_TMP_FILE); 
	
	
	fp	 = OpenFileX ( dest_filename );	
	if ( fp == NULL)
	{
		PopupMessageBox(alm_pop_shell[crt_n],"MSG_OPER_FILE_ERROR!");   //"MSG_OPER_FILE_ERROR"  操作文件错误
		return  FALSE;
	}
	
	stat(dest_filename,&buff);
	
	len	= buff.st_size ;
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
		PopupMessageBox(alm_pop_shell[crt_n],infoMessage);
		fseek(fp, MAX_FILE_RECORD_NUM*REC_ENTRY_SIZE, SEEK_END);
		fread ( &hist_rec_addr[crt_n].rec, MAX_FILE_RECORD_NUM*REC_ENTRY_SIZE , 1, fp );
		count1 =  MAX_FILE_RECORD_NUM  ; 						
	}
	else
		fread ( &hist_rec_addr[crt_n].rec, len , 1, fp );
	
	fclose ( fp );
	for (i = 0 ; i <count1 ; i++ )
		SwapRecEntryByte(&hist_rec_addr[crt_n].rec[i]) ; 
	
	hist_rec_addr[crt_n].head.count	= count1;						
	hist_rec_addr[crt_n].head.buf_size = MAX_FILE_RECORD_NUM;
	if (count1 ==  MAX_FILE_RECORD_NUM )
		hist_rec_addr[crt_n].head.header_ptr = 0 ; 
	else if (count1<  MAX_FILE_RECORD_NUM )
		hist_rec_addr[crt_n].head.header_ptr = count1; 
	else
		;   //error ;
	
	/** InitAlarmFilter(&FilterAlarm[crt_n],crt_n);*/
	alm_rec_mode[crt_n]	= ALM_REC_HISTRY1;
	FilterAlarm[crt_n].phead   =  &hist_rec_addr[crt_n].head ;
	FilterAlarm[crt_n].pRec    =  (REC_ENTRY_UNION* )(&hist_rec_addr[crt_n].rec);
	FilterAlarm[crt_n].is_change = TRUE;					
	return TRUE; 
	
}

void  SetHistFileAlmCName(GtkWidget * widget, gpointer data)



{
	int crt_n ;
	int i = (gint) data ;
	//	GetCrtNumberOfWgt (widget, &crt_n );
	crt_n  = 0 ; 
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return ;
	g_object_set_data(G_OBJECT(widget),"iNameMode", GINT_TO_POINTER (i));  
	eventFilter.search_type =  i  ; 
	g_message ("iNameMode = %d \n " , i )  ;
	
}

gboolean updateHisEventSearch(gpointer data)
{
	gchar *pConvert=0;

	int crt_n=GPOINTER_TO_INT(data);
	if (crt_n < 0 || crt_n > MAX_CRT_NUMBER)
	{
		return FALSE;
	}

	if (shm_syscomm_addr->event_search.search_flag) 
	{
		return FALSE;
	}

	if (event_search_timer_id[crt_n] != 0)
	{
		g_source_remove(event_search_timer_id[crt_n]);
	}
	
	alm_rec_mode[crt_n]  =  ALM_REC_FILE_SEARCH;
	ReadSearchHitsRecFile (  crt_n,shm_syscomm_addr->event_search.event_num) ;
	
	if (alm_pop_shell[crt_n])
	{
		gtk_window_set_title(GTK_WINDOW(alm_pop_shell[crt_n]), pConvert=_toUtf8("MSG_HIS_FILE_SERP...")); //"MSG_HIS_FILE_SERP" 历史文件搜索结果...
		G_SAFE_FREE(pConvert);
	}
	AlmDspProc( crt_n ); 

	return TRUE ;
}

int HistFileDBAlarmOK(GtkWidget *w , gpointer data)
{ 
	int crt_n;
	char *ss1 ;
	//	int iMode ; 
	int ret ; 
	int year,month,day ,minute,second,hour; 
	GtkWidget *startEntry , *endEntry,*cNameEntry ;
    char runAPPcmd[128] ; 
	gchar *pConvert=0;

	//	time_t startTm  ,endTm ;
	GtkWidget *window =data ;	
	GetCrtNumberOfWgt (w, &crt_n );
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return TRUE ;
	
	startEntry  = g_object_get_data(G_OBJECT(window),"startWgt");
	endEntry   = g_object_get_data(G_OBJECT(window), "endWgt");
	cNameEntry  = g_object_get_data(G_OBJECT(window), "cNameWgt");
	//	iMode  =  iHisNameMode[crt_n] ;
	ss1  = (gchar*)gtk_entry_get_text(GTK_ENTRY(startEntry));
	if (strcmp(ss1,"")!=0)
	{   
		sscanf(ss1,"%4d-%02d-%02d %02d:%02d",&year,&month,&day,&hour,&minute,&second) ;
		eventFilter.start_t.year	= 	 year ;
		eventFilter.start_t.month	=	 month;
		eventFilter.start_t.day		= 	 day;
		eventFilter.start_t.hour	= 	 hour ;
		eventFilter.start_t.minute	= 	 minute;
		eventFilter.start_t.second	= 	 second ;
	}
	ss1  = (gchar*)gtk_entry_get_text(GTK_ENTRY(endEntry));
	if (strcmp(ss1,"")!=0)
	{   
		sscanf(ss1,"%4d-%02d-%02d %02d:%02d",&year,&month,&day,&hour,&minute,&second) ;
		eventFilter.end_t.year		= 	 year ;
		eventFilter.end_t.month		=	 month;
		eventFilter.end_t.day		= 	 day;
		eventFilter.end_t.hour		= 	 hour ;
		eventFilter.end_t.minute	= 	 minute;
		eventFilter.end_t.second	= 	 second ;
	}
	
	ss1  = (gchar*)gtk_entry_get_text(GTK_ENTRY(cNameEntry));
	if (strcmp(ss1,"")!=0)
	{
		strcpy(eventFilter.search_str, pConvert=_fromUtf8(ss1));
		G_SAFE_FREE(pConvert);
	}
	eventFilter.search_flag =1;   //start search ......
	memcpy(&shm_syscomm_addr->event_search,&eventFilter,sizeof(EVENT_SEARCH)) ;

#ifdef SUN_OS 
	strcpy (runAPPcmd,"//home//ems//h9000//bin//event_search");
	strcat(runAPPcmd,TASK_SUFFIX_NAME) ; 
	
#endif
#ifdef WINDOWS_OS 
	strcpy (runAPPcmd,"//home//ems//h9000//bin//event_search");
	strcat(runAPPcmd,TASK_SUFFIX_NAME) ; 
#endif
	ret = system("//home//ems//h9000//bin//event_search.exe"   /*runAPPcmd*/);
// 	if (ret) 
// 	{
// 		PopupMessageBox(alm_pop_shell[crt_n] , "ERROR occur while run event_search!"); 
// 		return TRUE ;
// 	}
	if (event_search_timer_id[crt_n] != 0)
	{
		g_source_remove(event_search_timer_id[crt_n]);
	}
	event_search_timer_id[crt_n] = g_timeout_add (100, (GSourceFunc)updateHisEventSearch, GINT_TO_POINTER(crt_n));
	updateHisEventSearch(GINT_TO_POINTER(crt_n));
	
	HistDBFileAlmDlgDelete( w ,   data);  
	
	
	//Fail:
// 	PopupMessageBox(alm_pop_shell[crt_n] , "MSG_DATA_FORMATE_ERROR!");  //"MSG_DATA_FORMATE_ERROR" 数据格式出错 
// 	return TRUE ;
// 	
// 	
// 	alm_rec_mode[0]   =  ALM_REC_FILE_SEARCH;
// 	return TRUE ;
}


const char *sCName[] = {"通过点号搜索","通过逻辑名搜索","通过长名搜索"};
GtkWidget *CreateHistDBFileAlmDlg(GtkWidget *parent ,int crt_n)
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
	if (eventFilter.start_t.year  ==0 && eventFilter.start_t.month ==0  && eventFilter.start_t.day ==0 
		&&  eventFilter.start_t.hour ==0 &&  eventFilter.start_t.minute ==0 && eventFilter.start_t.second ==0 )
	{
		eventFilter.start_t.year		= 	 devtime.tm_year+1900 ;
		eventFilter.start_t.month		=	 devtime.tm_mon+1;
		eventFilter.start_t.day			= 	 devtime.tm_mday ;
		eventFilter.start_t.hour		= 	 devtime.tm_hour ;
		eventFilter.start_t.minute		= 	 devtime.tm_min ;
		eventFilter.start_t.second		= 	 0 ;
		
	}
	sprintf (Stimer , "%4d-%02d-%02d %02d:%02d" ,eventFilter.start_t.year,
		eventFilter.start_t.month ,eventFilter.start_t.day, eventFilter.start_t.hour, eventFilter.start_t.minute  ) ;
	
	if (eventFilter.end_t.year  ==0 && eventFilter.end_t.month ==0  && eventFilter.end_t.day ==0 
		&&  eventFilter.end_t.hour ==0 &&  eventFilter.end_t.minute ==0 && eventFilter.end_t.second ==0 )
	{
		eventFilter.end_t.year		= 	 devtime.tm_year+1900 ;
		eventFilter.end_t.month		=    devtime.tm_mon+1;
		eventFilter.end_t.day		= 	 devtime.tm_mday ;
		eventFilter.end_t.hour		= 	 devtime.tm_hour ;
		eventFilter.end_t.minute	=    devtime.tm_min ;
		eventFilter.end_t.second	= 	 0 ;
		
	}
	sprintf (Etimer , "%4d-%02d-%02d %02d:%02d" ,eventFilter.end_t.year,
		eventFilter.end_t.month ,eventFilter.end_t.day, eventFilter.end_t.hour, eventFilter.end_t.minute  ) ;
	
	
	windows1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(windows1), pConvert=_toUtf8EX(OIX_STR_HIS_FILE_SEARCH_WIN));  //"MSG_HIS_FILE_SEARCH_WIN" 历史文件搜索窗口
	G_SAFE_FREE(pConvert);
	gtk_window_set_transient_for(GTK_WINDOW(windows1), GTK_WINDOW(parent)); 
	gtk_window_set_destroy_with_parent(GTK_WINDOW(windows1),TRUE);
	
	gtk_signal_connect (GTK_OBJECT (windows1), "delete_event", 
		GTK_SIGNAL_FUNC (HistDBFileAlmDlgDelete), GINT_TO_POINTER (crt_n)); 
	
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
	if (eventFilter.search_type==0)
		eventFilter.search_type =1  ; 
	for (i = 1 ; i < 3  ; i++)
	{
		items = gtk_menu_item_new_with_label(pConvert=_toUtf8((char*)sCName[i]));	
		G_SAFE_FREE(pConvert);
		gtk_widget_show( items);
		gtk_container_add(GTK_CONTAINER(menu),items);
		gtk_signal_connect (GTK_OBJECT (items), "activate",GTK_SIGNAL_FUNC (SetHistFileAlmCName), GINT_TO_POINTER(i));
	}   
	
	gtk_menu_set_active ( GTK_MENU(menu), eventFilter.search_type-1  ); 
	gtk_option_menu_set_menu(GTK_OPTION_MENU(optionmenu),menu);
	
	
	/***********************************************************************************/
	entry3 = gtk_entry_new ();
	gtk_widget_show (entry3);
	gtk_table_attach (GTK_TABLE (table1), entry3, 1, 2, 2, 3,
		(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
		(GtkAttachOptions) (0), 0, 0);
	
	
	if(strlen(eventFilter.search_str )>0)
	{
		gtk_entry_set_text(GTK_ENTRY(entry3), pConvert=_toUtf8(eventFilter.search_str));
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
	
	gtk_signal_connect (GTK_OBJECT (button1), "clicked",GTK_SIGNAL_FUNC (HistDBFileAlmDlgDelete), windows1);  
	gtk_signal_connect (GTK_OBJECT (button2), "clicked",GTK_SIGNAL_FUNC (HistFileDBAlarmOK), windows1);
	
	gtk_window_set_position(GTK_WINDOW(windows1),GTK_WIN_POS_CENTER);  
	//	iHisNameMode[crt_n] =  1 ;  
	return  windows1 ;
	
}

int HistDBFileAlmDlgDelete (GtkWidget *w , gpointer data)
{
	int  crt_n = (gint)data;
	GetCrtNumberOfWgt (w, &crt_n );
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE ;
	gtk_widget_destroy(HistFileDBDlg[crt_n] );
	HistFileDBDlg[crt_n]  = (GtkWidget*)NULL ; 
	return TRUE;
}

int SaveFilterXml(gchar *filterName,FILTER_T *pFilter,const char *nodeName)
{
	xmlDocPtr   pdoc = NULL;
	xmlNodePtr  proot = NULL, curNode = NULL,pFilterNode = NULL,pEntryNode = NULL;
//	GtkTreeIter iter;
	char	err_str[256];
//	xmlChar *key; 
	gchar *pConvert=0;
	int i,offset = 0;
	char stn_id[2],stn_mask[9],cmd_type[2],fault_mask[9],type_id_mask[9],dev_mask[9],group_mask[450],datatype_mask[9],attrib_mask[18],start_time[30],end_time[30],logic_name[50],long_name[MAX_SEARCH_NAME_SIZE];
	xmlKeepBlanksDefault(0);
	pdoc = xmlReadFile (FILE_OIX_CONFIG_XML, "GB2312", XML_PARSE_NOBLANKS);
	if (pdoc == NULL)
    {
		sprintf(err_str, "Fail to open %s", FILE_OIX_CONFIG_XML);
		Err_log(err_str);
		return -1;
    }
	
	proot = xmlDocGetRootElement (pdoc);
	if (proot == NULL)
    {
		sprintf(err_str, "%s not have root", FILE_OIX_CONFIG_XML);
		Err_log(err_str);
		return -1;
    }
	curNode = proot->xmlChildrenNode;
	while (curNode != NULL)
	{
		if (!xmlStrcmp(curNode->name, BAD_CAST nodeName))
		{
			pFilterNode = xmlNewNode(NULL,BAD_CAST"FILTER");
			xmlAddChild(curNode,pFilterNode);
			xmlNewProp(pFilterNode, BAD_CAST "name", BAD_CAST filterName);
			sprintf(stn_id,"%u",pFilter->stn_id);
			xmlNewTextChild(pFilterNode, NULL, BAD_CAST "STN_ID", BAD_CAST stn_id);
			sprintf(stn_mask,"%.08X",pFilter->stn_mask);
			xmlNewTextChild(pFilterNode, NULL, BAD_CAST "STN_MASK", BAD_CAST stn_mask);
			sprintf(cmd_type,"%u",pFilter->cmd_type);
			xmlNewTextChild(pFilterNode, NULL, BAD_CAST "CMD_TYPE", BAD_CAST cmd_type);
			sprintf(fault_mask,"%u",pFilter->fault_mask);
			xmlNewTextChild(pFilterNode, NULL, BAD_CAST "FAULT_MASK", BAD_CAST fault_mask);
			sprintf(type_id_mask,"%u",pFilter->type_id_mask);
			xmlNewTextChild(pFilterNode, NULL, BAD_CAST "TYPE_ID_MASK", BAD_CAST type_id_mask);
			sprintf(dev_mask,"%.08X",pFilter->dev_mask[0]);
			xmlNewTextChild(pFilterNode, NULL, BAD_CAST "DEV_MASK", BAD_CAST dev_mask);
			for (i=0;i<MAX_GROUP_MASK_NUM;i++)
			{
				offset += sprintf(group_mask + offset, "%.08X", pFilter->group_mask[i]); 
			}
			xmlNewTextChild(pFilterNode, NULL, BAD_CAST "GROUP_MASK", BAD_CAST group_mask);
			sprintf(datatype_mask,"%.08X",pFilter->datatype_mask);
			xmlNewTextChild(pFilterNode, NULL, BAD_CAST "DATATYPE_MASK", BAD_CAST datatype_mask);
			offset = sprintf(attrib_mask,"%.08X",pFilter->attrib_mask[0]);
			sprintf(attrib_mask + offset,"%.08X",pFilter->attrib_mask[1]);
			xmlNewTextChild(pFilterNode, NULL, BAD_CAST "ATTRIB_MASK", BAD_CAST attrib_mask);
			strcpy(start_time,TimeToString(pFilter->start_time));
			xmlNewTextChild(pFilterNode, NULL, BAD_CAST "START_TIME", BAD_CAST start_time);
			strcpy(end_time,TimeToString(pFilter->end_time));
			xmlNewTextChild(pFilterNode, NULL, BAD_CAST "END_TIME", BAD_CAST end_time);
			strcpy(logic_name,pFilter->LogicName);
			xmlNewTextChild(pFilterNode, NULL, BAD_CAST "LOGIC_NAME", BAD_CAST (pConvert = _toUtf8(logic_name)));
			G_SAFE_FREE(pConvert);
			strcpy(long_name,pFilter->search_str);
			xmlNewTextChild(pFilterNode, NULL, BAD_CAST "LONG_NAME", BAD_CAST (pConvert = _toUtf8(long_name)));
			G_SAFE_FREE(pConvert);
		}
		curNode = curNode->next;
	}
	xmlSaveFormatFile(FILE_OIX_CONFIG_XML,pdoc,1);
	xmlFreeDoc(pdoc); 
	xmlCleanupParser ();
	return 0;
}
int DelFilterXml(gchar *filterName,const char *nodeName)
{
	xmlDocPtr   pdoc = NULL;
	xmlNodePtr  proot = NULL, pTypeNode = NULL,pFilterNode = NULL,pEntryNode = NULL;
//	GtkTreeIter iter;
	char	err_str[256];
//	xmlChar *key; 
	xmlKeepBlanksDefault(0);
	pdoc = xmlReadFile (FILE_OIX_CONFIG_XML, "GB2312", XML_PARSE_NOBLANKS);
	if (pdoc == NULL)
    {
		sprintf(err_str, "Fail to open %s", FILE_OIX_CONFIG_XML);
		Err_log(err_str);
		return -1;
    }
	
	proot = xmlDocGetRootElement (pdoc);
	if (proot == NULL)
    {
		sprintf(err_str, "%s not have root", FILE_OIX_CONFIG_XML);
		Err_log(err_str);
		return -1;
    }
	pTypeNode = proot->xmlChildrenNode;
	while (pTypeNode != NULL)
	{
		if (!xmlStrcmp(pTypeNode->name, BAD_CAST nodeName))
		{
			pFilterNode = pTypeNode->xmlChildrenNode;
			while(pFilterNode != NULL)
			{
				if (!xmlStrcmp(pFilterNode->name, BAD_CAST "FILTER"))
				{
					if (!xmlStrcmp(xmlGetProp(pFilterNode,BAD_CAST "name"),filterName))
					{
						xmlNodePtr tempNode;
						tempNode = pFilterNode->next;
						xmlUnlinkNode(pFilterNode);
						xmlFreeNode(pFilterNode);
						pFilterNode = tempNode;
					}
				}
				if (pFilterNode != NULL)
					pFilterNode = pFilterNode->next;
			}
		}
		pTypeNode = pTypeNode->next;
	}
	xmlSaveFormatFile(FILE_OIX_CONFIG_XML,pdoc,1);
	xmlFreeDoc(pdoc); 
	xmlCleanupParser ();
	return 0;
}
int ReadFilterXml(GtkListStore *listStore,const char *nodeName)
{
	xmlDocPtr   pdoc = NULL;
	xmlNodePtr  proot = NULL, pTypeNode = NULL,pFilterNode = NULL,pEntryNode = NULL;
	GtkTreeIter iter;
	char	err_str[256];
	xmlChar *key; 
//	FILTER_T	customFilter;
	xmlKeepBlanksDefault(0);
	pdoc = xmlReadFile (FILE_OIX_CONFIG_XML, "GB2312", XML_PARSE_NOBLANKS);
	if (pdoc == NULL)
    {
		sprintf(err_str, "Fail to open %s", FILE_OIX_CONFIG_XML);
		Err_log(err_str);
		return -1;
    }
	
	proot = xmlDocGetRootElement (pdoc);
	if (proot == NULL)
    {
		sprintf(err_str, "%s not have root", FILE_OIX_CONFIG_XML);
		Err_log(err_str);
		return -1;
    }

	pTypeNode = proot->xmlChildrenNode;
	while (pTypeNode != NULL)
    {
		if (!xmlStrcmp (pTypeNode->name, BAD_CAST nodeName))
        {
			pFilterNode = pTypeNode->xmlChildrenNode;
			while(pFilterNode != NULL)
			{
				if (!xmlStrcmp (pFilterNode->name, BAD_CAST "FILTER"))
				{
					gtk_list_store_insert(listStore, &iter,1);
					gtk_list_store_set(listStore, &iter, eName_Filter, xmlGetProp(pFilterNode,BAD_CAST "name"),-1);
					pEntryNode = pFilterNode->xmlChildrenNode;
					while (pEntryNode != NULL)
					{ 
						if((!xmlStrcmp(pEntryNode->name, BAD_CAST "STN_ID")))
						{
							key=xmlNodeListGetString(pdoc, pEntryNode->xmlChildrenNode, 1);
							gtk_list_store_set(listStore, &iter, eStnId_Filter, key,-1);
							xmlFree(key);
						}
						else if((!xmlStrcmp(pEntryNode->name, BAD_CAST "STN_MASK")))
						{
							key=xmlNodeListGetString(pdoc, pEntryNode->xmlChildrenNode, 1);
							gtk_list_store_set(listStore, &iter, eStnMask_Filter, key,-1);
							xmlFree(key);
						}
						else if((!xmlStrcmp(pEntryNode->name, BAD_CAST "CMD_TYPE")))
						{
							key=xmlNodeListGetString(pdoc, pEntryNode->xmlChildrenNode, 1);
							gtk_list_store_set(listStore, &iter, eCmdType_Filter, key,-1);
							xmlFree(key);
						}
						else if((!xmlStrcmp(pEntryNode->name, BAD_CAST "FAULT_MASK")))
						{
							key=xmlNodeListGetString(pdoc, pEntryNode->xmlChildrenNode, 1);
							gtk_list_store_set(listStore, &iter, eFaultMask_Filter, key,-1);
							xmlFree(key);
						}
						else if((!xmlStrcmp(pEntryNode->name, BAD_CAST "TYPE_ID_MASK")))
						{
							key=xmlNodeListGetString(pdoc, pEntryNode->xmlChildrenNode, 1);
							gtk_list_store_set(listStore, &iter, eTypeIdMask_Filter, key,-1);
							xmlFree(key);
						}
						else if((!xmlStrcmp(pEntryNode->name, BAD_CAST "DEV_MASK")))
						{
							key=xmlNodeListGetString(pdoc, pEntryNode->xmlChildrenNode, 1);
							gtk_list_store_set(listStore, &iter, eDevMask_Filter, key,-1);
							xmlFree(key);
						}
						else if((!xmlStrcmp(pEntryNode->name, BAD_CAST "GROUP_MASK")))
						{
							key=xmlNodeListGetString(pdoc, pEntryNode->xmlChildrenNode, 1);
							gtk_list_store_set(listStore, &iter, eGroupMask_Filter, key,-1);
							xmlFree(key);
						}
						else if((!xmlStrcmp(pEntryNode->name, BAD_CAST "DATATYPE_MASK")))
						{
							key=xmlNodeListGetString(pdoc, pEntryNode->xmlChildrenNode, 1);
							gtk_list_store_set(listStore, &iter, eDataTypeMask_Filter, key,-1);
							xmlFree(key);
						}
						else if((!xmlStrcmp(pEntryNode->name, BAD_CAST "ATTRIB_MASK")))
						{
							key=xmlNodeListGetString(pdoc, pEntryNode->xmlChildrenNode, 1);
							gtk_list_store_set(listStore, &iter, eAttribMask_Filter, key,-1);
							xmlFree(key);
						}
						else if((!xmlStrcmp(pEntryNode->name, BAD_CAST "START_TIME")))
						{
							key=xmlNodeListGetString(pdoc, pEntryNode->xmlChildrenNode, 1);
							gtk_list_store_set(listStore, &iter, eStartTime_Filter, key,-1);
							xmlFree(key);
						}
						else if((!xmlStrcmp(pEntryNode->name, BAD_CAST "END_TIME")))
						{
							key=xmlNodeListGetString(pdoc, pEntryNode->xmlChildrenNode, 1);
							gtk_list_store_set(listStore, &iter, eEndTime_Filter, key,-1);
							xmlFree(key);
						}
						else if((!xmlStrcmp(pEntryNode->name, BAD_CAST "LOGIC_NAME")))
						{
							key=xmlNodeListGetString(pdoc, pEntryNode->xmlChildrenNode, 1);
							gtk_list_store_set(listStore, &iter, eLogicName_Filter, key,-1);
							xmlFree(key);
						}
						else if((!xmlStrcmp(pEntryNode->name, BAD_CAST "LONG_NAME")))
						{
							key=xmlNodeListGetString(pdoc, pEntryNode->xmlChildrenNode, 1);
							if (key != NULL)
							{
								gtk_list_store_set(listStore, &iter, eLongName_Filter, _fromUtf8(key),-1);
								xmlFree(key);
							}
						}
						pEntryNode = pEntryNode->next;
					}
				}
				pFilterNode = pFilterNode->next;
			}
        }
		pTypeNode = pTypeNode->next;
    }

	xmlFreeDoc (pdoc);
	xmlCleanupParser ();
	return gtk_tree_model_iter_n_children(GTK_TREE_MODEL(listStore),NULL);
}

//////////////////////////////////////////////////////////////////////////

GtkWidget *PtEventDlg[MAX_CRT_NUMBER];
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

GtkTreeModel* mx_treeview_create_model()
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

void mx_treeview_set_header(GtkWidget* treeview)
{
//	char str[500];
	gchar * pConvert=0;
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

void mx_treeview_insert_data(GtkTreeModel* store, gchar* num, gchar* time, gchar* logicname,gchar* msg,gchar* status,gchar* user)
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
gint ptEventDlgQuit(GtkWidget *widget,   gpointer data)
{
	int crt_n = (gint) data ;
	/*  GetCrtNumberOfWgt(widget,&crt_n);  */
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE; 
	
	gtk_widget_destroy(GTK_WIDGET(PtEventDlg[crt_n])); 
	PtEventDlg[crt_n] = (GtkWidget *)NULL ;
	
	return TRUE;
	
}

GtkWidget * createptEventDlg(int crt_n,POINTER pt)
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
		store = mx_treeview_create_model();
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), store);
	
    /* 设置表头 */
    mx_treeview_set_header(treeview);
	
    /* 插入数据 */
	gtk_object_set_data(GTK_OBJECT(window),"store",store);
	//  for(i = 0; i < TOP; i++)
	//      mx_treeview_insert_data(store, datas[i].lanage, datas[i].level, datas[i].dir);
// 	loadptEvent( store,crt_n,pt);	

		//loadptEvent( store,crt_n,pt);
	
    g_object_unref(store);
	
    /* 事件 */
    //g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
	gtk_signal_connect(GTK_OBJECT(window),
		"destroy",
		GTK_SIGNAL_FUNC(ptEventDlgQuit),
		GINT_TO_POINTER (crt_n));
    /* 显示 */
   gtk_widget_show_all(window);

    return window;
}

int loadptEvent( GtkTreeModel *store,int crt_n,POINTER pt)
{
//	int header_ptr;
	int i;
//	REC_HEAD *phead;
	ALM_STR			alarm_str ;
	char record[64];
//	char state_str[200];
	char hostname[64];
	char str[500];

	int index=0;

	gchar *pConvert=0, *pConvert1=0, *pConvert2=0, *pConvert3=0;

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
				pConvert1=_toUtf8EX(alarm_str.state_str), pConvert2=_toUtf8EX(alarm_str.cmd_result), pConvert3=_toUtf8EX(hostname ));
			G_SAFE_FREE(pConvert);
			G_SAFE_FREE(pConvert1);
			G_SAFE_FREE(pConvert2);
			G_SAFE_FREE(pConvert3);
			mx_treeview_insert_data(store,record,alarm_str.time_str, pConvert=_toUtf8EX(alarm_str.longname),
				pConvert1=_toUtf8EX(alarm_str.state_str), pConvert2=_toUtf8EX(alarm_str.cmd_result),pConvert3=_toUtf8EX(hostname ));
			G_SAFE_FREE(pConvert);
			G_SAFE_FREE(pConvert1);
			G_SAFE_FREE(pConvert2);
			G_SAFE_FREE(pConvert3);
			//	mx_treeview_insert_data(store,str,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
			
			index++;
			
		}
		start=(start+1)%shm_rec_addr->head.buf_size;
		
	}

	for (i=0;i<20;i++)
	{
		
		mx_treeview_insert_data(store,NULL,NULL,NULL,NULL,
				NULL,NULL);
	}
	return TRUE;
}
int ptEventProg(GtkWidget *parent,int crt_n,POINTER pt)
{
	
	GtkTreeModel *store;
	if (PtEventDlg[crt_n]==(GtkWidget*)NULL)
	{
		PtEventDlg[crt_n]=createptEventDlg(crt_n,pt);
	}else
	{
		gtk_window_present(GTK_WINDOW(PtEventDlg[crt_n]));
	}

	store=gtk_object_get_data(GTK_OBJECT(PtEventDlg[crt_n]), "store");
	
	
	loadptEvent( store,crt_n,pt);
	return TRUE;
}
/*gint GetGroupDef(void)
{
	int i,j,station_num,group_num;
	FILE *fp;
	char err_str[256];
	char buffer[MAX_CHAR_NUM_IN_LINE];
	char strbuf[5][MAX_CHAR_NUM_IN_LINE];
	int lineNo = 0;
	char stationName[STATION_NAME_SIZE],devName[GROUP_NAME_SIZE];
	if( (fp=fopen(GROUP_DEFINE_FILE, "r"))==NULL )
	{
		sprintf(err_str, "Fail to open %s", GROUP_DEFINE_FILE);
		Err_log(err_str);
		return -1;
	}
	GetStationNum(&station_num);
	while(TRUE)
	{
		lineNo++;
		if (fgets(buffer,sizeof(buffer),fp)==NULL)
			break;
		
		if (buffer[0]=='\t')
			buffer[0]=' ';

		if (GetStringsFromStr(buffer,sizeof(strbuf)/sizeof(strbuf[0]),strbuf)==-1)
		{
			sprintf(err_str, "%s line %d: %s: Read error", lineNo, buffer);
			Err_log(err_str);
			return -1;
		}

		if ( !strcmp(strbuf[0], "") )
			continue;
		
		if ( !strcmp(strbuf[0], "END") )
			break;

		if (!strcmp(strbuf[0],"STATION") & !strcmp(strbuf[1],"NAME"))
		{
			if(!strcmp(strbuf[2], ""))
			{
				sprintf(err_str, "DBIOGROUP.DEF line %d: %s: No station name", lineNo, buffer);
				Err_log(err_str);
				return -1;
			}
			strcpy(stationName,strbuf[2]);
			for (i=1;i<=station_num;i++)
			{
				if (stationName==NULL)
				{
					sprintf(err_str, "get stationName error!");
					Err_log(err_str);
					return -1;
				}
				if (!strcmp(alarmStnDef[i].Name,stationName))
					break;
			}
			continue;
		}

		if (!strcmp(strbuf[0],"GROUP") & !strcmp(strbuf[1],"NAME"))
		{
			if(!strcmp(strbuf[2], ""))
			{
				sprintf(err_str, "DBIOGROUP.DEF line %d: %s: No dev name", lineNo, buffer);
				Err_log(err_str);
				return -1;
			}
			group_num = 0;
			strcpy(devName,strbuf[2]);
			for (j=1;j<=alarmStnDef[i].dev_num;j++)
			{
				if (stationName==NULL)
				{
					sprintf(err_str, "get devName error!");
					Err_log(err_str);
					return -1;
				}
				if (!strcmp(alarmStnDef[i].dev[j].Name,devName))
					break;
			}
			continue;
		}
		if (!strcmp(strbuf[0],"IOGROUP_ID"))
		{
			if (!sscanf(strbuf[1], "%d", &alarmStnDef[i].dev[j].group[++group_num].Id))
			{
				sprintf(err_str, "DBIOGROUP.DEF line %d: %s: group_id error", 
					lineNo, buffer);
				Err_log(err_str);
				return -1;
			}
			continue;
		}
		if (!strcmp(strbuf[0],"IOGROUP_COMMENT"))
		{
			if(strlen(strbuf[1])>=GROUP_NAME_SIZE)
			{
				sprintf(err_str, "DBIOGROUP.DEF line %d: %s: group name too long", 
					lineNo, buffer);
				Err_log(err_str);
				strbuf[1][GROUP_NAME_SIZE-1]=0;
				return -1;
			}
			strcpy(alarmStnDef[i].dev[j].group[group_num].CName, strbuf[1]);
			alarmStnDef[i].dev[j].group_num = group_num;
			continue;
		}
	}
	return 0;
}*/
