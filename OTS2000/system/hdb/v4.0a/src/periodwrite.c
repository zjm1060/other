/********************************************************************************************************
FILENAME:	periodwrite.c

FUNCTION:	
	
	source code of  hdbscan module,include:
	(1) connect to history database
	(2) update some points  in real db to history database periodly
	(3) write alarm information to hdb

VERSION:	v4.0a

OPERATE SYSTEM:	window 2000, Unix

HISTORY:

2007.5.22	create this file for hdb		yyp
2007.12.25  update for new db structure		yyp
2011.03.19 added buffer area to resolve problem of missing minute data.		wayne
2011.03.23 correct datetime.tm_min in function UpdateHdbByPeriod_z through "sscanf".	wayne
2011.05.10 add some debug info because some pt can not be writed into min table         wayne
*********************************************************************************************************/
#include "../inc/hdbhead_all.h"

#define BUFSIZE 4//restirct the buffer size

#ifdef MSWIN32
 DWORD		ThreadID[2];
#endif

#ifdef SUN
 pthread_t	ThreadID[2], tid_terminated;
#endif

#define DEBUGSTR "%s(line:%d): "
#define DEBUGARGS __FILE__,__LINE__
/*
sprintf(log_str,DEBUGSTR"%s",DEBUGARGS,""); 
log_write(INF,1,log_str);
 */

void GenPeriodInfoTable();
void GetOnePeriodData(unsigned char second);
int CreatPeriodYcInfo( unsigned char stnid );
int UpdatePeriodInHDB(unsigned char interval);
int ClearPeriodData(unsigned char interval);
unsigned char GetPeriodInterval(POINTER *point);
void *PeriodWrite(void *arg);
void *GetPeriod(void *arg);
void GetIdFromHdb();
int CompPeriodData();
int InitPointInfo();

//added by wayne
struct yc_period_seg_z
{
	POINTER point;
	int id;					/* added by yyp 07.12.27 for hdb */
	char logicalname[50];
	unsigned char period;
	float realvalue;
	struct yc_period_data data_cur;
//	struct yc_period_data data_last;
};

typedef struct hdb_yc_period_z
{
	int num;
	int flag;
	char timestr[30];
	struct yc_period_seg_z seg[MAX_YC_NUM];
} HDB_YC_PERIOD_Z;


typedef HDB_YC_PERIOD_Z elemType;

//queue
struct sNode{
    elemType data;            /* 值域 */
    struct sNode *next;       /* 链接指针 */
};

struct queueLK{
    struct sNode *front;    /* 队首指针 */
    struct sNode *rear;        /* 队尾指针 */
};


void initQueue(struct queueLK *hq);
void enQueue(struct queueLK *hq, elemType *x);
void outQueue(struct queueLK *hq, elemType * outdata);
elemType peekQueue(struct queueLK *hq);
int emptyQueue(struct queueLK *hq);
void clearQueue(struct queueLK *hq);

int UpdateHdbByPeriod_z(cocictx *ctxptr, struct hdb_yc_period_z *p_data);
//end


//added for debug by wayne
//int sleepTmList[10]={140,23,36,33,34,14,12,78,25,36};
//int slpPt;

int quecount = 0;

HDB_YC_PERIOD_Z	prddata;
struct array_period_data a_data[MAX_YC_NUM];//added by wayne
struct queueLK zqlk;
//HDB_TREND	trenddata;
int UpdateFlag =FALSE ;
int IsValid_All = TRUE;
int AllWriteFlag = TRUE;
int InitFlag = FALSE;
int	HostOldStat, HostNewStat;
char hostname[20];
BOOL FirstMinute=TRUE;
BOOL FirstFiveMinute=TRUE;
BOOL FirstFifteenMinute=TRUE;
BOOL FirstOneHour=TRUE;
BOOL FirstOneDay=TRUE;
char LoginStr[50];
int	prdnum;
int trendnum;
/********************************************************************************************************
 main function
*********************************************************************************************************/

void main(int argc, char *argv[])
{	
	char log_str[256];
#ifdef SUN
	int 	res;
#else
	HANDLE res;
	wsa_startup();
#endif
	//wsa_startup();
	SetLogLevel(argc, argv);
#ifdef SUN
	signal(SIGKILL,sig_termp); 
	signal(SIGTERM,sig_termp);
	signal(SIGINT,sig_termp);
	signal(SIGPIPE,SIG_IGN);
#endif

/************************************************************************/
/* added by wayne 3.19                                                  */
/************************************************************************/
	initQueue(&zqlk);


/*	app_id = HDB_APP;*/
	gethostname(hostname,20); 	
  	ReadDefFile();
	sprintf(strPwd,"%s/%s@",strUsername,strPassword);
 	strcpy(LoginID,strPwd);	
 	strcat(LoginID,SIDSTR);
	strcpy(LoginStr,LoginID);
	if(MountDB(DB_NAME) == -1)
	{
		printf("==== periodwrite.c: call MountDB(DATABASE_NAME) error\n");		
		exit(0);
	}
	if(lan_init_all() == FALSE)
	{
		printf("==== periodwrite.c: call Lan_init_all() error\n");		
		exit(0);
	}
	/* added by yyp 07.05.29 */
	if(strcmp(OUTNET_HOST,hostname)==0)
	{
		g_wranyhost = TRUE;
	}
	if(GetPtConf()==FALSE)	
	{
		printf("\n==== periodwrite.c: GetPtConf() error!\n");
		exit(0);
	}
	else
	{
		printf("\n==== GetPtConf() successfully!\n");
	}
	
	if(get_hdb_shm()==FALSE)
	{
		exit(0);
	}	
		
	GenPeriodInfoTable();
	if(InitPointInfo()==FALSE)
	{
		exit(0);
	}

	for(;;)
	{
		if( ConnectHDB_B(&ctx,SIDSTR,strUsername,strPassword)==FALSE )
		{	
			sprintf(log_str,"\n==== Connect '%s' failed ==== \n",SIDSTR);
			err_write(log_str); 
			HdbConn = FALSE;
			MySleep(10000);
			continue; 
		}
		else
		{	
			sprintf(log_str,"\n==== Connect '%s' Success  ====\n",SIDSTR); 
			log_write(INF,1,log_str);
			HdbConn = TRUE;
			break;
		}
	}


	/* GetPeriod(NULL);  */

#ifdef SUN	
	/************  create thread  ************/
	res =   thr_create(NULL, MAX_HDB_STACK_SIZE, GetPeriod, NULL, 0, &ThreadID[0]);
/*	if (res != -1  ) 
	{	    
		sprintf(log_str,"==== periodwrite.c: Create GetPeriod thread %d  OK",(int)ThreadID[0]);	
		log_printf(INF,1,HDB_APP,log_str);
	}		
	else
	{ 
		sprintf(log_str,"==== periodwrite.c: Create GetPeriod thread failed");	
		log_printf(INF,1,HDB_APP,log_str);
	}*/
	MySleep(200);
	
	res =   thr_create(NULL, MAX_HDB_STACK_SIZE, PeriodWrite, NULL, 0, &ThreadID[1]);
/*	if (res != -1  ) 
	{	    
		sprintf(log_str,"==== periodwrite.c: Create PeriodWrite thread %d  OK",(int)ThreadID[1]);	
		log_printf(INF,1,HDB_APP,log_str);
	}		
	else
	{ 
		sprintf(log_str,"==== periodwrite.c: Create PeriodWrite thread failed");	
		log_printf(INF,1,HDB_APP,log_str);
	}*/
	MySleep(2000);
#else
	res =  CreateThread(NULL, MAX_HDB_STACK_SIZE, (LPTHREAD_START_ROUTINE)GetPeriod, NULL, 0, &ThreadID[0]);
/*	if (res != NULL  ) 
	{	    
		sprintf(log_str,"==== periodwrite.c: Create GetPeriod thread %d  OK",(int)ThreadID[0]);	
		log_printf(INF,1,HDB_APP,log_str);
	}		
	else
	{ 
		sprintf(log_str,"==== periodwrite.c: Create GetPeriod thread failed");	
		log_printf(INF,1,HDB_APP,log_str);
	}*/
	MySleep(200);
	
	res =   CreateThread(NULL, MAX_HDB_STACK_SIZE, (LPTHREAD_START_ROUTINE)PeriodWrite, NULL, 0, &ThreadID[1]);
	/*if (res != NULL  ) 
	{	    
		sprintf(log_str,"==== periodwrite.c: Create PeriodWrite thread %d  OK",(int)ThreadID[1]);	
		log_printf(INF,1,HDB_APP,log_str);
	}		
	else
	{ 
		sprintf(log_str,"==== periodwrite.c: Create PeriodWrite thread failed");	
		log_printf(INF,1,HDB_APP,log_str);
	}*/
	MySleep(2000);
#endif	
	for(;;)
	{
#ifdef SUN
		pause();
#else
		MySleep(1000);
#endif
	}
}

/********************************************************************************************************
 PeriodWrite function
*********************************************************************************************************/

void *PeriodWrite(void *arg)
{	
	char log_str[256];
	DATE_TIME curtime;
	unsigned char hour,minute;
	int acc_link=0;
	printf("==== Start PeriodWrite Thread\n");
	GetSysTime(&curtime);
	hour = curtime.hour;
 	for(;;)
	{	
		MySleep(200);
		GetSysTime(&curtime);
		minute = curtime.minute;
		HostNewStat=GetLocalStat( hostname );
		if(g_wranyhost == TRUE)
		{
			HostNewStat = CUR_WS_IN_HOST;
		} 
		if( HostNewStat!=CUR_WS_IN_HOST )
		{
			sprintf(log_str,"periodwrite.c: %s hoststate=%d is not primary!",hostname,HostNewStat);
			log_write(INF,1,log_str);
			MySleep(10000);	
			continue;
		}
		if(quecount>1000)
			quecount = 0;
		
		//if(prddata.flag ==TRUE)
		if(!emptyQueue(&zqlk))
		{	
			//prddata.flag =FALSE;			
			MySleep(200);
			sprintf(log_str,DEBUGSTR"begain to UpdatePeriodInHDB!\n",DEBUGARGS);
			log_write(INF,1,log_str);
			UpdatePeriodInHDB(ONE_MIN);
			if(/*hour != curtime.hour || */shm_hdb_addr->readpt_period == 1 )
			{
				shm_hdb_addr->readpt_period = 0;
				hour = curtime.hour;
				GenPeriodInfoTable();
				if(InitPointInfo()==FALSE)
				{
					exit(0);
				}
			}
		}
		if( (++acc_link)>=10000 || HdbConn==FALSE )
		{	
			DisconnectHDB_B(&ctx);
			HdbConn=FALSE;
			for(;;)
			{
				if( ConnectHDB_B(&ctx,SIDSTR,strUsername,strPassword)==FALSE )
				{	
					sprintf(log_str,"\n==== Connect '%s' failed ==== \n",SIDSTR);
					err_write(log_str); 
					HdbConn = FALSE;
					MySleep(10000);
					continue; 
				}
				else
				{	
					sprintf(log_str,"\n==== Connect '%s' Success  ====\n",SIDSTR); 
					log_write(INF,1,log_str);
					HdbConn = TRUE;
					break;
				}
			}	

#ifdef SUN
			signal(SIGINT,sig_termp);
#endif
			acc_link=0;
		}
	}
}

/********************************************************************************************************
 Get Period Data from RDB function
*********************************************************************************************************/

void *GetPeriod(void *arg)
{	
	DATE_TIME curtime;
	int i;
	unsigned char hour,minute,second,day;

	struct tm timetmp;

	GetSysTime(&curtime);
	day = curtime.day;
	hour = curtime.hour;
	minute = curtime.minute;
	second = curtime.second;
	printf("==== Start GetPeriod Thread, wait for a minute...\n");
	ClearPeriodData(0);
 	for(;;)
	{	
		MySleep(50);
		HostNewStat=GetLocalStat( hostname );
		if(g_wranyhost == TRUE)
		{
			HostNewStat = CUR_WS_IN_HOST;
		}			
		if( HostNewStat!=CUR_WS_IN_HOST )
		{
			continue;
		}		
		GetSysTime(&curtime);
		if(curtime.minute!=minute)
		{
			minute = curtime.minute;
			hour = curtime.hour;
			day = curtime.day;
			if(FirstMinute == TRUE)
			{
				FirstMinute = FALSE;
				for(i=0;i<prddata.num;i++)
				{
					prddata.seg[i].data_cur.startdata = prddata.seg[i].realvalue;
					prddata.seg[i].data_cur.max = prddata.seg[i].data_cur.startdata;
					prddata.seg[i].data_cur.min = prddata.seg[i].data_cur.startdata;				
				}
				ClearPeriodData(ONE_MIN);
				continue;
			}

			sprintf(prddata.timestr,"%4d-%02d-%02d %02d:%02d:%02d",curtime.year,
						curtime.month,curtime.day,curtime.hour,curtime.minute,0);
		//	printf("%s\n",prddata.timestr);
			StrToTime(prddata.timestr,&timetmp,ONE_MIN,1);
			DateMinus(&timetmp,ONE_MIN);
			tmtostring(timetmp, prddata.timestr);
		//	printf("%s\n",prddata.timestr);

			if(quecount>=BUFSIZE){
				clearQueue(&zqlk);
				quecount = 0;
			}

			enQueue(&zqlk,&prddata);
			quecount++;

			for(i=0;i<prddata.num;i++)
			{
//				prddata.seg[i].data_last = prddata.seg[i].data_cur;//deleted by wayne
				prddata.seg[i].data_cur.startdata = prddata.seg[i].realvalue;
				prddata.seg[i].data_cur.max = prddata.seg[i].data_cur.startdata;
				prddata.seg[i].data_cur.min = prddata.seg[i].data_cur.startdata;
			}
			/* added by 08.03.17 */
			MySleep(400);
		//	prddata.flag = TRUE;
			ClearPeriodData(ONE_MIN);
			/* PeriodWrite(NULL); */

		} /* end of if(curtime.minute!=minute) */

		/* modified by yyp 07.04.28*/
		if(curtime.second != second)
		{
			second = curtime.second;
			//hour =  curtime.hour;
			GetOnePeriodData(second);//modified by wayne

			/* added by yyp 07.07.06 */
			if( curtime.second == 0 )
			{
				for(i=0;i<prddata.num;i++)
				{
					if(prddata.seg[i].period == ONE_MIN)
					{
						prddata.seg[i].data_cur.startdata = prddata.seg[i].realvalue;
					}
				}
			}/* end of if(curtime.second == 0) */

		}/* end of if (curtime.second != second) */
	}/* end of for() */
	
}


/********************************************************************************************************
 Write Point Info to file
*********************************************************************************************************/

void GenPeriodInfoTable()
{	
	int	i;
	char log_str[256];
#ifdef DEBUG
	printf("\n==== start GenYcPeriodInfoTable() ====\n");
#endif	
	prddata.num = 0;
	for(i=1;i<=g_stn_num;i++)
	{

		if( g_cascade != YES )
		{
			if(i>1) break;
		}
		if( g_dev_num[i]!=0 )
		{
			printf("\n==== CYCPeriod Station %d  (%d--%d) \n", i,1, g_dev_num[i]);
			
			if( CreatPeriodYcInfo( (unsigned char)i )==FALSE ) 
			{
				break;
			}	
		}
	}
	
	sprintf(log_str,"\n==== All PeriodNum==%d\n",prddata.num);
	log_write(INF,1,log_str);
}


/********************************************************************************************************
 Get Id from  Hdb
*********************************************************************************************************/
void GetIdFromHdb()
{
	int i,ptnum;
	char log_str[256];
	ptnum = prddata.num;
	SelectIdFromHdb(cda_cvw);
	for(i=0;i<ptnum;i++)
	{
		prddata.seg[i].id= -1;
		if(GetHdbIDByName((unsigned int *)&(prddata.seg[i].id),prddata.seg[i].logicalname)==FALSE)
		{
			sprintf(log_str,"\nGetHdbIDByName( %s ) error",prddata.seg[i].logicalname);
			err_write(log_str);
			continue;
		}
	}
}

/********************************************************************************************************
 create period Yc point def file
*********************************************************************************************************/

int CreatPeriodYcInfo( unsigned char StnId )
{	
	int			DevId,	DataType,	PointId,	Number;
	char			lname[100];
	POINTER			point;
	POINT_NAME		pt_name;
 	DB_DATA			db_data;
 	unsigned short	num;
 	struct tm		datetime;
 	long			t;
 	int 			Start,End;
	char			log_str[256];
 	Start = 1;
 	End = g_dev_num[StnId];
 	t=time(NULL); datetime=*localtime((long *)&t);
	num=prddata.num;
	for(DevId=Start;DevId<=End;DevId++)
	{		
#ifndef MULTIDEV
		if(DevId>1) break;
#endif
		//added by yyp 2011-01-10 for sx
	//	if(DevId>=DD_LCU_START)
	//	{
	//		break;
	//	}
		for(DataType=1;DataType<=MAX_DATATYPE_NUM;DataType++)
		{
			/* modified by yyp 07.05.28 */
			if( DataType==ANA_TYPE  || DataType==PARAM_TYPE || DataType==IMP_TYPE )
			{
				point.stn_id	  =	StnId;
				point.dev_id      =	DevId;
				point.data_type   =	DataType;
				point.pt_id    =	0;
				Number = GetPointNumber ( &point );
				for(PointId=0;PointId<Number;PointId++)
				{
					point.stn_id	  =	StnId;
					point.dev_id      =	DevId;
					point.data_type   =	DataType;
					point.pt_id    =	PointId;
					
					if(GetPtNameById( &point, lname, &pt_name ) ==-1)
					{
						continue;
					}
					if( ReadDBByName( lname, &db_data )!=TRUE )
					{
						sprintf(log_str,"No %s in RDB\n",lname);					
						err_write(log_str);
						continue;
					}
					/* added by 07.12.27 */
					if(IsPtIntoCurveHdb(&point) == FALSE)
					{
						continue;
					}
					prddata.seg[num].point  = point;	
					strcpy(prddata.seg[num].logicalname,lname); 					
					if( (++num) < MAX_YC_NUM )
					{
						continue; 
					}
					else
					{
						break;
					}
				}
			}
		}
	}
	prddata.num = num;
	return(TRUE);
}



/********************************************************************************************************
 Get Yc Changed Point data from real db
*********************************************************************************************************/
void GetOnePeriodData(unsigned char second)
{ 
	int		i;
	char log_str[256];
  	DB_DATA		db_data;
 	unsigned char	StnId =1;
	sprintf(log_str,"==== start GetOnePeriodData( %d ) PeriodNum=%d quecount=%d====\n",second,prddata.num,quecount);
	log_write(INF,1,log_str);
	
	for(i=0;i<prddata.num;i++)
	{
		if(ReadDBByName( prddata.seg[i].logicalname, &db_data )!=FALSE)
		{
			prddata.seg[i].realvalue= db_data.fvalue;
			
			if(second==0)
			{
				prddata.seg[i].data_cur.startdata = prddata.seg[i].realvalue;
				prddata.seg[i].data_cur.max = prddata.seg[i].data_cur.startdata;
				prddata.seg[i].data_cur.min = prddata.seg[i].data_cur.startdata;
			}
			prddata.seg[i].data_cur.sum += prddata.seg[i].realvalue;
			if(prddata.seg[i].realvalue > prddata.seg[i].data_cur.max)
			{
				prddata.seg[i].data_cur.max = prddata.seg[i].realvalue;
			}
			/* added by yyp 07.06.03 */
			if(prddata.seg[i].point.data_type == IMP_TYPE) 
			{
				prddata.seg[i].data_cur.max  = (float)db_data.LongValue;
			}

			if(prddata.seg[i].realvalue < prddata.seg[i].data_cur.min)
			{
				prddata.seg[i].data_cur.min = prddata.seg[i].realvalue;
			}
		}	
		else{
			sprintf(log_str,DEBUGSTR"%s(%d) can not find!",DEBUGARGS,prddata.seg[i].logicalname,prddata.seg[i].id);
			log_write(INF,1,log_str);
		}
	}
}

int ClearPeriodData(unsigned char interval )
{
	int		i;
 	unsigned 	StnId =1;
	
	for(i=0;i<prddata.num;i++)
	{
		prddata.seg[i].realvalue = 0 ; 
		/* prddata.seg[i].data_cur.startdata = 0 ; */
		/* prddata.seg[i].data_cur.max = 0 ; */
		/* prddata.seg[i].data_cur.min = 0 ; */
		prddata.seg[i].data_cur.sum = 0 ;
	}
	return TRUE;
}

int UpdatePeriodInHDB(unsigned char interval)
{
	DATE_TIME curtime;
 	unsigned int 		Start,End;
	int	total_len = 0;
 	unsigned 	StnId =1;
	char timestr[30];
	char log_str[256];
	HDB_YC_PERIOD_Z prddatatmp;//added by wayne
 	Start = 1;
	End = g_dev_num[StnId];
 	GetSysTime(&curtime);

	sprintf(timestr,"%4d-%2d-%2d %2d:%2d:%2d",curtime.year,curtime.month,curtime.day,curtime.hour,curtime.minute,curtime.second);

	sprintf(log_str,"==== UpdatePeriodInHDB: start UpdatePeriodInHDB(%d) time=%s quecount = %d",interval,timestr,quecount);
	log_write(INF,1,log_str);
	GetDevCond();
	
	outQueue(&zqlk,&prddatatmp);
	quecount--;

	if(UpdateHdbByPeriod_z(&ctx,&prddatatmp) !=TRUE )
	{
		HdbConn=FALSE;
		printf("InsertHdbByPeriod() error\n");
		return FALSE;
	}

	GetSysTime(&curtime);
 	sprintf(timestr,"%4d-%2d-%2d %2d:%2d:%2d",curtime.year,curtime.month,curtime.day,
 		curtime.hour,curtime.minute,curtime.second);

	sprintf(log_str,"==== UpdatePeriodInHDB:  end UpdatePeriodInHDB(%d) time=%s",interval, timestr);
	log_write(INF,1,log_str); 
	return TRUE;
}


unsigned char GetPeriodInterval(POINTER *point)
{
	unsigned int interval;
	if(GetPtHdbPeriodById( point, &interval ) == -1)
	{
		return 0;
	}
	return ((unsigned char)interval);
}

int InitPointInfo()
{
	char log_str[256];
	for(;;)
	{
		if( ConnectHDB(&lda_cvw,&cda_cvw,LoginStr)==FALSE )
		{	
			sprintf(log_str,"\n==== Connect '%s' failed ==== \n",SIDSTR);
			err_write(log_str); 
			MySleep(10000);
			continue; 
		}
		else
		{	
			sprintf(log_str,"\n==== Connect '%s' Success  ====\n",SIDSTR); 
			log_write(INF,1,log_str);
			break;
		}
	}

	SelectDevCondFromHdb(cda_cvw);
	GetDevCond();	
	GetIdFromHdb();

	DisconnectHDB(&lda_cvw,&cda_cvw);
#ifdef SUN
	signal(SIGINT,sig_termp);
#endif	
	return TRUE;
}

int UpdateHdbByPeriod_z(cocictx *ctxptr, struct hdb_yc_period_z *p_data)
{	
	char	FormatStr[200]="insert into ems.minute(id,min_datetime,min_data,min_max,min_min,min_avg) values(%d, '%s',%5.2f, %5.2f, %5.2f, %5.2f)";
	int		totalsec;
	//time_t t;
	struct tm datetime;
	char	UpdateStr[300];
//	char	timestr[30];
	int		i,rownum,iscond;
	ub4 valsk1 = (ub4) sizeof(struct array_period_data); /* value skip */
	ub4 indsk = 0; /*  indicator skips */
	ub4 rlsk = 0; /*  return length skips */
	ub4 rcsk = 0; /*  return code skips */
	text *ins_stmt1 = (text *)"INSERT INTO ems.minute(id,min_datetime,min_data,min_max,min_min,min_avg) VALUES(:1, :2,:3,:4,:5,:6)";
	OCIBind *bndp1 = (OCIBind *) NULL;
	OCIBind *bndp2 = (OCIBind *) NULL;
	OCIBind *bndp3 = (OCIBind *) NULL;
	OCIBind *bndp4 = (OCIBind *) NULL;
	OCIBind *bndp5 = (OCIBind *) NULL;
	OCIBind *bndp6 = (OCIBind *) NULL;

	totalsec = 60;
 //	t=time(NULL); 
//	datetime=*localtime((long *)&t);
//	DateMinus(&datetime,ONE_MIN);
//	datetime.tm_sec = 0;
//	tmtostring(datetime, timestr);
	sscanf(p_data->timestr,"%d-%d-%d %d:%d:%d",
	   &(datetime.tm_year),&(datetime.tm_mon),&(datetime.tm_mday),&(datetime.tm_hour),&(datetime.tm_min),&(datetime.tm_sec));
	rownum = 0;

	printf("\n%s\n",p_data->timestr);
	//for debug by wayne
	//printf("==== before insert min table,quecount=%d\n",quecount);
	//Sleep(1000*sleepTmList[slpPt]);
	//slpPt++;
	//if(slpPt == 10)
		//slpPt = 0;
	//end

	for(i=0;i<p_data->num;i++)
	{
		iscond = TRUE;
		GetHdbCondByName((unsigned int *)&(iscond),	p_data->seg[i].logicalname);
		if(cond[p_data->seg[i].point.stn_id][p_data->seg[i].point.dev_id].status == 1 
			&& iscond == TRUE && datetime.tm_min !=0  && g_wrcond == TRUE)  
		
		{
			continue;
		}
		if( p_data->seg[i].id<=0 )	
		{
			continue;
		}


		a_data[rownum].id = p_data->seg[i].id;

	
		strcpy(a_data[rownum].timestr,p_data->timestr);
		a_data[rownum].startdata  = p_data->seg[i].data_cur.startdata;
		a_data[rownum].max  = p_data->seg[i].data_cur.max;
		a_data[rownum].min  = p_data->seg[i].data_cur.min;
		a_data[rownum].avg   = p_data->seg[i].data_cur.sum/totalsec ;

		rownum++;
	}


	sprintf(UpdateStr, FormatStr, a_data[0].id,a_data[0].timestr,a_data[0].startdata, 
		a_data[0].max , a_data[0].min , a_data[0].avg);
	log_write(INF,3,UpdateStr);
	checkerr(ctxptr->errhp, OCIStmtPrepare(ctxptr->stmthp, ctxptr->errhp, 
                          ins_stmt1, (ub4) strlen((char *)ins_stmt1), 
                          (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT));

	checkerr(ctxptr->errhp, OCIBindByPos(ctxptr->stmthp, &bndp1, 
				  ctxptr->errhp, (ub4) 1, (dvoid *) &(a_data[0].id), 
				  (sb4) sizeof(a_data[0].id), SQLT_INT,
				  (dvoid *) 0, (ub2 *)0, (ub2 *)0,
				  (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT));

	checkerr(ctxptr->errhp, OCIBindByPos(ctxptr->stmthp, &bndp2, 
				  ctxptr->errhp, (ub4) 2, (dvoid *)a_data[0].timestr, 
				  (sb4) 22, SQLT_STR,
				  (dvoid *) 0, (ub2 *)0, (ub2 *)0,
				  (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT));

	checkerr(ctxptr->errhp, OCIBindByPos(ctxptr->stmthp, &bndp3, 
				  ctxptr->errhp, (ub4) 3, (dvoid *)&(a_data[0].startdata), 
				  (sb4) sizeof(a_data[0].startdata), SQLT_FLT,
				  (dvoid *) 0, (ub2 *)0, (ub2 *)0,
				  (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT));


	checkerr(ctxptr->errhp, OCIBindByPos(ctxptr->stmthp, &bndp4, 
				  ctxptr->errhp, (ub4) 4, (dvoid *)&(a_data[0].max), 
				  (sb4) sizeof(a_data[0].max), SQLT_FLT,
				  (dvoid *) 0, (ub2 *)0, (ub2 *)0,
				  (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT));

	checkerr(ctxptr->errhp, OCIBindByPos(ctxptr->stmthp, &bndp5, 
				  ctxptr->errhp, (ub4) 5, (dvoid *)&(a_data[0].min), 
				  (sb4) sizeof(a_data[0].min), SQLT_FLT,
				  (dvoid *) 0, (ub2 *)0, (ub2 *)0,
				  (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT));
	checkerr(ctxptr->errhp, OCIBindByPos(ctxptr->stmthp, &bndp6, 
				  ctxptr->errhp, (ub4) 6, (dvoid *)&(a_data[0].avg), 
				  (sb4) sizeof(a_data[0].avg), SQLT_FLT,
				  (dvoid *) 0, (ub2 *)0, (ub2 *)0,
				  (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT));

	if (OCIBindArrayOfStruct(bndp1,ctxptr->errhp,valsk1, sizeof(sb2), sizeof(ub2), sizeof(ub2))
	   || OCIBindArrayOfStruct(bndp2,ctxptr->errhp,valsk1, sizeof(sb2), sizeof(ub2), sizeof(ub2))
	   || OCIBindArrayOfStruct(bndp3,ctxptr->errhp,valsk1, sizeof(sb2), sizeof(ub2), sizeof(ub2))
	   || OCIBindArrayOfStruct(bndp4,ctxptr->errhp,valsk1, sizeof(sb2), sizeof(ub2), sizeof(ub2))
	   || OCIBindArrayOfStruct(bndp5,ctxptr->errhp,valsk1, sizeof(sb2), sizeof(ub2), sizeof(ub2))
	   || OCIBindArrayOfStruct(bndp6,ctxptr->errhp,valsk1, sizeof(sb2), sizeof(ub2), sizeof(ub2))
	   )
	  {
		printf("FAILED: OCIBindeArrayOfStruct()\n");
		return FALSE;
	  }    

	if(checkerr(ctxptr->errhp, OCIStmtExecute(ctxptr->svchp, ctxptr->stmthp, 
						  ctxptr->errhp, (ub4) rownum, (ub4)0,
						  (OCISnapshot *) NULL, (OCISnapshot *) NULL,
						  OCI_DEFAULT)) == FALSE)
	{
		HdbConn=FALSE;
	}
	
	checkerr(ctxptr->errhp, OCITransCommit(ctxptr->svchp, ctxptr->errhp, (ub4) 0));

	return(TRUE);
}
