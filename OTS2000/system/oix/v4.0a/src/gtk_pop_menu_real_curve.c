#define	 POP_MENU_REAL_CUEVE
#ifdef   POP_MENU_REAL_CUEVE

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

extern  CURVE_SHM_DATA *shm_real_curve;

extern CURVE_DEF   	RealCCurvePt;
#ifdef  SUN_OS
extern CURVE_DEF tmpRealCCurvePt ;  /*RealCCurvePt--Global Variant*/
#endif

#define OIX_CSTR_DISPLAY_START_TIME "OIX_CSTR_DISPLAY_START_TIME"


GtkWidget 	*pop_menu_real_curve_main_w[MAX_CRT_NUMBER];
gint pop_menu_real_curve_timer_id[MAX_CRT_NUMBER];

//实时曲线
typedef struct
{
	GtkWidget*	pButton[eCurveOpEnd];

	gint nToolType;					//工具按钮，0:十字指针 1:手指针 2:放大 3:缩小 4:区域放大

	struct tm startTime, endTime;
	struct tm nowTime;
}SPopMenuRealTimeCurve;

SPopMenuRealTimeCurve* sPopMenuRealCurve[MAX_CRT_NUMBER];
GtkCurveWidget popMenuCurveWidget[MAX_CRT_NUMBER];

enum {eItemName, eItemStnId, eItemDevId, eItemDataType, eItemPtId, eItemColEnd};

/********************** end ********************************/

extern char *_toUtf8(char *c);
extern void	PopupMessageBox (GtkWidget *parent,char msg[] );
extern char *_toUtf8EX(char *c);

extern void popupErrorMsgBox(GtkWidget* pParent, gchar* strMsg);
extern void popupInfoMsgBox(GtkWidget* pParent, gchar* strMsg);
extern GtkWidget* create_pixmap (GtkWidget *widget, const gchar *filename);
extern GdkPixbuf* create_pixbuf (const gchar *filename);

extern GdkCursor* createCursor(GtkWidget* pParent, const gchar *filename, gboolean bCenter);

gboolean updatePopMenuRealData(gpointer data);
void updatePopMenuRealCurves(POINTER* ptr, gint crt_n);

GtkWidget *CreatePopMenuRealCurvToolbar (GtkWidget * parent, gint crt_n);
static GtkWidget *CreateProgPopMenuRealCurveShell (GtkWidget *parent , int  crt_n );

/*
 * 点击后 将横坐标改为输入时间范围
 */
void addCurveToPopMenuRealCurves(POINTER* ptr,   int crt_n)
{
	struct tm startTime, endTime;
	struct tm  tm_now;
	time_t   now;

	gdouble dStart, dEnd;

//	gchar strText[128];
//	gchar *pConvert=0;

	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER || ptr == NULL)
		return ;

	time(&now);
	tm_now = *localtime(&now);

	startTime=tm_now;
	endTime=tm_now;

	startTime.tm_hour-=12;
	startTime.tm_isdst=-1;
	mktime(&startTime);

	endTime.tm_hour+=12;
	endTime.tm_isdst=-1;
	mktime(&endTime);
	
	sPopMenuRealCurve[crt_n]->startTime=startTime;
	sPopMenuRealCurve[crt_n]->endTime=endTime;
	sPopMenuRealCurve[crt_n]->nowTime=tm_now;

//  	sprintf(strText, "%04d-%02d-%02d %02d:%02d --- %04d-%02d-%02d %02d:%02d", 
//  		startTime.tm_year+1900, startTime.tm_mon+1, startTime.tm_mday,startTime.tm_hour, startTime.tm_min,
// 		endTime.tm_year+1900, endTime.tm_mon+1, endTime.tm_mday, endTime.tm_hour, endTime.tm_min);
//  	gtk_curve_widget_set_text(&popMenuCurveWidget[crt_n], strText, eTextTitle);

	gtk_curve_widget_enable(&popMenuCurveWidget[crt_n], FALSE, eEnableAutoSubTitle);
	gtk_curve_widget_set_text(&popMenuCurveWidget[crt_n], "", eTextSubtitle);
	gtk_curve_widget_canvas_zoom_100(&popMenuCurveWidget[crt_n]);

	dStart=mktime(&(sPopMenuRealCurve[crt_n]->startTime));
	dEnd=mktime(&(sPopMenuRealCurve[crt_n]->endTime));

	gtk_curve_widget_set_time_scale(&popMenuCurveWidget[crt_n], dStart, dEnd);

	dStart=mktime(&tm_now);
	gtk_curve_widget_canvas_set_zoom_rect(&popMenuCurveWidget[crt_n], dStart, dStart+60);

	updatePopMenuRealCurves(ptr, crt_n);


	gtk_curve_widget_frame_redraw(&popMenuCurveWidget[crt_n]);
	
	return;
}

void updatePopMenuRealCurves(POINTER* ptr, gint crt_n)
{
	DMS_COMMON	pdt;
	char* pConvert=0;

	char strStn[STATION_LONGNAME_SIZE],strDev[GROUP_LONGNAME_SIZE];
	char strEntryName[ENTRY_LONGNAME_SIZE];	
	char strLongName[128];

	GCurve* pCurve;

	if( crt_n < 0 || crt_n >= MAX_CRT_NUMBER || sPopMenuRealCurve[crt_n] == NULL || ptr == NULL)
		return ;

	if (pop_menu_real_curve_timer_id[crt_n] != 0)
	{
		g_source_remove(pop_menu_real_curve_timer_id[crt_n]);
	}

	gtk_curve_widget_clear_all_curve(&popMenuCurveWidget[crt_n]);

	pCurve=gtk_curve_create(DATA_NUM, eModeSec);
	gtk_curve_set_color(pCurve, &pixels[crt_n][GREEN1]);
		
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
		
	gtk_curve_widget_add_curve(&popMenuCurveWidget[crt_n], pCurve);
	

	pop_menu_real_curve_timer_id[crt_n] = g_timeout_add (1000, (GSourceFunc)updatePopMenuRealData, GINT_TO_POINTER(crt_n));

	updatePopMenuRealData(GINT_TO_POINTER(crt_n));
}

void popMenuReadCurveToolBarMgr(int crt_n, gint nToolType)
{
	gint nType;
	
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sPopMenuRealCurve[crt_n]->pButton[nToolType])))
	{
		if (nToolType != sPopMenuRealCurve[crt_n]->nToolType)
		{
			nType=sPopMenuRealCurve[crt_n]->nToolType;
			sPopMenuRealCurve[crt_n]->nToolType=nToolType;
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(sPopMenuRealCurve[crt_n]->pButton[nType]), FALSE);
		}
	}
	else
	{
		if (nToolType == sPopMenuRealCurve[crt_n]->nToolType)
		{
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(sPopMenuRealCurve[crt_n]->pButton[nToolType]), TRUE);
		}
	}
	
	gtk_curve_widget_set_current_operation(&popMenuCurveWidget[crt_n], nToolType);
}

void onToggledBtnPopMenuRealCurveScale(GtkWidget *widget, int crt_n, int type)
{
	if (type>=eCurveOpEnd)
	{
		return;
	}
	
	popMenuReadCurveToolBarMgr(crt_n, type);
}

void onClickedBtnTrack(GtkWidget *widget, gpointer data)
{
	int crt_n=GPOINTER_TO_INT(data);
	if( crt_n < 0 || crt_n >= MAX_CRT_NUMBER || sPopMenuRealCurve[crt_n] == NULL)
		return ;
	
	onToggledBtnPopMenuRealCurveScale(widget, crt_n, eCurveOpPointer);
}

void onClickedBtnHand(GtkWidget *widget, gpointer data)
{
	int crt_n=GPOINTER_TO_INT(data);
	if( crt_n < 0 || crt_n >= MAX_CRT_NUMBER || sPopMenuRealCurve[crt_n] == NULL)
		return ;
	
	onToggledBtnPopMenuRealCurveScale(widget, crt_n, eCurveOpHand);
}

void onClickedBtnZoomIn(GtkWidget *widget, gpointer data)
{
	int crt_n=GPOINTER_TO_INT(data);
	if( crt_n < 0 || crt_n >= MAX_CRT_NUMBER || sPopMenuRealCurve[crt_n] == NULL)
		return ;
	
	onToggledBtnPopMenuRealCurveScale(widget, crt_n, eCurveOpZoomIn);
}

void onClickedBtnZoomOut(GtkWidget *widget, gpointer data)
{
	int crt_n=GPOINTER_TO_INT(data);
	if( crt_n < 0 || crt_n >= MAX_CRT_NUMBER || sPopMenuRealCurve[crt_n] == NULL)
		return ;
	
	onToggledBtnPopMenuRealCurveScale(widget, crt_n, eCurveOpZoomOut);
}

void onClickedBtnZoomRect(GtkWidget *widget, gpointer data)
{
	int crt_n=GPOINTER_TO_INT(data);
	if( crt_n < 0 || crt_n >= MAX_CRT_NUMBER || sPopMenuRealCurve[crt_n] == NULL)
		return ;

	onToggledBtnPopMenuRealCurveScale(widget, crt_n, eCurveOpZoomRect);
}

void onClickedBtnZoom100(GtkWidget *widget, gpointer data)
{
	gdouble dStart;
	int crt_n=GPOINTER_TO_INT(data);
	if( crt_n < 0 || crt_n >= MAX_CRT_NUMBER || sPopMenuRealCurve[crt_n] == NULL)
		return ;

	gtk_curve_widget_canvas_zoom_100(&popMenuCurveWidget[crt_n]);

	dStart=mktime(&sPopMenuRealCurve[crt_n]->nowTime);
	gtk_curve_widget_canvas_set_zoom_rect(&popMenuCurveWidget[crt_n], dStart, dStart+60);
	
	gtk_curve_widget_frame_redraw(&popMenuCurveWidget[crt_n]);
}

void onToggledBtnCheckNumeral(GtkToggleButton *togglebutton, gpointer data)
{
	gboolean bCheck=FALSE;

	int crt_n=GPOINTER_TO_INT(data);
	if( crt_n < 0 || crt_n >= MAX_CRT_NUMBER || sPopMenuRealCurve[crt_n] == NULL)
		return ;

	bCheck=gtk_toggle_button_get_active(togglebutton);
	gtk_curve_widget_canvas_zoom_100(&popMenuCurveWidget[crt_n]);
	if (bCheck)
	{
		gtk_curve_widget_set_data_scale(&popMenuCurveWidget[crt_n], eCalcAuto, 0, 0);
	}
	else
	{
		gtk_curve_widget_set_data_scale(&popMenuCurveWidget[crt_n], eCalcScale, 0, 0);
	}
	
	gtk_curve_widget_frame_redraw(&popMenuCurveWidget[crt_n]);
}

void onToggledBtnCheckMark(GtkToggleButton *widget, gpointer data)
{
	gboolean bCheck=FALSE;
	
	int crt_n=GPOINTER_TO_INT(data);
	if( crt_n < 0 || crt_n >= MAX_CRT_NUMBER || sPopMenuRealCurve[crt_n] == NULL)
		return ;

	bCheck=gtk_toggle_button_get_active(widget);

	if (bCheck)
	{
		gtk_curve_widget_enable(&popMenuCurveWidget[crt_n], TRUE, eEnableCurvesMark);
	}
	else
	{
		gtk_curve_widget_enable(&popMenuCurveWidget[crt_n], FALSE, eEnableCurvesMark);
	}
	
	gtk_curve_widget_canvas_redraw(&popMenuCurveWidget[crt_n]);
}

void ExitPopMenuRealCurveShell(GtkWidget *widget,   gpointer data)
{
	int crt_n = (gint) data ;

	if( crt_n < 0 || crt_n >= MAX_CRT_NUMBER)
		return ;

	gtk_widget_destroy(GTK_WIDGET(pop_menu_real_curve_main_w[crt_n])); 
}

void DestoryPopMenuRealCurveShell(GtkWidget *widget,   gpointer data)
{
	int crt_n = (gint) data ;
	
	if( crt_n < 0 || crt_n >= MAX_CRT_NUMBER)
		return ; 
	
	g_message("\n quit realcurve !!!!! \n");
	
	if (pop_menu_real_curve_timer_id[crt_n] != 0)
	{
		g_source_remove(pop_menu_real_curve_timer_id[crt_n]);
		pop_menu_real_curve_timer_id[crt_n]  = 0 ;
	}
	
	pop_menu_real_curve_main_w[crt_n] = NULL ;

	free(sPopMenuRealCurve[crt_n]);
	sPopMenuRealCurve[crt_n]=NULL;
}

gboolean updatePopMenuRealData(gpointer data)
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
	gdouble* pBuffer;

	int crt_n=GPOINTER_TO_INT(data);
	if (crt_n < 0 || crt_n > MAX_CRT_NUMBER || sPopMenuRealCurve[crt_n] == NULL)
	{
		return FALSE;
	}

	nCurveCount=gtk_curve_widget_get_curve_count(&popMenuCurveWidget[crt_n]);
	if (nCurveCount <= 0)
	{
		return FALSE;
	}

	dCurTime=shm_real_curve->nTime;
	dStartTime=mktime(&(sPopMenuRealCurve[crt_n]->startTime));

	nBufLen=(gint)(dCurTime-dStartTime);
	if (nBufLen <= 0)
	{
		for (nIndex=0; nIndex < nCurveCount; nIndex++)
		{
			pCurve=gtk_curve_widget_get_curve(&popMenuCurveWidget[crt_n], nIndex);
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

		gtk_curve_widget_frame_redraw(&popMenuCurveWidget[crt_n]);
		return TRUE;
	}

	nBufLen=nBufLen<DATA_NUM? nBufLen:DATA_NUM;

	nNextPt=shm_real_curve->nextpt;
	for (nIndex=0; nIndex < nCurveCount; nIndex++)
	{
		pCurve=gtk_curve_widget_get_curve(&popMenuCurveWidget[crt_n], nIndex);
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

 			pBuffer=(gdouble*)malloc(nBufLen*sizeof(gdouble));
			
			if(pdt.point.data_type==ANA_TYPE)
			{
				for (i=0; i<nBufLen; i++)
				{
					pBuffer[i]=shm_real_curve->dat.fvalue[nPtId][(nNextPt-1-nBufLen+DATA_NUM+i)%(DATA_NUM)];
				}
			}
			else
			{
				for (i=0; i<nBufLen; i++)
				{
					pBuffer[i]=shm_real_curve->dat.status[nPtId][(nNextPt-1-nBufLen+DATA_NUM+i)%(DATA_NUM)];
				}
			}
			
			gtk_curve_set_data(pCurve, &dStartTime , pBuffer, nBufLen, 0, nBufLen, 1);
			gtk_curve_set_boundary(pCurve, TRUE, Y_start, Y_end);

			free(pBuffer);
		}
	}

	gtk_curve_widget_canvas_redraw(&popMenuCurveWidget[crt_n]);

	return TRUE;
}

void ProgPopMenuRealCurve (GtkWidget *parent, POINTER ptr ,int  crt_n)
{
	if (crt_n<0 ||crt_n>=MAX_CRT_NUMBER)
	{
		gdk_beep();
		return;
	}
	
	if (pop_menu_real_curve_main_w[crt_n]==(GtkWidget*)NULL)
	{
		sPopMenuRealCurve[crt_n]=(SPopMenuRealTimeCurve*)malloc(sizeof(SPopMenuRealTimeCurve));
		pop_menu_real_curve_main_w[crt_n]  = CreateProgPopMenuRealCurveShell (parent, crt_n);
		
		if (pop_menu_real_curve_timer_id[crt_n]  !=0 )
			gtk_timeout_remove(pop_menu_real_curve_timer_id[crt_n] );

		gtk_widget_show_all(pop_menu_real_curve_main_w[crt_n] );
		
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(
			sPopMenuRealCurve[crt_n]->pButton[sPopMenuRealCurve[crt_n]->nToolType]), TRUE);
		gtk_curve_widget_set_current_operation(&popMenuCurveWidget[crt_n], eCurveOpPointer);
		gtk_curve_widget_set_data_scale(&popMenuCurveWidget[crt_n], eCalcAuto, 0, 0);
	}
	
	gtk_window_present(GTK_WINDOW(pop_menu_real_curve_main_w[crt_n]) );

	addCurveToPopMenuRealCurves(&ptr, crt_n);
	gtk_curve_widget_frame_redraw(&popMenuCurveWidget[crt_n]);
}


static GtkWidget *CreateProgPopMenuRealCurveShell (GtkWidget *parent , int  crt_n )
{
	GtkWidget  *windows;
	GtkWidget  *vbox, *toolbar, *frame;

	gchar *pConvert=0;
	
	windows=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	
	gtk_window_set_default_size(GTK_WINDOW(windows), 400, 300);
	
	gtk_window_set_title(GTK_WINDOW(windows), (pConvert=_toUtf8EX("MSG_REAL_TIME_CURVE")));
	G_SAFE_FREE(pConvert);

	gtk_window_set_transient_for(GTK_WINDOW(windows),GTK_WINDOW(parent));
	
	gtk_signal_connect(GTK_OBJECT(windows), "destroy", GTK_SIGNAL_FUNC( DestoryPopMenuRealCurveShell), (gpointer)crt_n);
	
	vbox=gtk_vbox_new(FALSE, 5);
	gtk_container_set_border_width(GTK_CONTAINER(vbox), 0);
	gtk_container_add(GTK_CONTAINER(windows), vbox);
	gtk_widget_show(vbox);
	
	toolbar =  CreatePopMenuRealCurvToolbar(windows, crt_n);
	gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, TRUE, 0);
	gtk_widget_show(toolbar);    
	
	/***************** draw area code ***************************************/
	
	frame=gtk_curve_widget_create(windows, &popMenuCurveWidget[crt_n]);
	gtk_box_pack_start(GTK_BOX(vbox ), frame, TRUE, TRUE, 0);
	
	gtk_curve_widget_enable(&popMenuCurveWidget[crt_n], TRUE, eEnableScale);
	gtk_curve_widget_enable(&popMenuCurveWidget[crt_n], TRUE, eEnableAutoSubTitle);
	sPopMenuRealCurve[crt_n]->nToolType=eCurveOpPointer;

	return windows;
}

GtkWidget *CreatePopMenuRealCurvToolbar (GtkWidget * parent, gint crt_n)
{
	GtkWidget *hbox;  		
	GtkWidget *toolbar;   	
	GtkWidget *pBtnExit,*pBtnPointer,*pBtnHand,*pBtnZoomIn,*pBtnZoomOut,*pBtnZoomRect,*pBtnZoom100;
	GtkWidget *pCheckBtnMark;

	gchar *pConvert=0;

	GtkWidget *radiobtnNum;
	GSList * radiobutton_group = NULL;
	GtkWidget *radiobtnPcent;

	GtkWidget  *sep1,*sep2 ;

	if( crt_n < 0 || crt_n >= MAX_CRT_NUMBER || sPopMenuRealCurve[crt_n] == NULL)
	{
		printf("CreatePopMenuRealCurvToolbar Failed!");
		return NULL;
	}
	
	
	toolbar = gtk_handle_box_new();
	
	hbox = gtk_hbox_new (FALSE, 4);
	gtk_container_add (GTK_CONTAINER (toolbar), hbox);
	gtk_container_border_width (GTK_CONTAINER (hbox), 5);
	gtk_widget_show (hbox);
	
	/******************  display  button   *********************/
	pBtnPointer=gtk_toggle_button_new();
	sPopMenuRealCurve[crt_n]->pButton[eCurveOpPointer]=pBtnPointer;

#if !(GTK_CHECK_VERSION(2, 6, 0))
	gtk_button_set_label(GTK_BUTTON(pBtnPointer), (pConvert=_toUtf8EX(OIX_CSTR_TRACK)));
	G_SAFE_FREE(pConvert);
#else
	gtk_button_set_image(GTK_BUTTON(pBtnPointer), create_pixmap (parent, "pointer1.png"));
#endif
	gtk_box_pack_start (GTK_BOX (hbox), pBtnPointer, FALSE, FALSE, 2); 
	gtk_widget_show (pBtnPointer);	
	gtk_signal_connect(GTK_OBJECT(pBtnPointer), "toggled", GTK_SIGNAL_FUNC(onClickedBtnTrack), 
		GINT_TO_POINTER(eCurveOpPointer));

	pBtnHand= gtk_toggle_button_new();
	sPopMenuRealCurve[crt_n]->pButton[eCurveOpHand]=pBtnHand;
#if !(GTK_CHECK_VERSION(2, 6, 0))
	gtk_button_set_label(GTK_BUTTON(pBtnHand), (pConvert=_toUtf8EX(OIX_CSTR_HAND)));
	G_SAFE_FREE(pConvert);
#else
	gtk_button_set_image(GTK_BUTTON(pBtnHand), create_pixmap (parent, "hand.png"));
#endif
	
	gtk_box_pack_start (GTK_BOX (hbox), pBtnHand, FALSE, FALSE, 2); 
	gtk_widget_show (pBtnHand);	
	gtk_signal_connect(GTK_OBJECT(pBtnHand), "toggled", GTK_SIGNAL_FUNC(onClickedBtnHand), 
		GINT_TO_POINTER(crt_n));

	pBtnZoomIn= gtk_toggle_button_new();
	sPopMenuRealCurve[crt_n]->pButton[eCurveOpZoomIn]=pBtnZoomIn;
#if !(GTK_CHECK_VERSION(2, 6, 0))
	gtk_button_set_label(GTK_BUTTON(pBtnZoomIn), (pConvert=_toUtf8EX(OIX_CSTR_ZOOM_IN)));
	G_SAFE_FREE(pConvert);
#else
	gtk_button_set_image(GTK_BUTTON(pBtnZoomIn), create_pixmap (parent, "zoomin.png"));
#endif
	
	gtk_box_pack_start (GTK_BOX (hbox), pBtnZoomIn, FALSE, FALSE, 2); 
	gtk_widget_show (pBtnZoomIn);	
	gtk_signal_connect(GTK_OBJECT(pBtnZoomIn), "toggled", GTK_SIGNAL_FUNC(onClickedBtnZoomIn),
		GINT_TO_POINTER(crt_n));
	
	pBtnZoomOut= gtk_toggle_button_new();
	sPopMenuRealCurve[crt_n]->pButton[eCurveOpZoomOut]=pBtnZoomOut;
#if !(GTK_CHECK_VERSION(2, 6, 0))
	gtk_button_set_label(GTK_BUTTON(pBtnZoomOut), (pConvert=_toUtf8EX(OIX_CSTR_ZOOM_OUT)));
	G_SAFE_FREE(pConvert);
#else
	gtk_button_set_image(GTK_BUTTON(pBtnZoomOut), create_pixmap (parent, "zoomout.png"));
#endif
	
	gtk_box_pack_start (GTK_BOX (hbox), pBtnZoomOut, FALSE, FALSE, 2); 
	gtk_widget_show (pBtnZoomOut);	
	gtk_signal_connect(GTK_OBJECT(pBtnZoomOut), "toggled", GTK_SIGNAL_FUNC(onClickedBtnZoomOut), 
		GINT_TO_POINTER(crt_n));

	pBtnZoomRect= gtk_toggle_button_new();
	sPopMenuRealCurve[crt_n]->pButton[eCurveOpZoomRect]=pBtnZoomRect;
#if !(GTK_CHECK_VERSION(2, 6, 0))
	gtk_button_set_label(GTK_BUTTON(pBtnZoomRect), (pConvert=_toUtf8EX(OIX_CSTR_ZOOM_RECT)));
	G_SAFE_FREE(pConvert);
#else
	gtk_button_set_image(GTK_BUTTON(pBtnZoomRect), create_pixmap (parent, "zoomrect.png"));
#endif
	
	gtk_box_pack_start (GTK_BOX (hbox), pBtnZoomRect, FALSE, FALSE, 2); 
	gtk_widget_show (pBtnZoomRect);	
	gtk_signal_connect(GTK_OBJECT(pBtnZoomRect), "toggled", GTK_SIGNAL_FUNC(onClickedBtnZoomRect),
		GINT_TO_POINTER(crt_n));

	pBtnZoom100= gtk_button_new();
#if !(GTK_CHECK_VERSION(2, 6, 0))
	gtk_button_set_label(GTK_BUTTON(pBtnZoom100), (pConvert=_toUtf8EX(OIX_CSTR_ZOOM_FIT)));
	G_SAFE_FREE(pConvert);
#else
	gtk_button_set_image(GTK_BUTTON(pBtnZoom100), create_pixmap (parent, "zoomfit.png"));
#endif
	
	gtk_box_pack_start (GTK_BOX (hbox), pBtnZoom100, FALSE, FALSE, 2); 
	gtk_widget_show (pBtnZoom100);	
	gtk_signal_connect(GTK_OBJECT(pBtnZoom100), "clicked", GTK_SIGNAL_FUNC(onClickedBtnZoom100), GINT_TO_POINTER(crt_n));


	sep1 = gtk_separator_tool_item_new();
	gtk_box_pack_start(GTK_BOX(hbox), sep1, FALSE, FALSE, 2);

	radiobtnNum = gtk_radio_button_new_with_mnemonic (NULL, pConvert=_toUtf8EX(OIX_CSTR_NUMERICAL));
	G_SAFE_FREE(pConvert);
	gtk_widget_show (radiobtnNum);
	gtk_box_pack_start(GTK_BOX(hbox), radiobtnNum, FALSE, FALSE, 2);
	gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobtnNum), radiobutton_group);
	radiobutton_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobtnNum));
	g_signal_connect ((gpointer) radiobtnNum, "toggled",G_CALLBACK (onToggledBtnCheckNumeral), GINT_TO_POINTER(crt_n));	

	radiobtnPcent = gtk_radio_button_new_with_mnemonic (NULL, pConvert=_toUtf8EX(OIX_CSTR_PERCENT));
	G_SAFE_FREE(pConvert);
	gtk_widget_show (radiobtnPcent);
	gtk_box_pack_start(GTK_BOX(hbox), radiobtnPcent, FALSE, FALSE, 2);
	gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobtnPcent), radiobutton_group);
	radiobutton_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobtnPcent));
	g_signal_connect ((gpointer) radiobtnPcent, "toggled",G_CALLBACK (onToggledBtnCheckNumeral), GINT_TO_POINTER(crt_n));
	
	sep2 = gtk_separator_tool_item_new();
	gtk_box_pack_start(GTK_BOX(hbox), sep2, FALSE, FALSE, 2);

	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radiobtnNum),TRUE) ;

	pCheckBtnMark=gtk_check_button_new_with_label((pConvert=_toUtf8EX(OIX_CSTR_MARK)));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start(GTK_BOX(hbox), pCheckBtnMark, FALSE, FALSE, 2);
	gtk_widget_show(pCheckBtnMark);
	g_signal_connect(G_OBJECT(pCheckBtnMark), "toggled", G_CALLBACK(onToggledBtnCheckMark), GINT_TO_POINTER(crt_n));
	
    pBtnExit = gtk_button_new_with_label((pConvert=_toUtf8EX( OIX_CSTR_EXIT)));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_end (GTK_BOX (hbox), pBtnExit, FALSE, FALSE, 0); 
	gtk_widget_show (pBtnExit);
	
	gtk_signal_connect(GTK_OBJECT(pBtnExit), "clicked", GTK_SIGNAL_FUNC(ExitPopMenuRealCurveShell), GINT_TO_POINTER(crt_n));
	
	
	
	
	/***********************************************************************/
	
	
	return (toolbar);
}




#endif


