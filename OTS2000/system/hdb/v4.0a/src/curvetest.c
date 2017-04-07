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

void GenCurveInfoTable();
void GetOneCurveData(unsigned char second);
int CreatCurveYcInfo( unsigned char stnid );
int InsertCurveInHDB(unsigned char hour,unsigned char minute);
int ClearCurveData();
void *CurveWrite(void *arg);

unsigned int	CurveNum;
unsigned char 	g_second;
CURVE_SEC curve_data[2][MAX_LCU_NUM+1];

/********************************************************************************************************
 main function
*********************************************************************************************************/

void main()
{	
	DATE_TIME curtime;
	short	ispause;
	unsigned char hour,minute,second;
	char	tmp[100];
	int	i,j;	
 	char hostname[20];
#ifdef SUN
	signal(SIGKILL,sig_termp); 
	signal(SIGTERM,sig_termp);
	signal(SIGINT,sig_termp);
	signal(SIGPIPE,SIG_IGN);
#endif
	gethostname(hostname,20); 	
  	ReadDefFile();
 	strcpy(LoginID,strPwd);	
 	strcat(LoginID,SIDSTR);
	if(MountDB(DB_NAME) == -1)
	{
		printf("==== hdbwrite.c: call MountDB(DATABASE_NAME) error\n");		
		exit(0);
	}
	if(lan_init_all() == FALSE)
	{
		printf("==== hdbwrite.c: call Lan_init_all() error\n");		
		exit(0);
	}

	if(GetPtConf()==FALSE)	
	{
		printf("\n==== hdbwrite.c: GetPtConf() error!\n");
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
	
	/*************** connect hdb *****************/
	if( ConnectHDB(&lda_cvw,&cda_cvw,LoginID)==FALSE )
	{	printf("ConnectHDB() Orcal error !   ( %s )\n",LoginID);  exit(-1); }
	else
	{	printf("\n(^_^) --- Connect '%s' Success --- (^_^)\n",SIDSTR); }	
#ifdef SUN
	signal(SIGINT,sig_termp);
#endif
 	GenCurveInfoTable();
 	
 	CurveWrite(NULL);
}

/********************************************************************************************************
 CurveWrite function
*********************************************************************************************************/

void *CurveWrite(void *arg)
{	
	char	tmp[100];
	int	i,j;	
	DATE_TIME curtime;
	short	ispause;
	unsigned char hour,minute,second;

	GetSysTime(&curtime);
        hour = curtime.hour;
	minute = curtime.minute;
	second = curtime.second;
	printf("==== Starttime %d:%d:%d,please wait a minute...\n",curtime.hour,curtime.minute,curtime.second);
	ispause =0;
	ClearCurveData();

 	for(;;)
	{	
		MySleep(5000);
		for(i=0;i<60;i++)
		{
			GetOneCurveData(i);
		}
		GetSysTime(&curtime);
		if( curtime.second!=second )
		{
			g_second = curtime.second;
			InsertCurveInHDB(hour,minute);
			minute = curtime.minute;
			hour = curtime.hour;
			second = curtime.second;
			/*ClearCurveData();*/
		}
	}
	
}

/********************************************************************************************************
 Write Point Info to file
*********************************************************************************************************/

void GenCurveInfoTable()
{	
	int	i;
#ifdef DEBUG
	printf("\n==== start GenYcInfoTable() ====\n");
#endif	
	CurveNum =0;
	for(i=1;i<=g_stn_num;i++)
	{
		if(i>1) break;
		if( g_dev_num[i]!=0 )
		{
			printf("\n========= CYC Station %d  (%d--%d) ===========\n", i,1, g_dev_num[i]);
			
			if( CreatCurveYcInfo( i )==FALSE ) 
			{
				break;
			}	
		}
	}
	printf("CurveNum==%d\n",CurveNum);
}

/********************************************************************************************************
 create curve Yc point def file
*********************************************************************************************************/

int CreatCurveYcInfo( unsigned char StnId )
{	
	int			DevId,	DataType,	PointId,	Number;
	char			tmp[200],	FileName[200],	cname[100],	lname[100];
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
 	CurveNum=0;
	for(DevId=Start;DevId<=End;DevId++)
	{		
		num=0;
		curve_data[StnId][DevId].num = 0;
		curve_data[StnId][DevId].stnid =StnId;
		curve_data[StnId][DevId].devid =DevId;
		for(DataType=1;DataType<=13;DataType++)
		{								
			if( DataType==ANA_TYPE )
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
					curve_data[StnId][DevId].point_data[num].point = point;	
					curve_data[StnId][DevId].point_data[num].ioattr = db_data.ioattr;
					strcpy(curve_data[StnId][DevId].point_data[num].logicalname,lname); 					
  					strcpy(curve_data[StnId][DevId].point_data[num].name,pt_name.entryName); 
					if( (++num)<MAX_POINT_NUM ) continue; 
					else break; 
				}
			}
		}
		curve_data[StnId][DevId].num = num;
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
#ifdef DEBUG
	printf("\n==== start GetOneCurveData( %d ) CurveNum=%d ====\n",second,CurveNum);
#endif
 	Start = 1;
 	End = g_dev_num[StnId];
	for(DevId=Start;DevId<=End;DevId++)
	{		
		for(i=0;i<curve_data[StnId][DevId].num;i++)
		{		
			ReadDBByName( curve_data[StnId][DevId].point_data[i].logicalname, &db_data );		
			curve_data[StnId][DevId].point_data[i].value[second] = curve_data[StnId][DevId].point_data[i].value[second]+ 1 +second ;
			/*curve_data[StnId][DevId].point_data[i].value[second] = db_data.fvalue ;*/
			curve_data[StnId][DevId].point_data[i].status[second] = db_data.status ;
			curve_data[StnId][DevId].point_data[i].isvalid[second] =1;
		}
	}	
}

int ClearCurveData()
{
	int		i,j;
 	int 		Start,End;
 	unsigned 	StnId =1,DevId;
 	Start = 1;
 	End = g_dev_num[StnId];
	for(DevId=Start;DevId<=End;DevId++)
	{		
		for(i=0;i<curve_data[StnId][DevId].num;i++)
		{		
			for(j=0;j<60;j++)
			{
				if(j!=0 )
				{
					curve_data[StnId][DevId].point_data[i].value[j] = curve_data[StnId][DevId].point_data[i].value[j-1];
				}
				else
				{
					curve_data[StnId][DevId].point_data[i].value[j] = 999.001 +j;
				}	
				curve_data[StnId][DevId].point_data[i].status[j] = -1 ;
				curve_data[StnId][DevId].point_data[i].isvalid[j] =0;
			}
		}
	}	
}

int InsertCurveInHDB(unsigned char hour,unsigned char minute)
{
	int i,j;
	DATE_TIME curtime;
 	int 		Start,End;
 	unsigned 	StnId =1,DevId;
	char timestr[30],timestr2[30];	
 	Start = 1;
        End = g_dev_num[StnId];
 	GetSysTime(&curtime);
 
        sprintf(timestr,"%4d-%2d-%2d %2d:%2d:00",curtime.year,curtime.month,curtime.day,curtime.hour,curtime.minute);
 	sprintf(timestr2,"%4d-%2d-%2d %2d:%2d:%2d",curtime.year,curtime.month,curtime.day,curtime.hour,curtime.minute,curtime.second);
#ifdef DEBUG
	printf("\n==== start InsertCurveInHDB() time=%s ====\n",timestr2);
#endif

        for(DevId=Start;DevId<=End;DevId++)
	{
		if(curve_data[StnId][DevId].num <1) continue;
		strcpy(curve_data[StnId][DevId].timestr,timestr);

                if( PkgCurveSec( curve_data[StnId][DevId],pkgstr) != TRUE )
		{
			strcpy(curve_data[StnId][DevId].pkgstr,"");
			continue;
		}
		else
		{
			strcpy(curve_data[StnId][DevId].pkgstr,pkgstr);

			if(InsertHdbByCurve(cda_cvw,&curve_data[StnId][DevId]) !=TRUE )
			{
				printf("InsertHdbByCurve() error\n");
			}
			printf("\n==== InsertHdbByCurve() OK. stnid =%d,devid=%d,pkglen= %d\n",StnId,DevId,
				strlen(curve_data[StnId][DevId].pkgstr));	
		}

	}
	GetSysTime(&curtime);
 	sprintf(timestr2,"%4d-%2d-%2d %2d:%2d:%2d",curtime.year,curtime.month,curtime.day,
 		curtime.hour,curtime.minute,curtime.second);
#ifdef DEBUG
	printf("\n==== end InsertCurveInHDB() time=%s ====\n",timestr2);
#endif	
 
	return TRUE;
}

