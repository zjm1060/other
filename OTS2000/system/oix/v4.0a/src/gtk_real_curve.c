
/*-----------------------------------------------------------------------

  
	Created by chi.hailong
	Create  Date : 2005.11.26
	Last Update  : 2006.6.11
	
	  
---------------------------------------------------------------------*/
#define	 REAL_CUEVE
#ifdef   REAL_CUEVE


//#include <stdio.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>
#include <time.h>
//#include 	<pthread.h>
//#include 	<thread.h>
#include 	<signal.h>
//#include 	<sys/shm.h>
#include	"../inc/oix.h"
#include        "../../../pas/v4.0a/inc/trend_data_rec.h"
#include "../inc/gtk_curve_widget.h"

//#include "\home\ems\system\pas\v4.0a\inc\trend_data_rec.h"
#define    REAL_CURVE_FILE   "/home/ems/h9000/dat/curvefile.dat"  //实时曲线配置结果
#define    REAL_CURVE_DIS_FILE	"/home/ems/h9000/dat/curvedisfile.dat" //六条显示实时曲线

//short	real_dev = 1,realCurv_stn=1,real_dtype=1;
//DB_TAB_WINDOW_INFO real_curve_db_info[MAX_CRT_NUMBER];
//WDQS_SHM_DATA  *real_curve_pt ;        //old 缓冲区
extern  CURVE_SHM_DATA *shm_real_curve;//new 缓冲区
int real_timeout_id ; 
/********************* gtk_real_curve.h ************************/

#define		MAX_POINTER_NUM		5000      /*  The number of points which will draw curve */
#define		CANVAS_MAX_POINTER	900
#define		MAX_DIS_CURVE_NUM		6   

#define 	NUM_THREADS 		5
#define 	SLEEP_TIME 		10
#define		IDLE_FLAG		0
#define		LOCK_FLAG		1

#define OIX_CSTR_DISPLAY_START_TIME "OIX_CSTR_DISPLAY_START_TIME"

typedef struct
{
	POINTER         ptr[MAX_POINTER_NUM];
	SHORT           next_pos;
}       TRENDDATAHIS;

typedef struct
{
	POINTER         ptr[MAX_DIS_CURVE_NUM];
	SHORT           next_pos;
} REALCURVEDISDATA;


GtkWidget 	*real_curve_main_w[MAX_CRT_NUMBER]={NULL};

GtkWidget 	*starTmEntry,*endTmEntry;
GtkWidget *realUnselectList=NULL, *realSelectList=NULL;

//实时曲线
typedef struct
{
	GtkWidget*	pButton[eCurveOpEnd];

	gint nToolType;					//工具按钮，0:十字指针 1:手指针 2:放大 3:缩小 4:区域放大

	struct tm startTime,endTime;	//查询开始时间 结束时间
}SRealTimeCurve;

SRealTimeCurve sRealCurve;

GtkCurveWidget realCurveWidget;
gdouble dRealValue[DATA_NUM];

int nRealCurveRef=0;

gint    real_curve_timer_id = 0;
short   CLR_GROUP[MAX_DIS_CURVE_NUM] = {GREEN1,YLW1,WHITE,RED1,PINK1,CHOCLT1} ;

enum {eItemName, eItemStnId, eItemDevId, eItemDataType, eItemPtId, eItemColEnd};

/********************** end ********************************/

extern char *_toUtf8(char *c);
//void realCurvRedrawE (int crt_n);
int _s_to_i(const char *str);
extern void	PopupMessageBox (GtkWidget *parent,char msg[] );
extern char *_toUtf8EX(char *c);

extern void popupErrorMsgBox(GtkWidget* pParent, gchar* strMsg);
extern void popupInfoMsgBox(GtkWidget* pParent, gchar* strMsg);
//extern gint transform(gdouble dAxis, gdouble dMin, gdouble dMax, gint nLength, gboolean bRev);
extern GtkWidget* create_pixmap (GtkWidget *widget, const gchar *filename);
extern GdkPixbuf* create_pixbuf (const gchar *filename);

GdkCursor* createCursor(GtkWidget* pParent, const gchar *filename, gboolean bCenter);

gboolean updateRealData(gpointer data);
void updateRealCurves(gint crt_n);

const 		gchar *list_item_data_r = "list_item_row_num" ;
const		gchar *lst_item_data   = "lst_item_data"   ;

TRENDDATAHIS   trenddatahis;
REALCURVEDISDATA	RealCurvePt; //six real curve line

#ifdef  SUN_OS
REALCURVEDISDATA	tmpRealCurvePt;
#endif

CURVE_DEF   	RealCCurvePt;
#ifdef  SUN_OS
CURVE_DEF tmpRealCCurvePt ;  /*RealCCurvePt--Global Variant*/
#endif

GtkWidget *CreateRealCurvToolbar (GtkWidget * parent, gint crt_n);
static GtkWidget *CreateProgRealCurveShell (GtkWidget *parent , int  crt_n );

gboolean isRealCurvePt(const POINTER* pt)
{
	int i;
	POINTER* pPtr;
	
	for (i=0; i<RealCCurvePt.curve_num;i++)
	{
		pPtr=&RealCCurvePt.buffer[i];
		if (pPtr->stn_id == pt->stn_id && pPtr->dev_id == pt->dev_id && pPtr->data_type == pt->data_type
			&& pPtr->pt_id == pt->pt_id)
		{
			return TRUE;
		}
	}

	return FALSE;
}

void	InitRealCurvePt ()	//zlb 用于右键菜单调用实时曲线窗口
{
	short           i;
	POINTER         *ptr;
	FILE	*fp;
	
	if ((fp = fopen(REAL_CURVE_FILE, "rb")) == NULL) 
	{
		printf("OIX:unable to open %s\n", REAL_CURVE_FILE);
		return;
	}
	
	if (fread(&RealCCurvePt,sizeof(CURVE_DEF),1,fp)!=1)
		printf("in updata realCurve Unselect list fread error\n");
	fclose(fp);
#ifdef  SUN_OS
	Swap2Byte(&RealCCurvePt.curve_num);
#endif
	if( RealCCurvePt.curve_num > MAX_POINTER_NUM)
	{       
		gdk_beep();
		printf("Oix ::in gtk_pop_menu_real_curve.c InitRealCurvePt:  RealCCurvePt.curve_num=%d > MAX_POINTER_NUM \n", RealCCurvePt.curve_num);
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
		printf("in InitRealCurvePt: dev_id=%d, data_type=%d, pt_id = %d\n",
			ptr->dev_id, ptr->data_type, ptr->pt_id);
#endif
	}
	
}

void	UpdateAnaValList1 ( int crt_n  )
{
	short           i;
	POINTER         *ptr;
//	char            longname[128];   
	FILE	*fp;

	GtkListStore* pStore;
	GtkTreeIter it;

	DMS_COMMON	pdt;
	
	char strStn[STATION_LONGNAME_SIZE],strDev[GROUP_LONGNAME_SIZE];
	char strEntryName[ENTRY_LONGNAME_SIZE];	
	char strLongName[128];
	char *pConvert=0;

	pStore=GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(realUnselectList)));
	gtk_list_store_clear(pStore);
	
	if ((fp = fopen(REAL_CURVE_FILE, "rb")) == NULL) 
	{
		printf("OIX:unable to open %s\n", REAL_CURVE_FILE);
		return;
	}
	
	if (fread(&RealCCurvePt,sizeof(CURVE_DEF),1,fp)!=1)
		printf("in updata realCurve Unselect list fread error\n");
	fclose(fp);
#ifdef  SUN_OS
	Swap2Byte(&RealCCurvePt.curve_num);
#endif
	if( RealCCurvePt.curve_num > MAX_POINTER_NUM)
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
		printf("in UpdateAnaValList1: dev_id=%d, data_type=%d, pt_id = %d\n",
			ptr->dev_id, ptr->data_type, ptr->pt_id);
#endif
		
		if ( ReadEntryById ( ptr, &pdt ) == -1)
		{
			sprintf(strLongName, "No such pt in db: %d.%d.%d.%d", ptr->stn_id, ptr->dev_id, ptr->data_type, 
				ptr->pt_id);
		}
		else
		{	
			GetStnCommentById(pdt.point.stn_id, strStn);
			GetGroupCommentById(pdt.point.stn_id, (UCHAR)pdt.point.dev_id, strDev);
			GetEntryLongnameById (&pdt.point, strEntryName);
			sprintf(strLongName, "%s - %s - %s", strStn, strDev, strEntryName);	
		}

		gtk_list_store_append(pStore, &it);
		gtk_list_store_set(pStore, &it, eItemName,  (pConvert=_toUtf8(strLongName)), eItemStnId, ptr->stn_id, 
				eItemDevId, ptr->dev_id, eItemDataType, ptr->data_type, eItemPtId, ptr->pt_id, -1);
		G_SAFE_FREE(pConvert);

// 		if (  GetPtLongnameById/*GetEntryLongnameById*/( ptr,  longname )==-1)
// 		{
// 			sprintf(longname, "No pt: %d.%d.%d.%d", ptr->stn_id, ptr->dev_id, ptr->data_type, ptr->pt_id);
// 			printf("OIX: realCurve no this point in the database file !!!\n");
// 		}
// 		else
// 		{
// 			gtk_list_store_append(pStore, &it);
// 			gtk_list_store_set(pStore, &it, eItemName,  _toUtf8(longname), eItemStnId, ptr->stn_id, 
// 				eItemDevId, ptr->dev_id, eItemDataType, ptr->data_type, eItemPtId, ptr->pt_id, -1);
// 		}
	}
	
}



void	UpdateAnaLstSel (int crt_n ) //更新左下list
{
	short           i;
	POINTER         *ptr;
	DMS_COMMON      pdt;
//	char            longname[128];   
	
	FILE*	fp;

	GtkListStore* pStore;
	GtkTreeIter it;

	char strStn[STATION_LONGNAME_SIZE],strDev[GROUP_LONGNAME_SIZE];
	char strEntryName[ENTRY_LONGNAME_SIZE];	
	char strLongName[128];
	char *pConvert=0;

	pStore=GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(realSelectList)));
	gtk_list_store_clear(pStore);
	
	if ((fp = fopen(REAL_CURVE_DIS_FILE, "rb")) == NULL) 
	{
		printf("OIX:RealCurve unable to open %s\n", REAL_CURVE_DIS_FILE);
		return;
	}
	
	if (fread(&RealCurvePt,sizeof(REALCURVEDISDATA),1,fp)!=1)
		printf("in updata RealCurve Select list fread error\n");
	fclose(fp);
#ifdef  SUN_OS
	Swap2Byte(&RealCurvePt.next_pos);
#endif
	if( RealCurvePt.next_pos > MAX_POINTER_NUM)
	{       
		gdk_beep();
		printf("Oix ::in gtk_real_curve.c UpdateAnaLstSel:  RealCurvePt.next_pos=%d > MAX_POINTER_NUM \n", RealCurvePt.next_pos);
		RealCurvePt.next_pos= 0;   
		return;
	}
#ifdef  SUN_OS
	for( i=0; i<RealCurvePt.next_pos; i++)
		Swap2Byte(&RealCurvePt.ptr[i].pt_id);
#endif
	
	for (i=0; i<RealCurvePt.next_pos; i++)
	{
		ptr	= &RealCurvePt.ptr[i];
#ifdef  DEBUG
		printf("in UpdateAnaLstSel: dev_id=%d, data_type=%d, pt_id = %d\n",
			ptr->dev_id, ptr->data_type, ptr->pt_id);
#endif
		
		
		if ( ReadEntryById ( ptr, &pdt )  == -1)
		{
			printf("OIX: TrendList no this point in the database file !!!\n");
			sprintf(strLongName, "No such pt in db: %d.%d.%d.%d", ptr->stn_id, ptr->dev_id, ptr->data_type, 
				ptr->pt_id);
		}
		else
		{			
			GetStnCommentById(pdt.point.stn_id, strStn);
			GetGroupCommentById(pdt.point.stn_id, (UCHAR)pdt.point.dev_id, strDev);
			GetEntryLongnameById (&pdt.point, strEntryName);
			sprintf(strLongName, "%s - %s - %s", strStn, strDev, strEntryName);	

// 			strcpy(longname,StnDef[pdt.point.stn_id].LongName);
// 			strcat(longname,StnDef[pdt.point.stn_id].dev[pdt.point.dev_id].LongName);
// 			strcat(longname,pdt.fixed.entryLongname);
// 			sprintf(longname, "%s", _toUtf8(longname));
		}

		gtk_list_store_append(pStore, &it);
		gtk_list_store_set(pStore, &it, eItemName,  (pConvert=_toUtf8(strLongName)), eItemStnId, 
		ptr->stn_id, eItemDevId, ptr->dev_id, eItemDataType, ptr->data_type, eItemPtId, ptr->pt_id, -1);
		G_SAFE_FREE(pConvert);
	}
	
}

void addItemToRealSelectList(POINTER* ptr, GtkListStore* pStore)
{
	DMS_COMMON      pdt;
	
	char longname[128];
	char *pConvert=0;
	
	GtkTreeIter it;
	
#ifdef  DEBUG
	printf("RealCurve Add Item : dev_id=%d, data_type=%d, pt_id = %d\n",
		ptr->dev_id, ptr->data_type, ptr->pt_id);
#endif
	
	
	if ( ReadEntryById ( ptr, &pdt )  == -1)
	{
		sprintf(longname, "No such pt in db: %d.%d.%d.%d", ptr->stn_id, ptr->dev_id, ptr->data_type, 
			ptr->pt_id);
	}
	else
	{			
		strcpy(longname,StnDef[pdt.point.stn_id].LongName);
		strcat(longname,StnDef[pdt.point.stn_id].dev[pdt.point.dev_id].LongName);
		strcat(longname,pdt.fixed.entryLongname);
		
		sprintf(longname, "%s", (pConvert=_toUtf8(longname)));
		G_SAFE_FREE(pConvert);
	}
	
	gtk_list_store_append(pStore, &it);
	gtk_list_store_set(pStore, &it, eItemName,  longname, eItemStnId, 
		ptr->stn_id, eItemDevId, ptr->dev_id, eItemDataType, ptr->data_type, eItemPtId, ptr->pt_id, -1);
}

void	LstValAddCB (GtkWidget *w, gpointer data)
{
	int crt_n=GPOINTER_TO_INT(data);
	int  i;

	FILE	*fp;
	   
	GtkTreeSelection* pSelect;
	GtkTreeModel* pModel;
	GtkTreeIter it;
	   
	GtkListStore* pStore;
	   
	POINTER pt;
	gint nStnId, nDevId, nDataType, nPtId;

	if( crt_n < 0 || crt_n >= MAX_CRT_NUMBER )
		return ;
	
	pSelect=gtk_tree_view_get_selection (GTK_TREE_VIEW (realUnselectList));
	pModel=gtk_tree_view_get_model(GTK_TREE_VIEW(realUnselectList));
	if (!gtk_tree_selection_get_selected (pSelect, &pModel, &it))
	{
		gdk_beep();
		printf("Real Curve Add Point: No Selected!\n");
		return;
	}
	
	if ( RealCurvePt.next_pos  >=  MAX_DIS_CURVE_NUM)
	{
		gdk_beep();
		PopupMessageBox(real_curve_main_w[crt_n],"\nOIX: RealCurvePt.next_pos >= 6\n");
		return;
	}

	gtk_tree_model_get (pModel, &it, eItemStnId, &nStnId, eItemDevId, &nDevId, eItemDataType, &nDataType, 
		eItemPtId, &nPtId, -1);
	
	pt.stn_id=(UCHAR)nStnId;
	pt.dev_id=(UCHAR)nDevId;
	pt.data_type=(UCHAR)nDataType;
	pt.pt_id=(USHORT)nPtId;
	
	//hcl 使用实时曲线配置 赋值
	for(i=0; i<RealCurvePt.next_pos;i++)
	{
		if((RealCurvePt.ptr[i].stn_id  == pt.stn_id) &&
			(RealCurvePt.ptr[i].dev_id  == pt.dev_id) &&
			(RealCurvePt.ptr[i].data_type  == pt.data_type) && 
			(RealCurvePt.ptr[i].pt_id  == pt.pt_id))
		{
			gdk_beep();
			printf("Real Curve Add Point: The point has been added!\n");
			return ;
		}
	}
	
	RealCurvePt.ptr[RealCurvePt.next_pos] = pt;
	RealCurvePt.next_pos++;

	/**********************************************/
	if ((fp = fopen(REAL_CURVE_DIS_FILE, "wb+")) == NULL)
	{
		printf("TrendAdd unable to open file\n");
		return;
	}
	
#ifdef  SUN_OS   
	tmpRealCurvePt = RealCurvePt;
	for( i=0; i< tmpRealCurvePt.next_pos; i++)
		Swap2Byte(&tmpRealCurvePt.ptr[i].pt_id);
	Swap2Byte(&tmpRealCurvePt.next_pos);
	if (fwrite (&tmpRealCurvePt, sizeof(REALCURVEDISDATA), 1, fp) != 1)
		printf("OIX:in TrendAddCB write error\n");
#else
	if (fwrite (&RealCurvePt, sizeof(REALCURVEDISDATA), 1, fp) != 1)
		printf("OIX:in TrendAddCB write error\n");
#endif
	
	fclose (fp);         
	
	
	/************************************************/    
	
	pStore=GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(realSelectList)));
	addItemToRealSelectList(&pt, pStore);

	updateRealCurves(crt_n);
}

void realListDbClick(GtkTreeView *tree_view,GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data)
{
	LstValAddCB(NULL, user_data);
}

void	LstValRemoveCB (GtkWidget* pWidget, gpointer data)
{
	int crt_n;
	FILE	*fp;
	int i;

	GtkTreeSelection* pSelect;
	GtkTreeModel* pModel;
	GtkTreeIter it;
	
	GtkListStore* pStore;
	
	POINTER pt;
	
	gint nStnId, nDevId, nDataType, nPtId;

	crt_n=GPOINTER_TO_INT(data);
	if( crt_n < 0 || crt_n >= MAX_CRT_NUMBER )
		return;

	pSelect=gtk_tree_view_get_selection (GTK_TREE_VIEW (realSelectList));
	
	pModel=gtk_tree_view_get_model(GTK_TREE_VIEW(realSelectList));
	
	if (!gtk_tree_selection_get_selected (pSelect, &pModel, &it))
	{
		gdk_beep();
		printf("Real Curve:No Selected!\n");
		return;
	}

	gtk_tree_model_get (pModel, &it, eItemStnId, &nStnId, eItemDevId, &nDevId, eItemDataType, 
		&nDataType, eItemPtId, &nPtId, -1);
	
	pt.stn_id=(UCHAR)nStnId;
	pt.dev_id=(UCHAR)nDevId;
	pt.data_type=(UCHAR)nDataType;
	pt.pt_id=(USHORT)nPtId;
	
	pStore=GTK_LIST_STORE(pModel);
	gtk_list_store_remove(pStore, &it);
	
	if ((fp = fopen(REAL_CURVE_DIS_FILE, "rb+")) == NULL) 
	{
		printf("unable to open %s\n", REAL_CURVE_DIS_FILE);
		return;
	}       
	
	for (i=0; i<RealCurvePt.next_pos; i++)
	{
		if (RealCurvePt.ptr[i].stn_id == pt.stn_id && RealCurvePt.ptr[i].dev_id == pt.dev_id 
			&& RealCurvePt.ptr[i].data_type == pt.data_type && RealCurvePt.ptr[i].pt_id == pt.pt_id)
		{
			break;
		}
	}
	
	if (i >= RealCurvePt.next_pos)
	{
		gdk_beep();
		printf("Real Curve Remove:No Find Point in Data!\n");
		return;
	}
	
	for (; i<RealCurvePt.next_pos-1;i++)
	{
		RealCurvePt.ptr[i]=RealCurvePt.ptr[i+1];
	}
	
	RealCurvePt.next_pos--;
	
#ifdef  SUN_OS   
	tmpRealCurvePt = RealCurvePt;
	for( i=0; i< tmpRealCurvePt.next_pos; i++)
		Swap2Byte(&tmpRealCurvePt.ptr[i].pt_id);
	Swap2Byte(&tmpRealCurvePt.next_pos);
	if (fwrite (&tmpRealCurvePt, sizeof(REALCURVEDISDATA), 1, fp) != 1)
		printf("OIX:in TrendAddCB write error\n");
#else
	if (fwrite (&RealCurvePt, sizeof(REALCURVEDISDATA), 1, fp) != 1)
		printf("OIX:in TrendAddCB write error\n");
#endif
	
	fclose (fp);   

	updateRealCurves(crt_n);
	
}

/*
 * 点击后 将横坐标改为输入时间范围
 */
void onOkBtnClick(GtkWidget *widget,   gpointer data)
{
	struct tm startTime, endTime;
	struct tm  tm_now;
	time_t   now,res;

	gdouble dStart, dEnd;

	gchar strTime[32];
	gchar strText[128];
	gchar *pConvert=0;

	int crt_n= GPOINTER_TO_INT(data);
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return ;

	if (RealCurvePt.next_pos <= 0)
	{
		gdk_beep();
		popupInfoMsgBox(real_curve_main_w[crt_n],"OIX: Must first select one curve at least!");
		return;
	}

	memset(&startTime, 0, sizeof(struct tm));
	sscanf(gtk_entry_get_text(GTK_ENTRY(starTmEntry)) , "%4d-%2d-%2d %2d:%2d" , &startTime.tm_year,
		&startTime.tm_mon,&startTime.tm_mday, &startTime.tm_hour,&startTime.tm_min);
	startTime.tm_year-=1900;
	startTime.tm_mon--;
	startTime.tm_sec=0;
	startTime.tm_isdst=-1;

	res=mktime(&startTime);
	if (res  == (time_t) -1)
	{
		popupErrorMsgBox(real_curve_main_w[crt_n], (pConvert=_toUtf8EX("OIX_CSTR_START_TIME_FORMAT_ERROR")));
		G_SAFE_FREE(pConvert);
		return;
	}

	memset(&endTime, 0, sizeof(struct tm));
	sscanf(gtk_entry_get_text(GTK_ENTRY(endTmEntry)) , "%4d-%2d-%2d %2d:%2d" , &endTime.tm_year,
		&endTime.tm_mon,&endTime.tm_mday, &endTime.tm_hour, &endTime.tm_min);
	endTime.tm_year-=1900;
	endTime.tm_mon--;
	endTime.tm_sec=0;
	endTime.tm_isdst=-1;
	
	res=mktime(&endTime);
	if (res  == (time_t) -1)
	{
		popupErrorMsgBox(real_curve_main_w[crt_n], (pConvert=_toUtf8EX("OIX_CSTR_END_TIME_FORMAT_ERROR")));
		G_SAFE_FREE(pConvert);
		return;
	}

	time(&now);
	tm_now = *localtime(&now);

	tm_now.tm_hour-=24;
	tm_now.tm_isdst=-1;

	res=mktime(&startTime)-mktime(&tm_now);
	if (res < 0)
	{
		startTime=tm_now;
	}

	res=mktime(&endTime)-mktime(&startTime);
	if (res > 24*60*60)
	{
		endTime=startTime;
		endTime.tm_hour+=24;
	}
	else if (res <= 60)
	{
		endTime=startTime;
		endTime.tm_min+=1;
	}

	endTime.tm_isdst=-1;
	mktime(&endTime);

	sprintf(strTime,"%04d-%02d-%02d %02d:%02d", startTime.tm_year+1900, startTime.tm_mon+1, startTime.tm_mday,
		startTime.tm_hour, startTime.tm_min) ;
	gtk_entry_set_text(GTK_ENTRY(starTmEntry), strTime);

	sprintf(strTime,"%04d-%02d-%02d %02d:%02d", endTime.tm_year+1900, endTime.tm_mon+1, endTime.tm_mday,
		endTime.tm_hour, endTime.tm_min) ;
	gtk_entry_set_text(GTK_ENTRY(endTmEntry), strTime);

	sRealCurve.startTime=startTime;
	sRealCurve.endTime=endTime;
	sprintf(strText, "%s --- %s", 
		gtk_entry_get_text(GTK_ENTRY(starTmEntry)), gtk_entry_get_text(GTK_ENTRY(endTmEntry)));
	gtk_curve_widget_set_text(&realCurveWidget, strText, eTextTitle);

	gtk_curve_widget_canvas_zoom_100(&realCurveWidget);

	dStart=mktime(&sRealCurve.startTime);
	dEnd=mktime(&sRealCurve.endTime);
	
	gtk_curve_widget_set_time_scale(&realCurveWidget, dStart, dEnd);

	updateRealCurves(crt_n);


	gtk_curve_widget_frame_redraw(&realCurveWidget);
	
	return;
}

void updateRealCurves(gint crt_n)
{
	gint nCurId;
	POINTER		*ptr;
	DMS_COMMON	pdt;
	char* pConvert=0;
//	gchar		longname[128];

	char strStn[STATION_LONGNAME_SIZE],strDev[GROUP_LONGNAME_SIZE];
	char strEntryName[ENTRY_LONGNAME_SIZE];	
	char strLongName[128];

	GCurve* pCurve;

	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return ;

	if (real_curve_timer_id != 0)
	{
		g_source_remove(real_curve_timer_id);
	}

	gtk_curve_widget_clear_all_curve(&realCurveWidget);

	for (nCurId=0; nCurId<RealCurvePt.next_pos; nCurId++)
	{
		pCurve=gtk_curve_create(DATA_NUM, eModeSec);
		gtk_curve_set_color(pCurve, &pixels[crt_n][CLR_GROUP[nCurId]]);

		ptr	= &RealCurvePt.ptr[nCurId];
		
		if (ReadEntryById(ptr, &pdt ) == -1)
		{
			sprintf (strLongName, "%s: %d.%d.%d.%d", (pConvert=_toUtf8EX("OIX_CSTR_SGZY_NO_SUCH_POINT")), 
			ptr->stn_id, ptr->dev_id, ptr->data_type, ptr->pt_id);

			G_SAFE_FREE(pConvert);
		}
		else
		{			
			GetStnCommentById(pdt.point.stn_id, strStn);
			GetGroupCommentById(pdt.point.stn_id, (UCHAR)pdt.point.dev_id, strDev);
			GetEntryLongnameById (&pdt.point, strEntryName);
			sprintf(strLongName, "%s - %s - %s", strStn, strDev, strEntryName);	
			sprintf(strLongName, "%s", (pConvert=_toUtf8(strLongName)));
			G_SAFE_FREE(pConvert);

// 			strcpy(longname,StnDef[pdt.point.stn_id].LongName);
// 			strcat(longname,StnDef[pdt.point.stn_id].dev[pdt.point.dev_id].LongName);
// 			strcat(longname,pdt.fixed.entryLongname);
// 			sprintf(longname, "%s", _toUtf8(longname));
		}

		if (ptr->data_type != ANA_TYPE)
		{
			gtk_curve_set_type(pCurve, eCurveLadder);
		}

		gtk_curve_set_text(pCurve, strLongName);
		
		gtk_curve_set_user_data(pCurve, GINT_TO_POINTER(ptr->stn_id), 0);
		gtk_curve_set_user_data(pCurve, GINT_TO_POINTER(ptr->dev_id), 1);
		gtk_curve_set_user_data(pCurve, GINT_TO_POINTER(ptr->data_type), 2);
		gtk_curve_set_user_data(pCurve, GINT_TO_POINTER(ptr->pt_id), 3);
		
		gtk_curve_widget_add_curve(&realCurveWidget, pCurve);
	}
	
	real_curve_timer_id = g_timeout_add (1000, (GSourceFunc)updateRealData, GINT_TO_POINTER(crt_n));

	updateRealData(GINT_TO_POINTER(crt_n));
}

void toolBarMgr(gint nToolType)
{
	gint nType;
	
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sRealCurve.pButton[nToolType])))
	{
		if (nToolType != sRealCurve.nToolType)
		{
			nType=sRealCurve.nToolType;
			sRealCurve.nToolType=nToolType;
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(sRealCurve.pButton[nType]), FALSE);
		}
	}
	else
	{
		if (nToolType == sRealCurve.nToolType)
		{
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(sRealCurve.pButton[nToolType]), TRUE);
		}
	}
	
	gtk_curve_widget_set_current_operation(&realCurveWidget, nToolType);
}

void onToggledBtnHisScale(GtkWidget *widget, gpointer data)
{
	int nType=GPOINTER_TO_INT(data);
	if (nType>=eCurveOpEnd)
	{
		return;
	}
	
	toolBarMgr(nType);
}

void onBtnClickZoom100(GtkWidget *widget, gpointer data)
{
	gtk_curve_widget_canvas_zoom_100(&realCurveWidget);
	
	gtk_curve_widget_frame_redraw(&realCurveWidget);
}

void onToggledCheckBtnNumeral(GtkToggleButton *togglebutton, gpointer data)
{
	gboolean bCheck=data;//gtk_toggle_button_get_active(widget);
	gtk_curve_widget_canvas_zoom_100(&realCurveWidget);
	if (bCheck)
	{
		gtk_curve_widget_set_data_scale(&realCurveWidget, eCalcAuto, 0, 0);
	}
	else
	{
		gtk_curve_widget_set_data_scale(&realCurveWidget, eCalcScale, 0, 0);
	}
	
	gtk_curve_widget_frame_redraw(&realCurveWidget);
}

void onToggledCheckBtnMark(GtkToggleButton *widget, gpointer data)
{
	gboolean bCheck=gtk_toggle_button_get_active(widget);

	if (bCheck)
	{
		gtk_curve_widget_enable(&realCurveWidget, TRUE, eEnableCurvesMark);
	}
	else
	{
		gtk_curve_widget_enable(&realCurveWidget, FALSE, eEnableCurvesMark);
	}
	
	gtk_curve_widget_canvas_redraw(&realCurveWidget);
}

void ExitRealCurveShell(GtkWidget *widget,   gpointer data)
{
	int crt_n = (gint) data ;
	gtk_widget_destroy(GTK_WIDGET(real_curve_main_w[crt_n])); 
}

void DestoryRealCurveShell(GtkWidget *widget,   gpointer data)
{
	int crt_n = (gint) data ;
	
	if( crt_n < 0 || crt_n >= MAX_CRT_NUMBER )
		return ; 
	
	g_message("\n quit realcurve !!!!! \n");
	
	if (real_curve_timer_id != 0)
	{
		g_source_remove(real_curve_timer_id);
		real_curve_timer_id  = 0 ;
	}
	
	real_curve_main_w[crt_n] = NULL ;

	nRealCurveRef--;

	return ;
}

gboolean updateRealData(gpointer data)
{
	gint nIndex, i;
	gdouble dCurTime, dStartTime;
	gint nNextPt;
	gint nCurveCount;

	GCurve* pCurve;
	POINTER pt;
	POINTER* pPtr;
	DMS_COMMON pdt;

	gint nPtId;

	gdouble Y_end, Y_start;
	gint nBufLen;

	int crt_n=GPOINTER_TO_INT(data);
	if (crt_n < 0 || crt_n > MAX_CRT_NUMBER)
	{
		return FALSE;
	}

	nCurveCount=gtk_curve_widget_get_curve_count(&realCurveWidget);
	if (nCurveCount <= 0)
	{
		return FALSE;
	}

	dCurTime=shm_real_curve->nTime;
	dStartTime=mktime(&sRealCurve.startTime);

	nBufLen=(gint)(dCurTime-dStartTime);
	if (nBufLen <= 0)
	{
		for (nIndex=0; nIndex < nCurveCount; nIndex++)
		{
			pCurve=gtk_curve_widget_get_curve(&realCurveWidget, nIndex);
			if (pCurve != NULL)
			{
				pt.stn_id=GPOINTER_TO_INT(gtk_curve_get_user_data(pCurve, 0));
				pt.dev_id=GPOINTER_TO_INT(gtk_curve_get_user_data(pCurve, 1));
				pt.data_type=GPOINTER_TO_INT(gtk_curve_get_user_data(pCurve, 2));
				pt.pt_id=GPOINTER_TO_INT(gtk_curve_get_user_data(pCurve, 3));
				
				if ( ReadEntryById(&pt, &pdt) != -1)
				{
					if(pdt.point.data_type==ANA_TYPE)
					{
						Y_end  = pdt.var.anlg.eng_para.HI_RANGE;	
						Y_start	= pdt.var.anlg.eng_para.LO_RANGE;
						if ( Y_start > 0 )
							Y_start = 0.0;
					}
					else
					{
						Y_end = 1.0;
						Y_start=0.0;
					}
				}
				else
				{
					Y_start=Y_end=0.0;
				}

				gtk_curve_set_boundary(pCurve, TRUE, Y_start, Y_end);
			}
		}

		gtk_curve_widget_canvas_redraw(&realCurveWidget);
		return TRUE;
	}

	nBufLen=nBufLen<DATA_NUM? nBufLen:DATA_NUM;

	nNextPt=shm_real_curve->nextpt;
	for (nIndex=0; nIndex < nCurveCount; nIndex++)
	{
		pCurve=gtk_curve_widget_get_curve(&realCurveWidget, nIndex);
		if (pCurve != NULL)
		{
			pt.stn_id=GPOINTER_TO_INT(gtk_curve_get_user_data(pCurve, 0));
			pt.dev_id=GPOINTER_TO_INT(gtk_curve_get_user_data(pCurve, 1));
			pt.data_type=GPOINTER_TO_INT(gtk_curve_get_user_data(pCurve, 2));
			pt.pt_id=GPOINTER_TO_INT(gtk_curve_get_user_data(pCurve, 3));
			
			if ( ReadEntryById(&pt, &pdt) != -1)
			{
				if(pdt.point.data_type==ANA_TYPE)
				{
					Y_end  = pdt.var.anlg.eng_para.HI_RANGE;	
					Y_start	= pdt.var.anlg.eng_para.LO_RANGE;
					if ( Y_start > 0 )
						Y_start = 0.0;
				}
				else
				{
					Y_end = 1.0;
					Y_start=0.0;
				}
			}
			else
			{
				Y_start=Y_end=0.0;
			}
			
			nPtId=-1;
			for (i=0; i<RealCCurvePt.curve_num;i++)
			{
				pPtr=&RealCCurvePt.buffer[i];
				if (pPtr->stn_id == pt.stn_id && pPtr->dev_id == pt.dev_id && pPtr->data_type == pt.data_type
					&& pPtr->pt_id == pt.pt_id)
				{
					nPtId=i;
					break;
				}
			}
			
			if (nPtId<0 || nPtId >= RealCCurvePt.curve_num)
			{
				continue;
			}
			
			if(pdt.point.data_type==ANA_TYPE)
			{
				for (i=0; i<nBufLen; i++)
				{
					dRealValue[i]=shm_real_curve->dat.fvalue[nPtId][(nNextPt-1-nBufLen+DATA_NUM+i)%(DATA_NUM)];
				}
			}
			else
			{
				for (i=0; i<nBufLen; i++)
				{
					dRealValue[i]=shm_real_curve->dat.status[nPtId][(nNextPt-1-nBufLen+DATA_NUM+i)%(DATA_NUM)];
				}
			}
			
			gtk_curve_set_data(pCurve, &dStartTime , dRealValue, DATA_NUM, 0, nBufLen, 1);
			gtk_curve_set_boundary(pCurve, TRUE, Y_start, Y_end);
		}
	}

	gtk_curve_widget_canvas_redraw(&realCurveWidget);

	return TRUE;
}

void ProgRealCurve (GtkWidget *parent ,int  crt_n )
{
	if (crt_n<0 ||crt_n>=MAX_CRT_NUM)
	{
		gdk_beep();
		return;
	}
	
	if (real_curve_main_w[crt_n]==(GtkWidget*)NULL)
	{
		if (nRealCurveRef >= 1)
		{
			gdk_beep(); 
			PopupMessageBox(parent , "OIX: Real Curve has opened !!");
			return;
		}
		nRealCurveRef++;

		real_curve_main_w[crt_n]  = CreateProgRealCurveShell (parent, crt_n);
		//gtk_window_set_position(GTK_WINDOW(real_curve_main_w[crt_n]) ,GTK_WIN_POS_CENTER_ON_PARENT);
		
		/*	hcl 老缓冲区
		real_curve_pt	= LinktoShareMem(SHM_WDQS_KEY_CHAR );
		if( real_curve_pt == (void *)-1)
		{
		perror("OIX ::in ProgTrendSet ERROR: real_curve_pt= -1!!!"); 
		return;
		}	
		*/
		
		//	UpdateAnaValList ( crt_n ); 
		UpdateAnaValList1(crt_n);
		UpdateAnaLstSel ( crt_n ) ;
		//初始化曲线参数
//		InitCurveParameter();
		//SendCanvasMsg(0  ,1 , NULL ,NULL    )  ; 
		if (real_curve_timer_id  !=0 )
			gtk_timeout_remove(real_curve_timer_id ); 
		
//		trenddatahis.next_pos = 0;

		gtk_widget_show_all(real_curve_main_w[crt_n] );
		
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(sRealCurve.pButton[sRealCurve.nToolType]), TRUE);
		gtk_curve_widget_set_current_operation(&realCurveWidget, eCurveOpPointer);
		gtk_curve_widget_set_data_scale(&realCurveWidget, eCalcAuto, 0, 0);
		gtk_curve_widget_frame_redraw(&realCurveWidget);

//		realCurvRedrawE(crt_n);
	}
	else
	{
		gtk_window_present(GTK_WINDOW(real_curve_main_w[crt_n]) );
	}
}


static GtkWidget *CreateProgRealCurveShell (GtkWidget *parent , int  crt_n )
{
	GtkWidget  *vbox,*vbox1, *hbox,*hbox1,*button_add_point,*button_del_point;
	GtkWidget  *frame;
	GtkWidget  *toolbar,*ana_name_list_scrollw,*sel_ana_list_scrollw;
	//	GtkWidget   *vrule;
	GtkWidget  *windows ;
	
	//	GtkWidget *image1 ;
	//	SHORT	i   ;
	//	int	shmid2;

	GtkCellRenderer *pRenderText;
	GtkListStore *pStore;
	GtkTreeSelection *pSelection;
	GtkTreeViewColumn *pColumn;

	gchar *pConvert=0;

	gchar strText[128];
	
#ifdef SUN_OS
	GdkScreen* pScreen;
#endif
	
	gtk_set_locale();
	/*   gtk_rc_add_default_file("./gtkrc.zh");  RM BY CHI*/
	
	windows=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	
//    gtk_window_set_default_size(GTK_WINDOW(windows),(int)(1280*xscale[crt_n]), (int)(1024*yscale[crt_n]));
#ifdef SUN_OS
	pScreen=gtk_window_get_screen(GTK_WINDOW(windows));
	if (pScreen != NULL)
	{
		gtk_window_set_default_size(GTK_WINDOW(windows), gdk_screen_get_width(pScreen), gdk_screen_get_height(pScreen));
	}
#else
	gtk_window_maximize(GTK_WINDOW(windows));
#endif
	
	gtk_window_set_title(GTK_WINDOW(windows), (pConvert=_toUtf8EX("MSG_REAL_TIME_CURVE")));    //"MSG_REAL_TIME_CURVE" 实时采集曲线
	G_SAFE_FREE(pConvert);

	gtk_window_set_transient_for(GTK_WINDOW(windows),GTK_WINDOW(parent));
	
	gtk_signal_connect(GTK_OBJECT(windows), "destroy", GTK_SIGNAL_FUNC( DestoryRealCurveShell), (gpointer)crt_n);

// 	gtk_signal_connect (GTK_OBJECT (windows), "key_release_event", GTK_SIGNAL_FUNC(real_frame_key_release_events), 
// 		(gpointer)crt_n);
	
	vbox=gtk_vbox_new(FALSE, 5);
	gtk_container_set_border_width(GTK_CONTAINER(vbox), 0);
	gtk_container_add(GTK_CONTAINER(windows), vbox);
	gtk_widget_show(vbox);
	
	toolbar =  CreateRealCurvToolbar(windows, crt_n);
	gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, TRUE, 0);
	gtk_widget_show(toolbar);    
	
	/************************** frame construct!! ******************************/
	hbox=gtk_hbox_new(FALSE, 3);    
	
	gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE,TRUE, 0);  
	gtk_widget_show(hbox);
	vbox1=gtk_vbox_new(FALSE, 3);
	
	gtk_widget_set_usize (vbox1, 250, -1);
	gtk_box_pack_start(GTK_BOX(hbox ), vbox1, FALSE, TRUE, 0);
	gtk_widget_show(vbox1);    
	
	/***************** list1 window code ***************************************/                     
	pRenderText = gtk_cell_renderer_text_new();
	
	pStore=gtk_list_store_new(eItemColEnd, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT);
	realUnselectList=gtk_tree_view_new_with_model(GTK_TREE_MODEL(pStore));
	pSelection=gtk_tree_view_get_selection(GTK_TREE_VIEW(realUnselectList));
	gtk_tree_selection_set_mode(pSelection, GTK_SELECTION_SINGLE);
	pColumn=gtk_tree_view_column_new_with_attributes((pConvert=_toUtf8EX("OIX_CSTR_UNSELECTED_DEVICE")), pRenderText, 
		"text", eItemName, NULL);
	G_SAFE_FREE(pConvert);
	gtk_tree_view_append_column (GTK_TREE_VIEW (realUnselectList), pColumn);
	
	g_object_unref(pStore);
	
	sel_ana_list_scrollw=gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (sel_ana_list_scrollw), GTK_SHADOW_ETCHED_IN);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (sel_ana_list_scrollw),
		GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
//	gtk_widget_set_usize (sel_ana_list_scrollw, -1, 750); 
	
	gtk_box_pack_start(GTK_BOX(vbox1), sel_ana_list_scrollw, TRUE, TRUE, 0);
	gtk_widget_show (sel_ana_list_scrollw); 
	
//	list_ana_val = gtk_list_new(); 
	
	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(sel_ana_list_scrollw), realUnselectList);  
	gtk_widget_show (realUnselectList); 
	
// 	gtk_signal_connect(GTK_OBJECT(list_ana_val), "selection_changed",
// 		GTK_SIGNAL_FUNC(SelRealValueListCB), (gpointer)ANA_LIST); 

	g_signal_connect(G_OBJECT(GTK_TREE_VIEW(realUnselectList)), "row_activated", G_CALLBACK(realListDbClick), 
		GINT_TO_POINTER(crt_n));

	/********************* two button code here !! *********************************/
	
	hbox1=gtk_hbox_new(FALSE, 15);
	gtk_widget_set_usize (GTK_WIDGET(hbox1),-1 , 30);
	gtk_box_pack_start(GTK_BOX(vbox1), hbox1 ,FALSE, FALSE, 0);
	gtk_widget_show(hbox1);
	
	
	button_add_point = gtk_button_new_with_label((pConvert=_toUtf8EX(OIX_CSTR_ADD))); 
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start (GTK_BOX (hbox1), button_add_point, TRUE, TRUE, 0);

	button_del_point = gtk_button_new_with_label((pConvert=_toUtf8EX( OIX_CSTR_DEL )));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start (GTK_BOX (hbox1), button_del_point, TRUE, TRUE, 0);     
	
	g_signal_connect(G_OBJECT(button_add_point), "clicked", G_CALLBACK(LstValAddCB), GINT_TO_POINTER(crt_n));
	g_signal_connect(G_OBJECT(button_del_point), "clicked", G_CALLBACK(LstValRemoveCB), GINT_TO_POINTER(crt_n));			      
	
	gtk_widget_show(button_add_point);
	gtk_widget_show(button_del_point);   	       
	
	/***************** list2 window code ***************************************/
	pStore=gtk_list_store_new(eItemColEnd, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT);
	realSelectList=gtk_tree_view_new_with_model(GTK_TREE_MODEL(pStore));
	pSelection=gtk_tree_view_get_selection(GTK_TREE_VIEW(realSelectList));
	gtk_tree_selection_set_mode(pSelection, GTK_SELECTION_SINGLE);
	pColumn=gtk_tree_view_column_new_with_attributes((pConvert=_toUtf8EX("OIX_CSTR_SELECTED_DEVICE")), pRenderText, 
		"text", eItemName, NULL);
	G_SAFE_FREE(pConvert);
	gtk_tree_view_append_column (GTK_TREE_VIEW (realSelectList), pColumn);
	
	g_object_unref(pStore);

	ana_name_list_scrollw=gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (ana_name_list_scrollw), GTK_SHADOW_ETCHED_IN);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (ana_name_list_scrollw),
		GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_widget_set_usize (GTK_WIDGET(ana_name_list_scrollw),-1 , 150);
	gtk_box_pack_start(GTK_BOX(vbox1), ana_name_list_scrollw, FALSE,FALSE, 0);
	gtk_widget_show (ana_name_list_scrollw); 
	
	/**************** gtk_widget_show (ana_name_list_scrollw);  ****************/
	
//	sel_ana_val_list = gtk_list_new ();    
	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(ana_name_list_scrollw),realSelectList );    
	gtk_widget_show (realSelectList);
	
// 	gtk_signal_connect(GTK_OBJECT(sel_ana_val_list), "selection_changed",
// 		GTK_SIGNAL_FUNC(SelRealValueListCB),SEL_ANA_LIST); 		        
	
	
	
	/***************** draw area code ***************************************/
	
	frame=gtk_curve_widget_create(windows, &realCurveWidget);
	gtk_box_pack_start(GTK_BOX(hbox ), frame, TRUE, TRUE, 0);
	
	gtk_curve_widget_enable(&realCurveWidget, TRUE, eEnableScale);
	gtk_curve_widget_enable(&realCurveWidget, TRUE, eEnableAutoSubTitle);
	sRealCurve.nToolType=eCurveOpPointer;

	sprintf(strText, "%s --- %s", 
		gtk_entry_get_text(GTK_ENTRY(starTmEntry)), gtk_entry_get_text(GTK_ENTRY(endTmEntry)));
	gtk_curve_widget_set_text(&realCurveWidget, strText, eTextTitle);

	return windows;
}

GtkWidget *
CreateRealCurvToolbar (GtkWidget * parent, gint crt_n)
{
	GtkWidget *box, *tempwid;  		
	GtkWidget *toolbar;   	
	GtkWidget *pBtnExit,*pBtnOk,*pBtnPointer,*pBtnHand,*pBtnZoomIn,*pBtnZoomOut,*pBtnZoomRect,*pBtnZoom100;
	GtkWidget *pCheckBtnNumeral, *pCheckBtnMark;

	struct tm when ;
	time_t now1;
	gchar strTime[32];
	gchar *pConvert=0;

	GtkWidget *radiobtnNum;
	GSList * radiobutton_group = NULL;
	GtkWidget *radiobtnPcent;

	GtkWidget  *sep1,*sep2 ;

	if( crt_n < 0 || crt_n >= MAX_CRT_NUMBER)
	{
		printf("CreateRealCurvToolbar Failed!");
		return NULL;
	}
	
	
	toolbar = gtk_handle_box_new ();
	
	box = gtk_hbox_new (FALSE, 4);
	gtk_container_add (GTK_CONTAINER (toolbar), box);
	gtk_container_border_width (GTK_CONTAINER (box), 5);
	gtk_widget_show (box);
	
	tempwid = gtk_label_new ((pConvert=_toUtf8EX( OIX_CSTR_START_TIME  )));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start (GTK_BOX (box), tempwid, FALSE, FALSE, 0);
	gtk_widget_show (tempwid);

	starTmEntry = gtk_entry_new_with_max_length (50); 	
	
	gtk_widget_set_usize (starTmEntry, 180, -1);
    
	gtk_entry_set_position(GTK_ENTRY (starTmEntry),0);
	gtk_entry_set_editable(GTK_ENTRY (starTmEntry),TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(starTmEntry),TRUE);
	gtk_box_pack_start (GTK_BOX (box), starTmEntry, FALSE, FALSE, 0);
	gtk_widget_show (starTmEntry); 
	
	/*  *************  end  time entry!!!!  ****************** */ 
	
    tempwid = gtk_label_new ((pConvert=_toUtf8EX( OIX_CSTR_END_TIME )));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start (GTK_BOX (box), tempwid, FALSE, FALSE, 0);
	gtk_widget_show (tempwid);

	endTmEntry = gtk_entry_new_with_max_length (50); 	
	
	gtk_widget_set_usize (endTmEntry, 180, -1);
	
	gtk_entry_set_position(GTK_ENTRY (endTmEntry),0);
	gtk_entry_set_editable(GTK_ENTRY (endTmEntry),TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(endTmEntry),TRUE);
	gtk_box_pack_start (GTK_BOX (box), endTmEntry, FALSE, FALSE, 0);
	gtk_widget_show (endTmEntry);
	
	time(&now1);  
	when = *localtime(&now1);

	when.tm_sec=0;
	sprintf(strTime,"%04d-%02d-%02d %02d:%02d",when.tm_year+1900,when.tm_mon+1,when.tm_mday,when.tm_hour,
			when.tm_min,when.tm_sec) ;
	gtk_entry_set_text(GTK_ENTRY(starTmEntry), strTime);

	sRealCurve.startTime=when;

	when.tm_mday++;
	when.tm_isdst=-1;
	mktime(&when);
	sprintf(strTime,"%04d-%02d-%02d %02d:%02d",when.tm_year+1900,when.tm_mon+1,when.tm_mday,when.tm_hour,
		when.tm_min,when.tm_sec) ;
	gtk_entry_set_text(GTK_ENTRY(endTmEntry), strTime);

	sRealCurve.endTime=when;
	
	/******************  display  button   *********************/
	/* hcl test*/
	pBtnOk= gtk_button_new_with_label((pConvert=_toUtf8EX(OIX_CSTR_OK)));
	G_SAFE_FREE(pConvert);

	gtk_box_pack_start (GTK_BOX (box), pBtnOk, FALSE, FALSE, 5); 
	gtk_widget_show (pBtnOk);
	
	gtk_signal_connect(GTK_OBJECT(pBtnOk), "clicked", GTK_SIGNAL_FUNC(onOkBtnClick), GINT_TO_POINTER(crt_n));

	pBtnPointer=gtk_toggle_button_new();
	sRealCurve.pButton[eCurveOpPointer]=pBtnPointer;

#if !(GTK_CHECK_VERSION(2, 6, 0))
	gtk_button_set_label(GTK_BUTTON(pBtnPointer), (pConvert=_toUtf8EX(OIX_CSTR_TRACK)));
	G_SAFE_FREE(pConvert);
#else
	gtk_button_set_image(GTK_BUTTON(pBtnPointer), create_pixmap (parent, "pointer1.png"));
#endif
	gtk_box_pack_start (GTK_BOX (box), pBtnPointer, FALSE, FALSE, 2); 
	gtk_widget_show (pBtnPointer);	
	gtk_signal_connect(GTK_OBJECT(pBtnPointer), "toggled", GTK_SIGNAL_FUNC(onToggledBtnHisScale), 
		GINT_TO_POINTER(eCurveOpPointer));

	pBtnHand= gtk_toggle_button_new();
	sRealCurve.pButton[eCurveOpHand]=pBtnHand;
#if !(GTK_CHECK_VERSION(2, 6, 0))
	gtk_button_set_label(GTK_BUTTON(pBtnHand), (pConvert=_toUtf8EX(OIX_CSTR_HAND)));
	G_SAFE_FREE(pConvert);
#else
	gtk_button_set_image(GTK_BUTTON(pBtnHand), create_pixmap (parent, "hand.png"));
#endif
	
	gtk_box_pack_start (GTK_BOX (box), pBtnHand, FALSE, FALSE, 2); 
	gtk_widget_show (pBtnHand);	
	gtk_signal_connect(GTK_OBJECT(pBtnHand), "toggled", GTK_SIGNAL_FUNC(onToggledBtnHisScale), 
		GINT_TO_POINTER(eCurveOpHand));

	pBtnZoomIn= gtk_toggle_button_new();
	sRealCurve.pButton[eCurveOpZoomIn]=pBtnZoomIn;
#if !(GTK_CHECK_VERSION(2, 6, 0))
	gtk_button_set_label(GTK_BUTTON(pBtnZoomIn), (pConvert=_toUtf8EX(OIX_CSTR_ZOOM_IN)));
	G_SAFE_FREE(pConvert);
#else
	gtk_button_set_image(GTK_BUTTON(pBtnZoomIn), create_pixmap (parent, "zoomin.png"));
#endif
	
	gtk_box_pack_start (GTK_BOX (box), pBtnZoomIn, FALSE, FALSE, 2); 
	gtk_widget_show (pBtnZoomIn);	
	gtk_signal_connect(GTK_OBJECT(pBtnZoomIn), "toggled", GTK_SIGNAL_FUNC(onToggledBtnHisScale),
		GINT_TO_POINTER(eCurveOpZoomIn));
	
	pBtnZoomOut= gtk_toggle_button_new();
	sRealCurve.pButton[eCurveOpZoomOut]=pBtnZoomOut;
#if !(GTK_CHECK_VERSION(2, 6, 0))
	gtk_button_set_label(GTK_BUTTON(pBtnZoomOut), (pConvert=_toUtf8EX(OIX_CSTR_ZOOM_OUT)));
	G_SAFE_FREE(pConvert);
#else
	gtk_button_set_image(GTK_BUTTON(pBtnZoomOut), create_pixmap (parent, "zoomout.png"));
#endif
	
	gtk_box_pack_start (GTK_BOX (box), pBtnZoomOut, FALSE, FALSE, 2); 
	gtk_widget_show (pBtnZoomOut);	
	gtk_signal_connect(GTK_OBJECT(pBtnZoomOut), "toggled", GTK_SIGNAL_FUNC(onToggledBtnHisScale), 
		GINT_TO_POINTER(eCurveOpZoomOut));

	pBtnZoomRect= gtk_toggle_button_new();
	sRealCurve.pButton[eCurveOpZoomRect]=pBtnZoomRect;
#if !(GTK_CHECK_VERSION(2, 6, 0))
	gtk_button_set_label(GTK_BUTTON(pBtnZoomRect), (pConvert=_toUtf8EX(OIX_CSTR_ZOOM_RECT)));
	G_SAFE_FREE(pConvert);
#else
	gtk_button_set_image(GTK_BUTTON(pBtnZoomRect), create_pixmap (parent, "zoomrect.png"));
#endif
	
	gtk_box_pack_start (GTK_BOX (box), pBtnZoomRect, FALSE, FALSE, 2); 
	gtk_widget_show (pBtnZoomRect);	
	gtk_signal_connect(GTK_OBJECT(pBtnZoomRect), "toggled", GTK_SIGNAL_FUNC(onToggledBtnHisScale),
		GINT_TO_POINTER(eCurveOpZoomRect));

	pBtnZoom100= gtk_button_new();
#if !(GTK_CHECK_VERSION(2, 6, 0))
	gtk_button_set_label(GTK_BUTTON(pBtnZoom100), (pConvert=_toUtf8EX(OIX_CSTR_ZOOM_FIT)));
	G_SAFE_FREE(pConvert);
#else
	gtk_button_set_image(GTK_BUTTON(pBtnZoom100), create_pixmap (parent, "zoomfit.png"));
#endif
	
	gtk_box_pack_start (GTK_BOX (box), pBtnZoom100, FALSE, FALSE, 2); 
	gtk_widget_show (pBtnZoom100);	
	gtk_signal_connect(GTK_OBJECT(pBtnZoom100), "clicked", GTK_SIGNAL_FUNC(onBtnClickZoom100), NULL);

// 	pCheckBtnNumeral=gtk_check_button_new_with_label((pConvert=_toUtf8EX("OIX_CSTR_NUMERICAL")));
// 	G_SAFE_FREE(pConvert);
// 	gtk_box_pack_start(GTK_BOX(box), pCheckBtnNumeral, FALSE, FALSE, 2);
// 	gtk_widget_show(pCheckBtnNumeral);
// 	g_signal_connect(G_OBJECT(pCheckBtnNumeral), "toggled", G_CALLBACK(onToggledCheckBtnNumeral), NULL);

	
	sep1 = gtk_separator_tool_item_new();
	gtk_box_pack_start(GTK_BOX(box), sep1, FALSE, FALSE, 2);

	radiobtnNum = gtk_radio_button_new_with_mnemonic (NULL, pConvert=_toUtf8EX(OIX_CSTR_NUMERICAL));
	G_SAFE_FREE(pConvert);
	gtk_widget_show (radiobtnNum);
		gtk_box_pack_start(GTK_BOX(box), radiobtnNum, FALSE, FALSE, 2);
	gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobtnNum), radiobutton_group);
	radiobutton_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobtnNum));
	g_signal_connect ((gpointer) radiobtnNum, "toggled",G_CALLBACK (onToggledCheckBtnNumeral),1);	

	radiobtnPcent = gtk_radio_button_new_with_mnemonic (NULL, pConvert=_toUtf8EX(OIX_CSTR_PERCENT));
	G_SAFE_FREE(pConvert);
	gtk_widget_show (radiobtnPcent);
	gtk_box_pack_start(GTK_BOX(box), radiobtnPcent, FALSE, FALSE, 2);
	gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobtnPcent), radiobutton_group);
	radiobutton_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobtnPcent));
	g_signal_connect ((gpointer) radiobtnPcent, "toggled",G_CALLBACK (onToggledCheckBtnNumeral),0);
	
	sep2 = gtk_separator_tool_item_new();
	gtk_box_pack_start(GTK_BOX(box), sep2, FALSE, FALSE, 2);

	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radiobtnNum),TRUE) ;

	pCheckBtnMark=gtk_check_button_new_with_label((pConvert=_toUtf8EX(OIX_CSTR_MARK)));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start(GTK_BOX(box), pCheckBtnMark, FALSE, FALSE, 2);
	gtk_widget_show(pCheckBtnMark);
	g_signal_connect(G_OBJECT(pCheckBtnMark), "toggled", G_CALLBACK(onToggledCheckBtnMark), NULL);
	
    pBtnExit = gtk_button_new_with_label((pConvert=_toUtf8EX( OIX_CSTR_EXIT)));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_end (GTK_BOX (box), pBtnExit, FALSE, FALSE, 0); 
	gtk_widget_show (pBtnExit);
	
	gtk_signal_connect(GTK_OBJECT(pBtnExit), "clicked", GTK_SIGNAL_FUNC(ExitRealCurveShell), GINT_TO_POINTER(crt_n));
	
	
	
	
	/***********************************************************************/
	
	
	return (toolbar);
}




#endif

GdkCursor* createCursor(GtkWidget* pParent, const gchar *filename, gboolean bCenter)
{
	GdkPixbuf* pPixBuf=NULL;
	GdkCursor* pCursor=NULL;
	gint x=0,y=0;
	
	pPixBuf = create_pixbuf(filename);

	if (pPixBuf == NULL)
	{
		return gdk_cursor_new(GDK_CROSS);
	}

	if (bCenter)
	{
		x=gdk_pixbuf_get_width(pPixBuf)/2;
		y=gdk_pixbuf_get_height(pPixBuf)/2;
	}

	pCursor=gdk_cursor_new_from_pixbuf(gtk_widget_get_display(pParent), pPixBuf, x, y);

	return pCursor;
}


int _s_to_i(const char *str)
{
	int num, i;
	num = *str - '0';
	i   = 1;
	
	while (str[i] && isdigit(str[i]))
		num = num * 10 + str[i++] - '0';
	
	return num;
}


