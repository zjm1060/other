/********************************************************************************************************
FILENAME:	curvewrite.c

FUNCTION:	
	
	source code of  hdbscan module,include:
	(1) connect to history database
	(2) update some points  in real db to history database periodly
	(3) write alarm information to hdb

VERSION:	v4.0a

OPERATE SYSTEM:	window 2000, Unix

HISTORY:

2005.12.8	start to update the file from H9000 V3.0 to V4.0		yyp

*********************************************************************************************************/
#include "../inc/hdbhead_all.h"

#ifdef MSWIN32
 DWORD		ThreadID[2];
#endif
#ifdef SUN
 pthread_t	ThreadID[2],tid_terminated;
#endif

void GenCurveInfoTable();
void GetOneCurveData(unsigned char second);
int CreatCurveYcInfo( unsigned char stnid );
int InsertCurveInHDB(unsigned char hour,unsigned char minute);
int ClearCurveData();
void *CurveWrite(void *arg);
void *GetCurve(void *arg);
int CompCurveData();
int InitPointInfo();

unsigned int	CurveNum;
//CURVE_SEC curve_data[2][MAX_LCU_NUM+1];
//CURVE_SEC curve_data_tmp[2][MAX_LCU_NUM+1];
CURVE_SEC curve_data[MAX_DEV_NUM_HDB];
CURVE_SEC curve_data_tmp[MAX_DEV_NUM_HDB]; 



int UpdateFlag =FALSE ;
int InitFlag = FALSE;
int IsValid_All = TRUE;
int AllWriteFlag = TRUE;
int	HostOldStat, HostNewStat;
 char hostname[20];
 char LoginStr[50];
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
	/* SetLogLevel(argc, argv); */
	WSADATA WSAData;	
	if(WSAStartup(MAKEWORD(2,2),&WSAData)!=0)
#ifdef SUN
	signal(SIGKILL,sig_termp); 
	signal(SIGTERM,sig_termp);
	signal(SIGINT,sig_termp);
	signal(SIGPIPE,SIG_IGN);
#endif

	CreateLockFile(".curvewrite.lock");
	if(-1==InitSysLog(H9000_MODULE_CURVEWRITE, "CurveWrite"))
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
		printf("==== curvewrite.c: call MountDB(DATABASE_NAME) error\n");		
		exit(0);
	}
	if(lan_init_all() == FALSE)
	{
		printf("==== curvewrite.c: call Lan_init_all() error\n");		
		exit(0);
	}
	/* added by yyp 07.05.29 */
	if(strcmp(OUTNET_HOST,hostname)==0)
	{
		g_wranyhost = TRUE;
	}
	if(GetPtConf()==FALSE)	
	{
		printf("\n==== curvewrite.c: GetPtConf() error!\n");
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

 	GenCurveInfoTable();	
	
	if(InitPointInfo()==FALSE)
	{
		exit(0);
	}

	HdbConn = FALSE;
	
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
#ifndef MULTITHREAD	
	GetCurve(NULL); 
#endif
#ifdef SUN	
	/************  create thread  ************/
	res =   thr_create(NULL, MAX_HDB_STACK_SIZE, GetCurve, NULL, 0, &ThreadID[0]);
	if (res != -1  ) 
	{	    
		sprintf(log_str,"==== curvewrite.c: Create GetCurve thread %d  OK",(int)ThreadID[0]);	
		log_printf(INF,1,HDB_APP,log_str);
	}		
	else
	{ 
		sprintf(log_str,"==== curvewrite.c: Create GetCurve thread failed");	
		log_printf(INF,1,HDB_APP,log_str);
	}
	MySleep(200);
	
	res =   thr_create(NULL, MAX_HDB_STACK_SIZE, CurveWrite, NULL, 0, &ThreadID[1]);
	if (res != -1  ) 
	{	    
		sprintf(log_str,"==== curvewrite.c: Create CurveWrite thread %d  OK",(int)ThreadID[1]);	
		log_printf(INF,1,HDB_APP,log_str);
	}		
	else
	{ 
		sprintf(log_str,"==== curvewrite.c: Create CurveWrite thread failed");	
		log_printf(INF,1,HDB_APP,log_str);
	}
	MySleep(2000);
#else
	res =  CreateThread(NULL, MAX_HDB_STACK_SIZE, (LPTHREAD_START_ROUTINE)GetCurve, NULL, 0, &ThreadID[0]);
	if (res != NULL  ) 
	{	    
		sprintf(log_str,"==== curvewrite.c: Create GetCurve thread %d  OK",(int)ThreadID[0]);	
		log_printf(INF,1,HDB_APP,log_str);
	}		
	else
	{ 
		sprintf(log_str,"==== curvewrite.c: Create GetCurve thread failed");	
		log_printf(INF,1,HDB_APP,log_str);
	}
	MySleep(200);
	
	res =   CreateThread(NULL, MAX_HDB_STACK_SIZE, (LPTHREAD_START_ROUTINE)CurveWrite, NULL, 0, &ThreadID[1]);
	if (res != NULL  ) 
	{	    
		sprintf(log_str,"==== curvewrite.c: Create CurveWrite thread %d  OK",(int)ThreadID[1]);	
		log_printf(INF,1,HDB_APP,log_str);
	}		
	else
	{ 
		sprintf(log_str,"==== curvewrite.c: Create CurveWrite thread failed");	
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
 CurveWrite function
*********************************************************************************************************/

void *CurveWrite(void *arg)
{	

	DATE_TIME curtime;
	unsigned char hour,minute;
	int acc_link=0;
	printf("==== Start CurveWrite Thread\n");

 	for(;;)
	{	
		MySleep(200);
		HostNewStat=GetLocalStat( hostname );
		if(g_wranyhost == TRUE)
		{
			HostNewStat = CUR_WS_IN_HOST;
		}			
		if( HostNewStat!=CUR_WS_IN_HOST )
		{
			sprintf(log_str,"curvewrite.c: %s hoststate=%d is not primary!",hostname,HostNewStat);
			log_write(INF,2,log_str);
			MySleep(10000);	
			continue;
		}
		if(UpdateFlag ==TRUE)
		{	
			UpdateFlag =FALSE;			
			MySleep(200);
			GetSysTime(&curtime);
			minute = curtime.minute;
			hour = curtime.hour;
			//printf("\n ***zyf:: come into InsertCurveInHDB***\n");			
			InsertCurveInHDB(hour,minute);
			printf("\n ***zyf:: come over InsertCurveInHDB***\n");
			if(InitFlag==TRUE)
			{
				InitFlag = FALSE;
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
 Get Curve Data from RDB function
*********************************************************************************************************/

void *GetCurve(void *arg)
{	
	DATE_TIME curtime;
	unsigned char hour,minute,second;

	GetSysTime(&curtime);
	hour = curtime.hour;
	minute = curtime.minute;
	second = curtime.second;
	printf("==== Start GetCurve Thread, wait for a minute...\n");
	ClearCurveData();
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
			if(UpdateFlag==FALSE)
			{
				CompCurveData();				
				if(IsValid_All == TRUE)
				{
					memcpy(curve_data_tmp,curve_data,sizeof(CURVE_SEC)*(MAX_DEV_NUM_HDB));
					UpdateFlag =TRUE;
					if(curtime.hour != hour || shm_hdb_addr->readpt_curve == 1 )
					{
						shm_hdb_addr->readpt_curve = 0;
						GenCurveInfoTable();
						hour = curtime.hour;
					}
#ifndef MULTITHREAD	
					CurveWrite(NULL); 
#endif
				}
				
				ClearCurveData();				
			}	
			minute = curtime.minute;
			hour = curtime.hour;
		}
		/* modified by yyp 07.04.28*/
		if(curtime.second != second)
		{
			second = curtime.second;
			GetOneCurveData(second);
		}
	}
	
}


/********************************************************************************************************
 Write Point Info to file
*********************************************************************************************************/

void GenCurveInfoTable()
{	
	int	i;
	printf("\n==== start GenCurveInfoTable() ====\n");

	CurveNum =0;
	for(i=1;i<=g_stn_num;i++)
	{
		if( g_cascade != YES )
		{
			if(i>1) break;
		}
		if( g_dev_num[i]!=0 )
		{
			printf("\n==== CYC Station %d  (%d--%d) \n", i,1, g_dev_num[i]);
			
			if( CreatCurveYcInfo( (unsigned char)i )==FALSE ) 
			{
				break;
			}	
		}
	}
	InitFlag = TRUE;
	printf("\n==== CurveNum==%d\n",CurveNum);
}

/********************************************************************************************************
 create curve Yc point def file
*********************************************************************************************************/

int CreatCurveYcInfo( unsigned char StnId )
{	
	int			DevId,	DataType,	PointId,	Number;
	char			lname[100];
	POINTER			point;
	POINT_NAME		pt_name;
 	DB_DATA			db_data;
 	unsigned short		num;
 	struct tm		datetime;
 	long			t;
 	int 			Start,End;
 	Start = 1;
 	End = g_dev_num[StnId];
 	t=time(NULL); datetime=*localtime((long *)&t);
 	//CurveNum=0;
	for(DevId=Start;DevId<=End;DevId++)
	{
		curve_data[ dev_pos[StnId][DevId] ].num = 0;
	}
	for(DevId=Start;DevId<=End;DevId++)
	{		
#ifndef MULTIDEV
		if(DevId>1) break;
#endif		
		num=0;
		curve_data[dev_pos[StnId][DevId]].num = 0;
		curve_data[dev_pos[StnId][DevId]].stnid =StnId;
		curve_data[dev_pos[StnId][DevId]].devid =DevId;
		for(DataType=1;DataType<=13;DataType++)
		{								
			if( DataType==ANA_TYPE  || DataType==PARAM_TYPE || DataType==IMP_TYPE)
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
						printf("No %s in RDB\n",lname);					
						continue;
					}
					if(IsPtIntoCurveHdb(&point) != TRUE) continue;

					curve_data[dev_pos[StnId][DevId]].point_data[num].point = point;	
					curve_data[dev_pos[StnId][DevId]].point_data[num].ioattr = db_data.ioattr;
					strcpy(curve_data[dev_pos[StnId][DevId]].point_data[num].logicalname,lname);	
					if( (++num)<MAX_POINT_NUM ) continue; 
					else break; 
				}
			}
		}
		curve_data[dev_pos[StnId][DevId]].num = num;
		CurveNum += num;
	}
	return(TRUE);
}



/********************************************************************************************************
 Get Yc Changed Point data from real db
*********************************************************************************************************/

void GetOneCurveData(unsigned char second)
{ 
	int		i;
  	DB_DATA		db_data;
 	int 		Start,End;
 	unsigned char	StnId =1,DevId;
	sprintf(log_str,"==== start GetOneCurveData( %d ) CurveNum=%d ====\n",second,CurveNum);
	log_write(INF,1,log_str);
 	Start = 1;
 	End = g_dev_num[StnId];

	for(StnId=1;StnId<=(unsigned char)g_stn_num;StnId++)
	{
		for(DevId=1;DevId<=(unsigned char)g_dev_num[StnId];DevId++)
		{

		//for(DevId=Start;DevId<=End;DevId++)
		//{		
			for(i=0;i<curve_data[dev_pos[StnId][DevId]].num;i++)
			{		
				if(ReadDBByName( curve_data[dev_pos[StnId][DevId]].point_data[i].logicalname, &db_data )!=FALSE)
				{		
					curve_data[dev_pos[StnId][DevId]].point_data[i].value[second] = db_data.fvalue ;
					curve_data[dev_pos[StnId][DevId]].point_data[i].status[second] = db_data.status ;
				}	
				curve_data[dev_pos[StnId][DevId]].point_data[i].isvalid[second] =1;
			}
			MySleep(20);
		}
	}
}

int CompCurveData()
{
	DATE_TIME curtime;
	int		i,j;
 	int 		Start,End;
 	unsigned char	StnId =1,DevId;
 	float		tmpval,off_val;
 	short		tmpstatus;
 	Start = 1;
 	End = g_dev_num[StnId];
	GetSysTime(&curtime);
	sprintf(log_str,"CompCurveData() starttime=%d:%d:%d-%d",curtime.hour,curtime.minute,
		curtime.second,curtime.ms);
	log_write(INF,2,log_str);

	for(StnId=1;StnId<=(unsigned char)g_stn_num;StnId++)
	{
		for(DevId=1;DevId<=(unsigned char)g_dev_num[StnId];DevId++)
		{

	//for(DevId=Start;DevId<=End;DevId++)
	//{		
		for(i=0;i<curve_data[dev_pos[StnId][DevId]].num;i++)
		{		
			tmpval = curve_data[dev_pos[StnId][DevId]].point_data[i].value[0];
			tmpstatus = curve_data[dev_pos[StnId][DevId]].point_data[i].status[0];
			for(j=1;j<60;j++)
			{
				if(curve_data[dev_pos[StnId][DevId]].point_data[i].isvalid[j] ==1 )
				{
					if(fabs(curve_data[dev_pos[StnId][DevId]].point_data[i].value[j]-tmpval) > 
						curve_data[dev_pos[StnId][DevId]].point_data[i].offset ||
					 	tmpstatus != curve_data[dev_pos[StnId][DevId]].point_data[i].status[j])
					{
						curve_data[dev_pos[StnId][DevId]].point_data[i].chgflag =1 ;
						curve_data[dev_pos[StnId][DevId]].point_data[i].ischg[j] =1 ;
						tmpval = curve_data[dev_pos[StnId][DevId]].point_data[i].value[j];
						tmpstatus = curve_data[dev_pos[StnId][DevId]].point_data[i].status[j];
					}
				}
			}	
			if(curve_data[dev_pos[StnId][DevId]].point_data[i].chgflag != 1 )
			{
				for(j=59;j>=0;j--)
				{
					if(curve_data_tmp[dev_pos[StnId][DevId]].point_data[i].isvalid[j]==1)
					{
						off_val = curve_data[dev_pos[StnId][DevId]].point_data[i].value[0] - 
							curve_data_tmp[dev_pos[StnId][DevId]].point_data[i].value[j];
						if(fabs(off_val) > curve_data[dev_pos[StnId][DevId]].point_data[i].offset)
						{
							curve_data[dev_pos[StnId][DevId]].point_data[i].chgflag =1 ;
						}	
					}	
				}
			}
			if(AllWriteFlag == TRUE) 
			{
				curve_data[dev_pos[StnId][DevId]].point_data[i].chgflag =1 ;
			}
		}

		MySleep(5);
		}
	}
	GetSysTime(&curtime);
	sprintf(log_str,"CompCurveData() endtime=%d:%d:%d-%d",curtime.hour,curtime.minute,
		curtime.second,curtime.ms);
	log_write(INF,2,log_str);
	return TRUE;
}

int ClearCurveData()
{
	int		i,j;
 	unsigned int 		Start,End;
 	unsigned 	StnId =1,DevId;
 	Start = 1;
 	End = g_dev_num[StnId];

	for(StnId=1;StnId<=(unsigned char)g_stn_num;StnId++)
	{
		for(DevId=1;DevId<=(unsigned char)g_dev_num[StnId];DevId++)
		{

	//for(DevId=Start;DevId<=End;DevId++)
	//{		
		for(i=0;i<curve_data[dev_pos[StnId][DevId]].num;i++)
		{		
			curve_data[dev_pos[StnId][DevId]].point_data[i].chgflag =0 ;
			for(j=0;j<60;j++)
			{
				curve_data[dev_pos[StnId][DevId]].point_data[i].value[j] = MIN_NEG_NUM ; 
				curve_data[dev_pos[StnId][DevId]].point_data[i].status[j] = -1 ;
				curve_data[dev_pos[StnId][DevId]].point_data[i].isvalid[j] =0;
				curve_data[dev_pos[StnId][DevId]].point_data[i].ischg[j] =0 ;
			}
		}
	}
	}
	return TRUE;
}

int InsertCurveInHDB(unsigned char hour,unsigned char minute)
{
	DATE_TIME curtime;
 	unsigned int 		Start,End;
	int	total_len = 0;
 	unsigned char	StnId =1,DevId;
	char timestr[30],timestr2[30];	
	time_t now;
	int i,j,PtID;
	int iscond;
	struct tm datetime;
	int curvenum;
	time(&now);
	datetime=*localtime(&now);
	DateMinus(&datetime,ONE_MIN);
	GetDevCond();
	/* added by yyp 07.07.07 */
	datetime.tm_sec = 0;
	tmtostring(datetime,timestr); 

 	Start = 1;
	End = g_dev_num[StnId];
 	GetSysTime(&curtime);
	
 	sprintf(timestr2,"%4d-%2d-%2d %2d:%2d:%2d",curtime.year,curtime.month,curtime.day,curtime.hour,curtime.minute,curtime.second);
        
	sprintf(log_str,"==== InsertCurveInHDB: start InsertCurveInHDB() time=%s",timestr2);
	log_write(INF,1,log_str);

	for(j=0;j<60;j++)
	{
		memset(sec_ischg[j],0,MAX_IN_ROWS*sizeof(signed short));
	}
	i=0;
	for(StnId=1;StnId<=(unsigned char)g_stn_num;StnId++)
	{
		for(DevId=1;DevId<=(unsigned char)g_dev_num[StnId];DevId++)
		{
			for(PtID=0;PtID<curve_data_tmp[dev_pos[StnId][DevId]].num;PtID++)
			{
				iscond = TRUE;
				GetHdbCondByName((unsigned int *)&(iscond),
					curve_data_tmp[dev_pos[StnId][DevId]].point_data[PtID].logicalname);
				if(cond[StnId][DevId].status == 1 && iscond == TRUE && g_wrcond == 1 )
				{
					continue;
				}
				GetHdbIDByName((unsigned int *)&(data_all[i].id),
					curve_data_tmp[dev_pos[StnId][DevId]].point_data[PtID].logicalname);
				if(data_all[i].id == -1 )
				{
					sprintf(log_str,"\nGetHdbIDByName(%s) error",curve_data_tmp[dev_pos[StnId][DevId]].point_data[PtID].logicalname);
					err_write(log_str);
					continue;
				}
				strcpy(data_all[i].datetime,timestr);
				for(j=0;j<60;j++)
				{
					data_all[i].data[j] = curve_data_tmp[dev_pos[StnId][DevId]].point_data[PtID].value[j];
					if(curve_data_tmp[dev_pos[StnId][DevId]].point_data[PtID].ischg[j]!=1 || 
						curve_data_tmp[dev_pos[StnId][DevId]].point_data[PtID].value[j] == MIN_NEG_NUM)
					{
						sec_ischg[j][i] = -1;
					}
					else
					{
						sec_ischg[j][i] = 0;
					}
					if(j==0 && curve_data_tmp[dev_pos[StnId][DevId]].point_data[PtID].value[j] != MIN_NEG_NUM)
					{
						sec_ischg[j][i] = 0;
					}
				}
				i++;
			}
		}
	}
	curvenum = i;
	//printf("\n**zyf::come to curve_array_insert\n");
	if(curve_array_insert(&msqlctx,curvenum) !=TRUE )
	{
		HdbConn=FALSE;
		printf("curve_array_insert() error\n");
		return FALSE;
	}
	//printf("\n**zyf::come over InsertCurveInHDB\n");
	GetSysTime(&curtime);
 	sprintf(timestr2,"%4d-%2d-%2d %2d:%2d:%2d",curtime.year,curtime.month,curtime.day,
 		curtime.hour,curtime.minute,curtime.second);
	sprintf(log_str,"==== end InsertCurveInHDB(), num=%d, time=%s",curvenum,timestr2);
	log_write(INF,1,log_str); 
	
	return TRUE;
}

int InitPointInfo()
{
	unsigned char	StnId =1,DevId;
	int PtID;
	/*************** connect hdb *****************/
	InitFlag = FALSE;
	for(;;)
	{
		if( ConnectHDB(&mysql_ctx_cvw,strUsername,strPassword,strDbname,strHostToken)==FALSE )
		{	
			sprintf(log_str,"Curvewrite ConnectHDB() Orcal error !   ( %s )\n",SIDSTR);  
			err_write(log_str);
			MySleep(10000);
			continue; 
		}
		else
		{	
			sprintf(log_str,"\n==== Curvewrite Connect '%s' Success  ====\n",SIDSTR); 
			log_write(INF,1,log_str);
			break;
		}
	}	
#ifdef SUN
	signal(SIGINT,sig_termp);
#endif

	SelectDevCondFromHdb(&mysql_ctx_cvw);
	GetDevCond();
	if(SelectIdFromHdb(&mysql_ctx_cvw)== FALSE)
	{
		err_write("==== Curvewrite: SelectIdFromHdb() failed!");
		return FALSE;
	}
	for(StnId=1;StnId<=g_stn_num;StnId++)
	{
		for(DevId=1;DevId<=g_dev_num[StnId];DevId++)
		{
			for(PtID=0;PtID<curve_data[dev_pos[StnId][DevId]].num;PtID++)
			{
				if(g_wroffset == TRUE)
				{
					if(GetPtHdbOffsetByName( curve_data[dev_pos[StnId][DevId]].point_data[PtID].logicalname,
						&(curve_data[dev_pos[StnId][DevId]].point_data[PtID].offset) ) ==FALSE)
					{
						continue;
					}
				}
				else
				{
					curve_data[dev_pos[StnId][DevId]].point_data[PtID].offset = 0;
				}
			}
		}
	}
	DisconnectHDB(&mysql_ctx_cvw);	
	return TRUE;
}
