#ifndef GTK_CURVE_WIDGET
#define GTK_CURVE_WIDGET

#include <gtk/gtk.h>
#include <gdk/gdk.h>

#define G_ASSERT_NULL(p) g_assert(p != 0)

#define G_SAFE_FREE(p)	\
	if (p != 0)			\
	{					\
		g_free(p);		\
		p=0;			\
	}

#define COUNT_OF(p) sizeof(p)/sizeof(p[0])
#define M_PI	3.141592

#define MAX_STRING_LEN 256
#define MAX_USR_DATA	16

enum ECurveType		{eCurveLine=0, eCurveLadder, eCurveFill, eCurveDash, eCurveIndicator, eCurveTypeEnd};
enum ECurveTimeMode	{eModeNormal=0, eModeSec, eModeMin, eModeHour, eModeNormalAndNoSample, eModeEnd};
enum ECurveDataType	{eDataFloat, eDataInt, eDataTypeEnd};
enum ECurveLabelPos	
{
	eCurveNoLabel, 
	eCurveLabelStart, 
	eCurveLabelMid, 
	eCurveLabelEnd,
	eCurveLabelPosEnd
};

typedef struct
{
	gchar strText[MAX_STRING_LEN];

	GdkColor color;
	gint nCurveType;	//曲线显示类型
	gint nDataType;		//数字类型
	gint nLabelPos;		//曲线文字显示位置

	gint nTimeMode;		//pXAxis表示数据缓冲区还是起始时间
	gdouble dStep;		//当时间模式采用间隔模式时的时间间隔

	gint nNumPt;		//绘制顶点数，当其大于数据缓冲区时循环显示
	gint nLenBuf;		//数据缓冲区长度
	gint nOffset;		//初始点偏移量

	gdouble* pYAxis;	//数据缓冲区
	gdouble* pXAxis;	//数据缓冲区，或在间隔模式下的起始时间

	gdouble dMin, dMax;	//曲线上限和下限

	gpointer usrData[MAX_USR_DATA];	//用户数据
}GCurve;

GCurve* gtk_curve_create(gint nLenBuf, gint nTimeMode);
void gtk_curve_destroy(GCurve* pCurve);
void gtk_curve_change_buf_size(GCurve* pCurve, gint nLenBuf, gint nTimeMode);
void gtk_curve_set_text(GCurve* pCurve, gchar* strText);
void gtk_curve_set_label_pos(GCurve* pCurve, gint pos);
GdkColor gtk_curve_get_color(GCurve* pCurve);
void gtk_curve_set_color(GCurve* pCurve, GdkColor* color);
void gtk_curve_set_rgb_color(GCurve* pCurve, guint8 r, guint8 g, guint8 b);
void gtk_curve_set_int_color(GCurve* pCurve, int color);
void gtk_curve_set_type(GCurve* pCurve, gint nType);
void gtk_curve_set_data(GCurve* pCurve, gdouble* pXAxis, gdouble* pYAxis, gint nLenBuf, 
						gint nOffset, gint nNumPt, gdouble dStep);
void gtk_curve_set_boundary(GCurve* pCurve, gboolean bMenual, gdouble dMin, gdouble dMax);
gdouble gtk_curve_get_max_range(GCurve* pCurve);
gdouble gtk_curve_get_min_range(GCurve* pCurve);
void gtk_curve_set_data_type(GCurve* pCurve, gint nDataType);
void gtk_curve_set_data_offset(GCurve* pCurve, gint nOffset);
void gtk_curve_set_data_points_num(GCurve* pCurve, gint nNumPt);

void gtk_curve_set_user_data(GCurve* pCurve, gpointer usrData, gint nIndex);
gpointer gtk_curve_get_user_data(GCurve* pCurve, gint nIndex);

gboolean gtk_curve_get_sample_value(GCurve* pCurve, gdouble dSample, gdouble dRange, gdouble dStep, gint* nNumPt,
								gdouble* dAxis, gint* nOffset, gdouble* dValue);

enum EColorType 
{
	eColorTitle=0,			//标题
	eColorSubTitle,			//副标题
	eColorLabel,			//坐标轴名
	eColorAxisLabel,		//时间轴
	eColorAxis,				//坐标轴
	eColorScale,			//短标尺
	eColorMajor,			//主坐标轴标签
	eColorMinor,			//副坐标轴标签
	eColorPositive,			//数字正数标签
	eColorZero,				//数值0标签
	eColorNegative,			//数值负数标签
	eColorChooseRect,		//区域放大选择框
	eColorTrackLine,
	eColorTrackTime,
	eColorMessage,
	eColorCurvsMark,
	eColorEnd
};

enum ETimeType	{eTimeSec=0, eTimeMin, eTimeHour, eTimeDay, eTimeMon, eTimeYear, eTimeEnd};

enum EEnableStatus 
{
	eEnableAutoSubTitle=0,		//是否自动生成副标题	(默认关闭)
	eEnableScale,				//是否提供缩放功能	(默认关闭)
	eEnableTrack,				//是否提供Track提示功能	(默认开启)
	eEnableNegativeScale,		//是否显示负量程，仅对比例量程有效	(默认开启)
	eEnableKeepZero,			//量程是否从0开始，仅对数值量程有效	(默认开启)
	eEnableMessage,				//消息提示
	eEnableMessageClose,		//消息提示关闭按钮
	eEnableCurvesMark,			//绘制曲线标识
	eEnableAutoLabel,			//绘制坐标轴标签
	eEnableTimeAxis,			//X轴是否是时间轴(默认开启)
	eEnableEnd
};

enum EFontDescType 
{
	eFontDescTitle=0, 
	eFontDescSubTitle, 
	eFontDescAxis, 
	eFontDescTrack,
	eFontDescMessage,
	eFontDescCurvesMark,
	eFontDescEnd
};

enum ECurveOpType 
{
	eCurveOpPointer=0, 
	eCurveOpHand, 
	eCurveOpZoomIn, 
	eCurveOpZoomOut,
	eCurveOpZoomRect,
	eCurveOpEnd
};

enum ETextType 
{
	eTextTitle=0, 
	eTextSubtitle, 
	eTextLabel, 
	eTextMessage,
	eTextXAxisLabel,	//用于x轴为非时间轴模式时
	eTextEnd
};

enum ECalcType 
{
	eCalcScale=0,		//按比例显示，比例量程(-100.0～100.0或0～100.0)，各曲线自动生成量程(默认值)
	eCalcScaleAuto,		//按比例显示，比例量程(-100.0～100.0或0～100.0)，自动生成数值量程
	eCalcScaleManual,	//按比例显示，比例量程(-100.0～100.0或0～100.0)，需设置数值量程
	eCalcManual,		//按数值显示，需设置量程
	eCalcAuto,			//按数值显示，自动生成量程
	eCalcEnd
};

#define MAX_NUM_CURVES	64

enum ECloseBtnStatus {eCloseBtnNormal=0, eCloseBtnAbove, eCloseBtnPressed, eCloseBtnDisAbled};

typedef struct 
{
	gchar strText[eTextEnd][MAX_STRING_LEN];

	gboolean bEnable[eEnableEnd];

	gint nCurveOpType;

	GdkCursor* pCursor[eCurveOpEnd];		//鼠标形状

	GtkWidget	*pFrame;		//画板
//	GdkGC		*pFrameGc;		//画板gc

	GtkWidget	*pCanvas;		//画布
//	GdkGC		*pCanvasGc;		//画布gc

	GdkRectangle rectCanvas;

	PangoLayout* pPangoLayout;
	PangoFontDescription* pFontDesc[eFontDescEnd];

	gint nTimeType;
	gint nNumMajorCol;
	gdouble dTimeMin, dTimeMax, dTimeInterval;

	gdouble dYAixsMin, dYAxisMax, dYAxisInterval;

	gdouble dScaleTop, dScaleBottom;
	gdouble dScaleLeft, dScaleRight;

	gdouble dScaleRate;

	gboolean bPressed;
	gdouble dPtStartX, dPtStartY;
	gdouble dPtEndX, dPtEndY;

	GdkColor	color[eColorEnd];

	gint nSamplePixel;

	gint nCalc;
	gint nNext;
	GCurve* pCurves[MAX_NUM_CURVES];

	GdkRectangle rectMessage;
	GdkRectangle rectMessageClose;
	gint nCloseBtnStatue;

	GtkWidget* pToolBtn[eCurveOpEnd];
	int nToolType;
}GtkCurveWidget;

GtkWidget* gtk_curve_widget_create(GtkWidget* pTopWidget, GtkCurveWidget* pCurveWidget);

void gtk_curve_widget_set_text(GtkCurveWidget* pCurveWidget, gchar* str, gint nStrType);
void gtk_curve_widget_set_canvas_bg_color(GtkCurveWidget *pCurveWidget, GdkColor colorBg);
void gtk_curve_widget_set_canvas_bg_rgb_color(GtkCurveWidget* pCurveWidget, guint8 r, guint8 g, guint8 b);
void gtk_curve_widget_set_font_desc(GtkCurveWidget* pCurveWidget, PangoFontDescription* pFontDesc, gint nFontDescType);
void gtk_curve_widget_enable(GtkCurveWidget* pCurveWidget, gboolean bEnable, gint nType);
void gtk_curve_widget_set_color(GtkCurveWidget* pCurveWidget, GdkColor* color, gint nType);
void gtk_curve_widget_set_rgb_color(GtkCurveWidget* pCurveWidget, guint8 r, guint8 g, guint8 b, gint nType);
void gtk_curve_widget_set_current_operation(GtkCurveWidget* pCurveWidget, gint nCurveOp);
gint gtk_curve_widget_get_current_operation(GtkCurveWidget* pCurveWidget);
void gtk_curve_widget_set_scale_rate(GtkCurveWidget* pCurveWidget, gdouble dRate);
void gtk_curve_widget_frame_redraw(GtkCurveWidget* pCurveWidget);
void gtk_curve_widget_canvas_redraw(GtkCurveWidget* pCurveWidget);
void gtk_curve_widget_canvas_zoom_100(GtkCurveWidget* pCurveWidget);
GtkWidget* gtk_curve_widget_get_frame(GtkCurveWidget* pCurveWidget);

void gtk_curve_widget_add_scale_toolbox(GtkCurveWidget* curvewidget, GtkWidget* box, GtkWidget * parent);

void gtk_curve_widget_tool_btn_manager(GtkCurveWidget* curvewidget, gint nToolType);
void gtk_curve_widget_toggled_btn_pointer(GtkWidget *widget, gpointer data);
void gtk_curve_widget_toggled_btn_hand(GtkWidget *widget, gpointer data);
void gtk_curve_widget_toggled_btn_zoom_in(GtkWidget *widget, gpointer data);
void gtk_curve_widget_toggled_btn_zoom_out(GtkWidget *widget, gpointer data);
void gtk_curve_widget_toggled_btn_zoom_rect(GtkWidget *widget, gpointer data);
void gtk_curve_widget_toggled_btn_zoom_100(GtkWidget *widget, gpointer data);

gint gtk_curve_widget_add_curve(GtkCurveWidget* pCurveWidget, GCurve* pCurve);
gboolean gtk_curve_widget_remove_curve(GtkCurveWidget* pCurveWidget, gint nIndex);
void gtk_curve_widget_clear_all_curve(GtkCurveWidget* pCurveWidget);
gboolean gtk_curve_widget_check_add_curve(GtkCurveWidget* pCurveWidget);
gint gtk_curve_widget_get_curve_count(GtkCurveWidget* pCurveWidget);
GCurve* gtk_curve_widget_get_curve(GtkCurveWidget* pCurveWidget, gint nIndex);

gboolean gtk_curve_widget_print(cairo_t* cr, GtkCurveWidget* pCurveWidget, 
								gdouble x, gdouble y, gdouble width, gdouble height);

void gtk_curve_widget_set_time_scale(GtkCurveWidget* pCurveWidget, gdouble dMin, gdouble dMax);
void gtk_curve_widget_set_data_scale(GtkCurveWidget* pCurveWidget, gint nCalcType, gdouble dMin, gdouble dMax);
void gtk_curve_widget_adjust_data_scale(GtkCurveWidget* curvewidget, double *min, double *max);

void gtk_curve_widget_set_sample_pixel_interval(GtkCurveWidget* pCurveWidget, gint nNumPixel);

void gtk_curve_widget_calculate_data_scale(GtkCurveWidget* pCurveWidget, GCurve* pCurve, gdouble* dMin, gdouble* dMax);

void gtk_curve_widget_adjust_axis(GtkCurveWidget* pCurveWidget);

void gtk_curve_widget_destroy(GtkWidget* pWidget, gpointer usr_data);
void gtk_curve_widget_frame_realize(GtkWidget *pWidget, gpointer usr_data);
void gtk_curve_widget_frame_allocate(GtkWidget *pWidget, GtkAllocation *pAlloc, gpointer usr_data);
void gtk_curve_widget_adjust_layout(GtkWidget *pWidget, GtkAllocation *pAlloc, gpointer usr_data);
void gtk_curve_widget_frame_expose_event(GtkWidget *pWidget,GdkEventExpose *pEvent, gpointer usr_data);
void gtk_curve_widget_frame_draw(GtkWidget *pWidget, gpointer usr_data);
void gtk_curve_widget_frame_draw_contents(cairo_t* cr, GtkCurveWidget* pCurveWidget, GdkRectangle rect);

void gtk_curve_widget_canvas_realize(GtkWidget* pWidget, gpointer usr_data);
void gtk_curve_widget_canvas_expose_event(GtkWidget *pWidget, GdkEventExpose *pEvent, gpointer usr_data);
void gtk_curve_widget_canvas_draw(GtkWidget *pWidget, gpointer usr_data);
void gtk_curve_widget_canvas_draw_contents(cairo_t* cr, GtkCurveWidget* pCurveWidget);
void gtk_curve_widget_canvas_draw_grid(cairo_t* cr, GtkCurveWidget* pCurveWidget);
void gtk_curve_widget_canvas_draw_choose_rect(cairo_t* cr, GtkCurveWidget* pCurveWidget);
void gtk_curve_widget_canvas_draw_curves(cairo_t* cr, GtkCurveWidget* pCurveWidget);
void gtk_curve_widget_canvas_draw_track(GtkWidget* pWidget, cairo_t* cr, GtkCurveWidget* pCurveWidget);
void gtk_curve_widget_canvas_draw_message(cairo_t* cr, GtkCurveWidget* pCurveWidget);
void gtk_curve_widget_canvas_draw_curves_mark(cairo_t* cr, GtkCurveWidget* pCurveWidget);

gboolean gtk_curve_widget_canvas_btn_press_event(GtkWidget *pWidget, GdkEventButton *pEvent, gpointer usr_data);
gboolean gtk_curve_widget_canvas_btn_release_event(GtkWidget *pWidget, GdkEventButton *pEvent, gpointer usr_data);
gboolean gtk_curve_widget_canvas_motion_notify_event(GtkWidget *pWidget, GdkEventButton *pEvent, gpointer usr_data);
gboolean gtk_curve_widget_canvas_key_release_event(GtkWidget *pWidget,GdkEventKey *pEvent, gpointer usr_data);

void gtk_curve_widget_draw_pango_layout(cairo_t* cr, GdkColor* color, gdouble x, gdouble y, PangoLayout* layout);
void gtk_curve_widget_draw_line(cairo_t* cr, gdouble x1, gdouble y1, gdouble x2, gdouble y2);
void gtk_curve_widget_draw_rectangle(cairo_t* cr, gdouble x, gdouble y, gdouble width, gdouble height, gboolean fill);
void gtk_curve_widget_draw_polygon(cairo_t* cr, gdouble* x, gdouble* y, int size);
void gtk_curve_widget_draw_indicator(cairo_t* cr, gdouble x, gdouble y);
void gtk_curve_widget_draw_pixpuf(cairo_t* cr, GdkPixbuf* pixbuf, 
								  gdouble sx, gdouble sy, gdouble sWidth, gdouble sHeight, 
								  gdouble x, gdouble y, gdouble width, gdouble height);

void gtk_curve_widget_canvas_hand_move(GtkCurveWidget* pCurveWidget, gdouble x, gdouble y);
void gtk_curve_widget_canvas_zoom_in(GtkCurveWidget* pCurveWidget, gdouble x, gdouble y);
void gtk_curve_widget_canvas_zoom_out(GtkCurveWidget* pCurveWidget, gdouble x, gdouble y);
void gtk_curve_widget_canvas_zoom_rect(GtkCurveWidget* pCurveWidget);

void gtk_curve_widget_canvas_set_zoom_rect(GtkCurveWidget* pCurveWidget, double dLeft, double dRight);

gint gtk_curve_widget_transform(gdouble dAxis, gdouble dMin, gdouble dMax, gint nLength, gboolean bRev);
gdouble gtk_curve_widget_invTransform(gdouble dPos, gdouble dMin, gdouble dMax, gdouble dLength, gboolean bInv);
gdouble gtk_curve_widget_scale_value(gdouble dScale, gdouble dMin, gdouble dMax);
gint gtk_curve_widget_sample(gdouble dMin, gdouble dMax, gint nWidth, gint nPixelInterval);

GdkCursor* create_cursor(GtkCurveWidget* pCurveWidget, GdkPixbuf* pPixBuf, gboolean bCenter);
void gtk_curve_widget_create_cursor(GtkCurveWidget* pCurveWidget);
void gtk_curve_widget_destroy_cursor(GtkCurveWidget* pCurveWidget);

void gtk_curve_widget_ref_cursor_pixbuf();
void gtk_curve_widget_unref_cursor_pixbuf();

void gtk_curve_widget_ref_pixbuf();
void gtk_curve_widget_unref_pixbuf();

void adjust_data_scale(gdouble* dMin, gdouble* dMax);
gint binary_find(gdouble* pData, gint nLenBuf, gint nOffset, gint nNumPt, gdouble dValue);
gint step_find(gint nLenBuf, gint nOffset, gint nNunPt, gdouble dStart, gdouble dStep, gdouble dValue, gboolean bNear);

GdkColor rgb2gdk(guint8 r, guint8 g, guint8 b);
gboolean gtk_rectangle_contains(GdkRectangle rect, gdouble x, gdouble y);

typedef struct  
{
	gint nPos;
	GdkColor* color;
}ColorStack;

typedef struct
{
	ColorStack colorUsed, colorUnused;
}GtkColorStack;

GtkColorStack* gtk_color_stack_create(GdkColor** pColors, gint nNum);
void gtk_color_stack_destroy(GtkColorStack* pStack);
GdkColor gtk_color_stack_pop_color(GtkColorStack* pStack);
void gtk_color_stack_push_color(GtkColorStack* pStack, GdkColor* color);
void gtk_color_stack_reset(GtkColorStack* pStack);

#endif
