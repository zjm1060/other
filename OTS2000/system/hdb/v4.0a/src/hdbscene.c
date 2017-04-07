/********************************************************************************************************
FILENAME:	hdbscene.c

FUNCTION:	
	


VERSION:	v4.0a

OPERATE SYSTEM:	window 2000, Unix

HISTORY:

2009.8.19	create this file for ots		yyp

*********************************************************************************************************/
#include "../inc/hdbhead_all.h"



struct yc_seg yc_all[MAX_YC_NUM];
struct yx_seg yx_all[MAX_YX_NUM];

int HostNewStat=0;

int	YcNum,	YxNum,	ScDataNum;
char LoginStr[50];
INFO_RECORD		*shm_info_rec_addr; 

int HdbConn_ctx = FALSE;
int GetStatId(int id);		
void GetYcDataScene(int HostStat);
void UpdateYcInHDBScene();
void UpdateYxInHDBScene();
void GetYxDataScene(int HostStat);
int InsertSceneHdb(int type,char *timestr);
int DoQuerySceneList();
int DoQuerySceneData(int scene_id);
int DoDelScene(int scene_id);
int DoWriteSceneFromReal();
int DoWriteSceneFromHdb( char *timestr );

/********************************************************************************************************
 main function
*********************************************************************************************************/

void main(int argc, char *argv[])
{	
	int	i;	
 	char hostname[20];
	//wsa_startup();
	//SetLogLevel(argc, argv);
#ifdef SUN
	signal(SIGKILL,sig_termp); 
	signal(SIGTERM,sig_termp);
	signal(SIGINT,sig_termp);
	signal(SIGPIPE,SIG_IGN);
#endif
	gethostname(hostname,20); 
	app_id = HDB_APP;
  	ReadDefFile();
	sprintf(strPwd,"%s/%s@",strUsername,strPassword);
 	strcpy(LoginID,strPwd);
 	strcat(LoginID,SIDSTR);
	strcpy(LoginStr,LoginID);
	if(MountDB(DB_NAME) == -1)
	{
		printf("==== hdbscene.c: call MountDB(DATABASE_NAME) error\n");		
		exit(0);
	}
	if(lan_init_all() == FALSE)
	{
		printf("==== hdbscene.c: call Lan_init_all() error\n");		
		exit(0);
	}
	/* added by yyp 07.05.29 */
	if(strcmp(OUTNET_HOST,hostname)==0)
	{
		g_wranyhost = TRUE;
	}

	if(GetPtConf()==FALSE)	
	{
		printf("\n==== hdbscene.c: GetPtConf() error!\n");
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
	
	GenInfoTable();

	HdbConn=FALSE;

	/******** clear the shm  **********/
	
	shm_hdb_addr->data_scene.queryflag=0;
	shm_hdb_addr->data_scene.scenenum=0;
	shm_hdb_addr->data_scene.scene_id=0;
	shm_hdb_addr->data_scene.querytype=0;

 	for(;;)
	{	
		MySleep(200);
		
		if( HdbConn==FALSE )
		{	
			sprintf(log_str,"\n==== Connect '%s' HdbConn =%d ==== \n",
				SIDSTR,HdbConn);
			log_write(INF,1,log_str);
			
			//DisconnectHDB(&lda_rw,&cda_rw);

			MySleep(2000);
			HdbConn=FALSE;
			if( ConnectHDB(&lda_rw,&cda_rw,LoginStr)!=TRUE )
			{ 
				sprintf(log_str,"\n==== hdbscene Connect '%s' failed ==== \n",SIDSTR);
				log_write(ERR,1,log_str);
				MySleep(60000); 
				continue; 
			}
			else
			{
				sprintf(log_str,"\n==== hdbscene Connect '%s' Success ==== \n",SIDSTR);
				log_write(INF,1,log_str);	
				
				HdbConn=TRUE;
				if(SelectIdFromHdb(cda_rw)== FALSE)
				{
					err_write("==== hdbscene: SelectIdFromHdb() failed!");
					exit(0);
				}
				else
				{
					
				}

				for(i=0;i<YcNum;i++)
				{
					yc[i].id = -1;
					if(GetHdbIDByName((unsigned int *)&(yc[i].id),yc[i].LogiName)==FALSE)
					{
						sprintf(log_str,"\nGetHdbIDByName( %s ) error",yc[i].LogiName);
						err_write(log_str);
						continue;
					}
				}

				for(i=0;i<YxNum;i++)
				{
					yx[i].id = -1;
					if(GetHdbIDByName((unsigned int *)&(yx[i].id),yx[i].LogiName)==FALSE)
					{
						sprintf(log_str,"\nGetHdbIDByName( %s ) error",yx[i].LogiName);
						err_write(log_str);
						continue;
					}
				}
			}
			printf("\n==== waitting for new scene cmd...... \n");
#ifdef SUN
			signal(SIGINT,sig_termp);
#endif
		}

		if(shm_hdb_addr->data_scene.queryflag==1)
		{		
			if(shm_hdb_addr->data_scene.querytype==0)
			{
				DoQuerySceneList();
				shm_hdb_addr->data_scene.queryflag=0;
			}
			else if(shm_hdb_addr->data_scene.querytype==1)
			{
				DoQuerySceneData(shm_hdb_addr->data_scene.scene_id);
				shm_hdb_addr->data_scene.queryflag=0;
			}
			else if(shm_hdb_addr->data_scene.querytype==2)
			{
				DoWriteSceneFromReal();
				shm_hdb_addr->data_scene.queryflag=0;
			}
			else if(shm_hdb_addr->data_scene.querytype==3)
			{
				DoWriteSceneFromHdb(shm_hdb_addr->data_scene.scene_time);
				shm_hdb_addr->data_scene.queryflag=0;
			}
			else if(shm_hdb_addr->data_scene.querytype==4)
			{
				DoDelScene(shm_hdb_addr->data_scene.scene_id);
				shm_hdb_addr->data_scene.queryflag=0;
			}
			shm_hdb_addr->data_scene.queryflag=0;			
			printf("\n==== waitting for new scene cmd...... \n");
			continue;
		}
		else
		{
			continue;
				
		}

		//debug yyp
		for(;;)
		{
			MySleep(2000);
		}

	} //end for
}



int DoQuerySceneList()
{
	time_t now;
	struct tm *datetime, *datetime2;
	char	TimeStr[30];
	unsigned int scenenum;				/* when querytype = 0 */
	int result;
	struct hdb_scene_seg scene_data[MAX_HDB_SCENE];	
	time(&now);
	datetime=localtime(&now);
	tmtostring(*datetime,TimeStr);
	sprintf(log_str,"==== DoQuerySceneList: starttime: %s",TimeStr);
	log_write(INF,1,log_str);
	
	//to be added	
	scenenum=0;
	result = SelectSceneListFromHdb(cda_rw,&scenenum,scene_data);
	if( result == FALSE )
	{
		printf("SelectSceneListFromHdb !\n");
		HdbConn=FALSE;
	}
	else
	{
		shm_hdb_addr->data_scene.scenenum = scenenum;
		memcpy(shm_hdb_addr->data_scene.scene_data,scene_data,sizeof(struct hdb_scene_seg) * scenenum );
	}
	time(&now);
	datetime2=localtime(&now);
	tmtostring(*datetime2,TimeStr);
	sprintf(log_str,"==== DoQuerySceneList: endtime:  %s",TimeStr);		
	log_write(INF,1,log_str);
	sprintf(log_str,"==== End DoQuerySceneList() num=%d \n",scenenum);
	log_write(INF,1,log_str);
	return TRUE;
}

int DoQuerySceneData(int scene_id)
{
	time_t now;
	struct tm *datetime,*datetime2;
	char TimeStr[30];
	int i,result;
	int type;
	POINTER		point;
	WR_VAR_UNION	new_value;
	ENTRY_DEVOBJ_VAL obj_value;
	IOST		iost;
	DATE_TIME	updatetime;
	GetSysTime(&updatetime);
	time(&now);
	datetime=localtime(&now);
	tmtostring(*datetime,TimeStr);
	sprintf(log_str,"==== DoQuerySceneData: starttime: %s",TimeStr);
	log_write(INF,1,log_str);
	
	//to be added
	//to be added	
	ScDataNum=0;

	result = SelectSceneDataFromHdb(cda_rw,scene_id,&ScDataNum,scdata);		
	if( result == FALSE )
	{	
		printf("SelectSceneDataFromHdb !\n");
		HdbConn=FALSE;
		return FALSE;
	}
	else
	{
		for(i=0;i<ScDataNum;i++)
		{
			if(GetPtIdByNameStr( scdata[i].LogiName, &point )==-1)
			{
				continue;
			}
			type = point.data_type;
			if(type == ANA_TYPE || type == OUTA_TYPE || type == PARAM_TYPE || type == CALC_TYPE || type == IMP_TYPE)
			{
				new_value.anlg.status.chan_fail		=0;
				new_value.anlg.status.ctrl_block	=0;
				new_value.anlg.status.alm_type		=0;
				new_value.anlg.status.trend_state	=0;
				new_value.anlg.status.lmt_state		=0;				
				switch	( type )
				{
				case	ANA_TYPE:
				case	OUTA_TYPE:
				case	PARAM_TYPE:
					new_value.anlg.data = (int)  (1000  * scdata[i].value) ;
					
					SetPtValueIostById( &point, new_value, iost, updatetime, RW_RT_VALUE|RW_UPDATE_TIME );
					break;
					
				case	CALC_TYPE:
					new_value.calc.data= (int)scdata[i].value;
					SetPtValueIostById( &point, new_value, iost, updatetime, RW_RT_VALUE|RW_UPDATE_TIME );
					break;
					
				case	IMP_TYPE:
					new_value.imp.status.option= 0;
					new_value.imp.iValue= (int)scdata[i].value ; 
					SetPtValueIostById( &point, new_value, iost, updatetime, RW_RT_VALUE|RW_UPDATE_TIME);
					break;
					
				default:
					break;
				}
			}
			else
			{
				switch	( type )
				{
				case	SOE_TYPE:
				case	POL_TYPE:
				case	OUTS_TYPE:
				case	SYSMSG_TYPE:					
					new_value.ind.rt_state= scdata[i].status  ;  
					SetPtValueIostById( &point, new_value, iost, updatetime, RW_RT_VALUE|RW_UPDATE_TIME );											
					break;
				case     DEVOBJ_TYPE:
					memcpy ( &obj_value.fb_status1 ,  & (scdata[i].status), sizeof(FB_STATUS1)) ;   //shm_realdb_addr->yx_data[i].status[j];
					SetDevObjPtVarById( &point, obj_value );
					break;                                    
				}
			}
		}
	}	
	time(&now);
	datetime2=localtime(&now);
	tmtostring(*datetime2,TimeStr);
	sprintf(log_str,"==== DoQuerySceneData: endtime: %s, num=%d ",TimeStr,ScDataNum);		 
	log_write(INF,1,log_str);
	return TRUE;
}

int DoDelScene(int sceneId)
{
	time_t now;
	struct tm *datetime, *datetime2;
	char	TimeStr[30];
	unsigned int scenenum;				/* when querytype = 0 */
	int result;
	time(&now);
	datetime=localtime(&now);
	tmtostring(*datetime,TimeStr);
	sprintf(log_str,"==== DoDelScene: starttime: %s",TimeStr);
	log_write(INF,1,log_str);
	
	//to be added	
	scenenum=0;
	result = DelSceneFromHDB(cda_rw,sceneId);
	if( result == FALSE )
	{
		printf("DelSceneFromHdb !\n");
		HdbConn=FALSE;
		return FALSE;
	}
	time(&now);
	datetime2=localtime(&now);
	tmtostring(*datetime2,TimeStr);
	sprintf(log_str,"==== DoDelScene: endtime:  %s",TimeStr);
	log_write(INF,1,log_str);
	return TRUE;
}


int DoWriteSceneFromReal()
{
	int scene_id;
	char tmpstr[30];
	GetYxDataScene( HostNewStat );
	GetYcDataScene( HostNewStat );
	scene_id = -1;
	scene_id = InsertSceneHdb(0,tmpstr);
	if( scene_id <= 0 )
	{
		return FALSE;
	}
	UpdateYxInHDBScene(scene_id);
	UpdateYcInHDBScene(scene_id);
	return TRUE;
}

int DoWriteSceneFromHdb( char *timestr )
{
	int i,scene_id;
	time_t now;
	struct tm *datetime, *datetime2;
	char	TimeStr[30];
	time(&now);
	datetime=localtime(&now);
	tmtostring(*datetime,TimeStr);
	sprintf(log_str,"==== DoWriteSceneFromHdb: starttime: %s",TimeStr);
	log_write(INF,1,log_str);
	
	//to be added	
	scene_id = -1;
	scene_id = InsertSceneHdb(1, timestr);
	if( scene_id <= 0 )
	{
		return FALSE;
	}
	for(i=0;i<YcNum;i++)
	{	
		if( yc[i].id<=0 ) 
		{
			continue;
		}
		if(InsertOneYcSceneFromHDB(cda_rw,yc[i].LogiName, timestr, scene_id) !=TRUE)
		{
			HdbConn=FALSE; 	
			return FALSE;
		}
		MySleep(2);	
	}

	for(i=0;i<YxNum;i++)
	{	
		if( yx[i].id<=0 ) 
		{
			continue;
		}
		if(InsertOneYxSceneFromHDB(cda_rw,yx[i].LogiName, timestr, scene_id) !=TRUE)
		{
			HdbConn=FALSE; 	
			return FALSE;
		}
		MySleep(2);	
	}

	time(&now);
	datetime2=localtime(&now);
	tmtostring(*datetime2,TimeStr);
	sprintf(log_str,"==== DoWriteSceneFromHdb: endtime:  %s",TimeStr);		
	log_write(INF,1,log_str);
	return TRUE;
}

int InsertSceneHdb(int type, char *timestr)
{
	time_t now;
	int scene_id;
	struct tm *datetime;
	char	TimeStr[30];
	time(&now);
	scene_id = (int) now;
	datetime=localtime(&now);

	printf("==== Start InsertSceneHdb()... \n");
	tmtostring(*datetime,TimeStr);
	sprintf(log_str,"==== InsertSceneHdb: datetime: %s",TimeStr);
	log_write(INF,1,log_str);
	if(type==0)
	{
		strcpy(timestr,TimeStr);
	}
	if(InsertHdbByScene( cda_rw, scene_id, timestr, type,TimeStr ) !=TRUE)
	{
		HdbConn=FALSE; 	
		return FALSE;
	}
	return scene_id;
}


/********************************************************************************************************
 Update Yc changed point to HDB
*********************************************************************************************************/
void UpdateYcInHDBScene(int scene_id)
{	
	time_t now;
	struct tm *datetime, *datetime2;
	int	i,	n=0;
	char	TimeStr[30];
	time(&now);
	datetime=localtime(&now);
	
	printf("==== Start UpdateYcInHDB()... \n");
	tmtostring(*datetime,TimeStr);
	sprintf(log_str,"==== UpdateYcInHDB:starttime: %s",TimeStr);
	log_write(INF,1,log_str);
	
	for(i=0;i<YcNum;i++)
	{	
		yc_all[n].id = yc[i].id;
		strcpy(yc_all[n].TimeTag,yc[i].TimeTag);
		yc_all[n].value = yc[i].value;
		yc_all[n].status = yc[i].status;
		n++;
		if( yc[i].id<=0 ) 
		{
			continue;
		}
		if(InsertHdbBySceneData( cda_rw, scene_id, yc[i].id, yc[i].value, yc[i].status ) !=TRUE)
		{
			HdbConn=FALSE; 	
			return;
		}
		yc[i].ChgFlag = 0;
		MySleep(2);	
	}
	time(&now);
	datetime2=localtime(&now);
	tmtostring(*datetime2,TimeStr);
	sprintf(log_str,"==== UpdateYcInHDB: endtime:  %s",TimeStr);		
	log_write(INF,1,log_str);
	printf("==== End UpdateYcInHDB(), ChgYcNum=%d\n",n);
	
}

/********************************************************************************************************
 Update Yx changed point to HDB
*********************************************************************************************************/

void UpdateYxInHDBScene(int scene_id)
{ 	
	time_t now;
	struct tm *datetime, *datetime2;	
	int	i,	n=0;
	char	TimeStr[30];
	unsigned short state;
	time(&now);
	datetime=localtime(&now);	
	
	printf("==== Start UpdateYxInHDBScene()... \n");
	tmtostring(*datetime,TimeStr);
	sprintf(log_str,"==== UpdateYxInHDBScene: starttime: %s",TimeStr);
	log_write(INF,1,log_str);	
	for(i=0;i<YxNum;i++)
	{	
		/* modified by yyp 07.07.02 */
		if(yx[i].data_type != DEVOBJ_TYPE)
		{
			state = yx[i].status.state;
		}
		else
		{
			/* modified by yyp 07.07.05 */
			GetObjPtStateByNameStr( yx[i].LogiName, &state );					
			/*
			memcpy(&obj_state,&(yx[i].status),2);
			state = obj_state.p0;
			*/
		}
		if( yx[i].id<=0 ) 
		{
			continue;
		}
		if(InsertHdbBySceneData( cda_rw, scene_id, yx[i].id, 0, state ) != TRUE)
		{
			HdbConn=FALSE;
			return;
		}
		yx_all[n].id = yx[i].id;
		strcpy(yx_all[n].TimeTag,yx[i].TimeTag);
		yx_all[n].fvalue = (float) state;
		yx_all[n].quality = yx[i].quality;
		n++;
		MySleep(2);
		yx[i].ChgFlag = 0;

	}

	time(&now);
	datetime2=localtime(&now);
	tmtostring(*datetime2,TimeStr);
	sprintf(log_str,"==== UpdateYxInHDB: endtime:  %s",TimeStr);		
	log_write(INF,1,log_str);
	sprintf(log_str,"==== End UpdateYxInHDB(), ChgYxNum=%d \n",n);
	log_write(INF,1,log_str);

}

/********************************************************************************************************
 Get Yc Changed Point data from real db
*********************************************************************************************************/

void GetYcDataScene(int HostStat)
{ 
	int		i;
  	DB_DATA		db_data;
  	float		off_val;
	time_t now;
	struct tm *datetime;
	printf("\n==== start GetYcData() YcNum=%d\n",YcNum);

	time(&now);
	datetime=localtime(&now);	
	for(i=0;i<YcNum;i++)
	{
		yc[i].ChgFlag	=	0;
		if(ReadDBByName( yc[i].LogiName, &db_data )==-1)
		{	
			sprintf(log_str,"%s invalid!",yc[i].LogiName);
			err_write(log_str);
		}
		off_val = db_data.fvalue - yc[i].value	;	
		if( fabs(off_val)>yc[i].offset || db_data.status!=yc[i].status )
		{	
			sprintf(log_str,"%s data changed!",yc[i].LogiName);
			log_write(INF,3, log_str);
			if(db_data.year<=1) 
			{
				db_data.year = datetime->tm_year+1900;
			}
			sprintf(yc[i].TimeTag,"%04d-%02d-%02d %02d:%02d:%02d",
				db_data.year,db_data.month,db_data.day,
				db_data.hour,db_data.minute,db_data.second );

			yc[i].value	=	db_data.fvalue;
			yc[i].status	=	db_data.status ;		
			yc[i].ChgFlag	=	1;
		}

	}	
}


/********************************************************************************************************
 Get Yx Changed Point data from real db
*********************************************************************************************************/

void GetYxDataScene(int HostStat)
{ 
	int		i,n;
  	DB_DATA	db_data;
  	IND_DATA status;
  	unsigned short state1,state2;
	time_t now;
	struct tm *datetime;	
	char TimeStr[22];
	time(&now);
	datetime=localtime(&now);	
	tmtostring(*datetime,TimeStr);	
	printf("\n==== start GetYxData() YxNum=%d\n",YxNum);
	for(i=0;i<YxNum;i++)
	{	
		yx[i].ChgFlag	= 0;
		if(ReadDBByName( yx[i].LogiName, &db_data )== FALSE)
		{
			yx[i].IsValid = FALSE;
		}
		else
		{
			yx[i].IsValid = TRUE;
		}
		memcpy(&status,&db_data.status,2);
		strcpy(yx[i].ReadTime,TimeStr);
		
		/* modified by yyp 07.07.03 */
		memcpy(&state1,&status,2);
		memcpy(&state2,&(yx[i].status),2);
		if(db_data.year<=1) 
		{
			db_data.year = datetime->tm_year+1900;
		}			
		sprintf(yx[i].TimeTag,"%04d-%02d-%02d %02d:%02d:%02d",
			db_data.year,db_data.month,db_data.day,
			db_data.hour,db_data.minute,db_data.second );
		if( state1 != state2 )
		{	

			if(yx[i].status.state != status.state)
			{
				if( ( n = GetStatId(yx[i].id) ) != -1 ) 
				{
					if( status.state < 16)
					{
						yxstat[n].oldstatus =  yxstat[n].curstatus;
						yxstat[n].curstatus = status.state;
						strcpy(yxstat[n].lasttime,yxstat[n].starttime);
						strcpy(yxstat[n].starttime,yx[i].TimeTag);
						yxstat[n].ischg = 1;
					}
				}
			}
			yx[i].status    = status;
			yx[i].quality    = (unsigned char)db_data.quality;			
			yx[i].ms	= db_data.ms;

			yx[i].ChgFlag	= 1;

		}
	}
}

/********************************************************************************************************
 Write Point Info to file
*********************************************************************************************************/

void GenInfoTable()
{	
	FILE	*fp;
	int	i;
#ifdef DEBUG
	printf("\n==== start GenInfoTable()\n");
#endif	
	YcNum = YxNum =0;
	for(i=0;i<MAX_YC_NUM;i++) strcpy(yc[i].LogiName,"");
	for(i=0;i<MAX_YX_NUM;i++) strcpy(yx[i].LogiName,"");
	if( (fp=fopen(YC_TABLE, "w"))==NULL )
	{	printf("can't open  %s  !!!\n",YC_TABLE); exit(-1);  }    

	for(i=1;i<=g_stn_num;i++)
	{
		if( g_cascade != YES )
		{
			if(i>1) break;
		}
		if( g_dev_num[i]!=0 )
		{
			printf("\n==== YC Station %d  (%d--%d) \n", i,1, g_dev_num[i]);
			
			if( CreatYcInfo( fp, (unsigned char)i )==-1 ) 
				break;	
		}
	}
	fclose( fp );
	printf("\n==== YcNum= %d\n",YcNum);

	if( (fp=fopen(YX_TABLE, "w"))==NULL )
	{	printf("can't open  %s  !!!\n",YX_TABLE); exit(-1);   }    

	for(i=1;i<=g_stn_num;i++)
	{
		if( g_cascade != YES )
		{
			if(i>1) break;
		}	
		if( g_dev_num[i]!=0 )
		{
			printf("\n==== YX Station %d (%d--%d) \n",i,1, g_dev_num[i]);
			
			if( CreatYxInfo( fp, (unsigned char)i )==-1 )
				break;
		}
	}
	fclose( fp );
	printf("\n==== YxNum=%d\n",YxNum);
	
}

/********************************************************************************************************
 create Yc point def file
*********************************************************************************************************/

int CreatYcInfo( FILE *fp, unsigned char stnid )
{	
	int			DevId,	DataType,	PointId,	Number;
	char			cname[100],	lname[100];
	POINTER			point;
	POINT_NAME		pt_name;
 	DB_DATA			db_data;
 	struct tm		datetime;
 	long			t;
 	int 			Start,End;
 	Start = 1;
 	End = g_dev_num[stnid];
 	t=time(NULL); datetime=*localtime((long *)&t);
	for(DevId=Start;DevId<=End;DevId++)
	{		
#ifndef MULTIDEV
		if(DevId>1) break;
#endif	
		for(DataType=1;DataType<=MAX_DATATYPE_NUM;DataType++)
		{								
			if( DataType==ANA_TYPE || DataType==PARAM_TYPE || DataType==IMP_TYPE)
			{	
				point.stn_id	  =	stnid;	
				point.dev_id      =	DevId;
				point.data_type   =	DataType;
				point.pt_id    =	0;
				Number = GetPointNumber ( &point );
				for(PointId=0;PointId<Number;PointId++)
				{
					point.stn_id	  =	stnid;
					point.dev_id      =	DevId;
					point.data_type   =	DataType;
					point.pt_id    =	PointId;
					if(IsPtIntoHdb(&point) != TRUE) 
					{
						continue;
					}
					if(GetPtNameById( &point, lname, &pt_name ) ==-1)
					{
						continue;
					}
					GetPtLongnameById( &point, cname);  
				
					strcpy(yc[YcNum].LogiName,lname);  
					if( ReadDBByName( lname, &db_data )!=TRUE )
					{
						printf("No %s in RDB\n",lname);					
						continue;
					}
						
					yc[YcNum].status	=	db_data.status;
					yc[YcNum].HI_limit	=	db_data.HI_limit;
					yc[YcNum].value		=	db_data.fvalue;
					yc[YcNum].ioattr	=	db_data.ioattr;
					yc[YcNum].ChgFlag	=	1;
					
					fprintf( fp, "%4d  %30s  %40s  \n", YcNum, yc[YcNum].LogiName, cname);
										
  					sprintf(yc[YcNum].TimeTag,"%04d-%02d-%02d %02d:%02d:%02d",
  						datetime.tm_year+1900,datetime.tm_mon+1,datetime.tm_mday,
  						datetime.tm_hour,datetime.tm_min,datetime.tm_sec );
					if( (++YcNum)<MAX_YC_NUM ) continue; else return(-1); 
				}
			}
		}
	}
	return(TRUE);
}

/********************************************************************************************************
 create Yx point def file
*********************************************************************************************************/

int CreatYxInfo( FILE *fp, unsigned char stnid )
{	
	int		DevId,	DataType,	PointId,	Number;
	char			cname[100],	lname[100];
	POINTER		point;
	POINT_NAME	pt_name;	
 	DB_DATA		db_data;
   	IND_DATA	status;	
 	struct tm	datetime;
 	long		t; 	   
 	int 		Start,End;
 	Start = 1;
 	End = g_dev_num[stnid];
 	t=time(NULL); datetime=*localtime((long *)&t);
 	
	for(DevId=Start;DevId<=End;DevId++)
	{
#ifndef MULTIDEV
		if(DevId>1) break;
#endif			 
		for(DataType=1;DataType<=MAX_DATATYPE_NUM;DataType++)
		{								
			if( DataType==SOE_TYPE || DataType==POL_TYPE || DataType==SYSMSG_TYPE 
				|| DataType==DEVOBJ_TYPE || DataType==OUTS_TYPE)
			{	
				point.stn_id	  =	stnid;	 		
				point.dev_id      =	DevId;
				point.data_type   =	DataType;
				point.pt_id    =	0;
				Number = GetPointNumber ( &point );
				for(PointId=0;PointId<Number;PointId++)
				{
					point.stn_id	  =	stnid;
					point.dev_id      =	DevId;
					point.data_type   =	DataType;
					point.pt_id    =	PointId;

					if(IsPtIntoHdb(&point) != TRUE) 
					{
						continue;
					}					
					if(GetPtNameById( &point, lname, &pt_name ) ==-1)
					{
						continue;
					}
					GetPtLongnameById( &point, cname);    
					
					strcpy(yx[YxNum].LogiName,lname); 
										
					if( ReadDBByName( lname, &db_data )!=TRUE )
					{	
						printf("No %s in RDB\n",lname);	
						continue;				
					}
					
					memcpy(&status,&db_data.status,2);
					yx[YxNum].data_type =	DataType;
					yx[YxNum].quality	=	(unsigned char)db_data.quality;
					yx[YxNum].status	=	status;
					yx[YxNum].ioattr	=	db_data.ioattr;
					yx[YxNum].ChgFlag	=	1;
					
					fprintf( fp, "%4d  %30s  %40s  \n", YxNum, yx[YxNum].LogiName, cname);
					
  					sprintf(yx[YxNum].TimeTag,"%04d-%02d-%02d %02d:%02d:%02d",
  						datetime.tm_year+1900,datetime.tm_mon+1,datetime.tm_mday,
  						datetime.tm_hour,datetime.tm_min,datetime.tm_sec );

					if( (++YxNum)<MAX_YX_NUM ) continue; 
						else return(-1);
				}
			}
		}
	}
	return(0);
}


int GetStatId(int id)
{
	int i;
	for(i=0;i<StatNum;i++)
	{
		if( yxstat[i].id == id )
			return i;
	}
	return -1;
}