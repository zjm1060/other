/***************************************
updated by zyp on 2005.11 by gtk
updated by zyp on 2006.3 for v4.0db
HAVE  SYNC   2008.4.10    BY CHI.HAILONG 
*****************************************/

#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include <stdio.h>

#include <sys/types.h>
#include        "../inc/oix.h"
#include "../inc/gtk_curve_widget.h"

char	trend_hist_file_name[MAX_TREND_FILE_NAME_LEN];

extern void	PopupMessageBox (GtkWidget *parent,char msg[] );
extern void	ProgHardCopy(int crt_n ,int print_mode);
extern char *_toUtf8EX(char *c);
extern char *_toUtf8(char *c);
extern char *_fromUtf8(char *c);

gint nCurTrendPageNum=0;
GtkWidget *btnTrendPageUp, *btnTrendPageDn;

#define MAX_TREND_HIS_CURVE_WIDGET 4
GtkCurveWidget trendHisCurveWidget[MAX_TREND_HIS_CURVE_WIDGET];

gint trend_his_curve_timer_id = 0;

gdouble dTrendValue[WDQS_NUM];

int nTrendHistoryRef=0;

gboolean TrendHistRedraw (gint crt_n);
void TrendHistPageControl (int  direction, gpointer usr_data);
void updataTrendToolBar();

extern FILE	*OpenFileX();

#if (GTK_CHECK_VERSION(2, 10, 0))
#define PrintTrendHis 1
extern void onPrint(GtkWidget *parent, gint nContents);
#endif

void    PrintTrendHist (GtkButton* w, gpointer user_data)
{
	int	crt_n=GPOINTER_TO_INT(user_data);
	if( crt_n < 0 || crt_n > MAX_CRT_NUM)
		return;
	
/*#ifdef SUN_OS
	char exec_print[140] = {""};
	
	strcpy (exec_print, "/PASSRC/wddpr.dxe ");
	strcat (exec_print, trend_hist_file_name);
	system (exec_print); 
#endif
	
#ifdef WINDOWS_OS	*/
	if(!CheckUserPrivilege(cur_usr_name, PRIV_PRINT)) //判断打印权限
	{
		PopupMessageBox ( main_w[crt_n], MSG_RPT_PRINTING  ); 
		return  ;
	}
#if !(GTK_CHECK_VERSION(2, 10, 0)) || !defined(WINDOWS_OS)
	ProgHardCopy( crt_n,SGZY_MODE );
#else
	onPrint(trend_history_main_w[crt_n], PrintTrendHis);
#endif
/*#endif*/
	
} 

void ReadTrendFile (int crt_n, char	*filename)
{
	FILE      *fp;
	/*#ifdef  SUN_OS*/
	int      i,j;
	/*#endif   */
	printf("in ReadTrendFile,filename=%s\n",filename);
	fp	= OpenFileX( filename );
	if ( fp == NULL)
	{
		gdk_beep();
		return;
	}
	fread(&trend_hist_da->wdhd.time,sizeof(DATE_TIME),1,fp);
#ifdef  SUN_OS
	Swap2Byte(&trend_hist_da->wdhd.time.ms);
	Swap2Byte(&trend_hist_da->wdhd.time.year);
#endif   
	fread(&trend_hist_da->wdhd.zynum,sizeof(short),1,fp);
	
#ifdef  SUN_OS
	Swap2Byte(&trend_hist_da->wdhd.zynum);
#endif        
	if( trend_hist_da->wdhd.zynum > MAX_WDQS_COUNT )
	{
		printf( "in  ReadTrendFile : error : num=%d > MAX_WDQS_COUNT\n", trend_hist_da->wdhd.zynum );
		trend_hist_da->wdhd.zynum= 0;
		return;
	}	
	fread(trend_hist_da->wdhd.longname,POINT_LONGNAME_SIZE,trend_hist_da->wdhd.zynum,fp);
	   fread(trend_hist_da->wdhd.data_type,sizeof(CHAR),trend_hist_da->wdhd.zynum,fp);
	   fread(trend_hist_da->wdhd.hi_alm,sizeof(float),trend_hist_da->wdhd.zynum,fp);
	   fread(trend_hist_da->wdhd.lo_alm,sizeof(float),trend_hist_da->wdhd.zynum,fp);
	   fread(trend_hist_da->wdhd.pointer,sizeof(POINTER),trend_hist_da->wdhd.zynum,fp);
	   
	   for(j=0;j<trend_hist_da->wdhd.zynum; j++)
		   printf("zynum=%d,longname=%s,data_type=%d\n",trend_hist_da->wdhd.zynum,trend_hist_da->wdhd.longname[j],
		   trend_hist_da->wdhd.data_type[j]);
	   
#ifdef SUN_OS
	   for( j=0; j< trend_hist_da->wdhd.zynum; j++)
	   {	
		   Swap4Byte(&trend_hist_da->wdhd.hi_alm[j]);
		   
		   Swap4Byte(&trend_hist_da->wdhd.lo_alm[j]);

#ifndef _AIX
		   Swap2Byte(&trend_hist_da->wdhd.pointer[j].pt_id);
#endif
	   }
#endif
	   
	   for(j=0;j<trend_hist_da->wdhd.zynum;j++)
	   {		
		   for(i=0;i<WDQS_NUM;i++)
		   {
			   if(fread(&trend_hist_da->wd_dat.status[j][i],sizeof(int),1,fp)!=1 )
			   {
				   printf ("OIX: ReadTrendFile read error %s\n",filename);
				   return;
			   }
			   /*else
			   printf("fread ReadTrendFile suceed\n");*/
		   }
	   }
	   
	   
#ifdef SUN_OS
	   for( j=0; j< trend_hist_da->wdhd.zynum; j++)
	   {
		   for( i=0; i< WDQS_NUM; i++ )
		   {		
			   Swap4Byte(&trend_hist_da->wd_dat.status[j][i]);
			   
			   /*if(trend_hist_da->wdhd.data_type[j]!=ANA_TYPE)
			   printf("trend_hist_da->wd_dat[%d][%d].data=%d ",j,i,trend_hist_da->wd_dat.status[j][i]);
			   else
			   printf("trend_hist_da->wd_dat[%d][%d].fdata=%.2f ",j,i,trend_hist_da->wd_dat.fvalue[j][i]);
			   if(i%20==0)
			   printf("\n");*/
		   }		
	   }
#endif
	   
	   fclose (fp);
	   strcpy ( trend_hist_file_name, filename);

	   nCurTrendPageNum=0;

	   updataTrendToolBar();

	   TrendHistRedraw(crt_n);

// 	   sgzy_page_cnt	= 0;
// 	   TrendHistRedraw ( wgts[crt_n][WgtN_trend_hist_area] );/* added by zyp on 2006.5 */
// 	   gdk_window_clear_area_e(wgts[crt_n][WgtN_trend_hist_area]->window,0,0,(gint)(1280*xscale[crt_n]), (gint)(1000*yscale[crt_n]));
}

void updataTrendToolBar()
{
	if (strcmp(trend_hist_file_name, "") == 0)
	{
		nCurTrendPageNum=0;
		gtk_widget_set_sensitive(btnTrendPageDn, FALSE);
		gtk_widget_set_sensitive(btnTrendPageUp, FALSE);
	}
	else
	{
		if (nCurTrendPageNum == 0)
		{
			gtk_widget_set_sensitive(btnTrendPageUp, FALSE);
		}
		else
		{
			gtk_widget_set_sensitive(btnTrendPageUp, TRUE);
		}

		if ((nCurTrendPageNum+1)*MAX_TREND_HIS_CURVE_WIDGET < trend_hist_da->wdhd.zynum)
		{
			gtk_widget_set_sensitive(btnTrendPageDn, TRUE);
		}
		else
		{
			gtk_widget_set_sensitive(btnTrendPageDn, FALSE);
		}
	}
}

void TrendHistPageUpControl(GtkButton* w, gpointer user_data)
{
	TrendHistPageControl(WgtN_trend_hist_page_up,user_data);
}

void TrendHistPageDnControl(GtkButton* w, gpointer user_data)
{
	TrendHistPageControl(WgtN_trend_hist_page_down, user_data);
}

void TrendHistPageControl (int  direction, gpointer usr_data)
{
	int  crt_n=GPOINTER_TO_INT(usr_data);
	if (crt_n<0 || crt_n>=MAX_CRT_NUM)
	{
		return;
	}

	printf("in 	TrendHistPageControl: PageNum=%d,direction=%d,WgtN_trend_hist_page_up=%d,WgtN_trend_hist_page_down=%d\n",
		nCurTrendPageNum, direction,WgtN_trend_hist_page_up,WgtN_trend_hist_page_down);
	switch  (direction)
	{
	case    WgtN_trend_hist_page_up:
		if (nCurTrendPageNum > 0)
		{
			nCurTrendPageNum--;
		}
		else
		{
			gdk_beep();
		}
		break;
	case    WgtN_trend_hist_page_down:
		if ((nCurTrendPageNum+1)*MAX_TREND_HIS_CURVE_WIDGET < trend_hist_da->wdhd.zynum)
		{
			nCurTrendPageNum++;
		}
		else
		{
			gdk_beep();
		}
		break;	
	default:
		break;
	}

	updataTrendToolBar();
	
	TrendHistRedraw (crt_n);
//	gdk_window_clear_area_e(wgts[crt_n][WgtN_trend_hist_area]->window,0,0,(gint)(1280*xscale[crt_n]), (gint)(1000*yscale[crt_n]));
}



gboolean    TrendHistRedraw (gint crt_n)
{
	int		i;
	char longname[POINT_LONGNAME_SIZE];
	float Y_start, Y_end;
	char str[128];
	gint nIndex, nTemp, j;
	gdouble dXValue;
	struct tm tmTime;
	DMS_COMMON pdt;

	GCurve* pCurve;
	char *pConvert=0;
	
	if (crt_n<0 || crt_n>=MAX_CRT_NUM)
	{
		return FALSE;
	}
	
	if( strcmp( trend_hist_file_name ,"") == 0)
	{
		for (i=0;i<MAX_TREND_HIS_CURVE_WIDGET; i++)
		{
			gtk_curve_widget_clear_all_curve(&trendHisCurveWidget[i]);
			
			gtk_curve_widget_set_text(&trendHisCurveWidget[i], "Unused", eTextTitle);
			gtk_curve_widget_set_text(&trendHisCurveWidget[i], "", eTextSubtitle);
			gtk_curve_widget_frame_redraw(&trendHisCurveWidget[i]);
		}
		return TRUE;
	}
	
	sprintf(str,"%4d-%02d-%02d%  02d:%02d:%02d.%04d",
		trend_hist_da->wdhd.time.year,
		trend_hist_da->wdhd.time.month,
		trend_hist_da->wdhd.time.day,
		trend_hist_da->wdhd.time.hour,
		trend_hist_da->wdhd.time.minute,
		trend_hist_da->wdhd.time.second,
		trend_hist_da->wdhd.time.ms);
	
//	gdk_gc_set_foreground ( draw_gc, &pixels[crt_n][RED1]);	
	printf("&&& str=%s\n",str);
	/* gdk_draw_text( w->window, font_set[SGZY_FONT], draw_gc, 500,20, str,strlen(str ));*/
	
//     tempx=(int)(500  );
// 	tempy=(int)(40 );
//	oix_draw_text( w->window, font_set[SGZY_FONT], draw_gc,tempx ,tempy, str,strlen(str),crt_n); /*hcl 此处高度20可能导致标题遮挡*/
	
#ifdef	DEBUG
	printf ("OIX:in TrendHistRedraw: Curve Point Number= %d\n", trend_hist_da->wdhd.zynum);
#endif
	
// 	sgzy_page_numb	= trend_hist_da->wdhd.zynum/SGZY_HIST_PAGE_SIZE+1;
// 	
// 	/*sgzy_page_numb	= trend_hist_da->wdhd.zynum/CURVE_NUM+1;*/
// 	
// 	if ( sgzy_page_cnt > sgzy_page_numb-1 )
// 		sgzy_page_cnt	= sgzy_page_numb-1;
// 	
// 	i1	= sgzy_page_cnt * SGZY_HIST_PAGE_SIZE;
// 	i2	= MIN ((sgzy_page_cnt+1)*SGZY_HIST_PAGE_SIZE, trend_hist_da->wdhd.zynum);
// 	printf("i1=%d,i2=%d\n",i1,i2);
// 
// gdk_gc_set_foreground ( draw_gc, &pixels[crt_n][GREEN4]);	

	/*i1	= sgzy_page_cnt * CURVE_NUM;
	i2	= MIN ((sgzy_page_cnt+1)*CURVE_NUM, trend_hist_da->wdhd.zynum);*/

	nIndex=nCurTrendPageNum*MAX_TREND_HIS_CURVE_WIDGET;
	
	tmTime.tm_year=trend_hist_da->wdhd.time.year-1900;
	tmTime.tm_mon=trend_hist_da->wdhd.time.month-1;
	tmTime.tm_mday=trend_hist_da->wdhd.time.day;
	tmTime.tm_hour=trend_hist_da->wdhd.time.hour;
	tmTime.tm_min=trend_hist_da->wdhd.time.minute;
	tmTime.tm_sec=trend_hist_da->wdhd.time.second;
	tmTime.tm_isdst=-1;
	
	dXValue=mktime(&tmTime);
	dXValue-=WDQS_NUM;

	for (i=0;i<MAX_TREND_HIS_CURVE_WIDGET; i++)
	{
		gtk_curve_widget_clear_all_curve(&trendHisCurveWidget[i]);
		
		nTemp=nIndex+i;
		if (nTemp < trend_hist_da->wdhd.zynum)
		{
			if ((strcmp((pConvert=_toUtf8EX("OIX_TREND_HIS_LIMIT")), "no")==0)
				|| (trend_hist_da->wdhd.data_type[nTemp]==SOE_TYPE)
				|| (trend_hist_da->wdhd.data_type[nTemp]==POL_TYPE)
				|| (trend_hist_da->wdhd.data_type[nTemp]==OUTS_TYPE)
				|| (trend_hist_da->wdhd.data_type[nTemp]==SYSMSG_TYPE)
				|| (ReadEntryById(&trend_hist_da->wdhd.pointer[nTemp], &pdt ) == -1))
			{
				Y_end	=	trend_hist_da->wdhd.hi_alm[nTemp];		
				Y_start =	trend_hist_da->wdhd.lo_alm[nTemp];
			}
			else
			{
				Y_end=pdt.var.anlg.eng_para.HI_RANGE;
				Y_start=pdt.var.anlg.eng_para.LO_RANGE;

// 				printf("point=%d.%d.%d.%d Y_end=%.3f Y_start=%.3f HI_ALM=%.3f LO_ALM=%.3f", pdt.point.stn_id, 
// 					pdt.point.dev_id, pdt.point.data_type, pdt.point.pt_id, Y_end, Y_start, 
// 					trend_hist_da->wdhd.hi_alm[nTemp], trend_hist_da->wdhd.lo_alm[nTemp]);
			}

			G_SAFE_FREE(pConvert);
				 	
// 			if ( Y_start > 0 )
// 				Y_start = 0.0;
			
			strcpy(longname, trend_hist_da->wdhd.longname[nTemp]);

			gtk_curve_widget_set_text(&trendHisCurveWidget[i], (pConvert=_toUtf8(longname)), eTextTitle);
			G_SAFE_FREE(pConvert);
			gtk_curve_widget_set_text(&trendHisCurveWidget[i], (pConvert=_toUtf8(str)), eTextSubtitle);
			G_SAFE_FREE(pConvert);

			gtk_curve_widget_set_data_scale(&trendHisCurveWidget[i], eCalcAuto, 0, 0);
			
			pCurve=gtk_curve_create(WDQS_NUM, eModeSec);
			gtk_curve_set_rgb_color(pCurve, 0, 255, 0);
			gtk_curve_set_text(pCurve, (pConvert=_toUtf8(longname)));
			G_SAFE_FREE(pConvert);
			
			if(trend_hist_da->wdhd.data_type[nTemp] == ANA_TYPE)
			{
				for (j=0; j<WDQS_NUM; j++)
				{
					dTrendValue[j]=trend_hist_da->wd_dat.fvalue[nTemp][j];
				}
			}
			else
			{
				for (j=0; j<WDQS_NUM; j++)
				{
					dTrendValue[j]=trend_hist_da->wd_dat.status[nTemp][j];
				}
			}
			
			gtk_curve_set_data(pCurve, &dXValue , dTrendValue, WDQS_NUM, 0, WDQS_NUM, 1.0);
			gtk_curve_set_boundary(pCurve, TRUE, Y_start, Y_end);
			
			gtk_curve_widget_add_curve(&trendHisCurveWidget[i], pCurve);
		}
		else
		{
			gtk_curve_widget_set_text(&trendHisCurveWidget[i], "Unused", eTextTitle);
			gtk_curve_widget_set_text(&trendHisCurveWidget[i], "", eTextSubtitle);
		}
		
		gtk_curve_widget_set_time_scale(&trendHisCurveWidget[i], dXValue, dXValue+WDQS_NUM-1);
		gtk_curve_widget_frame_redraw(&trendHisCurveWidget[i]);
	}

	return TRUE;

// 	for ( i=i1; i< i2; i++)
// 	{
// 		Y_end   = trend_hist_da->wdhd.hi_alm[i]; 		
// 		Y_start = trend_hist_da->wdhd.lo_alm[i];                		
// 		
// 		if ( Y_start > 0 )
// 			Y_start = 0.0;
// 		
// 		sgzy_mmi_mode	= TREND_HISTORY;
// 		
// 		itmp3	= itmp1%SGZY_PAGE_SIZE;
// 		itmp2 = itmp1/SGZY_PAGE_SIZE;
// 		
// 		/*itmp3	= itmp1%CURVE_NUM;
// 		itmp2 = itmp1/CURVE_NUM;*/		
// 		
// 		BlankFillCopy (longname, trend_hist_da->wdhd.longname[i], POINT_LONGNAME_SIZE); 
// 		
//         /* gdk_draw_text( w->window, font_set[TREND_FONT], draw_gc,
// 		TREND_X_LEFT+TREND_WIDTH/2-100+(TREND_WIDTH+TREND_X_SPACE)*itmp2, 
// 		TREND_Y_TOP-10+itmp3*(TREND_HEIGHT+TREND_Y_SPACE), longname, strlen(longname));  
// 		*/
// 		tempx=(int)((TREND_X_LEFT+TREND_WIDTH/2-100 +(TREND_WIDTH+TREND_X_SPACE)*itmp2)) ;
// 		tempy=(TREND_Y_TOP-10+itmp3*(TREND_HEIGHT+TREND_Y_SPACE)) ;
// 		oix_draw_text( w->window, font_set[TREND_FONT], draw_gc,
// 			tempx, 
// 			tempy, longname,strlen(longname),crt_n); 
// 		
// 		tempx=(int)((TREND_X_LEFT+(TREND_WIDTH+TREND_X_SPACE)*itmp2) );
// 		tempy=(int)((TREND_Y_TOP+(TREND_HEIGHT+TREND_Y_SPACE)*itmp3) );
// 		tempWidth=(int)(TREND_WIDTH );
// 		tempHeight=(int)(TREND_HEIGHT );
// 		DrawSgzyCoord ( crt_n, w, draw_gc, tempx, 
// 			tempy,tempWidth, tempHeight,trend_hist_da->wdhd.data_type[i] );
// 
// 		tempx=(TREND_X_LEFT+(TREND_WIDTH+TREND_X_SPACE)*itmp2) ;
// 		tempy=(TREND_Y_TOP+(TREND_HEIGHT+TREND_Y_SPACE)*itmp3) ;
// 		tempWidth=TREND_WIDTH ;
// 		tempHeight=TREND_HEIGHT ;
//         
// 		//坐标
// 		DrawSgzyCurve ( crt_n, w, draw_gc, WDQS_NUM, (SHORT)i,
// 			tempx, tempy, 
// 			tempWidth,tempHeight, Y_start, Y_end,longname,trend_hist_da->wdhd.data_type[i]);
// 		
// 
// 		tempx=(int)((TREND_X_LEFT+(TREND_WIDTH+TREND_X_SPACE)*itmp2) );
// 		tempy=(int)((TREND_Y_TOP+(TREND_HEIGHT+TREND_Y_SPACE)*itmp3) );
// 		tempWidth=(int)(TREND_WIDTH);
// 		tempHeight=(int)(TREND_HEIGHT);
// 		
// 		//画线
// 		DrawSgzyCurveLine ( crt_n, w, draw_gc, WDQS_NUM, (SHORT)i,
// 			tempx, tempy, 
// 			tempWidth,tempHeight, Y_start, Y_end,longname,trend_hist_da->wdhd.data_type[i]);
// 
// 		itmp1++;
// 	}
}           


void TrendFileButton(GtkWidget * widget, gpointer usr_data)
{
// 	int crt_n=0;
// 	file_mode[crt_n]       = TREND_HIST_FILE; 			
// 	GetCrtNumberOfWgt (widget, &crt_n );
// 	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
// 		return  ;		
// 	PopFileSelectionDlg(main_w[crt_n],crt_n,_CS_(OIX_STR_WDQS_FILE_SELECT_BOX),SGZY_FILE_PATH);//"e:/home/ems/h9000/his/sgzy"); 

	GtkWidget *pDlg;
	GtkFileFilter	*pFilter;
	
	gchar strFilePath[256];
	gchar* filename;
	gchar *pConvert=0;
	
	int crt_n=GPOINTER_TO_INT(usr_data);    
	if( crt_n < 0 || crt_n >= MAX_CRT_NUMBER )
		return  ;
    
	strcpy(strFilePath,SGZY_FILE_PATH);
	pDlg = gtk_file_chooser_dialog_new ((pConvert=_toUtf8EX(OIX_STR_WDQS_FILE_SELECT_BOX)),
		GTK_WINDOW(main_w[crt_n]),
		GTK_FILE_CHOOSER_ACTION_OPEN,
		GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
		GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
		NULL);
	G_SAFE_FREE(pConvert);

	gtk_dialog_set_alternative_button_order (GTK_DIALOG (pDlg), GTK_RESPONSE_ACCEPT, GTK_RESPONSE_CANCEL, -1);
	
	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(pDlg), strFilePath);
	
	pFilter=gtk_file_filter_new();
	gtk_file_filter_set_name(pFilter, (pConvert=_toUtf8EX("OIX_CSTR_TREND_HIS_FILE")));
	G_SAFE_FREE(pConvert);
	gtk_file_filter_add_pattern(pFilter, "*.WDQS");
	gtk_file_filter_add_pattern(pFilter, "*.wdqs");
	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(pDlg), pFilter);
	
	if (gtk_dialog_run (GTK_DIALOG (pDlg)) == GTK_RESPONSE_ACCEPT)
	{
		filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (pDlg));

		if (filename != NULL)
		{
#ifdef SUN_OS
			sprintf(strFilePath, "%s", filename);
#else
			sprintf(strFilePath, "%s", pConvert=_fromUtf8(filename));
			G_SAFE_FREE(pConvert);
#endif
			ReadTrendFile(crt_n, strFilePath);
			
			g_free (filename);
		}
	}
	
//	g_object_unref(pFilter);
	gtk_widget_destroy (pDlg);
	
	TrendHistRedraw(crt_n);
}

void TrendHisDlgExitProc(GtkWidget *w, gpointer data)
{
	gint crt_n=GPOINTER_TO_INT(data);
	if (crt_n<0 || crt_n>=MAX_CRT_NUM)
	{
		return;
	}
	
	gtk_widget_destroy(trend_history_main_w[crt_n]);
}

GtkWidget* CreateTrendHistToolbar (GtkWidget * parent, int crt_n)
{
	GtkWidget *box;	
	GtkWidget *toolbar;    	
	GtkWidget	*button_trend_file, *button_exit, *button_print;
//	int toolbarHeight,toolbarWidth;

	gchar *pConvert=0;

	if( crt_n < 0 || crt_n>=MAX_CRT_NUM)
		return NULL;
	
	toolbar = gtk_handle_box_new ();
	
	box = gtk_hbox_new (FALSE, 4);
	gtk_container_add (GTK_CONTAINER (toolbar), box);
	gtk_container_border_width (GTK_CONTAINER (box), 5);
	gtk_widget_show (box);
	
	/******sgfile button   add   del   *********/ 	
	   
	button_trend_file = gtk_button_new_with_label((pConvert=_toUtf8EX(OIX_CSTR_TREND_FILE)));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start (GTK_BOX (box), button_trend_file, FALSE, FALSE, 0);   
	
	g_signal_connect(G_OBJECT(button_trend_file), "clicked", G_CALLBACK(TrendFileButton), GINT_TO_POINTER(crt_n));
	
   	gtk_widget_show(button_trend_file);  
	
	btnTrendPageUp = gtk_button_new_with_label((pConvert=_toUtf8EX(OIX_CSTR_PAGE_UP)));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start (GTK_BOX (box), btnTrendPageUp, FALSE, FALSE, 0);
	btnTrendPageDn = gtk_button_new_with_label((pConvert=_toUtf8EX(OIX_CSTR_PAGE_DOWN)));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start (GTK_BOX (box), btnTrendPageDn, FALSE, FALSE, 0);
	g_signal_connect(G_OBJECT(btnTrendPageUp), "clicked", G_CALLBACK(TrendHistPageUpControl), GINT_TO_POINTER(crt_n));
   	g_signal_connect(G_OBJECT(btnTrendPageDn), "clicked", G_CALLBACK(TrendHistPageDnControl), GINT_TO_POINTER(crt_n));
	
	gtk_widget_show (btnTrendPageUp);
	gtk_widget_show (btnTrendPageDn);

	button_print = gtk_button_new_with_label((pConvert=_toUtf8EX(OIX_CSTR_PRINT)));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start (GTK_BOX (box), button_print, FALSE, FALSE, 0);     	     
	g_signal_connect(G_OBJECT(button_print), "clicked", G_CALLBACK(PrintTrendHist), GINT_TO_POINTER(crt_n));
	gtk_widget_show (button_print);	 
	
	button_exit = gtk_button_new_with_label((pConvert=_toUtf8EX(OIX_CSTR_EXIT)));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_end(GTK_BOX (box), button_exit, FALSE, FALSE, 5);    	
	g_signal_connect(G_OBJECT(button_exit), "clicked", G_CALLBACK(TrendHisDlgExitProc), GINT_TO_POINTER(crt_n));        	
	gtk_widget_show (button_exit);

	return (toolbar);
}

// void 
// trend_hist_draw_area_realize(GtkWidget *widget,gpointer data)
// {
// 	int crt_n=0;
// 	
// //	GdkGC *gc;
// //	GdkColor clr;
// //	float red ,blue, green ;
// 	
// 	GetCrtNumberOfWgt ( widget, &crt_n );
// 	if( crt_n < 0 )
// 		return;
// 	wgts[crt_n][WgtN_trend_hist_area] = widget ;
//     
// 	draw_gc =  gdk_gc_new(widget->window);   
// 	
// 	gdk_window_set_background(widget->window,&pixels[crt_n][BLACK]); 
// }

// void  trend_hist_expose_events(	GtkWidget *widget,GdkEventExpose *event, gpointer data)
// {                    
// 	TrendHistRedraw ( widget );
// 	printf("in trend_hist_expose_events:: w->window=%d,draw_gc=%d\n",widget->window,draw_gc);
// }

void DestroyTrendHisDlg(GtkWidget *w, gpointer data)
{
	gint crt_n=GPOINTER_TO_INT(data);
	if (crt_n<0 || crt_n>=MAX_CRT_NUM)
	{
		return;
	}

	if (trend_his_curve_timer_id != 0)
	{
		g_source_remove(trend_his_curve_timer_id);
		trend_his_curve_timer_id  = 0 ;
	}
	
	g_free(trend_hist_da);
	
	trend_history_main_w[crt_n]=NULL;

	nTrendHistoryRef--;
}

GtkWidget* CreateProgTrendHistoryShell(GtkWidget *parent , int crt_n)
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
	
    gtk_window_set_title(GTK_WINDOW(windows), (pConvert=_toUtf8EX(OIX_CSTR_TREND_HIST_DATA_TITLE)));
	G_SAFE_FREE(pConvert);
    g_signal_connect(G_OBJECT(windows), "destroy", G_CALLBACK(DestroyTrendHisDlg), GINT_TO_POINTER(crt_n));
    
    vbox=gtk_vbox_new(FALSE, 5);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 0);
    gtk_container_add(GTK_CONTAINER(windows), vbox);
    gtk_widget_show(vbox);
    
    toolbar =  CreateTrendHistToolbar(windows, crt_n);
    gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, TRUE, 0);
    gtk_widget_show(toolbar);    
    
	table=gtk_table_new((MAX_TREND_HIS_CURVE_WIDGET+1)/2, 2, TRUE);
    gtk_container_add(GTK_CONTAINER(vbox), table);
    gtk_widget_show(table);
    
	for (nIndex=0; nIndex<MAX_TREND_HIS_CURVE_WIDGET; nIndex++)
	{
		pFrame=gtk_curve_widget_create(windows, &trendHisCurveWidget[nIndex]);
//		gtk_curve_widget_set_font_desc(&trendHisCurveWidget[nIndex], pango_font_description_from_string("sans 10"),
//			eFontDescTitle);
//		gtk_curve_widget_set_font_desc(&trendHisCurveWidget[nIndex], pango_font_description_from_string("sans 8"),
//			eFontDescSubTitle);
//		gtk_curve_widget_set_font_desc(&trendHisCurveWidget[nIndex], pango_font_description_from_string("sans 8"),
//			eFontDescAxis);
//		gtk_curve_widget_set_font_desc(&trendHisCurveWidget[nIndex], pango_font_description_from_string("sans 8"),
//			eFontDescTrack);
		gtk_table_attach_defaults(GTK_TABLE(table), pFrame, nIndex%2, nIndex%2+1, nIndex/2, nIndex/2+1);
		gtk_curve_widget_set_data_scale(&trendHisCurveWidget[nIndex], eCalcAuto, 0, 0);
		gtk_curve_widget_enable(&trendHisCurveWidget[nIndex], FALSE, eEnableCurvesMark);
	}

	return windows;
}

void GetTrendHistory (int crt_n )
{
//	GtkWidget *vbox, *hbox;
//	GtkWidget  *toolbar/*,*trend_name_list_scrollw*//*,*trend_list_scrollw*/;
	//GtkWidget /**table,*/*hrule/*, *vrule*/;
//	gchar	title[50];
	
//	sgzy_mmi_mode	= TREND_HISTORY;
	
	/*draw_gc = info[crt_n].gc;*/
// 	trend_hist_file_name[0] = 0;

	gint nIndex;

	if (crt_n<0 || crt_n>=MAX_CRT_NUM)
	{
		gdk_beep();
		return;
	}

	if (trend_history_main_w[crt_n] == NULL)
	{
		if (nTrendHistoryRef >= 1)
		{
			gdk_beep(); 
			PopupMessageBox(main_w[crt_n] , "OIX: Trend History has opened !!");
			return;
		}
		nTrendHistoryRef++;

		trend_hist_da	= (WDQS_DATA*)g_new(WDQS_DATA, 1);
		memset(trend_hist_file_name, 0, MAX_TREND_FILE_NAME_LEN);
		trend_history_main_w[crt_n]=CreateProgTrendHistoryShell(main_w[crt_n], crt_n);

		TrendHistRedraw(crt_n);

		gtk_widget_show_all(trend_history_main_w[crt_n]);
		
		for (nIndex=0; nIndex<MAX_TREND_HIS_CURVE_WIDGET; nIndex++)
		{
			gtk_curve_widget_set_current_operation(&trendHisCurveWidget[nIndex], eCurveOpPointer);
		}
		
		updataTrendToolBar();

		trend_his_curve_timer_id = g_timeout_add (1000, (GSourceFunc)TrendHistRedraw, GINT_TO_POINTER(crt_n));
	}
	else
	{
		gtk_window_present(GTK_WINDOW(trend_history_main_w[crt_n]));
	}
}