
/****
2005.11 updated by zyp by gtk  ;
2006.04.10  UNIX WINDOWS SYNC by chichi;
****/

#include <gtk/gtk.h>
#include <gdk/gdk.h>


#include <stdio.h>
#include <math.h>
#include <time.h>

#include	"../inc/oix.h"
#include	"../inc/gtk_widgets.h"
 
/*  #define	DEBUG   */

GtkAdjustment  *adj2[MAX_CRT_NUMBER] ;
PAGE_INFO		pageDb[MAX_CRT_NUMBER];
void 	DspPolDB (int crt_n,GdkDrawable* w,GdkGC *gc );
void	DspImpDB ( int crt_n,GdkDrawable* w,GdkGC *gc  );
int		DbDspProc ( int crt_n);
void 	DbTabPageControl (int crt_n,int direction );
void	DbDspProc1 ( GdkDrawable *drawable ,int crt_n );
void	AcknDbTabFlag (GtkWidget * db,int crt_n );
void  cb_scroll_page_db(GtkAdjustment *adj , int crt_n);

extern void DrawDbItemTag ( int crt_n, GdkDrawable *drawable,GdkGC *gc );
extern void GetCrtNumberOfWgt ( GtkWidget  *wgt, int     *crt_n );
extern void SetPageInfo(PAGE_INFO *PageInfo );
extern void SetAdjInfo(PAGE_INFO *PageInfo,GtkAdjustment *AdjSet);
extern void    windk_draw_text(GdkDrawable * drawable ,GdkFont    *font,GdkGC  *gc, int x,int y,const char *text,int  strlen ,int crt_n) ;
extern void	BlankFillCopy (char    *dst,char    *src,int len);
extern void DrawDbItemTagNew ( int crt_n, GdkDrawable *drawable,GdkGC  *gc ,int x ,int y );
extern void GetYesNoStr(int value,char *str);
extern void	PopupMessageBox (GtkWidget *parent,char msg[] );
extern void 	CmdBufInit ( int crt_n );
extern void	ProgDoAckCelBrdcst (int crt_n );
extern int getMenuDataPointer(int crtn,int data);
extern int getCrtNumFromPointer(int data);
extern int getDataFromPointer(int data);
extern char* _CS_(	char* indent );
extern char *_toUtf8EX(char *c);
extern char *_toUtf8(char *c);
void DbTracingCB (GtkWidget * db,GdkEventButton *event, gpointer        data);

GdkPixmap *DbTabOffDrawable[MAX_CRT_NUMBER] ;
GtkWidget *db_canvas[MAX_CRT_NUMBER] ;
GtkWidget *db_tab_search_dlg[MAX_CRT_NUMBER] ;
void	UnmanageDbIoandInfoBox(int crt_n);

static	char	buf[600];
char	LongNameStr[100];

int db_timeout_id[MAX_CRT_NUMBER];
int db_blink_st[MAX_CRT_NUMBER];

void UpdateDbDevMenu(int crt_n); 
void UpdateDbDataTypeMenu(int crt_n); 
void GetObjFixedInfo(int crt_n,DMS_COMMON *db_cmm,char value_str[30][POINT_LONGNAME_SIZE],char label_str[20][POINT_LONGNAME_SIZE],int *col_num);

DB_TAB_WINDOW_INFO db_tab_window_info[MAX_CRT_NUMBER]; //数据一览表点详细信息,此全局变量改变会影响数据一览表
DB_TAB_WINDOW_INFO db_tab_info_tmp[MAX_CRT_NUMBER];//其他方式查看点详细信息
	char	*scan_str[2]		= { DPS_STR_PROHIBIT, DPS_STR_ALLOW };
	char	*alm_str[2]		= { DPS_STR_PROHIBIT, DPS_STR_ALLOW };
	char	*ack_str[2]		= { DPS_STR_ACKED, DPS_STR_UNACKED };


	char	*audio_str[2]		= { OIX_CSTR_NO, OIX_CSTR_YES };
	char	*mimic_str[2]		= { OIX_CSTR_NO, OIX_CSTR_YES };

	char	*dq_str[4]		= { DPS_STR_NORMAL, DPS_STR_MANUAL, DPS_STR_FAIL, "xxx" };


int   dbtab_scroll_event (GtkWidget*widget,   GdkEventScroll *event, gpointer  data)  
{   int crt_n  = (int) data; 
    
	if (event->direction == GDK_SCROLL_DOWN)
	{
      // down code
		adj2[crt_n]->value ++ ; 
//printf ("---------scroll++++++++++++++++++++1\n");
	}
    else if (event->direction == GDK_SCROLL_UP)
    {
      // up code
		adj2[crt_n]->value -- ; 
//printf ("---------scroll---------------------1\n");
    }

    
    
	cb_scroll_page_db(adj2[crt_n] ,   crt_n);
   
	return TRUE;
		
}

/*****************************
 input CurPage &&  RecCount %;
******************************/
void InitPageDbInfo(PAGE_INFO *PageInfo , int crt_n,int recCount)
{
		if(-1==GetEntryNumById( db_tab_window_info[crt_n].select_pt.stn_id, 
			db_tab_window_info[crt_n].select_pt.dev_id, db_tab_window_info[crt_n].select_pt.data_type,&db_pid_cnt[crt_n] ))
				return;
       PageInfo->RecCount   = db_pid_cnt[crt_n] ;
       PageInfo->PageCount  = (PageInfo->RecCount+PageInfo->PageSize-1)/PageInfo->PageSize ;
       PageInfo->CurPage    =   0 ;
       PageInfo->isSelected =-1;
	   PageInfo->SelectStart = PageInfo->SelectEnd =-1;
       PageInfo->PageStart  =  (PageInfo->CurPage) * (PageInfo->PageSize) ;
       PageInfo->PageEnd    =   MIN ((PageInfo->PageStart + PageInfo->PageSize -1) , (PageInfo->RecCount-1) )   ;        

}

void  db_canvas_allocate(GtkWidget *widget,GtkAllocation *a ,gpointer data) 
{
        int crt_n = (gint) data ; 
       // AlarmCanasRECT[crt_n].width =  a->width ;
       // AlarmCanasRECT[crt_n].height =  a->height ;
		pageDb[crt_n].PageSize =  ( a->height-50)/ (int)(DBH0*yscale[crt_n]) ;
        pageDb[crt_n].PageSize -- ;   
 /* g_message("alloresize ****** alarm page is %d \n", page[crt_n].PageSize);  */
        
}


void	SetBlinkForeground (int crt_n, GdkGC *gc, int ackn_st, COLOR_PAIR	*cp )
{
 	int	color;

	if ( ackn_st == UNACKN )
	{
         	color	= cp->clr[db_blink_st[crt_n]];
	}
        else
	{
         	color	= cp->clr[0];
	}
      
	gdk_gc_set_foreground ( gc, &pixels[crt_n][color]);
}

void	DbBarIncrease (GtkWidget *w, gpointer data)
{
	int	crt_n=0;

	 GetCrtNumberOfWgt ( w, &crt_n );
	if( crt_n < 0 )
		return ;
//	DbTabPageControl ( crt_n, WgtN_db_page_down );

	pageDb[crt_n].CurPage ++ ;
    if ( pageDb[crt_n].CurPage > MAX( pageDb[crt_n].PageCount -1   ,0)) 
	{
		gdk_beep(    ); 
 		pageDb[crt_n].CurPage  = pageDb[crt_n].PageCount  -1 ;
                return ;
	} 
	SetPageInfo (&pageDb[crt_n] );
    pageDb[crt_n].PageStart =  (pageDb[crt_n].CurPage) * (pageDb[crt_n].PageSize) ;
    pageDb[crt_n].PageEnd   =   MIN ((pageDb[crt_n].PageStart + pageDb[crt_n].PageSize -1) , (pageDb[crt_n].RecCount-1) )   ;
    SetAdjInfo(&pageDb[crt_n],adj2[crt_n]);

	DbDspProc ( crt_n );



}

void	DbBarDecrease (GtkWidget *w, gpointer data)
{
	int	crt_n=0;

	 GetCrtNumberOfWgt ( w, &crt_n );
	if( crt_n < 0 )
		return; 
//	DbTabPageControl ( crt_n, WgtN_db_page_up );
		pageDb[crt_n].CurPage -- ;
        if (pageDb[crt_n].CurPage < 0  )  
    	{
			gdk_beep(    ); 
 			pageDb[crt_n].CurPage  = 0 ;
                return ; 
		}
		SetPageInfo (&pageDb[crt_n]);
        
        pageDb[crt_n].PageStart =  (pageDb[crt_n].CurPage) * (pageDb[crt_n].PageSize) ;
        pageDb[crt_n].PageEnd   =   MIN ((pageDb[crt_n].PageStart + pageDb[crt_n].PageSize -1) , (pageDb[crt_n].RecCount-1) )   ;
        SetAdjInfo(&pageDb[crt_n],adj2[crt_n]);

			DbDspProc ( crt_n );
}



void DbTabPageControl (int crt_n,int direction )
{
	 switch	( direction )
	{
	case	WgtN_db_page_up:
		if ( db_page_cnt[crt_n] > 0 )
			db_page_cnt[crt_n]--;
		else
			db_page_cnt[crt_n]= db_page_numb[crt_n]-1;
		break;

	case	WgtN_db_page_down:
		if ( db_page_cnt[crt_n] < db_page_numb[crt_n]-1 )
			db_page_cnt[crt_n]++;
		else
			db_page_cnt[crt_n]= 0;
			break;

	default:
			break;
	}

	DbDspProc ( crt_n );

}

void  cb_scroll_page_db(GtkAdjustment *adj , int crt_n)
{
    
 
    int maxNum ; 
   //PageInfo->RecCount   =
    maxNum  =    db_pid_cnt[crt_n] ;;
  //  printf ("adj.value = %d,adj.upper = %8.3f,adj.page_increment = %8.3f,crt = %d\n",(int)adj->value,adj->upper,adj->page_increment,crt_n);   
    pageDb[crt_n].PageStart = MIN((int)adj->value,maxNum-pageDb[crt_n].PageSize)  ; 
    if (pageDb[crt_n].PageStart <0 ) pageDb[crt_n].PageStart  = 0 ;
    pageDb[crt_n].PageEnd   =  MIN ((pageDb[crt_n].PageStart + pageDb[crt_n].PageSize -1) , (pageDb[crt_n].RecCount-1) )   ;
    

   DbDspProc(crt_n);

	 
}

void GetIostBitsStr (IOST iost,DB_DA_STATUS_NAME *np )
{	
  	strcpy( np->name_scan,_CS_(scan_str[iost.scanEnable]));
	strcpy( np->name_alm,_CS_(alm_str[iost.almEnable]));
	strcpy( np->name_almack, _CS_(ack_str[iost.almNoAck]));
	
	return;
}

void DrawTempDbHead(GdkDrawable *drawable, GdkGC *gc,int crt_n)
{ 	
	int item=0,temp_init_pos=220;

	windk_draw_text( drawable,  font_set[DBTAB_FONT], gc, DBX0, DBY1, _CS_(OIX_STR_IO_ADDRESS), -1,crt_n);
	windk_draw_text( drawable, font_set[DBTAB_FONT], gc, 60, DBY1, _CS_(OIX_STR_TEMP_NAME), -1 ,crt_n);	
	
	for(item=2;item<TEMP_col;item++)		
		windk_draw_text( drawable, font_set[DBTAB_FONT], gc,temp_init_pos+DB_VAL_STR_WIDTH*item, DBY1,_CS_(TEMP_titles[item]) , -1,crt_n);	

}

void DrawImpDbHead(GdkDrawable *drawable, GdkGC *gc,int crt_n)
{
	int item=0,imp_init_pos=170;
	windk_draw_text( drawable, font_set[DBTAB_FONT], gc,   DBX0, DBY1, _CS_(OIX_STR_IO_ADDRESS), strlen(OIX_STR_IO_ADDRESS) ,crt_n);
	windk_draw_text( drawable, font_set[DBTAB_FONT], gc, DBX0+50, DBY1, _CS_(OIX_STR_UPDATE_TIME), strlen(OIX_STR_UPDATE_TIME) ,crt_n);

        windk_draw_text( drawable, font_set[DBTAB_FONT], gc, DBX0+140, DBY1, _CS_(OIX_CSTR_NAME), strlen(OIX_CSTR_NAME),crt_n );
	
        windk_draw_text( drawable, font_set[DBTAB_FONT], gc, 300, DBY1, _CS_(OIX_STR_PULSE_NAME), strlen(OIX_STR_PULSE_NAME),crt_n );
        
        windk_draw_text( drawable, font_set[DBTAB_FONT], gc, 500, DBY1, _CS_(OIX_CSTR_CHANNEL_QULITY), strlen(OIX_CSTR_CHANNEL_QULITY) ,crt_n);

	for(item=4;item<IMP_col;item++)		
		windk_draw_text( drawable, font_set[DBTAB_FONT], gc,imp_init_pos+(DB_VAL_STR_WIDTH+20)*item, DBY1,_CS_(IMP_titles[item]) , strlen(IMP_titles[item]) ,crt_n);
}


void DrawAnaDbHead(GdkDrawable *drawable, GdkGC *gc, int data_type,int crt_n)
{
	int db_val_col=0,db_str_col=0,db_init_width;

	windk_draw_text( drawable, font_set[DBTAB_FONT], gc, DBX0, DBY1, _CS_(OIX_STR_IO_ADDRESS), -1 ,crt_n);
    windk_draw_text( drawable, font_set[DBTAB_FONT], gc, DBX0+50, DBY1, _CS_(OIX_STR_UPDATE_TIME), -1 ,crt_n);

    windk_draw_text( drawable, font_set[DBTAB_FONT], gc, DBX0+150, DBY1, _CS_(OIX_CSTR_NAME), -1,crt_n );
	switch  ( data_type )
	{
		case    ANA_TYPE:
		windk_draw_text( drawable, font_set[DBTAB_FONT], gc, DBX0+280, DBY1, _CS_(OIX_STR_ANALOG_NAME), -1,crt_n );		
			break;
		case    PARAM_TYPE:
		windk_draw_text( drawable, font_set[DBTAB_FONT], gc, DBX0+280, DBY1, _CS_(OIX_STR_SETTING_NAME), -1 ,crt_n);			
			break;
		case    OUTA_TYPE:
		windk_draw_text( drawable, font_set[DBTAB_FONT], gc, DBX0+280, DBY1, _CS_(OIX_STR_OUTA_NAME), -1 ,crt_n);
			break;	
		case	CALC_TYPE:
		windk_draw_text( drawable, font_set[DBTAB_FONT], gc, DBX0+280, DBY1, _CS_(OIX_STR_CALC_NAME), -1 ,crt_n);
		break;
		default:
			break;
	}	
	windk_draw_text( drawable, font_set[DBTAB_FONT], gc, DBX0+580, DBY1, _CS_(OIX_STR_RT_VALUE), -1 ,crt_n);
	db_init_width = DBX0+580;
	db_val_col++;
        windk_draw_text( drawable, font_set[DBTAB_FONT], gc, db_init_width+DB_VAL_COL_WIDTH*db_val_col+DB_VAL_STR_WIDTH*db_str_col, DBY1, _CS_(OIX_STR_H_WRN_LMT), -1,crt_n );
        db_val_col++;
        windk_draw_text( drawable, font_set[DBTAB_FONT], gc, db_init_width+DB_VAL_COL_WIDTH*db_val_col+DB_VAL_STR_WIDTH*db_str_col, DBY1, _CS_(OIX_STR_L_WRN_LMT), -1,crt_n );
        db_val_col++;  
	windk_draw_text( drawable, font_set[DBTAB_FONT], gc, db_init_width+DB_VAL_COL_WIDTH*db_val_col+(DB_VAL_STR_WIDTH-5)*db_str_col, DBY1, _CS_(OIX_CSTR_CHANNEL_QULITY), -1,crt_n );
	db_str_col++;
        windk_draw_text( drawable, font_set[DBTAB_FONT], gc, db_init_width+DB_VAL_COL_WIDTH*db_val_col+(DB_VAL_STR_WIDTH-5)*db_str_col, DBY1, _CS_(OIX_CSTR_BLOCK), -1,crt_n );
	db_str_col++;	
        windk_draw_text( drawable, font_set[DBTAB_FONT], gc, db_init_width+DB_VAL_COL_WIDTH*db_val_col+(DB_VAL_STR_WIDTH-5)*db_str_col, DBY1, _CS_(OIX_STR_SCAN), -1,crt_n );
	db_str_col++;
	windk_draw_text( drawable, font_set[DBTAB_FONT], gc, db_init_width+DB_VAL_COL_WIDTH*db_val_col+(DB_VAL_STR_WIDTH-5)*db_str_col, DBY1, _CS_(OIX_STR_ALM), -1,crt_n );
	db_str_col++;
	windk_draw_text( drawable, font_set[DBTAB_FONT], gc, db_init_width+DB_VAL_COL_WIDTH*db_val_col+(DB_VAL_STR_WIDTH-5)*db_str_col, DBY1, _CS_(OIX_CSTR_AlarmAck), -1 ,crt_n);
}

void	DspAnaDB ( int crt_n,GdkDrawable *drawable ,GdkGC *gc )
{
	POINTER		point;
	int		db_val_col,db_str_col,db_init_width;

	int     db_ackn_st;
	char    flag_lmt;
	int     i;
	int	cur_page_startid,cur_page_endid;
	char  LongNamerStr1[128];
	DMS_COMMON	db_cmm;

	DB_DA_STATUS_NAME	ana_sta;
        char    channel[10],block[10],h_lmt[20],l_lmt[20];

	char	real_data_text[16],entry_name[ENTRY_NAME_SIZE];
	
	/*char	time_tm[20];*/

	gdk_gc_set_foreground ( gc, &pixels[crt_n][L_BLUE1]);

	point= db_tab_window_info[crt_n].select_pt;
	DrawAnaDbHead( drawable,gc,point.data_type,crt_n);
       /* printf("db_page_cnt=%d,db_pid_cnt=%d\n",db_page_cnt[crt_n],db_pid_cnt[crt_n]);*/
/*	cur_page_startid= db_page_cnt[crt_n] * DBTAB_PAGE_SIZE;
	cur_page_endid	= Min (( db_page_cnt[crt_n]+1)*DBTAB_PAGE_SIZE, db_pid_cnt[crt_n] );
*/
 
	cur_page_startid = pageDb[crt_n].PageStart ;
	cur_page_endid	=  pageDb[crt_n].PageEnd   ;
 
	for ( i= cur_page_startid; i <= cur_page_endid ; i++)
	{
		db_val_col = 0;
		db_str_col = 0;

		point.pt_id	= i;
		
		if ( ReadEntryById (&point, &db_cmm ) == -1)
			continue;
        
               
		
        if(db_cmm.var.anlg.value.status.chan_fail)                    
                    strcpy(channel,_CS_(DPS_STR_FAIL));                
		else
			strcpy(channel,_CS_(DPS_STR_NORMAL));
                
        if(db_cmm.var.anlg.value.status.ctrl_block)
                    strcpy(block,_CS_(DPS_STR_CTRL_BLOCK));  
		else
			strcpy(block,_CS_(DPS_STR_NORMAL));   
                
		GetIostBitsStr (db_cmm.fixed.iost, &ana_sta );
        sprintf (h_lmt, "%.2f",  db_cmm.var.anlg.lmt_para.HI_WARN );
        sprintf (l_lmt, "%.2f",  db_cmm.var.anlg.lmt_para.LO_WARN );
 
        strcpy(entry_name,db_cmm.fixed.entryName);
		GetGroupLongnameById( db_cmm.point.stn_id, db_cmm.point.dev_id, LongNameStr );
		GetEntryLongnameById (&db_cmm.point,LongNamerStr1 );
		strcat( LongNameStr, LongNamerStr1 );
		BlankFillCopy (name_item, LongNameStr, POINT_LONGNAME_SIZE);		
        flag_lmt    =(char) (db_cmm.var.anlg.value.status.lmt_state);
		db_ackn_st	= db_cmm.fixed.iost.almNoAck;

       if (i == pageDb[crt_n].SelectEnd)
            DrawDbItemTagNew (  crt_n,  drawable, gc ,DBX0 , DBY0 +( i-cur_page_startid)*DBH0 -15);

	//	gdk_gc_set_foreground ( gc, &pixels[crt_n][GREEN3]);				
		switch (flag_lmt)
        {
                case    ANA_ALM_NORMAL:
                case    ANA_ALM_HI_RECOVER:
                case    ANA_ALM_LO_RECOVER:
                        gdk_gc_set_foreground ( gc, &pixels[crt_n][GREEN3]);	
                        break;                                

                case    ANA_ALM_HI_WARN:
                case    ANA_ALM_LO_WARN:
                case    ANA_ALM_HI_ALARM:
                case    ANA_ALM_LO_ALARM:
                        SetBlinkForeground ( crt_n, gc, db_ackn_st, &red_cp );
                        break;           
                       

                default : break;
         }

                
		/*sprintf (str_time,"%d-%d-%d ", db_cmm.var.anlg.update_time.year,db_cmm.var.anlg.update_time.month,db_cmm.var.anlg.update_time.day);
		*/	
		sprintf (str_time, "%02d:%02d:%02d", db_cmm.var.anlg.update_time.hour,
			db_cmm.var.anlg.update_time.minute,db_cmm.var.anlg.update_time.second);
		
		/*strcat (str_time, time_tm);*/
	
		sprintf (real_data_text, "%.3f", db_cmm.var.anlg.fValue) ; 
		
		sprintf(buf,"%4d",i);			

		windk_draw_text( drawable, font_set[DBTAB_FONT], gc, DBX0 , DBY0 +( i-cur_page_startid)*DBH0,buf, strlen(buf) ,crt_n);
		windk_draw_text( drawable, font_set[DBTAB_FONT], gc, DBX0+50 , DBY0 +( i-cur_page_startid)*DBH0, str_time, strlen(str_time) ,crt_n);	

                windk_draw_text( drawable, font_set[DBTAB_FONT], gc, DBX0+150 , DBY0 +( i-cur_page_startid)*DBH0,entry_name, strlen(entry_name),crt_n );
                sprintf(buf,"%s",name_item);
		windk_draw_text( drawable, font_set[DBTAB_FONT], gc, DBX0+280 , DBY0 +( i-cur_page_startid)*DBH0,buf, strlen(buf) ,crt_n);					
		
		windk_draw_text( drawable, font_set[DBTAB_FONT], gc, DBX0+580 , DBY0 +( i-cur_page_startid)*DBH0, real_data_text, strlen(real_data_text) ,crt_n);
		db_init_width= DBX0+580;
		db_val_col++;         
                windk_draw_text( drawable, font_set[DBTAB_FONT], gc, db_init_width+DB_VAL_COL_WIDTH*db_val_col+DB_VAL_STR_WIDTH*db_str_col , DBY0 +( i-cur_page_startid)*DBH0, h_lmt, strlen(h_lmt),crt_n );	
		db_val_col++;
                windk_draw_text( drawable, font_set[DBTAB_FONT], gc, db_init_width+DB_VAL_COL_WIDTH*db_val_col+DB_VAL_STR_WIDTH*db_str_col , DBY0 +( i-cur_page_startid)*DBH0, l_lmt, strlen(l_lmt),crt_n );	
		db_val_col++;
		windk_draw_text( drawable, font_set[DBTAB_FONT], gc, db_init_width+DB_VAL_COL_WIDTH*db_val_col+(DB_VAL_STR_WIDTH-5)*db_str_col , DBY0 +( i-cur_page_startid)*DBH0, channel, strlen(channel),crt_n );	
		db_str_col++;
                windk_draw_text( drawable, font_set[DBTAB_FONT], gc, db_init_width+DB_VAL_COL_WIDTH*db_val_col+(DB_VAL_STR_WIDTH-5)*db_str_col , DBY0 +( i-cur_page_startid)*DBH0, block, strlen(block),crt_n );	
		db_str_col++;
                
		windk_draw_text( drawable, font_set[DBTAB_FONT], gc, db_init_width+DB_VAL_COL_WIDTH*db_val_col+(DB_VAL_STR_WIDTH-5)*db_str_col , DBY0 +( i-cur_page_startid)*DBH0, ana_sta.name_scan, strlen(ana_sta.name_scan),crt_n );	
		db_str_col++;
		windk_draw_text( drawable, font_set[DBTAB_FONT], gc, db_init_width+DB_VAL_COL_WIDTH*db_val_col+(DB_VAL_STR_WIDTH-5)*db_str_col , DBY0 +( i-cur_page_startid)*DBH0, ana_sta.name_alm, strlen(ana_sta.name_alm) ,crt_n);	
		db_str_col++;
		windk_draw_text( drawable, font_set[DBTAB_FONT], gc, db_init_width+DB_VAL_COL_WIDTH*db_val_col+(DB_VAL_STR_WIDTH-5)*db_str_col , DBY0 +( i-cur_page_startid)*DBH0, ana_sta.name_almack, strlen(ana_sta.name_almack),crt_n );
	}	
}

void DrawCurveDbHead(GdkDrawable *drawable, GdkGC *gc,int  crt_n)
{
	int db_val_col=0,item=0,db_init_width;

	windk_draw_text( drawable, font_set[DBTAB_FONT], gc, DBX0, DBY1, _CS_(OIX_STR_IO_ADDRESS), -1 ,crt_n );
	
	windk_draw_text( drawable, font_set[DBTAB_FONT], gc, 150, DBY1, _CS_(OIX_STR_CURVE_NAME), -1,crt_n );			
		
	windk_draw_text( drawable, font_set[DBTAB_FONT], gc, 350, DBY1, _CS_(OIX_STR_UPDATE_TIME), -1,crt_n );
	windk_draw_text( drawable, font_set[DBTAB_FONT], gc, 480, DBY1, _CS_(OIX_STR_SEGMENT_NUM), -1,crt_n );
	
	db_init_width = 170;		

	for(item=4;item<CURVE_col;item++)		
		windk_draw_text( drawable, font_set[DBTAB_FONT], gc,db_init_width+DB_VAL_COL_WIDTH*item, DBY1,_CS_(CURVE_titles[item]) , -1,crt_n );

}

void	DspCurveDB ( int crt_n,GdkDrawable *drawable ,GdkGC *gc )
{
	POINTER		point;
	int			db_val_col,db_str_col,db_init_width;	
	
	int			i,item;
	int			cur_page_startid,cur_page_endid;
	char LongNamerStr1[128];
	DMS_COMMON	db_cmm;	
	
	char 	ana_tr_hi_range[100],ana_tr_lo_range[100], ana_tr_base[100], ana_tr_compensate[100],
		ana_tr_dead_band[100],ana_tr_eng_factor[100];	
 
	char	curve_point_num[16];
	
	char	time_tm[20],value_str[CURVE_col][POINT_NAME_SIZE];

	gdk_gc_set_foreground ( gc, &pixels[crt_n][L_BLUE1]);

	DrawCurveDbHead( drawable,gc,crt_n);
        
   
/*		
	cur_page_startid= db_page_cnt[crt_n] * DBTAB_PAGE_SIZE;
	cur_page_endid	= MIN (( db_page_cnt[crt_n]+1)*DBTAB_PAGE_SIZE, db_pid_cnt[crt_n] );
*/
	cur_page_startid = pageDb[crt_n].PageStart ;
	cur_page_endid	=  pageDb[crt_n].PageEnd   ;

	point= db_tab_window_info[crt_n].select_pt;
	for ( i= cur_page_startid; i <= cur_page_endid; i++)
	{
		db_val_col = 0;
		db_str_col = 0;

		point.pt_id	= i;

      if (i == pageDb[crt_n].SelectEnd)
            DrawDbItemTagNew (  crt_n,  drawable, gc ,DBX0 , DBY0 +( i-cur_page_startid)*DBH0 -15);

     gdk_gc_set_foreground ( gc, &pixels[crt_n][GREEN3]);		
		if ( ReadEntryById (&point, &db_cmm ) == -1)
			continue;

		sprintf (curve_point_num,"%d",db_cmm.var.curve.seg_num );

		sprintf (ana_tr_hi_range, "%.3f", db_cmm.var.curve.eng_para.HI_RANGE );
		sprintf (ana_tr_lo_range, "%.3f", db_cmm.var.curve.eng_para.LO_RANGE ); 
		sprintf (ana_tr_base, "%.3f", db_cmm.var.curve.eng_para.ZERO ); 
		sprintf (ana_tr_compensate, "%.3f",db_cmm.var.curve.eng_para.COMPSATE ); 				
		sprintf (ana_tr_dead_band, "%.3f",db_cmm.var.curve.eng_para.D_BAND); 				
		sprintf (ana_tr_eng_factor, "%.3f", db_cmm.var.curve.eng_para.ENG_CVT );

		GetGroupLongnameById( db_cmm.point.stn_id, db_cmm.point.dev_id, LongNameStr );
		GetEntryLongnameById (&db_cmm.point,LongNamerStr1 );
		strcat( LongNameStr, LongNamerStr1 );
	
		BlankFillCopy (name_item, LongNameStr, POINT_LONGNAME_SIZE);		
		
		sprintf (str_time,"%d-%d-%d ", db_cmm.var.curve.update_time.year,db_cmm.var.curve.update_time.month,db_cmm.var.curve.update_time.day);			
		
		sprintf (time_tm, "%02d:%02d:%02d", db_cmm.var.curve.update_time.hour,
			db_cmm.var.curve.update_time.minute,db_cmm.var.curve.update_time.second);
		
		strcat (str_time, time_tm);
		
		sprintf(buf,"%4d",i);			

		windk_draw_text( drawable, font_set[DBTAB_FONT], gc, DBX0 , DBY0 +( i-cur_page_startid)*DBH0,buf, strlen(buf) ,crt_n);
		sprintf(buf,"%s",name_item);
		windk_draw_text( drawable, font_set[DBTAB_FONT], gc, 60 , DBY0 +( i-cur_page_startid)*DBH0,buf, strlen(buf),crt_n );					
		
		windk_draw_text( drawable, font_set[DBTAB_FONT], gc, 350 , DBY0 +( i-cur_page_startid)*DBH0, str_time, strlen(str_time),crt_n );	
		windk_draw_text( drawable, font_set[DBTAB_FONT], gc, 480 , DBY0 +( i-cur_page_startid)*DBH0, curve_point_num, strlen(curve_point_num),crt_n );	
		db_init_width= 570;
		
		item=4;
		strcpy(value_str[item],ana_tr_hi_range);item++;
		strcpy(value_str[item],ana_tr_lo_range);item++;
		strcpy(value_str[item],ana_tr_base);	item++;
		strcpy(value_str[item],ana_tr_compensate);item++;
		strcpy(value_str[item],ana_tr_dead_band);item++;
		strcpy(value_str[item],ana_tr_eng_factor);item++;
		db_init_width = 170;		

		for(item=4;item<CURVE_col;item++)		
			windk_draw_text( drawable, font_set[DBTAB_FONT], gc,db_init_width+DB_VAL_COL_WIDTH*item, DBY0 +( i-cur_page_startid)*DBH0,value_str[item] , strlen(value_str[item]),crt_n );
	}	
}

void DrawObjDbHead(GdkDrawable *drawable, GdkGC *gc,char label_obj[50][POINT_LONGNAME_SIZE],int col,int crt_n )
{
	int db_val_col=0,item=0,db_init_width;

	windk_draw_text( drawable, font_set[DBTAB_FONT], gc, DBX0, DBY1, _CS_(label_obj[item]), -1,crt_n );
	item++;
	windk_draw_text( drawable, font_set[DBTAB_FONT], gc, 60, DBY1,  _CS_(label_obj[item]), -1,crt_n );			
	item++;	
	windk_draw_text( drawable, font_set[DBTAB_FONT], gc, 180, DBY1,  _CS_(label_obj[item]), -1 ,crt_n);
        item++;
	windk_draw_text( drawable, font_set[DBTAB_FONT], gc, 300, DBY1,  _CS_(label_obj[item]), -1,crt_n );
	item++;
	db_init_width = 450;		

	for(item=4;item<col;item++)		
		windk_draw_text( drawable, font_set[DBTAB_FONT], gc,db_init_width+DB_VAL_STR_WIDTH*(item-3), DBY1,_CS_(label_obj[item]) , -1 ,crt_n);

}

void	DspObjDB ( int crt_n,GdkDrawable *drawable ,GdkGC *gc )
{
	POINTER		point;
	int			db_val_col,db_str_col,db_init_width;	
	char    val_obj[50][POINT_LONGNAME_SIZE],label_obj[50][POINT_LONGNAME_SIZE];
        
	int			i,item,col_num;
	int			cur_page_startid,cur_page_endid;
	
	DMS_COMMON	db_cmm;		

	gdk_gc_set_foreground ( gc, &pixels[crt_n][L_BLUE1]);
/*	
	cur_page_startid= db_page_cnt[crt_n] * DBTAB_PAGE_SIZE;
	cur_page_endid	= MIN (( db_page_cnt[crt_n]+1)*DBTAB_PAGE_SIZE, db_pid_cnt[crt_n] );
*/
	cur_page_startid = pageDb[crt_n].PageStart ;
	cur_page_endid	=  pageDb[crt_n].PageEnd   ;

	point= db_tab_window_info[crt_n].select_pt;
        point.pt_id	=0;
        GetObjFixedInfo(crt_n,&db_cmm,val_obj,label_obj,&col_num); 
        DrawObjDbHead( drawable,gc,label_obj,col_num,crt_n);
        
       
	for ( i= cur_page_startid; i <= cur_page_endid; i++)           
	{
		db_val_col = 0;
		db_str_col = 0;

		point.pt_id	= i;
		
		if ( ReadEntryById (&point, &db_cmm ) == -1)
			continue; 
    if (i == pageDb[crt_n].SelectEnd)
            DrawDbItemTagNew (  crt_n,  drawable, gc ,DBX0 , DBY0 +( i-cur_page_startid)*DBH0 -15);
	
		gdk_gc_set_foreground ( gc, &pixels[crt_n][GREEN3]);	
		GetObjFixedInfo(crt_n,&db_cmm,val_obj,label_obj,&col_num); 
		sprintf(buf,"%4d",i);			
                item=0;
		windk_draw_text( drawable, font_set[DBTAB_FONT], gc, DBX0 , DBY0 +( i-cur_page_startid)*DBH0,buf, strlen(buf),crt_n );
		item++;
		windk_draw_text( drawable, font_set[DBTAB_FONT], gc, 60 , DBY0 +( i-cur_page_startid)*DBH0,val_obj[item], strlen(val_obj[item]) ,crt_n);					
		item++;
                
		windk_draw_text( drawable, font_set[DBTAB_FONT], gc, 180 , DBY0 +( i-cur_page_startid)*DBH0, val_obj[item], strlen(val_obj[item]) ,crt_n);	
		item++;
                
        windk_draw_text( drawable, font_set[DBTAB_FONT], gc, 300 , DBY0 +( i-cur_page_startid)*DBH0, val_obj[item], strlen(val_obj[item]),crt_n );	
		
		db_init_width = 450;		

		for(item=4;item<col_num;item++)		
			windk_draw_text( drawable, font_set[DBTAB_FONT], gc,db_init_width+DB_VAL_STR_WIDTH*(item-3), DBY0 +( i-cur_page_startid)*DBH0,val_obj[item] , strlen(val_obj[item]),crt_n );
	}	
}
void DrawTimDBHead(GdkDrawable *drawable, GdkGC *gc, int crt_n)
{
	int db_val_col=0,db_str_col=0,db_init_width;
	
	windk_draw_text( drawable, font_set[DBTAB_FONT], gc, DBX0, DBY1, _CS_(OIX_STR_IO_ADDRESS), -1 ,crt_n);
    windk_draw_text( drawable, font_set[DBTAB_FONT], gc, DBX0+50, DBY1, _CS_(OIX_STR_UPDATE_TIME), -1 ,crt_n);
	
    windk_draw_text( drawable, font_set[DBTAB_FONT], gc, DBX0+150, DBY1, _CS_(OIX_CSTR_NAME), -1,crt_n );
	
	windk_draw_text( drawable, font_set[DBTAB_FONT], gc, DBX0+280, DBY1, _CS_(OIX_STR_TIME_NAME), -1,crt_n );		

	windk_draw_text( drawable, font_set[DBTAB_FONT], gc, DBX0+580, DBY1, _CS_(OIX_STR_TEMP_SET_TIME), -1 ,crt_n);
	db_init_width = DBX0+580;
	db_val_col++;
	windk_draw_text( drawable, font_set[DBTAB_FONT], gc, db_init_width+DB_VAL_COL_WIDTH*db_val_col+DB_VAL_STR_WIDTH*db_str_col, DBY1, _CS_("X"), -1,crt_n );
	db_val_col++;
	windk_draw_text( drawable, font_set[DBTAB_FONT], gc, db_init_width+DB_VAL_COL_WIDTH*db_val_col+DB_VAL_STR_WIDTH*db_str_col, DBY1, _CS_("Y"), -1,crt_n );
	db_val_col++;  
	windk_draw_text( drawable, font_set[DBTAB_FONT], gc, db_init_width+DB_VAL_COL_WIDTH*db_val_col+(DB_VAL_STR_WIDTH-5)*db_str_col, DBY1, _CS_("Z"), -1,crt_n );

}
void DspTimDB( int crt_n,GdkDrawable *drawable ,GdkGC *gc )
{
	POINTER		point;
	int		db_val_col,db_str_col,db_init_width;
	
//	int     db_ackn_st;
//	char    flag_lmt;
	int     i;
	int	cur_page_startid,cur_page_endid;
	char  LongNamerStr1[128];
	DMS_COMMON	db_cmm;
	
//	DB_DA_STATUS_NAME	ana_sta;
//	char    block[10];
	
	char	set_time[25],entry_name[ENTRY_NAME_SIZE];
	
	struct tm newTime;
//		long ltime;
	/*char	time_tm[20];*/
	
	gdk_gc_set_foreground ( gc, &pixels[crt_n][L_BLUE1]);
	
	point= db_tab_window_info[crt_n].select_pt;
	DrawTimDBHead( drawable,gc,crt_n);

	cur_page_startid = pageDb[crt_n].PageStart ;
	cur_page_endid	=  pageDb[crt_n].PageEnd   ;
	for ( i= cur_page_startid; i <= cur_page_endid; i++)   
	{
		db_val_col = 0;
		db_str_col = 0;
		
		point.pt_id	= i;
		
		if ( ReadEntryById (&point, &db_cmm ) == -1)
			continue; 
		if (i == pageDb[crt_n].SelectEnd)
            DrawDbItemTagNew (  crt_n,  drawable, gc ,DBX0 , DBY0 +( i-cur_page_startid)*DBH0 -15);
		
		gdk_gc_set_foreground ( gc, &pixels[crt_n][GREEN3]);		
		
		strcpy(entry_name,db_cmm.fixed.entryName);
		
		GetGroupLongnameById( db_cmm.point.stn_id, db_cmm.point.dev_id, LongNameStr );
		GetEntryLongnameById (&db_cmm.point,LongNamerStr1 );
		strcat( LongNameStr, LongNamerStr1 );
		BlankFillCopy (name_item, LongNameStr, POINT_LONGNAME_SIZE);	

		sprintf (str_time, "%02d:%02d:%02d", db_cmm.var.anlg.update_time.hour,
			db_cmm.var.anlg.update_time.minute,db_cmm.var.anlg.update_time.second);
		
		/*strcat (str_time, time_tm);*/
		
		
		sprintf(buf,"%4d",i);			
		
		windk_draw_text( drawable, font_set[DBTAB_FONT], gc, DBX0 , DBY0 +( i-cur_page_startid)*DBH0,buf, strlen(buf) ,crt_n);//序号

		windk_draw_text( drawable, font_set[DBTAB_FONT], gc, DBX0+50 , DBY0 +( i-cur_page_startid)*DBH0, str_time, strlen(str_time) ,crt_n);	//更新时间
		
		windk_draw_text( drawable, font_set[DBTAB_FONT], gc, DBX0+150 , DBY0 +( i-cur_page_startid)*DBH0,entry_name, strlen(entry_name),crt_n ); //逻辑名
		sprintf(buf,"%s",name_item);
		windk_draw_text( drawable, font_set[DBTAB_FONT], gc, DBX0+280 , DBY0 +( i-cur_page_startid)*DBH0,buf, strlen(buf) ,crt_n);	//汉字名				
		
		newTime = *localtime(&db_cmm.var.time.value_sec);
		sprintf(set_time,"%04d-%02d-%02d %02d:%02d:%02d ",newTime.tm_year+1900,
			newTime.tm_mon+1, newTime.tm_mday, newTime.tm_hour,
		newTime.tm_min, newTime.tm_sec);		

		windk_draw_text( drawable, font_set[DBTAB_FONT], gc, DBX0+580 , DBY0 +( i-cur_page_startid)*DBH0, set_time, strlen(set_time) ,crt_n);//人工设置时间
		db_init_width= DBX0+580;
		db_val_col++;         
		sprintf(buf,"%.4f",db_cmm.var.time.coords.x);
		windk_draw_text( drawable, font_set[DBTAB_FONT], gc, db_init_width+DB_VAL_COL_WIDTH*db_val_col+DB_VAL_STR_WIDTH*db_str_col , DBY0 +( i-cur_page_startid)*DBH0, buf, strlen(buf),crt_n );	
		db_val_col++;         
		sprintf(buf,"%.4f",db_cmm.var.time.coords.y);
		windk_draw_text( drawable, font_set[DBTAB_FONT], gc, db_init_width+DB_VAL_COL_WIDTH*db_val_col+DB_VAL_STR_WIDTH*db_str_col , DBY0 +( i-cur_page_startid)*DBH0, buf, strlen(buf),crt_n );	
		db_val_col++;         
		sprintf(buf,"%.4f",db_cmm.var.time.coords.z);
		windk_draw_text( drawable, font_set[DBTAB_FONT], gc, db_init_width+DB_VAL_COL_WIDTH*db_val_col+DB_VAL_STR_WIDTH*db_str_col , DBY0 +( i-cur_page_startid)*DBH0, buf, strlen(buf),crt_n );	
	
	}
}

int  DbDspProc( int crt_n  )
{   

	GtkWidget *scroll ;
	if( crt_n < 0 )
		return FALSE;  
     
	gdk_gc_set_background ( db_draw_gc[crt_n], &pixels[crt_n][BLACK]);
	gdk_gc_set_foreground ( db_draw_gc[crt_n], &pixels[crt_n][BLACK]);
	gdk_draw_rectangle (DbTabOffDrawable[crt_n],db_draw_gc[crt_n],TRUE, 0,0,(int)(DB_CANVAS_WINDOW_WIDTH*xscale[crt_n]),(int)((DB_CANVAS_WINDOW_HEIGHT+1500)*yscale[crt_n])); 
	
       if(db_io_set_box[crt_n])
           /*gtk_window_set_keep_above(GTK_WINDOW(db_io_set_box[crt_n]),TRUE);*/
            gtk_window_present(GTK_WINDOW(db_io_set_box[crt_n]));
        if(db_tab_search_dlg[crt_n])
            gtk_window_present(GTK_WINDOW(db_tab_search_dlg[crt_n]));
       /* if(db_detail_info[crt_n])
            gtk_window_present(GTK_WINDOW(db_detail_info[crt_n]));  */      
    DrawDbItemTag(crt_n, DbTabOffDrawable[crt_n], db_draw_gc[crt_n]);
	
    DbDspProc1 ( DbTabOffDrawable[crt_n] ,crt_n);

	gdk_window_copy_area (db_canvas[crt_n]->window,db_draw_gc[crt_n],0,0,
	DbTabOffDrawable[crt_n],0,0,(int)(DB_CANVAS_WINDOW_WIDTH*xscale[crt_n]),(int)((DB_CANVAS_WINDOW_HEIGHT+1500)*yscale[crt_n]));


	scroll = g_object_get_data(G_OBJECT(db_pop_shell[crt_n]),"scrollbar" );
	gtk_widget_queue_draw(scroll);
	return TRUE;

}

void GetObjFixedInfo(int crt_n,DMS_COMMON *db_cmm,char value_str[50][POINT_LONGNAME_SIZE],char label_str[20][POINT_LONGNAME_SIZE],int *col_num)
{	
	int		item=0;
	char  YesNoStr[64] ;
        strcpy(label_str[item],OIX_STR_IO_ADDRESS);
        sprintf(value_str[item],"%d",db_cmm->point.pt_id); 
	item++;
        
        strcpy(label_str[item],OIX_STR_UPDATE_TIME);
	sprintf (value_str[item], "%02d:%02d:%02d:%03d", db_cmm->var.devobj.update_time.hour,
			db_cmm->var.devobj.update_time.minute,db_cmm->var.devobj.update_time.second,db_cmm->var.devobj.update_time.ms);
	item++;	
        
        strcpy(label_str[item],OIX_STR_OBJ_NAME);
        sprintf(value_str[item],db_cmm->fixed.entryName);	
	item++;	
        
        strcpy(label_str[item],OIX_CSTR_LONGNAME);
	GetEntryLongnameById(&db_cmm->point,value_str[item]);        
	item++;	       
        
        strcpy(label_str[item],OIX_STR_CONDC0);        
        if(db_cmm->var.devobj.fb_status1.condc0==0)
            strcpy(value_str[item],_CS_(OIX_CSTR_UNSATISFIED));
        else
            strcpy(value_str[item],_CS_(OIX_CSTR_SATISFIED));
	item++;   
        
        strcpy(label_str[item],OIX_STR_CONDC1);        
        if(db_cmm->var.devobj.fb_status1.condc1==0)
            strcpy(value_str[item],_CS_(OIX_CSTR_UNSATISFIED));
        else
            strcpy(value_str[item],_CS_(OIX_CSTR_SATISFIED));
	item++;
        
        strcpy(label_str[item],OIX_STR_P0);
	sprintf(value_str[item],"%d",db_cmm->var.devobj.fb_status1.p0);
	item++;	
        
        strcpy(label_str[item],OIX_STR_P1);
	sprintf(value_str[item],"%d",db_cmm->var.devobj.fb_status1.p1);
	item++;
        
        strcpy(label_str[item],OIX_STR_SWGF_N);       
        if(db_cmm->var.devobj.fb_status1.swgf_n==0)
            strcpy(value_str[item],_CS_(DPS_STR_FAIL ) );
        else
            strcpy(value_str[item],  _CS_(DPS_STR_NORMAL));
	item++;
        
        strcpy(label_str[item],_CS_(OIX_STR_POSD0I));        
        GetYesNoStr(db_cmm->var.devobj.fb_status2.posd0,YesNoStr);
		strcpy(value_str[item],_CS_(YesNoStr));
		strcpy(YesNoStr,"");
	item++;
        
        strcpy(label_str[item],_CS_(OIX_STR_POSD1I));
        GetYesNoStr(db_cmm->var.devobj.fb_status2.posd1,YesNoStr);
		strcpy(value_str[item],_CS_(YesNoStr));
			strcpy(YesNoStr,"");
	item++;
        
        strcpy(label_str[item],_CS_(OIX_STR_SCAN));
	strcpy( value_str[item],_CS_(alm_str[db_cmm->fixed.iost.scanEnable]));
	item++;	
        
        strcpy(label_str[item],_CS_(OIX_STR_ALM));	
        strcpy( value_str[item],_CS_(alm_str[db_cmm->fixed.iost.almEnable]));
	item++;	     
        
        *col_num = item;
}


void	DbDspProc1 ( GdkDrawable *drawable ,int crt_n )
{
	if (! db_pop_shell[crt_n] )
	{
		gtk_timeout_remove(db_timeout_id[crt_n]);	
		db_timeout_id[crt_n]= 0;
		return;
	}	


	if(-1==GetEntryNumById( db_tab_window_info[crt_n].select_pt.stn_id, 
		db_tab_window_info[crt_n].select_pt.dev_id, db_tab_window_info[crt_n].select_pt.data_type,&db_pid_cnt[crt_n] ))
		return;
    
//	db_page_numb[crt_n]	= (db_pid_cnt[crt_n]+DBTAB_PAGE_SIZE-1)/DBTAB_PAGE_SIZE;
       /* printf("in DbDspProc1: db_page_numb=%d,db_page_cnt=%d\n",db_page_numb[crt_n],db_page_cnt[crt_n]);*/
/*	if ( db_page_cnt[crt_n] > db_page_numb[crt_n]-1)
		db_page_cnt[crt_n]	= db_page_numb[crt_n]-1;
    if(db_page_cnt[crt_n]<0)
         db_page_cnt[crt_n]=0;
		 */
   // pageDb[crt_n].CurPage  =  db_page_cnt[crt_n] ; 
//	InitPageInfo(&pageDb[crt_n] , crt_n,db_pid_cnt[crt_n]) ;
		SetPageInfo( &pageDb[crt_n] );  
        pageDb[crt_n].CurPage = (pageDb[crt_n].PageStart+ 2 )/ pageDb[crt_n].PageSize    ;  
        
        
 		SetPageInfo( &pageDb[crt_n] );
        pageDb[crt_n].PageEnd   =   MIN ((pageDb[crt_n].PageStart + pageDb[crt_n].PageSize -1) , (pageDb[crt_n].RecCount-1) )   ;
        SetAdjInfo( &pageDb[crt_n],adj2[crt_n] );  




	switch	( db_tab_window_info[crt_n].select_pt.data_type )
	{
	case	ANA_TYPE:
	case	PARAM_TYPE:
	case	OUTA_TYPE: 	
	case	CALC_TYPE:
		DspAnaDB ( crt_n ,drawable,db_draw_gc[crt_n]);		
		break;

	case    POL_TYPE:
	case    SOE_TYPE:
	case    SYSMSG_TYPE:
	case    OUTS_TYPE:
		DspPolDB ( crt_n,drawable,db_draw_gc[crt_n] );
		break;

	case    IMP_TYPE:
	case	CHARA_TYPE:
		DspImpDB ( crt_n,drawable,db_draw_gc[crt_n] );
		break;

	case	CURVE_TYPE:
		DspCurveDB(crt_n,drawable,db_draw_gc[crt_n]);
		break;
                
        case	DEVOBJ_TYPE:            
		DspObjDB(crt_n,drawable,db_draw_gc[crt_n]);
		break;

		case TIME_TYPE:
			DspTimDB(crt_n,drawable,db_draw_gc[crt_n]);
			
	default:
		break;
	}
	
	if ( db_blink_st[crt_n] == 0)
	{	db_blink_st[crt_n]	= 1;	}
	else
	{	db_blink_st[crt_n]	= 0;	}
}

void DrawPolDbHead	(GdkDrawable *drawable,GdkGC *gc, int data_type ,int crt_n)
{
	int db_str_col=0,db_init_width,item;

	windk_draw_text( drawable,font_set[DBTAB_FONT], gc, DBX0, DBY1, _CS_(OIX_STR_IO_ADDRESS), -1 ,crt_n);
        windk_draw_text( drawable, font_set[DBTAB_FONT], gc, DBX0+50, DBY1, _CS_(OIX_STR_UPDATE_TIME), -1,crt_n );

        windk_draw_text( drawable, font_set[DBTAB_FONT], gc, DBX0+250, DBY1, _CS_(OIX_CSTR_NAME), -1,crt_n );
	switch	( data_type )
	{
	case	POL_TYPE:
		windk_draw_text( drawable, font_set[DBTAB_FONT], gc, DBX0+400, DBY1,_CS_(OIX_STR_SCAN_NAME), -1,crt_n );		
		break;

	case	SOE_TYPE:
		windk_draw_text( drawable, font_set[DBTAB_FONT], gc, DBX0+400, DBY1,_CS_(OIX_STR_INTERUPTIVE_NAME), -1,crt_n);		
		break;

	case	SYSMSG_TYPE:
		windk_draw_text( drawable, font_set[DBTAB_FONT], gc, DBX0+400,DBY1, _CS_(OIX_STR_SYSMSG_NAME), -1 ,crt_n);		
		break;
	case	OUTS_TYPE:
		windk_draw_text( drawable, font_set[DBTAB_FONT], gc, DBX0+400, DBY1,_CS_(OIX_STR_OUTS_NAME), -1 ,crt_n);
		break;
	default:
		break;
	}  
	
/*	windk_draw_text( drawable, font_set[DBTAB_FONT], gc, DBX0+500, DBY1, OIX_STR_UPDATE_TIME, strlen(OIX_STR_UPDATE_TIME) );
*/
	db_init_width = 700;
	
	for(item=3;item<POL_col;item++)
		windk_draw_text( drawable, font_set[DBTAB_FONT], gc, db_init_width+(DB_VAL_COL_WIDTH-5)*(item-3), DBY1, _CS_(POL_titles[item]), -1,crt_n);	
}

void DspPolDB (int crt_n,GdkDrawable *drawable,GdkGC *gc)
{
	POINTER	point;
	int     flag_type, db_ackn_st,item;
	int     flag_st, fail_id_flag, breaker_fail_sta;
	int     i,db_init_width;
	int	cur_page_startid,cur_page_endid;
        char    man_set[128];
        
	DMS_COMMON	db_cmm;

	DB_DA_STATUS_NAME    pol_sta;
	char	status_str[100],value_str[POL_col][POINT_NAME_SIZE],entry_name[ENTRY_NAME_SIZE];        
        
	char	time_tm[20];
	char  LongNamerStr1[128];
 	gdk_gc_set_foreground ( gc, &pixels[crt_n][L_BLUE1]);

	point= db_tab_window_info[crt_n].select_pt;
	DrawPolDbHead	(drawable, gc, point.data_type ,crt_n);
/*
	cur_page_startid= db_page_cnt[crt_n] * DBTAB_PAGE_SIZE;
	cur_page_endid	= MIN (( db_page_cnt[crt_n]+1)*DBTAB_PAGE_SIZE, db_pid_cnt[crt_n] );
*/

	cur_page_startid = pageDb[crt_n].PageStart ;
	cur_page_endid	=  pageDb[crt_n].PageEnd   ;

	for ( i= cur_page_startid; i <= cur_page_endid; i++)
	{
		point.pt_id	= i;
		
		if ( ReadEntryById (&point, &db_cmm ) == -1)
			continue;

    if (i == pageDb[crt_n].SelectEnd)
            DrawDbItemTagNew (  crt_n,  drawable, gc ,DBX0 , DBY0 +( i-cur_page_startid)*DBH0 -15);



		flag_type = db_cmm.fixed.ioattr.procType;
                if(db_cmm.var.ind.status.state ==db_cmm.fixed.ioattr.normalState)
                    flag_st = 0;
                else
                    flag_st =1;

		GetIostBitsStr  (db_cmm.fixed.iost, &pol_sta );
                if(db_cmm.var.ind.status.chan_fail==0)                    
                    strcpy(pol_sta.name_qua,_CS_(DPS_STR_NORMAL));
                else
                    strcpy(pol_sta.name_qua,_CS_(DPS_STR_FAIL));
                
		GetYesNoStr(db_cmm.var.ind.status.man_set, man_set);
                
		GetGroupLongnameById( db_cmm.point.stn_id, db_cmm.point.dev_id, LongNameStr );

		GetEntryLongnameById (&db_cmm.point,LongNamerStr1 );
	

		strcat( LongNameStr,LongNamerStr1);

		BlankFillCopy (name_item, LongNameStr, POINT_LONGNAME_SIZE);
		
                strcpy(entry_name,db_cmm.fixed.entryName);
		/*GetIndPtStateStrById( &db_cmm.point, status_str );*/
                
                GetIndPtRtStateStrById(&db_cmm.point, status_str );
		sprintf(status_str, "%s(%d)", status_str, db_cmm.var.ind.status.rt_state);

		fail_id_flag   = db_cmm.fixed.ioattr.fail;
		breaker_fail_sta = db_cmm.var.ind.status.fault_tag;
		db_ackn_st	= db_cmm.fixed.iost.almNoAck;
		gdk_gc_set_foreground ( gc, &pixels[crt_n][GREEN3]);
		switch  ( flag_type )
		{
		case    DEV_TYPE_RELAY: 
       		case    DEV_TYPE_2BREAKER:
		case    DEV_TYPE_3BREAKER:
		case    DEV_TYPE_2DEVICE:
		case    DEV_TYPE_3DEVICE:
        	case    DEV_TYPE_2VALVE:
		case    DEV_TYPE_3VALVE:
        	case    DEV_TYPE_LCUMSG:
		case    DEV_TYPE_STREAM:
        	case    DEV_TYPE_NETMSG:
        	case    DEV_TYPE_LOGMSG:
			if ( ( ( flag_st == 1 ) && (fail_id_flag == 1 ) ) || (breaker_fail_sta))
                        	SetBlinkForeground ( crt_n, gc, db_ackn_st, &red_cp );
                	/*else
				gdk_gc_set_foreground ( gc, &pixels[crt_n][GREEN3]); */                       	
        	 	break; 
 
		case    DEV_TYPE_HOSTMSG:
                	if ( ( (flag_st == 2) && (fail_id_flag == 1 ) ) || (breaker_fail_sta))
                        	SetBlinkForeground ( crt_n, gc, db_ackn_st, &red_cp );                	
                        	
                break;

		default:
			/*gdk_gc_set_foreground ( gc, &pixels[crt_n][green_cp.clr[0]]);	*/				
			break;
		}
			
		sprintf (str_time,"%d-%d-%d ", db_cmm.var.curve.update_time.year,db_cmm.var.curve.update_time.month,db_cmm.var.curve.update_time.day);
			
		sprintf (time_tm, "%02d:%02d:%02d:%03d", db_cmm.var.ind.update_time.hour,
				db_cmm.var.ind.update_time.minute,db_cmm.var.ind.update_time.second,
				db_cmm.var.ind.update_time.ms);
			
		strcat (str_time, time_tm);	
			
		sprintf(buf,"%d",i);
		windk_draw_text( drawable, font_set[DBTAB_FONT], gc, DBX0 , DBY0 +( i-cur_page_startid)*DBH0,buf, strlen(buf) ,crt_n);
		windk_draw_text( drawable, font_set[DBTAB_FONT], gc, DBX0+50 , DBY0 +( i-cur_page_startid)*DBH0, str_time, strlen(str_time),crt_n );	
		
                windk_draw_text( drawable, font_set[DBTAB_FONT], gc, DBX0+250 , DBY0 +( i-cur_page_startid)*DBH0,entry_name, strlen(entry_name),crt_n );
               
                sprintf(buf,"%s",name_item);
		windk_draw_text( drawable, font_set[DBTAB_FONT], gc, 400 , DBY0 +( i-cur_page_startid)*DBH0,buf, strlen(buf),crt_n );					
			
	/*	windk_draw_text( drawable, font_set[DBTAB_FONT], gc, 500 , DBY0 +( i-cur_page_startid)*DBH0, str_time, strlen(str_time) );	
	*/	
		item =3;
		strcpy(value_str[item],status_str);item++;
		strcpy(value_str[item],pol_sta.name_qua);item++;
                strcpy(value_str[item],_CS_(man_set));item++;
		strcpy(value_str[item],pol_sta.name_scan);item++;
		strcpy(value_str[item],pol_sta.name_alm);item++;
		strcpy(value_str[item],pol_sta.name_almack);item++;		

		db_init_width= 700;
	
		for(item=3;item<POL_col;item++)
			windk_draw_text( drawable, font_set[DBTAB_FONT], gc, db_init_width+(DB_VAL_COL_WIDTH-5)*(item-3), DBY0 +( i-cur_page_startid)*DBH0, value_str[item], strlen(value_str[item]),crt_n);			
	
	}				     
}

void	DspImpDB ( int crt_n ,GdkDrawable*drawable, GdkGC *gc)
{
	POINTER		point;
	char LongNamerStr1[128];
	int     i,j,col,db_val_col,db_str_col,db_init_width;
	int	cur_page_startid,cur_page_endid;
	char	ana_tr[20][100],imp_col[20][100];
	DMS_COMMON	db_cmm;
        char    str_time[50],entry_name[ENTRY_NAME_SIZE];
       
	gdk_gc_set_foreground ( gc, &pixels[crt_n][L_BLUE1]);

	point= db_tab_window_info[crt_n].select_pt;
	switch  ( point.data_type )
	{
	case    IMP_TYPE:
		DrawImpDbHead( drawable,gc ,crt_n);
		break;

	case	CHARA_TYPE:
		DrawTempDbHead( drawable,gc,crt_n );
		break;

	default:
		break;
	}	


/*
	cur_page_startid= db_page_cnt[crt_n] * DBTAB_PAGE_SIZE;
	cur_page_endid	= MIN (( db_page_cnt[crt_n]+1)*DBTAB_PAGE_SIZE, db_pid_cnt[crt_n] );

*/
	cur_page_startid = pageDb[crt_n].PageStart ;
	cur_page_endid	=  pageDb[crt_n].PageEnd   ;
	for ( i= cur_page_startid; i <= cur_page_endid; i++)
	{
		db_val_col=0;
		db_str_col=0;

		point.pt_id	= i;
		
		if ( ReadEntryById (&point, &db_cmm ) == -1)		
			continue;

    if (i == pageDb[crt_n].SelectEnd)
            DrawDbItemTagNew (  crt_n,  drawable, gc ,DBX0 , DBY0 +( i-cur_page_startid)*DBH0 -15);

	   gdk_gc_set_foreground ( gc, &pixels[crt_n][GREEN3]);

		switch  ( point.data_type )
		{
		case    IMP_TYPE:
			/*GetGroupLongnameById( db_cmm.point.stn_id, db_cmm.point.dev_id, LongNameStr );   */		
	       	GetEntryLongnameById (&db_cmm.point,LongNamerStr1 );	
			strcpy( LongNameStr, LongNamerStr1 );
			BlankFillCopy (name_item, LongNameStr, POINT_LONGNAME_SIZE);

			sprintf(buf,"%d",i);
			/*点号*/
			windk_draw_text( drawable, font_set[DBTAB_FONT], gc, DBX0 , DBY0 +( i-cur_page_startid)*DBH0,buf, strlen(buf) ,crt_n);
			                        	
            sprintf (str_time, "%02d:%02d:%02d", db_cmm.var.anlg.update_time.hour,
						db_cmm.var.anlg.update_time.minute,db_cmm.var.anlg.update_time.second);
			/*更新时间*/
			windk_draw_text( drawable, font_set[DBTAB_FONT], gc, DBX0+50 , DBY0 +( i-cur_page_startid)*DBH0,str_time, strlen(str_time) ,crt_n);	
			
            strcpy(entry_name,db_cmm.fixed.entryName);
            windk_draw_text( drawable, font_set[DBTAB_FONT], gc, DBX0+130 , DBY0 +( i-cur_page_startid)*DBH0,entry_name, strlen(entry_name),crt_n );	
			
            sprintf(buf,"%s",name_item);
            windk_draw_text( drawable, font_set[DBTAB_FONT], gc, DBX0+270 , DBY0 +( i-cur_page_startid)*DBH0,buf, strlen(buf) ,crt_n);	
			/*通道质量*/
            if(db_cmm.var.imp.entry.status.chan_fail)
                strcpy(buf,_CS_(DPS_STR_FAIL));
            else if(db_cmm.var.imp.entry.status.man_set)
                    strcpy(buf,_CS_(DPS_STR_MANUAL));
            else
                    strcpy(buf,_CS_(DPS_STR_NORMAL));

            windk_draw_text( drawable, font_set[DBTAB_FONT], gc, DBX0+500 , DBY0 +( i-cur_page_startid)*DBH0,buf, strlen(buf) ,crt_n);	
			
            db_init_width = 570;
                        
                        
			j=0;
			/*脉冲数*/
			sprintf(imp_col[j],"%d",db_cmm.var.imp.entry.rt_value);	j++;
			/*电度表读数 */
			sprintf(imp_col[j],"%f",db_cmm.var.imp.entry.d_accum);  j++;
			/*日剑锋值*/
			sprintf(imp_col[j],"%.3f",db_cmm.var.imp.ppeak.day);    j++;
			/*日高峰值*/
			sprintf(imp_col[j],"%.1f",db_cmm.var.imp.peak.day);     j++;
			/*日平均值*/
            sprintf(imp_col[j],"%.1f",db_cmm.var.imp.normal.day);	j++;
			/*日低谷值*/
			sprintf(imp_col[j],"%.1f",db_cmm.var.imp.valley.day);	j++;
			/*日总值*/
			sprintf(imp_col[j],"%.3f",db_cmm.var.imp.sum.day);		j++;
			
			col = j;
			for(j=0;j<col;j++)
				windk_draw_text( drawable, font_set[DBTAB_FONT], gc, db_init_width+(DB_VAL_STR_WIDTH+20)*j , DBY0 +( i-cur_page_startid)*DBH0, imp_col[j], strlen(imp_col[j]),crt_n );

			break;		

		case    CHARA_TYPE:
			GetGroupLongnameById( db_cmm.point.stn_id, db_cmm.point.dev_id,LongNameStr );
			GetEntryLongnameById (&db_cmm.point,LongNamerStr1 );

			strcat( LongNameStr, LongNamerStr1);
			BlankFillCopy (name_item, LongNameStr, POINT_LONGNAME_SIZE);			
			
			sprintf ( buf, "%d", i);
			windk_draw_text( drawable, font_set[DBTAB_FONT], gc, DBX0, DBY0 +( i-cur_page_startid)*DBH0,
					buf, strlen(buf ),crt_n);
			sprintf(buf,"%s",name_item);
			windk_draw_text( drawable, font_set[DBTAB_FONT], gc, DBX0+50, DBY0 +( i-cur_page_startid)*DBH0,
					buf, strlen(buf ),crt_n);
			j=0;
			sprintf(ana_tr[j],"%.2f",db_cmm.var.chara.chara_data.max_no/100.);			j++;//wzg
			sprintf( ana_tr[j],"%.3f",db_cmm.var.chara.chara_data.max_value);		j++;
			sprintf(ana_tr[j],"%.2f",db_cmm.var.chara.chara_data.min_no/100.);			j++;				
			sprintf(ana_tr[j],"%.3f",db_cmm.var.chara.chara_data.min_value);		j++;
			sprintf(ana_tr[j],"%.3f",db_cmm.var.chara.chara_data.avg_value);		j++;
			sprintf(ana_tr[j],"%.3f",db_cmm.var.chara.chara_data.std_value);		j++;
			sprintf(ana_tr[j],"%.2f",db_cmm.var.chara.chara_data.max_rate_no/100.);		j++;
			sprintf(ana_tr[j],"%.3f",db_cmm.var.chara.chara_data.max_rate);			j++;
			sprintf(ana_tr[j],"%.2f",db_cmm.var.chara.chara_data.min_rate_no/100.);		j++;
			sprintf(ana_tr[j],"%.3f",db_cmm.var.chara.chara_data.min_rate);			j++;
			col = j;
			for(j=0;j<col;j++)
				windk_draw_text( drawable, font_set[DBTAB_FONT], gc, DBX0 + 380+j*DB_VAL_STR_WIDTH, DBY0 +( i-cur_page_startid)*DBH0,
					ana_tr[j], strlen(ana_tr[j] ),crt_n);
			
			break;

		default:
       			 break;
		}
	}
}


void DestroyDbSearchDlg(GtkWidget *w, gpointer data)
{	
	int crt_n= (int)data;
        
	gtk_widget_destroy(db_tab_search_dlg[crt_n]);
    db_tab_search_dlg[crt_n]=NULL;
}



gint  SearchDbPoint(GtkWidget *w, gpointer data)
{	
	POINTER		point;
	char		entry_name[128];
	char		err_str[128];
	gboolean	entryname_radiobutton_state;
	int			crt_n= (int)data;
    GtkWidget *name_radio_btn,*name_entry,*id_entry;
	
    name_radio_btn = g_object_get_data(G_OBJECT(db_tab_search_dlg[crt_n]),"SearchEntryName_radio");
	entryname_radiobutton_state = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(name_radio_btn));        

	point= db_tab_window_info[crt_n].select_pt;
	if(entryname_radiobutton_state==TRUE)
	{
            name_entry = g_object_get_data(G_OBJECT(db_tab_search_dlg[crt_n]),"SearchEntryName_entry");
                if(strcmp(gtk_entry_get_text(GTK_ENTRY(name_entry)),"")!=0)
                {
                    
                    strcpy(entry_name, gtk_entry_get_text(GTK_ENTRY(name_entry)));
		
                    if ( GetEntryIdByName( entry_name, &point ) == -1 )
                    {
						sprintf(err_str, "%s: %s",_CS_(OIX_STR_NOT_FIND), entry_name);
						PopupMessageBox(db_pop_shell[crt_n], err_str);
						return TRUE;
                    }
                }                
	}
	else
	{
            id_entry = g_object_get_data(G_OBJECT(db_tab_search_dlg[crt_n]),"SearchPtId_entry");
            if(strcmp(gtk_entry_get_text(GTK_ENTRY(id_entry)),"")!=0)
            {                    
                    strcpy(entry_name, gtk_entry_get_text(GTK_ENTRY(id_entry)));
                   
                    point.pt_id = atoi(entry_name);
                    if(!IsPointExist(&point))
                    {
			sprintf(err_str, "%s: %s",OIX_STR_NOT_FIND, entry_name);
			PopupMessageBox(db_pop_shell[crt_n], err_str);
			return TRUE;
                    }
            }
	}
		
	db_tab_window_info[crt_n].select_pt= point;
	db_tag[crt_n] = ITEM_TAG_ON;	
//	db_page_cnt[crt_n]= point.pt_id/DBTAB_PAGE_SIZE;
//	pageDb[crt_n].CurPage = point.pt_id/pageDb[crt_n].PageSize ;
    pageDb[crt_n].PageStart = pageDb[crt_n].SelectEnd = point.pt_id ; 
	 
	DbDspProc(crt_n);
	return TRUE;
}


int SetEntryNameRadioState(GtkWidget *text_w, GdkEventFocus *e ,gpointer data)
{
        GtkWidget *name_radio,*name_entry;
	int		crt_n= (int)data;
        
        name_entry = g_object_get_data(G_OBJECT(db_tab_search_dlg[crt_n]),"SearchEntryName_entry");
        name_radio = g_object_get_data(G_OBJECT(db_tab_search_dlg[crt_n]),"SearchEntryName_radio");
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(name_radio),TRUE);
        gtk_widget_grab_focus(name_entry);
return 0 ; 
}

int SetPtIdRadioState(GtkWidget *text_w, GdkEventFocus *e ,gpointer data)
{
	GtkWidget *id_radio,*id_entry;
	int		crt_n= (int)data;
   
        id_entry = g_object_get_data(G_OBJECT(db_tab_search_dlg[crt_n]),"SearchPtId_entry");
        id_radio = g_object_get_data(G_OBJECT(db_tab_search_dlg[crt_n]),"SearchPtId_radio");
 	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(id_radio),TRUE);
        gtk_widget_grab_focus(id_entry);
	 return 0 ; 
}

void PopupSearchDlg(GtkWidget *w, int crt_n )
{
	GtkWidget /**dialog,*/*button_ack, *button_exit;	
	GtkWidget *SearchEntryName_entry,*SearchPtId_entry,*SearchEntryName_radio,*SearchPtId_radio;
	gchar *pConvert=0;
        
        if(db_tab_search_dlg[crt_n])
            {
            gtk_window_present(GTK_WINDOW(db_tab_search_dlg[crt_n]));
            return;
        }
        
	db_tab_search_dlg[crt_n] = gtk_dialog_new();
	 
	gtk_signal_connect(GTK_OBJECT(db_tab_search_dlg[crt_n]),
        "destroy",  GTK_SIGNAL_FUNC(DestroyDbSearchDlg),  (gpointer)  crt_n );

	gtk_window_set_policy(GTK_WINDOW(db_tab_search_dlg[crt_n]),FALSE,FALSE,FALSE);
	gtk_window_set_title(GTK_WINDOW(db_tab_search_dlg[crt_n]), pConvert=_toUtf8EX(OIX_STR_SEARCH));
	G_SAFE_FREE(pConvert);
	gtk_window_set_transient_for(GTK_WINDOW(db_tab_search_dlg[crt_n]), GTK_WINDOW(db_pop_shell[crt_n]));
	gtk_window_set_destroy_with_parent(GTK_WINDOW(db_tab_search_dlg[crt_n]),TRUE);	
	
	SearchPtId_radio =gtk_radio_button_new_with_label(NULL, pConvert=_toUtf8EX(OIX_CSTR_SEARCH_PTID));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(db_tab_search_dlg[crt_n])->vbox),SearchPtId_radio,TRUE,TRUE,0);
	/*db_tab_window_info[crt_n].SearchPtId_radio= SearchPtId_radio;*/
	g_object_set_data(G_OBJECT(db_tab_search_dlg[crt_n]),"SearchPtId_radio",SearchPtId_radio);
                
	SearchPtId_entry =  gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(db_tab_search_dlg[crt_n])->vbox),SearchPtId_entry,TRUE,TRUE,0); 
	gtk_window_set_focus(GTK_WINDOW(db_tab_search_dlg[crt_n]), SearchPtId_entry);
 	gtk_window_activate_focus(GTK_WINDOW(db_tab_search_dlg[crt_n]));
          gtk_signal_connect(GTK_OBJECT(SearchPtId_entry), "activate",
		       GTK_SIGNAL_FUNC(SearchDbPoint),(gpointer)crt_n);   
  	gtk_signal_connect(GTK_OBJECT(SearchPtId_entry),"focus_in_event",
		       GTK_SIGNAL_FUNC(SetPtIdRadioState),(gpointer)crt_n);     
        g_object_set_data(G_OBJECT(db_tab_search_dlg[crt_n]),"SearchPtId_entry",SearchPtId_entry);
	/*db_tab_window_info[crt_n].SearchPtId_entry= SearchPtId_entry;*/
 	       
	SearchEntryName_radio = gtk_radio_button_new_with_label(gtk_radio_button_group(GTK_RADIO_BUTTON(SearchPtId_radio)),
		pConvert=_toUtf8EX(OIX_CSTR_SEARCH_PROMPT));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(db_tab_search_dlg[crt_n])->vbox),SearchEntryName_radio,TRUE,TRUE,0);
	/*db_tab_window_info[crt_n].SearchEntryName_radio= SearchEntryName_radio;*/
        g_object_set_data(G_OBJECT(db_tab_search_dlg[crt_n]),"SearchEntryName_radio",SearchEntryName_radio);
	
	SearchEntryName_entry = gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(db_tab_search_dlg[crt_n])->vbox),SearchEntryName_entry,TRUE,TRUE,0);
        gtk_signal_connect(GTK_OBJECT(SearchEntryName_entry), "activate",
		       GTK_SIGNAL_FUNC(SearchDbPoint),(gpointer)crt_n);
	gtk_signal_connect(GTK_OBJECT(SearchEntryName_entry),"focus_in_event",
		       GTK_SIGNAL_FUNC(SetEntryNameRadioState),(gpointer)crt_n);
	/*db_tab_window_info[crt_n].SearchEntryName_entry= SearchEntryName_entry;*/
        g_object_set_data(G_OBJECT(db_tab_search_dlg[crt_n]),"SearchEntryName_entry",SearchEntryName_entry);
	
	button_ack = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_OK));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(db_tab_search_dlg[crt_n])->action_area),button_ack,FALSE,FALSE,0);
	gtk_signal_connect(GTK_OBJECT(button_ack), "clicked", GTK_SIGNAL_FUNC(SearchDbPoint), (gpointer)crt_n);

	button_exit = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_EXIT));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(db_tab_search_dlg[crt_n])->action_area),button_exit,FALSE,FALSE,0);
	gtk_signal_connect(GTK_OBJECT (button_exit), "clicked",
			       GTK_SIGNAL_FUNC(DestroyDbSearchDlg), (gpointer)crt_n);

	gtk_window_set_position(GTK_WINDOW(db_tab_search_dlg[crt_n]),GTK_WIN_POS_CENTER);
	gtk_widget_show_all(db_tab_search_dlg[crt_n]);
}



void Data_Polling( gpointer w )
{    
    int crt_n;

	GetCrtNumberOfWgt ( GTK_WIDGET(w), &crt_n );
	if( crt_n < 0 )
		return; 	     
	CmdBufInit ( crt_n );
	cmd_buf[crt_n].stn_id	= db_tab_window_info[crt_n].select_pt.stn_id;
	cmd_buf[crt_n].dev_id	= db_tab_window_info[crt_n].select_pt.dev_id;
	cmd_buf[crt_n].point_id	= db_tab_window_info[crt_n].select_pt.pt_id;  
	cmd_buf[crt_n].data_type= db_tab_window_info[crt_n].select_pt.data_type; 
	cmd_buf[crt_n].type_id	= C_DATA_POLLING;	
	cmd_buf[crt_n].data.iValue	=0;
	cmd_buf[crt_n].ctrl_addr	=0;

	cmd_buf[crt_n].status.opr = DATA_POLLING;
	
	ProgDoAckCelBrdcst ( crt_n );
    return;
}

void dbtab_draw_area_realize(GtkWidget *widget,gpointer data)
{
	int crt_n=(gint)data;

    db_draw_gc[crt_n] =  gdk_gc_new(widget->window);
    db_canvas[crt_n]  =  widget;

	DbTabOffDrawable[crt_n] = gdk_pixmap_new(widget->window,(int)(DB_CANVAS_WINDOW_WIDTH*xscale[crt_n]),(int)((DB_CANVAS_WINDOW_HEIGHT+1500)*yscale[crt_n]),-1);

	gdk_window_set_background(widget->window,&pixels[crt_n][BLACK]);
}

void  dbtab_expose_events(	GtkWidget *widget,GdkEventExpose *event, gpointer data)
{
	int	crt_n;

    GetCrtNumberOfWgt (widget, &crt_n );
	if( crt_n < 0 || crt_n >= MAX_CRT_NUMBER )
		return ;

	DbDspProc( crt_n );
}



void db_tab_quit(GtkWidget *w, int crt_n)
{	
	if(db_timeout_id[crt_n]!=0)
	{
		gtk_timeout_remove(db_timeout_id[crt_n]);	
		db_timeout_id[crt_n]= 0;
	}
	g_object_unref (DbTabOffDrawable[crt_n]); //added by zyp on 2008.12.25 
	gtk_widget_destroy(db_pop_shell[crt_n]); 

	db_pop_shell[crt_n] =NULL;

	db_tag[crt_n]	= ITEM_TAG_OFF;
	db_select_type[crt_n] = SEL_NO_DB_PNT;		

}	




void UpdateDbStnSelect(GtkWidget* widget, gpointer data)
{
	int		crt_n;
    int realdata;
	//	GetCrtNumberOfWgt(widget, &crt_n);
	crt_n=getCrtNumFromPointer((int)data);
	realdata=getDataFromPointer((int)data);
	if( crt_n < 0 || crt_n >= MAX_CRT_NUMBER )
		return;

	UnmanageDbIoandInfoBox(crt_n);

	db_tab_window_info[crt_n].select_pt.stn_id= (UCHAR)(int)realdata;
	UpdateDbDevMenu(crt_n);
	UpdateDbDataTypeMenu(crt_n);
	InitPageDbInfo(&pageDb[crt_n] ,  crt_n,0) ;
	DbDspProc(crt_n);
}


void UpdateDbDevSelect(GtkWidget* widget, gpointer data)
{
	int		crt_n;
    int realdata;
	//GetCrtNumberOfWgt(widget, &crt_n);
	crt_n=getCrtNumFromPointer((int)data);
	realdata=getDataFromPointer((int)data);
	if( crt_n < 0 || crt_n >= MAX_CRT_NUMBER )
		return;

	UnmanageDbIoandInfoBox(crt_n);

	db_tab_window_info[crt_n].select_pt.dev_id= (UCHAR)(int)realdata;
	UpdateDbDataTypeMenu(crt_n);
	InitPageDbInfo(&pageDb[crt_n] ,  crt_n,0) ;
	DbDspProc(crt_n);

}



void UpdateDbDataTypeSelect(GtkWidget* widget, gpointer data)
{
	int		crt_n;
	int realdata;
	//GetCrtNumberOfWgt(widget, &crt_n);
	crt_n=getCrtNumFromPointer((int)data);
	realdata=getDataFromPointer((int)data);
	if( crt_n < 0 || crt_n >= MAX_CRT_NUMBER )
		return;

	UnmanageDbIoandInfoBox(crt_n);

	db_tab_window_info[crt_n].select_pt.data_type= (UCHAR)(int)realdata;
	InitPageDbInfo(&pageDb[crt_n] ,  crt_n,0) ;
	DbDspProc(crt_n);
}




void UpdateDbDevMenu(int crt_n) 
{
	char	dev_comment[GROUP_COMMENT_SIZE];
	UCHAR	stn_id, dev_id;
	int		i,grp_num;
 	GtkWidget *menu,*items;
	int MenuPointer;
	gchar *pConvert=0;
  
	menu =gtk_option_menu_get_menu(GTK_OPTION_MENU(db_tab_window_info[crt_n].dev_optionmenu));
	gtk_option_menu_remove_menu(GTK_OPTION_MENU(db_tab_window_info[crt_n].dev_optionmenu));

	menu =gtk_menu_new();
	gtk_widget_set_usize(menu, 130, -1);

	stn_id= db_tab_window_info[crt_n].select_pt.stn_id;
	if(-1==GetGroupNumById(stn_id, &grp_num))
		return;

	dev_id= 0;
 	for(i=1; i<=grp_num; i++) 
  	{  		
		if(-1==GetGroupCommentById(stn_id, (UCHAR)i, dev_comment))
			continue;
		items = gtk_menu_item_new_with_label(pConvert=_toUtf8(dev_comment));
		G_SAFE_FREE(pConvert);
		gtk_container_add(GTK_CONTAINER(menu),items);
		MenuPointer=getMenuDataPointer(crt_n,i);
		gtk_signal_connect(GTK_OBJECT(items), "activate", GTK_SIGNAL_FUNC(UpdateDbDevSelect), (gpointer)MenuPointer);
		if(!dev_id)
			dev_id= i;
	}

    gtk_option_menu_set_menu(GTK_OPTION_MENU(db_tab_window_info[crt_n].dev_optionmenu),menu);
	gtk_option_menu_set_history(GTK_OPTION_MENU(db_tab_window_info[crt_n].dev_optionmenu), 0);
	db_tab_window_info[crt_n].select_pt.dev_id= dev_id;
	gtk_widget_show_all(db_tab_window_info[crt_n].dev_optionmenu);
}



void UpdateDbDataTypeMenu(int crt_n) 
{
	char	datatype_longname[RECORD_LONGNAME_SIZE];
	UCHAR	stn_id, dev_id, data_type;
	int		i,j,rec_num,entry_num;
 	GtkWidget  *menu,*items;
	int		menu_index, active_index;
	int MenuPointer;
	gchar *pConvert=0;

	menu =gtk_option_menu_get_menu(GTK_OPTION_MENU(db_tab_window_info[crt_n].datatype_optionmenu));
	gtk_option_menu_remove_menu(GTK_OPTION_MENU(db_tab_window_info[crt_n].datatype_optionmenu));

	menu =gtk_menu_new();
	gtk_widget_set_usize(menu, 130, -1);

	stn_id= db_tab_window_info[crt_n].select_pt.stn_id;
	dev_id= db_tab_window_info[crt_n].select_pt.dev_id;
	if(-1==GetRecordNumById(stn_id, dev_id, &rec_num))
		return;

	data_type= 0;
	menu_index= active_index= 0;
 	for(i=1,j=0; j<rec_num; i++) 
  	{  		
		if(i>=MAX_DATATYPE_NUM)
			break;
		if(-1==GetEntryNumById(stn_id, dev_id, (UCHAR)i, &entry_num))
			continue;
		GetRecLongnameByType((UCHAR)i, (char *)datatype_longname);
		items = gtk_menu_item_new_with_label(pConvert=_toUtf8(datatype_longname));
		G_SAFE_FREE(pConvert);
		gtk_container_add(GTK_CONTAINER(menu),items);
	
		MenuPointer=getMenuDataPointer(crt_n,i);		
		gtk_signal_connect(GTK_OBJECT(items), "activate", GTK_SIGNAL_FUNC(UpdateDbDataTypeSelect), (gpointer)MenuPointer);
	
		j++;
		if(!data_type)
		{
			data_type= i;
			active_index= 0;
		}
		if(i==db_tab_window_info[crt_n].select_pt.data_type)
		{
			data_type= i;
			active_index= menu_index;
		}
		menu_index++;
	}

    gtk_option_menu_set_menu(GTK_OPTION_MENU(db_tab_window_info[crt_n].datatype_optionmenu),menu);
	gtk_option_menu_set_history(GTK_OPTION_MENU(db_tab_window_info[crt_n].datatype_optionmenu), active_index);
	db_tab_window_info[crt_n].select_pt.data_type= data_type;
	gtk_widget_show_all(db_tab_window_info[crt_n].datatype_optionmenu);
}



GtkWidget* CreateDbStnMenu(int crt_n) 
{
	char	stn_comment[STATION_COMMENT_SIZE];
	int		index,stn_num;
 	GtkWidget *optionmenu, *menu,*items;
	int		menu_index, active_index;
	UCHAR	stn_id;
	int MenuPointer;
	gchar *pConvert=0;
//	int menu[]
  
	menu =gtk_menu_new();
	gtk_widget_set_usize(menu, 130, -1);

  	GetStationNum(&stn_num);
	stn_id= 0;
	menu_index= active_index= 0;
 	for(index=1; index<=stn_num; index++) 
  	{
		if(-1==GetStnIdByIndex(index, &stn_id))
			continue;
		
 		if( GetStnCommentById((UCHAR)stn_id,(char*)( stn_comment))==-1 )
			continue;

		items = gtk_menu_item_new_with_label(pConvert=_toUtf8(stn_comment));
		G_SAFE_FREE(pConvert);
		gtk_container_add(GTK_CONTAINER(menu),items);

		MenuPointer=getMenuDataPointer(crt_n,stn_id);
		gtk_signal_connect(GTK_OBJECT(items), "activate", GTK_SIGNAL_FUNC(UpdateDbStnSelect), (gpointer)MenuPointer);
		
		if(stn_id==db_tab_window_info[crt_n].select_pt.stn_id)
			active_index= menu_index;
		menu_index++;
  	}



	optionmenu = gtk_option_menu_new();
    gtk_option_menu_set_menu(GTK_OPTION_MENU(optionmenu),menu);
	gtk_option_menu_set_history(GTK_OPTION_MENU(optionmenu), active_index);
	GetStnIdByIndex(active_index+1, &stn_id);
	db_tab_window_info[crt_n].select_pt.stn_id= stn_id;
	db_tab_window_info[crt_n].stn_optionmenu= optionmenu;
	return optionmenu;
}




GtkWidget* CreateDbDevMenu(int crt_n) 
{
	char	dev_comment[GROUP_COMMENT_SIZE];
	UCHAR	stn_id, dev_id;
	int		i,grp_num;
 	GtkWidget *optionmenu, *menu,*items;
	int		menu_index, active_index;
	int MenuPointer;
	gchar *pConvert=0;
  
	menu =gtk_menu_new();
	gtk_widget_set_usize(menu, 130, -1);

	stn_id= db_tab_window_info[crt_n].select_pt.stn_id;
	if(-1==GetGroupNumById(stn_id, &grp_num))
		return (GtkWidget*)NULL;

	dev_id= 0;
	menu_index= active_index= 0;
 	for(i=1; i<=grp_num; i++) 
  	{  		
		if(-1==GetGroupCommentById(stn_id, (UCHAR)i, dev_comment))
			continue;
		items = gtk_menu_item_new_with_label(pConvert=_toUtf8(dev_comment));	 
		G_SAFE_FREE(pConvert);
		gtk_container_add(GTK_CONTAINER(menu),items);

		MenuPointer=getMenuDataPointer(crt_n,i);
		gtk_signal_connect(GTK_OBJECT(items), "activate", GTK_SIGNAL_FUNC(UpdateDbDevSelect), (gpointer)MenuPointer);
		
		if(!dev_id)
		{
			dev_id= i;
			active_index= 0;
		}
		if(i==db_tab_window_info[crt_n].select_pt.dev_id)
		{
			dev_id= i;
			active_index= menu_index;
		}
		menu_index++;
	}

	optionmenu = gtk_option_menu_new();
        gtk_option_menu_set_menu(GTK_OPTION_MENU(optionmenu),menu);
	gtk_option_menu_set_history(GTK_OPTION_MENU(optionmenu), active_index);
	db_tab_window_info[crt_n].select_pt.dev_id= dev_id;
	db_tab_window_info[crt_n].dev_optionmenu= optionmenu;
	return optionmenu;
}



GtkWidget* CreateDbDataTypeMenu(int crt_n) 
{
	char	datatype_longname[RECORD_LONGNAME_SIZE];
	UCHAR	stn_id, dev_id, data_type;
	int		i,j,rec_num,entry_num;
 	GtkWidget *optionmenu, *menu,*items;
	int		menu_index, active_index;
	int MenuPointer;
	gchar *pConvert=0;
  
	menu =gtk_menu_new();
	gtk_widget_set_usize(menu, 130, -1);

	stn_id= db_tab_window_info[crt_n].select_pt.stn_id;
	dev_id= db_tab_window_info[crt_n].select_pt.dev_id;
	if(-1==GetRecordNumById(stn_id, dev_id, &rec_num))
		return (GtkWidget*) NULL;

	data_type= 0;
	menu_index= active_index= 0;
 	for(i=1,j=0; j<rec_num; i++) 
  	{  		
		if(i>=MAX_DATATYPE_NUM)
			break;
		if(-1==GetEntryNumById(stn_id, dev_id, (UCHAR)i, &entry_num))
			continue;
		GetRecLongnameByType((UCHAR)i, (char *)datatype_longname);
		items = gtk_menu_item_new_with_label(pConvert=_toUtf8(datatype_longname));	 
		G_SAFE_FREE(pConvert);
		gtk_container_add(GTK_CONTAINER(menu),items);
		
		MenuPointer=getMenuDataPointer(crt_n,i);
		gtk_signal_connect(GTK_OBJECT(items), "activate", GTK_SIGNAL_FUNC(UpdateDbDataTypeSelect), (gpointer)MenuPointer);
		
		j++;
		if(!data_type)
		{
			data_type= i;
			active_index= 0;
		}
		if(i==db_tab_window_info[crt_n].select_pt.data_type)
		{
			data_type= i;
			active_index= menu_index;
		}
		menu_index++;
	}

	optionmenu = gtk_option_menu_new();
    gtk_option_menu_set_menu(GTK_OPTION_MENU(optionmenu),menu);
	gtk_option_menu_set_history(GTK_OPTION_MENU(optionmenu), active_index);
	db_tab_window_info[crt_n].select_pt.data_type= data_type;
	db_tab_window_info[crt_n].datatype_optionmenu= optionmenu;
	return optionmenu;
}




GtkWidget *CreateDbToolbar (GtkWidget * parent, int crt_n)
{
  	GtkWidget *box;  	
	GtkWidget *toolbar; 
  	GtkWidget *button_search,*button_exit,*button_page_up,*button_page_down,*button_data_polling,*button_alm_ack ; 
 	gchar *pConvert=0;

  	toolbar = gtk_handle_box_new();

 	box = gtk_hbox_new (FALSE, 5);
 	gtk_container_add(GTK_CONTAINER(toolbar), box);
  	gtk_container_border_width(GTK_CONTAINER(box), 5);

	gtk_box_pack_start(GTK_BOX(box), CreateDbStnMenu(crt_n), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box), CreateDbDevMenu(crt_n), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box), CreateDbDataTypeMenu(crt_n), FALSE, FALSE, 0);

	
    	
	button_search = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_STR_SEARCH));  
	G_SAFE_FREE(pConvert);

	gtk_box_pack_start(GTK_BOX(box), button_search, FALSE, FALSE, 0);    
    
  	gtk_signal_connect(GTK_OBJECT(button_search), "clicked", GTK_SIGNAL_FUNC(PopupSearchDlg), (gpointer)crt_n);   

	button_page_up = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_PAGE_UP)); 
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start(GTK_BOX(box), button_page_up, FALSE, FALSE, 0);   
   	gtk_signal_connect(GTK_OBJECT(button_page_up), "clicked", GTK_SIGNAL_FUNC(DbBarDecrease), NULL);   

	button_page_down = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_PAGE_DOWN)); 
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start(GTK_BOX(box), button_page_down, FALSE, FALSE, 0);   
   	gtk_signal_connect(GTK_OBJECT(button_page_down), "clicked", GTK_SIGNAL_FUNC(DbBarIncrease), NULL);   

	//召唤数据 默认显示，str_text_list未定义OIX_SELF_SHOW_DATA_POLLING串时显示，
	//OIX_SELF_SHOW_DATA_POLLING 为0时不显示
	if (strcmp( _CS_(OIX_SELF_SHOW_DATA_POLLING),"1")==0||strcmp( _CS_(OIX_SELF_SHOW_DATA_POLLING),"OIX_SELF_SHOW_DATA_POLLING")==0)
	{
		button_data_polling = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_DATA_POLLING)); 
		G_SAFE_FREE(pConvert);
		gtk_box_pack_start(GTK_BOX(box), button_data_polling, FALSE, FALSE, 0);   
   		gtk_signal_connect(GTK_OBJECT(button_data_polling), "clicked", GTK_SIGNAL_FUNC(Data_Polling), NULL); 
	}

	button_alm_ack = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_AlarmAck)); 
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start(GTK_BOX(box), button_alm_ack, FALSE, FALSE, 0);   
   	gtk_signal_connect(GTK_OBJECT(button_alm_ack), "clicked", GTK_SIGNAL_FUNC(AcknDbTabFlag), (gpointer)crt_n); 	  
    
	button_exit = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_EXIT)); 
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start(GTK_BOX(box), button_exit, FALSE, FALSE, 0);   
	gtk_signal_connect(GTK_OBJECT(button_exit), "clicked", GTK_SIGNAL_FUNC(db_tab_quit), (gpointer)crt_n);   
		      	      	    
  	return (toolbar);
}


static  GtkWidget* CreateDbTabWindow(GtkWidget *parent,	int crt_n)
{
	GtkWidget	*window;
	GtkWidget	*vbox,*hbox4;
	GtkWidget	*toolbar,*scrollbar ; 
	gchar *pConvert=0;
/*
    gtk_set_locale();
    gtk_rc_add_default_file(GTK_RC_FILE);  */
					

    window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    
     gtk_window_set_default_size(GTK_WINDOW(window),(int)(DBTAB_WINDOW_WIDTH*xscale[crt_n]),(int) (DBTAB_WINDOW_HEIGHT*yscale[crt_n]));
	gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(parent));
        gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	/*gtk_window_set_default_size(GTK_WINDOW(window), DBTAB_WINDOW_WIDTH, DBTAB_WINDOW_HEIGHT);  */
	gtk_window_set_title(GTK_WINDOW(window), pConvert=_toUtf8EX(OIX_CSTR_Title));
	G_SAFE_FREE(pConvert);
	gtk_signal_connect(GTK_OBJECT(window),"destroy",GTK_SIGNAL_FUNC(db_tab_quit), (gpointer)crt_n);
    

	vbox=gtk_vbox_new(FALSE, 5);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

	toolbar =  CreateDbToolbar(window, crt_n);
	gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, TRUE, 0);

    
	db_scrolled_window = gtk_scrolled_window_new (NULL, NULL);
//	gtk_box_pack_start(GTK_BOX(vbox), db_scrolled_window, TRUE, TRUE, 0);
   
	db_tab_canvas[crt_n]= gtk_drawing_area_new();
	gtk_widget_set_usize(db_tab_canvas[crt_n],(int)(DB_CANVAS_WINDOW_WIDTH*xscale[crt_n]),(int)(DB_CANVAS_WINDOW_HEIGHT*yscale[crt_n])); 
    
	hbox4 =  gtk_hbox_new(FALSE, 0);
    gtk_widget_show(hbox4); 
    gtk_box_pack_start(GTK_BOX(vbox), hbox4, TRUE, TRUE, 0);
	
    gtk_box_pack_start(GTK_BOX(hbox4), db_tab_canvas[crt_n], TRUE, TRUE, 0);
    adj2[crt_n] =(GtkAdjustment*)( gtk_adjustment_new(0.0,0.0,101.0,0.1,1.0,1.0));
    scrollbar = gtk_vscrollbar_new(GTK_ADJUSTMENT(adj2[crt_n]));
    gtk_widget_show(scrollbar)   ;


    g_object_set_data(G_OBJECT(window),"scrollbar",scrollbar);
    gtk_box_pack_start(GTK_BOX(hbox4), scrollbar, FALSE, FALSE, 0);


	gtk_widget_set_events(db_tab_canvas[crt_n], GDK_EXPOSURE_MASK 
		| GDK_SCROLL_MASK 
		| GDK_LEAVE_NOTIFY_MASK 
		| GDK_BUTTON_PRESS_MASK 
		| GDK_POINTER_MOTION_MASK );
    gtk_signal_connect (GTK_OBJECT (db_tab_canvas[crt_n]),"scroll-event",GTK_SIGNAL_FUNC(dbtab_scroll_event),GINT_TO_POINTER(crt_n)) ;
	gtk_signal_connect (GTK_OBJECT (db_tab_canvas[crt_n]),"expose_event",GTK_SIGNAL_FUNC(dbtab_expose_events),(gpointer)crt_n) ;
	gtk_signal_connect (GTK_OBJECT (db_tab_canvas[crt_n]), "realize",GTK_SIGNAL_FUNC (dbtab_draw_area_realize),(gpointer)crt_n);
	gtk_signal_connect (GTK_OBJECT (db_tab_canvas[crt_n]), "button_press_event",GTK_SIGNAL_FUNC (DbTracingCB),(gpointer)crt_n );
	gtk_signal_connect (GTK_OBJECT (db_tab_canvas[crt_n]),"size_allocate",GTK_SIGNAL_FUNC(db_canvas_allocate),GINT_TO_POINTER(crt_n)) ;
   // gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(db_scrolled_window),db_tab_canvas[crt_n] );
	g_signal_connect(G_OBJECT(adj2[crt_n]),"value_changed",G_CALLBACK(cb_scroll_page_db),GINT_TO_POINTER (crt_n));
	gtk_widget_realize(GTK_WIDGET(db_tab_canvas[crt_n])) ;
	
	return  window ;  
}



void 	ProgDbTab (GtkWidget *parent, int crt_n )
{	

	GetCrtNumberOfWgt ( GTK_WIDGET(parent), &crt_n);
	if( crt_n < 0 )
		return; 	     

 	if( db_pop_shell[crt_n] != (GtkWidget*)NULL )
    {
        gtk_window_present(GTK_WINDOW(db_pop_shell[crt_n]));
		return;
	}

	db_tab_window_info[crt_n].subwindow_select_flag = 0; 
	db_tag[crt_n]	= ITEM_TAG_OFF;

	if(db_tab_window_info[crt_n].select_pt.stn_id==0)
		db_tab_window_info[crt_n].select_pt.stn_id= 1;
	if(db_tab_window_info[crt_n].select_pt.dev_id==0)
		db_tab_window_info[crt_n].select_pt.dev_id= 1;
	if(db_tab_window_info[crt_n].select_pt.data_type==0)
		db_tab_window_info[crt_n].select_pt.data_type= ANA_TYPE;
	
	db_tab_window_info[crt_n].crt_n= crt_n;

	db_pop_shell[crt_n] = CreateDbTabWindow( parent, crt_n ) ;
       
	gtk_window_set_position(GTK_WINDOW(db_pop_shell[crt_n]),GTK_WIN_POS_CENTER);
	gtk_widget_show_all(db_pop_shell[crt_n]);
   
	if (db_timeout_id[crt_n] != 0 )
	{
		gtk_timeout_remove(db_timeout_id[crt_n]);	
		db_timeout_id[crt_n]= 0;
	}
	
	db_timeout_id[crt_n]= gtk_timeout_add (1000, (GtkFunction)DbDspProc, (gpointer)crt_n); 
	InitPageDbInfo(&pageDb[crt_n] ,  crt_n,0) ;
// 	
// #ifdef  SUN_OS
// 	pthread_t tid1,tid2;
// 	int error;
// #endif
// #ifdef	WINDOWS_OS
// 	HANDLE  Hthread ; 
// #endif 
// 	
// #ifdef  SUN_OS
// 	error = pthread_create(&tid1, NULL, test, (GtkWidget * )(parent ));
// 	if(error!=-1)
// 	{
// 		printf("Create test Success!\n");
// 		
// 	}
// 	else
// 	{
// 		printf("Create test failed!!! error=%d\n",error);
// 		
// 	}
// #endif 
// 	
// #ifdef  WINDOWS_OS
// 	Hthread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)test,  (GtkWidget * )(parent), 0, NULL) ; 
// 	if(Hthread!=NULL)
// 	{
// 		printf("Create test Success!\n");
// 		
// 	}
// 	else
// 	{
// 		printf("Create test failed!!! error=%d\n",Hthread);
// 	}
// #endif
}

void	AcknDbTabFlag (GtkWidget *w,int crt_n )
{
	int	i, i1, i2, ackn_st,send_cnt = 0;
	POINTER tmp_ptr;
	DMS_COMMON	db_cmm;
        
	if(!CheckUserPrivilege(cur_usr_name, PRIV_ALM_ACK))
	{
		PopupMessageBox ( db_pop_shell[crt_n], MSG_ALARM_ACK);
		return ;
	}

	i1=	pageDb[crt_n].PageStart	;
	i2=pageDb[crt_n].PageEnd;
		


	tmp_ptr = db_tab_window_info[crt_n].select_pt;
	for ( i= i1; i < i2; i++)
     {
		tmp_ptr.pt_id	= i;
		
		if ( ReadEntryById ( &tmp_ptr, &db_cmm ) == -1)
		{
			printf ("AcknDbTabFlag: No this point in the database file !!!\n" );
		}
		else
		{
			ackn_st = db_cmm.fixed.iost.almNoAck;
			if ( ackn_st == UNACKN )
			{
				CmdBufInit ( crt_n );
				cmd_buf[crt_n].stn_id		= db_cmm.point.stn_id;
        		cmd_buf[crt_n].dev_id		= db_cmm.point.dev_id;
				cmd_buf[crt_n].point_id		= db_cmm.point.pt_id;
				cmd_buf[crt_n].data_type	= db_cmm.point.data_type;
				cmd_buf[crt_n].type_id		= DPS_IOST_SET;  
				cmd_buf[crt_n].status.state = ACKN;
				cmd_buf[crt_n].status.opr  = IOST_SET_ALM_NOACK;

				ProgDoAckCelBrdcst ( crt_n );
				send_cnt++;	
				if ( send_cnt > MAX_NETCMD_SEG_NUM )
				{
					MySleep ( 10 );
					send_cnt = 0;
				}
            }
        }
	}
}




