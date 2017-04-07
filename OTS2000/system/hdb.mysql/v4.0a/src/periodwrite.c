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

*********************************************************************************************************/
#include "../inc/hdbhead_all.h"

#ifdef MSWIN32
 DWORD		ThreadID[2];
#endif
#ifdef SUN
 pthread_t	ThreadID[2],tid_terminated;
#endif

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

HDB_YC_PERIOD	prddata;
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
/********************************************************************************************************
 main function
*********************************************************************************************************/

void main(int argc, char *argv[])
{	
#ifdef SUN
	int 	res;
#else
	HANDLE res;
#endif
	/* wsa_startup(); */
	/* SetLogLevel(argc, argv);  */
	WSADATA WSAData;	
	if(WSAStartup(MAKEWORD(2,2),&WSAData)!=0)
#ifdef SUN
	signal(SIGKILL,sig_termp); 
	signal(SIGTERM,sig_termp);
	signal(SIGINT,sig_termp);
	signal(SIGPIPE,SIG_IGN);
#endif

	CreateLockFile(".periodwrite.lock");
	if(-1==InitSysLog(H9000_MODULE_PERIODWRITE, "PeriodWrite"))
	{
		Err_log("Fail to InitSysLog!\n");
		exit(-1);
	}

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
		if( ConnectHDB(&msqlctx,strUsername,strPassword,strDbname,strHostToken)==FALSE )
		{	
			sprintf(log_str,"\n==== Connect '%s' failed ==== \n",strDbname);
			err_write(log_str); 
			HdbConn = FALSE;
			MySleep(10000);
			continue; 
		}
		else
		{	
			sprintf(log_str,"\n==== Connect '%s' Success  ====\n",strDbname); 
			log_write(INF,1,log_str);
			HdbConn = TRUE;
			break;
		}
	}


	/* GetPeriod(NULL);  */

#ifdef SUN	
	/************  create thread  ************/
	res =   thr_create(NULL, MAX_HDB_STACK_SIZE, GetPeriod, NULL, 0, &ThreadID[0]);
	if (res != -1  ) 
	{	    
		sprintf(log_str,"==== periodwrite.c: Create GetPeriod thread %d  OK",(int)ThreadID[0]);	
		log_printf(INF,1,HDB_APP,log_str);
	}		
	else
	{ 
		sprintf(log_str,"==== periodwrite.c: Create GetPeriod thread failed");	
		log_printf(INF,1,HDB_APP,log_str);
	}
	MySleep(200);
	
	res =   thr_create(NULL, MAX_HDB_STACK_SIZE, PeriodWrite, NULL, 0, &ThreadID[1]);
	if (res != -1  ) 
	{	    
		sprintf(log_str,"==== periodwrite.c: Create PeriodWrite thread %d  OK",(int)ThreadID[1]);	
		log_printf(INF,1,HDB_APP,log_str);
	}		
	else
	{ 
		sprintf(log_str,"==== periodwrite.c: Create PeriodWrite thread failed");	
		log_printf(INF,1,HDB_APP,log_str);
	}
	MySleep(2000);
#else
	res =  CreateThread(NULL, MAX_HDB_STACK_SIZE, (LPTHREAD_START_ROUTINE)GetPeriod, NULL, 0, &ThreadID[0]);
	if (res != NULL  ) 
	{	    
		sprintf(log_str,"==== periodwrite.c: Create GetPeriod thread %d  OK",(int)ThreadID[0]);	
		log_printf(INF,1,HDB_APP,log_str);
	}		
	else
	{ 
		sprintf(log_str,"==== periodwrite.c: Create GetPeriod thread failed");	
		log_printf(INF,1,HDB_APP,log_str);
	}
	MySleep(200);
	
	res =   CreateThread(NULL, MAX_HDB_STACK_SIZE, (LPTHREAD_START_ROUTINE)PeriodWrite, NULL, 0, &ThreadID[1]);
	if (res != NULL  ) 
	{	    
		sprintf(log_str,"==== periodwrite.c: Create PeriodWrite thread %d  OK",(int)ThreadID[1]);	
		log_printf(INF,1,HDB_APP,log_str);
	}		
	else
	{ 
		sprintf(log_str,"==== periodwrite.c: Create PeriodWrite thread failed");	
		log_printf(INF,1,HDB_APP,log_str);
	}
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
		if(prddata.flag ==TRUE)
		{	
			prddata.flag =FALSE;			
			MySleep(200);
			UpdatePeriodInHDB(ONE_MIN);
			if(hour != curtime.hour || shm_hdb_addr->readpt_period == 1 )
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
			DisconnectHDB(&msqlctx);
			HdbConn=FALSE;
			for(;;)
			{
				if( ConnectHDB(&msqlctx,strUsername,strPassword,strDbname,strHostToken)==FALSE )
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
			for(i=0;i<prddata.num;i++)
			{
				prddata.seg[i].data_last = prddata.seg[i].data_cur;
				prddata.seg[i].data_cur.startdata = prddata.seg[i].realvalue;
				prddata.seg[i].data_cur.max = prddata.seg[i].data_cur.startdata;
				prddata.seg[i].data_cur.min = prddata.seg[i].data_cur.startdata;
			}
			/* added by 08.03.17 */
			MySleep(400);
			prddata.flag = TRUE;
			ClearPeriodData(ONE_MIN);
			/* PeriodWrite(NULL); */

		} /* end of if(curtime.minute!=minute) */

		/* modified by yyp 07.04.28*/
		if(curtime.second != second)
		{
			second = curtime.second;
			GetOnePeriodData(second);

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
	ptnum = prddata.num;
	SelectIdFromHdb(&mysql_ctx_cvw);
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
 	Start = 1;
 	End = g_dev_num[StnId];
 	t=time(NULL); datetime=*localtime((long *)&t);
	num=prddata.num;
	for(DevId=Start;DevId<=End;DevId++)
	{		
#ifndef MULTIDEV
		if(DevId>1) break;
#endif	
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
  	DB_DATA		db_data;
 	unsigned char	StnId =1;
	sprintf(log_str,"==== start GetOnePeriodData( %d ) PeriodNum=%d ====\n",second,prddata.num);
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
 	Start = 1;
	End = g_dev_num[StnId];
 	GetSysTime(&curtime);
      
	sprintf(timestr,"%4d-%2d-%2d %2d:%2d:%2d",curtime.year,curtime.month,curtime.day,curtime.hour,curtime.minute,curtime.second);
        
	sprintf(log_str,"==== UpdatePeriodInHDB: start UpdatePeriodInHDB(%d) time=%s",interval,timestr);
	log_write(INF,1,log_str);
	GetDevCond();
	if(UpdateHdbByPeriod(&msqlctx,&prddata) !=TRUE )
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
	for(;;)
	{
		if( ConnectHDB(&mysql_ctx_cvw,strUsername,strPassword,strDbname,strHostToken)==FALSE )
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

	SelectDevCondFromHdb(&mysql_ctx_cvw);
	GetDevCond();	
	GetIdFromHdb();

	DisconnectHDB(&mysql_ctx_cvw);
#ifdef SUN
	signal(SIGINT,sig_termp);
#endif	
	return TRUE;
}
