
/*
2005.10.   updated   by zyp by gtk
HAVE  SYNC   2008.4.10    BY CHI.HAILONG  
*/



#include <stdio.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>

/*#include <sys/shm.h>*/
#include        "../inc/oix.h"
#include "../inc/gtk_curve_widget.h"

extern char *_toUtf8(char *c);
extern char *_fromUtf8(char *c);
extern char *_toUtf8EX(char *c);

extern void	PopupMessageBox (GtkWidget *parent,char msg[] );

#ifdef  SUN_OS
static  WDQS_DY  tmp_trenddata;
#endif

enum {eStnName=0, eStnId, eStnColNum};
enum {eDevName=0, eDevId, eDevColNum};
enum {eDataTypeName=0, eDataTypeId, eDataTypeColNum};
enum {eTrendName, eTrendStnId, eTrendDevId, eTrendDataType, eTrendPtId, eTrendColNum};
enum {eTrendCheck, eTrendCheckName, eTrendCheckStnId, eTrendCheckDevId, eTrendCheckDataType, 
		eTrendCheckPtId, eTrendCheckColNum};
GtkCurveWidget trendCurveWidget;

GtkWidget* pListTrend, *pListTrendAdd;
GtkWidget *pComboBoxTrendStn, *pComboBoxTrendDev, *pComboBoxTrendDataType;

#define TREND_MAX_CURVE_NUM		16
GtkColorStack* pColorStackTrend=0;
extern GdkColor* curveColor[TREND_MAX_CURVE_NUM];

gdouble dValueTrend[WDQS_NUM];

guint trend_curve_timer_id=0;

int nTrendRef=0;

gboolean trendRedraw(POINTER* ptr, gboolean bCheck);
gboolean writeProgTrendData(const gchar* pFileName, WDQS_DY* pData);

gint getCurTrendDataType()
{
	GtkTreeModel* pModel;
	GtkTreeIter it;
	
	gint nDataType=-1;
	
	pModel=gtk_combo_box_get_model(GTK_COMBO_BOX(pComboBoxTrendDataType));
	gtk_combo_box_get_active_iter(GTK_COMBO_BOX(pComboBoxTrendDataType), &it);
	gtk_tree_model_get(pModel, &it, eDataTypeId, &nDataType, -1);
	
	return nDataType;
}

void	UpdateTrendNameList ( int crt_n,int stn_no,int dev_no )
{
	short		i;
	POINTER		ptr;
	DMS_COMMON	pdt;
	int entry_num;			
	char strLongName[128];
	char *pConvert=0;
	
	GtkListStore* pStore;
	GtkTreeIter it;

	if (crt_n<0 || crt_n>=MAX_CRT_NUM)
	{
		return;
	}
	
	pStore=GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(pListTrend)));
	gtk_list_store_clear(pStore);
	
	ptr.stn_id	= stn_no;
	ptr.dev_id	= dev_no;
	ptr.data_type = getCurTrendDataType();
	
	if (GetEntryNumById((UCHAR)stn_no,(UCHAR)dev_no,(UCHAR)ptr.data_type, (GULONG*)&entry_num) == -1)
	{
		return;
	}
	
	for ( i=0; i<entry_num; i++)
	{
		ptr.pt_id	=i;
		
		if ( ReadEntryById ( &ptr, &pdt ) == -1)
		{
			sprintf(strLongName, "No pt: %d.%d.%d.%d", ptr.stn_id, ptr.dev_id, ptr.data_type, ptr.pt_id);
		}
		else
		{
			GetEntryLongnameById (&pdt.point, strLongName );
		}

		gtk_list_store_append(pStore, &it);
		gtk_list_store_set(pStore, &it, eTrendName,  (pConvert=_toUtf8(strLongName)), eTrendStnId, ptr.stn_id, 
				eTrendDevId, ptr.dev_id, eTrendDataType, ptr.data_type, eTrendPtId, ptr.pt_id, -1);
		G_SAFE_FREE(pConvert);
	}
}

void addItemToTrendAddList(POINTER* ptr, GtkListStore* pStore)
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
	
	for (nIndex=0; nIndex<trendSelect.next_pos; nIndex++)
	{
		pPt=&trendSelect.buffer[nIndex];
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
	gtk_list_store_set(pStore, &it, eTrendCheck, bCheck, eTrendCheckName,  (pConvert=_toUtf8(strLongName)), eTrendCheckStnId, 
		ptr->stn_id, eTrendCheckDevId, ptr->dev_id, eTrendCheckDataType, ptr->data_type, 
		eTrendCheckPtId, ptr->pt_id, -1);
	G_SAFE_FREE(pConvert);
	
	if (bCheck)
	{
		trendRedraw(ptr, bCheck);
	}
}

void	UpdateTrendList (int crt_n )
{
	short		i;
	POINTER	 	*ptr;

	GtkListStore* pStore;
	
	if (crt_n<0 || crt_n>=MAX_CRT_NUM)
	{
		return;
	}

	gtk_color_stack_reset(pColorStackTrend);
	gtk_curve_widget_clear_all_curve(&trendCurveWidget);
	
	pStore=GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(pListTrendAdd)));
	gtk_list_store_clear(pStore);
	
	for (i=0; i<trenddata.next_pos; i++)
	{
		ptr	= &trenddata.buffer[i];

		addItemToTrendAddList(ptr, pStore);
	}
}


void	TrendAddCB (GtkWidget* w, gpointer usr_data)
{
	int i;
	int	crt_n;
	
	POINTER* ptr;
	
	GtkTreeSelection* pSelect;
	GtkTreeModel* pModel;
	GtkTreeIter it;
	
	GtkListStore* pStore;
	
	POINTER pt;

	gint nStnId, nDevId, nDataType, nPtId;
	
	crt_n=GPOINTER_TO_INT(usr_data);
	if( crt_n < 0 || crt_n >= MAX_CRT_NUMBER)
		return;
	
	printf("##in TrendAddCB,next_pos=%d\n",trenddata.next_pos);
	if ( trenddata.next_pos >= MAX_WDQS_COUNT)
	{
		gdk_beep();
		return;
	}
	
	pSelect=gtk_tree_view_get_selection (GTK_TREE_VIEW (pListTrend));
	
	pModel=gtk_tree_view_get_model(GTK_TREE_VIEW(pListTrend));
	
	if (!gtk_tree_selection_get_selected (pSelect, &pModel, &it))
	{
		gdk_beep();
		printf("SGZY Add Point: No Selected!\n");
		return;
	}
	
	gtk_tree_model_get (pModel, &it, eTrendStnId, &nStnId, eTrendDevId, &nDevId, eTrendDataType, &nDataType, 
		eTrendPtId, &nPtId, -1);

	pt.stn_id=(UCHAR)nStnId;
	pt.dev_id=(UCHAR)nDevId;
	pt.data_type=(UCHAR)nDataType;
	pt.pt_id=(USHORT)nPtId;
	
	for(i=0; i<trenddata.next_pos;i++)
	{
		if((trenddata.buffer[i].stn_id    == pt.stn_id)&&
			(trenddata.buffer[i].dev_id    == pt.dev_id) && 
			(trenddata.buffer[i].data_type == pt.data_type) &&
			(trenddata.buffer[i].pt_id  == pt.pt_id))
		{
			gdk_beep();
			return;
		}
	}    

	pStore=GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(pListTrendAdd)));

	trenddata.buffer[trenddata.next_pos].stn_id		= pt.stn_id;
	trenddata.buffer[trenddata.next_pos].dev_id		= pt.dev_id;
	trenddata.buffer[trenddata.next_pos].data_type	= pt.data_type;
	trenddata.buffer[trenddata.next_pos].pt_id 		= pt.pt_id;
	ptr=&trenddata.buffer[trenddata.next_pos++];
	
	addItemToTrendAddList(ptr, pStore);

	writeProgTrendData(TREND_FILE, &trenddata);
	
	trend_pt->flg 	= 1;               /**  ----set flag report WDQS_SHM_DATA_____**/
}

void	TrendRemoveCB ( GtkWidget* w, gpointer user_data)
{
	int		i;
	int	crt_n;
	
	GtkTreeSelection* pSelect;
	GtkTreeModel* pModel;
	GtkTreeIter it;
	
	GtkListStore* pStore;
	
	POINTER pt;
	gboolean bCheck;

	gint nStnId, nDevId, nDataType, nPtId;
	
	crt_n=GPOINTER_TO_INT(user_data);
	if( crt_n < 0 || crt_n >= MAX_CRT_NUMBER )
		return;

	pSelect=gtk_tree_view_get_selection (GTK_TREE_VIEW (pListTrendAdd));
	
	pModel=gtk_tree_view_get_model(GTK_TREE_VIEW(pListTrendAdd));
	
	if (!gtk_tree_selection_get_selected (pSelect, &pModel, &it))
	{
		gdk_beep();
		printf("TREND:No Selected!\n");
		return;
	}
	
	gtk_tree_model_get (pModel, &it, eTrendCheck, &bCheck, eTrendCheckStnId, &nStnId, eTrendCheckDevId, 
		&nDevId, eTrendCheckDataType, &nDataType, eTrendCheckPtId, &nPtId, -1);

	pt.stn_id=(UCHAR)nStnId;
	pt.dev_id=(UCHAR)nDevId;
	pt.data_type=(UCHAR)nDataType;
	pt.pt_id=(USHORT)nPtId;
	
	if (bCheck)
	{
		trendRedraw(&pt, FALSE);
	}
	
	pStore=GTK_LIST_STORE(pModel);
	gtk_list_store_remove(pStore, &it);

	for (i=0; i<trenddata.next_pos; i++)
	{
		if (trenddata.buffer[i].stn_id == pt.stn_id && trenddata.buffer[i].dev_id == pt.dev_id 
			&& trenddata.buffer[i].data_type == pt.data_type && trenddata.buffer[i].pt_id == pt.pt_id)
		{
			break;
		}
	}
	
	if (i >= trenddata.next_pos)
	{
		gdk_beep();
		printf("SGZY Remove:No Find Point in Data!\n");
		return;
	}
	
	for (; i<trenddata.next_pos-1;i++)
	{
		trenddata.buffer[i]=trenddata.buffer[i+1];
	}
	
	trenddata.next_pos--;

	writeProgTrendData(TREND_FILE, &trenddata);
	
	trend_pt->flg 	= 1;		/**------set flag report WDQS_SHM_DATA-----**/
}

void	TrendSaveCB ( GtkWidget* w, gpointer data)
{
	int crt_n=GPOINTER_TO_INT(data);
	if( crt_n < 0 || crt_n>=MAX_CRT_NUM)
		return; 
  		
	if ( trenddata.next_pos == 0)
	{
		gdk_beep();
		return;
	}
	trend_pt->save_flg   = 1;            /**------set save_flag report WDQS_SHM_DATA-----**/
	
	printf("@@@@  in TrendSaveCB\n");
}

gboolean addTrendCurve(POINTER* ptr)
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
	char *pConvert=0;

	gdouble dXValue;

	if (!gtk_curve_widget_check_add_curve(&trendCurveWidget) || 
		gtk_curve_widget_get_curve_count(&trendCurveWidget) >= TREND_MAX_CURVE_NUM)
	{
		return FALSE;
	}

	pCurve=gtk_curve_create(WDQS_NUM, eModeSec);
	color=gtk_color_stack_pop_color(pColorStackTrend);
	gtk_curve_set_color(pCurve, &color);

	if (trend_pt->dCurTime <= 0)
	{
		dXValue=time(NULL)-WDQS_NUM;
	}
	else
	{
		dXValue=trend_pt->dCurTime-WDQS_NUM;
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
		
		for (i=0; i<trenddata.next_pos; i++)
		{
			pPtr=&trenddata.buffer[i];
			if (pPtr->stn_id == ptr->stn_id && pPtr->dev_id == ptr->dev_id && pPtr->data_type == ptr->data_type
				&& pPtr->pt_id == ptr->pt_id)
			{
				nPtId=i;
				break;
			}
		}
		
		if (nPtId<0 || nPtId >= trenddata.next_pos)
		{
			gtk_curve_destroy(pCurve);
			return FALSE;
		}
		
		if(pPtr->data_type==ANA_TYPE)
		{
			for (i=0; i<WDQS_NUM; i++)
			{
				dValueTrend[i]=trend_pt->wd_dat.fvalue[nPtId][i];
			}
		}
		else
		{
			gtk_curve_set_type(pCurve, eCurveLadder);
			for (i=0; i<WDQS_NUM; i++)
			{
				dValueTrend[i]=trend_pt->wd_dat.status[nPtId][i];
			}
		}
		
		gtk_curve_set_data(pCurve, &dXValue , dValueTrend, WDQS_NUM, trend_pt->nextpt, WDQS_NUM, 1.0);
		gtk_curve_set_boundary(pCurve, TRUE, Y_start, Y_end);
	}

	gtk_curve_set_user_data(pCurve, GINT_TO_POINTER(ptr->stn_id), 0);
	gtk_curve_set_user_data(pCurve, GINT_TO_POINTER(ptr->dev_id), 1);
	gtk_curve_set_user_data(pCurve, GINT_TO_POINTER(ptr->data_type), 2);
	gtk_curve_set_user_data(pCurve, GINT_TO_POINTER(ptr->pt_id), 3);

	gtk_curve_widget_set_time_scale(&trendCurveWidget, dXValue, dXValue+WDQS_NUM);
	gtk_curve_widget_add_curve(&trendCurveWidget, pCurve);

	return TRUE;
}

void removeTrendCurve(POINTER* ptr)
{
	gint nIndex;
	GCurve* pCurve;
	POINTER pt;
	GdkColor color;
	
	for (nIndex=0; nIndex<gtk_curve_widget_get_curve_count(&trendCurveWidget); nIndex++)
	{
		pCurve=gtk_curve_widget_get_curve(&trendCurveWidget, nIndex);
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
				gtk_color_stack_push_color(pColorStackTrend, &color);
				gtk_curve_widget_remove_curve(&trendCurveWidget, nIndex);
			}
		}
	}
}

gboolean adjustTrendCurve(gpointer usr_data)
{
	gint nIndex, i;
	GCurve* pCurve;
	POINTER* pPtr;
	POINTER pt;
	gint nPtId;
	
	gdouble dXValue;
	
	if (gtk_curve_widget_get_curve_count(&trendCurveWidget) == 0)
	{
		return FALSE;
	}
	
	if (trend_pt->dCurTime <= 0)
	{
		dXValue=time(NULL)-WDQS_NUM;
	}
	else
	{
		dXValue=trend_pt->dCurTime-WDQS_NUM;
	}
	
	for (nIndex=0; nIndex<gtk_curve_widget_get_curve_count(&trendCurveWidget); nIndex++)
	{
		pCurve=gtk_curve_widget_get_curve(&trendCurveWidget, nIndex);
		if (pCurve != NULL)
		{
			pt.stn_id=GPOINTER_TO_INT(gtk_curve_get_user_data(pCurve, 0));
			pt.dev_id=GPOINTER_TO_INT(gtk_curve_get_user_data(pCurve, 1));
			pt.data_type=GPOINTER_TO_INT(gtk_curve_get_user_data(pCurve, 2));
			pt.pt_id=GPOINTER_TO_INT(gtk_curve_get_user_data(pCurve, 3));
			
			for (i=0; i<trenddata.next_pos; i++)
			{
				pPtr=&trenddata.buffer[i];
				if (pPtr->stn_id == pt.stn_id && pPtr->dev_id == pt.dev_id && pPtr->data_type == pt.data_type
					&& pPtr->pt_id == pt.pt_id)
				{
					nPtId=i;
					break;
				}
			}
			
			if (nPtId<0 || nPtId >= trenddata.next_pos)
			{
				break;
			}
			
			if(pPtr->data_type==ANA_TYPE)
			{
				for (i=0; i<WDQS_NUM; i++)
				{
					dValueTrend[i]=trend_pt->wd_dat.fvalue[nPtId][i];
				}
			}
			else
			{
				for (i=0; i<WDQS_NUM; i++)
				{
					dValueTrend[i]=trend_pt->wd_dat.status[nPtId][i];
				}
			}
			
			gtk_curve_set_data(pCurve, &dXValue , dValueTrend, WDQS_NUM, trend_pt->nextpt, WDQS_NUM, 1.0);
		}
	}
	
	gtk_curve_widget_set_time_scale(&trendCurveWidget, dXValue, dXValue+WDQS_NUM);
	gtk_curve_widget_frame_redraw(&trendCurveWidget);
	
	return TRUE;
}

gboolean trendRedraw(POINTER* ptr, gboolean bCheck)
{
	gint nIndex, nPos=0;
	POINTER* pPt;
	
	if (bCheck)
	{
		if (trend_curve_timer_id != 0)
		{
			g_source_remove(trend_curve_timer_id);
			trend_curve_timer_id=0;
		}
		
		bCheck=addTrendCurve(ptr);
		
		if (bCheck)
		{
			for (nIndex=0; nIndex<trendSelect.next_pos; nIndex++)
			{
				pPt=&trendSelect.buffer[nIndex];
				if (pPt->stn_id == ptr->stn_id && pPt->dev_id==ptr->dev_id && pPt->data_type==ptr->data_type
					&& pPt->pt_id == ptr->pt_id)
				{
					nPos=nIndex;
					break;
				}
			}
			
			if (nIndex>=trendSelect.next_pos)
			{
				trendSelect.buffer[trendSelect.next_pos].stn_id=ptr->stn_id;
				trendSelect.buffer[trendSelect.next_pos].dev_id=ptr->dev_id;
				trendSelect.buffer[trendSelect.next_pos].data_type=ptr->data_type;
				trendSelect.buffer[trendSelect.next_pos].pt_id=ptr->pt_id;
				
				trendSelect.next_pos++;
			}
		}
		
		trend_curve_timer_id = g_timeout_add (1000, (GSourceFunc)adjustTrendCurve, NULL);
		
		gtk_curve_widget_frame_redraw(&trendCurveWidget);

		return bCheck;
	}
	else
	{
		removeTrendCurve(ptr);
		
		for (nIndex=0; nIndex<trendSelect.next_pos; nIndex++)
		{
			pPt=&trendSelect.buffer[nIndex];
			if (pPt->stn_id == ptr->stn_id && pPt->dev_id==ptr->dev_id && pPt->data_type==ptr->data_type
				&& pPt->pt_id == ptr->pt_id)
			{
				nPos=nIndex;
				break;
			}
		}
		
		if (nIndex<trendSelect.next_pos)
		{
			for (nIndex=nPos; nIndex<trendSelect.next_pos-1; nIndex++)
			{
				trendSelect.buffer[nIndex]=trendSelect.buffer[nIndex+1];
			}
			
			trendSelect.next_pos--;
		}

		gtk_curve_widget_frame_redraw(&trendCurveWidget);
		
		return FALSE;
	}
}

void CreateTrendDevBtn(GtkComboBox* pComboBox, gpointer data)
{
	int  nStnId=-1, grp_num, nIndex;
	char	dev_comment[GROUP_COMMENT_SIZE];
	
	GtkTreeModel* pModel;
	GtkListStore* pStore;
	GtkTreeIter it;
	char *pConvert=0;
	
	gtk_combo_box_set_active(GTK_COMBO_BOX(pComboBoxTrendDev), -1);

	pStore=GTK_LIST_STORE(gtk_combo_box_get_model(GTK_COMBO_BOX(pComboBoxTrendDev)));
	gtk_list_store_clear(pStore);

	pModel=gtk_combo_box_get_model(GTK_COMBO_BOX(pComboBoxTrendStn));
	if (!gtk_combo_box_get_active_iter(GTK_COMBO_BOX(pComboBoxTrendStn), &it))
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
	
	gtk_combo_box_set_active(GTK_COMBO_BOX(pComboBoxTrendDev), 0);
}

void TrendDevSelectCB (GtkComboBox* w, gpointer data)
{
	int	stn_id=-1,dev_id=-1;
	
	GtkTreeModel* pModel;
	GtkTreeIter it;
	
	gint crt_n=GPOINTER_TO_INT(data);
	if( crt_n < 0 || crt_n>=MAX_CRT_NUM)
		return; 

	pModel=gtk_combo_box_get_model(GTK_COMBO_BOX(pComboBoxTrendStn));
	if (gtk_combo_box_get_active_iter(GTK_COMBO_BOX(pComboBoxTrendStn), &it))
	{
		gtk_tree_model_get(pModel, &it, eStnId, &stn_id, -1);
	}
	
	pModel=gtk_combo_box_get_model(GTK_COMBO_BOX(pComboBoxTrendDev));
	if (gtk_combo_box_get_active_iter(GTK_COMBO_BOX(pComboBoxTrendDev), &it))
	{
		gtk_tree_model_get(pModel, &it, eDevId, &dev_id, -1);
	}

	UpdateTrendNameList ( crt_n, stn_id, dev_id);
}

void TrendDlgExitProc(GtkWidget *w, gpointer data)
{
	gint crt_n=GPOINTER_TO_INT(data);
	if (crt_n < 0 || crt_n >= MAX_CRT_NUM)
	{
		return;
	}
	
	gtk_widget_destroy(trend_main_w[crt_n]);
}

void onToggledCheckBtnTrendNumeral(GtkToggleButton *widget, gpointer data)
{
	gboolean bCheck=gtk_toggle_button_get_active(widget);
	gtk_curve_widget_canvas_zoom_100(&trendCurveWidget);
	if (bCheck)
	{
		gtk_curve_widget_set_data_scale(&trendCurveWidget, eCalcAuto, 0, 0);
	}
	else
	{
		gtk_curve_widget_set_data_scale(&trendCurveWidget, eCalcScale, 0, 0);
	}
	
	gtk_curve_widget_frame_redraw(&trendCurveWidget);
}

void onToggledCheckBtnTrendMark(GtkToggleButton *widget, gpointer data)
{
	gboolean bCheck=gtk_toggle_button_get_active(widget);
	
	if (bCheck)
	{
		gtk_curve_widget_enable(&trendCurveWidget, TRUE, eEnableCurvesMark);
	}
	else
	{
		gtk_curve_widget_enable(&trendCurveWidget, FALSE, eEnableCurvesMark);
	}
	
	gtk_curve_widget_canvas_redraw(&trendCurveWidget);
}

GtkWidget* CreateTrendToolbar (GtkWidget * parent, gint crt_n)
{
	int	i,stn_num,data_type_num;
	GtkWidget *box, *tempwid;
	GtkWidget *toolbar; 
	
	GtkWidget *button_trendfile,*button_exit;
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

	pComboBoxTrendStn=gtk_combo_box_new_with_model(GTK_TREE_MODEL(pStoreStn));

	pRenderStn = gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(pComboBoxTrendStn), pRenderStn, FALSE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(pComboBoxTrendStn), pRenderStn, "text", eStnName, NULL);
	g_object_unref(pStoreStn);

	gtk_widget_set_usize(pComboBoxTrendStn, 130, -1);
	gtk_box_pack_start(GTK_BOX(box), pComboBoxTrendStn, FALSE, FALSE, 5);
	gtk_widget_show(pComboBoxTrendStn);

	g_signal_connect (G_OBJECT(pComboBoxTrendStn), "changed", G_CALLBACK(CreateTrendDevBtn), GINT_TO_POINTER(crt_n));  

	printf("@@@@@ ComboBoxTrendStn end!!!\n\n");

	/*  *************  dev   combobox  ****************** */ 
	tempwid = gtk_label_new ((pConvert=_toUtf8EX(OIX_CSTR_ALM_DEV_FILTER)));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start (GTK_BOX (box), tempwid, FALSE, FALSE, 0);
	gtk_widget_show (tempwid);

	pRenderDev = gtk_cell_renderer_text_new();

	pStoreDev=gtk_list_store_new(eDevColNum, G_TYPE_STRING, G_TYPE_INT);
	pComboBoxTrendDev = gtk_combo_box_new_with_model(GTK_TREE_MODEL(pStoreDev));
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(pComboBoxTrendDev), pRenderDev, FALSE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(pComboBoxTrendDev), pRenderDev, "text", eDevName, NULL);
	g_object_unref(pStoreDev);

	gtk_widget_set_usize(pComboBoxTrendDev, 130, -1);
	gtk_box_pack_start (GTK_BOX (box), pComboBoxTrendDev, FALSE, FALSE, 5);
	gtk_widget_show (pComboBoxTrendDev);

	g_signal_connect (G_OBJECT(pComboBoxTrendDev), "changed", G_CALLBACK(TrendDevSelectCB), GINT_TO_POINTER(crt_n));

	printf("#### cmbTrendDev end\n\n");

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
			gtk_list_store_set(pStoreDataType, &it, eDataTypeName, (pConvert=_toUtf8(datatype_longname)),
				eDataTypeId, i, -1);
			G_SAFE_FREE(pConvert);
		}
	}

	pRenderDataType = gtk_cell_renderer_text_new();
	pComboBoxTrendDataType= gtk_combo_box_new_with_model(GTK_TREE_MODEL(pStoreDataType));
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(pComboBoxTrendDataType), pRenderDataType, FALSE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(pComboBoxTrendDataType), pRenderDataType, "text", 0, NULL);
	g_object_unref(pStoreDataType);
	gtk_widget_set_usize(pComboBoxTrendDataType, 130, -1);
	gtk_box_pack_start (GTK_BOX (box), pComboBoxTrendDataType, FALSE, FALSE, 5);
	gtk_widget_show (pComboBoxTrendDataType);

	gtk_combo_box_set_active(GTK_COMBO_BOX(pComboBoxTrendDataType), 0);
	
	g_signal_connect(G_OBJECT(pComboBoxTrendDataType), "changed", G_CALLBACK(TrendDevSelectCB), GINT_TO_POINTER(crt_n));
	
	/******sgfile button   add   del   *********/ 	
	button_trendfile = gtk_button_new_with_label((pConvert=_toUtf8EX(OIX_CSTR_TREND_SAVE)));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start (GTK_BOX (box), button_trendfile, FALSE, FALSE, 5);
    
	g_signal_connect(G_OBJECT(button_trendfile), "clicked", G_CALLBACK(TrendSaveCB), GINT_TO_POINTER(crt_n));
	gtk_widget_show(button_trendfile);

	pCheckBtnNumeral=gtk_check_button_new_with_label((pConvert=_toUtf8EX("OIX_CSTR_NUMERICAL")));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start(GTK_BOX(box), pCheckBtnNumeral, FALSE, FALSE, 2);
	gtk_widget_show(pCheckBtnNumeral);
	g_signal_connect(G_OBJECT(pCheckBtnNumeral), "toggled", G_CALLBACK(onToggledCheckBtnTrendNumeral), NULL);
	
	pCheckBtnMark=gtk_check_button_new_with_label((pConvert=_toUtf8EX("OIX_CSTR_MARK")));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start(GTK_BOX(box), pCheckBtnMark, FALSE, FALSE, 2);
	gtk_widget_show(pCheckBtnMark);
	g_signal_connect(G_OBJECT(pCheckBtnMark), "toggled", G_CALLBACK(onToggledCheckBtnTrendMark), NULL);

	button_exit = gtk_button_new_with_label((pConvert=_toUtf8EX(OIX_CSTR_EXIT)));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_end(GTK_BOX (box), button_exit, FALSE, FALSE, 2);     	     
	g_signal_connect(G_OBJECT(button_exit), "clicked", G_CALLBACK(TrendDlgExitProc),  GINT_TO_POINTER(crt_n));
	gtk_widget_show (button_exit);
	
	return (toolbar);
}

gboolean writeProgTrendData(const gchar* pFileName, WDQS_DY* pData)
{
	FILE* fp;

#ifdef SUN_OS
	gint i;
#endif

	if (pFileName == NULL || strcmp(pFileName, "") == 0 || pData == NULL)
	{
		return FALSE;
	}

	if ((fp = fopen(pFileName, "wb")) == NULL)
	{
		printf("TrendAdd unable to open file\n");
		return FALSE;
	}
	
#ifdef  SUN_OS
	tmp_trenddata= *pData;
	for( i=0; i<tmp_trenddata.next_pos; i++)
		Swap2Byte(&tmp_trenddata.buffer[i].pt_id);
	Swap2Byte(&tmp_trenddata.next_pos);
	if (fwrite (&tmp_trenddata, sizeof(WDQS_DY), 1, fp) != 1)
		printf("OIX:in TrendAddCB write error\n");
#else		
	if (fwrite (pData, sizeof(WDQS_DY), 1, fp) != 1)
		printf("OIX:in TrendAddCB write error\n");
#endif
	
	fclose (fp);

	return TRUE;
}

gboolean readProgTrendData(const gchar* pFileName, WDQS_DY* pData)
{
	FILE* fp;

#ifdef  SUN_OS
	SHORT	i;
#endif

	if (pFileName == NULL || strcmp(pFileName, "") == 0 || pData == NULL)
	{
		return FALSE;
	}

	memset(pData, 0, sizeof(WDQS_DY));
	if ((fp=fopen(pFileName, "rb")) != NULL)
	{
		if ( fread(pData, sizeof(WDQS_DY), 1, fp) !=1 )
			printf("unable to fread trend file\n");
		fclose( fp ); 
#ifdef  SUN_OS
		Swap2Byte(&pData->next_pos);
#endif
		if(pData->next_pos > MAX_WDQS_COUNT)
		{
			printf("in ProgTrendSet:read error : next_pos=%d > MAX_WDQS_COUNT\n", pData->next_pos);
			pData->next_pos= 0;
			return FALSE;
		}
#ifdef  SUN_OS
		for( i=0; i<pData->next_pos; i++)
			Swap2Byte(&pData->buffer[i].pt_id);
#endif

		return TRUE;
	}

	return FALSE;
}

void trendListDbClick(GtkTreeView *tree_view,GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data)
{
	TrendAddCB(NULL, user_data);
}

void DestroyTrendDlg(GtkWidget *w, gpointer data)
{
	gint crt_n=GPOINTER_TO_INT(data);
	if (crt_n<0 || crt_n>MAX_CRT_NUM)
	{
		return;
	}

	writeProgTrendData(TREND_FILE_SELECT, &trendSelect);
	
	if (trend_curve_timer_id != 0)
	{
		g_source_remove(trend_curve_timer_id);
		trend_curve_timer_id=0;
	}

	if (pColorStackTrend != 0)
	{
		gtk_color_stack_destroy(pColorStackTrend);
		pColorStackTrend=NULL;
	}

	trend_main_w[crt_n]=NULL;

	nTrendRef--;
}

void trendCheckBoxToggled(GtkCellRendererToggle *cell, gchar *path_str, gpointer data)
{
	GtkTreeModel *model = (GtkTreeModel *)data;
	GtkTreeIter  iter;
	GtkTreePath *path = gtk_tree_path_new_from_string (path_str);
	gboolean bCheck;
	POINTER pt; 

	gint nStnId, nDevId, nDataType, nPtId;
	
	gtk_tree_model_get_iter (model, &iter, path);
	gtk_tree_model_get (model, &iter, eTrendCheck, &bCheck, eTrendCheckStnId, &nStnId, eTrendCheckDevId, &nDevId,
		eTrendCheckDataType, &nDataType, eTrendCheckPtId, &nPtId, -1);

	pt.stn_id=(UCHAR)nStnId;
	pt.dev_id=(UCHAR)nDevId;
	pt.data_type=(UCHAR)nDataType;
	pt.pt_id=(USHORT)nPtId;
	
	bCheck ^= 1;
	
	bCheck=trendRedraw(&pt, bCheck);
	
	gtk_list_store_set (GTK_LIST_STORE (model), &iter, eTrendCheck, bCheck, -1);
	
	gtk_tree_path_free (path);
}

gboolean Trend_Add_list_key_release_events(GtkWidget *widget,GdkEventKey *event, gpointer usr_data)
{	
	if (event->keyval == GDK_Delete)
	{
		TrendRemoveCB(NULL, usr_data);
		
		return TRUE;
	}
	
	return FALSE;
}

GtkWidget* CreateProgTrendShell (GtkWidget *parent , int crt_n )
{
	GtkWidget *vbox, *hbox, *vSubBox,*hSubBox;
	GtkWidget  *toolbar,*trend_list_scrollw,*trend_add_list_scrollw;
	GtkWidget	*button_add_point,*button_del_point;
	
	GtkWidget* windows;
	
	GtkWidget *pFrame=NULL;
	
	GtkListStore *pStoreTrend, *pStoreTrendAdd;
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
		
    gtk_window_set_title(GTK_WINDOW(windows), (pConvert=_toUtf8EX(OIX_CSTR_TREND_SET_TITLE)));
	G_SAFE_FREE(pConvert);
    gtk_window_set_transient_for(GTK_WINDOW(windows),GTK_WINDOW(parent));
    g_signal_connect(G_OBJECT(windows), "destroy", G_CALLBACK(DestroyTrendDlg), GINT_TO_POINTER(crt_n));
    
    vbox=gtk_vbox_new(FALSE, 5);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 0);
    gtk_container_add(GTK_CONTAINER(windows), vbox);
    gtk_widget_show(vbox);
    
    toolbar =  CreateTrendToolbar(windows, crt_n);
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
	
	pStoreTrend=gtk_list_store_new(eTrendColNum, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT);
	pListTrend=gtk_tree_view_new_with_model(GTK_TREE_MODEL(pStoreTrend));
	pSelection=gtk_tree_view_get_selection(GTK_TREE_VIEW(pListTrend));
	gtk_tree_selection_set_mode(pSelection, GTK_SELECTION_SINGLE);
	pColumn=gtk_tree_view_column_new_with_attributes((pConvert=_toUtf8EX("OIX_CSTR_TREND_DEV")), pRenderText, 
		"text", eTrendName, NULL);
	G_SAFE_FREE(pConvert);
	gtk_tree_view_append_column (GTK_TREE_VIEW (pListTrend), pColumn);

	g_object_unref(pStoreTrend);
    
    trend_list_scrollw=gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (trend_list_scrollw), GTK_SHADOW_ETCHED_IN);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (trend_list_scrollw), GTK_POLICY_AUTOMATIC, 
		GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(vSubBox), trend_list_scrollw, TRUE, TRUE, 0);
    gtk_widget_show (trend_list_scrollw); 
	
	gtk_container_add (GTK_CONTAINER (trend_list_scrollw), pListTrend);
    gtk_widget_show (pListTrend); 

	g_signal_connect(G_OBJECT(GTK_TREE_VIEW(pListTrend)), "row_activated", G_CALLBACK(trendListDbClick), 
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
	
	g_signal_connect(G_OBJECT(button_add_point), "clicked", G_CALLBACK(TrendAddCB), GINT_TO_POINTER(crt_n));
   	g_signal_connect(G_OBJECT(button_del_point), "clicked", G_CALLBACK(TrendRemoveCB), GINT_TO_POINTER(crt_n));
	
	gtk_widget_show(button_add_point);
	gtk_widget_show(button_del_point);

	pStoreTrendAdd=gtk_list_store_new(eTrendCheckColNum, G_TYPE_BOOLEAN, G_TYPE_STRING, G_TYPE_INT, 
		G_TYPE_INT, G_TYPE_INT, G_TYPE_INT);
	pListTrendAdd =gtk_tree_view_new_with_model(GTK_TREE_MODEL(pStoreTrendAdd));
	pSelection=gtk_tree_view_get_selection(GTK_TREE_VIEW(pListTrendAdd));
	gtk_tree_selection_set_mode(pSelection, GTK_SELECTION_SINGLE);

	pRenderCheckBox=gtk_cell_renderer_toggle_new();
	gtk_cell_renderer_toggle_set_radio(GTK_CELL_RENDERER_TOGGLE(pRenderCheckBox), FALSE);
	g_signal_connect (pRenderCheckBox, "toggled", G_CALLBACK (trendCheckBoxToggled), GTK_TREE_MODEL(pStoreTrendAdd));

	pColumn=gtk_tree_view_column_new_with_attributes(NULL, pRenderCheckBox, "active", 0, NULL);
	gtk_tree_view_column_set_sizing (GTK_TREE_VIEW_COLUMN (pColumn), GTK_TREE_VIEW_COLUMN_FIXED);
	gtk_tree_view_column_set_fixed_width (GTK_TREE_VIEW_COLUMN (pColumn), 30);
	gtk_tree_view_append_column (GTK_TREE_VIEW (pListTrendAdd), pColumn);

	pColumn=gtk_tree_view_column_new_with_attributes((pConvert=_toUtf8EX("OIX_CSTR_TREND_DEV_SEL")), pRenderText, 
		"text", eTrendCheckName, NULL);
	G_SAFE_FREE(pConvert);
	gtk_tree_view_append_column (GTK_TREE_VIEW (pListTrendAdd), pColumn);

	g_object_unref(pStoreTrendAdd);
    
    trend_add_list_scrollw=gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_set_usize (GTK_WIDGET(trend_add_list_scrollw),-1 , 200);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (trend_add_list_scrollw), GTK_SHADOW_ETCHED_IN);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (trend_add_list_scrollw), GTK_POLICY_AUTOMATIC, 
		GTK_POLICY_AUTOMATIC);
	
    gtk_box_pack_start(GTK_BOX(vSubBox), trend_add_list_scrollw, FALSE, FALSE, 0);
    gtk_widget_show (trend_add_list_scrollw);

	gtk_container_add (GTK_CONTAINER (trend_add_list_scrollw), pListTrendAdd);
    gtk_widget_show (pListTrendAdd);

	g_signal_connect (G_OBJECT(pListTrendAdd), "key_release_event", G_CALLBACK(Trend_Add_list_key_release_events), 
		GINT_TO_POINTER(crt_n));

	pFrame=gtk_curve_widget_create(windows, &trendCurveWidget);
	gtk_box_pack_start(GTK_BOX(hbox ), pFrame, TRUE, TRUE, 0);

	gtk_curve_widget_set_text(&trendCurveWidget, (pConvert=_toUtf8EX("OIX_CSTR_TREND_TITLE")), eTextTitle);
	G_SAFE_FREE(pConvert);
	gtk_curve_widget_set_text(&trendCurveWidget, (pConvert=_toUtf8EX("OIX_CSTR_SGZY_PERCENTAGE")), eTextLabel);
	G_SAFE_FREE(pConvert);
	gtk_curve_widget_enable(&trendCurveWidget, TRUE, eEnableAutoSubTitle);

	gtk_combo_box_set_active(GTK_COMBO_BOX(pComboBoxTrendStn), 0);
	
	UpdateTrendList (crt_n);

	gtk_widget_show_all(windows);
	
	gtk_curve_widget_set_current_operation(&trendCurveWidget, eCurveOpPointer);
	gtk_curve_widget_frame_redraw(&trendCurveWidget);

	return windows;
}

void ProgTrendSet (int  crt_n )
{
	if (crt_n<0 || crt_n>=MAX_CRT_NUM)
	{
		gdk_beep();
		return;
	}

	if (trend_main_w[crt_n] == NULL)
	{
		if (nTrendRef >= 1)
		{
			gdk_beep(); 
			PopupMessageBox(main_w[crt_n] , "OIX: Trend Setting has opened !!");
			return;
		}
		nTrendRef++;

		pColorStackTrend=gtk_color_stack_create(curveColor, TREND_MAX_CURVE_NUM);

		readProgTrendData(TREND_FILE_SELECT, &trendSelect);
		readProgTrendData(TREND_FILE, &trenddata);

		trend_pt	= LinktoShareMem(SHM_WDQS_KEY_CHAR );
		if( trend_pt == (void *)-1)
		{
			perror("in ProgTrendSet ERROR: trend_pt= -1!!!"); 
			nTrendRef--;
			return;
		}	

		trend_main_w[crt_n]=CreateProgTrendShell(main_w[crt_n], crt_n);
	}
	else
	{
		gtk_window_present(GTK_WINDOW(trend_main_w[crt_n]));
	}
}
