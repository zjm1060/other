
/*-----------------------------------------------------------------------

	 
	   Created by chi.hailong
	   Create  Date : 2007.08.14
	   Last Update  : 2006.08.14
	   Unix SYN TO Window 2006.08.15 
	   
---------------------------------------------------------------------*/
#define	 REAL_CUEVE
#ifdef   REAL_CUEVE
#include <stdio.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <time.h>
#include 	<signal.h>
#include	"../inc/oix.h"
#include        "../../../pas/v4.0a/inc/trend_data_rec.h"
#define    REAL_CURVE_FILE   "/home/ems/h9000/dat/curvefile.dat"  //实时曲线配置

//#define    REAL_CURVE_DIS_FILE	"/home/ems/h9000/dat/curvedisfile.dat" //六条显示实时曲线

short	realC_dev = 1,realCurvC_stn=1,realC_dtype=1;
DB_TAB_WINDOW_INFO real_curve_db_info[MAX_CRT_NUMBER];
//WDQS_SHM_DATA  *real_curve_pt ;
int real_timeout_id ; 
GtkWidget *cmbHisCurvDev;
GList *realCurvC_stn_lst=NULL,*realCurvC_datatype_lst=NULL;
/********************* gtk_real_curve.h ************************/

extern GtkWidget 	*real_curve_main_w[MAX_CRT_NUMBER];
extern void	UpdateAnaValList1(int crt_n);

#define		SEL_ANA_LIST		0
#define		ANA_LIST		1
#define		STAR_T			0
#define		END_T			1
#define		MAX_POINTER_NUM		5000      /*  The number of points which will draw curve */


#define		CANVAS_MAX_POINTER	900 
//#define		MAX_CURVE_NUM		MAX_CURVE_NUM   

#define		IDLE_FLAG		0
#define		LOCK_FLAG		1

typedef struct
{
	POINTER         ptr[MAX_POINTER_NUM];
	SHORT           next_pos;
}       TRENDDATAHIS;

GtkWidget 	*real_curve_conf_w[MAX_CRT_NUMBER];
GtkWidget 	*starTmEntry,*endTmEntry,*list_ana_val,*sel_ana_val_list,   *scroll_win ; 

int 		mins , seconds, hours;     /* How many minitues or seconds or hours  */
extern  CURVE_SHM_DATA *shm_real_curve;  


void UpdateCRealDevMenu(int crt_n) ;


/********************** end ********************************/


extern void 	DestroyDlg(GtkWidget *w);
extern void 	SgzyDevSelectCB (GtkWidget* w, gpointer data);
extern void		DrawSgzyCurve(int crt_n,GtkWidget* w,GdkGC* gc,SHORT now_i,SHORT num,int x_left,int y_top,int width,int height,float ys,float ye,char longname[DBNAME_LEN*2]);
extern void		BlankFillCopy (char    *dst,char    *src,int len);
extern void		GetCrtNumberOfWgt ( GtkWidget  *wgt, int     *crt_n );
extern void		PopupMessageBox (GtkWidget *parent,char msg[] );
extern int		getMenuDataPointer(int crtn,int data);
extern int		getCrtNumFromPointer(int data);
extern int		getDataFromPointer(int data);
extern char		*_toUtf8(char *c);
extern char		*_fromUtf8(char *c);
extern char		*_toUtf8EX(char *c);

void            real_draw_area_realize(GtkWidget *widget,gpointer data);
void            realCurvRedraw (int crt_n,GdkDrawable *drawable , GdkGC *gc ,int width ,int height);

void            UpdateCRealDataTypeMenu(int crt_n) ;

const 		gchar *listC_item_data_r = "list_item_row_no" ;
const		gchar *lstC_item_data   = "lstC_item_data"   ;

/****** **                static	variable    *******/
extern foreground[9][10];
short	FetchFloor();	

static  short   trend_add_item ;
static  short   trend_del_item ;

int trend_station_id, i;
int  select_point,select_points[MAX_WDQS_COUNT],selC_pt_num=0;
POINTER  curv_ptr[6]  ;
TRENDDATAHIS   trenddatahis;
CURVE_DEF   	RealCCurvePt,tmpRealCCurvePt ;  /*RealCCurvePt--Global Variant*/

GtkWidget *CreateCRealCurvToolbar (GtkWidget * parent);
static GtkWidget *CreateProgRealCurveConf (GtkWidget *parent , int  crt_n );
/*extern	CURVE_SHM_DATA  *shm_real_curve;*/

void  CurveSampleFun(int startT , int endT , int curid, float PtValues[CANVAS_MAX_POINTER] ,int *nextPos)
{
	float DeltaT , fTime;
	int i,j; 
	int nEnd;

	int nNumPt, nStartPt;
	struct tm sTime;

	if (curid<0 || curid>=RealCCurvePt.curve_num || startT > endT)
	{
		*nextPos=0;
		return ;
	}

	DeltaT  = (float)(endT-startT)/(float) CANVAS_MAX_POINTER;

	if (endT > DATA_NUM)
	{
		endT=DATA_NUM;
	}

	nNumPt=endT - startT+1;
	startT%=DATA_NUM;
	endT=startT+nNumPt-1;

	if (nNumPt > DATA_NUM)
	{
		endT=startT+DATA_NUM-1;
		nNumPt=DATA_NUM;
	}

	if (endT < DATA_NUM)
	{
		sTime=*localtime(&shm_real_curve->nTime);
		nEnd=sTime.tm_hour*3600+sTime.tm_min*60+sTime.tm_sec;
		nStartPt=(shm_real_curve->nextpt-1-nEnd+startT+DATA_NUM)%(DATA_NUM);
		if (nEnd >= startT && nEnd <= endT)
		{
			nNumPt=nEnd-startT+1;
		}

		fTime=0;
		if (RealCCurvePt.buffer[curid].data_type == ANA_TYPE)
		{
			for (i=0, j=0; i+(int)fTime<nNumPt; j++)
			{
				PtValues[j]=shm_real_curve->dat.fvalue[curid][(nStartPt+i+(int)fTime)%(DATA_NUM)];
				fTime+=DeltaT;
			}
		}
		else
		{
			for (i=0, j=0; i+(int)fTime<nNumPt; j++)
			{
				PtValues[j]=(float)(shm_real_curve->dat.status[curid][(nStartPt+i+(int)fTime)%(DATA_NUM)]);
				fTime+=DeltaT;
			}
		}
	}
	
	*nextPos=j;
	
	
// 	for (i = 0 , j=0; i< CANVAS_MAX_POINTER ; i++)
// 	{
//         V = startT +  (int)(DeltaT  *(float)i);
// 		if (V >  DATA_NUM    ) continue ; 
// 		if( RealCCurvePt.buffer[curid].data_type    ==  ANA_TYPE)
// 			PtValues[i] = fvalue[V]  ;
// 		else
// 			PtValues[i] = (float)(status[V])  ;
// 		
// 		if (V <nNumPt)  j++ ;
// 		
// 		if (V >nNumPt)  { 
// 			
// 			PtValues[i]= 0  ; 
// 		}
// 		
// 		//             printf ("PtValues[%d] = %f ; j = %d  \t" ,i ,PtValues[i] ,j ) ;
// 	}
// 	
// 	*nextPos = j ;  
}

int    InitRealCCurvePt()
{
	FILE	*fp;
	char curvefilename[256] ; 
	memset(&RealCCurvePt , 0 , sizeof(CURVE_DEF)) ; 

	sprintf(curvefilename, "%s%s", SYS_DATA_PATH, CURVE_DATA_FILE);
	if ((fp = fopen(curvefilename, "rb")) == NULL) 
	{
		printf("OIX:unable to open %s\n", curvefilename);
		return FALSE;
	}
	
	if (fread(&RealCCurvePt,sizeof(CURVE_DEF),1,fp)!=1)
	{
		printf("in updatatrendlist fread error\n");
		return  FALSE ; 
	}
	
	
	fclose(fp);
#ifdef  SUN_OS
	Swap2Byte(&RealCCurvePt.curve_num);
#endif
	if( RealCCurvePt.curve_num > MAX_CURVE_NUM)
	{       
		gdk_beep();
		printf("Oix ::in gtk_real_curve.c UpdateCAnaLstSel:  RealCCurvePt.curve_num=%d > MAX_POINTER_NUM \n", RealCCurvePt.curve_num);
		RealCCurvePt.curve_num = 0;   
		return FALSE ;
	}
#ifdef  SUN_OS
	for( i=0; i<RealCCurvePt.curve_num; i++)
		Swap2Byte(&RealCCurvePt.buffer[i].pt_id);
#endif
	shm_real_curve->reloadflag  =1 ;
	return  TRUE ; 
	
}

void	SelRealValueCListCB (GtkWidget* w, gpointer list)
{
	GList *dlist;
	GtkObject       *list_item;
	int	select_row;	
	
	dlist = GTK_LIST(w)->selection;
	if(!dlist)
	{
		return;
	}
	while(dlist)
	{
		list_item=GTK_OBJECT(dlist->data);		
		
		select_row=(int)(gtk_object_get_data(list_item,listC_item_data_r ));	
		
		dlist= dlist->next;
	}
	
	switch  ( (int) list )
	{
	case	SEL_ANA_LIST:	
		trend_del_item	= select_row;
		select_point	= select_row-1;
		break;
		
	case	ANA_LIST:	
		trend_add_item	=select_row ;		
		break;
		
	default:	;
		
	}
	printf("OIX: trend_add_item is %d  !!!\n" , trend_add_item);
	
}


void	UpdateCAnaValList ( int crt_n  ) //左list
{
	GtkWidget       *list_item;
	short		i;
	POINTER		ptr;
//	DMS_COMMON	pdt;
	char longname[DBNAME_LEN];
	
	int		db_num[MAX_CRT_NUM];			
	gchar *pConvert=0;
	
	gtk_list_clear_items(GTK_LIST(list_ana_val),0,-1);
    trenddatahis.next_pos  = 0 ;
	
    
    ptr = real_curve_db_info[crt_n].select_pt ;
	
	
	db_num[crt_n] =  0   ;        
    if(-1==GetEntryNumById( real_curve_db_info[crt_n].select_pt.stn_id, 
		real_curve_db_info[crt_n].select_pt.dev_id, real_curve_db_info[crt_n].select_pt.data_type,&db_num[crt_n]))
		/*return*/  ;
		for (i=0; i< db_num[crt_n]; i++)
		{
			ptr.pt_id	=i ;
			
		
			if (  GetEntryLongnameById( &ptr,  longname ) == -1)
			{
				printf("OIX: TrendName No this point in the database file !!!\n");
			}
			else
			{    
				
				
				
			//	BlankFillCopy (longname, pdt.fixed.entryLongname,ENTRY_LONGNAME_SIZE);
				trenddatahis.ptr[trenddatahis.next_pos] = ptr ; 
				
				trenddatahis.next_pos ++ ; 
				
				list_item = gtk_list_item_new_with_label(pConvert=_toUtf8(longname));
				G_SAFE_FREE(pConvert);
				
				gtk_container_add(GTK_CONTAINER(list_ana_val),list_item );
				gtk_widget_show(list_item);
				gtk_object_set_data(GTK_OBJECT(list_item),listC_item_data_r,(gpointer)trenddatahis.next_pos);	
			}
		}
		
		printf("OIX:  total nextpos is %d  and dbnum is%d !!!\n" ,trenddatahis.next_pos ,  db_num);
		
}



void	UpdateCAnaLstSel (int crt_n )  //右list
{
	GtkWidget       *list_item;
	
	short           i;
	POINTER         *ptr;
//	DMS_COMMON      pdt;
	char            longname[POINT_LONGNAME_SIZE];   

	char strStn[STATION_LONGNAME_SIZE],strDev[GROUP_LONGNAME_SIZE];
	char strEntryName[ENTRY_LONGNAME_SIZE];	
	char strLongName[128];

	GdkColor color ;  
	FILE	*fp ;
	char curvefilename[256] ; 
	gchar *pConvert=0;
	int stn_num;
	
	sprintf(curvefilename, "%s%s", SYS_DATA_PATH, CURVE_DATA_FILE);
	gtk_list_clear_items(GTK_LIST(sel_ana_val_list),0,-1);
	
	gdk_color_parse("red",&color);
	
	if ((fp = fopen(curvefilename, "rb")) == NULL) 
	{
		printf("OIX:unable to open %s\n", curvefilename);
		return;
	}
	
	if (fread(&RealCCurvePt,sizeof(CURVE_DEF),1,fp)!=1)
		printf("in updatatrendlist fread error\n");
	fclose(fp);
#ifdef  SUN_OS
	Swap2Byte(&RealCCurvePt.curve_num);
#endif
	if( RealCCurvePt.curve_num > MAX_CURVE_NUM)
	{       
		gdk_beep();
		printf("Oix ::in gtk_real_curve.c UpdateCAnaLstSel:  RealCCurvePt.curve_num=%d > MAX_POINTER_NUM \n", RealCCurvePt.curve_num);
		RealCCurvePt.curve_num = 0;   
		return;
	}
#ifdef  SUN_OS
	for( i=0; i<RealCCurvePt.curve_num; i++)
		Swap2Byte(&RealCCurvePt.buffer[i].pt_id);
#endif
	
	for (i=0; i<RealCCurvePt.curve_num; i++)
	{
		ptr	= &RealCCurvePt.buffer[i];
#ifdef  DEBUG
		printf("in UpdateCAnaLstSel: dev_id=%d, data_type=%d, pt_id = %d\n",
			ptr->dev_id, ptr->data_type, ptr->pt_id);
#endif
		
		if (  GetPtLongnameById/*GetEntryLongnameById*/( ptr,  longname )==-1)
		{
			printf("OIX: TrendList no this point in the database file !!!\n");
			sprintf (longname ,"No such data point") ; 
			list_item = gtk_list_item_new_with_label(pConvert=_toUtf8(longname));
			G_SAFE_FREE(pConvert);
			gtk_widget_modify_bg(list_item, GTK_STATE_NORMAL,&color);
			gtk_container_add(GTK_CONTAINER(sel_ana_val_list),list_item );			
			gtk_widget_show(list_item);
			gtk_object_set_data(GTK_OBJECT(list_item),listC_item_data_r,GINT_TO_POINTER(i+1));

		}
		else
		{							
			GetStnCommentById(ptr->stn_id, strStn);
			sprintf(strLongName, "%s-%s", strStn, longname);	

			GetStationNum(&stn_num);
			if (stn_num>1)
			{
				pConvert=_toUtf8(strLongName);
			}else
			{
				pConvert=_toUtf8(longname);
				}

			list_item = gtk_list_item_new_with_label(pConvert);
			G_SAFE_FREE(pConvert);
			gtk_widget_modify_bg(list_item, GTK_STATE_NORMAL,&color);
			gtk_container_add(GTK_CONTAINER(sel_ana_val_list),list_item );			
			gtk_widget_show(list_item);
			gtk_object_set_data(GTK_OBJECT(list_item),listC_item_data_r,GINT_TO_POINTER(i+1));
		}
	}
	
}


void	LstCValAddCB (GtkWidget *w  )
{
       int crt_n   ; int i;
	   FILE	*fp;
	   char curvefilename[256] ; 
	   
	   sprintf(curvefilename, "%s%s", SYS_DATA_PATH, CURVE_DATA_FILE);
       GetCrtNumberOfWgt(w, &crt_n);
       if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		   return ; 	   
	   if ( RealCCurvePt.curve_num  >=  MAX_CURVE_NUM)
	   {		   
		   gdk_beep();
		   PopupMessageBox(real_curve_conf_w[crt_n],"\nOIX: RealCCurvePt.curve_num >= MAX_CURVE_NUM\n");
		   return;
	   }
	   
	   for(i=0; i<RealCCurvePt.curve_num;i++)
	   {
		   if(memcmp((void*)&RealCCurvePt.buffer[i] , (void*)&trenddatahis.ptr[trend_add_item-1],sizeof(POINTER)) ==0 )
		   {
			   gdk_beep();
			   return ;
		   }
	   }           
	   if ( trend_del_item == 0 )
		   trend_del_item = RealCCurvePt.curve_num;
	   else
	   {
		   for (i=RealCCurvePt.curve_num; i>trend_del_item; i--)
			   RealCCurvePt.buffer[i] = RealCCurvePt.buffer[i-1];
	   }
	   RealCCurvePt.buffer[trend_del_item] = trenddatahis.ptr[trend_add_item-1] ;   
	   RealCCurvePt.curve_num++;
	       
	   /**********************************************/
	   if ((fp = fopen(curvefilename, "rb+")) == NULL)
	   {
		   printf("TrendAdd unable to open file\n");
		   return;
	   }
	   
#ifdef  SUN_OS   
	   tmpRealCCurvePt = RealCCurvePt;
	   for( i=0; i< tmpRealCCurvePt.curve_num; i++)
		   Swap2Byte(&tmpRealCCurvePt.buffer[i].pt_id);
	   Swap2Byte(&tmpRealCCurvePt.curve_num);
	   if (fwrite (&tmpRealCCurvePt, sizeof(CURVE_DEF), 1, fp) != 1)
		   printf("OIX:in TrendAddCB write error\n");
#else
	   if (fwrite (&RealCCurvePt, sizeof(CURVE_DEF), 1, fp) != 1)
		   printf("OIX:in TrendAddCB write error\n");
#endif	   
	   fclose (fp);         
	   	   
	   /************************************************/            	   
	   UpdateCAnaLstSel ( crt_n );	   
	   trend_del_item  = RealCCurvePt.curve_num; 
	   shm_real_curve->reloadflag  =1  ; 
	   
	   for (i=0; i<MAX_CRT_NUMBER; i++)
	   {
		   if (real_curve_main_w[i] != NULL)
		   {
			   UpdateAnaValList1(i);
		   }
	   }
}


void	LstCValRemoveCB ()
{
	int crt_n=0;
	FILE	*fp;
	int             i;

	char curvefilename[256] ; 	   
	sprintf(curvefilename, "%s%s", SYS_DATA_PATH, CURVE_DATA_FILE);

	if (trend_del_item ==0)
	{
		gdk_beep();
		return;
	}
	
	if ((fp = fopen(curvefilename, "rb+")) == NULL) 
	{
		printf("unable to open %s\n", curvefilename);
		return;
	}       
	
	gtk_list_clear_items(GTK_LIST(sel_ana_val_list),trend_del_item-1,trend_del_item);
	
	
	RealCCurvePt.curve_num = RealCCurvePt.curve_num-1;
	for (i=trend_del_item; i<RealCCurvePt.curve_num+1; i++)
	{ RealCCurvePt.buffer[i-1]	= RealCCurvePt.buffer[i]; }
	
#ifdef  SUN_OS   
	tmpRealCCurvePt = RealCCurvePt;
	for( i=0; i< tmpRealCCurvePt.curve_num; i++)
		Swap2Byte(&tmpRealCCurvePt.buffer[i].pt_id);
	Swap2Byte(&tmpRealCCurvePt.curve_num);
	if (fwrite (&tmpRealCCurvePt, sizeof(CURVE_DEF), 1, fp) != 1)
		printf("OIX:in TrendAddCB write error\n");
#else
	if (fwrite (&RealCCurvePt, sizeof(CURVE_DEF), 1, fp) != 1)
		printf("OIX:in TrendAddCB write error\n");
#endif
	
	fclose (fp);   
	
	
	
	UpdateCAnaLstSel(crt_n);	 
	trend_del_item	= 0;
	shm_real_curve->reloadflag  =1  ; 
	
}



void	DisplayCRealCurve (int crt_n  )
{
	crt_n = 0 ;
 
}


void ExitCRealCurveShell(GtkWidget *widget,   gpointer data)
{
	int crt_n = (gint) data ;
	gtk_widget_destroy(GTK_WIDGET(real_curve_conf_w[crt_n])); 
}

void  RealCurveFileRest (GtkWidget *widget,   gpointer data) 
{
	char curvefilename[256] ;
	int crt_n ; 
	FILE  *fp ; 
	CURVE_DEF   	 rstRealCCurvePt ; 	
	sprintf(curvefilename, "%s%s", SYS_DATA_PATH, CURVE_DATA_FILE);
	crt_n = (gint) data ;
	memset (&rstRealCCurvePt, 0,sizeof (CURVE_DEF)) ; 

	if ((fp = fopen(curvefilename, "wb")) == NULL) 
	{
		printf("OIX:unable to open %s\n", curvefilename);
		goto doError;
	}
	if (fwrite (&rstRealCCurvePt, sizeof(CURVE_DEF), 1, fp) != 1)
	{
		printf("OIX:in TrendAddCB write error\n");
		goto  doError; 
	}
	if (fp) fclose (fp ); 
return ; 
	
doError:
	PopupMessageBox(main_w[crt_n],"Reset Error!!!") ;
	if (fp) fclose (fp ); 
return ; 

}

void DestoryCRealCurveShell(GtkWidget *widget,   gpointer data)
{     
 
	int crt_n = (gint) data ;
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return ; 
    
	g_message("\n quit realcurve !!!!! \n");
	
	
	real_curve_conf_w[crt_n] = (GtkWidget *)NULL ;
	shm_real_curve->reloadflag  =1 ;
	return ;
}

gint	RealCDatatypePressProc (GtkWidget *w, gpointer data )
{
	int i;
	char cur_datatype_name[20];
	int	crt_n=0;
	gchar *pConvert=0;
	
	GetCrtNumberOfWgt ( w, &crt_n );
	if( crt_n < 0 )
		return FALSE; 
  		
	strcpy(cur_datatype_name,gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(data)->entry)));
	
	strcpy(cur_datatype_name, pConvert=_fromUtf8(cur_datatype_name));
	G_SAFE_FREE(pConvert);
	if(strcmp(cur_datatype_name,"")==0)
		return FALSE;
	for(i=0;i<sizeof(recordlongname)/sizeof(recordlongname[0]);i++)
	{
		if(strcmp(recordlongname[i],cur_datatype_name)==0)  
		{	
			break;
		}
	}
	
	realC_dtype    		= i;
	/*
	UpdateTrendNameList ( crt_n, trend_stn,realC_dev );	 
	*/   	
	return TRUE;
}

void ProgRealConfig (GtkWidget *parent ,int  crt_n )
{
    if (  crt_n  >0)  
    {    
		
		gdk_beep(); 
		PopupMessageBox(parent , OIX_DISPLAY_ON_SCREEN_0);
		return ; 
    } 
    
    
	if (real_curve_conf_w[crt_n]==(GtkWidget*)NULL)
    {
		real_curve_conf_w[crt_n]  = CreateProgRealCurveConf ( parent, crt_n   ) ;
		gtk_window_set_position(GTK_WINDOW(real_curve_conf_w[crt_n]) ,GTK_WIN_POS_CENTER_ON_PARENT);
		
		gtk_widget_show(real_curve_conf_w[crt_n] );
		
		
		
		
		UpdateCAnaValList ( crt_n ); 
		UpdateCAnaLstSel ( crt_n ) ;
		trenddatahis.next_pos = 0; 
    }else
    {
        gtk_window_present(GTK_WINDOW(real_curve_conf_w[crt_n]) );
    }
}

static GtkWidget *CreateProgRealCurveConf (GtkWidget *parent , int  crt_n )
{
	GtkWidget  *vbox,*vbox1, *hbox,*button_add_point,*button_del_point;
	GtkWidget  *toolbar,*ana_name_list_scrollw,*sel_ana_list_scrollw;
	GtkWidget  *windows ;
	gchar *pConvert=0;
													
    windows=gtk_window_new(GTK_WINDOW_TOPLEVEL);		
    gtk_window_set_default_size(GTK_WINDOW( windows),500, 400);		
			
    gtk_window_set_title(GTK_WINDOW(windows), pConvert=_toUtf8EX(OIX_CONFIG_REAL_CRUVE));
	G_SAFE_FREE(pConvert);
    gtk_window_set_transient_for(GTK_WINDOW(windows),GTK_WINDOW(parent));
				
    gtk_signal_connect(GTK_OBJECT(windows),
		"destroy",
		GTK_SIGNAL_FUNC( DestoryCRealCurveShell),
		GINT_TO_POINTER(crt_n));
	
    vbox=gtk_vbox_new(FALSE, 5);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 0);
    gtk_container_add(GTK_CONTAINER(windows), vbox);
    gtk_widget_show(vbox);
	
    toolbar =  CreateCRealCurvToolbar(windows);
    gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, TRUE, 0);
    gtk_widget_show(toolbar);    
	
	/************************** frame construct!! ******************************/
    hbox=gtk_hbox_new(FALSE, 3);    
    gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE,TRUE, 0);  
    gtk_widget_show(hbox);	
	/***************** list1 window code ***************************************/                     
	
    sel_ana_list_scrollw=gtk_scrolled_window_new (NULL, NULL);      
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (sel_ana_list_scrollw),
		GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
    gtk_widget_set_usize (sel_ana_list_scrollw, 200, -1); 
	
    gtk_box_pack_start(GTK_BOX(hbox), sel_ana_list_scrollw, FALSE, FALSE, 0);
    gtk_widget_show (sel_ana_list_scrollw); 
	
	
    list_ana_val = gtk_list_new(); 
	
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(sel_ana_list_scrollw),list_ana_val );  
    gtk_widget_show (list_ana_val); 
	
    gtk_signal_connect(GTK_OBJECT(list_ana_val), "selection_changed",
		GTK_SIGNAL_FUNC(SelRealValueCListCB), GINT_TO_POINTER(ANA_LIST)); 
	
	
	/********************* two button code here !! *********************************/	
    vbox1=gtk_vbox_new(FALSE, 3);
    gtk_widget_set_usize (vbox1,  60, 20);
    gtk_box_pack_start(GTK_BOX(hbox ), vbox1, FALSE, TRUE, 0);
    gtk_widget_show(vbox1);   
	

	button_add_point = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_ADD)); 
	G_SAFE_FREE(pConvert);

    gtk_box_pack_start (GTK_BOX (vbox1), button_add_point, TRUE, TRUE, 0);
    button_del_point = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_DEL));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start (GTK_BOX (vbox1), button_del_point, TRUE, TRUE, 0);     
	
    gtk_signal_connect(GTK_OBJECT(button_add_point), "clicked",
		GTK_SIGNAL_FUNC(LstCValAddCB), button_add_point);
	
   	gtk_signal_connect(GTK_OBJECT(button_del_point), "clicked",
		GTK_SIGNAL_FUNC(LstCValRemoveCB), button_del_point);			      
	
   	gtk_widget_show(button_add_point);
    gtk_widget_show(button_del_point);   	       
	
	/***************** list2 window code ***************************************/ 
	
    ana_name_list_scrollw=gtk_scrolled_window_new (NULL, NULL);      
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (ana_name_list_scrollw),
		GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
	
	
    gtk_box_pack_start(GTK_BOX(hbox), ana_name_list_scrollw, TRUE,TRUE, 0);
	
	/**************** gtk_widget_show (ana_name_list_scrollw);  ****************/
	
    sel_ana_val_list = gtk_list_new ();    
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(ana_name_list_scrollw),sel_ana_val_list );    
    gtk_widget_show (sel_ana_val_list);     
	
	gtk_signal_connect(GTK_OBJECT(sel_ana_val_list), "selection_changed",
		GTK_SIGNAL_FUNC(SelRealValueCListCB),SEL_ANA_LIST); 		        
	
    gtk_widget_show_all(windows);	       
    return windows;
}



void UpdateCRealStnSelect(GtkWidget* widget, gpointer data)
{
	int		crt_n;
	int realdata;
	//GetCrtNumberOfWgt(widget, &crt_n);
	
	crt_n=getCrtNumFromPointer((int)data);
	realdata=getDataFromPointer((int)data);
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return;
	
	real_curve_db_info[crt_n].select_pt.stn_id= (UCHAR)(int)realdata;
	UpdateCRealDevMenu(crt_n );
	UpdateCAnaValList ( crt_n );  
}


void UpdateCRealDevSelect(GtkWidget* widget, gpointer data)
{
	int		crt_n;
	int realdata;
	//GetCrtNumberOfWgt(widget, &crt_n);
	crt_n=getCrtNumFromPointer((int)data);
	realdata=getDataFromPointer((int)data);
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return;
	
	real_curve_db_info[crt_n].select_pt.dev_id= (UCHAR)(int)realdata;
	UpdateCRealDataTypeMenu(crt_n);
	UpdateCAnaValList (   crt_n );
}



void UpdateCRealDataTypeSelect(GtkWidget* widget, gpointer data)
{
	int		crt_n;
	int realdata;
	//GetCrtNumberOfWgt(widget, &crt_n);
	crt_n=getCrtNumFromPointer((int)data);
	realdata=getDataFromPointer((int)data);
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return;
	
	real_curve_db_info[crt_n].select_pt.data_type= (UCHAR)(int)realdata;
	UpdateCAnaValList ( crt_n );  
}




void UpdateCRealDevMenu(int crt_n) 
{
	char	dev_comment[GROUP_COMMENT_SIZE];
	UCHAR	stn_id, dev_id;
	int		i,grp_num;
	GtkWidget *menu,*items;
	gchar *pConvert=0;
	
	menu =gtk_option_menu_get_menu(GTK_OPTION_MENU(real_curve_db_info[crt_n].dev_optionmenu));
	gtk_option_menu_remove_menu(GTK_OPTION_MENU(real_curve_db_info[crt_n].dev_optionmenu));
	
	menu =gtk_menu_new();
	gtk_widget_set_usize(menu, 130, -1);
	
	stn_id= real_curve_db_info[crt_n].select_pt.stn_id;
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
		gtk_signal_connect(GTK_OBJECT(items), "activate", GTK_SIGNAL_FUNC(UpdateCRealDevSelect), (gpointer)i);
		if(!dev_id)
			dev_id= i;
	}
	
	gtk_option_menu_set_menu(GTK_OPTION_MENU(real_curve_db_info[crt_n].dev_optionmenu),menu);
	gtk_option_menu_set_history(GTK_OPTION_MENU(real_curve_db_info[crt_n].dev_optionmenu), 0);
	real_curve_db_info[crt_n].select_pt.dev_id= dev_id;
	gtk_widget_show_all(real_curve_db_info[crt_n].dev_optionmenu);
}



void UpdateCRealDataTypeMenu(int crt_n) 
{
	char	datatype_longname[RECORD_LONGNAME_SIZE];
	UCHAR	stn_id, dev_id, data_type;
	int		i,j,rec_num,entry_num;
	GtkWidget  *menu,*items;
	int		menu_index, active_index;
	gchar *pConvert=0;
	
	menu =gtk_option_menu_get_menu(GTK_OPTION_MENU(real_curve_db_info[crt_n].datatype_optionmenu));
	gtk_option_menu_remove_menu(GTK_OPTION_MENU(real_curve_db_info[crt_n].datatype_optionmenu));
	
	menu =gtk_menu_new();
	gtk_widget_set_usize(menu, 130, -1);
	
	stn_id= real_curve_db_info[crt_n].select_pt.stn_id;
	dev_id= real_curve_db_info[crt_n].select_pt.dev_id;
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
		GetRecLongnameByType((UCHAR)i, datatype_longname);
		items = gtk_menu_item_new_with_label(pConvert=_toUtf8(datatype_longname));	 
		G_SAFE_FREE(pConvert);
		gtk_container_add(GTK_CONTAINER(menu),items);
		gtk_signal_connect(GTK_OBJECT(items), "activate", GTK_SIGNAL_FUNC(UpdateCRealDataTypeSelect), (gpointer)i);
		j++;
		if(!data_type)
		{
			data_type= i;
			active_index= 0;
		}
		if(i==real_curve_db_info[crt_n].select_pt.data_type)
		{
			data_type= i;
			active_index= menu_index;
		}
		menu_index++;
	}
	
	gtk_option_menu_set_menu(GTK_OPTION_MENU(real_curve_db_info[crt_n].datatype_optionmenu),menu);
	gtk_option_menu_set_history(GTK_OPTION_MENU(real_curve_db_info[crt_n].datatype_optionmenu), active_index);
	real_curve_db_info[crt_n].select_pt.data_type= data_type;
	gtk_widget_show_all(real_curve_db_info[crt_n].datatype_optionmenu);
}




GtkWidget* CreateCRealStnMenu(int crt_n) 
{
	char	stn_comment[STATION_COMMENT_SIZE];
	int		i,stn_num;
	GtkWidget *optionmenu, *menu,*items;
	int		menu_index, active_index;
	UCHAR	stn_id;
	int MenuPointer;
	gchar *pConvert=0;
	
	menu =gtk_menu_new();
	gtk_widget_set_usize(menu, 130, -1);
	
	GetStationNum(&stn_num);
	stn_id= 0;
	menu_index= active_index= 0;
	for(i=1; i<=stn_num; i++) 
	{  		
		if(-1==GetStnIdByIndex((UCHAR)i, &stn_id))
			continue;
		
		if( GetStnCommentById((UCHAR)stn_id, stn_comment)==-1 )
			continue;
		
		items = gtk_menu_item_new_with_label(pConvert=_toUtf8(stn_comment));
		G_SAFE_FREE(pConvert);
		gtk_container_add(GTK_CONTAINER(menu),items);

		MenuPointer=getMenuDataPointer(crt_n,i);
		gtk_signal_connect(GTK_OBJECT(items), "activate", GTK_SIGNAL_FUNC(UpdateCRealStnSelect), (gpointer)MenuPointer);
	
		if(stn_id==real_curve_db_info[crt_n].select_pt.stn_id)
			active_index= menu_index;

		menu_index++;
	}
	
	optionmenu = gtk_option_menu_new();
	gtk_option_menu_set_menu(GTK_OPTION_MENU(optionmenu),menu);
	gtk_option_menu_set_history(GTK_OPTION_MENU(optionmenu), active_index);
	GetStnIdByIndex(active_index+1, &stn_id);
	real_curve_db_info[crt_n].select_pt.stn_id= stn_id;
	real_curve_db_info[crt_n].stn_optionmenu= optionmenu;
	return optionmenu;
}




GtkWidget* CreateCRealDevMenu(int crt_n) 
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
	
	stn_id= real_curve_db_info[crt_n].select_pt.stn_id;
	if(-1==GetGroupNumById(stn_id, &grp_num))
		return (GtkWidget*) NULL;
	
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
		gtk_signal_connect(GTK_OBJECT(items), "activate", GTK_SIGNAL_FUNC(UpdateCRealDevSelect), (gpointer)MenuPointer);
		
		if(!dev_id)
		{
			dev_id= i;
			active_index= 0;
		}
		if(i==real_curve_db_info[crt_n].select_pt.dev_id)
		{
			dev_id= i;
			active_index= menu_index;
		}
		menu_index++;
	}
	
	optionmenu = gtk_option_menu_new();
	gtk_option_menu_set_menu(GTK_OPTION_MENU(optionmenu),menu);
	(GTK_OPTION_MENU(optionmenu), active_index);
	real_curve_db_info[crt_n].select_pt.dev_id= dev_id;
	real_curve_db_info[crt_n].dev_optionmenu= optionmenu;
	return optionmenu;
}



GtkWidget* CreateCRealDataTypeMenu(int crt_n) 
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
	
	stn_id= real_curve_db_info[crt_n].select_pt.stn_id;
	dev_id= real_curve_db_info[crt_n].select_pt.dev_id;
	if(-1==GetRecordNumById(stn_id, dev_id, &rec_num))
		return (GtkWidget*)NULL;
	
	data_type= 0;
	menu_index= active_index= 0;
	for(i=1,j=0; j<rec_num; i++) 
	{  		
		if(i>=MAX_DATATYPE_NUM)
			break;
		if(-1==GetEntryNumById(stn_id, dev_id, (UCHAR)i, &entry_num))
			continue;
		GetRecLongnameByType((UCHAR)i, datatype_longname);
		items = gtk_menu_item_new_with_label(pConvert=_toUtf8(datatype_longname));	
		G_SAFE_FREE(pConvert);
		gtk_container_add(GTK_CONTAINER(menu),items);

		MenuPointer=getMenuDataPointer(crt_n,i);		
		gtk_signal_connect(GTK_OBJECT(items), "activate", GTK_SIGNAL_FUNC(UpdateCRealDataTypeSelect), (gpointer)MenuPointer);
		
		j++;
		if(!data_type)
		{
			data_type= i;
			active_index= 0;
		}
		if(i==real_curve_db_info[crt_n].select_pt.data_type)
		{
			data_type= i;
			active_index= menu_index;
		}
		menu_index++;
	}
	
	optionmenu = gtk_option_menu_new();
	gtk_option_menu_set_menu(GTK_OPTION_MENU(optionmenu),menu);
	gtk_option_menu_set_history(GTK_OPTION_MENU(optionmenu), active_index);
	real_curve_db_info[crt_n].select_pt.data_type= data_type;
	real_curve_db_info[crt_n].datatype_optionmenu= optionmenu;
	gtk_widget_show(optionmenu);
	return optionmenu;
}




GtkWidget * CreateCDataSelectToolbar (GtkWidget * parent, int crt_n)
{
	GtkWidget *box;  	
	GtkWidget *toolbar; 
	GtkWidget *menuStn ,*menuDev ,*menuDataType ; 
	
	toolbar = gtk_handle_box_new();
	
	box = gtk_hbox_new (FALSE, 5);
	gtk_container_add(GTK_CONTAINER(toolbar), box);
	gtk_container_border_width(GTK_CONTAINER(box), 5);
	menuStn =  CreateCRealStnMenu(crt_n) ;
	menuDev =  CreateCRealDevMenu(crt_n) ;
	menuDataType = CreateCRealDataTypeMenu(crt_n) ; 
	gtk_widget_show (menuStn);
	gtk_widget_show (menuDev);
	gtk_widget_show (menuDataType);
	gtk_box_pack_start(GTK_BOX(box), menuStn , FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box),menuDev  , FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box), menuDataType , FALSE, FALSE, 0);
	
	

	return (toolbar);
}


GtkWidget *
CreateCRealCurvToolbar (GtkWidget * parent)
{
	int	crt_n ;
	GtkWidget *box;  		
	GtkWidget *toolbar;   	
	GtkWidget	*button_exit,*btn_reset;
    GtkWidget *menuStn ,*menuDev ,*menuDataType ; 

	gchar *pConvert=0;

   	GetCrtNumberOfWgt (parent, &crt_n );
    if( crt_n < 0 )
		return (GtkWidget*)NULL;
	
	toolbar = gtk_handle_box_new ();
	
	box = gtk_hbox_new (FALSE, 4);
	gtk_container_add (GTK_CONTAINER (toolbar), box);
	gtk_container_border_width (GTK_CONTAINER (box), 5);
	gtk_widget_show (box);
	
	
	
	
 	 
	button_exit = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_EXIT));
	G_SAFE_FREE(pConvert);
    gtk_box_pack_end (GTK_BOX (box), button_exit, FALSE, FALSE, 0); 
    gtk_widget_show (button_exit);
	
	gtk_signal_connect(GTK_OBJECT(button_exit),
		"clicked",
		GTK_SIGNAL_FUNC(ExitCRealCurveShell),
		GINT_TO_POINTER(crt_n)  );
	
	/***********************************************************************/
	
    menuStn =  CreateCRealStnMenu(crt_n) ;
    menuDev =  CreateCRealDevMenu(crt_n) ;
    menuDataType = CreateCRealDataTypeMenu(crt_n) ; 
	 
	btn_reset = gtk_button_new_with_label(pConvert=_toUtf8EX(OIX_CSTR_RESET_FILE));
	G_SAFE_FREE(pConvert);
    gtk_box_pack_end (GTK_BOX (box), button_exit, FALSE, FALSE, 0); 
    gtk_widget_show (button_exit);
	
	gtk_signal_connect(GTK_OBJECT(btn_reset),
		"clicked",
		GTK_SIGNAL_FUNC(RealCurveFileRest),
		GINT_TO_POINTER(crt_n)  );
    gtk_widget_show (menuStn);
    gtk_widget_show (menuDev);
    gtk_widget_show (menuDataType);
	gtk_box_pack_start(GTK_BOX(box), menuStn , FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box),menuDev  , FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box), menuDataType , FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box), btn_reset , FALSE, FALSE, 0);
	
	
	return (toolbar);
}




#endif
