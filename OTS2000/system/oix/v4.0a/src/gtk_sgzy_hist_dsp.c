/*** 
updated by zyp on 2005.11 by gtk
SYNC by hcl 2007.12.09
***/

#include <stdio.h>
#include <sys/types.h>


#define	MAX_SGZY_FILE_NAME_LEN	100
#include "../inc/oix.h"

#include "../inc/gtk_curve_widget.h"

char	sgzy_hist_file_name[MAX_SGZY_FILE_NAME_LEN]; 
FILE	*OpenFileX();


extern void	PopupMessageBox (GtkWidget *parent,char msg[] );
extern void	ProgHardCopy(int crt_n ,int print_mode);
extern char *_toUtf8EX(char *c);
extern char *_toUtf8(char *c);
extern char *_fromUtf8(char *c);
extern GdkPixbuf* create_pixbuf (const gchar *filename);

#define MAX_SGZY_HIS_CURVE_WIDGET 4
GtkCurveWidget sgzyHisCurveWidget[MAX_SGZY_HIS_CURVE_WIDGET];
gint nCurPageNum=0;

GtkWidget *btnSgzyPageUp, *btnSgzyPageDn;

gdouble dYValue[SGZY_NUM];

int nSgzyHistoryRef=0;

void updataSgzyToolBar();
void sgzyHisRedraw(gint crt_n);

//打印
#if (GTK_CHECK_VERSION(2, 10, 0))

GtkPrintSettings* printSettings=NULL;
enum EPrintContents {PrintSgzyHis=0, PrintTrendHis};

#define MAX_TREND_HIS_CURVE_WIDGET 4
extern GtkCurveWidget trendHisCurveWidget[MAX_TREND_HIS_CURVE_WIDGET];

void beginPrint (GtkPrintOperation* operation, GtkPrintContext* context, gpointer user_data)
{
	gtk_print_operation_set_n_pages (operation, 1);
}

void drawPage (GtkPrintOperation* operation, GtkPrintContext* context, gint page_nr, gpointer user_data)
{
	cairo_t *cr;
	gdouble dWidth, dHeight;
	gint nIndex;

	gint nContents=GPOINTER_TO_INT(user_data);
	
	cr = gtk_print_context_get_cairo_context(context);
	dWidth = gtk_print_context_get_width(context);
	dHeight=gtk_print_context_get_height(context);
	
	cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
	gtk_curve_widget_draw_rectangle(cr, 0, 0, dWidth, dHeight, TRUE);
	
	switch (nContents)
	{
	case PrintSgzyHis:
		{
			for (nIndex=0; nIndex<MAX_SGZY_HIS_CURVE_WIDGET; nIndex++)
			{
				gtk_curve_widget_print(cr, &sgzyHisCurveWidget[nIndex], (nIndex%2)*dWidth/2, (nIndex/2)*dHeight/2,
					dWidth/2, dHeight/2);
			}
		}
		break;
	case PrintTrendHis:
		{
			for (nIndex=0; nIndex<MAX_TREND_HIS_CURVE_WIDGET; nIndex++)
			{
				gtk_curve_widget_print(cr, &trendHisCurveWidget[nIndex], (nIndex%2)*dWidth/2, (nIndex/2)*dHeight/2,
					dWidth/2, dHeight/2);
			}
		}
		break;
	default:
		break;
	}
}

void endPrint(GtkPrintOperation* operation, GtkPrintContext* context, gpointer user_data)
{
	char *pConvert=0;

	GtkWidget* parent=(GtkWidget*)user_data;
	GtkWidget* dialog=gtk_message_dialog_new (GTK_WINDOW(parent),
		GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE,
		(pConvert=_toUtf8EX("OIX_CSTR_PRINT_FINISHED")));
	G_SAFE_FREE(pConvert);
	gtk_dialog_run (GTK_DIALOG (dialog));
	gtk_widget_destroy (dialog);
}

void onPrint(GtkWidget *parent, gint nContents)		//打印
{
	GtkPrintOperation* operation;
	GError* error=NULL;
	GtkPrintOperationResult res;
	
	operation = gtk_print_operation_new ();

	if (printSettings == NULL)
	{
		printSettings=gtk_print_settings_new();
		gtk_print_settings_set_orientation(printSettings, GTK_PAGE_ORIENTATION_LANDSCAPE);
	}

	gtk_print_operation_set_print_settings(operation, printSettings);
	
	g_signal_connect (G_OBJECT (operation), "begin_print",  G_CALLBACK (beginPrint), GINT_TO_POINTER(nContents));
	g_signal_connect (G_OBJECT (operation), "draw_page", G_CALLBACK (drawPage), GINT_TO_POINTER(nContents));
	g_signal_connect (G_OBJECT (operation), "end_print", G_CALLBACK (endPrint), (gpointer)parent);
	
	res=gtk_print_operation_run (operation, GTK_PRINT_OPERATION_ACTION_PRINT_DIALOG, GTK_WINDOW (parent), &error);
	
	if (res == GTK_PRINT_OPERATION_RESULT_ERROR)
    {
		GtkWidget *dialog;
		
		dialog = gtk_message_dialog_new (GTK_WINDOW (parent), GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL,
			GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "%s", error->message);
		g_error_free (error);
		
		g_signal_connect (dialog, "response", G_CALLBACK (gtk_widget_destroy), NULL);
		
		gtk_widget_show (dialog);
    }
	else if (res == GTK_PRINT_OPERATION_RESULT_APPLY)
	{
		if (printSettings != NULL)
		{
			g_object_unref (printSettings);
		}
		printSettings=g_object_ref (gtk_print_operation_get_print_settings (operation));
	}

	g_object_unref (operation);

// 	cairo_t *cr;
// 	cairo_surface_t* surface=NULL;
// 	gdouble dWidth=1280, dHeight=800;
// 	gint nIndex;
// 
// 	surface=cairo_image_surface_create(CAIRO_FORMAT_ARGB32, (gint)dWidth, (gint)dHeight);
// 	cr=cairo_create(surface);
// 
// 	cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
// 	gtk_curve_widget_draw_rectangle(cr, 0, 0, dWidth, dHeight, TRUE);
// 
// 	for (nIndex=0; nIndex<MAX_SGZY_HIS_CURVE_WIDGET; nIndex++)
// 	{
// 		gtk_curve_widget_print(cr, &sgzyHisCurveWidget[nIndex], (nIndex%2)*dWidth/2, (nIndex/2)*dHeight/2,
// 				dWidth/2, dHeight/2);
// 	}
// 	cairo_surface_write_to_png(surface, "f:/gtk_print.png");
// 	cairo_destroy(cr);
// 	cairo_surface_destroy(surface);
}
#endif

 void    PrintSgzyHist (GtkButton* w, gpointer user_data)
{
	int	crt_n=GPOINTER_TO_INT(user_data);
	
//	GetCrtNumberOfWgt (w, &crt_n);
	if( crt_n < 0 || crt_n > MAX_CRT_NUM)
		return;
/*#ifdef SUN_OS
	char exec_print[140] =  {""};;
	printf("##### in PrintSgzyHist\n");
	strcpy (exec_print, "/PASSRC/zydpr.dxe ");
	strcat (exec_print, sgzy_hist_file_name);
	system (exec_print);
#endif
	
#ifdef WINDOWS_OS*/	
	if(!CheckUserPrivilege(cur_usr_name, PRIV_PRINT)) //判断打印权限
	{
		PopupMessageBox ( main_w[crt_n], MSG_RPT_PRINTING  ); 
		return  ;
	}

#if !(GTK_CHECK_VERSION(2, 10, 0)) || !defined(WINDOWS_OS)
	ProgHardCopy( crt_n ,SGZY_MODE);
#else
	onPrint(sgzy_history_main_w[crt_n], PrintSgzyHis);
#endif
/*#endif*/	
} 

void	ReadSgzyFile (int crt_n, char	*filename)
{
	FILE      *fp;
	gint j;

#ifdef  SUN_OS
	gint i;
#endif   
	
	fp	= OpenFileX( filename );
	if ( fp == NULL)
	{
		gdk_beep();
		return;
	}
	
	fread(&sgzy_hist_da->alarm,sizeof(INT_ALRM),1,fp);
#ifdef  SUN_OS
	Swap2Byte(&sgzy_hist_da->alarm.ptr.pt_id);
	Swap2Byte(&sgzy_hist_da->alarm.status);
	Swap2Byte(&sgzy_hist_da->alarm.time.ms);
	Swap2Byte(&sgzy_hist_da->alarm.time.year);
#endif   
	
	fread(&sgzy_hist_da->zyhd.zynum,sizeof(short),1,fp);
	printf("zynum=%d\n",sgzy_hist_da->zyhd.zynum);
#ifdef  SUN_OS
	Swap2Byte(&sgzy_hist_da->zyhd.zynum);
#endif        
	if( sgzy_hist_da->zyhd.zynum > MAX_SGZY_COUNT )
	{
		printf( "in  ReadSgzyFile : error : num=%d > MAX_SGZY_COUNT\n", sgzy_hist_da->zyhd.zynum );
		sgzy_hist_da->zyhd.zynum= 0;
		return;
	}	
	fread(sgzy_hist_da->zyhd.sgname,sizeof(sgzy_hist_da->zyhd.sgname),1,fp);    
	
	fread(sgzy_hist_da->zyhd.longname,sizeof(sgzy_hist_da->zyhd.longname[0]),sgzy_hist_da->zyhd.zynum,fp);
	//  printf("sizeoflongname=%d,POINT_LONGNAME_SIZE=%d\n",sizeof(sgzy_hist_da->zyhd.longname[0]),POINT_LONGNAME_SIZE);
	
	fread(sgzy_hist_da->zyhd.data_type,sizeof(sgzy_hist_da->zyhd.data_type[0]),sgzy_hist_da->zyhd.zynum,fp);    
	
	fread(sgzy_hist_da->zyhd.hi_alm,sizeof(sgzy_hist_da->zyhd.hi_alm[0]),sgzy_hist_da->zyhd.zynum,fp);        
	fread(sgzy_hist_da->zyhd.lo_alm,sizeof(sgzy_hist_da->zyhd.lo_alm[0]),sgzy_hist_da->zyhd.zynum,fp);
	fread(sgzy_hist_da->zyhd.pointer,sizeof(POINTER),sgzy_hist_da->zyhd.zynum,fp);	//zlb 2011-9-8
	for( j=0; j< sgzy_hist_da->zyhd.zynum; j++)	
		printf("hi_alm[%d]=%.1f,longname[%d]=%s\n",j,sgzy_hist_da->zyhd.hi_alm[j],j,sgzy_hist_da->zyhd.longname);
#ifdef SUN_OS
	for( j=0; j< sgzy_hist_da->zyhd.zynum; j++)
	{
		printf("hi_alm[%d]=%.1f\n",sgzy_hist_da->zyhd.hi_alm[j]);
		Swap4Byte(&sgzy_hist_da->zyhd.hi_alm[j]);
		Swap4Byte(&sgzy_hist_da->zyhd.lo_alm[j]);
		Swap2Byte(&sgzy_hist_da->zyhd.pointer[j].pt_id);	//zlb 2011-9-8
	}
#endif
	fread(&sgzy_hist_da->dateTime,sizeof(DATE_TIME),1,fp);
	Swap2Byte(&sgzy_hist_da->dateTime.ms);
	Swap2Byte(&sgzy_hist_da->dateTime.year);
	
	printf("data_type=%d\n",sgzy_hist_da->alarm.ptr.data_type);
	if(sgzy_hist_da->alarm.ptr.data_type==ANA_TYPE)
	{
		if((int)fread(sgzy_hist_da->sg_dat.fvalue,sizeof(sgzy_hist_da->sg_dat.fvalue),sgzy_hist_da->zyhd.zynum*SGZY_NUM,fp)!=sgzy_hist_da->zyhd.zynum*SGZY_NUM)
		{
			printf ("OIX: ReadSgzyFile readerror %s\n",filename);
			return;
		}
	}
	else
	{
		if((int)fread(sgzy_hist_da->sg_dat.status,sizeof(float/*sgzy_hist_da->sg_dat.status*/),sgzy_hist_da->zyhd.zynum*SGZY_NUM,fp)!=sgzy_hist_da->zyhd.zynum*SGZY_NUM)	
		{
			printf ("OIX: ReadSgzyFile readerror %s\n",filename);
			return;
		}		
    }
	//	printf("###SGZY_NUM=%d\n",SGZY_NUM);
	/*	if(fread(sgzy_hist_da->sg_dat.status,sizeof(int),sgzy_hist_da->zyhd.zynum*SGZY_NUM,fp)!=sgzy_hist_da->zyhd.zynum*SGZY_NUM)	
	{
	printf ("OIX: ReadSgzyFile readerror %s\n",filename);
	return;
    }
    else
	printf("read Sgzy file sucess:zynum=%d\n",sgzy_hist_da->zyhd.zynum);*/
#ifdef SUN_OS
	for( j=0; j< sgzy_hist_da->zyhd.zynum; j++)
		for( i=0; i< SGZY_NUM; i++ )
		{
			Swap4Byte(&sgzy_hist_da->sg_dat.status[j][i]);			
		}
#endif	
		
        fclose (fp);
        strcpy ( sgzy_hist_file_name, filename);

		nCurPageNum=0;

		updataSgzyToolBar();

		sgzyHisRedraw(crt_n);

//		sgzy_page_cnt	= 0;
//		SgzyHistRedraw ( wgts[crt_n][WgtN_sgzy_hist_area] );/* added by zyp on 2006.5*/
//		gdk_window_clear_area_e(wgts[crt_n][WgtN_sgzy_hist_area]->window,0,0,(int)(1280*xscale[crt_n]), (int)(1000*yscale[crt_n]));
}

void updataSgzyToolBar()
{
	if (strcmp( sgzy_hist_file_name ,"") == 0 || strcmp(sgzy_hist_da->zyhd.sgname, "")== 0)
	{
		nCurPageNum=0;
		gtk_widget_set_sensitive(btnSgzyPageDn, FALSE);
		gtk_widget_set_sensitive(btnSgzyPageUp, FALSE);
	}
	else
	{
		if (nCurPageNum == 0)
		{
			gtk_widget_set_sensitive(btnSgzyPageUp, FALSE);
		}
		else
		{
			gtk_widget_set_sensitive(btnSgzyPageUp, TRUE);
		}
		
		if ((nCurPageNum+1)*MAX_SGZY_HIS_CURVE_WIDGET < sgzy_hist_da->zyhd.zynum)
		{
			gtk_widget_set_sensitive(btnSgzyPageDn, TRUE);
		}
		else
		{
			gtk_widget_set_sensitive(btnSgzyPageDn, FALSE);
		}
	}
}

void sgzyHisRedraw(gint crt_n)
{
	gchar longname[POINT_LONGNAME_SIZE];
	gchar str[128];
	gint nIndex, i, nTemp, j;
	gdouble Y_end,Y_start;
	gdouble dXValue;
	struct tm tmTime;
	DMS_COMMON pdt;
	
	GCurve* pCurve;
	gchar* pConvert=0;
	
	if (crt_n<0 || crt_n>=MAX_CRT_NUM)
	{
		return;
	}
	
	if( strcmp( sgzy_hist_file_name ,"") == 0 || strcmp(sgzy_hist_da->zyhd.sgname, "")== 0)
	{
		for (i=0;i<MAX_SGZY_HIS_CURVE_WIDGET; i++)
		{
			gtk_curve_widget_clear_all_curve(&sgzyHisCurveWidget[i]);
			
			gtk_curve_widget_set_text(&sgzyHisCurveWidget[i], "Unused", eTextTitle);
			gtk_curve_widget_set_text(&sgzyHisCurveWidget[i], "", eTextSubtitle);
			gtk_curve_widget_frame_redraw(&sgzyHisCurveWidget[i]);
		}
		return;
	}
// 	BlankFillCopy (longname, sgzy_hist_da->zyhd.sgname, POINT_LONGNAME_SIZE);
	
	sprintf(str,"%4d-%02d-%02d%  02d:%02d:%02d.%04d%40s",
		sgzy_hist_da->alarm.time.year,
		sgzy_hist_da->alarm.time.month,
		sgzy_hist_da->alarm.time.day,
		sgzy_hist_da->alarm.time.hour,
		sgzy_hist_da->alarm.time.minute,
		sgzy_hist_da->alarm.time.second,
		sgzy_hist_da->alarm.time.ms,
		sgzy_hist_da->zyhd.sgname);
	
	nIndex=nCurPageNum*MAX_SGZY_HIS_CURVE_WIDGET;
	
	tmTime.tm_year=sgzy_hist_da->dateTime.year-1900;
	tmTime.tm_mon=sgzy_hist_da->dateTime.month-1;
	tmTime.tm_mday=sgzy_hist_da->dateTime.day;
	tmTime.tm_hour=sgzy_hist_da->dateTime.hour;
	tmTime.tm_min=sgzy_hist_da->dateTime.minute;
	tmTime.tm_sec=sgzy_hist_da->dateTime.second;
	tmTime.tm_isdst=-1;
	
	dXValue=mktime(&tmTime);
	dXValue-=SGZY_NUM;

	for (i=0;i<MAX_SGZY_HIS_CURVE_WIDGET; i++)
	{
		gtk_curve_widget_clear_all_curve(&sgzyHisCurveWidget[i]);
		
		nTemp=nIndex+i;
		if (nTemp < sgzy_hist_da->zyhd.zynum)
		{
			if ((strcmp((pConvert=_toUtf8EX("OIX_SGZY_HIS_LIMIT")), "no")==0)
				|| (sgzy_hist_da->zyhd.data_type[nTemp]==SOE_TYPE)
				|| (sgzy_hist_da->zyhd.data_type[nTemp]==POL_TYPE)
				|| (sgzy_hist_da->zyhd.data_type[nTemp]==OUTS_TYPE)
				|| (sgzy_hist_da->zyhd.data_type[nTemp]==SYSMSG_TYPE)
				|| (ReadEntryById(&sgzy_hist_da->zyhd.pointer[nTemp], &pdt ) == -1))
			{
				Y_end	=	sgzy_hist_da->zyhd.hi_alm[nTemp];		
				Y_start =	sgzy_hist_da->zyhd.lo_alm[nTemp];
			}
			else
			{
				Y_end=pdt.var.anlg.eng_para.HI_RANGE;
				Y_start=pdt.var.anlg.eng_para.LO_RANGE;
			}

			G_SAFE_FREE(pConvert);
			 	
// 			if ( Y_start > 0 )
// 				Y_start = 0.0;
			
			strcpy(longname, sgzy_hist_da->zyhd.longname[nTemp]);
			
			gtk_curve_widget_set_text(&sgzyHisCurveWidget[i], (pConvert=_toUtf8(longname)), eTextTitle);
			G_SAFE_FREE(pConvert);
			gtk_curve_widget_set_text(&sgzyHisCurveWidget[i], (pConvert=_toUtf8(str)), eTextSubtitle);
			G_SAFE_FREE(pConvert);
			gtk_curve_widget_set_data_scale(&sgzyHisCurveWidget[i], eCalcAuto, 0, 0);
			
			pCurve=gtk_curve_create(SGZY_NUM, eModeSec);
			gtk_curve_set_rgb_color(pCurve, 0, 255, 0);
			gtk_curve_set_text(pCurve, (pConvert=_toUtf8(longname)));
			G_SAFE_FREE(pConvert);
			
			if(sgzy_hist_da->zyhd.data_type[nTemp] == ANA_TYPE)
			{
				for (j=0; j<SGZY_NUM; j++)
				{
					dYValue[j]=sgzy_hist_da->sg_dat.fvalue[nTemp][j];
				}
			}
			else
			{
				for (j=0; j<SGZY_NUM; j++)
				{
					dYValue[j]=sgzy_hist_da->sg_dat.status[nTemp][j];
				}
			}
			
			gtk_curve_set_data(pCurve, &dXValue , dYValue, SGZY_NUM, 0, SGZY_NUM, 1.0);
			gtk_curve_set_boundary(pCurve, TRUE, Y_start, Y_end);
			
			gtk_curve_widget_add_curve(&sgzyHisCurveWidget[i], pCurve);
		}
		else
		{
			gtk_curve_widget_set_text(&sgzyHisCurveWidget[i], "Unused", eTextTitle);
			gtk_curve_widget_set_text(&sgzyHisCurveWidget[i], "", eTextSubtitle);
		}
		
		gtk_curve_widget_set_time_scale(&sgzyHisCurveWidget[i], dXValue, dXValue+SGZY_NUM-1);
		gtk_curve_widget_frame_redraw(&sgzyHisCurveWidget[i]);
	}
}

void	SgzyHistPageControl (int  direction, gpointer usr_data)
{
	int  crt_n=GPOINTER_TO_INT(usr_data);
	if (crt_n<0 || crt_n>=MAX_CRT_NUM)
	{
		return;
	}

	switch  ( direction )
	{
	case    WgtN_sgzy_hist_page_up:
		if (nCurPageNum > 0)
		{
			nCurPageNum--;
		}
		else
		{
			gdk_beep();
		}
		break;
	case    WgtN_sgzy_hist_page_down:
		if ((nCurPageNum+1)*MAX_SGZY_HIS_CURVE_WIDGET < sgzy_hist_da->zyhd.zynum)
		{
			nCurPageNum++;
		}
		else
		{
			gdk_beep();
		}
		break;
	default:
		break;
	}

	updataSgzyToolBar();
	
	sgzyHisRedraw(crt_n);
}

void SgzyHistPageUpControl(GtkButton* w, gpointer user_data)
{
	SgzyHistPageControl(WgtN_sgzy_hist_page_up,user_data);
}

void SgzyHistPageDnControl(GtkButton* w, gpointer user_data)
{
	SgzyHistPageControl(WgtN_sgzy_hist_page_down, user_data);
}

void SgzyFileButton(GtkWidget * widget, gpointer usr_data)
{
	GtkWidget *pDlg;
	GtkFileFilter	*pFilter;
	gchar* filename;
	gchar* pConvert=0;
	
	gchar strFilePath[256];
	
	int crt_n=GPOINTER_TO_INT(usr_data);    
	if( crt_n < 0 || crt_n >= MAX_CRT_NUMBER )
		return  ;
    
	strcpy(strFilePath,SGZY_FILE_PATH);
	pDlg = gtk_file_chooser_dialog_new ((pConvert=_toUtf8EX(OIX_STR_SGZY_FILE_SELECT_BOX)),
		GTK_WINDOW(main_w[crt_n]),
		GTK_FILE_CHOOSER_ACTION_OPEN,
		GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
		GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
		NULL);
	G_SAFE_FREE(pConvert);

	gtk_dialog_set_alternative_button_order (GTK_DIALOG (pDlg), GTK_RESPONSE_ACCEPT, GTK_RESPONSE_CANCEL, -1);
	
	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(pDlg), strFilePath);
	
	pFilter=gtk_file_filter_new();
	gtk_file_filter_set_name(pFilter, (pConvert=_toUtf8EX("OIX_CSTR_SGZY_HIS_FILE")));
	G_SAFE_FREE(pConvert);
	gtk_file_filter_add_pattern(pFilter, "*.SGZY");
	gtk_file_filter_add_pattern(pFilter, "*.sgzy");
	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(pDlg), pFilter);
	
	if (gtk_dialog_run (GTK_DIALOG (pDlg)) == GTK_RESPONSE_ACCEPT)
	{
		filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (pDlg));
#ifdef SUN_OS
		sprintf(strFilePath, "%s", filename);
#else
		sprintf(strFilePath, "%s", (pConvert=_fromUtf8(filename)));
		G_SAFE_FREE(pConvert);
#endif

		ReadSgzyFile(crt_n, strFilePath);
		
		g_free (filename);
	}
	
//	g_object_unref(pFilter);
	gtk_widget_destroy (pDlg);

	sgzyHisRedraw(crt_n);
}

void SgzyHisDlgExitProc(GtkWidget *w, gpointer data)
{
	gint crt_n=GPOINTER_TO_INT(data);
	if (crt_n<0 || crt_n>=MAX_CRT_NUM)
	{
		return;
	}

	gtk_widget_destroy(sgzy_history_main_w[crt_n]);
}

GtkWidget* CreateSgzyHistToolbar (GtkWidget * parent, gint crt_n)
{
	GtkWidget *box;
	GtkWidget *toolbar;    	
	GtkWidget	*button_sgzy_file, *button_exit, *button_print;

	gchar *pConvert=0;

	if( crt_n < 0 || crt_n>=MAX_CRT_NUM)
		return NULL;
	
	toolbar = gtk_handle_box_new ();
	
	box = gtk_hbox_new (FALSE, 4);
	gtk_container_add (GTK_CONTAINER (toolbar), box);
	gtk_container_border_width (GTK_CONTAINER (box), 0);
	gtk_widget_show (box);
	
	/******sgfile button   add   del   *********/ 	
	   
	button_sgzy_file = gtk_button_new_with_label((pConvert=_toUtf8EX(OIX_CSTR_SGZY_DATA_FILE)));
	G_SAFE_FREE(pConvert);

	gtk_box_pack_start (GTK_BOX (box), button_sgzy_file, FALSE, FALSE, 0);   
	
	g_signal_connect(G_OBJECT(button_sgzy_file), "clicked", G_CALLBACK(SgzyFileButton), GINT_TO_POINTER(crt_n));
	
   	gtk_widget_show(button_sgzy_file);
    
	btnSgzyPageUp = gtk_button_new_with_label((pConvert=_toUtf8EX(OIX_CSTR_PAGE_UP)));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start (GTK_BOX (box), btnSgzyPageUp, FALSE, FALSE, 0);
	btnSgzyPageDn = gtk_button_new_with_label((pConvert=_toUtf8EX(OIX_CSTR_PAGE_DOWN)));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start (GTK_BOX (box), btnSgzyPageDn, FALSE, FALSE, 0);

	g_signal_connect(G_OBJECT(btnSgzyPageUp), "clicked", G_CALLBACK(SgzyHistPageUpControl), GINT_TO_POINTER(crt_n));
   	g_signal_connect(GTK_OBJECT(btnSgzyPageDn), "clicked", G_CALLBACK(SgzyHistPageDnControl), GINT_TO_POINTER(crt_n));
	
	gtk_widget_show (btnSgzyPageUp);
	gtk_widget_show (btnSgzyPageDn);

	button_print = gtk_button_new_with_label((pConvert=_toUtf8EX(OIX_CSTR_PRINT)));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start (GTK_BOX (box), button_print, FALSE, FALSE, 0);     	     

	g_signal_connect(G_OBJECT(button_print), "clicked", G_CALLBACK(PrintSgzyHist), GINT_TO_POINTER(crt_n));
	
	gtk_widget_show (button_print);	
	
	button_exit = gtk_button_new_with_label((pConvert=_toUtf8EX(OIX_CSTR_EXIT)));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_end(GTK_BOX (box), button_exit, FALSE, FALSE, 5);     	     
	g_signal_connect(GTK_OBJECT(button_exit), "clicked", G_CALLBACK(SgzyHisDlgExitProc), GINT_TO_POINTER(crt_n));
	
	gtk_widget_show (button_exit);
	
	return (toolbar);
}

void DestroySgzyHisDlg(GtkWidget *w, gpointer data)
{
	gint crt_n=GPOINTER_TO_INT(data);
	if (crt_n<0 || crt_n>=MAX_CRT_NUM)
	{
		return;
	}

	g_free(sgzy_hist_da);

	if (printSettings != NULL)
	{
		g_object_unref (printSettings);
		printSettings=NULL;
	}

	sgzy_history_main_w[crt_n]=NULL;

	nSgzyHistoryRef--;
}

GtkWidget* CreateProgSgzyHistoryShell (GtkWidget *parent , int crt_n)
{
	GtkWidget* windows, *pFrame;

	GtkWidget *vbox;
	GtkWidget* table;
	GtkWidget  *toolbar;

	gint nIndex;
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
	
    gtk_window_set_title(GTK_WINDOW(windows), (pConvert=_toUtf8EX(OIX_CSTR_SGZY_HIST_DATA_TITLE)));
	G_SAFE_FREE(pConvert);
    g_signal_connect(G_OBJECT(windows), "destroy", G_CALLBACK(DestroySgzyHisDlg), GINT_TO_POINTER(crt_n));
    
    vbox=gtk_vbox_new(FALSE, 5);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 0);
    gtk_container_add(GTK_CONTAINER(windows), vbox);
    gtk_widget_show(vbox);
    
    toolbar =  CreateSgzyHistToolbar(windows, crt_n);
    gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, TRUE, 0);
    gtk_widget_show(toolbar);    
    
	table=gtk_table_new((MAX_SGZY_HIS_CURVE_WIDGET+1)/2, 2, TRUE);
    gtk_container_add(GTK_CONTAINER(vbox), table);
    gtk_widget_show(table);
    
	for (nIndex=0; nIndex<MAX_SGZY_HIS_CURVE_WIDGET; nIndex++)
	{
		pFrame=gtk_curve_widget_create(windows, &sgzyHisCurveWidget[nIndex]);
// 		gtk_curve_widget_set_font_desc(&sgzyHisCurveWidget[nIndex], pango_font_description_from_string("sans 10"),
// 			eFontDescTitle);
// 		gtk_curve_widget_set_font_desc(&sgzyHisCurveWidget[nIndex], pango_font_description_from_string("sans 8"),
// 			eFontDescSubTitle);
// 		gtk_curve_widget_set_font_desc(&sgzyHisCurveWidget[nIndex], pango_font_description_from_string("sans 8"),
// 			eFontDescAxis);
// 		gtk_curve_widget_set_font_desc(&sgzyHisCurveWidget[nIndex], pango_font_description_from_string("sans 8"),
// 		eFontDescTrack);
		gtk_table_attach_defaults(GTK_TABLE(table), pFrame, nIndex%2, nIndex%2+1, nIndex/2, nIndex/2+1);
		gtk_curve_widget_set_data_scale(&sgzyHisCurveWidget[nIndex], eCalcAuto, 0, 0);
		gtk_curve_widget_enable(&sgzyHisCurveWidget[nIndex], FALSE, eEnableCurvesMark);
	}

	return windows;
}

void	GetSgzyHistory (int crt_n )
{
	gint nIndex;

	if (crt_n<0 || crt_n>=MAX_CRT_NUM)
	{
		gdk_beep(); 
		return;
	}

	if (sgzy_history_main_w[crt_n] == NULL)
	{
		if (nSgzyHistoryRef >= 1)
		{
			gdk_beep(); 
			PopupMessageBox(main_w[crt_n] , "OIX: SGZY History has opened !!");
			return;
		}
		nSgzyHistoryRef++;

		sgzy_hist_da	= (SGZY_DATA *)g_new(SGZY_DATA, 1);// malloc(sizeof(SGZY_DATA));
		memset(sgzy_hist_da->zyhd.sgname, 0, POINT_LONGNAME_SIZE);

		sgzy_history_main_w[crt_n]=CreateProgSgzyHistoryShell(main_w[crt_n], crt_n);

		sgzyHisRedraw(crt_n);

		gtk_widget_show_all(sgzy_history_main_w[crt_n]);
		
		for (nIndex=0; nIndex<MAX_SGZY_HIS_CURVE_WIDGET; nIndex++)
		{
			gtk_curve_widget_set_current_operation(&sgzyHisCurveWidget[nIndex], eCurveOpPointer);
		}
		
		updataSgzyToolBar();
	}
	else
	{
		gtk_window_present(GTK_WINDOW(sgzy_history_main_w[crt_n]));
	}
}


