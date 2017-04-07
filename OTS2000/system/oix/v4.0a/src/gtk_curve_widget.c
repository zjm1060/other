#include "../inc/gtk_curve_widget.h"

#include <time.h>
#include <math.h>
#include <gdk/gdkkeysyms.h>

#include "../inc/oix.h"

#define DEFAULT_TITLE_FONT_DESC		"sans 14"
#define DEFAULT_SUBTILTE_FONT_DESC	"sans 10"
#define DEFAULT_AXIS_FONT_DESC		"sans 10"
#define DEFAULT_TRACK_FONT_DESC		"sans 10"
#define DEFAULT_MESSAGE_FONT_DESC	"sans 14"
#define DEFAULT_CURVES_MARK_FONT_DESC "sans 10"

#define ROW_MAJOR_MAX	16
#define ROW_MAJOR_MIN	8
#define ROW_MINOR_NUM	4
const gdouble g_dRowMajorInterval[]={5.0, 4.0, 2.0, 1.0};

#define COLUMN_MAJOR_MAX	24
#define COLUMN_MAJOR_MIN	12
#define COLUMN_MINOR_NUM	6
const gdouble g_dInterval[]={30.0, 15.0, 10.0, 5.0, 3.0, 2.0, 1.0};
const gdouble g_dHourInterval[]={12.0, 6.0, 4.0, 2.0, 1.0};
const gdouble g_dMonInterval[]={6.0, 4.0, 2.0, 1.0};
const gdouble g_dYearInterval[]={5.0, 4.0, 2.0, 1.0};

#define COLUMN_MINOR_MIN_NUM	COLUMN_MAJOR_MIN+(COLUMN_MAJOR_MAX-COLUMN_MAJOR_MIN)/3

const gdouble g_dash[]={10.0, 5.0};

extern char *_toUtf8EX(char *c);
extern GdkPixbuf* create_pixbuf (const gchar *filename);
extern GtkWidget* create_pixmap (GtkWidget *widget, const gchar *filename);
extern int GetColor(short id , GdkColor *clr );

gdouble dash_list[]={1.0, 3.0};

gint g_nCursorRef=0;
GdkPixbuf* g_pPixbufCursor[eCurveOpEnd]={0, 0, 0, 0, 0};

enum EPixbufType	{ePixbufMessage=0, ePixbufMessageClose, ePixbufEnd};
gint g_nPixbufRef=0;
GdkPixbuf* g_pPixbuf[ePixbufEnd]={NULL};

GCurve* gtk_curve_create(gint nLenBuf, gint nTimeMode)
{
	GCurve* pCurve=(GCurve*)g_new(GCurve, 1);

	memset(pCurve, 0, sizeof(GCurve));

	gtk_curve_change_buf_size(pCurve, nLenBuf, nTimeMode);

	return pCurve;
}

void gtk_curve_destroy(GCurve* pCurve)
{
	G_ASSERT_NULL(pCurve);

	G_SAFE_FREE(pCurve->pXAxis);
	G_SAFE_FREE(pCurve->pYAxis);

	g_free((gpointer)pCurve);
}

void gtk_curve_change_buf_size(GCurve* pCurve, gint nLenBuf, gint nTimeMode)
{
	gint nLen;

	G_ASSERT_NULL(pCurve);
	g_assert(nLenBuf > 0 && nTimeMode >=0 && nTimeMode < eModeEnd);

	G_SAFE_FREE(pCurve->pXAxis);
	G_SAFE_FREE(pCurve->pYAxis);

	pCurve->nTimeMode=nTimeMode;

	pCurve->pYAxis=g_new(gdouble, nLenBuf);
	
	if (nTimeMode == eModeNormal || nTimeMode == eModeNormalAndNoSample)
	{	
		nLen=nLenBuf;
	}
	else
	{
		nLen=1;
	}
	
	pCurve->pXAxis=g_new(gdouble, nLen);
}

void gtk_curve_set_text(GCurve* pCurve, gchar* strText)
{
	G_ASSERT_NULL(pCurve);
	sprintf(pCurve->strText, "%s", strText);
}

void gtk_curve_set_label_pos(GCurve* pCurve, gint pos)
{
	G_ASSERT_NULL(pCurve);
	g_assert(pos < eCurveLabelPosEnd);

	pCurve->nLabelPos=pos;
}

GdkColor gtk_curve_get_color(GCurve* pCurve)
{
	G_ASSERT_NULL(pCurve);

	return pCurve->color;
}

void gtk_curve_set_color(GCurve* pCurve, GdkColor* color)
{
	G_ASSERT_NULL(pCurve);
	if (color != NULL)
	{
		pCurve->color=*color;
	}
}

void gtk_curve_set_rgb_color(GCurve* pCurve, guint8 r, guint8 g, guint8 b)
{
	GdkColor color=rgb2gdk(r, g, b);
	
	gtk_curve_set_color(pCurve, &color);
}

void gtk_curve_set_int_color(GCurve* pCurve, int color)
{
	guint8 r, g, b;

	r=(color&0x1f)<<3;
	g=((color>>5)&0x1f)<<3; 
	b=((color>>10)&0x1f)<<3;

	gtk_curve_set_rgb_color(pCurve, r, g, b);
}

void gtk_curve_set_type(GCurve* pCurve, gint nType)
{
	G_ASSERT_NULL(pCurve);
	pCurve->nCurveType=nType;
}

void gtk_curve_set_data(GCurve* pCurve, gdouble* pXAxis, gdouble* pYAxis, gint nLenBuf, 
						gint nOffset, gint nNumPt, gdouble dStep)
{
	gint nLen;

	G_ASSERT_NULL(pCurve);

	if (nLenBuf > 0)
	{
		memcpy(pCurve->pYAxis, pYAxis, sizeof(gdouble)*nLenBuf);

		if (pCurve->nTimeMode == eModeNormal || pCurve->nTimeMode == eModeNormalAndNoSample)
		{	
			nLen=nLenBuf;
		}
		else
		{
			nLen=1;
		}
		
		memcpy(pCurve->pXAxis, pXAxis, sizeof(gdouble)*nLen);
	}
	
	pCurve->nLenBuf=nLenBuf;
	pCurve->nOffset=nOffset;
	pCurve->nNumPt=nNumPt;
	pCurve->dStep=dStep;
}

void gtk_curve_set_data_type(GCurve* pCurve, gint nDataType)
{
	G_ASSERT_NULL(pCurve);
	g_assert(nDataType < eDataTypeEnd);

	pCurve->nDataType=nDataType;
}

void gtk_curve_set_data_offset(GCurve* pCurve, gint nOffset)
{
	G_ASSERT_NULL(pCurve);
	g_assert(nOffset>0);

	pCurve->nOffset=nOffset%pCurve->nLenBuf;
}
void gtk_curve_set_data_points_num(GCurve* pCurve, gint nNumPt)
{
	G_ASSERT_NULL(pCurve);
	g_assert(nNumPt>0);

	pCurve->nNumPt=nNumPt;
}

void gtk_curve_set_boundary(GCurve* pCurve, gboolean bMenual, gdouble dMin, gdouble dMax)
{
	gint i,nIndex;

	G_ASSERT_NULL(pCurve);
	if (!bMenual)
	{
		nIndex=pCurve->nOffset%pCurve->nLenBuf;
		if (pCurve->nNumPt > 0)
		{
			dMax=dMin=pCurve->pYAxis[nIndex];

			for (i=1; i<pCurve->nNumPt; i++)
			{
				nIndex=(i+pCurve->nOffset)%pCurve->nLenBuf;
				dMin=pCurve->pYAxis[nIndex]<dMin? pCurve->pYAxis[nIndex]:dMin;
				dMax=pCurve->pYAxis[nIndex]>dMax? pCurve->pYAxis[nIndex]:dMax;
			}
		}
		else
		{
			dMin=dMax=0.0;
		}
		
		adjust_data_scale(&dMin, &dMax);
	}

	pCurve->dMin=dMin;
	pCurve->dMax=dMax;
}

gdouble gtk_curve_get_max_range(GCurve* pCurve)
{
	G_ASSERT_NULL(pCurve);

	return pCurve->dMax;
}

gdouble gtk_curve_get_min_range(GCurve* pCurve)
{
	G_ASSERT_NULL(pCurve);
	
	return pCurve->dMin;
}

void gtk_curve_set_user_data(GCurve* pCurve, gpointer usrData, gint nIndex)
{
	G_ASSERT_NULL(pCurve);
	g_assert(nIndex>=0 && nIndex<MAX_USR_DATA);

	pCurve->usrData[nIndex]=usrData;
}

gpointer gtk_curve_get_user_data(GCurve* pCurve, gint nIndex)
{
	G_ASSERT_NULL(pCurve);
	if (nIndex<0 || nIndex>=MAX_USR_DATA)
	{
		return NULL;
	}

	return pCurve->usrData[nIndex];
}

GtkWidget* gtk_curve_widget_create(GtkWidget* pTopWidget, GtkCurveWidget* pCurveWidget)
{
	time_t tNow;

	G_ASSERT_NULL(pCurveWidget);

	memset(pCurveWidget, 0, sizeof(GtkCurveWidget));

	pCurveWidget->pFrame=gtk_frame_new(NULL);

	pCurveWidget->dYAixsMin=-100.0;
	pCurveWidget->dYAxisMax=100.0;

	gtk_curve_widget_canvas_zoom_100(pCurveWidget);

	tNow=time(NULL);
	gtk_curve_widget_set_time_scale(pCurveWidget, tNow, tNow+24*3600.0);

	gtk_curve_widget_ref_pixbuf();

	gtk_curve_widget_enable(pCurveWidget, FALSE, eEnableScale);
	gtk_curve_widget_enable(pCurveWidget, FALSE, eEnableAutoSubTitle);
	gtk_curve_widget_enable(pCurveWidget, TRUE, eEnableTrack);
	gtk_curve_widget_enable(pCurveWidget, TRUE, eEnableNegativeScale);
	gtk_curve_widget_enable(pCurveWidget, TRUE, eEnableKeepZero);
	gtk_curve_widget_enable(pCurveWidget, FALSE, eEnableMessage);
	gtk_curve_widget_enable(pCurveWidget, FALSE, eEnableMessageClose);
	gtk_curve_widget_enable(pCurveWidget, FALSE, eEnableCurvesMark);
	gtk_curve_widget_enable(pCurveWidget, TRUE, eEnableAutoLabel);
	gtk_curve_widget_enable(pCurveWidget, TRUE, eEnableTimeAxis);

	pCurveWidget->nCloseBtnStatue=eCloseBtnNormal;

	gtk_curve_widget_set_rgb_color(pCurveWidget, 80, 80, 80, eColorTitle);
	gtk_curve_widget_set_rgb_color(pCurveWidget, 128, 128, 128, eColorSubTitle);
	gtk_curve_widget_set_rgb_color(pCurveWidget, 128, 128, 128, eColorScale);
	gtk_curve_widget_set_rgb_color(pCurveWidget, 0, 129, 29, eColorPositive);
	gtk_curve_widget_set_rgb_color(pCurveWidget, 255, 0, 0, eColorZero);
	gtk_curve_widget_set_rgb_color(pCurveWidget, 242, 122, 25, eColorNegative);
	gtk_curve_widget_set_rgb_color(pCurveWidget, 0, 0, 255, eColorLabel);
	gtk_curve_widget_set_rgb_color(pCurveWidget, 0, 128, 0, eColorAxisLabel);
	gtk_curve_widget_set_rgb_color(pCurveWidget, 128, 128, 128, eColorMajor);
	gtk_curve_widget_set_rgb_color(pCurveWidget, 80, 80, 80, eColorMinor);
	gtk_curve_widget_set_rgb_color(pCurveWidget, 0, 255, 0, eColorChooseRect);
	gtk_curve_widget_set_rgb_color(pCurveWidget, 249, 250, 68, eColorTrackLine);
	gtk_curve_widget_set_rgb_color(pCurveWidget, 212, 212, 212, eColorTrackTime);
	gtk_curve_widget_set_rgb_color(pCurveWidget, 58, 64, 78, eColorMessage);
	gtk_curve_widget_set_rgb_color(pCurveWidget, 212, 212, 212, eColorCurvsMark);
	gtk_curve_widget_set_scale_rate(pCurveWidget, 0.1);

	gtk_curve_widget_set_sample_pixel_interval(pCurveWidget, 1);

	pCurveWidget->bPressed=FALSE;

	pCurveWidget->pCanvas=gtk_drawing_area_new();

	gtk_container_add(GTK_CONTAINER(pCurveWidget->pFrame), pCurveWidget->pCanvas);

//	pCurveWidget->pPangoLayout=gtk_widget_create_pango_layout(pCurveWidget->pFrame, NULL);

	gtk_curve_widget_set_font_desc(pCurveWidget, pango_font_description_from_string(DEFAULT_TITLE_FONT_DESC),
		eFontDescTitle);
	gtk_curve_widget_set_font_desc(pCurveWidget, pango_font_description_from_string(DEFAULT_SUBTILTE_FONT_DESC),
		eFontDescSubTitle);
	gtk_curve_widget_set_font_desc(pCurveWidget, pango_font_description_from_string(DEFAULT_AXIS_FONT_DESC),
		eFontDescAxis);
	gtk_curve_widget_set_font_desc(pCurveWidget, pango_font_description_from_string(DEFAULT_TRACK_FONT_DESC),
		eFontDescTrack);
	gtk_curve_widget_set_font_desc(pCurveWidget, pango_font_description_from_string(DEFAULT_MESSAGE_FONT_DESC),
		eFontDescMessage);
	gtk_curve_widget_set_font_desc(pCurveWidget, pango_font_description_from_string(DEFAULT_CURVES_MARK_FONT_DESC),
		eFontDescCurvesMark);

	gtk_widget_set_events(pCurveWidget->pFrame, GDK_EXPOSURE_MASK);
	gtk_widget_set_events(pCurveWidget->pCanvas,  GDK_POINTER_MOTION_MASK |GDK_BUTTON_PRESS_MASK 
		| GDK_BUTTON_RELEASE_MASK | GDK_EXPOSURE_MASK | GDK_KEY_RELEASE_MASK);

	g_signal_connect(G_OBJECT(pCurveWidget->pFrame), "destroy", G_CALLBACK(gtk_curve_widget_destroy), 
		(gpointer)pCurveWidget);
	g_signal_connect(G_OBJECT(pCurveWidget->pFrame), "realize", G_CALLBACK(gtk_curve_widget_frame_realize), 
		(gpointer)pCurveWidget);
	g_signal_connect(G_OBJECT(pCurveWidget->pFrame), "size_allocate", G_CALLBACK(gtk_curve_widget_frame_allocate), 
		(gpointer)pCurveWidget);
	g_signal_connect(G_OBJECT(pCurveWidget->pFrame), "expose_event", G_CALLBACK(gtk_curve_widget_frame_expose_event), 
		(gpointer)pCurveWidget);
	
	g_signal_connect(G_OBJECT(pCurveWidget->pCanvas), "realize", G_CALLBACK(gtk_curve_widget_canvas_realize), 
		(gpointer)pCurveWidget);
	g_signal_connect(G_OBJECT(pCurveWidget->pCanvas),"expose_event",G_CALLBACK(gtk_curve_widget_canvas_expose_event), 
		(gpointer)pCurveWidget); 
	
	
	g_signal_connect(G_OBJECT(pCurveWidget->pCanvas), "button_press_event", 
		G_CALLBACK(gtk_curve_widget_canvas_btn_press_event), (gpointer)pCurveWidget);
	g_signal_connect(G_OBJECT(pCurveWidget->pCanvas), "button_release_event", 
		G_CALLBACK(gtk_curve_widget_canvas_btn_release_event), (gpointer)pCurveWidget);
	g_signal_connect(G_OBJECT(pCurveWidget->pCanvas),"motion_notify_event", 
		G_CALLBACK(gtk_curve_widget_canvas_motion_notify_event), (gpointer)pCurveWidget);

	g_signal_connect(G_OBJECT(pTopWidget), "key_release_event", G_CALLBACK(gtk_curve_widget_canvas_key_release_event), 
		(gpointer)pCurveWidget);
	
//	gtk_widget_realize(pCurveWidget->pCanvas);
//	gtk_widget_realize(pCurveWidget->pFrame);

// 	gtk_curve_widget_create_cursor(pCurveWidget);
// 	gtk_curve_widget_set_current_operation(pCurveWidget, eCurveOpPointer);

//	gtk_widget_show_all(pCurveWidget->pFrame);

	pCurveWidget->nNext=0;

	gtk_curve_widget_set_data_scale(pCurveWidget, eCalcScale, 0, 0);

	return pCurveWidget->pFrame;
}

void gtk_curve_widget_destroy(GtkWidget* pWidget, gpointer usr_data)
{
	gint nIndex;

	GtkCurveWidget* pCurveWidget=(GtkCurveWidget*)usr_data;
	G_ASSERT_NULL(pCurveWidget);

	gtk_curve_widget_clear_all_curve(pCurveWidget);

	g_object_unref (pCurveWidget->pPangoLayout);
	for (nIndex=0; nIndex<eFontDescEnd; nIndex++)
	{
		pango_font_description_free (pCurveWidget->pFontDesc[nIndex]);
	}

// 	gdk_gc_unref(pCurveWidget->pFrameGc);
// 	gdk_gc_unref(pCurveWidget->pCanvasGc);

	gtk_curve_widget_destroy_cursor(pCurveWidget);

	gtk_curve_widget_unref_pixbuf();
}

GtkWidget* gtk_curve_widget_get_frame(GtkCurveWidget* pCurveWidget)
{
	G_ASSERT_NULL(pCurveWidget);

	return pCurveWidget->pFrame;
}

void gtk_curve_widget_set_text(GtkCurveWidget* pCurveWidget, gchar* strText, gint nStrType)
{
	G_ASSERT_NULL(pCurveWidget);
	g_assert(nStrType < eTextEnd);

	sprintf(pCurveWidget->strText[nStrType], "%s", strText);
}

void gtk_curve_widget_frame_realize(GtkWidget* pWidget, gpointer usr_data)
{
	GtkCurveWidget* pCurveWidget=(GtkCurveWidget*)usr_data;
	G_ASSERT_NULL(pCurveWidget);

//	pCurveWidget->pFrameGc = gdk_gc_new(pWidget->window);

	pCurveWidget->pPangoLayout=gtk_widget_create_pango_layout(pCurveWidget->pFrame, NULL);
}

void gtk_curve_widget_frame_allocate(GtkWidget *pWidget, GtkAllocation *pAlloc ,gpointer usr_data)
{
	gtk_curve_widget_adjust_layout(pWidget, pAlloc, usr_data);
	
	gtk_widget_queue_draw(pWidget);
}

void gtk_curve_widget_adjust_axis(GtkCurveWidget* pCurveWidget)
{
	gint nIndex,nMajor;
	gdouble dInterval, dTemp;
	gint nExp;
	
	gdouble dMin, dMax;

	G_ASSERT_NULL(pCurveWidget);

	switch (pCurveWidget->nCalc)
	{
	case eCalcScale:
		{
			pCurveWidget->dYAixsMin=(pCurveWidget->bEnable[eEnableNegativeScale]? -100.0:0);
			pCurveWidget->dYAxisMax=100.0;
		}
		break;
	case eCalcScaleAuto:
		{
			if (pCurveWidget->nNext > 0)
			{
				dMin=pCurveWidget->pCurves[0]->dMin;
				dMax=pCurveWidget->pCurves[0]->dMax;
				
				for (nIndex=1; nIndex<pCurveWidget->nNext; nIndex++)
				{
					dMin=pCurveWidget->pCurves[nIndex]->dMin<dMin? pCurveWidget->pCurves[nIndex]->dMin:dMin;
					dMax=pCurveWidget->pCurves[nIndex]->dMax>dMax? pCurveWidget->pCurves[nIndex]->dMax:dMax;
				}
				
				adjust_data_scale(&dMin, &dMax);
				
				dMax=(fabs(dMax)>fabs(dMin)? fabs(dMax):fabs(dMin));
				dMin=(pCurveWidget->bEnable[eEnableNegativeScale]? -dMax:0.0);
				
				pCurveWidget->dYAixsMin=dMin;
				pCurveWidget->dYAxisMax=dMax;
			}
			else
			{
				pCurveWidget->dYAixsMin=(pCurveWidget->bEnable[eEnableNegativeScale]? -100.0:0);
				pCurveWidget->dYAxisMax=100.0;
			}
		}
		break;
	case eCalcScaleManual:
//		{
// 			dMin=pCurveWidget->dYAixsMin;
// 			dMax=pCurveWidget->dYAxisMax;
// 
// 			adjust_data_scale(&dMin, &dMax);
// 			
// 			dMax=(fabs(dMax)>fabs(dMin)? fabs(dMax):fabs(dMin));
// 			dMin=(pCurveWidget->bEnable[eEnableNegativeScale]? -dMax:0.0);
// 			
// 			pCurveWidget->dYAixsMin=dMin;
// 			pCurveWidget->dYAxisMax=dMax;
//		}
		break;
	case eCalcManual:
//		{
// 			dMin=pCurveWidget->dYAixsMin;
// 			dMax=pCurveWidget->dYAxisMax;
// 
// 			if (dMin > 0.0)
// 			{
// 				dMin=pCurveWidget->bEnable[eEnableKeepZero]? 0.0:dMin;
// 			}
// 
// 			adjust_data_scale(&dMin, &dMax);
// 			pCurveWidget->dYAixsMin=dMin;
// 			pCurveWidget->dYAxisMax=dMax;
//		}
		break;
	case eCalcAuto:
		{
			if (pCurveWidget->nNext > 0)
			{
				dMin=pCurveWidget->pCurves[0]->dMin;
				dMax=pCurveWidget->pCurves[0]->dMax;
				
				for (nIndex=1; nIndex<pCurveWidget->nNext; nIndex++)
				{
					dMin=pCurveWidget->pCurves[nIndex]->dMin<dMin? pCurveWidget->pCurves[nIndex]->dMin:dMin;
					dMax=pCurveWidget->pCurves[nIndex]->dMax>dMax? pCurveWidget->pCurves[nIndex]->dMax:dMax;
				}
				
				adjust_data_scale(&dMin, &dMax);
				
				if (dMin > 0.0)
				{
					dMin=pCurveWidget->bEnable[eEnableKeepZero]? 0.0:dMin;
				}
				
				pCurveWidget->dYAixsMin=dMin;
				pCurveWidget->dYAxisMax=dMax;
			}
			else
			{
				pCurveWidget->dYAixsMin=0.0;
				pCurveWidget->dYAxisMax=100.0;
			}
		}
		break;
	default:
		{
			pCurveWidget->dYAixsMin=0.0;
			pCurveWidget->dYAxisMax=100.0;
		}
		break;
	}
	
	if (pCurveWidget->nCalc < eCalcManual)
	{
		dMin=(pCurveWidget->bEnable[eEnableNegativeScale]? -100.0:0);
		dMax=100.0;
	}
	else
	{
		gtk_curve_widget_adjust_data_scale(pCurveWidget, &dMin, &dMax);
// 		dMin=pCurveWidget->dYAixsMin;
// 		dMax=pCurveWidget->dYAxisMax;
	}

	dInterval=(dMax-dMin)*(pCurveWidget->dScaleTop-pCurveWidget->dScaleBottom);
	nExp=0;
	dTemp=dInterval;
	while (dTemp >= ROW_MAJOR_MAX*g_dRowMajorInterval[0])
	{
		nExp++;
		dTemp=dInterval/pow(10.0, nExp);
	}

	for (nIndex=0; nIndex < COUNT_OF(g_dRowMajorInterval); nIndex++)
	{
		nMajor=(gint)(dInterval/(g_dRowMajorInterval[nIndex]*pow(10.0, nExp)));
		if (nMajor>=ROW_MAJOR_MIN && nMajor<ROW_MAJOR_MAX)
		{
			pCurveWidget->dYAxisInterval=g_dRowMajorInterval[nIndex]*pow(10.0, nExp);
			break;
		}
	}

	if (pCurveWidget->bEnable[eEnableTimeAxis])
	{
		if (pCurveWidget->dTimeMax-pCurveWidget->dTimeMin < COLUMN_MAJOR_MIN)
		{
			pCurveWidget->dTimeMax=pCurveWidget->dTimeMin+COLUMN_MAJOR_MIN;
		}
		
		dInterval=(pCurveWidget->dTimeMax-pCurveWidget->dTimeMin)*(pCurveWidget->dScaleRight-pCurveWidget->dScaleLeft);

		if (dInterval < COLUMN_MAJOR_MAX*g_dHourInterval[0]*3600.0)		//十二天以内
		{
			nExp=0;
			dTemp=dInterval;
			while (dTemp >= COLUMN_MAJOR_MAX*g_dInterval[0])
			{
				dTemp=dInterval/pow(60.0, ++nExp);
			}
			pCurveWidget->nTimeType=nExp;
			
			if (nExp == 2)		//间隔从1小时到12小时
			{
				for (nIndex=0; nIndex < sizeof(g_dHourInterval)/sizeof(g_dHourInterval[0]); nIndex++)
				{
					nMajor=(gint)(dInterval/(g_dHourInterval[nIndex]*pow(60.0, nExp)));
					if (nMajor>=COLUMN_MAJOR_MIN && nMajor<COLUMN_MAJOR_MAX)
					{
						pCurveWidget->nNumMajorCol=nMajor;
						pCurveWidget->dTimeInterval=g_dHourInterval[nIndex]*pow(60.0, nExp);
						break;
					}
				}
			}
			else				//间隔小于1小时
			{
				for (nIndex=0; nIndex < sizeof(g_dInterval)/sizeof(g_dInterval[0]); nIndex++)
				{
					nMajor=(gint)(dInterval/(g_dInterval[nIndex]*pow(60.0, nExp)));
					if (nMajor>=COLUMN_MAJOR_MIN && nMajor<COLUMN_MAJOR_MAX)
					{
						pCurveWidget->nNumMajorCol=nMajor;
						pCurveWidget->dTimeInterval=g_dInterval[nIndex]*pow(60.0, nExp);
						break;
					}
				}
			}
		}
		else if (dInterval < 365*24*3600.0)		//间隔小于一年
		{
			pCurveWidget->nTimeType=eTimeDay;
			
			for (nIndex=0; nIndex < sizeof(g_dInterval)/sizeof(g_dInterval[0]); nIndex++)
			{
				nMajor=(gint)(dInterval/(g_dInterval[nIndex]*24*3600));
				if (nMajor>=COLUMN_MAJOR_MIN && nMajor<COLUMN_MAJOR_MAX)
				{
					pCurveWidget->nNumMajorCol=nMajor;
					pCurveWidget->dTimeInterval=g_dInterval[nIndex];
					break;
				}
			}
		}
		else if (dInterval < COLUMN_MAJOR_MAX*365*24*3600.0/2)	//间隔小于十二年
		{
			pCurveWidget->nTimeType=eTimeMon;
			
			for (nIndex=0; nIndex < sizeof(g_dMonInterval)/sizeof(g_dMonInterval[0]); nIndex++)
			{
				nMajor=(gint)(dInterval/(g_dMonInterval[nIndex]*30*24*3600));
				if (nMajor>=COLUMN_MAJOR_MIN && nMajor<COLUMN_MAJOR_MAX)
				{
					pCurveWidget->nNumMajorCol=nMajor;
					pCurveWidget->dTimeInterval=g_dMonInterval[nIndex];
					break;
				}
			}
		}
		else		//间隔大于12年
		{
			nExp=0;
			dTemp=dInterval;
			while (dTemp >= COLUMN_MAJOR_MAX*g_dYearInterval[0]*365*24*3600)
			{
				dTemp=dInterval/pow(10.0, ++nExp);
			}
			pCurveWidget->nTimeType=eTimeYear;
			
			for (nIndex=0; nIndex < sizeof(g_dYearInterval)/sizeof(g_dYearInterval[0]); nIndex++)
			{
				nMajor=(gint)(dInterval/(g_dYearInterval[nIndex]*pow(10.0, nExp)*365*24*3600));
				if (nMajor>=COLUMN_MAJOR_MIN && nMajor<COLUMN_MAJOR_MAX)
				{
					pCurveWidget->nNumMajorCol=nMajor;
					pCurveWidget->dTimeInterval=g_dYearInterval[nIndex]*pow(10.0, nExp);
					break;
				}
			}
		}
	}
	else
	{
		dMin=pCurveWidget->dTimeMin;
		dMax=pCurveWidget->dTimeMax;

		adjust_data_scale(&dMin, &dMax);

		dInterval=(dMax-dMin)*(pCurveWidget->dScaleRight-pCurveWidget->dScaleLeft);
		nExp=0;
		dTemp=dInterval;
		while (dTemp >= COLUMN_MAJOR_MAX*g_dRowMajorInterval[0])
		{
			nExp++;
			dTemp=dInterval/pow(10.0, nExp);
		}
		
		for (nIndex=0; nIndex < COUNT_OF(g_dRowMajorInterval); nIndex++)
		{
			nMajor=(gint)(dInterval/(g_dRowMajorInterval[nIndex]*pow(10.0, nExp)));
			if (nMajor>=COLUMN_MAJOR_MIN && nMajor<COLUMN_MAJOR_MAX)
			{
				pCurveWidget->nNumMajorCol=nMajor;
				pCurveWidget->dTimeInterval=g_dRowMajorInterval[nIndex]*pow(10.0, nExp);
				break;
			}
		}
	}
}

void gtk_curve_widget_adjust_data_scale(GtkCurveWidget* curvewidget, double *min, double *max)
{
	double dMin, dMax;

	G_ASSERT_NULL(curvewidget);

	switch (curvewidget->nCalc)
	{
	case eCalcScaleManual:
		{
			dMin=curvewidget->dYAixsMin;
			dMax=curvewidget->dYAxisMax;
			
			adjust_data_scale(&dMin, &dMax);
			
			dMax=(fabs(dMax)>fabs(dMin)? fabs(dMax):fabs(dMin));
			dMin=(curvewidget->bEnable[eEnableNegativeScale]? -dMax:0.0);
		}
		break;
	case eCalcManual:
		{
			dMin=curvewidget->dYAixsMin;
			dMax=curvewidget->dYAxisMax;
			
			if (dMin > 0.0)
			{
				dMin=curvewidget->bEnable[eEnableKeepZero]? 0.0:dMin;
			}
			
//			adjust_data_scale(&dMin, &dMax);
		}
		break;
	default:
		{
			dMax=curvewidget->dYAxisMax;
			dMin=curvewidget->dYAixsMin;
		}
		break;
	}

	if (max != NULL)
	{
		*max=dMax;
	}
	
	if (min != NULL)
	{
		*min=dMin;
	}
}

void gtk_curve_widget_adjust_layout(GtkWidget *pWidget, GtkAllocation *pAlloc, gpointer usr_data)
{
	gint nWidth=0,nHeight=0,nStrWidth=0,nStrHeight=0,nTempWidth,nTempHeight;

	time_t tTime;
	struct tm tmTime;

	gdouble dTemp, dMin, dMax;

	gdouble dXFactor, dYFactor;
	
	PangoLayout* pPangoLayout;
	PangoFontDescription* pDescTitle;
	PangoFontDescription* pDescSubTitle;
	PangoFontDescription* pDescAxis;

	gchar strAxis[32];
	gchar *pConvert=0;
	GtkCurveWidget* pCurveWidget=(GtkCurveWidget*)usr_data;
	G_ASSERT_NULL(pCurveWidget);

	gtk_curve_widget_adjust_axis(pCurveWidget);

	pPangoLayout=pCurveWidget->pPangoLayout;
	pDescTitle=pCurveWidget->pFontDesc[eFontDescTitle];
	pDescSubTitle=pCurveWidget->pFontDesc[eFontDescSubTitle];
	pDescAxis=pCurveWidget->pFontDesc[eFontDescAxis];

	if (strlen(pCurveWidget->strText[eTextTitle]) != 0)
	{
		pango_layout_set_font_description (pPangoLayout, pDescTitle);
		pango_layout_set_text(pPangoLayout, pCurveWidget->strText[eTextTitle], -1);
		pango_layout_get_pixel_size(pPangoLayout, &nStrWidth, &nStrHeight);
		nHeight+=nStrHeight;
	}

	if (pCurveWidget->bEnable[eEnableTimeAxis] && pCurveWidget->bEnable[eEnableAutoSubTitle])
	{
		dTemp=pCurveWidget->dTimeMin+(pCurveWidget->dTimeMax-pCurveWidget->dTimeMin)*pCurveWidget->dScaleLeft;
		tTime=(time_t)dTemp;
		tmTime=*localtime(&tTime);
		sprintf(pCurveWidget->strText[eTextSubtitle],"%s:%04d-%02d-%02d %02d:%02d:%02d",
			(pConvert=_toUtf8EX("OIX_CSTR_DISPLAY_START_TIME")),tmTime.tm_year+1900, tmTime.tm_mon+1, tmTime.tm_mday, 
			tmTime.tm_hour, tmTime.tm_min, tmTime.tm_sec);
	G_SAFE_FREE(pConvert);//dzy1017
	}

	if (strlen(pCurveWidget->strText[eTextSubtitle]) != 0)
	{
		pango_layout_set_font_description (pPangoLayout, pDescSubTitle);
		pango_layout_set_text(pPangoLayout, pCurveWidget->strText[eTextSubtitle], -1);
		pango_layout_get_pixel_size(pPangoLayout, &nStrWidth, &nStrHeight);
		nHeight+=nStrHeight;
	}

	pango_layout_set_font_description (pPangoLayout, pDescAxis);

	if (pCurveWidget->bEnable[eEnableAutoLabel])
	{
		if (pCurveWidget->nCalc < eCalcManual)
		{
			gtk_curve_widget_set_text(pCurveWidget, (pConvert=_toUtf8EX("OIX_CSTR_SGZY_PERCENTAGE")), eTextLabel);
			G_SAFE_FREE(pConvert);//dzy1017
		}
		else
		{
			gtk_curve_widget_set_text(pCurveWidget, "", eTextLabel);
		}
	}

	if (strlen(pCurveWidget->strText[eTextLabel]) != 0)
	{
		pango_layout_set_text(pPangoLayout, pCurveWidget->strText[eTextLabel], -1);
		pango_layout_get_pixel_size(pPangoLayout, &nStrWidth, &nStrHeight);

		nHeight=nHeight>nStrHeight? nHeight:nStrHeight;
	}
	
	gtk_curve_widget_adjust_data_scale(pCurveWidget, &dMin, &dMax);

	sprintf(strAxis, "%d", (gint)dMax);
	pango_layout_set_text(pPangoLayout, strAxis, -1);
	pango_layout_get_pixel_size(pPangoLayout, &nStrWidth, &nStrHeight);

	sprintf(strAxis, "%d", (gint)dMin);
	pango_layout_set_text(pPangoLayout, strAxis, -1);
	pango_layout_get_pixel_size(pPangoLayout, &nTempWidth, &nTempHeight);

	nStrWidth=(nStrWidth>nTempWidth? nStrWidth:nTempWidth);
	nStrHeight=(nStrHeight>nTempHeight? nStrHeight:nTempHeight);

	pCurveWidget->rectCanvas.x=pAlloc->x+nStrWidth+10+5;
	pCurveWidget->rectCanvas.y=pAlloc->y+nHeight+nStrHeight/2+5;
	
	if (pCurveWidget->bEnable[eEnableTimeAxis])
	{
		switch (pCurveWidget->nTimeType)
		{
		case eTimeYear:
			pango_layout_set_text(pPangoLayout, (pConvert=_toUtf8EX("OIX_CSTR_TIME_YEAR")), -1);
			break;
		case eTimeMon:
			pango_layout_set_text(pPangoLayout, (pConvert=_toUtf8EX("OIX_CSTR_TIME_MON")), -1);
			break;
		case eTimeDay:
			pango_layout_set_text(pPangoLayout, (pConvert=_toUtf8EX("OIX_CSTR_TIME_DAY")), -1);
			break;
		case eTimeHour:
			pango_layout_set_text(pPangoLayout, (pConvert=_toUtf8EX("OIX_CSTR_TIME_HOUR")), -1);
			break;
		case eTimeMin:
			pango_layout_set_text(pPangoLayout, (pConvert=_toUtf8EX("OIX_CSTR_TIME_MIN")), -1);
			break;
		case eTimeSec:
			pango_layout_set_text(pPangoLayout, (pConvert=_toUtf8EX("OIX_CSTR_TIME_SEC")), -1);
			break;
		}
		G_SAFE_FREE(pConvert);//dzy1017

		pango_layout_get_pixel_size(pPangoLayout, &nStrWidth, &nStrHeight);
		
		if (pCurveWidget->nTimeType != eTimeYear)
		{
			nStrHeight<<=1;
		}
	}
	else
	{
		pango_layout_set_text(pPangoLayout, pCurveWidget->strText[eTextXAxisLabel], -1);
		pango_layout_get_pixel_size(pPangoLayout, &nStrWidth, &nStrHeight);
	}
	
	pCurveWidget->rectCanvas.width=pAlloc->width-pCurveWidget->rectCanvas.x+pAlloc->x-nStrWidth-15;
	pCurveWidget->rectCanvas.height=pAlloc->height-pCurveWidget->rectCanvas.y+pAlloc->y-nStrHeight-15;
	
	gdk_window_move_resize(pCurveWidget->pCanvas->window, pCurveWidget->rectCanvas.x, pCurveWidget->rectCanvas.y, 
		pCurveWidget->rectCanvas.width, pCurveWidget->rectCanvas.height);

	//计算消息窗口位置
	if (g_pPixbuf[ePixbufMessage] != NULL)
	{
		nWidth=gdk_pixbuf_get_width(g_pPixbuf[ePixbufMessage]);
		nHeight=gdk_pixbuf_get_height(g_pPixbuf[ePixbufMessage]);
	}
	else
	{
		nWidth=320;
		nHeight=240;
	}

	dXFactor=nWidth;
	dYFactor=nHeight;

	nWidth=nWidth<pCurveWidget->rectCanvas.width/2? nWidth:pCurveWidget->rectCanvas.width/2;
	nHeight=nHeight<pCurveWidget->rectCanvas.height/2? nHeight:pCurveWidget->rectCanvas.height/2;

	dXFactor=nWidth/dXFactor;
	dYFactor=nHeight/dYFactor;

	pCurveWidget->rectMessage.x=(pCurveWidget->rectCanvas.width-nWidth)/2;
	pCurveWidget->rectMessage.y=(pCurveWidget->rectCanvas.height-nHeight)/2;
	pCurveWidget->rectMessage.width=nWidth;
	pCurveWidget->rectMessage.height=nHeight;

	//计算消息窗口关闭按钮位置
	if (g_pPixbuf[ePixbufMessageClose] != NULL)
	{
		nWidth=gdk_pixbuf_get_width(g_pPixbuf[ePixbufMessageClose])/2;
		nHeight=gdk_pixbuf_get_height(g_pPixbuf[ePixbufMessageClose]);
	}
	else
	{
		nWidth=18;
		nHeight=18;
	}
	
	pCurveWidget->rectMessageClose.x=pCurveWidget->rectMessage.x+pCurveWidget->rectMessage.width-(gint)(nWidth*dXFactor);
	pCurveWidget->rectMessageClose.y=pCurveWidget->rectMessage.y;
	pCurveWidget->rectMessageClose.width=(gint)(nWidth*dXFactor);
	pCurveWidget->rectMessageClose.height=(gint)(nHeight*dYFactor);
}

void gtk_curve_widget_frame_expose_event(GtkWidget *pWidget, GdkEventExpose *pEvent, gpointer usr_data)
{
	gtk_curve_widget_frame_draw(pWidget, usr_data);
}

void gtk_curve_widget_draw_pango_layout(cairo_t* cr, GdkColor* color, gdouble x, gdouble y, PangoLayout* layout)
{
	G_ASSERT_NULL(cr);
	G_ASSERT_NULL(layout);

	if (color != NULL)
	{
		gdk_cairo_set_source_color(cr, color);
	}
	
	cairo_move_to(cr, x, y);
	pango_cairo_show_layout(cr, layout);
}

void gtk_curve_widget_draw_line(cairo_t* cr, gdouble x1, gdouble y1, gdouble x2, gdouble y2)
{
	G_ASSERT_NULL(cr);

	cairo_move_to(cr, x1, y1);
	cairo_line_to(cr, x2, y2);

	cairo_stroke (cr);
}

void gtk_curve_widget_draw_polygon(cairo_t* cr, gdouble* x, gdouble* y, int size)
{
	int i;

	if (size <= 1)
	{
		return;
	}

	G_ASSERT_NULL(cr);

	if (size == 2)
	{
		gtk_curve_widget_draw_line(cr, *x, *y, *(x+1), *(y+1));
	}
	else
	{
		cairo_move_to(cr, *x, *y);
		for (i=1; i<size; i++)
		{
			cairo_line_to(cr, *(x+i), *(y+i));
		}
	}

	cairo_fill(cr);
}

void gtk_curve_widget_draw_indicator(cairo_t* cr, gdouble x, gdouble y)
{
	GdkColor color=rgb2gdk(128,128,128);

	cairo_arc(cr, x, y, 5, 0, 2*M_PI);
	cairo_fill(cr);

	cairo_save(cr);
	gdk_cairo_set_source_color(cr, &color);
	gtk_curve_widget_draw_line(cr, x-7, y, x+7, y);
	gtk_curve_widget_draw_line(cr, x, y-7, x, y+7);
	cairo_restore(cr);
}

void gtk_curve_widget_draw_rectangle(cairo_t* cr, gdouble x, gdouble y, gdouble width, gdouble height, gboolean fill)
{
	G_ASSERT_NULL(cr);

	cairo_rectangle(cr, x, y, width, height);

	if (fill)
	{
		cairo_fill(cr);
	}
	else
	{
		cairo_stroke (cr);
	}
}

void gtk_curve_widget_draw_pixpuf(cairo_t* cr, GdkPixbuf* pixbuf, 
								  gdouble sx, gdouble sy, gdouble sWidth, gdouble sHeight, 
								  gdouble x, gdouble y, gdouble width, gdouble height)
{
	GdkPixbuf* pSubPixbuf=NULL;
	GdkPixbuf* pScalePixbuf=NULL;

	G_ASSERT_NULL(cr);
	if (pixbuf == NULL)
	{
		return;
	}

	pSubPixbuf=gdk_pixbuf_new_subpixbuf(pixbuf, (gint)sx, (gint)sy, (gint)sWidth, (gint)sHeight);
	if (pSubPixbuf == NULL)
	{
		printf("gtk_curve_widget_draw_pixpuf() : pSubPixbuf==NULL, gdk_pixbuf_new_subpixbuf() Failed!");
		return;
	}

	pScalePixbuf=gdk_pixbuf_scale_simple(pSubPixbuf, (gint)width, (gint)height, GDK_INTERP_NEAREST);
	if (pScalePixbuf == NULL)
	{
		printf("gtk_curve_widget_draw_pixpuf() : pScalePixbuf==NULL, gdk_pixbuf_scale_simple() Failed!");
		gdk_pixbuf_unref(pSubPixbuf);
		return;
	}

	gdk_cairo_set_source_pixbuf(cr, pScalePixbuf, x, y);

	cairo_paint(cr);

	gdk_pixbuf_unref(pSubPixbuf);
	gdk_pixbuf_unref(pScalePixbuf);
}

gboolean gtk_curve_widget_print(cairo_t* cr, GtkCurveWidget* pCurveWidget, 
								gdouble x, gdouble y, gdouble width, gdouble height)
{
	cairo_surface_t* pSurface=NULL;
	cairo_t* crSurface=NULL;

	cairo_surface_t* pCanvasSurface=NULL;
	cairo_t* crCanvasSurface=NULL;

	gint nWidth=pCurveWidget->pFrame->allocation.width;
	gint nHeight=pCurveWidget->pFrame->allocation.height;

	GdkRectangle rect;

	G_ASSERT_NULL(cr);
	G_ASSERT_NULL(pCurveWidget);

	pSurface=cairo_image_surface_create(CAIRO_FORMAT_ARGB32, nWidth, nHeight);
	if (pSurface == NULL)
	{
		printf("gtk_curve_widget_print() : pSurface==NULL");
		return FALSE;
	}

	crSurface=cairo_create(pSurface);
	if (crSurface == NULL)
	{
		printf("gtk_curve_widget_print() : crSurface==NULL");
		cairo_surface_destroy(pSurface);
		return FALSE;
	}

	cairo_set_source_rgb(crSurface, 1.0, 1.0, 1.0);
	gtk_curve_widget_draw_rectangle(crSurface, 0, 0, nWidth, nHeight, TRUE);

	rect=pCurveWidget->rectCanvas;
	rect.x-=pCurveWidget->pFrame->allocation.x;
	rect.y-=pCurveWidget->pFrame->allocation.y;

	gtk_curve_widget_frame_draw_contents(crSurface, pCurveWidget, rect);

	pCanvasSurface=cairo_image_surface_create(CAIRO_FORMAT_ARGB32, pCurveWidget->rectCanvas.width, 
		pCurveWidget->rectCanvas.height);
	if (pCanvasSurface == NULL)
	{
		printf("gtk_curve_widget_print() : pCanvasSurface==NULL");
		cairo_destroy(crSurface);
		cairo_surface_destroy(pSurface);
		return FALSE;
	}

	crCanvasSurface=cairo_create(pCanvasSurface);
	if (crCanvasSurface == NULL)
	{
		printf("gtk_curve_widget_print() : crCanvasSurface==NULL");
		cairo_destroy(crSurface);
		cairo_surface_destroy(pSurface);
		cairo_surface_destroy(pCanvasSurface);
		return FALSE;
	}

	cairo_set_source_rgb(crCanvasSurface, 1.0, 1.0, 1.0);
	gtk_curve_widget_draw_rectangle(crCanvasSurface, 0, 0, 
		pCurveWidget->rectCanvas.width, pCurveWidget->rectCanvas.height, TRUE);

	gtk_curve_widget_canvas_draw_contents(crCanvasSurface, pCurveWidget);

	cairo_set_source_surface(crSurface, pCanvasSurface, rect.x, rect.y);
	cairo_paint(crSurface);

	cairo_destroy(crCanvasSurface);
	cairo_surface_destroy(pCanvasSurface);

	cairo_save(cr);

	cairo_scale(cr, width/nWidth, height/nHeight);
	cairo_set_source_surface(cr, pSurface, x*nWidth/width, y*nHeight/height);
	cairo_paint(cr);

	cairo_restore(cr);

	cairo_destroy(crSurface);
	cairo_surface_destroy(pSurface);

	return TRUE;
}

void gtk_curve_widget_frame_draw_contents(cairo_t* cr, GtkCurveWidget* pCurveWidget, GdkRectangle rect)
{
	gint64 nMajor, nMinor;
	gdouble dTemp,dTempInterval;
	gchar label[32];
	gboolean bFirst=TRUE;
	gdouble dInterval;
	gint nAixs;
	gint nStrAxisWidth=0, nStrAxisHeight=0;
	gint nStrWidth=0, nStrHeight=0;
	time_t timeTemp;
	gdouble dMin, dMax;
	gdouble dTime,dTempTime;
	gint nValue, nTemp=0;

	gchar *pConvert=0;
	struct tm curTime, tmTemp;

	G_ASSERT_NULL(cr);
	G_ASSERT_NULL(pCurveWidget);

	gtk_curve_widget_adjust_axis(pCurveWidget);

	cairo_set_line_width(cr, 1);
	cairo_set_antialias(cr, CAIRO_ANTIALIAS_NONE);

	if (pCurveWidget->nCalc < eCalcManual)
	{
		dMax=100.0;
		dMin=(pCurveWidget->bEnable[eEnableNegativeScale]? -100.0:0.0);
	}
	else
	{
		gtk_curve_widget_adjust_data_scale(pCurveWidget, &dMin, &dMax);
	}

	pango_layout_set_font_description (pCurveWidget->pPangoLayout, pCurveWidget->pFontDesc[eFontDescAxis]);

	sprintf(label, "%d", (gint)dMax);
	pango_layout_set_text(pCurveWidget->pPangoLayout, label, -1);
	pango_layout_get_pixel_size(pCurveWidget->pPangoLayout, &nStrAxisWidth, &nStrAxisHeight);

	sprintf(label, "%d", (gint)dMin);
	pango_layout_set_text(pCurveWidget->pPangoLayout, label, -1);
	pango_layout_get_pixel_size(pCurveWidget->pPangoLayout, &nStrWidth, &nStrHeight);

	nStrAxisWidth=nStrAxisWidth>nStrWidth? nStrAxisWidth:nStrWidth;
	nStrAxisHeight=nStrAxisHeight>nStrHeight? nStrAxisHeight:nStrHeight;
	
	if (strlen(pCurveWidget->strText[eTextSubtitle]) != 0)
	{
		pango_layout_set_font_description (pCurveWidget->pPangoLayout, pCurveWidget->pFontDesc[eFontDescSubTitle]);
		pango_layout_set_text(pCurveWidget->pPangoLayout, pCurveWidget->strText[eTextSubtitle], -1);
		pango_layout_get_pixel_size(pCurveWidget->pPangoLayout, &nStrWidth, &nStrHeight);

		gtk_curve_widget_draw_pango_layout(cr, &pCurveWidget->color[eColorSubTitle], rect.x+(rect.width-nStrWidth)/2,
			rect.y-nStrHeight-nStrAxisHeight/2, pCurveWidget->pPangoLayout);

		nTemp=nStrHeight;
	}

	if (strlen(pCurveWidget->strText[eTextTitle]) != 0)
	{
		pango_layout_set_font_description (pCurveWidget->pPangoLayout, pCurveWidget->pFontDesc[eFontDescTitle]);
		pango_layout_set_text(pCurveWidget->pPangoLayout, pCurveWidget->strText[eTextTitle], -1);
		pango_layout_get_pixel_size(pCurveWidget->pPangoLayout, &nStrWidth, &nStrHeight);

		gtk_curve_widget_draw_pango_layout(cr, &pCurveWidget->color[eColorTitle], rect.x+(rect.width-nStrWidth)/2, 
			rect.y-nStrHeight-nTemp-nStrAxisHeight/2, pCurveWidget->pPangoLayout);
	}

	pango_layout_set_font_description (pCurveWidget->pPangoLayout, pCurveWidget->pFontDesc[eFontDescAxis]);

	if (strlen(pCurveWidget->strText[eTextLabel]) != 0)
	{
		pango_layout_set_text(pCurveWidget->pPangoLayout, pCurveWidget->strText[eTextLabel], -1);
		pango_layout_get_pixel_size(pCurveWidget->pPangoLayout, &nStrWidth, &nStrHeight);

		gtk_curve_widget_draw_pango_layout(cr, &pCurveWidget->color[eColorLabel], 
			rect.x-10-(nStrAxisWidth>nStrWidth? nStrWidth:nStrAxisWidth),
			rect.y-nStrHeight-nStrAxisHeight/4-5, pCurveWidget->pPangoLayout);
	}
	
	//画Y轴
	gdk_cairo_set_source_color(cr, &pCurveWidget->color[eColorAxis]);
	gtk_curve_widget_draw_line(cr, rect.x-2, rect.y, rect.x-2, rect.y+rect.height);

	dInterval=pCurveWidget->dYAxisInterval;
// 	if (pCurveWidget->nCalc < eCalcManual)
// 	{
// 		dMin=(pCurveWidget->bEnable[eEnableNegativeScale]? -100.0:0);
// 		dMax=100.0;
// 	}
// 	else
// 	{
// 		dMin=pCurveWidget->dYAixsMin;
// 		dMax=pCurveWidget->dYAxisMax;
// 	}
	dTemp=gtk_curve_widget_scale_value(pCurveWidget->dScaleBottom, dMin, dMax);
	dMax=gtk_curve_widget_scale_value(pCurveWidget->dScaleTop, dMin, dMax);
	dMin=dTemp;

	//draw Minor
	dTempInterval=dInterval/ROW_MINOR_NUM;
	nMinor=(gint64)ceil(dMin/dTempInterval);
	gdk_cairo_set_source_color(cr, &pCurveWidget->color[eColorMinor]);
	while ((dTemp=dTempInterval*nMinor) <= dMax)
	{
		nAixs=rect.y+gtk_curve_widget_transform(dTemp, dMin, dMax, rect.height,TRUE);
		gtk_curve_widget_draw_line(cr, rect.x-5, nAixs, rect.x-2, nAixs);
		
		++nMinor;
	}

	//draw Major
	nMajor=(gint64)ceil(dMin/dInterval);
	while ((dTemp=dInterval*nMajor) <= dMax)
	{
		gdk_cairo_set_source_color(cr, &pCurveWidget->color[eColorMajor]);
		nAixs=rect.y+gtk_curve_widget_transform(dTemp, dMin, dMax, rect.height,TRUE);
		gtk_curve_widget_draw_line(cr, rect.x-8, nAixs, rect.x-2, nAixs);

		sprintf(label, "%d", (gint)dTemp);
		pango_layout_set_text(pCurveWidget->pPangoLayout, label, -1);
		pango_layout_get_pixel_size(pCurveWidget->pPangoLayout, &nStrWidth, &nStrHeight);

		if (dTemp > 0.0f)
		{
			gdk_cairo_set_source_color(cr, &pCurveWidget->color[eColorPositive]);
		}
		else if (dTemp == 0.0f)
		{
			gdk_cairo_set_source_color(cr, &pCurveWidget->color[eColorZero]);
		}
		else
		{
			gdk_cairo_set_source_color(cr, &pCurveWidget->color[eColorNegative]);
		}

		gtk_curve_widget_draw_pango_layout(cr, NULL, rect.x-10-nStrWidth, nAixs-nStrHeight/2, pCurveWidget->pPangoLayout);

		++nMajor;
	}

	//画X轴
	dMin=gtk_curve_widget_scale_value(pCurveWidget->dScaleLeft, pCurveWidget->dTimeMin, pCurveWidget->dTimeMax);
	dMax=gtk_curve_widget_scale_value(pCurveWidget->dScaleRight, pCurveWidget->dTimeMin, pCurveWidget->dTimeMax);
	dInterval=pCurveWidget->dTimeInterval;

	gdk_cairo_set_source_color(cr, &pCurveWidget->color[eColorAxis]);
	gtk_curve_widget_draw_line(cr, rect.x, rect.y+rect.height+2, rect.x+rect.width, rect.y+rect.height+2);

	if (pCurveWidget->bEnable[eEnableTimeAxis])
	{
		switch (pCurveWidget->nTimeType)
		{
		case eTimeYear:
			pango_layout_set_text(pCurveWidget->pPangoLayout, (pConvert=_toUtf8EX("OIX_CSTR_TIME_YEAR")), -1);
			break;
		case eTimeMon:
			pango_layout_set_text(pCurveWidget->pPangoLayout, (pConvert=_toUtf8EX("OIX_CSTR_TIME_MON")), -1);
			break;
		case eTimeDay:
			pango_layout_set_text(pCurveWidget->pPangoLayout, (pConvert=_toUtf8EX("OIX_CSTR_TIME_DAY")), -1);
			break;
		case eTimeHour:
			pango_layout_set_text(pCurveWidget->pPangoLayout, (pConvert=_toUtf8EX("OIX_CSTR_TIME_HOUR")), -1);
			break;
		case eTimeMin:
			pango_layout_set_text(pCurveWidget->pPangoLayout, (pConvert=_toUtf8EX("OIX_CSTR_TIME_MIN")), -1);
			break;
		case eTimeSec:
			pango_layout_set_text(pCurveWidget->pPangoLayout, (pConvert=_toUtf8EX("OIX_CSTR_TIME_SEC")), -1);
			break;
		}
		G_SAFE_FREE(pConvert);//dzy1017
		pango_layout_get_pixel_size(pCurveWidget->pPangoLayout, &nStrWidth, &nStrHeight);
		
		gtk_curve_widget_draw_pango_layout(cr, &pCurveWidget->color[eColorLabel], rect.x+rect.width+10,
			rect.y+rect.height+10, pCurveWidget->pPangoLayout);

		if (pCurveWidget->nTimeType < eTimeDay)
		{
			nMajor=(gint64)ceil(dMin/dInterval);
			
			//draw Minor
			dTempInterval=dInterval/COLUMN_MINOR_NUM;
			nMinor=(gint64)ceil(dMin/dTempInterval);
			gdk_cairo_set_source_color(cr, &pCurveWidget->color[eColorMinor]);
			while ((dTemp=dTempInterval*nMinor) <= dMax)
			{
				nAixs=rect.x+gtk_curve_widget_transform(dTemp, dMin, dMax, rect.width,FALSE);

				gtk_curve_widget_draw_line(cr, nAixs, rect.y+rect.height+2, nAixs, rect.y+rect.height+5);
				
				++nMinor;
			}
			
			//draw Major
			dTime=dInterval*nMajor;
			timeTemp=(time_t)dTime;
			curTime=*localtime(&timeTemp);
			curTime.tm_isdst=-1;
			while ((dTemp=dInterval*nMajor) <= dMax)
			{
				gdk_cairo_set_source_color(cr, &pCurveWidget->color[eColorMajor]);
				nAixs=rect.x+gtk_curve_widget_transform(dTemp, dMin, dMax, rect.width, FALSE);
				gtk_curve_widget_draw_line(cr, nAixs, rect.y+rect.height+2, nAixs, rect.y+rect.height+8);
				
				switch (pCurveWidget->nTimeType)
				{
				case eTimeHour:
					nValue=curTime.tm_hour;
					break;
				case eTimeMin:
					nValue=curTime.tm_min;
					break;
				case eTimeSec:
					nValue=curTime.tm_sec;
					break;
				}
				sprintf(label, "%d", nValue);
				
				pango_layout_set_text(pCurveWidget->pPangoLayout, label, -1);
				pango_layout_get_pixel_size(pCurveWidget->pPangoLayout, &nStrWidth, &nStrHeight);

				gtk_curve_widget_draw_pango_layout(cr, &pCurveWidget->color[eColorAxis], nAixs-nStrWidth/2,
					rect.y+rect.height+10, pCurveWidget->pPangoLayout);
				
				if (nValue == 0 || bFirst)
				{
					bFirst=FALSE;
					switch (pCurveWidget->nTimeType)
					{
					case eTimeHour:
						sprintf(label,"%d%s", curTime.tm_mday, (pConvert=_toUtf8EX(OIX_CSTR_TIME_DAY)));
						break;
					case eTimeMin:
						sprintf(label,"%d%s", curTime.tm_hour, (pConvert=_toUtf8EX(OIX_CSTR_TIME_HOUR)));
						break;
					case eTimeSec:
						sprintf(label,"%d%s", curTime.tm_min, (pConvert=_toUtf8EX(OIX_CSTR_TIME_MIN)));
						break;
					}
					G_SAFE_FREE(pConvert);//dzy1017
					
					pango_layout_set_text(pCurveWidget->pPangoLayout, label, -1);
					pango_layout_get_pixel_size(pCurveWidget->pPangoLayout, &nStrWidth, &nStrHeight);

					gtk_curve_widget_draw_pango_layout(cr, &pCurveWidget->color[eColorAxisLabel], nAixs-nStrWidth/2,
						rect.y+rect.height+nStrHeight+8, pCurveWidget->pPangoLayout);
				}
				
				++nMajor;
				curTime.tm_sec+=(gint)dInterval;
				mktime(&curTime);
			}
		}
		else if (pCurveWidget->nTimeType == eTimeDay)
		{
			timeTemp=(time_t)dMin;
			curTime=*localtime(&timeTemp);
			nTemp=(gint)pCurveWidget->dTimeInterval;
			curTime.tm_mday=1;
			curTime.tm_hour=0;
			curTime.tm_min=0;
			curTime.tm_sec=0;
			curTime.tm_isdst=-1;

			dTempTime=nTemp*24*3600.0;

			dTemp=mktime(&curTime);
			while (dTemp < dMax)
			{
				curTime.tm_mon++;
				dTime=mktime(&curTime);

				while (dTemp < dTime)
				{
					//draw minor
					gdk_cairo_set_source_color(cr, &pCurveWidget->color[eColorMinor]);
					for (nMinor=0; nMinor<COLUMN_MINOR_NUM; nMinor++)
					{
						dTempInterval=dTemp+nMinor*dTempTime/COLUMN_MINOR_NUM;
						
						if (dTempInterval < dTime)
						{
							nAixs=rect.x+gtk_curve_widget_transform(dTempInterval, dMin, dMax, rect.width, FALSE);
							if (nAixs>=rect.x && nAixs<=rect.x+rect.width)
							{
								gtk_curve_widget_draw_line(cr, nAixs, rect.y+rect.height+2, nAixs, rect.y+rect.height+5);
							}
						}
					}
					
					if (dTime-dTemp > dTempTime/3)
					{
						//draw major
						gdk_cairo_set_source_color(cr, &pCurveWidget->color[eColorMajor]);
						nAixs=rect.x+gtk_curve_widget_transform(dTemp, dMin, dMax, rect.width, FALSE);
						if (nAixs>=rect.x && nAixs<=rect.x+rect.width)
						{
							gtk_curve_widget_draw_line(cr, nAixs, rect.y+rect.height+2, nAixs, rect.y+rect.height+8);
							
							timeTemp=(time_t)dTemp;
							tmTemp=*localtime(&timeTemp);
							
							sprintf(label, "%d", tmTemp.tm_mday);
							
							pango_layout_set_text(pCurveWidget->pPangoLayout, label, -1);
							pango_layout_get_pixel_size(pCurveWidget->pPangoLayout, &nStrWidth, &nStrHeight);
							gtk_curve_widget_draw_pango_layout(cr, &pCurveWidget->color[eColorAxis], nAixs-nStrWidth/2,
								rect.y+rect.height+10, pCurveWidget->pPangoLayout);
							
							if (tmTemp.tm_mday == 1 || bFirst)
							{
								bFirst=FALSE;
								sprintf(label,"%d%s", tmTemp.tm_mon+1, (pConvert=_toUtf8EX("OIX_CSTR_TIME_MON")));
								G_SAFE_FREE(pConvert);//dzy1017
								
								pango_layout_set_text(pCurveWidget->pPangoLayout, label, -1);
								pango_layout_get_pixel_size(pCurveWidget->pPangoLayout, &nStrWidth, &nStrHeight);
								gtk_curve_widget_draw_pango_layout(cr, &pCurveWidget->color[eColorAxisLabel], 
									nAixs-nStrWidth/2,rect.y+rect.height+nStrHeight+8, pCurveWidget->pPangoLayout);
							}
						}
					}
					
					dTemp+=dTempTime;
				}

				dTemp=dTime;
			}
		}
		else if (pCurveWidget->nTimeType == eTimeMon)
		{
			timeTemp=(time_t)dMin;
			curTime=*localtime(&timeTemp);
			nTemp=(gint)pCurveWidget->dTimeInterval;
			curTime.tm_mon=(curTime.tm_mon/nTemp)*nTemp;
			curTime.tm_mday=1;
			curTime.tm_hour=0;
			curTime.tm_min=0;
			curTime.tm_sec=0;
			curTime.tm_isdst=-1;
			tmTemp=curTime;
			
			//draw minor
			gdk_cairo_set_source_color(cr, &pCurveWidget->color[eColorMinor]);
			dTemp=mktime(&tmTemp);
			while (dTemp <= dMax)
			{
				tmTemp.tm_mon+=nTemp;
				dTime=mktime(&tmTemp);
				dTempInterval=(dTime-dTemp)/COLUMN_MINOR_NUM;

				for (nMinor=0; nMinor<COLUMN_MINOR_NUM; nMinor++)
				{
					nAixs=rect.x+gtk_curve_widget_transform(dTemp+nMinor*dTempInterval, dMin, dMax, rect.width, FALSE);
					if (nAixs>= rect.x && nAixs<=rect.x+rect.width)
					{
						gtk_curve_widget_draw_line(cr, nAixs, rect.y+rect.height+2, nAixs, rect.y+rect.height+5);
					}
				}

				dTemp=dTime;
			}
			
			//draw major
			while ((dTemp=mktime(&curTime)) < dMax)
			{
				gdk_cairo_set_source_color(cr, &pCurveWidget->color[eColorMajor]);
				nAixs=rect.x+gtk_curve_widget_transform(dTemp, dMin, dMax, rect.width, FALSE);
				if (nAixs>=rect.x && nAixs<=rect.x+rect.width)
				{
					gtk_curve_widget_draw_line(cr, nAixs, rect.y+rect.height+2, nAixs, rect.y+rect.height+8);
					
					sprintf(label, "%d", curTime.tm_mon+1);
					
					pango_layout_set_text(pCurveWidget->pPangoLayout, label, -1);
					pango_layout_get_pixel_size(pCurveWidget->pPangoLayout, &nStrWidth, &nStrHeight);

					gtk_curve_widget_draw_pango_layout(cr, &pCurveWidget->color[eColorAxis], nAixs-nStrWidth/2,
						rect.y+rect.height+10, pCurveWidget->pPangoLayout);
					
					if (curTime.tm_mon == 0 || bFirst)
					{
						bFirst=FALSE;
						sprintf(label,"%d%s", curTime.tm_year+1900, (pConvert=_toUtf8EX("OIX_CSTR_TIME_YEAR")));
						G_SAFE_FREE(pConvert);//dzy1017
						
						pango_layout_set_text(pCurveWidget->pPangoLayout, label, -1);
						pango_layout_get_pixel_size(pCurveWidget->pPangoLayout, &nStrWidth, &nStrHeight);

						gtk_curve_widget_draw_pango_layout(cr, &pCurveWidget->color[eColorAxisLabel], nAixs-nStrWidth/2,
							rect.y+rect.height+nStrHeight+8, pCurveWidget->pPangoLayout);
					}
				}

				curTime.tm_mon+=nTemp;
			}
		}
		else
		{
			timeTemp=(time_t)dMin;
			curTime=*localtime(&timeTemp);
			nTemp=(gint)pCurveWidget->dTimeInterval;
			curTime.tm_year=(curTime.tm_year/nTemp)*nTemp;
			curTime.tm_mon=0;
			curTime.tm_mday=1;
			curTime.tm_hour=0;
			curTime.tm_min=0;
			curTime.tm_sec=0;
			curTime.tm_isdst=-1;
			tmTemp=curTime;

			//draw minor
			dTempInterval=pCurveWidget->dTimeInterval*12/COLUMN_MINOR_NUM;
			gdk_cairo_set_source_color(cr, &pCurveWidget->color[eColorMinor]);
			while ((dTemp=mktime(&tmTemp)) <= dMax)
			{
				nAixs=rect.x+gtk_curve_widget_transform(dTemp, dMin, dMax, rect.width, FALSE);
				if (nAixs>=rect.x && nAixs<=rect.x+rect.width)
				{
					gtk_curve_widget_draw_line(cr, nAixs, rect.y+rect.height+2, nAixs, rect.y+rect.height+5);
				}
				
				tmTemp.tm_mon+=(gint)dTempInterval;
			}
			
			//draw major
			while ((dTemp=mktime(&curTime)) < dMax)
			{
				gdk_cairo_set_source_color(cr, &pCurveWidget->color[eColorMajor]);
				nAixs=rect.x+gtk_curve_widget_transform(dTemp, dMin, dMax, rect.width, FALSE);
				if (nAixs>=rect.x && nAixs<=rect.x+rect.width)
				{
					gtk_curve_widget_draw_line(cr, nAixs, rect.y+rect.height+2,nAixs, rect.y+rect.height+8);
					
					sprintf(label, "%d", curTime.tm_year+1900);
					
					pango_layout_set_text(pCurveWidget->pPangoLayout, label, -1);
					pango_layout_get_pixel_size(pCurveWidget->pPangoLayout, &nStrWidth, &nStrHeight);

					gtk_curve_widget_draw_pango_layout(cr, &pCurveWidget->color[eColorAxis], nAixs-nStrWidth/2,
						rect.y+rect.height+10, pCurveWidget->pPangoLayout);
				}
				
				curTime.tm_year+=(gint)pCurveWidget->dTimeInterval;
			}
		}
	}
	else
	{
		if (strlen(pCurveWidget->strText[eTextXAxisLabel]) != 0)
		{
			pango_layout_set_text(pCurveWidget->pPangoLayout, pCurveWidget->strText[eTextXAxisLabel], -1);
			pango_layout_get_pixel_size(pCurveWidget->pPangoLayout, &nStrWidth, &nStrHeight);
			
			gtk_curve_widget_draw_pango_layout(cr, &pCurveWidget->color[eColorLabel], rect.x+rect.width+15,
					rect.y+rect.height+10, pCurveWidget->pPangoLayout);
		}

		//draw minor
		dTempInterval=dInterval/ROW_MINOR_NUM;
		nMinor=(gint64)ceil(dMin/dTempInterval);
		gdk_cairo_set_source_color(cr, &pCurveWidget->color[eColorMinor]);
		while ((dTemp=dTempInterval*nMinor) <= dMax)
		{
			nAixs=rect.x+gtk_curve_widget_transform(dTemp, dMin, dMax, rect.width, FALSE);
			if (nAixs>=rect.x && nAixs<=rect.x+rect.width)
			{
				gtk_curve_widget_draw_line(cr, nAixs, rect.y+rect.height+2, nAixs, rect.y+rect.height+5);
			}
			
			++nMinor;
		}
		
		//draw major
		nMajor=(gint64)ceil(dMin/dInterval);
		while ((dTemp=dInterval*nMajor) <= dMax)
		{
			gdk_cairo_set_source_color(cr, &pCurveWidget->color[eColorMajor]);
			nAixs=rect.x+gtk_curve_widget_transform(dTemp, dMin, dMax, rect.width, FALSE);
			if (nAixs>=rect.x && nAixs<=rect.x+rect.width)
			{
				gtk_curve_widget_draw_line(cr, nAixs, rect.y+rect.height+2,nAixs, rect.y+rect.height+8);
				
				sprintf(label, "%d", (int)dTemp);
				
				pango_layout_set_text(pCurveWidget->pPangoLayout, label, -1);
				pango_layout_get_pixel_size(pCurveWidget->pPangoLayout, &nStrWidth, &nStrHeight);
				
				gtk_curve_widget_draw_pango_layout(cr, &pCurveWidget->color[eColorMinor], nAixs-nStrWidth/2,
					rect.y+rect.height+10, pCurveWidget->pPangoLayout);
			}
			
			nMajor++;
		}
	}
}

void gtk_curve_widget_frame_draw(GtkWidget *pWidget, gpointer usr_data)
{
	cairo_t* cr=NULL;

	GtkCurveWidget* pCurveWidget=(GtkCurveWidget*)usr_data;
	G_ASSERT_NULL(pWidget);
	G_ASSERT_NULL(pCurveWidget);

	if (pWidget->window == NULL)
	{
		printf("gtk_curve_widget_frame_draw(): pWidget->window==NULL");
		return;
	}

	cr=gdk_cairo_create(pWidget->window);
	if (cr == NULL)
	{
		printf("gtk_curve_widget_frame_draw(): cr == NULL, gdk_cairo_create() failed!");
		return;
	}

	gtk_curve_widget_frame_draw_contents(cr, pCurveWidget, pCurveWidget->rectCanvas);
	
	cairo_destroy(cr);
}

void gtk_curve_widget_canvas_realize(GtkWidget* pWidget, gpointer usr_data)
{
	GtkCurveWidget* pCurveWidget=(GtkCurveWidget*)usr_data;
	G_ASSERT_NULL(pCurveWidget);
	
//	pCurveWidget->pCanvasGc = gdk_gc_new(pWidget->window);

	gtk_curve_widget_create_cursor(pCurveWidget);
	gtk_curve_widget_set_current_operation(pCurveWidget, eCurveOpPointer);

	gtk_curve_widget_set_canvas_bg_rgb_color(pCurveWidget, 0, 0, 0);
}

void gtk_curve_widget_set_canvas_bg_color(GtkCurveWidget* pCurveWidget, GdkColor colorBg)
{	
	G_ASSERT_NULL(pCurveWidget);
	G_ASSERT_NULL(pCurveWidget->pCanvas->window);

	gdk_window_set_background(pCurveWidget->pCanvas->window, &colorBg);
}

void gtk_curve_widget_set_canvas_bg_rgb_color(GtkCurveWidget* pCurveWidget, guint8 r, guint8 g, guint8 b)
{
	GdkColor color;
	color.red=(r<<8);
	color.green=(g<<8);
	color.blue=(b<<8);
	color.pixel=(r<<16)+(g<<8)+b;

	gtk_curve_widget_set_canvas_bg_color(pCurveWidget, color);
}

void gtk_curve_widget_canvas_expose_event(GtkWidget *pWidget, GdkEventExpose *pEvent, gpointer usr_data)
{
	gtk_curve_widget_canvas_draw(pWidget, usr_data);
}

void gtk_curve_widget_canvas_draw(GtkWidget *pWidget, gpointer usr_data)
{
	cairo_t* cr=NULL;

	GtkCurveWidget* pCurveWidget=(GtkCurveWidget*)usr_data;
	G_ASSERT_NULL(pCurveWidget);
	
	if (pWidget->window == NULL)
	{
		printf("gtk_curve_widget_canvas_draw() : pWidget->window==NULL");
		return;
	}
	
	cr=gdk_cairo_create(pWidget->window);
	if (cr == NULL)
	{
		printf("gtk_curve_widget_canvas_draw() : cr== NULL ,gdk_cairo_create() Failed!");
		return;
	}

	gtk_curve_widget_canvas_draw_contents(cr, pCurveWidget);

	cairo_destroy(cr);
}

void gtk_curve_widget_canvas_draw_contents(cairo_t* cr, GtkCurveWidget* pCurveWidget)
{
	G_ASSERT_NULL(cr);
	G_ASSERT_NULL(pCurveWidget);

	gtk_curve_widget_adjust_axis(pCurveWidget);

	cairo_set_line_width(cr, 1);
	cairo_set_antialias(cr, CAIRO_ANTIALIAS_NONE);
	
	gtk_curve_widget_canvas_draw_grid(cr, pCurveWidget);
	gtk_curve_widget_canvas_draw_curves_mark(cr, pCurveWidget);
	gtk_curve_widget_canvas_draw_choose_rect(cr, pCurveWidget);
	gtk_curve_widget_canvas_draw_curves(cr, pCurveWidget);
	gtk_curve_widget_canvas_draw_track(pCurveWidget->pCanvas, cr, pCurveWidget);
	gtk_curve_widget_canvas_draw_message(cr, pCurveWidget);
}

void gtk_curve_widget_canvas_draw_curves_mark(cairo_t* cr, GtkCurveWidget* pCurveWidget)
{
	gint nX=0,nY=0, nWidth=0, nHeight=0, nStrWidth=0, nStrHeight=0;
	gint nCurId, nIndex=-1;
	gdouble dMin, dMax;
	
	gchar str[256];
	gchar *pConvert=0;
	GCurve* pCurve;

	G_ASSERT_NULL(pCurveWidget);

	if (cr == NULL)
	{
		printf("gtk_curve_widget_canvas_draw_curves_mark() : cr == NULL");
		return;
	}

	if (!pCurveWidget->bEnable[eEnableCurvesMark])
	{
		return;
	}

	if (pCurveWidget->nNext <= 0)
	{
		return;
	}

	pango_layout_set_font_description (pCurveWidget->pPangoLayout, pCurveWidget->pFontDesc[eFontDescCurvesMark]);
	
	pango_layout_set_text(pCurveWidget->pPangoLayout, (pConvert=_toUtf8EX("OIX_CSTR_MAX_RANGE")), -1);
	G_SAFE_FREE(pConvert);//dzy1017
	pango_layout_get_pixel_size(pCurveWidget->pPangoLayout, NULL, &nStrHeight);
	nHeight=nStrHeight;

	pango_layout_set_text(pCurveWidget->pPangoLayout, (pConvert=_toUtf8EX("OIX_CSTR_MIN_RANGE")), -1);
	G_SAFE_FREE(pConvert);//dzy1017
	pango_layout_get_pixel_size(pCurveWidget->pPangoLayout, NULL, &nStrHeight);
	nHeight=nHeight>nStrHeight? nHeight:nStrHeight;

	if (pCurveWidget->nCalc != eCalcScale)
	{
		pango_layout_set_text(pCurveWidget->pPangoLayout, (pConvert=_toUtf8EX("OIX_STR_HI_RANGE")), -1);
		G_SAFE_FREE(pConvert);//dzy1017
		pango_layout_get_pixel_size(pCurveWidget->pPangoLayout, NULL, &nStrHeight);
		nHeight=nHeight>nStrHeight? nHeight:nStrHeight;
		
		pango_layout_set_text(pCurveWidget->pPangoLayout, (pConvert=_toUtf8EX("OIX_STR_LO_RANGE")), -1);
		G_SAFE_FREE(pConvert);//dzy1017
		pango_layout_get_pixel_size(pCurveWidget->pPangoLayout, NULL, &nStrHeight);
		nHeight=nHeight>nStrHeight? nHeight:nStrHeight;
	}

	nY=nHeight;

	//绘制曲线名称
	for(nCurId=0; nCurId<pCurveWidget->nNext; nCurId++)
	{
		pCurve=pCurveWidget->pCurves[nCurId];
		
		sprintf(str, "%s", pCurve->strText);
		pango_layout_set_text(pCurveWidget->pPangoLayout, str, -1);
		pango_layout_get_pixel_size(pCurveWidget->pPangoLayout, &nStrWidth, &nStrHeight);
		gtk_curve_widget_draw_pango_layout(cr, &pCurve->color, nX, nY, pCurveWidget->pPangoLayout);
		
		nY+=nStrHeight;
		nWidth=nWidth>nStrWidth? nWidth:nStrWidth;
	}

	nX+=nWidth+10;
	//绘制曲线上限
	pango_layout_set_text(pCurveWidget->pPangoLayout, (pConvert=_toUtf8EX("OIX_STR_HI_RANGE")), -1);
	G_SAFE_FREE(pConvert);//dzy1017

	pango_layout_get_pixel_size(pCurveWidget->pPangoLayout, &nStrWidth, &nStrHeight);
	gtk_curve_widget_draw_pango_layout(cr, &pCurveWidget->color[eColorCurvsMark], nX, 0, pCurveWidget->pPangoLayout);
	nWidth=nStrWidth;
	
	nY=nHeight;
	for(nCurId=0; nCurId<pCurveWidget->nNext; nCurId++)
	{
		pCurve=pCurveWidget->pCurves[nCurId];
		
		sprintf(str, "%.3f", gtk_curve_get_max_range(pCurve));
		pango_layout_set_text(pCurveWidget->pPangoLayout, str, -1);
		pango_layout_get_pixel_size(pCurveWidget->pPangoLayout, &nStrWidth, &nStrHeight);
		gtk_curve_widget_draw_pango_layout(cr, &pCurve->color, nX, nY, pCurveWidget->pPangoLayout);
		
		nY+=nStrHeight;
		nWidth=nWidth>nStrWidth? nWidth:nStrWidth;
	}

	nX+=nWidth+10;
	//绘制曲线下限
	pango_layout_set_text(pCurveWidget->pPangoLayout, (pConvert=_toUtf8EX("OIX_STR_LO_RANGE")), -1);
	G_SAFE_FREE(pConvert);//dzy1017

	pango_layout_get_pixel_size(pCurveWidget->pPangoLayout, &nStrWidth, &nStrHeight);
	gtk_curve_widget_draw_pango_layout(cr, &pCurveWidget->color[eColorCurvsMark], nX, 0, pCurveWidget->pPangoLayout);
	nWidth=nStrWidth;
	
	nY=nHeight;
	for(nCurId=0; nCurId<pCurveWidget->nNext; nCurId++)
	{
		pCurve=pCurveWidget->pCurves[nCurId];
		
		sprintf(str, "%.3f", gtk_curve_get_min_range(pCurve));
		pango_layout_set_text(pCurveWidget->pPangoLayout, str, -1);
		pango_layout_get_pixel_size(pCurveWidget->pPangoLayout, &nStrWidth, &nStrHeight);
		gtk_curve_widget_draw_pango_layout(cr, &pCurve->color, nX, nY, pCurveWidget->pPangoLayout);
		
		nY+=nStrHeight;
		nWidth=nWidth>nStrWidth? nWidth:nStrWidth;
	}

	if (pCurveWidget->nCalc != eCalcScale)
	{
		nX+=nWidth+10;
		//绘制曲线最大量程
		pango_layout_set_text(pCurveWidget->pPangoLayout, (pConvert=_toUtf8EX("OIX_CSTR_MAX_RANGE")), -1);
		G_SAFE_FREE(pConvert);//dzy1017
		
		pango_layout_get_pixel_size(pCurveWidget->pPangoLayout, &nStrWidth, &nStrHeight);
		gtk_curve_widget_draw_pango_layout(cr, &pCurveWidget->color[eColorCurvsMark], nX, 0, pCurveWidget->pPangoLayout);
		nWidth=nStrWidth;
		
		nY=nHeight;
		for(nCurId=0; nCurId<pCurveWidget->nNext; nCurId++)
		{
			pCurve=pCurveWidget->pCurves[nCurId];
			
			gtk_curve_widget_calculate_data_scale(pCurveWidget, pCurve, NULL, &dMax);
			sprintf(str, "%.3f", dMax);
			pango_layout_set_text(pCurveWidget->pPangoLayout, str, -1);
			pango_layout_get_pixel_size(pCurveWidget->pPangoLayout, &nStrWidth, &nStrHeight);
			gtk_curve_widget_draw_pango_layout(cr, &pCurve->color, nX, nY, pCurveWidget->pPangoLayout);
			
			nY+=nStrHeight;
			nWidth=nWidth>nStrWidth? nWidth:nStrWidth;
		}
		
		nX+=nWidth+10;
		//绘制曲线最小量程
		pango_layout_set_text(pCurveWidget->pPangoLayout, (pConvert=_toUtf8EX("OIX_CSTR_MIN_RANGE")), -1);
		G_SAFE_FREE(pConvert);//dzy1017
		
		pango_layout_get_pixel_size(pCurveWidget->pPangoLayout, &nStrWidth, &nStrHeight);
		gtk_curve_widget_draw_pango_layout(cr, &pCurveWidget->color[eColorCurvsMark], nX, 0, pCurveWidget->pPangoLayout);
		nWidth=nStrWidth;
		
		nY=nHeight;
		for(nCurId=0; nCurId<pCurveWidget->nNext; nCurId++)
		{
			pCurve=pCurveWidget->pCurves[nCurId];
			
			gtk_curve_widget_calculate_data_scale(pCurveWidget, pCurve, &dMin, NULL);
			sprintf(str, "%.3f", dMin);
			pango_layout_set_text(pCurveWidget->pPangoLayout, str, -1);
			pango_layout_get_pixel_size(pCurveWidget->pPangoLayout, &nStrWidth, &nStrHeight);
			gtk_curve_widget_draw_pango_layout(cr, &pCurve->color, nX, nY, pCurveWidget->pPangoLayout);
			
			nY+=nStrHeight;
			nWidth=nWidth>nStrWidth? nWidth:nStrWidth;
		}
	}
}

void gtk_curve_widget_canvas_draw_message(cairo_t* cr, GtkCurveWidget* pCurveWidget)
{
	GdkRectangle rect;
	gint nWidth=0, nHeight=0;
	GdkColor color;

	G_ASSERT_NULL(pCurveWidget);
	G_ASSERT_NULL(cr);


	if (!pCurveWidget->bEnable[eEnableMessage])
	{
		pCurveWidget->nCloseBtnStatue=eCloseBtnNormal;
		return;
	}

	cairo_save(cr);

	rect=pCurveWidget->rectCanvas;

	if (g_pPixbuf[ePixbufMessage] != NULL)
	{
		nWidth=gdk_pixbuf_get_width(g_pPixbuf[ePixbufMessage]);
		nHeight=gdk_pixbuf_get_height(g_pPixbuf[ePixbufMessage]);

		gtk_curve_widget_draw_pixpuf(cr, g_pPixbuf[ePixbufMessage], 0, 0, nWidth, nHeight, 
			pCurveWidget->rectMessage.x, pCurveWidget->rectMessage.y, 
			pCurveWidget->rectMessage.width, pCurveWidget->rectMessage.height);
	}
	else
	{
		nWidth=pCurveWidget->rectMessage.width;
		nHeight=pCurveWidget->rectMessage.height;
		color=rgb2gdk(212, 212, 212);

		gdk_cairo_set_source_color(cr, &color);
		gtk_curve_widget_draw_rectangle(cr, (rect.width-nWidth)/2, (rect.height-nHeight)/2, nWidth, nHeight, TRUE);
	}
	
	if (strlen(pCurveWidget->strText[eTextMessage]) != 0)
	{
		pango_layout_set_font_description (pCurveWidget->pPangoLayout, pCurveWidget->pFontDesc[eFontDescMessage]);
		pango_layout_set_text(pCurveWidget->pPangoLayout, pCurveWidget->strText[eTextMessage], -1);
		pango_layout_get_pixel_size(pCurveWidget->pPangoLayout, &nWidth, &nHeight);

		gtk_curve_widget_draw_pango_layout(cr, &pCurveWidget->color[eColorMessage], (rect.width-nWidth)/2,
			(rect.height-nHeight)/2, pCurveWidget->pPangoLayout);
	}
	
	if (pCurveWidget->bEnable[eEnableMessageClose])
	{
		if (g_pPixbuf[ePixbufMessageClose] != NULL)
		{
			nWidth=gdk_pixbuf_get_width(g_pPixbuf[ePixbufMessageClose])/2;
			nHeight=gdk_pixbuf_get_height(g_pPixbuf[ePixbufMessageClose]);

			gtk_curve_widget_draw_pixpuf(cr, g_pPixbuf[ePixbufMessageClose], pCurveWidget->nCloseBtnStatue*nWidth, 0,
				nWidth, nHeight, pCurveWidget->rectMessageClose.x, pCurveWidget->rectMessageClose.y,
				pCurveWidget->rectMessageClose.width, pCurveWidget->rectMessageClose.height);
		}
		else
		{
			if (pCurveWidget->nCloseBtnStatue == eCloseBtnNormal)
			{
				color=rgb2gdk(0, 0, 0);
			}
			else
			{
				color=rgb2gdk(255, 0, 0);
			}

			cairo_set_line_width(cr, 2);
			gdk_cairo_set_source_color(cr, &color);
			
			rect=pCurveWidget->rectMessageClose;

			gtk_curve_widget_draw_line(cr, rect.x+rect.width/5, rect.y+rect.height/5,
				rect.x+rect.width*4/5, rect.y+rect.height*4/5);
			gtk_curve_widget_draw_line(cr, rect.x+rect.width/5, rect.y+rect.height*4/5,
				rect.x+rect.width*4/5, rect.y+rect.height/5);
		}
	}

	cairo_restore(cr);
}

void gtk_curve_widget_canvas_draw_grid(cairo_t* cr, GtkCurveWidget* pCurveWidget)
{
	gint64 nMajor,nMinor;
	gdouble dInterval,dTempInterval;
	gdouble dTemp;
	gint nAxis;
	gdouble dMin,dMax;

	time_t timeTemp;
	struct tm curTime, tmTemp;
	gint nTemp;
	gdouble dTime, dTempTime;

	GdkRectangle rect;

	G_ASSERT_NULL(pCurveWidget);

	if (cr == NULL)
	{
		printf("gtk_curve_widget_canvas_draw_grid() : cr== NULL");
		return;
	}
	
	rect=pCurveWidget->rectCanvas;
	
	cairo_save(cr);
	
	//绘制栅格
	cairo_set_dash(cr, dash_list, COUNT_OF(dash_list), 0);
	
	//画Y轴栅格	
	dInterval=pCurveWidget->dYAxisInterval;
	if (pCurveWidget->nCalc < eCalcManual)
	{
		dMin=(pCurveWidget->bEnable[eEnableNegativeScale]? -100.0:0);
		dMax=100.0;
	}
	else
	{
		gtk_curve_widget_adjust_data_scale(pCurveWidget, &dMin, &dMax);
	}

	dTemp=gtk_curve_widget_scale_value(pCurveWidget->dScaleBottom, dMin, dMax);
	dMax=gtk_curve_widget_scale_value(pCurveWidget->dScaleTop, dMin, dMax);
	dMin=dTemp;
	
	//draw Minor
	dTempInterval=dInterval/2;
	nMinor=(gint64)ceil(dMin/dTempInterval);
	gdk_cairo_set_source_color(cr, &pCurveWidget->color[eColorMinor]);
	while ((dTemp=dTempInterval*nMinor) <= dMax)
	{
		nAxis=gtk_curve_widget_transform(dTemp, dMin, dMax, rect.height,TRUE);
		gtk_curve_widget_draw_line(cr, 0, nAxis, rect.width, nAxis);
		
		++nMinor;
	}
	
	//draw Major
	nMajor=(gint64)ceil(dMin/dInterval);
	gdk_cairo_set_source_color(cr, &pCurveWidget->color[eColorMajor]);
	while ((dTemp=dInterval*nMajor) <= dMax)
	{
		nAxis=gtk_curve_widget_transform(dTemp, dMin, dMax, rect.height,TRUE);

		if (dTemp == 0.0)
		{
			cairo_save(cr);

			cairo_set_dash(cr, NULL, 0, 0);
			cairo_set_line_width(cr, 2);
			gdk_cairo_set_source_color(cr, &pCurveWidget->color[eColorZero]);
			
			gtk_curve_widget_draw_line(cr, 0, nAxis, rect.width, nAxis);

			cairo_restore(cr);
		}
		else
		{
			gtk_curve_widget_draw_line(cr, 0, nAxis, rect.width, nAxis);
		}
		
		++nMajor;
	}

	//画X轴栅格
	dMin=gtk_curve_widget_scale_value(pCurveWidget->dScaleLeft, pCurveWidget->dTimeMin, pCurveWidget->dTimeMax);
	dMax=gtk_curve_widget_scale_value(pCurveWidget->dScaleRight, pCurveWidget->dTimeMin, pCurveWidget->dTimeMax);
	dInterval=pCurveWidget->dTimeInterval;

	if (pCurveWidget->bEnable[eEnableTimeAxis])
	{
		if (pCurveWidget->nTimeType < eTimeDay)
		{
			//draw Minor
			if (pCurveWidget->nNumMajorCol < COLUMN_MINOR_MIN_NUM)
			{
				dTempInterval=dInterval/2;
				nMinor=(gint64)ceil(dMin/dTempInterval);
				gdk_cairo_set_source_color(cr, &pCurveWidget->color[eColorMinor]);
				while ((dTemp=dTempInterval*nMinor) <= dMax)
				{
					nAxis=gtk_curve_widget_transform(dTemp, dMin, dMax, rect.width,FALSE);
					gtk_curve_widget_draw_line(cr, nAxis, 0, nAxis, rect.height);
					
					++nMinor;
				}
			}
			
			//draw Major
			nMajor=(gint64)ceil(dMin/dInterval);
			gdk_cairo_set_source_color(cr, &pCurveWidget->color[eColorMajor]);
			while ((dTemp=dInterval*nMajor) <= dMax)
			{
				nAxis=gtk_curve_widget_transform(dTemp, dMin, dMax, rect.width, FALSE);
				gtk_curve_widget_draw_line(cr, nAxis, 0, nAxis, rect.height);
				
				++nMajor;
			}
		}
		else if (pCurveWidget->nTimeType == eTimeDay)
		{
			timeTemp=(time_t)dMin;
			curTime=*localtime(&timeTemp);
			nTemp=(gint)pCurveWidget->dTimeInterval;
			curTime.tm_mday=1;
			curTime.tm_hour=0;
			curTime.tm_min=0;
			curTime.tm_sec=0;
			curTime.tm_isdst=-1;
			
			dTempTime=nTemp*24*3600.0;
			
			dTemp=mktime(&curTime);
			while (dTemp < dMax)
			{
				curTime.tm_mon++;
				dTime=mktime(&curTime);
				
				while (dTemp < dTime)
				{
					if (pCurveWidget->nNumMajorCol < COLUMN_MINOR_MIN_NUM)
					{
						//draw minor
						gdk_cairo_set_source_color(cr, &pCurveWidget->color[eColorMinor]);
						for (nMinor=0; nMinor<2; nMinor++)
						{
							dTempInterval=dTemp+nMinor*dTempTime/2;
							
							if (dTime-dTempInterval > dTempTime/6)
							{
								nAxis=gtk_curve_widget_transform(dTempInterval, dMin, dMax, rect.width, FALSE);
								if (nAxis>=0 && nAxis<=rect.width)
								{
									gtk_curve_widget_draw_line(cr, nAxis, 0, nAxis, rect.height);
								}
							}
						}
					}
					
					if (dTime-dTemp > dTempTime/3)
					{
						//draw major
						gdk_cairo_set_source_color(cr, &pCurveWidget->color[eColorMajor]);
						nAxis=gtk_curve_widget_transform(dTemp, dMin, dMax, rect.width, FALSE);
						if (nAxis>=0 && nAxis<=rect.width)
						{
							gtk_curve_widget_draw_line(cr, nAxis, 0, nAxis, rect.height);
						}
					}
					
					dTemp+=dTempTime;
				}
				
				dTemp=dTime;
			}
		}
		else if (pCurveWidget->nTimeType == eTimeMon)
		{
			timeTemp=(time_t)dMin;
			curTime=*localtime(&timeTemp);
			nTemp=(gint)pCurveWidget->dTimeInterval;
			curTime.tm_mon=(curTime.tm_mon/nTemp)*nTemp;
			curTime.tm_mday=1;
			curTime.tm_hour=0;
			curTime.tm_min=0;
			curTime.tm_sec=0;
			curTime.tm_isdst=-1;
			tmTemp=curTime;
			
			if (pCurveWidget->nNumMajorCol < COLUMN_MINOR_MIN_NUM)
			{
				//draw minor
				gdk_cairo_set_source_color(cr, &pCurveWidget->color[eColorMinor]);
				dTemp=mktime(&tmTemp);
				while (dTemp <= dMax)
				{
					tmTemp.tm_mon+=nTemp;
					dTime=mktime(&tmTemp);
					dTempInterval=(dTime-dTemp)/2;
					
					for (nMinor=0; nMinor<2; nMinor++)
					{
						nAxis=gtk_curve_widget_transform(dTemp+nMinor*dTempInterval, dMin, dMax, rect.width, FALSE);
						if (nAxis>= 0 && nAxis<=rect.width)
						{
							gtk_curve_widget_draw_line(cr, nAxis, 0, nAxis, rect.height);
						}
					}
					
					dTemp=dTime;
				}
			}

			//draw major
			gdk_cairo_set_source_color(cr, &pCurveWidget->color[eColorMajor]);
			while ((dTemp=mktime(&curTime)) < dMax)
			{
				nAxis=gtk_curve_widget_transform(dTemp, dMin, dMax, rect.width, FALSE);
				if (nAxis>=0 && nAxis<=rect.width)
				{
					gtk_curve_widget_draw_line(cr, nAxis, 0, nAxis, rect.height);
				}

				curTime.tm_mon+=(gint)pCurveWidget->dTimeInterval;
				mktime(&curTime);
			}
		}
		else
		{
			timeTemp=(time_t)dMin;
			curTime=*localtime(&timeTemp);
			nTemp=(gint)pCurveWidget->dTimeInterval;
			curTime.tm_year=(curTime.tm_year/nTemp)*nTemp;
			curTime.tm_mon=0;
			curTime.tm_mday=1;
			curTime.tm_hour=0;
			curTime.tm_min=0;
			curTime.tm_sec=0;
			curTime.tm_isdst=-1;
			tmTemp=curTime;

			//draw minor
			if (pCurveWidget->nNumMajorCol < COLUMN_MINOR_MIN_NUM)
			{
				dTempInterval=pCurveWidget->dTimeInterval*12/2;
				gdk_cairo_set_source_color(cr, &pCurveWidget->color[eColorMinor]);
				while ((dTemp=mktime(&tmTemp)) <= dMax)
				{
					nAxis=gtk_curve_widget_transform(dTemp, dMin, dMax, rect.width, FALSE);
					if (nAxis>=0 && nAxis<=rect.width)
					{
						gtk_curve_widget_draw_line(cr, nAxis, 0, nAxis, rect.height);
					}
					
					tmTemp.tm_mon+=(gint)dTempInterval;
					mktime(&tmTemp);
				}
			}
			
			//draw major
			gdk_cairo_set_source_color(cr, &pCurveWidget->color[eColorMajor]);
			while ((dTemp=mktime(&curTime)) < dMax)
			{
				nAxis=gtk_curve_widget_transform(dTemp, dMin, dMax, rect.width, FALSE);
				if (nAxis>=0 && nAxis<=rect.width)
				{
					gtk_curve_widget_draw_line(cr, nAxis, 0, nAxis, rect.height);
				}
				
				curTime.tm_year+=nTemp;
				mktime(&curTime);
			}
		}
		cairo_restore(cr);
	}
	else
	{
		//draw minor
		if (pCurveWidget->nNumMajorCol < COLUMN_MINOR_MIN_NUM)
		{
			dTempInterval=pCurveWidget->dTimeInterval/2;
			nMinor=(gint64)ceil(dMin/dTempInterval);
			gdk_cairo_set_source_color(cr, &pCurveWidget->color[eColorMinor]);
			while ((dTemp=dTempInterval*nMinor) <= dMax)
			{
				nAxis=gtk_curve_widget_transform(dTemp, dMin, dMax, rect.width, FALSE);
				if (nAxis>=0 && nAxis<=rect.width)
				{
					gtk_curve_widget_draw_line(cr, nAxis, 0, nAxis, rect.height);
				}

				++nMinor;
			}
		}
		
		//draw major
		nMajor=(gint64)ceil(dMin/pCurveWidget->dTimeInterval);
		gdk_cairo_set_source_color(cr, &pCurveWidget->color[eColorMajor]);
		while ((dTemp=dInterval*nMajor) <= dMax)
		{
			nAxis=gtk_curve_widget_transform(dTemp, dMin, dMax, rect.width, FALSE);
			if (nAxis>=0 && nAxis<=rect.width)
			{
				gtk_curve_widget_draw_line(cr, nAxis, 0, nAxis, rect.height);
			}
			
			nMajor++;
		}

		cairo_restore(cr);
	}
}

void gtk_curve_widget_canvas_draw_choose_rect(cairo_t* cr, GtkCurveWidget* pCurveWidget)
{
	gint nx,ny,nWidth,nHeight;		//区域放大矩形框

	G_ASSERT_NULL(pCurveWidget);

	if (cr == NULL)
	{
		printf("gtk_curve_widget_canvas_draw_choose_rect() : cr==NULL");
		return;
	}

	cairo_save(cr);

	//绘制缩放矩形框
	if (pCurveWidget->bPressed && pCurveWidget->nCurveOpType== eCurveOpZoomRect)
	{
		gdk_cairo_set_source_color(cr, &pCurveWidget->color[eColorChooseRect]);
		
		if (pCurveWidget->dPtStartX < pCurveWidget->dPtEndX)
		{
			nx=(gint)pCurveWidget->dPtStartX;
			nWidth=(gint)(pCurveWidget->dPtEndX-pCurveWidget->dPtStartX);
		}
		else
		{
			nx=(gint)pCurveWidget->dPtEndX;
			nWidth=(gint)(pCurveWidget->dPtStartX-pCurveWidget->dPtEndX);
		}
		
		if (pCurveWidget->dPtStartY < pCurveWidget->dPtEndY)
		{
			ny=(gint)pCurveWidget->dPtStartY;
			nHeight=(gint)(pCurveWidget->dPtEndY-pCurveWidget->dPtStartY);
		}
		else
		{
			ny=(gint)(pCurveWidget->dPtEndY);
			nHeight=(gint)(pCurveWidget->dPtStartY-pCurveWidget->dPtEndY);
		}
		
		gtk_curve_widget_draw_rectangle(cr, nx, ny, nWidth, nHeight, FALSE);
	}

	cairo_restore(cr);
}

void gtk_curve_widget_calculate_data_scale(GtkCurveWidget* pCurveWidget, GCurve* pCurve, gdouble* dMin, gdouble* dMax)
{
	gdouble dLow, dHigh;

	G_ASSERT_NULL(pCurveWidget);
	G_ASSERT_NULL(pCurve);

	//y轴量程
	if (pCurveWidget->nCalc==eCalcScale)
	{
		dLow=pCurve->dMin;
		dHigh=pCurve->dMax;
		
//		adjust_data_scale(&dLow, &dHigh);

		if (dLow == dHigh)
		{
			dHigh=dLow+10.0;
		}
		
		if (pCurveWidget->bEnable[eEnableNegativeScale])
		{
			dHigh=(fabs(dHigh)>fabs(dLow)? fabs(dHigh):fabs(dLow));
			dLow=-dHigh;
		}
	}
	else
	{
		gtk_curve_widget_adjust_data_scale(pCurveWidget, &dLow, &dHigh);
// 		dLow=pCurveWidget->dYAixsMin;
// 		dHigh=pCurveWidget->dYAxisMax;
	}

	if (dMin != NULL)
	{
		*dMin=dLow;
	}

	if (dMax != NULL)
	{
		*dMax=dHigh;
	}
}

void gtk_curve_widget_canvas_draw_curves(cairo_t* cr, GtkCurveWidget* pCurveWidget)
{
	gint nCurId,nOffset;
	gdouble dMin,dMax,dStep,dTemp;
	gdouble dTimeMin, dTimeMax;
	gint x1,y1,x2,y2;
	gint nNumPt;
	gdouble dAxis, dTime, dValue;
	gboolean bRtn;

	gint nSample;
	gint i;

	GCurve* pCurve;

	PangoLayout* pPangoLayout;
	PangoFontDescription* pFontDesc;
	int nStrWidth, nStrHeight;

	G_ASSERT_NULL(pCurveWidget);

	if (pCurveWidget->nNext <= 0)
	{
		return;
	}

	G_ASSERT_NULL(cr);
	cairo_save(cr);

	dTimeMin=gtk_curve_widget_scale_value(pCurveWidget->dScaleLeft, pCurveWidget->dTimeMin, pCurveWidget->dTimeMax);
	dTimeMax=gtk_curve_widget_scale_value(pCurveWidget->dScaleRight, pCurveWidget->dTimeMin, pCurveWidget->dTimeMax);

	nSample=gtk_curve_widget_sample(dTimeMin, dTimeMax, pCurveWidget->rectCanvas.width, pCurveWidget->nSamplePixel);

	//绘制曲线
	for(nCurId=0; nCurId < pCurveWidget->nNext; nCurId++)
	{
		pCurve=pCurveWidget->pCurves[nCurId];

		if (pCurve->nLenBuf <= 0)
		{
			continue;
		}

		cairo_save(cr);

		gdk_cairo_set_source_color(cr, &pCurve->color);

		//y轴量程
// 		if (pCurveWidget->nCalc == eCalcScale)
// 		{
// 			dMin=pCurve->dMin;
// 			dMax=pCurve->dMax;
// 			
// 			adjust_data_scale(&dMin, &dMax);
// 			
// 			dMax=(fabs(dMax)>fabs(dMin)? fabs(dMax):fabs(dMin));
// 			dMin=(pCurveWidget->bEnable[eEnableNegativeScale]? -dMax:0);
// 		}
// 		else
// 		{
// 			dMin=pCurveWidget->dYAixsMin;
// 			dMax=pCurveWidget->dYAxisMax;
// 		}

		gtk_curve_widget_calculate_data_scale(pCurveWidget, pCurve, &dMin, &dMax);

		dTemp=gtk_curve_widget_scale_value(pCurveWidget->dScaleBottom, dMin, dMax);
		dMax=gtk_curve_widget_scale_value(pCurveWidget->dScaleTop, dMin, dMax);
		dMin=dTemp;

		//绘制
		nNumPt=pCurve->nNumPt;
		nOffset=pCurve->nOffset%pCurve->nLenBuf;

		if (pCurve->nTimeMode == eModeNormalAndNoSample)
		{
			if (nNumPt <= 0)
			{
				break;
			}

			if (pCurve->nCurveType == eCurveDash)
			{
				cairo_set_dash(cr, g_dash, 2, 0);
			}
			else
			{
				cairo_set_dash(cr, NULL, 0, 0);
			}

			dAxis=pCurve->pXAxis[nOffset];
			dValue=pCurve->pYAxis[nOffset];
			x1=gtk_curve_widget_transform(dAxis, dTimeMin, dTimeMax, pCurveWidget->rectCanvas.width, FALSE);
			y1=gtk_curve_widget_transform(dValue, dMin, dMax, pCurveWidget->rectCanvas.height, TRUE);

			if (pCurve->nCurveType == eCurveFill)
			{
				cairo_move_to(cr, x1, y1);
			}

			if (pCurve->nCurveType == eCurveIndicator)
			{
				gtk_curve_widget_draw_indicator(cr, x1, y1);
			}

			for (i=1; i<nNumPt; i++)
			{
				dAxis=pCurve->pXAxis[(nOffset+i)%pCurve->nLenBuf];
				dValue=pCurve->pYAxis[(nOffset+i)%pCurve->nLenBuf];

				x2=gtk_curve_widget_transform(dAxis, dTimeMin, dTimeMax, pCurveWidget->rectCanvas.width, FALSE);
				y2=gtk_curve_widget_transform(dValue, dMin, dMax, pCurveWidget->rectCanvas.height, TRUE);

				if (pCurve->nCurveType == eCurveFill)
				{
					cairo_line_to(cr, x2, y2);
				}
				else if (pCurve->nCurveType == eCurveIndicator)
				{
					gtk_curve_widget_draw_indicator(cr, x2, y2);
				}
				else if (pCurve->nCurveType == eCurveLadder && y1 != y2)
				{
					gtk_curve_widget_draw_line(cr, x1, y1, x2, y1);
					gtk_curve_widget_draw_line(cr, x2, y1, x2, y2);
				}
				else
				{
					gtk_curve_widget_draw_line(cr, x1, y1, x2, y2);
				}
				
				x1=x2;
				y1=y2;
			}

			if (pCurve->nCurveType == eCurveFill)
			{
				cairo_fill(cr);
			}
		}
		else 
		{
			if (pCurve->nTimeMode == eModeNormal)
			{
				dAxis=pCurve->pXAxis[nOffset];
			}
			else
			{
				dStep=pCurve->dStep*pow(60.0, pCurve->nTimeMode-1);
				dAxis=*pCurve->pXAxis;
			}

			dTime=dTimeMin;
			do 
			{
				bRtn=gtk_curve_get_sample_value(pCurve, dTime, nSample, dStep, &nNumPt, &dAxis, &nOffset, &dValue);
				if (bRtn || nNumPt <= 0)
				{
					break;
				}

				dTime+=nSample;
			} while (TRUE);

			if (nNumPt <= 0)
			{
				continue;
			}

			if (pCurve->nCurveType == eCurveDash)
			{
				cairo_set_dash(cr, g_dash, 2, 0);
			}
			else
			{
				cairo_set_dash(cr, NULL, 0, 0);
			}

			x1=gtk_curve_widget_transform(dTime, dTimeMin, dTimeMax, pCurveWidget->rectCanvas.width, FALSE);
			y1=gtk_curve_widget_transform(dValue, dMin, dMax, pCurveWidget->rectCanvas.height, TRUE);

			while(dTime <= dTimeMax && nNumPt > 0)
			{
				dTime+=nSample;

				do 
				{
					bRtn=gtk_curve_get_sample_value(pCurve, dTime, nSample, dStep, &nNumPt, &dAxis, &nOffset, &dValue);
					if (bRtn || nNumPt <= 0)
					{
						break;
					}
					
					dTime+=nSample;
				} while (TRUE);

				x2=gtk_curve_widget_transform(dTime, dTimeMin, dTimeMax, pCurveWidget->rectCanvas.width, FALSE);
				y2=gtk_curve_widget_transform(dValue, dMin, dMax, pCurveWidget->rectCanvas.height, TRUE);

				if (pCurve->nCurveType == eCurveLadder && y1 != y2)
				{
					gtk_curve_widget_draw_line(cr, x1, y1, x2, y1);
					gtk_curve_widget_draw_line(cr, x2, y1, x2, y2);
				}
				else
				{
					gtk_curve_widget_draw_line(cr, x1, y1, x2, y2);
				}
				
				x1=x2;
				y1=y2;
			}
		}

		cairo_restore(cr);
	}

	//画label
	pPangoLayout=pCurveWidget->pPangoLayout;
	pFontDesc=pCurveWidget->pFontDesc[eFontDescTrack];
	pango_layout_set_font_description (pPangoLayout, pFontDesc);
	for(nCurId=0; nCurId < pCurveWidget->nNext; nCurId++)
	{
		pCurve=pCurveWidget->pCurves[nCurId];
		
		if (pCurve->nTimeMode != eModeNormalAndNoSample || pCurve->nLenBuf <= 0 || pCurve->nLabelPos == eCurveNoLabel)
		{
			continue;
		}
		
		gdk_cairo_set_source_color(cr, &pCurve->color);
		
		pango_layout_set_text(pPangoLayout, pCurve->strText, -1);

		nNumPt=pCurve->nNumPt;
		nOffset=pCurve->nOffset%pCurve->nLenBuf;

		switch (pCurve->nLabelPos)
		{
		case eCurveLabelStart:
			{
				dAxis=pCurve->pXAxis[nOffset];
				dValue=pCurve->pYAxis[nOffset];
			}
			break;
		case eCurveLabelMid:
			{
				dAxis=pCurve->pXAxis[(nOffset+(nNumPt-1)/2)%pCurve->nLenBuf];
				dValue=pCurve->pYAxis[(nOffset+(nNumPt-1)/2)%pCurve->nLenBuf];

				if (nNumPt%2 == 0)
				{
					dAxis+=pCurve->pXAxis[(nOffset+nNumPt/2)%pCurve->nLenBuf];
					dValue+=pCurve->pYAxis[(nOffset+nNumPt/2)%pCurve->nLenBuf];

					dAxis/=2;
					dValue/=2;
				}
			}
			break;
		case eCurveLabelEnd:
			{
				dAxis=pCurve->pXAxis[(nOffset+(nNumPt-1))%pCurve->nLenBuf];
				dValue=pCurve->pYAxis[(nOffset+(nNumPt-1))%pCurve->nLenBuf];
			}
			break;
		default:
			break;
		}

		x1=gtk_curve_widget_transform(dAxis, dTimeMin, dTimeMax, pCurveWidget->rectCanvas.width, FALSE);
		y1=gtk_curve_widget_transform(dValue, dMin, dMax, pCurveWidget->rectCanvas.height, TRUE);

		pango_layout_get_pixel_size(pPangoLayout, &nStrWidth, &nStrHeight);

		switch (pCurve->nLabelPos)
		{
		case eCurveLabelStart:
			{
				gtk_curve_widget_draw_pango_layout(cr, NULL, x1-nStrWidth-2, y1-nStrHeight-2, pPangoLayout);
			}
			break;
		case eCurveLabelMid:
			{
				gtk_curve_widget_draw_pango_layout(cr, NULL, x1-nStrWidth/2, y1-nStrHeight/2, pPangoLayout);
			}
			break;
		case eCurveLabelEnd:
			{
				gtk_curve_widget_draw_pango_layout(cr, NULL, x1+2, y1-nStrHeight/2, pPangoLayout);
			}
			break;
		default:
			break;
		}
	}

	cairo_restore(cr);
}

/**
* @Param dSample 采样点
* @Param dRange 采样范围
* @Param dStep 曲线步长模式下的步长，其他不使用
* @Param nNumPt input/output 最多检测点数,返回剩余未检测点数
* @Param dAxis input/output 曲线步长模式下nOffset时的x轴坐标值
* @Param nOffset  input/output 数据数组起始偏移量
* @Param dValue output 采样值
* @Return bool true 采样值有效，false 采样值无效
**/
gboolean gtk_curve_get_sample_value(GCurve* pCurve, gdouble dSample, gdouble dRange, gdouble dStep, gint* nNumPt,
								gdouble* dAxis, gint* nOffset, gdouble* dValue)
{
	gdouble dData, dTemp;
	gint nIndex, nNum=0;

	G_ASSERT_NULL(pCurve);
	g_assert(dStep > 0.0);

	if (*nNumPt <= 0)
	{
		return FALSE;
	}

	nIndex=(*nOffset)%pCurve->nLenBuf;
	if (pCurve->nTimeMode == eModeNormal)
	{
		if (pCurve->pXAxis[nIndex] > dSample+dRange/2)
		{
			return FALSE;	//采样点范围内无值
		}

		while (pCurve->pXAxis[nIndex] < dSample-dRange/2 && nNum<*nNumPt)
		{
			nNum++;
			nIndex++;
			nIndex%=pCurve->nLenBuf;
		}

		if (nNum >= *nNumPt)
		{
			*nNumPt-=nNum;
			*nOffset=nIndex;
			return FALSE;
		}

		dData=pCurve->pYAxis[nIndex];
		nNum++;
		nIndex++;
		nIndex%=pCurve->nLenBuf;
		while(pCurve->pXAxis[nIndex] <= dSample+dRange/2 && nNum<*nNumPt)
		{
			dData=pCurve->pYAxis[nIndex]>dData? pCurve->pYAxis[nIndex]:dData;
			nNum++;
			nIndex++;
			nIndex%=pCurve->nLenBuf;
		}
	}
	else
	{
		dTemp=*dAxis;
		if (dTemp > dSample+dRange/2)
		{
			return FALSE;	//采样点范围内无值
		}

		while (dTemp < dSample-dRange/2 && nNum < *nNumPt)
		{
			dTemp+=dStep;
			nNum++;
			nIndex++;
			nIndex%=pCurve->nLenBuf;
		}
		
		if (nNum >= *nNumPt)
		{
			*nNumPt-=nNum;
			*nOffset=nIndex;
			return FALSE;
		}

		dData=pCurve->pYAxis[nIndex];

		dTemp+=dStep;
		nNum++;
		nIndex++;
		nIndex%=pCurve->nLenBuf;
		while(dTemp <= dSample+dRange/2 && nNum<*nNumPt)
		{
			dData=pCurve->pYAxis[nIndex]>dData? pCurve->pYAxis[nIndex]:dData;

			dTemp+=dStep;
			nIndex++;
			nNum++;
			nIndex%=pCurve->nLenBuf;
		}

		*dAxis=dTemp;
	}

	*nNumPt-=nNum;
	*nOffset=nIndex;
	*dValue=dData;

	return TRUE;
}

void gtk_curve_widget_canvas_draw_track(GtkWidget* pWidget, cairo_t* cr, GtkCurveWidget* pCurveWidget)
{
	gint nPosX,nPosY;
	gint nWidthTime;
	gint nX=0,nY=0,nWidth=0,nHeight=0,nStrWidth=0,nStrHeight=0,nTemp=0;
	gdouble dTemp;
	gint nCurId, nIndex=-1;

	gint nDataIndex[MAX_NUM_CURVES];
	
	time_t nTime;
	struct tm tmTime;
	gdouble dMin,dMax;
	
	gchar str[256];
	gchar strTime[32];
	gchar *pConvert=0;
	GdkRectangle rect;
	PangoLayout* pPangoLayout;
	PangoFontDescription* pFontDesc;

	GCurve* pCurve;

	G_ASSERT_NULL(pCurveWidget);

	if (!pCurveWidget->bEnable[eEnableTimeAxis] || !pCurveWidget->bEnable[eEnableTrack] || !pCurveWidget->bPressed)
	{
		return;
	}

	if (pCurveWidget->bEnable[eEnableScale] && pCurveWidget->nCurveOpType != eCurveOpPointer)
	{
		return;
	}

	G_ASSERT_NULL(pWidget);
	G_ASSERT_NULL(cr);

	cairo_save(cr);
	
	rect=pCurveWidget->rectCanvas;
	pPangoLayout=pCurveWidget->pPangoLayout;
	pFontDesc=pCurveWidget->pFontDesc[eFontDescTrack];

	gtk_widget_get_pointer(pWidget, &nPosX, &nPosY);
	
	nPosX=nPosX>0? nPosX:0;
	nPosX=nPosX<rect.width? nPosX:rect.width;
	nPosY=nPosY>0? nPosY:0;
	nPosY=nPosY<rect.height? nPosY:rect.height;

	dMin=gtk_curve_widget_scale_value(pCurveWidget->dScaleLeft, pCurveWidget->dTimeMin, pCurveWidget->dTimeMax);
	dMax=gtk_curve_widget_scale_value(pCurveWidget->dScaleRight, pCurveWidget->dTimeMin, pCurveWidget->dTimeMax);
	dTemp=gtk_curve_widget_invTransform(nPosX, dMin, dMax, rect.width, FALSE);
	nTime=(time_t)(dTemp+0.5);

	gdk_cairo_set_source_color(cr, &pCurveWidget->color[eColorTrackLine]);
	gtk_curve_widget_draw_line(cr, nPosX, 0, nPosX, rect.height);
	
	tmTime=*localtime(&nTime);

	pango_layout_set_font_description (pPangoLayout, pFontDesc);
	
	sprintf(strTime, "%04d-%02d-%02d %02d:%02d:%02d", tmTime.tm_year+1900, tmTime.tm_mon+1, tmTime.tm_mday,
		tmTime.tm_hour, tmTime.tm_min, tmTime.tm_sec) ;
	
	sprintf(str, "%s:%s", (pConvert=_toUtf8EX(OIX_STR_TIME)), strTime);
	G_SAFE_FREE(pConvert);//dzy1017

	pango_layout_set_text(pPangoLayout, str, -1);
	pango_layout_get_pixel_size(pPangoLayout, &nStrWidth, &nStrHeight);
	nHeight+=nStrHeight;
	nWidthTime=nStrWidth;
	
	nTemp=0;
	for(nCurId=0; nCurId<pCurveWidget->nNext; nCurId++)
	{
		pCurve=pCurveWidget->pCurves[nCurId];

		if (pCurve->nTimeMode == eModeNormal)
		{
			nIndex=binary_find(pCurve->pXAxis, pCurve->nLenBuf, pCurve->nOffset, pCurve->nNumPt, dTemp);
		}
		else 
		{
			nIndex=step_find(pCurve->nLenBuf, pCurve->nOffset, pCurve->nNumPt, *pCurve->pXAxis, 
				pCurve->dStep*pow(60.0, pCurve->nTimeMode-1), dTemp, pCurve->nCurveType!=eCurveLadder);
		}

		nDataIndex[nCurId]=nIndex;

		if (nIndex < 0)
		{
			strcpy(str,"");
		}
		else
		{
			if (pCurve->nDataType == eDataInt)
			{
				sprintf(str, "%d", pCurve->pYAxis[nIndex]);
			}
			else if (pCurve->nDataType == eDataFloat)
			{
				sprintf(str, "%.3f", pCurve->pYAxis[nIndex]);
			}
		}

		pango_layout_set_text(pPangoLayout, str, -1);
		pango_layout_get_pixel_size(pPangoLayout, &nStrWidth, NULL);
		nTemp=nTemp>nStrWidth? nTemp:nStrWidth;
	}
	
	for(nCurId = 0; nCurId<pCurveWidget->nNext; nCurId++)
	{
		sprintf(str, "%s:", pCurveWidget->pCurves[nCurId]->strText);
		pango_layout_set_text(pPangoLayout, str, -1);
		pango_layout_get_pixel_size(pPangoLayout, &nStrWidth, &nStrHeight);
		nWidth=nWidth>nStrWidth? nWidth:nStrWidth;
		nHeight+=nStrHeight;
	}
	
	nX=nPosX;
	nY=nPosY-nHeight;
	if (nX+nWidthTime > rect.width)
	{
		nX=rect.width-nWidthTime;
	}
	
	if (nX+nWidth+nTemp > rect.width)
	{
		nX=rect.width-nWidth-nTemp;
	}
	nY=nY>0?nY:0;
	
	sprintf(str, "%s:%s", (pConvert=_toUtf8EX(OIX_STR_TIME)), strTime);
	G_SAFE_FREE(pConvert);//dzy1017

	pango_layout_set_text(pPangoLayout, str, -1);
	pango_layout_get_pixel_size(pPangoLayout, NULL, &nStrHeight);
	gtk_curve_widget_draw_pango_layout(cr, &pCurveWidget->color[eColorTrackTime], nX, nY, pPangoLayout);
	nY+=nStrHeight;
	
	for(nCurId=0; nCurId<pCurveWidget->nNext; nCurId++)
	{
		pCurve=pCurveWidget->pCurves[nCurId];
		gdk_cairo_set_source_color(cr, &pCurve->color);
		
		sprintf(str, "%s:", pCurve->strText);
		pango_layout_set_text(pPangoLayout, str, -1);
		pango_layout_get_pixel_size(pPangoLayout,NULL, &nStrHeight);
		gtk_curve_widget_draw_pango_layout(cr, NULL, nX, nY, pPangoLayout);
		
		nIndex=nDataIndex[nCurId];
		if (nIndex >= 0)
		{
			if (pCurve->nDataType == eDataInt)
			{
				sprintf(str, "%d", pCurve->pYAxis[nIndex]);
			}
			else if (pCurve->nDataType == eDataFloat)
			{
				sprintf(str, "%.3f", pCurve->pYAxis[nIndex]);
			}
			
			pango_layout_set_text(pPangoLayout, str, -1);
			gtk_curve_widget_draw_pango_layout(cr, NULL, nX+nWidth, nY, pPangoLayout);
		}
		
		nY+=nStrHeight;
	}

	cairo_restore(cr);
}

gboolean gtk_curve_widget_canvas_btn_press_event(GtkWidget *pWidget, GdkEventButton *pEvent, gpointer usr_data)
{
	GtkCurveWidget* pCurveWidget;
	if ((pEvent->button == 1))
	{
		pCurveWidget=(GtkCurveWidget*)usr_data;
		G_ASSERT_NULL(pCurveWidget);

		pCurveWidget->bPressed=TRUE;

		if (pCurveWidget->bEnable[eEnableMessage] && pCurveWidget->bEnable[eEnableMessageClose])
		{
			pCurveWidget->dPtEndX=pCurveWidget->dPtStartX=pEvent->x;
			pCurveWidget->dPtEndY=pCurveWidget->dPtStartY=pEvent->y;
		}
		
		if (pCurveWidget->bEnable[eEnableScale])
		{
			if (pCurveWidget->nCurveOpType == eCurveOpHand || pCurveWidget->nCurveOpType == eCurveOpZoomRect)
			{
				pCurveWidget->dPtEndX=pCurveWidget->dPtStartX=pEvent->x;
				pCurveWidget->dPtEndY=pCurveWidget->dPtStartY=pEvent->y;
			}
		}

		gtk_curve_widget_canvas_redraw(pCurveWidget);
		
		return TRUE;
     }

	return FALSE;
}

gboolean gtk_curve_widget_canvas_btn_release_event(GtkWidget *pWidget, GdkEventButton *pEvent, gpointer usr_data)
{
	GtkCurveWidget* pCurveWidget;
	pCurveWidget=(GtkCurveWidget*)usr_data;
	G_ASSERT_NULL(pCurveWidget);

	if ((pEvent->button == 1))
	{
		pCurveWidget->bPressed=FALSE;

		if (pCurveWidget->bEnable[eEnableMessage] && pCurveWidget->bEnable[eEnableMessageClose])
		{
			if (pCurveWidget->nCloseBtnStatue == eCloseBtnAbove && 
				gtk_rectangle_contains(pCurveWidget->rectMessageClose, pCurveWidget->dPtStartX, pCurveWidget->dPtStartY))
			{
				gtk_curve_widget_enable(pCurveWidget, FALSE, eEnableMessage);
			}
		}
		
		if (pCurveWidget->bEnable[eEnableScale])
		{
			if (pCurveWidget->nCurveOpType == eCurveOpZoomIn)		//放大
			{
				gtk_curve_widget_canvas_zoom_in(pCurveWidget,pEvent->x, pEvent->y);
			}
			else if (pCurveWidget->nCurveOpType == eCurveOpZoomOut)		//缩小
			{
				gtk_curve_widget_canvas_zoom_out(pCurveWidget,pEvent->x, pEvent->y);
			}
			else if (pCurveWidget->nCurveOpType == eCurveOpZoomRect)
			{
				gtk_curve_widget_canvas_zoom_rect(pCurveWidget);
			}
		}

		gtk_curve_widget_frame_redraw(pCurveWidget);

		return TRUE;
	}

	return FALSE;
}

gboolean gtk_curve_widget_canvas_motion_notify_event(GtkWidget *pWidget, GdkEventButton *pEvent, gpointer usr_data)
{
	gdouble x, y;

	GtkCurveWidget* pCurveWidget;
	pCurveWidget=(GtkCurveWidget*)usr_data;
	G_ASSERT_NULL(pCurveWidget);

	if (pCurveWidget->bEnable[eEnableMessage] && pCurveWidget->bEnable[eEnableMessageClose])
	{
		if (gtk_rectangle_contains(pCurveWidget->rectMessageClose, pEvent->x, pEvent->y))
		{
			pCurveWidget->nCloseBtnStatue=eCloseBtnAbove;
		}
		else
		{
			pCurveWidget->nCloseBtnStatue=eCloseBtnNormal;
		}

		if (!pCurveWidget->bPressed)
		{
			gtk_curve_widget_canvas_redraw(pCurveWidget);
			return TRUE;
		}
	}

	if (pCurveWidget->bPressed)
	{
		if (pCurveWidget->nCurveOpType == eCurveOpHand)
		{
			gtk_curve_widget_canvas_hand_move(pCurveWidget, pEvent->x, pEvent->y);
		}
		else if (pCurveWidget->nCurveOpType == eCurveOpZoomRect)
		{
			x=pEvent->x;
			x=x>0? x:0;
			x=x<pCurveWidget->rectCanvas.width? x:pCurveWidget->rectCanvas.width;
			
			y=pEvent->y;
			y=y>0? y:0;
			y=y<pCurveWidget->rectCanvas.height? y:pCurveWidget->rectCanvas.height;
			
			pCurveWidget->dPtEndX=x;
			pCurveWidget->dPtEndY=y;
		}

		gtk_curve_widget_frame_redraw(pCurveWidget);

		return TRUE;
	}

	return FALSE;
}

gboolean gtk_curve_widget_canvas_key_release_event(GtkWidget *pWidget,GdkEventKey *pEvent, gpointer usr_data)
{
	GtkCurveWidget* pCurveWidget=(GtkCurveWidget*)usr_data;
	G_ASSERT_NULL(pCurveWidget);

	if (pCurveWidget->bEnable[eEnableScale])
	{
		switch (pEvent->keyval)
		{
		case GDK_Page_Up:
			{
				gtk_curve_widget_canvas_zoom_in(pCurveWidget, pCurveWidget->rectCanvas.width/2, 
					pCurveWidget->rectCanvas.height/2);
			}
			break;
		case GDK_Page_Down:
			{
				gtk_curve_widget_canvas_zoom_out(pCurveWidget, pCurveWidget->rectCanvas.width/2, 
				pCurveWidget->rectCanvas.height/2);
			}
			break;
		case GDK_Home:
			{
				gtk_curve_widget_canvas_zoom_100(pCurveWidget);
			}
			break;
		default:
			return FALSE;
		}

		gtk_curve_widget_frame_redraw(pCurveWidget);
		return TRUE;
	}

	return FALSE;
}

void gtk_curve_widget_set_font_desc(GtkCurveWidget* pCurveWidget, PangoFontDescription* pFontDesc, gint nFontDescType)
{
	G_ASSERT_NULL(pCurveWidget);
	G_ASSERT_NULL(pFontDesc);
	g_assert(nFontDescType < eFontDescEnd);

	if (pFontDesc != NULL)
	{
		pango_font_description_free(pCurveWidget->pFontDesc[nFontDescType]);
		pCurveWidget->pFontDesc[nFontDescType]=NULL;
	}
	
	pCurveWidget->pFontDesc[nFontDescType]=pFontDesc;
}

void gtk_curve_widget_enable(GtkCurveWidget* pCurveWidget, gboolean bEnable, gint nType)
{
	G_ASSERT_NULL(pCurveWidget);
	g_assert(nType < eEnableEnd);

	pCurveWidget->bEnable[nType]=bEnable;
}

void gtk_curve_widget_set_color(GtkCurveWidget* pCurveWidget, GdkColor* color, gint nType)
{
	G_ASSERT_NULL(pCurveWidget);
	g_assert(nType < eColorEnd);

	pCurveWidget->color[nType]=*color;
}

void gtk_curve_widget_set_rgb_color(GtkCurveWidget* pCurveWidget, guint8 r, guint8 g, guint8 b, gint nType)
{
	GdkColor color=rgb2gdk(r, g, b);

	gtk_curve_widget_set_color(pCurveWidget, &color, nType);
}

gint gtk_curve_widget_transform(gdouble dAxis, gdouble dMin, gdouble dMax, gint nLength, gboolean bRev)
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

gdouble gtk_curve_widget_invTransform(gdouble dPos, gdouble dMin, gdouble dMax, gdouble dLength, gboolean bInv)
{
	if (bInv)
	{
		return dMin+(dLength-dPos)*(dMax-dMin)/dLength;
	}
	else
	{
		return dMin+dPos*(dMax-dMin)/dLength;
	}
}

gdouble gtk_curve_widget_scale_value(gdouble dScale, gdouble dMin, gdouble dMax)
{
	return dMin+(dMax-dMin)*dScale;
}

void gtk_curve_widget_set_current_operation(GtkCurveWidget* pCurveWidget, gint nCurveOp)
{
	G_ASSERT_NULL(pCurveWidget);
	g_assert(nCurveOp < eCurveOpEnd);

	pCurveWidget->nCurveOpType=pCurveWidget->bEnable[eEnableScale]? nCurveOp:eCurveOpPointer;

//#ifdef WINDOWS_OS
	if (pCurveWidget->pCanvas->window != NULL || pCurveWidget->pCursor[nCurveOp] != NULL)
	{
		gdk_window_set_cursor(pCurveWidget->pCanvas->window, pCurveWidget->pCursor[nCurveOp]);
	}
//#endif
}

gint gtk_curve_widget_get_current_operation(GtkCurveWidget* pCurveWidget)
{
	G_ASSERT_NULL(pCurveWidget);

	return pCurveWidget->nCurveOpType;
}

void gtk_curve_widget_canvas_hand_move(GtkCurveWidget* pCurveWidget, gdouble x, gdouble y)
{
	gdouble dx,dy,dMin,dMax;

	G_ASSERT_NULL(pCurveWidget);

	dx=(x-pCurveWidget->dPtStartX)*(pCurveWidget->dScaleRight-pCurveWidget->dScaleLeft)/pCurveWidget->rectCanvas.width;
	dy=(pCurveWidget->dPtStartY-y)*(pCurveWidget->dScaleTop-pCurveWidget->dScaleBottom)/pCurveWidget->rectCanvas.height;
	
	dMin=pCurveWidget->dScaleLeft-dx;
	dMax=pCurveWidget->dScaleRight-dx;
	
	if (dMin < 0.0)
	{
		dMax-=dMin;
		dMin=0.0;
	}
	else if (dMax > 1.0)
	{
		dMin+=1.0-dMax;
		dMax=1.0;
	}
	
	pCurveWidget->dScaleLeft=dMin;
	pCurveWidget->dScaleRight=dMax;
	
	dMin=pCurveWidget->dScaleBottom-dy;
	dMax=pCurveWidget->dScaleTop-dy;
	
	if (dMin < 0.0)
	{
		dMax-=dMin;
		dMin=0.0;
	}
	else if (dMax > 1.0)
	{
		dMin+=1.0-dMax;
		dMax=1.0;
	}
	
	pCurveWidget->dScaleBottom=dMin;
	pCurveWidget->dScaleTop=dMax;
	
	pCurveWidget->dPtStartX=x;
	pCurveWidget->dPtStartY=y;
}

void gtk_curve_widget_canvas_zoom_in(GtkCurveWidget* pCurveWidget, gdouble x, gdouble y)
{
	gdouble dx,dy,dMin,dMax;
	gdouble dTemp;

	dx=gtk_curve_widget_invTransform(x, pCurveWidget->dScaleLeft, pCurveWidget->dScaleRight, 
		pCurveWidget->rectCanvas.width, FALSE);
	
	dMin=dx-(dx-pCurveWidget->dScaleLeft)*(1.0-pCurveWidget->dScaleRate);
	dMax=dx+(pCurveWidget->dScaleRight-dx)*(1.0-pCurveWidget->dScaleRate);
				
	dTemp=COLUMN_MAJOR_MIN/(pCurveWidget->dTimeMax-pCurveWidget->dTimeMin);
	if (dMax-dMin > dTemp)
	{
		pCurveWidget->dScaleLeft=dMin;
		pCurveWidget->dScaleRight=dMax;
	}
				
	dy=gtk_curve_widget_invTransform(y, pCurveWidget->dScaleBottom,pCurveWidget->dScaleTop,
		pCurveWidget->rectCanvas.height, TRUE);
	dMin=dy-(dy-pCurveWidget->dScaleBottom)*(1.0-pCurveWidget->dScaleRate);
	dMax=dy+(pCurveWidget->dScaleTop-dy)*(1.0-pCurveWidget->dScaleRate);
			
	if (pCurveWidget->nCalc < eCalcManual)
	{
		dTemp=pCurveWidget->bEnable[eEnableNegativeScale]? 200.0:100.0;
	}
	else
	{
		dTemp=pCurveWidget->dYAxisMax-pCurveWidget->dYAixsMin;
	}
	if (dMax-dMin > ROW_MAJOR_MIN/dTemp)
	{
		pCurveWidget->dScaleTop=dMax;
		pCurveWidget->dScaleBottom=dMin;
	}
}

void gtk_curve_widget_canvas_zoom_out(GtkCurveWidget* pCurveWidget, gdouble x, gdouble y)
{
	gdouble dx,dy,dMin,dMax;

	dx=gtk_curve_widget_invTransform(x, pCurveWidget->dScaleLeft, pCurveWidget->dScaleRight,
		pCurveWidget->rectCanvas.width, FALSE);
				
	dMin=dx-(dx-pCurveWidget->dScaleLeft)*(1.0+pCurveWidget->dScaleRate);
	dMax=dx+(pCurveWidget->dScaleRight-dx)*(1.0+pCurveWidget->dScaleRate);
				
	dMin=dMin>0.0? dMin:0.0;
	dMax=dMax<1.0? dMax:1.0;
				
	pCurveWidget->dScaleLeft=dMin;
	pCurveWidget->dScaleRight=dMax;
				
	dy=gtk_curve_widget_invTransform(y, pCurveWidget->dScaleBottom, pCurveWidget->dScaleTop,
		pCurveWidget->rectCanvas.height, TRUE);

	dMin=dy-(dy-pCurveWidget->dScaleBottom)*(1.0+pCurveWidget->dScaleRate);
	dMax=dy+(pCurveWidget->dScaleTop-dy)*(1.0+pCurveWidget->dScaleRate);
				
	dMin=dMin>0.0? dMin:0.0;
	dMax=dMax<1.0? dMax:1.0;
				
	pCurveWidget->dScaleTop=dMax;
	pCurveWidget->dScaleBottom=dMin;
}

void gtk_curve_widget_canvas_set_zoom_rect(GtkCurveWidget* pCurveWidget, double left, double right)
{
	gdouble dLeft,dRight;
	gdouble dMin,dMax;
	gdouble dTemp;

	if (left < right)
	{
		dMin=left;
		dMax=right;
	}
	else
	{
		dMin=right;
		dMax=left;
	}

	dLeft=gtk_curve_widget_invTransform(dMin-pCurveWidget->dTimeMin, pCurveWidget->dScaleLeft, pCurveWidget->dScaleRight, 
		pCurveWidget->dTimeMax-pCurveWidget->dTimeMin, FALSE);
	dRight=gtk_curve_widget_invTransform(dMax-pCurveWidget->dTimeMin, pCurveWidget->dScaleLeft, pCurveWidget->dScaleRight, 
		pCurveWidget->dTimeMax-pCurveWidget->dTimeMin, FALSE);

	
	dTemp=COLUMN_MAJOR_MIN/(pCurveWidget->dTimeMax-pCurveWidget->dTimeMin);

	if (dRight-dLeft > dTemp)
	{
		pCurveWidget->dScaleLeft=dLeft;
		pCurveWidget->dScaleRight=dRight;
	}
}

void gtk_curve_widget_canvas_zoom_rect(GtkCurveWidget* pCurveWidget)
{
	gdouble dLeft,dRight,dTop,dBottom;
	gdouble dMin,dMax;
	gdouble dTemp;

	if (pCurveWidget->dPtStartX < pCurveWidget->dPtEndX)
	{
		dMin=pCurveWidget->dPtStartX;
		dMax=pCurveWidget->dPtEndX;
	}
	else
	{
		dMin=pCurveWidget->dPtEndX;
		dMax=pCurveWidget->dPtStartX;
	}

	dLeft=gtk_curve_widget_invTransform(dMin, pCurveWidget->dScaleLeft, pCurveWidget->dScaleRight, 
		pCurveWidget->rectCanvas.width, FALSE);
	dRight=gtk_curve_widget_invTransform(dMax, pCurveWidget->dScaleLeft, pCurveWidget->dScaleRight, 
		pCurveWidget->rectCanvas.width, FALSE);

	dTemp=COLUMN_MAJOR_MIN/(pCurveWidget->dTimeMax-pCurveWidget->dTimeMin);

	if (dRight-dLeft > dTemp)
	{
		pCurveWidget->dScaleLeft=dLeft;
		pCurveWidget->dScaleRight=dRight;
	}

	if (pCurveWidget->dPtStartY < pCurveWidget->dPtEndY)
	{
		dMin=pCurveWidget->dPtStartY;
		dMax=pCurveWidget->dPtEndY;
	}
	else
	{
		dMin=pCurveWidget->dPtEndY;
		dMax=pCurveWidget->dPtStartY;
	}
	
	dTop=gtk_curve_widget_invTransform(dMin, pCurveWidget->dScaleBottom, pCurveWidget->dScaleTop, 
		pCurveWidget->rectCanvas.height, TRUE);
	dBottom=gtk_curve_widget_invTransform(dMax, pCurveWidget->dScaleBottom, pCurveWidget->dScaleTop, 
		pCurveWidget->rectCanvas.height, TRUE);

	if (pCurveWidget->nCalc < eCalcManual)
	{
		dTemp=pCurveWidget->bEnable[eEnableNegativeScale]? 200.0:100.0;
	}
	else
	{
		dTemp=pCurveWidget->dYAxisMax-pCurveWidget->dYAixsMin;
	}

	if (dTop-dBottom > ROW_MAJOR_MIN/dTemp)
	{
		pCurveWidget->dScaleBottom=dBottom;
		pCurveWidget->dScaleTop=dTop;
	}
}

void gtk_curve_widget_canvas_zoom_100(GtkCurveWidget* pCurveWidget)
{
	G_ASSERT_NULL(pCurveWidget);

	pCurveWidget->dScaleLeft=pCurveWidget->dScaleBottom=0.0;
	pCurveWidget->dScaleRight=pCurveWidget->dScaleTop=1.0;
}

void gtk_curve_widget_set_scale_rate(GtkCurveWidget* pCurveWidget, gdouble dRate)
{
	G_ASSERT_NULL(pCurveWidget);
	g_assert(dRate > 0.0 || dRate < 1.0);

	pCurveWidget->dScaleRate=dRate;
}

void gtk_curve_widget_frame_redraw(GtkCurveWidget* pCurveWidget)
{
	G_ASSERT_NULL(pCurveWidget);

	if (pCurveWidget->pFrame->window != NULL)
	{
		gtk_widget_queue_resize(pCurveWidget->pFrame);
	}
}

void gtk_curve_widget_canvas_redraw(GtkCurveWidget* pCurveWidget)
{
	G_ASSERT_NULL(pCurveWidget);

	if (pCurveWidget->pCanvas->window != NULL)
	{
		gtk_widget_queue_draw(pCurveWidget->pCanvas);
	}
}

void gtk_curve_widget_create_cursor(GtkCurveWidget* pCurveWidget)
{
	G_ASSERT_NULL(pCurveWidget);

	gtk_curve_widget_ref_cursor_pixbuf();

	pCurveWidget->pCursor[eCurveOpPointer]=create_cursor(pCurveWidget, g_pPixbufCursor[eCurveOpPointer], TRUE);
	pCurveWidget->pCursor[eCurveOpHand]=create_cursor(pCurveWidget, g_pPixbufCursor[eCurveOpHand], TRUE);
	pCurveWidget->pCursor[eCurveOpZoomIn]=create_cursor(pCurveWidget, g_pPixbufCursor[eCurveOpZoomIn], TRUE);
	pCurveWidget->pCursor[eCurveOpZoomOut]=create_cursor(pCurveWidget, g_pPixbufCursor[eCurveOpZoomOut], TRUE);
	pCurveWidget->pCursor[eCurveOpZoomRect]=create_cursor(pCurveWidget, g_pPixbufCursor[eCurveOpZoomRect], TRUE);
}

void gtk_curve_widget_destroy_cursor(GtkCurveWidget* pCurveWidget)
{
	gint nIndex;

	G_ASSERT_NULL(pCurveWidget);
	
	for (nIndex=0; nIndex < eCurveOpEnd; nIndex++)
	{
		if (pCurveWidget->pCursor[nIndex] != NULL)
		{
			gdk_cursor_unref(pCurveWidget->pCursor[nIndex]);
		}
	}

	gtk_curve_widget_unref_cursor_pixbuf();
}

void gtk_curve_widget_ref_cursor_pixbuf()
{
	g_nCursorRef++;
	if (g_nCursorRef == 1)
	{
		g_pPixbufCursor[eCurveOpPointer]=create_pixbuf("pointer.png");
		g_pPixbufCursor[eCurveOpHand]=create_pixbuf("hand.png");
		g_pPixbufCursor[eCurveOpZoomIn]=create_pixbuf("zoomin1.png");
		g_pPixbufCursor[eCurveOpZoomOut]=create_pixbuf("zoomout1.png");
		g_pPixbufCursor[eCurveOpZoomRect]=create_pixbuf("pointer2.png");
	}
}

void gtk_curve_widget_unref_cursor_pixbuf()
{
	gint nIndex;
	GdkPixbuf* pPixbufCursor[eCurveOpEnd];
	
	memcpy(pPixbufCursor, g_pPixbufCursor, sizeof(GdkPixbuf*)*eCurveOpEnd);

	g_nCursorRef--;
	
	g_nCursorRef=g_nCursorRef>0? g_nCursorRef:0;
	if (g_nCursorRef == 0)
	{
		for (nIndex=0; nIndex < eCurveOpEnd; nIndex++)
		{
			if (pPixbufCursor[nIndex] != NULL)
			{
				gdk_pixbuf_unref(pPixbufCursor[nIndex]);
			}
		}
	}
}

GdkCursor* create_cursor(GtkCurveWidget* pCurveWidget, GdkPixbuf* pPixBuf, gboolean bCenter)
{
	GdkCursor* pCursor=NULL;
	gint x=0,y=0;

	G_ASSERT_NULL(pCurveWidget->pCanvas);
	
	if (pPixBuf == NULL)
	{
		return gdk_cursor_new(GDK_CROSSHAIR);
	}
	
	if (bCenter)
	{
		x=gdk_pixbuf_get_width(pPixBuf)/2;
		y=gdk_pixbuf_get_height(pPixBuf)/2;
	}
	
	pCursor=gdk_cursor_new_from_pixbuf(gtk_widget_get_display(pCurveWidget->pCanvas), pPixBuf, x, y);
	
	return pCursor;
}

void gtk_curve_widget_ref_pixbuf()
{
	if (g_nPixbufRef <= 0)
	{
		g_pPixbuf[ePixbufMessage]=create_pixbuf("border.png");
		g_pPixbuf[ePixbufMessageClose]=create_pixbuf("close.png");
	}

	g_nPixbufRef++;
}

void gtk_curve_widget_unref_pixbuf()
{
	gint nIndex;
	
	g_nPixbufRef--;
	
	g_nPixbufRef=g_nPixbufRef>0? g_nPixbufRef:0;
	if (g_nPixbufRef == 0)
	{
		for (nIndex=0; nIndex < ePixbufEnd; nIndex++)
		{
			if (g_pPixbuf[nIndex] != NULL)
			{
				gdk_pixbuf_unref(g_pPixbuf[nIndex]);
				g_pPixbuf[nIndex]=NULL;
			}
		}
	}
}

gint gtk_curve_widget_add_curve(GtkCurveWidget* pCurveWidget, GCurve* pCurve)
{
	G_ASSERT_NULL(pCurveWidget);

	if (pCurve == NULL)
	{
		return -1;
	}

	if (pCurveWidget->nNext >= MAX_NUM_CURVES)
	{
		return -1;
	}

	pCurveWidget->pCurves[pCurveWidget->nNext++]=pCurve;

	return pCurveWidget->nNext-1;
}

gboolean gtk_curve_widget_remove_curve(GtkCurveWidget* pCurveWidget, gint nIndex)
{
	gint i;

	G_ASSERT_NULL(pCurveWidget);

	if (nIndex<0 || nIndex >=pCurveWidget->nNext)
	{
		return FALSE;
	}

	gtk_curve_destroy(pCurveWidget->pCurves[nIndex]);

	for (i=nIndex; i<pCurveWidget->nNext-1;i++)
	{
		pCurveWidget->pCurves[i]=pCurveWidget->pCurves[i+1];
	}

	pCurveWidget->nNext--;

	return TRUE;
}

void gtk_curve_widget_clear_all_curve(GtkCurveWidget* pCurveWidget)
{
	gint nIndex;

	G_ASSERT_NULL(pCurveWidget);

	for (nIndex=0; nIndex<pCurveWidget->nNext; nIndex++)
	{
		if (pCurveWidget->pCurves[nIndex] != NULL)
		{
			gtk_curve_destroy(pCurveWidget->pCurves[nIndex]);
		}
	}
	pCurveWidget->nNext=0;
}

gboolean gtk_curve_widget_check_add_curve(GtkCurveWidget* pCurveWidget)
{
	G_ASSERT_NULL(pCurveWidget);
	if (pCurveWidget->nNext < MAX_NUM_CURVES)
	{
		return TRUE;
	}

	printf("GtkCurveWidget: Max Curves = %d", MAX_NUM_CURVES);
	return FALSE;
}

gint gtk_curve_widget_get_curve_count(GtkCurveWidget* pCurveWidget)
{
	G_ASSERT_NULL(pCurveWidget);
	
	return pCurveWidget->nNext;
}

GCurve* gtk_curve_widget_get_curve(GtkCurveWidget* pCurveWidget, gint nIndex)
{
	G_ASSERT_NULL(pCurveWidget);

	if (nIndex<0 || nIndex>=gtk_curve_widget_get_curve_count(pCurveWidget))
	{
		return NULL;
	}

	return pCurveWidget->pCurves[nIndex];
}

void gtk_curve_widget_set_time_scale(GtkCurveWidget* pCurveWidget, gdouble dMin, gdouble dMax)
{
	G_ASSERT_NULL(pCurveWidget);

	pCurveWidget->dTimeMin=dMin;
	pCurveWidget->dTimeMax=dMax;
}

void gtk_curve_widget_set_data_scale(GtkCurveWidget* pCurveWidget, gint nCalcType, gdouble dMin, gdouble dMax)
{
	G_ASSERT_NULL(pCurveWidget);

	pCurveWidget->nCalc=nCalcType;

	if (nCalcType==eCalcScaleManual || nCalcType == eCalcManual)
	{	
		pCurveWidget->dYAixsMin=dMin;
		pCurveWidget->dYAxisMax=dMax;
	}
}

void gtk_curve_widget_set_sample_pixel_interval(GtkCurveWidget* pCurveWidget, gint nNumPixel)
{
	G_ASSERT_NULL(pCurveWidget);

	nNumPixel=nNumPixel>1? nNumPixel:1;
	pCurveWidget->nSamplePixel=nNumPixel;
}

gint gtk_curve_widget_sample(gdouble dMin, gdouble dMax, gint nWidth, gint nPixelInterval)
{
	gint nInterval;
	
	nInterval=(gint)ceil((dMax-dMin)*nPixelInterval/nWidth);
	
	nInterval=nInterval>1? nInterval:1;
	
	return nInterval;
}

void adjust_data_scale(gdouble* dMin, gdouble* dMax)
{
	*dMin=floor((*dMin-0.001)/10)*10.0;
	*dMax=ceil((*dMax+0.001)/10)*10.0;
	
	if (*dMax-*dMin < 10.0)
	{
		*dMax=*dMin+10.0;
	}
}

gint binary_find(gdouble* pData, gint nLenBuf, gint nOffset, gint nNumPt, gdouble dValue)
{
	gint nStart, nEnd, nMid;

	G_ASSERT_NULL(pData);

	if (nLenBuf <= 0 || nNumPt<= 0)
	{
		return -1;
	}
	
	nStart=nOffset;
	nEnd=nOffset+nNumPt;
	nMid=(nEnd+nStart)>>1;
	while(nStart <= nEnd)
	{
		if (pData[nMid%nLenBuf] == dValue)
		{
			return nMid;
		}
		else if (pData[nMid%nLenBuf] < dValue)
		{
			nStart=nMid+1;
		}
		else
		{
			nEnd=nMid-1;
		}
		
		nMid=(nEnd+nStart)>>1;
	}
	
	return nStart%nLenBuf;
}

gint step_find(gint nLenBuf, gint nOffset, gint nNumPt, gdouble dStart, gdouble dStep, gdouble dValue, gboolean bNear)
{
	gint nIndex;

	if (nLenBuf<=0 || nNumPt<=0 || dStart > dValue)
	{
		return -1;
	}

	nIndex=(gint)((dValue-dStart)/dStep+ (bNear? 0.5:0));

	if (nIndex > nNumPt)
	{
		return -1;
	}

	nIndex=(nIndex+nOffset)%nLenBuf;

	return nIndex;
}

GdkColor rgb2gdk(guint8 r, guint8 g, guint8 b)
{
	GdkColor color;
	color.red=(r<<8);
	color.green=(g<<8);
	color.blue=(b<<8);
	color.pixel=(r<<16)+(g<<8)+b;

	return color;
}

gboolean gtk_rectangle_contains(GdkRectangle rect, gdouble x, gdouble y)
{
	return (x >= rect.x && x<=rect.x+rect.width && y >= rect.y && y <= rect.y+rect.height);
}

GtkColorStack* gtk_color_stack_create(GdkColor** pColors, gint nNum)
{
	int nIndex;
	
	GtkColorStack* pStackColor;
	G_ASSERT_NULL(pColors);

	g_assert(nNum>0);

	pStackColor=(GtkColorStack*)g_new(GtkColorStack, 1);
	
	pStackColor->colorUsed.color=(GdkColor*)g_new(GdkColor, nNum);
	pStackColor->colorUsed.nPos=0;
	
	pStackColor->colorUnused.color=(GdkColor*)g_new(GdkColor, nNum);
	pStackColor->colorUnused.nPos=nNum-1;
	for (nIndex=0; nIndex<nNum; nIndex++)
	{
		pStackColor->colorUnused.color[nIndex]=*pColors[nNum-nIndex-1];
	}

	return pStackColor;
}

void gtk_color_stack_destroy(GtkColorStack* pStack)
{
	if (pStack == NULL)
	{
		return;
	}

	G_SAFE_FREE(pStack->colorUsed.color);
	G_SAFE_FREE(pStack->colorUnused.color);
	G_SAFE_FREE(pStack);
}

void gtk_color_stack_reset(GtkColorStack* pStack)
{
	G_ASSERT_NULL(pStack);

	while (pStack->colorUsed.nPos > 0)
	{
		gtk_color_stack_push_color(pStack, &pStack->colorUsed.color[pStack->colorUsed.nPos-1]);
	}
}

GdkColor gtk_color_stack_pop_color(GtkColorStack* pStack)
{
	GdkColor colorTemp;

	G_ASSERT_NULL(pStack);
	
	if (pStack->colorUnused.nPos<0)
	{
		return rgb2gdk(255, 0, 0);
	}
	
	colorTemp=pStack->colorUnused.color[pStack->colorUnused.nPos];
	pStack->colorUnused.nPos--;
	
	pStack->colorUsed.color[pStack->colorUsed.nPos]=colorTemp;
	pStack->colorUsed.nPos++;
	
	return colorTemp;
}

void gtk_color_stack_push_color(GtkColorStack* pStack, GdkColor* color)
{
	gint nIndex;
	GdkColor* pColorTemp;
	
	G_ASSERT_NULL(pStack);
	G_ASSERT_NULL(color);

	if (pStack->colorUsed.nPos<=0)
	{
		return;
	}
	
	for (nIndex=0; nIndex<pStack->colorUsed.nPos; nIndex++)
	{
		pColorTemp=&pStack->colorUsed.color[nIndex];
		if (color->red == pColorTemp->red && color->green == pColorTemp->green && color->blue == pColorTemp->blue)
		{
			break;
		}
	}
	
	if (nIndex < pStack->colorUsed.nPos)
	{
		for (;nIndex<pStack->colorUsed.nPos-1; nIndex++)
		{
			pStack->colorUsed.color[nIndex]=pStack->colorUsed.color[nIndex+1];
		}
		pStack->colorUsed.nPos--;
		
		pStack->colorUnused.nPos++;
		pStack->colorUnused.color[pStack->colorUnused.nPos]=*color;
	}
}

void gtk_curve_widget_tool_btn_manager(GtkCurveWidget* curvewidget, gint nToolType)
{
	gint nType;
	
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(curvewidget->pToolBtn[nToolType])))
	{
		if (nToolType != curvewidget->nToolType)
		{
			nType=curvewidget->nToolType;
			curvewidget->nToolType=nToolType;
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(curvewidget->pToolBtn[nType]), FALSE);
		}
	}
	else
	{
		if (nToolType == curvewidget->nToolType)
		{
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(curvewidget->pToolBtn[nToolType]), TRUE);
		}
	}
	
	gtk_curve_widget_set_current_operation(curvewidget, nToolType);
}

void gtk_curve_widget_toggled_btn_pointer(GtkWidget *widget, gpointer data)
{
	GtkCurveWidget* curvewidget=(GtkCurveWidget*)data;

	gtk_curve_widget_tool_btn_manager(curvewidget, eCurveOpPointer);
}

void gtk_curve_widget_toggled_btn_hand(GtkWidget *widget, gpointer data)
{
	GtkCurveWidget* curvewidget=(GtkCurveWidget*)data;
	
	gtk_curve_widget_tool_btn_manager(curvewidget, eCurveOpHand);
}

void gtk_curve_widget_toggled_btn_zoom_in(GtkWidget *widget, gpointer data)
{
	GtkCurveWidget* curvewidget=(GtkCurveWidget*)data;
	
	gtk_curve_widget_tool_btn_manager(curvewidget, eCurveOpZoomIn);
}

void gtk_curve_widget_toggled_btn_zoom_out(GtkWidget *widget, gpointer data)
{
	GtkCurveWidget* curvewidget=(GtkCurveWidget*)data;
	
	gtk_curve_widget_tool_btn_manager(curvewidget, eCurveOpZoomOut);
}

void gtk_curve_widget_toggled_btn_zoom_rect(GtkWidget *widget, gpointer data)
{
	GtkCurveWidget* curvewidget=(GtkCurveWidget*)data;
	
	gtk_curve_widget_tool_btn_manager(curvewidget, eCurveOpZoomRect);
}

void gtk_curve_widget_toggled_btn_zoom_100(GtkWidget *widget, gpointer data)
{
	GtkCurveWidget* curvewidget=(GtkCurveWidget*)data;
	G_ASSERT_NULL(curvewidget);
	
	gtk_curve_widget_canvas_zoom_100(curvewidget);
	
	gtk_curve_widget_frame_redraw(curvewidget);
}

void gtk_curve_widget_add_scale_toolbox(GtkCurveWidget* curvewidget, GtkWidget* box, GtkWidget * parent)
{
	GtkWidget *pBtnPointer,*pBtnHand,*pBtnZoomIn,*pBtnZoomOut,*pBtnZoomRect,*pBtnZoom100;

#if !(GTK_CHECK_VERSION(2, 6, 0))
	gchar *ptr;
	gchar *pConvert=0;
#endif

	G_ASSERT_NULL(curvewidget);

	curvewidget->nToolType=eCurveOpPointer;

	pBtnPointer=gtk_toggle_button_new();
	curvewidget->pToolBtn[eCurveOpPointer]=pBtnPointer;
	
#if !(GTK_CHECK_VERSION(2, 6, 0))
	gtk_button_set_label(GTK_BUTTON(pBtnPointer), (pConvert=_toUtf8EX("OIX_CSTR_TRACK")));
	G_SAFE_FREE(pConvert);//dzy1017
#else
	gtk_button_set_image(GTK_BUTTON(pBtnPointer), create_pixmap (parent, "pointer1.png"));
#endif
	gtk_box_pack_start (GTK_BOX (box), pBtnPointer, FALSE, FALSE, 2); 
	gtk_widget_show (pBtnPointer);	
	g_signal_connect(G_OBJECT(pBtnPointer), "toggled", G_CALLBACK(gtk_curve_widget_toggled_btn_pointer), curvewidget);
	
	pBtnHand= gtk_toggle_button_new();
	curvewidget->pToolBtn[eCurveOpHand]=pBtnHand;
#if !(GTK_CHECK_VERSION(2, 6, 0))
	gtk_button_set_label(GTK_BUTTON(pBtnHand), (pConvert=_toUtf8EX("OIX_CSTR_HAND")));
	G_SAFE_FREE(pConvert);//dzy1017
#else
	gtk_button_set_image(GTK_BUTTON(pBtnHand), create_pixmap (parent, "hand.png"));
#endif
	
	gtk_box_pack_start (GTK_BOX (box), pBtnHand, FALSE, FALSE, 2); 
	gtk_widget_show (pBtnHand);	
	g_signal_connect(G_OBJECT(pBtnHand), "toggled", G_CALLBACK(gtk_curve_widget_toggled_btn_hand), curvewidget);
	
	pBtnZoomIn= gtk_toggle_button_new();
	curvewidget->pToolBtn[eCurveOpZoomIn]=pBtnZoomIn;
#if !(GTK_CHECK_VERSION(2, 6, 0))
	gtk_button_set_label(GTK_BUTTON(pBtnZoomIn), (pConvert=_toUtf8EX("OIX_CSTR_ZOOM_IN")));
	G_SAFE_FREE(pConvert);//dzy1017
#else
	gtk_button_set_image(GTK_BUTTON(pBtnZoomIn), create_pixmap (parent, "zoomin.png"));
#endif
	
	gtk_box_pack_start (GTK_BOX (box), pBtnZoomIn, FALSE, FALSE, 2); 
	gtk_widget_show (pBtnZoomIn);	
	g_signal_connect(G_OBJECT(pBtnZoomIn), "toggled", 
		G_CALLBACK(gtk_curve_widget_toggled_btn_zoom_in), curvewidget);
	
	pBtnZoomOut= gtk_toggle_button_new();
	curvewidget->pToolBtn[eCurveOpZoomOut]=pBtnZoomOut;
#if !(GTK_CHECK_VERSION(2, 6, 0))
	gtk_button_set_label(GTK_BUTTON(pBtnZoomOut), (pConvert=_toUtf8EX("OIX_CSTR_ZOOM_OUT")));
	G_SAFE_FREE(pConvert);//dzy1017
#else
	gtk_button_set_image(GTK_BUTTON(pBtnZoomOut), create_pixmap (parent, "zoomout.png"));
#endif
	
	gtk_box_pack_start (GTK_BOX (box), pBtnZoomOut, FALSE, FALSE, 2); 
	gtk_widget_show (pBtnZoomOut);	
	g_signal_connect(G_OBJECT(pBtnZoomOut), "toggled", 
		G_CALLBACK(gtk_curve_widget_toggled_btn_zoom_out), curvewidget);
	
	pBtnZoomRect= gtk_toggle_button_new();
	curvewidget->pToolBtn[eCurveOpZoomRect]=pBtnZoomRect;
#if !(GTK_CHECK_VERSION(2, 6, 0))
	gtk_button_set_label(GTK_BUTTON(pBtnZoomRect), (pConvert=_toUtf8EX("OIX_CSTR_ZOOM_RECT")));
	G_SAFE_FREE(pConvert);//dzy1017
#else
	gtk_button_set_image(GTK_BUTTON(pBtnZoomRect), create_pixmap (parent, "zoomrect.png"));
#endif
	
	gtk_box_pack_start (GTK_BOX (box), pBtnZoomRect, FALSE, FALSE, 2); 
	gtk_widget_show (pBtnZoomRect);	
	g_signal_connect(G_OBJECT(pBtnZoomRect), "toggled", 
		G_CALLBACK(gtk_curve_widget_toggled_btn_zoom_rect), curvewidget);
	
	pBtnZoom100= gtk_button_new();
#if !(GTK_CHECK_VERSION(2, 6, 0))
	gtk_button_set_label(GTK_BUTTON(pBtnZoom100), (pConvert=_toUtf8EX("OIX_CSTR_ZOOM_FIT")));
	G_SAFE_FREE(pConvert);//dzy1017
#else
	gtk_button_set_image(GTK_BUTTON(pBtnZoom100), create_pixmap (parent, "zoomfit.png"));
#endif
	
	gtk_box_pack_start (GTK_BOX (box), pBtnZoom100, FALSE, FALSE, 2); 
	gtk_widget_show (pBtnZoom100);	
	g_signal_connect(G_OBJECT(pBtnZoom100), "clicked", G_CALLBACK(gtk_curve_widget_toggled_btn_zoom_100), curvewidget);

	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(curvewidget->pToolBtn[curvewidget->nToolType]), TRUE);
}

