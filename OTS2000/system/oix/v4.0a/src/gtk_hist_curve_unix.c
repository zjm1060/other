
  
/*-----------------------------------------------------------------------

	 
	   Created by chi.hailong
	   Create  Date : 2005.11.26
	   Last Update  : 2006.6.11
	   
		 
---------------------------------------------------------------------*/



#include <stdio.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <time.h>

#ifdef SUN_OS
#include 	<pthread.h>
#include 	<sys/shm.h>
#endif


#include 	<signal.h>
#include	"../inc/oix.h"
#include "../inc/gtk_curve_widget.h"

/********************* gtk_hist_curve.h ************************/

#define		MAX_INTERVAL_NUM	2   	 /*  interval num  : 1's / 1'min /5'min /15'min /1'hour */
//#define		SEL_ANA_LIST		0
//#define		ANA_LIST		1
//#define		STAR_T			0
//#define		END_T			1
//#define		MAX_POINTER_NUM		5000      /*  The number of points which will draw curve */
//#define		CANVAS_MAX_POINTER	900
#define		MAX_CURVE_NUM		6

#define 	NUM_THREADS 		5
#define 	SLEEP_TIME 		10
//#define		X_ORGIN			50		/*  margin  of right down  **/
//#define		Y_ORGIN			50
//#define		X2_ORGIN		50
//#define		Y2_ORGIN		50	
#define		IDLE_FLAG		0
#define		LOCK_FLAG		1

//#define	ROW_MAJOR_NUM		10
//#define ROW_MINOR_NUM		4
//#define COLUMN_MINOR_NUM	6
#define TIME_INTERVAL_MIN	15		//显示总分钟数
#define TIME_INTERVAL_HOUR	24		//显示总小时数

gchar *interval[] = {OIX_CSTR_1_SEC , OIX_CSTR_1_MIN , OIX_CSTR_5_MIN , OIX_CSTR_15_MIN , OIX_CSTR_1_HOUR };

GtkWidget 	*hist_curve_main_w[MAX_CRT_NUMBER];

GtkWidget 	*starTmEntry,*endTmEntry,*list_ana_val,*sel_ana_val_list/*,   *scroll_win */;
enum 		T_Interval{A_SEC1,A_MIN1,FIV_MIN1,FIFTEEN_MIN1,A_HOUR1};

SHM_HDB data_curve,*shm_curve_addr ;   

#ifdef SUN_OS
pthread_t 	tid[NUM_THREADS];      /* array of thread IDs */
#endif

gint    curve_timer_id = 0;

short   COLOR_GROUP[MAX_CURVE_NUM] = {GREEN1,YLW1,WHITE,RED1,PINK1,CHOCLT1} ;

enum {eStnName=0, eStnId, eStnColNum};
enum {eDevName=0, eDevId, eDevColNum};
enum {eDataTypeName=0, eDataTypeId, eDataTypeColNum};
enum {eTimeIntervalName=0, eTimeIntervalType, eTimeIntervalColNum};
enum {eHisName, eHisStnId, eHisDevId, eHisDataType, eHisPtId, eHisColNum};

gdouble dHisValue[MAX_SELECT_REC];

GtkWidget *menuStn ,*menuDev ,*menuDataType,*menuInterval;

/********************** end ********************************/
extern char *_toUtf8(char *c);
extern void	PopupMessageBox (GtkWidget *parent,char msg[] );
extern char *_toUtf8EX(char *c);
void *FetchDataFromSrv(void *);     /* thread routine */
void WriteShareMem (gint nInterval, gint crt_n);
gboolean getTimeFromFormat(struct tm* pTime, const gchar* strTime, int add_num, enum T_Interval type);
int MakeFormatTime(struct tm  *  , char *  ,int   , enum  T_Interval, gboolean bShowAll) ;
gboolean DestoryHistCurveShell(GtkWidget *widget,   gpointer data);

/****** **                static	variable    *******/
gboolean exit_flag = FALSE;

WDQS_DY seldatahis ;

int nHisCurveRef=0;

GtkCurveWidget hisCurveWidget;

typedef struct _sHisQueryData
{
	gint nIntervalType;		//时间间隔类型
	WDQS_DY hisData;		//查询点

	struct tm tmStart;		//历史数据查询起始时间
	struct tm tmEnd;		//历史数据查询结束时间

	GtkWidget* pToolBtn[eCurveOpEnd];
	int nToolType;
}SHisQueryData;

SHisQueryData sHisQueryData;

gboolean setQueryData(int crt_n);

#ifdef WINDOWS_OS
typedef HANDLE pid_t;
#define	kill(run_pid, SIGNAL)	TerminateProcess(run_pid, 0)
#endif

pid_t hProcess;

GtkWidget *CreateHisCurvToolbar (GtkWidget * parent, int crt_n);
GtkWidget *CreateHisScaleToolbar (GtkWidget * parent, int crt_n);
GtkWidget *CreateProgHisCurveShell (GtkWidget *parent , int  crt_n );

void popupErrorMsgBox(GtkWidget* pParent, gchar* strMsg);
void popupInfoMsgBox(GtkWidget* pParent, gchar* strMsg);

extern GtkWidget* create_pixmap (GtkWidget *widget, const gchar *filename);

int     InitShareMemory_S(int crt_n )
{
#ifdef  WINDOWS_OS
	HANDLE Hthread	 ; 
#endif	
#ifdef SUN_OS	
	int res;
#endif
	if (get_hdb_shm()==FALSE)
	{
		printf("can't get shared memory\n");
		return FALSE;
	}
	
	shm_curve_addr= shm_hdb_addr;
	
	for(;;){
		
		if(shm_curve_addr->data_curve[crt_n].lockflag == IDLE_FLAG)	break;
		
		 
		MySleep(10) ; 
	}  
	
	
	shm_curve_addr->data_curve[crt_n].lockflag=LOCK_FLAG  ;    
	shm_curve_addr->data_curve[crt_n].querytype=0;
	shm_curve_addr->data_curve[crt_n].num = 0 ;
	shm_curve_addr->data_curve[crt_n].queryflag = 1 ;
	shm_curve_addr->data_curve[crt_n].lockflag=IDLE_FLAG    ; 
	
	
	/* ********  here will create  a thread    ******** */               
	printf(" \n ^_^  Here will create   thread_ !!!!!()\n");     
#ifdef SUN_OS	
	res =   pthread_create(&tid[0], NULL, FetchDataFromSrv, (void *)SLEEP_TIME);
	if (res == -1  ) 
	{	    
		printf ("Oix :Error when create thread res:d% !!!  \n",res);
		return(FALSE);
	}		
	else 
		printf("Oix : reporting that  thread  have create successfully\n" );	    

#endif	

#ifdef  WINDOWS_OS
	Hthread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)FetchDataFromSrv, (void *)(SLEEP_TIME) , 0, NULL) ; 
	if(Hthread!=NULL)
		printf("Create WriteDb Success!\n");
	else
		printf("Create WriteDb failed!!! error \n");
#endif
	return TRUE;
}    

gint getCurHisDataType()
{
	GtkTreeModel* pModel;
	GtkTreeIter it;
	
	gint nDataType=-1;
	
	pModel=gtk_combo_box_get_model(GTK_COMBO_BOX(menuDataType));
	gtk_combo_box_get_active_iter(GTK_COMBO_BOX(menuDataType), &it);
	gtk_tree_model_get(pModel, &it, eDataTypeId, &nDataType, -1);
	
	return nDataType;
}

gint getCurHisIntervalType()
{
	GtkTreeModel* pModel;
	GtkTreeIter it;
	
	gint nInterval=-1;
	
	pModel=gtk_combo_box_get_model(GTK_COMBO_BOX(menuInterval));
	gtk_combo_box_get_active_iter(GTK_COMBO_BOX(menuInterval), &it);
	gtk_tree_model_get(pModel, &it, eTimeIntervalType, &nInterval, -1);
	
	return nInterval;
}

gint getHisInterval(gint nIntervalType)
{
	gint nInterval;

	switch (nIntervalType)
	{
	case A_SEC1:
		nInterval=TIME_INTERVAL_MIN*60;
		break;
	case A_MIN1:
		nInterval=TIME_INTERVAL_HOUR*60;
		break;
	}

	return nInterval;
}

gint getHisIntervalSec(gint nType)
{
	gint nInterval=0;
	
	switch (nType)
	{
	case A_SEC1:
		nInterval=TIME_INTERVAL_MIN*60;
		break;
	case A_MIN1:
		nInterval=TIME_INTERVAL_HOUR*3600;
		break;
	}
	
	return nInterval;
}

gint getHisMinIntervalSec(gint nType)
{
	gint nInterval=0;

	switch (nType)
	{
	case A_SEC1:
		nInterval=30;
		break;
	case A_MIN1:
		nInterval=30*60;
		break;
	}
	
	return nInterval;
}

gint getHisStep(gint nIntervalType)
{
	gint nStep;
	
	switch (nIntervalType)
	{
	case A_SEC1:
		nStep=1;
		break;
	case A_MIN1:
		nStep=60;
		break;
	}
	
	return nStep;
}

void UpdateAnaValLst(int crt_n,int stn_id,int dev_id)
{
	short		i;
	POINTER		ptr;
	DMS_COMMON	pdt;
	char strLongName[128];
	int entry_num;
	int nInterval;
	
	GtkListStore* pStore;
	GtkTreeIter it;
	gchar *pConvert=0;

	if (crt_n<0 || crt_n>=MAX_CRT_NUM)
	{
		return;
	}

	nInterval=getCurHisIntervalType();
	if (nInterval<0)
	{
		return;
	}
	
	pStore=GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list_ana_val)));
	gtk_list_store_clear(pStore);
	
	ptr.stn_id	= stn_id;
	ptr.dev_id	= dev_id;
	ptr.data_type = getCurHisDataType();
	
	if (GetEntryNumById((UCHAR)ptr.stn_id,(UCHAR)ptr.dev_id,(UCHAR)ptr.data_type, (GULONG*)&entry_num) == -1)
	{
		return;
	}

	for (i=0; i<entry_num; i++)
	{
		ptr.pt_id=i ;
		if ( ReadEntryById ( &ptr, &pdt ) == -1)
		{
			sprintf(strLongName, "No pt: %d.%d.%d.%d", ptr.stn_id, ptr.dev_id, ptr.data_type, ptr.pt_id);
			printf("OIX: TrendName No this point in the database file !!!\n");
		}
		else
		{
			if (GetEntryLongnameById (&ptr,strLongName) == -1)
			{
				continue;
			}

			gtk_list_store_append(pStore, &it);
			gtk_list_store_set(pStore, &it, eHisName,  (pConvert=_toUtf8(strLongName)), eHisStnId, ptr.stn_id, 
				eHisDevId, ptr.dev_id, eHisDataType, ptr.data_type, eHisPtId, ptr.pt_id, -1);
			G_SAFE_FREE(pConvert);//dzy1017
		}
	}	
}

void addItemToHisAddList(POINTER* ptr, GtkListStore* pStore)
{
	DMS_COMMON	pdt;

//	char longname[128];
	char strLongName[128];

	char strStn[STATION_LONGNAME_SIZE],strDev[GROUP_LONGNAME_SIZE];
	char strEntryName[ENTRY_LONGNAME_SIZE];	
	
	GtkTreeIter it;
	gchar *pConvert = 0;
	if (ReadEntryById(ptr, &pdt ) == -1)
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
	gtk_list_store_set(pStore, &it, eHisName,  (pConvert=_toUtf8(strLongName)), eHisStnId, 
		ptr->stn_id, eHisDevId, ptr->dev_id, eHisDataType, ptr->data_type, eHisPtId, ptr->pt_id, -1);
	G_SAFE_FREE(pConvert);//dzy1017
}

void	ListValAddCB (GtkWidget *w, gpointer data)
{
	int i;
	   
	POINTER* ptr;
	   
	GtkTreeSelection* pSelect;
	GtkTreeModel* pModel;
	GtkTreeIter it;
	   
	GtkListStore* pStore;
	   
	POINTER pt;
	gint nStnId, nDevId, nDataType, nPtId;

	int crt_n = GPOINTER_TO_INT(data) ;
	if( crt_n < 0 || crt_n >= MAX_CRT_NUMBER)
		return ; 

	pSelect=gtk_tree_view_get_selection (GTK_TREE_VIEW (list_ana_val));
	pModel=gtk_tree_view_get_model(GTK_TREE_VIEW(list_ana_val));
	if (!gtk_tree_selection_get_selected (pSelect, &pModel, &it))
	{
		gdk_beep();
		printf("History Curve Add Point: No Selected!\n");
		return;
	}

	if ( seldatahis.next_pos >= MAX_CURVE_NUM)
	{
		gdk_beep();
		PopupMessageBox(hist_curve_main_w[crt_n],"\nOIX: The number of curver must less than 6!!!\n");
		return;
	}

	gtk_tree_model_get (pModel, &it, eHisStnId, &nStnId, eHisDevId, &nDevId, eHisDataType, &nDataType, 
		eHisPtId, &nPtId, -1);

	pt.stn_id=(UCHAR)nStnId;
	pt.dev_id=(UCHAR)nDevId;
	pt.data_type=(UCHAR)nDataType;
	pt.pt_id=(USHORT)nPtId;
	   
	for(i=0; i<seldatahis.next_pos;i++)
	{
		if((seldatahis.buffer[i].stn_id  == pt.stn_id) &&
			(seldatahis.buffer[i].dev_id  == pt.dev_id) &&
			(seldatahis.buffer[i].data_type  == pt.data_type) && 
			(seldatahis.buffer[i].pt_id  == pt.pt_id))
		{
			gdk_beep();
			return ;
		}
	}

	pStore=GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(sel_ana_val_list)));

	seldatahis.buffer[seldatahis.next_pos].stn_id = pt.stn_id;
	seldatahis.buffer[seldatahis.next_pos].dev_id = pt.dev_id;
	seldatahis.buffer[seldatahis.next_pos].data_type	= pt.data_type;
	seldatahis.buffer[seldatahis.next_pos].pt_id = pt.pt_id;
	ptr=&seldatahis.buffer[seldatahis.next_pos++];

	addItemToHisAddList(ptr, pStore);
}


void	ListValRemoveCB (GtkWidget *w, gpointer user_data)
{
	int i;
	int	crt_n;
	
	GtkTreeSelection* pSelect;
	GtkTreeModel* pModel;
	GtkTreeIter it;
	
	GtkListStore* pStore;
	
	POINTER pt;

	gint nStnId, nDevId, nDataType, nPtId;
	
	crt_n=GPOINTER_TO_INT(user_data);
	if( crt_n < 0 || crt_n >= MAX_CRT_NUMBER )
		return;
	
	pSelect=gtk_tree_view_get_selection (GTK_TREE_VIEW (sel_ana_val_list));
	
	pModel=gtk_tree_view_get_model(GTK_TREE_VIEW(sel_ana_val_list));
	
	if (!gtk_tree_selection_get_selected (pSelect, &pModel, &it))
	{
		gdk_beep();
		printf("History Curve:No Selected!\n");
		return;
	}
	
	gtk_tree_model_get (pModel, &it, eHisStnId, &nStnId, eHisDevId, &nDevId, eHisDataType, 
		&nDataType, eHisPtId, &nPtId, -1);

	pt.stn_id=(UCHAR)nStnId;
	pt.dev_id=(UCHAR)nDevId;
	pt.data_type=(UCHAR)nDataType;
	pt.pt_id=(USHORT)nPtId;
	
	pStore=GTK_LIST_STORE(pModel);
	gtk_list_store_remove(pStore, &it);
	
	for (i=0; i<seldatahis.next_pos; i++)
	{
		if (seldatahis.buffer[i].stn_id == pt.stn_id && seldatahis.buffer[i].dev_id == pt.dev_id 
			&& seldatahis.buffer[i].data_type == pt.data_type && seldatahis.buffer[i].pt_id == pt.pt_id)
		{
			break;
		}
	}
	
	if (i >= seldatahis.next_pos)
	{
		gdk_beep();
		printf("SGZY Remove:No Find Point in Data!\n");
		return;
	}
	
	for (; i<seldatahis.next_pos-1;i++)
	{
		seldatahis.buffer[i]=seldatahis.buffer[i+1];
	}
	
	seldatahis.next_pos--;
}

gint transform(gdouble dAxis, gdouble dMin, gdouble dMax, gint nLength, gboolean bRev)
{
	if (bRev)
	{
		return nLength-(gint)((dAxis-dMin)*nLength/(dMax-dMin));
	}
	else
	{
		return (gint)((dAxis-dMin)*nLength/(dMax-dMin));
	}
}

gboolean addHisCurve(POINTER* ptr, GdkColor color, gint crt_n, int nCurId)
{
	GCurve* pCurve;

//	int 	i;
	DMS_COMMON	 pdt;
//	char long_str[POINT_LONGNAME_SIZE];
	float Y_start, Y_end;	
	char LongNamerStr1[128];
	char *str, *str1, *str2;

	char strStn[STATION_LONGNAME_SIZE],strDev[GROUP_LONGNAME_SIZE];
	char strEntryName[ENTRY_LONGNAME_SIZE];	

	gint nInterval;

	gdouble dXValue;
	gchar *pConvert = 0;
	if (!gtk_curve_widget_check_add_curve(&hisCurveWidget))
	{
		return FALSE;
	}

	nInterval=getHisInterval(sHisQueryData.nIntervalType);

	pCurve=gtk_curve_create(MAX_SELECT_REC, eModeSec);

	gtk_curve_set_color(pCurve, &color);

	dXValue=mktime(&sHisQueryData.tmStart);

	if ( ReadEntryById(ptr, &pdt) == -1)
	{
		sprintf(LongNamerStr1, "%s: %d.%d.%d.%d", (pConvert=_toUtf8EX("OIX_CSTR_SGZY_NO_SUCH_POINT")), 
			ptr->stn_id, ptr->dev_id, ptr->data_type, ptr->pt_id);
		G_SAFE_FREE(pConvert);//dzy1017
	}
	else
	{
		GetStnCommentById(pdt.point.stn_id, strStn);
		GetGroupCommentById(pdt.point.stn_id, (UCHAR)pdt.point.dev_id, strDev);
		GetEntryLongnameById (&pdt.point, strEntryName);
		sprintf(LongNamerStr1, "%s - %s - %s", (str=_toUtf8(strStn)), (str1=_toUtf8(strDev)), 
			(str2=_toUtf8(strEntryName)));
		G_SAFE_FREE(str);//dzy1017
		G_SAFE_FREE(str1);//dzy1017
		G_SAFE_FREE(str2);//dzy1017

// 		GetGroupLongnameById( ptr->stn_id, ptr->dev_id, long_str);
// 		GetEntryLongnameById (ptr,LongNamerStr1);
// 		sprintf(LongNamerStr1, "%s%s", _toUtf8(long_str), _toUtf8(LongNamerStr1));
				
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

		if (ptr->data_type!=ANA_TYPE)
		{
			gtk_curve_set_type(pCurve, eCurveLadder);
		}
		
// 		for (i=0; i<data_curve.data_curve[crt_n].seg[nCurId].rscount; i++)
// 		{
// 			dHisValue[i]=data_curve.data_curve[crt_n].seg[nCurId].data[i];
// 		}
// 		
// 		gtk_curve_set_data(pCurve, &dXValue , dHisValue, MAX_SELECT_REC, 0,
// 			data_curve.data_curve[crt_n].seg[nCurId].rscount, getHisStep(sHisQueryData.nIntervalType));
// 		gtk_curve_set_boundary(pCurve, TRUE, Y_start, Y_end);
	}

	gtk_curve_set_text(pCurve, LongNamerStr1);

	gtk_curve_set_user_data(pCurve, GINT_TO_POINTER(ptr->stn_id), 0);
	gtk_curve_set_user_data(pCurve, GINT_TO_POINTER(ptr->dev_id), 1);
	gtk_curve_set_user_data(pCurve, GINT_TO_POINTER(ptr->data_type), 2);
	gtk_curve_set_user_data(pCurve, GINT_TO_POINTER(ptr->pt_id), 3);

	gtk_curve_widget_add_curve(&hisCurveWidget, pCurve);

	return TRUE;
}

gboolean adjustHisCurve(gpointer usr_data)
{
	gint nIndex, i;
	GCurve* pCurve;
	POINTER* pPtr;
	gint nPtId;
	POINTER pt;
	DMS_COMMON pdt;
	float Y_start, Y_end;
	gint nInterval;
	
	gdouble dXValue;
	gint nNumCurve;
	gchar *pConvert=0;
	gint crt_n=GPOINTER_TO_INT(usr_data);
	if (crt_n<0 || crt_n>=MAX_CRT_NUM)
	{
		return FALSE;
	}
	
	nNumCurve=gtk_curve_widget_get_curve_count(&hisCurveWidget);
	if (nNumCurve <= 0)
	{
		return FALSE;
	}

	if (data_curve.data_curve[crt_n].finishedFlag != 1)
	{
		if (data_curve.connflag_curve != 1)
		{
			gtk_curve_widget_set_text(&hisCurveWidget, (pConvert=_toUtf8EX("OIX_CSTR_NO_CONNECTED")), eTextMessage);
			gtk_curve_widget_enable(&hisCurveWidget, TRUE, eEnableMessageClose);
		}
		else
		{
			gtk_curve_widget_set_text(&hisCurveWidget, (pConvert=_toUtf8EX("OIX_CSTR_QUERYING")), eTextMessage);
			gtk_curve_widget_enable(&hisCurveWidget, FALSE, eEnableMessageClose);
		}
		G_SAFE_FREE(pConvert);//dzy1017
		for (nIndex=0; nIndex<nNumCurve; nIndex++)
		{
			pCurve=gtk_curve_widget_get_curve(&hisCurveWidget, nIndex);
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
				
				gtk_curve_set_boundary(pCurve, TRUE, Y_start, Y_end);
			}
		}

		gtk_curve_widget_canvas_redraw(&hisCurveWidget);

		return TRUE;
	}

	gtk_curve_widget_enable(&hisCurveWidget, FALSE, eEnableMessage);
	
	dXValue=mktime(&sHisQueryData.tmStart);

	nInterval=getHisInterval(sHisQueryData.nIntervalType);
	
	for (nIndex=0; nIndex<gtk_curve_widget_get_curve_count(&hisCurveWidget); nIndex++)
	{
		pCurve=gtk_curve_widget_get_curve(&hisCurveWidget, nIndex);
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

			for (i=0; i<sHisQueryData.hisData.next_pos;i++)
			{
				pPtr=&sHisQueryData.hisData.buffer[i];
				if (pPtr->stn_id == pt.stn_id && pPtr->dev_id == pt.dev_id && pPtr->data_type == pt.data_type
					&& pPtr->pt_id == pt.pt_id)
				{
					nPtId=i;
					break;
				}
			}

			if (nPtId<0 || nPtId >= sHisQueryData.hisData.next_pos)
			{
				break;
			}
			
			for (i=0; i<data_curve.data_curve[crt_n].seg[nPtId].rscount; i++)
			{
				dHisValue[i]=data_curve.data_curve[crt_n].seg[nPtId].data[i];
			}
			
			gtk_curve_set_data(pCurve, &dXValue , dHisValue, MAX_SELECT_REC, 0, 
				data_curve.data_curve[crt_n].seg[nPtId].rscount, getHisStep(sHisQueryData.nIntervalType));
			gtk_curve_set_boundary(pCurve, TRUE, Y_start, Y_end);
		}
	}
	
	gtk_curve_widget_frame_redraw(&hisCurveWidget);
	
	return TRUE;
}

void	DisplayCB (GtkWidget* pWidget, gpointer data)
{
	int i;
	POINTER* ptr;
	gchar *pConvert=0;
	int crt_n=GPOINTER_TO_INT(data);
	if (crt_n<0 || crt_n>=MAX_CRT_NUM)
	{
		gdk_beep();
		return;
	}

	if (curve_timer_id  !=0 )
	{
		g_source_remove(curve_timer_id );
		curve_timer_id=0;
	}

	gtk_curve_widget_clear_all_curve(&hisCurveWidget);
	
	if ( seldatahis.next_pos <= 0)
	{
		gdk_beep();
		popupInfoMsgBox(hist_curve_main_w[crt_n],"OIX: Must first select one curve at least!");
		return;
	}

	if (!setQueryData(crt_n))		//保存查询状态
	{
		return;
	}

	WriteShareMem (sHisQueryData.nIntervalType, crt_n);

	gtk_curve_widget_canvas_zoom_100(&hisCurveWidget);

	for (i=0; i<seldatahis.next_pos; i++)
	{
		ptr=&seldatahis.buffer[i];
		addHisCurve(ptr, pixels[crt_n][COLOR_GROUP[i]], crt_n, i);
	}

	curve_timer_id     =g_timeout_add (1000, (GSourceFunc)adjustHisCurve, GINT_TO_POINTER(crt_n));
	adjustHisCurve(GINT_TO_POINTER(crt_n));

	gtk_curve_widget_set_text(&hisCurveWidget, (pConvert=_toUtf8EX("OIX_CSTR_QUERYING")), eTextMessage);
	G_SAFE_FREE(pConvert);//dzy1017

	gtk_curve_widget_enable(&hisCurveWidget, TRUE, eEnableMessage);
	gtk_curve_widget_enable(&hisCurveWidget, FALSE, eEnableMessageClose);
  	
	gtk_curve_widget_frame_redraw(&hisCurveWidget);


	printf("@@@@  in DisplayCB\n");
}

gboolean getTimeFromFormat(struct tm* pTime, const gchar* strTime, int add_num, enum T_Interval type)
{
	time_t  result;

	struct tm sTime;
	memset(&sTime, 0, sizeof(struct tm));
	
	switch (type)
	{
	case A_SEC1:
		{
			sscanf(strTime , "%4d-%2d-%2d %2d:%2d:%2d" , &sTime.tm_year,&sTime.tm_mon,&sTime.tm_mday,
				&sTime.tm_hour, &sTime.tm_min, &sTime.tm_sec);
			sTime.tm_sec += add_num;
		}
		break;
	case A_MIN1:
		{
			sTime.tm_min += add_num;
			sscanf(strTime , "%4d-%2d-%2d %2d:%2d" , &sTime.tm_year,&sTime.tm_mon,&sTime.tm_mday,&sTime.tm_hour,
				&sTime.tm_min);
		}
		break;
	case FIV_MIN1:
		{
			sTime.tm_min  +=  add_num * 5;
			sscanf(strTime , "%4d-%2d-%2d %2d:%2d" , &sTime.tm_year,&sTime.tm_mon,&sTime.tm_mday,&sTime.tm_hour);
		}
		break;
	case FIFTEEN_MIN1:
		{
			sTime.tm_min +=  add_num * 15;
			sscanf(strTime , "%4d-%2d-%2d %2d:%2d" , &sTime.tm_year,&sTime.tm_mon,&sTime.tm_mday,&sTime.tm_hour);
		}
		break;
	case A_HOUR1:
		{
			sTime.tm_hour +=  add_num  ;
			sscanf(strTime , "%4d-%2d-%2d %2d:%2d" , &sTime.tm_year,&sTime.tm_mon,&sTime.tm_mday);
		}
		break;
	default:
		return FALSE;
	}

	sTime.tm_year-=1900;
	sTime.tm_mon--;
	
	sTime.tm_isdst = -1 ;

	result = mktime(&sTime);
	if (result == (time_t) -1)
	{
		return FALSE;	//Error
	}

	memcpy(pTime, &sTime, sizeof(struct tm));

	return TRUE;
};


/**************************************************************************************************              
s--return time string ; add_num--want add num , type--kind         

************************************************************************************************* */
int MakeFormatTime(struct tm  *Tm , char *s ,int add_num , enum T_Interval type, gboolean bShowAll)
{
	time_t  result;
	gchar *pConvert=0;
	switch(type)
	{
	case A_SEC1:
		Tm->tm_sec += add_num;
		break;
	case A_MIN1:
		Tm->tm_min += add_num; 
		break;
	case FIV_MIN1:
		Tm->tm_min  +=  add_num * 5; 
		break;
	case FIFTEEN_MIN1:
		Tm->tm_min +=  add_num * 15;
		break;
	case A_HOUR1:
		Tm->tm_hour +=  add_num  ;
		break;
	default:
		break; 	
	}

	Tm->tm_isdst = -1 ;
	result = mktime(Tm);
	if (result == (time_t) -1)
	{
		sprintf(s, "%s","YYYY-MM-DD HH:MM:SS\n");
		return -1;	//Error
	}

	if (bShowAll)
	{
		sprintf(s,"%04d-%02d-%02d %02d:%02d:%02d",Tm->tm_year+1900,Tm->tm_mon+1,Tm->tm_mday,Tm->tm_hour,
			Tm->tm_min,Tm->tm_sec) ;
	}
	else
	{
		switch (type)
		{
		case A_SEC1:
			{
				sprintf(s,"%04d-%02d-%02d %02d:%02d:%02d",Tm->tm_year+1900,Tm->tm_mon+1,Tm->tm_mday,Tm->tm_hour,
					Tm->tm_min,Tm->tm_sec) ;
			}
			break;
		case A_MIN1:
			{
				Tm->tm_sec=0;
				
				sprintf(s,"%04d-%02d-%02d %02d:%02d",Tm->tm_year+1900,Tm->tm_mon+1,Tm->tm_mday,Tm->tm_hour,
					Tm->tm_min) ;
			}
			break;
		case FIV_MIN1:
			{
				Tm->tm_sec=0;
				Tm->tm_min=(Tm->tm_min/5)*5;
				
				sprintf(s,"%04d-%02d-%02d %02d:%02d",Tm->tm_year+1900,Tm->tm_mon+1,Tm->tm_mday,Tm->tm_hour,
					Tm->tm_min) ;
			}
			break;
		case FIFTEEN_MIN1:
			{
				Tm->tm_sec=0;
				Tm->tm_min=(Tm->tm_min/15)*15;
				
				sprintf(s,"%04d-%02d-%02d %02d:%02d",Tm->tm_year+1900,Tm->tm_mon+1,Tm->tm_mday,Tm->tm_hour,
					Tm->tm_min) ;
			}
			break;
		case A_HOUR1:
			{
				Tm->tm_sec=0;
				Tm->tm_min=0;
				
				sprintf(s,"%04d-%02d-%02d %02d%s",Tm->tm_year+1900,Tm->tm_mon+1,Tm->tm_mday,Tm->tm_hour,
					(pConvert=_toUtf8EX(OIX_CSTR_TIME_HOUR))) ;
				G_SAFE_FREE(pConvert);//dzy1017
			}
			break;
		default:
			break;
		}
	}

	return 0;
}

void ChangeInterval(GtkWidget * widget, gpointer data)
{
	struct tm when ;
	time_t now1;
	char strTime[22];

	gint nType =  getCurHisIntervalType();

	time(&now1);  
	when = *localtime(&now1);

	MakeFormatTime(&when, strTime, 0, nType, FALSE);
	gtk_entry_set_text(GTK_ENTRY(starTmEntry), strTime);

	switch(nType)
	{
	case A_SEC1:
		when.tm_min+=TIME_INTERVAL_MIN;
		break;
	case A_MIN1:
		when.tm_hour+=TIME_INTERVAL_HOUR;
		break;
	case FIV_MIN1:
		break;
	case FIFTEEN_MIN1:
		break;
	case A_HOUR1:
		break;
	default:
		break; 	
	}

	mktime(&when);
	MakeFormatTime(&when, strTime, 0, nType, FALSE);
	gtk_entry_set_text(GTK_ENTRY(endTmEntry), strTime);
}

gboolean setQueryData(gint crt_n)
{
	gchar strStartTime[24];
	gchar strEndTime[24];
	gchar strText[128];
	gchar *ptr, *ptr1;
	gchar *pConvert=0;

	gdouble dStart,dEnd;

	struct tm tmStart, tmEnd;
	time_t res;

	gint nType,nSec,nMinSec;

	gboolean bRet;

	nType=getCurHisIntervalType();

	strcpy(strStartTime, gtk_entry_get_text(GTK_ENTRY(starTmEntry)));
	strcpy(strEndTime, gtk_entry_get_text(GTK_ENTRY(endTmEntry)));

	bRet=getTimeFromFormat(&tmStart, strStartTime, 0, nType);
	if (!bRet)
	{
		gdk_beep();
		popupErrorMsgBox(hist_curve_main_w[crt_n], (pConvert=_toUtf8EX("OIX_CSTR_START_TIME_FORMAT_ERROR")));
		G_SAFE_FREE(pConvert);//dzy1017

		return bRet;
	}
	MakeFormatTime(&tmStart, strStartTime, 0, nType, FALSE);
	gtk_entry_set_text(GTK_ENTRY(starTmEntry), strStartTime);

	bRet=getTimeFromFormat(&tmEnd, strEndTime, 0, nType);
	if (!bRet)
	{
		gdk_beep();
		popupErrorMsgBox(hist_curve_main_w[crt_n], (pConvert=_toUtf8EX("OIX_CSTR_END_TIME_FORMAT_ERROR")));
		G_SAFE_FREE(pConvert);//dzy1017

		return bRet;
	}

	res=mktime(&tmEnd)-mktime(&tmStart);

	nSec=getHisIntervalSec(nType);
	nMinSec=getHisMinIntervalSec(nType);
	if (res<nMinSec)
	{
		res=nMinSec;
	}
	else if (res > nSec)
	{
		res=nSec;
	}

	tmEnd=tmStart;
	tmEnd.tm_sec+=res;

	MakeFormatTime(&tmEnd, strEndTime, 0, nType, FALSE);
	gtk_entry_set_text(GTK_ENTRY(endTmEntry), strEndTime);

	sHisQueryData.nIntervalType=nType;

	sHisQueryData.tmStart=tmStart;
	sHisQueryData.tmEnd=tmEnd;
	
	sprintf(strText, "%s --- %s", strStartTime, strEndTime);
	gtk_curve_widget_set_text(&hisCurveWidget, strText, eTextTitle);

	sprintf(strText, "%s:%s", (ptr=_toUtf8EX("OIX_CSTR_TIME_INTERVAL")),
		(ptr1=_toUtf8EX(interval[sHisQueryData.nIntervalType-A_SEC1])));
	G_SAFE_FREE(ptr);//dzy1017
	G_SAFE_FREE(ptr1);//dzy1017
	gtk_curve_widget_set_text(&hisCurveWidget, strText, eTextSubtitle);

	memcpy(&sHisQueryData.hisData, &seldatahis, sizeof(WDQS_DY));

	dStart=mktime(&sHisQueryData.tmStart);
	dEnd=mktime(&sHisQueryData.tmEnd);

	gtk_curve_widget_set_time_scale(&hisCurveWidget, dStart, dEnd);

	return TRUE;
}

void ExitHistCurveShell(GtkWidget *widget,   gpointer data)
{
	int crt_n = (gint) data ;
	if (crt_n<0 || crt_n>=MAX_CRT_NUM)
	{
		return;
	}

	gtk_widget_destroy(hist_curve_main_w[crt_n]);
}

gboolean DestoryHistCurveShell(GtkWidget *widget,   gpointer data)
{
	int crt_n = (gint) data ;
	if( crt_n < 0 || crt_n >= MAX_CRT_NUMBER )
		return TRUE; 
 
	g_message("\n quit histcurve !!!!! \n");
	exit_flag = TRUE;

	if (curve_timer_id != 0)
	{
		g_source_remove(curve_timer_id ); 
		curve_timer_id=0;
	}

	hist_curve_main_w[crt_n] =NULL;

// 	if (hProcess != (pid_t)-1)
// 	{
// 		kill(hProcess, SIGTERM);
// 	}

	nHisCurveRef--;

	return TRUE;
}

void ProgHisCurve (GtkWidget *parent ,int  crt_n )
{
	if (crt_n<0 || crt_n>=MAX_CRT_NUM)
	{
		gdk_beep(); 
		return;
	}

    if (hist_curve_main_w[crt_n]==(GtkWidget*)NULL)
    {
		if (nHisCurveRef >= 1)
		{
			gdk_beep(); 
			PopupMessageBox(parent , "OIX: History Curve has opened !!");
			return;
		}
		nHisCurveRef++;

		if (InitShareMemory_S(crt_n )== FALSE)
		{    
			gdk_beep(); 
			PopupMessageBox(parent , "OIX: Open share Mem error!");

			nHisCurveRef--;
			return ; 
		} 

		seldatahis.next_pos=0;

		hist_curve_main_w[crt_n]  = CreateProgHisCurveShell(parent, crt_n) ;
//		gtk_window_set_position((GtkWindow *)hist_curve_main_w[crt_n] ,GTK_WIN_POS_CENTER_ON_PARENT);
		
//		gtk_widget_show(hist_curve_main_w[crt_n] );
//		UpdateAnaValLst ( crt_n ); 
//		seldatahis.next_pos  = trenddatahis.next_pos = 0;

		gtk_combo_box_set_active(GTK_COMBO_BOX(menuStn), 0);
    }
	else
	{
		gtk_window_present((GtkWindow *)hist_curve_main_w[crt_n]);
	}

//	runProcess("curvescan", crt_n);
}

void hisListDbClick(GtkTreeView *tree_view,GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data)
{
	ListValAddCB(NULL, user_data);
}

GtkWidget *CreateProgHisCurveShell (GtkWidget *parent , int  crt_n )
{
	GtkWidget  *vbox,*vbox1, *hbox,*hbox1,*button_add_point,*button_del_point;
	GtkWidget  *toolbar, *scaleToolBar,*ana_name_list_scrollw,*sel_ana_list_scrollw;
	GtkWidget  *windows ;
	GtkWidget *pFrame;

	GtkListStore *pStoreAna, *pStoreAnaSel;
	GtkTreeSelection *pSelection;
	GtkCellRenderer *pRenderText;
	GtkTreeViewColumn *pColumn;

	gchar *ptr;

	/* gtk_set_locale();
	gtk_rc_add_default_file("./gtkrc.zh");  */

#ifdef SUN_OS
	GdkScreen* pScreen;
#endif
	
	windows=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	/*     gtk_widget_realize(GTK_WIDGET(windows)) ;  */
	
//	gtk_window_set_default_size(GTK_WINDOW( windows),1224, 1000);
#ifdef SUN_OS
	pScreen=gtk_window_get_screen(GTK_WINDOW(windows));
	if (pScreen != NULL)
	{
		gtk_window_set_default_size(GTK_WINDOW(windows), gdk_screen_get_width(pScreen), gdk_screen_get_height(pScreen));
	}
#else
	gtk_window_maximize(GTK_WINDOW(windows));
#endif
	
	gtk_window_set_title(GTK_WINDOW(windows), (ptr=_toUtf8EX(OIX_CSTR_CURVE_VIEW)));
	G_SAFE_FREE(ptr);//dzy1017

	gtk_window_set_transient_for(GTK_WINDOW(windows),GTK_WINDOW(parent));
	
	g_signal_connect(G_OBJECT(windows), "destroy", G_CALLBACK(DestoryHistCurveShell), GINT_TO_POINTER(crt_n));
	
	vbox=gtk_vbox_new(FALSE, 5);
	gtk_container_set_border_width(GTK_CONTAINER(vbox), 0);
	gtk_container_add(GTK_CONTAINER(windows), vbox);
	gtk_widget_show(vbox);
	
	toolbar =  CreateHisCurvToolbar(windows, crt_n);
	gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, TRUE, 0);
	gtk_widget_show(toolbar);

	scaleToolBar=CreateHisScaleToolbar(windows, crt_n);
	gtk_box_pack_start(GTK_BOX(vbox), scaleToolBar, FALSE, TRUE, 0);
	gtk_widget_show(scaleToolBar);
	
    /************************** frame construct!! ******************************/
	
	hbox=gtk_hbox_new(FALSE, 3);    
	gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, 0);
	gtk_widget_show(hbox);

	vbox1=gtk_vbox_new(FALSE, 3);
	gtk_widget_set_usize (vbox1, 250, -1);
	gtk_box_pack_start(GTK_BOX(hbox), vbox1, FALSE, TRUE, 0);
	gtk_widget_show(vbox1);
	
	/***************** list1 window code ***************************************/                     
	
	pRenderText = gtk_cell_renderer_text_new();
	
	pStoreAna=gtk_list_store_new(eHisColNum, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT);
	list_ana_val=gtk_tree_view_new_with_model(GTK_TREE_MODEL(pStoreAna));
	pSelection=gtk_tree_view_get_selection(GTK_TREE_VIEW(list_ana_val));
	gtk_tree_selection_set_mode(pSelection, GTK_SELECTION_SINGLE);
	pColumn=gtk_tree_view_column_new_with_attributes((ptr=_toUtf8EX("OIX_CSTR_UNSELECTED_DEVICE")), pRenderText, 
		"text", eHisName, NULL);
	G_SAFE_FREE(ptr);//dzy1017

	gtk_tree_view_append_column (GTK_TREE_VIEW (list_ana_val), pColumn);
	
	g_object_unref(pStoreAna);
    
    ana_name_list_scrollw=gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (ana_name_list_scrollw), GTK_SHADOW_ETCHED_IN);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (ana_name_list_scrollw), GTK_POLICY_AUTOMATIC, 
		GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(vbox1), ana_name_list_scrollw, TRUE, TRUE, 0);
    gtk_widget_show (ana_name_list_scrollw); 
	
	gtk_container_add (GTK_CONTAINER (ana_name_list_scrollw), list_ana_val);
    gtk_widget_show (list_ana_val); 
	
	g_signal_connect(G_OBJECT(GTK_TREE_VIEW(list_ana_val)), "row_activated", G_CALLBACK(hisListDbClick), 
		GINT_TO_POINTER(crt_n));
	
	/********************* two button code here !! *********************************/
	
	hbox1=gtk_hbox_new(FALSE, 15);
	gtk_widget_set_usize (GTK_WIDGET(hbox1),-1 , 30);
	gtk_box_pack_start(GTK_BOX(vbox1), hbox1 ,FALSE, FALSE, 0);
	gtk_widget_show(hbox1);
	
	button_add_point = gtk_button_new_with_label((ptr=_toUtf8EX(OIX_CSTR_ADD))); 
	G_SAFE_FREE(ptr);//dzy1017

	gtk_box_pack_start (GTK_BOX (hbox1), button_add_point, TRUE, TRUE, 0);
	button_del_point = gtk_button_new_with_label((ptr=_toUtf8EX( OIX_CSTR_DEL)));
	G_SAFE_FREE(ptr);//dzy1017

	gtk_box_pack_start (GTK_BOX (hbox1), button_del_point, TRUE, TRUE, 0);     
	
	g_signal_connect(G_OBJECT(button_add_point), "clicked", G_CALLBACK(ListValAddCB), GINT_TO_POINTER(crt_n));
   	g_signal_connect(G_OBJECT(button_del_point), "clicked", G_CALLBACK(ListValRemoveCB), GINT_TO_POINTER(crt_n));			      
	
   	gtk_widget_show(button_add_point);
	gtk_widget_show(button_del_point);   
	
	/***************** list2 window code ***************************************/ 

	pStoreAnaSel=gtk_list_store_new(eHisColNum, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT);
	sel_ana_val_list=gtk_tree_view_new_with_model(GTK_TREE_MODEL(pStoreAnaSel));
	pSelection=gtk_tree_view_get_selection(GTK_TREE_VIEW(sel_ana_val_list));
	gtk_tree_selection_set_mode(pSelection, GTK_SELECTION_SINGLE);
	pColumn=gtk_tree_view_column_new_with_attributes((ptr=_toUtf8EX("OIX_CSTR_SELECTED_DEVICE")), pRenderText, 
		"text", eHisName, NULL);
	G_SAFE_FREE(ptr);//dzy1017
	gtk_tree_view_append_column (GTK_TREE_VIEW (sel_ana_val_list), pColumn);
	
	g_object_unref(pStoreAnaSel);
    
    sel_ana_list_scrollw=gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (sel_ana_list_scrollw), GTK_SHADOW_ETCHED_IN);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (sel_ana_list_scrollw), GTK_POLICY_AUTOMATIC, 
		GTK_POLICY_AUTOMATIC);
	gtk_widget_set_usize (GTK_WIDGET(sel_ana_list_scrollw),-1 , 150);
    gtk_box_pack_start(GTK_BOX(vbox1), sel_ana_list_scrollw, FALSE, FALSE, 0);
    gtk_widget_show (sel_ana_list_scrollw); 
	
	gtk_container_add (GTK_CONTAINER (sel_ana_list_scrollw), sel_ana_val_list);
    gtk_widget_show (sel_ana_val_list); 
	
    /***************************** draw area code ***************************************/     

	pFrame=gtk_curve_widget_create(windows, &hisCurveWidget);
	gtk_box_pack_start(GTK_BOX(hbox ), pFrame, TRUE, TRUE, 0);
	
	gtk_curve_widget_enable(&hisCurveWidget, TRUE, eEnableScale);
	sHisQueryData.nToolType=eCurveOpPointer;
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(sHisQueryData.pToolBtn[sHisQueryData.nToolType]), TRUE);
//	gtk_curve_widget_set_text(&hisCurveWidget, _toUtf8EX("OIX_CSTR_SGZY_PERCENTAGE"), eTextLabel);

	setQueryData(crt_n);

	gtk_widget_show_all(windows);

	gtk_curve_widget_set_current_operation(&hisCurveWidget, eCurveOpPointer);
	gtk_curve_widget_frame_redraw(&hisCurveWidget);

	return windows;
}

void WriteShareMem (gint nInterval, gint crt_n)
{     
    int i ;
	char  longname[100] ;
	POINT_NAME  pname; 

	struct tm sTime;

 	for(;;){
		
		if(shm_curve_addr->data_curve[crt_n].lockflag == IDLE_FLAG)	break;	
		MySleep(10) ; 
	} 

	shm_curve_addr->data_curve[crt_n].querytype = 0;
	shm_curve_addr->data_curve[crt_n].lockflag = LOCK_FLAG  ;    /* IDLE_FLAG */

	getTimeFromFormat(&sTime, gtk_entry_get_text(GTK_ENTRY(starTmEntry)), 0, nInterval);
	sprintf(shm_curve_addr->data_curve[crt_n].starttime, "%04d-%02d-%02d %02d:%02d:%02d" , sTime.tm_year+1900, 
		sTime.tm_mon+1, sTime.tm_mday, sTime.tm_hour, sTime.tm_min, sTime.tm_sec);

	getTimeFromFormat(&sTime, gtk_entry_get_text(GTK_ENTRY(endTmEntry)), 0, nInterval);
	sprintf(shm_curve_addr->data_curve[crt_n].endtime,  "%04d-%02d-%02d %02d:%02d:%02d" , sTime.tm_year+1900, 
		sTime.tm_mon+1, sTime.tm_mday, sTime.tm_hour, sTime.tm_min, sTime.tm_sec);

	for (i = 0 ; i < MIN(6,seldatahis.next_pos) ;i++)
	{
		if ( -1 ==  GetPtNameById( &seldatahis.buffer[i] ,(char*)&longname, &pname ))  
			continue ;
		sprintf(shm_curve_addr->data_curve[crt_n].seg[i].logicalname,longname);
		memset(&shm_curve_addr->data_curve[crt_n].seg[i].data, 0,  sizeof(float) * MAX_SELECT_REC );                         
	}
	shm_curve_addr->data_curve[crt_n].num = MIN(6,seldatahis.next_pos)  ; 
	shm_curve_addr->data_curve[crt_n].interval= (char)nInterval   /*ONE_MIN*/     ; 
	shm_curve_addr->data_curve[crt_n].queryflag=1;
	shm_curve_addr->data_curve[crt_n].finishedFlag=0;
	shm_curve_addr->data_curve[crt_n].lockflag= IDLE_FLAG;  	
}



void *FetchDataFromSrv(void *arg)
{

	unsigned char curid=0;
	exit_flag = FALSE;     
	while(  exit_flag == FALSE )   
	{  
        if(shm_curve_addr == NULL)
		{
			printf ("OIX: error when  copy shm_curve_addr to  data_curve !!!  ");	
			goto ExitThr;
		} 
		memcpy( &data_curve.data_curve,  &shm_curve_addr->data_curve , sizeof(HDB_CURVE)*MAX_CRT_NUM); 

		data_curve.connflag_curve=shm_curve_addr->connflag_curve;
		/*      printf("thread %d sleeping 5 seconds ...\n", thr_self() );   */
		MySleep(1000) ;  
		
	}      
ExitThr:  
#ifdef SUN_OS 	
	printf("\nthread %d return \n", pthread_self());
	pthread_exit(0);
#endif 
#ifdef WINDOWS_OS
	ExitThread(0) ; 
#endif 
	return (NULL);
}

void UpdateHistDevSelect(GtkComboBox* w, gpointer data)
{
	int	stn_id=-1,dev_id=-1;

	GtkTreeModel* pModel;
	GtkTreeIter it;

	int crt_n=GPOINTER_TO_INT(data);
	if( crt_n < 0 || crt_n >= MAX_CRT_NUMBER)
		return;

	pModel=gtk_combo_box_get_model(GTK_COMBO_BOX(menuStn));
	if (gtk_combo_box_get_active_iter(GTK_COMBO_BOX(menuStn), &it))
	{
		gtk_tree_model_get(pModel, &it, eStnId, &stn_id, -1);
	}
	
	pModel=gtk_combo_box_get_model(GTK_COMBO_BOX(menuDev));
	if (gtk_combo_box_get_active_iter(GTK_COMBO_BOX(menuDev), &it))
	{
		gtk_tree_model_get(pModel, &it, eDevId, &dev_id, -1);
	}
	
	UpdateAnaValLst (crt_n, stn_id, dev_id);
}

void CreateHistDevMenu(GtkComboBox* pComboBox, gpointer data) 
{
	char	dev_comment[GROUP_COMMENT_SIZE];
	int	stn_id;
	int	i,grp_num;
	
	GtkTreeModel* pModel;
	GtkListStore* pStore;
	GtkTreeIter it;

	char *ptr;

	gint crt_n=GPOINTER_TO_INT(data);
	if (crt_n<0 || crt_n>=MAX_CRT_NUM)
	{
		return;
	}

	gtk_combo_box_set_active(GTK_COMBO_BOX(menuDev), -1);

	pStore=GTK_LIST_STORE(gtk_combo_box_get_model(GTK_COMBO_BOX(menuDev)));
	gtk_list_store_clear(pStore);
	
	pModel=gtk_combo_box_get_model(GTK_COMBO_BOX(menuStn));
	if (!gtk_combo_box_get_active_iter(GTK_COMBO_BOX(menuStn), &it))
	{
		return;
	}

	gtk_tree_model_get(pModel, &it, eStnId, &stn_id, -1);

	if(-1 != GetGroupNumById((UCHAR)stn_id, &grp_num))
	{
		for(i=1; i<=grp_num; i++) 
		{  		
			if(-1==GetGroupCommentById((UCHAR)stn_id, (UCHAR)i, dev_comment))
				continue;

			if (strcmp(dev_comment, "") != 0)
			{
				gtk_list_store_append (pStore, &it);
				gtk_list_store_set (pStore, &it, eDevName, (ptr=_toUtf8(dev_comment)), eDevId, i, -1);
				G_SAFE_FREE(ptr);//dzy1017
			}
		}
	}
	
	gtk_combo_box_set_active(GTK_COMBO_BOX(menuDev), 0);
}

GtkWidget *CreateHisCurvToolbar (GtkWidget * parent, int crt_n)
{
	int	i,stn_num,data_type_num;

	GtkWidget *box, *tempwid;  	
	GtkWidget *toolbar;
	GtkWidget	*button_display,*button_exit;

	GtkListStore *pStoreStn, *pStoreDev, *pStoreDataType, *pStoreTime;
	GtkTreeIter it;
	
	GtkCellRenderer *pRenderStn, *pRenderDev, *pRenderDataType, *pRenderTime;
	
	char stn_comment[STATION_COMMENT_SIZE];
	char datatype_longname[RECORD_LONGNAME_SIZE];
	char *ptr;

	toolbar = gtk_handle_box_new ();
	
	box = gtk_hbox_new (FALSE, 4);
	gtk_container_add (GTK_CONTAINER (toolbar), box);
	gtk_container_border_width (GTK_CONTAINER (box), 5);
	gtk_widget_show (box);
	
	/***********************************************************************/
	//厂站
	tempwid = gtk_label_new ((ptr=_toUtf8EX(OIX_CSTR_STATION)));
	G_SAFE_FREE(ptr);//dzy1017

	gtk_box_pack_start (GTK_BOX (box), tempwid, FALSE, FALSE, 0);
	gtk_widget_show (tempwid);

	pStoreStn=gtk_list_store_new(eStnColNum, G_TYPE_STRING, G_TYPE_INT);
	
	GetStationNum(&stn_num);
	for(i=1; i<=stn_num; i++)
	{ 
		UCHAR stn_id;
		
		if(-1==GetStnIdByIndex(i, &stn_id))
			continue;
		
		if( GetStnCommentById((UCHAR)stn_id, stn_comment)==-1 )
			continue;
		if (strcmp(stn_comment, "") != 0)
		{
			gtk_list_store_append(pStoreStn, &it);
			gtk_list_store_set(pStoreStn, &it, eStnName,  (ptr=_toUtf8(stn_comment)), eStnId, stn_id, -1);
			G_SAFE_FREE(ptr);//dzy1017
		}
	}

	menuStn=gtk_combo_box_new_with_model(GTK_TREE_MODEL(pStoreStn));
	
	pRenderStn = gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(menuStn), pRenderStn, FALSE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(menuStn), pRenderStn, "text", eStnName, NULL);
	g_object_unref(pStoreStn);
	
	gtk_widget_set_usize(menuStn, 100, -1);
	gtk_box_pack_start(GTK_BOX(box), menuStn, FALSE, FALSE, 5);
	gtk_widget_show(menuStn);
	
	g_signal_connect (G_OBJECT(menuStn), "changed", G_CALLBACK(CreateHistDevMenu), GINT_TO_POINTER(crt_n));

	//设备
	tempwid = gtk_label_new ((ptr=_toUtf8EX(OIX_CSTR_ALM_DEV_FILTER)));
	G_SAFE_FREE(ptr);//dzy1017

	gtk_box_pack_start (GTK_BOX (box), tempwid, FALSE, FALSE, 0);
	gtk_widget_show (tempwid);
	
	pRenderDev = gtk_cell_renderer_text_new();
	
	pStoreDev=gtk_list_store_new(eDevColNum, G_TYPE_STRING, G_TYPE_INT);
	menuDev = gtk_combo_box_new_with_model(GTK_TREE_MODEL(pStoreDev));
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(menuDev), pRenderDev, FALSE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(menuDev), pRenderDev, "text", eDevName, NULL);
	g_object_unref(pStoreDev);
	
	gtk_widget_set_usize(menuDev, 130, -1);
	gtk_box_pack_start (GTK_BOX (box), menuDev, FALSE, FALSE, 5);
	gtk_widget_show (menuDev);
	
	g_signal_connect (G_OBJECT(menuDev), "changed", G_CALLBACK(UpdateHistDevSelect), GINT_TO_POINTER(crt_n));

	//数据类型
	pStoreDataType=gtk_list_store_new(eDataTypeColNum, G_TYPE_STRING, G_TYPE_INT);
	
	if(IsDevObjDisable())
		data_type_num=sizeof(recordlongname)/sizeof(recordlongname[0])-1;            
	else
		data_type_num=sizeof(recordlongname)/sizeof(recordlongname[0]);
	for(i=0;i<data_type_num;i++)
	{
		if(strcmp(recordlongname[i],"")!=0)
		{	
			if (GetRecLongnameByType((UCHAR)i, datatype_longname) == -1)
			{
				continue;
			}
			
			gtk_list_store_append(pStoreDataType, &it);
			gtk_list_store_set(pStoreDataType, &it, eDataTypeName, 
				(ptr=_toUtf8(datatype_longname)), eDataTypeId, i, -1);
			G_SAFE_FREE(ptr);//dzy1017
		}
	}

	pRenderDataType = gtk_cell_renderer_text_new();
	menuDataType= gtk_combo_box_new_with_model(GTK_TREE_MODEL(pStoreDataType));
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(menuDataType), pRenderDataType, FALSE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(menuDataType), pRenderDataType, "text", 0, NULL);
	g_object_unref(pStoreDataType);
	gtk_widget_set_usize(menuDataType, 100, -1);
	gtk_box_pack_start (GTK_BOX (box), menuDataType, FALSE, FALSE, 5);
	gtk_widget_show (menuDataType);
	
	gtk_combo_box_set_active(GTK_COMBO_BOX(menuDataType), 0);
	
	g_signal_connect(G_OBJECT(menuDataType), "changed", G_CALLBACK(UpdateHistDevSelect), GINT_TO_POINTER(crt_n));

	//时间间隔
	pStoreTime=gtk_list_store_new(eTimeIntervalColNum, G_TYPE_STRING, G_TYPE_INT);
	
	for(i=0; i< MAX_INTERVAL_NUM ; i++) 
	{
		gtk_list_store_append(pStoreTime, &it);
		gtk_list_store_set(pStoreTime, &it, eTimeIntervalName, 
			(ptr=_toUtf8EX(interval[i])), eTimeIntervalType, A_SEC1+i, -1);
		G_SAFE_FREE(ptr);//dzy1017
	}
	
	pRenderTime = gtk_cell_renderer_text_new();
	menuInterval= gtk_combo_box_new_with_model(GTK_TREE_MODEL(pStoreTime));
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(menuInterval), pRenderTime, FALSE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(menuInterval), pRenderTime, "text", 0, NULL);
	g_object_unref(pStoreTime);
	gtk_widget_set_usize(menuInterval, 70, -1);
	gtk_box_pack_start (GTK_BOX (box), menuInterval, FALSE, FALSE, 5);
	gtk_widget_show (menuInterval);
	
	gtk_combo_box_set_active(GTK_COMBO_BOX(menuInterval), 0);
	
	g_signal_connect(G_OBJECT(menuInterval), "changed", G_CALLBACK(ChangeInterval), GINT_TO_POINTER(crt_n));
	
	/*  *************  Time Interval  combobox  ****************** */
	tempwid = gtk_label_new ((ptr=_toUtf8EX( OIX_CSTR_START_TIME )));
	G_SAFE_FREE(ptr);//dzy1017

	gtk_box_pack_start (GTK_BOX (box), tempwid, FALSE, FALSE, 0);
	gtk_widget_show (tempwid);
	
	starTmEntry = gtk_entry_new_with_max_length (50);
	
	gtk_widget_set_usize (starTmEntry, 130, -1);
    
	gtk_entry_set_position(GTK_ENTRY (starTmEntry),0);
	gtk_entry_set_editable(GTK_ENTRY (starTmEntry),TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(starTmEntry),TRUE);
	gtk_box_pack_start (GTK_BOX (box), starTmEntry, FALSE, FALSE, 0);
	gtk_widget_show (starTmEntry); 
	
	/*  *************  end  time entry!!!!  ****************** */ 
	
	tempwid = gtk_label_new ((ptr=_toUtf8EX( OIX_CSTR_END_TIME )));
	G_SAFE_FREE(ptr);//dzy1017

	gtk_box_pack_start (GTK_BOX (box), tempwid, FALSE, FALSE, 0);
	gtk_widget_show (tempwid);
	
	endTmEntry = gtk_entry_new_with_max_length (50); 	
	
	gtk_widget_set_usize (endTmEntry, 180, -1);
	
	gtk_entry_set_position(GTK_ENTRY (endTmEntry),0);
	gtk_entry_set_editable(GTK_ENTRY (endTmEntry),TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(endTmEntry),TRUE);
	gtk_box_pack_start (GTK_BOX (box), endTmEntry, FALSE, FALSE, 0);
	gtk_widget_show (endTmEntry); 

	ChangeInterval(GTK_WIDGET(endTmEntry), NULL);
	
	/******************  display  button   *********************/
	
	button_display = gtk_button_new_with_label((ptr=_toUtf8EX(OIX_CSTR_DISPLAY)));
	G_SAFE_FREE(ptr);//dzy1017

	gtk_box_pack_start (GTK_BOX (box), button_display, FALSE, FALSE, 0);    	
	gtk_signal_connect(GTK_OBJECT(button_display), "clicked", GTK_SIGNAL_FUNC(DisplayCB), GINT_TO_POINTER(crt_n));
	
	gtk_widget_show(button_display);
	
	button_exit = gtk_button_new_with_label((ptr=_toUtf8EX(STR_QUIT)));
	G_SAFE_FREE(ptr);//dzy1017

	gtk_box_pack_end (GTK_BOX (box), button_exit, FALSE, FALSE, 0); 
	gtk_widget_show (button_exit);
	
	g_signal_connect(G_OBJECT(button_exit), "clicked", G_CALLBACK(ExitHistCurveShell),  GINT_TO_POINTER(crt_n));
	
	return (toolbar);
}

void toolBtnManager(gint nToolType)
{
	gint nType;
	
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sHisQueryData.pToolBtn[nToolType])))
	{
		if (nToolType != sHisQueryData.nToolType)
		{
			nType=sHisQueryData.nToolType;
			sHisQueryData.nToolType=nToolType;
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(sHisQueryData.pToolBtn[nType]), FALSE);
		}
	}
	else
	{
		if (nToolType == sHisQueryData.nToolType)
		{
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(sHisQueryData.pToolBtn[nToolType]), TRUE);
		}
	}
	
	gtk_curve_widget_set_current_operation(&hisCurveWidget, nToolType);
}

void onToggledBtnScale(GtkWidget *widget, gpointer data)
{
	int nType=GPOINTER_TO_INT(data);
	if (nType>=eCurveOpEnd)
	{
		return;
	}

	toolBtnManager(nType);
}

void onBtnClickScaleZoom100(GtkWidget *widget, gpointer data)
{
	gtk_curve_widget_canvas_zoom_100(&hisCurveWidget);

	gtk_curve_widget_frame_redraw(&hisCurveWidget);
}

void onToggledCheckBtnHisNumeral(GtkToggleButton *widget, gpointer data)
{
	gboolean bCheck=gtk_toggle_button_get_active(widget);
	gtk_curve_widget_canvas_zoom_100(&hisCurveWidget);
	if (bCheck)
	{
		gtk_curve_widget_set_data_scale(&hisCurveWidget, eCalcAuto, 0, 0);
	}
	else
	{
		gtk_curve_widget_set_data_scale(&hisCurveWidget, eCalcScale, 0, 0);
	}
	
	gtk_curve_widget_frame_redraw(&hisCurveWidget);
}

void onToggledCheckBtnHisMark(GtkToggleButton *widget, gpointer data)
{
	gboolean bCheck=gtk_toggle_button_get_active(widget);
	
	if (bCheck)
	{
		gtk_curve_widget_enable(&hisCurveWidget, TRUE, eEnableCurvesMark);
	}
	else
	{
		gtk_curve_widget_enable(&hisCurveWidget, FALSE, eEnableCurvesMark);
	}
	
	gtk_curve_widget_canvas_redraw(&hisCurveWidget);
}

GtkWidget *CreateHisScaleToolbar (GtkWidget * parent, int crt_n)
{
	GtkWidget *pToolBar,*box;
	GtkWidget *pBtnPointer,*pBtnHand,*pBtnZoomIn,*pBtnZoomOut,*pBtnZoomRect,*pBtnZoom100;
	GtkWidget *pCheckBtnNumeral, *pCheckBtnMark;
	gchar* ptr;

	if( crt_n < 0 || crt_n >= MAX_CRT_NUMBER)
	{
		printf("CreateHisScaleToolbar Failed!");
		return NULL;
	}

	pToolBar = gtk_handle_box_new ();
	
	box = gtk_hbox_new (FALSE, 4);
	gtk_container_add (GTK_CONTAINER (pToolBar), box);
	gtk_container_border_width (GTK_CONTAINER (box), 5);
	gtk_widget_show (box);

	pBtnPointer=gtk_toggle_button_new();
	sHisQueryData.pToolBtn[eCurveOpPointer]=pBtnPointer;
	
#if !(GTK_CHECK_VERSION(2, 6, 0))
	gtk_button_set_label(GTK_BUTTON(pBtnPointer), (ptr=_toUtf8EX("OIX_CSTR_TRACK")));
	G_SAFE_FREE(ptr);//dzy1017
#else
	gtk_button_set_image(GTK_BUTTON(pBtnPointer), create_pixmap (parent, "pointer1.png"));
#endif
	gtk_box_pack_start (GTK_BOX (box), pBtnPointer, FALSE, FALSE, 2); 
	gtk_widget_show (pBtnPointer);	
	gtk_signal_connect(GTK_OBJECT(pBtnPointer), "toggled", GTK_SIGNAL_FUNC(onToggledBtnScale), 
		GINT_TO_POINTER(eCurveOpPointer));
	
	pBtnHand= gtk_toggle_button_new();
	sHisQueryData.pToolBtn[eCurveOpHand]=pBtnHand;
#if !(GTK_CHECK_VERSION(2, 6, 0))
	gtk_button_set_label(GTK_BUTTON(pBtnHand), (ptr=_toUtf8EX("OIX_CSTR_HAND")));
	G_SAFE_FREE(ptr);//dzy1017
#else
	gtk_button_set_image(GTK_BUTTON(pBtnHand), create_pixmap (parent, "hand.png"));
#endif
	
	gtk_box_pack_start (GTK_BOX (box), pBtnHand, FALSE, FALSE, 2); 
	gtk_widget_show (pBtnHand);	
	gtk_signal_connect(GTK_OBJECT(pBtnHand), "toggled", GTK_SIGNAL_FUNC(onToggledBtnScale), 
		GINT_TO_POINTER(eCurveOpHand));
	
	pBtnZoomIn= gtk_toggle_button_new();
	sHisQueryData.pToolBtn[eCurveOpZoomIn]=pBtnZoomIn;
#if !(GTK_CHECK_VERSION(2, 6, 0))
	gtk_button_set_label(GTK_BUTTON(pBtnZoomIn), (ptr=_toUtf8EX("OIX_CSTR_ZOOM_IN")));
	G_SAFE_FREE(ptr);//dzy1017
#else
	gtk_button_set_image(GTK_BUTTON(pBtnZoomIn), create_pixmap (parent, "zoomin.png"));
#endif
	
	gtk_box_pack_start (GTK_BOX (box), pBtnZoomIn, FALSE, FALSE, 2); 
	gtk_widget_show (pBtnZoomIn);	
	gtk_signal_connect(GTK_OBJECT(pBtnZoomIn), "toggled", GTK_SIGNAL_FUNC(onToggledBtnScale),
		GINT_TO_POINTER(eCurveOpZoomIn));
	
	pBtnZoomOut= gtk_toggle_button_new();
	sHisQueryData.pToolBtn[eCurveOpZoomOut]=pBtnZoomOut;
#if !(GTK_CHECK_VERSION(2, 6, 0))
	gtk_button_set_label(GTK_BUTTON(pBtnZoomOut), (ptr=_toUtf8EX("OIX_CSTR_ZOOM_OUT")));
	G_SAFE_FREE(ptr);//dzy1017
#else
	gtk_button_set_image(GTK_BUTTON(pBtnZoomOut), create_pixmap (parent, "zoomout.png"));
#endif
	
	gtk_box_pack_start (GTK_BOX (box), pBtnZoomOut, FALSE, FALSE, 2); 
	gtk_widget_show (pBtnZoomOut);	
	gtk_signal_connect(GTK_OBJECT(pBtnZoomOut), "toggled", GTK_SIGNAL_FUNC(onToggledBtnScale),
		GINT_TO_POINTER(eCurveOpZoomOut));
	
	pBtnZoomRect= gtk_toggle_button_new();
	sHisQueryData.pToolBtn[eCurveOpZoomRect]=pBtnZoomRect;
#if !(GTK_CHECK_VERSION(2, 6, 0))
	gtk_button_set_label(GTK_BUTTON(pBtnZoomRect), (ptr=_toUtf8EX("OIX_CSTR_ZOOM_RECT")));
	G_SAFE_FREE(ptr);//dzy1017
#else
	gtk_button_set_image(GTK_BUTTON(pBtnZoomRect), create_pixmap (parent, "zoomrect.png"));
#endif
	
	gtk_box_pack_start (GTK_BOX (box), pBtnZoomRect, FALSE, FALSE, 2); 
	gtk_widget_show (pBtnZoomRect);	
	gtk_signal_connect(GTK_OBJECT(pBtnZoomRect), "toggled", GTK_SIGNAL_FUNC(onToggledBtnScale), 
		GINT_TO_POINTER(eCurveOpZoomRect));
	
	pBtnZoom100= gtk_button_new();
#if !(GTK_CHECK_VERSION(2, 6, 0))
	gtk_button_set_label(GTK_BUTTON(pBtnZoom100), (ptr=_toUtf8EX("OIX_CSTR_ZOOM_FIT")));
	G_SAFE_FREE(ptr);//dzy1017
#else
	gtk_button_set_image(GTK_BUTTON(pBtnZoom100), create_pixmap (parent, "zoomfit.png"));
#endif
	
	gtk_box_pack_start (GTK_BOX (box), pBtnZoom100, FALSE, FALSE, 2); 
	gtk_widget_show (pBtnZoom100);	
	gtk_signal_connect(GTK_OBJECT(pBtnZoom100), "clicked", GTK_SIGNAL_FUNC(onBtnClickScaleZoom100), NULL);

	pCheckBtnNumeral=gtk_check_button_new_with_label((ptr=_toUtf8EX("OIX_CSTR_NUMERICAL")));
	G_SAFE_FREE(ptr);//dzy1017

	gtk_box_pack_start(GTK_BOX(box), pCheckBtnNumeral, FALSE, FALSE, 2);
	gtk_widget_show(pCheckBtnNumeral);
	g_signal_connect(G_OBJECT(pCheckBtnNumeral), "toggled", G_CALLBACK(onToggledCheckBtnHisNumeral), NULL);
	
	pCheckBtnMark=gtk_check_button_new_with_label((ptr=_toUtf8EX("OIX_CSTR_MARK")));
	G_SAFE_FREE(ptr);//dzy1017
	gtk_box_pack_start(GTK_BOX(box), pCheckBtnMark, FALSE, FALSE, 2);
	gtk_widget_show(pCheckBtnMark);
	g_signal_connect(G_OBJECT(pCheckBtnMark), "toggled", G_CALLBACK(onToggledCheckBtnHisMark), NULL);

	return pToolBar;
}

// gboolean runProcess(gchar* pStrName, gint crt_n)
// {
// 	gchar name[256];
// 	gchar strInfo[256];
// 	
// #ifdef	WINDOWS_OS
// 	STARTUPINFO				sui;
// 	PROCESS_INFORMATION		proc_info;
// 	static	char	*suffix= ".exe"; 
// #else
// 	pid_t	process_id;
// 	static	char	*suffix= ".dxe"; 
// #endif
// 	
// 	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER)
// 	{
// 		printf("CreateHisCurvToolbar Failed!");
// 		return FALSE;
// 	}
// 
// 	sprintf(name, "%s%s%s", SYS_RUN_PATH, pStrName, suffix);
// 	
// #ifdef	WINDOWS_OS
//     sui.cb       		 = sizeof (STARTUPINFO);
//     sui.lpReserved       = NULL;
// 	sui.cbReserved2      = 0;
//     sui.lpReserved2      = NULL;
// 	sui.lpDesktop 		 = NULL;
// 	sui.dwFlags          = 0;
//     sui.lpDesktop        = NULL;
//     sui.lpTitle          = NULL;
//     sui.dwX              = 0;
//     sui.dwY              = 0;
//     sui.dwXSize          = 0;
//     sui.dwYSize          = 0;
//     sui.dwXCountChars    = 0;
//     sui.dwYCountChars    = 0;
//     sui.dwFillAttribute  = 0;
//     sui.wShowWindow      = 0;
//     
// 	if(CreateProcess(NULL, name, NULL, NULL, FALSE, 0,  NULL, NULL, &sui, &proc_info))
// 	{
// 		hProcess=proc_info.hProcess;
// 		return TRUE;
// 	}
// 
// 	hProcess=(pid_t)-1;
// 	sprintf(strInfo, "%s %s", pStrName, _toUtf8EX("OIX_CSTR_RUN_PROCESS_FAILED"));
// 	popupInfoMsgBox(hist_curve_main_w[crt_n], strInfo);
// 	return FALSE;
// 	
// #else
// 	
// 	hProcess= fork();
// 
// 	if (hProcess == 0)
// 	{
// 		if(execv(name, NULL) >= 0)
// 		{
// 			return TRUE;
// 		}
// 	}
// 
// 	hProcess=(pid_t)-1;
// 
// 	sprintf(strInfo, "%s %s", pStrName, _toUtf8EX("OIX_CSTR_RUN_PROCESS_FAILED"));
// 	popupInfoMsgBox(hist_curve_main_w[crt_n], strInfo);
// 	return FALSE;
// #endif
// }

void popupErrorMsgBox(GtkWidget* pParent, gchar* strMsg)
{
	GtkWidget* pDlgMessage;		//消息对话框
	
	pDlgMessage = gtk_message_dialog_new (GTK_WINDOW(pParent), GTK_DIALOG_DESTROY_WITH_PARENT,
		GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "%s", strMsg);
	gtk_dialog_run (GTK_DIALOG (pDlgMessage));
	gtk_widget_destroy (pDlgMessage);
}

void popupInfoMsgBox(GtkWidget* pParent, gchar* strMsg)
{
	GtkWidget* pDlgMessage;	
	
	pDlgMessage = gtk_message_dialog_new (GTK_WINDOW(pParent), GTK_DIALOG_DESTROY_WITH_PARENT,
		GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, "%s", strMsg);
	
	gdk_beep();
	gtk_widget_show(pDlgMessage);
	
	g_signal_connect_swapped (pDlgMessage, "response", G_CALLBACK(gtk_widget_destroy), pDlgMessage);
}
