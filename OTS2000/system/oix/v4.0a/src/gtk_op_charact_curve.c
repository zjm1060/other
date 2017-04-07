#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include "../inc/oix.h"
#include "../inc/gtk_curve_widget.h"

#include <libxml/parser.h>
#include <libxml/tree.h>

GtkWidget* op_character_curve[MAX_CRT_NUM];
int nOpCharacterCurveRef=0;

gint    op_character_curve_timer_id = 0;

gint	op_character_curve_detect_timer_id=0;
enum OperatingArea
{
	UndefinedArea=0,
	InhibitArea,
	StableArea,
	RestrictArea
};
gint	op_character_curve_detect_ref=0;
	
#define OP_CHARACTER_INHIBIT_AREA_STR		"Inhibit"
#define OP_CHARACTER_STABLE_AREA_STR		"Stable"
#define OP_CHARACTER_RESTRICT_AREA_STR		"Restrict"

enum {eStnName=0, eStnId, eStnColNum};
enum {eDevName=0, eDevId, eDevColNum};

GtkWidget *op_character_menu_stn, *op_character_menu_dev;

GtkCurveWidget opCharacterCurveWidget;

#define OP_CHARACTER_CURVE_MAX_POINTS	256
#define OP_CHARACTER_CURVE_INDICATOR_POINT	-1

enum {ePtVaneOpening, ePtActive, ePtReactive, ePtSpeed, ePtEnd};

#define OP_CHARACTER_CURVE_MAX_CURVE_TYPE	32

typedef struct _op_character_curve 
{
	int type;
	guint8 color[3];
	guint8 reserve;

	int labelpos;

	char label[32];

	int mode;

	int num;
	double x[OP_CHARACTER_CURVE_MAX_POINTS];
	double y[OP_CHARACTER_CURVE_MAX_POINTS];
}op_character_curve_t;

typedef struct _op_character_graph 
{
	int id;

	int xMin, xMax;
	int yMin, yMax;

	int num;
	op_character_curve_t	curves[MAX_NUM_CURVES-1];
}op_character_graph_t;

typedef struct _op_character_device
{
	int id;
	int graph;

	int area; //设备正在运行的区域

	UCHAR	data_type[ePtEnd];
	USHORT	pt_id[ePtEnd];

	int colorgroup;
}op_character_device_t;

#define OP_CHARACTER_CURVE_MAX_DEV	32
typedef struct _op_character_station
{
	POINTER waterhead;

	int num;
	op_character_device_t	dev[OP_CHARACTER_CURVE_MAX_DEV];
}op_character_station_t;

#define OP_CHARACTER_CURVE_MAX_GRAPH	64
#define OP_CHARACTER_CURVE_MAX_STN		8

typedef struct _op_character_curve_perf
{
	int numCurveType;
	char curveType[OP_CHARACTER_CURVE_MAX_CURVE_TYPE][32];

	int numGraph;
	op_character_graph_t	graph[OP_CHARACTER_CURVE_MAX_GRAPH];
	
	int numStn;
	op_character_station_t	stn[OP_CHARACTER_CURVE_MAX_STN];

	int inhibitArea;
	int stableArea;
	int restrictArea;
}op_character_curve_perf_t;
op_character_curve_perf_t g_opCharacterCurvePerf;

typedef struct _op_character_curve_data 
{
	POINTER waterhead;
	UCHAR	dev_id;

	UCHAR	data_type[ePtEnd];
	USHORT	pt_id[ePtEnd];
}op_character_curve_data_t;
op_character_curve_data_t op_character_curve_query_data;

GtkWidget* op_character_widget[ePtEnd+2];

guint8 op_character_color[2][3]={{255, 255, 255}, {0, 30, 128}};
int op_character_flash=0;

#define XML_NODE_OP_CHARACTER_CURVE_STR	"OperPerf"

extern COLOR_GRP cgdb[MAX_COLOR_GRPS];
extern int cgdb_cnt;

extern void PopupMessageBox (GtkWidget *parent,char msg[] );
extern char *_toUtf8(char *c);
extern char *_toUtf8EX(char *c);
extern void readRealCurvePt();

int parseDouble(char* str, double* data, int size);
int parseInt(char* str, int* data, int size);

int getUnitConditionbyId(POINTER*pt);	//机组运行工况

void destroyOpCharacterCurveData()
{
	op_character_curve_detect_ref--;
	op_character_curve_detect_ref=op_character_curve_detect_ref>0?op_character_curve_detect_ref:0;
	
	if (op_character_curve_detect_ref == 0)
	{
		if (op_character_curve_detect_timer_id != 0)
		{
			g_source_remove(op_character_curve_detect_timer_id);
			op_character_curve_detect_timer_id  = 0 ;
		}
	}
}

void DestoryOpCharacterCurveShell(GtkWidget *widget,   gpointer data)
{
	int crt_n = GPOINTER_TO_INT(data);
	
	if( crt_n < 0 || crt_n >= MAX_CRT_NUM )
		return ; 
	
	g_message("\n quit Operating Chararcteristic Curve Configure !!!!! \n");

//	destroyOpCharacterCurveData();
	
	if (op_character_curve_timer_id != 0)
	{
		g_source_remove(op_character_curve_timer_id);
		op_character_curve_timer_id  = 0 ;
	}
	
	op_character_curve[crt_n] = NULL ;
	
	nOpCharacterCurveRef--;
	
	return ;
}

void ExitOpCharacterCurveShell(GtkWidget *widget,   gpointer data)
{
	int crt_n = GPOINTER_TO_INT(data);
	if( crt_n < 0 || crt_n >= MAX_CRT_NUM)
	{
		printf("ExitOpCharacterCurveShell Failed! crt_n is invalid!!");
		return;
	}

	gtk_widget_destroy(GTK_WIDGET(op_character_curve[crt_n])); 
}

void CreateOpCharacterCurveDevMenu(GtkComboBox* pComboBox, gpointer data) 
{
	char	dev_comment[GROUP_COMMENT_SIZE];
	int	stn_id;
	int	i,grp_num;
	
	GtkTreeModel* pModel;
	GtkListStore* pStore;
	GtkTreeIter it;

	gchar *pConvert=0;
	
	gint crt_n=GPOINTER_TO_INT(data);
	if (crt_n<0 || crt_n>=MAX_CRT_NUM)
	{
		return;
	}
	
	gtk_combo_box_set_active(GTK_COMBO_BOX(op_character_menu_dev), -1);
	
	pStore=GTK_LIST_STORE(gtk_combo_box_get_model(GTK_COMBO_BOX(op_character_menu_dev)));
	gtk_list_store_clear(pStore);
	
	pModel=gtk_combo_box_get_model(GTK_COMBO_BOX(op_character_menu_stn));
	if (!gtk_combo_box_get_active_iter(GTK_COMBO_BOX(op_character_menu_stn), &it))
	{
		return;
	}
	
	gtk_tree_model_get(pModel, &it, eStnId, &stn_id, -1);
	
	if(-1 != GetGroupNumById((UCHAR)stn_id, (GULONG *)&grp_num))
	{
		for(i=1; i<=grp_num; i++) 
		{  		
			if(-1==GetGroupCommentById((UCHAR)stn_id, (UCHAR)i, dev_comment))
				continue;
			
			if (strcmp(dev_comment, "") != 0)
			{
				gtk_list_store_append (pStore, &it);
				gtk_list_store_set (pStore, &it, eDevName, (pConvert=_toUtf8(dev_comment)), eDevId, i, -1);
				G_SAFE_FREE(pConvert);
			}
		}
	}
	
	gtk_combo_box_set_active(GTK_COMBO_BOX(op_character_menu_dev), 0);
}

int parseXmlProp(xmlNodePtr node, const char *name)
{
	int ret=-1;
	xmlChar* pValue;

	if (xmlHasProp(node, BAD_CAST name))
	{
		pValue=xmlGetProp(node ,BAD_CAST name);
		ret=atoi((char*)pValue);

		xmlFree(pValue);
	}
	
	return ret;
}

gboolean readOpCharacterCurveRealPtData(POINTER *pt, double* value)
{
	FLOAT va;

	if (pt == 0 || value == 0)
	{
		return FALSE;
	}

	if (GetAnaPtfValueById(pt, &va) == -1)
	{
		printf("readOpCharacterCurveRealPtData: No find %d.%d.%d.%d", pt->stn_id, pt->dev_id, pt->pt_id, pt->data_type);
		return FALSE;
	}
	
	*value=va;

	return TRUE;
}

gboolean insidePolygon(double xPt, double yPt, double* x, double *y, int num)
{
	int count=0;
    int i;
    double xinters;
    double x1, x2;
	double y1, y2;

	if (x==NULL || y==NULL)
	{
		printf("insidePolygon: x==NULL or y==NULL!!!");
		return FALSE;
	}

	if (x[0] == x[num-1] && y[0] == y[num-1])
	{
		num--;
	}

	if (num < 2)
	{
		return FALSE;
	}
    
	x1=x[0];
	y1=y[0];
    for (i=1; i<=num; i++)
	{
        x2=x[i%num];
		y2=y[i%num];

        if (yPt > MIN(y1, y2) && yPt <= MAX(y1, y2))
		{
            if (xPt <= MAX(x1,x2))
			{
				if (y1 != y2)
				{
					xinters = (yPt-y1)*(x2-x1)/(y2-y1)+x1;
					if (x1 == x2 || xPt <= xinters)
					{
						count++;
					}
				}
			}
		}

		x1=x2;
		y1=y2;
	}

    if (count % 2 != 0)
	{
        return TRUE;
	}

	return FALSE;
}

gboolean detectOpCharacterCurve(gpointer usr_data)
{
	int i, j;

	POINTER	pt;
	
	int graph;

	double x=0, y=0;
	op_character_curve_t* pCurveInhibit=NULL;
	op_character_curve_t* pCurveStable=NULL;
	op_character_curve_t* pCurveRestrict=NULL;

	for (i=0; i<g_opCharacterCurvePerf.numStn; i++)
	{
		pt.stn_id=g_opCharacterCurvePerf.stn[i].waterhead.stn_id;
		for (j=0; j<g_opCharacterCurvePerf.stn[i].num; j++)
		{
			graph=g_opCharacterCurvePerf.stn[i].dev[j].graph;

			pt.dev_id=g_opCharacterCurvePerf.stn[i].dev[j].id;
			pt.data_type=g_opCharacterCurvePerf.stn[i].dev[j].data_type[ePtActive];
			pt.pt_id=g_opCharacterCurvePerf.stn[i].dev[j].pt_id[ePtActive];
			
			g_opCharacterCurvePerf.stn[i].dev[j].area=UndefinedArea;
			if (!readOpCharacterCurveRealPtData(&pt, &x))
			{
				continue;
			}
			
			pt=g_opCharacterCurvePerf.stn[i].waterhead;
			
			if (!readOpCharacterCurveRealPtData(&pt, &y))
			{
				continue;
			}

			if (g_opCharacterCurvePerf.inhibitArea != -1 &&
				g_opCharacterCurvePerf.inhibitArea < g_opCharacterCurvePerf.graph[graph].num)
			{
				pCurveInhibit=&g_opCharacterCurvePerf.graph[graph].curves[g_opCharacterCurvePerf.inhibitArea];
			}

			if (g_opCharacterCurvePerf.stableArea != -1 &&
				g_opCharacterCurvePerf.stableArea < g_opCharacterCurvePerf.graph[graph].num)
			{
				pCurveStable=&g_opCharacterCurvePerf.graph[graph].curves[g_opCharacterCurvePerf.stableArea];
			}

			if (g_opCharacterCurvePerf.restrictArea != -1 &&
				g_opCharacterCurvePerf.restrictArea < g_opCharacterCurvePerf.graph[graph].num)
			{
				pCurveRestrict=&g_opCharacterCurvePerf.graph[graph].curves[g_opCharacterCurvePerf.restrictArea];
			}
			
			if (pCurveInhibit != NULL && insidePolygon(x, y, pCurveInhibit->x, pCurveInhibit->y, pCurveInhibit->num))
			{
				g_opCharacterCurvePerf.stn[i].dev[j].area=InhibitArea;
			}
			else if (pCurveStable != NULL && insidePolygon(x, y, pCurveStable->x, pCurveStable->y, pCurveStable->num))
			{
				g_opCharacterCurvePerf.stn[i].dev[j].area=StableArea;
			}
			if (pCurveRestrict != NULL && insidePolygon(x, y, pCurveRestrict->x, pCurveRestrict->y, pCurveRestrict->num))
			{
				g_opCharacterCurvePerf.stn[i].dev[j].area=RestrictArea;
			}
		}
	}

	return TRUE;
}

int readOpCharacterCurveData()
{
	xmlDocPtr   pDoc = NULL;
	xmlNodePtr  pRoot = NULL, pNodeSubRoot=NULL, pNodeStation = NULL,pNodeDev = NULL, pNodePt=NULL;
	xmlNodePtr	pNodeCurve=NULL, pNodeCurvePt;
	xmlChar *pValue;
	char str[128];

	int nMin, nMax;

	int nNumXPt, nNumYPt, nNum;
	int i;
	gdouble xAxis[OP_CHARACTER_CURVE_MAX_POINTS], yAxis[OP_CHARACTER_CURVE_MAX_POINTS];

	int color[3];

	char strLabel[32];

	op_character_curve_t curve;
	op_character_graph_t graph;
	op_character_device_t dev;
	op_character_station_t stn;

	if (op_character_curve_detect_ref > 0)
	{
		return 0;
	}

	op_character_curve_detect_ref++;

	g_opCharacterCurvePerf.numStn=0;
	g_opCharacterCurvePerf.numGraph=0;
	g_opCharacterCurvePerf.numCurveType=0;
	g_opCharacterCurvePerf.inhibitArea=UndefinedArea;
	g_opCharacterCurvePerf.stableArea=UndefinedArea;
	g_opCharacterCurvePerf.restrictArea=UndefinedArea;

	xmlKeepBlanksDefault(0);
	pDoc = xmlReadFile (FILE_OIX_CONFIG_XML, "GB2312", XML_PARSE_NOBLANKS);
	if (pDoc == NULL)
    {
		sprintf(str, "Fail to open %s or format error!", FILE_OIX_CONFIG_XML);
		Err_log(str);
		return -1;
    }
	
	pRoot = xmlDocGetRootElement (pDoc);
	if (pRoot == NULL)
    {
		sprintf(str, "%s not have root", FILE_OIX_CONFIG_XML);
		Err_log(str);
		return -1;
    }
	
	pNodeSubRoot = pRoot->xmlChildrenNode;
	while (pNodeSubRoot != NULL)
    {
		if (xmlStrcmp (pNodeSubRoot->name, BAD_CAST(XML_NODE_OP_CHARACTER_CURVE_STR)) == 0)
        {
			pNodeStation = pNodeSubRoot->xmlChildrenNode;
			while(pNodeStation != NULL)
			{
				if (xmlStrcmp (pNodeStation->name, BAD_CAST "Station") == 0)
				{
					stn.num=0;
					
					stn.waterhead.stn_id=(UCHAR)parseXmlProp(pNodeStation, "id");
					stn.waterhead.dev_id=(UCHAR)-1;
					stn.waterhead.data_type=(UCHAR)-1;
					stn.waterhead.pt_id=(USHORT)-1;

					pNodeDev = pNodeStation->xmlChildrenNode;
					while (pNodeDev != NULL)
					{
						if (xmlStrcmp(pNodeDev->name, BAD_CAST "WaterHead") == 0)
						{
							stn.waterhead.dev_id=(UCHAR)parseXmlProp(pNodeDev, "dev");
							stn.waterhead.data_type=(UCHAR)parseXmlProp(pNodeDev, "datatype");
							stn.waterhead.pt_id=(USHORT)parseXmlProp(pNodeDev, "point");
						}
						else if(xmlStrcmp(pNodeDev->name, BAD_CAST "Dev") == 0)
						{
							dev.id=parseXmlProp(pNodeDev, "id");

							dev.graph=parseXmlProp(pNodeDev, "graph");

							dev.colorgroup=-1;
							if (xmlHasProp(pNodeDev, BAD_CAST "colorGroup"))
							{
								strcpy(str, (char*)xmlGetProp(pNodeDev ,BAD_CAST "colorGroup"));

								for (i=0; i<cgdb_cnt; i++)
								{
									if (strcmp(str, cgdb[i].name) == 0)
									{
										dev.colorgroup=i;
										break;
									}
								}
							}

							pNodePt=pNodeDev->xmlChildrenNode;
							while (pNodePt != NULL)
							{
								if(xmlStrcmp(pNodePt->name, BAD_CAST "Point") == 0)
								{
									dev.pt_id[ePtVaneOpening]=parseXmlProp(pNodePt, "guideVaneOpening");
									dev.pt_id[ePtActive]=parseXmlProp(pNodePt, "activePower");
									dev.pt_id[ePtReactive]=parseXmlProp(pNodePt, "reactivePower");
									dev.pt_id[ePtSpeed]=parseXmlProp(pNodePt, "speed");
								}
								else if (xmlStrcmp(pNodePt->name, BAD_CAST "DataType") == 0)
								{
									dev.data_type[ePtVaneOpening]=parseXmlProp(pNodePt, "guideVaneOpening");
									dev.data_type[ePtActive]=parseXmlProp(pNodePt, "activePower");
									dev.data_type[ePtReactive]=parseXmlProp(pNodePt, "reactivePower");
									dev.data_type[ePtSpeed]=parseXmlProp(pNodePt, "speed");
								}

								pNodePt = pNodePt->next;
							}

							stn.dev[stn.num++]=dev;
						}
						
						pNodeDev = pNodeDev->next;
					}

					g_opCharacterCurvePerf.stn[g_opCharacterCurvePerf.numStn++]=stn;
				}
				else if (xmlStrcmp (pNodeStation->name, BAD_CAST "Graph") == 0)
				{
					graph.num=0;

					graph.id=parseXmlProp(pNodeStation, "id");
					
					nMin=parseXmlProp(pNodeStation, "xMin");
					nMax=parseXmlProp(pNodeStation, "xMax");
					if (nMin > nMax)
					{
						nMax=nMin+100;
					}

					graph.xMin=nMin;
					graph.xMax=nMax;

					nMin=parseXmlProp(pNodeStation, "yMin");
					nMax=parseXmlProp(pNodeStation, "yMax");
					if (nMin > nMax)
					{
						nMax=nMin+100;
					}

					graph.yMin=nMin;
					graph.yMax=nMax;

					pNodeCurve = pNodeStation->xmlChildrenNode;
					while (pNodeCurve != NULL)
					{
						if(xmlStrcmp(pNodeCurve->name, BAD_CAST "Curve") != 0 
							|| !xmlHasProp(pNodeCurve, BAD_CAST "type"))
						{
							pNodeCurve=pNodeCurve->next;
							continue;
						}

						curve.color[0]=0;
						curve.color[1]=0;
						curve.color[2]=255;
						curve.mode=eCurveLine;
						curve.labelpos=eCurveNoLabel;
						curve.num=0;
						memset(curve.label, 0, sizeof(curve.label));

						strcpy(str, xmlGetProp(pNodeCurve,BAD_CAST "type"));
						for (i=0; i<g_opCharacterCurvePerf.numCurveType; i++)
						{
							if (strcmp(g_opCharacterCurvePerf.curveType[i], str) == 0)
							{
								curve.type=i;
								break;
							}
						}

						if (i >= g_opCharacterCurvePerf.numCurveType)
						{
							if (strcmp(str, OP_CHARACTER_INHIBIT_AREA_STR) == 0)
							{
								g_opCharacterCurvePerf.inhibitArea=g_opCharacterCurvePerf.numCurveType;
							}
							else if (strcmp(str, OP_CHARACTER_STABLE_AREA_STR) == 0)
							{
								g_opCharacterCurvePerf.stableArea=g_opCharacterCurvePerf.numCurveType;
							}
							else if (strcmp(str, OP_CHARACTER_RESTRICT_AREA_STR) == 0)
							{
								g_opCharacterCurvePerf.restrictArea=g_opCharacterCurvePerf.numCurveType;
							}

							curve.type=g_opCharacterCurvePerf.numCurveType;
							strcpy(g_opCharacterCurvePerf.curveType[g_opCharacterCurvePerf.numCurveType++], str);
						}

						if (xmlHasProp(pNodeCurve, BAD_CAST "rgb"))
						{
							parseInt((char*)xmlGetProp(pNodeCurve,BAD_CAST "rgb"), color, COUNT_OF(color));
							curve.color[0]=(guint8)color[0];
							curve.color[1]=(guint8)color[1];
							curve.color[2]=(guint8)color[2];
						}

						if (xmlHasProp(pNodeCurve, BAD_CAST "mode"))
						{
							strcpy(strLabel, (char*)xmlGetProp(pNodeCurve,BAD_CAST "mode"));
							if (strcmp(strLabel, "fill") == 0)
							{
								curve.mode=eCurveFill;
							}
							else if (strcmp(strLabel, "dash") == 0)
							{
								curve.mode=eCurveDash;
							}
							else if (strcmp(strLabel, "indicator") == 0)
							{
								curve.mode=eCurveIndicator;
							}
						}

						if (xmlHasProp(pNodeCurve, BAD_CAST "label"))
						{
							strcpy(curve.label, (char*)xmlGetProp(pNodeCurve,BAD_CAST "label"));
						}

						if (xmlHasProp(pNodeCurve, BAD_CAST "labelpos"))
						{
							strcpy(strLabel, (char*)xmlGetProp(pNodeCurve,BAD_CAST "labelpos"));
							if (strcmp(strLabel, "start") == 0)
							{
								curve.labelpos=eCurveLabelStart;
							}
							else if (strcmp(strLabel, "mid") == 0)
							{
								curve.labelpos=eCurveLabelMid;
							}
							else if (strcmp(strLabel, "end") == 0)
							{
								curve.labelpos=eCurveLabelEnd;
							}
						}

						nNumYPt=nNumXPt=0;
						pNodeCurvePt=pNodeCurve->xmlChildrenNode;
						while (pNodeCurvePt != NULL)
						{
							if(nNumXPt==0 && xmlStrcmp(pNodeCurvePt->name, BAD_CAST "x") == 0)
							{
								pValue=xmlNodeGetContent(pNodeCurvePt);
								
								nNumXPt=parseDouble((char*)pValue, xAxis, OP_CHARACTER_CURVE_MAX_POINTS);
								
								xmlFree(pValue);
							}
							else if (nNumYPt==0 && xmlStrcmp(pNodeCurvePt->name, BAD_CAST "y") == 0)
							{
								pValue=xmlNodeGetContent(pNodeCurvePt);
								
								nNumYPt=parseDouble((char*)pValue, yAxis, OP_CHARACTER_CURVE_MAX_POINTS);
								
								xmlFree(pValue);
							}

							pNodeCurvePt=pNodeCurvePt->next;
						}

						nNum=nNumXPt<nNumYPt? nNumXPt:nNumYPt;
						if (nNum > 0)
						{
							curve.num=nNum;
							memcpy(curve.x, xAxis, sizeof(gdouble)*nNum);
							memcpy(curve.y, yAxis, sizeof(gdouble)*nNum);

							if (graph.num < COUNT_OF(graph.curves))
							{
								graph.curves[graph.num++]=curve;
							}
 						}
						
						pNodeCurve=pNodeCurve->next;
					}

					g_opCharacterCurvePerf.graph[g_opCharacterCurvePerf.numGraph++]=graph;
				}

				pNodeStation = pNodeStation->next;
			}
        }

		pNodeSubRoot = pNodeSubRoot->next;
    }
	
	xmlFreeDoc (pDoc);
	xmlCleanupParser();

	if (op_character_curve_detect_timer_id == 0)
	{
		op_character_curve_detect_timer_id =g_timeout_add (1000, (GSourceFunc)detectOpCharacterCurve, NULL);
	}

	return 0;
}

gboolean adjustOpCharacterCurve(gpointer usr_data)
{
	time_t curTime;
	int nIndex, i;

	GCurve* pCurve;
	int data;

	POINTER	pt;

	char text[32];

	struct tm tmTime;

	double x=0, y=0;
	FLOAT temp;

	curTime=time(NULL);

	tmTime=*localtime(&curTime);
	sprintf(text, "%04d-%02d-%02d %02d:%02d:%02d", tmTime.tm_year+1900, tmTime.tm_mon+1, tmTime.tm_mday,
		tmTime.tm_hour, tmTime.tm_min, tmTime.tm_sec) ;

	gtk_entry_set_text(GTK_ENTRY(op_character_widget[ePtEnd]), text);
	
	pt=op_character_curve_query_data.waterhead;

	nIndex=-1;
	if (GetAnaPtfValueById(&pt, &temp) != -1)
	{
		y=temp;

		sprintf(text, "%.3lf", y);
		gtk_entry_set_text(GTK_ENTRY(op_character_widget[ePtEnd+1]), text);
	}
	else
	{
		printf("adjustOpCharacterCurve: No find %d.%d.%d.%d\n", pt.stn_id, pt.dev_id, pt.pt_id, pt.data_type);
	}

	for (i=0; i<ePtEnd; i++)
	{
		pt.dev_id=op_character_curve_query_data.dev_id;
		pt.pt_id=op_character_curve_query_data.pt_id[i];
		pt.data_type=op_character_curve_query_data.data_type[i];

		nIndex=-1;

		if (GetAnaPtfValueById(&pt, &temp) == -1)
		{
			printf("adjustOpCharacterCurve: No find %d.%d.%d.%d", pt.stn_id, pt.dev_id, pt.pt_id, pt.data_type);
			continue;
		}

		if (i == ePtActive)
		{
			x=temp;
		}
		
		sprintf(text, "%.3lf", temp);

		gtk_entry_set_text(GTK_ENTRY(op_character_widget[i]), text);
	}

	for (nIndex=0; nIndex < gtk_curve_widget_get_curve_count(&opCharacterCurveWidget); nIndex++)
	{
		pCurve=gtk_curve_widget_get_curve(&opCharacterCurveWidget, nIndex);
		if (pCurve != NULL)
		{
			data=GPOINTER_TO_INT(gtk_curve_get_user_data(pCurve, 0));
			if (data != OP_CHARACTER_CURVE_INDICATOR_POINT)
			{
				continue;
			}
			
			gtk_curve_set_data(pCurve, &x , &y, 1, 0, 1, 0);
			gtk_curve_set_boundary(pCurve, FALSE, 0, 0);

			op_character_flash=(++op_character_flash)%2;
			gtk_curve_set_rgb_color(pCurve, op_character_color[op_character_flash][0],
			op_character_color[op_character_flash][1], op_character_color[op_character_flash][2]);
		}
	}

	gtk_curve_widget_frame_redraw(&opCharacterCurveWidget);

	return TRUE;
}

void UpdateOpCharacterDevSelect(GtkComboBox* pComboBox, gpointer data)
{
	int stn_id, dev_id;

	GtkTreeModel* pModel;
	GtkTreeIter it;

	char stn_comment[STATION_COMMENT_SIZE];
	char dev_comment[GROUP_COMMENT_SIZE];

	char strText[128];
	char *pConvert, *pConvert1, *pConvert2;

	GCurve* pCurve;
	int i, j;
	int graph=-1;
	int colorgroup=-1;

	op_character_curve_t* pOpCharacterCurve;
	
	int crt_n=GPOINTER_TO_INT(data);
	if( crt_n < 0 || crt_n >= MAX_CRT_NUM)
		return;

	gtk_curve_widget_clear_all_curve(&opCharacterCurveWidget);

	if (op_character_curve_timer_id != 0)
	{
		g_source_remove(op_character_curve_timer_id);
		op_character_curve_timer_id  = 0 ;
	}

	op_character_curve_query_data.waterhead.stn_id=(UCHAR)-1;
	op_character_curve_query_data.dev_id=(UCHAR)-1;
	
	pModel=gtk_combo_box_get_model(GTK_COMBO_BOX(op_character_menu_stn));
	if (gtk_combo_box_get_active_iter(GTK_COMBO_BOX(op_character_menu_stn), &it))
	{
		gtk_tree_model_get(pModel, &it, eStnId, &stn_id, -1);
		op_character_curve_query_data.waterhead.stn_id=(UCHAR)stn_id;
	}
	
	pModel=gtk_combo_box_get_model(GTK_COMBO_BOX(op_character_menu_dev));
	if (gtk_combo_box_get_active_iter(GTK_COMBO_BOX(op_character_menu_dev), &it))
	{
		gtk_tree_model_get(pModel, &it, eDevId, &dev_id, -1);
		op_character_curve_query_data.dev_id=(UCHAR)dev_id;
	}

	if (op_character_curve_query_data.waterhead.stn_id == (UCHAR)-1 ||
		op_character_curve_query_data.dev_id == (UCHAR)-1)
	{
		return;
	}

	GetStnCommentById(op_character_curve_query_data.waterhead.stn_id, stn_comment);
	GetGroupCommentById(op_character_curve_query_data.waterhead.stn_id, 
		op_character_curve_query_data.dev_id, dev_comment);

	sprintf(strText, "%s --- %s %s", (pConvert=_toUtf8(stn_comment)), (pConvert1=_toUtf8(dev_comment)), 
		(pConvert2=_toUtf8EX("OIX_CSTR_OPERATING_CHARACTERISTIC_CURVE")));
	G_SAFE_FREE(pConvert);
	G_SAFE_FREE(pConvert1);
	G_SAFE_FREE(pConvert2);

	gtk_curve_widget_set_text(&opCharacterCurveWidget, strText, eTextTitle);

	for (i=0; i<g_opCharacterCurvePerf.numStn; i++)
	{
		if (g_opCharacterCurvePerf.stn[i].waterhead.stn_id != op_character_curve_query_data.waterhead.stn_id)
		{
			continue;
		}

		op_character_curve_query_data.waterhead=g_opCharacterCurvePerf.stn[i].waterhead;

		for (j=0; j<g_opCharacterCurvePerf.stn[i].num; j++)
		{
			if ((UCHAR)g_opCharacterCurvePerf.stn[i].dev[j].id == op_character_curve_query_data.dev_id)
			{
				memcpy(op_character_curve_query_data.data_type, g_opCharacterCurvePerf.stn[i].dev[j].data_type, 
					sizeof(ePtEnd));
				memcpy(op_character_curve_query_data.pt_id, g_opCharacterCurvePerf.stn[i].dev[j].pt_id, 
					sizeof(ePtEnd)*sizeof(USHORT));

				colorgroup=g_opCharacterCurvePerf.stn[i].dev[j].colorgroup;
				graph=g_opCharacterCurvePerf.stn[i].dev[j].graph;
				break;
			}
		}
	}

	if (graph != -1)
	{
		for (i=0; i<g_opCharacterCurvePerf.numGraph; i++)
		{
			if (g_opCharacterCurvePerf.graph[i].id != graph)
			{
				continue;
			}

			gtk_curve_widget_set_time_scale(&opCharacterCurveWidget, g_opCharacterCurvePerf.graph[i].xMin, 
				g_opCharacterCurvePerf.graph[i].xMax);

			gtk_curve_widget_set_data_scale(&opCharacterCurveWidget, eCalcManual, g_opCharacterCurvePerf.graph[i].yMin,
				g_opCharacterCurvePerf.graph[i].yMax);

			for (j=0; j<g_opCharacterCurvePerf.graph[i].num; j++)
			{
				pOpCharacterCurve=&g_opCharacterCurvePerf.graph[i].curves[j];

				pCurve=gtk_curve_create(OP_CHARACTER_CURVE_MAX_POINTS, eModeNormalAndNoSample);
				
				if (colorgroup != -1)
				{
					if (pOpCharacterCurve->type == g_opCharacterCurvePerf.inhibitArea)
					{
						gtk_curve_set_int_color(pCurve, cgdb[colorgroup].cgrp_cell[InhibitArea].c_normal);
					}
					else if (pOpCharacterCurve->type == g_opCharacterCurvePerf.stableArea)
					{
						gtk_curve_set_int_color(pCurve, cgdb[colorgroup].cgrp_cell[StableArea].c_normal);
					}
					else if (pOpCharacterCurve->type == g_opCharacterCurvePerf.restrictArea)
					{
						gtk_curve_set_int_color(pCurve, cgdb[colorgroup].cgrp_cell[RestrictArea].c_normal);
					}
					else
					{
						gtk_curve_set_rgb_color(pCurve, pOpCharacterCurve->color[0], pOpCharacterCurve->color[1],
							pOpCharacterCurve->color[2]);
					}
				}
				else
				{
					gtk_curve_set_rgb_color(pCurve, pOpCharacterCurve->color[0], pOpCharacterCurve->color[1],
					pOpCharacterCurve->color[2]);
				}
				
				gtk_curve_set_type(pCurve, pOpCharacterCurve->mode);
				gtk_curve_set_label_pos(pCurve, pOpCharacterCurve->labelpos);
				gtk_curve_set_text(pCurve, pOpCharacterCurve->label);
				gtk_curve_set_data(pCurve, pOpCharacterCurve->x , pOpCharacterCurve->y,
					OP_CHARACTER_CURVE_MAX_POINTS, 0, pOpCharacterCurve->num, 0);
				gtk_curve_set_boundary(pCurve, FALSE, 0, 0);
				
				gtk_curve_widget_add_curve(&opCharacterCurveWidget, pCurve);
			}
		}
	}

	if (gtk_curve_widget_check_add_curve(&opCharacterCurveWidget))
	{
		pCurve=gtk_curve_create(1, eModeNormalAndNoSample);
		
		op_character_flash=(++op_character_flash)%2;
		gtk_curve_set_rgb_color(pCurve, op_character_color[op_character_flash][0],
			op_character_color[op_character_flash][1], op_character_color[op_character_flash][2]);
		
		gtk_curve_set_type(pCurve, eCurveIndicator);
		gtk_curve_set_user_data(pCurve, GINT_TO_POINTER(OP_CHARACTER_CURVE_INDICATOR_POINT), 0);
		
		gtk_curve_widget_add_curve(&opCharacterCurveWidget, pCurve);
	}

	if (op_character_curve_timer_id == 0)
	{
		op_character_curve_timer_id =g_timeout_add (1000, (GSourceFunc)adjustOpCharacterCurve, 
			GINT_TO_POINTER(crt_n));
	}

	gtk_curve_widget_frame_redraw(&opCharacterCurveWidget);
}

GtkWidget* CreateOpCharacterCurveToolbar(GtkWidget * parent, gint crt_n)
{
	GtkWidget *box;  		
	GtkWidget *toolbar;
	GtkWidget *label;
	GtkWidget *pBtnExit;

	int stn_num, i;

	char stn_comment[STATION_COMMENT_SIZE];
	char *pConvert;

	GtkListStore *pStoreStn, *pStoreDev;
	GtkTreeIter it;
	
	GtkCellRenderer *pRenderStn, *pRenderDev;

	if( crt_n < 0 || crt_n >= MAX_CRT_NUM)
	{
		printf("CreateOpCharacterCurveToolbar Failed!");
		return NULL;
	}
	
	toolbar = gtk_handle_box_new ();
	
	box = gtk_hbox_new (FALSE, 4);
	gtk_container_add (GTK_CONTAINER (toolbar), box);
	gtk_container_border_width (GTK_CONTAINER (box), 5);
	gtk_widget_show (box);

	//厂站
	label = gtk_label_new ((pConvert=_toUtf8EX(OIX_CSTR_STATION)));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start (GTK_BOX (box), label, FALSE, FALSE, 0);
	gtk_widget_show (label);
	
	pStoreStn=gtk_list_store_new(eStnColNum, G_TYPE_STRING, G_TYPE_INT);
	
	GetStationNum((GULONG *)&stn_num);
	for(i=1; i<=stn_num; i++)
	{ 
		UCHAR	stn_id;
		
		if(-1==GetStnIdByIndex((UCHAR)i, &stn_id))
			continue;
		
		if( GetStnCommentById(stn_id, stn_comment)==-1 )
			continue;
		if (strcmp(stn_comment, "") != 0)
		{
			gtk_list_store_append(pStoreStn, &it);
			gtk_list_store_set(pStoreStn, &it, eStnName,  (pConvert=_toUtf8(stn_comment)), eStnId, stn_id, -1);
			G_SAFE_FREE(pConvert);
		}
	}
	
	op_character_menu_stn=gtk_combo_box_new_with_model(GTK_TREE_MODEL(pStoreStn));
	
	pRenderStn = gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(op_character_menu_stn), pRenderStn, FALSE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(op_character_menu_stn), pRenderStn, "text", eStnName, NULL);
	g_object_unref(pStoreStn);
	
	gtk_widget_set_usize(op_character_menu_stn, 100, -1);
	gtk_box_pack_start(GTK_BOX(box), op_character_menu_stn, FALSE, FALSE, 5);
	gtk_widget_show(op_character_menu_stn);
	
	g_signal_connect (G_OBJECT(op_character_menu_stn), "changed", 
		G_CALLBACK(CreateOpCharacterCurveDevMenu), GINT_TO_POINTER(crt_n));
	
	//设备
	label = gtk_label_new ((pConvert=_toUtf8EX(OIX_CSTR_ALM_DEV_FILTER)));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start (GTK_BOX (box), label, FALSE, FALSE, 0);
	gtk_widget_show (label);
	
	pRenderDev = gtk_cell_renderer_text_new();
	
	pStoreDev=gtk_list_store_new(eDevColNum, G_TYPE_STRING, G_TYPE_INT);
	op_character_menu_dev = gtk_combo_box_new_with_model(GTK_TREE_MODEL(pStoreDev));
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(op_character_menu_dev), pRenderDev, FALSE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(op_character_menu_dev), pRenderDev, "text", eDevName, NULL);
	g_object_unref(pStoreDev);
	
	gtk_widget_set_usize(op_character_menu_dev, 130, -1);
	gtk_box_pack_start (GTK_BOX (box), op_character_menu_dev, FALSE, FALSE, 5);
	gtk_widget_show (op_character_menu_dev);
	
	g_signal_connect (G_OBJECT(op_character_menu_dev), "changed", 
		G_CALLBACK(UpdateOpCharacterDevSelect), GINT_TO_POINTER(crt_n));

	//运行状况
	for (i=0; i<ePtEnd+2; i++)
	{
		op_character_widget[i]= gtk_entry_new_with_max_length (i==ePtEnd? 50:15);
		
		gtk_widget_set_usize (op_character_widget[i], i==ePtEnd? 130:60, -1);
		gtk_entry_set_position(GTK_ENTRY (op_character_widget[i]),0);
 		gtk_entry_set_editable(GTK_ENTRY (op_character_widget[i]),FALSE);
 		gtk_widget_set_sensitive(GTK_WIDGET(op_character_widget[i]),FALSE);
	}

	label = gtk_label_new ((pConvert=_toUtf8EX("OIX_CSTR_OPERATING_CHARACTERISTIC_CURVE_HEAD_UNIT")));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start (GTK_BOX (box), label, FALSE, FALSE, 0);

	gtk_box_pack_start (GTK_BOX (box), op_character_widget[ePtEnd+1], FALSE, FALSE, 0);

	label = gtk_label_new ((pConvert=_toUtf8EX("OIX_CSTR_OPERATING_CHARACTERISTIC_CURVE_OUTPUT_UNIT")));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start (GTK_BOX (box), label, FALSE, FALSE, 0);
	
	gtk_box_pack_start (GTK_BOX (box), op_character_widget[ePtActive], FALSE, FALSE, 0);

	label = gtk_label_new ((pConvert=_toUtf8EX("OIX_CSTR_OPERATING_CHARACTERISTIC_CURVE_SPEED")));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start (GTK_BOX (box), label, FALSE, FALSE, 0);
	
	gtk_box_pack_start (GTK_BOX (box), op_character_widget[ePtSpeed], FALSE, FALSE, 0);

	label = gtk_label_new ((pConvert=_toUtf8EX("OIX_CSTR_OPERATING_CHARACTERISTIC_CURVE_OPENING")));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start (GTK_BOX (box), label, FALSE, FALSE, 0);
	
	gtk_box_pack_start (GTK_BOX (box), op_character_widget[ePtVaneOpening], FALSE, FALSE, 0);

	label = gtk_label_new ((pConvert=_toUtf8EX("OIX_CSTR_OPERATING_CHARACTERISTIC_CURVE_REACTIVE")));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start (GTK_BOX (box), label, FALSE, FALSE, 0);
	
	gtk_box_pack_start (GTK_BOX (box), op_character_widget[ePtReactive], FALSE, FALSE, 0);

	gtk_box_pack_start (GTK_BOX (box), op_character_widget[ePtEnd], FALSE, FALSE, 0);
	
    pBtnExit = gtk_button_new_with_label((pConvert=_toUtf8EX(OIX_CSTR_EXIT)));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_end (GTK_BOX (box), pBtnExit, FALSE, FALSE, 0);
	
	g_signal_connect(GTK_OBJECT(pBtnExit), "clicked", G_CALLBACK(ExitOpCharacterCurveShell), GINT_TO_POINTER(crt_n));

	return (toolbar);
}

GtkWidget *CreateProgOpCharacterCurveShell (GtkWidget *parent , int crt_n )
{
	GtkWidget *vbox;
	GtkWidget *windows;
	GtkWidget *toolbar;
	GtkWidget *frame;
	
#ifdef SUN_OS
	GdkScreen* pScreen;
#endif

	gchar *pConvert=0;
	
	gtk_set_locale();
	
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
	
	gtk_window_set_title(GTK_WINDOW(windows), 
		(pConvert=_toUtf8EX("MSG_OPERATING_CHARACTERISTIC_CURVE")));    //运转特性曲线
	G_SAFE_FREE(pConvert);
	gtk_window_set_transient_for(GTK_WINDOW(windows),GTK_WINDOW(parent));
	
	g_signal_connect(G_OBJECT(windows), "destroy", 
		G_CALLBACK(DestoryOpCharacterCurveShell), GINT_TO_POINTER(crt_n));
	
	vbox=gtk_vbox_new(FALSE, 5);
	gtk_container_set_border_width(GTK_CONTAINER(vbox), 0);
	gtk_container_add(GTK_CONTAINER(windows), vbox);
	gtk_widget_show(vbox);
	
	toolbar =  CreateOpCharacterCurveToolbar(windows, crt_n);
	gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, TRUE, 0);
	gtk_widget_show(toolbar);

	frame=gtk_curve_widget_create(windows, &opCharacterCurveWidget);
	
	gtk_box_pack_start(GTK_BOX(vbox ), frame, TRUE, TRUE, 0);
	
	gtk_curve_widget_set_text(&opCharacterCurveWidget, 
		(pConvert=_toUtf8EX("OIX_CSTR_OPERATING_CHARACTERISTIC_CURVE")), eTextTitle);
	G_SAFE_FREE(pConvert);
	
	gtk_curve_widget_set_data_scale(&opCharacterCurveWidget, eCalcAuto, 0, 0);
	gtk_curve_widget_set_time_scale(&opCharacterCurveWidget, 0, 100);
	gtk_curve_widget_enable(&opCharacterCurveWidget, FALSE, eEnableTimeAxis);
	gtk_curve_widget_enable(&opCharacterCurveWidget, FALSE, eEnableKeepZero);

	gtk_curve_widget_enable(&opCharacterCurveWidget, FALSE, eEnableAutoLabel);
	gtk_curve_widget_set_text(&opCharacterCurveWidget, 
		(pConvert=_toUtf8EX("OIX_CSTR_OPERATING_CHARACTERISTIC_CURVE_HEAD_UNIT")), eTextLabel);
	G_SAFE_FREE(pConvert);

	gtk_curve_widget_set_text(&opCharacterCurveWidget, 
		(pConvert=_toUtf8EX("OIX_CSTR_OPERATING_CHARACTERISTIC_CURVE_OUTPUT_UNIT")), eTextXAxisLabel);
	G_SAFE_FREE(pConvert);
	
	gtk_widget_show_all(windows);

	gtk_combo_box_set_active(GTK_COMBO_BOX(op_character_menu_stn), 0);
	
	return windows;
}

void ProgOpCharacterCurve(GtkWidget *parent ,int crt_n )
{
	if (crt_n<0 ||crt_n>=MAX_CRT_NUM)
	{
		gdk_beep();
		return;
	}
	
	if (op_character_curve[crt_n]==(GtkWidget*)NULL)
	{
		if (nOpCharacterCurveRef >= 1)
		{
			gdk_beep(); 
			PopupMessageBox(parent , "OIX: Operating Characteristic Curve Configure has opened !!");
			return;
		}
		nOpCharacterCurveRef++;

		op_character_curve[crt_n]  = CreateProgOpCharacterCurveShell (parent, crt_n);

		gtk_widget_show_all(op_character_curve[crt_n] );
	}
	else
	{
		gtk_window_present(GTK_WINDOW(op_character_curve[crt_n]) );
	}
}

int parseDouble(char* str, double* data, int size)
{
	int nNum=0;
	int nLen=strlen(str);
	int i=0, j=0;
	
	char* pStr=str;
	char content[32];

	if (str == 0 || (data != 0 && size <= 0))
	{
		return 0;
	}
	
	while (j<=nLen)
	{
		if (*pStr == ' ' || *pStr == '\t' || *pStr == '\n' || j == nLen)
		{
			if (j > i)
			{
				memcpy(content, str+i, j-i);
				content[j-i]='\0';

				nNum++;
				
				if (data != 0)
				{
					sscanf(content, "%lf", data);

					if (nNum > size)
					{
						break;
					}
					
					data++;
				}
			}
			
			i=j+1;
		}
		
		pStr++;
		j++;
	}
	
	return nNum;
}

int parseInt(char* str, int* data, int size)
{
	int nNum=0;
	int nLen=strlen(str);
	int i=0, j=0;
	
	char* pStr=str;
	char content[32];

	if (str == 0 || (data != 0 && size <= 0))
	{
		return 0;
	}
	
	while (j<=nLen)
	{
		if (*pStr == ' ' || *pStr == '\t' || *pStr == '\n' || j == nLen)
		{
			if (j > i)
			{
				memcpy(content, str+i, j-i);
				content[j-i]='\0';
				
				nNum++;

				if (data != 0)
				{
					sscanf(content, "%d", data);

					if (nNum > size)
					{
						break;
					}
					
					data++;
				}
			}
			
			i=j+1;
		}
		
		pStr++;
		j++;
	}
	
	return nNum;
}

int getUnitConditionbyId(POINTER* pt)
{
	int i, j;

	if (pt == 0)
	{
		printf("getUnitConditionbyId: pt==0!!!");
		
		return UndefinedArea;
	}
	
	for (i=0; i<g_opCharacterCurvePerf.numStn; i++)
	{
		if (g_opCharacterCurvePerf.stn[i].waterhead.stn_id != pt->stn_id)
		{
			continue;
		}

		for (j=0; j<g_opCharacterCurvePerf.stn[i].num; j++)
		{
			if (g_opCharacterCurvePerf.stn[i].dev[j].id == pt->dev_id)
			{
				return g_opCharacterCurvePerf.stn[i].dev[j].area;
			}
		}
	}
	
	return UndefinedArea;
}
