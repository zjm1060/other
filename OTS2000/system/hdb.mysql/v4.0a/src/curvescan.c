/************************************************************************************************************************
FILENAME:	curvescan.c

FUNCTION:	
	
	source code of  curvescan module,include:
	(1) connect to history database
	(2) get curve, minute, five,fifteen,hour data from history database
	(3) write data to curve shared memory

VERSION:	v4.0a

OPERATE SYSTEM:	window 2000, Unix

HISTORY:

2005.12.8	create the file for  V4.0		yyp
2006.5.16	modify the code for new db structure		yyp
**************************************************************************************************************************/
#include "../inc/hdbhead_all.h"

float	CurveData[MAX_CRT_NUM][MAX_SELECT_SEG][MAX_SELECT_REC];		
struct tm CurveTime[MAX_CRT_NUM][MAX_SELECT_SEG][MAX_SELECT_REC];	
int 	lastpos[MAX_CRT_NUM][MAX_SELECT_SEG];		
int	ChgFlag[MAX_CRT_NUM];				
int	CrtID=0;
char LoginStr[50];
/***************************************** main function ****************************************************************/

int	main(int argc, char *argv[])
{   
 	struct hdb_curve rs[MAX_CRT_NUM];			
 	struct tm starttime[MAX_CRT_NUM];
 	struct tm endtime[MAX_CRT_NUM];
 	struct tm lasttime[MAX_CRT_NUM];
 	time_t	ret1,ret2;
 	int	result;
 	char interval[MAX_CRT_NUM];
 	int i,count;
 	unsigned char curid=0;
 	int rscount=0;
 	char hostname[20];
#ifdef SUN
	signal(SIGKILL,sig_termp); 
	signal(SIGTERM,sig_termp);
	signal(SIGINT,sig_termp);
	signal(SIGPIPE,SIG_IGN);
#endif

	CreateLockFile(".curvescan.lock");
	if(-1==InitSysLog(H9000_MODULE_CURVESCAN, "CurveScan"))
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
		printf("==== curvescan.c: call MountDB(DATABASE_NAME) error\n");		
		exit(0);
	}
	if(lan_init_all() == FALSE)
	{
		printf("==== curvescan.c: call Lan_init_all() error\n");		
		exit(0);
	}
	if(GetPtConf()==FALSE)	
	{
		printf("\n==== curvescan.c: GetPtConf() error!\n");
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
	for(CrtID=0;CrtID<MAX_CRT_NUM;CrtID++)
	{
		starttime[CrtID].tm_isdst =-1;
		endtime[CrtID].tm_isdst =-1;
		lasttime[CrtID].tm_isdst =-1;
	}
	for(;;)
	{
		if( ConnectHDB(&mysql_ctx_cv,strUsername,strPassword,strDbname,strHostToken)==FALSE )
		{	
			/* BdSms("curvescan 连接历史库失败"); */
			shm_hdb_addr->connflag_curve=2;
			printf("ConnectHDB() mysql error !   ( %s )\n",strDbname);  
			MySleep(10000);
			HdbConn=FALSE;
			continue; 
		}
		else
		{	
			/* BdSms("curvescan 连接历史库成功"); */
			shm_hdb_addr->connflag_curve=1;
			printf("\n==== Connect '%s' Success ====\n",strDbname); 
			HdbConn=TRUE;
			break;
		}
	}
#ifdef SUN	
	signal(SIGINT,sig_termp);	
#endif	
	count=0;

	for(CrtID=0;CrtID<MAX_CRT_NUM;CrtID++)
	{
		shm_hdb_addr->data_curve[CrtID].queryflag=0;
		shm_hdb_addr->data_curve[CrtID].num=0;
		shm_hdb_addr->data_curve[CrtID].querytype=0;
		shm_hdb_addr->data_curve[CrtID].lockflag=0;
		for(curid=0;curid<MAX_SELECT_SEG;curid++)
		{
			rs[CrtID].seg[curid].lastrscount=0;
			rs[CrtID].seg[curid].rscount=0;
			lastpos[CrtID][curid]=-1;
		}
	}
	printf("\n==== waitting for new curve query ====");

	for(;;)
	{
		MySleep(10);
		for(CrtID=0;CrtID<MAX_CRT_NUM;CrtID++)
		{
			ChgFlag[CrtID]=0;

			if(shm_hdb_addr->data_curve[CrtID].queryflag==1)	
			{
				if(InitCurve(&rs[CrtID],CrtID) == FALSE) 
				{
					printf("\n=============== Invalid query info\n");
					shm_hdb_addr->data_curve[CrtID].queryflag=0;
					shm_hdb_addr->data_curve[CrtID].num=0;
					shm_hdb_addr->data_curve[CrtID].querytype=0;
					rs[CrtID].num = 0;
					break;
				}
				for(curid=0;curid<MAX_SELECT_SEG;curid++)
				{
					
					rs[CrtID].seg[curid].lastrscount=0;
					rs[CrtID].seg[curid].rscount=0;
					for(;;)
					{
						if(shm_hdb_addr->data_curve[CrtID].lockflag == 0)	
							break;
						MySleep(10);
					} 
					shm_hdb_addr->data_curve[CrtID].lockflag = 1;
					shm_hdb_addr->data_curve[CrtID].seg[curid].rscount=0;
					shm_hdb_addr->data_curve[CrtID].lockflag = 0;
					strcpy(rs[CrtID].seg[curid].lasttime,rs[CrtID].starttime);
					lastpos[CrtID][curid]=-1;
				}
				interval[CrtID]=rs[CrtID].interval;
				StrToTime(rs[CrtID].starttime,&starttime[CrtID],interval[CrtID],1);
				StrToTime(rs[CrtID].endtime,&endtime[CrtID],interval[CrtID],1);
				printf("\n zyf::starttime==%s, endtime==%s\n",rs[CrtID].starttime,rs[CrtID].endtime);
				shm_hdb_addr->data_curve[CrtID].queryflag=0;
			}
			else
			{
				/*if real query type,then end CurveTime +1*interval */
				if(rs[CrtID].querytype==1)
				{
					DateAdd(&endtime[CrtID],interval[CrtID]);
					tmtostring(endtime[CrtID],rs[CrtID].endtime);
					DateAdd(&starttime[CrtID],interval[CrtID]);
					tmtostring(starttime[CrtID],rs[CrtID].starttime);
					for(curid=0;curid<rs[CrtID].num;curid++)
					{
						
						for(i=1;i<lastpos[CrtID][curid]+1;i++)
						{
							CurveData[CrtID][curid][i-1]=CurveData[CrtID][curid][i];
							CurveTime[CrtID][curid][i-1]=CurveTime[CrtID][curid][i];
						}
						if(lastpos[CrtID][curid]>0)
						{
							lastpos[CrtID][curid]--;
						}
					}
				}
				else if(rs[CrtID].querytype==0)
				{
					MySleep(200);
					continue;
				}
			}
			if(CrtID != 0)	continue;
			if(rs[CrtID].num<1 || rs[CrtID].num>6)	continue;
			
			for(curid=0;curid<rs[CrtID].num;curid++)
			{	
				StrToTime(rs[CrtID].seg[curid].lasttime,&lasttime[CrtID],interval[CrtID],1);
				printf(" \n\ncurid=%d,lastpos=%d\nendtime=	%s",curid,
					lastpos[CrtID][curid]+1,  myasctime(endtime[CrtID]));
				printf("\nlasttime=	%s", myasctime(lasttime[CrtID]));	
				printf("\nstarttime=	%s \n", myasctime(starttime[CrtID]));
		
				if((lastpos[CrtID][curid]+1) >=MAX_SELECT_REC)	continue;
		
				if((ret1=mktime(&lasttime[CrtID]))>(ret2=mktime(&endtime[CrtID])))	
					continue;
				if(rs[CrtID].interval != ONE_SEC )
				{
					result = SelectYcRecFromHdb(&mysql_ctx_cv,curid,&rs[CrtID]);
				}
				else
				{
					result = SelectCurveFromHdb(&mysql_ctx_cv,curid,&rs[CrtID]);
				}
				if( result == FALSE )
				{	
					printf("SelectYcRecFromHdb( curid= %d ) error !\n",curid);
					HdbConn = FALSE;
				}
				else
				{	
					printf("\nSelectYcRecFromHdb( curid= %d ) successfully !\n",curid);
					rscount=rs[CrtID].seg[curid].rscount;
					if(rscount>0)
					{
						ChgFlag[CrtID]=1;
					}
					printf("==== rs return code =  %d \n",rscount);
#ifdef DEBUG
					for(i=0;i<rscount;i++)
					{
						printf("i=%4d %8.3f  %s \n",i,rs[CrtID].seg[curid].data[i],
							rs[CrtID].seg[curid].timestr[i]);
					}
#endif
				}
			}
			if(ChgFlag[CrtID] == 1)
			{
				InterCurveData(&rs[CrtID],CrtID);

				UpdateCurveShm(&rs[CrtID],CrtID);					
			}
			
			for(;;)
			{
				if(shm_hdb_addr->data_curve[CrtID].lockflag==0)	break;
				MySleep(1);
			}
			shm_hdb_addr->data_curve[CrtID].lockflag=1;
			shm_hdb_addr->data_curve[CrtID].finishedFlag=1;
			shm_hdb_addr->data_curve[CrtID].lockflag=0;
			printf(" \nHDB curve scan query finished!\n");
			
			MySleep(1000);
			count++;
			if( count>=MAXLOOPCOUNT || HdbConn!=TRUE )
			{	
				DisconnectHDB(&mysql_ctx_cv);
				MySleep(2000);
				if( ConnectHDB(&mysql_ctx_cv,strUsername,strPassword,strDbname,strHostToken)==FALSE )
				{	
					printf("REConnectHDB() mysql error !   ( %s )\n",SIDSTR);  
					MySleep(60000);
				}
				else
				{	
					printf("\n==== ReConnect '%s' Success ====\n",SIDSTR);
					HdbConn=TRUE;
				}
#ifdef SUN				
				signal(SIGINT,sig_termp);
#endif				
				count=0;	
			}
		} /* end for CrtID */
		
	}/* end for(;;) */
		
	if( DisconnectHDB(&mysql_ctx_cv)==FALSE )
	{	printf("DisconnectHDB() Orcal error !\n");  exit(-1); }
	else
	{	printf("\n==== Disconnect '%s' Success ====\n",SIDSTR); }	
	

}

/*********************** get the curve shm **********************************************/

int InitCurve(struct hdb_curve *p_rs,int CrtID)
{
	int i;
	POINTER pt;
	p_rs->queryflag=shm_hdb_addr->data_curve[CrtID].queryflag;
	p_rs->querytype=shm_hdb_addr->data_curve[CrtID].querytype;
	p_rs->lockflag=shm_hdb_addr->data_curve[CrtID].lockflag;
	p_rs->num=shm_hdb_addr->data_curve[CrtID].num;
	p_rs->interval=shm_hdb_addr->data_curve[CrtID].interval;
	strcpy(p_rs->starttime,shm_hdb_addr->data_curve[CrtID].starttime);
	strcpy(p_rs->endtime,shm_hdb_addr->data_curve[CrtID].endtime);
	for(i=0;i<p_rs->num;i++)
	{
		strcpy(p_rs->seg[i].logicalname,shm_hdb_addr->data_curve[CrtID].seg[i].logicalname);
		if(GetPtIdByNameStr( p_rs->seg[i].logicalname, &pt )==-1) return FALSE;	
		p_rs->seg[i].stnid= pt.stn_id;
		p_rs->seg[i].devid= pt.dev_id;
		p_rs->seg[i].pointid= pt.pt_id;
	}	
	
	memset(CurveData[CrtID], 0, sizeof(float)*MAX_SELECT_SEG*MAX_SELECT_REC);
	return TRUE;	
}

/************** parse the data array from hdb, insert the data lacked *******************/

int InterCurveData(struct hdb_curve *p_rs, int CrtID)
{
	struct tm lasttime,endtime,curtime,tmptime;
	time_t	ret1,ret2;				
	int 	rscount;
	int 	currs;		
	int i,j;
	char interval;
	unsigned char curid=0;
	interval=p_rs->interval;
	StrToTime(p_rs->endtime,&endtime,interval,1);
	tmptime=endtime;
	for(curid=0;curid<p_rs->num;curid++)
	{
		rscount=p_rs->seg[curid].rscount;
		if(rscount<=0) continue;
		StrToTime(p_rs->seg[curid].lasttime,&lasttime,interval,1);
		if(rscount<=0 || lastpos[CrtID][curid]>=(MAX_SELECT_REC-1))	
			continue;
		j=lastpos[CrtID][curid]+1;
		curtime=lasttime;
		currs=0;
		i=currs;
		while((ret1=mktime(&curtime))<=(ret2=mktime(&endtime)) && j<MAX_SELECT_REC && i<rscount)
		{		
			
			for(i=currs;i<rscount;i++)
			{
				if(i<0) continue;
				StrToTime(p_rs->seg[curid].timestr[i],&tmptime,interval,0);
				if((ret1=mktime(&curtime))<(ret2=mktime(&tmptime)) )
				{
					if(i>=1)
					{
						CurveData[CrtID][curid][j]=p_rs->seg[curid].data[i-1];
					}
					else if(i<1 && j>=1)
					{
						CurveData[CrtID][curid][j]=CurveData[CrtID][curid][j-1];
					}					
					else if(i<1 && j<1)
					{
						CurveData[CrtID][curid][j]=0;
					}
					CurveTime[CrtID][curid][j]=curtime;
					break;
				}
				else if((ret1=mktime(&curtime))==(ret2=mktime(&tmptime)) )
				{
					CurveData[CrtID][curid][j]=p_rs->seg[curid].data[i];
					CurveTime[CrtID][curid][j]=curtime;
					break;						
				}
			}
			currs=i;
			if(i > rscount)
			{
				CurveData[CrtID][curid][j]=p_rs->seg[curid].data[i-1];
				CurveTime[CrtID][curid][j]=curtime;
				DateAdd(&curtime,interval);
				j++;
				break;	
			}
			else if( i== rscount)
			{
				DateAdd(&curtime,interval);
				j++;
				break;				
			}
			DateAdd(&curtime,interval);
			j++;
		}
		lastpos[CrtID][curid]=j-1;
		printf("\nInterCurveData(): lastpos=%d,lasttime=%s\n",j,myasctime(curtime));
		tmtostring(curtime,p_rs->seg[curid].lasttime);
	}
	printf("==== inter curve data completed\n");
	return TRUE;
}

/***************** update data got from hdb  to share memories **************************/

int UpdateCurveShm(struct hdb_curve *p_rs,int CrtID)
{
 	unsigned char curid=0;
 	int rscount=0;
	for(;;)
	{
		if(shm_hdb_addr->data_curve[CrtID].lockflag==0)	break;
		MySleep(1);
	}
	shm_hdb_addr->data_curve[CrtID].lockflag=1;
	for(curid=0;curid<p_rs->num;curid++)
	{	
		memcpy(shm_hdb_addr->data_curve[CrtID].seg[curid].data, 
			CurveData[CrtID][curid], 4*MAX_SELECT_REC);
		shm_hdb_addr->data_curve[CrtID].seg[curid].rscount=lastpos[CrtID][curid]+1;
		printf(" \nHDB curve sharem memory ( curid= %d ) rscount=%d !\n",\
			curid,shm_hdb_addr->data_curve[CrtID].seg[curid].rscount );
/*#ifdef DEBUG
		for(i=0;i<shm_hdb_addr->data_curve[CrtID].seg[curid].rscount;i++)
		{
			tmtostring(CurveTime[CrtID][curid][i],str);
			
			printf("i=%4d %8.3f  %s \n",i,CurveData[CrtID][curid][i],str);
		}
#endif		
*/
	}
	shm_hdb_addr->data_curve[CrtID].lockflag=0;
	printf("==== update curve shared memeries successfully!\n");
	MySleep(1);	
	return TRUE;	
}

