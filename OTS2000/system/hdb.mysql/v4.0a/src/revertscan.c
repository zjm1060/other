/********************************************************************************************************
FILENAME:	revertscan.c

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


float YcRevertData[MAX_REVERT_NUM][MAX_REVERT_REC];			/*data buffer of query */
char RevertTime[MAX_REVERT_REC][22];			/*time of the record*/
short YxRevertData[MAX_REVERT_NUM][MAX_REVERT_REC];			/*data buffer of query */
struct point_value yc_data[MAX_REVERT_NUM];
struct point_value yx_data[MAX_REVERT_NUM]; 
struct yx_revert yx_revert_data[MAX_REVERT_NUM];
struct yc_seg yc_init_data[MAX_YC_NUM];
struct yx_seg yx_init_data[MAX_YX_NUM];
HDB_REVERT init_data;
int	RevertChgFlag;				
//struct hdb_revert rs;
char LoginStr[50];
int lastptr=0;
unsigned int pt_num_yx;
unsigned int pt_num_yc;
int total_count;
BOOL init_flag ;
char log_str[100];
int WriteAlarmShm();
int GetPtFromDev(unsigned char stn_id,unsigned char dev_id);
int WriteRevertData();
int WriteInitData();
void GetInitData();

/********************************************************************************************************
 main function
*********************************************************************************************************/

void main(int argc, char *argv[])
{	
 	struct tm starttime;
 	struct tm endtime;
	time_t	ret1,ret2;	
	time_t t1,t2;
 	int	result;
	int n=0;
 	char interval;
 	int count;
	unsigned short curid=0; 
 	int rscount=0;
 	char hostname[20];
	init_flag = FALSE;
#ifdef WINDOWS_OS
	wsa_startup();
#endif
	//SetLogLevel(argc, argv);
#ifdef SUN
	signal(SIGKILL,sig_termp); 
	signal(SIGTERM,sig_termp);
	signal(SIGINT,sig_termp);
	signal(SIGPIPE,SIG_IGN);
#endif
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

	if(get_ots_shm()==FALSE)
	{
		exit(0);
	}
	
	/*************** connect hdb *****************/
	for(;;)
	{
		if( ConnectHDB(&mysql_ctx_rt,strUsername,strPassword,strDbname,strHostToken)==FALSE)
		{	
			/* BdSms("revertscan 连接历史库失败"); */
			shm_hdb_addr->connflag_revert=2;
			printf("ConnectHDB()  error !   ( %s )\n",SIDSTR);  
			HdbConn=FALSE;
			MySleep(10000);
			continue; 
		}
		else
		{	
			/* BdSms("revertscan 连接历史库成功"); */
			shm_hdb_addr->connflag_revert=1;
			printf("\n==== Connect '%s' Success ==== \n",SIDSTR); 
			HdbConn=TRUE;
			break;
		}
	}


#ifdef SUN
	signal(SIGINT,sig_termp);
#endif

	if(SelectIdFromHdb(&mysql_ctx_rt)== FALSE)
	{
		Err_log("==== Hdbwrite: SelectIdFromHdb() failed!");
		exit(0);
	}

	starttime.tm_isdst =-1;
	endtime.tm_isdst =-1;
	count=0;
	
	/******** clear the shm  **********/
	
	shm_hdb_addr->data_revert.queryflag=0;
	shm_hdb_addr->data_revert.ycnum=0;
	shm_hdb_addr->data_revert.yxnum=0;
	shm_hdb_addr->data_revert.querytype=0;
	shm_hdb_addr->data_revert.stopflag=0;
	shm_hdb_addr->data_revert.speed=8;
	for(curid=0;curid<MAX_REVERT_NUM;curid++)
	{
		yc_data[curid].rscount=0;
	}
	printf("\n==== waitting for new revert query...... \n");
 	for(;;)
	{	
		MySleep(200);
		RevertChgFlag=0;
		if (init_flag)
		{
			WriteInitData();
		}
		/******* if new query, clear the shm *********/
		if(shm_hdb_addr->data_revert.queryflag==1)
		{
			printf("get new revert type =%d\n",shm_hdb_addr->data_revert.querytype);
			if(shm_hdb_addr->data_revert.querytype == 1)
			{
				if(InitRevert(&shm_hdb_addr->data_revert) == FALSE)
				{
					printf("\n=============== Invalid query info\n");
					/* shm_hdb_addr->data_revert.queryflag=0; */
					shm_hdb_addr->data_revert.ycnum=0;
					shm_hdb_addr->data_revert.yxnum=0;
					shm_hdb_addr->data_revert.querytype=0;
					shm_hdb_addr->data_revert.queryflag=0;
					pt_num_yc =0;
					pt_num_yx =0;
					continue;
				}
			}	
			else
			{
				if(GetPtFromDev(shm_hdb_addr->data_revert.stn_id,shm_hdb_addr->data_revert.dev_id) != TRUE)
				{
					shm_hdb_addr->data_revert.queryflag=0;
					continue;
				}
			}
			strcpy(shm_hdb_addr->data_revert.curtime,shm_hdb_addr->data_revert.starttime);
			shm_hdb_addr->data_revert.ycnum = pt_num_yc;
			shm_hdb_addr->data_revert.yxnum = pt_num_yx;
			GetInitData();
			for(curid=0;curid<MAX_REVERT_NUM;curid++)
			{
				yc_data[curid].rscount=0;
				yx_data[curid].rscount=0;
				shm_hdb_addr->data_revert.curptr=0;
				StrToTime(shm_hdb_addr->data_revert.starttime,&yc_data[curid].basetime,ONE_MIN,1);
			}
			lastptr=0;
			interval=ONE_SEC;
			StrToTime(shm_hdb_addr->data_revert.starttime,&starttime,interval,1);
			StrToTime(shm_hdb_addr->data_revert.endtime,&endtime,interval,1);
			ret1=mktime(&starttime);
			ret2=mktime(&endtime);
			total_count = ret2 - ret1;
			if( total_count > MAX_REVERT_REC )
			{
				printf("err: time len too max \n");
				continue;
			}
		}
		else
		{
			//added by yyp 2009.8.20

			continue;
		}
		WriteAlarmShm();
		shm_hdb_addr->data_revert.queryflag = 3 ;
		printf("\nStart SelectRevertFromHdb,ycnum=%d...\n",shm_hdb_addr->data_revert.ycnum);
		if(shm_hdb_addr->data_revert.ycnum>=1 && shm_hdb_addr->data_revert.ycnum<MAX_REVERT_NUM)
		{
			for(curid=0;curid<shm_hdb_addr->data_revert.ycnum;curid++)
			{	
				if(shm_hdb_addr->data_revert.queryflag==2 )
				{
					break;
				}
				//printf(" starttime=	%s ", asctime(&starttime));
				result = SelectRevertFromHdb(&mysql_ctx_rt,&(shm_hdb_addr->data_revert),&(yc_data[curid]));
				if( result == FALSE )
				{	
					printf("SelectRevertFromHdb( curid= %d ) error !\n",curid);
					HdbConn=FALSE;
				}
				else
				{	
					//printf("\nSelectRevertFromHdb( curid= %d ) successfully !\n",curid);
					rscount=yc_data[curid].rscount;
					if(rscount>0)
					{
						RevertChgFlag=1;
					}
					//printf("-- yc rs return code =  %d --\n",rscount);
#ifdef HDBDEBUG
					for(i=0;i<yc_data[curid].rscount;i++)
					{
						printf("i=%d, %8.3f ,  %s\n",i,yc_data[curid].value[i],
							yc_data[curid].timestr[i]);
					}
#endif			
				}
			}/* end of for(curid) */

		}/* end if */
		printf("\nEnd SelectYcRevertFromHdb\n");			
		if(shm_hdb_addr->data_revert.queryflag==2 )
		{ 	
			continue;
		}
		printf("\nStart SelectYxRevertFromHdb,yxnum=%d...\n",shm_hdb_addr->data_revert.yxnum);
		time(&t1);		
		if(shm_hdb_addr->data_revert.yxnum>=1 && shm_hdb_addr->data_revert.yxnum<MAX_REVERT_NUM)
		{
			for(curid=0;curid<shm_hdb_addr->data_revert.yxnum;curid++)
			{
				if(shm_hdb_addr->data_revert.queryflag==2 )
				{
					break;
				}

				result = SelectYxRevertFromHdb(&mysql_ctx_rt,&(shm_hdb_addr->data_revert),&(yx_data[curid]));
				if( result == FALSE )
				{	
					printf("SelectYxRevertFromHdb( curid= %d ) error !\n",curid);
					HdbConn=FALSE;
				}
				else
				{	
					//printf("\nSelectYxRevertFromHdb( curid= %d ) successfully !\n",curid);
					rscount=yx_data[curid].rscount;
					if(rscount>0)
					{
						RevertChgFlag=1;
					}
					//printf("-- yx rs return code =  %d --\n",rscount);
				}		
				yx_revert_data[curid].id = yx_data[curid].id;
				strcpy(yx_revert_data[curid].starttime,shm_hdb_addr->data_revert.starttime); 
				strcpy(yx_revert_data[curid].endtime,shm_hdb_addr->data_revert.endtime); 
			}

		}
		time(&t2);	
		printf("\nEnd SelectYxRevertFromHdb,cost %d secs\n",t2-t1);	
		
		if(shm_hdb_addr->data_revert.queryflag==2 )
		{	
			continue;
		}
		if(RevertChgFlag == 1)
		{
			/***** interpolate the data *****/
			if(InterRevertData(&(shm_hdb_addr->data_revert)) != TRUE)
			{
				printf("InterRevertData error\n");
			}
			shm_hdb_addr->data_revert.queryflag = 4 ;			
			/***** Update data to shm *****/
			WriteRevertData();
			
			printf("\n\n==== waitting for new revert query...... \n");					
		}
		else
		{
			shm_hdb_addr->data_revert.queryflag = 2 ;
			printf("\n\n==== waitting for new revert query...... \n");
		}
		if( shm_hdb_addr->data_revert.queryflag == 1 )
		{
			shm_hdb_addr->data_revert.queryflag = 0;
		}
		MySleep(1000);
		count++;
		/****** reconnect hdb when loop count reach max ******/
		if( count>=MAXLOOPCOUNT  || HdbConn!=TRUE)
		{	
			DisconnectHDB(&mysql_ctx_rt);
			MySleep(2000);
			if( ConnectHDB(&mysql_ctx_rt,strUsername,strPassword,strDbname,strHostToken)==FALSE)
			{	
				printf("REConnectHDB() Orcal error !   ( %s )\n",LoginStr);
				MySleep(60000);  
			}
			else
			{	
				printf("\n==== ReConnect '%s' Success ==== \n",LoginStr); 
				HdbConn=TRUE;
			}
#ifdef SUN
			signal(SIGINT,sig_termp);
#endif
			count=0;	
		}
		
	}/* end of for(;;) */

}

/************** parse the data array from hdb, insert the data lacked *******************/

int InterRevertData(struct hdb_revert *p_rs)
{
	struct tm starttime,endtime,curtime,tmptime;
			
	int 	rscount;
	int 	currs,startrs;		/* row id of hdb recordset */
	int i,j;
	char interval;
	int pos[MAX_REVERT_REC],offset;
	time_t	ret1,ret2,ret3;
	unsigned short curid=0;
	interval=ONE_SEC;
	StrToTime(p_rs->starttime,&starttime,interval,1);
	StrToTime(p_rs->endtime,&endtime,interval,1);
	
	curtime=starttime;

	for(i=0;i<total_count;i++)
	{
		DateAdd(&curtime,interval);
		tmtostring(curtime,RevertTime[i]);	
	}
	for(curid=0;curid<p_rs->ycnum;curid++)
	{
		memset(pos,-1,MAX_REVERT_REC * sizeof(int));
		currs=0;
		j=0;
		i=0;
		tmptime=endtime;
		curtime=starttime;                
		rscount=yc_data[curid].rscount;
		if(rscount<=0) continue;
		ret1=mktime(&starttime);
		ret2=mktime(&endtime);
		for(i=0;i<rscount;i++)
		{
			StrToTime(yc_data[curid].timestr[i],&tmptime,interval,0);
			startrs=0;
			if(  (ret3=mktime(&tmptime)) > ret1 )
			{
				if(i>=1)
				{
					pos[i-1]=0;
					startrs=i-1;
				}
				break;
			}
			else
			{
				if( i== rscount-1)
				{
					pos[i]=0;
					startrs=i;
				}
			}
		}//end for
		for( i=startrs; i<rscount; i++ )
		{
			StrToTime(yc_data[curid].timestr[i],&tmptime,interval,0);
			ret3=mktime(&tmptime);
			offset = ret3-ret1;
			if(offset<0) offset=0;
			pos[i]=offset;
		}
		currs=startrs;
		YcRevertData[curid][0] =  yc_data[curid].value[currs];
		for(j=1;j<total_count;j++)
		{
			YcRevertData[curid][j] = YcRevertData[curid][j-1];		
			if( currs < rscount-1 )
			{	
				if( j >= pos[currs+1] )
				{
					YcRevertData[curid][j] = yc_data[curid].value[currs+1];
					currs++;
				}
			}
		}

        yc_data[curid].rscount = j;                
	}/* end for */ 

	for(curid=0;curid<p_rs->yxnum;curid++)
	{
		currs=0;
		j=0;
		i=0;
		tmptime=endtime;
		curtime=starttime;
		rscount=yx_data[curid].rscount;
		if(rscount<=0) continue;
		while((ret1=mktime(&curtime))<=(ret2=mktime(&endtime)) && j<MAX_REVERT_REC)
		{		
			for(i=currs;i<rscount;i++)
			{
				StrToTime(yx_data[curid].timestr[i],&tmptime,interval,0);
				if((ret1=mktime(&curtime))<(ret2=mktime(&tmptime)) )
				{
					if(i>=1)
					{
						YxRevertData[curid][j]=yx_data[curid].status[i-1];
					}
					else if(i<1 && j>=1)
					{
						YxRevertData[curid][j]=YxRevertData[curid][j-1];
					}					
					else if(i<1 && j<1)
					{
						/* if first rec time in hdb max than starttime,then data 
						   before first rec be the first data */
						YxRevertData[curid][j]=yx_data[curid].status[0];
					}
					break;
				}
				else if((ret1=mktime(&curtime))==(ret2=mktime(&tmptime)) )
				{
					YxRevertData[curid][j]=yx_data[curid].status[i];
					i++;
					break;						
				}
			}
			
			currs=i;
			
			/*if curtime > all time in hdb record then break  */
			if(i >= rscount)
			{
				YxRevertData[curid][j]=yx_data[curid].status[i-1];
				DateAdd(&curtime,interval);
				j++;
				continue;	
			}
			/*
			else if( i== (rscount-1) )
			{
				DateAdd(&curtime,interval);
				j++;
				break;				
			}
			*/
			DateAdd(&curtime,interval);
			j++;
		}/* end while */
		yx_data[curid].rscount = j;
	}/* end for */        
        
	for(curid=0;curid<p_rs->ycnum;curid++)
	{
		strcpy(p_rs->yc_data[curid].logicalname,yc_data[curid].logicalname);
		p_rs->yc_data[curid].rscount = yc_data[curid].rscount;
		for(i=0;i<yc_data[curid].rscount;i++)
		{
			yc_data[curid].value[i] = YcRevertData[curid][i];
			p_rs->yc_data[curid].value[i] = yc_data[curid].value[i];
		}
	} 

	for(curid=0;curid<p_rs->yxnum;curid++)
	{
		strcpy(p_rs->yx_data[curid].logicalname,yx_data[curid].logicalname);
		p_rs->yx_data[curid].rscount = yx_data[curid].rscount;		
		for(i=0;i<yx_data[curid].rscount;i++)
		{
			yx_data[curid].status[i] = YxRevertData[curid][i];
			p_rs->yx_data[curid].status[i] = yx_data[curid].status[i];
		}   
	}          

	printf("========== inter revert data completed ==========\n");
	return TRUE;
}

/***************** update data got from hdb  to share memories **************************/
int WriteRevertData()
{
 	unsigned short curid=0;
 	int rscount=0;
	float speed;
	int lastpos, offset;
	int first=TRUE;
	DATE_TIME curtime;
	unsigned char last_sec;
	BOOL print_pause_flag;

	GetSysTime(&curtime);
	last_sec = curtime.second;

	speed =(float) shm_hdb_addr->data_revert.speed / 8;
	
	lastpos=-1;
	offset=0;
	init_flag = TRUE;
	print_pause_flag = TRUE;
	shm_hdb_addr->data_revert.timeoffset = 0;
	printf("Start to WriteRevertData()\n");

	//yc 0, yx 64
	printf("Starttime: %s		endtime: %s\n",shm_hdb_addr->data_revert.starttime,shm_hdb_addr->data_revert.endtime);
	for(;;)
	{
		MySleep(50);
		GetSysTime(&curtime);
		if( shm_hdb_addr->data_revert.queryflag==1 || shm_hdb_addr->data_revert.queryflag==2 )
		{
			WriteInitData();
			printf("current revert stop\n");
			return FALSE;
		}
		if (shm_hdb_addr->data_revert.stopflag==1)
		{
			if (print_pause_flag)
			{
				printf("current revert pause......\n");
				print_pause_flag = FALSE;
			}
			continue;
		}
		offset = offset + shm_hdb_addr->data_revert.timeoffset;
		shm_hdb_addr->data_revert.timeoffset = 0;
		print_pause_flag = TRUE;
		if(curtime.second == last_sec)
		{
			continue;
		}
		last_sec = 	 curtime.second; 	
		for(curid=0;curid<pt_num_yc;curid++)
		{	
			//yyp debug
			//if(curid!=0) continue;
			if(offset>=yc_data[curid].rscount)
			{
				continue;
			}
			WrANA(yc_data[curid].logicalname,yc_data[curid].value[offset],yc_data[curid].status[offset]);
		}
		MySleep(20);
		for(curid=0;curid<pt_num_yx;curid++)
		{	
			//yyp debug
			//if(curid!=64) continue;
			if(offset>=yx_data[curid].rscount)
			{
				continue;
			}			
			WrIND(yx_data[curid].logicalname,yx_data[curid].status[offset]);
			//BdINDByName(yx_data[curid].logicalname,yx_data[curid].status[offset]);
		}
		strcpy(shm_hdb_addr->data_revert.curtime,RevertTime[offset]);
		printf("curtime =%s\n",RevertTime[offset]);
		offset++;
		if( offset>=total_count || offset>=MAX_REVERT_REC )
		{
			break;
		}
	}
	shm_hdb_addr->data_revert.queryflag=2;
	printf("Write revert data successfully!\n");

	return TRUE;	
}


int WriteAlarmShm()
{
	if(shm_hdb_addr->data_revert.queryflag==1)
	{
  		shm_hdb_addr->data_al.queryflag=1;
  		shm_hdb_addr->data_al.querytype=2;
		shm_hdb_addr->data_al.lockflag=LOCK_FLAG  ;    /* IDLE_FLAG */
		strcpy(shm_hdb_addr->data_al.starttime,shm_hdb_addr->data_revert.starttime);
		strcpy(shm_hdb_addr->data_al.endtime,shm_hdb_addr->data_revert.endtime);
		shm_hdb_addr->data_al.lockflag= IDLE_FLAG;	
	}
	printf("\nRevertScan Write alm shared memeries successfully!\n");
	return TRUE;
}


int GetPtFromDev(unsigned char stn_id,unsigned char dev_id)
{
	int i=0,j=0;
	int		DataType,	PointId,	Number;
	char		lname[100];
	POINTER		point;
	POINT_NAME	pt_name;	
 	int 		Start,End;
	unsigned int id;
	pt_num_yc = 0;
	pt_num_yx = 0;
 	if( dev_id == 0 )
	{
		Start = 1;
 		End = g_dev_num[stn_id];
	}
	else
	{
		Start = dev_id;
 		End = dev_id;
	}
	
	for(dev_id=Start;dev_id<=End;dev_id++)
	{
		for(DataType=1;DataType<=MAX_DATATYPE_NUM;DataType++)
		{								
			if( DataType==ANA_TYPE || DataType==PARAM_TYPE || DataType==IMP_TYPE || DataType==SOE_TYPE || 
				DataType==POL_TYPE || DataType==SYSMSG_TYPE || DataType==DEVOBJ_TYPE || DataType==OUTS_TYPE )
			{	
				point.stn_id	  =	stn_id;	 		
				point.dev_id      =	dev_id;
				point.data_type   =	DataType;
				point.pt_id    =	0;
				Number = GetPointNumber ( &point );
				for(PointId=0;PointId<Number;PointId++)
				{
					point.stn_id	  =	stn_id;
					point.dev_id      =	dev_id;
					point.data_type   =	DataType;
					point.pt_id    =	PointId;

					if(GetPtNameById( &point, lname, &pt_name ) ==-1)
					{
						continue;
					}
					if(GetHdbIDByName(&id,lname)==FALSE)
					{
						sprintf(log_str,"\nGetHdbIDByName( %s ) error",lname);
						Err_log(log_str);
						continue;
					}
					if( DataType==ANA_TYPE || DataType==PARAM_TYPE || DataType==IMP_TYPE ) 
					{
						strcpy(yc_data[i].logicalname,lname); 
						yc_data[i].id = id;
						if( (++i)<MAX_REVERT_NUM ) continue; 
							else return FALSE;
					}
					else
					{
						strcpy(yx_data[j].logicalname,lname); 
						yx_data[j].id = id;
						if( (++j)<MAX_REVERT_NUM ) continue; 
							else return FALSE;
					}
				}
			}
		}
	}
	pt_num_yc = i;
	pt_num_yx = j;
	printf("get revert point, yx=%d,yc=%d\n",pt_num_yx,pt_num_yc);
	return TRUE;
}

/*********************** get the revert shm **********************************************/
int InitRevert(struct hdb_revert *p_rs)
{
	int i;
	POINTER pt;
	p_rs->queryflag=shm_hdb_addr->data_revert.queryflag;
	p_rs->querytype=shm_hdb_addr->data_revert.querytype;
	p_rs->lockflag=shm_hdb_addr->data_revert.lockflag;
	//added by yyp 2010-11-01
	for(i=0;i<shm_ots_addr->data_point.ycnum;i++)
	{
		strcpy(yc_data[i].logicalname,shm_ots_addr->data_point.ycname[i]);
		strcpy(shm_hdb_addr->data_revert.yc_data[i].logicalname,yc_data[i].logicalname);
	}
	for(i=0;i<shm_ots_addr->data_point.yxnum;i++)
	{
		strcpy(yx_data[i].logicalname,shm_ots_addr->data_point.yxname[i]);
		strcpy(shm_hdb_addr->data_revert.yx_data[i].logicalname,yx_data[i].logicalname);
	}
	pt_num_yc= shm_ots_addr->data_point.ycnum;
	pt_num_yx = shm_ots_addr->data_point.yxnum;
	
	for(i=0;i<pt_num_yc;i++)
	{

		if(GetPtIdByNameStr( yc_data[i].logicalname, &pt )==-1) return FALSE;	
		yc_data[i].point= pt;
		GetHdbIDByName(&(yc_data[i].id),yc_data[i].logicalname);
	}
	for(i=0;i<pt_num_yx;i++)
	{
		
		if(GetPtIdByNameStr( yx_data[i].logicalname, &pt )==-1) return FALSE;	
		yx_data[i].point= pt;
		GetHdbIDByName(&(yx_data[i].id),yx_data[i].logicalname);
	} 
	

	return TRUE;
}

void GetInitData()
{
	int		i,n;
	DB_DATA	db_data_yx,db_data_yc;
	IND_DATA status;
	time_t now;
	struct tm *datetime;	
	char TimeStr[22];
	time(&now);
	datetime=localtime(&now);	
	tmtostring(*datetime,TimeStr);	
	printf("\n==== start GetInitData YxNum=%d  YcNum=%d\n",pt_num_yx,pt_num_yc);
	for(i=0;i<pt_num_yx;i++)
	{
		strcpy(yx_init_data[i].LogiName , yx_data[i].logicalname);
		if(ReadDBByName( yx_init_data[i].LogiName, &db_data_yx )== -1)
		{	
			sprintf(log_str,"%s invalid!",yx_data[i].logicalname);
			Err_log(log_str);
			continue;
		}
// 		strcpy(yx_init_data[i].basetime,TimeStr);
// 		if(db_data_yx.year<=1) 
// 		{
// 			db_data_yx.year = datetime->tm_year+1900;
// 		}			
// 		sprintf(yx_init_data[i].TimeTag,"%04d-%02d-%02d %02d:%02d:%02d",
// 			db_data_yx.year,db_data_yx.month,db_data_yx.day,
// 			db_data_yx.hour,db_data_yx.minute,db_data_yx.second );
		memcpy(&yx_init_data[i].status,&db_data_yx.status,2);
		//yx_init_data[i].status.state = db_data_yx.status;
	}
	for(i=0;i<pt_num_yc;i++)
	{
		strcpy(yc_init_data[i].LogiName , yc_data[i].logicalname);
		if(ReadDBByName( yc_init_data[i].LogiName, &db_data_yc )==-1)
		{	
			sprintf(log_str,"%s invalid!",yc_data[i].logicalname);
			Err_log(log_str);
			continue;
		}
		yc_init_data[i].value	=	db_data_yc.fvalue;
		yc_init_data[i].status	=	db_data_yc.status;
	}	
}
int WriteInitData()
{
	unsigned short curid=0;
	short status;
	init_flag = FALSE;
	printf("Start to WriteInitData\n");
#ifdef DMDEBUG
	for (curid=0;curid<pt_num_yx;curid++)
	{
		printf("LogiName= %s    status= %d\n",yx_init_data[curid].LogiName,yx_init_data[curid].status.state);
	}
#endif
	for(curid=0;curid<pt_num_yc;curid++)
	{	
		WrANA(yc_init_data[curid].LogiName,yc_init_data[curid].value,yc_init_data[curid].status);
	}
	for(curid=0;curid<pt_num_yx;curid++)
	{			
		memcpy(&status,&yx_init_data[curid].status,2);
		WrIND(yx_init_data[curid].LogiName,status);
	}
	printf("Write init data successfully!\n");
	
	return TRUE;	
}