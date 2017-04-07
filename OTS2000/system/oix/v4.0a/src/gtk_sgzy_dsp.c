/***
2005.11  updated by zyp by gtk 
updated on 2006.3.26 by zyp for v4.0db
SYNC by hcl 2007.12.09
******/


#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>

#include <stdio.h>

/*#include <sys/shm.h>*/

#include "../inc/gtk_curve_widget.h"

#define	DEBUG
#include	"../inc/oix.h"
#include	"../inc/gtk_widgets.h"

#define OIX_CSTR_SGZY_FAIL_FILE "OIX_CSTR_SGZY_FAIL_FILE"
#define OIX_CSTR_SGZY_ALL_FILE "OIX_CSTR_SGZY_ALL_FILE"
#define OIX_CSTR_ALL_FILE "OIX_CSTR_ALL_FILE"
#define OIX_CSTR_SGZY_DEV "OIX_CSTR_SGZY_DEV"
#define OIX_CSTR_SGZY_DEV_SEL "OIX_CSTR_SGZY_DEV_SEL"

// short	cur_stn		=1;
// short	cur_dev		=1;
// short	cur_dtype	=1;
// short   sgzy_add_item    =0;
// short   sgzy_del_item    =0;

//SGZYDATA	data;
//SGZY_SHM_DATA	*sgzy_pt;
//SGZY_DATA	*sgzy_hist_da;
//SGZYDATA        faildata;

//TRENDDATA	trenddata;
//WDQS_SHM_DATA	*trend_pt;
//WDQS_DATA	*trend_hist_da;

//GdkGC	*draw_gc;

//GtkWidget*	sgzy_main_w[MAX_CRT_NUMBER];
//GtkWidget*	sgzy_history_main_w[MAX_CRT_NUMBER];
//GtkWidget*  	trend_main_w[MAX_CRT_NUMBER];
//GtkWidget*  	trend_history_main_w[MAX_CRT_NUMBER];

// int	sgzy_page_numb;
// int	sgzy_page_cnt;
// int	trend_page_numb;
// int	trend_page_cnt;


//short	sgzy_mmi_mode;

//extern void UpdateTrendNameList ( int crt_n,int stn_no, int dev_no );
//void	DrawSgzyCoord ( int  , GtkWidget * , GdkGC * , int  , int , int  , int ,char  );
//void	SgzyRedraw (GtkWidget*  );
//extern void    windk_draw_text(GdkDrawable * drawable ,GdkFont    *font,GdkGC  *gc, int x,int y,const char      *text,int  strlength,int crt_n) ;

//extern short trend_stn,trend_dev;
//extern gint trend_timeout_id[MAX_CRT_NUMBER];

//void 	UpdateSgzyDsp (GtkWidget* w);

void	UpdateSgzyList ( int crt_n);
extern int	Min(int x,int y);
extern char *_toUtf8(char *c);
extern char *_fromUtf8(char *c);
extern char *_toUtf8EX(char *c);
extern void	PopupMessageBox (GtkWidget *parent,char msg[] );

FILE	*OpenFileX ();	
static  char	fail_file_name[MAX_FAIL_FILE_NAME_LEN];


#ifdef  SUN_OS
static 	SGZY_DY   tmp_sgzydata;
#endif

GtkWidget	*pListSgzy, *pListName;
GtkWidget *pComboBoxStn, *pComboBoxDev, *pComboBoxDataType;

GtkCurveWidget curveWidget;

gboolean bIsFailFile=FALSE;

enum {eStnName=0, eStnId, eStnColNum};
enum {eDevName=0, eDevId, eDevColNum};
enum {eDataTypeName=0, eDataTypeId, eDataTypeColNum};
enum {eSgzyName, eSgzyStnId, eSgzyDevId, eSgzyDataTypeId, eSgzyPtId, eSgzyColNum};
enum {eSgzyCheck, eSgzyCheckName, eSgzyCheckStnId, eSgzyCheckDevId, eSgzyCheckDataType, 
		eSgzyCheckPtId, eSgzyCheckColNum};

gdouble dXValue;
gdouble dYValue[SGZY_NUM];
guint sgzy_curve_timer_id;
SGZY_DY dataSelect;

extern gboolean writeProgSgzyData(const gchar* pFileName, SGZY_DY* pData);
extern gboolean readProgSgzyData(const gchar* pFileName, SGZY_DY* pData);

const gchar *list_item_data_key="list_item_row_no";

#define SGZY_MAX_CURVE_NUM	16
GdkColor* curveColor[SGZY_MAX_CURVE_NUM]={
	&pixels[0][GREEN1], &pixels[0][L_BLUE1], &pixels[0][YLW1], &pixels[0][GRAY1],
	&pixels[0][PINK1], &pixels[0][RED1], &pixels[0][CHOCLT1], &pixels[0][SEA_BLUE1],
	&pixels[0][GREEN3], &pixels[0][L_BLUE3], &pixels[0][YLW3], &pixels[0][GRAY3],
	&pixels[0][PINK3], &pixels[0][RED3], &pixels[0][CHOCLT3], &pixels[0][SEA_BLUE3]
};

GtkColorStack* pSgzyColorStack=0;
int nSgzyRef=0;

void SgzyDlgExitProc(GtkWidget *w, gpointer data)
{
	gint crt_n=GPOINTER_TO_INT(data);
	if (crt_n < 0 || crt_n > MAX_CRT_NUM)
	{
		return;
	}

	gtk_widget_destroy(sgzy_main_w[crt_n]);
}

void DestroySgzyDlg(GtkWidget *w, gpointer data)
{	
	gint crt_n=GPOINTER_TO_INT(data);
	if (crt_n<0 || crt_n>MAX_CRT_NUM)
	{
		return;
	}

	writeProgSgzyData(SGZY_SELECT_FILE, &dataSelect);

	if (sgzy_curve_timer_id != 0)
	{
		g_source_remove(sgzy_curve_timer_id);
		sgzy_curve_timer_id=0;
	}

	if (pSgzyColorStack != 0)
	{
		gtk_color_stack_destroy(pSgzyColorStack);
		pSgzyColorStack=NULL;
	}

	sgzy_main_w[crt_n]=NULL;

	nSgzyRef--;
}

gboolean addCurve(POINTER* ptr)
{
	GCurve* pCurve;
	GdkColor color;

	int 	i;
	POINTER* pPtr;
	DMS_COMMON	 pdt;
	char long_str[POINT_LONGNAME_SIZE];
	float Y_start, Y_end;	
	char dev_comment[GROUP_COMMENT_SIZE];
	char stn_comment[STATION_COMMENT_SIZE];
	char LongNamerStr1[128];
	gint nPtId;
	gchar *pConvert=0;

	if (!gtk_curve_widget_check_add_curve(&curveWidget) || 
		gtk_curve_widget_get_curve_count(&curveWidget) >= SGZY_MAX_CURVE_NUM)
	{
		return FALSE;
	}

	pCurve=gtk_curve_create(SGZY_NUM, eModeSec);
	color=gtk_color_stack_pop_color(pSgzyColorStack);
	gtk_curve_set_color(pCurve, &color);

	if (sgzy_pt->dCurTime <= 0)
	{
		dXValue=time(NULL)-SGZY_NUM;
	}
	else
	{
		dXValue=sgzy_pt->dCurTime-SGZY_NUM;
	}

	if ( ReadEntryById(ptr, &pdt) == -1)
	{
		sprintf(LongNamerStr1, "%s: %d.%d.%d.%d", (pConvert=_toUtf8EX("OIX_CSTR_SGZY_NO_SUCH_POINT")), 
			ptr->stn_id, ptr->dev_id, ptr->data_type, ptr->pt_id);
		G_SAFE_FREE(pConvert);
		gtk_curve_set_text(pCurve, LongNamerStr1);
	}
	else
	{
		GetStnCommentById(pdt.point.stn_id, stn_comment);
		GetGroupCommentById(pdt.point.stn_id, (UCHAR)pdt.point.dev_id, dev_comment);
		GetEntryLongnameById (&pdt.point,long_str);

		sprintf(LongNamerStr1, "%s - %s - %s", stn_comment, dev_comment, long_str);
		
		gtk_curve_set_text(pCurve, (pConvert=_toUtf8(LongNamerStr1)));
		G_SAFE_FREE(pConvert);
		
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
		
		for (i=0; i<data.next_pos; i++)
		{
			pPtr=&data.buffer[i];
			if (pPtr->stn_id == ptr->stn_id && pPtr->dev_id == ptr->dev_id && pPtr->data_type == ptr->data_type
				&& pPtr->pt_id == ptr->pt_id)
			{
				nPtId=i;
				break;
			}
		}
		
		if (nPtId<0 || nPtId >= data.next_pos)
		{
			gtk_curve_destroy(pCurve);
			return FALSE;
		}
		
		if(pPtr->data_type==ANA_TYPE)
		{
			for (i=0; i<SGZY_NUM; i++)
			{
				dYValue[i]=sgzy_pt->sg_dat.fvalue[nPtId][i];
			}
		}
		else
		{
			gtk_curve_set_type(pCurve, eCurveLadder);
			for (i=0; i<SGZY_NUM; i++)
			{
				dYValue[i]=(gdouble)sgzy_pt->sg_dat.status[nPtId][i];
			}
		}
		
		gtk_curve_set_data(pCurve, &dXValue , dYValue, SGZY_NUM, sgzy_pt->nextpt, SGZY_NUM, 1.0);
		gtk_curve_set_boundary(pCurve, TRUE, Y_start, Y_end);
	}

	gtk_curve_set_user_data(pCurve, GINT_TO_POINTER(ptr->stn_id), 0);
	gtk_curve_set_user_data(pCurve, GINT_TO_POINTER(ptr->dev_id), 1);
	gtk_curve_set_user_data(pCurve, GINT_TO_POINTER(ptr->data_type), 2);
	gtk_curve_set_user_data(pCurve, GINT_TO_POINTER(ptr->pt_id), 3);

	gtk_curve_widget_set_time_scale(&curveWidget, dXValue, dXValue+SGZY_NUM);
	gtk_curve_widget_add_curve(&curveWidget, pCurve);

	return TRUE;
}

void removeCurve(POINTER* ptr)
{
	gint nIndex;
	GCurve* pCurve;
	POINTER pt;
	GdkColor color;

	for (nIndex=0; nIndex<gtk_curve_widget_get_curve_count(&curveWidget); nIndex++)
	{
		pCurve=gtk_curve_widget_get_curve(&curveWidget, nIndex);
		if (pCurve != NULL)
		{
			pt.stn_id=GPOINTER_TO_INT(gtk_curve_get_user_data(pCurve, 0));
			pt.dev_id=GPOINTER_TO_INT(gtk_curve_get_user_data(pCurve, 1));
			pt.data_type=GPOINTER_TO_INT(gtk_curve_get_user_data(pCurve, 2));
			pt.pt_id=GPOINTER_TO_INT(gtk_curve_get_user_data(pCurve, 3));

			if (pt.stn_id==ptr->stn_id && pt.dev_id==ptr->dev_id && pt.data_type==ptr->data_type
				&& pt.pt_id==ptr->pt_id)
			{
				color=gtk_curve_get_color(pCurve);
				gtk_color_stack_push_color(pSgzyColorStack, &color);
				gtk_curve_widget_remove_curve(&curveWidget, nIndex);
			}
		}
	}
}

gboolean adjustCurve(gpointer usr_data)
{
	gint nIndex, i;
	GCurve* pCurve;
	POINTER* pPtr;
	POINTER pt;
	gint nPtId;

	gdouble dXValue;

	if (gtk_curve_widget_get_curve_count(&curveWidget) == 0)
	{
		return FALSE;
	}

	if (sgzy_pt->dCurTime <= 0)
	{
		dXValue=time(NULL)-SGZY_NUM;
	}
	else
	{
		dXValue=sgzy_pt->dCurTime-SGZY_NUM;
	}
	
	for (nIndex=0; nIndex<gtk_curve_widget_get_curve_count(&curveWidget); nIndex++)
	{
		pCurve=gtk_curve_widget_get_curve(&curveWidget, nIndex);
		if (pCurve != NULL)
		{
			pt.stn_id=GPOINTER_TO_INT(gtk_curve_get_user_data(pCurve, 0));
			pt.dev_id=GPOINTER_TO_INT(gtk_curve_get_user_data(pCurve, 1));
			pt.data_type=GPOINTER_TO_INT(gtk_curve_get_user_data(pCurve, 2));
			pt.pt_id=GPOINTER_TO_INT(gtk_curve_get_user_data(pCurve, 3));

			for (i=0; i<data.next_pos; i++)
			{
				pPtr=&data.buffer[i];
				if (pPtr->stn_id == pt.stn_id && pPtr->dev_id == pt.dev_id && pPtr->data_type == pt.data_type
					&& pPtr->pt_id == pt.pt_id)
				{
					nPtId=i;
					break;
				}
			}
			
			if (nPtId<0 || nPtId >= data.next_pos)
			{
				break;
			}
			
			if(pPtr->data_type==ANA_TYPE)
			{
				for (i=0; i<SGZY_NUM; i++)
				{
					dYValue[i]=sgzy_pt->sg_dat.fvalue[nPtId][i];
				}
			}
			else
			{
				for (i=0; i<SGZY_NUM; i++)
				{
					dYValue[i]=(gdouble)sgzy_pt->sg_dat.status[nPtId][i];
				}
			}
			
			gtk_curve_set_data(pCurve, &dXValue , dYValue, SGZY_NUM, sgzy_pt->nextpt, SGZY_NUM, 1.0);
		}
	}

	gtk_curve_widget_set_time_scale(&curveWidget, dXValue, dXValue+SGZY_NUM);
	gtk_curve_widget_frame_redraw(&curveWidget);

	return TRUE;
}

gboolean sgzyRedraw(POINTER* ptr, gboolean bCheck)
{
	gint nIndex, nPos=0;
	POINTER* pPt;

	if (bCheck)
	{
		if (sgzy_curve_timer_id != 0)
		{
			g_source_remove(sgzy_curve_timer_id);
			sgzy_curve_timer_id=0;
		}

		bCheck=addCurve(ptr);

		if (bCheck)
		{
			for (nIndex=0; nIndex<dataSelect.next_pos; nIndex++)
			{
				pPt=&dataSelect.buffer[nIndex];
				if (pPt->stn_id == ptr->stn_id && pPt->dev_id==ptr->dev_id && pPt->data_type==ptr->data_type
					&& pPt->pt_id == ptr->pt_id)
				{
					nPos=nIndex;
					break;
				}
			}
			
			if (nIndex>=dataSelect.next_pos)
			{
				dataSelect.buffer[dataSelect.next_pos].stn_id=ptr->stn_id;
				dataSelect.buffer[dataSelect.next_pos].dev_id=ptr->dev_id;
				dataSelect.buffer[dataSelect.next_pos].data_type=ptr->data_type;
				dataSelect.buffer[dataSelect.next_pos].pt_id=ptr->pt_id;
				
				dataSelect.next_pos++;
			}
		}

		sgzy_curve_timer_id = g_timeout_add (1000, (GSourceFunc)adjustCurve, NULL);

		return bCheck;
	}
	else
	{
		removeCurve(ptr);

		for (nIndex=0; nIndex<dataSelect.next_pos; nIndex++)
		{
			pPt=&dataSelect.buffer[nIndex];
			if (pPt->stn_id == ptr->stn_id && pPt->dev_id==ptr->dev_id && pPt->data_type==ptr->data_type
				&& pPt->pt_id == ptr->pt_id)
			{
				nPos=nIndex;
				break;
			}
		}
		
		if (nIndex<dataSelect.next_pos)
		{
			for (nIndex=nPos; nIndex<dataSelect.next_pos-1; nIndex++)
			{
				dataSelect.buffer[nIndex]=dataSelect.buffer[nIndex+1];
			}
			
			dataSelect.next_pos--;
		}

		return FALSE;
	}
}

gint getCurDataType()
{
	GtkTreeModel* pModel;
	GtkTreeIter it;

	gint nDataType=-1;

	pModel=gtk_combo_box_get_model(GTK_COMBO_BOX(pComboBoxDataType));
	gtk_combo_box_get_active_iter(GTK_COMBO_BOX(pComboBoxDataType), &it);
	gtk_tree_model_get(pModel, &it, eDataTypeId, &nDataType, -1);

	return nDataType;
}

void	UpdateNameList (int	crt_n,int stn_no,int dev_no )
{
	short		i;
	POINTER		ptr;
	DMS_COMMON	pdt;
	int		entry_num;			
	char  LongNamerStr1[128];
	char *pConvert=0;

	GtkListStore* pStore;
	GtkTreeIter it;

	if (crt_n<0 || crt_n>=MAX_CRT_NUM)
	{
		return;
	}
 	
	pStore=GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(pListName)));
	gtk_list_store_clear(pStore);

	ptr.stn_id	= stn_no;
	ptr.dev_id	= dev_no;
	ptr.data_type = getCurDataType();
	
	if (GetEntryNumById((char)stn_no,(char)dev_no,(char)ptr.data_type,&entry_num) == -1)
	{
		return;
	}
			
	for ( i=0; i<entry_num; i++)
	{
		ptr.pt_id	=i;
		
		if ( ReadEntryById ( &ptr, &pdt ) == -1)
		{
			sprintf(LongNamerStr1, "No pt: %d.%d.%d.%d", ptr.stn_id, ptr.dev_id, ptr.data_type, ptr.pt_id);
		}
		else
		{
			GetEntryLongnameById (&pdt.point,LongNamerStr1 );
		}
		
		gtk_list_store_append(pStore, &it);
		gtk_list_store_set(pStore, &it, eSgzyName,  (pConvert=_toUtf8(LongNamerStr1)), eSgzyStnId, ptr.stn_id, 
				eSgzyDevId, ptr.dev_id, eSgzyDataTypeId, ptr.data_type, eSgzyPtId, ptr.pt_id, -1);
		G_SAFE_FREE(pConvert);
	}
}

void addItemToSgzyList(POINTER* ptr, GtkListStore* pStore)
{
	DMS_COMMON	pdt;

	char strStn[STATION_LONGNAME_SIZE],strDev[GROUP_LONGNAME_SIZE];
	char strEntryName[ENTRY_LONGNAME_SIZE];	
	char strLongName[128];
	char *pConvert=0;

	GtkTreeIter it;
	gboolean bCheck=FALSE;
	gint nIndex;
	POINTER* pPt;

	for (nIndex=0; nIndex<dataSelect.next_pos; nIndex++)
	{
		pPt=&dataSelect.buffer[nIndex];
		if (ptr->stn_id == pPt->stn_id && ptr->dev_id==pPt->dev_id && ptr->data_type==pPt->data_type 
			&& ptr->pt_id == pPt->pt_id)
		{
			bCheck=TRUE;
			break;
		}
	}

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
	gtk_list_store_set(pStore, &it, eSgzyCheck, bCheck, eSgzyCheckName,  (pConvert=_toUtf8(strLongName)), eSgzyCheckStnId, 
		ptr->stn_id, eSgzyCheckDevId, ptr->dev_id, eSgzyCheckDataType, ptr->data_type, eSgzyCheckPtId, ptr->pt_id, -1);
	G_SAFE_FREE(pConvert);

	if (bCheck)
	{
		sgzyRedraw(ptr, bCheck);
	}
}

void	UpdateSgzyList ( int crt_n)
{
	short		i;
	POINTER	 	*ptr;

	gchar strFileName[MAX_FAIL_FILE_NAME_LEN];
	gchar strTitle[128];
	SGZY_DY *pData;
	gchar *pConvert=0, *pConvert1=0;

	GtkListStore* pStore;

	if (crt_n<0 || crt_n>=MAX_CRT_NUM)
	{
		return;
	}

	gtk_color_stack_reset(pSgzyColorStack);
	gtk_curve_widget_clear_all_curve(&curveWidget);

	pStore=GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(pListSgzy)));
	gtk_list_store_clear(pStore);

	if (bIsFailFile)
	{
		memcpy(strFileName, fail_file_name, MAX_FAIL_FILE_NAME_LEN);
		pData=&faildata;

		strcpy(strTitle, (pConvert=_toUtf8EX("OIX_STR_EVENT_RECALL_FAIL")));
		G_SAFE_FREE(pConvert);
	}
	else
	{
		strcpy(strFileName, SGZY_FILE);
		pData=&data;

		strcpy(strTitle, (pConvert=_toUtf8EX(OIX_STR_EVENT_RECALL)));
		G_SAFE_FREE(pConvert);
	}

	gtk_curve_widget_set_text(&curveWidget, strTitle, eTextTitle);

	sprintf(strTitle, "%s - %s", (pConvert=_toUtf8EX(OIX_CSTR_SGZY_SETTING_TITLE)), (pConvert1=_toUtf8(strFileName)));
	G_SAFE_FREE(pConvert);
	G_SAFE_FREE(pConvert1);
	gtk_window_set_title(GTK_WINDOW(sgzy_main_w[crt_n]), strTitle);

	readProgSgzyData(strFileName, pData);
	
	for ( i=0; i<pData->next_pos; i++)
	{
		ptr	= &pData->buffer[i];

		addItemToSgzyList(ptr, pStore);
	}	
}

void 	SgzyDevSelectCB (GtkComboBox* w, gpointer data)
{
	int	stn_id=-1,dev_id=-1;

	GtkTreeModel* pModel;
	GtkTreeIter it;

	gint crt_n=GPOINTER_TO_INT(data);
	if( crt_n < 0 || crt_n>=MAX_CRT_NUM)
		return; 

	pModel=gtk_combo_box_get_model(GTK_COMBO_BOX(pComboBoxStn));
	if (gtk_combo_box_get_active_iter(GTK_COMBO_BOX(pComboBoxStn), &it))
	{
		gtk_tree_model_get(pModel, &it, eStnId, &stn_id, -1);
	}
	
	pModel=gtk_combo_box_get_model(GTK_COMBO_BOX(pComboBoxDev));
	if (gtk_combo_box_get_active_iter(GTK_COMBO_BOX(pComboBoxDev), &it))
	{
		gtk_tree_model_get(pModel, &it, eDevId, &dev_id, -1);
	}
	
	UpdateNameList ( crt_n, stn_id, dev_id);
}

void SgzyAddCB( GtkButton* w, gpointer user_data)
{
	int		i;
	int	crt_n;

	POINTER* ptr;
	
	GtkTreeSelection* pSelect;
	GtkTreeModel* pModel;
	GtkTreeIter it;
	
	GtkListStore* pStore;
	
	POINTER pt;
	SGZY_DY *pData;

	gint nStnId, nDevId, nDataType, nPtId;
	
	crt_n=GPOINTER_TO_INT(user_data);
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return;

	pData=bIsFailFile? &faildata:&data;

	if (pData->next_pos >= MAX_SGZY_COUNT)
	{
		gdk_beep();
		printf("SGZY Add Point: The Max Number of Points is %d", MAX_SGZY_COUNT);
		return;
	}
	
	pSelect=gtk_tree_view_get_selection (GTK_TREE_VIEW (pListName));
	
	pModel=gtk_tree_view_get_model(GTK_TREE_VIEW(pListName));
	
	if (!gtk_tree_selection_get_selected (pSelect, &pModel, &it))
	{
		gdk_beep();
		printf("SGZY Add Point: No Selected!\n");
		return;
	}
	
	gtk_tree_model_get (pModel, &it, eSgzyStnId, &nStnId, eSgzyDevId, &nDevId, eSgzyDataTypeId, &nDataType, 
		eSgzyPtId, &nPtId, -1);

	pt.stn_id=(UCHAR)nStnId;
	pt.dev_id=(UCHAR)nDevId;
	pt.data_type=(UCHAR)nDataType;
	pt.pt_id=(USHORT)nPtId;
	
	for (i=0; i<pData->next_pos; i++)
	{
		if (pData->buffer[i].stn_id == pt.stn_id && pData->buffer[i].dev_id == pt.dev_id 
			&& pData->buffer[i].data_type == pt.data_type && pData->buffer[i].pt_id == pt.pt_id)
		{
			gdk_beep();
			printf("SGZY Add Point: Selected Point has existed!\n");
			return;
		}
	}

	pStore=GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(pListSgzy)));
	
	pData->buffer[pData->next_pos].stn_id=pt.stn_id;
	pData->buffer[pData->next_pos].dev_id=pt.dev_id;
	pData->buffer[pData->next_pos].data_type=pt.data_type;
	pData->buffer[pData->next_pos].pt_id=pt.pt_id;
	ptr=&pData->buffer[pData->next_pos++];

	addItemToSgzyList(ptr, pStore);
	
	if (bIsFailFile)
	{
		writeProgSgzyData(fail_file_name, pData);

		for (i=0; i<data.next_pos; i++)
		{
			if (data.buffer[i].stn_id == pt.stn_id && data.buffer[i].dev_id == pt.dev_id 
				&& data.buffer[i].data_type == pt.data_type && data.buffer[i].pt_id == pt.pt_id)
			{
				gdk_beep();
				printf("SGZY Add Point: Selected Point has existed!\n");
				return;
			}
		}
		
		data.buffer[data.next_pos].stn_id=pt.stn_id;
		data.buffer[data.next_pos].dev_id=pt.dev_id;
		data.buffer[data.next_pos].data_type=pt.data_type;
		data.buffer[data.next_pos].pt_id=pt.pt_id;
		data.next_pos++;
	}

	writeProgSgzyData(SGZY_FILE, &data);

	sgzy_pt->flg 	= 1;		/**  ----set flag report SGZY_SHM_DATA_____**/
	printf("sgzy_pt->flg 	= 1\n");
}

void sgzyCheckBoxToggled(GtkCellRendererToggle *cell, gchar *path_str, gpointer data)
{
	GtkTreeModel *model = (GtkTreeModel *)data;
	GtkTreeIter  iter;
	GtkTreePath *path = gtk_tree_path_new_from_string (path_str);
	gboolean bCheck;
	POINTER pt; 

	gint nStnId, nDevId, nDataType, nPtId;
	
	gtk_tree_model_get_iter (model, &iter, path);
	gtk_tree_model_get (model, &iter, eSgzyCheck, &bCheck, eSgzyCheckStnId, &nStnId, eSgzyCheckDevId, &nDevId,
		eSgzyCheckDataType, &nDataType, eSgzyCheckPtId, &nPtId, -1);

	pt.stn_id=(UCHAR)nStnId;
	pt.dev_id=(UCHAR)nDevId;
	pt.data_type=(UCHAR)nDataType;
	pt.pt_id=(USHORT)nPtId;
	
	bCheck ^= 1;
	
	bCheck=sgzyRedraw(&pt, bCheck);

	gtk_list_store_set (GTK_LIST_STORE (model), &iter, eSgzyCheck, bCheck, -1);
	
	gtk_tree_path_free (path);
}

void SgzyNameListDbClick(GtkTreeView *tree_view,GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data)
{
	SgzyAddCB(NULL, user_data);
}

void	SgzyRemoveCB ( GtkButton* w, gpointer user_data)
{
	int		i;
	int	crt_n;

 	GtkTreeSelection* pSelect;
	GtkTreeModel* pModel;
	GtkTreeIter it;

	GtkListStore* pStore;

	POINTER pt;
	SGZY_DY *pData;
	gboolean bCheck;

	gint nStnId, nDevId, nDataType, nPtId;

	crt_n=GPOINTER_TO_INT(user_data);
	if( crt_n < 0 || crt_n >= MAX_CRT_NUMBER )
		return;

	pSelect=gtk_tree_view_get_selection (GTK_TREE_VIEW (pListSgzy));

	pModel=gtk_tree_view_get_model(GTK_TREE_VIEW(pListSgzy));

	if (!gtk_tree_selection_get_selected (pSelect, &pModel, &it))
	{
		gdk_beep();
 		printf("SGZY:No Selected!\n");
		return;
	}
	
	gtk_tree_model_get (pModel, &it, eSgzyCheck, &bCheck, eSgzyCheckStnId, &nStnId, eSgzyCheckDevId, 
		&nDevId, eSgzyCheckDataType, &nDataType, eSgzyCheckPtId, &nPtId, -1);

	pt.stn_id=(UCHAR)nStnId;
	pt.dev_id=(UCHAR)nDevId;
	pt.data_type=(UCHAR)nDataType;
	pt.pt_id=(USHORT)nPtId;

	if (bCheck)
	{
		sgzyRedraw(&pt, FALSE);
	}

	pStore=GTK_LIST_STORE(pModel);
	gtk_list_store_remove(pStore, &it);

	pData=bIsFailFile? &faildata:&data;
	
	for (i=0; i<pData->next_pos; i++)
	{
		if (pData->buffer[i].stn_id == pt.stn_id && pData->buffer[i].dev_id == pt.dev_id 
			&& pData->buffer[i].data_type == pt.data_type && pData->buffer[i].pt_id == pt.pt_id)
		{
			break;
		}
	}

	if (i >= pData->next_pos)
	{
		gdk_beep();
 		printf("SGZY Remove:No Find Point in Data!\n");
		return;
	}

	for (; i<pData->next_pos-1;i++)
	{
		pData->buffer[i]=pData->buffer[i+1];
	}

	pData->next_pos--;

	if (bIsFailFile)
	{
		writeProgSgzyData(fail_file_name, pData);
	}
	else
	{
		writeProgSgzyData(SGZY_FILE, pData);

		printf("in sgzy Remove:: before flg=ON,fail_file_name=%s,SGZY_FILE=%s\n",fail_file_name,SGZY_FILE);
		sgzy_pt->flg 	= 1;		/*------set flag report SGZY_SHM_DATA-----*/
		printf("sgzy_pt->flg 	= 1\n");
	}
	
 	printf("Remove end\n");
}

gboolean Sgzy_list_key_release_events(GtkWidget *widget,GdkEventKey *event, gpointer usr_data)
{
	if (event->keyval == GDK_Delete)
	{
		SgzyRemoveCB(NULL, usr_data);

		return TRUE;
	}

	return FALSE;
}

void CreateSgzyDevBtn(GtkComboBox* pComboBox, gpointer data)
{
	int  nStnId=-1, grp_num, nIndex;
	char	dev_comment[GROUP_COMMENT_SIZE];
	char *pConvert=0;

	GtkTreeModel* pModel;
	GtkListStore* pStore;
	GtkTreeIter it;

	gtk_combo_box_set_active(GTK_COMBO_BOX(pComboBoxDev), -1);

	pStore=GTK_LIST_STORE(gtk_combo_box_get_model(GTK_COMBO_BOX(pComboBoxDev)));
	gtk_list_store_clear(pStore);
	
	pModel=gtk_combo_box_get_model(GTK_COMBO_BOX(pComboBoxStn));
	if (!gtk_combo_box_get_active_iter(GTK_COMBO_BOX(pComboBoxStn), &it))
	{
		return;
	}

	gtk_tree_model_get(pModel, &it, eStnId, &nStnId, -1);

	if(-1==GetGroupNumById((UCHAR)nStnId, &grp_num))
		return;
	
	for(nIndex=0; nIndex<=grp_num; nIndex++)
	{
		if(-1==GetGroupCommentById((UCHAR)nStnId, (UCHAR)nIndex, dev_comment))
			continue;
		if(strcmp(dev_comment,"")!=0)
		{
			gtk_list_store_append (pStore, &it);
			gtk_list_store_set (pStore, &it, eDevName, (pConvert=_toUtf8(dev_comment)), eDevId, nIndex, -1);
			G_SAFE_FREE(pConvert);
		}
	}

	gtk_combo_box_set_active(GTK_COMBO_BOX(pComboBoxDev), 0);
}

void SgFileButton(GtkWidget* widget, gpointer usr_data)
{
	GtkWidget *pDlg;
	GtkFileFilter	*pFilter, *pFailFilter;

	gchar strFilePath[256];
	gchar *pConvert=0;

	int crt_n=GPOINTER_TO_INT(usr_data);    
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return  ;
    
	file_mode[crt_n] = FAIL_FILE; 
	strcpy(strFilePath,SG_FILE_PATH);

	pDlg = gtk_file_chooser_dialog_new ((pConvert=_toUtf8EX(OIX_STR_FAIL_FILE_SELECT_BOX)),
		GTK_WINDOW(sgzy_main_w[crt_n]),
		GTK_FILE_CHOOSER_ACTION_OPEN,
		GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
		GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
		NULL);

	G_SAFE_FREE(pConvert);

	gtk_dialog_set_alternative_button_order (GTK_DIALOG (pDlg), GTK_RESPONSE_ACCEPT, GTK_RESPONSE_CANCEL, -1);

	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(pDlg), strFilePath);

	pFailFilter=gtk_file_filter_new();
	gtk_file_filter_set_name(pFailFilter, (pConvert=_toUtf8EX(OIX_CSTR_SGZY_FAIL_FILE)));
	G_SAFE_FREE(pConvert);

	gtk_file_filter_add_pattern(pFailFilter, "*.FAIL");
	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(pDlg), pFailFilter);

	pFilter=gtk_file_filter_new();
	gtk_file_filter_set_name(pFilter, (pConvert=_toUtf8EX(OIX_CSTR_SGZY_ALL_FILE)));
	G_SAFE_FREE(pConvert);
	gtk_file_filter_add_pattern(pFilter, "*.dat");
	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(pDlg), pFilter);
	
	if (gtk_dialog_run (GTK_DIALOG (pDlg)) == GTK_RESPONSE_ACCEPT)
	{
		gchar* filename;
		
		filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (pDlg));
		sprintf(strFilePath, "%s", pConvert=_fromUtf8(filename));
		G_SAFE_FREE(pConvert);

		if (strcmp(strFilePath, SGZY_FILE) == 0)
		{
			bIsFailFile=FALSE;
			memset(fail_file_name, 0, MAX_FAIL_FILE_NAME_LEN);
		}
		else
		{
			bIsFailFile=TRUE;
			strcpy(fail_file_name, strFilePath);
		}

		UpdateSgzyList(crt_n);

		g_free (filename);
	}
	
	g_object_unref(pFailFilter);
	gtk_widget_destroy (pDlg);
}

void onToggledCheckBtnSgzyNumeral(GtkToggleButton *widget, gpointer data)
{
	gboolean bCheck=gtk_toggle_button_get_active(widget);
	gtk_curve_widget_canvas_zoom_100(&curveWidget);
	if (bCheck)
	{
		gtk_curve_widget_set_data_scale(&curveWidget, eCalcAuto, 0, 0);
	}
	else
	{
		gtk_curve_widget_set_data_scale(&curveWidget, eCalcScale, 0, 0);
	}
	
	gtk_curve_widget_frame_redraw(&curveWidget);
}

void onToggledCheckBtnSgzyMark(GtkToggleButton *widget, gpointer data)
{
	gboolean bCheck=gtk_toggle_button_get_active(widget);
	
	if (bCheck)
	{
		gtk_curve_widget_enable(&curveWidget, TRUE, eEnableCurvesMark);
	}
	else
	{
		gtk_curve_widget_enable(&curveWidget, FALSE, eEnableCurvesMark);
	}
	
	gtk_curve_widget_canvas_redraw(&curveWidget);
}

GtkWidget* CreateSgzyToolbar (GtkWidget * parent, gint crt_n)
{
	int	i,stn_num,data_type_num;
	GtkWidget *box, *tempwid;
	GtkWidget *toolbar; 
	
	GtkWidget *button_sgfile,*button_exit;//, *pBtnPgDn, *pBtnPgUp;
	GtkWidget *pCheckBtnNumeral, *pCheckBtnMark;

	GtkListStore *pStoreStn, *pStoreDev, *pStoreDataType;
	GtkTreeIter it;

	GtkCellRenderer *pRenderStn, *pRenderDev, *pRenderDataType;

	char	stn_comment[STATION_COMMENT_SIZE];
	char    datatype_longname[RECORD_LONGNAME_SIZE];
	char *pConvert=0;
	
	if( crt_n < 0 || crt_n >= MAX_CRT_NUMBER )
		return NULL ;
	
	toolbar = gtk_handle_box_new ();
	
	box = gtk_hbox_new (FALSE, 4);
	gtk_container_add (GTK_CONTAINER (toolbar), box);
	gtk_container_border_width (GTK_CONTAINER (box), 5);
	gtk_widget_show (box);
	
	/*  *************  Power station combobox  ****************** */
   	tempwid = gtk_label_new ((pConvert=_toUtf8EX(OIX_CSTR_STATION)));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start (GTK_BOX (box), tempwid, FALSE, FALSE, 0);
	gtk_widget_show (tempwid);

	pStoreStn=gtk_list_store_new(eStnColNum, G_TYPE_STRING, G_TYPE_INT);

	GetStationNum(&stn_num);
	for(i=1; i<=stn_num; i++)
	{
		UCHAR stn_id;

		if(-1==GetStnIdByIndex(i, &stn_id))
			continue;
		
		if( GetStnCommentById((UCHAR)stn_id,(char*)( stn_comment))==-1 )
			continue;
		
		if (strcmp(stn_comment, "") != 0)
		{
			gtk_list_store_append(pStoreStn, &it);
			gtk_list_store_set(pStoreStn, &it, eStnName,  (pConvert=_toUtf8(stn_comment)), eStnId, stn_id, -1);
			G_SAFE_FREE(pConvert);
		}
	}

	pComboBoxStn=gtk_combo_box_new_with_model(GTK_TREE_MODEL(pStoreStn));

	pRenderStn = gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(pComboBoxStn), pRenderStn, FALSE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(pComboBoxStn), pRenderStn, "text", eStnName, NULL);
	g_object_unref(pStoreStn);

	gtk_widget_set_usize(pComboBoxStn, 130, -1);
	gtk_box_pack_start(GTK_BOX(box), pComboBoxStn, FALSE, FALSE, 5);
	gtk_widget_show(pComboBoxStn);

	g_signal_connect (G_OBJECT(pComboBoxStn), "changed", G_CALLBACK(CreateSgzyDevBtn), GINT_TO_POINTER(crt_n));  

	printf("@@@@@ ComboBoxSgzyStn end!!!\n\n");

	/*  *************  dev   combobox  ****************** */ 
	tempwid = gtk_label_new ((pConvert=_toUtf8EX(OIX_CSTR_ALM_DEV_FILTER)));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start (GTK_BOX (box), tempwid, FALSE, FALSE, 0);
	gtk_widget_show (tempwid);

	pRenderDev = gtk_cell_renderer_text_new();

	pStoreDev=gtk_list_store_new(eDevColNum, G_TYPE_STRING, G_TYPE_INT);
	pComboBoxDev = gtk_combo_box_new_with_model(GTK_TREE_MODEL(pStoreDev));
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(pComboBoxDev), pRenderDev, FALSE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(pComboBoxDev), pRenderDev, "text", eDevName, NULL);
	g_object_unref(pStoreDev);

	gtk_widget_set_usize(pComboBoxDev, 130, -1);
	gtk_box_pack_start (GTK_BOX (box), pComboBoxDev, FALSE, FALSE, 5);
	gtk_widget_show (pComboBoxDev);

	g_signal_connect (G_OBJECT(pComboBoxDev), "changed", G_CALLBACK(SgzyDevSelectCB), GINT_TO_POINTER(crt_n));

	printf("#### cmbSgzyDev end\n\n");
	 
	/*  *************  datatype   combobox  ****************** */ 
	pStoreDataType=gtk_list_store_new(eDataTypeColNum, G_TYPE_STRING, G_TYPE_INT);

	if(IsDevObjDisable())
		data_type_num=sizeof(recordlongname)/sizeof(recordlongname[0])-1;            
	else
		data_type_num=sizeof(recordlongname)/sizeof(recordlongname[0]);
	for(i=0;i<data_type_num;i++)
	{
		if(strcmp(recordlongname[i],"")!=0)
		{		
			GetRecLongnameByType((UCHAR)i, datatype_longname);

			gtk_list_store_append(pStoreDataType, &it);
			gtk_list_store_set(pStoreDataType, &it, eDataTypeName, 
				(pConvert=_toUtf8(datatype_longname)), eDataTypeId, i, -1);
			G_SAFE_FREE(pConvert);
		}
	}

	pRenderDataType = gtk_cell_renderer_text_new();
	pComboBoxDataType= gtk_combo_box_new_with_model(GTK_TREE_MODEL(pStoreDataType));
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(pComboBoxDataType), pRenderDataType, FALSE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(pComboBoxDataType), pRenderDataType, "text", 0, NULL);
	g_object_unref(pStoreDataType);
	gtk_widget_set_usize(pComboBoxDataType, 130, -1);
	gtk_box_pack_start (GTK_BOX (box), pComboBoxDataType, FALSE, FALSE, 5);
	gtk_widget_show (pComboBoxDataType);

	gtk_combo_box_set_active(GTK_COMBO_BOX(pComboBoxDataType), 0);
	
	g_signal_connect(G_OBJECT(pComboBoxDataType), "changed", G_CALLBACK(SgzyDevSelectCB), GINT_TO_POINTER(crt_n));
	
	/******sgfile button   add   del   *********/ 	
	button_sgfile = gtk_button_new_with_label((pConvert=_toUtf8EX(OIX_CSTR_SGZY_FILE)));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start (GTK_BOX (box), button_sgfile, FALSE, FALSE, 5);
    
	g_signal_connect(G_OBJECT(button_sgfile), "clicked", G_CALLBACK(SgFileButton), GINT_TO_POINTER(crt_n));
	
	gtk_widget_show(button_sgfile);

	pCheckBtnNumeral=gtk_check_button_new_with_label((pConvert=_toUtf8EX("OIX_CSTR_NUMERICAL")));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start(GTK_BOX(box), pCheckBtnNumeral, FALSE, FALSE, 2);
	gtk_widget_show(pCheckBtnNumeral);
	g_signal_connect(G_OBJECT(pCheckBtnNumeral), "toggled", G_CALLBACK(onToggledCheckBtnSgzyNumeral), NULL);
	
	pCheckBtnMark=gtk_check_button_new_with_label((pConvert=_toUtf8EX("OIX_CSTR_MARK")));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start(GTK_BOX(box), pCheckBtnMark, FALSE, FALSE, 2);
	gtk_widget_show(pCheckBtnMark);
	g_signal_connect(G_OBJECT(pCheckBtnMark), "toggled", G_CALLBACK(onToggledCheckBtnSgzyMark), NULL);
	
	button_exit = gtk_button_new_with_label((pConvert=_toUtf8EX(OIX_CSTR_EXIT)));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_end(GTK_BOX (box), button_exit, FALSE, FALSE, 2);     	     
	g_signal_connect(G_OBJECT(button_exit), "clicked", G_CALLBACK(SgzyDlgExitProc),  GINT_TO_POINTER(crt_n));
	gtk_widget_show (button_exit);
	
	return (toolbar);
}

gboolean writeProgSgzyData(const gchar* pFileName, SGZY_DY* pData)
{
#ifdef SUN_OS
	gint i;
#endif

	FILE* fp;

	if (pFileName == NULL || strcmp(pFileName, "") == 0 || pData == NULL)
	{
		return FALSE;
	}

	if (( fp = fopen(pFileName, "wb")) == NULL)
	{
		printf ("unable to open %s\n", pFileName);
		return FALSE;
	}

#ifdef  SUN_OS
	tmp_sgzydata= *pData;
	for( i=0; i<tmp_sgzydata.next_pos; i++)
		Swap2Byte(&tmp_sgzydata.buffer[i].pt_id);
	Swap2Byte(&tmp_sgzydata.next_pos);
	if ( fwrite (&tmp_sgzydata, sizeof (SGZY_DY), 1, fp) != 1)
		printf ("OIX:in ProgSgzySet write error\n");
#else
	if ( fwrite (pData, sizeof (SGZY_DY), 1, fp) != 1)
		printf ("OIX:in ProgSgzySet write error\n");
#endif
	fclose ( fp );

	return TRUE;
}

gboolean readProgSgzyData(const gchar* pFileName, SGZY_DY* pData)
{
	FILE* fp;
	gint i, j;

	if (pFileName == NULL || strcmp(pFileName, "") == 0 || pData == NULL)
	{
		return FALSE;
	}

	memset(pData, 0, sizeof(SGZY_DY));

	if (( fp = fopen(pFileName, "rb")) != NULL) 
	{
		if ( fread(pData, sizeof (SGZY_DY), 1, fp) !=1 )
			printf ("unable to fread sgzy file\n");
		fclose ( fp );
#ifdef  SUN_OS
		Swap2Byte(&pData->next_pos);
#endif
		if( pData->next_pos > MAX_SGZY_COUNT )
		{
			printf("in ProgSgzySet:read ERROR:pData->next_pos=%d > MAX_SGZY_COUNT\n", pData->next_pos);
			pData->next_pos= 0;
			return FALSE;
		}

#ifdef	SUN_OS
		for( i=0; i<pData->next_pos; i++)
			Swap2Byte(&pData->buffer[i].pt_id);
#endif

		if (bIsFailFile)
		{
			for (i=0; i<faildata.next_pos; i++)
			{
				if (data.next_pos >= MAX_SGZY_COUNT)
				{
					break;
				}

				for (j=0; j<data.next_pos; j++)
				{
					if (faildata.buffer[i].stn_id == data.buffer[j].stn_id 
						&& faildata.buffer[i].dev_id == data.buffer[j].dev_id 
						&& faildata.buffer[i].data_type == data.buffer[j].data_type
						&& faildata.buffer[i].pt_id == data.buffer[j].pt_id)
					{
						break;
					}
				}

				if (j < data.next_pos)
				{
					continue;
				}
				
				data.buffer[data.next_pos++]=faildata.buffer[i];
			}

			writeProgSgzyData(SGZY_FILE, &data);
			sgzy_pt->flg 	= 1;		/*------set flag report SGZY_SHM_DATA-----*/
			printf("sgzy_pt->flg 	= 1\n");
		}

		return TRUE;
	}

	return FALSE;
}

GtkWidget* CreateProgSgzyShell (GtkWidget *parent , int  crt_n )
{
	GtkWidget *vbox, *hbox, *vSubBox,*hSubBox;
	GtkWidget  *toolbar,*name_list_scrollw,*sgzy_list_scrollw;
	GtkWidget	*button_add_point,*button_del_point;

	GtkWidget* windows;

	GtkWidget *pFrame=NULL;

	GtkListStore *pStoreName, *pStoreSgzy;
	GtkTreeSelection *pSelection;
	GtkCellRenderer *pRenderText, *pRenderCheckBox;
	GtkTreeViewColumn *pColumn;

	gchar *pConvert=0;

#ifdef SUN_OS
	GdkScreen* pScreen;
#endif
	
    windows=gtk_window_new(GTK_WINDOW_TOPLEVEL);
#ifdef SUN_OS
	pScreen=gtk_window_get_screen(GTK_WINDOW(windows));
	if (pScreen != NULL)
	{
		gtk_window_set_default_size(GTK_WINDOW(windows), gdk_screen_get_width(pScreen), gdk_screen_get_height(pScreen));
	}
#else
	gtk_window_maximize(GTK_WINDOW(windows));
#endif

	gtk_window_set_transient_for(GTK_WINDOW(windows),GTK_WINDOW(parent));
		
    gtk_signal_connect(GTK_OBJECT(windows), "destroy", GTK_SIGNAL_FUNC(DestroySgzyDlg), (gpointer)crt_n);
    
    vbox=gtk_vbox_new(FALSE, 5);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 0);
    gtk_container_add(GTK_CONTAINER(windows), vbox);
    gtk_widget_show(vbox);
    
    toolbar =  CreateSgzyToolbar(windows, crt_n);
    gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, TRUE, 0);
    gtk_widget_show(toolbar);    
    
    hbox=gtk_hbox_new(FALSE, 3); 
	gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, 0);
    gtk_widget_show(hbox);
    
	vSubBox=gtk_vbox_new(FALSE, 3);
	gtk_widget_set_usize(vSubBox, 250, -1);
	gtk_box_pack_start(GTK_BOX(hbox), vSubBox, FALSE, TRUE, 0);
	gtk_widget_show(vSubBox);

	pRenderText = gtk_cell_renderer_text_new();
	
	pStoreName=gtk_list_store_new(eSgzyColNum, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT);
	pListName=gtk_tree_view_new_with_model(GTK_TREE_MODEL(pStoreName));
	pSelection=gtk_tree_view_get_selection(GTK_TREE_VIEW(pListName));
	gtk_tree_selection_set_mode(pSelection, GTK_SELECTION_SINGLE);
	pColumn=gtk_tree_view_column_new_with_attributes((pConvert=_toUtf8EX(OIX_CSTR_SGZY_DEV)), 
		pRenderText, "text", eSgzyName, NULL);
	G_SAFE_FREE(pConvert);
	gtk_tree_view_append_column (GTK_TREE_VIEW (pListName), pColumn);

	g_object_unref(pStoreName);
    
    name_list_scrollw=gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (name_list_scrollw), GTK_SHADOW_ETCHED_IN);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (name_list_scrollw), GTK_POLICY_AUTOMATIC, 
		GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(vSubBox), name_list_scrollw, TRUE, TRUE, 0);
    gtk_widget_show (name_list_scrollw); 
	
	gtk_container_add (GTK_CONTAINER (name_list_scrollw), pListName);
    gtk_widget_show (pListName); 
    
	g_signal_connect(G_OBJECT(GTK_TREE_VIEW(pListName)), "row_activated", G_CALLBACK(SgzyNameListDbClick), 
		GINT_TO_POINTER(crt_n));

	hSubBox=gtk_hbox_new(FALSE, 15);
	gtk_widget_set_usize(hSubBox, -1, 30);
	gtk_box_pack_start(GTK_BOX(vSubBox), hSubBox, FALSE, FALSE, 0);
	gtk_widget_show(hSubBox);

	button_add_point = gtk_button_new_with_label((pConvert=_toUtf8EX(OIX_CSTR_ADD)));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start (GTK_BOX (hSubBox), button_add_point, TRUE, TRUE, 0);
	button_del_point = gtk_button_new_with_label((pConvert=_toUtf8EX(OIX_CSTR_DEL)));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start (GTK_BOX (hSubBox), button_del_point, TRUE, TRUE, 0);
	
	g_signal_connect(G_OBJECT(button_add_point), "clicked", G_CALLBACK(SgzyAddCB), GINT_TO_POINTER(crt_n));
   	g_signal_connect(G_OBJECT(button_del_point), "clicked", G_CALLBACK(SgzyRemoveCB), GINT_TO_POINTER(crt_n));
	
	gtk_widget_show(button_add_point);
	gtk_widget_show(button_del_point);

	pStoreSgzy=gtk_list_store_new(eSgzyCheckColNum, G_TYPE_BOOLEAN, G_TYPE_STRING, G_TYPE_INT, 
		G_TYPE_INT, G_TYPE_INT, G_TYPE_INT);
	pListSgzy =gtk_tree_view_new_with_model(GTK_TREE_MODEL(pStoreSgzy));
	pSelection=gtk_tree_view_get_selection(GTK_TREE_VIEW(pListSgzy));
	gtk_tree_selection_set_mode(pSelection, GTK_SELECTION_SINGLE);

	pRenderCheckBox=gtk_cell_renderer_toggle_new();
	gtk_cell_renderer_toggle_set_radio(GTK_CELL_RENDERER_TOGGLE(pRenderCheckBox), FALSE);
	g_signal_connect (pRenderCheckBox, "toggled", G_CALLBACK (sgzyCheckBoxToggled), GTK_TREE_MODEL(pStoreSgzy));

	pColumn=gtk_tree_view_column_new_with_attributes(NULL, pRenderCheckBox, "active", 0, NULL);
	gtk_tree_view_column_set_sizing (GTK_TREE_VIEW_COLUMN (pColumn), GTK_TREE_VIEW_COLUMN_FIXED);
	gtk_tree_view_column_set_fixed_width (GTK_TREE_VIEW_COLUMN (pColumn), 30);
	gtk_tree_view_append_column (GTK_TREE_VIEW (pListSgzy), pColumn);

	pColumn=gtk_tree_view_column_new_with_attributes((pConvert=_toUtf8EX(OIX_CSTR_SGZY_DEV_SEL)), pRenderText, 
		"text", eSgzyCheckName, NULL);
	G_SAFE_FREE(pConvert);
	gtk_tree_view_append_column (GTK_TREE_VIEW (pListSgzy), pColumn);

	g_object_unref(pStoreSgzy);
    
    sgzy_list_scrollw=gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_set_usize (GTK_WIDGET(sgzy_list_scrollw),-1 , 200);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (sgzy_list_scrollw), GTK_SHADOW_ETCHED_IN);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (sgzy_list_scrollw), GTK_POLICY_AUTOMATIC, 
		GTK_POLICY_AUTOMATIC);
	
    gtk_box_pack_start(GTK_BOX(vSubBox), sgzy_list_scrollw, FALSE, FALSE, 0);
    gtk_widget_show (sgzy_list_scrollw);
    
	gtk_container_add (GTK_CONTAINER (sgzy_list_scrollw), pListSgzy);
    gtk_widget_show (pListSgzy);

	g_signal_connect (GTK_OBJECT (pListSgzy), "key_release_event", G_CALLBACK(Sgzy_list_key_release_events), 
		GINT_TO_POINTER(crt_n));

	pFrame=gtk_curve_widget_create(windows, &curveWidget);
	gtk_box_pack_start(GTK_BOX(hbox ), pFrame, TRUE, TRUE, 0);

	gtk_curve_widget_set_text(&curveWidget, (pConvert=_toUtf8EX("OIX_CSTR_SGZY_PERCENTAGE")), eTextLabel);
	G_SAFE_FREE(pConvert);
	gtk_curve_widget_enable(&curveWidget, TRUE, eEnableAutoSubTitle);

	gtk_combo_box_set_active(GTK_COMBO_BOX(pComboBoxStn), 0);
	
	UpdateSgzyList (crt_n);

	gtk_widget_show_all(windows);

	gtk_curve_widget_set_current_operation(&curveWidget, eCurveOpPointer);
	gtk_curve_widget_frame_redraw(&curveWidget);

	return windows;
}

void ProgSgzySet (int crt_n )	
{
	if (crt_n<0 || crt_n>=MAX_CRT_NUM)
	{
		gdk_beep();
		return;
	}

	if (sgzy_main_w[crt_n] == NULL)
	{
		if (nSgzyRef >= 1)
		{
			gdk_beep(); 
			PopupMessageBox(main_w[crt_n] , "OIX: SGZY Setting has opened !!");
			return;
		}
		nSgzyRef++;

		pSgzyColorStack=gtk_color_stack_create(curveColor, SGZY_MAX_CURVE_NUM);

		readProgSgzyData(SGZY_SELECT_FILE, &dataSelect);
		readProgSgzyData(SGZY_FILE, &data);

		sgzy_pt=LinktoShareMem(SHM_SGZY_KEY_CHAR);
		if( sgzy_pt == (void*)-1 )
		{
			perror("ERROR: sgzy_pt= -1!!!");
			return;
		}

		sgzy_main_w[crt_n]=CreateProgSgzyShell(main_w[crt_n], crt_n);
	}
	else
	{
		gtk_window_present(GTK_WINDOW(sgzy_main_w[crt_n]));
	}
}

