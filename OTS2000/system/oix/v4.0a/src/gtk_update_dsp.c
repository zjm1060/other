/*-----------------------------------------------------------------------

  Upadate from X11 to GTK ;
  Created by chi.hailong
  Date : 2005.9.17 
  updated by zyp on 2006.3 for v4.0db
  UNIX 已经与WINDOWS版本同步!!!SYNC chihailong  2007.05.24 
  last SYNC hcl 2007.11.28
---------------------------------------------------------------------*/

#include <time.h>
#include <stdio.h>
#include <math.h>

#include <gtk/gtk.h>


/*  #define	DEBUG		*/

#define	DAY_RPT		1
#define	MONTH_RPT	2
#define YEAR_RPT        3

#define CALC_ACC       "24_D"
#define CALC_ACC1       "_S"

#define MAX_CANVAS_PT  900
#define DSP_TIMEOUT_DEF
#include	"../inc/oix.h"
#include	"../inc/gtk_widgets.h"
#include        "../../../pas/v4.0a/inc/trend_data_rec.h"
#include        "/home/ems/system/pas/v4.0a/inc/rpt_def.h"
extern   int  iSetDispEntry[MAX_CRT_NUM] ;

extern   short		interlock_pt_num;
extern   short		interlock_gt_num;
extern   short		interlock_et_num;
extern   InterLock	*interlock;
extern   I_Group	*group;
extern   I_Entry	*entry;
extern   CURVE_SHM_DATA  *shm_real_curve;
extern	 CURVE_DEF   	RealCCurvePt ;
extern	POINTER	pnt_tab_ptr[MAX_CRT_NUMBER];
int		blink_flag[MAX_CRT_NUMBER];
short	blink_status[MAX_CRT_NUMBER];
extern char cmd_str[100];
int ana_sbo_id[MAX_CRT_NUMBER];
extern int ReadRptEntryByName ( char*, short, short, short,  char*, int*, RPT_DATA	*);
extern int   GetColor(short id , GdkColor *clr );
extern int SelectLayer[MAX_CRT_NUMBER][LAYERNUM];
struct 
{
    char	man_set[2];
    char	chan_fail[2];
    char    ctrl_block[2];
	char    draw_info[4]  ;
}   data_quality ;

int	anim_cnt[MAX_CRT_NUMBER]	= {0, 0};
int	fault_sta = 0;

time_t 		t;
struct tm       time_ptr;
int		hour;



void	DrawDynamicRealCurve (int crt_n, GdkDrawable	*drawable, GdkGC	*gc, GBRCD	*rcd ) ; 

extern void GetAnaLmtStateStr(DMS_COMMON *db_cmm,char *value_str);
extern void GetAnaTrLmtStateStr(DMS_COMMON *db_cmm,char *value_str);

void	DrawSubDynamicRcd (int crt_n,GBRCD	*rcd, GdkGC	*gc, GdkDrawable	*drawable, int sub_win_n);
void	DrawSubDynamicName (int crt_n, GdkWindow	*drawable, GdkGC	*gc, GBRCD	*rcd,int sub_win_n );

void	DrawDynamicDsp (int crt_n, GdkWindow	*window );
void	DrawDynamicRcd (int crt_n,GBRCD	*rcd, GdkGC	*gc, GdkWindow	*drawable );
void	DrawDynamicColorGrp (int crt_n,GdkWindow *drawable, GdkGC *gc, GBRCD *rcd );
void	DrawDynamicBarChart (int crt_n,GdkWindow	*drawable, GdkGC	*gc, GBRCD	*rcd );
int		GetDynamicColor (int  crt_n, GBRCD	*rcd,short rt_status,short alm_status );
void	DrawDynLinePlot (int crt_n,GdkWindow	*drawable, GdkGC	*gc, GBRCD	*rcd );
void	DrawDynPQ_Chart (int crt_n,GdkWindow	*drawable, GdkGC	*gc, GBRCD	*rcd );
/*void DrawHistBarChart (int crt_n,GdkWindow	*drawable, GdkGC	*gc, GBRCD	*rcd );*/
void	DrawItemTag (int crt_n,GdkWindow	*drawable, GdkGC	*gc, BACKRCD	*rcd ,int tagtype);
void	DrawDynamicSymbolGrp (int crt_n, GdkWindow	*drawable, GdkGC	*gc, GBRCD	*rcd );
void	DrawDynamicImpSet (int crt_n,GdkWindow	*drawable, GdkGC	*gc, GBRCD	*rcd );
void	DrawDynamicAnaStatus (int crt_n, GdkWindow	*drawable, GdkGC	*gc, GBRCD	*rcd );
void	DrawDynamicName (int crt_n,GdkWindow	*drawable, GdkGC	*gc, GBRCD	*rcd );
void	DrawDynamicSboStatus (int crt_n, GdkWindow	*drawable, GdkGC	*gc, GBRCD	*rcd );
void DrawRptNumeric (int crt_n,GdkWindow	*drawable, GdkGC	*gc, GBRCD	*rcd );
void	DrawRptTime (int crt_n,GdkWindow	*drawable, GdkGC	*gc, GBRCD	*rcd );
void	DrawDynamicNumeric (int crt_n, GdkWindow	*drawable, GdkGC	*gc, GBRCD	*rcd );
void	DrawDynamicInteger (int crt_n,GdkWindow	*drawable, GdkGC	*gc, GBRCD	*rcd );
void	DrawDynamicStringGrp (int crt_n, GdkWindow	*drawable, GdkGC	*gc, GBRCD   *rcd );
void    DrawDialMeter (int crt_n,GdkDrawable *drawable, GdkGC *gc, GBRCD *rcd , float value );
void	DrawDynamicDialChart (int crt_n,GdkWindow	*drawable, GdkGC	*gc, GBRCD	*rcd );
void	DrawDynamicCurvePt (int crt_n, GdkWindow	*drawable, GdkGC	*gc, GBRCD	*rcd ) ;
void	DrawDynamicCurveLoad (int crt_n, GdkWindow	*drawable, GdkGC	*gc, GBRCD	*rcd ) ;
void	DrawDynamicTempChara (int crt_n, GdkWindow	*drawable, GdkGC	*gc, GBRCD	*rcd ) ;

void	DrawDynamicVibraArea (int crt_n,GdkWindow	*drawable, GdkGC	*gc, GBRCD	*rcd ) ;
void	DrawDynamicManSetTime (int crt_n,GdkWindow	*drawable, GdkGC	*gc, GBRCD	*rcd );
extern void	GetX_Coord_F (float h_lmt,float l_lmt,float v,float x1,float x2,float *y1,float *y2 );
extern void	PopupMessageBox (GtkWidget *parent,char msg[] );
extern void 	CmdBufInit ( int crt_n );
extern void	ProgDoAckCelBrdcst (int crt_n );
extern void	DrawSymbol (int crt_n,  GdkDrawable	*drawable,  GdkGC *gc, BACKRCD	*rec,int fb,BOOL isDYN, BOOL disable_flag );
extern void	DrawRcd (int crt_n, GdkDrawable	*drawable, GdkGC *gc, BACKRCD *rcd ,BOOL isTrutype);
extern int ColorId3to4(int id3);
extern BOOL	GetCtrlRight (int crt_n,DMS_COMMON dms_cmm,int IsCascade);
extern int	XtoSX (int crt_n,int x);
extern int	YtoSY (int crt_n,int y);
extern void    SetRcdColor (int crt_n,   GdkGC	*gc, BACKRCD 	*rcd);
extern void	DrawDynBarLmtRcd (int crt_n, GdkDrawable *drawable, GBRCD	*rcd,  BAR_GRAPH_LMT *lmt ) ;
extern int	Min(int x,int y);
extern void	DrawDynLine (int crt_n,GBRCD	*rcd, float	curve[] ) ;
extern void	DrawDynPQ_Dot (int crt_n, GdkDrawable *drawable , GBRCD	*rcd,float	curve[] );
extern void	DrawTag ( int crt_n, GdkDrawable  *drawable, GdkGC *gc, GBRCD	*rcd   );
extern void	GetRptTimeFromSelectFile (short crt_n, char select_rpt_hist_data[MAX_FULL_FILE_NAME_LEN], char	*char_date );
extern void DrawGif (int crt_n,   GdkDrawable *drawable, GdkGC	*gc, BACKRCD	*rcd );
extern void    windk_draw_text(GdkDrawable * drawable ,GdkFont    *font,GdkGC  *gc, int x,int y,const char      *text,int  strlength,int ctr_n) ;
extern void  CurveSampleFun(int startT , int endT , int curid, float*   ,int *nextPos);
extern void	DrawDspBackground (int crt_n,  GdkDrawable *drawable );
extern int    gdk_rgb(int red  ,int green, int  blue , GdkColor *clr  );

void UpdatePromptFrame(void);
void ClearArea (int crt_n, GdkDrawable *drawable,int x1,int y1,int width,int height,int expose_mode );


gint    UpdateDsp (int crt_n)
{
	
	if (   crt_n  <0   ||  crt_n > MAX_CRT_NUMBER)
		return FALSE ;
	//////////////////////////////////////////////////////////////////////////
	// hcl delete old code 
	//use layer filter 
	// 	gdk_window_copy_area(pixmap[crt_n], info[crt_n].gc ,0 , 0 , bakgrd_pixmap[crt_n] ,0 , 0 ,Canvas_RECT[crt_n].width, Canvas_RECT[crt_n].height    );
	// 	gdk_draw_pixmap(pixmap[crt_n], info[crt_n].gc, bakgrd_pixmap[crt_n] ,0 , 0 ,0 , 0 ,Canvas_RECT[crt_n].width, Canvas_RECT[crt_n].height  );	
	//     
	// 	DrawDynamicDsp ( crt_n, pixmap[crt_n]);    
	// 	gdk_draw_drawable(canvas[crt_n]->window, info[crt_n].gc ,pixmap[crt_n] ,0 , 0 ,  0 , 0 ,Canvas_RECT[crt_n].width, Canvas_RECT[crt_n].height    );
	//////////////////////////////////////////////////////////////////////////
	//use layer filter  DrawDspBackground need flash
	if (UseLayerFilter[crt_n].changeflag)
	{
		ClearArea (crt_n,  bakgrd_pixmap[crt_n], 0, 0, Canvas_RECT[crt_n].width+200, Canvas_RECT[crt_n].height+400, k_no_expose);  
		DrawDspBackground (crt_n, bakgrd_pixmap[crt_n]);  	
	}
	UseLayerFilter[crt_n].changeflag=0;
	
	gdk_draw_pixmap(pixmap[crt_n], info[crt_n].gc, bakgrd_pixmap[crt_n] ,0 , 0 ,0 , 0 ,Canvas_RECT[crt_n].width, Canvas_RECT[crt_n].height  );	
	
	DrawDynamicDsp (crt_n, pixmap[crt_n]);    
	gdk_draw_drawable(canvas[crt_n]->window, info[crt_n].gc ,pixmap[crt_n] ,0 , 0 ,  0 , 0 ,Canvas_RECT[crt_n].width, Canvas_RECT[crt_n].height    );
	//////////////////////////////////////////////////////////////////////////
	
	UpdatePromptFrame();
	
	if ( rpt_tag[crt_n]	 == RPT_TAG_OFF)
	{
		if (dsp_timer_id[crt_n] !=0 )
		{
			gtk_timeout_remove(dsp_timer_id[crt_n]); 
			dsp_timer_id[crt_n]= 0;
		}
		dsp_timer_id[crt_n]   =gtk_timeout_add (  1000, (GtkFunction)UpdateDsp,(gpointer)crt_n); 
	}
	else if ( rpt_tag[crt_n] == RPT_TAG_ON)
	{
		if (dsp_timer_id[crt_n] !=0 )
		{
			gtk_timeout_remove(dsp_timer_id[crt_n]); 
			dsp_timer_id[crt_n]= 0;
		}
		dsp_timer_id[crt_n]   =gtk_timeout_add (  10000, (GtkFunction)UpdateDsp,(gpointer)crt_n);
	}
	
	if (blink_flag[crt_n] == 0)
	{	blink_flag[crt_n]	= 1;	}
	else
	{	blink_flag[crt_n]	= 0;	}
	
	
	if ( anim_cnt[crt_n] >= MAX_ANIMATE_SYMBOLS-1 )
	{
		anim_cnt[crt_n] = 0;
	}
	else
	{
		anim_cnt[crt_n]++;
	}
	if (shm_ots_addr->dbinflag  )
	{
		OpenDspFile(crt_n, shm_ots_addr->dbinname );
		shm_ots_addr->dbinflag  = 0 ;  
		
	}
	
    if  (shm_ots_addr->childflag ==1)
	{
		strcpy(sub_dsp_name,shm_ots_addr->child.name );
		OpenSubDspFile (   crt_n, (void *)NULL ,   shm_ots_addr);
		shm_ots_addr->childflag   = 0 ; 
	}
	else if (shm_ots_addr->childflag == 2  /*|| shm_ots_addr->childflag == 0 */  )  /*gtoto next step and close subwindows*/
	{
		DestroySubWin(main_w[crt_n],1) ;   /*仿真时,如果有前一个子画面就关掉其;*/			
		sub_win_shell[1][crt_n]=(void*)NULL ;	
		shm_ots_addr->childflag   = 0 ; 
	}
	return  TRUE; 
}

void	DrawSubDynamicDsp (int crt_n,GdkDrawable	* drawable ,int sub_win_n)
{
	int	i;
	GBRCD	rcd;
	GCOND_BUFF	*layer;
	/*printf("##in DrawSubDynamicDsp::sub_win_n=%d\n",sub_win_n);*/
	layer	= &gcondb[sub_win_n][crt_n];
	for ( i=0; i < layer->nextpos; i++ )
	{
		rcd	= layer->buffer[i];
		if ( rcd.lnk.dyn.dyn_tag == DYN_TAG_ON 
			|| ( rcd.lnk.cgrp.tag == DYN_CGRP_TAG_ON && rcd.lnk.cgrp.cgrp_id !=0) 
			|| ( rcd.lnk.cgrp.tag == DYN_CGRP_TAG_ADD_ON && rcd.lnk.cgrp.cgrp_id !=0)) 
		{                 
			rcd.bck.rect.x1=rcd.bck.rect.x1-sub_win[sub_win_n][crt_n].x1;
			rcd.bck.rect.y1=rcd.bck.rect.y1-sub_win[sub_win_n][crt_n].y1;
			rcd.bck.rect.x2=rcd.bck.rect.x2-sub_win[sub_win_n][crt_n].x1;
			rcd.bck.rect.y2=rcd.bck.rect.y2-sub_win[sub_win_n][crt_n].y1;
			/*  printf("in DrawSubDynamicDsp: x1=%d,y1=%d,x2=%d,y2=%d,sub_x1=%d,sub_y1=%d\n",rcd.bck.rect.x1,rcd.bck.rect.y1,
			rcd.bck.rect.x2,rcd.bck.rect.y2,sub_win[sub_win_n][crt_n].x1,sub_win[sub_win_n][crt_n].y1);
			printf("info[%d].sub_gc=%d\n",crt_n,info[crt_n].sub_gc[sub_win_n]);
			printf("drawable=%d\n",drawable);*/
			DrawSubDynamicRcd ( crt_n, &rcd, info[crt_n].sub_gc[sub_win_n], drawable ,sub_win_n); 
		}
	}
}

void DrawDynamicDsp (int crt_n, GdkDrawable	*window )
{
	int	i;
	//	int layernum;
	GDBUFF	*layer;
	GBRCD	*rcd;
	DMS_COMMON dms_cmm;
	
	double  r ;  GdkColor  bkColor ;
	char devName[64]	;
	char entryName[128] ,dispName[256];
	char *l1  , *l2 ; 
	int x , y ,w,h; 
	layer	= &gdbase[crt_n];
	/*printf("info[%d].gc=%d,*window=%d\n",crt_n,info[crt_n].gc,*window);*/
	
	for ( i=0; i < layer->nextpos; i++ )
	{
		rcd	= &layer->buffer[i];
		if( (!SelectLayer[crt_n][rcd->bck.layer])&&UseLayerFilter[crt_n].UseFilter)//hcl add  layerfalter
		{
			continue;
		}
		
		if (( zoomdb[crt_n].factor >= gdbase[crt_n].layer[rcd->bck.layer].min_visible &&
			zoomdb[crt_n].factor <= gdbase[crt_n].layer[rcd->bck.layer].max_visible )||UseLayerFilter[crt_n].UseFilter)//hcl add  layerfalter
		{
			/* if rcd is a dynamic or colorgroup element update the element dsp with RTDB data. */
			
			if ( rcd->lnk.dyn.dyn_tag == DYN_TAG_ON  
				||( rcd->lnk.cgrp.tag == DYN_CGRP_TAG_ON && rcd->lnk.cgrp.cgrp_id !=0
				||(rcd->lnk.cgrp.tag == DYN_CGRP_TAG_ADD_ON && rcd->lnk.cgrp.cgrp_id !=0)) ) 
			{			
				
				DrawDynamicRcd ( crt_n,rcd, info[crt_n].gc, window );     
				
			}
			if(strcmp(rcd->lnk.dbname,"")!=0  || strcmp ( rcd->lnk.act.fmt_def.dsp.name,"")!=0 )  
			{
				//////////////////////////////////////////////////////////////////////////////
				if (  iSetDispEntry[crt_n])
				{
					
					r = (double)( (float)rand() / (float)32767 );  //r--随机数
					r =1.0   ; 
					if(strcmp(rcd->lnk.dbname,"")!=0)
					{
						memset(devName,0,sizeof(devName)) ;
						memset(entryName,0,sizeof(devName)) ;
						memset(dispName,0,sizeof(devName)) ; 
						l1 =  strchr(rcd->lnk.dbname,'.')  +1	  ;
						l2 =  strchr(l1,'.')     ;			
						strncpy  (devName, l1  ,   l2 - l1) ;
						l1  = strrchr(rcd->lnk.dbname,'.');
						strcpy  (entryName, l1 +1  ) ;
						sprintf( dispName ,"%s.%s" , devName  ,  entryName );
					}else    /*dsp name file */
					{
						
						sprintf( dispName ,"%s" ,    rcd->lnk.act.fmt_def.dsp.name);
					}
					x =  MIN(rcd->bck.rect.x1,  rcd->bck.rect.x2) ; 
					y =  MIN(rcd->bck.rect.y1,  rcd->bck.rect.y2) ;
					w =  abs(rcd->bck.rect.x1  -  rcd->bck.rect.x2);
					h =  abs(rcd->bck.rect.y1  - rcd->bck.rect.y2 ) ;
					
					gdk_rgb( (int)(4*r),(int)(251*r),(int)(196 *r ),&bkColor )  ;
					gdk_gc_set_foreground (info[crt_n].gc, &bkColor);
					//gdk_gc_set_function(info[crt_n].gc, GDK_AND_INVERT);
					
					//printf ("#############%%%%%%%the rand number r = %f  rand = %5d", r,rand() ); 
					
					windk_draw_text(window ,font_set[0],info[crt_n].gc, XtoSX ( crt_n, x) + (int) (r * w), YtoSY ( crt_n, y) + (int) (r*h)  ,dispName,   strlen(dispName),crt_n );
					
					gdk_gc_set_function(info[crt_n].gc, GDK_COPY);
					
				}
				////////////////////////////////////////////////////////////////////////////////////////
				/*   printf("\nin Dynamic::gdbase[%d].buf[%d].lnk.dbname=%s,sizeof(name)=%d\n",crt_n,i,rcd->lnk.dbname,sizeof(rcd->lnk.dbname));
				*/     
				if ( rcd->lnk.dbname[0] && ReadEntryByNameStr ( rcd->lnk.dbname,&dms_cmm ) != -1 )
				{
					
					//hcl test	if(dms_cmm.fixed.iost.ctrlLock==YES||rcd->bck.tag == ITEM_TAG_ON)
					if(dms_cmm.fixed.iost.tagState>0)
					{    
						DrawItemTag ( crt_n, window , info[crt_n].gc, &rcd->bck ,dms_cmm.fixed.iost.tagState); 
						
					}
				}
			}
		}
	}	
}




/*	AcknDspAlmFlag is added by wdk 1999.5.3. This subroutine can acknoledge all alarm 
signal in the current display.
*/
void	AcknDspAlmFlag (GtkWidget*parent,int crt_n )
{
	int	i, ackn_st,  send_cnt = 0;
	GDBUFF  *layer;
	GBRCD   *rcd;
	DMS_COMMON	 dms_cmm;
	
	if(!CheckUserPrivilege(cur_usr_name, PRIV_ALM_ACK))
	{
		PopupMessageBox (parent, MSG_ALARM_ACK);
		return ;
	}
	
	layer   = &gdbase[crt_n];
	
	for ( i=0; i < layer->nextpos; i++ )
	{
		rcd	= &layer->buffer[i];
		if ( rcd->lnk.dyn.dyn_tag == DYN_TAG_ON
			||( rcd->lnk.cgrp.tag == DYN_CGRP_TAG_ON && rcd->lnk.cgrp.cgrp_id !=0) )
		{
			
			if ( ReadEntryByNameStr ( rcd->lnk.dbname, &dms_cmm ) == -1 )
			{
				printf ( "OIX: in AcknDspAlmFlag %s is not found in database.\n", rcd->lnk.dbname );
			}
			else
			{
				ackn_st = dms_cmm.fixed.iost.almNoAck;
				if ( ackn_st == UNACKN )
				{
					CmdBufInit ( crt_n );
					cmd_buf[crt_n].stn_id		= dms_cmm.point.stn_id;
					cmd_buf[crt_n].dev_id		= dms_cmm.point.dev_id;
					cmd_buf[crt_n].data_type	= dms_cmm.point.data_type;
					cmd_buf[crt_n].point_id	= dms_cmm.point.pt_id;
					cmd_buf[crt_n].type_id		= DPS_IOST_SET;  
					cmd_buf[crt_n].status.state = ACKN;
					cmd_buf[crt_n].status.opr  = IOST_SET_ALM_NOACK;
					
					ProgDoAckCelBrdcst ( crt_n );  
					
					send_cnt++;
					if ( send_cnt > MAX_NETCMD_SEG_NUM )
					{
						/*Sleep ( 1 )  */ MySleep(1);
						send_cnt = 0;
					}
				}
			}	
		}
	}
}


void	DrawSubDynamicRcd (int crt_n,GBRCD	*rcd, GdkGC	*gc, GdkDrawable	*drawable, int sub_win_n)
{
	
	BACKRCD	bk_rcd;
	int  Height,Width;
	
	strcpy(data_quality.man_set,"");
	strcpy(data_quality.chan_fail,"");
	strcpy(data_quality.ctrl_block,"");
	strcpy(data_quality.draw_info ,"") ;
	
	
	switch	( rcd->lnk.dyn.dyn_type )
	{
	case	DF_SYMBOL_GRP:           
		DrawDynamicSymbolGrp ( crt_n,drawable, gc, rcd );
		break;
		
	case	DF_ANA_STATUS:
		DrawDynamicAnaStatus ( crt_n,drawable, gc, rcd );
		break;
		
	case	DF_STRING_GRP:
		/*   DrawDynamicStringGrp ( crt_n, drawable, gc, rcd );  */
		
		Width = rcd->bck.rect.x2-rcd->bck.rect.x1 ;
		Height =rcd->bck.rect.y2 -rcd->bck.rect.y1 ;
		
		DrawDynamicStringGrp ( crt_n, drawable, gc, rcd );
		
		break;
		
	case	DF_BAR_CHART:
		DrawDynamicBarChart ( crt_n, drawable, gc, rcd ); 
		
		if ( rcd->lnk.cgrp.tag == DYN_CGRP_TAG_ON || rcd->lnk.cgrp.tag == DYN_CGRP_TAG_ADD_ON)
		{
			DrawDynamicColorGrp ( crt_n, drawable, gc, rcd );   
		}
		break;
		
    case    DF_DIAL_METER:                
        DrawDynamicDialChart (  crt_n,drawable, gc, rcd );
        break;  
		
	case    DF_CURVE_PT:  /*  added by chichi*/
		DrawDynamicCurvePt(crt_n, drawable, gc, rcd) ;
		break;
	case    DF_CUR_LOAD:   /*  added by chichi*/
		DrawDynamicCurveLoad(crt_n, drawable, gc, rcd) ;
		break;
		
	case	DF_REAL_CURVE:  /* added by chichi */
		DrawDynamicRealCurve (  crt_n,   drawable,  gc,  rcd ) ;
		break;
	case DF_TEMP_CHAR:
		DrawDynamicTempChara ( crt_n, drawable, gc, rcd );
		break;
	case	DF_INTEGER:
		DrawDynamicInteger ( crt_n, drawable, gc, rcd );
		break;
		
	case	DF_NUMERIC:
		DrawDynamicNumeric ( crt_n, drawable, gc, rcd );
		break;
		
	case	DF_RPT_DATE:
		DrawRptTime ( crt_n, drawable, gc, rcd );
		break;
		
	case	DF_ENTRY_NAME:
		DrawSubDynamicName ( crt_n, drawable, gc, rcd, sub_win_n );
		break;
		
	case	DF_SBO_STATUS:
		DrawDynamicSboStatus ( crt_n, drawable, gc, rcd );
		break;  
		
	case	DF_IMP_SET:
		DrawDynamicImpSet ( crt_n, drawable, gc, rcd );
		break;
		
	case	DF_LINE_PLOT:   /*	added by. wdk 99.5.16.	*/
		
		DrawDynLinePlot ( crt_n, drawable, gc, rcd );
		break;
		
	case	DF_PQ_CHART:
		DrawDynPQ_Chart ( crt_n, drawable, gc, rcd );
		break;
		
	case	DF_DYN_COLOR:
		if ( ( rcd->lnk.cgrp.tag == DYN_CGRP_TAG_ON || rcd->lnk.cgrp.tag == DYN_CGRP_TAG_ADD_ON) 
			&& rcd->lnk.cgrp.cgrp_id != 0)
		{
			DrawDynamicColorGrp ( crt_n, drawable, gc, rcd );
		}
		break;
		
	default:
		break;
	}
	switch  ( rcd->bck.draw_type )
	{
	case	DrwT_SYMBOL:            
		DrawSymbol ( crt_n, drawable,   gc,   &rcd->bck, FOREGROUND,TRUE, 0 );               
		break;
		
	default:
		if(rcd->lnk.dyn.dyn_type!= DF_BAR_CHART )
			DrawRcd ( crt_n, drawable,gc , &rcd->bck ,TRUE);              
		break;
	}
	
	if(  strcmp(data_quality.man_set,"")!=0 ||  strcmp(data_quality.chan_fail,"")!=0  ||
		strcmp(data_quality.draw_info,"")!=0 ||strcmp(data_quality.ctrl_block,"")!=0 )
	{            
		bk_rcd= rcd->bck;
        bk_rcd.draw_type	= DrwT_TEXT;
        bk_rcd.ftset_id         = 0;
        bk_rcd.bg	= ColorId3to4(SEA_BLUE4);
        bk_rcd.rect.y1	= MIN(bk_rcd.rect.y1,bk_rcd.rect.y2) -5;
        bk_rcd.rect.x1	= MAX(bk_rcd.rect.x2,bk_rcd.rect.x1) + 5;
		/*		if( strcmp(data_quality.man_set,"")!=0 )
		{
		bk_rcd.fg	= ColorId3to4(MANUAL_COLOR);			
		strcpy( bk_rcd.data.text.text, data_quality.man_set );
		}
		if( strcmp(data_quality.chan_fail,"")!=0 )
		{
		bk_rcd.fg	= ColorId3to4(FAULT_COLOR);			
		strcpy( bk_rcd.data.text.text, data_quality.chan_fail ); 
		}
		*/
		if( strcmp(data_quality.draw_info,"F")==0 ||  strcmp(data_quality.draw_info,"B")==0 )
			bk_rcd.fg	= ColorId3to4(FAULT_COLOR);
		else
			bk_rcd.fg	= ColorId3to4(MANUAL_COLOR);			
		
		
		strcpy( bk_rcd.data.text.text, data_quality.draw_info );
        DrawRcd ( crt_n,drawable,gc , &bk_rcd ,TRUE);
	}
}

void	DrawDynamicRcd (int crt_n,GBRCD	*rcd, GdkGC	*gc, GdkDrawable	*drawable )
{
	
	BACKRCD	bk_rcd;
	int  Height,Width;
	
    strcpy(data_quality.man_set,"");
    strcpy(data_quality.chan_fail,"");
    strcpy(data_quality.ctrl_block,"");
	strcpy(data_quality.draw_info,"");
	
	
	switch	( rcd->lnk.dyn.dyn_type )
	{
	case	DF_SYMBOL_GRP:
		DrawDynamicSymbolGrp ( crt_n,drawable, gc, rcd );
		break;
		
	case	DF_ANA_STATUS:
		DrawDynamicAnaStatus ( crt_n,drawable, gc, rcd );
		break;
		
	case	DF_STRING_GRP:
		
		Width = rcd->bck.rect.x2-rcd->bck.rect.x1 ;
		Height =rcd->bck.rect.y2 -rcd->bck.rect.y1 ;
		
		DrawDynamicStringGrp ( crt_n, drawable, gc, rcd );
		
		break;
		
	case	DF_BAR_CHART:
		DrawDynamicBarChart ( crt_n, drawable, gc, rcd ); 
		
		if ( rcd->lnk.cgrp.tag == DYN_CGRP_TAG_ON || rcd->lnk.cgrp.tag == DYN_CGRP_TAG_ADD_ON)
		{
			DrawDynamicColorGrp ( crt_n, drawable, gc, rcd );   
		}
		break;
		
    case    DF_DIAL_METER:
		
		DrawDynamicDialChart (  crt_n,drawable, gc, rcd );
		break; 
	case   DF_VIBRA_AREA:
		DrawDynamicVibraArea (  crt_n,drawable, gc, rcd );
		break;
		
        
		
		/*case	DF_HIST_BAR:
		DrawHistBarChart ( crt_n, drawable, gc, rcd );  
		if ( rcd->lnk.cgrp.tag == DYN_CGRP_TAG_ON || rcd->lnk.cgrp.tag == DYN_CGRP_TAG_ADD_ON)
		{
		DrawDynamicColorGrp ( crt_n,drawable, gc, rcd );
		}
		break;*/
	case    DF_CURVE_PT:  /*  added by chichi*/
		DrawDynamicCurvePt(crt_n, drawable, gc, rcd) ;
		break;
	case    DF_CUR_LOAD:   /*  added by chichi*/
		DrawDynamicCurveLoad(crt_n, drawable, gc, rcd) ;
		break; 
	case    DF_REAL_CURVE:   /*  added by chichi*/
		DrawDynamicRealCurve(crt_n, drawable, gc, rcd) ;
		break; 
	case DF_TEMP_CHAR:
		DrawDynamicTempChara ( crt_n, drawable, gc, rcd );
		break;
	case	DF_INTEGER:
		DrawDynamicInteger ( crt_n, drawable, gc, rcd );
		break;
		
	case	DF_NUMERIC:
		DrawDynamicNumeric ( crt_n, drawable, gc, rcd );
		break;
		
	case	DF_RPT_DATE:
		DrawRptTime ( crt_n, drawable, gc, rcd );
		break;
		
	case	DF_ENTRY_NAME:
		DrawDynamicName ( crt_n, drawable, gc, rcd );
		break;
		
	case	DF_SBO_STATUS:
		DrawDynamicSboStatus ( crt_n, drawable, gc, rcd );
		break;  
		
	case	DF_IMP_SET:
		DrawDynamicImpSet ( crt_n, drawable, gc, rcd );
		break;
		
		/*	case	DF_IMP_TIME:
		DrawDynamicImpTime ( crt_n, drawable, gc, rcd );
		break;
		*/
		
	case	DF_RPT_NUMERIC:
		DrawRptNumeric ( crt_n, drawable, gc, rcd );
		break;
		
	case	DF_LINE_PLOT:   /*	added by. wdk 99.5.16.	*/
		
		DrawDynLinePlot ( crt_n, drawable, gc, rcd );
		break;
		
	case	DF_PQ_CHART:
		DrawDynPQ_Chart ( crt_n, drawable, gc, rcd );
		break;
		
	case	DF_DYN_COLOR:
		if ( ( rcd->lnk.cgrp.tag == DYN_CGRP_TAG_ON || rcd->lnk.cgrp.tag == DYN_CGRP_TAG_ADD_ON) 
			&& rcd->lnk.cgrp.cgrp_id != 0)
		{
			DrawDynamicColorGrp ( crt_n, drawable, gc, rcd );
		}
		break;
	case    DF_CURVE_PT_INPUT:  /*  added by chichi*/
		
		break;         
	case    DF_MAN_SET_TIME:  /*  added by hcl*/
		DrawDynamicManSetTime ( crt_n, drawable, gc, rcd );
		break; 
		
	default:
		break;
	}
	
	
    
	switch  ( rcd->bck.draw_type )
	{
	case	DrwT_SYMBOL:		 
		DrawSymbol ( crt_n, drawable,   gc,   &rcd->bck, FOREGROUND,TRUE, 0 );               
		break;
		
		/*	removed by wdk 1998.12.28 
		case	DrwT_GIF:
		DrawGif ( crt_n,   drawable, info[crt_n].gc, &rcd->bck );    
		break;
		*/
	default:
		if(rcd->lnk.dyn.dyn_type!= DF_PQ_CHART&&rcd->lnk.dyn.dyn_type!= DF_BAR_CHART && rcd->lnk.dyn.dyn_type!= DF_VIBRA_AREA )
			DrawRcd ( crt_n, drawable,gc , &rcd->bck ,TRUE);              
		break;
	}
	
	if(  strcmp(data_quality.man_set,"")!=0 ||  strcmp(data_quality.chan_fail,"")!=0  
		||  strcmp(data_quality.draw_info,"")!=0       ||strcmp(data_quality.ctrl_block,"")!=0 )
	{            
		bk_rcd= rcd->bck;
		bk_rcd.draw_type	= DrwT_TEXT;
		bk_rcd.ftset_id         = 0 ;  
		bk_rcd.bg	= ColorId3to4(SEA_BLUE4);
		bk_rcd.rect.y1	= MIN(bk_rcd.rect.y1 ,bk_rcd.rect.y2)-5;
		bk_rcd.rect.x1	= MAX(bk_rcd.rect.x2 ,bk_rcd.rect.x1)+ 5;
		/*		if( strcmp(data_quality.man_set,"")!=0 )
		{
		bk_rcd.fg	= ColorId3to4(MANUAL_COLOR);			
		strcpy( bk_rcd.data.text.text, data_quality.man_set );
		}
		if( strcmp(data_quality.chan_fail,"")!=0 )
		{
		bk_rcd.fg	= ColorId3to4(FAULT_COLOR);			
		strcpy( bk_rcd.data.text.text, data_quality.chan_fail ); 
		}	
		*/		
		if( strcmp(data_quality.draw_info,"F")==0  || strcmp(data_quality.draw_info,"B")==0)
			bk_rcd.fg	= ColorId3to4(FAULT_COLOR);			
		else
			bk_rcd.fg	= ColorId3to4(MANUAL_COLOR);
		//added by yyp 2012.11.12
		strcpy(data_quality.draw_info,"");
        strcpy( bk_rcd.data.text.text, data_quality.draw_info );
		DrawRcd ( crt_n,drawable,gc , &bk_rcd ,TRUE);
	}
}



void	DrawDynamicColorGrp (int crt_n,GdkWindow *drawable, GdkGC *gc, GBRCD *rcd )
{
	DMS_COMMON	dms_cmm;
	short   rt_status;
	short   alm_status;
	
	if ( ReadEntryByNameStr ( rcd->lnk.dbname,  &dms_cmm ) == -1 )
	{
		printf ( "OIX: in DrawDynamicColorGrp  %s is not found in database.\n", rcd->lnk.dbname );
		return;
	}
	
	switch  ( dms_cmm.point.data_type )
	{	
	case	POL_TYPE:
	case	SOE_TYPE:
	case	SYSMSG_TYPE:
	case	OUTS_TYPE:
		rt_status = dms_cmm.var.ind.status.state;
		/* data_quality.man_set = dms_cmm.var.ind.status.man_set; 
		data_quality.chan_fail = dms_cmm.var.ind.status.chan_fail; */
		
		if(dms_cmm.var.ind.status.chan_fail)
			strcpy(data_quality.draw_info,"F");
		if(dms_cmm.var.ind.status.man_set)
			strcpy(data_quality.draw_info,"M");
		alm_status = dms_cmm.fixed.iost.almNoAck;
		rcd->bck.fg     = GetDynamicColor ( crt_n, rcd, rt_status, alm_status );
		break;
		
	default:
		break;
	}
}


int	GetDynamicSymColor (int crt_n, GBRCD	*rcd, short rt_status,short alm_status,short fault_status )
{
	int	fg;
	int	flash_index;
	if( fault_status == YES )
	{
		fault_sta = 1;
		switch( alm_status )
		{
		case	ACKN:
			printf ( " in ACKN \n: "); 
			fg =  cgdb[rcd->lnk.cgrp.cgrp_id].cgrp_cell[rt_status].c_normal; 
			break;
			
		case	UNACKN:
			flash_index = cgdb[rcd->lnk.cgrp.cgrp_id].cgrp_cell[rt_status].flash;
			fg = clr_pair[flash_index].clr[blink_flag[crt_n]];  
			break;
			
		default:
			fg =  cgdb[rcd->lnk.cgrp.cgrp_id].cgrp_cell[rt_status].c_normal;
			break;
		}
	}
	else 
		fg =  cgdb[rcd->lnk.cgrp.cgrp_id].cgrp_cell[rt_status].c_normal; 
	
	return	fg;
}

int	GetDynamicColor (int  crt_n, GBRCD	*rcd,short rt_status,short alm_status )
{
	int	fg;
	int	flash_index;
	
	switch( alm_status )
	{
	case	ACKN:
		fg =  cgdb[rcd->lnk.cgrp.cgrp_id].cgrp_cell[rt_status].c_normal; 
		/*   printf("in GetDynamicColor:rt_status=%d,fg=%d\n",rt_status,fg);*/
		break;
		
	case	UNACKN:
		flash_index = cgdb[rcd->lnk.cgrp.cgrp_id].cgrp_cell[rt_status].flash;
		fg = ColorId3to4(clr_pair[flash_index].clr[blink_flag[crt_n]]);  
		break;
		
	default:
		fg = WHITE;
		break;
	}
	return	fg;
}


int	GetCtrlPtDynamicColor (int  crt_n, GBRCD	*rcd )
{
	DMS_COMMON dms_cmm;
	int	fg;	
	char    type_id_str[20];
	char ana_sbo_pnt_name[MAX_CRT_NUMBER][SBO_NAME_LEN];
	int     IsCascade;
	
	IsCascade = IsLocalHostDispatch();
	
	
	if(rcd->lnk.act.fmt_def.prog.id==PrgN_C_GATE)
		strcpy(type_id_str,"C_GATE");
	else if(rcd->lnk.act.fmt_def.prog.id==PrgN_C_TRANS)
		strcpy(type_id_str,"C_TRANS");
	else if(rcd->lnk.act.fmt_def.prog.id==PrgN_C_APOWER)
		strcpy(type_id_str,"C_APOWER");
	else if(rcd->lnk.act.fmt_def.prog.id==PrgN_C_RPOWER)
		strcpy(type_id_str,"C_RPOWER"); 
	else if(rcd->lnk.act.fmt_def.prog.id==PrgN_OP_VALUE_SET)
		strcpy(type_id_str,"C_OP_VALUE_SET");
	else
		strcpy(type_id_str,"");
	
	sprintf( ana_sbo_pnt_name[crt_n],"%s.%s.%s",rcd->lnk.dbname,type_id_str,rcd->lnk.act.fmt_def.prog.status); /* Modified by zyp on 2006.5.10*/
	
	sbo_result[crt_n] = InterLock_Judge( ana_sbo_pnt_name[crt_n] );  
	if ( ReadEntryByNameStr ( rcd->lnk.dbname,&dms_cmm ) == -1 )
	{        		                  
		fg= ColorId3to4(GREEN1);
		return fg;
	}
	else
		GetEntryLongnameById( &dms_cmm.point, sbo_longname[crt_n] );
	ana_sbo_id[crt_n]=1;      
	
	if ( (sbo_result[crt_n] != 0   )&&GetCtrlRight(crt_n,dms_cmm,IsCascade)==TRUE)
		fg= ColorId3to4(L_BLUE1);
	else
		fg= ColorId3to4(GREEN1);
	
	return	fg;
}


/* Dial meter  add by chichi */
void    DrawDialMeter (int crt_n,GdkDrawable *drawable, GdkGC *gc, GBRCD *rcd , float  value )
{
    int j =0 ;
    gint xc, yc ;
    DIAL_METER  dial ;
    GdkRectangle  rc ;
    double s,c;
    
    double theta, last, increment;
    int upper, lower;
    int tick_length;
    int i, inc ;
    float y1, y2 ;
    GdkPoint points[6];
    rc.x = MIN(XtoSX(crt_n,rcd->bck.rect.x1),XtoSX(crt_n,rcd->bck.rect.x2));
    rc.y = MIN(YtoSY(crt_n,rcd->bck.rect.y1),YtoSY(crt_n,rcd->bck.rect.y2));
    rc.width  =  abs(XtoSX(crt_n,rcd->bck.rect.x2) - XtoSX(crt_n,rcd->bck.rect.x1));
    rc.height =  abs(YtoSY(crt_n,rcd->bck.rect.y2) - YtoSY(crt_n,rcd->bck.rect.y1));
    dial.radius = (int)( MIN (rc.width, rc.height) * 0.45);
    dial.pointer_width = dial.radius / 5;
    dial.angle =(float)( G_PI / 6 - j* G_PI / 60 );
    j ++ ;
    upper = 100 ;
    lower = 0 ;
    
    GetX_Coord_F ( rcd->lnk.dyn.fmt_def.bar_chart.hi_limit, 
		rcd->lnk.dyn.fmt_def.bar_chart.low_limit, 
		value, (float)(-G_PI / 6) ,(float) (G_PI *7 / 6), &y1, &y2 );
    
    
    dial.angle = y1 ;
    if (dial.angle < (float)(-G_PI / 6) ) 
		dial.angle = (float)(-G_PI /6) ;
	
    if ( dial.angle > (float)(G_PI *7/6)  )
		dial.angle = (float)(G_PI *7/6) ;   
	
    xc = rc.x + rc.width / 2;
    yc = rc.y + rc.height / 2;
    s = sin (dial.angle);
    c = cos (dial.angle);
    
    points[0].x =(int)( (xc + s*dial.pointer_width/2)*xscale[crt_n]);
    points[0].y =(int)( (yc + c*dial.pointer_width/2)*yscale[crt_n]);
    points[1].x =(int)( (xc + c*dial.radius)*xscale[crt_n]);
    points[1].y =(int)( (yc - s*dial.radius)*yscale[crt_n]);
    points[2].x =(int)( (xc - s*dial.pointer_width/2)*xscale[crt_n]);
    points[2].y =(int)( (yc - c*dial.pointer_width/2)*yscale[crt_n]);
    points[3].x =(int)( (xc - c*dial.radius/10)*xscale[crt_n]);
    points[3].y =(int)( (yc + s*dial.radius/10)*yscale[crt_n]);
    points[4].x =(int) ((points[0].x));
    points[4].y = (points[0].y);
    SetRcdColor ( crt_n,   gc,(BACKRCD*) rcd );
    gdk_draw_polygon (drawable,gc,TRUE, points, 5);
    
    
    
    /* Draw Ticks */
    increment = (100*G_PI) / (dial.radius*dial.radius);
	
    inc = (upper - lower);
	
    while (inc < 100) inc *= 10;
    while (inc >= 1000) inc /= 10;
    last = -1;
	
    for (i = 0; i <= inc; i++)
    {
        theta = ((float)i*G_PI / (18* inc/24.) - G_PI/6.);
		
        if ((theta - last) < (increment))
            continue;     
		last = theta;
		
		s = sin (theta);
		c = cos (theta);
		
		tick_length = ((i%(inc/10) == 0) ? dial.pointer_width : dial.pointer_width / 2);
		
		gdk_draw_line (drawable,gc, (int)((xc + c*(dial.radius - tick_length))*xscale[crt_n]),
			(int)((yc - s*(dial.radius - tick_length))*yscale[crt_n]),
			(int)((xc + c*dial.radius)*xscale[crt_n]),
			(int)((yc - s*dial.radius)*yscale[crt_n]));
    }
    
}
void	DrawDynamicDialChart (int crt_n,GdkDrawable	*drawable, GdkGC	*gc, GBRCD	*rcd )
{
	DMS_COMMON	dms_cmm;
	int		data_type;
	float	value;
	POINTER pt ;
	if ( ReadEntryByNameStr ( rcd->lnk.dbname, &dms_cmm ) == -1 )
	{
		printf ( "OIX:in DrawDynamicBarChart  %s is not found in database.\n", rcd->lnk.dbname );
		return;
	}
	if (GetPtIdByNameStr(rcd->lnk.dbname,&pt) == -1)
	{
		printf ( "OIX:in DrawDynamicBarChart  %s data type found in database.\n", rcd->lnk.dbname );
		return;
	}
	data_type = pt.data_type ;
	if ( data_type == ANA_TYPE || data_type == PARAM_TYPE)
		value	= dms_cmm.var.anlg.fValue;
	if ( data_type == CHARA_TYPE) 
		value = dms_cmm.var.chara.chara_data.max_value;	  
	/*     DrawDynBarRcd ( crt_n, drawable, rcd, value );   */
	DrawDialMeter ( crt_n, drawable,  gc,  rcd , value ) ;
}





void	DrawDynBarRcd (int crt_n, GdkDrawable *drawable, GBRCD	*rcd, float rt_value ); 
void	DrawDynVibraAreaRcd (int crt_n, GdkDrawable *drawable, GBRCD	*rcd, float upRange ,float lowRange );

void	DrawDynamicVibraArea (int crt_n,GdkWindow	*drawable, GdkGC	*gc, GBRCD	*rcd )
{
	DMS_COMMON	dms_cmm1 ;
	float	value1,value2;
	if (strcmp( rcd->lnk.dbname,"")!=0)  /*read  hi_limit*/
		if ( ReadEntryByNameStr ( rcd->lnk.dbname, &dms_cmm1 ) == -1 )
		{
			printf ( "OIX:in DrawDynamicBarChart  %s is not found in database.\n", rcd->lnk.dbname );
			return;
		}
        value1  =  dms_cmm1.var.anlg.fValue ;
        if (strcmp(rcd->lnk.dyn.fmt_def.vibra_area.dbname2,"") != 0 )  /*read  low_limit*/
            if ( ReadEntryByNameStr ( rcd->lnk.dyn.fmt_def.vibra_area.dbname2, &dms_cmm1 ) == -1 )
            {
				printf ( "OIX:in DrawDynamicBarChart  %s is not found in database.\n", rcd->lnk.dbname );
				return;
            }
			
			value2 =  dms_cmm1.var.anlg.fValue  ;
			
			if (value1 > value2)  
				DrawDynVibraAreaRcd ( crt_n, drawable, rcd, value1,value2 ); 
			
}


void	DrawDynamicBarChart (int crt_n,GdkWindow	*drawable, GdkGC	*gc, GBRCD	*rcd )
{
	DMS_COMMON	dms_cmm;
	int		data_type ,draw_type;
	float	value;
	BAR_GRAPH_LMT   bar_lmt;
	POINTER pt ;
	if ( ReadEntryByNameStr ( rcd->lnk.dbname, &dms_cmm ) == -1 )
	{
		printf ( "OIX:in DrawDynamicBarChart  %s is not found in database.\n", rcd->lnk.dbname );
		return;
	}
	if (GetPtIdByNameStr(rcd->lnk.dbname,&pt) == -1)
	{
		printf ( "OIX:in DrawDynamicBarChart  %s data type found in database.\n", rcd->lnk.dbname );
		return;
	}
	data_type = pt.data_type ;
	if ( data_type == ANA_TYPE || data_type == PARAM_TYPE)
	{ 
		value       =   dms_cmm.var.anlg.fValue;
		draw_type  = 0 ;
		
		bar_lmt.hi_limit1    =   dms_cmm.var.anlg.lmt_para.HI_ALARM;
		bar_lmt.low_limit1   =   dms_cmm.var.anlg.lmt_para.LO_ALARM;
		
		draw_type  =  ((value >  bar_lmt.hi_limit1)<<1 )| ((value < bar_lmt.low_limit1)) ;
		
		
		DrawDynBarRcd ( crt_n, drawable, rcd, dms_cmm.var.anlg.fValue );
		if (value >  bar_lmt.hi_limit1  ||   value < bar_lmt.low_limit1 )
			DrawDynBarLmtRcd( crt_n, drawable, rcd, &bar_lmt );
	}
	if ( data_type == CHARA_TYPE)
	{
		value = dms_cmm.var.chara.chara_data.max_value;
		DrawDynBarRcd ( crt_n, drawable, rcd, value ); 
	}	
}

/*
99.05.15.	created by wdk to get multiple analog value from database. It can 
be used by subsequent curve display routines.
*/

void	DrawDynLinePlot (int crt_n,GdkWindow	*drawable, GdkGC	*gc, GBRCD	*rcd )
{
	DMS_COMMON	dms_cmm;
	POINTER		ptr;
	float	value[MAX_CURVE_ENTRY];
	int	offset	= rcd->lnk.dyn.f_by_off;	/* specify the number of entries in this curve.	*/
	int	max_n, i, i1, i2, i0,entry_num;
	int	data_type;
	
	
	if ( ReadEntryByNameStr ( rcd->lnk.dbname, &dms_cmm ) == -1 )
	{
		printf ( "OIX in DrawDynLinePlot  %s is not found in database.\n", rcd->lnk.dbname );
		return;
	}
    data_type =  dms_cmm.point.data_type  ; 	
	if (data_type != ANA_TYPE && data_type != PARAM_TYPE)
	{
		printf ( "OIX in DrawDynLinePlot %s is not analog points.\n", rcd->lnk.dbname );
		return;
	}
	
	if ( offset <= 0 ) 
	{
		printf ( "in : offset < 0.\n" );
		return;
	}
	ptr	= dms_cmm.point;
	offset	= Min ( offset, MAX_CURVE_ENTRY );
	GetEntryNumById ( ptr.stn_id,ptr.dev_id,ptr.data_type,&entry_num); 
	max_n	= MIN ( ptr.pt_id+offset, entry_num);
	
	i0	= ptr.pt_id;
	i1	= i0;
	i2	= max_n;
	for ( i= i1; i < i2; i++ )
	{
		ptr.pt_id  = i;
		ReadEntryById ( &ptr, &dms_cmm );
		value[i-i0]	= dms_cmm.var.anlg.fValue;
	}
	
	for ( i= i2; i < rcd->lnk.dyn.f_by_off; i++ )
	{
		value[i-i0]	= 0.0;
	}
	
	DrawDynLine ( crt_n, rcd, value );
}

void	DrawDynPQ_Chart (int crt_n,GdkWindow	*drawable, GdkGC	*gc, GBRCD	*rcd )
{
	DMS_COMMON	dms_cmm;
	float	value[2];	/* value[0]: P; value[1]: Q.	*/
//	char	entry_name[DBNAME_LEN];
	char db_name[DBNAME_LEN];
	
	if ( ReadEntryByNameStr ( rcd->lnk.dbname, &dms_cmm ) == -1 )
	{
		printf ( "OIX in DrawDynPQ_Chart  %s is not found in database.\n", rcd->lnk.dbname );
		return;
	}
	
	if ( dms_cmm.point.data_type != ANA_TYPE && dms_cmm.point.data_type != PARAM_TYPE)
	{
		printf ( "OIX in DrawDynPQ_Chart %s is not analog points.\n", rcd->lnk.dbname );
		return;
	}
	value[0]	= dms_cmm.var.anlg.fValue;
	
// 	GetEntryNameById(&dms_cmm.point,entry_name);
// 	strncpy(db_name,rcd->lnk.dbname,strlen(rcd->lnk.dbname)-strlen(entry_name));
// 	strcat ( db_name, ".ANA.WG_MVAR" );
// 	/*data_quality.man_set = dms_cmm.var.ind.status.man_set; 
// 	data_quality.chan_fail = dms_cmm.var.ind.status.chan_fail; */
// 	if(!dms_cmm.fixed.iost.scanEnable)
// 		strcpy(data_quality.man_set,"M");
// 	if(dms_cmm.var.anlg.value.status.chan_fail)
// 		strcpy(data_quality.chan_fail,"F");
// 	if(dms_cmm.var.anlg.value.status.ctrl_block)
// 		strcpy(data_quality.ctrl_block,"B");
	
	if ( ReadEntryByNameStr ( rcd->lnk.dyn.fmt_def.curve.dbname2,&dms_cmm ) == -1 )
	{
		printf ( "OIX in DrawDynPQ_Chart  %s is not found in database.\n", db_name );
		return;
	}
	value[1]	= dms_cmm.var.anlg.fValue;
	DrawDynPQ_Dot ( crt_n,drawable, rcd, value );
}


/*void DrawHistBarChart (int crt_n, GdkWindow	*drawable, GdkGC	*gc, GBRCD	*rcd )
{
DMS_COMMON	dms_cmm;
ANLG_DATA	*anlg_data;
float	value;
short	req_type;
short	time_id;
char	db_name[DBNAME_LEN];

  int	rpt_data_type  ;	
  RPT_DATA	rpt_data;	
  int	rpt_return;
  
	if ( ReadEntryByNameStr ( rcd->lnk.dbname,&dms_cmm ) == -1 )
	{
	printf ( "OIX:in DrawDynamicBarChart  %s is not found in database.\n", rcd->lnk.dbname );
	return;
	}
	
	  strcpy( db_name, rcd->lnk.dbname );
	  if ( dms_cmm.point.data_type== ANA_TYPE || dms_cmm.point.data_type == PARAM_TYPE )
	  {
	  time_id = rcd->lnk.dyn.fmt_def.hist_bar.hour_id;
	  
		if( ((strcmp(&db_name[strlen(db_name)-strlen(CALC_ACC)], CALC_ACC) == 0) && (time_id == 23))
		      || ((strcmp(&db_name[strlen(db_name)-strlen(CALC_ACC1)], CALC_ACC1) == 0) && (time_id == 23)) )
			  {
			  time_id= hour;
			  }
			  
				req_type = REQ_HOURLY;
                GetCurDateString( crt_n);  
				rpt_return = ReadRptEntryByName ( rcd->lnk.dbname, req_type, time_id, 
				NULL, select_rpt_hist_name[crt_n], &rpt_data_type , &rpt_data);
				if (rpt_return == -1)
				{
				printf ( "OIX: DrawHistBarChart: %s is not found in rpt table.\n", rcd->lnk.dbname );
				sprintf ( rcd->bck.data.text.text,"");
				return;
				}
				
				  value = rpt_data.value.float_data;
				  
					#ifdef	DEBUG
					printf ( " in DrawHistBarChart: value=%f, time_id=%d\n", value, time_id );
					#endif
					DrawDynBarRcd ( crt_n, drawable, rcd, value ); 
					}
}*/


void	DrawDynamicStringGrp (int crt_n, GdkWindow	*drawable, GdkGC	*gc, GBRCD   *rcd )
{
	DMS_COMMON	dms_cmm;
	short		status;	
	extern BOOL oix_use_msg_def_str_flag;
	/*	
	printf("in DrawDynamicStringGrp: dbname=%s\n",rcd->lnk.dbname);
	*/	
	if ( ReadEntryByNameStr ( rcd->lnk.dbname,&dms_cmm ) == -1 )
	{
		printf ( "OIX: in DrawDynamicStringGrp  %s is not found in database.\n", rcd->lnk.dbname );
		return;
	}
	
	if ( dms_cmm.point.data_type == POL_TYPE || dms_cmm.point.data_type == SOE_TYPE 
		|| dms_cmm.point.data_type == SYSMSG_TYPE ||dms_cmm.point.data_type ==OUTS_TYPE)
	{
		status	  = dms_cmm.var.ind.status.state;
		
        if(dms_cmm.var.ind.status.chan_fail)                    
            strcpy(data_quality.draw_info,"F");  
		
		if(dms_cmm.var.ind.status.man_set)                    
            strcpy(data_quality.draw_info,"M");
		
		//画面动态字符串从数据库中读取，或者从字符串组中读取
		if(oix_use_msg_def_str_flag)
		{
			char	str[256];
			GetIndPtAlmMsgStrById(&dms_cmm.point, dms_cmm.var.ind.status.state, str);
			strncpy ( rcd->bck.data.text.text, str, sizeof(rcd->bck.data.text.text)-1);
		}
		else
			strcpy ( rcd->bck.data.text.text, strgdb[rcd->lnk.dyn.fmt_def.str.str_grp].str[status]);
		
		if ( rcd->lnk.cgrp.tag == DYN_CGRP_TAG_ON || rcd->lnk.cgrp.tag == DYN_CGRP_TAG_ADD_ON)
		{
			blink_status[crt_n] = dms_cmm.fixed.iost.almNoAck;
			rcd->bck.fg = GetDynamicColor ( crt_n, rcd, status, blink_status[crt_n] );
		}
	}
	DrawTag (   crt_n,  drawable,  gc,  rcd   );
}

void	DrawDynamicInteger (int crt_n,GdkWindow	*drawable, GdkGC	*gc, GBRCD	*rcd )
{
	DMS_COMMON	dms_cmm;
	short		lmt_type;
	float		rt_value;
	int		int_value;
	
	if ( ReadEntryByNameStr ( rcd->lnk.dbname,&dms_cmm ) == -1 )
	{		
		printf ( "OIX: DrawDynamicInteger: %s is not found in database.\n", rcd->lnk.dbname );
		return;
	}
	
	if ( dms_cmm.point.data_type!= ANA_TYPE && dms_cmm.point.data_type != PARAM_TYPE
		&&dms_cmm.point.data_type!=OUTA_TYPE)
	{
		printf ( "OIX: %s is not an analog/numeric/param point.\n", rcd->lnk.dbname );
		return;
	}
	else
	{		
		rt_value	= dms_cmm.var.anlg.fValue;
		int_value = ( int)rt_value;
		
		sprintf ( rcd->bck.data.text.text,"%*d\0", strlen( rcd->bck.data.text.text),int_value );
		
		if ( rcd->lnk.cgrp.tag == DYN_CGRP_TAG_ON || rcd->lnk.cgrp.tag == DYN_CGRP_TAG_ADD_ON)
		{
			blink_status[crt_n]	=  dms_cmm.fixed.iost.almNoAck;
			lmt_type		= dms_cmm.var.anlg.value.status.lmt_state;
			rcd->bck.fg		= GetDynamicColor ( crt_n, rcd, lmt_type, blink_status[crt_n] );
		}
	}
	DrawTag (   crt_n,  drawable,  gc,  rcd   );
}

void	DrawDynamicManSetTime (int crt_n,GdkWindow	*drawable, GdkGC	*gc, GBRCD	*rcd )
{
	DMS_COMMON	dms_cmm;
	//	short		lmt_type;
	//	float		rt_value;
	//	int		int_value;
	struct tm newTime;
	//	time_t nTime;
	
	int time0=0;
	if ( ReadEntryByNameStr ( rcd->lnk.dbname,&dms_cmm ) == -1 )
	{		
		printf ( "OIX: DrawDynamicInteger: %s is not found in database.\n", rcd->lnk.dbname );
		return;
	}
	
	
	
	if ( dms_cmm.point.data_type!= TIME_TYPE )
	{
		printf ( "OIX: %s is not an mansettime point.\n", rcd->lnk.dbname );
		return;
	}
	if (dms_cmm.var.time.value_sec<=0)
	{
		printf ( "OIX: %s is mansettime=0 \n", rcd->lnk.dbname );
		return;
	}
	else
	{
		
		
		newTime = *localtime(&(dms_cmm.var.time.value_sec));
		sprintf(rcd->bck.data.text.text,"%04d/%02d/%02d %02d:%02d:%02d ",newTime.tm_year+1900,
			newTime.tm_mon+1, newTime.tm_mday, newTime.tm_hour,
			newTime.tm_min, newTime.tm_sec);
		
	}
}
/*added by chi.hl  2007.08.16*/
int    GetRealCurveIDByName (    char* dbName )
{
	POINTER  pt ; 
	int i ; 
	//GetPtIdByNameStr( char *ptNameStr, POINTER *point );
	if (GetPtIdByNameStr( dbName ,&pt  ) == -1 ) 
	{		
		printf ( "OIX: GetRealCurveIDByName: %s is not found in database.\n", dbName );
		return -1;
	}
	
	for ( i  = 0 ; i < RealCCurvePt.curve_num ;i++  )
	{
		//if (memcmp((void*) &RealCCurvePt.buffer[i] , (void*)&pt ,sizeof(POINTER)  ) ==0 )
		if (RealCCurvePt.buffer[i].stn_id == pt.stn_id && 
			RealCCurvePt.buffer[i].dev_id == pt.dev_id && 
			RealCCurvePt.buffer[i].data_type == pt.data_type && 
			RealCCurvePt.buffer[i].pt_id == pt.pt_id )
			return i ; 
	}
	return  -1 ;
}
/********************************real curve RCD added by chi09.06.22******************************************************/

char   *timeLable[10] = {"0:00","3:00","6:00","9:00","12:00","15:00","18:00","21:00","24:00"};
void	DrawDynamicRealCurve (int crt_n, GdkDrawable	*drawable, GdkGC	*gc, GBRCD	*rcd )
{
	//	DMS_COMMON	dms_cmm;
	int  point_id  ,i ,j ,xScale,cur_id ,nextpos;   	
	float hi_limit,low_limit ,last_value;
	float xStep ;int xAxis;float  rangeStep ;
	int TimeLength;
    GdkRectangle  rc ;
    char Buffer[30];
	GdkPoint  pts1[MAX_CANVAS_PT*2] ;
	float PtValues[MAX_CANVAS_PT] ; 
    char  timeScale[128] ; 
	
    memset( pts1,0,MAX_CANVAS_PT) ;
    memset(PtValues , 0 , MAX_CANVAS_PT) ; 
    rc.x = MIN(XtoSX(crt_n,rcd->bck.rect.x1),XtoSX(crt_n,rcd->bck.rect.x2));
    rc.y = MIN(YtoSY(crt_n,rcd->bck.rect.y1),YtoSY(crt_n,rcd->bck.rect.y2));
    rc.width  =  abs(XtoSX(crt_n,rcd->bck.rect.x2) - XtoSX(crt_n,rcd->bck.rect.x1));
    rc.height =  abs(YtoSY(crt_n,rcd->bck.rect.y2) - YtoSY(crt_n,rcd->bck.rect.y1));
	
    TimeLength =rcd->lnk.dyn.fmt_def.real_curve.TimeLength  ;
    point_id = rcd->lnk.dyn.fmt_def.real_curve.point_id ;
	hi_limit = rcd->lnk.dyn.fmt_def.real_curve.hi_limit ;
	low_limit = rcd->lnk.dyn.fmt_def.real_curve .low_limit ;
	
	gdk_gc_set_line_attributes(gc,0,GDK_LINE_SOLID,GDK_CAP_NOT_LAST,GDK_JOIN_ROUND);	
	/*draw x scale!!!*/
	xStep = (float )(rc.width /10.00) ;   /*24*/
	xAxis = (int)(rc.y +  rc.height) ;
	
	for (i = 0; i <= 10 ;i++)
	{
		
		if (!(i%2 )  )
		{
			sprintf (timeScale,"%d",(int) ((TimeLength/10)*i) ); 
			gdk_gc_set_foreground ( gc, &pixels[ crt_n][GRN_BLUE8]) ;               	 
			gdk_draw_text( drawable, font_set[0], gc,  (int)((rc.x + i * xStep-10)*xscale[crt_n]),(int) ((xAxis +20)*yscale[crt_n]), 
				timeScale, strlen(timeScale));
		}
		
		gdk_gc_set_foreground ( gc, &pixels[ crt_n][GREEN4]) ; /* CHOCLT1 */     
		gdk_draw_line (  drawable, gc, (int)((rc.x + i * xStep)*xscale[crt_n]) , (int)(xAxis*yscale[crt_n]) ,(int)((rc.x + i * xStep)*xscale[crt_n]), 
			(int)((xAxis -  ((i%3)?10:15))*yscale[crt_n])  );
		
	}
	/*draw y scale line !!!*/
	xStep = (float )(rc.height /4.00) ;
	rangeStep =(float) ((hi_limit - low_limit)/4.00) ;
	if (rcd->lnk.dyn.tagi_loc  ==  0)  /*left */
		xScale  = rc.x - 80 ;
	else
		xScale  = rc.x + rc.width  -10 ; 
	for (i = 0; i <= 4 ;i++)
	{
		sprintf (Buffer,"%10.1f\0" , hi_limit - i *rangeStep  );
		/*   gdk_gc_set_foreground ( gc, &pixels[ crt_n][GRN_BLUE8]) ;   */
		SetRcdColor ( crt_n,   gc, (BACKRCD *)rcd );
		gdk_draw_text( drawable, font_set[0], gc, (int)(xScale*xscale[crt_n]) ,  (int)((rc.y + i*xStep-10)*yscale[crt_n]), 
			Buffer, strlen(Buffer));
		
		/*gdk_gc_set_foreground ( gc, &pixels[ crt_n][GREEN4]) ;  */
		gdk_draw_line (  drawable, gc, (int)(rc.x*xscale[crt_n])   ,(int) ((rc.y + i*xStep)*yscale[crt_n] ),(int)((rc.x + rc.width)*xscale[crt_n]), (int)((rc.y + i*xStep)*yscale[crt_n]));      
	}
	
	/****************** Draw  Real curve  line ****************/
	if (rcd->lnk.dbname[0] != 0  )
	{
		cur_id = 0 ;
		cur_id = GetRealCurveIDByName ( rcd->lnk.dbname     );
		if (cur_id ==-1) {
			printf ("$$$$$$   WARN :::: OIX:::: CUR_ID = -1 !!!!!!!\n" );
			return ;
		}
		CurveSampleFun(0 , TimeLength  ,   cur_id,   PtValues ,&nextpos ); 
		xStep = (float )rc.width /MAX_CANVAS_PT;
		for (i=0 ,j = 0,last_value= 0; i< MIN(MAX_CANVAS_PT ,nextpos ); j++ ,i++)
		{
			pts1[j].x = (int)(rc.x + i *xStep) ;
			
			if (PtValues[i]<=low_limit )
			{   pts1[j].y  =  xAxis ;
			last_value =  low_limit ;
			} 
			else if(PtValues[i]> hi_limit)
			{  pts1[j].y  =  xAxis -  rc.height  ;
			last_value =  hi_limit ;
			}
			else
			{
				if (last_value != PtValues[i])   /* insert new points in inflexion */
				{
					pts1[j].y  = (int)(xAxis - (last_value- low_limit) /(hi_limit - low_limit)*(rc.height)); 
					pts1[j].y = (int)(pts1[j].y  *yscale[crt_n] ) ;
					pts1[j].x = (int)(pts1[j].x  *xscale[crt_n] ) ;
					j ++ ;
					pts1[j].x = (int)(rc.x + i *xStep) ; 
				}
				
				pts1[j].y =   (int)(xAxis - (PtValues[i]- low_limit) /(hi_limit - low_limit)*(rc.height));
				
				last_value  =  PtValues[i] ;   
			}
			pts1[j].x = (int)(pts1[j].x  *xscale[crt_n] ) ;
			pts1[j].y = (int)(pts1[j].y  *yscale[crt_n] ) ;
			
			
		} 	
		
		printf ("@@@@@@@@@@J = %d    \n" , j )   ;    
		gdk_gc_set_line_attributes( gc,  0, GDK_LINE_SOLID,
			GDK_LINE_ON_OFF_DASH,
			GDK_JOIN_MITER);
		gdk_gc_set_function(gc, GDK_COPY);
		gdk_gc_set_foreground ( gc, &pixels[ crt_n][RED1]) ;   /*L_BLUE1]*/
		gdk_draw_lines(drawable,gc,pts1  , j ); 
		//  my_draw_lines( drawable,  gc,&pts1 ,	j , 0 , 10000   ) ;
		return ;
		for (j= 0 ; j< 2*MAX_CANVAS_PT ; j++)
		{
			printf (" x=%d ; pts1[%d].y = %d" ,pts1[j].x,  j ,  pts1[j].y) ;
		}
	} // for 
		  
}




/************************************************************************************************************/
void	DrawDynamicCurveLoad (int crt_n, GdkDrawable	*drawable, GdkGC	*gc, GBRCD	*rcd )
{
	DMS_COMMON	dms_cmm;
	int  point_id  ,i ,j ,xScale,cur_id ,nextpos;   	
	float hi_limit,low_limit ,last_value;
	float xStep ;int xAxis;float  rangeStep ;
    GdkRectangle  rc ;
    char Buffer[30];
    GdkPoint  pts[MAX_CURVE_SEG_NUM *2] ;
	GdkPoint  pts1[MAX_CANVAS_PT*2] ;
	float PtValues[MAX_CANVAS_PT] ; 
	GdkColor  bg ,fg;
	if (GetColor(rcd->bck.bg  , &bg )== -1) return ;
	if (GetColor(rcd->bck.fg , &fg )== -1) return ;	
	
	
    memset( pts1,0,MAX_CANVAS_PT) ;
    memset(PtValues , 0 , MAX_CANVAS_PT) ; 
    rc.x = MIN(XtoSX(crt_n,rcd->bck.rect.x1),XtoSX(crt_n,rcd->bck.rect.x2));
    rc.y = MIN(YtoSY(crt_n,rcd->bck.rect.y1),YtoSY(crt_n,rcd->bck.rect.y2));
    rc.width  =  abs(XtoSX(crt_n,rcd->bck.rect.x2) - XtoSX(crt_n,rcd->bck.rect.x1));
    rc.height =  abs(YtoSY(crt_n,rcd->bck.rect.y2) - YtoSY(crt_n,rcd->bck.rect.y1));
	
    
    point_id  = rcd->lnk.dyn.fmt_def.load_curve.point_id ;
	hi_limit  = rcd->lnk.dyn.fmt_def.load_curve.hi_limit;
	low_limit = rcd->lnk.dyn.fmt_def.load_curve.low_limit;
	
	gdk_gc_set_line_attributes(gc,0,GDK_LINE_SOLID,GDK_CAP_NOT_LAST,GDK_JOIN_ROUND);	
	/*draw x scale!!!*/
	xStep = (float )(rc.width /24.00) ; 
	xAxis = (int)(rc.y +  rc.height) ;
	
	for (i = 0; i <= 24 ;i++)
	{
		if (!(i%3 )  )
		{
			gdk_gc_set_foreground ( gc, &fg/*pixels[ crt_n][GRN_BLUE8]*/) ;               	 
			gdk_draw_text( drawable, font_set[0], gc,  (int)((rc.x + i * xStep-10)*xscale[crt_n]),(int) ((xAxis +20)*yscale[crt_n]), 
				timeLable[i/3], strlen(timeLable[i/3]));
		}
		
		gdk_gc_set_foreground ( gc, &fg/*pixels[ crt_n][GREEN4]*/) ; /* CHOCLT1 */     
		gdk_draw_line (  drawable, gc, (int)((rc.x + i * xStep)*xscale[crt_n]) , (int)(xAxis*yscale[crt_n]) ,(int)((rc.x + i * xStep)*xscale[crt_n]), 
			(int)((xAxis -  ((i%3)?10:15))*yscale[crt_n])  );
		
	}
	/*draw y scale line !!!*/
	xStep = (float )(rc.height /4.00) ;
	rangeStep =(float) ((hi_limit - low_limit)/4.00) ;
	if (rcd->lnk.dyn.tagi_loc  ==  0)  /*left */
		xScale  = rc.x - 120 ;
	else
		xScale  = rc.x + rc.width  -10 ; 
	for (i = 0; i <= 4 ;i++)
	{
		sprintf (Buffer,"%10.1f\0" , hi_limit - i *rangeStep  );
		gdk_gc_set_foreground ( gc, &fg/*pixels[ crt_n][GRN_BLUE8]*/) ;   
		//SetRcdColor ( crt_n,   gc, (BACKRCD *)rcd );
		gdk_draw_text( drawable, font_set[0], gc, (int)(xScale*xscale[crt_n]) ,  (int)((rc.y + i*xStep-10)*yscale[crt_n]), 
			Buffer, strlen(Buffer));
		
		/*gdk_gc_set_foreground ( gc, &pixels[ crt_n][GREEN4]) ;  */
		gdk_draw_line (  drawable, gc, (int)(rc.x*xscale[crt_n])   ,(int) ((rc.y + i*xStep)*yscale[crt_n] ),(int)((rc.x + rc.width)*xscale[crt_n]), (int)((rc.y + i*xStep)*yscale[crt_n]));      
	}
    
	/*********** Draw  Plan Curve  line ****************/
	if (rcd->lnk.dbname[0] != 0 )
    {
		if ( ReadEntryByNameStr ( rcd->lnk.dbname, &dms_cmm ) == -1 )return;
		
		if ( dms_cmm.point.data_type != CURVE_TYPE )
		{
			printf ( "OIX: %s data type is not an curve point!!\n\n", rcd->lnk.dbname );
			return;
		}
		
		
		if ( dms_cmm.var.curve.seg_num > MAX_CURVE_SEG_NUM)
		{
			printf ( "OIX: %s SegNum more than  MAX curve number  !!\n\n", rcd->lnk.dbname  );
			return;
		} 
		xStep = (float )rc.width /dms_cmm.var.curve.seg_num ;
		last_value  =  0 ;
		for (i=0 ,j = 0,last_value= 0; i<= dms_cmm.var.curve.seg_num  ; j++ ,i++)
		{
			pts[j].x =(int) (rc.x + i *xStep) ;
			
			if (dms_cmm.var.curve.data.value[i]<low_limit )
			{   pts[j].y  =  xAxis ;
			last_value =  low_limit ;
			} 
			else if(dms_cmm.var.curve.data.value[i]> hi_limit)
			{  pts[j].y  =  xAxis -  rc.height  ;
			last_value =  hi_limit ;
			}
			else
			{
				if (last_value != dms_cmm.var.curve.data.value[i])   /* insert new points in inflexion */
				{
					pts[j].y  = (int)(xAxis - (last_value- low_limit) /(hi_limit - low_limit)*(rc.height)); 
					pts[j].x = (int)(pts[j].x *xscale[crt_n]) ;
					pts[j].y = (int)(pts[j].y *yscale[crt_n]) ;
					j ++ ;
					pts[j].x =(int) (rc.x + i *xStep) ; 
				}
				
				pts[j].y =   (int)(xAxis - (dms_cmm.var.curve.data.value[i]- low_limit) /(hi_limit - low_limit)*(rc.height));
				
				last_value  =  dms_cmm.var.curve.data.value[i] ;   
			}
			pts[j].x = (int)(pts[j].x *xscale[crt_n]) ;
			pts[j].y = (int)(pts[j].y *yscale[crt_n]) ;
			
		}  
		/*gdk_gc_set_foreground ( gc, &pixels[ crt_n][GRN_BLUE8]) ;   GDK_LINE_SOLID*/
		gdk_gc_set_line_attributes( gc,  0, GDK_LINE_ON_OFF_DASH,
			GDK_LINE_ON_OFF_DASH,
			GDK_JOIN_MITER);
		//SetRcdColor ( crt_n,   gc, (BACKRCD *)rcd );
		gdk_gc_set_foreground ( gc, &bg/*pixels[ crt_n][GRN_BLUE8]*/) ;
		gdk_draw_lines(drawable,gc,pts , j); 
		
		
	}  // end for              
	
		  /*********** Draw  Real curve  line ****************/
	if (rcd->lnk.dyn.fmt_def.load_curve.LoadDBName[0] != 0 )
	{
		cur_id = 0 ;
		cur_id = GetRealCurveIDByName ( rcd->lnk.dyn.fmt_def.load_curve.LoadDBName     );
		if (cur_id ==-1) {
			printf ("$$$$$$   WARN :::: OIX:::: CUR_ID = -1 !!!!!!!\n" );
			return ;}
		CurveSampleFun(0 , 3600 *24  ,   cur_id,   PtValues ,&nextpos ); 
		xStep = (float )rc.width /MAX_CANVAS_PT ;
		for (i=0 ,j = 0,last_value =0; i< MIN(MAX_CANVAS_PT ,nextpos ); j++ ,i++)
		{
			pts1[j].x = (int)(rc.x + i *xStep) ;
			
			if (PtValues[i]<=low_limit )
			{   pts1[j].y  =  xAxis ;
			last_value =  low_limit ;
			} 
			else if(PtValues[i]> hi_limit)
			{  pts1[j].y  =  xAxis -  rc.height  ;
			last_value =  hi_limit ;
			}
			else
			{
				if (last_value != PtValues[i])   /* insert new points in inflexion */
				{
					pts1[j].y  = (int)(xAxis - (last_value- low_limit) /(hi_limit - low_limit)*(rc.height)); 
					pts1[j].x = (int)(pts1[j].x *xscale[crt_n]) ;
					pts1[j].y = (int)(pts1[j].y *yscale[crt_n]) ;
					j ++ ;
					pts1[j].x = (int)(rc.x + i *xStep) ; 
				}
				
				pts1[j].y =   (int)(xAxis - (PtValues[i]- low_limit) /(hi_limit - low_limit)*(rc.height));
				
				last_value  =  PtValues[i] ;   
			}
			pts1[j].x = (int)(pts1[j].x *xscale[crt_n]) ;
			pts1[j].y = (int)(pts1[j].y *yscale[crt_n]) ;
			
			
		} 	
		
		printf ("@@@@@@@@@@J = %d    \n" , j )   ;    
		gdk_gc_set_line_attributes( gc,  0, GDK_LINE_SOLID,
			GDK_LINE_ON_OFF_DASH,
			GDK_JOIN_MITER);
		gdk_gc_set_foreground ( gc, &fg/*pixels[ crt_n][L_BLUE1]*/) ; 
		/*
		for (j= 0 ; j< 500 ; j++)
		{
		printf (" x=%d ; pts1[%d].y = %d" ,pts1[j].x,  j ,  pts1[j].y) ;
	}*/
		
		
		gdk_draw_lines(drawable,gc,pts1  , j ); 
		//  my_draw_lines( drawable,  gc,&pts1 ,	j , 0 , 10000   ) ;
		return ;
		for (j= 0 ; j< 2*MAX_CANVAS_PT ; j++)
		{
			printf (" x=%d ; pts1[%d].y = %d" ,pts1[j].x,  j ,  pts1[j].y) ;
		}
	} // for 
		  
}


/* draw curve point added by chichi  */
void	DrawDynamicCurvePt (int crt_n, GdkWindow	*drawable, GdkGC	*gc, GBRCD	*rcd )
{
	DMS_COMMON	dms_cmm;
	int  point_id ;   	
	float		rt_value;
	if ( ReadEntryByNameStr ( rcd->lnk.dbname, &dms_cmm ) == -1 )return;
	
	if ( dms_cmm.point.data_type != CURVE_TYPE )
	{
		printf ( "OIX: %s data type is not an curve point!!\n\n", rcd->lnk.dbname );
		return;
	}
	
	
	point_id = rcd->lnk.dyn.fmt_def.curve_point.point_id ;
	if (point_id >dms_cmm.var.curve.seg_num || point_id > MAX_CURVE_SEG_NUM)
	{
		printf ( "OIX: %s Point id more than  seg_num [%d]!!\n\n", rcd->lnk.dbname,dms_cmm.var.curve.seg_num );
		return;
	} 
	rt_value	= dms_cmm.var.curve.data.value[point_id]; /* "%*.2f\0"  */
	/*       sprintf (FormatS,"%%*.%df\0"  ,rcd->lnk.dyn.fmt_def.rpt_num.dec_pt); */          
	sprintf ( rcd->bck.data.text.text,"%10.0f\0",    rt_value );
	
}

/* draw character of temperature added by chichi  */
void	DrawDynamicTempChara (int crt_n, GdkWindow	*drawable, GdkGC	*gc, GBRCD	*rcd )
{
	DMS_COMMON	dms_cmm;
	int   kind_id;   	
	float		rt_value;
	char            FormatS[20];
	if ( ReadEntryByNameStr ( rcd->lnk.dbname, &dms_cmm ) == -1 )return;
	
	if ( dms_cmm.point.data_type != CHARA_TYPE )
	{
		printf ( "OIX: %s data type is not an charater!!\n\n", rcd->lnk.dbname );
		return;
	}
	sprintf (FormatS,"%%10.%df\0"  ,rcd->lnk.dyn.fmt_def.temp_chara.length);
	
	
	kind_id = rcd->lnk.dyn.fmt_def.temp_chara.temp_chara_id ;
	switch (kind_id)
	{   
	case 0:    /* NULL  */
		
		strcpy ( rcd->bck.data.text.text,"" );
		break;
	case 1:    /* MAX VALUE  */
		rt_value =  dms_cmm.var.chara.chara_data.max_value;
		/* sprintf ( rcd->bck.data.text.text,(char*)FormatS,    rt_value );*/
		sprintf ( rcd->bck.data.text.text,"%.2f",    rt_value );
		break;
	case 2:    /* Min VALUE  */
		rt_value =  dms_cmm.var.chara.chara_data.min_value;
		/*sprintf ( rcd->bck.data.text.text,(char*)FormatS,    rt_value );*/
		sprintf ( rcd->bck.data.text.text,"%.2f",    rt_value );
		break; 
	case 3:    /* Std VALUE  */
		rt_value =  dms_cmm.var.chara.chara_data.std_value;
		/* sprintf ( rcd->bck.data.text.text,(char*)FormatS,    rt_value );*/
		sprintf ( rcd->bck.data.text.text,"%.2f",    rt_value );
		break; 
	case 4:    /* Average VALUE  */
		rt_value =  dms_cmm.var.chara.chara_data.avg_value;
		/* sprintf ( rcd->bck.data.text.text,(char*)FormatS,    rt_value );*/
		sprintf ( rcd->bck.data.text.text,"%.2f",    rt_value );
		break;
	case 5:    /* Max rate VALUE  */
		rt_value =  dms_cmm.var.chara.chara_data.max_rate;
		/*  sprintf ( rcd->bck.data.text.text,(char*)FormatS,    rt_value );*/
		sprintf ( rcd->bck.data.text.text,"%.2f",    rt_value );
		break;
	case 6:    /* Min rate VALUE  */
		rt_value =  dms_cmm.var.chara.chara_data.min_rate;
		/*sprintf ( rcd->bck.data.text.text,(char*)FormatS,    rt_value );*/
		sprintf ( rcd->bck.data.text.text,"%.2f",    rt_value );
		break;
	case 7:    /* Max Pt Num    */
		rt_value =  dms_cmm.var.chara.chara_data.max_no/100.; //wzg
		sprintf ( rcd->bck.data.text.text,"%.2f",    rt_value );
		break;
	case 8:    /* Min Pt Num    */
		rt_value =  dms_cmm.var.chara.chara_data.min_no/100.;
		sprintf ( rcd->bck.data.text.text,"%.2f",    rt_value );
		break;
	case 9:    /* Max change Pt   */
		rt_value =  dms_cmm.var.chara.chara_data.max_rate_no/100.;
		sprintf ( rcd->bck.data.text.text,"%.2f",    rt_value );
		break;
	case 10:    /* Min change Pt   */
		rt_value =  dms_cmm.var.chara.chara_data.min_rate_no/100.;
		sprintf ( rcd->bck.data.text.text,"%.2f",    rt_value );
		break;
	}
	
	
}


void	DrawDynamicNumeric (int crt_n, GdkWindow	*drawable, GdkGC	*gc, GBRCD	*rcd )
{
	DMS_COMMON	dms_cmm;
	short		lmt_type;	
	float		rt_value;
    char            FormatS[20];
	
	if ( ReadEntryByNameStr ( rcd->lnk.dbname, &dms_cmm ) == -1 )
	{
	/*wenzhengguo		
	printf ( "OIX: DrawDynamicNumeric: %s is not found in database.\n", rcd->lnk.dbname );
		*/		return;
	}
	
	if ( dms_cmm.point.data_type != ANA_TYPE && dms_cmm.point.data_type != PARAM_TYPE  
		&& dms_cmm.point.data_type != CHARA_TYPE &&dms_cmm.point.data_type !=OUTA_TYPE)
	{
		printf ( "OIX: %s is not an analog/numeric/param point.\n", rcd->lnk.dbname );
		return;
	}
	else
	{                
		
        if(dms_cmm.var.anlg.value.status.chan_fail)
            strcpy(data_quality.draw_info,"F");
		
		if(!dms_cmm.fixed.iost.scanEnable)
            strcpy(data_quality.draw_info,"M");
		
        if(dms_cmm.var.anlg.value.status.ctrl_block)
            strcpy(data_quality.draw_info,"B");  
		
		rt_value	= dms_cmm.var.anlg.fValue;  
		
		if (rcd->lnk.dyn.fmt_def.rpt_num.dec_pt >= 0 && rcd->lnk.dyn.fmt_def.rpt_num.dec_pt <6 )
			sprintf (FormatS,"%%*.%df\0"  ,rcd->lnk.dyn.fmt_def.rpt_num.dec_pt);
		else     
			strcpy (FormatS,"%.3f\0"  );
		
		/*     sprintf (FormatS,"%%10.%df\0"  ,rcd->lnk.dyn.fmt_def.rpt_num.dec_pt);  */
		
		
        if (((dms_cmm.var.anlg.value.status.chan_fail && dms_cmm.point.data_type != PARAM_TYPE && !dms_cmm.fixed.ioattr.sysVirtual )
			||dms_cmm.var.anlg.value.status.ctrl_block) )
			sprintf ( rcd->bck.data.text.text,"%s", "****" );
		else
			sprintf ( rcd->bck.data.text.text,(char*)FormatS, strlen( rcd->bck.data.text.text),  rt_value );
		
		if ( rcd->lnk.cgrp.tag == DYN_CGRP_TAG_ON || rcd->lnk.cgrp.tag == DYN_CGRP_TAG_ADD_ON)
		{
			blink_status[crt_n]	= dms_cmm.fixed.iost.almNoAck;
			lmt_type		= dms_cmm.var.anlg.value.status.lmt_state;                       
			
            rcd->bck.fg	= GetDynamicColor ( crt_n, rcd, lmt_type, blink_status[crt_n] );                                             
		}
		
        if(dms_cmm.fixed.ioattr.ctrl!=0)
			rcd->bck.fg	= GetCtrlPtDynamicColor ( crt_n, rcd);
		
		if ( ReadEntryByNameStr ( rcd->lnk.dbname, &dms_cmm ) == -1 )
		{
			return;
		}
		
		if(!dms_cmm.fixed.iost.scanEnable) //人工置数后模拟量黄色显示
			rcd->bck.fg=1023; //yellow
		
		DrawTag (   crt_n,  drawable,  gc,  rcd   );
	}
}

void	DrawRptTime (int crt_n,GdkWindow	*drawable, GdkGC	*gc, GBRCD	*rcd )
{
	char	char_date[40];
	memset(char_date,0,40);
	
	GetRptTimeFromSelectFile ( (short)crt_n, select_rpt_hist_name[crt_n], char_date );
	if ( char_date != ( char* )NULL )
		strcpy( rcd->bck.data.text.text, char_date );
}


void DrawRptNumeric (int crt_n,GdkWindow	*drawable, GdkGC	*gc, GBRCD	*rcd )
{
	
	
	
	char            FormatS[20];
	short		req_type;
	short		pkvly_data_type; 
	short		time_id;		
	
	short		length;
	float		rt_value;
	
	int	rpt_data_type;
	short	hist_tab_date = 0;
	RPT_DATA	rpt_data;	
	int	rpt_return;
	
	char	db_name[DBNAME_LEN];
	
	req_type = rcd->lnk.dyn.fmt_def.rpt_num.req_type;
	pkvly_data_type = rcd->lnk.dyn.fmt_def.rpt_num.special_type;
	strcpy( db_name, rcd->lnk.dbname );
	
	switch( req_type ) 
	{
	case	REQ_HOURLY:
		time_id = rcd->lnk.dyn.fmt_def.rpt_num.hour_id;
		
		if( ((strcmp(&db_name[strlen(db_name)-strlen(CALC_ACC)], CALC_ACC) == 0) && (time_id == 23))
			|| ((strcmp(&db_name[strlen(db_name)-strlen(CALC_ACC1)], CALC_ACC1) == 0) && (time_id == 23)) )
		{
			time_id= hour;
			printf("time_id=hour=%d\n", hour);
		}	
		
		/*	if ( rpt_tab_date[crt_n] == CUR_DAY_RPT_DATA||rpt_tab_date[crt_n] ==CUR_MONTH_RPT_DATA)
		hist_tab_date = rpt_tab_date[crt_n];
		else	
		hist_tab_date = HIST_RPT_DATA;*/
		
		break;
		
	case	REQ_DAYLY:	
	case	REQ_MONTHLY:
	case	REQ_YEARLY:
		time_id = rcd->lnk.dyn.fmt_def.rpt_num.date_id;
		/*if ( rpt_tab_date[crt_n] == CUR_DAY_RPT_DATA||rpt_tab_date[crt_n] ==CUR_MONTH_RPT_DATA)
		hist_tab_date = rpt_tab_date[crt_n];
		else	
		hist_tab_date = CUR_DAY_MONTH;*/
		break;
		
	default:
		break;
	}
	
	rpt_return = ReadRptEntryByName ( db_name, req_type, time_id, 
		pkvly_data_type, select_rpt_hist_name[crt_n], &rpt_data_type, &rpt_data );  
	
#ifdef	DEBUG	
	printf ( "in OIX: name =%s, req_type= %d, time_id=%d, pkvly_data_type= %d, select_rpt_hist_name=%s\n",
		db_name,req_type, time_id, pkvly_data_type, select_rpt_hist_name );
#endif
	if ( rpt_return ==  -1)
	{
		printf ( "OIX: DrawRptNumeric: %s is not found in rpt table.\n", rcd->lnk.dbname );
		sprintf ( rcd->bck.data.text.text,"Err");
		return;
	}
	length = rcd->lnk.dyn.fmt_def.rpt_num.length;
	if (rcd->lnk.dyn.fmt_def.rpt_num.dec_pt >= 0 && rcd->lnk.dyn.fmt_def.rpt_num.dec_pt <6 )
		sprintf (FormatS,"%%*.%df\0"  ,rcd->lnk.dyn.fmt_def.rpt_num.dec_pt);
	else     
		strcpy (FormatS,"%.3f\0"  );
	
	if ( rpt_data_type == DAY_RPT )
	{
		rt_value	= rpt_data.value.float_data;
		/*printf("in DAY_RPT:: rt_value=%.1f\n",rt_value);*/
		if ( pkvly_data_type   == OPN_NUM  
			|| pkvly_data_type == CLS_NUM
			|| pkvly_data_type == SG_CLS_NUM
			|| pkvly_data_type == SG_OPN_NUM )
		{
			if ( rt_value == 0)
				strcpy( rcd->bck.data.text.text,"0");
			else
				sprintf ( rcd->bck.data.text.text,"%-d\0", ( int)rt_value );
		}
		else
		{
			if ( rt_value == 0.0)
				strcpy( rcd->bck.data.text.text,"0.0");
			else
			{					
				{ 
					// 					if(  rt_value >1000.0 )
					// 						sprintf ( rcd->bck.data.text.text,"%-4.1f\0",  rt_value);
					// 					else
					// 						sprintf ( rcd->bck.data.text.text,"%-4.1f\0",rt_value ); 
					sprintf ( rcd->bck.data.text.text,(char*)FormatS, strlen( rcd->bck.data.text.text),  rt_value );
					
				}				
			}
		}
	}
	
	if ( rpt_data_type == MONTH_RPT||rpt_data_type == YEAR_RPT )
	{
		rt_value	= rpt_data.value.float_data;
		
		if ( pkvly_data_type == OPN_NUM  
			|| pkvly_data_type == CLS_NUM
			|| pkvly_data_type == SG_CLS_NUM
			|| pkvly_data_type == SG_OPN_NUM )
		{
			if ( rt_value == 0)
				strcpy( rcd->bck.data.text.text,"0");
			else
				sprintf ( rcd->bck.data.text.text,"%-d\0", ( int)rt_value );
		}
		else
		{
			if ( rt_value == 0.0)
				strcpy( rcd->bck.data.text.text,"0.0");
			else
			{
				if(  rt_value >10000.0 || rt_value< (float)-10000.0 )
					sprintf ( rcd->bck.data.text.text,"%-7.1f\0",  rt_value);
				else
					sprintf ( rcd->bck.data.text.text,"%-6.2f\0",  rt_value);
			}
		}
	}
}


void	DrawDynamicSboStatus (int crt_n,GdkWindow	*drawable, GdkGC	*gc, GBRCD	*rcd )
{
	short	status;
	status = sbo_result[crt_n]+1;
	if ( status == (SBO_RESULT_OK +1))
		blink_status[crt_n] = ACKN;
	else
		blink_status[crt_n] = UNACKN;
	if ( rcd->lnk.cgrp.tag == DYN_CGRP_TAG_ON || rcd->lnk.cgrp.tag == DYN_CGRP_TAG_ADD_ON)
	{
		rcd->bck.fg = GetDynamicColor ( crt_n, rcd, status, blink_status[crt_n] );
	}
}

void	DrawDynamicName (int crt_n, GdkWindow	*drawable, GdkGC	*gc, GBRCD	*rcd )
{
	DMS_COMMON	dms_cmm;
	ENTRY_IMP_VAL	*imp;
	
	ENTRY_ANLG_VAL	*anlg;
	ENTRY_STAT_VAL	*ind;
	short		status;
	char		state_buff[30]= {""};
	char		name_buff[30]= {""};
	short		itmp, flag_type;
	float		value;
	char		whole_logicname[POINT_NAME_SIZE];	
	
	if(strcmp(rcd->lnk.dbname,"")!=0)
	{            
		if(ReadEntryByNameStr ( rcd->lnk.dbname, &dms_cmm ) == -1)
		{
			printf("\n n DrawDynamicName:: No this point in the database file!! dbname=%s\n",rcd->lnk.dbname);
			return;
		}
		pnt_tab_ptr[crt_n]= dms_cmm.point;           
	}            
	
	else if (!(pnt_tab_ptr[crt_n].stn_id == 0 && pnt_tab_ptr[crt_n].dev_id == 0 && pnt_tab_ptr[crt_n].data_type && pnt_tab_ptr[crt_n].pt_id   ))   
	{
		if(ReadEntryById ( &pnt_tab_ptr[crt_n], &dms_cmm ) == -1)
		{
			printf("\n n DrawDynamicName:: No this point in the database file!! dbname=%s\n",rcd->lnk.dbname);
			return;
		}
		
	}
	
	
	switch( rcd->lnk.dyn.data_class )
	{
	case	StnName:
		GetStnLongnameById(pnt_tab_ptr[crt_n].stn_id,rcd->bck.data.text.text );
		strcat ( rcd->bck.data.text.text, " ( ");
		sprintf ( name_buff, "%2d", pnt_tab_ptr[crt_n].stn_id );
		strcat ( rcd->bck.data.text.text, name_buff );
		strcat ( rcd->bck.data.text.text, " )" );				
		break; 
		
		 	case	DevName:				
				GetGroupLongnameById(pnt_tab_ptr[crt_n].stn_id,pnt_tab_ptr[crt_n].dev_id,rcd->bck.data.text.text );
				
				sprintf ( name_buff, "(%2d)", pnt_tab_ptr[crt_n].dev_id );
				strcat ( rcd->bck.data.text.text, name_buff );	                                
				break;  
				
			case	CmdName:                            
				strcpy( rcd->bck.data.text.text,cmd_str);
				break;
				
			case 	LONG_NAME:
				GetPtLongnameById( &pnt_tab_ptr[crt_n], rcd->bck.data.text.text );
				//	strcpy ( rcd->bck.data.text.text, dms_cmm.fixed.entryLongname );                               
				break;			
				
			case	LOGIC_NAME:			
				GetPtNameById( &pnt_tab_ptr[crt_n], whole_logicname, NULL );
				strcpy( rcd->bck.data.text.text,whole_logicname ); 				
				break;
				
			case	Pt_LongName:			
				sprintf(rcd->bck.data.text.text,"%d.%d.%d.%d",pnt_tab_ptr[crt_n].stn_id,
					pnt_tab_ptr[crt_n].dev_id,pnt_tab_ptr[crt_n].data_type,pnt_tab_ptr[crt_n].pt_id);
				break;
				
			case	ALM_PROHIBIT:
				itmp	= dms_cmm.fixed.iost.almEnable;
				if ( itmp == ENABLE )
					rcd->bck.fg = ColorId3to4(GREEN1); 
				else
					rcd->bck.fg = ColorId3to4(RED1);
				
				strcpy ( rcd->bck.data.text.text, alm_str[itmp]); 
				break;
				
			case	OUT_OF_SCAN:
				itmp	= dms_cmm.fixed.iost.scanEnable;
				if ( itmp == ENABLE )
					rcd->bck.fg = ColorId3to4(GREEN1); 
				else
					rcd->bck.fg = ColorId3to4(RED1);
				
				strcpy ( rcd->bck.data.text.text, scan_str[itmp]);   
				
				break;
				
			case	ALM_ACK:
				itmp	= dms_cmm.fixed.iost.almNoAck;
				if ( itmp == ACKN)
					rcd->bck.fg = ColorId3to4(GREEN1); 
				else
					rcd->bck.fg = ColorId3to4(RED1);
				
				strcpy ( rcd->bck.data.text.text, ack_str[itmp]);    
				break;
				
			case	SCAN_ADDR:
				sprintf ( rcd->bck.data.text.text, "%5d\0", dms_cmm.point.pt_id );
				break;
				
			case	CTRL_ADDR:
				sprintf ( rcd->bck.data.text.text, "%3d\0", dms_cmm.fixed.ctrl_addr);
				break;
				
			case	FAULT_PNT:
				itmp	= dms_cmm.fixed.ioattr.fail;
				strcpy ( rcd->bck.data.text.text, mimic_str[itmp]); 
				break;
				
			case	Key_Signal:
				itmp	= dms_cmm.fixed.ioattr.keySignal;
				strcpy ( rcd->bck.data.text.text, mimic_str[itmp]); 
				break;
				
			case	Pt_Sgzy:
				itmp	= dms_cmm.fixed.ioattr.faultRec;
				strcpy ( rcd->bck.data.text.text, mimic_str[itmp]); 
				break;
				
			case	Soe:
				itmp	= dms_cmm.fixed.ioattr.soe;
				strcpy ( rcd->bck.data.text.text, mimic_str[itmp]); 
				break;
				
			case	Pt_TrndAlm:
				itmp  = dms_cmm.fixed.ioattr.trendAlm;
				strcpy ( rcd->bck.data.text.text, mimic_str[itmp]); 
				break;
				
			case	Pt_CTRL:			
				itmp	= dms_cmm.fixed.ioattr.ctrl;
				strcpy ( rcd->bck.data.text.text, mimic_str[itmp]); 
				break;
				
			case	AUDIO_ALM:
				itmp	= dms_cmm.fixed.ioattr.audAlm;
				strcpy ( rcd->bck.data.text.text, audio_str[itmp]);  
				break;			
				
			case	PT_CALC:
				itmp	= dms_cmm.fixed.ioattr.statistic;
				printf("###in PT_CALC: itmp=%d,data_type=%d\n",itmp,pnt_tab_ptr[crt_n].data_type);
				printf("*** mimic_str[%d]=%s\n",itmp,mimic_str[itmp]);
				strcpy ( rcd->bck.data.text.text, mimic_str[itmp]); 
				break;
				
			case	Virtual_Pnt:
				itmp	= dms_cmm.fixed.ioattr.sysVirtual;
				strcpy ( rcd->bck.data.text.text, mimic_str[itmp]); 
				break;
				
			case	Lcu_Virtual:
				itmp	= dms_cmm.fixed.ioattr.lcuVirtual;
				strcpy ( rcd->bck.data.text.text, mimic_str[itmp]); 
				break;
				
			case	Update_State:
				itmp	= !dms_cmm.fixed.iost.noUpdate;
				strcpy ( rcd->bck.data.text.text, mimic_str[itmp]); 
				if ( itmp == YES)
					rcd->bck.fg = ColorId3to4(GREEN1); 
				else
					rcd->bck.fg = ColorId3to4(RED1);
				break;
				
			case	Ctrl_lock_state:
				itmp 	= dms_cmm.fixed.iost.ctrlLock;
				strcpy ( rcd->bck.data.text.text, mimic_str[itmp]);
				break;
				
			case	Repeat_Alm:
				itmp 	= dms_cmm.fixed.iost.almRepeat;
				strcpy ( rcd->bck.data.text.text, mimic_str[itmp]);
				if ( itmp == NO)
					rcd->bck.fg = ColorId3to4(GREEN1); 
				else
					rcd->bck.fg = ColorId3to4(RED1);
				break;
				
			case	Tag_State:
				itmp 	= dms_cmm.fixed.iost.tagState;
				if(itmp==0)
					strcpy ( rcd->bck.data.text.text, mimic_str[itmp]);
				else
					strcpy ( rcd->bck.data.text.text, mimic_str[1]);
				if ( itmp == NO)
					rcd->bck.fg = ColorId3to4(GREEN1); 
				else
					rcd->bck.fg = ColorId3to4(RED1);
				break;
				
			case	Io_Group:
				sprintf(rcd->bck.data.text.text,"%d",dms_cmm.fixed.io_group);
				break;
				
			case	Proctype:
				sprintf(rcd->bck.data.text.text,"%d",dms_cmm.fixed.ioattr.procType);
				break;
				
			case	Alm_Cba:
				itmp 	= dms_cmm.fixed.ioattr.almCba;
				strcpy(rcd->bck.data.text.text,mimic_str[itmp]);
				break;
				
			case	Fail_Pnt:
				itmp 	= dms_cmm.fixed.ioattr.fail;
				strcpy(rcd->bck.data.text.text,mimic_str[itmp]);
				break;
				
			case	Reverse_Pnt:
				itmp 	= dms_cmm.fixed.ioattr.normalClose;
				strcpy(rcd->bck.data.text.text,mimic_str[itmp]);
				break;
				
			case	Double_Pnt:
				itmp 	= dms_cmm.fixed.ioattr.doublePt;
				strcpy(rcd->bck.data.text.text,mimic_str[itmp]);
				break;
				
			case	Cascade_Pnt:
				itmp 	= dms_cmm.fixed.ioattr.cascade;
				strcpy(rcd->bck.data.text.text,mimic_str[itmp]);
				break;
				
			case	Hdb_Curve:
				itmp 	= dms_cmm.fixed.ioattr.hdbCurve;
				strcpy(rcd->bck.data.text.text,mimic_str[itmp]);
				break;
				
			case	Hdb_Period:
				itmp 	= dms_cmm.fixed.ioattr.hdbPeriod;
				sprintf(rcd->bck.data.text.text,"%d",itmp);
				break;
				
			case	Terminal_No:
				strcpy(rcd->bck.data.text.text,dms_cmm.fixed.hard_addr.terminal_no);
				break;
				
			case	Unit_No:
				strcpy(rcd->bck.data.text.text,dms_cmm.fixed.hard_addr.unit_no);
				break;
				
			case	Slot_No:
				strcpy(rcd->bck.data.text.text,dms_cmm.fixed.hard_addr.slot_no);
				break;
				
			case	Chanel_No:
				strcpy(rcd->bck.data.text.text,dms_cmm.fixed.hard_addr.channel_no);
				break;
				
			case	Dsp_Filename:
				strcpy(rcd->bck.data.text.text,dms_cmm.fixed.dsp_filename);
				break;	
				
			case	IO_HDB:
				itmp	= dms_cmm.fixed.ioattr.hdbRec;
				strcpy ( rcd->bck.data.text.text, mimic_str[itmp]); 
				break;
				
			case	Dec_Num:
				sprintf(rcd->bck.data.text.text,"%d",dms_cmm.fixed.iost.viewPoint);
				break;							
		}
		switch 	(  pnt_tab_ptr[crt_n].data_type  )
		{
		case	POL_TYPE:
		case	SOE_TYPE:
		case	SYSMSG_TYPE:
		case	OUTS_TYPE:
			ind	= &dms_cmm.var.ind;
			switch	( rcd->lnk.dyn.data_class )
			{
			case	RT_STATE:
				flag_type	= dms_cmm.fixed.ioattr.procType;
				status	= ind->status.state;
				
				GetIndPtStateStrById( &pnt_tab_ptr[crt_n], rcd->bck.data.text.text ) ;
				strcat ( rcd->bck.data.text.text, " ( ");
				sprintf ( state_buff,"%3d", status );
				strcat ( rcd->bck.data.text.text, state_buff);
				strcat ( rcd->bck.data.text.text, " )" );
				
				blink_status[crt_n] = dms_cmm.fixed.iost.almNoAck;
				if ( rcd->lnk.cgrp.tag == DYN_CGRP_TAG_ON || rcd->lnk.cgrp.tag== DYN_CGRP_TAG_ADD_ON)
				{
					rcd->bck.fg = GetDynamicColor ( crt_n, rcd, status, blink_status[crt_n] );
				} 
				if ( status == 0)
					rcd->bck.fg = ColorId3to4(GREEN1);
				else if ( status == 1 )
					rcd->bck.fg = ColorId3to4(RED1);
				else if ( status == 2 )
					rcd->bck.fg = ColorId3to4(YLW1);
				else
					rcd->bck.fg = ColorId3to4(BLUE);
				break;
				
			case	Channel_Fail:				
				itmp = ind->status.chan_fail;
				if ( itmp == NORMAL_STATUS)
				{
					rcd->bck.fg = ColorId3to4(GREEN1); 
					strcpy ( rcd->bck.data.text.text,DPS_STR_NORMAL);
				}				
				else
				{
					rcd->bck.fg = ColorId3to4(RED1);
					strcpy ( rcd->bck.data.text.text,DPS_STR_FAIL);
				}				
				break;
				
			case    Man_Set:
				itmp = ind->status.man_set;
				strcpy ( rcd->bck.data.text.text,dq_str[itmp]);
				if ( itmp == NORMAL_STATUS)				
					rcd->bck.fg = ColorId3to4(GREEN1); 						
				else				
					rcd->bck.fg = ColorId3to4(RED1);				
				break;
				
			case	Open_Num:
				itmp	= dms_cmm.fixed.ioattr.statistic;
				if ( itmp == PNT_CALC_YES)
					sprintf ( rcd->bck.data.text.text, "%5d\0", dms_cmm.var.ind.stc.norm_open);
				else
					sprintf ( rcd->bck.data.text.text, "%5d\0", -1); 
				break;
				
			case	Close_Num:
				itmp	= dms_cmm.fixed.ioattr.statistic;
				if ( itmp == PNT_CALC_YES)
					sprintf ( rcd->bck.data.text.text, "%5d\0", dms_cmm.var.ind.stc.norm_close);
				else
					sprintf ( rcd->bck.data.text.text, "%5d\0", -1); 
				break;
				
			case	SgOpnNum:
				itmp	= dms_cmm.fixed.ioattr.statistic;                                
				if ( itmp == PNT_CALC_YES)
					sprintf ( rcd->bck.data.text.text, "%5d\0", dms_cmm.var.ind.stc.fault_open);
				else
					sprintf ( rcd->bck.data.text.text, "%5d\0", -1);                               
				break;
				
			case	SgClsNum:
				itmp	= dms_cmm.fixed.ioattr.statistic;
				if ( itmp == PNT_CALC_YES)
					sprintf ( rcd->bck.data.text.text, "%5d\0", dms_cmm.var.ind.stc.fault_close);
				else
					sprintf ( rcd->bck.data.text.text, "%5d\0", -1); 
				break;
				
			case	Upate_Time:
				sprintf (rcd->bck.data.text.text, "%02d:%02d:%02d:%03d", dms_cmm.var.ind.update_time.hour,
					dms_cmm.var.ind.update_time.minute,dms_cmm.var.ind.update_time.second,
					dms_cmm.var.ind.update_time.ms);
				rcd->bck.fg = ColorId3to4(GREEN1); 
				break;
				
			case	Bj_update_time:
				sprintf (rcd->bck.data.text.text, "%02d:%02d:%02d:%03d", dms_cmm.var.ind.change_time.hour,
					dms_cmm.var.ind.change_time.minute,dms_cmm.var.ind.change_time.second,
					dms_cmm.var.ind.change_time.ms);
				rcd->bck.fg = ColorId3to4(GREEN1); 
				break;
				
			case	FAULT_TAG:
				sprintf (rcd->bck.data.text.text, "%d", dms_cmm.var.ind.status.fault_tag);
				if ( itmp == NO)
					rcd->bck.fg = ColorId3to4(GREEN1); 
				else
					rcd->bck.fg = ColorId3to4(RED1);
				break;
				
			default:
				break;
			}
			break;
			
                case	DEVOBJ_TYPE:
                    switch	( rcd->lnk.dyn.data_class )
					{
					case	Condc0:
						sprintf (rcd->bck.data.text.text, "%d",dms_cmm.var.devobj.fb_status1.condc0 );
						break;
					case	Condc1:
						sprintf (rcd->bck.data.text.text, "%d",dms_cmm.var.devobj.fb_status1.condc1 );
						break;
					case	Ma:
						sprintf (rcd->bck.data.text.text, "%d",dms_cmm.var.devobj.fb_status1.ma );
						break;
					case	C0:
						sprintf (rcd->bck.data.text.text, "%d",dms_cmm.var.devobj.fb_status1.c0 );
						break;
					case	C1:
						sprintf (rcd->bck.data.text.text, "%d",dms_cmm.var.devobj.fb_status1.c1 );
						break;
					case	A0:
						sprintf (rcd->bck.data.text.text, "%d",dms_cmm.var.devobj.fb_status1.a0 );
						break;
					case	A1:
						sprintf (rcd->bck.data.text.text, "%d",dms_cmm.var.devobj.fb_status1.a1 );
						break;
					case	Swgf_n:
						sprintf (rcd->bck.data.text.text, "%d",dms_cmm.var.devobj.fb_status1.swgf_n );
						break;
					case	P0:
						sprintf (rcd->bck.data.text.text, "%d",dms_cmm.var.devobj.fb_status1.p0 );
						break;
					case	P1:
						sprintf (rcd->bck.data.text.text, "%d",dms_cmm.var.devobj.fb_status1.p1 );
						break;
					case	Localm:
						sprintf (rcd->bck.data.text.text, "%d",dms_cmm.var.devobj.fb_status1.localm );
						break;
					case	Lculm:
						sprintf (rcd->bck.data.text.text, "%d",dms_cmm.var.devobj.fb_status1.lculm );
						break;
					case	Resetf:                            
						break;
					case	Actt:
						sprintf (rcd->bck.data.text.text, "%d",dms_cmm.var.devobj.param );
						break;
					case	Pulset:
						break;
					case	Addr:
						break;
					case	O0:
						sprintf (rcd->bck.data.text.text, "%d",dms_cmm.var.devobj.fb_status2.q0 );
						break; 
					case	O1:
						sprintf (rcd->bck.data.text.text, "%d",dms_cmm.var.devobj.fb_status2.q1 );
						break; 
					case	Orderm:
						sprintf (rcd->bck.data.text.text, "%d",dms_cmm.var.devobj.fb_status2.orderm );
						break;
					case	Posd0i:
						sprintf (rcd->bck.data.text.text, "%d",dms_cmm.var.devobj.fb_status2.posd0 );
						break;
					case	Posd1i:
						sprintf (rcd->bck.data.text.text, "%d",dms_cmm.var.devobj.fb_status2.posd1 );
						break;
					default:
						break;
                    }
					break;
					
					case	ANA_TYPE:
					case	OUTA_TYPE:
					case	PARAM_TYPE:
#ifdef  DEBUG
						printf("****** ANA_TYPE,data_class=%d\n",rcd->lnk.dyn.data_class);
#endif 
						anlg	= &dms_cmm.var.anlg;			
						switch	( rcd->lnk.dyn.data_class )
						{
						case	RT_VALUE:				
							blink_status[crt_n] = dms_cmm.fixed.iost.almNoAck;
							
							sprintf ( rcd->bck.data.text.text,"%.3f\0",  anlg->fValue );
							
							if ( rcd->lnk.cgrp.tag == DYN_CGRP_TAG_ON || rcd->lnk.cgrp.tag == DYN_CGRP_TAG_ADD_ON)
							{
								rcd->bck.fg = GetDynamicColor ( crt_n, rcd,dms_cmm.var.anlg.value.status.lmt_state,blink_status[crt_n] );
							}
							break;
							
						case	Channel_Fail:				
							itmp = anlg->value.status.chan_fail;
							if ( itmp == NORMAL_STATUS)
							{
								rcd->bck.fg = ColorId3to4(GREEN1); 
								strcpy ( rcd->bck.data.text.text,DPS_STR_NORMAL);
							}				
							else
							{
								rcd->bck.fg = ColorId3to4(RED1);
								strcpy ( rcd->bck.data.text.text,DPS_STR_FAIL);
							}				
							break;
							
						case    Man_Set:
							itmp = anlg->value.status.ctrl_block;
							strcpy ( rcd->bck.data.text.text,DPS_STR_CTRL_BLOCK);
							if ( itmp == NORMAL_STATUS)				
								rcd->bck.fg = ColorId3to4(GREEN1); 						
							else				
								rcd->bck.fg = ColorId3to4(RED1);				
							break;
							
						case	Unit_Name:
							sprintf(rcd->bck.data.text.text,"%s",dms_cmm.var.anlg.unitName);
							break;
							
						case	H_LMT:		
							sprintf ( rcd->bck.data.text.text,"%.1f\0", anlg->lmt_para.HI_WARN );		
							break;
							
						case	L_LMT:		
							sprintf ( rcd->bck.data.text.text,"%.1f\0", anlg->lmt_para.LO_WARN );	
							break;
							
						case	HH_LMT:			
							sprintf ( rcd->bck.data.text.text,"%.1f\0", anlg->lmt_para.HI_ALARM );
							break;
							
						case	LL_LMT:			
							sprintf ( rcd->bck.data.text.text,"%.1f\0", anlg->lmt_para.LO_ALARM );
							break;
							
						case	HRTN_LMT:			
							sprintf ( rcd->bck.data.text.text,"%.1f\0", anlg->lmt_para.HI_RECOVER );	
							break;
							
						case	LRTN_LMT:			
							sprintf ( rcd->bck.data.text.text,"%.3f\0", anlg->lmt_para.LO_RECOVER );
							break;
							
						case	DEAD_BAND:			
							sprintf ( rcd->bck.data.text.text,"%.3f\0",anlg->eng_para.D_BAND );		
							break;
							
						case	ENG_CVTER:			
							sprintf ( rcd->bck.data.text.text,"%.3f\0", anlg->eng_para.ENG_CVT );		
							break;
							
						case	HIRANGE:			
							sprintf ( rcd->bck.data.text.text,"%.3f\0", anlg->eng_para.HI_RANGE );		
							break;
							
						case	LORANGE:
							value	= anlg->eng_para.LO_RANGE;			
							sprintf ( rcd->bck.data.text.text,"%.3f\0", value );		
							break;
							
						case	BASE:				
							sprintf ( rcd->bck.data.text.text,"%.3f\0", anlg->eng_para.ZERO );		
							break;
							
						case	COMPENSATE:	
							sprintf ( rcd->bck.data.text.text,"%.3f\0",anlg->eng_para.COMPSATE);		
							break;
							
						case	Hi_Code:					
							sprintf ( rcd->bck.data.text.text,"%.3f\0",anlg->eng_para.HI_CODE );
							break;
							
						case	Lo_Code:
							sprintf ( rcd->bck.data.text.text,"%.3f\0",anlg->eng_para.LO_CODE );
							break;
							
						case	Tr_WARN:
							sprintf ( rcd->bck.data.text.text,"%.3f\0",anlg->lmt_para.TR_WARN);
							break;
							
						case	Tr_RECOVER:
							sprintf ( rcd->bck.data.text.text,"%.3f\0",anlg->lmt_para.TR_RECOVER);
							break;
							
						case	Lmt_State:
							GetAnaLmtStateStr(&dms_cmm,rcd->bck.data.text.text);
							if(strcmp(rcd->bck.data.text.text,DPS_STR_NORMAL)==0)
								rcd->bck.fg = ColorId3to4(GREEN1);
							else if(strcmp(rcd->bck.data.text.text,DPS_STR_H_ALM_LMT)==0
								||strcmp(rcd->bck.data.text.text,DPS_STR_L_ALM_LMT)==0)
								rcd->bck.fg = ColorId3to4(RED1);
							else if(strcmp(rcd->bck.data.text.text,DPS_STR_H_WRN_LMT)==0
								||strcmp(rcd->bck.data.text.text,DPS_STR_L_WRN_LMT)==0)
								rcd->bck.fg = ColorId3to4(YELLOW);
							else 
								rcd->bck.fg = ColorId3to4(WHITE);
							break;
							
						case	TrLmt_State:				
							GetAnaTrLmtStateStr(&dms_cmm,rcd->bck.data.text.text);
							if(strcmp(rcd->bck.data.text.text,DPS_STR_NORMAL)==0)
								rcd->bck.fg = ColorId3to4(GREEN1);
							else if( strcmp(rcd->bck.data.text.text,DPS_STR_OVER_TREND_LMT)==0 )
								rcd->bck.fg = ColorId3to4(RED1);
							else 
								rcd->bck.fg = ColorId3to4(WHITE);
							break;
							
						case	Upate_Time:
							sprintf (rcd->bck.data.text.text, "%02d:%02d:%02d:%03d", dms_cmm.var.anlg.update_time.hour,
								dms_cmm.var.anlg.update_time.minute,dms_cmm.var.anlg.update_time.second,
								dms_cmm.var.anlg.update_time.ms);
							rcd->bck.fg = ColorId3to4(GREEN1); 
							break;
						default:			
							break;
			}
			break;
			
		case	IMP_TYPE:                     
			imp	= &dms_cmm.var.imp.entry;
			switch	( rcd->lnk.dyn.data_class )
			{
			case	ENG_CVTER:
				value		= dms_cmm.var.imp.entry.eng_para.ENG_CVT;
				printf(" in ENG_CVTER: value=%f\n", value);
				sprintf ( rcd->bck.data.text.text,"%g\0", value );
				break;
				
			case	RT_VALUE:
				value	=	(float)(dms_cmm.var.imp.entry.rt_value);
				sprintf ( rcd->bck.data.text.text,"%f", value );
				
				if ( rcd->lnk.cgrp.tag == DYN_CGRP_TAG_ON || rcd->lnk.cgrp.tag == DYN_CGRP_TAG_ADD_ON)
				{
					rcd->bck.fg = GetDynamicColor ( crt_n, rcd,
						dms_cmm.var.anlg.value.status.lmt_state,blink_status[crt_n]);
				}
				break;
				
			case	Channel_Fail:				
				itmp = imp->status.chan_fail;
				if ( itmp == NORMAL_STATUS)
				{
					rcd->bck.fg = ColorId3to4(GREEN1); 
					strcpy ( rcd->bck.data.text.text,DPS_STR_NORMAL);
				}				
				else
				{
					rcd->bck.fg = ColorId3to4(RED1);
					strcpy ( rcd->bck.data.text.text,DPS_STR_FAIL);
				}				
				break;
				
			case    Man_Set:
				itmp = imp->status.man_set;
				strcpy ( rcd->bck.data.text.text,dq_str[itmp]);
				if ( itmp == NORMAL_STATUS)				
					rcd->bck.fg = ColorId3to4(GREEN1); 						
				else				
					rcd->bck.fg = ColorId3to4(RED1);				
				break;
				
			case	Upate_Time:
				sprintf (rcd->bck.data.text.text, "%02d:%02d:%02d:%03d", dms_cmm.var.imp.entry.update_time.hour,
					dms_cmm.var.imp.entry.update_time.minute,dms_cmm.var.imp.entry.update_time.second,
					dms_cmm.var.imp.entry.update_time.ms);
				rcd->bck.fg = ColorId3to4(GREEN1); 
				break;
				
            case    NORM_DAY:
                sprintf (rcd->bck.data.text.text, "%.1f", dms_cmm.var.imp.normal.day);
				rcd->bck.fg = ColorId3to4(GREEN1); 
				break;
				
			default:
				break;
			}
			default:
				break;
	}		
}

void	DrawSubDynamicName (int crt_n, GdkWindow	*drawable, GdkGC	*gc, GBRCD	*rcd,int sub_win_n )
{
	DMS_COMMON	dms_cmm[MAX_SUB_WIN_NUM];             
	char name_buff[10];
	char  LongNamerStr1[128];
	
	if(strstr(rcd->lnk.act.fmt_def.dsp.name,".sbin")!=(char *)NULL &&strcmp(rcd->lnk.dbname,"")!=0)
	{            
		if(ReadEntryByNameStr ( rcd->lnk.dbname, &dms_cmm[sub_win_n] ) == -1)
		{
			printf("\n n DrawDynamicName:: No this point in the database file!! dbname=%s\n",rcd->lnk.dbname);
			return;
		}            
		obj_pnt_tab_ptr[sub_win_n][crt_n] = dms_cmm[sub_win_n].point;
	}     
	else if ( ReadEntryById( & obj_pnt_tab_ptr[sub_win_n][crt_n], &dms_cmm[sub_win_n] ) == -1 )
	{
		printf ("\n in DrawDynamicName:: No this point in the database file!!%d.%d.%d.%d \n",
			obj_pnt_tab_ptr[sub_win_n][crt_n].stn_id,obj_pnt_tab_ptr[sub_win_n][crt_n].dev_id,obj_pnt_tab_ptr[sub_win_n][crt_n].data_type,obj_pnt_tab_ptr[sub_win_n][crt_n].pt_id);
		return;
	}
    /*   printf("obj_pnt_tab_ptr[%d][%d].data_type=%d\n",sub_win_n,crt_n,obj_pnt_tab_ptr[sub_win_n][crt_n].data_type);*/
	switch( rcd->lnk.dyn.data_class )
	{
	case	StnName:
		GetStnLongnameById(obj_pnt_tab_ptr[sub_win_n][crt_n].stn_id,rcd->bck.data.text.text );
		strcat ( rcd->bck.data.text.text, " ( ");
		sprintf ( name_buff, "%2d", pnt_tab_ptr[crt_n].stn_id );
		strcat ( rcd->bck.data.text.text, name_buff );
		strcat ( rcd->bck.data.text.text, " )" );				
		break; 
		
	case	DevName:				
		GetGroupLongnameById(obj_pnt_tab_ptr[sub_win_n][crt_n].stn_id,obj_pnt_tab_ptr[sub_win_n][crt_n].dev_id,rcd->bck.data.text.text );
		sprintf ( name_buff, "(%2d)", obj_pnt_tab_ptr[sub_win_n][crt_n].dev_id );
		strcat ( rcd->bck.data.text.text, name_buff );	                                
		break;                            
		
		
	case 	LONG_NAME:
		GetEntryLongnameById (&dms_cmm[sub_win_n].point,LongNamerStr1 );
		strcpy ( rcd->bck.data.text.text, LongNamerStr1 );                               
		break;			
		
		/*case	LOGIC_NAME:			
		GetPtNameById( &obj_pnt_tab_ptr[sub_win_n][crt_n], whole_logicname, NULL );
		strcpy( rcd->bck.data.text.text,whole_logicname ); 				
		break;*/
	default:
		break;
	}
	switch 	(  obj_pnt_tab_ptr[sub_win_n][crt_n].data_type  )
	{                        
	case	DEVOBJ_TYPE:
		switch	( rcd->lnk.dyn.data_class )
		{
		case	Condc0:
			sprintf (rcd->bck.data.text.text, "%d",dms_cmm[sub_win_n].var.devobj.fb_status1.condc0 );
			break;
		case	Condc1:
			sprintf (rcd->bck.data.text.text, "%d",dms_cmm[sub_win_n].var.devobj.fb_status1.condc1 );
			break;
		case	Ma:
			sprintf (rcd->bck.data.text.text, "%d",dms_cmm[sub_win_n].var.devobj.fb_status1.ma );
			break;
		case	C0:
			sprintf (rcd->bck.data.text.text, "%d",dms_cmm[sub_win_n].var.devobj.fb_status1.c0 );
			break;
		case	C1:
			sprintf (rcd->bck.data.text.text, "%d",dms_cmm[sub_win_n].var.devobj.fb_status1.c1 );
			break;
		case	A0:
			sprintf (rcd->bck.data.text.text, "%d",dms_cmm[sub_win_n].var.devobj.fb_status1.a0 );
			break;
		case	A1:
			sprintf (rcd->bck.data.text.text, "%d",dms_cmm[sub_win_n].var.devobj.fb_status1.a1 );
			break;
		case	Swgf_n:
			sprintf (rcd->bck.data.text.text, "%d",dms_cmm[sub_win_n].var.devobj.fb_status1.swgf_n );
			break;
		case	P0:
			sprintf (rcd->bck.data.text.text, "%d",dms_cmm[sub_win_n].var.devobj.fb_status1.p0 );
			break;
		case	P1:
			sprintf (rcd->bck.data.text.text, "%d",dms_cmm[sub_win_n].var.devobj.fb_status1.p1 );
			break;
		case	Localm:
			sprintf (rcd->bck.data.text.text, "%d",dms_cmm[sub_win_n].var.devobj.fb_status1.localm );
			break;
		case	Lculm:
			sprintf (rcd->bck.data.text.text, "%d",dms_cmm[sub_win_n].var.devobj.fb_status1.lculm );
			break;                            
		case	Actt:
			sprintf (rcd->bck.data.text.text, "%d",dms_cmm[sub_win_n].var.devobj.param );
			break;                       
		case	O0:
			sprintf (rcd->bck.data.text.text, "%d",dms_cmm[sub_win_n].var.devobj.fb_status2.q0 );
			break; 
		case	O1:
			sprintf (rcd->bck.data.text.text, "%d",dms_cmm[sub_win_n].var.devobj.fb_status2.q1 );
			break; 
		case	Orderm:
			sprintf (rcd->bck.data.text.text, "%d",dms_cmm[sub_win_n].var.devobj.fb_status2.orderm );
			break;
		case	Posd0i:
			sprintf (rcd->bck.data.text.text, "%d",dms_cmm[sub_win_n].var.devobj.fb_status2.posd0 );
			break;
		case	Posd1i:
			sprintf (rcd->bck.data.text.text, "%d",dms_cmm[sub_win_n].var.devobj.fb_status2.posd1 );
			break;
		default:
			break;
		}
		default:
			break;	
	}		
}

void	DrawDynamicImpSet (int crt_n,GdkWindow	*drawable, GdkGC	*gc, GBRCD	*rcd )
{
	DMS_COMMON	dms_cmm;
	/*ENTRY_IMP_VAL	*imp;*/
	
	char		state_buff[30]= {""};
	char            FormatS[20];
#ifdef	DEBUG
	printf ( " OIX: in DrawDynamicImpSet \n");
#endif
	if(ReadEntryByNameStr ( rcd->lnk.dbname, &dms_cmm ) == -1)
	{
		printf("\n n DrawDynamicImpSet:: No this point in the database file!! dbname=%s\n",rcd->lnk.dbname);
		return;
	}
	
	
	switch(dms_cmm.point.data_type)
	{
	case	IMP_TYPE:
		/*  imp	= &dms_cmm.var.imp;  */
		break;
	default:
		return;
	}
	
	if (rcd->lnk.dyn.fmt_def.rpt_num.dec_pt >= 0 && rcd->lnk.dyn.fmt_def.rpt_num.dec_pt <6 )
		sprintf (FormatS,"%%.%df\0"  ,rcd->lnk.dyn.fmt_def.rpt_num.dec_pt);
	else     
		strcpy (FormatS,"%.3f\0"  );
	
	
	switch  ( rcd->lnk.dyn.data_class )
	{
	case    IMP_CONSTANT:
		sprintf ( rcd->bck.data.text.text, "%d", dms_cmm.var.imp.entry.eng_para.CONSTANT);
		break;
		
	case    ACCUM:
		sprintf ( rcd->bck.data.text.text, (char*)FormatS, dms_cmm.var.imp.entry.d_accum);
		break;  
		
	case	PEAK_DAY:
		sprintf ( rcd->bck.data.text.text, (char*)FormatS, dms_cmm.var.imp.peak.day/*imp->eng_para.DAY_RATE*imp->peak.day*/);
		break;
		
	case	PEAK_MONTH:
		sprintf ( rcd->bck.data.text.text, (char*)FormatS, dms_cmm.var.imp.peak.month);
		break;
		
	case	PEAK_YEAR:
		sprintf ( rcd->bck.data.text.text, (char*)FormatS, dms_cmm.var.imp.peak.year);
		break;
		
	case	PPEAK_DAY:
		sprintf ( rcd->bck.data.text.text, (char*)FormatS, dms_cmm.var.imp.ppeak.day);
		break;
		
	case	PPEAK_MONTH:
		sprintf ( rcd->bck.data.text.text, (char*)FormatS, dms_cmm.var.imp.ppeak.month);
		break;
		
	case	PPEAK_YEAR:
		sprintf ( rcd->bck.data.text.text, (char*)FormatS, dms_cmm.var.imp.ppeak.year);
		break;
		
	case	NORM_DAY:
		sprintf ( rcd->bck.data.text.text, (char*)FormatS, dms_cmm.var.imp.normal.day);
		break;
		
	case	NORM_MONTH:
		sprintf ( rcd->bck.data.text.text, (char*)FormatS, dms_cmm.var.imp.normal.month);
		break;
		
	case	NORM_YEAR:
		sprintf ( rcd->bck.data.text.text, (char*)FormatS, dms_cmm.var.imp.normal.year);
		break;
		
	case	TOTAL_DAY:
		sprintf ( rcd->bck.data.text.text, (char*)FormatS, dms_cmm.var.imp.sum.day);
		break;
		
	case	TOTAL_MONTH:
		sprintf ( rcd->bck.data.text.text, (char*)FormatS,dms_cmm.var.imp.sum.month);
		break;
		
	case	TOTAL_YEAR:
		sprintf ( rcd->bck.data.text.text, (char*)FormatS, dms_cmm.var.imp.sum.year);
		break;
		
	case	VALLEY_DAY:
		sprintf ( rcd->bck.data.text.text, (char*)FormatS, dms_cmm.var.imp.valley.day);
		break;
		
	case	VALLEY_MONTH:
		sprintf ( rcd->bck.data.text.text,(char*)FormatS, dms_cmm.var.imp.valley.month);
		break;
		
	case	VALLEY_YEAR:
		sprintf ( rcd->bck.data.text.text, (char*)FormatS, dms_cmm.var.imp.valley.year);
		break;
		
	case	CVT_F:
		sprintf ( rcd->bck.data.text.text, (char*)FormatS, dms_cmm.var.imp.entry.eng_para.ENG_CVT);
		break;
		
	case	DAY_SUM_INI:
		sprintf ( rcd->bck.data.text.text, (char*)FormatS, dms_cmm.var.imp.init.day);
		break;
		
	case	MONTH_SUM_INI:
		sprintf ( rcd->bck.data.text.text, (char*)FormatS, dms_cmm.var.imp.init.month);
		break;
		
	case	YEAR_SUM_INI:
		sprintf ( rcd->bck.data.text.text, (char*)FormatS, dms_cmm.var.imp.init.year);
		break;
		
	case	PT_CVT:
		sprintf ( rcd->bck.data.text.text,(char*)FormatS, dms_cmm.var.imp.entry.eng_para.PT);
		break;
		
	case	CT_CVT:			
		sprintf ( rcd->bck.data.text.text, (char*)FormatS, dms_cmm.var.imp.entry.eng_para.CT);
		break;
		
	case    IMP_REAL_NUM:
		sprintf ( rcd->bck.data.text.text, "%d", dms_cmm.var.imp.entry.rt_value);
		break;
		
	case    IMP_NUM_SAV:
		sprintf ( rcd->bck.data.text.text, "%d", dms_cmm.var.imp.entry.base_value);
		break;
		
	case    LAST_DAY:
		sprintf ( rcd->bck.data.text.text, (char*)FormatS, dms_cmm.var.imp.last.day);
		break;
		
	case    LAST_MONTH:
		sprintf ( rcd->bck.data.text.text, (char*)FormatS, dms_cmm.var.imp.last.month);
		break;
		
	case    LAST_YEAR:
		sprintf ( rcd->bck.data.text.text, (char*)FormatS, dms_cmm.var.imp.last.year);
		break;
		
	default:
		break;
		}
		
		
}

void	DrawDynamicImpTime (int crt_n,GdkWindow	*drawable, GdkGC	*gc, GBRCD	*rcd )
{
#ifdef	DEBUG
	printf ( " OIX:in DrawDynamicImpTime \n");
#endif
}

void DrawDynamicSymbolGrp (int crt_n, GdkWindow	*drawable, GdkGC	*gc, GBRCD	*rcd )
{
	DMS_COMMON	dms_cmm;
	int		a_cnt;
	short	status,blink_s;
	short	sg_status;
	
	/* printf("!!!! in DrawDynamicSymbolGrp: dbname=%s\n",rcd->lnk.dbname);  */
	
	if ( ReadEntryByNameStr ( rcd->lnk.dbname, &dms_cmm ) == -1 )
	{
		printf ( "OIX in DrawDynamicSymbolGrp  %s is not found in database.\n", rcd->lnk.dbname );
		return;
	}
    if ( dms_cmm.point.data_type == DEVOBJ_TYPE  )   
	{
		GetObjPtStateById(&dms_cmm.point ,&status);			
		
		sg_status = 0;
		
		blink_s = 0;
		if(dms_cmm.var.devobj.fb_status1.swgf_n==0 ||dms_cmm.var.devobj.fb_status2.posd0==1
            ||	dms_cmm.var.devobj.fb_status2.posd1==1)
            strcpy(data_quality.draw_info,"F");
		
		if(dms_cmm.var.devobj.fb_status1.lculm)
			strcpy(data_quality.draw_info,"Bu");
		
		if(dms_cmm.var.devobj.fb_status1.localm)
			strcpy(data_quality.draw_info,"L");
        
		
		
		a_cnt	= anim_cnt[crt_n];
		
		if (rcd->lnk.state == 1)  /*  0*/
			rcd->bck.data.sym	= symgdb[rcd->lnk.dyn.fmt_def.sym.sym_grp].symbol[dms_cmm.var.devobj.fb_status1.p0][a_cnt];
		else if (rcd->lnk.state == 2)  /* 1 */
			rcd->bck.data.sym	= symgdb[rcd->lnk.dyn.fmt_def.sym.sym_grp].symbol[dms_cmm.var.devobj.fb_status1.p1][a_cnt];
		else 
			rcd->bck.data.sym	= symgdb[rcd->lnk.dyn.fmt_def.sym.sym_grp].symbol[status][a_cnt]; 
		
		/*		wdk 99.05.03.	rcd->bck.fg is used to store blinking color for sym_grp.	*/
		if ( ( rcd->lnk.cgrp.tag == DYN_CGRP_TAG_ON || rcd->lnk.cgrp.tag == DYN_CGRP_TAG_ADD_ON) 
			&& rcd->lnk.cgrp.cgrp_id != 0)
		{
			rcd->bck.fg     = GetDynamicSymColor ( crt_n, rcd, status, blink_s, sg_status );
		}
		else		
			rcd->bck.fg	= BLACK4;
	}
	
	if ( dms_cmm.point.data_type == POL_TYPE || dms_cmm.point.data_type == SOE_TYPE 
		|| dms_cmm.point.data_type == SYSMSG_TYPE||dms_cmm.point.data_type ==OUTS_TYPE
		||dms_cmm.point.data_type ==ANA_TYPE)   
	{
		status  = dms_cmm.var.ind.status.state;			
		
		sg_status = dms_cmm.var.ind.status.fault_tag;
		
		blink_s = dms_cmm.fixed.iost.almNoAck;
		
		/*data_quality.man_set = dms_cmm.var.ind.status.man_set;
		data_quality.chan_fail = dms_cmm.var.ind.status.chan_fail;
		data_quality.ctrl_block= dms_cmm.fixed.iost.almEnable;*/
		
		if(dms_cmm.var.ind.status.chan_fail)
			strcpy(data_quality.draw_info,"F");   
		
		if(dms_cmm.var.ind.status.man_set)
			strcpy(data_quality.draw_info,"M");
		
		
		a_cnt	= anim_cnt[crt_n];
		
		
		if (rcd->lnk.state == 1)  /*  channel fail*/
		{
			switch (dms_cmm.point.data_type) 
			{
			case POL_TYPE:
			case SOE_TYPE:
			case OUTS_TYPE:
			case SYSMSG_TYPE:
				
				rcd->bck.data.sym	= symgdb[rcd->lnk.dyn.fmt_def.sym.sym_grp].symbol[dms_cmm.var.ind.status.chan_fail][a_cnt];
				break ;
			case ANA_TYPE:
				rcd->bck.data.sym	= symgdb[rcd->lnk.dyn.fmt_def.sym.sym_grp].symbol[dms_cmm.var.anlg.value.status.chan_fail][a_cnt];
				break ;
			}
		}
		else if (rcd->lnk.state == 2)  /* man set */
		{
			
			switch (dms_cmm.point.data_type) 
			{
			case POL_TYPE:
			case SOE_TYPE:
			case OUTS_TYPE:
			case   SYSMSG_TYPE:
				rcd->bck.data.sym	= symgdb[rcd->lnk.dyn.fmt_def.sym.sym_grp].symbol[dms_cmm.var.ind.status.man_set][a_cnt];
				break ;
			}
			
		}
		else 
			rcd->bck.data.sym	= symgdb[rcd->lnk.dyn.fmt_def.sym.sym_grp].symbol[status][a_cnt];
		
		/*		wdk 99.05.03.	rcd->bck.fg is used to store blinking color for sym_grp.	*/
		
		if ( ( rcd->lnk.cgrp.tag == DYN_CGRP_TAG_ON || rcd->lnk.cgrp.tag == DYN_CGRP_TAG_ADD_ON) 
			&& rcd->lnk.cgrp.cgrp_id != 0)
		{
			rcd->bck.fg     = GetDynamicSymColor ( crt_n, rcd, status, blink_s, sg_status );                       
		}
		else		
			rcd->bck.fg	= BLACK4;
		
	}
	DrawTag (   crt_n,  drawable,  gc,  rcd   );
}

#define	LIGHT_ON	1
#define	LIGHT_OFF	0
void	DrawDynamicAnaStatus (int crt_n,GdkWindow	*drawable, GdkGC	*gc, GBRCD	*rcd )
{
	DMS_COMMON	dms_cmm;
	int		a_cnt;
	short		limit_type;
	short		limit_status;
	short		rst_status;
	
	
	if ( ReadEntryByNameStr ( rcd->lnk.dbname, &dms_cmm ) == -1 )
	{
		printf ( "OIX in DrawDynamicAnaStatus  %s is not found in database.\n", rcd->lnk.dbname );
		return;
	}
	limit_type = rcd->lnk.dyn.fmt_def.ana_status.limit_type;	
	
	limit_status = dms_cmm.var.anlg.value.status.lmt_state;
	
	if ( limit_type == limit_status )
		rst_status = LIGHT_ON;
	else
		rst_status = LIGHT_OFF;
	
	/*		wdk 97.12.14.	*/
	a_cnt	= anim_cnt[crt_n];
	rcd->bck.data.sym	= symgdb[rcd->lnk.dyn.fmt_def.sym.sym_grp].symbol[rst_status][a_cnt];
}

void	DrawItemTag (int crt_n, GdkWindow	*drawable, GdkGC	*gc, BACKRCD	*rcd ,int tagtype)
{
	BACKRCD	tmprcd;
	int	color_pair	=25;	/* red pair blink	*/
	
	tmprcd.draw_type =DrwT_RECTANGLE;
	tmprcd.ftset_id=2;
	tmprcd.fg		=1023;// ColorId3to4(clr_pair[color_pair].clr[blink_flag[crt_n]]); 
	tmprcd.bg		=1023;// ColorId3to4(clr_pair[color_pair].clr[blink_flag[crt_n]]);  
	tmprcd.line_width	= 0;
	
 			tmprcd.rect.x1		= rcd->rect.x1-8;
			tmprcd.rect.y1		= rcd->rect.y1-6;
			tmprcd.rect.x2		= rcd->rect.x1-2;
			tmprcd.rect.y2		= rcd->rect.y1;
			
			
			tmprcd.fill.fill_type	= 1;
			
			DrawRcd ( crt_n,  drawable, gc, &tmprcd ,FALSE);
			
			tmprcd.draw_type =DrwT_CIRCLE;
			tmprcd.ftset_id=2;
			tmprcd.fg		=1023;// ColorId3to4(clr_pair[color_pair].clr[blink_flag[crt_n]]); 
			tmprcd.bg		=1023;// ColorId3to4(clr_pair[color_pair].clr[blink_flag[crt_n]]);  
			tmprcd.line_width	= 1;
			
			
			tmprcd.rect.x1		= rcd->rect.x1-8;
			tmprcd.rect.y1		= rcd->rect.y1-10;
			tmprcd.rect.x2		= rcd->rect.x1-2;
			tmprcd.rect.y2		= rcd->rect.y1-3;
			
			
			tmprcd.fill.fill_type	= 0;
			tmprcd.fill.fillbkmode	= 1;
			
			DrawRcd ( crt_n,  drawable, gc, &tmprcd ,FALSE);
			
			
}

void DrawSubDspBackground (int crt_n,GdkDrawable	*drawable,int sub_win_n )
{
	int	i;
	GBRCD	rcd,tmp_rcd;
	GCOND_BUFF	*layer;
	
#ifdef  DEBUG
	printf("     in DrawSubDspBackground,sub_win_n=%d   \n",sub_win_n );
#endif
	layer = &gcondb[sub_win_n][crt_n];
	
	for ( i=0; i < layer->nextpos; i++ )
	{
		rcd	= layer->buffer[i];
		tmp_rcd = layer->buffer[i];
		if ( rcd.lnk.dyn.dyn_tag == DYN_TAG_OFF 
			&&  rcd.lnk.cgrp.tag == DYN_CGRP_TAG_OFF ) 
		{
			tmp_rcd.bck.rect.x1=rcd.bck.rect.x1-sub_win[sub_win_n][crt_n].x1;
			tmp_rcd.bck.rect.y1=rcd.bck.rect.y1-sub_win[sub_win_n][crt_n].y1;
			tmp_rcd.bck.rect.x2=rcd.bck.rect.x2-sub_win[sub_win_n][crt_n].x1;
			tmp_rcd.bck.rect.y2=rcd.bck.rect.y2-sub_win[sub_win_n][crt_n].y1;
			
			if ( rcd.bck.draw_type != DrwT_SYMBOL)
			{                                
				DrawRcd ( crt_n,  drawable, info[crt_n].sub_gc[sub_win_n], &tmp_rcd.bck,FALSE );                               
			}
			else
			{                                
				DrawSymbol ( crt_n,drawable, info[crt_n].sub_gc[sub_win_n], &tmp_rcd.bck, BACKGROUND,FALSE, 0 );
			}
		}
	}
}

void	DrawDspBackground (int crt_n,  GdkDrawable *drawable )
{
	int	i;
	GBRCD	*rcd;
	GDBUFF	*layer;
	
#ifdef DEBUG 
	printf("    !! in DrawDspBackground   \n" );
#endif
	
	layer = &gdbase[crt_n]; 
	
	for ( i=0; i < layer->nextpos; i++ )
	{
		
		rcd	= &layer->buffer[i];
		
		if ((!SelectLayer[crt_n][rcd->bck.layer])&&UseLayerFilter[crt_n].UseFilter)//hcl add  layerfalter
		{
			continue;
		}
		
		if (( zoomdb[crt_n].factor >= gdbase[crt_n].layer[rcd->bck.layer].min_visible &&
			zoomdb[crt_n].factor <= gdbase[crt_n].layer[rcd->bck.layer].max_visible )||UseLayerFilter[crt_n].UseFilter)   //hcl add  layerfalter
		{ 
			
			if ( rcd->lnk.dyn.dyn_tag == DYN_TAG_OFF   
				&& rcd->lnk.cgrp.tag == DYN_CGRP_TAG_OFF) 
			{
				switch	( rcd->bck.draw_type )
				{
				case	DrwT_SYMBOL:
					DrawSymbol ( crt_n, drawable, info[crt_n].gc,   &rcd->bck, BACKGROUND,FALSE, 0 );
					break;
					
				case	DrwT_GIF:
					DrawGif ( crt_n,   drawable ,  info[crt_n].gc, &rcd->bck );      
					break;    
					
				default:
					if(rcd->lnk.dyn.dyn_type!= DF_BAR_CHART )   
						DrawRcd ( crt_n, drawable, info[crt_n].gc ,  &rcd->bck ,FALSE);   
					break;
				}
			}
		}
	}
}


int	GetGreenRedColor (short crt_n, GBRCD	*rcd, short vt_status )
{
	int	fg;
	
	if ( vt_status == ENABLE )	 
		fg = ColorId3to4(GREEN1);
	else 
		fg = ColorId3to4(RED1);
	return	fg;
}
int BracketDspProc(int crt_n, GdkWindow *drawable,int pos_x,int pos_y,int numb, int start_gnum, int end_gnum)
{
	int	i,k,e_end,cur_gnum;
	float	value;
	int	flag,data_type;
	char  FaultFlag[10] ;
	GdkGC     *interlock_gc= info[crt_n].interlock_gc;
	DMS_COMMON	dms_cmm;
	unsigned int	stat;
	short	rt_status;
	int 	fg;	
	int     start_flag,stint;
	char    dsp_item_name[POINT_LONGNAME_SIZE],val_str[20];	
	short last_layer_num;
	char tmp_str[20];
    char  LongNamerStr1[128];
	{
		gdk_gc_set_foreground (  interlock_gc , &pixels[ crt_n][WHITE]) ;
		
		e_end =0;
		for(i=start_gnum;i<=end_gnum;i++)
			e_end = MAX(group[i].offset+group[i].e_num,e_end);		
		
		start_flag= 1;
		/*printf("group[%d].offset=%d,e_num=%d\n",j,group[j].offset,group[j].e_num);*/
		cur_gnum = start_gnum+1;
		last_layer_num = 0;
		for(k=group[start_gnum].offset;k<e_end;k++)
		{
			/* printf("entry[%d].opr=%d\n",k,entry[k].opr);*/
			
			flag=0;
			fg= UNPASS_COLOR;
			
			gdk_gc_set_foreground (  interlock_gc , &pixels[ crt_n][WHITE]) ;                
			
			if(k==group[cur_gnum].offset)
			{
				if(entry[k-1].opr!=BRACKET_F)                                 
					windk_draw_text( drawable, font_set[SBO_FONT], interlock_gc, pos_x-25, pos_y, "+", strlen("+") ,crt_n);
				
				cur_gnum++;
				/*  printf("cur_gnum=%d\n",cur_gnum);*/
				
			}    
			
			
            if(entry[k].opr == BRACKET_F)
			{
				windk_draw_text( drawable, font_set[SBO_FONT], interlock_gc, pos_x-15, pos_y, "(", strlen("(") ,crt_n);
				last_layer_num=group[cur_gnum].layer;                  
				continue;
			}
			
			if (ReadEntryByNameStr(entry[k].name,&dms_cmm) ==-1)
			{				
				gdk_gc_set_foreground (  interlock_gc , &pixels[ crt_n][fg]) ;
				strcpy( dsp_item_name, _CS_(OIX_STR_DB_NO_POINT));
				strcat( dsp_item_name, entry[k].name);		
                if(entry[k].opr != BRACKET_F)
                {
					windk_draw_text( drawable, font_set[SBO_FONT], interlock_gc, pos_x, pos_y, dsp_item_name, strlen(dsp_item_name) ,crt_n);
					pos_y+= 20;
                }
				continue;
			}  
			strcpy(FaultFlag,"");
			switch  ( dms_cmm.point.data_type )
			{	
			case	POL_TYPE:
			case	SOE_TYPE:
			case	SYSMSG_TYPE:
				rt_status = dms_cmm.var.ind.status.state;     
				if(dms_cmm.var.ind.status.chan_fail)
					strcpy(FaultFlag,"CF");
				
				break;
			case	ANA_TYPE:
			case	PARAM_TYPE:
			case 	CHARA_TYPE:
			case 	OUTA_TYPE:
				if(dms_cmm.var.anlg.value.status.chan_fail)
					strcpy(FaultFlag,"CF");
				break ; 
				
			default:
				break;
			}
			
			
			data_type = dms_cmm.point.data_type;
            GetGroupLongnameById( dms_cmm.point.stn_id, dms_cmm.point.dev_id, dsp_item_name );
			
			GetEntryLongnameById (&dms_cmm.point,LongNamerStr1 );
			strcat( dsp_item_name, LongNamerStr1 );
			
			if (entry[k].opr==BIN_LG)
			{			
				if ((data_type!=SOE_TYPE) &&(data_type!=POL_TYPE) &&(data_type!=SYSMSG_TYPE)&&(data_type!=-1))
				{
					printf ( "Err:The data_type of %s isn\'t a ON_OFF variable!\n",entry[k].name);
					gdk_gc_set_foreground (  interlock_gc , &pixels[ crt_n][fg]) ;					
					GetGroupLongnameById( dms_cmm.point.stn_id, dms_cmm.point.dev_id, dsp_item_name );
					
					GetEntryLongnameById (&dms_cmm.point,LongNamerStr1 );
					strcat( dsp_item_name, LongNamerStr1);
					strcat( dsp_item_name, MSG_SBO_TYPE_ERROR);					
					windk_draw_text( drawable, font_set[SBO_FONT], interlock_gc, pos_x, pos_y, dsp_item_name, strlen(dsp_item_name) ,crt_n);
					pos_y+= 20;
					continue;
					
				}
				
				stat=dms_cmm.var.ind.status.rt_state;
				stint=entry[k].data.status&0xff;                           
				
				
				if ((entry[k].data.status&0xff00)==0)
				{
					if (stat==(unsigned int)stint)					
						flag=1;					
				}
				else
				{
					if (stat!=(unsigned int)stint)					
						flag=1;					
				}
				
			}
			else
			{                            
				if (data_type!=ANA_TYPE &&data_type!=CALC_TYPE&& data_type!=PARAM_TYPE)
				{
					printf ( "Err:The data_type of %s isn\'t a ANALOG variable!\n",entry[k].name);
					gdk_gc_set_foreground (  interlock_gc , &pixels[ crt_n][fg]) ;
					
					GetGroupLongnameById( dms_cmm.point.stn_id, dms_cmm.point.dev_id, dsp_item_name );
					
					GetEntryLongnameById (&dms_cmm.point,LongNamerStr1 );
					strcat( dsp_item_name, LongNamerStr1 );
					strcat( dsp_item_name, MSG_SBO_TYPE_ERROR);
					windk_draw_text( drawable, font_set[SBO_FONT], interlock_gc, pos_x, pos_y, dsp_item_name, strlen(dsp_item_name),crt_n );
					pos_y+= 20;
					continue;
				}                               
				
				
				value= dms_cmm.var.anlg.fValue;
                sprintf(val_str,"%.2f",entry[k].data.value);
				switch(entry[k].opr)
				{
				case LESS: if (value<entry[k].data.value)
						   {
							   flag=1;
							   strcat(dsp_item_name,"<");
							   strcat(dsp_item_name,val_str);
						   }
					break;
					
				case LESS_E: if (value<=entry[k].data.value)
							 {
								 flag=1;
								 strcat(dsp_item_name,"<=");
								 strcat(dsp_item_name,val_str);
							 }
					break;
					
				case LARGE: if (value>entry[k].data.value)
							{
								flag=1;
								strcat(dsp_item_name,">");
								strcat(dsp_item_name,val_str);
							}
					break;
					
				case LARGE_E	: if (value>=entry[k].data.value)
								  {
									  flag=1;
									  strcat(dsp_item_name,">=");
									  strcat(dsp_item_name,val_str);
								  }
					break;
					
				case EQUAL: if (value==entry[k].data.value)
							{
								flag=1;
								strcat(dsp_item_name,"=");
								strcat(dsp_item_name,val_str);
							}
					break;
					
				case N_EQUAL: if (value!=entry[k].data.value)
							  {
								  flag=1;
								  strcat(dsp_item_name,"!=");
								  strcat(dsp_item_name,val_str);
							  }
					break;
					
				}   /*switch*/
				
			}  /*else*/
			
			
			if( flag )
				fg= PASSED_COLOR;
			
			gdk_gc_set_foreground (  interlock_gc , &pixels[ crt_n][fg]) ;		
			
			/* printf("dsp_item_name[%d]=%s\n",k,dsp_item_name);   */      
			windk_draw_text( drawable, font_set[SBO_FONT], interlock_gc, pos_x, pos_y, dsp_item_name, strlen(dsp_item_name),crt_n );    			
			if (strcmp(FaultFlag,"")!=0)
				gdk_draw_text( drawable, font_set[SBO_FONT], interlock_gc, pos_x-20, pos_y, FaultFlag, strlen(FaultFlag)  );    	 		
			
			if(last_layer_num >group[cur_gnum].layer)
            {  
                strcpy(tmp_str,")");
                for(i=group[cur_gnum].layer;i<last_layer_num-1;i++)
                    strcat(tmp_str,")");
                gdk_gc_set_foreground (  interlock_gc , &pixels[ crt_n][WHITE]) ;
                windk_draw_text( drawable, font_set[SBO_FONT], interlock_gc, pos_x+240, pos_y, tmp_str, strlen(tmp_str) ,crt_n);
                last_layer_num=group[cur_gnum].layer;
			}                     
			pos_y+= 20;
			
		} /*for*/
		
		pos_x+= 250;
		
	} /*for*/
	return TRUE;
}/*return*/

void	DrawOperCondDsp (int crt_n, GdkWindow *drawable )
{
	int	numb;
	int	start_gnum,end_gnum;	
	
	GdkGC     *interlock_gc= info[crt_n].interlock_gc;	
	
	int 	fg;	
	int     pos_x, pos_y;
	char    dsp_head_name[POINT_LONGNAME_SIZE];
	
	
	for(numb=0;numb<interlock_pt_num;numb++)
		if (strcmp(sbo_pnt_name[crt_n],interlock[numb].o_name)==0)
			break;
		printf("###in DrawOperCondDsp:::sbo_pnt_name[%d]=%s,numb=%d\n",crt_n,sbo_pnt_name[crt_n],numb);
		if (numb>=interlock_pt_num)
		{
			printf ( "the point %s not find in InterLockGen.dat\n",sbo_pnt_name[crt_n]);
			return ;
		}
		
		fg= UNPASS_COLOR;
		
		gdk_gc_set_foreground (  interlock_gc , &pixels[ crt_n][fg]) ;
		strcpy(dsp_head_name,_CS_(MSG_SBO_FAIL));
		
		windk_draw_text( drawable, font_set[SBO_FONT], interlock_gc, /*(interlock[numb].g_num+1)*125-50*/50, 20, dsp_head_name, strlen(dsp_head_name),crt_n );
		
        pos_x= 50;
		pos_y= 50;
        start_gnum= interlock[numb].offset;
		end_gnum=interlock[numb].offset+interlock[numb].g_num;        
		
        BracketDspProc(crt_n,drawable,pos_x,pos_y,numb, start_gnum, end_gnum);
}

