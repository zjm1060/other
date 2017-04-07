#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include "../inc/oix.h"

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "../inc/gtk_curve_widget.h"
#include "../../../pas/v4.0a/inc/trend_data_rec.h"

GtkWidget* gen_power_plan_curve[MAX_CRT_NUM];
int nGenPowerPlanCurveRef=0;

gint    gen_power_plan_curve_timer_id = 0;

enum {eStnName=0, eStnId, eStnColNum};
enum {ePowerStnName=0, ePowerStnId, ePowerStnColNum};
enum {eGenPowerPlanCurve, eGenPowerRealCurve, eGenPowerCurveNum};

GtkWidget *gen_power_plan_menu_stn, *gen_power_plan_menu_power;
GtkWidget *gen_power_plan_tm_entry;

#define MAX_STATION_NAME_LEN	32
#define MAX_STATION_ENTRY_NAME_LEN	64
typedef struct _gen_power_plan_power_station 
{
	int id;
	char interval;

	POINTER pt;

	char name[MAX_STATION_NAME_LEN];
}gen_power_plan_power_station_t;

#define MAX_GEN_POWER_PLAN_POWER_STATIONS	24
typedef struct _gen_power_plan_power_stations
{
	char name[MAX_STATION_NAME_LEN];	//厂站名

	int num;	//电站数
	gen_power_plan_power_station_t stations[MAX_GEN_POWER_PLAN_POWER_STATIONS];
}gen_power_plan_power_stations_t;

#define MAX_GEN_POWER_PLAN_STATIONS	12
typedef struct _gen_power_plan_stations 
{
	int num;	//厂站数
	gen_power_plan_power_stations_t stations[MAX_GEN_POWER_PLAN_STATIONS];
}gen_power_plan_stations_t;

gen_power_plan_stations_t gen_power_plan_stations;

GtkCurveWidget genPowerPlanCurveWidget;

SHM_HDB gen_power_plan_curve_data,*shm_gen_power_plan_curve ;

#define 	SLEEP_TIME 		10

gboolean gen_power_plan_thread_exit_flag = FALSE;

typedef struct _gen_power_plan_query_data
{
	int stn_id;			//厂站ID
	int power_id;		//电站ID

	gboolean finished;		//查询是否完毕

	struct tm time;		//查询起始时间
}gen_power_plan_query_data_t;
gen_power_plan_query_data_t queryData;

short   gen_power_plan_curve_color_group[2] = {RED1,GREEN1};
char gen_power_plan_curve_text[eGenPowerCurveNum][48]={"OIX_CSTR_GEN_POWER_PLAN_CURVE", "OIX_CSTR_GEN_POWER_REAL_CURVE"};

#define		MAX_POINTER_NUM		5000
double dGenPowerPlanCurveData[DATA_NUM];

extern CURVE_SHM_DATA *shm_real_curve;
extern CURVE_DEF RealCCurvePt;

extern void PopupMessageBox (GtkWidget *parent,char msg[] );
extern char *_toUtf8EX(char *c);
extern void popupErrorMsgBox(GtkWidget* pParent, gchar* strMsg);

void UpdateGenPowerPlanCurveWidget(gpointer data);

#define XML_NODE_GEN_POWER_PLAN_STATIONS_STR	"GenPowerPlan"
int readGenPowerPlanStations()
{
	xmlDocPtr   pDoc = NULL;
	xmlNodePtr  pRoot = NULL, pNodeSubRoot=NULL, pNodeStation = NULL,pNodePower = NULL;
	char	err_str[256];
	int index, powerIndex, interval;

	for (index=0; index<gen_power_plan_stations.num; index++)
	{
		gen_power_plan_stations.stations[index].num=0;
	}

	gen_power_plan_stations.num=0;

	xmlKeepBlanksDefault(0);
	pDoc = xmlReadFile (FILE_OIX_CONFIG_XML, "GB2312", XML_PARSE_NOBLANKS);
	if (pDoc == NULL)
    {
		sprintf(err_str, "Fail to open %s or format error!", FILE_OIX_CONFIG_XML);
		Err_log(err_str);
		return -1;
    }
	
	pRoot = xmlDocGetRootElement (pDoc);
	if (pRoot == NULL)
    {
		sprintf(err_str, "%s not have root", FILE_OIX_CONFIG_XML);
		Err_log(err_str);
		return -1;
    }
	
	pNodeSubRoot = pRoot->xmlChildrenNode;
	while (pNodeSubRoot != NULL)
    {
		if (xmlStrcmp (pNodeSubRoot->name, BAD_CAST(XML_NODE_GEN_POWER_PLAN_STATIONS_STR)) == 0)
        {
			pNodeStation = pNodeSubRoot->xmlChildrenNode;
			while(pNodeStation != NULL)
			{
				if (xmlStrcmp (pNodeStation->name, BAD_CAST "Station") == 0)
				{
					index=gen_power_plan_stations.num++;
					strcpy(gen_power_plan_stations.stations[index].name, xmlGetProp(pNodeStation,BAD_CAST "name"));

					pNodePower = pNodeStation->xmlChildrenNode;
					while (pNodePower != NULL)
					{ 
						if(xmlStrcmp(pNodePower->name, BAD_CAST "Power") == 0)
						{
							powerIndex=gen_power_plan_stations.stations[index].num++;
							
							gen_power_plan_stations.stations[index].stations[powerIndex].id=
								atoi(xmlGetProp(pNodePower,BAD_CAST "id"));

							interval=atoi(xmlGetProp(pNodePower,BAD_CAST "interval"));
							if (interval<=0)
							{
								interval=15;
							}
							
							switch (interval)
							{
							case 5:
								gen_power_plan_stations.stations[index].stations[powerIndex].interval=2;
								break;
							case 15:
							default:
								gen_power_plan_stations.stations[index].stations[powerIndex].interval=3;
								break;
							}

							strcpy(gen_power_plan_stations.stations[index].stations[powerIndex].name, 
								xmlGetProp(pNodePower,BAD_CAST "name"));

							gen_power_plan_stations.stations[index].stations[powerIndex].pt.stn_id=
								(UCHAR)atoi(xmlGetProp(pNodePower,BAD_CAST "stn"));

							gen_power_plan_stations.stations[index].stations[powerIndex].pt.dev_id=
								(UCHAR)atoi(xmlGetProp(pNodePower,BAD_CAST "dev"));

							gen_power_plan_stations.stations[index].stations[powerIndex].pt.data_type=
								(UCHAR)atoi(xmlGetProp(pNodePower,BAD_CAST "datatype"));

							gen_power_plan_stations.stations[index].stations[powerIndex].pt.pt_id=
								(USHORT)atoi(xmlGetProp(pNodePower,BAD_CAST "pt"));
						}
						
						pNodePower = pNodePower->next;
					}
				}
				pNodeStation = pNodeStation->next;
			}
        }
		pNodeSubRoot = pNodeSubRoot->next;
    }
	
	xmlFreeDoc (pDoc);
	xmlCleanupParser();

	return gen_power_plan_stations.num;
}

void readRealCurvePt()
{
	short           i;
	POINTER         *ptr;
 
	FILE	*fp;
	
	DMS_COMMON	pdt;
	
	char strStn[STATION_LONGNAME_SIZE],strDev[GROUP_LONGNAME_SIZE];
	char strEntryName[ENTRY_LONGNAME_SIZE];	
	char strLongName[128];
	
	if ((fp = fopen(REAL_CURVE_FILE, "rb")) == NULL) 
	{
		printf("OIX:unable to open %s\n", REAL_CURVE_FILE);
		return;
	}
	
	if (fread(&RealCCurvePt,sizeof(CURVE_DEF),1,fp)!=1)
		printf("in updata GenPowerPlanCurve Unselect list fread error\n");
	fclose(fp);
#ifdef  SUN_OS
	Swap2Byte(&RealCCurvePt.curve_num);
#endif
	if( RealCCurvePt.curve_num > MAX_POINTER_NUM)
	{       
		gdk_beep();
		printf("Oix ::readRealCurvePt:  RealCCurvePt.curve_num=%d > MAX_POINTER_NUM \n", RealCCurvePt.curve_num);
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
		printf("in readRealCurvePt: dev_id=%d, data_type=%d, pt_id = %d\n",
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
	}
}

void *FetchGenPowerPlanDataFromSrv(void *arg)
{
	unsigned char curid=0;
	gen_power_plan_thread_exit_flag = FALSE;     
	while(  gen_power_plan_thread_exit_flag == FALSE )   
	{ 
        if(shm_gen_power_plan_curve == NULL)
		{
			printf ("OIX: error when  copy shm_gen_power_plan_curve to   gen_power_plan_curve_data!!!  ");	
			goto ExitThr;
		} 
		memcpy( &gen_power_plan_curve_data.data_curve,  &shm_gen_power_plan_curve->data_curve, 
			sizeof(HDB_CURVE)*MAX_CRT_NUM); 
		
		gen_power_plan_curve_data.connflag_curve=shm_gen_power_plan_curve->connflag_curve;
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

int InitGenPowerPlanShareMemory(int crt_n )
{
#ifdef  WINDOWS_OS
	HANDLE Hthread; 
#elif defined(SUN_OS)
	pthread_t 	tid;
	int res;
#endif
	if (get_hdb_shm()==FALSE)
	{
		printf("can't get shared memory\n");
		return FALSE;
	}
	
	shm_gen_power_plan_curve= shm_hdb_addr;
	
	for(;;){
		
		if(shm_gen_power_plan_curve->data_curve[crt_n].lockflag == IDLE_FLAG)	break;
		
		
		MySleep(10) ; 
	}  
	
	
	shm_gen_power_plan_curve->data_curve[crt_n].lockflag=LOCK_FLAG;
	shm_gen_power_plan_curve->data_curve[crt_n].querytype=0;
	shm_gen_power_plan_curve->data_curve[crt_n].num=0;
	shm_gen_power_plan_curve->data_curve[crt_n].queryflag=0;
	shm_gen_power_plan_curve->data_curve[crt_n].lockflag=IDLE_FLAG;
	
	
	/* ********  here will create  a thread    ******** */               
	printf(" \n Here will create   thread_ !!!!!()\n");
#ifdef SUN_OS	
	res =   pthread_create(&tid, NULL, FetchGenPowerPlanDataFromSrv, (void *)SLEEP_TIME);
	if (res == -1  ) 
	{	    
		printf ("Oix :Error when create thread res:d% !!!  \n",res);
		return(FALSE);
	}		
	else 
		printf("Oix : reporting that  thread  have create successfully\n" );	    
	
#endif	
	
#ifdef  WINDOWS_OS
	Hthread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)FetchGenPowerPlanDataFromSrv, 
		(void *)(SLEEP_TIME) , 0, NULL) ; 
	if(Hthread!=NULL)
		printf("Create WriteDb Success!\n");
	else
		printf("Create WriteDb failed!!! error \n");
#endif
	return TRUE;
}

void DestoryGenPowerPlanCurveShell(GtkWidget *widget,   gpointer data)
{
	int crt_n = GPOINTER_TO_INT(data);
	
	if( crt_n < 0 || crt_n >= MAX_CRT_NUM )
		return ; 
	
	g_message("\n quit Operating Chararcteristic Curve Configure !!!!! \n");
	
	gen_power_plan_thread_exit_flag=TRUE;

	if (gen_power_plan_curve_timer_id != 0)
	{
		g_source_remove(gen_power_plan_curve_timer_id);
		gen_power_plan_curve_timer_id  = 0 ;
	}
	
	gen_power_plan_curve[crt_n] = NULL ;
	
	nGenPowerPlanCurveRef--;
	
	return ;
}

void ExitGenPowerPlanCurveShell(GtkWidget *widget,   gpointer data)
{
	int crt_n = GPOINTER_TO_INT(data);
	if( crt_n < 0 || crt_n >= MAX_CRT_NUM)
	{
		printf("ExitOpCharacterCurveConfigShell Failed! crt_n is invalid!!");
		return;
	}

	gtk_widget_destroy(GTK_WIDGET(gen_power_plan_curve[crt_n])); 
}

void onGenPowerPlanOkBtnClick(GtkWidget *widget,   gpointer data)
{
	char strTime[22];

	time_t result;
	struct tm date;

	char *pConvert=0;

	gint crt_n=GPOINTER_TO_INT(data);
	if (crt_n<0 || crt_n>=MAX_CRT_NUM)
	{
		return;
	}

	if (!queryData.finished)
	{
		return;
	}

	memset(&date, 0, sizeof(struct tm));

	strcpy(strTime, gtk_entry_get_text(GTK_ENTRY(gen_power_plan_tm_entry)));
	
	sscanf(strTime , "%4d-%2d-%2d" , &date.tm_year,&date.tm_mon,&date.tm_mday);
	
	date.tm_year-=1900;
	date.tm_mon--;
	
	date.tm_isdst = -1 ;
	
	result = mktime(&date);
	if (result != (time_t) -1)
	{
		queryData.time=date;
		
		UpdateGenPowerPlanCurveWidget(data);
	}
	else
	{
		gdk_beep();
		popupErrorMsgBox(gen_power_plan_curve[crt_n], (pConvert=_toUtf8EX("OIX_CSTR_TIME_FORMAT_ERROR")));
		G_SAFE_FREE(pConvert);
	}
}

void onGenPowerPlanPreviousBtnClick(GtkWidget *widget,   gpointer data)
{
	char strText[22];

	if (!queryData.finished)
	{
		return;
	}

	queryData.time.tm_mday--;
	mktime(&queryData.time);

	sprintf(strText,"%04d-%02d-%02d",queryData.time.tm_year+1900, queryData.time.tm_mon+1, queryData.time.tm_mday);
	gtk_entry_set_text(GTK_ENTRY(gen_power_plan_tm_entry), strText);

	UpdateGenPowerPlanCurveWidget(data);
}

void onGenPowerPlanAfterBtnClick(GtkWidget *widget,   gpointer data)
{
	char strText[22];

	if (!queryData.finished)
	{
		return;
	}

	queryData.time.tm_mday++;
	mktime(&queryData.time);

	sprintf(strText,"%04d-%02d-%02d",queryData.time.tm_year+1900, queryData.time.tm_mon+1, queryData.time.tm_mday);
	gtk_entry_set_text(GTK_ENTRY(gen_power_plan_tm_entry), strText);

	UpdateGenPowerPlanCurveWidget(data);
}

void CreateGenPowerPlanCurvePowerMenu(GtkComboBox* pComboBox, gpointer data) 
{
	int	i, stn_id;
	gen_power_plan_power_stations_t* pPower;
	
	GtkTreeModel* pModel;
	GtkListStore* pStore;
	GtkTreeIter it;
	
	gint crt_n=GPOINTER_TO_INT(data);
	if (crt_n<0 || crt_n>=MAX_CRT_NUM)
	{
		return;
	}
	
	gtk_combo_box_set_active(GTK_COMBO_BOX(gen_power_plan_menu_power), -1);
	
	pStore=GTK_LIST_STORE(gtk_combo_box_get_model(GTK_COMBO_BOX(gen_power_plan_menu_power)));
	gtk_list_store_clear(pStore);
	
	pModel=gtk_combo_box_get_model(GTK_COMBO_BOX(gen_power_plan_menu_stn));
	if (!gtk_combo_box_get_active_iter(GTK_COMBO_BOX(gen_power_plan_menu_stn), &it))
	{
		return;
	}
	
	gtk_tree_model_get(pModel, &it, eStnId, &stn_id, -1);
	
	if(stn_id < gen_power_plan_stations.num)
	{
		pPower=&gen_power_plan_stations.stations[stn_id];
		for(i=0; i<=pPower->num; i++) 
		{  		
			if (strcmp(pPower->stations[i].name, "") != 0)
			{
				gtk_list_store_append (pStore, &it);
				gtk_list_store_set (pStore, &it, ePowerStnName, pPower->stations[i].name, ePowerStnId, i, -1);
			}
		}
	}
	
	gtk_combo_box_set_active(GTK_COMBO_BOX(gen_power_plan_menu_power), 0);
}

gboolean setGenPowerPlanQueryData()
{
	GtkTreeModel* pModel;
	GtkTreeIter it;
	
	int stn_id, power_id;
	gchar strText[128];

	double dStartTime;
	gchar *pConvert=0;

	pModel=gtk_combo_box_get_model(GTK_COMBO_BOX(gen_power_plan_menu_stn));
	if (!gtk_combo_box_get_active_iter(GTK_COMBO_BOX(gen_power_plan_menu_stn), &it))
	{
		return FALSE;
	}
	
	gtk_tree_model_get(pModel, &it, eStnId, &stn_id, -1);
	
	if(stn_id >= gen_power_plan_stations.num)
	{
		printf("UpdateGenPowerPlanMenuSelect: stn_id >= gen_power_plan_stations.num");
		return FALSE;
	}
	
	pModel=gtk_combo_box_get_model(GTK_COMBO_BOX(gen_power_plan_menu_power));
	if (!gtk_combo_box_get_active_iter(GTK_COMBO_BOX(gen_power_plan_menu_power), &it))
	{
		return FALSE;
	}
	
	gtk_tree_model_get(pModel, &it, eStnId, &power_id, -1);
	
	if(power_id >= gen_power_plan_stations.stations[stn_id].num)
	{
		printf("UpdateGenPowerPlanMenuSelect: power_id >= gen_power_plan_stations.stations[stn_id].num");
		return FALSE;
	}
	
	//保存查询状态
	queryData.stn_id=stn_id;
	queryData.power_id=power_id;

	sprintf(strText, "%s --- %s %s", gen_power_plan_stations.stations[stn_id].name, 
		gen_power_plan_stations.stations[stn_id].stations[power_id].name, 
		(pConvert=_toUtf8EX("OIX_CSTR_GEN_POWER_PLAN_CURVE")));
	G_SAFE_FREE(pConvert);
	gtk_curve_widget_set_text(&genPowerPlanCurveWidget, strText, eTextTitle);

	dStartTime=mktime(&queryData.time);
	gtk_curve_widget_set_time_scale(&genPowerPlanCurveWidget, dStartTime, dStartTime+24*60*60);

	queryData.finished=FALSE;

	return TRUE;
}

void WriteGenPowerPlanShareMem (gint crt_n)
{
	int i;

	struct tm when ;
	time_t now;

	char  longname[100] ;
	POINT_NAME  pname; 

	for(;;){
		
		if(shm_gen_power_plan_curve->data_curve[crt_n].lockflag == IDLE_FLAG)	break;
		MySleep(10) ; 
	} 
	
	shm_gen_power_plan_curve->data_curve[crt_n].lockflag = LOCK_FLAG  ;    /* IDLE_FLAG */

	shm_gen_power_plan_curve->data_curve[crt_n].querytype = 2;		//查询发电计划曲线
	
	when=queryData.time;
	sprintf(shm_gen_power_plan_curve->data_curve[crt_n].starttime, "%04d-%02d-%02d %02d:%02d:%02d" , 
		when.tm_year+1900, when.tm_mon+1, when.tm_mday, when.tm_hour, when.tm_min, when.tm_sec);

	when.tm_mday++;
	mktime(&when);

	sprintf(shm_gen_power_plan_curve->data_curve[crt_n].endtime, "%04d-%02d-%02d %02d:%02d:%02d" , 
		when.tm_year+1900, when.tm_mon+1, when.tm_mday, when.tm_hour, when.tm_min, when.tm_sec);
	
	for (i=0; i<eGenPowerCurveNum; i++)
	{
		shm_gen_power_plan_curve->data_curve[crt_n].seg[i].rscount=0;
		memset(&shm_gen_power_plan_curve->data_curve[crt_n].seg[i].data, 0,  sizeof(float) * MAX_SELECT_REC );
	}

	shm_gen_power_plan_curve->data_curve[crt_n].seg[eGenPowerPlanCurve].stnid=
		gen_power_plan_stations.stations[queryData.stn_id].stations[queryData.power_id].id;
	
	shm_gen_power_plan_curve->data_curve[crt_n].num = 1; 

	time(&now);  
	when = *localtime(&now);
	
	when.tm_hour=0;
	when.tm_min=0;
	when.tm_sec=0;
	
	if (mktime(&when) != mktime(&queryData.time))
	{
		if ( -1 !=  GetPtNameById( &gen_power_plan_stations.stations[queryData.stn_id].stations[queryData.power_id].pt,
			(char*)&longname, &pname ))
		{
			shm_gen_power_plan_curve->data_curve[crt_n].num++;

			sprintf(shm_gen_power_plan_curve->data_curve[crt_n].seg[eGenPowerRealCurve].logicalname, longname);
		}
	}

	shm_gen_power_plan_curve->data_curve[crt_n].queryflag=1;
	shm_gen_power_plan_curve->data_curve[crt_n].interval=
		gen_power_plan_stations.stations[queryData.stn_id].stations[queryData.power_id].interval;
	shm_gen_power_plan_curve->data_curve[crt_n].finishedFlag=0;
	shm_gen_power_plan_curve->data_curve[crt_n].lockflag= IDLE_FLAG;
}

gboolean adjustGenPowerPlanCurve(gpointer usr_data)
{
	gint nIndex, i;
	GCurve* pCurve;

	gint nInterval;

	int id;
	
	gdouble dXValue;
	gint nNumCurve;

	struct tm when ;
	time_t now;

	gdouble dCurTime, dStartTime;
	gint nNextPt;
	
	POINTER	*ptr, *pt;
	DMS_COMMON pdt;
	
	gint stn_id, power_id;
	
	gdouble Y_end, Y_start;
	gint nBufLen;
	gint nPtId;
	gchar *pConvert=0;

	gint crt_n=GPOINTER_TO_INT(usr_data);
	if (crt_n<0 || crt_n>=MAX_CRT_NUM)
	{
		return FALSE;
	}
	
	nNumCurve=gtk_curve_widget_get_curve_count(&genPowerPlanCurveWidget);
	if (nNumCurve <= 0)
	{
		return FALSE;
	}

	if (!queryData.finished)
	{
		if (gen_power_plan_curve_data.data_curve[crt_n].finishedFlag != 1)
		{
			if (gen_power_plan_curve_data.connflag_curve != 1)
			{
				gtk_curve_widget_set_text(&genPowerPlanCurveWidget, (pConvert=_toUtf8EX("OIX_CSTR_NO_CONNECTED")),
					eTextMessage);
				G_SAFE_FREE(pConvert);
				gtk_curve_widget_enable(&genPowerPlanCurveWidget, TRUE, eEnableMessageClose);

				queryData.finished=TRUE;
			}
			else
			{
				gtk_curve_widget_set_text(&genPowerPlanCurveWidget, (pConvert=_toUtf8EX("OIX_CSTR_QUERYING")),
					eTextMessage);
				G_SAFE_FREE(pConvert);
				gtk_curve_widget_enable(&genPowerPlanCurveWidget, FALSE, eEnableMessageClose);
			}
		}
		else
		{	
			queryData.finished=TRUE;

			dXValue=mktime(&queryData.time);
			
			for (nIndex=0; nIndex<gtk_curve_widget_get_curve_count(&genPowerPlanCurveWidget); nIndex++)
			{
				pCurve=gtk_curve_widget_get_curve(&genPowerPlanCurveWidget, nIndex);
				if (pCurve != NULL)
				{
					id=GPOINTER_TO_INT(gtk_curve_get_user_data(pCurve, 0));
					
					if (id == eGenPowerPlanCurve)
					{
						switch (gen_power_plan_stations.stations[queryData.stn_id].stations[queryData.power_id].interval)
						{
						case 2:
							nInterval=5;
						case 3:
						default:
							nInterval=15;
							break;
						}
					}
					else if (id == eGenPowerRealCurve)
					{
						nInterval=60;
					}
					else
					{
						break;
					}
					
					for (i=0; i<gen_power_plan_curve_data.data_curve[crt_n].seg[id].rscount; i++)
					{
						dGenPowerPlanCurveData[i]=gen_power_plan_curve_data.data_curve[crt_n].seg[id].data[i];
					}
					
					gtk_curve_set_data(pCurve, &dXValue , dGenPowerPlanCurveData, MAX_SELECT_REC, 0, 
						gen_power_plan_curve_data.data_curve[crt_n].seg[id].rscount, nInterval);
					gtk_curve_set_boundary(pCurve, FALSE, 0, 0);
				}
			}

			
			gtk_curve_widget_enable(&genPowerPlanCurveWidget, FALSE, eEnableMessage);
		}
	}

	time(&now);  
	when = *localtime(&now);
	
	when.tm_hour=0;
	when.tm_min=0;
	when.tm_sec=0;

	if (mktime(&when) == mktime(&queryData.time))
	{
		dCurTime=shm_real_curve->nTime;
		dStartTime=mktime(&queryData.time);

		nBufLen=(gint)(dCurTime-dStartTime);

		if (nBufLen > 0)
		{
			nBufLen=nBufLen<DATA_NUM? nBufLen:DATA_NUM;
			
			nNextPt=shm_real_curve->nextpt;
			for (nIndex=0; nIndex < gtk_curve_widget_get_curve_count(&genPowerPlanCurveWidget); nIndex++)
			{
				pCurve=gtk_curve_widget_get_curve(&genPowerPlanCurveWidget, nIndex);
				if (pCurve != NULL)
				{
					id=GPOINTER_TO_INT(gtk_curve_get_user_data(pCurve, 0));
					if (id != eGenPowerRealCurve)
					{
						continue;
					}

					stn_id=GPOINTER_TO_INT(gtk_curve_get_user_data(pCurve, 1));
					power_id=GPOINTER_TO_INT(gtk_curve_get_user_data(pCurve, 2));

					if ( ReadEntryById(&gen_power_plan_stations.stations[stn_id].stations[power_id].pt, &pdt) != -1)
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
					pt=&pdt.point;
					for (i=0; i<RealCCurvePt.curve_num;i++)
					{
						ptr=&RealCCurvePt.buffer[i];
						if (ptr->stn_id == pt->stn_id && ptr->dev_id == pt->dev_id && ptr->data_type == pt->data_type
							&& ptr->pt_id == pt->pt_id)
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
							dGenPowerPlanCurveData[i]=shm_real_curve->dat.fvalue[nPtId][(nNextPt-1-nBufLen+DATA_NUM+i)%(DATA_NUM)];
						}
					}
					else
					{
						for (i=0; i<nBufLen; i++)
						{
							dGenPowerPlanCurveData[i]=shm_real_curve->dat.status[nPtId][(nNextPt-1-nBufLen+DATA_NUM+i)%(DATA_NUM)];
						}
					}
					
					gtk_curve_set_data(pCurve, &dStartTime , dGenPowerPlanCurveData, DATA_NUM, 0, nBufLen, 1);
					gtk_curve_set_boundary(pCurve, TRUE, Y_start, Y_end);
				}
			}
		}
	}
	else
	{
		if (queryData.finished)
		{
			if (gen_power_plan_curve_timer_id != 0)
			{
				g_source_remove(gen_power_plan_curve_timer_id);
				gen_power_plan_curve_timer_id  = 0 ;
			}
		}
	}
	
	gtk_curve_widget_frame_redraw(&genPowerPlanCurveWidget);
	
	return TRUE;
}

void UpdateGenPowerPlanCurveWidget(gpointer data)
{
	GCurve* pCurve;

	int i;
	char text[128];
	char *pConvert=0;

	gint crt_n=GPOINTER_TO_INT(data);
	if (crt_n<0 || crt_n>=MAX_CRT_NUM)
	{
		return;
	}

	gtk_curve_widget_clear_all_curve(&genPowerPlanCurveWidget);

	setGenPowerPlanQueryData();
	
	WriteGenPowerPlanShareMem (crt_n);
	
	gtk_curve_widget_canvas_zoom_100(&genPowerPlanCurveWidget);
	
	for (i=0; i<eGenPowerCurveNum; i++)
	{
		if (i == eGenPowerPlanCurve)
		{
			pCurve=gtk_curve_create(MAX_SELECT_REC, eModeMin);
			gtk_curve_set_type(pCurve, eCurveLadder);
		}
		else if (i == eGenPowerRealCurve)
		{
			pCurve=gtk_curve_create(DATA_NUM, eModeSec);
		}
		
		gtk_curve_set_color(pCurve, &pixels[crt_n][gen_power_plan_curve_color_group[i]]);
		
		sprintf(text, "%s -- %s %s", gen_power_plan_stations.stations[queryData.stn_id].name, 
			gen_power_plan_stations.stations[queryData.stn_id].stations[queryData.power_id].name, 
			(pConvert=_toUtf8EX(gen_power_plan_curve_text[i])));
		G_SAFE_FREE(pConvert);
		gtk_curve_set_text(pCurve, text);
		
		gtk_curve_set_user_data(pCurve, GINT_TO_POINTER(i), 0);

		if (i == eGenPowerRealCurve)
		{
			gtk_curve_set_user_data(pCurve, GINT_TO_POINTER(queryData.stn_id), 1);
			gtk_curve_set_user_data(pCurve, GINT_TO_POINTER(queryData.power_id), 2);
		}
		
		gtk_curve_widget_add_curve(&genPowerPlanCurveWidget, pCurve);
	}

	if (gen_power_plan_curve_timer_id == 0)
	{
		gen_power_plan_curve_timer_id =g_timeout_add (1000, (GSourceFunc)adjustGenPowerPlanCurve, GINT_TO_POINTER(crt_n));
	}
	
	gtk_curve_widget_set_text(&genPowerPlanCurveWidget, (pConvert=_toUtf8EX("OIX_CSTR_QUERYING")), eTextMessage);
	G_SAFE_FREE(pConvert);
	gtk_curve_widget_enable(&genPowerPlanCurveWidget, TRUE, eEnableMessage);
	gtk_curve_widget_enable(&genPowerPlanCurveWidget, FALSE, eEnableMessageClose);

	gtk_curve_widget_frame_redraw(&genPowerPlanCurveWidget);
}

void onToggledCheckBtnGenPowerPlanCurveMark(GtkToggleButton *widget, gpointer data)
{
	gboolean bCheck=gtk_toggle_button_get_active(widget);
	
	if (bCheck)
	{
		gtk_curve_widget_enable(&genPowerPlanCurveWidget, TRUE, eEnableCurvesMark);
	}
	else
	{
		gtk_curve_widget_enable(&genPowerPlanCurveWidget, FALSE, eEnableCurvesMark);
	}
	
	gtk_curve_widget_canvas_redraw(&genPowerPlanCurveWidget);
}

GtkWidget* CreateGenPowerPlanCurveToolbar(GtkWidget * parent, gint crt_n)
{
	GtkWidget *box;  		
	GtkWidget *toolbar; 
	GtkWidget *label;
	GtkWidget *pBtnPrevious, *pBtnAfter, *pBtnOk, *pBtnExit;
	GtkWidget *pCheckBtnMark;

	GtkListStore *pStoreStn, *pStorePower;
	GtkTreeIter it;
	
	GtkCellRenderer *pRenderStn, *pRenderPower;

	gchar *pConvert=0;

	int	i;

	if( crt_n < 0 || crt_n >= MAX_CRT_NUM)
	{
		printf("CreateGenPowerPlanCurveToolbar Failed!");
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
	
	for(i=0; i<=gen_power_plan_stations.num; i++)
	{ 
		if (strcmp(gen_power_plan_stations.stations[i].name, "") != 0)
		{
			gtk_list_store_append(pStoreStn, &it);
			gtk_list_store_set(pStoreStn, &it, eStnName,  gen_power_plan_stations.stations[i].name, 
				eStnId, i, -1);
		}
	}
	
	gen_power_plan_menu_stn=gtk_combo_box_new_with_model(GTK_TREE_MODEL(pStoreStn));
	
	pRenderStn = gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(gen_power_plan_menu_stn), pRenderStn, FALSE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(gen_power_plan_menu_stn), pRenderStn, "text", eStnName, NULL);
	g_object_unref(pStoreStn);
	
	gtk_widget_set_usize(gen_power_plan_menu_stn, 100, -1);
	gtk_box_pack_start(GTK_BOX(box), gen_power_plan_menu_stn, FALSE, FALSE, 5);
	gtk_widget_show(gen_power_plan_menu_stn);
	
	g_signal_connect (G_OBJECT(gen_power_plan_menu_stn), "changed", 
		G_CALLBACK(CreateGenPowerPlanCurvePowerMenu), GINT_TO_POINTER(crt_n));

	//电站
	label = gtk_label_new ((pConvert=_toUtf8EX("OIX_CSTR_POWER_STATION_FILTER")));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start (GTK_BOX (box), label, FALSE, FALSE, 0);
	gtk_widget_show (label);
	
	pRenderPower = gtk_cell_renderer_text_new();
	
	pStorePower=gtk_list_store_new(ePowerStnColNum, G_TYPE_STRING, G_TYPE_INT);
	gen_power_plan_menu_power = gtk_combo_box_new_with_model(GTK_TREE_MODEL(pStorePower));
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(gen_power_plan_menu_power), pRenderPower, FALSE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(gen_power_plan_menu_power), pRenderPower, 
		"text", ePowerStnName, NULL);
	g_object_unref(pStorePower);
	
	gtk_widget_set_usize(gen_power_plan_menu_power, 130, -1);
	gtk_box_pack_start (GTK_BOX (box), gen_power_plan_menu_power, FALSE, FALSE, 5);
	gtk_widget_show (gen_power_plan_menu_power);

	//日期
	label = gtk_label_new ((pConvert=_toUtf8EX("OIX_CSTR_DATE")));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start (GTK_BOX (box), label, FALSE, FALSE, 0);
	gtk_widget_show (label);
	
	gen_power_plan_tm_entry = gtk_entry_new_with_max_length (50);
	
	gtk_widget_set_usize (gen_power_plan_tm_entry, 130, -1);
	gtk_entry_set_position(GTK_ENTRY (gen_power_plan_tm_entry),0);
	gtk_entry_set_editable(GTK_ENTRY (gen_power_plan_tm_entry),TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(gen_power_plan_tm_entry),TRUE);
	gtk_box_pack_start (GTK_BOX (box), gen_power_plan_tm_entry, FALSE, FALSE, 0);
	gtk_widget_show (gen_power_plan_tm_entry);

	//确定按钮
	pBtnOk= gtk_button_new_with_label((pConvert=_toUtf8EX("OIX_CSTR_OK")));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start (GTK_BOX (box), pBtnOk, FALSE, FALSE, 5); 
	gtk_widget_show (pBtnOk);

	g_signal_connect(G_OBJECT(pBtnOk), "clicked", G_CALLBACK(onGenPowerPlanOkBtnClick), GINT_TO_POINTER(crt_n));

	//前一日、后一日
	pBtnPrevious= gtk_button_new_with_label((pConvert=_toUtf8EX("OIX_CSTR_PREVIOUS")));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start (GTK_BOX (box), pBtnPrevious, FALSE, FALSE, 5); 
	gtk_widget_show (pBtnPrevious);
	
	g_signal_connect(G_OBJECT(pBtnPrevious), "clicked", G_CALLBACK(onGenPowerPlanPreviousBtnClick), GINT_TO_POINTER(crt_n));
	
	pBtnAfter= gtk_button_new_with_label((pConvert=_toUtf8EX("OIX_CSTR_NEXT")));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start (GTK_BOX (box), pBtnAfter, FALSE, FALSE, 5); 
	gtk_widget_show (pBtnAfter);
	
	g_signal_connect(G_OBJECT(pBtnAfter), "clicked", G_CALLBACK(onGenPowerPlanAfterBtnClick), GINT_TO_POINTER(crt_n));
	
	gtk_curve_widget_add_scale_toolbox(&genPowerPlanCurveWidget, box, parent);
	
	pCheckBtnMark=gtk_check_button_new_with_label((pConvert=_toUtf8EX("OIX_CSTR_MARK")));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_start (GTK_BOX (box), pCheckBtnMark, FALSE, FALSE, 5); 
	gtk_widget_show(pCheckBtnMark);
	g_signal_connect(G_OBJECT(pCheckBtnMark), "toggled", G_CALLBACK(onToggledCheckBtnGenPowerPlanCurveMark), NULL);
	
    pBtnExit = gtk_button_new_with_label((pConvert=_toUtf8EX(OIX_CSTR_EXIT)));
	G_SAFE_FREE(pConvert);
	gtk_box_pack_end (GTK_BOX (box), pBtnExit, FALSE, FALSE, 0);
	gtk_widget_show (pBtnExit);
	
	g_signal_connect(G_OBJECT(pBtnExit), "clicked", G_CALLBACK(ExitGenPowerPlanCurveShell), 
		GINT_TO_POINTER(crt_n));
	
	return (toolbar);
}

GtkWidget *CreateProgGenPowerPlanCurveShell (GtkWidget *parent , int crt_n )
{
	GtkWidget  *vbox;
	GtkWidget  *windows, *frame;
	GtkWidget  *toolbar;

	struct tm when ;
	time_t now;
	char strText[128];
	char *pConvert=0;
	double dStartTime;
	
#ifdef SUN_OS
	GdkScreen* pScreen;
#endif
	
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
	
	gtk_window_set_title(GTK_WINDOW(windows), (pConvert=_toUtf8EX("STR_GEN_POWER_PLAN_CURVE")));    //发电计划曲线
	G_SAFE_FREE(pConvert);
	gtk_window_set_transient_for(GTK_WINDOW(windows),GTK_WINDOW(parent));
	
	g_signal_connect(G_OBJECT(windows), "destroy", 
		G_CALLBACK(DestoryGenPowerPlanCurveShell), GINT_TO_POINTER(crt_n));
	
	vbox=gtk_vbox_new(FALSE, 5);
	gtk_container_set_border_width(GTK_CONTAINER(vbox), 0);
	gtk_container_add(GTK_CONTAINER(windows), vbox);
	gtk_widget_show(vbox);

	frame=gtk_curve_widget_create(windows, &genPowerPlanCurveWidget);
	
	toolbar =  CreateGenPowerPlanCurveToolbar(windows, crt_n);
	gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, TRUE, 0);
	gtk_widget_show(toolbar);

	time(&now);  
	when = *localtime(&now);
	
	when.tm_hour=0;
	when.tm_min=0;
	when.tm_sec=0;
	dStartTime=mktime(&when);
	queryData.time=when;

	sprintf(strText,"%04d-%02d-%02d",when.tm_year+1900,when.tm_mon+1,when.tm_mday);
	gtk_entry_set_text(GTK_ENTRY(gen_power_plan_tm_entry), strText);

	gtk_box_pack_start(GTK_BOX(vbox ), frame, TRUE, TRUE, 0);
	
	gtk_curve_widget_enable(&genPowerPlanCurveWidget, TRUE, eEnableAutoSubTitle);
	gtk_curve_widget_set_data_scale(&genPowerPlanCurveWidget, eCalcAuto, 0, 0);

	sprintf(strText, "%s", (pConvert=_toUtf8EX("OIX_CSTR_GEN_POWER_PLAN_CURVE")));
	G_SAFE_FREE(pConvert);
	gtk_curve_widget_set_text(&genPowerPlanCurveWidget, strText, eTextTitle);

	gtk_curve_widget_enable(&genPowerPlanCurveWidget, FALSE, eEnableAutoLabel);
	sprintf(strText, "%s", (pConvert=_toUtf8EX("OIX_CSTR_GEN_POWER_CURVE_UNIT")));
	G_SAFE_FREE(pConvert);
	gtk_curve_widget_set_text(&genPowerPlanCurveWidget, strText, eTextLabel);
	
	gtk_curve_widget_set_time_scale(&genPowerPlanCurveWidget, dStartTime, dStartTime+24*60*60);

	gtk_curve_widget_enable(&genPowerPlanCurveWidget, TRUE, eEnableScale);
	
	gtk_widget_show_all(windows);
	
	gtk_curve_widget_set_current_operation(&genPowerPlanCurveWidget, eCurveOpPointer);
	gtk_curve_widget_frame_redraw(&genPowerPlanCurveWidget);

	return windows;
}

void ProgGenPowerPlanCurve(GtkWidget *parent ,int crt_n )
{
	if (crt_n<0 ||crt_n>=MAX_CRT_NUM)
	{
		gdk_beep();
		return;
	}
	
	if (gen_power_plan_curve[crt_n]==(GtkWidget*)NULL)
	{
		if (nGenPowerPlanCurveRef >= 1)
		{
			gdk_beep(); 
			PopupMessageBox(parent , "OIX: Generate Power Curve Widget has opened !!");
			return;
		}
		nGenPowerPlanCurveRef++;

		if (InitGenPowerPlanShareMemory(crt_n )== FALSE)
		{    
			gdk_beep(); 
			PopupMessageBox(parent , "OIX: Open share Mem error!");
			
			nGenPowerPlanCurveRef--;
			return ; 
		} 

		if (readGenPowerPlanStations() <= 0)	//读取发电计划配置信息
		{
			printf("ProgGenPowerPlanCurve: No find stations in %s File", FILE_OIX_CONFIG_XML);
		}

		readRealCurvePt();		//读取实时库点

		queryData.finished=TRUE;

		gen_power_plan_curve[crt_n]  = CreateProgGenPowerPlanCurveShell(parent, crt_n);

		gtk_combo_box_set_active(GTK_COMBO_BOX(gen_power_plan_menu_stn), 0);

		gtk_widget_show_all(gen_power_plan_curve[crt_n] );

		onGenPowerPlanOkBtnClick(NULL, GINT_TO_POINTER(crt_n));
	}
	else
	{
		gtk_window_present(GTK_WINDOW(gen_power_plan_curve[crt_n]) );
	}
}
