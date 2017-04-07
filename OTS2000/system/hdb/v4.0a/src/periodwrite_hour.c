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

2011.03.25  modified from periodwrite.c used for write hour table in his     wayne
2011.04.14  delete "prddata_hour.seg[i].timestr_max_cur" and "prddata_hour.seg[i].timestr_min_cur" in GetOnePeriodData when hour time                        wayne
2011.04.14  add a new thread to correct the "lost hour"
2011.0428  modified the action time of collecting "lost hour"
2011.0504 clear the bug of record of max and min time

*********************************************************************************************************/
#include "../inc/hdbhead_all.h"

#define DEBUGSTR "%s(line:%d): "
#define DEBUGARGS __FILE__,__LINE__

#ifdef MSWIN32
 DWORD		ThreadID[2];
#endif

#ifdef SUN
 pthread_t	ThreadID[2], tid_terminated;
#endif

struct yc_period_seg_hour
{
	POINTER point;
	int id;					/* added by yyp 07.12.27 for hdb */
	char logicalname[50];
	unsigned char period;
	float realvalue;
	struct yc_period_data data_cur;
	struct yc_period_data data_last;
	struct yc_period_data data_lost;//added by wayne 11.4.18
	char timestr_max_cur[22];
	char timestr_max_last[22];
	char timestr_max_lost[22];//added by wayne 11.4.18
	char timestr_min_cur[22];
	char timestr_min_last[22];
	char timestr_min_lost[22];//added by wayne 11.4.18
};

typedef struct hdb_yc_hour
{
	int num;
	int flag;
	char timestr[22];
	struct yc_period_seg_hour seg[MAX_YC_NUM];
} HDB_YC_HOUR;

HDB_YC_HOUR	prddata_hour;//similar as prddata in periodwrite.c

void GenPeriodInfoTable();
void GetOnePeriodData(DATE_TIME * curtime);
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
int UpdateHdbOfHourTable(cocictx *ctxptr, struct hdb_yc_hour *p_data);
int Get_Pt_Hour_Property_From_Min_Table(int id, int ptNo, DATE_TIME * curtime);
void * GetDataOfLostHour(void * arg);

//HDB_YC_PERIOD	prddata;



HDB_TREND	trenddata;
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

BOOL FlaginThreadGetLostHour = TRUE;


struct array_hour_data
{
	int		id;
	char	timestr[22];
	float startdata;
	float max;
	float min;
	float avg;
	char	timestr_max[22];
	char	timestr_min[22];
};


struct array_hour_data a_data_hour[MAX_YC_NUM];

/********************************************************************************************************
 main function
*********************************************************************************************************/

void main(int argc, char *argv[])
{	
#ifdef SUN
	int 	res;
#else
	HANDLE res;
	wsa_startup();
#endif
	
	SetLogLevel(argc, argv);
#ifdef SUN
	signal(SIGKILL,sig_termp); 
	signal(SIGTERM,sig_termp);
	signal(SIGINT,sig_termp);
	signal(SIGPIPE,SIG_IGN);
#endif
//	app_id = HDB_APP;
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
			sprintf(log_str,"\n==== Connect '%s' failed in %s==== \n",SIDSTR,__FILE__);
			err_write(log_str); 
			HdbConn = FALSE;
			MySleep(10000);
			continue; 
		}
		else
		{	
			sprintf(log_str,"\n==== Connect '%s' Success in %s ====\n",SIDSTR,__FILE__); 
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
	/*if (res != -1) 
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

	res =   thr_create(NULL, MAX_HDB_STACK_SIZE, GetDataOfLostHour, NULL, 0, &ThreadID[2]);
/*	if (res != -1) 
	{
		sprintf(log_str,"==== periodwrite.c: Create GetDataOfLostHour thread %d  OK",(int)ThreadID[2]);	
		log_printf(INF,1,HDB_APP,log_str);
	}		
	else
	{
		sprintf(log_str,"==== periodwrite.c: Create GetDataOfLostHour thread failed");	
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
		sprintf(log_str,"==== periodwrite_hour.c: Create PeriodWrite thread %d  OK",(int)ThreadID[1]);	
		log_printf(INF,1,HDB_APP,log_str);
	}		
	else
	{ 
		sprintf(log_str,"==== periodwrite_hour.c: Create PeriodWrite thread failed");	
		log_printf(INF,1,HDB_APP,log_str);
	}*/
	MySleep(200);

	res =   CreateThread(NULL, MAX_HDB_STACK_SIZE, (LPTHREAD_START_ROUTINE)GetDataOfLostHour, NULL, 0, &ThreadID[2]);
	/*if (res != NULL  ) 
	{	    
		sprintf(log_str,"==== periodwrite_hour.c: Create GetDataOfLostHour thread %d  OK",(int)ThreadID[2]);	
		log_printf(INF,1,HDB_APP,log_str);
	}		
	else
	{ 
		sprintf(log_str,"==== periodwrite_hour.c: Create GetDataOfLostHour thread failed");	
		log_printf(INF,1,HDB_APP,log_str);
	}*/
	MySleep(200);


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

	DATE_TIME curtime;
	unsigned char hour,minute;
	int acc_link=0;

	struct tm timetmp;

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
			sprintf(log_str,"periodwrite.c: %s hoststate=%d is not primary!:%s",hostname,HostNewStat,__FILE__);
			log_write(INF,1,log_str);
			MySleep(10000);	
			continue;
		} 
		if(prddata_hour.flag ==TRUE)
		{	
			prddata_hour.flag =FALSE;			
			MySleep(200);

		//modified by wayne
			if(FlaginThreadGetLostHour == TRUE){
				sprintf(prddata_hour.timestr,"%4d-%02d-%02d %02d:%02d:%02d",curtime.year,
							curtime.month,curtime.day,curtime.hour,curtime.minute,0);
			//	printf("%s\n",prddata.timestr);
				StrToTime(prddata_hour.timestr,&timetmp,ONE_HOUR,1);
				DateMinus(&timetmp,ONE_HOUR);
				tmtostring(timetmp, prddata_hour.timestr);
			}
			else{
				FlaginThreadGetLostHour = TRUE;
			}
//end

			UpdatePeriodInHDB(ONE_HOUR);
			if(/*hour != curtime.hour ||*/ shm_hdb_addr->readpt_period == 1 )
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

	GetSysTime(&curtime);
	day = curtime.day;
	hour = curtime.hour;
	minute = curtime.minute;
	second = curtime.second;
//	printf("==== Start GetPeriod Thread, wait for a minute...\n");
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
		if(curtime.hour!=hour)
	//	if(curtime.minute % 5 == 0)
	//	if(curtime.minute != minute)
		{
			minute = curtime.minute;
			hour = curtime.hour;
			day = curtime.day;
			if(FirstMinute == TRUE)//init when run program
			{
				FirstMinute = FALSE;
				for(i=0;i<prddata_hour.num;i++)
				{
					prddata_hour.seg[i].data_cur.startdata = prddata_hour.seg[i].realvalue;
					prddata_hour.seg[i].data_cur.max = prddata_hour.seg[i].data_cur.startdata;
					prddata_hour.seg[i].data_cur.min = prddata_hour.seg[i].data_cur.startdata;
					sprintf(prddata_hour.seg[i].timestr_max_cur,"%4d-%02d-%02d %02d:%02d:%02d",curtime.year,curtime.month,  
		curtime.day,curtime.hour,curtime.minute,curtime.second);
					sprintf(prddata_hour.seg[i].timestr_min_cur,"%4d-%02d-%02d %02d:%02d:%02d",curtime.year,curtime.month,  
		curtime.day,curtime.hour,curtime.minute,curtime.second);
				}
				ClearPeriodData(ONE_MIN);
				continue;
			}
			for(i=0;i<prddata_hour.num;i++)
			{
				prddata_hour.seg[i].data_last = prddata_hour.seg[i].data_cur;
				prddata_hour.seg[i].data_cur.startdata = prddata_hour.seg[i].realvalue;
				prddata_hour.seg[i].data_cur.max = prddata_hour.seg[i].data_cur.startdata;
				prddata_hour.seg[i].data_cur.min = prddata_hour.seg[i].data_cur.startdata;
				strcpy(prddata_hour.seg[i].timestr_max_last,prddata_hour.seg[i].timestr_max_cur);
				strcpy(prddata_hour.seg[i].timestr_min_last,prddata_hour.seg[i].timestr_min_cur);
				sprintf(prddata_hour.seg[i].timestr_max_cur,"%4d-%02d-%02d %02d:%02d:%02d",curtime.year,curtime.month,  
		curtime.day,curtime.hour,curtime.minute,curtime.second);
				sprintf(prddata_hour.seg[i].timestr_min_cur,"%4d-%02d-%02d %02d:%02d:%02d",curtime.year,curtime.month,  
		curtime.day,curtime.hour,curtime.minute,curtime.second);
			}
			/* added by 08.03.17 */
			MySleep(400);
			prddata_hour.flag = TRUE;
			ClearPeriodData(ONE_HOUR);
			/* PeriodWrite(NULL); */

		} /* end of if(curtime.minute!=minute) */

		/* modified by yyp 07.04.28*/
		if(curtime.second != second)
		{
			second = curtime.second;
			printf("before GetOnePeriodData, time is %d \n", curtime.second);
			GetOnePeriodData(&curtime);

			/* added by yyp 07.07.06 *///update startdata
			if( curtime.minute == 0 )
			{
				for(i=0;i<prddata_hour.num;i++)
				{
					if(prddata_hour.seg[i].period == ONE_HOUR)
					{
						prddata_hour.seg[i].data_cur.startdata = prddata_hour.seg[i].realvalue;
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
#ifdef DEBUG
	printf("\n==== start GenYcPeriodInfoTable() ====\n");
#endif	
	prddata_hour.num = 0;
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
	
	sprintf(log_str,"\n==== All PeriodNum==%d :%s\n",prddata_hour.num,__FILE__);
	log_write(INF,1,log_str);
}


/********************************************************************************************************
 Get Id from  Hdb
*********************************************************************************************************/
void GetIdFromHdb()
{
	int i,ptnum;
	ptnum = prddata_hour.num;
	SelectIdFromHdb(cda_cvw);
	for(i=0;i<ptnum;i++)
	{
		prddata_hour.seg[i].id= -1;
		if(GetHdbIDByName((unsigned int *)&(prddata_hour.seg[i].id),prddata_hour.seg[i].logicalname)==FALSE)
		{
			sprintf(log_str,"\nGetHdbIDByName( %s ) error :%s",prddata_hour.seg[i].logicalname,__FILE__);
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
 	Start = 1;
 	End = g_dev_num[StnId];
 	t=time(NULL); datetime=*localtime((long *)&t);
	num=prddata_hour.num;
	for(DevId=Start;DevId<=End;DevId++)
	{		
#ifndef MULTIDEV
		if(DevId>1) break;
#endif
		//added by yyp 2011-01-10 for sx
		//if(DevId>=DD_LCU_START)
		//{
		//	break;
		//}
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
					prddata_hour.seg[num].point  = point;	
					strcpy(prddata_hour.seg[num].logicalname,lname); 					
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
	prddata_hour.num = num;
	return(TRUE);
}



/********************************************************************************************************
 Get Yc Changed Point data from real db
*********************************************************************************************************/
void GetOnePeriodData(DATE_TIME * curtime)//modified by wayne
{
	int		i;
  	DB_DATA		db_data;
 	unsigned char	StnId =1;
	sprintf(log_str,"==== start GetOnePeriodData( %d ) PeriodNum=%d ==== :%s\n",curtime->second,prddata_hour.num,__FILE__);
	log_write(INF,1,log_str);
	printf("------------GetOnePeriodData be called----------- \n");
	for(i=0;i<prddata_hour.num;i++)
	{
		if(ReadDBByName( prddata_hour.seg[i].logicalname, &db_data )!=FALSE)
		{
			prddata_hour.seg[i].realvalue= db_data.fvalue;
			if(curtime->minute==0&&curtime->second ==0)
			{
				prddata_hour.seg[i].data_cur.startdata = prddata_hour.seg[i].realvalue;
				prddata_hour.seg[i].data_cur.max = prddata_hour.seg[i].data_cur.startdata;
				prddata_hour.seg[i].data_cur.min = prddata_hour.seg[i].data_cur.startdata;
			//	sprintf(prddata_hour.seg[i].timestr_max_cur,"%4d-%02d-%02d %02d:%02d:%02d",curtime->year,curtime->month,  
			//		curtime->day,curtime->hour,curtime->minute,curtime->second);
			//	sprintf(prddata_hour.seg[i].timestr_min_cur,"%4d-%02d-%02d %02d:%02d:%02d",curtime->year,curtime->month,  
			//		curtime->day,curtime->hour,curtime->minute,curtime->second);
			}
			prddata_hour.seg[i].data_cur.sum += prddata_hour.seg[i].realvalue;
			if(prddata_hour.seg[i].realvalue > prddata_hour.seg[i].data_cur.max)
			{
				prddata_hour.seg[i].data_cur.max = prddata_hour.seg[i].realvalue;
				sprintf(prddata_hour.seg[i].timestr_max_cur,"%4d-%02d-%02d %02d:%02d:%02d",curtime->year,curtime->month,  
					curtime->day,curtime->hour,curtime->minute,curtime->second);
			}
			/* added by yyp 07.06.03 */
			if(prddata_hour.seg[i].point.data_type == IMP_TYPE) 
			{
				prddata_hour.seg[i].data_cur.max  = (float)db_data.LongValue;
				sprintf(prddata_hour.seg[i].timestr_max_cur,"%4d-%02d-%02d %02d:%02d:%02d",curtime->year,curtime->month,  
					curtime->day,curtime->hour,curtime->minute,curtime->second);

			}

			if(prddata_hour.seg[i].realvalue < prddata_hour.seg[i].data_cur.min)
			{
				prddata_hour.seg[i].data_cur.min = prddata_hour.seg[i].realvalue;
				sprintf(prddata_hour.seg[i].timestr_min_cur,"%4d-%02d-%02d %02d:%02d:%02d",curtime->year,curtime->month,  
					curtime->day,curtime->hour,curtime->minute,curtime->second);
			}
		}	
	}
}

int ClearPeriodData(unsigned char interval )
{
	int		i;
 	unsigned 	StnId =1;
	
	for(i=0;i<prddata_hour.num;i++)
	{
		prddata_hour.seg[i].realvalue = 0 ; 
		/* prddata.seg[i].data_cur.startdata = 0 ; */
		/* prddata.seg[i].data_cur.max = 0 ; */
		/* prddata.seg[i].data_cur.min = 0 ; */
		prddata_hour.seg[i].data_cur.sum = 0 ;
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
 	Start = 1;
	End = g_dev_num[StnId];
 	GetSysTime(&curtime);

	sprintf(timestr,"%4d-%2d-%2d %2d:%2d:%2d",curtime.year,curtime.month,curtime.day,curtime.hour,curtime.minute,curtime.second);

	sprintf(log_str,"==== UpdatePeriod_hour InHDB: start UpdatePeriodInHDB(%d) time=%s, :in %s\n",interval,timestr,__FILE__);
	printf("==== UpdatePeriodInHDB: start UpdatePeriodInHDB(%d) time=%s :%s",interval,timestr,__FILE__);
	log_write(INF,1,log_str);
	GetDevCond();
//	if(UpdateHdbByPeriod(&ctx,&prddata_hour) !=TRUE )
	if(UpdateHdbOfHourTable(&ctx,&prddata_hour) !=TRUE )
	{
		HdbConn=FALSE;
		printf("InsertHdbByPeriod() error\n");
		return FALSE;
	}

	GetSysTime(&curtime);
 	sprintf(timestr,"%4d-%2d-%2d %2d:%2d:%2d",curtime.year,curtime.month,curtime.day,
 		curtime.hour,curtime.minute,curtime.second);

	sprintf(log_str,"==== UpdatePeriodInHDB:  end UpdatePeriodInHDB(%d) time=%s :%s",interval, timestr,__FILE__);
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

int UpdateHdbOfHourTable(cocictx *ctxptr, struct hdb_yc_hour *p_data)
{	
	char	FormatStr[]="insert into ems.hour(id,hour_datetime,hour_data,hour_max,hour_min,hour_avg,max_time,min_time) values(%d, '%s',%5.2f, %5.2f, %5.2f, %5.2f, '%s', '%s')";
	int		totalsec;
	time_t t;
	struct tm datetime;
	char	UpdateStr[200];
//	char	timestr[30];
	int		i,rownum,iscond;
	ub4 valsk1 = (ub4) sizeof(struct array_hour_data); /* value skip */
	ub4 indsk = 0; /*  indicator skips */
	ub4 rlsk = 0; /*  return length skips */
	ub4 rcsk = 0; /*  return code skips */
	text *ins_stmt1 = (text *)"INSERT INTO ems.hour(id,hour_datetime,hour_data,hour_max,hour_min,hour_avg,max_time,min_time) VALUES(:1,:2,:3,:4,:5,:6,:7,:8)";
	OCIBind *bndp1 = (OCIBind *) NULL;
	OCIBind *bndp2 = (OCIBind *) NULL;
	OCIBind *bndp3 = (OCIBind *) NULL;
	OCIBind *bndp4 = (OCIBind *) NULL;
	OCIBind *bndp5 = (OCIBind *) NULL;
	OCIBind *bndp6 = (OCIBind *) NULL;
	OCIBind *bndp7 = (OCIBind *) NULL;
	OCIBind *bndp8 = (OCIBind *) NULL;

	//totalsec = 60;//5minute
	totalsec = 3600;
 	t=time(NULL); 
	datetime=*localtime((long *)&t);
//	DateMinus(&datetime,ONE_HOUR);
	
//	datetime.tm_hour = (datetime.tm_hour==0?23:(datetime.tm_hour - 1));

	datetime.tm_sec = 0;
//	tmtostring(datetime, p_data->timestr);
	rownum = 0;

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
		a_data_hour[rownum].id = p_data->seg[i].id;

		strcpy(a_data_hour[rownum].timestr,p_data->timestr);

 		a_data_hour[rownum].startdata  = p_data->seg[i].data_last.startdata;
 		a_data_hour[rownum].max  = p_data->seg[i].data_last.max;
 		a_data_hour[rownum].min  = p_data->seg[i].data_last.min;
 		a_data_hour[rownum].avg   = p_data->seg[i].data_last.sum/totalsec ;

 		strcpy(a_data_hour[rownum].timestr_max,p_data->seg[i].timestr_max_last);
 		strcpy(a_data_hour[rownum].timestr_min,p_data->seg[i].timestr_min_last);

		rownum++;
	}

	sprintf(UpdateStr, FormatStr, a_data_hour[0].id,a_data_hour[0].timestr,a_data_hour[0].startdata, 
		a_data_hour[0].max , a_data_hour[0].min , a_data_hour[0].avg, a_data_hour[0].timestr_max,a_data_hour[0].timestr_min);
	log_write(INF,1,UpdateStr);

	sprintf(log_str,DEBUGSTR"insert record num=%d,insert time %s",DEBUGARGS,rownum,p_data->timestr);
	log_write(INF,1,log_str);

	printf("\n%s\n",UpdateStr);
	
	checkerr(ctxptr->errhp, OCIStmtPrepare(ctxptr->stmthp, ctxptr->errhp, 
                          ins_stmt1, (ub4) strlen((char *)ins_stmt1), 
                          (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT));

	checkerr(ctxptr->errhp, OCIBindByPos(ctxptr->stmthp, &bndp1, 
				  ctxptr->errhp, (ub4) 1, (dvoid *) &(a_data_hour[0].id), 
				  (sb4) sizeof(a_data_hour[0].id), SQLT_INT,
				  (dvoid *) 0, (ub2 *)0, (ub2 *)0,
				  (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT));

	checkerr(ctxptr->errhp, OCIBindByPos(ctxptr->stmthp, &bndp2, 
				  ctxptr->errhp, (ub4) 2, (dvoid *)a_data_hour[0].timestr, 
				  (sb4) 22, SQLT_STR,
				  (dvoid *) 0, (ub2 *)0, (ub2 *)0,
				  (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT));

	checkerr(ctxptr->errhp, OCIBindByPos(ctxptr->stmthp, &bndp3, 
				  ctxptr->errhp, (ub4) 3, (dvoid *)&(a_data_hour[0].startdata), 
				  (sb4) sizeof(a_data_hour[0].startdata), SQLT_FLT,
				  (dvoid *) 0, (ub2 *)0, (ub2 *)0,
				  (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT));

	checkerr(ctxptr->errhp, OCIBindByPos(ctxptr->stmthp, &bndp4, 
				  ctxptr->errhp, (ub4) 4, (dvoid *)&(a_data_hour[0].max), 
				  (sb4) sizeof(a_data_hour[0].max), SQLT_FLT,
				  (dvoid *) 0, (ub2 *)0, (ub2 *)0,
				  (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT));

	checkerr(ctxptr->errhp, OCIBindByPos(ctxptr->stmthp, &bndp5, 
				  ctxptr->errhp, (ub4) 5, (dvoid *)&(a_data_hour[0].min), 
				  (sb4) sizeof(a_data_hour[0].min), SQLT_FLT,
				  (dvoid *) 0, (ub2 *)0, (ub2 *)0,
				  (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT));

	checkerr(ctxptr->errhp, OCIBindByPos(ctxptr->stmthp, &bndp6, 
				  ctxptr->errhp, (ub4) 6, (dvoid *)&(a_data_hour[0].avg), 
				  (sb4) sizeof(a_data_hour[0].avg), SQLT_FLT,
				  (dvoid *) 0, (ub2 *)0, (ub2 *)0,
				  (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT));

	checkerr(ctxptr->errhp, OCIBindByPos(ctxptr->stmthp, &bndp7, 
		ctxptr->errhp, (ub4) 7, (dvoid *)a_data_hour[0].timestr_max, 
		(sb4) 22, SQLT_STR,	(dvoid *) 0, (ub2 *)0, (ub2 *)0,
				  (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT));

	checkerr(ctxptr->errhp, OCIBindByPos(ctxptr->stmthp, &bndp8, 
		ctxptr->errhp, (ub4) 8, (dvoid *)a_data_hour[0].timestr_min, 
		(sb4) 22, SQLT_STR,	(dvoid *) 0, (ub2 *)0, (ub2 *)0,
				  (ub4) 0, (ub4 *) 0, (ub4) OCI_DEFAULT));

	if (OCIBindArrayOfStruct(bndp1,ctxptr->errhp,valsk1, sizeof(sb2), sizeof(ub2), sizeof(ub2))
	   || OCIBindArrayOfStruct(bndp2,ctxptr->errhp,valsk1, sizeof(sb2), sizeof(ub2), sizeof(ub2))
	   || OCIBindArrayOfStruct(bndp3,ctxptr->errhp,valsk1, sizeof(sb2), sizeof(ub2), sizeof(ub2))
	   || OCIBindArrayOfStruct(bndp4,ctxptr->errhp,valsk1, sizeof(sb2), sizeof(ub2), sizeof(ub2))
	   || OCIBindArrayOfStruct(bndp5,ctxptr->errhp,valsk1, sizeof(sb2), sizeof(ub2), sizeof(ub2))
	   || OCIBindArrayOfStruct(bndp6,ctxptr->errhp,valsk1, sizeof(sb2), sizeof(ub2), sizeof(ub2))
	   || OCIBindArrayOfStruct(bndp7,ctxptr->errhp,valsk1, sizeof(sb2), sizeof(ub2), sizeof(ub2))
	   || OCIBindArrayOfStruct(bndp8,ctxptr->errhp,valsk1, sizeof(sb2), sizeof(ub2), sizeof(ub2))
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

//added by wayne, this function can be called after half an hour
int Get_Pt_Hour_Property_From_Min_Table(int id,int ptNo, DATE_TIME * curtime){

	char	sqlstr[]="select MIN_MAX,MIN_MIN,MIN_AVG,MIN_DATETIME,MIN_DATA from ems.minute where id=%d and MIN_DATETIME>'%d-%d-%d %d:01:00' and MIN_DATETIME<='%d-%d-%d %d:01:00' order by min_datetime"; 
	//char	sqlstr[]="select MIN_MAX,MIN_MIN,MIN_AVG,MIN_DATETIME from ems.minute where id=%d and MIN_DATETIME>'%d-%d-%d %d:01:00' and MIN_DATETIME<='%d-%d-%d %d:01:00'"; 
	char	cmd[300];
	float min_data;
	float min_max;
	float min_min;
	float min_avg;
	float hour_sum;
	int searchResultNo;
	char min_datetime[30];
	char dttmp[22];
	
//	DATE_TIME curtime2;
	struct tm timetmp;

	char logtxt[150]="in %s at line %d: %s";

//	GetSysTime(&curtime2);

	


	sprintf(dttmp,"%4d-%02d-%02d %02d:%02d:%02d",curtime->year,
		curtime->month,curtime->day,curtime->hour,curtime->minute,0);
	//	printf("%s\n",prddata.timestr);
	StrToTime(dttmp,&timetmp,ONE_HOUR,1);
	DateAdd(&timetmp,ONE_HOUR);
		



	sprintf(cmd,sqlstr,id,curtime->year,curtime->month,curtime->day,curtime->hour,
		timetmp.tm_year+1900,timetmp.tm_mon+1,timetmp.tm_mday,timetmp.tm_hour);
//	printf("%s\n",cmd);
//	log_write(INF,1,cmd);


	if( oparse(&cda, (unsigned char *)cmd, (sb4) -1, (sword) TRUE, (ub4) 2) )
	{
		OracleError(&cda);
		err_write("Oparse Orcal error !\n");
		return(FALSE);
	}


	if (odefin(&cda, 1, (ub1 *) &min_max, (sword) sizeof(float), SQLT_FLT,
				-1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
	{
		OracleError(&cda);
		return(FALSE);   
	}
	
	if (odefin(&cda, 2, (ub1 *) &min_min, (sword) sizeof(float), SQLT_FLT,
				-1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
	{
		OracleError(&cda);
		return(FALSE);   
	}
	
	if (odefin(&cda, 3, (ub1 *) &min_avg, (sword) sizeof(float), SQLT_FLT,
				-1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
	{
		OracleError(&cda);
		return(FALSE);   
	}

	
	if (odefin(&cda, 4, (ub1 *) min_datetime, 30,SQLT_STR,
				-1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
	{
		OracleError(&cda);
		return(FALSE);
	}


	if (odefin(&cda, 5, (ub1 *) &min_data, (sword) sizeof(float), SQLT_FLT,
		-1, (sb2 *) -1, (text *)0, (sword) 0, (sword) 0,(ub2 *) 0, (ub2 *) 0))
	{
		OracleError(&cda);
		return(FALSE);   
	}

	if (oexec(&cda))
	{
		OracleError(&cda);
		return(FALSE);
	}

	prddata_hour.seg[ptNo].id = id;
	prddata_hour.seg[ptNo].data_lost.startdata = -999999.0;
	prddata_hour.seg[ptNo].data_lost.max = -999999.0;
	prddata_hour.seg[ptNo].data_lost.min = 999999.0;
	strcpy(prddata_hour.seg[ptNo].timestr_max_lost,"1111-11-11 11:11:11");
	strcpy(prddata_hour.seg[ptNo].timestr_min_lost,"1111-11-11 11:11:11");
	hour_sum = 0;
	searchResultNo = 0;

	while( ofetch(&cda)==0 )
	{
		sprintf(logtxt,DEBUGSTR"id(%d),min_max(%f),min_min(%f)",DEBUGARGS,id,min_max,min_min);
		log_write(INF,1,logtxt);


		sprintf(logtxt,DEBUGSTR"lost_max(%f),lost_min(%f)",DEBUGARGS,prddata_hour.seg[ptNo].data_lost.max,prddata_hour.seg[ptNo].data_lost.min);
		log_write(INF,1,logtxt);

		if(prddata_hour.seg[ptNo].data_lost.startdata < -999998.0)
			prddata_hour.seg[ptNo].data_lost.startdata = min_data;

		if(prddata_hour.seg[ptNo].data_lost.max < min_max){
			prddata_hour.seg[ptNo].data_lost.max = min_max;
			strcpy(prddata_hour.seg[ptNo].timestr_max_lost,min_datetime);
		}
		
		if(prddata_hour.seg[ptNo].data_lost.min > min_min){
			prddata_hour.seg[ptNo].data_lost.min = min_min;
			strcpy(prddata_hour.seg[ptNo].timestr_min_lost,min_datetime);
		}
	
		hour_sum = hour_sum + min_avg;
		searchResultNo++;
	}
	hour_sum = hour_sum/searchResultNo*3600;
	prddata_hour.seg[ptNo].data_lost.sum = hour_sum;

//	logtxt[150]="in %s at line %d: %s%d";
	

//	printf("get id %d info of one hour: hour_sum %f, hour_max %f, hour_min %f\n",hour_sum,prddata_hour.seg[ptNo].data_last.max,prddata_hour.seg[ptNo].data_last.min);
	return TRUE;
}

void * GetDataOfLostHour(void * arg){

	DATE_TIME curtime;

	DATE_TIME curtime2;

	int i;
	int sleepTime=0;
	
	printf("==== Start GetDataOfLostHour Thread======\n");
	GetSysTime(&curtime);

	sleepTime = ((60-curtime.minute)+5)*60*1000;
	MySleep(sleepTime);
	

	sprintf(log_str,"\n==== start get lost hour data(num:%d)====%s,%d\n",prddata_hour.num,__FILE__,__LINE__); 
	log_write(INF,1,log_str);
	//login
	for(;;)
	{
		if( ConnectHDB(&lda,&cda,LoginStr)==FALSE )
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


	for(i = 0; i < prddata_hour.num; i++){
		Get_Pt_Hour_Property_From_Min_Table(prddata_hour.seg[i].id,i,&curtime);
		printf("=!==!===!===!==!=Get_Pt_Hour_Property_From_Min_Table loop(%d)=!==!===!===!==!=\n",i);
	}



	GetSysTime(&curtime2);

	sprintf(log_str,"==== exe Get_Pt_Hour_Property_From_Min_Table from %d to %d(min)==== :%s\n",curtime.minute,curtime2.minute,__FILE__);
	log_write(INF,1,log_str);

	if(curtime2.minute<=30){
		sleepTime = (30-curtime2.minute)*60*1000;
		MySleep(sleepTime);
	}
	else{
		sleepTime = ((60-curtime2.minute)+30)*60*1000;
		MySleep(sleepTime);
	}

	for(i = 0; i < prddata_hour.num; i++){
		prddata_hour.seg[i].data_last = prddata_hour.seg[i].data_lost;

		strcpy(prddata_hour.seg[i].timestr_max_last,prddata_hour.seg[i].timestr_max_lost);
		strcpy(prddata_hour.seg[i].timestr_min_last,prddata_hour.seg[i].timestr_min_lost);
		printf(DEBUGSTR"for counter:%d\n",DEBUGARGS,i);
	}

	
	sprintf(prddata_hour.timestr,"%4d-%02d-%02d %02d:%02d:%02d",curtime.year,curtime.month,curtime.day,curtime.hour,0,0);

	prddata_hour.flag = TRUE;
	FlaginThreadGetLostHour = FALSE;

	sprintf(log_str,DEBUGSTR"mission of GetDataOfLostHours complete!\n",DEBUGARGS);
	log_write(INF,1,log_str);

	DisconnectHDB(&lda,&cda);

	return NULL;
}
